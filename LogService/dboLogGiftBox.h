// dboLogGiftBox.h : Declaration of the CdboLogGiftBox

#pragma once
#ifndef _DBO_LOG_GIFTBOX_H_
#define _DBO_LOG_GIFTBOX_H_
#include "Dbo.h"


class CdboLogGiftBoxAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogGiftBoxAccessor)

	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(item_id, INT)
	DEFINE_ENTRY(itemType, INT)
	DEFINE_ENTRY(quantity, INT)
	DEFINE_ENTRY_STR(serialNo, 17)
	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogGiftBoxAccessor)
	//ENABLE_PARAM_MAP(CdboLogGiftBoxAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_acct_id)
		COLUMN_ENTRY(2, m_char_id)
		COLUMN_ENTRY(3, m_item_id)
		COLUMN_ENTRY(4, m_itemType)
		COLUMN_ENTRY(5, m_quantity)
		COLUMN_ENTRY(6, m_serialNo)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogGiftBoxAccessor, L"SELECT * FROM LogGiftBox")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogGiftBox : public Cdbo<CCommand<CAccessor<CdboLogGiftBoxAccessor> > >
{
public:

	HRESULT Log(UInt32 acct_id, UInt32 char_id, UInt32 item_id, UInt8 itemType, UInt8 quantity, PCSTRING pSerialNo)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		
		m_acct_id.QuadPart = acct_id;
		m_char_id.QuadPart = char_id;
		m_item_id = item_id;
		m_itemType = itemType;
		m_quantity = quantity;
		COLUMN_STRING_COPY(m_serialNo, pSerialNo)

		return OpenCommand(_T("INSERT INTO LogGiftBox (acct_id, char_id, item_id, itemType, quantity, serialNo, create_time) VALUES (?, ?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_GIFTBOX_H_
