//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/WorldEvent.h"
//-- Library
#include "Proxy/Service/MessageService_proxy.h"
#include "Stub/Service/WorldService.h"
#include "WorldService/Configure.h"
#include "WorldService/Global.h"
#include "WorldService/TaskScheduler.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResEventSchedule.h"

REGISTER_CLASSTYPE(CWorldEvent, WorldEvent_CLASSID);

#define MIN_SECOND			60
#define HOUR_SECOND			3600
#define DAY_SECOND			86400UL
#define DAY_WEEK			604800UL

typedef StlMap<NpcInfo, UInt16, std::less<NpcInfo>, 
		ManagedAllocator<std::pair<NpcInfo, UInt16> > > NpcInfoMap;

enum WE_MsgType
{
	WE_MSGTYPE_NONE = 0,
	WE_MSGTYPE_HORSE,
	WE_MSGTYPE_RUMOR_S,
	WE_MSGTYPE_RUMOR_L,
	WE_MSGTYPE_SYSTEM,
	WE_MSGTYPE_FACTION,
	WE_MSGTYPE_GUILD,
};

enum WE_PeriodType
{
	WE_PERIOD_NONE = 0,
	WE_PERIOD_DAY,
	WE_PERIOD_WEEK,
	WE_PERIOD_MONTH,
};

bool NpcInfo::operator == (const NpcInfo& b) const
{
	return (npcId == b.npcId && mapId == b.mapId);
}

bool NpcInfo::operator < (const NpcInfo& b) const
{
	return (npcId < b.npcId || (npcId == b.npcId && mapId < b.mapId));
}

String GetTimeStr(time_t& t)
{
	static char buf[256];
	strftime(buf, 256, "%Y/%m/%d %H:%M:%S", localtime(&t));
	String s(buf);
	return s;
}

CWorldEvent::CWorldEvent()
{
	m_pData = NULL;
	m_startSchTime = 0;
	m_endSchTime = 0;
	m_startEvtTime = 0;
	m_endEvtTime = 0;
	m_status = ST_NONE;
	m_bNeedSchedule = BOOLEAN_TRUE;
}

CWorldEvent::~CWorldEvent()
{
	CancelAllEvent();
}

