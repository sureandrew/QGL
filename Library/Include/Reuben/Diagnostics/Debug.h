#pragma once
#ifndef _REUBEN_DIAGNOSTICS_DEBUG_H_
#define _REUBEN_DIAGNOSTICS_DEBUG_H_

//-- Local
#include "Trace.h"

namespace Reuben { namespace Diagnostics {

	class DebugException : public TraceException
	{
	public:

		DebugException(ErrorCode ec, PCSTRING pSourceLine, PCSTRING pStatement)
			: TraceException(ec, pSourceLine, pStatement)
		{
		}

		DebugException(ErrorCode ec, PCSTRING pSourceLine, PCSTRING pStatement, PCSTRING pMessage)
			: TraceException(ec, pSourceLine, pStatement, pMessage)
		{
		}

	};

}; }; // Reuben::Diagnostics

#if defined(COMPILER_DEBUG)

#define DEBUG_BASIC(_error_code_, _statement_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		throw Reuben::Diagnostics::DebugException(_error_code_, _SOURCE_LINE_, _T(#_statement_)); \
	} \
}

#define DEBUG_BASIC_0(_error_code_, _statement_, _message_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		throw Reuben::Diagnostics::DebugException(_error_code_, _SOURCE_LINE_, _T(#_statement_), _message_); \
	} \
}

