// dboGuild.h : Declaration of the CdboGuild

#pragma once
#ifndef _DBO_GUILD_H_
#define _DBO_GUILD_H_
#include "Dbo.h"

// code generated on Thursday, March 22, 2007, 11:14 AM

class CdboGuildAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboGuildAccessor)

	DEFINE_ENTRY(guild_uid, LARGE_INTEGER)
	DEFINE_ENTRY_STR(name, 21)
	DEFINE_ENTRY_STR(creator, 21)
	DEFINE_ENTRY_STR(master, 21)
	DEFINE_ENTRY(alliance, LARGE_INTEGER)
	DEFINE_ENTRY(enemy, LARGE_INTEGER)
	DEFINE_ENTRY_STR(secMaster, 21)
	DEFINE_ENTRY(level, LONG)
	DEFINE_ENTRY(prosper, LARGE_INTEGER)
	DEFINE_ENTRY(maxProsper, LONG)
	DEFINE_ENTRY(fund, LARGE_INTEGER)
	DEFINE_ENTRY(totalMember, LONG)
	DEFINE_ENTRY(maxMember, LONG)
	DEFINE_ENTRY_STR(aim, 101)
	DEFINE_ENTRY_STR(rule, 301)
	DEFINE_ENTRY(guildPvPScore, LARGE_INTEGER)
	DEFINE_ENTRY(guildPvPBid, LARGE_INTEGER)
	DEFINE_ENTRY(guildPvPGroup, INT)
	DEFINE_ENTRY_STR(groupPvPVSGuild, 21)
	DEFINE_ENTRY(manorScale, INT)
	DEFINE_ENTRY(manorRoom, INT)
	DEFINE_ENTRY(manorMaterial, LARGE_INTEGER)
	DEFINE_ENTRY(manorGuard, INT)	
	DEFINE_ENTRY(manorTrainPlayerExPow, INT)
	DEFINE_ENTRY(manorTrainPlayerInPow, INT)
	DEFINE_ENTRY(manorTrainPartnerExPow, INT)	
	DEFINE_ENTRY(manorTrainPartnerInPow, INT)
	DEFINE_ENTRY(manorRoomDrugLv, INT)	
	DEFINE_ENTRY(manorRoomCookLv, INT)	
	DEFINE_ENTRY(manorRoomWineLv, INT)	
	DEFINE_ENTRY(manorRoomHPLv, INT)
	DEFINE_ENTRY(manorRoomSPLv, INT)	
	DEFINE_ENTRY(manorMaintainNum, LARGE_INTEGER)
	DEFINE_ENTRY(lastMaintainDate, DBTIMESTAMP)	
	DEFINE_ENTRY(manorAura, INT)
	DEFINE_ENTRY(offlinetime, DBTIMESTAMP)		
	DEFINE_ENTRY(clearBusinessDate, DBTIMESTAMP)
	DEFINE_ENTRY(autoManageBusiness, INT)
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider

	//ENABLE_COLUMN_MAP(CdboMessengerAccessor)
	//ENABLE_PARAM_MAP(CdboMessengerAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS( 1, m_guild_uid)
		COLUMN_ENTRY_LS( 2, m_name)
		COLUMN_ENTRY_LS( 3, m_creator)
		COLUMN_ENTRY_LS( 4, m_master)
		COLUMN_ENTRY_LS( 5, m_alliance)
		COLUMN_ENTRY_LS( 6, m_enemy)
		COLUMN_ENTRY_LS( 7, m_secMaster)
		COLUMN_ENTRY_LS( 8, m_level)
		COLUMN_ENTRY_LS( 9, m_prosper)
		COLUMN_ENTRY_LS(10, m_maxProsper)
		COLUMN_ENTRY_LS(11, m_fund)
		COLUMN_ENTRY_LS(12, m_totalMember)
		COLUMN_ENTRY_LS(13, m_maxMember)
		COLUMN_ENTRY_LS(14, m_aim)
		COLUMN_ENTRY_LS(15, m_rule)
		COLUMN_ENTRY_LS(16, m_guildPvPScore)
		COLUMN_ENTRY_LS(17, m_guildPvPBid)
		COLUMN_ENTRY_LS(18, m_guildPvPGroup)
		COLUMN_ENTRY_LS(19, m_groupPvPVSGuild)
		COLUMN_ENTRY_LS(20, m_manorScale)
		COLUMN_ENTRY_LS(21, m_manorRoom)
		COLUMN_ENTRY_LS(22, m_manorMaterial)
		COLUMN_ENTRY_LS(23, m_manorGuard)
		COLUMN_ENTRY_LS(24, m_manorTrainPlayerExPow)
		COLUMN_ENTRY_LS(25, m_manorTrainPlayerInPow)
		COLUMN_ENTRY_LS(26, m_manorTrainPartnerExPow)
		COLUMN_ENTRY_LS(27, m_manorTrainPartnerInPow)
		COLUMN_ENTRY_LS(28, m_manorRoomDrugLv)
		COLUMN_ENTRY_LS(29, m_manorRoomCookLv)
		COLUMN_ENTRY_LS(30, m_manorRoomWineLv)
		COLUMN_ENTRY_LS(31, m_manorRoomHPLv)
		COLUMN_ENTRY_LS(32, m_manorRoomSPLv)
		COLUMN_ENTRY_LS(33, m_manorMaintainNum)
		COLUMN_ENTRY_LS(34, m_lastMaintainDate)
		COLUMN_ENTRY_LS(35, m_manorAura)
		COLUMN_ENTRY_LS(36, m_offlinetime)	
		COLUMN_ENTRY_LS(37, m_clearBusinessDate)
		COLUMN_ENTRY_LS(38, m_autoManageBusiness)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetRank)
		int i = 0;
		COLUMN_ENTRY_LS(++i, m_guild_uid)
		COLUMN_ENTRY_LS(++i, m_name)
		COLUMN_ENTRY_LS(++i, m_master)
		COLUMN_ENTRY_LS(++i, m_fund)
		COLUMN_ENTRY_LS(++i, m_totalMember)
		COLUMN_ENTRY_LS(++i, m_maxMember)
		COLUMN_ENTRY_LS(++i, m_guildPvPScore)
		COLUMN_ENTRY_LS(++i, m_guildPvPBid)		
		COLUMN_ENTRY_LS(++i, m_guildPvPGroup)


	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
	COLUMN_ENTRY_TEST(m_params, guild_uid)
		COLUMN_ENTRY_TEST_ELSE(name)
		COLUMN_ENTRY_TEST_ELSE(creator)
		COLUMN_ENTRY_TEST_ELSE(master)
		COLUMN_ENTRY_TEST_ELSE(alliance)
		COLUMN_ENTRY_TEST_ELSE(enemy)
		COLUMN_ENTRY_TEST_ELSE(secMaster)
		COLUMN_ENTRY_TEST_ELSE(level)
		COLUMN_ENTRY_TEST_ELSE(prosper)
		COLUMN_ENTRY_TEST_ELSE(maxProsper)
		COLUMN_ENTRY_TEST_ELSE(fund)
		COLUMN_ENTRY_TEST_ELSE(totalMember)
		COLUMN_ENTRY_TEST_ELSE(maxMember)
		COLUMN_ENTRY_TEST_ELSE(aim)
		COLUMN_ENTRY_TEST_ELSE(rule)
		COLUMN_ENTRY_TEST_ELSE(guildPvPScore)
		COLUMN_ENTRY_TEST_ELSE(guildPvPBid)
		COLUMN_ENTRY_TEST_ELSE(guildPvPGroup)
		COLUMN_ENTRY_TEST_ELSE(groupPvPVSGuild)
		COLUMN_ENTRY_TEST_ELSE(manorScale)
		COLUMN_ENTRY_TEST_ELSE(manorRoom)
		COLUMN_ENTRY_TEST_ELSE(manorMaterial)
		COLUMN_ENTRY_TEST_ELSE(manorGuard)
		COLUMN_ENTRY_TEST_ELSE(manorTrainPlayerExPow)
		COLUMN_ENTRY_TEST_ELSE(manorTrainPlayerInPow)
		COLUMN_ENTRY_TEST_ELSE(manorTrainPartnerExPow)
		COLUMN_ENTRY_TEST_ELSE(manorTrainPartnerInPow)
		COLUMN_ENTRY_TEST_ELSE(manorRoomDrugLv)
		COLUMN_ENTRY_TEST_ELSE(manorRoomCookLv)
		COLUMN_ENTRY_TEST_ELSE(manorRoomWineLv)
		COLUMN_ENTRY_TEST_ELSE(manorRoomHPLv)
		COLUMN_ENTRY_TEST_ELSE(manorRoomSPLv)
		COLUMN_ENTRY_TEST_ELSE(manorMaintainNum)
		COLUMN_ENTRY_TEST_ELSE(lastMaintainDate)
		COLUMN_ENTRY_TEST_ELSE(manorAura)	
		COLUMN_ENTRY_TEST_ELSE(offlinetime)
		COLUMN_ENTRY_TEST_ELSE(clearBusinessDate)
		COLUMN_ENTRY_TEST_ELSE(autoManageBusiness)

		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_name)
		COLUMN_ENTRY(2, m_creator)
		COLUMN_ENTRY(3, m_master)
		COLUMN_ENTRY(4, m_totalMember)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Delete)
		COLUMN_ENTRY(1, m_guild_uid)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_GetByID)
		COLUMN_ENTRY(1, m_guild_uid)
	END_PARAM_MAP_FUNC()

	BEGIN_COLUMN_MAP_FUNC(_GetIdentity)
		COLUMN_ENTRY_LS(1, m_guild_uid)
	END_COLUMN_MAP()

	DEFINE_COMMAND_EX(CdboGuildAccessor, L"SELECT * FROM Guild")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboGuild : public Cdbo<CCommand<CAccessor<CdboGuildAccessor> > >
{
	/*
	friend_char_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(friend_relation, 50)
	DEFINE_ENTRY(friend_group, INT)
	DEFINE_ENTRY(friend_friendly, LONG)
	*/
public:
	HRESULT Insert(LPCTSTR guild_name, LPCTSTR guild_creator, LPCTSTR guild_master, int totalMember)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);
		COLUMN_STRING_COPY(m_name, guild_name);
		COLUMN_STRING_COPY(m_creator, guild_creator);
		COLUMN_STRING_COPY(m_master, guild_master);
		m_totalMember = totalMember;
		
		return OpenCommand(_T("INSERT INTO Guild (name, creator, \"master\", totalMember) ")
			_T("OUTPUT INSERTED.* VALUES (?, ?, ?, ?)"));
	}

	HRESULT Delete(DWORD guild_uid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Delete);
		m_guild_uid.QuadPart = guild_uid;

		// delete skill
		return OpenCommand(_T("DELETE FROM Guild WHERE guild_uid = ?"));
	}


	HRESULT RSByID(DWORD guild_uid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_GetByID);
		m_guild_uid.QuadPart = guild_uid;
		
		//return OpenCommand(_T("SELECT * FROM Buff"));
		return OpenCommand(_T("SELECT * FROM Guild AS M WHERE M.guild_uid = ?"));
	}

	HRESULT RSALL()
	{
		SetGetBindFunc(_GetAll);
		
		//return OpenCommand(_T("SELECT * FROM Buff"));
		return OpenCommand(_T("SELECT * FROM Guild"));
	}

	HRESULT UpdateByAny(DWORD Guild_uid)
	{
		if (m_params.empty()) return S_FALSE;

		String sql(_T("UPDATE Guild SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE guild_uid = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_guild_uid.QuadPart = Guild_uid;
		m_params.push_back(_T("guild_uid"));		
			
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s\n"), sql.c_str());

		return OpenCommand(sql.c_str());
	}

	HRESULT ResetBid()
	{
		String sql(_T("UPDATE Guild SET guildPvPBid = 0"));
		return OpenCommand(sql.c_str());
	}

	HRESULT ResetGroup()
	{
		String sql(_T("UPDATE Guild SET guildPvPGroup = 0"));
		return OpenCommand(sql.c_str());
	}

	HRESULT RSByGuildPvP()
	{
		SetGetBindFunc(_GetAll);

			String sql;
#ifdef DB_MYSQL
		sql = _T("SELECT * FROM Guild WHERE guildPvPGroup > 0 LIMIT 16 ORDER BY guildPvPGroup DESC");
#else
		sql = _T("SELECT TOP 16 * FROM Guild WHERE guildPvPGroup > 0 ORDER BY guildPvPGroup DESC");
#endif // MYSQL

		return OpenCommand(sql.c_str());
	}
	
	HRESULT RSTopCase(LPCTSTR whereStr, int top)
	{
		SetGetBindFunc(_GetRank);
		SetGetParamsFunc(NULL);
		
		String sql;
#ifdef DB_MYSQL
		sql.Format(_T("SELECT guild_uid, name, master, fund, totalMember, maxMember, guildPvPScore, guildPvPBid, guildPvPGroup ")			        
					_T("FROM Guild WHERE %s LIMIT %d"), whereStr, top);
#else
		sql.Format(_T("SELECT TOP %d guild_uid,  name, master, fund, totalMember, maxMember, guildPvPScore, guildPvPBid, guildPvPGroup ")
					_T("FROM Guild WHERE %s"), top, whereStr);
#endif // MYSQL

		return OpenCommand(sql.c_str());
	}

};

#endif // _DBO_GUILD_H_
