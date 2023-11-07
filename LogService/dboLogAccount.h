// dboLogAccount.h : Declaration of the CdboLogAccount

#pragma once
#ifndef _DBO_LOG_ACCOUNT_H_
#define _DBO_LOG_ACCOUNT_H_
#include "Dbo.h"

// code generated on Tuesday, August 12, 2008, 18:21 AM

class CdboLogAccountAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogAccountAccessor)

	DEFINE_ENTRY_STR(sess_id, 13)
	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(ip, 16)
	DEFINE_ENTRY(action_type, INT)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogAccountAccessor)
	//ENABLE_PARAM_MAP(CdboLogAccountAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_sess_id)
		COLUMN_ENTRY(2, m_acct_id)
		COLUMN_ENTRY(3, m_ip)
		COLUMN_ENTRY(4, m_action_type)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogAccountAccessor, L"SELECT * FROM LogAccount")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogAccount : public Cdbo<CCommand<CAccessor<CdboLogAccountAccessor> > >
{
public:

	HRESULT Log(PCSTRING pSessId, UInt32 acct_id, PCSTRING pIp, UInt8 action_type)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		COLUMN_STRING_COPY(m_sess_id, pSessId)
		m_acct_id.QuadPart = acct_id;
		COLUMN_STRING_COPY(m_ip, pIp);
		m_action_type = action_type;
		return OpenCommand(_T("INSERT INTO LogAccount (sess_id, acct_id, ip, action_type, create_time) VALUES (?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_ACCOUNT_H_
