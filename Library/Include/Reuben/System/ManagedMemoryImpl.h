#pragma once
#ifndef _REUBEN_SYSTEM_MANAGED_MEMORY_IMPL_H_
#define _REUBEN_SYSTEM_MANAGED_MEMORY_IMPL_H_

namespace Reuben { namespace System {

	//---------------------------------------------------------
	//					 Allocate Heap Function
	//---------------------------------------------------------

	// TEMPLATE FUNCTION __Allocate
	template<typename _TYPE_>
		INLINE _TYPE_ * __FASTCALL Memory::__Allocate(Size count, PCSTRING pSourceLine)
	{	// allocate storage for _Count elements of type _Ty
		return ((_TYPE_ *)MemoryManager::GetInstance().Allocate(count * sizeof (_TYPE_), pSourceLine));
	}

	//---------------------------------------------------------
	//					 Release Heap Function
	//---------------------------------------------------------

	// TEMPLATE FUNCTION __Deallocate
	template <typename _TYPE_>
		INLINE VOID __FASTCALL Memory::__Deallocate(_TYPE_ *& p, PCSTRING pSourceLine)
	{
		if (p)
		{
			MemoryManager::GetInstance().Deallocate(p, sizeof(_TYPE_), pSourceLine);
			p = NULL;
		}
	}

	// TEMPLATE FUNCTION __Destruct
	template <typename _TYPE_>
		INLINE VOID __FASTCALL Memory::__Destruct(_TYPE_ * p)
	{	// destruct object at p
		(p)->~_TYPE_();
	}

	template <>
		INLINE VOID __FASTCALL Memory::__Destruct(char *)
	{	// destruct a char (do nothing)
	}

	template <>
		INLINE VOID __FASTCALL Memory::__Destruct(wchar_t *)
	{	// destruct a wchar_t (do nothing)
	}

	template<typename _TYPE_>
		INLINE VOID __FASTCALL Memory::__DeleteObject(_TYPE_ *& pObject, PCSTRING pSourceLine)
	{
		if (pObject)
		{
			__Destruct(pObject);
			__Deallocate(pObject, pSourceLine);
			pObject = NULL;
		}
	}

	template<typename _TYPE_>
		INLINE VOID __FASTCALL Memory::__Delete(_TYPE_ *& pObject, PCSTRING pSourceLine)
	{
		if (pObject)
		{
			delete pObject;
			pObject = NULL;
		}
	}

	template<typename _TYPE_>
		INLINE VOID __FASTCALL Memory::__DeleteArray(_TYPE_ *& pObject, PCSTRING pSourceLine)
	{
		if (pObject)
		{
			delete [] pObject;
			pObject = NULL;
		}
	}

	template<typename _TYPE_>
		INLINE VOID __FASTCALL Memory::__Free(_TYPE_ *& pObject, PCSTRING pSourceLine)
	{
		if (pObject)
		{
			free(pObject);
			pObject = NULL;
		}
	}

	template<typename _TYPE_>
		INLINE VOID __FASTCALL Memory::__Release(_TYPE_ *& pObject, PCSTRING pSourceLine)
	{
		if (pObject)
		{
			pObject->Release();
			pObject = NULL;
		}
	}

	INLINE ErrorCode __FASTCALL Memory::__CheckMemory(CONST VOID* pBlock)
	{
		return (MemoryManager::GetInstance().CheckMemory(pBlock));
	}

	template <typename _TYPE_>
	struct __SafeDeleteObjectProc
	{
		INLINE __SafeDeleteObjectProc(_TYPE_*& p, PCSTRING pSourceLine)
		{
			Memory::__DeleteObject(p, pSourceLine);
		}
	};

	template <typename _TYPE_>
	struct __SafeDeallocateProc
	{
		INLINE __SafeDeallocateProc(_TYPE_*& p, PCSTRING pSourceLine)
		{
			Memory::__Deallocate(p, pSourceLine);
		}
	};

	template <typename _TYPE_>
	struct __SafeDeleteProc
	{
		INLINE __SafeDeleteProc(_TYPE_*& p, PCSTRING pSourceLine)
		{
			Memory::__Delete(p, pSourceLine);
		}
	};

	template <typename _TYPE_>
	struct __SafeDeleteArrayProc
	{
		INLINE __SafeDeleteArrayProc(_TYPE_*& p, PCSTRING pSourceLine)
		{
			Memory::__DeleteArray(p, pSourceLine);
		}
	};

	template <typename _TYPE_>
	struct __SafeFreeProc
	{
		INLINE __SafeFreeProc(_TYPE_*& p, PCSTRING pSourceLine)
		{
			Memory::__Free(p, pSourceLine);
		}
	};

	template <typename _TYPE_>
	struct __SafeReleaseProc
	{
		INLINE __SafeReleaseProc(_TYPE_*& p, PCSTRING pSourceLine)
		{
			Memory::__Release(p, pSourceLine);
		}
	};

	template <typename _TYPE_, typename _DP_>
	struct __SafeAutoPointer
	{
		_TYPE_* m_pPointer;

		INLINE __SafeAutoPointer(_TYPE_* pPointer)
			: m_pPointer(pPointer)
		{}

		INLINE ~__SafeAutoPointer(VOID)
		{
			_DP_(m_pPointer, _SOURCE_LINE_);
		}

		INLINE _TYPE_* operator -> (VOID)
		{
			DEBUG_CHECK(m_pPointer != NULL);
			return (m_pPointer);
		}

		INLINE operator _TYPE_* (VOID) CONST
		{
			DEBUG_CHECK(m_pPointer != NULL);
			return (m_pPointer);
		}

		INLINE bool operator ! (VOID)
		{
			return (m_pPointer == NULL);
		}

		INLINE bool operator == (CONST __SafeAutoPointer& ap)
		{
			return (m_pPointer == ap.m_pPointer);
		}

		INLINE bool operator != (CONST __SafeAutoPointer& ap)
		{
			return (m_pPointer != ap.m_pPointer);
		}

		INLINE __SafeAutoPointer& operator = (CONST __SafeAutoPointer& ap)
		{
			_DP_(m_pPointer, _SOURCE_LINE_);
			m_pPointer = ap.m_pPointer;
			return (*this);
		}

		INLINE __SafeAutoPointer& operator = (_TYPE_* pPointer)
		{
			_DP_(m_pPointer, _SOURCE_LINE_);
			m_pPointer = pPointer;
			return (*this);
		}

	};

}; };  // Reuben::System

#endif // _REUBEN_SYSTEM_MANAGED_MEMORY_IMPL_H_
