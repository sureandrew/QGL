#pragma once
#ifndef RPC_QUESTTYPE_ENUM
#define RPC_QUESTTYPE_ENUM

enum RPC_QuestType_Enum
{
	QuestType_CLASSID	= 31010,
	RPC_QuestType_ENUM_ST	= 31010,
	RPC_QuestType_UP_STATE	= 31019,
	RPC_QuestType_REQ_STATE	= 31018,
	RPC_QuestType_ENUM_ED	= 31019
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_QuestType()
{
	RPC_ENUM_MAP[RPC_QuestType_UP_STATE] = 169762045;
	RPC_ENUM_MAP[RPC_QuestType_REQ_STATE] = 346048650;
	return 0;
}
static int _addrpc_QuestType = _AddRpcEnum_QuestType();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_QuestType()
{
	RPC_NONBLOCK_MAP[RPC_QuestType_UP_STATE] = true;
	return 0;
}
static int _nonblock_QuestType = _NonblockMap_QuestType();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_QUESTTYPE_ENUM
