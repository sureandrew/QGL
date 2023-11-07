#pragma once
#ifndef RPC_QUESTSYSTEM_ENUM
#define RPC_QUESTSYSTEM_ENUM

enum RPC_QuestSystem_Enum
{
	QuestSystem_CLASSID	= 36000,
	RPC_QuestSystem_ENUM_ST	= 36000,
	RPC_QuestSystem_UP_STATE	= 36335,
	RPC_QuestSystem_REQ_STATE	= 36456,
	RPC_QuestSystem_CancelQuest	= 36450,
	RPC_QuestSystem_CB_QuestLogLst	= 36813,
	RPC_QuestSystem_CB_UpdateQuests	= 36867,
	RPC_QuestSystem_CB_AddQuestLogs	= 36431,
	RPC_QuestSystem_CB_RemoveQuestLogs	= 36717,
	RPC_QuestSystem_CB_AddQuestTypes	= 36815,
	RPC_QuestSystem_CB_RemoveQuestTypes	= 36457,
	RPC_QuestSystem_CB_AddQuestHints	= 36209,
	RPC_QuestSystem_CB_RemoveQuestHints	= 36658,
	RPC_QuestSystem_CB_AddJournals	= 36122,
	RPC_QuestSystem_CB_RemoveJournals	= 36216,
	RPC_QuestSystem_CB_QuestLogFlagChangeLst	= 36725,
	RPC_QuestSystem_CB_QuestLogFlagLoadLst	= 36440,
	RPC_QuestSystem_ENUM_ED	= 36999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_QuestSystem()
{
	RPC_ENUM_MAP[RPC_QuestSystem_UP_STATE] = 342585362;
	RPC_ENUM_MAP[RPC_QuestSystem_REQ_STATE] = 1576818990;
	RPC_ENUM_MAP[RPC_QuestSystem_CancelQuest] = 39481760;
	RPC_ENUM_MAP[RPC_QuestSystem_CB_QuestLogLst] = 1979213267;
	RPC_ENUM_MAP[RPC_QuestSystem_CB_UpdateQuests] = 1727944504;
	RPC_ENUM_MAP[RPC_QuestSystem_CB_AddQuestLogs] = 1682118163;
	RPC_ENUM_MAP[RPC_QuestSystem_CB_RemoveQuestLogs] = 485173062;
	RPC_ENUM_MAP[RPC_QuestSystem_CB_AddQuestTypes] = 425151109;
	RPC_ENUM_MAP[RPC_QuestSystem_CB_RemoveQuestTypes] = 1429745850;
	RPC_ENUM_MAP[RPC_QuestSystem_CB_AddQuestHints] = 729164973;
	RPC_ENUM_MAP[RPC_QuestSystem_CB_RemoveQuestHints] = 722804170;
	RPC_ENUM_MAP[RPC_QuestSystem_CB_AddJournals] = 1919627079;
	RPC_ENUM_MAP[RPC_QuestSystem_CB_RemoveJournals] = 86638933;
	RPC_ENUM_MAP[RPC_QuestSystem_CB_QuestLogFlagChangeLst] = 2065201949;
	RPC_ENUM_MAP[RPC_QuestSystem_CB_QuestLogFlagLoadLst] = 1884699685;
	return 0;
}
static int _addrpc_QuestSystem = _AddRpcEnum_QuestSystem();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_QuestSystem()
{
	RPC_NONBLOCK_MAP[RPC_QuestSystem_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_QuestSystem_CB_QuestLogLst] = true;
	RPC_NONBLOCK_MAP[RPC_QuestSystem_CB_UpdateQuests] = true;
	RPC_NONBLOCK_MAP[RPC_QuestSystem_CB_AddQuestLogs] = true;
	RPC_NONBLOCK_MAP[RPC_QuestSystem_CB_RemoveQuestLogs] = true;
	RPC_NONBLOCK_MAP[RPC_QuestSystem_CB_AddQuestTypes] = true;
	RPC_NONBLOCK_MAP[RPC_QuestSystem_CB_RemoveQuestTypes] = true;
	RPC_NONBLOCK_MAP[RPC_QuestSystem_CB_AddQuestHints] = true;
	RPC_NONBLOCK_MAP[RPC_QuestSystem_CB_RemoveQuestHints] = true;
	RPC_NONBLOCK_MAP[RPC_QuestSystem_CB_AddJournals] = true;
	RPC_NONBLOCK_MAP[RPC_QuestSystem_CB_RemoveJournals] = true;
	RPC_NONBLOCK_MAP[RPC_QuestSystem_CB_QuestLogFlagChangeLst] = true;
	RPC_NONBLOCK_MAP[RPC_QuestSystem_CB_QuestLogFlagLoadLst] = true;
	return 0;
}
static int _nonblock_QuestSystem = _NonblockMap_QuestSystem();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_QUESTSYSTEM_ENUM
