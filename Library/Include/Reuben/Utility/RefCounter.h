#pragma once
#ifndef _REUBEN_UTILITY_REFERENCE_COUNTER_H_
#define _REUBEN_UTILITY_REFERENCE_COUNTER_H_

//-- Local
#ifdef COMPILER_MULTITHREADING
#include "..\Platform\Thread.h"
#endif // COMPILER_MULTITHREADING

namespace Reuben { namespace Utility {

	template <typename _TYPE_>
	struct IRefCounter
	{
		virtual _TYPE_* AddRef(VOID) CONST = 0;
		virtual VOID Release(VOID) CONST = 0;
		virtual _TYPE_ * GetPointer(VOID) CONST = 0;

	};

	template <typename _TYPE_, typename _DP_>
	class RefCounter
		: public IRefCounter<_TYPE_>
	{
	private:

#ifdef COMPILER_MULTITHREADING
		mutable ThreadKey m_key;
#endif // COMPILER_MULTITHREADING

		mutable Count m_nCount;

	public:

		INLINE RefCounter(VOID)
			: m_nCount(C_COUNT(0))
		{}

		INLINE virtual _TYPE_* AddRef(VOID) CONST
		{
#ifdef COMPILER_MULTITHREADING
			ThreadGuard guard(m_key);
#endif // COMPILER_MULTITHREADING
			++m_nCount;
			return ((_TYPE_*)this);
		}

		INLINE virtual VOID Release(VOID) CONST
		{
			DEBUG_CHECK(m_nCount > 0);
			Boolean bDestroy;
			// to check is it need to destroy
			{
#ifdef COMPILER_MULTITHREADING
				ThreadGuard guard(m_key);
#endif // COMPILER_MULTITHREADING
				bDestroy = BOOLEAN_FALSE;
				--m_nCount;
				if (m_nCount == 0)
					bDestroy = BOOLEAN_TRUE;
			}
			if (bDestroy)
			{
				_TYPE_* p = GetPointer();
				_DP_(p, _SOURCE_LINE_);
			}
		}

		INLINE virtual _TYPE_* GetPointer(VOID) CONST
		{
			return ((_TYPE_*)this);
		}
	};

};}; // Reuben::Utility 

#endif // _REUBEN_UTILITY_REFERENCE_COUNTER_H_
