#pragma once
#ifndef _REUBEN_SYSTEM_STRING_PRIMITIVE_H_
#define _REUBEN_SYSTEM_STRING_PRIMITIVE_H_

#if defined(REUBEN_DLL)
	#define STDSTRING_DLL
	#if defined(REUBEN_EXPORTS)
		#define STDSTRING_EXPORTS
	#endif
#endif
#define SS_NO_IMPLICIT_CAST
#include <StdString.h>

//---------------------------------------------------------
//					Define String
//---------------------------------------------------------

#if defined(PLATFORM_WINDOWS)
#	define PATH_SLASH		_T("\\")
#	define PATH_SLASH_CHAR	_T('\\')
#else
#	define PATH_SLASH		_T("//")
#	define PATH_SLASH_CHAR	_T('//')
#endif

#define STRING_EMPTY		_T("")
#define STRING_NULL			_T("NULL")
// :  warning C4005: 'STRING_NONE' : macro redefinition, C:\Program Files\Microsoft SDKs\Windows\v7.1\Include\winspool.h(875)
#pragma warning(push)
#pragma warning(disable: 4005)
#define STRING_NONE			_T("NONE")
#pragma warning(pop)

namespace Reuben { namespace System {

	//-- A
	typedef CStdStr<char, ManagedAllocator<char> >			StringA;

	//-- W
	typedef CStdStr<wchar_t, ManagedAllocator<wchar_t> >	StringW;

}; }; // Reuben::System

INLINE Reuben::System::StringA operator + (CONST Reuben::System::StringA & s1, CONST Reuben::System::StringA & s2)
{
	Reuben::System::StringA sRet(SSREF(s1));
	sRet.append(s2);
	return (sRet);
}

INLINE Reuben::System::StringA operator + (CONST Reuben::System::StringA & s1, Reuben::System::StringA::value_type t)
{
	Reuben::System::StringA sRet(SSREF(s1));
	sRet.append(1, t);
	return (sRet);
}

INLINE Reuben::System::StringA operator + (CONST Reuben::System::StringA & s1, PCSTRINGA pA)
{
	Reuben::System::StringA sRet(SSREF(s1));
	sRet.append(pA);
	return (sRet);
}

INLINE Reuben::System::StringA operator + (PCSTRINGA pA, CONST Reuben::System::StringA & sA)
{
	Reuben::System::StringA sRet;
	Reuben::System::StringA::size_type nObjSize = sA.size();
	Reuben::System::StringA::size_type nLitSize = static_cast<Reuben::System::StringA::size_type>(sslen(pA));
	sRet.reserve(nLitSize + nObjSize);
	sRet.assign(pA);
	sRet.append(sA);
	return (sRet);
}

INLINE Reuben::System::StringA operator + (CONST Reuben::System::StringA & s1, CONST Reuben::System::StringW & s2)
{
	return (s1 + Reuben::System::StringA(s2.c_str()));
}

INLINE Reuben::System::StringW operator + (CONST Reuben::System::StringW & s1, CONST Reuben::System::StringW & s2)
{
	Reuben::System::StringW sRet(SSREF(s1));
	sRet.append(s2);
	return (sRet);
}

INLINE Reuben::System::StringA operator + (CONST Reuben::System::StringA & s1, PCSTRINGW pW)
{
	return (s1 + Reuben::System::StringA(pW));
}

#if defined(COMPILER_UNICODE)

INLINE Reuben::System::StringW operator + (PCSTRINGW pW, CONST Reuben::System::StringA & sA)
{
	return (Reuben::System::StringW(pW) + Reuben::System::StringW(sA.c_str()));
}

INLINE Reuben::System::StringW operator + (PCSTRINGA pA, CONST Reuben::System::StringW & sW)
{
	return (Reuben::System::StringW(pA) + sW);
}

#else // COMPILER_UNICODE

INLINE Reuben::System::StringA operator + (PCSTRINGW pW, CONST Reuben::System::StringA & sA)
{
	return (Reuben::System::StringA(pW) + sA);
}

INLINE Reuben::System::StringA operator + (PCSTRINGA pA, CONST Reuben::System::StringW & sW)
{
	return (pA + Reuben::System::StringA(sW.c_str()));
}

#endif // COMPILER_UNICODE

INLINE Reuben::System::StringW operator + (CONST Reuben::System::StringW & s1, Reuben::System::StringW::value_type t)
{
	Reuben::System::StringW sRet(SSREF(s1));
	sRet.append(1, t);
	return (sRet);
}

