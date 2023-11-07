#pragma once
#ifndef RPC_SKILL_ENUM
#define RPC_SKILL_ENUM

enum RPC_Skill_Enum
{
	Skill_CLASSID	= 33000,
	RPC_Skill_ENUM_ST	= 33000,
	RPC_Skill_UP_STATE	= 33444,
	RPC_Skill_REQ_STATE	= 33898,
	RPC_Skill_ENUM_ED	= 33999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Skill()
{
	RPC_ENUM_MAP[RPC_Skill_UP_STATE] = 923693173;
	RPC_ENUM_MAP[RPC_Skill_REQ_STATE] = 860775349;
	return 0;
}
static int _addrpc_Skill = _AddRpcEnum_Skill();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Skill()
{
	RPC_NONBLOCK_MAP[RPC_Skill_UP_STATE] = true;
	return 0;
}
static int _nonblock_Skill = _NonblockMap_Skill();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_SKILL_ENUM
