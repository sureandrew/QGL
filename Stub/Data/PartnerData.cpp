//-- Common
#include "Common.h"
#include "Common/SettingTable.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/PartnerData.h"
//-- Library
#include "Stub/Data/SkillData.h"
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboPartner.h"
#include "ObjectService/Global.h"
#include "ObjectService/DboSourcePool.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResMob.h"
#include "Common/common_binary.h"

REGISTER_CLASSTYPE(CPartnerData, PartnerData_CLASSID);

struct PartnerRequest : public WorkRequest<UInt32>
{
	UInt8 inWareHouse;
	PartnerRequest(UInt32 id, LPCPEER pPeer, RPCContext &context)
		: WorkRequest<UInt32>(id, pPeer, context)
	{}
};

CPartnerData* NewPartner();
CdboPartner* &pdbPartner = gGlobal.gTables.pPartner;
StringVector CdboPartnerAccessor::m_params;
ThreadKey CPartnerData::m_key;

NetGroup* FindPartnerGroup(UInt32 char_id);
NetGroup* FindWareHousePartnerGroup(UInt32 char_id);

////////////////////////////////////////////////////////////////////////
////						Partner Data							////
////////////////////////////////////////////////////////////////////////

CPartnerData* NewPartner()
{
	CPartnerData *pPartnerData = NULL;
	UInt32 owner_id = (UInt32) pdbPartner->m_owner_id.LowPart;
	UInt32 partner_id = (UInt32) pdbPartner->m_partner_id.LowPart;
	DataObject &dobj = gGlobal.partnerIdMap[partner_id];

	if (dobj.pobj == NULL) {
		pPartnerData = CPartnerData::NewInstance();
		pPartnerData->SetData(pdbPartner);			// assign values to partner data
		dobj.pobj = pPartnerData;
	} else {
		pPartnerData = CastNetObject<CPartnerData>(dobj.pobj);
	}

	if (pPartnerData)
	{
		// found existing partners in cache
		NetGroup* partnerGroup = NULL;
		if (pPartnerData->GetInWareHouse() == 0)
			partnerGroup = FindPartnerGroup(owner_id);
		else
			partnerGroup = FindWareHousePartnerGroup(owner_id);

		if (partnerGroup)
		{
			partnerGroup->Add(pPartnerData);
		}
		else
		{
			if (pPartnerData->GetInWareHouse() == 0)
			{
				DataObject& dobj3 = gGlobal.partnerGroupMap[owner_id];
				if (dobj3.pobj == NULL) {
					partnerGroup = SafeCreateObject(NetGroup);
					dobj3.pobj = partnerGroup;
				} else {
					partnerGroup = (NetGroup*) dobj3.pobj;
				}
				partnerGroup->Add(pPartnerData);
			}
			else
			{
				DataObject& dobj3 = gGlobal.wareHousePartnerGroupMap[owner_id];
				if (dobj3.pobj == NULL) {
					partnerGroup = SafeCreateObject(NetGroup);
					dobj3.pobj = partnerGroup;
				} else {
					partnerGroup = (NetGroup*) dobj3.pobj;
				}
				partnerGroup->Add(pPartnerData);
			}
		}
	}
	return pPartnerData;
}

CPartnerData* FindPartner(UInt32 partner_id)
{
	DataIdMap::iterator iter = gGlobal.partnerIdMap.find(partner_id);
	CPartnerData* pPartnerData = NULL;

	// found existing account in cache
	if (iter != gGlobal.partnerIdMap.end())
		pPartnerData = CastNetObject<CPartnerData>(iter->second.pobj);

	return pPartnerData;
}

NetGroup* FindPartnerGroup(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.partnerGroupMap.find(char_id);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.partnerGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}

NetGroup* FindWareHousePartnerGroup(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.wareHousePartnerGroupMap.find(char_id);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.wareHousePartnerGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}

HRESULT RemovePartner(UInt32 partner_id)
{
	DataIdMap::iterator iter = gGlobal.partnerIdMap.find(partner_id);
	if (iter != gGlobal.partnerIdMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CPartnerData* pPartnerData = CastNetObject<CPartnerData>(iter->second.pobj);
		if (pPartnerData) 
		{
			NetGroup* partnerGroup = NULL;
			if (pPartnerData->GetInWareHouse() == 0)
				partnerGroup = FindPartnerGroup(pPartnerData->GetOwner_id());
			else
				partnerGroup = FindWareHousePartnerGroup(pPartnerData->GetOwner_id());

			if (partnerGroup)
				partnerGroup->Remove(pPartnerData);
			gGlobal.m_deleteGroup.Add(pPartnerData);
		}
		gGlobal.partnerIdMap.erase(iter);
	}
	RemoveSkills(partner_id, SKILL_OWNER_TYPE_PARTNER);

	return S_OK;
}

