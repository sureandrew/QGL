#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "Common.h"
#include "Common/SessionManager.h"
#include "Reuben/Math/Random.h"
#include "Common/common_binary.h"

#include <atltime.h>

#define MAP_MAX_LINE		11			// 0 line is special line

class CAccount;
class CServerResManager;
class CRankingControl;
class CTaskScheduler;
class CChargeShopControl;
class CCharacterInfo;
class CSellYuanBaoGroup;

#define CONFIGURATION_INI_FILE		_T(".\\Data\\Setting\\fileconfig.ini")

class Session : public BaseSession
{
public:
	CAccount*		pAccount;
	Reuben::Network::IP ip;

	Session();
	VOID SetPeer(LPCPEER pPeer) { m_pPeer = pPeer; }
	LPCPEER GetPeer() { return m_pPeer; }
	RPCContext& GetContext() { return m_context; }
	VOID SetContext(CONST RPCContext &context) { m_context = context; }
	VOID SetSessionId(CONST SessionID &sid);
	CONST SessionID& GetSessionId() { return m_context.sessionId; }
	VOID Release(CONST SessionID &sid);

private:
	LPCPEER		m_pPeer;					// client peer
	RPCContext	m_context;					// client context contain session Id
};

struct MapInfo
{
	MapIndex index;
	LPCPEER	pPeer;
	Boolean	bOpen;
	UInt16	instId;
	MapInfo() : pPeer(NULL), bOpen(BOOLEAN_TRUE), instId(0) {}
	MapInfo(const MapIndex& index, LPCPEER pPeer, Boolean bOpen) : 
		index(index), pPeer(pPeer), bOpen(bOpen), instId(0) {}
};

struct NpcLiveInfo;
struct NpcCreateType;
typedef StlMap<MapIndex, MapInfo, std::less<MapIndex>, 
			ManagedAllocator<std::pair<MapIndex, MapInfo> > > MapIndexList;
typedef StlMap<UInt32, CAccount*, std::less<UInt32>, 
			ManagedAllocator<std::pair<UInt32, CAccount*> > > AccountMap;
typedef StlSet<LPCPEER, std::less<LPCPEER>, ManagedAllocator<LPCPEER> > PeerSet;
typedef StlDeque<SessionID, ManagedAllocator<SessionID> > SessionQueue;
typedef StlMultiMap<UInt32, NpcLiveInfo*, std::less<UInt32>, 
			ManagedAllocator<std::pair<UInt32, NpcLiveInfo*> > > NpcLiveMap;
typedef StlMap<InstanceIndex, LPCPEER, std::less<InstanceIndex>, 
			ManagedAllocator<std::pair<InstanceIndex, LPCPEER> > > InstanceMap;
typedef StlVector<NpcCreateType, ManagedAllocator<NpcCreateType> > NpcCreateList;
typedef StlMap<MapIndex, NpcCreateList*, std::less<MapIndex>, 
			ManagedAllocator<std::pair<MapIndex, NpcCreateList*> > > NpcCreateMap;
typedef StlMap<UInt32, UInt32, std::less<UInt32>,
			ManagedAllocator<std::pair<UInt32, UInt32> > >	CharTimeCharacterEnterMap;
typedef StlMap<NpcFlagIndex, UInt16, std::less<NpcFlagIndex>, 
ManagedAllocator<std::pair<NpcFlagIndex, UInt16> > > NpcFlagMap;
typedef StlSet<NpcFlagIndex, std::less<NpcFlagIndex>, ManagedAllocator<NpcFlagIndex> > NpcFlagSet;

struct Global
{
	Reuben::Math::Random	m_rand;				// random number
	ThreadKey				m_key;
	MapIndexList			m_mapIndices;		// map index(with line_id) list
	PeerSet					m_mapPeers;			// map peer set
	Service					dummyService;
	Service*				pObjectService;
	Service*				pMessageService;
	Service*				pLogService;
	LPCPEER					pManagerPeer;		// manager peer
	AccountMap				m_accountMap;
	SYSTEMTIME				m_serviceTime;						
	CServerResManager*		m_resource;
	Boolean					m_bExit;
	Int32Vector				m_lineCounts;		// player count in each lines
	UInt8					m_loadLevel;		// current loading level
	SessionQueue			m_waitQueue;		// waiting session id queue
	UINT32					m_loginTime;		// last process of login account time
	NetGroup*				m_Rankings[MAX_CHAR_RANK_TYPE];
	NetGroup*				m_GuildRanks;
	CRankingControl *		m_rankingControl;
	CTaskScheduler*			m_pTaskScheduler;
	CChargeShopControl*		m_pChargeShopControl;
	NpcLiveMap				m_npcLiveMap;
	NpcCreateMap			m_npcCreateMap;
	InstanceMap				m_instanceMap;
	CSellYuanBaoGroup*		m_pSellYuanBaoGroup;
	CharTimeCharacterEnterMap	m_charTimeCharacterEnterMap;
	NpcFlagMap				m_AllNpcFlags;
	NpcFlagSet				m_npcFlagChgSet;

