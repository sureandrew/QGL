#pragma once
#ifndef _REUBEN_UTILITY_SMART_POINTER_H_
#define _REUBEN_UTILITY_SMART_POINTER_H_

#include "RefCounter.h"

namespace Reuben { namespace Utility {

	template <typename _TYPE_, typename _DP_>
	class SmartPointer
	{
	private:

		class DefaultRefCounter
			: public IRefCounter<_TYPE_>
		{
		private:

			mutable volatile Count m_nCount;
			mutable _TYPE_* m_p;

		public:

			INLINE DefaultRefCounter(_TYPE_* p)
				: m_nCount(C_COUNT(0))
				, m_p(p)
			{
				DEBUG_CHECK(p);
			}

			INLINE virtual ~DefaultRefCounter(VOID)
			{
				_TYPE_* p = GetPointer();
				_DP_(p, _SOURCE_LINE_);
			}

			INLINE virtual _TYPE_* AddRef(VOID) CONST
			{
				::InterlockedIncrement(reinterpret_cast<volatile LONG *>(&m_nCount));
				return (m_p);
			}

			INLINE virtual VOID Release(VOID) CONST
			{
				DEBUG_CHECK(m_nCount > 0);
				::InterlockedDecrement(reinterpret_cast<volatile LONG *>(&m_nCount));
				if (m_nCount == 0)
				{
					DefaultRefCounter* p = const_cast<DefaultRefCounter*>(this);
					SafeDeleteObject(p);
				}
			}

			INLINE virtual _TYPE_* GetPointer(VOID) CONST
			{
				return (m_p);
			}
		};

		CONST IRefCounter<_TYPE_>* m_pRefCount;

		INLINE VOID Assign(CONST VOID* p)
		{
			if (p == NULL)
				Assign((IRefCounter<_TYPE_>*)NULL);
			else
				Assign(SafeCreateObject1(DefaultRefCounter, static_cast<_TYPE_*>(p)));
		}

		INLINE VOID Assign(CONST IRefCounter<_TYPE_>* pRefCount)
		{
			if (pRefCount != NULL)
				pRefCount->AddRef();
			CONST IRefCounter<_TYPE_>* pOldRef = m_pRefCount;
			m_pRefCount = pRefCount;
			if (pOldRef != NULL)
				pOldRef->Release();
		}

	public:

		INLINE SmartPointer(VOID)
			: m_pRefCount(NULL)
		{}

		INLINE SmartPointer(CONST _TYPE_* p)
			: m_pRefCount(NULL)
		{
			Assign(p);
		}

		INLINE SmartPointer(CONST SmartPointer& sp)
			: m_pRefCount(NULL)
		{
			Assign(sp.m_pRefCount);
		}

		INLINE virtual ~SmartPointer(VOID)
		{
			Assign((IRefCounter<_TYPE_>*)NULL);
		}

		INLINE _TYPE_* GetPointer(VOID) CONST
		{
			return ((m_pRefCount == NULL) ? NULL : m_pRefCount->GetPointer());
		}

		INLINE SmartPointer& operator = (CONST SmartPointer& sp)
		{
			Assign(sp.m_pRefCount);
			return (*this);
		}

		INLINE SmartPointer& operator = (_TYPE_* p)
		{
			Assign(p);
			return (*this);
		}

		INLINE _TYPE_* operator -> ()
		{
			DEBUG_CHECK(GetPointer() != NULL);
			return (GetPointer());
		}

		INLINE operator _TYPE_* () CONST
		{
			return (GetPointer());
		}

		INLINE bool operator ! ()
		{
			return (GetPointer() == NULL);
		}

		INLINE bool operator == (CONST SmartPointer& sp)
		{
			return (GetPointer() == sp.GetPointer());
		}

		INLINE bool operator != (CONST SmartPointer& sp)
		{
			return (GetPointer() != sp.GetPointer());
		}

	};

};}; // Reuben::Utility

#endif // _REUBEN_UTILITY_SMART_POINTER_H_
