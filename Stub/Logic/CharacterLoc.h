#pragma once
#ifndef _CHARACTERLOC_H_
#define _CHARACTERLOC_H_

#include "Stub/Logic/CharacterLoc_Stub.h"

class CCharacterData;

class CCharacterLoc : public CharacterLoc_Stub
{
private:
	typedef CharacterLoc_Stub Parent;
	UInt8			m_lineId;			// temp line id
	Peer*			m_pClientPeer;
	RPCContext*		m_pContext;

public:
	enum {
		ST_ENTER = 1,
		ST_LEAVE,
	};

	class CEnterState;
	class CLeaveState;
	State* m_state;			// [outside, inside, going, leaving], 
	SessionID m_sessionId;

	DEFINE_CLASS(CharacterLoc_CLASSID);
	CCharacterLoc();
	virtual ~CCharacterLoc();
	inline static CCharacterLoc* NewInstance();
	inline void SetPeer(Peer *clientPeer, RPCContext *pContext);
	void SetData(CCharacterData *pCharData);	// set character by CCharacterData
	void WriteData(CCharacterData *pCharData);	// write character to CCharacterData
	void PackBuffer(Buffer &buffer);			// pack buffer using CCharacterData
	inline void SetLine_id(UInt8 line_id) { m_lineId = line_id; }
	inline UInt8 GetLine_id() { return m_lineId; }
};

class CCharacterLoc::CEnterState : public State
{
	DEFINE_STATE(ST_ENTER)

	/*
	virtual CONST Boolean CanTransition(CONST StateID targetId) CONST
	{
		return (targetId == ST_LEAVE);
	}
	*/
};

class CCharacterLoc::CLeaveState : public State
{
	DEFINE_STATE(ST_LEAVE)

	/*
	virtual CONST Boolean CanTransition(CONST StateID targetId) CONST
	{
		return (targetId == ST_ENTER);
	}
	*/
};

inline CCharacterLoc::CCharacterLoc()
{
	m_state = NULL;
	m_lineId = 0;

	State::ChangeState(m_state, SafeCreateObject(CLeaveState));
}

inline CCharacterLoc::~CCharacterLoc()
{
	State::ChangeState(m_state, NULL);
}

inline CCharacterLoc* CCharacterLoc::NewInstance()
{
	return dynamic_cast<CCharacterLoc *>(CreateNetObject(CharacterLoc_CLASSID));
}

inline void CCharacterLoc::SetPeer(Peer *clientPeer, RPCContext *pContext)
{
	m_pClientPeer = clientPeer;
	m_pContext = pContext;
}

#endif //_CHARACTERLOC_H_
