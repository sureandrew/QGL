///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_CHARACTER_PROXY_H_
#define _RPC_CHARACTER_PROXY_H_

#include "Common/RPCFunc.h"
#include "MapActor.h"
#include "Common/common_binary.h"
struct HawkerItemInfo
{
	UInt32 item_uid;
	UInt32 item_id;
	UInt8 item_stack;
	UInt32 item_price;
	UInt8 item_index;
};
struct HawkerHistory
{
	UInt32 historyId;
	UInt32 char_id;		
	UInt8 itemType;
	UInt8 stack;
	UInt32 totalPrice;
	UInt32 tax;
	UInt32 historyDate;
};
typedef StlVector<HawkerItemInfo, ManagedAllocator<HawkerItemInfo> > HawkerItemInfoVector;
typedef StlVector<HawkerHistory, ManagedAllocator<HawkerHistory> > HawkerHistoryVector;
typedef StlVector<String, ManagedAllocator<String> > StringVector;
typedef StlVector<Byte, ManagedAllocator<Byte> > ByteVector; 
#include "RPCEnum/Character_enum.h"

class Character_Proxy : public CMapActor
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_RequestJoinBattle(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag);
	virtual RPCResult CB_PKRequest(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag, const UInt32 &Target_ID, const UInt8 &teamCount);
	virtual RPCResult CB_SetEnablePK(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag);
	virtual RPCResult CB_JoinBattle(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag);
	virtual RPCResult CB_CancelAuto(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_LevelUp(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AddHelper(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CB_RejectHelper(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_LeaveHelpersList(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CB_ClearHelpersList(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_HelpRequestFail(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_WarningMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const Int32 &value, const UInt8 &showIn);
	virtual RPCResult CB_SearchChar(LPCPEER pPeer, RPCContext &context, const CharInfoVector &CharInfoList);
	virtual RPCResult CB_OpenWeaponsRankPanel(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_SetTarget(LPCPEER pPeer, RPCContext &context, const Boolean &bResult, const UInt32 &char_id, const UInt16 &x, const UInt16 &y);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_JoinParty(LPCPEER pPeer, RPCContext &context, NetGroup* party);
	virtual RPCResult CB_ChangeDesc(LPCPEER pPeer, RPCContext &context, NetGroup* party);
	virtual RPCResult CB_PartyRequestOk(LPCPEER pPeer, RPCContext &context, const String &name);
	virtual RPCResult CB_PartyRequestNo(LPCPEER pPeer, RPCContext &context, const UInt16 &nState);
	virtual RPCResult CB_GetPlayerEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CB_PartyInviteOk(LPCPEER pPeer, RPCContext &context, const String &name);
	virtual RPCResult CB_PartyJoinRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId);
	virtual RPCResult CB_PartyInviteRequest(LPCPEER pPeer, RPCContext &context, NetGroup* party);
	virtual RPCResult CB_PartyClearRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const Boolean &showMsg);
	virtual RPCResult CB_PartyClearInvite(LPCPEER pPeer, RPCContext &context, const RPCNetID &partyId, const Boolean &showMsg);
	virtual RPCResult CB_PartyMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgType, const bool &inviteOrJoin, const String &charName);
	virtual RPCResult CB_SearchParty(LPCPEER pPeer, RPCContext &context, const PartyInfoVector &PartyInfoList);
	virtual RPCResult CB_GetPartyInfo(LPCPEER pPeer, RPCContext &context, const PartyInfoVector &PartyInfoList);
	virtual RPCResult CB_OpenSearchPartyPanel(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_HandleGuideEvent(LPCPEER pPeer, RPCContext &context, const UInt16 &ID);
	virtual RPCResult CB_RespondTest(LPCPEER pPeer, RPCContext &context, const UInt16 &playerCount);
	virtual RPCResult CB_LearnSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag);
	virtual RPCResult CB_UseSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag, const String &Msg, const UInt32 &SrcID, const UInt32 &TargetID);
	virtual RPCResult CB_UpGradeSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag);
	virtual RPCResult CB_RemoveSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag);
	virtual RPCResult CB_SkillShopLearn(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag);
	virtual RPCResult CB_GivenProcess(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgIndex);
	virtual RPCResult CB_GivenItem(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const UInt32Vector &itemID, const UInt8Vector &itemStacks);
	virtual RPCResult CB_GivenPartner(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const StringVector &PartnerNames);
	virtual RPCResult CB_GivenPet(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const StringVector &PetNames);
	virtual RPCResult CB_GivenMoney(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const UInt32 &gmoney);
	virtual RPCResult CB_UpdateShortCutSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_SaveSystemSetting(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AskForTrading(LPCPEER pPeer, RPCContext &context, const RPCNetID &askId);
	virtual RPCResult CB_ReplyReqTrading(LPCPEER pPeer, RPCContext &context, const Boolean &trade);
	virtual RPCResult CB_TradingMessage(LPCPEER pPeer, RPCContext &context, const Boolean &trade, const UInt8 &msgIndex);
	virtual RPCResult CB_TradingObject(LPCPEER pPeer, RPCContext &context, NetObject* trading);
	virtual RPCResult CB_TalkToNPC(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const bool &bResult);
	virtual RPCResult CB_TitleListUpdate(LPCPEER pPeer, RPCContext &context, const UInt16Vector &title_id_list, const StringVector &title_string_list, const UInt8Vector &updateState);
	virtual RPCResult CB_AntiHack(LPCPEER pPeer, RPCContext &context, const ByteArray &image, const AntiHackData &data);
	virtual RPCResult CB_ReportAntiHack(LPCPEER pPeer, RPCContext &context, const Boolean &is_right, const UInt8 &cwrong, const UInt16 &reward);
	virtual RPCResult CB_DepositInBank(LPCPEER pPeer, RPCContext &context, const Boolean &success);
	virtual RPCResult CB_WithdrawFmBank(LPCPEER pPeer, RPCContext &context, const Boolean &success);
	virtual RPCResult CB_StartHawk(LPCPEER pPeer, RPCContext &context, NetGroup* hawkGroup, const UInt8 &successIndex);
	virtual RPCResult CB_GetHawk(LPCPEER pPeer, RPCContext &context, const Boolean &self, NetGroup* hawkGroup, NetGroup* itemGroup, NetGroup* partnerGroup, const UInt16Vector &partnerSkillID, NetGroup* petGroup, const UInt16Vector &petSkillID);
	virtual RPCResult CB_HawkMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult CB_HawkHistory(LPCPEER pPeer, RPCContext &context, const HawkerHistoryVector &historys, const StringVector &itemNames);
	virtual RPCResult CB_UpdateHawkHistory(LPCPEER pPeer, RPCContext &context, const HawkerHistoryVector &historys, const StringVector &itemNames, const UInt32Vector &deleteHistoryID);
	virtual RPCResult CB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &Result, const UInt32 &resultFlag);
	virtual RPCResult CB_OpenCreateGuild(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AwardMsg(LPCPEER pPeer, RPCContext &context, const String &msg);
	virtual RPCResult CB_OpenGiftBox(LPCPEER pPeer, RPCContext &context, NetGroup* giftBox);
	virtual RPCResult CB_AddictionWarning(LPCPEER pPeer, RPCContext &context, const String &warnMsg);
	virtual RPCResult CB_AddictCurrentStatus(LPCPEER pPeer, RPCContext &context, const String &statusMsg);
	virtual RPCResult CB_AddictionShutDown(LPCPEER pPeer, RPCContext &context, const UInt32 &accOffTime);
	virtual RPCResult CB_PlyInterestDwn(LPCPEER pPeer, RPCContext &context, const UInt8 &interest);
	virtual RPCResult CB_UseItemCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &item_id, const UInt32 &SrcID, const UInt32 &TargetID);
	virtual RPCResult CB_AskEnterQuestion(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_EnterQuestion(LPCPEER pPeer, RPCContext &context, NetObject* pQuestion);
	virtual RPCResult CB_ExitQuestion(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AnswerQuestion(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const UInt32 &CharID);
	virtual RPCResult CB_OpenWareHouse(LPCPEER pPeer, RPCContext &context, NetGroup* items, NetGroup* partners, NetGroup* pets, NetGroup* partnerSkillGroup, NetGroup* petSkillGroup);
	virtual RPCResult CB_AssetProtectResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &index, const UInt8 &assetType);
	virtual RPCResult CB_AssetReleaseLockResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &index, const UInt8 &assetType);
	virtual RPCResult CB_SetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success);
	virtual RPCResult CB_ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success);
	virtual RPCResult CB_SetRobot(LPCPEER pPeer, RPCContext &context, const Boolean &bRobot, const String &robotScript);
	virtual RPCResult CB_RobotMapAction(LPCPEER pPeer, RPCContext &context, const String &func, const RPCNetID &mapNetId, const RPCNetID &charNetId, const RPCNetID &otherNetId);
	virtual RPCResult CB_RobotBattleTurn(LPCPEER pPeer, RPCContext &context, const String &func, const RPCNetID &battleNetId, const RPCNetID &charNetId, const RPCNetID &batCharNetId, const RPCNetID &partnerNetId);
	virtual RPCResult CB_SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context, const Boolean &hit);
	virtual RPCResult CB_DonateResult(LPCPEER pPeer, RPCContext &context, const UInt32 &donate, const UInt8 &returnFlag, const UInt32 &donateMoney);
	virtual RPCResult CB_GetPartnerCollectionList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &PartnerIDs);
	virtual RPCResult CB_UsePartnerCollectionBuff(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag);
	virtual RPCResult CB_AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &MOBid, const BYTE &returnFlag);
	virtual RPCResult CB_ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult CB_GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult CB_RequestTargetEquip(LPCPEER pPeer, RPCContext &context, NetGroup* items);
	virtual RPCResult CB_RelationSystem(LPCPEER pPeer, RPCContext &context, NetObject* pRelationSystem);
	virtual RPCResult CB_SetTargetNameFormHawk(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &char_name, const Boolean &isonline);
	virtual RPCResult CB_InitTargetHawkConcernState(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CB_ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId);
	virtual RPCResult CB_ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos);
	virtual RPCResult CB_ForgiveLineup(LPCPEER pPeer, RPCContext &context, const int &iResault);
	virtual RPCResult CB_SetChangePos(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex);
	virtual RPCResult CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money, const Boolean &result);
	virtual RPCResult CB_JailDelivery(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_WantedBail(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_WantedTrace(LPCPEER pPeer, RPCContext &context, const String &name, const UInt32 &lineId, const UInt32 &mapId, const UInt32 &mapX, const UInt32 &mapY);
	virtual RPCResult CB_RequestWantedTrace(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_GetGenerationGroup(LPCPEER pPeer, RPCContext &context, NetGroup* pGntGrp);
	virtual RPCResult CB_RequestGenerationTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiRequstId);
	virtual RPCResult CB_RequestGenerationResult(LPCPEER pPeer, RPCContext &context, const int &iResultCode, const UInt32 &uiRelatedId);
	virtual RPCResult CB_DeleteGeneration(LPCPEER pPeer, RPCContext &context, const int &iResultCode, const UInt32 &uiRelatedId);
	virtual RPCResult CB_ChangeGenerationType(LPCPEER pPeer, RPCContext &context, const int &iType);
	virtual RPCResult CB_UpdateGenerationAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &uiCurAcc, const UInt32 &uiHisAcc);
	virtual RPCResult CB_BugAccShopItem(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &iIndex);
	virtual RPCResult CB_GenerationGainAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &iGainedAcc);
	virtual RPCResult CB_BuyPointsShopItem(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &uiPoints);
	virtual RPCResult CB_OpenBusinessLog(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_SetMarriageNpcAction(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_uid, const UInt8 &atcion, const UInt8 &direction);
	virtual RPCResult CB_ShowMarriageEff(LPCPEER pPeer, RPCContext &context, const UInt8 &state, const UInt32 &husbandID, const UInt32 &wifeID);
	virtual RPCResult CB_ResetChangeLine(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_PetUseSkillMsg(LPCPEER pPeer, RPCContext &context, const String &Msg);
	virtual RPCResult CB_LeaveTeamTemporarily(LPCPEER pPeer, RPCContext &context, const UInt32 &uRetCode, const UInt32 &uId);
	virtual RPCResult CB_GetPartyInfoWhenJumpMap(LPCPEER pPeer, RPCContext &context, NetGroup* pParty);
	virtual RPCResult CB_ReturnParty(LPCPEER pPeer, RPCContext &context, const UInt32 &uRetCode, NetObject* pChar);
	virtual RPCResult CB_RefreshPartyCharInfo(LPCPEER pPeer, RPCContext &context, NetObject* pPartyCharInfo);
	virtual RPCResult CB_RestorePartyAsTemp(LPCPEER pPeer, RPCContext &context, NetGroup* pParty);
	virtual RPCResult CB_SetChangeModelSkillGroup(LPCPEER pPeer, RPCContext &context, NetGroup* skillGroup);
	virtual RPCResult CB_ShowIncognitoBoxEff(LPCPEER pPeer, RPCContext &context, const UInt16 &posx, const UInt16 &posy, const UInt8 &effType);
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &acct_id, const UInt32 &char_id);
	virtual void OnUpdateLevelInfo(const UInt16 &level, const UInt32 &nextLvExp);
	virtual void OnUpdateExp(const UInt32 &exp);
	virtual void OnUpdateAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI);
	virtual void OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD);
	virtual void OnUpdateAttrPoint(const UInt16 &attrPoint);
	virtual void OnUpdateHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP);
	virtual void OnUpdateSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP);
	virtual void OnUpdateDP(const UInt16 &DP, const UInt16 &mDP, const UInt16 &maxDP);
	virtual void OnUpdateWC(const UInt16 &WC, const UInt16 &maxWC, const UInt16 &WcExp, const UInt32 &nextWCExp);
	virtual void OnUpdateVP(const UInt16 &VP, const UInt16 &maxVP);
	virtual void OnUpdateGoodness(const Int32 &goodness);
	virtual void OnUpdateHonor(const UInt32 &honor);
	virtual void OnUpdateTitle(const String &title, const UInt16 &title_id);
	virtual void OnUpdateManor(const UInt32 &manor, const UInt32 &pettwo_uid, const UInt32 &pettwo_id, const String &pettwoName, const UInt16 &pettwoColorIndex);
	virtual void OnUpdateDonate(const UInt32 &fDonate, const UInt32 &totalFDonate, const UInt32 &mDonate, const UInt32 &totalMDonate);
	virtual void OnUpdateYuanBao(const UInt32 &yuanBao, const UInt32 &yuanBaoTicket);
	virtual void OnUpdateMoney(const UInt32 &money);
	virtual void OnUpdateBankMoney(const UInt32 &bankMoney);
	virtual void OnUpdateSMoney(const UInt32 &SMoney);
	virtual void OnUpdatePartner(const UInt32 &partner_id, const UInt32 &partnerMob_id, const String &partnerName, const UInt16 &partnerColorIndex);
	virtual void OnUpdatePet(const UInt32 &pet_uid, const UInt32 &pet_id, const String &petName, const UInt16 &petColorIndex);
	virtual void OnUpdateMaxOther(const UInt8 &maxPartner, const UInt8 &maxPet);
	virtual void OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor);
	virtual void OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);
	virtual void OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
	virtual void OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor);
	virtual void OnUpdateState(const UInt8 &state);
	virtual void OnUpdateBuff(const String &buff);
	virtual void OnUpdateBuffCache(const String &buffCache);
	virtual void OnUpdateShortcut(const UInt16 &shortcut0, const UInt16 &shortcut1, const UInt16 &shortcut2, const UInt16 &shortcut3, const UInt16 &shortcut4, const UInt16 &shortcut5, const UInt16 &shortcut6, const UInt16 &shortcut7, const UInt16 &shortcut8, const UInt16 &shortcut9, const UInt8 &shortcuttype0, const UInt8 &shortcuttype1, const UInt8 &shortcuttype2, const UInt8 &shortcuttype3, const UInt8 &shortcuttype4, const UInt8 &shortcuttype5, const UInt8 &shortcuttype6, const UInt8 &shortcuttype7, const UInt8 &shortcuttype8, const UInt8 &shortcuttype9);
	virtual void OnUpdatePartyInfo(const Boolean &hasParty, const Boolean &isLeader, const RPCNetID &partyNetID);
	virtual void OnUpdatePKInfo(const UInt16 &PK_Value);
	virtual void OnUpdateBattleInfo(const Boolean &EnablePK);
	virtual void OnUpdateSettingInfo(const Boolean &EnableBlockGiven, const Boolean &EnableBlockParty, const Boolean &EnableBlockMsg, const Boolean &EnableBlockTrading, const Boolean &EnableBlockMakeFriend, const Boolean &EnableShowEquip);
	virtual void OnUpdateOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor);
	virtual void OnUpdateOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor);
	virtual void OnUpdateSpeakCount(const UInt16 &speakCount);
	virtual void OnUpdateOfflineTime(const UInt32 &offlineTime);
	virtual void OnUpdateHawkInfo(const String &hawkName);
	virtual void OnUpdateTP(const UInt16 &TP, const UInt32 &Model_mobid);
	virtual void OnUpdateSpeakBlock(const UInt32 &speakBlock);
	virtual void OnUpdateHawkerTime(const UInt32 &hawkerTime);
	virtual void OnUpdateAvailBackPack(const UInt8 &availBackPack);
	virtual void OnUpdateAvailWareHouse(const UInt8 &availItemWareHouse, const UInt8 &availPartnerWareHouse, const UInt8 &availPetWareHouse);
	virtual void OnUpdateFollowerList(const UInt32Vector &followerList);
	virtual void OnUpdatePvPScoreSingle(const UInt32 &PvPScoreSingle);
	virtual void OnUpdateUnlockTime(const UInt32 &unlock_time);
	virtual void OnUpdateYuanbaoLock(const Boolean &yuanbaoLock);
	virtual void OnUpdateBankLock(const Boolean &bankLock);
	virtual void OnUpdateSiblingId(const UInt32 &sibling_id);
	virtual void OnUpdateMark(const UInt32 &mark);
	virtual void OnUpdateWantedReward(const UInt32 &wantedReward);
	virtual void OnUpdateGuildUid(const UInt32 &guildUid);
	virtual void OnUpdateGenerationAcc(const UInt32 &iHisGenAcc, const UInt32 &uiCurGenAcc);
