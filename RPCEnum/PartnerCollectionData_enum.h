#pragma once
#ifndef RPC_PARTNERCOLLECTIONDATA_ENUM
#define RPC_PARTNERCOLLECTIONDATA_ENUM

enum RPC_PartnerCollectionData_Enum
{
	PartnerCollectionData_CLASSID	= 10300,
	RPC_PartnerCollectionData_ENUM_ST	= 10300,
	RPC_PartnerCollectionData_UP_STATE	= 10307,
	RPC_PartnerCollectionData_REQ_STATE	= 10303,
	RPC_PartnerCollectionData_ENUM_ED	= 10309
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_PartnerCollectionData()
{
	RPC_ENUM_MAP[RPC_PartnerCollectionData_UP_STATE] = 1650458909;
	RPC_ENUM_MAP[RPC_PartnerCollectionData_REQ_STATE] = 2041345554;
	return 0;
}
static int _addrpc_PartnerCollectionData = _AddRpcEnum_PartnerCollectionData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_PartnerCollectionData()
{
	RPC_NONBLOCK_MAP[RPC_PartnerCollectionData_UP_STATE] = true;
	return 0;
}
static int _nonblock_PartnerCollectionData = _NonblockMap_PartnerCollectionData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_PARTNERCOLLECTIONDATA_ENUM
