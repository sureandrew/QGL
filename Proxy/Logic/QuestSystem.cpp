//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/QuestSystem.h"
//-- Library
#include "Proxy/Logic/QuestLog.h"
#include "Proxy/Logic/QuestType.h"
#include "Proxy/Logic/QuestJournal.h"
#include "Proxy/Logic/MapNPC.h"
#include "Proxy/Data/QuestFlagData.h"
#include "QGL/Global.h"
#include "QGL/Configure.h"
#include "QGL/MapNPCControl.h"
#include "QGL/MenuQuestLogProcess.h"
#include "QGL/MapObjectView.h"
#include "QGL/QGLApplication.h"
#include "QGL/MenuHelpProcess.h"
#include "QGL/MenuQuestJournal.h"
#include "QGL/CharacterControl.h"

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

	DeleteNetChilds(&m_journalGroup);
}

RPCResult CQuestSystem::CB_QuestLogFlagChangeLst(LPCPEER pPeer, RPCContext &context, const UInt16Vector &questIDs, const UInt32Vector &questFlags, const UInt16Vector &Flagdel_ids)
{
	UInt16 nSize = MIN(questIDs.size(), questFlags.size());
	for (UInt16 i = 0; i < nSize; ++i)
	{
		m_flagMap[questIDs[i]] = questFlags[i]; // create /change flag
	}

	for (UInt16Vector::const_iterator it = Flagdel_ids.begin(); it != Flagdel_ids.end(); ++it)
	{
		m_flagMap.erase(*it);
	}

	if (gGlobal.g_pQuestLogProcess)
	gGlobal.g_pQuestLogProcess->TestReConstructAllData();

	if (gGlobal.g_pQuestLogProcess)
	gGlobal.g_pQuestLogProcess->OnUpdateNoteUI();

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_QuestLogFlagLoadLst(LPCPEER pPeer, RPCContext &context, const UInt16Vector &questIDs, const UInt32Vector &questFlags)
{
	m_flagMap.clear();
	UInt16 nSize = MIN(questIDs.size(), questFlags.size());

	for (UInt16 i = 0; i < nSize; ++i)
	{
		m_flagMap[questIDs[i]] = questFlags[i];
	}

	return RPC_RESULT_OK;
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

	if (gGlobal.g_pQuestLogProcess)
	{
		gGlobal.g_pQuestLogProcess->TestReConstructAllData();
	}

	RefreshNpcHint();

	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_UpdateQuests(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (gGlobal.g_pQuestLogProcess)
	{
//		gGlobal.g_pQuestLogProcess->TestReConstructAllData(BOOLEAN_TRUE);
		gGlobal.g_pQuestLogProcess->OnUpdateNoteUI();
	}
	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_AddQuestLogs(LPCPEER pPeer, RPCContext &context, NetGroup* questLogGrp)
{
	CQuestLog* pQuest = NULL;
	for (UInt16 i = 0; i < questLogGrp->GetCount(); i++)
	{
		pQuest = questLogGrp->GetChild<CQuestLog>(i);
		if (pQuest)
		{
			m_questMap[pQuest->GetQuest_id()] = pQuest;
//			gGlobal.g_pQuestLogProcess->AddNoteInfo(pQuest->GetType_id(), pQuest->GetQuest_id());
		}
	}
	if (questLogGrp->GetCount() > 0)
	gGlobal.g_pMenuHelpProcess->HandleEvent(HELP_TIPS_FIRST_GETGUEST);
	DeleteNetGroup(questLogGrp);

	
	if (gGlobal.g_pQuestLogProcess) {
		gGlobal.g_pQuestLogProcess->TestReConstructAllData(BOOLEAN_TRUE);
		gGlobal.g_pQuestLogProcess->CallMissionButtonEffect();
		if (pQuest)
			gGlobal.g_pQuestLogProcess->SetCurQuest(pQuest->GetQuest_id(), 
				pQuest->GetType_id());
		gGlobal.g_pQuestLogProcess->OnUpdateNoteUI();
	}
	RefreshNpcHint();


	return RPC_RESULT_OK;
}

RPCResult CQuestSystem::CB_RemoveQuestLogs(LPCPEER pPeer, RPCContext &context, 
										   const UInt16Vector &quest_ids)
{
	for (UInt16Vector::const_iterator it = quest_ids.begin(); it != quest_ids.end(); ++it)
	{
		gGlobal.g_pQuestLogProcess->RemoveQuestHandle(*it);

		CQuestLog *pQuest = m_questMap[*it];// prevent memory leakage. If it causes error, delete this code.
//		gGlobal.g_pQuestLogProcess->DelNoteInfo(pQuest->GetType_id(), pQuest->GetQuest_id());
		DeleteNetObject(pQuest);  // prevent memory leakage. If it causes error, delete this code. 
		m_questMap.erase(*it);  
	}

	if (gGlobal.g_pQuestLogProcess)
	{
		gGlobal.g_pQuestLogProcess->TestReConstructAllData(BOOLEAN_TRUE);
		gGlobal.g_pQuestLogProcess->OnUpdateNoteUI();
	}
	RefreshNpcHint();

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
	RefreshNpcHint();

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
	RefreshNpcHint();

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
		{
			m_journalGroup.Add(pJournal);
			gGlobal.g_pMenuQuestJournal->AddJournalToList(pJournal);
		}
	}
	DeleteNetGroup(grp);

	if (gGlobal.g_pMenuQuestJournal->IsOpenJournal())
		gGlobal.g_pMenuQuestJournal->ShowQuestJournal();

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
				gGlobal.g_pMenuQuestJournal->RemoveJournalFromList(pJournal);
				m_journalGroup.Remove(pJournal);
				break;
			}
		}
	}

	if (gGlobal.g_pMenuQuestJournal->IsOpenJournal())
		gGlobal.g_pMenuQuestJournal->ShowQuestJournal();

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

