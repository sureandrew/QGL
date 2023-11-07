#pragma once
#ifndef __DBOSIBLING_H_
#define __DBOSIBLING_H_
#include "Dbo.h"

class CdboSiblingAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboSiblingAccessor)

		DEFINE_ENTRY(sibling_id, LARGE_INTEGER)
		DEFINE_ENTRY_STR(sibling_title_prefix, 21)
		DEFINE_ENTRY(member_id1, LARGE_INTEGER)
		DEFINE_ENTRY_STR(member_title_suffix1, 21)
		DEFINE_ENTRY(member_id2, LARGE_INTEGER)
		DEFINE_ENTRY_STR(member_title_suffix2, 21)
		DEFINE_ENTRY(member_id3, LARGE_INTEGER)
		DEFINE_ENTRY_STR(member_title_suffix3, 21)
		DEFINE_ENTRY(member_id4, LARGE_INTEGER)
		DEFINE_ENTRY_STR(member_title_suffix4, 21)
		DEFINE_ENTRY(member_id5, LARGE_INTEGER)
		DEFINE_ENTRY_STR(member_title_suffix5, 21)
		DEFINE_ENTRY(member_id6, LARGE_INTEGER)
		DEFINE_ENTRY_STR(member_title_suffix6, 21)
		DEFINE_ENTRY(member_id7, LARGE_INTEGER)
		DEFINE_ENTRY_STR(member_title_suffix7, 21)
		DEFINE_ENTRY(member_id8, LARGE_INTEGER)
		DEFINE_ENTRY_STR(member_title_suffix8, 21)
		DEFINE_ENTRY(member_id9, LARGE_INTEGER)
		DEFINE_ENTRY_STR(member_title_suffix9, 21)
		DEFINE_ENTRY(member_id10, LARGE_INTEGER)
		DEFINE_ENTRY_STR(member_title_suffix10, 21)


		BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_sibling_id)
		COLUMN_ENTRY_LS(2, m_sibling_title_prefix)
		COLUMN_ENTRY_LS(3, m_member_id1)
		COLUMN_ENTRY_LS(4, m_member_title_suffix1)
		COLUMN_ENTRY_LS(5, m_member_id2)
		COLUMN_ENTRY_LS(6, m_member_title_suffix2)
		COLUMN_ENTRY_LS(7, m_member_id3)
		COLUMN_ENTRY_LS(8, m_member_title_suffix3)
		COLUMN_ENTRY_LS(9, m_member_id4)
		COLUMN_ENTRY_LS(10, m_member_title_suffix4)
		COLUMN_ENTRY_LS(11, m_member_id5)
		COLUMN_ENTRY_LS(12, m_member_title_suffix5)
		COLUMN_ENTRY_LS(13, m_member_id6)
		COLUMN_ENTRY_LS(14, m_member_title_suffix6)
		COLUMN_ENTRY_LS(15, m_member_id7)
		COLUMN_ENTRY_LS(16, m_member_title_suffix7)
		COLUMN_ENTRY_LS(17, m_member_id8)
		COLUMN_ENTRY_LS(18, m_member_title_suffix8)
		COLUMN_ENTRY_LS(19, m_member_id9)
		COLUMN_ENTRY_LS(20, m_member_title_suffix9)
		COLUMN_ENTRY_LS(21, m_member_id10)
		COLUMN_ENTRY_LS(22, m_member_title_suffix10)

		END_COLUMN_MAP()	

	BEGIN_PARAM_MAP_FUNC(_ParamInsert)
		COLUMN_ENTRY(1, m_sibling_title_prefix)
		COLUMN_ENTRY(2, m_member_id1)
		COLUMN_ENTRY(3, m_member_title_suffix1)
		COLUMN_ENTRY(4, m_member_id2)
		COLUMN_ENTRY(5, m_member_title_suffix2)
		COLUMN_ENTRY(6, m_member_id3)
		COLUMN_ENTRY(7, m_member_title_suffix3)
		COLUMN_ENTRY(8, m_member_id4)
		COLUMN_ENTRY(9, m_member_title_suffix4)
		COLUMN_ENTRY(10, m_member_id5)
		COLUMN_ENTRY(11, m_member_title_suffix5)
		COLUMN_ENTRY(12, m_member_id6)
		COLUMN_ENTRY(13, m_member_title_suffix6)
		COLUMN_ENTRY(14, m_member_id7)
		COLUMN_ENTRY(15, m_member_title_suffix7)
		COLUMN_ENTRY(16, m_member_id8)
		COLUMN_ENTRY(17, m_member_title_suffix8)
		COLUMN_ENTRY(18, m_member_id9)
		COLUMN_ENTRY(19, m_member_title_suffix9)
		COLUMN_ENTRY(20, m_member_id10)
		COLUMN_ENTRY(21, m_member_title_suffix10)

	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
	COLUMN_ENTRY_TEST(m_params, sibling_id)
		COLUMN_ENTRY_TEST_ELSE(sibling_title_prefix)
		COLUMN_ENTRY_TEST_ELSE(member_id1)
		COLUMN_ENTRY_TEST_ELSE(member_title_suffix1)
		COLUMN_ENTRY_TEST_ELSE(member_id2)
		COLUMN_ENTRY_TEST_ELSE(member_title_suffix2)
		COLUMN_ENTRY_TEST_ELSE(member_id3)
		COLUMN_ENTRY_TEST_ELSE(member_title_suffix3)
		COLUMN_ENTRY_TEST_ELSE(member_id4)
		COLUMN_ENTRY_TEST_ELSE(member_title_suffix4)
		COLUMN_ENTRY_TEST_ELSE(member_id5)
		COLUMN_ENTRY_TEST_ELSE(member_title_suffix5)
		COLUMN_ENTRY_TEST_ELSE(member_id6)
		COLUMN_ENTRY_TEST_ELSE(member_title_suffix6)
		COLUMN_ENTRY_TEST_ELSE(member_id7)
		COLUMN_ENTRY_TEST_ELSE(member_title_suffix7)
		COLUMN_ENTRY_TEST_ELSE(member_id8)
		COLUMN_ENTRY_TEST_ELSE(member_title_suffix8)
		COLUMN_ENTRY_TEST_ELSE(member_id9)
		COLUMN_ENTRY_TEST_ELSE(member_title_suffix9)
		COLUMN_ENTRY_TEST_ELSE(member_id10)
		COLUMN_ENTRY_TEST_ELSE(member_title_suffix10)
	COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamBySiblingID)
		COLUMN_ENTRY(1, m_sibling_id)
	END_PARAM_MAP_FUNC()
	
	DEFINE_COMMAND_EX(CdboSiblingAccessor, L"SELECT * FROM Sibling")
};

