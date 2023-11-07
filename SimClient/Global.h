#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "Common.h"
#include "Reuben/Math/Random.h"
#include "Reuben/Network/MiddleFilter.h"
#include "SimClient/SimClient.h"

class CSimClient;
class CAccount;
class CCharacter;
class CMap;
class CBatCharacter;
class CBatMob;
class CBattle;
class CChannel;
class CItemBag;
class CClientResManager;
class CScriptSystem;
class PathFinder;

struct MapDB_Client;
struct lua_State;

typedef StlVector<String, ManagedAllocator<String> > StringList;
typedef Reuben::Utility::Singleton<Reuben::Utility::ProcessManager> ProcessManager;

#define OPT_SHOW_SELF		0x0001				// show self update
#define OPT_SHOW_OTHER		0x0002				// show other update
#define OPT_SHOW_ITEM		0x0004				// show item update
#define OPT_SHOW_MOVE		0x0008				// show each move step position
#define OPT_AUTO_SELECT		0x0010				// auto select character to play
#define OPT_AUTO_PING		0x0020				// auto select character to play
#define OPT_ALL				0xFFFF				// all options

struct PeerData
	: public Reuben::Network::FilterData
{
	CSimClient* pSimClient;

	PeerData() : pSimClient(NULL)
	{
	}

	virtual VOID OnDispose(VOID)
	{
		PeerData* pData = const_cast<PeerData*>(this);
		SafeDeleteObject(pData);
		pSimClient = NULL;
	}
};

class Global : public Reuben::Network::ISocketHandler
{
public:
	typedef StlMap<SocketID, CSimClient*, std::less<SocketID>, 
		ManagedAllocator<std::pair<SocketID, CSimClient*> > > ClientSocketMap;
	typedef StlMap<String, CSimClient*, std::less<String>, 
		ManagedAllocator<std::pair<String, CSimClient*> > > ClientNameMap;
	typedef StlMap<UInt16, MapDB_Client*, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, MapDB_Client*> > > MapDbList;
	typedef StlMap<UInt16, PathFinder*, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, PathFinder*> > > PathFinderMap;

	CClientResManager *m_resource;
	MapDbList		m_mapDbList;
	PathFinderMap	m_pathFinders;
	lua_State*		m_pLuaState;
	StringList		m_msgList;
	Logger*			m_defaultLogger;
	CSimClient*		m_pSimClient;			// current SimClient
	CSimClient*		m_controlClient;		// controlled SimClient
	ClientSocketMap	m_clientSockets;		// SimClient socket Map
	ClientNameMap	m_clientNames;			// SimClient name Map
	UInt32			m_option;				// SimClient option flag
	Boolean			m_bExit;				// need to exit?
	Boolean			m_bMulti;				// is multiple client?
	UInt32			m_multInterval;			// multiple client interval startup time
	UInt32			m_robotInterval;		// robot script call interval
	UInt32			m_startRunTime;			// SimClient start running time
	ThreadKey		m_key;					// multi-thread key to lock
	LPCPEER			m_pManagerPeer;			// manager peer
	String			m_prog;					// SimClient program full path
	String			m_title;				// SimClient title name
	String			m_cmdLine;				// SimClient program command line
	String			m_workDir;				// SimClient working path
	UInt32			m_downTime;				// services shutting down time
	UInt8			m_nextLine;				// next selection line
	UInt8			m_shadowNum;			// no. of shadow account
	UInt32			m_clientTotal;			// no. of total running clients

	Reuben::Math::Random m_rand;	// random number

	Global();
	virtual ~Global();
	Boolean Initialize(VOID);
	VOID Uninitialize(VOID);
	void Update(CONST UInt32 dt);

	void ReadAuto(const Char *filename);
	void Respond(const Char *msg);
	MapDB_Client* GetMapDb(UInt16 mapid);
	PathFinder* GetPathFinder(MapDB_Client* mapdb, UInt16 mapid);
	void SetLogger(Logger* pLogger);
	Boolean LoadMapData(UInt16 map_id);
	Boolean RunLua(lua_State* luaState, const char* filename);
	void NewLuaThread(lua_State* &lstate, lua_State* parent, int &refKey);
	void ReleaseLuaThread(lua_State* &lstate, lua_State* parent, int refKey);
	void ReadOption(const TCHAR *optLine);
	void ShowOption();
	void LoadChatMessage();
	void SingleClient(const Char *accountName, const Char *password);
	void MultiClient(const Char *listFile);
	void MultiClient(const Char *startName, const Char *endName);
	void LoadRobotLua(const Char *scriptFile);
	void AddClient(CSimClient* pSimClient);
	void SwitchClient(const Char *accountName);
	void RegisterManager(const Char *prog, String &title, String &args);
	void RestartClient(UInt32 atTime);
	void CheckDownTime(UInt32 dt);
	PCSTRING GetNpcName(UInt32 npc_id);
	PCSTRING GetMapName(UInt16 map_id);

private:
	virtual VOID OnAccept(CONST Listener* listener, CONST Peer* pPeer);
	virtual VOID OnFull(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener, CONST Peer* pPeer);
	virtual VOID OnClose(CONST Listener* listener);
	virtual VOID OnClose(CONST Listener* listener, CONST Peer* pPeer);

	Logger* CreateLogger(const Char *accountName);
};

extern Global gGlobal;

// customized pre-receive function handler for multiple clients
Boolean RecvHandle(LPCPEER pPeer, RPCContext &context);

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

INLINE void SetOption(UInt32 optvalue, bool enable)
{
	if (enable)
		gGlobal.m_option |= optvalue;
	else
		gGlobal.m_option &= ~optvalue;
}

INLINE bool IsOption(UInt32 optvalue)
{
	return ((gGlobal.m_option & optvalue) == optvalue);
}

const char* ConvAnsiString(const String &s);
int UTF8ToStr(PCSTRINGA src, StringW &dst);
int StrToUTF8(PCSTRINGW src, StringA &dst);
void RunNewGame();

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
