// dboLogCheat.h : Declaration of the CdboLogCheat

#pragma once
#ifndef _DBO_LOG_CHEAT_H_
#define _DBO_LOG_CHEAT_H_
#include "Dbo.h"


class CdboLogCheatAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogCheatAccessor)

	DEFINE_ENTRY(exe_id, INT)
	DEFINE_ENTRY(source_acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(target_acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(target_char_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(command, 256)
	DEFINE_ENTRY(result, INT)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogCheatAccessor)
	//ENABLE_PARAM_MAP(CdboLogCheatAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_exe_id)
		COLUMN_ENTRY(2, m_source_acct_id)
		COLUMN_ENTRY(3, m_target_acct_id)
		COLUMN_ENTRY(4, m_target_char_id)
		COLUMN_ENTRY(5, m_command)
		COLUMN_ENTRY(6, m_result)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogCheatAccessor, L"SELECT * FROM LogCheat")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogCheat : public Cdbo<CCommand<CAccessor<CdboLogCheatAccessor> > >
{
public:

	HRESULT Log(UInt32 exe_id, UInt32 source_acct_id, UInt32 target_acct_id, UInt32 target_char_id, PCSTRING command, UInt8 result)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		
		m_exe_id					= exe_id;
		m_source_acct_id.QuadPart	= source_acct_id;
		m_target_acct_id.QuadPart	= target_acct_id;
		m_target_char_id.QuadPart	= target_char_id;
		COLUMN_STRING_COPY(m_command, command);
		m_result					= result;

		return OpenCommand(_T("INSERT INTO LogCheat (exe_id, source_acct_id, target_acct_id, target_char_id, command, result, create_time) VALUES (?, ?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_CHEAT_H_
