#pragma once
#ifndef RPC_CHARACTERINFO_ENUM
#define RPC_CHARACTERINFO_ENUM

enum RPC_CharacterInfo_Enum
{
	CharacterInfo_CLASSID	= 12000,
	RPC_CharacterInfo_ENUM_ST	= 12000,
	RPC_CharacterInfo_UP_STATE	= 12079,
	RPC_CharacterInfo_REQ_STATE	= 12246,
	RPC_CharacterInfo_ENUM_ED	= 12999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_CharacterInfo()
{
	RPC_ENUM_MAP[RPC_CharacterInfo_UP_STATE] = 1730051561;
	RPC_ENUM_MAP[RPC_CharacterInfo_REQ_STATE] = 1767385828;
	return 0;
}
static int _addrpc_CharacterInfo = _AddRpcEnum_CharacterInfo();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_CharacterInfo()
{
	RPC_NONBLOCK_MAP[RPC_CharacterInfo_UP_STATE] = true;
	return 0;
}
static int _nonblock_CharacterInfo = _NonblockMap_CharacterInfo();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_CHARACTERINFO_ENUM
