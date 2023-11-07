//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/CallLuaFunc.h"
#include "Common/Procedure.h"
#include "Common/StateCode.h"
#include "Common/BattleCode.h"
#include "Common/SettingTable.h"
//-- Self
#include "Stub/Logic/Battle.h"
//-- Library
#include "Stub/Logic/Character.h"
#include "Stub/Logic/Map.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/Partner.h"
#include "Stub/Logic/BatCharacter.h"
#include "Stub/Logic/BatMob.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/Item.h"
#include "Stub/Logic/Party.h"
#include "Stub/Logic/ScriptSystem.h"
#include "Stub/Logic/QuestSystem.h"
#include "Stub/Service/MapService_stub.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "Proxy/Service/MessageService_proxy.h"
#include "Proxy/Service/LogService.h"
#include "Proxy/Service/LogService_proxy.h"
#include "MapService/Global.h"
#include "MapService/Configure.h"
#include "MapService/MapApplication.h"
#include "MapService/antigameaddiction.h"
#include "Resource/ResNpc.h"
#include "Resource/ResMob.h"
#include "Resource/ResItem.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResMob.h"
#include "Resource/GameStringTable.h"
#include "MapService/AnimTimer.h"
#include <Reuben/Math/Math.h>
#include <algorithm>
#include "Resource/ResLineup.h"

#include "Resource/ResRandTable.h"

REGISTER_CLASSTYPE(CBattle, Battle_CLASSID);

#define DROP_ITEM_LIMIT		3
#define MAX_DROPITEM_PERCENTAGE	100000

#define NEXT_TURN_ADJUST		8000		// next turn adjust time
#define BATTLE_DEADLOCK_TIME	5000		// deadlock detection time
#define BUFFER_SEGMENT_SIZE		922			// buffer segment size (try to fit for 1k packet)
#define TIME_DIFF_DELTA			100

#define	PET_DISCERN_SKILL		7020		// if has this skill,frist turn use

#define BAT_ON_INIT				"OnInit"
#define BAT_ON_PRETURN			"OnPreTurn"
#define BAT_ON_POSTTURN			"OnPostTurn"
#define BAT_ON_FINISH			"OnFinish"
#define BAT_ON_ESCAPE			"OnEscape"

#define GUILD_WAR_REST_TIME		30000
const UInt16 KEYPOSEFFERTBUFFID = 54;
PROTO_LuaPush(CBatActor)
PROTO_LuaPush(CBatMob)
PROTO_LuaPush(CBatCharacter)
PROTO_LuaPush(CBattle)
PROTO_LuaPush(CCharacter)
PROTO_LuaPush(CPartner)

INLINE CBatActor* CBattle::GetActor(UInt8 index)
{
	if (index < (UInt8) m_actors.size())
		return FindNetObject<CBatActor>(m_actors[index]);
	return NULL;
}

INLINE UInt32 RoundToUInt32(float value)
{
	return (UInt32) Reuben::Math::GetRound<float>(value);
}

bool BatClient::IsConnect()
{ 
	return (pOwner && pOwner->GetPeer() != NULL);
}

LPCPEER BatClient::GetPeer()
{ 
	return (pOwner ? pOwner->GetPeer() : NULL);
}

RPCContext& BatClient::GetContext()
{
	static RPCContext dummyContext;

	return (pOwner ? pOwner->GetContext() : dummyContext);
}

//////////////////////////////////////
//			RPC methods				//
//////////////////////////////////////


//////////////////////////////////////
//		Init Battle methods			//
//////////////////////////////////////

CBattle::CBattle()
{
	m_state = NULL;
	m_pNpcData = NULL;
	m_npcId = 0;
	m_curTurn = 0;
	m_curStep = 0;
	m_winTeam = 255;
	m_forceEnd = BOOLEAN_FALSE;
	m_turnTime = 0;
	m_animTime = 0;
	m_totalMonitor = 0;
	m_pScriptSys = NULL;
	m_bNpcScript = BOOLEAN_FALSE;
	m_bPunish = BOOLEAN_TRUE;
	m_bJumpReborn = BOOLEAN_TRUE;
	m_talkNum = 0;
	m_nextState = ST_PRETURN;
	State::ChangeState(m_state, (State *) SafeCreateObject(CPrepareState));
	m_pScriptSys = SafeCreateObject(CScriptSystem);
	TRACE_ENSURE(m_pScriptSys);
	m_pScriptSys->SetBattle(this);
	m_pScriptSys->SetRunOnce(BOOLEAN_FALSE);
	ZeroMemory(m_cmdBlockMap, sizeof(m_cmdBlockMap));

	m_turnBuffer1 = SafeCreateObject(Buffer);		// buffer for play in turn for team 1
	m_turnBuffer2 = SafeCreateObject(Buffer);		// buffer for play in turn for tema 2
	m_turnBufferMon = SafeCreateObject(Buffer);

	m_bPvPScript = BOOLEAN_FALSE;
	m_pvpAddMobPosLst.clear();
}

CBattle::~CBattle()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("destroying.."));
	ClearContent();
	SafeDeleteObject(m_pScriptSys);
	State::ChangeState(m_state, NULL);
	TRACE_INFODTL(GLOBAL_LOGGER, _F("destroyed"));
	m_pvpAddMobPosLst.clear();

	try
	{
		SafeDeleteObject(m_turnBuffer1);
	}
	catch(...)
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER, _FE("m_turnBuffer1 delete error in battle %d:%d:%d"), GetID(), GetNetId().srvId, GetNetId().objId);
		TRACE_ERRORDTL_3(REUBEN_LOGGER, _FE("m_turnBuffer1 delete error in battle %d:%d:%d"), GetID(), GetNetId().srvId, GetNetId().objId);
	}

	try
	{
		SafeDeleteObject(m_turnBuffer2);
	}
	catch(...)
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER, _FE("m_turnBuffer2 delete error in battle %d:%d:%d"), GetID(), GetNetId().srvId, GetNetId().objId);
		TRACE_ERRORDTL_3(REUBEN_LOGGER, _FE("m_turnBuffer2 delete error in battle %d:%d:%d"), GetID(), GetNetId().srvId, GetNetId().objId);
	}

	try
	{
		SafeDeleteObject(m_turnBufferMon);
	}
	catch(...)
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER, _FE("m_turnBufferMon delete error in battle %d:%d:%d"), GetID(), GetNetId().srvId, GetNetId().objId);
		TRACE_ERRORDTL_3(REUBEN_LOGGER, _FE("m_turnBufferMon delete error in battle %d:%d:%d"), GetID(), GetNetId().srvId, GetNetId().objId);
	}

}

Boolean CBattle::Initialize(UInt8 battleType, UInt32 npcId, const RPCNetID &netId, 
							UInt8 partyCount, UInt16 averageLv, CCharacter * pLeader)
{
	SetBattleType(battleType);
	m_npcId = npcId;
	if (battleType == NPC_BATTLE && !CreateMob(npcId, partyCount, pLeader, averageLv))
		return BOOLEAN_FALSE;

	// try to init script system
	if (battleType == NPC_BATTLE && npcId != 0)
		InitScript(pLeader, netId);
	if (battleType == INCOGNITO_PK_BATTLE && npcId !=0 )
	{
		CCharacter* pPvPCharTarget = FindNetObject<CCharacter>(netId);
		if(pPvPCharTarget && pPvPCharTarget->GetPvPBattleScriptID() == pLeader->GetPvPBattleScriptID())
			InitPvPScript(pLeader,INCOGNITO_PK_BATTLE,pLeader->GetPvPBattleScriptID());
	}
	gGlobal.m_aliveBattles.push_back(this);		// add battle to alive list
	return BOOLEAN_TRUE;
}

CBatActor* CBattle::GetActorByPosition(UInt8 pos, UInt8 team)
{
	CBatActor* actor;
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it) 
	{
		actor = FindNetObject<CBatActor>(*it);
		if (actor && !actor->GetLeftBattle() && actor->GetPosition() == pos && actor->GetTeam() == team) 			
			return actor;
	}
	return NULL;
}
CBatActor* CBattle::GetActor(UInt8 targetType, UInt32 tid)
{
	CBatActor* actor;
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it) 
	{
		actor = FindNetObject<CBatActor>(*it);
		if (actor && !actor->GetLeftBattle()) 
		{
			if (targetType == CBatActor::TARGET_CHARACTER) 
			{
				if (actor->m_pBatChar && actor->m_pBatChar->GetChar_id() == tid)
					return actor;
			} 
			else if (targetType == CBatActor::TARGET_PARTNER) 
			{
				if (actor->m_pMob && actor->m_pMob->GetPartner_id() == tid)
					return actor;
			}
		}
	}

	return NULL;
}

Boolean CBattle::AddCharacter(CCharacter* pChar, UInt8 team)
{
	TRACE_INFODTL_6(GLOBAL_LOGGER, 
		_F("sessionId %s, char_id %d, battle %d:%d:%d, prepare state %d"), 
		pChar->GetSessionId().GetString().c_str(), pChar->GetChar_id(), 
		GetID(), GetNetId().srvId, GetNetId().objId, IsState(ST_PREPARE));

	if (pChar->IsState(CCharacter::ST_BATTLE) && pChar->m_BattleNetID != RPCNetID()) 
	{
		TRACE_WARNDTL_3(GLOBAL_LOGGER, 
			_T("CBattle AddCharacter Warning: char_id %d already inside battle %d:%d"), 
			pChar->GetChar_id(), pChar->m_BattleNetID.srvId, 
			pChar->m_BattleNetID);
		return BOOLEAN_FALSE;
	}

	if (IsState(ST_FINISH)) 
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CBattle AddCharacter Warning: battle already finished"));
		return BOOLEAN_FALSE;
	}

	CBatCharacter* pBatChar = CBatCharacter::NewInstance();
	BatClient *pClient = NewClient(pChar);

	pBatChar->SetBattle(this);
	pBatChar->SetTeam(team);
	pBatChar->SetData(pChar);
	pBatChar->m_pClient = pClient;
	pBatChar->m_pOwner = pChar;
	pChar->m_pBatChar = pBatChar;
	pBatChar->m_pBag = pChar->m_pBag;					// set item bag

	if(pBatChar->GetModel_mobid() > 0)
		pBatChar->SetSkillMap(pBatChar->GetOwner()->GetSkillMapById(pBatChar->GetModel_mobid()));
	else
		pBatChar->SetSkillMap(pChar->m_skillMap);

	pBatChar->UpdateSkillBuff();			// set character skills
	pChar->m_battleTeam = team;
	pChar->m_BattleNetID = GetNetId();
	SetOwnObject(pChar->GetContext().sessionId, this);
	SetOwnObject(pChar->GetContext().sessionId, pBatChar);

	pChar->m_pBag->PreBattle();							// bag prepare for battle
	pClient->team = team;
	pClient->pBatChar = pBatChar;

	if (pChar->m_autoCount > 0)							// previous auto count left
	{
		pClient->autoCount = pChar->m_autoCount;
		pClient->bAuto = BOOLEAN_TRUE;
	} 
	else if (pChar->GetPeer() == NULL) 
	{				// client disconnected
		pClient->autoCount = 65535;
		pClient->bAuto = BOOLEAN_TRUE;
	}
	
	if (pBatChar->IsLeader())
	{
		if (team == 0)
			m_team0Leader = pBatChar->GetNickName();
		else
			m_team1Leader = pBatChar->GetNickName();
	}
	if (pChar->GetMap())
		m_mapString = pChar->GetMap()->GetName();

	pClient->teamIndex = m_teamData[team].playerCount;
	m_teamData[team].playerCount ++;

	if (IsState(ST_PREPARE))
	{
		// tell map service that add character success
		pClient->status = CBattle::CLIENT_BATTLE;
		InsertActor(pBatChar);
	} 
	else 
	{
		// act as observer before characters is really appeared
		if (pClient->IsLeft())
			AddMonitor(pChar, team);
		else
			m_totalMonitor --;				// monitor before, reduce monitor count
		// add to battle waiting list
		AddWaiting(pBatChar);
		pClient->status = CBattle::CLIENT_WAITING;
	}

	//full hp sp when pvp
	if (GetBattleType() == MATCH_BATTLE ||
		GetBattleType() == GUILD_WAR_BATTLE ||
		GetBattleType() == INCOGNITO_PK_BATTLE)
	{
		pBatChar->SetHP(pBatChar->GetMaxHP());
		pBatChar->SetMHP(pBatChar->GetMaxHP());
		pBatChar->SetSP(pBatChar->GetMaxSP());
		pBatChar->SetMSP(pBatChar->GetMaxSP());
	}
	else if (GetBattleType() == LOW_MARTIAL_BATTLE)
	{
		pBatChar->SetHP(pBatChar->GetMaxHP());
		pBatChar->SetMHP(pBatChar->GetMaxHP());
		pBatChar->SetSP(pBatChar->GetMaxSP());
		pBatChar->SetMSP(pBatChar->GetMaxSP());
		pChar->ChangeMoney(- LOW_MARTIAL_MONEY);
	}
	else if (GetBattleType() == HIGH_MARTIAL_BATTLE)
	{
		pBatChar->SetHP(pBatChar->GetMaxHP());
		pBatChar->SetMHP(pBatChar->GetMaxHP());
		pBatChar->SetSP(pBatChar->GetMaxSP());
		pBatChar->SetMSP(pBatChar->GetMaxSP());
		pChar->ChangeMoney(- HIGH_MARTIAL_MONEY);
	}
	//DP random 80 ~ 100% when DP >= 100
	if (pBatChar->GetDP() >= 100)
		pBatChar->SetDP(pBatChar->GetDP() * (100 - gGlobal.m_rand.GetRandom(20)) / 100);

	if(pBatChar->GetModel_mobid() > 0)
	{
		String msg;
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_CHANGEMOB_NOT_USEPARTNRT")));
		pBatChar->GetOwner()->CB_PetUseSkillMsg(pBatChar->GetOwner()->GetPeer(), pBatChar->GetOwner()->GetContext(),  msg);	
		return BOOLEAN_TRUE;
	}
	// add partners
	for (UInt16 i = 0; i < pChar->m_pPartnerGroup->GetCount(); i++) 
	{
		CPartner* partner = pChar->m_pPartnerGroup->GetChild<CPartner>(i);

		if (partner == NULL || partner->m_data == NULL)
			continue;

		CBatMob* pBatMob = CBatMob::NewInstance();
		pBatMob->SetPartnerData(partner);
		pBatMob->SetBattle(this);
		pBatMob->SetTeam(team);
		pBatMob->SetIndex(255);
		pBatMob->ExtractBuffString();
		pBatMob->ExtractBuffCacheString();
		pBatMob->SetSkillMap(partner->m_skillMap);
		pBatMob->UpdateSkillBuff();			// set partner skills
		pBatMob->ClearUpdateMask();
		pBatMob->m_pBag = pChar->m_pBag;


		if (pBatChar->m_partnerGroup == NULL)
			pBatChar->m_partnerGroup = CreateNetGroup();
		pBatChar->m_partnerGroup->Add(pBatMob);
		AddStandby(pBatMob);
		SetOwnObject(pChar->GetContext().sessionId, pBatMob);

		//full hp sp when pvp
		if (GetBattleType() == MATCH_BATTLE ||
			GetBattleType() == LOW_MARTIAL_BATTLE ||
			GetBattleType() == HIGH_MARTIAL_BATTLE ||
			GetBattleType() == GUILD_WAR_BATTLE ||
			GetBattleType() == INCOGNITO_PK_BATTLE)
		{
			pBatMob->SetHP(pBatMob->GetMaxHP());
			pBatMob->SetMHP(pBatMob->GetMaxHP());
			pBatMob->SetSP(pBatMob->GetMaxSP());
			pBatMob->SetMSP(pBatMob->GetMaxSP());
		}

		if (partner->GetState() == CPartner::ST_FIGHT && pClient->pPartner == NULL)
		{
			gGlobal.g_TempResult = 0;
			CallLuaFunc(gGlobal.g_pLuaState, "PartnerJoinBattle", String(), partner->GetFavor());
			UInt suitableLv = pBatChar->GetLevel() + 5;

			if (gGlobal.g_TempResult > 0.5  && partner->GetHealth() > 50 && 
				partner->GetLevel() <=  suitableLv && 
				partner->m_data->req_lv <= pBatChar->GetLevel() )
			{
				pBatMob->m_pClient = pClient;
				pClient->pPartner = pBatMob;
				pBatMob->SetNew(true);
				if (IsState(ST_PREPARE)) 
				{
					// send new added mob at battle started
					pBatMob->SetPosition(pBatChar->GetPosition());
					InsertActor(pBatMob);
				} 
				else 
				{
					// add to battle waiting list
					AddWaiting(pBatMob);
				}
			} 
			else 
			{
				UInt8 code = 0;
				if (partner->GetHealth() <= 50)
					code = BATTLE_ERROR_MSG_NOT_HEALTH;
				else if (partner->GetLevel() > suitableLv || partner->m_data->req_lv > pBatChar->GetLevel() )
					code = BATTLE_ERROR_MSG_LEVEL_TOO_HIGH;
				else
					code = BATTLE_ERROR_MSG_NOT_FAVOR;
				partner->CB_FalseJoinBattle(pClient->GetPeer(), pClient->GetContext(), code);
				partner->ChangeState(CPartner::ST_STANDBY);
			}
		}
	}

	return BOOLEAN_TRUE;
}

Boolean CBattle::AddMonitor(CCharacter* pChar, UInt8 team)
{
	TRACE_ENSURE(pChar);

	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("battle %d:%d:%d, sessionId %s, char_id %d"), 
		GetID(), GetNetId().srvId, GetNetId().objId, pChar->GetSessionId().GetString().c_str(), 
		pChar->GetChar_id());

	if (pChar->HasParty())
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("CBattle AddMonitor Warning: [character in party], sessionId %s, char_id %d"),
			pChar->GetSessionId().GetString().c_str(), pChar->GetChar_id());
		return BOOLEAN_FALSE;
	}

	if (IsState(ST_PREPARE) || IsState(ST_FINISH))
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("CBattle AddMonitor Warning: [battle in prepare or finish state], sessionId %s, char_id %d"),
			pChar->GetSessionId().GetString().c_str(), pChar->GetChar_id());
		return BOOLEAN_FALSE;
	}

	BatClient* pClient = GetClient(pChar->GetSessionId());

	if (pClient && !pClient->IsLeft())
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("client already exist, sessionId %s, char_id %d"),
			pChar->GetSessionId().GetString().c_str(), pChar->GetChar_id());
		return BOOLEAN_FALSE;
	}

	pChar->Stop();									// stop moving before monitor
	if (!pChar->ChangeState(CCharacter::ST_MONITOR))
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("cannot change to monitor state, sessionId %s, char_id %d"),
			pChar->GetSessionId().GetString().c_str(), pChar->GetChar_id());
		return BOOLEAN_FALSE;
	}

	NetGroup grp;
	pClient = NewClient(pChar);
	pClient->team = team;
	pClient->status = CLIENT_MONITOR;
	pClient->pBatChar = NULL;
	pClient->pPartner = NULL;
	pChar->m_BattleNetID = GetNetId();
	pChar->m_battleTeam = team;
	SetOwnObject(pChar->GetContext().sessionId, this);
	m_totalMonitor ++;

	PrepareSendGroup(grp, 255);			// prepare NetGroup to send to client
	SetSendMask(AllMask);
	MapService_Stub::CB_EnterBattle(pChar->GetPeer(), pChar->GetContext(), this, 
		&grp, team, BATTLE_STATE_MONITOR, m_npcId);


	if (m_teamData[team].needHelp)
	{
		Boolean bCanNeedHelp = BOOLEAN_TRUE;
		if (m_teamData[team].pLeader != NULL) 
		{
			if (m_teamData[team].pLeader->GetParty() != NULL || m_teamData[team].pLeader->GetTempParty() != NULL) 
			{
				/*if (m_teamData[team].pLeader->GetParty()->GetCount() >= PARTY_MAX_MEMBER 
					&& ! m_teamData[team].pLeader->GetParty()->IsMember(pChar)) */
					bCanNeedHelp = BOOLEAN_FALSE;
				
			}
			/*else if(m_teamData[team].pLeader->GetTempParty() != NULL)
			{
				bCanNeedHelp = BOOLEAN_FALSE;
			}*/
		}
		if (bCanNeedHelp) 
			CB_NeedHelp(pChar->GetPeer(), pChar->GetContext(), BOOLEAN_TRUE);
	}

	return BOOLEAN_TRUE;
}

