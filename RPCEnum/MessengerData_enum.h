#pragma once
#ifndef RPC_MESSENGERDATA_ENUM
#define RPC_MESSENGERDATA_ENUM

enum RPC_MessengerData_Enum
{
	MessengerData_CLASSID	= 10100,
	RPC_MessengerData_ENUM_ST	= 10100,
	RPC_MessengerData_UP_STATE	= 10104,
	RPC_MessengerData_REQ_STATE	= 10102,
	RPC_MessengerData_ENUM_ED	= 10109
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_MessengerData()
{
	RPC_ENUM_MAP[RPC_MessengerData_UP_STATE] = 74606478;
	RPC_ENUM_MAP[RPC_MessengerData_REQ_STATE] = 1926431383;
	return 0;
}
static int _addrpc_MessengerData = _AddRpcEnum_MessengerData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_MessengerData()
{
	RPC_NONBLOCK_MAP[RPC_MessengerData_UP_STATE] = true;
	return 0;
}
static int _nonblock_MessengerData = _NonblockMap_MessengerData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_MESSENGERDATA_ENUM
