#pragma once
#ifndef RPC_QUESTFLAGDATA_ENUM
#define RPC_QUESTFLAGDATA_ENUM

enum RPC_QuestFlagData_Enum
{
	QuestFlagData_CLASSID	= 10080,
	RPC_QuestFlagData_ENUM_ST	= 10080,
	RPC_QuestFlagData_UP_STATE	= 10089,
	RPC_QuestFlagData_REQ_STATE	= 10088,
	RPC_QuestFlagData_ENUM_ED	= 10089
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_QuestFlagData()
{
	RPC_ENUM_MAP[RPC_QuestFlagData_UP_STATE] = 1277844274;
	RPC_ENUM_MAP[RPC_QuestFlagData_REQ_STATE] = 413611357;
	return 0;
}
static int _addrpc_QuestFlagData = _AddRpcEnum_QuestFlagData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_QuestFlagData()
{
	RPC_NONBLOCK_MAP[RPC_QuestFlagData_UP_STATE] = true;
	return 0;
}
static int _nonblock_QuestFlagData = _NonblockMap_QuestFlagData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_QUESTFLAGDATA_ENUM
