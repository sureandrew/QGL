#pragma once
#ifndef RPC_CHARGESHOP_ENUM
#define RPC_CHARGESHOP_ENUM

enum RPC_ChargeShop_Enum
{
	ChargeShop_CLASSID	= 41000,
	RPC_ChargeShop_ENUM_ST	= 41000,
	RPC_ChargeShop_UP_STATE	= 41255,
	RPC_ChargeShop_REQ_STATE	= 41145,
	RPC_ChargeShop_BuyItemInChargeShop	= 41477,
	RPC_ChargeShop_CB_BuyItemInChargeShop	= 41021,
	RPC_ChargeShop_ENUM_ED	= 41899
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_ChargeShop()
{
	RPC_ENUM_MAP[RPC_ChargeShop_UP_STATE] = 1096633362;
	RPC_ENUM_MAP[RPC_ChargeShop_REQ_STATE] = 1033780252;
	RPC_ENUM_MAP[RPC_ChargeShop_BuyItemInChargeShop] = 4988591;
	RPC_ENUM_MAP[RPC_ChargeShop_CB_BuyItemInChargeShop] = 1677995046;
	return 0;
}
static int _addrpc_ChargeShop = _AddRpcEnum_ChargeShop();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_ChargeShop()
{
	RPC_NONBLOCK_MAP[RPC_ChargeShop_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_ChargeShop_CB_BuyItemInChargeShop] = true;
	return 0;
}
static int _nonblock_ChargeShop = _NonblockMap_ChargeShop();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_CHARGESHOP_ENUM
