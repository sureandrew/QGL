#pragma once
#ifndef RPC_SIBLINGDATA_ENUM
#define RPC_SIBLINGDATA_ENUM

enum RPC_SiblingData_Enum
{
	SiblingData_CLASSID	= 10310,
	RPC_SiblingData_ENUM_ST	= 10310,
	RPC_SiblingData_UP_STATE	= 10315,
	RPC_SiblingData_REQ_STATE	= 10310,
	RPC_SiblingData_ENUM_ED	= 10319
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_SiblingData()
{
	RPC_ENUM_MAP[RPC_SiblingData_UP_STATE] = 1063933249;
	RPC_ENUM_MAP[RPC_SiblingData_REQ_STATE] = 1411188480;
	return 0;
}
static int _addrpc_SiblingData = _AddRpcEnum_SiblingData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_SiblingData()
{
	RPC_NONBLOCK_MAP[RPC_SiblingData_UP_STATE] = true;
	return 0;
}
static int _nonblock_SiblingData = _NonblockMap_SiblingData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_SIBLINGDATA_ENUM
