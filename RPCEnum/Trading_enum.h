#pragma once
#ifndef RPC_TRADING_ENUM
#define RPC_TRADING_ENUM

enum RPC_Trading_Enum
{
	Trading_CLASSID	= 39000,
	RPC_Trading_ENUM_ST	= 39000,
	RPC_Trading_UP_STATE	= 39393,
	RPC_Trading_REQ_STATE	= 39246,
	RPC_Trading_SetTradingItemSlot	= 39543,
	RPC_Trading_RemoveTradingItemSlot	= 39669,
	RPC_Trading_SetTradingPartnerSlot	= 39396,
	RPC_Trading_RemoveTradingPartnerSlot	= 39679,
	RPC_Trading_SetTradingPetSlot	= 39237,
	RPC_Trading_RemoveTradingPetSlot	= 39664,
	RPC_Trading_SetTradingLock	= 39153,
	RPC_Trading_SetTradingMoney	= 39452,
	RPC_Trading_CancelTrading	= 39991,
	RPC_Trading_ConfirmTrading	= 39706,
	RPC_Trading_CB_SetItemSlot	= 39696,
	RPC_Trading_CB_RemoveItemSlot	= 39951,
	RPC_Trading_CB_SetPartnerSlot	= 39613,
	RPC_Trading_CB_RemovePartnerSlot	= 39832,
	RPC_Trading_CB_SetPetSlot	= 39977,
	RPC_Trading_CB_RemovePetSlot	= 39001,
	RPC_Trading_CB_SetLock	= 39792,
	RPC_Trading_CB_SetMoney	= 39794,
	RPC_Trading_CB_CancelTrading	= 39412,
	RPC_Trading_CB_ConfirmTrading	= 39164,
	RPC_Trading_CB_TradingProcess	= 39752,
	RPC_Trading_CB_TradingItem	= 39521,
	RPC_Trading_CB_TradingPartner	= 39344,
	RPC_Trading_CB_TradingPet	= 39840,
	RPC_Trading_CB_TradingMoney	= 39751,
	RPC_Trading_ENUM_ED	= 39999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Trading()
{
	RPC_ENUM_MAP[RPC_Trading_UP_STATE] = 959381602;
	RPC_ENUM_MAP[RPC_Trading_REQ_STATE] = 523592870;
	RPC_ENUM_MAP[RPC_Trading_SetTradingItemSlot] = 1871933618;
	RPC_ENUM_MAP[RPC_Trading_RemoveTradingItemSlot] = 41514946;
	RPC_ENUM_MAP[RPC_Trading_SetTradingPartnerSlot] = 133844652;
	RPC_ENUM_MAP[RPC_Trading_RemoveTradingPartnerSlot] = 1111360652;
	RPC_ENUM_MAP[RPC_Trading_SetTradingPetSlot] = 874194450;
	RPC_ENUM_MAP[RPC_Trading_RemoveTradingPetSlot] = 1081767140;
	RPC_ENUM_MAP[RPC_Trading_SetTradingLock] = 271393074;
	RPC_ENUM_MAP[RPC_Trading_SetTradingMoney] = 1330329215;
	RPC_ENUM_MAP[RPC_Trading_CancelTrading] = 143218057;
	RPC_ENUM_MAP[RPC_Trading_ConfirmTrading] = 896626933;
	RPC_ENUM_MAP[RPC_Trading_CB_SetItemSlot] = 2134127773;
	RPC_ENUM_MAP[RPC_Trading_CB_RemoveItemSlot] = 20530320;
	RPC_ENUM_MAP[RPC_Trading_CB_SetPartnerSlot] = 1743412576;
	RPC_ENUM_MAP[RPC_Trading_CB_RemovePartnerSlot] = 130701276;
	RPC_ENUM_MAP[RPC_Trading_CB_SetPetSlot] = 293800319;
	RPC_ENUM_MAP[RPC_Trading_CB_RemovePetSlot] = 1745489520;
	RPC_ENUM_MAP[RPC_Trading_CB_SetLock] = 164179813;
	RPC_ENUM_MAP[RPC_Trading_CB_SetMoney] = 799174000;
	RPC_ENUM_MAP[RPC_Trading_CB_CancelTrading] = 85228765;
	RPC_ENUM_MAP[RPC_Trading_CB_ConfirmTrading] = 633995943;
	RPC_ENUM_MAP[RPC_Trading_CB_TradingProcess] = 88889239;
	RPC_ENUM_MAP[RPC_Trading_CB_TradingItem] = 315894103;
	RPC_ENUM_MAP[RPC_Trading_CB_TradingPartner] = 1744515603;
	RPC_ENUM_MAP[RPC_Trading_CB_TradingPet] = 1410363786;
	RPC_ENUM_MAP[RPC_Trading_CB_TradingMoney] = 672272129;
	return 0;
}
static int _addrpc_Trading = _AddRpcEnum_Trading();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Trading()
{
	RPC_NONBLOCK_MAP[RPC_Trading_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_SetItemSlot] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_RemoveItemSlot] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_SetPartnerSlot] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_RemovePartnerSlot] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_SetPetSlot] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_RemovePetSlot] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_SetLock] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_SetMoney] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_CancelTrading] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_ConfirmTrading] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_TradingProcess] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_TradingItem] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_TradingPartner] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_TradingPet] = true;
	RPC_NONBLOCK_MAP[RPC_Trading_CB_TradingMoney] = true;
	return 0;
}
static int _nonblock_Trading = _NonblockMap_Trading();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_TRADING_ENUM
