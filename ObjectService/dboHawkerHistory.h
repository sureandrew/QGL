#pragma once
#ifndef __DBOHAWKERHISTORY_H_
#define __DBOHAWKERHISTORY_H_
#include "Dbo.h"

class CdboHawkerHistoryAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboHawkerHistoryAccessor)

	DEFINE_ENTRY(hawkerHistory_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(itemName, 21)
	DEFINE_ENTRY(itemType, INT)
	DEFINE_ENTRY(stack , INT)
	DEFINE_ENTRY(totalPrice , LARGE_INTEGER)
	DEFINE_ENTRY(tax , LARGE_INTEGER)
	DEFINE_ENTRY(hawkerDate , DBTIMESTAMP)
	
	
	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_hawkerHistory_id)
		COLUMN_ENTRY_LS(2, m_char_id)
		COLUMN_ENTRY_LS(3, m_itemName)
		COLUMN_ENTRY_LS(4, m_itemType)
		COLUMN_ENTRY_LS(5, m_stack)
		COLUMN_ENTRY_LS(6, m_totalPrice)
		COLUMN_ENTRY_LS(7, m_tax)
		COLUMN_ENTRY_LS(8, m_hawkerDate)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByID)
		COLUMN_ENTRY(1, m_hawkerHistory_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByCharID)
		COLUMN_ENTRY(1, m_char_id)
	END_PARAM_MAP_FUNC()
	
	DEFINE_COMMAND_EX(CdboHawkerHistoryAccessor, L"SELECT * FROM HawkerHistory")

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, hawkerHistory_id)
		COLUMN_ENTRY_TEST_ELSE(char_id)
		COLUMN_ENTRY_TEST_ELSE(itemName)
		COLUMN_ENTRY_TEST_ELSE(itemType)
		COLUMN_ENTRY_TEST_ELSE(stack)
		COLUMN_ENTRY_TEST_ELSE(totalPrice)
		COLUMN_ENTRY_TEST_ELSE(tax)
		COLUMN_ENTRY_TEST_ELSE(hawkerDate)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_char_id)
		COLUMN_ENTRY(2, m_itemName)
		COLUMN_ENTRY(3, m_itemType)
		COLUMN_ENTRY(4, m_stack)
		COLUMN_ENTRY(5, m_totalPrice)
		COLUMN_ENTRY(6, m_tax)
	END_PARAM_MAP_FUNC()
};

class CdboHawkerHistory : public Cdbo<CCommand<CAccessor<CdboHawkerHistoryAccessor> > >
{
public:
	HRESULT Delete(DWORD hid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByID);
		m_hawkerHistory_id.QuadPart	= hid;
		return OpenCommand(_T("DELETE FROM HawkerHistory WHERE hawkerHistory_id = ? "));
	}

	HRESULT RSByCharID(DWORD cid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByCharID);
		m_char_id.QuadPart = cid;
		return OpenCommand(_T("SELECT * FROM HawkerHistory WHERE char_id = ?"));
	}

	HRESULT Insert(DWORD char_id, const String& itemName, int itemType, int stack, DWORD totalPrice, DWORD tax)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);
		
		m_char_id.QuadPart = char_id;
		COLUMN_STRING_COPY(m_itemName, itemName.c_str() );
		m_itemType = itemType;
		m_stack = stack;
		m_totalPrice.QuadPart = totalPrice;
		m_tax.QuadPart = tax;

		return OpenCommand(_T("INSERT INTO HawkerHistory (char_id, itemName, itemType, ")
			_T("stack, totalPrice, tax, hawkerDate) ")
			_T("OUTPUT INSERTED.* VALUES (?, ?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // __DBOHAWKERHISTORY_H_
