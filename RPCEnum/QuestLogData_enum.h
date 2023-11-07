#pragma once
#ifndef RPC_QUESTLOGDATA_ENUM
#define RPC_QUESTLOGDATA_ENUM

enum RPC_QuestLogData_Enum
{
	QuestLogData_CLASSID	= 10070,
	RPC_QuestLogData_ENUM_ST	= 10070,
	RPC_QuestLogData_UP_STATE	= 10073,
	RPC_QuestLogData_REQ_STATE	= 10079,
	RPC_QuestLogData_ENUM_ED	= 10079
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_QuestLogData()
{
	RPC_ENUM_MAP[RPC_QuestLogData_UP_STATE] = 1894523640;
	RPC_ENUM_MAP[RPC_QuestLogData_REQ_STATE] = 418150674;
	return 0;
}
static int _addrpc_QuestLogData = _AddRpcEnum_QuestLogData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_QuestLogData()
{
	RPC_NONBLOCK_MAP[RPC_QuestLogData_UP_STATE] = true;
	return 0;
}
static int _nonblock_QuestLogData = _NonblockMap_QuestLogData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_QUESTLOGDATA_ENUM