Boolean CBattle::PrepareDone()
{
	// change to wait client command state
	if (!IsState(ST_PREPARE))
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, 
			_T("CBattle PrepareDone Warning: not in prepare state, netId %d:%d"), 
			GetNetId().srvId, GetNetId().objId);
		return BOOLEAN_FALSE;
	}

	ShowAllDetail();
	m_curTurn = 0;
	m_talkNum = 0;

	if (m_bNpcScript || m_bPvPScript) 			// call battle init script
		m_pScriptSys->CallBatMethod(BAT_ON_INIT, 0);

	// tell all client to enter battle
	RPCMask charMask;

	charMask.MaskBits(CBatCharacter::DPMask, CBatCharacter::WCMask, CBatCharacter::attrib2Mask,CBatCharacter::attrib1Mask, -1);
	
	// prepare NetGroup to send
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		if (!client.IsBattle() || !client.IsConnect())
			continue;

		
		CBatCharacter *pBatChar = client.pBatChar;
		CBatMob *pPartner = client.pPartner;
		LPCPEER pPeer = client.pOwner->GetPeer();
		RPCContext &context = client.pOwner->GetContext();

		NetGroup grp;
		PrepareSendGroup(grp, pBatChar->GetTeam());
		pBatChar->MarkSend(charMask);
		SetSendMask(AllMask);
		MapService_Stub::CB_EnterBattle(pPeer, context, this, &grp, client.team,
			BATTLE_STATE_IN_WAR, m_npcId);

		pBatChar->UnmarkSend(charMask);
		pBatChar->InitPrevCmd();			// init previous command before controlling character
		CB_ControlCharacter(pPeer, context, pBatChar->GetIndex());
		if (pPartner)
		{
			pPartner->InitPrevCmd();		// init previous command before controlling partner
			CB_ControlPartner(pPeer, context, pBatChar->GetIndex(), pPartner->GetIndex());
			pPartner->SetNew(false);
		}

		//CB_NextTurn(pPeer, context, m_curTurn, CMD_TIMEOUT, client.autoCount);

		// resume auto in previous battle
		if (client.bAuto)
			pBatChar->SetAuto(&client);
		
		/// Anti-Hack Pop UP Mechanism ////
		// 1 out of 20 chance for Anti-Hack Panel popping up for each person 
		if (client.pOwner && client.pOwner->GetAntiHackEnable())
		{
			UInt32 PopUpChance = gConfigure.hackFreq; 
			//anti hack only when hack freq > 0 and character is not robot
			if (gConfigure.hackFreq != 0) //&& !client.pOwner->IsRobot())
			{
				if ((gGlobal.m_rand.GetRandom(PopUpChance) == 0 && (client.pOwner->GetNoAntiHackTime() < (UInt32) time(NULL)))||(client.pOwner->GetHackWrongAns() > 0)||(client.pOwner->m_bForcePopUp))
				{   //probability equation: 1/20 = (1/PopUpChance)
					client.pOwner->AntiHack();
					client.pOwner->m_bForcePopUp = BOOLEAN_FALSE;
				}
			}
		}
	}

	NextState(ST_PRETURN);		// next to pre-turn state
	RunTurn();

	return BOOLEAN_TRUE;
}

BatClient* CBattle::NewClient(CCharacter* pChar)
{
	BatClient *pClient = &m_clients[pChar->GetSessionId()];
	pClient->pOwner = pChar;

	return pClient;
}

void CBattle::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_CMD_TIMEOUT:		// timeout for client command input
		TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, 
			_T("CBattle HandleEvent EVT_CMD_TIMEOUT: battle %d:%d"),
			GetNetId().srvId, GetNetId().objId);
		RunTurn();
		break;

	case EVT_TALK_TIMEOUT:		// timeout for npc talking
		TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, 
			_T("CBattle HandleEvent EVT_TALK_TIMEOUT: battle %d:%d"),
			GetNetId().srvId, GetNetId().objId);
		if (m_pScriptSys && m_pScriptSys->IsChoosing())
			CancelTalk();
		RunTurn();
		break;

	case EVT_NEXT_TURN:			// next turn timeout for too short end turn
		TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("CBattle HandleEvent EVT_NEXT_TURN: battle %d:%d"), GetNetId().srvId, GetNetId().objId);
		RunTurn();
		break;

	case EVT_CHAR_ESCAPED:		// char escaped timeout for too short end turn
		for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
		{
			BatClient &client = it->second;
			if (client.IsBattle() && client.pBatChar->GetChar_id() == event.GetTargetID())
			{
				TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("CBattle HandleEvent EVT_CHAR_ESCAPED: battle %d:%d actor %d"), GetNetId().srvId, GetNetId().objId, event.GetTargetID());
				//CharEscaped(&client);		// should not escape here, should do EndTurn again
				EndTurn(client.GetPeer(), client.GetContext());
				break;
			}
		}
		break;
	}
}

void CBattle::ClearContent()
{
	CancelAllEvent();			// change all events
	ChangeState(ST_FINISH);		// change to finish state before clear

	TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("CBattle ClearContent id %d, netId %d:%d"), GetID(), GetNetId().srvId, GetNetId().objId);

	UInt counter;
	counter = 0;
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("actorsList size %d"), (UInt32)m_actors.size());
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		try
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("actor number %d"), counter);
			CBatActor * actor = FindNetObject<CBatActor>(*it);
			DeleteNetObject(actor);
		}
		catch(...)
		{
			TRACE_ERRORDTL(REUBEN_LOGGER, _FE("exception caught on m_actors DeleteNetObject"));
			TRACE_ERRORDTL(GLOBAL_LOGGER, _FE("exception caught on m_actors DeleteNetObject"));
		}
		++counter;
	}
	m_actors.clear();

	counter = 0;
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("waitList size %d"), (UInt32)m_waitlist.size());
	for (Actors::iterator it = m_waitlist.begin(); it != m_waitlist.end(); ++it)
	{
		try
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("wait actor number %d"), counter);
			CBatActor * actor = FindNetObject<CBatActor>(*it);
			DeleteNetObject(actor);
		}
		catch(...)
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _FE("exception caught on m_waitlist DeleteNetObject"));
			TRACE_ERRORDTL(REUBEN_LOGGER, _FE("exception caught on m_waitlist DeleteNetObject"));
		}
		++counter;
	}
	m_waitlist.clear();

	counter = 0;
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("standbyList size %d"), (UInt32)m_standbyList.size());
	for (Actors::iterator it = m_standbyList.begin(); it != m_standbyList.end(); ++it)
	{
		try
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("standby actor number %d"), counter);
			CBatActor * actor = FindNetObject<CBatActor>(*it);
			DeleteNetObject(actor);
		}
		catch(...)
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _FE("exception caught on m_standbyList DeleteNetObject"));
			TRACE_ERRORDTL(REUBEN_LOGGER, _FE("exception caught on m_standbyList DeleteNetObject"));
		}
		++counter;
	}
	m_standbyList.clear();
	m_clients.clear();
	TRACE_INFODTL(GLOBAL_LOGGER, _F("end"));
}

void CBattle::InsertActor(CBatActor *actor)
{
	if (actor == NULL) return;
	if (std::find(m_actors.begin(), m_actors.end(), actor->GetNetId()) != m_actors.end())
		return;

	// remove from standby list if exist
	Actors::iterator it = std::find(m_standbyList.begin(), m_standbyList.end(), actor->GetNetId());
	if (it != m_standbyList.end())
		m_standbyList.erase(it);

	// find available position, according following coordinations
	//    server position      client position
	//    ---------------      ---------------
	//    13 11 10 12 14       18 16 15 17 19
	//     8  6  5  7  9   >>  13 11 10 12 14
	//     3  1  0  2  4        8  6  5  7  9
	//    18 16 15 17 19        3  1  0  2  4

	UInt8 index = (UInt8) m_actors.size();
	UInt8 pos = actor->GetPosition();
	UInt8 team = actor->GetTeam();
	CBatActor* actor2;
	if (pos < TEAMPOS_MAX)
	{	
		// try specified position available
		actor2 = GetActor(m_teamData[team].posIndex[pos]);
		if (actor2 && !actor2->GetLeftBattle())
		{
			UInt8 stpos = ((pos / TEAMMATE_MAX) * TEAMMATE_MAX);
			pos = 255;
			// find minimum position available at specified row
			// specific for normal mob
			if (actor->IsMob() && !actor->IsParnter())
			{
				for (UInt8 i = stpos; i < stpos + TEAMPOS_MIN; i++)
				{
					actor2 = GetActor(m_teamData[team].posIndex[i % (TEAMPOS_MIN)]);
					if (actor2 == NULL || actor2->GetLeftBattle()) 
					{
						pos = i % (TEAMPOS_MIN);
						break;
					}
				}
			}
			else
			{
				for (UInt8 i = stpos; i < stpos + TEAMMATE_MAX; i++)
				{
					actor2 = GetActor(m_teamData[team].posIndex[i]);
					if (actor2 == NULL || actor2->GetLeftBattle()) 
					{
						pos = i;
						break;
					}
				}
			}
		}
	}
	else
	{
		// find minimum position available
		for (UInt8 i = 0; i < TEAMPOS_MIN; i++)
		{
			actor2 = GetActor(m_teamData[team].posIndex[i]);
			if (actor2 == NULL || actor2->GetLeftBattle()) 
			{
				pos = i;
				break;
			}
		}
	}

	TRACE_INFODTL_8(GLOBAL_LOGGER, _F("position: %d actor index %d actor %d:%d:%d , battle %d:%d:%d"),
		pos, actor->GetIndex(), actor->GetID(), actor->GetNetId().srvId, actor->GetNetId().objId,
		GetID(), GetNetId().srvId, GetNetId().objId);

	//if (pos < TEAMPOS_MAX) 
	{			// have available position
		m_teamData[team].posIndex[pos % TEAMPOS_MAX] = index;
		actor->SetPosition((pos + TEAMMATE_MAX) % TEAMPOS_MAX);		// calc actual position
		actor->SetIndex(index);
		actor->SetLeftBattle(BOOLEAN_FALSE);
		m_actors.push_back(actor->GetNetId());
	} 
	if (pos >= TEAMPOS_MAX) 
	{							// haven't available position
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("Position not found pos: %d obj id: %d battle obj id: %d"),
			pos, actor->GetNetId().objId, GetNetId().objId);
	}

	// add to mob id list exclude player partner
	if (actor->m_pMob && !actor->IsParnter())
		m_mobIndices.push_back(actor->m_pMob->GetMob_id());

	actor->PreSend();					// prepare to send
	actor->TurnClear();					// turn clear
}

UInt8 CBattle::GetCharacterCount(UInt8 team)
{
	UInt8 count = 0;
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		CBatCharacter *pBatChar = it->second.pBatChar;
		if (pBatChar &&	!pBatChar->GetLeftBattle() && pBatChar->GetTeam() == team)
			count ++;
	}

	return count;
}

void CBattle::AddStandby(CBatActor *actor)
{
	m_standbyList.push_back(actor->GetNetId());
}

void CBattle::AddWaiting(CBatActor* actor)
{
	// remove from standby list if exist
	Actors::iterator it = std::find(m_standbyList.begin(), m_standbyList.end(), actor->GetNetId());
	if (it != m_standbyList.end())
		m_standbyList.erase(it);

	m_waitlist.push_back(actor->GetNetId());
}

void CBattle::RemoveWaiting(CBatActor* actor)
{
	// remove from standby list if exist
	Actors::iterator it = std::find(m_waitlist.begin(), m_waitlist.end(), actor->GetNetId());
	if (it != m_waitlist.end())
		m_waitlist.erase(it);
	m_standbyList.push_back(actor->GetNetId());
}

void CBattle::RemoveWaiting(BatClient* pClient)
{
	if (pClient == NULL || !pClient->IsWaiting() || pClient->pOwner == NULL)
		return;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("sessionId %s, status %d"), 
		pClient->pOwner->GetContext().sessionId.GetString().c_str(), pClient->status);

	pClient->status = CLIENT_LEFT;
	pClient->pOwner->ChangeState(CCharacter::ST_NOBATTLE);
	pClient->pOwner->m_BattleNetID = RPCNetID();
	pClient->pOwner->m_pBatChar = NULL;
	if (pClient->IsConnect())
		CB_EndBattle(pClient->pOwner->GetPeer(), pClient->pOwner->GetContext(), END_MONITOR);
	if (pClient->pBatChar)
		RemoveWaiting(pClient->pBatChar);
	if (pClient->pPartner)
		RemoveWaiting(pClient->pPartner);
	if (pClient->pOwner && pClient->pOwner->CheckNeedJumpReborn())
	{
		pClient->pOwner->JumpReborn();
		CB_Error(pClient->pOwner->GetPeer(), pClient->pOwner->GetContext(), 0, _T("MSG_MAP_FORCE_LEAVE"));
	}
}

CBatCharacter* CBattle::FindCharacter(UInt32 char_id)
{
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		CBatCharacter *pBatChar = it->second.pBatChar;
		if (pBatChar && pBatChar->GetChar_id() == char_id)
			return pBatChar;
	}

	return NULL;
}

CBatMob* CBattle::FindPartner(UInt32 mob_id)
{
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		CBatMob *pMob = it->second.pPartner;
		if (pMob && pMob->GetMob_id() == mob_id)
			return pMob;
	}

	return NULL;
}

Boolean CBattle::CreateMob(UInt32 npcId, UInt8 partyCount, CCharacter* pLeader, UInt16 averageLv)
{
	// add mob id list from npc information
	m_pNpcData = gGlobal.m_resource->GetNpcData(npcId);

	if (m_pNpcData == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CreateMob Error: NPC data not found by npcId %d"), npcId);
		return BOOLEAN_FALSE;
	}
	else if (m_pNpcData->mobLackey.empty())
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CreateMob Error: No mob exist in NPC data of npcId %d"), npcId);
		return BOOLEAN_FALSE;
	}

	UInt nBoss = 0;
	UInt randno;
	UInt tempaccumulator;
	NPCMobInfo mob_data;
	CBatMob* pMob;

	// random leader mob
	if (!m_pNpcData->mobLeader.empty())
	{
		UInt encounttotal = 0;
		for( Index i = C_INDEX(0); i < m_pNpcData->mobLeader.size(); ++i )
			encounttotal += m_pNpcData->mobLeader[i].Encount;

		int rLeader = -1;
		if( encounttotal > 0 )
		{
			tempaccumulator = 0;
			//randno = GetRand((int)encounttotal);
			randno = GetRand((int)encounttotal);
			for( Index i = C_INDEX(0); i < m_pNpcData->mobLeader.size(); ++i )
			{
				tempaccumulator += m_pNpcData->mobLeader[i].Encount;
				if( randno < tempaccumulator )
				{
					rLeader = i;
					break;
				}
			}
			if( rLeader < 0 )
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("rLeader < 0.  Default random is used temporarily.  Npc ID = %i"), npcId);
				rLeader = GetRand((int)m_pNpcData->mobLeader.size());
			}
		}
		else
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("Err: CreateMob, mob Leader Total Encounter = 0, where it cannot be zero.  Default random is used temporarily.  Npc ID = %i"), npcId);
			rLeader = GetRand((int)m_pNpcData->mobLeader.size());
		}
		mob_data = m_pNpcData->mobLeader[rLeader];
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Add Leader Mob: %d"), mob_data.MobId);
		pMob = CBatMob::NewInstance();
		pMob->SetBattle(this);
		{
			const MobData *pMobInfo = gGlobal.m_resource->GetMobData(mob_data.MobId);
			if ( pMobInfo && pMobInfo->mob_type == 1 )
				pMob->SetData(mob_data.MobId, averageLv);
			else
				pMob->SetData(mob_data.MobId);
		}
		/// check if it will be appended with a post name
		pMob->SetTeam(ENEMY_TEAM);
		pMob->MobLearnSkill();
		InsertActor(pMob);						// added to actors list
		nBoss = 1;
	}

	gGlobal.g_TempResult = 1;
	CallLuaFunc(gGlobal.g_pLuaState, "CalMobCount", String(), pLeader, 
		m_pNpcData->minLackey, m_pNpcData->maxLackey, partyCount);
	int nMob = (int)gGlobal.g_TempResult;
	UInt encounttotal = 0;
	for( Index i = C_INDEX(0); i < m_pNpcData->mobLackey.size(); ++i )
		encounttotal += m_pNpcData->mobLackey[i].Encount;

	if (nMob + nBoss > 10)
		nMob = 10 - nBoss;

	int rLackey = -1;
	for (int i = 0; i < nMob; i++)
	{
		// random for first time for single kind mob or each time for multiple kind mobs
		if (rLackey == -1 || m_pNpcData->squad != 0)
		{
			if( encounttotal > 0 )
			{
				tempaccumulator = 0;
				randno = GetRand((int)encounttotal);
				for( Index j = C_INDEX(0); j < m_pNpcData->mobLackey.size(); ++j )
				{
					tempaccumulator += m_pNpcData->mobLackey[j].Encount;
					if( randno < tempaccumulator )
					{
						rLackey = j;
						break;
					}
				}
				if( rLackey < 0 )
				{
					TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("rLackey < 0.  Default random is used temporarily.  Npc ID = %i"), npcId);
					rLackey = GetRand((int)m_pNpcData->mobLackey.size());
				}
			}
			else
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("Err: CreateMob, mob Lackey Total Encounter = 0, where it cannot be zero.  Default random is used temporarily.  Npc ID = %i"), npcId);
				rLackey = GetRand((int)m_pNpcData->mobLackey.size());
			}
		}
		mob_data = m_pNpcData->mobLackey[rLackey];
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Add Lackey Mob: %d"), mob_data.MobId);
		pMob = CBatMob::NewInstance();
		pMob->SetBattle(this);
		{
			const MobData *pMobInfo = gGlobal.m_resource->GetMobData(mob_data.MobId);
			if ( pMobInfo && pMobInfo->mob_type == 1 )
				pMob->SetData(mob_data.MobId, averageLv);
			else
				pMob->SetData(mob_data.MobId);
		}
		pMob->SetTeam(ENEMY_TEAM);
		pMob->MobLearnSkill();
		InsertActor(pMob);						// added to actors list
	}
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("Battle netId %d:%d: Created %d Boss and %d Mobs for Party Count %d"),
		GetNetId().srvId, GetNetId().objId, nBoss, nMob, partyCount);

	return BOOLEAN_TRUE;
}

