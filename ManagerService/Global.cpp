//-- Common
#include "Common.h"
//-- Self
#include "ManagerService/Global.h"
//-- Library
#include "Stub/Service/ServiceStatus_stub.h"
#include "Proxy/Service/ManagerService_proxy.h"
#include "ManagerService/Configure.h"
#include "ManagerService/ManagerApplication.h"
#include "dboServiceStatus.h"
#include "dboExecuteCheat.h"
#include "ExecuteCheat.h"
#include "DBPWDecrypt.h"

#define ARCHIVE_PATH			_T("./Archive")
#define ERR_WAIT_TIME			5000				// maximum wait error service shutdown time
#define MINUTE_TIME				60000
#define SECOND_TIME				1000

#define ExecuteCheat_CLASSID 54000

static PCSTRING gRunningStatusString[] =
{
	_T("UNKNOWN"),
	_T("STARTED"),
	_T("STOPPED"),
	_T("STARTING"),
	_T("STOPPING"),
	_T("WAITSTART"),
	_T("WAITSTOP"),
	_T("WAITRESTART"),
	_T("ERROR"),
	_T("[NONE]"),
};

const TCHAR* GetRunningStatusStr(UInt32 state)
{
	if (state < SRS_MAX)
		return gRunningStatusString[state];
	else
		return gRunningStatusString[SRS_MAX];
}

Boolean Global::Initialize(VOID)
{
	ReadOption(gConfigure.strOption.c_str());
	m_pdboExecuteCheat = NULL;
	m_pExecuteCheat = (CExecuteCheat*)Reuben::Simulation::CreateObject(ExecuteCheat_CLASSID);

	return (BOOLEAN_TRUE);
}

VOID Global::Uninitialize(VOID)
{
	ThreadGuard guard(m_key);

	m_srvExMap.clear();
	m_managerMap.clear();
	m_srvPeerMap.clear();
	m_commandList.clear();
	m_bExit = BOOLEAN_TRUE;
	m_pdboExecuteCheat = NULL;
	Reuben::Simulation::DeleteObject(m_pExecuteCheat);
}

Boolean Global::ConnectDatabase()
{
	HRESULT hr;

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Begin to Connect Databasae"));

	if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("oledb com initialization failed, hr 0x%08X"), hr);
		return BOOLEAN_FALSE;
	}

	// OLEDB object MUST create using 'new' instead of our ManagedAllocator
	const Database& db = gConfigure.dbManage;
	if (m_pManageDb == NULL)
		m_pManageDb = SafeCreateObject(CDboSource);
#ifdef CONNECT_STR
	String cstr1;
	cstr1.Format(CONNECT_STR, db.strServer.c_str(), db.strDatabase.c_str(), 
		db.strLogin.c_str(), DBPWDecrypt(db.strPassword).c_str());
	if (DB_FAILED(hr = m_pManageDb->Open(cstr1.c_str())))
#else
	if (DB_FAILED(hr = m_pManageDb->Open(db.strServer.c_str(), db.strDatabase.c_str(), 
		db.strLogin.c_str(), DBPWDecrypt(db.strPassword).c_str())))
#endif	// CONNECT_STR
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("cannot connect to manage database, hr 0x%08X"), hr);
		m_pManageDb->ShowError(hr);
		ReleaseOLEDB();
		return BOOLEAN_FALSE;
	}

	m_pdboServiceStatus = SafeCreateObject(CdboServiceStatus);
	m_pdboServiceStatus->SetDataSource(m_pManageDb);

	m_pdboExecuteCheat = SafeCreateObject(CdboExecuteCheat);
	m_pdboExecuteCheat->SetDataSource(m_pManageDb);

	m_pExecuteCheat->Start();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Connect Database Success"));

	return (BOOLEAN_TRUE);
}

Boolean Global::ReleaseOLEDB()
{
	// release COM objects
	if (m_pdboServiceStatus)
	{
		m_pdboServiceStatus->Close();
		SafeDeleteObject(m_pdboServiceStatus);		// don't use SafeDelete for COM object
	}
	if (m_pdboExecuteCheat)
	{
		m_pdboExecuteCheat->Close();
		SafeDeleteObject(m_pdboExecuteCheat);		// don't use SafeDelete for COM object
		m_pdboExecuteCheat = NULL;
	}

	if (m_pManageDb)
	{
		m_pManageDb->Close();
		SafeDeleteObject(m_pManageDb);
	}

	CoUninitialize();

	return (BOOLEAN_TRUE);
}

void Global::RegisterManager(LPCPEER pPeer)
{
	if (pPeer == NULL) return;

	Service& service = m_managerMap[pPeer->GetAddressRemote().GetIP()];

	service.type = SERVICE_MANAGER;
	service.strName = _T("Manager");
	service.serviceId = gConfigure.serviceId;
	service.protocol = Reuben::Network::PROTOCOL_TCP;
	service.address = pPeer->GetAddressRemote();
	service.bConnected = BOOLEAN_TRUE;
	service.pPeer = pPeer;
	service.bTrust = BOOLEAN_TRUE;
	AddSrvPeer(pPeer, &service);

	CheckAutoStart();
}

void Global::UnregisterManager(LPCPEER pPeer)
{
	if (pPeer == NULL) return;

	m_managerMap.erase(pPeer->GetAddressRemote().GetIP());
}

void Global::AddSrvPeer(LPCPEER pPeer, Service* pService)
{
	ThreadGuard guard(m_key);
	ServicePeerMap::iterator it = m_srvPeerMap.find(pPeer);
	if (it == m_srvPeerMap.end() || it->second == NULL)
		m_srvPeerMap[pPeer] = pService;
}

Service* Global::RemoveSrvPeer(LPCPEER pPeer)
{
	ThreadGuard guard(m_key);
	Service *pService = NULL;
	ServicePeerMap::iterator it = m_srvPeerMap.find(pPeer);
	if (it != m_srvPeerMap.end()) {
		pService = it->second;
		m_srvPeerMap.erase(it);
	}

	return pService;
}

ServiceEx* Global::GetServiceEx(const ServiceID serviceId)
{
	ServiceExMap::iterator it = m_srvExMap.find(serviceId);
	if (it != m_srvExMap.end())
		return &it->second;
	else
		return NULL;
}

