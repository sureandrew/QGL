#pragma once
#ifndef _REUBEN_SYSTEM_MANAGED_ALLOCATOR_H_
#define _REUBEN_SYSTEM_MANAGED_ALLOCATOR_H_

#pragma warning(disable: 4100)

namespace Reuben { namespace System {

	// TEMPLATE CLASS ManagedAllocator
	template<typename _TYPE_>
	class ManagedAllocator
	{	// generic allocator for objects of class _TYPE_
	public:

		typedef typename _TYPE_ value_type;
		typedef value_type * pointer;
		typedef value_type & reference;
		typedef CONST value_type * const_pointer;
		typedef CONST value_type & const_reference;

		typedef Size size_type;
		typedef ptrdiff_t difference_type;

		template<typename _OTHER_>
		struct rebind
		{	// convert an ManagedAllocator<_Ty> to an ManagedAllocator <_Other>
			typedef ManagedAllocator<_OTHER_> other;
		};

		INLINE ManagedAllocator()
		{	// construct default allocator (do nothing)
		}

		INLINE ManagedAllocator(CONST ManagedAllocator<value_type> &)
		{	// construct by copying (do nothing)
		}

		template<typename _OTHER_>
			INLINE ManagedAllocator(CONST ManagedAllocator<_OTHER_> &)
		{	// construct from a related allocator (do nothing)
		}

		template<typename _OTHER_>
			INLINE ManagedAllocator<value_type>& operator = (CONST ManagedAllocator<_OTHER_> &)
		{	// assign from a related allocator (do nothing)
			return (*this);
		}

		INLINE pointer address(reference val) CONST
		{	// return address of mutable val
			return (&val);
		}

		INLINE const_pointer address(const_reference val) CONST
		{	// return address of nonmutable val
			return (&val);
		}

		INLINE VOID deallocate(pointer p, size_type)
		{	// deallocate object at p, ignore size
			SafeDeallocate(p);
		}

		INLINE pointer allocate(size_type count)
		{	// allocate array of count elements
			return SafeAllocate(value_type, count);
		}

		INLINE pointer allocate(size_type count, CONST VOID *)
		{	// allocate array of count elements, ignore hint
			return (allocate(count));
		}

		INLINE VOID construct(pointer p, CONST value_type & val)
		{	// construct object at p with value val
			new ((VOID*)p) _TYPE_(val);
		}

		INLINE VOID destroy(pointer p)
		{	// destroy object at p
			Memory::__Destruct(p);
		}

		INLINE Size max_size() CONST
		{	// estimate maximum array size
			Size count = (Size)(-1) / sizeof (value_type);
			return (0 < count ? count : 1);
		}
	};

	// ManagedAllocator TEMPLATE OPERATORS
	template<typename _TYPE_, typename _OTHER_>
		INLINE bool operator == (CONST ManagedAllocator<_TYPE_> &, CONST ManagedAllocator<_OTHER_> &)
	{	// test for ManagedAllocator equality (always true)
		return (true);
	}

	template<typename _TYPE_, typename _OTHER_>
		INLINE bool operator != (CONST ManagedAllocator<_TYPE_> &, CONST ManagedAllocator<_OTHER_> &)
	{	// test for ManagedAllocator inequality (always false)
		return (false);
	}

	// CLASS ManagedAllocator<VOID>
	template<>
	class ManagedAllocator<VOID>
	{	// generic ManagedAllocator for type VOID
	public:
		typedef VOID _TYPE_;
		typedef _TYPE_ * pointer;
		typedef CONST _TYPE_ * const_pointer;
		typedef _TYPE_ value_type;

		template<typename _OTHER_>
		struct rebind
		{	// convert an ManagedAllocator<VOID> to an ManagedAllocator <_OTHER_>
			typedef ManagedAllocator<_OTHER_> other;
		};

		INLINE ManagedAllocator()
		{	// construct default ManagedAllocator (do nothing)
		}

		INLINE ManagedAllocator(CONST ManagedAllocator<_TYPE_> &)
		{	// construct by copying (do nothing)
		}

		template<typename _OTHER_>
			INLINE ManagedAllocator(CONST ManagedAllocator<_OTHER_> &)
		{	// construct from related ManagedAllocator (do nothing)
		}

		template<typename _OTHER_>
			INLINE ManagedAllocator<_TYPE_> & operator = (CONST ManagedAllocator<_OTHER_> &)
		{	// assign from a related ManagedAllocator (do nothing)
			return (*this);
		}
	};

}; };  // Reuben::System

#pragma warning(default: 4100)

#endif // _REUBEN_SYSTEM_MANAGED_ALLOCATOR_H_
