//-- Common
#include "Common.h"
#include "Common/DataCode.h"
//-- Self

#include "BusinessShopControl.h"
#include "Global.h"
#include "Common/common_binary.h"
#include "Resource/ResBusinessShop.h"
#include "Resource/ServerResManager.h"
#include "Stub/Service/MessageService.h"

#define	BUSINESSSHOP_UPDATE_TIME	(1000 * 60 * 10)

CBusinessShopControl::CBusinessShopControl()
{
	m_updateBuyAndSellTime = GetTickCount() + 1000 * 10;//delay 10 s
	m_updateSellTime = m_updateBuyAndSellTime + (BUSINESSSHOP_UPDATE_TIME >> 1);//after 5 min later
}

CBusinessShopControl::~CBusinessShopControl()
{

}

void CBusinessShopControl::BuyItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId,
								   const UInt16 &count, const UInt32 &money)
{
	String errMsg;
	do
	{
		BusinessShopList::iterator IterShop = m_BusinessShopList.find(shopId);
		if (IterShop == m_BusinessShopList.end())
		{
			errMsg = _T("Can not find the BusinessShop id");
			break;
		}

		BusinessShop * pBusinessShop = IterShop->second;
		if (!pBusinessShop)
		{
			errMsg = _T("BusinessShop invalid");
			break;
		}

		BusinessShopItemList::iterator IterItem = pBusinessShop->shopItemList.find(itemId);
		if (IterItem == pBusinessShop->shopItemList.end())
		{
			errMsg = _T("Can not find the item id in the BusinessShop");
			break;
		}

		BusinessShopItemInfo * pBusinessShopItem = IterItem->second;
		if (!pBusinessShopItem)
		{
			errMsg = _T("the item invalid");
			break;
		}
		if (0 == pBusinessShopItem->buyPrice)
		{
			errMsg = _T("the item not provide");
			break;
		}
		if (pBusinessShopItem->count == 0)
		{
			errMsg = _T("the item count is 0");
			break;
		}
		if (pBusinessShopItem->count < count)
		{
			errMsg = _T("the item count is less than you want to buy");
			break;
		}

		UInt16 countCanbuy = money / pBusinessShopItem->buyPrice;
		if (countCanbuy == 0 || countCanbuy < count)
		{
			errMsg = _T("Not enough money to buy");
			break;
		}

		pBusinessShopItem->count -= count;
		MessageService_Stub::CB_BuyItemFromBusinessShop(pPeer, context,true, count, count * pBusinessShopItem->buyPrice);
	}
	while (false);

	if (!errMsg.IsEmpty())
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("BuyItem Warning: %s"), errMsg.c_str());
		MessageService_Stub::CB_BuyItemFromBusinessShop(pPeer, context,false, 0, 0);
	}
}
void CBusinessShopControl::SellItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId, const UInt16 &count)
{
	String errMsg;
	do
	{
		BusinessShopList::iterator IterShop = m_BusinessShopList.find(shopId);
		if (IterShop == m_BusinessShopList.end())
		{
			errMsg = _T("Can not find the BusinessShop id");
			break;
		}
		BusinessShop * pBusinessShop = IterShop->second;
		if (!pBusinessShop)
		{
			errMsg = _T("BusinessShop invalid");
			break;
		}

		//if (!pBusinessShop->sell)
		//{
		//	errMsg = _T("can't sell to this shop ,only buy item from it");
		//	break;
		//}

		BusinessShopItemList::iterator IterItem = pBusinessShop->shopItemList.find(itemId);
		if (IterItem == pBusinessShop->shopItemList.end())
		{
			errMsg = _T("Can not find the item id in the BusinessShop");
			break;
		}

		BusinessShopItemInfo * pBusinessShopItem = IterItem->second;
		if (!pBusinessShopItem)
		{
			errMsg = _T("the item invalid");
			break;
		}

		MessageService_Stub::CB_SellItemToBusinessShop(pPeer, context,true, pBusinessShopItem->sellPrice * count);
	}
	while (false);

	if (!errMsg.IsEmpty())
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("BuyItem Warning: %s"), errMsg.c_str());
		MessageService_Stub::CB_SellItemToBusinessShop(pPeer, context,false, 0);
	}
}

