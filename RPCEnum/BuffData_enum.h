#pragma once
#ifndef RPC_BUFFDATA_ENUM
#define RPC_BUFFDATA_ENUM

enum RPC_BuffData_Enum
{
	BuffData_CLASSID	= 10060,
	RPC_BuffData_ENUM_ST	= 10060,
	RPC_BuffData_UP_STATE	= 10066,
	RPC_BuffData_REQ_STATE	= 10062,
	RPC_BuffData_ENUM_ED	= 10069
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_BuffData()
{
	RPC_ENUM_MAP[RPC_BuffData_UP_STATE] = 348675274;
	RPC_ENUM_MAP[RPC_BuffData_REQ_STATE] = 309682342;
	return 0;
}
static int _addrpc_BuffData = _AddRpcEnum_BuffData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_BuffData()
{
	RPC_NONBLOCK_MAP[RPC_BuffData_UP_STATE] = true;
	return 0;
}
static int _nonblock_BuffData = _NonblockMap_BuffData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_BUFFDATA_ENUM