***/

protected:
	typedef CMapActor Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		levelInfoMask	= Parent::NextFreeMask + 1,
		expMask	= Parent::NextFreeMask + 2,
		attrib1Mask	= Parent::NextFreeMask + 3,
		attrib2Mask	= Parent::NextFreeMask + 4,
		attrPointMask	= Parent::NextFreeMask + 5,
		HPMask	= Parent::NextFreeMask + 6,
		SPMask	= Parent::NextFreeMask + 7,
		DPMask	= Parent::NextFreeMask + 8,
		WCMask	= Parent::NextFreeMask + 9,
		VPMask	= Parent::NextFreeMask + 10,
		goodnessMask	= Parent::NextFreeMask + 11,
		honorMask	= Parent::NextFreeMask + 12,
		titleMask	= Parent::NextFreeMask + 13,
		manorMask	= Parent::NextFreeMask + 14,
		donateMask	= Parent::NextFreeMask + 15,
		yuanBaoMask	= Parent::NextFreeMask + 16,
		moneyMask	= Parent::NextFreeMask + 17,
		bankMoneyMask	= Parent::NextFreeMask + 18,
		SMoneyMask	= Parent::NextFreeMask + 19,
		partnerMask	= Parent::NextFreeMask + 20,
		petMask	= Parent::NextFreeMask + 21,
		maxOtherMask	= Parent::NextFreeMask + 22,
		styleMask	= Parent::NextFreeMask + 23,
		eqWeaponMask	= Parent::NextFreeMask + 24,
		eqHeadMask	= Parent::NextFreeMask + 25,
		eqClothMask	= Parent::NextFreeMask + 26,
		stateMask	= Parent::NextFreeMask + 27,
		buffMask	= Parent::NextFreeMask + 28,
		buffCacheMask	= Parent::NextFreeMask + 29,
		shortcutMask	= Parent::NextFreeMask + 30,
		partyInfoMask	= Parent::NextFreeMask + 31,
		PKInfoMask	= Parent::NextFreeMask + 32,
		BattleInfoMask	= Parent::NextFreeMask + 33,
		SettingInfoMask	= Parent::NextFreeMask + 34,
		orgHairStyleMask	= Parent::NextFreeMask + 35,
		orgEqClothMask	= Parent::NextFreeMask + 36,
		speakCountMask	= Parent::NextFreeMask + 37,
		offlineTimeMask	= Parent::NextFreeMask + 38,
		hawkInfoMask	= Parent::NextFreeMask + 39,
		TPMask	= Parent::NextFreeMask + 40,
		speakBlockMask	= Parent::NextFreeMask + 41,
		hawkerTimeMask	= Parent::NextFreeMask + 42,
		availBackPackMask	= Parent::NextFreeMask + 43,
		availWareHouseMask	= Parent::NextFreeMask + 44,
		followerListMask	= Parent::NextFreeMask + 45,
		PvPScoreSingleMask	= Parent::NextFreeMask + 46,
		unlockTimeMask	= Parent::NextFreeMask + 47,
		yuanbaoLockMask	= Parent::NextFreeMask + 48,
		bankLockMask	= Parent::NextFreeMask + 49,
		SiblingIdMask	= Parent::NextFreeMask + 50,
		markMask	= Parent::NextFreeMask + 51,
		wantedRewardMask	= Parent::NextFreeMask + 52,
		guildUidMask	= Parent::NextFreeMask + 53,
		GenerationAccMask	= Parent::NextFreeMask + 54,
		NextFreeMask	= Parent::NextFreeMask + 55
	};
	virtual RPCResult MoveTo(LPCPEER pPeer, RPCContext &context, const UInt16 &posX, const UInt16 &posY, const UInt16 &hint1, const UInt16 &hint2);
	virtual RPCResult StopMove(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult RespondTest(LPCPEER pPeer, RPCContext &context, const UInt32 &minRespond, const UInt32 &maxRespond, const float &avgRespond, const UInt32 &respondCount);
	virtual RPCResult TestBattle(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CancelAuto(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult LevelUp(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult RaiseAttrib(LPCPEER pPeer, RPCContext &context, const UInt16 &sta, const UInt16 &spr, const UInt16 &str, const UInt16 &con, const UInt16 &agi);
	virtual RPCResult PKRequest(LPCPEER pPeer, RPCContext &context, const UInt32 &TargetID, const BYTE &Type);
	virtual RPCResult LeaveWatching(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult MonitorBattle(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult JoinBattle(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult RequestJoinBattle(LPCPEER pPeer, RPCContext &context, const Boolean &Force);
	virtual RPCResult RejectHelper(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult SetFollowPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id);
	virtual RPCResult SetFollowPet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_id);
	virtual RPCResult ChangeMapLine(LPCPEER pPeer, RPCContext &context, const UInt8 &line_id);
	virtual RPCResult UpdateYuanBao(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_RequestJoinBattle(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag) = 0;
	virtual RPCResult CB_PKRequest(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag, const UInt32 &Target_ID, const UInt8 &teamCount) = 0;
	virtual RPCResult CB_SetEnablePK(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag) = 0;
	virtual RPCResult CB_JoinBattle(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag) = 0;
	virtual RPCResult CB_CancelAuto(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_LevelUp(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_AddHelper(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult CB_RejectHelper(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_LeaveHelpersList(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult CB_ClearHelpersList(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_HelpRequestFail(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_WarningMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const Int32 &value, const UInt8 &showIn) = 0;
	virtual RPCResult SearchChar(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_SearchChar(LPCPEER pPeer, RPCContext &context, const CharInfoVector &CharInfoList) = 0;
	virtual RPCResult CB_OpenWeaponsRankPanel(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult SetTarget(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CB_SetTarget(LPCPEER pPeer, RPCContext &context, const Boolean &bResult, const UInt32 &char_id, const UInt16 &x, const UInt16 &y) = 0;
	virtual RPCResult CreateParty(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult PartyJoinRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId);
	virtual RPCResult PartyJoinLevelControl(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const UInt16 &levelDown, const UInt16 &levelUp);
	virtual RPCResult PartyJoinReply(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const Boolean &agree, const Boolean &showMsg);
	virtual RPCResult PartyInviteRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId);
	virtual RPCResult PartyInviteReply(LPCPEER pPeer, RPCContext &context, const RPCNetID &partyId, const Boolean &agree, const Boolean &showMsg);
	virtual RPCResult SearchParty(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult GetPartyInfo(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg) = 0;
	virtual RPCResult CB_JoinParty(LPCPEER pPeer, RPCContext &context, NetGroup* party) = 0;
	virtual RPCResult CB_ChangeDesc(LPCPEER pPeer, RPCContext &context, NetGroup* party) = 0;
	virtual RPCResult CB_PartyRequestOk(LPCPEER pPeer, RPCContext &context, const String &name) = 0;
	virtual RPCResult CB_PartyRequestNo(LPCPEER pPeer, RPCContext &context, const UInt16 &nState) = 0;
	virtual RPCResult CB_GetPlayerEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult CB_PartyInviteOk(LPCPEER pPeer, RPCContext &context, const String &name) = 0;
	virtual RPCResult CB_PartyJoinRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId) = 0;
	virtual RPCResult CB_PartyInviteRequest(LPCPEER pPeer, RPCContext &context, NetGroup* party) = 0;
	virtual RPCResult CB_PartyClearRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const Boolean &showMsg) = 0;
	virtual RPCResult CB_PartyClearInvite(LPCPEER pPeer, RPCContext &context, const RPCNetID &partyId, const Boolean &showMsg) = 0;
	virtual RPCResult CB_PartyMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgType, const bool &inviteOrJoin, const String &charName) = 0;
	virtual RPCResult CB_SearchParty(LPCPEER pPeer, RPCContext &context, const PartyInfoVector &PartyInfoList) = 0;
	virtual RPCResult CB_GetPartyInfo(LPCPEER pPeer, RPCContext &context, const PartyInfoVector &PartyInfoList) = 0;
	virtual RPCResult CB_OpenSearchPartyPanel(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult ShutNewGuide(LPCPEER pPeer, RPCContext &context, const UInt8 &flag);
	virtual RPCResult CB_HandleGuideEvent(LPCPEER pPeer, RPCContext &context, const UInt16 &ID) = 0;
	virtual RPCResult CheatCode(LPCPEER pPeer, RPCContext &context, const UInt32 &orderMakerAcct_id, const String &code);
	virtual RPCResult CB_RespondTest(LPCPEER pPeer, RPCContext &context, const UInt16 &playerCount) = 0;
	virtual RPCResult LearnSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id);
	virtual RPCResult UseSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const UInt32 &TargetID, const BYTE &TargetType);
	virtual RPCResult UpGradeSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id);
	virtual RPCResult RemoveSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id);
	virtual RPCResult CB_LearnSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag) = 0;
	virtual RPCResult CB_UseSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag, const String &Msg, const UInt32 &SrcID, const UInt32 &TargetID) = 0;
	virtual RPCResult CB_UpGradeSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag) = 0;
	virtual RPCResult CB_RemoveSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag) = 0;
	virtual RPCResult SkillShopLearn(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id);
	virtual RPCResult LeaveSkillShop(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_SkillShopLearn(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag) = 0;
	virtual RPCResult GivenProcess(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const UInt8Vector &itemIndexs, const UInt8Vector &itemStacks, const UInt32Vector &PartnerIDs, const UInt32Vector &PetIDs, const UInt32 &gmoney);
	virtual RPCResult CB_GivenProcess(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgIndex) = 0;
	virtual RPCResult CB_GivenItem(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const UInt32Vector &itemID, const UInt8Vector &itemStacks) = 0;
	virtual RPCResult CB_GivenPartner(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const StringVector &PartnerNames) = 0;
	virtual RPCResult CB_GivenPet(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const StringVector &PetNames) = 0;
	virtual RPCResult CB_GivenMoney(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const UInt32 &gmoney) = 0;
	virtual RPCResult UpdateShortCutList(LPCPEER pPeer, RPCContext &context, const UInt16Vector &shortcut, const UInt8Vector &shortcuttype);
	virtual RPCResult UpdateShortCut(LPCPEER pPeer, RPCContext &context, const UInt16 &shortcut, const UInt8 &shortcuttype, const UInt8 &position);
	virtual RPCResult CB_UpdateShortCutSuccess(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult SaveSystemSetting(LPCPEER pPeer, RPCContext &context, const Boolean &EnableBlockGiven, const Boolean &EnableBlockParty, const Boolean &EnableBlockMsg, const Boolean &EnableBlockTrading, const Boolean &EnableBlockMakeFriend, const Boolean &EnableShowEquip);
	virtual RPCResult CB_SaveSystemSetting(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult ChangeMsgIcon(LPCPEER pPeer, RPCContext &context, const UInt16 &iconID);
	virtual RPCResult AskForTrading(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId);
	virtual RPCResult CB_AskForTrading(LPCPEER pPeer, RPCContext &context, const RPCNetID &askId) = 0;
	virtual RPCResult ReplyReqTrading(LPCPEER pPeer, RPCContext &context, const Boolean &trade, const RPCNetID &targetId);
	virtual RPCResult CB_ReplyReqTrading(LPCPEER pPeer, RPCContext &context, const Boolean &trade) = 0;
	virtual RPCResult CB_TradingMessage(LPCPEER pPeer, RPCContext &context, const Boolean &trade, const UInt8 &msgIndex) = 0;
	virtual RPCResult CB_TradingObject(LPCPEER pPeer, RPCContext &context, NetObject* trading) = 0;
	virtual RPCResult TalkToNPC(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id);
	virtual RPCResult CB_TalkToNPC(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const bool &bResult) = 0;
	virtual RPCResult ChangeTitle(LPCPEER pPeer, RPCContext &context, const UInt16 &title_id);
	virtual RPCResult CB_TitleListUpdate(LPCPEER pPeer, RPCContext &context, const UInt16Vector &title_id_list, const StringVector &title_string_list, const UInt8Vector &updateState) = 0;
	virtual RPCResult CB_AntiHack(LPCPEER pPeer, RPCContext &context, const ByteArray &image, const AntiHackData &data) = 0;
	virtual RPCResult AnswerAntiHack(LPCPEER pPeer, RPCContext &context, const UInt8 &answer, const Boolean &is_right);
	virtual RPCResult CB_ReportAntiHack(LPCPEER pPeer, RPCContext &context, const Boolean &is_right, const UInt8 &cwrong, const UInt16 &reward) = 0;
	virtual RPCResult DepositInBank(LPCPEER pPeer, RPCContext &context, const UInt32 &money);
	virtual RPCResult WithdrawFmBank(LPCPEER pPeer, RPCContext &context, const UInt32 &money);
	virtual RPCResult CB_DepositInBank(LPCPEER pPeer, RPCContext &context, const Boolean &success) = 0;
	virtual RPCResult CB_WithdrawFmBank(LPCPEER pPeer, RPCContext &context, const Boolean &success) = 0;
	virtual RPCResult StartHawk(LPCPEER pPeer, RPCContext &context, const String &hawkName, const HawkerItemInfoVector &itemList, const HawkerItemInfoVector &partnerList, const HawkerItemInfoVector &petList);
	virtual RPCResult CB_StartHawk(LPCPEER pPeer, RPCContext &context, NetGroup* hawkGroup, const UInt8 &successIndex) = 0;
	virtual RPCResult GetHawkFromTarget(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult CB_GetHawk(LPCPEER pPeer, RPCContext &context, const Boolean &self, NetGroup* hawkGroup, NetGroup* itemGroup, NetGroup* partnerGroup, const UInt16Vector &partnerSkillID, NetGroup* petGroup, const UInt16Vector &petSkillID) = 0;
	virtual RPCResult CB_HawkMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex) = 0;
	virtual RPCResult CB_HawkHistory(LPCPEER pPeer, RPCContext &context, const HawkerHistoryVector &historys, const StringVector &itemNames) = 0;
	virtual RPCResult CB_UpdateHawkHistory(LPCPEER pPeer, RPCContext &context, const HawkerHistoryVector &historys, const StringVector &itemNames, const UInt32Vector &deleteHistoryID) = 0;
	virtual RPCResult ClearHawkerHistory(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &Result, const UInt32 &resultFlag) = 0;
	virtual RPCResult SetListen(LPCPEER pPeer, RPCContext &context, const BOOL &IsListen);
	virtual RPCResult DonateMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &money);
	virtual RPCResult CreateGuild(LPCPEER pPeer, RPCContext &context, const String &name, const bool &cancel);
	virtual RPCResult CB_OpenCreateGuild(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_AwardMsg(LPCPEER pPeer, RPCContext &context, const String &msg) = 0;
	virtual RPCResult CB_OpenGiftBox(LPCPEER pPeer, RPCContext &context, NetGroup* giftBox) = 0;
	virtual RPCResult CB_AddictionWarning(LPCPEER pPeer, RPCContext &context, const String &warnMsg) = 0;
	virtual RPCResult CB_AddictCurrentStatus(LPCPEER pPeer, RPCContext &context, const String &statusMsg) = 0;
	virtual RPCResult CB_AddictionShutDown(LPCPEER pPeer, RPCContext &context, const UInt32 &accOffTime) = 0;
	virtual RPCResult CB_PlyInterestDwn(LPCPEER pPeer, RPCContext &context, const UInt8 &interest) = 0;
	virtual RPCResult CB_UseItemCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &item_id, const UInt32 &SrcID, const UInt32 &TargetID) = 0;
	virtual RPCResult EnterQuestion(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult ExitQuestion(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AskEnterQuestion(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_EnterQuestion(LPCPEER pPeer, RPCContext &context, NetObject* pQuestion) = 0;
	virtual RPCResult CB_ExitQuestion(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_AnswerQuestion(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const UInt32 &CharID) = 0;
	virtual RPCResult CloseWareHouse(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_OpenWareHouse(LPCPEER pPeer, RPCContext &context, NetGroup* items, NetGroup* partners, NetGroup* pets, NetGroup* partnerSkillGroup, NetGroup* petSkillGroup) = 0;
	virtual RPCResult LockAsset(LPCPEER pPeer, RPCContext &context, const UInt32 &bagIndex, const UInt8 &assetType);
	virtual RPCResult UnlockAsset(LPCPEER pPeer, RPCContext &context, const ByteArray &password, const UInt32 &bagIndex, const UInt8 &assetType);
	virtual RPCResult SetAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &password);
	virtual RPCResult ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult ChangeAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &oldPassword, const ByteArray &newPassword);
	virtual RPCResult CB_AssetProtectResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &index, const UInt8 &assetType) = 0;
	virtual RPCResult CB_AssetReleaseLockResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &index, const UInt8 &assetType) = 0;
	virtual RPCResult CB_SetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success) = 0;
	virtual RPCResult CB_ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success) = 0;
	virtual RPCResult CB_SetRobot(LPCPEER pPeer, RPCContext &context, const Boolean &bRobot, const String &robotScript) = 0;
	virtual RPCResult CB_RobotMapAction(LPCPEER pPeer, RPCContext &context, const String &func, const RPCNetID &mapNetId, const RPCNetID &charNetId, const RPCNetID &otherNetId) = 0;
	virtual RPCResult CB_RobotBattleTurn(LPCPEER pPeer, RPCContext &context, const String &func, const RPCNetID &battleNetId, const RPCNetID &charNetId, const RPCNetID &batCharNetId, const RPCNetID &partnerNetId) = 0;
	virtual RPCResult SetAutoWalk(LPCPEER pPeer, RPCContext &context, const Boolean &bRobot);
	virtual RPCResult SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context, const Boolean &hit);
	virtual RPCResult CB_SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context, const Boolean &hit) = 0;
	virtual RPCResult KickMember(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &reason);
	virtual RPCResult CB_DonateResult(LPCPEER pPeer, RPCContext &context, const UInt32 &donate, const UInt8 &returnFlag, const UInt32 &donateMoney) = 0;
	virtual RPCResult CB_GetPartnerCollectionList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &PartnerIDs) = 0;
	virtual RPCResult UsePartnerCollectionBuff(LPCPEER pPeer, RPCContext &context, const bool &bUse);
	virtual RPCResult CB_UsePartnerCollectionBuff(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag) = 0;
	virtual RPCResult AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id);
	virtual RPCResult CB_AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &MOBid, const BYTE &returnFlag) = 0;
	virtual RPCResult ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &changeType, const UInt32 &num, const ByteArray &password);
	virtual RPCResult CB_ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex) = 0;
	virtual RPCResult AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &pricePerUnit, const ByteArray &password);
	virtual RPCResult CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex) = 0;
	virtual RPCResult BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID, const UInt32 &YBT, const UInt32 &moneyPerUnit, const ByteArray &password);
	virtual RPCResult CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex) = 0;
	virtual RPCResult GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const ByteArray &password);
	virtual RPCResult CB_GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex) = 0;
	virtual RPCResult RequestTargetEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CB_RequestTargetEquip(LPCPEER pPeer, RPCContext &context, NetGroup* items) = 0;
	virtual RPCResult CB_RelationSystem(LPCPEER pPeer, RPCContext &context, NetObject* pRelationSystem) = 0;
	virtual RPCResult AddStartHawkItem(LPCPEER pPeer, RPCContext &context, const HawkerItemInfo &itemInfo, const StartHawkAddType &addType);
	virtual RPCResult CB_SetTargetNameFormHawk(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &char_name, const Boolean &isonline) = 0;
	virtual RPCResult CB_InitTargetHawkConcernState(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId);
	virtual RPCResult CB_ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId) = 0;
	virtual RPCResult ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos);
	virtual RPCResult CB_ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos) = 0;
	virtual RPCResult ForgiveLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId);
	virtual RPCResult CB_ForgiveLineup(LPCPEER pPeer, RPCContext &context, const int &iResault) = 0;
	virtual RPCResult SetChangePos(LPCPEER pPeer, RPCContext &context, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex);
	virtual RPCResult CB_SetChangePos(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex) = 0;
	virtual RPCResult UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const String &targetName, const Int32 &money);
	virtual RPCResult CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money, const Boolean &result) = 0;
	virtual RPCResult JailDelivery(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult CB_JailDelivery(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult WantedBail(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult CB_WantedBail(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult WantedTrace(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult CB_WantedTrace(LPCPEER pPeer, RPCContext &context, const String &name, const UInt32 &lineId, const UInt32 &mapId, const UInt32 &mapX, const UInt32 &mapY) = 0;
	virtual RPCResult CB_RequestWantedTrace(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult GetGenerationGroup(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_GetGenerationGroup(LPCPEER pPeer, RPCContext &context, NetGroup* pGntGrp) = 0;
	virtual RPCResult RequestGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId);
	virtual RPCResult CB_RequestGenerationTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiRequstId) = 0;
	virtual RPCResult RequestGenerationAns(LPCPEER pPeer, RPCContext &context, const BOOL &bConfirm, const UInt32 &uiRelatedId);
	virtual RPCResult CB_RequestGenerationResult(LPCPEER pPeer, RPCContext &context, const int &iResultCode, const UInt32 &uiRelatedId) = 0;
	virtual RPCResult DeleteGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiRelatedId);
	virtual RPCResult CB_DeleteGeneration(LPCPEER pPeer, RPCContext &context, const int &iResultCode, const UInt32 &uiRelatedId) = 0;
	virtual RPCResult CB_ChangeGenerationType(LPCPEER pPeer, RPCContext &context, const int &iType) = 0;
	virtual RPCResult GetSavedExp(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_UpdateGenerationAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &uiCurAcc, const UInt32 &uiHisAcc) = 0;
	virtual RPCResult BuyAccShopItem(LPCPEER pPeer, RPCContext &context, const UInt32 &iIndex);
	virtual RPCResult CB_BugAccShopItem(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &iIndex) = 0;
	virtual RPCResult CB_GenerationGainAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &iGainedAcc) = 0;
	virtual RPCResult BuyPointsShopItem(LPCPEER pPeer, RPCContext &context, const int &iItemID, const UInt32 &nPointsShopID, const UInt32 &nPointsShopTypeID);
	virtual RPCResult CB_BuyPointsShopItem(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &uiPoints) = 0;
	virtual RPCResult CB_OpenBusinessLog(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_SetMarriageNpcAction(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_uid, const UInt8 &atcion, const UInt8 &direction) = 0;
	virtual RPCResult CB_ShowMarriageEff(LPCPEER pPeer, RPCContext &context, const UInt8 &state, const UInt32 &husbandID, const UInt32 &wifeID) = 0;
	virtual RPCResult CB_ResetChangeLine(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_PetUseSkillMsg(LPCPEER pPeer, RPCContext &context, const String &Msg) = 0;
	virtual RPCResult RequestCrossMapJump(LPCPEER pPeer, RPCContext &context, const UInt32 &uSrcJmpSeq, const UInt32 &uDstJmpSeq);
	virtual RPCResult LeaveTeamTemporarily(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_LeaveTeamTemporarily(LPCPEER pPeer, RPCContext &context, const UInt32 &uRetCode, const UInt32 &uId) = 0;
	virtual RPCResult CB_GetPartyInfoWhenJumpMap(LPCPEER pPeer, RPCContext &context, NetGroup* pParty) = 0;
	virtual RPCResult ReturnParty(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_ReturnParty(LPCPEER pPeer, RPCContext &context, const UInt32 &uRetCode, NetObject* pChar) = 0;
	virtual RPCResult CB_RefreshPartyCharInfo(LPCPEER pPeer, RPCContext &context, NetObject* pPartyCharInfo) = 0;
	virtual RPCResult CB_RestorePartyAsTemp(LPCPEER pPeer, RPCContext &context, NetGroup* pParty) = 0;
	virtual RPCResult CB_SetChangeModelSkillGroup(LPCPEER pPeer, RPCContext &context, NetGroup* skillGroup) = 0;
	virtual RPCResult CB_ShowIncognitoBoxEff(LPCPEER pPeer, RPCContext &context, const UInt16 &posx, const UInt16 &posy, const UInt8 &effType) = 0;
	const UInt32& GetAcct_id() const { return m.acct_id; }
	const UInt32& GetChar_id() const { return m.char_id; }
	const UInt16& GetLevel() const { return m.level; }
	const UInt32& GetNextLvExp() const { return m.nextLvExp; }
	const UInt32& GetExp() const { return m.exp; }
	const UInt16& GetSTA() const { return m.STA; }
	const UInt16& GetSPR() const { return m.SPR; }
	const UInt16& GetSTR() const { return m.STR; }
	const UInt16& GetCON() const { return m.CON; }
	const UInt16& GetAGI() const { return m.AGI; }
	const UInt16& GetATK() const { return m.ATK; }
	const UInt16& GetDEF() const { return m.DEF; }
	const UInt16& GetHIT() const { return m.HIT; }
	const UInt16& GetEVA() const { return m.EVA; }
	const UInt16& GetPOW() const { return m.POW; }
	const UInt16& GetSPD() const { return m.SPD; }
	const UInt16& GetAttrPoint() const { return m.attrPoint; }
	const UInt16& GetHP() const { return m.HP; }
	const UInt16& GetMHP() const { return m.mHP; }
	const UInt16& GetMaxHP() const { return m.maxHP; }
	const UInt16& GetSP() const { return m.SP; }
	const UInt16& GetMSP() const { return m.mSP; }
	const UInt16& GetMaxSP() const { return m.maxSP; }
	const UInt16& GetDP() const { return m.DP; }
	const UInt16& GetMDP() const { return m.mDP; }
	const UInt16& GetMaxDP() const { return m.maxDP; }
	const UInt16& GetWC() const { return m.WC; }
	const UInt16& GetMaxWC() const { return m.maxWC; }
	const UInt16& GetWcExp() const { return m.WcExp; }
	const UInt32& GetNextWCExp() const { return m.nextWCExp; }
	const UInt16& GetVP() const { return m.VP; }
	const UInt16& GetMaxVP() const { return m.maxVP; }
	const Int32& GetGoodness() const { return m.goodness; }
	const UInt32& GetHonor() const { return m.honor; }
	const String& GetTitle() const { return m.title; }
	const UInt16& GetTitle_id() const { return m.title_id; }
	const UInt32& GetManor() const { return m.manor; }
	const UInt32& GetPettwo_uid() const { return m.pettwo_uid; }
	const UInt32& GetPettwo_id() const { return m.pettwo_id; }
	const String& GetPettwoName() const { return m.pettwoName; }
	const UInt16& GetPettwoColorIndex() const { return m.pettwoColorIndex; }
	const UInt32& GetFDonate() const { return m.fDonate; }
	const UInt32& GetTotalFDonate() const { return m.totalFDonate; }
	const UInt32& GetMDonate() const { return m.mDonate; }
	const UInt32& GetTotalMDonate() const { return m.totalMDonate; }
	const UInt32& GetYuanBao() const { return m.yuanBao; }
	const UInt32& GetYuanBaoTicket() const { return m.yuanBaoTicket; }
	const UInt32& GetMoney() const { return m.money; }
	const UInt32& GetBankMoney() const { return m.bankMoney; }
	const UInt32& GetSMoney() const { return m.SMoney; }
	const UInt32& GetPartner_id() const { return m.partner_id; }
	const UInt32& GetPartnerMob_id() const { return m.partnerMob_id; }
	const String& GetPartnerName() const { return m.partnerName; }
	const UInt16& GetPartnerColorIndex() const { return m.partnerColorIndex; }
	const UInt32& GetPet_uid() const { return m.pet_uid; }
	const UInt32& GetPet_id() const { return m.pet_id; }
	const String& GetPetName() const { return m.petName; }
	const UInt16& GetPetColorIndex() const { return m.petColorIndex; }
	const UInt8& GetMaxPartner() const { return m.maxPartner; }
	const UInt8& GetMaxPet() const { return m.maxPet; }
	const UInt16& GetPhoto_id() const { return m.photo_id; }
	const UInt16& GetHairStyle1() const { return m.hairStyle1; }
	const UInt16& GetHairStyle2() const { return m.hairStyle2; }
	const UInt16& GetHairColor() const { return m.hairColor; }
	const UInt32& GetEq_weapon() const { return m.eq_weapon; }
	const UInt16& GetEq_weaponColor() const { return m.eq_weaponColor; }
	const UInt32& GetEq_headBand() const { return m.eq_headBand; }
	const UInt16& GetEq_headColor() const { return m.eq_headColor; }
	const UInt32& GetEq_cloth() const { return m.eq_cloth; }
	const UInt16& GetEq_clothColor() const { return m.eq_clothColor; }
	const UInt8& GetState() const { return m.state; }
	const String& GetBuff() const { return m.buff; }
	const String& GetBuffCache() const { return m.buffCache; }
	const UInt16& GetShortcut(const UInt8 i) const { return m.shortcut[i]; }
	const UInt8& GetShortcuttype(const UInt8 i) const { return m.shortcuttype[i]; }
	const Boolean& GetHasParty() const { return m.hasParty; }
	const Boolean& GetIsLeader() const { return m.isLeader; }
	const RPCNetID& GetPartyNetID() const { return m.partyNetID; }
	const UInt16& GetPK_Value() const { return m.PK_Value; }
	const Boolean& GetEnablePK() const { return m.EnablePK; }
	const Boolean& GetEnableBlockGiven() const { return m.EnableBlockGiven; }
	const Boolean& GetEnableBlockParty() const { return m.EnableBlockParty; }
	const Boolean& GetEnableBlockMsg() const { return m.EnableBlockMsg; }
	const Boolean& GetEnableBlockTrading() const { return m.EnableBlockTrading; }
	const Boolean& GetEnableBlockMakeFriend() const { return m.EnableBlockMakeFriend; }
	const Boolean& GetEnableShowEquip() const { return m.EnableShowEquip; }
	const UInt16& GetOrg_hairStyle1() const { return m.org_hairStyle1; }
	const UInt16& GetOrg_hairColor() const { return m.org_hairColor; }
	const UInt32& GetOrg_eq_cloth() const { return m.org_eq_cloth; }
	const UInt16& GetOrg_eq_clothColor() const { return m.org_eq_clothColor; }
	const UInt16& GetSpeakCount() const { return m.speakCount; }
	const UInt32& GetOfflineTime() const { return m.offlineTime; }
	const String& GetHawkName() const { return m.hawkName; }
	const UInt16& GetTP() const { return m.TP; }
	const UInt32& GetModel_mobid() const { return m.Model_mobid; }
	const UInt32& GetSpeakBlock() const { return m.speakBlock; }
	const UInt32& GetHawkerTime() const { return m.hawkerTime; }
	const UInt8& GetAvailBackPack() const { return m.availBackPack; }
	const UInt8& GetAvailItemWareHouse() const { return m.availItemWareHouse; }
	const UInt8& GetAvailPartnerWareHouse() const { return m.availPartnerWareHouse; }
	const UInt8& GetAvailPetWareHouse() const { return m.availPetWareHouse; }
	const UInt32Vector& GetFollowerList() const { return m.followerList; }
	const UInt32& GetPvPScoreSingle() const { return m.PvPScoreSingle; }
	const UInt32& GetUnlock_time() const { return m.unlock_time; }
	const Boolean& GetYuanbaoLock() const { return m.yuanbaoLock; }
	const Boolean& GetBankLock() const { return m.bankLock; }
	const UInt32& GetSibling_id() const { return m.sibling_id; }
	const UInt32& GetMark() const { return m.mark; }
	const UInt32& GetWantedReward() const { return m.wantedReward; }
	const UInt32& GetGuildUid() const { return m.guildUid; }
	const UInt32& GetIHisGenAcc() const { return m.iHisGenAcc; }
	const UInt32& GetUiCurGenAcc() const { return m.uiCurGenAcc; }
	virtual void OnUpdateId(const UInt32 &acct_id, const UInt32 &char_id) {};
	virtual void OnUpdateLevelInfo(const UInt16 &level, const UInt32 &nextLvExp) {};
	virtual void OnUpdateExp(const UInt32 &exp) {};
	virtual void OnUpdateAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI) {};
	virtual void OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD) {};
	virtual void OnUpdateAttrPoint(const UInt16 &attrPoint) {};
	virtual void OnUpdateHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP) {};
	virtual void OnUpdateSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP) {};
	virtual void OnUpdateDP(const UInt16 &DP, const UInt16 &mDP, const UInt16 &maxDP) {};
	virtual void OnUpdateWC(const UInt16 &WC, const UInt16 &maxWC, const UInt16 &WcExp, const UInt32 &nextWCExp) {};
	virtual void OnUpdateVP(const UInt16 &VP, const UInt16 &maxVP) {};
	virtual void OnUpdateGoodness(const Int32 &goodness) {};
	virtual void OnUpdateHonor(const UInt32 &honor) {};
	virtual void OnUpdateTitle(const String &title, const UInt16 &title_id) {};
	virtual void OnUpdateManor(const UInt32 &manor, const UInt32 &pettwo_uid, const UInt32 &pettwo_id, const String &pettwoName, const UInt16 &pettwoColorIndex) {};
	virtual void OnUpdateDonate(const UInt32 &fDonate, const UInt32 &totalFDonate, const UInt32 &mDonate, const UInt32 &totalMDonate) {};
	virtual void OnUpdateYuanBao(const UInt32 &yuanBao, const UInt32 &yuanBaoTicket) {};
	virtual void OnUpdateMoney(const UInt32 &money) {};
	virtual void OnUpdateBankMoney(const UInt32 &bankMoney) {};
	virtual void OnUpdateSMoney(const UInt32 &SMoney) {};
	virtual void OnUpdatePartner(const UInt32 &partner_id, const UInt32 &partnerMob_id, const String &partnerName, const UInt16 &partnerColorIndex) {};
	virtual void OnUpdatePet(const UInt32 &pet_uid, const UInt32 &pet_id, const String &petName, const UInt16 &petColorIndex) {};
	virtual void OnUpdateMaxOther(const UInt8 &maxPartner, const UInt8 &maxPet) {};
	virtual void OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor) {};
	virtual void OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor) {};
	virtual void OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor) {};
	virtual void OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor) {};
	virtual void OnUpdateState(const UInt8 &state) {};
	virtual void OnUpdateBuff(const String &buff) {};
	virtual void OnUpdateBuffCache(const String &buffCache) {};
	virtual void OnUpdateShortcut(const UInt16 &shortcut0, const UInt16 &shortcut1, const UInt16 &shortcut2, const UInt16 &shortcut3, const UInt16 &shortcut4, const UInt16 &shortcut5, const UInt16 &shortcut6, const UInt16 &shortcut7, const UInt16 &shortcut8, const UInt16 &shortcut9, const UInt8 &shortcuttype0, const UInt8 &shortcuttype1, const UInt8 &shortcuttype2, const UInt8 &shortcuttype3, const UInt8 &shortcuttype4, const UInt8 &shortcuttype5, const UInt8 &shortcuttype6, const UInt8 &shortcuttype7, const UInt8 &shortcuttype8, const UInt8 &shortcuttype9) {};
	virtual void OnUpdatePartyInfo(const Boolean &hasParty, const Boolean &isLeader, const RPCNetID &partyNetID) {};
	virtual void OnUpdatePKInfo(const UInt16 &PK_Value) {};
	virtual void OnUpdateBattleInfo(const Boolean &EnablePK) {};
	virtual void OnUpdateSettingInfo(const Boolean &EnableBlockGiven, const Boolean &EnableBlockParty, const Boolean &EnableBlockMsg, const Boolean &EnableBlockTrading, const Boolean &EnableBlockMakeFriend, const Boolean &EnableShowEquip) {};
	virtual void OnUpdateOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor) {};
	virtual void OnUpdateOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor) {};
	virtual void OnUpdateSpeakCount(const UInt16 &speakCount) {};
	virtual void OnUpdateOfflineTime(const UInt32 &offlineTime) {};
	virtual void OnUpdateHawkInfo(const String &hawkName) {};
	virtual void OnUpdateTP(const UInt16 &TP, const UInt32 &Model_mobid) {};
	virtual void OnUpdateSpeakBlock(const UInt32 &speakBlock) {};
	virtual void OnUpdateHawkerTime(const UInt32 &hawkerTime) {};
	virtual void OnUpdateAvailBackPack(const UInt8 &availBackPack) {};
	virtual void OnUpdateAvailWareHouse(const UInt8 &availItemWareHouse, const UInt8 &availPartnerWareHouse, const UInt8 &availPetWareHouse) {};
	virtual void OnUpdateFollowerList(const UInt32Vector &followerList) {};
	virtual void OnUpdatePvPScoreSingle(const UInt32 &PvPScoreSingle) {};
	virtual void OnUpdateUnlockTime(const UInt32 &unlock_time) {};
	virtual void OnUpdateYuanbaoLock(const Boolean &yuanbaoLock) {};
	virtual void OnUpdateBankLock(const Boolean &bankLock) {};
	virtual void OnUpdateSiblingId(const UInt32 &sibling_id) {};
	virtual void OnUpdateMark(const UInt32 &mark) {};
	virtual void OnUpdateWantedReward(const UInt32 &wantedReward) {};
	virtual void OnUpdateGuildUid(const UInt32 &guildUid) {};
	virtual void OnUpdateGenerationAcc(const UInt32 &iHisGenAcc, const UInt32 &uiCurGenAcc) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 acct_id;
	UInt32 char_id;
	//levelInfo
	UInt16 level;
	UInt32 nextLvExp;
	//exp
	UInt32 exp;
	//attrib1
	UInt16 STA;
	UInt16 SPR;
	UInt16 STR;
	UInt16 CON;
	UInt16 AGI;
	//attrib2
	UInt16 ATK;
	UInt16 DEF;
	UInt16 HIT;
	UInt16 EVA;
	UInt16 POW;
	UInt16 SPD;
	//attrPoint
	UInt16 attrPoint;
	//HP
	UInt16 HP;
	UInt16 mHP;
	UInt16 maxHP;
	//SP
	UInt16 SP;
	UInt16 mSP;
	UInt16 maxSP;
	//DP
	UInt16 DP;
	UInt16 mDP;
	UInt16 maxDP;
	//WC
	UInt16 WC;
	UInt16 maxWC;
	UInt16 WcExp;
	UInt32 nextWCExp;
	//VP
	UInt16 VP;
	UInt16 maxVP;
	//goodness
	Int32 goodness;
	//honor
	UInt32 honor;
	//title
	String title;
	UInt16 title_id;
	//manor
	UInt32 manor;
	UInt32 pettwo_uid;
	UInt32 pettwo_id;
	String pettwoName;
	UInt16 pettwoColorIndex;
	//donate
	UInt32 fDonate;
	UInt32 totalFDonate;
	UInt32 mDonate;
	UInt32 totalMDonate;
	//yuanBao
	UInt32 yuanBao;
	UInt32 yuanBaoTicket;
	//money
	UInt32 money;
	//bankMoney
	UInt32 bankMoney;
	//SMoney
	UInt32 SMoney;
	//partner
	UInt32 partner_id;
	UInt32 partnerMob_id;
	String partnerName;
	UInt16 partnerColorIndex;
	//pet
	UInt32 pet_uid;
	UInt32 pet_id;
	String petName;
	UInt16 petColorIndex;
	//maxOther
	UInt8 maxPartner;
	UInt8 maxPet;
	//style
	UInt16 photo_id;
	UInt16 hairStyle1;
	UInt16 hairStyle2;
	UInt16 hairColor;
	//eqWeapon
	UInt32 eq_weapon;
	UInt16 eq_weaponColor;
	//eqHead
	UInt32 eq_headBand;
	UInt16 eq_headColor;
	//eqCloth
	UInt32 eq_cloth;
	UInt16 eq_clothColor;
	//state
	UInt8 state;
	//buff
	String buff;
	//buffCache
	String buffCache;
	//shortcut
	UInt16 shortcut[10];
	UInt8 shortcuttype[10];
	//partyInfo
	Boolean hasParty;
	Boolean isLeader;
	RPCNetID partyNetID;
	//PKInfo
	UInt16 PK_Value;
	//BattleInfo
	Boolean EnablePK;
	//SettingInfo
	Boolean EnableBlockGiven;
	Boolean EnableBlockParty;
	Boolean EnableBlockMsg;
	Boolean EnableBlockTrading;
	Boolean EnableBlockMakeFriend;
	Boolean EnableShowEquip;
	//orgHairStyle
	UInt16 org_hairStyle1;
	UInt16 org_hairColor;
	//orgEqCloth
	UInt32 org_eq_cloth;
	UInt16 org_eq_clothColor;
	//speakCount
	UInt16 speakCount;
	//offlineTime
	UInt32 offlineTime;
	//hawkInfo
	String hawkName;
	//TP
	UInt16 TP;
	UInt32 Model_mobid;
	//speakBlock
	UInt32 speakBlock;
	//hawkerTime
	UInt32 hawkerTime;
	//availBackPack
	UInt8 availBackPack;
	//availWareHouse
	UInt8 availItemWareHouse;
	UInt8 availPartnerWareHouse;
	UInt8 availPetWareHouse;
	//followerList
	UInt32Vector followerList;
	//PvPScoreSingle
	UInt32 PvPScoreSingle;
	//unlockTime
	UInt32 unlock_time;
	//yuanbaoLock
	Boolean yuanbaoLock;
	//bankLock
	Boolean bankLock;
	//SiblingId
	UInt32 sibling_id;
	//mark
	UInt32 mark;
	//wantedReward
	UInt32 wantedReward;
	//guildUid
	UInt32 guildUid;
	//GenerationAcc
	UInt32 iHisGenAcc;
	UInt32 uiCurGenAcc;
} m;