INLINE Reuben::System::StringW operator + (CONST Reuben::System::StringW & s1, PCSTRINGW pW)
{
	Reuben::System::StringW sRet(SSREF(s1));
	sRet.append(pW);
	return (sRet);
}

INLINE Reuben::System::StringW operator + (PCSTRINGW pW, CONST Reuben::System::StringW & sW)
{
	Reuben::System::StringW sRet;
	Reuben::System::StringW::size_type nObjSize = sW.size();
	Reuben::System::StringW::size_type nLitSize = static_cast<Reuben::System::StringW::size_type>(sslen(pW));
	sRet.reserve(nLitSize + nObjSize);
	sRet.assign(pW);
	sRet.append(sW);
	return (sRet);
}

INLINE Reuben::System::StringW operator + (CONST Reuben::System::StringW & s1, CONST Reuben::System::StringA & s2)
{
	return (s1 + Reuben::System::StringW(s2.c_str()));
}

INLINE Reuben::System::StringW operator + (CONST Reuben::System::StringW & s1, PCSTRINGA pA)
{
	return (s1 + Reuben::System::StringW(pA));
}

#ifdef SS_SAFE_FORMAT

template<>
struct FmtArg<Reuben::System::StringA>
{
	explicit FmtArg(CONST Reuben::System::StringA & arg) : a_(arg) {}
	PCSTRINGA operator()() CONST { return (a_.c_str()); }
	CONST Reuben::System::StringA & a_;
private:
	FmtArg<Reuben::System::StringA> & operator = (CONST FmtArg<Reuben::System::StringA> &) { return (*this); }
};

template<>
struct FmtArg<Reuben::System::StringW>
{
	explicit FmtArg(CONST Reuben::System::StringW & arg) : a_(arg) {}
	PCSTRINGW operator()() CONST { return (a_.c_str()); }
	CONST Reuben::System::StringW & a_;
private:
	FmtArg<Reuben::System::StringW> & operator = (CONST FmtArg<Reuben::System::StringW> &) { return (*this); }
};

#endif // #ifdef SS_SAFEFORMAT

#ifdef COMPILER_MFC

INLINE CArchive & AFXAPI operator << (CArchive & ar, CONST Reuben::System::StringA & strA)
{
	CString strTemp(strA.c_str());
	return (ar << strTemp);
}

INLINE CArchive & AFXAPI operator << (CArchive & ar, CONST Reuben::System::StringW & strW)
{
	CString strTemp(strW.c_str());
	return (ar << strTemp);
}

INLINE CArchive & AFXAPI operator >> (CArchive& ar, Reuben::System::StringA & strA)
{
	CString strTemp;
	ar >> strTemp;
	strA = strTemp;
	return (ar);
}

INLINE CArchive & AFXAPI operator >> (CArchive & ar, Reuben::System::StringW & strW)
{
	CString strTemp;
	ar >> strTemp;
	strW = strTemp;
	return (ar);
}

#endif	// COMPILER_MFC

namespace Reuben { namespace System {

	//-- T
#if defined(COMPILER_UNICODE)
	typedef StringW				String;
#else // COMPILER_UNICODE
	typedef StringA				String;
#endif // COMPILER_UNICODE

	//-- Null to String Empty

	INLINE PCSTRING NULL2EMPTY(PCSTRING pString)
	{
		return (pString == NULL) ? STRING_EMPTY : pString;
	}

	//-- Null to String Null

	INLINE PCSTRING NULL2NULL(PCSTRING pString)
	{
		return (pString == NULL) ? STRING_NULL : pString;
	}

	//-- Null to String None

	INLINE PCSTRING NULL2NONE(PCSTRING pString)
	{
		return (pString == NULL) ? STRING_NONE : pString;
	}

	//-- Char to Visible Char

	INLINE CharA Char2VisbleChar(CONST CharA c)
	{
		return (c < 0x20 || c > 0x7E) ? '.' : c;
	}

	INLINE CONST String IntToString(CONST Int nValue)
	{
		String strValue;
		_itot(nValue, strValue.GetBuffer(PATH_SIZE), 10);
		strValue.ReleaseBuffer();
		return (strValue);
	}

	INLINE CONST Int StringToInt(PCSTRING pValue)
	{
		return ((Int)_ttoi(pValue));
	}

}; }; // Reuben::System

#define C2VC(_char_) Reuben::System::Char2VisbleChar(_char_)

#endif // _REUBEN_SYSTEM_STRING_PRIMITIVE_H_