ServiceEx* Global::GetServiceEx(const String& srvName)
{
	ServiceEx* pService = NULL;
	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		if (_tcsicmp(it->second.strName.c_str(), srvName.c_str()) == 0) {
			pService = &it->second;
			break;
		}
	}

	return pService;
}

ServiceEx* Global::GetServiceEx(const ServiceType type)
{
	ServiceEx* pService = NULL;
	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		if (it->second.type == type)
		{
			pService = &it->second;
			break;
		}
	}

	return pService;
}

void Global::ReadDependList(ServiceEx* pService, Reuben::Utility::ConfigFile &config)
{
	if (pService == NULL)
		return;

	ServiceID serviceId;
	String strName;
	String strServices;
	PSTRING pToken;
	PSTRING pDelim = _T(", \t");

	// get the service dependence map
	if (pService->strName != ServiceTypeToString(pService->type))
	{
		// read "Services" in current service section and common service section
		String noneStr = _T("_NONE_");
		strServices = config.GetString(pService->strName.c_str(), _T("Services"), 
			noneStr.c_str());
		if (strServices == noneStr)		// not found, try find in common section
			strServices = config.GetString(ServiceTypeToString(pService->type), 
				_T("Services"), _T(""));
	}
	else	// read "Services" only in current service
		strServices = config.GetString(pService->strName.c_str(), _T("Services"), _T(""));

		
	pToken = ::_tcstok(strServices.GetBuffer(MAX_SIZE), pDelim);
	while (pToken != NULL)
	{
		strName = pToken;
		strName.Trim();
		serviceId = (ServiceID) gConfigure.config.GetInt(strName.c_str(),
			_T("ServiceID"), 0);
		if (serviceId > 0)
			pService->dependList.push_back(serviceId);
		pToken = ::_tcstok(NULL, pDelim);
	}
}

void Global::ReadConfig(PCSTRING configFile)
{
	Configure newConfig;

	if (!newConfig.Initialize(configFile, gConfigure.strServiceName.c_str()))
	{
		newConfig.Uninitialize();
		return;
	}

	LocalRPCManager& rpcmgr = LocalRPCManager::GetInstance();
	UInt32 loopback = Reuben::Network::StringToIP(_T("127.0.0.1"));
	ServiceEx* pServiceEx;

	// disconnect all services and managers first
	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx& service = it->second;
		if (service.pPeer)
			rpcmgr.GetModel().CloseSocket(service.pPeer->GetSocketID());
		service.statuses[SS_RUNNING] = SRS_UNKNOWN;
		service.next_running_status = SRS_UNKNOWN;
		service.bLocal = BOOLEAN_FALSE;
		service.dependList.clear();
	}

	for (ManagerMap::iterator it = m_managerMap.begin(); it != m_managerMap.end(); ++it)
	{
		Service& service = it->second;
		if (service.pPeer)
			rpcmgr.GetModel().CloseSocket(service.pPeer->GetSocketID());
	}

	gConfigure.services.clear();
	gGlobal.m_bAllStarted = BOOLEAN_FALSE;

	// re-add services and manager services
	for (ServiceMap::iterator it = newConfig.services.begin(); it != newConfig.services.end(); ++it)
	{
		Service& service = it->second;

		pServiceEx = &gGlobal.m_srvExMap[service.serviceId];
		pServiceEx->Copy(service);
		pServiceEx->statuses[SS_RUNNING] = SRS_UNKNOWN;

		// get the service dependence map
		ReadDependList(pServiceEx, newConfig.config);
		gConfigure.services[service.serviceId] = service;

		// set local service flag
		pServiceEx->bLocal = (service.address.GetIP() == gConfigure.ip || 
							service.address.GetIP() == loopback);

		// get manager service from service IP
		if (service.address.GetIP() != gConfigure.ip && service.address.GetIP() != loopback &&
			gGlobal.m_managerMap.find(service.address.GetIP()) == gGlobal.m_managerMap.end())
		{
			Service& mansrv = gGlobal.m_managerMap[service.address.GetIP()];
			
			mansrv.strName = gConfigure.strServiceName;
			mansrv.type = gConfigure.type;
			mansrv.serviceId = gConfigure.serviceId;
			mansrv.protocol = Reuben::Network::PROTOCOL_TCP;
			mansrv.bConnected = BOOLEAN_FALSE;
			SafeRelease(mansrv.pPeer);
			mansrv.address.SetIP(service.address.GetIP());
			mansrv.address.SetPort(gConfigure.port);
		}
	}

	gConfigure = newConfig;
	ReadOption(gConfigure.strOption.c_str());

	ManagerApplication::GetInstance().ConnectAfter(100);
}

void Global::AllReadConfig(PCSTRING configFile)
{
	ReadConfig(configFile);
	for (ManagerMap::iterator it = m_managerMap.begin(); it != m_managerMap.end(); ++it)
	{
		Service& service = it->second;
		if (service.bConnected && service.pPeer)
			ManagerService_Proxy::LoadConfig(service.pPeer, m_context, configFile);
	}
}

void Global::ReadOption(const TCHAR *optLine)
{
	if (optLine == NULL) return;

	TCHAR optstr[1024] = _T("");
	TCHAR *delim = _T("|, \t");
	TCHAR *token;
	Boolean enable;

	_tcscpy(optstr, optLine);
	token = _tcstok(optstr, delim);
	while (token)
	{
		if (token[0] == _T('-')) {
			++token;
			enable = false;
		} else if (token[0] == _T('+')) {
			++token;
			enable = true;
		} else {
			enable = true;
		}
		if (_tcsicmp(token, _T("restart")) == 0 || _tcsicmp(token, _T("r")) == 0)
			SetOption(OPT_AUTO_RESTART, enable);
		else if (_tcsicmp(token, _T("autostart")) == 0 || _tcsicmp(token, _T("s")) == 0)
			SetOption(OPT_AUTO_START, enable);
		else if (_tcsicmp(token, _T("autostop")) == 0 || _tcsicmp(token, _T("t")) == 0)
			SetOption(OPT_AUTO_STOP, enable);
		token = _tcstok(NULL, delim);
	}
}