RPCResult CBattle::EndTurn(LPCPEER pPeer, RPCContext &context)
{
	if (!IsState(ST_PLAYACTION))
		return RPC_RESULT_FAIL;

	TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, _F("battle %d:%d:%d, sessionId %s"), GetID(), GetNetId().srvId, GetNetId().objId, context.sessionId.GetString().c_str());

	// mark client end turn
	BatClients::iterator cit = m_clients.find(context.sessionId);
	if (cit != m_clients.end() && cit->second.IsBattle())
	{
		BatClient &client = cit->second;
		if (client.bEndTurn) // don't allow double end turn for same client
			return RPC_RESULT_OK;

		client.bEndTurn = BOOLEAN_TRUE;
		// move character and its partner to backup state when escaped
		if (client.pBatChar->IsEscaped())
		{
			UInt32 playedTime = ::timeGetTime() - m_turnTime;
			if (playedTime + TIME_DIFF_DELTA >= client.pBatChar->m_escapeWait)
			{
				CharEscaped(&client);
			}
			else
			{
				// delay character escaped battle if the client end turn too fast
				Event evt(EVT_CHAR_ESCAPED, this->GetID(), client.pBatChar->GetChar_id());
				TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, _F("CBattle EndTurn (Escape Timeout) char_id %d, battle %d:%d, after %d"),
					client.pBatChar->GetChar_id(), GetNetId().srvId, GetNetId().objId, client.pBatChar->m_escapeWait - playedTime);
				client.bEndTurn = BOOLEAN_FALSE;		// act as not end turn
				Reuben::Simulation::RaiseUniqEvent(evt, client.pBatChar->m_escapeWait -	playedTime);
				return RPC_RESULT_OK;
			}
		}
	}

	// wait for all clients end turn
	Boolean bAllEnd = BOOLEAN_TRUE;
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		if (client.IsBattle() && client.IsConnect() && client.bEndTurn == BOOLEAN_FALSE)
		{
			TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("CBattle EndTurn (Set AllEnd to False): battle %d:%d:%d"), GetID(), GetNetId().srvId, GetNetId().objId);
			bAllEnd = BOOLEAN_FALSE;
			break;
		}
	}

	// process next turn if all client has played animation
	if (bAllEnd)
	{
		UInt32 playedTime = ::timeGetTime() - m_turnTime;
		if (playedTime + TIME_DIFF_DELTA >= m_animTime)
		{
			TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, _F("CBattle EndTurn (Call NextTurn): battle %d:%d:%d timeDiff: %d"), GetID(), GetNetId().srvId, GetNetId().objId, playedTime - m_animTime);
			RunTurn();
		}
		else
		{
			// delay next turn time if all clients end turn too fast
			TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("CBattle EndTurn (Timeout At): battle %d:%d, after %d"),
				GetNetId().srvId, GetNetId().objId, m_animTime - playedTime);
			//CancelUniqEvent(EVT_NEXT_TURN);
			RaiseUniqEventID(EVT_NEXT_TURN, m_animTime - playedTime);
		}
	}
	else
	{
		TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("CBattle EndTurn (Not AllEnd): battle %d:%d:%d"), 
			GetID(), GetNetId().srvId, GetNetId().objId);
	}

	return RPC_RESULT_OK;
}

RPCResult CBattle::NextTalk(LPCPEER pPeer, RPCContext &context)
{
	if (m_pScriptSys == NULL || m_talkNum == 0||
		m_teamData[PLAYER_TEAM].pLeader == NULL ||
		m_teamData[PLAYER_TEAM].pLeader->GetSessionId() != context.sessionId)
		return RPC_RESULT_FAIL;

	if (m_talkNum > 0)
		m_talkNum --;
	if (m_talkNum > 0) {
		WaitTalk();
	} else {
		CancelUniqEvent(EVT_TALK_TIMEOUT);
		RunTurn();
	}

	return RPC_RESULT_OK;
}

void CBattle::SetBatEvent(UInt8 eventType)
{
	m_batEvents[m_curTurn] = eventType;
	CheckBatEvent();
}

void CBattle::SetBatEvent(UInt8 turn, UInt8 eventType)
{
	// for compatible reason, turn = turn - 1
	m_batEvents[turn + 1] = eventType;
	CheckBatEvent();
}

void CBattle::DisableCmd(UInt8 cmd)
{
	if (cmd < MAX_ACTION_CMD + 1)
	{
		if (cmd == MAX_ACTION_CMD) {
			for (UInt8 i = 1; i < MAX_ACTION_CMD; i++)
				m_cmdBlockMap[i] |= 0x02;
			
		} else if (cmd != 0) {
			m_cmdBlockMap[cmd] |= 0x02;
		}
	}

	UInt32 result = 0;
	for (UInt8 i = 0; i < MAX_ACTION_CMD; i++)
	{
		if (m_cmdBlockMap[i] > 0)
			result += 1 << i;
	}

	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		// skip left character
		if (client.IsLeft() || !client.IsConnect() || client.pOwner == NULL)
			continue;

		CB_DisableCmd(client.pOwner->GetPeer(), client.pOwner->GetContext(), result);
	}

	
}

void CBattle::CheckBatEvent()
{
	BatEvents::iterator it = m_batEvents.find(m_curTurn);
	if (it == m_batEvents.end())
		return;

	switch (it->second) {
	case BSEVENT_SURRENDER:
		Surrender(ENEMY_TEAM);
		break;

	case BSEVENT_ENDBATTLE:
		m_forceEnd = BOOLEAN_TRUE;
		break;

	case BSEVENT_SKIPTURN:
		break;
	};

	m_batEvents.erase(it);
}

void CBattle::CheckTeamStatus()
{
	// reset exist & kill count
	m_teamData[ENEMY_TEAM].existCount = 0;
	m_teamData[PLAYER_TEAM].existCount = 0;
	m_teamData[ENEMY_TEAM].killCount = 0;
	m_teamData[PLAYER_TEAM].killCount = 0;

	// check battle win one team all dead or escaped
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor *actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL) 
			continue;
		UInt8 myTeam = actor->GetTeam();
		UInt8 otherTeam = 1 - myTeam;
		if ((actor->IsValidTarget() || (actor->m_pMob && actor->m_pMob->IsNew())))
		{
			if (GetBattleType() == NPC_BATTLE)
			{
				// [npc battle] player team: player & partner, enemy team: mob
				if (myTeam == PLAYER_TEAM && (actor->IsPlayer() || actor->IsParnter())
					|| myTeam == ENEMY_TEAM && actor->IsMob())
				m_teamData[myTeam].existCount++;
			}
			else
			{
				// [pk battle] player team: player & partner, enemy team: player & partner
				if (actor->IsPlayer() || actor->IsParnter())
					m_teamData[myTeam].existCount++;
			}
		}
		else if (actor->IsDead() && !actor->IsGuest())
			// count dead player, partner or mob as kill count except guest npc
			m_teamData[otherTeam].killCount++;
	}

	if (m_teamData[ENEMY_TEAM].existCount == 0 && 				// player team win
		m_teamData[PLAYER_TEAM].existCount > 0)
		m_winTeam = PLAYER_TEAM;
	else if (m_teamData[PLAYER_TEAM].existCount == 0 && 		// enemy team win
		m_teamData[ENEMY_TEAM].existCount > 0)
		m_winTeam = ENEMY_TEAM;
}

void CBattle::Surrender(UInt8 team)
{
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor *actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL) 
			continue;
		if (actor->GetLeftBattle())
			continue;
		if (actor->GetTeam() == team) {
			actor->SetState(CHAR_ST_SURRENDER);
			actor->m_command = BATCMD_ESCAPE;
			//actor->m_bInput = BOOLEAN_TRUE;
			actor->SetIsInput(BOOLEAN_TRUE);
		} else {
			actor->m_command = BATCMD_IDLE;
			//actor->m_bInput = BOOLEAN_TRUE;
			actor->SetIsInput(BOOLEAN_TRUE);
		}
	}
	m_winTeam = 1 - team;
	NextState(ST_PLAYACTION);
}

void CBattle::SetForceEnd(Boolean bEnd)
{
	m_forceEnd = bEnd;
	if (!bEnd)
	{
		m_winTeam = 255;

		m_turnBuffer1->ResetCursor();
		m_turnBuffer1->SetSize(m_turnBuffer1->GetCursor());
		m_turnBuffer2->ResetCursor();
		m_turnBuffer2->SetSize(m_turnBuffer2->GetCursor());
		m_turnBufferMon->ResetCursor();
		m_turnBufferMon->SetSize(m_turnBufferMon->GetCursor());
		m_turnTime = ::timeGetTime();			// play turn start time
		m_animTime = 0;

		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
		{
			CBatActor *actor = FindNetObject<CBatActor>(*it);
			if(actor == NULL)
				continue;
			if (actor->GetLeftBattle() || actor->IsParnter() && actor->m_pMob->IsNew())
				continue;
			actor->UpdateBuff(0);
			actor->PackBuff();
		}
		Boolean noWait = BOOLEAN_TRUE;
		ByteArray array(BUFFER_SEGMENT_SIZE);
		UInt32 totalSize = 0;
		CONST Byte* pData = NULL;
		UInt32 arraySize;

		for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
		{
			BatClient &client = it->second;
			if (client.IsLeft() || !client.IsConnect())	// skip left character
				continue;

			if (it->second.IsMonitor())
			{
				totalSize = (UInt32) m_turnBufferMon->GetSize() - REUBEN_NETWORK_BUFFER_HEADER_SIZE;
				pData = m_turnBufferMon->GetData() + REUBEN_NETWORK_BUFFER_HEADER_SIZE;
			}
			else
			{
				if (it->second.pBatChar == NULL)
					continue;
				if (it->second.pBatChar->GetTeam() == ENEMY_TEAM)
				{
					totalSize = (UInt32) m_turnBuffer1->GetSize() - REUBEN_NETWORK_BUFFER_HEADER_SIZE;
					pData = m_turnBuffer1->GetData() + REUBEN_NETWORK_BUFFER_HEADER_SIZE;
				}
				else
				{
					totalSize = (UInt32) m_turnBuffer2->GetSize() - REUBEN_NETWORK_BUFFER_HEADER_SIZE;
					pData = m_turnBuffer2->GetData() + REUBEN_NETWORK_BUFFER_HEADER_SIZE;
				}
			}

			for (UInt32 index = 0; index < totalSize; index += BUFFER_SEGMENT_SIZE)
			{
				client.bEndTurn = BOOLEAN_FALSE;		// reset end turn flag
				if (noWait)
					noWait = BOOLEAN_FALSE;				// need to wait

				if (m_friendGroup.GetCount() > 0) {		// send all coming friends
					CB_AddPartners(client.pOwner->GetPeer(), client.pOwner->GetContext(), 
						&m_friendGroup, client.team);
				}
				// send to all client to start play
				if (index + BUFFER_SEGMENT_SIZE < totalSize)
					arraySize = BUFFER_SEGMENT_SIZE;
				else
					arraySize = totalSize - index;
				array.Clone(pData, arraySize);

				if (index == 0) {		// first segment
					CB_PlayTurn(client.pOwner->GetPeer(), client.pOwner->GetContext(), m_curTurn,
						array, totalSize);
				} else {				// more segment
					CB_PlayTurnMore(client.pOwner->GetPeer(), client.pOwner->GetContext(), array,
						totalSize);
				}
				if (index + BUFFER_SEGMENT_SIZE < totalSize)
					pData += BUFFER_SEGMENT_SIZE;
			}
		}
	}
}

void CBattle::RunTurn()
{
	Boolean bNext = BOOLEAN_TRUE;

	while (bNext) {
		switch (m_nextState) {
		// 1. "Pre-Turn" phase
		case ST_PRETURN:
			if (m_forceEnd) {
				NextState(ST_FINISH);
				break;
			}
			bNext = PreTurn();
			break;

		// 2. "Wait Command" phase
		case ST_WAITCMD:
			if (m_forceEnd) {
				NextState(ST_POSTTURN);
				break;
			}
			bNext = WaitCmd();
			break;

		// 3. "Play Action" phase
		case ST_PLAYACTION:
			if (m_forceEnd) {
				NextState(ST_POSTTURN);
				break;
			}
			bNext = PlayAction();
			break;

		// 4. "Post-Turn" phase
		case ST_POSTTURN:
			bNext = PostTurn();
			break;

		// 5. "Finish Battle" phase
		case ST_FINISH:
			FinishBattle();
			bNext = BOOLEAN_FALSE;
			break;

		default:
			bNext = BOOLEAN_FALSE;
		};
	}
}

Boolean CBattle::PreTurn()
{
	m_curTurn++;				// set to next turn
	m_animTime = 0;
	m_talkNum = 0;

	TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("CBattle PreTurn: battle %d:%d:%d, current turn %d, pstate %d"),
		GetID(), GetNetId().srvId, GetNetId().objId, m_curTurn, m_state->GetStateID());
	ChangeState(ST_PRETURN);	// change to pre-turn state
	NextState(ST_WAITCMD);	// next to wait command phase

	if(m_curTurn == 1)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _T("CBattle PreTurn m_actors.size = %d "), m_actors.size());
		Actors teamOneHasPetVec,teamOneAllVec,teamTowHasPetVec,teamTowAllVec;
		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
		{
			CBatActor *actor = FindNetObject<CBatActor>(*it);
			if(actor == NULL) 
				continue;
			if (actor->GetTeam() == 0)
			{
				teamOneAllVec.push_back(actor->GetNetId());
				if (actor->GetUsingPet() && actor->GetUsingPet()->HasSkill(PET_DISCERN_SKILL))
				{
					teamOneHasPetVec.push_back(actor->GetNetId());
				}
			}
			else if(actor->GetTeam() == 1)
			{
				teamTowAllVec.push_back(actor->GetNetId());
				if (actor->GetUsingPet() && actor->GetUsingPet()->HasSkill(PET_DISCERN_SKILL))
				{
					teamTowHasPetVec.push_back(actor->GetNetId());
				}
			}
			else
			{
				TRACE_INFODTL_1(GLOBAL_LOGGER, _T("CBattle PreTurn m_actors team is = %d "), actor->GetTeam());
			}
		}
		if (teamOneHasPetVec.size() > 0 && teamTowAllVec.size() > 0)
		{
			for (int i = 0; i < teamOneHasPetVec.size(); i++)
			{
				CBatActor *actor = FindNetObject<CBatActor>(teamOneHasPetVec[i]);
				if(actor == NULL) 
					continue;
				UInt8 TargetIndex = gGlobal.m_rand.GetRandom(teamTowAllVec.size());
				TRACE_INFODTL_2(GLOBAL_LOGGER, _T("CBattle PreTurn teamTowAllVec size = %d,random index = %d "),teamTowAllVec.size(),TargetIndex);
				CBatActor *targetActor = FindNetObject<CBatActor>(teamTowAllVec[TargetIndex]);
				if (targetActor == NULL)
					continue;
				String SkillMsg;
				if(targetActor->m_pMob)
				{
					SkillMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_USE_PETSKILL")),targetActor->m_pMob->m_data->mid_name,targetActor->GetHP(),targetActor->GetMHP(),
						targetActor->GetSP(),targetActor->GetMSP(),targetActor->GetDP());
				}
				else if (targetActor->GetOwner())
				{
					SkillMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_USE_PETSKILL")),targetActor->GetOwner()->GetNickName(),targetActor->GetHP(),targetActor->GetMHP(),
						targetActor->GetSP(),targetActor->GetMSP(),targetActor->GetDP());
				}
				//TRACE_INFODTL_1(GLOBAL_LOGGER, _T("CBattle PreTurn m_actors use skill is = %s "), SkillMsg.c_str());
				actor->GetOwner()->CB_PetUseSkillMsg(actor->GetOwner()->GetPeer(),actor->GetOwner()->GetContext(),SkillMsg);
			}
		}
		if (teamTowHasPetVec.size() > 0 && teamOneAllVec.size() > 0)
		{
			for (int i = 0; i < teamTowHasPetVec.size(); i++)
			{
				CBatActor *actor = FindNetObject<CBatActor>(teamTowHasPetVec[i]);
				if(actor == NULL) 
					continue;
				UInt8 TargetIndex = gGlobal.m_rand.GetRandom(teamOneAllVec.size());
				TRACE_INFODTL_2(GLOBAL_LOGGER, _T("CBattle PreTurn teamOneAllVec size = %d,random index = %d "),teamOneAllVec.size(),TargetIndex);
				CBatActor *targetActor = FindNetObject<CBatActor>(teamOneAllVec[TargetIndex]);
				if(targetActor == NULL)
					continue;
				String SkillMsg;
				if(targetActor->m_pMob)
				{
					SkillMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_USE_PETSKILL")),targetActor->m_pMob->m_data->mid_name,targetActor->GetHP(),targetActor->GetMHP(),
						targetActor->GetSP(),targetActor->GetMSP(),targetActor->GetDP());
				}
				else if (targetActor->GetOwner())
				{
					SkillMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_USE_PETSKILL")),targetActor->GetOwner()->GetNickName(),targetActor->GetHP(),targetActor->GetMHP(),
						targetActor->GetSP(),targetActor->GetMSP(),targetActor->GetDP());
				}
				actor->GetOwner()->CB_PetUseSkillMsg(actor->GetOwner()->GetPeer(),actor->GetOwner()->GetContext(),SkillMsg);
			}
		}
	}

	// turn clear for each actor
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor *actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL) 
			continue;
		if (!actor->GetLeftBattle())
		{
			actor->TurnClear();
		}
	}


	CheckBatEvent();			// check any battle event happen
	if (m_bNpcScript || m_bPvPScript)
		m_pScriptSys->CallBatMethod(BAT_ON_PRETURN, m_curTurn);

	if (m_talkNum > 0)
		WaitTalk();

	return (m_talkNum == 0);
}

Boolean CBattle::WaitCmd()
{
	TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, _F("CBattle WaitCmd: battle %d:%d:%d, pstate %d"), 
		GetID(), GetNetId().srvId, GetNetId().objId, m_state->GetStateID());
	ChangeState(ST_WAITCMD);				// change to wait command state
	NextState(ST_PLAYACTION);				// next to play action state

	Boolean bWaitAdded = BOOLEAN_FALSE;
	NetGroup addedGrp;

	// added waiting list character to main actors list if not battle end
	if (m_waitlist.size() > 0)
	{
		// add waiting list character
		for (Actors::iterator it = m_waitlist.begin(); it != m_waitlist.end(); ++it)
		{
			CBatActor *actor = FindNetObject<CBatActor>(*it);
			if(actor == NULL)
				continue;
			if (actor->IsParnter()) {		// find partner position according its owner
				CBatActor* pBatChar = GetActor(CBatActor::TARGET_CHARACTER, 
					actor->m_pMob->GetOwner_id());
				if (pBatChar)
					actor->SetPosition(pBatChar->GetPosition());
			}
			InsertActor(actor);					// insert to available slot index
			addedGrp.Add(actor);
		}
		m_waitlist.clear();
		bWaitAdded = BOOLEAN_TRUE;
	}

	// tell client for next turn
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		if (client.IsLeft())				// skip left character
			continue;

		CBatCharacter *pBatChar = client.pBatChar;
		CCharacter* pChar = client.pOwner;
		LPCPEER	pPeer = pChar->GetPeer();
		RPCContext &context = pChar->GetContext();

		// try to send new added actors
		if (bWaitAdded && pPeer)
			//PrepareSendGroup(addedGrp);
			CB_AddActors(pPeer, context, &addedGrp, client.team);

		// if client is waiting, send back the control
		if (client.IsWaiting()) {
			if (pBatChar) {
				pBatChar->InitPrevCmd();	// init previous command before controlling character				
				pBatChar->SendAttr(pPeer, context, CBatCharacter::AllMask);
				CB_ControlCharacter(pPeer, context, pBatChar->GetIndex());
			}
			if (client.pPartner) {
				client.pPartner->InitPrevCmd();	// init previous command before controlling partner
				client.pPartner->SendAttr(pPeer, context, CBatMob::AllMask);
				CB_ControlPartner(pPeer, context, pBatChar->GetIndex(), 
					client.pPartner->GetIndex());
			}
			client.status = CLIENT_BATTLE;
		}
		else if (client.pPartner && client.pPartner->IsNew())
		{
			// no init previous for new called friend
			CB_ControlPartner(pPeer, context, pBatChar->GetIndex(), 
				client.pPartner->GetIndex());
			client.pPartner->SetNew(false);
		}

		// tell client for next turn
		Boolean bInput = client.pBatChar ? client.pBatChar->m_bInput : BOOLEAN_TRUE;
		Boolean bPartnerInput = client.pPartner ? client.pPartner->m_bInput : BOOLEAN_TRUE;
		CB_NextTurn(pPeer, context, m_curTurn, CMD_TIMEOUT, client.autoCount,
			bInput, bPartnerInput);

		if (client.bAuto && pBatChar)
		{
			// set auto event
			pBatChar->RaiseAutoEvent();
		}

		/*else if (pChar->IsRobot() && pPeer)
		{
			// client robot action for battle
			if (!bInput)		// character turn
				pChar->CB_RobotBattleTurn(pPeer, context, _T("BattleTurn"), GetNetId(), 
					pChar->GetNetId(), pBatChar->GetNetId(), client.pPartner ? 
					client.pPartner->GetNetId() : RPCNetID());
			if (!bPartnerInput && client.pPartner)	// partner turn
				pChar->CB_RobotBattleTurn(pPeer, context, _T("PartnerTurn"), GetNetId(), 
					pChar->GetNetId(), pBatChar->GetNetId(), client.pPartner->GetNetId());
			if (pBatChar == NULL)	// monitor turn
				pChar->CB_RobotBattleTurn(pPeer, context, _T("MonitorTurn"), GetNetId(), 
					pChar->GetNetId(), RPCNetID(), RPCNetID());
		}*/

	}
	
	Actors actTmp;
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor *actor = FindNetObject<CBatActor>(*it);
		if (actor == NULL)
			continue;

		if (actor->GetLeftBattle())
			continue;

		if (actor->m_postSkillTurn > 0)
			actor->m_postSkillTurn --;

		if (actor->IsDead() && !actor->IsPlayer())
			continue;

		if (actor->AllCmdBlock() || actor->m_postSkillTurn > 0)
		{
			// notify back to Battle
			actor->m_command = BATCMD_IDLE;
			//actor->m_bInput = true;
			actor->SetIsInput(BOOLEAN_TRUE);
			if (actor->m_pClient)
				actor->SendCommandSuccess(BATCMD_IDLE);
			actTmp.push_back(actor->GetNetId());
		}
		else if (actor->m_postSkillID > 0 && actor->m_postSkillTurn == 0)
		{
			actor->m_command = BATCMD_SKILL;
			actor->m_cmdTarget = actor->m_postSkillTarget;

			//actor->m_bInput = true;
			actor->SetIsInput(BOOLEAN_TRUE);
			if (actor->m_pClient)
				actor->SendCommandSuccess(BATCMD_SKILL);
			actTmp.push_back(actor->GetNetId());
		}
		else if (!actor->m_AI.IsEmpty())
		{
			CallLuaFunc(gGlobal.g_pLuaState, actor->m_AI.c_str(), String(), this, actor);
		}
		else
		{
			actor->ResetAction();
		}
	}
	
	for (Actors::iterator it= actTmp.begin();it != actTmp.end();it++) 
	{
		CBatActor *actor = FindNetObject<CBatActor>(*it);
		if (actor == NULL)
			continue;
		NotifyInput(actor);
	}

	RaiseUniqEventID(EVT_CMD_TIMEOUT, CMD_TIMEOUT + TIMEOUT_DELAY);

	return BOOLEAN_FALSE;
}