#define DEBUG_BASIC_1(_error_code_, _statement_, _message_, _arg1_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_); \
		throw Reuben::Diagnostics::DebugException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define DEBUG_BASIC_2(_error_code_, _statement_, _message_, _arg1_, _arg2_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_); \
		throw Reuben::Diagnostics::DebugException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define DEBUG_BASIC_3(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_); \
		throw Reuben::Diagnostics::DebugException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define DEBUG_BASIC_4(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_, _arg4_); \
		throw Reuben::Diagnostics::DebugException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define DEBUG_BASIC_5(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); \
		throw Reuben::Diagnostics::DebugException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define DEBUG_BASIC_6(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); \
		throw Reuben::Diagnostics::DebugException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define DEBUG_BASIC_7(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); \
		throw Reuben::Diagnostics::DebugException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define DEBUG_BASIC_8(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); \
		throw Reuben::Diagnostics::DebugException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define DEBUG_BASIC_9(_error_code_, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
{ \
	if (!(_statement_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); \
		throw Reuben::Diagnostics::DebugException(_error_code_, _SOURCE_LINE_, _T(#_statement_), strMessage.c_str()); \
	} \
}

#define DEBUG_ISWRITEPOINTER(_pointer_) \
{ \
	if (IsBadWritePtr((LPVOID)_pointer_, sizeof(LPVOID))) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_T("Pointer = 0x%08X is not a valid write pointer"), _pointer_); \
		throw Reuben::Diagnostics::DebugException(EC_WRITE_INVALID, _SOURCE_LINE_, _T(#_pointer_), strMessage.c_str()); \
	} \
}

#define DEBUG_ISREADPOINTER(_pointer_) \
{ \
	if (IsBadReadPtr((LPVOID)_pointer_, sizeof(LPVOID))) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_T("Pointer = 0x%08X is not a valid read pointer"), _pointer_); \
		throw Reuben::Diagnostics::DebugException(EC_READ_INVALID, _SOURCE_LINE_, _T(#_pointer_), strMessage.c_str()); \
	} \
}

#define DEBUG_ISWRITEDATA(_pointer_, _size_) \
{ \
	if (IsBadWritePtr((LPVOID)_pointer_, _size_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_T("Pointer = 0x%08X, Size = %d is not a valid write area"), _pointer_, _size_); \
		throw Reuben::Diagnostics::DebugException(EC_WRITE_INVALID, _SOURCE_LINE_, _T(#_pointer_) _T(" ") _T(#_size_), strMessage.c_str()); \
	} \
}

#define DEBUG_ISREADDATA(_pointer_, _size_) \
{ \
	if (IsBadReadPtr((LPVOID)_pointer_, _size_)) \
	{ \
		TRACE_BREAK \
		String strMessage; \
		strMessage.Format(_T("Pointer = 0x%08X, Size = %d is not a valid read area"), _pointer_, _size_); \
		throw Reuben::Diagnostics::DebugException(EC_READ_INVALID, _SOURCE_LINE_, _T(#_pointer_) _T(" ") _T(#_size_), strMessage.c_str()); \
	} \
}

#define DEBUG_CHECK(_statement_) \
	DEBUG_BASIC(EC_CHECK_INVALID, _statement_)
#define DEBUG_CHECK_0(_statement_, _message_) \
	DEBUG_BASIC_0(EC_CHECK_INVALID, _statement_, _message_)
#define DEBUG_CHECK_1(_statement_, _message_, _arg1_) \
	DEBUG_BASIC_1(EC_CHECK_INVALID, _statement_, _message_, _arg1_)
#define DEBUG_CHECK_2(_statement_, _message_, _arg1_, _arg2_) \
	DEBUG_BASIC_2(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_)
#define DEBUG_CHECK_3(_statement_, _message_, _arg1_, _arg2_, _arg3_) \
	DEBUG_BASIC_3(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_)
#define DEBUG_CHECK_4(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_BASIC_4(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_CHECK_5(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_BASIC_5(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_CHECK_6(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_BASIC_6(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_CHECK_7(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_BASIC_7(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_CHECK_8(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_BASIC_8(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_CHECK_9(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_BASIC_9(EC_CHECK_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_ENSURE(_statement_) \
	DEBUG_BASIC(EC_ENSURE_INVALID, _statement_)
#define DEBUG_ENSURE_0(_statement_, _message_) \
	DEBUG_BASIC_0(EC_ENSURE_INVALID, _statement_, _message_)
#define DEBUG_ENSURE_1(_statement_, _message_, _arg1_) \
	DEBUG_BASIC_1(EC_ENSURE_INVALID, _statement_, _message_, _arg1_)
#define DEBUG_ENSURE_2(_statement_, _message_, _arg1_, _arg2_) \
	DEBUG_BASIC_2(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_)
#define DEBUG_ENSURE_3(_statement_, _message_, _arg1_, _arg2_, _arg3_) \
	DEBUG_BASIC_3(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ENSURE_4(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_BASIC_4(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ENSURE_5(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_BASIC_5(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ENSURE_6(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_BASIC_6(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ENSURE_7(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_BASIC_7(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ENSURE_8(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_BASIC_8(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ENSURE_9(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_BASIC_9(EC_ENSURE_INVALID, _statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_LOG(_log_, _lv_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.Write(_msg_); }
#define DEBUG_LOG_1(_log_, _lv_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_); }
#define DEBUG_LOG_2(_log_, _lv_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_); }
#define DEBUG_LOG_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_); }
#define DEBUG_LOG_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); }
#define DEBUG_LOG_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); }
#define DEBUG_LOG_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); }
#define DEBUG_LOG_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); }
#define DEBUG_LOG_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); }
#define DEBUG_LOG_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); }
#define DEBUG_LOGL(_log_, _lv_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteLine(_msg_); }
#define DEBUG_LOGL_1(_log_, _lv_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_); }
#define DEBUG_LOGL_2(_log_, _lv_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_); }
#define DEBUG_LOGL_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_); }
#define DEBUG_LOGL_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); }
#define DEBUG_LOGL_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); }
#define DEBUG_LOGL_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); }
#define DEBUG_LOGL_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); }
#define DEBUG_LOGL_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); }
#define DEBUG_LOGL_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel()) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); }

#define DEBUG_LOGDT(_log_, _lv_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.Write(_msg_); } }
#define DEBUG_LOGDT_1(_log_, _lv_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_); } }
#define DEBUG_LOGDT_2(_log_, _lv_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_); } }
#define DEBUG_LOGDT_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_); } }
#define DEBUG_LOGDT_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); } }
#define DEBUG_LOGDT_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); } }
#define DEBUG_LOGDT_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); } }
#define DEBUG_LOGDT_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); } }
#define DEBUG_LOGDT_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); } }
#define DEBUG_LOGDT_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); } }
#define DEBUG_LOGDTL(_log_, _lv_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteLine(_msg_); } }
#define DEBUG_LOGDTL_1(_log_, _lv_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_); } }
#define DEBUG_LOGDTL_2(_log_, _lv_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_); } }
#define DEBUG_LOGDTL_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_); } }
#define DEBUG_LOGDTL_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); } }
#define DEBUG_LOGDTL_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); } }
#define DEBUG_LOGDTL_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); } }
#define DEBUG_LOGDTL_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); } }
#define DEBUG_LOGDTL_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); } }
#define DEBUG_LOGDTL_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel()) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); } }

#define DEBUG_LOG_IF(_log_, _lv_, _check_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.Write(_msg_); }
#define DEBUG_LOG_IF_1(_log_, _lv_, _check_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_); }
#define DEBUG_LOG_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_); }
#define DEBUG_LOG_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_); }
#define DEBUG_LOG_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); }
#define DEBUG_LOG_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); }
#define DEBUG_LOG_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); }
#define DEBUG_LOG_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); }
#define DEBUG_LOG_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); }
#define DEBUG_LOG_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); }
#define DEBUG_LOGL_IF(_log_, _lv_, _check_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteLine(_msg_); }
#define DEBUG_LOGL_IF_1(_log_, _lv_, _check_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_); }
#define DEBUG_LOGL_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_); }
#define DEBUG_LOGL_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_); }
#define DEBUG_LOGL_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); }
#define DEBUG_LOGL_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); }
#define DEBUG_LOGL_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); }
#define DEBUG_LOGL_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); }
#define DEBUG_LOGL_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); }
#define DEBUG_LOGL_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); }

#define DEBUG_LOGDT_IF(_log_, _lv_, _check_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.Write(_msg_); } }
#define DEBUG_LOGDT_IF_1(_log_, _lv_, _check_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_); } }
#define DEBUG_LOGDT_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_); } }
#define DEBUG_LOGDT_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_); } }
#define DEBUG_LOGDT_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); } }
#define DEBUG_LOGDT_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); } }
#define DEBUG_LOGDT_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); } }
#define DEBUG_LOGDT_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); } }
#define DEBUG_LOGDT_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); } }
#define DEBUG_LOGDT_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormat(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); } }
#define DEBUG_LOGDTL_IF(_log_, _lv_, _check_, _msg_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteLine(_msg_); } }
#define DEBUG_LOGDTL_IF_1(_log_, _lv_, _check_, _msg_, _arg1_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_); } }
#define DEBUG_LOGDTL_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_); } }
#define DEBUG_LOGDTL_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_); } }
#define DEBUG_LOGDTL_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_); } }
#define DEBUG_LOGDTL_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_); } }
#define DEBUG_LOGDTL_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_); } }
#define DEBUG_LOGDTL_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_); } }
#define DEBUG_LOGDTL_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_); } }
#define DEBUG_LOGDTL_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	{ if (_lv_ <= _log_.GetLevel() && (_check_)) { if (_log_.HasDateTime()) _log_.WriteCurrentDateTime(); _log_.WriteFormatLine(_msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_); } }

#else // COMPILER_DEBUG

#define DEBUG_ISWRITEPOINTER(_pointer_)			0
#define DEBUG_ISREADPOINTER(_pointer_)			0
#define DEBUG_ISWRITEDATA(_pointer_, _size_)	0
#define DEBUG_ISREADDATA(_pointer_, _size_)		0

#define DEBUG_CHECK(_statement_)	0
#define DEBUG_CHECK_0(_statement_, _message_)	0
#define DEBUG_CHECK_1(_statement_, _message_, _arg1_)	0
#define DEBUG_CHECK_2(_statement_, _message_, _arg1_, _arg2_)	0
#define DEBUG_CHECK_3(_statement_, _message_, _arg1_, _arg2_, _arg3_)	0
#define DEBUG_CHECK_4(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define DEBUG_CHECK_5(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define DEBUG_CHECK_6(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define DEBUG_CHECK_7(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define DEBUG_CHECK_8(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define DEBUG_CHECK_9(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0

#define DEBUG_ENSURE(_statement_)	0
#define DEBUG_ENSURE_0(_statement_, _message_)	0
#define DEBUG_ENSURE_1(_statement_, _message_, _arg1_)	0
#define DEBUG_ENSURE_2(_statement_, _message_, _arg1_, _arg2_)	0
#define DEBUG_ENSURE_3(_statement_, _message_, _arg1_, _arg2_, _arg3_)	0
#define DEBUG_ENSURE_4(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define DEBUG_ENSURE_5(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define DEBUG_ENSURE_6(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define DEBUG_ENSURE_7(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define DEBUG_ENSURE_8(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define DEBUG_ENSURE_9(_statement_, _message_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0

#define DEBUG_LOG(_log_, _lv_, _msg_)	0
#define DEBUG_LOG_1(_log_, _lv_, _msg_, _arg1_)	0
#define DEBUG_LOG_2(_log_, _lv_, _msg_, _arg1_, _arg2_)	0
#define DEBUG_LOG_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define DEBUG_LOG_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define DEBUG_LOG_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define DEBUG_LOG_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define DEBUG_LOG_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define DEBUG_LOG_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define DEBUG_LOG_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0
#define DEBUG_LOGL(_log_, _lv_, _msg_)	0
#define DEBUG_LOGL_1(_log_, _lv_, _msg_, _arg1_)	0
#define DEBUG_LOGL_2(_log_, _lv_, _msg_, _arg1_, _arg2_)	0
#define DEBUG_LOGL_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define DEBUG_LOGL_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define DEBUG_LOGL_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define DEBUG_LOGL_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define DEBUG_LOGL_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define DEBUG_LOGL_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define DEBUG_LOGL_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0

#define DEBUG_LOGDT(_log_, _lv_, _msg_)	0
#define DEBUG_LOGDT_1(_log_, _lv_, _msg_, _arg1_)	0
#define DEBUG_LOGDT_2(_log_, _lv_, _msg_, _arg1_, _arg2_)	0
#define DEBUG_LOGDT_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define DEBUG_LOGDT_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define DEBUG_LOGDT_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define DEBUG_LOGDT_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define DEBUG_LOGDT_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define DEBUG_LOGDT_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define DEBUG_LOGDT_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0
#define DEBUG_LOGDTL(_log_, _lv_, _msg_)	0
#define DEBUG_LOGDTL_1(_log_, _lv_, _msg_, _arg1_)	0
#define DEBUG_LOGDTL_2(_log_, _lv_, _msg_, _arg1_, _arg2_)	0
#define DEBUG_LOGDTL_3(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define DEBUG_LOGDTL_4(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define DEBUG_LOGDTL_5(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define DEBUG_LOGDTL_6(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define DEBUG_LOGDTL_7(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define DEBUG_LOGDTL_8(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define DEBUG_LOGDTL_9(_log_, _lv_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0

#define DEBUG_LOG_IF(_log_, _lv_, _check_, _msg_)	0
#define DEBUG_LOG_IF_1(_log_, _lv_, _check_, _msg_, _arg1_)	0
#define DEBUG_LOG_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_)	0
#define DEBUG_LOG_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define DEBUG_LOG_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define DEBUG_LOG_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define DEBUG_LOG_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define DEBUG_LOG_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define DEBUG_LOG_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define DEBUG_LOG_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0
#define DEBUG_LOGL_IF(_log_, _lv_, _check_, _msg_)	0
#define DEBUG_LOGL_IF_1(_log_, _lv_, _check_, _msg_, _arg1_)	0
#define DEBUG_LOGL_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_)	0
#define DEBUG_LOGL_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define DEBUG_LOGL_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define DEBUG_LOGL_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define DEBUG_LOGL_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define DEBUG_LOGL_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define DEBUG_LOGL_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define DEBUG_LOGL_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0

#define DEBUG_LOGDT_IF(_log_, _lv_, _check_, _msg_)	0
#define DEBUG_LOGDT_IF_1(_log_, _lv_, _check_, _msg_, _arg1_)	0
#define DEBUG_LOGDT_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_)	0
#define DEBUG_LOGDT_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define DEBUG_LOGDT_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define DEBUG_LOGDT_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define DEBUG_LOGDT_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define DEBUG_LOGDT_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define DEBUG_LOGDT_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define DEBUG_LOGDT_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0
#define DEBUG_LOGDTL_IF(_log_, _lv_, _check_, _msg_)	0
#define DEBUG_LOGDTL_IF_1(_log_, _lv_, _check_, _msg_, _arg1_)	0
#define DEBUG_LOGDTL_IF_2(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_)	0
#define DEBUG_LOGDTL_IF_3(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_)	0
#define DEBUG_LOGDTL_IF_4(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)	0
#define DEBUG_LOGDTL_IF_5(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)	0
#define DEBUG_LOGDTL_IF_6(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)	0
#define DEBUG_LOGDTL_IF_7(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)	0
#define DEBUG_LOGDTL_IF_8(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)	0
#define DEBUG_LOGDTL_IF_9(_log_, _lv_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)	0

#endif // COMPILER_DEBUG

//-- For Always

#define DEBUG_ALWAYS(_log_, _msg_) \
	DEBUG_LOG(_log_, LOG_LV_ALWAYS, _msg_)
#define DEBUG_ALWAYS_1(_log_, _msg_, _arg1_) \
	DEBUG_LOG_1(_log_, LOG_LV_ALWAYS, _msg_, _arg1_)
#define DEBUG_ALWAYS_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOG_2(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_)
#define DEBUG_ALWAYS_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOG_3(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ALWAYS_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOG_4(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ALWAYS_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOG_5(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ALWAYS_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOG_6(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ALWAYS_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOG_7(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ALWAYS_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOG_8(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ALWAYS_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOG_9(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_ALWAYSL(_log_, _msg_) \
	DEBUG_LOGL(_log_, LOG_LV_ALWAYS, _msg_)
#define DEBUG_ALWAYSL_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGL_1(_log_, LOG_LV_ALWAYS, _msg_, _arg1_)
#define DEBUG_ALWAYSL_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGL_2(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_)
#define DEBUG_ALWAYSL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGL_3(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ALWAYSL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGL_4(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ALWAYSL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGL_5(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ALWAYSL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGL_6(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ALWAYSL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGL_7(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ALWAYSL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGL_8(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ALWAYSL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGL_9(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_ALWAYSDT(_log_, _msg_) \
	DEBUG_LOGDT(_log_, LOG_LV_ALWAYS, _msg_)
#define DEBUG_ALWAYSDT_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGDT_1(_log_, LOG_LV_ALWAYS, _msg_, _arg1_)
#define DEBUG_ALWAYSDT_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDT_2(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_)
#define DEBUG_ALWAYSDT_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDT_3(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ALWAYSDT_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDT_4(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ALWAYSDT_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDT_5(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ALWAYSDT_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDT_6(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ALWAYSDT_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDT_7(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ALWAYSDT_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDT_8(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ALWAYSDT_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDT_9(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_ALWAYSDTL(_log_, _msg_) \
	DEBUG_LOGDTL(_log_, LOG_LV_ALWAYS, _msg_)
#define DEBUG_ALWAYSDTL_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGDTL_1(_log_, LOG_LV_ALWAYS, _msg_, _arg1_)
#define DEBUG_ALWAYSDTL_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDTL_2(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_)
#define DEBUG_ALWAYSDTL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDTL_3(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ALWAYSDTL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDTL_4(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ALWAYSDTL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDTL_5(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ALWAYSDTL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDTL_6(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ALWAYSDTL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDTL_7(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ALWAYSDTL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDTL_8(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ALWAYSDTL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDTL_9(_log_, LOG_LV_ALWAYS, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_ALWAYS_IF(_log_, _check_, _msg_) \
	DEBUG_LOG_IF(_log_, LOG_LV_ALWAYS, _check_, _msg_)
#define DEBUG_ALWAYS_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOG_IF_1(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_)
#define DEBUG_ALWAYS_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOG_IF_2(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_ALWAYS_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOG_IF_3(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ALWAYS_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOG_IF_4(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ALWAYS_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOG_IF_5(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ALWAYS_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOG_IF_6(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ALWAYS_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOG_IF_7(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ALWAYS_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOG_IF_8(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ALWAYS_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOG_IF_9(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_ALWAYSL_IF(_log_, _check_, _msg_) \
	DEBUG_LOGL_IF(_log_, LOG_LV_ALWAYS, _check_, _msg_)
#define DEBUG_ALWAYSL_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGL_IF_1(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_)
#define DEBUG_ALWAYSL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGL_IF_2(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_ALWAYSL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGL_IF_3(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ALWAYSL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGL_IF_4(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ALWAYSL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGL_IF_5(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ALWAYSL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGL_IF_6(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ALWAYSL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGL_IF_7(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ALWAYSL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGL_IF_8(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ALWAYSL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGL_IF_9(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_ALWAYSDT_IF(_log_, _check_, _msg_) \
	DEBUG_LOGDT_IF(_log_, LOG_LV_ALWAYS, _check_, _msg_)
#define DEBUG_ALWAYSDT_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGDT_IF_1(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_)
#define DEBUG_ALWAYSDT_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDT_IF_2(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_ALWAYSDT_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDT_IF_3(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ALWAYSDT_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDT_IF_4(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ALWAYSDT_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDT_IF_5(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ALWAYSDT_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDT_IF_6(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ALWAYSDT_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDT_IF_7(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ALWAYSDT_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDT_IF_8(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ALWAYSDT_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDT_IF_9(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_ALWAYSDTL_IF(_log_, _check_, _msg_) \
	DEBUG_LOGDTL_IF(_log_, LOG_LV_ALWAYS, _check_, _msg_)
#define DEBUG_ALWAYSDTL_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGDTL_IF_1(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_)
#define DEBUG_ALWAYSDTL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDTL_IF_2(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_ALWAYSDTL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDTL_IF_3(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ALWAYSDTL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDTL_IF_4(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ALWAYSDTL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDTL_IF_5(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ALWAYSDTL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDTL_IF_6(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ALWAYSDTL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDTL_IF_7(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ALWAYSDTL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDTL_IF_8(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ALWAYSDTL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDTL_IF_9(_log_, LOG_LV_ALWAYS, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

//-- For Error

#define DEBUG_ERROR(_log_, _msg_) \
	DEBUG_LOG(_log_, LOG_LV_ERROR, _msg_)
#define DEBUG_ERROR_1(_log_, _msg_, _arg1_) \
	DEBUG_LOG_1(_log_, LOG_LV_ERROR, _msg_, _arg1_)
#define DEBUG_ERROR_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOG_2(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_)
#define DEBUG_ERROR_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOG_3(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ERROR_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOG_4(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ERROR_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOG_5(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ERROR_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOG_6(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ERROR_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOG_7(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ERROR_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOG_8(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ERROR_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOG_9(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_ERRORL(_log_, _msg_) \
	DEBUG_LOGL(_log_, LOG_LV_ERROR, _msg_)
#define DEBUG_ERRORL_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGL_1(_log_, LOG_LV_ERROR, _msg_, _arg1_)
#define DEBUG_ERRORL_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGL_2(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_)
#define DEBUG_ERRORL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGL_3(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ERRORL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGL_4(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ERRORL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGL_5(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ERRORL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGL_6(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ERRORL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGL_7(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ERRORL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGL_8(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ERRORL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGL_9(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_ERRORDT(_log_, _msg_) \
	DEBUG_LOGDT(_log_, LOG_LV_ERROR, _msg_)
#define DEBUG_ERRORDT_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGDT_1(_log_, LOG_LV_ERROR, _msg_, _arg1_)
#define DEBUG_ERRORDT_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDT_2(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_)
#define DEBUG_ERRORDT_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDT_3(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ERRORDT_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDT_4(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ERRORDT_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDT_5(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ERRORDT_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDT_6(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ERRORDT_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDT_7(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ERRORDT_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDT_8(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ERRORDT_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDT_9(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_ERRORDTL(_log_, _msg_) \
	DEBUG_LOGDTL(_log_, LOG_LV_ERROR, _msg_)
#define DEBUG_ERRORDTL_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGDTL_1(_log_, LOG_LV_ERROR, _msg_, _arg1_)
#define DEBUG_ERRORDTL_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDTL_2(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_)
#define DEBUG_ERRORDTL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDTL_3(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ERRORDTL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDTL_4(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ERRORDTL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDTL_5(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ERRORDTL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDTL_6(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ERRORDTL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDTL_7(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ERRORDTL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDTL_8(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ERRORDTL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDTL_9(_log_, LOG_LV_ERROR, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_ERROR_IF(_log_, _check_, _msg_) \
	DEBUG_LOG_IF(_log_, LOG_LV_ERROR, _check_, _msg_)
#define DEBUG_ERROR_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOG_IF_1(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_)
#define DEBUG_ERROR_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOG_IF_2(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_ERROR_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOG_IF_3(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ERROR_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOG_IF_4(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ERROR_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOG_IF_5(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ERROR_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOG_IF_6(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ERROR_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOG_IF_7(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ERROR_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOG_IF_8(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ERROR_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOG_IF_9(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_ERRORL_IF(_log_, _check_, _msg_) \
	DEBUG_LOGL_IF(_log_, LOG_LV_ERROR, _check_, _msg_)
#define DEBUG_ERRORL_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGL_IF_1(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_)
#define DEBUG_ERRORL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGL_IF_2(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_ERRORL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGL_IF_3(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ERRORL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGL_IF_4(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ERRORL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGL_IF_5(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ERRORL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGL_IF_6(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ERRORL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGL_IF_7(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ERRORL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGL_IF_8(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ERRORL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGL_IF_9(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_ERRORDT_IF(_log_, _check_, _msg_) \
	DEBUG_LOGDT_IF(_log_, LOG_LV_ERROR, _check_, _msg_)
#define DEBUG_ERRORDT_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGDT_IF_1(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_)
#define DEBUG_ERRORDT_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDT_IF_2(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_ERRORDT_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDT_IF_3(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ERRORDT_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDT_IF_4(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ERRORDT_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDT_IF_5(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ERRORDT_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDT_IF_6(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ERRORDT_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDT_IF_7(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ERRORDT_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDT_IF_8(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ERRORDT_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDT_IF_9(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_ERRORDTL_IF(_log_, _check_, _msg_) \
	DEBUG_LOGDTL_IF(_log_, LOG_LV_ERROR, _check_, _msg_)
#define DEBUG_ERRORDTL_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGDTL_IF_1(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_)
#define DEBUG_ERRORDTL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDTL_IF_2(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_ERRORDTL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDTL_IF_3(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_ERRORDTL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDTL_IF_4(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_ERRORDTL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDTL_IF_5(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_ERRORDTL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDTL_IF_6(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_ERRORDTL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDTL_IF_7(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_ERRORDTL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDTL_IF_8(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_ERRORDTL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDTL_IF_9(_log_, LOG_LV_ERROR, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

//-- For Warning

#define DEBUG_WARN(_log_, _msg_) \
	DEBUG_LOG(_log_, LOG_LV_WARN, _msg_)
#define DEBUG_WARN_1(_log_, _msg_, _arg1_) \
	DEBUG_LOG_1(_log_, LOG_LV_WARN, _msg_, _arg1_)
#define DEBUG_WARN_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOG_2(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_)
#define DEBUG_WARN_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOG_3(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_WARN_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOG_4(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_WARN_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOG_5(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_WARN_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOG_6(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_WARN_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOG_7(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_WARN_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOG_8(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_WARN_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOG_9(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_WARNL(_log_, _msg_) \
	DEBUG_LOGL(_log_, LOG_LV_WARN, _msg_)
#define DEBUG_WARNL_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGL_1(_log_, LOG_LV_WARN, _msg_, _arg1_)
#define DEBUG_WARNL_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGL_2(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_)
#define DEBUG_WARNL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGL_3(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_WARNL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGL_4(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_WARNL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGL_5(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_WARNL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGL_6(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_WARNL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGL_7(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_WARNL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGL_8(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_WARNL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGL_9(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_WARNDT(_log_, _msg_) \
	DEBUG_LOGDT(_log_, LOG_LV_WARN, _msg_)
#define DEBUG_WARNDT_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGDT_1(_log_, LOG_LV_WARN, _msg_, _arg1_)
#define DEBUG_WARNDT_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDT_2(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_)
#define DEBUG_WARNDT_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDT_3(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_WARNDT_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDT_4(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_WARNDT_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDT_5(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_WARNDT_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDT_6(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_WARNDT_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDT_7(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_WARNDT_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDT_8(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_WARNDT_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDT_9(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_WARNDTL(_log_, _msg_) \
	DEBUG_LOGDTL(_log_, LOG_LV_WARN, _msg_)
#define DEBUG_WARNDTL_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGDTL_1(_log_, LOG_LV_WARN, _msg_, _arg1_)
#define DEBUG_WARNDTL_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDTL_2(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_)
#define DEBUG_WARNDTL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDTL_3(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_WARNDTL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDTL_4(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_WARNDTL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDTL_5(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_WARNDTL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDTL_6(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_WARNDTL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDTL_7(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_WARNDTL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDTL_8(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_WARNDTL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDTL_9(_log_, LOG_LV_WARN, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_WARN_IF(_log_, _check_, _msg_) \
	DEBUG_LOG_IF(_log_, LOG_LV_WARN, _check_, _msg_)
#define DEBUG_WARN_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOG_IF_1(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_)
#define DEBUG_WARN_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOG_IF_2(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_WARN_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOG_IF_3(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_WARN_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOG_IF_4(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_WARN_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOG_IF_5(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_WARN_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOG_IF_6(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_WARN_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOG_IF_7(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_WARN_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOG_IF_8(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_WARN_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOG_IF_9(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_WARNL_IF(_log_, _check_, _msg_) \
	DEBUG_LOGL_IF(_log_, LOG_LV_WARN, _check_, _msg_)
#define DEBUG_WARNL_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGL_IF_1(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_)
#define DEBUG_WARNL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGL_IF_2(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_WARNL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGL_IF_3(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_WARNL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGL_IF_4(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_WARNL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGL_IF_5(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_WARNL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGL_IF_6(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_WARNL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGL_IF_7(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_WARNL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGL_IF_8(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_WARNL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGL_IF_9(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_WARNDT_IF(_log_, _check_, _msg_) \
	DEBUG_LOGDT_IF(_log_, LOG_LV_WARN, _check_, _msg_)
#define DEBUG_WARNDT_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGDT_IF_1(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_)
#define DEBUG_WARNDT_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDT_IF_2(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_WARNDT_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDT_IF_3(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_WARNDT_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDT_IF_4(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_WARNDT_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDT_IF_5(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_WARNDT_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDT_IF_6(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_WARNDT_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDT_IF_7(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_WARNDT_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDT_IF_8(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_WARNDT_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDT_IF_9(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_WARNDTL_IF(_log_, _check_, _msg_) \
	DEBUG_LOGDTL_IF(_log_, LOG_LV_WARN, _check_, _msg_)
#define DEBUG_WARNDTL_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGDTL_IF_1(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_)
#define DEBUG_WARNDTL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDTL_IF_2(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_WARNDTL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDTL_IF_3(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_WARNDTL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDTL_IF_4(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_WARNDTL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDTL_IF_5(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_WARNDTL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDTL_IF_6(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_WARNDTL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDTL_IF_7(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_WARNDTL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDTL_IF_8(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_WARNDTL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDTL_IF_9(_log_, LOG_LV_WARN, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

//-- For Information

#define DEBUG_INFO(_log_, _msg_) \
	DEBUG_LOG(_log_, LOG_LV_INFO, _msg_)
#define DEBUG_INFO_1(_log_, _msg_, _arg1_) \
	DEBUG_LOG_1(_log_, LOG_LV_INFO, _msg_, _arg1_)
#define DEBUG_INFO_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOG_2(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_)
#define DEBUG_INFO_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOG_3(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_INFO_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOG_4(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_INFO_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOG_5(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_INFO_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOG_6(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_INFO_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOG_7(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_INFO_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOG_8(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_INFO_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOG_9(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_INFOL(_log_, _msg_) \
	DEBUG_LOGL(_log_, LOG_LV_INFO, _msg_)
#define DEBUG_INFOL_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGL_1(_log_, LOG_LV_INFO, _msg_, _arg1_)
#define DEBUG_INFOL_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGL_2(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_)
#define DEBUG_INFOL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGL_3(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_INFOL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGL_4(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_INFOL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGL_5(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_INFOL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGL_6(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_INFOL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGL_7(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_INFOL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGL_8(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_INFOL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGL_9(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_INFODT(_log_, _msg_) \
	DEBUG_LOGDT(_log_, LOG_LV_INFO, _msg_)
#define DEBUG_INFODT_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGDT_1(_log_, LOG_LV_INFO, _msg_, _arg1_)
#define DEBUG_INFODT_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDT_2(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_)
#define DEBUG_INFODT_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDT_3(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_INFODT_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDT_4(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_INFODT_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDT_5(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_INFODT_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDT_6(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_INFODT_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDT_7(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_INFODT_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDT_8(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_INFODT_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDT_9(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_INFODTL(_log_, _msg_) \
	DEBUG_LOGDTL(_log_, LOG_LV_INFO, _msg_)
#define DEBUG_INFODTL_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGDTL_1(_log_, LOG_LV_INFO, _msg_, _arg1_)
#define DEBUG_INFODTL_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDTL_2(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_)
#define DEBUG_INFODTL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDTL_3(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_INFODTL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDTL_4(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_INFODTL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDTL_5(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_INFODTL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDTL_6(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_INFODTL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDTL_7(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_INFODTL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDTL_8(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_INFODTL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDTL_9(_log_, LOG_LV_INFO, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_INFO_IF(_log_, _check_, _msg_) \
	DEBUG_LOG_IF(_log_, LOG_LV_INFO, _check_, _msg_)
#define DEBUG_INFO_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOG_IF_1(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_)
#define DEBUG_INFO_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOG_IF_2(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_INFO_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOG_IF_3(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_INFO_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOG_IF_4(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_INFO_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOG_IF_5(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_INFO_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOG_IF_6(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_INFO_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOG_IF_7(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_INFO_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOG_IF_8(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_INFO_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOG_IF_9(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_INFOL_IF(_log_, _check_, _msg_) \
	DEBUG_LOGL_IF(_log_, LOG_LV_INFO, _check_, _msg_)
#define DEBUG_INFOL_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGL_IF_1(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_)
#define DEBUG_INFOL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGL_IF_2(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_INFOL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGL_IF_3(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_INFOL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGL_IF_4(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_INFOL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGL_IF_5(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_INFOL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGL_IF_6(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_INFOL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGL_IF_7(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_INFOL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGL_IF_8(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_INFOL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGL_IF_9(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_INFODT_IF(_log_, _check_, _msg_) \
	DEBUG_LOGDT_IF(_log_, LOG_LV_INFO, _check_, _msg_)
#define DEBUG_INFODT_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGDT_IF_1(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_)
#define DEBUG_INFODT_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDT_IF_2(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_INFODT_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDT_IF_3(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_INFODT_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDT_IF_4(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_INFODT_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDT_IF_5(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_INFODT_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDT_IF_6(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_INFODT_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDT_IF_7(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_INFODT_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDT_IF_8(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_INFODT_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDT_IF_9(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_INFODTL_IF(_log_, _check_, _msg_) \
	DEBUG_LOGDTL_IF(_log_, LOG_LV_INFO, _check_, _msg_)
#define DEBUG_INFODTL_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGDTL_IF_1(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_)
#define DEBUG_INFODTL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDTL_IF_2(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_INFODTL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDTL_IF_3(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_INFODTL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDTL_IF_4(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_INFODTL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDTL_IF_5(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_INFODTL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDTL_IF_6(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_INFODTL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDTL_IF_7(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_INFODTL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDTL_IF_8(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_INFODTL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDTL_IF_9(_log_, LOG_LV_INFO, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

//-- For Verbose

#define DEBUG_VERBOSE(_log_, _msg_) \
	DEBUG_LOG(_log_, LOG_LV_VERBOSE, _msg_)
#define DEBUG_VERBOSE_1(_log_, _msg_, _arg1_) \
	DEBUG_LOG_1(_log_, LOG_LV_VERBOSE, _msg_, _arg1_)
#define DEBUG_VERBOSE_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOG_2(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_)
#define DEBUG_VERBOSE_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOG_3(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_VERBOSE_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOG_4(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_VERBOSE_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOG_5(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_VERBOSE_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOG_6(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_VERBOSE_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOG_7(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_VERBOSE_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOG_8(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_VERBOSE_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOG_9(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_VERBOSEL(_log_, _msg_) \
	DEBUG_LOGL(_log_, LOG_LV_VERBOSE, _msg_)
#define DEBUG_VERBOSEL_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGL_1(_log_, LOG_LV_VERBOSE, _msg_, _arg1_)
#define DEBUG_VERBOSEL_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGL_2(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_)
#define DEBUG_VERBOSEL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGL_3(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_VERBOSEL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGL_4(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_VERBOSEL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGL_5(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_VERBOSEL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGL_6(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_VERBOSEL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGL_7(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_VERBOSEL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGL_8(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_VERBOSEL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGL_9(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_VERBOSEDT(_log_, _msg_) \
	DEBUG_LOGDT(_log_, LOG_LV_VERBOSE, _msg_)
#define DEBUG_VERBOSEDT_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGDT_1(_log_, LOG_LV_VERBOSE, _msg_, _arg1_)
#define DEBUG_VERBOSEDT_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDT_2(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_)
#define DEBUG_VERBOSEDT_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDT_3(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_VERBOSEDT_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDT_4(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_VERBOSEDT_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDT_5(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_VERBOSEDT_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDT_6(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_VERBOSEDT_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDT_7(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_VERBOSEDT_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDT_8(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_VERBOSEDT_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDT_9(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_VERBOSEDTL(_log_, _msg_) \
	DEBUG_LOGDTL(_log_, LOG_LV_VERBOSE, _msg_)
#define DEBUG_VERBOSEDTL_1(_log_, _msg_, _arg1_) \
	DEBUG_LOGDTL_1(_log_, LOG_LV_VERBOSE, _msg_, _arg1_)
#define DEBUG_VERBOSEDTL_2(_log_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDTL_2(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_)
#define DEBUG_VERBOSEDTL_3(_log_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDTL_3(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_VERBOSEDTL_4(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDTL_4(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_VERBOSEDTL_5(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDTL_5(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_VERBOSEDTL_6(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDTL_6(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_VERBOSEDTL_7(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDTL_7(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_VERBOSEDTL_8(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDTL_8(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_VERBOSEDTL_9(_log_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDTL_9(_log_, LOG_LV_VERBOSE, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_VERBOSE_IF(_log_, _check_, _msg_) \
	DEBUG_LOG_IF(_log_, LOG_LV_VERBOSE, _check_, _msg_)
#define DEBUG_VERBOSE_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOG_IF_1(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_)
#define DEBUG_VERBOSE_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOG_IF_2(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_VERBOSE_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOG_IF_3(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_VERBOSE_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOG_IF_4(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_VERBOSE_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOG_IF_5(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_VERBOSE_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOG_IF_6(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_VERBOSE_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOG_IF_7(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_VERBOSE_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOG_IF_8(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_VERBOSE_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOG_IF_9(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_VERBOSEL_IF(_log_, _check_, _msg_) \
	DEBUG_LOGL_IF(_log_, LOG_LV_VERBOSE, _check_, _msg_)
#define DEBUG_VERBOSEL_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGL_IF_1(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_)
#define DEBUG_VERBOSEL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGL_IF_2(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_VERBOSEL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGL_IF_3(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_VERBOSEL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGL_IF_4(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_VERBOSEL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGL_IF_5(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_VERBOSEL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGL_IF_6(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_VERBOSEL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGL_IF_7(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_VERBOSEL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGL_IF_8(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_VERBOSEL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGL_IF_9(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#define DEBUG_VERBOSEDT_IF(_log_, _check_, _msg_) \
	DEBUG_LOGDT_IF(_log_, LOG_LV_VERBOSE, _check_, _msg_)
#define DEBUG_VERBOSEDT_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGDT_IF_1(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_)
#define DEBUG_VERBOSEDT_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDT_IF_2(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_VERBOSEDT_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDT_IF_3(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_VERBOSEDT_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDT_IF_4(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_VERBOSEDT_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDT_IF_5(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_VERBOSEDT_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDT_IF_6(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_VERBOSEDT_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDT_IF_7(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_VERBOSEDT_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDT_IF_8(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_VERBOSEDT_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDT_IF_9(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)
#define DEBUG_VERBOSEDTL_IF(_log_, _check_, _msg_) \
	DEBUG_LOGDTL_IF(_log_, LOG_LV_VERBOSE, _check_, _msg_)
#define DEBUG_VERBOSEDTL_IF_1(_log_, _check_, _msg_, _arg1_) \
	DEBUG_LOGDTL_IF_1(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_)
#define DEBUG_VERBOSEDTL_IF_2(_log_, _check_, _msg_, _arg1_, _arg2_) \
	DEBUG_LOGDTL_IF_2(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_)
#define DEBUG_VERBOSEDTL_IF_3(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_) \
	DEBUG_LOGDTL_IF_3(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_)
#define DEBUG_VERBOSEDTL_IF_4(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_) \
	DEBUG_LOGDTL_IF_4(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_)
#define DEBUG_VERBOSEDTL_IF_5(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_) \
	DEBUG_LOGDTL_IF_5(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_)
#define DEBUG_VERBOSEDTL_IF_6(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_) \
	DEBUG_LOGDTL_IF_6(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_)
#define DEBUG_VERBOSEDTL_IF_7(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_) \
	DEBUG_LOGDTL_IF_7(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_)
#define DEBUG_VERBOSEDTL_IF_8(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_) \
	DEBUG_LOGDTL_IF_8(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_)
#define DEBUG_VERBOSEDTL_IF_9(_log_, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_) \
	DEBUG_LOGDTL_IF_9(_log_, LOG_LV_VERBOSE, _check_, _msg_, _arg1_, _arg2_, _arg3_, _arg4_, _arg5_, _arg6_, _arg7_, _arg8_, _arg9_)

#endif // _REUBEN_DIAGNOSTICS_DEBUG_H_
