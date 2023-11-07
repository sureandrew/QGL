// dboLogGoodness.h : Declaration of the CdboLogGoodness

#pragma once
#ifndef _DBO_LOG_GOODNESS_H_
#define _DBO_LOG_GOODNESS_H_
#include "Dbo.h"

// code generated on Tuesday, August 12, 2008, 18:21 AM

class CdboLogGoodnessAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogGoodnessAccessor)

	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(source, INT)
	DEFINE_ENTRY(goodness, INT)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogGoodnessAccessor)
	//ENABLE_PARAM_MAP(CdboLogGoodnessAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_acct_id)
		COLUMN_ENTRY(2, m_char_id)
		COLUMN_ENTRY(3, m_source)
		COLUMN_ENTRY(4, m_goodness)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogGoodnessAccessor, L"SELECT * FROM LogGoodness")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogGoodness : public Cdbo<CCommand<CAccessor<CdboLogGoodnessAccessor> > >
{
public:

	HRESULT Log(UInt32 acct_id, UInt32 char_id, UInt16 source, Int32 goodness)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		m_acct_id.QuadPart = acct_id;
		m_char_id.QuadPart = char_id;
		m_source = source;
		m_goodness = goodness;
		return OpenCommand(_T("INSERT INTO LogGoodness (acct_id, char_id, source, goodness, create_time) VALUES (?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_GOODNESS_H_