void CBattle::DoBuffActiveList(StlVector<CBatActor *> & buffActiveList)
{
	UInt maxTime = 0;
	for (UInt i = 0; i < buffActiveList.size(); i ++)
	{
		if (i == buffActiveList.size() - 1)
			buffActiveList[i]->SetLinkNext(BOOLEAN_FALSE);
		else
			buffActiveList[i]->SetLinkNext(BOOLEAN_TRUE);
		UInt8 numShowCount = 0;

		{
			if (buffActiveList[i]->IsUpdated(CBatActor::HPMask) &&
				buffActiveList[i]->GetDShowHP())
				numShowCount ++;

			if (buffActiveList[i]->IsUpdated(CBatActor::SPMask) &&
				buffActiveList[i]->GetDShowSP())
				numShowCount ++;

			if (buffActiveList[i]->IsUpdated(CBatActor::DPMask) &&
				buffActiveList[i]->GetDShowDP())
				numShowCount ++;

			if (buffActiveList[i]->IsUpdated(CBatActor::WCMask) &&
				buffActiveList[i]->GetDShowWC())
				numShowCount ++;

			if (buffActiveList[i]->IsUpdated(CBatActor::VPMask) &&
				buffActiveList[i]->GetDShowVP())
				numShowCount ++;
		}

		buffActiveList[i]->SetReaction(BATCMD_BUFF_EFFECT_HIT);									
		buffActiveList[i]->MarkUpdate(CBatActor::reactionMask);
		buffActiveList[i]->WriteReaction();

		if (maxTime < (UInt)(numShowCount * NUM_DISPLAY_TIME))
			maxTime = numShowCount * NUM_DISPLAY_TIME;

	}

	m_animTime += maxTime;
	buffActiveList.clear();
}

void CBattle::WriteExtraActor()
{
	for (UInt i = 0; i < m_ExtraActorMap.size(); i ++)
	{
		CBatActor * pActor = FindNetObject<CBatActor>(m_ExtraActorMap[i]);
		if(pActor == NULL)
			continue;
		pActor->SetLinkNext(BOOLEAN_TRUE);					
		pActor->SetHitTogether(BOOLEAN_TRUE);
		pActor->MarkUpdate(CBatActor::hitTogetherMask);
		pActor->SetReaction(pActor->GetTempReaction());
		pActor->WriteReaction();
	}
	m_ExtraActorMap.clear();
}

Boolean CBattle::PlayAction()
{
	TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, _F("CBattle PlayAction: battle %d:%d:%d, pstate %d"), 
		GetID(), GetNetId().srvId, GetNetId().objId, m_state->GetStateID());
	ChangeState(ST_PLAYACTION);				// change to play action state
	NextState(ST_POSTTURN);		// next to post-turn state

	m_turnBuffer1->ResetCursor();
	m_turnBuffer1->SetSize(m_turnBuffer1->GetCursor());
	m_turnBuffer2->ResetCursor();
	m_turnBuffer2->SetSize(m_turnBuffer2->GetCursor());
	m_turnBufferMon->ResetCursor();
	m_turnBufferMon->SetSize(m_turnBufferMon->GetCursor());
	m_turnTime = ::timeGetTime();			// play turn start time
	m_animTime = 0;

	// sort mobs and character by speed
	SpeedOrder orderList;
	SpeedOrder deadOrderList;
	
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor *actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL) 
			continue;
		if (actor->GetLeftBattle() || actor->AllCmdBlock() ||
			(actor->IsDead() && !actor->IsPlayer()))
			continue;

		// set mobs command by AI system
		if (!actor->m_bInput && actor->m_pMob && !actor->IsParnter())
		{
			// User define mob command by the game designer
//			if (m_pScriptSys)
//			{
//				if (!(m_pScriptSys->MobCmdEventHandle(actor)))
//					m_pScriptSys->CallBatScript(BAT_MOBCMD_PART, actor);
//			}

			TRACE_INFODTL_5(GLOBAL_LOGGER, _F("set command by ai, battle %d:%d:%d, actor %d, mob %d"),
				GetID(), GetNetId().srvId, GetNetId().objId, actor, actor->m_pMob);
			// run the AI of the mob if user has not inputed the command
			actor->m_pMob->SetCommandByAI();
		} else if (!actor->m_bInput) {
			// User enforce to change the battle command 
//			if (m_pScriptSys)
//				m_pScriptSys->CallBatScript(BAT_PLAYERCMD_PART, actor);
			if (actor->m_pBatChar && actor->m_pClient->bAuto)
				actor->m_pBatChar->StopAutoEvent();		// stop auto event
			//actor->AutoInput();							// repeat steps
			
			//force normal attack
			actor->m_command = BATCMD_ATTACK;
			actor->m_cmdTarget = 0;
			if (actor->NextTarget(TRUE) == NULL)
				actor->m_command = BATCMD_IDLE;
			
			//actor->m_bInput = BOOLEAN_TRUE;
			actor->SetIsInput(BOOLEAN_TRUE);
		}
		
		//speed = (float) actor->GetSPD() * GetRand(0.9f, 1.1f);
		orderList.insert(std::make_pair(actor->m_speed, actor));
		
		if (actor->IsDead() && actor->IsPlayer())
			deadOrderList.insert(std::make_pair(actor->m_speed, actor));
	}

	//check buff effects before start
	StlVector<CBatActor *> buffActiveList;
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor *actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL) 
			continue;
		if (actor->GetLeftBattle())	
			continue;
		actor->DoBuffActive(NULL, BUFF_ACTIVE_TIME_TYPE_TURN_START, &buffActiveList);
	}
	DoBuffActiveList(buffActiveList);

	// resort command action in order, and put all sequence in m_turnBuffer
	for (SpeedOrder::iterator it = orderList.begin(); it != orderList.end(); ++it)
	{
 		CBatActor *actor = it->second;

		// skip left, dead or escaped actor, or action is done
		if (actor == NULL || !actor->IsValidTarget() || actor->m_bAction) continue;

		// look forward to check if combo attack happen
		if (actor->m_command != BATCMD_ATTACK || !CheckCombo(orderList, it))
		{
			actor->DoAction();
		}

		// check force to end
		if (CheckForceEnd()) break;
	}

	//check dead actor list that may reborn
	for (SpeedOrder::iterator it = deadOrderList.begin(); it != deadOrderList.end(); ++it)
	{
 		CBatActor *actor = it->second;

		// skip left, dead or escaped actor, or action is done
		if (actor == NULL || !actor->IsValidTarget() || actor->m_bAction) continue;

		// look forward to check if combo attack happen
		if (actor->m_command != BATCMD_ATTACK || !CheckCombo(deadOrderList, it))
		{
			actor->DoAction();
		}

		// check force to end
		if (CheckForceEnd()) break;
	}

	// check whether the battle is finish
	CheckTeamStatus();

	// check buff effects when end and the battle is not finished
	if (m_winTeam == 255)
	{
		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
		{
			CBatActor *actor = FindNetObject<CBatActor>(*it);
			if(actor == NULL) 
				continue;
			if (actor->GetLeftBattle() ||
				actor->IsParnter() && actor->m_pMob->IsNew())
				continue;
			actor->DoBuffActive( NULL, BUFF_ACTIVE_TIME_TYPE_TURN_END, &buffActiveList);
		}
		DoBuffActiveList(buffActiveList);

		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
		{
			CBatActor *actor = FindNetObject<CBatActor>(*it);
			if(actor == NULL) 
				continue;
			if (actor->GetLeftBattle() ||
				actor->IsParnter() && actor->m_pMob->IsNew())
				continue;
			actor->UpdateBuff(0);
			actor->PackBuff();
		}
	}

	// send action buffer to all clients
	Boolean noWait = BOOLEAN_TRUE;

	ByteArray array(BUFFER_SEGMENT_SIZE);
	UInt32 totalSize = 0;
	CONST Byte* pData = NULL;
	UInt32 arraySize;

	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		if (client.IsLeft() || !client.IsConnect())	// skip left character
			continue;

		if (it->second.IsMonitor())
		{
			totalSize = (UInt32) m_turnBufferMon->GetSize() - REUBEN_NETWORK_BUFFER_HEADER_SIZE;
			pData = m_turnBufferMon->GetData() + REUBEN_NETWORK_BUFFER_HEADER_SIZE;
		}
		else
		{
			if (it->second.pBatChar == NULL)
				continue;
			if (it->second.pBatChar->GetTeam() == ENEMY_TEAM)
			{
				totalSize = (UInt32) m_turnBuffer1->GetSize() - REUBEN_NETWORK_BUFFER_HEADER_SIZE;
				pData = m_turnBuffer1->GetData() + REUBEN_NETWORK_BUFFER_HEADER_SIZE;
			}
			else
			{
				totalSize = (UInt32) m_turnBuffer2->GetSize() - REUBEN_NETWORK_BUFFER_HEADER_SIZE;
				pData = m_turnBuffer2->GetData() + REUBEN_NETWORK_BUFFER_HEADER_SIZE;
			}
		}

		for (UInt32 index = 0; index < totalSize; index += BUFFER_SEGMENT_SIZE)
		{
			client.bEndTurn = BOOLEAN_FALSE;		// reset end turn flag
			if (noWait)
				noWait = BOOLEAN_FALSE;				// need to wait

			if (m_friendGroup.GetCount() > 0) {		// send all coming friends
				CB_AddPartners(client.pOwner->GetPeer(), client.pOwner->GetContext(), 
					&m_friendGroup, client.team);
			}
			// send to all client to start play
			if (index + BUFFER_SEGMENT_SIZE < totalSize)
				arraySize = BUFFER_SEGMENT_SIZE;
			else
				arraySize = totalSize - index;
			array.Clone(pData, arraySize);

			if (index == 0) {		// first segment
				CB_PlayTurn(client.pOwner->GetPeer(), client.pOwner->GetContext(), m_curTurn,
					array, totalSize);
			} else {				// more segment
				CB_PlayTurnMore(client.pOwner->GetPeer(), client.pOwner->GetContext(), array,
					totalSize);
			}
			if (index + BUFFER_SEGMENT_SIZE < totalSize)
				pData += BUFFER_SEGMENT_SIZE;
		}
	}

	

	if (m_friendGroup.GetCount() > 0)
		m_friendGroup.Clear();

	TRACE_INFODTL_5(GLOBAL_LOGGER, 
		_T("CBattle PlayAction: battle %d:%d, turn %d, buffer size %d, animTime %d"), 
		GetNetId().srvId, GetNetId().objId, m_curTurn, m_turnBuffer1->GetSize(), m_animTime);

	if (m_animTime > 120000) {
		TRACE_WARNDTL_1(GLOBAL_LOGGER, 
			_T("CBattle PlayAction Warning: animation time Too large, animTime = %d"), m_animTime);
		m_animTime = 120000;
	}

	// if no need to wait, schedule to next turn
	if (noWait)
		RaiseUniqEventID(EVT_NEXT_TURN, m_animTime);
	else
		RaiseUniqEventID(EVT_NEXT_TURN, m_animTime + NEXT_TURN_ADJUST);

	return BOOLEAN_FALSE;
}

Boolean CBattle::PostTurn()
{
	CancelAllEvent();			// cancel all schedule events
	CheckTeamStatus();			// check all team status	
	CheckBatEvent();			// check any battle event happen

	TRACE_VERBOSEDTL_7(GLOBAL_LOGGER, _F("CBattle PostTurn: Battle netId %d:%d, turn %d,player count %d, enemy count %d, animtime diff %d, pstate %d"),
		GetNetId().srvId, GetNetId().objId, m_curTurn, m_teamData[PLAYER_TEAM].existCount,
		m_teamData[ENEMY_TEAM].existCount, ::timeGetTime() - m_turnTime - m_animTime,
		m_state->GetStateID());
	ChangeState(ST_POSTTURN);				// change to post-turn state
	NextState(ST_PRETURN);		// next to pre-turn state

	m_talkNum = 0;
	if ((m_bNpcScript || m_bPvPScript) && m_pScriptSys->GetOwner())
		m_pScriptSys->CallBatMethod(BAT_ON_POSTTURN, m_curTurn);

	// valid check if all characters left
	bool teamLeft[2] = { true, true };
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		if (client.pBatChar && (!client.IsLeft() && !client.pBatChar->GetLeftBattle()))
			teamLeft[client.pBatChar->GetTeam()] = false;
	}
	if (teamLeft[0] == true && teamLeft[1] == true)
		m_forceEnd = BOOLEAN_TRUE;

	// if one team win or escaped, battle is ended
	if (m_winTeam != 255)
	{
		TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, 
			_F("End battle %s team win, battle %d:%d:%d"),
			m_winTeam == PLAYER_TEAM ? _T("player") : _T("enemy"), 
			GetID(), GetNetId().srvId, GetNetId().objId);
		m_forceEnd = BOOLEAN_TRUE;
	} else if (m_curTurn >= gGlobal.g_MaxBattleTurn)		// excess max turn
		m_forceEnd = BOOLEAN_TRUE;

	if (m_forceEnd)					// change to finish phase when force end
		NextState(ST_FINISH);

	if (m_talkNum > 0)
		WaitTalk();

	return (m_talkNum == 0);
}

void CBattle::FinishBattle()
{
//	UInt32 playedTime = ::timeGetTime() - m_turnTime;
//	if (!m_forceEnd || playedTime < m_animTime)
//		return;
	if (!m_forceEnd)
		return;

	CancelAllEvent();					// make sure to cancel all event
	ChangeState(ST_FINISH);				// change to finish state
	CheckTeamStatus();
	TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("CBattle Finish: battle %d:%d:%d, winTeam %d, pstate %d"), 
		GetID(), GetNetId().srvId, GetNetId().objId, m_winTeam, m_state->GetStateID());

	if (m_bNpcScript || m_bPvPScript)
	{
		int result = END_DRAW;
		if (m_winTeam == PLAYER_TEAM)
			result = END_WIN;
		else if (m_winTeam == ENEMY_TEAM)
			result = END_LOSE;
		m_pScriptSys->SetRunOnce(BOOLEAN_TRUE);			// release lua after that
		m_pScriptSys->CallBatMethod(BAT_ON_FINISH, m_curTurn, NULL, result);
	}
#ifdef LINEUP_SYSTEM_ON
	CancleLineupBuffer(0);
	CancleLineupBuffer(1);
	CancleLineupAllelopathy();
#endif

	UInt32Vector winCharList;
	
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		if (client.IsLeft())				// skip left character
			continue;

		CBatCharacter *pBatChar = client.pBatChar;

		CCharacter* pChar = client.pOwner;		
		
		LPCPEER	pPeer = pChar->GetPeer();
		RPCContext &context = pChar->GetContext();

	
		if (pBatChar && client.IsBattle())
		{
			
			if (m_winTeam == 255)								// draw game
				DrawnGame(&client);
			else if (pBatChar->IsDead() && (GetBattleType() == NPC_BATTLE || GetBattleType() == PK_BATTLE || GetBattleType() == FORCE_PK_BATTLE ||
				GetBattleType() == FREE_PK_BATTLE))						// player lose
				DeadPenalty(&client);
			else if (pBatChar->IsEscaped())						// player escaped
				CharEscaped(&client);
			else if (m_curTurn >= gGlobal.g_MaxBattleTurn)		// turn over
			{
				DrawnGame(&client);
				CB_Error(pPeer, context, BATTLE_SPECIAL_END_TIMEOUT, "");
			}
			else if (pBatChar->GetTeam() == m_winTeam)			// player win
			{
				WinReward(&client, m_teamData[m_winTeam].playerCount, 
					m_teamData[m_winTeam].killCount, m_teamData[1 - m_winTeam].playerCount);
				winCharList.push_back(pBatChar->GetChar_id());
			}
			else
				DeadPenalty(&client);
		} 
		else 
		{
			if (client.IsWaiting())
			{
				RemoveWaiting(&client);
			}
			else
				RemoveMonitor(&client);
		}
		if (pChar->m_BattleNetID != RPCNetID())
		{
			TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("battle object not NULL char sid: %s battle %d:%d client state: %d"),
				pChar->GetSessionId().GetString().c_str(), 
				pChar->m_BattleNetID.srvId, pChar->m_BattleNetID.objId,
				client.status);

			pChar->ChangeState(CCharacter::ST_NOBATTLE);
			pChar->m_BattleNetID = RPCNetID();
			pChar->m_pBatChar = NULL;

		}
	}		
	
	if (GetBattleType() == HIGH_MARTIAL_BATTLE)
	{
		String martialMsg;
		if (m_winTeam == ENEMY_TEAM)
			martialMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_HIGH_MARTIAL_WIN")), m_mapString.c_str(), m_team0Leader.c_str(), m_team1Leader.c_str());
		else if (m_winTeam == PLAYER_TEAM)
			martialMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_HIGH_MARTIAL_WIN")), m_mapString.c_str(), m_team1Leader.c_str(), m_team0Leader.c_str());
		else
			martialMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_HIGH_MARTIAL_DRAW")), m_mapString.c_str(), m_team0Leader.c_str(), m_team1Leader.c_str());

		MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
			RPCContext(), martialMsg, 0, 1);
	}

	if (!winCharList.empty())
		MessageService_Proxy::BattleEndUpdate(gGlobal.pMessageService->pPeer, 
			RPCContext(), winCharList);

	//if (IsAllLeft())
	DeleteBattle();
}

void CBattle::WaitTalk()
{
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		if (client.IsLeft() || !client.IsConnect())		// skip left character
			continue;
		CB_WaitTalk(client.pOwner->GetPeer(), client.pOwner->GetContext(), 
			m_curTurn, TALK_TIMEOUT);
	}
	RaiseUniqEventID(EVT_TALK_TIMEOUT, TALK_TIMEOUT + TIMEOUT_DELAY);
}

void CBattle::CancelTalk()
{
	if (m_pScriptSys == NULL || m_talkNum == 0)
		return;

	m_pScriptSys->StopScript();						// resume until end function

	m_talkNum = 0;
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		if (client.IsLeft() || !client.IsConnect())		// skip left character
			continue;
		CB_CancelTalk(client.pOwner->GetPeer(), client.pOwner->GetContext());
	}
}

