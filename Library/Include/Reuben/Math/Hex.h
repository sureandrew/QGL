#pragma once
#ifndef _REUBEN_MATH_HEX_H_
#define _REUBEN_MATH_HEX_H_

namespace Reuben { namespace Math {

	Boolean BytesToHex(PSTRING pDest, Size& len, CONST Byte* pSource, CONST Size size);
	INLINE Boolean BytesToHex(String& strDest, CONST Byte* pSource, CONST Size size)
	{
		Size len = 0;
		BytesToHex(NULL, len, pSource, size);
		PSTRING pDest = strDest.GetBuffer((Int)len + 1);
		Boolean bResult = BytesToHex(pDest, len, pSource, size);
		*(pDest + len + 1) = _T('\0');
		strDest.ReleaseBuffer();
		return (bResult);
	}

	Boolean HexToBytes(Byte* pDest, Size& size, PCSTRING pSource, CONST Size len);
	INLINE Boolean HexToBytes(Byte* pDest, Size& size, CONST String& strSource)
	{
		return (HexToBytes(pDest, size, strSource.c_str(), strSource.GetLength()));
	}

};}; // Reuben::Math

#endif // _REUBEN_MATH_HEX_H_
