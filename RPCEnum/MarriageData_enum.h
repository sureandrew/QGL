#pragma once
#ifndef RPC_MARRIAGEDATA_ENUM
#define RPC_MARRIAGEDATA_ENUM

enum RPC_MarriageData_Enum
{
	MarriageData_CLASSID	= 10320,
	RPC_MarriageData_ENUM_ST	= 10320,
	RPC_MarriageData_UP_STATE	= 10323,
	RPC_MarriageData_REQ_STATE	= 10321,
	RPC_MarriageData_ENUM_ED	= 10329
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_MarriageData()
{
	RPC_ENUM_MAP[RPC_MarriageData_UP_STATE] = 462297967;
	RPC_ENUM_MAP[RPC_MarriageData_REQ_STATE] = 1093954973;
	return 0;
}
static int _addrpc_MarriageData = _AddRpcEnum_MarriageData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_MarriageData()
{
	RPC_NONBLOCK_MAP[RPC_MarriageData_UP_STATE] = true;
	return 0;
}
static int _nonblock_MarriageData = _NonblockMap_MarriageData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_MARRIAGEDATA_ENUM
