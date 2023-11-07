// dboAccount.h : Declaration of the CdboAccount

#pragma once
#ifndef _DBO_BUFF_H_
#define _DBO_BUFF_H_
#include "Dbo.h"

// code generated on Thursday, March 22, 2007, 11:14 AM

class CdboBuffAccessor
{
public:

	static StringVector m_params;
	DEFINE_ENTRY(buff_id, LONG)
	DEFINE_ENTRY(owner_id, LARGE_INTEGER)
	DEFINE_ENTRY(owner_type, BYTE)
	DEFINE_ENTRY(buff_value, LONG)
	DEFINE_ENTRY(cool_down, LARGE_INTEGER)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider

	ENABLE_COLUMN_MAP(CdboBuffAccessor)
	ENABLE_PARAM_MAP(CdboBuffAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_buff_id)
		COLUMN_ENTRY_LS(2, m_owner_id)
		COLUMN_ENTRY_LS(3, m_owner_type)
		COLUMN_ENTRY_LS(4, m_buff_value)
		COLUMN_ENTRY_LS(5, m_cool_down)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
	COLUMN_ENTRY_TEST(m_params, buff_id)
		COLUMN_ENTRY_TEST_ELSE(owner_id)
		COLUMN_ENTRY_TEST_ELSE(owner_type)
		COLUMN_ENTRY_TEST_ELSE(buff_value)
		COLUMN_ENTRY_TEST_ELSE(cool_down)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_buff_id)
		COLUMN_ENTRY(2, m_owner_id)
		COLUMN_ENTRY(3, m_owner_type)
		COLUMN_ENTRY(4, m_buff_value)
		COLUMN_ENTRY(5, m_cool_down)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Delete)
		COLUMN_ENTRY(1, m_buff_id)
		COLUMN_ENTRY(2, m_owner_id)
		COLUMN_ENTRY(3, m_owner_type)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_GetByID)
		COLUMN_ENTRY(1, m_owner_id)
		COLUMN_ENTRY(2, m_owner_type)
	END_PARAM_MAP_FUNC()

	BEGIN_COLUMN_MAP_FUNC(_GetIdentity)
		COLUMN_ENTRY_LS(1, m_buff_id)
		COLUMN_ENTRY_LS(2, m_owner_id)
		COLUMN_ENTRY_LS(3, m_owner_type)
	END_COLUMN_MAP()

	DEFINE_COMMAND_EX(CdboBuffAccessor, L"SELECT * FROM Buff")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
	INLINE CdboBuffAccessor()
	{
		ClearRecord();
	}

	~CdboBuffAccessor()
	{
		ClearParam();
	}

	void ClearRecord()
	{
		memset(this, 0, sizeof(*this));
	}

	INLINE void AddParam(const String &param)
	{
		if (std::find(m_params.begin(), m_params.end(), param) == m_params.end())
			m_params.push_back(param);
	}
	
	INLINE void ClearParam()
	{
		m_params.clear();
	}
};

class CdboBuff : public Cdbo<CCommand<CAccessor<CdboBuffAccessor> > >
{
public:
	HRESULT Insert(LONG buff_id, DWORD owner_id, BYTE owner_type, LONG buff_value, DWORD CoolDown)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		m_buff_id = buff_id;
		m_owner_id.QuadPart = owner_id;
		m_owner_type = owner_type;
		m_buff_value = buff_value;
		m_cool_down.QuadPart = CoolDown;
		return OpenCommand(_T("INSERT INTO Buff (buff_id, owner_id, owner_type, buff_value, cool_down) VALUES (?, ?, ?, ?, ?)"));
	}

	HRESULT Delete(LONG buff_id, DWORD owner_id, BYTE owner_type)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Delete);
		m_buff_id = buff_id;
		m_owner_id.QuadPart = owner_id;
		m_owner_type = owner_type;
		// delete skill
		return OpenCommand(_T("DELETE FROM Buff WHERE buff_id = ? AND owner_id = ? AND owner_type = ?"));
	}

	HRESULT RSByID(DWORD owner_id, BYTE owner_type)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_GetByID);
		m_owner_id.QuadPart = owner_id;
		m_owner_type = owner_type;
		//return OpenCommand(_T("SELECT * FROM Buff"));
		return OpenCommand(_T("SELECT * FROM Buff AS B WHERE B.owner_id = ? AND B.owner_type = ?"));
	}

	HRESULT UpdateByAny(LONG buff_id, DWORD owner_id, BYTE owner_type)
	{
		if (m_params.empty()) return S_FALSE;

		String sql(_T("UPDATE Skill SET "));

		sql += m_params.front() + _T(" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); it++)
		{
			String &param = *it;
			sql += _T(", ") + param + _T(" = ?");
		}
		sql += _T(" WHERE  buff_id = ? AND owner_id = ? AND owner_type = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_buff_id = buff_id;
		m_owner_id.QuadPart = owner_id;
		m_owner_type = owner_type;
		m_params.push_back(_T("buff_id"));		
		m_params.push_back(_T("owner_id"));		
		m_params.push_back(_T("owner_type"));		
		TRACE_INFODTL_1(GLOBAL_LOGGER, _T("UpdateByAny SQL: %s\n"), sql.c_str());

		return OpenCommand(sql.c_str());

		/*SetGetBindFunc(_GetIdentity);
		SetGetParamsFunc(_UpdateLv);
		m_skill_id = skill_id;
		m_owner_id.LowPart = owner_id;
		m_owner_type = owner_type;
		m_skill_lv = skill_lv;
		return OpenCommand(_T("UPDATE Skill AS S SET(skill_lv = ?) WHERE  S.skill_id = ? AND S.owner_id = ? AND S.owner_type = ?"));
		*/
	}


};

#endif // _DBO_ACCOUNT_H_