	Global(VOID)
		: pObjectService(&dummyService)
		, pMessageService(&dummyService)
		, pLogService(&dummyService)
		, pManagerPeer(NULL)
		, m_resource(NULL)
		, m_bExit(BOOLEAN_FALSE)
		, m_loadLevel(0)
		, m_loginTime(0)
	{
		Service emptyService = {};
		dummyService = emptyService;
	}

	CAccount* GetAccount(UInt32 acct_id);
	Boolean Initialize(VOID);
	VOID Uninitialize(VOID);
	Int32 GetLineCount(UInt8 line_id);
	UInt16 GetTotalCount();
	void IncLineCount(UInt8 line_id);
	void DecLineCount(UInt8 line_id);
	void ChgLineCount(UInt8 srcLine, UInt8 dstLine);
	UInt8 GetTotalLines();
	VOID CheckLoadLevel();
	void RegisterMap(const MapIndex& idx, LPCPEER pPeer, Boolean bOpen);
	void UnregisterMap(const MapIndex& idx);
	const MapInfo& GetMapInfo(const MapIndex& idx);
	const MapInfo& GetAvailMapInfo(const MapIndex& idx);
	void SetMapOpen(UInt16 map_id, Boolean bOpen);
	VOID OnMapServiceClose(LPCPEER pPeer);
	RPCNetID GetCharNetId(const UInt32 &char_id);
	CCharacterInfo* GetCharInfo(const UInt32 &char_id);
	VOID PreSendRankGroup(NetGroup * grp);
	VOID PreSendGuildRankGroup(NetGroup * grp);
	void GetLocalTime2(LPSYSTEMTIME st);
	void AddWaitQueue(const SessionID &sid);
	Boolean RemoveWaitQueue(const SessionID &sid);
	UInt32 CalcWaitTime();
	UInt32 GetAcctIDByCharID(const UInt32 &char_id);
	bool CanUsedName(const String &name);
	UInt8 GetMapType(UInt16 map_id);
	void GetInMapSessions(SessionIDVector &sessionList, PeerSet &proxySet);
	// char enter lock
	void SetCharacterEnterLock(UInt32 char_id);
	bool IsCharacterEnterLocked(UInt32 char_id);
	UInt32 GetCharacterEnterLockSince(UInt32 char_id);
	void RemoveCharacterEnterLock(UInt32 char_id);
	void UpdateGlobalNpcFlags();
};

INLINE Int32 Global::GetLineCount(UInt8 line_id)
{
	return (line_id < m_lineCounts.size() ? m_lineCounts[line_id] : -1);
}

extern Global gGlobal;

// Session functions
INLINE Session* AddSession(CONST SessionID& sid, CONST Session& session)
{
	return SessionManager::GetInstance().Add<Session>(sid, session);
}

INLINE Session* GetSession(CONST SessionID& sid)
{
	return SessionManager::GetInstance().Get<Session>(sid);
}

INLINE VOID ChangeSession(CONST SessionID& old_sid, CONST SessionID& sid)
{
	SessionManager::GetInstance().Change(old_sid, sid);
}

INLINE VOID RemoveSession(CONST SessionID& sid)
{
	SessionManager::GetInstance().Remove(sid);
}

INLINE VOID RemoveSession(CONST ServiceID& srvId)
{
	SessionManager::GetInstance().Remove(srvId);
}

INLINE VOID RedirectSessionId(SessionID& sid)
{
	SessionManager::GetInstance().RedirectSessionId(sid);
}

INLINE VOID SetOwnObject(const SessionID &sid, NetObject* obj)
{
	Session* psess = GetSession(sid);
	if (psess)
		psess->SetOwnObject(obj);
}

INLINE VOID UnsetOwnObject(const SessionID &sid, NetObject* obj)
{
	Session* psess = GetSession(sid);
	if (psess)
		psess->UnsetOwnObject(obj);
}

INLINE VOID ClearDeadSession(VOID)
{
	SessionManager::GetInstance().ClearDead();
}

// random functions
INLINE int GetRand(int imax)
{
	return gGlobal.m_rand.GetRandom(imax);
}

INLINE int GetRand(int imin, int imax)
{
	return (gGlobal.m_rand.GetRandom(imax - imin + 1) + imin);
}

INLINE float GetRand(float fmax)
{
	float r = (float) gGlobal.m_rand.GetRandom(INT32_MAX_VALUE);

	return ((r / INT32_MAX_VALUE) * fmax);
}

INLINE float GetRand(float fmin, float fmax)
{
	float r = (float) gGlobal.m_rand.GetRandom(INT32_MAX_VALUE);

	return ((r / INT32_MAX_VALUE) * (fmax - fmin) + fmin);
}

PCSTRINGA ConvAnsiString(const String &s);

template <typename T>
INLINE String GetVectorString(const T &v)
{
	String s;
	for (T::const_iterator it = v.begin(); it != v.end(); ++it) {
		if (s.empty())
			s.Format(_T("%d"), *it);
		else
			s.AppendFormat(_T(", %d"), *it);
	}
	return s;
}

#endif // _GLOBAL_H_
