#pragma once
#ifndef _LOGSERVICE_H_
#define _LOGSERVICE_H_
#include "Proxy/Service/LogService_Proxy.h"

// public functions here
namespace LogService_Proxy
{
	namespace Account
	{
		enum
		{
			ACT_LOGIN = 1,
			ACT_LOGOUT
		};
	};

	namespace Character
	{
		enum
		{
			ACT_CREATE = 1,
			ACT_DESTROY,
			ACT_CONNECT,
			ACT_DISCONNECT
		};
	};

	namespace Item
	{
		enum
		{
			TARGET_UNKNOW = 1,
			TARGET_PLAYER,
			TARGET_PARTNER,
			TARGET_PET,
			TARGET_MOB
		};

		enum
		{
			ACT_CREATE = 1,
			ACT_DESTROY,
			ACT_TRANSFER,
			ACT_STACK_IN,
			ACT_STACK_OUT,
			ACT_DROP,
			ACT_USE,
			ACT_CREATE_TRANSFER,
			ACT_CREATE_WAREHOUSE,
		};
	};

	namespace Quest
	{
		enum
		{
			SOURCE_UNKNOW = 1,
			SOURCE_ITEM,
			SOURCE_NPC
		};

		enum
		{
			ACT_ADD = 1,
			ACT_REMOVE,
			ACT_CANCEL
		};
	};

	namespace Exchange
	{
		enum ProcessType
		{
			PROCESS_TRADING = 0,
			PROCESS_GIVEN,
			PROCESS_HAWKER,
			PROCESS_BANK_DEPOSIT,
			PROCESS_BANK_WITHDRAW,
		};

		enum ExchangeItemType
		{	
			EXCHANGE_ITEM = 0,
			EXCHANGE_PARTNER,
			EXCHANGE_PET,
			EXCHANGE_MONEY,
		};
	};

	namespace YuanBao
	{
		enum
		{
			USEIN_CHARGESHOP = 0,
			USEIN_CHANGECHATFACE,
			USEIN_CHEAT_ADD,
			USEIN_CHEAT_SUB,
		};
	};

	namespace Money_And_Exp
	{
		enum
		{
			SOURCE_UNKNOWN = 0,
			SOURCE_MAINQUEST,		// script
			SOURCE_BATTLE,			// script
			SOURCE_SHOP_SELL,
			SOURCE_ANTIHACK_AWARD,	// no money

			SOURCE_LEVELUP = 1000,
			SOURCE_SHOP_BUY,
			SOURCE_SKILLSHOP_BUY,
			SOURCE_EQ_REPAIR,
			SOURCE_EQ_MAKE,
			SOURCE_LIVESKILL_MAKE,	// useless
			SOURCE_HAWK_TAX,
			SOURCE_BATTLE_DEAD,
			SOURCE_GUILD_BUY,		
			SOURCE_GUILDBUILDING_BUY,
			SOURCE_HOTEL_REST,		// script
			SOURCE_FRIEND_BROTHER_MAKE,		// script
			SOURCE_FRIEND_MARRIAGE_MAKE,	// script
			SOURCE_GENERATION_ACC_BUY,
			SOURCE_GENERATION_EXP_GET,
			SOURCE_WANTED_REWARD_GIVEN,
			SOURCE_WANTED_REWARD_GET,
			SOURCE_FORCE_PK,
		};
	};

	namespace Mix
	{
		enum
		{
			MIX_EQUIPREPAIR = 0, 
			MIX_EQUIPDRILL,
			MIX_GEMINLAY,
			MIX_GEMREMOVE,
			MIX_WEAPONMAKING,
			MIX_CLOTHMAKING,
			MIX_ORNAMENTMAKING,
			MIX_COOKING,
			MIX_HERMETIC,
			MIX_MAKEWINE,
			MIX_GEMMIX,
			MIX_STONEMIX,
		};
	};

	namespace SellYuanBao
	{
		enum
		{
			CHANGE_YB_TO_YBT = 0,
			CHANGE_YBT_TO_YB,
			SELL_YBT,
			BUY_YBT,
			GET_IN_TEMPBANK,
		};
	};

	namespace Partner
	{
		enum
		{
			PARTNER_CREATE = 0,
			PARTNER_DELETE,
		};
	};

	namespace Guild_Fund
	{
		enum
		{
			NO_CHANGE = 0,
			SOURCE_MANOR,
			SOURCE_DONATE,
			SOURCE_BID,
			SOURCE_GUILDWAR,
			SOURCE_OTHER,
		};
	};

	namespace Guild_Prosper
	{
		enum
		{
			NO_CHANGE = 0,
			SOURCE_MANOR,
			SOURCE_OTHER,
		};
	};

	namespace Guild_Donate
	{
		enum
		{
			UNKNOWN = 0,
			SOURCE_MEMBER_DONATE,
			SOURCE_BATTLE_REWARD,
		};
	};


};

#endif //_LOGSERVICE_H_
