#pragma once
#ifndef RPC_CHARACTER_ENUM
#define RPC_CHARACTER_ENUM

enum RPC_Character_Enum
{
	Character_CLASSID	= 13000,
	RPC_Character_ENUM_ST	= 13000,
	RPC_Character_UP_STATE	= 13669,
	RPC_Character_REQ_STATE	= 13240,
	RPC_Character_MoveTo	= 13795,
	RPC_Character_StopMove	= 13375,
	RPC_Character_RespondTest	= 13955,
	RPC_Character_TestBattle	= 13149,
	RPC_Character_CancelAuto	= 13328,
	RPC_Character_LevelUp	= 13142,
	RPC_Character_RaiseAttrib	= 13558,
	RPC_Character_PKRequest	= 13277,
	RPC_Character_LeaveWatching	= 13056,
	RPC_Character_MonitorBattle	= 13004,
	RPC_Character_JoinBattle	= 13083,
	RPC_Character_RequestJoinBattle	= 13521,
	RPC_Character_RejectHelper	= 13109,
	RPC_Character_SetFollowPartner	= 13765,
	RPC_Character_SetFollowPet	= 13219,
	RPC_Character_ChangeMapLine	= 13568,
	RPC_Character_UpdateYuanBao	= 13204,
	RPC_Character_CB_RequestJoinBattle	= 13688,
	RPC_Character_CB_PKRequest	= 13424,
	RPC_Character_CB_SetEnablePK	= 13235,
	RPC_Character_CB_JoinBattle	= 13064,
	RPC_Character_CB_CancelAuto	= 13709,
	RPC_Character_CB_LevelUp	= 13114,
	RPC_Character_CB_RaiseAttrib	= 13005,
	RPC_Character_CB_AddHelper	= 13137,
	RPC_Character_CB_RejectHelper	= 13063,
	RPC_Character_CB_LeaveHelpersList	= 13011,
	RPC_Character_CB_ClearHelpersList	= 13341,
	RPC_Character_CB_HelpRequestFail	= 13581,
	RPC_Character_CB_WarningMessage	= 13451,
	RPC_Character_SearchChar	= 13441,
	RPC_Character_CB_SearchChar	= 13878,
	RPC_Character_CB_OpenWeaponsRankPanel	= 13293,
	RPC_Character_SetTarget	= 13183,
	RPC_Character_CB_SetTarget	= 13636,
	RPC_Character_CreateParty	= 13089,
	RPC_Character_PartyJoinRequest	= 13987,
	RPC_Character_PartyJoinLevelControl	= 13814,
	RPC_Character_PartyJoinReply	= 13824,
	RPC_Character_PartyInviteRequest	= 13170,
	RPC_Character_PartyInviteReply	= 13815,
	RPC_Character_SearchParty	= 13690,
	RPC_Character_GetPartyInfo	= 13097,
	RPC_Character_CB_Error	= 13015,
	RPC_Character_CB_JoinParty	= 13628,
	RPC_Character_CB_ChangeDesc	= 13278,
	RPC_Character_CB_PartyRequestOk	= 13995,
	RPC_Character_CB_PartyRequestNo	= 13336,
	RPC_Character_CB_GetPlayerEquip	= 13228,
	RPC_Character_CB_PartyInviteOk	= 13086,
	RPC_Character_CB_PartyJoinRequest	= 13730,
	RPC_Character_CB_PartyInviteRequest	= 13686,
	RPC_Character_CB_PartyClearRequest	= 13279,
	RPC_Character_CB_PartyClearInvite	= 13201,
	RPC_Character_CB_PartyMessage	= 13322,
	RPC_Character_CB_SearchParty	= 13349,
	RPC_Character_CB_GetPartyInfo	= 13737,
	RPC_Character_CB_OpenSearchPartyPanel	= 13996,
	RPC_Character_ShutNewGuide	= 13335,
	RPC_Character_CB_HandleGuideEvent	= 13342,
	RPC_Character_CheatCode	= 13682,
	RPC_Character_CB_RespondTest	= 13934,
	RPC_Character_LearnSkill	= 13369,
	RPC_Character_UseSkill	= 13627,
	RPC_Character_UpGradeSkill	= 13062,
	RPC_Character_RemoveSkill	= 13495,
	RPC_Character_CB_LearnSkill	= 13900,
	RPC_Character_CB_UseSkill	= 13483,
	RPC_Character_CB_UpGradeSkill	= 13821,
	RPC_Character_CB_RemoveSkill	= 13577,
	RPC_Character_SkillShopLearn	= 13481,
	RPC_Character_LeaveSkillShop	= 13712,
	RPC_Character_CB_SkillShopLearn	= 13294,
	RPC_Character_GivenProcess	= 13866,
	RPC_Character_CB_GivenProcess	= 13910,
	RPC_Character_CB_GivenItem	= 13376,
	RPC_Character_CB_GivenPartner	= 13915,
	RPC_Character_CB_GivenPet	= 13813,
	RPC_Character_CB_GivenMoney	= 13725,
	RPC_Character_UpdateShortCutList	= 13194,
	RPC_Character_UpdateShortCut	= 13469,
	RPC_Character_CB_UpdateShortCutSuccess	= 13398,
	RPC_Character_SaveSystemSetting	= 13786,
	RPC_Character_CB_SaveSystemSetting	= 13604,
	RPC_Character_ChangeMsgIcon	= 13347,
	RPC_Character_AskForTrading	= 13410,
	RPC_Character_CB_AskForTrading	= 13290,
	RPC_Character_ReplyReqTrading	= 13677,
	RPC_Character_CB_ReplyReqTrading	= 13260,
	RPC_Character_CB_TradingMessage	= 13289,
	RPC_Character_CB_TradingObject	= 13881,
	RPC_Character_TalkToNPC	= 13790,
	RPC_Character_CB_TalkToNPC	= 13537,
	RPC_Character_ChangeTitle	= 13708,
	RPC_Character_CB_TitleListUpdate	= 13090,
	RPC_Character_CB_AntiHack	= 13455,
	RPC_Character_AnswerAntiHack	= 13261,
	RPC_Character_CB_ReportAntiHack	= 13655,
	RPC_Character_DepositInBank	= 13214,
	RPC_Character_WithdrawFmBank	= 13721,
	RPC_Character_CB_DepositInBank	= 13539,
	RPC_Character_CB_WithdrawFmBank	= 13193,
	RPC_Character_StartHawk	= 13673,
	RPC_Character_CB_StartHawk	= 13868,
	RPC_Character_GetHawkFromTarget	= 13832,
	RPC_Character_CB_GetHawk	= 13498,
	RPC_Character_CB_HawkMessage	= 13330,
	RPC_Character_CB_HawkHistory	= 13394,
	RPC_Character_CB_UpdateHawkHistory	= 13106,
	RPC_Character_ClearHawkerHistory	= 13629,
	RPC_Character_CB_SendGrpMessage	= 13819,
	RPC_Character_SetListen	= 13388,
	RPC_Character_DonateMoney	= 13265,
	RPC_Character_CreateGuild	= 13127,
	RPC_Character_CB_OpenCreateGuild	= 13138,
	RPC_Character_CB_AwardMsg	= 13612,
	RPC_Character_CB_OpenGiftBox	= 13700,
	RPC_Character_CB_AddictionWarning	= 13415,
	RPC_Character_CB_AddictCurrentStatus	= 13331,
	RPC_Character_CB_AddictionShutDown	= 13547,
	RPC_Character_CB_PlyInterestDwn	= 13310,
	RPC_Character_CB_UseItemCharacter	= 13253,
	RPC_Character_EnterQuestion	= 13560,
	RPC_Character_ExitQuestion	= 13920,
	RPC_Character_CB_AskEnterQuestion	= 13023,
	RPC_Character_CB_EnterQuestion	= 13575,
	RPC_Character_CB_ExitQuestion	= 13189,
	RPC_Character_CB_AnswerQuestion	= 13530,
	RPC_Character_CloseWareHouse	= 13828,
	RPC_Character_CB_OpenWareHouse	= 13506,
	RPC_Character_LockAsset	= 13414,
	RPC_Character_UnlockAsset	= 13428,
	RPC_Character_SetAssetPassword	= 13263,
	RPC_Character_ForceResetAssetPassword	= 13200,
	RPC_Character_ChangeAssetPassword	= 13197,
	RPC_Character_CB_AssetProtectResult	= 13238,
	RPC_Character_CB_AssetReleaseLockResult	= 13965,
	RPC_Character_CB_SetAssetPassword	= 13528,
	RPC_Character_CB_ForceResetAssetPassword	= 13572,
	RPC_Character_CB_SetRobot	= 13053,
	RPC_Character_CB_RobotMapAction	= 13312,
	RPC_Character_CB_RobotBattleTurn	= 13947,
	RPC_Character_SetAutoWalk	= 13777,
	RPC_Character_SetAutoWalkHitBoss	= 13213,
	RPC_Character_CB_SetAutoWalkHitBoss	= 13071,
	RPC_Character_KickMember	= 13147,
	RPC_Character_CB_DonateResult	= 13158,
	RPC_Character_CB_GetPartnerCollectionList	= 13038,
	RPC_Character_UsePartnerCollectionBuff	= 13234,
	RPC_Character_CB_UsePartnerCollectionBuff	= 13602,
	RPC_Character_AddPartnerCollection	= 13715,
	RPC_Character_CB_AddPartnerCollection	= 13413,
	RPC_Character_ChangeBetweenYBandYBT	= 13998,
	RPC_Character_CB_ChangeBetweenYBandYBT	= 13327,
	RPC_Character_AddSellYuanBao	= 13366,
	RPC_Character_CB_AddSellYuanBao	= 13332,
	RPC_Character_BuySellYuanBao	= 13355,
	RPC_Character_CB_BuySellYuanBao	= 13096,
	RPC_Character_GetAllMoneyAndYBT	= 13272,
	RPC_Character_CB_GetAllMoneyAndYBT	= 13717,
	RPC_Character_RequestTargetEquip	= 13475,
	RPC_Character_CB_RequestTargetEquip	= 13672,
	RPC_Character_CB_RelationSystem	= 13780,
	RPC_Character_AddStartHawkItem	= 13954,
	RPC_Character_CB_SetTargetNameFormHawk	= 13658,
	RPC_Character_CB_InitTargetHawkConcernState	= 13169,
	RPC_Character_ModifyLineup	= 13516,
	RPC_Character_CB_ModifyLineup	= 13143,
	RPC_Character_ModifyLineupKeyPos	= 13614,
	RPC_Character_CB_ModifyLineupKeyPos	= 13110,
	RPC_Character_ForgiveLineup	= 13180,
	RPC_Character_CB_ForgiveLineup	= 13948,
	RPC_Character_SetChangePos	= 13493,
	RPC_Character_CB_SetChangePos	= 13230,
	RPC_Character_UpdateWantedReward	= 13188,
	RPC_Character_CB_UpdateWantedReward	= 13807,
	RPC_Character_JailDelivery	= 13350,
	RPC_Character_CB_JailDelivery	= 13991,
	RPC_Character_WantedBail	= 13728,
	RPC_Character_CB_WantedBail	= 13365,
	RPC_Character_WantedTrace	= 13750,
	RPC_Character_CB_WantedTrace	= 13804,
	RPC_Character_CB_RequestWantedTrace	= 13772,
	RPC_Character_GetGenerationGroup	= 13490,
	RPC_Character_CB_GetGenerationGroup	= 13985,
	RPC_Character_RequestGeneration	= 13592,
	RPC_Character_CB_RequestGenerationTrans	= 13077,
	RPC_Character_RequestGenerationAns	= 13668,
	RPC_Character_CB_RequestGenerationResult	= 13803,
	RPC_Character_DeleteGeneration	= 13320,
	RPC_Character_CB_DeleteGeneration	= 13192,
	RPC_Character_CB_ChangeGenerationType	= 13416,
	RPC_Character_GetSavedExp	= 13903,
	RPC_Character_CB_UpdateGenerationAcc	= 13032,
	RPC_Character_BuyAccShopItem	= 13848,
	RPC_Character_CB_BugAccShopItem	= 13223,
	RPC_Character_CB_GenerationGainAcc	= 13597,
	RPC_Character_BuyPointsShopItem	= 13760,
	RPC_Character_CB_BuyPointsShopItem	= 13447,
	RPC_Character_CB_OpenBusinessLog	= 13756,
	RPC_Character_CB_SetMarriageNpcAction	= 13973,
	RPC_Character_CB_ShowMarriageEff	= 13473,
	RPC_Character_CB_ResetChangeLine	= 13598,
	RPC_Character_CB_PetUseSkillMsg	= 13315,
	RPC_Character_RequestCrossMapJump	= 13754,
	RPC_Character_LeaveTeamTemporarily	= 13300,
	RPC_Character_CB_LeaveTeamTemporarily	= 13761,
	RPC_Character_CB_GetPartyInfoWhenJumpMap	= 13462,
	RPC_Character_ReturnParty	= 13764,
	RPC_Character_CB_ReturnParty	= 13646,
	RPC_Character_CB_RefreshPartyCharInfo	= 13134,
	RPC_Character_CB_RestorePartyAsTemp	= 13111,
	RPC_Character_CB_SetChangeModelSkillGroup	= 13256,
	RPC_Character_CB_ShowIncognitoBoxEff	= 13699,
	RPC_Character_ENUM_ED	= 13999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Character()
{
	RPC_ENUM_MAP[RPC_Character_UP_STATE] = 265173520;
	RPC_ENUM_MAP[RPC_Character_REQ_STATE] = 1898843120;
	RPC_ENUM_MAP[RPC_Character_MoveTo] = 1374051356;
	RPC_ENUM_MAP[RPC_Character_StopMove] = 1931221114;
	RPC_ENUM_MAP[RPC_Character_RespondTest] = 954938274;
	RPC_ENUM_MAP[RPC_Character_TestBattle] = 1106660933;
	RPC_ENUM_MAP[RPC_Character_CancelAuto] = 778976857;
	RPC_ENUM_MAP[RPC_Character_LevelUp] = 207552678;
	RPC_ENUM_MAP[RPC_Character_RaiseAttrib] = 449666965;
	RPC_ENUM_MAP[RPC_Character_PKRequest] = 949372009;
	RPC_ENUM_MAP[RPC_Character_LeaveWatching] = 1228280283;
	RPC_ENUM_MAP[RPC_Character_MonitorBattle] = 900603833;
	RPC_ENUM_MAP[RPC_Character_JoinBattle] = 364977655;
	RPC_ENUM_MAP[RPC_Character_RequestJoinBattle] = 107413759;
	RPC_ENUM_MAP[RPC_Character_RejectHelper] = 1112875477;
	RPC_ENUM_MAP[RPC_Character_SetFollowPartner] = 1639598290;
	RPC_ENUM_MAP[RPC_Character_SetFollowPet] = 1596522635;
	RPC_ENUM_MAP[RPC_Character_ChangeMapLine] = 238376788;
	RPC_ENUM_MAP[RPC_Character_UpdateYuanBao] = 2045444688;
	RPC_ENUM_MAP[RPC_Character_CB_RequestJoinBattle] = 884230030;
	RPC_ENUM_MAP[RPC_Character_CB_PKRequest] = 1392330330;
	RPC_ENUM_MAP[RPC_Character_CB_SetEnablePK] = 472343456;
	RPC_ENUM_MAP[RPC_Character_CB_JoinBattle] = 977666519;
	RPC_ENUM_MAP[RPC_Character_CB_CancelAuto] = 1666675435;
	RPC_ENUM_MAP[RPC_Character_CB_LevelUp] = 2106011260;
	RPC_ENUM_MAP[RPC_Character_CB_RaiseAttrib] = 787505021;
	RPC_ENUM_MAP[RPC_Character_CB_AddHelper] = 272109665;
	RPC_ENUM_MAP[RPC_Character_CB_RejectHelper] = 825713249;
	RPC_ENUM_MAP[RPC_Character_CB_LeaveHelpersList] = 1499016738;
	RPC_ENUM_MAP[RPC_Character_CB_ClearHelpersList] = 1043758759;
	RPC_ENUM_MAP[RPC_Character_CB_HelpRequestFail] = 1840727934;
	RPC_ENUM_MAP[RPC_Character_CB_WarningMessage] = 440934718;
	RPC_ENUM_MAP[RPC_Character_SearchChar] = 2106742247;
	RPC_ENUM_MAP[RPC_Character_CB_SearchChar] = 708988917;
	RPC_ENUM_MAP[RPC_Character_CB_OpenWeaponsRankPanel] = 1924562885;
	RPC_ENUM_MAP[RPC_Character_SetTarget] = 1941011522;
	RPC_ENUM_MAP[RPC_Character_CB_SetTarget] = 944992856;
	RPC_ENUM_MAP[RPC_Character_CreateParty] = 1089035928;
	RPC_ENUM_MAP[RPC_Character_PartyJoinRequest] = 2017599356;
	RPC_ENUM_MAP[RPC_Character_PartyJoinLevelControl] = 265438586;
	RPC_ENUM_MAP[RPC_Character_PartyJoinReply] = 1367028673;
	RPC_ENUM_MAP[RPC_Character_PartyInviteRequest] = 62459794;
	RPC_ENUM_MAP[RPC_Character_PartyInviteReply] = 243883628;
	RPC_ENUM_MAP[RPC_Character_SearchParty] = 711031067;
	RPC_ENUM_MAP[RPC_Character_GetPartyInfo] = 1765372048;
	RPC_ENUM_MAP[RPC_Character_CB_Error] = 2004365301;
	RPC_ENUM_MAP[RPC_Character_CB_JoinParty] = 2073589861;
	RPC_ENUM_MAP[RPC_Character_CB_ChangeDesc] = 2016682674;
	RPC_ENUM_MAP[RPC_Character_CB_PartyRequestOk] = 451881969;
	RPC_ENUM_MAP[RPC_Character_CB_PartyRequestNo] = 611017579;
	RPC_ENUM_MAP[RPC_Character_CB_GetPlayerEquip] = 1586313843;
	RPC_ENUM_MAP[RPC_Character_CB_PartyInviteOk] = 1788162777;
	RPC_ENUM_MAP[RPC_Character_CB_PartyJoinRequest] = 263587870;
	RPC_ENUM_MAP[RPC_Character_CB_PartyInviteRequest] = 408096850;
	RPC_ENUM_MAP[RPC_Character_CB_PartyClearRequest] = 1680621049;
	RPC_ENUM_MAP[RPC_Character_CB_PartyClearInvite] = 1617130850;
	RPC_ENUM_MAP[RPC_Character_CB_PartyMessage] = 164526723;
	RPC_ENUM_MAP[RPC_Character_CB_SearchParty] = 1868450067;
	RPC_ENUM_MAP[RPC_Character_CB_GetPartyInfo] = 788955121;
	RPC_ENUM_MAP[RPC_Character_CB_OpenSearchPartyPanel] = 316170255;
	RPC_ENUM_MAP[RPC_Character_ShutNewGuide] = 1190462234;
	RPC_ENUM_MAP[RPC_Character_CB_HandleGuideEvent] = 1857582614;
	RPC_ENUM_MAP[RPC_Character_CheatCode] = 430328429;
	RPC_ENUM_MAP[RPC_Character_CB_RespondTest] = 719536770;
	RPC_ENUM_MAP[RPC_Character_LearnSkill] = 263073416;
	RPC_ENUM_MAP[RPC_Character_UseSkill] = 2005626063;
	RPC_ENUM_MAP[RPC_Character_UpGradeSkill] = 1711035980;
	RPC_ENUM_MAP[RPC_Character_RemoveSkill] = 1355220836;
	RPC_ENUM_MAP[RPC_Character_CB_LearnSkill] = 1640973944;
	RPC_ENUM_MAP[RPC_Character_CB_UseSkill] = 440018441;
	RPC_ENUM_MAP[RPC_Character_CB_UpGradeSkill] = 101261627;
	RPC_ENUM_MAP[RPC_Character_CB_RemoveSkill] = 301232851;
	RPC_ENUM_MAP[RPC_Character_SkillShopLearn] = 38816823;
	RPC_ENUM_MAP[RPC_Character_LeaveSkillShop] = 1171791729;
	RPC_ENUM_MAP[RPC_Character_CB_SkillShopLearn] = 19806500;
	RPC_ENUM_MAP[RPC_Character_GivenProcess] = 38934929;
	RPC_ENUM_MAP[RPC_Character_CB_GivenProcess] = 1341011045;
	RPC_ENUM_MAP[RPC_Character_CB_GivenItem] = 2120963231;
	RPC_ENUM_MAP[RPC_Character_CB_GivenPartner] = 1958285418;
	RPC_ENUM_MAP[RPC_Character_CB_GivenPet] = 57557994;
	RPC_ENUM_MAP[RPC_Character_CB_GivenMoney] = 1349417469;
	RPC_ENUM_MAP[RPC_Character_UpdateShortCutList] = 1252286919;
	RPC_ENUM_MAP[RPC_Character_UpdateShortCut] = 1733707110;
	RPC_ENUM_MAP[RPC_Character_CB_UpdateShortCutSuccess] = 1195318687;
	RPC_ENUM_MAP[RPC_Character_SaveSystemSetting] = 233990746;
	RPC_ENUM_MAP[RPC_Character_CB_SaveSystemSetting] = 1417040156;
	RPC_ENUM_MAP[RPC_Character_ChangeMsgIcon] = 2126270512;
	RPC_ENUM_MAP[RPC_Character_AskForTrading] = 1446146462;
	RPC_ENUM_MAP[RPC_Character_CB_AskForTrading] = 1983474478;
	RPC_ENUM_MAP[RPC_Character_ReplyReqTrading] = 1007357642;
	RPC_ENUM_MAP[RPC_Character_CB_ReplyReqTrading] = 753271876;
	RPC_ENUM_MAP[RPC_Character_CB_TradingMessage] = 1233933636;
	RPC_ENUM_MAP[RPC_Character_CB_TradingObject] = 1591901068;
	RPC_ENUM_MAP[RPC_Character_TalkToNPC] = 938031349;
	RPC_ENUM_MAP[RPC_Character_CB_TalkToNPC] = 1600547679;
	RPC_ENUM_MAP[RPC_Character_ChangeTitle] = 79375478;
	RPC_ENUM_MAP[RPC_Character_CB_TitleListUpdate] = 174658801;
	RPC_ENUM_MAP[RPC_Character_CB_AntiHack] = 1582920557;
	RPC_ENUM_MAP[RPC_Character_AnswerAntiHack] = 1359102115;
	RPC_ENUM_MAP[RPC_Character_CB_ReportAntiHack] = 1776368642;
	RPC_ENUM_MAP[RPC_Character_DepositInBank] = 761400209;
	RPC_ENUM_MAP[RPC_Character_WithdrawFmBank] = 1746292659;
	RPC_ENUM_MAP[RPC_Character_CB_DepositInBank] = 294275337;
	RPC_ENUM_MAP[RPC_Character_CB_WithdrawFmBank] = 1436378679;
	RPC_ENUM_MAP[RPC_Character_StartHawk] = 1878358502;
	RPC_ENUM_MAP[RPC_Character_CB_StartHawk] = 1480197420;
	RPC_ENUM_MAP[RPC_Character_GetHawkFromTarget] = 1610298636;
	RPC_ENUM_MAP[RPC_Character_CB_GetHawk] = 1847356866;
	RPC_ENUM_MAP[RPC_Character_CB_HawkMessage] = 664404854;
	RPC_ENUM_MAP[RPC_Character_CB_HawkHistory] = 895776164;
	RPC_ENUM_MAP[RPC_Character_CB_UpdateHawkHistory] = 853112723;
	RPC_ENUM_MAP[RPC_Character_ClearHawkerHistory] = 291527370;
	RPC_ENUM_MAP[RPC_Character_CB_SendGrpMessage] = 1159035620;
	RPC_ENUM_MAP[RPC_Character_SetListen] = 1076771333;
	RPC_ENUM_MAP[RPC_Character_DonateMoney] = 180895262;
	RPC_ENUM_MAP[RPC_Character_CreateGuild] = 1859083855;
	RPC_ENUM_MAP[RPC_Character_CB_OpenCreateGuild] = 850605881;
	RPC_ENUM_MAP[RPC_Character_CB_AwardMsg] = 1389179735;
	RPC_ENUM_MAP[RPC_Character_CB_OpenGiftBox] = 1802335796;
	RPC_ENUM_MAP[RPC_Character_CB_AddictionWarning] = 161423045;
	RPC_ENUM_MAP[RPC_Character_CB_AddictCurrentStatus] = 1538414089;
	RPC_ENUM_MAP[RPC_Character_CB_AddictionShutDown] = 733353603;
	RPC_ENUM_MAP[RPC_Character_CB_PlyInterestDwn] = 551160062;
	RPC_ENUM_MAP[RPC_Character_CB_UseItemCharacter] = 1531910651;
	RPC_ENUM_MAP[RPC_Character_EnterQuestion] = 1428378490;
	RPC_ENUM_MAP[RPC_Character_ExitQuestion] = 1316497505;
	RPC_ENUM_MAP[RPC_Character_CB_AskEnterQuestion] = 1674016061;
	RPC_ENUM_MAP[RPC_Character_CB_EnterQuestion] = 1280472680;
	RPC_ENUM_MAP[RPC_Character_CB_ExitQuestion] = 1821640660;
	RPC_ENUM_MAP[RPC_Character_CB_AnswerQuestion] = 605690981;
	RPC_ENUM_MAP[RPC_Character_CloseWareHouse] = 1064722433;
	RPC_ENUM_MAP[RPC_Character_CB_OpenWareHouse] = 1243417419;
	RPC_ENUM_MAP[RPC_Character_LockAsset] = 379535554;
	RPC_ENUM_MAP[RPC_Character_UnlockAsset] = 2109887002;
	RPC_ENUM_MAP[RPC_Character_SetAssetPassword] = 1849129414;
	RPC_ENUM_MAP[RPC_Character_ForceResetAssetPassword] = 2129725712;
	RPC_ENUM_MAP[RPC_Character_ChangeAssetPassword] = 408777961;
	RPC_ENUM_MAP[RPC_Character_CB_AssetProtectResult] = 841229587;
	RPC_ENUM_MAP[RPC_Character_CB_AssetReleaseLockResult] = 580604749;
	RPC_ENUM_MAP[RPC_Character_CB_SetAssetPassword] = 1607548830;
	RPC_ENUM_MAP[RPC_Character_CB_ForceResetAssetPassword] = 185282243;
	RPC_ENUM_MAP[RPC_Character_CB_SetRobot] = 270298762;
	RPC_ENUM_MAP[RPC_Character_CB_RobotMapAction] = 1022762810;
	RPC_ENUM_MAP[RPC_Character_CB_RobotBattleTurn] = 1967736293;
	RPC_ENUM_MAP[RPC_Character_SetAutoWalk] = 764886076;
	RPC_ENUM_MAP[RPC_Character_SetAutoWalkHitBoss] = 793737424;
	RPC_ENUM_MAP[RPC_Character_CB_SetAutoWalkHitBoss] = 681538378;
	RPC_ENUM_MAP[RPC_Character_KickMember] = 1765681134;
	RPC_ENUM_MAP[RPC_Character_CB_DonateResult] = 997279627;
	RPC_ENUM_MAP[RPC_Character_CB_GetPartnerCollectionList] = 1794267068;
	RPC_ENUM_MAP[RPC_Character_UsePartnerCollectionBuff] = 257650583;
	RPC_ENUM_MAP[RPC_Character_CB_UsePartnerCollectionBuff] = 1430018473;
	RPC_ENUM_MAP[RPC_Character_AddPartnerCollection] = 1860713316;
	RPC_ENUM_MAP[RPC_Character_CB_AddPartnerCollection] = 944062561;
	RPC_ENUM_MAP[RPC_Character_ChangeBetweenYBandYBT] = 1131102497;
	RPC_ENUM_MAP[RPC_Character_CB_ChangeBetweenYBandYBT] = 1247889129;
	RPC_ENUM_MAP[RPC_Character_AddSellYuanBao] = 65100298;
	RPC_ENUM_MAP[RPC_Character_CB_AddSellYuanBao] = 1987536093;
	RPC_ENUM_MAP[RPC_Character_BuySellYuanBao] = 1649043431;
	RPC_ENUM_MAP[RPC_Character_CB_BuySellYuanBao] = 1081898104;
	RPC_ENUM_MAP[RPC_Character_GetAllMoneyAndYBT] = 12871473;
	RPC_ENUM_MAP[RPC_Character_CB_GetAllMoneyAndYBT] = 1673862953;
	RPC_ENUM_MAP[RPC_Character_RequestTargetEquip] = 1242454806;
	RPC_ENUM_MAP[RPC_Character_CB_RequestTargetEquip] = 1645497323;
	RPC_ENUM_MAP[RPC_Character_CB_RelationSystem] = 1094416333;
	RPC_ENUM_MAP[RPC_Character_AddStartHawkItem] = 1269175953;
	RPC_ENUM_MAP[RPC_Character_CB_SetTargetNameFormHawk] = 1254319356;
	RPC_ENUM_MAP[RPC_Character_CB_InitTargetHawkConcernState] = 2042908018;
	RPC_ENUM_MAP[RPC_Character_ModifyLineup] = 1428050325;
	RPC_ENUM_MAP[RPC_Character_CB_ModifyLineup] = 1567647451;
	RPC_ENUM_MAP[RPC_Character_ModifyLineupKeyPos] = 1345213674;
	RPC_ENUM_MAP[RPC_Character_CB_ModifyLineupKeyPos] = 969433236;
	RPC_ENUM_MAP[RPC_Character_ForgiveLineup] = 1056645410;
	RPC_ENUM_MAP[RPC_Character_CB_ForgiveLineup] = 53431052;
	RPC_ENUM_MAP[RPC_Character_SetChangePos] = 1375165978;
	RPC_ENUM_MAP[RPC_Character_CB_SetChangePos] = 765602416;
	RPC_ENUM_MAP[RPC_Character_UpdateWantedReward] = 425606938;
	RPC_ENUM_MAP[RPC_Character_CB_UpdateWantedReward] = 1451180764;
	RPC_ENUM_MAP[RPC_Character_JailDelivery] = 1577334774;
	RPC_ENUM_MAP[RPC_Character_CB_JailDelivery] = 431580735;
	RPC_ENUM_MAP[RPC_Character_WantedBail] = 1362182386;
	RPC_ENUM_MAP[RPC_Character_CB_WantedBail] = 1406492635;
	RPC_ENUM_MAP[RPC_Character_WantedTrace] = 1330533952;
	RPC_ENUM_MAP[RPC_Character_CB_WantedTrace] = 609051308;
	RPC_ENUM_MAP[RPC_Character_CB_RequestWantedTrace] = 683047974;
	RPC_ENUM_MAP[RPC_Character_GetGenerationGroup] = 1394807366;
	RPC_ENUM_MAP[RPC_Character_CB_GetGenerationGroup] = 474378968;
	RPC_ENUM_MAP[RPC_Character_RequestGeneration] = 1717319932;
	RPC_ENUM_MAP[RPC_Character_CB_RequestGenerationTrans] = 266625076;
	RPC_ENUM_MAP[RPC_Character_RequestGenerationAns] = 1011309521;
	RPC_ENUM_MAP[RPC_Character_CB_RequestGenerationResult] = 1510801710;
	RPC_ENUM_MAP[RPC_Character_DeleteGeneration] = 368979257;
	RPC_ENUM_MAP[RPC_Character_CB_DeleteGeneration] = 1402830072;
	RPC_ENUM_MAP[RPC_Character_CB_ChangeGenerationType] = 1219760122;
	RPC_ENUM_MAP[RPC_Character_GetSavedExp] = 1830372486;
	RPC_ENUM_MAP[RPC_Character_CB_UpdateGenerationAcc] = 270404594;
	RPC_ENUM_MAP[RPC_Character_BuyAccShopItem] = 1948866788;
	RPC_ENUM_MAP[RPC_Character_CB_BugAccShopItem] = 829967264;
	RPC_ENUM_MAP[RPC_Character_CB_GenerationGainAcc] = 1728914333;
	RPC_ENUM_MAP[RPC_Character_BuyPointsShopItem] = 1083574795;
	RPC_ENUM_MAP[RPC_Character_CB_BuyPointsShopItem] = 947990062;
	RPC_ENUM_MAP[RPC_Character_CB_OpenBusinessLog] = 1214472072;
	RPC_ENUM_MAP[RPC_Character_CB_SetMarriageNpcAction] = 1208045910;
	RPC_ENUM_MAP[RPC_Character_CB_ShowMarriageEff] = 2102293522;
	RPC_ENUM_MAP[RPC_Character_CB_ResetChangeLine] = 1832715624;
	RPC_ENUM_MAP[RPC_Character_CB_PetUseSkillMsg] = 1198619385;
	RPC_ENUM_MAP[RPC_Character_RequestCrossMapJump] = 1326982159;
	RPC_ENUM_MAP[RPC_Character_LeaveTeamTemporarily] = 1174040656;
	RPC_ENUM_MAP[RPC_Character_CB_LeaveTeamTemporarily] = 1326673354;
	RPC_ENUM_MAP[RPC_Character_CB_GetPartyInfoWhenJumpMap] = 2144671047;
	RPC_ENUM_MAP[RPC_Character_ReturnParty] = 1322191304;
	RPC_ENUM_MAP[RPC_Character_CB_ReturnParty] = 993791940;
	RPC_ENUM_MAP[RPC_Character_CB_RefreshPartyCharInfo] = 526338154;
	RPC_ENUM_MAP[RPC_Character_CB_RestorePartyAsTemp] = 1958242924;
	RPC_ENUM_MAP[RPC_Character_CB_SetChangeModelSkillGroup] = 1810517220;
	RPC_ENUM_MAP[RPC_Character_CB_ShowIncognitoBoxEff] = 236878823;
	return 0;
}
static int _addrpc_Character = _AddRpcEnum_Character();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Character()
{
	RPC_NONBLOCK_MAP[RPC_Character_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_Character_MoveTo] = true;
	RPC_NONBLOCK_MAP[RPC_Character_StopMove] = true;
	RPC_NONBLOCK_MAP[RPC_Character_RespondTest] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RequestJoinBattle] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_PKRequest] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SetEnablePK] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_JoinBattle] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_CancelAuto] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_LevelUp] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RaiseAttrib] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AddHelper] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RejectHelper] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_LeaveHelpersList] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ClearHelpersList] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_HelpRequestFail] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_WarningMessage] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SearchChar] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_OpenWeaponsRankPanel] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SetTarget] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_Error] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_JoinParty] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ChangeDesc] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_PartyRequestOk] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_PartyRequestNo] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GetPlayerEquip] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_PartyInviteOk] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_PartyJoinRequest] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_PartyInviteRequest] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_PartyClearRequest] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_PartyClearInvite] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_PartyMessage] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SearchParty] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GetPartyInfo] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_OpenSearchPartyPanel] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_HandleGuideEvent] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RespondTest] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_LearnSkill] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_UseSkill] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_UpGradeSkill] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RemoveSkill] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SkillShopLearn] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GivenProcess] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GivenItem] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GivenPartner] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GivenPet] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GivenMoney] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_UpdateShortCutSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SaveSystemSetting] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AskForTrading] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ReplyReqTrading] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_TradingMessage] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_TradingObject] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_TalkToNPC] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_TitleListUpdate] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AntiHack] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ReportAntiHack] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_DepositInBank] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_WithdrawFmBank] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_StartHawk] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GetHawk] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_HawkMessage] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_HawkHistory] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_UpdateHawkHistory] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SendGrpMessage] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_OpenCreateGuild] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AwardMsg] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_OpenGiftBox] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AddictionWarning] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AddictCurrentStatus] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AddictionShutDown] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_PlyInterestDwn] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_UseItemCharacter] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AskEnterQuestion] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_EnterQuestion] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ExitQuestion] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AnswerQuestion] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_OpenWareHouse] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AssetProtectResult] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AssetReleaseLockResult] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SetAssetPassword] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ForceResetAssetPassword] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SetRobot] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RobotMapAction] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RobotBattleTurn] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SetAutoWalkHitBoss] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_DonateResult] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GetPartnerCollectionList] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_UsePartnerCollectionBuff] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AddPartnerCollection] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ChangeBetweenYBandYBT] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_AddSellYuanBao] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_BuySellYuanBao] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GetAllMoneyAndYBT] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RequestTargetEquip] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RelationSystem] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SetTargetNameFormHawk] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_InitTargetHawkConcernState] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ModifyLineup] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ModifyLineupKeyPos] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ForgiveLineup] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SetChangePos] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_UpdateWantedReward] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_JailDelivery] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_WantedBail] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_WantedTrace] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RequestWantedTrace] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GetGenerationGroup] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RequestGenerationTrans] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RequestGenerationResult] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_DeleteGeneration] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ChangeGenerationType] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_UpdateGenerationAcc] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_BugAccShopItem] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GenerationGainAcc] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_BuyPointsShopItem] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_OpenBusinessLog] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SetMarriageNpcAction] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ShowMarriageEff] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ResetChangeLine] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_PetUseSkillMsg] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_LeaveTeamTemporarily] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_GetPartyInfoWhenJumpMap] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ReturnParty] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RefreshPartyCharInfo] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_RestorePartyAsTemp] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_SetChangeModelSkillGroup] = true;
	RPC_NONBLOCK_MAP[RPC_Character_CB_ShowIncognitoBoxEff] = true;
	return 0;
}
static int _nonblock_Character = _NonblockMap_Character();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_CHARACTER_ENUM
