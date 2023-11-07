#pragma once
#ifndef _REUBEN_DIAGNOSTICS_TRACE_H_
#define _REUBEN_DIAGNOSTICS_TRACE_H_

namespace Reuben { namespace Diagnostics {

	class TraceException : public Exception
	{
	private:

		String m_strStatement;

	public:

		TraceException(ErrorCode ec, PCSTRING pSourceLine, PCSTRING pStatement)
			: Exception(ec, pSourceLine)
			, m_strStatement(pStatement)
		{
		}

		TraceException(ErrorCode ec, PCSTRING pSourceLine, PCSTRING pStatement, PCSTRING pMessage)
			: Exception(ec, pSourceLine, pMessage)
			, m_strStatement(pStatement)
		{
		}

		INLINE virtual CONST String GetErrorMessage(VOID);
		INLINE CONST String & GetStatement(VOID) CONST;

	};

	INLINE CONST String TraceException::GetErrorMessage(VOID)
	{
		String strErrorMessage;
		strErrorMessage.Format(_T("%s [%s 0x%08X] Statement = (%s) %s"), m_strSourceLine.c_str(), NULL2NONE(GetErrorCodeString(m_ec)), m_ec, m_strStatement.c_str(), m_strMessage.c_str());
		return (strErrorMessage);
	}

	INLINE CONST String & TraceException::GetStatement(VOID) CONST
	{
		return (m_strStatement);
	}

}; }; // Reuben::Diagnostics

#if defined(REUBEN_CONFIG_TRACE_ENABLE)

#if defined(COMPILER_DEBUG)
	#define TRACE_BREAK //DebugBreak();
#else // COMPILER_DEBUG
	#define TRACE_BREAK
#endif // COMPILER_DEBUG

#define TRACE_BASIC(_error_code_, _statement_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		throw Reuben::Diagnostics::TraceException(_error_code_, _SOURCE_LINE_, _T(#_statement_)); \
	} \
}

#define TRACE_BASIC_0(_error_code_, _statement_, _message_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		throw Reuben::Diagnostics::TraceException(_error_code_, _SOURCE_LINE_, _T(#_statement_), _message_); \
	} \
}

#define TRACE_BASIC_1(_error_code_, _statement_, _message_, _arg1_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_); \
		throw Reuben::Diagnostics::TraceException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define TRACE_BASIC_2(_error_code_, _statement_, _message_, _arg1_, _arg2_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_); \
		throw Reuben::Diagnostics::TraceException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define TRACE_BASIC_3(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_); \
		throw Reuben::Diagnostics::TraceException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define TRACE_BASIC_4(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_, _arg4_); \
		throw Reuben::Diagnostics::TraceException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define TRACE_BASIC_5(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); \
		throw Reuben::Diagnostics::TraceException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define TRACE_BASIC_6(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); \
		throw Reuben::Diagnostics::TraceException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define TRACE_BASIC_7(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); \
		throw Reuben::Diagnostics::TraceException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define TRACE_BASIC_8(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); \
		throw Reuben::Diagnostics::TraceException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define TRACE_BASIC_9(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); \
		throw Reuben::Diagnostics::TraceException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define TRACE_ISWRITEPOINTER(_pointer_) \
{ \
	if (IsBadWritePtr((LPVOID)_pointer_, sizeof(LPVOID))) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_T("Pointer = 0x%08X is not a valid write pointer"), _pointer_); \
		throw Reuben::Diagnostics::TraceException(EC_WRITE_INVALID, _SOURCE_LINE_, _T(#_pointer_), strMessage.c_str()); \
	} \
}

#define TRACE_ISREADPOINTER(_pointer_) \
{ \
	if (IsBadReadPtr((LPVOID)_pointer_, sizeof(LPVOID))) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_T("Pointer = 0x%08X is not a valid read pointer"), _pointer_); \
		throw Reuben::Diagnostics::TraceException(EC_READ_INVALID, _SOURCE_LINE_, _T(#_pointer_), strMessage.c_str()); \
	} \
}

