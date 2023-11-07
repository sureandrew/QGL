// dboServiceStatus.h : Declaration of the CdboServiceStatus

#pragma once
#ifndef _DBO_SERVICE_STATUS_H_
#define _DBO_SERVICE_STATUS_H_
#include "Dbo.h"

// code generated on Tuesday, August 12, 2008, 18:21 AM

class CdboServiceStatusAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboServiceStatusAccessor)

		DEFINE_ENTRY(service_id, INT)
		DEFINE_ENTRY(status, INT)
		DEFINE_ENTRY(state, INT)

		// In order to fix several issues with some providers, the code below may bind
		// columns in a different order than reported by the provider
		//ENABLE_COLUMN_MAP(CdboServiceStatusAccessor)
		//ENABLE_PARAM_MAP(CdboServiceStatusAccessor)

		BEGIN_COLUMN_MAP_FUNC(_Select)
			COLUMN_ENTRY(1, m_service_id)
			COLUMN_ENTRY(2, m_status)
			COLUMN_ENTRY(3, m_state)
		END_COLUMN_MAP_FUNC()

		BEGIN_PARAM_MAP_FUNC(_Params)
			COLUMN_ENTRY(1, m_status)
		END_PARAM_MAP_FUNC()

		DEFINE_COMMAND_EX(CdboServiceStatusAccessor, L"SELECT * FROM ServiceStatus")

		// You may wish to call this function if you are inserting a record and wish to
		// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboServiceStatus : public Cdbo<CCommand<CAccessor<CdboServiceStatusAccessor> > >
{
public:

	HRESULT Select(VOID)
	{
		SetGetBindFunc(_Select);
		SetGetParamsFunc(_Params);
		m_status = SS_CONNECTION_COUNT;
		return OpenCommand(_T("SELECT service_id, status, state FROM ServiceStatus WHERE status = ?;"));
	}
};

#endif // _DBO_SERVICE_STATUS_H_
