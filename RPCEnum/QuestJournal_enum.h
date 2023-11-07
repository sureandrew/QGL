#pragma once
#ifndef RPC_QUESTJOURNAL_ENUM
#define RPC_QUESTJOURNAL_ENUM

enum RPC_QuestJournal_Enum
{
	QuestJournal_CLASSID	= 31020,
	RPC_QuestJournal_ENUM_ST	= 31020,
	RPC_QuestJournal_UP_STATE	= 31026,
	RPC_QuestJournal_REQ_STATE	= 31029,
	RPC_QuestJournal_ENUM_ED	= 31029
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_QuestJournal()
{
	RPC_ENUM_MAP[RPC_QuestJournal_UP_STATE] = 1715610412;
	RPC_ENUM_MAP[RPC_QuestJournal_REQ_STATE] = 28716839;
	return 0;
}
static int _addrpc_QuestJournal = _AddRpcEnum_QuestJournal();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_QuestJournal()
{
	RPC_NONBLOCK_MAP[RPC_QuestJournal_UP_STATE] = true;
	return 0;
}
static int _nonblock_QuestJournal = _NonblockMap_QuestJournal();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_QUESTJOURNAL_ENUM
