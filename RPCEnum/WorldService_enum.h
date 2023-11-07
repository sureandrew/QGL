#pragma once
#ifndef RPC_WORLDSERVICE_ENUM
#define RPC_WORLDSERVICE_ENUM

enum RPC_WorldService_Enum
{
	RPC_WorldService_ENUM_ST	= 2000,
	RPC_WorldService_RegisterProxy	= 2791,
	RPC_WorldService_UnregisterProxy	= 2994,
	RPC_WorldService_RegisterMap	= 2575,
	RPC_WorldService_UnregisterMap	= 2479,
	RPC_WorldService_RegisterInstance	= 2864,
	RPC_WorldService_UnregisterInstance	= 2090,
	RPC_WorldService_ValidateMap	= 2420,
	RPC_WorldService_QueryInstance	= 2551,
	RPC_WorldService_CB_QueryInstance	= 2279,
	RPC_WorldService_CanChangeLine	= 2116,
	RPC_WorldService_RemoteCreateNpc	= 2595,
	RPC_WorldService_ReleaseSession	= 2784,
	RPC_WorldService_CharacterLeave	= 2948,
	RPC_WorldService_CharJumpMap	= 2052,
	RPC_WorldService_NpcJumpMap	= 2746,
	RPC_WorldService_CharUpdateLocation	= 2438,
	RPC_WorldService_CB_JumpSuccess	= 2142,
	RPC_WorldService_CB_ValidMap	= 2233,
	RPC_WorldService_CB_UpdateLocation	= 2387,
	RPC_WorldService_CB_CanChangeLine	= 2456,
	RPC_WorldService_GetWorldGameTime	= 2307,
	RPC_WorldService_CB_GetWorldGameTime	= 2313,
	RPC_WorldService_NotifyNpcCreate	= 2457,
	RPC_WorldService_NotifyNpcDead	= 2916,
	RPC_WorldService_QueryNpcExist	= 2901,
	RPC_WorldService_ForceLogout	= 2576,
	RPC_WorldService_CB_NpcNotExist	= 2174,
	RPC_WorldService_CB_StartEvent	= 2204,
	RPC_WorldService_CB_StopEvent	= 2589,
	RPC_WorldService_CB_PeriodEvent	= 2041,
	RPC_WorldService_CB_TimeZoneChanged	= 2167,
	RPC_WorldService_StartEvent	= 2042,
	RPC_WorldService_LoginAccount	= 2681,
	RPC_WorldService_KickAccount	= 2066,
	RPC_WorldService_GetMapLines	= 2567,
	RPC_WorldService_VersionChecking	= 2490,
	RPC_WorldService_CB_GetMapLines	= 2332,
	RPC_WorldService_CB_LoginResult	= 2642,
	RPC_WorldService_CB_WaitAccount	= 2302,
	RPC_WorldService_CB_LoginAccount	= 2430,
	RPC_WorldService_CB_VersionCorrect	= 2148,
	RPC_WorldService_CB_ForceLogout	= 2212,
	RPC_WorldService_CheckCanBuyLimitedItem	= 2530,
	RPC_WorldService_ReduceLimitItem	= 2918,
	RPC_WorldService_CB_CheckCanBuyLimitedItem	= 2383,
	RPC_WorldService_SendCheatCommand	= 2213,
	RPC_WorldService_CB_AntiHack	= 2648,
	RPC_WorldService_AnswerAntiHack	= 2164,
	RPC_WorldService_WorldCheat	= 2514,
	RPC_WorldService_GetCharacterRank	= 2139,
	RPC_WorldService_GetGuildRank	= 2236,
	RPC_WorldService_StoreApolloLog	= 2728,
	RPC_WorldService_CB_Error	= 2819,
	RPC_WorldService_CB_WorldCheat	= 2570,
	RPC_WorldService_CB_GetCharacterRank	= 2467,
	RPC_WorldService_CB_RankChangeMsg	= 2372,
	RPC_WorldService_CB_GetGuildRank	= 2820,
	RPC_WorldService_ApolloMsg	= 2545,
	RPC_WorldService_CB_ApolloMsg	= 2847,
	RPC_WorldService_GetSellYuanBao	= 2389,
	RPC_WorldService_CB_GetSellYuanBao	= 2521,
	RPC_WorldService_AddSellYuanBao	= 2280,
	RPC_WorldService_CB_AddSellYuanBao	= 2955,
	RPC_WorldService_BuySellYuanBao	= 2535,
	RPC_WorldService_CB_BuySellYuanBao	= 2053,
	RPC_WorldService_GetTempBank	= 2218,
	RPC_WorldService_CB_GetTempBank	= 2401,
	RPC_WorldService_ClientDisconnect	= 2993,
	RPC_WorldService_UpdateWantedReward	= 2416,
	RPC_WorldService_CB_UpdateWantedReward	= 2253,
	RPC_WorldService_WantedDetail	= 2404,
	RPC_WorldService_CB_WantedDetail	= 2366,
	RPC_WorldService_RemoveFromWantedRank	= 2846,
	RPC_WorldService_DeleteGenerationTrans	= 2722,
	RPC_WorldService_RecvGenerationAccmulationTrans	= 2569,
	RPC_WorldService_GenerationOnlineNotifyTrans	= 2063,
	RPC_WorldService_GenerationOfflineNotifyTrans	= 2076,
	RPC_WorldService_HasGlobalNpcFlag	= 2787,
	RPC_WorldService_CB_HasGlobalNpcFlag	= 2452,
	RPC_WorldService_GetGlobalNpcFlag	= 2758,
	RPC_WorldService_CB_GetGlobalNpcFlag	= 2699,
	RPC_WorldService_SetGlobalNpcFlag	= 2975,
	RPC_WorldService_CB_SetGlobalNpcFlag	= 2054,
	RPC_WorldService_RemoveGlobalNpcFlag	= 2220,
	RPC_WorldService_CB_RemoveGlobalNpcFlag	= 2807,
	RPC_WorldService_ENUM_ED	= 2999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_WorldService()
{
	RPC_ENUM_MAP[RPC_WorldService_RegisterProxy] = 0;
	RPC_ENUM_MAP[RPC_WorldService_UnregisterProxy] = 0;
	RPC_ENUM_MAP[RPC_WorldService_RegisterMap] = 0;
	RPC_ENUM_MAP[RPC_WorldService_UnregisterMap] = 0;
	RPC_ENUM_MAP[RPC_WorldService_RegisterInstance] = 0;
	RPC_ENUM_MAP[RPC_WorldService_UnregisterInstance] = 0;
	RPC_ENUM_MAP[RPC_WorldService_ValidateMap] = 0;
	RPC_ENUM_MAP[RPC_WorldService_QueryInstance] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_QueryInstance] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CanChangeLine] = 0;
	RPC_ENUM_MAP[RPC_WorldService_RemoteCreateNpc] = 0;
	RPC_ENUM_MAP[RPC_WorldService_ReleaseSession] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CharacterLeave] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CharJumpMap] = 0;
	RPC_ENUM_MAP[RPC_WorldService_NpcJumpMap] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CharUpdateLocation] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_JumpSuccess] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_ValidMap] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_UpdateLocation] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_CanChangeLine] = 0;
	RPC_ENUM_MAP[RPC_WorldService_GetWorldGameTime] = 617243634;
	RPC_ENUM_MAP[RPC_WorldService_CB_GetWorldGameTime] = 962679827;
	RPC_ENUM_MAP[RPC_WorldService_NotifyNpcCreate] = 0;
	RPC_ENUM_MAP[RPC_WorldService_NotifyNpcDead] = 0;
	RPC_ENUM_MAP[RPC_WorldService_QueryNpcExist] = 0;
	RPC_ENUM_MAP[RPC_WorldService_ForceLogout] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_NpcNotExist] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_StartEvent] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_StopEvent] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_PeriodEvent] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_TimeZoneChanged] = 0;
	RPC_ENUM_MAP[RPC_WorldService_StartEvent] = 0;
	RPC_ENUM_MAP[RPC_WorldService_LoginAccount] = 64366381;
	RPC_ENUM_MAP[RPC_WorldService_KickAccount] = 1747418293;
	RPC_ENUM_MAP[RPC_WorldService_GetMapLines] = 1334859944;
	RPC_ENUM_MAP[RPC_WorldService_VersionChecking] = 283667935;
	RPC_ENUM_MAP[RPC_WorldService_CB_GetMapLines] = 969285436;
	RPC_ENUM_MAP[RPC_WorldService_CB_LoginResult] = 277704283;
	RPC_ENUM_MAP[RPC_WorldService_CB_WaitAccount] = 550449771;
	RPC_ENUM_MAP[RPC_WorldService_CB_LoginAccount] = 1343100704;
	RPC_ENUM_MAP[RPC_WorldService_CB_VersionCorrect] = 318650066;
	RPC_ENUM_MAP[RPC_WorldService_CB_ForceLogout] = 427715877;
	RPC_ENUM_MAP[RPC_WorldService_CheckCanBuyLimitedItem] = 0;
	RPC_ENUM_MAP[RPC_WorldService_ReduceLimitItem] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_CheckCanBuyLimitedItem] = 0;
	RPC_ENUM_MAP[RPC_WorldService_SendCheatCommand] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_AntiHack] = 1517552885;
	RPC_ENUM_MAP[RPC_WorldService_AnswerAntiHack] = 275790643;
	RPC_ENUM_MAP[RPC_WorldService_WorldCheat] = 0;
	RPC_ENUM_MAP[RPC_WorldService_GetCharacterRank] = 0;
	RPC_ENUM_MAP[RPC_WorldService_GetGuildRank] = 0;
	RPC_ENUM_MAP[RPC_WorldService_StoreApolloLog] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_Error] = 720059219;
	RPC_ENUM_MAP[RPC_WorldService_CB_WorldCheat] = 1523673697;
	RPC_ENUM_MAP[RPC_WorldService_CB_GetCharacterRank] = 1840188879;
	RPC_ENUM_MAP[RPC_WorldService_CB_RankChangeMsg] = 817654996;
	RPC_ENUM_MAP[RPC_WorldService_CB_GetGuildRank] = 1603016263;
	RPC_ENUM_MAP[RPC_WorldService_ApolloMsg] = 1724152093;
	RPC_ENUM_MAP[RPC_WorldService_CB_ApolloMsg] = 2080578402;
	RPC_ENUM_MAP[RPC_WorldService_GetSellYuanBao] = 627513020;
	RPC_ENUM_MAP[RPC_WorldService_CB_GetSellYuanBao] = 1616532101;
	RPC_ENUM_MAP[RPC_WorldService_AddSellYuanBao] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_AddSellYuanBao] = 0;
	RPC_ENUM_MAP[RPC_WorldService_BuySellYuanBao] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_BuySellYuanBao] = 0;
	RPC_ENUM_MAP[RPC_WorldService_GetTempBank] = 0;
	RPC_ENUM_MAP[RPC_WorldService_CB_GetTempBank] = 0;
	RPC_ENUM_MAP[RPC_WorldService_ClientDisconnect] = 0;
	RPC_ENUM_MAP[RPC_WorldService_UpdateWantedReward] = 1526680949;
	RPC_ENUM_MAP[RPC_WorldService_CB_UpdateWantedReward] = 695082008;
	RPC_ENUM_MAP[RPC_WorldService_WantedDetail] = 1772356250;
	RPC_ENUM_MAP[RPC_WorldService_CB_WantedDetail] = 1708657089;
	RPC_ENUM_MAP[RPC_WorldService_RemoveFromWantedRank] = 434708509;
	RPC_ENUM_MAP[RPC_WorldService_DeleteGenerationTrans] = 1925659521;
	RPC_ENUM_MAP[RPC_WorldService_RecvGenerationAccmulationTrans] = 511794514;
	RPC_ENUM_MAP[RPC_WorldService_GenerationOnlineNotifyTrans] = 1225659217;
	RPC_ENUM_MAP[RPC_WorldService_GenerationOfflineNotifyTrans] = 2097624507;
	RPC_ENUM_MAP[RPC_WorldService_HasGlobalNpcFlag] = 1895772719;
	RPC_ENUM_MAP[RPC_WorldService_CB_HasGlobalNpcFlag] = 242774850;
	RPC_ENUM_MAP[RPC_WorldService_GetGlobalNpcFlag] = 666986740;
	RPC_ENUM_MAP[RPC_WorldService_CB_GetGlobalNpcFlag] = 676280625;
	RPC_ENUM_MAP[RPC_WorldService_SetGlobalNpcFlag] = 364331590;
	RPC_ENUM_MAP[RPC_WorldService_CB_SetGlobalNpcFlag] = 1994195922;
	RPC_ENUM_MAP[RPC_WorldService_RemoveGlobalNpcFlag] = 1483754257;
	RPC_ENUM_MAP[RPC_WorldService_CB_RemoveGlobalNpcFlag] = 1535713103;
	return 0;
}
static int _addrpc_WorldService = _AddRpcEnum_WorldService();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_WorldService()
{
	RPC_NONBLOCK_MAP[RPC_WorldService_RegisterProxy] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_UnregisterProxy] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_RegisterMap] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_UnregisterMap] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_RegisterInstance] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_UnregisterInstance] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_ValidateMap] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_QueryInstance] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_QueryInstance] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CanChangeLine] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_RemoteCreateNpc] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_JumpSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_ValidMap] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_UpdateLocation] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_CanChangeLine] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_GetWorldGameTime] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_GetWorldGameTime] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_NotifyNpcCreate] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_NotifyNpcDead] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_QueryNpcExist] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_ForceLogout] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_NpcNotExist] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_StartEvent] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_StopEvent] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_PeriodEvent] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_TimeZoneChanged] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_GetMapLines] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_VersionChecking] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_GetMapLines] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_LoginResult] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_WaitAccount] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_LoginAccount] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_VersionCorrect] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_ForceLogout] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_CheckCanBuyLimitedItem] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_SendCheatCommand] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_AntiHack] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_WorldCheat] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_GetCharacterRank] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_GetGuildRank] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_StoreApolloLog] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_Error] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_WorldCheat] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_GetCharacterRank] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_RankChangeMsg] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_GetGuildRank] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_ApolloMsg] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_ApolloMsg] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_GetSellYuanBao] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_GetSellYuanBao] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_AddSellYuanBao] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_BuySellYuanBao] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_GetTempBank] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_ClientDisconnect] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_UpdateWantedReward] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_UpdateWantedReward] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_WantedDetail] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_WantedDetail] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_RemoveFromWantedRank] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_DeleteGenerationTrans] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_RecvGenerationAccmulationTrans] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_GenerationOnlineNotifyTrans] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_GenerationOfflineNotifyTrans] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_HasGlobalNpcFlag] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_HasGlobalNpcFlag] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_GetGlobalNpcFlag] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_GetGlobalNpcFlag] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_SetGlobalNpcFlag] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_SetGlobalNpcFlag] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_RemoveGlobalNpcFlag] = true;
	RPC_NONBLOCK_MAP[RPC_WorldService_CB_RemoveGlobalNpcFlag] = true;
	return 0;
}
static int _nonblock_WorldService = _NonblockMap_WorldService();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_WORLDSERVICE_ENUM