void CWorldEvent::CalcSchedule(int nextInterval)
{
	if(nextInterval<0 || nextInterval>1)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("nextInterval can only be 0 or 1, nextInterval %d"), nextInterval);
		return;
	}

	if (m_pData == NULL)
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("pData null"));
		return;
	}

	time_t now = gGlobal.m_pTaskScheduler->GetTime();
	if (now < m_startSchTime)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("originally avoided case now < m_startSchTime, id %d, startSchTime %s"), m_pData->id, GetTimeStr(m_startSchTime).c_str());
	}
	if (now >= m_endSchTime)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("event schedule has ended, id %d, endSchTime %s"), m_pData->id, GetTimeStr(m_endSchTime).c_str());
		return;
	}

	if( m_pData->period==WE_PERIOD_NONE )
	{
		// non-periodic case
		m_startEvtTime = m_startSchTime;
		m_endEvtTime = m_startEvtTime + m_pData->length;
		if (m_endEvtTime >= m_endSchTime)
			m_endEvtTime = m_endSchTime;
		if (now >= m_endEvtTime)
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("event schedule has ended, id %d, endSchTime %s"), m_pData->id, GetTimeStr(m_endSchTime).c_str());
			return;
		}
	}
	else
	{
		struct tm* ptime = localtime(&m_pData->scheduleStart);
		ptime->tm_hour = (m_pData->startTime / HOUR_SECOND);
		ptime->tm_min = ((m_pData->startTime % HOUR_SECOND) / MIN_SECOND);
		ptime->tm_sec = 0;

		// periodic cases
		do {
			// calculate start event time
			switch (m_pData->period)
			{
			case WE_PERIOD_DAY:
				{
					if (m_pData->periodDay == 0)
						return;
					time_t interval = m_pData->periodDay * DAY_SECOND;
					time_t timeProcessed = max(now - m_startSchTime, 0); 
					m_startEvtTime = m_startSchTime + (timeProcessed / interval +
										nextInterval) * interval;
				}
				break;

			case WE_PERIOD_WEEK:
				{
					if (m_pData->periodWeek < 1 || m_pData->periodWeek > 7)
						return;

					int dayDiff = ((int) m_pData->periodWeek % 7) - ptime->tm_wday +
									nextInterval * 7;
					m_startEvtTime = mktime(ptime) + (dayDiff * DAY_SECOND);
				}
				break;

			case WE_PERIOD_MONTH:
				{
					if (m_pData->periodMonth < 1 || m_pData->periodMonth > 31)
						return;

					ptime->tm_mday = m_pData->periodMonth;
					if (nextInterval > 0) {		// try next month
						if (ptime->tm_mon < 12) {
							ptime->tm_mon ++;
						} else {
							ptime->tm_year ++;
							ptime->tm_mon = 0;
						}
					}
					m_startEvtTime = mktime(ptime);
				}
				break;

			default:
				return;
			}

			// calculate end event time
			m_endEvtTime = m_startEvtTime + m_pData->length;
			// event end time don't exceed schedule end time
			if (m_endEvtTime >= m_endSchTime)
				m_endEvtTime = m_endSchTime;
			nextInterval ++;			// try next event day
		} while (now >= m_endEvtTime);	// try next day if end time over
	}
	// show result schedule time
	TRACE_INFODTL_3(GLOBAL_LOGGER, 
		_T("CalcSchedule: eventId %d, startSchTime %s, endSchTime %s"), 
		GetEventId(), GetTimeStr(m_startSchTime).c_str(), 
		GetTimeStr(m_endSchTime).c_str());
	TRACE_INFODTL_3(GLOBAL_LOGGER, 
		_T("CalcSchedule: eventId %d, startEvtTime %s, endEvtTime %s"), 
		GetEventId(), GetTimeStr(m_startEvtTime).c_str(), 
		GetTimeStr(m_endEvtTime).c_str());
	SetEndTime((UInt32) m_endEvtTime);
}

Boolean CWorldEvent::InSchedule(time_t* t)
{
	time_t now;
	
	if (t == NULL) {
		now = gGlobal.m_pTaskScheduler->GetTime();
		t = &now;
	}

	return (*t >= m_startSchTime && *t < m_endSchTime);
}

Boolean CWorldEvent::IsRunning(time_t* t)
{
	time_t now;
	
	if (t == NULL) {
		now = gGlobal.m_pTaskScheduler->GetTime();
		t = &now;
	}

	return (*t >= m_startEvtTime && *t < m_endEvtTime);
}

