//-- Common
#include "Common.h"
//-- Self
#include "Stub/Service/ManagerService_stub.h"

#pragma optimize("", off)
RPCResult ManagerService_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
#ifndef CLIENT
	case RPC_ManagerService_RegisterManager:
		{
			_result = RegisterManager(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_RegisterSlave:
		{
			_result = RegisterSlave(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_ForwardStatus:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			UInt32 status;
			pBuf->Read<UInt32>(status);
			UInt32 state;
			pBuf->Read<UInt32>(state);
			_result = ForwardStatus(pPeer, context, serviceId, status, state);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_ForwardStatuses:
		{
			ServiceStatusesMap services_statuses;
			pBuf->Read<ServiceStatusesMap>(services_statuses);
			_result = ForwardStatuses(pPeer, context, services_statuses);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_ForwardCheats:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = ForwardCheats(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_LoadConfig:
		{
			String configFile;
			pBuf->Read<String>(configFile);
			_result = LoadConfig(pPeer, context, configFile);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_CheckAllServices:
		{
			_result = CheckAllServices(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_StartAllServices:
		{
			_result = StartAllServices(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_StopAllServices:
		{
			Boolean bStopClose;
			pBuf->Read<Boolean>(bStopClose);
			_result = StopAllServices(pPeer, context, bStopClose);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_RestartAllServices:
		{
			Boolean bErr;
			pBuf->Read<Boolean>(bErr);
			_result = RestartAllServices(pPeer, context, bErr);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_CheckService:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			_result = CheckService(pPeer, context, serviceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_StartService:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			_result = StartService(pPeer, context, serviceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_StopService:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			_result = StopService(pPeer, context, serviceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_RestartService:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			_result = RestartService(pPeer, context, serviceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_ErrorArchive:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			_result = ErrorArchive(pPeer, context, serviceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_ShutdownAt:
		{
			UInt32 downTime;
			pBuf->Read<UInt32>(downTime);
			_result = ShutdownAt(pPeer, context, downTime);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_RegisterSimClient:
		{
			String prog;
			pBuf->Read<String>(prog);
			String title;
			pBuf->Read<String>(title);
			String workDir;
			pBuf->Read<String>(workDir);
			String args;
			pBuf->Read<String>(args);
			_result = RegisterSimClient(pPeer, context, prog, title, workDir, args);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_UnregisterSimClient:
		{
			_result = UnregisterSimClient(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ManagerService_StartProcess:
		{
			UInt32 time;
			pBuf->Read<UInt32>(time);
			String prog;
			pBuf->Read<String>(prog);
			String title;
			pBuf->Read<String>(title);
			String workDir;
			pBuf->Read<String>(workDir);
			String cmd;
			pBuf->Read<String>(cmd);
			_result = StartProcess(pPeer, context, time, prog, title, workDir, cmd);
		}
		break;
#endif // CLIENT
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}
#pragma optimize("", on)

