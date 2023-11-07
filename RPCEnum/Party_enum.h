#pragma once
#ifndef RPC_PARTY_ENUM
#define RPC_PARTY_ENUM

enum RPC_Party_Enum
{
	Party_CLASSID	= 22000,
	RPC_Party_ENUM_ST	= 22000,
	RPC_Party_UP_STATE	= 22567,
	RPC_Party_REQ_STATE	= 22137,
	RPC_Party_Promote	= 22008,
	RPC_Party_Kick	= 22298,
	RPC_Party_Leave	= 22478,
	RPC_Party_Destroy	= 22510,
	RPC_Party_JoinChannel	= 22918,
	RPC_Party_ChangeDesc	= 22344,
	RPC_Party_CB_Error	= 22987,
	RPC_Party_CB_LeaderChanged	= 22595,
	RPC_Party_CB_Join	= 22091,
	RPC_Party_CB_Leave	= 22050,
	RPC_Party_CB_Destroy	= 22806,
	RPC_Party_CB_Regroup	= 22022,
	RPC_Party_ENUM_ED	= 22999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Party()
{
	RPC_ENUM_MAP[RPC_Party_UP_STATE] = 148968060;
	RPC_ENUM_MAP[RPC_Party_REQ_STATE] = 1428171303;
	RPC_ENUM_MAP[RPC_Party_Promote] = 1734695565;
	RPC_ENUM_MAP[RPC_Party_Kick] = 1415006757;
	RPC_ENUM_MAP[RPC_Party_Leave] = 887490946;
	RPC_ENUM_MAP[RPC_Party_Destroy] = 367799813;
	RPC_ENUM_MAP[RPC_Party_JoinChannel] = 1174107192;
	RPC_ENUM_MAP[RPC_Party_ChangeDesc] = 223169529;
	RPC_ENUM_MAP[RPC_Party_CB_Error] = 861240151;
	RPC_ENUM_MAP[RPC_Party_CB_LeaderChanged] = 154238635;
	RPC_ENUM_MAP[RPC_Party_CB_Join] = 794261500;
	RPC_ENUM_MAP[RPC_Party_CB_Leave] = 948005376;
	RPC_ENUM_MAP[RPC_Party_CB_Destroy] = 809373378;
	RPC_ENUM_MAP[RPC_Party_CB_Regroup] = 1355247430;
	return 0;
}
static int _addrpc_Party = _AddRpcEnum_Party();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Party()
{
	RPC_NONBLOCK_MAP[RPC_Party_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_Party_CB_Error] = true;
	RPC_NONBLOCK_MAP[RPC_Party_CB_LeaderChanged] = true;
	RPC_NONBLOCK_MAP[RPC_Party_CB_Join] = true;
	RPC_NONBLOCK_MAP[RPC_Party_CB_Leave] = true;
	RPC_NONBLOCK_MAP[RPC_Party_CB_Destroy] = true;
	RPC_NONBLOCK_MAP[RPC_Party_CB_Regroup] = true;
	return 0;
}
static int _nonblock_Party = _NonblockMap_Party();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_PARTY_ENUM
