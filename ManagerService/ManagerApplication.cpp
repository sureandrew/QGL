//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "ManagerService/ManagerApplication.h"
//-- Library
#include "ManagerService/Configure.h"
#include "ManagerService/Global.h"
#include "Stub/Service/ServiceStatus_stub.h"
#include "Proxy/Service/ManagerService_proxy.h"
#include "Stub/Service/ManagerService_stub.h"
#include <Iphlpapi.h>
#include "dboServiceStatus.h"

#define UPDATE_INTERVAL		10					// update loop interval 10ms
#define CONNECT_INTERVAL	60000				// connect interval 1 min
#define CHECK_INTERVAL		1000				// checking interval 1 second

void ManagerApplication::ConnectAfter(UInt32 time)
{
	m_connTime = time;
}

Boolean ManagerApplication::OnCreate(VOID)
{
	if (!gGlobal.Initialize())
		return (BOOLEAN_FALSE);

	if (!StartService(gConfigure))
		return (BOOLEAN_FALSE);

	ConnectServices();
	ConnectManagers();
	gGlobal.CheckAutoStart();

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("memory status, %s"), 
		MemoryManager::GetInstance().GetStatus().c_str());

	return (BOOLEAN_TRUE);
}

VOID ManagerApplication::OnDispose(VOID)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("memory status, %s"), 
		MemoryManager::GetInstance().GetStatus().c_str());

	gGlobal.ReleaseOLEDB();

	StopService(gConfigure);

	// disconnect all managers
	LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();
	for (ManagerMap::iterator it = gGlobal.m_managerMap.begin(); 
		it != gGlobal.m_managerMap.end(); ++it)
	{
		Service& service = it->second;
		if (service.bConnected)
		{
			rpcmgr.GetModel().CloseSocket(service.pPeer->GetSocketID());
			SafeRelease(service.pPeer);
		}
	}

	// disconnect all other connected peers
	for (ServicePeerMap::iterator it = gGlobal.m_srvPeerMap.begin(); 
		it != gGlobal.m_srvPeerMap.end(); ++it)
	{
		if (it->first && it->second == NULL)
		{
			LPCPEER pPeer = it->first;
			rpcmgr.GetModel().CloseSocket(pPeer->GetSocketID());
			SafeRelease(pPeer);
		}
	}

	Sleep(100);

	LocalRPCManager::GetInstance().GetModel().Shutdown();

	SessionManager::GetInstance().ClearAll();
	gGlobal.Uninitialize();
	LocalRPCManager::GetInstance().DeleteAll();
	Reuben::Simulation::ObjectInstance::GetInstance().DeleteAll();
	ProcedureManager::DestroyInstance();
	LocalRPCManager::DestroyInstance();
	SessionManager::DestroyInstance();

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("ManagerApplication::OnDispose : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());
}

VOID ManagerApplication::OnUpdate(UInt32 uTimeDelta)
{
	if (uTimeDelta > 1000)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("TimeDelta %d"), uTimeDelta);
	}

	// process RPC message
	LocalRPCManager::GetInstance().OnUpdate(uTimeDelta);
	Reuben::Simulation::ScheduleInstance::GetInstance().OnUpdate(uTimeDelta);

	// connect and check services and managers status
	if (m_connTime != 0)
	{
		if (m_connTime <= uTimeDelta)
		{
			m_connTime = 0;
			ConnectServices();
			ConnectManagers();
		}
		else
		{
			m_connTime -= uTimeDelta;
		}
	}

	m_checkTime += uTimeDelta;
	if (m_checkTime >= CHECK_INTERVAL)
	{
		m_checkTime -= CHECK_INTERVAL;
		if (gGlobal.m_bCheck)
			ConnectStarting();

		// abnormal error happen, archive log
		if (gGlobal.m_errServiceId != 0)
		{
			gGlobal.ErrorArchive(gGlobal.m_errServiceId, BOOLEAN_FALSE);

			if (IsOption(OPT_AUTO_RESTART))
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("ManagerService Error Restarting after 10 second ..."));
				Sleep(10000);
				gGlobal.RestartAllServices(BOOLEAN_FALSE, BOOLEAN_TRUE);
			}
			gGlobal.m_errServiceId = 0;
		}
	}

	// execute console command
	if (!gGlobal.m_commandList.empty())
		gGlobal.ExecuteCommand();

	// check scheduling time and counting down
	if (gGlobal.m_schedTime > 0)
		gGlobal.CheckSchedule(uTimeDelta);

	if (m_manage_interval > gConfigure.manage_interval)
	{
		DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("ManagerApplication::OnUpdate : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());
		m_manage_interval = 0;
	}
	m_manage_interval += uTimeDelta;

	SleepEx(UPDATE_INTERVAL, TRUE);

	if (gGlobal.m_bExit)
		Terminate(0);
}

