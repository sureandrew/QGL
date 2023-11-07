#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "Common.h"
#include "Common/SessionManager.h"
#include "Common/common_binary.h"
#include "Reuben/Math/Random.h"
#include "DboSourcePool.h"
#include <oledb.h>
#include <deque>

#define DB_THREAD_POOL
//#define DB_WORKER_FLAG		WT_EXECUTEINPERSISTENTTHREAD	// only use persistent threads
//#define DB_WORKER_FLAG		WT_EXECUTEDEFAULT				// use all worker thread
#define DB_WORKER_FLAG		WT_EXECUTELONGFUNCTION		// use to create more worker threads
#ifdef DB_THREAD_POOL
	#define COINIT_FLAG		COINIT_MULTITHREADED
#else
	#define COINIT_FLAG		COINIT_APARTMENTTHREADED
#endif

class CDboSource;
class CdboAccount;
class CdboCharacter;
class CdboItem;
class CdboPartner;
class CdboSkill;
class CdboQuestLog;
class CdboQuestFlag;
class CdboQuestType;
class CServerResManager;
class CdboMessenger;
class CdboPet;
class CdboChargeShop;
class CdboTitle;
class CdboMsgCharacter;
class CdboGuild;
class CdboGuildMember;
class CdboGift;
class CdboHawkerHistory;
class CdboPartnerCollection;
class CdboSellYuanBao;
class CdboSibling;
class CdboGeneration;
class CdboMarriage;
class CdboQuestNpcFlag;


struct DatabaseTables
{
	CdboAccount*	pAccount;
	CdboCharacter*	pCharacter;
	CdboItem*		pItem;
	CdboPartner*	pPartner;
	CdboSkill*		pSkill;
	CdboQuestLog*	pQuestLog;
	CdboQuestFlag*	pQuestFlag;
	CdboQuestType*	pQuestType;
	CdboMessenger*	pMessenger;
	CdboPet*		pPet;
	CdboChargeShop*	pChargeShop;
	CdboTitle*		pTitle;
	CdboMsgCharacter*	pMsgCharacter;
	CdboGuild*			pGuild;
	CdboGuildMember*	pGuildMember;
	CdboGift*		pGift;
	CdboHawkerHistory*		pHawkerHistory;
	CdboPartnerCollection*	pPartnerCollection;
	CdboSellYuanBao* pSellYuanBao;
	CdboSibling* pSibling;
	CdboGeneration* pGeneration;
	CdboMarriage* pMarriage;
	CdboQuestNpcFlag* pQuestNpcFlag;

	DatabaseTables()
		: pAccount(NULL)
		, pCharacter(NULL)
		, pItem(NULL)
		, pPartner(NULL)
		, pSkill(NULL)
		, pQuestLog(NULL)
		, pQuestFlag(NULL)
		, pQuestType(NULL)
		, pMessenger(NULL)
		, pPet(NULL)
		, pChargeShop(NULL)
		, pTitle(NULL)
		, pMsgCharacter(NULL)
		, pGuild(NULL)
		, pGuildMember(NULL)
		, pGift(NULL)
		, pHawkerHistory(NULL)
		, pPartnerCollection(NULL)
		, pSellYuanBao(NULL)
		, pSibling(NULL)
		, pGeneration(NULL)
		, pMarriage(NULL)
		, pQuestNpcFlag(NULL)
	{}
};

struct DataObject
{
	UInt32		updateTime;		// last update to database time
	NetObject*	pobj;			// pointer to network object
	RPCNetID    objNetId;
	bool		alive;			// data object is alive

	DataObject()
		: updateTime(UINT32_MAX_VALUE)
		, pobj(NULL)
		, objNetId(RPCNetID())
		, alive(true)
	{}
};

struct MessengerDataKey
{
	UInt32	player_char_id;
	UInt32	friend_char_id;
	
	MessengerDataKey()
		: player_char_id(0)
		, friend_char_id(0)
	{}

