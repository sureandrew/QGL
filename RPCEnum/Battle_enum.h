#pragma once
#ifndef RPC_BATTLE_ENUM
#define RPC_BATTLE_ENUM

enum RPC_Battle_Enum
{
	Battle_CLASSID	= 17000,
	RPC_Battle_ENUM_ST	= 17000,
	RPC_Battle_UP_STATE	= 17567,
	RPC_Battle_REQ_STATE	= 17935,
	RPC_Battle_EndTurn	= 17115,
	RPC_Battle_NextTalk	= 17063,
	RPC_Battle_ChooseResult	= 17010,
	RPC_Battle_SendBattleMessage	= 17527,
	RPC_Battle_CB_Error	= 17085,
	RPC_Battle_CB_PlayTurn	= 17566,
	RPC_Battle_CB_PlayTurnMore	= 17920,
	RPC_Battle_CB_NextTurn	= 17270,
	RPC_Battle_CB_EndBattle	= 17556,
	RPC_Battle_CB_AddActors	= 17389,
	RPC_Battle_CB_AddPartners	= 17292,
	RPC_Battle_CB_ControlCharacter	= 17472,
	RPC_Battle_CB_ControlPartner	= 17416,
	RPC_Battle_CB_Reward	= 17904,
	RPC_Battle_CB_Penalty	= 17324,
	RPC_Battle_CB_NeedHelp	= 17180,
	RPC_Battle_CB_WaitTalk	= 17338,
	RPC_Battle_CB_Choose	= 17417,
	RPC_Battle_CB_Say	= 17858,
	RPC_Battle_CB_CancelTalk	= 17833,
	RPC_Battle_CB_DisableCmd	= 17061,
	RPC_Battle_CB_HideCmd	= 17665,
	RPC_Battle_CB_MartialResult	= 17900,
	RPC_Battle_ENUM_ED	= 17999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Battle()
{
	RPC_ENUM_MAP[RPC_Battle_UP_STATE] = 336615913;
	RPC_ENUM_MAP[RPC_Battle_REQ_STATE] = 1224412626;
	RPC_ENUM_MAP[RPC_Battle_EndTurn] = 938747213;
	RPC_ENUM_MAP[RPC_Battle_NextTalk] = 1479308481;
	RPC_ENUM_MAP[RPC_Battle_ChooseResult] = 1133995094;
	RPC_ENUM_MAP[RPC_Battle_SendBattleMessage] = 1528389021;
	RPC_ENUM_MAP[RPC_Battle_CB_Error] = 1746015519;
	RPC_ENUM_MAP[RPC_Battle_CB_PlayTurn] = 568265299;
	RPC_ENUM_MAP[RPC_Battle_CB_PlayTurnMore] = 1191783985;
	RPC_ENUM_MAP[RPC_Battle_CB_NextTurn] = 764877343;
	RPC_ENUM_MAP[RPC_Battle_CB_EndBattle] = 1872119112;
	RPC_ENUM_MAP[RPC_Battle_CB_AddActors] = 541138277;
	RPC_ENUM_MAP[RPC_Battle_CB_AddPartners] = 265845785;
	RPC_ENUM_MAP[RPC_Battle_CB_ControlCharacter] = 718021376;
	RPC_ENUM_MAP[RPC_Battle_CB_ControlPartner] = 331966810;
	RPC_ENUM_MAP[RPC_Battle_CB_Reward] = 559032103;
	RPC_ENUM_MAP[RPC_Battle_CB_Penalty] = 439100286;
	RPC_ENUM_MAP[RPC_Battle_CB_NeedHelp] = 1185026100;
	RPC_ENUM_MAP[RPC_Battle_CB_WaitTalk] = 245784539;
	RPC_ENUM_MAP[RPC_Battle_CB_Choose] = 397361136;
	RPC_ENUM_MAP[RPC_Battle_CB_Say] = 1684411447;
	RPC_ENUM_MAP[RPC_Battle_CB_CancelTalk] = 880754745;
	RPC_ENUM_MAP[RPC_Battle_CB_DisableCmd] = 1861091827;
	RPC_ENUM_MAP[RPC_Battle_CB_HideCmd] = 1201948691;
	RPC_ENUM_MAP[RPC_Battle_CB_MartialResult] = 1053907274;
	return 0;
}
static int _addrpc_Battle = _AddRpcEnum_Battle();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Battle()
{
	RPC_NONBLOCK_MAP[RPC_Battle_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_Error] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_PlayTurn] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_PlayTurnMore] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_NextTurn] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_EndBattle] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_AddActors] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_AddPartners] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_ControlCharacter] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_ControlPartner] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_Reward] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_Penalty] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_NeedHelp] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_WaitTalk] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_Choose] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_Say] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_CancelTalk] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_DisableCmd] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_HideCmd] = true;
	RPC_NONBLOCK_MAP[RPC_Battle_CB_MartialResult] = true;
	return 0;
}
static int _nonblock_Battle = _NonblockMap_Battle();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_BATTLE_ENUM
