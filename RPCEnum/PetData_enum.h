#pragma once
#ifndef RPC_PETDATA_ENUM
#define RPC_PETDATA_ENUM

enum RPC_PetData_Enum
{
	PetData_CLASSID	= 10120,
	RPC_PetData_ENUM_ST	= 10120,
	RPC_PetData_UP_STATE	= 10120,
	RPC_PetData_REQ_STATE	= 10129,
	RPC_PetData_ENUM_ED	= 10129
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_PetData()
{
	RPC_ENUM_MAP[RPC_PetData_UP_STATE] = 1033896416;
	RPC_ENUM_MAP[RPC_PetData_REQ_STATE] = 979073569;
	return 0;
}
static int _addrpc_PetData = _AddRpcEnum_PetData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_PetData()
{
	RPC_NONBLOCK_MAP[RPC_PetData_UP_STATE] = true;
	return 0;
}
static int _nonblock_PetData = _NonblockMap_PetData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_PETDATA_ENUM
