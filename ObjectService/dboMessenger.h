// dboAccount.h : Declaration of the CdboAccount

#pragma once
#ifndef _DBO_MESSENGER_H_
#define _DBO_MESSENGER_H_
#include "Dbo.h"

// code generated on Thursday, March 22, 2007, 11:14 AM

class CdboMessengerAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboMessengerAccessor)

	DEFINE_ENTRY(player_char_id, LARGE_INTEGER)
	DEFINE_ENTRY(friend_char_id, LARGE_INTEGER)
	DEFINE_ENTRY(friend_relation, INT)
	DEFINE_ENTRY(friend_group, INT)
	DEFINE_ENTRY(friend_friendly, LONG)
	DEFINE_ENTRY(canupdate, INT)
	DEFINE_ENTRY(relation, INT)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider

	//ENABLE_COLUMN_MAP(CdboMessengerAccessor)
	//ENABLE_PARAM_MAP(CdboMessengerAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_player_char_id)
		COLUMN_ENTRY_LS(2, m_friend_char_id)
		COLUMN_ENTRY_LS(3, m_friend_relation)
		COLUMN_ENTRY_LS(4, m_friend_group)
		COLUMN_ENTRY_LS(5, m_friend_friendly)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
	COLUMN_ENTRY_TEST(m_params, player_char_id)
		COLUMN_ENTRY_TEST_ELSE(friend_char_id)
		COLUMN_ENTRY_TEST_ELSE(friend_relation)
		COLUMN_ENTRY_TEST_ELSE(friend_group)
		COLUMN_ENTRY_TEST_ELSE(friend_friendly)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_player_char_id)
		COLUMN_ENTRY(2, m_friend_char_id)
		COLUMN_ENTRY(3, m_friend_relation)
		COLUMN_ENTRY(4, m_friend_group)
		COLUMN_ENTRY(5, m_friend_friendly)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Delete)
		COLUMN_ENTRY(1, m_player_char_id)
		COLUMN_ENTRY(2, m_friend_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_DeleteByOwner)
		COLUMN_ENTRY(1, m_player_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_GetByID)
		COLUMN_ENTRY(1, m_player_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_GetByFID)
		COLUMN_ENTRY(1, m_player_char_id)
		COLUMN_ENTRY(2, m_friend_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_COLUMN_MAP_FUNC(_GetIdentity)
		COLUMN_ENTRY_LS(1, m_player_char_id)
		COLUMN_ENTRY_LS(2, m_friend_char_id)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetRemoveRelation)
		COLUMN_ENTRY_LS(1, m_friend_relation)
		COLUMN_ENTRY_LS(2, m_canupdate)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_RemoveRelation)
		COLUMN_ENTRY(1, m_player_char_id)
		COLUMN_ENTRY(2, m_friend_char_id)
		COLUMN_ENTRY(3, m_relation)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboMessengerAccessor, L"SELECT * FROM Buff")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboMessenger : public Cdbo<CCommand<CAccessor<CdboMessengerAccessor> > >
{
	/*
	friend_char_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(friend_relation, 50)
	DEFINE_ENTRY(friend_group, INT)
	DEFINE_ENTRY(friend_friendly, LONG)
	*/
public:
	HRESULT Insert(DWORD player_char_id, DWORD friend_char_id, UInt16 friend_relation, INT friend_group, LONG friend_friendly)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		m_player_char_id.QuadPart = player_char_id;
		m_friend_char_id.QuadPart = friend_char_id;
		m_friend_relation = friend_relation;
		m_friend_group = friend_group;
		m_friend_friendly = friend_friendly;
		return OpenCommand(_T("INSERT INTO Messenger (player_char_id, friend_char_id, friend_relation, friend_group, friend_friendly) VALUES (?, ?, ?, ?, ?)"));
	}

	HRESULT Delete(DWORD player_char_id, DWORD friend_char_id)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Delete);
		m_player_char_id.QuadPart = player_char_id;
		m_friend_char_id.QuadPart = friend_char_id;

		// delete skill
		return OpenCommand(_T("DELETE FROM Messenger WHERE player_char_id = ? AND friend_char_id = ?"));
	}

	HRESULT DeleteByOwner(DWORD player_char_id)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_DeleteByOwner);
		m_player_char_id.QuadPart = player_char_id;
		// delete all skills by owner
		HRESULT hr = OpenCommand(_T("DELETE FROM Messenger WHERE player_char_id = ?"));

		if (hr != S_OK)
			return hr;

		return OpenCommand(_T("DELETE FROM Messenger WHERE friend_char_id = ?"));
	}

	HRESULT RSByID(DWORD player_char_id)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_GetByID);
		m_player_char_id.QuadPart = player_char_id;
		
		//return OpenCommand(_T("SELECT * FROM Buff"));
		return OpenCommand(_T("SELECT * FROM Messenger AS M WHERE M.player_char_id = ?"));
	}

	HRESULT RSByFID(DWORD player_char_id, DWORD friend_char_id)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_GetByFID);
		m_player_char_id.QuadPart = player_char_id;
		m_friend_char_id.QuadPart = friend_char_id;
		
		//return OpenCommand(_T("SELECT * FROM Buff"));
		return OpenCommand(_T("SELECT * FROM Messenger AS M WHERE M.player_char_id = ? and M.friend_char_id = ?"));
	}

	HRESULT UpdateByAny(DWORD player_char_id, DWORD friend_char_id)
	{
		if (m_params.empty()) return S_FALSE;

		String sql(_T("UPDATE Messenger SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE  player_char_id = ? AND friend_char_id = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_player_char_id.QuadPart = player_char_id;
		m_friend_char_id.QuadPart = friend_char_id;
		m_params.push_back(_T("player_char_id"));		
		m_params.push_back(_T("friend_char_id"));		
			
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s\n"), sql.c_str());

		return OpenCommand(sql.c_str());
	}

	HRESULT RemoveRelation(UInt32 owner_id, UInt32 target_id,int relation)
	{
		SetGetBindFunc(_GetRemoveRelation);
		SetGetParamsFunc(_RemoveRelation);
		m_player_char_id.QuadPart = owner_id;
		m_friend_char_id.QuadPart = target_id;
		m_relation = 1<<relation;

//#ifdef DB_MYSQL
//		SetGetBindFunc(NULL);
//		// some error reading problems for store procedure in mysql
//		//return OpenCommand(_T("CALL sp_UpdateYuanBao(?, ?);"));
//		return OpenCommand(_T("UPDATE Messenger SET friend_relation = friend_relation& ~? WHERE acct_id = ?;"));
//#else
		return OpenCommand(_T("EXEC sp_RemoveRelation ?, ?, ?;"));
//#endif // DB_MYSQL
	}

};

#endif // _DBO_ACCOUNT_H_
