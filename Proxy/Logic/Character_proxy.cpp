//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Character_proxy.h"


void Character_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.acct_id);
		buf.Read<UInt32>(m.char_id);
	}
	if (m_updateMask.IsBit(levelInfoMask))
	{
		buf.Read<UInt16>(m.level);
		buf.Read<UInt32>(m.nextLvExp);
	}
	if (m_updateMask.IsBit(expMask))
	{
		buf.Read<UInt32>(m.exp);
	}
	if (m_updateMask.IsBit(attrib1Mask))
	{
		buf.Read<UInt16>(m.STA);
		buf.Read<UInt16>(m.SPR);
		buf.Read<UInt16>(m.STR);
		buf.Read<UInt16>(m.CON);
		buf.Read<UInt16>(m.AGI);
	}
	if (m_updateMask.IsBit(attrib2Mask))
	{
		buf.Read<UInt16>(m.ATK);
		buf.Read<UInt16>(m.DEF);
		buf.Read<UInt16>(m.HIT);
		buf.Read<UInt16>(m.EVA);
		buf.Read<UInt16>(m.POW);
		buf.Read<UInt16>(m.SPD);
	}
	if (m_updateMask.IsBit(attrPointMask))
	{
		buf.Read<UInt16>(m.attrPoint);
	}
	if (m_updateMask.IsBit(HPMask))
	{
		buf.Read<UInt16>(m.HP);
		buf.Read<UInt16>(m.mHP);
		buf.Read<UInt16>(m.maxHP);
	}
	if (m_updateMask.IsBit(SPMask))
	{
		buf.Read<UInt16>(m.SP);
		buf.Read<UInt16>(m.mSP);
		buf.Read<UInt16>(m.maxSP);
	}
	if (m_updateMask.IsBit(DPMask))
	{
		buf.Read<UInt16>(m.DP);
		buf.Read<UInt16>(m.mDP);
		buf.Read<UInt16>(m.maxDP);
	}
	if (m_updateMask.IsBit(WCMask))
	{
		buf.Read<UInt16>(m.WC);
		buf.Read<UInt16>(m.maxWC);
		buf.Read<UInt16>(m.WcExp);
		buf.Read<UInt32>(m.nextWCExp);
	}
	if (m_updateMask.IsBit(VPMask))
	{
		buf.Read<UInt16>(m.VP);
		buf.Read<UInt16>(m.maxVP);
	}
	if (m_updateMask.IsBit(goodnessMask))
	{
		buf.Read<Int32>(m.goodness);
	}
	if (m_updateMask.IsBit(honorMask))
	{
		buf.Read<UInt32>(m.honor);
	}
	if (m_updateMask.IsBit(titleMask))
	{
		buf.Read<String>(m.title);
		buf.Read<UInt16>(m.title_id);
	}
	if (m_updateMask.IsBit(manorMask))
	{
		buf.Read<UInt32>(m.manor);
		buf.Read<UInt32>(m.pettwo_uid);
		buf.Read<UInt32>(m.pettwo_id);
		buf.Read<String>(m.pettwoName);
		buf.Read<UInt16>(m.pettwoColorIndex);
	}
	if (m_updateMask.IsBit(donateMask))
	{
		buf.Read<UInt32>(m.fDonate);
		buf.Read<UInt32>(m.totalFDonate);
		buf.Read<UInt32>(m.mDonate);
		buf.Read<UInt32>(m.totalMDonate);
	}
	if (m_updateMask.IsBit(yuanBaoMask))
	{
		buf.Read<UInt32>(m.yuanBao);
		buf.Read<UInt32>(m.yuanBaoTicket);
	}
	if (m_updateMask.IsBit(moneyMask))
	{
		buf.Read<UInt32>(m.money);
	}
	if (m_updateMask.IsBit(bankMoneyMask))
	{
		buf.Read<UInt32>(m.bankMoney);
	}
	if (m_updateMask.IsBit(SMoneyMask))
	{
		buf.Read<UInt32>(m.SMoney);
	}
	if (m_updateMask.IsBit(partnerMask))
	{
		buf.Read<UInt32>(m.partner_id);
		buf.Read<UInt32>(m.partnerMob_id);
		buf.Read<String>(m.partnerName);
		buf.Read<UInt16>(m.partnerColorIndex);
	}
	if (m_updateMask.IsBit(petMask))
	{
		buf.Read<UInt32>(m.pet_uid);
		buf.Read<UInt32>(m.pet_id);
		buf.Read<String>(m.petName);
		buf.Read<UInt16>(m.petColorIndex);
	}
	if (m_updateMask.IsBit(maxOtherMask))
	{
		buf.Read<UInt8>(m.maxPartner);
		buf.Read<UInt8>(m.maxPet);
	}
	if (m_updateMask.IsBit(styleMask))
	{
		buf.Read<UInt16>(m.photo_id);
		buf.Read<UInt16>(m.hairStyle1);
		buf.Read<UInt16>(m.hairStyle2);
		buf.Read<UInt16>(m.hairColor);
	}
	if (m_updateMask.IsBit(eqWeaponMask))
	{
		buf.Read<UInt32>(m.eq_weapon);
		buf.Read<UInt16>(m.eq_weaponColor);
	}
	if (m_updateMask.IsBit(eqHeadMask))
	{
		buf.Read<UInt32>(m.eq_headBand);
		buf.Read<UInt16>(m.eq_headColor);
	}
	if (m_updateMask.IsBit(eqClothMask))
	{
		buf.Read<UInt32>(m.eq_cloth);
		buf.Read<UInt16>(m.eq_clothColor);
	}
	if (m_updateMask.IsBit(stateMask))
	{
		buf.Read<UInt8>(m.state);
	}
	if (m_updateMask.IsBit(buffMask))
	{
		buf.Read<String>(m.buff);
	}
	if (m_updateMask.IsBit(buffCacheMask))
	{
		buf.Read<String>(m.buffCache);
	}
	if (m_updateMask.IsBit(shortcutMask))
	{
		buf.ReadArray<UInt16>(m.shortcut, 10);
		buf.ReadArray<UInt8>(m.shortcuttype, 10);
	}
	if (m_updateMask.IsBit(partyInfoMask))
	{
		buf.Read<Boolean>(m.hasParty);
		buf.Read<Boolean>(m.isLeader);
		buf.Read<RPCNetID>(m.partyNetID);
	}
	if (m_updateMask.IsBit(PKInfoMask))
	{
		buf.Read<UInt16>(m.PK_Value);
	}
	if (m_updateMask.IsBit(BattleInfoMask))
	{
		buf.Read<Boolean>(m.EnablePK);
	}
	if (m_updateMask.IsBit(SettingInfoMask))
	{
		buf.Read<Boolean>(m.EnableBlockGiven);
		buf.Read<Boolean>(m.EnableBlockParty);
		buf.Read<Boolean>(m.EnableBlockMsg);
		buf.Read<Boolean>(m.EnableBlockTrading);
		buf.Read<Boolean>(m.EnableBlockMakeFriend);
		buf.Read<Boolean>(m.EnableShowEquip);
	}
	if (m_updateMask.IsBit(orgHairStyleMask))
	{
		buf.Read<UInt16>(m.org_hairStyle1);
		buf.Read<UInt16>(m.org_hairColor);
	}
	if (m_updateMask.IsBit(orgEqClothMask))
	{
		buf.Read<UInt32>(m.org_eq_cloth);
		buf.Read<UInt16>(m.org_eq_clothColor);
	}
	if (m_updateMask.IsBit(speakCountMask))
	{
		buf.Read<UInt16>(m.speakCount);
	}
	if (m_updateMask.IsBit(offlineTimeMask))
	{
		buf.Read<UInt32>(m.offlineTime);
	}
	if (m_updateMask.IsBit(hawkInfoMask))
	{
		buf.Read<String>(m.hawkName);
	}
	if (m_updateMask.IsBit(TPMask))
	{
		buf.Read<UInt16>(m.TP);
		buf.Read<UInt32>(m.Model_mobid);
	}
	if (m_updateMask.IsBit(speakBlockMask))
	{
		buf.Read<UInt32>(m.speakBlock);
	}
	if (m_updateMask.IsBit(hawkerTimeMask))
	{
		buf.Read<UInt32>(m.hawkerTime);
	}
	if (m_updateMask.IsBit(availBackPackMask))
	{
		buf.Read<UInt8>(m.availBackPack);
	}
	if (m_updateMask.IsBit(availWareHouseMask))
	{
		buf.Read<UInt8>(m.availItemWareHouse);
		buf.Read<UInt8>(m.availPartnerWareHouse);
		buf.Read<UInt8>(m.availPetWareHouse);
	}
	if (m_updateMask.IsBit(followerListMask))
	{
		buf.Read<UInt32Vector>(m.followerList);
	}
	if (m_updateMask.IsBit(PvPScoreSingleMask))
	{
		buf.Read<UInt32>(m.PvPScoreSingle);
	}
	if (m_updateMask.IsBit(unlockTimeMask))
	{
		buf.Read<UInt32>(m.unlock_time);
	}
	if (m_updateMask.IsBit(yuanbaoLockMask))
	{
		buf.Read<Boolean>(m.yuanbaoLock);
	}
	if (m_updateMask.IsBit(bankLockMask))
	{
		buf.Read<Boolean>(m.bankLock);
	}
	if (m_updateMask.IsBit(SiblingIdMask))
	{
		buf.Read<UInt32>(m.sibling_id);
	}
	if (m_updateMask.IsBit(markMask))
	{
		buf.Read<UInt32>(m.mark);
	}
	if (m_updateMask.IsBit(wantedRewardMask))
	{
		buf.Read<UInt32>(m.wantedReward);
	}
	if (m_updateMask.IsBit(guildUidMask))
	{
		buf.Read<UInt32>(m.guildUid);
	}
	if (m_updateMask.IsBit(GenerationAccMask))
	{
		buf.Read<UInt32>(m.iHisGenAcc);
		buf.Read<UInt32>(m.uiCurGenAcc);
	}
}

