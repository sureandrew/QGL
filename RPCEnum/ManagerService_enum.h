#pragma once
#ifndef RPC_MANAGERSERVICE_ENUM
#define RPC_MANAGERSERVICE_ENUM

enum RPC_ManagerService_Enum
{
	RPC_ManagerService_ENUM_ST	= 7000,
	RPC_ManagerService_RegisterManager	= 7027,
	RPC_ManagerService_RegisterSlave	= 7403,
	RPC_ManagerService_ForwardStatus	= 7455,
	RPC_ManagerService_ForwardStatuses	= 7290,
	RPC_ManagerService_ForwardCheats	= 7257,
	RPC_ManagerService_LoadConfig	= 7053,
	RPC_ManagerService_CheckAllServices	= 7120,
	RPC_ManagerService_StartAllServices	= 7084,
	RPC_ManagerService_StopAllServices	= 7079,
	RPC_ManagerService_RestartAllServices	= 7230,
	RPC_ManagerService_CheckService	= 7138,
	RPC_ManagerService_StartService	= 7158,
	RPC_ManagerService_StopService	= 7145,
	RPC_ManagerService_RestartService	= 7148,
	RPC_ManagerService_ErrorArchive	= 7306,
	RPC_ManagerService_ShutdownAt	= 7310,
	RPC_ManagerService_RegisterSimClient	= 7365,
	RPC_ManagerService_UnregisterSimClient	= 7078,
	RPC_ManagerService_StartProcess	= 7150,
	RPC_ManagerService_ENUM_ED	= 7499
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_ManagerService()
{
	RPC_ENUM_MAP[RPC_ManagerService_RegisterManager] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_RegisterSlave] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_ForwardStatus] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_ForwardStatuses] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_ForwardCheats] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_LoadConfig] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_CheckAllServices] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_StartAllServices] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_StopAllServices] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_RestartAllServices] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_CheckService] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_StartService] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_StopService] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_RestartService] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_ErrorArchive] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_ShutdownAt] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_RegisterSimClient] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_UnregisterSimClient] = 0;
	RPC_ENUM_MAP[RPC_ManagerService_StartProcess] = 0;
	return 0;
}
static int _addrpc_ManagerService = _AddRpcEnum_ManagerService();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
#endif // RPC_NONBLOCK_MAP

#endif //RPC_MANAGERSERVICE_ENUM
