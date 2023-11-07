#pragma once
#ifndef RPC_GENERATIONDATA_ENUM
#define RPC_GENERATIONDATA_ENUM

enum RPC_GenerationData_Enum
{
	GenerationData_CLASSID	= 10330,
	RPC_GenerationData_ENUM_ST	= 10330,
	RPC_GenerationData_UP_STATE	= 10330,
	RPC_GenerationData_REQ_STATE	= 10334,
	RPC_GenerationData_ENUM_ED	= 10339
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_GenerationData()
{
	RPC_ENUM_MAP[RPC_GenerationData_UP_STATE] = 2008179101;
	RPC_ENUM_MAP[RPC_GenerationData_REQ_STATE] = 776623162;
	return 0;
}
static int _addrpc_GenerationData = _AddRpcEnum_GenerationData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_GenerationData()
{
	RPC_NONBLOCK_MAP[RPC_GenerationData_UP_STATE] = true;
	return 0;
}
static int _nonblock_GenerationData = _NonblockMap_GenerationData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_GENERATIONDATA_ENUM
