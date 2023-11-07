#pragma once
#ifndef _REUBEN_MATH_CRC_H_
#define _REUBEN_MATH_CRC_H_

namespace Reuben { namespace Math {

	CONST UInt32 CRC32Reflect(UInt32 uValue);
	VOID CRC32GenLookupTable(CONST UInt32 uPolynomial = C_UINT32(0xEDB88320)); //Reflect32(0x04C11DB7)
	CONST UInt32 CRC32(CONST Byte* byStruct, CONST UInt32 uSize);
	CONST UInt32 CRC32(CONST Byte* byStruct, CONST UInt32 uSize, UInt32& uCRC32);
	CONST UInt32 CRC32(PCSTRING pFileName);
	CONST UInt32 CRC32(PCSTRING pFileName, UInt32& uCRC32);

};}; // Reuben::Math

#endif // _REUBEN_MATH_CRC_H_
