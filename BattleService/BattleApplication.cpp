//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "BattleService/BattleApplication.h"
//-- Library
#include "BattleService/Configure.h"
#include "BattleService/Global.h"
#include "Proxy/Service/MapService_proxy.h"
#include "Proxy/Service/ServiceStatus.h"
#include "Stub/Logic/MapBattle.h"

#define UPDATE_INTERVAL				10

Boolean BattleApplication::OnCreate(VOID)
{
	if (!gGlobal.Initialize())
		return BOOLEAN_FALSE;

	// set direct pointer to service peer	
	for (ServiceMap::iterator it = gConfigure.services.begin(); it != gConfigure.services.end(); ++it)
	{
		switch (it->second.type)
		{
		case SERVICE_OBJECT:
			if (gGlobal.pObjectService == NULL)
			{
				gGlobal.pObjectService = &it->second;
			}
			break;
		case SERVICE_MAP:
			gGlobal.mapServices[it->second.serviceId] = &it->second;
			break;
		}
	}

	if (!StartService(gConfigure))
		return (BOOLEAN_FALSE);

	ConnectServices(gConfigure);

	// register battle service in map service
	for (ServicePointerMap::iterator it2 = gGlobal.mapServices.begin(); it2 != gGlobal.mapServices.end(); ++it2)
	{
		if (it2->second->pPeer)
			MapService_Proxy::RegisterBattle(it2->second->pPeer, RPCContext(), gConfigure.serviceId);
	}

	return (BOOLEAN_TRUE);
}

VOID BattleApplication::OnDispose(VOID)
{
	// tell manager service this service termination normally
	if (gGlobal.pManagerPeer) {
		ServiceStatus_Proxy::UpdateStatus(gGlobal.pManagerPeer, RPCContext(), 
			gConfigure.serviceId, SS_STOPPED);
		gGlobal.pManagerPeer = NULL;
		Sleep(10);
	}

	gGlobal.m_sessionManager.ClearAll();
	StopService(gConfigure);
	LocalRPCManager::GetInstance().GetModel().Shutdown();

	gGlobal.Uninitialize();
	LocalRPCManager::GetInstance().DeleteAll();
	Reuben::Simulation::ObjectInstance::GetInstance().DeleteAll();
	ProcedureManager::DestroyInstance();
	LocalRPCManager::DestroyInstance();
}

VOID BattleApplication::OnFatalError(Exception& ex)
{
	gGlobal.DestroyGameContext();
}

VOID BattleApplication::OnUpdate(UInt32 uTimeDelta)
{
	static UInt32 t1 = 0;
	static UInt32 t2 = 0;
	static UInt32 t3 = 0;
	UInt32 lastTime = ::GetTickCount();
	UInt32 dt;

	// process RPC message
	LocalRPCManager::GetInstance().OnUpdate(uTimeDelta);

	dt = GetTickCount() - lastTime;
	lastTime += dt;
	if (t1 < dt) {
		t1 = dt;
		TRACE_INFODTL_1(GLOBAL_LOGGER, _T("OnUpdate (1) LocalRPCManager: runtime %d"), t1);
	}

	// process procedure
	ProcedureManager::GetInstance().OnUpdate(uTimeDelta);

	// process attached process
	//m_processManager.UpdateProcesses(uTimeDelta);

	// process events
	Reuben::Simulation::ScheduleInstance::GetInstance().OnUpdate(uTimeDelta);

	dt = GetTickCount() - lastTime;
	lastTime += dt;
	if (t2 < dt) {
		t2 = dt;
		TRACE_INFODTL_1(GLOBAL_LOGGER, _T("OnUpdate (2): runtime %d"), t2);
	}

	ClearDeadSession();

	// clear dead battles
	for (MapBattleList::iterator it = gGlobal.m_deadBattles.begin();
		it != gGlobal.m_deadBattles.end(); ++it)
	{
		DeleteNetObject(*it);
	}
	gGlobal.m_deadBattles.clear();

	dt = GetTickCount() - lastTime;
	lastTime += dt;
	if (t3 < dt) {
		t3 = dt;
		TRACE_INFODTL_1(GLOBAL_LOGGER, _T("OnUpdate (3): runtime %d"), t3);
	}

	SleepEx(UPDATE_INTERVAL, TRUE);

	if (gGlobal.m_bExit)
		Terminate(0);
}

VOID BattleApplication::OnAccept(CONST Listener* listener, LPCPEER pPeer)
{
}

VOID BattleApplication::OnFull(CONST Listener* listener)
{
}

VOID BattleApplication::OnOpen(CONST Listener* listener)
{
}

VOID BattleApplication::OnOpen(CONST Listener* listener, LPCPEER pPeer)
{
}

VOID BattleApplication::OnClose(CONST Listener* listener)
{
}

VOID BattleApplication::OnClose(CONST Listener* listener, LPCPEER pPeer, Buffer* pIncomplete)
{
	if (gGlobal.pManagerPeer == pPeer)
		gGlobal.pManagerPeer = NULL;

	ServiceID srvId = UnregisterProxy(pPeer);
	if (srvId != INVALID_SERVICE_ID)
	{
	}
}
