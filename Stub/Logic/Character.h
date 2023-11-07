#pragma once
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "Proxy/Data/CharacterData.h"
#include "Stub/Logic/Character_Stub.h"
#include "Common/common_binary.h"
#include "Common/ChannelType.h"
#include "MapService/ViewManager.h"
#include "Stub/Logic/BatActor.h"

#define SPEAK_REST_TIME	2

struct ExtractedBuffData;
struct RpcCharInfo;
struct SkillData;

class CParty;
class CBattle;
class CItemBag;
class CScriptSystem;
class CPartner;
class CPartnerGroup;
class CPet;
class CPetGroup;
class CMapNPC;
class MapNPC_Stub;
class CSkill;
class CSkillGroup;
class CQuestSystem;
class CBuff;
class ViewManager;
class CTrading;
class CTitleData;
class CBatCharacter;
class CHawkGroup;
class CGuild;
class CGiftBox;
class CGuildMember;
class CGiftBox;
class CAntiGameAddiction;
class CMsgCharacterData;
class CQuestion;
class CGuild;
class CRelationSystem;
class CGenerationGroup;
class CItem;
class CMessageCharacter;
class CBatActor;

typedef StlMap<LPCPEER, SessionIDVector, std::less<LPCPEER>, 
		ManagedAllocator< std::pair<LPCPEER, SessionIDVector> > > ProxyListenersMap;

#define LOW_MARTIAL_MONEY		10000
#define LOW_MARTIAL_MONEY_TAX	2000
#define HIGH_MARTIAL_MONEY		500000
#define HIGH_MARTIAL_MONEY_TAX	100000
#define TIME_MARK_MASTER_STUDENT 101
#define FLAG_MASTER_GRATUATE_STUDENT_COUNT 100

