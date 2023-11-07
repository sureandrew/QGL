// dboLogHonor.h : Declaration of the CdboLogHonor

#pragma once
#ifndef _DBO_LOG_HONOR_H_
#define _DBO_LOG_HONOR_H_
#include "Dbo.h"

class CdboLogHonorAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogHonorAccessor)

		DEFINE_ENTRY_STR(sess_id, 13)
		DEFINE_ENTRY(char_id, LARGE_INTEGER)
		DEFINE_ENTRY(change, INT)

		// In order to fix several issues with some providers, the code below may bind
		// columns in a different order than reported by the provider
		//ENABLE_COLUMN_MAP(CdboLogHonorAccessor)
		//ENABLE_PARAM_MAP(CdboLogHonorAccessor)

		BEGIN_PARAM_MAP_FUNC(_Insert)
			COLUMN_ENTRY(1, m_sess_id)
			COLUMN_ENTRY(2, m_char_id)
			COLUMN_ENTRY(3, m_change)
		END_PARAM_MAP_FUNC()

		DEFINE_COMMAND_EX(CdboLogHonorAccessor, L"SELECT * FROM LogHonor")

		// You may wish to call this function if you are inserting a record and wish to
		// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogHonor : public Cdbo<CCommand<CAccessor<CdboLogHonorAccessor> > >
{
public:

	HRESULT Log(PCSTRING pSessId, UInt32 char_id, Int32 change)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);

		COLUMN_STRING_COPY(m_sess_id, pSessId)
		m_char_id.QuadPart = char_id;
		m_change = change;

		return OpenCommand(_T("INSERT INTO LogHonor (sess_id, char_id, change, create_time) VALUES (?, ?, ?, GetDate());"));
	}
};

#endif // _DBO_LOG_HONOR_H_