public:
	INLINE Character_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult Character_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1898843120>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::MoveTo(LPCPEER pPeer, RPCContext &context, const UInt16 &posX, const UInt16 &posY, const UInt16 &hint1, const UInt16 &hint2)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_MoveTo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(posX);
	pBuf->Write<UInt16>(posY);
	pBuf->Write<UInt16>(hint1);
	pBuf->Write<UInt16>(hint2);
	return RPCSend<1374051356>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::StopMove(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_StopMove);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1931221114>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::RespondTest(LPCPEER pPeer, RPCContext &context, const UInt32 &minRespond, const UInt32 &maxRespond, const float &avgRespond, const UInt32 &respondCount)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_RespondTest);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(minRespond);
	pBuf->Write<UInt32>(maxRespond);
	pBuf->Write<float>(avgRespond);
	pBuf->Write<UInt32>(respondCount);
	return RPCSend<954938274>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::TestBattle(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_TestBattle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1106660933>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::CancelAuto(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CancelAuto);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<778976857>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::LevelUp(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_LevelUp);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<207552678>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::RaiseAttrib(LPCPEER pPeer, RPCContext &context, const UInt16 &sta, const UInt16 &spr, const UInt16 &str, const UInt16 &con, const UInt16 &agi)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_RaiseAttrib);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(sta);
	pBuf->Write<UInt16>(spr);
	pBuf->Write<UInt16>(str);
	pBuf->Write<UInt16>(con);
	pBuf->Write<UInt16>(agi);
	return RPCSend<449666965>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::PKRequest(LPCPEER pPeer, RPCContext &context, const UInt32 &TargetID, const BYTE &Type)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_PKRequest);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(TargetID);
	pBuf->Write<BYTE>(Type);
	return RPCSend<949372009>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::LeaveWatching(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_LeaveWatching);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1228280283>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::MonitorBattle(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_MonitorBattle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(targetId);
	return RPCSend<900603833>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::JoinBattle(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_JoinBattle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<364977655>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::RequestJoinBattle(LPCPEER pPeer, RPCContext &context, const Boolean &Force)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_RequestJoinBattle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(Force);
	return RPCSend<107413759>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::RejectHelper(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_RejectHelper);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<1112875477>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::SetFollowPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_SetFollowPartner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(partner_id);
	return RPCSend<1639598290>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::SetFollowPet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_SetFollowPet);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(pet_id);
	return RPCSend<1596522635>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ChangeMapLine(LPCPEER pPeer, RPCContext &context, const UInt8 &line_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ChangeMapLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(line_id);
	return RPCSend<238376788>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::UpdateYuanBao(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_UpdateYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<2045444688>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::SearchChar(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_SearchChar);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<2106742247>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::SetTarget(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_SetTarget);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<1941011522>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::CreateParty(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CreateParty);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1089035928>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::PartyJoinRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_PartyJoinRequest);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(memberId);
	return RPCSend<2017599356>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::PartyJoinLevelControl(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const UInt16 &levelDown, const UInt16 &levelUp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_PartyJoinLevelControl);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(targetId);
	pBuf->Write<UInt16>(levelDown);
	pBuf->Write<UInt16>(levelUp);
	return RPCSend<265438586>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::PartyJoinReply(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const Boolean &agree, const Boolean &showMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_PartyJoinReply);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(targetId);
	pBuf->Write<Boolean>(agree);
	pBuf->Write<Boolean>(showMsg);
	return RPCSend<1367028673>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::PartyInviteRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_PartyInviteRequest);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(targetId);
	return RPCSend<62459794>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::PartyInviteReply(LPCPEER pPeer, RPCContext &context, const RPCNetID &partyId, const Boolean &agree, const Boolean &showMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_PartyInviteReply);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(partyId);
	pBuf->Write<Boolean>(agree);
	pBuf->Write<Boolean>(showMsg);
	return RPCSend<243883628>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::SearchParty(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_SearchParty);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<711031067>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::GetPartyInfo(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_GetPartyInfo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(memberId);
	return RPCSend<1765372048>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ShutNewGuide(LPCPEER pPeer, RPCContext &context, const UInt8 &flag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ShutNewGuide);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(flag);
	return RPCSend<1190462234>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::CheatCode(LPCPEER pPeer, RPCContext &context, const UInt32 &orderMakerAcct_id, const String &code)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CheatCode);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(orderMakerAcct_id);
	pBuf->Write<String>(code);
	return RPCSend<430328429>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::LearnSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_LearnSkill);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(skill_id);
	return RPCSend<263073416>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::UseSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const UInt32 &TargetID, const BYTE &TargetType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_UseSkill);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(skill_id);
	pBuf->Write<UInt32>(TargetID);
	pBuf->Write<BYTE>(TargetType);
	return RPCSend<2005626063>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::UpGradeSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_UpGradeSkill);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(skill_id);
	return RPCSend<1711035980>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::RemoveSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_RemoveSkill);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(skill_id);
	return RPCSend<1355220836>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::SkillShopLearn(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_SkillShopLearn);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(skill_id);
	return RPCSend<38816823>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::LeaveSkillShop(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_LeaveSkillShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1171791729>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::GivenProcess(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const UInt8Vector &itemIndexs, const UInt8Vector &itemStacks, const UInt32Vector &PartnerIDs, const UInt32Vector &PetIDs, const UInt32 &gmoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_GivenProcess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(targetId);
	pBuf->Write<UInt8Vector>(itemIndexs);
	pBuf->Write<UInt8Vector>(itemStacks);
	pBuf->Write<UInt32Vector>(PartnerIDs);
	pBuf->Write<UInt32Vector>(PetIDs);
	pBuf->Write<UInt32>(gmoney);
	return RPCSend<38934929>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::UpdateShortCutList(LPCPEER pPeer, RPCContext &context, const UInt16Vector &shortcut, const UInt8Vector &shortcuttype)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_UpdateShortCutList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16Vector>(shortcut);
	pBuf->Write<UInt8Vector>(shortcuttype);
	return RPCSend<1252286919>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::UpdateShortCut(LPCPEER pPeer, RPCContext &context, const UInt16 &shortcut, const UInt8 &shortcuttype, const UInt8 &position)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_UpdateShortCut);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(shortcut);
	pBuf->Write<UInt8>(shortcuttype);
	pBuf->Write<UInt8>(position);
	return RPCSend<1733707110>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::SaveSystemSetting(LPCPEER pPeer, RPCContext &context, const Boolean &EnableBlockGiven, const Boolean &EnableBlockParty, const Boolean &EnableBlockMsg, const Boolean &EnableBlockTrading, const Boolean &EnableBlockMakeFriend, const Boolean &EnableShowEquip)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_SaveSystemSetting);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(EnableBlockGiven);
	pBuf->Write<Boolean>(EnableBlockParty);
	pBuf->Write<Boolean>(EnableBlockMsg);
	pBuf->Write<Boolean>(EnableBlockTrading);
	pBuf->Write<Boolean>(EnableBlockMakeFriend);
	pBuf->Write<Boolean>(EnableShowEquip);
	return RPCSend<233990746>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ChangeMsgIcon(LPCPEER pPeer, RPCContext &context, const UInt16 &iconID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ChangeMsgIcon);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(iconID);
	return RPCSend<2126270512>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::AskForTrading(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_AskForTrading);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(targetId);
	return RPCSend<1446146462>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ReplyReqTrading(LPCPEER pPeer, RPCContext &context, const Boolean &trade, const RPCNetID &targetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ReplyReqTrading);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(trade);
	pBuf->Write<RPCNetID>(targetId);
	return RPCSend<1007357642>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::TalkToNPC(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_TalkToNPC);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(npc_id);
	return RPCSend<938031349>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ChangeTitle(LPCPEER pPeer, RPCContext &context, const UInt16 &title_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ChangeTitle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(title_id);
	return RPCSend<79375478>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::AnswerAntiHack(LPCPEER pPeer, RPCContext &context, const UInt8 &answer, const Boolean &is_right)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_AnswerAntiHack);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(answer);
	pBuf->Write<Boolean>(is_right);
	return RPCSend<1359102115>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::DepositInBank(LPCPEER pPeer, RPCContext &context, const UInt32 &money)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_DepositInBank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(money);
	return RPCSend<761400209>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::WithdrawFmBank(LPCPEER pPeer, RPCContext &context, const UInt32 &money)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_WithdrawFmBank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(money);
	return RPCSend<1746292659>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::StartHawk(LPCPEER pPeer, RPCContext &context, const String &hawkName, const HawkerItemInfoVector &itemList, const HawkerItemInfoVector &partnerList, const HawkerItemInfoVector &petList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_StartHawk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(hawkName);
	pBuf->Write<HawkerItemInfoVector>(itemList);
	pBuf->Write<HawkerItemInfoVector>(partnerList);
	pBuf->Write<HawkerItemInfoVector>(petList);
	return RPCSend<1878358502>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::GetHawkFromTarget(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_GetHawkFromTarget);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(targetId);
	return RPCSend<1610298636>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ClearHawkerHistory(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ClearHawkerHistory);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<291527370>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::SetListen(LPCPEER pPeer, RPCContext &context, const BOOL &IsListen)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_SetListen);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<BOOL>(IsListen);
	return RPCSend<1076771333>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::DonateMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &money)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_DonateMoney);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<UInt32>(money);
	return RPCSend<180895262>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::CreateGuild(LPCPEER pPeer, RPCContext &context, const String &name, const bool &cancel)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CreateGuild);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(name);
	pBuf->Write<bool>(cancel);
	return RPCSend<1859083855>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::EnterQuestion(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_EnterQuestion);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1428378490>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ExitQuestion(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ExitQuestion);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1316497505>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::CloseWareHouse(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CloseWareHouse);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1064722433>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::LockAsset(LPCPEER pPeer, RPCContext &context, const UInt32 &bagIndex, const UInt8 &assetType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_LockAsset);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(bagIndex);
	pBuf->Write<UInt8>(assetType);
	return RPCSend<379535554>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::UnlockAsset(LPCPEER pPeer, RPCContext &context, const ByteArray &password, const UInt32 &bagIndex, const UInt8 &assetType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_UnlockAsset);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<ByteArray>(password);
	pBuf->Write<UInt32>(bagIndex);
	pBuf->Write<UInt8>(assetType);
	return RPCSend<2109887002>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::SetAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_SetAssetPassword);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<ByteArray>(password);
	return RPCSend<1849129414>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ForceResetAssetPassword);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<2129725712>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ChangeAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &oldPassword, const ByteArray &newPassword)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ChangeAssetPassword);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<ByteArray>(oldPassword);
	pBuf->Write<ByteArray>(newPassword);
	return RPCSend<408777961>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::SetAutoWalk(LPCPEER pPeer, RPCContext &context, const Boolean &bRobot)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_SetAutoWalk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(bRobot);
	return RPCSend<764886076>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context, const Boolean &hit)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_SetAutoWalkHitBoss);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(hit);
	return RPCSend<793737424>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::KickMember(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &reason)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_KickMember);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<String>(reason);
	return RPCSend<1765681134>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::UsePartnerCollectionBuff(LPCPEER pPeer, RPCContext &context, const bool &bUse)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_UsePartnerCollectionBuff);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<bool>(bUse);
	return RPCSend<257650583>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_AddPartnerCollection);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(partner_id);
	return RPCSend<1860713316>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &changeType, const UInt32 &num, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ChangeBetweenYBandYBT);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(changeType);
	pBuf->Write<UInt32>(num);
	pBuf->Write<ByteArray>(password);
	return RPCSend<1131102497>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &pricePerUnit, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_AddSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(YBT);
	pBuf->Write<UInt32>(pricePerUnit);
	pBuf->Write<ByteArray>(password);
	return RPCSend<65100298>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID, const UInt32 &YBT, const UInt32 &moneyPerUnit, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_BuySellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(sellYuanBaoID);
	pBuf->Write<UInt32>(YBT);
	pBuf->Write<UInt32>(moneyPerUnit);
	pBuf->Write<ByteArray>(password);
	return RPCSend<1649043431>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_GetAllMoneyAndYBT);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<ByteArray>(password);
	return RPCSend<12871473>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::RequestTargetEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_RequestTargetEquip);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<1242454806>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::AddStartHawkItem(LPCPEER pPeer, RPCContext &context, const HawkerItemInfo &itemInfo, const StartHawkAddType &addType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_AddStartHawkItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<HawkerItemInfo>(itemInfo);
	pBuf->Write<StartHawkAddType>(addType);
	return RPCSend<1269175953>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ModifyLineup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(uiLineupId);
	return RPCSend<1428050325>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ModifyLineupKeyPos);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<int>(iKeyPos);
	return RPCSend<1345213674>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ForgiveLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ForgiveLineup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(uiLineupId);
	return RPCSend<1056645410>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::SetChangePos(LPCPEER pPeer, RPCContext &context, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_SetChangePos);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(ui8PreIndex);
	pBuf->Write<UInt8>(ui8DestIndex);
	return RPCSend<1375165978>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const String &targetName, const Int32 &money)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_UpdateWantedReward);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(targetId);
	pBuf->Write<String>(targetName);
	pBuf->Write<Int32>(money);
	return RPCSend<425606938>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::JailDelivery(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_JailDelivery);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(targetId);
	return RPCSend<1577334774>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::WantedBail(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_WantedBail);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(targetId);
	return RPCSend<1362182386>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::WantedTrace(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_WantedTrace);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(targetId);
	return RPCSend<1330533952>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::GetGenerationGroup(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_GetGenerationGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1394807366>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::RequestGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_RequestGeneration);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(uiTargetId);
	return RPCSend<1717319932>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::RequestGenerationAns(LPCPEER pPeer, RPCContext &context, const BOOL &bConfirm, const UInt32 &uiRelatedId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_RequestGenerationAns);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<BOOL>(bConfirm);
	pBuf->Write<UInt32>(uiRelatedId);
	return RPCSend<1011309521>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::DeleteGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiRelatedId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_DeleteGeneration);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(uiRelatedId);
	return RPCSend<368979257>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::GetSavedExp(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_GetSavedExp);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1830372486>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::BuyAccShopItem(LPCPEER pPeer, RPCContext &context, const UInt32 &iIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_BuyAccShopItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(iIndex);
	return RPCSend<1948866788>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::BuyPointsShopItem(LPCPEER pPeer, RPCContext &context, const int &iItemID, const UInt32 &nPointsShopID, const UInt32 &nPointsShopTypeID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_BuyPointsShopItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<int>(iItemID);
	pBuf->Write<UInt32>(nPointsShopID);
	pBuf->Write<UInt32>(nPointsShopTypeID);
	return RPCSend<1083574795>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::RequestCrossMapJump(LPCPEER pPeer, RPCContext &context, const UInt32 &uSrcJmpSeq, const UInt32 &uDstJmpSeq)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_RequestCrossMapJump);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(uSrcJmpSeq);
	pBuf->Write<UInt32>(uDstJmpSeq);
	return RPCSend<1326982159>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::LeaveTeamTemporarily(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_LeaveTeamTemporarily);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1174040656>(pPeer, pBuf);
}

