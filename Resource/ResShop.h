#pragma once
#ifndef _RESOURCE_SHOP_H_
#define _RESOURCE_SHOP_H_

#include <Reuben\System\Reuben.h>

struct ShopLstData
{
	UInt32					ShopId;
	Int						ShopName;
	Int						CanSell;
	Int						CanRepair;
	UInt16					BuyDiscount;
	UInt16					SellDiscount;
	UInt16					RepairDiscount;
	UInt16					uiShopType;

	UInt32Vector			ItemLst;

	ShopLstData()
		: ShopId(C_UINT32(0))
		, ShopName(C_INT(0))
		, CanSell(C_INT(0))
		, CanRepair(C_INT(0))
		, BuyDiscount(C_UINT16(0))
		, SellDiscount(C_UINT16(0))
		, RepairDiscount(C_UINT16(0))
		, uiShopType(C_UINT16(0))
	{}

	~ShopLstData()
	{
		ItemLst.clear();
	};
};

#endif // _RESOURCE_SHOP_H_