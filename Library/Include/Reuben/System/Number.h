#pragma once
#ifndef _REUBEN_SYSTEM_NUMBER_PRIMITIVE_H_
#define _REUBEN_SYSTEM_NUMBER_PRIMITIVE_H_

//---------------------------------------------------------
//					Define Common
//---------------------------------------------------------

#undef FALSE
#undef TRUE
#undef NULL

#define FALSE   0
#define TRUE    1
#define NULL    0
#undef VOID
#define VOID	void
#undef CONST
#define CONST	const

//---------------------------------------------------------
//					Define Boolean
//---------------------------------------------------------

#define BOOLEAN8_TRUE true
#define BOOLEAN8_FALSE false
#define BOOLEAN32_TRUE 1
#define BOOLEAN32_FALSE 0
#define BOOLEAN_TRUE BOOLEAN8_TRUE
#define BOOLEAN_FALSE BOOLEAN8_FALSE

//---------------------------------------------------------
//					Define Unsigned Integer
//---------------------------------------------------------

#define UINT1_MAX_VALUE					(1u)
#define UINT2_MAX_VALUE					(3u)
#define UINT3_MAX_VALUE					(7u)
#define UINT4_MAX_VALUE					(15u)
#define UINT5_MAX_VALUE					(31u)
#define UINT6_MAX_VALUE					(63u)
#define UINT7_MAX_VALUE					(127u)
#define UINT8_MAX_VALUE					(255u)
#define UINT9_MAX_VALUE					(511u)
#define UINT10_MAX_VALUE				(1023u)
#define UINT11_MAX_VALUE				(2047u)
#define UINT12_MAX_VALUE				(4095u)
#define UINT13_MAX_VALUE				(8191u)
#define UINT14_MAX_VALUE				(16383u)
#define UINT15_MAX_VALUE				(32767u)
#define UINT16_MAX_VALUE				(65535u)
#define UINT17_MAX_VALUE				(131071u)
#define UINT18_MAX_VALUE				(262143u)
#define UINT19_MAX_VALUE				(524287u)
#define UINT20_MAX_VALUE				(1048575u)
#define UINT21_MAX_VALUE				(2097151u)
#define UINT22_MAX_VALUE				(4194303u)
#define UINT23_MAX_VALUE				(8388607u)
#define UINT24_MAX_VALUE				(16777215u)
#define UINT25_MAX_VALUE				(33554431u)
#define UINT26_MAX_VALUE				(67108863u)
#define UINT27_MAX_VALUE				(134217727u)
#define UINT28_MAX_VALUE				(268435455u)
#define UINT29_MAX_VALUE				(536870911u)
#define UINT30_MAX_VALUE				(1073741823u)
#define UINT31_MAX_VALUE				(2147483647u)
#define UINT32_MIN_VALUE				(0u)
#define UINT32_MAX_VALUE				(4294967295u)
#define UINT64_MIN_VALUE				(0u)
#define UINT64_MAX_VALUE				(18446744073709551615ull)

#define UINT8_REAL_MAX_VALUE			(255.0)
#define UINT16_REAL_MAX_VALUE			(65535.0)
#define UINT32_REAL_MAX_VALUE			(4294967295.0)
#define UINT64_REAL_MAX_VALUE			(18446744073709551615.0l)

//---------------------------------------------------------
//					Define Signed Integer
//---------------------------------------------------------

#define INT_ZERO_VALUE					(0)

#define INT8_MIN_VALUE					(-127 - 1)
#define INT8_MAX_VALUE					(127)
#define INT16_MIN_VALUE					(-32767 - 1)
#define INT16_MAX_VALUE					(32767)
#define INT32_MIN_VALUE					(-2147483647 - 1)
#define INT32_MAX_VALUE					(2147483647)
#define INT64_MIN_VALUE					(-9223372036854775807l - 1)
#define INT64_MAX_VALUE					(9223372036854775807l)

#define INT8_REAL_MIN_VALUE				(-128.0)
#define INT8_REAL_MAX_VALUE				(127.0)
#define INT16_REAL_MIN_VALUE			(-32768.0)
#define INT16_REAL_MAX_VALUE			(32767.0)
#define INT32_REAL_MIN_VALUE			(-2147483648.0)
#define INT32_REAL_MAX_VALUE			(2147483647.0)
#define INT64_REAL_MIN_VALUE			(-9223372036854775808.0l)
#define INT64_REAL_MAX_VALUE			(9223372036854775807.0l)