float CBattle::GetNpcLevel()
{
	// calculate average level of mobs
	UInt16 totalLevel = 0;
	UInt8 mobCount = 0;
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor *actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL) 
			continue;
		if (actor->GetTeam() == ENEMY_TEAM) {
			totalLevel += actor->GetLevel();
			mobCount++;
		}
	}
	return float(totalLevel) / mobCount;
}

void CBattle::UpdateToMap(BatClient *pClient, UInt8 endState, bool jumpBack)
{
	if (pClient == NULL)
		return;

	CBatCharacter *pBatChar = pClient->pBatChar;
	CCharacter* pChar = pClient->pOwner;
	SessionID &sid = pClient->GetContext().sessionId;
	Session* psess = GetSession(sid);
	Boolean bDead = pBatChar->IsDead();
	UInt32 nTeam = pBatChar->GetTeam();

	if (psess == NULL) {
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, 
			_FE("session id not found, sessionId %s"), 
			sid.GetString().c_str());
	} else {
		psess->UnsetOwnObject(this);
		psess->UnsetOwnObject(pBatChar);
	}

	TRACE_VERBOSEDTL_6(GLOBAL_LOGGER, 
		_F("battle finished, battle %d:%d:%d, char_id %d, sessionId %s, endState %d"), 
		GetID(), GetNetId().srvId, GetNetId().objId, pBatChar->GetChar_id(), 
		sid.GetString().c_str(), endState);
	CB_EndBattle(pChar->GetPeer(), pChar->GetContext(), endState);

	// update partners of character
	if (pBatChar->m_partnerGroup && pBatChar->m_partnerGroup->GetCount() > 0)
	{
		CPartner* partner;
		CBatMob* pMob;

		for (UInt16 i = 0; i < pBatChar->m_partnerGroup->GetCount(); i++) {
			pMob = pBatChar->m_partnerGroup->GetChild<CBatMob>(i);
			if (pMob == NULL)
				continue;
			if (pMob->GetOwner_id() != 0 && pMob->GetOwner_id() != pChar->GetChar_id())
			{
				TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("mismatch partner owner, sessionId %s, char_id %d, owner_id %d"),
					sid.GetString().c_str(), pChar->GetChar_id(),
					pMob->GetOwner_id());
				continue;
			}

			if (pMob->GetPartner_id() == 0)
			{			// new partner
				partner = CPartner::NewInstance();
				partner->SetId(0, pMob->GetMob_id());
				partner->SetLevel(pMob->GetLevel());
				partner->SetOwner(pChar->GetChar_id());
				if (!pChar->m_pPartnerGroup->AddNewPartner(partner))
				{	// fail to add partner
					DeleteNetObject(partner);
				}
			}
			else
			{									// update partner
				partner = pChar->m_pPartnerGroup->GetPartner(pMob->GetPartner_id());
				if (partner && partner->GetState() != CPartner::ST_STANDBY &&
					GetBattleType() != MATCH_BATTLE && GetBattleType() != LOW_MARTIAL_BATTLE && GetBattleType() != HIGH_MARTIAL_BATTLE &&
					GetBattleType() != GUILD_WAR_BATTLE  && GetBattleType() != INCOGNITO_PK_BATTLE)
				{
					CallLuaFunc(gGlobal.g_pLuaState, "partnerEndBattle", String(), pBatChar, pMob);
					pMob->RemoveBuffAfterBattle();
					pMob->UpdateHealthAndFavor();
					pMob->WritePartnerData(partner);
					
					if (partner->GetOwner_id() == 0)
					{
						pChar->m_pPartnerGroup->RemovePartner(partner);
					}
					else
					{
						partner->CalcAttrib();
						pMob->SaveLastCmd();
						if (partner->GetState() == CPartner::ST_FIGHT &&
							partner->CanLevelUp()) 
						{
							partner->LevelUp();
						}
						if (partner->GetState() == CPartner::ST_REST)
							partner->SetState(CPartner::ST_STANDBY);
						CallLuaFunc(gGlobal.g_pLuaState, "CalPartnerFav", String(), pChar, partner);
					}
					//for saving last Cmd

					pMob->m_pPartner = NULL;
				}
			}
			psess->UnsetOwnObject(pMob);
		}
	}

	//check any effect after battle
	CallLuaFunc(gGlobal.g_pLuaState, "EndBattle", String(), pBatChar);
	// update level & exp to object service
	pBatChar->RemoveBuffAfterBattle();
	
	pBatChar->ConstructBuffString();
	pBatChar->ConstructBuffCacheString();
	
	if (GetBattleType() != MATCH_BATTLE &&
		GetBattleType() != LOW_MARTIAL_BATTLE &&
		GetBattleType() != HIGH_MARTIAL_BATTLE &&
		GetBattleType() != GUILD_WAR_BATTLE &&
		GetBattleType() != INCOGNITO_PK_BATTLE)
		pBatChar->WriteData(pChar);

	//only DP should always overwrite to character
	if (GetBattleType() != MATCH_BATTLE && (pBatChar->GetDP() != pChar->GetDP() || pBatChar->GetMDP() != pChar->GetMDP() || 
		pBatChar->GetMaxDP() != pChar->GetMaxDP()))
		pChar->SetDP(pBatChar->GetDP(), pBatChar->GetMaxDP(), pBatChar->GetMaxDP());

	pChar->CalcAttrib();

	//if (pClient->autoCount == 255)
	if (pClient->autoCount == 65535)
		pChar->m_autoCount = 0;
	else
		pChar->m_autoCount = pClient->autoCount;
	pBatChar->SaveLastCmd();
	pChar->ChangeState(CCharacter::ST_NOBATTLE);
	pChar->m_BattleNetID = RPCNetID();
	pChar->m_pBatChar = NULL;
	pClient->pBatChar = NULL;
	pClient->pPartner = NULL;
	pBatChar->m_pOwner = NULL;
	pBatChar->m_pBag = NULL;
	pBatChar->ClearAllPartnerItemBag();

	pChar->m_pQuestSys->CheckAfterBattle(endState, m_npcId, m_mobIndices);
	//if (pChar->GetScriptSystem()->IsRunning())
	//{
	//	pChar->GetScriptSystem()->StopScript();
	//}

	// reset this BatClient
	*pClient = BatClient();

	// character is escaped or lose after battle
	if (endState == END_LOSE && GetBattleType() !=  MATCH_BATTLE && GetBattleType() != LOW_MARTIAL_BATTLE 
		&&  GetBattleType() != GUILD_WAR_BATTLE  && GetBattleType() != INCOGNITO_PK_BATTLE)
	{
		// force stop script before leave party
		if (pChar->GetScript()->IsRunning())
			pChar->StopScript();
		// leave party if have
		if (pChar->GetParty())
		{
			//pChar->GetParty()->Leave(pChar);
			if (pChar->IsLeader()) 
				pChar->GetParty()->SetTempLeaveAll();
			else
				pChar->LeaveTeamTemporarily(pChar->GetPeer(),pChar->GetContext());
		}
		pChar->m_stepMod = 0;
	}
	else if (endState == END_ESCAPED) 
	{
		if (pChar->GetScript()->IsRunning())
			pChar->StopScript();
		if (pChar->GetParty()) 
			pChar->GetParty()->Leave(pChar);
		pChar->m_stepMod = 0;
	}
	else if (endState == END_WIN)
	{
		if (pChar->GetParty() == NULL || pChar->GetParty()->GetLeader() == pChar)
			CallLuaFunc(gGlobal.g_pLuaState, "CalStepMod", String(), pChar, m_curTurn);
		pChar->m_pQuestSys->CheckNpcDie(m_npcId);
	}

	// logout player when should logout
	if (pChar->IsLogout())
	{
		pChar->ScheduleLogout(0);
	}
	else
	{
		pChar->RaiseUpdate();						// raise update character event
		pChar->m_pBag->ReduceEqEnduring();
		pChar->m_pBag->RaiseUpdate();				// raise update item bag event
		pChar->m_pPartnerGroup->RaiseUpdate();		// raise update partner group event
	}

	// check character need to jump to other place or reborn place
	if (bDead&&GetBattleType() != MATCH_BATTLE &&
		GetBattleType() != LOW_MARTIAL_BATTLE &&
		GetBattleType() != GUILD_WAR_BATTLE && 
		GetBattleType() != INCOGNITO_PK_BATTLE &&
		pChar->GetPK_Value() > 0 &&
		HasLuaFunc(gGlobal.g_pLuaState, "OnCharacterImprison"))
	{
		CallLuaFunc(gGlobal.g_pLuaState, "OnCharacterImprison", String(), pChar);
		UInt32 result = (UInt32)gGlobal.g_TempResult;
		UInt32 nReward = pChar->GetWantedReward();
		if(result)
		{
			if(( m_winTeam == PLAYER_TEAM || m_winTeam == ENEMY_TEAM ) && m_teamData[m_winTeam].pLeader && m_winTeam != nTeam)
			{

				m_teamData[m_winTeam].pLeader->ChangeMoney((pChar->GetPK_Value()*500 + nReward),LogService_Proxy::Money_And_Exp::SOURCE_WANTED_REWARD_GET);
				String msg;
				msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_RUMOR_WANTED_CATCH")), m_teamData[m_winTeam].pLeader->GetNickName().c_str(),pChar->GetNickName().c_str(),pChar->GetPK_Value()*500 + nReward);
				MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
					RPCContext(), msg, 0, 1);
			}
			else if(pChar->GetPK_Value() > 5)
			{
				String msg;
				msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_RUMOR_WANTED_GOVERNMENT_CATCH")),pChar->GetNickName().c_str());
				MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
					RPCContext(), msg, 0, 1);
			}
		}
		if(nReward > 0)
			pChar->UpdateWantedReward(pChar->GetChar_id(),-(Int32)nReward);
	}
	else if (pChar->GetScript()->NeedJump())
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateToMap: Character with uid %d need to jump map after WinReward"), pChar->GetChar_id()); 
		pChar->RaiseUniqEventID(CCharacter::EVT_CHECK_JUMP);
		if (pChar->m_bManorExpel)
			pChar->m_bManorExpel = BOOLEAN_FALSE;
	}
	else if (jumpBack && pChar->GetMap())
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateToMap: Character with uid %d died during the battle so need to reborn"), pChar->GetChar_id()); 
		pChar->JumpReborn();
		if (pChar->m_bManorExpel)
			pChar->m_bManorExpel = BOOLEAN_FALSE;
	}
	else if (pChar->m_bManorExpel)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateToMap: Character with uid %d is forced to reborn because he is expelled from instance during the battle"), pChar->GetChar_id()); 
		pChar->m_bManorExpel = BOOLEAN_FALSE;
		pChar->JumpReborn();
	}
	else if (pChar->CheckNeedJumpReborn())
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateToMap: Character with uid %d jump map after battle"), pChar->GetChar_id()); 
		pChar->JumpReborn();
		if (pChar->m_bManorExpel)
			pChar->m_bManorExpel = BOOLEAN_FALSE;
		CB_Error(pChar->GetPeer(), pChar->GetContext(), 0, _T("MSG_MAP_FORCE_LEAVE"));
	}
	else if (GetBattleType() == GUILD_WAR_BATTLE)
	{
		if (pChar->GetVP() == 0 && endState != END_WIN)
		{
			UInt result = 3;
			CallLuaFunc(gGlobal.g_pLuaState,"CalGuildWarReward", String(), pChar, result);
			if (pChar->GetParty())
				pChar->GetParty()->Leave(pChar);
			
			pChar->JumpReborn(gConfigure.guildWarRebornMapID, gConfigure.guildWarRebornMapX, gConfigure.guildWarRebornMapY);
		}
		//kick player when guild war end
		else if (gGlobal.g_GuildWarState != GUILD_WAR_STATE_IN_WAR &&  pChar->GetMap()->GetGuildWarKick())
		{
			if (pChar->GetParty())
				pChar->GetParty()->Leave(pChar);
			pChar->JumpReborn(gConfigure.guildWarRebornMapID, gConfigure.guildWarRebornMapX, gConfigure.guildWarRebornMapY);
		}
	}
}

void CBattle::DeleteBattle()
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("DeleteBattle: netId %d:%d, endTurn %d, winTeam %d"), 
		GetNetId().srvId, GetNetId().objId, m_curTurn, m_winTeam);

	BattleList::iterator it = std::find(gGlobal.m_aliveBattles.begin(), 
		gGlobal.m_aliveBattles.end(), this);
	if (it != gGlobal.m_aliveBattles.end())
		gGlobal.m_aliveBattles.erase(it);
	gGlobal.m_deadBattles.push_back(this);
}

void CBattle::WinReward(BatClient *pClient, UInt8 teamCount, UInt8 killCount, UInt8 otherTeamCount)
{
	if (pClient == NULL || pClient->pBatChar == NULL || pClient->pOwner == NULL)
		return;

	CBatCharacter *pBatChar = pClient->pBatChar;
	CBatMob *pPartner = pClient->pPartner;
	CCharacter* pOwner = pBatChar->m_pOwner;
	UInt8 team = pBatChar->GetTeam();

	pBatChar->SetLeftBattle(BOOLEAN_TRUE);
	if (pPartner)
		pPartner->SetLeftBattle(BOOLEAN_TRUE);
	pClient->status = CLIENT_LEFT;					// client is left

	if (GetBattleType() == PK_BATTLE || GetBattleType() == FREE_PK_BATTLE)
	{
		if(HasLuaFunc(gGlobal.g_pLuaState, "OnCharacterPKWinReward"))
			CallLuaFunc(gGlobal.g_pLuaState, "OnCharacterPKWinReward", String(), pOwner,team,killCount);
	}
	if (GetBattleType() == FORCE_PK_BATTLE)
	{
			

		if(pBatChar->GetTeam() == 1)
		{
			int nKillCount = 0;
			for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
			{
				CBatActor* actor = FindNetObject<CBatActor>(*it);
				if(actor == NULL) 
					continue;
				if (actor->m_pBatChar && actor->GetTeam() != pBatChar->GetTeam() && actor->IsDead()&&actor->GetClassID() == BatCharacter_CLASSID)
					nKillCount ++;
			}
			pBatChar->SetPK_Value(MIN(20,pBatChar->GetPK_Value() + nKillCount));
			pOwner->SetPK_Value(MIN(20,pOwner->GetPK_Value() + nKillCount));
		}
		if(HasLuaFunc(gGlobal.g_pLuaState, "OnCharacterForcePKWinReward"))
			CallLuaFunc(gGlobal.g_pLuaState, "OnCharacterForcePKWinReward", String(), pOwner,team,killCount);
		
	}

	// for character, compare level different and gain exp, drop item
	UInt32Vector gotItems;
	UInt32Vector discardItems;
	Boolean levelUp = BOOLEAN_FALSE;
	Boolean partnerLevelUp = BOOLEAN_FALSE;
	UInt32 expGain = 0;
	UInt32 partnerExpGain = 0;
	UInt32 boundsExpGain = 0;
	UInt32 boundsPartnerExpGain = 0;

	Int moneyEarn = 0;
	if (m_pNpcData)
	{
		float npcLevel = GetNpcLevel();
		gGlobal.g_TempResult = 0;
		pBatChar->SetBoundsExp(0);
		UInt8 maplevel = 0;
		if (pBatChar->m_pOwner && pBatChar->m_pOwner->GetMap() && pBatChar->m_pOwner->GetMap()->GetMapInfo())
			maplevel = pBatChar->m_pOwner->GetMap()->GetMapInfo()->m_basicinfo->m_maplevel;
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("player exp gain, battle %d:%d:%d, bat char %d"),
			GetID(), GetNetId().srvId, GetNetId().objId, pBatChar);
		CallLuaFunc(gGlobal.g_pLuaState, "playerExpGain", String(), pBatChar, killCount, npcLevel,
			m_pNpcData->exp, teamCount, maplevel);
		expGain = (UInt32)gGlobal.g_TempResult;
		boundsExpGain = pBatChar->GetBoundsExp();

		////// special reward by anti-hack system /////
		if (pOwner->m_expReward) // experience reward 
		{
			UInt32 ExtraExp = (UInt32) ((expGain + boundsExpGain) * 0.05f);
			if (ExtraExp == 0)
				ExtraExp = 1;
			pOwner->ChangeExp(expGain + boundsExpGain, LogService_Proxy::Money_And_Exp::SOURCE_BATTLE);
			pOwner->ChangeExp(ExtraExp, LogService_Proxy::Money_And_Exp::SOURCE_ANTIHACK_AWARD);
			pOwner->m_expReward = BOOLEAN_FALSE;	
		}
		else
            pOwner->ChangeExp(expGain + boundsExpGain, LogService_Proxy::Money_And_Exp::SOURCE_BATTLE);
		if (pOwner->m_hpReward) // hp reward
		{
			pBatChar->SetMHP(pBatChar->GetMaxHP());
			pBatChar->SetHP(pBatChar->GetMaxHP());
            pBatChar->SetSP(pBatChar->GetMaxSP());	
			pOwner->m_hpReward = BOOLEAN_FALSE;
  		}
		////////////////////////////////////////////////

		// for partner, compare level different and gain exp
		if (pPartner && pPartner->GetPartner_id() != 0 && 
			pPartner->GetState() != CHAR_ST_DEAD)
		{
			CPartner* pOwnPartner = pPartner->m_pPartner;
			gGlobal.g_TempResult = 0;
			pPartner->SetBoundsExp(0);
			if (pOwnPartner)
			{
				TRACE_INFODTL_5(GLOBAL_LOGGER, _F("partner exp gain, battle %d:%d:%d, bat char %d, partner %d"),
					GetID(), GetNetId().srvId, GetNetId().objId, pBatChar, pPartner);
				CallLuaFunc(gGlobal.g_pLuaState, "partnerExpGain", String(), pPartner, pBatChar, 
					killCount, npcLevel, m_pNpcData->exp, teamCount, maplevel);
			}

			boundsPartnerExpGain = pPartner->GetBoundsExp();
			if (pPartner->GetPartner_type() != LEVEL_NOT_CHANGE_PARTNER && 
				pOwnPartner->GetHealth() != 0)
			{
				partnerExpGain = (UInt32)gGlobal.g_TempResult;
			} else {
				partnerExpGain = 0;
			}

			if (partnerExpGain > 0) {
				partnerLevelUp = ((pOwnPartner->GetExp() + partnerExpGain + boundsPartnerExpGain) > pOwnPartner->GetNextLvExp());
				pOwnPartner->ChangeExp(partnerExpGain + boundsPartnerExpGain);
				if (partnerLevelUp)
				{
					pPartner->SetMaxHP(pOwnPartner->GetMaxHP());
					pPartner->SetMHP(pOwnPartner->GetMaxHP());
					pPartner->SetHP(pOwnPartner->GetMaxHP());
					pPartner->SetMaxSP(pOwnPartner->GetMaxSP());
					pPartner->SetMSP(pOwnPartner->GetMaxSP());
					pPartner->SetSP(pOwnPartner->GetMaxSP());
				}
			}
		}

		// "Drop Item" reads from CSV file named "drop item"
		CONST CSVItemData* pRandomItemLst = gGlobal.m_resource->GetCSVItemInfo(
											m_pNpcData->dropitemcsv);
		if (pRandomItemLst)
		{
			CSVItemRow* pCurrRow;
			UInt totaladded = 0;
			UInt32 randrow, randpos;
			UInt32 additemid;
			for (Index i = C_INDEX(0); i < pRandomItemLst->RowLst.size(); ++i)
			{
				pCurrRow = pRandomItemLst->RowLst[i];
				randrow = GetRand(MAX_DROPITEM_PERCENTAGE);
				// anti-addiction adjustment //
				UInt32 range = pCurrRow->uRange;
				if (pOwner && pOwner->m_pAntiGameAddict && (pOwner->m_pAntiGameAddict->m_playerInterest < 1.0f))
					range = (UInt32) ((float) range * pOwner->m_pAntiGameAddict->m_playerInterest);
				///////////////////////////////
				if (pCurrRow == NULL || randrow >= range)
					continue;
				randpos = GetRand(pCurrRow->nCount);
				if( randpos >= (UInt32)pCurrRow->ItemLst.size())
					continue;
				additemid = pCurrRow->ItemLst[randpos];
				// item add
				CItem* pItem = pBatChar->m_pBag->NewItem(additemid, 1);
				if (pItem == NULL) continue;
				if (pBatChar->m_pBag->AddItem(pItem, BAG_CURRENT)) {
					gotItems.push_back(additemid);
				} else {
					DeleteNetObject(pItem);
					discardItems.push_back(additemid);
				}
				++totaladded;
				if (totaladded >= DROP_ITEM_LIMIT)
					break;
			}
		}
	}

	else if (GetBattleType() == LOW_MARTIAL_BATTLE)
	{
		UInt count = 0;
		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
		{
			CBatActor* actor = FindNetObject<CBatActor>(*it);
			if(actor == NULL) 
				continue;
			if (actor->m_pBatChar && actor->GetTeam() != pBatChar->GetTeam())
				count ++;
		}
		moneyEarn = LOW_MARTIAL_MONEY + (LOW_MARTIAL_MONEY - LOW_MARTIAL_MONEY_TAX)  * count / teamCount;
		pOwner->ChangeMoney(moneyEarn, LogService_Proxy::Money_And_Exp::SOURCE_BATTLE);

		pOwner->ChangePvPScoreSingle(count);
		CB_MartialResult(pClient->pOwner->GetPeer(), pClient->pOwner->GetContext(), END_WIN, moneyEarn, count);
		
	}
	else if (GetBattleType() == HIGH_MARTIAL_BATTLE)
	{
		UInt count = 0;
		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
		{
			CBatActor* actor = FindNetObject<CBatActor>(*it);
			if(actor == NULL) 
				continue;
			if (actor->m_pBatChar && actor->GetTeam() != pBatChar->GetTeam())
				count ++;
		}

		moneyEarn = HIGH_MARTIAL_MONEY + (HIGH_MARTIAL_MONEY - HIGH_MARTIAL_MONEY_TAX)  * count / teamCount;
		pOwner->ChangeMoney(moneyEarn, LogService_Proxy::Money_And_Exp::SOURCE_BATTLE);
		
		pOwner->ChangePvPScoreSingle(count);
		CB_MartialResult(pClient->pOwner->GetPeer(), pClient->pOwner->GetContext(), END_WIN, moneyEarn, count);
	}
	else if (GetBattleType() == GUILD_WAR_BATTLE)
	{
		UInt count = 0;
		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
		{
			CBatActor* actor = FindNetObject<CBatActor>(*it);
			if(actor == NULL) 
				continue;
			if (actor->m_pBatChar && actor->GetTeam() != pBatChar->GetTeam())
				count ++;
		}

		pOwner->ChangeGDonate(count * 3, LogService_Proxy::Guild_Donate::SOURCE_BATTLE_REWARD);
		pOwner->m_battleRestTime = ::timeGetTime() + GUILD_WAR_REST_TIME;
		String msg;
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILDWAR_WIN_REWARD_BATTLE")), count * 3);
		pBatChar->CB_BattleMsg(pClient->pOwner->GetPeer(), pClient->pOwner->GetContext(), -1, msg, 0);
	}
	
	
	// tell client what reward
	CB_Reward(pClient->pOwner->GetPeer(), pClient->pOwner->GetContext(), levelUp, expGain, 
		gotItems, discardItems, partnerExpGain, partnerLevelUp, boundsExpGain, boundsPartnerExpGain);

	UpdateToMap(pClient, END_WIN, false);
}

