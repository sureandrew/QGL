#pragma once
#ifndef RPC_CHARGESHOPDATA_ENUM
#define RPC_CHARGESHOPDATA_ENUM

enum RPC_ChargeShopData_Enum
{
	ChargeShopData_CLASSID	= 10110,
	RPC_ChargeShopData_ENUM_ST	= 10110,
	RPC_ChargeShopData_UP_STATE	= 10111,
	RPC_ChargeShopData_REQ_STATE	= 10115,
	RPC_ChargeShopData_ENUM_ED	= 10119
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_ChargeShopData()
{
	RPC_ENUM_MAP[RPC_ChargeShopData_UP_STATE] = 1620454845;
	RPC_ENUM_MAP[RPC_ChargeShopData_REQ_STATE] = 1609197875;
	return 0;
}
static int _addrpc_ChargeShopData = _AddRpcEnum_ChargeShopData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_ChargeShopData()
{
	RPC_NONBLOCK_MAP[RPC_ChargeShopData_UP_STATE] = true;
	return 0;
}
static int _nonblock_ChargeShopData = _NonblockMap_ChargeShopData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_CHARGESHOPDATA_ENUM
