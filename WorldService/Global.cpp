//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "WorldService/Global.h"
//-- Library
#include "Proxy/Service/MapService_proxy.h"
#include "Stub/Logic/Account.h"
#include "Stub/Logic/CharacterInfo.h"
#include "Stub/Logic/RankCharacter.h"
#include "Stub/Logic/RankGuild.h"
#include "WorldService/ChargeShopControl.h"
#include "Stub/Service/WorldService_stub.h"
#include "WorldService/WorldApplication.h"
#include "WorldService/Configure.h"
#include "WorldService/RankingControl.h"
#include "WorldService/TaskScheduler.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResMap.h"
#include "Stub/Logic/SellYuanBaoGroup.h"
#include "Proxy/Data/QuestNpcFlagData.h"
#include "Proxy/Service/ObjectService_proxy.h"

Session::Session()
	: pAccount(NULL), m_pPeer(NULL), ip(INVALID_IP)
{
}

VOID Session::SetSessionId(CONST SessionID &sid)
{
	m_pPeer = WorldApplication::GetInstance().GetProxy(sid.serviceId);
	m_context.sessionId = sid;
}

VOID Session::Release(CONST SessionID &sid)
{
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("Session Deleted: session %s"), 
		sid.GetString().c_str());

	if (pAccount)
		UnsetOwnObject(pAccount);

	ProcedureManager::GetInstance().RemoveBySession(sid);
}

Boolean Global::Initialize(VOID)
{
	m_rand.Randomize();
	for (int i = 0; i < MAP_MAX_LINE; i++)
	{
		m_lineCounts.push_back(-1);
	}

	ZeroMemory(&m_serviceTime, sizeof(SYSTEMTIME));
	Reuben::Utility::ConfigFile config(_T("Config/LoginSetting.ini"));

	m_serviceTime.wYear = config.GetInt(_T("ServiceTime"), _T("YEAR"), 0);
	m_serviceTime.wMonth = config.GetInt(_T("ServiceTime"), _T("MOUTH"), 0);
	m_serviceTime.wDay = config.GetInt(_T("ServiceTime"), _T("DAY"), 0);
	m_serviceTime.wHour = config.GetInt(_T("ServiceTime"), _T("HOUR"), 0);

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Resource Manager..."));
	m_resource = SafeCreateObject(CServerResManager);
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Start Character Attribute..."));
	m_resource->LoadStartCharCSV();
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Block Word..."));
	m_resource->LoadBlockWordCSV();
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Event Schedule..."));
	m_resource->LoadEventScheduleCSV();
	m_pTaskScheduler = SafeCreateObject(CTaskScheduler);
	m_pTaskScheduler->LoadEvent();
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Map Data..."));
	m_resource->LoadMapDAT();
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Title..."));
	m_resource->LoadTitleCSV();
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading ChargeShop List..."));
	m_resource->LoadChargeShopCSV();
	for (UInt i = 0; i < MAX_CHAR_RANK_TYPE; i ++)
	{
		m_Rankings[i] = NULL;
	}
	m_GuildRanks = NULL;
	m_rankingControl = SafeCreateObject(CRankingControl);
	m_pChargeShopControl = SafeCreateObject(CChargeShopControl);
	m_pSellYuanBaoGroup = CSellYuanBaoGroup::NewInstance();
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Done.  Finish reading"));

	// add empty session ID for services ownership
	SessionID sid;
	AddSession(sid, Session());

	return (BOOLEAN_TRUE);
}

