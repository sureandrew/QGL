#pragma once
#ifndef _BATTLE_H_
#define _BATTLE_H_

#include "Stub/Logic/Battle_Stub.h"
#include "Common/BattleCode.h"

#include <vector>
#include <map>

#define TEAMMATE_MAX		5
#define TEAMPOS_MIN			(TEAMMATE_MAX * 2)
#define TEAMPOS_MAX			(TEAMMATE_MAX * 4)
#define GUEST_POS			15
#define ENEMY_TEAM			0
#define PLAYER_TEAM			1
#define DRAWGAME			255
#define AUTO_TIMEOUT		2000		// 2 second delay for auto command
#define CMD_TIMEOUT			20000		// 20 second timeout for command input
#define TALK_TIMEOUT		20000		// 20 second timeout for talking timeout
#define TIMEOUT_DELAY		500			// 0.5 delay for turn timeout
#define MAX_HELPCOUNT		4
#define MAX_PLAYER_IN_ROW	5

//define the follow open lineup system
#define LINEUP_SYSTEM_ON

//pvp attackers team = 0, defensers team = 1
struct NpcData;
class CCharacter;
class CPartner;
class CBatActor;
class CBatCharacter;
class CBatMob;
class CScriptSystem;

enum BatEventEnum				// battle event enum
{
	BSEVENT_NONE = 0,			// no event
	BSEVENT_SURRENDER,			// surrender event
	BSEVENT_ENDBATTLE,			// force end battle event
	BSEVENT_SKIPTURN,			// skip turn event
};

enum BatEndEnum {				// battle result enum
	END_WIN = 1,				// player team win
	END_LOSE,					// player team lose
	END_ESCAPED,				// player team escaped
	END_DRAW,					// no team win or lose 
	END_MONITOR,				// monitor end
};

typedef StlVector<RPCNetID, ManagedAllocator<RPCNetID> > Actors;

struct BatClient
{
	CCharacter* pOwner;					// client character owner
	UInt8	status;						// client status
	Boolean bAuto;						// client is auto-play
	Boolean bEndTurn;					// client is ended turn
	CBatCharacter *pBatChar;			// client battle character
	CBatMob *pPartner;					// client battle partner
	UInt16	autoCount;					// client auto turn count
	UInt8	team;						// team no
	UInt8	teamIndex;					// team index
	BatClient() : pOwner(NULL), status(0), bAuto(BOOLEAN_FALSE), autoCount(0),
		bEndTurn(BOOLEAN_FALSE), pBatChar(NULL), pPartner(NULL), team(0), teamIndex(0) {}
	bool IsLeft();
	bool IsBattle();
	bool IsMonitor();
	bool IsWaiting();
	bool IsConnect();
	LPCPEER	GetPeer();
	RPCContext& GetContext();
};

struct BatActorLinkLst
{
	CBatActor * pBatActor;
	BatActorLinkLst * pNext;

	BatActorLinkLst()
	{
		pBatActor = NULL;
		pNext = NULL;
	}

	CBatActor * GetActor()
	{
		return pBatActor;
	}

	BatActorLinkLst * GetNext()
	{
		return pNext;
	}
};

struct TeamData
{
	bool		needHelp;
	CCharacter *pLeader;
	UInt8		playerCount;
	UInt8		existCount;				// player exist count
	UInt8		killCount;				// player kill mob count
	UInt8		posIndex[TEAMPOS_MAX];	// positional index

	UInt32      uiLineupId;				//lineup id
	int			iLineupKeyPos;			//lineup key position
	BOOL		bDoLineupBuffer;		//has DoLineupBuffer?
	BOOL		bDoLineupAllelopathy;	//has DoLineupAllelopathy?

	TeamData() 
		: needHelp(false),
		pLeader(NULL), 
		playerCount(0), 
		existCount(0), 
		killCount(0),
		uiLineupId(13),
		iLineupKeyPos(0),
		bDoLineupAllelopathy(FALSE),
		bDoLineupBuffer(FALSE)
	{
		FillMemory(posIndex, sizeof(UInt8) * TEAMPOS_MAX, 255);
	}
};

