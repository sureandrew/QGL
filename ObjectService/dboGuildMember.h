// dboGuildMember.h : Declaration of the CdboGuildMember

#pragma once
#ifndef _DBO_GUILDMEMBER_H_
#define _DBO_GUILDMEMBER_H_
#include "Dbo.h"

// code generated on Thursday, March 22, 2007, 11:14 AM

class CdboGuildMemberAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboGuildMemberAccessor)

	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(guild_uid, LARGE_INTEGER)
	DEFINE_ENTRY(donate, LARGE_INTEGER)
	DEFINE_ENTRY(totalDonate, LARGE_INTEGER)
	DEFINE_ENTRY(post, INT)
	DEFINE_ENTRY(joinDate, DBTIMESTAMP)
	DEFINE_ENTRY(businessCount, LONG)
	DEFINE_ENTRY(businessProfit, LARGE_INTEGER)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider

	//ENABLE_COLUMN_MAP(CdboMessengerAccessor)
	//ENABLE_PARAM_MAP(CdboMessengerAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_char_id)
		COLUMN_ENTRY_LS(2, m_guild_uid)
		COLUMN_ENTRY_LS(3, m_donate)
		COLUMN_ENTRY_LS(4, m_totalDonate)
		COLUMN_ENTRY_LS(5, m_post)
		COLUMN_ENTRY_LS(6, m_joinDate)
		COLUMN_ENTRY_LS(7, m_businessCount)
		COLUMN_ENTRY_LS(8, m_businessProfit)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
	COLUMN_ENTRY_TEST(m_params, char_id)
		COLUMN_ENTRY_TEST_ELSE(guild_uid)
		COLUMN_ENTRY_TEST_ELSE(donate)
		COLUMN_ENTRY_TEST_ELSE(totalDonate)
		COLUMN_ENTRY_TEST_ELSE(post)
		COLUMN_ENTRY_TEST_ELSE(joinDate)
		COLUMN_ENTRY_TEST_ELSE(businessCount)
		COLUMN_ENTRY_TEST_ELSE(businessProfit)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_char_id)
		COLUMN_ENTRY(2, m_guild_uid)
		COLUMN_ENTRY(3, m_post)
		COLUMN_ENTRY(4, m_joinDate)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Delete)
		COLUMN_ENTRY(1, m_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_DeleteByGuild)
		COLUMN_ENTRY(1, m_guild_uid)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_GetByGuildID)
		COLUMN_ENTRY(1, m_guild_uid)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_GetByPlayer_ID)
		COLUMN_ENTRY(1, m_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_COLUMN_MAP_FUNC(_GetIdentity)
		COLUMN_ENTRY_LS(1, m_char_id)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetGuildID)
		COLUMN_ENTRY_LS(1, m_guild_uid)
	END_COLUMN_MAP()

	DEFINE_COMMAND_EX(CdboGuildMemberAccessor, L"SELECT * FROM GuildMember")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboGuildMember : public Cdbo<CCommand<CAccessor<CdboGuildMemberAccessor> > >
{
	/*
	friend_char_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(friend_relation, 50)
	DEFINE_ENTRY(friend_group, INT)
	DEFINE_ENTRY(friend_friendly, LONG)
	*/
public:
	HRESULT Insert(DWORD player_char_id, DWORD guild_uid, INT guildPost, DBTIMESTAMP time)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);
		m_char_id.QuadPart = player_char_id;
		m_guild_uid.QuadPart = guild_uid;
		m_post = guildPost;
		m_joinDate = time;
		
		return OpenCommand(_T("INSERT INTO GuildMember (char_id, guild_uid, post, joinDate) ")
			_T("OUTPUT INSERTED.* VALUES (?, ?, ?, ?)"));
	}

	HRESULT Delete(DWORD player_char_id)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Delete);
		m_char_id.QuadPart = player_char_id;

		// delete skill
		return OpenCommand(_T("DELETE FROM GuildMember WHERE char_id = ?"));
	}

	HRESULT DeleteByGuild(DWORD guild_uid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_DeleteByGuild);
		m_guild_uid.QuadPart = guild_uid;

		return OpenCommand(_T("DELETE FROM GuildMember WHERE guild_uid = ?"));
	}

	HRESULT RSByID(DWORD guild_uid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_GetByGuildID);
		m_guild_uid.QuadPart = guild_uid;
		
		//return OpenCommand(_T("SELECT * FROM Buff"));
		return OpenCommand(_T("SELECT * FROM GuildMember WHERE guild_uid = ?"));
	}

	HRESULT RSByCharID(DWORD char_id)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_GetByPlayer_ID);
		m_char_id.QuadPart = char_id;
		
		//return OpenCommand(_T("SELECT * FROM Buff"));
		return OpenCommand(_T("SELECT * FROM GuildMember WHERE char_id = ?"));
	}

	HRESULT GetGuildUid(DWORD char_id)
	{
		SetGetBindFunc(_GetGuildID);
		SetGetParamsFunc(_GetByPlayer_ID);
		m_char_id.QuadPart = char_id;
		
		//return OpenCommand(_T("SELECT * FROM Buff"));
		return OpenCommand(_T("SELECT guild_uid FROM GuildMember WHERE char_id = ?"));
	}

	HRESULT UpdateByAny(DWORD player_char_id)
	{
		if (m_params.empty()) return S_FALSE;

		String sql(_T("UPDATE GuildMember SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE  char_id = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_char_id.QuadPart = player_char_id;
		m_params.push_back(_T("char_id"));			
			
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s\n"), sql.c_str());

		return OpenCommand(sql.c_str());
	}

};

#endif // _DBO_ACCOUNT_H_
