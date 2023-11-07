#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "Common.h"
#include "Common/common_binary.h"
#include "Common/SessionManager.h"
#include "Common/ChannelType.h"
#include "Reuben/Math/Random.h"

#define MAX_MEMBER_EACH_GROUP	50
#define MAX_MEMBER_TEMP_GROUP	20

const UInt32 SpeakRestTime[MAX_CHANNELTYPE] = {2, 2, 30, 5, 2, 0, 2, 2, 2, 2, 2, 2, 0, 2};
const UInt32 MaxHorseLightMess = 30;
const UInt32 MaxMsgLongLen = 100;


class CChannel;
class CServerResManager;
class CMessageCharacter;
class CMessenger;
class CGuildControl;
class CGuild;
class CGuildMember;
class CMsgCharacterData;

class Session : public BaseSession
{
public:
	CMessageCharacter*	pMsgChar;
	RPCNetID			msgCharNetId;
	Session() :	pMsgChar(NULL) {}
	virtual VOID Release(CONST SessionID &sid);
};

struct MsgSchedule
{
	String msg;
	UInt32	startTime;
	UInt16	periodCount;
	UInt16	count;
	UInt32	period;
	UInt32	sendTimeCount;	
	UInt8	msgType;
	UInt32	srcID;

	MsgSchedule()
	{
		startTime = 0;
		periodCount = 0;
		count = 0;
		period = 0;
		sendTimeCount = 0;
		msgType = 0;
		srcID = 0;
	}
	~MsgSchedule()
	{
		msg.Empty();
	}
};

struct OfflineMsg
{
	UInt32 srcID;
	String name;
	String msg;
	OfflineMsg()
	{
	}
	OfflineMsg(UInt32 srcID, const String &name, const String &msg)
		: srcID(srcID), name(name), msg(msg)
	{
	}
	~OfflineMsg()
	{
		name.Empty();
		msg.Empty();
	}
};

struct CharMsgInfo
{
	SessionID  SID;
	CMessageCharacter * pMsgChar;
	NetGroup* messengers;
	UInt32	messengerCount[MAX_FRIEND_GROUP_TYPE];
	UInt32 PreSpeakTime;
	BOOL Listen;
	LPCPEER pProxyPeer;

	CharMsgInfo()
	{
		messengers = NULL;
		pMsgChar = NULL;
		PreSpeakTime = 0;
		Listen = TRUE;
		pProxyPeer = NULL;
		for (UInt i = 0; i < MAX_FRIEND_GROUP_TYPE; i++)
			messengerCount[i] = 0;
	}
	~CharMsgInfo()
	{
	}
};

struct ChannelCharData
{
	SessionID  SID;
	UInt32 PreSpeakTime;
	BOOL Listen;
	CMessageCharacter * pMsgChar;
	
	ChannelCharData()
	{
		PreSpeakTime = 0;
		Listen = TRUE;
		pMsgChar = NULL;
	}
	~ChannelCharData()
	{
	}
};

typedef StlSet<LPCPEER, std::less<LPCPEER>, ManagedAllocator<LPCPEER> > PeerSet;
typedef StlMap<MapIndex, LPCPEER, std::less<MapIndex>, 
			ManagedAllocator<std::pair<MapIndex, LPCPEER> > > MapIndexList;
typedef StlMap<UInt32, CharMsgInfo*, std::less<UInt32>, 
		ManagedAllocator< std::pair<UInt32, CharMsgInfo*> > > AllCharMap;
typedef StlMap<UInt32, CMessageCharacter*, std::less<UInt32>, 
		ManagedAllocator< std::pair<UInt32, CMessageCharacter*> > > MsgCharMap;
typedef StlMap<String, UInt32, std::less<String>, 
		ManagedAllocator< std::pair<String, UInt32> > > CharNameMap;
typedef StlMap<UInt32, ChannelCharData, std::less<UInt32>, 
		ManagedAllocator< std::pair<UInt32, ChannelCharData> > > ChannelCharMap;
typedef StlMap<String, CChannel*, std::less<String>, 
		ManagedAllocator< std::pair<String, CChannel*> > > ChannelNameMap;
typedef StlMultiMap<UInt32, OfflineMsg, std::less<UInt32>, 
		ManagedAllocator< std::pair<UInt32, OfflineMsg> > > OfflineMsgMap;
typedef StlMap<UInt32, CGuild*, std::less<UInt32>, 
		ManagedAllocator< std::pair<UInt32, CGuild*> > > GuildMap;
typedef StlMap<UInt32, MsgSchedule, std::less<UInt32>, 
		ManagedAllocator< std::pair<UInt32, MsgSchedule> > > MsgScheduleMap;
typedef StlSet<UInt32, std::less<UInt32>, ManagedAllocator<UInt32> > WatcherSet;
typedef StlMap<UInt32, WatcherSet, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, WatcherSet > > > WatcherMap;
typedef StlMap<LPCPEER, SessionIDVector, std::less<LPCPEER>, 
		ManagedAllocator< std::pair<LPCPEER, SessionIDVector> > > ProxySidMap;
