#pragma once
#ifndef RPC_GIFTDATA_ENUM
#define RPC_GIFTDATA_ENUM

enum RPC_GiftData_Enum
{
	GiftData_CLASSID	= 10170,
	RPC_GiftData_ENUM_ST	= 10170,
	RPC_GiftData_UP_STATE	= 10175,
	RPC_GiftData_REQ_STATE	= 10176,
	RPC_GiftData_ENUM_ED	= 10179
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_GiftData()
{
	RPC_ENUM_MAP[RPC_GiftData_UP_STATE] = 158225106;
	RPC_ENUM_MAP[RPC_GiftData_REQ_STATE] = 1852788960;
	return 0;
}
static int _addrpc_GiftData = _AddRpcEnum_GiftData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_GiftData()
{
	RPC_NONBLOCK_MAP[RPC_GiftData_UP_STATE] = true;
	return 0;
}
static int _nonblock_GiftData = _NonblockMap_GiftData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_GIFTDATA_ENUM
