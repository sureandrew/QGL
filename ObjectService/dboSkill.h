// dboAccount.h : Declaration of the CdboAccount

#pragma once
#ifndef _DBO_SKILL_H_
#define _DBO_SKILL_H_
#include "Dbo.h"

// code generated on Thursday, March 22, 2007, 11:14 AM

class CdboSkillAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboSkillAccessor)

	DEFINE_ENTRY(skill_id, LONG)
	DEFINE_ENTRY(owner_id, LARGE_INTEGER)
	DEFINE_ENTRY(owner_type, INT)
	DEFINE_ENTRY(skill_lv, LONG)
	DEFINE_ENTRY(cool_down, LARGE_INTEGER)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboSkillAccessor)
	//ENABLE_PARAM_MAP(CdboSkillAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_skill_id)
		COLUMN_ENTRY_LS(2, m_owner_id)
		COLUMN_ENTRY_LS(3, m_owner_type)
		COLUMN_ENTRY_LS(4, m_skill_lv)
		COLUMN_ENTRY_LS(5, m_cool_down)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
	COLUMN_ENTRY_TEST(m_params, skill_id)
		COLUMN_ENTRY_TEST_ELSE(owner_id)
		COLUMN_ENTRY_TEST_ELSE(owner_type)
		COLUMN_ENTRY_TEST_ELSE(skill_lv)
		COLUMN_ENTRY_TEST_ELSE(cool_down)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_skill_id)
		COLUMN_ENTRY(2, m_owner_id)
		COLUMN_ENTRY(3, m_owner_type)
		COLUMN_ENTRY(4, m_skill_lv)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Delete)
		COLUMN_ENTRY(1, m_skill_id)
		COLUMN_ENTRY(2, m_owner_id)
		COLUMN_ENTRY(3, m_owner_type)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_DeleteByOwner)
		COLUMN_ENTRY(1, m_owner_id)
		COLUMN_ENTRY(2, m_owner_type)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_GetByID)
		COLUMN_ENTRY(1, m_owner_id)
		COLUMN_ENTRY(2, m_owner_type)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_GetByIdentity)
		COLUMN_ENTRY(1, m_skill_id)
		COLUMN_ENTRY(2, m_owner_id)
		COLUMN_ENTRY(3, m_owner_type)
	END_PARAM_MAP_FUNC()

	BEGIN_COLUMN_MAP_FUNC(_GetIdentity)
		COLUMN_ENTRY_LS(1, m_skill_id)
		COLUMN_ENTRY_LS(2, m_owner_id)
		COLUMN_ENTRY_LS(3, m_owner_type)
	END_COLUMN_MAP()

	DEFINE_COMMAND_EX(CdboSkillAccessor, L"SELECT * FROM Skill")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboSkill : public Cdbo<CCommand<CAccessor<CdboSkillAccessor> > >
{
public:
	HRESULT Insert(LONG skill_id, DWORD owner_id, INT owner_type, LONG skill_lv)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);
		m_skill_id = skill_id;
		m_owner_id.QuadPart = owner_id;
		m_owner_type = owner_type;
		m_skill_lv = skill_lv;
		return OpenCommand(_T("INSERT INTO Skill (skill_id, owner_id, owner_type, skill_lv) OUTPUT INSERTED.* VALUES (?, ?, ?, ?)"));
	}

	HRESULT Delete(LONG skill_id, DWORD owner_id, INT owner_type)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Delete);
		m_skill_id = skill_id;
		m_owner_id.QuadPart = owner_id;
		m_owner_type = owner_type;
		// delete skill
		return OpenCommand(_T("DELETE FROM Skill WHERE skill_id = ? AND owner_id = ? AND owner_type = ?"));
	}

	HRESULT DeleteByOwner(DWORD owner_id, INT owner_type)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_DeleteByOwner);
		m_owner_id.QuadPart = owner_id;
		m_owner_type = owner_type;
		// delete all skills by owner
		return OpenCommand(_T("DELETE FROM Skill WHERE owner_id = ? AND owner_type = ?"));
	}

	HRESULT RSByID(DWORD owner_id, INT owner_type)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_GetByID);
		m_owner_id.QuadPart = owner_id;
		m_owner_type = owner_type;
		//return OpenCommand
		return OpenCommand(_T("SELECT * FROM Skill WHERE owner_id = ? AND owner_type = ?"));
	}

	HRESULT UpdateByAny(LONG skill_id, DWORD owner_id, INT owner_type)
	{
		if (m_params.empty()) return S_FALSE;

		String sql(_T("UPDATE Skill SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE  skill_id = ? AND owner_id = ? AND owner_type = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_skill_id = skill_id;
		m_owner_id.QuadPart = owner_id;
		m_owner_type = owner_type;
		m_params.push_back(_T("skill_id"));		
		m_params.push_back(_T("owner_id"));		
		m_params.push_back(_T("owner_type"));		
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}


};

#endif // _DBO_ACCOUNT_H_
