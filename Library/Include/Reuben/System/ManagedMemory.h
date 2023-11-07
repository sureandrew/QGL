#pragma once
#ifndef _REUBEN_SYSTEM_MANAGED_MEMORY_H_
#define _REUBEN_SYSTEM_MANAGED_MEMORY_H_

namespace Reuben { namespace System {

	struct Memory
	{

		//---------------------------------------------------------
		//					 Allocate Heap Function
		//---------------------------------------------------------

		// TEMPLATE FUNCTION __Allocate
		template<typename _TYPE_>
			static INLINE _TYPE_ * __FASTCALL __Allocate(Size count, PCSTRING pSourceLine);

		//---------------------------------------------------------
		//					 Release Heap Function
		//---------------------------------------------------------

		// TEMPLATE FUNCTION __Deallocate
		template <typename _TYPE_>
			static INLINE VOID __FASTCALL __Deallocate(_TYPE_ *& p, PCSTRING pSourceLine);

		// TEMPLATE FUNCTION __Destruct
		template <typename _TYPE_>
			static INLINE VOID __FASTCALL __Destruct(_TYPE_ * p);

		template<typename _TYPE_>
			static INLINE VOID __FASTCALL __DeleteObject(_TYPE_ *& pObject, PCSTRING pSourceLine);

		template<typename _TYPE_>
			static INLINE VOID __FASTCALL __Delete(_TYPE_ *& pObject, PCSTRING pSourceLine);

		template<typename _TYPE_>
			static INLINE VOID __FASTCALL __DeleteArray(_TYPE_ *& pObject, PCSTRING pSourceLine);

		template<typename _TYPE_>
			static INLINE VOID __FASTCALL __Free(_TYPE_ *& pObject, PCSTRING pSourceLine);

		template<typename _TYPE_>
			static INLINE VOID __FASTCALL __Release(_TYPE_ *& pObject, PCSTRING pSourceLine);

		static INLINE ErrorCode __FASTCALL __CheckMemory(CONST VOID* pBlock);

	};

	template <typename _TYPE_>
	struct __SafeDeleteObjectProc;

	template <typename _TYPE_>
	struct __SafeDeallocateProc;

	template <typename _TYPE_>
	struct __SafeDeleteProc;

	template <typename _TYPE_>
	struct __SafeDeleteArrayProc;

	template <typename _TYPE_>
	struct __SafeFreeProc;

	template <typename _TYPE_>
	struct __SafeReleaseProc;

	template <typename _TYPE_, typename _DP_>
	struct __SafeAutoPointer;

}; };  // Reuben::System

//---------------------------------------------------------
//					 Marco
//---------------------------------------------------------

//-- Allocate

#define SafeCreateObject(_type_) \
	(new ((VOID*)Reuben::System::Memory::__Allocate<_type_>(1, _SOURCE_LINE_)) _type_())
#define SafeCreateObject1(_type_, _arg1_) \
	(new ((VOID*)Reuben::System::Memory::__Allocate<_type_>(1, _SOURCE_LINE_)) _type_(_arg1_))
#define SafeCreateObject2(_type_, _arg1_, _arg2_) \
	(new ((VOID*)Reuben::System::Memory::__Allocate<_type_>(1, _SOURCE_LINE_)) _type_(_arg1_, _arg2_))
#define SafeCreateObject3(_type_, _arg1_, _arg2_, _arg3_) \
	(new ((VOID*)Reuben::System::Memory::__Allocate<_type_>(1, _SOURCE_LINE_)) _type_(_arg1_, _arg2_, _arg3_))
#define SafeCreateObject4(_type_, _arg1_, _arg2_, _arg3_, _arg4_) \
	(new ((VOID*)Reuben::System::Memory::__Allocate<_type_>(1, _SOURCE_LINE_)) _type_(_arg1_, _arg2_, _arg3_, _arg4_))
#define SafeCreateObject5(_type_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	(new ((VOID*)Reuben::System::Memory::__Allocate<_type_>(1, _SOURCE_LINE_)) _type_(_arg1_, _arg2_, _arg3_, _arg4_, _arg5_))
#define SafeCreateObject6(_type_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	(new ((VOID*)Reuben::System::Memory::__Allocate<_type_>(1, _SOURCE_LINE_)) _type_(_arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_))
#define SafeCreateObject7(_type_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	(new ((VOID*)Reuben::System::Memory::__Allocate<_type_>(1, _SOURCE_LINE_)) _type_(_arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_))
#define SafeCreateObject8(_type_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	(new ((VOID*)Reuben::System::Memory::__Allocate<_type_>(1, _SOURCE_LINE_)) _type_(_arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_))
#define SafeCreateObject9(_type_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	(new ((VOID*)Reuben::System::Memory::__Allocate<_type_>(1, _SOURCE_LINE_)) _type_(_arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_))

#define SafeAllocate(_type_, _count_) \
	Reuben::System::Memory::__Allocate<_type_>(_count_, _SOURCE_LINE_)

//-- Deallocate

#define SafeDeleteObject(_object_) \
	Reuben::System::Memory::__DeleteObject(_object_, _SOURCE_LINE_)

#define SafeDeallocate(_object_) \
	Reuben::System::Memory::__Deallocate(_object_, _SOURCE_LINE_)

#define SafeDelete(_object_) \
	Reuben::System::Memory::__Delete(_object_, _SOURCE_LINE_)

#define SafeDeleteArray(_object_) \
	Reuben::System::Memory::__DeleteArray(_object_, _SOURCE_LINE_)

#define SafeFree(_object_) \
	Reuben::System::Memory::__Free(_object_, _SOURCE_LINE_)

#define SafeRelease(_object_) \
	Reuben::System::Memory::__Release(_object_, _SOURCE_LINE_)

#if defined(REUBEN_CONFIG_MEMORY_DEBUG_CHECK_ENABLE) && defined (COMPILER_DEBUG)
	#define SafeMemoryCheck(_object_) \
		Reuben::System::Memory::__CheckMemory(_object_, _SOURCE_LINE_)
#else // REUBEN_CONFIG_MEMORY_DEBUG_CHECK_ENABLE && COMPILER_DEBUG
	#define SafeMemoryCheck(_object_) 0
#endif // REUBEN_CONFIG_MEMORY_DEBUG_CHECK_ENABLE && COMPILER_DEBUG

#define SafeDeleteObjectProc Reuben::System::__SafeDeleteObjectProc

#define SafeDeallocateProc Reuben::System::__SafeDeallocateProc

#define SafeDeleteProc Reuben::System::__SafeDeleteProc

#define SafeDeleteArrayProc Reuben::System::__SafeDeleteArrayProc

#define SafeFreeProc Reuben::System::__SafeFreeProc

#define SafeReleaseProc Reuben::System::__SafeReleaseProc

#define SafeAutoPointer Reuben::System::__SafeAutoPointer

//-- Friend

#define MANAGED_OBJECT() friend Reuben::System::Memory

#endif // _REUBEN_SYSTEM_MANAGED_MEMORY_H_