void Character_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Character_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.acct_id, m.char_id);
	if (IsUpdated(levelInfoMask)) OnUpdateLevelInfo(m.level, m.nextLvExp);
	if (IsUpdated(expMask)) OnUpdateExp(m.exp);
	if (IsUpdated(attrib1Mask)) OnUpdateAttrib1(m.STA, m.SPR, m.STR, m.CON, m.AGI);
	if (IsUpdated(attrib2Mask)) OnUpdateAttrib2(m.ATK, m.DEF, m.HIT, m.EVA, m.POW, m.SPD);
	if (IsUpdated(attrPointMask)) OnUpdateAttrPoint(m.attrPoint);
	if (IsUpdated(HPMask)) OnUpdateHP(m.HP, m.mHP, m.maxHP);
	if (IsUpdated(SPMask)) OnUpdateSP(m.SP, m.mSP, m.maxSP);
	if (IsUpdated(DPMask)) OnUpdateDP(m.DP, m.mDP, m.maxDP);
	if (IsUpdated(WCMask)) OnUpdateWC(m.WC, m.maxWC, m.WcExp, m.nextWCExp);
	if (IsUpdated(VPMask)) OnUpdateVP(m.VP, m.maxVP);
	if (IsUpdated(goodnessMask)) OnUpdateGoodness(m.goodness);
	if (IsUpdated(honorMask)) OnUpdateHonor(m.honor);
	if (IsUpdated(titleMask)) OnUpdateTitle(m.title, m.title_id);
	if (IsUpdated(manorMask)) OnUpdateManor(m.manor, m.pettwo_uid, m.pettwo_id, m.pettwoName, m.pettwoColorIndex);
	if (IsUpdated(donateMask)) OnUpdateDonate(m.fDonate, m.totalFDonate, m.mDonate, m.totalMDonate);
	if (IsUpdated(yuanBaoMask)) OnUpdateYuanBao(m.yuanBao, m.yuanBaoTicket);
	if (IsUpdated(moneyMask)) OnUpdateMoney(m.money);
	if (IsUpdated(bankMoneyMask)) OnUpdateBankMoney(m.bankMoney);
	if (IsUpdated(SMoneyMask)) OnUpdateSMoney(m.SMoney);
	if (IsUpdated(partnerMask)) OnUpdatePartner(m.partner_id, m.partnerMob_id, m.partnerName, m.partnerColorIndex);
	if (IsUpdated(petMask)) OnUpdatePet(m.pet_uid, m.pet_id, m.petName, m.petColorIndex);
	if (IsUpdated(maxOtherMask)) OnUpdateMaxOther(m.maxPartner, m.maxPet);
	if (IsUpdated(styleMask)) OnUpdateStyle(m.photo_id, m.hairStyle1, m.hairStyle2, m.hairColor);
	if (IsUpdated(eqWeaponMask)) OnUpdateEqWeapon(m.eq_weapon, m.eq_weaponColor);
	if (IsUpdated(eqHeadMask)) OnUpdateEqHead(m.eq_headBand, m.eq_headColor);
	if (IsUpdated(eqClothMask)) OnUpdateEqCloth(m.eq_cloth, m.eq_clothColor);
	if (IsUpdated(stateMask)) OnUpdateState(m.state);
	if (IsUpdated(buffMask)) OnUpdateBuff(m.buff);
	if (IsUpdated(buffCacheMask)) OnUpdateBuffCache(m.buffCache);
	if (IsUpdated(shortcutMask)) OnUpdateShortcut(m.shortcut[0], m.shortcut[1], m.shortcut[2], m.shortcut[3], m.shortcut[4], m.shortcut[5], m.shortcut[6], m.shortcut[7], m.shortcut[8], m.shortcut[9], m.shortcuttype[0], m.shortcuttype[1], m.shortcuttype[2], m.shortcuttype[3], m.shortcuttype[4], m.shortcuttype[5], m.shortcuttype[6], m.shortcuttype[7], m.shortcuttype[8], m.shortcuttype[9]);
	if (IsUpdated(partyInfoMask)) OnUpdatePartyInfo(m.hasParty, m.isLeader, m.partyNetID);
	if (IsUpdated(PKInfoMask)) OnUpdatePKInfo(m.PK_Value);
	if (IsUpdated(BattleInfoMask)) OnUpdateBattleInfo(m.EnablePK);
	if (IsUpdated(SettingInfoMask)) OnUpdateSettingInfo(m.EnableBlockGiven, m.EnableBlockParty, m.EnableBlockMsg, m.EnableBlockTrading, m.EnableBlockMakeFriend, m.EnableShowEquip);
	if (IsUpdated(orgHairStyleMask)) OnUpdateOrgHairStyle(m.org_hairStyle1, m.org_hairColor);
	if (IsUpdated(orgEqClothMask)) OnUpdateOrgEqCloth(m.org_eq_cloth, m.org_eq_clothColor);
	if (IsUpdated(speakCountMask)) OnUpdateSpeakCount(m.speakCount);
	if (IsUpdated(offlineTimeMask)) OnUpdateOfflineTime(m.offlineTime);
	if (IsUpdated(hawkInfoMask)) OnUpdateHawkInfo(m.hawkName);
	if (IsUpdated(TPMask)) OnUpdateTP(m.TP, m.Model_mobid);
	if (IsUpdated(speakBlockMask)) OnUpdateSpeakBlock(m.speakBlock);
	if (IsUpdated(hawkerTimeMask)) OnUpdateHawkerTime(m.hawkerTime);
	if (IsUpdated(availBackPackMask)) OnUpdateAvailBackPack(m.availBackPack);
	if (IsUpdated(availWareHouseMask)) OnUpdateAvailWareHouse(m.availItemWareHouse, m.availPartnerWareHouse, m.availPetWareHouse);
	if (IsUpdated(followerListMask)) OnUpdateFollowerList(m.followerList);
	if (IsUpdated(PvPScoreSingleMask)) OnUpdatePvPScoreSingle(m.PvPScoreSingle);
	if (IsUpdated(unlockTimeMask)) OnUpdateUnlockTime(m.unlock_time);
	if (IsUpdated(yuanbaoLockMask)) OnUpdateYuanbaoLock(m.yuanbaoLock);
	if (IsUpdated(bankLockMask)) OnUpdateBankLock(m.bankLock);
	if (IsUpdated(SiblingIdMask)) OnUpdateSiblingId(m.sibling_id);
	if (IsUpdated(markMask)) OnUpdateMark(m.mark);
	if (IsUpdated(wantedRewardMask)) OnUpdateWantedReward(m.wantedReward);
	if (IsUpdated(guildUidMask)) OnUpdateGuildUid(m.guildUid);
	if (IsUpdated(GenerationAccMask)) OnUpdateGenerationAcc(m.iHisGenAcc, m.uiCurGenAcc);
}

