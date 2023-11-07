#pragma once
#ifndef RPC_RANKGUILD_ENUM
#define RPC_RANKGUILD_ENUM

enum RPC_RankGuild_Enum
{
	RankGuild_CLASSID	= 57000,
	RPC_RankGuild_ENUM_ST	= 57000,
	RPC_RankGuild_UP_STATE	= 57324,
	RPC_RankGuild_REQ_STATE	= 57151,
	RPC_RankGuild_ENUM_ED	= 57999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_RankGuild()
{
	RPC_ENUM_MAP[RPC_RankGuild_UP_STATE] = 500916192;
	RPC_ENUM_MAP[RPC_RankGuild_REQ_STATE] = 2126587474;
	return 0;
}
static int _addrpc_RankGuild = _AddRpcEnum_RankGuild();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_RankGuild()
{
	RPC_NONBLOCK_MAP[RPC_RankGuild_UP_STATE] = true;
	return 0;
}
static int _nonblock_RankGuild = _NonblockMap_RankGuild();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_RANKGUILD_ENUM
