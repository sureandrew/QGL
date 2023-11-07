// dboLogMoney.h : Declaration of the CdboLogMoney

#pragma once
#ifndef _DBO_LOG_MONEY_H_
#define _DBO_LOG_MONEY_H_
#include "Dbo.h"

// code generated on Tuesday, August 12, 2008, 18:21 AM
#ifdef LOG_FOR_DETAIL
class CdboLogMoneyAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogMoneyAccessor)

	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(source, INT)
	DEFINE_ENTRY(money, INT)
	DEFINE_ENTRY(totalMoney, LARGE_INTEGER)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogMoneyAccessor)
	//ENABLE_PARAM_MAP(CdboLogMoneyAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_acct_id)
		COLUMN_ENTRY(2, m_char_id)
		COLUMN_ENTRY(3, m_source)
		COLUMN_ENTRY(4, m_money)
		COLUMN_ENTRY(5, m_totalMoney)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogMoneyAccessor, L"SELECT * FROM LogMoney")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogMoney : public Cdbo<CCommand<CAccessor<CdboLogMoneyAccessor> > >
{
public:

	HRESULT Log(UInt32 acct_id, UInt32 char_id, UInt16 source, UInt32 money, UInt32 totalMoney)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		m_acct_id.QuadPart = acct_id;
		m_char_id.QuadPart = char_id;
		m_source = source;
		m_money = money;
		m_totalMoney.QuadPart = totalMoney;
		return OpenCommand(_T("INSERT INTO LogMoney (acct_id, char_id, source, money, create_time, totalMoney) VALUES (?, ?, ?, ?, GetDate(), ?)"));
	}
};


#else
class CdboLogMoneyAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogMoneyAccessor)

	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(source, INT)
	DEFINE_ENTRY(money, INT)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogMoneyAccessor)
	//ENABLE_PARAM_MAP(CdboLogMoneyAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_acct_id)
		COLUMN_ENTRY(2, m_char_id)
		COLUMN_ENTRY(3, m_source)
		COLUMN_ENTRY(4, m_money)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogMoneyAccessor, L"SELECT * FROM LogMoney")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogMoney : public Cdbo<CCommand<CAccessor<CdboLogMoneyAccessor> > >
{
public:

	HRESULT Log(UInt32 acct_id, UInt32 char_id, UInt16 source, UInt32 money)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		m_acct_id.QuadPart = acct_id;
		m_char_id.QuadPart = char_id;
		m_source = source;
		m_money = money;
		return OpenCommand(_T("INSERT INTO LogMoney (acct_id, char_id, source, money, create_time) VALUES (?, ?, ?, ?, GetDate())"));
	}
};


#endif

#endif // _DBO_LOG_MONEY_H_