void CWorldEvent::SetData(EventScheduleData* pData)
{
	if (pData == NULL)
		return;

	m_pData = pData;

	// calculate schedule date and time
	m_startSchTime = m_pData->scheduleStart;
	m_startSchTime += m_pData->startTime;
	m_endSchTime = m_pData->scheduleEnd - DAY_SECOND + 1 + 
					m_pData->startTime + m_pData->length;

	SetInfo((UInt16) m_pData->id, m_pData->type, m_pData->mapType, (UInt32) m_endSchTime);
	switch (m_pData->type)
	{
	case WE_TYPE_BUFF:
		{
			TRACE_ENSURE(m_pData->buffData);
			UInt16Vector map_ids;
			UInt16Vector buff_ids;

			for (UInt32Vector::const_iterator it = m_pData->map.begin();
				it != m_pData->map.end(); ++it)
				map_ids.push_back(*it);
			for (UInt32Vector::const_iterator it = m_pData->buffData->buff.begin();
				it != m_pData->buffData->buff.end(); ++it)
				buff_ids.push_back(*it);
			SetMapList(map_ids);
			SetBuffEvent(buff_ids, m_pData->buffData->buffPeriod, 
				(Boolean) (m_pData->buffData->buffClear > 0), 
				(Boolean) (m_pData->buffData->removalOnLeaveMap > 0));
		}
		break;

	case WE_TYPE_NPC:
		{
			TRACE_ENSURE(m_pData->npcData);
			/*
			// position information list
			if (!m_pData->npcData->npcMapData.empty()) {
				PosInfo posInfo;
				PosInfoVec posInfoList;
				EventScheduleMapDataVector &mapVec = m_pData->npcData->npcMapData;
				EventScheduleMapData* pMap;
				for (EventScheduleMapDataVector::iterator it = mapVec.begin();
					it != mapVec.end(); ++it)
				{
					pMap = *it;
					if (pMap) {
						posInfo.mapId = pMap->mapId;
						posInfo.x = pMap->x;
						posInfo.y = pMap->y;
						posInfoList.push_back(posInfo);
					}
				}
				if (!posInfoList.empty())
					SetNpcPosList(posInfoList);
			}
			*/
			SetNpcLiveTime(m_pData->npcData->npcLiveTime / 
							gConfigure.eventTimeTwistMultiply);
		}
		break;

	case WE_TYPE_MAP:
		{
			UInt16Vector map_ids;
			for (UInt32Vector::const_iterator it = m_pData->map.begin();
				it != m_pData->map.end(); ++it)
				map_ids.push_back(*it);
			SetMapList(map_ids);
		}
		break;
	case WE_TYPE_SCRIPT:
		{
			SetScript(m_pData->script);
		}
		break;
	default:
		break;
	}
}

UInt16 CWorldEvent::RandMapId()
{
	if (m_pData->type == WE_TYPE_NPC && m_pData->npcData && 
		!m_pData->npcData->npcMapData.empty())
	{
		EventScheduleMapDataVector &mapVec = m_pData->npcData->npcMapData;
		EventScheduleMapData *pData = mapVec[GetRand((UInt16) mapVec.size())];
		return (pData ? pData->mapId : 0);
	} else if (!m_pData->map.empty()) {
		return m_pData->map[GetRand((UInt16) m_pData->map.size())];
	} else {
		return 0;
	}
}

void CWorldEvent::RandGenNpc(UInt8 line_id)
{
	if (m_pData == NULL || m_pData->type != WE_TYPE_NPC || 
		line_id >= gGlobal.GetTotalLines())
		return;

	TRACE_ENSURE(m_pData->npcData);
	UInt16 npcIdNum = (UInt16) m_pData->npcData->npcId.size();
	if (npcIdNum == 0)
		return;

	// random NPC list
	NpcInfo npcInfo;
	NpcInfoVec npcList;
	UInt16 num = GetRand((int) m_pData->npcData->npcMin, 
						(int) m_pData->npcData->npcMax);
	for (UInt16 i = 0; i < num; i++) {
		npcInfo.npcId = m_pData->npcData->npcId[GetRand(npcIdNum)];
		npcInfo.mapId = RandMapId();
		npcList.push_back(npcInfo);
	}

	if (!npcList.empty()) {
		SetNpcList(npcList);

		// set the npc increase number in each line
		UInt16Vector lineCounts;
		lineCounts.push_back(0);			// line_id 0
		for (UInt8 i = 1; i < gGlobal.GetTotalLines(); i++) {
			if (gGlobal.GetLineCount(i) >= 0 && num > m_npcLives[i]) {
				lineCounts.push_back(num - m_npcLives[i]);
				m_npcLives[i] = num;
			} else {
				lineCounts.push_back(0);
			}
		}
		SetNpcLineCounts(lineCounts);

		// set npc life time not excess the event time
		UInt32 realLifeTime = m_pData->npcData->npcLiveTime / gConfigure.eventTimeTwistMultiply;
		UInt32 lifeTime = realLifeTime;
		if (m_pData->repeat)
		{
			lifeTime = CalcRepeatDiff(realLifeTime);
		}
		else
		{
			time_t now = gGlobal.m_pTaskScheduler->GetTime();
			if (now < m_endEvtTime && (UInt32) (m_endEvtTime - now) < realLifeTime)
				lifeTime = (UInt32) (m_endEvtTime - now);
		}
		SetNpcLiveTime(lifeTime);
	}
}

