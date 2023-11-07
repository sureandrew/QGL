#pragma once
#ifndef RPC_HAWKGROUP_ENUM
#define RPC_HAWKGROUP_ENUM

enum RPC_HawkGroup_Enum
{
	HawkGroup_CLASSID	= 47000,
	RPC_HawkGroup_ENUM_ST	= 47000,
	RPC_HawkGroup_UP_STATE	= 47036,
	RPC_HawkGroup_REQ_STATE	= 47384,
	RPC_HawkGroup_BuyHawkItem	= 47430,
	RPC_HawkGroup_CB_UpdateHawkItem	= 47837,
	RPC_HawkGroup_CB_BuyHawkItemSuccess	= 47642,
	RPC_HawkGroup_ChangeHawkName	= 47119,
	RPC_HawkGroup_CB_ChangeHawkName	= 47929,
	RPC_HawkGroup_CloseHawkForUser	= 47132,
	RPC_HawkGroup_CB_CloseHawkForUser	= 47447,
	RPC_HawkGroup_StopHawk	= 47286,
	RPC_HawkGroup_CB_StopHawk	= 47974,
	RPC_HawkGroup_CB_AddSellStartHawkItemInfo	= 47641,
	RPC_HawkGroup_ENUM_ED	= 47999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_HawkGroup()
{
	RPC_ENUM_MAP[RPC_HawkGroup_UP_STATE] = 408894990;
	RPC_ENUM_MAP[RPC_HawkGroup_REQ_STATE] = 677989840;
	RPC_ENUM_MAP[RPC_HawkGroup_BuyHawkItem] = 1007042140;
	RPC_ENUM_MAP[RPC_HawkGroup_CB_UpdateHawkItem] = 665529049;
	RPC_ENUM_MAP[RPC_HawkGroup_CB_BuyHawkItemSuccess] = 435233807;
	RPC_ENUM_MAP[RPC_HawkGroup_ChangeHawkName] = 1431323677;
	RPC_ENUM_MAP[RPC_HawkGroup_CB_ChangeHawkName] = 569115728;
	RPC_ENUM_MAP[RPC_HawkGroup_CloseHawkForUser] = 605962040;
	RPC_ENUM_MAP[RPC_HawkGroup_CB_CloseHawkForUser] = 1991732788;
	RPC_ENUM_MAP[RPC_HawkGroup_StopHawk] = 506137941;
	RPC_ENUM_MAP[RPC_HawkGroup_CB_StopHawk] = 189008725;
	RPC_ENUM_MAP[RPC_HawkGroup_CB_AddSellStartHawkItemInfo] = 1838372492;
	return 0;
}
static int _addrpc_HawkGroup = _AddRpcEnum_HawkGroup();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_HawkGroup()
{
	RPC_NONBLOCK_MAP[RPC_HawkGroup_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_HawkGroup_CB_UpdateHawkItem] = true;
	RPC_NONBLOCK_MAP[RPC_HawkGroup_CB_BuyHawkItemSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_HawkGroup_CB_ChangeHawkName] = true;
	RPC_NONBLOCK_MAP[RPC_HawkGroup_CB_CloseHawkForUser] = true;
	RPC_NONBLOCK_MAP[RPC_HawkGroup_CB_StopHawk] = true;
	RPC_NONBLOCK_MAP[RPC_HawkGroup_CB_AddSellStartHawkItemInfo] = true;
	return 0;
}
static int _nonblock_HawkGroup = _NonblockMap_HawkGroup();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_HAWKGROUP_ENUM
