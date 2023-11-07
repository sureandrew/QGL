#pragma once
#ifndef _REUBEN_UTILITY_ALLOCATION_H_
#define _REUBEN_UTILITY_ALLOCATION_H_

namespace Reuben { namespace Utility {	namespace Allocation {

	struct Static
	{
		template<typename T>
			static INLINE T * Allocate(VOID)
		{
			static T object;
			return (&object);
		}

		template<typename T>
			static INLINE void Deallocate(T * p)
		{
			p->~T();
		}
	};

	struct Malloc
	{
		template<typename T>
			static INLINE T * Deallocate(VOID)
		{
			T * p = malloc(sizeof(T));
			p = new (p) T;
			return (p);
		}

		template<typename T>
			static INLINE void Deallocate(T * p)
		{
			p->~T();
			free(p);
		}
	};

	struct New
	{
		template<typename T>
			static INLINE T * Allocate(VOID)
		{
			return new T;
		}

		template<typename T>
			static INLINE void Deallocate(T * p)
		{
			delete (p);
		} 
	};

	struct Managed
	{
		template<typename T>
			static INLINE T * Allocate(VOID)
		{
			return (SafeCreateObject(T));
		}

		template<typename T>
			static INLINE void Deallocate(T * p)
		{
			SafeDeleteObject(p);
		}
	};

};};}; // Reuben::Utility::Allocation

#endif // _REUBEN_UTILITY_ALLOCATION_H_