VOID Global::Uninitialize(VOID)
{
	// remove all existing account
	for (AccountMap::iterator it = m_accountMap.begin(); it != m_accountMap.end(); ++it)
		DeleteNetObject(it->second);
	m_accountMap.clear();

	SafeDeleteObject(m_resource);

	SafeDeleteObject(m_pTaskScheduler);
	for (UInt i = 0; i < MAX_CHAR_RANK_TYPE; i ++)
	{
		if (m_Rankings[i])
			DeleteNetGroup(m_Rankings[i], BOOLEAN_TRUE);
		m_Rankings[i] = NULL;
	}
	DeleteNetGroup(m_GuildRanks, BOOLEAN_TRUE);
	SafeDeleteObject(m_rankingControl);
	SafeDeleteObject(m_pChargeShopControl);
	DeleteNetGroup(m_pSellYuanBaoGroup, BOOLEAN_TRUE);

	for (NpcLiveMap::iterator it = m_npcLiveMap.begin(); it != m_npcLiveMap.end(); ++it)
		SafeDeleteObject(it->second);
	m_npcLiveMap.clear();

	// remove all npc creation buffer map
	for (NpcCreateMap::iterator it = m_npcCreateMap.begin(); it != m_npcCreateMap.end(); ++it)
		SafeDeleteObject(it->second);
	m_npcCreateMap.clear();
	m_lineCounts.clear();
}

void Global::RegisterMap(const MapIndex& idx, LPCPEER pPeer, Boolean bOpen)
{
	if (pPeer == NULL) return;

	ThreadGuard guard(m_key);
	m_mapIndices[idx] = MapInfo(idx, pPeer->AddRef(), bOpen);
	m_mapPeers.insert(pPeer);

	MapService_Proxy::RankTitleUpdate(pPeer, RPCContext(), m_rankingControl->m_char_ids, m_rankingControl->m_title_ids);

	// set line available
	if (idx.line_id < m_lineCounts.size() && m_lineCounts[idx.line_id] == -1)
		gGlobal.m_lineCounts[idx.line_id] = 0;
}

void Global::UnregisterMap(const MapIndex& idx)
{
	ThreadGuard guard(m_key);
	MapIndexList::iterator itfind = m_mapIndices.find(idx);
	if (itfind != m_mapIndices.end())
	{
		MapInfo &mInfo = itfind->second;
		SafeRelease(mInfo.pPeer);
		m_mapIndices.erase(itfind);
	}

	// set line unavailable
	if (idx.line_id < gGlobal.m_lineCounts.size())
		gGlobal.m_lineCounts[idx.line_id] = -1;
}

const MapInfo& Global::GetMapInfo(const MapIndex& idx)
{
	static MapInfo dummyInfo;
	ThreadGuard guard(m_key);
	MapIndexList::const_iterator iter = m_mapIndices.find(idx);
	if (iter != m_mapIndices.end())
		return iter->second;
	return dummyInfo;
}

const MapInfo& Global::GetAvailMapInfo(const MapIndex& idx)
{
	if (idx.line_id != 0 || idx.owner_id != 0)
		return GetMapInfo(idx);

	static MapInfo dummyInfo;
	ThreadGuard guard(m_key);
	MapIndex newIdx(idx.map_id, 0, 0);
	UInt8 line_id = 0;
	Int32 leastCount = INT32_MAX_VALUE;

	// found the least available map and line_id 
	for (UInt i = 1; i < MAP_MAX_LINE; i++)
	{
		newIdx.line_id = i;
		MapIndexList::const_iterator iter = m_mapIndices.find(newIdx);
		if (iter != m_mapIndices.end() && m_lineCounts[i] >= 0)
		{
			const MapInfo& mapInfo = iter->second;
			if (mapInfo.bOpen && m_lineCounts[i] < leastCount)
			{
				line_id = i;
				leastCount = m_lineCounts[i];
			}
		}
	}

	if (line_id != 0)
	{
		newIdx.line_id = line_id;
		MapIndexList::const_iterator iter = m_mapIndices.find(newIdx);
		return iter->second;
	}
	else
	{
		return dummyInfo;
	}
}

void Global::SetMapOpen(UInt16 map_id, Boolean bOpen)
{
	for (MapIndexList::iterator it = m_mapIndices.begin(); it != m_mapIndices.end(); ++it)
	{
		if (it->first.map_id == map_id) {
			MapInfo& mInfo = it->second;
			mInfo.bOpen = bOpen;
		}
	}
}

