// dboLogItem.h : Declaration of the CdboLogItem

#pragma once
#ifndef _DBO_LOG_ITEM_H_
#define _DBO_LOG_ITEM_H_
#include "Dbo.h"

// code generated on Tuesday, August 12, 2008, 18:21 AM

class CdboLogItemAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogItemAccessor)

	DEFINE_ENTRY_STR(sess_id, 13)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(target_type, INT)
	DEFINE_ENTRY(target_id, LARGE_INTEGER)
	DEFINE_ENTRY(item_uid, LARGE_INTEGER)
	DEFINE_ENTRY(item_id, LARGE_INTEGER)
	DEFINE_ENTRY(item_count, INT)
	DEFINE_ENTRY(action_type, INT)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogItemAccessor)
	//ENABLE_PARAM_MAP(CdboLogItemAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_sess_id)
		COLUMN_ENTRY(2, m_char_id)
		COLUMN_ENTRY(3, m_target_type)
		COLUMN_ENTRY(4, m_target_id)
		COLUMN_ENTRY(5, m_item_uid)
		COLUMN_ENTRY(6, m_item_id)
		COLUMN_ENTRY(7, m_item_count)
		COLUMN_ENTRY(8, m_action_type)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogItemAccessor, L"SELECT * FROM LogItem")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogItem : public Cdbo<CCommand<CAccessor<CdboLogItemAccessor> > >
{
public:

	HRESULT Log(PCSTRING pSessId, UInt32 char_id, UInt8 target_type, UInt32 target_id, UInt32 item_uid, UInt32 item_id, UInt16 item_count, UInt8 action_type)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		COLUMN_STRING_COPY(m_sess_id, pSessId)
		m_char_id.QuadPart = char_id;
		m_target_type = target_type;
		m_target_id.QuadPart = target_id;
		m_item_uid.QuadPart = item_uid;
		m_item_id.QuadPart = item_id;
		m_item_count = item_count;
		m_action_type = action_type;
		return OpenCommand(_T("INSERT INTO LogItem (sess_id, char_id, target_type, target_id, item_uid, item_id, item_count, action_type, create_time) VALUES (?, ?, ?, ?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_ITEM_H_
