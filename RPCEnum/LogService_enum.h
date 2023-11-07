#pragma once
#ifndef RPC_LOGSERVICE_ENUM
#define RPC_LOGSERVICE_ENUM

enum RPC_LogService_Enum
{
	RPC_LogService_ENUM_ST	= 6000,
	RPC_LogService_LogAccount	= 6689,
	RPC_LogService_LogCharacter	= 6083,
	RPC_LogService_LogItem	= 6729,
	RPC_LogService_LogQuest	= 6942,
	RPC_LogService_LogPlayerCount	= 6341,
	RPC_LogService_LogGiftBox	= 6572,
	RPC_LogService_LogChargeShop	= 6740,
	RPC_LogService_LogExchange	= 6909,
	RPC_LogService_LogExchangeForItemList	= 6248,
	RPC_LogService_LogYuanBao	= 6804,
	RPC_LogService_LogCheat	= 6698,
	RPC_LogService_LogCharExp	= 6639,
	RPC_LogService_LogMoney	= 6235,
	RPC_LogService_LogSMoney	= 6784,
	RPC_LogService_LogMix	= 6213,
	RPC_LogService_LogGoodness	= 6816,
	RPC_LogService_LogHack	= 6577,
	RPC_LogService_LogSellYuanBao	= 6162,
	RPC_LogService_LogHonor	= 6913,
	RPC_LogService_LogPartner	= 6212,
	RPC_LogService_LogGenerationAcc	= 6752,
	RPC_LogService_LogManor	= 6610,
	RPC_LogService_LogGuildDonate	= 6089,
	RPC_LogService_ENUM_ED	= 6999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_LogService()
{
	RPC_ENUM_MAP[RPC_LogService_LogAccount] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogCharacter] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogItem] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogQuest] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogPlayerCount] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogGiftBox] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogChargeShop] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogExchange] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogExchangeForItemList] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogYuanBao] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogCheat] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogCharExp] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogMoney] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogSMoney] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogMix] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogGoodness] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogHack] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogSellYuanBao] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogHonor] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogPartner] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogGenerationAcc] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogManor] = 0;
	RPC_ENUM_MAP[RPC_LogService_LogGuildDonate] = 0;
	return 0;
}
static int _addrpc_LogService = _AddRpcEnum_LogService();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
#endif // RPC_NONBLOCK_MAP

#endif //RPC_LOGSERVICE_ENUM
