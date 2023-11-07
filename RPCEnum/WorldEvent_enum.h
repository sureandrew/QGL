#pragma once
#ifndef RPC_WORLDEVENT_ENUM
#define RPC_WORLDEVENT_ENUM

enum RPC_WorldEvent_Enum
{
	WorldEvent_CLASSID	= 46000,
	RPC_WorldEvent_ENUM_ST	= 46000,
	RPC_WorldEvent_UP_STATE	= 46822,
	RPC_WorldEvent_REQ_STATE	= 46347,
	RPC_WorldEvent_ENUM_ED	= 46999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_WorldEvent()
{
	RPC_ENUM_MAP[RPC_WorldEvent_UP_STATE] = 909051045;
	RPC_ENUM_MAP[RPC_WorldEvent_REQ_STATE] = 1237604466;
	return 0;
}
static int _addrpc_WorldEvent = _AddRpcEnum_WorldEvent();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_WorldEvent()
{
	RPC_NONBLOCK_MAP[RPC_WorldEvent_UP_STATE] = true;
	return 0;
}
static int _nonblock_WorldEvent = _NonblockMap_WorldEvent();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_WORLDEVENT_ENUM