VOID ManagerApplication::OnAccept(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID ManagerApplication::OnFull(CONST Listener* listener)
{
}

VOID ManagerApplication::OnOpen(CONST Listener* listener)
{
}

VOID ManagerApplication::OnOpen(CONST Listener* listener, CONST Peer* pPeer)
{
	gGlobal.AddSrvPeer(pPeer, NULL);
}

VOID ManagerApplication::OnClose(CONST Listener* listener)
{
}

VOID ManagerApplication::OnClose(CONST Listener* listener, CONST Peer* pPeer)
{
	Service* pService = gGlobal.RemoveSrvPeer(pPeer);
	if (pService)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("%s Service disconnected, ip %s"), 
			pService->strName.c_str(),
			Reuben::Network::IPToString(pPeer->GetAddressRemote().GetIP()).c_str());
		pService->bConnected = BOOLEAN_FALSE;
		SafeRelease(pService->pPeer);
		if (pService->type == SERVICE_MANAGER)
		{
			for (ServiceExMap::iterator it = gGlobal.m_srvExMap.begin(); 
				it != gGlobal.m_srvExMap.end(); ++it)
			{
				ServiceEx& service = it->second;
				if (service.address.GetIP() == pService->address.GetIP())
					service.statuses[SS_RUNNING] = SRS_UNKNOWN;
			}
			if (gGlobal.m_bStopClose)
				gGlobal.CheckAllStopped();
		}
		else
		{
			bool updated = false;
			ServiceEx *pServiceEx = (ServiceEx*) pService;
			if (pServiceEx->statuses[SS_RUNNING] == SRS_STOPPING)
			{
				TRACE_INFODTL_4(GLOBAL_LOGGER, _F("OnClose %s service, serviceId %d, status %d, state %d"),
					pServiceEx->strName.c_str(), pServiceEx->serviceId, SS_RUNNING, SRS_STOPPED);
				pServiceEx->statuses[SS_RUNNING] = SRS_STOPPED;
				if (gGlobal.m_pdboServiceStatus)
				{
					HRESULT hr = gGlobal.m_pdboServiceStatus->Update(pServiceEx->serviceId, SS_RUNNING, SRS_STOPPED);
					if (DB_FAILED(hr))
					{
						TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("db failed, hr %d, service_id %d, status %s, state %s"),
							hr, pService->serviceId, GetServiceStatusesString(SS_RUNNING), GetServiceRunningStatusString(SRS_STOPPED));
					}
				}
				updated = true;
			}
			else if (pServiceEx->statuses[SS_RUNNING] == SRS_STARTED)
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("OnClose error : abnormal shutdown, %s service, serviceId %d"), 
					pService->strName.c_str(), pService->serviceId);
				pServiceEx->statuses[SS_RUNNING] = SRS_UNKNOWN;
				if (gGlobal.m_pdboServiceStatus)
				{
					HRESULT hr = gGlobal.m_pdboServiceStatus->Update(pServiceEx->serviceId, SS_RUNNING, SRS_UNKNOWN);
					if (DB_FAILED(hr))
					{
						TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("db failed, hr %d, service_id %d, status %s, state %s"),
							hr, pService->serviceId, GetServiceStatusesString(SS_RUNNING), GetServiceRunningStatusString(SRS_UNKNOWN));
					}
				}
				updated = true;
				// mark serviceId for abnormal shutdown
				if (gGlobal.m_bAllStarted && gGlobal.m_errServiceId == 0)
				{
					gGlobal.m_errServiceId = pService->serviceId;
					m_checkTime = CHECK_INTERVAL;		// immediately check
				}
			}
			if (gGlobal.m_bStopClose)
				gGlobal.CheckAllStopped();
			if (updated)
			{
				gGlobal.m_bAllStarted = BOOLEAN_FALSE;
				// forward updated status to all managers
				for (ManagerMap::iterator it = gGlobal.m_managerMap.begin();
					it != gGlobal.m_managerMap.end(); ++it)
				{
					Service& service = it->second;
					if (service.pPeer)
						ManagerService_Proxy::ForwardStatus(service.pPeer, RPCContext(), 
							pServiceEx->serviceId, SS_RUNNING, pServiceEx->statuses[SS_RUNNING]);
				}
			}
			gGlobal.KickWaiting(pServiceEx->serviceId, pServiceEx->statuses[SS_RUNNING]);
		}
	}
	else if (gGlobal.m_bAllStarted)
	{
		// if all services started and registered simclient shutdown abnormally, restart it
		SimClientMap::iterator it = gGlobal.m_simClientMap.find(pPeer);
		if (it != gGlobal.m_simClientMap.end())
		{
			ProgInfo& info = it->second;

			TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("SimClient Warning: shutdown abnormally and restart in 5s, pPeer %d, title %s, args %s"), 
				pPeer, info.title.c_str(), info.cmd.c_str());

			Sleep(5000);			// sleep 5 second for simclient restart
			gGlobal.CreateProcess(info.prog.c_str(), info.title.c_str(), 
				(LPTSTR) info.cmd.c_str(), info.workDir.c_str());
		}
	}
}

