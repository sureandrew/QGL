//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Logic/QuestSystem.h"
#include "Common/CallLuaFunc.h"
//-- Library
#include "Stub/Logic/Character.h"
#include "Stub/Logic/MapNPC.h"
#include "Stub/Logic/Map.h"
#include "Stub/Logic/QuestLog.h"
#include "Stub/Logic/QuestType.h"
#include "Stub/Logic/QuestJournal.h"
#include "Proxy/Data/QuestLogData.h"
#include "Proxy/Data/QuestTypeData.h"
#include "Proxy/Data/QuestFlagData.h"
#include "Proxy/Data/QuestNpcFlagData.h"
#include "Proxy/Service/ObjectService.h"
#include "Proxy/Service/LogService.h"
#include "Proxy/Service/WorldService.h"
#include "MapService/Global.h"
#include "Resource/ResRandTable.h"
#include "Resource/ResMob.h"
#include "Resource/ServerResManager.h"

#define WEEKDAY_OFFSET					345600			// 1970/1/1 sunday offset
#define WEEK_SECOND						604800
#define DAY_SECOND						86400
#define BUFF_UPDATETIME					5000
#define TIME_MARK_START					32767

REGISTER_CLASSTYPE(CQuestSystem, QuestSystem_CLASSID);
PROTO_LuaPush(CCharacter)
CQuestSystem::CQuestSystem()
{
	m_pOwner = NULL;
}

CQuestSystem::~CQuestSystem()
{
	m_pOwner = NULL;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
		DeleteNetObject(it->second);
	m_questMap.clear();

	for (TypeMap::iterator it = m_typeMap.begin(); it != m_typeMap.end(); ++it)
		DeleteNetObject(it->second);
	m_typeMap.clear();
}

LPCPEER CQuestSystem::GetOwnerPeer()
{
	if (m_pOwner)
		return m_pOwner->GetPeer();
	else
		return NULL;
}

RPCContext &CQuestSystem::GetOwnerContext()
{
	static RPCContext dummyContext;
	if (m_pOwner)
		return m_pOwner->GetContext();
	else
		return dummyContext;
}

///////////////////////////////////////
///			quest log method		///
///////////////////////////////////////
bool CQuestSystem::HasQuest(UInt16 quest_id)
{
	QuestMap::iterator itr = m_questMap.find(quest_id);
	return (itr != m_questMap.end() && itr->second && itr->second->IsExist());
}

CQuestLog* CQuestSystem::GetQuest(UInt16 quest_id)
{
	QuestMap::iterator it = m_questMap.find(quest_id);
	if (it != m_questMap.end() && it->second->IsExist())
		return it->second;
	else
		return NULL;
}

// if id exists, return NULL; else 1) create object, 2) update to DB; 3) return CQuestLog.
CQuestLog* CQuestSystem::AddQuest(UInt16 quest_id, const String &quest_name, UInt16 type_id, 
								  const String &type_name)
{
	QuestMap::iterator it = m_questMap.find(quest_id);
	CQuestLog* pQuest = NULL;
	if (it != m_questMap.end()) {
		pQuest = it->second;
		if (pQuest->IsExist())		// return NULL if already exists
			return NULL;
		else
			pQuest->ResetData();	// reuse deleted quest
	}

	CQuestType* pType = GetType(type_id);
	if (pType == NULL)
		pType = AddType(type_id);
	if (pType->GetName().IsEmpty())
		pType->SetName(type_name);

	if (pQuest == NULL) {
		pQuest = CQuestLog::NewInstance();
		pQuest->SetStatus(CQuestLog::ST_NEW);
	}
	pQuest->SetQuestSystem(this);
	pQuest->SetId(m_pOwner->GetChar_id(), quest_id);
	String desc = _T("");
	String qname = quest_name.GetLength() > QUEST_NAME_LEN ? 
					quest_name.substr(0, QUEST_NAME_LEN - 1) : quest_name;
	pQuest->SetInfo(type_id, qname, _T(""));
	pQuest->SetCancel(BOOLEAN_TRUE);		// default quest can cancel
	m_questMap[quest_id] = pQuest;

	LogService_Proxy::LogQuest(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), 
		m_pOwner->GetChar_id(), m_pOwner->GetMap()->GetLine_id(), 
		LogService_Proxy::Quest::SOURCE_NPC,
		pQuest->GetBeginNpc(), pQuest->GetQuest_id(), LogService_Proxy::Quest::ACT_ADD);

	RaiseUpdate();

	return pQuest;
}

CQuestLog* CQuestSystem::AddQuest(UInt16 quest_id, PCSTRINGA quest_name, UInt16 type_id, 
								  PCSTRINGA type_name)
{
	String qname, tname;

	UTF8ToStr(quest_name, qname);
	UTF8ToStr(type_name, tname);

	return AddQuest(quest_id, qname, type_id, tname);
}

// if id exists, return NULL; else 1) create object, 2) update to DB; 3) return CQuestLog.
CQuestLog* CQuestSystem::AddQuest(UInt16 quest_id)
{
	if (HasQuest(quest_id))							// return NULL if already exists
		return NULL;
	
	CONST CSVQuestRow* pInfo = gGlobal.m_resource->GetCSVRandomQuest(quest_id);
	if (pInfo == NULL)
		return NULL;

	CQuestLog* pQuest = AddQuest(quest_id, pInfo->Name, pInfo->TypeId, pInfo->TypeName);
	if (pQuest)
		pQuest->SetData(pInfo);

	return pQuest;
}

Boolean CQuestSystem::RemoveQuest(UInt16 quest_id)
{
	CQuestLog* pQuest = GetQuest(quest_id);
	if (pQuest)
	{
		pQuest->SetStatus(CQuestLog::ST_DELETED);
		if (m_pOwner && pQuest->HasEncountNPC())			// remove encounter in owner
			m_pOwner->RemoveEncounter(quest_id);
		if (m_pOwner && pQuest->HasFollower())				// remove follower in owner
			pQuest->RemoveFollower();
		RaiseUpdate();
		LogService_Proxy::LogQuest(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), 
			m_pOwner->GetChar_id(), m_pOwner->GetMap()->GetLine_id(), 
			LogService_Proxy::Quest::SOURCE_NPC, 
			pQuest->GetBeginNpc(), pQuest->GetQuest_id(), LogService_Proxy::Quest::ACT_REMOVE);
		return BOOLEAN_TRUE;
	}
	else
	{
		return BOOLEAN_FALSE;
	}
}

