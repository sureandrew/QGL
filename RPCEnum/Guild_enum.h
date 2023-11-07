#pragma once
#ifndef RPC_GUILD_ENUM
#define RPC_GUILD_ENUM

enum RPC_Guild_Enum
{
	Guild_CLASSID	= 49000,
	RPC_Guild_ENUM_ST	= 49000,
	RPC_Guild_UP_STATE	= 49714,
	RPC_Guild_REQ_STATE	= 49905,
	RPC_Guild_CB_AddMember	= 49037,
	RPC_Guild_CB_RemoveMember	= 49712,
	RPC_Guild_CB_AddApplicant	= 49717,
	RPC_Guild_CB_RemoveApplicant	= 49888,
	RPC_Guild_ENUM_ED	= 49999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Guild()
{
	RPC_ENUM_MAP[RPC_Guild_UP_STATE] = 327943234;
	RPC_ENUM_MAP[RPC_Guild_REQ_STATE] = 1953582571;
	RPC_ENUM_MAP[RPC_Guild_CB_AddMember] = 898267510;
	RPC_ENUM_MAP[RPC_Guild_CB_RemoveMember] = 104226238;
	RPC_ENUM_MAP[RPC_Guild_CB_AddApplicant] = 477824439;
	RPC_ENUM_MAP[RPC_Guild_CB_RemoveApplicant] = 1885145231;
	return 0;
}
static int _addrpc_Guild = _AddRpcEnum_Guild();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Guild()
{
	RPC_NONBLOCK_MAP[RPC_Guild_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_Guild_CB_AddMember] = true;
	RPC_NONBLOCK_MAP[RPC_Guild_CB_RemoveMember] = true;
	RPC_NONBLOCK_MAP[RPC_Guild_CB_AddApplicant] = true;
	RPC_NONBLOCK_MAP[RPC_Guild_CB_RemoveApplicant] = true;
	return 0;
}
static int _nonblock_Guild = _NonblockMap_Guild();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_GUILD_ENUM
