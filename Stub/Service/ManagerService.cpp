//-- Common
#include "Common.h"
//-- Self
#include "Stub/Service/ManagerService_stub.h"
//-- Library
#include "Proxy/Service/ManagerService_proxy.h"
#include "Stub/Service/ServiceStatus_stub.h"
#include "ManagerService/Global.h"
#include "ManagerService/ExecuteCheat.h"
#include "dboServiceStatus.h"

RPCResult ManagerService_Stub::RegisterManager(LPCPEER pPeer, RPCContext &context)
{
	gGlobal.RegisterManager(pPeer);

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::RegisterSlave(LPCPEER pPeer, RPCContext &context)
{
	//gGlobal.RegisterManager(pPeer);

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::ForwardStatus(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt32 &status, const UInt32 &state)
{
	ServiceEx *pService = gGlobal.GetServiceEx(serviceId);

	// serviceId not found
	if (pService == NULL)
		return RPC_RESULT_OK;

	// no status changed
	if (pService->statuses.find(status) != pService->statuses.end() &&
		pService->statuses[status] == state)
		return RPC_RESULT_OK;

	TRACE_INFODTL_5(GLOBAL_LOGGER, 
		_T("ForwardStatus: %s service, serviceId %d, status %d, state %d, from %s"), 
		pService->strName.c_str(), serviceId, status, state,
		Reuben::Network::IPToString(pPeer->GetAddressRemote().GetIP()).c_str());
	pService->statuses[status] = state;
	if (gGlobal.m_pdboServiceStatus)
	{
		HRESULT hr = gGlobal.m_pdboServiceStatus->Update(serviceId, status, state);
		if (DB_FAILED(hr))
		{
			TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("db failed, hr %d, service_id %d, status %s, state %s"),
				hr, pService->serviceId, GetServiceStatusesString(status), GetServiceRunningStatusString(state));
		}
	}
	if (status == SS_RUNNING)
	{
		if (state == SRS_STARTED || state == SRS_STOPPED || state == SRS_UNKNOWN)
			gGlobal.KickWaiting(serviceId, state);
		if (state == SRS_STARTED)
			gGlobal.CheckAllStarted();
		else if (gGlobal.m_bStopClose && (state == SRS_STOPPED || state == SRS_UNKNOWN))
			gGlobal.CheckAllStopped();
	}

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::ForwardStatuses(LPCPEER pPeer, RPCContext &context, const ServiceStatusesMap &services_statuses)
{
	if (services_statuses.empty())
		return RPC_RESULT_OK;

	bool bCheckAllStarted = false;

	for (ServiceStatusesMap::const_iterator it = services_statuses.begin(); it != services_statuses.end(); ++it)
	{
		if (!it->second.empty())
		{
			ServiceEx* pService = gGlobal.GetServiceEx(it->first);
			if (pService == NULL)
				continue;
			const StatusesMap& statuses = it->second;
			for (StatusesMap::const_iterator its = statuses.begin(); its != statuses.end(); ++its)
			{
				if (pService->statuses.find(its->first) != pService->statuses.end() &&
					pService->statuses[its->first] == its->second)
					continue;

				TRACE_INFODTL_5(GLOBAL_LOGGER, 
					_T("ForwardStatuses: %s service, serviceId %d, status %d, state %d, from %s"), 
					pService->strName.c_str(), pService->serviceId, its->first, its->second,
					Reuben::Network::IPToString(pPeer->GetAddressRemote().GetIP()).c_str());
				pService->statuses[its->first] = its->second;
				if (gGlobal.m_pdboServiceStatus)
				{
					HRESULT hr = gGlobal.m_pdboServiceStatus->Update(it->first, its->first, its->second);
					if (DB_FAILED(hr))
					{
						TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("db failed, hr %d, service_id %d, status %s, state %s"),
							hr, pService->serviceId, GetServiceStatusesString(its->first), GetServiceRunningStatusString(its->second));
					}
				}
				if (its->first == SS_RUNNING)
				{
					if (its->second == SRS_STARTED || its->second == SRS_STOPPED || its->second == SRS_UNKNOWN)
						gGlobal.KickWaiting(pService->serviceId, its->second);
					if (!bCheckAllStarted && its->second == SRS_STARTED)
						bCheckAllStarted = true;
				}
			}
		}
	}

	if (bCheckAllStarted)
		gGlobal.CheckAllStarted();
	
	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::ForwardCheats(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (gGlobal.m_pExecuteCheat)
		gGlobal.m_pExecuteCheat->SendCheat(grp);

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::LoadConfig(LPCPEER pPeer, RPCContext &context, const String &configFile)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("LoadConfig: %s"), configFile.c_str());

	gGlobal.ReadConfig(configFile.c_str());

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::CheckAllServices(LPCPEER pPeer, RPCContext &context)
{
	ServiceStatusesMap service_statuses;

	for (ServiceExMap::iterator it = gGlobal.m_srvExMap.begin(); it != gGlobal.m_srvExMap.end(); ++it)
	{
		ServiceEx& service = it->second;
		if (service.bLocal && !service.statuses.empty())
			service_statuses[service.serviceId] = service.statuses;
	}

	// for testing only
	for (ServiceStatusesMap::iterator it = service_statuses.begin(); it!= service_statuses.end(); ++it)
	{
		StatusesMap& statuses = it->second;
		for (StatusesMap::iterator it2 = statuses.begin(); it2 != statuses.end(); ++it2)
		{
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("service_id %d, status %d, state %d"), it->first, it2->first, it2->second);
		}
	}

	if (!service_statuses.empty())
		ManagerService_Proxy::ForwardStatuses(pPeer, context, service_statuses);

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::StartAllServices(LPCPEER pPeer, RPCContext &context)
{
	gGlobal.StartAllServices(BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::StopAllServices(LPCPEER pPeer, RPCContext &context, const Boolean &bStopClose)
{
	if (bStopClose)
		gGlobal.m_bStopClose = bStopClose;
	gGlobal.StopAllServices(BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::RestartAllServices(LPCPEER pPeer, RPCContext &context, 
												  const Boolean &bErr)
{
	gGlobal.RestartAllServices(BOOLEAN_TRUE, bErr);

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::CheckService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	ServiceExMap::iterator it = gGlobal.m_srvExMap.find(serviceId);

	if (it != gGlobal.m_srvExMap.end())
		ManagerService_Proxy::ForwardStatus(pPeer, context, serviceId, SS_RUNNING, it->second.statuses[SS_RUNNING]);

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::StartService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	gGlobal.StartService(serviceId, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::StopService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	gGlobal.StopService(serviceId, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::RestartService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	gGlobal.RestartService(serviceId, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::ErrorArchive(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	gGlobal.ErrorArchive(serviceId, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::RegisterSimClient(LPCPEER pPeer, RPCContext &context, 
												 const String &prog, const String &title, 
												 const String &workDir, const String &cmd)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("RegisterSimClient: pPeer=%d, cmd=%s"), pPeer, cmd.c_str());

	ProgInfo &info = gGlobal.m_simClientMap[pPeer];

	info.prog = prog;
	info.title = title;
	info.workDir = workDir;
	info.cmd = cmd;

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::UnregisterSimClient(LPCPEER pPeer, RPCContext &context)
{
	SimClientMap::iterator it = gGlobal.m_simClientMap.find(pPeer);

	if (it != gGlobal.m_simClientMap.end()) {
		ProgInfo& info = it->second;

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("UnregisterSimClient: pPeer=%d, title=%s, cmd=%s"),
			pPeer, info.title.c_str(), info.cmd.c_str());
		gGlobal.m_simClientMap.erase(it);
	}

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::StartProcess(LPCPEER pPeer, RPCContext &context, 
											  const UInt32 &time, const String &prog, 
											  const String &title, const String &workDir, 
											  const String &cmd)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("StartProcess: pPeer=%d, time=%d, title=%s, cmd=%s"), 
		pPeer, time, title.c_str(), cmd.c_str());

	Sleep(time);			// sleep time before restart process
	gGlobal.CreateProcess(prog.c_str(), title.c_str(), (LPTSTR) cmd.c_str(), workDir.c_str());

	return RPC_RESULT_OK;
}

RPCResult ManagerService_Stub::ShutdownAt(LPCPEER pPeer, RPCContext &context, const UInt32 &downTime)
{
	gGlobal.m_bWillStop = BOOLEAN_TRUE;
	gGlobal.m_schedTime = downTime;
	gGlobal.m_schedCmd[0] = _T('\0');

	// forward shutdown time to all services
	for (ServiceExMap::iterator it = gGlobal.m_srvExMap.begin();
		it != gGlobal.m_srvExMap.end(); ++it)
	{
		ServiceEx &service = it->second;
		if (service.bLocal)
			ServiceStatus_Stub::CB_ShutdownAt(service.pPeer, context, downTime);
	}
	return RPC_RESULT_OK;
}
