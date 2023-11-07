#pragma once
#ifndef __DBOPARTNERCOLLECTION_H_
#define __DBOPARTNERCOLLECTION_H_
#include "Dbo.h"

class CdboPartnerCollectionAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboPartnerCollectionAccessor)

	DEFINE_ENTRY(collection_uid, LARGE_INTEGER)
	DEFINE_ENTRY(collection_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	
	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_collection_uid)
		COLUMN_ENTRY_LS(2, m_collection_id)
		COLUMN_ENTRY_LS(3, m_char_id)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamInsert)
		COLUMN_ENTRY(1, m_collection_id)
		COLUMN_ENTRY(2, m_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByCharID)
		COLUMN_ENTRY(1, m_char_id)
	END_PARAM_MAP_FUNC()
	
	DEFINE_COMMAND_EX(CdboPartnerCollectionAccessor, L"SELECT * FROM PartnerCollection")
};

class CdboPartnerCollection : public Cdbo<CCommand<CAccessor<CdboPartnerCollectionAccessor> > >
{
public:
	HRESULT Delete(DWORD char_id)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByCharID);
		m_char_id.QuadPart			= char_id;
		return OpenCommand(_T("DELETE FROM PartnerCollection WHERE char_id = ? "));
	}

	HRESULT Insert(DWORD collection_id, DWORD char_id)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamInsert);
		m_collection_id.QuadPart	= collection_id;
		m_char_id.QuadPart			= char_id;

		return OpenCommand(_T("INSERT INTO PartnerCollection (collection_id, char_id)")
			_T("OUTPUT INSERTED.* VALUES (?, ?)"));
	}

	HRESULT RSByCharID(DWORD char_id)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByCharID);
		m_char_id.QuadPart			= char_id;
		return OpenCommand(_T("SELECT * FROM PartnerCollection WHERE char_id = ?"));
	}
};

#endif // __DBOPARTNERCOLLECTION_H_
