#pragma once
#ifndef _WORLDEVENT_H_
#define _WORLDEVENT_H_

#include "Stub/Logic/WorldEvent_Stub.h"

struct EventScheduleData;
class CTaskScheduler;

class CWorldEvent : public WorldEvent_Stub,
					public Reuben::Simulation::IEventHandler<CWorldEvent>
{
public:
	DEFINE_CLASS(WorldEvent_CLASSID);
	CWorldEvent();
	virtual ~CWorldEvent();
	Boolean InSchedule(time_t* t=NULL);
	Boolean IsRunning(time_t* t=NULL);
	Boolean NeedSchedule() { return m_bNeedSchedule; }
	Boolean IsStarted() { return (m_status == ST_STARTED); }
	Boolean IsStopped() { return (m_status == ST_STOPPED); }
	Boolean IsPaused() { return (m_status == ST_PAUSED); }
	void SetData(EventScheduleData* pData);
	void Schedule(Boolean bForce=BOOLEAN_TRUE);
	UInt32 CalcRepeatDiff(UInt32 timeLen);

	void Prepare();
	void StartEvent();
	void StopEvent(Boolean bForce=BOOLEAN_FALSE);
	void PeriodEvent(UInt8 line_id=0, Boolean bSchedule=BOOLEAN_TRUE);
	void NotifyNpcDead(UInt32 npcId, UInt8 line_id);
	String GetDetail();
	virtual void HandleEvent(CONST Event &event);
	static CWorldEvent* NewInstance();

	const EventScheduleData*	m_pData;
	CTaskScheduler*				m_pTaskScheduler;

private:
	UInt16 RandMapId();
	void RandGenNpc(UInt8 line_id=0);
	void OpenMap(Boolean bOpen);
	void SendMessage(UInt8 msgType, PCSTRING msg);
	void CalcSchedule(int nextInterval);
	Boolean InDate(time_t* t);
	Boolean InTime(time_t* t);

	typedef WorldEvent_Stub Parent;
	enum WE_Type
	{
		WE_TYPE_NONE = 0,
		WE_TYPE_BUFF,
		WE_TYPE_NPC,
		WE_TYPE_MAP,
		WE_TYPE_SCRIPT,
	};

	enum EventEnum
	{
		EVT_START		= MAKE_EVENTID(WorldEvent_CLASSID,  1),
		EVT_STOP		= MAKE_EVENTID(WorldEvent_CLASSID,  2),
		EVT_PERIOD		= MAKE_EVENTID(WorldEvent_CLASSID,  3),
	};

	enum EventStatus
	{
		ST_NONE = 0,
		ST_STARTED,
		ST_STOPPED,
		ST_PAUSED,
	};

	UInt16Vector	m_npcLives;			// npc alive number in different lines
	time_t			m_startSchTime;		// start schedule time
	time_t			m_endSchTime;		// end schedule time
	time_t			m_startEvtTime;		// start event time
	time_t			m_endEvtTime;		// end event time
	Boolean			m_bNeedSchedule;	// really need to schedule again?
	UInt8			m_status;
};

INLINE CWorldEvent* CWorldEvent::NewInstance()
{
	return CreateNetObject<CWorldEvent>();
}

#endif //_WORLDEVENT_H_
