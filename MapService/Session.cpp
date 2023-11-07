#include "Common.h"
#include "Stub/Logic/Account.h"
#include "Stub/Logic/CharacterInfo.h"
#include "WorldService/Global.h"
#include "WorldService/WorldApplication.h"

Session::Session()
	: m_bConnect(BOOLEAN_TRUE), 
	  pAccount(NULL), pCharInfo(NULL)
{
}

VOID Session::SetSessionId(CONST SessionID &sid)
{
	m_pPeer = WorldApplication::GetInstance().GetProxy(sid.serviceId);
	m_context.sessionId = sid;
}

VOID Session::Release(CONST SessionID &sid)
{
	ProcedureManager::GetInstance().RemoveBySession(sid);

	// delete account object for owner session
	if (pAccount) {
		pAccount->RemoveSessionId(sid);
	}
}

Session* AddSession(CONST SessionID& sid, CONST Session& session)
{
	return gGlobal.m_sessionManager.Add(sid, session);
}

Session* GetSession(CONST SessionID& sid)
{
	return gGlobal.m_sessionManager.Get(sid);
}

VOID ChangeSession(CONST SessionID& old_sid, CONST SessionID& sid)
{
	gGlobal.m_sessionManager.Change(old_sid, sid);
}

VOID RemoveSession(CONST SessionID& sid)
{
	gGlobal.m_sessionManager.Remove(sid);
}

VOID RemoveSession(CONST ServiceID& srvId)
{
	gGlobal.m_sessionManager.Remove(srvId);
}

VOID ClearDeadSession(VOID)
{
	gGlobal.m_sessionManager.ClearDead();
}
