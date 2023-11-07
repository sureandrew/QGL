#pragma once
#ifndef _REUBEN_SIMULATION_OBJECTMANAGER_H_
#define _REUBEN_SIMULATION_OBJECTMANAGER_H_

#include "Simulation.h"
#include "Object.h"
#include <map>
#include <set>

namespace Reuben { namespace Simulation {

	class ObjectManager
	{
	public:

		typedef Object* (*CreateFunc)();
		typedef StlMap<ClassID, CreateFunc, std::less<ClassID>, 
			ManagedAllocator<std::pair<ClassID, CreateFunc> > > ClassCreationMap;
		typedef StlSet<Object*, std::less<Object*>, ManagedAllocator<Object*> > ObjectSet;
		typedef StlMap<UInt32, Object*, std::less<UInt32>, 
			ManagedAllocator<std::pair<UInt32, Object*> > > ObjIndexMap;

		ObjectManager(VOID);
		virtual ~ObjectManager(VOID);

		template <typename _CLASS_>
			INLINE static Object* CreateObject(VOID);
		template <typename _CLASS_>
			INLINE CONST Boolean Register(CONST ClassID& cid);

		VOID Unregister(CONST ClassID& cid);
		Object* Create(CONST ClassID& cid);
		VOID Delete(CONST ObjectID& id);
		VOID Delete(Object* pObject);
		Object* LookupByID(CONST ObjectID& id);
		VOID SetLogger(Reuben::Diagnostics::Logger* logger);
		VOID DeleteAll(VOID);

	private:

		ClassCreationMap	m_mapCreation;
		ObjectSet			m_setObjects;
		Reuben::Diagnostics::Logger* m_pLogger;
		UInt32				m_curIndex;				// incremental object index
		ObjIndexMap			m_objIndices;			// object index map when delete all objects
		ThreadKey			m_key;					// safe thread key
	};

	template <typename _CLASS_>
		INLINE static Object* ObjectManager::CreateObject(VOID)
	{
		return (SafeCreateObject(_CLASS_));
	}

	template <typename _CLASS_>
		INLINE CONST Boolean ObjectManager::Register(CONST ClassID& cid)
	{
		ThreadGuard guard(m_key);
		if (m_mapCreation.find(cid) != m_mapCreation.end())
			return (BOOLEAN_FALSE);

		m_mapCreation[cid] = &CreateObject<_CLASS_>;
		return BOOLEAN_TRUE;
	}

	typedef Reuben::Utility::Singleton<ObjectManager> ObjectInstance;

	template <typename _CLASS_>
	INLINE CONST Boolean RegisterClassId(CONST ClassID& cid)
	{
		return ObjectInstance::GetInstance().Register<_CLASS_>(cid);
	}

	INLINE VOID UnregisterClassId(CONST ClassID& cid)
	{
		ObjectInstance::GetInstance().Unregister(cid);
	}

	INLINE Object* CreateObject(CONST ClassID& cid)
	{
		return ObjectInstance::GetInstance().Create(cid);
	}

	template <typename _CLASS_>
		INLINE _CLASS_* CreateObject(VOID)
	{
		return ((_CLASS_*)ObjectInstance::GetInstance().Create(_CLASS_::_GetClassID()));
	}

	template <typename _CLASS_>
		INLINE _CLASS_* CreateObject(CONST ClassID &cid)
	{
		Object* obj = ObjectInstance::GetInstance().Create(cid);
		if (obj && obj->IsSubClass(_CLASS_::_GetClassID()))
			return ((_CLASS_*) obj);
		else
			return NULL;
	}

	INLINE VOID DeleteObject(CONST ObjectID& id)
	{
		ObjectInstance::GetInstance().Delete(id);
	}

	INLINE VOID DeleteObject(Object* pObject)
	{
		ObjectInstance::GetInstance().Delete(pObject);
	}

	INLINE Object* FindObject(CONST ObjectID& id)
	{
		return ObjectInstance::GetInstance().LookupByID(id);
	}

	template <typename _CLASS_>
		INLINE _CLASS_* FindObject(CONST ObjectID& id)
	{
		Object* pObject = ObjectInstance::GetInstance().LookupByID(id);
		if (pObject && pObject->IsSubClass(_CLASS_::_GetClassID()))
			return (pObject);
		else
			return (NULL);
	}

	INLINE VOID SetObjectLogger(Reuben::Diagnostics::Logger* logger)
	{
		ObjectInstance::GetInstance().SetLogger(logger);
	}

}; }; // Reuben::Simulation

#endif // _REUBEN_SIMULATION_OBJECTMANAGER_H_
