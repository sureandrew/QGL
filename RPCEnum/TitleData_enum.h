#pragma once
#ifndef RPC_TITLEDATA_ENUM
#define RPC_TITLEDATA_ENUM

enum RPC_TitleData_Enum
{
	TitleData_CLASSID	= 10130,
	RPC_TitleData_ENUM_ST	= 10130,
	RPC_TitleData_UP_STATE	= 10137,
	RPC_TitleData_REQ_STATE	= 10132,
	RPC_TitleData_ENUM_ED	= 10139
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_TitleData()
{
	RPC_ENUM_MAP[RPC_TitleData_UP_STATE] = 432293902;
	RPC_ENUM_MAP[RPC_TitleData_REQ_STATE] = 661741758;
	return 0;
}
static int _addrpc_TitleData = _AddRpcEnum_TitleData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_TitleData()
{
	RPC_NONBLOCK_MAP[RPC_TitleData_UP_STATE] = true;
	return 0;
}
static int _nonblock_TitleData = _NonblockMap_TitleData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_TITLEDATA_ENUM
