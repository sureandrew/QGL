//-- Common
#include "Common.h"
//-- Self
#include "Stub/Service/ServiceStatus_stub.h"
//-- Library
#include "Proxy/Service/ManagerService_proxy.h"
#include "ManagerService/Global.h"
#include "dboServiceStatus.h"

RPCResult ServiceStatus_Stub::UpdateStatus(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt32 &status, const UInt32 &state)
{
	ServiceEx *pService = gGlobal.GetServiceEx(serviceId);

	// serviceId not found
	if (pService == NULL)
		return RPC_RESULT_OK;

	// no status changed
	if (pService->statuses.find(status) != pService->statuses.end() &&
		pService->statuses[status] == state)
		return RPC_RESULT_OK;

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("UpdateStatus: %s service, serviceId %d, status %d, state %d"), 
		pService->strName.c_str(), serviceId, status, state);
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

	// forward updated status to all managers
	for (ManagerMap::iterator it = gGlobal.m_managerMap.begin(); it != gGlobal.m_managerMap.end(); ++it)
	{
		Service& service = it->second;
		if (service.pPeer)
			ManagerService_Proxy::ForwardStatus(service.pPeer, context, serviceId, status, state);
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
