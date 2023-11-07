#pragma once
#ifndef _REUBEN_SIMULATION_SCHEDULE_MANAGER_H_
#define _REUBEN_SIMULATION_SCHEDULE_MANAGER_H_

#include "..\Utility\Process.h"
#include "Object.h"
#include "Event.h"
#include <deque>

namespace Reuben { namespace Simulation {
	
	struct EventInfo
	{
		UInt32		handleId;
		UInt32		atTime;
		EventInfo*	next;
		Event		event;
		Boolean		alive;	

		EventInfo() : handleId(0), atTime(0), next(NULL), alive(BOOLEAN_TRUE) {}
		EventInfo(UInt32 id, UInt32 t, CONST Event &evt)
			: handleId(id), atTime(t), event(evt), next(NULL), alive(BOOLEAN_TRUE) {}
	};

	class ScheduleManager : public Reuben::Utility::Process
	{
	public:
		ScheduleManager();
		virtual ~ScheduleManager();
		virtual VOID OnUpdate(CONST UInt32 dt);
		HandleID RaiseEvent(CONST Event& event, UInt32 atTime = 0);
		HandleID RaiseUniqEvent(CONST Event& event, UInt32 atTime = 0);
		EventInfo* FindEvent(CONST HandleID handle);
		EventInfo* FindEvent(CONST EventID evtId, CONST ObjectID srcId);
		VOID ChangePriority(CONST HandleID handle, CONST Priority priority);
		VOID ChangeTime(CONST HandleID handle, CONST UInt32 atTime);
		Boolean CancelEvent(CONST HandleID handle);
		Boolean CancelUniqEvent(CONST EventID evtId, CONST ObjectID srcId);
		Boolean CancelAllEvent(CONST ObjectID srcId);

	private:
		EventInfo*	m_root;						// root EventInfo node
		EventInfo*	m_deadlist;					// dead EventInfo nodes
		HandleID	m_lastHandle;

		void DebugInfo();
	};

	typedef Reuben::Utility::Singleton<ScheduleManager> ScheduleInstance;

	INLINE HandleID RaiseEvent(CONST Event& event, UInt32 atTime)
	{
		return ScheduleInstance::GetInstance().RaiseEvent(event, atTime);
	}

	INLINE HandleID RaiseUniqEvent(CONST Event& event, UInt32 atTime)
	{
		return ScheduleInstance::GetInstance().RaiseUniqEvent(event, atTime);
	}

	INLINE Event* FindEvent(CONST HandleID handle)
	{
		EventInfo *pinfo = ScheduleInstance::GetInstance().FindEvent(handle);
		if (pinfo)
			return &pinfo->event;
		else
			return NULL;
	}

	INLINE Event* FindEvent(CONST EventID evtId, CONST ObjectID srcId)
	{
		EventInfo *pinfo = ScheduleInstance::GetInstance().FindEvent(evtId, srcId);
		if (pinfo)
			return &pinfo->event;
		else
			return NULL;
	}

	INLINE VOID ChangePriority(CONST HandleID handle, CONST Priority priority)
	{
		ScheduleInstance::GetInstance().ChangePriority(handle, priority);
	}

	INLINE VOID ChangeTime(CONST HandleID handle, CONST UInt32 atTime)
	{
		ScheduleInstance::GetInstance().ChangeTime(handle, atTime);
	}

	INLINE Boolean CancelEvent(CONST HandleID handle)
	{
		return ScheduleInstance::GetInstance().CancelEvent(handle);
	}

	INLINE Boolean CancelUniqEvent(CONST EventID evtId, CONST ObjectID srcId)
	{
		return ScheduleInstance::GetInstance().CancelUniqEvent(evtId, srcId);
	}

	INLINE Boolean CancelAllEvent(CONST ObjectID srcId)
	{
		return ScheduleInstance::GetInstance().CancelAllEvent(srcId);
	}

}; }; // Reuben::Simulation

#endif // _REUBEN_SIMULATION_SCHEDULE_MANAGER_H_