void Global::ShowOption()
{
	// show options
	TRACE_INFODT(GLOBAL_LOGGER, _F("Show Options: "));
	if (IsOption(OPT_AUTO_RESTART))
		TRACE_INFO(GLOBAL_LOGGER, _F("restart "));
	if (IsOption(OPT_AUTO_START))
		TRACE_INFO(GLOBAL_LOGGER, _F("autostart "));
	if (IsOption(OPT_AUTO_STOP))
		TRACE_INFO(GLOBAL_LOGGER, _F("autostop "));
	TRACE_INFOL(GLOBAL_LOGGER, _F(""));
}

Boolean Global::CheckAllStarted()
{
	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx &service = it->second;
		if (service.statuses[SS_RUNNING] != SRS_STARTED)
		{
			m_bAllStarted = BOOLEAN_FALSE;
			return BOOLEAN_FALSE;
		}
	}

	m_bAllStarted = BOOLEAN_TRUE;
	return BOOLEAN_TRUE;
}

void Global::CheckAutoStart()
{
	if (!IsOption(OPT_AUTO_START))
		return;

	bool trigger = false;
	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx &service = it->second;
		if (service.statuses[SS_RUNNING] != SRS_UNKNOWN)
			return;
		// only manager with object service should issue starting of all services
		if (service.bLocal && service.type == SERVICE_OBJECT)
			trigger = true;
	}
	if (!trigger)
		return;

	for (ManagerMap::iterator it = m_managerMap.begin(); it != m_managerMap.end(); ++it)
	{
		Service& service = it->second;
		if (!service.bConnected)
			return;
	}

	// put "start all" command to execute
	TRACE_INFODTL(GLOBAL_LOGGER, _F("AutoStart: starting all services ..."));
	AddCommand(_T("start all"));
}

Boolean Global::CheckAllStopped()
{
	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx &service = it->second;
		if (service.statuses[SS_RUNNING] != SRS_UNKNOWN && service.statuses[SS_RUNNING] != SRS_STOPPED)
			return BOOLEAN_FALSE;
	}
	if (m_bStopClose) {
		TRACE_INFODTL(GLOBAL_LOGGER, _F("AutoStop: all services stopped, manger terminate ..."));
		SleepEx(200, TRUE);
		m_bStopClose = BOOLEAN_FALSE;
		m_bExit = BOOLEAN_TRUE;
	}

	return BOOLEAN_TRUE;
}

void Global::CheckAutoStop()
{
	if (m_bStopClose || !IsOption(OPT_AUTO_STOP))
		return;

	Boolean bStopped = CheckAllStopped();
	ServiceEx* pService = GetServiceEx(SERVICE_OBJECT);

	if (bStopped) {
		TRACE_INFODTL(GLOBAL_LOGGER, _F("AutoStop: all services stopped, manager terminate immediately ..."));
		StopAllServices(BOOLEAN_FALSE);
		m_bExit = BOOLEAN_TRUE;
	} else if (pService && pService->bLocal) {
		// stop all only issue by manager service running object service
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("AutoStop: stopping all services at %d second..."), 
			gConfigure.shutdownTime);

		UInt32 atTime = gConfigure.shutdownTime + 1;
		ScheduleCommand(atTime, _T("stop all 0"));
		m_bStopClose = BOOLEAN_TRUE;
	} else {
		m_bStopClose = BOOLEAN_TRUE;
	}
}

void Global::ReportStatus()
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Manager Service Report: IP %s"),
		Reuben::Network::IPToString(gConfigure.ip).c_str());

	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx& service = it->second;
		TRACE_INFOL_5(GLOBAL_LOGGER, _F("%s Service: serviceId %d, status %s, IP %s%s"),
			service.strName.c_str(), it->first, GetRunningStatusStr(service.statuses[SS_RUNNING]), 
			service.bLocal ? _T("local") :
			Reuben::Network::IPToString(service.address.GetIP()).c_str(),
			service.bConnected ? _T(", connected") : _T(""));
	}

	for (ManagerMap::iterator it = m_managerMap.begin(); it != m_managerMap.end(); ++it)
	{
		Service& service = it->second;
		TRACE_INFOL_5(GLOBAL_LOGGER, _F("%s Service: serviceId %d, status %s, IP %s%s"),
			service.strName.c_str(), service.serviceId, 
			GetRunningStatusStr(service.bConnected ? SRS_STARTED : SRS_STOPPED),
			Reuben::Network::IPToString(it->first).c_str(),
			service.bConnected ? _T(", connected") : _T(""));
	}
}

void Global::ResetStatus()
{
	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx &service = it->second;
		service.statuses[SS_RUNNING] = SRS_UNKNOWN;
		service.next_running_status = SRS_UNKNOWN;
	}
	ManagerApplication::GetInstance().ConnectServices();
}

Boolean Global::ReadyStart(ServiceEx &service)
{
	for (UInt8Vector::iterator it = service.dependList.begin(); 
		it != service.dependList.end(); ++it)
	{
		ServiceEx &service = m_srvExMap[*it];
		// make sure depending services started
		if (!(service.statuses[SS_RUNNING] == SRS_STARTED || 
			  service.statuses[SS_RUNNING] == SRS_WAITSTOP &&
			  service.next_running_status == SRS_UNKNOWN))
			return BOOLEAN_FALSE;
	}

	return BOOLEAN_TRUE;
}

Boolean Global::ReadyStop(ServiceEx &service)
{
	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx &service2 = it->second;

		if (service2.serviceId == service.serviceId)
			continue;

		// make sure all services depend this service stopped
		if (std::find(service2.dependList.begin(), service2.dependList.end(), 
			service.serviceId) != service2.dependList.end())
		{
			if (service2.statuses[SS_RUNNING] != SRS_STOPPED &&
				service2.statuses[SS_RUNNING] != SRS_UNKNOWN &&
				service2.statuses[SS_RUNNING] != SRS_WAITSTART)
				return BOOLEAN_FALSE;
		}
	}

	return BOOLEAN_TRUE;
}

void Global::AddCommand(PCSTRING cmd)
{
	ThreadGuard guard(m_key);
	m_commandList.push_back(cmd);
}

