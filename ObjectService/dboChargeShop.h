#pragma once
#ifndef __DBOCHARGESHOP_H_
#define __DBOCHARGESHOP_H_
#include "Dbo.h"

class CdboChargeShopAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboChargeShopAccessor)

	DEFINE_ENTRY(ChargeShopID, LARGE_INTEGER)
	DEFINE_ENTRY_STR(Name, 21)
	DEFINE_ENTRY(Active, INT)
	DEFINE_ENTRY(Type, INT)
	DEFINE_ENTRY(ItemType, INT)
	DEFINE_ENTRY(LV, LONG)
	DEFINE_ENTRY(PreviewType, INT)
	DEFINE_ENTRY(ItemID, LARGE_INTEGER)
	DEFINE_ENTRY(YuanBao, LARGE_INTEGER)
	DEFINE_ENTRY(Discount, INT)
	DEFINE_ENTRY(TotalStock, LONG)
	DEFINE_ENTRY(LimitType, INT)
	DEFINE_ENTRY(StartTime, DBTIMESTAMP)
	DEFINE_ENTRY(EndTime, DBTIMESTAMP)
	DEFINE_ENTRY(NeedRestore, INT)
	DEFINE_ENTRY(Remain, LONG)
	DEFINE_ENTRY(LastOrderDate, DBTIMESTAMP)
	DEFINE_ENTRY(TotalSell, LARGE_INTEGER)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboChargeShopAccessor)
	//ENABLE_PARAM_MAP(CdboChargeShopAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_ChargeShopID)
		COLUMN_ENTRY_LS(2, m_Name)
		COLUMN_ENTRY_LS(3, m_Active)
		COLUMN_ENTRY_LS(4, m_Type)
		COLUMN_ENTRY_LS(5, m_ItemType)
		COLUMN_ENTRY_LS(6, m_LV)
		COLUMN_ENTRY_LS(7, m_PreviewType)
		COLUMN_ENTRY_LS(8, m_ItemID)
		COLUMN_ENTRY_LS(9, m_YuanBao)
		COLUMN_ENTRY_LS(10, m_Discount)
		COLUMN_ENTRY_LS(11, m_TotalStock)
		COLUMN_ENTRY_LS(12, m_LimitType)
		COLUMN_ENTRY_LS(13, m_StartTime)
		COLUMN_ENTRY_LS(14, m_EndTime)
		COLUMN_ENTRY_LS(15, m_NeedRestore)
		COLUMN_ENTRY_LS(16, m_Remain)
		COLUMN_ENTRY_LS(17, m_LastOrderDate)
		COLUMN_ENTRY_LS(18, m_TotalSell)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByID)
		COLUMN_ENTRY(1, m_ChargeShopID)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByActive)
		COLUMN_ENTRY(1, m_Active)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, ChargeShopID)
		COLUMN_ENTRY_TEST_ELSE(Name)
		COLUMN_ENTRY_TEST_ELSE(Active)
		COLUMN_ENTRY_TEST_ELSE(Type)
		COLUMN_ENTRY_TEST_ELSE(ItemType)
		COLUMN_ENTRY_TEST_ELSE(LV)
		COLUMN_ENTRY_TEST_ELSE(PreviewType)
		COLUMN_ENTRY_TEST_ELSE(ItemID)
		COLUMN_ENTRY_TEST_ELSE(YuanBao)
		COLUMN_ENTRY_TEST_ELSE(Discount)
		COLUMN_ENTRY_TEST_ELSE(TotalStock)
		COLUMN_ENTRY_TEST_ELSE(LimitType)
		COLUMN_ENTRY_TEST_ELSE(StartTime)
		COLUMN_ENTRY_TEST_ELSE(EndTime)
		COLUMN_ENTRY_TEST_ELSE(NeedRestore)
		COLUMN_ENTRY_TEST_ELSE(Remain)
		COLUMN_ENTRY_TEST_ELSE(LastOrderDate)
		COLUMN_ENTRY_TEST_ELSE(TotalSell)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_ChargeShopID)
		COLUMN_ENTRY(2, m_Name)
		COLUMN_ENTRY(3, m_Active)
		COLUMN_ENTRY(4, m_Type)
		COLUMN_ENTRY(5, m_ItemType)
		COLUMN_ENTRY(6, m_LV)
		COLUMN_ENTRY(7, m_PreviewType)
		COLUMN_ENTRY(8, m_ItemID)
		COLUMN_ENTRY(9, m_YuanBao)
		COLUMN_ENTRY(10, m_Discount)
		COLUMN_ENTRY(11, m_TotalStock)
		COLUMN_ENTRY(12, m_LimitType)
		COLUMN_ENTRY(13, m_StartTime)
		COLUMN_ENTRY(14, m_EndTime)
		COLUMN_ENTRY(15, m_NeedRestore)
		COLUMN_ENTRY(16, m_Remain)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboChargeShopAccessor, L"SELECT * FROM ChargeShop")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboChargeShop : public Cdbo<CCommand<CAccessor<CdboChargeShopAccessor> > >
{
public:
	HRESULT Insert(DWORD chargeShopId, const String& name)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);

		m_ChargeShopID.QuadPart = chargeShopId;
		COLUMN_STRING_COPY(m_Name, name.c_str() );
		
		return OpenCommand(_T("INSERT INTO ChargeShop (ChargeShopID, Name, Active, ")
			_T("Type, ItemType, LV, PreviewType, ItemID, YuanBao, Discount, TotalStock, ")
			_T("LimitType, StartTime, EndTime, NeedRestore, Remain) ")
			_T("OUTPUT INSERTED.* VALUES (")
			_T("?, ?, ?, ?, ?, ?, ?, ?, ?, ? ,? ,? ,? ,? ,? ,? )"));
	}

	HRESULT Delete(DWORD cid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByID);
		m_ChargeShopID.QuadPart	= cid;
		return OpenCommand(_T("DELETE FROM ChargeShop WHERE ChargeShopID = ? "));
	}
 
	HRESULT RSByID(DWORD cid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByID);
		m_ChargeShopID.QuadPart = cid;
		return OpenCommand(_T("SELECT * FROM ChargeShop WHERE ChargeShopID = ?"));
	}

	HRESULT RSByActive()
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByActive);
		m_Active = 1;
		return OpenCommand(_T("SELECT * FROM ChargeShop WHERE Active = ?"));
	}

	HRESULT RSByDisactive()
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByActive);
		m_Active = 0;
		return OpenCommand(_T("SELECT * FROM ChargeShop WHERE Active = ?"));
	}

	HRESULT RSByAll()
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(NULL);
		
		return OpenCommand(_T("SELECT * FROM ChargeShop"));
	}

	HRESULT UpdateByAny(DWORD chargeShopID)
	{
		if (m_params.empty()) {
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("UpdateByAny Error (1) [empty paramater] ChargeShopID=%d"),
				chargeShopID);
			return S_FALSE;
		}

		String sql(_T("UPDATE ChargeShop SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE ChargeShopID = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_ChargeShopID.QuadPart	= chargeShopID;
		m_params.push_back(_T("ChargeShopID"));
		
		//TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}
};

#endif // __DBOCHARGESHOP_H_
