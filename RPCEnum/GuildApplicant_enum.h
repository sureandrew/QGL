#pragma once
#ifndef RPC_GUILDAPPLICANT_ENUM
#define RPC_GUILDAPPLICANT_ENUM

enum RPC_GuildApplicant_Enum
{
	GuildApplicant_CLASSID	= 51000,
	RPC_GuildApplicant_ENUM_ST	= 51000,
	RPC_GuildApplicant_UP_STATE	= 51166,
	RPC_GuildApplicant_REQ_STATE	= 51847,
	RPC_GuildApplicant_ENUM_ED	= 51999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_GuildApplicant()
{
	RPC_ENUM_MAP[RPC_GuildApplicant_UP_STATE] = 480271848;
	RPC_ENUM_MAP[RPC_GuildApplicant_REQ_STATE] = 3559346;
	return 0;
}
static int _addrpc_GuildApplicant = _AddRpcEnum_GuildApplicant();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_GuildApplicant()
{
	RPC_NONBLOCK_MAP[RPC_GuildApplicant_UP_STATE] = true;
	return 0;
}
static int _nonblock_GuildApplicant = _NonblockMap_GuildApplicant();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_GUILDAPPLICANT_ENUM