void CWorldEvent::Schedule(Boolean bForce)
{
	if (!bForce && !m_bNeedSchedule)
		return;

	CalcSchedule(bForce ? 1 : 0);
	m_bNeedSchedule = BOOLEAN_FALSE;

	time_t now = gGlobal.m_pTaskScheduler->GetTime();

	// start event when within schedule time
	if (m_status == ST_NONE) {
		if (now < m_startEvtTime) {
			UInt32 t = (UInt32) (m_startEvtTime - now) * 1000 /
				gConfigure.eventTimeTwistMultiply;
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Schedule: eventId %d, start after %ds"),
				GetEventId(), t / 1000);
			RaiseUniqEventID(EVT_START, t);
		} else if (now >= m_startEvtTime && now < m_endEvtTime) {
			// schedule repeat event except map type event
			if (m_pData->type != WE_TYPE_MAP && m_pData->repeat > 0) {
				// previous repeat (theoretical calculation)
				UInt32 dt = (UInt32)(now - (m_startEvtTime + (now - m_startEvtTime) / 
							m_pData->repeat * m_pData->repeat));
				UInt32 repeatLife = 0;
				if (m_pData->type == WE_TYPE_BUFF && m_pData->buffData)
					repeatLife = m_pData->buffData->buffPeriod;
				else if (m_pData->type == WE_TYPE_NPC && m_pData->npcData)
					repeatLife = m_pData->npcData->npcLiveTime;

				// if previous repeat-event does not end, do it now.
				if (dt < repeatLife) {
					RaiseUniqEventID(EVT_START, 0);
				} else {
					// if the next repeat-event exists, schedule it.
					time_t nextRepeat = m_startEvtTime + ((now - m_startEvtTime) /
										m_pData->repeat + 1) * m_pData->repeat;
					if (nextRepeat < m_endEvtTime) {
						UInt32 t = (UInt32) (nextRepeat - now) * 1000 / 
							gConfigure.eventTimeTwistMultiply;
						TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Schedule: eventId %d, start after %ds"),
							GetEventId(), t / 1000);
						RaiseUniqEventID(EVT_START, t);
					} else {
						m_status = ST_STOPPED;
					}
				}
			}
			else
			{
				RaiseUniqEventID(EVT_START, 0);
			}
		} else if (now > m_endEvtTime) {
			m_status = ST_STOPPED;
		}
	}
}

UInt32 CWorldEvent::CalcRepeatDiff(UInt32 timeLen)
{
	// previous repeat time corresponding to event start time
	UInt32 diffTime = timeLen;
	time_t now = gGlobal.m_pTaskScheduler->GetTime();
	UInt32 dt =  (UInt32)(now - (m_startEvtTime + ((now - m_startEvtTime) / 
		m_pData->repeat * m_pData->repeat)));
	// This repeat is feasible if a) now is before endEvtTime, and
	// b) prev. repeat has not ended yet.
	if (now < m_endEvtTime && dt < timeLen)
	{
		// remaining time since prev. repeat to now, and up to endEvtTime
		diffTime = timeLen - dt;
		if (diffTime > (UInt32)(m_endEvtTime - now))
			diffTime = (UInt32) (m_endEvtTime - now);
	}

	return diffTime;
}

