#pragma once
#ifndef _REUBEN_SYSTEM_OPTION_H_
#define _REUBEN_SYSTEM_OPTION_H_

//---------------------------------------------------------
//					C++ Library
//---------------------------------------------------------

#if !defined(__cplusplus)
#	error Reuben library only support on C++
#endif

//---------------------------------------------------------
//					Platform Option
//---------------------------------------------------------

#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#	define PLATFORM_WINDOWS
#	define PLATFORM_WIN32
#elif defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
#	define PLATFORM_WINDOWS
#	define PLATFORM_WIN64
#elif defined(__linux__)
#	define PLATFORM_LINUX
#elif defined(macintosh) || (defined(__APPLE__) && defined(__MACH__))
#	define PLATFORM_MACOS
#else
#	error Reuben library only support on Windows, Linux and Mac
#endif

//---------------------------------------------------------
//					Compiler Option
//---------------------------------------------------------

#if defined(_MSC_VER)
#	define COMPILER_VC _MSC_VER
#	if (_MSC_VER < 1300)				//-- 1200 = VC6
#		define COMPILER_VC6 COMPILER_VC
#	elif (_MSC_VER < 1400)				//-- 1300 = VC7
#		define COMPILER_VC7 COMPILER_VC
#	elif (_MSC_VER < 1500)				//-- 1400 = VC8
#		define COMPILER_VC8 COMPILER_VC
#	endif
#elif defined(__GNUC__) || defined(__BEOS__) || defined(__linux__) || defined(__CYGWIN__)
#	define COMPILER_GCC __GNUC__
#elif defined(__BORLANDC__)
#	define COMPILER_BORLAND __BORLANDC__
#else
#	error Reuben library only support on Visual C, Borland C and GCC
#endif

#if defined(_MFC_VER)
#	define COMPILER_MFC _MFC_VER
#endif

#if defined(_DEBUG) || defined(DEBUG)
#	define COMPILER_DEBUG
#endif

#if defined(UNICODE) || defined(_UNICODE)
#	define COMPILER_UNICODE
#endif

#if defined(_MT)
#	define COMPILER_MULTITHREADING
#endif

#if defined(_CPPRTTI)
#	define COMPILER_RTTI
#endif

#if defined(_CPPUNWIND)
#	define COMPILER_EXCEPTION_HANDLING
#endif

#if defined(_CHAR_UNSIGNED)
#	define COMPILER_UNSIGNED_CHARACTER
#endif

//---------------------------------------------------------
//					 Function Call Symbol
//---------------------------------------------------------

#define __CDECL			__cdecl
#define __STDCALL		__stdcall
#define __FASTCALL		__fastcall

//---------------------------------------------------------
//					Compiler Option Restrict
//---------------------------------------------------------

#if (REUBEN_VER == REUBEN_VER_0_0)
#	if !defined(COMPILER_VC)
#		error Reuben Library 0.0 only support on Visual C
#	endif
#endif

#if defined(COMPILER_UNSIGNED_CHARACTER)
#	error Character must be signed
#endif

//---------------------------------------------------------
//					INLINE
//---------------------------------------------------------

#if defined(COMPILER_VC)
#	pragma inline_depth(32)
#	define INLINE __forceinline
#else
#	define INLINE inline
#endif

//---------------------------------------------------------
//					Define Concatenate Code
//---------------------------------------------------------

#define RN_CAT_CODE(_prefix_, _postfix_)		_prefix_##_postfix_

//---------------------------------------------------------
//					 Unique Variable Name
//---------------------------------------------------------

#define RN_GET_UNIQUE_NAME_2(_variable_name_, _postfix_)	RN_CAT_CODE(_variable_name_, _postfix_)
#define RN_GET_UNIQUE_NAME(_variable_name_)					RN_GET_UNIQUE_NAME_2(_variable_name_, __LINE__)

//---------------------------------------------------------
//					 Source Line String
//---------------------------------------------------------

#define RN_GET_QUOTED_CODE( _code_ )			_T(#_code_)
#define RN_GET_PROCESSED_CODE( _code_ )			_code_

#define RN_GET_QUOTED_CONSTANT_3(_constant_)	RN_GET_QUOTED_CODE(_constant_)
#define RN_GET_QUOTED_CONSTANT_2(_constant_)	RN_GET_QUOTED_CONSTANT_3(RN_GET_PROCESSED_CODE(_constant_))
#define RN_GET_QUOTED_CONSTANT(_constant_)		RN_GET_QUOTED_CONSTANT_2(_constant_)

#define _SOURCE_LINE_							_T(__FILE__) _T(" (") RN_GET_QUOTED_CONSTANT(__LINE__) _T(") : ")

//---------------------------------------------------------
//					 Zero Pointer
//---------------------------------------------------------

#define ZERO_STRUCT(_pointer_, _struct_) \
	memset(_pointer_, 0, sizeof(_struct_))

#define ZERO_DATA(_pointer_, _size_) \
	memset(_pointer_, 0, _size_)

#endif // _REUBEN_SYSTEM_OPTION_H_
