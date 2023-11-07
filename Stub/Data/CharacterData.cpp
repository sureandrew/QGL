//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/CharacterData.h"
//-- Library
#include "Stub/Data/AccountData.h"
#include "Stub/Data/SkillData.h"
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboCharacter.h"
#include "ObjectService/Global.h"
#include "ObjectService/Configure.h"
#include "Common/common_binary.h"
#include "ObjectService/DboSourcePool.h"
#include "Stub/Data/MsgCharacterData.h"
#include "Stub/Data/SiblingData.h"
#include "Stub/Data/MarriageData.h"
#include <Reuben/Math/Hex.h>

#define TOP_NUM				100

REGISTER_CLASSTYPE(CCharacterData, CharacterData_CLASSID);

struct CharRequest : public WorkRequest<UInt32>
{
	UInt32			char_id;
	StringVector	sqls;
	CharRequest(UInt32 id, LPCPEER pPeer, RPCContext &context)
		: WorkRequest<UInt32>(id, pPeer, context)
		, char_id(0)
	{}
};

CCharacterData* NewCharacter();
CdboCharacter* &pdbCharacter = gGlobal.gTables.pCharacter;
StringVector CdboCharacterAccessor::m_params;
ThreadKey CCharacterData::m_key;

////////////////////////////////////////////////////////////////////////
////						Character Data							////
////////////////////////////////////////////////////////////////////////
CCharacterData* NewCharacter()
{
	CCharacterData *pCharData = NULL;
	UInt32 acct_id = (UInt32) pdbCharacter->m_acct_id.LowPart;
	UInt32 char_id = (UInt32) pdbCharacter->m_char_id.LowPart;
	DataObject &dobj = gGlobal.charIdMap[char_id];
	DataObject &dobj2 = gGlobal.charNameMap[pdbCharacter->m_nickName];

	if (dobj.pobj == NULL)
	{
		pCharData = CCharacterData::NewInstance();
		pCharData->SetData(pdbCharacter);			// assign values to character data
		dobj.pobj = pCharData;
		dobj2 = dobj;
	}
	else
	{
		pCharData = CastNetObject<CCharacterData>(dobj.pobj);
	}

	return pCharData;
}

CCharacterData* FindChar(UInt32 char_id)
{
	DataIdMap::iterator iter = gGlobal.charIdMap.find(char_id);
	CCharacterData* pCharData = NULL;

	// found existing character in cache
	if (iter != gGlobal.charIdMap.end())
		pCharData = CastNetObject<CCharacterData>(iter->second.pobj);

	return pCharData;
}

CCharacterData* FindChar(const String &name)
{
	DataNameMap::iterator iter = gGlobal.charNameMap.find(name);
	CCharacterData* pCharData = NULL;

	// found existing character in cache
	if (iter != gGlobal.charNameMap.end())
		pCharData = CastNetObject<CCharacterData>(iter->second.pobj);

	return pCharData;
}

void ReleaseCharacter(UInt32 char_id)
{
	CCharacterData* pCharData = NULL;
	DataIdMap::iterator itfind = gGlobal.charIdMap.find(char_id);
	if (itfind != gGlobal.charIdMap.end())
	{
		pCharData = CastNetObject<CCharacterData>(itfind->second.pobj);
		gGlobal.DequeueUpdate(&itfind->second);		// remove from update queue
		gGlobal.charIdMap.erase(itfind);
	}

	if (pCharData)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("acct_id %d, char_id %d"), 
			pCharData->GetAcct_id(), pCharData->GetChar_id());

		CAccountData* pAcctData = FindAccount(pCharData->GetAcct_id());
		if (pAcctData)
			pAcctData->ReleaseCharId(char_id);

		gGlobal.charNameMap.erase(pCharData->GetNickName());
		pCharData->ReleaseBelong();	// release character belonging
		if (pCharData->IsUpdated())					// write to db if updated
			gGlobal.m_releaseGroup.Add(pCharData);
		else
			DeleteNetObject(pCharData);
	}
}

Boolean CharacterNameExist(const String& name)
{
	DataNameMap::iterator iter = gGlobal.charNameMap.find(name);
	// found existing character in cache
	if (iter != gGlobal.charNameMap.end())
		return BOOLEAN_TRUE;
	
	Boolean bFound = BOOLEAN_FALSE;
	if (DB_SUCCEEDED(pdbCharacter->RSNameByName(name.c_str())))
	{
		if (DB_SUCCEEDED(pdbCharacter->MoveFirst()))
			bFound = BOOLEAN_TRUE;
	}
	pdbCharacter->Close();
	if (DB_SUCCEEDED(pdbCharacter->InactiveName(name.c_str())))
	{
		if (DB_SUCCEEDED(pdbCharacter->MoveFirst()))
			bFound = BOOLEAN_TRUE;
	}
	pdbCharacter->Close();

	return bFound;	
}

