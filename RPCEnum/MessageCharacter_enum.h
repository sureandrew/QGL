#pragma once
#ifndef RPC_MESSAGECHARACTER_ENUM
#define RPC_MESSAGECHARACTER_ENUM

enum RPC_MessageCharacter_Enum
{
	MessageCharacter_CLASSID	= 40000,
	RPC_MessageCharacter_ENUM_ST	= 40000,
	RPC_MessageCharacter_UP_STATE	= 40816,
	RPC_MessageCharacter_REQ_STATE	= 40239,
	RPC_MessageCharacter_SelfSettingChange	= 40592,
	RPC_MessageCharacter_ENUM_ED	= 40999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_MessageCharacter()
{
	RPC_ENUM_MAP[RPC_MessageCharacter_UP_STATE] = 444148693;
	RPC_ENUM_MAP[RPC_MessageCharacter_REQ_STATE] = 276770739;
	RPC_ENUM_MAP[RPC_MessageCharacter_SelfSettingChange] = 537623301;
	return 0;
}
static int _addrpc_MessageCharacter = _AddRpcEnum_MessageCharacter();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_MessageCharacter()
{
	RPC_NONBLOCK_MAP[RPC_MessageCharacter_UP_STATE] = true;
	return 0;
}
static int _nonblock_MessageCharacter = _NonblockMap_MessageCharacter();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_MESSAGECHARACTER_ENUM
