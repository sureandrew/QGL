//-- Common
#include "Common.h"
//-- Self
#include "SimClient/QuestSystem.h"
//-- Library
#include "SimClient/QuestLog.h"
#include "SimClient/QuestType.h"
#include "SimClient/Map.h"
#include "SimClient/Global.h"
#include "SimClient/Character.h"
#include "SimClient/MapNPC.h"
#include "SimClient/QuestJournal.h"
#include "Common/StateCode.h"
#include "Common/CallLuaFunc.h"

PROTO_LuaPush(CQuestSystem)
REGISTER_CLASSTYPE(CQuestSystem, QuestSystem_CLASSID);

CQuestSystem::CQuestSystem()
{
}

CQuestSystem::~CQuestSystem()
{
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
		DeleteNetObject(it->second);
	m_questMap.clear();

	for (TypeMap::iterator it = m_typeMap.begin(); it != m_typeMap.end(); ++it)
		DeleteNetObject(it->second);
	m_typeMap.clear();

	m_questTaskMap.clear();
	m_questTypeMap.clear();
	DeleteNetChilds(&m_journalGroup);
}

RPCResult CQuestSystem::CB_QuestLogLst(LPCPEER pPeer, RPCContext &context, NetGroup* questLogGrp,
									   NetGroup* questTypeGrp)
{
	TRACE_ENSURE(questLogGrp && questTypeGrp);

	TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, _F("CQuestSystem CB_QuestLogLst: questLogGrp %d, count %d, questTypeGrp %d, cuont %d"),
		questLogGrp->GetID(), questLogGrp->GetCount(), questTypeGrp->GetID(), questTypeGrp->GetCount());

	for (UInt16 i = 0; i < questLogGrp->GetCount(); i++)
	{
		CQuestLog* pQuest = questLogGrp->GetChild<CQuestLog>(i);
		if (pQuest)
			m_questMap[pQuest->GetQuest_id()] = pQuest;
	}

	for (UInt16 i = 0; i < questTypeGrp->GetCount(); i++)
	{
		CQuestType* pType = questTypeGrp->GetChild<CQuestType>(i);
		if (pType)
			m_typeMap[pType->GetType_id()] = pType;
	}

	DeleteNetGroup(questLogGrp);			// only delete the net group, keep childs
	DeleteNetGroup(questTypeGrp);

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_UpdateQuests(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	DeleteNetGroup(grp);

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_AddQuestLogs(LPCPEER pPeer, RPCContext &context, NetGroup* questLogGrp)
{
	for (UInt16 i = 0; i < questLogGrp->GetCount(); i++)
	{
		CQuestLog* pQuest = questLogGrp->GetChild<CQuestLog>(i);
		if (pQuest)
			m_questMap[pQuest->GetQuest_id()] = pQuest;
	}
	DeleteNetGroup(questLogGrp);

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_RemoveQuestLogs(LPCPEER pPeer, RPCContext &context, 
										   const UInt16Vector &quest_ids)
{
	if (IsOutstandingQuestTask() && gGlobal.m_pSimClient->m_bRobot)
	{
		int questID = FindQuestIDByLocation();
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Quest %d is successfully completed"), questID);	
		RemoveQuestTask(questID);

		CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheckSubQuestEnd", String(), questID);
	}	

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_RemoveQuestLogs: quest_ids %s"), 
		GetVectorString(quest_ids).c_str());
	
	for (UInt16Vector::const_iterator it = quest_ids.begin(); it != quest_ids.end(); ++it)
	{
		CQuestLog *pQuest = m_questMap[*it];
		DeleteNetObject(pQuest);
		m_questMap.erase(*it);
	}

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_AddQuestTypes(LPCPEER pPeer, RPCContext &context, 
										 NetGroup* questTypeGrp)
{
	for (UInt16 i = 0; i < questTypeGrp->GetCount(); i++)
	{
		CQuestType* pType = questTypeGrp->GetChild<CQuestType>(i);
		if (pType)
			m_typeMap[pType->GetType_id()] = pType;
	}
	DeleteNetGroup(questTypeGrp);

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_RemoveQuestTypes(LPCPEER pPeer, RPCContext &context, 
											const UInt16Vector &qtype_ids)
{
	for (UInt16Vector::const_iterator it = qtype_ids.begin(); it != qtype_ids.end(); ++it)
		m_typeMap.erase(*it);

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_AddQuestHints(LPCPEER pPeer, RPCContext &context, 
										 const RPCNetIDVector &npcIds, 
										 const UInt16Vector &questIds)
{
	UInt16Vector::const_iterator it2 = questIds.begin();
	for (RPCNetIDVector::const_iterator it = npcIds.begin();
		it != npcIds.end() && it2 != questIds.end(); ++it, ++it2)
	{
		m_hintMap.insert(std::make_pair(*it, *it2));
	}
	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_RemoveQuestHints(LPCPEER pPeer, RPCContext &context, 
											const RPCNetIDVector &npcIds, 
											const UInt16Vector &questIds)
{
	if (npcIds.empty()) {			// clear all
		m_hintMap.clear();
	} else {						// remove specify ids
		HintMap::iterator itfind;
		UInt16Vector::const_iterator it2 = questIds.begin();
		for (RPCNetIDVector::const_iterator it = npcIds.begin();
			it != npcIds.end() && it2 != questIds.end(); ++it, ++it2)
		{
			for (itfind = m_hintMap.find(*it); itfind != m_hintMap.end(); ++itfind)
			{
				if (itfind->second == *it2) {		// match quest id
					m_hintMap.erase(itfind);
					break;
				}
			}
		}
	}

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_AddJournals(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	TRACE_ENSURE(grp);
	CQuestJournal* pJournal;
	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		pJournal = grp->GetChild<CQuestJournal>(i);
		if (pJournal)
			m_journalGroup.Add(pJournal);
	}
	DeleteNetGroup(grp);

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_RemoveJournals(LPCPEER pPeer, RPCContext &context, 
										  const UInt16Vector &journal_ids)
{
	CQuestJournal* pJournal;
	for (UInt16Vector::const_iterator it = journal_ids.begin(); 
		it != journal_ids.end(); ++it)
	{
		for (UInt16 i = 0; i < m_journalGroup.GetCount(); i++)
		{
			pJournal = m_journalGroup.GetChild<CQuestJournal>(i);
			if (pJournal && pJournal->GetJournal_id() == *it) {
				m_journalGroup.Remove(pJournal);
				break;
			}
		}
	}

	return RPC_RESULT_OK;
}

bool CQuestSystem::HasQuest(UInt16 quest_id)
{
	return (m_questMap.find(quest_id) != m_questMap.end());
}

CQuestLog* CQuestSystem::GetQuest(UInt16 quest_id)
{
	QuestMap::iterator it = m_questMap.find(quest_id);
	if (it != m_questMap.end())
		return it->second;
	else
		return NULL;
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
		if (pQuest)
			return pQuest;
	}

	return NULL;
}

UInt16 CQuestSystem::GetQuestCount()
{
	return ((UInt16) m_questMap.size());
}

bool CQuestSystem::HasType(UInt16 type_id)
{
	return (m_typeMap.find(type_id) != m_typeMap.end());
}

CQuestType* CQuestSystem::GetType(UInt16 type_id)
{
	TypeMap::iterator it = m_typeMap.find(type_id);
	if (it != m_typeMap.end())
		return it->second;
	else
		return NULL;
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
		if (pType)
			return pType;
	}

	return NULL;
}

UInt16 CQuestSystem::GetTypeCount()
{
	return ((UInt16) m_typeMap.size());
}

void CQuestSystem::ShowNpcHint(CMapNPC* pNpc)
{
	if (pNpc == NULL)
		return;

	String s;
	RPCNetID &npcNetId = pNpc->GetNetId();

	// search for all completed quests belong to that npc
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		CQuestLog* pQuest = it->second;
		if (pQuest && pQuest->IsCompleted(pNpc->GetNpc_id(),
			pNpc->GetMap_id(), pNpc->GetCurPosX(), pNpc->GetCurPosY()))
			s.AppendFormat(_T("%d?, "), it->first);
	}

	// search for all quests belong to that npc
	for (HintMap::iterator it = m_hintMap.find(npcNetId);
		it != m_hintMap.end() && it->first == npcNetId; ++it)
	{
		// show only if haven't got that quest
		UInt16 quest_id = it->second;
		if (!HasQuest(quest_id))
			s.AppendFormat(_T("%d!, "), quest_id);
	}
	if (!s.IsEmpty())
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("   : Hint %s"), s.c_str());
}

void CQuestSystem::ShowQuestLog()
{
	if (!m_questMap.empty()) {
		for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
		{
			CQuestLog* pQuest = it->second;
			if (pQuest)
				pQuest->ShowDetail();
		}
	}
	else
		TRACE_INFODTL(GLOBAL_LOGGER, _F("Quest log is empty"));	
}

UInt32 CQuestSystem::GetRandomNPCuid()
{
    UInt32 totalquest = (UInt32) m_questMap.size();
	if (totalquest > 0)
	{
		--totalquest;
		const UInt16 questRand = gGlobal.m_rand.GetRandom(totalquest);
		QuestMap::iterator it = m_questMap.begin();
		for (int i = 0; i < questRand; ++i)
			++it;
		return it->first;
	}
	else
		return 0;
}

bool CQuestSystem::CheckQuestValid(UInt16 questID)
{
	bool valid = FALSE;

	if (m_questMap.size() > 0)
	{
		UInt8 questNum = 0;	
		for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
		{
			CQuestLog* pQuest = it->second;
			if (pQuest)
			{
				if (questID == it->first)
				{
					valid = TRUE;
					break;
				}
			}
			++questNum;	
		}
	}

	return valid;
    
}

UInt32 CQuestSystem::GetQuestJumpMap(UInt32 questID)
{
	CQuestLog *temp = GetQuest(questID);
	if (temp)
	{
		return temp->GetFinishNpc_Map();
	}
	else
		return 0;
    
}

UInt32 CQuestSystem::GetQuestJumpXCor(UInt32 questID)
{
	CQuestLog *temp = GetQuest(questID);
	if (temp)
	{
		return temp->GetFinishNpc_x();
	}
	else
		return 0;
    
}

UInt32 CQuestSystem::GetQuestJumpYCor(UInt32 questID)
{
	CQuestLog *temp = GetQuest(questID);
	if (temp)
	{
		return temp->GetFinishNpc_y();
	}
	else
		return 0;
    
}

bool CQuestSystem::IsMobQuestCompleted(UInt32 questID)
{
	CQuestLog *temp = GetQuest(questID);
	if (temp)
	{
		if (temp->GetTarget1_num() >= temp->GetTarget1_max())
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
    
}

UInt32 CQuestSystem::GetRandomQuestID()
{
	if (m_questMap.size() > 0)
	{
		UInt16 questCount = (UInt16) m_questMap.size();
		UInt16 index = gGlobal.m_rand.GetRandom(questCount);

		QuestMap::iterator it = m_questMap.begin();
		int count = 0;
		while (count < questCount)
		{
			if (index == count)
				return it->first;
			++it;
			++count;
		}
		return 0;

	}
	else
		return 0;
}

UInt32 CQuestSystem::GetFinishNPCID(UInt32 questID)
{
	CQuestLog *temp = GetQuest(questID);
	if (temp)
	{
		int fid = temp->GetFinishNpc();
		return gGlobal.m_pSimClient->m_pMap->FindNpcUID(fid);

	}
	else
		return 0;
}

bool CQuestSystem::AddQuestTask(UInt32 questID, UInt16 type)
{
	bool main = FALSE;
	bool success = FALSE;
	for (QTaskTypeMap::iterator it = m_questTypeMap.begin(); it != m_questTypeMap.end(); ++it)
	{
		if (it->second == 5) // main quest
		{
			main = TRUE;
			break;
		}
	}
	
	if ((!main)||(type == 6))// if there is no main quests in the quest tasks list or the type is subquest (6 value)
	{
		QTaskMap::iterator iter = m_questTaskMap.find(questID);
		if (iter == m_questTaskMap.end())
		{
			m_questTaskMap[questID] = 0;
			m_questTypeMap[questID] = type;
			success = TRUE;
		}
	}
	return success;
    	    
}

bool CQuestSystem::CheckQuestTaskInProcess(UInt32 questID)
{
	QTaskMap::iterator it = m_questTaskMap.find(questID);
	if (it != m_questTaskMap.end())
		return TRUE;
	else
		return FALSE;
}

bool CQuestSystem::IsOutstandingQuestTask()
{
	if (m_questTaskMap.size() > 0)
		return TRUE;
	else
		return FALSE;
}

void CQuestSystem::CheckAllQuestRemainingTasks()
{
	QTaskMap::iterator it = m_questTaskMap.begin();
	if (m_questTaskMap.size() > 0)
	{
		while (it != m_questTaskMap.end())
		{
			CheckEachQuestRemainingTasks(it->first);
			++it;

		}
	}
}

void CQuestSystem::RemoveQuestTask(UInt32 questID)
{
	QTaskMap::iterator it = m_questTaskMap.find(questID);
	if (it != m_questTaskMap.end())
	{
		m_questTaskMap.erase(it);

	}
	QTaskTypeMap::iterator iter = m_questTypeMap.find(questID);
	if (iter != m_questTypeMap.end())
	{
		m_questTypeMap.erase(iter);
	}
}

bool CQuestSystem::SpecialTypeTaskUnfinish(UInt32 type, UInt32 taskNum, UInt32 questID) // a function make up for some quest tasks with target_num 
{                                                                                       // cannot be automatically increased to target_max after cheat code is placed.  

	if (type == 2)
	{
		if (m_questTaskMap[questID] >= taskNum)
			return FALSE;
		else
			return TRUE;
	}
	else
		return TRUE;
}

void CQuestSystem::CheckEachQuestRemainingTasks(UInt32 questID)
{

	if ((gGlobal.m_pSimClient == NULL)||(gGlobal.m_pSimClient->m_pMap == NULL))
		return;
	CQuestLog *pQuest = GetQuest(questID);
	QTaskMap::iterator it = m_questTaskMap.find(questID);
	// return if all tasks are done
	if (pQuest == NULL || it == m_questTaskMap.end() || 
		m_questTaskMap[questID] == ALL_TASK_CHEATED)
		return;

	// check all tasks
	for (UInt8 i = 1; i <= 5; i++)
	{
		if (pQuest->GetTarget_id(i) == 0) {		// all tasks are finished
			CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheatJumpQuest", String(), questID,
				         gGlobal.m_pSimClient->m_pMap->GetLine_id());
			m_questTaskMap[questID] = ALL_TASK_CHEATED;
			break;
		}
		// if task i is not finished
		else if (pQuest->GetTarget_num(i) < pQuest->GetTarget_max(i) && 
			SpecialTypeTaskUnfinish(pQuest->GetTarget_vtype(i), i, questID))
		{
			if (m_questTaskMap[questID] < i) {	// if the task is not cheated
				DispatchQuestCheatCode(pQuest->GetTarget_id(i), 
					pQuest->GetTarget_max(i), pQuest->GetTarget_vtype(i), 
					pQuest->GetTarget_v1(i));
				m_questTaskMap[questID] = i;
			}
			break;
		}
	}

/*		
		if (temp &&((temp->GetTarget1_num() < temp->GetTarget1_max())&&SpecialTypeTaskUnfinish(temp->GetTarget1_vtype(), 1, questID))) // check if the first tasks are finished or not
		{// if task 1 is not finished
            if (m_questTaskMap[questID] < 1) // check if the task 1 is already cheated
			{// if the task1 is not cheated yet
				if (temp->GetTarget1_vtype() == 1)// to check if the first task is to get someone to talk to
				{
					if (temp->GetTarget2_id() == 0)// to check if there is no more tasks except just task to target1(task 1)
					{

						if (temp->GetTarget1_id() == temp->GetFinishNpc())
                            CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "TalkCondition", 
								questID, gGlobal.m_pSimClient->m_pMap->GetLine_id());
						else
						{   // below is the case of talking to NPC A, then jump back to finish NPC
							CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheatNPCFindQuest",
							temp->GetTarget1_id(), temp->GetTarget1_v1());
							m_questTaskMap[questID] = ALL_TASK_CHEATED;
							CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheatJumpQuest", questID,
					           gGlobal.m_pSimClient->m_pMap->GetLine_id());	

						}
						
					}
					else
					{
						CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheatNPCFindQuest",
						temp->GetTarget1_id(), temp->GetTarget1_v1());
						m_questTaskMap[questID] = 1;	
					}

				}
				else // if 1st task is doing something else
				{
					DispatchQuestCheatCode(temp->GetTarget1_id(), temp->GetTarget1_max(), temp->GetTarget1_vtype(), temp->GetTarget1_v1());
					m_questTaskMap[questID] = 1; 
				}

			}
                           
		}
		else if (temp && (((temp->GetTarget2_num() < temp->GetTarget2_max())&&SpecialTypeTaskUnfinish(temp->GetTarget2_vtype(), 2, questID))||(temp->GetTarget2_id() == 0))) // if task 1 is finished, then check 2nd task if it is finished
		{// if task 2 is not finished
			if (temp->GetTarget2_id() == 0)
			{// it means all tasks are finished since task 2 is empty
				CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheatJumpQuest", questID,
					         gGlobal.m_pSimClient->m_pMap->GetLine_id());
				m_questTaskMap[questID] = ALL_TASK_CHEATED;
			}
			else
			{
				if (m_questTaskMap[questID] < 2)// check if the 2nd task is cheated
				{// if the task is not cheated
					DispatchQuestCheatCode(temp->GetTarget2_id(), temp->GetTarget2_max(), temp->GetTarget2_vtype(), temp->GetTarget2_v1());	
					m_questTaskMap[questID] = 2;
				}

			}

		}					
		else if (temp && (((temp->GetTarget3_num() < temp->GetTarget3_max())&&SpecialTypeTaskUnfinish(temp->GetTarget3_vtype(), 3, questID))||(temp->GetTarget3_id() == 0))) // if task 2 is finished, then check 3rd task if it is finished
		{// if task 3 is not finished
			if (temp->GetTarget3_id() == 0)
			{// it means all tasks are finished since task 3 is empty
				CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheatJumpQuest", questID,
					         gGlobal.m_pSimClient->m_pMap->GetLine_id());
				m_questTaskMap[questID] = ALL_TASK_CHEATED;
			}
			else
			{
				if (m_questTaskMap[questID] < 3)// check if the 3rd task is cheated
				{// if the task is not cheated
					DispatchQuestCheatCode(temp->GetTarget3_id(), temp->GetTarget3_max(), temp->GetTarget3_vtype(), temp->GetTarget3_v1());	
					m_questTaskMap[questID] = 3;
				}

			}

		}				
		else if (temp && (((temp->GetTarget4_num() < temp->GetTarget4_max())&&SpecialTypeTaskUnfinish(temp->GetTarget4_vtype(), 4, questID))||(temp->GetTarget4_id() == 0))) // if task 3 is finished, then check 4th task if it is finished
		{// if task 4 is not finished
			if (temp->GetTarget4_id() == 0)
			{// it means all tasks are finished since task 4 is empty
				CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheatJumpQuest", questID,
					         gGlobal.m_pSimClient->m_pMap->GetLine_id());
				m_questTaskMap[questID] = ALL_TASK_CHEATED;
			}
			else
			{
				if (m_questTaskMap[questID] < 4)// check if the 4th task is cheated
				{// if the task is not cheated
					DispatchQuestCheatCode(temp->GetTarget4_id(), temp->GetTarget4_max(), temp->GetTarget4_vtype(), temp->GetTarget4_v1());	
					m_questTaskMap[questID] = 4;
				}

			}

		}		         
		else if (temp && (((temp->GetTarget5_num() < temp->GetTarget5_max())&&SpecialTypeTaskUnfinish(temp->GetTarget5_vtype(), 5, questID))||(temp->GetTarget5_id() == 0))) // if task 4 is finished, then check 5th task if it is finished
		{// if task 5 is not finished
			if (temp->GetTarget5_id() == 0)
			{// it means all tasks are finished since task 5 is empty
				CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheatJumpQuest", questID,
					         gGlobal.m_pSimClient->m_pMap->GetLine_id());
				m_questTaskMap[questID] = ALL_TASK_CHEATED;
			}
			else
			{
				if (m_questTaskMap[questID] < 5)// check if the 5th task is cheated
				{// if the task is not cheated
					DispatchQuestCheatCode(temp->GetTarget5_id(), temp->GetTarget5_max(), temp->GetTarget5_vtype(), temp->GetTarget5_v1());	
					m_questTaskMap[questID] = ALL_TASK_CHEATED;
				}

			}

		}
	*/
}

void CQuestSystem::SetQuestTaskCheated(UInt32 questID)
{
	QTaskMap::iterator it = m_questTaskMap.find(questID);
	if (it != m_questTaskMap.end())
		m_questTaskMap[questID] = ALL_TASK_CHEATED;

}

UInt32 CQuestSystem::FindQuestIDByLocation()
{
	if ((gGlobal.m_pSimClient == NULL)||(gGlobal.m_pSimClient->m_pCharacter == NULL))
		return 0;
	QuestMap::iterator it = m_questMap.begin();
	if (m_questMap.size() > 0)
	{
		while (it != m_questMap.end())
		{
			CQuestLog *temp = GetQuest(it->first);
			if (temp->GetFinishNpc_Map() == gGlobal.m_pSimClient->m_pCharacter->GetMap_id())
				if (temp->GetFinishNpc_x() == gGlobal.m_pSimClient->m_pCharacter->GetCurPosX())
					if (temp->GetFinishNpc_y() == gGlobal.m_pSimClient->m_pCharacter->GetCurPosY())
						return it->first; 	
            ++it;

		}
	}
	return 0;

}

void CQuestSystem::DispatchQuestCheatCode(UInt32 targetID, UInt32 max, UInt32 type, UInt32 mapID)
{
	switch (type)
	{
		case Q_TARGET_NPC:
			CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheatNPCFindQuest", String(),
						targetID, mapID);
			break;
		case Q_TARGET_ITEM:
			CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheatItemAddQuest", String(),
						targetID, max);
			break;
		case Q_TARGET_MOB:
			CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheatMobQuest", String(),
						targetID, max);
			break;
		case Q_TARGET_PARTNER:
			{
				int lineID = gGlobal.m_pSimClient->m_pMap->GetLine_id();
                CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheatPartnerAddQuest", String(),
						targetID, lineID);
			}
			break;
	}
}

void CQuestSystem::ShowJournals()
{
	CQuestJournal* pJournal;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Show Journals: total %d"), m_journalGroup.GetCount());
	// show enable journals first
	for (UInt16 i = 0; i < m_journalGroup.GetCount(); i++)
	{
		pJournal = m_journalGroup.GetChild<CQuestJournal>(i);
		if (pJournal && pJournal->GetEnable())
			pJournal->ShowDetail();
	}
	// show disable journals next
	for (UInt16 i = 0; i < m_journalGroup.GetCount(); i++)
	{
		pJournal = m_journalGroup.GetChild<CQuestJournal>(i);
		if (pJournal && !pJournal->GetEnable())
			pJournal->ShowDetail();
	}
}

RPCResult CQuestSystem::CB_QuestLogFlagChangeLst(LPCPEER pPeer, RPCContext &context, const UInt16Vector &questIDs, const UInt32Vector &questFlags, const UInt16Vector &Flagdel_ids)
{
	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_QuestLogFlagLoadLst(LPCPEER pPeer, RPCContext &context, const UInt16Vector &questIDs, const UInt32Vector &questFlags)
{
	return RPC_RESULT_OK;
}
