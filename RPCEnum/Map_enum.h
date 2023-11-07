#pragma once
#ifndef RPC_MAP_ENUM
#define RPC_MAP_ENUM

enum RPC_Map_Enum
{
	Map_CLASSID	= 16000,
	RPC_Map_ENUM_ST	= 16000,
	RPC_Map_UP_STATE	= 16560,
	RPC_Map_REQ_STATE	= 16797,
	RPC_Map_GetCharInRange	= 16630,
	RPC_Map_GetJumpPoints	= 16864,
	RPC_Map_CB_Error	= 16569,
	RPC_Map_CB_GetCharInRange	= 16358,
	RPC_Map_CB_GetJumpPoints	= 16187,
	RPC_Map_CB_ObjectChanged	= 16889,
	RPC_Map_ENUM_ED	= 16999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Map()
{
	RPC_ENUM_MAP[RPC_Map_UP_STATE] = 1831614892;
	RPC_ENUM_MAP[RPC_Map_REQ_STATE] = 467337577;
	RPC_ENUM_MAP[RPC_Map_GetCharInRange] = 1471414690;
	RPC_ENUM_MAP[RPC_Map_GetJumpPoints] = 421754030;
	RPC_ENUM_MAP[RPC_Map_CB_Error] = 1564824712;
	RPC_ENUM_MAP[RPC_Map_CB_GetCharInRange] = 814557048;
	RPC_ENUM_MAP[RPC_Map_CB_GetJumpPoints] = 1947233008;
	RPC_ENUM_MAP[RPC_Map_CB_ObjectChanged] = 1878792989;
	return 0;
}
static int _addrpc_Map = _AddRpcEnum_Map();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Map()
{
	RPC_NONBLOCK_MAP[RPC_Map_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_Map_CB_Error] = true;
	RPC_NONBLOCK_MAP[RPC_Map_CB_GetCharInRange] = true;
	RPC_NONBLOCK_MAP[RPC_Map_CB_GetJumpPoints] = true;
	RPC_NONBLOCK_MAP[RPC_Map_CB_ObjectChanged] = true;
	return 0;
}
static int _nonblock_Map = _NonblockMap_Map();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_MAP_ENUM
