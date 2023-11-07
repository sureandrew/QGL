#pragma once
#ifndef RPC_GUILDMEMBER_ENUM
#define RPC_GUILDMEMBER_ENUM

enum RPC_GuildMember_Enum
{
	GuildMember_CLASSID	= 50000,
	RPC_GuildMember_ENUM_ST	= 50000,
	RPC_GuildMember_UP_STATE	= 50535,
	RPC_GuildMember_REQ_STATE	= 50130,
	RPC_GuildMember_ResponseApplicant	= 50112,
	RPC_GuildMember_PositionChange	= 50432,
	RPC_GuildMember_ChangeAim	= 50429,
	RPC_GuildMember_ChangeRule	= 50005,
	RPC_GuildMember_ClearGuildBusinessLog	= 50051,
	RPC_GuildMember_CB_Error	= 50693,
	RPC_GuildMember_CB_PositionChange	= 50966,
	RPC_GuildMember_ENUM_ED	= 50999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_GuildMember()
{
	RPC_ENUM_MAP[RPC_GuildMember_UP_STATE] = 1010581609;
	RPC_ENUM_MAP[RPC_GuildMember_REQ_STATE] = 2085039862;
	RPC_ENUM_MAP[RPC_GuildMember_ResponseApplicant] = 1299078484;
	RPC_ENUM_MAP[RPC_GuildMember_PositionChange] = 254953419;
	RPC_ENUM_MAP[RPC_GuildMember_ChangeAim] = 832781680;
	RPC_ENUM_MAP[RPC_GuildMember_ChangeRule] = 971653655;
	RPC_ENUM_MAP[RPC_GuildMember_ClearGuildBusinessLog] = 2104772248;
	RPC_ENUM_MAP[RPC_GuildMember_CB_Error] = 986982435;
	RPC_ENUM_MAP[RPC_GuildMember_CB_PositionChange] = 944125522;
	return 0;
}
static int _addrpc_GuildMember = _AddRpcEnum_GuildMember();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_GuildMember()
{
	RPC_NONBLOCK_MAP[RPC_GuildMember_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_GuildMember_CB_Error] = true;
	RPC_NONBLOCK_MAP[RPC_GuildMember_CB_PositionChange] = true;
	return 0;
}
static int _nonblock_GuildMember = _NonblockMap_GuildMember();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_GUILDMEMBER_ENUM