void GetIpAddress(UInt32Vector &ipList)
{
	PMIB_IPADDRTABLE pIpAddrTable = (PMIB_IPADDRTABLE) SafeAllocate(Byte, 1024);
	UInt32 loopback = Reuben::Network::StringToIP(_T("127.0.0.1"));
	DWORD dwSize = 1024;

	// get all ip address except loopback address
	if (GetIpAddrTable(pIpAddrTable, &dwSize, 0) == NO_ERROR)
	{
		for (DWORD i = 0; i < pIpAddrTable->dwNumEntries; i++)
		{
			if (pIpAddrTable->table[i].dwAddr != loopback)
				ipList.push_back(pIpAddrTable->table[i].dwAddr);
		}
	}
	SafeDeallocate(pIpAddrTable);
}

Boolean ManagerApplication::StartService(ServiceConfigure& config)
{
	UInt32Vector ipList;
	LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Starting %s Service"), config.strServiceName.c_str());

	bool found = false;
	UInt32 loopback = Reuben::Network::StringToIP(_T("127.0.0.1"));
	bool is_master = false;

	GetIpAddress(ipList);						// get all local ip address
	config.ip = loopback;						// default set loopback ip
	for (ServiceMap::iterator it = config.services.begin(); it != config.services.end(); ++it)
	{
		Service& service = it->second;
		ServiceEx& serviceEx = gGlobal.m_srvExMap[service.serviceId];

		serviceEx.Copy(service);
		if (!found)
		{
			// find suitable ip address for this manager
			for (UInt32Vector::iterator it2 = ipList.begin(); it2 != ipList.end(); ++it2)
			{
				Reuben::Network::IP ip = *it2;
				if (service.address.GetIP() == ip)
				{
					config.ip = ip;
					found = true;
					break;
				}
			}
		}

		// set local service flag
		serviceEx.bLocal = (service.address.GetIP() == config.ip || 
							service.address.GetIP() == loopback);
		serviceEx.statuses[SS_RUNNING] = SRS_UNKNOWN;

		// get the service dependence map
		gGlobal.ReadDependList(&serviceEx, gConfigure.config);

		// set master if object service in local machine
		if (serviceEx.bLocal && serviceEx.type == SERVICE_OBJECT)
			is_master = true;

		// get manager service from service IP
		if (service.address.GetIP() != config.ip &&
			gGlobal.m_managerMap.find(service.address.GetIP()) == gGlobal.m_managerMap.end())
		{
			Service& mansrv = gGlobal.m_managerMap[service.address.GetIP()];
			
			mansrv.strName = config.strServiceName;
			mansrv.type = config.type;
			mansrv.serviceId = config.serviceId;
			mansrv.protocol = Reuben::Network::PROTOCOL_TCP;
			mansrv.bConnected = BOOLEAN_FALSE;
			mansrv.pPeer = NULL;
			mansrv.address.SetIP(service.address.GetIP());
			mansrv.address.SetPort(config.port);
		}
	}

	// set the first ip address if not found
	if (!found && !ipList.empty())
		config.ip = ipList.front();

	rpcmgr.SetServiceId(config.serviceId);
	rpcmgr.GetModel().SetSocketHandler(this);
	// listen to all interface include loopback
	m_pListener = rpcmgr.GetModel().Listen(Address(Reuben::Network::IP_ANY, config.port), 
		Reuben::Network::PROTOCOL_TCP, config.maxInQueue, config.maxConnection, BOOLEAN_TRUE);

	if (m_pListener == NULL)
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("StartService Error: %s Service fail to listen at IP %s, port %d"), 
			config.strServiceName.c_str(), Reuben::Network::IPToString(config.ip).c_str(),
			Reuben::Network::NetToHost(config.port));
		return (BOOLEAN_FALSE);
	}

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("%s Service Started at IP %s port %d"), 
		config.strServiceName.c_str(), Reuben::Network::IPToString(config.ip).c_str(),
		Reuben::Network::NetToHost(config.port));

	if (is_master)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("Master Manager Service Initialize..."));
		if (!gGlobal.ConnectDatabase())
			return (BOOLEAN_FALSE);
		if (gGlobal.m_pdboServiceStatus)
		{
			HRESULT hr = gGlobal.m_pdboServiceStatus->Delete();
			if (DB_FAILED(hr))
			{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("db failed on delete service statuses, hr %d"), hr);
			}
		}
		TRACE_INFODTL(GLOBAL_LOGGER, _F("done"));
	}

	return (BOOLEAN_TRUE);
}

