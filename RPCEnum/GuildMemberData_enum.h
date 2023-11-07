#pragma once
#ifndef RPC_GUILDMEMBERDATA_ENUM
#define RPC_GUILDMEMBERDATA_ENUM

enum RPC_GuildMemberData_Enum
{
	GuildMemberData_CLASSID	= 10160,
	RPC_GuildMemberData_ENUM_ST	= 10160,
	RPC_GuildMemberData_UP_STATE	= 10168,
	RPC_GuildMemberData_REQ_STATE	= 10165,
	RPC_GuildMemberData_ENUM_ED	= 10169
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_GuildMemberData()
{
	RPC_ENUM_MAP[RPC_GuildMemberData_UP_STATE] = 759827620;
	RPC_ENUM_MAP[RPC_GuildMemberData_REQ_STATE] = 22637123;
	return 0;
}
static int _addrpc_GuildMemberData = _AddRpcEnum_GuildMemberData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_GuildMemberData()
{
	RPC_NONBLOCK_MAP[RPC_GuildMemberData_UP_STATE] = true;
	return 0;
}
static int _nonblock_GuildMemberData = _NonblockMap_GuildMemberData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_GUILDMEMBERDATA_ENUM
