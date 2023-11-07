#pragma once
#ifndef RPC_QUESTLOG_ENUM
#define RPC_QUESTLOG_ENUM

enum RPC_QuestLog_Enum
{
	QuestLog_CLASSID	= 31000,
	RPC_QuestLog_ENUM_ST	= 31000,
	RPC_QuestLog_UP_STATE	= 31003,
	RPC_QuestLog_REQ_STATE	= 31009,
	RPC_QuestLog_ENUM_ED	= 31009
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_QuestLog()
{
	RPC_ENUM_MAP[RPC_QuestLog_UP_STATE] = 786441412;
	RPC_ENUM_MAP[RPC_QuestLog_REQ_STATE] = 350555199;
	return 0;
}
static int _addrpc_QuestLog = _AddRpcEnum_QuestLog();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_QuestLog()
{
	RPC_NONBLOCK_MAP[RPC_QuestLog_UP_STATE] = true;
	return 0;
}
static int _nonblock_QuestLog = _NonblockMap_QuestLog();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_QUESTLOG_ENUM
