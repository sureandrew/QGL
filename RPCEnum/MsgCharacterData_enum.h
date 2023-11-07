#pragma once
#ifndef RPC_MSGCHARACTERDATA_ENUM
#define RPC_MSGCHARACTERDATA_ENUM

enum RPC_MsgCharacterData_Enum
{
	MsgCharacterData_CLASSID	= 10140,
	RPC_MsgCharacterData_ENUM_ST	= 10140,
	RPC_MsgCharacterData_UP_STATE	= 10144,
	RPC_MsgCharacterData_REQ_STATE	= 10145,
	RPC_MsgCharacterData_ENUM_ED	= 10149
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_MsgCharacterData()
{
	RPC_ENUM_MAP[RPC_MsgCharacterData_UP_STATE] = 1978142268;
	RPC_ENUM_MAP[RPC_MsgCharacterData_REQ_STATE] = 344475483;
	return 0;
}
static int _addrpc_MsgCharacterData = _AddRpcEnum_MsgCharacterData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_MsgCharacterData()
{
	RPC_NONBLOCK_MAP[RPC_MsgCharacterData_UP_STATE] = true;
	return 0;
}
static int _nonblock_MsgCharacterData = _NonblockMap_MsgCharacterData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_MSGCHARACTERDATA_ENUM
