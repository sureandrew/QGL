#pragma once
#ifndef _CHARGE_SHOP_DATA_H_
#define _CHARGE_SHOP_DATA_H_


struct ChargeShopData
{
	UInt32 chargeShop_ID;
	TCHAR chargeShop_ItemName[21];
	UInt8 chargeShop_Active;
	UInt8 chargeShop_Type;
	UInt8 chargeShop_ItemType;
	UInt8 chargeShop_LV;
	UInt8 chargeShop_PreviewType;
	UInt32 chargeShop_ItemID;
	UInt32 chargeShop_Mmoney;
	UInt8 chargeShop_Discount;
	UInt16 chargeShop_TotalStock;
	UInt8 chargeShop_LimitType;
	TCHAR chargeShop_StartTime[15];
	TCHAR chargeShop_EndTime[15];
	UInt8 chargeShop_Restore;

	ChargeShopData()
		: chargeShop_ID(C_UINT32(0))
		, chargeShop_Active(C_UINT8(0))
		, chargeShop_Type(C_UINT8(0))
		, chargeShop_ItemType(C_UINT8(0))
		, chargeShop_LV(C_UINT8(0))
		, chargeShop_PreviewType(C_UINT8(0))
		, chargeShop_ItemID(C_UINT32(0))
		, chargeShop_Mmoney(C_UINT32(0))
		, chargeShop_Discount(C_UINT8(0))
		, chargeShop_TotalStock(C_UINT16(0))
		, chargeShop_LimitType(C_UINT8(0))
		, chargeShop_Restore(C_UINT8(0))
	{
		ZeroMemory(chargeShop_ItemName, sizeof(TCHAR) * 21);
		ZeroMemory(chargeShop_StartTime, sizeof(TCHAR) * 15);
		ZeroMemory(chargeShop_EndTime, sizeof(TCHAR) * 15);
	}

	~ChargeShopData(){};
	
};

#endif // _CHARGESHOP_H_