#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "Common.h"
#include "Common/common_binary.h"
#include "Common/SessionManager.h"
#include "Reuben/Math/Random.h"


class CMap;
class CCharacter;
class ViewManager;
class PathFinder;
class CServerResManager;
class CBattle;
class CAnimTimer;
class CMessenger;
class CWorldEvent;
class CGuild;
class CGuildMember;
class CChargeShop;
class CMsgCharacterData;
class CInstance;
class CBatActor;
class CQuestNpcFlagData;

struct PathFindLoadData;
struct ItemData;
struct lua_State;

#define CONFIGURATION_INI_FILE		_T(".\\Data\\Setting\\fileconfig.ini")
#define NEW_SCRIPT_PATH				"./Data/NewScript/"
#define SCRIPT_NPC_PATH				(NEW_SCRIPT_PATH "Npc/")
#define SCRIPT_BAT_PATH				(NEW_SCRIPT_PATH "Battle/")
#define SCRIPT_ITEM_PATH			(NEW_SCRIPT_PATH "Item/")
#define SCRIPT_QUEST_PATH			(NEW_SCRIPT_PATH "Quest/")
#define SCRIPT_INST_PATH			(NEW_SCRIPT_PATH "Instance/")
#define QGLLIB_LUA					(NEW_SCRIPT_PATH "QGL_LIB.lua")
#define LUA_SRC_EXT					".lua"
#define LUA_BIN_EXT					".out"

	
class Session : public BaseSession
{
public:
	CCharacter*	pchar;
	Session() : pchar(NULL) {}
	virtual void Release(CONST SessionID &sid);
};

struct MsgKeyService
{
	UInt32 player_char_id;
	UInt32 friend_char_id;
	MsgKeyService()
	{
		player_char_id = 0;
		friend_char_id = 0;
	}

	MsgKeyService(UInt32 p_cid, UInt32 f_cid)
	{
		player_char_id = p_cid;
		friend_char_id = f_cid;
	}

	bool operator < (const MsgKeyService& b) const
	{
		return (player_char_id < b.player_char_id || (player_char_id == b.player_char_id && friend_char_id < b.friend_char_id));
	}

	bool operator == (const MsgKeyService& b) const
	{
		return (player_char_id == b.player_char_id && friend_char_id == b.friend_char_id);
	}
};

struct ViewRing
{
	ViewRing* next;
	ViewManager* child;
	UInt32 count;
	ViewRing()
		: next(NULL)
		, child(NULL)
		, count(0)
	{}
};

struct ItemLimit
{
	UInt32 item_id;
	UInt32 endTime;
	UInt16 stack;
};

typedef StlMap<MapIndex, CMap *, std::less<MapIndex>, 
		ManagedAllocator<std::pair<MapIndex, CMap *> > > MapIndexList;
typedef StlMap<ServiceID, LPCPEER, std::less<ServiceID>, 
		ManagedAllocator<std::pair<ServiceID, LPCPEER> > > BattlePeers;
typedef StlMap<UInt16, PathFinder*, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, PathFinder*> > > PathFinderMap;
typedef StlMap<UInt32, NPCDB*, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, NPCDB*> > > NpcDBMap;
typedef StlMap<UInt32, MobDB*, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, MobDB*> > > MobDBMap;
typedef StlVector<CBattle*, ManagedAllocator<CBattle*> > BattleList;
typedef StlMap<MsgKeyService, CMessenger*, std::less<MsgKeyService>, 
		ManagedAllocator< std::pair<MsgKeyService, CMessenger*> > > MessengerMap;
typedef StlMultiMap<UInt32, UInt16, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, UInt16> > > RankTitleMap;
typedef StlMap<UInt16, CWorldEvent*, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, CWorldEvent*> > > WorldEventMap;
typedef StlSet<UInt16, std::less<UInt16>, ManagedAllocator<UInt16> > ClosedMapSet;
typedef StlMap<UInt32, CMsgCharacterData*, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, CMsgCharacterData*> > > MsgCharDataMap;
typedef StlMap<UInt32, ItemLimit, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, ItemLimit> > > ItemLimitMap;
typedef StlMap<InstanceIndex, CInstance *, std::less<InstanceIndex>, 
		ManagedAllocator<std::pair<InstanceIndex, CInstance *> > > InstanceMap;
typedef StlMap<UInt32, CGuild*, std::less<UInt32>,
		ManagedAllocator<std::pair<UInt32, CGuild*> > > GuildMap;
typedef StlMap<UInt32, CGuildMember*, std::less<UInt32>,
		ManagedAllocator<std::pair<UInt32, CGuildMember*> > > GuildMemberMap;
