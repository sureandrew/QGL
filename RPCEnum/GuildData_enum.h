#pragma once
#ifndef RPC_GUILDDATA_ENUM
#define RPC_GUILDDATA_ENUM

enum RPC_GuildData_Enum
{
	GuildData_CLASSID	= 10150,
	RPC_GuildData_ENUM_ST	= 10150,
	RPC_GuildData_UP_STATE	= 10151,
	RPC_GuildData_REQ_STATE	= 10152,
	RPC_GuildData_ENUM_ED	= 10159
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_GuildData()
{
	RPC_ENUM_MAP[RPC_GuildData_UP_STATE] = 1361462902;
	RPC_ENUM_MAP[RPC_GuildData_REQ_STATE] = 339936166;
	return 0;
}
static int _addrpc_GuildData = _AddRpcEnum_GuildData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_GuildData()
{
	RPC_NONBLOCK_MAP[RPC_GuildData_UP_STATE] = true;
	return 0;
}
static int _nonblock_GuildData = _NonblockMap_GuildData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_GUILDDATA_ENUM