	MessengerDataKey(UInt32 pid, UInt32	fid)
		: player_char_id(pid)
		, friend_char_id(fid)
	{}
	
	bool operator < (const MessengerDataKey& b) const
	{
		return (player_char_id < b.player_char_id || (player_char_id == b.player_char_id && friend_char_id < b.friend_char_id));
	}

	
};

struct GuildMemDataKey
{
	UInt32	player_char_id;
	UInt32	guild_id;
	
	GuildMemDataKey()
		: player_char_id(0)
		, guild_id(0)
	{}

	GuildMemDataKey(UInt32 pid, UInt32	gid)
		: player_char_id(pid)
		, guild_id(gid)
	{}
	
	bool operator < (const GuildMemDataKey& b) const
	{
		return (player_char_id < b.player_char_id || (player_char_id == b.player_char_id && guild_id < b.guild_id));
	}

	
};

struct SkillGroupKey
{
	UInt32	owner_id;
	UInt8	owner_type;
	
	SkillGroupKey()
		: owner_id(0)
		, owner_type(0)
	{}

	SkillGroupKey(UInt32 oid, UInt8	otype)
		: owner_id(oid)
		, owner_type(otype)
	{}
	
	bool operator < (const SkillGroupKey& b) const
	{
		return (owner_id < b.owner_id || (owner_id == b.owner_id && owner_type < b.owner_type));
	}

	
};

struct SkillDataKey
{
	UInt16	skill_id;
	UInt32	owner_id;
	UInt8	owner_type;
	
	SkillDataKey()
		: skill_id(0)
		, owner_id(0)
		, owner_type(0)
	{}

	SkillDataKey(UInt16	sid, UInt32 oid, UInt8	otype)
		:skill_id(sid)
		,owner_id(oid)
		,owner_type(otype)
	{}

	bool operator < (const SkillDataKey& b) const
	{
		return skill_id < b.skill_id
			|| (skill_id == b.skill_id && owner_id < b.owner_id)
			|| (skill_id == b.skill_id && owner_id == b.owner_id && owner_type < b.owner_type);
	}

	
};

struct GenerationDataKey
{
	UInt32	uiYoungerId;
	UInt32  uiElderId;
	BOOL    bYoung;

	GenerationDataKey()
		: uiYoungerId(0)
		, uiElderId(0)
		, bYoung(TRUE)
	{}

	GenerationDataKey(UInt32 uiYoungId, UInt32 uiEldId,BOOL bFlag)
		: uiYoungerId(uiYoungId)
		, uiElderId(uiEldId)
		, bYoung(bFlag)
	{}

	bool operator < (const GenerationDataKey& b) const
	{
		return (uiYoungerId < b.uiYoungerId || (uiYoungerId == b.uiYoungerId && uiElderId < b.uiElderId) || (uiYoungerId == b.uiYoungerId && uiElderId == b.uiElderId && bYoung < b.bYoung));
	}
};

struct GenerationGroupKey
{
	UInt32	uiOwnerId;
	BOOL bYoung;
	GenerationGroupKey()
		: uiOwnerId(0)
		,bYoung(TRUE)
	{}

	GenerationGroupKey(UInt32 uiId, BOOL	bflag)
		: uiOwnerId(uiId)
		, bYoung(bflag)
	{}

	bool operator < (const GenerationGroupKey& b) const
	{
		return (uiOwnerId < b.uiOwnerId || (uiOwnerId == b.uiOwnerId && bYoung < b.bYoung));
	}
};
struct QuestLogDataKey
{
	UInt32	owner_id;
	UInt16	quest_id;

	QuestLogDataKey()
		: owner_id(0)
		, quest_id(0)
	{}

	QuestLogDataKey(UInt32 new_oid, UInt16 new_qid)
		: owner_id(new_oid)
		, quest_id(new_qid)
	{}

	bool operator < (const QuestLogDataKey& b) const
	{
		return (owner_id < b.owner_id || (owner_id == b.owner_id && quest_id < b.quest_id));
	}

	
};

