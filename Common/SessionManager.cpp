//-- Common
#include "Common.h"
//-- Self
#include "Common/SessionManager.h"

SessionManager::~SessionManager()
{
	for (SessionMap::iterator it = m_sessionMap.begin();
		it != m_sessionMap.end(); ++it)
	{
		SafeDeleteObject(it->second);
	}
}

VOID SessionManager::GetSessionIds(ServiceID srvId, SessionIdList &idList)
{
	for (SessionMap::iterator it = m_sessionMap.begin();
		it != m_sessionMap.end(); ++it)
	{
		if (it->first.serviceId == srvId)
			idList.push_back(it->first);
	}
}

VOID SessionManager::Change(CONST SessionID& old_sid, CONST SessionID& sid)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SessionManager::Change old_sid=%s, new_sid=%s"), old_sid.GetString().c_str(), sid.GetString().c_str());
	if (old_sid == sid) return;

	SessionMap::iterator it = m_sessionMap.find(old_sid);

	if (it != m_sessionMap.end()) {
		BaseSession* temp = it->second;
		m_sessionMap.erase(it);
		m_sessionMap[sid] = temp;
		// remove old session Id for redirect
		RemoveRedirect(old_sid);
		// add old session Id to redirect map
		m_redirectMap[old_sid] = sid;
	}
}

VOID SessionManager::Remove(CONST SessionID& sid)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SessionManager::Remove sid=%s"), sid.GetString().c_str());
	SessionMap::iterator it = m_sessionMap.find(sid);
	if (it != m_sessionMap.end())
	{
		m_deadSessions[it->first] = it->second;
		BaseSession* psess = it->second;
		m_sessionMap.erase(it);
		//if (psess)
		//	psess->Release(sid);
		// remove old session Id for redirect
		RemoveRedirect(sid);
	}

}

VOID SessionManager::Remove(ServiceID srvId)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SessionManager::Remove srvId=%d"), srvId);
	for (SessionMap::iterator it = m_sessionMap.begin();
		it != m_sessionMap.end();)
	{
		if (it->first.serviceId == srvId) {
			m_deadSessions[it->first] = it->second;
			BaseSession* psess = it->second;
			it = m_sessionMap.erase(it);
			//if (psess)
			//	psess->Release(it->first);
			// remove old session Id for redirect
			RemoveRedirect(it->first);
		} else
			++it;
	}

}

VOID SessionManager::RedirectSessionId(SessionID& sid)
{
	RedirectMap::iterator it = m_redirectMap.find(sid);

	if (it != m_redirectMap.end())
		sid = it->second;
}

VOID SessionManager::RemoveRedirect(CONST SessionID& sid)
{
	for (RedirectMap::iterator it = m_redirectMap.begin(); it != m_redirectMap.end(); ++it)
	{
		if (sid == it->second) {
			it = m_redirectMap.erase(it);
			break;
		}
	}
}

VOID SessionManager::ClearDead(VOID)
{
	if (m_deadSessions.empty()) return;
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SessionManager::ClearDead %d"), m_deadSessions.size());

	for (SessionMap::iterator it = m_deadSessions.begin(); it != m_deadSessions.end(); ++it)
	{
		if (it->second)
		{
			BaseSession* psess = it->second;
			try
			{
				psess->Release(it->first);
				SafeDeleteObject(psess);
			}
			catch(...)
			{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("SessionManager::ClearDead Exception: pointer %d"), psess);
			}
		}
	}

	m_deadSessions.clear();
}

VOID SessionManager::ClearAll()
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SessionManager::ClearAll %d"), m_sessionMap.size());
	for (SessionMap::iterator it = m_sessionMap.begin();
		it != m_sessionMap.end(); ++it)
	{
		if (it->second)
			it->second->Release(it->first);
		SafeDeleteObject(it->second);
	}
	m_sessionMap.clear();
	ClearDead();
}