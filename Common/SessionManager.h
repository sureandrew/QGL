#pragma once
#ifndef _SESSION_MANAGER_H_
#define _SESSION_MANAGER_H_

#include <Reuben/Utility/Singleton.h>

typedef StlVector<SessionID, ManagedAllocator<SessionID> > SessionIdList;

class BaseSession
{
public:
	virtual VOID Release(CONST SessionID &sid) {}

	INLINE VOID SetOwnObject(NetObject* obj)
	{
		if (obj)
			ownNetIds.insert(obj->GetNetId());
	}

	INLINE VOID UnsetOwnObject(NetObject* obj)
	{
		if (obj)
			ownNetIds.erase(obj->GetNetId());
	}

	INLINE Boolean IsOwnObject(NetObject* obj)
	{
		return (obj && ownNetIds.find(obj->GetNetId()) != ownNetIds.end());
	}

private:
	typedef StlSet<RPCNetID, std::less<RPCNetID>, ManagedAllocator<RPCNetID> > NetIdSet;
	NetIdSet	ownNetIds;
};

class SessionManager : public Reuben::Utility::Singleton<SessionManager>
{
public:
	~SessionManager();
	VOID GetSessionIds(ServiceID srvId, SessionIdList &idList);
	VOID Change(CONST SessionID& old_sid, CONST SessionID& sid);
	VOID Remove(CONST SessionID& sid);
	VOID Remove(ServiceID srvId);
	VOID RedirectSessionId(SessionID& sid);
	VOID ClearDead(VOID);
	VOID ClearAll();

	template <typename _SESSION_>
	INLINE _SESSION_* Add(CONST SessionID& sid, CONST _SESSION_& session)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SessionManager::Add sid=%s, session=%p"), sid.GetString().c_str(), &session);
		_SESSION_ *psess = SafeCreateObject(_SESSION_);
		*psess = session;
		m_sessionMap[sid] = psess;
		return psess;
	}

	template <typename _SESSION_>
	INLINE _SESSION_* Get(CONST SessionID& sid)
	{
		SessionMap::iterator it = m_sessionMap.find(sid);

		if (it != m_sessionMap.end())
			return ((_SESSION_*) it->second);
		else
			return NULL;
	}

	INLINE Boolean IsOwnObject(const SessionID &sid, NetObject* obj)
	{
		if (obj == NULL)
			return BOOLEAN_FALSE;

		SessionMap::iterator it = m_sessionMap.find(sid);
		if (it == m_sessionMap.end())
			return BOOLEAN_FALSE;

		return (it->second->IsOwnObject(obj));
	}

private:

	typedef StlMap<SessionID, BaseSession*, std::less<SessionID>, 
		ManagedAllocator<std::pair<SessionID, BaseSession*> > > SessionMap;
	typedef StlMap<SessionID, SessionID, std::less<SessionID>, 
		ManagedAllocator<std::pair<SessionID, SessionID> > > RedirectMap;

	SessionMap		m_sessionMap;		// alive sessions map
	SessionMap		m_deadSessions;		// dead sessions map
	RedirectMap		m_redirectMap;		// session Id redirect map
	VOID RemoveRedirect(CONST SessionID& sid);		// remove redirect to that sessionID
};

#endif // _SESSION_MANAGER_H_