struct QuestTypeDataKey
{
	UInt32	owner_id;
	UInt16	type_id;

	QuestTypeDataKey()
		: owner_id(0)
		, type_id(0)
	{}

	QuestTypeDataKey(UInt32 newoid, UInt16 newtid)
		: owner_id(newoid)
		, type_id(newtid)
	{}

	bool operator < (const QuestTypeDataKey& b) const
	{
		return (owner_id < b.owner_id || (owner_id == b.owner_id && type_id < b.type_id));
	}
};

struct QuestFlagDataKey
{
	UInt32	owner_id;
	UInt16	flag_id;
	
	QuestFlagDataKey()
		: owner_id(0)
		, flag_id(0)
	{}

	QuestFlagDataKey(UInt32 newoid, UInt16 newfid)
		: owner_id(newoid)
		, flag_id(newfid)
	{}

	bool operator < (const QuestFlagDataKey& b) const
	{
		return (owner_id < b.owner_id || (owner_id == b.owner_id && flag_id < b.flag_id));
	}
};

template <typename _KEY_>
struct WorkRequest
{
	_KEY_ key;
	LPCPEER		pPeer;
	RPCContext	context;

	WorkRequest(_KEY_ _key, LPCPEER _pPeer, CONST RPCContext& _context)
		: key(_key)
		, pPeer(_pPeer)
		, context(_context)
	{}
};

typedef StlMap<UInt32, UInt32, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, UInt32> > > ReleaseMap;
typedef StlDeque<DataObject*, ManagedAllocator<DataObject*> > UpdateQueue;
typedef StlMap<UInt32, DataObject, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, DataObject> > > DataIdMap;
typedef StlMap<String, DataObject, std::less<String>, ManagedAllocator<std::pair<String, DataObject> > > DataNameMap;
typedef StlSet<String, std::less<String>, ManagedAllocator<String> > NameSet;
typedef StlMap<UInt32, DataObject, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, DataObject> > > GroupMap;
typedef StlMap<UInt32, UInt32Vector, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, UInt32Vector> > > RemainGiftMap;
typedef StlMap<UInt16, UInt16, std::less<UInt16>,
	ManagedAllocator<std::pair<UInt16, UInt16> > > UInt16Map;
typedef StlMap<UInt32, UInt16Map, std::less<UInt32>, 
	ManagedAllocator<std::pair<UInt32, UInt16Map> > > CharToUInt16MapMap;
// Skill
typedef StlMap<SkillGroupKey, DataObject, std::less<SkillGroupKey>, ManagedAllocator<std::pair<SkillGroupKey, DataObject> > > SkillGroupMap;
typedef StlMap<SkillDataKey, DataObject, std::less<SkillDataKey>, ManagedAllocator<std::pair<SkillDataKey, DataObject> > > SkillDataMap;
// Messenger
typedef StlMap<MessengerDataKey, DataObject, std::less<MessengerDataKey>, ManagedAllocator<std::pair<MessengerDataKey, DataObject> > > MessengerDataMap;
// Quest
typedef StlMap<QuestLogDataKey, DataObject, std::less<QuestLogDataKey>, ManagedAllocator<std::pair<QuestLogDataKey, DataObject> > >		QuestLogDataMap;
typedef StlMap<QuestFlagDataKey, DataObject, std::less<QuestFlagDataKey>, ManagedAllocator<std::pair<QuestFlagDataKey, DataObject> > >	QuestFlagDataMap;
typedef StlMap<QuestTypeDataKey, DataObject, std::less<QuestTypeDataKey>, ManagedAllocator<std::pair<QuestTypeDataKey, DataObject> > >	QuestTypeDataMap;
//Generation
typedef StlMap<GenerationGroupKey, DataObject, std::less<GenerationGroupKey>, ManagedAllocator<std::pair<GenerationGroupKey, DataObject> > > GenerationGroupMap;
typedef StlMap<GenerationDataKey, DataObject, std::less<GenerationDataKey>, ManagedAllocator<std::pair<GenerationDataKey, DataObject> > > GenerationDataMap;