typedef StlVector<UInt32, ManagedAllocator<UInt32> > MarriageTimeVec;

typedef StlMap<NpcFlagIndex, UInt16, std::less<NpcFlagIndex>, 
		ManagedAllocator<std::pair<NpcFlagIndex, UInt16> > > NpcFlagMap;

typedef StlVector<UInt32, ManagedAllocator<UInt32> > CharIncognitoPkLst;

class CGameStringTable;
class CharSessionRef
{
	typedef StlMap<UInt32, Session*, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, Session*> > > CharSessionMap;

public:
	CharSessionRef()	{m_charToSession.clear();};
	~CharSessionRef()	{m_charToSession.clear();};

private:
	CharSessionMap	m_charToSession;

public:
	CCharacter* FindCharacter(UInt32 char_id);
	void InsertCharToSession(UInt32 char_id, Session* session);
	void RemoveCharToSession(UInt32 char_id);
};

class Global
{
public:
	Boolean Initialize(void);
	void Uninitialize(void);
	CMap* GetMap(UInt16 map_id, UInt8 line_id, UInt32 owner_id=0);
	PCSTRING GetNpcName(UInt32 npc_id);
	PCSTRING GetMobName(UInt32 mob_id);
	PCSTRING GetMapName(UInt16 map_id);
	PCSTRING GetItemName(UInt32 item_id);
	void LinkView(ViewManager* viewMan);
	void UnlinkView(ViewManager* viewMan);
	void ChgLoadLevel(UInt8 level);
	bool LoadMapData();
	CMap* ActivateMap(MapIndex mapidx);
	CMap* DeactivateMap(MapIndex mapidx);
	BOOL ChangeDynamicMapOBJ(Int objid, Int state, Int16 lineid);	
	StringA	GetPrefixStr(UInt16 ID, StringA pre);
	BOOL CanUsedName(String name);
	VOID BlockWord(String &Msg);
	VOID ResetLuaState(lua_State * state);
	CGameStringTable* GetStringTable() {return m_pStringTable;};

	//guild war 
	VOID StartGuildWar();
	VOID EndGuildWar();


	Global(void)
		: pObjectService(&dummyService)
		, pLogService(&dummyService)
		, pWorldService(&dummyService)
		, pMessageService(&dummyService)
		, pManagerPeer(NULL)
		, m_resource(NULL)
		, m_playerCount(0)
		, m_viewRing(NULL)
		, m_animTimer(NULL)
		, m_bExit(BOOLEAN_FALSE)
		, m_loadLevel(0)
		, g_pLuaState(NULL)
		, g_pScriptState(NULL)
		, m_pStringTable(NULL)
	{
		Service emptyService = {};
		dummyService = emptyService;
	}

	CharSessionRef	m_charToSession;

	Service			dummyService;
	Service*		pObjectService;
	Service*		pLogService;
	Service*		pWorldService;
	Service*		pMessageService;
	LPCPEER			pManagerPeer;	// manager peer

	Reuben::Math::Random m_rand;	// random number
	Boolean			m_bExit;		// exit flag

	MapIndexList	m_mapIndices;
	CServerResManager* m_resource;

	lua_State*		g_pLuaState;		// global main lua state
	lua_State*		g_pScriptState;		// script system lua state (main lua state)

	UInt16			m_playerCount;		// current player count
	UInt8			m_loadLevel;		// current loading level
	ViewRing*		m_viewRing;
	BattleList		m_aliveBattles;		// existing battle
	BattleList		m_deadBattles;		// dead battle
	CAnimTimer*		m_animTimer;		// battle animation timer
	WorldEventMap	m_worldEventMap;	// world event map
	ClosedMapSet	m_closeMapSet;		// closed map id set
	ItemLimitMap	m_itemLimitMap;
	InstanceMap		m_instanceMap;		// instance map

	Int				g_ComboRate;
	double			g_TempResult;
	Int				g_MaxBattleTurn;

	RankTitleMap	g_RankTitleLst;
	GuildMap		g_AllGuilds;
	GuildMemberMap	g_AllGuildMembers;
	NetGroup		m_journalGroup;			// quest journal group
	CChargeShop*	m_pChargeShop;
	MsgCharDataMap	m_msgCharMap;		// updated message character data map

	CGameStringTable * m_pStringTable;

	UInt8	g_GuildWarState;
	MarriageTimeVec g_MarriageTimeLst;

	NpcFlagMap g_AllNpcFlags;

	Boolean			g_bShowItemPartnerPetOfOtherLine;