VOID Global::OnMapServiceClose(LPCPEER pPeer)
{
	ThreadGuard guard(m_key);
	for (MapIndexList::iterator it = m_mapIndices.begin(); it != m_mapIndices.end();)
	{
		MapInfo& mInfo = it->second;
		if (mInfo.pPeer == pPeer) 
		{
			SafeRelease(mInfo.pPeer);
			it = m_mapIndices.erase(it);
		} 
		else 
		{
			++it;
		}
	}
	m_mapPeers.erase(pPeer);
}

CCharacterInfo* Global::GetCharInfo(const UInt32 &char_id)
{
	CAccount* pAccount;
	CCharacterInfo* pCharInfo;
	for (AccountMap::iterator it = m_accountMap.begin(); it != m_accountMap.end(); ++it)
	{
		pAccount = it->second;
		if (pAccount)
		{
			pCharInfo = pAccount->GetLoginChar();
			if (pCharInfo && pCharInfo->GetChar_id() == char_id)
				return pCharInfo;
		}
	}

	return NULL;
}

RPCNetID Global::GetCharNetId(const UInt32 &char_id)
{
	CCharacterInfo* pCharInfo = GetCharInfo(char_id);
	if (pCharInfo)
		return pCharInfo->GetNetId();
	else
		return RPCNetID();
}

UInt32 Global::GetAcctIDByCharID(const UInt32 &char_id)
{
	CCharacterInfo* pCharInfo = GetCharInfo(char_id);
	if (pCharInfo)
		return pCharInfo->GetAcct_id();
	else
		return 0;
}

UInt16 Global::GetTotalCount()
{
	UInt16 total = 0;
	for (int i = 0; i < (int)gGlobal.m_lineCounts.size(); i++)
	{
		if (gGlobal.m_lineCounts[i] > 0)
			total += (UInt16) gGlobal.m_lineCounts[i];
	}

	return total;
}

void Global::IncLineCount(UInt8 line_id)
{
	if (line_id >= m_lineCounts.size() || m_lineCounts[line_id] < 0)
		return;

	m_lineCounts[line_id]++;
	CheckLoadLevel();
}

void Global::DecLineCount(UInt8 line_id)
{
	if (line_id >= m_lineCounts.size() || m_lineCounts[line_id] <= 0)
		return;

	m_lineCounts[line_id]--;
	CheckLoadLevel();
}

void Global::ChgLineCount(UInt8 srcLine, UInt8 dstLine)
{
	if (srcLine >= m_lineCounts.size() || dstLine >= m_lineCounts.size())
		return;

	if( srcLine==0 && m_lineCounts[srcLine]==-1 )
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("srcLine==0 && lineCounts[srcLine]==-1"));
	}

	m_lineCounts[srcLine]--;
	m_lineCounts[dstLine]++;
}

UInt8 Global::GetTotalLines()
{
	return ((UInt8) m_lineCounts.size());
}

VOID Global::PreSendRankGroup(NetGroup * grp)
{
	if (grp)
	{
		for (int i = 0; i < grp->GetCount(); ++i)
		{
			CRankCharacter *pRankChar = ((NetGroup*) grp)->GetChild<CRankCharacter>(i);
			TRACE_ENSURE(pRankChar != NULL);
			pRankChar->PreSend();
		}
	}
}

VOID Global::PreSendGuildRankGroup(NetGroup * grp)
{
	if (grp)
	{
		for (int i = 0; i < grp->GetCount(); ++i)
		{
			CRankGuild *pRankGuild = ((NetGroup*) grp)->GetChild<CRankGuild>(i);
			TRACE_ENSURE(pRankGuild != NULL);
			pRankGuild->PreSend();
		}
	}
}
VOID Global::CheckLoadLevel()
{
	UInt16 total = GetTotalCount();

	// not reached level border
	if (total < (UInt16) gConfigure.baseLoadLevel || 
		((total - gConfigure.baseLoadLevel) % gConfigure.nextLoadLevel) != 0)
		return;

	UInt8 level = (UInt8) ((total - gConfigure.baseLoadLevel) / gConfigure.nextLoadLevel);

	if (level == m_loadLevel)			// same level, no need to change
		return;

	TRACE_INFODTL_3(GLOBAL_LOGGER, 
		_T("ChangeLoadLevel: old level %d, new level %d, player count %d"),
		m_loadLevel, level, total);

	m_loadLevel = level;
	// change all loading level in all map service
	{
		ThreadGuard guard(m_key);
		LPCPEER pPeer;
		RPCContext context;
		
		for (PeerSet::iterator it = m_mapPeers.begin(); it != m_mapPeers.end(); ++it)
		{
			pPeer = *it;
			MapService_Proxy::ChangeLoadLevel(pPeer, context, m_loadLevel);
		}
	}
}