//npc flag
typedef StlMap<NpcFlagIndex, DataObject, std::less<NpcFlagIndex>, ManagedAllocator<std::pair<NpcFlagIndex, DataObject> > > NpcFlagMap;
// Thread Pool
typedef BOOL(WINAPI * LPQueueWork)(LPTHREAD_START_ROUTINE, PVOID, ULONG);



struct Global
{
	DatabaseTables		gTables;
	Reuben::Math::Random m_rand;		// random number
	Boolean				m_bExit;		// service exit flag
	ReleaseMap			m_acctRelMap;	// account release map (acct_id, lastTime)
	ReleaseMap			m_charRelMap;	// character release map (char_id, lastTime)
	ReleaseMap			m_msgCharRelMap;	// message character release map (char_id, lastTime)
	UpdateQueue			m_updateQueue;	// update database queue
	NetGroup			m_releaseGroup;	// released NetObject group need for DB update
	NetGroup			m_deleteGroup;	// deleted NetObject group need for DB deletion
	CDboSource*			m_pAccountDb;	// main thread Account DB source
	CDboSource*			m_pGameDb;		// main thread Game DB source
	LPCPEER				pManagerPeer;	// manager peer
	LPCPEER				pLocalPeer;		// local loopback peer to self object service
	CServerResManager*	m_resource;
	LPQueueWork			QueueUserWorkItem;		// Thread Pool function pointer
	ByteArray           m_hashed_empty_pw;	

	NameSet				badAcctNames;
	DataIdMap			acctIdMap;
	DataNameMap			acctNameMap;
	DataIdMap			charIdMap;
	DataNameMap			charNameMap;
	DataIdMap			itemIdMap;
	GroupMap			itemGroupMap;
	DataIdMap			partnerIdMap;
	GroupMap			partnerGroupMap;
	SkillDataMap		skillMap;
	SkillGroupMap		skillGroupMap;
	QuestLogDataMap		questLogMap;
	GroupMap			questLogGroupMap;
	QuestFlagDataMap	questFlagMap;
	GroupMap			questFlagGroupMap;
	QuestTypeDataMap	questTypeMap;
	GroupMap			questTypeGroupMap;
	MessengerDataMap	messengerDataMap;
	GroupMap			messengerGroupMap;
	DataIdMap			petIdMap;
	DataIdMap			chargeShopIdMap;
	GroupMap			petGroupMap;
	DataIdMap			titleIdMap;
	GroupMap			titleGroupMap;
	DataIdMap			msgCharMap;
	DataIdMap			guildMap;
	DataIdMap			guildMemberMap;
	GroupMap			guildMemberGrpMap;
	DataIdMap			giftIdMap;
	//GroupMap			giftGroupMap;
	CDboSourcePool		acctDbPool;
	CDboSourcePool		gameDbPool;
	DataIdMap			hawkerHistoryIdMap;
	GroupMap			hawkerHistoryGroupMap;
	GroupMap			wareHouseItemGroupMap;
	GroupMap			wareHousePartnerGroupMap;
	GroupMap			wareHousePetGroupMap;
	DataIdMap			partnerCollectionIdMap;
	GroupMap			partnerCollectionGroupMap;
	GenerationDataMap   generationDataMap;
	GenerationGroupMap  generationGroupMap;
	NpcFlagMap          npcFlagMap;

	DataIdMap			sellYBIdMap;
	DataIdMap			siblingIdMap;
	DataIdMap			marriageIdMap;

	GroupMap			existItemGift;
	GroupMap			existPartnerGift;
	GroupMap			existPetGift;

	RemainGiftMap		remainItemGift;
	RemainGiftMap		remainPartnerGift;
	RemainGiftMap		remainPetGift;

	UInt32Vector		giftDelMap;

