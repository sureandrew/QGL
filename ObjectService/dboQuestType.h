// dboQuestType.H : Declaration of the CdboQuestType class

#pragma once
#ifndef __DBOQUESTTYPE_H_
#define __DBOQUESTTYPE_H_
#include "Dbo.h"

// code generated on Monday, November 5, 2007, 4:40 PM

class CdboQuestTypeAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboQuestTypeAccessor)

	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(type_id, LONG)
	DEFINE_ENTRY_STR(Name, 33)
	DEFINE_ENTRY(Round, LONG)
	DEFINE_ENTRY(MaxRound, LONG)
	DEFINE_ENTRY(TotalRound, LONG)
	DEFINE_ENTRY(LimitRound, LONG)
	DEFINE_ENTRY(LimitType, INT)
	DEFINE_ENTRY(LastClearTime, DBTIMESTAMP)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboQuestTypeAccessor)
	//ENABLE_PARAM_MAP(CdboQuestTypeAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_char_id)
		COLUMN_ENTRY_LS(2, m_type_id)
		COLUMN_ENTRY_LS(3, m_Name)
		COLUMN_ENTRY_LS(4, m_Round)
		COLUMN_ENTRY_LS(5, m_MaxRound)
		COLUMN_ENTRY_LS(6, m_TotalRound)
		COLUMN_ENTRY_LS(7, m_LimitRound)
		COLUMN_ENTRY_LS(8, m_LimitType)
		COLUMN_ENTRY_LS(9, m_LastClearTime)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByOwner)
		COLUMN_ENTRY(1, m_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByQuestID)
		COLUMN_ENTRY(1, m_char_id)
		COLUMN_ENTRY(2, m_type_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, char_id)
		COLUMN_ENTRY_TEST_ELSE(type_id)
		COLUMN_ENTRY_TEST_ELSE(Name)
		COLUMN_ENTRY_TEST_ELSE(Round)
		COLUMN_ENTRY_TEST_ELSE(MaxRound)
		COLUMN_ENTRY_TEST_ELSE(TotalRound)
		COLUMN_ENTRY_TEST_ELSE(LimitRound)
		COLUMN_ENTRY_TEST_ELSE(LimitType)
		COLUMN_ENTRY_TEST_ELSE(LastClearTime)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_char_id)
		COLUMN_ENTRY(2, m_type_id)
		COLUMN_ENTRY(3, m_Name)
		COLUMN_ENTRY(4, m_Round)
		COLUMN_ENTRY(5, m_MaxRound)
		COLUMN_ENTRY(6, m_TotalRound)
		COLUMN_ENTRY(7, m_LimitRound)
		COLUMN_ENTRY(8, m_LimitType)
		COLUMN_ENTRY(9, m_LastClearTime)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboQuestTypeAccessor, L"SELECT * FROM QuestType")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboQuestType : public Cdbo<CCommand<CAccessor<CdboQuestTypeAccessor> > >
{
public:
	HRESULT Insert()
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);

		return OpenCommand(_T("INSERT INTO QuestType (char_id, type_id, Name, Round, ")
			_T("MaxRound, TotalRound, LimitRound, LimitType, LastClearTime) ")
			_T("OUTPUT INSERTED.* VALUES (")
			_T("?, ?, ?, ?, ?, ?, ?, ?, ?)"));
	}

	HRESULT DeleteByOwner(DWORD cid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByOwner);
		m_char_id.QuadPart = cid;
		return OpenCommand(_T("DELETE FROM QuestType WHERE char_id = ?"));
	}

	HRESULT Delete(DWORD cid, LONG type_id)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByQuestID);
		m_char_id.QuadPart = cid;
		m_type_id	= type_id;
		return OpenCommand(_T("DELETE FROM QuestType WHERE char_id = ? AND type_id = ?"));
	}
 
	HRESULT RSByOwner(DWORD cid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByOwner);
		m_char_id.QuadPart = cid;
		return OpenCommand(_T("SELECT * FROM QuestType WHERE char_id = ?"));
	}

	HRESULT UpdateByAny(DWORD char_id, DWORD tid)
	{
		if (m_params.empty()) {
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("UpdateByAny Error (1) [empty paramater] char_id=%d, TypeId=%d"),
				m_char_id, m_type_id);
			return S_FALSE;
		}

		String sql(_T("UPDATE QuestType SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE char_id = ? AND type_id = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_char_id.QuadPart = char_id;
		m_type_id	= tid;
		m_params.push_back(_T("char_id"));
		m_params.push_back(_T("type_id"));
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}
};

#endif // __DBOQUESTTYPE_H_
