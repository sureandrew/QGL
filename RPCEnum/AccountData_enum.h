#pragma once
#ifndef RPC_ACCOUNTDATA_ENUM
#define RPC_ACCOUNTDATA_ENUM

enum RPC_AccountData_Enum
{
	AccountData_CLASSID	= 10010,
	RPC_AccountData_ENUM_ST	= 10010,
	RPC_AccountData_UP_STATE	= 10019,
	RPC_AccountData_REQ_STATE	= 10016,
	RPC_AccountData_ENUM_ED	= 10019
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_AccountData()
{
	RPC_ENUM_MAP[RPC_AccountData_UP_STATE] = 1209368035;
	RPC_ENUM_MAP[RPC_AccountData_REQ_STATE] = 174461286;
	return 0;
}
static int _addrpc_AccountData = _AddRpcEnum_AccountData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_AccountData()
{
	RPC_NONBLOCK_MAP[RPC_AccountData_UP_STATE] = true;
	return 0;
}
static int _nonblock_AccountData = _NonblockMap_AccountData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_ACCOUNTDATA_ENUM
