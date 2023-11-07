// dboCharacter.h : Declaration of the CdboMsgCharacter

#pragma once
#ifndef _DBO_MSGCHARACTER_H_
#define _DBO_MSGCHARACTER_H_
#include "Dbo.h"

// code generated on Thursday, March 22, 2007, 11:14 AM

class CdboMsgCharacterAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboMsgCharacterAccessor)

	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(nickName, 11)
	DEFINE_ENTRY(sex, INT)
	DEFINE_ENTRY(cclass, INT)
	DEFINE_ENTRY(faction, INT)

	DEFINE_ENTRY(level, LONG)
	
	DEFINE_ENTRY_STR(title, 21)

	DEFINE_ENTRY(photo_id, LONG)
	DEFINE_ENTRY(hairStyle1, LONG)
	DEFINE_ENTRY(hairStyle2, LONG)
	DEFINE_ENTRY(hairColor, LONG)

	DEFINE_ENTRY(eq_weapon, LARGE_INTEGER)
	DEFINE_ENTRY(eq_weaponColor, LONG)
	DEFINE_ENTRY(eq_headBand, LARGE_INTEGER)
	DEFINE_ENTRY(eq_headColor, LONG)
	DEFINE_ENTRY(eq_cloth, LARGE_INTEGER)
	DEFINE_ENTRY(eq_clothColor, LONG)

	DEFINE_ENTRY(org_hairStyle1, LONG)
	DEFINE_ENTRY(org_hairColor, LONG)
	DEFINE_ENTRY(org_eq_cloth, LARGE_INTEGER)
	DEFINE_ENTRY(org_eq_clothColor, LONG)
	DEFINE_ENTRY(friend_status, INT)

	DEFINE_ENTRY(friend_icon, INT)
	DEFINE_ENTRY_STR(friend_about, 81)
	DEFINE_ENTRY_STR(friend_onlineAutoReply, 81)
	DEFINE_ENTRY_STR(friend_offlineAutoReply, 81)

	DEFINE_ENTRY(speakCount, LONG)

	DEFINE_ENTRY(speakBlock, DBTIMESTAMP)
	DEFINE_ENTRY(offlineTime, DBTIMESTAMP)
	DEFINE_ENTRY(guild_uid, LARGE_INTEGER)
	DEFINE_ENTRY(money, LARGE_INTEGER)
	DEFINE_ENTRY(HistoryGenAccmulation,LARGE_INTEGER)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboMsgCharacterAccessor)
	//ENABLE_PARAM_MAP(CdboMsgCharacterAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		int i = 0;
		COLUMN_ENTRY_LS(++i, m_char_id)
		COLUMN_ENTRY_LS(++i, m_nickName)
		COLUMN_ENTRY_LS(++i, m_sex)
		COLUMN_ENTRY_LS(++i, m_cclass)
		COLUMN_ENTRY_LS(++i, m_faction)
		COLUMN_ENTRY_LS(++i, m_level)
		COLUMN_ENTRY_LS(++i, m_title)
		COLUMN_ENTRY_LS(++i, m_photo_id)
		COLUMN_ENTRY_LS(++i, m_hairStyle1)
		COLUMN_ENTRY_LS(++i, m_hairStyle2)
		COLUMN_ENTRY_LS(++i, m_hairColor)
		COLUMN_ENTRY_LS(++i, m_eq_weapon)
		COLUMN_ENTRY_LS(++i, m_eq_weaponColor)
		COLUMN_ENTRY_LS(++i, m_eq_headBand)
		COLUMN_ENTRY_LS(++i, m_eq_headColor)
		COLUMN_ENTRY_LS(++i, m_eq_cloth)
		COLUMN_ENTRY_LS(++i, m_eq_clothColor)
		COLUMN_ENTRY_LS(++i, m_org_hairStyle1)
		COLUMN_ENTRY_LS(++i, m_org_hairColor)
		COLUMN_ENTRY_LS(++i, m_org_eq_cloth)
		COLUMN_ENTRY_LS(++i, m_org_eq_clothColor)
		COLUMN_ENTRY_LS(++i, m_friend_status)
		COLUMN_ENTRY_LS(++i, m_friend_icon)
		COLUMN_ENTRY_LS(++i, m_friend_about)
		COLUMN_ENTRY_LS(++i, m_friend_onlineAutoReply)
		COLUMN_ENTRY_LS(++i, m_friend_offlineAutoReply)
		COLUMN_ENTRY_LS(++i, m_speakCount)
		COLUMN_ENTRY_LS(++i, m_speakBlock)
		COLUMN_ENTRY_LS(++i, m_offlineTime)
		COLUMN_ENTRY_LS(++i, m_guild_uid)
		COLUMN_ENTRY_LS(++i, m_money)
		COLUMN_ENTRY_LS(++i, m_HistoryGenAccmulation)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetIdName)
		COLUMN_ENTRY_LS(1, m_char_id)
		COLUMN_ENTRY_LS(2, m_nickName)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByName)
		COLUMN_ENTRY(1, m_nickName)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByID)
		COLUMN_ENTRY(1, m_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, char_id)
		COLUMN_ENTRY_TEST_ELSE(title)	
		COLUMN_ENTRY_TEST_ELSE(friend_status)	
		COLUMN_ENTRY_TEST_ELSE(friend_icon)
		COLUMN_ENTRY_TEST_ELSE(friend_about)
		COLUMN_ENTRY_TEST_ELSE(friend_onlineAutoReply)
		COLUMN_ENTRY_TEST_ELSE(friend_offlineAutoReply)
		COLUMN_ENTRY_TEST_ELSE(speakCount)
		COLUMN_ENTRY_TEST_ELSE(speakBlock)
		COLUMN_ENTRY_TEST_ELSE(offlineTime)
		COLUMN_ENTRY_TEST_ELSE(guild_uid)
		COLUMN_ENTRY_TEST_ELSE(money)

		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	#define MSG_CHAR_FIELDS							\
		FIELD_FIRST("", char_id)					\
		FIELD_NEXT("", nickName)					\
		FIELD_NEXT("", sex)							\
		FIELD_NEXT("", cclass)						\
		FIELD_NEXT("", faction)						\
		FIELD_NEXT("", level)						\
		FIELD_NEXT("", title)						\
		FIELD_NEXT("", photo_id)					\
		FIELD_NEXT("", hairStyle1)					\
		FIELD_NEXT("", hairStyle2)					\
		FIELD_NEXT("", hairColor)					\
		FIELD_NEXT("", eq_weapon)					\
		FIELD_NEXT("", eq_weaponColor)				\
		FIELD_NEXT("", eq_headBand)					\
		FIELD_NEXT("", eq_headColor)				\
		FIELD_NEXT("", eq_cloth)					\
		FIELD_NEXT("", eq_clothColor)				\
		FIELD_NEXT("", org_hairStyle1)				\
		FIELD_NEXT("", org_hairColor)				\
		FIELD_NEXT("", org_eq_cloth)				\
		FIELD_NEXT("", org_eq_clothColor)			\
		FIELD_NEXT("", friend_status)				\
		FIELD_NEXT("", friend_icon)					\
		FIELD_NEXT("", friend_about)				\
		FIELD_NEXT("", friend_onlineAutoReply)		\
		FIELD_NEXT("", friend_offlineAutoReply)		\
		FIELD_NEXT("", speakCount)					\
		FIELD_NEXT("", speakBlock)					\
		FIELD_NEXT("", offlineTime)					\
		FIELD_NEXT("", guild_uid)					\
		FIELD_NEXT("", money)						\
		FIELD_NEXT("", HistoryGenAccmulation)	

	DEFINE_COMMAND_EX(CdboMsgCharacterAccessor, 
		_T("SELECT ") MSG_CHAR_FIELDS _T(" FROM MsgCharView"))

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboMsgCharacter : public Cdbo<CCommand<CAccessor<CdboMsgCharacterAccessor> > >
{
public:

	HRESULT RSByID(DWORD cid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByID);
		m_char_id.QuadPart = cid;
		return OpenCommand(_T("SELECT ") MSG_CHAR_FIELDS _T(" FROM MsgCharView WHERE char_id = ?"));
	}

	HRESULT RSByIdStr(LPCTSTR idStr)
	{
		String sql;

		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(NULL);
		sql.Format(_T("SELECT ") MSG_CHAR_FIELDS _T(" FROM MsgCharView WHERE char_id IN (%s)"), 
			idStr);
		
		return OpenCommand(sql.c_str());
	}

	HRESULT InactiveIdStr(LPCTSTR idStr)
	{
		String sql;

		SetGetBindFunc(_GetIdName);
		SetGetParamsFunc(NULL);
		sql.Format(_T("SELECT char_id, nickName FROM InactiveCharacter WHERE char_id IN (%s)"), 
			idStr);
		
		return OpenCommand(sql.c_str());
	}

	HRESULT UpdateByAny(DWORD cid)
	{
		if (m_params.empty()) {
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("UpdateByAny Error: empty paramater for char_id %d"),
				cid);
			return S_FALSE;
		}

		String sql(_T("UPDATE MsgCharView SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE char_id = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_char_id.QuadPart = cid;
		m_params.push_back(_T("char_id"));		// add char_id at the end of params
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}

};

#endif // _DBO_CHARACTER_H_
