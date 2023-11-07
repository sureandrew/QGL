//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "WorldService/WorldApplication.h"
//-- Library
#include "Proxy/Service/ServiceStatus.h"
#include "Stub/Service/WorldService.h"
#include "Proxy/Service/LogService.h"
#include "WorldService/Configure.h"
#include "WorldService/Global.h"
#include "Resource/ServerResManager.h"
#include "Resource/EventScheduleManager.h"
#include "Resource/ResEventSchedule.h"
#include "WorldService/RankingControl.h"
#include "WorldService/TaskScheduler.h"
#include "WorldService/ChargeShopControl.h"
#include "Stub/Logic/SellYuanBaoGroup.h"
#include "Proxy/Service/ObjectService.h"

#define UPDATE_INTERVAL				10
//#define EVENT_CHECK_INTERVAL		60000		// 1 min for statistic collection
#define EVENT_CHECK_INTERVAL		10000		// 1 min for statistic collection
#define TIMEZONECHANGE_INTERVAL		600000

UInt8 gTimezone = 0;
UInt32 gEventTime = 0;
UInt32 gTimezoneTime = 0;
UInt32 gCharRankPreUpdateTime = 0xFFFFFFFF;
UInt32 gLevelRankPreUpdateTime = 0xFFFFFFFF;
UInt32 gFamousRankPreUpdateTime = 0xFFFFFFFF;
UInt32 gEvilRankPreUpdateTime = 0xFFFFFFFF;
UInt32 gMoneyRankPreUpdateTime = 0xFFFFFFFF;

