#pragma once
#ifndef RPC_CHARGESHOPITEM_ENUM
#define RPC_CHARGESHOPITEM_ENUM

enum RPC_ChargeShopItem_Enum
{
	ChargeShopItem_CLASSID	= 42000,
	RPC_ChargeShopItem_ENUM_ST	= 42000,
	RPC_ChargeShopItem_UP_STATE	= 42268,
	RPC_ChargeShopItem_REQ_STATE	= 42181,
	RPC_ChargeShopItem_ENUM_ED	= 42999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_ChargeShopItem()
{
	RPC_ENUM_MAP[RPC_ChargeShopItem_UP_STATE] = 596444539;
	RPC_ENUM_MAP[RPC_ChargeShopItem_REQ_STATE] = 474165627;
	return 0;
}
static int _addrpc_ChargeShopItem = _AddRpcEnum_ChargeShopItem();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_ChargeShopItem()
{
	RPC_NONBLOCK_MAP[RPC_ChargeShopItem_UP_STATE] = true;
	return 0;
}
static int _nonblock_ChargeShopItem = _NonblockMap_ChargeShopItem();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_CHARGESHOPITEM_ENUM
