#pragma once
#ifndef RPC_ACCOUNT_ENUM
#define RPC_ACCOUNT_ENUM

enum RPC_Account_Enum
{
	Account_CLASSID	= 11000,
	RPC_Account_ENUM_ST	= 11000,
	RPC_Account_UP_STATE	= 11425,
	RPC_Account_REQ_STATE	= 11253,
	RPC_Account_GetOwnCharacters	= 11672,
	RPC_Account_CheckCharacter	= 11534,
	RPC_Account_CheckCharacterName	= 11213,
	RPC_Account_SelectCharacter	= 11184,
	RPC_Account_Logout	= 11104,
	RPC_Account_AddCharacter	= 11315,
	RPC_Account_RemoveCharacter	= 11941,
	RPC_Account_FreezeCharacter	= 11492,
	RPC_Account_RecoverCharacter	= 11115,
	RPC_Account_CB_Error	= 11417,
	RPC_Account_CB_CheckCharacter	= 11370,
	RPC_Account_CB_GetOwnCharacters	= 11254,
	RPC_Account_CB_SelectCharacter	= 11118,
	RPC_Account_CB_LogoutSuccess	= 11004,
	RPC_Account_CB_CreateSuccess	= 11439,
	RPC_Account_CB_RemoveCharSuccess	= 11743,
	RPC_Account_CB_CheckCharacterName	= 11647,
	RPC_Account_CB_RecoverCharSuccess	= 11929,
	RPC_Account_CB_FreezeCharSuccess	= 11267,
	RPC_Account_ENUM_ED	= 11999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Account()
{
	RPC_ENUM_MAP[RPC_Account_UP_STATE] = 97800821;
	RPC_ENUM_MAP[RPC_Account_REQ_STATE] = 2014207959;
	RPC_ENUM_MAP[RPC_Account_GetOwnCharacters] = 160132587;
	RPC_ENUM_MAP[RPC_Account_CheckCharacter] = 1201502748;
	RPC_ENUM_MAP[RPC_Account_CheckCharacterName] = 188553919;
	RPC_ENUM_MAP[RPC_Account_SelectCharacter] = 507605114;
	RPC_ENUM_MAP[RPC_Account_Logout] = 2091078578;
	RPC_ENUM_MAP[RPC_Account_AddCharacter] = 317921466;
	RPC_ENUM_MAP[RPC_Account_RemoveCharacter] = 188507704;
	RPC_ENUM_MAP[RPC_Account_FreezeCharacter] = 1376462057;
	RPC_ENUM_MAP[RPC_Account_RecoverCharacter] = 1617326016;
	RPC_ENUM_MAP[RPC_Account_CB_Error] = 1474721487;
	RPC_ENUM_MAP[RPC_Account_CB_CheckCharacter] = 1339323336;
	RPC_ENUM_MAP[RPC_Account_CB_GetOwnCharacters] = 310063790;
	RPC_ENUM_MAP[RPC_Account_CB_SelectCharacter] = 1728582379;
	RPC_ENUM_MAP[RPC_Account_CB_LogoutSuccess] = 1944146759;
	RPC_ENUM_MAP[RPC_Account_CB_CreateSuccess] = 1890454112;
	RPC_ENUM_MAP[RPC_Account_CB_RemoveCharSuccess] = 417215886;
	RPC_ENUM_MAP[RPC_Account_CB_CheckCharacterName] = 1423646894;
	RPC_ENUM_MAP[RPC_Account_CB_RecoverCharSuccess] = 237001887;
	RPC_ENUM_MAP[RPC_Account_CB_FreezeCharSuccess] = 1536905089;
	return 0;
}
static int _addrpc_Account = _AddRpcEnum_Account();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Account()
{
	RPC_NONBLOCK_MAP[RPC_Account_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_Account_GetOwnCharacters] = true;
	RPC_NONBLOCK_MAP[RPC_Account_CheckCharacter] = true;
	RPC_NONBLOCK_MAP[RPC_Account_CheckCharacterName] = true;
	RPC_NONBLOCK_MAP[RPC_Account_SelectCharacter] = true;
	RPC_NONBLOCK_MAP[RPC_Account_Logout] = true;
	RPC_NONBLOCK_MAP[RPC_Account_CB_Error] = true;
	RPC_NONBLOCK_MAP[RPC_Account_CB_CheckCharacter] = true;
	RPC_NONBLOCK_MAP[RPC_Account_CB_GetOwnCharacters] = true;
	RPC_NONBLOCK_MAP[RPC_Account_CB_SelectCharacter] = true;
	RPC_NONBLOCK_MAP[RPC_Account_CB_LogoutSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_Account_CB_CreateSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_Account_CB_RemoveCharSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_Account_CB_CheckCharacterName] = true;
	RPC_NONBLOCK_MAP[RPC_Account_CB_RecoverCharSuccess] = true;
	RPC_NONBLOCK_MAP[RPC_Account_CB_FreezeCharSuccess] = true;
	return 0;
}
static int _nonblock_Account = _NonblockMap_Account();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_ACCOUNT_ENUM
