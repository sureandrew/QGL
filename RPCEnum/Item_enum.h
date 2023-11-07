#pragma once
#ifndef RPC_ITEM_ENUM
#define RPC_ITEM_ENUM

enum RPC_Item_Enum
{
	Item_CLASSID	= 24000,
	RPC_Item_ENUM_ST	= 24000,
	RPC_Item_UP_STATE	= 24826,
	RPC_Item_REQ_STATE	= 24746,
	RPC_Item_ENUM_ED	= 24999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Item()
{
	RPC_ENUM_MAP[RPC_Item_UP_STATE] = 98268314;
	RPC_ENUM_MAP[RPC_Item_REQ_STATE] = 2078244581;
	return 0;
}
static int _addrpc_Item = _AddRpcEnum_Item();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Item()
{
	RPC_NONBLOCK_MAP[RPC_Item_UP_STATE] = true;
	return 0;
}
static int _nonblock_Item = _NonblockMap_Item();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_ITEM_ENUM
