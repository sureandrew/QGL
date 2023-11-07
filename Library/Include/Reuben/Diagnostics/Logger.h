#pragma once
#ifndef _REUBEN_DIAGNOSTICS_LOGGER_H_
#define _REUBEN_DIAGNOSTICS_LOGGER_H_

//-- Local
#include "..\Utility\ConfigFile.h"

namespace Reuben { namespace Diagnostics {

	class Logger
	{
	public:

		enum LevelEnum
		{
			LEVEL_ALWAYS,
			LEVEL_ERROR,
			LEVEL_WARN,
			LEVEL_INFO,
			LEVEL_VERBOSE,

			LEVEL_COUNT
		};

	private:

		String m_strName;
		String m_strParam1;
		String m_strParam2;
		String m_strLogFileName;
		Boolean m_bConsole;
		Boolean m_bDateTime;
		Boolean m_bAppend;
		LevelEnum m_eLevel;
		size_t m_sizeLimit;
		String m_strArchive;
		FILE* m_pFile;

	public:

		Logger(PCSTRING pName, PCSTRING pParam1 = STRING_EMPTY, PCSTRING pParam2 = STRING_EMPTY);
		virtual ~Logger(VOID);

	private:

		CONST Boolean Open(VOID);
		Boolean CheckSizeLimit(Boolean bForce=BOOLEAN_FALSE);

	public:

		CONST Boolean Open(CONST Reuben::Utility::ConfigFile& config);
		CONST Boolean Open(
			PCSTRING pLogFileName,
			Boolean bConsole = BOOLEAN_FALSE,
			Boolean bDateTime = BOOLEAN_TRUE,
			Boolean bAppend = BOOLEAN_FALSE,
			LevelEnum eLevel = LEVEL_VERBOSE,
			Int sizeLimit = 0,
			PCSTRING pszArchive = NULL
		);
        VOID Close(VOID);

		INLINE CONST String& GetName(VOID) CONST;
		INLINE CONST String& GetParamm1(VOID) CONST;
		INLINE CONST String& GetParamm2(VOID) CONST;
		INLINE CONST String& GetLogFileName(VOID) CONST;
		INLINE CONST Boolean HasConsole(VOID) CONST;
		INLINE CONST Boolean HasDateTime(VOID) CONST;
		INLINE CONST Boolean IsAppend(VOID) CONST;
		INLINE VOID SetLevel(LevelEnum eLevel);
		INLINE VOID SetConsole(Boolean bConsole);
		INLINE VOID SetFile(FILE* file);
		INLINE LevelEnum GetLevel(VOID) CONST;
		VOID ForceArchive();

		VOID WriteCurrentDateTime(VOID) CONST;

		VOID Write(PCSTRING pMessage);
		VOID WriteFormat(PCSTRING pFormat, ...);
		VOID WriteFormat(PCSTRING pFormat, va_list marker);
		VOID WriteLine(PCSTRING pMessage);
		VOID WriteFormatLine(PCSTRING pFormat, ...);
		VOID WriteFormatLine(PCSTRING pFormat, va_list marker);

	};

	INLINE CONST String& Logger::GetName(VOID) CONST
	{
		return (m_strName);
	}

	INLINE CONST String& Logger::GetParamm1(VOID) CONST
	{
		return (m_strParam1);
	}

	INLINE CONST String& Logger::GetParamm2(VOID) CONST
	{
		return (m_strParam2);
	}

	INLINE CONST String& Logger::GetLogFileName(VOID) CONST
	{
		return (m_strLogFileName);
	}

	INLINE CONST Boolean Logger::HasConsole(VOID) CONST
	{
		return (m_bConsole);
	}

	INLINE CONST Boolean Logger::HasDateTime(VOID) CONST
	{
		return (m_bDateTime);
	}

	INLINE CONST Boolean Logger::IsAppend(VOID) CONST
	{
		return (m_bAppend);
	}

	INLINE VOID Logger::SetConsole(Boolean bConsole)
	{
		m_bConsole = bConsole;
	}

	INLINE VOID Logger::SetFile(FILE* file)
	{
		m_pFile = file;
	}

	INLINE VOID Logger::SetLevel(Logger::LevelEnum eLevel)
	{
		m_eLevel = eLevel;
	}

	INLINE Logger::LevelEnum Logger::GetLevel(VOID) CONST
	{
		return (m_eLevel);
	}

}; }; // Reuben::Diagnostics

#define LOG_LV_ALWAYS Reuben::Diagnostics::Logger::LEVEL_ALWAYS
#define LOG_LV_ERROR Reuben::Diagnostics::Logger::LEVEL_ERROR
#define LOG_LV_WARN Reuben::Diagnostics::Logger::LEVEL_WARN
#define LOG_LV_INFO Reuben::Diagnostics::Logger::LEVEL_INFO
#define LOG_LV_VERBOSE Reuben::Diagnostics::Logger::LEVEL_VERBOSE
#define _F(_msg_) _T(__FUNCTION__) _T(" : ") _T(_msg_)
#define _FE(_msg_) _T(__FUNCTION__) _T(" Error : ") _T(_msg_)
#define _FW(_msg_) _T(__FUNCTION__) _T(" Warning : ") _T(_msg_)

#endif // _REUBEN_DIAGNOSTICS_LOGGER_H_