void ReleasePartners(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.partnerGroupMap.find(char_id);

	if (iter != gGlobal.partnerGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		CPartnerData *pPartnerData;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++) {
			pPartnerData = grp->GetChild<CPartnerData>(i);
			if (pPartnerData) {
				DataIdMap::iterator it = gGlobal.partnerIdMap.find(
					pPartnerData->GetPartner_id());
				if (it != gGlobal.partnerIdMap.end()) {		// remove from update queue
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.partnerIdMap.erase(it);
				}
				ReleaseSkills(pPartnerData->GetPartner_id(), 1);
				if (pPartnerData->IsUpdated())				// write to db if updated
					gGlobal.m_releaseGroup.Add(pPartnerData);
				else
					DeleteNetObject(pPartnerData);
			}
		}
		gGlobal.partnerGroupMap.erase(iter);
		SafeDeleteObject(grp);
	}

	iter = gGlobal.wareHousePartnerGroupMap.find(char_id);

	if (iter != gGlobal.wareHousePartnerGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		CPartnerData *pPartnerData;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++) {
			pPartnerData = grp->GetChild<CPartnerData>(i);
			if (pPartnerData) {
				DataIdMap::iterator it = gGlobal.partnerIdMap.find(
					pPartnerData->GetPartner_id());
				if (it != gGlobal.partnerIdMap.end()) {		// remove from update queue
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.partnerIdMap.erase(it);
				}
				ReleaseSkills(pPartnerData->GetPartner_id(), 1);
				if (pPartnerData->IsUpdated())				// write to db if updated
					gGlobal.m_releaseGroup.Add(pPartnerData);
				else
					DeleteNetObject(pPartnerData);
			}
		}
		gGlobal.wareHousePartnerGroupMap.erase(iter);
		SafeDeleteObject(grp);
	}
}

void CPartnerData::SetGroupDetail(NetGroup* partnerGroup, UInt8 detail)
{
	if (partnerGroup == NULL) return;

	for (UInt16 i = 0; i < partnerGroup->GetCount(); i++) {
		CPartnerData *pPartnerData = partnerGroup->GetChild<CPartnerData>(i);
		if (pPartnerData)
			pPartnerData->SetDetail(detail);
		else
			partnerGroup->Replace(i, NULL);
	}
}

DWORD WINAPI QueueGetPartners(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();

	PartnerRequest* pRequest = (PartnerRequest*) lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
	{
		NetGroup partnerGroup;
		CPartnerData* pPartnerData;
		CdboPartner* dboPartner = SafeCreateObject(CdboPartner);

		partnerGroup.SetValidate(BOOLEAN_FALSE);
		dboPartner->SetDataSource(dbSrc);
		if (DB_SUCCEEDED(hr = dboPartner->RSByOwnerID(pRequest->key, pRequest->inWareHouse)))
		{
			hr = dboPartner->MoveFirst();
			while (hr == S_OK)
			{
				pPartnerData = SafeCreateObject(CPartnerData);
				pPartnerData->SetData(dboPartner);
				pPartnerData->SetDetail(CPartnerData::DETAIL_ALL);
				partnerGroup.Add(pPartnerData);
				hr = dboPartner->MoveNext();
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetPartners"), hr, dboPartner->GetErrorMsg(hr), 
			pRequest->context, &partnerGroup);
		dboPartner->Close();
		SafeDeleteObject(dboPartner);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, owner_id %d"),
			pRequest->context.sessionId.GetString().c_str(), pRequest->key);
	}
	// show query time
	UInt32 dt = ::GetTickCount() - curTime;
	if (dt > 1000)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("QueueGetPartners Profile: utime %d, char_id %d"), 
			dt, pRequest->key);
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