CQuestLog* CQuestSystem::GetEachQuest(int index)
{
	if (index >= 0 && index < (int) m_questMap.size())
	{
		QuestMap::iterator it = m_questMap.begin();
		while (index > 0)
		{
			++it;
			index--;
		}
		CQuestLog* pQuest = it->second;
		if (pQuest && pQuest->IsExist())
			return pQuest;
	}

	return NULL;
}

UInt16 CQuestSystem::GetQuestCount()
{
	return ((UInt16) m_questMap.size());
}

///////////////////////////////////////
///			quest type method		///
///////////////////////////////////////
bool CQuestSystem::HasType(UInt16 type_id)
{
	TypeMap::iterator it = m_typeMap.find(type_id);
	return (it != m_typeMap.end() && it->second->IsExist());
}

CQuestType* CQuestSystem::GetType(UInt16 type_id)
{
	TypeMap::iterator it = m_typeMap.find(type_id);
	if (it != m_typeMap.end() && it->second->IsExist())
		return it->second;
	else
		return NULL;
}

CQuestType* CQuestSystem::AddType(UInt16 type_id)
{
	TypeMap::iterator it = m_typeMap.find(type_id);
	CQuestType* pType = NULL;
	if (it != m_typeMap.end()) {
		pType = it->second;
		if (pType->IsExist())			// return NULL if already exists
			return NULL;
		else
			pType->ResetData();
	}

	if (pType == NULL) {
		pType = CQuestType::NewInstance();
		pType->SetStatus(CQuestType::ST_NEW);
	}

	pType->SetQuestSystem(this);
	pType->SetId(m_pOwner->GetChar_id(), type_id);
	pType->SetClearTime();
	m_typeMap[type_id] = pType;
	RaiseUpdate();

	return pType;
}

Boolean CQuestSystem::RemoveType(UInt16 type_id)
{
	CQuestType* pType = GetType(type_id);
	if (pType) {
		pType->SetStatus(CQuestType::ST_DELETED);
		RaiseUpdate();
		return BOOLEAN_TRUE;
	} else {
		return BOOLEAN_FALSE;
	}
}

void CQuestSystem::ClearTotalRound(UInt16 type_id)
{
	UInt32 now = gGlobal.GetServiceTime();
	CQuestType* pType;
	TypeMap::iterator it;
	TypeMap::iterator endIt;

	if (type_id == 0) {					// clear all types round
		it = m_typeMap.begin(); 
		endIt == m_typeMap.end();
	} else {							// only specified round
		it = m_typeMap.find(type_id);
		endIt = it;
		if (it != m_typeMap.end())
			++endIt;
	}
	for (; it != endIt; ++it)
	{
		pType = it->second;
		if (pType && pType->IsExist() && pType->GetLimitRound() > 0) {
			pType->SetTotalRound(0);
			pType->SetClearTime();
		}
	}
}

void CQuestSystem::CheckTotalRound()
{
	CQuestType* pType;
	bool updated = false;
	UInt32 now = gGlobal.GetServiceTime();

	for (TypeMap::iterator it = m_typeMap.begin(); it != m_typeMap.end(); ++it)
	{
		pType = it->second;
		if (pType == NULL || !pType->IsExist() || pType->GetLimitRound() == 0)
			continue;
		
		if (pType->GetLimitType() == CQuestType::TYPE_PER_DAY && 
			(now - pType->GetLastClearTime()) >= DAY_SECOND || 
			pType->GetLimitType() == CQuestType::TYPE_PER_WEEK && 
			(now - pType->GetLastClearTime()) >= WEEK_SECOND) 
		{
			pType->SetTotalRound(0);
			pType->SetClearTime();
			updated = true;
		}
	}

	if (updated)
		RaiseUpdate();
}

void CQuestSystem::CheckEncountNPC()
{
	CQuestLog *pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest)
			pQuest->CheckEncountNPC();
	}
}

void CQuestSystem::CheckFollower()
{
	CQuestLog *pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest)
			pQuest->CheckFollower();
	}
}

String CQuestSystem::GetTypeName(UInt16 quest_id)
{
	CQuestLog* pQuest = GetQuest(quest_id);
	if (pQuest)
	{
		CQuestType* pType = GetType(pQuest->GetType_id());
		if (pType)
			return pType->GetName();
	}

	return _T("");
}

void CQuestSystem::SetRoundInfo(UInt16 type_id, UInt16 maxRound, UInt16 limitRound, 
								UInt16 limitType)
{
	CQuestType* pType = GetType(type_id);
	if (pType == NULL)
		pType = AddType(type_id);

	if (pType && (pType->GetMaxRound() != maxRound || 
		pType->GetLimitRound() != limitRound || 
		pType->GetLimitType() != limitType))
	{
		pType->SetMaxRound(maxRound);
		pType->SetLimitRound(limitRound);
		pType->SetLimitType(limitType);
		pType->SetClearTime();
		RaiseUpdate();
	}
}

CQuestType* CQuestSystem::GetEachType(int index)
{
	if (index >= 0 && index < (int) m_typeMap.size())
	{
		TypeMap::iterator it = m_typeMap.begin();
		while (index > 0)
		{
			++it;
			index--;
		}
		CQuestType* pType = it->second;
		if (pType && pType->IsExist())
			return pType;
	}

	return NULL;
}

UInt16 CQuestSystem::GetTypeCount(UInt16 type_id)
{
	if (type_id == 0) {
		return ((UInt16) m_typeMap.size());
	} else {
		UInt16 count = 0;
		CQuestLog* pQuest;
		for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
		{
			pQuest = it->second;
			if (pQuest && pQuest->GetType_id() == type_id)
				count ++;
		}
		return count;
	}
}

///////////////////////////////////////
///			quest flag method		///
///////////////////////////////////////
bool CQuestSystem::HasFlag(UInt16 flag_id)
{
	return (m_flagMap.find(flag_id) != m_flagMap.end());
}

UInt32 CQuestSystem::GetFlag(UInt16 flag_id)
{
	FlagMap::iterator it = m_flagMap.find(flag_id);
	if (it != m_flagMap.end())
		return it->second;
	else
		return 0;
}

