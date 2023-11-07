//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "WorldService/TaskScheduler.h"
//-- Library
#include "Proxy/Service/MessageService_proxy.h"
#include "Stub/Logic/WorldEvent.h"
#include "Stub/Service/WorldService.h"
#include "WorldService/Configure.h"
#include "WorldService/Global.h"
#include "Resource/ServerResManager.h"

CTaskScheduler::CTaskScheduler()
{
	m_startTime = 0;
}

CTaskScheduler::~CTaskScheduler()
{
	CWorldEvent *pEvent;
	for (WorldEventMap::iterator it = m_eventMap.begin(); 
		it != m_eventMap.end(); ++it)
	{
		pEvent = it->second;
		if (pEvent)
			DeleteNetObject(pEvent);
	}
	m_eventMap.clear();
}

time_t CTaskScheduler::GetTime()
{
	if (gConfigure.eventTimeTwistMultiply > 1)
		return ((::time(NULL) - m_startTime) * gConfigure.eventTimeTwistMultiply +
				m_startTime);
	else
		return ::time(NULL);
}

CWorldEvent* CTaskScheduler::GetEvent(UInt16 eventId)
{
	WorldEventMap::iterator it = m_eventMap.find(eventId);
	if (it != m_eventMap.end())
		return it->second;
	else
		return NULL;
}

void CTaskScheduler::LoadEvent()
{
	EventScheduleMap eventMap;
	EventScheduleData* pData;
	CWorldEvent* pEvent;

	Stop();
	gGlobal.m_resource->GetEventScheduleVector(eventMap);
	for (EventScheduleMap::iterator it = eventMap.begin(); it != eventMap.end(); ++it)
	{
		pData = it->second;
		if (pData == NULL)
			continue;
		pEvent = CWorldEvent::NewInstance();
		TRACE_ENSURE(pEvent);
		pEvent->SetData(pData);
		m_eventMap[pEvent->GetEventId()] = pEvent;
	}

}

void CTaskScheduler::Start(LPCPEER pPeer)
{
	CWorldEvent* pEvent;
	//NetGroup grp;
	time_t now;
	RPCContext context;

	::time(&now);
	if (m_startTime == 0) {
		m_startTime = now;
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TaskScheduler Start: time %d"),
			m_startTime);
	}

	for (WorldEventMap::iterator it = m_eventMap.begin(); 
		it != m_eventMap.end(); ++it)
	{
		pEvent = it->second;
		if (pEvent == NULL)
			continue;

		pEvent->Schedule(BOOLEAN_FALSE);
		//if (pEvent->GetEventType() != 0 && pEvent->IsRunning(&now)) {
		//	pEvent->Prepare();
		//	grp.Add(pEvent);
		//}
	}

	//if (grp.GetCount() > 0) {
	//	if (pPeer) {
	//		WorldService_Stub::CB_StartEvent(pPeer, context, &grp);
	//	} else {
	//		for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); 
	//			it != gGlobal.m_mapPeers.end(); ++it)
	//		{
	//			pPeer = *it;
	//			if (pPeer)
	//				WorldService_Stub::CB_StartEvent(pPeer, context, &grp);
	//		}
	//	}
	//}
}

void CTaskScheduler::Stop(LPCPEER pPeer)
{
	if (m_eventMap.empty())
		return;

	RPCContext context;
	UInt32Vector eventIds;
	CWorldEvent* pEvent;
	time_t now = GetTime();

	for (WorldEventMap::iterator it = m_eventMap.begin(); 
		it != m_eventMap.end(); ++it)
	{
		pEvent = it->second;
		if (pEvent == NULL)
			continue;
		if (pEvent->IsRunning(&now))
			eventIds.push_back(pEvent->GetEventId());
		DeleteNetObject(pEvent);
	}
	m_eventMap.clear();

	if (!eventIds.empty()) {
		if (pPeer) {
			WorldService_Stub::CB_StopEvent(pPeer, context, eventIds,
				BOOLEAN_TRUE);
		} else {
			for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); 
				it != gGlobal.m_mapPeers.end(); ++it)
			{
				pPeer = *it;
				if (pPeer)
					WorldService_Stub::CB_StopEvent(pPeer, context, eventIds,
						BOOLEAN_TRUE);
			}
		}
	}
}

void CTaskScheduler::Reschedule()
{
	CWorldEvent* pEvent;
	for (WorldEventMap::iterator it = m_eventMap.begin(); 
		it != m_eventMap.end(); ++it)
	{
		pEvent = it->second;
		if (pEvent)
			pEvent->Schedule(BOOLEAN_FALSE);
	}
}

void CTaskScheduler::TimeZoneChanged(UInt8 gameTime)
{
	LPCPEER pPeer;
	RPCContext context;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TaskScheduler TimeZoneChanged: gameTime %d"),
		gameTime);

	for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); 
		it != gGlobal.m_mapPeers.end(); ++it)
	{
		pPeer = *it;
		if (pPeer)
			WorldService_Stub::CB_TimeZoneChanged(pPeer, context, gameTime);
	}
}