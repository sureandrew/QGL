#pragma once
#ifndef RPC_PARTNERGROUP_ENUM
#define RPC_PARTNERGROUP_ENUM

enum RPC_PartnerGroup_Enum
{
	PartnerGroup_CLASSID	= 29000,
	RPC_PartnerGroup_ENUM_ST	= 29000,
	RPC_PartnerGroup_UP_STATE	= 29546,
	RPC_PartnerGroup_REQ_STATE	= 29435,
	RPC_PartnerGroup_DeletePartner	= 29064,
	RPC_PartnerGroup_ChangePartnerState	= 29869,
	RPC_PartnerGroup_PutToWareHouse	= 29198,
	RPC_PartnerGroup_GetFromWareHouse	= 29611,
	RPC_PartnerGroup_PartnerCultivationUpByPartner	= 29101,
	RPC_PartnerGroup_PartnerCultivationUpByItem	= 29644,
	RPC_PartnerGroup_CB_DeleteListSuccess	= 29726,
	RPC_PartnerGroup_CB_AddPartnerGroup	= 29964,
	RPC_PartnerGroup_CB_ChangePartnerState	= 29472,
	RPC_PartnerGroup_CB_AddPartner	= 29396,
	RPC_PartnerGroup_CB_UpdateAllPartner	= 29000,
	RPC_PartnerGroup_CB_AddSkills	= 29292,
	RPC_PartnerGroup_CB_WareHouse	= 29269,
	RPC_PartnerGroup_CB_PartnerCultivationUp	= 29625,
	RPC_PartnerGroup_ENUM_ED	= 29999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_PartnerGroup()
{
	RPC_ENUM_MAP[RPC_PartnerGroup_UP_STATE] = 852316315;
	RPC_ENUM_MAP[RPC_PartnerGroup_REQ_STATE] = 1535271379;
	RPC_ENUM_MAP[RPC_PartnerGroup_DeletePartner] = 377357034;
	RPC_ENUM_MAP[RPC_PartnerGroup_ChangePartnerState] = 1793207641;
	RPC_ENUM_MAP[RPC_PartnerGroup_PutToWareHouse] = 939066783;
	RPC_ENUM_MAP[RPC_PartnerGroup_GetFromWareHouse] = 1552559496;
	RPC_ENUM_MAP[RPC_PartnerGroup_PartnerCultivationUpByPartner] = 497361897;
	RPC_ENUM_MAP[RPC_PartnerGroup_PartnerCultivationUpByItem] = 540747360;
	RPC_ENUM_MAP[RPC_PartnerGroup_CB_DeleteListSuccess] = 1305766728;
	RPC_ENUM_MAP[RPC_PartnerGroup_CB_AddPartnerGroup] = 1607038446;
	RPC_ENUM_MAP[RPC_PartnerGroup_CB_ChangePartnerState] = 251283557;
	RPC_ENUM_MAP[RPC_PartnerGroup_CB_AddPartner] = 362050677;
	RPC_ENUM_MAP[RPC_PartnerGroup_CB_UpdateAllPartner] = 1209149290;
	RPC_ENUM_MAP[RPC_PartnerGroup_CB_AddSkills] = 1252151694;
	RPC_ENUM_MAP[RPC_PartnerGroup_CB_WareHouse] = 1840984601;
	RPC_ENUM_MAP[RPC_PartnerGroup_CB_PartnerCultivationUp] = 677792347;
	return 0;
}
static int _addrpc_PartnerGroup = _AddRpcEnum_PartnerGroup();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_PartnerGroup()
{
	RPC_NONBLOCK_MAP[RPC_PartnerGroup_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_PartnerGroup_CB_DeleteListSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_PartnerGroup_CB_AddPartnerGroup] = true;
	RPC_NONBLOCK_MAP[RPC_PartnerGroup_CB_ChangePartnerState] = true;
	RPC_NONBLOCK_MAP[RPC_PartnerGroup_CB_AddPartner] = true;
	RPC_NONBLOCK_MAP[RPC_PartnerGroup_CB_UpdateAllPartner] = true;
	RPC_NONBLOCK_MAP[RPC_PartnerGroup_CB_AddSkills] = true;
	RPC_NONBLOCK_MAP[RPC_PartnerGroup_CB_WareHouse] = true;
	RPC_NONBLOCK_MAP[RPC_PartnerGroup_CB_PartnerCultivationUp] = true;
	return 0;
}
static int _nonblock_PartnerGroup = _NonblockMap_PartnerGroup();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_PARTNERGROUP_ENUM
