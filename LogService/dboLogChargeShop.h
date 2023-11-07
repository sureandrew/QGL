// dboLogChargeShop.h : Declaration of the CdboLogChargeShop

#pragma once
#ifndef _DBO_LOG_CHARGESHOP_H_
#define _DBO_LOG_CHARGESHOP_H_
#include "Dbo.h"

class CdboLogChargeShopAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogChargeShopAccessor)

	DEFINE_ENTRY_STR(sess_id, 13)
	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(item_id, INT)
	DEFINE_ENTRY(item_count, INT)
	DEFINE_ENTRY(item_type, INT)
	DEFINE_ENTRY(yuanbao, INT)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_sess_id)
		COLUMN_ENTRY(2, m_acct_id)
		COLUMN_ENTRY(3, m_char_id)
		COLUMN_ENTRY(4, m_item_id)
		COLUMN_ENTRY(5, m_item_count)
		COLUMN_ENTRY(6, m_item_type)
		COLUMN_ENTRY(7, m_yuanbao)
	END_PARAM_MAP_FUNC()
	
	DEFINE_COMMAND_EX(CdboLogChargeShopAccessor, L"SELECT * FROM LogChargeShop")

};

class CdboLogChargeShop : public Cdbo<CCommand<CAccessor<CdboLogChargeShopAccessor> > >
{
public:
	HRESULT Log(PCSTRING pSessId, UInt32 acct_id, UInt32 char_id, UInt32 item_id, UInt16 item_count, UInt8 item_type, UInt32 yuanbao)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		COLUMN_STRING_COPY(m_sess_id, pSessId)
		m_acct_id.QuadPart = acct_id;
		m_char_id.QuadPart = char_id;
		m_item_id = item_id;
		m_item_count = item_count;
		m_item_type = item_type;
		m_yuanbao = yuanbao;
		return OpenCommand(_T("INSERT INTO LogChargeShop (sess_id, acct_id, char_id, item_id, item_count, item_type, yuanbao, create_time) VALUES (?, ?, ?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_CHARGESHOP_H_