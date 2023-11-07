#pragma once
#ifndef _REUBEN_SIMULATION_EVENT_H_
#define _REUBEN_SIMULATION_EVENT_H_

#include "Simulation.h"
#include <vector>

#define MAKE_EVENTID(_classid_, _enum_)		((Reuben::Simulation::EventID(_classid_) << 16) | Reuben::Simulation::EventID(_enum_))

namespace Reuben { namespace Simulation {

	class Object;
	class Event;

	INLINE HandleID RaiseEvent(CONST Event& event, UInt32 atTime = 0);
	INLINE HandleID RaiseUniqEvent(CONST Event& event, UInt32 atTime = 0);
	INLINE Event* FindEvent(CONST HandleID handle);
	INLINE Event* FindEvent(CONST EventID evtId, CONST ObjectID srcId);
	INLINE VOID ChangePriority(CONST HandleID handle, CONST Priority priority);
	INLINE VOID ChangeTime(CONST HandleID handle, CONST UInt32 atTime);
	INLINE Boolean CancelEvent(CONST HandleID handle);
	INLINE Boolean CancelUniqEvent(CONST EventID evtId, CONST ObjectID srcId);
	INLINE Boolean CancelAllEvent(CONST ObjectID srcId);

	class Event
	{
	private:
		EventID m_EventId;
		Priority m_Priority;
		ObjectID m_SourceId;
		ObjectID m_TargetId;

	public:
		enum PriorityEnum
		{
			PRI_IMMEDIATE	= 0,
			PRI_HIGH		= 50,
			PRI_NORMAL		= 100,
			PRI_LOW			= 200,
			PRI_LAST		= 255,
		};

		INLINE Event() {}
		INLINE Event(EventID eventId, ObjectID sourceId, ObjectID targetId = 0);
		INLINE Event(CONST Event& event);

		INLINE CONST ClassID GetClassID(VOID) CONST;
		INLINE CONST EventID GetID(VOID) CONST;
		INLINE CONST ObjectID GetSourceID(VOID) CONST;
		INLINE CONST ObjectID GetTargetID(VOID) CONST;
		INLINE CONST Priority GetPriority(VOID) CONST;
		INLINE VOID SetPriority(CONST Priority priority);
	};

	INLINE Event::Event(EventID eventId, ObjectID sourceId, ObjectID targetId)
		: m_EventId(eventId), m_SourceId(sourceId), m_TargetId(targetId)
	{
		m_Priority = PRI_NORMAL;
	}

	INLINE Event::Event(CONST Event& event)
		: m_EventId(event.GetID()), m_Priority(event.GetPriority())
		, m_SourceId(event.GetSourceID()), m_TargetId(event.GetTargetID())
	{
	}

	INLINE CONST ClassID Event::GetClassID(VOID) CONST
	{
		// : Fix a defect here
		//return (ClassID(m_EventId) >> 16);
		return (ClassID) (m_EventId >> 16);
	}

	INLINE CONST EventID Event::GetID(VOID) CONST
	{
		return (m_EventId);
	}

	INLINE CONST ObjectID Event::GetSourceID(VOID) CONST
	{
		return (m_SourceId);
	}

	INLINE CONST ObjectID Event::GetTargetID(VOID) CONST
	{
		return (m_TargetId);
	}

	INLINE CONST Priority Event::GetPriority(VOID) CONST
	{
		return (m_Priority);
	}

	INLINE VOID Event::SetPriority(CONST Priority priority)
	{
		m_Priority = priority;
	}

	template <typename _CLASS_>
	class IEventHandler
	{
	public:
		INLINE virtual ~IEventHandler() { CancelAllEvent(); }
		INLINE HandleID RaiseEventID(CONST EventID evtId, CONST UInt32 atTime = 0)
		{
			Event event(evtId, ((CONST _CLASS_*) this)->GetID());
			return Reuben::Simulation::RaiseEvent(event, atTime);
		}

		INLINE HandleID RaiseUniqEventID(CONST EventID evtId, CONST UInt32 atTime = 0)
		{
			Event event(evtId, ((CONST _CLASS_*) this)->GetID());
			return Reuben::Simulation::RaiseUniqEvent(event, atTime);
		}

		INLINE Boolean CancelEvent(CONST HandleID handle)
		{
			return Reuben::Simulation::CancelEvent(handle);
		}

		INLINE Boolean CancelUniqEvent(CONST EventID evtId)
		{
			return Reuben::Simulation::CancelUniqEvent(evtId, ((CONST _CLASS_*) this)->GetID());
		}

		INLINE Boolean CancelAllEvent()
		{
			return Reuben::Simulation::CancelAllEvent(((CONST _CLASS_*) this)->GetID());
		}
	};

}; }; // Reuben::Simulation

#endif // _REUBEN_SIMULATION_EVENT_H_
