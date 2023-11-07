#pragma once
#ifndef RPC_MESSENGER_ENUM
#define RPC_MESSENGER_ENUM

enum RPC_Messenger_Enum
{
	Messenger_CLASSID	= 38000,
	RPC_Messenger_ENUM_ST	= 38000,
	RPC_Messenger_UP_STATE	= 38788,
	RPC_Messenger_REQ_STATE	= 38398,
	RPC_Messenger_MoveGroup	= 38163,
	RPC_Messenger_ModifyRelation	= 38626,
	RPC_Messenger_ENUM_ED	= 38999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Messenger()
{
	RPC_ENUM_MAP[RPC_Messenger_UP_STATE] = 494946743;
	RPC_ENUM_MAP[RPC_Messenger_REQ_STATE] = 1774213878;
	RPC_ENUM_MAP[RPC_Messenger_MoveGroup] = 1860468663;
	RPC_ENUM_MAP[RPC_Messenger_ModifyRelation] = 2088570675;
	return 0;
}
static int _addrpc_Messenger = _AddRpcEnum_Messenger();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Messenger()
{
	RPC_NONBLOCK_MAP[RPC_Messenger_UP_STATE] = true;
	return 0;
}
static int _nonblock_Messenger = _NonblockMap_Messenger();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_MESSENGER_ENUM