void CPartnerData::SetData(CdboPartner* dboPartner)
{
	String raname(dboPartner->m_raname);

	SetId( (UInt32)dboPartner->m_partner_id.LowPart, (UInt32)dboPartner->m_mob_id.LowPart );
	SetOwner( (UInt32)dboPartner->m_owner_id.LowPart);
	SetInfo(raname.Trim());
	SetLevel((UInt16)dboPartner->m_level);
	SetExp( (UInt32)dboPartner->m_exp.LowPart);
	SetAttrib1( (UInt16)dboPartner->m_STA, (UInt16)dboPartner->m_SPR, (UInt16)dboPartner->m_STR,
		(UInt16)dboPartner->m_CON, (UInt16)dboPartner->m_AGI);
	
	SetAttrPoint( (UInt16)dboPartner->m_attrPoint);
	SetNative((UInt16)dboPartner->m_HP_native, (UInt16)dboPartner->m_SP_native, (UInt16)dboPartner->m_ATK_native,
		(UInt16)dboPartner->m_DEF_native, (UInt16)dboPartner->m_SPD_native, (UInt16)dboPartner->m_POW_native,
		(UInt16)dboPartner->m_HIT_native, (UInt16)dboPartner->m_EVA_native);
	SetGrow(dboPartner->m_grow);
	SetHP((UInt16)dboPartner->m_HP, (UInt16)dboPartner->m_maxHP);
	
	SetSP((UInt16)dboPartner->m_SP, (UInt16)dboPartner->m_maxSP);
	
	SetFavor(dboPartner->m_favour);
	SetState((UInt8)dboPartner->m_state);
	SetBuff(dboPartner->m_buff);
	SetHealth((UInt16)dboPartner->m_health);
	SetGoodness((Int32)dboPartner->m_goodness);
	SetSoulboundState((UInt8)dboPartner->m_soulboundState);
	time_t uTime;
	TimeConvert(dboPartner->m_expireDate, uTime);
	SetExpireDate((UInt32)uTime);

	SetElement((UInt8)dboPartner->m_element);
	SetOutLookColor((UInt16)dboPartner->m_outLookColor);
	SetInWareHouse((UInt8)dboPartner->m_inWareHouse);
	SetLock((Boolean)(dboPartner->m_lock == 1));
	SetCultivation((UInt16)dboPartner->m_cultivation);
	SetPractice((UInt16)dboPartner->m_practice);
	ClearUpdateMask();
}

void CPartnerData::SetDetail(UInt8 detail)
{
	
	RPCMask mask;

	switch (detail)
	{
	case DETAIL_NONE:
		break;

	case DETAIL_INFO:
		mask.MaskBits(idMask, ownerMask, infoMask, levelMask, expMask, -1);
		break;

	case DETAIL_MAP:
		mask.MaskBits(idMask, ownerMask, infoMask, levelMask, expMask, attrib1Mask, attrPointMask, 
			nativeMask, growMask, HPMask, SPMask, favorMask, stateMask, healthMask, goodnessMask, 
			soulboundStateMask, ExpireDateMask, ElementMask, OutLookColorMask, inWareHouseMask, assetLockMask, CultivationMask, PracticeMask, -1);
		break;

	case DETAIL_BATTLE:
		mask.MaskBits(idMask, ownerMask, infoMask, levelMask, expMask, attrib1Mask,
			attrPointMask, nativeMask, growMask, HPMask, SPMask, favorMask, stateMask, healthMask, 
			goodnessMask, ElementMask, OutLookColorMask, CultivationMask, PracticeMask, -1);
		break;

	case DETAIL_ALL:
		mask = CPartnerData::AllMask;
		break;
	}

	SetSendMask(mask);
}

