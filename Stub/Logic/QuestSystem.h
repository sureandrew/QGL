#pragma once
#ifndef _QUESTSYSTEM_H_
#define _QUESTSYSTEM_H_

#include "Stub/Logic/QuestSystem_Stub.h"
#include "Common/common_binary.h"

class CCharacter;
class CMapNPC;
class CQuestLog;
class CQuestType;
struct QuestHint;

class CQuestSystem : public QuestSystem_Stub,
					public Reuben::Simulation::IEventHandler<CQuestSystem>
{
public:
	DEFINE_CLASS(QuestSystem_CLASSID);
	virtual RPCResult CancelQuest(LPCPEER pPeer, RPCContext &context, const UInt16 &quest_id);

	CQuestSystem();
	virtual ~CQuestSystem();
	INLINE static CQuestSystem* NewInstance();
	INLINE void SetOwner(CCharacter* pChar) { m_pOwner = pChar; };
	CCharacter* GetOwner() { return m_pOwner; }
	LPCPEER GetOwnerPeer();
	RPCContext& GetOwnerContext();

	// quest log method
	bool HasQuest(UInt16 quest_id);
	CQuestLog* GetQuest(UInt16 quest_id);
	CQuestLog* AddQuest(UInt16 quest_id, const String &quest_name, UInt16 type_id, 
						const String &type_name);
	CQuestLog* AddQuest(UInt16 quest_id, PCSTRINGA quest_name, UInt16 type_id, 
						PCSTRINGA type_name);
	CQuestLog* AddQuest(UInt16 quest_id);
	Boolean RemoveQuest(UInt16 quest_id);
	Boolean RemoveEncounterByNpcID(UInt16 quest_id, UInt32 uiNpcID);
	Boolean RemoveEncounter(UInt16 quest_id);
	Boolean RemoveFollower(UInt16 quest_id);

	CQuestLog* GetEachQuest(int index);
	UInt16 GetQuestCount();

	// quest type method
	bool HasType(UInt16 type_id);
	CQuestType* GetType(UInt16 type_id);
	CQuestType* AddType(UInt16 type_id);
	Boolean RemoveType(UInt16 type_id);
	void ClearTotalRound(UInt16 type_id=0);
	void CheckTotalRound();
	void CheckEncountNPC();
	void CheckFollower();
	String GetTypeName(UInt16 quest_id);
	UInt32 GetRoundAttr(UInt16 type_id, Int attrType);
	void SetRoundInfo(UInt16 type_id, UInt16 maxRound, UInt16 limitRound, UInt16 limitType);
	CQuestType* GetEachType(int index);
	UInt16 GetTypeCount(UInt16 type_id=0);

	// quest flag method
	bool HasFlag(UInt16 flag_id);
	UInt32 GetFlag(UInt16 flag_id);
	Boolean SetFlag(UInt16 flag_id, UInt32 flag);
	Boolean RemoveFlag(UInt16 flag_id);
	UInt32 GetTimeMark(UInt16 markId);
	Boolean SetTimeMark(UInt16 markId);
	Boolean RemoveTimeMark(UInt16 markId);
	Boolean TimeMarkOver(UInt16 markId, UInt16 diffMin);

	bool HasNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id);
	UInt32 GetNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id);
	Boolean SetNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id, UInt16 flag);
	Boolean RemoveNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id);

	// quest check method
	void CheckOverTime();				// quest battle lose when over time
	Boolean CheckJoinParty();			// check can join party for all quests
	void CheckAfterBattle(UInt8 result, UInt32 npcId, const UInt32Vector& mobIndices);
	void CheckPartnerDie(UInt32 npcId);
	void CheckNpcDie(UInt32 npcId);
	void CheckNpcTalk(UInt32 npc_id, UInt16 map_id);
	void CheckItem(UInt32 item_id, int count);
	void CheckPartner(UInt32 mob_id, Int count);
	void CheckPet(UInt32 mob_id, Int count);
	void CheckAllTarget();
	Boolean CheckFinished(UInt16 quest_id, UInt32 npc_id=0, UInt16 map_id=0);
	void CheckNpcExist();

	// quest system method
	void AddHintNpc(CMapNPC* pNpc);
	void ClearQuestHint();
	void CheckAllHints();
	void CheckQuestHint(CMapNPC* pNpc, RPCNetIDVector &addNpcIds, UInt16Vector &addQuestIds,
						RPCNetIDVector &delNpcIds, UInt16Vector &delQuestIds);
	void CheckJournals();			// check and send quest journals to client
	void Update();					// raise quest system update
	void SendAll();					// send all quest logs and quest types to client
	void SendQuestHint();			// send quest hints to client
	Boolean IsUpdated();
	void ScheduleNextDay();
	void LeaderChanged();			// quest process when party leader changed
	RPCResult LoadQuests(RPCContext &context);
	RPCResult UpdateQuests(RPCContext &context);
	RPCResult CheckNpcExist(RPCContext &context);

	virtual void HandleEvent(CONST Event &event);
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event

	enum EventEnum
	{
		EVT_UPDATE_OBJECT		= MAKE_EVENTID(QuestSystem_CLASSID,  1),
		EVT_DAY_OVER			= MAKE_EVENTID(QuestSystem_CLASSID,  2),
	};

private:
	Boolean MatchQuestHint(QuestHint &hint);

	typedef QuestSystem_Stub Parent;
	typedef StlMap<UInt16, CQuestLog*, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, CQuestLog*> > > QuestMap;
	typedef StlMap<UInt16, CQuestType*, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, CQuestType*> > > TypeMap;
	typedef StlMap<UInt16, UInt32, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, UInt32> > > FlagMap;
	typedef StlSet<UInt16, std::less<UInt16>, ManagedAllocator<UInt16> > FlagSet;
	typedef StlSet<NpcFlagIndex, std::less<NpcFlagIndex>, ManagedAllocator<NpcFlagIndex> > NpcFlagSet;

	struct QuestHintID
	{
		RPCNetID npc_netId;
		UInt16 quest_id;

		QuestHintID() : quest_id(0) {}
		QuestHintID(RPCNetID &netId, UInt16 questId) : npc_netId(netId) , quest_id(questId) {}
		INLINE bool operator < (const QuestHintID& b) const
		{
			return (npc_netId < b.npc_netId || 
					(npc_netId == b.npc_netId && quest_id < b.quest_id));
		}
	};
	typedef StlSet<QuestHintID, std::less<QuestHintID>,
			ManagedAllocator<QuestHintID> > QuestHintIDSet;

	CCharacter*			m_pOwner;				// quest system owner
	QuestMap			m_questMap;				// quest log access map
	TypeMap				m_typeMap;				// quest type access map
	FlagMap				m_flagMap;				// quest flag access map
	FlagSet				m_flagChgSet;			// quest changed flag id list
	QuestHintIDSet		m_hintIds;				// quest hint ID set
	NetGroup			m_journalGroup;			// quest journal shown group
	NpcFlagSet          m_npcFlagChgSet;
};

INLINE CQuestSystem* CQuestSystem::NewInstance()
{
	return CreateNetObject<CQuestSystem>();
}

INLINE void CQuestSystem::RaiseUpdate()
{
	RaiseUniqEventID(EVT_UPDATE_OBJECT);
}

INLINE void CQuestSystem::CancelUpdate()
{
	CancelUniqEvent(EVT_UPDATE_OBJECT);
}

#endif //_QUESTSYSTEM_H_
