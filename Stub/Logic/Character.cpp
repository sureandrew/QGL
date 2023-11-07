//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/ChannelType.h"
#include "Common/SettingTable.h"
#include "Common/DataCode.h"
#include "Common/StateCode.h"
#include "Common/common_binary.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Stub/Logic/Character.h"
//-- Library
#include "Proxy/Service/ObjectService.h"
#include "Proxy/Service/LogService.h"
#include "Proxy/Service/MessageService_proxy.h"
#include "Proxy/Service/WorldService_proxy.h"
#include "Proxy/Data/SkillData.h"
#include "Proxy/Data/ItemData.h"
#include "Proxy/Data/BuffData.h"
#include "Proxy/Data/MsgCharacterData.h"
#include "Proxy/Data/TitleData.h"
#include "Proxy/Data/HawkerHistoryData.h"
#include "Proxy/Data/GenerationData.h"
#include "Proxy/Logic/MessageCharacter.h"
#include "Proxy/Logic/Messenger.h"
#include "Proxy/Logic/Guild.h"
#include "Proxy/Logic/GuildMember.h"
#include "Stub/Service/MapService.h"
#include "Stub/Logic/Map.h"
#include "Stub/Logic/MapNPC.h"
#include "Stub/Logic/Party.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/Partner.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/Item.h"
#include "Stub/Logic/Battle.h"
#include "Stub/Logic/ScriptSystem.h"
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/SkillGroup.h"
#include "Stub/Logic/QuestLog.h"
#include "Stub/Logic/QuestType.h"
#include "Stub/Logic/QuestSystem.h"
#include "Stub/Logic/Trading.h"
#include "Stub/Logic/Pet.h"
#include "Stub/Logic/PetGroup.h"
#include "Stub/Logic/BatCharacter.h"
#include "Stub/Logic/Question.h"
#include "Stub/Logic/Trading.h"
#include "Stub/Logic/HawkGroup.h"
#include "Stub/Logic/GiftBox.h"
#include "Stub/Logic/GenerationGroup.h"
#include "MapService/MapApplication.h"
#include "MapService/Global.h"
#include "MapService/Configure.h"
#include "MapService/ViewManager.h"
#include "MapService/AntiGameAddiction.h"
#include "MapService/QuestionControl.h"
#include "MapService/Instance.h"
#include "MapService/MapCheat.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResSkill.h"
#include "Resource/ResItem.h"
#include "Resource/ResMob.h"
#include "Resource/ResSkillShop.h"
#include "Resource/ResCharacter.h"
#include "Resource/ResNpc.h"
#include "Resource/ResBuff.h"
#include "Resource/ResRandTable.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResTitle.h"
#include "Resource/ResHackImage.h"
#include "Resource/GameStringTable.h"
#include "Resource/ResAccmulationShop.h"
#include "Resource/ResPointsShop.h"
#include "Resource/AccmulationShop.h"
#include "Stub/Logic/Trading.h"
#include "Resource/ResPet.h"
#include "Resource/ResTitle.h"
#include "RPCEnum/Account_enum.h"
#include "Stub/Logic/Generation.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include "MapService/Math_QGL.h"
#include "Proxy/Data/MarriageData.h"

#include "Resource/ResPartnerCollection.h"
#include "Proxy/Data/PartnerCollectionData.h"

#include "Resource/ResAutoPath.h"
#include "Resource/ResSkillCombination.h"

#include "Stub/Logic/BatActor.h"
#include "Stub/Logic/BatMob.h"

typedef StlMap<UInt32,const PartnerCollectionData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, const PartnerCollectionData*> > > PartnerCollectionDataMap;

#include <Reuben/Platform/File.h>
#include "Stub/Logic/RelationSystem.h"

typedef Reuben::Platform::File File;
typedef Reuben::Platform::FileException FileException;

#define IDLETIMER_LIMIT			1000
#define AP_PER_LEVEL			5
#define AP_PER_LEVEL2			10
#define AP_PER_LEVEL3			15
#define PK_LEVEL_LIMIT			30
//#define PK_MIN_HORNOR			150
#define MAX_HELP_COUNT			4

#define HELPER_REJECT_DELAY		180000
#define BATTLE_DEADLOCK_TIME	10000
#define TRADING_TIMELIMIT		10000
#define MAX_GUILD_NAME_SIZE		6

#define CHAR_LEVEL_LIMIT	89
#define CHAR_LEVEL_LIMIT2	129
#define CHAR_LEVEL_LIMIT3	149
#define MAX_HISTORY 45
#define MAX_AVAIL_BACKPACK	3

#define ROBOT_INTERVAL 2000

const int GENERATIONLIMITEDNUM = 30;
const UInt16 GENERATIONLIMITEDLEVEL = 50;

#define ROBOT_SCRIPT			_T("Data\\qgl_robot.out")

enum {
	ANTI_HACK_IMAGE_ONE = 0,
	ANTI_HACK_IMAGE_TWO = 1,
	ANTI_HACK_IMAGE_THREE = 2,
	ANTI_HACK_IMAGE_FOUR = 3,
	ANTI_HACK_IMAGE_FIVE = 4,
	ANTI_HACK_IMAGE_SIX = 5,

};

PROTO_LuaPush(CCharacter)
PROTO_LuaPush(AttriStruct)
PROTO_LuaPush(ExtractedBuffData)
PROTO_LuaPush(StartCharData)
PROTO_LuaPush(CRelationSystem)

enum {
	CONCERN_NONE = 0,
	CONCERN_NPC = 1,
	CONCERN_CHAR = 2,
	CONCERN_MEMBER = 3,
	CONCERN_SELF = 4,
};

REGISTER_CLASSTYPE(CCharacter, Character_CLASSID);

// init class static values
RPCMask CCharacter::m_initMask = RPCMask();
RPCMask CCharacter::m_npcMask = RPCMask();
RPCMask CCharacter::m_charMask = RPCMask();
RPCMask CCharacter::m_memberMask = RPCMask();
RPCMask CCharacter::m_selfMask = RPCMask();
RPCMask CCharacter::m_simpleMask = RPCMask();

///////////////////////////////////////////////////////////////////////////////
///																			///
///								State methods								///
///																			///
///////////////////////////////////////////////////////////////////////////////

VOID CCharacter::CNoBattleState::Enter(VOID)
{
	m_pchar->m_BattleNetID = RPCNetID();
	m_pchar->m_pBatChar = NULL;
	m_pchar->SetAction(ST_NOBATTLE, ::timeGetTime());

	// jump to reborn when current map closed
	if (m_pchar->GetMap() && !m_pchar->GetMap()->IsOpen())
		m_pchar->JumpReborn();
}

CONST Boolean CCharacter::CBattleState::CanTransitionFrom(CONST StateID currentId) CONST
{
	return ((currentId == ST_NOBATTLE || currentId == ST_MONITOR) && 
			m_pchar->IsState(ST_ENTER_MAP) && !m_pchar->IsState(ST_HAWK));
}

VOID CCharacter::CBattleState::Enter(VOID)
{
	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("char_id %d, sessionId %s"), m_pchar->GetChar_id(), m_pchar->GetSessionId().GetString().c_str());
	m_pchar->m_mapState->Block();
	m_pchar->m_moveState->Block();
	m_pchar->SetAction(ST_BATTLE, ::timeGetTime());
}

VOID CCharacter::CBattleState::Exit(VOID)
{
	m_pchar->m_moveState->Unblock();
	m_pchar->m_mapState->Unblock();
	m_pchar->ClearAllHelpers();
	m_pchar->m_MatchCharID = 0;
	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("char_id %d, sessionId %s"), m_pchar->GetChar_id(), m_pchar->GetSessionId().GetString().c_str());
}

CONST Boolean CCharacter::CMonitorState::CanTransitionFrom(CONST StateID currentId) CONST
{
	return (currentId == ST_NOBATTLE && 
			m_pchar->IsState(ST_ENTER_MAP) && !m_pchar->IsState(ST_HAWK));
}

VOID CCharacter::CMonitorState::Enter(VOID)
{
	m_pchar->m_mapState->Block();
	m_pchar->m_moveState->Block();
	m_pchar->SetAction(ST_MONITOR, ::timeGetTime());
}

VOID CCharacter::CMonitorState::Exit(VOID)
{
	m_pchar->m_moveState->Unblock();
	m_pchar->m_mapState->Unblock();
	m_pchar->LeaveHelpersList();
}

CONST Boolean CCharacter::CTalkState::CanTransitionFrom(CONST StateID currentId) CONST
{
	return (currentId == ST_STOP);
}

VOID CCharacter::CTalkState::Enter(VOID)
{
	m_pchar->SetAction(ST_TALK, ::timeGetTime());
}

VOID CCharacter::CTalkState::Exit(VOID)
{
	m_pchar->SetAction(ST_STOP, ::timeGetTime());
}

///////////////////////////////////////////////////////////////////////////////
///																			///
///								Character methods							///
///																			///
///////////////////////////////////////////////////////////////////////////////

CCharacter::CCharacter()
{
	//m_initAutoCount = 25;
	m_initAutoCount = 600;	//2012.1.7 Modified by Jay, Default value SET 600
	m_bAntiHackEnable = BOOLEAN_TRUE;

	m_pPeer = NULL;
	m_battleState = NULL;
	ChangeState(ST_NOBATTLE);
	m_pParty = NULL;
	m_BattleNetID = RPCNetID();
	m_movedStep = 0;
	m_stepMod = 0;
	m_sessLogout = BOOLEAN_FALSE;
	m_logoutHandle = 0;
	m_autoCount = 0;
	m_ReturnFlag = 0;
	m_pBatChar = NULL;
	m_pHawkGroup = NULL;
	m_pGiftBox = NULL;
	m_bForcePopUp = BOOLEAN_FALSE;	
	//m_pHawkHistoryGroup = NULL;
	m_hitBoss = BOOLEAN_TRUE;


	m_pBag = CItemBag::NewInstance();
	m_pBag->SetOwner(this);
	m_pSkillGroup = CSkillGroup::NewInstance();
	m_pSkillGroup->SetOwner(this);
	m_pPartnerGroup = CPartnerGroup::NewInstance();
	m_pPartnerGroup->SetOwner(this);
	m_pScriptSys = CScriptSystem::NewInstance();
	m_pScriptSys->SetOwner(this);
	m_pScriptSys->SetRunOnce(BOOLEAN_TRUE);
	m_pQuestSys = CQuestSystem::NewInstance();
	m_pQuestSys->SetOwner(this);
	m_pPetGroup = CPetGroup::NewInstance();
	m_pPetGroup->SetOwner(this);
	m_pQuestion = CQuestion::NewInstance();
	m_pQuestion->SetOwner(this);
	m_pRelationSystem = CRelationSystem::NewInstance();
	m_pRelationSystem->SetOwner(this);

	m_changeMobSkillLst = CSkillGroup::NewInstance();
	m_changeMobSkillLst->SetOwner(this);

	m_pGeneration = SafeCreateObject(CGenerationGroup);
	if (NULL != m_pGeneration) {
		m_pGeneration->SetOwner(this);
	}

	m_pAntiGameAddict = NULL;
	m_bAssetPasswordVerified = BOOLEAN_FALSE;	

	for (Int i = 0; i < MAX_CHANNELTYPE; i++)
	{
		m_speakRights[i] = true;
	}
	m_Listen = TRUE;
	m_PreSpeakTime = 0;

	SetPKInfo(0);
	SetBattleInfo(TRUE);

	m_pTrading = NULL;
	m_pTradingTargetNetID = RPCNetID();
	m_battleTeam = 0;
	m_OpeningGuild = FALSE;

	m_MatchCharID = 0;
	m_tempSkillShopID = 0;
	m_EncounterBattle = false;

	m_bCacheWareHouse = false;
	m_viewManager = SafeCreateObject1(ViewManager, this);

	m_TempSkillID = 0;
	m_TempSkillTargetID = 0;
	m_cmd = BATCMD_ATTACK;
	m_sub_type = 0;
	m_cmdTarget = 0;
	m_cmdTargetType = 1;
	m_additional = 0;
	m_WrongAns = 0;	
	m_iLastJumpLineTime = GetTickCount();

	m_ownGuild = NULL;
	m_ownGuildMember = NULL;

	m_expReward = BOOLEAN_FALSE;	
	m_hpReward = BOOLEAN_FALSE;	
	m_bStartBattle = BOOLEAN_FALSE;

	m_bIsUpdatedYuanBao = false;
	m_bRobot = false;

	m_hawkerHistoryMap.clear();

	m_noHackPopUpTime = 0;

	m_battleRestTime = 0;

	m_creatingGuild = FALSE;

	m_bUsePartnerCollectionBuff = false;
	m_marriageCharNpc = NULL;
	m_charViewLst.clear();

	m_bGenerationInit=FALSE;

	m_bManorExpel = BOOLEAN_FALSE;
	m_bEnterDone = BOOLEAN_FALSE;

	m_pTemporaryParty = NULL;

	m_pUseTwoPet = NULL;

	m_pvpBattleScriptID = 0;

	if (!m_initMask.IsZero()) return;

	// initialize static mask
	m_initMask.MaskBits(infoMask, mapMask, curPosMask, -1);

	// simple mask for quick preview of actor
	m_simpleMask.MaskBits(infoMask, mapMask, curPosMask, dstPosMask, actionMask,
		speedMask, followIdMask, appearedMask, idMask, -1);

	m_npcMask.MaskBits(CMapNPC::idMask, CMapNPC::dstPosMask, CMapNPC::actionMask,
		CMapNPC::speedMask, CMapNPC::followIdMask, CMapNPC::visibleMask,-1);

	m_charMask.MaskBits(idMask, levelInfoMask, styleMask, eqWeaponMask, eqHeadMask, 
		eqClothMask, dstPosMask, actionMask, speedMask, partnerMask, petMask,
		followIdMask, partyInfoMask, BattleInfoMask, PKInfoMask, buffMask,buffCacheMask,
		orgEqClothMask, orgHairStyleMask, SettingInfoMask, titleMask, hawkInfoMask, 
		followerListMask,markMask,manorMask,TPMask, -1); 

	m_memberMask.Mask(m_charMask);
	m_memberMask.MaskBits(HPMask, SPMask, -1);

	m_selfMask.MaskBits(infoMask, idMask, levelInfoMask, expMask, attrib1Mask, attrib2Mask,
		attrPointMask, 
		HPMask, SPMask, DPMask, WCMask, VPMask, goodnessMask, honorMask, titleMask, donateMask,
		yuanBaoMask, moneyMask, bankMoneyMask, partnerMask, petMask, styleMask,  shortcutMask,
		eqWeaponMask, maxOtherMask, eqClothMask, eqHeadMask, stateMask, 
		buffMask,buffCacheMask,  partyInfoMask, curPosMask, dstPosMask, actionMask, 
		speedMask, followIdMask, BattleInfoMask, PKInfoMask, buffMask, orgEqClothMask, 
		orgHairStyleMask, SettingInfoMask, yuanbaoLockMask, bankLockMask,
		offlineTimeMask, speakCountMask, TPMask, speakBlockMask, hawkerTimeMask, hawkInfoMask,
		SMoneyMask, availBackPackMask, availWareHouseMask, followerListMask, unlockTimeMask, manorMask,-1);
}

CCharacter::~CCharacter()
{
	if (m_pQuestion)
		CQuestionControl::GetInstance().OnQuestionExit(m_pQuestion,FALSE);

	CMarriageData * pMarriageData = m_pRelationSystem->GetMarriageData();
	CCharacter* pChar = NULL;
	if (pMarriageData != NULL)
	{
		pChar = gGlobal.m_charToSession.FindCharacter(pMarriageData->GetHusband_id() == GetChar_id() ? pMarriageData->GetWife_id():pMarriageData->GetHusband_id());
	}
	if( pChar != NULL && pChar != this) 
	{
		CMapNPC * pMapNPC = pChar->GetCharNpc();
		if (pMapNPC)
		{
			pMapNPC->SetLifeTime(0);
			pMapNPC->Destroy();
			pChar->SetCharNpc(NULL);
			pChar->SetVisible(BOOLEAN_TRUE);
		}
		pChar->GetRelationSystem()->OnCompanionExit(GetChar_id());
	}

	if(m_marriageCharNpc != NULL)
	{
		m_marriageCharNpc->SetLifeTime(0);
		m_marriageCharNpc->Destroy();
		m_marriageCharNpc = NULL;
	}

	m_BattleNetID = RPCNetID();
	m_pBatChar = NULL;
	m_charViewLst.clear();
	CancelAllEvent();				// cancel all events
	CancelAllRequests();
	ClearAllHelpers();
	ChangeState(ST_LEAVE_MAP);

	if (m_pParty) {
		m_pParty->Leave(this);
		m_pParty = NULL;
	}
	
	if (NULL != m_pTemporaryParty) 
	{
		m_pTemporaryParty->Leave(this);
		m_pTemporaryParty = NULL;
	}
	//if (m_pTrading)
	//	m_pTrading->StopTrading(GetChar_id());
	CMap *pmap = m_pMap;
	OnLeaveMap();
	ClearPeer();

	// safe remove from other character view in map
	if (pmap)
		pmap->SafeRemove(this);

	CBattle * pBattle = FindNetObject<CBattle>(m_BattleNetID);
	if (pBattle)
	{
		TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("battle exist when delete character, sessionId %s, char_id %d, battle %d:%d"),
			m_context.sessionId.GetString().c_str(), GetChar_id(), pBattle->GetNetId().srvId,
			pBattle->GetNetId().objId);
	}

	State::ChangeState(m_battleState, NULL);
	DeleteNetObject(m_pBag);
	DeleteNetObject(m_pScriptSys);
	DeleteNetObject(m_pQuestSys);
	DeleteNetGroup(m_pPartnerGroup, BOOLEAN_TRUE);
	DeleteNetGroup(m_pSkillGroup, BOOLEAN_TRUE);
	DeleteNetGroup(m_pPetGroup, BOOLEAN_TRUE);
	DeleteNetObject(m_pTrading);
	DeleteNetObject(m_pQuestion);
	DeleteNetObject(m_pRelationSystem);
	DeleteNetObject(m_pUseTwoPet);
	DeleteNetGroup(m_changeMobSkillLst,BOOLEAN_TRUE);

	if (m_pHawkGroup)
		DeleteNetGroup(m_pHawkGroup, BOOLEAN_TRUE);

	if (m_pGiftBox)
		DeleteNetGroup(m_pGiftBox, BOOLEAN_TRUE);

	SafeDeleteObject(m_viewManager);

	for (TitleMap::iterator Itr = m_titleDataLst.begin(); 
		Itr != m_titleDataLst.end(); ++Itr)
	{
		SafeDeleteObject(Itr->second);
	}

	m_titleDataLst.clear();
	m_hawkerHistoryMap.clear();
	if (m_pAntiGameAddict){
		SafeDeleteObject(m_pAntiGameAddict);
		m_pAntiGameAddict = NULL;
	}

	m_iLastJumpLineTime = 0;

	for (PartnerCollectionMap::iterator Itr = m_PartnerCollectionLst.begin(); 
		Itr != m_PartnerCollectionLst.end(); ++Itr)
	{
		SafeDeleteObject(Itr->second);
	}
	m_PartnerCollectionLst.clear();
	
	if (NULL != m_pGeneration) 
		SafeDeleteObject(m_pGeneration);

	m_pvpBattleScriptID = 0;
}

String CCharacter::DebugInfo()
{
	String s(Parent::DebugInfo());
	s.AppendFormat(_T(", acct_id %d, char_id %d, curRegion %d, newRegion %d"), 
		GetAcct_id(), GetChar_id(), m_curRegion, m_newRegion);
	return s;
}

VOID CCharacter::ClearPeer()
{
	//m_context.sessionId.serviceId = INVALID_SERVICE_ID;
	//m_context.sessionId.socketId = INVALID_SOCKET_ID;
	//m_context.sessionId.ticketId = 0;

	if (m_pPeer) {			// decrease player count when clear peer
		gGlobal.m_playerCount --;
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Player Count: decreased %d, char_id %d"), 
			gGlobal.m_playerCount, GetChar_id());
	}

	SafeRelease(m_pPeer);
	m_pPeer = NULL;
}

VOID CCharacter::SetSessionId(const SessionID &sid)
{
	m_context.sessionId = sid;
	m_sessionId = sid;
	SetOwnObject(sid, this);
	SetOwnObject(sid, m_pBag);
	SetOwnObject(sid, m_pSkillGroup);
	SetOwnObject(sid, m_pPartnerGroup);
	m_pPartnerGroup->InitPartnerOwnObject();
	SetOwnObject(sid, m_pQuestSys);
	SetOwnObject(sid, m_pScriptSys);
	SetOwnObject(sid, m_pPetGroup);
	SetOwnObject(sid, m_pQuestion);
	SetOwnObject(sid, m_pRelationSystem);
	m_pPetGroup->InitPetOwnObject();	
}

VOID CCharacter::SetPeer(const SessionID& sid)
{
	LPCPEER pPeer = GetProxy(sid.serviceId);
	if (pPeer == NULL) {
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("Character SetSession Warning: no peer for sessionId %s"),
			sid.GetString().c_str());
		return;
	}
	if (m_pPeer == NULL) {				// increase player count when new peer
		gGlobal.m_playerCount ++;
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Player Count: increased %d, char_id %d"), 
			gGlobal.m_playerCount, GetChar_id());
	}
	m_pPeer = pPeer->AddRef();
}

void CCharacter::SetCharInfo(RpcCharInfo* pCharInfo, UInt32Vector &partnerIds)
{
	if (pCharInfo == NULL) return;

	UInt32 partnerId;

	pCharInfo->sid = m_sessionId;
	pCharInfo->netId = GetNetId();
	pCharInfo->char_id = GetChar_id();
	
	pCharInfo->bConnected = (m_pPeer != NULL);
	pCharInfo->petID = GetPet_id();
	pCharInfo->autoData.autoCount = m_autoCount;
	pCharInfo->autoData.cmd = m_cmd;
	pCharInfo->autoData.sub_type = m_sub_type;
	pCharInfo->autoData.target = m_cmdTarget;
	pCharInfo->autoData.additional = m_additional;

	for (Index i = C_INDEX(0); i < m_pPartnerGroup->GetCount(); ++i)
	{
		partnerId = m_pPartnerGroup->GetPartnerID(i);
		if (partnerId != 0)
			partnerIds.push_back(partnerId);
	}
}

void CCharacter::SendTitle()
{
	UInt16Vector id_list;
	StringVector labels; 
	UInt8Vector states;

	for (TitleMap::iterator Itr = m_titleDataLst.begin();
		Itr != m_titleDataLst.end(); ++Itr)
	{
		TitleInfo* pTitle = Itr->second;
		if (pTitle == NULL || !pTitle->IsExist())
			continue;
		id_list.push_back(pTitle->title_id);
		labels.push_back(pTitle->title_name);
		states.push_back(TITLE_STATE_ADD);
	}

	CB_TitleListUpdate(m_pPeer, m_context, id_list, labels, states);

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_TitleListUpdate char_id: %d"), GetChar_id());
}


void CCharacter::CalcAddiction(const AntiAddictInfo &addictInfo)
{
	/// AntiAddiction system part 1 kicks in ///
	if (m_pAntiGameAddict == NULL)
		CreateAntiAddictionObject();
	if (m_pAntiGameAddict)
	{
		// set the data
		m_pAntiGameAddict->SetAddictData(addictInfo);
		// if antihack system activate
		if (gConfigure.antiAddict == 1 && addictInfo.needAntiAddict == 1)
		{
			if (!addictInfo.startHack)
				m_pAntiGameAddict->LoginInitialization(m_pPeer, m_context, 
					this, addictInfo);
			m_pAntiGameAddict->SetCurrentAddictionHealth();	
		}
		else
		{
			m_pAntiGameAddict->m_healthstatus = 0;
			m_pAntiGameAddict->m_playerInterest = 1;
		}
	}
}

void CCharacter::SendClient(Boolean bDetail, Boolean bSendOther)
{
	if (m_pPeer == NULL || m_pMap == NULL)
		return;

	TRACE_ENSURE(m_pPeer);
	RPCMask mask;

	mask.MaskBit(CMap::infoMask);
	m_pMap->SetSendMask(mask);

	TRACE_INFODTL_7(GLOBAL_LOGGER, _F("detail %d, char_id %d, map_id %d, map netId %d:%d, char netId %d:%d"), 
		bDetail, GetChar_id(), m_pMap->GetMap_id(), m_pMap->GetNetId().srvId, m_pMap->GetNetId().objId,
		GetNetId().srvId, GetNetId().objId);

	// check if local jump
	if (bDetail) {
		mask = m_initMask;
		mask.Mask(m_selfMask);
	} else {
		mask.Clear();
		mask.MaskBits(CCharacter::mapMask, CCharacter::curPosMask, CCharacter::dstPosMask, 
			CCharacter::actionMask, -1);
		mask.Mask(GetViewMask());
	}
	SetSendMask(mask);
	m_viewManager->ClearView(this, mask);

	UInt8 healthstatus = 0;
	if (m_pAntiGameAddict)
		healthstatus = m_pAntiGameAddict->m_healthstatus;	

	MapService_Stub::CB_EnterSuccess(m_pPeer, m_context, m_pMap, this);

	if (bDetail)
		MapService_Stub::CB_CharacterStuff(m_pPeer, m_context, m_pBag, m_pScriptSys, 
			m_pQuestSys, m_pSkillGroup, m_pPartnerGroup, m_pPetGroup, healthstatus);

	if (bSendOther)
	{
		m_pBag->SendAll();
		m_pPartnerGroup->SendAll();
		m_pPetGroup->SendAll();
		m_pSkillGroup->SendAll();
		m_pQuestSys->SendAll();
		m_pQuestSys->SendQuestHint();	// resend quest hint
		SendTitle();
		SendPartnerCollection();

		if (GetAction() == ST_HAWK && m_pHawkGroup)
		{
			NetGroup dummy, dummy1, dummy2;
			UInt16Vector dummyVector, dummyVector1;

			m_pHawkGroup->StopOfflineCount();
			m_pHawkGroup->AddCharContext(GetChar_id(), GetContext());
			m_pHawkGroup->PreSend();
			CB_GetHawk(m_pPeer, m_context, BOOLEAN_TRUE, m_pHawkGroup, &dummy, &dummy1, 
				dummyVector, &dummy2, dummyVector1);
			if (m_hawkerHistoryMap.size() > 0)
				SendAllHistoryToClient();
		}
	}
}

CPartner* CCharacter::GetCurPartner()
{
	return m_pPartnerGroup->GetFightPartner();
}

CPet* CCharacter::GetCurPet()
{
	return m_pPetGroup->GetCurPet();
}

CItem* HelpToGetItem(CCharacter* pCharacter, CItemBag* pBag, UInt8 uIndex, UInt uStep)
{
	//__try
	//{
		return (pBag->GetItem(uIndex));
	//}
	//__except(EXCEPTION_EXECUTE_HANDLER)
	//{
	//	TRACE_ERRORDTL_4(GLOBAL_LOGGER, _F("HelpToGetItem Error: (pBag->GetItem(uIndex) Failed) Character %d, pBag %d, i %d, Step %d"), pCharacter, pBag, uIndex, uStep);
	//}
	return NULL;
}

void CCharacter::CalcAttrib()
{
	if (m_pBag == NULL)
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CCharacter CalcAttrib Warning: null bag"));
		return;
	}

	// base STA, SPR, STR, CON, AGI
	Int16 sta = GetSTA();
	Int16 spr = GetSPR();
	Int16 str = GetSTR();
	Int16 con = GetCON();
	Int16 agi = GetAGI();

	// first increase STA, SPR, STR, CON, AGI by equipment
	for (UInt i = BAG_MAX_ITEM; i < BAG_MAX_ITEM + BAG_EQUIP_COUNT; i++)
	{
		CItem *item = HelpToGetItem(this, m_pBag, i, 1);
		if (item && item->GetEndure() > 0) {
			sta += item->GetSTA();
			spr += item->GetSPR();
			str += item->GetSTR();
			con += item->GetCON();
			agi += item->GetAGI();
		}
	}

	// increase ... by itemset
	m_pBag->CalcItemSetBasicAttrib(sta, spr, str, con, agi);

	if (sta < 0 )
		sta = 0;
	if (spr < 0)
		spr = 0;
	if (str < 0)
		str = 0;
	if (con < 0)
		con = 0;
	if (agi < 0)
		agi = 0;

	// attribute formula table index
	//const CharAttrSet &f = gCharAttrSet[GetCclass()-1];
	const StartCharData* pCharData = gGlobal.m_resource->GetStartCharData(GetSexClass());

	StartCharData temp;
	temp = *pCharData; 
	CallLuaFunc(gGlobal.g_pLuaState, "CalCharAttr", String(), this, &temp, sta, spr, str, con, agi);
	
	float atk = GetATK();
	float def = GetDEF();
	float hit = GetHIT();
	float eva = GetEVA();
	float pow = GetPOW();
	float spd = GetSPD();
	UInt16 eq_maxHp = 0;
	UInt16 eq_maxSp = 0;
	
	// second increase ATK, DEF, HIT, EVA, POW, SPD by equipment
	for (UInt i = BAG_MAX_ITEM; i < BAG_MAX_ITEM + BAG_EQUIP_COUNT; ++i)
	{
		CItem* item = HelpToGetItem(this, m_pBag, i, 2);
		if (item && item->GetEndure() > 0) {
			atk += item->GetATK();
			def += item->GetDEF();
			hit += item->GetHIT();
			eva += item->GetEVA();
			pow += item->GetPOW();
			spd += item->GetSPD();
			eq_maxHp += item->GetMaxHP();
			eq_maxSp += item->GetMaxSP();
		}
	}


	// increase ... by itemset
	m_pBag->CalcItemSetBatAttrib(atk, def, hit, eva, pow, spd, eq_maxHp, eq_maxSp);

	// calculate Max HP,SP by attribute

	//float maxHp = sta * pCharData->MaxHP_STA + 160 + eq_maxHp;
	//float maxSp = (GetLevel() + spr) * pCharData->MaxSP_SPR + 80 + eq_maxSp;

	if (atk < 0)
		atk = 0;
	if (def < 0)
		def = 0;
	if (hit < 0)
		hit = 0;
	if (eva < 0)
		eva = 0;
	if (pow < 0)
		pow = 0;
	if (spd < 0)
		spd = 0;
	if (eq_maxHp < 0)
		eq_maxHp = 0;
	if (eq_maxSp < 0)
		eq_maxSp = 0;

	float maxHp = (float)GetMaxHP() + eq_maxHp;
	float maxSp = (float)GetMaxSP() + eq_maxSp;

	// increase ... by itemset
	m_pBag->CalcItemSetBatAttribMaxHpSp(maxHp, maxSp);

	UInt16 maxDp = GetMaxDP();
	UInt16 maxWc = GetMaxWC();

	AttriStruct TempAttri;
	TempAttri.uIntAtk = (int)atk;
	TempAttri.uIntDef = (int)def;
	TempAttri.uIntHit = (int)hit;
	TempAttri.uIntEva = (int)eva;
	TempAttri.uIntPow = (int)pow;
	TempAttri.uIntSpd = (int)spd;
	TempAttri.uIntMaxHp = (int)maxHp;
	TempAttri.uIntMaxSp = (int)maxSp;
	TempAttri.uIntMaxDp = maxDp;
	TempAttri.uIntMaxWc = maxWc;
	for (SkillMap::iterator Itr = m_skillMap.begin(); Itr != m_skillMap.end(); ++Itr)
	{
		const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(Itr->second->GetSkill_id());
		if (pSkillData)
		{
			StringA LuaFunc;
			LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second->GetSkill_id(), "skill").c_str(), CommonSkillFuncName[SKILL_FUNC_NAME_ATTRI].c_str());
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), this, 
					Itr->second->GetSkill_level(), &TempAttri);
		}
	}

	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData)// && Itr->second.pBuffData->IsActive(ActiveType))
		{
			for (Int k = 0; k < (Int)Itr->second.pBuffData->Buff_acttimes.size(); k++)
			{
				if (Itr->second.pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
				{
					StringA LuaFunc;
					LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second.pBuffData->id, "buff").c_str(), CommonBuffFuncName[BUFF_FUNC_NAME_ATTRI].c_str());
					if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(),  this, 
							&Itr->second, &TempAttri);
				}
			}
		}
	}

	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData)// && Itr->second.pBuffData->IsActive(ActiveType))
		{
			for (Int k = 0; k < (Int)Itr->second.pBuffData->Buff_acttimes.size(); k++)
			{
				if (Itr->second.pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
				{
					StringA LuaFunc;
					LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second.pBuffData->id, "buff").c_str(), CommonBuffFuncName[BUFF_FUNC_NAME_ATTRI].c_str());
					if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), this,  
							&Itr->second, &TempAttri);
				}
			}
		}
	}

	SetAttrib2((UInt16) TempAttri.uIntAtk, (UINT16) TempAttri.uIntDef , (UINT16) TempAttri.uIntHit , 
		(UINT16) TempAttri.uIntEva , (UINT16) TempAttri.uIntPow , (UINT16) TempAttri.uIntSpd);

	SetHP(GetHP(), GetMHP(), (UInt16) TempAttri.uIntMaxHp);
	if (GetHP() > GetMaxHP())
		SetHP(GetMaxHP());
	if (GetMHP() > GetMaxHP())
		SetMHP(GetMaxHP());

	SetSP(GetSP(), (UInt16) TempAttri.uIntMaxSp, (UInt16) TempAttri.uIntMaxSp);
	if (GetSP() > GetMaxSP())
		SetSP(GetMaxSP());
	if (GetMSP() > GetMaxSP())
		SetMSP(GetMaxSP());
	SetDP(GetDP(), GetMDP(),  TempAttri.uIntMaxDp);
	SetWC(GetWC(), TempAttri.uIntMaxWc, GetWcExp(), GetNextWCExp());

	UInt8 maxPet = (Int)(GetLevel() / 10 ) + 1;
	if (maxPet > 8)
		maxPet = 8;
	SetMaxOther(GetMaxPartner(), maxPet);
	
}

void CCharacter::CalcNextLevel()
{
	CallLuaFunc(gGlobal.g_pLuaState, "CalPlayerNextLvExp", String(), GetLevel() + 1);
	SetNextLvExp((UInt32)gGlobal.g_TempResult);
}

void CCharacter::CalcNextWcLevel()
{
	CallLuaFunc(gGlobal.g_pLuaState, "CalWCNextLvExp", String(), GetWC() + 1);
	SetNextWCExp((UInt32)gGlobal.g_TempResult);
}

void CCharacter::WCLevelUp()
{
	SetWC(GetWC() + 1);
	SetDP(GetDP(), 150 , 150);
	CalcNextWcLevel();
}
		
void CCharacter::ResetCharLevel(UInt16 lv)
{
	SetLevel(lv);
	//SetExp(0);
	UInt16 wPoint = (lv * AP_PER_LEVEL);
	if ( lv > CHAR_LEVEL_LIMIT2 )
		wPoint = wPoint + (lv - CHAR_LEVEL_LIMIT2) * (AP_PER_LEVEL3 - AP_PER_LEVEL2);
	if ( lv > CHAR_LEVEL_LIMIT )
		wPoint = wPoint + (lv - CHAR_LEVEL_LIMIT) * (AP_PER_LEVEL2 - AP_PER_LEVEL);
	SetAttrPoint(wPoint);

	const StartCharData* charData = gGlobal.m_resource->GetStartCharData(1 + (GetCclass() - 1) * 2 + GetSex());
	if ( charData )
	{
		UInt16 sta = charData->STA;
		UInt16 spr = charData->SPR;
		UInt16 str = charData->STR;
		UInt16 con = charData->CON;
		UInt16 agi = charData->AGI;

		SetAttrib1(sta + lv, spr + lv, str + lv, con + lv, agi + lv);

		//const CharAttrSet &f = gCharAttrSet[GetCclass()-1];
		/*
		float atk = str * f.atk_str + spr * f.atk_spr + 40;
		float def = con * f.def_con;
		float hit = str * f.hit_str + 30;
		float eva = agi * f.eva_agi;
		float pow = spr * f.pow_spr + sta * f.pow_sta + con * f.pow_con + str * f.pow_str;
		float spd = agi * f.spd_agi + sta * f.spd_sta + str * f.spd_str;
		*/

		float hp = (charData->STA * charData->MaxHP_STA + 100);
		float sp = (GetLevel() + charData->SPR) * charData->MaxSP_SPR;

		UInt16 maxHp = (UInt16)hp;
		UInt16 maxSp = (UInt16)sp;

		SetHP(maxHp, maxHp, maxHp);
		SetSP(maxSp, maxSp, maxSp);
	}

	
	/*
	SetAttrib1(GetSTA() + sta, GetSPR() + spr, GetSTR() + str,
		GetCON() + con, GetAGI() + agi);
	SetAttrPoint(GetAttrPoint() - totalAp);
	CalcAttrib();
	*/

	CalcAttrib();
	CalcNextLevel();

	SetHP(GetMaxHP(), GetMaxHP(), GetMaxHP());
	SetSP(GetMaxSP(), GetMaxSP(), GetMaxSP());
	if (m_pQuestSys) {
		m_pQuestSys->CheckAllHints();
		m_pQuestSys->CheckJournals();
	}
}

Boolean CCharacter::ReverseAttrib(UInt8 attrType, UInt16 point)
{
	const StartCharData* charData = gGlobal.m_resource->GetStartCharData(GetSexClass());
	if (charData == NULL)
		return BOOLEAN_FALSE;

	UInt16 level = GetLevel();
	UInt16 sta = GetSTA();
	UInt16 spr = GetSPR();
	UInt16 str = GetSTR();
	UInt16 con = GetCON();
	UInt16 agi = GetAGI();

	switch(attrType)
	{
	case 1:
		if (sta < point || sta - point < charData->STA + level)
			return false;
		sta -= point;
		break;
	case 2:
		if (spr < point || spr - point < charData->SPR + level)
			return false;
		spr -= point;
		break;
	case 3:
		if (str < point || str - point < charData->STR + level)
			return false;
		str -= point;
		break;
	case 4:
		if (con < point || con - point < charData->CON + level)
			return false;
		con -= point;
		break;
	case 5:
		if (agi < point || agi - point < charData->AGI + level)
			return false;
		agi -= point;
		break;
	default:
		return false;
	}

	SetAttrib1(sta, spr, str, con, agi);
	SetAttrPoint(GetAttrPoint() + point);
	
	CalcAttrib();
	SetHP(GetMaxHP(), GetMaxHP(), GetMaxHP());
	SetSP(GetMaxSP(), GetMaxSP(), GetMaxSP());
	RaiseUpdate();

	return BOOLEAN_TRUE;
}

RPCResult CCharacter::LevelUp(LPCPEER pPeer, RPCContext &context)
{
	
	if (IsState(ST_BATTLE))
	{
//		CB_Error(pPeer, context, 1, _T("Character LevelUp Error: (cannot raise level inside battle)"));
		return (RPC_RESULT_FAIL);
	}

	if(HasLuaFunc(gGlobal.g_pLuaState,"CanLevelUp"))
	{
		CallLuaFunc(gGlobal.g_pLuaState,"CanLevelUp", String(), this);
		Boolean result = (gGlobal.g_TempResult > 0.0f);
		if (!result)
		{
			return (RPC_RESULT_FAIL);
		}
	}

	if (GetLevel() + 1 > CHAR_LEVEL_LIMIT3)
	{
		CB_Error(pPeer, context, 1, _T("MSG_LEVEL_LIMIT_90"));
		return (RPC_RESULT_FAIL);
	}
	
	UInt32 uiNextLvExp = GetNextLvExp();
	if (GetExp() < uiNextLvExp)
	{
//		CB_Error(pPeer, context, 1, _T("CCharacter LevelUp Error: (not enough EXP to level up)"));
		return RPC_RESULT_FAIL;
	}

	// raise up one level
	SetLevel(GetLevel() + 1);
	ChangeExp(- (Int32)GetNextLvExp(), LogService_Proxy::Money_And_Exp::SOURCE_LEVELUP);
	if (GetLevel() < 10)
	{
		if (GetCclass() == 1)
			SetAttrib1(GetSTA(), GetSPR() + 4, GetSTR(), GetCON() + 1, GetAGI());
		else if(GetCclass() == 2)
			SetAttrib1(GetSTA() + 2, GetSPR(), GetSTR(), GetCON() + 2, GetAGI() + 1);
		else if(GetCclass() == 3)
			SetAttrib1(GetSTA() + 2, GetSPR() + 1, GetSTR(), GetCON() + 2, GetAGI());
		else if(GetCclass() == 4)
			SetAttrib1(GetSTA() + 1, GetSPR(), GetSTR() + 4, GetCON(), GetAGI());
	}
	else
	{
		if (GetLevel() > CHAR_LEVEL_LIMIT2)
		{
			SetAttrPoint(GetAttrPoint() + AP_PER_LEVEL3);
		}
		else if (GetLevel() > CHAR_LEVEL_LIMIT)
		{
			SetAttrPoint(GetAttrPoint() + AP_PER_LEVEL2);
		}
		else
		{
			SetAttrPoint(GetAttrPoint() + AP_PER_LEVEL);
		}
	}
	SetAttrib1(GetSTA() + 1, GetSPR() + 1, GetSTR() + 1, GetCON() + 1, GetAGI() + 1);
	CalcNextLevel();
	CalcAttrib();
	CalMaxVP();
	SetHP(GetMaxHP(), GetMaxHP(), GetMaxHP());
	SetSP(GetMaxSP(), GetMaxSP(), GetMaxSP());
	RaiseUpdate();
	if (m_pMap)
		RaiseUniqEventID(EVT_MSGDATA_UPDATE);

	CB_LevelUp(pPeer, context);

	if (m_pQuestSys) {
		m_pQuestSys->CheckAllHints();
		m_pQuestSys->CheckJournals();
	}
	
	if (GetLevel() -1 < 50) {
		CalcGenerationExpCont(uiNextLvExp);
		CalcGenerationAccCont();
	}

	if (GetLevel() == 50) {
		ChangeGenerationType();
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::RaiseAttrib(LPCPEER pPeer, RPCContext &context, const UInt16 &sta, 
								  const UInt16 &spr, const UInt16 &str, const UInt16 &con, 
								  const UInt16 &agi)
{
	if (IsState(ST_BATTLE))
	{
		CB_Error(pPeer, context, 1, _T("Character RaiseAttrib Error: (cannot raise attribute inside battle)"));
		return (RPC_RESULT_FAIL);
	}

	UInt16 totalAp = sta + spr + str + con + agi;
	if (totalAp == 0)
	{
		CB_Error(pPeer, context, 1, _T("CCharacter RaiseAttrib Error: empty attribute points assigned"));
		return RPC_RESULT_FAIL;
	}
	else if (totalAp > GetAttrPoint())
	{
		CB_Error(pPeer, context, 1, _T("CCharacter RaiseAttrib Error: not enough attribute points"));
		return RPC_RESULT_FAIL;
	}

	SetAttrib1(GetSTA() + sta, GetSPR() + spr, GetSTR() + str,
		GetCON() + con, GetAGI() + agi);
	SetAttrPoint(GetAttrPoint() - totalAp);
	CalcAttrib();
	if (spr > 0)
		SetSP(GetSP(), GetMaxSP(), GetMaxSP());
	
	RaiseUpdate();

	if (m_pMap)
		RaiseUniqEventID(EVT_MSGDATA_UPDATE);

	CB_RaiseAttrib(pPeer, context);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::UpdateShortCutList(LPCPEER pPeer, RPCContext &context, 
										 const UInt16Vector &shortcut, 
										 const UInt8Vector &shortcuttype)
{
	if (shortcut.size() != 10 || shortcuttype.size() != 10)
	{
		CB_Error(pPeer, context, 1, _T("Character UpdateShortCutList Error: (shortcut size is not correct)"));
		return (RPC_RESULT_FAIL);
	}

	SetShortcut(shortcut[0], shortcut[1], shortcut[2], shortcut[3], shortcut[4], 
				shortcut[5], shortcut[6], shortcut[7], shortcut[8], shortcut[9],
				shortcuttype[0], shortcuttype[1], shortcuttype[2], shortcuttype[3],
				shortcuttype[4], shortcuttype[5], shortcuttype[6], shortcuttype[7], 
				shortcuttype[8], shortcuttype[9]);

	CB_UpdateShortCutSuccess(pPeer, context);
	RaiseUpdate();

	return RPC_RESULT_OK;
}

RPCResult CCharacter::UpdateShortCut(LPCPEER pPeer, RPCContext &context, 
									 const UInt16 &shortcut, const UInt8 &shortcuttype, 
									 const UInt8 &position)
{
	if (position > 9)
	{
		CB_Error(pPeer, context, 1, _T("Character UpdateShortCut Error: (position cannot larger than 9)"));
		return (RPC_RESULT_FAIL);
	}
	
	SetShortcut(position, shortcut);
	SetShortcuttype(position, shortcuttype);
	CB_UpdateShortCutSuccess(pPeer, context);
	RaiseUpdate();

	return RPC_RESULT_OK;
}

RPCResult CCharacter::SaveSystemSetting(LPCPEER pPeer, RPCContext &context, 
										const Boolean &EnableBlockGiven, 
										const Boolean &EnableBlockParty, 
										const Boolean &EnableBlockMsg, 
										const Boolean &EnableBlockTrading, 
										const Boolean &EnableBlockMakeFriend,
										const Boolean &EnableShowEquip)
{
	SetSettingInfo(EnableBlockGiven, EnableBlockParty, EnableBlockMsg, EnableBlockTrading, EnableBlockMakeFriend, EnableShowEquip);

	if (m_pMap)
		RaiseUniqEventID(EVT_MSGDATA_UPDATE);
	CB_SaveSystemSetting(pPeer, context);
	return RPC_RESULT_OK;
} 

void CCharacter::SetConcernMask(CMapActor *actor, RPCMask &mask)
{
	m_viewManager->SetConcernMask(actor, mask);
}

void CCharacter::ShowRumor(PCSTRINGA msg, Boolean bTitle)
{
	String s;
	UTF8ToStr(msg, s);

	MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer,
		m_context, s, GetChar_id(), (BYTE) bTitle);
}

void CCharacter::ShowRank(UInt8 rankType)
{
	WorldService_Proxy::GetCharacterRank(gGlobal.pWorldService->pPeer, 
		m_context, rankType);
}

void CCharacter::ShowGuildRank()
{
	WorldService_Proxy::GetGuildRank(gGlobal.pWorldService->pPeer, 
		m_context);
}

void CCharacter::ChangeSTA(Int change)
{
	SetSTA(MAX((Int) GetSTA() + change, 0));
}

void CCharacter::ChangeSPR(Int change)
{
	SetSPR(MAX((Int) GetSPR() + change, 0));
}

void CCharacter::ChangeSTR(Int change)
{
	SetSTR(MAX((Int) GetSTR() + change, 0));
}

void CCharacter::ChangeCON(Int change)
{
	SetCON(MAX((Int) GetCON() + change, 0));
}

void CCharacter::ChangeAGI(Int change)
{
	SetAGI(MAX((Int) GetAGI() + change, 0));
}

void CCharacter::ChangeATK(Int change)
{
	SetATK(MAX((Int) GetATK() + change, 0));
}

void CCharacter::ChangeDEF(Int change)
{
	SetDEF(MAX((Int) GetDEF() + change, 0));
}

void CCharacter::ChangeHIT(Int change)
{
	SetHIT(MAX((Int) GetHIT() + change, 0));
}

void CCharacter::ChangeEVA(Int change)
{
	SetEVA(MAX((Int) GetEVA() + change, 0));
}

void CCharacter::ChangePOW(Int change)
{
	SetPOW(MAX((Int) GetPOW() + change, 0));
}

void CCharacter::ChangeSPD(Int change)
{
	SetSPD(MAX((Int) GetSPD() + change, 0));
}

void CCharacter::ChangePvPScoreSingle(Int32 change)
{
	SetPvPScoreSingle(MAX((Int) GetPvPScoreSingle() + change, 0));
}
void CCharacter::ChangeHP(Int change)
{
	SetHP(WITHIN((Int) GetHP() + change, 1, GetMHP()));
}

void CCharacter::ChangeMHP(Int change)
{
	SetMHP(WITHIN((Int) GetMHP() + change, 1, GetMaxHP()));
	if (GetHP() > GetMHP())
		SetHP(GetMHP());
}

void CCharacter::ChangeMaxHP(Int change)
{
	SetMaxHP(MAX((Int) GetMaxHP() + change, 1));
	if (GetHP() > GetMaxHP())
		SetHP(GetMaxHP());
	if (GetMHP() > GetMaxHP())
		SetMHP(GetMaxHP());
}

void CCharacter::ChangeSP(Int change)
{
	SetSP(WITHIN((Int) GetSP() + change, 0, GetMSP()));
}

void CCharacter::ChangeMSP(Int change)
{
	SetMSP(WITHIN((Int) GetMSP() + change, 0, GetMaxSP()));
	if (GetSP() > GetMSP())
		SetSP(GetMSP());
}

void CCharacter::ChangeMaxSP(Int change)
{
	SetMaxSP(MAX((Int) GetMaxSP() + change, 1));
	if (GetSP() > GetMaxSP())
		SetSP(GetMaxSP());
	if (GetMSP() > GetMaxSP())
		SetMSP(GetMaxSP());
}

void CCharacter::ChangeDP(Int change)
{
	SetDP(WITHIN((Int) GetDP() + change, 0, GetMDP()));
}

void CCharacter::ChangeMDP(Int change)
{
	SetMDP(WITHIN((Int) GetMDP() + change, 0, GetMaxDP()));
}

void CCharacter::ChangeMaxDP(Int change)
{
	SetMaxDP(MAX((Int) GetMaxDP() + change, 1));
}

void CCharacter::ChangeWC(Int change)
{
	SetWC(WITHIN((Int) GetWC() + change, 0, GetMaxWC()));
}

void CCharacter::ChangeMaxWC(Int change)
{
	SetMaxWC(MAX((Int) GetMaxWC() + change, 0));
}

void CCharacter::ChangeWcExp(Int change)
{

	SetWcExp(MAX((Int) GetWcExp() + change, 0));
	if (GetWcExp() >= GetNextWCExp())
	{
		WCLevelUp();
		SetWcExp(0);
	}

}

void CCharacter::ChangeVP(Int change)
{	
	if (m_pAntiGameAddict && (m_pAntiGameAddict->m_playerInterest < 1) && (change > 0))
	{
        change = (Int) (change * m_pAntiGameAddict->m_playerInterest);	
		UInt8 interest = (m_pAntiGameAddict->m_playerInterest == 0.5)? 1 : 0;
		CB_PlyInterestDwn(m_pPeer, m_context, interest);
	}
	SetVP(WITHIN((Int) GetVP() + change, 0, GetMaxVP()));
	
}

void CCharacter::ChangeMaxVP(Int change)
{
	SetMaxVP(MAX((Int) GetMaxVP() + change, 0));
}

void CCharacter::ChangeFDonate(Int change)
{
	if (m_pAntiGameAddict && (m_pAntiGameAddict->m_playerInterest < 1) && (change > 0))
	{
        change = (Int) (change * m_pAntiGameAddict->m_playerInterest);
		UInt8 interest = (m_pAntiGameAddict->m_playerInterest == 0.5)? 1 : 0;
		CB_PlyInterestDwn(m_pPeer, m_context, interest);
	}	
	SetFDonate(MAX((Int) GetFDonate() + change, 0));
	SetTotalFDonate(MAX((Int) GetTotalFDonate() + change, 0));

}

void CCharacter::ChangeGDonate(Int change, UInt8 source)
{
	MessageService_Proxy::ChangeGDonate(gGlobal.pMessageService->pPeer, m_context,
		GetGuildUid(), GetChar_id(), change, source);
}

UInt32 CCharacter::GetGDonate()
{
	if (m_ownGuildMember)
		return m_ownGuildMember->GetDonate();
	return 0;
}

UInt32 CCharacter::GetGTotalDonate()
{
	if (m_ownGuildMember)
		return m_ownGuildMember->GetTotalDonate();
	return 0;
}
void CCharacter::ChangeMDonate(Int change)
{
	if (m_pAntiGameAddict && (m_pAntiGameAddict->m_playerInterest < 1) && (change > 0))
	{
        change = (Int) (change * m_pAntiGameAddict->m_playerInterest);	
		UInt8 interest = (m_pAntiGameAddict->m_playerInterest == 0.5)? 1 : 0;
		CB_PlyInterestDwn(m_pPeer, m_context, interest);
	}
	SetMDonate(MAX((Int) GetMDonate() + change, 0));
	SetTotalMDonate(MAX((Int) GetTotalMDonate() + change, 0));
	
}

void CCharacter::ChangeSpeakCount(Int16 change)
{
	MessageService_Proxy::AddSpeakCount(gGlobal.pMessageService->pPeer, m_context,
		GetChar_id(), change);
	SetSpeakCount(MAX((Int) GetSpeakCount() + change, 0));
}

void CCharacter::ChangeLevel(Int16 change)
{
	SetLevel(MAX((Int) GetLevel() + change, 1));
	if (m_pMap)
		RaiseUniqEventID(EVT_MSGDATA_UPDATE);
}

void CCharacter::ChangeExp(Int32 change, Int32 exp_type)
{
	if (change > 0)
	{
		if (m_pAntiGameAddict && (m_pAntiGameAddict->m_playerInterest < 1))
		{
			change = (Int) (change * m_pAntiGameAddict->m_playerInterest);	
			UInt8 interest = (m_pAntiGameAddict->m_playerInterest == 0.5)? 1 : 0;
			CB_PlyInterestDwn(m_pPeer, m_context, interest);
		}

		UInt32 maxExp = GetNextLvExp() * gConfigure.expBindFactor;
		if ((GetExp() + change) > maxExp)
		{
			if (GetExp() < maxExp)
				change = maxExp - GetExp();
			else
				change = 0;

			CB_WarningMessage(GetPeer(), GetContext(), WARN_CHARACTER_OVERFLOW_EXP, change, WARN_SHOW_IN_ALL);
		}
	}


	SetExp(MAX((Int32) GetExp() + change, 0));

	//log the player change
	if (exp_type != LogService_Proxy::Money_And_Exp::SOURCE_UNKNOWN)
		LogService_Proxy::LogCharExp(gGlobal.pLogService->pPeer, m_context, exp_type, change, GetAcct_id(), GetChar_id(), GetExp(), GetLevel());
}

void CCharacter::ChangeMoney(Int32 change, Int32 src_type)
{
	if (m_pAntiGameAddict && (m_pAntiGameAddict->m_playerInterest < 1) && (change > 0))
	{
        change = (Int) (change * m_pAntiGameAddict->m_playerInterest);
		UInt8 interest = (m_pAntiGameAddict->m_playerInterest == 0.5)? 1 : 0;
		CB_PlyInterestDwn(m_pPeer, m_context, interest);	
	}
	if (src_type != LogService_Proxy::Money_And_Exp::SOURCE_UNKNOWN)
		LogService_Proxy::LogMoney(gGlobal.pLogService->pPeer, m_context, GetAcct_id(),
			GetChar_id(), src_type, change, GetMoney() + GetBankMoney());
	SetMoney(MAX((Int32) GetMoney() + change, 0));

}

void CCharacter::SetMoney(const UInt32 &money)
{
	if (GetMoney() == money)
		return;

	Parent::SetMoney(money);
	MessageService_Proxy::SetMoney(gGlobal.pMessageService->pPeer, m_context, 
		GetChar_id(), money);
}

void CCharacter::ChangeSMoney(Int32 change, Int32 src_type)
{
	if (m_pAntiGameAddict && (m_pAntiGameAddict->m_playerInterest < 1) && (change > 0))
	{
        change = (Int) (change * m_pAntiGameAddict->m_playerInterest);	
		UInt8 interest = (m_pAntiGameAddict->m_playerInterest == 0.5)? 1 : 0;
		CB_PlyInterestDwn(m_pPeer, m_context, interest);
	}	
	if (src_type != LogService_Proxy::Money_And_Exp::SOURCE_UNKNOWN)
		LogService_Proxy::LogSMoney(gGlobal.pLogService->pPeer, m_context, GetAcct_id(),
			GetChar_id(), src_type, change);
	SetSMoney(MAX((Int32) GetSMoney() + change, 0));

}

void CCharacter::ChangeBankMoney(Int32 change, Int32 src_type)
{
	if (src_type != LogService_Proxy::Money_And_Exp::SOURCE_UNKNOWN )
		LogService_Proxy::LogMoney(gGlobal.pLogService->pPeer, m_context, GetAcct_id(), GetChar_id(), src_type, change, GetMoney() + GetBankMoney());
	SetBankMoney(MAX((Int32) GetBankMoney() + change, 0));
}

void CCharacter::ChangeHonor(Int32 change)
{
	if (m_pAntiGameAddict && (m_pAntiGameAddict->m_playerInterest < 1) && (change > 0))
	{
        change = (Int) (change * m_pAntiGameAddict->m_playerInterest);
		UInt8 interest = (m_pAntiGameAddict->m_playerInterest == 0.5) ? 1 : 0;
		CB_PlyInterestDwn(m_pPeer, m_context, interest);
	}
	LogService_Proxy::LogHonor(gGlobal.pLogService->pPeer, m_context, GetChar_id(), change);
	SetHonor(MAX((Int32) GetHonor() + change, 0));
}

void CCharacter::ChangeGoodness(Int32 change, Int32 exp_type)
{
	if (change == 0)
		return;

	if (exp_type != LogService_Proxy::Money_And_Exp::SOURCE_UNKNOWN)
	LogService_Proxy::LogGoodness(gGlobal.pLogService->pPeer, m_context, GetAcct_id(),
		GetChar_id(), 0, change);
	SetGoodness(GetGoodness() + change);
}

void CCharacter::SetFull(bool HP, bool SP, bool DP, bool VP)
{
	if (HP) {
		SetHP(GetMaxHP());
		SetMHP(GetMaxHP());
	}
	if (SP) {
		SetSP(GetMaxSP());
		SetMSP(GetMaxSP());
	}
	if (DP) {
		SetDP(GetMaxDP());
		SetMDP(GetMaxDP());
	}
	if (VP) {
		SetVP(GetMaxVP());
	}
}

void CCharacter::OpenInterface(String menu, Int32 param1)
{
	m_tempSkillShopID = (UInt16)param1;
	if (m_pScriptSys) {
		StringA menuA(menu.c_str());
		m_pScriptSys->CB_OpenInterface(m_pPeer, m_context, menuA, param1);
	}
}

void CCharacter::OpenHawkerOwnerMenu()
{
	if (m_pPeer && m_pMap)
	{
		if (m_pScriptSys && GetLevel() >= 20 && m_pMap->IsHawkZone(GetCurPosX(), GetCurPosY()) && 
			!m_pMap->IsHawkOccupied(GetCurPosX(), GetCurPosY()) && m_pParty == NULL) 
		{	
			
			PROCEDURE_DO_METHOD(CCharacter, GetHawkHistoryGroup, this, m_context)			

			StringA menuA = "HawkerOwnerMenu";
			m_pScriptSys->CB_OpenInterface(m_pPeer, m_context, menuA, 0);
		}
		else
		{
			if (GetLevel() < 20)
				CB_HawkMessage(m_pPeer, m_context, HAWK_NOT_ENOUGH_LV);
			else if (!m_pMap->IsHawkZone(GetCurPosX(), GetCurPosY() ))
				CB_HawkMessage(m_pPeer, m_context, HAWK_NOT_RIGHT_PLACE);
			else if (m_pMap->IsHawkOccupied(GetCurPosX(), GetCurPosY()))
				CB_HawkMessage(m_pPeer, m_context, HAWK_PLACE_HAVE_HAWKER);
			else if (m_pParty)
				CB_HawkMessage(m_pPeer, m_context, HAWK_CANT_HAVE_PARTY);
		}
	}
}

bool CCharacter::IsHawking()
{
	if (GetAction() == ST_HAWK && m_pHawkGroup)
		return true;
	return false;
}

void CCharacter::SetFaction(UInt8 faction)
{
	if (faction > 0)
	{
		String channelName;
		channelName.Format(_T("%s_%d"), ChannelPrefix[CHANNELTYPE_GANG], faction);
		MessageService_Proxy::JoinMessageChannel(gGlobal.pMessageService->pPeer, 
					m_context, GetChar_id(), GetNickName(), channelName, CHANNEL_JOIN_MODE_CREATE_IF_NOT_EXIST );
	}
	if (GetFaction() > 0 && GetFaction() != faction)
	{
		String channelName;
		channelName.Format(_T("%s_%d"), ChannelPrefix[CHANNELTYPE_GANG], GetFaction());
		MessageService_Proxy::LeaveChannel(gGlobal.pMessageService->pPeer, 
					m_context, GetChar_id(), channelName );
	}

	Parent::SetFaction(faction);
	if (m_pQuestSys) {
		m_pQuestSys->CheckAllHints();
		m_pQuestSys->CheckJournals();
	}
}

void CCharacter::StopScript()
{
	if (m_pScriptSys->IsRunning())
		m_pScriptSys->StopScript();
}

void CCharacter::AddAvailBackPack(UInt8 numOfAvail)
{
	if (GetAvailBackPack() < MAX_AVAIL_BACKPACK)
	{
		if ( (numOfAvail + GetAvailBackPack()) <= MAX_AVAIL_BACKPACK)
			SetAvailBackPack(GetAvailBackPack() + numOfAvail);
		else
			SetAvailBackPack(MAX_AVAIL_BACKPACK);
	}
}

void CCharacter::AddAvailItemWareHouse(UInt8 numOfAvail)
{
	if (GetAvailItemWareHouse() < BAG_MAX_WAREHOUSE)
	{
		if ((numOfAvail + GetAvailItemWareHouse()) <= BAG_MAX_WAREHOUSE)
			SetAvailItemWareHouse(numOfAvail + GetAvailItemWareHouse());
		else
			SetAvailItemWareHouse(BAG_MAX_WAREHOUSE);
	}
}
	
void CCharacter::AddAvailPartnerWareHouse(UInt8 numOfAvail)
{
	if (GetAvailPartnerWareHouse() < PARTNER_MAX_WAREHOUSE)
	{
		if ((numOfAvail + GetAvailPartnerWareHouse()) <= PARTNER_MAX_WAREHOUSE)
			SetAvailPartnerWareHouse(numOfAvail + GetAvailPartnerWareHouse());
		else
			SetAvailPartnerWareHouse(PARTNER_MAX_WAREHOUSE);
	}
}

void CCharacter::AddAvailPetWareHouse(UInt8 numOfAvail)
{
	if (GetAvailPetWareHouse() < PET_MAX_WAREHOUSE)
	{
		if ((numOfAvail + GetAvailPetWareHouse()) <= PET_MAX_WAREHOUSE)
			SetAvailPetWareHouse(numOfAvail + GetAvailPetWareHouse());
		else
			SetAvailPetWareHouse(PET_MAX_WAREHOUSE);
	}
}

bool CCharacter::CanCheat(UInt32 acct_id, PCSTRING cheatCode)
{
	if (m_pPeer == NULL)
	{
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("peer null, cheat failed, cheater_acct %d, target_acct_id %d, target_char_id %d"), 
			acct_id, GetAcct_id(), GetChar_id());
		return false;
	}

	if (IsHawking() && 
		(!cheatCode || cheatCode && _tcsicmp(cheatCode, _T("kick")) != 0 && _tcsicmp(cheatCode, _T("speakBlock")) != 0
		&& _tcsicmp(cheatCode, _T("superkick")) != 0))
	{
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("target is hawking, cheat failed, cheater_acct %d, target_acct_id %d, target_char_id %d"), 
			acct_id, GetAcct_id(), GetChar_id());
		return false;
	}

	bool bCheat = CMapCheat::IsGameMaster(acct_id ? acct_id : GetAcct_id());
	if (!bCheat)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, 
			_FW("[character is not GM], acct_id %d, char_id %d"), 
			acct_id ? acct_id : GetAcct_id(), GetChar_id());
	}

	return bCheat;
}

RPCResult CCharacter::CheatCode(LPCPEER pPeer, RPCContext &context, 
								const UInt32& orderMakerAcct_id, const String &code)
{
	CMapCheat::DoCheat(this, pPeer, context, orderMakerAcct_id, code);

	return RPC_RESULT_OK;
}

void CCharacter::RemoveTitle(UInt16 title_id)
{
	TitleMap::iterator Itr = m_titleDataLst.find(title_id);
	if (Itr != m_titleDataLst.end())
	{
		const TitleData * titleDataCSV = gGlobal.m_resource->GetTitleData(title_id);
		TitleInfo* pTitle = Itr->second;
		if (titleDataCSV && pTitle)
		{
			if (titleDataCSV->Title_Type != TITLE_TYPE_DB_ADD && titleDataCSV->Title_Type != TITLE_TYPE_FORCE_TOP)
			{
				if (pTitle->title_name == GetTitle())
					SetTitle(String(), 0, true);
				pTitle->status = TITLE_STATE_REMOVE;
				RaiseUniqEventID(EVT_UPDATE_TITLE);
			}
			else
			{
				PROCEDURE_DO_METHOD1(CCharacter, RemoveTitle, this, GetContext(), 
					UInt32, pTitle->title_uid)
			}
		}
	}
}

RPCResult CCharacter::RemoveTitle(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CCharacter, RemoveTitle, this, context, UInt32, title_uid)

		ObjectService_Proxy::DeleteTitle(gGlobal.pObjectService->pPeer, context, title_uid);

	PROCEDURE_WAIT(1, ObjectService, CB_DeleteTitle)
	
		for (TitleMap::iterator Itr = m_titleDataLst.begin();
			Itr != m_titleDataLst.end(); ++Itr)
		{
			TitleInfo* pTitle = Itr->second;
			if (pTitle && pTitle->title_uid == title_uid)
			{
				if (pTitle->title_name == GetTitle())
					SetTitle(String(), 0, true);
				pTitle->status = TITLE_STATE_REMOVE;
				RaiseUniqEventID(EVT_UPDATE_TITLE);
				CheckTitle();
				break;
			}
		}


	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("RemoveTitle Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CCharacter::CheckTitle()
{
	TRACE_INFODTL_1(GLOBAL_LOGGER,_F("CCharacter::CheckTitle pre:%s"),GetTitle().c_str());
	for (TitleMap::iterator Itr = m_titleDataLst.begin();
		Itr != m_titleDataLst.end(); ++Itr)
	{
		TitleInfo* pTitle = Itr->second;
		if (pTitle && pTitle->IsExist() && pTitle->title_name == GetTitle())
		{
			SetTitle(GetTitle(), Itr->second->title_id);
			return;
		}
	}
	SetTitle(String());

}

void CCharacter::CalTitleExpire()
{
	CancelUniqEvent(EVT_TITLE_EXPIRE);

	UInt32 nextExpireTime = 0xFFFFFFFF;
	bool found = false;
	time_t now;
	time(&now);

	StlVector<UInt16> rmID;
	for (TitleMap::iterator Itr = m_titleDataLst.begin();
		Itr != m_titleDataLst.end(); ++Itr)
	{
		TitleInfo* pTitle = Itr->second;
		if (pTitle && pTitle->IsExist() && pTitle->expireDate != 0)
		{
			if ((UInt32)now >= Itr->second->expireDate)
			{
				rmID.push_back(Itr->second->title_id);
			}
			else if ((Itr->second->expireDate - (UInt32)now) * 1000 < nextExpireTime)
			{
				nextExpireTime = (Itr->second->expireDate - (UInt32)now) * 1000;
				found = true;
			}
		}
	}

	for (UInt i = 0; i < rmID.size(); i ++)
		RemoveTitle(rmID[i]);

	if (found)
		RaiseUniqEventID(EVT_TITLE_EXPIRE, nextExpireTime);
}

Boolean CCharacter::HasTitle(UInt16 title_id)
{
	TitleMap::iterator it = m_titleDataLst.find(title_id);
	return (it != m_titleDataLst.end() && it->second && it->second->IsExist());
}

const String& CCharacter::GetTitle(UInt16 title_id)
{
	static String emptyStr;

	TitleMap::iterator it = m_titleDataLst.find(title_id);
	if (it != m_titleDataLst.end() && it->second && it->second->IsExist())
		return it->second->title_name;
	else
		return emptyStr;
}

void CCharacter::AddTitle(UInt16 title_id)
{
	TitleMap::iterator Itr = m_titleDataLst.find(title_id);
	if (Itr != m_titleDataLst.end())
	{
		if (Itr->second->status == TITLE_STATE_REMOVE)
			Itr->second->status = TITLE_STATE_ADD;
		return;
	}

	const TitleData * titleDataCSV = gGlobal.m_resource->GetTitleData(title_id);
	if (titleDataCSV == NULL)
		return;

	if (titleDataCSV->Title_Type != TITLE_TYPE_DB_ADD && titleDataCSV->Title_Type != TITLE_TYPE_FORCE_TOP)
	{
		if (titleDataCSV->Title_Type == TITLE_TYPE_RELATION && titleDataCSV->Title_PreNameType == TITLE_PRENAME_TYPE_MASTER)
		{
			PROCEDURE_DO_METHOD1(CCharacter, AddTitleMaster, this, GetContext(), UInt16, title_id)
			return;
		}

		if (titleDataCSV->Title_Type == TITLE_TYPE_RELATION && titleDataCSV->Title_PreNameType == TITLE_PRENAME_TYPE_HUSBAND)
		{
			PROCEDURE_DO_METHOD1(CCharacter, AddTitleHusband, this, GetContext(), UInt16, title_id)
			return;
		}

		if (titleDataCSV->Title_Type == TITLE_TYPE_RELATION && titleDataCSV->Title_PreNameType == TITLE_PRENAME_TYPE_WIFE)
		{
			PROCEDURE_DO_METHOD1(CCharacter, AddTitleWife, this, GetContext(), UInt16, title_id)
			return;
		}

		String text;
		if (titleDataCSV->Title_Type == TITLE_TYPE_GUILD && m_ownGuild)
			text.Format(_T("%s%s"), m_ownGuild->GetName().c_str(), titleDataCSV->Title_Name);
		else
			text = titleDataCSV->Title_Name;
		AddTitle(title_id, 0, 0, text);
	}
	else
	{
		PROCEDURE_DO_METHOD1(CCharacter, AddTitle, this, GetContext(), 
			UInt16, title_id)
	}
}

void CCharacter::AddTitle(UInt16 title_id, UInt32 title_uid, UInt32 expireData, const String &name)
{
	TitleMap::iterator Itr = m_titleDataLst.find(title_id);
	if (Itr != m_titleDataLst.end())
	{
		if (Itr->second->status == TITLE_STATE_REMOVE)
		{
			Itr->second->status = TITLE_STATE_UPDATE;
			Itr->second->title_uid = title_uid;
			Itr->second->title_id = title_id;
			Itr->second->expireDate = expireData;
			Itr->second->title_name = name;
		}
		return;
	}	

	TitleInfo * pMapTitleData = SafeCreateObject(CCharacter::TitleInfo);
	pMapTitleData->title_uid = title_uid;
	pMapTitleData->title_id = title_id;
	pMapTitleData->expireDate = expireData;
	pMapTitleData->title_name = name;
	pMapTitleData->status = TITLE_STATE_ADD;

	m_titleDataLst.insert(std::make_pair(title_id, pMapTitleData));
	RaiseUniqEventID(EVT_UPDATE_TITLE);
}

RPCResult CCharacter::AddTitleMaster(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CCharacter, AddTitleMaster, this, context, UInt16, title_id)

	MessageService_Proxy::GetMessengersName(gGlobal.pMessageService->pPeer,context, GetChar_id(), FRIEND_RELATION_MASTER);
	PROCEDURE_WAIT2(1, MessageService, CB_GetMessengersName, UInt32, owner_id,StringVector, messagers)

	const TitleData* titleDataCSV = gGlobal.m_resource->GetTitleData(title_id);
	if (titleDataCSV && messagers.size() == 1)
	{
		String text;
		text.Format(_T("%s%s"), messagers[0].c_str(), titleDataCSV->Title_Name);
		AddTitle(title_id, 0, 0, text);
		SetTitle(text,title_id);
	}

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(MessageService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("AddTitleMaster Error: MessageService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CCharacter::RemoveGuildTitle()
{
	UInt16Vector rmList;
	TitleMap::iterator Itr = m_titleDataLst.begin();

	while (Itr != m_titleDataLst.end()) 
	{
		const TitleData * titleDataCSV = gGlobal.m_resource->GetTitleData(Itr->second->title_id);
		if (titleDataCSV && titleDataCSV->Title_Type == TITLE_TYPE_GUILD)
			rmList.push_back(Itr->second->title_id);
		++Itr;
	}

	for (UInt i = 0; i < rmList.size(); i ++)
		RemoveTitle(rmList[i]);
}

RPCResult CCharacter::AddTitle(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CPetGroup, AddTitle, this, context, UInt16, title_id)

		ObjectService_Proxy::CreateTitle(gGlobal.pObjectService->pPeer, context, GetChar_id(), title_id);

	PROCEDURE_WAIT1(1, ObjectService, CB_CreateTitle, NetObject*, title)
	
	CTitleData * pTitleData = CastNetObject<CTitleData>(title);

	if (pTitleData)
	{
		const TitleData * titleDataCSV = gGlobal.m_resource->GetTitleData(pTitleData->GetTitle_id());
		String text;
		if (titleDataCSV)
		{
			text = titleDataCSV->Title_Name;
			SetTitle(text, titleDataCSV->Title_id);		//change the name imm
		}

		AddTitle(pTitleData->GetTitle_id(), pTitleData->GetTitle_uid(), 
			pTitleData->GetExpireDate(), text);

		if (pTitleData->GetExpireDate() != 0)
			CalTitleExpire();
	}

	DeleteNetObject(title);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("AddPet Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CCharacter::UpdateTitle()
{
	// note: only called by HandleEvent(), shouldn't call directly
	UInt16Vector title_ids;
	StringVector labels; 
	UInt8Vector states;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_TitleListUpdate char_id: %d"), GetChar_id());

	for (TitleMap::iterator it = m_titleDataLst.begin(); it != m_titleDataLst.end();)
	{
		TitleInfo* pTitle = it->second;
		if (pTitle == NULL || pTitle->status == TITLE_STATE_NORMAL)
		{
			++it;
			continue;
		}
		title_ids.push_back(pTitle->title_id);
		labels.push_back(pTitle->status == TITLE_STATE_REMOVE ? _T("") : pTitle->title_name);
		states.push_back(pTitle->status);
		if (pTitle->status == TITLE_STATE_REMOVE)
		{
			SafeDeleteObject(pTitle);
			it = m_titleDataLst.erase(it);
		}
		else
		{
			pTitle->status = TITLE_STATE_NORMAL;
			++it;
		}
	}

	if (!title_ids.empty())
		CB_TitleListUpdate(m_pPeer, m_context, title_ids, labels, states);
}

void CCharacter::ChangeBaseSpeed(UInt16 speed)
{
	m_baseSpeed = speed;
	UpdateSpeed();
}

bool CCharacter::CheckProtectedPK()
{
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_ProtectPK)
		{
			return true;
		}
	}
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_ProtectPK)
		{
			return true;
		}
	}
	return false;
}

bool CCharacter::CheckBlockPK()
{
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_BlockPK)
		{
			return true;
		}
	}
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_BlockPK)
		{
			return true;
		}
	}
	return false;
}

bool CCharacter::CheckBlockItem(UInt8 itemType)
{
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_BlockItemType.size() > 0)
		{
			for (Int i = 0; i < (Int)Itr->second.pBuffData->Buff_BlockItemType.size(); i++)
			{
				if (Itr->second.pBuffData->Buff_BlockItemType[i] == itemType)
					return true;
			}
		}
	}
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_BlockItemType.size() > 0)
		{
			for (Int i = 0; i < (Int)Itr->second.pBuffData->Buff_BlockItemType.size(); i++)
			{
				if (Itr->second.pBuffData->Buff_BlockItemType[i] == itemType)
					return true;
			}
		}
	}
	return false;
}

RPCResult CCharacter::ChangeMsgIcon(LPCPEER pPeer, RPCContext &context, const UInt16 &iconID)
{
	PROCEDURE_START_RPC1(Character, ChangeMsgIcon, this, UInt16, iconID)

		//PROCEDURE_WAIT_TIMEOUT(1, 500) //delay 0.5 second (wrong delay statement)

		if (GetYuanbaoLock())
		{
			CB_WarningMessage(pPeer, context, WARN_CHAT_CHANGE_ICON_YUANBAO_LOCK, 0, WARN_SHOW_IN_CHATBOX);	
			PROCEDURE_RETURN	
		}
		MessageService_Proxy::ChangeMsgIcon(gGlobal.pMessageService->pPeer, 
			context, GetChar_id(), iconID, BOOLEAN_FALSE);

	PROCEDURE_WAIT1(2, MessageService, CB_ChangeMsgIcon, Boolean, canChange)

		if (canChange)
		{
			ObjectService_Proxy::UpdateYuanBao(gGlobal.pObjectService->pPeer, context, GetAcct_id(), -980);			// 980 yuan bao, bad hard code
		}
		else
		{
			PROCEDURE_RETURN
		}

	PROCEDURE_WAIT2(3, ObjectService, CB_UpdateYuanBao, UInt32, yuanbao, Boolean, buySuccess)

		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("ChangeMsgIconInChangeYuanBao: sessionId %s, acct_id %d, char_id %d, exist yuanbao %d, updated yuanbao %d"), 
			context.sessionId.GetString().c_str(), GetAcct_id(), GetChar_id(), GetYuanBao(), yuanbao);
		SetYuanBao(yuanbao);
		if (buySuccess)
		{
			MessageService_Proxy::ChangeMsgIcon(gGlobal.pMessageService->pPeer, 
				context, GetChar_id(), iconID, BOOLEAN_TRUE);
			LogService_Proxy::LogYuanBao(gGlobal.pLogService->pPeer, m_context, 
				GetAcct_id(), GetChar_id(),
				LogService_Proxy::YuanBao::USEIN_CHANGECHATFACE ,980);
		}
		else
			CB_WarningMessage(pPeer, context, WARN_CHAT_CHANGE_ICON_NO_YUANBAO, 0, WARN_SHOW_IN_CHATBOX);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(MessageService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("ChangeMsgIcon Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

		PROCEDURE_EXCEPTION_DEFAULT


	PROCEDURE_END_CATCH
	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::ChangeTitle(LPCPEER pPeer, RPCContext &context, const UInt16 &title_id)
{
	if (m_pMap && m_pMap->GetMapInfo() && m_pMap->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR)
	{
		return CB_Error(m_pPeer, m_context, 0, _T("MSG_GUILDWAR_TITLE_CANNOT_CHANGE"));
	}
	if (title_id == 0)
	{
		SetTitle(String(), title_id);
	}
	else
	{		
		TitleMap::iterator Itr = m_titleDataLst.find(title_id);
		if (Itr != m_titleDataLst.end())
		{
			SetTitle(Itr->second->title_name, title_id);
		}
	}
	return RPC_RESULT_OK;
}


RPCResult CCharacter::TalkToNPC(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_uid)
{
	Boolean bSuccess(BOOLEAN_FALSE);
	if (m_pMap && m_pScriptSys && IsLeader() && !IsHawking() && !IsState(ST_TALK))
	{   
		CMapNPC* pNpc = CastNetObject<CMapNPC>(m_pMap->FindMapActor(MapNPC_CLASSID, npc_uid));
		// check if can talk
		if (pNpc && pNpc->CanTalk(this) && IsInMap() && Stop() && TryState(ST_TALK)) {
			ChangeState(ST_TALK);
			bSuccess = m_pScriptSys->ExecuteNpcScript(pNpc);
			if (!bSuccess)
				ChangeState(ST_STOP);
		}
	}
	if (!bSuccess)
		CB_TalkToNPC(pPeer, context, npc_uid, BOOLEAN_FALSE);

	return RPC_RESULT_OK;
}

void CCharacter::SkillFail(PCSTRINGA Text)
{
	m_ReturnFlag = 1;
	UTF8ToStr(Text, m_Msg);

}
void CCharacter::UseSkillMsg(PCSTRINGA Text)
{
	UTF8ToStr(Text, m_Msg);
}

void CCharacter::ItemFail(PCSTRINGA Text)
{
	m_ReturnFlag = 1;
	UTF8ToStr(Text, m_Msg);

}
void CCharacter::UseItemMsg(PCSTRINGA Text)
{
	UTF8ToStr(Text, m_Msg);
}

void CCharacter::UpdateSpeed()
{
	UInt32 TempSpeed = m_baseSpeed;

	if (m_pPetGroup)
	{
		for (UInt i = 0; i < m_pPetGroup->GetCount(); i ++)
		{
			CPet* pPet = m_pPetGroup->GetChild<CPet>(i);
			if (pPet && pPet->GetState() == PET_STATE_TYPE_USE)
			{
				const PetData * pPetData = gGlobal.m_resource->GetPetData(pPet->GetPet_id());
				if (pPetData)
				{
					TempSpeed = (TempSpeed * (100 + pPetData->pet_Speed)) / 100;
				}
				break;
			}
		}
	}
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_Speed != 0)
		{
			TempSpeed = (TempSpeed * (100 + Itr->second.pBuffData->Buff_Speed)) / 100;
		}
	}
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_Speed != 0)
		{
			TempSpeed = (TempSpeed * (100 + Itr->second.pBuffData->Buff_Speed)) / 100;
		}
	}
	SetSpeed(TempSpeed);
}

RPCResult CCharacter::RespondTest(LPCPEER pPeer, RPCContext &context, 
								  const UInt32 &minRespond, const UInt32 &maxRespond, 
								  const float &avgRespond, const UInt32 &respondCount)
{
	m_minRespond = minRespond;
	m_maxRespond = maxRespond;
	m_avgRespond = avgRespond;
	m_respondCount = respondCount;

	return CB_RespondTest(pPeer, context, gGlobal.m_playerCount);
}

void CCharacter::CreateAntiAddictionObject()
{
	if (!m_pAntiGameAddict)
		m_pAntiGameAddict = SafeCreateObject(CAntiGameAddiction);
	
}
///////////////////////////////////////////////////////////////////////////////
///																			///
///							Event and State methods							///
///																			///
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////
//                                             //
//         Event for Anti-addiction            //
//                                             // 
/////////////////////////////////////////////////

void CCharacter::CancelAllGameHealthEvt()
{
	CancelUniqEvent(EVT_GHEALTH_FST_WARN);
	CancelUniqEvent(EVT_GHEALTH_SND_WARN);
	CancelUniqEvent(EVT_GHEALTH_THIRD_WARN);
	CancelUniqEvent(EVT_GHEALTH_FOURTH_WARN);
	CancelUniqEvent(EVT_GHEALTH_CHG_GDAVG);
	CancelUniqEvent(EVT_GHEALTH_CHG_AVGBAD);
	CancelUniqEvent(EVT_GHEALTH_INTEREST_HALF);
	CancelUniqEvent(EVT_GHEALTH_INTEREST_ZERO);
	CancelUniqEvent(EVT_GHEALTH_OFFTIME_CLEAR);
	CancelUniqEvent(EVT_GHEALTH_CLEAR_ONLINETIME);	
}

void CCharacter::RaiseGeneralGameHealthEvt(UInt32 evtType, UInt32 EvtTime)
{
	RaiseUniqEventID(evtType, EvtTime);	

}

void CCharacter::RaiseGameHealthChg(UInt32 evtType, UInt32 EvtTime)
{
	switch (evtType)
	{
	case EVT_GHEALTH_CHG_GDAVG:
		{
            RaiseUniqEventID(EVT_GHEALTH_CHG_GDAVG, EvtTime);	            
		}
		break;
	case EVT_GHEALTH_CHG_AVGBAD:
		{
			RaiseUniqEventID(EVT_GHEALTH_CHG_AVGBAD, EvtTime);	
		}
		break;
	}

}
void CCharacter::RaiseGameHealthWarn(UInt32 evtType, UInt32 EvtTime)
{
	switch (evtType)
	{
	case EVT_GHEALTH_FST_WARN:
		{
            RaiseUniqEventID(EVT_GHEALTH_FST_WARN, EvtTime);	            
		}
		break;
	case EVT_GHEALTH_SND_WARN :
		{
			RaiseUniqEventID(EVT_GHEALTH_SND_WARN, EvtTime);	
		}
		break;
	case EVT_GHEALTH_THIRD_WARN :
		{
			RaiseUniqEventID(EVT_GHEALTH_THIRD_WARN, EvtTime);	
		}
		break;
	case EVT_GHEALTH_FOURTH_WARN:
		{
			RaiseUniqEventID(EVT_GHEALTH_FOURTH_WARN, EvtTime);	
		}
		break;
	}
}

void CCharacter::RaiseInterestReduction(UInt32 evtType, UInt32 EvtTime)
{
	switch (evtType)
	{
	case EVT_GHEALTH_INTEREST_ZERO:
		{
            RaiseUniqEventID(EVT_GHEALTH_INTEREST_ZERO, EvtTime);	            
		}
		break;
	case EVT_GHEALTH_INTEREST_HALF:
		{
			RaiseUniqEventID(EVT_GHEALTH_INTEREST_HALF, EvtTime);	
		}
		break;
	}
}

void CCharacter::RaiseOffTimeClear(UInt32 EvtTime)
{
	RaiseUniqEventID(EVT_GHEALTH_OFFTIME_CLEAR, EvtTime);

}


void CCharacter::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_UPDATE_OBJECT:				// update all partner to object service
		{
			PROCEDURE_DO_METHOD(CCharacter, UpdateToObject, this, m_context)
		}
		break;
	case EVT_SESSION_LOGOUT:
		{
			if(HasLuaFunc(gGlobal.g_pLuaState,"OnCharacterLeave"))
				CallLuaFunc(gGlobal.g_pLuaState, "OnCharacterLeave", String(), this);

			CBattle * pBattle = FindNetObject<CBattle>(m_BattleNetID);
			// leave monitor when session logout
			if (pBattle && IsState(ST_MONITOR))
				pBattle->RemoveMonitor(this);	

			if (IsHawking())
				m_pHawkGroup->StopHawk();

			// remove session event when not in battle and in EnterMap state
			if (IsInMap() && pBattle == NULL)
			{
				if (!IsStartBattle())
				{
					m_sessLogout = BOOLEAN_FALSE;
					PROCEDURE_DO_FUNC(MapService_Stub::LeaveWorld, m_context)
				}
				else
				{
					m_sessLogout = BOOLEAN_TRUE;
				}
			}
			else
			{
				m_sessLogout = BOOLEAN_TRUE;
			}
			m_logoutHandle = 0;
		}
		break;

	case EVT_USE_SKILL:
		{
			if (m_pMap)
			{
				const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(m_TempSkillID);

				if (pSkillData)
				{
					Int skillLv = 0;
					if (pSkillData->reqBaseSkill == 0)
						skillLv = GetSkillLv(m_TempSkillID);
					else
						skillLv = GetSkillLv(pSkillData->reqBaseSkill);

					CCharacter * pTarget = CastNetObject<CCharacter>(m_pMap->FindMapActor(Character_CLASSID, m_TempSkillTargetID));
					if (pTarget)
					{
						m_Msg = _T("");
						m_ReturnFlag = 0;
						StringA LuaFunc;
						LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(m_TempSkillID, "skill").c_str(), CommonSkillFuncName[SKILL_FUNC_NAME_USE].c_str());
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), this,
							pTarget, NULL, skillLv);

						if (!m_Msg.IsEmpty() && GetPeer())
							CB_UseSkill(GetPeer(), GetContext(), m_TempSkillTargetID, 1, m_Msg, GetChar_id(), pTarget->GetChar_id());

						if (this != pTarget && !pTarget->m_Msg.IsEmpty() && pTarget->GetPeer())
							CB_UseSkill(pTarget->GetPeer(), pTarget->GetContext(), m_TempSkillTargetID, 1, pTarget->m_Msg, GetChar_id(), pTarget->GetChar_id());	
					}
				}
			}
				
			m_TempSkillID = 0;
			m_TempSkillTargetID = 0;
			m_TempSkillTargetType = 0;
		}
		break;

	case EVT_UPDATE_BUFF:
		{
			CheckBuffUpdate();
		}
		break;

	case EVT_BATTLE_LOCKED:
		{
			TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("Deadlock Warning: [battle state locked], sessionId %s, char_id %d, action %d"), 
				m_context.sessionId.GetString().c_str(), GetChar_id(), GetAction());
			//if (!ChangeState(ST_NOBATTLE)) {
			//	TRACE_WARNDTL(GLOBAL_LOGGER, _F("Deadlock Warning (2): change to NO_BATTLE state failed"));
			//}
		}
		break;

	case EVT_TRADING_REQ_TIME:
		{
			if (m_pTradingTargetNetID != RPCNetID() && m_pTrading == NULL)
			{
				CB_TradingMessage(GetPeer(), GetContext(), FALSE, TRADING_FAIL_REQ_TIMEOUT);//_T("MSG_TRADING_REQ_TIMEOUT"));
				CCharacter* target = FindNetObject<CCharacter>(m_pTradingTargetNetID);
				if (target)
					target->CB_TradingMessage(target->GetPeer(), target->GetContext(), FALSE, TRADING_FAIL_REQ_TIMEOUT);//_T("MSG_TRADING_REQ_TIMEOUT"));
				ClearReqTrading();
			}
		}
		break;

	case EVT_TITLE_EXPIRE:
		CalTitleExpire();
		break;

	case EVT_UPDATE_TITLE:
		UpdateTitle();
		break;

	case EVT_CHECK_JUMP:
		m_pScriptSys->CheckJump();
		break;

	case EVT_GHEALTH_FST_WARN:
	case EVT_GHEALTH_SND_WARN:
	case EVT_GHEALTH_THIRD_WARN:
	case EVT_GHEALTH_FOURTH_WARN:
	case EVT_GHEALTH_CHG_GDAVG:
	case EVT_GHEALTH_CHG_AVGBAD:
	case EVT_GHEALTH_INTEREST_HALF:
	case EVT_GHEALTH_INTEREST_ZERO:
	case EVT_GHEALTH_OFFTIME_CLEAR:
	case EVT_GHEALTH_CLEAR_ONLINETIME:
        if (m_pAntiGameAddict)
            m_pAntiGameAddict->EventHandler(GetPeer(), GetContext(), this, event.GetID());
		else
		{
			TRACE_WARNDTL(GLOBAL_LOGGER, _F("CAccount::HandleEvent Warning: no anit-addiction object created."));	
		}
        break;

	case EVT_MSGDATA_UPDATE:
		gGlobal.UpdateMsgChar(this);
		break;

	case EVT_CLEAR_WAREHOUSE:
		{
			m_pBag->ClearWareHouse();
			m_pPartnerGroup->ClearWareHouse();
			m_pPetGroup->ClearWareHouse();
			m_bCacheWareHouse = false;
		}
		break;

	case EVT_UPDATE_YUANBAO_TIMEOUT:
		m_bIsUpdatedYuanBao = false;
		break;

	case EVT_ROBOT_ACTION:
		RobotAction();
		break;

	default:
		CMapActor::HandleEvent(event);
	}
}

Boolean CCharacter::ChangeState(CONST StateID state)
{
	switch (state)
	{
	case ST_NOBATTLE:
		return State::ChangeState(m_battleState, SafeCreateObject1(CNoBattleState, this));
	case ST_BATTLE:
		return State::ChangeState(m_battleState, SafeCreateObject1(CBattleState, this));
	case ST_MONITOR:
		return State::ChangeState(m_battleState, SafeCreateObject1(CMonitorState, this));
	case ST_TALK:
		return State::ChangeState(m_moveState, SafeCreateObject1(CTalkState, this));
	case ST_HAWK:
		return State::ChangeState(m_moveState, SafeCreateObject(CHawkState));
	default:
		return Parent::ChangeState(state);
	}

	return BOOLEAN_FALSE;
}

Boolean CCharacter::IsState(CONST StateID state)
{
	switch (state)
	{
	case ST_NOBATTLE:
	case ST_BATTLE:
	case ST_MONITOR:
		return (m_battleState->GetStateID() == state);

	case ST_TALK:
	case ST_HAWK:
		return (m_moveState->GetStateID() == state);

	default:
		return Parent::IsState(state);
	}

	return BOOLEAN_FALSE;
}

Boolean CCharacter::TryState(CONST StateID state)
{
	switch (state)
	{
	case ST_NOBATTLE:
		return State::TestChangeState(m_battleState, SafeCreateObject1(CNoBattleState, this));
	case ST_BATTLE:
		return State::TestChangeState(m_battleState, SafeCreateObject1(CBattleState, this));
	case ST_MONITOR:
		return State::TestChangeState(m_battleState, SafeCreateObject1(CMonitorState, this));
	case ST_TALK:
		return State::TestChangeState(m_moveState, SafeCreateObject1(CTalkState, this));
	case ST_HAWK:
		return State::TestChangeState(m_moveState, SafeCreateObject(CHawkState));
	default:
		return Parent::TryState(state);
	}

	return BOOLEAN_FALSE;
}

void CCharacter::ScheduleLogout(UInt32 logoutTime)
{
	m_sessLogout = BOOLEAN_FALSE;
	m_logoutHandle = RaiseUniqEventID(EVT_SESSION_LOGOUT, logoutTime);
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("logoutTime %d, sessionId %s, handle %d"),
		logoutTime, m_sessionId.GetString().c_str(), m_logoutHandle);
}

void CCharacter::CancelLogout()
{
	m_sessLogout = BOOLEAN_FALSE;
	if (m_logoutHandle != 0)
	{
		TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("Character CancelLogout: sessionId %s, handle %d"),
			m_sessionId.GetString().c_str(), m_logoutHandle);
		Reuben::Simulation::CancelEvent(m_logoutHandle);
		m_logoutHandle = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
///																			///
///							Data Exchange methods							///
///																			///
///////////////////////////////////////////////////////////////////////////////

void CCharacter::SetData(CMessageCharacter * pMsgchar)
{
	if (pMsgchar->IsUpdated(CMessageCharacter::speakBlockMask))
	{
		time_t now = time(NULL);
		if ((UInt32) now >= pMsgchar->GetSpeakBlock())
			SetSpeakBlock(0);
		else
			SetSpeakBlock(pMsgchar->GetSpeakBlock());
	}
}
void CCharacter::SetData(CCharacterData* pCharData)
{
	if (pCharData->IsUpdated(CCharacterData::idMask)) {
		SetId(pCharData->GetAcct_id(), pCharData->GetChar_id());
		if (m_pSkillGroup)
			m_pSkillGroup->SetOwner(pCharData->GetChar_id(), SKILL_OWNER_TYPE_CHAR);
	}

	if (pCharData->IsUpdated(CCharacterData::infoMask))
	{
		UInt8 SexClass = (pCharData->GetCclass() > 0 ? 
			((pCharData->GetCclass() - 1) * MAX_SEXTYPE + pCharData->GetSex() + 1) : 0);

		SetInfo(pCharData->GetNickName(), pCharData->GetSex(),
			pCharData->GetCclass(), SexClass, pCharData->GetFaction());
	}

	if (pCharData->IsUpdated(CCharacterData::levelInfoMask)) {
		SetLevel(pCharData->GetLevel());
		CalcNextLevel();
	}

	if (pCharData->IsUpdated(CCharacterData::expMask))
		SetExp(pCharData->GetExp());

	if (pCharData->IsUpdated(CCharacterData::attribMask)) {
		SetAttrib1(pCharData->GetSTA(), pCharData->GetSPR(), 
			pCharData->GetSTR(), pCharData->GetCON(), 
			pCharData->GetAGI());
	}

	if (pCharData->IsUpdated(CCharacterData::titleMask))
		SetTitle(pCharData->GetTitle());

	if (pCharData->IsUpdated(CCharacterData::HPMask)) {
		SetHP(pCharData->GetHP(), pCharData->GetMHP(), pCharData->GetMaxHP());
	}
	if (pCharData->IsUpdated(CCharacterData::SPMask)) {
		SetSP(pCharData->GetSP(), pCharData->GetMSP(), pCharData->GetMaxSP());
	}
	if (pCharData->IsUpdated(CCharacterData::DPMask)) {
		SetDP(pCharData->GetDP(), pCharData->GetMDP(), pCharData->GetMaxDP());
	}
	if (pCharData->IsUpdated(CCharacterData::WCMask)) {
		SetWC(pCharData->GetWC());
		SetMaxWC(pCharData->GetMaxWC());
		SetWcExp(pCharData->GetWcExp());
		CalcNextWcLevel();
		SetDP(GetDP(), 150, 150);
	}
	if (pCharData->IsUpdated(CCharacterData::VPMask)) {
		SetVP(pCharData->GetVP(), pCharData->GetMaxVP());
		CalMaxVP();
	}

	if (pCharData->IsUpdated(CCharacterData::stateMask)) {
		SetState(pCharData->GetState());
	}

	if (pCharData->IsUpdated(CCharacterData::buffMask)) {
		SetBuff(pCharData->GetBuff());
		ExtractBuffString();
	}

	if (pCharData->IsUpdated(CCharacterData::mapMask)) {
		SetMap(pCharData->GetMap_id(), 
			pCharData->GetMap_x(), pCharData->GetMap_y(), 0, pCharData->GetMap_ownerId());
	}

	if (pCharData->IsUpdated(CCharacterData::rebornMask)) {
		SetReborn(pCharData->GetReborn_map(), 
			pCharData->GetReborn_x(), pCharData->GetReborn_y());
	}

	if (pCharData->IsUpdated(CCharacterData::styleMask)) {
		SetStyle(pCharData->GetPhoto_id(), pCharData->GetHairStyle1(), 
			pCharData->GetHairStyle2(), pCharData->GetHairColor());
	}

	if (pCharData->IsUpdated(CCharacterData::eqWeaponMask)) {
		SetEqWeapon(pCharData->GetEq_weapon(), pCharData->GetEq_weaponColor());
	}
	if (pCharData->IsUpdated(CCharacterData::eqHeadMask)) {
		SetEqHead(pCharData->GetEq_headBand(), pCharData->GetEq_headColor());
	}
	if (pCharData->IsUpdated(CCharacterData::eqClothMask)) {
		SetEqCloth(pCharData->GetEq_cloth(), pCharData->GetEq_clothColor());
	}
	if (pCharData->IsUpdated(CCharacterData::attrPointMask)) {
		SetAttrPoint(pCharData->GetAttrPoint());
	}
	if (pCharData->IsUpdated(CCharacterData::PKInfoMask)) {
		SetPKInfo(pCharData->GetPK_Value());
	}
	if (pCharData->IsUpdated(CCharacterData::maxOtherMask))
	{
		SetMaxOther(pCharData->GetMaxPartner(), pCharData->GetMaxPet());
	}
	if (pCharData->IsUpdated(CCharacterData::goodnessMask))
	{
		SetGoodness(pCharData->GetGoodness());
	}
	if (pCharData->IsUpdated(CCharacterData::honorMask))
	{
		SetHonor(pCharData->GetHonor());
	}
	if (pCharData->IsUpdated(CCharacterData::moneyMask))
	{
		Parent::SetMoney(pCharData->GetMoney());
	}
	if (pCharData->IsUpdated(CCharacterData::bankMoneyMask))
	{
		SetBankMoney(pCharData->GetBankMoney());
	}

	if (pCharData->IsUpdated(CCharacterData::donateMask))
	{
		SetDonate(pCharData->GetFDonate(), pCharData->GetTotalFDonate(), pCharData->GetMDonate(), pCharData->GetTotalMDonate());
	}

	if (pCharData->IsUpdated(CCharacterData::petMask))
	{
		SetPet(pCharData->GetPet(), GetPet_id(), GetPetName(), GetPetColorIndex());
	}

	if (pCharData->IsUpdated(CCharacterData::buffMask))
	{
		SetBuff(pCharData->GetBuff());
	}
	if (pCharData->IsUpdated(CCharacterData::shortcutMask))
	{
		SetShortcut(pCharData->GetShortcut(0), pCharData->GetShortcut(1), pCharData->GetShortcut(2),
			pCharData->GetShortcut(3), pCharData->GetShortcut(4), pCharData->GetShortcut(5),
			pCharData->GetShortcut(6), pCharData->GetShortcut(7), pCharData->GetShortcut(8),
			pCharData->GetShortcut(9), pCharData->GetShortcuttype(0), pCharData->GetShortcuttype(1), pCharData->GetShortcuttype(2),
			pCharData->GetShortcuttype(3), pCharData->GetShortcuttype(4), pCharData->GetShortcuttype(5),
			pCharData->GetShortcuttype(6), pCharData->GetShortcuttype(7), pCharData->GetShortcuttype(8),
			pCharData->GetShortcuttype(9) );
	}
	
	if (pCharData->IsUpdated(CCharacterData::orgEqClothMask))
	{
		SetOrgEqCloth(pCharData->GetOrg_eq_cloth(), pCharData->GetOrg_eq_clothColor());
	}
	if (pCharData->IsUpdated(CCharacterData::orgHairStyleMask))
	{
		SetOrgHairStyle(pCharData->GetOrg_hairStyle1(), pCharData->GetOrg_hairColor());
	}

	if (pCharData->IsUpdated(CCharacterData::speakCountMask))
	{
		SetSpeakCount(pCharData->GetSpeakCount());
	}

	if (pCharData->IsUpdated(CCharacterData::offlineTimeMask))
	{
		SetOfflineTime(pCharData->GetOfflineTime());
	}

	if (pCharData->IsUpdated(CCharacterData::TPMask))
	{
		SetTP(pCharData->GetTP());
	}

	if (pCharData->IsUpdated(CCharacterData::speakBlockMask))
	{
		time_t now = time(NULL);
		if ((UInt32) now >= pCharData->GetSpeakBlock())
			SetSpeakBlock(0);
		else
			SetSpeakBlock(0);
	}

	if (pCharData->IsUpdated(CCharacterData::hawkerTimeMask))
	{
		SetHawkerTime(pCharData->GetHawkerTime());
	}

	if (pCharData->IsUpdated(CCharacterData::PvPScoreSingleMask))
	{
		SetPvPScoreSingle(pCharData->GetPvPScoreSingle());
	}

	if (pCharData->IsUpdated(CCharacterData::SMoneyMask))
	{
		SetSMoney(pCharData->GetSMoney());
	}
	
	if (pCharData->IsUpdated(CCharacterData::availBackPackMask))
	{
		SetAvailBackPack(pCharData->GetAvailBackPack());
	}
	if (pCharData->IsUpdated(CCharacterData::availWareHouseMask))
	{
		SetAvailWareHouse(pCharData->GetAvailItemWareHouse(), pCharData->GetAvailPartnerWareHouse(), pCharData->GetAvailPetWareHouse());
	}

	SetPartyInfo(BOOLEAN_FALSE, BOOLEAN_TRUE, RPCNetID());

	if (pCharData->IsUpdated(CCharacterData::bankLockMask))
	{   
		SetBankLock(pCharData->GetBankLock());
	}	

	if (pCharData->IsUpdated(CCharacterData::SiblingIdMask))
	{   
		SetSiblingId(pCharData->GetSibling_id());
	}

	if (pCharData->IsUpdated(CCharacterData::markMask))
	{   
		SetMark(pCharData->GetMark());
	}

	if (pCharData->IsUpdated(CCharacterData::wantedRewardMask))
	{   
		SetWantedReward(pCharData->GetWantedReward());
	}

	if (pCharData->IsUpdated(CCharacterData::GenerationAccMask)) {
		SetIHisGenAcc(pCharData->GetIHisGenAcc());
		SetUiCurGenAcc(pCharData->GetUiCurGenAcc());
	}
	
	SetEnablePK(((GetMark() & (1 << MARK_ENABLE_PK)) > 0)?1:0);
	SetVisible(BOOLEAN_TRUE);

	ClearAllMask();
}

void CCharacter::WriteData(CCharacterData* pCharData)
{
	TRACE_ENSURE(pCharData);

	pCharData->ClearUpdateMask();
	if (IsModified(infoMask))
		pCharData->SetInfo(GetNickName(), GetSex(), GetCclass(), GetFaction());
	if (IsModified(levelInfoMask))
		pCharData->SetLevel(GetLevel());
	if (IsModified(expMask))
		pCharData->SetExp(GetExp());
	if (IsModified(attrib1Mask))
		pCharData->SetAttrib(GetSTA(), GetSPR(), GetSTR(), GetCON(), GetAGI());
	if (IsModified(titleMask))
		pCharData->SetTitle(GetTitle());
	if (IsModified(HPMask))
		pCharData->SetHP(GetHP(), GetMHP(), GetMaxHP());
	if (IsModified(SPMask))
		pCharData->SetSP(GetSP(), GetMSP(), GetMaxSP());
	if (IsModified(DPMask))
		pCharData->SetDP(GetDP(), GetMDP(), GetMaxDP());
	if (IsModified(WCMask))
		pCharData->SetWC(GetWC(), GetMaxWC(), GetWcExp());
	if (IsModified(VPMask))
		pCharData->SetVP(GetVP(), GetMaxVP());
	
	if (IsModified(stateMask))
		pCharData->SetState(GetState());
	if (IsModified(buffMask))
		pCharData->SetBuff(GetBuff());
	if (IsModified(mapMask))
		pCharData->SetMap(GetMap_id(), GetMap_x(), GetMap_y(), GetMap_ownerId());
	if (IsModified(rebornMask))
		pCharData->SetReborn(GetReborn_map(), GetReborn_x(), GetReborn_y());
	if (IsModified(styleMask))
		pCharData->SetStyle(GetPhoto_id(), GetHairStyle1(), 
			GetHairStyle2(), GetHairColor());
	if (IsModified(eqWeaponMask))
		pCharData->SetEqWeapon(GetEq_weapon(), GetEq_weaponColor());
	if (IsModified(eqHeadMask))
		pCharData->SetEqHead(GetEq_headBand(), GetEq_headColor());
	if (IsModified(eqClothMask))
		pCharData->SetEqCloth(GetEq_cloth(), GetEq_clothColor());
	
	if (IsModified(attrPointMask))
		pCharData->SetAttrPoint(GetAttrPoint());
	if (IsModified(PKInfoMask))
		pCharData->SetPKInfo(GetPK_Value());
	if (IsModified(maxOtherMask))
		pCharData->SetMaxOther(GetMaxPartner(), GetMaxPet());
	if (IsModified(goodnessMask)){
		pCharData->SetGoodness(GetGoodness());
	}
	if (IsModified(honorMask))
		pCharData->SetHonor(GetHonor());
	if (IsModified(moneyMask))
		pCharData->SetMoney(GetMoney());
	if (IsModified(bankMoneyMask))
		pCharData->SetBankMoney(GetBankMoney());
	if (IsModified(donateMask))
		pCharData->SetDonate(GetFDonate(), GetTotalFDonate(), GetMDonate(), GetTotalMDonate());
	if (IsModified(petMask))
		pCharData->SetPet(GetPet_uid());

	if (IsModified(shortcutMask))
	{
		pCharData->SetShortcut(GetShortcut(0), GetShortcut(1), GetShortcut(2),
			GetShortcut(3), GetShortcut(4), GetShortcut(5),	GetShortcut(6), 
			GetShortcut(7), GetShortcut(8),	GetShortcut(9), GetShortcuttype(0), GetShortcuttype(1), GetShortcuttype(2),
			GetShortcuttype(3), GetShortcuttype(4), GetShortcuttype(5),	GetShortcuttype(6), 
			GetShortcuttype(7), GetShortcuttype(8),	GetShortcuttype(9) );
	
	}
	if (IsModified(orgEqClothMask))
	{
		pCharData->SetOrgEqCloth(GetOrg_eq_cloth(), GetOrg_eq_clothColor());
	}
	if (IsModified(orgHairStyleMask))
	{
		pCharData->SetOrgHairStyle(GetOrg_hairStyle1(), GetOrg_hairColor());
	}
	
	if (IsModified(offlineTimeMask))
	{
		pCharData->SetOfflineTime(GetOfflineTime());
	}

	if (IsModified(TPMask))
	{
		pCharData->SetTP(GetTP());
	}
	

	if (IsModified(hawkerTimeMask))
	{
		pCharData->SetHawkerTime(GetHawkerTime());
	}

	if (IsModified(SMoneyMask))
	{
		pCharData->SetSMoney(GetSMoney());
	}

	if (IsModified(availBackPackMask))
	{
		pCharData->SetAvailBackPack(GetAvailBackPack());
	}
	if (IsModified(availWareHouseMask))
	{
		pCharData->SetAvailWareHouse(GetAvailItemWareHouse(), GetAvailPartnerWareHouse(), GetAvailPetWareHouse());
	}

	if (IsModified(bankLockMask))
	{
		pCharData->SetBankLock(GetBankLock());
	}

	if (IsModified(PvPScoreSingleMask))
	{
		pCharData->SetPvPScoreSingle(GetPvPScoreSingle());
	}

	if (IsModified(SiblingIdMask))
	{
		pCharData->SetSiblingId(GetSibling_id());
	}

	if (IsModified(markMask))
	{
		pCharData->SetMark(GetMark());
	}

	if (IsModified(wantedRewardMask))
	{
		pCharData->SetWantedReward(GetWantedReward());
	}
	
	pCharData->SetGenerationAcc(GetIHisGenAcc(),GetUiCurGenAcc());

	if (pCharData->IsUpdated()) {
		pCharData->SetId(GetAcct_id(), GetChar_id());
		pCharData->SetSendMask(pCharData->GetUpdateMask());
	}

}

Boolean CCharacter::NeedUpdate()
{
	RPCMask mask;

	mask.MaskBits(infoMask, levelInfoMask, expMask, attrib1Mask, HPMask, SPMask, DPMask, WCMask, 
		VPMask, stateMask, buffMask, mapMask, rebornMask, styleMask, eqWeaponMask, eqHeadMask,
		eqClothMask, attrPointMask, PKInfoMask, maxOtherMask, goodnessMask, honorMask, titleMask,
		yuanBaoMask, moneyMask, bankMoneyMask, donateMask, partnerMask, petMask, shortcutMask, 
		orgEqClothMask, offlineTimeMask, TPMask, hawkerTimeMask, 
		hawkInfoMask, availBackPackMask, availWareHouseMask, unlockTimeMask, yuanbaoLockMask,
		bankLockMask, PvPScoreSingleMask, SiblingIdMask, guildUidMask, markMask, SMoneyMask,
		wantedRewardMask,manorMask,TPMask, -1);

	return IsModified(mask);
}

void CCharacter::GetMsgCharData(CMsgCharacterData* pData)
{
	if (IsUpdated(infoMask))
		pData->SetInfo(GetNickName(), GetSex(), GetCclass(), GetFaction()); 
	if (IsUpdated(levelInfoMask))
		pData->SetLevel(GetLevel());
	if (IsUpdated(titleMask))
		pData->SetTitle(GetTitle());
	if (IsUpdated(styleMask))
		pData->SetStyle(GetPhoto_id(), GetHairStyle1(), GetHairStyle2(), GetHairColor());
	if (IsUpdated(eqWeaponMask))
		pData->SetEqWeapon(GetEq_weapon(), GetEq_weaponColor());
	if (IsUpdated(eqHeadMask))
		pData->SetEqHead(GetEq_headBand(), GetEq_headColor());
	if (IsUpdated(eqClothMask))
		pData->SetEqCloth(GetEq_cloth(), GetEq_clothColor());
	if (IsUpdated(orgEqClothMask))
		pData->SetOrgEqCloth(GetOrg_eq_cloth(), GetOrg_eq_clothColor());
	if (IsUpdated(orgHairStyleMask))
		pData->SetOrgHairStyle(GetOrg_hairStyle1(), GetOrg_hairColor());
	if (IsUpdated(speakCountMask))
		pData->SetSpeakCount(GetSpeakCount());
	if (IsUpdated(speakBlockMask))
		pData->SetSpeakBlock(GetSpeakBlock());
	if (IsUpdated(offlineTimeMask))
		pData->SetOfflineTime(GetOfflineTime());
	if (m_pMap)
		pData->SetLine(m_pMap->GetLine_id());
	pData->SetEnablePK(GetEnablePK());
	pData->SetSettingInfo(GetEnableBlockMakeFriend(), GetEnableBlockMsg());
	pData->SetId(GetChar_id());
}

RPCResult CCharacter::UpdateToObject(RPCContext &context)
{
	PROCEDURE_START_METHOD(CCharacter, UpdateToObject, this, context)

		if (NeedUpdate())
		{
			CCharacterData charData;
			WriteData(&charData);
			PROCEDURE_STORE_VALUE1(RPCMask, GetModifyMask())
			ClearModifyMask();
			ObjectService_Proxy::UpdateCharacter(gGlobal.pObjectService->pPeer, context, 
												GetChar_id(), &charData);
		}
		else
		{
			// no need update
			PROCEDURE_RETURN
		}

	PROCEDURE_WAIT(1, ObjectService, CB_UpdateCharacter)

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CCharacter UpdateToObject Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

		// restore update mask after failed
		PROCEDURE_RESTORE_VALUE1(RPCMask, oldMask)
		MarkModify(oldMask);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::UpdateAllObjects(RPCContext &context)
{
	PROCEDURE_START_METHOD(CCharacter, UpdateAllObjects, this, context)

		// update character
		CancelUpdate();					// cancel character update event first
		PROCEDURE_DO_METHOD(CCharacter, UpdateToObject, this, context)

	PROCEDURE_WAIT_RETURN(1)

		// update item bag
		m_pBag->CancelUpdate();			// cancel item update event first
		m_pBag->UpdateTimeBuff();
		PROCEDURE_DO_METHOD(CItemBag, UpdateItems, m_pBag, context)

	PROCEDURE_WAIT_RETURN(2)

		// update all skills
		PROCEDURE_DO_METHOD(CSkillGroup, UpdateSkills, m_pSkillGroup, context)

	PROCEDURE_WAIT_RETURN(3)

		// update all partners
		m_pPartnerGroup->CancelUpdate();	// cancel partner update event first
		PROCEDURE_DO_METHOD1(CPartnerGroup, UpdateAllPartner, m_pPartnerGroup, context, Boolean,
			BOOLEAN_TRUE)

	PROCEDURE_WAIT_RETURN(4)

		// update all quest log
		m_pQuestSys->CancelUpdate();		// cancel quest system update event first
		PROCEDURE_DO_METHOD(CQuestSystem, UpdateQuests, m_pQuestSys, context)

	PROCEDURE_WAIT_RETURN(5)

	// update all pet type
		m_pPetGroup->CancelUpdate();	// cancel quest log update event first
		PROCEDURE_DO_METHOD1(CPetGroup, UpdateAllPet, m_pPetGroup, context,  Boolean,
			BOOLEAN_TRUE)

	PROCEDURE_WAIT_RETURN(6)

		PROCEDURE_DO_METHOD(CGenerationGroup, UpdateGenerationGroup, m_pGeneration,context)

	PROCEDURE_WAIT_RETURN(7)

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("session_id %s, char_id %d"),
				context.sessionId.GetString().c_str(), GetChar_id());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

VOID CCharacter::RaiseAllUpdate()
{
	RaiseUpdate();
	m_pBag->RaiseUpdate();
	m_pSkillGroup->RaiseUpdate();
	m_pPartnerGroup->RaiseUpdate();
	m_pQuestSys->RaiseUpdate();
	m_pPetGroup->RaiseUpdate();
}

VOID CCharacter::CancelAllUpdate()
{

	CancelUpdate();
	m_pBag->CancelUpdate();
	m_pSkillGroup->CancelUpdate();
	m_pPartnerGroup->CancelUpdate();
	m_pQuestSys->CancelUpdate();
	m_pPetGroup->CancelUpdate();
}

void CCharacter::SetRobot(Boolean bRobot, UInt32 interval)
{
	Char script[256];

	m_bRobot = bRobot;
	GetPrivateProfileString(_T("Robot"), _T("Script"), ROBOT_SCRIPT, script, 255,
		 CONFIGURATION_INI_FILE);
	if (m_bRobot)
		CB_SetRobot(m_pPeer, m_context, bRobot, script);
	else
		CB_SetRobot(m_pPeer, m_context, bRobot, _T(""));
	m_robotInterval = interval;
	if (bRobot)
		RaiseUniqEventID(EVT_ROBOT_ACTION);
	else
		CancelUniqEvent(EVT_ROBOT_ACTION);
}

void CCharacter::RobotAction()
{
	if (IsInMap() && m_pPeer)
		CB_RobotMapAction(m_pPeer, m_context, _T("MapAction"), m_pMap->GetNetId(), 
			GetNetId(), RPCNetID());
	RaiseUniqEventID(EVT_ROBOT_ACTION, m_robotInterval);
}

///////////////////////////////////////////////////////////////////////////////
///																			///
///								Map methods									///
///																			///
///////////////////////////////////////////////////////////////////////////////

RPCResult CCharacter::MoveTo(LPCPEER pPeer, RPCContext &context, 
							 const UInt16 &posX, const UInt16 &posY,
							 const UInt16 &hint1, const UInt16 &hint2)
{
	// check if move to same destination position, do nothing or try to stop
	if (IsState(ST_MOVE) && posX == GetDstPosX() && posY == GetDstPosY())
		return RPC_RESULT_OK;

	if (m_pMap == NULL || m_pMap->GetPathFinder() == NULL) {
		//CB_Error(pPeer, context, 1, _T("CCharacter MoveTo Error: invalid map or path finder"));
		return RPC_RESULT_FAIL;
	} else if (!IsLeader() || m_pScriptSys->IsRunning()) {
		// can't move if not leader or script running
		return RPC_RESULT_FAIL;
	}

	// check if out range of map
	if (posX >= m_pMap->GetWidth() || posY >= m_pMap->GetHeight()) {
//		String s;
//		s.Format(_T("CCharacter MoveTo Error: move %d:%d out of map %d range %d:%d"), 
//			posX, posY, m_pMap->GetMap_id(), m_pMap->GetWidth(), m_pMap->GetHeight());
//		CB_Error(pPeer, context, 1, s);
		return RPC_RESULT_FAIL;
	}

	if (!TryState(ST_MOVE)) {
		//CB_Error(pPeer, context, 1, _T("CCharacter MoveTo Error: cannot move at this moment"));
		return RPC_RESULT_FAIL;
	}
	Move(posX, posY, hint1, hint2);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::StopMove(LPCPEER pPeer, RPCContext &context)
{
	Stop();
	return RPC_RESULT_OK;
}

RPCResult CCharacter::SetTarget(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
#ifndef REUBEN_PUBLISH
	CCharacter* pTarget = CastNetObject<CCharacter>(m_pMap->FindMapActor(Character_CLASSID, char_id));
	if (pTarget)
		CB_SetTarget(pPeer, context, BOOLEAN_TRUE, char_id, pTarget->GetCurPosX(), pTarget->GetCurPosY());
	else
		CB_SetTarget(pPeer, context, BOOLEAN_FALSE, char_id, 0, 0);
#endif // REUBEN_PUBLISH
	return RPC_RESULT_OK;
}

VOID CCharacter::SetExclusiveNpc(CMapNPC* pNpc)
{
	if (pNpc)
	{
		pNpc->ClearAcceptId();
		if (m_pParty) {
			CCharacter* member;
			for (Index i = C_INDEX(0); i < m_pParty->GetCount(); ++i )
			{
				member = m_pParty->GetMember(i);
				if (member)
					pNpc->AddAcceptId(member->GetChar_id());
			}
		} else {
			pNpc->AddAcceptId(GetChar_id());
		}
	}
}

Boolean CCharacter::PreCharacterEnter(CMap *pmap, const EnterInfo &info,
									  const UInt32Vector &memberIdList, Boolean bLocal,
									  UInt16 ui16LineupId,int iKeyPos,CParty* pTempParty)
{
	TRACE_ENSURE(pmap != NULL);

	if( GetMap() )
	{
		return BOOLEAN_FALSE;
	}

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("char %d, map_id %d, line_id %d, speed %d"), 
		GetActorId(), info.map_id, info.line_id, GetSpeed());

	if (!ChangeState(CCharacter::ST_ENTER_MAP))
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("cannot change to ENTER state, char_id %d"),
			GetChar_id());
		return BOOLEAN_FALSE;
	}

	if (m_sessLogout)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("sessionId %s, character %d should logout"),
			m_sessionId.GetString().c_str(), GetChar_id());
		ScheduleLogout(0);
		return BOOLEAN_FALSE;
	}

	m_pMap = pmap;

	if( !GetMap() )
	{
		return BOOLEAN_FALSE;
	}
	
	TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("session_id %s, char_id %d, map_id %d, map_x %d, map_y %d"),
		GetContext().sessionId.GetString().c_str(), GetChar_id(), info.map_id, info.x, info.y);
	SetMap(pmap->GetMap_id(), info.x, info.y, 0, info.ownerId);
	SetCurPos(info.x, info.y);
	SetDstPos(info.x, info.y);
	m_path.clear();
	m_nextStep.PosX = info.x;
	m_nextStep.PosY = info.y;
	SetAction(CCharacter::ST_STOP, ::timeGetTime());

	SendClient(!bLocal, BOOLEAN_FALSE);			// send first part to client

	// enter map event
	OnEnterMap(pmap);
	ClearUpdateMask();
	ClearViewMask();

	// try to join back party in current map
	UInt16 partyCount = 0;
	if (!memberIdList.empty())
	{
		CCharacter* member;
		CParty* pParty = pTempParty;

		// find party in any member
		if (NULL == pParty) 
		{
			for (UInt32Vector::const_iterator it = memberIdList.begin(); 
				it != memberIdList.end(); ++it)
			{
				member = (CCharacter*)(pmap->FindMapActor(Character_CLASSID, *it));
				if (member && member != this && member->GetParty())
				{
					pParty = member->GetParty();
					break;
				}
			}
		}

		if (pParty == NULL)
		{
			// no one have party, create unavailable party to regroup
			pParty = CParty::NewInstance();
			TRACE_ENSURE(pParty);
			pParty->SetAvail(BOOLEAN_FALSE);
#ifdef LINEUP_SYSTEM_ON
			pParty->SetLineup(ui16LineupId);
			pParty->SetLineupkeypos(iKeyPos);
#endif
		}

		// regroup this character back to party if remote character jump
		if (NULL == pTempParty)
		{
			if (!pParty->Regroup(this, memberIdList))
				CB_WarningMessage(m_pPeer, m_context, WARN_PARTY_REGROUP_FAIL, 0, WARN_SHOW_IN_CHATBOX);
		}
		else//local jump
		{
			if (NULL != GetParty())// not temporarily leave 
			{
				for(UInt16 i=0;i<pParty->GetCount();i++)
				{
					CCharacter* pChar = pParty->GetChild<CCharacter>(i);
					if (NULL != pChar && pChar != this)
					{
						if (!pParty->GetM_bTempLeave(i) && !pChar->IsState(ST_JUMP_MAP)) 
						{
							if (pParty->GetLeader() == this) 
								pChar->FollowTarget(this);
						}
					}
				}

				if (NULL != pParty->GetLeader() 
					&& !pParty->GetLeader()->IsState(ST_JUMP_MAP)
					&& pParty->GetLeader() != this)
					FollowTarget(pParty->GetLeader());
				
				SetParty(pParty,pParty->GetLeader() == this);
				//check if all member jumped
				Boolean bJumpCompleted = BOOLEAN_TRUE;
				for (int j=0;j<memberIdList.size();j++) 
				{
					CCharacter* pMember = pParty->FindMember(memberIdList[j]);
					if (NULL != pMember) 
					{
						if (pMember->IsState(ST_JUMP_MAP)) 
							bJumpCompleted =BOOLEAN_FALSE;
					}
				}

				//if completed,Send Party information to all member
				if (bJumpCompleted) 
				{
					RPCMask mask(CCharacter::m_initMask);
					mask.MaskBits(CCharacter::idMask, CCharacter::followIdMask, CCharacter::partyInfoMask, 
						CCharacter::HPMask, CCharacter::SPMask, -1);
					for (int k=0;k<memberIdList.size();k++) 
					{
						CCharacter* pMember = pParty->FindMember(memberIdList[k]);
						if (NULL != pMember) 
						{
							for (int l=0;l<memberIdList.size();l++) 
							{//set all jump map member send mask
								CCharacter* pMember = pParty->FindMember(memberIdList[l]);
								if (NULL != pMember) 
									pMember->SetSendMask(mask);
							}
							//set party send mask and send to client
							pParty->ClearUpdateMask();
							pParty->SetSendMask(CParty::AllMask);
							CB_GetPartyInfoWhenJumpMap(pMember->GetPeer(),pMember->GetContext(),pParty);
						}
					}
				}
				else
				{
					pParty->ClearUpdateMask();
					pParty->SetSendMask(CParty::AllMask);
					CB_GetPartyInfoWhenJumpMap(GetPeer(),GetContext(),pParty);
				}
			}
		}
	}
	else if (info.followId != 0)
	{
		// follow target move for single
		CMapActor* actor = pmap->FindMapActor(Character_CLASSID, info.followId);
		if (actor)
			FollowTarget(actor);
	}

	return BOOLEAN_TRUE;
}

Boolean CCharacter::PostCharacterEnter(CMap *pmap, const EnterInfo &info,
									 const UInt32Vector &memberIdList, Boolean bLocal)
{
	TRACE_ENSURE(pmap != NULL);

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("char %d, map_id %d, line_id %d, speed %d"), 
		GetActorId(), info.map_id, info.line_id, GetSpeed());

	bool bJumped = false;
	m_bEnterDone = BOOLEAN_TRUE;		// character enter finished
	CalcAttrib();						// calculate final attribute
	UpdateSpeed();
	// Special Jump criteria
	if (CheckNeedJumpReborn())
	{
		JumpReborn();
		bJumped = true;
		pmap->CB_Error(m_pPeer, m_context, 0, _T("MSG_MAP_FORCE_LEAVE"));
	}
	
	//if (pmap->GetMap_id() == CQuestionControl::GetInstance().GetQuestionMapID())
	//	JumpReborn();

	//check guild war map
	if (pmap->GetMapInfo() && pmap->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR)
	{
		if (!bLocal || (gGlobal.g_GuildWarState != GUILD_WAR_STATE_IN_WAR && gGlobal.g_GuildWarState != GUILD_WAR_STATE_WAR_PREPARE))
		{
			UInt8 line_id = pmap->GetLine_id();					
			JumpMap(gConfigure.guildWarRebornMapID, gConfigure.guildWarRebornMapX, gConfigure.guildWarRebornMapY, line_id);
			bJumped = true;
		}
		else
			ChangeGuildWarTitle();
	}
	
	// check if need to enter instance with full party
	if (!bJumped && info.instId != 0)
		OnEnterInst(info.instId, info.ownerId);

	if (m_pMap)
		RaiseUniqEventID(EVT_MSGDATA_UPDATE);

	CB_ResetChangeLine(GetPeer(), GetContext());
	return BOOLEAN_TRUE;
}

void CCharacter::RemoveDupItemBuff()
{
	for (UInt i = BAG_MAX_ITEM; i < BAG_MAX_ITEM + BAG_EQUIP_COUNT; ++i)
	{
		CItem *item = HelpToGetItem(this, m_pBag, i, 1);
		if (item && item->GetBuffID()) {
			RemoveBuff(item->GetBuffID() ,false);
		}
	}
}

RPCResult CCharacter::DoCharacterLeave(RPCContext &context)
{
	PROCEDURE_START_METHOD2(CCharacter, DoCharacterLeave, this, context, Boolean, bCallback, Boolean, bRemoteJump)

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("DoCharacterLeave: sessionId %s, char_id %d, curPos %d:%d"), 
			context.sessionId.GetString().c_str(), GetChar_id(), GetCurPosX(), GetCurPosY());
		
		if( GetMap() )
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("map_id %d, line_id %d"), m_pMap->GetMap_id(), m_pMap->GetLine_id());
		}
		else
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("null map object"));
		}

		CBattle * pBattle = FindNetObject<CBattle>(m_BattleNetID);
		if (IsState(ST_BATTLE) || pBattle ||  m_bStartBattle)
		{
			TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("inside battle, battle state %d, battle %d, starting battle %d"),
				GetBattleStateID(), pBattle, m_bStartBattle);
			m_sessLogout = BOOLEAN_TRUE;
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		// unblock the map state when character leave 
		UnBlockMapState();

		// character set to leave state
		if (IsState(CCharacter::ST_ENTER_MAP) && !ChangeState(CCharacter::ST_LEAVE_MAP)) 
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, 
				_T("CharacterLeave Warning: Character char_id %d cannot change to LEAVE state"),
				GetChar_id());
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		//check buff update
		CheckBuffUpdate();
		ConstructBuffString();
		if (bRemoteJump)
			RemoveDupItemBuff();
		if (m_pBag && !bRemoteJump)
			m_pBag->RemoveBuffAfterOffline();

		if (m_pParty)
		{
			m_pParty->Leave(this, BOOLEAN_FALSE, BOOLEAN_FALSE);
			m_pParty = NULL;
		}
		if (NULL != m_pTemporaryParty)
		{
			if (m_pTemporaryParty->Leave(this,BOOLEAN_FALSE,BOOLEAN_FALSE)) 
				m_pTemporaryParty = NULL;
		}

		if (m_pQuestion)
			CQuestionControl::GetInstance().OnQuestionExit(m_pQuestion,FALSE);

		CMap* pmap = GetMap();
		OnLeaveMap();
		SetMap(GetMap_id(), GetCurPosX(), GetCurPosY(), 0, GetMap_ownerId());

		
		//check guild war map
		if (pmap && pmap->GetMapInfo() && pmap->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR)
			SetMap(gConfigure.guildWarRebornMapID, gConfigure.guildWarRebornMapX, gConfigure.guildWarRebornMapY, 0, GetMap_ownerId());
		TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("DoCharacterLeave: session_id %s, char_id %d, map_id %d, map_x %d, map_y %d"),
			context.sessionId.GetString().c_str(), GetChar_id(), GetMap_id(), GetCurPosX(), GetCurPosY());

		//leave channel
		
		// sync character and all belongings
		PROCEDURE_DO_METHOD(CCharacter, UpdateAllObjects, this, context)

	PROCEDURE_WAIT_RETURN(1)

		bool bSync = false;

		if (bCallback && m_pPeer) {
			MapService_Stub::CB_LeaveSuccess(m_pPeer, m_context, BOOLEAN_TRUE);
			context.bCallback = BOOLEAN_FALSE;
			MapService_Stub::CB_SyncProxy(m_pPeer, context);
			context.bCallback = BOOLEAN_TRUE;
			bSync = true;					// to ensure sync proxy service between map services
		}
		ClearPeer();

		if (!bSync) {
			PROCEDURE_RETURN
		}
		// prevent no reply of proxy when session id not found
		PROCEDURE_SET_TIMEOUT(2000, BOOLEAN_TRUE)

	PROCEDURE_WAIT(2, MapService, SyncProxy)

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_TIMEOUT
			TRACE_WARNDTL_2(GLOBAL_LOGGER, 
				_T("CharacterLeave Warning: procedure timeout, session_id %s, char_id %d"),
				context.sessionId.GetString().c_str(), GetChar_id());

		PROCEDURE_EXCEPTION_DEFAULT
			// resume procedure even when sub procedure errors
			PROCEDURE_ERROR_BREAK(0)

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}



RPCResult CCharacter::CancelAuto(LPCPEER pPeer, RPCContext &context)
{
	if (m_autoCount > 0) {
		m_autoCount = 0;
		CB_CancelAuto(pPeer, context);
	} else {
		CB_Error(pPeer, context, 1, _T("CCharacter CancelAuto Error: auto count zero"));
		return RPC_RESULT_FAIL;
	}

	return RPC_RESULT_OK;
}

void CCharacter::OnEnterMap(CMap* pmap)
{
	CMapActor::OnEnterMap(pmap);

	//check guild war map
	if (pmap && pmap->GetMapInfo() && pmap->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR)
	{		
		ChangeGuildWarTitle();
	}
}

void CCharacter::OnLeaveMap()
{
	CancelAllRequests();			// cancel all party request
	ClearAllHelpers();
	//m_viewManager->RemoveAllView();
	if (IsTrading())
		ClearTrading();

	if (m_pBag)
		m_pBag->ResetMixShopStartTime();

	if (m_pHawkGroup)
	{
		m_pHawkGroup->CancelHawkNameUpdate();
		m_pHawkGroup->StopHawk();
	}
	CMapActor::OnLeaveMap();
}

RPCResult CCharacter::SetFollowPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id)
{
	CPartner* partner = m_pPartnerGroup->GetPartner(partner_id);
	if (partner && partner->m_data && partner->m_data->req_lv <= GetLevel())
		SetPartner(partner_id, partner->GetMob_id(), partner->GetRaname(), 
			partner->GetOutLookColor());
	else
		SetPartner(0, 0, _T(""), 0);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::SetFollowPet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_id)
{
	SetPet_id(pet_id);
	RaiseUpdate();

	return RPC_RESULT_OK;
}

RPCResult CCharacter::ChangeMapLine(LPCPEER pPeer, RPCContext &context, const UInt8 &line_id)
{
	PROCEDURE_START_RPC1(Character, ChangeMapLine, this, UInt8, line_id)

		if (m_pMap == NULL) {
			CB_WarningMessage(pPeer, context, WARN_CHARACTER_CHANGE_LINE_FAIL, 0, WARN_SHOW_IN_CHATBOX);
			PROCEDURE_RETURN
		} else if (line_id == m_pMap->GetLine_id()) {
			CB_WarningMessage(pPeer, context, WARN_CHARACTER_CHANGE_LINE_FAIL, 0, WARN_SHOW_IN_CHATBOX);
			PROCEDURE_RETURN
		} else if (!IsLeader()){//(m_pParty) {
			CB_WarningMessage(pPeer, context, WARN_PARTY_CANT_CHANGE_LINE, 0, WARN_SHOW_IN_CHATBOX);
			PROCEDURE_RETURN
		} else if (!IsInMap()) {
			CB_WarningMessage(pPeer, context, WARN_CHARACTER_CHANGE_LINE_FAIL, 0, WARN_SHOW_IN_CHATBOX);
			PROCEDURE_RETURN
		} else if (IsTrading()) {
			CB_WarningMessage(pPeer, context, WARN_TRADING_CANT_CHANGE_LINE, 0, WARN_SHOW_IN_CHATBOX);
			PROCEDURE_RETURN
		} else if (IsHawking()) {
			CB_WarningMessage(pPeer, context, WARN_HAWKING_CANT_CHANGE_LINE, 0, WARN_SHOW_IN_CHATBOX);
			PROCEDURE_RETURN
		} else if (InGuildWar()){
			CB_WarningMessage(pPeer, context, WARN_GUILDWAR_CANT_CHANGE_LINE, 0, WARN_SHOW_IN_CHATBOX);
			PROCEDURE_RETURN
		} else if (m_pMap->GetMapInfo() && m_pMap->GetMapInfo()->m_basicinfo &&
					m_pMap->GetMapInfo()->m_basicinfo->m_changelineenabled){
			CB_WarningMessage(pPeer, context, WARN_SPECIFICMAP_CANT_CHANGE_LINE, 0, WARN_SHOW_IN_CHATBOX);
			PROCEDURE_RETURN
		}
		else {
			UInt32 diffTime = GetTickCount() - m_iLastJumpLineTime;
			if (diffTime < (UInt32)(1000 * 60 * gConfigure.changeLineTimeLimit ) ) // default is 5 min
			{
				CB_WarningMessage(pPeer, context, WARN_CHARACTER_CHANGE_LINE_TIME_LIMIT, 0, WARN_SHOW_IN_CHATBOX);
				PROCEDURE_RETURN
			}
		}

		WorldService_Proxy::CanChangeLine(gGlobal.pWorldService->pPeer, context, line_id);

	PROCEDURE_WAIT1(1, WorldService, CB_CanChangeLine, Boolean, canChange)

		if (canChange)
		{
			m_iLastJumpLineTime = GetTickCount();
			CB_WarningMessage(pPeer, context, WARN_CHARACTER_CHANGE_LINE_SUCCESS, 0, WARN_SHOW_IN_CHATBOX);
			MapService_Stub::JumpMap(this, m_pMap->GetMap_id(), line_id, m_pMap->GetOwner_id(),
				GetCurPosX(), GetCurPosY());
		}
		else
			CB_WarningMessage(pPeer, context, WARN_SYS_LINE_FULL, 0, WARN_SHOW_IN_CHATBOX);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("ChangeMsgIcon Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

		PROCEDURE_EXCEPTION_DEFAULT


	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}
Boolean CCharacter::CanJumpMap()
{
	if (!IsLeader())
		return BOOLEAN_FALSE;

	if (m_pScriptSys->IsRunning() || IsInBattle())
		return BOOLEAN_FALSE;
	return BOOLEAN_TRUE;
}
void CCharacter::JumpMap(UInt16 map_id, UInt16 x, UInt16 y, UInt8 line_id, UInt32 ownerId,
						 UInt16 instId,Boolean bCheckLua)
{
	if (!IsLeader())
		return;

	if(bCheckLua && HasLuaFunc(gGlobal.g_pLuaState, "CheckJumpMap"))
	{
		CallLuaFunc(gGlobal.g_pLuaState, "CheckJumpMap", String(), this);
		UInt32 result = (UInt32)gGlobal.g_TempResult;
		if( result == 0 )
			return;
	}

	if (line_id == 0 && ownerId == 0)
	{
		//line_id = 1;
		if (m_pMap)
			line_id = m_pMap->GetLine_id();		
	}

	if (m_pScriptSys->IsRunning() || IsInBattle())
		m_pScriptSys->SetJump(map_id, x, y, line_id, ownerId, instId,bCheckLua);
	else if (m_pMap && IsInMap())
		MapService_Stub::JumpMap(this, map_id, line_id, ownerId, x, y, instId);
}

BOOL CCharacter::CheckNeedJumpReborn()
{
	if (GetMap() && !GetMap()->IsOpen())
		return TRUE;
	return FALSE;
}

VOID CCharacter::JumpReborn(UInt16 map_id, UInt16 x, UInt16 y)
{
	//break the team if jump reborn happen
	if (GetParty())
	{
		//GetParty()->Leave(this);
		if (IsLeader()) 
			GetParty()->SetTempLeaveAll();
		else
			LeaveTeamTemporarily(GetPeer(),GetContext());
	}
	// Compulsory evacuation of a reborn point map causing infinite loop --> go to default jumppoint
	// TRUE  : Function caller try infinite loop
	// FALSE : Function caller try once only.  No retry until successful required.
	if (map_id == 0)
	{
		map_id = GetReborn_map();
		x = GetReborn_x();
		y = GetReborn_y();
	}
	UInt8 line_id = m_pMap ? m_pMap->GetLine_id() : 0;
	JumpMap(map_id, x, y, line_id);
}

void CCharacter::EnterInstance(PCSTRINGA script, UInt16 instId, UInt32 ownerId, 
							   UInt16 map_id, UInt16 x, UInt16 y)
{
	// check if already inside that instance
	if (m_pMap && m_pMap->GetInst_id() == instId)
		return;

	CInstance* pInst = CInstance::GetInstance(instId, ownerId);
	if (pInst)
	{
		UInt16 playerCount = (m_pParty ? m_pParty->GetCount() : 1);
		if (pInst->GetAllowCount() >= playerCount)
			JumpMap(map_id, x, y, 0, ownerId, instId);
		else
			CB_WarningMessage(GetPeer(), GetContext(), WARN_INSTANCE_FULL, 0, WARN_SHOW_IN_CHATBOX);
	}
	else
	{
		String s;
		UTF8ToStr(script, s);
		PROCEDURE_DO_FUNC6(CInstance::JumpInstance, m_context, String, s, 
			UInt16, instId, UInt32, ownerId, UInt16, map_id, UInt16, x, UInt16, y)
	}
}

void CCharacter::EnterEmptyInstance(PCSTRINGA script, UInt16 instId, UInt32 ownerId, 
							   UInt16 map_id, UInt16 x, UInt16 y)
{
	// check if already inside that instance
	if (m_pMap && m_pMap->GetInst_id() == instId)
		return;

	CInstance* pInst = CInstance::GetInstance(instId, ownerId);
	if (pInst)
	{
		String errMsg;
		errMsg = gGlobal.GetStringTable()->Get(_T("MSG_INSTANCE_OCCUPIED_FAIL_ENTRY"));
		CB_AwardMsg(GetPeer(), GetContext(), errMsg);
		
	}
	else
	{
		String s;
		UTF8ToStr(script, s);
		PROCEDURE_DO_FUNC6(CInstance::JumpEmptyInstance, m_context, String, s, 
			UInt16, instId, UInt32, ownerId, UInt16, map_id, UInt16, x, UInt16, y)
	}
}


void CCharacter::LeaveInstance(UInt16 instId, UInt32 ownerId, UInt16 map_id, UInt16 x, UInt16 y,
							   UInt8 line_id)
{
	// check if really inside that instance
	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("character uid %d with action status = %d is leaving instance by jumping into other location with mapId = %d, instance id = %d, lineId = %d, ownerID = %d"), 
		           GetChar_id(), GetAction(), map_id, instId, line_id, ownerId);
	if (m_pMap)
        TRACE_INFODTL_3(GLOBAL_LOGGER, _F("character uid %d is currently located in mapID = %d with instance ID = %d "), GetChar_id(), m_pMap->GetMap_id(), m_pMap->GetInst_id());
	if (m_pMap && m_pMap->GetInst_id() == instId)
		JumpMap(map_id, x, y, line_id);
}

void CCharacter::ResetInstance(UInt16 instId, UInt32 ownerId)
{
	CInstance* pInst = CInstance::GetInstance(instId, ownerId);
	if (pInst)
		pInst->Reset();
}

void CCharacter::CloseInstance(UInt16 instId, UInt32 ownerId)
{
	CInstance::DestroyInstance(instId, ownerId);
}

void CCharacter::OnEnterInst(UInt16 instId, UInt32 ownerId)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("character uid %d, instance id = %d, ownerID = %d"), GetChar_id(), instId, ownerId);

	// check if need to enter instance with full party
	if (instId != 0 && ownerId != 0 && (m_pParty == NULL || m_pParty->IsAllEntered()))
	{
		CInstance* pInst = CInstance::GetInstance(instId, ownerId);
		if (pInst)
			pInst->OnEnter(m_pParty ? m_pParty->GetLeader() : this);
	}
}

void CCharacter::OnLeaveInst(UInt16 instId, UInt32 ownerId)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("character uid %d, instance id = %d, ownerID = %d"), GetChar_id(), instId, ownerId);
	//if(m_pMap == NULL)
	//	return;
	//// invalid map
	//if (m_pMap != FindNetObject<CMap>(m_pMap->GetNetId()))
	//{
	//	TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("[invalid map], map %u, netId %d:%d"),
	//		m_pMap, m_pMap->GetNetId().srvId, m_pMap->GetNetId().objId);
	//	m_pMap = NULL;
	//	return;
	//}

	//// check if need to leave instance with full party
	//if (m_pMap->GetOwner_id() != 0 && ownerId == 0)
	//{
		//CInstance* pInst = CInstance::GetInstance(instId, ownerId);
		//if (pInst)
		//	pInst->OnLeave(m_pParty ? m_pParty->GetLeader() : this);
	//}
}

///////////////////////////////////////////////////////////////////////////////
///																			///
///								Battle methods								///
///																			///
///////////////////////////////////////////////////////////////////////////////

// test battle by client
RPCResult CCharacter::TestBattle(LPCPEER pPeer, RPCContext &context)
{
#ifndef REUBEN_PUBLISH
	// modifed moved steps to ensure encounter battle
	m_movedStep = 20000;
	EncounterBattle(CheckBattle());
#endif // REUBEN_PUBLISH
	return RPC_RESULT_OK;
}

// check battle encounter
UInt32 CCharacter::CheckBattle()
{
	// no battle if battle encounter setting to false
	if (!gConfigure.bBattleEncount)
		return 0;

	// no battle if not in no battle state, not leader and not connected
	if (m_pPeer == NULL || !IsInMap() || IsStartBattle() ||
		m_pParty && m_pParty->GetLeader() != this)
		return 0;

	// check all map data valid
	UInt16 mapId = GetMap_id();
	const MapDB* pMapData = GetMap()->GetMapInfo();
	if (pMapData == NULL || pMapData->m_basicinfo == NULL)
		return 0;

	UInt32 battleNpcId = 0;
	if(HasLuaFunc(gGlobal.g_pLuaState,"GetWantedBattleNpcId"))
	{
		if(GetParty() != NULL)
		{
			for (Index i = C_INDEX(0); i < GetParty()->GetCount(); ++i)
			{
				CCharacter *child = GetParty()->GetChild<CCharacter>(i);
				if (child)
				{
					CallLuaFunc(gGlobal.g_pLuaState,"GetWantedBattleNpcId", String(), child);
					battleNpcId = (UInt32)gGlobal.g_TempResult;
					if(battleNpcId != 0)
						break;
				}
			}
		}
		else
		{
			CallLuaFunc(gGlobal.g_pLuaState,"GetWantedBattleNpcId", String(), this);
			battleNpcId = (UInt32)gGlobal.g_TempResult;
		}
	}

	// check for quest encounter
	if (m_pQuestSys && !m_encounterMap.empty()) {
		EncounterInfo *pReachEnc = NULL;
		UInt16 quest_id = 0;
		for (EncounterMap::iterator it = m_encounterMap.begin(); 
			it != m_encounterMap.end();)
		{
			EncounterInfo &encInfo = it->second;
			// remove encounter if haven't that quest or max encounter reached
			if (!m_pQuestSys->HasQuest(it->first) || encInfo.curEnc >= encInfo.maxEnc) {
				it = m_encounterMap.erase(it);
				continue;
			}
			// inside encounter map: 
			// all, wild, town, cave, wild+town, wild+cave or specify map id
			if (encInfo.map_id == mapId || encInfo.map_id == ENC_MAP_ALL || 
				(encInfo.map_id == ENC_MAP_WILD || encInfo.map_id == ENC_MAP_WILDTOWN ||
				 encInfo.map_id == ENC_MAP_WILDCAVE) && 
				pMapData->m_basicinfo->m_maptype == 0 ||
				(encInfo.map_id == ENC_MAP_TOWN || encInfo.map_id == ENC_MAP_WILDTOWN) &&
				pMapData->m_basicinfo->m_maptype == 1 ||
				(encInfo.map_id == ENC_MAP_CAVE || encInfo.map_id == ENC_MAP_WILDCAVE) &&
				pMapData->m_basicinfo->m_maptype == 2)
			{
				encInfo.moved++;
				if (pReachEnc == NULL && encInfo.moved >= encInfo.maxStep) {
					pReachEnc = &encInfo;
					quest_id = it->first;
				}
			}
			++it;
		}
		if (pReachEnc) {					// reach max step for one of encounter
			CQuestLog* pQuest = m_pQuestSys->GetQuest(quest_id);
			UInt32 npcId = pReachEnc->npc_id;

			TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, 
				_T("Shadow Encounter: char_id %u, quest_id %u, npc_id %u, cur %u, max %d"), 
				GetChar_id(), quest_id, npcId, pReachEnc->curEnc, pReachEnc->maxEnc);
			pReachEnc->moved = 0;			// reset moved step
			if (pReachEnc->maxEnc != UINT16_MAX_VALUE) {		// if limited
				pReachEnc->curEnc++;			// increase encounter count
				if (pQuest) {					// update quest log encounter count
					pQuest->IncEncountCount(pReachEnc->index);	// increase count by 1
					m_pQuestSys->RaiseUpdate();
				}
				if (pReachEnc->curEnc >= pReachEnc->maxEnc)		// reach max
					m_encounterMap.erase(quest_id);
			}
			if(battleNpcId != 0)
				return battleNpcId;
			return npcId;					// return encounter npc id
		}
	}

	
	UInt charPos = GetCurPosY() * pMapData->m_zonedata_header->width + GetCurPosX();
	if (charPos >= pMapData->m_zonedata.size())
	{
		TRACE_ERRORDTL_6(GLOBAL_LOGGER,
			_FE("invalid position, session_id %s, char_id %d, map_id %d, x %d, y %d, pos %d"),
			GetSessionId().GetString().c_str(), GetChar_id(), mapId, GetCurPosX(), GetCurPosY(), charPos);
		return 0;
	}
	ServerMapZoneData* pZoneData = pMapData->m_zonedata[charPos];
	if (pZoneData == NULL || pZoneData->MeetEnemyZoneId <= 0)
		return 0;

	MeetEnemyZone* pMEZone = pMapData->GetMEZone((UInt)pZoneData->MeetEnemyZoneId);
	if (pMEZone == NULL)
		return 0;

	UInt dayTime = gGlobal.GetGameTime(1);
	dayTime = (dayTime * 2) % 24;

	if (pMEZone->encountTotals[dayTime] == 0)
		return 0;

	float levelMod = 1.0f;
	float encounterMod = 1.0f;

	m_movedStep++; // increase normal enemy move step

	if (m_pParty)
		levelMod = pMapData->m_basicinfo->m_maplevel / GetParty()->GetAvgLevel();
	else
		levelMod = pMapData->m_basicinfo->m_maplevel / float(GetLevel());
	levelMod = WITHIN(levelMod, 0.1f, 2.0f);
	UInt16 mapEncRate = pMapData->m_basicinfo->m_zoneencounterrate;
	/*float encRate = encounterMod * MAX(0, m_movedStep - m_stepMod) / MAX(1, mapEncRate); // * levelMod;		// no level modifier
	float HIT = GetRand(1.0f);

	if (encRate > HIT)		// battle happen!
	*/
	m_EncounterBattle = false;
	//m_MobCount = 0;
	CallLuaFunc(gGlobal.g_pLuaState, "CheckEncounterBattle", String(),this,  m_movedStep, 
		m_stepMod, mapEncRate, encounterMod, pMapData->m_basicinfo->m_maplevel);
	if (m_EncounterBattle)
	{	
		DWORD bits = (DWORD)pow((double)2,(int)dayTime);

		m_movedStep = 0;				// reset moved step
		UInt encountCounter = 0;		// random one number from 0 to max encounter
		UInt encountTarget = GetRand(0, pMEZone->encountTotals[dayTime]-1);		// watch where the target is
		for (Index i = C_INDEX(0); i < pMEZone->encountList.size(); ++i)
		{
			const NpcData * pNpc = gGlobal.m_resource->GetNpcData(pMEZone->encountList[i]->NpcId);
			if (pNpc && (pNpc->appearTime & bits) > 0)
			{
				encountCounter += pMEZone->encountList[i]->NpcRate;
				if (encountCounter > encountTarget)
				{
					if(battleNpcId != 0)
						return battleNpcId;
					return pMEZone->encountList[i]->NpcId;
				}
			}
		}
	}

	return 0;
}

void CCharacter::CheckBattleState()
{
	if (IsInBattle())
	{
		CBattle *pBattle = FindNetObject<CBattle>(m_BattleNetID);
		if (pBattle == NULL || !pBattle->PrepareDone())
		{
			ChangeState(CCharacter::ST_NOBATTLE);
			m_BattleNetID = RPCNetID();
			m_pBatChar = NULL;
			SetStartBattle(BOOLEAN_FALSE);
		}
	}
}

void CCharacter::EncounterBattle(UInt32 targetNpc)
{
	if (targetNpc == 0)
		return;

	if (!IsInMap())
		return;

	if (IsStartBattle())
		return;

	if (m_pParty && m_pParty->GetLeader() != this)
		return;

	if (IsHawking())
		return;

	// check that if they are trading, the trading will be cancelled.
	ClearTrading();

	Stop();		// stop moving before battle

	RPCNetID targetNetId;
	// get npc netId if player is talking to that npc
	if (m_pScriptSys->IsRunning() && m_pScriptSys->GetNpcId() == targetNpc &&
		m_pScriptSys->GetNpcNetId().IsValid())
	{
		targetNetId = m_pScriptSys->GetNpcNetId();
	}

	// battle encounter happen
	TRACE_INFODTL_5(GLOBAL_LOGGER, 
		_F("session %s, char_id %d, targetNpc %d, targetNetId %d:%d"),
		m_context.sessionId.GetString().c_str(), GetChar_id(), targetNpc, 
		targetNetId.srvId, targetNetId.objId);

	// check can start battle or not
	if (!CanStartBattle(targetNpc, targetNetId, NPC_BATTLE))
		return;

	PROCEDURE_DO_METHOD3(CCharacter, DoEncounterBattle, this, m_context, UInt32, targetNpc, 
		RPCNetID, targetNetId, UInt8, NPC_BATTLE)
}

Boolean CCharacter::CanStartBattle(UInt32 targetId, RPCNetID targetNetId,
								   UInt8 battleType)
{
	if (targetId == 0)
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("zero target_id, session_id %s, char_id %d, battle_type %d"),
		m_context.sessionId.GetString().c_str(), GetChar_id(), battleType);
		return BOOLEAN_FALSE;
	}

	CCharacter* pTarget = NULL;
	if (battleType != NPC_BATTLE)
		pTarget = FindNetObject<CCharacter>(targetNetId);
		
	if (!IsInMap() || m_bStartBattle || 
		pTarget && (!pTarget->IsInMap() || pTarget->IsStartBattle()))
	{
		TRACE_ERRORDTL_6(GLOBAL_LOGGER, 
			_FE("[character or target not in map], session_id %s, char_id %d, target_id %d, battle_type %d, start_battle %d, target start_battle %d"),
			m_context.sessionId.GetString().c_str(), GetChar_id(), targetId, battleType,
			m_bStartBattle, pTarget ? pTarget->IsStartBattle() : 0);
		return BOOLEAN_FALSE;
	}

	if (m_pParty && !IsLeader() || pTarget && pTarget->HasParty() && !pTarget->IsLeader())
	{
		TRACE_ERRORDT_4(GLOBAL_LOGGER, 
			_FE("[character or target not a leader], session_id %s, char_id %d, target_id %d, battle_type %d"),
			m_context.sessionId.GetString().c_str(), GetChar_id(), targetId, battleType);
		if (m_pParty)
		{
			CCharacter* pLeader = m_pParty->GetLeader();
			TRACE_ERRORL_6(GLOBAL_LOGGER, _F("character party %d:%d:%d, avail %d, leader %d(%s)"),
				m_pParty->GetID(), m_pParty->GetNetId().srvId, m_pParty->GetNetId().objId,
				m_pParty->GetAvail(), pLeader ? pLeader->GetChar_id() : -1,
				pLeader ? pLeader->GetSessionId().GetString().c_str() : _T(""));
		}
		if (pTarget && pTarget->HasParty())
		{
			CParty* pParty = pTarget->GetParty();
			CCharacter* pLeader = pParty->GetLeader();
			TRACE_ERRORL_6(GLOBAL_LOGGER, _F("target party %d:%d:%d, avail %d, leader %d(%s)"),
				pParty->GetID(), pParty->GetNetId().srvId, pParty->GetNetId().objId,
				pParty->GetAvail(), pLeader ? pLeader->GetChar_id() : -1,
				pLeader ? pLeader->GetSessionId().GetString().c_str() : _T(""));
		}
		return BOOLEAN_FALSE;
	}

	return BOOLEAN_TRUE;
}

RPCResult CCharacter::DoEncounterBattle(RPCContext &context)
{
	PROCEDURE_START_METHOD3(CCharacter, DoEncounterBattle, this, context, 
		UInt32, targetId, RPCNetID, targetNetId, UInt8, battleType)

		// double check can start battle or not
		if (!CanStartBattle(targetId, targetNetId, battleType))
		{
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CCharacter* pTarget = NULL;
		if (battleType != NPC_BATTLE)
			pTarget = FindNetObject<CCharacter>(targetNetId);
		String chars;

		// show character ids and sessionId
		if (m_pParty)
		{
			for (Index i = C_INDEX(0); i < m_pParty->GetCount(); i++)
			{
				CCharacter *child = m_pParty->GetChild<CCharacter>(i);
				if (child && !m_pParty->GetM_bTempLeave(i))
				{
					child->SetStartBattle(BOOLEAN_TRUE);
					chars.AppendFormat(_T("%d(%s), "), child->GetChar_id(), 
						child->GetContext().sessionId.GetString().c_str());
				}
			}
		}
		else
		{
			SetStartBattle(BOOLEAN_TRUE);
			chars.AppendFormat(_T("%d(%s), "), GetChar_id(), m_context.sessionId.GetString().c_str());
		}
		
		if (battleType != NPC_BATTLE && pTarget)
		{
			chars.AppendFormat(_T("VS "));
			if (pTarget->GetParty())
			{
				for (Index i = C_INDEX(0); i < pTarget->GetParty()->GetCount(); i++)
				{
					CCharacter *child = pTarget->GetParty()->GetChild<CCharacter>(i);
					if (child && !pTarget->GetParty()->GetM_bTempLeave(i))
					{
						child->SetStartBattle(BOOLEAN_TRUE);
						chars.AppendFormat(_T("%d(%s), "), child->GetChar_id(), 
							child->GetContext().sessionId.GetString().c_str());
					}
				}
			}
			else
			{
				pTarget->SetStartBattle(BOOLEAN_TRUE);
				chars.AppendFormat(_T("%d(%s), "), pTarget->GetChar_id(), 
					pTarget->GetContext().sessionId.GetString().c_str());
			}
		}

		TRACE_INFODTL_5(GLOBAL_LOGGER, 
			_F("battleType %d, targetId %d, targetNetId %d:%d, chars %s"), 
			battleType, targetId, targetNetId.srvId, targetNetId.objId, chars.c_str());

		PROCEDURE_DO_METHOD1(CCharacter, DoReadyBattle, this, context, SessionID, context.sessionId)

	PROCEDURE_WAIT_RETURN(1)

		UInt16 i = 1;
		
	PROCEDURE_FOR1(2, 4, m_pParty && i < m_pParty->GetRealMemNum(), UInt16, i)

		CCharacter *pchar = m_pParty->GetInPartyMember(i);
		TRACE_ENSURE(pchar != NULL);
		
		i++;
		PROCEDURE_STORE_COUNTER1(UInt16, i)

		RPCContext context2(pchar->GetSessionId(), context.pcdId);
		PROCEDURE_DO_METHOD1(CCharacter, DoReadyBattle, pchar, context2, SessionID, context.sessionId)

	PROCEDURE_WAIT_RETURN(3)

	PROCEDURE_END_FOR(4, 2)

		if (battleType == NPC_BATTLE)
		{
			PROCEDURE_GO_STEP(8)
		}

		TRACE_ENSURE(m_pMap);
		CCharacter *pTarget = FindNetObject<CCharacter>(targetNetId);
		if (pTarget == NULL)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("PVP target char_id %d not found"), targetId);
			PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
		}
		RPCContext context2(pTarget->GetSessionId(), context.pcdId);
		PROCEDURE_DO_METHOD1(CCharacter, DoReadyBattle, pTarget, context2, SessionID, context.sessionId)

	PROCEDURE_WAIT_RETURN(5)

		CCharacter *pTarget = FindNetObject<CCharacter>(targetNetId);
		if (pTarget == NULL)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("PVP target char_id %d not found"), targetId);
			PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
		}
		CParty *pParty = pTarget->GetParty();
		UInt16 i = 1;
		
	PROCEDURE_FOR2(6, 8, pParty && i < ((CParty*)pParty)->GetRealMemNum(), UInt16, i, void*, pParty)

		TRACE_ENSURE(pParty);
		CCharacter *pchar = ((CParty*)pParty)->GetInPartyMember(i);
		
		i++;
		PROCEDURE_STORE_COUNTER1(UInt16, i)

		RPCContext context2(pchar->GetSessionId(), context.pcdId);
		PROCEDURE_DO_METHOD1(CCharacter, DoReadyBattle, pchar, context2, SessionID, context.sessionId)

	PROCEDURE_WAIT_RETURN(7)
	
	PROCEDURE_END_FOR(8, 6)

	// it make battle fail when client disconnected at this time
	//PROCEDURE_WAIT_TIMEOUT(9, 100)		// just a little timeout for process other RPC

		TRACE_ENSURE(m_pMap);

		// create battle with npcId
		UInt8 partyCount = (m_pParty ? m_pParty->GetRealMemNum() : 1);
		UInt16 avgLv = GetLevel();
		if (partyCount && m_pParty)
			avgLv = (UInt16) m_pParty->GetAvgLevel();
		CBattle *pBattle = CBattle::NewInstance();
		if (!pBattle->Initialize(battleType, targetId, targetNetId, partyCount, avgLv, this))
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER,
				_FE("battle initialize failed, sessionId %s, char_id %d"),
				context.sessionId.GetString().c_str(), GetChar_id());
			DeleteNetObject(pBattle);
			PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
		}

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("create battle successful, session_id %s, battle %d:%d:%d"), 
			context.sessionId.GetString().c_str(), pBattle->GetID(),
			pBattle->GetNetId().srvId, pBattle->GetNetId().objId);

		// add all characters to battle
		UInt8 team = 1;			// first side is team 1

		pBattle->m_teamData[team].pLeader = this;
		if (m_pParty)
		{
			for (Index i = C_INDEX(0); i < m_pParty->GetCount(); ++i)
			{
				CCharacter *child = m_pParty->GetChild<CCharacter>(i);
				TRACE_ENSURE(child);
				if (m_pParty->GetM_bTempLeave(i))
					continue;
				if (pBattle->AddCharacter(child, team))
				{
					if (child != this && child->GetScript()->IsRunning())
					{
						child->GetScript()->StopScript();
					}
					child->m_BattleNetID = pBattle->GetNetId();
					child->SetStartBattle(BOOLEAN_FALSE);
				}
			}
#ifdef LINEUP_SYSTEM_ON
			if (m_pParty->GetRealMemNum() < TEAMMATE_MAX)
			{
				pBattle->SetLineupId(13,team);
				pBattle->SetLineupKeyPos(0,team);
			}
			else
			{
				pBattle->SetLineupId(m_pParty->GetM_uLineupId(),team);
				pBattle->SetLineupKeyPos(m_pParty->GetM_iLineupKeyPos(),team);
			}
#endif
		}
		else
		{	
			if (pBattle->AddCharacter(this, team))
			{
				m_BattleNetID = pBattle->GetNetId();
				SetStartBattle(BOOLEAN_FALSE);
			}
		}

		// add target id list from character team
		if (battleType != NPC_BATTLE)
		{
			CCharacter *pTarget = FindNetObject<CCharacter>(targetNetId);
			if (pTarget == NULL)
			{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("PVP target char_id %d not found"), targetId);
				PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
			}
			team = 0;			// second side is team 0
			pBattle->m_teamData[team].pLeader = pTarget;
			if (pTarget->GetParty())
			{
				for (Index i = C_INDEX(0); i < pTarget->GetParty()->GetCount(); ++i)
				{
					CCharacter *child = pTarget->GetParty()->GetChild<CCharacter>(i);
					TRACE_ENSURE(child);

					if (pTarget->GetParty()->GetM_bTempLeave(i))
						continue;
					if (pBattle->AddCharacter(child, team))
					{
						if (child->GetScript()->IsRunning())
						{
							child->GetScript()->StopScript();
						}
						child->m_BattleNetID = pBattle->GetNetId();
						child->SetStartBattle(BOOLEAN_FALSE);
					}
				}
#ifdef LINEUP_SYSTEM_ON
				if (pTarget->GetParty()->GetRealMemNum() < TEAMMATE_MAX)
				{
					pBattle->SetLineupId(13,team);
					pBattle->SetLineupKeyPos(0,team);
				}
				else
				{
					pBattle->SetLineupId(pTarget->GetParty()->GetM_uLineupId(),team);
					pBattle->SetLineupKeyPos(pTarget->GetParty()->GetM_iLineupKeyPos(),team);
				}
#endif
			}
			else
			{	
				if (pBattle->AddCharacter(pTarget, team))
				{
					if (pTarget->GetScript()->IsRunning())
						pTarget->GetScript()->StopScript();
					pTarget->m_BattleNetID = pBattle->GetNetId();;
					pTarget->SetStartBattle(BOOLEAN_FALSE);
				}
			}
		}
		else
		{
			CMapNPC* pNpc = FindNetObject<CMapNPC>(targetNetId);
			if (pNpc && pBattle->GetScript())
				pBattle->GetScript()->SetNpc(pNpc);
		}
#ifdef LINEUP_SYSTEM_ON
		pBattle->DoLineupBuffer(0);
		pBattle->DoLineupBuffer(1);
		pBattle->DoLineupAllelopathy();
		pBattle->SetLineupPosition();
#endif

		// preparation done
		if (!pBattle->PrepareDone())
		{
			PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
		}

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("[battle fail], session_id %s, char_id %d, errRpcId %d"),
				context.sessionId.GetString().c_str(), GetChar_id(), pinfo->errRpcId);

			// terminate battle if entered
			CBattle* pBattle = FindNetObject<CBattle>(m_BattleNetID);
			if (pBattle)
			{
				TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("[battle fail], battle %d:%d:%d"),
					pBattle->GetID(), pBattle->GetNetId().srvId, pBattle->GetNetId().objId);
			}

			// all party members exit battle for readyStep >= 1
			//if (readyStep > 0)
			//{
				if (m_pParty)
				{
					for (Index i = C_INDEX(0); i < m_pParty->GetCount(); ++i)
					{
						CCharacter *child = m_pParty->GetChild<CCharacter>(i);
						if (child && !m_pParty->GetM_bTempLeave(i))
							child->CheckBattleState();
					}
				}
				else
				{
					CheckBattleState();
				}
			//}

			// all target members exit battle for readyStep >= 2
			if (battleType != NPC_BATTLE) // && readyStep > 1)
			{
				CCharacter *pTarget = FindNetObject<CCharacter>(targetNetId);
				if (pTarget)
				{
					if (pTarget->GetParty())
					{
						for (Index i = C_INDEX(0); i < pTarget->GetParty()->GetCount(); ++i)
						{
							CCharacter *child = pTarget->GetParty()->GetChild<CCharacter>(i);
							if (child && pTarget->GetParty()->GetM_bTempLeave(i))
								child->CheckBattleState();
						}
					}
					else
					{
						pTarget->CheckBattleState();
					}
				}
			}
			
			// terminate the battle immediately
			if (pBattle)
				pBattle->DeleteBattle();

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}



RPCResult CCharacter::DoReadyBattle(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CCharacter, DoReadyBattle, this, context, SessionID, targetSid)

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("session_id %s, char_id %d, target_session_id %s"), 
			context.sessionId.GetString().c_str(), GetChar_id(), targetSid.GetString().c_str());
		
		if (IsStartBattle()) 
		{
			if (context.sessionId != targetSid)
			{
				Session* psess = GetSession(targetSid);
				if (psess == NULL || psess->pchar == NULL || !psess->pchar->IsInBattle())
				{
					TRACE_WARNDTL_3(GLOBAL_LOGGER, 
						_FW("[target not in battle], session_id %s, char_id %d, target session_id %s"),
						context.sessionId.GetString().c_str(), GetChar_id(), 
						targetSid.GetString().c_str());
					PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
				}
			}

			if (!ChangeState(ST_BATTLE))
			{
				TRACE_WARNDTL_4(GLOBAL_LOGGER, 
					_FW("[cannot change state], session_id %s, char_id %d, battle_state %d, map_state %d"),
					context.sessionId.GetString().c_str(), GetChar_id(), GetBattleStateID(), GetMapStateID());
				PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
			}

			// save the current position before battle
			SetMap(GetMap_id(), GetCurPosX(), GetCurPosY(), 0, GetMap_ownerId());
			m_pScriptSys->SetNoJump();

			CheckBuffUpdate();
			ConstructBuffString();
			ConstructBuffCacheString();
		}
	PROCEDURE_END

	return RPC_RESULT_OK;
}

void CCharacter::BattleHelp(Boolean bNeed)
{
	if (!IsState(CCharacter::ST_BATTLE))
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER,
			_FE("not in battle, char_id %d"),
			GetChar_id());
		return;
	}

	if (bNeed)
		SetAction(ST_NEEDHELP, ::timeGetTime());
	else
		SetAction(ST_BATTLE, ::timeGetTime());
}

RPCResult CCharacter::MonitorBattle(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("session_id %s, char_id %d, target_session_id %d"), 
		m_sessionId.GetString().c_str(), GetChar_id(), targetId);

	CCharacter* pTarget = NULL;
	CBattle* pBattle = NULL;

	// find target character
	if (m_pMap)
	{
		pTarget = CastNetObject<CCharacter>(m_pMap->FindMapActor(Character_CLASSID, targetId));
	}

	if (pTarget == NULL  || !pTarget->IsInBattle())
	{
		CB_Error(pPeer, context, 1, _T("MonitorBattle Error : target character not found or target is not in battle"));
		return RPC_RESULT_FAIL;
	}
	else if (m_pParty)
	{
		CB_Error(pPeer, context, 1, _T("MonitorBattle Error : cannot monitor when in party"));
		return RPC_RESULT_FAIL;
	}
	else if (IsHawking())
	{		
		CB_Error(pPeer, context, 1, _T("MSG_HAWK_HAWKING_CANT_PK"));
		return RPC_RESULT_FAIL;
	}

	pBattle = FindNetObject<CBattle>(m_BattleNetID);
	if (pBattle && pBattle->m_totalMonitor < gConfigure.maxMonitors)
	{
		pBattle->AddMonitor(this, pTarget->m_battleTeam);
	}
	else
	{
		CB_Error(pPeer, context, 1, _T("MonitorBattle Error : monitor target character failed"));
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::PKRequest(LPCPEER pPeer, RPCContext &context, const UInt32 &TargetID, const BYTE &Type)
{
	CONST Int GuildWarVP = 40;
	PROCEDURE_START_RPC2(Character, PKRequest, this, UInt32, TargetID, BYTE, Type)

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("session_id %s, char_id %d, target_char_id %d, type %d"), 
		m_sessionId.GetString().c_str(), GetChar_id(), TargetID, Type);

	BYTE Result = PK_REQUEST_RESULT_SUCCESS;
	UInt32 ResultId = 0;
	bool bBattleNow = false;
	bool bMonitor = false;
	CCharacter * pTarget = NULL;
	LPCPEER pTargetPeer = NULL;
	RPCContext* pTargetContext = NULL;
	UInt8 teamCount  = 1;
	UInt8 battleType = Type;
	if (GetParty())
		teamCount = (UInt8)GetParty()->GetRealMemNum();

	do
	{
		// check if self in battle
		if (IsInBattle())
		{
			Result = PK_REQUEST_RESULT_FAIL_IN_BATTLE_SELF;
			ResultId = 0;
			break;
		}

		// find target character first
		if (m_pMap)
		{
			pTarget = CastNetObject<CCharacter>(m_pMap->FindMapActor(Character_CLASSID, TargetID));
			if (m_pMap->GetMapInfo() && m_pMap->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR)
				battleType = GUILD_WAR_BATTLE;
		}

		if (pTarget == NULL)
		{
			Result = PK_REQUEST_RESULT_FAIL_NOT_FOUND_TARGET;
			ResultId = 0;
			break;
		}

		if(battleType == FORCE_PK_BATTLE && HasLuaFunc(gGlobal.g_pLuaState, "TargetIsEnablePK"))
		{
			CallLuaFunc(gGlobal.g_pLuaState, "TargetIsEnablePK", String(), pTarget);
			UInt32 result = (UInt32)gGlobal.g_TempResult;
			if(result == 0)
			{
				Result = PK_REQUEST_RESULT_FAIL_TARGET_NOT_ENABLE;
				break;
			}
		}

		//check whether he or u are resting
		if (IsResting()) 
		{
			Result = PK_REQUEST_RESULT_FAIL_RESTING;
			ResultId = GetChar_id();
			break;
		}
		
		if (pTarget->IsResting())
		{
			Result = PK_REQUEST_RESULT_FAIL_RESTING;
			ResultId = pTarget->GetChar_id();
			break;
		}

		if (pTarget->IsHawking())
		{
			Result = PK_REQUEST_RESULT_FAIL_TARGET_HAWKING;
			ResultId = pTarget->GetChar_id();
			break;
		}

		// auto target to leader if in party
		if (pTarget->GetParty())
			pTarget = pTarget->GetParty()->GetLeader();

		if (pTarget == this)
		{
			Result = PK_REQUEST_RESULT_FAIL_CANNOT_TARGET_SELF;
			ResultId = GetChar_id();
			break;
		}

		if (pTarget->m_pMap != m_pMap)
		{
			Result = PK_REQUEST_RESULT_FAIL_NOT_BATTLEFIELD_TARGET;
			ResultId = pTarget->GetChar_id();
			break;
		}

		if (battleType == REJECT_MATCH_BATTLE)
		{
			pTargetPeer = pTarget->GetPeer();
			pTargetContext = &pTarget->GetContext();
			Result = PK_REQUEST_RESULT_MATCH_BATTLE_FAIL_REJECT;
			m_MatchCharID = 0;
			pTarget->m_MatchCharID = 0;
			ResultId = GetChar_id();
			break;
		} 

		if (IsTrading())
		{
			Result = PK_REQUEST_RESULT_FAIL_TARGET_BUSY;
			ResultId = GetChar_id();
			break;
		}

		if (pTarget->IsTrading())
		{
			Result = PK_REQUEST_RESULT_FAIL_TARGET_BUSY;
			ResultId = pTarget->GetChar_id();
			break;
		}

		//find whether monitor the battle or not
		CBattle * pTargetBattle = FindNetObject<CBattle>(pTarget->m_BattleNetID);
		if (pTargetBattle && (pTarget->IsState(ST_BATTLE) || (pTarget->IsState(ST_MONITOR) && battleType != GUILD_WAR_BATTLE) ))
		{	
			Result = PK_REQUEST_RESULT_FAIL_IN_BATTLE_TARGET;
			//monitor the battle
			if (m_pParty == NULL)
			{
				m_battleTeam = pTarget->m_battleTeam;
				if (pTargetBattle->m_totalMonitor < gConfigure.maxMonitors)
				{
					bMonitor = true;
					pTargetBattle->AddMonitor(this, pTarget->m_battleTeam);
				}
				else
					Result = PK_REQUEST_RESULT_FAIL_MONITOR_FULL;
			}
			break;
		}

		if (Result != PK_REQUEST_RESULT_SUCCESS)
		{
			ResultId = pTarget->GetChar_id();
			break;
		}

		//check zone data here
		

		//check same guild if it is in guild war map
		if (GetMap() && GetMap()->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR)
		{ 
			if (GetGuildUid() == pTarget->GetGuildUid())
			{
				Result = PK_REQUEST_RESULT_FAIL_SAME_GUILD;
				ResultId = pTarget->GetChar_id();
				m_MatchCharID = 0;
				if (pTarget->m_MatchCharID == GetChar_id())
					pTarget->m_MatchCharID = 0;
				break;
			}
			else if (gGlobal.g_GuildWarState != GUILD_WAR_STATE_IN_WAR)
			{
				Result = PK_REQUEST_RESULT_FAIL_WAR_NOT_START;
				ResultId = pTarget->GetChar_id();
				m_MatchCharID = 0;
				if (pTarget->m_MatchCharID == GetChar_id())
					pTarget->m_MatchCharID = 0;
				break;
			}

			battleType = GUILD_WAR_BATTLE;
		}
		else if (GetMap() && GetMap()->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_FREE_PK)
		{
			battleType = FREE_PK_BATTLE;
		}
		else
		{
			//check attacker
			UInt zonepos			= GetCurPosX() + m_pMap->GetMapInfo()->m_zonedata_header->width * GetCurPosY();
			UInt final_pk_value		= PK_VALUE_NOT_ALLOW;
			WORD tempvalue			= 0;
			MapDB* tmpmapdata	= m_pMap->GetMapInfo();
			if (zonepos < tmpmapdata->m_zonedata.size())
			{
				// loop all zone id of current block
				for( Index i = C_INDEX(0); i < (UInt)tmpmapdata->m_zonedata[zonepos]->ZoneIdCount; ++i )
				{
					WORD& zoneId = tmpmapdata->m_zonedata[zonepos]->ZoneIdLst[i];
					if( tmpmapdata->m_zoneinfo[zoneId] )
					{
						tempvalue = tmpmapdata->m_zoneinfo[zoneId]->zone_PK;
						// confirmed by Ah To, that final value of any field in zone is obtained by its maximum value
						if( tempvalue > (UInt)final_pk_value )
							final_pk_value = tempvalue;
						continue;
					}
					TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("attacker zone null, pk set false, zonepos=%d id=%d"), zonepos, zoneId);
				}
			}
			else
			{
				TRACE_ERRORDTL_6(GLOBAL_LOGGER,
					_FE("invalid position, session_id %s, char_id %d, map_id %d, x %d, y %d, pos %d"),
					GetSessionId().GetString().c_str(), GetChar_id(), GetMap_id(), GetCurPosX(), GetCurPosY(), zonepos);
			}

			if (final_pk_value == PK_VALUE_NOT_ALLOW)
			{
				Result = PK_REQUEST_RESULT_FAIL_NOT_BATTLEFIELD_SELF;
				ResultId = pTarget->GetChar_id();
				m_MatchCharID = 0;
				if (pTarget->m_MatchCharID == GetChar_id())
					pTarget->m_MatchCharID = 0;
				break;
			}

			UInt Def_zonepos = pTarget->GetCurPosX() + pTarget->m_pMap->GetMapInfo()->m_zonedata_header->width * pTarget->GetCurPosY();
			UInt Def_final_pk_value = PK_VALUE_NOT_ALLOW;
			WORD Def_tempvalue = 0;
			tmpmapdata = pTarget->m_pMap->GetMapInfo();
			// loop all zone id of current block
			if (Def_zonepos < tmpmapdata->m_zonedata.size())
			{
				for( Index i = C_INDEX(0); i < (UInt)tmpmapdata->m_zonedata[Def_zonepos]->ZoneIdCount; ++i )
				{
					WORD& zoneId = tmpmapdata->m_zonedata[Def_zonepos]->ZoneIdLst[i];
					if( tmpmapdata->m_zoneinfo[zoneId] )
					{
						Def_tempvalue = tmpmapdata->m_zoneinfo[zoneId]->zone_PK;
						// confirmed by Ah To, that final value of any field in zone is obtained by its maximum value
						if( Def_tempvalue > (UInt)Def_final_pk_value )
							Def_final_pk_value = tempvalue;
						continue;
					}
					TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("defender zone null, pk set false, zonepos=%d id=%d"), zonepos, zoneId);
				}
			}
			else
			{
				TRACE_ERRORDTL_6(GLOBAL_LOGGER,
					_FE("invalid position, session_id %s, char_id %d, map_id %d, x %d, y %d, pos %d"),
					pTarget->GetSessionId().GetString().c_str(), pTarget->GetChar_id(), pTarget->GetMap_id(), pTarget->GetCurPosX(), pTarget->GetCurPosY(), Def_zonepos);
			}

			if (Def_final_pk_value != final_pk_value)
			{
				Result = PK_REQUEST_RESULT_FAIL_NOT_BATTLEFIELD_TARGET;
				ResultId = pTarget->GetChar_id();
				m_MatchCharID = 0;
				if (pTarget->m_MatchCharID == GetChar_id())
					pTarget->m_MatchCharID = 0;
				break;
			}

			if (final_pk_value == PK_VALUE_MATCH || 
			final_pk_value == PK_VALUE_LOW_MARTIAL_PK ||
			final_pk_value == PK_VALUE_HIGH_MARTIAL_PK)
			{
				//check whether the player is monitoring

				if (battleType == PK_BATTLE)
				{
					if (m_MatchCharID != 0)
					{
						CCharacter * pPreChar = CastNetObject<CCharacter>(m_pMap->FindMapActor(Character_CLASSID, m_MatchCharID));
						if (pPreChar)
						{
							if (pPreChar == pTarget)
							{
								Result = PK_REQUEST_RESULT_FAIL_TARGET_ALREADY_ASKED;
								ResultId = GetChar_id();
								break;
							}
							else
							{
								Result = PK_REQUEST_RESULT_MATCH_BATTLE_FAIL_TARGET_CHANGE;
								ResultId = GetChar_id();
								pTargetPeer = pPreChar->GetPeer();
								pTargetContext = &pPreChar->GetContext();
								//break;
								if (pTargetPeer && pTargetContext)
									pPreChar->CB_PKRequest(pTargetPeer, *pTargetContext, Result, ResultId, teamCount);
							}
						}

					}
					m_MatchCharID = pTarget->GetChar_id();
					if (final_pk_value == PK_VALUE_MATCH)
						Result = PK_REQUEST_RESULT_MATCH_BATTLE_REQUEST;
					else if (final_pk_value == PK_VALUE_LOW_MARTIAL_PK)
						Result = PK_REQUEST_RESULT_LOW_MARTIAL_BATTLE_REQUEST;
					else
						Result = PK_REQUEST_RESULT_HIGH_MARTIAL_BATTLE_REQUEST;
					ResultId = GetChar_id();
					pTargetPeer = pTarget->GetPeer();
					pTargetContext = &pTarget->GetContext();

				}
				else if (battleType == MATCH_BATTLE ||
					battleType == LOW_MARTIAL_BATTLE ||
					battleType == HIGH_MARTIAL_BATTLE)
				{
					if (pTarget->m_MatchCharID != GetChar_id())
					{
						Result = PK_REQUEST_RESULT_MATCH_BATTLE_FAIL_TARGET_CHANGE;
						ResultId = pTarget->GetChar_id();
						break;
					}

					if (final_pk_value == PK_VALUE_LOW_MARTIAL_PK)
						battleType = LOW_MARTIAL_BATTLE;
					else if (final_pk_value == PK_VALUE_HIGH_MARTIAL_PK)
						battleType = HIGH_MARTIAL_BATTLE;

				}
			}
			else if(!GetEnablePK())
			{
				Result = PK_REQUEST_RESULT_FAIL_NO_PK_ENABLE_SELF;
				break;
			}
		}

		

		if (battleType == PK_BATTLE)
		{
			if (CheckBlockPK())
				Result = PK_REQUEST_RESULT_FAIL_BLOCK_PK;
			else if (pTarget->CheckProtectedPK())
				Result = PK_REQUEST_RESULT_FAIL_PROTECT_PK;
		}
		else if (m_pParty && m_pParty->GetLeader() != this)
		{
			Result = PK_REQUEST_RESULT_FAIL_NOT_LEADER_SELF;
			break;
		}
		else if ((battleType == PK_BATTLE || battleType == FORCE_PK_BATTLE)&& !GetEnablePK())
		{
			Result = PK_REQUEST_RESULT_FAIL_NO_PK_ENABLE_SELF;
			break;
		}
		else if ((battleType != MATCH_BATTLE && battleType != LOW_MARTIAL_BATTLE && battleType != HIGH_MARTIAL_BATTLE) && GetLevel() < PK_LEVEL_LIMIT)
		{
			Result = PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_LV_SELF;
			break;
		}
		else if (m_battleState->GetStateID() == CCharacter::ST_BATTLE)
		{
			Result = PK_REQUEST_RESULT_FAIL_IN_BATTLE_SELF;	
			break;
		}
		else if ((battleType == LOW_MARTIAL_BATTLE && GetMoney() < LOW_MARTIAL_MONEY) || 
			(battleType == HIGH_MARTIAL_BATTLE && GetMoney() < HIGH_MARTIAL_MONEY))
		{
			Result = PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_MONEY;
			String errMsg;
			errMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_MARTIAL_MONEY_NOT_ENOUGH")), GetNickName().c_str());
			CB_Error(pPeer, context, 0, errMsg);
			pTarget->CB_Error(pTarget->GetPeer(), pTarget->GetContext(), 0, errMsg);
			m_MatchCharID = 0;
			if (pTarget->m_MatchCharID == GetChar_id())
				pTarget->m_MatchCharID = 0;
			break;
		}


		if (Result == PK_REQUEST_RESULT_SUCCESS)
		{
			//check target zone data here
			//.......

			UInt TargetCount = 1;
			if (pTarget->GetParty() && pTarget->GetParty()->GetLeader() != pTarget)
				pTarget = pTarget->GetParty()->GetLeader();

			CParty* pParty = pTarget->GetParty();
			Boolean bHasRedNameMember = BOOLEAN_FALSE;
			if(pParty)
			{
				for (Index i = C_INDEX(0); i < pParty->GetCount(); ++i)
				{
					if (pParty->GetM_bTempLeave(i))
						continue;
					CCharacter *child = pParty->GetChild<CCharacter>(i);
					if (child && child->GetPK_Value() > 0)
					{
						bHasRedNameMember = BOOLEAN_TRUE;
						break;
					}
				}
			}

			if (battleType == PK_BATTLE && !pTarget->GetEnablePK() && !bHasRedNameMember)
				Result = PK_REQUEST_RESULT_FAIL_NO_PK_ENABLE_TARGET;
			else if (battleType != MATCH_BATTLE && pTarget->GetLevel() < PK_LEVEL_LIMIT)
				Result = PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_LV_TARGET;
			else if ((battleType == LOW_MARTIAL_BATTLE && pTarget->GetMoney() < LOW_MARTIAL_MONEY) ||
				(battleType == HIGH_MARTIAL_BATTLE && pTarget->GetMoney() < HIGH_MARTIAL_MONEY))
			{
				Result = PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_MONEY;
				m_MatchCharID = 0;
				if (pTarget->m_MatchCharID == GetChar_id())
					pTarget->m_MatchCharID = 0;
				String errMsg;
				errMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_MARTIAL_MONEY_NOT_ENOUGH")), pTarget->GetNickName().c_str());
				CB_Error(pPeer, context, 0, errMsg);
				pTarget->CB_Error(pTarget->GetPeer(), pTarget->GetContext(), 0, errMsg);

			}

			if (Result == PK_REQUEST_RESULT_SUCCESS)
			{
				//defender team

				if (pTarget->GetParty())
				{
					StlVector<CCharacter *> RmLst;
					for (Index i = C_INDEX(0); i < pTarget->GetParty()->GetCount(); ++i)
					{
						if (pTarget->GetParty()->GetM_bTempLeave(i))
							continue;
						CCharacter * member = pTarget->GetParty()->GetChild<CCharacter>(i);
						if (member) {
							BOOL PKOK = TRUE;
							//if (battleType == PK_BATTLE && !member->GetEnablePK())
							//	PKOK = FALSE;
							if (battleType != MATCH_BATTLE && member->GetLevel() < PK_LEVEL_LIMIT)
								PKOK = FALSE;
							else if ((battleType == LOW_MARTIAL_BATTLE && member->GetMoney() < LOW_MARTIAL_MONEY) ||
								(battleType == HIGH_MARTIAL_BATTLE && member->GetMoney() < HIGH_MARTIAL_MONEY))
							{
								PKOK = FALSE;
								String errMsg;
								errMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_MARTIAL_MONEY_NOT_ENOUGH")), member->GetNickName().c_str());
								CB_Error(pPeer, context, 0, errMsg);
								pTarget->CB_Error(pTarget->GetPeer(), pTarget->GetContext(), 0, errMsg);
								Result = PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_MONEY;
								m_MatchCharID = 0;
								if (pTarget->m_MatchCharID == GetChar_id())
									pTarget->m_MatchCharID = 0;
								break;
							}
							else if (member->m_battleState->GetStateID() == CCharacter::ST_BATTLE)
								PKOK = FALSE;
							else if (battleType == GUILD_WAR_BATTLE && member->IsResting())
							{								
								Result = PK_REQUEST_RESULT_FAIL_RESTING;
								ResultId = member->GetChar_id();
								m_MatchCharID = 0;
								if (member->m_MatchCharID == GetChar_id())
									member->m_MatchCharID = 0;								
								break;								
							}

							if (!PKOK)
								RmLst.push_back(member);
						}
					}

					for (Int i = (Int)RmLst.size() - 1; i >= 0; i --)
					{
						if (pTarget->GetParty())
							pTarget->GetParty()->Leave(RmLst[i]);
					}
				}

				if (pTarget->GetParty())
					TargetCount = pTarget->GetParty()->GetRealMemNum();

				//kick out all not qualify players
				if (battleType == FORCE_PK_BATTLE && GetHonor() < gConfigure.forcePkHonorConsume)
				{
					String errMsg;
					errMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_HONOR_SELF")), gConfigure.forcePkHonorConsume);
					CB_Error(pPeer, context, 0, errMsg);
					Result = PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_HONOR_SELF;
				}
				else
				{
					//remove watch
					CBattle * pTargetBattle = FindNetObject<CBattle>(pTarget->m_BattleNetID);
					if (pTargetBattle && pTarget->IsState(ST_MONITOR))
						pTargetBattle->RemoveMonitor(pTarget);

					//attacker team
					if (m_pParty)
					{
						StlVector<CCharacter *> RmLst;
						for (Index i = C_INDEX(0); i < m_pParty->GetCount(); ++i)
						{
							if (m_pParty->GetM_bTempLeave(i))
								continue;
							CCharacter * member = m_pParty->GetChild<CCharacter>(i);
							if (member) {
								BOOL PKOK = TRUE;
								if (battleType == PK_BATTLE && !member->GetEnablePK())
									PKOK = FALSE;
								else if ((battleType != MATCH_BATTLE && battleType != LOW_MARTIAL_BATTLE && battleType != HIGH_MARTIAL_BATTLE) && member->GetLevel() < PK_LEVEL_LIMIT)
									PKOK = FALSE;
								else if ((battleType == LOW_MARTIAL_BATTLE  && member->GetMoney() < LOW_MARTIAL_MONEY) ||
									(battleType == HIGH_MARTIAL_BATTLE  && member->GetMoney() < HIGH_MARTIAL_MONEY))
								{
									PKOK = FALSE;
									String errMsg;
									errMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_MARTIAL_MONEY_NOT_ENOUGH")), member->GetNickName().c_str());
									CB_Error(pPeer, context, 0, errMsg);
									pTarget->CB_Error(pTarget->GetPeer(), pTarget->GetContext(), 0, errMsg);
									Result = PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_MONEY;
									m_MatchCharID = 0;
									if (pTarget->m_MatchCharID == GetChar_id())
										pTarget->m_MatchCharID = 0;
									break;
								}
								else if (battleType == GUILD_WAR_BATTLE)									
								{								
									if (member->IsResting())
									{
										Result = PK_REQUEST_RESULT_FAIL_RESTING;
										ResultId = member->GetChar_id();
										m_MatchCharID = 0;
										if (member->m_MatchCharID == GetChar_id())
											member->m_MatchCharID = 0;
										PKOK = FALSE;
										break;								
									}
									else if (member->GetVP() < GuildWarVP)
									{
										Result = PK_REQUEST_RESULT_FAIL_VP_NOT_ENOUGH;
										ResultId = member->GetChar_id();
										m_MatchCharID = 0;
										if (member->m_MatchCharID == GetChar_id())
											member->m_MatchCharID = 0;
										PKOK = FALSE;
										break;								
										
									}
									else
										member->ChangeVP(-GuildWarVP);
								}
								else if (member->m_battleState->GetStateID() == CCharacter::ST_BATTLE)
									PKOK = FALSE;

								if (!PKOK)
									RmLst.push_back(member);
							}
						}

						for (Int i = 0; i < (Int)RmLst.size(); i ++)
						{
							if (m_pParty)
								m_pParty->Leave(RmLst[i]);
						}
					}
					else if (battleType == GUILD_WAR_BATTLE)
					{
						if (GetVP() < GuildWarVP)
						{
							Result = PK_REQUEST_RESULT_FAIL_VP_NOT_ENOUGH;
							ResultId = GetChar_id();
							break;								
						}
						else 
							ChangeVP(-GuildWarVP);
					}
					if (battleType == FORCE_PK_BATTLE)
					{
						ChangeHonor(-Int32(gConfigure.forcePkHonorConsume));
					}
					//create battle
					bBattleNow = Result == PK_REQUEST_RESULT_SUCCESS;
				}
			}
		}
		

		
	}
	while (false);

	// all callback to original client
	CB_PKRequest(pPeer, context, 
		(Result == PK_REQUEST_RESULT_MATCH_BATTLE_REQUEST ||
		Result == PK_REQUEST_RESULT_LOW_MARTIAL_BATTLE_REQUEST ||
		Result == PK_REQUEST_RESULT_HIGH_MARTIAL_BATTLE_REQUEST) ? PK_REQUEST_RESULT_SUCCESS : Result,
		ResultId, teamCount);

	// callback to target client
	if (pTargetPeer && pTargetContext)
		pTarget->CB_PKRequest(pTargetPeer, *pTargetContext, Result, ResultId, teamCount);

	if (bBattleNow && pTarget)
	{
		if (m_pParty)
		{
			for (Index i = C_INDEX(0); i < m_pParty->GetCount(); ++i)
			{
				CCharacter *child = m_pParty->GetChild<CCharacter>(i);
				if (m_pParty->GetM_bTempLeave(i))
					continue;
				if(child != NULL&&(battleType == PK_BATTLE || battleType == FORCE_PK_BATTLE) && !child->GetEnablePK())
				{
					m_pParty->Leave(child);
				}
			}
		}
		PROCEDURE_DO_METHOD3(CCharacter, DoEncounterBattle, this, context, 
			UInt32, pTarget->GetChar_id(), RPCNetID, pTarget->GetNetId(), UInt8, battleType)
	}
	else
	{
		PROCEDURE_RETURN
	}

	PROCEDURE_WAIT_RETURN(1)

		PROCEDURE_END

	return RPC_RESULT_OK;
}

void CCharacter::SetEnablePKEx(Boolean enable)
{
	SetBattleInfo(enable);
	CB_SetEnablePK(m_pPeer, m_context, enable);
	if(enable)
	{
		SetMark(GetMark() | (1 << MARK_ENABLE_PK));
	}
	else
	{	
		SetMark(GetMark() & ~(1 << MARK_ENABLE_PK));
	}

	if (m_pMap)
		RaiseUniqEventID(EVT_MSGDATA_UPDATE);
	SendAttr(m_pPeer, m_context,GetUpdateMask());
}


//RPCResult CCharacter::SetEnablePK(LPCPEER pPeer, RPCContext &context, const Boolean &Value)
//{
//	SetEnablePK(Value);
//	return RPC_RESULT_OK;
//}

RPCResult CCharacter::JoinBattle(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	PROCEDURE_START_RPC1(Character, JoinBattle, this, UInt32, char_id)

		BYTE result = JOIN_BATTLE_OK;
		CCharacter* pHelper = NULL;
		CBattle * pBattle = FindNetObject<CBattle>(m_BattleNetID);
		if (pBattle == NULL)
		{
			PROCEDURE_RETURN
		}

		do
		{
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("session_id %s, char_id %d, helper char_id %d"), 
				m_sessionId.GetString().c_str(), GetChar_id(), char_id);

			if (!IsInBattle())
			{
				TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("character not in battle, char_id %d, sessionId %s, helper char_id %d"), 
					GetChar_id(), context.sessionId.GetString().c_str(), char_id);
				result = JOIN_BATTLE_INVALID;
				break;
			}

			HelperList::iterator it = m_HelperList.find(char_id);
			if (it != m_HelperList.end())
				pHelper = it->second.pHelperChar;

			if (pHelper == NULL || m_BattleNetID == RPCNetID() || !pHelper->TryState(ST_BATTLE) ||
				pHelper->GetParty())
			{
				result = JOIN_BATTLE_INVALID;
				break;
			}
			SetMap(GetMap_id(), GetCurPosX(), GetCurPosY(), 0, GetMap_ownerId());
			if (NeedUpdate())				// update to object service
				RaiseUpdate();

		} while (false);

		PROCEDURE_STORE_VALUE1(BYTE, result)
		if (result != JOIN_BATTLE_OK) {
			PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
		}

		CBattle * pHelperBattle = FindNetObject<CBattle>(pHelper->m_BattleNetID);
		//CBattle * pBattle = FindNetObject<CBattle>(m_BattleNetID);
		if (pHelperBattle && pHelperBattle != pBattle)
		{
			PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
		}

		RPCContext context2(pHelper->GetSessionId(), context.pcdId);
		PROCEDURE_DO_METHOD1(CCharacter, DoReadyBattle, pHelper, context2, SessionID, context.sessionId);

	PROCEDURE_WAIT_RETURN(1)

		BYTE result = JOIN_BATTLE_OK;
		CCharacter* pHelper = CastNetObject<CCharacter>(
								m_pMap->FindMapActor(Character_CLASSID, char_id));

		LPCPEER pBattlePeer = NULL;

		do {
			if (m_BattleNetID == RPCNetID()) {
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("JoinBattle: target char_id %d not in battle"), 
					GetChar_id());
				result = JOIN_BATTLE_INVALID;
				break;
			}
			if (pHelper == NULL) { 
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("JoinBattle: helper char_id %d not found"),
					char_id);
				result = JOIN_BATTLE_INVALID;
				break;
			}
			//create party or join party
			if (m_pParty == NULL && !CreateParty(BOOLEAN_TRUE)) {
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("JoinBattle: create party failed, char_id %d"),
					GetChar_id());
				result = JOIN_BATTLE_INVALID;
				break;
			} else if (!m_pQuestSys->CheckJoinParty() || !pHelper->GetQuests()->CheckJoinParty()) {
				TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("cannot join party char_id %d, helper char_id %d"),
					GetChar_id(), pHelper->GetChar_id());
				result = JOIN_BATTLE_INVALID;
				break;
			}

			if (m_pBag && m_pBag->HaveBusinessItemOrMoney())
			{
				CB_Error(pPeer, context, 1, _T("MSG_PARTY_HAVE_BUSINESS_ITEM"));
				result = JOIN_BATTLE_INVALID;
				break;
			}

			if (m_pParty->IsMember(pHelper)) 
			{//temporarily leave member return party first
				pHelper->ReturnParty(pHelper->GetPeer(),pHelper->GetContext());
			}
			else if (m_pParty->Join(pHelper, BOOLEAN_TRUE, BOOLEAN_TRUE))
			{
				m_pParty->PreSend();
				pHelper->ClearSendMask();
				pHelper->CB_JoinParty(pHelper->GetPeer(), pHelper->GetContext(),
					m_pParty);
			} else{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("JoinBattle: helper char_id %d join party failed"),
					pHelper->GetChar_id());
				result = JOIN_BATTLE_FAIL_FULL;
				break;
			}
		} while (false);

		PROCEDURE_STORE_VALUE1(BYTE, result)
		if (result != JOIN_BATTLE_OK) {
			PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
		}

//		if (m_pScriptSys)
//			m_pScriptSys->StopScript();

		RpcCharVec charList;
		RpcCharInfo charInfo;
		UInt32Vector partnerIds;

		pHelper->SetCharInfo(&charInfo, partnerIds);
		charInfo.team = m_battleTeam;
		charList.push_back(charInfo);

		
		pHelper->m_BattleNetID = RPCNetID();

		CBattle * pBattle = FindNetObject<CBattle>(m_BattleNetID);
		if (pBattle == NULL || !pBattle->AddCharacter(pHelper, m_battleTeam)) {
			PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
		}

		//sync to all characters that the leader has select people to join	
		for (HelperList::iterator it = m_HelperList.begin(); it != m_HelperList.end(); ++it)
		{
			if (it->first != char_id) {
				CCharacter* pHelper = it->second.pHelperChar;
				CB_JoinBattle(pHelper->GetPeer(), pHelper->GetContext(), JOIN_BATTLE_ALREADYJOIN);
			}
		}

		SetAction(ST_BATTLE, ::timeGetTime());
		pHelper->SetAction(ST_BATTLE, ::timeGetTime());
		
		ClearAllHelpers();
		CB_ClearHelpersList(pPeer, context);
		CB_JoinBattle(pPeer, context, JOIN_BATTLE_OK);
		if (m_pMap) {
			CCharacter* pHelper = CastNetObject<CCharacter>(
					m_pMap->FindMapActor(Character_CLASSID, char_id));
			if (pHelper) {
				pHelper->CB_JoinBattle(pHelper->GetPeer(), pHelper->GetContext(), JOIN_BATTLE_OK);
			}
		}

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT

			PROCEDURE_RESTORE_VALUE1(BYTE, result)
			if (result == JOIN_BATTLE_OK)
				result = JOIN_BATTLE_INVALID;
			if (m_pMap) {
				CCharacter* pchar = CastNetObject<CCharacter>(
						m_pMap->FindMapActor(Character_CLASSID, char_id));
				if (pchar) {
					if (pchar->IsState(ST_BATTLE)) {
						CBattle * pBattle = FindNetObject<CBattle>(pchar->m_BattleNetID);
						if (pBattle)
							pchar->ChangeState(ST_MONITOR);
						else 
							pchar->ChangeState(ST_NOBATTLE);
					}
					if (pchar->GetParty() && pchar->GetParty() == m_pParty)
						pchar->GetParty()->Leave(pchar);
					pchar->CB_JoinBattle(pchar->GetPeer(), pchar->GetContext(), 
						result);
				}
			}
			CB_JoinBattle(pPeer, context, result);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::RequestJoinBattle(LPCPEER pPeer, RPCContext &context, 
										const Boolean &Force)
{
	BYTE result = REQUEST_JOIN_BATTLE_OK;
	CCharacter* pChar = NULL;
 
	CBattle * pBattle = FindNetObject<CBattle>(m_BattleNetID);
	if (pBattle)
		pChar = pBattle->m_teamData[m_battleTeam].pLeader;

	if (pBattle == NULL || pChar == NULL || !pChar->IsInBattle() || 
		pChar->GetAction() != CCharacter::ST_NEEDHELP || !IsState(ST_MONITOR) ||
		pBattle->GetBattleType() != NPC_BATTLE)
	{ 
		result = REQUEST_JOIN_BATTLE_FAIL;
	}
	else
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("RequestJoinBattle: char_id %d, sessionId %s, target char_id %d"), 
			GetChar_id(), m_sessionId.GetString().c_str(), pChar->GetChar_id());
		HelperList::iterator it = pChar->m_HelperList.find(GetChar_id());
		if (it != pChar->m_HelperList.end())
		{
			if (::timeGetTime() < it->second.rejectTime + HELPER_REJECT_DELAY && it->second.rejectTime > 0)
			{
				result = REQUEST_JOIN_BATTLE_FAIL_TOO_FRE;
			}
			else if (it->second.rejectTime != 0)
			{
				it->second.rejectTime = 0;
			}
			else
				result = REQUEST_JOIN_BATTLE_FAIL_ALREADY_INSIDE;
		}
		else
		{
			//check pk status
			if (pBattle->GetBattleType() == PK_BATTLE ||
				pBattle->GetBattleType() == FORCE_PK_BATTLE)
			{
				if(!GetEnablePK())
				{
					//defenser team
					if (!(pChar->m_battleTeam == 0 && pBattle->GetBattleType() == FORCE_PK_BATTLE))
						result = REQUEST_JOIN_BATTLE_FAIL_PK_ENABLE;
				}
				if (GetLevel() < PK_LEVEL_LIMIT)
					result = REQUEST_JOIN_BATTLE_FAIL_PK_LV;

				//attacker team
				if (Force && pChar->m_battleTeam == 1 && pBattle->GetBattleType() == FORCE_PK_BATTLE)
					result = REQUEST_JOIN_BATTLE_FAIL_PK_FORCE;
			}

			if (result == REQUEST_JOIN_BATTLE_OK)
			{
				
				HelperData Data;
				Data.pHelperChar = this;
				pChar->m_HelperList.insert(std::make_pair(GetChar_id(), Data));
				PROCEDURE_DO_RPC1(Character, JoinBattle, pChar, pChar->GetPeer(), pChar->GetContext(), UInt32, GetChar_id());
			}
		}
		//if (result == REQUEST_JOIN_BATTLE_OK && pChar->GetPeer())
		//	pChar->CB_AddHelper(pChar->GetPeer(), pChar->GetContext(), GetChar_id());
	}

	CB_RequestJoinBattle(pPeer, context, result);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::RejectHelper(LPCPEER pPeer, RPCContext &context, 
								   const UInt32 &char_id)
{
	HelperList::iterator it = m_HelperList.find(char_id);
	if (it != m_HelperList.end())
	{
		CCharacter *pchar = it->second.pHelperChar;
		if (pchar && pchar->GetPeer()) {
			pchar->CB_RejectHelper(pchar->GetPeer(), pchar->GetContext());
		}
		CB_LeaveHelpersList(pPeer, context, char_id);
		it->second.rejectTime = ::timeGetTime();
	}
	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::LeaveWatching(LPCPEER pPeer, RPCContext &context)
{
	CBattle * pBattle = FindNetObject<CBattle>(m_BattleNetID);
	if (pBattle)
		pBattle->RemoveMonitor(this);
	m_BattleNetID = RPCNetID();
	
	return RPC_RESULT_OK;
}

VOID CCharacter::ClearAllHelpers()
{
	m_HelperList.clear();
}

VOID CCharacter::LeaveHelpersList()
{
	if (m_pMap == NULL) return;

	CCharacter* pChar = NULL;
	CBattle * pBattle = FindNetObject<CBattle>(m_BattleNetID);
	if (pBattle)
		pChar = pBattle->m_teamData[m_battleTeam].pLeader;

	if (pChar)
	{
		HelperList::iterator it = pChar->m_HelperList.find(GetChar_id());
		if (it != pChar->m_HelperList.end())
		{
			if (pChar->GetPeer())
				pChar->CB_LeaveHelpersList(pChar->GetPeer(), pChar->GetContext(), GetChar_id());
			pChar->m_HelperList.erase(it);
		}
	}
}


StateID CCharacter::GetBattleStateID(VOID)
{
	return (m_battleState->GetStateID());
}

StateID CCharacter::GetMapStateID(VOID)
{
	return (m_mapState->GetStateID());
}

void CCharacter::AddEncounter(UInt8 index, UInt16 quest_id, UInt32 npc_id, UInt16 map_id, 
							  UInt16 maxStep, UInt16 curEnc, UInt16 maxEnc)
{	
	m_encounterMap.insert(std::make_pair(quest_id, 
		EncounterInfo(index, npc_id, map_id, maxStep, curEnc, maxEnc)));
}

void CCharacter::RemoveEncounter(UInt16 quest_id)
{
	m_encounterMap.erase(quest_id);
}

///////////////////////////////////////////////////////////////////////////////
///																			///
///								Party methods								///
///																			///
///////////////////////////////////////////////////////////////////////////////

bool CCharacter::IsLeader()
{
	return (m_pParty == NULL || m_pParty->GetAvail() && GetIsLeader());
}

VOID CCharacter::SetParty(CParty* pParty, Boolean isLeader)
{
	m_pParty = pParty;
	RPCNetID partyNetId = RPCNetID();
	if (pParty)
		partyNetId = pParty->GetNetId();
		SetPartyInfo(pParty != NULL, isLeader, partyNetId);
}

VOID CCharacter::SetTempParty(CParty* pParty)
{
	m_pTemporaryParty=pParty;
}
RPCResult CCharacter::CreateParty(LPCPEER pPeer, RPCContext &context)
{
	if (m_pBag && m_pBag->HaveBusinessItemOrMoney())
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_HAVE_BUSINESS_ITEM"));
		return RPC_RESULT_FAIL;
	}
	CParty* pParty = CreateParty();
	if (pParty == NULL)
	{
		CB_WarningMessage(pPeer, context, WARN_PARTY_CANT_INVITE_MYSELF, 0, WARN_SHOW_IN_CHATBOX);
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("failed as party null, char_id %d(%s)"),
			GetChar_id(), GetSessionId().GetString().c_str());
		return RPC_RESULT_FAIL;
	}

	return RPC_RESULT_OK;
}

CParty* CCharacter::CreateParty(Boolean bForce)
{
	if (m_pParty)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("player inside party, party=%d, char_id=%d"), 
			m_pParty, GetChar_id());
		return (NULL);
	}

	if (m_pTemporaryParty) 
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("player inside party, party=%d, char_id=%d"), 
			m_pTemporaryParty, GetChar_id());
		return (NULL);
	}

	if (GetAction() == ST_HAWK)
	{
		CB_WarningMessage(m_pPeer, m_context, WARN_HAWKING_CANT_PARTY, 0, WARN_SHOW_IN_CHATBOX);
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("failed as in hawk, char_id %d(%s)"),
			GetChar_id(), GetSessionId().GetString().c_str());
		return (NULL);
	}

	if (!bForce && IsState(ST_BATTLE))
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("cannot create party in battle when not force create] char_id=%d"), GetChar_id());
		return (NULL);
	}

	if (IsState(ST_TALK) || IsState(ST_HAWK))
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("cannot create party when talk or hawk, char_id=%d"), GetChar_id());
		return (NULL);
	}

	CParty* pParty = CParty::NewInstance();
	if (!pParty->Join(this, BOOLEAN_FALSE, bForce))
	{
		DeleteNetObject(pParty);
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreateParty [join party failed] Party Object=%p, char_id=%d"), pParty, GetChar_id());
		return (NULL);
	}

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("party %d, char_id %d(%s)"),
		pParty, GetChar_id(), GetSessionId().GetString().c_str());

	if (m_pPeer)
		CB_JoinParty(m_pPeer, m_context, pParty);

	return (pParty);
}

void CCharacter::PrePartySend()
{
	RPCMask mask;
	mask.MaskBits(CCharacter::infoMask, CCharacter::HPMask, CCharacter::SPMask, -1);
	SetSendMask(mask);
}

RPCResult CCharacter::PartyJoinRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("sessionId %s, char_id %d, memberId %d:%d"), 
		context.sessionId.GetString().c_str(), GetChar_id(), memberId.srvId, memberId.objId);

	if (IsState(ST_BATTLE))
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_IN_BATTLE"));
		return (RPC_RESULT_FAIL);
	}

	if (GetAction() == ST_HAWK)
	{
		CB_Error(pPeer, context, 1, _T("MSG_HAWK_HAWKING_CANT_PARTY"));
		return (RPC_RESULT_FAIL);
	}

	if (m_pParty || m_pTemporaryParty)
	{
		//CB_Error(pPeer, context, 1, _T("PartyJoinRequest: player already inside a party."));
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_HAVE_PARTY"));
		return RPC_RESULT_FAIL;
	}

	CCharacter* member = FindNetObject<CCharacter>(memberId);

	if (member == NULL)
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_NO_TARGET"));
		return RPC_RESULT_FAIL;
	}

	// cannot join party in different map
	if (GetMap_id() != member->GetMap_id() ||
		GetMap() == NULL || member->GetMap() == NULL ||
		GetMap()->GetLine_id() != member->GetMap()->GetLine_id())
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_CANNOT_JOIN"));
		return RPC_RESULT_FAIL;
	}

	CParty* pParty = member->GetParty();
	if (pParty == NULL)
	{
		//CB_Error(pPeer, context, 1, _T("PartyJoinRequest: target character without any party"));
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_TARGET_DONT_HAVE_PARTY"));
		return RPC_RESULT_FAIL;
	}
	if (pParty->GetCount() >= PARTY_MAX_MEMBER)
	{
		//CB_Error(pPeer, context, 1, _T("PartyJoinRequest: target party member full"));
		CB_Error(pPeer, context, 1, _T("MSG_SYS_TARGET_PARTY_FULL"));
		return RPC_RESULT_FAIL;
	}
	CCharacter* pLeader = pParty->GetLeader();
	if (pLeader == NULL || !pParty->GetAvail())
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_CANNOT_JOIN"));
		return RPC_RESULT_FAIL;
	}

	if (m_pBag && m_pBag->HaveBusinessItemOrMoney())
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_HAVE_BUSINESS_ITEM"));
		return RPC_RESULT_FAIL;
	}

	//cannot join different guild in guildwar
	if (GetMap() && GetMap()->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR && 
		GetGuildUid() != pLeader->GetGuildUid())
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_CANNOT_JOIN"));
		return RPC_RESULT_FAIL;
	}

	UInt16 nLevelDown = pParty->GetJoinPartyLevelDown();
	UInt16 nLevelUp = pParty->GetJoinPartyLevelUp();
	UInt16 nLevel = 0;
	nLevel = nLevelDown < nLevelUp ? nLevelDown : nLevelUp;
	nLevelUp = nLevelDown > nLevelUp ? nLevelDown : nLevelUp;
	nLevelDown = nLevel;
	nLevel = member->GetLevel();

	TCHAR szBuffer[512];  
	_snwprintf(szBuffer,sizeof(szBuffer),L" nLevel = %d\n",nLevel);
	OutputDebugString(szBuffer);
	_snwprintf(szBuffer,sizeof(szBuffer),L" nLevelDown = %d\n",nLevelDown);
	OutputDebugString(szBuffer);
	_snwprintf(szBuffer,sizeof(szBuffer),L" nLevelUp = %d\n",nLevelUp);
	OutputDebugString(szBuffer);

	if(nLevelDown != 0 || nLevelUp != 0)
	{
		if( nLevelDown > nLevelUp )
		return RPC_RESULT_FAIL;
		else if(nLevelDown <= nLevelUp)
		{
			if(!( nLevelDown <= nLevel && nLevel<= nLevelUp))
			{
				//CB_Error(pPeer, context, 1, _T("MSG_PARTY_LEVEL_UNQUALIFIED"));
				CB_PartyRequestNo(pPeer, context, 1);
				return RPC_RESULT_FAIL;
			}
		}
	}
	

	if (HasRequested(pLeader))
	{
		//CB_Error(pPeer, context, 1, _T("PartyJoinRequest: already request to join that party"));
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_JWAIT"));
		return RPC_RESULT_FAIL;
	}

	// already invited character before, directly join party
	if (pLeader->HasInvited(this))
	{
		if (pLeader->IsState(ST_TALK) || !pLeader->GetQuests()->CheckJoinParty() ||
			!m_pQuestSys->CheckJoinParty() || !pParty->Join(this, BOOLEAN_TRUE))
		{
			CB_Error(pPeer, context, 1, _T("MSG_PARTY_CANNOT_JOIN"));
			return RPC_RESULT_FAIL;
		}
		pParty->PreSend();
		CB_PartyRequestOk(pPeer, context, pLeader->GetNickName());		// callback to client
		CB_JoinParty(pPeer, context, pParty);
	} else {
		if (pLeader->GetRequestCount() >= PARTY_MAX_REQUEST)
		{
			CB_Error(pPeer, context, 1, _T("MSG_PARTY_REQUEST_FULL"));
			return RPC_RESULT_FAIL;
		}
		pLeader->AddRequest(this);	// add this character to join requests of party leader
		AddRequest(pLeader);		// add party leader to this character join requests
		CB_PartyRequestOk(pPeer, context, pLeader->GetNickName());
		if (pLeader->m_viewManager && pLeader->m_viewManager->AddView(this))
		{
			pLeader->m_viewManager->SyncClient(0);
		}
		pLeader->CB_PartyJoinRequest(pLeader->GetPeer(), pLeader->GetContext(), GetNetId());
	}

	TRACE_INFODTL_5(GLOBAL_LOGGER, 
		_F("sessionId %s, char_id %d, memberId %d:%d, leader char_id %d"), 
		context.sessionId.GetString().c_str(), GetChar_id(), memberId.srvId, 
		memberId.objId, pLeader->GetChar_id());

	return RPC_RESULT_OK;
}

RPCResult CCharacter::PartyJoinLevelControl(LPCPEER pPeer, RPCContext &context,const RPCNetID &targetId, const UInt16 &levelDown, const UInt16 &levelUp)
{
	
	CCharacter* member = FindNetObject<CCharacter>(targetId);
	if (member == NULL)
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_NO_TARGET"));
		return RPC_RESULT_FAIL;
	}
	if(!member->IsLeader())
	{
		return RPC_RESULT_FAIL;
	}
	CParty* pParty = member->GetParty();
	if (pParty == NULL)
	{
		//CB_Error(pPeer, context, 1, _T("PartyJoinRequest: target character without any party"));
		return RPC_RESULT_FAIL;
	}
	pParty->SetJoinPartyLevelDown(levelDown);
	pParty->SetJoinPartyLevelUp(levelUp);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::PartyJoinReply(LPCPEER pPeer, RPCContext &context, 
									 const RPCNetID &targetId, const Boolean &agree,
									 const Boolean &showMsg)
{
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("sessionId %s, char_id %d, targetId %d:%d, agree %d"), 
		context.sessionId.GetString().c_str(), GetChar_id(), targetId.srvId, targetId.objId,
		agree);

	UInt8 msgIndex = PARTY_MSG_NO_MSG;
	CParty* pParty = GetParty();
	CParty* pTempParty = GetTempParty();
	do 
	{
		if (pParty == NULL && NULL == pTempParty)
		{
			//CB_Error(pPeer, context, 1, _T("PartyJoinReply: no party for this character"));
			msgIndex = PARTY_MSG_NO_PARTY;
			break;
		}
		if (NULL == pParty) 
			pParty = pTempParty;
		if (pParty->GetLeader() != this || !pParty->GetAvail())
		{
			//CB_Error(pPeer, context, 1, _T("PartyJoinReply: only leader can reply"));
			msgIndex = PARTY_MSG_NOT_LEADER_CANT_REPLAY;
			break;
		}
		if (IsState(ST_BATTLE))
		{
			//CB_Error(pPeer, context, 1, _T("PartyJoinReply: cannot reply when inside the battle"));
			msgIndex = PARTY_MSG_MYSELF_IN_BATTLE;
			break;
		}

		CCharacter* target = FindNetObject<CCharacter>(targetId);
		if (target == NULL)
		{
			//CB_Error(pPeer, context, 1, _T("PartyJoinReply: target Id not found"));
			msgIndex = PARTY_MSG_TARGET_NOT_FIND;
			break;
		}
		if (target->IsState(ST_BATTLE))
		{
			//CB_Error(pPeer, context, 1, _T("PartyJoinReply: target still inside a battle"));
			msgIndex = PARTY_MSG_TARGET_IN_BATTLE;
			break;
		}
		// cannot invite target in different map
		if (GetMap_id() != target->GetMap_id() ||
			GetMap() == NULL || target->GetMap() == NULL ||
			GetMap()->GetLine_id() != target->GetMap()->GetLine_id())
		{
			//CB_Error(pPeer, context, 1, _T("PartyJoinReply: target not in same map"));
			msgIndex = PARTY_MSG_TARGET_NOT_IN_SAME_MAP;
			break;
		}
		if (target->GetParty()) {
			//CB_Error(pPeer, context, 1, _T("PartyJoinReply: target already have party"));
			msgIndex = PARTY_MSG_TARGET_HAVE_PARTY;
			break;
		}
		if (!HasRequested(target))
		{
			//CB_Error(pPeer, context, 1, _T("PartyJoinReply: target haven't request yet"));
			msgIndex = PARTY_MSG_TARGET_HAVENOT_REQUEST;
			break;
		}
		if (target->IsState(ST_TALK))
		{
			msgIndex = PARTY_MSG_PARTY_CANNOT_JOIN_TALK;
			break;
		}

		if (agree)
		{
			/*int dx = (int)GetCurPosX() - (int)target->GetCurPosX();
			dx *= dx;
			int dy = (int)GetCurPosY() - (int)target->GetCurPosY();
			dy *= dy;

			if ((dx + dy) > 700)
			{
				//CB_Error(pPeer, context, 1, _T("MSG_PARTY_DISTANCE_TOO_FAR"));
				msgIndex = PARTY_MSG_REPLAY_DISTANCE_TOO_FAR;
				break;
			}*/

			if (pParty->GetCount() >= PARTY_MAX_MEMBER)
			{
				//CB_Error(pPeer, context, 1, _T("PartyJoinReply: target character party full"));
				msgIndex = PARTY_MSG_PARTY_FULL;
				break;
			}
		}
	}
	while(false);

	CCharacter* target = FindNetObject<CCharacter>(targetId);
	if (target)
	{
		if (agree && msgIndex == PARTY_MSG_NO_MSG)
		{
			//if (target->IsState(ST_TALK))
			//{
			//	CB_Error(pPeer, context, 1, _T("MSG_PARTY_CANNOT_JOIN_TALK"));
			//	return RPC_RESULT_FAIL;
			//}
			CancelRequest(target, false);
			target->CancelRequest(this, false);
			if (!target->GetQuests()->CheckJoinParty() ||
				!m_pQuestSys->CheckJoinParty() || !pParty->Join(target, BOOLEAN_TRUE))
			{
				CB_Error(pPeer, context, 1, _T("MSG_PARTY_CANNOT_INVITE"));
				return RPC_RESULT_FAIL;
			}
			TRACE_INFODTL_5(GLOBAL_LOGGER, 
				_F("sessionId %s, char_id %d, targetId %d:%d, target char_id %d"), 
				context.sessionId.GetString().c_str(), GetChar_id(), 
				targetId.srvId, targetId.objId, target->GetChar_id());
			pParty->PreSend();
			if (target->GetPeer())
				target->CB_JoinParty(target->GetPeer(), target->GetContext(), pParty);

			if (m_pMap)
			{
				m_pMap->RemoveActor(target);
				m_pMap->AddActor(target);
			}
		}
		else
		{
			bool readyShowMsg = false;
			if (msgIndex != PARTY_MSG_NO_MSG)
			{
				//readyShowMsg = false;
				CB_PartyMessage(pPeer, context, msgIndex, true, String());
				target->CB_PartyMessage(target->GetPeer(), target->GetContext(), 
					PARTY_MSG_JOIN_FAIL, true, GetNickName());
			}
			else
			{
				target->CB_PartyMessage(target->GetPeer(), target->GetContext(), 
					PARTY_MSG_JREJECT, true, GetNickName());
			}
			CancelRequest(target, readyShowMsg);
			target->CancelRequest(this, readyShowMsg);
		}
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::PartyInviteRequest(LPCPEER pPeer, RPCContext &context, 
										 const RPCNetID &targetId)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("sessionId %s, char_id %d, targetId %d:%d"), 
		context.sessionId.GetString().c_str(), GetChar_id(), targetId.srvId, targetId.objId);

	if (IsState(ST_BATTLE))
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_IN_BATTLE"));
		return (RPC_RESULT_FAIL);
	}

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("PartyInviteRequest: memberId %d:%d"), targetId.srvId, targetId.objId);

	CCharacter* target = FindNetObject<CCharacter>(targetId);

	if (target == NULL)
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_NO_TARGET"));
		return RPC_RESULT_FAIL;
	}
	if (target->GetAction() == ST_HAWK)
	{
		CB_Error(pPeer, context, 1, _T("MSG_HAWK_TARGET_HAWKING_CANT_PARTY"));
		return RPC_RESULT_FAIL;
	}
	if (target->GetParty() || target->GetTempParty())
	{
		//CB_Error(pPeer, context, 1, _T("PartyInviteRequest: target character already inside a party"));
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_TARGET_HAVE_PARTY"));
		return RPC_RESULT_FAIL;
	}
	if (target->GetEnableBlockParty())
	{
		CB_Error(pPeer, context, 1, _T("MSG_BLOCK_PARTY"));
		return RPC_RESULT_FAIL;
	}
	// cannot invite target in different map
	if (GetMap_id() != target->GetMap_id() ||
		GetMap() == NULL || target->GetMap() == NULL ||
		GetMap()->GetLine_id() != target->GetMap()->GetLine_id())
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_NO_TARGET"));
		return RPC_RESULT_FAIL;
	}
	if (target->GetInviteCount() >= PARTY_MAX_INVITE) {
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_INVITE_FULL"));
		return RPC_RESULT_FAIL;
	}
	if (!target->IsInMap())
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_TARGET_IN_BATTLE"));
		return RPC_RESULT_FAIL;
	}
	if (m_pParty == NULL)
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_TARGET_DONT_HAVE_PARTY"));
		return RPC_RESULT_FAIL;
	}
	if (m_pParty->GetCount() >= PARTY_MAX_MEMBER)
	{
		//CB_Error(pPeer, context, 1, _T("PartyInviteRequest: character party full"));
		CB_Error(pPeer, context, 1, _T("MSG_SYS_TARGET_PARTY_FULL"));
		return RPC_RESULT_FAIL;
	}

	if (m_pParty->GetLeader() != this || !m_pParty->GetAvail())
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_CANNOT_JOIN"));
		return RPC_RESULT_FAIL;
	}

	// cannot join different guild in guildwar
	if (GetMap() && GetMap()->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR && 
		GetGuildUid() != target->GetGuildUid())
	{
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_CANNOT_JOIN"));
		return RPC_RESULT_FAIL;
	}
	if (HasInvited(target))
	{
		//CB_Error(pPeer, context, 1, _T("PartyInviteRequest: already invited to join the party"));
		CB_Error(pPeer, context, 1, _T("MSG_PARTY_IWAIT"));
		return RPC_RESULT_FAIL;
	}

	// target character already request before, directly join party
	if (HasRequested(target))
	{
		if (target->IsState(ST_TALK) || !target->GetQuests()->CheckJoinParty() ||
			!m_pQuestSys->CheckJoinParty() || !m_pParty->Join(target, BOOLEAN_TRUE))
		{
			CB_Error(pPeer, context, 1, _T("MSG_PARTY_CANNOT_INVITE"));
			return RPC_RESULT_FAIL;
		}
		m_pParty->PreSend();
		CB_PartyInviteOk(pPeer, context, target->GetNickName());	// callback to client
		target->CB_JoinParty(target->GetPeer(), target->GetContext(), m_pParty);
	} else {
		AddInvite(target);					// add this character to invite requests of party leader
		target->AddInvite(this);			// add party leader to this character invite requests
		m_pParty->PreSend(BOOLEAN_TRUE);
		CB_PartyInviteOk(pPeer, context, target->GetNickName());
		target->CB_PartyInviteRequest(target->GetPeer(), target->GetContext(), m_pParty);
	}
	TRACE_INFODTL_5(GLOBAL_LOGGER, 
		_F("sessionId %s, char_id %d, targetId %d:%d, target char_id %d"), 
		context.sessionId.GetString().c_str(), GetChar_id(), targetId.srvId, targetId.objId,
		target->GetChar_id());

	return RPC_RESULT_OK;
}

RPCResult CCharacter::PartyInviteReply(LPCPEER pPeer, RPCContext &context, 
									   const RPCNetID &partyId, const Boolean &agree, 
									   const Boolean &showMsg)
{
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("sessionId %s, char_id %d, partyId %d:%d, agree %d"), 
		context.sessionId.GetString().c_str(), GetChar_id(), partyId.srvId, partyId.objId,
		agree);

	UInt8 msgIndex = PARTY_MSG_NO_MSG;
	do
	{
		if (GetParty() || GetTempParty())
		{
			//CB_Error(pPeer, context, 1, _T("PartyInviteReply: character already have party"));
			msgIndex = PARTY_MSG_MYSELF_HAVE_PARTY;
			break;
		}
		if (IsState(ST_BATTLE))
		{
			//CB_Error(pPeer, context, 1, _T("PartyJoinRequest: player cannot join party in battle"));
			msgIndex = PARTY_MSG_MYSELF_IN_BATTLE;
			break;
		}

		CParty* pParty = FindNetObject<CParty>(partyId);
		if (pParty == NULL)
		{
			//CB_Error(pPeer, context, 1, _T("PartyInviteReply: target party not found"));
			msgIndex = PARTY_MSG_TARGET_NO_PARTY;
			break;
		}

		CCharacter *pLeader = pParty->GetLeader();
		if (pLeader == NULL || !pParty->GetAvail())
		{
			//CB_Error(pPeer, context, 1, _T("PartyInviteReply: invalid party leader"));
			msgIndex = PARTY_MSG_NO_LEADER;
			break;
		}
		if (pLeader->IsState(ST_BATTLE))
		{
			//CB_Error(pPeer, context, 1, _T("PartyInviteReply: target party in battle state"));
			msgIndex = PARTY_MSG_TARGET_IN_BATTLE;
			break;
		}

		// cannot invite pLeader in different map
		if (GetMap_id() != pLeader->GetMap_id() ||
			GetMap() == NULL || pLeader->GetMap() == NULL ||
			GetMap()->GetLine_id() != pLeader->GetMap()->GetLine_id())
		{
			//CB_Error(pPeer, context, 1, _T("PartyInviteReply: party leader not in same map"));
			msgIndex = PARTY_MSG_TARGET_NOT_IN_SAME_MAP;
			break;
		}
		if (!pLeader->HasInvited(this)) {
			//CB_Error(pPeer, context, 1, _T("PartyInviteReply: character haven't invited to join"));
			msgIndex = PARTY_MSG_MYSELF_HAVENOT_INVITED;
			break;
		}
		if (pLeader->IsState(ST_TALK))
		{
			msgIndex = PARTY_MSG_PARTY_CANNOT_JOIN_TALK;
			break;
		}

		if (agree)
		{
			/*int dx = (int)pParty->GetLeader()->GetCurPosX() - (int)GetCurPosX();
			dx *= dx;
			int dy = (int)pParty->GetLeader()->GetCurPosY() - (int)GetCurPosY();
			dy *= dy;

			if ((dx + dy) > 700)
			{
				msgIndex = PARTY_MSG_REPLAY_DISTANCE_TOO_FAR;
				break;
			}*/

			if (pParty->GetCount() >= PARTY_MAX_MEMBER)
			{
				//CB_Error(pPeer, context, 1, _T("PartyInviteReply: target party full"));
				msgIndex = PARTY_MSG_PARTY_FULL;
				break;
			}
			
		}
	}
	while(false);

	CParty* pParty = FindNetObject<CParty>(partyId);
	if (pParty)
	{
		CCharacter *pLeader = pParty->GetLeader();
		if (pLeader && agree && msgIndex == PARTY_MSG_NO_MSG)
		{
			//if (m_pBag && m_pBag->HaveBusinessItemOrMoney())
			//{
			//	CB_Error(pPeer, context, 1, _T("MSG_PARTY_HAVE_BUSINESS_ITEM"));
			//	return RPC_RESULT_FAIL;
			//}
			//if (pLeader->IsState(ST_TALK))
			//{
			//	CB_Error(pPeer, context, 1, _T("MSG_PARTY_CANNOT_JOIN_TALK"));
			//	return RPC_RESULT_FAIL;
			//}
			CancelInvite(pLeader, false);
			pLeader->CancelInvite(this, false);
			if (!pLeader->GetQuests()->CheckJoinParty() ||
				!m_pQuestSys->CheckJoinParty() || !pParty->Join(this, BOOLEAN_TRUE))
			{
				CB_Error(pPeer, context, 1, _T("MSG_PARTY_CANNOT_JOIN"));
				return RPC_RESULT_FAIL;
			}
			TRACE_INFODTL_5(GLOBAL_LOGGER, 
				_F("sessionId %s, char_id %d, partyId %d:%d, leader char_id %d"), 
				context.sessionId.GetString().c_str(), GetChar_id(), 
				partyId.srvId, partyId.objId, pLeader->GetChar_id());
			pParty->PreSend();
			CB_JoinParty(pPeer, context, pParty);

			if (m_pMap)
			{
				m_pMap->RemoveActor(this);
				m_pMap->AddActor(this);
			}
		}
		else
		{
			if (pLeader) {
				bool readyShowMsg = false;
				if (msgIndex != PARTY_MSG_NO_MSG)
				{
					//readyShowMsg = false;
					pLeader->CB_PartyMessage(pPeer, context, msgIndex, false, String());
					pLeader->CB_PartyMessage(pLeader->GetPeer(), pLeader->GetContext(), 
						PARTY_MSG_INVITE_FAIL, false, GetNickName());
				}
				else
				{
					pLeader->CB_PartyMessage(pLeader->GetPeer(), pLeader->GetContext(), 
						PARTY_MSG_IREJECT, false, GetNickName());
				}
				CancelInvite(pLeader, readyShowMsg);
				pLeader->CancelInvite(this, readyShowMsg);
			}
		}
	}

	return RPC_RESULT_OK;
}

Boolean CCharacter::HasRequested(CCharacter *target)
{
	if (target == NULL) return BOOLEAN_FALSE;

	return (std::find(m_requestList.begin(), m_requestList.end(), target) != 
		m_requestList.end());
}

Boolean CCharacter::HasInvited(CCharacter *target)
{
	if (target == NULL) return BOOLEAN_FALSE;

	return (std::find(m_inviteList.begin(), m_inviteList.end(), target) != 
		m_inviteList.end());
}

Size CCharacter::GetRequestCount()
{
	return m_requestList.size();
}

Size CCharacter::GetInviteCount()
{
	return m_inviteList.size();
}

VOID CCharacter::AddRequest(CCharacter *target)
{
	if (target == NULL) return;

	m_requestList.push_back(target);
}

VOID CCharacter::AddInvite(CCharacter *target)
{
	if (target == NULL) return;

	m_inviteList.push_back(target);
}

VOID CCharacter::CancelAllRequests()
{
	Boolean clearRequest = !m_requestList.empty();
	Boolean clearInvite = !m_inviteList.empty();

	// nothing to cancel
	if (!clearRequest && !clearInvite)
		return;

	// clear all join requests
	for (TargetList::iterator it = m_requestList.begin(); it != m_requestList.end(); ++it)
	{
		CCharacter *pchar = *it;
		if (pchar)
			pchar->CancelRequest(this);
	}
	m_requestList.clear();

	// clear all invitations
	for (TargetList::iterator it = m_inviteList.begin(); it != m_inviteList.end(); ++it)
	{
		CCharacter *pchar = *it;
		if (pchar)
			pchar->CancelInvite(this);
	}
	m_inviteList.clear();

	RPCNetID noneId;	// invalid net id mean clear all requests

	// this character is party leader
	if (clearRequest && m_pParty && m_pParty->GetLeader() == this && m_pPeer) {
		CB_PartyClearRequest(m_pPeer, m_context, noneId, false);
	} else if (clearInvite && m_pParty == NULL && m_pPeer) {
		CB_PartyClearInvite(m_pPeer, m_context, noneId, false);
	}
}

VOID CCharacter::CancelRequest(CCharacter *target, Boolean showMsg)
{
	if (target == NULL) return;

	TargetList::iterator it = std::find(m_requestList.begin(), m_requestList.end(), target);
	if (it != m_requestList.end())
	{
		// this character is party leader
		if (m_pParty && m_pParty->GetLeader() == this && m_pPeer)
		{
			CB_PartyClearRequest(m_pPeer, m_context, target->GetNetId(), showMsg);
		}
		if (showMsg && IsLeader() && target->GetPeer())
		{
			target->CB_PartyMessage(target->GetPeer(), target->GetContext(), 
				PARTY_MSG_JOIN_FAIL, false, GetNickName());
		}
		m_requestList.erase(it);
	}
}

VOID CCharacter::CancelInvite(CCharacter *target, Boolean showMsg)
{
	if (target == NULL) return;

	TargetList::iterator it = std::find(m_inviteList.begin(), m_inviteList.end(), target);
	if (it != m_inviteList.end())
	{
		// this character is invited
		if (m_pParty == NULL && target->GetParty() && m_pPeer)
		{
			CB_PartyClearInvite(m_pPeer, m_context, target->GetParty()->GetNetId(), showMsg);
		}
		if (showMsg && target->IsLeader() && m_pPeer)
		{
			CB_PartyMessage(m_pPeer, m_context, 
				PARTY_MSG_INVITE_FAIL, false, target->GetNickName());
		}
		m_inviteList.erase(it);
	}
}

///////////////////////////////////////////////////////////////////////////////
///																			///
///								Skill methods								///
///																			///
///////////////////////////////////////////////////////////////////////////////

RPCResult CCharacter::DoLearnSkill(RPCContext &context)
{
	PROCEDURE_START_METHOD2(CCharacter, DoLearnSkill, this, context, UInt16Vector, skill_ids, UInt8, Level)

		UInt32Vector owner_ids;
		UInt16Vector skill_levels;
		owner_ids.resize(skill_ids.size());
		skill_levels.resize(skill_ids.size());
		std::fill_n(owner_ids.begin(), skill_ids.size(), GetChar_id());
		std::fill_n(skill_levels.begin(), skill_ids.size(), Level);
		ObjectService_Proxy::CreateSkillList(gGlobal.pObjectService->pPeer, context, 
			SKILL_OWNER_TYPE_CHAR, owner_ids, skill_ids, skill_levels);
	
	PROCEDURE_WAIT1(1, ObjectService, CB_GetSkillList, NetGroup*, skillDataGroup)

		TRACE_ENSURE(skillDataGroup);
		
		NetGroup grp;
		bool calAttri = false;
		for (Int i = 0; i < skillDataGroup->GetCount(); i++)
		{
			CSkillData * pSkillData = skillDataGroup->GetChild<CSkillData >(i);
			if (pSkillData && gGlobal.m_resource->GetSkillData(pSkillData->GetSkill_id()))
			{
				CSkill * pSkill = CSkill::NewInstance();
				pSkill->SetData(pSkillData);
				pSkill->PreSend();
				m_pSkillGroup->Add(pSkill);
				grp.Add(pSkill);
				m_skillMap.insert(std::make_pair(pSkill->GetSkill_id(), pSkill));

				const SkillData * pSkillDataStr = gGlobal.m_resource->GetSkillData(pSkill->GetSkill_id());
				if (pSkillDataStr)
				{
					calAttri = true;
				}
					
			}
		}

		AddChildSkill(&grp);

		if (calAttri)
			CalcAttrib();

		DeleteNetGroup(skillDataGroup, BOOLEAN_TRUE);
		if (m_pPeer)
			m_pSkillGroup->CB_AddSkills(m_pPeer, m_context, &grp);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CCharacter DoLearnSkill Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::DoRemoveSkill(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CCharacter, DoRemoveSkill, this, context, UInt16Vector, skill_ids)

	ObjectService_Proxy::DeleteSkillList(gGlobal.pObjectService->pPeer, context, GetChar_id(), skill_ids, 0);

	PROCEDURE_WAIT(1, ObjectService, CB_DeleteSKillDone)

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CCharacter DoRemoveSkill (1): skill_ids %s"),
			GetVectorString(skill_ids).c_str());

		for (Int x = 0; x < (Int)skill_ids.size(); x++)
		{
			for (UInt16 i = 0; i < m_pSkillGroup->GetCount(); ++i)
			{
				CSkill * pChild = m_pSkillGroup->GetChild<CSkill>(i);

				if (pChild && pChild->GetSkill_id() == skill_ids[x])
				{
					m_pSkillGroup->Remove(pChild);
					DeleteNetObject(pChild);
					break;
				}
			}

			SkillMap::iterator Itr = m_skillMap.find(skill_ids[x]);
			if (Itr != m_skillMap.end())
			{
				m_skillMap.erase(Itr);
			}
		}
		if (m_pPeer)
			m_pSkillGroup->CB_RemoveSkills(m_pPeer, m_context, skill_ids);

		PROCEDURE_CATCH

	PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CCharacter DoRemoveSkill Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}

RPCResult CCharacter::LearnSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id)
{
	SkillMap::iterator Itr = m_skillMap.find(skill_id);
	if (Itr != m_skillMap.end())
	{
		return RPC_RESULT_OK;
	}

	UInt16Vector IdLst;
	IdLst.push_back(skill_id);
	/*gGlobal.m_resource->GetSkillLink(skill_id, IdLst);
	//remove skill that has been learn
	UInt16Vector::iterator ItrId = IdLst.begin();
	while (ItrId != IdLst.end())
	{
		Itr = m_skillMap.find(*ItrId);
		if (Itr != m_skillMap.end())
			ItrId = IdLst.erase(ItrId);
		else
			++ItrId;
	}
	*/
	PROCEDURE_DO_METHOD2(CCharacter, DoLearnSkill, this, m_context, UInt16Vector, IdLst, UInt8, 1)
	return RPC_RESULT_OK;
}

void CCharacter::SetUseSkillReturn(BYTE flag)
{
	m_ReturnFlag = flag;
}

bool CCharacter::IsSkillValidTarget(CCharacter * pTarget, UInt16 targetType)
{
	switch(targetType)
		{
		case TARGET_TYPE_ANY:
			return TRUE;
			break;
		case TARGET_TYPE_SELF:
			if (this == pTarget)
				return TRUE;
			break;
		case TARGET_TYPE_ANY_SAME_TEAM:
			if ((m_pParty && m_pParty->IsMember(pTarget)) ||
				this == pTarget)
				return TRUE;
			break;
		case TARGET_TYPE_ANY_ENEMY_TEAM:
			if ((m_pParty && !m_pParty->IsMember(pTarget)) ||
				(m_pParty == NULL && this != pTarget))
				return TRUE;
			break;
		}
		return FALSE;
}
RPCResult CCharacter::UseSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const UInt32 &TargetID, const BYTE &TargetType)
{
	SkillMap::iterator Itr = m_skillMap.find(skill_id);
	if (Itr == m_skillMap.end())
	{
		return RPC_RESULT_OK;
	}

	const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(skill_id);

	if (pSkillData == NULL)
		return RPC_RESULT_OK;

	if(m_pQuestion->GetState() == CQuestion::ST_ENTER || m_pQuestion->GetState() == CQuestion::ST_QUESTION )
	{
		CB_UseSkill(pPeer, context, skill_id, SKILL_USE_FAIL_TYPE_BLOCK, m_Msg, GetChar_id(), 0);
		return RPC_RESULT_OK;
	}

	if (m_TempSkillID != 0)
	{
		CB_UseSkill(pPeer, context, skill_id, SKILL_USE_FAIL_TYPE_USING, m_Msg, GetChar_id(), 0);
		return RPC_RESULT_OK;
	}
	if (TargetType == SKILL_USE_TARGET_PLAYER)
	{
		
		StringA LuaFunc;
		LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(skill_id, "skill").c_str(), CommonSkillFuncName[SKILL_FUNC_NAME_CANUSE].c_str());

		Int skillLv = 0;
		if (pSkillData->reqBaseSkill == 0)
			skillLv = GetSkillLv(skill_id);
		else
			skillLv = GetSkillLv(pSkillData->reqBaseSkill);

		m_Msg = _T("");
		if (m_pScriptSys->IsRunning() || 
			skillLv < pSkillData->reqBaseSkillRank ||
			GetLevel() < pSkillData->reqLv)
			return CB_UseSkill(pPeer, context, skill_id, 1, m_Msg, GetChar_id(), 0);

		m_ReturnFlag = 0;
		int result = 0;
	

		if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
			CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(),this, skillLv, 
				pSkillData->reqBaseSkillRank, pSkillData->reqLv);
		if (result != 0 || m_ReturnFlag)
		{
			return CB_UseSkill(pPeer, context, skill_id, 1, m_Msg, GetChar_id(), 0);
		}

		if (m_pMap)
		{
			CCharacter * pTarget = CastNetObject<CCharacter>(m_pMap->FindMapActor(Character_CLASSID, TargetID));

			if (pTarget)
			{
				//check valid target
				if (!IsSkillValidTarget(pTarget, pSkillData->targetType))
					return CB_UseSkill(pPeer, context, skill_id, 1, m_Msg, GetChar_id(), 0);
				//calculate the total time

				UInt32 TotalTime = 0;
				for (Int i = 0; i < 3; i ++)
				{
					CQGLCSVTReader reader;
					String buf;
					if (i == 0)
						buf.Format(_T("Data\\EntDelay\\%s.csv"), pSkillData->effect_Atk);
					else if (i == 1)
						buf.Format(_T("Data\\EntDelay\\%s.csv"), pSkillData->effect_Hurt);
					else
						buf.Format(_T("Data\\EntDelay\\%s.csv"), pSkillData->effect_Spark);
					if (reader.Open(buf.c_str(), true))
					{
						if (!reader.IsLineEmpty())
						{
							UInt32 delay;
							reader.Read(_T("Delay"),	delay);
							TotalTime += delay;
						}
						reader.Close();
					}
				}			
				
				RaiseUniqEventID(EVT_USE_SKILL, TotalTime);

				m_TempSkillID = skill_id;
				m_TempSkillTargetID = TargetID;
				m_TempSkillTargetType = TargetType;

				if (m_viewManager)
				{
					CharacterPtrList chars;
					m_viewManager->GetCharacters(chars);

					for (CharacterPtrList::iterator it = chars.begin(); it != chars.end(); ++it)
					{
						if (*it && !(*it)->IsInBattle() )
						{
							(*it)->CB_UseSkill((*it)->GetPeer(), (*it)->GetContext(), skill_id, 0,  pTarget->m_Msg, GetChar_id(), pTarget->GetChar_id());
						}
					}
				}

				if (GetPeer() && !IsInBattle())
					CB_UseSkill(GetPeer(), GetContext(), skill_id, 0,  pTarget->m_Msg, GetChar_id(), pTarget->GetChar_id());
				
				/*
				m_ReturnFlag = 0;
				StringA LuaFunc;
				LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(skill_id, "skill").c_str(), CommonSkillFuncName[SKILL_FUNC_NAME_USE].c_str());
				result = CallLuaFuncA(gGlobal.g_pLuaState, LuaFunc.c_str(), this, pTarget, NULL, skillLv);


				//if (!m_Msg.IsEmpty())

				if (result == 0 && m_ReturnFlag == 0)
				{
					if (m_pMap)
					{
						for (Int i = 0; i < m_pMap->GetRegionCount(); i ++)
						{
							Region * pRegion = m_pMap->FindRegion(i);
							if (pRegion)
							{
								//for (Int j = 0; j < pRegion->m_actors.size(); j ++)
								for (Region::MapActorMap::iterator Itr = pRegion->m_actors.begin();
									Itr != pRegion->m_actors.end(); ++Itr)
								{
									CCharacter *pChar = CastNetObject<CCharacter>(Itr->second);
									if (pChar)
									{
										if (pChar == pTarget)
											pChar->CB_UseSkill(pChar->GetPeer(), pChar->GetContext(), 
											skill_id, 0,  pTarget->m_Msg, GetChar_id(), pTarget->GetChar_id());
										else
											pChar->CB_UseSkill(pChar->GetPeer(), pChar->GetContext(), 
											skill_id, 0, m_Msg, GetChar_id(), pTarget->GetChar_id());

									}
								}
							}

						}
					}
					else
					{
						CB_UseSkill(pPeer, context, skill_id, 0, m_Msg, GetChar_id(), pTarget->GetChar_id());

						if (this != pTarget && !pTarget->m_Msg.IsEmpty())
							CB_UseSkill(pTarget->GetPeer(), pTarget->GetContext(), skill_id, 0, pTarget->m_Msg, GetChar_id(), pTarget->GetChar_id());
					}
				}
				else
				{
					return CB_UseSkill(pPeer, context, skill_id, 1, m_Msg, GetChar_id(), 0);
				}
				*/
			}
			//CB_UseSkill(pPeer, context, 
			
		}
	}

	return RPC_RESULT_OK;
}
RPCResult CCharacter::UpGradeSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id)
{
	//find skill data
	CSkill * pSkill = NULL;
	SkillMap::iterator Itr = m_skillMap.find(skill_id);
	if (Itr != m_skillMap.end())
	{
		pSkill = Itr->second;
	}

	if (pSkill)
	{
		const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(pSkill->GetSkill_id());
		if (pSkillData)
		{
			if (pSkill->GetSkill_level() < pSkillData->maxRank)
			{
				pSkill->SetLevel(pSkill->GetSkill_level() + 1);
				CalcAttrib();
				m_pSkillGroup->RaiseUpdate();
			}
		}
	}
	return RPC_RESULT_OK;
}

void CCharacter::AddChildSkill(NetGroup * grp)
{
	StlVector<UInt16> missingSkill;
	for (SkillMap::iterator Itr = m_skillMap.begin(); Itr != m_skillMap.end(); ++Itr)
	{
		const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(Itr->second->GetSkill_id());
		if (pSkillData)
		{
			UInt16Vector link;
			gGlobal.m_resource->GetSkillLink(Itr->second->GetSkill_id(), link);

			for (UInt i = 0; i < link.size(); i++)
			{
				SkillMap::iterator ItrChild = m_skillMap.find(link[i]);
				if (ItrChild == m_skillMap.end())
				{
					missingSkill.push_back(link[i]);
				}
			}
		}
	}

	for (UInt i = 0; i < missingSkill.size(); i++)
	{		
		const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(missingSkill[i]);
		if (pSkillData)
		{
			CSkill * pSkill = CSkill::NewInstance();			
			
			pSkill->SetInfo(missingSkill[i]);
			pSkill->m_data = pSkillData;				
			pSkill->SetOwner_info(GetChar_id(), SKILL_OWNER_TYPE_CHAR);			
			pSkill->SetLevel(1);			
			pSkill->SetCoolDown(0);
			pSkill->PreSend();
			m_pSkillGroup->Add(pSkill);	
			if (grp)
				grp->Add(pSkill);	
			m_skillMap.insert(std::make_pair(pSkill->GetSkill_id(), pSkill));			
		}
	}
}
RPCResult CCharacter::RemoveSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id)
{
	SkillMap::iterator Itr = m_skillMap.find(skill_id);
	if (Itr == m_skillMap.end())
	{
		return RPC_RESULT_OK;
	}

	UInt16Vector IdLst;
	gGlobal.m_resource->GetSkillLink(skill_id, IdLst);

	//remove skill that has not been learnt
	UInt16Vector::iterator ItrId = IdLst.begin();
	while (ItrId != IdLst.end())
	{
		Itr = m_skillMap.find(*ItrId);
		if (Itr == m_skillMap.end())
			ItrId = IdLst.erase(ItrId);
		else
			++ItrId;
	}

	PROCEDURE_DO_METHOD1(CCharacter, DoRemoveSkill, this, m_context, UInt16Vector, IdLst)
	return RPC_RESULT_OK;
}


UInt16 CCharacter::GetSkillLv(UInt16 ID)
{
	SkillMap::iterator Itr = m_skillMap.find(ID);
	if (Itr != m_skillMap.end())
		return Itr->second->GetSkill_level();
	return 0;
}

void CCharacter::SetSkillCoolDown(UInt16 ID, DWORD countDown)
{
	SkillMap::iterator Itr = m_skillMap.find(ID);
	if (Itr != m_skillMap.end())
	{
		Itr->second->SetCoolDown(countDown);
		m_pSkillGroup->RaiseUpdate();
	}
}

bool CCharacter::HasSkill(UInt16 SkillId)
{
	SkillMap::iterator Itr = m_skillMap.find(SkillId);
	if (Itr != m_skillMap.end())
		return true;
	else
		return false;
}

CSkill* CCharacter::GetSkill(UInt16 SkillId)
{
	SkillMap::iterator Itr = m_skillMap.find(SkillId);
	if (Itr != m_skillMap.end())
		return Itr->second;
	else
		return NULL;
}

VOID CCharacter::AddSkill(UInt16 skillId)
{
#ifdef LINEUP_SYSTEM_ON
	if (skillId < 14 && skillId != 0) {
		int iLineuTotal=0;
		SkillMap::iterator itLineup = m_skillMap.begin();
		for (;itLineup != m_skillMap.end();itLineup++) {
			if (itLineup->first < 14 && itLineup->first != 0)
				iLineuTotal++;
		}
		if (iLineuTotal >= 5)
			return;
	}
#endif

	SkillMap::iterator Itr = m_skillMap.find(skillId);
	if (Itr == m_skillMap.end())
	{
		UInt16Vector IdLst;
		gGlobal.m_resource->GetSkillLink(skillId, IdLst);
		PROCEDURE_DO_METHOD2(CCharacter, DoLearnSkill, this, m_context, UInt16Vector, IdLst, UInt8, 1)
	}
	else
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CCharacter AddSkill Warning : Skill Exists"));
	}
}
bool CCharacter::InFreePKMap()
{
	if (m_pMap && m_pMap->GetMapInfo() && m_pMap->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_FREE_PK)
	{
		return true;
	}

	return false;
}
bool CCharacter::InGuildWar()
{
	if (m_pMap && m_pMap->GetMapInfo() && m_pMap->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR)
	{
		return true;
	}

	return false;
}
VOID CCharacter::ChangeSkillLv(UInt16 skillId, Int level)
{
	//find skill
	SkillMap::iterator it = m_skillMap.find(skillId);
	if (it != m_skillMap.end())
	{
		CSkill *pSkill = it->second;
		if (pSkill)
		{
			pSkill->SetLevel(MAX((Int) pSkill->GetSkill_level() + level, 1));
			CalcAttrib();
			m_pSkillGroup->RaiseUpdate();
		}
	}
}

VOID CCharacter::RemoveSkill(UInt16 skillId)
{
	SkillMap::iterator Itr = m_skillMap.find(skillId);
	if (Itr != m_skillMap.end())
	{
		UInt16Vector IdLst;
		gGlobal.m_resource->GetSkillLink(skillId, IdLst);

		PROCEDURE_DO_METHOD1(CCharacter, DoRemoveSkill, this, m_context, UInt16Vector, IdLst)
	}
}

RPCResult CCharacter::SkillShopLearn(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id)
{
	Int skillLv = 0;
	const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(skill_id);
	if (pSkillData == NULL)
		return RPC_RESULT_OK;

	const SkillShopData * pSkillShopData = gGlobal.m_resource->GetSkillShop(m_tempSkillShopID);
	if (pSkillShopData == NULL)
		return RPC_RESULT_OK;

	bool found = false;
	for (Int i = 0; i < (Int) pSkillShopData->SkillShopIDLst.size(); i++)
	{
		if (pSkillShopData->SkillShopIDLst[i] == skill_id)
		{
			found = true;
			break;
		}
	}

	if (!found)
		return RPC_RESULT_OK;

	CSkill * pSkill = NULL;

	SkillMap::iterator Itr = m_skillMap.find(skill_id);
	if (Itr != m_skillMap.end())
	{
		pSkill = Itr->second;
		skillLv = pSkill->GetSkill_level();
	}

	UInt32 useExp =  0;
	UInt32 useMoney =  0;
	UInt honor = 0;
	UInt32 useFDonate = 0;
	UInt32 useGDonate = 0;
	
	if (pSkillShopData->UseEXP)
	{
		CallLuaFunc(gGlobal.g_pLuaState, "CalSkillNextLvExp", String(),skillLv + 1, m_tempSkillShopID);
		useExp = (UInt32)gGlobal.g_TempResult;			
	}

	if (pSkillShopData->UseMoney)
	{
		CallLuaFunc(gGlobal.g_pLuaState, "CalSkillNextLvMoney", String(),skillLv + 1, m_tempSkillShopID);
		useMoney = (UInt32)gGlobal.g_TempResult;			
	}

	if (pSkillShopData->UseHonor)
	{
		CallLuaFunc(gGlobal.g_pLuaState, "CalSkillNextLvHonor", String(), skillLv + 1, m_tempSkillShopID);
		honor = (UInt32)gGlobal.g_TempResult;
	}

	if (pSkillShopData->UseDonateFaction)
	{
		CallLuaFunc(gGlobal.g_pLuaState, "CalSkillNextLvDonateFaction", String(), skillLv + 1, m_tempSkillShopID);
		useFDonate = (UInt32)gGlobal.g_TempResult;			
	}

	if (pSkillShopData->UseDonateManor)
	{
		CallLuaFunc(gGlobal.g_pLuaState, "CalSkillNextLvDonateManor", String(), skillLv + 1, m_tempSkillShopID);
		useGDonate = (UInt32)gGlobal.g_TempResult;			
	}


	BYTE result = SKILL_SHOP_LEARN_OK;
	if (GetLevel() < pSkillData->reqLv)
	{
		result = SKILL_SHOP_FAIL_LV;
	}
	else if (skillLv >= pSkillData->maxRank)
	{
		result = SKILL_SHOP_FAIL_MAX_RANK;
	}
	else if (GetExp() < useExp)
	{
		result = SKILL_SHOP_FAIL_EXP;
	}
	else if ((!pSkillShopData->UseSmoney && GetMoney() + GetBankMoney() < useMoney) ||
		(pSkillShopData->UseSmoney && GetMoney() + GetSMoney() + GetBankMoney() < useMoney))
	{
		result = SKILL_SHOP_FAIL_MONEY;
	}
	else if (skillLv - GetLevel() >=  10)
	{
		result = SKILL_SHOP_FAIL_LV2;
	}
	else if (GetHonor() < honor)
	{
		result = SKILL_SHOP_FAIL_HONOR;
	}
	else if (GetFDonate() < useFDonate)
	{
		result = SKILL_SHOP_FAIL_DONATE;
	}

	if (result == SKILL_SHOP_LEARN_OK)
	{
		ChangeExp(- (Int32)useExp, LogService_Proxy::Money_And_Exp::SOURCE_SKILLSHOP_BUY);
		SetFDonate(GetFDonate() - useFDonate);
		
		if (honor > 0)
			ChangeHonor(-(Int32)honor);

		if (pSkillShopData->UseSmoney)
		{
			UInt32 useSMoney = 0;
			if (useMoney > GetSMoney())
			{
				useSMoney = GetSMoney();
				useMoney -= GetSMoney();
				SetSMoney(0);
			}
			else
			{
				SetSMoney(GetSMoney() - useMoney);
				useSMoney = useMoney;
				useMoney = 0;				
			}
			if (useSMoney > 0)
				LogService_Proxy::LogSMoney(gGlobal.pLogService->pPeer, m_context, 
					GetAcct_id(), GetChar_id(), 
					LogService_Proxy::Money_And_Exp::SOURCE_SKILLSHOP_BUY, -(Int32)useSMoney);
		}
		if (useMoney > GetMoney())
		{
			useMoney -= GetMoney();
			SetMoney(0);
			SetBankMoney(GetBankMoney() - useMoney);
		}
		else
			SetMoney(GetMoney() - useMoney);
		if (useMoney > 0)
			LogService_Proxy::LogMoney(gGlobal.pLogService->pPeer, m_context, 
				GetAcct_id(), GetChar_id(), 
				LogService_Proxy::Money_And_Exp::SOURCE_SKILLSHOP_BUY, useMoney, GetMoney() + GetBankMoney());

		if (pSkill == NULL)
			LearnSkill(pPeer, context, skill_id);
		else
			UpGradeSkill(pPeer, context, skill_id);

		RaiseUpdate();
	}
	CB_SkillShopLearn(pPeer, context, result);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::LeaveSkillShop(LPCPEER pPeer, RPCContext &context)
{
	m_tempSkillShopID = 0;
	return RPC_RESULT_OK;
}

///////////////////////////////////////////////////////////////////////////////
///																			///
///								Buff methods								///
///																			///
///////////////////////////////////////////////////////////////////////////////

bool CCharacter::AddBuff(UInt16 Buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro, bool calc)
{
	ExtractedBuffData Data;
	Data.ID = Buff_id;
	Data.CoolDown = Buff_CoolDown;
	Data.Value1 = Buff_Value1;
	Data.Value2 = Buff_Value2;
	Data.Value3 = Buff_Value3;
	Data.Pro = Pro;

	bool added = false;
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		Data.pBuffData = pBuffData;
		InitialRepeatBuff(Data, pBuffData); 	
		if (pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_MINUTES)
			Data.CoolDown *= 60;
		if (pBuffData->Buff_RemoveAftOffline ||
			pBuffData->Buff_type == BUFF_TYPE_EQ )
		{
			BuffMap::iterator Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
			if (Itr != m_BuffCacheMap.end())
			{
				if (Data >= Itr->second)
				{
					//m_BuffCacheMap.erase(Itr);
					RemoveBuff(Itr->second.ID);
					m_BuffCacheMap.insert(std::make_pair(pBuffData->Buff_school, Data));
					
					added = true;
				}
			}
			else
			{
				m_BuffCacheMap.insert(std::make_pair(pBuffData->Buff_school, Data));
				
				added = true;
			}
		}
		else
		{
			if (m_BuffMap.size() == MAX_BUFF_NUM)
				return false;
			BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
			if (Itr != m_BuffMap.end())
			{
				if (Data >= Itr->second)
				{
					RemoveBuff(Itr->second.ID);
					m_BuffMap.insert(std::make_pair(pBuffData->Buff_school, Data));
					
					added = true;
				}
			}
			else
			{
				m_BuffMap.insert(std::make_pair(pBuffData->Buff_school, Data));
				
				added = true;
			}
		}

		if (added)
		{
			CheckBuffUpdate();
			ConstructBuffString();
			ConstructBuffCacheString();
			if (pBuffData->Buff_Speed != 0)
				UpdateSpeed();
			if (pBuffData->Buff_BlockChat.size() > 0)
				UpdateBuffBlockChannel();
			StringA LuaFunc;

			LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pBuffData->id, "buff").c_str(), 
				CommonBuffFuncName[BUFF_FUNC_NAME_ADD].c_str());
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), &Data,  this);

			if (calc)
			{
				for (Int k = 0; k < (Int)pBuffData->Buff_acttimes.size(); k++)
				{
					if (pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
						CalcAttrib();
				}
			}
		}

	}
	CheckBuffUpdate();

	return added;
}

void CCharacter::RemoveBuff(UInt16 Buff_id, bool calc)
{
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffMap.end() &&
			Itr->second.ID == Buff_id)
		{
			ExtractedBuffData Data = Itr->second;
			m_BuffMap.erase(Itr);
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CCharacter::RemoveBuff, Buff id = %d is removed from m_BuffMap list, m_BuffMap size is %d"), Buff_id, m_BuffMap.size());
			
			ConstructBuffString();
			if (pBuffData->Buff_Speed != 0)
				UpdateSpeed();
			if (pBuffData->Buff_BlockChat.size() > 0)
				UpdateBuffBlockChannel();
			StringA LuaFunc;
			LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pBuffData->id, "buff").c_str(), 
				CommonBuffFuncName[BUFF_FUNC_NAME_REMOVE].c_str());
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), &Data,  this);

			if (calc)
			{
				for (Int k = 0; k < (Int)pBuffData->Buff_acttimes.size(); k++)
				{
					if (pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
						CalcAttrib();
				}
			}
			return;
		}

		Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffCacheMap.end() &&
			Itr->second.ID == Buff_id)
		{
			ExtractedBuffData Data = Itr->second;
			m_BuffCacheMap.erase(Itr);
			
			ConstructBuffCacheString();
			if (pBuffData->Buff_Speed != 0)
				UpdateSpeed();
			if (pBuffData->Buff_BlockChat.size() > 0)
				UpdateBuffBlockChannel();
			StringA LuaFunc;
			LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pBuffData->id, "buff").c_str(), 
				CommonBuffFuncName[BUFF_FUNC_NAME_REMOVE].c_str());
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), &Data,  this);
			if (calc)
			{
				for (Int k = 0; k < (Int)pBuffData->Buff_acttimes.size(); k++)
				{
					if (pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
						CalcAttrib();
				}
			}
			return;
		}
	}
}

void CCharacter::RemoveBuffSchool(UInt16 school_id)
{
	BuffMap::iterator it = m_BuffMap.find(school_id);
	if (it != m_BuffMap.end()) {
		RemoveBuff(it->second.ID);
	} else {
		it = m_BuffCacheMap.find(school_id);
		if (it != m_BuffCacheMap.end())
			RemoveBuff(it->second.ID);
	}
}

bool CCharacter::HasBuff(UInt16 Buff_id)
{
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffMap.end() &&
			Itr->second.ID == Buff_id)
		{
			return true;
		}

		Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffCacheMap.end() &&
			Itr->second.ID == Buff_id)
			return true;
	}
	return false;
}

ExtractedBuffData * CCharacter::FindBuff(UInt16 Buff_id)
{
	
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		CheckBuffUpdate();
		BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffMap.end() &&
			Itr->second.ID == Buff_id)
		{
			return &Itr->second;
		}

		Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffCacheMap.end() &&
			Itr->second.ID == Buff_id)
			return &Itr->second;
	}
	return NULL;
}


VOID CCharacter::ExtractBuffCacheString()
{
	m_BuffCacheMap.clear();
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	TCHAR *value = NULL; 
	TCHAR *buf = SafeAllocate(TCHAR, GetBuffCache().size() + 1);
	buf[GetBuffCache().size()] = _T('\0');
	_tcscpy(buf, GetBuffCache().c_str());
	_tcstok(buf, valueDelim);
	bool first = true;
	while (true)
	{
		ExtractedBuffData Data;
		//buff id
		if (first)
		{
			if (buf)
				Data.ID = _ttoi(buf);
			else
				break;
			first = false;
		}
		else
		{

			value = _tcstok(NULL, valueDelim);
			if (value)
				Data.ID = _ttoi(value);
			else
				break;
		}

		//buff Value1
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value1 = _ttoi(value);
		else
			break;

		//buff Value2
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value2 = _ttoi(value);
		else
			break;

		//DOT Value3
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value3 = _ttoi(value);
		else
			break;

		//Pro
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Pro = _ttoi(value);
		else
			break;

		//buff CoolDown
		value = _tcstok(NULL, buffDelim);
		if (value)
		{
			Data.CoolDown = _ttoi(value);
			while (value && (((*value >= 48) && (*value <= 57)) || (*value == 44) || (*value == 130))) // while the values are numbers or ","while (value != NULL)
			{
				if (value && (*value == 44 || *value == 130)) // check if there is a comma "," in the string
					break;
				value++;
			}
			if (value && (*value == 44 || *value == 130))// buff Repeat
			{
				value++;
				if (value && (*value >= 48) && (*value <= 57))
				{
					Data.Repeat = _ttoi(value);
				}
				else
					Data.Repeat = 0;
			}
			else
				Data.Repeat = 0;
		}
		else
			break;

		const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Data.ID);
		if (pBuffData)
		{
			Data.pBuffData = pBuffData;
			m_BuffCacheMap.insert(std::make_pair(pBuffData->Buff_school, Data));
		}
	}
	SafeDeallocate(buf);

	UpdateSpeed();
	
	UpdateBuffBlockChannel();
}

void CCharacter::UpdateBuffBlockChannel()
{
	bool tempSpeakRights[MAX_CHANNELTYPE];
	for (Int i = 0; i < MAX_CHANNELTYPE; i++)
	{
		tempSpeakRights[i] = m_speakRights[i];
		m_speakRights[i] = true;
	}
	for (Int i = 0; i < 2; i ++)
	{
		BuffMap tempMap = m_BuffMap;
		if (i == 1)
			tempMap = m_BuffCacheMap;
		for (BuffMap::iterator Itr = tempMap.begin(); Itr != tempMap.end(); Itr++)
		{
			for (Int j = 0; j < (Int)Itr->second.pBuffData->Buff_BlockChat.size(); j++)
			{
				if (Itr->second.pBuffData->Buff_BlockChat[j] <= MAX_CHANNELTYPE && Itr->second.pBuffData->Buff_BlockChat[j] > 0)
				{
					m_speakRights[Itr->second.pBuffData->Buff_BlockChat[j] - 1] = false;
				}
			}
		}
	}

	UInt8Vector bkChannels;
	UInt8Vector unBkChannels;

	for (Int i = 0; i < MAX_CHANNELTYPE; i++)
	{
		if (tempSpeakRights[i] != m_speakRights[i])
		{
			if (m_speakRights[i])
				unBkChannels.push_back(i);
			else
				bkChannels.push_back(i);
		}
	}

	if (unBkChannels.size() > 0 || bkChannels.size() > 0)
		MessageService_Proxy::ChannelsBlockHandle(gGlobal.pMessageService->pPeer, 
				m_context, GetChar_id(), bkChannels, unBkChannels);
}

void CCharacter::CheckBuffUpdate()
{
	UInt32 curTime = ::timeGetTime();

	UInt32 SmallestDelay = 0xFFFFFFFF;
	StlVector<UInt16> removebuffIDs;
	bool update = false;
	BuffMap tempMap;
	tempMap.clear();
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); ++Itr)
	{
		if (Itr->second.pBuffData)
			tempMap.insert(std::make_pair(Itr->second.pBuffData->Buff_school, Itr->second));
	}


	for (BuffMap::iterator Itr = tempMap.begin(); Itr != tempMap.end(); ++ Itr)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_endtype == BUFF_END_TYPE_TIME_OUT_END)
		{
			if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_MINUTES)
			{
				if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_type == BUFF_TYPE_REPEAT)
				{
					UInt32 repeatTime = 0;
					if (curTime > Itr->second.PreUpdateTime)
					{
						repeatTime = (curTime - Itr->second.PreUpdateTime) / 1000;
						if (repeatTime > 0)
							Itr->second.PreUpdateTime += repeatTime * 1000;
					}
					TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CheckBuffUpdate: Buff ID = %d, CoolDown Value = %d,  Repeat Value = %d, deducttime = %d"),
													Itr->second.pBuffData->id, Itr->second.CoolDown, Itr->second.Repeat, repeatTime);

					if (Itr->second.CoolDown > repeatTime)
						Itr->second.CoolDown -= repeatTime;
					else
						Itr->second.CoolDown = 0;

					if (Itr->second.Repeat > repeatTime)
						Itr->second.Repeat -= repeatTime;
					else
					{
						Itr->second.Repeat = 60 * Itr->second.pBuffData->Buff_TimeOut;
						// call the buff lua
						StringA LuaFunc;
						LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second.pBuffData->id, "buff").c_str(), CommonBuffFuncName[BUFF_FUNC_NAME_REPEAT].c_str());
						if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
							CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Itr->second,  this, 0);
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CheckBuffUpdate: Buff ID = %d, has called a repeat buff lua "), Itr->second.pBuffData->id);

					}
					TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CheckBuffUpdate: Buff ID = %d, CoolDown Value = %d,  Repeat Value = %d, deducttime = %d"),
												 Itr->second.pBuffData->id, Itr->second.CoolDown, Itr->second.Repeat, repeatTime);

					if (Itr->second.CoolDown == 0)
					{
						removebuffIDs.push_back(Itr->second.ID); 
						// call the buff lua
						StringA LuaFunc;
						LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second.pBuffData->id, "buff").c_str(), CommonBuffFuncName[BUFF_FUNC_NAME_REPEAT].c_str());
						if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
							CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Itr->second,  this, 1);	
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CheckBuffUpdate: repeat Buff ID = %d, is removed"), Itr->second.pBuffData->id); 
									

					}

					if ((Itr->second.CoolDown > 0 && Itr->second.Repeat > 0) && 
						(Itr->second.CoolDown * 1000 < SmallestDelay || Itr->second.Repeat * 1000 < SmallestDelay))
						SmallestDelay = MIN(Itr->second.Repeat, Itr->second.CoolDown) * 1000;

					TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CheckBuffUpdate: Buff ID = %d, SmallestDelay = %d"), Itr->second.pBuffData->id, 
														SmallestDelay);
					
					update = true;
				}
				else
				{
					{
						UInt32 delTime = 0 ;
						if (curTime > Itr->second.PreUpdateTime)
						{
							delTime = (curTime - Itr->second.PreUpdateTime) / 1000;
							if (delTime > 0)
								Itr->second.PreUpdateTime += delTime * 1000;
						}
						if (Itr->second.CoolDown > delTime)
							Itr->second.CoolDown -= delTime;
						else
							Itr->second.CoolDown = 0;

						if (Itr->second.CoolDown == 0)
							removebuffIDs.push_back(Itr->second.ID);
					}

					if (Itr->second.CoolDown > 0 && Itr->second.CoolDown * 1000 < SmallestDelay)
						SmallestDelay = Itr->second.CoolDown * 1000;

					TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CheckBuffUpdate: non repeat Buff ID = %d, SmallestDelay = %d"), Itr->second.pBuffData->id, 
									SmallestDelay);
					//BUFF_UPDATETIME
					update = true;
				}
				m_BuffMap[Itr->second.pBuffData->Buff_school] = Itr->second;

			}
		}

	}

	tempMap.clear();
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); ++Itr)
	{
		if (Itr->second.pBuffData)
			tempMap.insert(std::make_pair(Itr->second.pBuffData->Buff_school, Itr->second));
	}

	for (BuffMap::iterator Itr = tempMap.begin(); Itr != tempMap.end(); ++ Itr)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_endtype == BUFF_END_TYPE_TIME_OUT_END)
		{
			if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_MINUTES)
			{
				if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_type == BUFF_TYPE_REPEAT)
				{

					UInt32 repeatTime = 0;
					if (curTime > Itr->second.PreUpdateTime)
					{
						repeatTime = (curTime - Itr->second.PreUpdateTime) / 1000;
						if (repeatTime > 0)
							Itr->second.PreUpdateTime += repeatTime * 1000;
					}
					TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CheckBuffUpdate: Cachebuff, Buff ID = %d, CoolDown Value = %d,  Repeat Value = %d, deducttime = %d"),
								Itr->second.pBuffData->id, Itr->second.CoolDown, Itr->second.Repeat, repeatTime);
					if (Itr->second.CoolDown > repeatTime)
						Itr->second.CoolDown -= repeatTime;
					else
						Itr->second.CoolDown = 0;

					if (Itr->second.Repeat > repeatTime)
						Itr->second.Repeat -= repeatTime;
					else
					{
						Itr->second.Repeat = 60 * Itr->second.pBuffData->Buff_TimeOut;
						// call the buff lua
						StringA LuaFunc;
						LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second.pBuffData->id, "buff").c_str(), CommonBuffFuncName[BUFF_FUNC_NAME_REPEAT].c_str());
						if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
							CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Itr->second,  this);	
					}
					TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CheckBuffUpdate: Cachebuff, Buff ID = %d, CoolDown Value = %d,  Repeat Value = %d, deducttime = %d"),
								Itr->second.pBuffData->id, Itr->second.CoolDown, Itr->second.Repeat, repeatTime);
					if (Itr->second.CoolDown == 0)
					{
						removebuffIDs.push_back(Itr->second.ID);   
						// call the buff lua
						StringA LuaFunc;
						LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second.pBuffData->id, "buff").c_str(), CommonBuffFuncName[BUFF_FUNC_NAME_REPEAT].c_str());
						if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
							CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Itr->second,  this, 1);
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CheckBuffUpdate: Cachebuff, repeat Buff ID = %d, is removed"), Itr->second.pBuffData->id); 
					}

					if ((Itr->second.CoolDown > 0 && Itr->second.Repeat > 0) && 
						(Itr->second.CoolDown * 1000 < SmallestDelay || Itr->second.Repeat * 1000 < SmallestDelay))
						SmallestDelay = MIN(Itr->second.Repeat, Itr->second.CoolDown) * 1000;

					TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CheckBuffUpdate: Cachebuff, Buff ID = %d, SmallestDelay = %d"), Itr->second.pBuffData->id, 
									SmallestDelay);
					
					update = true;
				}
				else
				{
					{
						UInt32 delTime = 0 ;
						if (curTime > Itr->second.PreUpdateTime)
						{
							delTime = (curTime - Itr->second.PreUpdateTime) / 1000;
							if (delTime > 0)
								Itr->second.PreUpdateTime += delTime * 1000;
						}
						if (Itr->second.CoolDown > delTime)
							Itr->second.CoolDown -= delTime;
						else
							Itr->second.CoolDown = 0;

						if (Itr->second.CoolDown == 0)
							removebuffIDs.push_back(Itr->second.ID);
					}
					if (Itr->second.CoolDown > 0 && Itr->second.CoolDown * 1000 < SmallestDelay)
						SmallestDelay = Itr->second.CoolDown * 1000;

					TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CheckBuffUpdate: Cachebuff, non repeat Buff ID = %d, SmallestDelay = %d"), Itr->second.pBuffData->id, 
						SmallestDelay);
					update = true;
				}
				m_BuffCacheMap[Itr->second.pBuffData->Buff_school] = Itr->second;

			}
		}
	}

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CheckBuffUpdate: result buff size of m_BuffMap is  %d"), m_BuffMap.size()); 

	for (Index i = C_INDEX(0); i < removebuffIDs.size(); i++)
	{
		RemoveBuff(removebuffIDs[i]);
	}
	
	if (SmallestDelay != 0xFFFFFFFF)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CheckBuffUpdate: final SmallestDelay is %d"), SmallestDelay);
		CancelUniqEvent(EVT_UPDATE_BUFF);
		RaiseUniqEventID(EVT_UPDATE_BUFF, SmallestDelay);
	}

	if (update)
	{			
		ConstructBuffString();
		ConstructBuffCacheString();
		if (m_pPeer)
			SendAttr(m_pPeer, m_context, buffMask);
	}
}
void CCharacter::ConstructBuffCacheString()
{
	String BuffList;
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); ++ Itr)
	{
		String Buff;
		Buff.Format(_T("%d%s%d%s%d%s%d%s%d%s%d%s%d%s"), Itr->second.ID, valueDelim, Itr->second.Value1,
			valueDelim, Itr->second.Value2, valueDelim, Itr->second.Value3, valueDelim, Itr->second.Pro, valueDelim, Itr->second.CoolDown, valueDelim, Itr->second.Repeat, buffDelim);
		BuffList += Buff.c_str();
	}

	SetBuffCache(BuffList);
}

//extract the buff string
VOID CCharacter::ExtractBuffString()
{
	m_BuffMap.clear();
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	TCHAR *value = NULL; 
	TCHAR *buf = SafeAllocate(TCHAR, GetBuff().size() + 1);
	buf[GetBuff().size()] = _T('\0');
	_tcscpy(buf, GetBuff().c_str());
	_tcstok(buf, valueDelim);
	bool first = true;
	while (true)
	{
		ExtractedBuffData Data;
		//buff id
		if (first)
		{
			if (buf)
				Data.ID = _ttoi(buf);
			else
				break;
			first = false;
		}
		else
		{

			value = _tcstok(NULL, valueDelim);
			if (value)
				Data.ID = _ttoi(value);
			else
				break;
		}

		//buff Value1
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value1 = _ttoi(value);
		else
			break;

		//buff Value2
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value2 = _ttoi(value);
		else
			break;

		//DOT Value3
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value3 = _ttoi(value);
		else
			break;

		//Pro
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Pro = _ttoi(value);
		else
			break;

		//buff CoolDown
		value = _tcstok(NULL, buffDelim);
		if (value)
		{
			Data.CoolDown = _ttoi(value); // input the cool down value first
			while (value && (((*value >= 48) && (*value <= 57)) || (*value == 44) || (*value == 130))) // while the values are numbers or ","
			{
				if (value && (*value == 44 || *value == 130)) // check if there is a comma "," in the string
					break;
				value++;
			}
			if (value && (*value == 44 || *value == 130))// buff Repeat
			{
				value++;
				if (value && (*value >= 48) && (*value <= 57))
				{
					Data.Repeat = _ttoi(value);
				}
				else
					Data.Repeat = 0;
			}
			else
				Data.Repeat = 0;
		}
		else
			break;

		const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Data.ID);
		if (pBuffData)
		{
			Data.pBuffData = pBuffData;
			m_BuffMap.insert(std::make_pair(pBuffData->Buff_school, Data));
		}
	}
	SafeDeallocate(buf);

	UpdateSpeed();

	UpdateBuffBlockChannel();
}

void CCharacter::ConstructBuffString()
{
	String BuffList;
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); ++ Itr)
	{
		String Buff;
		Buff.Format(_T("%d%s%d%s%d%s%d%s%d%s%d%s%d%s"), Itr->second.ID, valueDelim, Itr->second.Value1,
			valueDelim, Itr->second.Value2, valueDelim, Itr->second.Value3, valueDelim, Itr->second.Pro, valueDelim, Itr->second.CoolDown, valueDelim, Itr->second.Repeat, buffDelim);
		BuffList += Buff.c_str();
	}

	SetBuff(BuffList);
	//if (IsUpdated(CCharacter::buffMask))
}

///////////////////////////////////////////////////////////////////////////////
///																			///
///								Trade methods								///
///																			///
///////////////////////////////////////////////////////////////////////////////

RPCResult CCharacter::GivenProcess(LPCPEER pPeer, RPCContext &context, 
								   const RPCNetID &targetId, const UInt8Vector &itemIndexs,
								   const UInt8Vector &itemStacks, 
								   const UInt32Vector &PartnerIDs, const UInt32Vector &PetIDs,
								   const UInt32 &gmoney)
{

	PROCEDURE_START_RPC6(Character, GivenProcess, this, RPCNetID, targetId, 
						UInt8Vector, itemIndexs, UInt8Vector, itemStacks, 
						UInt32Vector, PartnerIDs, UInt32Vector, PetIDs, UInt32, gmoney)
		
		CCharacter* target = FindNetObject<CCharacter>(targetId);
		if (target == NULL || target == this)
		{
			CB_GivenProcess(pPeer, context, BOOLEAN_FALSE, GIVEN_CHAR_FAIL);
			PROCEDURE_RETURN
		}

		// set timeout 5s to prevent procedure dead lock
		PROCEDURE_SET_TIMEOUT(5000, BOOLEAN_TRUE)

		RPCContext context2(target->GetSessionId(), context.pcdId);
		PROCEDURE_DO_METHOD1(CCharacter, WaitAvailable, target, context2, SessionID, context.sessionId)

	PROCEDURE_WAIT_RETURN(1)

		UInt8 msgIndex = GIVEN_SUCCESS;
		CCharacter* target = FindNetObject<CCharacter>(targetId);
		if (target == NULL || target == this)
		{
			CB_GivenProcess(pPeer, context, BOOLEAN_FALSE, GIVEN_CHAR_FAIL);
			PROCEDURE_RETURN
		}

		UInt32 flowerCount = 0;
		for (Index i = C_INDEX(0); i < itemIndexs.size(); ++i)
		{
			CItem* orgItem = m_pBag->GetItem(itemIndexs[i]);
			if (orgItem == NULL || orgItem->m_data == NULL)
			{
				msgIndex = GIVEN_GIVE_NOTHING; 
				break;
			}
			if (orgItem->m_data->item_type == ITEMTYPE_FLOWER || orgItem->m_data->item_type == ITEMTYPE_WEAPON_MODEL)
			{
				flowerCount++;
			}
			//++tempBagIndex;
		}
		PROCEDURE_STORE_VALUE2(UInt8,msgIndex,UInt32,flowerCount);
		if (flowerCount > 0 && msgIndex == GIVEN_SUCCESS)
		{
			if(HasLuaFunc(gGlobal.g_pLuaState, "CanGiveWeaponModelOrFlower"))
			{
				CallLuaFunc(gGlobal.g_pLuaState, "CanGiveWeaponModelOrFlower", String(), this,target,flowerCount);
				UInt32 result = (UInt32)gGlobal.g_TempResult;
				if( result == 0 )
				{
					CB_GivenProcess(pPeer, context, BOOLEAN_FALSE, GIVEN_FLOWER_TIME_LIMIT);
					PROCEDURE_RETURN;
				}
			}

			MessageService_Proxy::GetFriendly(gGlobal.pMessageService->pPeer,context, 
				this->GetChar_id(), target->GetChar_id());
		}
		else
		{
			PROCEDURE_GO_STEP1(3,UInt32,65535);
		}

		PROCEDURE_WAIT1(2, MessageService, CB_GetFriendly, UInt32, friendly);
		PROCEDURE_RESTORE_VALUE2(UInt8,msgIndex,UInt32,flowerCount);
		
		if(flowerCount>0 && msgIndex == GIVEN_SUCCESS)
		{
			if(friendly < 10)
			{
				msgIndex = GIVEN_FLOWER_NOT_ENOUGH_FRIENDLY;
			}
			if(friendly >= MAX_FRIENDLY)
			{
				msgIndex = GIVEN_FLOWER_FULL_FRIENDLY;
			}
		}

		CCharacter* target = FindNetObject<CCharacter>(targetId);
		if (target == NULL || target == this)
		{
			CB_GivenProcess(pPeer, context, BOOLEAN_FALSE, GIVEN_CHAR_FAIL);
			PROCEDURE_RETURN
		}

		PROCEDURE_STORE_VALUE2(UInt8,msgIndex,UInt32,flowerCount);
		MessageService_Proxy::GetFriendly(gGlobal.pMessageService->pPeer,context, 
			target->GetChar_id(),this->GetChar_id() );
		PROCEDURE_WAIT1(3, MessageService, CB_GetFriendly, UInt32, friendly);
		PROCEDURE_RESTORE_VALUE2(UInt8,msgIndex,UInt32,flowerCount);
		if(flowerCount>0 && msgIndex == GIVEN_SUCCESS)
		{
			if(friendly < 10)
			{
				msgIndex = GIVEN_FLOWER_NOT_ENOUGH_FRIENDLY;
			}
			if(friendly >= MAX_FRIENDLY)
			{
				msgIndex = GIVEN_FLOWER_FULL_FRIENDLY;
			}
		}


		UInt32 addFriendly = 0;

		do {
			if (msgIndex != GIVEN_SUCCESS)
				break;

			if (m_pScriptSys->IsRunning())
			{
				msgIndex = GIVEN_RUNNING_SCRIPT_CANT_GIVEN;
				break;
			}

			CCharacter* target = FindNetObject<CCharacter>(targetId);
			UInt8 tempBagIndex = 0;

			if (target == NULL || !IsInMap() )
			{
				msgIndex = GIVEN_CHAR_FAIL; 
				break;
			}

			if (target == this || target->GetChar_id() == this->GetChar_id())
			{
				msgIndex = GIVEN_MYSELF;
				break;
			}

			if (target->GetScriptSystem() && target->GetScriptSystem()->IsRunning())
			{
				msgIndex = GIVEN_RUNNING_SCRIPT_CANT_GIVEN;
				break;
			}
			if (target->GetEnableBlockGiven())
			{
				msgIndex = GIVEN_BLOCK; //msg = _T("MSG_BLOCK_GIVEN");
				break;
			}
			if (target->IsTrading())
			{
				msgIndex = GIVEN_TRADING_CANT_GIVEN; //msg = _T("MSG_GIVEN_TRADING_FAIL");
				break;
			}
			if (target->GetPeer() == NULL && target->GetAction() == ST_HAWK)
			{
				msgIndex = GIVEN_OFFLINE_HAWKING_CANT_GIVEN; //msg = _T("MSG_HAWK_DISCONNECT_HAWKING_CANT_GIVEN");
				break;
			}

			UInt x = target->GetCurPosX() > GetCurPosX() ? 
				(target->GetCurPosX() - GetCurPosX()):(GetCurPosX() - target->GetCurPosX());
			
			UInt y = target->GetCurPosY() > GetCurPosY() ? 
				(target->GetCurPosY() - GetCurPosY()):(GetCurPosY() - target->GetCurPosY());

			x *= x;
			y *= y;

			if ( (x + y) > 2008)
			{
				msgIndex = GIVEN_DIS_FAIL; //msg = _T("MSG_GIVEN_DIS_FAIL");
				break;
			}
		
			for (Index i = C_INDEX(0); i < itemIndexs.size(); ++i)
			{
				CItem* orgItem = m_pBag->GetItem(itemIndexs[i]);
				if (orgItem == NULL || orgItem->m_data == NULL)
				{
					msgIndex = GIVEN_GIVE_NOTHING; //msg = _T("MSG_GIVEN_GIVE_NOTHING");
					break;
				}
				if (orgItem->GetStack() < itemStacks[i])
				{
					msgIndex = GIVEN_ITEM_STACK_FAIL; //msg = _T("MSG_GIVEN_STACK_FAIL");
					break;
				}
				if (orgItem->GetItem_soulboundState() == SOULBOUND_ALREADY_BOUND)
				{
					msgIndex = GIVEN_ITEM_BOUND; //msg = _T("MSG_GIVEN_ITEM_BOUND");
					break;
				}
				if (orgItem->GetLock()) // check if the item is locked
				{
					msgIndex = GIVEN_ITEM_LOCK; //msg = _T("MSG_GIVEN_ITEM_LOCK");
					break;
				}
				if (!target->m_pBag->FindEmptyIndex(0, tempBagIndex, tempBagIndex))
				{
					msgIndex = GIVEN_ITEM_NO_SLOT; //msg = _T("MSG_GIVEN_SLOT_FAIL");
					break;
				}

				if (orgItem->m_data->item_type == ITEMTYPE_FLOWER || orgItem->m_data->item_type == ITEMTYPE_WEAPON_MODEL)
				{
					if(orgItem->GetItemValue1() != 0)
					{
						msgIndex = GIVEN_FLOWER_NOT_MINE;
						addFriendly = 0;
						break;
					}
					if(orgItem->m_data->item_type == ITEMTYPE_FLOWER && (target->GetSex() != 1))
					{
						msgIndex = GIVEN_FLOWER_NOT_SEX_MAN;
						addFriendly = 0;
						break;
					}
					if(orgItem->m_data->item_type == ITEMTYPE_WEAPON_MODEL && (target->GetSex() != 0))
					{
						msgIndex = GIVEN_FLOWER_NOT_SEX_WOMAN;
						addFriendly = 0;
						break;
					}
					addFriendly += orgItem->m_data->add_friendly;
				}
				++tempBagIndex;
			}
			//if (!msg.IsEmpty())
			//	break;
			if (msgIndex != GIVEN_SUCCESS)
				break;

			if ((target->GetMaxPartner() - target->m_pPartnerGroup->GetCount()) < (int)PartnerIDs.size())
			{
				msgIndex = GIVEN_PARTNER_FAIL; //msg = _T("MSG_GIVEN_PARTNER_FAIL");
				break;
			}
			if (target->IsInBattle() && PartnerIDs.size() > 0)
			{
				msgIndex = GIVEN_PARTNER_FAIL;
				break;
			}
			for (Index i = C_INDEX(0); i < PartnerIDs.size(); ++i)
			{
				CPartner* partner = m_pPartnerGroup->GetPartner(PartnerIDs[i]);
				if (!partner)
				{
					msgIndex = GIVEN_GIVE_NOTHING; //msg = _T("MSG_GIVEN_GIVE_NOTHING");
					break;
				}
				if (partner && partner->GetOwner_id() != GetChar_id())
				{
					msgIndex = GIVEN_GIVE_NOTHING; //msg = _T("MSG_GIVEN_GIVE_NOTHING");
					break;
				}
				if (partner && partner->GetSoulboundState() == SOULBOUND_ALREADY_BOUND )
				{
					msgIndex = GIVEN_PARTNER_BOUND; //msg = _T("MSG_GIVEN_PARTNER_BOUND");
					break;
				}
				if (partner && partner->GetLock()) // check if the partner is locked
				{
					msgIndex = GIVEN_PARTNER_LOCK; // msg = _T("MSG_GIVEN_PARTNER_LOCK"); 
					break;
				}
			}
			//if (!msg.IsEmpty())
			//	break;
			if (msgIndex != GIVEN_SUCCESS)
				break;

			if ((target->GetMaxPet() - target->m_pPetGroup->GetCount()) < (int)PetIDs.size())
			{
				msgIndex = GIVEN_PET_FAIL; //msg = _T("MSG_GIVEN_PET_FAIL");
				break;
			}
			if (target->IsInBattle() && PetIDs.size() > 0)
			{
				msgIndex = GIVEN_PET_FAIL;
				break;
			}
			for (Index i = C_INDEX(0); i < PetIDs.size(); ++i)
			{
				CPet* pet = m_pPetGroup->GetPet(PetIDs[i]);
				if (pet && pet->GetSoulbound() == SOULBOUND_ALREADY_BOUND)
				{
					msgIndex = GIVEN_PET_BOUND; //msg = _T("MSG_GIVEN_PET_BOUND");
					break;
				}
				if (pet && pet->GetLock()) // check if pet is locked
				{
					msgIndex = GIVEN_PET_LOCK; //msg = _T("MSG_GIVEN_PET_LOCK"); 
					break;
				}
			}
			//if (!msg.IsEmpty())
			//	break;
			if (msgIndex != GIVEN_SUCCESS)
				break;

			if (gmoney > GetMoney())
			{
				msgIndex = GIVEN_MONEY_FAIL; //msg = _T("MSG_GIVEN_MONEY_FAIL");
				break;
			}

			UInt32Vector logUIDs;
			UInt32Vector logIDs;
			UInt8Vector logItemTypes;
			UInt8Vector logStacks;

			UInt32Vector itemIDs;
			UInt32Vector changeOwnerItemIDs;	//the item only chnage the owner, such as the weapon
			UInt32Vector changeOwnerItemUIDs;
			
			for (Index i = C_INDEX(0); i < itemIndexs.size(); ++i)
			{
				CItem* orgItem = m_pBag->GetItem(itemIndexs[i]);
				if (orgItem && orgItem->m_data)
				{
					logUIDs.push_back(orgItem->GetItem_uid());
					logIDs.push_back(orgItem->GetItem_id());
					logItemTypes.push_back(LogService_Proxy::Exchange::EXCHANGE_ITEM);

					if (orgItem->m_data->item_type == ITEMTYPE_FLOWER || orgItem->m_data->item_type == ITEMTYPE_WEAPON_MODEL)
					{
						orgItem->SetItemValue1(1);
					}

					itemIDs.push_back(orgItem->GetItem_id());
					if (orgItem->m_data->overlap == 1)
					{
						m_pBag->RemoveItem(orgItem, BOOLEAN_FALSE);
						target->m_pBag->AddItem(orgItem, 0);
						changeOwnerItemUIDs.push_back(orgItem->GetItem_uid());
						changeOwnerItemIDs.push_back(orgItem->GetItem_id());
						logStacks.push_back(1);
					}
					else
					{							
						if (target->m_pBag->AddItem(orgItem, 0, itemStacks[i]))
						{								
							if (!orgItem->IsExist())
							{
								m_pBag->AddToDeleteGroup(orgItem->GetBagIndex());
							}
							else if (orgItem->m_data->item_type == ITEMTYPE_FLOWER || orgItem->m_data->item_type == ITEMTYPE_WEAPON_MODEL)
							{
								orgItem->SetItemValue1(0);
							}
							logStacks.push_back(itemStacks[i]);
						}
						else
						{	
							if (orgItem->m_data->item_type == ITEMTYPE_FLOWER || orgItem->m_data->item_type == ITEMTYPE_WEAPON_MODEL)
							{
								orgItem->SetItemValue1(0);
							}
							msgIndex = GIVEN_ITEM_NO_SLOT;//msg = _T("MSG_GIVEN_SLOT_FAIL");
							break;
						}
					}
					
				}
			}
			//if (!msg.IsEmpty())
			//	break;
			if (msgIndex != GIVEN_SUCCESS)
				break;
			if (itemIDs.size() > 0)
			{
				if (GetPeer())
					CB_GivenItem(GetPeer(), GetContext(), FALSE, target->GetNickName(), itemIDs, itemStacks);

				if (target->GetPeer())
					target->CB_GivenItem(target->GetPeer(), target->GetContext(), TRUE, GetNickName(), itemIDs, itemStacks);
				
				if (changeOwnerItemUIDs.size() > 0 && GetPeer())
				{
					m_pBag->ChangeOwnerID(changeOwnerItemUIDs, changeOwnerItemIDs, target->GetChar_id());
					m_pBag->CB_EqItemGiven(GetPeer(), GetContext(), changeOwnerItemUIDs);
				}
				m_pBag->RaiseUpdate();
				target->m_pBag->RaiseUpdate();
			}

			UInt32Vector changeCharPartnerIDs;
			NetGroup partnerGroups;
			NetGroup skillGroups;
			StringVector partnerNames;
			for (Index i = C_INDEX(0); i < PartnerIDs.size(); ++i)
			{
				CPartner* partner = m_pPartnerGroup->GetPartner(PartnerIDs[i]);
				if (partner)
				{
					if (partner->GetPartner_id() == GetPartner_id())
						SetPartner(0, 0, _T(""), 0);
					
					changeCharPartnerIDs.push_back(partner->GetPartner_id());
					partner->MarkSend(CPartner::AllMask);
					partnerGroups.Add(partner);
					skillGroups.Add(partner->GetSkillGroup());
					partnerNames.push_back(partner->GetRaname());

					logUIDs.push_back(partner->GetPartner_id());
					logIDs.push_back(partner->GetMob_id());
					logItemTypes.push_back(LogService_Proxy::Exchange::EXCHANGE_PARTNER);
					logStacks.push_back(1);
				}
			}

			if (partnerGroups.GetCount() > 0)
			{
				m_pPartnerGroup->ChangeOwnerID(changeCharPartnerIDs, target->GetChar_id());
				for (Index i = 0; i < partnerGroups.GetCount(); ++i)
				{
					CPartner* partner = partnerGroups.GetChild<CPartner>(i);
					if (partner)
					{
						partner->RemoveBuffSchool(PARTNER_COLLECTION_BUFF_SCHOOL_ID);
						ZeroMemory(&partner->m_AttriOfPartnerCollection, sizeof(AttriStruct2));
						partner->CalcAttrib();
						target->m_pPartnerGroup->Add(partner);
					}
				}
				target->ResetPartnerCollectionBuff();

				if (target->GetPeer())
				{
					target->m_pPartnerGroup->CB_AddPartnerGroup(target->GetPeer(), target->GetContext(), 
						&partnerGroups, &skillGroups);
					target->CB_GivenPartner(target->GetPeer(), target->GetContext(), TRUE, GetNickName(), partnerNames);
				}

				if (GetPeer())
				{
					if (changeCharPartnerIDs.size() > 0)
						m_pPartnerGroup->CB_DeleteListSuccess(GetPeer(), GetContext(), changeCharPartnerIDs, FALSE);	
			
					CB_GivenPartner(m_pPeer, m_context, FALSE, target->GetNickName(), partnerNames);
				}
			}

			UInt32Vector changeCharPetIDs;
			NetGroup petGroups;
			NetGroup petSkillGroups;
			StringVector petNames;

			for (Index i = C_INDEX(0); i < PetIDs.size(); ++i)
			{
				CPet* pet = m_pPetGroup->GetPet(PetIDs[i]);
				if (pet)
				{
					if (pet->GetPet_uid() == this->GetPet_uid())
						SetPet(0, 0, _T(""), 0);
									
					changeCharPetIDs.push_back(pet->GetPet_uid());
					
					pet->MarkSend(CPet::AllMask);
					petGroups.Add(pet);
					petSkillGroups.Add(pet->GetSkillGroup());
					petNames.push_back(pet->GetName());

					logUIDs.push_back(pet->GetPet_uid());
					logIDs.push_back(pet->GetPet_id());
					logItemTypes.push_back(LogService_Proxy::Exchange::EXCHANGE_PET);
					logStacks.push_back(1);
				}
			}

			if (petGroups.GetCount() > 0)
			{
				m_pPetGroup->ChangeOwnerID(changeCharPetIDs, target->GetChar_id());
				for (Index i = C_INDEX(0); i < petGroups.GetCount(); ++i)
					target->m_pPetGroup->Add(petGroups.GetChild<CPet>(i));

				if (target->GetPeer())
				{
					target->m_pPetGroup->CB_AddPets(target->GetPeer(), target->GetContext(), 
						&petGroups, &petSkillGroups);
					target->CB_GivenPet(target->GetPeer(), target->GetContext(), TRUE, GetNickName(), petNames);
				}

				if (GetPeer())
				{
					if (changeCharPetIDs.size() > 0)
						m_pPetGroup->CB_RemovePets(GetPeer(), GetContext(), changeCharPetIDs);	
			
					CB_GivenPet(m_pPeer, m_context, FALSE, target->GetNickName(), petNames);
				}
				target->m_pPetGroup->RaiseUpdate();
			}

			if (logUIDs.size() > 0)
			{
				LogService_Proxy::LogExchangeForItemList(gGlobal.pLogService->pPeer, context, 
					GetChar_id(), target->GetChar_id(), LogService_Proxy::Exchange::PROCESS_GIVEN, 
					logItemTypes, logUIDs, logIDs, logStacks);
			}

			if (gmoney > 0)
			{
				ChangeMoney(-(Int32)gmoney);
				target->ChangeMoney(gmoney);
				if (GetPeer())
					CB_GivenMoney(GetPeer(), GetContext(), FALSE, target->GetNickName(), gmoney);
				if (target->GetPeer())
					target->CB_GivenMoney(target->GetPeer(), target->GetContext(), TRUE, GetNickName(), gmoney);
				RaiseUpdate();
				target->RaiseUpdate();
				LogService_Proxy::LogExchange(gGlobal.pLogService->pPeer, context, GetChar_id(), 
					target->GetChar_id(), LogService_Proxy::Exchange::PROCESS_GIVEN, 
					LogService_Proxy::Exchange::EXCHANGE_MONEY, 0, 0, 0, gmoney, 0);
			}
		} while (false);

		CCharacter* target = FindNetObject<CCharacter>(targetId);
		if (target == NULL || target == this)
		{
			CB_GivenProcess(pPeer, context, BOOLEAN_FALSE, GIVEN_CHAR_FAIL);
			PROCEDURE_RETURN
		}
		PROCEDURE_STORE_VALUE3(UInt8,msgIndex,UInt32,addFriendly,UInt32,flowerCount);
		if ( target!= NULL && msgIndex == GIVEN_SUCCESS && addFriendly != 0)
		{
			MessageService_Proxy::ChangeFriendly(gGlobal.pMessageService->pPeer,context, 
				target->GetChar_id(), this->GetChar_id(), addFriendly);
		}
		else
		{
			PROCEDURE_GO_STEP(4);
		}
		PROCEDURE_WAIT(4, MessageService, CB_ChangeFriendly);
		PROCEDURE_RESTORE_VALUE3(UInt8,msgIndex,UInt32,addFriendly,UInt32,flowerCount);
		CCharacter* target = FindNetObject<CCharacter>(targetId);
		if (target == NULL || target == this)
		{
			CB_GivenProcess(pPeer, context, BOOLEAN_FALSE, GIVEN_CHAR_FAIL);
			PROCEDURE_RETURN
		}
		if ( target!= NULL && msgIndex == GIVEN_SUCCESS && addFriendly != 0)
		{
			if(HasLuaFunc(gGlobal.g_pLuaState, "OnGiveWeaponModelOrFlowerResult"))
			{
				CallLuaFunc(gGlobal.g_pLuaState, "OnGiveWeaponModelOrFlowerResult", String(), this,target,flowerCount);
			}
		}

		Boolean success = BOOLEAN_FALSE;

		if (msgIndex == GIVEN_SUCCESS)
			success = BOOLEAN_TRUE;

		CB_GivenProcess(pPeer, context, success, msgIndex);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT
			CB_GivenProcess(pPeer, context, BOOLEAN_FALSE, GIVEN_CHAR_FAIL);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

//ask the other character to do the trading
RPCResult CCharacter::AskForTrading(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId)
{
	UInt msgIndex = TRADING_ASKING;
	CCharacter* target = FindNetObject<CCharacter>(targetId);
	do
	{
		if (m_pScriptSys->IsRunning())
		{
			msgIndex = TRADING_FAIL_SCRIPT;
			break;
		}

		if (target == NULL)
		{
			msgIndex = TRADING_FAIL_CHAR_NOT_FIND;
			break;
		}

		if (target == this || target->GetChar_id() == this->GetChar_id())
		{
			msgIndex = TRADING_FAIL_MYSELF;
			break;
		}

		if (target->GetScriptSystem() && target->GetScriptSystem()->IsRunning())
		{
			msgIndex = TRADING_FAIL_SCRIPT;
			break;
		}

		if (target->GetEnableBlockTrading())
		{
			msgIndex = TRADING_FAID_BLOCK;
			break;
		}

		if (target->IsInBattle())
		{
			msgIndex = TRADING_FAIL_BATTLE;
			break;
		}

		if (InGuildWar() || target->InGuildWar())
		{
			msgIndex = TRADING_FAIL_GUILD_WAR;
			break;
		}

		if (InFreePKMap() || target->InFreePKMap())
		{
			msgIndex = TRADING_FAIL_NOT_SUIT_MAP;
			break;
		}
		if (!target->IsInMap() || !IsInMap())
		{
			msgIndex = TRADING_FAIL_CHAR_NOT_FIND;
			break;
		}

		if (target->GetAction() == ST_HAWK)
		{
			msgIndex = TRADING_FAIL_HAWKING;
			break;
		}

		if (m_pTrading)
		{
			msgIndex = TRADING_FAIL_ALREADY_TRADING;
			break;
		}

		if (target->IsTrading())
		{
			msgIndex = TRADING_FAIL_TARGET_ALREADY_TRADING;
			break;
		}

		Int x = 0;		
		x = GetCurPosX() - target->GetCurPosX();		
		x *= x;

		Int y = 0;
		y = GetCurPosY() - target->GetCurPosY();
		y *= y;

		if ( (x + y) > 2008)
		{
			msgIndex = TRADING_FAIL_DISTANCE_TOO_FAR;
			break;
		}
	}while (false);

	if (msgIndex != TRADING_ASKING)
	{
		CB_TradingMessage(pPeer, context, FALSE, msgIndex);
		CB_ReplyReqTrading(pPeer, context, FALSE);
		return RPC_RESULT_OK;
	}

	SetTradingTarget(targetId);
	target->SetTradingTarget(GetNetId());

	if (target->GetPeer())
		target->CB_AskForTrading(target->GetPeer(), target->GetContext(), GetNetId());
	
	RaiseUniqEventID(EVT_TRADING_REQ_TIME, TRADING_TIMELIMIT);

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("AskForTrading: sessionId %s, char_id %d ask char_id %d to trading"),
				context.sessionId.GetString().c_str(), GetChar_id(), target->GetChar_id());

	return RPC_RESULT_OK;
}


//reply to other character to accept the trading or not
//if yes, the trading pointer will point to the trading object 
//of the target character( using the same trading object for both character)
RPCResult CCharacter::ReplyReqTrading(LPCPEER pPeer, RPCContext &context, const Boolean &trade, const RPCNetID &targetId)
{
	CCharacter* target = FindNetObject<CCharacter>(targetId);
	CCharacter* tradingTarget = FindNetObject<CCharacter>(m_pTradingTargetNetID);
	UInt8 msgIndex = TRADING_ASKING;
	do 
	{
		if (target == NULL || tradingTarget == NULL )
		{
			msgIndex = TRADING_FAIL_CHAR_NOT_FIND;
			break;
		}

		if (m_pTrading)
		{
			msgIndex = TRADING_FAIL_ALREADY_TRADING;
			break;
		}

		if (target != tradingTarget)
		{
			tradingTarget->CB_ReplyReqTrading(tradingTarget->GetPeer(), tradingTarget->GetContext(), FALSE);
			tradingTarget->CB_TradingMessage(tradingTarget->GetPeer(), tradingTarget->GetContext(), FALSE, TRADING_FAIL_CHAR_NOT_FIND);
			msgIndex = TRADING_FAIL_CHAR_NOT_FIND;
			break;
		}

		if (tradingTarget->GetTradingTarget() != GetNetId())
		{
			msgIndex = TRADING_FAIL_CHAR_NOT_FIND;
			break;
		}

		if (tradingTarget->GetAction() == ST_HAWK)
		{
			tradingTarget->ClearReqTrading();
			tradingTarget->StopReqTradingTimeOutEvent();
			msgIndex = TRADING_FAIL_HAWKING;
			break;
		}

		if (tradingTarget->IsInBattle())
		{
			tradingTarget->ClearReqTrading();
			tradingTarget->StopReqTradingTimeOutEvent();
			msgIndex = TRADING_FAIL_BATTLE;
			break;
		}

		if (!IsInMap())
		{
			tradingTarget->ClearReqTrading();
			tradingTarget->StopReqTradingTimeOutEvent();
			msgIndex = TRADING_FAIL_CHAR_NOT_FIND;
			break;
		}

	}while(false);

	if (msgIndex != TRADING_ASKING)
	{
		CB_TradingMessage(pPeer, context, FALSE, msgIndex);
		CB_ReplyReqTrading(pPeer, context, FALSE);
		ResetTradingTarget();
		return RPC_RESULT_OK;
	}

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("ReplyReqTrading: sessionId %s, char_id %d reply char_id %d, %d "),
		context.sessionId.GetString().c_str(), GetChar_id(), target->GetChar_id(), (trade)?1:0);

	tradingTarget->StopReqTradingTimeOutEvent();
	if (trade)
	{
		m_pTrading = CTrading::NewInstance();
		tradingTarget->SetTradingObject(m_pTrading);

		m_pTrading->SetCharacterNetID1(tradingTarget->GetNetId());
		m_pTrading->SetCharacterNetID2(GetNetId());

		SetOwnObject(GetContext().sessionId, m_pTrading);
		SetOwnObject(tradingTarget->GetContext().sessionId, m_pTrading);

		CB_TradingObject(pPeer, context, m_pTrading);
		tradingTarget->CB_TradingObject(tradingTarget->GetPeer(), tradingTarget->GetContext(), m_pTrading);		
	}
	else
	{
		tradingTarget->CB_TradingMessage(tradingTarget->GetPeer(), tradingTarget->GetContext(), FALSE, TRADING_FAIL_REQ_CANCEL);
		ClearReqTrading();
	}
	return RPC_RESULT_OK;
}

void CCharacter::ClearReqTrading()
{
	CCharacter* tradingTarget = FindNetObject<CCharacter>(m_pTradingTargetNetID);
	if (tradingTarget)
	{
		tradingTarget->CB_ReplyReqTrading(tradingTarget->GetPeer(), tradingTarget->GetContext(), FALSE);
		tradingTarget->ResetTradingTarget();
	}
	ResetTradingTarget();
	m_pTrading = NULL;
	CB_ReplyReqTrading(GetPeer(), GetContext(), FALSE);
}

void CCharacter::ClearTrading()
{
	if (m_pTrading && m_pTrading->IsAllCharReady())
		m_pTrading->StopTrading(GetChar_id());
	else if (m_pTradingTargetNetID != RPCNetID())
		ClearReqTrading();

	SetTradingObject(NULL);
	ResetTradingTarget();
}

RPCResult CCharacter::DepositInBank(LPCPEER pPeer, RPCContext &context, const UInt32 &money)
{
    if (ChkDeptValid(money))
	{// if the amount is valid
		ChangeMoney(-(Int32)money);
		ChangeBankMoney(money);
		CB_DepositInBank(pPeer, context, TRUE);
		LogService_Proxy::LogExchange(gGlobal.pLogService->pPeer, context, GetChar_id(), 0, 
			LogService_Proxy::Exchange::PROCESS_BANK_DEPOSIT, LogService_Proxy::Exchange::EXCHANGE_MONEY,
			0, 0, 0, money, 0);
	}
	else// if not valid
		CB_DepositInBank(pPeer, context, FALSE);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::WithdrawFmBank(LPCPEER pPeer, RPCContext &context,  const UInt32 &money)
{
	if (ChkWithDrValid(money) && !GetBankLock())
	{// if the amount is valid
		ChangeMoney(money);
		ChangeBankMoney(-(Int32)money);
		CB_WithdrawFmBank(pPeer, context, TRUE);
		LogService_Proxy::LogExchange(gGlobal.pLogService->pPeer, context, GetChar_id(), 0, 
			LogService_Proxy::Exchange::PROCESS_BANK_WITHDRAW, LogService_Proxy::Exchange::EXCHANGE_MONEY,
			0, 0, 0, money, 0);
	}
	else// if not valid
		CB_WithdrawFmBank(pPeer, context, FALSE);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::AddStartHawkItem(LPCPEER pPeer, RPCContext &context, const HawkerItemInfo& itemInfo,const StartHawkAddType& addType)
{
	if(m_pHawkGroup)
	{
		m_pHawkGroup->AddStartHawkItemInfo(itemInfo,addType);

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("sessionId %s, char_id %d"),
			context.sessionId.GetString().c_str(), GetChar_id());
	}
	return RPC_RESULT_OK;
}


Boolean CCharacter::ChkDeptValid(UInt32 money)
{
	Boolean valid = TRUE;
	// now check if the input value is valid or not
	UInt32 pocketCash = GetMoney();
	UInt32 bankDeposit = GetBankMoney();
	if (money <= pocketCash)// if proposed deposit money <= cash in pocket
	{
		UInt32 maxMoney = 0;
		if(HasLuaFunc(gGlobal.g_pLuaState,"GetDepositMoneyMax"))
		{
			CallLuaFunc(gGlobal.g_pLuaState,"GetDepositMoneyMax", String(), this);
			maxMoney = (UInt32)gGlobal.g_TempResult;
		}

		if ((money + bankDeposit) > (maxMoney > 0 ? maxMoney : MAX_DEPOSIT) ) // if the total deposit exceeds 10 million
			valid = FALSE; // the input value is invalid
	}
	else// if the proposed deposit amount exceed what player has in his pocket 
		valid = FALSE;

	return valid;

}
Boolean CCharacter::ChkWithDrValid(UInt32 money)
{
	Boolean valid = TRUE;	
	// now check if the input value is valid or not
	UInt32 bankDeposit = GetBankMoney();
	if (money > bankDeposit)
	{// if the withdrawal amount exceed what player has in his bank saving account 
		valid = FALSE;	
	}
	return valid;
}

void CCharacter::AntiHack()
{
	PROCEDURE_DO_METHOD(CCharacter, DoAntiHack, this, m_context)
}

RPCResult CCharacter::DoAntiHack(RPCContext& context)
{
	PROCEDURE_START_METHOD(CCharacter, AntiHack, this, context)

		context.bCallback = BOOLEAN_FALSE;
		CB_AntiHack(m_pPeer, context, Reuben::System::ByteArray(), AntiHackData());
		PROCEDURE_SET_TIMEOUT(gConfigure.antiHackTimeout, BOOLEAN_TRUE)

	PROCEDURE_WAIT_OBJ2(1, Character, AnswerAntiHack, pchar, UInt8, answer, Boolean, is_right)
		
		PROCEDURE_SET_TIMEOUT(30000, PCD_DEFAULT_TIMEOUT_FAIL)

		UInt16 reward = 0;
		if (is_right)
		{
			if (gGlobal.m_rand.GetRandom(4) == 0)
			{
				reward = 1;
				m_hpReward = BOOLEAN_TRUE;
			}
			else if (gGlobal.m_rand.GetRandom(4) == 0)
			{
				reward = 2;	
				m_expReward = BOOLEAN_TRUE;	
			}
			m_WrongAns = 0;
		}
		else
		{
			++m_WrongAns;
			m_noHackPopUpTime = time(NULL);
		}
		CB_ReportAntiHack(m_pPeer, context, is_right, m_WrongAns, reward);	
		if (m_WrongAns >= 3)
		{
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("force logout, session_id %s, char_id %d"),
				context.sessionId.GetString().c_str(), GetChar_id());
			m_WrongAns = 0;
			m_noHackPopUpTime = time(NULL) + gConfigure.noAntiHackTime;
			ForceLogout(_T("MSG_VERIFY_FAILED"));
		}

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT

			++m_WrongAns;
			CB_ReportAntiHack(m_pPeer, context, BOOLEAN_FALSE, m_WrongAns, 0);
			if (m_WrongAns >= 3)
			{
				TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("force logout, session_id %s, char_id %d"),
					context.sessionId.GetString().c_str(), GetChar_id());
				m_WrongAns = 0;
				ForceLogout(_T("MSG_VERIFY_FAILED"));
			}

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::AnswerAntiHack(LPCPEER pPeer, RPCContext &context,  const UInt8 &answer, const Boolean &is_right)
{
	return RPC_RESULT_OK;	        
}

void CCharacter::ForceLogout(PCSTRING pForceMsg)
{
	if (m_pPeer)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Force Logout, sessionId %s"), m_context.sessionId.GetString().c_str());
		MapService_Stub::CB_ForceLogout(m_pPeer, m_context, pForceMsg);
		ClearPeer();
	}
}

bool CCharacter::AddEquipmentBuff(UInt8 index, UInt16 Buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro)
{
	if (m_pBag)
	{
		CItem * pItem = m_pBag->GetItem(BAG_MAX_ITEM + index);
		if (pItem)
		{
			pItem->AddBuff(Buff_id, Buff_Value1, Buff_Value2, Buff_Value3, Buff_CoolDown, Pro);
			m_pBag->RaiseUpdate();
			return true;
		}
	}
	return false;
}

bool CCharacter::HasEquipment(UInt8 index)
{
	if (m_pBag)
	{
		CItem * pItem = m_pBag->GetItem(BAG_MAX_ITEM + index);
		if (pItem)
			return true;
	}
	return false;
}

//////////////////////////////////
//		Hawk method				//
//////////////////////////////////

RPCResult CCharacter::StartHawk(LPCPEER pPeer, RPCContext &context, const String &hawkName, const HawkerItemInfoVector &itemList, const HawkerItemInfoVector &partnerList, const HawkerItemInfoVector &petList)
{
	if (m_pParty)
	{
		NetGroup temp;
		CB_StartHawk(pPeer, context, &temp, HAWK_CANT_HAVE_PARTY);
	}	
	else if (m_pMap && (!m_pMap->IsHawkZone(GetCurPosX(), GetCurPosY() ) || m_pMap->IsHawkOccupied(GetCurPosX(), GetCurPosY() ) ) )
	{
		NetGroup temp;
		if (!m_pMap->IsHawkZone(GetCurPosX(), GetCurPosY() ))
			CB_StartHawk(pPeer, context, &temp, HAWK_NOT_RIGHT_PLACE);	
		else
			CB_StartHawk(pPeer, context, &temp, HAWK_PLACE_HAVE_HAWKER);	
	}
	else
	{
		for (Index i = C_INDEX(0); i < itemList.size(); ++i)
		{
			CItem *item = NULL;
			if (m_pBag)
				item = m_pBag->GetItemByUID(itemList[i].item_uid);
			if (item && item->GetLock())
			{
                NetGroup temp;
				CB_StartHawk(pPeer, context, &temp, HAWK_ITEM_LOCK);
				return RPC_RESULT_OK;
			}

			if (item && (item->m_data->item_type == ITEMTYPE_FLOWER || item->m_data->item_type == ITEMTYPE_WEAPON_MODEL) && item->GetItemValue1() != 0)
			{
				NetGroup temp;
				CB_StartHawk(pPeer, context, &temp, HAWK_ITEM_FLOWER_MODEL);
				return RPC_RESULT_OK;
			}
		}
		for (Index i = C_INDEX(0); i < partnerList.size(); ++i)
		{
			CPartner* partner = NULL;
			if (m_pPartnerGroup)
				partner = m_pPartnerGroup->GetPartner(partnerList[i].item_uid);	
			if (partner && partner->GetLock())
			{
                NetGroup temp;
				CB_StartHawk(pPeer, context, &temp, HAWK_PARTNER_LOCK);
				return RPC_RESULT_OK;	
			}
		}
		for (Index i = C_INDEX(0); i < petList.size(); ++i)
		{
			CPet* pet = NULL;
			if (m_pPetGroup)
				pet = m_pPetGroup->GetPet(petList[i].item_uid);	
			if (pet && pet->GetLock())
			{
                NetGroup temp;
				CB_StartHawk(pPeer, context, &temp, HAWK_PET_LOCK);
				return RPC_RESULT_OK;	
			}
		}

		if (m_pHawkGroup == NULL)
			m_pHawkGroup = CHawkGroup::NewInstance();
		
		m_pHawkGroup->SetHawkName(hawkName);
		m_pHawkGroup->SetHawker(this);
		m_pHawkGroup->SetHawkItem(itemList, partnerList, petList);
		SetAction(ST_HAWK, ::timeGetTime());
		SetHawkInfo(_T(""));
		SetHawkInfo(hawkName);
		m_pHawkGroup->SetOwnerID(GetChar_id());
		m_pHawkGroup->PreSend();
		m_pHawkGroup->AddCharContext(GetChar_id(), GetContext());
		CB_StartHawk(pPeer, context, m_pHawkGroup, HAWK_START);
		CancelAllRequests();

//		ActivatePartnerCollectionBuff(false);

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("sessionId %s, char_id %d"),
				context.sessionId.GetString().c_str(), GetChar_id());
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::GetHawkFromTarget(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	CCharacter* pchar = gGlobal.m_charToSession.FindCharacter(targetId);
	if (pchar && pchar->m_pHawkGroup && IsInMap() && pchar->IsInMap())
	{
		Boolean self = false;
		if (targetId == GetChar_id())
			self = true;
		NetGroup itemGroup, partnerGroup, petGroup;
		UInt16Vector partnerSkillID, petSkillID;
		pchar->m_pHawkGroup->GetItemNetGroup(&itemGroup);
		pchar->m_pHawkGroup->GetPartnerNetGroup(&partnerGroup, partnerSkillID);
		pchar->m_pHawkGroup->GetPetNetGroup(&petGroup, petSkillID);
		pchar->m_pHawkGroup->PreSend();
		pchar->m_pHawkGroup->AddCharContext(GetChar_id(), GetContext());
		CB_GetHawk(pPeer, context, self, pchar->m_pHawkGroup, &itemGroup, &partnerGroup, partnerSkillID, &petGroup, petSkillID);
		CB_InitTargetHawkConcernState(pPeer, context,targetId);
		DeleteNetChilds(&partnerGroup);
		PROCEDURE_DO_METHOD1(CCharacter, DrawTargetUidFromHawk, this, m_context, UInt32, targetId)
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::GetHawkHistoryGroup(RPCContext &context)
{
	PROCEDURE_START_METHOD(CCharacter, GetHawkHistoryGroup, this, context)
	
		if (m_hawkerHistoryMap.size() > 0)
		{
			SendAllHistoryToClient();
			PROCEDURE_RETURN
		}

		ObjectService_Proxy::GetHawkerHistory(gGlobal.pObjectService->pPeer, context, GetChar_id());

	PROCEDURE_WAIT1(1, ObjectService, CB_GetHawkerHistory, NetGroup*, historyGrp)

		for (Index i = C_INDEX(0); i < historyGrp->GetCount(); ++i)
		{
			CHawkerHistoryData* pData = historyGrp->GetChild<CHawkerHistoryData>(i);
			if (pData)
			{
				HawkerHistoryInfo pHistory ;
				pHistory.data.historyId = pData->GetHistoryId();
				pHistory.data.char_id = pData->GetChar_id();
				pHistory.data.itemType = pData->GetItemType();
				pHistory.data.stack = pData->GetStack();
				pHistory.data.totalPrice = pData->GetTotalPrice();
				pHistory.data.tax = pData->GetTax();
				pHistory.data.historyDate = pData->GetHistoryDate();
				pHistory.itemName = pData->GetItemName();		
				m_hawkerHistoryMap.insert(std::make_pair(pHistory.data.historyDate, pHistory));
			}
		}

		SendAllHistoryToClient();
		DeleteNetGroup(historyGrp, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("GetHawkHistoryGroup Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::ClearHawkerHistory(LPCPEER pPeer, RPCContext &context)
{
	m_hawkerHistoryMap.clear();
	return RPC_RESULT_OK;
}

void CCharacter::SendAllHistoryToClient()
{
	StringVector itemNames;
	HawkerHistoryVector historys;
	for (HawkerHistoryInfoMap::reverse_iterator itr = m_hawkerHistoryMap.rbegin(); 
			itr != m_hawkerHistoryMap.rend(); ++itr)
	{
		itemNames.push_back(itr->second.itemName);
		historys.push_back(itr->second.data);
	}

	CB_HawkHistory(GetPeer(), GetContext(), historys, itemNames);
}

void CCharacter::AddHistory(String itemName, UInt8 itemType, UInt8 stack, UInt32 totalPrice, UInt32 tax)
{
	PROCEDURE_DO_METHOD5(CCharacter, AddHistory, this, GetContext(), String, itemName, UInt8, itemType, UInt8, stack, UInt32, totalPrice, UInt32, tax)
}

RPCResult CCharacter::AddHistory(RPCContext &context)
{
	PROCEDURE_START_METHOD5(CCharacter, AddHistory, this, context, String, itemName, UInt8, itemType, UInt8, stack, UInt32, totalPrice, UInt32, tax)

		CHawkerHistoryData* newData = CreateNetObject<CHawkerHistoryData>();
		CHawkerHistoryData* deleteData = NULL;
		NetGroup updateGroup;
		newData->SetHistoryInfo(GetChar_id(), itemName, itemType, stack, totalPrice, tax);
		newData->SetSendMask(newData->GetUpdateMask());
		updateGroup.Add(newData);

		UInt32Vector deleteHistoryID;
		deleteHistoryID.clear();
		
		while(m_hawkerHistoryMap.size() >= MAX_HISTORY)
		{
			HawkerHistoryInfoMap::iterator itr = m_hawkerHistoryMap.begin();
			if (itr != m_hawkerHistoryMap.end())
			{
				deleteData = CreateNetObject<CHawkerHistoryData>();				
				deleteData->SetHistoryId(itr->second.data.historyId);
				deleteData->SetChar_id(0);
				deleteData->SetSendMask(deleteData->GetUpdateMask());
				deleteHistoryID.push_back(itr->second.data.historyId);
				m_hawkerHistoryMap.erase(itr);
				updateGroup.Add(deleteData);
			}
		}
		
		ObjectService_Proxy::UpdateHawkerHistory(gGlobal.pObjectService->pPeer, context, GetChar_id(), &updateGroup);

		PROCEDURE_STORE_VALUE1(UInt32Vector, deleteHistoryID)

		DeleteNetChilds(&updateGroup);

	PROCEDURE_WAIT1(1, ObjectService, CB_UpdateHawkerHistory, NetGroup*, newHistorys)

		PROCEDURE_RESTORE_VALUE1(UInt32Vector, deleteHistoryID)

		StringVector itemNames;
		HawkerHistoryVector historys;

		for (Index i = C_INDEX(0); i < newHistorys->GetCount(); ++i)
		{
			CHawkerHistoryData* pData = newHistorys->GetChild<CHawkerHistoryData>(i);
			if (pData)
			{
				HawkerHistoryInfo pHistory ;
				pHistory.data.historyId = pData->GetHistoryId();
				pHistory.data.char_id = pData->GetChar_id();
				pHistory.data.itemType = pData->GetItemType();
				pHistory.data.stack = pData->GetStack();
				pHistory.data.totalPrice = pData->GetTotalPrice();
				pHistory.data.tax = pData->GetTax();
				pHistory.data.historyDate = pData->GetHistoryDate();
				pHistory.itemName = pData->GetItemName();		
				m_hawkerHistoryMap.insert(std::make_pair(pHistory.data.historyDate, pHistory));
				historys.push_back(pHistory.data);
				itemNames.push_back(pHistory.itemName);
			}
		}
			
		CB_UpdateHawkHistory(GetPeer(), GetContext(), historys, itemNames, deleteHistoryID);
		
		DeleteNetGroup(newHistorys, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("GetHawkHistoryGroup Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::SetListen(LPCPEER pPeer, RPCContext &context, const BOOL &IsListen)
{
	m_Listen = IsListen;
	return RPC_RESULT_OK;
}

RPCResult CCharacter::DonateMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &money)
{
	const UInt maxDonateMoney = 1000000;
	UInt32 exactMoney = money;
	if (exactMoney > maxDonateMoney)
		exactMoney = maxDonateMoney;

	if (GetMoney() >= exactMoney)
	{
		PROCEDURE_DO_METHOD2(CCharacter, DonateMoney, this, context, UInt32, guild_uid, UInt32, exactMoney)
	}
	else if (m_pPeer)
		CB_Error(m_pPeer, m_context, 0 , "MSG_GUILD_DONATE_NOT_ENOUGH");
	return RPC_RESULT_OK;
}

RPCResult CCharacter::DonateMoney(RPCContext &context)
{
	PROCEDURE_START_METHOD2(CCharacter, DonateMoney, this, context, UInt32, guild_uid, UInt32, money)
		
		MessageService_Proxy::DonateMoney(gGlobal.pMessageService->pPeer, context, guild_uid, GetChar_id(), money);

	PROCEDURE_WAIT3(1, MessageService, CB_DonateMoney, UInt32, addedDonate, UInt8 ,returnFlag, UInt32,  addGMoney)
	
		if (returnFlag == GUILD_DONATE_RESULT_SUCC)
		{
			ChangeMoney(- (Int) addGMoney);
			CB_DonateResult(GetPeer(), GetContext(), addedDonate, returnFlag, addGMoney);
		}
		else
			CB_DonateResult(GetPeer(), GetContext(), addedDonate, returnFlag, 0);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("RemoveTitle Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CreateGuild(LPCPEER pPeer, RPCContext &context, const String &name, const bool &cancel)
{
	if (m_OpeningGuild)
	{
		if (!cancel)
		{
			PROCEDURE_DO_METHOD1(CCharacter, CreateGuild, this, m_context, String, name)			
		}
		m_OpeningGuild = FALSE;
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CreateGuild(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CCharacter, UpdateToObject, this, context, String, name)

		MessageService_Proxy::CreateGuild(gGlobal.pMessageService->pPeer, context, GetChar_id(), name, GetNickName(), GetNickName()); 
		m_creatingGuild = TRUE;

	PROCEDURE_WAIT1(1, MessageService, CB_CreateGuild, UInt8, result)

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F(" result: %d char_id: %d, name: %s"), result, GetChar_id(), name.c_str());
		m_creatingGuild = FALSE;
	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(MessageService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CreateGuild Error: MessageService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);
		m_creatingGuild = FALSE;

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

//////////////////////////////////////////////
//					GiftBox					//
//////////////////////////////////////////////

void CCharacter::OpenGiftBox()
{
	if (m_pGiftBox)
	{
		UnsetOwnObject(m_context.sessionId, m_pGiftBox);
		DeleteNetGroup(m_pGiftBox, BOOLEAN_TRUE);
	}
	m_pGiftBox = NULL;

	PROCEDURE_DO_METHOD(CCharacter, OpenGiftBox, this, m_context)
}

RPCResult CCharacter::OpenGiftBox(RPCContext &context)
{
	PROCEDURE_START_METHOD(CCharacter, OpenGiftBox, this, context)
		
		ObjectService_Proxy::GetGiftBoxByAcctID(gGlobal.pObjectService->pPeer, context, GetAcct_id());

	PROCEDURE_WAIT1(1, ObjectService, CB_GetGiftBoxByAcctID, NetGroup*, giftGroup)

		m_pGiftBox = CGiftBox::NewInstance();
		m_pGiftBox->SetGiftBox(giftGroup, true);
		m_pGiftBox->SetOwner(this);
		SetOwnObject(m_context.sessionId, m_pGiftBox);
		CB_OpenGiftBox(GetPeer(), m_context, m_pGiftBox);

		DeleteNetGroup(giftGroup, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("OpenGiftBox Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CCharacter::OpenGuildApplyMenu()
{
	MessageService_Proxy::OpenGuildApply(gGlobal.pMessageService->pPeer, m_context, GetChar_id());
}

void CCharacter::RemoveGuildApply()
{
	MessageService_Proxy::RemoveAllGuildApplicant(gGlobal.pMessageService->pPeer, m_context, GetChar_id()); 
}

bool CCharacter::CreateGuild(PCSTRINGA nameA)
{
	String name;
	UTF8ToStr(nameA, name);

	if (name.empty())
		return false;

	//cheat whether has guild
	if (HasGuild())
	{
		if (m_pPeer)
			CB_Error(m_pPeer, m_context, 0 , "MSG_ALREADY_HAVE_GUILD");
		return false;
	}

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CreateGuild: sessionId %s, acct_id %d, char_id %d, guild_name: %s"), 
		m_context.sessionId.GetString().c_str(), GetAcct_id(), GetChar_id(), name.c_str());
	//cheat duplicate name
	for (GuildMap::iterator Itr = gGlobal.g_AllGuilds.begin(); 
		Itr != gGlobal.g_AllGuilds.end(); ++Itr)
	{
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CreateGuild: check info sessionId %s, acct_id %d, char_id %d, key: %d"), 
			m_context.sessionId.GetString().c_str(), GetAcct_id(), GetChar_id(), Itr->first);
		TRACE_INFODTL_6(GLOBAL_LOGGER, _F("CreateGuild: check dup name sessionId %s, acct_id %d, char_id %d, guild_name: %s cur_guild_id: %d, cur_guild_name: %s"), 
			m_context.sessionId.GetString().c_str(), GetAcct_id(), GetChar_id(), name.c_str(), Itr->second->GetGuild_uid(), Itr->second->GetName().c_str());
		if (Itr->second->GetName() == name)
		{			
			if (m_pPeer)
				CB_Error(m_pPeer, m_context, 0 , "MSG_CREATE_GUILD_FAIL_NAME_DUP");
			return false;
		}
	}
	
	if (gGlobal.IsBlockName(name) || name.size() > MAX_GUILD_NAME_SIZE)
	{
		if (m_pPeer)
			CB_Error(m_pPeer, m_context, 0 , "MSG_CREATE_GUILD_FAIL_NAME_INVALID");
		return false;
	}
	PROCEDURE_DO_METHOD1(CCharacter, CreateGuild, this, m_context, String, name)	

	return true;

}

void CCharacter::OpenCreateGuildMenu()
{
	m_OpeningGuild = TRUE;
	CB_OpenCreateGuild(GetPeer(), m_context);
}

UInt32 CCharacter::GetGuildUid()
{
	return (m_ownGuild ? m_ownGuild->GetGuild_uid() : 0);
}

UInt32 CCharacter::GetOppGuildUid()
{
	if (m_ownGuild)
	{
		for (GuildMap::iterator Itr = gGlobal.g_AllGuilds.begin();
		Itr != gGlobal.g_AllGuilds.end(); Itr ++)
		{
			if (m_ownGuild != Itr->second && 
				m_ownGuild->GetGuildPvPGroup() == Itr->second->GetGuildPvPGroup())
			{
				return Itr->second->GetGuild_uid();
			}
		}
	}
	return 0;
}

void CCharacter::CalMaxVP()
{
	CallLuaFunc(gGlobal.g_pLuaState, "CalPlayerMaxVP", String(), this);
	SetVP(WITHIN(GetVP(), 0, (UInt16)gGlobal.g_TempResult), (UInt16)gGlobal.g_TempResult);
}

RPCResult CCharacter::ChangeYuanBao(RPCContext &context)
{	
	PROCEDURE_START_METHOD1(CCharacter, ChangeYuanBao, this, context, Int32, change)

		PROCEDURE_WAIT_TIMEOUT(1, 1000) //delay 1 second

		ObjectService_Proxy::UpdateYuanBao(gGlobal.pObjectService->pPeer, context, GetAcct_id(), change);

		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("ChangeYuanBao: sessionId %s, acct_id %d, char_id %d, exist yuanbao %d, change %d"), 
			context.sessionId.GetString().c_str(), GetAcct_id(), GetChar_id(), GetYuanBao(), change);

	PROCEDURE_WAIT2(2, ObjectService, CB_UpdateYuanBao, UInt32, yuanbao, Boolean, buySuccess)

		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("ChangeYuanBao: sessionId %s, acct_id %d, char_id %d, exist yuanbao %d, updated yuanbao %d"), 
			context.sessionId.GetString().c_str(), GetAcct_id(), GetChar_id(), GetYuanBao(), yuanbao);

		SetYuanBao(yuanbao);
		RaiseUpdate();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("ChangeYuanBao Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::EnterQuestion(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CCharacter::EnterQuestion"));
	CQuestionControl::GetInstance().OnQuestionEnter(this);
	return RPC_RESULT_OK;
}
RPCResult CCharacter::ExitQuestion(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CCharacter::ExitQuestion"));
	CQuestionControl::GetInstance().OnQuestionExit(m_pQuestion);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::UpdateYuanBao(LPCPEER pPeer, RPCContext &context)
{
	PROCEDURE_START_RPC(Character, UpdateYuanBao, this)

		if (m_bIsUpdatedYuanBao)
		{
			PROCEDURE_RETURN
		}

		ObjectService_Proxy::GetYaunBaoByAcctID(gGlobal.pObjectService->pPeer, context, GetAcct_id());

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("sessionId %s, acct_id %d, char_id %d, exist yuanbao %d"), 
			context.sessionId.GetString().c_str(), GetAcct_id(), GetChar_id(), GetYuanBao());

	PROCEDURE_WAIT4(2, ObjectService, CB_GetYuanBaoByAcctID, UInt32, yuanbao, UInt32, yuanbaoTicket, Boolean, lock, UInt32, locktime)

		TRACE_INFODTL_6(GLOBAL_LOGGER, _F("sessionId %s, acct_id %d, char_id %d, exist yuanbao %d, updated yuanbao %d yuanbaoTicket %d"), 
			context.sessionId.GetString().c_str(), GetAcct_id(), GetChar_id(), GetYuanBao(), yuanbao, yuanbaoTicket);

		SetYuanBao(yuanbao, yuanbaoTicket);
		m_bIsUpdatedYuanBao = true;
		RaiseUniqEventID(EVT_UPDATE_YUANBAO_TIMEOUT, gConfigure.uGiftBoxRefreshTime); //1 min
		
	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}


////////////////////////////
//		wareHouse		  //
////////////////////////////

void CCharacter::OpenWareHouse()
{
	CancelUniqEvent(EVT_CLEAR_WAREHOUSE);
	if (m_bCacheWareHouse)
	{
		NetGroup itemGrp, partnerGrp, petGrp;
		NetGroup partnerSkillGroup, petSkillGroup;

		m_pBag->GetAllWareHouse(&itemGrp);
		m_pPartnerGroup->GetAllWareHousePartner(&partnerGrp, &partnerSkillGroup);
		m_pPetGroup->GetAllWareHousePet(&petGrp, &petSkillGroup);

		CB_OpenWareHouse(GetPeer(), GetContext(), &itemGrp, &partnerGrp, &petGrp, &partnerSkillGroup, &petSkillGroup);
	}
	else
	{
		PROCEDURE_DO_METHOD(CCharacter, OpenWareHouse, this, GetContext())
	}
}

RPCResult CCharacter::OpenWareHouse(RPCContext &context)
{
	PROCEDURE_START_METHOD(CCharacter, OpenWareHouse, this, context)

		ObjectService_Proxy::GetItemsByOwner(gGlobal.pObjectService->pPeer, context, 
		GetChar_id(), 1, CItemData::DETAIL_MAP);

	PROCEDURE_WAIT2(1, ObjectService, CB_GetItemGroup, NetGroup*, items, UInt8, detail)

		m_pBag->SetWareHouseItem(items);

		DeleteNetGroup(items, BOOLEAN_TRUE);

		ObjectService_Proxy::GetPartnersByCharID(gGlobal.pObjectService->pPeer, context, 
			GetChar_id(), 1, CPartnerData::DETAIL_MAP);

	PROCEDURE_WAIT2(2, ObjectService, CB_GetPartnerGroup, NetGroup*, partnerGroup, BYTE, detail)

		m_pPartnerGroup->SetWareHousePartner(partnerGroup);
		DeleteNetGroup(partnerGroup, BOOLEAN_TRUE);

		ObjectService_Proxy::GetPetList(gGlobal.pObjectService->pPeer, context, GetChar_id(), 1);

	PROCEDURE_WAIT1(3, ObjectService, CB_PetList, NetGroup*, petDataGroup)

		m_pPetGroup->SetWareHousePet(petDataGroup);
		DeleteNetGroup(petDataGroup, BOOLEAN_TRUE);

		UInt32Vector idVector;
		UInt8Vector skillTypeVector;
		m_pPartnerGroup->GetWareHousePartnerID(idVector);

		for (Index i = 0; i < idVector.size(); ++i)
			skillTypeVector.push_back(SKILL_OWNER_TYPE_PARTNER);

		m_pPetGroup->GetWareHousePetUID(idVector);
		for (UInt i = (UInt)skillTypeVector.size(); i < (UInt)idVector.size(); ++i)
			skillTypeVector.push_back(SKILL_OWNER_TYPE_PET);

		ObjectService_Proxy::GetSkillList(gGlobal.pObjectService->pPeer, context, idVector, skillTypeVector);

	PROCEDURE_WAIT1(4, ObjectService, CB_GetSkillList, NetGroup*, skillDataGroups)
		
		NetGroup itemGrp, partnerGrp, petGrp;
		NetGroup partnerSkillGroup, petSkillGroup;
		
		m_pPartnerGroup->SetAllPartnerSkills(skillDataGroups, NULL, false, 1);
		m_pPetGroup->SetAllPetSkills(skillDataGroups, NULL, false, 1);
		DeleteNetGroup(skillDataGroups, BOOLEAN_TRUE);

		m_pBag->GetAllWareHouse(&itemGrp);
		m_pPartnerGroup->GetAllWareHousePartner(&partnerGrp, &partnerSkillGroup);
		m_pPetGroup->GetAllWareHousePet(&petGrp, &petSkillGroup);

		CB_OpenWareHouse(GetPeer(), GetContext(), &itemGrp, &partnerGrp, &petGrp, &partnerSkillGroup, &petSkillGroup);

		m_bCacheWareHouse = true;

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CloseWareHouse(LPCPEER pPeer, RPCContext &context)
{
	RaiseUniqEventID(EVT_CLEAR_WAREHOUSE, 10 * 60 * 1000 ); //10min
	return RPC_RESULT_OK;
}

///////////////////////////////////////
//         Quest methods             //
///////////////////////////////////////

Boolean CCharacter::HasFollower(UInt32 mob_id)
{
	const UInt32Vector &followerList = GetFollowerList();
	for (UInt32Vector::const_iterator it = followerList.begin(); it != followerList.end(); ++it)
	{
		if (*it == mob_id)
			return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}

UInt32 CCharacter::GetFollowerId(UInt16 i)
{
	const UInt32Vector &followerList = GetFollowerList();
	if (i < (UInt16) followerList.size())
		return followerList[i];
	else
		return 0;
}

UInt16 CCharacter::GetFollowerCount(UInt32 mob_id)
{
	const UInt32Vector &followerList = GetFollowerList();
	return ((UInt16) std::count(followerList.begin(), followerList.end(), mob_id));
}

UInt16 CCharacter::GetFollowerTotal()
{
	return ((UInt16) GetFollowerList().size());
}

void CCharacter::AddMobFollower(UInt32 mob_id)
{
	UInt32Vector followerList = GetFollowerList();
	followerList.push_back(mob_id);
	SetFollowerList(followerList);
}

void CCharacter::RemoveMobFollower(UInt32 mob_id)
{
	UInt32Vector followerList = GetFollowerList();
	for (UInt32Vector::iterator it = followerList.begin(); it != followerList.end(); ++it)
	{
		if (*it == mob_id)
		{
			followerList.erase(it);
			SetFollowerList(followerList);
			return;
		}
	}
}

void CCharacter::ClearMobFollower()
{
	if (!GetFollowerList().empty())
	{
		UInt32Vector followerList;
		SetFollowerList(followerList);
	}
}

RPCResult CCharacter::ChangeAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &oldPassword, const ByteArray &newPassword)
{
	PROCEDURE_START_RPC2(Character, ChangeAssetPassword, this, ByteArray, oldPassword, ByteArray, newPassword)
		// asset protection operation cannot be performed under the following condition
		if (GetScriptSystem() && GetScriptSystem()->IsRunning())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_SCRIPT_ERR, 0, 0);
			PROCEDURE_RETURN
		}
		if (!IsInMap())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_CHAR_ERR, 0, 0);
			PROCEDURE_RETURN
		}
		if (IsInBattle())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_BATTLE_ERR, 0, 0);
			PROCEDURE_RETURN
		}
		if (IsTrading())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_TRADE_ERR, 0, 0);
			PROCEDURE_RETURN
		}
		if (IsHawking())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_HAWK_ERR, 0, 0);
			PROCEDURE_RETURN
		}

		if ((oldPassword.GetSize() != 32) || (newPassword.GetSize() != 32))
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_PASS_ERR, 0, 0);
			PROCEDURE_RETURN
		}
        // get the existing asset password from the object service
		ObjectService_Proxy::GetAssetPassword(gGlobal.pObjectService->pPeer, context, GetAcct_id());

	PROCEDURE_WAIT1(1, ObjectService, CB_GetAssetPassword, ByteArray, correctPassword)
		// before makes any change to the existing password, player has to enter the correct existing password first
		if ((correctPassword.GetSize() == 32) && (oldPassword == correctPassword))
		{   // player has entered the correct password, go to assign the new password to replace the old one.
			AssignAssetPassword(newPassword, BOOLEAN_TRUE);	
		}
		else
			CB_SetAssetPassword(pPeer, context, BOOLEAN_FALSE);	

	PROCEDURE_CATCH
	
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
		
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("ChangeAssetPassword Error: ObjectService:CB_Error, errcode %d, %s"), 
					err, errMsg.c_str());
			
		PROCEDURE_EXCEPTION_DEFAULT

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::SetAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &password)
{
	
	AssignAssetPassword(password, BOOLEAN_FALSE);	
	
	return RPC_RESULT_OK;
}

void CCharacter::AssignAssetPassword(const ByteArray &password, const Boolean &bChangePassword)
{
	PROCEDURE_DO_METHOD2(CCharacter, AssignAssetPassword, this, GetContext(),
				ByteArray, password, Boolean, bChangePassword)
}

RPCResult CCharacter::AssignAssetPassword(RPCContext &context)
{
	PROCEDURE_START_METHOD2(Character, AssignAssetPassword, this, context, ByteArray, password, Boolean, bChangePassword)
		
		// "bChangePassword == true" means player asks to replace the old password with the new one, while if "bChangePassword == false", it
		// means there is no existing password at all so player has to set one.

		if (GetScriptSystem() && GetScriptSystem()->IsRunning())
		{
			CB_AssetProtectResult(GetPeer(), context, ASSET_LOCK_SCRIPT_ERR, 0, 0);
			PROCEDURE_RETURN
		}
		if (!IsInMap())
		{
			CB_AssetProtectResult(GetPeer(), context, ASSET_LOCK_CHAR_ERR, 0, 0);
			PROCEDURE_RETURN
		}
		if (IsInBattle())
		{
			CB_AssetProtectResult(GetPeer(), context, ASSET_LOCK_BATTLE_ERR, 0, 0);
			PROCEDURE_RETURN
		}
		if (IsTrading())
		{
			CB_AssetProtectResult(GetPeer(), context, ASSET_LOCK_TRADE_ERR, 0, 0);
			PROCEDURE_RETURN
		}
		if (IsHawking())
		{
			CB_AssetProtectResult(GetPeer(), context, ASSET_LOCK_HAWK_ERR, 0, 0);
			PROCEDURE_RETURN
		}

		if (password.GetSize() != 32)
		{
			CB_AssetProtectResult(GetPeer(), context, ASSET_LOCK_PASS_ERR, 0, 0);
			PROCEDURE_RETURN
		}

        // to check if there are any existing asset password, or if player is required to reset his password 
		ObjectService_Proxy::IsAssetProtected(gGlobal.pObjectService->pPeer, context, GetAcct_id());

	PROCEDURE_WAIT2(1, ObjectService, CB_IsAssetProtected, Boolean, bProtect, Boolean, bNeedResetPassword)

		if (!bProtect && bNeedResetPassword) // fail to retrieve data from db
		{
			PROCEDURE_GO_STEP1(2, Boolean, BOOLEAN_FALSE)	
		}	
		if (!bChangePassword)// if player wants to set a password, supposedly there is no existing old password or need to reset the password
		{
			if (bProtect && !bNeedResetPassword)
			{// now double checked by objectservice, but the result is that there is existing old password in db and there is no need to reset it
				PROCEDURE_GO_STEP1(2, Boolean, BOOLEAN_FALSE) // then don't store the new password 	
			}
		}
		// go to store the password in db
		ObjectService_Proxy::StoreAssetPassword(gGlobal.pObjectService->pPeer, context, password, GetAcct_id());

	PROCEDURE_WAIT1(2, ObjectService, CB_StoreAssetPassword, Boolean, bSuccess)

		if (bSuccess)
			SetUnlock_time(0);	
		CB_SetAssetPassword(GetPeer(), context, bSuccess);

	PROCEDURE_CATCH
	
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
		
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("SetAssetPassword Error: ObjectService:CB_Error, errcode %d, %s"), 
					err, errMsg.c_str());
			
		PROCEDURE_EXCEPTION_DEFAULT

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context)
{
	PROCEDURE_START_RPC(Character, ForceResetAssetPassword, this)

		if (GetScriptSystem() && GetScriptSystem()->IsRunning())
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("ForceResetAssetPassword Error: Scripting is running"));	
			PROCEDURE_RETURN
		}	
		// to check if there are any existing asset password, or if player is required to reset his password 
		ObjectService_Proxy::IsAssetProtected(gGlobal.pObjectService->pPeer, context, GetAcct_id());

	PROCEDURE_WAIT2(1, ObjectService, CB_IsAssetProtected, Boolean, bProtect, Boolean, bNeedResetPassword)

		if (!bProtect) // if there is no existing password in db
		{	// no need to set the password reset date
			PROCEDURE_GO_STEP1(2, Boolean, BOOLEAN_FALSE)
		}

		UInt32 resetDate = (UInt32) time(NULL) + gConfigure.resetAssetPassTime;
		ObjectService_Proxy::SetAssetPassResetDate(gGlobal.pObjectService->pPeer, context, GetAcct_id(), resetDate);// set the reset date in db	

	PROCEDURE_WAIT1(2, ObjectService, CB_ResetAssetPassDate, Boolean, bSuccess)

        if (bSuccess)
		{
			UInt32 date = (UInt32) time(NULL) + gConfigure.resetAssetPassTime;
			SetUnlock_time(date);
		}

		CB_ForceResetAssetPassword(pPeer, context, bSuccess);
		
	PROCEDURE_CATCH
	
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
		
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("ForceResetAssetPassword Error: ObjectService:CB_Error, errcode %d, %s"), 
					err, errMsg.c_str());
			
		PROCEDURE_EXCEPTION_DEFAULT

	PROCEDURE_END_CATCH

    return RPC_RESULT_OK;	
}


RPCResult CCharacter::UnlockAsset(LPCPEER pPeer, RPCContext &context, const ByteArray &password, const UInt32 &bagIndex, const UInt8 &assetType)
{
	PROCEDURE_START_RPC3(Character, UnlockAsset, this, ByteArray, password, UInt32, bagIndex, UInt8, assetType)
		
		// asset protection operation cannot be preformed under the following condition

		if (GetScriptSystem() && GetScriptSystem()->IsRunning())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_SCRIPT_ERR, bagIndex, assetType);
			PROCEDURE_RETURN
		}
		if (!IsInMap())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_CHAR_ERR, bagIndex, assetType);
			PROCEDURE_RETURN
		}
		if (IsInBattle())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_BATTLE_ERR, bagIndex, assetType);
			PROCEDURE_RETURN
		}
		if (IsTrading())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_TRADE_ERR, bagIndex, assetType);
			PROCEDURE_RETURN
		}
		if (IsHawking())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_HAWK_ERR, bagIndex, assetType);
			PROCEDURE_RETURN
		}
		if (password.GetSize() != 32)
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_PASS_ERR, 0, 0);
			PROCEDURE_RETURN
		}
		// to check if there are any existing asset password, or if player is required to reset his password 
		ObjectService_Proxy::IsAssetProtected(gGlobal.pObjectService->pPeer, context, GetAcct_id());

	PROCEDURE_WAIT2(1, ObjectService, CB_IsAssetProtected, Boolean, bProtect, Boolean, bNeedResetPassword)

		if (!bProtect && bNeedResetPassword) // fail to retrieve data from db
		{
			PROCEDURE_RETURN	
		}
		if ((bNeedResetPassword) || (!bProtect))
		{// if it is reqired to reset the password or there is no existing password in db
			UInt8 result = bNeedResetPassword? ASSET_RESET_PASS : ASSET_INIT_PASS;
            CB_AssetProtectResult(pPeer, context, result, bagIndex, assetType);
			PROCEDURE_RETURN // no need to unlock asset. Instead, ask player to set new password
		}
		if (bProtect) // if there is existing old password in db, get it via objectservice. 
			ObjectService_Proxy::GetAssetPassword(gGlobal.pObjectService->pPeer, context, GetAcct_id());
		else
		{
			PROCEDURE_RETURN
		}

	PROCEDURE_WAIT1(2, ObjectService, CB_GetAssetPassword, ByteArray, correctPassword)
		// if the player has entered the right password, or the player has already entered the right password before so no need to enter
		// the password again (password verified)
		UInt8 lockMsg;	
		if (((password == correctPassword) || m_bAssetPasswordVerified) && (correctPassword.GetSize() == 32)) 
		{
			
			switch(assetType)
			{
			case ASSET_ITEM:
				{
					// set item to unlock
					if (m_pBag)
						m_bAssetPasswordVerified = (m_pBag->ChangeItemLockStatus(bagIndex, BOOLEAN_FALSE, lockMsg) || m_bAssetPasswordVerified);
					else
					{
						TRACE_ERRORDTL(GLOBAL_LOGGER, _F("UnLockItem Error: no itembag with corresponding character"));	
						PROCEDURE_RETURN
					}	
				}
				break;
			case ASSET_PARTNER:
				{
					// set partner to unlock
					if (m_pPartnerGroup)
						m_bAssetPasswordVerified = (m_pPartnerGroup->ChangePartnerLockStatus(bagIndex, BOOLEAN_FALSE, lockMsg)|| 
						m_bAssetPasswordVerified);
					else
					{
						TRACE_ERRORDTL(GLOBAL_LOGGER, _F("UnLockPartner Error: no PartnerGroup with corresponding character"));	
						PROCEDURE_RETURN	
					}
				}
				break;
			case ASSET_PET:
				{
					//set pet to unlock status
					if (m_pPetGroup)
						m_bAssetPasswordVerified = (m_pPetGroup->ChangePetLockStatus(bagIndex, BOOLEAN_FALSE, lockMsg) ||
						m_bAssetPasswordVerified);
					else
					{
						TRACE_ERRORDTL(GLOBAL_LOGGER, _F("UnLockPet Error: no PetGroup with corresponding character"));	
						PROCEDURE_RETURN	
					}
				}
				break;
			case ASSET_BANKMONEY:
				{
					//to set character's "banklock" property to unlock status
                    m_bAssetPasswordVerified = (ChangeBankMoneyStatus(BOOLEAN_FALSE, lockMsg) || m_bAssetPasswordVerified);
				}
				break;
			case ASSET_YUANBAO:
				{
					//to set accountdata's "yuanBaolock" property to unlock status 
					m_bAssetPasswordVerified = (ChangeYuanBaoLockStatus(context, BOOLEAN_FALSE, lockMsg) || m_bAssetPasswordVerified);
				}
				break;
			default:
				{
					PROCEDURE_RETURN
				}
			}

 		}
		else
		{
			// cb to client that the password is invalid
			UInt8 msg;	
			if (!m_bAssetPasswordVerified) //if the password is not verified before 
				msg = ASSET_PASSWORD_UNVERIFIED;
			else
                msg = ASSET_PASSWORD_INVALID;	
			CB_AssetReleaseLockResult(pPeer, context, msg, bagIndex, assetType);	
			PROCEDURE_RETURN
		}
		CB_AssetReleaseLockResult(pPeer, context, lockMsg, bagIndex, assetType);
		if (GetUnlock_time() != 0) // if reset password date is set, then cancel it because player has entered the correct password to unlock asset
		{
			ObjectService_Proxy::SetAssetPassResetDate(gGlobal.pObjectService->pPeer, context, GetAcct_id(), 0);
			SetUnlock_time(0);	
		}

	PROCEDURE_CATCH
	
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
		
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("SetAssetPassword Error: ObjectService:CB_Error, errcode %d, %s"), 
					err, errMsg.c_str());
			
		PROCEDURE_EXCEPTION_DEFAULT

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::ShutNewGuide(LPCPEER pPeer, RPCContext &context, const UInt8 &flag)
{
	GetQuests()->SetFlag(80010, flag);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::LockAsset(LPCPEER pPeer, RPCContext &context, const UInt32 &bagIndex, const UInt8 &assetType)
{
	PROCEDURE_START_RPC2(Character, LockAsset, this, UInt32, bagIndex, UInt8, assetType)

		// asset protection operation cannot be preformed under the following condition	
		if (GetScriptSystem() && GetScriptSystem()->IsRunning())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_SCRIPT_ERR, bagIndex, assetType);
			PROCEDURE_RETURN
		}
		if (!IsInMap())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_CHAR_ERR, bagIndex, assetType);
			PROCEDURE_RETURN
		}
		if (IsInBattle())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_BATTLE_ERR, bagIndex, assetType);
			PROCEDURE_RETURN
		}
		if (IsTrading())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_TRADE_ERR, bagIndex, assetType);
			PROCEDURE_RETURN
		}
		if (IsHawking())
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_HAWK_ERR, bagIndex, assetType);
			PROCEDURE_RETURN
		}
		// to check if there are any existing asset password, or if player is required to reset his password 
		ObjectService_Proxy::IsAssetProtected(gGlobal.pObjectService->pPeer, context, GetAcct_id());

	PROCEDURE_WAIT2(1, ObjectService, CB_IsAssetProtected, Boolean, bProtect, Boolean, bNeedResetPassword)

		if (!bProtect && bNeedResetPassword) // fail to retrieve the data from db
		{
			CB_AssetProtectResult(pPeer, context, ASSET_LOCK_FAIL, bagIndex, assetType);
			PROCEDURE_RETURN	
		}	
		if ((bNeedResetPassword) || (!bProtect))
		{// if it is reqired to reset the password or there is no existing password in db
			UInt8 result = bNeedResetPassword? ASSET_RESET_PASS : ASSET_INIT_PASS;
            CB_AssetProtectResult(pPeer, context, result, bagIndex, assetType);
			PROCEDURE_RETURN
		}

		UInt8 lockMsg;	
		switch(assetType)
		{
		case ASSET_ITEM:
			{
				// to lock the item
				if (m_pBag)
                    m_pBag->ChangeItemLockStatus(bagIndex, BOOLEAN_TRUE, lockMsg);
                else
				{
					TRACE_ERRORDTL(GLOBAL_LOGGER, _F("LockItem Error: no itembag with corresponding character"));	
					PROCEDURE_RETURN	
				}	
			}
			break;
		case ASSET_PARTNER:
			{
				// to lock partner
				if (m_pPartnerGroup)
					m_pPartnerGroup->ChangePartnerLockStatus(bagIndex, BOOLEAN_TRUE, lockMsg);
				else
				{
					TRACE_ERRORDTL(GLOBAL_LOGGER, _F("LockPartner Error: no PartnerGroup with corresponding character"));	
					PROCEDURE_RETURN
				}
			}
			break;
		case ASSET_PET:
			{
				// to lock pet
				if (m_pPetGroup)
					m_pPetGroup->ChangePetLockStatus(bagIndex, BOOLEAN_TRUE, lockMsg);
				else
				{
					TRACE_ERRORDTL(GLOBAL_LOGGER, _F("LockPet Error: no PetGroup with corresponding character"));	
					PROCEDURE_RETURN	
				}
			}
			break;
		case ASSET_BANKMONEY:
			{
				// to lock bankmoney
				ChangeBankMoneyStatus(BOOLEAN_TRUE, lockMsg);
			}
			break;
		case ASSET_YUANBAO:
			{
				// to lock yuanbao 
				ChangeYuanBaoLockStatus(context, BOOLEAN_TRUE, lockMsg);
			}
			break;
		default:
			{
				PROCEDURE_RETURN
			}
		}

		CB_AssetProtectResult(pPeer, context, lockMsg, bagIndex, assetType);	

	PROCEDURE_CATCH
	
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
		
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("RestoreCharacter Error: ObjectService:CB_Error, errcode %d, %s"), 
					err, errMsg.c_str());
			
		PROCEDURE_EXCEPTION_DEFAULT

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;

}

Boolean CCharacter::ChangeYuanBaoLockStatus(RPCContext &context, const Boolean &lock, UInt8 &msg)
{
	if (GetYuanBao() == 0)
	{
		msg = ASSET_INVALID_ERR;
		return BOOLEAN_FALSE;
	}
	if (GetYuanbaoLock() == lock)
	{
		if (lock)
			msg = ASSET_LOCK_ALREADY;	
		else
			msg = ASSET_UNLOCK_ALREADY;	
			
		return BOOLEAN_FALSE;
	}
	SetYuanbaoLock(lock);
	msg = lock? ASSET_LOCK_SUCCESS : ASSET_UNLOCK_SUCCESS;	
	ObjectService_Proxy::SetYuanBaoLock(gGlobal.pObjectService->pPeer, context, GetAcct_id(), lock);

	return BOOLEAN_TRUE;
}

Boolean CCharacter::ChangeBankMoneyStatus(const Boolean &lock, UInt8 &msg)
{
	if (GetBankMoney() == 0)
	{
		msg = ASSET_INVALID_ERR;
		return BOOLEAN_FALSE;
	}
	if (GetBankLock() == lock)
	{
		if (lock)
			msg = ASSET_LOCK_ALREADY;	
		else
			msg = ASSET_UNLOCK_ALREADY;	
			
		return BOOLEAN_FALSE;	
	}

	SetBankLock(lock);
	msg = lock? ASSET_LOCK_SUCCESS : ASSET_UNLOCK_SUCCESS;	
	return BOOLEAN_TRUE;
}

UInt8 CCharacter::GetGuildPos()
{
	if (m_ownGuildMember) 
		return m_ownGuildMember->GetPost(); 
	return 0;
}

UInt32 CCharacter::GetJoinGuildPeriod()
{
	if (m_ownGuildMember) 
	{
		time_t day = m_ownGuildMember->GetJoinDate();
		time_t now = time(0);
		return ((UInt32) (now - day));
	}
	return 0;
}

bool CCharacter::IsResting()
{
	return m_battleRestTime > ::timeGetTime();
}

bool CCharacter::IsGuildWarWin()
{
	if (m_pMap && m_pMap->GetMapInfo() && m_pMap->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR)
	{
		if (GetGuildUid() != 0 &&
			GetGuildUid() == m_pMap->GetGuildWarWinID())
			return true;
	}

	return false;
}

void CCharacter::ChangeGuildWarTitle()
{
	for (TitleMap::iterator Itr = m_titleDataLst.begin();
			Itr != m_titleDataLst.end(); ++Itr)
		{
			TitleInfo* pTitle = Itr->second;
			if (pTitle)
			{
				const TitleData * pData = gGlobal.m_resource->GetTitleData(pTitle->title_id);
				if (pData && pData->Title_Type == TITLE_TYPE_GUILD)
				{
					SetTitle(pTitle->title_name, pTitle->title_id);
					break;
				}
			}
		}
}

VOID CCharacter::AddSkillTemp(UInt16 skillId)
{
	SkillMap::iterator Itr = m_skillMap.find(skillId);
	if (Itr == m_skillMap.end())
	{
		UInt16Vector skill_ids;
		gGlobal.m_resource->GetSkillLink(skillId, skill_ids);

		NetGroup grp;
		bool calAttri = false;
		for (size_t i=0; i<skill_ids.size(); i++)
		{
			const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(skill_ids[i]);
			if (pSkillData)
			{
				if (pSkillData && gGlobal.m_resource->GetSkillData(pSkillData->id))
				{
					CSkill * pSkill = CSkill::NewInstance();
					pSkill->SetInfo(pSkillData->id);
					pSkill->m_data = gGlobal.m_resource->GetSkillData(pSkillData->id);
					if (pSkill->m_data == NULL)
						TRACE_WARNDTL_1(GLOBAL_LOGGER, 
						_T("CSkill Warning: cannot find skill_id %d in resource"), pSkillData->id);
					pSkill->SetOwner_info(GetChar_id(),pSkillData->type);
					pSkill->SetLevel(pSkillData->reqLv);
					pSkill->SetCoolDown(pSkillData->skill_cooldownType);
					pSkill->ClearAllMask();
					pSkill->PreSend();
					m_pSkillGroup->Add(pSkill);
					grp.Add(pSkill);
					m_skillMap.insert(std::make_pair(pSkill->GetSkill_id(), pSkill));
				}
			}
		}

		if (m_pPeer)
			m_pSkillGroup->CB_AddSkills(m_pPeer, m_context, &grp);

	}
	else
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CCharacter AddSkillTemp Warning : Skill Exists"));
	}
}

VOID CCharacter::RemoveSkillTemp(UInt16 skillId)
{
	SkillMap::iterator Itr = m_skillMap.find(skillId);
	if (Itr != m_skillMap.end())
	{
		UInt16Vector skill_ids;
		gGlobal.m_resource->GetSkillLink(skillId, skill_ids);

		for (Int x = 0; x < (Int)skill_ids.size(); x++)
		{
			for (UInt16 i = 0; i < m_pSkillGroup->GetCount(); ++i)
			{
				CSkill * pChild = m_pSkillGroup->GetChild<CSkill>(i);

				if (pChild && pChild->GetSkill_id() == skill_ids[x])
				{
					m_pSkillGroup->Remove(pChild);
					DeleteNetObject(pChild);
					break;
				}
			}

			SkillMap::iterator Itr = m_skillMap.find(skill_ids[x]);
			if (Itr != m_skillMap.end())
			{
				m_skillMap.erase(Itr);
			}
		}
		if (m_pPeer)
			m_pSkillGroup->CB_RemoveSkills(m_pPeer, m_context, skill_ids);

	}
	else
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CCharacter RemoveSkillTemp Warning : Skill not Exists"));
	}
}

RPCResult CCharacter::KickMember(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &reason)
{
	//return false if in guild war map
	if (InGuildWar())
	{
		return CB_Error(m_pPeer, m_context, 0, _T("MSG_GUILDWAR_KICK_FAIL"));
	}

	CCharacter* pchar = gGlobal.m_charToSession.FindCharacter(char_id);
	if (pchar && pchar->InGuildWar())
	{
		return CB_Error(m_pPeer, m_context, 0, _T("MSG_GUILDWAR_KICK_FAIL"));
	}

	if (m_ownGuildMember)
		MessageService_Proxy::KickMember(gGlobal.pMessageService->pPeer, m_context, 
			m_ownGuildMember->GetGuild_uid(), GetChar_id(), char_id, reason);

	return RPC_RESULT_OK;
}

UInt16 CCharacter::GetSkillCount(UInt16 type,UInt16 level)
{
	UInt16 nSkillCount = 0;
	for (UInt8 i = 0; i < m_pSkillGroup->GetCount(); ++i)
	{
		CSkill* pSkill = m_pSkillGroup->GetChild<CSkill>(i);
		if (pSkill && pSkill->GetSkill_level() >= level && pSkill->m_data && pSkill->m_data->type == type)
		{
			nSkillCount++;
		}
	}
	return nSkillCount;
}

RPCResult CCharacter::SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context,const Boolean& hit)
{
	m_hitBoss = hit;
	CB_SetAutoWalkHitBoss(m_pPeer, m_context,hit);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::SetAutoWalk(LPCPEER pPeer, RPCContext &context,const Boolean& bRobot)
{
	UInt32 interval = (0 ? (_ttoi(0) * 1000) : 
GetPrivateProfileInt(_T("Robot"), _T("Interval"), 
					 ROBOT_INTERVAL, CONFIGURATION_INI_FILE));
SetRobot(bRobot,interval);
return RPC_RESULT_OK;
}

void CCharacter::CreatePartnerCollection(UInt32 collection_id, UInt32 char_id)
{
	PROCEDURE_DO_METHOD2(CCharacter, CreatePartnerCollection, this, GetContext(), UInt32, collection_id, UInt32, char_id)
}

RPCResult CCharacter::CreatePartnerCollection(RPCContext &context)
{
	PROCEDURE_START_METHOD2(CCharacter, CreatePartnerCollection, this, context, UInt32, collection_id, UInt32, char_id)
		ObjectService_Proxy::CreatePartnerCollection(gGlobal.pObjectService->pPeer, context, collection_id, char_id);
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CCharacter::CreatePartnerCollection sessionId %s, collection_id %d, char_id %d"),
		context.sessionId.GetString().c_str(), collection_id, char_id);
	PROCEDURE_WAIT1(1, ObjectService, CB_CreatePartnerCollection, NetObject*, partnerCollection)

		CPartnerCollectionData * pPartnerCollectionData = CastNetObject<CPartnerCollectionData>(partnerCollection);

	if (pPartnerCollectionData)
	{
		partnerCollectionData * pMapPartnerCollectionData = SafeCreateObject(CCharacter::partnerCollectionData);
		pMapPartnerCollectionData->collection_uid = pPartnerCollectionData->GetCollection_uid();
		pMapPartnerCollectionData->collection_id = pPartnerCollectionData->GetCollection_id();
		pMapPartnerCollectionData->char_id = pPartnerCollectionData->GetChar_id();

		m_PartnerCollectionLst.insert(std::make_pair(pMapPartnerCollectionData->collection_id, pMapPartnerCollectionData));
		ResetPartnerCollectionBuff();
		//update the client partner collection
		UInt32Vector PartnerCollectionIDs;
		CCharacter::PartnerCollectionMap::iterator Iter = m_PartnerCollectionLst.begin();
		while (Iter != m_PartnerCollectionLst.end())
		{
			PartnerCollectionIDs.push_back(Iter->second->collection_id);
			Iter++;
		}
		if (PartnerCollectionIDs.size() > 0)
		{
			CB_GetPartnerCollectionList(m_pPeer, m_context,PartnerCollectionIDs);
		}
	}
	DeleteNetObject(partnerCollection);

	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("AddPet Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());
	PROCEDURE_END_CATCH

		return RPC_RESULT_OK;
}

void CCharacter::DeletePartnerCollection(UInt32 char_id)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CCharacter::DeletePartnerCollection char_id %d, sessionId %s"),char_id,m_context.sessionId.GetString().c_str());
	PartnerCollectionMap::iterator iter = m_PartnerCollectionLst.begin();
	while (iter != m_PartnerCollectionLst.end())
	{
		ObjectService_Proxy::DeletePartnerCollection(gGlobal.pObjectService->pPeer, m_context, iter->second->collection_uid);
		SafeDeleteObject(iter->second);
		iter++;
	}
	m_PartnerCollectionLst.clear();
}

void CCharacter::GetPartnerCollection(UInt32 char_id)
{
	PartnerCollectionMap::iterator iter = m_PartnerCollectionLst.begin();
	while (iter != m_PartnerCollectionLst.end())
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER,_F("(%d,%d,%d)"),
			iter->second->collection_uid,iter->second->collection_id,iter->second->char_id);
		iter++;
	}
}

void CCharacter::SetTitle(const String &title, const UInt16 &title_id, bool bRemove)
{
	if (GetTitle_id() == title_id && GetTitle() == title)
		return;

	if(!bRemove)
	{
		const TitleData * titleDataCSV = gGlobal.m_resource->GetTitleData(GetTitle_id());
		if (titleDataCSV != NULL)
		{
			if (titleDataCSV->Title_Type == TITLE_TYPE_FORCE_TOP)
			{
				CB_Error(GetPeer(), GetContext(), 0, _T("MSG_TITLE_CANNOT_CHANGE"));
				return;
			}
		}
	}
	Parent::SetTitle(title, title_id);
	MessageService_Proxy::ChangeTitle(gGlobal.pMessageService->pPeer, m_context, GetChar_id(), title);
}

void CCharacter::SetTitle(const String &title)
{
	if (GetTitle() != title)
	{
		Parent::SetTitle(title);
		MessageService_Proxy::ChangeTitle(gGlobal.pMessageService->pPeer, m_context, 
			GetChar_id(), title);
	}
}


UInt16 CCharacter::GetPartnerCollectionBuff(UInt8 race, AttriStruct2 &attri)
{
	UInt16 Count(0);
	const PartnerCollectionData * pPartnerCollectionData;
	ZeroMemory(&attri, sizeof(AttriStruct2));
	PartnerCollectionMap::iterator iter = m_PartnerCollectionLst.begin();
	while (iter != m_PartnerCollectionLst.end())
	{
		pPartnerCollectionData = gGlobal.m_resource->GetPartnerCollectionData(iter->second->collection_id);
		if (pPartnerCollectionData && race == pPartnerCollectionData->Type)
		{
			Count++;
			attri.SetAgi(attri.GetAgi() + pPartnerCollectionData->AGI);
			attri.SetCon(attri.GetCon() + pPartnerCollectionData->CON);
			attri.SetSpr(attri.GetSpr() + pPartnerCollectionData->SPR);
			attri.SetSta(attri.GetSta() + pPartnerCollectionData->STA);
			attri.SetStr(attri.GetStr() + pPartnerCollectionData->STR);
		}
		++iter;
	}
	return Count;
}

UInt16 CCharacter::GetPartnerCollectionCount(UInt8 race)
{
	UInt16 Count(0);
	const PartnerCollectionData * pPartnerCollectionData;
	PartnerCollectionMap::iterator iter = m_PartnerCollectionLst.begin();
	while (iter != m_PartnerCollectionLst.end())
	{
		pPartnerCollectionData = gGlobal.m_resource->GetPartnerCollectionData(iter->second->collection_id);
		if (pPartnerCollectionData && race == pPartnerCollectionData->Type)
		{
			Count++;
		}
		++iter;
	}
	return Count;
}

void CCharacter::ResetPartnerCollectionBuff()
{
	ActivatePartnerCollectionBuff(m_bUsePartnerCollectionBuff);
}

void CCharacter::SendPartnerCollection()
{
	UInt32Vector collectionIds;
	for (PartnerCollectionMap::iterator it = m_PartnerCollectionLst.begin();
		it != m_PartnerCollectionLst.end(); ++it)
	{
		collectionIds.push_back(it->second->collection_id);
	}
	if (collectionIds.size() > 0)
		CB_GetPartnerCollectionList(m_pPeer, m_context, collectionIds);
}

void CCharacter::ActivatePartnerCollectionBuff(bool bActivate)
{
	CPartnerGroup* pPartners = this->GetPartners();
	bool bUpdate = false;
	if (pPartners)
	{
		for (UInt8 i=0; i<pPartners->GetCount(true); i++)
		{
			CPartner * pPartner = pPartners->GetPartnerByIndex(i);
			if (pPartner)
			{
				if (bActivate)
				{
					UInt16 Count = GetPartnerCollectionBuff(pPartner->GetRace(), pPartner->m_AttriOfPartnerCollection);
					if (Count > 0)
					{
						UInt16 BuffID = gGlobal.m_resource->GetPartnerCollectionBuffID(Count, pPartner->GetRace());
						pPartner->RemoveBuffSchool(PARTNER_COLLECTION_BUFF_SCHOOL_ID);
						if (BuffID > 0)
						{
							pPartner->AddBuff(BuffID, 0, 0, 0, 99999, 0);
							pPartner->CalcAttrib();
							bUpdate = true;
						}
					}
					m_bUsePartnerCollectionBuff = true;
				} 
				else if (pPartner->HasBuffSchool(PARTNER_COLLECTION_BUFF_SCHOOL_ID))
				{
					ZeroMemory(&pPartner->m_AttriOfPartnerCollection, sizeof(AttriStruct2));
					pPartner->RemoveBuffSchool(PARTNER_COLLECTION_BUFF_SCHOOL_ID);
					pPartner->CalcAttrib();
					bUpdate = true;
					m_bUsePartnerCollectionBuff = false;
				}
			}
		}
		if (bUpdate)
			pPartners->RaiseUpdate();
	}
}

RPCResult CCharacter::UsePartnerCollectionBuff(LPCPEER pPeer, RPCContext &context, const bool &bUse)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("UsePartnerCollectionBuff bUse %d, sessionId %s, char_id %d"),
		bUse, context.sessionId.GetString().c_str(), GetChar_id());
	BYTE returnFlag(PARTNER_COLLECTION_ENABLE_OR_DISABLE_FAIL);
	if (m_pPartnerGroup && m_pPartnerGroup->GetCount() > 0)
	{
		ActivatePartnerCollectionBuff(bUse);
		if (bUse)
		{
			returnFlag = PARTNER_COLLECTION_ENABLE_SUCCESS;
		}
		else
		{
			returnFlag = PARTNER_COLLECTION_DISABLE_SUCCESS;
		}
	}
	CB_UsePartnerCollectionBuff(m_pPeer, m_context, returnFlag);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("AddPartnerCollection partner_id %d, sessionId %s, char_id %d"),
		partner_id, context.sessionId.GetString().c_str(), GetChar_id());
	UInt32 mob_id(0);
	BYTE returnFlag(PARTNER_COLLECTION_ADD_SUCCESS);
	TRACE_ENSURE(m_pPartnerGroup);
	CPartner * partner = m_pPartnerGroup->GetPartner(partner_id);
	if (!partner)
	{
		returnFlag = PARTNER_COLLECTION_ADD_NOT_EXIST;//player don't have this mob
		CB_AddPartnerCollection(m_pPeer, m_context, mob_id, returnFlag);
		return RPC_RESULT_OK;
	}
	mob_id = partner->GetMob_id();
	if (partner->GetReqLv() > GetLevel())
	{
		returnFlag = PARTNER_COLLECTION_ADD_LEVEL_REQUIRE;//player's level must >= require level
		CB_AddPartnerCollection(m_pPeer, m_context, mob_id, returnFlag);
		return RPC_RESULT_OK;
	}
	if (partner->GetLock())
	{
		returnFlag = PARTNER_COLLECTION_ADD_LOCKED;//the partner can't be locked
		CB_AddPartnerCollection(m_pPeer, m_context, mob_id, returnFlag);
		return RPC_RESULT_OK;
	}
	if (IsHawking())
	{
		CB_Error(m_pPeer, m_context, 1, _T("MSG_HAWK_HAWKING_CANT_DO_ANYTHING"));
		return RPC_RESULT_OK;
	}
	if (IsTrading())
	{
		CB_Error(m_pPeer, m_context, 1, _T("MSG_TRADING_GIVEN_CANT_DROP_PARTNER"));
		return RPC_RESULT_OK;
	}
	const PartnerCollectionData * PartnerCollectionDataCSV = gGlobal.m_resource->GetPartnerCollectionData(mob_id);
	if (!PartnerCollectionDataCSV)
	{
		returnFlag = PARTNER_COLLECTION_ADD_USELESS;//PartnerCollection don't need this mob
		CB_AddPartnerCollection(m_pPeer, m_context, mob_id, returnFlag);
		return RPC_RESULT_OK;
	}
	CCharacter::PartnerCollectionMap::iterator Iter = m_PartnerCollectionLst.begin();
	while (Iter != m_PartnerCollectionLst.end())
	{
		if (mob_id == Iter->second->collection_id)
		{
			returnFlag = PARTNER_COLLECTION_ADD_ALREADY_EXIST_IN_COLLECTION;//this mob all already exist in PartnerCollection
			CB_AddPartnerCollection(m_pPeer, m_context, mob_id, returnFlag);
			return RPC_RESULT_OK;
		}
		Iter++;
	}

	// remove this partner
	if (!IsInMap()) {
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("DeletePartner Warning: [owner not in map] char_id %d, sessionId %s, partner_id %d"), 
			GetChar_id(), context.sessionId.GetString().c_str(), partner_id);
		return RPC_RESULT_OK;
	}
	m_pPartnerGroup->RemovePartner(partner);

	// add to PartnerCollection
	CreatePartnerCollection(mob_id,GetChar_id());
	CB_AddPartnerCollection(m_pPeer, m_context,mob_id, returnFlag);

	return RPC_RESULT_OK;
}

Boolean CCharacter::AddPartnerCollection(UInt32 mob_id)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("mob_id %d,  char_id %d"), mob_id, GetChar_id());
	TRACE_ENSURE(m_pPartnerGroup);
	if (IsHawking())
	{
		return BOOLEAN_FALSE;
	}
	if (IsTrading())
	{
		return BOOLEAN_FALSE;
	}
	const PartnerCollectionData * PartnerCollectionDataCSV = gGlobal.m_resource->GetPartnerCollectionData(mob_id);
	if (!PartnerCollectionDataCSV)
	{
		return BOOLEAN_FALSE;
	}
	CCharacter::PartnerCollectionMap::iterator Iter = m_PartnerCollectionLst.begin();
	while (Iter != m_PartnerCollectionLst.end())
	{
		if (mob_id == Iter->second->collection_id)
		{
			return BOOLEAN_FALSE;
		}
		Iter++;
	}
	if (!IsInMap())
	{
		return BOOLEAN_FALSE;
	}

	CreatePartnerCollection(mob_id,GetChar_id());
	return BOOLEAN_TRUE;
}

RPCResult CCharacter::ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &changeType, const UInt32 &num, const ByteArray &password)
{
	PROCEDURE_START_RPC3(Character, ChangeBetweenYBandYBT, this, UInt8, changeType, UInt32, num, ByteArray, password)

		PROCEDURE_WAIT_TIMEOUT(1, 2000) //delay 2 second	
		
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("session_id %s, char_id %d, changeType %d, change %d "), 
			context.sessionId.GetString().c_str(), GetChar_id(), changeType, num);

		UInt msgIndex = CHANGE_YB_SUCCESS;
		do
		{
			if (!IsInMap())
			{
				msgIndex = CHANGE_YB_NOT_IN_MAP;
				break;
			}

			if (GetScriptSystem()->IsRunning())
			{
				msgIndex = CHANGE_YB_SCRIPT_RUNNING;
				break;
			}

			if (IsTrading())
			{
				msgIndex = CHANGE_YB_IN_TRADING;
				break;
			}

			if (IsHawking())
			{
				msgIndex = CHANGE_YB_IN_HAWKING;
				break;
			}
			if (GetYuanbaoLock())
			{
				msgIndex = CHANGE_YB_YUANBAOLOCK;
				break;
			}
			if (num == 0)
			{
				msgIndex = CHANGE_YB_FAIL;
				break;
			}
		}
		while(false);
		
		Int32 change = 0;
		switch (changeType)
		{
		case CHANGEYBT_YB_TO_YBT:
			change = num;
			break;
		case CHANGEYBT_YBT_TO_YB:
			change = -(Int)num;
			break;
		}

		if (change != 0 && msgIndex == CHANGE_YB_SUCCESS)
		{
			ObjectService_Proxy::UpdateYuanBaoTicket(gGlobal.pObjectService->pPeer, context, GetAcct_id(), change, password);
		}
		else
		{
			PROCEDURE_GO_STEP3(2, UInt32, 0, UInt32, 0, UInt8, msgIndex)
		}
		
	PROCEDURE_WAIT3(2, ObjectService, CB_UpdateYuanBaoTicket, UInt32, yuanbao, UInt32, yuanbaoTicket, UInt8, msgIndex)
		
		if (msgIndex == CHANGE_YB_SUCCESS)
		{
			switch (changeType)
			{
			case CHANGEYBT_YB_TO_YBT:
				LogService_Proxy::LogSellYuanBao(gGlobal.pLogService->pPeer, context, GetAcct_id(), 
					GetChar_id(), LogService_Proxy::SellYuanBao::CHANGE_YB_TO_YBT, 0, GetYuanBao() - yuanbao, num, 0, 0, 0, 0);
				break;
			case CHANGEYBT_YBT_TO_YB:
				LogService_Proxy::LogSellYuanBao(gGlobal.pLogService->pPeer, context, GetAcct_id(), 
					GetChar_id(), LogService_Proxy::SellYuanBao::CHANGE_YBT_TO_YB, 0, yuanbao - GetYuanBao(), num, 0, 0, 0, 0);
				break;
			}
			
			SetYuanBao(yuanbao,yuanbaoTicket);			
		}

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("session_id %s, char_id %d, success %d "), 
			context.sessionId.GetString().c_str(), GetChar_id(), (msgIndex == CHANGE_YB_SUCCESS)? 1 : 0 );

		CB_ChangeBetweenYBandYBT(pPeer, context, msgIndex);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());			

			CB_ChangeBetweenYBandYBT(pPeer, context, CHANGE_YB_FAIL);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::WaitAvailable(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CCharacter, WaitAvailable, this, context, SessionID, targetSid)

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("session_id %s, char_id %d, target_session_id %s"), 
			context.sessionId.GetString().c_str(), GetChar_id(), targetSid.GetString().c_str());

		//this function is called by other character for make sure 
		//that this character will finish the init before do somthing
		
	PROCEDURE_END

	return RPC_RESULT_OK;
}
RPCResult CCharacter::RequestTargetEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CCharacter::RequestTargetEquip, TargetChar_id = %d, sessionId %s, char_id %d"),
		char_id, context.sessionId.GetString().c_str(), GetChar_id());

	if (char_id == GetChar_id())
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F(" do not request self equip."));
		return RPC_RESULT_OK;
	}

	CCharacter* pchar = gGlobal.m_charToSession.FindCharacter(char_id);
	if (pchar)
	{
		if (pchar->GetEnableShowEquip())
		{
			NetGroup itemGroup;
			CItemBag * pItemBag = pchar->GetItems();
			if(pItemBag)
			{
				for (UInt8 i = 0; i < BAG_EQUIP_COUNT; ++i)
				{
					CItem * item = pItemBag->GetItem(i + BAG_MAX_ITEM);
					if(item)
					{
						item->SetSendMask(CItem::AllMask);
						itemGroup.Add(item);
					}
				}
			}

			if(itemGroup.GetCount() > 0)
			{
				CB_RequestTargetEquip(m_pPeer, m_context, &itemGroup);
			}
		}
		else
		{
			CB_Error(m_pPeer, m_context, 1, _T("MSG_TARGET_EQUIP_REQUEST_DISABLE"));
		}
	}
	else
	{
		CB_Error(m_pPeer, m_context, 1, _T("MSG_TARGET_EQUIP_REQUEST_LINE_FAIL"));
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" target offline, sessionId %s, char_id %d"), 
			context.sessionId.GetString().c_str(), GetChar_id());
	}
	return RPC_RESULT_OK;
}



RPCResult CCharacter::GetPlayerEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CCharacter::GetPlayerEquip, TargetChar_id = %d, sessionId %s, char_id %d"), 
		char_id, context.sessionId.GetString().c_str(), GetChar_id());

	OutputDebugString(L"CCharacter::GetPlayerEquip 11111111111\n");

	TCHAR szBuffer[512];  
	_snwprintf(szBuffer,sizeof(szBuffer),L"char_id = %d  GetChar_id =%d ",char_id,GetChar_id());
	OutputDebugString(szBuffer); 

	if (char_id == GetChar_id())
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F(" do not request self equip."));
		return RPC_RESULT_OK;
	}

	CCharacter* pchar = gGlobal.m_charToSession.FindCharacter(char_id);
	if (pchar)
	{
		OutputDebugString(L"CCharacter::GetPlayerEquip 22222222222222\n");
		if (pchar->GetEnableShowEquip())
		{
			OutputDebugString(L"CCharacter::GetPlayerEquip 333333333333\n");
			NetGroup itemGroup;

			CItemBag * pItemBag = pchar->GetItems();
			if(pItemBag)
			{
				OutputDebugString(L"CCharacter::GetPlayerEquip 444444444444\n");
				for (UInt8 i = 0; i < BAG_EQUIP_COUNT; ++i)
				{
					OutputDebugString(L"CCharacter::GetPlayerEquip 55555555555\n");
					CItem * item = pItemBag->GetItem(i + BAG_MAX_ITEM);
					if(item)
					{
						OutputDebugString(L"CCharacter::GetPlayerEquip 666666666666 \n");
						item->SetSendMask(CItem::AllMask);
						itemGroup.Add(item);
					}
				}
			}

			if(itemGroup.GetCount() > 0)
			{
				OutputDebugString(L"CCharacter::GetPlayerEquip 77777777777777\n");

				CB_GetPlayerEquip(m_pPeer, m_context,char_id);
			}
		}
		else
		{
			CB_Error(m_pPeer, m_context, 1, _T("MSG_TARGET_EQUIP_REQUEST_DISABLE"));
		}
	}
	else
	{
		CB_Error(m_pPeer, m_context, 1, _T("MSG_TARGET_EQUIP_REQUEST_LINE_FAIL"));
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" target offline, sessionId %s, char_id %d"), 
			context.sessionId.GetString().c_str(), GetChar_id());
	}
	OutputDebugString(L"CCharacter::GetPlayerEquip 8888888888888888888\n");
	return RPC_RESULT_OK;
}


//////////////////////////////////
///		SellYuanBao				//
//////////////////////////////////

void CCharacter::OpenSellYuanBao()
{	
	UInt msgIndex = 0;
	do
	{
		if (!IsInMap())
		{
			msgIndex = WARN_NOINMAP_CANT_SELLYUAN;
			break;
		}

		if (IsTrading())
		{
			msgIndex = WARN_TRADING_CANT_SELLYUAN;
			break;
		}

		if (IsHawking())
		{
			msgIndex = WARN_HAWKING_CANT_SELLYUAN;
			break;
		}
	}
	while(false);

	if (msgIndex == 0)
		WorldService_Proxy::GetSellYuanBao(gGlobal.pWorldService->pPeer, GetContext());
	else
		CB_WarningMessage(GetPeer(), GetContext(), msgIndex, 0, WARN_SHOW_IN_ALL);
}

RPCResult CCharacter::AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &pricePerUnit, const ByteArray &password)
{
	PROCEDURE_START_RPC3(Character, AddSellYuanBao, this, UInt32, YBT, UInt32, pricePerUnit, ByteArray, password)		

		PROCEDURE_WAIT_TIMEOUT(1, 2000) //delay 2 second	

		UInt msgIndex = SELL_YB_ADD_RECORD_SUCCESS;
		Int32 sellYuanBaoTax = 10000;
		do
		{
			if (!IsInMap())
			{
				msgIndex = SELL_YB_NOT_IN_MAP;
				break;
			}

			if (GetScriptSystem()->IsRunning())
			{
				msgIndex = SELL_YB_SCRIPT_RUNNING;
				break;
			}

			if (IsTrading())
			{
				msgIndex = SELL_YB_IN_TRADING;
				break;
			}

			if (IsHawking())
			{
				msgIndex = SELL_YB_IN_HAWKING;
				break;
			}

			if (GetMoney() < (UInt)sellYuanBaoTax)
			{
				msgIndex = SELL_YB_NOT_ENOUGH_MONEY;
				break;
			}

			if (GetYuanBaoTicket() < YBT)
			{
				msgIndex = SELL_YB_NOT_ENOUGH_YBT;
				break;
			}
		}
		while(false);

		if (msgIndex == SELL_YB_ADD_RECORD_SUCCESS) //10000 = tax
		{
			ChangeMoney(-sellYuanBaoTax);
			WorldService_Proxy::AddSellYuanBao(gGlobal.pWorldService->pPeer, context, YBT, pricePerUnit, password);
		}
		else
		{
			PROCEDURE_GO_STEP1(2, UInt8, msgIndex)
		}
		
	PROCEDURE_WAIT1(2, WorldService, CB_AddSellYuanBao, UInt8, msgIndex)

		Int32 sellYuanBaoTax = 10000;

		if (msgIndex == SELL_YB_ADD_RECORD_SUCCESS)
		{
			SetYuanBaoTicket(GetYuanBaoTicket() - YBT);
			String rumor;
			rumor.Format(gGlobal.GetStringTable()->Get(_T("MSG_YB_SELL_RUMOR")), YBT, pricePerUnit);
			MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
				RPCContext(), rumor, 0, 1);
		}
		else if (msgIndex == YB_MSG_WRONG_PASSWORD)
			ChangeMoney(sellYuanBaoTax);

		CB_AddSellYuanBao(pPeer, context, msgIndex);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());			

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID, const UInt32 &YBT, const UInt32 &moneyPerUnit, const ByteArray &password)
{
	PROCEDURE_START_RPC4(Character, BuySellYuanBao, this, UInt32, sellYuanBaoID, UInt32, YBT, UInt32, moneyPerUnit, ByteArray, password)		

		PROCEDURE_WAIT_TIMEOUT(1, 2000) //delay 2 second	

		UInt msgIndex = BUY_YB_SUCCESS;
		do
		{
			if (!IsInMap())
			{
				msgIndex = BUY_YB_NOT_IN_MAP;
				break;
			}

			if (GetScriptSystem()->IsRunning())
			{
				msgIndex = BUY_YB_SCRIPT_RUNNING;
				break;
			}

			if (IsTrading())
			{
				msgIndex = BUY_YB_IN_TRADING;
				break;
			}

			if (IsHawking())
			{
				msgIndex = BUY_YB_IN_HAWKING;
				break;
			}			

			if (YBT * moneyPerUnit > GetMoney())
			{
				msgIndex = BUY_YB_NO_MONEY;
				break;
			}

			if (YBT <= 0)
			{
				msgIndex = BUY_YB_ERROR;
				break;
			}
		}
		while(false);

		if (msgIndex == BUY_YB_SUCCESS)
		{
			WorldService_Proxy::BuySellYuanBao(gGlobal.pWorldService->pPeer, context, sellYuanBaoID, YBT, GetMoney(), moneyPerUnit, password);
			ChangeMoney(-(Int)(YBT * moneyPerUnit));
		}
		else
		{
			PROCEDURE_GO_STEP2(2, UInt8, msgIndex, Boolean, BOOLEAN_FALSE)
		}

	PROCEDURE_WAIT2(2, WorldService, CB_BuySellYuanBao, UInt8, msgIndex, Boolean, fromWorld)

		if (msgIndex == BUY_YB_SUCCESS)
		{
			SetYuanBaoTicket(GetYuanBaoTicket() + YBT);
			String rumor;
			rumor.Format(gGlobal.GetStringTable()->Get(_T("MSG_YB_BUY_RUMOR")), moneyPerUnit, YBT);
			MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
				RPCContext(), rumor, 0, 1);
		}
		else if (fromWorld)
			ChangeMoney(YBT * moneyPerUnit);

		CB_BuySellYuanBao(pPeer, context, msgIndex);
		
	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());			

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const ByteArray &password)
{
	PROCEDURE_START_RPC1(Character, GetAllMoneyAndYBT, this, ByteArray, password)	

		PROCEDURE_WAIT_TIMEOUT(1, 2000) //delay 2 second	
		
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("start session_id %s, char_id %d"), 
			context.sessionId.GetString().c_str(), GetChar_id());

		UInt msgIndex = TEMPBANK_SUCCESS;
		do
		{
			if (!IsInMap())
			{
				msgIndex = TEMPBANK_NOT_IN_MAP;
				break;
			}

			if (GetScriptSystem()->IsRunning())
			{
				msgIndex = TEMPBANK_SCRIPT_RUNNING;
				break;
			}

			if (IsTrading())
			{
				msgIndex = TEMPBANK_IN_TRADING;
				break;
			}

			if (IsHawking())
			{
				msgIndex = TEMPBANK_IN_HAWKING;
				break;
			}						
		}
		while(false);

		if (msgIndex == TEMPBANK_SUCCESS)
		{
			WorldService_Proxy::GetTempBank(gGlobal.pWorldService->pPeer, context, password);
		}
		else
		{
			PROCEDURE_GO_STEP3(2, UInt8, msgIndex, UInt32, 0, UInt32, 0)
		}

	PROCEDURE_WAIT3(2, WorldService, CB_GetTempBank, UInt8, msgIndex, UInt32, remainYBT, UInt32, totalMoney)

		if (msgIndex == TEMPBANK_SUCCESS)
		{
			ChangeMoney(totalMoney);
			SetYuanBaoTicket(GetYuanBaoTicket() + remainYBT);
		}

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("end session_id %s, char_id %d, success %d "), 
			context.sessionId.GetString().c_str(), GetChar_id(), (msgIndex == TEMPBANK_SUCCESS)? 1 : 0 );

		CB_GetAllMoneyAndYBT(pPeer, context, msgIndex);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());			

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::ForgiveLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId)
{
#ifdef LINEUP_SYSTEM_ON
	int iRet=-1;
	if ( uiLineupId != 0 && uiLineupId != 13) {
		if (HasLearedLineup(uiLineupId)) {
			RemoveSkill(uiLineupId);
			CParty* pParty = GetParty();
			if (NULL != pParty) {
				if ( uiLineupId == pParty->GetM_uLineupId() && IsLeader()) {
					pParty->SetLineup(13);
					pParty->SendToMembers();
				}
			}
			iRet=uiLineupId;
		}
		else
			iRet = -2;
	}
	CB_ForgiveLineup(pPeer, context,iRet);
#else
	//When Lineup system close notification write here
	String strMsg;
	strMsg.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_LINEUP_SYSTEM_OFF")));
	CB_Error(pPeer,context,1,strMsg);
#endif
	return RPC_RESULT_OK;
}

RPCResult CCharacter::ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId)
{
#ifdef LINEUP_SYSTEM_ON
	UInt16 uiRet = USHRT_MAX;
	if (HasParty()) {
		if (IsLeader()) {
			if (HasLearedLineup(uiLineupId)) {
				CParty* pParty = GetParty();
				if (NULL != pParty) {
					pParty->SetLineup(uiLineupId);
					pParty->SendToMembers();
					uiRet = uiLineupId;
				}
				else
					uiRet = USHRT_MAX-2;
			}
			else
				uiRet = USHRT_MAX-3;
		}
		else
			uiRet = USHRT_MAX-4;
	}
	else
		uiRet = USHRT_MAX-5;

	CB_ModifyLineup(pPeer,context,uiRet);
#else
	//When lineup close notification write here
	String strMsg;
	strMsg.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_LINEUP_SYSTEM_OFF")));
	CB_Error(pPeer,context,1,strMsg);
#endif
	return RPC_RESULT_OK;
}

RPCResult CCharacter::ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos)
{
#ifdef LINEUP_SYSTEM_ON
	int iRet = -1;
	if (HasParty()) {
		if (IsLeader()) {
			CParty* pParty = GetParty();
			if (NULL != pParty) {
				pParty->SetLineupkeypos(iKeyPos);
				pParty->SendToMembers();
				iRet = iKeyPos;
			}
			else
				iRet = -3;
		}
		else
			iRet = -2;
	}

	CB_ModifyLineupKeyPos(pPeer,context,iRet);
#else
	//When lineup system close,notification write here
	String strMsg;
	strMsg.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_LINEUP_SYSTEM_OFF")));
	CB_Error(pPeer,context,1,strMsg);
#endif
	return RPC_RESULT_OK;
}

BOOL CCharacter::HasLearedLineup(UInt16 uiLineupId)
{
	return NULL != m_pSkillGroup->GetSkill(uiLineupId);
}

RPCResult CCharacter::DrawTargetUidFromHawk(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CCharacter, DrawTargetUidFromHawk, this, context, UInt32, targetId);
	MessageService_Proxy::GetTargetFromHawk(gGlobal.pMessageService->pPeer, context,
		targetId);
	PROCEDURE_WAIT2(1, MessageService, CB_GetTargetFromHawk, bool, isonline, String, char_name);
	CB_SetTargetNameFormHawk(m_pPeer,m_context,targetId,char_name,isonline);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(MessageService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("DrawTargetUidFromHawk Error: MessageService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH
		return RPC_RESULT_OK;
}

RPCResult CCharacter::UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const String &targetName, const Int32 &money)
{
	PROCEDURE_START_RPC3(Character, UpdateWantedReward, this, UInt32, targetId, String, targetName, Int32, money)
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F(" CCharacter::WantedReward, targetId:%d, money:%d,GetMoney:%d"),
		targetId, money,GetMoney());
	if(money > 0 && GetMoney() < (UInt32)money)
	{
		m_pScriptSys->ShowError("MSG_WANTED_NOT_ENOUGH_MONEY");
		ShowRank(CHAR_RANK_TYPE_PKVALUE);
		PROCEDURE_RETURN;
	}
	WorldService_Proxy::UpdateWantedReward(gGlobal.pWorldService->pPeer, context,targetId, money);
	PROCEDURE_WAIT1(1, WorldService, CB_UpdateWantedReward, Boolean,result);
	if(result)
	{
		if(money > 0)
		{
			ChangeMoney(-(Int32)money,LogService_Proxy::Money_And_Exp::SOURCE_WANTED_REWARD_GIVEN);
		}
		if(money >= 1000000)
		{
			String msg;
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_RUMOR_WANTED_CATCH_REWARD")), GetNickName().c_str(),money,targetName.c_str());
			MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
				RPCContext(), msg, 0, 1);
		}
		CB_UpdateWantedReward(pPeer,context,targetId,money,BOOLEAN_TRUE);
		ShowRank(CHAR_RANK_TYPE_PKVALUE);
	}
	else
	{
		CB_UpdateWantedReward(pPeer,context,targetId,money,BOOLEAN_FALSE);
	}
	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("WorldService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());	
		CB_UpdateWantedReward(pPeer,context,targetId,money,BOOLEAN_FALSE);

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}

void CCharacter::UpdateWantedReward(UInt32 targetId, Int32 money)
{
	WorldService_Proxy::UpdateWantedReward(gGlobal.pWorldService->pPeer, m_context,targetId, money);
}

void CCharacter::RequestJailDeliveryId()
{
	CB_JailDelivery(GetPeer(),GetContext());
}

void CCharacter::RequestBailId()
{
	CB_WantedBail(GetPeer(),GetContext());
}

RPCResult CCharacter::JailDelivery(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	CCharacter* pTarget = gGlobal.m_charToSession.FindCharacter(targetId);
	if(targetId != 0 && HasLuaFunc(gGlobal.g_pLuaState, "OnCharacterJailDelivery"))
	{
		CallLuaFunc(gGlobal.g_pLuaState, "OnCharacterJailDelivery", String(), this,pTarget);

		UInt32 result = (UInt32)gGlobal.g_TempResult;
		if(result && pTarget != NULL )
		{

			String msg;
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_RUMOR_JAIL_DELIVERY")), pTarget->GetNickName().c_str());

			MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
				RPCContext(), msg, 0, 1);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::WantedBail(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	CCharacter* pTarget = gGlobal.m_charToSession.FindCharacter(targetId);
	if(targetId != 0 && HasLuaFunc(gGlobal.g_pLuaState, "OnCharacterWantedBail"))
	{
		CallLuaFunc(gGlobal.g_pLuaState, "OnCharacterWantedBail", String(), this,pTarget);

		UInt32 result = (UInt32)gGlobal.g_TempResult;
		if(result && pTarget != NULL )
		{

			String msg;
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_RUMOR_WANTED_BAIL")), GetNickName().c_str(),pTarget->GetNickName().c_str(),pTarget->GetNickName().c_str());
			MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
				RPCContext(), msg, 0, 1);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::WantedTrace(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	PROCEDURE_START_RPC1(Character, WantedTrace, this, UInt32, targetId)
		WorldService_Proxy::WantedDetail(gGlobal.pWorldService->pPeer, context, targetId);
		PROCEDURE_WAIT5(1, WorldService, CB_WantedDetail,String,name,UInt32,lineId,UInt32,mapId,UInt32,mapX,UInt32,mapY);
		if( mapId != 0 && HasLuaFunc(gGlobal.g_pLuaState, "OnCharacterWantedTrace") )
		{
			CallLuaFunc(gGlobal.g_pLuaState, "OnCharacterWantedTrace", String(), this);
			UInt32 result = (UInt32)gGlobal.g_TempResult;
			if(result == 1)
			{
				CB_WantedTrace(GetPeer(),context,name,lineId,mapId,mapX,mapY);
			}
		}
		else
		{
			CB_WantedTrace(GetPeer(),context,name,lineId,mapId,mapX,mapY);
		}
	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)
	PROCEDURE_END_CATCH
		return RPC_RESULT_OK;
}

void CCharacter::SetPK_Value(const UInt16 &PK_Value)
{
	if(GetPK_Value() > 5 && PK_Value < 6)
	{
		WorldService_Proxy::RemoveFromWantedRank(gGlobal.pWorldService->pPeer, m_context,GetChar_id());
	}
	Parent::SetPK_Value(PK_Value);
}

void CCharacter::RequestTraceId()
{
	CB_RequestWantedTrace(GetPeer(),GetContext());
}

RPCResult CCharacter::GetGenerationGroup(LPCPEER pPeer, RPCContext &context)
{
	NetGroup ngGntGrp;
	if (m_bGenerationInit) {
		if ( NULL != m_pGeneration
			&& m_pGeneration->m_mapGeneration.size() > 0 ) {
			CGenerationGroup::GenerationMap::iterator it = m_pGeneration->m_mapGeneration.begin();
			for (;it != m_pGeneration->m_mapGeneration.end();it++) {
				CGeneration* pCurGen=it->second;
				if (NULL != pCurGen) {
					pCurGen->SetSendMask(CGeneration::AllMask);
					ngGntGrp.Add(pCurGen);
				}
			}
		}
	}
	else{
		if ( NULL != m_pGeneration
			&& m_pGeneration->m_mapGeneration.size() > 0 )
		{
			UInt32Vector uiVtRequestIds;
			CGenerationGroup::GenerationMap::iterator it = m_pGeneration->m_mapGeneration.begin();
			for (;it != m_pGeneration->m_mapGeneration.end();it++) {
				CGeneration* pCurGen=it->second;
				if (NULL != pCurGen) {
					CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(pCurGen->GetUiRelatedId());
					if (NULL != pTargetChar) {
						pCurGen->SetUi16Level(pTargetChar->GetLevel());
						pCurGen->SetUi8Class(pTargetChar->GetSexClass());
						pCurGen->SetUiOfflineTime(0);
						if (pCurGen->GetBYoung()) {
							pCurGen->SetUiGiftPointOrHistAccm(pTargetChar->GetIHisGenAcc());
						}
					}
					else{
						pCurGen->SetUiOfflineTime(1);
						uiVtRequestIds.push_back(pCurGen->GetUiRelatedId());
					}
					pCurGen->SetSendMask(CGeneration::AllMask);
					ngGntGrp.Add(pCurGen);
				}
			}
			if (uiVtRequestIds.size() > 0)
				MessageService_Proxy::GetGenerationChars(gGlobal.pMessageService->pPeer,context,uiVtRequestIds);
		}
		m_bGenerationInit = TRUE;
	}
	
	CB_UpdateGenerationAcc(pPeer,context,GetUiCurGenAcc(),GetIHisGenAcc());
	CB_GetGenerationGroup(pPeer,context,&ngGntGrp);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::RequestGeneration(LPCPEER pPeer, 
										RPCContext &context,
										const UInt32 &uiTargetId)
{
	int iRet =0;
	CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(uiTargetId);
	if (NULL != pTargetChar) {
		if (NULL != m_pGeneration
			&& m_pGeneration->m_mapGeneration.size() < GENERATIONLIMITEDNUM ) {
				if (NULL != pTargetChar->m_pGeneration
					&& pTargetChar->m_pGeneration->m_mapGeneration.size() < GENERATIONLIMITEDNUM) {
				if ((GetLevel() < GENERATIONLIMITEDLEVEL && pTargetChar->GetLevel() >= GENERATIONLIMITEDLEVEL)
					|| (GetLevel() >= GENERATIONLIMITEDLEVEL && pTargetChar->GetLevel() < GENERATIONLIMITEDLEVEL)) 
				{
					if (NULL != m_pQuestSys) {
						if (m_pQuestSys->TimeMarkOver(gConfigure.ui32GenerationTimeMark,gConfigure.ui16GenerationTimeLong)) {
							if (NULL != pTargetChar->GetQuests()) {
								if (pTargetChar->GetQuests()->TimeMarkOver(gConfigure.ui32GenerationTimeMark,gConfigure.ui16GenerationTimeLong)) {
									CGenerationGroup::GenerationMap::iterator it = m_pGeneration->m_mapGeneration.find(uiTargetId);
									if (it == m_pGeneration->m_mapGeneration.end()) {
										if (!pTargetChar->IsInBattle()) {
											pTargetChar->CB_RequestGenerationTrans(pTargetChar->GetPeer(),
												pTargetChar->GetContext(),
												GetChar_id());
										}
										else
											iRet = -8;
									}
									else
										iRet = -1;
								}
								else
									iRet = -7;
							}
						}
						else
							iRet = -6;
					}
				}
				else
					iRet = -2;
			}
			else
				iRet = -3;
		}
		else
			iRet = -4;
	}
	else
		iRet = -5;
	
	if (iRet != 0) {
		CB_RequestGenerationResult(pPeer,context,iRet,uiTargetId);
	}

	return RPC_RESULT_OK;
}
RPCResult CCharacter::RequestGenerationAns(LPCPEER pPeer,
										   RPCContext &context, 
										   const BOOL &bConfirm,
										   const UInt32 &uiRelatedId)
{
	int iRet = 0;
	CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(uiRelatedId);
	if (NULL != pTargetChar) {
		if (bConfirm) {
			if ((GetLevel() < GENERATIONLIMITEDLEVEL && pTargetChar->GetLevel() >= GENERATIONLIMITEDLEVEL)
				|| (GetLevel() >= GENERATIONLIMITEDLEVEL && pTargetChar->GetLevel() < GENERATIONLIMITEDLEVEL)) 
			{
				if (NULL != m_pGeneration
					&& m_pGeneration->m_mapGeneration.size()< GENERATIONLIMITEDNUM) {
						if (NULL != pTargetChar->m_pGeneration
							&& pTargetChar->m_pGeneration->m_mapGeneration.size() < GENERATIONLIMITEDNUM) {
						if (NULL != m_pQuestSys) {
							if (m_pQuestSys->TimeMarkOver(gConfigure.ui32GenerationTimeMark,gConfigure.ui16GenerationTimeLong)) {
								if (NULL != pTargetChar->GetQuests()) {
									if (pTargetChar->GetQuests()->TimeMarkOver(gConfigure.ui32GenerationTimeMark,gConfigure.ui16GenerationTimeLong)) {
										CGenerationGroup::GenerationMap::iterator it = m_pGeneration->m_mapGeneration.find(uiRelatedId);
										if (it == m_pGeneration->m_mapGeneration.end()) {
											CreateGenerationLocal(uiRelatedId);
											pTargetChar->CreateGenerationLocal(GetChar_id());
										}
										else
											iRet= -1;
									}
									else
										iRet = -7;
								}
							}
							else
								iRet = -6;
						}
					}
					else
						iRet = -3;
				}
				else
					iRet = -4;
			}
			else
				iRet = -2;
		}
		else{
			pTargetChar->CB_RequestGenerationResult(pTargetChar->GetPeer(),
													pTargetChar->GetContext(),
													1,
													GetChar_id());
		}
	}
	else
		iRet = -5;

	if (iRet != 0) {
		CB_RequestGenerationResult(pPeer,context,iRet,uiRelatedId);
	}

	return RPC_RESULT_OK;
}
RPCResult CCharacter::DeleteGeneration(LPCPEER pPeer, 
									   RPCContext &context, 
									   const UInt32 &uiRelatedId)
{
	int iRet= 0;
	if (NULL == m_pGeneration) 
		return RPC_RESULT_FAIL;
	CGenerationGroup::GenerationMap::iterator it = m_pGeneration->m_mapGeneration.find(uiRelatedId);
	if (it != m_pGeneration->m_mapGeneration.end()) {
		if (m_pQuestSys->TimeMarkOver(gConfigure.ui32GenerationTimeMark,gConfigure.ui16GenerationTimeLong))
		{
			CGeneration* pGnt = it->second;
			if (NULL != pGnt) {
				if (pGnt->GetBYoung()) {
					ObjectService_Proxy::DeleteGeneration(gGlobal.pObjectService->pPeer,
						context,
						GetChar_id(),
						uiRelatedId,
						pGnt->GetBYoung());
				}
				else{
					ObjectService_Proxy::DeleteGeneration(gGlobal.pObjectService->pPeer,
						context,
						uiRelatedId,
						GetChar_id(),
						pGnt->GetBYoung());
				}
				CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(uiRelatedId);
				if (NULL != pTargetChar)
					pTargetChar->DeleteGenerationLocal(GetChar_id());
				else if ( 0 == pGnt->GetUiOfflineTime()) {
					WorldService_Proxy::DeleteGenerationTrans(gGlobal.pWorldService->pPeer,
						context,uiRelatedId,GetChar_id());
				}
				DeleteNetObject(pGnt);
			}

			m_pGeneration->m_mapGeneration.erase(it);
			if (NULL != m_pQuestSys) {
				m_pQuestSys->SetTimeMark(gConfigure.ui32GenerationTimeMark);
			}
		}
		else
			iRet= -2;
	}
	else
		iRet = -1;
	
	CB_DeleteGeneration(pPeer,context,iRet,uiRelatedId);
	return RPC_RESULT_OK;
}
void CCharacter::DeleteGenerationLocal(UInt32 uiRelatedId)
{
	CGenerationGroup::GenerationMap::iterator it = m_pGeneration->m_mapGeneration.find(uiRelatedId);
	if (it != m_pGeneration->m_mapGeneration.end()) {
		CGeneration* pGnt = it->second;
		if (NULL != pGnt)
			DeleteNetObject(pGnt);
		m_pGeneration->m_mapGeneration.erase(it);
		
		CB_DeleteGeneration(GetPeer(),GetContext(),0,uiRelatedId);
	}
}
void CCharacter::CreateGenerationLocal(UInt32 uiRelatedId)
{
	CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(uiRelatedId);
	if (NULL != pTargetChar && NULL != m_pGeneration && NULL != pTargetChar->m_pGeneration) {
		CGenerationGroup::GenerationMap::iterator it = m_pGeneration->m_mapGeneration.find(uiRelatedId);
		if (it == m_pGeneration->m_mapGeneration.end()) {
			CGeneration* pGnt = CreateNetObject<CGeneration>();
			if (NULL != pGnt) {
				BOOL bYoung = GetLevel() < GENERATIONLIMITEDLEVEL ? TRUE : FALSE;
				pGnt->SetUiRelatedId(uiRelatedId);
				pGnt->SetBYoung(bYoung);
				pGnt->SetStrNickName(pTargetChar->GetNickName());
				pGnt->SetUi16Level(pTargetChar->GetLevel());
				pGnt->SetUi8Class(pTargetChar->GetSexClass());
				pGnt->SetUiAccmulOrRecvExp(0);
				pGnt->SetUiOfflineTime(0);
				if (!bYoung) {
					pGnt->SetUiGiftPointOrHistAccm(3);
				}
				else{
					pGnt->SetUiGiftPointOrHistAccm(pTargetChar->GetIHisGenAcc());
					CGenerationData gntdata;
					gntdata.SetBYoung(pGnt->GetBYoung());
					gntdata.SetUiYoungerId(GetChar_id());
					gntdata.SetUiElderId(pGnt->GetUiRelatedId());
					gntdata.SetStrNickName(pGnt->GetStrNickName());
					gntdata.SetSendMask(CGenerationData::AllMask);
					ObjectService_Proxy::CreateGeneration(gGlobal.pObjectService->pPeer,
														GetContext(),&gntdata,GetNickName());
				}
				m_pGeneration->m_mapGeneration.insert(std::make_pair(uiRelatedId,pGnt));
				CB_RequestGenerationResult(GetPeer(),GetContext(),0,uiRelatedId);
			}
		}
		else
			CB_RequestGenerationResult(GetPeer(),GetContext(),-6,uiRelatedId);
	}
}
void CCharacter::CalcGenerationExpCont(UInt32 uiNextLvExp)
{
	int iOnlineCnt=0;
	CGenerationGroup::GenerationMap::iterator it = m_pGeneration->m_mapGeneration.begin();
	for (;it != m_pGeneration->m_mapGeneration.end();it++) {
		CGeneration* pGent = it->second;
		if (NULL != pGent) {
			if (pGent->GetUiOfflineTime() == 0) {
				iOnlineCnt++;
				pGent->SetUiAccmulOrRecvExp(pGent->GetUiAccmulOrRecvExp()+uiNextLvExp/200);
				pGent->SetModifyState(TRUE);
			}
		}
	}

	SetIHisGenAcc(GetIHisGenAcc()+iOnlineCnt*(uiNextLvExp/200));
	SetUiCurGenAcc(GetUiCurGenAcc()+iOnlineCnt*(uiNextLvExp/200));
	CB_UpdateGenerationAcc(GetPeer(),GetContext(),GetUiCurGenAcc(),GetIHisGenAcc());
}
void CCharacter::CalcGenerationAccCont()
{
	CGenerationGroup::GenerationMap::iterator it = m_pGeneration->m_mapGeneration.begin();
	for (;it != m_pGeneration->m_mapGeneration.end();it++) {
		CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(it->first);
		if (NULL != pTargetChar)
			pTargetChar->RecvGenerationAcc(GetChar_id(),GetLevel());
		else
			WorldService_Proxy::RecvGenerationAccmulationTrans(gGlobal.pWorldService->pPeer,
																GetContext(),
																it->first,GetChar_id(),GetLevel());
	}
}
RPCResult CCharacter::GetSavedExp(LPCPEER pPeer, RPCContext &context)
{
	if (GetLevel() < GENERATIONLIMITEDLEVEL) {
		if (GetUiCurGenAcc() > 0) {
			ChangeExp(GetUiCurGenAcc(),LogService_Proxy::Money_And_Exp::SOURCE_GENERATION_EXP_GET);
			SetUiCurGenAcc(0);
			CB_UpdateGenerationAcc(pPeer,context,0,GetIHisGenAcc());
		}
	}
	return RPC_RESULT_OK;
}
void CCharacter::ChangeGenerationType()
{	
	CGenerationGroup::GenerationMap::iterator it = m_pGeneration->m_mapGeneration.begin();
	for (;it != m_pGeneration->m_mapGeneration.end();it++) {
		CGeneration* pGent = it->second;
		if (NULL != pGent) {
			if (pGent->GetBYoung()) {
				ObjectService_Proxy::DeleteGeneration(gGlobal.pObjectService->pPeer,
					GetContext(),
					GetChar_id(),
					pGent->GetUiRelatedId(),
					pGent->GetBYoung());
			}
			else{
				ObjectService_Proxy::DeleteGeneration(gGlobal.pObjectService->pPeer,
					GetContext(),
					pGent->GetUiRelatedId(),
					GetChar_id(),
					pGent->GetBYoung());
			}
			CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(pGent->GetUiRelatedId());
			if (NULL != pTargetChar)
				pTargetChar->DeleteGenerationLocal(GetChar_id());
			else if ( 0 == pGent->GetUiOfflineTime()) {
				WorldService_Proxy::DeleteGenerationTrans(gGlobal.pWorldService->pPeer,
					GetContext(),pGent->GetUiRelatedId(),GetChar_id());
			}
			DeleteNetObject(pGent);
		}
	}
	
	m_pGeneration->m_mapGeneration.clear();
	if (NULL != m_pQuestSys) {
		m_pQuestSys->RemoveTimeMark(gConfigure.ui32GenerationTimeMark);
	}

	ChangeExp(GetUiCurGenAcc(),LogService_Proxy::Money_And_Exp::SOURCE_GENERATION_EXP_GET);
	SetUiCurGenAcc(0);
	SetIHisGenAcc(0);
	CB_UpdateGenerationAcc(GetPeer(),GetContext(),0,0);
	
	CB_ChangeGenerationType(GetPeer(),GetContext(),1);
}


void CCharacter::OpenBusinessShop(UInt16 shopId)
{
	if (m_pBag)
	{
		MessageService_Proxy::OpenBusinessShopMenu(gGlobal.pMessageService->pPeer, m_context ,GetChar_id(), shopId, m_pBag->GetBusinessMoney());
	}
}

void CCharacter::ChangeBusinessProfit(Int32 profit)
{
	UInt32 guild_uid = GetGuildUid();
	if (guild_uid > 0)
	{
		MessageService_Proxy::ChangeBusinessProfit(gGlobal.pMessageService->pPeer, m_context, guild_uid, GetChar_id(), profit);
	}
}

void CCharacter::OpenBusinessLog()
{
	if (HasGuild())
		CB_OpenBusinessLog(m_pPeer, m_context);
	else
		CB_Error(m_pPeer, m_context, 0, _T("MSG_NO_GUILD"));
}

void CCharacter::RecvGenerationAcc(UInt32 uiRelatedId,UInt32 uiRecvAcc)
{
	if (NULL != m_pGeneration) {
		CGenerationGroup::GenerationMap::iterator itTarget = m_pGeneration->m_mapGeneration.find(uiRelatedId);
		if (itTarget != m_pGeneration->m_mapGeneration.end()) {
			CGeneration* pGent = itTarget->second;
			if (NULL != pGent) {
				UInt32 uiGainedAcc = uiRecvAcc+pGent->GetUiAccmulOrRecvExp();
				SetIHisGenAcc(GetIHisGenAcc()+uiGainedAcc);
				SetUiCurGenAcc(GetUiCurGenAcc()+uiGainedAcc);
				LogService_Proxy::LogGenerationAcc(gGlobal.pLogService->pPeer,
					GetContext(),GetChar_id(),pGent->GetUiRelatedId(),uiGainedAcc,GetLevel(),pGent->GetUi16Level(),0,0);
				CB_UpdateGenerationAcc(GetPeer(),GetContext(),GetUiCurGenAcc(),GetIHisGenAcc());
				pGent->SetUiAccmulOrRecvExp(pGent->GetUiAccmulOrRecvExp()+uiRecvAcc);
				pGent->SetModifyState(TRUE);
				CB_GenerationGainAcc(GetPeer(),GetContext(),uiGainedAcc);
			}
		}
	}
}
void CCharacter::GenerationOnline(UInt32 uiRelatedId)
{
	if (NULL != m_pGeneration) {
		CGenerationGroup::GenerationMap::iterator itTarget = m_pGeneration->m_mapGeneration.find(uiRelatedId);
		if (itTarget != m_pGeneration->m_mapGeneration.end()) {
			CGeneration* pGnt= itTarget->second;
			if (NULL != pGnt) {
				CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(pGnt->GetUiRelatedId());
				if (NULL != pTargetChar) {
					pGnt->SetUi16Level(pTargetChar->GetLevel());
					pGnt->SetUi8Class(pTargetChar->GetSexClass());
					pGnt->SetUiOfflineTime(0);
					if (pGnt->GetBYoung()) {
						pGnt->SetUiGiftPointOrHistAccm(pTargetChar->GetIHisGenAcc());
					}
				}
				else{
					UInt32Vector uiRelatedId;
					uiRelatedId.push_back(pGnt->GetUiRelatedId());
					MessageService_Proxy::GetGenerationChars(gGlobal.pMessageService->pPeer,GetContext(),uiRelatedId);
				}
			}
		}
	}
}
void CCharacter::GenerationOffline(UInt32 uiRelatedId)
{
	if (NULL != m_pGeneration) {
		CGenerationGroup::GenerationMap::iterator itTarget = m_pGeneration->m_mapGeneration.find(uiRelatedId);
		if (itTarget != m_pGeneration->m_mapGeneration.end()) {
			CGeneration* pGnt= itTarget->second;
			if (NULL != pGnt) {
				time_t curTime;
				time(&curTime);
				pGnt->SetUiOfflineTime((UInt32)curTime);
			}
		}
	}
}

UInt32 CCharacter::GetGuildManorScale()
{
	return (m_ownGuild ? m_ownGuild->GetManorScale() : 0);
}

void CCharacter::ApplyTrader()
{
	if (HasGuild())
	{
		MessageService_Proxy::ApplyTrader(gGlobal.pMessageService->pPeer, m_context, GetGuildUid(), GetChar_id());
	}
}
void CCharacter::InitialRepeatBuff(ExtractedBuffData &dat, const BuffData *buffdat)
{
	// check it is a buff that will call the buff lua repeatedly over a fixed period
	if (!buffdat)
		return;
	if (buffdat->Buff_type == BUFF_TYPE_REPEAT)
	{
		dat.Repeat = 60*buffdat->Buff_TimeOut;  
	}
}

Boolean CCharacter::Sit()
{
	if (GetAction() == ST_STOP)
	{
		SetAction(ST_SIT, ::timeGetTime());
		return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}

Boolean CCharacter::IsSit()
{
	return (GetAction() == ST_SIT);
}

void CCharacter::ShowGuildMsg(PCSTRINGA msg, UInt32 guildUID)
{
	String s;
	UTF8ToStr(msg, s);

	GuildMap::iterator Itr = gGlobal.g_AllGuilds.find(guildUID);
	if (Itr != gGlobal.g_AllGuilds.end() && Itr->second != NULL)
	{
		MessageService_Proxy::TransmitGuildMsg(gGlobal.pMessageService->pPeer,
			m_context, s, guildUID);
	}

}

void CCharacter::ShowAwardMsg(PCSTRINGA msg)
{
	String s;
	UTF8ToStr(msg, s);
	if (s.IsEmpty())
		return;
	CB_AwardMsg(GetPeer(), GetContext(), s);
}

RPCResult CCharacter::SetChangePos(LPCPEER pPeer, RPCContext &context, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex)
{
	int iRetCode = -1;
	if (HasParty()) {
		if (IsLeader()) {
			CParty* pParty = GetParty();
			if (NULL != pParty) {
				iRetCode=pParty->SetChangePos(ui8PreIndex,ui8DestIndex);
			}
		}
		else
			iRetCode = -2;
	}
	if (iRetCode != 0) {
		CB_SetChangePos(GetPeer(),GetContext(),iRetCode,ui8PreIndex,ui8DestIndex);
	}
	return RPC_RESULT_OK;
}


void CCharacter::InitGenerationInfo(NetGroup* pOnlineChars)
{
	for (UInt16 i=0;i<pOnlineChars->GetCount();i++) {
		CMessageCharacter* pRelatedChar = pOnlineChars->GetChild<CMessageCharacter>(i);
		if (NULL != pRelatedChar) {
			CGenerationGroup::GenerationMap::iterator it = m_pGeneration->m_mapGeneration.find(pRelatedChar->GetChar_id());
			if (it != m_pGeneration->m_mapGeneration.end()) {
				CGeneration* pGnt = it->second;
				if (NULL != pGnt) {
					pGnt->SetUi16Level(pRelatedChar->GetLevel());
					pGnt->SetUi8Class(pRelatedChar->GetSexClass());
					pGnt->SetUiOfflineTime(0);
					if (pGnt->GetBYoung()) {
						pGnt->SetUiGiftPointOrHistAccm(pRelatedChar->GetUiHisGenAcc());
					}
				}
			}
		}
	}
	DeleteNetGroup(pOnlineChars,BOOLEAN_TRUE);
}

void CCharacter::SetCharNpcState(UInt8 direction,UInt8 atcion)
{
	if(m_marriageCharNpc)
	{
		m_marriageCharNpc->SetDirection(direction);
		if (m_viewManager)
		{
			m_charViewLst.clear();
			m_viewManager->GetCharacters(m_charViewLst);
			for (CharacterPtrList::iterator it = m_charViewLst.begin(); it != m_charViewLst.end(); ++it)
			{
				if (*it && !(*it)->IsInBattle() )
				{
					CB_SetMarriageNpcAction((*it)->GetPeer(),(*it)->GetContext(),m_marriageCharNpc->GetNpc_uid(),atcion,direction);
				}
			}
		}

		if (GetPeer() && !IsInBattle())
		{
			m_marriageCharNpc->SetNickName(GetNickName());
			CB_SetMarriageNpcAction(m_pPeer,m_context,m_marriageCharNpc->GetNpc_uid(),atcion,direction);
		}
	}
}

void CCharacter::ShowMarriageEffect(UInt8 state)
{
	if(m_marriageCharNpc)
	{
		if (m_viewManager)
		{
			m_charViewLst.clear();
			m_viewManager->GetCharacters(m_charViewLst);
			for (CharacterPtrList::iterator it = m_charViewLst.begin(); it != m_charViewLst.end(); ++it)
			{
				if (*it && !(*it)->IsInBattle() )
				{
					CB_ShowMarriageEff((*it)->GetPeer(),(*it)->GetContext(),state,GetChar_id(),GetChar_id());
				}
			}
		}
		if (GetPeer() && !IsInBattle())
			CB_ShowMarriageEff(GetPeer(),GetContext(),state,GetChar_id(),GetChar_id());
	}
}
////frame rotation
//enum FrameRotation
//{
//	SOUTH = 0,
//	SOUTHEAST,
//	EAST,
//	NORTHEAST,
//	NORTH,
//	NORTHWEST,
//	WEST,
//	SOUTHWEST,
//	MAXROTATION,
//};
void CCharacter::SetMarriageState(UInt8 state)
{
	switch (state)
	{
	case MARRIAGE_INIT:
		SetVisible(false);
		if (HasParty())
			m_pParty->Leave(this,true,true);
		
		SetCurPosX(gConfigure.marriageCharMapX);
		SetCurPosY(gConfigure.marriageCharMapY);
		if(GetSex() == 0)
		{
			//SetCurPosX(gConfigure.marriageHusbandMapX);
			//SetCurPosY(gConfigure.marriageHusbandMapY);
			if(m_marriageCharNpc == NULL)
				m_marriageCharNpc = GetMap()->CreateNpc(gConfigure.marriageHusbandNpcId, gConfigure.marriageHusbandMapX, gConfigure.marriageHusbandMapY);
		}
		else
		{
			//SetCurPosX(gConfigure.marriageWifeMapX);
			//SetCurPosY(gConfigure.marriageWifeMapY);
			if(m_marriageCharNpc == NULL)
				m_marriageCharNpc = GetMap()->CreateNpc(gConfigure.marriageWifeNpcId, gConfigure.marriageWifeMapX, gConfigure.marriageWifeMapY);
		}
		SetCharNpcState(NORTHWEST,0);
		break;
	case MARRIAGE_HEAVEN_EARTH:
		SetCharNpcState(NORTHWEST,1);
		break;
	case MARRIAGE_MASTER:
		SetCharNpcState(SOUTHEAST,1);
			break;
	case MARRIAGE_FACETOFACE_STAND:
		if(GetSex() == 0)
			SetCharNpcState(NORTHEAST,0);
		else
			SetCharNpcState(SOUTHWEST,0);
		break;
	case MARRIAGE_FACETOFACE_BOW:
		if(GetSex() == 0)
			SetCharNpcState(NORTHEAST,1);
		else
			SetCharNpcState(SOUTHWEST,1);
		break;
	case MARRIAGE_RESET:
		if(m_marriageCharNpc != NULL)
		{
			m_marriageCharNpc->SetLifeTime(0);
			m_marriageCharNpc->Destroy();
			m_marriageCharNpc = NULL;
		}
		SetVisible(true);
		//JumpMap(GetMap_id(),gConfigure.marriageCharMapX,gConfigure.marriageCharMapY);
		SetCurPosX(gConfigure.marriageCharMapX);
		SetCurPosY(gConfigure.marriageCharMapY);
		break;
	default:
		break;
	}
}



void CCharacter::SetVisible(Boolean visible)
{
	if(visible)
	{
		SetMark(GetMark() & ~(1 << MARK_VISIBLE));
	}
	else
	{	
		SetMark(GetMark() | (1 << MARK_VISIBLE));
	}
	if (m_pMap)
		RaiseUniqEventID(EVT_MSGDATA_UPDATE);
	SendAttr(m_pPeer, m_context,GetUpdateMask());
}

RPCResult CCharacter::AddTitleHusband(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CCharacter, AddTitleHusband, this, context, UInt16, title_id)

	MessageService_Proxy::GetMessengersName(gGlobal.pMessageService->pPeer,context, GetChar_id(), FRIEND_RELATION_WIFE);
	PROCEDURE_WAIT2(1, MessageService, CB_GetMessengersName, UInt32, owner_id,StringVector, messagers)

		const TitleData* titleDataCSV = gGlobal.m_resource->GetTitleData(title_id);
	if (titleDataCSV && messagers.size() > 0)
	{
		String text;
		text.Format(_T("%s%s"), messagers[0].c_str(), titleDataCSV->Title_Name);
		AddTitle(title_id, 0, 0, text);
		SetTitle(text,title_id);
	}

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(MessageService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("AddTitleHusband Error: MessageService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CCharacter::AddTitleWife(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CCharacter, AddTitleWife, this, context, UInt16, title_id)

	MessageService_Proxy::GetMessengersName(gGlobal.pMessageService->pPeer,context, GetChar_id(), FRIEND_RELATION_HUSBAND);
	PROCEDURE_WAIT2(1, MessageService, CB_GetMessengersName, UInt32, owner_id,StringVector, messagers)

	const TitleData* titleDataCSV = gGlobal.m_resource->GetTitleData(title_id);
	if (titleDataCSV && messagers.size() > 0)
	{
		String text;
		text.Format(_T("%s%s"), messagers[0].c_str(), titleDataCSV->Title_Name);
		AddTitle(title_id, 0, 0, text);
		SetTitle(text,title_id);
	}

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(MessageService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("AddTitleWife Error: MessageService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

		return RPC_RESULT_OK;

}
RPCResult CCharacter::BuyAccShopItem(LPCPEER pPeer, RPCContext &context, const UInt32 &iIndex)
{
	int iRet = 0;
	const ACCSHOPITEM* pAccShopItemData = gGlobal.m_resource->GetAccShopData(iIndex);
	if (NULL != pAccShopItemData) {
		if (pAccShopItemData->uiAccmulationCost < GetUiCurGenAcc() && GetLevel() >= GENERATIONLIMITEDLEVEL ) {
			CItemBag* pBag=GetItems();
			if (NULL != pBag) {
				CItem* pItem = pBag->NewItem(pAccShopItemData->uiItemId,1);
				if (NULL != pItem) {
					UInt8 i;
					for (i = BAG_CURRENT; i < BAG_TYPE_COUNT; i++)
					{
						if (pBag->AddItem(pItem, i))
							break;
					}
					if (i < BAG_TYPE_COUNT) {
						UInt32 uCurAcc = GetUiCurGenAcc();
						uCurAcc -= pAccShopItemData->uiAccmulationCost;
						SetUiCurGenAcc(uCurAcc);
						LogService_Proxy::LogGenerationAcc(gGlobal.pLogService->pPeer,
							context,GetChar_id(),0,pAccShopItemData->uiAccmulationCost,GetLevel(),0,pItem->GetItem_uid(),pAccShopItemData->uiItemId);
						CB_UpdateGenerationAcc(pPeer,context,GetUiCurGenAcc(),GetIHisGenAcc());
						if (IsUpdated())
							RaiseUpdate();
					}
					else{
						DeleteNetObject((NetObject*&)pItem);
						iRet = -5;
					}
				}
				else
					iRet = -4;
			}
			else
				iRet = -3;
		}
		else
			iRet = -1;
	}
	else
		iRet = -2;
	
	CB_BugAccShopItem(pPeer,context,iRet,iIndex);
	return RPC_RESULT_OK;
}

PCSTRING CCharacter::GetCharNameLink()
{
	static String  name;
	if(!GetNickName().empty())
	{
		name.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHARLINK")), GetNickName().c_str(), GetNickName().c_str());
		//name.Format("#W\[NAME:%s\][%s]\#",GetNickName().c_str(), GetNickName().c_str())
	}
	else
		name.Format(_T(""));
	return name.c_str();
}

PCSTRING CCharacter::GetCharPartnerNameLink(UInt32 partnerID,UInt16 partnerIndex)
{
	static String lineTextName;
	static String  partnerName("");
	lineTextName.Format(_T("MSG_INTERFACE_LINE_%d"), GetMap()->GetLine_id());
	CPartner* partner = NULL;
	if(partnerIndex < 0)
		partner = GetPartners()->GetPartnerByID(partnerID);
	else
	{
		partner = GetPartners()->GetPartnerByIndex(partnerIndex);
		if(partner == NULL && partnerID > 0)
			partner = GetPartners()->GetPartnerByID(partnerID);
	}
	if(partner)
	{

		partnerName.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNERLINK")),15,
			partner->GetPartner_id(),GetChar_id(),16,17,partner->m_data->mid_name,
			gGlobal.GetStringTable()->Get(lineTextName.c_str()) );
	}
	else
		partnerName.Format(_T(""));
	return partnerName.c_str();
}

PCSTRING CCharacter::GetCharPetNameLink(UInt32 petID,UInt16 petIndex)
{
	String lineTextName;
	static String  petName("");
	lineTextName.Format(_T("MSG_INTERFACE_LINE_%d"), GetMap()->GetLine_id());
	CPet* pet = NULL;
	if(petIndex < 0)
		pet = GetPets()->GetPet(petID);
	else
	{
		pet = GetPets()->GetPetByIndex(petIndex);
		if (pet == NULL && petID > 0)
			pet = GetPets()->GetPet(petID);
	}
	if(pet)
	{

		petName.Format(gGlobal.GetStringTable()->Get(_T("MSG_PETLINK")),15,
			pet->GetPet_uid(),GetChar_id(),16,17,pet->m_pPetData->pet_name,
			gGlobal.GetStringTable()->Get(lineTextName.c_str()) );
	}
	else
		petName.Format(_T(""));
	return petName.c_str();
}

PCSTRING CCharacter::GetCharItemNameLink(UInt32 itemID,UInt8 itemIndex)
{
	String lineTextName;
	lineTextName.Format(_T("MSG_INTERFACE_LINE_%d"), GetMap()->GetLine_id());
	CItem* item = NULL;
	static String itemName("");
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("GetCharItemNameLink: Item ID = %d, itemIndex = %d"),
		itemID, itemIndex);
	if(itemIndex < 0)
		item = GetBag()->GetItemByUID(itemID);
	else
	{
		item = GetBag()->GetItem(itemIndex);
		if(item == NULL && itemID >0)
			item = GetBag()->GetItemByUID(itemID);
	}
	if(item)
	{
		itemName.Format(gGlobal.GetStringTable()->Get(_T("MSG_ITEMLINK")),15,
			item->GetItem_uid(),GetChar_id(),16,17,item->m_data->name,
			gGlobal.GetStringTable()->Get(lineTextName.c_str()));
	}
	else
		itemName.Format(_T(""));
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GetCharItemNameLink: itemName  = %s"),
		itemName.c_str());
	return itemName.c_str();
}

PCSTRING CCharacter::GetCharItemNameLink(CItem* item)
{
	String lineTextName;
	lineTextName.Format(_T("MSG_INTERFACE_LINE_%d"), GetMap()->GetLine_id());
	static String itemName("");
	if(item)
	{
		itemName.Format(gGlobal.GetStringTable()->Get(_T("MSG_ITEMLINK")),15,
			item->GetItem_uid(),GetChar_id(),16,17,item->m_data->name,
			gGlobal.GetStringTable()->Get(lineTextName.c_str()));
	}
	else
		itemName.Format(_T(""));
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GetCharItemNameLink: itemName  = %s"),
		itemName.c_str());
	return itemName.c_str();
}
RPCResult CCharacter::RequestCrossMapJump(LPCPEER pPeer, RPCContext &context, const UInt32 &uSrcJmpSeq, const UInt32 &uDstJmpSeq)
{
	//check state
	if (IsState(CCharacter::ST_BATTLE)
		||IsState(CCharacter::ST_JUMP_MAP)
		||IsState(CCharacter::ST_LEAVE_MAP)
		||IsState(CCharacter::ST_FOLLOW)
		||IsHawking()) 
	{
		return RPC_RESULT_OK;
	}
	
	if (uSrcJmpSeq == 1) 
	{
		if (GetLevel() < 10)
			return RPC_RESULT_OK;
	}
	const JUMPPOINT* pSrc = gGlobal.m_resource->GetAutoPathData(uSrcJmpSeq);
	if (NULL != pSrc && pSrc->uJumpType > 0)//no src point
	{
		BOOL bConnected = FALSE;
		//check source connected destinate
		for (UInt32 i=0;i<pSrc->uvRelatedPoint.size();i++) 
		{
			if (pSrc->uvRelatedPoint[i] == uDstJmpSeq) 
			{
				bConnected = TRUE;
				break;
			}
		}
		
		if (bConnected) 
		{
			//check in same map
			if (NULL != m_pMap) 
			{
				if (m_pMap->GetMap_id() == pSrc->uMapId) 
				{
					Stop();
					//check distance
					BlockPos bCurPos;
					bCurPos.PosX = GetCurPosX();
					bCurPos.PosY = GetCurPosY();
					BlockPos bJumpPos;
					bJumpPos.PosX = pSrc->u16PosX;
					bJumpPos.PosY = pSrc->u16PosY;
					PixelDirection pd;
					UInt32 uDistance = QGLMath::CalculateBlockPosDistance(bCurPos,bJumpPos,pd);
					if (uDistance < 10) 
					{
						const JUMPPOINT* pDest = gGlobal.m_resource->GetAutoPathData(uDstJmpSeq);
						if (NULL != pDest) 
						{//jump map
							JumpMap(pDest->uMapId,pDest->u16PosXJumped,pDest->u16PosYJumped);
						}
					}
				}
			}
		}
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::SearchParty(LPCPEER pPeer, RPCContext &context)
{
	if (m_pMap)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("char id %d"), GetChar_id());
		PartyInfoVector PartyInfoList;
		m_pMap->GetAllPartyInfo(PartyInfoList);
		if (!PartyInfoList.empty())
		{
			CB_SearchParty(pPeer, context, PartyInfoList);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::SearchChar(LPCPEER pPeer, RPCContext &context)
{
	if (m_pMap)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("char id %d"), GetChar_id());
		CharInfoVector CharInfoList;
		m_pMap->GetAllCharInfo(CharInfoList);
		if (!CharInfoList.empty())
		{
			CB_SearchChar(pPeer, context, CharInfoList);
		}
	}
	return RPC_RESULT_OK;
}

void CCharacter::OpenSearchPartyPanel()
{
	if (m_pPeer)
	{
		CB_OpenSearchPartyPanel(m_pPeer, m_context);
	}
}

void CCharacter::OpenWeaponsRankPanel()
{
	if (m_pPeer)
	{
		CB_OpenWeaponsRankPanel(m_pPeer, m_context);
	}
}

CItemBag * CCharacter::GetPlayerEquipData(UInt32 ID)
{
	if (m_pPeer)
	{
		TCHAR szBuffer[512];  
		_snwprintf(szBuffer,sizeof(szBuffer),L"GetPlayerEquipData  %d\n",ID);
		OutputDebugString(szBuffer); 
	}

	return NULL;
}

void CCharacter::HandleGuideEvent(UInt16 ID)
{
	UInt32 flag = GetQuests()->GetFlag(80010);
	if (flag == 0)
	{
		if (m_pPeer)
		{
			CB_HandleGuideEvent(m_pPeer, m_context, ID);
		}
	}
}

RPCResult CCharacter::GetPartyInfo(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("char id %d"), GetChar_id());
	CCharacter* member = FindNetObject<CCharacter>(memberId);

	String errMsg;
	do 
	{
		if (member == NULL)
		{
			errMsg = _T("cannot found target");
			break;
		}

		if (GetMap_id() != member->GetMap_id() ||
			GetMap() == NULL || member->GetMap() == NULL ||
			GetMap()->GetLine_id() != member->GetMap()->GetLine_id())
		{
			errMsg = _T("cannot get party info in different map");
			break;
		}

		CParty* pParty = member->GetParty();
		if (pParty == NULL || !pParty->GetAvail())
		{
			errMsg = _T("cannot found the target party");
			break;
		}
		if (pParty->GetCount() >= PARTY_MAX_MEMBER)
		{
			errMsg = _T("the target party is full");
			break;
		}
	} while (false);

	if (!errMsg.IsEmpty())
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("char_id %d, session_id %s,%s"), GetChar_id(),
			GetSessionId().GetString().c_str(), errMsg.c_str());
		return RPC_RESULT_FAIL;
	}

	PartyInfoVector PartyInfoList;
	CParty* pParty = member->GetParty();
	for (UInt16 i=0; i<pParty->GetCount(); i++)
	{
		member = pParty->GetMember(i);
		if (member)
		{
			UInt8 SexClass = (member->GetCclass() > 0 ? 
				((member->GetCclass() - 1) * MAX_SEXTYPE + member->GetSex() + 1) : 0);
			PartyInfo info(member->GetNickName(), member->GetFaction(), SexClass, member->GetLevel(),
				0, _T(""), RPCNetID(), member->GetChar_id());
			PartyInfoList.push_back(info);
		}
	}
	if (!PartyInfoList.empty())
	{
		CB_GetPartyInfo(pPeer, context, PartyInfoList);
	}

	return RPC_RESULT_OK;
}

void CCharacter::JoinInstanceChannle(UInt32 InstanceID)
{
	if (gGlobal.pMessageService)
	{
		String NewChannelName;

		NewChannelName.Format(_T("%s%s%d"), ChannelPrefix[CHANNELTYPE_INSTANCE], 
			Del,InstanceID);

		if (gGlobal.pMessageService->pPeer)
			MessageService_Proxy::JoinMessageChannel(gGlobal.pMessageService->pPeer, 
			GetContext(),GetChar_id(), 
			GetNickName(), NewChannelName, 
			CHANNEL_JOIN_MODE_CREATE_IF_NOT_EXIST);
	}
}
RPCResult CCharacter::LeaveTeamTemporarily(LPCPEER pPeer, RPCContext &context)
{
	/*String msg;
	msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_TEMP_LEAVE_SHUTDOWN")));
	CB_Error(pPeer, context, 1, msg);
	return RPC_RESULT_OK;*/
	UInt32 uRetCode = 3;
	if (!IsLeader()) 
	{
		if (NULL != m_pParty) 
		{
			if (m_pParty->SetTempLeave(GetChar_id()))
			{
				uRetCode = 0;
				m_pTemporaryParty = m_pParty;
				SetParty(NULL);
			}
			else
				uRetCode = 1;
		}
		else
			uRetCode = 2;
	}
	if (uRetCode != 0) 
		CB_LeaveTeamTemporarily(pPeer,context,uRetCode,GetChar_id());

	return RPC_RESULT_OK;
}
RPCResult CCharacter::ReturnParty(LPCPEER pPeer, RPCContext &context)
{
	UInt32 uRetCode = 1;
	if (NULL == GetParty() && NULL != GetTempParty()) 
		uRetCode = GetTempParty()->ReturnParty(this);
	if (uRetCode != 0) 
	{
		CB_ReturnParty(pPeer,context,uRetCode,NULL);
	}
	else
	{
		SetParty(m_pTemporaryParty,BOOLEAN_FALSE);
		SetTempParty();
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::BuyPointsShopItem(LPCPEER pPeer, RPCContext &context, const int &iItemID, const UInt32 &nPointsShopID, const UInt32 &nPointsShopTypeID)
{
	int iRet = 0;
	if (iItemID >= 0)
	{
		int iPoints  = 0;
		switch(nPointsShopTypeID)
		{
		case 1:
			iPoints = GetQuests()->GetFlag(nPointsShopID);
			break;
		case 2:
			iPoints = GetGoodness();
			break;
		case 3:
			iPoints = GetHonor();
			break;
		case 4:
			break;
		default:
			break;
		}
		const POINTSSHOPITEM* pPointShopItemData = gGlobal.m_resource->GetPointsShopData(iItemID);
		if (NULL != pPointShopItemData) {
			if ((int)pPointShopItemData->uiPointsCost <= iPoints) {
				CItemBag* pBag=GetItems();
				if (NULL != pBag) {
					CItem* pItem = pBag->NewItem(pPointShopItemData->uiItemId,1);
					if (NULL != pItem) {
						UInt8 i;
						for (i = BAG_CURRENT; i < BAG_TYPE_COUNT; i++)
						{
							if (pBag->AddItem(pItem, i))
								break;
						}
						if (i < BAG_TYPE_COUNT) {
							/*
							UInt32 uCurAcc = GetUiCurGenAcc();
							uCurAcc -= pPointShopItemData->uiAccmulationCost;
							SetUiCurGenAcc(uCurAcc);
							LogService_Proxy::LogGenerationAcc(gGlobal.pLogService->pPeer,
							context,GetChar_id(),0,pPointShopItemData->uiAccmulationCost,GetLevel(),0,pItem->GetItem_uid(),pPointShopItemData->uiItemId);
							CB_UpdateGenerationAcc(pPeer,context,GetUiCurGenAcc(),GetIHisGenAcc());*/

							switch(nPointsShopTypeID)
							{
							case 1:
								if (GetQuests())
								{
									if (GetQuests()->SetFlag(nPointsShopID, iPoints - pPointShopItemData->uiPointsCost))
									{
										GetQuests()->RaiseUpdate();
									}
								}
								break;
							case 2:
								ChangeGoodness(-(int)pPointShopItemData->uiPointsCost);
								break;
							case 3:
								ChangeHonor(-(int)pPointShopItemData->uiPointsCost);
								break;
							case 4:
								break;
							default:
								break;
							}

							if (IsUpdated())
								RaiseUpdate();
						}
						else{
							DeleteNetObject((NetObject*&)pItem);
							iRet = -5;
						}
					}
					else
						iRet = -4;
				}
				else
					iRet = -3;
			}
			else
				iRet = -1;
		}
		else
		{
			iRet = -2;
		}	
	}

	else
	{
		iRet = -6;
	}

	switch(nPointsShopTypeID)
	{
	case 1:
		CB_BuyPointsShopItem(pPeer,context,iRet, GetQuests()->GetFlag(nPointsShopID));
		break;
	case 2:
		CB_BuyPointsShopItem(pPeer,context,iRet, GetGoodness());
		break;
	case 3:
		CB_BuyPointsShopItem(pPeer,context,iRet, GetHonor());
		break;
	case 4:
		break;
	default:
		break;
	}

	return RPC_RESULT_OK;
}

void CCharacter::SetCurTwoPet(CPet* pPet)
{
	m_pUseTwoPet = pPet;
	if (pPet)
		SetManor(GetManor(),pPet->GetPet_uid(),pPet->GetPet_id(),pPet->GetName(),pPet->GetColor());
	else
		SetManor(GetManor(),0,0,String(),0);
}

void CCharacter::RemoveEncounterByNpcID(UInt16 quest_id, UInt32 uiNpcID )
{
	EncounterMap::iterator it = m_encounterMap.find(quest_id);
	if (it == m_encounterMap.end()) return;

	EncounterInfo &Info = it->second;
	if (Info.npc_id == uiNpcID)
	{
		m_encounterMap.erase(it);
	}
}


void CCharacter::ChangeToMob(UInt32 mob_id)
{
	ChangeModel(mob_id);
	ChangeModelSkill(mob_id);
}

void CCharacter::ChangeModel(UInt32 mob_id)
{
	m_changeMobSkillLst->RemoveAllSkill();
	SetModel_mobid(mob_id);
	if (GetPets()&& GetCurPet())
	{
		GetPets()->ChangePetState(GetPeer(),GetContext(),GetCurPet()->GetPet_uid(),false);
	}
	if (m_pUseTwoPet)
	{
		GetPets()->ChangePetState(GetPeer(),GetContext(),m_pUseTwoPet->GetPet_uid(),false);
	}
}

void CCharacter::ChangeModelSkill(UInt32 mob_id)
{
	SkillMap mobskillmap = GetSkillMapById(mob_id);
	if (mobskillmap.size() <= 0)
	{
		CB_SetChangeModelSkillGroup(GetPeer(),GetContext(),NULL);
		return;
	}

	for (SkillMap::iterator itr = mobskillmap.begin(); itr != mobskillmap.end(); itr++)
	{

		CSkill* mobSkill = itr->second;
		UInt16 mobSkillId = itr->first;
		mobSkill->PreSend();
		m_changeMobSkillLst->Add(mobSkill);
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("Character ChangeModel SkillID = %d"),
			mobSkillId);
	}
	CB_SetChangeModelSkillGroup(GetPeer(),GetContext(),m_changeMobSkillLst);
}

CBatActor::SkillMap CCharacter::GetSkillMapById(UInt32 mob_id)
{
	SkillMap mobSkillMap;
	mobSkillMap.clear();
	if (mob_id == 0)
		return mobSkillMap;

	const MobData* m_data = gGlobal.m_resource->GetMobData(mob_id);
	if (m_data == NULL)
		return mobSkillMap;

	StringA mobSkillFile;
	mobSkillFile.Format("MobSkill%05d.csv",m_data->skill_file_id); 
	const MobSkillCombination* mobSkillLst = gGlobal.m_resource->GetMobSkillCombination(mobSkillFile.c_str());


	if (mobSkillLst && m_data)
	{
		UInt existSpecMobSkill = 0;
		UInt existNormMobSkill = 0;
		UInt maxSpecMobSkill = 7;
		UInt maxNormMobSkill = 0;

		switch (m_data->talent)
		{
		case MOB_TALENT_A:	maxNormMobSkill = GetPrivateProfileInt(_T("LoadLearnSkill"), _T("TalentAMaxNormalSkill"), 7, CONFIGURATION_INI_FILE);	break;
		case MOB_TALENT_B:	maxNormMobSkill = GetPrivateProfileInt(_T("LoadLearnSkill"), _T("TalentBMaxNormalSkill"), 5, CONFIGURATION_INI_FILE);	break;
		case MOB_TALENT_C:	maxNormMobSkill = GetPrivateProfileInt(_T("LoadLearnSkill"), _T("TalentCMaxNormalSkill"), 3, CONFIGURATION_INI_FILE);	break;
		}

		UInt16Vector skillIDs;

		for (Index i = C_INDEX(0); i < mobSkillLst->skillLst.size(); ++i)
		{
			MobSkillDef* pDef = mobSkillLst->skillLst[i];
			if (pDef)
			{
				if ( (pDef->skillType == PARTNER_NORMAL_SKILL && existNormMobSkill < maxNormMobSkill) )
				{
					skillIDs.push_back(pDef->skillId);
					++existNormMobSkill;
				}
				else if (pDef->skillType == PARTNER_SPECIAL_SKILL && existSpecMobSkill < maxSpecMobSkill)
				{
					skillIDs.push_back(pDef->skillId);
					++existSpecMobSkill;
				}
			}
		}
		//SkillMap mobSkillMap;
		if (!skillIDs.empty()) {
			CSkillData skillData;
			//			SkillMap* pSkillMap = SafeCreateObject(SkillMap);

			for (UInt16 j = 0; j < skillIDs.size(); ++j)
			{
				CSkill* pSkill = CSkill::NewInstance();

				skillData.SetOwner_info(mob_id, 1); //0: char type 1: mob/partner type
				skillData.SetInfo(skillIDs[j]);
				skillData.SetLevel(1);
				skillData.SetCoolDown(1); //for temp
				pSkill->SetData(&skillData);
				mobSkillMap[pSkill->GetSkill_id()] = pSkill;
			}
			return mobSkillMap;
		}
	}
	return mobSkillMap;
}

//RPCResult CCharacter::CreateWeaponsPointData(LPCPEER pPeer, RPCContext &context,
//											const UInt32 & char_id, const UInt32 & point,const String & nickName,
//											const UInt32 & cclass,const UInt32 & faction,const UInt32 & level, 
//											const time_t &weaponspoint_time)
//{
//	OutputDebugString(L"CCharacter::CreateWeaponsPointData");
//	ObjectService_Proxy::CreateWeaponsPoint(gGlobal.pObjectService->pPeer, context,char_id,point,
//											nickName,cclass,faction,level,weaponspoint_time);
//	return RPC_RESULT_OK;
//}
//
//RPCResult CCharacter::GetWeaponsPointData(LPCPEER pPeer, RPCContext &context,const UInt32 &char_id)
//{
//	OutputDebugString(L"CCharacter::GetWeaponsPointData");
//	ObjectService_Proxy::GetWeaponsPoint(gGlobal.pObjectService->pPeer, context,char_id);
//	return RPC_RESULT_OK;
//}