HRESULT CPartnerData::UpdateDbo(CdboPartner* dboPartner)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CPartnerData UpdateDbo: partner_id %d, owner_id %d updateMask %d,%d"), 
		GetPartner_id(), GetOwner_id(), m_updateMask.low, m_updateMask.high);

	dboPartner->ClearParam();

	if (IsUpdated(ownerMask))
	{
		COLUMN_UPDATE_LARGE(dboPartner, owner_id, GetOwner_id());
	}

	if (IsUpdated(infoMask))
	{
		COLUMN_UPDATE_STRING(dboPartner, raname, GetRaname());
	}

	if (IsUpdated(levelMask))
	{
		COLUMN_UPDATE(dboPartner, level, GetLevel());
	}

	if (IsUpdated(expMask))
	{
		COLUMN_UPDATE_LARGE(dboPartner, exp, GetExp());
	}

	if (IsUpdated(attrib1Mask))
	{
		COLUMN_UPDATE(dboPartner, STA, GetSTA());
		COLUMN_UPDATE(dboPartner, SPR, GetSPR());
		COLUMN_UPDATE(dboPartner, STR, GetSTR());
		COLUMN_UPDATE(dboPartner, CON, GetCON());
		COLUMN_UPDATE(dboPartner, AGI, GetAGI());
	}

	if (IsUpdated(attrPointMask))
	{
		COLUMN_UPDATE(dboPartner, attrPoint, GetAttrPoint());
	}

	if (IsUpdated(nativeMask))
	{
		COLUMN_UPDATE(dboPartner, HP_native, GetHP_native());
		COLUMN_UPDATE(dboPartner, SP_native, GetSP_native());
		COLUMN_UPDATE(dboPartner, ATK_native, GetATK_native());
		COLUMN_UPDATE(dboPartner, DEF_native, GetDEF_native());
		COLUMN_UPDATE(dboPartner, SPD_native, GetSPD_native());
		COLUMN_UPDATE(dboPartner, POW_native, GetPOW_native());
		COLUMN_UPDATE(dboPartner, HIT_native, GetHIT_native());
		COLUMN_UPDATE(dboPartner, EVA_native, GetEVA_native());
	}

	if (IsUpdated(growMask))
	{
		COLUMN_UPDATE(dboPartner, grow, GetGrow());
	}

	if (IsUpdated(HPMask))
	{
		COLUMN_UPDATE(dboPartner, HP, GetHP());
		COLUMN_UPDATE(dboPartner, maxHP, GetMaxHP());
		
	}

	if (IsUpdated(SPMask))
	{
		COLUMN_UPDATE(dboPartner, SP, GetSP());
		COLUMN_UPDATE(dboPartner, maxSP, GetMaxSP());
	}
	
	if (IsUpdated(favorMask))
	{
		COLUMN_UPDATE(dboPartner, favour, GetFavor());
	}

	if (IsUpdated(stateMask))
	{
		COLUMN_UPDATE(dboPartner, state, GetState());
	}

	if (IsUpdated(buffMask)) {
		COLUMN_UPDATE_STRING(dboPartner, buff, GetBuff());
	}

	if (IsUpdated(healthMask))
	{
		COLUMN_UPDATE(dboPartner, health, GetHealth());
	}

	if (IsUpdated(goodnessMask))
	{
		COLUMN_UPDATE(dboPartner, goodness, GetGoodness());
	}

	if (IsUpdated(soulboundStateMask))
	{
		COLUMN_UPDATE(dboPartner, soulboundState, GetSoulboundState());
	}
	
	if (IsUpdated(ExpireDateMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetExpireDate(), dbtime);
		COLUMN_UPDATE(dboPartner, expireDate, dbtime);
		
	}

	if (IsUpdated(ElementMask))
	{
		COLUMN_UPDATE(dboPartner, element, GetElement());
	}

	if (IsUpdated(OutLookColorMask))
	{
		COLUMN_UPDATE(dboPartner, outLookColor, GetOutLookColor());
	}	

	if (IsUpdated(inWareHouseMask))
	{
		COLUMN_UPDATE(dboPartner, inWareHouse, GetInWareHouse());
	}

	if (IsUpdated(assetLockMask))
	{
		UInt8 locked = GetLock()? 1:0;
		COLUMN_UPDATE(dboPartner, lock, locked);
	}

	if (IsUpdated(CultivationMask))
	{
		COLUMN_UPDATE(dboPartner, cultivation, GetCultivation());
	}

	if (IsUpdated(PracticeMask))
	{
		COLUMN_UPDATE(dboPartner, practice, GetPractice());
	}

	HRESULT hr;
	if (DB_FAILED(hr = dboPartner->UpdateByAny(GetPartner_id())))
		dboPartner->ShowError(hr);
	else
		ClearUpdateMask();
	dboPartner->ClearParam();
	dboPartner->Close();

	return hr;
}

HRESULT CPartnerData::DeleteDbo(CdboPartner* dboPartner)
{
	HRESULT hr;

	//ThreadGuard guard(m_key);
	if (DB_FAILED(hr = dboPartner->Delete(GetPartner_id())))
		dboPartner->ShowError(hr);
	dboPartner->Close();

	return hr;
}

