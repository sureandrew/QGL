#pragma once
#ifndef _REUBEN_UTILITY_SINGLETON_H_
#define _REUBEN_UTILITY_SINGLETON_H_

#if defined(COMPILER_MULTITHREADING)
#include "..\Platform\Thread.h"
#endif
#include "Allocation.h"

#define SINGLETON(Name)								\
	Name(const Name &);								\
	Name & operator = (const Name &);				\
	friend Reuben::Utility::Allocation::Static;		\
	friend Reuben::Utility::Allocation::Malloc;		\
	friend Reuben::Utility::Allocation::New;		\
	friend Reuben::Utility::Allocation::Managed;	\
	MANAGED_OBJECT()

namespace Reuben { namespace Utility {

	template<typename T, typename Allocator = Allocation::Managed>
	class Singleton
	{
	private:

		static T * sm_pObject;

	private:

		//-- Copy Constructor
		Singleton(CONST Singleton &);

		//-- Assign Operator
		Singleton & operator = (CONST Singleton &);

	protected:
		
		//-- Constructor / Destructor
		Singleton(VOID) {}

	public:

		static INLINE T & GetInstance(VOID)
		{
			if (sm_pObject == NULL)
			{
#if defined(COMPILER_MULTITHREADING)
				static ThreadKey key;
				ThreadGuard guard(key);
				if (sm_pObject == NULL)
					sm_pObject = Allocator::Allocate<T>();
#else
				sm_pObject = Allocator::Allocate<T>();
#endif
			}
			return (*sm_pObject);
		}

		static INLINE VOID DestroyInstance(VOID)
		{
			if (sm_pObject == NULL)
				return;
			Allocator::Deallocate<T>(sm_pObject);
			sm_pObject = NULL;
		}
	};

	template <typename T, typename A>
		T * Singleton<T, A>::sm_pObject = NULL;

};}; // Reuben::Utility

#endif // _REUBEN_UTILITY_SINGLETON_H_
