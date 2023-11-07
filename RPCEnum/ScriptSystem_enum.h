#pragma once
#ifndef RPC_SCRIPTSYSTEM_ENUM
#define RPC_SCRIPTSYSTEM_ENUM

enum RPC_ScriptSystem_Enum
{
	ScriptSystem_CLASSID	= 32000,
	RPC_ScriptSystem_ENUM_ST	= 32000,
	RPC_ScriptSystem_UP_STATE	= 32205,
	RPC_ScriptSystem_REQ_STATE	= 32992,
	RPC_ScriptSystem_ReplyAnswer	= 32131,
	RPC_ScriptSystem_ReplyAnswerString	= 32357,
	RPC_ScriptSystem_ReplyGiven	= 32812,
	RPC_ScriptSystem_StopTalk	= 32052,
	RPC_ScriptSystem_CB_TalkEnd	= 32960,
	RPC_ScriptSystem_CB_TalkNext	= 32391,
	RPC_ScriptSystem_CB_Choose	= 32095,
	RPC_ScriptSystem_CB_Say	= 32216,
	RPC_ScriptSystem_CB_OpenInterface	= 32389,
	RPC_ScriptSystem_CB_OpenShopMenu	= 32505,
	RPC_ScriptSystem_CB_ShowHint	= 32007,
	RPC_ScriptSystem_CB_OnPlaySound	= 32774,
	RPC_ScriptSystem_CB_AskGiven	= 32366,
	RPC_ScriptSystem_CB_GivenFinish	= 32775,
	RPC_ScriptSystem_CB_AskString	= 32988,
	RPC_ScriptSystem_ENUM_ED	= 32999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_ScriptSystem()
{
	RPC_ENUM_MAP[RPC_ScriptSystem_UP_STATE] = 271208504;
	RPC_ENUM_MAP[RPC_ScriptSystem_REQ_STATE] = 103765836;
	RPC_ENUM_MAP[RPC_ScriptSystem_ReplyAnswer] = 474753136;
	RPC_ENUM_MAP[RPC_ScriptSystem_ReplyAnswerString] = 283642252;
	RPC_ENUM_MAP[RPC_ScriptSystem_ReplyGiven] = 1548887685;
	RPC_ENUM_MAP[RPC_ScriptSystem_StopTalk] = 1260455611;
	RPC_ENUM_MAP[RPC_ScriptSystem_CB_TalkEnd] = 1665618048;
	RPC_ENUM_MAP[RPC_ScriptSystem_CB_TalkNext] = 64504023;
	RPC_ENUM_MAP[RPC_ScriptSystem_CB_Choose] = 687694366;
	RPC_ENUM_MAP[RPC_ScriptSystem_CB_Say] = 913659639;
	RPC_ENUM_MAP[RPC_ScriptSystem_CB_OpenInterface] = 79063645;
	RPC_ENUM_MAP[RPC_ScriptSystem_CB_OpenShopMenu] = 131608988;
	RPC_ENUM_MAP[RPC_ScriptSystem_CB_ShowHint] = 185803571;
	RPC_ENUM_MAP[RPC_ScriptSystem_CB_OnPlaySound] = 1454594331;
	RPC_ENUM_MAP[RPC_ScriptSystem_CB_AskGiven] = 518091936;
	RPC_ENUM_MAP[RPC_ScriptSystem_CB_GivenFinish] = 439170370;
	RPC_ENUM_MAP[RPC_ScriptSystem_CB_AskString] = 480398226;
	return 0;
}
static int _addrpc_ScriptSystem = _AddRpcEnum_ScriptSystem();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_ScriptSystem()
{
	RPC_NONBLOCK_MAP[RPC_ScriptSystem_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_ScriptSystem_CB_TalkEnd] = true;
	RPC_NONBLOCK_MAP[RPC_ScriptSystem_CB_TalkNext] = true;
	RPC_NONBLOCK_MAP[RPC_ScriptSystem_CB_Choose] = true;
	RPC_NONBLOCK_MAP[RPC_ScriptSystem_CB_Say] = true;
	RPC_NONBLOCK_MAP[RPC_ScriptSystem_CB_OpenInterface] = true;
	RPC_NONBLOCK_MAP[RPC_ScriptSystem_CB_OpenShopMenu] = true;
	RPC_NONBLOCK_MAP[RPC_ScriptSystem_CB_ShowHint] = true;
	RPC_NONBLOCK_MAP[RPC_ScriptSystem_CB_OnPlaySound] = true;
	RPC_NONBLOCK_MAP[RPC_ScriptSystem_CB_AskGiven] = true;
	RPC_NONBLOCK_MAP[RPC_ScriptSystem_CB_GivenFinish] = true;
	RPC_NONBLOCK_MAP[RPC_ScriptSystem_CB_AskString] = true;
	return 0;
}
static int _nonblock_ScriptSystem = _NonblockMap_ScriptSystem();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_SCRIPTSYSTEM_ENUM