Boolean CQuestSystem::SetFlag(UInt16 flag_id, UInt32 value)
{
	FlagMap::iterator it = m_flagMap.find(flag_id);
	if (it != m_flagMap.end() && it->second == value)		 // same as existing value
		return BOOLEAN_FALSE;

	m_flagMap[flag_id] = value;
	m_flagChgSet.insert(flag_id);
	RaiseUpdate();
	CheckAllHints();		// check all quest hints after flag changed
	CheckJournals();		// check all quest journals after flag changed

	return BOOLEAN_TRUE;
}

Boolean CQuestSystem::RemoveFlag(UInt16 flag_id)
{
	FlagMap::iterator it = m_flagMap.find(flag_id);
	if (it == m_flagMap.end())
		return BOOLEAN_FALSE;

	m_flagMap.erase(flag_id);
	m_flagChgSet.insert(flag_id);
	RaiseUpdate();

	return BOOLEAN_TRUE;
}
bool CQuestSystem::HasNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id)
{
	if(m_pOwner == NULL || m_pOwner->GetMap() == NULL)
	{
		return false;
	}
	NpcFlagIndex flagIndex(npc_id,m_pOwner->GetMap()->GetLine_id(),owner_id,flag_id);
	return (gGlobal.g_AllNpcFlags.find(flagIndex) != gGlobal.g_AllNpcFlags.end());
}
UInt32 CQuestSystem::GetNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id)
{
	if(m_pOwner == NULL || m_pOwner->GetMap() == NULL)
	{
		return 0;
	}
	NpcFlagIndex flagIndex(npc_id,m_pOwner->GetMap()->GetLine_id(),owner_id,flag_id);
	NpcFlagMap::iterator iter = gGlobal.g_AllNpcFlags.find(flagIndex);
	if (iter != gGlobal.g_AllNpcFlags.end())
			return iter->second;
	return 0;
}
Boolean CQuestSystem::SetNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id, UInt16 flag)
{
	if(m_pOwner == NULL || m_pOwner->GetMap() == NULL)
	{
		return false;
	}
	NpcFlagIndex flagIndex(npc_id,m_pOwner->GetMap()->GetLine_id(),owner_id,flag_id);
	NpcFlagMap::iterator iter = gGlobal.g_AllNpcFlags.find(flagIndex);
	if (iter != gGlobal.g_AllNpcFlags.end() && iter->second == flag)
		return true;

	gGlobal.g_AllNpcFlags[flagIndex] = flag;
	m_npcFlagChgSet.insert(flagIndex);
	RaiseUpdate();
	CheckAllHints();		// check all quest hints after flag changed
	CheckJournals();		// check all quest journals after flag changed
	return 0;
}
Boolean CQuestSystem::RemoveNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id)
{
	if(m_pOwner == NULL || m_pOwner->GetMap() == NULL)
	{
		return false;
	}
	NpcFlagIndex flagIndex(npc_id,m_pOwner->GetMap()->GetLine_id(),owner_id,flag_id);
	NpcFlagMap::iterator iter = gGlobal.g_AllNpcFlags.find(flagIndex);
	if (iter != gGlobal.g_AllNpcFlags.end())
	{
		gGlobal.g_AllNpcFlags.erase(flagIndex);
		m_npcFlagChgSet.insert(flagIndex);
		RaiseUpdate();
		return true;
	}
	return false;
}
UInt32 CQuestSystem::GetTimeMark(UInt16 markId)
{
	return GetFlag(TIME_MARK_START + markId);
}

Boolean CQuestSystem::SetTimeMark(UInt16 markId)
{
	UInt32 now = gGlobal.GetServiceTime();
	return SetFlag(TIME_MARK_START + markId, now);
}

Boolean CQuestSystem::RemoveTimeMark(UInt16 markId)
{
	return RemoveFlag(TIME_MARK_START + markId);
}

Boolean CQuestSystem::TimeMarkOver(UInt16 markId, UInt16 diffMin)
{
	UInt32 markTime = GetFlag(TIME_MARK_START + markId);

	if (markTime > 0) {
		UInt32 now = gGlobal.GetServiceTime();
		return ((now - markTime) >= ((UInt32) diffMin * 60));
	} else {
		return BOOLEAN_TRUE;
	}
}

///////////////////////////////////////////
///			quest system method			///
///////////////////////////////////////////
void CQuestSystem::Update()
{
	RaiseUpdate();
}

void CQuestSystem::CheckOverTime()
{
	bool bUpdate = false;

	// check all quest and set "failed" if QUEST FAIL due to certain partner's dead
	CQuestLog* pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest == NULL || !pQuest->IsExist() || pQuest->HasState())
			continue;

		pQuest->CheckOverTime();					// check quest if over time
		if (!bUpdate && pQuest->IsUpdated())
			bUpdate = true;
	}
	if (bUpdate)									// quest system update if any update happened
		RaiseUpdate();
}

Boolean CQuestSystem::CheckJoinParty()
{
	// check all quest and set "failed" if QUEST FAIL due to join party
	CQuestLog* pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest == NULL || !pQuest->IsExist() || pQuest->IsFailed())
			continue;

		if (pQuest->IsFailJoinParty())
			return BOOLEAN_FALSE;
	}

	if(HasLuaFunc(gGlobal.g_pLuaState, "CheckJoinParty"))
	{
		CallLuaFunc(gGlobal.g_pLuaState, "CheckJoinParty", String(), m_pOwner);
		UInt32 result = (UInt32)gGlobal.g_TempResult;
		return (result == 0 ? BOOLEAN_FALSE : BOOLEAN_TRUE);
	}
	return BOOLEAN_TRUE;
}

void CQuestSystem::CheckAfterBattle(UInt8 result, UInt32 npcId, const UInt32Vector& mobIndices)
{
	if (result == 0 || npcId == 0)
		return;

	bool bUpdate = false;
	CQuestLog* pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest == NULL || !pQuest->IsExist() || pQuest->HasState())
			continue;

		//pQuest->CheckOverTime();					// check quest if over time
		pQuest->CheckAfterBattle(result, npcId, mobIndices);	// check quest after battle
		if (!bUpdate && pQuest->IsUpdated())
			bUpdate = true;
	}
	if (bUpdate)									// quest system update if any update happened
		RaiseUpdate();
}

