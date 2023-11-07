// dboLogHack.h : Declaration of the CdboLogHack

#pragma once
#ifndef _DBO_LOG_HACK_H_
#define _DBO_LOG_HACK_H_
#include "Dbo.h"

// code generated on Tuesday, August 12, 2008, 18:21 AM

class CdboLogHackAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogHackAccessor)

	DEFINE_ENTRY_STR(Accounts, 32)
	DEFINE_ENTRY_STR(CharacterName, 64)
	DEFINE_ENTRY(Time, LONG)
	DEFINE_ENTRY(Type, INT)
	DEFINE_ENTRY_STR(TypeName, 32)
	DEFINE_ENTRY_STR(LogInfo, 260)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogHackAccessor)
	//ENABLE_PARAM_MAP(CdboLogHackAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_Accounts)
		COLUMN_ENTRY(2, m_CharacterName)
		COLUMN_ENTRY(3, m_Time)
		COLUMN_ENTRY(4, m_Type)
		COLUMN_ENTRY(5, m_TypeName)
		COLUMN_ENTRY(6, m_LogInfo)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogHackAccessor, L"SELECT Accounts, CharacterName, Time, Type, TypeName, LogInfo FROM LogHack")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogHack : public Cdbo<CCommand<CAccessor<CdboLogHackAccessor> > >
{
public:

	HRESULT Log(PCSTRING acct_name, PCSTRING char_name, UInt32 msg_time, UInt16 msg_type,
				PCSTRING type_name, PCSTRING msg)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		COLUMN_STRING_COPY(m_Accounts, acct_name);
		COLUMN_STRING_COPY(m_CharacterName, char_name);
		m_Time = msg_time;
		m_Type = msg_type;
		COLUMN_STRING_COPY(m_TypeName, type_name);
		COLUMN_STRING_COPY(m_LogInfo, msg);

		return OpenCommand(_T("INSERT INTO LogHack (Accounts, CharacterName, Time, Type, TypeName, LogInfo, logtime) VALUES (?, ?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_HACK_H_
