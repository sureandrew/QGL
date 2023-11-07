// dboLogCharacter.h : Declaration of the CdboLogCharacter

#pragma once
#ifndef _DBO_LOG_CHARACTER_H_
#define _DBO_LOG_CHARACTER_H_
#include "Dbo.h"

// code generated on Tuesday, August 12, 2008, 18:21 AM

class CdboLogCharacterAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogCharacterAccessor)

	DEFINE_ENTRY_STR(sess_id, 13)
	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(line_id, INT)
	DEFINE_ENTRY(action_type, INT)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogCharacterAccessor)
	//ENABLE_PARAM_MAP(CdboLogCharacterAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_sess_id)
		COLUMN_ENTRY(2, m_acct_id)
		COLUMN_ENTRY(3, m_char_id)
		COLUMN_ENTRY(4, m_line_id)
		COLUMN_ENTRY(5, m_action_type)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogCharacterAccessor, L"SELECT * FROM LogCharacter")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogCharacter : public Cdbo<CCommand<CAccessor<CdboLogCharacterAccessor> > >
{
public:

	HRESULT Log(PCSTRING pSessId, UInt32 acct_id, UInt32 char_id, UInt8 line_id, UInt8 action_type)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		COLUMN_STRING_COPY(m_sess_id, pSessId)
		m_acct_id.QuadPart = acct_id;
		m_char_id.QuadPart = char_id;
		m_line_id = line_id;
		m_action_type = action_type;
		return OpenCommand(_T("INSERT INTO LogCharacter (sess_id, acct_id, char_id, line_id, action_type, create_time) VALUES (?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_CHARACTER_H_