void CQuestSystem::CheckPartnerDie(UInt32 npcId)
{
	if (npcId == 0)
		return;

	bool bUpdate = false;

	// check all quest and set "failed" if QUEST FAIL due to certain partner's dead
	CQuestLog* pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest == NULL || !pQuest->IsExist() || pQuest->IsFailed())
			continue;

		pQuest->CheckPartnerDie(npcId);				// check quest partner die
		if (!bUpdate && pQuest->IsUpdated())
			bUpdate = true;
	}
	if (bUpdate)									// quest system update if any update happened
		RaiseUpdate();
}

void CQuestSystem::CheckNpcDie(UInt32 npcId)
{
	if (npcId == 0)
		return;

	bool bUpdate = false;

	// check all quest and set "failed" if QUEST FAIL due to certain partner's dead
	CQuestLog* pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest == NULL || !pQuest->IsExist() || pQuest->IsFailed())
			continue;

		pQuest->CheckNpcDie(npcId);					// check quest target npc die
		if (!bUpdate && pQuest->IsUpdated())
			bUpdate = true;
	}
	if (bUpdate)									// quest system update if any update happened
		RaiseUpdate();
}

void CQuestSystem::CheckNpcTalk(UInt32 npcId, UInt16 map_id)
{
	if (npcId == 0)
		return;

	bool bUpdate = false;
	CQuestLog* pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest == NULL || !pQuest->IsExist() || pQuest->IsFailed())
			continue;

		pQuest->CheckNpcTalk(npcId, map_id);				// check quest target npc die
		if (!bUpdate && pQuest->IsUpdated())
			bUpdate = true;
	}
	if (bUpdate)									// quest system update if any update happened
		RaiseUpdate();
}

void CQuestSystem::CheckItem(UInt32 item_id, int count)
{
	if (item_id == 0)
		return;

	bool bUpdate = false;
	CQuestLog* pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest == NULL || !pQuest->IsExist() || pQuest->IsFailed())
			continue;

		pQuest->CheckItem(item_id, count);			// check quest target item
		if (!bUpdate && pQuest->IsUpdated())
			bUpdate = true;
	}
	if (bUpdate)									// quest system update if any update happened
		RaiseUpdate();
}

void CQuestSystem::CheckPartner(UInt32 mob_id, int count)
{
	if (mob_id == 0)
		return;

	bool bUpdate = false;
	CQuestLog* pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest == NULL || !pQuest->IsExist() || pQuest->IsFailed())
			continue;

		pQuest->CheckPartner(mob_id, count);			// check quest target partner
		if (!bUpdate && pQuest->IsUpdated())
			bUpdate = true;
	}
	if (bUpdate)									// quest system update if any update happened
		RaiseUpdate();
}

void CQuestSystem::CheckPet(UInt32 pet_id, int count)
{
	if (pet_id == 0)
		return;

	bool bUpdate = false;
	CQuestLog* pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest == NULL || !pQuest->IsExist() || pQuest->IsFailed())
			continue;

		pQuest->CheckPet(pet_id, count);			// check quest target partner
		if (!bUpdate && pQuest->IsUpdated())
			bUpdate = true;
	}
	if (bUpdate)									// quest system update if any update happened
		RaiseUpdate();
}

void CQuestSystem::CheckAllTarget()
{
	CQuestLog* pQuest;

	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest == NULL || !pQuest->IsExist() || pQuest->IsFailed())
			continue;

		pQuest->CheckAllTarget();					// check quest all targets
	}
}

Boolean CQuestSystem::CheckFinished(UInt16 quest_id, UInt32 npc_id, UInt16 map_id)
{
	CQuestLog* pQuest = GetQuest(quest_id);
	if (pQuest == NULL)
		return BOOLEAN_FALSE;
	Boolean completed = pQuest->CheckFinished(npc_id, map_id);
	if (pQuest->IsUpdated())						// quest system update if any update happened
		RaiseUpdate();

	return completed;
}

void CQuestSystem::CheckNpcExist()
{
	if (m_pOwner && m_pOwner->GetContext().sessionId.serviceId != INVALID_SERVICE_ID) {
		PROCEDURE_DO_METHOD(CQuestSystem, CheckNpcExist, this, m_pOwner->GetContext())
	}
}

Boolean CQuestSystem::MatchQuestHint(QuestHint &hint)
{
	if ((hint.sex == 0 || m_pOwner->GetSex() == hint.sex) &&
		(hint.job == 0 || m_pOwner->GetCclass() == hint.job) &&
		(hint.faction == 0 || m_pOwner->GetFaction() == hint.faction) &&
		(hint.minLv == 0 || m_pOwner->GetLevel() >= hint.minLv) &&
		(hint.maxLv == 0 || m_pOwner->GetLevel() <= hint.maxLv))
	{
		// check flags if required
		if (hint.pFlagMap) {
			bool match = true;
			for (FlagReqMap::iterator it2 = hint.pFlagMap->begin();
				it2 != hint.pFlagMap->end(); ++it2)
			{
				UInt32 v = GetFlag(it2->first);
				if (it2->second != v)
					return BOOLEAN_FALSE;
			}
		}
		return BOOLEAN_TRUE;
	} else {
		return BOOLEAN_FALSE;
	}
}

void CQuestSystem::AddHintNpc(CMapNPC* pNpc)
{
	if (m_pOwner == NULL || pNpc == NULL || !pNpc->HasQuestHint())
		return;

	QuestHintMap &hintMap = pNpc->GetHintMap();
	for (QuestHintMap::iterator it = hintMap.begin(); it != hintMap.end(); ++it)
	{
		QuestHint &hint = it->second;
		if (MatchQuestHint(hint))
			m_hintIds.insert(QuestHintID(pNpc->GetNetId(), hint.quest_id));
	}
}

void CQuestSystem::ClearQuestHint()
{
	if (m_hintIds.empty())
		return;

	RPCNetIDVector delNpcIds;
	UInt16Vector delQuestIds;
	// clear all hints if empty ids given
	CB_RemoveQuestHints(m_pOwner->GetPeer(), m_pOwner->GetContext(), delNpcIds, delQuestIds);

	m_hintIds.clear();
}

