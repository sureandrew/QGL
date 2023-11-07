// dboQuestLog.H : Declaration of the CdboQuestLog class

#pragma once
#ifndef __DBOQUESTLOG_H_
#define __DBOQUESTLOG_H_
#include "Dbo.h"

// code generated on Monday, November 5, 2007, 4:40 PM

class CdboQuestLogAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboQuestLogAccessor)

	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(quest_id, LONG)
	DEFINE_ENTRY(type_id, LONG)
	DEFINE_ENTRY_STR(name,33)
	DEFINE_ENTRY_STR(descript, 801)
	DEFINE_ENTRY(beginNpc, LARGE_INTEGER)
	DEFINE_ENTRY(beginNpc_Map, LONG)
	DEFINE_ENTRY(beginNpc_x, LONG)
	DEFINE_ENTRY(beginNpc_y, LONG)
	DEFINE_ENTRY(finishNpc, LARGE_INTEGER)
	DEFINE_ENTRY(finishNpc_Map, LONG)
	DEFINE_ENTRY(finishNpc_x, LONG)
	DEFINE_ENTRY(finishNpc_y, LONG)
	DEFINE_ENTRY(target1_id, LARGE_INTEGER)
	DEFINE_ENTRY(target1_v1, LONG)
	DEFINE_ENTRY(target1_v2, LONG)
	DEFINE_ENTRY(target1_v3, INT)
	DEFINE_ENTRY(target1_vtype, INT)
	DEFINE_ENTRY(target1_num, LONG)
	DEFINE_ENTRY(target1_max, LONG)
	DEFINE_ENTRY(target2_id, LARGE_INTEGER)
	DEFINE_ENTRY(target2_v1, LONG)
	DEFINE_ENTRY(target2_v2, LONG)
	DEFINE_ENTRY(target2_v3, INT)
	DEFINE_ENTRY(target2_vtype, INT)
	DEFINE_ENTRY(target2_num, LONG)
	DEFINE_ENTRY(target2_max, LONG)
	DEFINE_ENTRY(target3_id, LARGE_INTEGER)
	DEFINE_ENTRY(target3_v1, LONG)
	DEFINE_ENTRY(target3_v2, LONG)
	DEFINE_ENTRY(target3_v3, INT)
	DEFINE_ENTRY(target3_vtype, INT)
	DEFINE_ENTRY(target3_num, LONG)
	DEFINE_ENTRY(target3_max, LONG)
	DEFINE_ENTRY(target4_id, LARGE_INTEGER)
	DEFINE_ENTRY(target4_v1, LONG)
	DEFINE_ENTRY(target4_v2, LONG)
	DEFINE_ENTRY(target4_v3, INT)
	DEFINE_ENTRY(target4_vtype, INT)
	DEFINE_ENTRY(target4_num, LONG)
	DEFINE_ENTRY(target4_max, LONG)
	DEFINE_ENTRY(target5_id, LARGE_INTEGER)
	DEFINE_ENTRY(target5_v1, LONG)
	DEFINE_ENTRY(target5_v2, LONG)
	DEFINE_ENTRY(target5_v3, INT)
	DEFINE_ENTRY(target5_vtype, INT)
	DEFINE_ENTRY(target5_num, LONG)
	DEFINE_ENTRY(target5_max, LONG)
	DEFINE_ENTRY(timeLimit, DBTIMESTAMP)
	DEFINE_ENTRY(interval, LONG)
	DEFINE_ENTRY(buff, LONG)
	DEFINE_ENTRY(state, INT)
	DEFINE_ENTRY(feature, LONG)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboQuestLogAccessor)
	//ENABLE_PARAM_MAP(CdboQuestLogAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS( 1, m_char_id)
		COLUMN_ENTRY_LS( 2, m_quest_id)
		COLUMN_ENTRY_LS( 3, m_type_id)
		COLUMN_ENTRY_LS( 4, m_name)
		COLUMN_ENTRY_LS( 5, m_descript)
		COLUMN_ENTRY_LS( 6, m_beginNpc)
		COLUMN_ENTRY_LS( 7, m_beginNpc_Map)
		COLUMN_ENTRY_LS( 8, m_beginNpc_x)
		COLUMN_ENTRY_LS( 9, m_beginNpc_y)
		COLUMN_ENTRY_LS(10, m_finishNpc)
		COLUMN_ENTRY_LS(11, m_finishNpc_Map)
		COLUMN_ENTRY_LS(12, m_finishNpc_x)
		COLUMN_ENTRY_LS(13, m_finishNpc_y)
		COLUMN_ENTRY_LS(14, m_target1_id)
		COLUMN_ENTRY_LS(15, m_target1_v1)
		COLUMN_ENTRY_LS(16, m_target1_v2)
		COLUMN_ENTRY_LS(17, m_target1_v3)
		COLUMN_ENTRY_LS(18, m_target1_vtype)
		COLUMN_ENTRY_LS(19, m_target1_num)
		COLUMN_ENTRY_LS(20, m_target1_max)
		COLUMN_ENTRY_LS(21, m_target2_id)
		COLUMN_ENTRY_LS(22, m_target2_v1)
		COLUMN_ENTRY_LS(23, m_target2_v2)
		COLUMN_ENTRY_LS(24, m_target2_v3)
		COLUMN_ENTRY_LS(25, m_target2_vtype)
		COLUMN_ENTRY_LS(26, m_target2_num)
		COLUMN_ENTRY_LS(27, m_target2_max)
		COLUMN_ENTRY_LS(28, m_target3_id)
		COLUMN_ENTRY_LS(29, m_target3_v1)
		COLUMN_ENTRY_LS(30, m_target3_v2)
		COLUMN_ENTRY_LS(31, m_target3_v3)
		COLUMN_ENTRY_LS(32, m_target3_vtype)
		COLUMN_ENTRY_LS(33, m_target3_num)
		COLUMN_ENTRY_LS(34, m_target3_max)
		COLUMN_ENTRY_LS(35, m_target4_id)
		COLUMN_ENTRY_LS(36, m_target4_v1)
		COLUMN_ENTRY_LS(37, m_target4_v2)
		COLUMN_ENTRY_LS(38, m_target4_v3)
		COLUMN_ENTRY_LS(39, m_target4_vtype)
		COLUMN_ENTRY_LS(40, m_target4_num)
		COLUMN_ENTRY_LS(41, m_target4_max)
		COLUMN_ENTRY_LS(42, m_target5_id)
		COLUMN_ENTRY_LS(43, m_target5_v1)
		COLUMN_ENTRY_LS(44, m_target5_v2)
		COLUMN_ENTRY_LS(45, m_target5_v3)
		COLUMN_ENTRY_LS(46, m_target5_vtype)
		COLUMN_ENTRY_LS(47, m_target5_num)
		COLUMN_ENTRY_LS(48, m_target5_max)
		COLUMN_ENTRY_LS(49, m_timeLimit)
		COLUMN_ENTRY_LS(50, m_interval)
		COLUMN_ENTRY_LS(51, m_buff)
		COLUMN_ENTRY_LS(52, m_state)
		COLUMN_ENTRY_LS(53, m_feature)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByOwner)
		COLUMN_ENTRY(1, m_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByQuestID)
		COLUMN_ENTRY(1, m_char_id)
		COLUMN_ENTRY(2, m_quest_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, char_id)
		COLUMN_ENTRY_TEST_ELSE(quest_id)
		COLUMN_ENTRY_TEST_ELSE(type_id)
		COLUMN_ENTRY_TEST_ELSE(name)
		COLUMN_ENTRY_TEST_ELSE(descript)
		COLUMN_ENTRY_TEST_ELSE(beginNpc)
		COLUMN_ENTRY_TEST_ELSE(beginNpc_Map)
		COLUMN_ENTRY_TEST_ELSE(beginNpc_x)
		COLUMN_ENTRY_TEST_ELSE(beginNpc_y)
		COLUMN_ENTRY_TEST_ELSE(finishNpc)
		COLUMN_ENTRY_TEST_ELSE(finishNpc_Map)
		COLUMN_ENTRY_TEST_ELSE(finishNpc_x)
		COLUMN_ENTRY_TEST_ELSE(finishNpc_y)
		COLUMN_ENTRY_TEST_ELSE(target1_id)
		COLUMN_ENTRY_TEST_ELSE(target1_v1)
		COLUMN_ENTRY_TEST_ELSE(target1_v2)
		COLUMN_ENTRY_TEST_ELSE(target1_v3)
		COLUMN_ENTRY_TEST_ELSE(target1_vtype)
		COLUMN_ENTRY_TEST_ELSE(target1_num)
		COLUMN_ENTRY_TEST_ELSE(target1_max)
		COLUMN_ENTRY_TEST_ELSE(target2_id)
		COLUMN_ENTRY_TEST_ELSE(target2_v1)
		COLUMN_ENTRY_TEST_ELSE(target2_v2)
		COLUMN_ENTRY_TEST_ELSE(target2_v3)
		COLUMN_ENTRY_TEST_ELSE(target2_vtype)
		COLUMN_ENTRY_TEST_ELSE(target2_num)
		COLUMN_ENTRY_TEST_ELSE(target2_max)
		COLUMN_ENTRY_TEST_ELSE(target3_id)
		COLUMN_ENTRY_TEST_ELSE(target3_v1)
		COLUMN_ENTRY_TEST_ELSE(target3_v2)
		COLUMN_ENTRY_TEST_ELSE(target3_v3)
		COLUMN_ENTRY_TEST_ELSE(target3_vtype)
		COLUMN_ENTRY_TEST_ELSE(target3_num)
		COLUMN_ENTRY_TEST_ELSE(target3_max)
		COLUMN_ENTRY_TEST_ELSE(target4_id)
		COLUMN_ENTRY_TEST_ELSE(target4_v1)
		COLUMN_ENTRY_TEST_ELSE(target4_v2)
		COLUMN_ENTRY_TEST_ELSE(target4_v3)
		COLUMN_ENTRY_TEST_ELSE(target4_vtype)
		COLUMN_ENTRY_TEST_ELSE(target4_num)
		COLUMN_ENTRY_TEST_ELSE(target4_max)
		COLUMN_ENTRY_TEST_ELSE(target5_id)
		COLUMN_ENTRY_TEST_ELSE(target5_v1)
		COLUMN_ENTRY_TEST_ELSE(target5_v2)
		COLUMN_ENTRY_TEST_ELSE(target5_v3)
		COLUMN_ENTRY_TEST_ELSE(target5_vtype)
		COLUMN_ENTRY_TEST_ELSE(target5_num)
		COLUMN_ENTRY_TEST_ELSE(target5_max)
		COLUMN_ENTRY_TEST_ELSE(timeLimit)
		COLUMN_ENTRY_TEST_ELSE(interval)
		COLUMN_ENTRY_TEST_ELSE(buff)
		COLUMN_ENTRY_TEST_ELSE(state)
		COLUMN_ENTRY_TEST_ELSE(feature)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY( 1, m_char_id)
		COLUMN_ENTRY( 2, m_quest_id)
		COLUMN_ENTRY( 3, m_type_id)
		COLUMN_ENTRY( 4, m_name)
		COLUMN_ENTRY( 5, m_descript)
		COLUMN_ENTRY( 6, m_beginNpc)
		COLUMN_ENTRY( 7, m_beginNpc_Map)
		COLUMN_ENTRY( 8, m_beginNpc_x)
		COLUMN_ENTRY( 9, m_beginNpc_y)
		COLUMN_ENTRY(10, m_finishNpc)
		COLUMN_ENTRY(11, m_finishNpc_Map)
		COLUMN_ENTRY(12, m_finishNpc_x)
		COLUMN_ENTRY(13, m_finishNpc_y)
		COLUMN_ENTRY(14, m_target1_id)
		COLUMN_ENTRY(15, m_target1_v1)
		COLUMN_ENTRY(16, m_target1_v2)
		COLUMN_ENTRY(17, m_target1_v3)
		COLUMN_ENTRY(18, m_target1_vtype)
		COLUMN_ENTRY(19, m_target1_num)
		COLUMN_ENTRY(20, m_target1_max)
		COLUMN_ENTRY(21, m_target2_id)
		COLUMN_ENTRY(22, m_target2_v1)
		COLUMN_ENTRY(23, m_target2_v2)
		COLUMN_ENTRY(24, m_target2_v3)
		COLUMN_ENTRY(25, m_target2_vtype)
		COLUMN_ENTRY(26, m_target2_num)
		COLUMN_ENTRY(27, m_target2_max)
		COLUMN_ENTRY(28, m_target3_id)
		COLUMN_ENTRY(29, m_target3_v1)
		COLUMN_ENTRY(30, m_target3_v2)
		COLUMN_ENTRY(31, m_target3_v3)
		COLUMN_ENTRY(32, m_target3_vtype)
		COLUMN_ENTRY(33, m_target3_num)
		COLUMN_ENTRY(34, m_target3_max)
		COLUMN_ENTRY(35, m_target4_id)
		COLUMN_ENTRY(36, m_target4_v1)
		COLUMN_ENTRY(37, m_target4_v2)
		COLUMN_ENTRY(38, m_target4_v3)
		COLUMN_ENTRY(39, m_target4_vtype)
		COLUMN_ENTRY(40, m_target4_num)
		COLUMN_ENTRY(41, m_target4_max)
		COLUMN_ENTRY(42, m_target5_id)
		COLUMN_ENTRY(43, m_target5_v1)
		COLUMN_ENTRY(44, m_target5_v2)
		COLUMN_ENTRY(45, m_target5_v3)
		COLUMN_ENTRY(46, m_target5_vtype)
		COLUMN_ENTRY(47, m_target5_num)
		COLUMN_ENTRY(48, m_target5_max)
		COLUMN_ENTRY(49, m_timeLimit)
		COLUMN_ENTRY(50, m_interval)
		COLUMN_ENTRY(51, m_buff)
		COLUMN_ENTRY(52, m_state)
		COLUMN_ENTRY(53, m_feature)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboQuestLogAccessor, L"SELECT * FROM QuestLog")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboQuestLog : public Cdbo<CCommand<CAccessor<CdboQuestLogAccessor> > >
{
public:
	HRESULT Insert()
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);

		return OpenCommand(_T("INSERT INTO QuestLog (")
			_T("char_id, quest_id, type_id, \"name\", descript, beginNpc, beginNpc_Map, beginNpc_x, beginNpc_y,")
			_T("finishNpc, finishNpc_Map, finishNpc_x, finishNpc_y, ")
			_T("target1_id, target1_v1, target1_v2, target1_v3, target1_vtype, target1_num, target1_max, ")
			_T("target2_id, target2_v1, target2_v2, target2_v3, target2_vtype, target2_num, target2_max, ")
			_T("target3_id, target3_v1, target3_v2, target3_v3, target3_vtype, target3_num, target3_max, ")
			_T("target4_id, target4_v1, target4_v2, target4_v3, target4_vtype, target4_num, target4_max, ")
			_T("target5_id, target5_v1, target5_v2, target5_v3, target5_vtype, target5_num, target5_max, ")
			_T("timeLimit, \"interval\", buff, state, feature")
			_T(") OUTPUT INSERTED.* VALUES (")
			_T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?,")
			_T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?,")
			_T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?,")
			_T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?,")
			_T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?,")
			_T("?, ?, ?)"));	
	}

	HRESULT DeleteByOwner(DWORD cid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByOwner);
		m_char_id.QuadPart = cid;
		return OpenCommand(_T("DELETE FROM QuestLog WHERE char_id = ?"));
	}

	HRESULT Delete(DWORD cid, LONG quest_id)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByQuestID);
		m_char_id.QuadPart = cid;
		m_quest_id		= quest_id;
		return OpenCommand(_T("DELETE FROM QuestLog WHERE char_id = ? AND quest_id = ?"));
	}

	HRESULT RSByOwner(DWORD cid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByOwner);
		m_char_id.QuadPart = cid;
		return OpenCommand(_T("SELECT * FROM QuestLog WHERE char_id = ?"));
	}

	HRESULT UpdateByAny(DWORD char_id, DWORD quest_id)
	{
		if (m_params.empty())
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("UpdateByAny Error (1) [empty paramater] char_id=%d, Id=%d"),
				char_id, quest_id);
			return S_FALSE;
		}

		String sql(_T("UPDATE QuestLog SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE char_id = ? AND quest_id = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_char_id.QuadPart = char_id;
		m_quest_id		= quest_id;
		m_params.push_back(_T("char_id"));
		m_params.push_back(_T("quest_id"));
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}
};

#endif // _DBO_CHARACTER_H_
