// dboLogSMoney.h : Declaration of the CdboLogSMoney

#pragma once
#ifndef _DBO_LOG_SMONEY_H_
#define _DBO_LOG_SMONEY_H_
#include "Dbo.h"

// code generated on Tuesday, August 12, 2008, 18:21 AM

class CdboLogSMoneyAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogSMoneyAccessor)

	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(source, INT)
	DEFINE_ENTRY(smoney, INT)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogSMoneyAccessor)
	//ENABLE_PARAM_MAP(CdboLogSMoneyAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_acct_id)
		COLUMN_ENTRY(2, m_char_id)
		COLUMN_ENTRY(3, m_source)
		COLUMN_ENTRY(4, m_smoney)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogSMoneyAccessor, L"SELECT * FROM LogSMoney")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogSMoney : public Cdbo<CCommand<CAccessor<CdboLogSMoneyAccessor> > >
{
public:

	HRESULT Log(UInt32 acct_id, UInt32 char_id, UInt16 source, UInt32 smoney)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		m_acct_id.QuadPart = acct_id;
		m_char_id.QuadPart = char_id;
		m_source = source;
		m_smoney = smoney;
		return OpenCommand(_T("INSERT INTO LogSMoney (acct_id, char_id, source, smoney, create_time) VALUES (?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_SMONEY_H_
