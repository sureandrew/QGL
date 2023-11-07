#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "Common/SessionManager.h"
#include "Common/common_binary.h"
#include "Reuben/Math/Random.h"
#include "Common/CallLuaFunc.h"
//#include "Resource/ServerResManager.h"
//#include "Common/common_char_map.h"

class CServerResManager;
struct BatClient;
class CBattle;
class CMapBattle;
class CBatActor;
class CAnimTimer;
struct SkillData;
struct lua_State;

struct Session
{
	BatClient*	pClient;
	CBattle*	pBattle;

	Session() : pClient(NULL), pBattle(NULL) {}
	VOID Release(CONST SessionID &sid);
};

typedef StlMap<Reuben::Network::ServiceID, Service*, std::less<Reuben::Network::ServiceID>, ManagedAllocator<std::pair<Reuben::Network::ServiceID, Service*> > > ServicePointerMap;
typedef StlMap<UInt32, NPCDB*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, NPCDB*> > > NpcDBMap;
typedef StlVector<BinaryServerNPCInstance*, ManagedAllocator<BinaryServerNPCInstance*> > NpcInstList;
typedef StlMap<UInt32, MobDB*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, MobDB*> > > MobDBMap;
typedef StlVector<CMapBattle*, ManagedAllocator<CMapBattle*> > MapBattleList;

struct Global
{
	SessionManager<Session> m_sessionManager;
	Service*				pObjectService;
	Service*				pMapService;
	CServerResManager*		m_resource;
	ServicePointerMap		mapServices;
	lua_State*				m_luaState;
	lua_State*				g_pParentBatLua;   // Don't use this lua state to do anything since it is used for create child state only !!!
	Boolean					m_bCreateChild;
	MapBattleList			m_aliveBattles;
	MapBattleList			m_deadBattles;
	Boolean					m_bExit;
	LPCPEER					pManagerPeer;	// manager peer
	CAnimTimer*				m_animTimer;

	Int		g_ComboRate;
	double	g_TempResult;
	Int		g_MaxBattleTurn;

	//SERVER

	BinaryBlockInfoHeader* m_mobdb_header;
	MobDBMap m_mobdb;
	int ImportServer_MOBInfo();

	Reuben::Math::Random m_rand;	// random number

	void DestroyGameContext();
	Boolean RunLua(const char* filename);
	Global();
	virtual ~Global();
	Boolean Initialize();
	VOID Uninitialize();

	bool IsValidTargetType(UInt8 targetType, CBatActor * pSrc, CBatActor * pTarget);
	StringA	GetPrefixStr(UInt16 ID, StringA pre);

	// Lua
	VOID InitParentBattleLua();
	VOID ReleaseParentBattleLua();
	lua_State * CreateChildState(int &RefKey);
};

extern Global gGlobal;

// Session functions
INLINE Session* AddSession(CONST SessionID& sid, CONST Session& session)
{
	return gGlobal.m_sessionManager.Add(sid, session);
}

INLINE Session* GetSession(CONST SessionID& sid)
{
	return gGlobal.m_sessionManager.Get(sid);
}

INLINE VOID ChangeSession(CONST SessionID& old_sid, CONST SessionID& sid)
{
	gGlobal.m_sessionManager.Change(old_sid, sid);
}

INLINE VOID RemoveSession(CONST SessionID& sid)
{
	gGlobal.m_sessionManager.Remove(sid);
}

INLINE VOID ClearDeadSession(VOID)
{
	gGlobal.m_sessionManager.ClearDead();
}

// random functions
INLINE int GetRand(int imax)
{
	return gGlobal.m_rand.GetRandom(imax);
}

INLINE int GetRand(int imin, int imax)
{
	return (gGlobal.m_rand.GetRandom(imax - imin) + imin);
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

const char* ConvAnsiString(const String &s);


#endif // _GLOBAL_H_