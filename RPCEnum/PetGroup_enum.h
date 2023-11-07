#pragma once
#ifndef RPC_PETGROUP_ENUM
#define RPC_PETGROUP_ENUM

enum RPC_PetGroup_Enum
{
	PetGroup_CLASSID	= 45000,
	RPC_PetGroup_ENUM_ST	= 45000,
	RPC_PetGroup_UP_STATE	= 45815,
	RPC_PetGroup_REQ_STATE	= 45441,
	RPC_PetGroup_DeletePet	= 45485,
	RPC_PetGroup_ChangePetState	= 45256,
	RPC_PetGroup_PutToWareHouse	= 45430,
	RPC_PetGroup_GetFromWareHouse	= 45509,
	RPC_PetGroup_CB_Error	= 45351,
	RPC_PetGroup_CB_AddSkills	= 45305,
	RPC_PetGroup_CB_AddPets	= 45480,
	RPC_PetGroup_CB_RemovePets	= 45559,
	RPC_PetGroup_CB_UpdateAllPets	= 45020,
	RPC_PetGroup_CB_WareHouse	= 45691,
	RPC_PetGroup_ENUM_ED	= 45999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_PetGroup()
{
	RPC_ENUM_MAP[RPC_PetGroup_UP_STATE] = 256533608;
	RPC_ENUM_MAP[RPC_PetGroup_REQ_STATE] = 480529416;
	RPC_ENUM_MAP[RPC_PetGroup_DeletePet] = 1333538885;
	RPC_ENUM_MAP[RPC_PetGroup_ChangePetState] = 556204408;
	RPC_ENUM_MAP[RPC_PetGroup_PutToWareHouse] = 298800387;
	RPC_ENUM_MAP[RPC_PetGroup_GetFromWareHouse] = 1463449910;
	RPC_ENUM_MAP[RPC_PetGroup_CB_Error] = 1109594468;
	RPC_ENUM_MAP[RPC_PetGroup_CB_AddSkills] = 1188848268;
	RPC_ENUM_MAP[RPC_PetGroup_CB_AddPets] = 1415327286;
	RPC_ENUM_MAP[RPC_PetGroup_CB_RemovePets] = 2006324838;
	RPC_ENUM_MAP[RPC_PetGroup_CB_UpdateAllPets] = 1222004327;
	RPC_ENUM_MAP[RPC_PetGroup_CB_WareHouse] = 1412698051;
	return 0;
}
static int _addrpc_PetGroup = _AddRpcEnum_PetGroup();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_PetGroup()
{
	RPC_NONBLOCK_MAP[RPC_PetGroup_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_PetGroup_CB_Error] = true;
	RPC_NONBLOCK_MAP[RPC_PetGroup_CB_AddSkills] = true;
	RPC_NONBLOCK_MAP[RPC_PetGroup_CB_AddPets] = true;
	RPC_NONBLOCK_MAP[RPC_PetGroup_CB_RemovePets] = true;
	RPC_NONBLOCK_MAP[RPC_PetGroup_CB_UpdateAllPets] = true;
	RPC_NONBLOCK_MAP[RPC_PetGroup_CB_WareHouse] = true;
	return 0;
}
static int _nonblock_PetGroup = _NonblockMap_PetGroup();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_PETGROUP_ENUM