	CharIncognitoPkLst g_charParty1;
	CharIncognitoPkLst g_charParty2;
	UInt32	g_startIncognitoPkTime;

	bool	g_IncognitoPKTimeOver;
public:
	void GetServiceTime(SYSTEMTIME& time);
	void GetServiceTime(time_t& time);
	UInt32 GetServiceTime();
	void SetServiceTime(SYSTEMTIME st);
	int GetGameTime(int Mode);
	void LoadAllScript();					// load all lua scripts
	void LoadNewScript();					// load all new lua scripts
	void InitLua(lua_State* &lstate);
	void ReleaseLua(lua_State* &lstate);
	Boolean RunLua(lua_State* lstate, PCSTRINGA filename);
	Boolean LoadLua(lua_State* lstate, PCSTRINGA filename, Boolean bLog=BOOLEAN_FALSE);
	Boolean LoadLuaBuffer(lua_State* lstate, PCSTRINGA buf, size_t len, PCSTRINGA name = "");
	int LuaResume(lua_State* lstate);
	void NewLuaThread(lua_State* &lstate, lua_State* parent, int &refKey);
	void ReleaseLuaThread(lua_State* &lstate, lua_State* parent, int refKey);
	bool IsBlockName(String &Msg);
	UInt NoCodeStringSize(String &Text);
	const ItemData *	GetItemCsvData(UInt32 item_id);
	void UpdateMsgChar(CCharacter* pChar);		// mark update message character
	void SendMsgChars();						// send update message character to message service
	UInt8 GetMapType(UInt16 map_id);
	UInt32 GetMapOwnerId(UInt16 map_id, CCharacter* pchar);

	UInt32 GetRealDay(UInt32 time = 0);
	UInt32 GetRealMonth(UInt32 time = 0);
	UInt32 GetRealYear(UInt32 time = 0);
	UInt32 GetRealHour(UInt32 time = 0);
	UInt32 GetRealMinute(UInt32 time = 0);

	PCSTRINGA ConvertUTF8(const String &src);

	void AddCharInIncognitoPkLstOne(UInt32 char_id);
	void AddCharInIncognitoPkLstTwo(UInt32 char_id);
	void CharLstCreateParty();
	void CharLstLeaveParty();
	void SetCharLstBattleOver();
	void StartIncognitoPK();
	void LeaveParty();
	void RandomIncognitoBox(UInt16 map_id,UInt8 boxType);
	void RemoveOutCharInParty();
	void AddIncognitoPkItem(UInt8 num);
	bool IncognitoPkIsTimeOver() {return g_IncognitoPKTimeOver;};
private:
	void LoadQuestJournal();
	PathFinderMap		m_pathFinderMap;	
	SYSTEMTIME			m_serviceSysTime;
	SYSTEMTIME			m_syncTime;

};

INLINE CMap* Global::GetMap(UInt16 map_id, UInt8 line_id, UInt32 owner_id)
{
	MapIndex mi(map_id, line_id, owner_id);
	MapIndexList::iterator iter = m_mapIndices.find(mi);

	if (iter != m_mapIndices.end())
		return (iter->second);
	else
		return (NULL);
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

INLINE void ChangeSession(CONST SessionID& old_sid, CONST SessionID& sid)
{
	SessionManager::GetInstance().Change(old_sid, sid);
}

INLINE void RemoveSession(CONST SessionID& sid)
{
	SessionManager::GetInstance().Remove(sid);
}

INLINE void RemoveSession(CONST ServiceID& srvId)
{
	SessionManager::GetInstance().Remove(srvId);
}

INLINE void RedirectSessionId(SessionID& sid)
{
	SessionManager::GetInstance().RedirectSessionId(sid);
}

INLINE void SetOwnObject(const SessionID &sid, NetObject* obj)
{
	Session* psess = GetSession(sid);
	if (psess)
		psess->SetOwnObject(obj);
}

INLINE void UnsetOwnObject(const SessionID &sid, NetObject* obj)
{
	Session* psess = GetSession(sid);
	if (psess)
		psess->UnsetOwnObject(obj);
}

INLINE void ClearDeadSession(void)
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
int UTF8ToStr(PCSTRINGA src, StringW &dst);
int StrToUTF8(PCSTRINGW src, StringA &dst);

INLINE Boolean IsFileExist(PCSTRINGA filename)
{
	DWORD attr = GetFileAttributesA(filename);
	return (attr != INVALID_FILE_ATTRIBUTES && attr != FILE_ATTRIBUTE_DIRECTORY);
}

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
