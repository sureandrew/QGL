#pragma once
#ifndef _QUESTSYSTEM_H_
#define _QUESTSYSTEM_H_

#include "Proxy/Logic/QuestSystem_Proxy.h"

class CQuestLog;
class CQuestType;
class CMapNPC;

const int ALL_TASK_CHEATED = 10;

class CQuestSystem : public QuestSystem_Proxy
{
public:
	DEFINE_CLASS(QuestSystem_CLASSID);
	virtual RPCResult CB_QuestLogLst(LPCPEER pPeer, RPCContext &context, NetGroup* questLogGrp, NetGroup* questTypeGrp);
	virtual RPCResult CB_UpdateQuests(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
	virtual RPCResult CB_AddQuestLogs(LPCPEER pPeer, RPCContext &context, NetGroup* questLogGrp);
	virtual RPCResult CB_RemoveQuestLogs(LPCPEER pPeer, RPCContext &context, const UInt16Vector &quest_ids);
	virtual RPCResult CB_AddQuestTypes(LPCPEER pPeer, RPCContext &context, NetGroup* questTypeGrp);
	virtual RPCResult CB_RemoveQuestTypes(LPCPEER pPeer, RPCContext &context, const UInt16Vector &gtype_ids);
	virtual RPCResult CB_AddQuestHints(LPCPEER pPeer, RPCContext &context, const RPCNetIDVector &npcIds, const UInt16Vector &questIds);
	virtual RPCResult CB_RemoveQuestHints(LPCPEER pPeer, RPCContext &context, const RPCNetIDVector &npcIds, const UInt16Vector &questIds);
	virtual RPCResult CB_AddJournals(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
	virtual RPCResult CB_RemoveJournals(LPCPEER pPeer, RPCContext &context, const UInt16Vector &journal_ids);
	virtual RPCResult CB_QuestLogFlagChangeLst(LPCPEER pPeer, RPCContext &context, const UInt16Vector &questIDs, const UInt32Vector &questFlags, const UInt16Vector &Flagdel_ids);
	virtual RPCResult CB_QuestLogFlagLoadLst(LPCPEER pPeer, RPCContext &context, const UInt16Vector &questIDs, const UInt32Vector &questFlags);

// OnUpdate can be implemented selectively

	CQuestSystem();
	virtual ~CQuestSystem();
	bool HasQuest(UInt16 quest_id);
	CQuestLog* GetQuest(UInt16 quest_id);
	CQuestLog* GetEachQuest(int index);
	UInt16 GetQuestCount();

	bool HasType(UInt16 type_id);
	CQuestType* GetType(UInt16 type_id);
	CQuestType* GetEachType(int index);
	UInt16 GetTypeCount();
	void ShowNpcHint(CMapNPC* pNpc);
	void ShowQuestLog();
	bool CheckQuestValid(UInt16 questID);
	UInt32 GetRandomNPCuid();	
	UInt32 GetQuestJumpMap(UInt32 questID);
	UInt32 GetQuestJumpXCor(UInt32 questID);
	UInt32 GetQuestJumpYCor(UInt32 questID);
	bool IsMobQuestCompleted(UInt32 questID);	

	UInt32 GetRandomQuestID();
	UInt32 GetFinishNPCID(UInt32 questID);
	bool AddQuestTask(UInt32 questID, UInt16 type);
	bool CheckQuestTaskInProcess(UInt32 questID);
	bool IsOutstandingQuestTask();
	void CheckEachQuestRemainingTasks(UInt32 questID);
	void SetQuestTaskCheated(UInt32 questID);
	void DispatchQuestCheatCode(UInt32 targetID, UInt32 max, UInt32 type, UInt32 mapID);
	void CheckAllQuestRemainingTasks();	
	UInt32 FindQuestIDByLocation();
	void RemoveQuestTask(UInt32 questID);
	bool SpecialTypeTaskUnfinish(UInt32 type, UInt32 taskNum, UInt32 questID);
	void ShowJournals();

private:
	typedef QuestSystem_Proxy Parent;
	typedef StlMap<UInt16, CQuestLog*, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, CQuestLog*> > > QuestMap;
	typedef StlMap<UInt16, CQuestType*, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, CQuestType*> > > TypeMap;
	typedef StlMultiMap<RPCNetID, UInt16, std::less<RPCNetID>, 
		ManagedAllocator<std::pair<RPCNetID, UInt16> > > HintMap;
	// the first element is quest ID, while the secons element is the task number of the task is already cheated
	typedef StlMap<UInt32, UInt16, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt32, UInt16> > > QTaskMap;
	// the 1st element is quest ID, while the 2nd element is the type (main quest or subquest
	typedef StlMap<UInt32, UInt16, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt32, UInt16> > > QTaskTypeMap;

	typedef StlMap<UInt16, UInt32, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, UInt32> > > FlagMap;
	typedef StlSet<UInt16, std::less<UInt16>, ManagedAllocator<UInt16> > FlagSet;

	QuestMap	m_questMap;				// quest log access map
	TypeMap		m_typeMap;				// quest type access map
	HintMap		m_hintMap;				// quest hint multi map
	QTaskMap    m_questTaskMap;         // quest tasks access map 
	QTaskTypeMap  m_questTypeMap;       // main/sub quests
	NetGroup	m_journalGroup;			// quest journal group
};

#endif //_QUESTSYSTEM_H_