class CdboSibling : public Cdbo<CCommand<CAccessor<CdboSiblingAccessor> > >
{
public:
	HRESULT Delete(DWORD sibling_id)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CdboSibling:Delete sibling_id %d"), sibling_id);
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamBySiblingID);
		m_sibling_id.QuadPart			= sibling_id;
		return OpenCommand(_T("DELETE FROM Sibling WHERE sibling_id = ? "));
	}

	HRESULT Insert(const String& sibling_title_prefix, 
		DWORD member_id1,const String& member_title_suffix1,
		DWORD member_id2,const String& member_title_suffix2,
		DWORD member_id3,const String& member_title_suffix3,
		DWORD member_id4,const String& member_title_suffix4,
		DWORD member_id5,const String& member_title_suffix5,
		DWORD member_id6,const String& member_title_suffix6,
		DWORD member_id7,const String& member_title_suffix7,
		DWORD member_id8,const String& member_title_suffix8,
		DWORD member_id9,const String& member_title_suffix9,
		DWORD member_id10,const String& member_title_suffix10)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamInsert);
		COLUMN_STRING_COPY(m_sibling_title_prefix, sibling_title_prefix.c_str());
		m_member_id1.QuadPart	= member_id1;
		COLUMN_STRING_COPY(m_member_title_suffix1, member_title_suffix1.c_str());
		m_member_id2.QuadPart	= member_id2;
		COLUMN_STRING_COPY(m_member_title_suffix2, member_title_suffix2.c_str());
		m_member_id3.QuadPart	= member_id3;
		COLUMN_STRING_COPY(m_member_title_suffix3, member_title_suffix3.c_str());
		m_member_id4.QuadPart	= member_id4;
		COLUMN_STRING_COPY(m_member_title_suffix4, member_title_suffix4.c_str());
		m_member_id5.QuadPart	= member_id5;
		COLUMN_STRING_COPY(m_member_title_suffix5, member_title_suffix5.c_str());
		m_member_id6.QuadPart	= member_id6;
		COLUMN_STRING_COPY(m_member_title_suffix6, member_title_suffix6.c_str());
		m_member_id7.QuadPart	= member_id7;
		COLUMN_STRING_COPY(m_member_title_suffix7, member_title_suffix7.c_str());
		m_member_id8.QuadPart	= member_id8;
		COLUMN_STRING_COPY(m_member_title_suffix8, member_title_suffix8.c_str());
		m_member_id9.QuadPart	= member_id9;
		COLUMN_STRING_COPY(m_member_title_suffix9, member_title_suffix9.c_str());
		m_member_id10.QuadPart	= member_id10;
		COLUMN_STRING_COPY(m_member_title_suffix10, member_title_suffix10.c_str());

		return OpenCommand(_T("INSERT INTO Sibling (sibling_title_prefix\
			,member_id1,member_title_suffix1\
			,member_id2,member_title_suffix2\
			,member_id3,member_title_suffix3\
			,member_id4,member_title_suffix4\
			,member_id5,member_title_suffix5\
			,member_id6,member_title_suffix6\
			,member_id7,member_title_suffix7\
			,member_id8,member_title_suffix8\
			,member_id9,member_title_suffix9\
			,member_id10,member_title_suffix10\
			) OUTPUT INSERTED.* VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"));
	}

	HRESULT RSBySiblingID(DWORD sibling_id)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamBySiblingID);
		m_sibling_id.QuadPart			= sibling_id;
		return OpenCommand(_T("SELECT * FROM Sibling WHERE sibling_id = ?"));
	}

	HRESULT UpdateByAny(DWORD sibling_id)
	{
		if (m_params.empty())
			return S_FALSE;

		String sql(_T("UPDATE Sibling SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE sibling_id = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_sibling_id.QuadPart = sibling_id;
		m_params.push_back(_T("sibling_id"));		// add sibling_id at the end of params
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}
};

#endif // __DBOSIBLING_H_
