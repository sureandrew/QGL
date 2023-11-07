#pragma once
#ifndef _TASKSCHEDULER_H_
#define _TASKSCHEDULER_H_

class CWorldEvent;

class CTaskScheduler
{
public:
	CTaskScheduler();
	virtual ~CTaskScheduler();
	time_t GetTime();
	time_t GetStartTime() { return m_startTime; }
	Boolean IsStarted() { return m_startTime > 0; }
	CWorldEvent* GetEvent(UInt16 eventId);
	void LoadEvent();
	void Start(LPCPEER pPeer=NULL);
	void Stop(LPCPEER pPeer=NULL);
	void Reschedule();
	void TimeZoneChanged(UInt8 gameTime);

private:
	typedef StlMap<UInt16, CWorldEvent*, std::less<UInt16>, 
			ManagedAllocator<std::pair<UInt16, CWorldEvent*> > > WorldEventMap;

	WorldEventMap	m_eventMap;
	time_t			m_startTime;
};

#endif // _TASKSCHEDULER_H_