void CWorldEvent::Prepare()
{
	if (m_pData == NULL)
		return;

	RPCMask mask;
	mask.MaskBit(infoMask);
	switch (m_pData->type)
	{
	case WE_TYPE_BUFF:
		{
			mask.MaskBits(buffEventMask, mapListMask, -1);
			
			UInt32 periodTime = m_pData->buffData->buffPeriod /
								gConfigure.eventTimeTwistMultiply;
			time_t now = gGlobal.m_pTaskScheduler->GetTime();
			if (periodTime == 0 ||
				now < m_endEvtTime && (time_t) periodTime < m_endEvtTime - now)
				periodTime = (UInt32) (m_endEvtTime - now);
			SetBuffPeriod(periodTime);
		}
		break;

	case WE_TYPE_NPC:
		{
			mask.MaskBits(npcListMask, npcLiveTimeMask, -1);
			// set npc live count
			m_npcLives.clear();
			for (UInt8 i = 0; i < gGlobal.GetTotalLines(); i++)
				m_npcLives.push_back(0);
			RandGenNpc();
		}
		break;

	case WE_TYPE_MAP:
		{
			mask.MaskBit(mapListMask);
			OpenMap(BOOLEAN_TRUE);
		}
		break;
	case WE_TYPE_SCRIPT:
		{
			mask.MaskBit(scriptMask);
		}
		break;
	}

	ClearUpdateMask();
	SetSendMask(mask);
}

void CWorldEvent::SendMessage(UInt8 msgType, PCSTRING msg)
{
	LPCPEER pPeer = gGlobal.pMessageService->pPeer;
	RPCContext context;

	if (pPeer == NULL)
		return;

	time_t now = gGlobal.m_pTaskScheduler->GetTime();
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("SendMessage: time %s, type %d, msg %s"),
		GetTimeStr(now).c_str(), msgType, msg);
	switch (msgType)
	{
	case WE_MSGTYPE_HORSE:
		MessageService_Proxy::SendHorseLightMessage(pPeer, context, msg, 0, 1);
		break;

	case WE_MSGTYPE_RUMOR_S:
		MessageService_Proxy::SendRumorMessage(pPeer, context, msg, 0, 1);
		break;

	case WE_MSGTYPE_RUMOR_L:
		MessageService_Proxy::SendRumorMessage(pPeer, context, msg, 0, 0);
		break;

	case WE_MSGTYPE_SYSTEM:
		MessageService_Proxy::SendSystemMessage(pPeer, context, msg, 0);
		break;

	case WE_MSGTYPE_FACTION:
		break;

	case WE_MSGTYPE_GUILD:
		break;
	}
}

void CWorldEvent::NotifyNpcDead(UInt32 npcId, UInt8 line_id)
{
	if (line_id == 0 || line_id >= (UInt8) m_npcLives.size())
		return;

	if (m_pData->type == WE_TYPE_NPC && m_npcLives[line_id] > 0 &&
		m_pData->npcData)
	{
		//TRACE_INFODTL_3(GLOBAL_LOGGER, 
		//	_T("NotifyNpcDead: eventId %d, npcId %d, line_id %d"),
		//	GetEventId(), npcId, line_id);
		m_npcLives[line_id]--;

		if (m_pData->msgKill[0] != _T('\0'))
		{
			String s;
			s.Format(m_pData->msgKill, m_npcLives[line_id]);
			SendMessage(m_pData->msgType, s.c_str());
		}

		if (m_npcLives[line_id] == 0 && m_pData->npcData->npcReborn == 1)
			PeriodEvent(line_id, BOOLEAN_FALSE);
	}
}

void CWorldEvent::OpenMap(Boolean bOpen)
{
	for (UInt32Vector::const_iterator it = m_pData->map.begin();
		it != m_pData->map.end(); ++it)
	{
		gGlobal.SetMapOpen(*it, bOpen);
	}
}

