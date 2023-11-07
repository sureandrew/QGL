#pragma once
#ifndef _QUESTSYSTEM_H_
#define _QUESTSYSTEM_H_

#include "Proxy/Logic/QuestSystem_Proxy.h"

class CQuestLog;
class CQuestType;
class CQuestFlagData;
class CQuestJournal;
class CMapNPCControl;

class CQuestSystem : public QuestSystem_Proxy
{
public:
	DEFINE_CLASS(QuestSystem_CLASSID);

	virtual RPCResult CB_QuestLogFlagChangeLst(LPCPEER pPeer, RPCContext &context, const UInt16Vector &questIDs, const UInt32Vector &questFlags, const UInt16Vector &Flagdel_ids);
	virtual RPCResult CB_QuestLogFlagLoadLst(LPCPEER pPeer, RPCContext &context, const UInt16Vector &questIDs, const UInt32Vector &questFlags);

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

	void ShowNpcHint(CMapNPCControl* pNpcCtrl);
	void RefreshNpcHint();
	Boolean IsRelated(UInt16 qtype, UInt32 id);
	Boolean IsHaveFlag(UInt16 nQuestID);
private:
	typedef QuestSystem_Proxy Parent;
	typedef StlMap<UInt16, CQuestLog*, 
		std::less<UInt16>, ManagedAllocator<std::pair<UInt16, CQuestLog*> > > QuestMap;
	typedef StlMap<UInt16, CQuestType*, 
		std::less<UInt16>, ManagedAllocator<std::pair<UInt16, CQuestType*> > > TypeMap;
	typedef StlMultiMap<RPCNetID, UInt16, 
		std::less<RPCNetID>, ManagedAllocator<std::pair<RPCNetID, UInt16> > > HintMap;
	typedef StlMap<UInt16, UInt32, 
		std::less<UInt16>, ManagedAllocator<std::pair<UInt16, UInt32> > > FlagMap;

	FlagMap		m_flagMap;				// quest log flags
	QuestMap	m_questMap;				// quest log access map
	TypeMap		m_typeMap;				// quest type access map
	HintMap		m_hintMap;				// quest hint multi map
	NetGroup	m_journalGroup;			// quest journal group
};

#endif //_QUESTSYSTEM_H_
