// dboExecuteCheat.h : Declaration of the CdboExecuteCheat

#pragma once
#ifndef _DBO_EXECUTE_CHEAT_H_
#define _DBO_EXECUTE_CHEAT_H_
#include "Dbo.h"

class CdboExecuteCheatAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboExecuteCheatAccessor)

	DEFINE_ENTRY(exe_id, LARGE_INTEGER)
	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(target_acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(target_char_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(command, 256)
	DEFINE_ENTRY(status, INT)
	DEFINE_ENTRY(create_time, DBTIMESTAMP)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboExecuteCheatAccessor)
	//ENABLE_PARAM_MAP(CdboExecuteCheatAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		int i = 0;
		COLUMN_ENTRY_LS(++i, m_exe_id)
		COLUMN_ENTRY_LS(++i, m_acct_id)
		COLUMN_ENTRY_LS(++i, m_target_acct_id)
		COLUMN_ENTRY_LS(++i, m_target_char_id)
		COLUMN_ENTRY_LS(++i, m_command)
		COLUMN_ENTRY_LS(++i, m_status)
		COLUMN_ENTRY_LS(++i, m_create_time)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_Update)
		COLUMN_ENTRY(1, m_exe_id)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboExecuteCheatAccessor, L"SELECT * FROM ExecuteCheat")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboExecuteCheat : public Cdbo<CCommand<CAccessor<CdboExecuteCheatAccessor> > >
{
public:

	HRESULT RSByUnread()
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(NULL);
		return OpenCommand(_T("SELECT * FROM ExecuteCheat WHERE status = 0"));
	}

	HRESULT UpdateAsRead(UInt32 exe_id)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Update);
		m_exe_id.QuadPart = exe_id;
		return OpenCommand(_T("UPDATE ExecuteCheat SET status = 1 WHERE exe_id = ?;"));
	}

	HRESULT UpdateUnreadAsFailed()
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(NULL);
		return OpenCommand(_T("UPDATE ExecuteCheat SET status = 2 WHERE status = 0;"));
	}
};

#endif // _DBO_EXECUTE_CHEAT_H_