void CQuestSystem::CheckAllHints()
{
	if (m_pOwner == NULL || m_pOwner->GetMap() == NULL)
		return;

	RPCNetIDVector addNpcIds, delNpcIds;
	UInt16Vector addQuestIds, delQuestIds;

	m_pOwner->GetMap()->CheckAllHints(m_pOwner, addNpcIds, addQuestIds, delNpcIds, delQuestIds);
	if (!addNpcIds.empty())
		CB_AddQuestHints(m_pOwner->GetPeer(), m_pOwner->GetContext(), addNpcIds, addQuestIds);
	if (!delNpcIds.empty())
		CB_RemoveQuestHints(m_pOwner->GetPeer(), m_pOwner->GetContext(), delNpcIds, delQuestIds);

	if (!addNpcIds.empty() || !delNpcIds.empty())
		TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("CheckAllHints: char_id %d, add quest_ids %s, del quest_ids %s"), 
			m_pOwner->GetChar_id(), GetVectorString(addQuestIds).c_str(),
			GetVectorString(delQuestIds).c_str());
}

void CQuestSystem::CheckQuestHint(CMapNPC* pNpc, 
								  RPCNetIDVector &addNpcIds, UInt16Vector &addQuestIds,
								  RPCNetIDVector &delNpcIds, UInt16Vector &delQuestIds)
{
	if (m_pOwner == NULL)
		return;

	Boolean match;
	Boolean found;
	QuestHintIDSet::iterator itfind;
	RPCNetID netId = pNpc->GetNetId();
	QuestHintMap &hintMap = pNpc->GetHintMap();
	for (QuestHintMap::iterator it = hintMap.begin(); it != hintMap.end(); ++it)
	{
		QuestHint &hint = it->second;
		itfind = m_hintIds.find(QuestHintID(netId, hint.quest_id));
		match = MatchQuestHint(hint);
		found = (itfind != m_hintIds.end());
		if (match && !found) {
			addNpcIds.push_back(netId);
			addQuestIds.push_back(hint.quest_id);
			m_hintIds.insert(QuestHintID(netId, hint.quest_id));
		} else if (!match && found) {
			delNpcIds.push_back(netId);
			delQuestIds.push_back(hint.quest_id);
			m_hintIds.erase(itfind);
		}
	}
}

void CQuestSystem::SendAll()
{
	TRACE_ENSURE(m_pOwner);

	NetGroup questGroup;
	CQuestLog* pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest && pQuest->IsExist()) {
			pQuest->SetUpdateIcon(BOOLEAN_FALSE);
			pQuest->PreSend();
//			pQuest->ClearUpdateMask();
			questGroup.Add(pQuest);
		}
	}

	NetGroup typeGroup;
	CQuestType* pType;
	for (TypeMap::iterator it = m_typeMap.begin(); it != m_typeMap.end(); ++it)
	{
		pType = it->second;
		if (pType && pType->IsExist()) {
			pType->PreSend();
			typeGroup.Add(pType);
		}
	}

	if (typeGroup.GetCount() > 0 || questGroup.GetCount() > 0)
		CB_QuestLogLst(m_pOwner->GetPeer(), m_pOwner->GetContext(), &questGroup, &typeGroup);


	if (!m_flagMap.empty())
	{
		UInt16Vector questIDs;
		UInt32Vector questFlags;
		for (FlagMap::iterator iter = m_flagMap.begin(); iter != m_flagMap.end(); ++iter)
		{
			std::pair<UInt16, UInt32> pair = *iter;
			questIDs.push_back(pair.first);
			questFlags.push_back(pair.second);
		}

		CB_QuestLogFlagLoadLst(m_pOwner->GetPeer(), m_pOwner->GetContext(), questIDs, questFlags);
	}

	// reset and check journals and send to client
	m_journalGroup.Clear();

	// quest system checking
	CheckJournals();
	CheckAllTarget();
}

void CQuestSystem::SendQuestHint()
{
	if (m_pOwner == NULL)
		return;

	// send quest hints
	RPCNetIDVector npcIds;
	UInt16Vector questIds;
	for (QuestHintIDSet::iterator it = m_hintIds.begin(); it != m_hintIds.end(); ++it)
	{
		QuestHintID &hintId = *it;
		npcIds.push_back(hintId.npc_netId);
		questIds.push_back(hintId.quest_id);
	}
	if (!npcIds.empty()) {
		TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("CB_AddQuestHints: char_id %d, quest_ids %s"), 
			m_pOwner->GetChar_id(), GetVectorString(questIds).c_str());
		CB_AddQuestHints(m_pOwner->GetPeer(), m_pOwner->GetContext(), npcIds, questIds);
	}
}

void CQuestSystem::CheckJournals()
{
	if (m_pOwner == NULL)
		return;

	// send quest journals
	NetGroup grp;
	UInt16Vector delIds;
	CQuestJournal *pJournal;
	bool exist, shown;
	String s;
	for (UInt16 i = 0; i < gGlobal.m_journalGroup.GetCount(); i++)
	{
		pJournal = gGlobal.m_journalGroup.GetChild<CQuestJournal>(i);
		if (pJournal == NULL)
			continue;
		exist = m_journalGroup.IsChild(pJournal);
		shown = pJournal->IsShown(m_pOwner);
		if (!exist && shown)		// new journal
		{
			pJournal->SetSendMask(CQuestJournal::AllMask);
			pJournal->ClearUpdateMask();
			m_journalGroup.Add(pJournal);
			grp.Add(pJournal);
			s.AppendFormat(_T("A%d,"), pJournal->GetJournal_id());
			// send when already have 20 journals, prevent too big packet
			if (grp.GetCount() >= 20) {
				CB_AddJournals(m_pOwner->GetPeer(), m_pOwner->GetContext(), &grp);
				grp.Clear();
			}
		}
		else if (exist && !shown)	// old journal not shown
		{
			m_journalGroup.Remove(pJournal);
			delIds.push_back(pJournal->GetJournal_id());
			s.AppendFormat(_T("D%d,"), pJournal->GetJournal_id());
		}
		else if (exist && pJournal->IsUpdated(CQuestJournal::enableMask))	// enable/disable
		{
			pJournal->SetSendMask(pJournal->GetUpdateMask());
			pJournal->ClearUpdateMask();
			grp.Add(pJournal);
			s.AppendFormat(_T("E%d,"), pJournal->GetJournal_id());
		}
	}
	if (!s.IsEmpty())
		TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("CheckJournals: char_id %d, journal_ids %s"), 
			m_pOwner->GetChar_id(), s.c_str());
	if (grp.GetCount() > 0)
		CB_AddJournals(m_pOwner->GetPeer(), m_pOwner->GetContext(), &grp);
	if (!delIds.empty())
		CB_RemoveJournals(m_pOwner->GetPeer(), m_pOwner->GetContext(), delIds);
}