CAccount* Global::GetAccount(UInt32 acct_id)
{
	AccountMap::iterator it = m_accountMap.find(acct_id);
	if (it != m_accountMap.end())
		return it->second;
	else
		return NULL;
}

void Global::GetLocalTime2(LPSYSTEMTIME st)
{
//#ifdef _DEBUG
	SYSTEMTIME nowst;
	GetLocalTime(&nowst);
	CTime nowtime = CTime(nowst);

	FILETIME fileTime;
	SystemTimeToFileTime( &nowst, &fileTime );

	ULARGE_INTEGER uli;
	uli.LowPart = fileTime.dwLowDateTime; // could use memcpy here!
	uli.HighPart = fileTime.dwHighDateTime;

	static __time64_t init_t_time = 0;
	static ULONGLONG init_large_time = 0;
	if( init_t_time==0 )
	{
		init_t_time = nowtime.GetTime();
		init_large_time = uli.QuadPart;
	}

	UInt32 twistedTimeDiff = (UInt32)(((uli.QuadPart - init_large_time)/10000 * gConfigure.eventTimeTwistMultiply)/1000);
	CTime twistedtime = CTime(init_t_time) + CTimeSpan(twistedTimeDiff/86400,(twistedTimeDiff%86400)/3600,(twistedTimeDiff%3600)/60,twistedTimeDiff%60);

	SYSTEMTIME initst, twistedst;
	twistedtime.GetAsSystemTime(twistedst);
	CTime(init_t_time).GetAsSystemTime(initst);
	twistedtime.GetAsSystemTime(*st);
//#else
//	GetLocalTime(&*st);
//#endif
}

void Global::AddWaitQueue(const SessionID &sid)
{
	m_waitQueue.push_back(sid);
}

Boolean Global::RemoveWaitQueue(const SessionID &sid)
{
	// remove session id in waiting queue
	for (SessionQueue::iterator it = m_waitQueue.begin(); 
		it != m_waitQueue.end(); ++it)
	{
		if (*it == sid) {
			m_waitQueue.erase(it);
			return BOOLEAN_TRUE;
		}
	}
	return BOOLEAN_FALSE;
}

UInt32 Global::CalcWaitTime()
{
	if (m_loginTime < (UInt32) gConfigure.loginInterval)
		return (((UInt32) m_waitQueue.size() + 1) * gConfigure.loginInterval - m_loginTime);
	else
		return 0;
}

PCSTRINGA ConvAnsiString(const String& s)
{
	static char buf[1024];
	int len;

	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), (int)s.length(), 
			buf, 1024, 0, 0);
	if (len > 0)
		buf[len] = '\0';
	else
		buf[0] = '\0';

	return buf;
}

bool Global::CanUsedName(const String &name)
{
	BlockWordDataList* blockWordPtr = gGlobal.m_resource->GetNameBlockWordList();
	if (blockWordPtr == NULL)
		return false;

	BlockWordDataList& blockWord = *blockWordPtr;

	String checkingWord;// = nickName;
	String space = _T(" ");
	int spaceIndex = 0;
	bool cantUse = false;//cannot use

	spaceIndex = (int)name.find(space.c_str(), spaceIndex);
	if (spaceIndex < 0)
	{
		checkingWord = name;
		checkingWord = checkingWord.ToLower();

		for (BlockWordDataList::iterator it = blockWord.begin(); it != blockWord.end(); ++it)
		{
			String& blockWordCompare = *it;
			int tempPos = (int)checkingWord.find(blockWordCompare.c_str());
			if (tempPos >= 0)
			{
				cantUse = true;	
				break;
			}
		}
	}
	else
		cantUse = true;	

	return (!cantUse);
}