HRESULT DoCreatePartner(UInt32 mob_id, UInt32 owner_id, UInt32 level , CPartnerData* &pPartnerData)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreatePartner: mob_id %d, owner_id %d"), mob_id,
		owner_id);

	HRESULT hr = S_OK;
	// get mob data from resource
	const MobData* data = gGlobal.m_resource->GetMobData(mob_id);
	if (data == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CreatePartner DB_FAILED: mob_id %d not found in resource"), mob_id);
		return S_FALSE;
	}

	pPartnerData = NULL;

	UInt16 health = data->Health_f;
	if (health == 0)
		health = 5000;
	else if (health > 2000)
		health = GetRand( (health - 2000 ), health);
	else
		health = GetRand(1, health);

	UInt8 potential = data->potential;
	if (potential > 10 )
		potential = 0;
	if (potential == 10)
	{
		MobPotentSet f;
		UInt8 total = 0;
		for (UInt8 i = 0; i < 5; ++i)
		{
			UInt8 randNo = GetRand(2);	
			if (total == 5)
				randNo = 0;
			else
			{
				total += randNo;
				if (total > 5)
				{
					randNo = total - 5;
					total = 5;
				}
			}
			switch(i)
			{
			case 0:	f.agi = 1 + randNo;	break;
			case 1:	f.con = 1 + randNo;	break;
			case 2:	f.spr = 1 + randNo;	break;
			case 3:	f.sta = 1 + randNo;	break;
			case 4:	f.str = 1 + randNo;	break;
			}
		}
		pdbPartner->m_STA = GetRand(data->STA_base_d, data->STA_base_u) + level * f.sta;
		pdbPartner->m_SPR = GetRand(data->SPR_base_d, data->SPR_base_u) + level * f.spr;
		pdbPartner->m_STR = GetRand(data->STR_base_d, data->STR_base_u) + level * f.str;
		pdbPartner->m_CON = GetRand(data->CON_base_d, data->CON_base_u) + level * f.con;
		pdbPartner->m_AGI = GetRand(data->AGI_base_d, data->AGI_base_u) + level * f.agi;
	}
	else
	{
		const MobPotentSet &f = gMobPotentSet[potential];	
		pdbPartner->m_STA = GetRand(data->STA_base_d, data->STA_base_u) + level * f.sta;
		pdbPartner->m_SPR = GetRand(data->SPR_base_d, data->SPR_base_u) + level * f.spr;
		pdbPartner->m_STR = GetRand(data->STR_base_d, data->STR_base_u) + level * f.str;
		pdbPartner->m_CON = GetRand(data->CON_base_d, data->CON_base_u) + level * f.con;
		pdbPartner->m_AGI = GetRand(data->AGI_base_d, data->AGI_base_u) + level * f.agi;
	}
	pdbPartner->m_level = level;
	pdbPartner->m_HP_native = GetRand(data->HP_native_d, data->HP_native_u);
	pdbPartner->m_SP_native = GetRand(data->SP_native_d, data->SP_native_u);
	pdbPartner->m_ATK_native = GetRand(data->ATK_native_d, data->ATK_native_u);
	pdbPartner->m_DEF_native = GetRand(data->DEF_native_d, data->DEF_native_u);
	pdbPartner->m_SPD_native = GetRand(data->SPD_native_d, data->SPD_native_u);
	pdbPartner->m_POW_native = GetRand(data->POW_native_d, data->POW_native_u);
	pdbPartner->m_HIT_native = GetRand(data->HIT_native_d, data->HIT_native_u);
	pdbPartner->m_EVA_native = GetRand(data->EVA_native_d, data->EVA_native_u);
	pdbPartner->m_grow = GetRand(data->grow_d, data->grow_u);
	pdbPartner->m_goodness = data->goodness;
	pdbPartner->m_health = health;
	pdbPartner->m_HP = (UInt16) (pdbPartner->m_STA * pdbPartner->m_grow * 6  + level * pow((double) pdbPartner->m_HP_native, 1.246) / 3200);
	pdbPartner->m_mHP = pdbPartner->m_HP;
	pdbPartner->m_maxHP = pdbPartner->m_HP;
	pdbPartner->m_SP = (UInt16) (pdbPartner->m_SPR * pdbPartner->m_grow * 5.5f + level * pow((double)pdbPartner->m_POW_native, 1.23) / 2000 - 14);
	pdbPartner->m_mSP = pdbPartner->m_SP;
	pdbPartner->m_maxSP = pdbPartner->m_SP;
	pdbPartner->m_favour = 100.0f;
	UInt8 soulbound = data->soulbound;
	if (soulbound == SOULBOUND_GET_BOUND)
		soulbound = SOULBOUND_ALREADY_BOUND;
	pdbPartner->m_soulboundState = soulbound;
	pdbPartner->m_outLookColor = data->outlookColor;

	pdbPartner->m_cultivation = 0;
	if (data->race == 1)
	{
		if(GetRand(100) < 70)
			pdbPartner->m_practice = 0;
		else
			pdbPartner->m_practice = GetRand(1, 10);
	}
	else
		pdbPartner->m_practice = 0;

	//TCHAR abc = data->color[11];
	//if (abc > 96)
	//	abc -= 97;
	
	if (data->TimeLimitType == PARTNER_TIME_LIMIT_TYPE_CREATE)
	{

		DBTIMESTAMP dbtime;
		UInt32 now = (UInt32) time(NULL);
		UInt32 expire;
		expire = now + 3600 * 24 * data->TimeLimit;
		TimeConvert((time_t) expire, dbtime);
		pdbPartner->m_expireDate = dbtime;
		hr = pdbPartner->InsertMobWithTimeLimit(mob_id, owner_id, data->mid_name);
	}
	else
	{
		hr = pdbPartner->InsertMob(mob_id, owner_id, data->mid_name);
	}

	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbPartner->MoveFirst()))
	{
		// create partner object in object service
		pPartnerData = NewPartner();
		
	}
	pdbPartner->Close();

	return hr;
}