void Global::ExecuteCommand()
{
	if (m_commandList.empty())
		return;

	StringVector executeList;

	//Thread Lock
	{
		ThreadGuard guard(m_key);
		m_commandList.swap(executeList);
	}

	PCSTRING cmd;
	Char buf[COMMAND_LENGTH] = _T("");
	Char *delim = _T(", \t");
	Char *token;
	Char *arg1, *arg2 = NULL, *arg3 = NULL;

	for (StringVector::iterator it = executeList.begin(); it != executeList.end(); ++it)
	{
		cmd = it->c_str();

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Execute %s ..."), cmd);
		buf[COMMAND_LENGTH - 1] = _T('\0');
		if (_tcslen(cmd) < COMMAND_LENGTH)
			_tcscpy(buf, cmd);
		else
			_tcsncpy(buf, cmd, COMMAND_LENGTH - 1);

		token = _tcstok(buf, delim);

		if (token == NULL) continue;

		else if (token[0] >= _T('0') && token[0] <= _T('9'))
		{	// schedule time to execute following command
			m_schedTime = _ttoi(token) * MINUTE_TIME;
			token = _tcstok(NULL, _T(""));					// get the command remain part
			if (token)
				ScheduleCommand(m_schedTime, token);
		}
		else if (_tcsicmp(token, _T("exit")) == 0 || _tcsicmp(token, _T("quit")) == 0)
		{
			m_bExit = BOOLEAN_TRUE;
			ManagerApplication::GetInstance().Terminate(0);
		} 
		else if (_tcsicmp(token, _T("status")) == 0 || _tcsicmp(token, _T("report")) == 0)
		{
			ReportStatus();
		} 
		else if (_tcsicmp(token, _T("connect")) == 0)
		{
			CancelSchedule();
			ManagerApplication::GetInstance().ConnectServices();
			ManagerApplication::GetInstance().ConnectManagers();
		} 
		else if (_tcsicmp(token, _T("config")) == 0)
		{
			arg1 = _tcstok(NULL, delim);
			if (arg1) {
				CancelSchedule();
				if (_tcsicmp(arg1, _T("reload")) == 0)
					AllReadConfig(gConfigure.strConfigFile.c_str());
				else if (IsFileExist(arg1))
					AllReadConfig(arg1);
				else
					_tprintf(_T("Input Error: config file %s not found\n"), arg1);
			}
		} 
		else if (_tcsicmp(token, _T("option")) == 0)
		{
			arg1 = _tcstok(NULL, _T(""));
			if (arg1)							// set/unset (+/-) option 
				ReadOption(arg1);
			ShowOption();
		}
		else if (_tcsicmp(token, _T("reset")) == 0)
		{
			CancelSchedule();
			ResetStatus();
		} 
		else if (_tcsicmp(token, _T("check")) == 0 || 
				_tcsicmp(token, _T("start")) == 0 ||
				_tcsicmp(token, _T("stop")) == 0 ||
				_tcsicmp(token, _T("restart")) == 0 ||
				_tcsicmp(token, _T("kill")) == 0)
		{
			UInt8 serviceId = 0;
			UInt32 atTime = 0;
			arg1 = _tcstok(NULL, delim);
			if (arg1 == NULL)
				continue;
			if (_tcsicmp(arg1, _T("all")) == 0) {
				serviceId = 255;
				arg2 = _tcstok(NULL, delim);
				if (arg2)
					atTime = _ttoi(arg2) * 1000;
				else
					atTime = gConfigure.shutdownTime;
			} else if (arg1[0] >= _T('0') && arg1[0] <= _T('9')) {
				serviceId = _ttoi(arg1);
			} else {
				ServiceEx* pService = GetServiceEx(arg1);
				if (pService)
					serviceId = pService->serviceId;
			}
			CancelSchedule();
			if (serviceId == 255) {
				ReportStatus();
				if (_tcsicmp(token, _T("check")) == 0)
					CheckAllServices(BOOLEAN_FALSE);
				else if (_tcsicmp(token, _T("start")) == 0)
					StartAllServices(BOOLEAN_FALSE);
				else if (_tcsicmp(token, _T("stop")) == 0) {
					if (atTime > 0)			// schedule shutdown
						ScheduleCommand(atTime, _T("stop all 0"));
					else
						StopAllServices(BOOLEAN_FALSE);
				}
				else if (_tcsicmp(token, _T("restart")) == 0) {
					if (atTime > 0)
						ScheduleCommand(atTime, _T("restart all 0"));
					else
						RestartAllServices(BOOLEAN_FALSE);
				}
			} else if (serviceId > 0) {
				if (_tcsicmp(token, _T("check")) == 0)
					CheckService(serviceId, BOOLEAN_FALSE);
				else if (_tcsicmp(token, _T("start")) == 0)
					StartService(serviceId, BOOLEAN_FALSE);
				else if (_tcsicmp(token, _T("stop")) == 0)
					StopService(serviceId, BOOLEAN_FALSE);
				else if (_tcsicmp(token, _T("restart")) == 0)
					RestartService(serviceId, BOOLEAN_FALSE);
				else if (_tcsicmp(token, _T("kill")) == 0)
					KillService(serviceId);
			} else {
				_tprintf(_T("Input Error: wrong input service ID/Name %s\n"), arg1);
			}
		} 
	}
}

void Global::ScheduleCommand(UInt32 atTime, PCSTRING cmd)
{
	if (cmd == NULL)
		return;

	if (atTime == 0) {
		AddCommand(cmd);
		return;
	}

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ScheduleCommand: atTime %d, %s"),
		atTime, cmd);
	m_schedTime = atTime;
	_tcscpy(m_schedCmd, cmd);				// copy to schedule command

	// check if schedule command is "stop all" or "restart all"
	Char buf[COMMAND_LENGTH] = _T("");
	Char *delim = _T(", \t");
	Char *arg1, *arg2;

	_tcscpy(buf, cmd);
	arg1 = _tcstok(buf, delim);
	arg2 = _tcstok(NULL, delim);
	if (arg2 == NULL || (_tcsicmp(arg1, _T("stop")) != 0 && 
		_tcsicmp(arg1, _T("restart")) != 0) ||
		_tcsicmp(arg2, _T("all")) != 0)
	{
		return;			// not stop all nor restart all
	}

	// send down message to all services
	m_bWillStop = BOOLEAN_TRUE;
	SendShutdownTime(atTime);
}

void Global::CancelSchedule()
{
	if (m_schedTime == 0)
		return;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CancelSchedule: atTime %d, %s"),
		m_schedTime, m_schedCmd);

	if (m_bWillStop) {
		m_bWillStop = BOOLEAN_FALSE;
		SendShutdownTime(0);
	}
	if (_tcslen(m_schedCmd) > 0)
		m_schedCmd[0] = _T('\0');
	m_schedTime = 0;
}