UInt8 Global::GetMapType(UInt16 map_id)
{
	const MapDB* mapdb = m_resource->GetMapData(map_id);
	if (mapdb)
		return mapdb->m_basicinfo->m_maptype;
	else
		return 0;
}


void Global::GetInMapSessions(SessionIDVector &sessionList, PeerSet &proxySet)
{
	CAccount* pAccount;
	Session* psess;

	sessionList.clear();
	proxySet.clear();
	for (AccountMap::iterator it = m_accountMap.begin(); it != m_accountMap.end(); ++it)
	{
		pAccount = it->second;
		if (pAccount->IsConnected() && pAccount->GetLoginChar())
		{
			psess = GetSession(pAccount->m_lastSessId);
			if (psess && psess->GetPeer())
			{
				sessionList.push_back(pAccount->m_lastSessId);
				proxySet.insert(psess->GetPeer());
			}
		}
	}
}

void Global::SetCharacterEnterLock(UInt32 char_id)
{
	time_t now;
	time(&now);
	m_charTimeCharacterEnterMap[char_id] = (UInt32)now;
}

bool Global::IsCharacterEnterLocked(UInt32 char_id)
{
	return m_charTimeCharacterEnterMap.find(char_id)!=m_charTimeCharacterEnterMap.end();
}

UInt32 Global::GetCharacterEnterLockSince(UInt32 char_id)
{
	CharTimeCharacterEnterMap::iterator itr = m_charTimeCharacterEnterMap.find(char_id);
	if( itr != m_charTimeCharacterEnterMap.end() )
		return itr->second;
	return 0;
}

void Global::RemoveCharacterEnterLock(UInt32 char_id)
{
	CharTimeCharacterEnterMap::iterator itr = m_charTimeCharacterEnterMap.find(char_id);
	if( itr != m_charTimeCharacterEnterMap.end() )
		m_charTimeCharacterEnterMap.erase(itr);
}
void Global::UpdateGlobalNpcFlags()
{
	// create/update/delete quest npc flags
	CQuestNpcFlagData* pQuestNpcFlagData = NULL;
	NetGroup grpDelete;	
	NetGroup grpUpdate;

	for (NpcFlagSet::iterator it = m_npcFlagChgSet.begin(); it != m_npcFlagChgSet.end();++it)
	{
		NpcFlagIndex flagIndex = *it;
		pQuestNpcFlagData = CreateNetObject<CQuestNpcFlagData>();
		pQuestNpcFlagData->SetNpc_id(flagIndex.npc_id);
		pQuestNpcFlagData->SetLine_id(flagIndex.line_id);
		pQuestNpcFlagData->SetOwner_id(flagIndex.owner_id);
		pQuestNpcFlagData->SetFlag_id(flagIndex.flag_id);
		NpcFlagMap::iterator it2 = gGlobal.m_AllNpcFlags.find(flagIndex);
		if (it2 == gGlobal.m_AllNpcFlags.end()) 
		{	
			pQuestNpcFlagData->SetSendMask(pQuestNpcFlagData->GetUpdateMask());
			grpDelete.Add(pQuestNpcFlagData);	
		} 
		else 
		{
			pQuestNpcFlagData->SetFlag(it2->second);
			pQuestNpcFlagData->SetSendMask(pQuestNpcFlagData->GetUpdateMask());
			grpUpdate.Add(pQuestNpcFlagData);
		}
	}

	m_npcFlagChgSet.clear();								// clear changed flag

	if (grpDelete.GetCount() == 0 && grpUpdate.GetCount() == 0) 
	{		// nothing to update
		return;
	}

	ObjectService_Proxy::UpdateNpcFlags(gGlobal.pObjectService->pPeer, RPCContext(), &grpUpdate, &grpDelete);
	DeleteNetChilds(&grpUpdate);			// delete data object after send
	DeleteNetChilds(&grpDelete);			// delete data object after send
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("m_npcFlagChgSet.size() = %d"),m_npcFlagChgSet.size());
}