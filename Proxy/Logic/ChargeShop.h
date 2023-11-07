#pragma once
#ifndef _CHARGESHOP_H_
#define _CHARGESHOP_H_

#include "Proxy/Logic/ChargeShop_Proxy.h"

class CChargeShop : public ChargeShop_Proxy
{
public:
	DEFINE_CLASS(ChargeShop_CLASSID);
	UInt32 GetTotalShopItem() { return GetCount(); }

private:
	typedef ChargeShop_Proxy Parent;
// OnUpdate can be implemented selectively

	virtual RPCResult CB_BuyItemInChargeShop(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode, const UInt32 &chargeItemID, const UInt8 &itemType, const UInt32 &stack, const UInt32 &yuanbao);
	
};

#endif //_CHARGESHOP_H_