Boolean WorldApplication::OnCreate(VOID)
{
	if (!gGlobal.Initialize())
		return (BOOLEAN_FALSE);

	// set direct pointer to service peer
	for (ServiceMap::iterator it = gConfigure.services.begin(); it != gConfigure.services.end(); ++it)
	{
		if (it->second.type == SERVICE_OBJECT)
		{
			gGlobal.pObjectService = &it->second;
		}
		else if (it->second.type == SERVICE_MESSAGE)
		{
			gGlobal.pMessageService = &it->second;
		}
		else if (it->second.type == SERVICE_LOG)
		{
			gGlobal.pLogService = &it->second;
		}
	}

	if (!StartService(gConfigure))
		return (BOOLEAN_FALSE);

	ConnectServices(gConfigure);

	DEBUG_WARNDTL_IF_1(GLOBAL_LOGGER, gConfigure.eventTimeTwistMultiply != 1, _T("WorldApplication::OnCreate Warning (1) [event Time Twist Multiply is changed] multiply=%d"), gConfigure.eventTimeTwistMultiply);

	gTimezone = GetGameTime();
	//gTimezoneTime = CalcInitTimezoneWait();
	gTimezoneTime = gConfigure.timeZoneUpdatePeriod;

	if (gGlobal.m_pChargeShopControl)
		gGlobal.m_pChargeShopControl->InitChargeShop();

	if (gGlobal.m_rankingControl)
	{
		gGlobal.m_rankingControl->CreateCharacterRank();
		gGlobal.m_rankingControl->CreateRankByID(CHAR_RANK_TYPE_LEVEL);
		gGlobal.m_rankingControl->CreateRankByID(CHAR_RANK_TYPE_MONEY);
		gGlobal.m_rankingControl->CreateRankByID(CHAR_RANK_TYPE_FAMOUS);
		gGlobal.m_rankingControl->CreateRankByID(CHAR_RANK_TYPE_EVIL);
		gCharRankPreUpdateTime = 0;
		gLevelRankPreUpdateTime = 0;
		gMoneyRankPreUpdateTime = 0;
		gFamousRankPreUpdateTime = 0;
		gEvilRankPreUpdateTime = 0;
	}

	if (gGlobal.m_pSellYuanBaoGroup)
		gGlobal.m_pSellYuanBaoGroup->InitSellYuanBao();

	ObjectService_Proxy::GetNpcFlagsByLineId(gGlobal.pObjectService->pPeer,RPCContext(),DEFAULT_NPCFLAG_LINE_ID);
	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("WorldApplication::OnCreate : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	return (BOOLEAN_TRUE);
}

VOID WorldApplication::OnDispose(VOID)
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
	
	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("WorldApplication::OnCreate : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());

}

VOID WorldApplication::OnFatalError(Exception& ex)
{
}

VOID WorldApplication::OnUpdate(UInt32 uTimeDelta)
{
	if (uTimeDelta > 1000)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("TimeDelta %d"), uTimeDelta);
	}

	// process RPC message
	LocalRPCManager::GetInstance().OnUpdate(uTimeDelta);

	// process procedure
	ProcedureManager::GetInstance().OnUpdate(uTimeDelta);

	// process events
	Reuben::Simulation::ScheduleInstance::GetInstance().OnUpdate(uTimeDelta);

	ClearDeadSession();

	if (gGlobal.m_loginTime < (UInt32) gConfigure.loginInterval)
	{
		gGlobal.m_loginTime += uTimeDelta;
		while (gGlobal.m_loginTime >= (UInt32) gConfigure.loginInterval && 
				!gGlobal.m_waitQueue.empty())
		{
			SessionID sessionId = gGlobal.m_waitQueue.front();

			gGlobal.m_waitQueue.pop_front();
			WorldService_Stub::ReturnAccount(sessionId);
		}
	}

	// game timezone swap check
	gTimezoneTime += uTimeDelta;
	if( gTimezoneTime >= gConfigure.timeZoneUpdatePeriod )
	{
		UInt8 tz = GetGameTime();
		if (tz != gTimezone) {
			gTimezone = tz;
			if (gGlobal.m_pTaskScheduler)
				gGlobal.m_pTaskScheduler->TimeZoneChanged(tz);
		}
		gTimezoneTime -= gConfigure.timeZoneUpdatePeriod;
	}

	if (gEventTime >= EVENT_CHECK_INTERVAL)
	{
		gEventTime = 0;
	}
	else
	{
		gEventTime += uTimeDelta;
	}

	if (gCharRankPreUpdateTime > gConfigure.rankUpdatePeriod)
	{
		gGlobal.m_rankingControl->CreateCharacterRank();
		gCharRankPreUpdateTime = 0;
	}
	gCharRankPreUpdateTime += uTimeDelta;

	time_t now = ::time(NULL);
	if (isInOverTimeUpdateRank(CHAR_RANK_TYPE_LEVEL,now) && gLevelRankPreUpdateTime > gConfigure.LevelrankUpdatePeriod)
	{
		gGlobal.m_rankingControl->CreateRankByID(CHAR_RANK_TYPE_LEVEL);
		gLevelRankPreUpdateTime = 0;
	}
	gLevelRankPreUpdateTime += uTimeDelta;

	if (isInOverTimeUpdateRank(CHAR_RANK_TYPE_MONEY,now) && gMoneyRankPreUpdateTime > gConfigure.MoneyrankUpdatePeriod)
	{
		gGlobal.m_rankingControl->CreateRankByID(CHAR_RANK_TYPE_MONEY);
		gMoneyRankPreUpdateTime = 0;
	}
	gMoneyRankPreUpdateTime += uTimeDelta;

	if (isInOverTimeUpdateRank(CHAR_RANK_TYPE_FAMOUS,now) && gFamousRankPreUpdateTime > gConfigure.FamousrankUpdatePeriod)
	{
		gGlobal.m_rankingControl->CreateRankByID(CHAR_RANK_TYPE_FAMOUS);
		gFamousRankPreUpdateTime = 0;
	}
	gFamousRankPreUpdateTime += uTimeDelta;

	if (isInOverTimeUpdateRank(CHAR_RANK_TYPE_EVIL,now) && gEvilRankPreUpdateTime > gConfigure.EvilrankUpdatePeriod)
	{
		gGlobal.m_rankingControl->CreateRankByID(CHAR_RANK_TYPE_EVIL);
		gEvilRankPreUpdateTime = 0;
	}
	gEvilRankPreUpdateTime += uTimeDelta;

	static int timeNpcFlagUpdateTime = 0;
	int interval = GetGameTime() - timeNpcFlagUpdateTime;

	if( interval < 0|| interval >= gConfigure.timeNpcFlagUpdatePeriod )
	{
		timeNpcFlagUpdateTime = GetGameTime();
		gGlobal.UpdateGlobalNpcFlags();
	}

	if (m_manage_interval > gConfigure.manage_interval)
	{
		String s;
		Int32 total = 0;
		for (int i = 0; i < (int)gGlobal.m_lineCounts.size(); i++)
		{
			if (gGlobal.m_lineCounts[i] > 0)
			{
				s.AppendFormat(_T("L%02d=%d, "), i, gGlobal.m_lineCounts[i]);
				total += gGlobal.m_lineCounts[i];
			}
		}
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("WorldService: player count %d, %s"), total, s.c_str());

		UInt32 player_sum = 0;
		for (Index idxLine = C_INDEX(0); idxLine < gGlobal.m_lineCounts.size(); ++idxLine)
		{
			Int count = gGlobal.m_lineCounts[idxLine];
			if (count != -1)
			{
				LogService_Proxy::LogPlayerCount(gGlobal.pLogService->pPeer, RPCContext(), idxLine, count);
				player_sum += count;
			}
		}
		ServiceStatus_Proxy::UpdateStatus(gGlobal.pManagerPeer, RPCContext(), gConfigure.serviceId, SS_WORLD_PLAYER_COUNT, player_sum);
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("WorldApplication::OnUpdate : memory status, %s"),
			MemoryManager::GetInstance().GetStatus().c_str());
		m_manage_interval = 0;
	}
	m_manage_interval += uTimeDelta;

	SleepEx(UPDATE_INTERVAL, TRUE);

	if (gGlobal.m_bExit)
		Terminate(0);
}

