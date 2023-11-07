#pragma once
#ifndef RPC_BATACTOR_ENUM
#define RPC_BATACTOR_ENUM

enum RPC_BatActor_Enum
{
	BatActor_CLASSID	= 20000,
	RPC_BatActor_ENUM_ST	= 20000,
	RPC_BatActor_UP_STATE	= 20458,
	RPC_BatActor_REQ_STATE	= 20260,
	RPC_BatActor_SendCommand	= 20950,
	RPC_BatActor_CheatCode	= 20320,
	RPC_BatActor_CancelAuto	= 20624,
	RPC_BatActor_CB_Error	= 20737,
	RPC_BatActor_CB_CommandSuccess	= 20712,
	RPC_BatActor_CB_Inputed	= 20313,
	RPC_BatActor_CB_AutoCount	= 20521,
	RPC_BatActor_CB_BattleMsg	= 20522,
	RPC_BatActor_ENUM_ED	= 20999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_BatActor()
{
	RPC_ENUM_MAP[RPC_BatActor_UP_STATE] = 1902991750;
	RPC_ENUM_MAP[RPC_BatActor_REQ_STATE] = 1940390731;
	RPC_ENUM_MAP[RPC_BatActor_SendCommand] = 1036143315;
	RPC_ENUM_MAP[RPC_BatActor_CheatCode] = 2117226740;
	RPC_ENUM_MAP[RPC_BatActor_CancelAuto] = 1743848763;
	RPC_ENUM_MAP[RPC_BatActor_CB_Error] = 1236285136;
	RPC_ENUM_MAP[RPC_BatActor_CB_CommandSuccess] = 766788022;
	RPC_ENUM_MAP[RPC_BatActor_CB_Inputed] = 91956427;
	RPC_ENUM_MAP[RPC_BatActor_CB_AutoCount] = 573711624;
	RPC_ENUM_MAP[RPC_BatActor_CB_BattleMsg] = 71531305;
	return 0;
}
static int _addrpc_BatActor = _AddRpcEnum_BatActor();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_BatActor()
{
	RPC_NONBLOCK_MAP[RPC_BatActor_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_BatActor_CB_Error] = true;
	RPC_NONBLOCK_MAP[RPC_BatActor_CB_CommandSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_BatActor_CB_Inputed] = true;
	RPC_NONBLOCK_MAP[RPC_BatActor_CB_AutoCount] = true;
	RPC_NONBLOCK_MAP[RPC_BatActor_CB_BattleMsg] = true;
	return 0;
}
static int _nonblock_BatActor = _NonblockMap_BatActor();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_BATACTOR_ENUM
