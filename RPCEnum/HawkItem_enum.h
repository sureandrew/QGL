#pragma once
#ifndef RPC_HAWKITEM_ENUM
#define RPC_HAWKITEM_ENUM

enum RPC_HawkItem_Enum
{
	HawkItem_CLASSID	= 48000,
	RPC_HawkItem_ENUM_ST	= 48000,
	RPC_HawkItem_UP_STATE	= 48082,
	RPC_HawkItem_REQ_STATE	= 48956,
	RPC_HawkItem_ENUM_ED	= 48999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_HawkItem()
{
	RPC_ENUM_MAP[RPC_HawkItem_UP_STATE] = 858285763;
	RPC_ENUM_MAP[RPC_HawkItem_REQ_STATE] = 1887579439;
	return 0;
}
static int _addrpc_HawkItem = _AddRpcEnum_HawkItem();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_HawkItem()
{
	RPC_NONBLOCK_MAP[RPC_HawkItem_UP_STATE] = true;
	return 0;
}
static int _nonblock_HawkItem = _NonblockMap_HawkItem();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_HAWKITEM_ENUM
