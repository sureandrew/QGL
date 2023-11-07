//-- Common
#include "Common.h"
//-- Self
#include "Configure.h"
#include "Global.h"
#include "ProxyApplication.h"
#include "ProxyManager.h"
#include "ApolloSvrLib.h"

#define UPDATE_INTERVAL 100
#define CONNECT_TIMEOUT 30000

BOOL _stdcall ApolloCallBack(DWORD dwIndex, PVOID pInfo, int iLen);

Boolean ProxyApplication::OnCreate(VOID)
{
	if (!gGlobal.Initialize())
		return (BOOLEAN_FALSE);

	// set direct pointer to service peer	
	for (ServiceMap::iterator it = gConfigure.services.begin(); it != gConfigure.services.end(); ++it)
	{
		switch (it->second.type)
		{
		case SERVICE_WORLD:
			if (gGlobal.pWorldService == NULL)
				gGlobal.pWorldService = &it->second;
			break;

		case SERVICE_MESSAGE:
			if (gGlobal.pMessageService == NULL)
				gGlobal.pMessageService = &it->second;
			break;
		}
	}
	
	ConnectServices();

	// setup apollo
	Apollo::SetApolloCallBackSRV(ApolloCallBack);
	Apollo::SetApolloSeedSRV(gConfigure.apolloInteval);

	if (!StartService())
		return (BOOLEAN_FALSE);

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("ProxyApplication::OnCreate : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	return (BOOLEAN_TRUE);
}

VOID ProxyApplication::OnDispose(VOID)
{
	// tell manager service this service termination normally
	if (gGlobal.pManagerPeer)
	{
		m_ProxyManager.UpdateStatus(gGlobal.pManagerPeer, RPCContext(), SS_RUNNING, SRS_STOPPED);
		SafeRelease(gGlobal.pManagerPeer);
		Sleep(10);
	}

	StopService();
	m_ProcessManager.Detach(&m_ProxyManager);
	m_ProxyManager.GetModel().Shutdown();

	gGlobal.Uninitialize();

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("ProxyApplication::OnDispose : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());
}

VOID ProxyApplication::OnFatalError(Exception& ex)
{
}

Boolean ProxyApplication::StartService(VOID)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Starting %s Service"), gConfigure.strServiceName.c_str());

	gGlobal.pWanListener = m_ProxyManager.GetModel().Listen(Address(gConfigure.publicIp, gConfigure.publicPort), Reuben::Network::PROTOCOL_TCP, gConfigure.publicMaxInQueue, gConfigure.publicMaxConnection, BOOLEAN_FALSE);
	if (!gGlobal.pWanListener)
		return (BOOLEAN_FALSE);

	gGlobal.pLanListener = m_ProxyManager.GetModel().Listen(Address(gConfigure.ip, gConfigure.port), Reuben::Network::PROTOCOL_TCP, gConfigure.maxInQueue, gConfigure.maxConnection, BOOLEAN_TRUE);
	if (!gGlobal.pLanListener)
		return (BOOLEAN_FALSE);

	m_ProcessManager.Attach(&m_ProxyManager);
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Proxy Manager Attached to Process Manager"));

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("%s Service Started"), gConfigure.strServiceName.c_str());

	return (BOOLEAN_TRUE);
}

Boolean ProxyApplication::ConnectServices(VOID)
{
	typedef StlVector<Service*, ManagedAllocator<Service*> > ServiceList;
	ServiceList tryList;					// try to connect list

	for (ServiceMap::iterator iter = gConfigure.services.begin(); 
		iter != gConfigure.services.end(); iter++)
	{
		tryList.push_back(&iter->second);
	}

	//-- Connect to Services
	DWORD startTime = ::timeGetTime();		// connect start time
	while (::timeGetTime() - startTime < CONNECT_TIMEOUT && !tryList.empty())
	{
		for (ServiceList::iterator iter = tryList.begin(); iter != tryList.end();)
		{
			Service* service = *iter;
			if (service->pPeer) continue;

			service->pPeer = m_ProxyManager.GetModel().Connect(service->address, service->protocol, service->bTrust);
			if (service->pPeer)
			{
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("%s Service Connected"), service->strName.c_str());
				service->bConnected = BOOLEAN_TRUE;
				iter = tryList.erase(iter);
			}
			else
			{
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Connecting %s Service ..."), service->strName.c_str());
				iter++;
			}
		}
		Sleep(200);
	}

	//-- Fail to Connect to Services finally
	for (ServiceList::iterator iter = tryList.begin(); iter != tryList.end(); iter++)
	{
		Service* service = *iter;
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Cannot connect to %s Service finally."), service->strName.c_str());
	}

	//-- Register Proxy
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Registering Proxy in all connected service ..."));
	for (ServiceMap::iterator iter = gConfigure.services.begin(); 
		iter != gConfigure.services.end(); iter++)
	{
		Service& service = iter->second;
		if (service.bConnected && m_ProxyManager.RegisterProxy(service.pPeer, 
			ProxyManager::GetRegisterProxyRPCID(service.type)))
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Register Proxy in %s Service, SocketID=%d"),
				service.strName.c_str(), service.pPeer->GetSocketID());
		}
	}

	return (BOOLEAN_TRUE);
}

VOID ProxyApplication::StopService(VOID)
{
	m_ProxyManager.GetModel().CloseSocket(gGlobal.pWanListener->GetSocketID());
	m_ProxyManager.GetModel().CloseSocket(gGlobal.pLanListener->GetSocketID());
	SafeRelease(gGlobal.pWanListener);
	SafeRelease(gGlobal.pLanListener);

	for (ServiceMap::iterator iter = gConfigure.services.begin(); 
		iter != gConfigure.services.end(); iter++)
	{
		Service* service = &iter->second;
		if (service->bConnected)
		{
			m_ProxyManager.GetModel().CloseSocket(service->pPeer->GetSocketID());
			SafeRelease(service->pPeer);
		}
	}
}

VOID ProxyApplication::OnUpdate(UInt32 uTimeDelta)
{
	if (uTimeDelta > 1000)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("TimeDelta %d"), uTimeDelta);
	}

	m_ProcessManager.UpdateProcesses(uTimeDelta);

	SleepEx(UPDATE_INTERVAL, TRUE);

	if (gGlobal.bExit)
		Terminate(0);
}

//apollo callbacks
BOOL _stdcall ApolloCallBack(DWORD dwIndex, PVOID pInfo, int iLen)
{
	return ProxyApplication::GetInstance().GetManager().ApolloCallBack(dwIndex, pInfo, iLen);
}