class CBattle : public Battle_Stub,
				public Reuben::Simulation::IEventHandler<CBattle>
{
	//////////////////////////////////////
	//			RPC methods				//
	//////////////////////////////////////
public:
	virtual RPCResult EndTurn(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult NextTalk(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult ChooseResult(LPCPEER pPeer, RPCContext &context, const UInt8 &ans);
	virtual RPCResult SendBattleMessage(LPCPEER pPeer, RPCContext &context, const RPCNetID &charNetID, const String &Msg);

	//////////////////////////////////////
	//		Init Battle methods			//
	//////////////////////////////////////
	DEFINE_CLASS(Battle_CLASSID);
	CBattle();
	virtual ~CBattle();
	INLINE static CBattle* NewInstance();
	Boolean Initialize(UInt8 battleType, UInt32 npcId, const RPCNetID &netId, 
						UInt8 partyCount, UInt16 averageLv, CCharacter * pLeader);
	Boolean CreateMob(UInt32 npcId, UInt8 partyCount, CCharacter * pLeader, UInt16 averageLv=0);
	Boolean AddCharacter(CCharacter* pChar, UInt8 team);
	Boolean AddMonitor(CCharacter* pChar, UInt8 team);
	Boolean PrepareDone();
	void PrepareSendGroup(NetGroup &grp, UInt8 teamNo);
	void CheckTeamStatus();
	void CheckBatEvent();
	void InitAutoTarget(CBatActor* batActor);

	//////////////////////////////////////
	//			Client methods			//
	//////////////////////////////////////
	BatClient* NewClient(CCharacter* pChar);
	BatClient* GetClient(const SessionID &sid);
	void ClientDisconnect(const SessionID &sid);
	void ChangeSessionId(const SessionID &oldSessId, const SessionID &sid);
	Boolean ResumeClient(const SessionID &sid);

	//////////////////////////////////////
	//			Actor methods			//
	//////////////////////////////////////
	CBatActor* GetActor(UInt8 index);
	CBatActor* GetActor(UInt8 targetType, UInt32 tid);
	UInt16 GetActorCount() { return ((UInt16) m_actors.size()); }
	CBatCharacter* GetCharacter(UInt8 index);
	UInt16 GetCharacterCount();
	CBatMob* GetMob(UInt8 index);
	UInt16 GetMobCount();
	CBatCharacter* GetTeamLeader(UInt8 team);
	float GetNpcLevel();
	Boolean CanInput();									// character can input command
	void NotifyInput(CBatActor *actor);					// notify by character input command
	void InsertActor(CBatActor *actor);
	void AddStandby(CBatActor *actor);					// added to standby list
	void AddWaiting(CBatActor *actor);					// added to waiting list
	void RemoveWaiting(CBatActor *actor);				// remove from waiting list
	void RemoveWaiting(BatClient* pClient);				// remove from waiting list
	UInt8 GetCharacterCount(UInt8 team);
	CBatActor* GetProtector(CBatActor *attacker, CBatActor *target, Boolean bCritHit = BOOLEAN_FALSE);
	CBatCharacter* FindCharacter(UInt32 char_id);
	CBatMob* FindPartner(UInt32 mob_id);

	CBatActor* GetActorByPosition(UInt8 pos, UInt8 team);

	//////////////////////////////////////
	//		End Battle methods			//
	//////////////////////////////////////
	void ClearContent();
	void RemoveMonitor(CCharacter* pChar);
	void RemoveMonitor(BatClient* pClient);
	void WinReward(BatClient *pClient, UInt8 teamCount, UInt8 killCount, UInt8 otherTeamCount);
	void DeadPenalty(BatClient *pClient);
	void DrawnGame(BatClient *pClient);
	void CharEscaped(BatClient *pClient);
	// update to map service
	void UpdateToMap(BatClient *pClient, UInt8 endState, bool jumpBack);
	void DeleteBattle();								// delete this battle object
	void TeamEscaped(UInt8 team);
	Boolean IsAllLeft();								// is all client left?
	void ShowAllDetail();
	void FinishBattle();			// battle finish and final outcome

	//////////////////////////////////////
	//		State/Event methods			//
	//////////////////////////////////////
	virtual void HandleEvent(CONST Event &event);
	Boolean ChangeState(CONST StateID state);			// try to change state
	Boolean IsState(CONST StateID state);				// is that state now?
	void NextState(CONST StateID state) { m_nextState = state; }	// set next state
	Boolean IsCmdBlocked(UInt8 cmd, Boolean atInput=FALSE);
	

	//////////////////////////////////////
	//		Lua Access methods			//
	//////////////////////////////////////
	CScriptSystem* GetScript() { return m_pScriptSys; }
	void SetForceEnd(Boolean bEnd);
	void AddMob(UInt32 mob_id, UInt8 pos=0, UInt8 level=0, UInt8 team=0, bool bGuest=false);
	void AddGuest(UInt32 mob_id, UInt8 pos=GUEST_POS);
	Boolean IsGuestExist(UInt8 pos=GUEST_POS);
	UInt8 GetTeamExist(UInt8 team);
	UInt16 GetTeamAvgLevel(UInt8 team);
	void SetPunish(Boolean bPunish) { m_bPunish = bPunish; }
	Boolean GetPunish() { return m_bPunish; }
	void SetJumpReborn(Boolean bJump) { m_bJumpReborn = bJump; }
	void SetBatEvent(UInt8 eventType);
	void SetBatEvent(UInt8 turn, UInt8 eventType);
	void PushReactionActor(CBatActor * pBatActor);
	void PushExtraActor(CBatActor * pBatActor);
	void WriteExtraActor();
	void BatSay(PCSTRINGA msg, int imageType, bool bLeft, UInt32 npc_id, UInt8 index);
	void Surrender(UInt8 team);
	void DisableCmd(UInt8 cmd);

	//////////////////////////////////////////////////////////////////////////
	//lineup related function
	//////////////////////////////////////////////////////////////////////////
	UInt32 SetLineupId(UInt32 uiLineupId,int iTeamIndex);
	UInt32 GetLineupId(int iTeamIndex) const;
	int SetLineupKeyPos(int iKeyPos,int iTeamIndex);
	int GetLineupKeyPos(int iTeamIndex) const;
	void DoLineupBuffer(int iTeamIndex);
	void CancleLineupBuffer(int iTeamIndex);
	void DoLineupAllelopathy();
	void CancleLineupAllelopathy();
	void SetLineupPosition();

	//////////////////////////////////////
	//	pvp Battle inScript	funtion		//
	//////////////////////////////////////
	UInt16 GetPvPCharacterCountFromTeam(UInt8 team);
	CBatCharacter* GetPvPCharacterFromTeam(UInt8 index,UInt8 team);
	void PvPAddMob(UInt32 mob_id, UInt8 pos, UInt8 level, UInt8 team);
	UInt16 GetPvPAddMobCount();
	CBatCharacter* GetPvPLeaderFromTeam(UInt8 team);
	Boolean InPvPAddMobID(UInt32 mob_id);
	//buff
	void DoBuffActiveList(StlVector<CBatActor *> & buffActiveList);

	void FilterActorData(ByteArray & array, CBatActor * pActor, UInt32 arraySize);
public:
	enum StateEnum {
		ST_PREPARE = 1,				// battle prepare phase
		ST_PRETURN,					// pre turn phase
		ST_WAITCMD,					// waiting client command phase
		ST_PLAYACTION,				// client play actions phase
		ST_POSTTURN,				// post turn phase
		ST_FINISH,					// battle finished phase
	};
	enum ClientEnum {
		CLIENT_LEFT = 0,
		CLIENT_BATTLE,
		CLIENT_WAITING,
		CLIENT_MONITOR,
	};
	enum EventEnum {
		EVT_CMD_TIMEOUT		= MAKE_EVENTID(Battle_CLASSID, 1),
		EVT_NEXT_TURN		= MAKE_EVENTID(Battle_CLASSID, 2),
		EVT_CHAR_ESCAPED	= MAKE_EVENTID(Battle_CLASSID, 3),
		EVT_TALK_TIMEOUT	= MAKE_EVENTID(Battle_CLASSID, 4),
//		EVT_AUTO_TIMEOUT = MAKE_EVENTID(Battle_CLASSID, 2),
//		EVT_FORCE_END_TURN = MAKE_EVENTID(Battle_CLASSID, 5),
//		EVT_BATTLE_LOCKED = MAKE_EVENTID(Battle_CLASSID, 6),
	};

	class CPrepareState;
	class CPreTurnState;
	class CWaitCmdState;
	class CPlayActionState;
	class CPostTurnState;
	class CFinishState;

	typedef StlMap<SessionID, BatClient, std::less<SessionID>, 
		ManagedAllocator<std::pair<SessionID, BatClient> > > BatClients;
	typedef StlVector<SessionID, ManagedAllocator<SessionID> > SessionIDList;
	typedef StlMap<UInt8, UInt8, std::less<UInt8>, 
		ManagedAllocator<std::pair<UInt8, UInt8> > > BatEvents;
	typedef StlVector<UInt32, ManagedAllocator<UInt32> > AddMobIDLst;

	const NpcData*	m_pNpcData;		// current NPC data
	State*		m_state;			// current phase state
	StateID		m_nextState;		// next phase state
	Actors		m_actors;			// actor group contain mobs and characters
	BatClients	m_clients;			// battle client list
	Actors		m_waitlist;			// actors at waiting list to join
	Actors		m_standbyList;		// actors at standy list to join
	UInt8		m_curTurn;			// current turn
	UInt8		m_curStep;			// current action step
	UInt32		m_turnTime;			// turn start time
	UInt8		m_winTeam;			// winning team
	Boolean		m_forceEnd;			// force to end battle
	Buffer*		m_turnBuffer1;		// buffer for play in turn for team 1
	Buffer*		m_turnBuffer2;		// buffer for play in turn for tema 2
	Buffer*		m_turnBufferMon;	// buffer for play in turn for monitors
	UInt32		m_npcId;			// current NPC id
	UInt16		m_totalMonitor;		// total number of character monitor
	UInt32		m_animTime;			// turn animation time
	UInt32Vector m_mobIndices;		// mob id list
	TeamData	m_teamData[2];
	Boolean		m_bPunish;			// has punishment
	Boolean		m_bJumpReborn;		// jump to reborn flag
	Actors		m_SkillReactionsMap;
	Actors		m_ExtraActorMap;	
	NetGroup	m_friendGroup;		// added friend group
	BatEvents	m_batEvents;		// battle event map for turn
	UInt8		m_talkNum;			// no. of talking text to npc
	CScriptSystem*	m_pScriptSys;
	Boolean		m_bNpcScript;		// exist npc script

	UInt8			m_cmdBlockMap[MAX_ACTION_CMD + 1];		// command block map

	String		m_team0Leader;
	String		m_team1Leader;
	String		m_mapString;

	Boolean		m_bPvPScript;		//exist pvp script
	AddMobIDLst m_pvpAddMobPosLst;
private:
	typedef Battle_Stub Parent;
	typedef std::multimap<float, CBatActor*, std::greater<float>, 
		ManagedAllocator<std::pair<float, CBatActor*> > > SpeedOrder;

	void InitScript(CCharacter* pOwner, const RPCNetID &netId);
	void SetAvailPos(CBatActor* pActor);	// set available position
	void RunTurn();					// running turn flow control
	Boolean PreTurn();				// pre-turn actions
	Boolean WaitCmd();				// wait command
	Boolean PlayAction();			// play actions
	Boolean PostTurn();				// post-turn actions
	void WaitTalk();				// tell client to wait for talking
	void CancelTalk();				// tell client to cancel for talking
	Boolean CheckForceEnd();		// check to check end battle
//	void SetAutoCommand();			// set auto command for players
	Boolean CheckCombo(SpeedOrder &orderList, SpeedOrder::iterator &it);
	void InitPvPScript(CCharacter* pOwner,UInt8 battleType,UInt32 pvpbattleID);

};

INLINE Boolean CBattle::IsCmdBlocked(UInt8 cmd, Boolean atInput)
{
	return (cmd > MAX_ACTION_CMD || m_cmdBlockMap[cmd] > (UInt8) atInput);
}

INLINE bool BatClient::IsLeft()
{
	return (status == CBattle::CLIENT_LEFT);
}

INLINE bool BatClient::IsBattle()
{
	return (status == CBattle::CLIENT_BATTLE);
}

INLINE bool BatClient::IsMonitor()
{
	return (status == CBattle::CLIENT_MONITOR);
}

INLINE bool BatClient::IsWaiting()
{
	return (status == CBattle::CLIENT_WAITING);
}

INLINE CBattle* CBattle::NewInstance()
{
	return CreateNetObject<CBattle>();
}

INLINE BatClient* CBattle::GetClient(const SessionID &sid)
{
	BatClients::iterator it = m_clients.find(sid);

	if (it != m_clients.end())
		return &(it->second);
	else
		return NULL;
}

INLINE Boolean CBattle::CanInput()
{
	return (m_state->GetStateID() == ST_WAITCMD);
}

class CBattle::CPrepareState : public State
{
	DEFINE_STATE(ST_PREPARE)
};

class CBattle::CPreTurnState : public State
{
    DEFINE_STATE(ST_PRETURN)
};

class CBattle::CWaitCmdState : public State
{
    DEFINE_STATE(ST_WAITCMD)
};

class CBattle::CPlayActionState : public State
{
	DEFINE_STATE(ST_PLAYACTION)
};

class CBattle::CPostTurnState : public State
{
    DEFINE_STATE(ST_POSTTURN)
};

class CBattle::CFinishState : public State
{
	DEFINE_STATE(ST_FINISH)
};

#endif //_BATTLE_H_
