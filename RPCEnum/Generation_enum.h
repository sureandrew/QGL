#pragma once
#ifndef RPC_GENERATION_ENUM
#define RPC_GENERATION_ENUM

enum RPC_Generation_Enum
{
	Generation_CLASSID	= 61500,
	RPC_Generation_ENUM_ST	= 61500,
	RPC_Generation_UP_STATE	= 61832,
	RPC_Generation_REQ_STATE	= 61844,
	RPC_Generation_ENUM_ED	= 61999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Generation()
{
	RPC_ENUM_MAP[RPC_Generation_UP_STATE] = 1512597031;
	RPC_ENUM_MAP[RPC_Generation_REQ_STATE] = 1707025338;
	return 0;
}
static int _addrpc_Generation = _AddRpcEnum_Generation();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Generation()
{
	RPC_NONBLOCK_MAP[RPC_Generation_UP_STATE] = true;
	return 0;
}
static int _nonblock_Generation = _NonblockMap_Generation();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_GENERATION_ENUM