void CBattle::DeadPenalty(BatClient *pClient)
{
	if (pClient == NULL || pClient->pBatChar == NULL || pClient->pOwner == NULL)
		return;

	CBatCharacter *pBatChar = pClient->pBatChar;
	CBatMob* pPartner = pClient->pPartner;
	CCharacter *pOwner = pClient->pOwner;
	UInt8 team = pBatChar->GetTeam();

	if (GetBattleType() == PK_BATTLE || GetBattleType() == FREE_PK_BATTLE)
	{
		if(HasLuaFunc(gGlobal.g_pLuaState, "OnCharacterPKDeadPenalty"))
			CallLuaFunc(gGlobal.g_pLuaState, "OnCharacterPKDeadPenalty", String(), pOwner,team);
	}
	if (GetBattleType() == FORCE_PK_BATTLE)
	{
		if(HasLuaFunc(gGlobal.g_pLuaState, "OnCharacterForcePKDeadPenalty"))
			CallLuaFunc(gGlobal.g_pLuaState, "OnCharacterForcePKDeadPenalty", String(), pOwner,team);
	}
	Int moneyLose = 0;
	if (GetBattleType() != MATCH_BATTLE &&
		GetBattleType() != LOW_MARTIAL_BATTLE &&
		GetBattleType() != GUILD_WAR_BATTLE &&
		GetBattleType() != INCOGNITO_PK_BATTLE)
	{
		Boolean bPunish = m_bPunish;

		// clear all non-permanence buff when dead
		
		pBatChar->SetLeftBattle(BOOLEAN_TRUE);
		if (pPartner)
			pPartner->SetLeftBattle(BOOLEAN_TRUE);
		pClient->status = CLIENT_LEFT;					// client is left

		// Designer can use battle script to set no punishment when the player died
		if (pClient->pBatChar && pClient->pBatChar->GetLevel() < 10)
		{
			bPunish = BOOLEAN_FALSE;
			pBatChar->SetMHP(pBatChar->GetMaxHP(), 0, 0);
			pBatChar->SetHP(pBatChar->GetMHP(), 0, 0);		// restore character HP/SP
			pBatChar->SetSP(pBatChar->GetMSP(), 0, 0);
		}
		else
		{
			pBatChar->SetMHP(pBatChar->GetMaxHP(), 0, 0);
			pBatChar->SetHP((UInt16)(pBatChar->GetMHP() * 0.1), 0, 0);		// restore character HP/SP
			pBatChar->SetSP((UInt16)(pBatChar->GetMSP() * 0.1), 0, 0);
		}
		if (!pBatChar->m_jumpBack && m_bJumpReborn && pClient->pBatChar->GetLevel() >= 10)
			pBatChar->m_jumpBack = true;
		
		CallLuaFunc(gGlobal.g_pLuaState, "DeadPenaltyCheck", String(), (CBatActor *)pBatChar);

		/*if(pOwner->GetPK_Value() > 0)
		{
			if(HasLuaFunc(gGlobal.g_pLuaState, "OnCharacterImprison"))
			{
				CallLuaFunc(gGlobal.g_pLuaState, "OnCharacterImprison", String(), pOwner);
				UInt32 result = (UInt32)gGlobal.g_TempResult;
				
				if(result)
				{
					UInt32 nReward = pOwner->GetWantedReward();
					if(( m_winTeam == PLAYER_TEAM || m_winTeam == ENEMY_TEAM ) && m_teamData[m_winTeam].pLeader)
					{
						
						m_teamData[m_winTeam].pLeader->ChangeMoney((pOwner->GetPK_Value()*500 + nReward));
						String msg;
						msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_RUMOR_WANTED_CATCH")), m_teamData[m_winTeam].pLeader->GetNickName().c_str(),pOwner->GetNickName().c_str(),pOwner->GetPK_Value()*500 + nReward);
						MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
							RPCContext(), msg, 0, 1);
					}
					else if(pOwner->GetPK_Value() > 5)
					{
						String msg;
						msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_RUMOR_WANTED_GOVERNMENT_CATCH")),pOwner->GetNickName().c_str());
						MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
							RPCContext(), msg, 0, 1);
					}
					pOwner->UpdateWantedReward(pOwner->GetChar_id(),-nReward);
					pBatChar->SetJumpBack(false);
				}
			}
		}*/

	
		Boolean bHasSafeguardCard = BOOLEAN_FALSE;
		if(HasLuaFunc(gGlobal.g_pLuaState, "GetSafeguardCardId"))
		{
			CallLuaFunc(gGlobal.g_pLuaState, "GetSafeguardCardId", String(), pOwner);
			UInt32 result = (UInt32)gGlobal.g_TempResult;
			if(pOwner->GetItems()->RemoveItem(result))
				bHasSafeguardCard = BOOLEAN_TRUE;
		}
		if (bPunish && pBatChar->m_punish && !bHasSafeguardCard)
		{
			// decrease exp by 10% of level different
			UInt32 expDec = 0;
			UInt32 moneyDec = 0;
			Int16 honorDec = 0;

			if (pBatChar->GetLevel() < CHAR_MAX_LEVEL - 1)		// not at max level
			{
				// 10% exp decrease
				//expDec = RoundToUInt32(pOwner->GetNextLvExp() * 0.01f);
				if (GetBattleType() == NPC_BATTLE)
					expDec = RoundToUInt32(pOwner->GetNextLvExp() * 0.01f);
				else if (GetBattleType() == PK_BATTLE || GetBattleType() == FORCE_PK_BATTLE || GetBattleType() == FREE_PK_BATTLE)
					expDec = RoundToUInt32(pOwner->GetNextLvExp() * 0.03f);

				if (expDec > pOwner->GetExp())
					expDec = pOwner->GetExp();

				pOwner->ChangeExp(- (Int32) expDec, LogService_Proxy::Money_And_Exp::SOURCE_BATTLE_DEAD);
			}

			// decrease money by 10%
			if (GetBattleType() == NPC_BATTLE)
				moneyDec = RoundToUInt32(pOwner->GetMoney() * 0.05f);
			else if (GetBattleType() == PK_BATTLE || GetBattleType() == FORCE_PK_BATTLE || GetBattleType() == FREE_PK_BATTLE)
				moneyDec = RoundToUInt32(pOwner->GetMoney() * 0.05f);
			if (moneyDec > pOwner->GetMoney())
				moneyDec = pOwner->GetMoney();
			pOwner->ChangeMoney(- (Int32) moneyDec, LogService_Proxy::Money_And_Exp::SOURCE_BATTLE_DEAD);

			//error show
			StringW s;
			s.Format(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_FAILSHOW")), expDec, moneyDec);
			pOwner->CB_Error(pOwner->GetPeer(), pOwner->GetContext(), 1, s);

			// tell client what penalty
			if (pClient->pOwner)
				CB_Penalty(pClient->pOwner->GetPeer(), pClient->pOwner->GetContext(), 
					expDec, moneyDec, honorDec);
		}
	}

	else if (GetBattleType() == GUILD_WAR_BATTLE)
	{
		pOwner->ChangeVP(-(pOwner->GetMaxVP() * 25 / 100 + 50));
		pOwner->m_battleRestTime = ::timeGetTime() + GUILD_WAR_REST_TIME;
	}
			
	// update character exp, honor and money after penalty
	UpdateToMap(pClient, END_LOSE, pBatChar->m_jumpBack);
}

void CBattle::DrawnGame(BatClient *pClient)
{
	if (pClient == NULL || pClient->pBatChar == NULL || pClient->pOwner == NULL)
		return;

	CBatCharacter *pBatChar = pClient->pBatChar;
	CBatMob* pPartner = pClient->pPartner;
	CCharacter *pOwner = pClient->pOwner;

	pBatChar->SetLeftBattle(BOOLEAN_TRUE);
	if (pPartner)
		pPartner->SetLeftBattle(BOOLEAN_TRUE);
	pClient->status = CLIENT_LEFT;		// client is left
	
	if (pBatChar->IsDead())
	{
		pBatChar->SetHP(1, 0, 0);		// restore character HP/SP
		pBatChar->SetSP(1, 0, 0);
	}
	
	Int moneyEarn = 0;
	if (pOwner)
	{
		if (GetBattleType() == LOW_MARTIAL_BATTLE)		
		{
			pOwner->ChangeMoney(LOW_MARTIAL_MONEY - LOW_MARTIAL_MONEY_TAX);		
			moneyEarn = LOW_MARTIAL_MONEY - LOW_MARTIAL_MONEY_TAX;
			CB_MartialResult(pClient->pOwner->GetPeer(), pClient->pOwner->GetContext(), END_DRAW, moneyEarn, 0);
		}
		else  if (GetBattleType() == HIGH_MARTIAL_BATTLE)
		{
			pOwner->ChangeMoney(HIGH_MARTIAL_MONEY - HIGH_MARTIAL_MONEY_TAX);	
			moneyEarn = HIGH_MARTIAL_MONEY - HIGH_MARTIAL_MONEY_TAX;
			CB_MartialResult(pClient->pOwner->GetPeer(), pClient->pOwner->GetContext(), END_DRAW, moneyEarn, 0);
		}

		
	}
	UpdateToMap(pClient, END_DRAW, false);
}

void CBattle::CharEscaped(BatClient *pClient)
{
	if (pClient == NULL || pClient->pBatChar == NULL || pClient->pOwner == NULL)
		return;

	CBatCharacter *pBatChar = pClient->pBatChar;
	CBatMob* pPartner = pClient->pPartner;
	UInt8 team = pBatChar->GetTeam();
	UInt8 posIdx = pBatChar->GetPosition() - 5;
	CCharacter* pOwner = pClient->pOwner;

	TRACE_INFODTL_8(GLOBAL_LOGGER, 
		_F("battle %d:%d:%d, char_id %d, leader %d, actor %d, posIdx %d, partner %d"),
		GetID(), GetNetId().srvId, GetNetId().objId,
		pClient->pOwner ? pClient->pOwner->GetChar_id() : -1,
		m_teamData[team].pLeader ? m_teamData[team].pLeader->GetChar_id() : -1,
		pBatChar->GetIndex(), posIdx,
		pClient->pPartner ? pClient->pPartner->GetPartner_id() : 0);

	// add pk value
	if (GetBattleType() == FORCE_PK_BATTLE && pOwner!= NULL  && pBatChar->GetTeam() == 1)
	{
		int nKillCount = 0;
		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
		{
			CBatActor* actor = FindNetObject<CBatActor>(*it);
			if(actor == NULL) 
				continue;
			if (actor->m_pBatChar && actor->GetTeam() != pBatChar->GetTeam() && actor->IsDead()&&actor->GetClassID() == BatCharacter_CLASSID)
				nKillCount ++;
		}
		pBatChar->SetPK_Value(MIN(20,pBatChar->GetPK_Value() + nKillCount));
		pOwner->SetPK_Value(MIN(20,pOwner->GetPK_Value() + nKillCount));
	}

	// call script OnEscape method if defined
	if ((m_bNpcScript|| m_bPvPScript) && m_pScriptSys->HasBatMethod(BAT_ON_ESCAPE))
		m_pScriptSys->CallBatMethod(BAT_ON_ESCAPE, m_curTurn, pBatChar);

	pBatChar->SetLeftBattle(BOOLEAN_TRUE);
	if (pPartner)
		pPartner->SetLeftBattle(BOOLEAN_TRUE);
	pClient->status = CLIENT_LEFT;		// client is left

	// reset team positional index
	m_teamData[team].posIndex[posIdx] = 255;

	// leader left, change leader to next member in party
	if (pBatChar->m_pOwner) {
		CCharacter *pChar = pBatChar->m_pOwner;
		// prevent owner left and script crash
		if (m_pScriptSys && m_pScriptSys->GetOwner() == pChar)
			m_pScriptSys->SetOwner(NULL);
		if (pChar == m_teamData[team].pLeader)
		{
			if (pChar->GetParty())
				m_teamData[team].pLeader = pChar->GetParty()->GetMember(1);
			else
				m_teamData[team].pLeader = NULL;

			// change script system owner when player team leader left
			if (m_pScriptSys && team == PLAYER_TEAM)
				m_pScriptSys->SetOwner(m_teamData[PLAYER_TEAM].pLeader);

			if (m_teamData[team].pLeader)
			{
				TRACE_INFODTL_6(GLOBAL_LOGGER, 
					_T("Change team %d Leader: battle %d:%d:%d, old char_id %d, new char_id %d"),
					team, GetID(), GetNetId().srvId, GetNetId().objId, pChar->GetChar_id(),
					m_teamData[team].pLeader->GetChar_id());
			}
		}

		if (GetBattleType() == GUILD_WAR_BATTLE)
		{
			pChar->ChangeVP(-(pChar->GetMaxVP() * 25 / 100 + 50));
			pChar->m_battleRestTime = ::timeGetTime() + GUILD_WAR_REST_TIME;
		}
	}
	// decrease client team index
	//for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	//{
	//	CBatActor *actor = *it;
	//	if (actor->GetLeftBattle() || actor->GetTeam() != pBatChar->GetTeam() || 
	//		actor->m_pBatChar == NULL || actor->m_pClient == NULL)
	//		continue;

	//	//actor->m_pClient->teamIndex --;
	//}
	
	m_teamData[pClient->team].playerCount --;
#ifdef LINEUP_SYSTEM_ON
	if (m_teamData[team].bDoLineupBuffer)
		CancleLineupBuffer(team);
#endif
	// update only character hp/sp, state and buff
	UpdateToMap(pClient, END_ESCAPED, false);
}

void CBattle::RemoveMonitor(CCharacter* pChar)
{
	if (pChar == NULL)
		return;
	BatClient* pClient = GetClient(pChar->GetSessionId());
	if (pClient)
	{
		RemoveMonitor(pClient);
	}
}

void CBattle::RemoveMonitor(BatClient* pClient)
{
	if (pClient == NULL || !pClient->IsMonitor() || pClient->pOwner == NULL)
		return;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("sessionId %s, status %d"), 
		pClient->pOwner->GetContext().sessionId.GetString().c_str(), pClient->status);

	pClient->status = CLIENT_LEFT;
	pClient->pOwner->ChangeState(CCharacter::ST_NOBATTLE);
	pClient->pOwner->m_BattleNetID = RPCNetID();
	pClient->pOwner->m_pBatChar = NULL;
	if (pClient->IsConnect())
		CB_EndBattle(pClient->pOwner->GetPeer(), pClient->pOwner->GetContext(), END_MONITOR);
	if (pClient->pOwner->CheckNeedJumpReborn())
	{
		pClient->pOwner->JumpReborn();
		CB_Error(pClient->pOwner->GetPeer(), pClient->pOwner->GetContext(), 0, _T("MSG_MAP_FORCE_LEAVE"));
	}
	m_totalMonitor --;
}

void CBattle::NotifyInput(CBatActor *actor)
{
	if (actor == NULL)
		return;

	if (!CanInput())
		return;

	TRACE_VERBOSEDTL_7(GLOBAL_LOGGER, _F("CBattle NotifyInput: Battle netId %d:%d, ID %d, actor %d, command %d, target %d, subtype %d"),
		GetNetId().srvId, GetNetId().objId, actor->GetID(), actor->GetIndex(), actor->m_command, actor->m_cmdTarget, actor->m_cmdSubType);
	//actor->m_bInput = BOOLEAN_TRUE;
	actor->SetIsInput(BOOLEAN_TRUE);

	// check all clients inputed
	Boolean allInput = BOOLEAN_TRUE;
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		if (!client.IsBattle() || !client.IsConnect())
			continue;

		// if existing character or partner not input
		//	player still can input when he is dead
		if (client.pBatChar && !client.pBatChar->m_bInput && 
			client.pBatChar->IsValidTarget(false) ||
			client.pPartner && !client.pPartner->m_bInput && 
			client.pPartner->IsValidTarget())
			allInput = BOOLEAN_FALSE;

		// notify other this actor has inputed
		if (client.pBatChar != actor && client.pPartner != actor)
			actor->CB_Inputed(client.pOwner->GetPeer(), client.pOwner->GetContext(), 
				actor->m_command);
	}

	// play turn when all clients are inputed
	if (allInput)
	{
		TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("CBattle NotifyInput (Call PlayTurn): Battle %d:%d:%d"), GetID(), GetNetId().srvId, GetNetId().objId);
		CancelUniqEvent(EVT_CMD_TIMEOUT);		// cancel command timeout
		RunTurn();
	}
}

void CBattle::TeamEscaped(UInt8 team)
{
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		if (client.IsLeft()) continue;		// skip left character
		if (client.pBatChar && client.pBatChar->GetTeam() == team) {
			if (client.pBatChar->IsValidTarget()) {
				client.pBatChar->SetState(CHAR_ST_ESCAPED);
			}
		}
	}
}

