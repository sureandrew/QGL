// dboLogYuanBao.h : Declaration of the CdboLogYuanBao

#pragma once
#ifndef _DBO_LOG_SELLYUANBAO_H_
#define _DBO_LOG_SELLYUANBAO_H_
#include "Dbo.h"


class CdboLogSellYuanBaoAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogSellYuanBaoAccessor)

	DEFINE_ENTRY_STR(sess_id, 13)
	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(process_type, INT)
	DEFINE_ENTRY(target_id, LARGE_INTEGER)
	DEFINE_ENTRY(YB, LARGE_INTEGER)
	DEFINE_ENTRY(YBT, LARGE_INTEGER)
	DEFINE_ENTRY(moneyPerUnit, LARGE_INTEGER)
	DEFINE_ENTRY(totalMoney, LARGE_INTEGER)
	DEFINE_ENTRY(tax, LARGE_INTEGER)
	DEFINE_ENTRY(expireDate, DBTIMESTAMP)


	
	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboLogYuanBaoAccessor)
	//ENABLE_PARAM_MAP(CdboLogYuanBaoAccessor)

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_sess_id)
		COLUMN_ENTRY(2, m_acct_id)
		COLUMN_ENTRY(3, m_char_id)
		COLUMN_ENTRY(4, m_process_type)
		COLUMN_ENTRY(5, m_target_id)
		COLUMN_ENTRY(6, m_YB)
		COLUMN_ENTRY(7, m_YBT)
		COLUMN_ENTRY(8, m_moneyPerUnit)
		COLUMN_ENTRY(9, m_totalMoney)
		COLUMN_ENTRY(10, m_tax)
		COLUMN_ENTRY(11, m_expireDate)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboLogSellYuanBaoAccessor, L"SELECT * FROM LogSellYuanBao")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboLogSellYuanBao : public Cdbo<CCommand<CAccessor<CdboLogSellYuanBaoAccessor> > >
{
public:

	HRESULT Log(PCSTRING pSessId, UInt32 acct_id, UInt32 char_id, UInt8 process_type, UInt32 target_id, 
		UInt32 YB, UInt32 YBT, UInt32 moneyPerUnit, UInt32 totalMoney, UInt32 tax, UInt32 expireDate)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		
		COLUMN_STRING_COPY(m_sess_id, pSessId)
		m_acct_id.QuadPart = acct_id;
		m_char_id.QuadPart = char_id;
		m_process_type = process_type;
		m_target_id.QuadPart = target_id;
		m_YB.QuadPart = YB;
		m_YBT.QuadPart = YBT;
		m_moneyPerUnit.QuadPart = moneyPerUnit;
		m_totalMoney.QuadPart = totalMoney;
		m_tax.QuadPart = tax;

		DBTIMESTAMP dbtime;
		time_t temp = (time_t)expireDate;
		struct tm *newtime = localtime(&temp);
		dbtime.year		= newtime->tm_year + 1900;
		dbtime.month	= newtime->tm_mon + 1;
		dbtime.day		= newtime->tm_mday;
		dbtime.hour		= newtime->tm_hour;
		dbtime.minute	= newtime->tm_min;
		dbtime.second	= newtime->tm_sec;
		dbtime.fraction	= 0;

		m_expireDate = dbtime;

		return OpenCommand(_T("INSERT INTO LogSellYuanBao (sess_id, acct_id, char_id, process_type, ")
			_T("target_id, YB, YBT, moneyPerUnit, totalMoney, tax, expireDate, create_time) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, GetDate())"));
	}
};

#endif // _DBO_LOG_SELLYUANBAO_H_