DWORD WINAPI QueueGetCharacters(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();

	CharRequest* pRequest = (CharRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
	{
		pRequest->context.bCallback = BOOLEAN_FALSE;
		NetGroup charGroup;
		CdboCharacter* dboChar = SafeCreateObject(CdboCharacter);
		CCharacterData* pCharData;
		String s;

		charGroup.SetValidate(BOOLEAN_FALSE);
		dboChar->SetDataSource(dbSrc);
		SleepEx(10, TRUE);
		if (pRequest->char_id != 0)
			hr = dboChar->RSByID(pRequest->char_id);
		else if (pRequest->key != 0)
			hr = dboChar->RSByAccountID(pRequest->key);

		if (pRequest->sqls.empty())
		{
			if (DB_SUCCEEDED(hr))
			{	
				hr = dboChar->MoveFirst();
				while (hr == S_OK)
				{
                    pCharData = SafeCreateObject(CCharacterData);
					pCharData->SetData(dboChar);	
					pCharData->SetDetail(CCharacterData::DETAIL_ALL);
					charGroup.Add(pCharData);
					s.AppendFormat(_T("%d, "), pCharData->GetChar_id());
					hr = dboChar->MoveNext();
				}
			}
		}
		else
		{
			for (StringVector::iterator it = pRequest->sqls.begin();
				it != pRequest->sqls.end(); ++it)
			{
				if (DB_SUCCEEDED(hr = dboChar->RSTopCase(it->c_str(), TOP_NUM)))
				{
					hr = dboChar->MoveFirst();
					while (hr == S_OK)
					{
						pCharData = SafeCreateObject(CCharacterData);
						pCharData->SetData(dboChar);
						pCharData->SetDetail(CCharacterData::DETAIL_RANK);
						charGroup.Add(pCharData);
						hr = dboChar->MoveNext();
					}
				}
			}
		}

		gGlobal.CB_QueueNetGroup(_T("QueueGetCharacters"), hr, dboChar->GetErrorMsg(hr), 
			pRequest->context, &charGroup);
		dboChar->Close();
		SafeDeleteObject(dboChar);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, acct_id %d"),
			pRequest->context.sessionId.GetString().c_str(), pRequest->key);
	}

	// show query time
	UInt32 dt = ::GetTickCount() - curTime;
	if (dt > 1000)
	{
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("profile, utime %d, acct_id %d, char_id %d"), 
			dt, pRequest->key, pRequest->char_id);
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

void CCharacterData::SetData(CdboCharacter* dboChar)
{
	String nickName(dboChar->m_nickName);

	SetId((UInt32) dboChar->m_acct_id.LowPart,
			(UInt32) dboChar->m_char_id.LowPart);

	SetInfo(nickName.Trim(), 
			(UInt8) dboChar->m_sex, 
			(UInt8) dboChar->m_cclass,
			(UInt8) dboChar->m_faction);

	SetLevelInfo((UInt16) dboChar->m_level);
	SetExp((UInt32) dboChar->m_exp.LowPart);
	SetAttrib((UInt16) dboChar->m_STA, 
			(UInt16) dboChar->m_SPR, 
			(UInt16) dboChar->m_STR, 
			(UInt16) dboChar->m_CON, 
			(UInt16) dboChar->m_AGI);
	SetAttrPoint((UInt16) dboChar->m_attrPoint);

	SetHP((UInt16) dboChar->m_HP,
			(UInt16) dboChar->m_mHP,
			(UInt16) dboChar->m_maxHP);
	SetSP((UInt16) dboChar->m_SP,
			(UInt16) dboChar->m_mSP,
			(UInt16) dboChar->m_maxSP);
	SetDP((UInt16) dboChar->m_DP,
			(UInt16) dboChar->m_mDP,
			(UInt16) dboChar->m_maxDP);
	SetWC((UInt16) dboChar->m_WC,
			(UInt16) dboChar->m_maxWC,
			(UInt16) dboChar->m_WcExp);
	SetVP((UInt16) dboChar->m_VP,
			(UInt16) dboChar->m_maxVP);

	SetGoodness((Int32) dboChar->m_goodness.LowPart);
	SetHonor((UInt32) dboChar->m_honor.LowPart);
	SetTitle(dboChar->m_title);
	SetManor((UInt32) dboChar->m_manor.LowPart);
	SetDonate((UInt32) dboChar->m_fDonate.LowPart,
				(UInt32) dboChar->m_totalFDonate.LowPart,
				(UInt32) dboChar->m_mDonate.LowPart,
				(UInt32) dboChar->m_totalMDonate.LowPart);
	SetMoney((UInt32) dboChar->m_money.LowPart);
	SetBankMoney((UInt32) dboChar->m_bankMoney.LowPart);
	SetPartner((UInt32) dboChar->m_partner.LowPart);
	SetPet((UInt32) dboChar->m_pet.LowPart);
	SetMaxOther((UInt8) dboChar->m_maxPartner,
				(UInt8) dboChar->m_maxPet);

	SetMap((UInt16) dboChar->m_map_id,
			(UInt16) dboChar->m_map_x,
			(UInt16) dboChar->m_map_y,
			(UInt32) dboChar->m_map_ownerId);
	SetReborn((UInt16) dboChar->m_reborn_map,
			(UInt16) dboChar->m_reborn_x,
			(UInt16) dboChar->m_reborn_y);
	SetStyle((UInt16) dboChar->m_photo_id,
			(UInt16) dboChar->m_hairStyle1,
			(UInt16) dboChar->m_hairStyle2,
			(UInt16) dboChar->m_hairColor);

	SetEqWeapon((UInt32) dboChar->m_eq_weapon.LowPart,
				(UInt16) dboChar->m_eq_weaponColor);
	SetEqHead((UInt32) dboChar->m_eq_headBand.LowPart,
				(UInt16) dboChar->m_eq_headColor);
	SetEqCloth((UInt32) dboChar->m_eq_cloth.LowPart,
				(UInt16) dboChar->m_eq_clothColor);
	
	SetState((UInt8) dboChar->m_state);
	SetBuff(dboChar->m_buff);

	SetShortcut((UInt16) dboChar->m_shortcut0, 
				(UInt16) dboChar->m_shortcut1, 
				(UInt16) dboChar->m_shortcut2, 
				(UInt16) dboChar->m_shortcut3, 
				(UInt16) dboChar->m_shortcut4, 
				(UInt16) dboChar->m_shortcut5, 
				(UInt16) dboChar->m_shortcut6, 
				(UInt16) dboChar->m_shortcut7, 
				(UInt16) dboChar->m_shortcut8, 
				(UInt16) dboChar->m_shortcut9,

				(UInt8) dboChar->m_shortcuttype0, 
				(UInt8) dboChar->m_shortcuttype1, 
				(UInt8) dboChar->m_shortcuttype2, 
				(UInt8) dboChar->m_shortcuttype3, 
				(UInt8) dboChar->m_shortcuttype4, 
				(UInt8) dboChar->m_shortcuttype5, 
				(UInt8) dboChar->m_shortcuttype6, 
				(UInt8) dboChar->m_shortcuttype7, 
				(UInt8) dboChar->m_shortcuttype8, 
				(UInt8) dboChar->m_shortcuttype9);

	SetOrgHairStyle((UInt16)dboChar->m_org_hairStyle1, (UInt16)dboChar->m_org_hairColor);
	SetOrgEqCloth((UInt32)dboChar->m_org_eq_cloth.LowPart, (UInt16)dboChar->m_org_eq_clothColor);
	SetSpeakCount((UInt16) dboChar->m_speakCount);

	SetPKInfo((UInt16) dboChar->m_PK_Value);
	time_t uTime;
	TimeConvert(dboChar->m_offlineTime, uTime);
	SetOfflineTime((UInt32)uTime);

	SetTP((UInt16) dboChar->m_TP);

	TimeConvert(dboChar->m_speakBlock, uTime);
	SetSpeakBlock((UInt32)uTime);
	
	SetHawkerTime((UInt32) dboChar->m_hawkerTime.LowPart);

	SetSMoney((UInt32) dboChar->m_SMoney.LowPart);
	SetAvailBackPack((UInt8) dboChar->m_availBackPack);
	SetAvailWareHouse((UInt8) dboChar->m_availItemWareHouse, 
					(UInt8) dboChar->m_availPartnerWareHouse, 
					(UInt8) dboChar->m_availPetWareHouse);

	SetPvPScoreSingle((UInt32) dboChar->m_PvPScoreSingle.LowPart);
	time_t timeToDelete;         
	TimeConvert(dboChar->m_deleteExpiryDate, timeToDelete);
	SetDeleteTime((UInt32)timeToDelete);
	SetBankLock((Boolean) (dboChar->m_bankLock == 1));
	SetSiblingId((UInt32)dboChar->m_siblingId.LowPart);
	SetMark((UInt32)dboChar->m_mark);
	SetWantedReward((UInt32)dboChar->m_wantedReward.LowPart);
	SetGuild_uid((UInt32)dboChar->m_guild_uid.LowPart);
	SetIHisGenAcc((UInt32)dboChar->m_HistoryGenAccmulation.LowPart);
	SetUiCurGenAcc((UInt32)dboChar->m_CurGenAccmulation.LowPart);
	ClearUpdateMask();
}

void CCharacterData::SetDetail(UInt8 detail)
{
	RPCMask mask;

	switch (detail)
	{
	case DETAIL_NONE:
		break;

	case DETAIL_INFO:
		mask.MaskBits(idMask, infoMask, levelInfoMask, mapMask, rebornMask, styleMask, eqWeaponMask, 
			eqHeadMask, eqClothMask, PKInfoMask, orgEqClothMask, orgHairStyleMask, speakCountMask, deleteTimeMask, -1);
		break;

	case DETAIL_LOC:
		mask.MaskBits(idMask, mapMask);
		break;

	case DETAIL_MAP:
		mask.MaskBits(idMask, infoMask, levelInfoMask, expMask, attribMask, attrPointMask,
			goodnessMask, honorMask, donateMask, moneyMask, bankMoneyMask, HPMask, SPMask, 
			DPMask, WCMask, VPMask, stateMask, maxOtherMask, petMask, mapMask, rebornMask, 
			styleMask, eqWeaponMask, eqHeadMask, eqClothMask, shortcutMask, PKInfoMask, 
			buffMask, orgEqClothMask, orgHairStyleMask, speakCountMask, offlineTimeMask,
			hawkerTimeMask, TPMask, speakBlockMask, titleMask, SMoneyMask, availBackPackMask, 
			availWareHouseMask, PvPScoreSingleMask, bankLockMask, SiblingIdMask, markMask,
			wantedRewardMask, GenerationAccMask,TPMask, -1);
		break;

	case DETAIL_BATTLE:
		mask.MaskBits(idMask, infoMask, levelInfoMask, expMask, attribMask, attrPointMask,
			goodnessMask,shortcutMask, honorMask, moneyMask, HPMask, SPMask, DPMask, WCMask,
			VPMask, stateMask, styleMask, maxOtherMask, eqWeaponMask, eqHeadMask, eqClothMask,
			PKInfoMask, buffMask, orgEqClothMask, orgHairStyleMask, TPMask,  SMoneyMask, -1);
		break;

	case DETAIL_MESSAGE:
		mask.MaskBits(idMask, infoMask, levelInfoMask, styleMask, eqWeaponMask, 
			eqHeadMask, eqClothMask, orgEqClothMask, orgHairStyleMask, speakCountMask, 
			speakBlockMask, guild_uidMask, -1);
		break;

	case DETAIL_RANK:
		mask.MaskBits(idMask, infoMask, levelInfoMask, moneyMask, PKInfoMask, honorMask,
			bankMoneyMask, SMoneyMask, PvPScoreSingleMask, wantedRewardMask, -1);
		break;

	case DETAIL_ALL:
		mask = CCharacterData::AllMask;
		break;
	}

	SetSendMask(mask);
}

// set character data detail in group
void CCharacterData::SetGroupDetail(NetGroup *charGroup, UInt8 detail)
{
	if (charGroup == NULL) return;

	for (UInt16 i = 0; i < charGroup->GetCount(); i++)
	{
		CCharacterData *pCharData = charGroup->GetChild<CCharacterData>(i);
		if (pCharData)
			pCharData->SetDetail(detail);
		else
			charGroup->Replace(i, NULL);
	}
}

HRESULT CCharacterData::UpdateDbo(CdboCharacter* dboChar)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("char_id %d, update_mask %d:%d"), 
		GetChar_id(), m_updateMask.low, m_updateMask.high);

	dboChar->ClearParam();

	if (IsUpdated(infoMask))
	{
		COLUMN_UPDATE_STRING(dboChar, nickName, GetNickName());
		COLUMN_UPDATE(dboChar, sex, GetSex());
		COLUMN_UPDATE(dboChar, cclass, GetCclass());
		COLUMN_UPDATE(dboChar, faction, GetFaction());
	}
	if (IsUpdated(levelInfoMask))
	{
		COLUMN_UPDATE(dboChar, level, GetLevel());
	}
	if (IsUpdated(expMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, exp, GetExp());
	}
	if (IsUpdated(attribMask))
	{
		COLUMN_UPDATE(dboChar, STA, GetSTA());
		COLUMN_UPDATE(dboChar, SPR, GetSPR());
		COLUMN_UPDATE(dboChar, STR, GetSTR());
		COLUMN_UPDATE(dboChar, CON, GetCON());
		COLUMN_UPDATE(dboChar, AGI, GetAGI());
	}
	if (IsUpdated(attrPointMask))
	{
		COLUMN_UPDATE(dboChar, attrPoint, GetAttrPoint());
	}
	if (IsUpdated(HPMask))
	{
		COLUMN_UPDATE(dboChar, HP, GetHP());
		COLUMN_UPDATE(dboChar, mHP, GetMHP());
		COLUMN_UPDATE(dboChar, maxHP, GetMaxHP());
	}
	if (IsUpdated(SPMask))
	{
		COLUMN_UPDATE(dboChar, SP, GetSP());
		COLUMN_UPDATE(dboChar, mSP, GetMSP());
		COLUMN_UPDATE(dboChar, maxSP, GetMaxSP());
	}
	if (IsUpdated(DPMask))
	{
		COLUMN_UPDATE(dboChar, DP, GetDP());
		COLUMN_UPDATE(dboChar, mDP, GetMDP());
		COLUMN_UPDATE(dboChar, maxDP, GetMaxDP());
	}
	if (IsUpdated(WCMask))
	{
		COLUMN_UPDATE(dboChar, WC, GetWC());
		COLUMN_UPDATE(dboChar, maxWC, GetMaxWC());
		COLUMN_UPDATE(dboChar, WcExp, GetWcExp());
	}
	if (IsUpdated(VPMask))
	{
		COLUMN_UPDATE(dboChar, VP, GetVP());
		COLUMN_UPDATE(dboChar, maxVP, GetMaxVP());
	}
	if (IsUpdated(goodnessMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, goodness, GetGoodness());
	}
	if (IsUpdated(honorMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, honor, GetHonor());
	}
	if (IsUpdated(titleMask))
	{
		COLUMN_UPDATE_STRING(dboChar, title, GetTitle());
	}
	if (IsUpdated(manorMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, manor, GetManor());
	}
	if (IsUpdated(donateMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, fDonate, GetFDonate());
		COLUMN_UPDATE_LARGE(dboChar, totalFDonate, GetTotalFDonate());
		COLUMN_UPDATE_LARGE(dboChar, mDonate, GetMDonate());
		COLUMN_UPDATE_LARGE(dboChar, totalMDonate, GetTotalMDonate());
	}
	
	if (IsUpdated(moneyMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, money, GetMoney());
	}
	if (IsUpdated(bankMoneyMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, bankMoney, GetBankMoney());
	}
	if (IsUpdated(SMoneyMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, SMoney, GetSMoney());
	}
	if (IsUpdated(partnerMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, partner, GetPartner());
	}
	if (IsUpdated(petMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, pet, GetPet());
	}
	if (IsUpdated(maxOtherMask))
	{
		COLUMN_UPDATE(dboChar, maxPartner, GetMaxPartner());
		COLUMN_UPDATE(dboChar, maxPet, GetMaxPet());
	}
	if (IsUpdated(mapMask))
	{
		COLUMN_UPDATE(dboChar, map_id, GetMap_id());
		COLUMN_UPDATE(dboChar, map_x, GetMap_x());
		COLUMN_UPDATE(dboChar, map_y, GetMap_y());
		COLUMN_UPDATE(dboChar, map_ownerId, GetMap_ownerId());
	}
	if (IsUpdated(rebornMask))
	{
		COLUMN_UPDATE(dboChar, reborn_map, GetReborn_map());
		COLUMN_UPDATE(dboChar, reborn_x, GetReborn_x());
		COLUMN_UPDATE(dboChar, reborn_y, GetReborn_y());
	}
	if (IsUpdated(styleMask))
	{
		COLUMN_UPDATE(dboChar, photo_id, GetPhoto_id());
		COLUMN_UPDATE(dboChar, hairStyle1, GetHairStyle1());
		COLUMN_UPDATE(dboChar, hairStyle2, GetHairStyle2());
		COLUMN_UPDATE(dboChar, hairColor, GetHairColor());
	}
	if (IsUpdated(eqWeaponMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, eq_weapon, GetEq_weapon());
		COLUMN_UPDATE(dboChar, eq_weaponColor, GetEq_weaponColor());
	}
	if (IsUpdated(eqHeadMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, eq_headBand, GetEq_headBand());
		COLUMN_UPDATE(dboChar, eq_headColor, GetEq_headColor());
	}
	if (IsUpdated(eqClothMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, eq_cloth, GetEq_cloth());
		COLUMN_UPDATE(dboChar, eq_clothColor, GetEq_clothColor());
	}
	if (IsUpdated(stateMask))
	{
		COLUMN_UPDATE(dboChar, state, GetState());
	}
	if (IsUpdated(buffMask))
	{
		COLUMN_UPDATE_STRING(dboChar, buff, GetBuff());
	}
	if (IsUpdated(shortcutMask))
	{
		COLUMN_UPDATE(dboChar, shortcut0, GetShortcut(0));
		COLUMN_UPDATE(dboChar, shortcut1, GetShortcut(1));
		COLUMN_UPDATE(dboChar, shortcut2, GetShortcut(2));
		COLUMN_UPDATE(dboChar, shortcut3, GetShortcut(3));
		COLUMN_UPDATE(dboChar, shortcut4, GetShortcut(4));
		COLUMN_UPDATE(dboChar, shortcut5, GetShortcut(5));
		COLUMN_UPDATE(dboChar, shortcut6, GetShortcut(6));
		COLUMN_UPDATE(dboChar, shortcut7, GetShortcut(7));
		COLUMN_UPDATE(dboChar, shortcut8, GetShortcut(8));
		COLUMN_UPDATE(dboChar, shortcut9, GetShortcut(9));
		COLUMN_UPDATE(dboChar, shortcuttype0, GetShortcuttype(0));
		COLUMN_UPDATE(dboChar, shortcuttype1, GetShortcuttype(1));
		COLUMN_UPDATE(dboChar, shortcuttype2, GetShortcuttype(2));
		COLUMN_UPDATE(dboChar, shortcuttype3, GetShortcuttype(3));
		COLUMN_UPDATE(dboChar, shortcuttype4, GetShortcuttype(4));
		COLUMN_UPDATE(dboChar, shortcuttype5, GetShortcuttype(5));
		COLUMN_UPDATE(dboChar, shortcuttype6, GetShortcuttype(6));
		COLUMN_UPDATE(dboChar, shortcuttype7, GetShortcuttype(7));
		COLUMN_UPDATE(dboChar, shortcuttype8, GetShortcuttype(8));
		COLUMN_UPDATE(dboChar, shortcuttype9, GetShortcuttype(9));
	}
	if (IsUpdated(PKInfoMask))
	{
		COLUMN_UPDATE(dboChar, PK_Value, GetPK_Value());
	}
	if (IsUpdated(orgEqClothMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, org_eq_cloth, GetOrg_eq_cloth());
		COLUMN_UPDATE(dboChar, org_eq_clothColor, GetOrg_eq_clothColor());
	}
	if (IsUpdated(orgHairStyleMask))
	{
		COLUMN_UPDATE(dboChar, org_hairStyle1, GetOrg_hairStyle1());
		COLUMN_UPDATE(dboChar, org_hairColor, GetOrg_hairColor());
	}
	if (IsUpdated(speakCountMask))
	{
		COLUMN_UPDATE(dboChar, speakCount, GetSpeakCount());
	}
	if (IsUpdated(offlineTimeMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetOfflineTime(), dbtime);
		COLUMN_UPDATE(dboChar, offlineTime, dbtime);
	}
	if (IsUpdated(TPMask))
	{
		COLUMN_UPDATE(dboChar, TP, GetTP());
	}
	if (IsUpdated(speakBlockMask))	// it runs only when Character initialization from Object Service, not from Message Service
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetSpeakBlock(), dbtime);
		COLUMN_UPDATE(dboChar, speakBlock, dbtime);
	}
	if (IsUpdated(hawkerTimeMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, hawkerTime, GetHawkerTime());
	}
	if (IsUpdated(availBackPackMask))
	{
		COLUMN_UPDATE(dboChar, availBackPack, GetAvailBackPack());
	}
	if (IsUpdated(availWareHouseMask))
	{
		COLUMN_UPDATE(dboChar, availItemWareHouse, GetAvailItemWareHouse());
		COLUMN_UPDATE(dboChar, availPartnerWareHouse, GetAvailPartnerWareHouse());
		COLUMN_UPDATE(dboChar, availPetWareHouse, GetAvailPetWareHouse());
	}

	if (IsUpdated(PvPScoreSingleMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, PvPScoreSingle, GetPvPScoreSingle());
	}

	if (IsUpdated(bankLockMask))
	{
		UInt8 locked = GetBankLock()? 1:0;
		COLUMN_UPDATE(dboChar, bankLock, locked);
	}

	if (IsUpdated(SiblingIdMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, siblingId, GetSibling_id());
	}

	if (IsUpdated(markMask))
	{
		COLUMN_UPDATE(dboChar, mark, GetMark());
	}

	if (IsUpdated(wantedRewardMask))
	{
		COLUMN_UPDATE_LARGE(dboChar, wantedReward, GetWantedReward());
	}

	if (IsUpdated(GenerationAccMask)) {
		COLUMN_UPDATE_LARGE(dboChar, HistoryGenAccmulation, GetIHisGenAcc());
		COLUMN_UPDATE_LARGE(dboChar,CurGenAccmulation,GetUiCurGenAcc());
	}

	HRESULT hr;
	if (DB_FAILED(hr = dboChar->UpdateByAny(GetChar_id())))
		dboChar->ShowError(hr);
	else
		ClearUpdateMask();
	dboChar->ClearParam();
	dboChar->Close();

	return hr;
}

