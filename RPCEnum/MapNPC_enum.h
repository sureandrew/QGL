#pragma once
#ifndef RPC_MAPNPC_ENUM
#define RPC_MAPNPC_ENUM

enum RPC_MapNPC_Enum
{
	MapNPC_CLASSID	= 15000,
	RPC_MapNPC_ENUM_ST	= 15000,
	RPC_MapNPC_UP_STATE	= 15137,
	RPC_MapNPC_REQ_STATE	= 15804,
	RPC_MapNPC_ENUM_ED	= 15999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_MapNPC()
{
	RPC_ENUM_MAP[RPC_MapNPC_UP_STATE] = 199364153;
	RPC_ENUM_MAP[RPC_MapNPC_REQ_STATE] = 714159708;
	return 0;
}
static int _addrpc_MapNPC = _AddRpcEnum_MapNPC();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_MapNPC()
{
	RPC_NONBLOCK_MAP[RPC_MapNPC_UP_STATE] = true;
	return 0;
}
static int _nonblock_MapNPC = _NonblockMap_MapNPC();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_MAPNPC_ENUM
