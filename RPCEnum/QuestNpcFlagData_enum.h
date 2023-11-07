#pragma once
#ifndef RPC_QUESTNPCFLAGDATA_ENUM
#define RPC_QUESTNPCFLAGDATA_ENUM

enum RPC_QuestNpcFlagData_Enum
{
	QuestNpcFlagData_CLASSID	= 10340,
	RPC_QuestNpcFlagData_ENUM_ST	= 10340,
	RPC_QuestNpcFlagData_UP_STATE	= 10346,
	RPC_QuestNpcFlagData_REQ_STATE	= 10343,
	RPC_QuestNpcFlagData_ENUM_ED	= 10349
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_QuestNpcFlagData()
{
	RPC_ENUM_MAP[RPC_QuestNpcFlagData_UP_STATE] = 1391466966;
	RPC_ENUM_MAP[RPC_QuestNpcFlagData_REQ_STATE] = 772083845;
	return 0;
}
static int _addrpc_QuestNpcFlagData = _AddRpcEnum_QuestNpcFlagData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_QuestNpcFlagData()
{
	RPC_NONBLOCK_MAP[RPC_QuestNpcFlagData_UP_STATE] = true;
	return 0;
}
static int _nonblock_QuestNpcFlagData = _NonblockMap_QuestNpcFlagData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_QUESTNPCFLAGDATA_ENUM
