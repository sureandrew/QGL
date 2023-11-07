#pragma once
#ifndef _REUBEN_NETWORK_NETGROUP_H_
#define _REUBEN_NETWORK_NETGROUP_H_

#include "NetObject.h"
#include <vector>
#include <algorithm>

#define NetGroup_CLASSID	4

namespace Reuben { namespace Network {

	class NetGroup : public NetObject
	{
	protected:

		typedef NetObject Parent;
		typedef StlVector<NetObject *, ManagedAllocator<NetObject *> > NetObjList;
		NetObjList m_children;
		Boolean m_bValidate;

	public:
		INLINE NetGroup() : m_bValidate(BOOLEAN_TRUE) {}
		INLINE VOID Clear(Boolean bDelete = BOOLEAN_FALSE);
		INLINE UInt16 GetCount() CONST;
		INLINE Boolean IsChild(NetObject *obj) CONST;
		INLINE NetObject* GetChild(UInt16 index) CONST;
		template <typename _TYPE_>
			INLINE _TYPE_* GetChild(UInt16 index) CONST;
		INLINE Boolean Add(NetObject *obj);
		INLINE Boolean Insert(NetObject *obj, UInt16 index);
		INLINE Boolean Remove(NetObject *obj);
		INLINE NetObject* Replace(UInt16 index, NetObject *obj);
		INLINE VOID Swap(NetGroup *grp);
		INLINE VOID Swap(UInt16 fromIdx, UInt16 count, NetGroup *grp);
		INLINE VOID Validate();
		INLINE VOID SetValidate(Boolean bNeed) { m_bValidate = bNeed; };
		virtual String DebugInfo();

		DEFINE_CLASS(NetGroup_CLASSID);
	};

	REGISTER_CLASSTYPE(NetGroup, NetGroup_CLASSID);

	INLINE VOID NetGroup::Clear(Boolean bDelete)
	{
		if (bDelete) {
			for (NetObjList::iterator iter = m_children.begin();
				iter != m_children.end(); iter++)
			{
				Reuben::Simulation::DeleteObject(*iter);
			}
		}
		m_children.clear();
	}

	INLINE UInt16 NetGroup::GetCount() CONST
	{
		return (UInt16) m_children.size();
	}

	INLINE Boolean NetGroup::IsChild(NetObject *obj) CONST
	{
		return (std::find(m_children.begin(), m_children.end(), obj) != 
			m_children.end());
	}

	INLINE NetObject* NetGroup::GetChild(UInt16 index) CONST
	{
		if (index < m_children.size())
		{
			NetObject *obj = m_children[index];
			if (!m_bValidate || obj == NULL || Reuben::Simulation::FindObject(obj->GetID()))
				return obj;
		}
		return NULL;
	}

	template <typename _TYPE_>
	INLINE _TYPE_* NetGroup::GetChild(UInt16 index) CONST
	{
		NetObject *obj = GetChild(index);
		if (obj && obj->IsSubClass(_TYPE_::_GetClassID()))
			return ((_TYPE_*) obj);
		else
			return NULL;
	}

	INLINE Boolean NetGroup::Add(NetObject *obj)
	{
		if (obj == NULL) return BOOLEAN_FALSE;

		if (std::find(m_children.begin(), m_children.end(), obj) == m_children.end()) {
			m_children.push_back(obj);
			return BOOLEAN_TRUE;
		}

		return BOOLEAN_FALSE;
	}

	INLINE Boolean NetGroup::Insert(NetObject *obj, UInt16 index)
	{
		if (obj == NULL || index > (UInt16) m_children.size())
			return BOOLEAN_FALSE;

		if (std::find(m_children.begin(), m_children.end(), obj) == m_children.end()) {
			m_children.insert(m_children.begin() + index, obj);
			return BOOLEAN_TRUE;
		}

		return BOOLEAN_FALSE;
	}

	INLINE Boolean NetGroup::Remove(NetObject *obj)
	{
		if (obj == NULL) return BOOLEAN_FALSE;

		NetObjList::iterator iter = std::find(m_children.begin(), m_children.end(), obj);
		if (iter != m_children.end()) {
			m_children.erase(iter);
			return BOOLEAN_TRUE;
		}

		return BOOLEAN_FALSE;
	}

	INLINE NetObject* NetGroup::Replace(UInt16 index, NetObject *obj)
	{
		NetObject *oldObj = NULL;
		if (index < m_children.size()) {
			oldObj = m_children[index];
			m_children[index] = obj;
		}

		return oldObj;
	}

	INLINE VOID NetGroup::Swap(NetGroup *grp)
	{
		if (grp == NULL)
			return;

		m_children.swap(grp->m_children);
	}

	INLINE VOID NetGroup::Swap(UInt16 fromIdx, UInt16 count, NetGroup *grp)
	{
		if (grp == NULL || (UInt16) m_children.size() < fromIdx + count)
			return;

		NetObjList::iterator it1 = m_children.begin() + fromIdx;
		NetObjList::iterator it2 = m_children.begin() + fromIdx + count;
		grp->m_children.clear();
		for (NetObjList::iterator it = it1; it != it2; ++it)
			grp->m_children.push_back(*it);

		m_children.erase(it1, it2);
	}

	INLINE VOID NetGroup::Validate()
	{
		if (!m_bValidate)
			return;

		for (NetObjList::iterator iter = m_children.begin(); iter != m_children.end();)
		{
			NetObject *obj = *iter;
			if (obj == NULL || !Reuben::Simulation::FindObject(obj->GetID()))
				iter = m_children.erase(iter);
			else
				++iter;
		}
	}

	INLINE String NetGroup::DebugInfo()
	{
		String s(Parent::DebugInfo());
		s.AppendFormat(_T(", count %d"), m_children.size());

		return s;
	}

};}; // Reuben::Network

#endif // _REUBEN_NETWORK_NETGROUP_H_