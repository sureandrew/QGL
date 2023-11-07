//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Logic/ChargeShopItem.h"
#include "Proxy/Data/ChargeShopData.h"
#include "MapService/Global.h"
#include "Proxy/Service/ObjectService.h"

REGISTER_CLASSTYPE(CChargeShopItem, ChargeShopItem_CLASSID);

void CChargeShopItem::PreSend()
{
	RPCMask mask;
	mask.MaskBit(CChargeShopItem::AllMask);
	SetSendMask(mask);
}

void CChargeShopItem::SetData(CChargeShopData* pChargeShopData, bool bClear)
{
	if (pChargeShopData->IsUpdated(CChargeShopData::idInfoMask))
	{
		SetIdInfo(pChargeShopData->GetChargeShopID());
	}

	if (pChargeShopData->IsUpdated(CChargeShopData::itemInfoMask))
	{
		SetItemInfo( pChargeShopData->GetName(), pChargeShopData->GetShopType(),
			pChargeShopData->GetItemType(), pChargeShopData->GetItemId(), pChargeShopData->GetYuanBao(),
			pChargeShopData->GetLV(), pChargeShopData->GetPreviewType() );
	}

	if (pChargeShopData->IsUpdated(CChargeShopData::discountInfoMask))
	{
		SetDiscountInfo(pChargeShopData->GetDiscount());
	}

	if (pChargeShopData->IsUpdated(CChargeShopData::stockInfoMask))
	{
		SetStockInfo(pChargeShopData->GetTotalStock());
	}

	if (pChargeShopData->IsUpdated(CChargeShopData::remainInfoMask))
	{
		SetRemainInfo(pChargeShopData->GetRemain());
	}
	
	if (pChargeShopData->IsUpdated(CChargeShopData::limitedInfoMask))
	{
		SetLimitedInfo(pChargeShopData->GetLimitType(), pChargeShopData->GetRestore());
	}

	if (pChargeShopData->IsUpdated(CChargeShopData::existTimeInfoMask))
	{
		SetExistTimeInfo(pChargeShopData->GetStartTime(), pChargeShopData->GetEndTime());
	}

	if (pChargeShopData->IsUpdated(CChargeShopData::lastOrderInfoMask))
	{
		SetLastOrderInfo(pChargeShopData->GetLastOrderDate());
	}

	if (pChargeShopData->IsUpdated(CChargeShopData::sellInfoMask))
	{
		SetSellInfo(pChargeShopData->GetTotalSell());
	}

	if (bClear)
		ClearAllMask();
}

Boolean CChargeShopItem::NeedUpdate()
{
	RPCMask mask;

	mask.MaskBits( remainInfoMask, lastOrderInfoMask, sellInfoMask, -1);

	return IsModified(mask);
}

void CChargeShopItem::WriteData(CChargeShopData* pChargeShopData)
{
	if (IsModified(itemInfoMask))
	{
		pChargeShopData->SetItemInfo(GetItemName(), GetShopType(), GetItemType(), GetItemId(), GetYuanBao(), GetLV(), GetPreviewType());
	}

	if (IsModified(discountInfoMask))
	{
		pChargeShopData->SetDiscountInfo(GetDiscount());
	}

	if (IsModified(stockInfoMask))
	{
		pChargeShopData->SetStockInfo(GetTotalStock());
	}

	if (IsModified(remainInfoMask))
	{
		pChargeShopData->SetRemainInfo(GetRemain());
	}

	if (IsModified(limitedInfoMask))
	{
		pChargeShopData->SetLimitedInfo(GetLimitType(), GetRestore());
	}

	if (IsModified(existTimeInfoMask))
	{
		pChargeShopData->SetExistTimeInfo(GetStartTime(), GetEndTime());
	}

	if (IsModified(lastOrderInfoMask))
	{
		pChargeShopData->SetLastOrderInfo(GetLastOrderDate());
	}

	if (IsModified(sellInfoMask))
	{
		pChargeShopData->SetSellInfo(GetTotalSell());
	}

	pChargeShopData->SetIdInfo(GetChargeShopID());
	pChargeShopData->SetSendMask(pChargeShopData->GetUpdateMask());
}

Boolean CChargeShopItem::IsOnSaleDay()
{
	time_t nowTime;
	time(&nowTime);
	struct tm *newtime = localtime(&nowTime);
	//newtime->tm_wday 0 : sun, 1 : mon...6 : sat

	if (newtime)
	{
		if (GetLimitType() == ONE_TIME_ONLY || GetLimitType() == EVERYDAY || 
			GetLimitType() == newtime->tm_wday || (GetLimitType() == EVERY_SUNDAY && newtime->tm_wday == 0)  )
			return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}

/*void CChargeShopItem::Restore()
{
	if (GetShopType() == LIMITED && GetRestore() == 1 && GetRemain() != GetTotalStock() && GetLimitType() != ONE_TIME_ONLY)
	{
		time_t nowTime;
		time(&nowTime);
		struct tm *newtime = localtime(&nowTime);

		time_t temptime	= GetLastOrderDate();
		struct tm *lasttime = localtime(&temptime);

		if (newtime && lasttime)
		{
			if ( ( GetLimitType() == newtime->tm_wday || (GetLimitType() == EVERY_SUNDAY && newtime->tm_wday == 0) ) && newtime->tm_mday != lasttime->tm_mday)
			{
				SetRemainInfo(GetTotalStock());
				RaiseUpdate();
				m_iPreBuy = 0;
			}
		}
	}
}*/