RPCResult ObjectService_Stub::CreatePartner(LPCPEER pPeer, RPCContext &context, const UInt32 &mob_id, const UInt32 &level, const UInt32 &owner_id, const UInt8 &detail)
{
	TRACE_ENSURE(pdbPartner != NULL);
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreatePartner: mob_id = %d, owner_id = %d"), mob_id, owner_id);

	HRESULT hr = S_OK;
	CPartnerData* pPartnerData = NULL;

	hr = DoCreatePartner(mob_id, owner_id, level, pPartnerData);
	if (DB_FAILED(hr)) {
		pdbPartner->ShowError(hr);
		CB_Error(pPeer, context, hr, _T("CreatePartner DB_FAILED:"));
		return RPC_RESULT_FAIL;
	}

	if (pPartnerData)
	{
		pPartnerData->SetDetail(detail);
	}
	
	CB_CreatePartnerSuccess(pPeer, context, pPartnerData);
	

	return RPC_RESULT_OK;

}

RPCResult ObjectService_Stub::GetPartnersByCharID(LPCPEER pPeer, RPCContext &context, 
												  const UInt32 &owner_id, const UInt8 &inWareHouse, const UInt8 &detail)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC3(ObjectService, GetPartnersByCharID, NULL, UInt32, owner_id, UInt8, inWareHouse, UInt8, detail)

		TRACE_ENSURE(pdbPartner != NULL);

		HRESULT hr = S_OK;
		CPartnerData* pPartnerData = NULL;
		NetGroup* partnerGroup = NULL;
		if (inWareHouse == 0)
			partnerGroup = FindPartnerGroup(owner_id);
		else
			partnerGroup = FindWareHousePartnerGroup(owner_id);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GetPartnersByCharID: owner_id %d"), owner_id);

		if (partnerGroup)
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("GetPartnersByCharID: partner group found owner_id %d, count = %d"), owner_id, partnerGroup->GetCount());
			// directly send back object group
			CPartnerData::SetGroupDetail(partnerGroup, detail);
			CB_GetPartnerGroup(pPeer, context, partnerGroup, detail);
			PROCEDURE_RETURN
		}
		else
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GetPartnersByCharID: need retrive partner group from db owner_id %d"), owner_id);
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			PartnerRequest *pRequest = SafeCreateObject3(PartnerRequest, owner_id, pPeer, context);
			if (pRequest)
				pRequest->inWareHouse = inWareHouse;
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetPartners, pRequest, DB_WORKER_FLAG);
#else
			QueueGetPartners(pRequest);