#define TRACE_ISWRITEDATA(_pointer_, _size_) \
{ \
	if (IsBadWritePtr((LPVOID)_pointer_, _size_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_T("Pointer = 0x%08X, Size = %d is not a valid write area"), _pointer_, _size_); \
		throw Reuben::Diagnostics::TraceException(EC_WRITE_INVALID, _SOURCE_LINE_, _T(#_pointer_) _T(" ") _T(#_size_), strMessage.c_str()); \
	} \
}

#define TRACE_ISREADDATA(_pointer_, _size_) \
{ \
	if (IsBadReadPtr((LPVOID)_pointer_, _size_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_T("Pointer = 0x%08X, Size = %d is not a valid read area"), _pointer_, _size_); \
		throw Reuben::Diagnostics::TraceException(EC_READ_INVALID, _SOURCE_LINE_, _T(#_pointer_) _T(" ") _T(#_size_), strMessage.c_str()); \
	} \
}

#define STRING_UNKNOWN_MODULE	_T("Unknown Module")

#define TRACE_CALL_STACK_BASE(_logger_, _ebp_) \
{ \
	MEMORY_BASIC_INFORMATION __mem_info__; \
	String __module_name__; \
	UInt* __ebp__ = _ebp_; \
	UInt __prev_frame__; \
	UInt __return_addr__; \
	UInt __counter__ = C_UINT(0); \
	do \
	{ \
		__prev_frame__ = C_UINT(0); \
		__return_addr__ = C_UINT(0); \
		if (!IsBadReadPtr(__ebp__, sizeof(UInt))) \
			__prev_frame__ = *__ebp__; \
		if (!IsBadReadPtr(__ebp__ + 1, sizeof(UInt))) \
			__return_addr__ = *(__ebp__ + 1); \
		if (VirtualQuery(UIntToPtr(__return_addr__), &__mem_info__, sizeof(MEMORY_BASIC_INFORMATION)) != 0) \
		{ \
			GetModuleFileName((HMODULE)__mem_info__.AllocationBase, __module_name__.GetBuffer(MAX_SIZE), MAX_SIZE); \
			__module_name__.ReleaseBuffer(); \
		} \
		else \
			__module_name__ = STRING_UNKNOWN_MODULE; \
		TRACE_ALWAYSL_5(_logger_, _T("EBP=0x%08X Frame=0x%08X Return=0x%08X Logical_Addr=%04X:%08X "), \
			__ebp__, __prev_frame__, __return_addr__, __mem_info__.Type, __return_addr__ - (UInt)__mem_info__.BaseAddress - 4096);	\
		TRACE_ALWAYSL_1(_logger_, _T("Module=%s"), __module_name__.c_str()); \
		TRACE_ALWAYS(_logger_, _T("\t")); \
		for (Index i = 0; i < 10; ++i) \
		{ \
			UInt* param = (__ebp__ + (i +2)); \
			if (!IsBadReadPtr(param, sizeof(UInt))) \
			{ \
				TRACE_ALWAYS_2(_logger_, _T("Param%d=0x%08X "), i + 1, *param); \
			} \
			else \
			{ \
				TRACE_ALWAYS_1(_logger_, _T("Param%d=BadReadPtr "), i + 1); \
			} \
			if (i == 4) \
			{ \
				TRACE_ALWAYSL(_logger_, _T("")); \
				TRACE_ALWAYS(_logger_, _T("\t")); \
			} \
		} \
		TRACE_ALWAYSL(_logger_, _T("")); \
		__ebp__ = (UInt*)UIntToPtr(__prev_frame__); \
		++__counter__; \
	} \
	while (__prev_frame__ != C_UINT(0) && __return_addr__ != C_UINT(0) && __counter__ < C_UINT(100)); \
}

#define TRACE_CALL_STACK(_logger_) \
{ \
	UInt* ___ebp___; \
	__asm { mov dword ptr [___ebp___], ebp }; \
	TRACE_CALL_STACK_BASE(_logger_, ___ebp___); \
}

#define TRACE_CHECK(_statement_) \
	TRACE_BASIC(EC_CHECK_INVALID, _statement_)
#define TRACE_CHECK_0(_statement_, _message_) \
	TRACE_BASIC_0(EC_CHECK_INVALID, _statement_, _message_)
#define TRACE_CHECK_1(_statement_, _message_, _arg1_) \
	TRACE_BASIC_1(EC_CHECK_INVALID, _statement_, _message_, _arg1_)
#define TRACE_CHECK_2(_statement_, _message_, _arg1_, _arg2_) \
	TRACE_BASIC_2(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_)
#define TRACE_CHECK_3(_statement_, _message_, _arg1_, _arg2_, _arg3_) \
	TRACE_BASIC_3(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_)
#define TRACE_CHECK_4(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_BASIC_4(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_CHECK_5(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_BASIC_5(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_CHECK_6(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_BASIC_6(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_CHECK_7(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_BASIC_7(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_CHECK_8(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_BASIC_8(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_CHECK_9(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_BASIC_9(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_ENSURE(_statement_) \
	TRACE_BASIC(EC_ENSURE_INVALID, _statement_)
#define TRACE_ENSURE_0(_statement_, _message_) \
	TRACE_BASIC_0(EC_ENSURE_INVALID, _statement_, _message_)
#define TRACE_ENSURE_1(_statement_, _message_, _arg1_) \
	TRACE_BASIC_1(EC_ENSURE_INVALID, _statement_, _message_, _arg1_)
#define TRACE_ENSURE_2(_statement_, _message_, _arg1_, _arg2_) \
	TRACE_BASIC_2(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_)
#define TRACE_ENSURE_3(_statement_, _message_, _arg1_, _arg2_, _arg3_) \
	TRACE_BASIC_3(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_)
#define TRACE_ENSURE_4(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_BASIC_4(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ENSURE_5(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_BASIC_5(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ENSURE_6(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_BASIC_6(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ENSURE_7(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_BASIC_7(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ENSURE_8(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_BASIC_8(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ENSURE_9(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_BASIC_9(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_LOG(_log_, _lv_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.Write(_msg_); }
#define TRACE_LOG_1(_log_, _lv_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_); }
#define TRACE_LOG_2(_log_, _lv_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_); }
#define TRACE_LOG_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_); }
#define TRACE_LOG_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); }
#define TRACE_LOG_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); }
#define TRACE_LOG_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); }
#define TRACE_LOG_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); }
#define TRACE_LOG_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); }
#define TRACE_LOG_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); }
#define TRACE_LOGL(_log_, _lv_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteLine(_msg_); }
#define TRACE_LOGL_1(_log_, _lv_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_); }
#define TRACE_LOGL_2(_log_, _lv_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_); }
#define TRACE_LOGL_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_); }
#define TRACE_LOGL_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); }
#define TRACE_LOGL_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); }
#define TRACE_LOGL_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); }
#define TRACE_LOGL_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); }
#define TRACE_LOGL_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); }
#define TRACE_LOGL_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); }

#define TRACE_LOGDT(_log_, _lv_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.Write(_msg_); } }
#define TRACE_LOGDT_1(_log_, _lv_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_); } }
#define TRACE_LOGDT_2(_log_, _lv_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_); } }
#define TRACE_LOGDT_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_); } }
#define TRACE_LOGDT_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); } }
#define TRACE_LOGDT_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); } }
#define TRACE_LOGDT_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); } }
#define TRACE_LOGDT_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); } }
#define TRACE_LOGDT_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); } }
#define TRACE_LOGDT_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); } }
#define TRACE_LOGDTL(_log_, _lv_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteLine(_msg_); } }
#define TRACE_LOGDTL_1(_log_, _lv_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_); } }
#define TRACE_LOGDTL_2(_log_, _lv_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_); } }
#define TRACE_LOGDTL_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_); } }
#define TRACE_LOGDTL_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); } }
#define TRACE_LOGDTL_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); } }
#define TRACE_LOGDTL_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); } }
#define TRACE_LOGDTL_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); } }
#define TRACE_LOGDTL_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); } }
#define TRACE_LOGDTL_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); } }

#define TRACE_LOG_IF(_log_, _lv_, _check_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.Write(_msg_); }
#define TRACE_LOG_IF_1(_log_, _lv_, _check_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_); }
#define TRACE_LOG_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_); }
#define TRACE_LOG_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_); }
#define TRACE_LOG_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); }
#define TRACE_LOG_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); }
#define TRACE_LOG_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); }
#define TRACE_LOG_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); }
#define TRACE_LOG_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); }
#define TRACE_LOG_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); }
#define TRACE_LOGL_IF(_log_, _lv_, _check_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteLine(_msg_); }
#define TRACE_LOGL_IF_1(_log_, _lv_, _check_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_); }
#define TRACE_LOGL_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_); }
#define TRACE_LOGL_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_); }
#define TRACE_LOGL_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); }
#define TRACE_LOGL_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); }
#define TRACE_LOGL_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); }
#define TRACE_LOGL_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); }
#define TRACE_LOGL_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); }
#define TRACE_LOGL_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); }

#define TRACE_LOGDT_IF(_log_, _lv_, _check_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.Write(_msg_); } }
#define TRACE_LOGDT_IF_1(_log_, _lv_, _check_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_); } }
#define TRACE_LOGDT_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_); } }
#define TRACE_LOGDT_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_); } }
#define TRACE_LOGDT_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); } }
#define TRACE_LOGDT_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); } }
#define TRACE_LOGDT_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); } }
#define TRACE_LOGDT_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); } }
#define TRACE_LOGDT_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); } }
#define TRACE_LOGDT_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); } }
#define TRACE_LOGDTL_IF(_log_, _lv_, _check_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteLine(_msg_); } }
#define TRACE_LOGDTL_IF_1(_log_, _lv_, _check_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_); } }
#define TRACE_LOGDTL_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_); } }
#define TRACE_LOGDTL_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_); } }
#define TRACE_LOGDTL_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); } }
#define TRACE_LOGDTL_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); } }
#define TRACE_LOGDTL_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); } }
#define TRACE_LOGDTL_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); } }
#define TRACE_LOGDTL_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); } }
#define TRACE_LOGDTL_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); } }

#else // REUBEN_CONFIG_TRACE_ENABLE

#define TRACE_CALL_STACK(_logger_)				0
#define TRACE_CALL_STACK_BASE(_logger_, _ebp_)	0
#define TRACE_ISWRITEPOINTER(_pointer_)			0
#define TRACE_ISREADPOINTER(_pointer_)			0
#define TRACE_ISWRITEDATA(_pointer_, _size_)	0
#define TRACE_ISREADDATA(_pointer_, _size_)		0

#define TRACE_CHECK(_statement_)	0
#define TRACE_CHECK_0(_statement_, _message_)	0
#define TRACE_CHECK_1(_statement_, _message_, _arg1_)	0
#define TRACE_CHECK_2(_statement_, _message_, _arg1_, _arg2_)	0
#define TRACE_CHECK_3(_statement_, _message_, _arg1_, _arg2_, _arg3_)	0
#define TRACE_CHECK_4(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define TRACE_CHECK_5(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define TRACE_CHECK_6(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define TRACE_CHECK_7(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define TRACE_CHECK_8(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define TRACE_CHECK_9(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0

#define TRACE_ENSURE(_statement_)	0
#define TRACE_ENSURE_0(_statement_, _message_)	0
#define TRACE_ENSURE_1(_statement_, _message_, _arg1_)	0
#define TRACE_ENSURE_2(_statement_, _message_, _arg1_, _arg2_)	0
#define TRACE_ENSURE_3(_statement_, _message_, _arg1_, _arg2_, _arg3_)	0
#define TRACE_ENSURE_4(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define TRACE_ENSURE_5(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define TRACE_ENSURE_6(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define TRACE_ENSURE_7(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define TRACE_ENSURE_8(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define TRACE_ENSURE_9(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0

#define TRACE_LOG(_log_, _lv_, _msg_)	0
#define TRACE_LOG_1(_log_, _lv_, _msg_, _arg1_)	0
#define TRACE_LOG_2(_log_, _lv_, _msg_, _arg1_, _arg2_)	0
#define TRACE_LOG_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define TRACE_LOG_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define TRACE_LOG_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define TRACE_LOG_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define TRACE_LOG_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define TRACE_LOG_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define TRACE_LOG_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0
#define TRACE_LOGL(_log_, _lv_, _msg_)	0
#define TRACE_LOGL_1(_log_, _lv_, _msg_, _arg1_)	0
#define TRACE_LOGL_2(_log_, _lv_, _msg_, _arg1_, _arg2_)	0
#define TRACE_LOGL_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define TRACE_LOGL_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define TRACE_LOGL_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define TRACE_LOGL_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define TRACE_LOGL_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define TRACE_LOGL_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define TRACE_LOGL_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0

#define TRACE_LOGDT(_log_, _lv_, _msg_)	0
#define TRACE_LOGDT_1(_log_, _lv_, _msg_, _arg1_)	0
#define TRACE_LOGDT_2(_log_, _lv_, _msg_, _arg1_, _arg2_)	0
#define TRACE_LOGDT_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define TRACE_LOGDT_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define TRACE_LOGDT_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define TRACE_LOGDT_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define TRACE_LOGDT_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define TRACE_LOGDT_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define TRACE_LOGDT_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0
#define TRACE_LOGDTL(_log_, _lv_, _msg_)	0
#define TRACE_LOGDTL_1(_log_, _lv_, _msg_, _arg1_)	0
#define TRACE_LOGDTL_2(_log_, _lv_, _msg_, _arg1_, _arg2_)	0
#define TRACE_LOGDTL_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define TRACE_LOGDTL_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define TRACE_LOGDTL_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define TRACE_LOGDTL_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define TRACE_LOGDTL_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define TRACE_LOGDTL_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define TRACE_LOGDTL_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0

#define TRACE_LOG_IF(_log_, _lv_, _check_, _msg_)	0
#define TRACE_LOG_IF_1(_log_, _lv_, _check_, _msg_, _arg1_)	0
#define TRACE_LOG_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_)	0
#define TRACE_LOG_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define TRACE_LOG_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define TRACE_LOG_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define TRACE_LOG_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define TRACE_LOG_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define TRACE_LOG_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define TRACE_LOG_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0
#define TRACE_LOGL_IF(_log_, _lv_, _check_, _msg_)	0
#define TRACE_LOGL_IF_1(_log_, _lv_, _check_, _msg_, _arg1_)	0
#define TRACE_LOGL_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_)	0
#define TRACE_LOGL_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define TRACE_LOGL_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define TRACE_LOGL_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define TRACE_LOGL_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define TRACE_LOGL_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define TRACE_LOGL_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define TRACE_LOGL_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0

#define TRACE_LOGDT_IF(_log_, _lv_, _check_, _msg_)	0
#define TRACE_LOGDT_IF_1(_log_, _lv_, _check_, _msg_, _arg1_)	0
#define TRACE_LOGDT_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_)	0
#define TRACE_LOGDT_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define TRACE_LOGDT_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define TRACE_LOGDT_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define TRACE_LOGDT_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define TRACE_LOGDT_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define TRACE_LOGDT_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define TRACE_LOGDT_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0
#define TRACE_LOGDTL_IF(_log_, _lv_, _check_, _msg_)	0
#define TRACE_LOGDTL_IF_1(_log_, _lv_, _check_, _msg_, _arg1_)	0
#define TRACE_LOGDTL_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_)	0
#define TRACE_LOGDTL_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define TRACE_LOGDTL_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define TRACE_LOGDTL_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define TRACE_LOGDTL_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define TRACE_LOGDTL_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define TRACE_LOGDTL_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define TRACE_LOGDTL_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0

#endif // REUBEN_CONFIG_TRACE_ENABLE

//-- For Always

#define TRACE_ALWAYS(_log_, _msg_) \
	TRACE_LOG(_log_, LOG_LV_ALWAYS, _msg_)
#define TRACE_ALWAYS_1(_log_, _msg_, _arg1_) \
	TRACE_LOG_1(_log_, LOG_LV_ALWAYS, _msg_, _arg1_)
#define TRACE_ALWAYS_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOG_2(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_)
#define TRACE_ALWAYS_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOG_3(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ALWAYS_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOG_4(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ALWAYS_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOG_5(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ALWAYS_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOG_6(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ALWAYS_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOG_7(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ALWAYS_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOG_8(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ALWAYS_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOG_9(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_ALWAYSL(_log_, _msg_) \
	TRACE_LOGL(_log_, LOG_LV_ALWAYS, _msg_)
#define TRACE_ALWAYSL_1(_log_, _msg_, _arg1_) \
	TRACE_LOGL_1(_log_, LOG_LV_ALWAYS, _msg_, _arg1_)
#define TRACE_ALWAYSL_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGL_2(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_)
#define TRACE_ALWAYSL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGL_3(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ALWAYSL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGL_4(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ALWAYSL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGL_5(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ALWAYSL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGL_6(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ALWAYSL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGL_7(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ALWAYSL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGL_8(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ALWAYSL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGL_9(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_ALWAYSDT(_log_, _msg_) \
	TRACE_LOGDT(_log_, LOG_LV_ALWAYS, _msg_)
#define TRACE_ALWAYSDT_1(_log_, _msg_, _arg1_) \
	TRACE_LOGDT_1(_log_, LOG_LV_ALWAYS, _msg_, _arg1_)
#define TRACE_ALWAYSDT_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDT_2(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_)
#define TRACE_ALWAYSDT_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDT_3(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ALWAYSDT_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDT_4(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ALWAYSDT_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDT_5(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ALWAYSDT_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDT_6(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ALWAYSDT_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDT_7(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ALWAYSDT_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDT_8(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ALWAYSDT_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDT_9(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_ALWAYSDTL(_log_, _msg_) \
	TRACE_LOGDTL(_log_, LOG_LV_ALWAYS, _msg_)
#define TRACE_ALWAYSDTL_1(_log_, _msg_, _arg1_) \
	TRACE_LOGDTL_1(_log_, LOG_LV_ALWAYS, _msg_, _arg1_)
#define TRACE_ALWAYSDTL_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDTL_2(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_)
#define TRACE_ALWAYSDTL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDTL_3(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ALWAYSDTL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDTL_4(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ALWAYSDTL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDTL_5(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ALWAYSDTL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDTL_6(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ALWAYSDTL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDTL_7(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ALWAYSDTL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDTL_8(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ALWAYSDTL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDTL_9(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_ALWAYS_IF(_log_, _check_, _msg_) \
	TRACE_LOG_IF(_log_, LOG_LV_ALWAYS, _check_, _msg_)
#define TRACE_ALWAYS_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOG_IF_1(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_)
#define TRACE_ALWAYS_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOG_IF_2(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_ALWAYS_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOG_IF_3(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ALWAYS_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOG_IF_4(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ALWAYS_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOG_IF_5(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ALWAYS_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOG_IF_6(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ALWAYS_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOG_IF_7(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ALWAYS_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOG_IF_8(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ALWAYS_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOG_IF_9(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_ALWAYSL_IF(_log_, _check_, _msg_) \
	TRACE_LOGL_IF(_log_, LOG_LV_ALWAYS, _check_, _msg_)
#define TRACE_ALWAYSL_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGL_IF_1(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_)
#define TRACE_ALWAYSL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGL_IF_2(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_ALWAYSL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGL_IF_3(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ALWAYSL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGL_IF_4(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ALWAYSL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGL_IF_5(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ALWAYSL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGL_IF_6(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ALWAYSL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGL_IF_7(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ALWAYSL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGL_IF_8(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ALWAYSL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGL_IF_9(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_ALWAYSDT_IF(_log_, _check_, _msg_) \
	TRACE_LOGDT_IF(_log_, LOG_LV_ALWAYS, _check_, _msg_)
#define TRACE_ALWAYSDT_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGDT_IF_1(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_)
#define TRACE_ALWAYSDT_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDT_IF_2(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_ALWAYSDT_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDT_IF_3(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ALWAYSDT_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDT_IF_4(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ALWAYSDT_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDT_IF_5(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ALWAYSDT_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDT_IF_6(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ALWAYSDT_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDT_IF_7(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ALWAYSDT_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDT_IF_8(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ALWAYSDT_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDT_IF_9(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_ALWAYSDTL_IF(_log_, _check_, _msg_) \
	TRACE_LOGDTL_IF(_log_, LOG_LV_ALWAYS, _check_, _msg_)
#define TRACE_ALWAYSDTL_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGDTL_IF_1(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_)
#define TRACE_ALWAYSDTL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDTL_IF_2(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_ALWAYSDTL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDTL_IF_3(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ALWAYSDTL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDTL_IF_4(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ALWAYSDTL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDTL_IF_5(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ALWAYSDTL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDTL_IF_6(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ALWAYSDTL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDTL_IF_7(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ALWAYSDTL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDTL_IF_8(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ALWAYSDTL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDTL_IF_9(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

//-- For Error

#define TRACE_ERROR(_log_, _msg_) \
	TRACE_LOG(_log_, LOG_LV_ERROR, _msg_)
#define TRACE_ERROR_1(_log_, _msg_, _arg1_) \
	TRACE_LOG_1(_log_, LOG_LV_ERROR, _msg_, _arg1_)
#define TRACE_ERROR_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOG_2(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_)
#define TRACE_ERROR_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOG_3(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ERROR_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOG_4(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ERROR_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOG_5(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ERROR_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOG_6(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ERROR_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOG_7(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ERROR_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOG_8(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ERROR_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOG_9(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_ERRORL(_log_, _msg_) \
	TRACE_LOGL(_log_, LOG_LV_ERROR, _msg_)
#define TRACE_ERRORL_1(_log_, _msg_, _arg1_) \
	TRACE_LOGL_1(_log_, LOG_LV_ERROR, _msg_, _arg1_)
#define TRACE_ERRORL_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGL_2(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_)
#define TRACE_ERRORL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGL_3(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ERRORL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGL_4(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ERRORL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGL_5(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ERRORL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGL_6(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ERRORL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGL_7(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ERRORL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGL_8(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ERRORL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGL_9(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_ERRORDT(_log_, _msg_) \
	TRACE_LOGDT(_log_, LOG_LV_ERROR, _msg_)
#define TRACE_ERRORDT_1(_log_, _msg_, _arg1_) \
	TRACE_LOGDT_1(_log_, LOG_LV_ERROR, _msg_, _arg1_)
#define TRACE_ERRORDT_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDT_2(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_)
#define TRACE_ERRORDT_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDT_3(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ERRORDT_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDT_4(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ERRORDT_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDT_5(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ERRORDT_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDT_6(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ERRORDT_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDT_7(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ERRORDT_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDT_8(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ERRORDT_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDT_9(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_ERRORDTL(_log_, _msg_) \
	TRACE_LOGDTL(_log_, LOG_LV_ERROR, _msg_)
#define TRACE_ERRORDTL_1(_log_, _msg_, _arg1_) \
	TRACE_LOGDTL_1(_log_, LOG_LV_ERROR, _msg_, _arg1_)
#define TRACE_ERRORDTL_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDTL_2(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_)
#define TRACE_ERRORDTL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDTL_3(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ERRORDTL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDTL_4(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ERRORDTL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDTL_5(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ERRORDTL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDTL_6(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ERRORDTL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDTL_7(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ERRORDTL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDTL_8(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ERRORDTL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDTL_9(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_ERROR_IF(_log_, _check_, _msg_) \
	TRACE_LOG_IF(_log_, LOG_LV_ERROR, _check_, _msg_)
#define TRACE_ERROR_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOG_IF_1(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_)
#define TRACE_ERROR_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOG_IF_2(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_ERROR_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOG_IF_3(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ERROR_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOG_IF_4(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ERROR_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOG_IF_5(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ERROR_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOG_IF_6(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ERROR_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOG_IF_7(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ERROR_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOG_IF_8(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ERROR_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOG_IF_9(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_ERRORL_IF(_log_, _check_, _msg_) \
	TRACE_LOGL_IF(_log_, LOG_LV_ERROR, _check_, _msg_)
#define TRACE_ERRORL_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGL_IF_1(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_)
#define TRACE_ERRORL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGL_IF_2(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_ERRORL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGL_IF_3(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ERRORL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGL_IF_4(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ERRORL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGL_IF_5(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ERRORL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGL_IF_6(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ERRORL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGL_IF_7(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ERRORL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGL_IF_8(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ERRORL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGL_IF_9(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_ERRORDT_IF(_log_, _check_, _msg_) \
	TRACE_LOGDT_IF(_log_, LOG_LV_ERROR, _check_, _msg_)
#define TRACE_ERRORDT_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGDT_IF_1(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_)
#define TRACE_ERRORDT_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDT_IF_2(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_ERRORDT_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDT_IF_3(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ERRORDT_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDT_IF_4(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ERRORDT_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDT_IF_5(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ERRORDT_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDT_IF_6(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ERRORDT_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDT_IF_7(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ERRORDT_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDT_IF_8(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ERRORDT_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDT_IF_9(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_ERRORDTL_IF(_log_, _check_, _msg_) \
	TRACE_LOGDTL_IF(_log_, LOG_LV_ERROR, _check_, _msg_)
#define TRACE_ERRORDTL_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGDTL_IF_1(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_)
#define TRACE_ERRORDTL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDTL_IF_2(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_ERRORDTL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDTL_IF_3(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_ERRORDTL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDTL_IF_4(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_ERRORDTL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDTL_IF_5(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_ERRORDTL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDTL_IF_6(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_ERRORDTL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDTL_IF_7(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_ERRORDTL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDTL_IF_8(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_ERRORDTL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDTL_IF_9(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

//-- For Warning

#define TRACE_WARN(_log_, _msg_) \
	TRACE_LOG(_log_, LOG_LV_WARN, _msg_)
#define TRACE_WARN_1(_log_, _msg_, _arg1_) \
	TRACE_LOG_1(_log_, LOG_LV_WARN, _msg_, _arg1_)
#define TRACE_WARN_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOG_2(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_)
#define TRACE_WARN_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOG_3(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_WARN_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOG_4(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_WARN_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOG_5(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_WARN_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOG_6(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_WARN_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOG_7(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_WARN_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOG_8(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_WARN_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOG_9(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_WARNL(_log_, _msg_) \
	TRACE_LOGL(_log_, LOG_LV_WARN, _msg_)
#define TRACE_WARNL_1(_log_, _msg_, _arg1_) \
	TRACE_LOGL_1(_log_, LOG_LV_WARN, _msg_, _arg1_)
#define TRACE_WARNL_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGL_2(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_)
#define TRACE_WARNL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGL_3(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_WARNL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGL_4(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_WARNL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGL_5(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_WARNL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGL_6(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_WARNL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGL_7(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_WARNL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGL_8(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_WARNL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGL_9(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_WARNDT(_log_, _msg_) \
	TRACE_LOGDT(_log_, LOG_LV_WARN, _msg_)
#define TRACE_WARNDT_1(_log_, _msg_, _arg1_) \
	TRACE_LOGDT_1(_log_, LOG_LV_WARN, _msg_, _arg1_)
#define TRACE_WARNDT_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDT_2(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_)
#define TRACE_WARNDT_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDT_3(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_WARNDT_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDT_4(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_WARNDT_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDT_5(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_WARNDT_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDT_6(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_WARNDT_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDT_7(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_WARNDT_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDT_8(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_WARNDT_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDT_9(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_WARNDTL(_log_, _msg_) \
	TRACE_LOGDTL(_log_, LOG_LV_WARN, _msg_)
#define TRACE_WARNDTL_1(_log_, _msg_, _arg1_) \
	TRACE_LOGDTL_1(_log_, LOG_LV_WARN, _msg_, _arg1_)
#define TRACE_WARNDTL_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDTL_2(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_)
#define TRACE_WARNDTL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDTL_3(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_WARNDTL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDTL_4(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_WARNDTL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDTL_5(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_WARNDTL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDTL_6(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_WARNDTL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDTL_7(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_WARNDTL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDTL_8(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_WARNDTL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDTL_9(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_WARN_IF(_log_, _check_, _msg_) \
	TRACE_LOG_IF(_log_, LOG_LV_WARN, _check_, _msg_)
#define TRACE_WARN_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOG_IF_1(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_)
#define TRACE_WARN_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOG_IF_2(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_WARN_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOG_IF_3(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_WARN_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOG_IF_4(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_WARN_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOG_IF_5(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_WARN_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOG_IF_6(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_WARN_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOG_IF_7(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_WARN_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOG_IF_8(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_WARN_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOG_IF_9(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_WARNL_IF(_log_, _check_, _msg_) \
	TRACE_LOGL_IF(_log_, LOG_LV_WARN, _check_, _msg_)
#define TRACE_WARNL_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGL_IF_1(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_)
#define TRACE_WARNL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGL_IF_2(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_WARNL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGL_IF_3(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_WARNL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGL_IF_4(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_WARNL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGL_IF_5(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_WARNL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGL_IF_6(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_WARNL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGL_IF_7(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_WARNL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGL_IF_8(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_WARNL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGL_IF_9(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_WARNDT_IF(_log_, _check_, _msg_) \
	TRACE_LOGDT_IF(_log_, LOG_LV_WARN, _check_, _msg_)
#define TRACE_WARNDT_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGDT_IF_1(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_)
#define TRACE_WARNDT_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDT_IF_2(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_WARNDT_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDT_IF_3(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_WARNDT_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDT_IF_4(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_WARNDT_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDT_IF_5(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_WARNDT_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDT_IF_6(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_WARNDT_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDT_IF_7(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_WARNDT_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDT_IF_8(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_WARNDT_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDT_IF_9(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_WARNDTL_IF(_log_, _check_, _msg_) \
	TRACE_LOGDTL_IF(_log_, LOG_LV_WARN, _check_, _msg_)
#define TRACE_WARNDTL_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGDTL_IF_1(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_)
#define TRACE_WARNDTL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDTL_IF_2(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_WARNDTL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDTL_IF_3(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_WARNDTL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDTL_IF_4(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_WARNDTL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDTL_IF_5(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_WARNDTL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDTL_IF_6(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_WARNDTL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDTL_IF_7(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_WARNDTL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDTL_IF_8(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_WARNDTL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDTL_IF_9(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

//-- For Information

#define TRACE_INFO(_log_, _msg_) \
	TRACE_LOG(_log_, LOG_LV_INFO, _msg_)
#define TRACE_INFO_1(_log_, _msg_, _arg1_) \
	TRACE_LOG_1(_log_, LOG_LV_INFO, _msg_, _arg1_)
#define TRACE_INFO_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOG_2(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_)
#define TRACE_INFO_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOG_3(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_INFO_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOG_4(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_INFO_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOG_5(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_INFO_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOG_6(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_INFO_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOG_7(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_INFO_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOG_8(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_INFO_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOG_9(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_INFOL(_log_, _msg_) \
	TRACE_LOGL(_log_, LOG_LV_INFO, _msg_)
#define TRACE_INFOL_1(_log_, _msg_, _arg1_) \
	TRACE_LOGL_1(_log_, LOG_LV_INFO, _msg_, _arg1_)
#define TRACE_INFOL_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGL_2(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_)
#define TRACE_INFOL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGL_3(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_INFOL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGL_4(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_INFOL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGL_5(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_INFOL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGL_6(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_INFOL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGL_7(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_INFOL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGL_8(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_INFOL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGL_9(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_INFODT(_log_, _msg_) \
	TRACE_LOGDT(_log_, LOG_LV_INFO, _msg_)
#define TRACE_INFODT_1(_log_, _msg_, _arg1_) \
	TRACE_LOGDT_1(_log_, LOG_LV_INFO, _msg_, _arg1_)
#define TRACE_INFODT_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDT_2(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_)
#define TRACE_INFODT_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDT_3(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_INFODT_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDT_4(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_INFODT_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDT_5(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_INFODT_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDT_6(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_INFODT_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDT_7(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_INFODT_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDT_8(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_INFODT_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDT_9(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_INFODTL(_log_, _msg_) \
	TRACE_LOGDTL(_log_, LOG_LV_INFO, _msg_)
#define TRACE_INFODTL_1(_log_, _msg_, _arg1_) \
	TRACE_LOGDTL_1(_log_, LOG_LV_INFO, _msg_, _arg1_)
#define TRACE_INFODTL_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDTL_2(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_)
#define TRACE_INFODTL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDTL_3(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_INFODTL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDTL_4(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_INFODTL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDTL_5(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_INFODTL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDTL_6(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_INFODTL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDTL_7(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_INFODTL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDTL_8(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_INFODTL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDTL_9(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_INFO_IF(_log_, _check_, _msg_) \
	TRACE_LOG_IF(_log_, LOG_LV_INFO, _check_, _msg_)
#define TRACE_INFO_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOG_IF_1(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_)
#define TRACE_INFO_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOG_IF_2(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_INFO_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOG_IF_3(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_INFO_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOG_IF_4(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_INFO_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOG_IF_5(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_INFO_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOG_IF_6(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_INFO_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOG_IF_7(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_INFO_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOG_IF_8(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_INFO_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOG_IF_9(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_INFOL_IF(_log_, _check_, _msg_) \
	TRACE_LOGL_IF(_log_, LOG_LV_INFO, _check_, _msg_)
#define TRACE_INFOL_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGL_IF_1(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_)
#define TRACE_INFOL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGL_IF_2(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_INFOL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGL_IF_3(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_INFOL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGL_IF_4(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_INFOL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGL_IF_5(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_INFOL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGL_IF_6(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_INFOL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGL_IF_7(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_INFOL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGL_IF_8(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_INFOL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGL_IF_9(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_INFODT_IF(_log_, _check_, _msg_) \
	TRACE_LOGDT_IF(_log_, LOG_LV_INFO, _check_, _msg_)
#define TRACE_INFODT_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGDT_IF_1(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_)
#define TRACE_INFODT_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDT_IF_2(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_INFODT_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDT_IF_3(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_INFODT_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDT_IF_4(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_INFODT_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDT_IF_5(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_INFODT_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDT_IF_6(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_INFODT_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDT_IF_7(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_INFODT_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDT_IF_8(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_INFODT_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDT_IF_9(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_INFODTL_IF(_log_, _check_, _msg_) \
	TRACE_LOGDTL_IF(_log_, LOG_LV_INFO, _check_, _msg_)
#define TRACE_INFODTL_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGDTL_IF_1(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_)
#define TRACE_INFODTL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDTL_IF_2(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_INFODTL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDTL_IF_3(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_INFODTL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDTL_IF_4(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_INFODTL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDTL_IF_5(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_INFODTL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDTL_IF_6(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_INFODTL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDTL_IF_7(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_INFODTL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDTL_IF_8(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_INFODTL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDTL_IF_9(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

//-- For Verbose

#define TRACE_VERBOSE(_log_, _msg_) \
	TRACE_LOG(_log_, LOG_LV_VERBOSE, _msg_)
#define TRACE_VERBOSE_1(_log_, _msg_, _arg1_) \
	TRACE_LOG_1(_log_, LOG_LV_VERBOSE, _msg_, _arg1_)
#define TRACE_VERBOSE_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOG_2(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_)
#define TRACE_VERBOSE_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOG_3(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_VERBOSE_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOG_4(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_VERBOSE_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOG_5(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_VERBOSE_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOG_6(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_VERBOSE_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOG_7(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_VERBOSE_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOG_8(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_VERBOSE_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOG_9(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_VERBOSEL(_log_, _msg_) \
	TRACE_LOGL(_log_, LOG_LV_VERBOSE, _msg_)
#define TRACE_VERBOSEL_1(_log_, _msg_, _arg1_) \
	TRACE_LOGL_1(_log_, LOG_LV_VERBOSE, _msg_, _arg1_)
#define TRACE_VERBOSEL_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGL_2(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_)
#define TRACE_VERBOSEL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGL_3(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_VERBOSEL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGL_4(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_VERBOSEL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGL_5(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_VERBOSEL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGL_6(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_VERBOSEL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGL_7(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_VERBOSEL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGL_8(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_VERBOSEL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGL_9(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_VERBOSEDT(_log_, _msg_) \
	TRACE_LOGDT(_log_, LOG_LV_VERBOSE, _msg_)
#define TRACE_VERBOSEDT_1(_log_, _msg_, _arg1_) \
	TRACE_LOGDT_1(_log_, LOG_LV_VERBOSE, _msg_, _arg1_)
#define TRACE_VERBOSEDT_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDT_2(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_)
#define TRACE_VERBOSEDT_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDT_3(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_VERBOSEDT_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDT_4(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_VERBOSEDT_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDT_5(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_VERBOSEDT_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDT_6(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_VERBOSEDT_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDT_7(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_VERBOSEDT_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDT_8(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_VERBOSEDT_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDT_9(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_VERBOSEDTL(_log_, _msg_) \
	TRACE_LOGDTL(_log_, LOG_LV_VERBOSE, _msg_)
#define TRACE_VERBOSEDTL_1(_log_, _msg_, _arg1_) \
	TRACE_LOGDTL_1(_log_, LOG_LV_VERBOSE, _msg_, _arg1_)
#define TRACE_VERBOSEDTL_2(_log_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDTL_2(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_)
#define TRACE_VERBOSEDTL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDTL_3(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_VERBOSEDTL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDTL_4(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_VERBOSEDTL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDTL_5(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_VERBOSEDTL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDTL_6(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_VERBOSEDTL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDTL_7(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_VERBOSEDTL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDTL_8(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_VERBOSEDTL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDTL_9(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_VERBOSE_IF(_log_, _check_, _msg_) \
	TRACE_LOG_IF(_log_, LOG_LV_VERBOSE, _check_, _msg_)
#define TRACE_VERBOSE_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOG_IF_1(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_)
#define TRACE_VERBOSE_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOG_IF_2(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_VERBOSE_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOG_IF_3(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_VERBOSE_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOG_IF_4(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_VERBOSE_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOG_IF_5(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_VERBOSE_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOG_IF_6(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_VERBOSE_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOG_IF_7(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_VERBOSE_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOG_IF_8(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_VERBOSE_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOG_IF_9(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_VERBOSEL_IF(_log_, _check_, _msg_) \
	TRACE_LOGL_IF(_log_, LOG_LV_VERBOSE, _check_, _msg_)
#define TRACE_VERBOSEL_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGL_IF_1(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_)
#define TRACE_VERBOSEL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGL_IF_2(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_VERBOSEL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGL_IF_3(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_VERBOSEL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGL_IF_4(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_VERBOSEL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGL_IF_5(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_VERBOSEL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGL_IF_6(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_VERBOSEL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGL_IF_7(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_VERBOSEL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGL_IF_8(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_VERBOSEL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGL_IF_9(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define TRACE_VERBOSEDT_IF(_log_, _check_, _msg_) \
	TRACE_LOGDT_IF(_log_, LOG_LV_VERBOSE, _check_, _msg_)
#define TRACE_VERBOSEDT_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGDT_IF_1(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_)
#define TRACE_VERBOSEDT_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDT_IF_2(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_VERBOSEDT_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDT_IF_3(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_VERBOSEDT_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDT_IF_4(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_VERBOSEDT_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDT_IF_5(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_VERBOSEDT_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDT_IF_6(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_VERBOSEDT_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDT_IF_7(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_VERBOSEDT_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDT_IF_8(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_VERBOSEDT_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDT_IF_9(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define TRACE_VERBOSEDTL_IF(_log_, _check_, _msg_) \
	TRACE_LOGDTL_IF(_log_, LOG_LV_VERBOSE, _check_, _msg_)
#define TRACE_VERBOSEDTL_IF_1(_log_, _check_, _msg_, _arg1_) \
	TRACE_LOGDTL_IF_1(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_)
#define TRACE_VERBOSEDTL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	TRACE_LOGDTL_IF_2(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_)
#define TRACE_VERBOSEDTL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	TRACE_LOGDTL_IF_3(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define TRACE_VERBOSEDTL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	TRACE_LOGDTL_IF_4(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define TRACE_VERBOSEDTL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	TRACE_LOGDTL_IF_5(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define TRACE_VERBOSEDTL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	TRACE_LOGDTL_IF_6(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define TRACE_VERBOSEDTL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	TRACE_LOGDTL_IF_7(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define TRACE_VERBOSEDTL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	TRACE_LOGDTL_IF_8(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define TRACE_VERBOSEDTL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	TRACE_LOGDTL_IF_9(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#endif // _REUBEN_DIAGNOSTICS_TRACE_H_
