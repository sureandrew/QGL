#pragma once
#ifndef __DBOSELLYUANBAO_H_
#define __DBOSELLYUANBAO_H_
#include "Dbo.h"

class CdboSellYuanBaoAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboSellYuanBaoAccessor)

	DEFINE_ENTRY(sellYuanBao_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(charName, 21)
	DEFINE_ENTRY(sellType, INT)
	DEFINE_ENTRY(totalYBT, LARGE_INTEGER)
	DEFINE_ENTRY(moneyPerYBT, LARGE_INTEGER)
	DEFINE_ENTRY(soldYBT, LARGE_INTEGER)
	DEFINE_ENTRY(expireDate, DBTIMESTAMP)
	DEFINE_ENTRY(success, INT)
	DEFINE_ENTRY(buyAmount, LARGE_INTEGER)

	DEFINE_ENTRY(soldMoney, LARGE_INTEGER)
	DEFINE_ENTRY(remainYBT, LARGE_INTEGER)

	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	
	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_sellYuanBao_id)
		COLUMN_ENTRY_LS(2, m_char_id)
		COLUMN_ENTRY_LS(3, m_charName)
		COLUMN_ENTRY_LS(4, m_sellType)
		COLUMN_ENTRY_LS(5, m_totalYBT)
		COLUMN_ENTRY_LS(6, m_moneyPerYBT)
		COLUMN_ENTRY_LS(7, m_soldYBT)
		COLUMN_ENTRY_LS(8, m_expireDate)
	END_COLUMN_MAP()	

	BEGIN_COLUMN_MAP_FUNC(_GetNewInsertID)
		COLUMN_ENTRY_LS(1, m_sellYuanBao_id)
	END_COLUMN_MAP()	

	BEGIN_PARAM_MAP_FUNC(_ParamByID)
		COLUMN_ENTRY(1, m_sellYuanBao_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByCharID)
		COLUMN_ENTRY(1, m_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_UpdateSold)		
		COLUMN_ENTRY(1, m_soldYBT)
		COLUMN_ENTRY(2, m_sellYuanBao_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_acct_id)
		COLUMN_ENTRY(2, m_char_id)		
		COLUMN_ENTRY(3, m_charName)
		COLUMN_ENTRY(4, m_sellType)
		COLUMN_ENTRY(5, m_totalYBT)
		COLUMN_ENTRY(6, m_moneyPerYBT)
		COLUMN_ENTRY(7, m_expireDate)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_BuyYBT)
		COLUMN_ENTRY(1, m_acct_id)
		COLUMN_ENTRY(2, m_sellYuanBao_id)
		COLUMN_ENTRY(3, m_buyAmount)
	END_PARAM_MAP_FUNC()

	BEGIN_COLUMN_MAP_FUNC(_GetBuyYBTResult)
		COLUMN_ENTRY_LS(1, m_success)
		COLUMN_ENTRY_LS(2, m_soldYBT)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetTempBank)
		COLUMN_ENTRY_LS(1, m_sellYuanBao_id)
		COLUMN_ENTRY_LS(2, m_soldMoney)
		COLUMN_ENTRY_LS(3, m_remainYBT)
	END_COLUMN_MAP()
	
	DEFINE_COMMAND_EX(CdboSellYuanBaoAccessor, L"SELECT * FROM SellYuanBao")
};

class CdboSellYuanBao : public Cdbo<CCommand<CAccessor<CdboSellYuanBaoAccessor> > >
{
public:
	HRESULT GetAll()
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(NULL);		
		return OpenCommand(_T("SELECT * FROM SellYuanBao"));
	}

	HRESULT Insert(DWORD aid, DWORD cid, const String& charName, int sellType, DWORD totalYBT, DWORD moneyPerYBT, DBTIMESTAMP dbtime)
	{
		
		SetGetBindFunc(_GetAll);
		//SetGetBindFunc(_GetNewInsertID);
		SetGetParamsFunc(_Insert);

		m_acct_id.QuadPart = aid;
		m_char_id.QuadPart = cid;

		COLUMN_STRING_COPY(m_charName, charName.c_str());
		m_sellType = sellType;
		m_totalYBT.QuadPart = totalYBT;
		m_moneyPerYBT.QuadPart = moneyPerYBT;
		m_expireDate = dbtime;

		return OpenCommand(_T("EXEC sp_InsertSellYuanBao ?, ?, ?, ?, ?, ?, ?;"));
		
	}

	/*HRESULT UpdateTotalSell(DWORD sid, DWORD sold)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_UpdateSold);

		m_sellYuanBao_id.QuadPart = sid;
		m_soldYBT.QuadPart = sold;

        return OpenCommand(_T("UPDATE SellYuanBao SET soldYBT = ? WHERE sellYuanBao_id = ?"));
	}*/

	HRESULT BuyYBT(DWORD aid, DWORD sid, DWORD amount)
	{
		SetGetBindFunc(_GetBuyYBTResult);
		SetGetParamsFunc(_BuyYBT);
		
		m_acct_id.QuadPart = aid;
		m_sellYuanBao_id.QuadPart = sid;
		m_buyAmount.QuadPart = amount;

		return OpenCommand(_T("EXEC sp_BuyYuanBaoTicket ?, ?, ?;"));
	}

	HRESULT GetTempBank(DWORD cid)
	{
		SetGetBindFunc(_GetTempBank);
		SetGetParamsFunc(_ParamByCharID);
		
		m_char_id.QuadPart = cid;
		
		return OpenCommand(_T("EXEC sp_GetTempBankMoneyYBT ?;"));
	}

};

#endif // __DBOGIFT_H_