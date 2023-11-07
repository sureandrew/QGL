#pragma once
#ifndef RPC_QUESTTYPEDATA_ENUM
#define RPC_QUESTTYPEDATA_ENUM

enum RPC_QuestTypeData_Enum
{
	QuestTypeData_CLASSID	= 10090,
	RPC_QuestTypeData_ENUM_ST	= 10090,
	RPC_QuestTypeData_UP_STATE	= 10097,
	RPC_QuestTypeData_REQ_STATE	= 10099,
	RPC_QuestTypeData_ENUM_ED	= 10099
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_QuestTypeData()
{
	RPC_ENUM_MAP[RPC_QuestTypeData_UP_STATE] = 676208992;
	RPC_ENUM_MAP[RPC_QuestTypeData_REQ_STATE] = 96279546;
	return 0;
}
static int _addrpc_QuestTypeData = _AddRpcEnum_QuestTypeData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_QuestTypeData()
{
	RPC_NONBLOCK_MAP[RPC_QuestTypeData_UP_STATE] = true;
	return 0;
}
static int _nonblock_QuestTypeData = _NonblockMap_QuestTypeData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_QUESTTYPEDATA_ENUM
