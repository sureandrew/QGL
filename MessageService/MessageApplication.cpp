//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "MessageService/MessageApplication.h"
//-- Library
#include "Proxy/Service/MapService_proxy.h"
#include "Proxy/Service/ServiceStatus.h"
#include "Stub/Logic/Channel.h"
#include "Stub/Service/MessageService_stub.h"
#include "MessageService/Configure.h"
#include "MessageService/Global.h"
#include "MessageService/GuildControl.h"
#include "MessageService/BusinessShopControl.h"

#define UPDATE_INTERVAL 10

enum MSG_SCHEDULE_TYPE
{
	MSG_SCHEDULE_TYPE_HL = 0,
	MSG_SCHEDULE_TYPE_GM,
	MSG_SCHEDULE_TYPE_RUMOR_CENTER,
	MSG_SCHEDULE_TYPE_RUMOR_LEFTDWON_ONLY,
};

Boolean MessageApplication::OnCreate(VOID)
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
		case SERVICE_LOG:
			if (gGlobal.pLogService == NULL)
			{
                gGlobal.pLogService = &it->second;
			}
			break;
		}
	}

	if (!StartService(gConfigure))
		return (BOOLEAN_FALSE);

	ConnectServices(gConfigure);

	//get all guildwar guild, max 16 guilds
	if (gGlobal.m_guildControl)
		gGlobal.m_guildControl->GetGuildByPvPGroup();

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("MessageApplication::OnCreate : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	return (BOOLEAN_TRUE);
}

VOID MessageApplication::OnDispose(VOID)
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
	CBusinessShopControl::DestroyInstance();

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("MessageApplication::OnCreate : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());
}

VOID MessageApplication::OnFatalError(Exception& ex)
{
	gGlobal.DestroyGameContext();
}

VOID MessageApplication::UpDateMsgSchedule(UInt32 uTimeDelta)
{
	MsgScheduleMap::iterator Itr = gGlobal.g_MsgScheduleMap.begin();
	
	while (Itr != gGlobal.g_MsgScheduleMap.end())
	{
		
		if (Itr->second.startTime == 0 &&
			Itr->second.sendTimeCount > Itr->second.period)
		{
			if (Itr->second.msgType == MSG_SCHEDULE_TYPE_HL)
				MessageService_Stub::SendHorseLightMessage(NULL, RPCContext(), Itr->second.msg, Itr->second.srcID, Itr->second.count);
			else if (Itr->second.msgType == MSG_SCHEDULE_TYPE_GM)
				MessageService_Stub::SendSystemMessage(NULL, RPCContext(), Itr->second.msg, Itr->second.srcID);
			else if (Itr->second.msgType == MSG_SCHEDULE_TYPE_RUMOR_CENTER)
				MessageService_Stub::SendRumorMessage(NULL, RPCContext(), Itr->second.msg, Itr->second.srcID, 0);
			else if (Itr->second.msgType == MSG_SCHEDULE_TYPE_RUMOR_LEFTDWON_ONLY)
				MessageService_Stub::SendRumorMessage(NULL, RPCContext(), Itr->second.msg, Itr->second.srcID, 1);

			if (Itr->second.periodCount > 0)
			{
				Itr->second.periodCount --;
				if (Itr->second.periodCount == 0)
				{
					Itr = gGlobal.g_MsgScheduleMap.erase(Itr);
					continue;
				}
			}
			Itr->second.sendTimeCount = 0;
		}

		if (Itr->second.startTime > uTimeDelta)
			Itr->second.startTime -= uTimeDelta;
		else
			Itr->second.startTime = 0;

		Itr->second.sendTimeCount += uTimeDelta;
		++Itr;
	}
}

