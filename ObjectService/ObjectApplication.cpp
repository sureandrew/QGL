//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "ObjectService/ObjectApplication.h"
//-- Library
#include "Proxy/Service/ServiceStatus.h"
#include "ObjectService/Global.h"
#include "ObjectService/Configure.h"
#include "ObjectService/DboSourcePool.h"

#define UPDATE_INTERVAL		10

Boolean ObjectApplication::OnCreate(VOID)
{
	if (!gGlobal.Initialize())
		return (BOOLEAN_FALSE);

	if (!gGlobal.ConnectDatabase())
		return (BOOLEAN_FALSE);

	if (!StartService(gConfigure))
		return (BOOLEAN_FALSE);

	ConnectServices(gConfigure);

	// connect local peer
	gGlobal.pLocalPeer = LocalRPCManager::GetInstance().GetModel().Connect(
		Reuben::Network::Address(gConfigure.ip, gConfigure.port), 
		Reuben::Network::PROTOCOL_TCP, BOOLEAN_TRUE);

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("ObjectApplication::OnCreate : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	return (BOOLEAN_TRUE);
}

VOID ObjectApplication::OnDispose(VOID)
{
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("ObjectApplication::OnDispose"));

	gGlobal.WorkQueueStop();			// stopping all working queues
	SessionManager::GetInstance().ClearAll();
	gGlobal.Uninitialize();

	if (gGlobal.pLocalPeer)
	{
		LocalRPCManager::GetInstance().GetModel().CloseSocket(gGlobal.pLocalPeer->GetSocketID());
		SafeRelease(gGlobal.pLocalPeer);
	}

	LocalRPCManager::GetInstance().DeleteAll();
	Reuben::Simulation::ObjectInstance::GetInstance().DeleteAll();
	StopService(gConfigure);

	// tell manager service this service termination normally
	if (gGlobal.pManagerPeer)
	{
		ServiceStatus_Proxy::UpdateStatus(gGlobal.pManagerPeer, RPCContext(), gConfigure.serviceId, SS_RUNNING, SRS_STOPPED);
		SafeRelease(gGlobal.pManagerPeer);
	}
	LocalRPCManager::GetInstance().GetModel().Shutdown();
	ProcedureManager::DestroyInstance();
	LocalRPCManager::DestroyInstance();
	SessionManager::DestroyInstance();

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("ObjectApplication::OnDispose : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());

}

VOID ObjectApplication::OnUpdate(UInt32 uTimeDelta)
{
	if (uTimeDelta > 1000)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("TimeDelta %d"), uTimeDelta);
	}
	static UInt32 lastCacheTime = ::GetTickCount();
	UInt32 curTime = ::GetTickCount();
	if (curTime - lastCacheTime >= 1000)
	{
		// one second checking
		gGlobal.ReduceCache(curTime);
		lastCacheTime = curTime;
	}

	// db source pool update (still not stable to close connection)
	gGlobal.acctDbPool.OnUpdate(uTimeDelta);
	gGlobal.gameDbPool.OnUpdate(uTimeDelta);

	// process RPC message
	curTime = ::GetTickCount();
	LocalRPCManager::GetInstance().OnUpdate(uTimeDelta);
	ProcedureManager::GetInstance().OnUpdate(uTimeDelta);
	UInt32 diffTime = ::GetTickCount() - curTime;
	if (diffTime > 1000)
	{
		// show busy time of local and procedure manager
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("TimeDelta LocalRPCManager %d"), diffTime);
	}
	curTime = ::GetTickCount();
	gGlobal.UpdateDb(curTime);
	diffTime = ::GetTickCount() - curTime;
	//if (diffTime > 1000)
	//{
	//	TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("TimeDelta UpdateDb %d"), diffTime);
	//}
	
	if (m_manage_interval > gConfigure.manage_interval)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("memory status, %s"), 
			MemoryManager::GetInstance().GetStatus().c_str());
		m_manage_interval = 0;
	}
	m_manage_interval += uTimeDelta;

	SleepEx(UPDATE_INTERVAL, TRUE);

	if (gGlobal.m_bExit)		// program exit
		Terminate(0);
}

VOID ObjectApplication::OnAccept(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID ObjectApplication::OnFull(CONST Listener* listener)
{
}

VOID ObjectApplication::OnOpen(CONST Listener* listener)
{
}

VOID ObjectApplication::OnOpen(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID ObjectApplication::OnClose(CONST Listener* listener)
{
}

VOID ObjectApplication::OnClose(CONST Listener* listener, CONST Peer* pPeer)
{
	if (gGlobal.pManagerPeer == pPeer)
		SafeRelease(gGlobal.pManagerPeer);
	if (gGlobal.pLocalPeer == pPeer)
		SafeRelease(gGlobal.pLocalPeer);
}
