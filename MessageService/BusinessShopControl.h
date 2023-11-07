#pragma once
#ifndef _BUSINESSSHOP_CONTROL_H_
#define _BUSINESSSHOP_CONTROL_H_
#include <Reuben/Utility/Singleton.h>
#include "Common/common_binary.h"

typedef StlMap<UInt32, BusinessShopItemInfo *, std::less<UInt32>, 
ManagedAllocator<std::pair<UInt32, BusinessShopItemInfo *> > > BusinessShopItemList;

struct BusinessShop 
{
	//bool					sell;//whether can sell Business item to this shop
	UInt8					shopName;
	UInt16					shop_id;
	BusinessShopItemList	shopItemList;
};
typedef StlMap<UInt16, BusinessShop *, std::less<UInt16>, 
ManagedAllocator<std::pair<UInt16, BusinessShop *> > > BusinessShopList;

class CBusinessShopControl
	: public Reuben::Utility::Singleton<CBusinessShopControl>
{
public:
	CBusinessShopControl();
	~CBusinessShopControl();

	void Initialize();
	void OnUpdate(UInt32 uTime);
	bool OpenBusinessShopMenu(LPCPEER pPeer, RPCContext &context, UInt32 shop_id, UInt32 money);
	void BuyItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId, const UInt16 &count, const UInt32 &money);
	void SellItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId, const UInt16 &count);
	void UpdateBusinessShopList(LPCPEER pPeer, RPCContext &context, const Int16 &shopId);
private:
	void UpdateSellPrice();
	void UpdateBuyAndSellPrice();
	BusinessShopList	m_BusinessShopList;
	UInt32				m_updateSellTime;
	UInt32				m_updateBuyAndSellTime;
};
#endif // _BUSINESSSHOP_CONTROL_H_