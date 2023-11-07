#pragma once
#ifndef RPC_SELLYUANBAO_ENUM
#define RPC_SELLYUANBAO_ENUM

enum RPC_SellYuanBao_Enum
{
	SellYuanBao_CLASSID	= 60000,
	RPC_SellYuanBao_ENUM_ST	= 60000,
	RPC_SellYuanBao_UP_STATE	= 60191,
	RPC_SellYuanBao_REQ_STATE	= 60297,
	RPC_SellYuanBao_ENUM_ED	= 60599
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_SellYuanBao()
{
	RPC_ENUM_MAP[RPC_SellYuanBao_UP_STATE] = 1102668347;
	RPC_ENUM_MAP[RPC_SellYuanBao_REQ_STATE] = 1386153849;
	return 0;
}
static int _addrpc_SellYuanBao = _AddRpcEnum_SellYuanBao();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_SellYuanBao()
{
	RPC_NONBLOCK_MAP[RPC_SellYuanBao_UP_STATE] = true;
	return 0;
}
static int _nonblock_SellYuanBao = _NonblockMap_SellYuanBao();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_SELLYUANBAO_ENUM
