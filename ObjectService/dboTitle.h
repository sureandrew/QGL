// dboAccount.h : Declaration of the CdboAccount

#pragma once
#ifndef _DBO_TITLE_H_
#define _DBO_TITLE_H_
#include "Dbo.h"

// code generated on Thursday, March 22, 2007, 11:14 AM

class CdboTitleAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboTitleAccessor)

	DEFINE_ENTRY(title_uid, LARGE_INTEGER)
	DEFINE_ENTRY(owner_id, LARGE_INTEGER)
	DEFINE_ENTRY(title_id, INT)
	DEFINE_ENTRY(expireDate, DBTIMESTAMP)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider

	//ENABLE_COLUMN_MAP(CdboTitleAccessor)
	//ENABLE_PARAM_MAP(CdboTitleAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_title_uid)
		COLUMN_ENTRY_LS(2, m_owner_id)
		COLUMN_ENTRY_LS(3, m_title_id)
		COLUMN_ENTRY_LS(4, m_expireDate)
		
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
	COLUMN_ENTRY_TEST(m_params, title_uid)
		COLUMN_ENTRY_TEST_ELSE(owner_id)
		COLUMN_ENTRY_TEST_ELSE(title_id)
		COLUMN_ENTRY_TEST_ELSE(expireDate)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_owner_id)
		COLUMN_ENTRY(2, m_title_id)
		COLUMN_ENTRY(3, m_expireDate)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Delete)
		COLUMN_ENTRY(1, m_title_uid)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_DeleteByOwner)
		COLUMN_ENTRY(1, m_owner_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_GetByID)
		COLUMN_ENTRY(1, m_title_uid)
	END_PARAM_MAP_FUNC()

	BEGIN_COLUMN_MAP_FUNC(_GetIdentity)
		COLUMN_ENTRY_LS(1, m_title_uid)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByOID)
		COLUMN_ENTRY(1, m_owner_id)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboTitleAccessor, L"SELECT * FROM Title")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboTitle : public Cdbo<CCommand<CAccessor<CdboTitleAccessor> > >
{
	/*
	friend_char_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(friend_relation, 50)
	DEFINE_ENTRY(friend_group, INT)
	DEFINE_ENTRY(friend_friendly, LONG)
	*/
public:
	HRESULT Insert(DWORD owner_id, DWORD title_id)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);
		
		m_owner_id.QuadPart = owner_id;
		m_title_id = title_id;

		return OpenCommand(_T("INSERT INTO Title (owner_id, title_id, expireDate) ")
			_T("OUTPUT INSERTED.* VALUES (?, ?, ?)"));
	}

	HRESULT Delete(DWORD title_uid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Delete);
		m_title_uid.QuadPart = title_uid;

		// delete skill
		return OpenCommand(_T("DELETE FROM Title WHERE title_uid = ?"));
	}

	HRESULT DeleteByOwner(DWORD player_char_id)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_DeleteByOwner);
		m_owner_id.QuadPart = player_char_id;
		// delete all skills by owner
		return OpenCommand(_T("DELETE FROM Title WHERE owner_id = ?"));
	}

	HRESULT RSByID(DWORD title_uid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_GetByID);
		m_title_uid.QuadPart = title_uid;
		
		//return OpenCommand(_T("SELECT * FROM Buff"));
		return OpenCommand(_T("SELECT * FROM Title AS M WHERE M.title_uid = ?"));
	}

	HRESULT RSByOwnerID(DWORD oid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByOID);
		m_owner_id.QuadPart = oid;
		return OpenCommand(_T("SELECT * FROM Title AS M WHERE M.owner_id = ?"));
	}

	HRESULT UpdateByAny(DWORD title_uid)
	{
		if (m_params.empty()) return S_FALSE;

		String sql(_T("UPDATE Title SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE  title_uid = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_title_uid.QuadPart = title_uid;
		m_params.push_back(_T("title_uid"));		
			
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s\n"), sql.c_str());

		return OpenCommand(sql.c_str());
	}


};

#endif // _DBO_ACCOUNT_H_