void Global::SendShutdownTime(UInt32 downTime)
{
	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx &service = it->second;

		if (!service.bLocal)
			continue;
		ServiceStatus_Stub::CB_ShutdownAt(service.pPeer, m_context, downTime);
	}

	for (ManagerMap::iterator it = m_managerMap.begin(); it != m_managerMap.end(); it++)
	{
		Service& service = it->second;
		if (service.bConnected && service.pPeer)
			ManagerService_Proxy::ShutdownAt(service.pPeer, m_context, downTime);
	}
}

void Global::CheckSchedule(UInt32 dt)
{
	static UInt32 showTime = 0;
	
	// show count down when shutting down
	if (m_bWillStop && m_schedTime > 0) {
		UInt32 tdiff = showTime - m_schedTime;
		int min = -1;
		int sec = -1;
		if (showTime == 0 || showTime < m_schedTime) {
			showTime = m_schedTime;
			tdiff = MINUTE_TIME;
		}

		if (m_schedTime >= MINUTE_TIME && tdiff >= MINUTE_TIME)
		{	// show count down by minute
			min = (m_schedTime + MINUTE_TIME / 2) / MINUTE_TIME;
			showTime = min * MINUTE_TIME;
		} else if (m_schedTime < MINUTE_TIME && m_schedTime > 10 * SECOND_TIME &&
					tdiff >= 10 * SECOND_TIME)
		{	// show count down by 10 second after 1 minute count down
			sec = (m_schedTime + SECOND_TIME / 2) / SECOND_TIME;
			if (sec % 10 == 0)
				showTime = sec / 10 * 10 * SECOND_TIME;
			else
				showTime = (sec / 10 + 1) * 10 * SECOND_TIME;
		} else if (m_schedTime < 10 * SECOND_TIME && tdiff >= SECOND_TIME)
		{	// show count down by 10 second after 10 second count down
			sec = (m_schedTime + SECOND_TIME / 2) / SECOND_TIME;
			showTime = sec * SECOND_TIME;
		} else if (m_schedTime <= dt) {
			sec = 0;
			showTime = 0;
		}
		if (min != -1)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, 
				_T("ManagerService: shutting down all services in %d min ..."),
				min);
		}
		else if (sec != -1)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, 
				_T("ManagerService: shutting down all services in %d sec ..."),
				sec);
		}
	}
	if (m_schedTime > dt) {
		m_schedTime -= dt;
	} else {
		if (m_schedCmd[0] != _T('\0'))
			AddCommand(m_schedCmd);
		m_schedCmd[0] = _T('\0');
		m_schedTime = 0;
		showTime = 0;
	}
}

/*
Boolean Global::LocalExecute(ServiceEx &service)
{
	String progName;
	String args;

	progName.Format(_T(".\\%sService%s.exe"), service.strName.c_str(), SRVMODE);
	args.Format(_T("%s %s"), gConfigure.strConfigFile.c_str(), service.strName.c_str());

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("LocalExecute: %s %s"), 
		progName.c_str(), args.c_str());

	service.hInst = ShellExecute(NULL, NULL, progName.c_str(), args.c_str(), 
		NULL, SW_SHOWMINIMIZED);

	if ((int) service.hInst <= 32) {
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("LocalExecute Error: fail to exec %s service, errno %d"), 
			service.strName.c_str(), service.hInst);
		service.hInst = 0;
		service.status = SS_ERROR;
		return BOOLEAN_FALSE;
	}

	service.status = SS_STARTING;
	return BOOLEAN_TRUE;
}
*/

INLINE HANDLE Global::CreateProcess(LPCTSTR progName, LPCTSTR title, LPTSTR cmdLine, 
									LPCTSTR curDir)
{
	STARTUPINFO startInfo;
	PROCESS_INFORMATION procInfo;

	ZeroMemory((void*)&startInfo, sizeof(STARTUPINFO));
	startInfo.cb = sizeof(startInfo);
	startInfo.lpDesktop = _T("");
	startInfo.lpTitle = (LPTSTR) title;
	startInfo.wShowWindow = (title ? SW_SHOWMINNOACTIVE : 0);
	startInfo.dwFlags = STARTF_USECOUNTCHARS | STARTF_USESHOWWINDOW;
	startInfo.dwXCountChars = 100;
	startInfo.dwYCountChars = 50;

	BOOL bCreated = ::CreateProcess(progName, cmdLine, NULL, NULL, 
		FALSE, CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP | CREATE_UNICODE_ENVIRONMENT,
		NULL, curDir, &startInfo, &procInfo);

	if (!bCreated) {
		DWORD lastErr = ::GetLastError();
		LPVOID lpMsgBuf = NULL;

		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, lastErr, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), // English language
			(LPTSTR) &lpMsgBuf, 0, NULL);
		TRACE_ERRORDTL_4(GLOBAL_LOGGER, 
			_T("CreateProcess Error: execute %s, cmd \"%s\", errno %d: %s"), 
			progName, cmdLine, lastErr, (lpMsgBuf ? (LPCTSTR) lpMsgBuf : _T("")));

		if (lpMsgBuf)				// free the buffer after use
			LocalFree(lpMsgBuf);
		return NULL;
	}

	return procInfo.hProcess;
}

Boolean Global::LocalExecute(ServiceEx &service)
{
	Char progName[256];
	Char cmdLine[COMMAND_LENGTH];
	Char title[256];

	_stprintf(progName, _T("%sService%s.exe"), ServiceTypeToString(service.type), SRVMODE);
	_stprintf(cmdLine, _T("%s %s %s"), progName, gConfigure.strConfigFile.c_str(), 
		service.strName.c_str());
	_tcscpy(title, service.strName.c_str());

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("LocalExecute: %s"), cmdLine);

	service.hproc = CreateProcess(progName, title, cmdLine);
	if (service.hproc != NULL) {
		service.statuses[SS_RUNNING] = SRS_STARTING;
		m_bCheck = BOOLEAN_TRUE;
	} else {
		service.statuses[SS_RUNNING] = SRS_ERROR;
	}

	return (service.hproc != NULL);
}

