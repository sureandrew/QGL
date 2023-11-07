#pragma once
#ifndef RPC_RANKCHARACTER_ENUM
#define RPC_RANKCHARACTER_ENUM

enum RPC_RankCharacter_Enum
{
	RankCharacter_CLASSID	= 43000,
	RPC_RankCharacter_ENUM_ST	= 43000,
	RPC_RankCharacter_UP_STATE	= 43555,
	RPC_RankCharacter_REQ_STATE	= 43601,
	RPC_RankCharacter_ENUM_ED	= 43999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_RankCharacter()
{
	RPC_ENUM_MAP[RPC_RankCharacter_UP_STATE] = 307298890;
	RPC_ENUM_MAP[RPC_RankCharacter_REQ_STATE] = 1978038091;
	return 0;
}
static int _addrpc_RankCharacter = _AddRpcEnum_RankCharacter();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_RankCharacter()
{
	RPC_NONBLOCK_MAP[RPC_RankCharacter_UP_STATE] = true;
	return 0;
}
static int _nonblock_RankCharacter = _NonblockMap_RankCharacter();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_RANKCHARACTER_ENUM
