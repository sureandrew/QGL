#pragma once
#ifndef _PARTY_H_
#define _PARTY_H_

#include "Stub/Logic/Party_Stub.h"
#include "Stub/Logic/Character.h"
#include "PartyCharInfo.h"

#define PARTY_MAX_MEMBER		5
#define PARTY_MAX_REQUEST		20
#define PARTY_MAX_INVITE		10

class CCharacter;
class CMap;

class CParty : public Party_Stub,
				public Reuben::Simulation::IEventHandler<CParty>
{
public:
	DEFINE_CLASS(Party_CLASSID);

	CParty();
	virtual ~CParty();
	INLINE static CParty* NewInstance();

	virtual RPCResult Promote(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId);
	virtual RPCResult Kick(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId);
	virtual RPCResult Leave(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult Destroy(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult JoinChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult ChangeDesc(LPCPEER pPeer, RPCContext &context, const String &desc);

	CCharacter* GetLeader();						// get party leader
	VOID SetLeader(CCharacter* pchar);				// set party leader
	// character join to party with notify option
	Boolean Join(CCharacter* pchar, Boolean bNotify, Boolean bForce = BOOLEAN_FALSE, Boolean shown=BOOLEAN_TRUE);
	// character leave from party
	Boolean Leave(CCharacter* pchar, Boolean bKick = BOOLEAN_FALSE, Boolean shown=BOOLEAN_TRUE);
	// destroy party
	VOID Destroy(Boolean shown=BOOLEAN_TRUE);
	// promote party member to leader
	Boolean Promote(CCharacter* pchar, Boolean bLeft=BOOLEAN_FALSE, Boolean shown=BOOLEAN_TRUE);
	Boolean Regroup(CCharacter* pchar, const UInt32Vector &memberIdList);
	void SetAvail(Boolean avail);
	VOID PreSend(Boolean bClear=BOOLEAN_FALSE);		// prepare whole party to send with clear set mask

	VOID GetMemberIds(UInt32Vector &memberIds);
	VOID GetSessionIds(SessionIDVector &sessIds);
	CCharacter* Get(int index) { return GetMember(index); }	// get by index
	CCharacter* GetMember(UInt16 index);			// get member by index
	CCharacter* FindMember(UInt32 char_id);			// find member by char_id
	Boolean IsMember(CONST RPCNetID &netId);		// Is member by netId
	Boolean IsMember(CCharacter* pchar);			// Is member by netId
	VOID LeaveTeamChannel(CCharacter* pchar);		// join team channel
	VOID JoinTeamChannel(CCharacter* pchar);		// leave team channel
	VOID CalAvgLevel();
	float GetAvgLevel() { return m_avgLevel; }
	UInt8 GetSexCount(int sex);
	UInt8 GetJobCount(int job);
	VOID SendToMembers();								// send party information to all party members
	VOID ShowMember();
	virtual void HandleEvent(CONST Event &event);
	UInt8 GetCharactorIndexById(UInt32 uiCharId);
	int SetChangePos(UInt8 ui8PreIndex,UInt8 ui8DestIndex);
	Boolean IsAllEntered();

	VOID SetJoinPartyLevelDown(UInt16 JoinPartyLevelDown){m_nJoinPartyLevelDown = JoinPartyLevelDown;}
	VOID SetJoinPartyLevelUp(UInt16 JoinPartyLevelUp){m_nJoinPartyLevelUp = JoinPartyLevelUp;}
	UInt16 GetJoinPartyLevelDown(){return m_nJoinPartyLevelDown;}
	UInt16 GetJoinPartyLevelUp(){return m_nJoinPartyLevelUp;}

	Boolean SetTempLeave(UInt32 uId);
	void SetTempLeaveAll();
	UInt16 GetRealMemNum();
	CCharacter* GetInPartyMember(UInt16 u16Index);
	UInt32 ReturnParty(CCharacter* pChar);
	VOID RefreshPartyCharInfo(CPartyCharInfo* pInfo,Boolean bTempLeave = BOOLEAN_TRUE);

private:
	enum EventEnum
	{
		EVT_REGROUP_TIMEOUT		= MAKE_EVENTID(Party_CLASSID,  1),
	};

	typedef Party_Stub Parent;
	CCharacter*		m_pLeader;
	float			m_avgLevel;
	RPCNetID        m_leaderNetId;
	UInt16 			m_nJoinPartyLevelDown;
	UInt16 			m_nJoinPartyLevelUp;

};

INLINE CParty* CParty::NewInstance()
{
	return CreateNetObject<CParty>();
}

INLINE CCharacter* CParty::GetLeader()
{
	m_pLeader = FindNetObject<CCharacter>(m_leaderNetId);
	return m_pLeader;
}

INLINE VOID CParty::SetLeader(CCharacter* pchar)
{
	m_pLeader = pchar;
	if(pchar)
		m_leaderNetId = pchar->GetNetId();
	else
		m_leaderNetId = RPCNetID();

}

INLINE CCharacter* CParty::GetMember(UInt16 index)
{
	if (index < GetCount())
		return (GetChild<CCharacter>(index));
	else
		return NULL;
}

#endif //_PARTY_H_
