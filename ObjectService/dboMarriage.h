#pragma once
#ifndef __DBOMARRIAGE_H_
#define __DBOMARRIAGE_H_
#include "Dbo.h"

class CdboMarriageAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboMarriageAccessor)

		DEFINE_ENTRY(marriage_id, LARGE_INTEGER)
		DEFINE_ENTRY(husband_id, LARGE_INTEGER)
		DEFINE_ENTRY(wife_id, LARGE_INTEGER)
		DEFINE_ENTRY(marriage_time, DBTIMESTAMP)
		DEFINE_ENTRY(state, INT)
		DEFINE_ENTRY(count, INT)

		BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_marriage_id)
		COLUMN_ENTRY_LS(2, m_husband_id)
		COLUMN_ENTRY_LS(3, m_wife_id)
		COLUMN_ENTRY_LS(4, m_marriage_time)
		COLUMN_ENTRY_LS(5, m_state)
		END_COLUMN_MAP()	

		BEGIN_COLUMN_MAP_FUNC(_GetCount)
		COLUMN_ENTRY_LS(1, m_count)
		END_COLUMN_MAP()	

	BEGIN_PARAM_MAP_FUNC(_ParamInsert)
		COLUMN_ENTRY(1, m_husband_id)
		COLUMN_ENTRY(2, m_wife_id)
		COLUMN_ENTRY(3, m_marriage_time)
		COLUMN_ENTRY(4, m_state)

	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, marriage_id)
		COLUMN_ENTRY_TEST_ELSE(husband_id)
		COLUMN_ENTRY_TEST_ELSE(wife_id)
		COLUMN_ENTRY_TEST_ELSE(marriage_time)
		COLUMN_ENTRY_TEST_ELSE(state)
	COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByHusbandID)
		COLUMN_ENTRY(1, m_husband_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByWifeID)
	COLUMN_ENTRY(1, m_wife_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByMarriageID)
	COLUMN_ENTRY(1, m_marriage_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamCount)
	COLUMN_ENTRY(1, m_count)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByState)
	COLUMN_ENTRY(1, m_state)
	END_PARAM_MAP_FUNC()
	
	DEFINE_COMMAND_EX(CdboMarriageAccessor, L"SELECT * FROM Marriage")
};

class CdboMarriage : public Cdbo<CCommand<CAccessor<CdboMarriageAccessor> > >
{
public:
	HRESULT Delete(DWORD marriage_id)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CdboMarriage:Delete marriage_id %d"), marriage_id);
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByMarriageID);
		m_marriage_id.QuadPart			= marriage_id;
		return OpenCommand(_T("DELETE FROM Marriage WHERE marriage_id = ? "));
	}

	HRESULT Insert(DWORD husband_id, DWORD wife_id,DBTIMESTAMP marriage_time,WORD state )
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamInsert);
		m_husband_id.QuadPart	= husband_id;
		m_wife_id.QuadPart	= wife_id;
		m_marriage_time = marriage_time;
		m_state	= state;

		return OpenCommand(_T("INSERT INTO Marriage (husband_id,wife_id,marriage_time,state) OUTPUT INSERTED.* VALUES (?,?,?,?)"));
	}

	HRESULT RSByHusbandID(DWORD husband_id)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByHusbandID);
		m_husband_id.QuadPart			= husband_id;
		return OpenCommand(_T("SELECT * FROM Marriage WHERE husband_id = ?"));
	}

	HRESULT RSByWifeID(DWORD wife_id)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByWifeID);
		m_wife_id.QuadPart			= wife_id;
		return OpenCommand(_T("SELECT * FROM Marriage WHERE wife_id = ?"));
	}

	HRESULT GetCount()
	{
		SetGetBindFunc(_GetCount);
		SetGetParamsFunc(NULL);
		return OpenCommand(_T("SELECT  COUNT(*) FROM Marriage WHERE state = 1"));
	}

	HRESULT RSByState(WORD state)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByState);
		m_state	= state;
		return OpenCommand(_T("SELECT * FROM Marriage WHERE state = ?"));
	}

	HRESULT UpdateByAny(DWORD marriage_id)
	{
		if (m_params.empty())
			return S_FALSE;

		String sql(_T("UPDATE Marriage SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE marriage_id = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_marriage_id.QuadPart = marriage_id;
		m_params.push_back(_T("marriage_id"));		// add marriage_id at the end of params
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}
};

#endif // __DBOMARRIAGE_H_