CBatActor* CBattle::GetProtector(CBatActor *attacker, CBatActor *target, Boolean bCritHit)
{
	TRACE_ENSURE(attacker != NULL && target != NULL);

	if (!attacker->IsProtectActive())
		return NULL;

	UInt16 highHp = 0;
	CBatActor* protector = NULL;

	// check any protector which have highest hp
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor *actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL) 
			continue;
		if (actor == target || !actor->IsValidTarget()) continue;

		if (!actor->m_bAction && actor->m_command == BATCMD_PROTECT && 
			actor->m_cmdTarget == target->GetIndex() && actor->m_protectCount == 0 && 
			actor->GetHP() > highHp)
		{			
			protector = actor;
			highHp = actor->GetHP();			
		}
	}

	// add by zc 2009 - 9 - 3 // open after get friendly
// 	if (!protector)
// 	{
// 		// check any friend's protecting which have highest hp
// 		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
// 		{
// 			CBatActor *actor = *it;
// 			if (actor == target || !actor->IsValidTarget()) continue;
// 
// 			if (
// 				target->m_command != BATCMD_PROTECT
// 				&& actor->m_protectCount == 0 
// 				&& actor->GetHP() > highHp
// 				&& ((!target->m_ignoreTeam) && (!actor->m_ignoreTeam) && target->GetTeam() == actor->GetTeam() )
// 				)
// 			{	
//  				gGlobal.g_TempResult = 0;
//  				CallLuaFunc(gGlobal.g_pLuaState, "FriendProtectActive", String(), target);
//  				if (gGlobal.g_TempResult > 0)
// 				{
// 					protector = actor;
// 					highHp = actor->GetHP();
// 				}
// 			}
// 		}
// 	}

	// check any protector by assisting relation

	//check pet protector

	if (protector)
		protector->m_protectCount ++;

	if (protector == NULL && target->m_pBatChar && 
		target->m_pBatChar->GetOwner()->GetPet_uid() > 0)
	{
		gGlobal.g_TempResult = 0;
		CallLuaFunc(gGlobal.g_pLuaState, "PetProtectActive", String(), target);
		if (gGlobal.g_TempResult > 0)
		{
			protector = target;
			target->PetActive();
		}
	}

	if (protector && protector != target) 
		target->m_pProtector = protector;
	return protector;
}

void CBattle::PrepareSendGroup(NetGroup &grp, UInt8 teamNo)
{
	grp.Clear();
	// set send mask and add to NetGroup to send
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor* actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL) 
			continue;
		if (actor->m_pBatChar)
		{
			if (teamNo == actor->m_pBatChar->GetTeam())
				actor->m_pBatChar->PreSend();
			else
				actor->m_pBatChar->PreSendEnemy();
		}
		else if (actor->IsParnter() && actor->m_pMob)
		{
			if (teamNo == actor->m_pMob->GetTeam())
				actor->m_pMob->PreSend();
			else
				actor->m_pMob->PreSendEnemy();
		}
		else if (actor->GetLeftBattle()) {
			RPCMask mask;
			mask.MaskBits(CBatActor::indexMask, CBatActor::teamMask, CBatActor::leftBattleMask, -1);
			actor->SetSendMask(mask);
		} 		
		else if (actor->m_pMob)
		{
			if (teamNo == actor->m_pMob->GetTeam())
				actor->m_pMob->PreSend();
			else
				actor->m_pMob->PreSendEnemy();
		}
		else
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("PrepareSendGroup: invalid actor classId %d"), 
				actor->GetClassID());
			continue;
		}
		grp.Add(actor);
	}
}

Boolean CBattle::CheckCombo(SpeedOrder &orderList, SpeedOrder::iterator &it)
{
	CBatActor *actor = it->second;
	CBatActor *pTarget = GetActor(actor->m_cmdTarget);
	SpeedOrder::iterator forwardIt;		// foward iterator
	Boolean bCombo = BOOLEAN_FALSE;

	// look forward to check if combo attack happen
	if (pTarget && actor->IsHitTarget(pTarget)) // is vliad and is my friend
	{
		Actors fellowList;						// fellow actor list for combo
		UInt16 ispeed = (UInt16) it->first;		// convert to integer speed
		forwardIt = it;
		++forwardIt;							// advance next actor
		while (forwardIt != orderList.end())	// check speed slower player
		{
			CBatActor *pOther = forwardIt->second;
			// stop checking if not same speed in integer <integer>
			if (UInt16(forwardIt->first) != ispeed)
				break;

			gGlobal.g_TempResult = 1;
			CallLuaFunc(gGlobal.g_pLuaState, validActionTargetStr.c_str(), String(), pOther, pTarget);
			if (gGlobal.g_TempResult > 0 &&
				pOther->IsValidTarget() && !pOther->m_bAction && // actor can do attack?
				pOther->m_command == BATCMD_ATTACK &&			// same attack?
				pOther->GetTeam() == actor->GetTeam() &&		// same team?
				pOther->m_cmdTarget == actor->m_cmdTarget &&	// same target?
				pOther->IsHitTarget(pTarget) &&					// also hit target?
				GetRand(0.0f, 1.0f) < (float)gGlobal.g_ComboRate / 100.0f
				)					// 10 % random
				
			//if (GetRand(0.0f, 1.	0f) < (float)gGlobal.g_ComboRate / 100.0f)					// 10 % random
			{
				pOther->m_command = BATCMD_IDLE;		// idle command after combo attack
				fellowList.push_back(pOther->GetNetId());
			}
			++forwardIt;						// advance next actor
		}

		// combo attack happen if fellow list not empty
		if (!fellowList.empty())
		{
			bCombo = BOOLEAN_TRUE;
			actor->DoComboAttack(fellowList);
		}
	}

	return bCombo;
}

Boolean CBattle::CheckForceEnd()
{
	// check battle win one team all dead or escaped
	UInt8 teamExist[2] = {0, 0};
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor *actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL) 
			continue;
		if (actor->IsValidTarget() || (actor->m_pMob && actor->m_pMob->IsNew()))
		{
			if (!actor->IsGuest())		// all kind of actor except guest
				teamExist[actor->GetTeam()] ++;
		}
	}

	if (teamExist[0] == 0 || teamExist[1] == 0)
		m_forceEnd = BOOLEAN_TRUE;

	return m_forceEnd;
}

Boolean CBattle::IsAllLeft()
{
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		if (!it->second.IsLeft() || it->second.pBatChar != NULL)
			return BOOLEAN_FALSE;
	}
	return BOOLEAN_TRUE;
}

Boolean CBattle::ChangeState(CONST StateID state)
{
	switch (state)
	{
	case ST_PREPARE:
		return State::ChangeState(m_state, SafeCreateObject(CPrepareState));
	case ST_PRETURN:
		return State::ChangeState(m_state, SafeCreateObject(CPreTurnState));
	case ST_WAITCMD:
		return State::ChangeState(m_state, SafeCreateObject(CWaitCmdState));
	case ST_PLAYACTION:
		return State::ChangeState(m_state, SafeCreateObject(CPlayActionState));
	case ST_POSTTURN:
		return State::ChangeState(m_state, SafeCreateObject(CPostTurnState));
	case ST_FINISH:
		return State::ChangeState(m_state, SafeCreateObject(CFinishState));
	}

	return BOOLEAN_FALSE;
}

Boolean CBattle::IsState(CONST StateID state)
{
	switch (state)
	{
	case ST_PREPARE:
	case ST_PRETURN:
	case ST_WAITCMD:
	case ST_PLAYACTION:
	case ST_POSTTURN:
	case ST_FINISH:
		return (m_state->GetStateID() == state);
	}

	return BOOLEAN_FALSE;
}

void CBattle::ClientDisconnect(const SessionID &sid)
{
	BatClient* pClient = GetClient(sid);
	if (pClient == NULL) {
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("client session not found, sessionId %s, battle %d:%d:%d"),
			sid.GetString().c_str(), GetID(), GetNetId().srvId, GetNetId().objId);
		return;
	}
	
	if (IsState(ST_FINISH))	// nothing do when battle is finished
		return;

	if (pClient->IsMonitor()) {			// remove monitor
		RemoveMonitor(pClient);
		return;
	}

	pClient->bAuto = BOOLEAN_TRUE;
	//pClient->autoCount = 255;
	pClient->autoCount = 65535;
	if (pClient->pBatChar)
	{
		TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("CBattle ClientDisconnect (Set Auto Input) client %d"), pClient);
		pClient->pBatChar->SetAuto(pClient);
		pClient->pBatChar->DefaultInput();
	}

	// if server waiting client to EndTurn, make a fake end turn
	if (pClient->IsBattle() && IsState(ST_PLAYACTION) && !pClient->bEndTurn) 
	{
		TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("CBattle ClientDisconnect (Call EndTurn) client %d"), pClient);
		EndTurn(NULL, pClient->pOwner->GetContext());
	}
}

void CBattle::ChangeSessionId(const SessionID &oldSessId, const SessionID &sid)
{
	if (oldSessId == sid)
		return;

	BatClient* pClient = GetClient(oldSessId);
	if (pClient == NULL) {
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("client session not found, sessionId %s, battle %d:%d:%d"),
			oldSessId.GetString().c_str(), GetID(), GetNetId().srvId, GetNetId().objId);
		return;
	}

	m_clients[sid] = *pClient;
	pClient = &m_clients[sid];
	pClient->pOwner->GetContext().sessionId = sid;

	if (pClient->pBatChar)
		pClient->pBatChar->m_pClient = pClient;
	if (pClient->pPartner)
		pClient->pPartner->m_pClient = pClient;
	m_clients.erase(oldSessId);
}

Boolean CBattle::ResumeClient(const SessionID &sid)
{
	BatClient* pClient = GetClient(sid);
	if (pClient == NULL) {
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("client session not found, sessionId %s, battle %d:%d:%d"),
			sid.GetString().c_str(), GetID(), GetNetId().srvId, GetNetId().objId);
		return BOOLEAN_FALSE;
	}

	if (!pClient->IsBattle() && !pClient->IsWaiting())
	{
		TRACE_WARNDTL_5(GLOBAL_LOGGER, _FW("no resume battle ID %d:%d:%d, sessionId %s, client left status %d"),
			GetID(), GetNetId().srvId, GetNetId().objId, sid.GetString().c_str(), pClient->status);
		return BOOLEAN_FALSE;
	}

	UInt16 stateId = m_state->GetStateID();
	if (stateId == ST_PREPARE || stateId == ST_FINISH)
	{
		TRACE_WARNDTL_5(GLOBAL_LOGGER, _FW("no resume, battle %d:%d:%d, sessionId %s, battle is preparing or already finished, status %d"),
			GetID(), GetNetId().srvId, GetNetId().objId, sid.GetString().c_str(), stateId);
		return BOOLEAN_FALSE;
	}

	if (pClient->pBatChar == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("Cinvalid client character, sessionId %s"),
			sid.GetString().c_str());
		return BOOLEAN_FALSE;
	}

	RPCMask charMask;
	NetGroup grp;
	LPCPEER pPeer = pClient->pOwner->GetPeer();
	RPCContext &context = pClient->pOwner->GetContext();

	PrepareSendGroup(grp, pClient->pBatChar->GetTeam());			// prepare NetGroup to send

	TRACE_INFODTL_7(GLOBAL_LOGGER, _F("battle %d:%d:%d, char_id %d, sessionId %s, actor count %d, send count %d"),
		GetID(), GetNetId().srvId, GetNetId().objId, pClient->pBatChar->GetChar_id(), sid.GetString().c_str(), m_actors.size(), grp.GetCount());

	charMask.MaskBits(CBatCharacter::AllMask, -1);
	pClient->pBatChar->MarkSend(charMask);
	SetSendMask(AllMask);
	MapService_Stub::CB_EnterBattle(pPeer, context, this, &grp, pClient->team,
		BATTLE_STATE_IN_WAR, m_npcId);

	pClient->pBatChar->UnmarkSend(charMask);

	if (pClient->IsBattle())
	{
		if (pClient->pBatChar)
		{
			// try to stop auto at next turn
			if (IsState(ST_WAITCMD) && pClient->pBatChar->m_bInput)
			{
				pClient->bAuto = BOOLEAN_FALSE;
				pClient->autoCount = 0;
			}
			else
			{
				pClient->autoCount = 1;
			}
			CB_ControlCharacter(pPeer, context, pClient->pBatChar->GetIndex());
		}
		if (pClient->pPartner)
			CB_ControlPartner(pPeer, context, pClient->pBatChar->GetIndex(), pClient->pPartner->GetIndex());
	}

	return (BOOLEAN_TRUE);
}

void CBattle::ShowAllDetail()
{
	Actors::iterator it;
	String s;

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Show Battle netId %d:%d: actor count %d"),
		GetNetId().srvId, GetNetId().objId, m_actors.size());

	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		CBatActor * actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL) 
			continue;
		if (actor->m_pMob)			// show mob or partner
		{
			CBatMob *pmob = actor->m_pMob;

			if (pmob->GetPartner_id() == 0)					// mob
				s.Format(_T("Mob %d: objId %d, mob_id %d"),
					pmob->GetIndex(), pmob->GetID(), pmob->GetMob_id());
			else
				s.Format(_T("Partner %d: objId %d, partner_id %d, owner_id %d"),
					pmob->GetIndex(), pmob->GetID(), pmob->GetPartner_id(), 
					pmob->GetOwner_id());
			s.AppendFormat(_T(", level %d, hp %d/%d, sp %d/%d, state %d"),
				pmob->GetLevel(), pmob->GetHP(), pmob->GetMHP(), pmob->GetSP(), 
				pmob->GetMSP(), pmob->GetState());
			TRACE_INFOL(GLOBAL_LOGGER, s.c_str());
			TRACE_INFOL_6(GLOBAL_LOGGER, _F("       ATK %d, DEF %d, SPD %d, POW %d, HIT %d, EVA %d"),
				pmob->GetATK(), pmob->GetDEF(), pmob->GetSPD(), pmob->GetPOW(),
				pmob->GetHIT(), pmob->GetEVA());
		}
		else if (actor->m_pBatChar) // show character
		{
			CBatCharacter *pchar = actor->m_pBatChar;
			TRACE_INFOL_9(GLOBAL_LOGGER, _F("Character %d: sessionId %s, objId %d, char_id %d, level %d, hp %d/%d, sp %d/%d"),
				pchar->GetIndex(), pchar->m_pOwner->GetSessionId().GetString().c_str(), 
				pchar->GetID(), pchar->GetChar_id(), pchar->GetLevel(), pchar->GetHP(), 
				pchar->GetMHP(), pchar->GetSP(), pchar->GetMSP());
			TRACE_INFOL_6(GLOBAL_LOGGER, _F("ATK %d, DEF %d, SPD %d, POW %d, HIT %d, EVA %d"),
				pchar->GetATK(), pchar->GetDEF(), pchar->GetSPD(), pchar->GetPOW(),
				pchar->GetHIT(), pchar->GetEVA());
		}
	}
}

void CBattle::PushReactionActor(CBatActor * pBatActor)
{
	if (pBatActor == NULL)
		return;

	m_SkillReactionsMap.push_back(pBatActor->GetNetId()); 
}

void CBattle::PushExtraActor(CBatActor * pBatActor)
{
	if (pBatActor == NULL)
		return;

	m_ExtraActorMap.push_back(pBatActor->GetNetId()); 
}

RPCResult CBattle::ChooseResult(LPCPEER pPeer, RPCContext &context, const UInt8 &ans)
{
	// check if script and is leader player
	if (m_pScriptSys == NULL || m_talkNum == 0 ||
		m_teamData[PLAYER_TEAM].pLeader == NULL ||
		m_teamData[PLAYER_TEAM].pLeader->GetSessionId() != context.sessionId)
		return RPC_RESULT_FAIL;

	m_talkNum --;
	if (m_talkNum == 0)
		CancelUniqEvent(EVT_TALK_TIMEOUT);
	m_pScriptSys->ReplyAnswer(pPeer, context, ans);

	if (m_talkNum > 0)
		WaitTalk();
	else
		RunTurn();

	return RPC_RESULT_OK;
}

void CBattle::BatSay(PCSTRINGA msg, int imageType, bool bLeft, UInt32 npc_id, 
					 UInt8 index)
{
	StringW s;
	UTF8ToStr(msg, s);

	if (imageType == 0 && npc_id == 0)
		npc_id = m_npcId;

	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		if (!client.IsBattle() || !client.IsConnect())
			continue;

		if (index == 255 || client.pBatChar->GetIndex() == index)
		{
			CB_Say(client.pOwner->GetPeer(), client.pOwner->GetContext(), s, 
				imageType, bLeft, npc_id);
		}
	}
	m_talkNum ++;
}

void CBattle::InitScript(CCharacter* pOwner, const RPCNetID &netId)
{
	m_pScriptSys->SetOwner(pOwner);
	if (m_pNpcData && m_pNpcData->battleScript && m_pNpcData->battleScript[0] != _T('\0'))
		m_bNpcScript = m_pScriptSys->ExecuteBattleScript(m_pNpcData->npc_id, netId);
}

void CBattle::AddMob(UInt32 mob_id, UInt8 pos, UInt8 level, UInt8 team, bool bGuest)
{
	if (gGlobal.m_resource->GetMobData(mob_id) == NULL) 
		return;

	if (m_actors.size() < 255) {
		CBatMob* pMob = CBatMob::NewInstance();
		if (pMob) {
			if (IsState(ST_PREPARE))
			{
				if(pMob->GetOwner() && pMob->GetOwner()->GetModel_mobid() >0)
				{
					String msg;
					msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_CHANGEMOB_NOT_USEPARTNRT")));
					pMob->GetOwner()->CB_PetUseSkillMsg(pMob->GetOwner()->GetPeer(), pMob->GetOwner()->GetContext(), msg);
					return;
				}
			}
			pMob->SetBattle(this);
			const MobData* mobData = gGlobal.m_resource->GetMobData(mob_id);
			if( mobData )
			{
				if (level > 0)
					pMob->SetData(mob_id, level);
				else if( mobData->mob_type==1 )
				{
					pMob->SetData(mob_id, GetTeamAvgLevel(PLAYER_TEAM));
				}
				else
				{
					pMob->SetData(mob_id);
				}
			} 
			
			pMob->SetTeam(team);
			pMob->SetPosition(pos);
			pMob->SetGuest(bGuest);
			pMob->MobLearnSkill();
			if (IsState(ST_PREPARE))
				InsertActor(pMob);
			else
				AddWaiting(pMob);
		}
	} else {
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CBattle AddMob , not enough space to add mob"));
	}
}

void CBattle::AddGuest(UInt32 mob_id, UInt8 pos)
{
	AddMob(mob_id, pos, 0, PLAYER_TEAM, true);
}

Boolean CBattle::IsGuestExist(UInt8 pos)
{
	CBatActor* actor = NULL;
	
	if (pos < TEAMPOS_MAX && m_teamData[PLAYER_TEAM].posIndex[pos] != 255)
		actor = GetActor(m_teamData[PLAYER_TEAM].posIndex[pos]);

	return (actor && actor->IsMob() && !actor->IsParnter() && !actor->GetLeftBattle() &&
			!actor->IsDead());
}

UInt8 CBattle::GetTeamExist(UInt8 team)
{
	if (team > 1)
		return 0;
	return m_teamData[team].existCount;
}

UInt16 CBattle::GetTeamAvgLevel(UInt8 team)
{
	UInt16 level = 0;
	UInt16 num = 0;
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		BatClient &client = it->second;
		if (client.IsBattle() && client.team == team)
		{
			level += client.pBatChar->GetLevel();
			++num;
		}
	}

	return (num != 0 ? level / num : 0);
}

void CBattle::SetAvailPos(CBatActor* pActor)
{
	if (pActor == NULL)
		return;

	UInt8 team = pActor->GetTeam();
	for (UInt8 i = 0; i < TEAMPOS_MAX; i++) {
		if (m_teamData[team].posIndex[i] == 255) {
			m_teamData[team].posIndex[i] = pActor->GetIndex();
			pActor->SetPosition((i + TEAMMATE_MAX) % TEAMPOS_MAX);
		}
	}
}