//---------------------------------------------------------
//					Define Real Number
//---------------------------------------------------------

#define REAL32_PRECISION				(1.19209290e-07f)
#define REAL32_SMALLEST_VALUE			(1.17549435e-37f)	// (1.17549435e-38f)
#define REAL32_MIN_VALUE				(-3.40282347e+37f)	// (-3.40282347e+38f)
#define REAL32_MAX_VALUE				(3.40282347e+37f)	// (3.40282347e+38f)
#define REAL32_SIGNIFICANT_DIGITS		(6)
#define REAL32_MIN_EXPONENT				(-37)					// (-38)
#define REAL32_MAX_EXPONENT				(37)					// (38)
#define REAL32_MAX_BIN_EXPONENT			(128)

#define REAL64_PRECISION				(2.2204460492503131e-16)
#define REAL64_SMALLEST_VALUE			(2.2250738585072014e-308)
#define REAL64_MIN_VALUE				(-1.7976931348623157e+308)
#define REAL64_MAX_VALUE				(1.7976931348623157e+308)
#define REAL64_SIGNIFICANT_DIGITS		(15)
#define REAL64_MIN_EXPONENT				(-307)
#define REAL64_MAX_EXPONENT				(308)
#define REAL64_MAX_BIN_EXPONENT			(1024)

#define REAL80_PRECISION				(1.232595164407830945955825883254353e-32l)
#define REAL80_SMALLEST_VALUE			(2.225073858507201383090232717332404e-308l)
#define REAL80_MIN_VALUE				(-1.797693134862315807937289714053023e+308l)
#define REAL80_MAX_VALUE				(1.797693134862315807937289714053023e+308l)
#define REAL80_SIGNIFICANT_DIGITS		(31)
#define REAL80_MIN_EXPONENT				(DBL_MIN_10_EXP)
#define REAL80_MAX_EXPONENT				(DBL_MAX_10_EXP)
#define REAL80_MAX_BIN_EXPONENT			(DBL_MAX_EXP)

#define REAL_BIT_COUNT 32

#if REAL_BIT_COUNT == 32
#define REAL_PRECISION				REAL32_PRECISION
#define REAL_MIN_VALUE				REAL32_MIN_VALUE
#define REAL_MAX_VALUE				REAL32_MAX_VALUE
#define REAL_SIGNIFICANT_DIGITS		REAL32_SIGNIFICANT_DIGITS
#define REAL_MIN_EXPONENT			REAL32_MIN_EXPONENT
#define REAL_MAX_EXPONENT			REAL32_MAX_EXPONENT
#define REAL_MAX_BIN_EXPONENT		REAL32_MAX_BIN_EXPONENT
#elif REAL_BIT_COUNT == 64 && !PLATFORM_PS2
#define REAL_PRECISION				REAL64_PRECISION
#define REAL_MIN_VALUE				REAL64_MIN_VALUE
#define REAL_MAX_VALUE				REAL64_MAX_VALUE
#define REAL_SIGNIFICANT_DIGITS		REAL64_SIGNIFICANT_DIGITS
#define REAL_MIN_EXPONENT			REAL64_MIN_EXPONENT
#define REAL_MAX_EXPONENT			REAL64_MAX_EXPONENT
#define REAL_MAX_BIN_EXPONENT		REAL64_MAX_BIN_EXPONENT
#else
#error Bad REAL_BIT_COUNT
#endif

//---------------------------------------------------------
//					Define Common Use Number
//---------------------------------------------------------

#if defined(PLATFORM_WIN64)
#	define UINT_MIN_VALUE			UINT64_MIN_VALUE
#	define UINT_MAX_VALUE			UINT64_MAX_VALUE

#	define INT_MIN_VALUE			INT64_MIN_VALUE
#	define INT_MAX_VALUE			INT64_MAX_VALUE
#else
#	define UINT_MIN_VALUE			UINT32_MIN_VALUE
#	define UINT_MAX_VALUE			UINT32_MAX_VALUE

#	define INT_MIN_VALUE			INT32_MIN_VALUE
#	define INT_MAX_VALUE			INT32_MAX_VALUE
#endif

