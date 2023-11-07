#pragma once
#ifndef _CHARGESHOPITEM_H_
#define _CHARGESHOPITEM_H_

#include "Proxy/Logic/ChargeShopItem_Proxy.h"

class CChargeShopItem : public ChargeShopItem_Proxy
{
public: 
	DEFINE_CLASS(ChargeShopItem_CLASSID);
private:
	typedef ChargeShopItem_Proxy Parent;
// OnUpdate can be implemented selectively
	virtual void OnUpdateItemInfo(const UInt32 &chargeShopID, const String &itemName, const UInt8 &shopType, const UInt8 &itemType, const UInt32 &itemId, const UInt32 &mMoney, const UInt8 &LV, const UInt8 &PreviewType);
	virtual void OnUpdateDiscountInfo(const UInt8 &discount);

	virtual void OnUpdateActiveInfo(const UInt8 &active);
	virtual void OnUpdateStockInfo(const UInt16 &totalStock);
	virtual void OnUpdateRemainInfo(const UInt16 &remain);
	virtual void OnUpdateLimitedInfo(const UInt8 &LimitType, const UInt8 &restore);
	virtual void OnUpdateExistTimeInfo(const UInt32 &StartTime, const UInt32 &EndTime);
	virtual void OnUpdateLastOrderInfo(const UInt32 &lastOrderDate);
	virtual void OnUpdateSellInfo(const UInt32 &totalSell);
};

#endif //_CHARGESHOPITEM_H_
