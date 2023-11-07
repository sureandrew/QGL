// dboQuestFlag.H : Declaration of the CdboQuestFlag class

#pragma once
#ifndef __DBOQUESTFLAG_H_
#define __DBOQUESTFLAG_H_
#include "Dbo.h"

// code generated on Friday, November 30, 2007, 5:40 PM

class CdboQuestFlagAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboQuestFlagAccessor)

	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(flag_id, LONG)
	DEFINE_ENTRY(flag, LONG)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboQuestFlagAccessor)
	//ENABLE_PARAM_MAP(CdboQuestFlagAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_char_id)
		COLUMN_ENTRY_LS(2, m_flag_id)
		COLUMN_ENTRY_LS(3, m_flag)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByOwner)
		COLUMN_ENTRY(1, m_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByFlagID)
		COLUMN_ENTRY(1, m_char_id)
		COLUMN_ENTRY(2, m_flag_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, char_id)
		COLUMN_ENTRY_TEST_ELSE(flag_id)
		COLUMN_ENTRY_TEST_ELSE(flag)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_char_id)
		COLUMN_ENTRY(2, m_flag_id)
		COLUMN_ENTRY(3, m_flag)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboQuestFlagAccessor, L"SELECT * FROM QuestFlag")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboQuestFlag : public Cdbo<CCommand<CAccessor<CdboQuestFlagAccessor> > >
{
public:
	HRESULT Insert()
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);
		return OpenCommand(_T("INSERT INTO QuestFlag (char_id, flag_id, flag) ")
			_T("OUTPUT INSERTED.* VALUES (?, ?, ?)"));
	}

	HRESULT DeleteByOwner(DWORD cid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByOwner);
		m_char_id.QuadPart = cid;
		return OpenCommand(_T("DELETE FROM QuestFlag WHERE char_id = ?"));
	}

	HRESULT Delete(DWORD cid, LONG fid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByFlagID);
		m_char_id.QuadPart = cid;
		m_flag_id = fid;
		return OpenCommand(_T("DELETE FROM QuestFlag WHERE char_id = ? AND flag_id = ?"));
	}

	HRESULT RSByOwner(DWORD cid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByOwner);
		m_char_id.QuadPart = cid;
		return OpenCommand(_T("SELECT * FROM QuestFlag WHERE char_id = ?"));
	}

	HRESULT UpdateByAny(DWORD char_id, DWORD flag_id)
	{
		if (m_params.empty()) {
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("UpdateByAny Error (1) [empty paramater] char_id=%d, flag_id=%d"),
				m_char_id, m_flag_id);
			return S_FALSE;
		}

		String sql(_T("UPDATE QuestFlag SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE char_id = ? AND flag_id = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_char_id.QuadPart = char_id;
		m_flag_id	= flag_id;
		m_params.push_back(_T("char_id"));
		m_params.push_back(_T("flag_id"));
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}
};

#endif // __DBOQUESTFLAG_H_