HRESULT CCharacterData::DeleteDbo(CdboCharacter* dboChar)
{
	HRESULT hr;
	//ThreadGuard guard(m_key);

	if (DB_FAILED(hr = dboChar->Delete(GetChar_id())))
		dboChar->ShowError(hr);
	dboChar->Close();

	return hr;
}

void CCharacterData::ReleaseBelong()
{
	UInt32 char_id = GetChar_id();

	// release character belonging
	ReleaseItems(char_id);
	ReleasePartners(char_id);
	ReleaseSkills(char_id, SKILL_OWNER_TYPE_CHAR);
	ReleaseQuests(char_id);
	ReleaseMessengers(char_id);
	ReleasePets(char_id);
	ReleaseHawkerHistory(char_id);
	ReleasePartnerCollections(char_id);
	CSiblingData::ReleaseSibling(char_id);
	CMarriageData::ReleaseMarriage(char_id);
	BOOL bYoung;
	if (GetLevel() < 50) 
		bYoung =TRUE;
	else
		bYoung =FALSE;
	ReleaseGeneration(char_id,bYoung);
}

RPCResult ObjectService_Stub::CreateCharacter(
	LPCPEER pPeer, RPCContext &context, 
	const UInt32 &acct_id, const String &nickName,
	const UInt8 &sex, const UInt8 &cclass, 
	NetObject* charData, const UInt8 &detail)
{
	CCharacterData *pNewData = CastNetObject<CCharacterData>(charData);
	HRESULT hr = S_OK;

	TRACE_ENSURE(pdbCharacter && pNewData);
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("session_id %s, acct_id %d, sex %d, cclass %d"), 
		context.sessionId.GetString().c_str(), acct_id, sex, cclass);

	if (CharacterNameExist(nickName))
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("a player already used the name, session_id %s, acct_id %d"),
			context.sessionId.GetString().c_str(), acct_id);
		// Don't change this message _FW("a player already used the name")
		CB_Error(pPeer, context, hr, _FW("a player already used the name"));
		DeleteNetObject(pNewData);
		return (RPC_RESULT_FAIL);
	}

	if (DB_FAILED(hr = pdbCharacter->Insert(acct_id, nickName.c_str(), sex, cclass, 0)))
	{
		pdbCharacter->ShowError(hr);
		CB_Error(pPeer, context, hr, _FW("DB_FAILED"));
		DeleteNetObject(pNewData);
		return (RPC_RESULT_FAIL);
	}

	CCharacterData* pCharData = NULL;
	if (DB_SUCCEEDED(hr = pdbCharacter->MoveFirst()))
		pCharData = NewCharacter();
	pdbCharacter->Close();
	
	if (DB_FAILED(hr))
	{
		pdbCharacter->ShowError(hr);
		CB_Error(pPeer, context, hr, _FW("DB_FAILED"));
		DeleteNetObject(pNewData);
		return RPC_RESULT_FAIL;
	}

	// send Character data to peer
	if (pCharData)
	{
		CAccountData* pAcctData = FindAccount(acct_id);
		if (pAcctData)
			pAcctData->AddCharId(pCharData->GetChar_id());
		pCharData->Clone(pNewData, pNewData->GetUpdateMask());
		pCharData->SetDetail(detail);
		DataIdMap::iterator iter = gGlobal.charIdMap.find(pCharData->GetChar_id());
		if (iter != gGlobal.charIdMap.end())
			gGlobal.EnqueueUpdate(&iter->second, true);
	}
	DeleteNetObject(pNewData);

	CB_CreateCharacterSuccess(pPeer, context, pCharData->GetChar_id());
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::RestoreCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	TRACE_ENSURE(pdbCharacter != NULL);
	pdbCharacter->Restore(char_id);
	pdbCharacter->Close();

	CCharacterData* pCharData = NULL;
	DataIdMap::iterator itfind = gGlobal.charIdMap.find(char_id);
	if (itfind != gGlobal.charIdMap.end())
		pCharData = CastNetObject<CCharacterData>(itfind->second.pobj);

	if (pCharData)
	{
		pCharData->SetDelete_time(0);	
	}
	CB_RestoreCharacterSuccess(pPeer, context);
	return RPC_RESULT_OK;	
}

