// dboLogQuest.h : Declaration of the CdboLogQuest

#pragma once
#ifndef _DBO_LOG_QUEST_H_
#define _DBO_LOG_QUEST_H_
#include "Dbo.h"

// code generated on Tuesday, August 12, 2008, 18:21 AM

class CdboLogQuestAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogQuestAccessor)

	DEFINE_ENTRY_STR(sess_id, 13)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(line_id, INT)
	DEFINE_ENTRY(source_type, INT)
	DEFINE_ENTRY(source_id, LARGE_INTEGER)
	DEFINE_ENTRY(quest_id, LARGE_INTEGER)
	DEFINE_ENTRY(action_type, INT)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogQuestAccessor)
	//ENABLE_PARAM_MAP(CdboLogQuestAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_sess_id)
		COLUMN_ENTRY(2, m_char_id)
		COLUMN_ENTRY(3, m_line_id)
		COLUMN_ENTRY(4, m_source_type)
		COLUMN_ENTRY(5, m_source_id)
		COLUMN_ENTRY(6, m_quest_id)
		COLUMN_ENTRY(7, m_action_type)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogQuestAccessor, L"SELECT * FROM LogQuest")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogQuest : public Cdbo<CCommand<CAccessor<CdboLogQuestAccessor> > >
{
public:

	HRESULT Log(PCSTRING pSessId, UInt32 char_id, UInt8 line_id, UInt8 source_type, UInt32 source_id, UInt32 quest_id, UInt8 action_type)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		COLUMN_STRING_COPY(m_sess_id, pSessId)
		m_char_id.QuadPart = char_id;
		m_line_id = line_id;
		m_source_type = source_type;
		m_source_id.QuadPart = source_id;
		m_quest_id.QuadPart = quest_id;
		m_action_type = action_type;
		return OpenCommand(_T("INSERT INTO LogQuest (sess_id, char_id, line_id, source_type, source_id, quest_id, action_type, create_time) VALUES (?, ?, ?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_QUEST_H_
