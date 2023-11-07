#pragma once
#ifndef __DBOGIFT_H_
#define __DBOGIFT_H_
#include "Dbo.h"

class CdboGiftAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboGiftAccessor)

	DEFINE_ENTRY(gift_id, LARGE_INTEGER)
	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(item_id, LONG)
	DEFINE_ENTRY(itemtype, INT)
	DEFINE_ENTRY(quantity , INT)
	DEFINE_ENTRY_STR(serialNo, 17)
	
	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_gift_id)
		COLUMN_ENTRY_LS(2, m_acct_id)
		COLUMN_ENTRY_LS(3, m_item_id)
		COLUMN_ENTRY_LS(4, m_itemtype)
		COLUMN_ENTRY_LS(5, m_quantity)
		COLUMN_ENTRY_LS(6, m_serialNo)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByID)
		COLUMN_ENTRY(1, m_gift_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByAcctID)
		COLUMN_ENTRY(1, m_acct_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByAcctIDGiftID)
		COLUMN_ENTRY(1, m_acct_id)
		COLUMN_ENTRY(2, m_gift_id)
	END_PARAM_MAP_FUNC()
	
	DEFINE_COMMAND_EX(CdboGiftAccessor, L"SELECT * FROM GiftBox")
};

class CdboGift : public Cdbo<CCommand<CAccessor<CdboGiftAccessor> > >
{
public:
	HRESULT Delete(DWORD gid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByID);
		m_gift_id.QuadPart	= gid;
		return OpenCommand(_T("DELETE FROM GiftBox WHERE gift_id = ? "));
	}

	HRESULT RSByAcctID(DWORD aid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByAcctID);
		m_acct_id.QuadPart = aid;
		return OpenCommand(_T("SELECT * FROM GiftBox WHERE acct_id = ?"));
	}

	HRESULT RSByAcctID(DWORD aid, DWORD gid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByAcctIDGiftID);
		m_acct_id.QuadPart = aid;
		m_gift_id.QuadPart	= gid;
		return OpenCommand(_T("SELECT * FROM GiftBox WHERE acct_id = ? AND gift_id > ?"));
	}
};

#endif // __DBOGIFT_H_
