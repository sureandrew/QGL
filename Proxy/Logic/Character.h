#pragma once
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "Proxy/Logic/Character_Proxy.h"
#include "Proxy/Logic/Generation.h"

class MapActor;
class Map;
class CCharacterControl;
class CPet;

class CCharacter : public Character_Proxy
{
public:
	DEFINE_CLASS(Character_CLASSID);
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
	virtual RPCResult CB_SetTarget(LPCPEER pPeer, RPCContext &context, const Boolean &bResult, const UInt32 &char_id, const UInt16 &x, const UInt16 &y);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_JoinParty(LPCPEER pPeer, RPCContext &context, NetGroup* party);
	virtual RPCResult CB_ChangeDesc(LPCPEER pPeer, RPCContext &context, NetGroup* party);
	virtual RPCResult CB_PartyRequestOk(LPCPEER pPeer, RPCContext &context, const String &name);
	virtual RPCResult CB_PartyRequestNo(LPCPEER pPeer, RPCContext &context, const UInt16 &nState);
	virtual RPCResult CB_PartyInviteOk(LPCPEER pPeer, RPCContext &context, const String &name);
	virtual RPCResult CB_PartyJoinRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId);
	virtual RPCResult CB_PartyInviteRequest(LPCPEER pPeer, RPCContext &context, NetGroup* party);
	virtual RPCResult CB_PartyClearRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const Boolean &showMsg);
	virtual RPCResult CB_PartyClearInvite(LPCPEER pPeer, RPCContext &context, const RPCNetID &partyId, const Boolean &showMsg);
	virtual RPCResult CB_PartyMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgType, const bool &inviteOrJoin, const String &charName);
	virtual RPCResult CB_SearchParty(LPCPEER pPeer, RPCContext &context, const PartyInfoVector &PartyInfoList);
	virtual RPCResult CB_SearchChar(LPCPEER pPeer, RPCContext &context, const CharInfoVector &CharInfoList);
	virtual RPCResult CB_GetPartyInfo(LPCPEER pPeer, RPCContext &context, const PartyInfoVector &PartyInfoList);
	virtual RPCResult CB_OpenSearchPartyPanel(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_OpenWeaponsRankPanel(LPCPEER pPeer, RPCContext &context);
	//virtual RPCResult CB_GetPlayerEquipData(LPCPEER pPeer, RPCContext &context , const UInt32 &ID);

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
	virtual RPCResult CB_SetRobot(LPCPEER pPeer, RPCContext &context, const Boolean &bRobot, const String &robotScript);
	virtual RPCResult CB_RobotMapAction(LPCPEER pPeer, RPCContext &context, const String &func, const RPCNetID &mapNetId, const RPCNetID &charNetId, const RPCNetID &otherNetId);
	virtual RPCResult CB_RobotBattleTurn(LPCPEER pPeer, RPCContext &context, const String &func, const RPCNetID &battleNetId, const RPCNetID &charNetId, const RPCNetID &batCharNetId, const RPCNetID &partnerNetId);
	virtual RPCResult CB_AssetProtectResult(LPCPEER pPeer, RPCContext &context, const UInt8 &resultMsg, const UInt32 &index, const UInt8 &type);	
	virtual RPCResult CB_SetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success);
	virtual RPCResult CB_AssetReleaseLockResult(LPCPEER pPeer, RPCContext &context, const UInt8 &resultMsg, const UInt32 &index, const UInt8 &type);	
	virtual RPCResult CB_ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &bSuccess);	
	virtual RPCResult CB_WarningMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const Int32 &value, const UInt8 &showIn);
	virtual RPCResult CB_DonateResult(LPCPEER pPeer, RPCContext &context, const UInt32 &donate, const UInt8 &returnFlag, const UInt32 &donateMoney);
	virtual RPCResult CB_SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context, const Boolean& hit);
	virtual RPCResult CB_GetPartnerCollectionList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &PartnerIDs);
	virtual RPCResult CB_UsePartnerCollectionBuff(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag);
	virtual RPCResult CB_AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &MOBid, const BYTE &returnFlag);
	virtual RPCResult CB_ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult CB_RequestTargetEquip(LPCPEER pPeer, RPCContext &context, NetGroup * items);
	//virtual RPCResult CB_GetPlayerEquip(LPCPEER pPeer, RPCContext &context, NetGroup * items,const UInt32 &char_id);

	virtual RPCResult CB_RelationSystem(LPCPEER pPeer, RPCContext &context, NetObject* pRelationSystem);
	virtual RPCResult CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult CB_GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
	virtual RPCResult CB_GetPlayerEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);



	virtual RPCResult CB_ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId);
	virtual RPCResult CB_ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos);
	virtual RPCResult CB_ForgiveLineup(LPCPEER pPeer, RPCContext &context, const int &iResault);
	virtual RPCResult CB_SetChangePos(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex);
	virtual RPCResult CB_SetTargetNameFormHawk(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id ,const String &char_name,const Boolean& isonline);
	virtual RPCResult CB_InitTargetHawkConcernState(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money, const Boolean &result);
	virtual RPCResult CB_JailDelivery(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_WantedBail(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_WantedTrace(LPCPEER pPeer, RPCContext &context, const String &name, const UInt32 &lineId, const UInt32 &mapId, const UInt32 &mapX, const UInt32 &mapY);
	virtual RPCResult CB_RequestWantedTrace(LPCPEER pPeer, RPCContext &context);
	
	virtual RPCResult CB_GetGenerationGroup(LPCPEER pPeer, RPCContext &context, NetGroup* pGntGrp);
	virtual RPCResult CB_RequestGenerationTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiRequstId);
	virtual RPCResult CB_RequestGenerationResult(LPCPEER pPeer, RPCContext &context, const int &iResultCode, const UInt32 &uiRelatedId) ;
	virtual RPCResult CB_DeleteGeneration(LPCPEER pPeer, RPCContext &context, const int &iResultCode, const UInt32 &uiRelatedId);
	virtual RPCResult CB_ChangeGenerationType(LPCPEER pPeer, RPCContext &context, const int &iType) ;
	virtual RPCResult CB_UpdateGenerationAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &uiCurAcc, const UInt32 &uiHisAcc);
	virtual RPCResult CB_OpenBusinessLog(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AwardMsg(LPCPEER pPeer, RPCContext &context,const String &msg);
	virtual RPCResult CB_SetMarriageNpcAction(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_uid, const UInt8 &atcion, const UInt8 &direction);
	virtual RPCResult CB_ShowMarriageEff(LPCPEER pPeer, RPCContext &context, const UInt8 &state, const UInt32 &husbandID, const UInt32 &wifeID);
	virtual RPCResult CB_BugAccShopItem(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &iIndex);
	virtual RPCResult CB_GenerationGainAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &iGainedAcc);

	virtual RPCResult CB_ResetChangeLine(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_LeaveTeamTemporarily(LPCPEER pPeer,RPCContext &context, const UInt32 &uRetCode, const UInt32 &uId);
	virtual RPCResult CB_GetPartyInfoWhenJumpMap(LPCPEER pPeer, RPCContext &context, NetGroup* pParty);
	virtual RPCResult CB_ReturnParty(LPCPEER pPeer, RPCContext &context, const UInt32 &uRetCode, NetObject* pChar);
	virtual RPCResult CB_RefreshPartyCharInfo(LPCPEER pPeer, RPCContext &context, NetObject* pPartyCharInfo);
	virtual RPCResult CB_BuyPointsShopItem(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &uiPoints);
	virtual RPCResult CB_GenerationGainPoints(LPCPEER pPeer, RPCContext &context, const UInt32 &iGainedPoints);
	virtual RPCResult CB_PetUseSkillMsg(LPCPEER pPeer, RPCContext &context, const String &Msg);
	virtual RPCResult CB_RestorePartyAsTemp(LPCPEER pPeer, RPCContext &context, NetGroup* pParty);

	virtual	RPCResult CB_SetChangeModelSkillGroup(LPCPEER pPeer, RPCContext &context, NetGroup* skillGroup);
	virtual RPCResult CB_ShowIncognitoBoxEff(LPCPEER pPeer, RPCContext &context, const UInt16 &posx, const UInt16 &posy, const UInt8 &effType);




	virtual VOID OnUpdateId(const UInt32 &acct_id, const UInt32 &char_id);
	virtual VOID OnUpdateLevelInfo(const UInt16 &level, const UInt32 &nextLvExp);
	virtual VOID OnUpdatePartyInfo(const Boolean &hasParty, const Boolean &isLeader, const RPCNetID &partyNetID);
	virtual VOID OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);
	virtual VOID OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
	virtual VOID OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor);
	virtual VOID OnUpdateHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP);
	virtual VOID OnUpdateDP(const UInt16 &DP, const UInt16 &mDP, const UInt16 &maxDP);
	virtual VOID OnUpdateSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP);
	virtual VOID OnUpdateWC(const UInt16 &WC, const UInt16 &maxWC, const UInt16 &WcExp, const UInt32 &nextWCExp);
	virtual VOID OnUpdateVP(const UInt16 &VP, const UInt16 &maxVP);
	virtual VOID OnUpdateExp(const UInt32 &exp);
	virtual VOID OnUpdateAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI);
	virtual VOID OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD);
	virtual VOID OnUpdateAttrPoint(const UInt16 &attrPoint);
	virtual VOID OnUpdateHonor(const UInt32 &honor);
	virtual void OnUpdateBattleInfo(const Boolean &EnablePK);
	virtual VOID OnUpdatePKInfo(const UInt16 &PK_Value);
	virtual VOID OnUpdatePartner(const UInt32 &partner, const UInt32 &partnerMob_id, const String &partnerName, const UInt16 &colorIndex);
	virtual void OnUpdatePet(const UInt32 &pet, const UInt32 &pet_id, const String &petName, const UInt16 &petColorIndex);
	virtual void OnUpdateBuff(const String &buff);
	virtual void OnUpdateBuffCache(const String &buffCache);
	virtual void OnUpdateYuanBao(const UInt32 &yuanBao, const UInt32 &yuanBaoTicket);
	virtual void OnUpdateMoney(const UInt32 &money);
	virtual void OnUpdateBankMoney(const UInt32 &bankMoney);
	virtual void OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor);
	virtual void OnUpdateOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor);
	virtual void OnUpdateOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor);
	virtual void OnUpdateSpeakCount(const UInt16 &globalSpeakCount);
	virtual void OnUpdateTitle(const String &title, const UInt16 &title_id);
	virtual void OnUpdateHawkInfo(const String &hawkName);
	virtual void OnUpdateGoodness(const Int32 &goodness);
	virtual void OnUpdateHawkerTime(const UInt32 &hawkerTime);
	virtual void OnUpdateSMoney(const UInt32 &SMoney);
	virtual void OnUpdateFollowerList(const UInt32Vector &followerList);
	virtual void OnUpdateAvailWareHouse(const UInt8 &availItemWareHouse, const UInt8 &availPartnerWareHouse, const UInt8 &availPetWareHouse);
	virtual void OnUpdateYuanbaoLock(const Boolean &yuanbaoLock);
	virtual void OnUpdateBankLock(const Boolean &bankLock);
	virtual void OnUpdateHistoryGenerationAcc(const UInt32 &iHisGenAcc);
	virtual void OnUpdateCurrentGenerationAcc(const UInt32 &uiCurGenAcc);
	virtual void OnUpdateMark(const UInt32 &mark);
	virtual void OnUpdateManor(const UInt32 &manor, const UInt32 &pettwo_uid, const UInt32 &pettwo_id, const String &pettwoName, const UInt16 &pettwoColorIndex);
	virtual void OnUpdateTP(const UInt16 &TP, const UInt32 &Model_mobid);
	CCharacter();
	virtual ~CCharacter();
	inline CCharacter* NewInstance();

	inline void SetCharacterView(CCharacterControl * pGivCharView){ pCharView = pGivCharView;}
	inline CCharacterControl * GetCharacterView(){ return pCharView;}
	inline UInt32 GetCurAcc() {return m_uiCurAcc;}
	void SetCurAcc(UInt32 uiCurAcc) { m_uiCurAcc = uiCurAcc;}
	inline UInt32 GetHisAcc() {return m_uiHisAcc;}
	void SetHisAcc(UInt32 uiHisAcc) { m_uiHisAcc = uiHisAcc;}

public:
	typedef StlVector<CGeneration*,ManagedAllocator<CGeneration*> > GenerationVector;
	 GenerationVector m_svtGeneration;
private:
	typedef Character_Proxy Parent;
	CCharacterControl * pCharView;
	UInt32 m_uiCurAcc;
	UInt32 m_uiHisAcc;

	
};


inline CCharacter* CCharacter::NewInstance()
{
	return CreateNetObject<CCharacter>();
}

#endif //_CHARACTER_H_