void Global::LocalShutdown(ServiceEx &service)
{
	if (service.pPeer == NULL)
		return;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("LocalShutdown: Stopping %s service, running_status %d"), 
		service.strName.c_str(), service.statuses[SS_RUNNING]);
	service.statuses[SS_RUNNING] = SRS_STOPPING;
	ServiceStatus_Stub::CB_StopService(service.pPeer, m_context, 
		gConfigure.graceStopTime);
	SleepEx(200, BOOLEAN_TRUE);			// wait for a little after sending shutdown
}

void Global::ErrorArchive(ServiceID serviceId, Boolean localOnly)
{
	ServiceExMap::iterator it = m_srvExMap.find(serviceId);

	if (it == m_srvExMap.end()) {
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("ErrorArchive: serviceId %d not found"), 
			serviceId);
		return;
	}

	ServiceEx &service = it->second;

	if (!localOnly) {
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ErrorArchive: %s service, running_status %d, wait for shutdown"), 
			service.strName.c_str(), service.statuses[SS_RUNNING]);
		// wait error service to shutdown completely
		if (service.hproc)
			::WaitForSingleObject(service.hproc, ERR_WAIT_TIME);
		else
			Sleep(1000);
	} else {
		// check if any service exists
		bool hasService = false;

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ErrorArchive: remote %s service shutdown, running_status %d"), 
			service.strName.c_str(), service.statuses[SS_RUNNING]);

		for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
		{
			ServiceEx& service = it->second;
			if (service.bLocal) {
				hasService = true;
				break;
			}
		}
		if (!hasService)		// no service for this manager, just return
			return;
	}

	// tell other manager to archive log at the same time
	if (!localOnly)  {
		for (ManagerMap::iterator it = m_managerMap.begin(); it != m_managerMap.end(); it++)
		{
			Service& service = it->second;
			if (service.bConnected && service.pPeer)
				ManagerService_Proxy::ErrorArchive(service.pPeer, m_context, serviceId);
		}
	}

	// create archive path if not exist
	DWORD attr = ::GetFileAttributes(ARCHIVE_PATH);
	if (attr == INVALID_FILE_ATTRIBUTES)
		::CreateDirectory(ARCHIVE_PATH, NULL);

	Char cmdLine[COMMAND_LENGTH];
	String zipFile;
	SYSTEMTIME st;

	// format zip filename
	::GetLocalTime(&st);
	zipFile.Format(_T("%s/err_%02d%02d%02d_%02d%02d%02d.zip"), ARCHIVE_PATH,
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	// zip all error files and all service logs to an error zip file
	_stprintf(cmdLine, _T("zip -m %s Error/* Log/*"), zipFile.c_str());
	HANDLE h = CreateProcess(NULL, NULL, cmdLine);
	if (h != NULL) {
		TRACE_INFODT_1(GLOBAL_LOGGER, _F("ErrorArchive: waiting execute command: %s ..."), 
			cmdLine);
		::WaitForSingleObject(h, INFINITE);			// wait zip command to finish
		TRACE_INFOL(GLOBAL_LOGGER, _F("done"));
	}
}

void Global::KickWaiting(ServiceID serviceId, UInt32 state)
{
	ServiceExMap::iterator it1 = m_srvExMap.find(serviceId);
	if (it1 == m_srvExMap.end())
		return;

	// check if next_running_status is to restart 
	ServiceEx &service1 = it1->second;
	if (service1.next_running_status == SRS_WAITSTART && service1.pPeer == NULL &&
		(state == SRS_STOPPED || state == SRS_UNKNOWN))
	{
		if (ReadyStart(service1))
			LocalExecute(service1);
		else
			service1.statuses[SS_RUNNING] = SRS_WAITSTART;
		service1.next_running_status = SRS_UNKNOWN;
	} else if (service1.next_running_status == SRS_WAITSTOP && service1.pPeer &&
		(state == SRS_STARTED || state == SRS_UNKNOWN)) {
		if (ReadyStop(service1))
			LocalShutdown(service1);
		else
			service1.statuses[SS_RUNNING] = SRS_WAITSTOP;
		service1.next_running_status = SRS_UNKNOWN;
	} else if (service1.next_running_status == SRS_WAITRESTART && service1.pPeer &&
		(state == SRS_STARTED || state == SRS_UNKNOWN)) {
		if (service1.statuses[SS_RUNNING] == SRS_STARTED) {
			if (ReadyStop(service1))
				LocalShutdown(service1);
			else
				service1.statuses[SS_RUNNING] = SRS_WAITSTOP;
		}
		service1.next_running_status = SRS_WAITSTART;
	}

	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx &service = it->second;
		if (!service.bLocal || service.serviceId == serviceId)
			continue;

		if (state == SRS_STARTED && service.statuses[SS_RUNNING] == SRS_WAITSTART &&
			std::find(service.dependList.begin(), service.dependList.end(), serviceId) != service.dependList.end())
		{
			if (ReadyStart(service))
				LocalExecute(service);
		}
		else if ((state == SRS_STOPPED || state == SRS_UNKNOWN) && 
					service.statuses[SS_RUNNING] == SRS_WAITSTOP)
		{
			if (ReadyStop(service))
				LocalShutdown(service);
		} else if (service.statuses[SS_RUNNING] == SRS_WAITRESTART)
		{
			if (ReadyStop(service) && ReadyStart(service))
				LocalExecute(service);
		}
	}
}

void Global::CheckAllServices(Boolean localOnly)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CheckAllServices: localOnly %d"), localOnly);

	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx &service = it->second;
		if (service.bLocal && service.pPeer)
			ServiceStatus_Stub::CB_CheckService(service.pPeer, m_context);
	}

	if (!localOnly) {
		for (ManagerMap::iterator it = m_managerMap.begin(); it != m_managerMap.end(); it++)
		{
			Service& service = it->second;
			if (service.bConnected && service.pPeer)
				ManagerService_Proxy::CheckAllServices(service.pPeer, m_context);
		}
	}
}

