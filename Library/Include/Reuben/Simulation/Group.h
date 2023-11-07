#pragma once
#ifndef _REUBEN_SIMULATION_GROUP_H_
#define _REUBEN_SIMULATION_GROUP_H_

#include "Simulation.h"
#include "Object.h"
#include <vector>

#define Group_CLASSID	2

namespace Reuben { namespace Simulation {

	class Group : public Object
	{
	protected:
		typedef Object Parent;
		ObjectList m_children;

	public:
		enum EventEnum
		{
			EVT_GROUP_MODIFLED	= 5,
		};

		INLINE VOID Clear();
		INLINE UInt16 GetCount() CONST;
		INLINE Object* GetChild(UInt16 index) CONST;
		INLINE Boolean Add(Object *obj);
		INLINE Boolean Remove(Object *obj);
		INLINE static Group* NewInstance();
		virtual String DebugInfo();

		DEFINE_CLASS(Group_CLASSID);
	};

	REGISTER_CLASSTYPE(Group, Group_CLASSID);

	INLINE VOID Group::Clear()
	{
		m_children.clear();
	}

	INLINE UInt16 Group::GetCount() CONST
	{
		return (UInt16) m_children.size();
	}

	INLINE Object* Group::GetChild(UInt16 index) CONST
	{
		if (index < m_children.size())
			return m_children[index];
		else
			return NULL;
	}

	INLINE Boolean Group::Add(Object *obj)
	{
		if (std::find(m_children.begin(), m_children.end(), obj) == m_children.end()) {
			m_children.push_back(obj);
			return BOOLEAN_TRUE;
		}
		return BOOLEAN_FALSE;
	}

	INLINE Boolean Group::Remove(Object *obj)
	{
		ObjectList::iterator iter = std::find(m_children.begin(), m_children.end(), obj);
		if (iter != m_children.end()) {
			m_children.erase(iter);
			return BOOLEAN_TRUE;
		}
		return BOOLEAN_FALSE;
	}

	INLINE Group* Group::NewInstance()
	{
		return (Group*)(Reuben::Simulation::CreateObject(Group_CLASSID));
	}

	INLINE String Group::DebugInfo()
	{
		String s(Parent::DebugInfo());
		s.AppendFormat(_T(", count %d"), m_children.size());

		return s;
	}

}; }; // Reuben::Simulation

#endif // _REUBEN_SIMULATION_GROUP_H_
