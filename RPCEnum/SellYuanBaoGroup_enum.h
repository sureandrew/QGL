#pragma once
#ifndef RPC_SELLYUANBAOGROUP_ENUM
#define RPC_SELLYUANBAOGROUP_ENUM

enum RPC_SellYuanBaoGroup_Enum
{
	SellYuanBaoGroup_CLASSID	= 60600,
	RPC_SellYuanBaoGroup_ENUM_ST	= 60600,
	RPC_SellYuanBaoGroup_UP_STATE	= 60976,
	RPC_SellYuanBaoGroup_REQ_STATE	= 60894,
	RPC_SellYuanBaoGroup_GetTempBank	= 60771,
	RPC_SellYuanBaoGroup_CloseSellYuanBao	= 60662,
	RPC_SellYuanBaoGroup_CB_AddSellYuanBao	= 60759,
	RPC_SellYuanBaoGroup_CB_GetAllYBTAndGMoney	= 60917,
	RPC_SellYuanBaoGroup_CB_BuyYuanBao	= 60842,
	RPC_SellYuanBaoGroup_CB_RemoveSellYuanBao	= 60602,
	RPC_SellYuanBaoGroup_CB_GetTempBank	= 60620,
	RPC_SellYuanBaoGroup_ENUM_ED	= 60999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_SellYuanBaoGroup()
{
	RPC_ENUM_MAP[RPC_SellYuanBaoGroup_UP_STATE] = 1271231939;
	RPC_ENUM_MAP[RPC_SellYuanBaoGroup_REQ_STATE] = 237203230;
	RPC_ENUM_MAP[RPC_SellYuanBaoGroup_GetTempBank] = 1516584533;
	RPC_ENUM_MAP[RPC_SellYuanBaoGroup_CloseSellYuanBao] = 215442988;
	RPC_ENUM_MAP[RPC_SellYuanBaoGroup_CB_AddSellYuanBao] = 924075596;
	RPC_ENUM_MAP[RPC_SellYuanBaoGroup_CB_GetAllYBTAndGMoney] = 1365920817;
	RPC_ENUM_MAP[RPC_SellYuanBaoGroup_CB_BuyYuanBao] = 1070680463;
	RPC_ENUM_MAP[RPC_SellYuanBaoGroup_CB_RemoveSellYuanBao] = 689386043;
	RPC_ENUM_MAP[RPC_SellYuanBaoGroup_CB_GetTempBank] = 1399658407;
	return 0;
}
static int _addrpc_SellYuanBaoGroup = _AddRpcEnum_SellYuanBaoGroup();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_SellYuanBaoGroup()
{
	RPC_NONBLOCK_MAP[RPC_SellYuanBaoGroup_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_SellYuanBaoGroup_CB_AddSellYuanBao] = true;
	RPC_NONBLOCK_MAP[RPC_SellYuanBaoGroup_CB_GetAllYBTAndGMoney] = true;
	RPC_NONBLOCK_MAP[RPC_SellYuanBaoGroup_CB_BuyYuanBao] = true;
	RPC_NONBLOCK_MAP[RPC_SellYuanBaoGroup_CB_RemoveSellYuanBao] = true;
	RPC_NONBLOCK_MAP[RPC_SellYuanBaoGroup_CB_GetTempBank] = true;
	return 0;
}
static int _nonblock_SellYuanBaoGroup = _NonblockMap_SellYuanBaoGroup();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_SELLYUANBAOGROUP_ENUM
