#pragma once
#ifndef RPC_PET_ENUM
#define RPC_PET_ENUM

enum RPC_Pet_Enum
{
	Pet_CLASSID	= 44000,
	RPC_Pet_ENUM_ST	= 44000,
	RPC_Pet_UP_STATE	= 44602,
	RPC_Pet_REQ_STATE	= 44405,
	RPC_Pet_CB_RaiseAttrib	= 44258,
	RPC_Pet_ChangePetName	= 44643,
	RPC_Pet_CB_LearnSkillMessage	= 44728,
	RPC_Pet_CB_ChangePetName	= 44188,
	RPC_Pet_ChangePetExp	= 44163,
	RPC_Pet_ChangePetVp	= 44182,
	RPC_Pet_ChangePetFavor	= 44306,
	RPC_Pet_ENUM_ED	= 44999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Pet()
{
	RPC_ENUM_MAP[RPC_Pet_UP_STATE] = 756689664;
	RPC_ENUM_MAP[RPC_Pet_REQ_STATE] = 1040144042;
	RPC_ENUM_MAP[RPC_Pet_CB_RaiseAttrib] = 1127433689;
	RPC_ENUM_MAP[RPC_Pet_ChangePetName] = 1504335914;
	RPC_ENUM_MAP[RPC_Pet_CB_LearnSkillMessage] = 568205284;
	RPC_ENUM_MAP[RPC_Pet_CB_ChangePetName] = 764498754;
	RPC_ENUM_MAP[RPC_Pet_ChangePetExp] = 187706123;
	RPC_ENUM_MAP[RPC_Pet_ChangePetVp] = 1436422192;
	RPC_ENUM_MAP[RPC_Pet_ChangePetFavor] = 1227570555;
	return 0;
}
static int _addrpc_Pet = _AddRpcEnum_Pet();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Pet()
{
	RPC_NONBLOCK_MAP[RPC_Pet_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_Pet_CB_RaiseAttrib] = true;
	RPC_NONBLOCK_MAP[RPC_Pet_CB_LearnSkillMessage] = true;
	RPC_NONBLOCK_MAP[RPC_Pet_CB_ChangePetName] = true;
	return 0;
}
static int _nonblock_Pet = _NonblockMap_Pet();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_PET_ENUM
