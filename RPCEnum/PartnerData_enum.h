#pragma once
#ifndef RPC_PARTNERDATA_ENUM
#define RPC_PARTNERDATA_ENUM

enum RPC_PartnerData_Enum
{
	PartnerData_CLASSID	= 10040,
	RPC_PartnerData_ENUM_ST	= 10040,
	RPC_PartnerData_UP_STATE	= 10040,
	RPC_PartnerData_REQ_STATE	= 10049,
	RPC_PartnerData_ENUM_ED	= 10049
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_PartnerData()
{
	RPC_ENUM_MAP[RPC_PartnerData_UP_STATE] = 1536901753;
	RPC_ENUM_MAP[RPC_PartnerData_REQ_STATE] = 1682840298;
	return 0;
}
static int _addrpc_PartnerData = _AddRpcEnum_PartnerData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_PartnerData()
{
	RPC_NONBLOCK_MAP[RPC_PartnerData_UP_STATE] = true;
	return 0;
}
static int _nonblock_PartnerData = _NonblockMap_PartnerData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_PARTNERDATA_ENUM
