#pragma once
#ifndef RPC_RELATIONSYSTEM_ENUM
#define RPC_RELATIONSYSTEM_ENUM

enum RPC_RelationSystem_Enum
{
	RelationSystem_CLASSID	= 59000,
	RPC_RelationSystem_ENUM_ST	= 59000,
	RPC_RelationSystem_UP_STATE	= 59777,
	RPC_RelationSystem_REQ_STATE	= 59094,
	RPC_RelationSystem_CB_RequestSetSiblingTitlePrefix	= 59211,
	RPC_RelationSystem_SetSiblingTitlePrefix	= 59188,
	RPC_RelationSystem_CB_RequestSetSiblingTitleSuffix	= 59705,
	RPC_RelationSystem_SetSiblingTitleSuffix	= 59316,
	RPC_RelationSystem_CB_RequestSelectMarriageTime	= 59521,
	RPC_RelationSystem_SelectMarriageTime	= 59431,
	RPC_RelationSystem_ENUM_ED	= 59999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_RelationSystem()
{
	RPC_ENUM_MAP[RPC_RelationSystem_UP_STATE] = 653212037;
	RPC_ENUM_MAP[RPC_RelationSystem_REQ_STATE] = 176564249;
	RPC_ENUM_MAP[RPC_RelationSystem_CB_RequestSetSiblingTitlePrefix] = 634673698;
	RPC_ENUM_MAP[RPC_RelationSystem_SetSiblingTitlePrefix] = 550316454;
	RPC_ENUM_MAP[RPC_RelationSystem_CB_RequestSetSiblingTitleSuffix] = 921392118;
	RPC_ENUM_MAP[RPC_RelationSystem_SetSiblingTitleSuffix] = 366479036;
	RPC_ENUM_MAP[RPC_RelationSystem_CB_RequestSelectMarriageTime] = 665868924;
	RPC_ENUM_MAP[RPC_RelationSystem_SelectMarriageTime] = 1460735450;
	return 0;
}
static int _addrpc_RelationSystem = _AddRpcEnum_RelationSystem();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_RelationSystem()
{
	RPC_NONBLOCK_MAP[RPC_RelationSystem_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_RelationSystem_CB_RequestSetSiblingTitlePrefix] = true;
	RPC_NONBLOCK_MAP[RPC_RelationSystem_CB_RequestSetSiblingTitleSuffix] = true;
	RPC_NONBLOCK_MAP[RPC_RelationSystem_CB_RequestSelectMarriageTime] = true;
	return 0;
}
static int _nonblock_RelationSystem = _NonblockMap_RelationSystem();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_RELATIONSYSTEM_ENUM
