// dboLogPartner.h : Declaration of the CdboLogPartner

#pragma once
#ifndef _DBO_LOG_PARTNER_H_
#define _DBO_LOG_PARTNER_H_
#include "Dbo.h"

// code generated on Tuesday, August 12, 2008, 18:21 AM

class CdboLogPartnerAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogPartnerAccessor)

	DEFINE_ENTRY_STR(sess_id, 13)
	DEFINE_ENTRY(partner_id, LARGE_INTEGER)
	DEFINE_ENTRY(mob_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)	
	DEFINE_ENTRY(process_type, INT)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogPartnerAccessor)
	//ENABLE_PARAM_MAP(CdboLogPartnerAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_sess_id)
		COLUMN_ENTRY(2, m_partner_id)
		COLUMN_ENTRY(3, m_mob_id)
		COLUMN_ENTRY(4, m_char_id)
		COLUMN_ENTRY(5, m_process_type)		
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogPartnerAccessor, L"SELECT * FROM LogPartner")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogPartner : public Cdbo<CCommand<CAccessor<CdboLogPartnerAccessor> > >
{
public:

	HRESULT Log(PCSTRING pSessId, UInt32 partner_id, UInt32 mob_id, UInt32 char_id, UInt8 process_type)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		COLUMN_STRING_COPY(m_sess_id, pSessId)
		m_partner_id.QuadPart = partner_id;
		m_mob_id.QuadPart = mob_id;
		m_char_id.QuadPart = char_id;
		m_process_type = process_type;		
		return OpenCommand(_T("INSERT INTO LogPartner (sess_id, partner_id, mob_id, char_id, process_type, create_time) VALUES (?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_PARTNER_H_
