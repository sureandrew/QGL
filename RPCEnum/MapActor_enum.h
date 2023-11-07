#pragma once
#ifndef RPC_MAPACTOR_ENUM
#define RPC_MAPACTOR_ENUM

enum RPC_MapActor_Enum
{
	MapActor_CLASSID	= 14000,
	RPC_MapActor_ENUM_ST	= 14000,
	RPC_MapActor_UP_STATE	= 14300,
	RPC_MapActor_REQ_STATE	= 14188,
	RPC_MapActor_ENUM_ED	= 14999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_MapActor()
{
	RPC_ENUM_MAP[RPC_MapActor_UP_STATE] = 1882347406;
	RPC_ENUM_MAP[RPC_MapActor_REQ_STATE] = 1964846251;
	return 0;
}
static int _addrpc_MapActor = _AddRpcEnum_MapActor();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_MapActor()
{
	RPC_NONBLOCK_MAP[RPC_MapActor_UP_STATE] = true;
	return 0;
}
static int _nonblock_MapActor = _NonblockMap_MapActor();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_MAPACTOR_ENUM
