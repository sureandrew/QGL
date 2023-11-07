#pragma once
#ifndef RPC_SKILLGROUP_ENUM
#define RPC_SKILLGROUP_ENUM

enum RPC_SkillGroup_Enum
{
	SkillGroup_CLASSID	= 34000,
	RPC_SkillGroup_ENUM_ST	= 34000,
	RPC_SkillGroup_UP_STATE	= 34657,
	RPC_SkillGroup_REQ_STATE	= 34935,
	RPC_SkillGroup_CB_AddSkills	= 34843,
	RPC_SkillGroup_CB_RemoveSkills	= 34938,
	RPC_SkillGroup_ENUM_ED	= 34999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_SkillGroup()
{
	RPC_ENUM_MAP[RPC_SkillGroup_UP_STATE] = 423504350;
	RPC_ENUM_MAP[RPC_SkillGroup_REQ_STATE] = 301226259;
	RPC_ENUM_MAP[RPC_SkillGroup_CB_AddSkills] = 148256391;
	RPC_ENUM_MAP[RPC_SkillGroup_CB_RemoveSkills] = 393032429;
	return 0;
}
static int _addrpc_SkillGroup = _AddRpcEnum_SkillGroup();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_SkillGroup()
{
	RPC_NONBLOCK_MAP[RPC_SkillGroup_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_SkillGroup_CB_AddSkills] = true;
	RPC_NONBLOCK_MAP[RPC_SkillGroup_CB_RemoveSkills] = true;
	return 0;
}
static int _nonblock_SkillGroup = _NonblockMap_SkillGroup();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_SKILLGROUP_ENUM