	ThreadKey			m_key;
	ThreadKey			m_updateKey;

// private
	CharToUInt16MapMap	m_charToUnreleasedTypeId;
	CharToUInt16MapMap	m_charToUnreleasedFlagId;

// private
	void AddUnreleasedId(UInt32 char_id, UInt16 element_id, CharToUInt16MapMap& unreleasedMap);
	UInt16 PopUnreleasedId(UInt32 char_id, CharToUInt16MapMap& unreleasedMap);	// return 0 whwn ended
	void ClearAllUnreleasedId(CharToUInt16MapMap& unreleasedMap);

// public
	void AddUnreleasedTypeId(UInt32 char_id, UInt16 type_id);
	UInt16 PopUnreleasedTypeId(UInt32 char_id);	// return 0 whwn ended
	void ClearAllUnreleasedTypeId();
	void AddUnreleasedFlagId(UInt32 char_id, UInt16 flag_id);
	UInt16 PopUnreleasedFlagId(UInt32 char_id);	// return 0 whwn ended
	void ClearAllUnreleasedFlagId();

	Global();
	Boolean Initialize(VOID);
	VOID Uninitialize(VOID);
	Boolean ConnectDatabase();
	Boolean ReleaseOLEDB();
	CDboSource* CreateSource(Boolean bAccount=BOOLEAN_FALSE);
	void ReleaseSource(CDboSource* dbsrc);
	void UpdateDb(UInt32 curTime, Boolean bForce=BOOLEAN_FALSE);	// update database
	// add data object into update queue
	void EnqueueUpdate(DataObject *dobj, bool atNow=false);
	void DequeueUpdate(DataObject *dobj);		// remove data object from update queue
	void CacheKeepAcct(UInt32 acct_id);			// keep account cache
	void CacheReleaseAcct(UInt32 acct_id);		// release account cache
	void CacheKeepChar(UInt32 char_id);			// keep character cache
	void CacheReleaseChar(UInt32 acct_id);		// release character cache
	void CacheKeepMsgChar(UInt32 char_id);		// keep msg character cache
	void CacheExtendMsgChar(UInt32 char_id);	// extend life time of msg character cache
	void CacheReleaseMsgChar(UInt32 char_id);	// release character cache
	void ReduceCache(UInt32 curTime);			// try to reduce object cache memory
	void WorkQueueBegin();						// call when working queue begin
	void WorkQueueEnd();						// call when working queue ended
	void WorkQueueStop();						// call when stopping all working queues
	void CheckValidData();						// check whether the database data is valid
	void CheckValidBuffData();					// check whether the database data is valid
	void CheckValidMapData();					// check whether the database data is valid

	void ExtractBuffString(String buff, UInt32 char_id);		//extract the buff with checking
	// callback got queue NetGroup
	void CB_QueueNetGroup(String funcName, HRESULT hr, String &dbErrMsg,
							RPCContext &context, NetGroup *grp);

private:
	LONG volatile	workCount;				// current thread working count
	HANDLE			completeEvent;			// all current thread working queue complete event
	UInt32			m_badNameTime;			// last bad name cache time
};

extern Global gGlobal;

void ReleaseAccount(UInt32 acct_id);
void ReleaseCharacters(UInt32 acct_id);
void ReleaseItems(UInt32 char_id);
void ReleasePartners(UInt32 char_id);
void ReleaseSkills(UInt32 owner_id, UInt8 owner_type);
void ReleaseQuests(UInt32 char_id);
void ReleaseMessengers(UInt32 char_id);
void ReleasePets(UInt32 char_id);
void ReleaseChargeShopItem(UInt32 chargeShopID);
void ReleaseMsgChar(UInt32 char_id);
void ReleaseGift(UInt32 acctID);
void ReleaseHawkerHistory(UInt32 char_id);
void ReleasePartnerCollections(UInt32 char_id);
void ReleaseGeneration(UInt32 uiCharId,BOOL bYoung);

void TimeConvert(const DBTIMESTAMP &fromTime, time_t &toTime);
void TimeConvert(const time_t &fromTime, DBTIMESTAMP &toTime);

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

#endif // _GLOBAL_H_
