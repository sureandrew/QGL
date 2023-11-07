// dboLogYuanBao.h : Declaration of the CdboLogYuanBao

#pragma once
#ifndef _DBO_LOG_YUANBAO_H_
#define _DBO_LOG_YUANBAO_H_
#include "Dbo.h"


class CdboLogYuanBaoAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogYuanBaoAccessor)

	DEFINE_ENTRY_STR(sess_id, 13)
	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(useplace, INT)
	DEFINE_ENTRY(yuanbao, INT)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogYuanBaoAccessor)
	//ENABLE_PARAM_MAP(CdboLogYuanBaoAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_sess_id)
		COLUMN_ENTRY(2, m_acct_id)
		COLUMN_ENTRY(3, m_char_id)
		COLUMN_ENTRY(4, m_useplace)
		COLUMN_ENTRY(5, m_yuanbao)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogYuanBaoAccessor, L"SELECT * FROM LogYuanBao")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogYuanBao : public Cdbo<CCommand<CAccessor<CdboLogYuanBaoAccessor> > >
{
public:

	HRESULT Log(PCSTRING pSessId, UInt32 acct_id, UInt32 char_id, UInt8 useplace, UInt32 yuanbao)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		
		COLUMN_STRING_COPY(m_sess_id, pSessId)
		m_acct_id.QuadPart = acct_id;
		m_char_id.QuadPart = char_id;
		m_useplace = useplace;
		m_yuanbao = yuanbao;

		return OpenCommand(_T("INSERT INTO LogYuanBao (sess_id, acct_id, char_id, useplace, yuanbao, create_time) VALUES (?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_YUANBAO_H_
