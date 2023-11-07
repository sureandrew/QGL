#pragma once
#ifndef RPC_PARTNER_ENUM
#define RPC_PARTNER_ENUM

enum RPC_Partner_Enum
{
	Partner_CLASSID	= 28000,
	RPC_Partner_ENUM_ST	= 28000,
	RPC_Partner_UP_STATE	= 28725,
	RPC_Partner_REQ_STATE	= 28209,
	RPC_Partner_RaiseAttrib	= 28757,
	RPC_Partner_ChangePartnerName	= 28149,
	RPC_Partner_ChangePartnerState	= 28485,
	RPC_Partner_CB_RaiseAttrib	= 28494,
	RPC_Partner_CB_ChangePartnerName	= 28390,
	RPC_Partner_CB_ChangePartnerState	= 28680,
	RPC_Partner_CB_UpdateSuccess	= 28370,
	RPC_Partner_CB_LearnSkillMessage	= 28959,
	RPC_Partner_CB_FalseJoinBattle	= 28794,
	RPC_Partner_ENUM_ED	= 28999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Partner()
{
	RPC_ENUM_MAP[RPC_Partner_UP_STATE] = 169612404;
	RPC_ENUM_MAP[RPC_Partner_REQ_STATE] = 1403781319;
	RPC_ENUM_MAP[RPC_Partner_RaiseAttrib] = 2124095243;
	RPC_ENUM_MAP[RPC_Partner_ChangePartnerName] = 1642414924;
	RPC_ENUM_MAP[RPC_Partner_ChangePartnerState] = 584076774;
	RPC_ENUM_MAP[RPC_Partner_CB_RaiseAttrib] = 318600191;
	RPC_ENUM_MAP[RPC_Partner_CB_ChangePartnerName] = 469255547;
	RPC_ENUM_MAP[RPC_Partner_CB_ChangePartnerState] = 1103260278;
	RPC_ENUM_MAP[RPC_Partner_CB_UpdateSuccess] = 371536329;
	RPC_ENUM_MAP[RPC_Partner_CB_LearnSkillMessage] = 1173311946;
	RPC_ENUM_MAP[RPC_Partner_CB_FalseJoinBattle] = 649489524;
	return 0;
}
static int _addrpc_Partner = _AddRpcEnum_Partner();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Partner()
{
	RPC_NONBLOCK_MAP[RPC_Partner_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_Partner_CB_RaiseAttrib] = true;
	RPC_NONBLOCK_MAP[RPC_Partner_CB_ChangePartnerName] = true;
	RPC_NONBLOCK_MAP[RPC_Partner_CB_ChangePartnerState] = true;
	RPC_NONBLOCK_MAP[RPC_Partner_CB_UpdateSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_Partner_CB_LearnSkillMessage] = true;
	RPC_NONBLOCK_MAP[RPC_Partner_CB_FalseJoinBattle] = true;
	return 0;
}
static int _nonblock_Partner = _NonblockMap_Partner();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_PARTNER_ENUM
