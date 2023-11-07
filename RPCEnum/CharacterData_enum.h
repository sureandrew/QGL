#pragma once
#ifndef RPC_CHARACTERDATA_ENUM
#define RPC_CHARACTERDATA_ENUM

enum RPC_CharacterData_Enum
{
	CharacterData_CLASSID	= 10020,
	RPC_CharacterData_ENUM_ST	= 10020,
	RPC_CharacterData_UP_STATE	= 10025,
	RPC_CharacterData_REQ_STATE	= 10023,
	RPC_CharacterData_ENUM_ED	= 10029
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_CharacterData()
{
	RPC_ENUM_MAP[RPC_CharacterData_UP_STATE] = 592655900;
	RPC_ENUM_MAP[RPC_CharacterData_REQ_STATE] = 1303466261;
	return 0;
}
static int _addrpc_CharacterData = _AddRpcEnum_CharacterData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_CharacterData()
{
	RPC_NONBLOCK_MAP[RPC_CharacterData_UP_STATE] = true;
	return 0;
}
static int _nonblock_CharacterData = _NonblockMap_CharacterData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_CHARACTERDATA_ENUM