RPCResult CQuestSystem::CancelQuest(LPCPEER pPeer, RPCContext &context, const UInt16 &quest_id)
{
	TRACE_ENSURE(m_pOwner);
	CQuestLog* pQuest = GetQuest(quest_id);
	if (pQuest && pQuest->IsCancel())
	{
		// reset round when cancel quest
		CQuestType* pType = GetType(pQuest->GetType_id());
		if (pType && pType->GetMaxRound() > 0)
			pType->SetRound(0);
		pQuest->SetStatus(CQuestLog::ST_DELETED);
		if (m_pOwner && pQuest->HasEncountNPC())			// remove encounter in owner
			m_pOwner->RemoveEncounter(quest_id);
		if (m_pOwner && pQuest->HasFollower())				// remove follower in owner
			pQuest->RemoveFollower();
		RaiseUpdate();
		if (m_pOwner->GetMap())
		{
			LogService_Proxy::LogQuest(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), 
				m_pOwner->GetChar_id(), m_pOwner->GetMap()->GetLine_id(), 
				LogService_Proxy::Quest::SOURCE_NPC, 
				pQuest->GetBeginNpc(), pQuest->GetQuest_id(), LogService_Proxy::Quest::ACT_CANCEL);
		}
		else
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s GetMap is NULL"), context.sessionId.GetString().c_str());
		}
	}

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::LoadQuests(RPCContext &context)
{
	TRACE_ENSURE(m_pOwner);

	PROCEDURE_START_METHOD(CQuestSystem, LoadQuests, this, context)

		if (!m_questMap.empty() || !m_typeMap.empty() || !m_flagMap.empty()) {
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CQuestSystem LoadQuests Error: quests already loaded, sessionId %d, char_id %d"), 
				m_pOwner->GetSessionId().GetString().c_str(), m_pOwner->GetChar_id());
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		ObjectService_Proxy::GetQuestList(gGlobal.pObjectService->pPeer, context, 
			m_pOwner->GetChar_id());

	PROCEDURE_WAIT3(1, ObjectService, CB_GetQuestList, NetGroup*, logGroup, 
					NetGroup*, typeGroup, NetGroup*, flagGroup)

		TRACE_ENSURE(logGroup && typeGroup && flagGroup);
		CQuestLogData *pQuestData;
		CQuestTypeData *pTypeData;
		CQuestFlagData *pFlagData;
		CQuestLog *pQuest;
		CQuestType *pType;

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CQuestSystem LoadQuests: char_id %d, questLogs %d, questTypes %d, questFlags %d"), 
			m_pOwner->GetChar_id(), logGroup->GetCount(), typeGroup->GetCount(), 
			flagGroup->GetCount());

		for (UInt16 i = 0; i < logGroup->GetCount(); i++)
		{
			pQuestData = logGroup->GetChild<CQuestLogData>(i);
			if (pQuestData && pQuestData->GetChar_id() == m_pOwner->GetChar_id()) {
				pQuest = CQuestLog::NewInstance();
				pQuest->SetQuestSystem(this);
				pQuest->SetData(pQuestData);
				pQuest->SetStatus(CQuestLog::ST_NORMAL);
				pQuest->ClearUpdateMask();
				m_questMap[pQuestData->GetQuest_id()] = pQuest;
			}
		}

		for (UInt16 i = 0; i < typeGroup->GetCount(); i++)
		{
			pTypeData = typeGroup->GetChild<CQuestTypeData>(i);
			if (pTypeData && pTypeData->GetChar_id() == m_pOwner->GetChar_id()) {
				pType = CQuestType::NewInstance();
				pType->SetQuestSystem(this);
				pType->SetData(pTypeData);
				pType->SetStatus(CQuestType::ST_NORMAL);
				pType->ClearUpdateMask();
				m_typeMap[pTypeData->GetType_id()] = pType;
			}
		}

		for (UInt16 i = 0; i < flagGroup->GetCount(); i++)
		{
			pFlagData = flagGroup->GetChild<CQuestFlagData>(i);
			if (pFlagData && pFlagData->GetChar_id() == m_pOwner->GetChar_id())
				m_flagMap[pFlagData->GetFlag_id()] = pFlagData->GetFlag();
		}

		CheckTotalRound();
		CheckEncountNPC();
		CheckFollower();
		ScheduleNextDay();
		DeleteNetGroup(logGroup, BOOLEAN_TRUE);
		DeleteNetGroup(typeGroup, BOOLEAN_TRUE);
		DeleteNetGroup(flagGroup, BOOLEAN_TRUE);

		if(HasLuaFunc(gGlobal.g_pLuaState,"OnCharacterEnter"))
			CallLuaFunc(gGlobal.g_pLuaState, "OnCharacterEnter", String(), m_pOwner);

		PROCEDURE_DO_METHOD(CQuestSystem, CheckNpcExist, this, context)

	PROCEDURE_WAIT_RETURN(2)

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CQuestSystem LoadQuests Error: ObjectService:CB_Error, errcode %d, %s"),
			err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::UpdateQuests(RPCContext &context)
{
	TRACE_ENSURE(m_pOwner);

	PROCEDURE_START_METHOD(CQuestSystem, UpdateQuests, this, context)

		// create/update/delete quest types
		CQuestType* pType;
		CQuestTypeData* pQuestTypeData;
		NetGroup grp;
		for (TypeMap::iterator it = m_typeMap.begin(); it != m_typeMap.end(); ++it)
		{
			pType = it->second;
			if (pType && pType->IsUpdated())
			{
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CQuestSystem UpdateQuests: update quest type, type_id = %d"), pType->GetType_id());

				pQuestTypeData  = CreateNetObject<CQuestTypeData>();
				TRACE_ENSURE(pQuestTypeData);
				pType->WriteData(pQuestTypeData);
				if (pQuestTypeData->IsUpdated()) {
					pQuestTypeData->SetSendMask(pQuestTypeData->GetUpdateMask());
					grp.Add(pQuestTypeData);
				}
			}
		}

		if (grp.GetCount() == 0) 
		{		// nothing to update
			PROCEDURE_GO_STEP(2)
		}

		ObjectService_Proxy::UpdateQuestTypes(gGlobal.pObjectService->pPeer, 
						context, m_pOwner->GetChar_id(), &grp);
		DeleteNetChilds(&grp);			// delete data object after send

	PROCEDURE_WAIT(2, ObjectService, CB_UpdateQuestTypes)

		TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestSystem UpdateQuests: CB_UpdateQuestTypes"));		
			
		NetGroup newGroup;
		NetGroup updGroup;
		UInt16Vector del_tids;			// deleted type_id list
		for (TypeMap::iterator it = m_typeMap.begin(); it != m_typeMap.end();)
		{
			CQuestType* pType = it->second;
			if (pType) {
				if (pType->GetStatus() == CQuestType::ST_DELETED) {
					del_tids.push_back(it->first);
					DeleteNetObject(pType);
					it = m_typeMap.erase(it);
					continue;
				}
				if (pType->GetStatus() == CQuestType::ST_NEW) {
					pType->SetStatus(CQuestType::ST_NORMAL);
					pType->PreSend();
					pType->ClearUpdateMask();
					newGroup.Add(pType);
				} else if (pType->IsUpdated()) {
					pType->SetSendMask(pType->GetUpdateMask());
					pType->ClearUpdateMask();
					updGroup.Add(pType);
				}
			}
			++it;
		}

		// send new quest types to client
		if (newGroup.GetCount() > 0)
			CB_AddQuestTypes(m_pOwner->GetPeer(), m_pOwner->GetContext(), &newGroup);
		if (updGroup.GetCount() > 0)
			CB_UpdateQuests(m_pOwner->GetPeer(), m_pOwner->GetContext(), &updGroup);
		if (!del_tids.empty())
			CB_RemoveQuestTypes(m_pOwner->GetPeer(), m_pOwner->GetContext(), del_tids);

		// create/update/delete quest logs
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CQuestSystem UpdateQuests: char_id %d"), m_pOwner->GetChar_id());

		CQuestLog* pQuest;
		CQuestLogData* pQuestLogData;
		NetGroup grp;
		for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
		{
			pQuest = it->second;
			if (pQuest && pQuest->IsModified())
			{
				TRACE_INFOL_1(GLOBAL_LOGGER, _F("CQuestSystem UpdateQuests: update quest log, quest_id = %d"), pQuest->GetQuest_id());

				pQuestLogData  = CreateNetObject<CQuestLogData>();
				TRACE_ENSURE(pQuestLogData);
				pQuest->WriteData(pQuestLogData);
				if (pQuestLogData->IsUpdated())
				{
					pQuestLogData->SetSendMask(pQuestLogData->GetUpdateMask());
					grp.Add(pQuestLogData);
				}
			}
		}

		if (grp.GetCount() == 0) 
		{		// nothing to update
			PROCEDURE_GO_STEP(1)
		}

		ObjectService_Proxy::UpdateQuestLogs(gGlobal.pObjectService->pPeer, 
						context, m_pOwner->GetChar_id(), &grp);
		DeleteNetChilds(&grp);			// delete data object after send

	PROCEDURE_WAIT(1, ObjectService, CB_UpdateQuestLogs)

		TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestSystem UpdateQuests: CB_UpdateQuestLogs"));		
			
		NetGroup newGroup;
		NetGroup updGroup;
		UInt16Vector del_ids;			// deleted quest_id list
		for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end();)
		{
			CQuestLog* pQuest = it->second;
			if (pQuest) {
				if (pQuest->GetStatus() == CQuestLog::ST_DELETED) {		// delete quest log
					del_ids.push_back(it->first);
					DeleteNetObject(pQuest);
					it = m_questMap.erase(it);
					continue;
				}
				if (pQuest->GetStatus() == CQuestLog::ST_NEW) {			// new quest log
					pQuest->SetUpdateIcon(BOOLEAN_TRUE);
					pQuest->SetStatus(CQuestLog::ST_NORMAL);
					pQuest->CheckAllTarget();
					pQuest->PreSend();
					pQuest->ClearUpdateMask();
					newGroup.Add(pQuest);
				} else if (pQuest->IsUpdated()) {						// update quest log
					pQuest->SetSendMask(pQuest->GetUpdateMask());
					pQuest->ClearUpdateMask();
					updGroup.Add(pQuest);
				}
			}
			++it;
		}

		// send new quest logs to client
		if (newGroup.GetCount() > 0)
			CB_AddQuestLogs(m_pOwner->GetPeer(), m_pOwner->GetContext(), &newGroup);
		if (updGroup.GetCount() > 0)
			CB_UpdateQuests(m_pOwner->GetPeer(), m_pOwner->GetContext(), &updGroup);
		if (!del_ids.empty())
			CB_RemoveQuestLogs(m_pOwner->GetPeer(), m_pOwner->GetContext(), del_ids);

		// create/update/delete quest flags
		CQuestFlagData* pQuestFlagData = NULL;
		UInt16Vector del_fids;						// deleted flag_id list
		NetGroup grp;
		FlagMap::iterator it2;
		UInt16Vector questIDs;
		UInt32Vector questFlags;

		for (FlagSet::iterator it = m_flagChgSet.begin(); it != m_flagChgSet.end();
			++it)
		{
			UInt16 flag_id = *it;
			it2 = m_flagMap.find(flag_id);
			if (it2 == m_flagMap.end()) 
			{			// flag deleted
				del_fids.push_back(flag_id);		// don't will can't find it ,becase i didn't removequest to db
			} 
			else 
			{											// create/update flag
					pQuestFlagData = CreateNetObject<CQuestFlagData>();
					pQuestFlagData->SetId(m_pOwner->GetChar_id(), flag_id);
					pQuestFlagData->SetFlag(it2->second);
					pQuestFlagData->SetSendMask(pQuestFlagData->GetUpdateMask());
					grp.Add(pQuestFlagData);

					questIDs.push_back(flag_id);
					questFlags.push_back(it2->second);
			}
		}

		m_flagChgSet.clear();								// clear changed flag

		if (del_fids.empty() && grp.GetCount() == 0) 
		{		// nothing to update
			PROCEDURE_GO_STEP(3)
		}

		CB_QuestLogFlagChangeLst(m_pOwner->GetPeer(), m_pOwner->GetContext(), questIDs, questFlags, del_fids);
		
		ObjectService_Proxy::UpdateQuestFlags(gGlobal.pObjectService->pPeer, context, m_pOwner->GetChar_id(), &grp, del_fids);
		DeleteNetChilds(&grp);			// delete data object after send

	PROCEDURE_WAIT(3, ObjectService, CB_UpdateQuestFlags)

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
		NpcFlagMap::iterator it2 = gGlobal.g_AllNpcFlags.find(flagIndex);
		if (it2 == gGlobal.g_AllNpcFlags.end()) 
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
		PROCEDURE_GO_STEP(4)
	}

	ObjectService_Proxy::UpdateNpcFlags(gGlobal.pObjectService->pPeer, context, &grpUpdate, &grpDelete);
	DeleteNetChilds(&grpUpdate);			// delete data object after send
	DeleteNetChilds(&grpDelete);			// delete data object after send

	PROCEDURE_WAIT(4, ObjectService, CB_UpdateNpcFlags)

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CQuestSystem::UpdateQuests Error: ObjectService:CB_Error, errcode %d, %s"),
			err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CheckNpcExist(RPCContext &context)
{
	TRACE_ENSURE(m_pOwner);

	PROCEDURE_START_METHOD(CQuestSystem, CheckNpcExist, this, context)

		if (gGlobal.pWorldService == NULL || gGlobal.pWorldService->pPeer == NULL)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, 
				_T("CheckNpcExist Error: invalid world service peer, sessionId %s"),
				context.sessionId.GetString().c_str());
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		// check target npc and finish npc exist
		UInt8 vtype;
		UInt16 num, max;
		NpcLiveInfo npc;
		NpcLiveList npcList;

		// get npc to query
		CQuestLog* pQuest;
		for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
		{
			pQuest = it->second;
			if (pQuest == NULL || !pQuest->IsExist() || pQuest->HasState())
				continue;
			npc.npcId = pQuest->GetFinishNpc();
			npc.mapId = pQuest->GetFinishNpc_Map();
			npc.x = pQuest->GetFinishNpc_x();
			npc.y = pQuest->GetFinishNpc_y();
			npc.eventId = pQuest->GetQuest_id();		// use eventId as questId
			//npc.ownerId = gGlobal.GetMapOwnerId(npc.mapId, m_pOwner);	// get map owner_id
			npcList.push_back(npc);
			for (int i = 1; i <= pQuest->GetTargetTotal(); i++) {
				if (pQuest->GetTarget(i, npc.npcId, npc.mapId, npc.x, npc.y, 
					vtype, num, max))
				{
					if (vtype == Q_TARGET_NPC)
					{
						npcList.push_back(npc);
					}
				}
			}
		}

		WorldService_Proxy::QueryNpcExist(gGlobal.pWorldService->pPeer, context, npcList);

	PROCEDURE_WAIT1(2, WorldService, CB_NpcNotExist, NpcLiveList, deadNpcList)

		// set quest fail if that target npc or finish npc not exist
		bool bUpdate = false;
		CQuestLog* pQuest;
		for (NpcLiveList::const_iterator it = deadNpcList.begin(); 
			it != deadNpcList.end(); ++it)
		{
			const NpcLiveInfo &npc = *it;
			pQuest = GetQuest(npc.eventId);		// use eventId as questId
			if (pQuest && !pQuest->HasState()) {
				pQuest->SetFail(BOOLEAN_TRUE);
				bUpdate = true;
			}
		}
		if (bUpdate)
			RaiseUpdate();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CQuestSystem LoadQuests Error: ObjectService:CB_Error, errcode %d, %s"),
			err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CQuestSystem::ScheduleNextDay()
{
	// get next day time
	time_t now_t = (time_t)gGlobal.GetServiceTime();

	// schedule event for next day 00:00:01
	struct tm* pCurTime = localtime(&now_t);
	pCurTime->tm_hour = 23;
	pCurTime->tm_min = 59;
	pCurTime->tm_sec = 59;
	UInt32 nextTime = (UInt32) (mktime(pCurTime) - now_t) + 2;
	RaiseUniqEventID(EVT_DAY_OVER, nextTime * 1000);
}

