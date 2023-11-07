//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/SessionManager.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "MapService/MapApplication.h"
//-- Library
#include "Proxy/Service/WorldService_proxy.h"
#include "Proxy/Service/ServiceStatus.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/Map.h"
#include "Stub/Logic/ChargeShop.h"
#include "Stub/Logic/Battle.h"
#include "Stub/Service/MapService_Stub.h"
#include "MapService/Configure.h"
#include "MapService/Global.h"
#include "MapService/ViewManager.h"
#include "MapService/QuestionControl.h"
#include "Stub/Logic/RelationSystem.h"
#include "Proxy/Data/MarriageData.h"
#include "Resource/GameStringTable.h"
#include "Proxy/Service/ObjectService.h"

#define UPDATE_INTERVAL				10

VOID MapApplication::Shutdown(UInt32 stopTime)
{
	SessionIdList sessIdList;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("MapApplication Shutdown: stopTime %d"),
		stopTime);

	gGlobal.m_bExit = BOOLEAN_TRUE;				// set exit flag
	m_stopTime = stopTime;						// graceful stopping time

	// all sessions characters leave world
	SessionManager::GetInstance().GetSessionIds(gConfigure.serviceId, sessIdList);
	for (SessionIdList::iterator it = sessIdList.begin(); 
		it != sessIdList.end(); ++it)
	{
		Session* psess = GetSession(*it);
		if (psess && psess->pchar)
			psess->pchar->ScheduleLogout(0);
	}

	// stop all battles
	BattleList batList;

	gGlobal.m_aliveBattles.swap(batList);
	for (BattleList::iterator it = batList.begin(); it != batList.end(); ++it)
	{
		CBattle *pBattle = *it;
		if (pBattle) {
			pBattle->SetForceEnd(BOOLEAN_TRUE);
			pBattle->FinishBattle();
		}
	}

}

Boolean MapApplication::OnCreate(VOID)
{
	if (!gGlobal.Initialize())
		return (BOOLEAN_FALSE);

	// set direct pointer to service peer	
	for (ServiceMap::iterator it = gConfigure.services.begin(); it != gConfigure.services.end(); ++it)
	{
		switch (it->second.type)
		{
		case SERVICE_OBJECT:
			gGlobal.pObjectService = &it->second;
			break;
		case SERVICE_WORLD:
			gGlobal.pWorldService = &it->second;
			break;
		case SERVICE_MESSAGE:
			gGlobal.pMessageService = &it->second;
			break;
		case SERVICE_LOG:
			gGlobal.pLogService = &it->second;
			break;
		}
	}

	if (!StartService(gConfigure))
		return (BOOLEAN_FALSE);

	ConnectServices(gConfigure);

	// load map data after connect services
	if (!gGlobal.LoadMapData())
		return (BOOLEAN_FALSE);

	WorldService_Proxy::GetWorldGameTime(gGlobal.pWorldService->pPeer, RPCContext());

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	gGlobal.m_pChargeShop->InitChargeShop();//LoadChargeShopItem();

	ObjectService_Proxy::GetSingleMarriageLst(gGlobal.pObjectService->pPeer,RPCContext() );

	UInt32Vector LineList;
	MapIndexSet::iterator iter = gConfigure.mapIndices.begin();
	MapIndexSet::iterator iterend = gConfigure.mapIndices.end();
	for(;iter != iterend; ++iter)
	{
		MapIndex mapIndex = *iter;
		if(std::find(LineList.begin(),LineList.end(),mapIndex.line_id) == LineList.end())
		{
			ObjectService_Proxy::GetNpcFlagsByLineId(gGlobal.pObjectService->pPeer,RPCContext(),mapIndex.line_id);
			LineList.push_back(mapIndex.line_id);
		}
	}

	return (BOOLEAN_TRUE);
}

VOID MapApplication::OnDispose(VOID)
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
	CQuestionControl::DestroyInstance();

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());
}

VOID MapApplication::OnFatalError(Exception& ex)
{
}

#define WARN_DIFF_TIME		200