#endif
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, partnerGroup)

		TRACE_ENSURE(partnerGroup);
		TRACE_ENSURE(partnerGroup->GetClassID() == NetGroup_CLASSID);

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("GetPartnersByCharID: retrived partner group from db owner_id %d, count %d"), owner_id, partnerGroup->GetCount());

		// store new partner group
		CPartnerData* pPartnerData;
		
		NetGroup* newGroup = NULL;
		if (inWareHouse == 0)
		{
			DataObject& dobj3 = gGlobal.partnerGroupMap[owner_id];
			if (dobj3.pobj) {
				newGroup = (NetGroup*) dobj3.pobj;
			} else {
				newGroup = SafeCreateObject(NetGroup);
				dobj3.pobj = newGroup;
			}
		}
		else
		{
			DataObject& dobj3 = gGlobal.wareHousePartnerGroupMap[owner_id];
			if (dobj3.pobj) {
				newGroup = (NetGroup*) dobj3.pobj;
			} else {
				newGroup = SafeCreateObject(NetGroup);
				dobj3.pobj = newGroup;
			}
		}
		for (UInt16 i = 0; i < partnerGroup->GetCount(); i++)
		{
			pPartnerData = partnerGroup->GetChild<CPartnerData>(i);
			if (pPartnerData && pPartnerData->GetOwner_id() == owner_id)
			{
				// need to register for new partner data
				DataObject &dobj = gGlobal.partnerIdMap[pPartnerData->GetPartner_id()];
				if (dobj.pobj) {
					DeleteNetObject(pPartnerData);
					pPartnerData = CastNetObject<CPartnerData>(dobj.pobj);
					if (pPartnerData->GetOwner_id() != owner_id)
					{
						TRACE_WARNDTL_4(GLOBAL_LOGGER, 
							_FW("not owned, partner_id %d, mob_id %d, owner_id %d, new data owner_id %d"),
							pPartnerData->GetPartner_id(), pPartnerData->GetMob_id(), 
							owner_id, pPartnerData->GetOwner_id());
						pPartnerData = NULL;
					}
				} else {
					LocalRPCManager::GetInstance().Register(pPartnerData);
					pPartnerData->ClearUpdateMask();
					dobj.pobj = pPartnerData;
				}
				if (pPartnerData)
					newGroup->Add(pPartnerData);
			}
		}

		// send partner data to peer
		CPartnerData::SetGroupDetail(newGroup, detail);
		CB_GetPartnerGroup(pPeer, context, newGroup, detail);
		DeleteNetGroup(partnerGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_T("GetPartnersByCharID Error: sessionId %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}
RPCResult ObjectService_Stub::UpdatePartnerList(
	LPCPEER pPeer, RPCContext &context, 
	const UInt32Vector &partnerList, 
	NetGroup* grp, const Boolean &separated)
{
	TRACE_ENSURE(pdbPartner && grp);

	HRESULT hr = S_OK;
	NetGroup newPartners;
	UInt16 i = 0;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdatePartnerList: partner_id %s"), 
		GetVectorString(partnerList).c_str());

	for (Index idx = C_INDEX(0); idx < partnerList.size(); ++idx)
	{
		const UInt32& partner_id = partnerList[idx];

		DataIdMap::iterator iter = gGlobal.partnerIdMap.end();
		CPartnerData* pPartnerData = NULL;
		CPartnerData* pNewData = NULL;
		NetGroup* partnerGroup = NULL;

		if (partner_id != 0)
		{
			iter = gGlobal.partnerIdMap.find(partner_id);
			if (iter != gGlobal.partnerIdMap.end())
			{
				// found existing partner in cache
				pPartnerData = CastNetObject<CPartnerData>(iter->second.pobj);
				TRACE_ENSURE(pPartnerData);
				if (partnerGroup == NULL)
				{
					if (pPartnerData->GetInWareHouse() == 0)
						partnerGroup = FindPartnerGroup(pPartnerData->GetOwner_id());
					else
						partnerGroup = FindWareHousePartnerGroup(pPartnerData->GetOwner_id());
				}
			}
			else
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("UpdatePartnerList Warning: partner_id %d not found"),
					partner_id);
				continue;
			}
		}

		if (pNewData == NULL || separated)
		{
			// separated data
			if (idx < grp->GetCount())
			{
				pNewData = grp->GetChild<CPartnerData>(idx);
			}
			else
			{
				TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("UpdatePartnerList Warning: no more data in NetGroup, count %d, partner_id %d"), grp->GetCount(), partner_id);
				continue;
			}
			if (pNewData == NULL)
			{
				// skip if null data
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("UpdatePartnerList Warning: invalid partner data, partner_id %d"), partner_id);
				continue;
			}
		}

		if (partner_id != pNewData->GetPartner_id())
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("UpdatePartnerList Error: [partner_id not match new data partner_id] partner_id %d, new data partner_id %d"),
				partner_id, pNewData->GetPartner_id());
			continue;
		}

		// remove partner if owner_id = 0
		if (pNewData->IsUpdated(CPartnerData::ownerMask) && pNewData->GetOwner_id() == 0)
		{
			hr = RemovePartner(partner_id);
			if (DB_FAILED(hr))
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("UpdatePartnerList DB_FAILED: partner delete failed, hr=%X, partner_id=%i"), hr, partner_id);
				pdbPartner->ShowError(hr);
				CB_Error(pPeer, context, hr, _T("UpdatePartnerList DB_FAILED: partner delete failed"));
				break;
			}
			
		}
		else if (partner_id == 0)
		{
			// create partner if partner_id == 0
			pPartnerData = pNewData;
			hr = DoCreatePartner(pPartnerData->GetMob_id(), pPartnerData->GetOwner_id(),
				pPartnerData->GetLevel(), pPartnerData);
			if (DB_FAILED(hr))
			{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("UpdatePartnerList DB_FAILED: partner create failed, hr=%X"), hr);
				pdbPartner->ShowError(hr);
				CB_Error(pPeer, context, hr, _T("UpdatePartnerList DB_FAILED: partner create failed"));
				break;
			}
			pPartnerData->SetDetail(CPartnerData::DETAIL_MAP);
			newPartners.Add(pPartnerData);
		}
		else
		{
			// else update partner			
			if (pNewData->IsUpdated(CPartnerData::ownerMask) && pNewData->GetOwner_id() != pPartnerData->GetOwner_id())	//change the ownerID in the GroupMap
			{
				NetGroup* tempPartnerGroup = NULL;
				if (pPartnerData->GetInWareHouse() == 0)
					tempPartnerGroup = FindPartnerGroup(pPartnerData->GetOwner_id());
				else
					tempPartnerGroup = FindWareHousePartnerGroup(pPartnerData->GetOwner_id());

				if (tempPartnerGroup && tempPartnerGroup->GetCount() > 0)
					tempPartnerGroup->Remove(pPartnerData);

				if (pNewData->IsUpdated(CPartnerData::inWareHouseMask) && pNewData->GetInWareHouse() == 1)
				{
					tempPartnerGroup = FindWareHousePartnerGroup(pNewData->GetOwner_id());
					if (tempPartnerGroup == NULL)
					{
						DataObject& dobj3 = gGlobal.wareHousePartnerGroupMap[pNewData->GetOwner_id()];
						if (dobj3.pobj) {
							tempPartnerGroup = (NetGroup*) dobj3.pobj;
						} else {
							tempPartnerGroup = SafeCreateObject(NetGroup);
							dobj3.pobj = tempPartnerGroup;
						}
					}
				}
				else
				{
					tempPartnerGroup = FindPartnerGroup(pNewData->GetOwner_id());
					if (tempPartnerGroup == NULL)
					{
						DataObject& dobj3 = gGlobal.partnerGroupMap[pNewData->GetOwner_id()];
						if (dobj3.pobj) {
							tempPartnerGroup = (NetGroup*) dobj3.pobj;
						} else {
							tempPartnerGroup = SafeCreateObject(NetGroup);
							dobj3.pobj = tempPartnerGroup;
						}
					}
				}
				pPartnerData->Clone(pNewData, pNewData->GetUpdateMask());
				tempPartnerGroup->Add(pPartnerData);
			}
			else if (pNewData->IsUpdated(CPartnerData::inWareHouseMask) &&
				pPartnerData->GetInWareHouse() != pNewData->GetInWareHouse())
			{
				NetGroup* srcGrp = NULL;
				NetGroup* desGrp = NULL;
				
				if (pPartnerData->GetInWareHouse() == 0)
				{
					srcGrp = FindPartnerGroup(pPartnerData->GetOwner_id());
					desGrp = FindWareHousePartnerGroup(pPartnerData->GetOwner_id());
				}
				else
				{
					srcGrp = FindWareHousePartnerGroup(pPartnerData->GetOwner_id());
					desGrp = FindPartnerGroup(pPartnerData->GetOwner_id());					
				}

				if (srcGrp && desGrp)
				{
					srcGrp->Remove(pPartnerData);
					desGrp->Add(pPartnerData);
				}				
				pPartnerData->Clone(pNewData, pNewData->GetUpdateMask());
			}
			else
				pPartnerData->Clone(pNewData, pNewData->GetUpdateMask());
			gGlobal.EnqueueUpdate(&iter->second);
		}
	}
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	pdbPartner->Close();

	if (DB_FAILED(hr))
	{
		pdbPartner->ShowError(hr);
		DeleteNetChilds(&newPartners);
		return RPC_RESULT_FAIL;
	}

	CB_UpdatePartnerList(pPeer, context, &newPartners);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::SetPartnerLock(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID, const Boolean &lock)
{
	HRESULT hr = S_OK;
	TRACE_ENSURE(pdbPartner);	
	DataIdMap::iterator itfind = gGlobal.partnerIdMap.find(partnerID);
	if (itfind != gGlobal.partnerIdMap.end())
	{
		CPartnerData* pData = CastNetObject<CPartnerData>(itfind->second.pobj);
		if (pData)
			pData->SetLock(lock);
	}
	Boolean success = BOOLEAN_FALSE;
	success = DB_SUCCEEDED(hr = pdbPartner->SetLock(partnerID, lock));
	pdbPartner->Close();
	CB_SetAssetLock(pPeer, context, success);

	return RPC_RESULT_OK;

}

