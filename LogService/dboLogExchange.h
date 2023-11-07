// dboLogExchange.h : Declaration of the CdboLogExchange

#pragma once
#ifndef _DBO_LOG_EXCHANGE_H_
#define _DBO_LOG_EXCHANGE_H_
#include "Dbo.h"

class CdboLogExchangeAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogExchangeAccessor)

	DEFINE_ENTRY_STR(sess_id, 13)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(target_char_id, LARGE_INTEGER)
	DEFINE_ENTRY(process_type, INT)
	DEFINE_ENTRY(item_type, INT)
	DEFINE_ENTRY(item_uid, LARGE_INTEGER)
	DEFINE_ENTRY(item_id, INT)
	DEFINE_ENTRY(item_count, INT)	
	DEFINE_ENTRY(money, INT)
	DEFINE_ENTRY(tax, INT)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_sess_id)
		COLUMN_ENTRY(2, m_char_id)
		COLUMN_ENTRY(3, m_target_char_id)
		COLUMN_ENTRY(4, m_process_type)
		COLUMN_ENTRY(5, m_item_type)
		COLUMN_ENTRY(6, m_item_uid)
		COLUMN_ENTRY(7, m_item_id)
		COLUMN_ENTRY(8, m_item_count)
		COLUMN_ENTRY(9, m_money)
		COLUMN_ENTRY(10, m_tax)		
	END_PARAM_MAP_FUNC()
	
	DEFINE_COMMAND_EX(CdboLogExchangeAccessor, L"SELECT * FROM LogExchange")

};

class CdboLogExchange : public Cdbo<CCommand<CAccessor<CdboLogExchangeAccessor> > >
{
public:
	HRESULT Log(PCSTRING pSessId, UInt32 char_id, UInt32 target_char_id, UInt8 process_type, UInt8 item_type, UInt32 item_uid, UInt32 item_id, UInt8 item_count,  UInt32 money, UInt32 tax)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		COLUMN_STRING_COPY(m_sess_id, pSessId)
		
		m_char_id.QuadPart = char_id;
		m_target_char_id.QuadPart = target_char_id;
		m_process_type = process_type;
		m_item_type = item_type;
		m_item_uid.QuadPart = item_uid;
		m_item_id = item_id;
		m_item_count = item_count;		
		m_money = money;
		m_tax = tax;

		return OpenCommand(_T("INSERT INTO LogExchange (sess_id, char_id, target_char_id, process_type, item_type, item_uid, item_id, item_count, money, tax, create_time) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif //_DBO_LOG_EXCHANGE_H_