VOID WorldApplication::OnAccept(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID WorldApplication::OnFull(CONST Listener* listener)
{
}

VOID WorldApplication::OnOpen(CONST Listener* listener)
{
}

VOID WorldApplication::OnOpen(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID WorldApplication::OnClose(CONST Listener* listener)
{
}

VOID WorldApplication::OnClose(CONST Listener* listener, CONST Peer* pPeer)
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
				PROCEDURE_DO_RPC2(WorldService, ReleaseSession, NULL, pPeer, context, 
					SessionID, context.sessionId, Boolean, BOOLEAN_FALSE)
			}
		}
		else
		{
			// may be map service lost
			gGlobal.OnMapServiceClose(pPeer);
		}
	}
}

UInt32 WorldApplication::CalcInitTimezoneWait()
{
	Int16 wDH = 0;

	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));
	gGlobal.GetLocalTime2(&st);
	
	UInt32 initwait = ((st.wHour % 2 * 60 + st.wMinute) * 60  + st.wSecond ) * 1000 + st.wMilliseconds;

	return initwait;
}

UInt8 WorldApplication::GetGameTime()
{
	Int16 wDH = 0;

	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));
	gGlobal.GetLocalTime2(&st);

	if ( gConfigure.GameDateTime.wHour > st.wHour )
		wDH = 24 - (gConfigure.GameDateTime.wHour - st.wHour);
	else
		wDH = (st.wHour - gConfigure.GameDateTime.wHour);

	UInt8 daynight = (UInt8)((((wDH % 2) * 60 + st.wMinute) / 10) % 12);

	return daynight;
}

Boolean WorldApplication::isInOverTimeUpdateRank(UInt8 rankType,time_t now)
{
	struct tm* ptime = localtime(&now);
	switch (rankType)
	{
	case CHAR_RANK_TYPE_LEVEL:
		{
			for(UInt i=0; i<gConfigure.LevelCount; i++)
			{
				if(gConfigure.pLevelRank[i].x == ptime->tm_hour && gConfigure.pLevelRank[i].y == ptime->tm_min)
					return true;
			}
			return false;
		}
	case CHAR_RANK_TYPE_MONEY:
		{
			for(UInt i=0; i<gConfigure.MoneyCount; i++)
			{
				if(gConfigure.pMoneyRank[i].x == ptime->tm_hour && gConfigure.pMoneyRank[i].y == ptime->tm_min)
					return true;
			}
			return false;
		}
	case CHAR_RANK_TYPE_FAMOUS:
		{
			for(UInt i=0; i<gConfigure.FamousCount; i++)
			{
				if(gConfigure.pFamousRank[i].x == ptime->tm_hour && gConfigure.pFamousRank[i].y == ptime->tm_min)
					return true;
			}
			return false;
		}
	case CHAR_RANK_TYPE_EVIL:
		{
			for(UInt i=0; i<gConfigure.EvilCount; i++)
			{
				if(gConfigure.pEvilRank[i].x == ptime->tm_hour && gConfigure.pEvilRank[i].y == ptime->tm_min)
					return true;
			}
			return false;
		}
	default:
		return false;
	}
}