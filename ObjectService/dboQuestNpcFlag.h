
#pragma once
#ifndef __DBOQUESTNPCFLAG_H_
#define __DBOQUESTNPCFLAG_H_
#include "Dbo.h"

class CdboQuestNpcFlagAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboQuestNpcFlagAccessor)

	DEFINE_ENTRY(npc_id, LARGE_INTEGER)
	DEFINE_ENTRY(line_id, LONG)
	DEFINE_ENTRY(owner_id, LARGE_INTEGER)
	DEFINE_ENTRY(flag_id, LONG)
	DEFINE_ENTRY(flag, LONG)


	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_npc_id)
		COLUMN_ENTRY_LS(2, m_line_id)
		COLUMN_ENTRY_LS(3, m_owner_id)
		COLUMN_ENTRY_LS(4, m_flag_id)
		COLUMN_ENTRY_LS(5, m_flag)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByLineId)
		COLUMN_ENTRY(1, m_line_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Delete)
	COLUMN_ENTRY(1, m_npc_id)
	COLUMN_ENTRY(2, m_line_id)
	COLUMN_ENTRY(3, m_owner_id)
	COLUMN_ENTRY(4, m_flag_id)
	END_PARAM_MAP_FUNC()


	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, npc_id)
		COLUMN_ENTRY_TEST_ELSE(line_id)
		COLUMN_ENTRY_TEST_ELSE(owner_id)
		COLUMN_ENTRY_TEST_ELSE(flag_id)
		COLUMN_ENTRY_TEST_ELSE(flag)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_npc_id)
		COLUMN_ENTRY(2, m_line_id)
		COLUMN_ENTRY(3, m_owner_id)
		COLUMN_ENTRY(4, m_flag_id)
		COLUMN_ENTRY(5, m_flag)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboQuestNpcFlagAccessor, L"SELECT * FROM QuestNpcFlag")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboQuestNpcFlag : public Cdbo<CCommand<CAccessor<CdboQuestNpcFlagAccessor> > >
{
public:

	HRESULT Insert(DWORD npc_id,DWORD line_id,DWORD owner_id,DWORD flag_id,DWORD flag)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);
		m_npc_id.QuadPart = npc_id;
		m_line_id = line_id;
		m_owner_id.QuadPart = owner_id;
		m_flag_id = flag_id;
		m_flag = flag;
		return OpenCommand(_T("INSERT INTO QuestNpcFlag (npc_id, line_id, owner_id, flag_id, flag) OUTPUT INSERTED.* VALUES (?, ?, ?, ?, ?)"));
	}

	HRESULT Delete(DWORD npc_id,DWORD line_id,DWORD owner_id,DWORD flag_id)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Delete);
		m_npc_id.QuadPart = npc_id;
		m_line_id = line_id;
		m_owner_id.QuadPart = owner_id;
		m_flag_id = flag_id;
		return OpenCommand(_T("DELETE FROM QuestNpcFlag WHERE npc_id = ? and line_id = ? and owner_id = ? and flag_id = ?"));
	}


	HRESULT RSByLineId(DWORD line_id)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByLineId);
		m_line_id = line_id;
		return OpenCommand(_T("SELECT * FROM QuestNpcFlag WHERE line_id = ?"));
	}

	HRESULT UpdateByAny(DWORD npc_id,DWORD line_id,DWORD owner_id,DWORD flag_id)
	{
		if (m_params.empty()) 
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("UpdateByAny Error (1) [empty paramater] npc_id=%d, flag_id=%d"),
				m_npc_id, m_flag_id);
			return S_FALSE;
		}

		String sql(_T("UPDATE QuestNpcFlag SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE npc_id = ? AND line_id = ? AND owner_id = ? AND flag_id = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_npc_id.QuadPart = npc_id;
		m_line_id = line_id;
		m_owner_id.QuadPart = owner_id;
		m_flag_id	= flag_id;
		m_params.push_back(_T("npc_id"));
		m_params.push_back(_T("line_id"));
		m_params.push_back(_T("owner_id"));
		m_params.push_back(_T("flag_id"));
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}
};

#endif // __DBOQUESTNPCFLAG_H_
