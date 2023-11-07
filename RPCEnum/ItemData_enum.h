#pragma once
#ifndef RPC_ITEMDATA_ENUM
#define RPC_ITEMDATA_ENUM

enum RPC_ItemData_Enum
{
	ItemData_CLASSID	= 10030,
	RPC_ItemData_ENUM_ST	= 10030,
	RPC_ItemData_UP_STATE	= 10033,
	RPC_ItemData_REQ_STATE	= 10036,
	RPC_ItemData_ENUM_ED	= 10039
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_ItemData()
{
	RPC_ENUM_MAP[RPC_ItemData_UP_STATE] = 2138537034;
	RPC_ENUM_MAP[RPC_ItemData_REQ_STATE] = 2000172110;
	return 0;
}
static int _addrpc_ItemData = _AddRpcEnum_ItemData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_ItemData()
{
	RPC_NONBLOCK_MAP[RPC_ItemData_UP_STATE] = true;
	return 0;
}
static int _nonblock_ItemData = _NonblockMap_ItemData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_ITEMDATA_ENUM