Boolean ManagerApplication::ConnectServices()
{
	LocalRPCManager& rpcmgr = LocalRPCManager::GetInstance();
	bool bTry = false;

	// connect to services which is same IP to manager
	for (ServiceExMap::iterator it = gGlobal.m_srvExMap.begin();
		it != gGlobal.m_srvExMap.end(); ++it)
	{
		ServiceEx& service = it->second;
		if (gGlobal.m_bExit)
			break;
		if (!service.bLocal)
			continue;
		if (!service.bConnected)
		{
			if (!bTry)
			{
				bTry = true;
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Try to connect services ... "));
			}
			service.pPeer = rpcmgr.GetModel().Connect(service.address, 
				service.protocol, service.bTrust);
			if (service.pPeer)
			{
				service.bConnected = BOOLEAN_TRUE;
				gGlobal.AddSrvPeer(service.pPeer, &service);
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Local %s Service Connected SocketID=%d"), 
					service.strName.c_str(), service.pPeer->GetSocketID());
			}
		}

		// get service status
		if (service.bConnected)
			ServiceStatus_Stub::CB_CheckService(service.pPeer, gGlobal.m_context);
	}

	if (bTry)
		TRACE_INFODTL(GLOBAL_LOGGER, _F("done"));

	return (BOOLEAN_TRUE);
}

Boolean ManagerApplication::ConnectManagers()
{
	LocalRPCManager& rpcmgr = LocalRPCManager::GetInstance();
	bool bTry = false;

	// connect to services which is same IP to manager
	for (ManagerMap::iterator it = gGlobal.m_managerMap.begin(); 
		it != gGlobal.m_managerMap.end(); it++)
	{
		Service& service = it->second;
		if (gGlobal.m_bExit)
			break;
		if (!service.bConnected)
		{
			if (!bTry)
			{
				bTry = true;
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Try to connect manager services ... "));
			}
			service.pPeer = rpcmgr.GetModel().Connect(service.address, 
				service.protocol, service.bTrust);
			if (service.pPeer)
			{
				service.bConnected = BOOLEAN_TRUE;
				gGlobal.AddSrvPeer(service.pPeer, &service);
				TRACE_INFODTL_3(GLOBAL_LOGGER, _F("%s Service Connected, socketId %d, IP %s"), 
					service.strName.c_str(), service.pPeer->GetSocketID(),
					Reuben::Network::IPToString(service.address.GetIP()).c_str());
			}
		}

		// get all services status
		if (service.bConnected && service.pPeer)
		{
			ManagerService_Proxy::RegisterManager(service.pPeer, gGlobal.m_context);
			ManagerService_Proxy::CheckAllServices(service.pPeer, gGlobal.m_context);
			ManagerService_Stub::CheckAllServices(service.pPeer, gGlobal.m_context);
		}
	}
	if (bTry)
		TRACE_INFODTL(GLOBAL_LOGGER, _F("done"));

	return (BOOLEAN_TRUE);
}

void ManagerApplication::ConnectStarting()
{
	Boolean stillCheck = BOOLEAN_FALSE;
	LocalRPCManager& rpcmgr = LocalRPCManager::GetInstance();

	// connect to services which is same IP to manager
	for (ServiceExMap::iterator it = gGlobal.m_srvExMap.begin();
		it != gGlobal.m_srvExMap.end(); ++it)
	{
		ServiceEx& service = it->second;
		if (!service.bLocal || service.statuses[SS_RUNNING] != SRS_STARTING)
			continue;

		stillCheck = BOOLEAN_TRUE;
		if (!service.bConnected)
		{
			service.pPeer = rpcmgr.GetModel().Connect(service.address, 
				service.protocol, service.bTrust);
			if (service.pPeer)
			{
				service.bConnected = BOOLEAN_TRUE;
				gGlobal.AddSrvPeer(service.pPeer, &service);
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Local %s Service Connected SocketID=%d"), 
					service.strName.c_str(), service.pPeer->GetSocketID());
				ServiceStatus_Stub::CB_CheckService(service.pPeer, gGlobal.m_context);
			}
		}
	}

	// no more checking need
	if (!stillCheck)
		gGlobal.m_bCheck = BOOLEAN_FALSE;
}