INLINE RPCResult Character_Proxy::ReturnParty(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_ReturnParty);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1322191304>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(265173520)
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1898843120)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1374051356)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1931221114)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(954938274)
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1106660933)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(778976857)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(207552678)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(449666965)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(949372009)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1228280283)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(900603833)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(364977655)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(107413759)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1112875477)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1639598290)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1596522635)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(238376788)
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(2045444688)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(884230030)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1392330330)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(472343456)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(977666519)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1666675435)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(2106011260)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(787505021)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(272109665)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(825713249)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1499016738)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1043758759)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1840727934)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(440934718)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(2106742247)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(708988917)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1924562885)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1941011522)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(944992856)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1089035928)
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(2017599356)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(265438586)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1367028673)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(62459794)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(243883628)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(711031067)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1765372048)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(2004365301)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(2073589861)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(2016682674)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(451881969)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(611017579)
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1586313843)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1788162777)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(263587870)
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(408096850)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1680621049)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1617130850)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(164526723)
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1868450067)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(788955121)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(316170255)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1190462234)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1857582614)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(430328429)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(719536770)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(263073416)
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(2005626063)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1711035980)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1355220836)
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1640973944)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(440018441)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(101261627)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(301232851)
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(38816823)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1171791729)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(19806500)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(38934929)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1341011045)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(2120963231)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1958285418)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(57557994)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1349417469)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1252286919)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1733707110)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1195318687)
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(233990746)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1417040156)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(2126270512)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1446146462)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1983474478)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1007357642)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(753271876)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1233933636)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1591901068)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(938031349)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1600547679)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(79375478)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(174658801)
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1582920557)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1359102115)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1776368642)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(761400209)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1746292659)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(294275337)
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1436378679)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1878358502)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1480197420)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1610298636)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1847356866)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(664404854)
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(895776164)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(853112723)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(291527370)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1159035620)
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1076771333)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(180895262)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1859083855)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(850605881)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1389179735)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1802335796)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(161423045)
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1538414089)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(733353603)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(551160062)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1531910651)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1428378490)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1316497505)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1674016061)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1280472680)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1821640660)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(605690981)
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1064722433)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1243417419)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(379535554)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(2109887002)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1849129414)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(2129725712)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(408777961)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(841229587)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(580604749)
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1607548830)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(185282243)
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(270298762)
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1022762810)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1967736293)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(764886076)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(793737424)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(681538378)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1765681134)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(997279627)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1794267068)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(257650583)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1430018473)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1860713316)
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(944062561)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1131102497)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1247889129)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(65100298)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1987536093)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1649043431)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1081898104)
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(12871473)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1673862953)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1242454806)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1645497323)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1094416333)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1269175953)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1254319356)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(2042908018)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1428050325)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1567647451)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1345213674)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(969433236)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1056645410)
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(53431052)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1375165978)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(765602416)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(425606938)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1451180764)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1577334774)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(431580735)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1362182386)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1406492635)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1330533952)
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(609051308)
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(683047974)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1394807366)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(474378968)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1717319932)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(266625076)
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1011309521)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1510801710)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(368979257)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1402830072)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1219760122)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1830372486)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(270404594)
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1948866788)
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(829967264)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1728914333)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1083574795)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(947990062)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1214472072)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1208045910)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(2102293522)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1832715624)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1198619385)
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1326982159)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1174040656)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1326673354)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(2144671047)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1322191304)
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(993791940)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(526338154)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1958242924)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1810517220)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(236878823)
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_CHARACTER_PROXY_H_