#pragma optimize("", off)
RPCResult Character_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Character_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_Character_CB_RequestJoinBattle:
		{
			BYTE returnFlag;
			pBuf->Read<BYTE>(returnFlag);
			_result = CB_RequestJoinBattle(pPeer, context, returnFlag);
		}
		break;
	case RPC_Character_CB_PKRequest:
		{
			BYTE returnFlag;
			pBuf->Read<BYTE>(returnFlag);
			UInt32 Target_ID;
			pBuf->Read<UInt32>(Target_ID);
			UInt8 teamCount;
			pBuf->Read<UInt8>(teamCount);
			_result = CB_PKRequest(pPeer, context, returnFlag, Target_ID, teamCount);
		}
		break;
	case RPC_Character_CB_SetEnablePK:
		{
			BYTE returnFlag;
			pBuf->Read<BYTE>(returnFlag);
			_result = CB_SetEnablePK(pPeer, context, returnFlag);
		}
		break;
	case RPC_Character_CB_JoinBattle:
		{
			BYTE returnFlag;
			pBuf->Read<BYTE>(returnFlag);
			_result = CB_JoinBattle(pPeer, context, returnFlag);
		}
		break;
	case RPC_Character_CB_CancelAuto:
		{
			_result = CB_CancelAuto(pPeer, context);
		}
		break;
	case RPC_Character_CB_LevelUp:
		{
			_result = CB_LevelUp(pPeer, context);
		}
		break;
	case RPC_Character_CB_RaiseAttrib:
		{
			_result = CB_RaiseAttrib(pPeer, context);
		}
		break;
	case RPC_Character_CB_AddHelper:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = CB_AddHelper(pPeer, context, char_id);
		}
		break;
	case RPC_Character_CB_RejectHelper:
		{
			_result = CB_RejectHelper(pPeer, context);
		}
		break;
	case RPC_Character_CB_LeaveHelpersList:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = CB_LeaveHelpersList(pPeer, context, char_id);
		}
		break;
	case RPC_Character_CB_ClearHelpersList:
		{
			_result = CB_ClearHelpersList(pPeer, context);
		}
		break;
	case RPC_Character_CB_HelpRequestFail:
		{
			_result = CB_HelpRequestFail(pPeer, context);
		}
		break;
	case RPC_Character_CB_WarningMessage:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			Int32 value;
			pBuf->Read<Int32>(value);
			UInt8 showIn;
			pBuf->Read<UInt8>(showIn);
			_result = CB_WarningMessage(pPeer, context, msgIndex, value, showIn);
		}
		break;
	case RPC_Character_CB_SearchChar:
		{
			CharInfoVector CharInfoList;
			pBuf->Read<CharInfoVector>(CharInfoList);
			_result = CB_SearchChar(pPeer, context, CharInfoList);
		}
		break;
	case RPC_Character_CB_OpenWeaponsRankPanel:
		{
			_result = CB_OpenWeaponsRankPanel(pPeer, context);
		}
		break;
	case RPC_Character_CB_SetTarget:
		{
			Boolean bResult;
			pBuf->Read<Boolean>(bResult);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt16 x;
			pBuf->Read<UInt16>(x);
			UInt16 y;
			pBuf->Read<UInt16>(y);
			_result = CB_SetTarget(pPeer, context, bResult, char_id, x, y);
		}
		break;
	case RPC_Character_CB_Error:
		{
			UInt32 errCode;
			pBuf->Read<UInt32>(errCode);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, errCode, errMsg);
		}
		break;
	case RPC_Character_CB_JoinParty:
		{
			NetGroup* party;
			pBuf->Read<NetGroup*>(party);
			_result = CB_JoinParty(pPeer, context, party);
		}
		break;
	case RPC_Character_CB_ChangeDesc:
		{
			NetGroup* party;
			pBuf->Read<NetGroup*>(party);
			_result = CB_ChangeDesc(pPeer, context, party);
		}
		break;
	case RPC_Character_CB_PartyRequestOk:
		{
			String name;
			pBuf->Read<String>(name);
			_result = CB_PartyRequestOk(pPeer, context, name);
		}
		break;
	case RPC_Character_CB_PartyRequestNo:
		{
			UInt16 nState;
			pBuf->Read<UInt16>(nState);
			_result = CB_PartyRequestNo(pPeer, context, nState);
		}
		break;
	case RPC_Character_CB_GetPlayerEquip:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = CB_GetPlayerEquip(pPeer, context, char_id);
		}
		break;
	case RPC_Character_CB_PartyInviteOk:
		{
			String name;
			pBuf->Read<String>(name);
			_result = CB_PartyInviteOk(pPeer, context, name);
		}
		break;
	case RPC_Character_CB_PartyJoinRequest:
		{
			RPCNetID targetId;
			pBuf->Read<RPCNetID>(targetId);
			_result = CB_PartyJoinRequest(pPeer, context, targetId);
		}
		break;
	case RPC_Character_CB_PartyInviteRequest:
		{
			NetGroup* party;
			pBuf->Read<NetGroup*>(party);
			_result = CB_PartyInviteRequest(pPeer, context, party);
		}
		break;
	case RPC_Character_CB_PartyClearRequest:
		{
			RPCNetID targetId;
			pBuf->Read<RPCNetID>(targetId);
			Boolean showMsg;
			pBuf->Read<Boolean>(showMsg);
			_result = CB_PartyClearRequest(pPeer, context, targetId, showMsg);
		}
		break;
	case RPC_Character_CB_PartyClearInvite:
		{
			RPCNetID partyId;
			pBuf->Read<RPCNetID>(partyId);
			Boolean showMsg;
			pBuf->Read<Boolean>(showMsg);
			_result = CB_PartyClearInvite(pPeer, context, partyId, showMsg);
		}
		break;
	case RPC_Character_CB_PartyMessage:
		{
			UInt8 msgType;
			pBuf->Read<UInt8>(msgType);
			bool inviteOrJoin;
			pBuf->Read<bool>(inviteOrJoin);
			String charName;
			pBuf->Read<String>(charName);
			_result = CB_PartyMessage(pPeer, context, msgType, inviteOrJoin, charName);
		}
		break;
	case RPC_Character_CB_SearchParty:
		{
			PartyInfoVector PartyInfoList;
			pBuf->Read<PartyInfoVector>(PartyInfoList);
			_result = CB_SearchParty(pPeer, context, PartyInfoList);
		}
		break;
	case RPC_Character_CB_GetPartyInfo:
		{
			PartyInfoVector PartyInfoList;
			pBuf->Read<PartyInfoVector>(PartyInfoList);
			_result = CB_GetPartyInfo(pPeer, context, PartyInfoList);
		}
		break;
	case RPC_Character_CB_OpenSearchPartyPanel:
		{
			_result = CB_OpenSearchPartyPanel(pPeer, context);
		}
		break;
	case RPC_Character_CB_HandleGuideEvent:
		{
			UInt16 ID;
			pBuf->Read<UInt16>(ID);
			_result = CB_HandleGuideEvent(pPeer, context, ID);
		}
		break;
	case RPC_Character_CB_RespondTest:
		{
			UInt16 playerCount;
			pBuf->Read<UInt16>(playerCount);
			_result = CB_RespondTest(pPeer, context, playerCount);
		}
		break;
	case RPC_Character_CB_LearnSkill:
		{
			UInt16 skill_id;
			pBuf->Read<UInt16>(skill_id);
			BYTE returnFlag;
			pBuf->Read<BYTE>(returnFlag);
			_result = CB_LearnSkill(pPeer, context, skill_id, returnFlag);
		}
		break;
	case RPC_Character_CB_UseSkill:
		{
			UInt16 skill_id;
			pBuf->Read<UInt16>(skill_id);
			BYTE returnFlag;
			pBuf->Read<BYTE>(returnFlag);
			String Msg;
			pBuf->Read<String>(Msg);
			UInt32 SrcID;
			pBuf->Read<UInt32>(SrcID);
			UInt32 TargetID;
			pBuf->Read<UInt32>(TargetID);
			_result = CB_UseSkill(pPeer, context, skill_id, returnFlag, Msg, SrcID, TargetID);
		}
		break;
	case RPC_Character_CB_UpGradeSkill:
		{
			UInt16 skill_id;
			pBuf->Read<UInt16>(skill_id);
			BYTE returnFlag;
			pBuf->Read<BYTE>(returnFlag);
			_result = CB_UpGradeSkill(pPeer, context, skill_id, returnFlag);
		}
		break;
	case RPC_Character_CB_RemoveSkill:
		{
			UInt16 skill_id;
			pBuf->Read<UInt16>(skill_id);
			BYTE returnFlag;
			pBuf->Read<BYTE>(returnFlag);
			_result = CB_RemoveSkill(pPeer, context, skill_id, returnFlag);
		}
		break;
	case RPC_Character_CB_SkillShopLearn:
		{
			BYTE returnFlag;
			pBuf->Read<BYTE>(returnFlag);
			_result = CB_SkillShopLearn(pPeer, context, returnFlag);
		}
		break;
	case RPC_Character_CB_GivenProcess:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_GivenProcess(pPeer, context, success, msgIndex);
		}
		break;
	case RPC_Character_CB_GivenItem:
		{
			Boolean GivenBy;
			pBuf->Read<Boolean>(GivenBy);
			String targetName;
			pBuf->Read<String>(targetName);
			UInt32Vector itemID;
			pBuf->Read<UInt32Vector>(itemID);
			UInt8Vector itemStacks;
			pBuf->Read<UInt8Vector>(itemStacks);
			_result = CB_GivenItem(pPeer, context, GivenBy, targetName, itemID, itemStacks);
		}
		break;
	case RPC_Character_CB_GivenPartner:
		{
			Boolean GivenBy;
			pBuf->Read<Boolean>(GivenBy);
			String targetName;
			pBuf->Read<String>(targetName);
			StringVector PartnerNames;
			pBuf->Read<StringVector>(PartnerNames);
			_result = CB_GivenPartner(pPeer, context, GivenBy, targetName, PartnerNames);
		}
		break;
	case RPC_Character_CB_GivenPet:
		{
			Boolean GivenBy;
			pBuf->Read<Boolean>(GivenBy);
			String targetName;
			pBuf->Read<String>(targetName);
			StringVector PetNames;
			pBuf->Read<StringVector>(PetNames);
			_result = CB_GivenPet(pPeer, context, GivenBy, targetName, PetNames);
		}
		break;
	case RPC_Character_CB_GivenMoney:
		{
			Boolean GivenBy;
			pBuf->Read<Boolean>(GivenBy);
			String targetName;
			pBuf->Read<String>(targetName);
			UInt32 gmoney;
			pBuf->Read<UInt32>(gmoney);
			_result = CB_GivenMoney(pPeer, context, GivenBy, targetName, gmoney);
		}
		break;
	case RPC_Character_CB_UpdateShortCutSuccess:
		{
			_result = CB_UpdateShortCutSuccess(pPeer, context);
		}
		break;
	case RPC_Character_CB_SaveSystemSetting:
		{
			_result = CB_SaveSystemSetting(pPeer, context);
		}
		break;
	case RPC_Character_CB_AskForTrading:
		{
			RPCNetID askId;
			pBuf->Read<RPCNetID>(askId);
			_result = CB_AskForTrading(pPeer, context, askId);
		}
		break;
	case RPC_Character_CB_ReplyReqTrading:
		{
			Boolean trade;
			pBuf->Read<Boolean>(trade);
			_result = CB_ReplyReqTrading(pPeer, context, trade);
		}
		break;
	case RPC_Character_CB_TradingMessage:
		{
			Boolean trade;
			pBuf->Read<Boolean>(trade);
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_TradingMessage(pPeer, context, trade, msgIndex);
		}
		break;
	case RPC_Character_CB_TradingObject:
		{
			NetObject* trading;
			pBuf->Read<NetObject*>(trading);
			_result = CB_TradingObject(pPeer, context, trading);
		}
		break;
	case RPC_Character_CB_TalkToNPC:
		{
			UInt32 npc_id;
			pBuf->Read<UInt32>(npc_id);
			bool bResult;
			pBuf->Read<bool>(bResult);
			_result = CB_TalkToNPC(pPeer, context, npc_id, bResult);
		}
		break;
	case RPC_Character_CB_TitleListUpdate:
		{
			UInt16Vector title_id_list;
			pBuf->Read<UInt16Vector>(title_id_list);
			StringVector title_string_list;
			pBuf->Read<StringVector>(title_string_list);
			UInt8Vector updateState;
			pBuf->Read<UInt8Vector>(updateState);
			_result = CB_TitleListUpdate(pPeer, context, title_id_list, title_string_list, updateState);
		}
		break;
	case RPC_Character_CB_AntiHack:
		{
			ByteArray image;
			pBuf->Read<ByteArray>(image);
			AntiHackData data;
			pBuf->Read<AntiHackData>(data);
			_result = CB_AntiHack(pPeer, context, image, data);
		}
		break;
	case RPC_Character_CB_ReportAntiHack:
		{
			Boolean is_right;
			pBuf->Read<Boolean>(is_right);
			UInt8 cwrong;
			pBuf->Read<UInt8>(cwrong);
			UInt16 reward;
			pBuf->Read<UInt16>(reward);
			_result = CB_ReportAntiHack(pPeer, context, is_right, cwrong, reward);
		}
		break;
	case RPC_Character_CB_DepositInBank:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			_result = CB_DepositInBank(pPeer, context, success);
		}
		break;
	case RPC_Character_CB_WithdrawFmBank:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			_result = CB_WithdrawFmBank(pPeer, context, success);
		}
		break;
	case RPC_Character_CB_StartHawk:
		{
			NetGroup* hawkGroup;
			pBuf->Read<NetGroup*>(hawkGroup);
			UInt8 successIndex;
			pBuf->Read<UInt8>(successIndex);
			_result = CB_StartHawk(pPeer, context, hawkGroup, successIndex);
		}
		break;
	case RPC_Character_CB_GetHawk:
		{
			Boolean self;
			pBuf->Read<Boolean>(self);
			NetGroup* hawkGroup;
			pBuf->Read<NetGroup*>(hawkGroup);
			NetGroup* itemGroup;
			pBuf->Read<NetGroup*>(itemGroup);
			NetGroup* partnerGroup;
			pBuf->Read<NetGroup*>(partnerGroup);
			UInt16Vector partnerSkillID;
			pBuf->Read<UInt16Vector>(partnerSkillID);
			NetGroup* petGroup;
			pBuf->Read<NetGroup*>(petGroup);
			UInt16Vector petSkillID;
			pBuf->Read<UInt16Vector>(petSkillID);
			_result = CB_GetHawk(pPeer, context, self, hawkGroup, itemGroup, partnerGroup, partnerSkillID, petGroup, petSkillID);
		}
		break;
	case RPC_Character_CB_HawkMessage:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_HawkMessage(pPeer, context, msgIndex);
		}
		break;
	case RPC_Character_CB_HawkHistory:
		{
			HawkerHistoryVector historys;
			pBuf->Read<HawkerHistoryVector>(historys);
			StringVector itemNames;
			pBuf->Read<StringVector>(itemNames);
			_result = CB_HawkHistory(pPeer, context, historys, itemNames);
		}
		break;
	case RPC_Character_CB_UpdateHawkHistory:
		{
			HawkerHistoryVector historys;
			pBuf->Read<HawkerHistoryVector>(historys);
			StringVector itemNames;
			pBuf->Read<StringVector>(itemNames);
			UInt32Vector deleteHistoryID;
			pBuf->Read<UInt32Vector>(deleteHistoryID);
			_result = CB_UpdateHawkHistory(pPeer, context, historys, itemNames, deleteHistoryID);
		}
		break;
	case RPC_Character_CB_SendGrpMessage:
		{
			UInt8 Result;
			pBuf->Read<UInt8>(Result);
			UInt32 resultFlag;
			pBuf->Read<UInt32>(resultFlag);
			_result = CB_SendGrpMessage(pPeer, context, Result, resultFlag);
		}
		break;
	case RPC_Character_CB_OpenCreateGuild:
		{
			_result = CB_OpenCreateGuild(pPeer, context);
		}
		break;
	case RPC_Character_CB_AwardMsg:
		{
			String msg;
			pBuf->Read<String>(msg);
			_result = CB_AwardMsg(pPeer, context, msg);
		}
		break;
	case RPC_Character_CB_OpenGiftBox:
		{
			NetGroup* giftBox;
			pBuf->Read<NetGroup*>(giftBox);
			_result = CB_OpenGiftBox(pPeer, context, giftBox);
		}
		break;
	case RPC_Character_CB_AddictionWarning:
		{
			String warnMsg;
			pBuf->Read<String>(warnMsg);
			_result = CB_AddictionWarning(pPeer, context, warnMsg);
		}
		break;
	case RPC_Character_CB_AddictCurrentStatus:
		{
			String statusMsg;
			pBuf->Read<String>(statusMsg);
			_result = CB_AddictCurrentStatus(pPeer, context, statusMsg);
		}
		break;
	case RPC_Character_CB_AddictionShutDown:
		{
			UInt32 accOffTime;
			pBuf->Read<UInt32>(accOffTime);
			_result = CB_AddictionShutDown(pPeer, context, accOffTime);
		}
		break;
	case RPC_Character_CB_PlyInterestDwn:
		{
			UInt8 interest;
			pBuf->Read<UInt8>(interest);
			_result = CB_PlyInterestDwn(pPeer, context, interest);
		}
		break;
	case RPC_Character_CB_UseItemCharacter:
		{
			UInt32 item_id;
			pBuf->Read<UInt32>(item_id);
			UInt32 SrcID;
			pBuf->Read<UInt32>(SrcID);
			UInt32 TargetID;
			pBuf->Read<UInt32>(TargetID);
			_result = CB_UseItemCharacter(pPeer, context, item_id, SrcID, TargetID);
		}
		break;
	case RPC_Character_CB_AskEnterQuestion:
		{
			_result = CB_AskEnterQuestion(pPeer, context);
		}
		break;
	case RPC_Character_CB_EnterQuestion:
		{
			NetObject* pQuestion;
			pBuf->Read<NetObject*>(pQuestion);
			_result = CB_EnterQuestion(pPeer, context, pQuestion);
		}
		break;
	case RPC_Character_CB_ExitQuestion:
		{
			_result = CB_ExitQuestion(pPeer, context);
		}
		break;
	case RPC_Character_CB_AnswerQuestion:
		{
			UInt8 index;
			pBuf->Read<UInt8>(index);
			UInt32 CharID;
			pBuf->Read<UInt32>(CharID);
			_result = CB_AnswerQuestion(pPeer, context, index, CharID);
		}
		break;
	case RPC_Character_CB_OpenWareHouse:
		{
			NetGroup* items;
			pBuf->Read<NetGroup*>(items);
			NetGroup* partners;
			pBuf->Read<NetGroup*>(partners);
			NetGroup* pets;
			pBuf->Read<NetGroup*>(pets);
			NetGroup* partnerSkillGroup;
			pBuf->Read<NetGroup*>(partnerSkillGroup);
			NetGroup* petSkillGroup;
			pBuf->Read<NetGroup*>(petSkillGroup);
			_result = CB_OpenWareHouse(pPeer, context, items, partners, pets, partnerSkillGroup, petSkillGroup);
		}
		break;
	case RPC_Character_CB_AssetProtectResult:
		{
			UInt8 result;
			pBuf->Read<UInt8>(result);
			UInt32 index;
			pBuf->Read<UInt32>(index);
			UInt8 assetType;
			pBuf->Read<UInt8>(assetType);
			_result = CB_AssetProtectResult(pPeer, context, result, index, assetType);
		}
		break;
	case RPC_Character_CB_AssetReleaseLockResult:
		{
			UInt8 result;
			pBuf->Read<UInt8>(result);
			UInt32 index;
			pBuf->Read<UInt32>(index);
			UInt8 assetType;
			pBuf->Read<UInt8>(assetType);
			_result = CB_AssetReleaseLockResult(pPeer, context, result, index, assetType);
		}
		break;
	case RPC_Character_CB_SetAssetPassword:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			_result = CB_SetAssetPassword(pPeer, context, success);
		}
		break;
	case RPC_Character_CB_ForceResetAssetPassword:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			_result = CB_ForceResetAssetPassword(pPeer, context, success);
		}
		break;
	case RPC_Character_CB_SetRobot:
		{
			Boolean bRobot;
			pBuf->Read<Boolean>(bRobot);
			String robotScript;
			pBuf->Read<String>(robotScript);
			_result = CB_SetRobot(pPeer, context, bRobot, robotScript);
		}
		break;
	case RPC_Character_CB_RobotMapAction:
		{
			String func;
			pBuf->Read<String>(func);
			RPCNetID mapNetId;
			pBuf->Read<RPCNetID>(mapNetId);
			RPCNetID charNetId;
			pBuf->Read<RPCNetID>(charNetId);
			RPCNetID otherNetId;
			pBuf->Read<RPCNetID>(otherNetId);
			_result = CB_RobotMapAction(pPeer, context, func, mapNetId, charNetId, otherNetId);
		}
		break;
	case RPC_Character_CB_RobotBattleTurn:
		{
			String func;
			pBuf->Read<String>(func);
			RPCNetID battleNetId;
			pBuf->Read<RPCNetID>(battleNetId);
			RPCNetID charNetId;
			pBuf->Read<RPCNetID>(charNetId);
			RPCNetID batCharNetId;
			pBuf->Read<RPCNetID>(batCharNetId);
			RPCNetID partnerNetId;
			pBuf->Read<RPCNetID>(partnerNetId);
			_result = CB_RobotBattleTurn(pPeer, context, func, battleNetId, charNetId, batCharNetId, partnerNetId);
		}
		break;
	case RPC_Character_CB_SetAutoWalkHitBoss:
		{
			Boolean hit;
			pBuf->Read<Boolean>(hit);
			_result = CB_SetAutoWalkHitBoss(pPeer, context, hit);
		}
		break;
	case RPC_Character_CB_DonateResult:
		{
			UInt32 donate;
			pBuf->Read<UInt32>(donate);
			UInt8 returnFlag;
			pBuf->Read<UInt8>(returnFlag);
			UInt32 donateMoney;
			pBuf->Read<UInt32>(donateMoney);
			_result = CB_DonateResult(pPeer, context, donate, returnFlag, donateMoney);
		}
		break;
	case RPC_Character_CB_GetPartnerCollectionList:
		{
			UInt32Vector PartnerIDs;
			pBuf->Read<UInt32Vector>(PartnerIDs);
			_result = CB_GetPartnerCollectionList(pPeer, context, PartnerIDs);
		}
		break;
	case RPC_Character_CB_UsePartnerCollectionBuff:
		{
			BYTE returnFlag;
			pBuf->Read<BYTE>(returnFlag);
			_result = CB_UsePartnerCollectionBuff(pPeer, context, returnFlag);
		}
		break;
	case RPC_Character_CB_AddPartnerCollection:
		{
			UInt32 MOBid;
			pBuf->Read<UInt32>(MOBid);
			BYTE returnFlag;
			pBuf->Read<BYTE>(returnFlag);
			_result = CB_AddPartnerCollection(pPeer, context, MOBid, returnFlag);
		}
		break;
	case RPC_Character_CB_ChangeBetweenYBandYBT:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_ChangeBetweenYBandYBT(pPeer, context, msgIndex);
		}
		break;
	case RPC_Character_CB_AddSellYuanBao:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_AddSellYuanBao(pPeer, context, msgIndex);
		}
		break;
	case RPC_Character_CB_BuySellYuanBao:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_BuySellYuanBao(pPeer, context, msgIndex);
		}
		break;
	case RPC_Character_CB_GetAllMoneyAndYBT:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_GetAllMoneyAndYBT(pPeer, context, msgIndex);
		}
		break;
	case RPC_Character_CB_RequestTargetEquip:
		{
			NetGroup* items;
			pBuf->Read<NetGroup*>(items);
			_result = CB_RequestTargetEquip(pPeer, context, items);
		}
		break;
	case RPC_Character_CB_RelationSystem:
		{
			NetObject* pRelationSystem;
			pBuf->Read<NetObject*>(pRelationSystem);
			_result = CB_RelationSystem(pPeer, context, pRelationSystem);
		}
		break;
	case RPC_Character_CB_SetTargetNameFormHawk:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			String char_name;
			pBuf->Read<String>(char_name);
			Boolean isonline;
			pBuf->Read<Boolean>(isonline);
			_result = CB_SetTargetNameFormHawk(pPeer, context, char_id, char_name, isonline);
		}
		break;
	case RPC_Character_CB_InitTargetHawkConcernState:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = CB_InitTargetHawkConcernState(pPeer, context, char_id);
		}
		break;
	case RPC_Character_CB_ModifyLineup:
		{
			UInt16 uiLineupId;
			pBuf->Read<UInt16>(uiLineupId);
			_result = CB_ModifyLineup(pPeer, context, uiLineupId);
		}
		break;
	case RPC_Character_CB_ModifyLineupKeyPos:
		{
			int iKeyPos;
			pBuf->Read<int>(iKeyPos);
			_result = CB_ModifyLineupKeyPos(pPeer, context, iKeyPos);
		}
		break;
	case RPC_Character_CB_ForgiveLineup:
		{
			int iResault;
			pBuf->Read<int>(iResault);
			_result = CB_ForgiveLineup(pPeer, context, iResault);
		}
		break;
	case RPC_Character_CB_SetChangePos:
		{
			int iRetCode;
			pBuf->Read<int>(iRetCode);
			UInt8 ui8PreIndex;
			pBuf->Read<UInt8>(ui8PreIndex);
			UInt8 ui8DestIndex;
			pBuf->Read<UInt8>(ui8DestIndex);
			_result = CB_SetChangePos(pPeer, context, iRetCode, ui8PreIndex, ui8DestIndex);
		}
		break;
	case RPC_Character_CB_UpdateWantedReward:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			Int32 money;
			pBuf->Read<Int32>(money);
			Boolean result;
			pBuf->Read<Boolean>(result);
			_result = CB_UpdateWantedReward(pPeer, context, targetId, money, result);
		}
		break;
	case RPC_Character_CB_JailDelivery:
		{
			_result = CB_JailDelivery(pPeer, context);
		}
		break;
	case RPC_Character_CB_WantedBail:
		{
			_result = CB_WantedBail(pPeer, context);
		}
		break;
	case RPC_Character_CB_WantedTrace:
		{
			String name;
			pBuf->Read<String>(name);
			UInt32 lineId;
			pBuf->Read<UInt32>(lineId);
			UInt32 mapId;
			pBuf->Read<UInt32>(mapId);
			UInt32 mapX;
			pBuf->Read<UInt32>(mapX);
			UInt32 mapY;
			pBuf->Read<UInt32>(mapY);
			_result = CB_WantedTrace(pPeer, context, name, lineId, mapId, mapX, mapY);
		}
		break;
	case RPC_Character_CB_RequestWantedTrace:
		{
			_result = CB_RequestWantedTrace(pPeer, context);
		}
		break;
	case RPC_Character_CB_GetGenerationGroup:
		{
			NetGroup* pGntGrp;
			pBuf->Read<NetGroup*>(pGntGrp);
			_result = CB_GetGenerationGroup(pPeer, context, pGntGrp);
		}
		break;
	case RPC_Character_CB_RequestGenerationTrans:
		{
			UInt32 uiRequstId;
			pBuf->Read<UInt32>(uiRequstId);
			_result = CB_RequestGenerationTrans(pPeer, context, uiRequstId);
		}
		break;
	case RPC_Character_CB_RequestGenerationResult:
		{
			int iResultCode;
			pBuf->Read<int>(iResultCode);
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			_result = CB_RequestGenerationResult(pPeer, context, iResultCode, uiRelatedId);
		}
		break;
	case RPC_Character_CB_DeleteGeneration:
		{
			int iResultCode;
			pBuf->Read<int>(iResultCode);
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			_result = CB_DeleteGeneration(pPeer, context, iResultCode, uiRelatedId);
		}
		break;
	case RPC_Character_CB_ChangeGenerationType:
		{
			int iType;
			pBuf->Read<int>(iType);
			_result = CB_ChangeGenerationType(pPeer, context, iType);
		}
		break;
	case RPC_Character_CB_UpdateGenerationAcc:
		{
			UInt32 uiCurAcc;
			pBuf->Read<UInt32>(uiCurAcc);
			UInt32 uiHisAcc;
			pBuf->Read<UInt32>(uiHisAcc);
			_result = CB_UpdateGenerationAcc(pPeer, context, uiCurAcc, uiHisAcc);
		}
		break;
	case RPC_Character_CB_BugAccShopItem:
		{
			int iRetCode;
			pBuf->Read<int>(iRetCode);
			UInt32 iIndex;
			pBuf->Read<UInt32>(iIndex);
			_result = CB_BugAccShopItem(pPeer, context, iRetCode, iIndex);
		}
		break;
	case RPC_Character_CB_GenerationGainAcc:
		{
			UInt32 iGainedAcc;
			pBuf->Read<UInt32>(iGainedAcc);
			_result = CB_GenerationGainAcc(pPeer, context, iGainedAcc);
		}
		break;
	case RPC_Character_CB_BuyPointsShopItem:
		{
			int iRetCode;
			pBuf->Read<int>(iRetCode);
			UInt32 uiPoints;
			pBuf->Read<UInt32>(uiPoints);
			_result = CB_BuyPointsShopItem(pPeer, context, iRetCode, uiPoints);
		}
		break;
	case RPC_Character_CB_OpenBusinessLog:
		{
			_result = CB_OpenBusinessLog(pPeer, context);
		}
		break;
	case RPC_Character_CB_SetMarriageNpcAction:
		{
			UInt32 npc_uid;
			pBuf->Read<UInt32>(npc_uid);
			UInt8 atcion;
			pBuf->Read<UInt8>(atcion);
			UInt8 direction;
			pBuf->Read<UInt8>(direction);
			_result = CB_SetMarriageNpcAction(pPeer, context, npc_uid, atcion, direction);
		}
		break;
	case RPC_Character_CB_ShowMarriageEff:
		{
			UInt8 state;
			pBuf->Read<UInt8>(state);
			UInt32 husbandID;
			pBuf->Read<UInt32>(husbandID);
			UInt32 wifeID;
			pBuf->Read<UInt32>(wifeID);
			_result = CB_ShowMarriageEff(pPeer, context, state, husbandID, wifeID);
		}
		break;
	case RPC_Character_CB_ResetChangeLine:
		{
			_result = CB_ResetChangeLine(pPeer, context);
		}
		break;
	case RPC_Character_CB_PetUseSkillMsg:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			_result = CB_PetUseSkillMsg(pPeer, context, Msg);
		}
		break;
	case RPC_Character_CB_LeaveTeamTemporarily:
		{
			UInt32 uRetCode;
			pBuf->Read<UInt32>(uRetCode);
			UInt32 uId;
			pBuf->Read<UInt32>(uId);
			_result = CB_LeaveTeamTemporarily(pPeer, context, uRetCode, uId);
		}
		break;
	case RPC_Character_CB_GetPartyInfoWhenJumpMap:
		{
			NetGroup* pParty;
			pBuf->Read<NetGroup*>(pParty);
			_result = CB_GetPartyInfoWhenJumpMap(pPeer, context, pParty);
		}
		break;
	case RPC_Character_CB_ReturnParty:
		{
			UInt32 uRetCode;
			pBuf->Read<UInt32>(uRetCode);
			NetObject* pChar;
			pBuf->Read<NetObject*>(pChar);
			_result = CB_ReturnParty(pPeer, context, uRetCode, pChar);
		}
		break;
	case RPC_Character_CB_RefreshPartyCharInfo:
		{
			NetObject* pPartyCharInfo;
			pBuf->Read<NetObject*>(pPartyCharInfo);
			_result = CB_RefreshPartyCharInfo(pPeer, context, pPartyCharInfo);
		}
		break;
	case RPC_Character_CB_RestorePartyAsTemp:
		{
			NetGroup* pParty;
			pBuf->Read<NetGroup*>(pParty);
			_result = CB_RestorePartyAsTemp(pPeer, context, pParty);
		}
		break;
	case RPC_Character_CB_SetChangeModelSkillGroup:
		{
			NetGroup* skillGroup;
			pBuf->Read<NetGroup*>(skillGroup);
			_result = CB_SetChangeModelSkillGroup(pPeer, context, skillGroup);
		}
		break;
	case RPC_Character_CB_ShowIncognitoBoxEff:
		{
			UInt16 posx;
			pBuf->Read<UInt16>(posx);
			UInt16 posy;
			pBuf->Read<UInt16>(posy);
			UInt8 effType;
			pBuf->Read<UInt8>(effType);
			_result = CB_ShowIncognitoBoxEff(pPeer, context, posx, posy, effType);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Character_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Character_UP_STATE:
			_result = RPCDecode<265173520>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RequestJoinBattle:
			_result = RPCDecode<884230030>(pPeer, pBuf);
		break;
	case RPC_Character_CB_PKRequest:
			_result = RPCDecode<1392330330>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SetEnablePK:
			_result = RPCDecode<472343456>(pPeer, pBuf);
		break;
	case RPC_Character_CB_JoinBattle:
			_result = RPCDecode<977666519>(pPeer, pBuf);
		break;
	case RPC_Character_CB_CancelAuto:
			_result = RPCDecode<1666675435>(pPeer, pBuf);
		break;
	case RPC_Character_CB_LevelUp:
			_result = RPCDecode<2106011260>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RaiseAttrib:
			_result = RPCDecode<787505021>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AddHelper:
			_result = RPCDecode<272109665>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RejectHelper:
			_result = RPCDecode<825713249>(pPeer, pBuf);
		break;
	case RPC_Character_CB_LeaveHelpersList:
			_result = RPCDecode<1499016738>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ClearHelpersList:
			_result = RPCDecode<1043758759>(pPeer, pBuf);
		break;
	case RPC_Character_CB_HelpRequestFail:
			_result = RPCDecode<1840727934>(pPeer, pBuf);
		break;
	case RPC_Character_CB_WarningMessage:
			_result = RPCDecode<440934718>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SearchChar:
			_result = RPCDecode<708988917>(pPeer, pBuf);
		break;
	case RPC_Character_CB_OpenWeaponsRankPanel:
			_result = RPCDecode<1924562885>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SetTarget:
			_result = RPCDecode<944992856>(pPeer, pBuf);
		break;
	case RPC_Character_CB_Error:
			_result = RPCDecode<2004365301>(pPeer, pBuf);
		break;
	case RPC_Character_CB_JoinParty:
			_result = RPCDecode<2073589861>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ChangeDesc:
			_result = RPCDecode<2016682674>(pPeer, pBuf);
		break;
	case RPC_Character_CB_PartyRequestOk:
			_result = RPCDecode<451881969>(pPeer, pBuf);
		break;
	case RPC_Character_CB_PartyRequestNo:
			_result = RPCDecode<611017579>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GetPlayerEquip:
			_result = RPCDecode<1586313843>(pPeer, pBuf);
		break;
	case RPC_Character_CB_PartyInviteOk:
			_result = RPCDecode<1788162777>(pPeer, pBuf);
		break;
	case RPC_Character_CB_PartyJoinRequest:
			_result = RPCDecode<263587870>(pPeer, pBuf);
		break;
	case RPC_Character_CB_PartyInviteRequest:
			_result = RPCDecode<408096850>(pPeer, pBuf);
		break;
	case RPC_Character_CB_PartyClearRequest:
			_result = RPCDecode<1680621049>(pPeer, pBuf);
		break;
	case RPC_Character_CB_PartyClearInvite:
			_result = RPCDecode<1617130850>(pPeer, pBuf);
		break;
	case RPC_Character_CB_PartyMessage:
			_result = RPCDecode<164526723>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SearchParty:
			_result = RPCDecode<1868450067>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GetPartyInfo:
			_result = RPCDecode<788955121>(pPeer, pBuf);
		break;
	case RPC_Character_CB_OpenSearchPartyPanel:
			_result = RPCDecode<316170255>(pPeer, pBuf);
		break;
	case RPC_Character_CB_HandleGuideEvent:
			_result = RPCDecode<1857582614>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RespondTest:
			_result = RPCDecode<719536770>(pPeer, pBuf);
		break;
	case RPC_Character_CB_LearnSkill:
			_result = RPCDecode<1640973944>(pPeer, pBuf);
		break;
	case RPC_Character_CB_UseSkill:
			_result = RPCDecode<440018441>(pPeer, pBuf);
		break;
	case RPC_Character_CB_UpGradeSkill:
			_result = RPCDecode<101261627>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RemoveSkill:
			_result = RPCDecode<301232851>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SkillShopLearn:
			_result = RPCDecode<19806500>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GivenProcess:
			_result = RPCDecode<1341011045>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GivenItem:
			_result = RPCDecode<2120963231>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GivenPartner:
			_result = RPCDecode<1958285418>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GivenPet:
			_result = RPCDecode<57557994>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GivenMoney:
			_result = RPCDecode<1349417469>(pPeer, pBuf);
		break;
	case RPC_Character_CB_UpdateShortCutSuccess:
			_result = RPCDecode<1195318687>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SaveSystemSetting:
			_result = RPCDecode<1417040156>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AskForTrading:
			_result = RPCDecode<1983474478>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ReplyReqTrading:
			_result = RPCDecode<753271876>(pPeer, pBuf);
		break;
	case RPC_Character_CB_TradingMessage:
			_result = RPCDecode<1233933636>(pPeer, pBuf);
		break;
	case RPC_Character_CB_TradingObject:
			_result = RPCDecode<1591901068>(pPeer, pBuf);
		break;
	case RPC_Character_CB_TalkToNPC:
			_result = RPCDecode<1600547679>(pPeer, pBuf);
		break;
	case RPC_Character_CB_TitleListUpdate:
			_result = RPCDecode<174658801>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AntiHack:
			_result = RPCDecode<1582920557>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ReportAntiHack:
			_result = RPCDecode<1776368642>(pPeer, pBuf);
		break;
	case RPC_Character_CB_DepositInBank:
			_result = RPCDecode<294275337>(pPeer, pBuf);
		break;
	case RPC_Character_CB_WithdrawFmBank:
			_result = RPCDecode<1436378679>(pPeer, pBuf);
		break;
	case RPC_Character_CB_StartHawk:
			_result = RPCDecode<1480197420>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GetHawk:
			_result = RPCDecode<1847356866>(pPeer, pBuf);
		break;
	case RPC_Character_CB_HawkMessage:
			_result = RPCDecode<664404854>(pPeer, pBuf);
		break;
	case RPC_Character_CB_HawkHistory:
			_result = RPCDecode<895776164>(pPeer, pBuf);
		break;
	case RPC_Character_CB_UpdateHawkHistory:
			_result = RPCDecode<853112723>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SendGrpMessage:
			_result = RPCDecode<1159035620>(pPeer, pBuf);
		break;
	case RPC_Character_CB_OpenCreateGuild:
			_result = RPCDecode<850605881>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AwardMsg:
			_result = RPCDecode<1389179735>(pPeer, pBuf);
		break;
	case RPC_Character_CB_OpenGiftBox:
			_result = RPCDecode<1802335796>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AddictionWarning:
			_result = RPCDecode<161423045>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AddictCurrentStatus:
			_result = RPCDecode<1538414089>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AddictionShutDown:
			_result = RPCDecode<733353603>(pPeer, pBuf);
		break;
	case RPC_Character_CB_PlyInterestDwn:
			_result = RPCDecode<551160062>(pPeer, pBuf);
		break;
	case RPC_Character_CB_UseItemCharacter:
			_result = RPCDecode<1531910651>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AskEnterQuestion:
			_result = RPCDecode<1674016061>(pPeer, pBuf);
		break;
	case RPC_Character_CB_EnterQuestion:
			_result = RPCDecode<1280472680>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ExitQuestion:
			_result = RPCDecode<1821640660>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AnswerQuestion:
			_result = RPCDecode<605690981>(pPeer, pBuf);
		break;
	case RPC_Character_CB_OpenWareHouse:
			_result = RPCDecode<1243417419>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AssetProtectResult:
			_result = RPCDecode<841229587>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AssetReleaseLockResult:
			_result = RPCDecode<580604749>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SetAssetPassword:
			_result = RPCDecode<1607548830>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ForceResetAssetPassword:
			_result = RPCDecode<185282243>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SetRobot:
			_result = RPCDecode<270298762>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RobotMapAction:
			_result = RPCDecode<1022762810>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RobotBattleTurn:
			_result = RPCDecode<1967736293>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SetAutoWalkHitBoss:
			_result = RPCDecode<681538378>(pPeer, pBuf);
		break;
	case RPC_Character_CB_DonateResult:
			_result = RPCDecode<997279627>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GetPartnerCollectionList:
			_result = RPCDecode<1794267068>(pPeer, pBuf);
		break;
	case RPC_Character_CB_UsePartnerCollectionBuff:
			_result = RPCDecode<1430018473>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AddPartnerCollection:
			_result = RPCDecode<944062561>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ChangeBetweenYBandYBT:
			_result = RPCDecode<1247889129>(pPeer, pBuf);
		break;
	case RPC_Character_CB_AddSellYuanBao:
			_result = RPCDecode<1987536093>(pPeer, pBuf);
		break;
	case RPC_Character_CB_BuySellYuanBao:
			_result = RPCDecode<1081898104>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GetAllMoneyAndYBT:
			_result = RPCDecode<1673862953>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RequestTargetEquip:
			_result = RPCDecode<1645497323>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RelationSystem:
			_result = RPCDecode<1094416333>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SetTargetNameFormHawk:
			_result = RPCDecode<1254319356>(pPeer, pBuf);
		break;
	case RPC_Character_CB_InitTargetHawkConcernState:
			_result = RPCDecode<2042908018>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ModifyLineup:
			_result = RPCDecode<1567647451>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ModifyLineupKeyPos:
			_result = RPCDecode<969433236>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ForgiveLineup:
			_result = RPCDecode<53431052>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SetChangePos:
			_result = RPCDecode<765602416>(pPeer, pBuf);
		break;
	case RPC_Character_CB_UpdateWantedReward:
			_result = RPCDecode<1451180764>(pPeer, pBuf);
		break;
	case RPC_Character_CB_JailDelivery:
			_result = RPCDecode<431580735>(pPeer, pBuf);
		break;
	case RPC_Character_CB_WantedBail:
			_result = RPCDecode<1406492635>(pPeer, pBuf);
		break;
	case RPC_Character_CB_WantedTrace:
			_result = RPCDecode<609051308>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RequestWantedTrace:
			_result = RPCDecode<683047974>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GetGenerationGroup:
			_result = RPCDecode<474378968>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RequestGenerationTrans:
			_result = RPCDecode<266625076>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RequestGenerationResult:
			_result = RPCDecode<1510801710>(pPeer, pBuf);
		break;
	case RPC_Character_CB_DeleteGeneration:
			_result = RPCDecode<1402830072>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ChangeGenerationType:
			_result = RPCDecode<1219760122>(pPeer, pBuf);
		break;
	case RPC_Character_CB_UpdateGenerationAcc:
			_result = RPCDecode<270404594>(pPeer, pBuf);
		break;
	case RPC_Character_CB_BugAccShopItem:
			_result = RPCDecode<829967264>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GenerationGainAcc:
			_result = RPCDecode<1728914333>(pPeer, pBuf);
		break;
	case RPC_Character_CB_BuyPointsShopItem:
			_result = RPCDecode<947990062>(pPeer, pBuf);
		break;
	case RPC_Character_CB_OpenBusinessLog:
			_result = RPCDecode<1214472072>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SetMarriageNpcAction:
			_result = RPCDecode<1208045910>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ShowMarriageEff:
			_result = RPCDecode<2102293522>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ResetChangeLine:
			_result = RPCDecode<1832715624>(pPeer, pBuf);
		break;
	case RPC_Character_CB_PetUseSkillMsg:
			_result = RPCDecode<1198619385>(pPeer, pBuf);
		break;
	case RPC_Character_CB_LeaveTeamTemporarily:
			_result = RPCDecode<1326673354>(pPeer, pBuf);
		break;
	case RPC_Character_CB_GetPartyInfoWhenJumpMap:
			_result = RPCDecode<2144671047>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ReturnParty:
			_result = RPCDecode<993791940>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RefreshPartyCharInfo:
			_result = RPCDecode<526338154>(pPeer, pBuf);
		break;
	case RPC_Character_CB_RestorePartyAsTemp:
			_result = RPCDecode<1958242924>(pPeer, pBuf);
		break;
	case RPC_Character_CB_SetChangeModelSkillGroup:
			_result = RPCDecode<1810517220>(pPeer, pBuf);
		break;
	case RPC_Character_CB_ShowIncognitoBoxEff:
			_result = RPCDecode<236878823>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
