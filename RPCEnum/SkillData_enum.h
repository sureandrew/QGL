#pragma once
#ifndef RPC_SKILLDATA_ENUM
#define RPC_SKILLDATA_ENUM

enum RPC_SkillData_Enum
{
	SkillData_CLASSID	= 10050,
	RPC_SkillData_ENUM_ST	= 10050,
	RPC_SkillData_UP_STATE	= 10058,
	RPC_SkillData_REQ_STATE	= 10055,
	RPC_SkillData_ENUM_ED	= 10059
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_SkillData()
{
	RPC_ENUM_MAP[RPC_SkillData_UP_STATE] = 950277788;
	RPC_ENUM_MAP[RPC_SkillData_REQ_STATE] = 1052715992;
	return 0;
}
static int _addrpc_SkillData = _AddRpcEnum_SkillData();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_SkillData()
{
	RPC_NONBLOCK_MAP[RPC_SkillData_UP_STATE] = true;
	return 0;
}
static int _nonblock_SkillData = _NonblockMap_SkillData();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_SKILLDATA_ENUM
