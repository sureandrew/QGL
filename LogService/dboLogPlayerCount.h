// dboLogPlayerCount.h : Declaration of the CdboLogPlayerCount

#pragma once
#ifndef _DBO_LOG_PLAYER_COUNT_H_
#define _DBO_LOG_PLAYER_COUNT_H_
#include "Dbo.h"

// code generated on Tuesday, August 12, 2008, 18:21 AM

class CdboLogPlayerCountAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogPlayerCountAccessor)

	DEFINE_ENTRY(line_id, INT)
	DEFINE_ENTRY(player_count, INT)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogPlayerCountAccessor)
	//ENABLE_PARAM_MAP(CdboLogPlayerCountAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_line_id)
		COLUMN_ENTRY(2, m_player_count)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogPlayerCountAccessor, L"SELECT * FROM LogPlayerCount")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogPlayerCount : public Cdbo<CCommand<CAccessor<CdboLogPlayerCountAccessor> > >
{
public:

	HRESULT Log(UInt8 line_id, UInt32 player_count)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		m_line_id = line_id;
		m_player_count = player_count;
		return OpenCommand(_T("INSERT INTO LogPlayerCount (line_id, player_count, create_time) VALUES (?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_PLAYER_COUNT_H_