CBatCharacter* CBattle::GetTeamLeader(UInt8 team)
{
	if (team >= 2)
		return NULL;

	CCharacter *pLeader = m_teamData[team].pLeader;
	if (pLeader == NULL)
		return NULL;

	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor* actor = FindNetObject<CBatActor>(*it);
		if (actor == NULL)
			continue;
		if (actor->m_pBatChar && actor->m_pBatChar->m_pOwner == pLeader)
			return CastNetObject<CBatCharacter>(actor);
	}
	return NULL;
}

CBatMob* CBattle::GetMob(UInt8 index)
{
	UInt8 count = 0;

	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor* actor = FindNetObject<CBatActor>(*it);
		if (actor == NULL)
			continue;
		if (actor->IsMob() && actor->m_pMob && !actor->GetLeftBattle()) {
			if (index == count)
				return actor->m_pMob;
			count ++;
		}
	}

	return NULL;
}

UInt16 CBattle::GetMobCount()
{
	UInt16 count = 0;

	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor* actor = FindNetObject<CBatActor>(*it);
		if (actor == NULL)
			continue;
		if (actor->IsMob() && actor->m_pMob && !actor->GetLeftBattle())
			count ++;
	}

	return count;
}

CBatCharacter* CBattle::GetCharacter(UInt8 index)
{
	UInt8 count = 0;

	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor* actor = FindNetObject<CBatActor>(*it);
		if (actor == NULL)
			continue;
		if (actor->m_pBatChar && !actor->GetLeftBattle()) {
			if (index == count)
				return actor->m_pBatChar;
			count ++;
		}
	}

	return NULL;
}

UInt16 CBattle::GetCharacterCount()
{
	UInt16 count = 0;

	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor* actor = FindNetObject<CBatActor>(*it);
		if (actor == NULL)
			continue;
		if (actor->m_pBatChar && !actor->GetLeftBattle())
			count ++;
	}

	return count;
}

RPCResult CBattle::SendBattleMessage(LPCPEER pPeer, RPCContext &context, const RPCNetID &charNetID, const String &Msg)
{
	CCharacter* pOwner = FindNetObject<CCharacter>(charNetID);
	if (pOwner && pOwner->GetPeer())
	{
		time_t now = time(NULL);
		if ((UInt32)now < SPEAK_REST_TIME + pOwner->m_PreSpeakTime)
		{
			pOwner->CB_SendGrpMessage(pOwner->GetPeer(), pOwner->GetContext(), MSG_RETURN_FAIL_TOO_FREQUENT, SPEAK_REST_TIME + pOwner->m_PreSpeakTime - (UInt32)now);
			return RPC_RESULT_OK;
		}
		else if (pOwner->GetSpeakBlock() > (UInt32)now)
		{
			pOwner->CB_SendGrpMessage(pOwner->GetPeer(), pOwner->GetContext(), MSG_RETURN_FAIL_NO_RIGHT_BLOCK, 0);
			return RPC_RESULT_OK;
		}
		//if (pOwner->m_viewManager)
		{
			ProxyListenersMap proxys;
			for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
			{
				BatClient &client = it->second;
				if (!client.pOwner)
					continue;

				if (!client.IsBattle() || !client.IsConnect())
					continue;

				if (client.pOwner && !client.pOwner->m_Listen)
					continue;

				if (client.pOwner->GetPeer())
					proxys[(client.pOwner)->GetPeer()].push_back((client.pOwner)->GetContext().sessionId);
			}
			String blockedMsg = Msg;
			gGlobal.BlockWord(blockedMsg);

			for (ProxyListenersMap::iterator it = proxys.begin(); it != proxys.end(); ++it)
				MapService_Stub::CB_ReceiveMapMessage(it->first, RPCContext(), blockedMsg, pOwner->GetNickName(), it->second);
			pOwner->m_PreSpeakTime = (UInt32)now;
		}
	}
	return RPC_RESULT_OK;
}

UInt32 CBattle::SetLineupId(UInt32 uiLineupId,int iTeamIndex)
{
	if (iTeamIndex >=0 && iTeamIndex < 2) {
		m_teamData[iTeamIndex].uiLineupId = uiLineupId;
		return uiLineupId;
	}
	return UINT_MAX;
}
UInt32 CBattle::GetLineupId(int iTeamIndex) const
{
	if (iTeamIndex >=0 && iTeamIndex < 2)
		return m_teamData[iTeamIndex].uiLineupId;
	return UINT_MAX;
}
int CBattle::SetLineupKeyPos(int iKeyPos,int iTeamIndex)
{
	if (iTeamIndex >=0 && iTeamIndex < 2) {
		m_teamData[iTeamIndex].iLineupKeyPos = iKeyPos;
		return iKeyPos;
	}
	return INT_MAX;
}
int CBattle::GetLineupKeyPos(int iTeamIndex) const
{
	if (iTeamIndex >=0 && iTeamIndex < 2) 
		return m_teamData[iTeamIndex].iLineupKeyPos;
	return INT_MAX;
}
void CBattle::DoLineupBuffer(int iTeamIndex)
{
#ifdef LINEUP_SYSTEM_ON
	if (iTeamIndex >=0 && iTeamIndex < 2) 
	{
		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("DoLineupBuffer Entrance:Show Battle netId %d:%d: actor count %d;TeamIndex=%d,LineupId=%d"),
			GetNetId().srvId, GetNetId().objId, m_actors.size(),iTeamIndex,m_teamData[iTeamIndex].uiLineupId);
		if (!m_teamData[iTeamIndex].bDoLineupBuffer ) 
		{
			const LINEUPINFO* pLineupInfo = gGlobal.m_resource->GetLineupData(m_teamData[iTeamIndex].uiLineupId);
			if (NULL != pLineupInfo) 
			{
				CParty* pParty=NULL;
				if (NULL != m_teamData[iTeamIndex].pLeader) 
				{
					pParty = m_teamData[iTeamIndex].pLeader->GetParty();
				}
				for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
				{
					CBatActor* actor = FindNetObject<CBatActor>(*it);
					if(actor == NULL)
						continue;
					UInt8 uTeam = actor->GetTeam();
					if (uTeam == iTeamIndex) 
					{
						UInt8 uIndex =0;
						if (NULL != pParty) 
						{
							if (BatCharacter_CLASSID == actor->GetClassID()
								&& actor->GetOwner()) 
							{
								uIndex = pParty->GetCharactorIndexById(actor->GetOwner()->GetChar_id());
							}
							else
								uIndex = TEAMMATE_MAX;
						}
						else
							uIndex = 0;

						if (uIndex < TEAMMATE_MAX) {
							actor->AddBuff(pLineupInfo->pb[uIndex].uiBufferId,0,0,0,99999,0);
							if (NULL != pParty) 
							{
								if (pParty->GetCount() == TEAMMATE_MAX
									&& uIndex == m_teamData[iTeamIndex].iLineupKeyPos 
									&& m_teamData[iTeamIndex].uiLineupId != 13) 
								{
									actor->AddBuff(KEYPOSEFFERTBUFFID,0,0,0,99999,0);
								}
							}
						}
					}
				}
				m_teamData[iTeamIndex].bDoLineupBuffer =TRUE;
			}
		}
	}
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("DoLineupBuffer Exit:Show Battle netId %d:%d: actor count %d"),
		GetNetId().srvId, GetNetId().objId, m_actors.size());
#endif
}
void CBattle::CancleLineupBuffer(int iTeamIndex)
{
#ifdef LINEUP_SYSTEM_ON
	if (iTeamIndex >=0 && iTeamIndex < 2) 
	{
		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("CancleLineupBuffer Entrance:Show Battle netId %d:%d: actor count %d;TeamIndex=%d,LineupId=%d"),
			GetNetId().srvId, GetNetId().objId, m_actors.size(),iTeamIndex,m_teamData[iTeamIndex].uiLineupId);
		if (m_teamData[iTeamIndex].bDoLineupBuffer) 
		{
			const LINEUPINFO* pLineupInfo=gGlobal.m_resource->GetLineupData(m_teamData[iTeamIndex].uiLineupId);
			if (NULL != pLineupInfo) 
			{
				CParty* pParty=NULL;
				if (NULL != m_teamData[iTeamIndex].pLeader) 
				{
					pParty = m_teamData[iTeamIndex].pLeader->GetParty();
				}
				for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
				{
					CBatActor* actor = FindNetObject<CBatActor>(*it);
					if (actor == NULL)
						continue;
					UInt8 uTeam = actor->GetTeam();
					if (uTeam == iTeamIndex) 
					{
						UInt8 uIndex =0;
						if (NULL != pParty) 
						{
							if (BatCharacter_CLASSID == actor->GetClassID()
								&& actor->GetOwner()) 
							{
								uIndex = pParty->GetCharactorIndexById(actor->GetOwner()->GetChar_id());
							}
							else
								uIndex = TEAMMATE_MAX;
						}
						else
							uIndex = 0;

						if (uIndex < TEAMMATE_MAX) 
						{
							actor->RemoveBuff(pLineupInfo->pb[uIndex].uiBufferId);
							if (NULL != pParty) 
							{
								if (pParty->GetCount() == TEAMMATE_MAX
									&& uIndex == m_teamData[iTeamIndex].iLineupKeyPos) 
								{
									actor->RemoveBuff(KEYPOSEFFERTBUFFID);
								}
							}
						}
					}
				}
			}
			m_teamData[iTeamIndex].bDoLineupBuffer =FALSE;
		}
	}
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CancleLineupBuffer Exit:Show Battle netId %d:%d: actor count %d"),
		GetNetId().srvId, GetNetId().objId, m_actors.size());
#endif
}
void CBattle::DoLineupAllelopathy()
{
#ifdef LINEUP_SYSTEM_ON
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("DoLineupAllelopathy Entrance:Show Battle netId %d:%d: actor count %d;LineupId0=%d,LineupId1=%d"),
		GetNetId().srvId, GetNetId().objId, m_actors.size(),m_teamData[0].uiLineupId,m_teamData[1].uiLineupId);
	const LINEUPINFO* pLineupInfo0=gGlobal.m_resource->GetLineupData(m_teamData[0].uiLineupId);
	const LINEUPINFO* pLineupInfo1=gGlobal.m_resource->GetLineupData(m_teamData[1].uiLineupId);
	if (NULL != pLineupInfo0 && NULL != pLineupInfo1 && (!m_teamData[0].bDoLineupAllelopathy) && (!m_teamData[1].bDoLineupAllelopathy)) {
		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
		{
			CBatActor* actor = FindNetObject<CBatActor>(*it);
			if (actor == NULL)
				continue;
			UInt8 uTeam = actor->GetTeam();
			if (uTeam == 0 && pLineupInfo1->uiId < MAXLINEUP) 
			{
				actor->AddBuff(pLineupInfo0->lui.Info[pLineupInfo1->uiId].uiLineupId,0,0,0,99999,0);
			}
			else if (uTeam == 1 && pLineupInfo0->uiId < MAXLINEUP) 
			{
				actor->AddBuff(pLineupInfo1->lui.Info[pLineupInfo0->uiId].uiLineupId,0,0,0,99999,0);
			}
		}
		m_teamData[0].bDoLineupAllelopathy =TRUE;
		m_teamData[1].bDoLineupAllelopathy=TRUE;
	}
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("DoLineupAllelopathy Exit:Show Battle netId %d:%d: actor count %d"),
		GetNetId().srvId, GetNetId().objId, m_actors.size());
#endif
}
void CBattle::CancleLineupAllelopathy()
{
#ifdef LINEUP_SYSTEM_ON
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("CancleLineupAllelopathy Entrance:Show Battle netId %d:%d: actor count %d;LineupId0=%d,LineupId1=%d"),
		GetNetId().srvId, GetNetId().objId, m_actors.size(),m_teamData[0].uiLineupId,m_teamData[1].uiLineupId);
	const LINEUPINFO* pLineupInfo0=gGlobal.m_resource->GetLineupData(m_teamData[0].uiLineupId);
	const LINEUPINFO* pLineupInfo1=gGlobal.m_resource->GetLineupData(m_teamData[1].uiLineupId);
	if (NULL != pLineupInfo0 && NULL != pLineupInfo1 && m_teamData[0].bDoLineupAllelopathy && m_teamData[1].bDoLineupAllelopathy) {
		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
		{
			CBatActor* actor = FindNetObject<CBatActor>(*it);
			if(actor == NULL)
				continue;
			UInt8 uTeam = actor->GetTeam();
			if (uTeam == 0 && m_teamData[uTeam].bDoLineupAllelopathy && pLineupInfo1->uiId < MAXLINEUP) {
				actor->RemoveBuff(pLineupInfo0->lui.Info[pLineupInfo1->uiId].uiLineupId);
			}
			else if (uTeam == 1 && m_teamData[uTeam].bDoLineupAllelopathy && pLineupInfo0->uiId < MAXLINEUP) {
				actor->RemoveBuff(pLineupInfo1->lui.Info[pLineupInfo0->uiId].uiLineupId);
			}
		}

		m_teamData[0].bDoLineupAllelopathy =FALSE;
		m_teamData[1].bDoLineupAllelopathy=FALSE;
	}
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CancleLineupAllelopathy Exit:Show Battle netId %d:%d: actor count %d"),
		GetNetId().srvId, GetNetId().objId, m_actors.size());
#endif
}

void CBattle::SetLineupPosition()
{
#ifdef LINEUP_SYSTEM_ON
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("SetLineupPosition Entrance:Show Battle netId %d:%d: actor count %d;LineupId0=%d,LineupId1=%d"),
		GetNetId().srvId, GetNetId().objId, m_actors.size(),m_teamData[0].uiLineupId,m_teamData[1].uiLineupId);
	const LINEUPINFO* pLineup0=gGlobal.m_resource->GetLineupData(m_teamData[0].uiLineupId);
	const LINEUPINFO* pLineup1=gGlobal.m_resource->GetLineupData(m_teamData[1].uiLineupId);
	if (NULL != pLineup0 && NULL != pLineup1) {
		CParty* pParty[2] ={0};
		if (NULL != m_teamData[0].pLeader) {
			pParty[0] = m_teamData[0].pLeader->GetParty();
		}
		if (NULL != m_teamData[1].pLeader) {
			pParty[1] = m_teamData[1].pLeader->GetParty();
		}
		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
		{
			CBatActor* actor = FindNetObject<CBatActor>(*it);
			if (actor == NULL)
				continue;
			UInt8 uTeam = actor->GetTeam();
			UInt8 uIndex = TEAMMATE_MAX;
			if (NULL != pParty[uTeam] && pParty[uTeam]->GetCount() == TEAMMATE_MAX) {
				if (BatCharacter_CLASSID == actor->GetClassID()) {
					uIndex = pParty[uTeam]->GetCharactorIndexById(((CBatCharacter*)actor)->GetOwner()->GetChar_id());
				}
				else
					uIndex = pParty[uTeam]->GetCharactorIndexById(((CBatMob*)actor)->m_pPartner->GetOwner_id());
			}

			if (uIndex < TEAMMATE_MAX ) {
				if (uTeam == 0) {
					if (GetBattleType()  != NPC_BATTLE){
						if (BatCharacter_CLASSID == actor->GetClassID()) {
							m_teamData[0].posIndex[actor->GetPosition()]=UCHAR_MAX;
							actor->SetPosition(pLineup0->pb[uIndex].iPos);
							int iServerPos=pLineup0->pb[uIndex].iPos-5;
							UInt8 uPos= iServerPos < 0 ? (TEAMPOS_MAX+iServerPos) : iServerPos;
							if (uPos < TEAMPOS_MAX)
								m_teamData[0].posIndex[uPos]=actor->GetIndex();
						}
						else
						{
							m_teamData[0].posIndex[actor->GetPosition()]=UCHAR_MAX;
							actor->SetPosition(pLineup0->pb[uIndex].iPosPartner);
							int iServerPos=pLineup0->pb[uIndex].iPosPartner-5;
							UInt8 uPos= iServerPos < 0 ? (TEAMPOS_MAX+iServerPos) : iServerPos;
							if (uPos < TEAMPOS_MAX)
								m_teamData[0].posIndex[uPos]=actor->GetIndex();
						}
					}
				}
				else
				{
					if (BatCharacter_CLASSID == actor->GetClassID()) {
						m_teamData[1].posIndex[actor->GetPosition()]=UCHAR_MAX;
						actor->SetPosition(pLineup1->pb[uIndex].iPos);
						int iServerPos=pLineup1->pb[uIndex].iPos-5;
						UInt8 uPos= iServerPos < 0 ? (TEAMPOS_MAX+iServerPos) : iServerPos;
						if (uPos < TEAMPOS_MAX)
							m_teamData[1].posIndex[uPos]=actor->GetIndex();
					}
					else
					{
						m_teamData[1].posIndex[actor->GetPosition()]=UCHAR_MAX;
						actor->SetPosition(pLineup1->pb[uIndex].iPosPartner);
						int iServerPos=pLineup1->pb[uIndex].iPosPartner-5;
						UInt8 uPos= iServerPos < 0 ? (TEAMPOS_MAX+iServerPos) : iServerPos;
						if (uPos < TEAMPOS_MAX)
							m_teamData[1].posIndex[uPos]=actor->GetIndex();
					}
				}
			}
		}
	}
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("SetLineupPosition Exit:Show Battle netId %d:%d: actor count %d"),
		GetNetId().srvId, GetNetId().objId, m_actors.size());
#endif
}


//pOwner is request pvp ower
void CBattle::InitPvPScript(CCharacter* pOwner,UInt8 battleType,UInt32 pvpbattleID)
{
	if (pvpbattleID > 0)
	{	
		m_pScriptSys->SetOwner(pOwner);
		if (battleType == INCOGNITO_PK_BATTLE)
		{
			m_bPvPScript = m_pScriptSys->ExecutePvPBattleScript(pvpbattleID);
		}
	}
	else
		TRACE_INFODTL(GLOBAL_LOGGER, _T("InitPvPScript Exit:Not has pvpBattleID"));
}

UInt16 CBattle::GetPvPCharacterCountFromTeam(UInt8 team)
{
	UInt8 count = 0;
	for (BatClients::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
	{
		CBatCharacter *pBatChar = it->second.pBatChar;
		if (pBatChar &&	!pBatChar->GetLeftBattle() && pBatChar->GetTeam() == team)
			count ++;
	}

	return count;
}

CBatCharacter* CBattle::GetPvPCharacterFromTeam(UInt8 index,UInt8 team)
{
	UInt8 count = 0;

	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor* actor =FindNetObject<CBatActor>(*it);
		if (actor == NULL)
			continue;
		if (actor->m_pBatChar && !actor->GetLeftBattle() && actor->m_pBatChar->GetTeam() == team) {
			if (index == count)
				return actor->m_pBatChar;
			count ++;
		}
	}

	return NULL;
}

void CBattle::PvPAddMob(UInt32 mob_id, UInt8 pos, UInt8 level, UInt8 team)
{
	AddMob(mob_id, pos, level, team, true);
	m_pvpAddMobPosLst.push_back(pos);
}

UInt16 CBattle::GetPvPAddMobCount()
{
	UInt16 count = 0;

	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor* actor = FindNetObject<CBatActor>(*it);
		if (actor == NULL)
			continue;
		if (actor->IsMob() && actor->m_pMob && !actor->GetLeftBattle())
		{
			if(InPvPAddMobID(actor->m_pMob->GetMob_id()))
				count ++;
		}
	}

	return count;
}

Boolean CBattle::InPvPAddMobID(UInt32 mob_id)
{
	for(int i = 0; i < m_pvpAddMobPosLst.size(); i++)
	{
		if (m_pvpAddMobPosLst[i] = mob_id)
			return true;
	}
	return false;
}

CBatCharacter* CBattle::GetPvPLeaderFromTeam(UInt8 team)
{
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		CBatActor* actor = FindNetObject<CBatActor>(*it);
		if (actor == NULL)
			continue;
		if (actor->m_pBatChar && !actor->GetLeftBattle() && actor->m_pBatChar->GetTeam() == team && actor->m_pBatChar->IsLeader()) {
			return actor->m_pBatChar;
		}
	}
	return NULL;
}