VOID MessageApplication::OnUpdate(UInt32 uTimeDelta)
{
	if (uTimeDelta > 1000)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("TimeDelta %d"), uTimeDelta);
	}

	// process RPC message
	LocalRPCManager::GetInstance().OnUpdate(uTimeDelta);

	// process procedure
	ProcedureManager::GetInstance().OnUpdate(uTimeDelta);

	// process attached process
	//m_processManager.UpdateProcesses(uTimeDelta);

	// process events
	Reuben::Simulation::ScheduleInstance::GetInstance().OnUpdate(uTimeDelta);

	//check whether need to send horse light message
	UInt32 CurrTime = ::GetTickCount();
	//UpDateHorseLightMessage(CurrTime);
	UpDateChannelLst(CurrTime);
	UpDateMsgSchedule(uTimeDelta);
	CBusinessShopControl::GetInstance().OnUpdate(CurrTime);

	ClearDeadSession();

	// decrease shutdown time
	if (gGlobal.m_downTime > 0)
	{
		if (gGlobal.m_downTime > uTimeDelta)
			gGlobal.m_downTime -= uTimeDelta;
		else
			gGlobal.m_downTime = 0;
	}

	if (m_manage_interval > gConfigure.manage_interval)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("MessageApplication::OnUpdate : memory status, %s"),
			MemoryManager::GetInstance().GetStatus().c_str());
		m_manage_interval = 0;
	}
	m_manage_interval += uTimeDelta;

	SleepEx(UPDATE_INTERVAL, TRUE);

	if (gGlobal.m_bExit)
		Terminate(0);
}

VOID MessageApplication::OnAccept(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID MessageApplication::OnFull(CONST Listener* listener)
{
}

VOID MessageApplication::OnOpen(CONST Listener* listener)
{
}

VOID MessageApplication::OnOpen(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID MessageApplication::OnClose(CONST Listener* listener)
{
}

VOID MessageApplication::OnClose(CONST Listener* listener, CONST Peer* pPeer)
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
				PROCEDURE_DO_RPC2(MessageService, ReleaseSession, NULL, pPeer, context, 
					SessionID, context.sessionId, Boolean, BOOLEAN_FALSE)
			}
		}
	}
}

VOID MessageApplication::UpDateHorseLightMessage(UInt32 uTimeDelta)
{
	if (uTimeDelta - gGlobal.m_PreHLMsgUpdateTime > HLMsgSendTime)
	{
		//send system message if there is no client msg or client msg send time more than SysHLMsgPerClientHLMsg
		if (gGlobal.ClientHorseLightMessageQuene.size() == 0 ||
			gGlobal.m_NumOfClientHLMsg >= SysHLMsgPerClientHLMsg)
		{
			//find system message 
			UInt Index = gGlobal.m_SysHLMsgIndex;
			for (; Index < MaxHorseLightMess; Index ++)
			{
				if (!gGlobal.SysHorseLightMessage[Index].empty())
				{
					MessageService_Stub::SendHorseLightMessage(NULL ,RPCContext(), gGlobal.SysHorseLightMessage[Index], 0, 5);
					break;
				}
			}

			if (Index >= MaxHorseLightMess)
			{
				for (Index = 0; Index < gGlobal.m_SysHLMsgIndex; Index ++)
				{
					if (!gGlobal.SysHorseLightMessage[Index].empty())
					{
						MessageService_Stub::SendHorseLightMessage(NULL ,RPCContext(), gGlobal.SysHorseLightMessage[Index], 0, 5);
						break;
					}
				}
			}

			gGlobal.m_CurrHorseLightMsg = gGlobal.SysHorseLightMessage[Index];
			gGlobal.m_SysHLMsgIndex = Index + 1;

			gGlobal.m_NumOfClientHLMsg = 0;
		}
		else //send client horse light message
		{
			StringVector::iterator Itr = gGlobal.ClientHorseLightMessageQuene.begin();
			if (Itr != gGlobal.ClientHorseLightMessageQuene.end())
			{
				gGlobal.m_CurrHorseLightMsg = *Itr;
				MessageService_Stub::SendHorseLightMessage(NULL ,RPCContext(), *Itr, 0, 5);
				gGlobal.ClientHorseLightMessageQuene.erase(Itr);

				gGlobal.m_NumOfClientHLMsg ++;
			}
		}
		gGlobal.m_PreHLMsgUpdateTime = uTimeDelta;
	}
}

VOID MessageApplication::UpDateChannelLst(UInt32 uTimeDelta)
{
	CChannel* pChannel;
	for (ChannelNameMap::iterator it = gGlobal.m_MapChannel.begin();
		it != gGlobal.m_MapChannel.end();)
	{
		pChannel = it->second;
		if (pChannel && pChannel->NumberOfPlayer() == 0 &&
			pChannel->GetChannelType() != CHANNELTYPE_GLOBAL &&
			pChannel->GetChannelType() != CHANNELTYPE_OFFICIAL &&
			pChannel->GetChannelType() != CHANNELTYPE_SPEAKER)
		{
			DeleteNetObject(pChannel);
			it = gGlobal.m_MapChannel.erase(it);
		} else {
			++it;
		}
	}
}