VOID MapApplication::OnUpdate(UInt32 uTimeDelta)
{
	if (uTimeDelta > 1000)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("TimeDelta %d"), uTimeDelta);
	}

	//static MapIndexList::iterator syncIt = gGlobal.m_mapIndices.begin();
	UInt32 lastTime = ::GetTickCount();
	UInt32 dt;

	// process RPC message
	LocalRPCManager::GetInstance().OnUpdate(uTimeDelta);
	dt = GetTickCount() - lastTime;
	lastTime += dt;
	if (dt > WARN_DIFF_TIME) {
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("OnUpdate (1) LocalRPCManager: runtime %d"), dt);
	}

	// process procedure
	ProcedureManager::GetInstance().OnUpdate(uTimeDelta);

	// process events
	Reuben::Simulation::ScheduleInstance::GetInstance().OnUpdate(uTimeDelta);

	// process attached process
	m_processManager.UpdateProcesses(uTimeDelta);

	dt = GetTickCount() - lastTime;
	lastTime += dt;
	if (dt > WARN_DIFF_TIME) {
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("OnUpdate (2): runtime %d"), dt);
	}

	// update moved actors in regions
	for (MapIndexList::iterator it = gGlobal.m_mapIndices.begin();
		it != gGlobal.m_mapIndices.end(); ++it)
	{
		it->second->UpdateRegions();
	}

	// send updated message character data to message service
	gGlobal.SendMsgChars();

	// synchronize to send updated information to client
	ViewManager* viewMan = gGlobal.m_viewRing->child;
	ViewManager* prevMan = NULL;
	// sync view dt time, ensure not zero
	UInt32 vdt = uTimeDelta * (gConfigure.viewRing + gGlobal.m_loadLevel) + 1;
	while (viewMan)
	{
		viewMan->SyncClient(vdt);
		prevMan = viewMan;
		viewMan = viewMan->m_nextView;
	}
	// cyclic next ring
	gGlobal.m_viewRing = gGlobal.m_viewRing->next;

	dt = GetTickCount() - lastTime;
	lastTime += dt;
	if (dt > WARN_DIFF_TIME)
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("OnUpdate (3) SyncClients: runtime %d"), dt);

	ClearDeadSession();

	// clear dead battles
	for (BattleList::iterator it = gGlobal.m_deadBattles.begin();
		it != gGlobal.m_deadBattles.end(); ++it)
	{
		DeleteNetObject(*it);
	}
	gGlobal.m_deadBattles.clear();

	if (m_manage_interval > gConfigure.manage_interval)
	{
		ServiceStatus_Proxy::UpdateStatus(gGlobal.pManagerPeer, RPCContext(), gConfigure.serviceId, SS_MAP_PLAYER_COUNT, gGlobal.m_playerCount);
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("player count %d"), gGlobal.m_playerCount);
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("memory status, %s"), 
			MemoryManager::GetInstance().GetStatus().c_str());
		m_manage_interval = 0;
	}
	m_manage_interval += uTimeDelta;

	SleepEx(UPDATE_INTERVAL, TRUE);

	if (gGlobal.m_bExit) 
	{
		if (m_stopTime <= uTimeDelta)
			Terminate(0);
		else
			m_stopTime -= uTimeDelta;
	}

}

VOID MapApplication::OnAccept(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID MapApplication::OnFull(CONST Listener* listener)
{
}

VOID MapApplication::OnOpen(CONST Listener* listener)
{
}

VOID MapApplication::OnOpen(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID MapApplication::OnClose(CONST Listener* listener)
{
}

VOID MapApplication::OnClose(CONST Listener* listener, CONST Peer* pPeer)
{
	if (gGlobal.pManagerPeer == pPeer)
	{
		SafeRelease(gGlobal.pManagerPeer);
		return;
	}

	Boolean bReconnect = BOOLEAN_FALSE;	
	if (!gGlobal.m_bExit)
	{
		// try reconnect service if suddenly closed
		bReconnect = TryReconnect(gConfigure, pPeer);
	}
	if (!bReconnect)
	{
		ServiceID srvId = UnregisterProxy(pPeer);
		if (srvId != INVALID_SERVICE_ID)
		{
			SessionIdList idList;
			SessionManager::GetInstance().GetSessionIds(srvId, idList);
			for (SessionIdList::iterator it = idList.begin(); it != idList.end(); ++it)
			{
				RPCContext context(*it);
				PROCEDURE_DO_RPC2(MapService, ReleaseSession, NULL, pPeer, context, 
					SessionID, context.sessionId, Boolean, BOOLEAN_FALSE)
			}
		}
	}
}
