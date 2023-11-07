// dboLogCharacter.h : Declaration of the CdboLogCharacter

#pragma once
#ifndef _DBO_LOG_CHAREXP_H_
#define _DBO_LOG_CHAREXP_H_
#include "Dbo.h"

// code generated on Tuesday, August 12, 2008, 18:21 AM
#ifdef LOG_FOR_DETAIL
class CdboLogChaExpAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogChaExpAccessor)

	DEFINE_ENTRY(exp_type, INT)
	DEFINE_ENTRY(exp_value, LARGE_INTEGER)
	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(total_exp_value, LARGE_INTEGER)
	DEFINE_ENTRY(level, LONG)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogChaExpAccessor)
	//ENABLE_PARAM_MAP(CdboLogChaExpAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_exp_type)
		COLUMN_ENTRY(2, m_exp_value)
		COLUMN_ENTRY(3, m_acct_id)
		COLUMN_ENTRY(4, m_char_id)
		COLUMN_ENTRY(5, m_total_exp_value)
		COLUMN_ENTRY(6, m_level)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogChaExpAccessor, L"SELECT * FROM LogCharExp")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogCharExp : public Cdbo<CCommand<CAccessor<CdboLogChaExpAccessor> > >
{
public:

	HRESULT Log(UInt16 exp_type, Int32 exp_value, UInt32 acct_id, UInt32 char_id, UInt32 total_exp_value, UInt16 level)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		m_exp_type = exp_type;
		m_exp_value.QuadPart = exp_value;
		m_acct_id.QuadPart = acct_id;
		m_char_id.QuadPart = char_id;
		m_total_exp_value.QuadPart = total_exp_value;
		m_level = level;

		return OpenCommand(_T("INSERT INTO LogCharExp (exp_type, exp_value, acct_id, char_id, create_time, total_exp_value, \"level\") VALUES (?, ?, ?, ?, GetDate(), ?, ?)"));
	}
};

#else
class CdboLogChaExpAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogChaExpAccessor)

	DEFINE_ENTRY(exp_type, INT)
	DEFINE_ENTRY(exp_value, LARGE_INTEGER)
	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogChaExpAccessor)
	//ENABLE_PARAM_MAP(CdboLogChaExpAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_exp_type)
		COLUMN_ENTRY(2, m_exp_value)
		COLUMN_ENTRY(3, m_acct_id)
		COLUMN_ENTRY(4, m_char_id)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogChaExpAccessor, L"SELECT * FROM LogCharExp")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogCharExp : public Cdbo<CCommand<CAccessor<CdboLogChaExpAccessor> > >
{
public:

	HRESULT Log(UInt16 exp_type, Int32 exp_value, UInt32 acct_id, UInt32 char_id)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		m_exp_type = exp_type;
		m_exp_value.QuadPart = exp_value;
		m_acct_id.QuadPart = acct_id;
		m_char_id.QuadPart = char_id;

		return OpenCommand(_T("INSERT INTO LogCharExp (exp_type, exp_value, acct_id, char_id, create_time) VALUES (?, ?, ?, ?, GetDate())"));
	}
};

#endif

#endif // _DBO_LOG_CHARACTER_H_