RPCResult ObjectService_Stub::SetBankLock(LPCPEER pPeer, RPCContext &context, const UInt32 &charID, const Boolean &lock)
{
 	HRESULT hr = S_OK;
	TRACE_ENSURE(pdbCharacter != NULL);
	CCharacterData* pCharData = NULL;	
	DataIdMap::iterator iter = gGlobal.charIdMap.find(charID);
	if (iter != gGlobal.charIdMap.end())
	{
		pCharData = CastNetObject<CCharacterData>(iter->second.pobj);
		if (pCharData)
            pCharData->SetBankLock(lock);
	}

	Boolean success = BOOLEAN_FALSE;
	success = DB_SUCCEEDED(hr = pdbCharacter->SetBankLock(charID, lock));
	pdbCharacter->Close();
    
	CB_SetAssetLock(pPeer, context, success);   

	return RPC_RESULT_OK;		
}

RPCResult ObjectService_Stub::SetCharacterExpiryDate(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &extendPeriod)
{

	TRACE_ENSURE(pdbCharacter != NULL);
	DBTIMESTAMP deadline;
	
	time_t timeNow = time(NULL);
	TimeConvert(timeNow + (time_t) extendPeriod, deadline);
	pdbCharacter->SetDeleteTime(char_id, deadline);
	pdbCharacter->Close();

	UInt32 deletetime = (UInt32)timeNow + extendPeriod;	
	CCharacterData* pCharData = NULL;
	DataIdMap::iterator itfind = gGlobal.charIdMap.find(char_id);
	if (itfind != gGlobal.charIdMap.end())
		pCharData = CastNetObject<CCharacterData>(itfind->second.pobj);

	if (pCharData)
	{
		pCharData->SetDelete_time(deletetime);
	}
	CB_SetCharExpiryDateSuccess(pPeer, context, deletetime);	
	return RPC_RESULT_OK;
}