void CWorldEvent::StartEvent()
{
	if (m_pData == NULL || m_status != ST_NONE)
		return;

	time_t now = gGlobal.m_pTaskScheduler->GetTime();

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("StartEvent: eventId %d, time %s, %s"), 
		GetEventId(), GetTimeStr(now).c_str(), GetDetail().c_str());

	// change status
	m_status = ST_STARTED;

	Prepare();				// prepare data

	// send to all map services
	if (m_pData->type != WE_TYPE_NONE) {
		RPCContext context;
		NetGroup grp;
		LPCPEER pPeer;

		grp.Add(this);
		for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); 
			it != gGlobal.m_mapPeers.end(); ++it)
		{
			pPeer = *it;
			if (pPeer)
			{
				bool bCanStart = false;
				if (m_pData->line != 0)
				{
					for (MapIndexList::iterator it2 = gGlobal.m_mapIndices.begin(); 
						it2 != gGlobal.m_mapIndices.end(); ++it2)
					{
						MapInfo& mInfo = it2->second;
						if (mInfo.pPeer == pPeer)
						{
							if (mInfo.index.line_id == m_pData->line)
								bCanStart = true;
							else
							{
								TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" skip line : %d"), mInfo.index.line_id);
							}
							break;
						}
					}
				}
				else
					bCanStart = true;

				if (bCanStart)
					WorldService_Stub::CB_StartEvent(pPeer, context, &grp);
			}
		}
	}

	// send to message service
	if (m_pData->msgStart[0] != _T('\0'))
		SendMessage((UInt8) m_pData->msgType, m_pData->msgStart);

	// schedule repeat event
	if (m_pData->repeat > 0 && (now + (time_t) m_pData->repeat < m_endEvtTime)) {
		UInt32 t = (UInt32) m_pData->repeat * 1000 / 
					gConfigure.eventTimeTwistMultiply;
		time_t nextRepeat = m_startEvtTime + ((now - m_startEvtTime) / m_pData->repeat + 1) * m_pData->repeat;
		if( nextRepeat < m_endEvtTime ) {
			t = (UInt32) (nextRepeat - now) * 1000 / 
				gConfigure.eventTimeTwistMultiply;
		//	RaiseUniqEventID(EVT_START, t);
		}
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Schedule: eventId %d, repeat after %d ms"),
			GetEventId(), t);
		RaiseUniqEventID(EVT_PERIOD, t);
	}

	// schedule stop event
	if (now < m_endEvtTime) {
		UInt32 t = (UInt32) (m_endEvtTime - now) * 1000 / 
					gConfigure.eventTimeTwistMultiply;
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Schedule: eventId %d, stop after %d ms"),
			GetEventId(), t);
		RaiseUniqEventID(EVT_STOP, t);
	}
}

void CWorldEvent::StopEvent(Boolean bForce)
{
	time_t now = gGlobal.m_pTaskScheduler->GetTime();

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("StopEvent: eventId %d, time %s"), 
		GetEventId(), GetTimeStr(now).c_str());

	// change status
	m_status = ST_STOPPED;

	// send to all map services
	if (m_pData->type != WE_TYPE_NONE) {
		RPCContext context;
		UInt32Vector eventIds;
		LPCPEER pPeer;

		if (m_pData->type == WE_TYPE_MAP)
			OpenMap(BOOLEAN_FALSE);

		eventIds.push_back(GetEventId());
		for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); 
			it != gGlobal.m_mapPeers.end(); ++it)
		{
			pPeer = *it;
			if (pPeer)
			{
				bool bCanStop = false;
				if (m_pData->line != 0)
				{
					for (MapIndexList::iterator it2 = gGlobal.m_mapIndices.begin(); 
						it2 != gGlobal.m_mapIndices.end(); ++it2)
					{
						MapInfo& mInfo = it2->second;
						if (mInfo.pPeer == pPeer)
						{
							if (mInfo.index.line_id == m_pData->line)
								bCanStop = true;
							else
							{
								TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" skip line : %d"), mInfo.index.line_id);
							}
							break;
						}
					}
				}
				else
					bCanStop = true;

				if (bCanStop)
					WorldService_Stub::CB_StopEvent(pPeer, context, eventIds, bForce);
			}
		}
	}

	// send to message service
	if (m_pData->msgEnd[0] != _T('\0'))
		SendMessage((UInt8) m_pData->msgType, m_pData->msgEnd);

	// if event still in schedule date, reschedule it
	if (InSchedule(&now) && InSchedule(&m_endEvtTime)) {
		m_status = ST_NONE;
		Schedule(BOOLEAN_TRUE);
	}
}

