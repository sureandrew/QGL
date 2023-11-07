#pragma once
#ifndef RPC_SERVICESTATUS_ENUM
#define RPC_SERVICESTATUS_ENUM

enum RPC_ServiceStatus_Enum
{
	RPC_ServiceStatus_ENUM_ST	= 7500,
	RPC_ServiceStatus_UpdateStatus	= 7738,
	RPC_ServiceStatus_CB_CheckService	= 7739,
	RPC_ServiceStatus_CB_ShutdownAt	= 7633,
	RPC_ServiceStatus_CB_StopService	= 7554,
	RPC_ServiceStatus_ENUM_ED	= 7999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_ServiceStatus()
{
	RPC_ENUM_MAP[RPC_ServiceStatus_UpdateStatus] = 0;
	RPC_ENUM_MAP[RPC_ServiceStatus_CB_CheckService] = 0;
	RPC_ENUM_MAP[RPC_ServiceStatus_CB_ShutdownAt] = 0;
	RPC_ENUM_MAP[RPC_ServiceStatus_CB_StopService] = 0;
	return 0;
}
static int _addrpc_ServiceStatus = _AddRpcEnum_ServiceStatus();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_ServiceStatus()
{
	RPC_NONBLOCK_MAP[RPC_ServiceStatus_CB_CheckService] = true;
	RPC_NONBLOCK_MAP[RPC_ServiceStatus_CB_ShutdownAt] = true;
	RPC_NONBLOCK_MAP[RPC_ServiceStatus_CB_StopService] = true;
	return 0;
}
static int _nonblock_ServiceStatus = _NonblockMap_ServiceStatus();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_SERVICESTATUS_ENUM