#define COUNT_NONE					UINT_MIN_VALUE
#define COUNT_MIN_VALUE				UINT_MIN_VALUE
#define COUNT_MAX_VALUE				UINT_MAX_VALUE

#define INDEX_NONE					UINT_MIN_VALUE
#define INDEX_MIN_VALUE				UINT_MIN_VALUE
#define INDEX_MAX_VALUE				UINT_MAX_VALUE

#define SIZE_NONE					UINT64_MIN_VALUE
#define SIZE_MIN_VALUE				UINT64_MIN_VALUE
#define SIZE_MAX_VALUE				UINT64_MAX_VALUE

#define ERROR_CODE_NONE				INT_ZERO_VALUE
#define ERROR_CODE_MIN_VALUE		INT32_MIN_VALUE
#define ERROR_CODE_MAX_VALUE		INT32_MAX_VALUE

//---------------------------------------------------------
//					 Define Constant Function
//---------------------------------------------------------

#define C_UINT8(_constant_)			((Reuben::System::UInt8)RN_CAT_CODE(_constant_, u))
#define C_UINT16(_constant_)		((Reuben::System::UInt16)RN_CAT_CODE(_constant_, u))
#define C_UINT32(_constant_)		((Reuben::System::UInt32)RN_CAT_CODE(_constant_, u))
#define C_UINT64(_constant_)		((Reuben::System::UInt64)RN_CAT_CODE(_constant_, ull))

#define C_INT8(_constant_)			((Reuben::System::Int8)_constant_)
#define C_INT16(_constant_)			((Reuben::System::Int16)_constant_)
#define C_INT32(_constant_)			((Reuben::System::Int32)_constant_)
#define C_INT64(_constant_)			((Reuben::System::Int64)RN_CAT_CODE(_constant_, ll))

#define C_REAL32(_constant_)		((Reuben::System::Real32)RN_CAT_CODE(_constant_, f))
#define C_REAL64(_constant_)		((Reuben::System::Real64)_constant_)
#if (REAL_BIT_COUNT == 32)
#	define C_REAL(_constant_)		C_REAL32(_constant_)
#else
#	define C_REAL(_constant_)		C_REAL64(_constant_)
#endif

#define C_BIT8(_constant_)			C_UINT8(_constant_)
#define C_BIT16(_constant_)			C_UINT16(_constant_)
#define C_BIT32(_constant_)			C_UINT32(_constant_)
#define C_BIT64(_constant_)			C_UINT64(_constant_)

#define C_BYTE(_constant_)			((Reuben::System::Byte)_constant_)
#define C_UINT(_constant_)			((Reuben::System::UInt)RN_CAT_CODE(_constant_, u))
#define C_INT(_constant_)			((Reuben::System::Int)_constant_)
#define C_COUNT(_constant_)			C_UINT(_constant_)
#define C_INDEX(_constant_)			C_UINT(_constant_)
#define C_SIZE(_constant_)			((Reuben::System::Size)RN_CAT_CODE(_constant_, ull))

//---------------------------------------------------------
//					Define Size Constant
//---------------------------------------------------------

#define MAX_SIZE	C_COUNT(512)
#define MID_SIZE	C_COUNT(256)
#define MIN_SIZE	C_COUNT(128)
#define MIC_SIZE	C_COUNT(64)

#define PATH_SIZE	C_COUNT(260)

//---------------------------------------------------------
//					 Define Cast Function
//---------------------------------------------------------

#define CAST_UINT8(_variable_)		(static_cast<Reuben::System::UInt8>(_variable_))
#define CAST_UINT16(_variable_)		(static_cast<Reuben::System::UInt16>(_variable_))
#define CAST_UINT32(_variable_)		(static_cast<Reuben::System::UInt32>(_variable_))
#define CAST_UINT64(_variable_)		(static_cast<Reuben::System::UInt64>(_variable_))

#define CAST_INT8(_variable_)		(static_cast<Reuben::System::Int8>(_variable_))
#define CAST_INT16(_variable_)		(static_cast<Reuben::System::Int16>(_variable_))
#define CAST_INT32(_variable_)		(static_cast<Reuben::System::Int32>(_variable_))
#define CAST_INT64(_variable_)		(static_cast<Reuben::System::Int64>(_variable_))