bool CBusinessShopControl::OpenBusinessShopMenu(LPCPEER pPeer, RPCContext &context, UInt32 shop_id, UInt32 money)
{
	BusinessShopList::iterator Iter = m_BusinessShopList.find(shop_id);
	if (Iter != m_BusinessShopList.end())
	{
		BusinessShop *pBusinessShop = Iter->second;
		BusinessShopItemInfoVector	ItemInfoList;
		if (pBusinessShop)
		{
			BusinessShopItemList::iterator Iter2 = pBusinessShop->shopItemList.begin();
			while (Iter2 != pBusinessShop->shopItemList.end())
			{
				ItemInfoList.push_back(*Iter2->second);
				++Iter2;
			}
			MessageService_Stub::CB_OpenBusinessShopMenu(pPeer, context,
				BOOLEAN_TRUE, shop_id, pBusinessShop->shopName,
				MIN(m_updateSellTime, m_updateBuyAndSellTime) - GetTickCount(), money, ItemInfoList);
			return true;
		}
	}
	return false;
}

void CBusinessShopControl::UpdateSellPrice()
{
	TRACE_INFO(GLOBAL_LOGGER,_F("UpdateSellPrice\n"));
	BusinessShopDataMap* pBusinessShopDataMap = gGlobal.m_resource->GetBusinessShopDataMap();
	if (pBusinessShopDataMap)
	{
		BusinessShopDataMap::iterator Iter = pBusinessShopDataMap->begin();
		while (Iter != pBusinessShopDataMap->end())
		{
			BusinessShopData * pBusinessShopData = Iter->second;
			if (pBusinessShopData)
			{
				BusinessShopList::iterator	IterShopLst = m_BusinessShopList.find(pBusinessShopData->shop_id);
				if (IterShopLst != m_BusinessShopList.end())
				{
					BusinessShop *pBusinessShop = IterShopLst->second;
					if (pBusinessShop)
					{
						for (UInt8 i=0; i<pBusinessShopData->numOfItem; i++)
						{
							BusinessShopItemList::iterator IterBusinessShopItemList = pBusinessShop->shopItemList.find(pBusinessShopData->item_idLst[i]);
							if (IterBusinessShopItemList != pBusinessShop->shopItemList.end())
							{
								BusinessShopItemInfo *pBusinessShopItem = IterBusinessShopItemList->second;
								if (pBusinessShopItem)
								{
									if (pBusinessShopData->countLst[i] > 0)
									{
										pBusinessShopItem->sellPrice	= GetRand((int)pBusinessShopData->minPriceLst[i],(int)pBusinessShopData->maxPriceLst[i]) >> 2;// 1/4 of the buy price
									} 
									else
									{
										pBusinessShopItem->sellPrice	= GetRand((int)pBusinessShopData->minPriceLst[i],(int)pBusinessShopData->maxPriceLst[i]);
									}
								}
							}
						}
					}
				}
			}
			++Iter;
		}
	}
}

void CBusinessShopControl::UpdateBuyAndSellPrice()
{
	TRACE_INFO(GLOBAL_LOGGER,_F("UpdateBuyAndSellPrice\n"));
	BusinessShopDataMap* pBusinessShopDataMap = gGlobal.m_resource->GetBusinessShopDataMap();
	if (pBusinessShopDataMap)
	{
		BusinessShopDataMap::iterator Iter = pBusinessShopDataMap->begin();
		while (Iter != pBusinessShopDataMap->end())
		{
			BusinessShopData * pBusinessShopData = Iter->second;
			if (pBusinessShopData)
			{
				BusinessShopList::iterator	IterShopLst = m_BusinessShopList.find(pBusinessShopData->shop_id);
				if (IterShopLst != m_BusinessShopList.end())
				{
					BusinessShop *pBusinessShop = IterShopLst->second;
					if (pBusinessShop)
					{
						for (UInt8 i=0; i<pBusinessShopData->numOfItem; i++)
						{
							BusinessShopItemList::iterator IterBusinessShopItemList = pBusinessShop->shopItemList.find(pBusinessShopData->item_idLst[i]);
							if (IterBusinessShopItemList != pBusinessShop->shopItemList.end())
							{
								BusinessShopItemInfo *pBusinessShopItem = IterBusinessShopItemList->second;
								if (pBusinessShopItem)
								{
									if (pBusinessShopData->countLst[i] > 0)
									{
										pBusinessShopItem->count		= pBusinessShopData->countLst[i];
										pBusinessShopItem->buyPrice		= GetRand((int)pBusinessShopData->minPriceLst[i],(int)pBusinessShopData->maxPriceLst[i]);
										pBusinessShopItem->sellPrice	= pBusinessShopItem->buyPrice >> 2;// 1/4 of the buy price
									} 
									else
									{
										pBusinessShopItem->sellPrice	= GetRand((int)pBusinessShopData->minPriceLst[i],(int)pBusinessShopData->maxPriceLst[i]);
									}
								}
							}
						}
					}
				}
			}
			++Iter;
		}
	}
}

