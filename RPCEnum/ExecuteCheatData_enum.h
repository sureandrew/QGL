#pragma once
#ifndef RPC_EXECUTECHEATDATA_ENUM
#define RPC_EXECUTECHEATDATA_ENUM

enum RPC_ExecuteCheatData_Enum
{
	ExecuteCheatData_CLASSID	= 10190,
	RPC_ExecuteCheatData_ENUM_ST	= 10190,
	RPC_ExecuteCheatData_UP_STATE	= 10196,
	RPC_ExecuteCheatData_REQ_STATE	= 10198,
	RPC_ExecuteCheatData_ENUM_ED	= 10200
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_ExecuteCheatData()
{
	RPC_ENUM_MAP[RPC_ExecuteCheatData_UP_STATE] = 1117547812;
	RPC_ENUM_MAP[RPC_ExecuteCheatData_REQ_STATE] = 905398378;
	return 0;
}
static int _addrpc_ExecuteCheatData = _AddRpcEnum_ExecuteCheatData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_ExecuteCheatData()
{
	RPC_NONBLOCK_MAP[RPC_ExecuteCheatData_UP_STATE] = true;
	return 0;
}
static int _nonblock_ExecuteCheatData = _NonblockMap_ExecuteCheatData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_EXECUTECHEATDATA_ENUM