void Global::StartAllServices(Boolean localOnly)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("StartAllServices: localOnly %d"), localOnly);

	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx &service = it->second;

		if (!service.bLocal || service.statuses[SS_RUNNING] == SRS_STARTED)
			continue;

		service.next_running_status = SRS_UNKNOWN;			// reset next status
		if (service.statuses[SS_RUNNING] == SRS_UNKNOWN || service.statuses[SS_RUNNING] == SRS_STOPPED)
		{
			if (ReadyStart(service))
				LocalExecute(service);
			else
				service.statuses[SS_RUNNING] = SRS_WAITSTART;
		}
		else
		{
			if (service.statuses[SS_RUNNING] == SRS_STOPPING)
				service.next_running_status = SRS_WAITSTART;
		}
	}

	if (!localOnly)
	{
		for (ManagerMap::iterator it = m_managerMap.begin(); it != m_managerMap.end(); it++)
		{
			Service& service = it->second;
			if (service.bConnected && service.pPeer)
				ManagerService_Proxy::StartAllServices(service.pPeer, m_context);
		}
	}
}

void Global::StopAllServices(Boolean localOnly)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("StopAllServices: localOnly %d"), localOnly);

	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx &service = it->second;

		if (!service.bLocal)
			continue;

		service.next_running_status = SRS_UNKNOWN;			// reset next status
		if (service.statuses[SS_RUNNING] == SRS_WAITSTART)
			service.statuses[SS_RUNNING] = SRS_UNKNOWN;
		if (service.pPeer && (service.statuses[SS_RUNNING] == SRS_UNKNOWN || service.statuses[SS_RUNNING] == SRS_STARTED))
		{
			if (ReadyStop(service))
				LocalShutdown(service);
			else
				service.statuses[SS_RUNNING] = SRS_WAITSTOP;
		}
		if (service.pPeer &&
			service.statuses[SS_RUNNING] != SRS_STOPPED &&
			service.statuses[SS_RUNNING] != SRS_STOPPING &&
			service.statuses[SS_RUNNING] != SRS_WAITSTOP)
			service.next_running_status = SRS_WAITSTOP;
	}

	if (!localOnly)
	{
		for (ManagerMap::iterator it = m_managerMap.begin(); it != m_managerMap.end(); it++)
		{
			Service& service = it->second;
			if (service.bConnected && service.pPeer)
				ManagerService_Proxy::StopAllServices(service.pPeer, m_context, m_bStopClose);
		}
	}
}

void Global::RestartAllServices(Boolean localOnly, Boolean bErr)
{
	bool bStopped = true;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("RestartAllServices: %slocalOnly %d"), 
		(bErr ? _T("Error Auto Restart, ") : _T("")), localOnly);

	// check if all service stopped
	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx &service = it->second;
		if (service.statuses[SS_RUNNING] == SRS_STARTED || service.statuses[SS_RUNNING] == SRS_STARTING)
		{
			bStopped = false;
			break;
		}
	}

	// if all service stopped, start all services
	if (bStopped)
	{
		StartAllServices(localOnly);
		return;
	} 

	// if any service not stopped, stop all services, and mark start at next status
	for (ServiceExMap::iterator it = m_srvExMap.begin(); it != m_srvExMap.end(); ++it)
	{
		ServiceEx &service = it->second;

		if (!service.bLocal)
			continue;

		service.next_running_status = SRS_UNKNOWN;			// reset next status
		// stop service if service is started
		if (service.pPeer && (service.statuses[SS_RUNNING] == SRS_UNKNOWN || service.statuses[SS_RUNNING] == SRS_STARTED))
		{
			if (ReadyStop(service))
				LocalShutdown(service);
			else
				service.statuses[SS_RUNNING] = SRS_WAITSTOP;
		}
		if (service.statuses[SS_RUNNING] == SRS_UNKNOWN)
			if (service.dependList.empty())
				service.statuses[SS_RUNNING] = SRS_WAITRESTART;
			else
				service.statuses[SS_RUNNING] = SRS_WAITSTART;
		if (service.statuses[SS_RUNNING] == SRS_STARTING)
			service.next_running_status = SRS_WAITRESTART;
		else if (service.statuses[SS_RUNNING] == SRS_STOPPING || service.statuses[SS_RUNNING] == SRS_WAITSTOP)
			service.next_running_status = SRS_WAITSTART;
	}

	if (!localOnly)
	{
		for (ManagerMap::iterator it = m_managerMap.begin(); it != m_managerMap.end(); it++)
		{
			Service& service = it->second;
			if (service.bConnected && service.pPeer)
				ManagerService_Proxy::RestartAllServices(service.pPeer, m_context, bErr);
		}
	}
}

void Global::CheckService(ServiceID serviceId, Boolean localOnly)
{
	ServiceExMap::iterator it = m_srvExMap.find(serviceId);

	if (it != m_srvExMap.end()) {
		ServiceEx &service = it->second;

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CheckService: serviceId %d, localOnly %d, %d service, running_status %s"), 
			serviceId, localOnly, service.strName.c_str(), GetRunningStatusStr(service.statuses[SS_RUNNING]));

		if (service.bLocal && service.pPeer)
		{
			ServiceStatus_Stub::CB_CheckService(service.pPeer, m_context);
		}
		else if (!service.bLocal && !localOnly)
		{
			ManagerMap::iterator it2 = m_managerMap.find(service.address.GetIP());
			if (it2 != m_managerMap.end())
			{
				Service &service2 = it2->second;
				if (service2.pPeer)
					ManagerService_Proxy::CheckService(service2.pPeer, m_context, serviceId);
			}
		}
	}
}

void Global::StartService(ServiceID serviceId, Boolean localOnly)
{
	ServiceExMap::iterator it = m_srvExMap.find(serviceId);

	if (it != m_srvExMap.end())
	{
		ServiceEx &service = it->second;

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("StartService: serviceId %d, localOnly %d, %d service, running_status %s"), 
			serviceId, localOnly, service.strName.c_str(), GetRunningStatusStr(service.statuses[SS_RUNNING]));
		if (service.bLocal && (service.statuses[SS_RUNNING] == SRS_UNKNOWN || service.statuses[SS_RUNNING] == SRS_STOPPED))
		{
			if (ReadyStart(service))
				LocalExecute(service);
			else
				service.statuses[SS_RUNNING] = SRS_WAITSTART;
		}
		else if (!service.bLocal && !localOnly)
		{
			ManagerMap::iterator it2 = m_managerMap.find(service.address.GetIP());
			if (it2 != m_managerMap.end()) {
				Service &service2 = it2->second;
				if (service2.pPeer)
					ManagerService_Proxy::StartService(service2.pPeer, m_context, serviceId);
			}
		}
	}
}

