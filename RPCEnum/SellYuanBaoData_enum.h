#pragma once
#ifndef RPC_SELLYUANBAODATA_ENUM
#define RPC_SELLYUANBAODATA_ENUM

enum RPC_SellYuanBaoData_Enum
{
	SellYuanBaoData_CLASSID	= 10200,
	RPC_SellYuanBaoData_ENUM_ST	= 10200,
	RPC_SellYuanBaoData_UP_STATE	= 10248,
	RPC_SellYuanBaoData_REQ_STATE	= 10226,
	RPC_SellYuanBaoData_ENUM_ED	= 10299
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_SellYuanBaoData()
{
	RPC_ENUM_MAP[RPC_SellYuanBaoData_UP_STATE] = 515912530;
	RPC_ENUM_MAP[RPC_SellYuanBaoData_REQ_STATE] = 588066567;
	return 0;
}
static int _addrpc_SellYuanBaoData = _AddRpcEnum_SellYuanBaoData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_SellYuanBaoData()
{
	RPC_NONBLOCK_MAP[RPC_SellYuanBaoData_UP_STATE] = true;
	return 0;
}
static int _nonblock_SellYuanBaoData = _NonblockMap_SellYuanBaoData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_SELLYUANBAODATA_ENUM
