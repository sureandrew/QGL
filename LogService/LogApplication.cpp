//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "LogService/LogApplication.h"
//-- Library
#include "Proxy/Service/ServiceStatus.h"
#include "LogService/Global.h"
#include "LogService/Configure.h"

#define UPDATE_INTERVAL		10

Boolean LogApplication::OnCreate(VOID)
{
	if (!gGlobal.Initialize())
		return (BOOLEAN_FALSE);

	if (!gGlobal.ConnectDatabase())
		return (BOOLEAN_FALSE);

	if (!StartService(gConfigure))
		return (BOOLEAN_FALSE);

	ConnectServices(gConfigure);

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("LogApplication::OnCreate : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	return (BOOLEAN_TRUE);
}

VOID LogApplication::OnDispose(VOID)
{	
	// tell manager service this service termination normally
	if (gGlobal.pManagerPeer)
	{
		ServiceStatus_Proxy::UpdateStatus(gGlobal.pManagerPeer, RPCContext(), gConfigure.serviceId, SS_RUNNING, SRS_STOPPED);
		SafeRelease(gGlobal.pManagerPeer);
		Sleep(10);
	}

	StopService(gConfigure);
	LocalRPCManager::GetInstance().GetModel().Shutdown();

	SessionManager::GetInstance().ClearAll();
	gGlobal.Uninitialize();

	LocalRPCManager::GetInstance().DeleteAll();
	Reuben::Simulation::ObjectInstance::GetInstance().DeleteAll();
	ProcedureManager::DestroyInstance();
	LocalRPCManager::DestroyInstance();
	SessionManager::DestroyInstance();

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("LogApplication::OnDispose : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());
}

VOID LogApplication::OnUpdate(UInt32 uTimeDelta)
{
	if (uTimeDelta > 1000)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("TimeDelta %d"), uTimeDelta);
	}

	// process RPC message
	LocalRPCManager::GetInstance().OnUpdate(uTimeDelta);

	if (m_manage_interval > gConfigure.manage_interval)
	{
		DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("LogApplication::OnUpdate : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());
		m_manage_interval = 0;
	}
	m_manage_interval += uTimeDelta;

	SleepEx(UPDATE_INTERVAL, TRUE);

	if (gGlobal.m_bExit)
		Terminate(0);
}

VOID LogApplication::OnAccept(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID LogApplication::OnFull(CONST Listener* listener)
{
}

VOID LogApplication::OnOpen(CONST Listener* listener)
{
}

VOID LogApplication::OnOpen(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID LogApplication::OnClose(CONST Listener* listener)
{
}

VOID LogApplication::OnClose(CONST Listener* listener, CONST Peer* pPeer)
{
	if (gGlobal.pManagerPeer == pPeer)
		SafeRelease(gGlobal.pManagerPeer);
}