RPCResult ObjectService_Stub::DeleteCharacter(
	LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &acct_id)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("session_id %s, char_id %d"),
		context.sessionId.GetString().c_str(), char_id);

	CCharacterData* pCharData = NULL;
	DataIdMap::iterator itfind = gGlobal.charIdMap.find(char_id);
	if (itfind != gGlobal.charIdMap.end())
		pCharData = CastNetObject<CCharacterData>(itfind->second.pobj);

	if (pCharData)
	{
		CAccountData* pAcctData = FindAccount(pCharData->GetAcct_id());
		if (pAcctData)
			pAcctData->RemoveCharId(char_id);
		gGlobal.CacheKeepChar(char_id);
		gGlobal.DequeueUpdate(&itfind->second);		// remove from update queue
		pCharData->ReleaseBelong();		// release character belonging
		gGlobal.charIdMap.erase(char_id);
		gGlobal.charNameMap.erase(pCharData->GetNickName());
		gGlobal.m_deleteGroup.Add(pCharData);
	}
	else
	{
		CAccountData* pAcctData = FindAccount(acct_id);	
		if (pAcctData && (pAcctData->IsCharacterIDExist(char_id)))
		{
			TRACE_ENSURE(pdbCharacter != NULL);
			pdbCharacter->Delete(char_id);
			pdbCharacter->Close();
		}
	}
	CB_DeleteCharacterSuccess(pPeer, context);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetCharacterRank(LPCPEER pPeer, RPCContext &context, 
											   const UInt8 &rankType, const UInt8 &subType)
{
	PROCEDURE_SET_NONBLOCK
		PROCEDURE_START_RPC2(ObjectService, GetCharacterRank, NULL, UInt8, rankType, UInt8, subType)

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("session_id %s, rank_type %d"),
		context.sessionId.GetString().c_str(), rankType);

	gGlobal.WorkQueueBegin();
	CharRequest *pRequest = SafeCreateObject3(CharRequest, 0, pPeer, context);
	StringVector &sqls = pRequest->sqls;
	if (rankType == CHAR_RANK_TYPE_LEVEL)
		sqls.push_back(_T("\"level\" > 0 ORDER BY \"level\" DESC"));
	else if (rankType == CHAR_RANK_TYPE_MONEY)
		sqls.push_back(_T("money + bankMoney > 0 ORDER BY money + bankMoney DESC"));
	else if (rankType == CHAR_RANK_TYPE_CLASS)
	{
		String s;
		s.Format(_T("\"level\" > 0 AND sex = %d AND cclass = %d ORDER BY \"level\" DESC"),
			subType % 2, (subType / 2) + 1);
		sqls.push_back(s);
	}
	else if (rankType == CHAR_RANK_TYPE_FAMOUS)
		sqls.push_back(_T("honor > 0 ORDER BY honor DESC"));
	else if (rankType == CHAR_RANK_TYPE_EVIL)
		sqls.push_back(_T("PK_Value > 0 ORDER BY PK_Value DESC"));
	else if (rankType == CHAR_RANK_TYPE_FACTION)
	{
		String s;
		s.Format(_T("\"level\" > 0 AND faction = %d ORDER BY \"level\" DESC"), subType + 1);
		sqls.push_back(s);
	}
	else if (rankType == CHAR_RANK_TYPE_PVPSCROE)
	{
		sqls.push_back(_T("PvPScoreSingle > 0 ORDER BY PvPScoreSingle DESC"));
	}
	else if (rankType == CHAR_RANK_TYPE_PKVALUE)
	{
		sqls.push_back(_T("PK_Value > 5 ORDER BY PK_Value DESC"));
	}

