#pragma once
#ifndef RPC_ITEMBAG_ENUM
#define RPC_ITEMBAG_ENUM

enum RPC_ItemBag_Enum
{
	ItemBag_CLASSID	= 23000,
	RPC_ItemBag_ENUM_ST	= 23000,
	RPC_ItemBag_UP_STATE	= 23404,
	RPC_ItemBag_REQ_STATE	= 23753,
	RPC_ItemBag_MoveItem	= 23621,
	RPC_ItemBag_DropItem	= 23572,
	RPC_ItemBag_UseItem	= 23401,
	RPC_ItemBag_EquipItem	= 23268,
	RPC_ItemBag_BuyItemFromShop	= 23351,
	RPC_ItemBag_SellItemToShop	= 23393,
	RPC_ItemBag_MarkMixStartTime	= 23747,
	RPC_ItemBag_MakeItemMix	= 23185,
	RPC_ItemBag_MixItem	= 23031,
	RPC_ItemBag_IdentifyItem	= 23423,
	RPC_ItemBag_PutToWareHouse	= 23078,
	RPC_ItemBag_GetFromWareHouse	= 23843,
	RPC_ItemBag_WareHouseItemMove	= 23739,
	RPC_ItemBag_BuyItemFromBusinessShop	= 23725,
	RPC_ItemBag_SellItemToBusinessShop	= 23550,
	RPC_ItemBag_UseFastFoodItem	= 23530,
	RPC_ItemBag_Jpack	= 23748,
	RPC_ItemBag_CleanWarehouse	= 23096,
	RPC_ItemBag_CB_Error	= 23284,
	RPC_ItemBag_CB_SyncItems	= 23304,
	RPC_ItemBag_CB_ItemCount	= 23723,
	RPC_ItemBag_CB_BuytItemSuccess	= 23995,
	RPC_ItemBag_CB_SelltItemSuccess	= 23893,
	RPC_ItemBag_CB_ItemUseResult	= 23233,
	RPC_ItemBag_CB_EqItemGiven	= 23125,
	RPC_ItemBag_CB_MarkMixStartTime	= 23625,
	RPC_ItemBag_CB_MakeItemMix	= 23390,
	RPC_ItemBag_CB_MixItem	= 23157,
	RPC_ItemBag_CB_IdentifyItem	= 23620,
	RPC_ItemBag_CB_WareHouse	= 23688,
	RPC_ItemBag_CB_BuyItemFromBusinessShop	= 23107,
	RPC_ItemBag_CB_SellItemToBusinessShop	= 23072,
	RPC_ItemBag_ENUM_ED	= 23999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_ItemBag()
{
	RPC_ENUM_MAP[RPC_ItemBag_UP_STATE] = 613501223;
	RPC_ENUM_MAP[RPC_ItemBag_REQ_STATE] = 177550296;
	RPC_ENUM_MAP[RPC_ItemBag_MoveItem] = 1746226056;
	RPC_ENUM_MAP[RPC_ItemBag_DropItem] = 1515401908;
	RPC_ENUM_MAP[RPC_ItemBag_UseItem] = 2141142025;
	RPC_ENUM_MAP[RPC_ItemBag_EquipItem] = 531775588;
	RPC_ENUM_MAP[RPC_ItemBag_BuyItemFromShop] = 842424956;
	RPC_ENUM_MAP[RPC_ItemBag_SellItemToShop] = 290469063;
	RPC_ENUM_MAP[RPC_ItemBag_MarkMixStartTime] = 302457678;
	RPC_ENUM_MAP[RPC_ItemBag_MakeItemMix] = 71172279;
	RPC_ENUM_MAP[RPC_ItemBag_MixItem] = 2144039645;
	RPC_ENUM_MAP[RPC_ItemBag_IdentifyItem] = 1295601119;
	RPC_ENUM_MAP[RPC_ItemBag_PutToWareHouse] = 135143194;
	RPC_ENUM_MAP[RPC_ItemBag_GetFromWareHouse] = 844128572;
	RPC_ENUM_MAP[RPC_ItemBag_WareHouseItemMove] = 1090149289;
	RPC_ENUM_MAP[RPC_ItemBag_BuyItemFromBusinessShop] = 2062907003;
	RPC_ENUM_MAP[RPC_ItemBag_SellItemToBusinessShop] = 1333754869;
	RPC_ENUM_MAP[RPC_ItemBag_UseFastFoodItem] = 631398010;
	RPC_ENUM_MAP[RPC_ItemBag_Jpack] = 1894130665;
	RPC_ENUM_MAP[RPC_ItemBag_CleanWarehouse] = 295327115;
	RPC_ENUM_MAP[RPC_ItemBag_CB_Error] = 1647133532;
	RPC_ENUM_MAP[RPC_ItemBag_CB_SyncItems] = 1349998161;
	RPC_ENUM_MAP[RPC_ItemBag_CB_ItemCount] = 1355561297;
	RPC_ENUM_MAP[RPC_ItemBag_CB_BuytItemSuccess] = 645411917;
	RPC_ENUM_MAP[RPC_ItemBag_CB_SelltItemSuccess] = 950555533;
	RPC_ENUM_MAP[RPC_ItemBag_CB_ItemUseResult] = 1372525941;
	RPC_ENUM_MAP[RPC_ItemBag_CB_EqItemGiven] = 1096832056;
	RPC_ENUM_MAP[RPC_ItemBag_CB_MarkMixStartTime] = 1348863082;
	RPC_ENUM_MAP[RPC_ItemBag_CB_MakeItemMix] = 1631455886;
	RPC_ENUM_MAP[RPC_ItemBag_CB_MixItem] = 1489919274;
	RPC_ENUM_MAP[RPC_ItemBag_CB_IdentifyItem] = 1449733307;
	RPC_ENUM_MAP[RPC_ItemBag_CB_WareHouse] = 1716606032;
	RPC_ENUM_MAP[RPC_ItemBag_CB_BuyItemFromBusinessShop] = 1999982718;
	RPC_ENUM_MAP[RPC_ItemBag_CB_SellItemToBusinessShop] = 1634229568;
	return 0;
}
static int _addrpc_ItemBag = _AddRpcEnum_ItemBag();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_ItemBag()
{
	RPC_NONBLOCK_MAP[RPC_ItemBag_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_Error] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_SyncItems] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_ItemCount] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_BuytItemSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_SelltItemSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_ItemUseResult] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_EqItemGiven] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_MarkMixStartTime] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_MakeItemMix] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_MixItem] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_IdentifyItem] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_WareHouse] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_BuyItemFromBusinessShop] = true;
	RPC_NONBLOCK_MAP[RPC_ItemBag_CB_SellItemToBusinessShop] = true;
	return 0;
}
static int _nonblock_ItemBag = _NonblockMap_ItemBag();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_ITEMBAG_ENUM
