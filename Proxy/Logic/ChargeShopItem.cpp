//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/ChargeShopItem.h"
#include "QGL/MenuOnlineShopProcess.h"
#include "QGL/Global.h"

REGISTER_CLASSTYPE(CChargeShopItem, ChargeShopItem_CLASSID);


void CChargeShopItem::OnUpdateItemInfo(const UInt32 &chargeShopID, const String &itemName, const UInt8 &shopType, const UInt8 &itemType, const UInt32 &itemId, const UInt32 &mMoney, const UInt8 &LV, const UInt8 &PreviewType)
{

}

void CChargeShopItem::OnUpdateDiscountInfo(const UInt8 &discount)
{

}

void CChargeShopItem::OnUpdateActiveInfo(const UInt8 &active)
{

}

void CChargeShopItem::OnUpdateStockInfo(const UInt16 &totalStock)
{

}

void CChargeShopItem::OnUpdateRemainInfo(const UInt16 &remain)
{
	if (gGlobal.g_pOnlineShopProcess)
	{
		gGlobal.g_pOnlineShopProcess->ShowLimitedItem();
	}
}

void CChargeShopItem::OnUpdateLimitedInfo(const UInt8 &LimitType, const UInt8 &restore)
{

}

void CChargeShopItem::OnUpdateExistTimeInfo(const UInt32 &StartTime, const UInt32 &EndTime)
{

}

void CChargeShopItem::OnUpdateLastOrderInfo(const UInt32 &lastOrderDate)
{

}

void CChargeShopItem::OnUpdateSellInfo(const UInt32 &totalSell)
{

}