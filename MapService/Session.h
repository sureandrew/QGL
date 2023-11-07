#pragma once
#ifndef _SESSION_H_
#define _SESSION_H_

#include "Common.h"
#include "Common/SessionManager.h"

class CAccount;
class CCharacterInfo;

struct Session
{
public:
	CAccount*		pAccount;
	CCharacterInfo*	pCharInfo;

	Session();
	VOID SetConnect(Boolean bConnect) { m_bConnect = bConnect; }
	Boolean IsConnected() { return m_bConnect; }
	//VOID SetPeer(Peer* pPeer) { m_pPeer = pPeer; }
	CONST Peer* GetPeer() { return m_pPeer; }
	RPCContext& GetContext() { return m_context; }
	VOID SetSessionId(CONST SessionID &sid);
	CONST SessionID& GetSessionId() { return m_context.sessionId; }
	VOID Release(CONST SessionID &sid);

private:
	Peer*		m_pPeer;					// client peer
	RPCContext	m_context;					// client context contain session Id
	Boolean		m_bConnect;					// is client are connected?
};

Session* AddSession(CONST SessionID& sid, CONST Session& session);
Session* GetSession(CONST SessionID& sid);
VOID ChangeSession(CONST SessionID& old_sid, CONST SessionID& sid);
VOID RemoveSession(CONST SessionID& sid);
VOID RemoveSession(CONST ServiceID& srvId);
VOID ClearDeadSession(VOID);

#endif // _SESSION_H_