void CQuestSystem::LeaderChanged()
{
	// fail npc encounter or follower quests if leader changed
	CQuestLog *pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (m_pOwner && pQuest && pQuest->IsExist() && !pQuest->HasState())
		{
			/*if (pQuest->HasEncountNPC() || pQuest->HasFollower())
				pQuest->SetFail(BOOLEAN_TRUE);*/

			if (pQuest->HasEncountNPC() && pQuest->IsQuestFailAfterLeaderChanged())
			{
				pQuest->SetFail(BOOLEAN_TRUE);
			}
			else if(pQuest->HasFollower())
			{
				pQuest->SetFail(BOOLEAN_TRUE);
			}
		}
	}
	if (m_pOwner)
		m_pOwner->ClearMobFollower();
}

void CQuestSystem::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_UPDATE_OBJECT:				// update all partner to object service
		if (m_pOwner)
		{
			PROCEDURE_DO_METHOD(CQuestSystem, UpdateQuests, this, m_pOwner->GetContext())
		}
		break;

	case EVT_DAY_OVER:
		if( m_pOwner )
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("DayOverEvent char_id %d"), m_pOwner->GetChar_id());
		CheckTotalRound();
		CheckJournals();
		ScheduleNextDay();
		break;
	}
}

Reuben::System::Boolean CQuestSystem::RemoveEncounter( UInt16 quest_id )
{
	CQuestLog* pQuest = GetQuest(quest_id);
	if (pQuest)
	{
		if (m_pOwner && pQuest->HasEncountNPC())			// remove encounter in owner
		{
			m_pOwner->RemoveEncounter(quest_id);
			return BOOLEAN_TRUE;
		}
	}
	return BOOLEAN_FALSE;
}

Reuben::System::Boolean CQuestSystem::RemoveFollower( UInt16 quest_id )
{
	CQuestLog* pQuest = GetQuest(quest_id);
	if (pQuest)
	{
		if (m_pOwner && pQuest->HasFollower())				// remove follower in owner
		{
			pQuest->RemoveFollower();
			return BOOLEAN_TRUE;
		}
	}
	return BOOLEAN_FALSE;
}

Reuben::System::Boolean CQuestSystem::RemoveEncounterByNpcID(UInt16 quest_id, UInt32 uiNpcID )
{
	CQuestLog* pQuest = GetQuest(quest_id);
	if (pQuest)
	{
		if (m_pOwner && pQuest->HasEncountNPC())			// remove encounter in owner
		{
			m_pOwner->RemoveEncounterByNpcID(quest_id, uiNpcID);
			return BOOLEAN_TRUE;
		}
	}
	return BOOLEAN_FALSE;
}