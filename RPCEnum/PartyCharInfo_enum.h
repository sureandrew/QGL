#pragma once
#ifndef RPC_PARTYCHARINFO_ENUM
#define RPC_PARTYCHARINFO_ENUM

enum RPC_PartyCharInfo_Enum
{
	PartyCharInfo_CLASSID	= 61000,
	RPC_PartyCharInfo_ENUM_ST	= 61000,
	RPC_PartyCharInfo_UP_STATE	= 61405,
	RPC_PartyCharInfo_REQ_STATE	= 61302,
	RPC_PartyCharInfo_ENUM_ED	= 61599
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_PartyCharInfo()
{
	RPC_ENUM_MAP[RPC_PartyCharInfo_UP_STATE] = 602479523;
	RPC_ENUM_MAP[RPC_PartyCharInfo_REQ_STATE] = 826571991;
	return 0;
}
static int _addrpc_PartyCharInfo = _AddRpcEnum_PartyCharInfo();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_PartyCharInfo()
{
	RPC_NONBLOCK_MAP[RPC_PartyCharInfo_UP_STATE] = true;
	return 0;
}
static int _nonblock_PartyCharInfo = _NonblockMap_PartyCharInfo();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_PARTYCHARINFO_ENUM
