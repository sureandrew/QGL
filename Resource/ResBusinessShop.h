#pragma once
#ifndef _RESOURCE_BUSINESSSHOP_H_
#define _RESOURCE_BUSINESSSHOP_H_

#include <Reuben\System\Reuben.h>
struct BusinessShopData
{
	bool			sell;
	UInt8			shopName;
	UInt16			shop_id;
	UInt16			numOfItem;
	UInt16Vector	countLst;
	UInt32Vector	item_idLst;
	UInt32Vector	minPriceLst;
	UInt32Vector	maxPriceLst;

	BusinessShopData()	{
		
	}

	~BusinessShopData()	{
		countLst.clear();
		item_idLst.clear();
		minPriceLst.clear();
		maxPriceLst.clear();
	}
};

#endif // _RESOURCE_BUSINESSSHOP_H_
