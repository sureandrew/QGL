// CdboLogGuildDonate.h : Declaration of the CdboLogGuildDonate

#pragma once
#ifndef _DBO_LOG_GUILDDONATE_H_
#define _DBO_LOG_GUILDDONATE_H_
#include "Dbo.h"


class CdboLogGuildDonateAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogGuildDonateAccessor)

	DEFINE_ENTRY(guild_uid, LARGE_INTEGER)
	DEFINE_ENTRY(char_uid, LARGE_INTEGER)
	DEFINE_ENTRY(donate, LARGE_INTEGER)
	DEFINE_ENTRY(total_donate, LARGE_INTEGER)
	DEFINE_ENTRY(source, INT)


	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_guild_uid)
		COLUMN_ENTRY(2, m_char_uid)
		COLUMN_ENTRY(3, m_donate)
		COLUMN_ENTRY(4, m_total_donate)
		COLUMN_ENTRY(5, m_source)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogGuildDonateAccessor, L"SELECT * FROM LogGuildDonate")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogGuildDonate : public Cdbo<CCommand<CAccessor<CdboLogGuildDonateAccessor> > >
{
public:

	HRESULT Log(UInt32 guild_uid, UInt32 char_uid, Int32 donate, UInt32 total_donate, UInt8 source)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);

		m_guild_uid.QuadPart = guild_uid;
		m_char_uid.QuadPart = char_uid;
		m_donate.QuadPart = donate;
		m_total_donate.QuadPart = total_donate;
		m_source = source;

		return OpenCommand(_T("INSERT INTO LogGuildDonate (guild_uid, char_uid, donate, total_donate, source, create_time) VALUES (?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_GUILDDONATE_H_