class CCharacter : public Character_Stub,
					public Reuben::Simulation::IEventHandler<CCharacter>
{
public:

	//////////////////////////////////////
	//			RPC methods				//
	//////////////////////////////////////
	virtual RPCResult BuyPointsShopItem(LPCPEER pPeer, RPCContext &context, const int &iItemID, const UInt32 &nPointsShopID, const UInt32 &nPointsShopTypeID);
	virtual RPCResult MoveTo(LPCPEER pPeer, RPCContext &context, const UInt16 &posX, const UInt16 &posY, const UInt16 &hint1, const UInt16 &hint2);
	virtual RPCResult StopMove(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult RespondTest(LPCPEER pPeer, RPCContext &context, const UInt32 &minRespond, const UInt32 &maxRespond, const float &avgRespond, const UInt32 &respondCount);
	virtual RPCResult TestBattle(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CancelAuto(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult LevelUp(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult RaiseAttrib(LPCPEER pPeer, RPCContext &context, const UInt16 &sta, const UInt16 &spr, const UInt16 &str, const UInt16 &con, const UInt16 &agi);
	virtual RPCResult PKRequest(LPCPEER pPeer, RPCContext &context, const UInt32 &TargetID, const BYTE &Type);
	//virtual RPCResult SetEnablePK(LPCPEER pPeer, RPCContext &context, const Boolean &Value);
	virtual RPCResult LeaveWatching(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult MonitorBattle(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult JoinBattle(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult RequestJoinBattle(LPCPEER pPeer, RPCContext &context, const Boolean &Force);
	virtual RPCResult RejectHelper(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult SetFollowPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id);
	virtual RPCResult SetFollowPet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_id);
	virtual RPCResult ChangeMapLine(LPCPEER pPeer, RPCContext &context, const UInt8 &line_id);
	virtual RPCResult SetTarget(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult SearchParty(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult SearchChar(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult GetPartyInfo(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId);
	virtual RPCResult CreateParty(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult PartyJoinRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId);
	virtual RPCResult PartyJoinLevelControl( LPCPEER pPeer, RPCContext &context,const RPCNetID &targetId,const UInt16 &levelDown, const UInt16 &levelUp);
	virtual RPCResult PartyJoinReply(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const Boolean &agree, const Boolean &showMsg);
	virtual RPCResult PartyInviteRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId);
	virtual RPCResult PartyInviteReply(LPCPEER pPeer, RPCContext &context, const RPCNetID &partyId, const Boolean &agree, const Boolean &showMsg);
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
	virtual RPCResult AnswerAntiHack(LPCPEER pPeer, RPCContext &context,  const UInt8 &answer, const Boolean &is_right);
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
	virtual RPCResult UpdateYuanBao(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult LockAsset(LPCPEER pPeer, RPCContext &context, const UInt32 &bagIndex, const UInt8 &assetType);
	virtual RPCResult ShutNewGuide(LPCPEER pPeer, RPCContext &context, const UInt8 &flag);
	virtual RPCResult UnlockAsset(LPCPEER pPeer, RPCContext &context, const ByteArray &password, const UInt32 &bagIndex, const UInt8 &assetType);
	virtual RPCResult ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult SetAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &password);
	virtual RPCResult ChangeAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &oldPassword, const ByteArray &newPassword);	
	virtual RPCResult KickMember(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &reason);
	virtual RPCResult SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context,const Boolean& hit);
	virtual RPCResult SetAutoWalk(LPCPEER pPeer, RPCContext &context,const Boolean& bRobot);
	virtual RPCResult UsePartnerCollectionBuff(LPCPEER pPeer, RPCContext &context, const bool &bUse);
	virtual RPCResult AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id);
	virtual RPCResult ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &changeType, const UInt32 &num, const ByteArray &password);
	virtual RPCResult AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &pricePerUnit, const ByteArray &password);
	virtual RPCResult BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID, const UInt32 &YBT, const UInt32 &moneyPerUnit, const ByteArray &password);
	virtual RPCResult GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const ByteArray &password);

	virtual RPCResult AddStartHawkItem(LPCPEER pPeer, RPCContext &context, const HawkerItemInfo& itemInfo,const StartHawkAddType& addType);
	virtual RPCResult ForgiveLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId);
	virtual RPCResult ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId);
	virtual RPCResult ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos);
	virtual RPCResult SetChangePos(LPCPEER pPeer, RPCContext &context, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex);

	virtual RPCResult RequestTargetEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult GetPlayerEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);

	virtual RPCResult UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const String &targetName, const Int32 &money);
	virtual RPCResult JailDelivery(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult WantedBail(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult WantedTrace(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
	virtual RPCResult GetGenerationGroup(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult RequestGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId);
	virtual RPCResult RequestGenerationAns(LPCPEER pPeer, RPCContext &context, const BOOL &bConfirm,const UInt32 &uiRelatedId);
	virtual RPCResult DeleteGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiRelatedId);
	virtual RPCResult GetSavedExp(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult BuyAccShopItem(LPCPEER pPeer, RPCContext &context, const UInt32 &iIndex);
	virtual RPCResult RequestCrossMapJump(LPCPEER pPeer, RPCContext &context, const UInt32 &uSrcJmpSeq, const UInt32 &uDstJmpSeq);
	//virtual RPCResult CreateWeaponsPointData(LPCPEER pPeer, RPCContext &context,
	//										const UInt32 & char_id, const UInt32 & point,const String & nickName,
	//										const UInt32 & cclass,const UInt32 & faction,const UInt32 & level, 
	//										const time_t &weaponspoint_time);
	//RPCResult CCharacter::GetWeaponsPointData(LPCPEER pPeer, RPCContext &context,const UInt32 &char_id);
	virtual RPCResult LeaveTeamTemporarily(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult ReturnParty(LPCPEER pPeer, RPCContext &context);
	//////////////////////////////////////
	//		Character methods			//
	//////////////////////////////////////
	DEFINE_CLASS(Character_CLASSID);
	CCharacter();
	virtual ~CCharacter();
	INLINE static CCharacter* NewInstance();
	virtual UInt32 GetActorId()  { return GetChar_id(); }
	virtual String DebugInfo();
	VOID SetSessionId(const SessionID &sid);
	CONST SessionID& GetSessionId() { return m_sessionId; }
	VOID SetPeer(const SessionID& sid);
	LPCPEER GetPeer() { return m_pPeer; }
	VOID ClearPeer();
	RPCContext& GetContext() { return m_context; }
	// field getter methods for lua
	INLINE CItemBag* GetItems() { return m_pBag; }
	INLINE CSkillGroup* GetSkills() { return m_pSkillGroup; }
	INLINE CPartnerGroup* GetPartners() { return m_pPartnerGroup; }
	INLINE CPetGroup* GetPets() { return m_pPetGroup; }
	INLINE CQuestSystem* GetQuests() { return m_pQuestSys; }
	INLINE CScriptSystem* GetScript() { return m_pScriptSys; }
	INLINE CScriptSystem* GetScriptSystem() { return m_pScriptSys; }
	CPartner* GetCurPartner();
	CPet* GetCurPet();

	void CalcAttrib();							// calculate final attribute values
	void CalcNextLevel();						// calculate next level exp
	void CalcNextWcLevel();						// calculate next level exp
	void WCLevelUp();
	void ResetCharLevel(UInt16 lv);
	Boolean ReverseAttrib(UInt8 attrType, UInt16 point);
	void SetCharInfo(RpcCharInfo* pCharInfo, UInt32Vector &partnerIds);
	virtual void ChangeBaseSpeed(UInt16 speed);
	void SendClient(Boolean bDetail, Boolean bSendOther);
	void UpdateSpeed();
	void UpdateBuffBlockChannel();
	bool CheckProtectedPK();
	bool CheckBlockPK();
	bool CheckBlockItem(UInt8 itemType);
	bool CanCheat(UInt32 acct_id=0, PCSTRING cheatCode=NULL);
	void SetConcernMask(CMapActor *actor, RPCMask &mask);	// change concern mask of actor view
	CItemBag* GetBag() { return m_pBag; }
	void SetEnablePKEx(Boolean enable);
	void SetVisible(Boolean visible);
	void ShowRumor(PCSTRINGA msg, Boolean bTitle);
	void ShowRank(UInt8 rankType);
	void ShowGuildRank();
	void ChangeSTA(Int change);
	void ChangeSPR(Int change);
	void ChangeSTR(Int change);
	void ChangeCON(Int change);
	void ChangeAGI(Int change);
	void ChangeATK(Int change);
	void ChangeDEF(Int change);
	void ChangeHIT(Int change);
	void ChangeEVA(Int change);
	void ChangePOW(Int change);
	void ChangeSPD(Int change);
	void ChangeHP(Int change);
	void ChangeMHP(Int change);
	void ChangeMaxHP(Int change);
	void ChangeSP(Int change);
	void ChangeMSP(Int change);
	void ChangeMaxSP(Int change);
	void ChangeDP(Int change);
	void ChangeMDP(Int change);
	void ChangeMaxDP(Int change);
	void ChangeWC(Int change);
	void ChangeMaxWC(Int change);
	void ChangeWcExp(Int change);
	void ChangeVP(Int change);
	void ChangeMaxVP(Int change);
	void ChangeFDonate(Int change);
	void ChangeMDonate(Int change);
	void ChangeGDonate(Int change, UInt8 source = 0);
	UInt32 GetGDonate();
	UInt32 GetGTotalDonate();
	void ChangeSpeakCount(Int16 count);
	void ChangeLevel(Int16 change);
	void ChangeExp(Int32 change, Int32 exp_type = 0);
	void ChangeMoney(Int32 change, Int32 src_type = 0);
	void ChangeBankMoney(Int32 change, Int32 src_type = 0);
	void ChangeSMoney(Int32 change, Int32 src_type = 0);
	void ChangeHonor(Int32 change);
	void ChangeGoodness(Int32 change, Int32 exp_type = 0);
	void ChangePvPScoreSingle(Int32 change);
	void SetFull(bool HP=true, bool SP=true, bool DP=true, bool VP=true);
	bool IsHawking();
	void SetFaction(UInt8 faction);
	void StopScript();
	void ClearTrading();
	void AddAvailBackPack(UInt8 numOfAvail);
	void AddAvailItemWareHouse(UInt8 numOfAvail);
	void AddAvailPartnerWareHouse(UInt8 numOfAvail);
	void AddAvailPetWareHouse(UInt8 numOfAvail);
	RPCResult ChangeYuanBao(RPCContext &context);
	Boolean IsRobot() { return m_bRobot; }
	void SetRobot(Boolean bRobot, UInt32 interval);
	void RobotAction();
	RPCResult WaitAvailable(RPCContext &context);
	Boolean IsEnterDone() { return m_bEnterDone; }

	//////////////////////////////////////
	//		Data Exchange methods		//
	//////////////////////////////////////
	void SetData(CCharacterData *pCharData);	// set character by CCharacterData
	void SetData(CMessageCharacter * pMsgchar);
	void WriteData(CCharacterData* pCharData);	// write modified character to CCharacterData
	Boolean NeedUpdate();						// need update to database?
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event
	void RaiseAllUpdate();						// raise all update event of character belonging
	void CancelAllUpdate();						// cancel all update event of character belonging
	void GetMsgCharData(CMsgCharacterData* pData);	// get message character data
	RPCResult UpdateToObject(RPCContext &context);	// update to object service
	RPCResult UpdateAllObjects(RPCContext &context);	// update all belong objects to object service
 
	//////////////////////////////////////
	//		Event & State methods		//
	//////////////////////////////////////
	virtual void HandleEvent(CONST Event &event);
	virtual Boolean ChangeState(CONST StateID state);		// try to change state
	virtual Boolean IsState(CONST StateID state);			// is that state now?
	virtual Boolean TryState(CONST StateID state);			// can change state?
	VOID SetLogout(CONST Boolean bLogout) { m_sessLogout = bLogout; }
	Boolean IsLogout() { return (m_sessLogout); }			// should logout?
	void ScheduleLogout(UInt32 logoutTime);					// schedule to logout character
	void CancelLogout();									// cancel schedule logout character
	virtual void OnEnterMap(CMap* pmap);					// enter map event
	virtual void OnLeaveMap();								// leave map event

	//////////////////////////////////////
	//			Map methods				//
	//////////////////////////////////////
	Boolean PreCharacterEnter(CMap *pmap, const EnterInfo &info, 
		const UInt32Vector &memberIdList, Boolean bLocal,
		UInt16 ui16LineupId = 13,int iKeyPos=0,CParty* pTempParty = NULL);
	Boolean PostCharacterEnter(CMap *pmap, const EnterInfo &info, 
							const UInt32Vector &memberIdList, Boolean bLocal);
	RPCResult DoCharacterLeave(RPCContext &context);
	Boolean IsInMap();
	Boolean CanJumpMap();
	void JumpMap(UInt16 map_id, UInt16 x, UInt16 y, UInt8 line_id=0, UInt32 ownerId=0,
				UInt16 instId=0,Boolean bCheckLua = BOOLEAN_TRUE);
	BOOL CheckNeedJumpReborn();
	VOID JumpReborn(UInt16 map_id=0, UInt16 x=0, UInt16 y=0);
	void EnterInstance(PCSTRINGA script, UInt16 instId, UInt32 ownerId, UInt16 map_id, 
						UInt16 x, UInt16 y);
	void EnterEmptyInstance(PCSTRINGA script, UInt16 instId, UInt32 ownerId, 
							   UInt16 map_id, UInt16 x, UInt16 y);
	void LeaveInstance(UInt16 instId, UInt32 ownerId, UInt16 map_id, UInt16 x, UInt16 y, 
						UInt8 line_id);
	void ResetInstance(UInt16 instId, UInt32 ownerId);
	void CloseInstance(UInt16 instId, UInt32 ownerId);
	void OnEnterInst(UInt16 instId, UInt32 ownerId);
	void OnLeaveInst(UInt16 instId, UInt32 ownerId);
	VOID SetExclusiveNpc(CMapNPC* pNpc);
	void OpenInterface(String menu, Int32 param1);
	void OpenGiftBox();
	void OpenWareHouse();
	RPCResult OpenGiftBox(RPCContext &context);
	void OpenHawkerOwnerMenu();
	RPCResult GetHawkHistoryGroup(RPCContext &context);
	RPCResult OpenWareHouse(RPCContext &context);
	void OpenSellYuanBao();

	//////////////////////////////////////
	//			Battle methods			//
	//////////////////////////////////////
	void BattleHelp(Boolean bNeed);							// need help in battle
	//VOID MonitorBattle(CBattle * pBattle, SessionID targetSid);
	StateID GetBattleStateID(VOID);
	StateID GetMapStateID(VOID);
	VOID ClearAllHelpers();
	VOID LeaveHelpersList();
	void CheckBattleState();
	RPCResult DoReadyBattle(RPCContext &context);
	RPCResult DoEncounterBattle(RPCContext &context);
	//RPCResult DoEncounterPVPBattle(RPCContext &context);
	//RPCResult DoJoinBattle(RPCContext &context);
	inline void SetEncounterBattle(bool value){m_EncounterBattle = value;}
	INLINE void SetStepMod(int stepMod){ m_stepMod = stepMod; }
	// add & remove quest encounter
	void AddEncounter(UInt8 index, UInt16 quest_id, UInt32 npc_id, UInt16 map_id, 
					UInt16 maxStep, UInt16 curEnc, UInt16 maxEnc);

	void RemoveEncounterByNpcID(UInt16 quest_id, UInt32 uiNpcID);

	void RemoveEncounter(UInt16 quest_id);
	Boolean IsInBattle();
	void UpdateWantedReward(UInt32 targetId, Int32 money);
	void RequestJailDeliveryId();
	void RequestBailId();
	void SetPK_Value(const UInt16 &PK_Value);

	Boolean	GetHitBoss(){return m_hitBoss;}
	//////////////////////////////////////
	//			Party methods			//
	//////////////////////////////////////
	bool IsLeader();
	bool HasParty() { return m_pParty != NULL; }
	bool HasTempParty() { return m_pTemporaryParty != NULL;}
	CParty* CreateParty(Boolean bForce = BOOLEAN_FALSE);
	CParty* GetParty() { return m_pParty; }
	CParty* GetTempParty() {return m_pTemporaryParty;}
	VOID SetTempParty(CParty* pParty = NULL);
	VOID SetParty(CParty* pParty, Boolean isLeader=BOOLEAN_TRUE);	// set party
	Boolean HasRequested(CCharacter *target);		// has requested before?
	Boolean HasInvited(CCharacter *target);			// has invited before?
	Size GetRequestCount();							// get join request count
	Size GetInviteCount();							// get invite count
	VOID AddRequest(CCharacter *target);			// add join request
	VOID AddInvite(CCharacter *target);				// add invite target
	VOID CancelAllRequests();								// cancel all join request and invitation
	VOID CancelRequest(CCharacter *target, Boolean showMsg = true);			// cancel request from target character
	VOID CancelInvite(CCharacter *target, Boolean showMsg = true);			// cancel invite from target character
	void OpenSearchPartyPanel();
	void OpenWeaponsRankPanel();
	CItemBag * GetPlayerEquipData(UInt32 ID);
	void HandleGuideEvent(UInt16 ID);

	//////////////////////////////////////
	//			Skill methods			//
	//////////////////////////////////////
	UInt16 GetSkillLv(UInt16 ID);
	void SetSkillCoolDown(UInt16 ID, DWORD countDown);
	bool HasSkill(UInt16 SkillId);
	CSkill* GetSkill(UInt16 SkillId);
	VOID AddSkill(UInt16 skillId);
	VOID ChangeSkillLv(UInt16 skillId, Int level);
	VOID RemoveSkill(UInt16 skillId);
	void SetUseSkillReturn(BYTE flag);
	RPCResult DoLearnSkill(RPCContext &context);
	RPCResult DoRemoveSkill(RPCContext &context);
	bool IsSkillValidTarget(CCharacter * pTarget, UInt16 targetType);
	VOID AddSkillTemp(UInt16 skillId);// not save to the DB
	VOID RemoveSkillTemp(UInt16 skillId);// not save to the DB
	UInt16 GetSkillCount(UInt16 type,UInt16 level);

	//////////////////////////////////////
	//			Buff methods			//
	//////////////////////////////////////
	void SetBuffGroup(NetGroup * BGrp);
	bool AddBuff(UInt16 Buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro, bool calc = true);
	void RemoveBuff(UInt16 Buff_id, bool calc = true);
	void RemoveBuffSchool(UInt16 school_id);
	bool HasBuff(UInt16 Buff_id);
	bool IsBuffExist(UInt16 Buff_id) { return HasBuff(Buff_id); }
	ExtractedBuffData * FindBuff(UInt16 Buff_id);
	void ExtractBuffString();
	void ConstructBuffString();
	void ExtractBuffCacheString();
	void ConstructBuffCacheString();
	void CheckBuffUpdate();
	bool AddEquipmentBuff(UInt8 index, UInt16 Buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro);
	bool HasEquipment(UInt8 index);
	void RemoveDupItemBuff();
	void InitialRepeatBuff(ExtractedBuffData &dat, const BuffData *buffdat);


	//////////////////////////////////////
	//			VP methods			//
	//////////////////////////////////////
	void CalMaxVP();

	//////////////////////////////////////
	//		Trading methods				//
	//////////////////////////////////////
	void SetTradingTarget(RPCNetID pCharNetID) { m_pTradingTargetNetID = pCharNetID; }
	void ResetTradingTarget() { m_pTradingTargetNetID = RPCNetID(); }
	CTrading* GetTradingObject(){ return m_pTrading; }
	void SetTradingObject(CTrading* tradingObject){ m_pTrading = tradingObject;}
	bool IsTrading() { return (m_pTrading != NULL || m_pTradingTargetNetID != RPCNetID() );}
	void ClearReqTrading();
	RPCNetID GetTradingTarget(){ return m_pTradingTargetNetID;}
	void StopReqTradingTimeOutEvent() { CancelUniqEvent(EVT_TRADING_REQ_TIME);}		

	////////////////////////////////////
	//		Hawker methods			 ///
	////////////////////////////////////
	void AddHistory(String itemName, UInt8 itemType, UInt8 stack, UInt32 totalPrice, UInt32 tax);
	RPCResult AddHistory(RPCContext &context); 
	void SendAllHistoryToClient();
	RPCResult DrawTargetUidFromHawk(RPCContext &context); 
	
	/////
	void SkillFail(PCSTRINGA Text);
	void UseSkillMsg(PCSTRINGA Text);
	void ItemFail(PCSTRINGA Text);
	void UseItemMsg(PCSTRINGA Text);
	//SkillData * GetSkillCsvData(UInt16 skill_id);
	
	//title method
	void SendTitle();
	Boolean HasTitle(UInt16 title_id);
	INLINE const String& GetTitle() { return Parent::GetTitle(); }
	const String& GetTitle(UInt16 title_id);
	void AddTitle(UInt16 title_id);
	void AddTitle(UInt16 title_id, UInt32 title_uid, UInt32 expireData, const String &name);
	void RemoveTitle(UInt16 title_id);
	RPCResult AddTitle(RPCContext &context);
	RPCResult AddTitleMaster(RPCContext &context);
	RPCResult RemoveTitle(RPCContext &context);
	RPCResult DonateMoney(RPCContext &context);
	void CalTitleExpire();
	void CheckTitle();
	void SetTitle(const String &title, const UInt16 &title_id,bool bRemove = false);
	void SetTitle(const String &title);

	////////////////////////////////////////
	//         Anti-Hack Methods          // 
	////////////////////////////////////////

	UInt8 m_WrongAns;
	UInt32 m_curHackAns;
	UInt32 m_noHackPopUpTime;
	Boolean m_expReward;
	Boolean m_hpReward;	
	Boolean m_bForcePopUp;	

	inline UInt8 GetHackWrongAns() { return m_WrongAns; }
	inline void ForceAntiHackPopUp(){ m_bForcePopUp = BOOLEAN_TRUE; }	
	void AntiHack();
	RPCResult DoAntiHack(RPCContext& context);
	void ForceLogout(PCSTRING pForceMsg);
	inline UInt32 GetNoAntiHackTime() { return m_noHackPopUpTime; } 
	
	///////////////////////////////////////
	//         Guilds methods	         //
	///////////////////////////////////////
	void OpenGuildApplyMenu();
	bool CreateGuild(PCSTRINGA nameA);
	void OpenCreateGuildMenu();
	inline bool HasGuild(){return m_ownGuild != NULL || m_ownGuildMember != NULL || m_creatingGuild;};
	void RemoveGuildTitle();
	void RemoveGuildApply();
	UInt32 GetGuildUid();	
	UInt32 GetOppGuildUid();	
	inline CGuild * GetGuild(){return m_ownGuild;};
	UInt8 GetGuildPos();
	UInt32 GetJoinGuildPeriod();
	bool IsResting();
	bool IsGuildWarWin();
	void ChangeGuildWarTitle();
	bool InGuildWar();
	bool InFreePKMap();
	RPCResult CreateGuild(RPCContext &context);	// update to object service
	UInt32 GetGuildManorScale();	
	Boolean Sit();
	Boolean IsSit();
	void ShowGuildMsg(PCSTRINGA msg, UInt32 guildUID);
	void ShowAwardMsg(PCSTRINGA msg);

	Boolean m_bManorExpel;	

	////////////////////////////////////////
	//         Anti-Addict                //
	///////////////////////////////////////
	CAntiGameAddiction* m_pAntiGameAddict;
	void CreateAntiAddictionObject();
	void CalcAddiction(const AntiAddictInfo &addictInfo);

	void RaiseGameHealthChg(UInt32 evtType, UInt32 EvtTime);
	void RaiseGameHealthWarn(UInt32 evtType, UInt32 EvtTime);
	void RaiseInterestReduction(UInt32 evtType, UInt32 EvtTime);
	void RaiseOffTimeClear(UInt32 EvtTime);
	void RaiseGeneralGameHealthEvt(UInt32 evtType, UInt32 EvtTime);
	void CancelAllGameHealthEvt();	

	///////////////////////////////////////
	//        NPC Bank Service           //
	///////////////////////////////////////
	enum BankEnum
	{
		MAX_DEPOSIT = 10000000,
	};
	Boolean ChkDeptValid(UInt32 money);
	Boolean ChkWithDrValid(UInt32 money);

	///////////////////////////////////////
	//         Quest methods             //
	///////////////////////////////////////
	Boolean HasFollower(UInt32 mob_id);
	UInt32 GetFollowerId(UInt16 i);
	UInt16 GetFollowerCount(UInt32 mob_id);
	UInt16 GetFollowerTotal();
	void AddMobFollower(UInt32 mob_id);
	void RemoveMobFollower(UInt32 mob_id);
	void ClearMobFollower();


	///////////////////////////////////
	//            Lock Asset         //
	///////////////////////////////////
	Boolean ChangeYuanBaoLockStatus(RPCContext &context, const Boolean &lock, UInt8 &msg);
    Boolean ChangeBankMoneyStatus(const Boolean &lock, UInt8 &msg);
	void AssignAssetPassword(const ByteArray &password, const Boolean &bChangePassword);
	RPCResult AssignAssetPassword(RPCContext &context);

	Boolean m_bAssetPasswordVerified;
	/////////////////////////
	void SetInitAutoCount(UInt16 autoCount){m_initAutoCount = autoCount;};
	UInt16 GetInitAutoCount(){return m_initAutoCount;};
	void SetAntiHackEnable(Boolean enable){m_bAntiHackEnable = enable;};
	//Boolean GetAntiHackEnable(){return m_bAntiHackEnable;};
	Boolean GetAntiHackEnable(){return false;};	//2012.1.7 Modified by Jay, Disable this Func

	///////////////////////////////////////
	//    Partner Collection  Methods    //
	///////////////////////////////////////
	void CreatePartnerCollection(UInt32 collection_id, UInt32 char_id);
	RPCResult CreatePartnerCollection(RPCContext &context);
	void DeletePartnerCollection(UInt32 char_id);
	void GetPartnerCollection(UInt32 char_id);
	UInt16 GetPartnerCollectionBuff(UInt8 race, AttriStruct2 &attri);
	void ActivatePartnerCollectionBuff(bool bActivate);
	void ResetPartnerCollectionBuff();
	void SendPartnerCollection();
	UInt16 GetPartnerCollectionCount(UInt8 race);
	Boolean AddPartnerCollection(UInt32 mob_id);

	
	//////////////////////////////////////
	//		Relation					//
	//////////////////////////////////////
	CRelationSystem* GetRelationSystem(){return m_pRelationSystem;};

	void SetMarriageState(UInt8 state);
	void ShowMarriageEffect(UInt8 state);
	RPCResult AddTitleHusband(RPCContext &context);
	RPCResult AddTitleWife(RPCContext &context);
	CMapNPC* GetCharNpc(){return m_marriageCharNpc;}
	void SetCharNpc(CMapNPC* pNpc){m_marriageCharNpc = pNpc;};
	void SetCharNpcState(UInt8 direction,UInt8 atcion);
	Boolean IsExcuteMarriage(){return m_marriageCharNpc != NULL;};
	//////////////////////////////////////
	//		Client Model				//
	//////////////////////////////////////
	void ChangeToMob(UInt32 mob_id);
	void ChangeModel(UInt32 mob_id);
	void ChangeModelSkill(UInt32 mob_id);
	CBatActor::SkillMap GetSkillMapById(UInt32 mob_id);
	void SetPvPBattleScriptID(UInt32 pvpbattleID){m_pvpBattleScriptID = pvpbattleID;};
	UInt32 GetPvPBattleScriptID(){return m_pvpBattleScriptID;};
	UInt32 m_pvpBattleScriptID;
private:
	
	CMapNPC* m_marriageCharNpc;
	CharacterPtrList m_charViewLst;
public:

	void SetMoney(const UInt32 &money);

	//////////////////////////////////////
	//		Skill						//
	//////////////////////////////////////
	void AddChildSkill(NetGroup * grp);

	//////////////////////////////////////////////////////////////////////////
	//lineup info
	//////////////////////////////////////////////////////////////////////////
	BOOL HasLearedLineup(UInt16 uiLineupId);

	//////////////////////////////////////////////////////////////////////////
	//Wanted
	//////////////////////////////////////////////////////////////////////////
	void RequestTraceId();
	
	//////////////////////////////////////////////////////////////////////////
	//Generation
	//////////////////////////////////////////////////////////////////////////
	void DeleteGenerationLocal(UInt32 uiRelatedId);
	void CreateGenerationLocal(UInt32 uiRelatedId);
	void CalcGenerationExpCont(UInt32 uiNextLvExp);
	void CalcGenerationAccCont();
	void ChangeGenerationType();
	void RecvGenerationAcc(UInt32 uiRelatedId,UInt32 uiRecvAcc);
	void GenerationOnline(UInt32 uiRelatedId);
	void GenerationOffline(UInt32 uiRelatedId);
	void InitGenerationInfo(NetGroup* pOnlineChars);

	//////////////////////////////////////
	//		Business Shop				//
	//////////////////////////////////////
	void OpenBusinessShop(UInt16 shopId);
	void ChangeBusinessProfit(Int32 profit);
	void OpenBusinessLog();
	void ApplyTrader();

	//////////////////////////////////////////////////////////////////////////
	//RumorLink 
	//////////////////////////////////////////////////////////////////////////
	PCSTRING GetCharNameLink();
	PCSTRING GetCharPartnerNameLink(UInt32 partnerID,UInt16 partnerIndex = -1);
	PCSTRING GetCharPetNameLink(UInt32 petID,UInt16 petIndex = -1);
	PCSTRING GetCharItemNameLink(UInt32 itemID,UInt8 itemIndex = -1);
	PCSTRING GetCharItemNameLink(CItem* item);

	////////////////////////////
	//MessageChannle          //
	////////////////////////////
	void JoinInstanceChannle(UInt32 InstanceID);
	////////////////////////////
	//UseTwoPet		          //
	////////////////////////////
	void SetCurTwoPet(CPet* pPet);
	//////////////////////////////////////
	//		Structure & Fields			//
	//////////////////////////////////////
	struct HelperData
	{
		CCharacter * pHelperChar;
		UInt32	rejectTime;
		HelperData()
		{
			pHelperChar = NULL;
			rejectTime = 0;
		}
	};
	typedef StlMap<UInt32, HelperData, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, HelperData> > > HelperList;

	struct TitleInfo
	{
		//key
		UInt32 title_uid;
		//info
		UInt16 title_id;
		UInt8 status;
		//expireDate
		UInt32 expireDate;

		String title_name;
		TitleInfo()
		{
			title_uid = 0;
			title_id = 0;
			expireDate = 0;
			title_name.Empty();
			status = TITLE_STATE_ADD;
		}
		~TitleInfo()
		{
			title_name.Empty();
		}
		bool IsExist() { return (status != TITLE_STATE_REMOVE); }
	};
	typedef StlMap<UInt16, TitleInfo*, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, TitleInfo*> > > TitleMap;

	struct HawkerHistoryInfo 
	{
		String itemName;
		HawkerHistory data;

		HawkerHistoryInfo()
		{
			itemName.Empty();
		}

		~HawkerHistoryInfo()
		{
			itemName.Empty();
		}
	};

	typedef StlMultiMap<UInt32, HawkerHistoryInfo, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, HawkerHistoryInfo> > > HawkerHistoryInfoMap;

	struct EncounterInfo
	{
		UInt8  index;			// target index
		UInt16 moved;			// moved step
		UInt16 maxStep;			// max step to encounter
		UInt16 curEnc;			// current encountered number
		UInt16 maxEnc;			// max encountered number
		UInt16 map_id;			// npc id to encounter
		UInt32 npc_id;			// npc id to encounter
		EncounterInfo() : moved(0), maxStep(0), curEnc(0), maxEnc(0), map_id(0), npc_id(0)
		{}
		EncounterInfo(UInt8 index, UInt32 npc_id, UInt16 map_id, UInt16 maxStep, 
					UInt16 curEnc, UInt16 maxEnc)
			:	index(index), moved(0), maxStep(maxStep), curEnc(curEnc), 
				maxEnc(maxEnc), npc_id(npc_id), map_id(map_id)
		{}
	};
	typedef StlMultiMap<UInt16, EncounterInfo, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, EncounterInfo> > > EncounterMap;

	struct partnerCollectionData
	{
		//key
		UInt32 collection_uid;
		//info
		UInt32 collection_id;
		UInt32 char_id;

		partnerCollectionData()
			: collection_uid(0)
			, collection_id(0)
			, char_id(0)
		{
		}

		partnerCollectionData(UInt32 Collection_uid, UInt32 Collection_id , UInt32 Char_id)
			: collection_uid(Collection_uid)
			, collection_id(Collection_id)
			, char_id(Char_id)
		{
		}
	};
	typedef StlMap<UInt32, partnerCollectionData*, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, partnerCollectionData*> > > PartnerCollectionMap;


	enum EventEnum
	{
		EVT_UPDATE_OBJECT				= MAKE_EVENTID(Character_CLASSID,  1),
		EVT_SESSION_LOGOUT				= MAKE_EVENTID(Character_CLASSID,  2),
		EVT_USE_SKILL					= MAKE_EVENTID(Character_CLASSID,  3),
		EVT_UPDATE_BUFF					= MAKE_EVENTID(Character_CLASSID,  4),
		EVT_BATTLE_LOCKED				= MAKE_EVENTID(Character_CLASSID,  5),
		EVT_TRADING_REQ_TIME			= MAKE_EVENTID(Character_CLASSID,  6),
		EVT_TITLE_EXPIRE				= MAKE_EVENTID(Character_CLASSID,  7),
		EVT_VP_UPDATE					= MAKE_EVENTID(Character_CLASSID,  8),
		EVT_CHECK_JUMP					= MAKE_EVENTID(Character_CLASSID,  9),
		EVT_GHEALTH_FST_WARN            = MAKE_EVENTID(Character_CLASSID,  10),
		EVT_GHEALTH_SND_WARN            = MAKE_EVENTID(Character_CLASSID,  11),
		EVT_GHEALTH_THIRD_WARN          = MAKE_EVENTID(Character_CLASSID,  12),
		EVT_GHEALTH_FOURTH_WARN         = MAKE_EVENTID(Character_CLASSID,  13),
		EVT_GHEALTH_CHG_GDAVG			= MAKE_EVENTID(Character_CLASSID,  14),
		EVT_GHEALTH_CHG_AVGBAD			= MAKE_EVENTID(Character_CLASSID,  15),
		EVT_GHEALTH_INTEREST_HALF       = MAKE_EVENTID(Character_CLASSID,  16),
		EVT_GHEALTH_INTEREST_ZERO       = MAKE_EVENTID(Character_CLASSID,  17),
		EVT_GHEALTH_OFFTIME_CLEAR       = MAKE_EVENTID(Character_CLASSID,  18), 
		EVT_MSGDATA_UPDATE				= MAKE_EVENTID(Character_CLASSID,  19),
		EVT_UPDATE_YUANBAO_TIMEOUT      = MAKE_EVENTID(Character_CLASSID,  20), 
		EVT_CLEAR_WAREHOUSE				= MAKE_EVENTID(Character_CLASSID,  21),
		EVT_ROBOT_ACTION				= MAKE_EVENTID(Character_CLASSID,  22),
		EVT_UPDATE_TITLE				= MAKE_EVENTID(Character_CLASSID,  23),
		EVT_GHEALTH_CLEAR_ONLINETIME    = MAKE_EVENTID(Character_CLASSID,  24), 	
	};

	enum MarriageEnum
	{
		MARRIAGE_INIT = 0,
		MARRIAGE_HEAVEN_EARTH = 1,
		MARRIAGE_FACETOFACE_STAND = 2,
		MARRIAGE_FACETOFACE_BOW= 3,
		MARRIAGE_RESET = 4,
		MARRIAGE_MASTER = 5,
	};

	enum MarriageEffectEnum
	{
		MARRIAGE_EFFECT_1 = 0,
		MARRIAGE_EFFECT_2 = 1,
		MARRIAGE_EFFECT_3 = 2,
		MARRIAGE_EFFECT_4= 3,
	};

	class CNoBattleState;
	class CBattleState;
	class CMonitorState;
	class CTalkState;
	class CHawkState;

	typedef StlMap<UInt16, CSkill*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, CSkill*> > > SkillMap;

	RPCNetID		m_infoNetId;		// CharacterInfo net ID
	//CBattle*		m_pBattle;
	RPCNetID		m_BattleNetID;
	CItemBag*		m_pBag;
	CPartnerGroup*	m_pPartnerGroup;
	UInt16			m_movedStep;		// moved step in battle zone
	UInt16			m_stepMod;			// step modifier for battle encounter happen
	EncounterMap	m_encounterMap;		// encounter info map by quest id
	Boolean			m_sessLogout;		// session logout after client disconnect
	Reuben::Simulation::HandleID	m_logoutHandle;		// logout event handle
	UInt16			m_autoCount;		// battle auto turn count
	UInt8			m_cmd;				// battle command
	UInt32			m_sub_type;			// battle sub type
	UInt32			m_additional;		// battle additional
	UInt32			m_cmdTarget;		// saved target id
	UInt8			m_cmdTargetType;	// BatActor target type
	UInt8			m_battleTeam;
	HelperList		m_HelperList;

	UInt32			m_minRespond;		// minimum respond time
	UInt32			m_maxRespond;		// maximum respond time
	float			m_avgRespond;		// average respond time
	UInt32			m_respondCount;		// respond count
	Boolean			m_hitBoss;			//autoWalk hit Boss

	static RPCMask	m_initMask;		// initial concern mask
	static RPCMask	m_npcMask;		// map npc concern mask
	static RPCMask	m_charMask;		// map character concern mask
	static RPCMask	m_memberMask;	// party member concern mask
	static RPCMask	m_selfMask;		// self concern mask
	static RPCMask	m_simpleMask;	// simple concern mask for quick preview

	CSkillGroup*	m_pSkillGroup;

	SkillMap		m_skillMap;
	BuffMap			m_BuffMap;
	BuffMap			m_BuffCacheMap;

	CGenerationGroup* m_pGeneration;

	UInt32			m_MatchCharID;
	BYTE			m_ReturnFlag;
	UInt16			m_tempSkillShopID;

	CQuestSystem*		m_pQuestSys;				// quest system
	ViewManager*		m_viewManager;				// view manager
	CTrading*			m_pTrading;
	CBatCharacter*		m_pBatChar;
	CHawkGroup*			m_pHawkGroup;
	CGiftBox*			m_pGiftBox;
	CQuestion*			m_pQuestion;

	String	m_Msg;

	UInt16	m_TempSkillID;
	UInt32	m_TempSkillTargetID;
	BYTE	m_TempSkillTargetType;

	//channel
	BOOL	m_Listen;
	UInt32	m_PreSpeakTime;

	//guild 
	BOOL	m_OpeningGuild;
	CGuild*	m_ownGuild;
	CGuildMember*	m_ownGuildMember;
	UInt32	m_battleRestTime;
	BOOL	m_creatingGuild;

	inline void ClearReturnType() { m_ReturnFlag = 0; }
	inline BYTE GetReturnType() { return m_ReturnFlag; }

	bool m_EncounterBattle;
	bool m_speakRights[MAX_CHANNELTYPE];

	bool m_bIsUpdatedYuanBao;

	CPetGroup * m_pPetGroup;
	TitleMap m_titleDataLst;

	// Battle API
	virtual UInt32 CheckBattle();
	virtual void EncounterBattle(UInt32 targetNpc);
	HawkerHistoryInfoMap m_hawkerHistoryMap;

	VOID SetStartBattle(CONST Boolean bStart) { m_bStartBattle = bStart; }
	CONST Boolean IsStartBattle(VOID) CONST { return (m_bStartBattle); }
	Boolean CanStartBattle(UInt32 targetId, RPCNetID targetNetId, UInt8 battleType);

	//wareHouse
	bool m_bCacheWareHouse;

	PartnerCollectionMap	m_PartnerCollectionLst;
	bool					m_bUsePartnerCollectionBuff;
	CPet*					m_pUseTwoPet;
	CSkillGroup*			m_changeMobSkillLst;
protected:

	State*	m_battleState;	// [no battle, battle, need help, monitor]
	CParty*	m_pParty;
	CParty* m_pTemporaryParty;
	CScriptSystem* m_pScriptSys;

	// Data variables

private:
	void PrePartySend();
	void UpdateTitle();

	typedef Character_Stub Parent;
	typedef StlVector<CCharacter*, ManagedAllocator<CCharacter*> > TargetList;
	typedef StlVector<CParty*, ManagedAllocator<CParty*> > PartyList;

	TargetList		m_requestList;
	TargetList		m_inviteList;
	LPCPEER			m_pPeer;
	RPCContext		m_context;
	SessionID		m_sessionId;
	RPCNetID		m_pTradingTargetNetID;
	UInt32			m_iLastJumpLineTime;
	Boolean			m_bStartBattle;
	Boolean			m_bRobot;					// is running robot script?
	UInt32			m_robotInterval;			// robot action interval
	UInt16			m_initAutoCount;
	Boolean			m_bAntiHackEnable;
	BOOL			m_bGenerationInit;
	Boolean			m_bEnterDone;				// to indicate character enter is finished

	CRelationSystem* m_pRelationSystem;        //relationship operation.master,sibling,husband and wife.
};

class CCharacter::CNoBattleState : public State
{
public:
	DEFINE_STATE(ST_NOBATTLE)
	CNoBattleState(CCharacter *pchar) : m_pchar(pchar) {}
	CCharacter *m_pchar;

protected:
	virtual VOID Enter(VOID);
};

class CCharacter::CBattleState : public State
{
public:
	DEFINE_STATE(ST_BATTLE)
	CBattleState(CCharacter *pchar) : m_pchar(pchar) {}
	CCharacter *m_pchar;

protected:
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST;
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
};

class CCharacter::CMonitorState : public State
{
public:
	DEFINE_STATE(ST_MONITOR)
	CMonitorState(CCharacter *pchar) : m_pchar(pchar) {}
	CCharacter *m_pchar;

protected:
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST;
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
};

class CCharacter::CTalkState : public State
{
public:
	DEFINE_STATE(ST_TALK)
	CTalkState(CCharacter *pchar) : m_pchar(pchar) {}
	CCharacter *m_pchar;

protected:
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST;
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
};

class CCharacter::CHawkState : public State
{
public:
	DEFINE_STATE(ST_HAWK)

};

INLINE CCharacter* CCharacter::NewInstance()
{
	return CreateNetObject<CCharacter>();
}

INLINE void CCharacter::RaiseUpdate()
{
	RaiseUniqEventID(EVT_UPDATE_OBJECT);
}

INLINE void CCharacter::CancelUpdate()
{
	CancelUniqEvent(EVT_UPDATE_OBJECT);
}

INLINE Boolean CCharacter::IsInMap()
{
	return (m_battleState->GetStateID() == ST_NOBATTLE && 
		m_mapState->GetStateID() == ST_ENTER_MAP &&
		!m_bStartBattle);
}

INLINE Boolean CCharacter::IsInBattle()
{
	return (m_bStartBattle || m_battleState->GetStateID() != ST_NOBATTLE);
}

#endif //_CHARACTER_H_
