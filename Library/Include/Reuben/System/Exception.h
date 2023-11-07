#pragma once
#ifndef _REUBEN_SYSTEM_EXCEPTION_H_
#define _REUBEN_SYSTEM_EXCEPTION_H_

namespace Reuben { namespace System {

	class Exception
	{
	protected:

		ErrorCode m_ec;
		String m_strSourceLine;
		String m_strMessage;

	public:

		INLINE Exception(CONST ErrorCode ec, PCSTRING pSourceLine);
		INLINE Exception(CONST ErrorCode ec, PCSTRING pSourceLine, PCSTRING pMessage);
		
		virtual CONST String GetErrorMessage(VOID);
		INLINE CONST ErrorCode GetErrorCode(VOID) CONST;
		INLINE CONST String & GetSourceLine(VOID) CONST;
		INLINE CONST String & GetMessage(VOID) CONST;

	};

	INLINE Exception::Exception(CONST ErrorCode ec, PCSTRING pSourceLine)
		: m_ec(ec)
		, m_strSourceLine(pSourceLine)
		, m_strMessage()
	{
	}

	INLINE Exception::Exception(CONST ErrorCode ec, PCSTRING pSourceLine, PCSTRING pMessage)
		: m_ec(ec)
		, m_strSourceLine(pSourceLine)
		, m_strMessage(pMessage)
	{
	}

	INLINE CONST ErrorCode Exception::GetErrorCode(VOID) CONST
	{
		return (m_ec);
	}

	INLINE CONST String & Exception::GetSourceLine(VOID) CONST
	{
		return (m_strSourceLine);
	}

	INLINE CONST String & Exception::GetMessage(VOID) CONST
	{
		return (m_strMessage);
	}

}; }; // Reuben::System

#endif // _REUBEN_SYSTEM_EXCEPTION_H_
