#pragma once
#ifndef RPC_BATMOB_ENUM
#define RPC_BATMOB_ENUM

enum RPC_BatMob_Enum
{
	BatMob_CLASSID	= 19000,
	RPC_BatMob_ENUM_ST	= 19000,
	RPC_BatMob_UP_STATE	= 19036,
	RPC_BatMob_REQ_STATE	= 19499,
	RPC_BatMob_ENUM_ED	= 19999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_BatMob()
{
	RPC_ENUM_MAP[RPC_BatMob_UP_STATE] = 270741010;
	RPC_ENUM_MAP[RPC_BatMob_REQ_STATE] = 39729214;
	return 0;
}
static int _addrpc_BatMob = _AddRpcEnum_BatMob();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_BatMob()
{
	RPC_NONBLOCK_MAP[RPC_BatMob_UP_STATE] = true;
	return 0;
}
static int _nonblock_BatMob = _NonblockMap_BatMob();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_BATMOB_ENUM
