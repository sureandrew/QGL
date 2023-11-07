#pragma once
#ifndef _REUBEN_SIMULATION_OBJECT_H_
#define _REUBEN_SIMULATION_OBJECT_H_

#include "Simulation.h"
#include <vector>

#define DEFINE_PARENT_CLASS(_parent_) \
	typedef _parent_ Parent

#define DEFINE_CLASS(_classid_) \
	static CONST Reuben::Simulation::ClassID _GetClassID(VOID) \
	{ \
		return (_classid_); \
	} \
	virtual CONST Reuben::Simulation::ClassID GetClassID(VOID) CONST \
	{ \
		return (_classid_); \
	} \
	virtual Boolean IsSubClass(Reuben::Simulation::ClassID cid) CONST \
	{ \
		return (cid == _classid_ || Parent::IsSubClass(cid));	\
	}

#define REGISTER_CLASSTYPE(_ctype_, _cid_) \
	static Boolean Value##_cid_ = Reuben::Simulation::RegisterClassId<_ctype_>(_cid_)

namespace Reuben { namespace Simulation {

	class Object;
	class Event;
	template <typename _CLASS_>
		INLINE CONST Boolean RegisterClassId(CONST ClassID& cid);
	INLINE VOID UnregisterClassId(CONST ClassID& cid);
	INLINE Object* CreateObject(CONST ClassID& cid);
	template <typename _CLASS_>
		INLINE _CLASS_* CreateObject(VOID);
	template <typename _CLASS_>
		INLINE _CLASS_* CreateObject(CONST ClassID& cid);
	INLINE VOID DeleteObject(CONST ObjectID& id);
	INLINE VOID DeleteObject(Object* pObject);
	INLINE Object* FindObject(CONST ObjectID& id);
	template <typename _CLASS_>
		INLINE _CLASS_* FindObject(CONST ObjectID& id);
	INLINE VOID SetObjectLogger(Reuben::Diagnostics::Logger* logger);

	typedef StlVector<Object *, ManagedAllocator<Object *> > ObjectList;

	class Object
	{
	public:
		INLINE Object() : m_index(0) {}
		INLINE virtual ~Object() {}
		INLINE CONST ObjectID GetID(VOID) CONST;
		INLINE CONST UInt32 GetObjIndex(VOID) CONST { return m_index; }
		INLINE VOID SetObjIndex(UInt32 index) { m_index = index; }

		virtual CONST ClassID GetClassID(VOID) CONST = 0;
		virtual Boolean IsSubClass(ClassID cid) CONST { return BOOLEAN_FALSE; }
		virtual VOID HandleEvent(CONST Event& event) {};
		virtual String DebugInfo();

	private:
		UInt32		m_index;			// object index
	};

	INLINE CONST ObjectID Object::GetID(VOID) CONST
	{
		return (ObjectID)(PtrToUint(this));
	}

	INLINE String Object::DebugInfo()
	{
		String s;
		s.Format(_T("classID %d, objID %d"), GetClassID(), GetID());

		return s;
	}

}; }; // Reuben::Simulation

#endif // _REUBEN_SIMULATION_OBJECT_H_
