///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_CHARACTER_STUB_H_
#define _RPC_CHARACTER_STUB_H_

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

class Character_Stub : public CMapActor
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	virtual RPCResult SearchChar(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult SetTarget(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CreateParty(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult PartyJoinRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId);
	virtual RPCResult PartyJoinLevelControl(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const UInt16 &levelDown, const UInt16 &levelUp);
	virtual RPCResult PartyJoinReply(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const Boolean &agree, const Boolean &showMsg);
	virtual RPCResult PartyInviteRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId);
	virtual RPCResult PartyInviteReply(LPCPEER pPeer, RPCContext &context, const RPCNetID &partyId, const Boolean &agree, const Boolean &showMsg);
	virtual RPCResult SearchParty(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult GetPartyInfo(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId);
	virtual RPCResult ShutNewGuide(LPCPEER pPeer, RPCContext &context, const UInt8 &flag);
	virtual RPCResult CheatCode(LPCPEER pPeer, RPCContext &context, const UInt32 &orderMakerAcct_id, const String &code);
	virtual RPCResult LearnSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id);
	virtual RPCResult UseSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const UInt32 &TargetID, const BYTE &TargetType);
	virtual RPCResult UpGradeSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id);
	virtual RPCResult RemoveSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id);
	virtual RPCResult SkillShopLearn(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id);
	virtual RPCResult LeaveSkillShop(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult GivenProcess(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const UInt8Vector &itemIndexs, const UInt8Vector &itemStacks, const UInt32Vector &PartnerIDs, const UInt32Vector &PetIDs, const UInt32 &gmoney);
	virtual RPCResult UpdateShortCutList(LPCPEER pPeer, RPCContext &context, const UInt16Vector &shortcut, const UInt8Vector &shortcuttype);
	virtual RPCResult UpdateShortCut(LPCPEER pPeer, RPCContext &context, const UInt16 &shortcut, const UInt8 &shortcuttype, const UInt8 &position);
	virtual RPCResult SaveSystemSetting(LPCPEER pPeer, RPCContext &context, const Boolean &EnableBlockGiven, const Boolean &EnableBlockParty, const Boolean &EnableBlockMsg, const Boolean &EnableBlockTrading, const Boolean &EnableBlockMakeFriend, const Boolean &EnableShowEquip);
	virtual RPCResult ChangeMsgIcon(LPCPEER pPeer, RPCContext &context, const UInt16 &iconID);
	virtual RPCResult AskForTrading(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId);
	virtual RPCResult ReplyReqTrading(LPCPEER pPeer, RPCContext &context, const Boolean &trade, const RPCNetID &targetId);
	virtual RPCResult TalkToNPC(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id);
	virtual RPCResult ChangeTitle(LPCPEER pPeer, RPCContext &context, const UInt16 &title_id);
	virtual RPCResult AnswerAntiHack(LPCPEER pPeer, RPCContext &context, const UInt8 &answer, const Boolean &is_right);
	virtual RPCResult DepositInBank(LPCPEER pPeer, RPCContext &context, const UInt32 &money);
	virtual RPCResult WithdrawFmBank(LPCPEER pPeer, RPCContext &context, const UInt32 &money);
	virtual RPCResult StartHawk(LPCPEER pPeer, RPCContext &context, const String &hawkName, const HawkerItemInfoVector &itemList, const HawkerItemInfoVector &partnerList, const HawkerItemInfoVector &petList);
	virtual RPCResult GetHawkFromTarget(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult ClearHawkerHistory(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult SetListen(LPCPEER pPeer, RPCContext &context, const BOOL &IsListen);
	virtual RPCResult DonateMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &money);
	virtual RPCResult CreateGuild(LPCPEER pPeer, RPCContext &context, const String &name, const bool &cancel);
	virtual RPCResult EnterQuestion(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult ExitQuestion(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CloseWareHouse(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult LockAsset(LPCPEER pPeer, RPCContext &context, const UInt32 &bagIndex, const UInt8 &assetType);
	virtual RPCResult UnlockAsset(LPCPEER pPeer, RPCContext &context, const ByteArray &password, const UInt32 &bagIndex, const UInt8 &assetType);
	virtual RPCResult SetAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &password);
	virtual RPCResult ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult ChangeAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &oldPassword, const ByteArray &newPassword);
	virtual RPCResult SetAutoWalk(LPCPEER pPeer, RPCContext &context, const Boolean &bRobot);
	virtual RPCResult SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context, const Boolean &hit);
	virtual RPCResult KickMember(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &reason);
	virtual RPCResult UsePartnerCollectionBuff(LPCPEER pPeer, RPCContext &context, const bool &bUse);
	virtual RPCResult AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id);
	virtual RPCResult ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &changeType, const UInt32 &num, const ByteArray &password);
	virtual RPCResult AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &pricePerUnit, const ByteArray &password);
	virtual RPCResult BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID, const UInt32 &YBT, const UInt32 &moneyPerUnit, const ByteArray &password);
	virtual RPCResult GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const ByteArray &password);
	virtual RPCResult RequestTargetEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult AddStartHawkItem(LPCPEER pPeer, RPCContext &context, const HawkerItemInfo &itemInfo, const StartHawkAddType &addType);
	virtual RPCResult ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId);
	virtual RPCResult ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos);
	virtual RPCResult ForgiveLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId);
	virtual RPCResult SetChangePos(LPCPEER pPeer, RPCContext &context, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex);
	virtual RPCResult UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const String &targetName, const Int32 &money);
	virtual RPCResult JailDelivery(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult WantedBail(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult WantedTrace(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult GetGenerationGroup(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult RequestGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId);
	virtual RPCResult RequestGenerationAns(LPCPEER pPeer, RPCContext &context, const BOOL &bConfirm, const UInt32 &uiRelatedId);
	virtual RPCResult DeleteGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiRelatedId);
	virtual RPCResult GetSavedExp(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult BuyAccShopItem(LPCPEER pPeer, RPCContext &context, const UInt32 &iIndex);
	virtual RPCResult BuyPointsShopItem(LPCPEER pPeer, RPCContext &context, const int &iItemID, const UInt32 &nPointsShopID, const UInt32 &nPointsShopTypeID);
	virtual RPCResult RequestCrossMapJump(LPCPEER pPeer, RPCContext &context, const UInt32 &uSrcJmpSeq, const UInt32 &uDstJmpSeq);
	virtual RPCResult LeaveTeamTemporarily(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult ReturnParty(LPCPEER pPeer, RPCContext &context);
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
	virtual RPCResult MoveTo(LPCPEER pPeer, RPCContext &context, const UInt16 &posX, const UInt16 &posY, const UInt16 &hint1, const UInt16 &hint2) = 0;
	virtual RPCResult StopMove(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult RespondTest(LPCPEER pPeer, RPCContext &context, const UInt32 &minRespond, const UInt32 &maxRespond, const float &avgRespond, const UInt32 &respondCount) = 0;
	virtual RPCResult TestBattle(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CancelAuto(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult LevelUp(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult RaiseAttrib(LPCPEER pPeer, RPCContext &context, const UInt16 &sta, const UInt16 &spr, const UInt16 &str, const UInt16 &con, const UInt16 &agi) = 0;
	virtual RPCResult PKRequest(LPCPEER pPeer, RPCContext &context, const UInt32 &TargetID, const BYTE &Type) = 0;
	virtual RPCResult LeaveWatching(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult MonitorBattle(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId) = 0;
	virtual RPCResult JoinBattle(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult RequestJoinBattle(LPCPEER pPeer, RPCContext &context, const Boolean &Force) = 0;
	virtual RPCResult RejectHelper(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult SetFollowPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id) = 0;
	virtual RPCResult SetFollowPet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_id) = 0;
	virtual RPCResult ChangeMapLine(LPCPEER pPeer, RPCContext &context, const UInt8 &line_id) = 0;
	virtual RPCResult UpdateYuanBao(LPCPEER pPeer, RPCContext &context) = 0;
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
	virtual RPCResult SearchChar(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_SearchChar(LPCPEER pPeer, RPCContext &context, const CharInfoVector &CharInfoList);
	virtual RPCResult CB_OpenWeaponsRankPanel(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult SetTarget(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult CB_SetTarget(LPCPEER pPeer, RPCContext &context, const Boolean &bResult, const UInt32 &char_id, const UInt16 &x, const UInt16 &y);
	virtual RPCResult CreateParty(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult PartyJoinRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId) = 0;
	virtual RPCResult PartyJoinLevelControl(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const UInt16 &levelDown, const UInt16 &levelUp) = 0;
	virtual RPCResult PartyJoinReply(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const Boolean &agree, const Boolean &showMsg) = 0;
	virtual RPCResult PartyInviteRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId) = 0;
	virtual RPCResult PartyInviteReply(LPCPEER pPeer, RPCContext &context, const RPCNetID &partyId, const Boolean &agree, const Boolean &showMsg) = 0;
	virtual RPCResult SearchParty(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult GetPartyInfo(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId) = 0;
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
	virtual RPCResult ShutNewGuide(LPCPEER pPeer, RPCContext &context, const UInt8 &flag) = 0;
	virtual RPCResult CB_HandleGuideEvent(LPCPEER pPeer, RPCContext &context, const UInt16 &ID);
	virtual RPCResult CheatCode(LPCPEER pPeer, RPCContext &context, const UInt32 &orderMakerAcct_id, const String &code) = 0;
	virtual RPCResult CB_RespondTest(LPCPEER pPeer, RPCContext &context, const UInt16 &playerCount);
	virtual RPCResult LearnSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id) = 0;
	virtual RPCResult UseSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const UInt32 &TargetID, const BYTE &TargetType) = 0;
	virtual RPCResult UpGradeSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id) = 0;
	virtual RPCResult RemoveSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id) = 0;
	virtual RPCResult CB_LearnSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag);
	virtual RPCResult CB_UseSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag, const String &Msg, const UInt32 &SrcID, const UInt32 &TargetID);
	virtual RPCResult CB_UpGradeSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag);
	virtual RPCResult CB_RemoveSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag);
	virtual RPCResult SkillShopLearn(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id) = 0;
	virtual RPCResult LeaveSkillShop(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_SkillShopLearn(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag);
	virtual RPCResult GivenProcess(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const UInt8Vector &itemIndexs, const UInt8Vector &itemStacks, const UInt32Vector &PartnerIDs, const UInt32Vector &PetIDs, const UInt32 &gmoney) = 0;
	virtual RPCResult CB_GivenProcess(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgIndex);
	virtual RPCResult CB_GivenItem(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const UInt32Vector &itemID, const UInt8Vector &itemStacks);
	virtual RPCResult CB_GivenPartner(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const StringVector &PartnerNames);
	virtual RPCResult CB_GivenPet(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const StringVector &PetNames);
	virtual RPCResult CB_GivenMoney(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const UInt32 &gmoney);
	virtual RPCResult UpdateShortCutList(LPCPEER pPeer, RPCContext &context, const UInt16Vector &shortcut, const UInt8Vector &shortcuttype) = 0;
	virtual RPCResult UpdateShortCut(LPCPEER pPeer, RPCContext &context, const UInt16 &shortcut, const UInt8 &shortcuttype, const UInt8 &position) = 0;
	virtual RPCResult CB_UpdateShortCutSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult SaveSystemSetting(LPCPEER pPeer, RPCContext &context, const Boolean &EnableBlockGiven, const Boolean &EnableBlockParty, const Boolean &EnableBlockMsg, const Boolean &EnableBlockTrading, const Boolean &EnableBlockMakeFriend, const Boolean &EnableShowEquip) = 0;
	virtual RPCResult CB_SaveSystemSetting(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult ChangeMsgIcon(LPCPEER pPeer, RPCContext &context, const UInt16 &iconID) = 0;
	virtual RPCResult AskForTrading(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId) = 0;
	virtual RPCResult CB_AskForTrading(LPCPEER pPeer, RPCContext &context, const RPCNetID &askId);
	virtual RPCResult ReplyReqTrading(LPCPEER pPeer, RPCContext &context, const Boolean &trade, const RPCNetID &targetId) = 0;
	virtual RPCResult CB_ReplyReqTrading(LPCPEER pPeer, RPCContext &context, const Boolean &trade);
	virtual RPCResult CB_TradingMessage(LPCPEER pPeer, RPCContext &context, const Boolean &trade, const UInt8 &msgIndex);
	virtual RPCResult CB_TradingObject(LPCPEER pPeer, RPCContext &context, NetObject* trading);
	virtual RPCResult TalkToNPC(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id) = 0;
	virtual RPCResult CB_TalkToNPC(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const bool &bResult);
	virtual RPCResult ChangeTitle(LPCPEER pPeer, RPCContext &context, const UInt16 &title_id) = 0;
	virtual RPCResult CB_TitleListUpdate(LPCPEER pPeer, RPCContext &context, const UInt16Vector &title_id_list, const StringVector &title_string_list, const UInt8Vector &updateState);
	virtual RPCResult CB_AntiHack(LPCPEER pPeer, RPCContext &context, const ByteArray &image, const AntiHackData &data);
	virtual RPCResult AnswerAntiHack(LPCPEER pPeer, RPCContext &context, const UInt8 &answer, const Boolean &is_right) = 0;
	virtual RPCResult CB_ReportAntiHack(LPCPEER pPeer, RPCContext &context, const Boolean &is_right, const UInt8 &cwrong, const UInt16 &reward);
	virtual RPCResult DepositInBank(LPCPEER pPeer, RPCContext &context, const UInt32 &money) = 0;
	virtual RPCResult WithdrawFmBank(LPCPEER pPeer, RPCContext &context, const UInt32 &money) = 0;
	virtual RPCResult CB_DepositInBank(LPCPEER pPeer, RPCContext &context, const Boolean &success);
	virtual RPCResult CB_WithdrawFmBank(LPCPEER pPeer, RPCContext &context, const Boolean &success);
	virtual RPCResult StartHawk(LPCPEER pPeer, RPCContext &context, const String &hawkName, const HawkerItemInfoVector &itemList, const HawkerItemInfoVector &partnerList, const HawkerItemInfoVector &petList) = 0;
	virtual RPCResult CB_StartHawk(LPCPEER pPeer, RPCContext &context, NetGroup* hawkGroup, const UInt8 &successIndex);
	virtual RPCResult GetHawkFromTarget(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId) = 0;
	virtual RPCResult CB_GetHawk(LPCPEER pPeer, RPCContext &context, const Boolean &self, NetGroup* hawkGroup, NetGroup* itemGroup, NetGroup* partnerGroup, const UInt16Vector &partnerSkillID, NetGroup* petGroup, const UInt16Vector &petSkillID);
	virtual RPCResult CB_HawkMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult CB_HawkHistory(LPCPEER pPeer, RPCContext &context, const HawkerHistoryVector &historys, const StringVector &itemNames);
	virtual RPCResult CB_UpdateHawkHistory(LPCPEER pPeer, RPCContext &context, const HawkerHistoryVector &historys, const StringVector &itemNames, const UInt32Vector &deleteHistoryID);
	virtual RPCResult ClearHawkerHistory(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &Result, const UInt32 &resultFlag);
	virtual RPCResult SetListen(LPCPEER pPeer, RPCContext &context, const BOOL &IsListen) = 0;
	virtual RPCResult DonateMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &money) = 0;
	virtual RPCResult CreateGuild(LPCPEER pPeer, RPCContext &context, const String &name, const bool &cancel) = 0;
	virtual RPCResult CB_OpenCreateGuild(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AwardMsg(LPCPEER pPeer, RPCContext &context, const String &msg);
	virtual RPCResult CB_OpenGiftBox(LPCPEER pPeer, RPCContext &context, NetGroup* giftBox);
	virtual RPCResult CB_AddictionWarning(LPCPEER pPeer, RPCContext &context, const String &warnMsg);
	virtual RPCResult CB_AddictCurrentStatus(LPCPEER pPeer, RPCContext &context, const String &statusMsg);
	virtual RPCResult CB_AddictionShutDown(LPCPEER pPeer, RPCContext &context, const UInt32 &accOffTime);
	virtual RPCResult CB_PlyInterestDwn(LPCPEER pPeer, RPCContext &context, const UInt8 &interest);
	virtual RPCResult CB_UseItemCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &item_id, const UInt32 &SrcID, const UInt32 &TargetID);
	virtual RPCResult EnterQuestion(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult ExitQuestion(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_AskEnterQuestion(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_EnterQuestion(LPCPEER pPeer, RPCContext &context, NetObject* pQuestion);
	virtual RPCResult CB_ExitQuestion(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AnswerQuestion(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const UInt32 &CharID);
	virtual RPCResult CloseWareHouse(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_OpenWareHouse(LPCPEER pPeer, RPCContext &context, NetGroup* items, NetGroup* partners, NetGroup* pets, NetGroup* partnerSkillGroup, NetGroup* petSkillGroup);
	virtual RPCResult LockAsset(LPCPEER pPeer, RPCContext &context, const UInt32 &bagIndex, const UInt8 &assetType) = 0;
	virtual RPCResult UnlockAsset(LPCPEER pPeer, RPCContext &context, const ByteArray &password, const UInt32 &bagIndex, const UInt8 &assetType) = 0;
	virtual RPCResult SetAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &password) = 0;
	virtual RPCResult ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult ChangeAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &oldPassword, const ByteArray &newPassword) = 0;
	virtual RPCResult CB_AssetProtectResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &index, const UInt8 &assetType);
	virtual RPCResult CB_AssetReleaseLockResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &index, const UInt8 &assetType);
	virtual RPCResult CB_SetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success);
	virtual RPCResult CB_ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success);
	virtual RPCResult CB_SetRobot(LPCPEER pPeer, RPCContext &context, const Boolean &bRobot, const String &robotScript);
	virtual RPCResult CB_RobotMapAction(LPCPEER pPeer, RPCContext &context, const String &func, const RPCNetID &mapNetId, const RPCNetID &charNetId, const RPCNetID &otherNetId);
	virtual RPCResult CB_RobotBattleTurn(LPCPEER pPeer, RPCContext &context, const String &func, const RPCNetID &battleNetId, const RPCNetID &charNetId, const RPCNetID &batCharNetId, const RPCNetID &partnerNetId);
	virtual RPCResult SetAutoWalk(LPCPEER pPeer, RPCContext &context, const Boolean &bRobot) = 0;
	virtual RPCResult SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context, const Boolean &hit) = 0;
	virtual RPCResult CB_SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context, const Boolean &hit);
	virtual RPCResult KickMember(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &reason) = 0;
	virtual RPCResult CB_DonateResult(LPCPEER pPeer, RPCContext &context, const UInt32 &donate, const UInt8 &returnFlag, const UInt32 &donateMoney);
	virtual RPCResult CB_GetPartnerCollectionList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &PartnerIDs);
	virtual RPCResult UsePartnerCollectionBuff(LPCPEER pPeer, RPCContext &context, const bool &bUse) = 0;
	virtual RPCResult CB_UsePartnerCollectionBuff(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag);
	virtual RPCResult AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id) = 0;
	virtual RPCResult CB_AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &MOBid, const BYTE &returnFlag);
	virtual RPCResult ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &changeType, const UInt32 &num, const ByteArray &password) = 0;
	virtual RPCResult CB_ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &pricePerUnit, const ByteArray &password) = 0;
	virtual RPCResult CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID, const UInt32 &YBT, const UInt32 &moneyPerUnit, const ByteArray &password) = 0;
	virtual RPCResult CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const ByteArray &password) = 0;
	virtual RPCResult CB_GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult RequestTargetEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult CB_RequestTargetEquip(LPCPEER pPeer, RPCContext &context, NetGroup* items);
	virtual RPCResult CB_RelationSystem(LPCPEER pPeer, RPCContext &context, NetObject* pRelationSystem);
	virtual RPCResult AddStartHawkItem(LPCPEER pPeer, RPCContext &context, const HawkerItemInfo &itemInfo, const StartHawkAddType &addType) = 0;
	virtual RPCResult CB_SetTargetNameFormHawk(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &char_name, const Boolean &isonline);
	virtual RPCResult CB_InitTargetHawkConcernState(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId) = 0;
	virtual RPCResult CB_ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId);
	virtual RPCResult ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos) = 0;
	virtual RPCResult CB_ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos);
	virtual RPCResult ForgiveLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId) = 0;
	virtual RPCResult CB_ForgiveLineup(LPCPEER pPeer, RPCContext &context, const int &iResault);
	virtual RPCResult SetChangePos(LPCPEER pPeer, RPCContext &context, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex) = 0;
	virtual RPCResult CB_SetChangePos(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex);
	virtual RPCResult UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const String &targetName, const Int32 &money) = 0;
	virtual RPCResult CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money, const Boolean &result);
	virtual RPCResult JailDelivery(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId) = 0;
	virtual RPCResult CB_JailDelivery(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult WantedBail(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId) = 0;
	virtual RPCResult CB_WantedBail(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult WantedTrace(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId) = 0;
	virtual RPCResult CB_WantedTrace(LPCPEER pPeer, RPCContext &context, const String &name, const UInt32 &lineId, const UInt32 &mapId, const UInt32 &mapX, const UInt32 &mapY);
	virtual RPCResult CB_RequestWantedTrace(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult GetGenerationGroup(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_GetGenerationGroup(LPCPEER pPeer, RPCContext &context, NetGroup* pGntGrp);
	virtual RPCResult RequestGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId) = 0;
	virtual RPCResult CB_RequestGenerationTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiRequstId);
	virtual RPCResult RequestGenerationAns(LPCPEER pPeer, RPCContext &context, const BOOL &bConfirm, const UInt32 &uiRelatedId) = 0;
	virtual RPCResult CB_RequestGenerationResult(LPCPEER pPeer, RPCContext &context, const int &iResultCode, const UInt32 &uiRelatedId);
	virtual RPCResult DeleteGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiRelatedId) = 0;
	virtual RPCResult CB_DeleteGeneration(LPCPEER pPeer, RPCContext &context, const int &iResultCode, const UInt32 &uiRelatedId);
	virtual RPCResult CB_ChangeGenerationType(LPCPEER pPeer, RPCContext &context, const int &iType);
	virtual RPCResult GetSavedExp(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_UpdateGenerationAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &uiCurAcc, const UInt32 &uiHisAcc);
	virtual RPCResult BuyAccShopItem(LPCPEER pPeer, RPCContext &context, const UInt32 &iIndex) = 0;
	virtual RPCResult CB_BugAccShopItem(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &iIndex);
	virtual RPCResult CB_GenerationGainAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &iGainedAcc);
	virtual RPCResult BuyPointsShopItem(LPCPEER pPeer, RPCContext &context, const int &iItemID, const UInt32 &nPointsShopID, const UInt32 &nPointsShopTypeID) = 0;
	virtual RPCResult CB_BuyPointsShopItem(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &uiPoints);
	virtual RPCResult CB_OpenBusinessLog(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_SetMarriageNpcAction(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_uid, const UInt8 &atcion, const UInt8 &direction);
	virtual RPCResult CB_ShowMarriageEff(LPCPEER pPeer, RPCContext &context, const UInt8 &state, const UInt32 &husbandID, const UInt32 &wifeID);
	virtual RPCResult CB_ResetChangeLine(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_PetUseSkillMsg(LPCPEER pPeer, RPCContext &context, const String &Msg);
	virtual RPCResult RequestCrossMapJump(LPCPEER pPeer, RPCContext &context, const UInt32 &uSrcJmpSeq, const UInt32 &uDstJmpSeq) = 0;
	virtual RPCResult LeaveTeamTemporarily(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_LeaveTeamTemporarily(LPCPEER pPeer, RPCContext &context, const UInt32 &uRetCode, const UInt32 &uId);
	virtual RPCResult CB_GetPartyInfoWhenJumpMap(LPCPEER pPeer, RPCContext &context, NetGroup* pParty);
	virtual RPCResult ReturnParty(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_ReturnParty(LPCPEER pPeer, RPCContext &context, const UInt32 &uRetCode, NetObject* pChar);
	virtual RPCResult CB_RefreshPartyCharInfo(LPCPEER pPeer, RPCContext &context, NetObject* pPartyCharInfo);
	virtual RPCResult CB_RestorePartyAsTemp(LPCPEER pPeer, RPCContext &context, NetGroup* pParty);
	virtual RPCResult CB_SetChangeModelSkillGroup(LPCPEER pPeer, RPCContext &context, NetGroup* skillGroup);
	virtual RPCResult CB_ShowIncognitoBoxEff(LPCPEER pPeer, RPCContext &context, const UInt16 &posx, const UInt16 &posy, const UInt8 &effType);
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
	void SetId(const UInt32 &acct_id, const UInt32 &char_id);
	void SetAcct_id(const UInt32 &acct_id);
	void SetChar_id(const UInt32 &char_id);
	void SetLevelInfo(const UInt16 &level, const UInt32 &nextLvExp);
	void SetLevel(const UInt16 &level);
	void SetNextLvExp(const UInt32 &nextLvExp);
	void SetExp(const UInt32 &exp);
	void SetAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI);
	void SetSTA(const UInt16 &STA);
	void SetSPR(const UInt16 &SPR);
	void SetSTR(const UInt16 &STR);
	void SetCON(const UInt16 &CON);
	void SetAGI(const UInt16 &AGI);
	void SetAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD);
	void SetATK(const UInt16 &ATK);
	void SetDEF(const UInt16 &DEF);
	void SetHIT(const UInt16 &HIT);
	void SetEVA(const UInt16 &EVA);
	void SetPOW(const UInt16 &POW);
	void SetSPD(const UInt16 &SPD);
	void SetAttrPoint(const UInt16 &attrPoint);
	void SetHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP);
	void SetHP(const UInt16 &HP);
	void SetMHP(const UInt16 &mHP);
	void SetMaxHP(const UInt16 &maxHP);
	void SetSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP);
	void SetSP(const UInt16 &SP);
	void SetMSP(const UInt16 &mSP);
	void SetMaxSP(const UInt16 &maxSP);
	void SetDP(const UInt16 &DP, const UInt16 &mDP, const UInt16 &maxDP);
	void SetDP(const UInt16 &DP);
	void SetMDP(const UInt16 &mDP);
	void SetMaxDP(const UInt16 &maxDP);
	void SetWC(const UInt16 &WC, const UInt16 &maxWC, const UInt16 &WcExp, const UInt32 &nextWCExp);
	void SetWC(const UInt16 &WC);
	void SetMaxWC(const UInt16 &maxWC);
	void SetWcExp(const UInt16 &WcExp);
	void SetNextWCExp(const UInt32 &nextWCExp);
	void SetVP(const UInt16 &VP, const UInt16 &maxVP);
	void SetVP(const UInt16 &VP);
	void SetMaxVP(const UInt16 &maxVP);
	void SetGoodness(const Int32 &goodness);
	void SetHonor(const UInt32 &honor);
	void SetTitle(const String &title, const UInt16 &title_id);
	void SetTitle(const String &title);
	void SetTitle_id(const UInt16 &title_id);
	void SetManor(const UInt32 &manor, const UInt32 &pettwo_uid, const UInt32 &pettwo_id, const String &pettwoName, const UInt16 &pettwoColorIndex);
	void SetManor(const UInt32 &manor);
	void SetPettwo_uid(const UInt32 &pettwo_uid);
	void SetPettwo_id(const UInt32 &pettwo_id);
	void SetPettwoName(const String &pettwoName);
	void SetPettwoColorIndex(const UInt16 &pettwoColorIndex);
	void SetDonate(const UInt32 &fDonate, const UInt32 &totalFDonate, const UInt32 &mDonate, const UInt32 &totalMDonate);
	void SetFDonate(const UInt32 &fDonate);
	void SetTotalFDonate(const UInt32 &totalFDonate);
	void SetMDonate(const UInt32 &mDonate);
	void SetTotalMDonate(const UInt32 &totalMDonate);
	void SetYuanBao(const UInt32 &yuanBao, const UInt32 &yuanBaoTicket);
	void SetYuanBao(const UInt32 &yuanBao);
	void SetYuanBaoTicket(const UInt32 &yuanBaoTicket);
	void SetMoney(const UInt32 &money);
	void SetBankMoney(const UInt32 &bankMoney);
	void SetSMoney(const UInt32 &SMoney);
	void SetPartner(const UInt32 &partner_id, const UInt32 &partnerMob_id, const String &partnerName, const UInt16 &partnerColorIndex);
	void SetPartner_id(const UInt32 &partner_id);
	void SetPartnerMob_id(const UInt32 &partnerMob_id);
	void SetPartnerName(const String &partnerName);
	void SetPartnerColorIndex(const UInt16 &partnerColorIndex);
	void SetPet(const UInt32 &pet_uid, const UInt32 &pet_id, const String &petName, const UInt16 &petColorIndex);
	void SetPet_uid(const UInt32 &pet_uid);
	void SetPet_id(const UInt32 &pet_id);
	void SetPetName(const String &petName);
	void SetPetColorIndex(const UInt16 &petColorIndex);
	void SetMaxOther(const UInt8 &maxPartner, const UInt8 &maxPet);
	void SetMaxPartner(const UInt8 &maxPartner);
	void SetMaxPet(const UInt8 &maxPet);
	void SetStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor);
	void SetPhoto_id(const UInt16 &photo_id);
	void SetHairStyle1(const UInt16 &hairStyle1);
	void SetHairStyle2(const UInt16 &hairStyle2);
	void SetHairColor(const UInt16 &hairColor);
	void SetEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);
	void SetEq_weapon(const UInt32 &eq_weapon);
	void SetEq_weaponColor(const UInt16 &eq_weaponColor);
	void SetEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
	void SetEq_headBand(const UInt32 &eq_headBand);
	void SetEq_headColor(const UInt16 &eq_headColor);
	void SetEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor);
	void SetEq_cloth(const UInt32 &eq_cloth);
	void SetEq_clothColor(const UInt16 &eq_clothColor);
	void SetState(const UInt8 &state);
	void SetBuff(const String &buff);
	void SetBuffCache(const String &buffCache);
	void SetShortcut(const UInt16 &shortcut0, const UInt16 &shortcut1, const UInt16 &shortcut2, const UInt16 &shortcut3, const UInt16 &shortcut4, const UInt16 &shortcut5, const UInt16 &shortcut6, const UInt16 &shortcut7, const UInt16 &shortcut8, const UInt16 &shortcut9, const UInt8 &shortcuttype0, const UInt8 &shortcuttype1, const UInt8 &shortcuttype2, const UInt8 &shortcuttype3, const UInt8 &shortcuttype4, const UInt8 &shortcuttype5, const UInt8 &shortcuttype6, const UInt8 &shortcuttype7, const UInt8 &shortcuttype8, const UInt8 &shortcuttype9);
	void SetShortcut(const UInt8 i, const UInt16 &shortcut);
	void SetShortcuttype(const UInt8 i, const UInt8 &shortcuttype);
	void SetPartyInfo(const Boolean &hasParty, const Boolean &isLeader, const RPCNetID &partyNetID);
	void SetHasParty(const Boolean &hasParty);
	void SetIsLeader(const Boolean &isLeader);
	void SetPartyNetID(const RPCNetID &partyNetID);
	void SetPKInfo(const UInt16 &PK_Value);
	void SetPK_Value(const UInt16 &PK_Value);
	void SetBattleInfo(const Boolean &EnablePK);
	void SetEnablePK(const Boolean &EnablePK);
	void SetSettingInfo(const Boolean &EnableBlockGiven, const Boolean &EnableBlockParty, const Boolean &EnableBlockMsg, const Boolean &EnableBlockTrading, const Boolean &EnableBlockMakeFriend, const Boolean &EnableShowEquip);
	void SetEnableBlockGiven(const Boolean &EnableBlockGiven);
	void SetEnableBlockParty(const Boolean &EnableBlockParty);
	void SetEnableBlockMsg(const Boolean &EnableBlockMsg);
	void SetEnableBlockTrading(const Boolean &EnableBlockTrading);
	void SetEnableBlockMakeFriend(const Boolean &EnableBlockMakeFriend);
	void SetEnableShowEquip(const Boolean &EnableShowEquip);
	void SetOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor);
	void SetOrg_hairStyle1(const UInt16 &org_hairStyle1);
	void SetOrg_hairColor(const UInt16 &org_hairColor);
	void SetOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor);
	void SetOrg_eq_cloth(const UInt32 &org_eq_cloth);
	void SetOrg_eq_clothColor(const UInt16 &org_eq_clothColor);
	void SetSpeakCount(const UInt16 &speakCount);
	void SetOfflineTime(const UInt32 &offlineTime);
	void SetHawkInfo(const String &hawkName);
	void SetHawkName(const String &hawkName);
	void SetTP(const UInt16 &TP, const UInt32 &Model_mobid);
	void SetTP(const UInt16 &TP);
	void SetModel_mobid(const UInt32 &Model_mobid);
	void SetSpeakBlock(const UInt32 &speakBlock);
	void SetHawkerTime(const UInt32 &hawkerTime);
	void SetAvailBackPack(const UInt8 &availBackPack);
	void SetAvailWareHouse(const UInt8 &availItemWareHouse, const UInt8 &availPartnerWareHouse, const UInt8 &availPetWareHouse);
	void SetAvailItemWareHouse(const UInt8 &availItemWareHouse);
	void SetAvailPartnerWareHouse(const UInt8 &availPartnerWareHouse);
	void SetAvailPetWareHouse(const UInt8 &availPetWareHouse);
	void SetFollowerList(const UInt32Vector &followerList);
	void SetPvPScoreSingle(const UInt32 &PvPScoreSingle);
	void SetUnlockTime(const UInt32 &unlock_time);
	void SetUnlock_time(const UInt32 &unlock_time);
	void SetYuanbaoLock(const Boolean &yuanbaoLock);
	void SetBankLock(const Boolean &bankLock);
	void SetSiblingId(const UInt32 &sibling_id);
	void SetSibling_id(const UInt32 &sibling_id);
	void SetMark(const UInt32 &mark);
	void SetWantedReward(const UInt32 &wantedReward);
	void SetGuildUid(const UInt32 &guildUid);
	void SetGenerationAcc(const UInt32 &iHisGenAcc, const UInt32 &uiCurGenAcc);
	void SetIHisGenAcc(const UInt32 &iHisGenAcc);
	void SetUiCurGenAcc(const UInt32 &uiCurGenAcc);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE Character_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_CHARACTER_STUB_H_