#ifdef DB_THREAD_POOL
	gGlobal.QueueUserWorkItem(QueueGetCharacters, pRequest, DB_WORKER_FLAG);
#else
	QueueGetCharacters(pRequest);
#endif

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, charGroup)

		CB_GetCharacterRank(pPeer, context, rankType, subType, charGroup);
	DeleteNetGroup(charGroup, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
		_FE("session_id %s, errcode %d, %s"),
		context.sessionId.GetString().c_str(), err, errMsg.c_str());
	CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

		return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::HasCharacter(LPCPEER pPeer, RPCContext &context, const String &charName)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("session_id %s"),
		context.sessionId.GetString().c_str());

	// send character data to peer
	CB_HasCharacter(pPeer, context, CharacterNameExist(charName));
	
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::KeepCharacter(LPCPEER pPeer, RPCContext &context, 
											const UInt32 &acct_id, const UInt32 &char_id)
{
	// only keep that char_id in account, release others
	CAccountData* pAcctData = FindAccount(acct_id);
	if (pAcctData)
		pAcctData->KeepCharId(char_id);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::ReleaseCharacter(LPCPEER pPeer, RPCContext &context, 
											const UInt32 &acct_id, const UInt32 &char_id)
{
	TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("session_id %s, acct_id %d, char_id %d"),
		context.sessionId.GetString().c_str(), acct_id, char_id);

	// release char id of account if exist
	CAccountData* pAcctData = FindAccount(acct_id);
	if (pAcctData)
		pAcctData->ReleaseCharId(char_id);
	gGlobal.CacheReleaseChar(char_id);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetCharacters(LPCPEER pPeer, RPCContext &context, 
											const UInt32 &acct_id, const UInt32 &char_id,
											const UInt8 &detail)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC3(ObjectService, GetCharacters, NULL, UInt32, acct_id, UInt32, char_id,
						UInt8, detail)

		TRACE_ENSURE(pdbCharacter != NULL);

		TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("session_id %s, acct_id %d, char_id %d"),
			context.sessionId.GetString().c_str(), acct_id, char_id);

		if (char_id != 0)
		{
			CCharacterData* pCharData = FindChar(char_id);
			if (pCharData)
			{
				NetGroup grp;
				pCharData->SetDetail(detail);
				grp.Add(pCharData);
				CB_GetCharacterGroup(pPeer, context, &grp, detail);
				PROCEDURE_RETURN
			}
		}
		else
		{
			CAccountData* pAcctData = FindAccount(acct_id);
			if (pAcctData == NULL)
			{
				TRACE_WARNDTL_2(GLOBAL_LOGGER, 
					_FW("account not exist, session_id %s, acct_id %d"),
					context.sessionId.GetString().c_str(), acct_id);
				CB_Error(pPeer, context, 1, _FW("account not exist"));
				PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
			}

			NetGroup* charGroup = pAcctData->GetCharGroup();

			if (charGroup)
			{
				// directly send back object group
				CCharacterData::SetGroupDetail(charGroup, detail);
				CB_GetCharacterGroup(pPeer, context, charGroup, detail);
				PROCEDURE_RETURN
			}
		}

		// wait for thread pool to process
		{
			gGlobal.WorkQueueBegin();
			CharRequest *pRequest = SafeCreateObject3(CharRequest, acct_id, pPeer, context);
			pRequest->char_id = char_id;
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetCharacters, pRequest, DB_WORKER_FLAG);
#else
			QueueGetCharacters(pRequest);