void CBusinessShopControl::OnUpdate(UInt32 uTime)
{
	if (uTime > m_updateSellTime)// update sell price,each 10 min
	{
		UpdateSellPrice();
		m_updateSellTime = uTime + BUSINESSSHOP_UPDATE_TIME;
	}
	if (uTime > m_updateBuyAndSellTime)// update buy price,each 10 min
	{
		UpdateBuyAndSellPrice();
		m_updateBuyAndSellTime = uTime + BUSINESSSHOP_UPDATE_TIME;
	}
}

void CBusinessShopControl::Initialize()
{
	BusinessShopDataMap* pBusinessShopDataMap = gGlobal.m_resource->GetBusinessShopDataMap();
	if (pBusinessShopDataMap)
	{
		BusinessShopDataMap::iterator Iter = pBusinessShopDataMap->begin();
		while (Iter != pBusinessShopDataMap->end())
		{
			BusinessShopData * pBusinessShopData = Iter->second;
			if (pBusinessShopData)
			{
				BusinessShop *pBusinessShop = SafeCreateObject(BusinessShop);
				pBusinessShop->shop_id	= pBusinessShopData->shop_id;
				pBusinessShop->shopName	= pBusinessShopData->shopName;
				for (UInt8 i=0; i<pBusinessShopData->numOfItem; i++)
				{
					BusinessShopItemInfo *pBusinessShopItem = SafeCreateObject(BusinessShopItemInfo);
					pBusinessShopItem->item_id		= pBusinessShopData->item_idLst[i];
					if (pBusinessShopData->countLst[i] > 0)
					{
						pBusinessShopItem->count		= pBusinessShopData->countLst[i];
						pBusinessShopItem->buyPrice		= GetRand((int)pBusinessShopData->minPriceLst[i],(int)pBusinessShopData->maxPriceLst[i]);
						pBusinessShopItem->sellPrice	= pBusinessShopItem->buyPrice >> 2;// 1/4 of the buy price
					} 
					else
					{
						pBusinessShopItem->count		= 0;
						pBusinessShopItem->buyPrice		= 0;
						pBusinessShopItem->sellPrice	= GetRand((int)pBusinessShopData->minPriceLst[i],(int)pBusinessShopData->maxPriceLst[i]);
					}
					pBusinessShop->shopItemList.insert(std::make_pair(pBusinessShopItem->item_id,pBusinessShopItem));
				}
				m_BusinessShopList[pBusinessShop->shop_id] = pBusinessShop;
			}

			++Iter;
		}
	}
}

void CBusinessShopControl::UpdateBusinessShopList(LPCPEER pPeer, RPCContext &context, const Int16 &shopId)
{
	if (pPeer)
	{
		BusinessShopList::iterator Iter = m_BusinessShopList.find(shopId);
		if (Iter != m_BusinessShopList.end())
		{
			BusinessShop *pBusinessShop = Iter->second;
			BusinessShopItemInfoVector	ItemInfoList;
			if (pBusinessShop)
			{
				BusinessShopItemList::iterator Iter2 = pBusinessShop->shopItemList.begin();
				while (Iter2 != pBusinessShop->shopItemList.end())
				{
					ItemInfoList.push_back(*(BusinessShopItemInfo *)Iter2->second);
					++Iter2;
				}
				MessageService_Stub::CB_UpdateBusinessShopList(pPeer, context, ItemInfoList);
				return;
			}
		}
	}
}