void CWorldEvent::PeriodEvent(UInt8 line_id, Boolean bSchedule)
{
	if (m_pData == NULL || m_status != ST_STARTED)
		return;

	time_t now = gGlobal.m_pTaskScheduler->GetTime();

	SetUpdateMask(infoMask);
	if (m_pData->type == WE_TYPE_NPC)	// regenerate NPC
		RandGenNpc(line_id);

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("PeriodEvent: eventId %d, time %s, %s"), 
		GetEventId(), GetTimeStr(now).c_str(), GetDetail().c_str());

	SetSendMask(GetUpdateMask());

	// send to all map services only for npc event
	if (m_pData->type == WE_TYPE_NPC) {
		RPCContext context;
		NetGroup grp;
		LPCPEER pPeer;

		grp.Add(this);
		for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); 
			it != gGlobal.m_mapPeers.end(); ++it)
		{
			pPeer = *it;
			if (pPeer)
			{
				bool bCanStart = false;
				if (m_pData->line != 0)
				{
					for (MapIndexList::iterator it2 = gGlobal.m_mapIndices.begin(); 
						it2 != gGlobal.m_mapIndices.end(); ++it2)
					{
						MapInfo& mInfo = it2->second;
						if (mInfo.pPeer == pPeer)
						{
							if (mInfo.index.line_id == m_pData->line)
								bCanStart = true;
							else
							{
								TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" skip line : %d"), mInfo.index.line_id);
							}
							break;
						}
					}
				}
				else
					bCanStart = true;

				if (bCanStart)
					WorldService_Stub::CB_PeriodEvent(pPeer, context, &grp, line_id);
			}
		}
	}

	// send to message service
	if (m_pData->msgPeriod[0] != _T('\0'))
		SendMessage((UInt8) m_pData->msgType, m_pData->msgPeriod);
	
	ClearUpdateMask();

	// schedule repeat event again if necessary
	if (bSchedule && m_pData->repeat > 0 && 
		(now + (time_t) m_pData->repeat < m_endEvtTime))
	{
		UInt32 t = (UInt32) m_pData->repeat * 1000 / 
					gConfigure.eventTimeTwistMultiply;
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Schedule: eventId %d, repeat after %d ms"),
			GetEventId(), t);
		RaiseUniqEventID(EVT_PERIOD, t);
	}
}

String CWorldEvent::GetDetail()
{
	String s;

	if (m_pData == NULL)
		return s;

	switch (m_pData->type)
	{
	case WE_TYPE_BUFF:
		{
			TRACE_ENSURE(m_pData->buffData);
			s.Format(_T("Buff Event: period %d, buff_ids %s, map_ids %s"),
				GetBuffPeriod(), GetVectorString(GetBuffList()).c_str(), 
				GetVectorString(GetMapList()).c_str());
		}
		break;

	case WE_TYPE_NPC:
		{
			TRACE_ENSURE(m_pData->npcData);
			s.Format(_T("Npc Event: lifetime %d, npc "), GetNpcLiveTime());
			const NpcInfoVec &npcList = GetNpcList();
			NpcInfoMap npcInfoMap;

			for (NpcInfoVec::const_iterator it = npcList.begin(); it != npcList.end(); ++it)
			{
				const NpcInfo &npcInfo = *it;
				npcInfoMap[npcInfo] ++;
			}
			for (NpcInfoMap::iterator it = npcInfoMap.begin(); it != npcInfoMap.end(); ++it)
			{
				const NpcInfo &npcInfo = it->first;
				UInt16 repeat = it->second;
				if (repeat > 1)
					s.AppendFormat(_T("%d(%d)x%d, "), npcInfo.npcId, npcInfo.mapId, repeat);
				else
					s.AppendFormat(_T("%d(%d), "), npcInfo.npcId, npcInfo.mapId);
			}
		}
		break;

	case WE_TYPE_MAP:
		{
			s.Format(_T("Map Event: map_ids %s"), GetVectorString(GetMapList()).c_str());
		}
		break;
	}

	return s;
}

void CWorldEvent::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_START:				// world event start
		StartEvent();
		break;

	case EVT_STOP:				// world event stop
		StopEvent();
		break;

	case EVT_PERIOD:			// world event restart
		PeriodEvent();
		break;
	}
}
