// CdboLogManor.h : Declaration of the CdboLogManor

#pragma once
#ifndef _DBO_LOG_MANOR_H_
#define _DBO_LOG_MANOR_H_
#include "Dbo.h"


class CdboLogManorAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogManorAccessor)

	DEFINE_ENTRY(guild_uid, LARGE_INTEGER)
	DEFINE_ENTRY(guild_fund, LARGE_INTEGER)
	DEFINE_ENTRY(guild_prosper, LARGE_INTEGER)
	DEFINE_ENTRY(manor_scale, INT)
	DEFINE_ENTRY(fund_source, INT)
	DEFINE_ENTRY(prosper_source, INT)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_guild_uid)
		COLUMN_ENTRY(2, m_guild_fund)
		COLUMN_ENTRY(3, m_guild_prosper)
		COLUMN_ENTRY(4, m_manor_scale)
		COLUMN_ENTRY(5, m_fund_source)
		COLUMN_ENTRY(6, m_prosper_source)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogManorAccessor, L"SELECT * FROM LogManor")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogManor : public Cdbo<CCommand<CAccessor<CdboLogManorAccessor> > >
{
public:

	HRESULT Log(UInt32 guild_uid, UInt32 fund, UInt32 prosper, UInt8 manor_scale, UInt8 fundSrc, UInt8 prosperSrc)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		
		m_guild_uid.QuadPart = guild_uid;
		m_guild_fund.QuadPart = fund;
		m_guild_prosper.QuadPart = prosper;
		m_manor_scale = manor_scale;
		m_fund_source = fundSrc;
		m_prosper_source = prosperSrc;

		return OpenCommand(_T("INSERT INTO LogManor (guild_uid, guild_fund, guild_prosper, manor_scale, fund_source, prosper_source, create_time) VALUES (?, ?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_MANOR_H_

