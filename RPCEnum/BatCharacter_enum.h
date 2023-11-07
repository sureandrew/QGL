#pragma once
#ifndef RPC_BATCHARACTER_ENUM
#define RPC_BATCHARACTER_ENUM

enum RPC_BatCharacter_Enum
{
	BatCharacter_CLASSID	= 18000,
	RPC_BatCharacter_ENUM_ST	= 18000,
	RPC_BatCharacter_UP_STATE	= 18012,
	RPC_BatCharacter_REQ_STATE	= 18739,
	RPC_BatCharacter_ENUM_ED	= 18999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_BatCharacter()
{
	RPC_ENUM_MAP[RPC_BatCharacter_UP_STATE] = 1983910738;
	RPC_ENUM_MAP[RPC_BatCharacter_REQ_STATE] = 664798000;
	return 0;
}
static int _addrpc_BatCharacter = _AddRpcEnum_BatCharacter();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_BatCharacter()
{
	RPC_NONBLOCK_MAP[RPC_BatCharacter_UP_STATE] = true;
	return 0;
}
static int _nonblock_BatCharacter = _NonblockMap_BatCharacter();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_BATCHARACTER_ENUM
