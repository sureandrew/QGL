#pragma once
#ifndef _REUBEN_DIAGNOSTICS_EXCEPTION_MANAGER_H_
#define _REUBEN_DIAGNOSTICS_EXCEPTION_MANAGER_H_

#include "..\Utility\Singleton.h"

namespace Reuben { namespace Diagnostics {

	class SEException : public Exception
	{
	private:

		EXCEPTION_POINTERS * m_pExceptionInfo;

	public:

		SEException(CONST ErrorCode ec, PCSTRING pSourceLine, EXCEPTION_POINTERS * pExceptionInfo);

		INLINE EXCEPTION_POINTERS * GetPointers(VOID) CONST;

	};

	class ExceptionHandler
	{
	public:

		virtual PCSTRING GetApplicationName(VOID) CONST = 0;
		virtual PCSTRING GetVersionString(VOID) CONST = 0;
		virtual CONST UInt32 GetVersionNumber(VOID) CONST = 0;
		virtual CONST UInt32 GetBuildNumber(VOID) CONST = 0;

		VOID Report(Exception & ex);
		VOID Report(EXCEPTION_POINTERS * pExceptionInfo);

	};

	template<typename THandler>
	class ExceptionManager
		: public THandler
		, public Reuben::Utility::Singleton<ExceptionManager<THandler>, Reuben::Utility::Allocation::Static>
	{
	private:

		ExceptionManager(VOID);
		SINGLETON(ExceptionManager);

	public:

		static LONG WINAPI SEFilter(EXCEPTION_POINTERS * pExceptionInfo);

		static VOID SETranslator(unsigned uCode, EXCEPTION_POINTERS * pExceptionInfo);

	};

	//-- Implementation

	INLINE EXCEPTION_POINTERS * SEException::GetPointers(VOID) CONST
	{
		return (m_pExceptionInfo);
	}

	template<typename THandler>
		ExceptionManager<THandler>::ExceptionManager(VOID)
	{
#ifdef COMPILER_DEBUG
		int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
		_CrtSetDbgFlag(tmpFlag | _CRTDBG_LEAK_CHECK_DF);
#endif // COMPILER_DEBUG
		::SetUnhandledExceptionFilter(SEFilter);
		::_set_se_translator(SETranslator);
	}

	template<typename THandler>
		LONG WINAPI ExceptionManager<THandler>::SEFilter(EXCEPTION_POINTERS * pExceptionInfo)
	{
		ExceptionManager<THandler>::GetInstance().Report(pExceptionInfo);
		return (EXCEPTION_EXECUTE_HANDLER);
	}

	template<typename THandler>
		VOID ExceptionManager<THandler>::SETranslator(unsigned uCode, EXCEPTION_POINTERS * pExceptionInfo)
	{
		ExceptionManager<THandler>::GetInstance().Report(pExceptionInfo);
		throw SEException((ErrorCode)uCode, _SOURCE_LINE_, pExceptionInfo);
	}

	template<typename THandler>
	class ExceptionManagerInitializer
	{
	public:

		ExceptionManagerInitializer(VOID)
		{ 
			ExceptionManager<THandler>::GetInstance();
		}

	};

}; }; // Reuben::Diagnostics

#define INIT_EXCEPTION_HANDLER(_handler_) \
	static Reuben::Diagnostics::ExceptionManagerInitializer<_handler_> g_exceptionManagerInitializer

#endif // _REUBEN_DIAGNOSTICS_EXCEPTION_MANAGER_H_