void Global::StopService(ServiceID serviceId, Boolean localOnly)
{
	ServiceExMap::iterator it = m_srvExMap.find(serviceId);

	if (it != m_srvExMap.end()) {
		ServiceEx &service = it->second;

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("StopService: serviceId %d, localOnly %d, %d service, running_status %s"), 
			serviceId, localOnly, service.strName.c_str(), GetRunningStatusStr(service.statuses[SS_RUNNING]));
		if (service.bLocal) {
			if (service.statuses[SS_RUNNING] == SRS_WAITSTART)
				service.statuses[SS_RUNNING] = SRS_UNKNOWN;
			if (service.pPeer && 
				(service.statuses[SS_RUNNING] == SRS_UNKNOWN || service.statuses[SS_RUNNING] == SRS_STARTED))
			{
				if (ReadyStop(service))
					LocalShutdown(service);
				else
					service.statuses[SS_RUNNING] = SRS_WAITSTOP;
			}
			else if (service.statuses[SS_RUNNING] == SRS_WAITSTART)
			{
				service.statuses[SS_RUNNING] = SRS_UNKNOWN;
			}
		}
		else if (!localOnly)
		{
			ManagerMap::iterator it2 = m_managerMap.find(service.address.GetIP());
			if (it2 != m_managerMap.end())
			{
				Service &service2 = it2->second;
				if (service2.pPeer)
					ManagerService_Proxy::StopService(service2.pPeer, m_context, serviceId);
			}
		}
	}
}

void Global::RestartService(ServiceID serviceId, Boolean localOnly)
{
	ServiceExMap::iterator it = m_srvExMap.find(serviceId);

	if (it != m_srvExMap.end()) {
		ServiceEx &service = it->second;

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("RestartService: serviceId %d, localOnly %d, %d service, running_status %s"), 
			serviceId, localOnly, service.strName.c_str(), GetRunningStatusStr(service.statuses[SS_RUNNING]));
		if (service.bLocal) {
			if (service.pPeer == NULL && 
				(service.statuses[SS_RUNNING] == SRS_UNKNOWN || service.statuses[SS_RUNNING] == SRS_STOPPED))
			{
				if (ReadyStart(service))
					LocalExecute(service);
				else
					service.statuses[SS_RUNNING] = SRS_WAITSTART;
			} else if (service.pPeer && 
				(service.statuses[SS_RUNNING] == SRS_UNKNOWN || service.statuses[SS_RUNNING] == SRS_STARTED))
			{
				if (ReadyStop(service))
					LocalShutdown(service);
				else
					service.statuses[SS_RUNNING] = SRS_WAITSTOP;
			}
			if (service.statuses[SS_RUNNING] == SRS_STOPPING || service.statuses[SS_RUNNING] == SRS_WAITSTOP)
				service.next_running_status = SRS_WAITSTART;
		}
		else if (!localOnly)
		{
			ManagerMap::iterator it2 = m_managerMap.find(service.address.GetIP());
			if (it2 != m_managerMap.end()) {
				Service &service2 = it2->second;
				if (service2.pPeer)
					ManagerService_Proxy::StopService(service2.pPeer, m_context, serviceId);
			}
		}
	}
}

void Global::KillService(ServiceID serviceId)
{
	ServiceExMap::iterator it = m_srvExMap.find(serviceId);

	if (it != m_srvExMap.end())
	{
		ServiceEx &service = it->second;

		if (service.pPeer == NULL)
			service.pPeer = LocalRPCManager::GetInstance().GetModel().Connect(service.address, service.protocol, service.bTrust);

		if (service.pPeer)
		{
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("KillService: serviceId %d, %d service, running_status %s"), 
				serviceId, service.strName.c_str(), GetRunningStatusStr(service.statuses[SS_RUNNING]));
			ServiceStatus_Stub::CB_StopService(service.pPeer, m_context, 0);
		}
		service.statuses[SS_RUNNING] = SRS_UNKNOWN;
		service.next_running_status = SRS_UNKNOWN;
	}
}

void Global::GetDBCheatCommand()
{
	HRESULT hr;
	if (DB_SUCCEEDED(hr = m_pdboExecuteCheat->RSByUnread()))
	{
		hr = m_pdboExecuteCheat->MoveFirst();
		while (hr == S_OK)
		{
			CheatCommandData cheat;
			cheat.acctId		= m_pdboExecuteCheat->m_acct_id.LowPart;
			cheat.command		= m_pdboExecuteCheat->m_command;
			cheat.exeId			= m_pdboExecuteCheat->m_exe_id.LowPart;
			cheat.targetAcctId	= m_pdboExecuteCheat->m_target_acct_id.LowPart;
			cheat.targetCharId	= m_pdboExecuteCheat->m_target_char_id.LowPart;
			cheat.requestSent	= FALSE;
			m_cheatCommandMap.insert(std::make_pair(cheat.exeId, cheat));
			hr = m_pdboExecuteCheat->MoveNext();

			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Global::GetDBCheatCommand Info (1) [record read] exeid %d, command %s\n"), 
				cheat.exeId, cheat.command.c_str());
		}
	}
	m_pdboExecuteCheat->Close();
}

void Global::MarkDBExecuteCheatRead()
{
	HRESULT hr;
	for( CheatCommandMap::iterator itr = m_cheatCommandMap.begin(); itr != m_cheatCommandMap.end(); )
	{
		// mark DB as read, then clear from list
		CheatCommandData& cheat = itr->second;
		if( DB_FAILED(hr = m_pdboExecuteCheat->UpdateAsRead(cheat.exeId)) )
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, 
				_T("Global::MarkDBCheatCommandAsRead Warning (1) [mark cheat read failed] exeid %d, command %s\n"), 
				cheat.exeId, cheat.command.c_str());
			 ++itr;
		}else
		{
			itr = m_cheatCommandMap.erase(itr);
		}
	}
}

void Global::CheatCommandDBFail()
{
	HRESULT hr;
	if( DB_FAILED(hr = m_pdboExecuteCheat->UpdateUnreadAsFailed()) )
		TRACE_INFODTL(GLOBAL_LOGGER, _F("Global::CheatCommandDBFail Warning (1) [db update failed]"));
}