#endif
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, charGroup)

		TRACE_ENSURE(charGroup);

		// store new character group
		NetGroup newGroup;
		CAccountData* pAcctData = FindAccount(acct_id);
		if (pAcctData)
			pAcctData->SetLoaded(BOOLEAN_TRUE);
		for (Index idx = C_INDEX(0); idx < charGroup->GetCount(); ++idx)
		{
			CCharacterData* pCharData = charGroup->GetChild<CCharacterData>(idx);
			if (pCharData)
			{
				if (pAcctData)
					pAcctData->AddCharId(pCharData->GetChar_id());
				// need to register for new char data
				DataObject &dobj = gGlobal.charIdMap[pCharData->GetChar_id()];
				if (dobj.pobj)
				{
					DeleteNetObject(pCharData);
					pCharData = CastNetObject<CCharacterData>(dobj.pobj);
				}
				else
				{
					DataObject &dobj2 = gGlobal.charNameMap[pCharData->GetNickName()];
					LocalRPCManager::GetInstance().Register(pCharData);
					pCharData->ClearUpdateMask();
					dobj.pobj = pCharData;
					dobj2 = dobj;
				}
				newGroup.Add(pCharData);
				if (pCharData)
					gGlobal.CacheKeepChar(pCharData->GetChar_id());		// prevent from release
			}
		}

		// send char data to peer
		CCharacterData::SetGroupDetail(&newGroup, detail);
		CB_GetCharacterGroup(pPeer, context, &newGroup, detail);
		DeleteNetGroup(charGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateCharacter(LPCPEER pPeer, RPCContext &context, 
											  const UInt32 &char_id, NetObject *charData)
{
	CCharacterData* pNewData = CastNetObject<CCharacterData>(charData);
	TRACE_ENSURE(pdbCharacter && pNewData);

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("session_id %s, char_id %d, updateMask %d:%d"),
		context.sessionId.GetString().c_str(), char_id, 
		pNewData->GetUpdateMask().high, pNewData->GetUpdateMask().low);

	CCharacterData* pCharData = FindChar(char_id);
	if (pCharData == NULL)
	{
		// not found in cache
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("character not found session_id %s, char_id %d"),
			context.sessionId.GetString().c_str(), char_id);
		CB_Error(pPeer, context, 1, _FW("character not found"));
		return RPC_RESULT_FAIL;
	}

	DataIdMap::iterator iter = gGlobal.charIdMap.find(char_id);
	pCharData->Clone(pNewData, pNewData->GetUpdateMask());

	//update to message character
	DataIdMap::iterator iterMsg = gGlobal.msgCharMap.find(char_id);
	if (iterMsg != gGlobal.msgCharMap.end())
	{
		CMsgCharacterData* pMsgChar = CastNetObject<CMsgCharacterData>(iterMsg->second.pobj);
		if (pMsgChar)
			pMsgChar->SetData(pCharData);
	}

	if (iter != gGlobal.charIdMap.end())
		gGlobal.EnqueueUpdate(&iter->second);
	DeleteNetObject(pNewData);

	CB_UpdateCharacter(pPeer, context);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateWantedReward(LPCPEER pPeer, RPCContext &context, 
												 const UInt32 &targetId, const UInt32 &money)
{
	CCharacterData* pCharData = FindChar(targetId);
	if (pCharData != NULL)
	{
		pCharData->SetWantedReward(pCharData->GetWantedReward()+money);
		DataIdMap::iterator iter = gGlobal.charIdMap.find(targetId);
		if (iter != gGlobal.charIdMap.end())
			gGlobal.EnqueueUpdate(&iter->second);
		return RPC_RESULT_OK;
	}
	else
	{
		pdbCharacter->UpdateWantedReward(targetId,money);
	}
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateCharGenAcc(LPCPEER pPeer, RPCContext &context, 
											  const UInt32 &uiCharId, 
											  const UInt32 &uiHistoryAcc,
											  const UInt32 &uiCurAcc)
{
	UInt32 uiRealHis=uiHistoryAcc,uiRealCurAcc=uiCurAcc;
	HRESULT hr;
	hr=gGlobal.gTables.pCharacter->RSByID(uiCharId);
	if (SUCCEEDED(hr)) {
		hr = gGlobal.gTables.pCharacter->MoveFirst();
		if(hr == S_OK)
		{
			uiRealHis += (UInt32)gGlobal.gTables.pCharacter->m_HistoryGenAccmulation.QuadPart;
			uiRealCurAcc += (UInt32)gGlobal.gTables.pCharacter->m_CurGenAccmulation.QuadPart;
		}
		gGlobal.gTables.pCharacter->Close();
	}

	
	hr=gGlobal.gTables.pCharacter->UpdateGeneration(uiCharId,uiRealHis,uiRealCurAcc);
	if (SUCCEEDED(hr)) 
		CB_UpdateCharGenAcc(pPeer,context,0);
	else
		CB_UpdateCharGenAcc(pPeer,context,-1);

	gGlobal.gTables.pCharacter->Close();

	return RPC_RESULT_OK;
}

String CCharacterData::DebugInfo()
{
	String s(Parent::DebugInfo());
	s.AppendFormat(_T(", acct_id %d, char_id %d"), GetAcct_id(), GetChar_id());
	return s;
}
