#pragma once
#ifndef RPC_GIFT_ENUM
#define RPC_GIFT_ENUM

enum RPC_Gift_Enum
{
	Gift_CLASSID	= 52000,
	RPC_Gift_ENUM_ST	= 52000,
	RPC_Gift_UP_STATE	= 52981,
	RPC_Gift_REQ_STATE	= 52651,
	RPC_Gift_ENUM_ED	= 52999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Gift()
{
	RPC_ENUM_MAP[RPC_Gift_UP_STATE] = 929695389;
	RPC_ENUM_MAP[RPC_Gift_REQ_STATE] = 1213148945;
	return 0;
}
static int _addrpc_Gift = _AddRpcEnum_Gift();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Gift()
{
	RPC_NONBLOCK_MAP[RPC_Gift_UP_STATE] = true;
	return 0;
}
static int _nonblock_Gift = _NonblockMap_Gift();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_GIFT_ENUM
