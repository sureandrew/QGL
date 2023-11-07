#pragma once
#ifndef RPC_MAPSERVICE_ENUM
#define RPC_MAPSERVICE_ENUM

enum RPC_MapService_Enum
{
	RPC_MapService_ENUM_ST	= 3000,
	RPC_MapService_RegisterProxy	= 3046,
	RPC_MapService_UnregisterProxy	= 3621,
	RPC_MapService_ReleaseSession	= 3423,
	RPC_MapService_SyncProxy	= 3007,
	RPC_MapService_CB_SyncProxy	= 3094,
	RPC_MapService_ChangeSessionId	= 3904,
	RPC_MapService_ResumeSession	= 3978,
	RPC_MapService_CB_ResumeSession	= 3483,
	RPC_MapService_RemoteCreateNpc	= 3998,
	RPC_MapService_QueryInstance	= 3870,
	RPC_MapService_CB_QueryInstance	= 3195,
	RPC_MapService_CharacterEnter	= 3421,
	RPC_MapService_CharacterLeave	= 3831,
	RPC_MapService_NpcEnter	= 3191,
	RPC_MapService_ChangeLoadLevel	= 3461,
	RPC_MapService_RankTitleUpdate	= 3247,
	RPC_MapService_GuildUpdate	= 3706,
	RPC_MapService_GuildRemove	= 3070,
	RPC_MapService_GuildMemberUpdate	= 3777,
	RPC_MapService_GuildMemberRemove	= 3664,
	RPC_MapService_MsgCharacterUpdate	= 3159,
	RPC_MapService_ManorForceOut	= 3180,
	RPC_MapService_CB_Error	= 3198,
	RPC_MapService_CB_EnterSuccess	= 3497,
	RPC_MapService_CB_CharacterStuff	= 3781,
	RPC_MapService_CB_LeaveSuccess	= 3559,
	RPC_MapService_CB_CharacterEnter	= 3162,
	RPC_MapService_CB_PartyEnter	= 3185,
	RPC_MapService_CB_NpcEnter	= 3015,
	RPC_MapService_CB_EnterBattle	= 3009,
	RPC_MapService_CharacterCheat	= 3639,
	RPC_MapService_CB_CharacterCheat	= 3238,
	RPC_MapService_UpdateLimitedItemRemain	= 3342,
	RPC_MapService_ShowPublicItemDetail	= 3012,
	RPC_MapService_ShowPublicPartnerDetail	= 3634,
	RPC_MapService_ShowPublicPetDetail	= 3153,
	RPC_MapService_OpenChargeShop	= 3540,
	RPC_MapService_CloseChargeShop	= 3600,
	RPC_MapService_CB_OpenChargeShop	= 3791,
	RPC_MapService_CB_SyncCharacter	= 3456,
	RPC_MapService_CB_SyncItem	= 3628,
	RPC_MapService_CB_SyncPartner	= 3126,
	RPC_MapService_CB_AddPartnersSuccess	= 3453,
	RPC_MapService_CB_ShowPublicItemDetail	= 3740,
	RPC_MapService_CB_ShowPublicPartnerDetail	= 3176,
	RPC_MapService_CB_ShowPublicPetDetail	= 3814,
	RPC_MapService_SendMapMessage	= 3546,
	RPC_MapService_SendMapEmotion	= 3562,
	RPC_MapService_CB_ReceiveMapMessage	= 3749,
	RPC_MapService_CB_ReceiveMapEmotion	= 3567,
	RPC_MapService_CB_ForceLogout	= 3897,
	RPC_MapService_ChangeMoney	= 3492,
	RPC_MapService_ClientDisconnect	= 3242,
	RPC_MapService_GetPosition	= 3687,
	RPC_MapService_CB_GetPosition	= 3384,
	RPC_MapService_UpdateWantedReward	= 3707,
	RPC_MapService_CB_UpdateWantedReward	= 3389,
	RPC_MapService_DeleteGenerationFromWorld	= 3795,
	RPC_MapService_RecvGenerationAccmulation	= 3477,
	RPC_MapService_GenerationOnlineNotify	= 3107,
	RPC_MapService_GenerationOfflineNotify	= 3158,
	RPC_MapService_GetGenenrationCharsResult	= 3330,
	RPC_MapService_ShowPublicItemDetailFromOtherLine	= 3096,
	RPC_MapService_ShowPublicPartnerDetailFromOtherLine	= 3894,
	RPC_MapService_ShowPublicPetDetailFromOtherLine	= 3482,
	RPC_MapService_CB_ShowPublicPartnerDetailFromOtherLine	= 3529,
	RPC_MapService_CB_ShowPublicPetDetailFromOtherLine	= 3336,
	RPC_MapService_CB_ShowPublicItemDetailFromOtherLine	= 3227,
	RPC_MapService_ENUM_ED	= 3999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_MapService()
{
	RPC_ENUM_MAP[RPC_MapService_RegisterProxy] = 0;
	RPC_ENUM_MAP[RPC_MapService_UnregisterProxy] = 0;
	RPC_ENUM_MAP[RPC_MapService_ReleaseSession] = 0;
	RPC_ENUM_MAP[RPC_MapService_SyncProxy] = 0;
	RPC_ENUM_MAP[RPC_MapService_CB_SyncProxy] = 0;
	RPC_ENUM_MAP[RPC_MapService_ChangeSessionId] = 0;
	RPC_ENUM_MAP[RPC_MapService_ResumeSession] = 0;
	RPC_ENUM_MAP[RPC_MapService_CB_ResumeSession] = 0;
	RPC_ENUM_MAP[RPC_MapService_RemoteCreateNpc] = 0;
	RPC_ENUM_MAP[RPC_MapService_QueryInstance] = 0;
	RPC_ENUM_MAP[RPC_MapService_CB_QueryInstance] = 0;
	RPC_ENUM_MAP[RPC_MapService_CharacterEnter] = 0;
	RPC_ENUM_MAP[RPC_MapService_CharacterLeave] = 0;
	RPC_ENUM_MAP[RPC_MapService_NpcEnter] = 0;
	RPC_ENUM_MAP[RPC_MapService_ChangeLoadLevel] = 0;
	RPC_ENUM_MAP[RPC_MapService_RankTitleUpdate] = 0;
	RPC_ENUM_MAP[RPC_MapService_GuildUpdate] = 0;
	RPC_ENUM_MAP[RPC_MapService_GuildRemove] = 0;
	RPC_ENUM_MAP[RPC_MapService_GuildMemberUpdate] = 0;
	RPC_ENUM_MAP[RPC_MapService_GuildMemberRemove] = 0;
	RPC_ENUM_MAP[RPC_MapService_MsgCharacterUpdate] = 0;
	RPC_ENUM_MAP[RPC_MapService_ManorForceOut] = 0;
	RPC_ENUM_MAP[RPC_MapService_CB_Error] = 950751607;
	RPC_ENUM_MAP[RPC_MapService_CB_EnterSuccess] = 1732650242;
	RPC_ENUM_MAP[RPC_MapService_CB_CharacterStuff] = 1241854683;
	RPC_ENUM_MAP[RPC_MapService_CB_LeaveSuccess] = 1047143100;
	RPC_ENUM_MAP[RPC_MapService_CB_CharacterEnter] = 1698965209;
	RPC_ENUM_MAP[RPC_MapService_CB_PartyEnter] = 688273772;
	RPC_ENUM_MAP[RPC_MapService_CB_NpcEnter] = 17040657;
	RPC_ENUM_MAP[RPC_MapService_CB_EnterBattle] = 1376742086;
	RPC_ENUM_MAP[RPC_MapService_CharacterCheat] = 0;
	RPC_ENUM_MAP[RPC_MapService_CB_CharacterCheat] = 0;
	RPC_ENUM_MAP[RPC_MapService_UpdateLimitedItemRemain] = 0;
	RPC_ENUM_MAP[RPC_MapService_ShowPublicItemDetail] = 1134259944;
	RPC_ENUM_MAP[RPC_MapService_ShowPublicPartnerDetail] = 1569917621;
	RPC_ENUM_MAP[RPC_MapService_ShowPublicPetDetail] = 1525690276;
	RPC_ENUM_MAP[RPC_MapService_OpenChargeShop] = 1353540095;
	RPC_ENUM_MAP[RPC_MapService_CloseChargeShop] = 1925281129;
	RPC_ENUM_MAP[RPC_MapService_CB_OpenChargeShop] = 1218269948;
	RPC_ENUM_MAP[RPC_MapService_CB_SyncCharacter] = 1421242889;
	RPC_ENUM_MAP[RPC_MapService_CB_SyncItem] = 406863196;
	RPC_ENUM_MAP[RPC_MapService_CB_SyncPartner] = 987312133;
	RPC_ENUM_MAP[RPC_MapService_CB_AddPartnersSuccess] = 1631546235;
	RPC_ENUM_MAP[RPC_MapService_CB_ShowPublicItemDetail] = 2122804530;
	RPC_ENUM_MAP[RPC_MapService_CB_ShowPublicPartnerDetail] = 181617556;
	RPC_ENUM_MAP[RPC_MapService_CB_ShowPublicPetDetail] = 1441406415;
	RPC_ENUM_MAP[RPC_MapService_SendMapMessage] = 210182392;
	RPC_ENUM_MAP[RPC_MapService_SendMapEmotion] = 1365247911;
	RPC_ENUM_MAP[RPC_MapService_CB_ReceiveMapMessage] = 1763596625;
	RPC_ENUM_MAP[RPC_MapService_CB_ReceiveMapEmotion] = 83128094;
	RPC_ENUM_MAP[RPC_MapService_CB_ForceLogout] = 1131569201;
	RPC_ENUM_MAP[RPC_MapService_ChangeMoney] = 647310699;
	RPC_ENUM_MAP[RPC_MapService_ClientDisconnect] = 0;
	RPC_ENUM_MAP[RPC_MapService_GetPosition] = 874598020;
	RPC_ENUM_MAP[RPC_MapService_CB_GetPosition] = 1108702494;
	RPC_ENUM_MAP[RPC_MapService_UpdateWantedReward] = 249116525;
	RPC_ENUM_MAP[RPC_MapService_CB_UpdateWantedReward] = 1292175556;
	RPC_ENUM_MAP[RPC_MapService_DeleteGenerationFromWorld] = 2137140722;
	RPC_ENUM_MAP[RPC_MapService_RecvGenerationAccmulation] = 727084692;
	RPC_ENUM_MAP[RPC_MapService_GenerationOnlineNotify] = 334890239;
	RPC_ENUM_MAP[RPC_MapService_GenerationOfflineNotify] = 1104742176;
	RPC_ENUM_MAP[RPC_MapService_GetGenenrationCharsResult] = 1931876113;
	RPC_ENUM_MAP[RPC_MapService_ShowPublicItemDetailFromOtherLine] = 1766868985;
	RPC_ENUM_MAP[RPC_MapService_ShowPublicPartnerDetailFromOtherLine] = 1897221527;
	RPC_ENUM_MAP[RPC_MapService_ShowPublicPetDetailFromOtherLine] = 1519024537;
	RPC_ENUM_MAP[RPC_MapService_CB_ShowPublicPartnerDetailFromOtherLine] = 509741308;
	RPC_ENUM_MAP[RPC_MapService_CB_ShowPublicPetDetailFromOtherLine] = 336362802;
	RPC_ENUM_MAP[RPC_MapService_CB_ShowPublicItemDetailFromOtherLine] = 1415328226;
	return 0;
}
static int _addrpc_MapService = _AddRpcEnum_MapService();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_MapService()
{
	RPC_NONBLOCK_MAP[RPC_MapService_CB_SyncProxy] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_ChangeSessionId] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_ResumeSession] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_QueryInstance] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_ChangeLoadLevel] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_RankTitleUpdate] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_GuildUpdate] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_GuildRemove] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_GuildMemberUpdate] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_GuildMemberRemove] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_MsgCharacterUpdate] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_ManorForceOut] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_Error] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_EnterSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_CharacterStuff] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_LeaveSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_CharacterEnter] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_PartyEnter] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_NpcEnter] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_EnterBattle] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_CharacterCheat] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_UpdateLimitedItemRemain] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_OpenChargeShop] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_SyncCharacter] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_SyncItem] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_SyncPartner] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_AddPartnersSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_ShowPublicItemDetail] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_ShowPublicPartnerDetail] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_ShowPublicPetDetail] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_SendMapMessage] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_SendMapEmotion] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_ReceiveMapMessage] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_ReceiveMapEmotion] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_ForceLogout] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_ClientDisconnect] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_GetPosition] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_GetPosition] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_UpdateWantedReward] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_UpdateWantedReward] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_DeleteGenerationFromWorld] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_RecvGenerationAccmulation] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_GenerationOnlineNotify] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_GenerationOfflineNotify] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_GetGenenrationCharsResult] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_ShowPublicItemDetailFromOtherLine] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_ShowPublicPartnerDetailFromOtherLine] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_ShowPublicPetDetailFromOtherLine] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_ShowPublicPartnerDetailFromOtherLine] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_ShowPublicPetDetailFromOtherLine] = true;
	RPC_NONBLOCK_MAP[RPC_MapService_CB_ShowPublicItemDetailFromOtherLine] = true;
	return 0;
}
static int _nonblock_MapService = _NonblockMap_MapService();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_MAPSERVICE_ENUM
