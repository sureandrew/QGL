#pragma once
#ifndef RPC_HAWKERHISTORYDATA_ENUM
#define RPC_HAWKERHISTORYDATA_ENUM

enum RPC_HawkerHistoryData_Enum
{
	HawkerHistoryData_CLASSID	= 10180,
	RPC_HawkerHistoryData_ENUM_ST	= 10180,
	RPC_HawkerHistoryData_UP_STATE	= 10184,
	RPC_HawkerHistoryData_REQ_STATE	= 10183,
	RPC_HawkerHistoryData_ENUM_ED	= 10189
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_HawkerHistoryData()
{
	RPC_ENUM_MAP[RPC_HawkerHistoryData_UP_STATE] = 1719150326;
	RPC_ENUM_MAP[RPC_HawkerHistoryData_REQ_STATE] = 1222730190;
	return 0;
}
static int _addrpc_HawkerHistoryData = _AddRpcEnum_HawkerHistoryData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_HawkerHistoryData()
{
	RPC_NONBLOCK_MAP[RPC_HawkerHistoryData_UP_STATE] = true;
	return 0;
}
static int _nonblock_HawkerHistoryData = _NonblockMap_HawkerHistoryData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_HAWKERHISTORYDATA_ENUM