typedef StlMap<UInt32, UInt32, std::less<UInt32>,
		ManagedAllocator<std::pair<UInt32, UInt32> > > CharGuildMap;


class CGameStringTable;
struct Global
{
	ChannelNameMap	m_MapChannel;
	AllCharMap		m_AllCharMap;			// All Player List
	CharNameMap		m_charNameMap;			// character name to char_id mapping
	MsgCharMap		m_msgCharMap;			// message character mapping
	OfflineMsgMap	m_AllOfflineMsg;
	WatcherMap		m_watcherMap;			// all character watcher map
	GuildMap		m_GuildMap;
	CGuildControl*	m_guildControl;
	Service*		pObjectService;
	Service*		pLogService;
	LPCPEER			pManagerPeer;			// manager peer
	MapIndexList	m_mapIndices;			// map index(with line_id) list
	PeerSet			m_mapPeers;				// map peer set
	ProxySidMap		m_proxySidMap;			// proxy peer contains session list map

	CServerResManager* m_resource;

	String			m_CurrHorseLightMsg;
	String			SysHorseLightMessage[MaxHorseLightMess];
	StringVector	ClientHorseLightMessageQuene;

	UInt			m_SysHLMsgIndex;
	UInt			m_NumOfClientHLMsg;
	UInt32			m_PreHLMsgUpdateTime;
	Boolean			m_bExit;
	UInt32			m_downTime;
	MsgScheduleMap	g_MsgScheduleMap;
	Reuben::Math::Random	m_rand;				// random number

	CharGuildMap m_guildApplyLock;

	CGameStringTable * m_pStringTable;

	UInt8		m_guildWarStatus;

	Global(VOID)
		: pObjectService(NULL)
		, pManagerPeer(NULL)
		, m_bExit(BOOLEAN_FALSE)
		, m_downTime(0)
		, m_resource(NULL)
		, m_guildWarStatus(255)
		, pLogService(NULL)
	{}

	Boolean Initialize(VOID);
	VOID Uninitialize(VOID);
	VOID LeaveAllChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &SourceId);
	VOID ReleaseChar(UInt32 char_id);
	VOID BlockWord(String &Msg);
	bool IsBlockName(String &Msg);
	VOID BlockColorWord(String &Msg);
	VOID OnUpdateCharStatus(CMessageCharacter* pMsgChar);
	VOID RemoveMessager(CMessenger * pMessenger);
	VOID InsertCharWatch(UInt32 viewer, UInt32 target);
	VOID RemoveCharWatch(UInt32 viewer, UInt32 target);
	VOID RemoveMessagerByOwnerID(UInt32 char_id);
	Boolean AddOfflineMsg(UInt32 targetId, UInt32 srcID, const String &name, const String &msg);
	bool IsBlockList(UInt32 listener, UInt32 speaker);
	CharMsgInfo* GetMsgInfo(UInt32 char_id);
	CharMsgInfo* GetMsgInfo(const String& name);
	CharMsgInfo* AddMsgInfo(UInt32 char_id, CMsgCharacterData* pData, SessionID &sid);
	void RemoveMsgInfo(UInt32 char_id);
	CMessageCharacter* AddMsgCharRef(UInt32 char_id, CMsgCharacterData* pData=NULL);
	CMessageCharacter* GetMsgCharRef(UInt32 char_id, bool bAdd=false);
	CMessageCharacter* GetMsgCharRef(const String& name);
	void ReleaseMsgCharRef(UInt32 char_id);
	void ShutdownAt(UInt32 downTime);
	void DestroyGameContext();
	CChannel* GetChannel(String ChannelName);
	CGuild* GetGuild(UInt32 guild_uid);
	CGuildMember* GetGuildMember(UInt32 guild_uid, UInt32 char_id);
	void RemoveGuild(CGuild* pGuild);
	void RegisterMap(const MapIndex& idx, LPCPEER pPeer);
	void UnregisterMap(const MapIndex& idx);
	LPCPEER GetMapPeer(const MapIndex& idx);
	VOID OnMapServiceClose(LPCPEER pPeer);
	VOID RemoveGuildApplicant(UInt32 char_id);
	UInt NoCodeStringSize(String &Text);

	VOID LockCharGuild(UInt32 char_id);
	VOID UnLockCharGuild(UInt32 char_id);
	BOOL IsLockCharGuild(UInt32 char_id);
	void AddProxySid(SessionID& sid);
	void RemoveProxySid(SessionID& sid);
	void UnregisterProxy(LPCPEER pPeer);

	CGameStringTable* GetStringTable() {return m_pStringTable;};
};

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

void WriteMsgLog(RPCContext &context, UInt channelType, UInt32 senderID, String &senderName, UInt32Vector &receiveIDs, String guildName, String &msg);

#endif // _GLOBAL_H_