void CQuestSystem::ShowNpcHint(CMapNPCControl* pNpcCtrl)
{
	if (pNpcCtrl == NULL || pNpcCtrl->GetNetObject() == NULL)
		return;

	CMapNPC* pNpc = pNpcCtrl->GetNetObject();
	RPCNetID &npcNetId = pNpc->GetNetId();
	
	// search for all completed quests belong to that npc
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		CQuestLog* pQuest = it->second;
		if (pQuest && pQuest->IsFinishNpc(pNpc->GetNpc_id(), pNpc->GetMap_id(), 
			pNpc->GetCurPosX(), pNpc->GetCurPosY()))
		{
			if (pQuest->IsCompleted(pNpc->GetNpc_id(), pNpc->GetMap_id(), 
				pNpc->GetCurPosX(), pNpc->GetCurPosY()))
				pNpcCtrl->SetHeadIcon(MISSION_ICON_QUESTION_GOLD);
			else
				pNpcCtrl->SetHeadIcon(MISSION_ICON_QUESTION_SLIVER);

			return;
		}
	}

	// search for all quests belong to that npc
	for (HintMap::iterator it = m_hintMap.find(npcNetId);
		it != m_hintMap.end() && it->first == npcNetId; ++it)
	{
		// show only if haven't got that quest
		UInt16 quest_id = it->second;
		if (!HasQuest(quest_id)) {
				pNpcCtrl->SetHeadIcon(MISSION_ICON_EXCLAMATION_GOLD);
			return;
		}
	}

	pNpcCtrl->RemoveIcon();
}

void CQuestSystem::RefreshNpcHint()
{
	const StlMap<UInt, CMapNPCControl *>* pNpcCtrlList = 
		QGLApplication::GetInstance().GetRenderMap()->GetNpcCtrlList();
 
	if (pNpcCtrlList == NULL)
		return;

	for (StlMap<UInt, CMapNPCControl *>::const_iterator it = pNpcCtrlList->begin();
		it != pNpcCtrlList->end(); ++it)
	{
		ShowNpcHint(it->second);
	}

	if (gGlobal.g_pMenuQuestJournal->IsOpenJournal())
		gGlobal.g_pMenuQuestJournal->ShowQuestJournal();
}

Boolean CQuestSystem::IsRelated(UInt16 qtype, UInt32 id)
{
	CQuestLog* pQuest;
	for (QuestMap::iterator it = m_questMap.begin(); it != m_questMap.end(); ++it)
	{
		pQuest = it->second;
		if (pQuest && pQuest->IsRelated(qtype, id))
			return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE; 
}

Reuben::System::Boolean CQuestSystem::IsHaveFlag( UInt16 nQuestID )
{
	return (m_flagMap.find(nQuestID) != m_flagMap.end());
}