#define CAST_REAL32(_variable_)		(static_cast<Reuben::System::Real32>(_variable_))
#define CAST_REAL64(_variable_)		(static_cast<Reuben::System::Real64>(_variable_))
#if (REAL_BIT_COUNT == 32)
#	define CAST_REAL(_variable_)	CAST_REAL32(_variable_)
#else
#	define CAST_REAL(_variable_)	CAST_REAL64(_variable_)
#endif

#define CAST_BIT8(_variable_)		CAST_UINT8(_variable_)
#define CAST_BIT16(_variable_)		CAST_UINT16(_variable_)
#define CAST_BIT32(_variable_)		CAST_UINT32(_variable_)
#define CAST_BIT64(_variable_)		CAST_UINT64(_variable_)

#define CAST_BYTE(_variable_)		(static_cast<Reuben::System::Byte>(_variable_))
#define CAST_UINT(_variable_)		(static_cast<Reuben::System::UInt>(_variable_))
#define CAST_INT(_variable_)		(static_cast<Reuben::System::Int>(_variable_))
#define CAST_COUNT(_variable_)		(static_cast<Reuben::System::Count>(_variable_))
#define CAST_INDEX(_variable_)		(static_cast<Reuben::System::Index>(_variable_))
#define CAST_SIZE(_variable_)		(static_cast<Reuben::System::Size>(_variable_))

namespace Reuben { namespace System {

	//---------------------------------------------------------
	//					 Boolean
	//---------------------------------------------------------

	typedef bool Boolean8;
	typedef int Boolean32;
	typedef Boolean8 Boolean;

	//---------------------------------------------------------
	//					 Unsigned Integer
	//---------------------------------------------------------

	typedef unsigned __int8  UInt8,  * PUInt8;
	typedef unsigned __int16 UInt16, * PUInt16;
	typedef unsigned __int32 UInt32, * PUInt32;
	typedef unsigned __int64 UInt64, * PUInt64;

	//---------------------------------------------------------
	//					 Signed Integer
	//---------------------------------------------------------

	typedef signed __int8  Int8,  * PInt8;
	typedef signed __int16 Int16, * PInt16;
	typedef signed __int32 Int32, * PInt32;
	typedef signed __int64 Int64, * PInt64;

	//---------------------------------------------------------
	//					 Real Number
	//---------------------------------------------------------

	typedef float Real32;
	typedef double Real64;
	typedef long double Real80;

#if REAL_BIT_COUNT == 32
	typedef Real32 Real;
#elif REAL_BIT_COUNT == 64 && !PLATFORM_PS2
	typedef Real64 Real;
#else
#error Bad REAL_BIT_COUNT
#endif

	//---------------------------------------------------------
	//					 Common Use Number
	//---------------------------------------------------------

	typedef unsigned char Byte;
	typedef unsigned int UInt;
	typedef int Int;
	typedef UInt Count;
	typedef UInt Index;
	typedef size_t Size;

	//---------------------------------------------------------
	//					 Color
	//---------------------------------------------------------

	typedef unsigned __int8  Color8;
	typedef unsigned __int16 Color16;
	typedef unsigned __int32 Color32;

	//---------------------------------------------------------
	//					 Char
	//---------------------------------------------------------

	typedef char CharA;
	typedef wchar_t CharW;

#if defined (COMPILER_UNICODE)
	typedef CharW Char;
#else
	typedef CharA Char;
#endif

	//---------------------------------------------------------
	//					 String Pointer
	//---------------------------------------------------------

	typedef CharA * PSTRINGA;
	typedef CharW * PSTRINGW;

	typedef CONST CharA * PCSTRINGA;
	typedef CONST CharW * PCSTRINGW;

#if defined (COMPILER_UNICODE)
	typedef PSTRINGW PSTRING;
	typedef PCSTRINGW PCSTRING;
#else
	typedef PSTRINGA PSTRING;
	typedef PCSTRINGA PCSTRING;
#endif

	//---------------------------------------------------------
	//					 Bit
	//---------------------------------------------------------

	typedef UInt8 Bit8;
	typedef UInt16 Bit16;
	typedef UInt32 Bit32;
	typedef UInt64 Bit64;

}; }; // Reuben::System

#endif // _REUBEN_SYSTEM_NUMBER_PRIMITIVE_H_

