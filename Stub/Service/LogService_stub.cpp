//-- Common
#include "Common.h"
//-- Self
#include "Stub/Service/LogService_stub.h"

#pragma optimize("", off)
RPCResult LogService_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
#ifndef CLIENT
	case RPC_LogService_LogAccount:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 ip;
			pBuf->Read<UInt32>(ip);
			UInt8 action_type;
			pBuf->Read<UInt8>(action_type);
			_result = LogAccount(pPeer, context, acct_id, ip, action_type);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogCharacter:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			UInt8 action_type;
			pBuf->Read<UInt8>(action_type);
			_result = LogCharacter(pPeer, context, acct_id, char_id, line_id, action_type);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogItem:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8 target_type;
			pBuf->Read<UInt8>(target_type);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			UInt32 item_uid;
			pBuf->Read<UInt32>(item_uid);
			UInt32 item_id;
			pBuf->Read<UInt32>(item_id);
			UInt16 item_count;
			pBuf->Read<UInt16>(item_count);
			UInt8 action_type;
			pBuf->Read<UInt8>(action_type);
			_result = LogItem(pPeer, context, char_id, target_type, target_id, item_uid, item_id, item_count, action_type);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogQuest:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			UInt8 source_type;
			pBuf->Read<UInt8>(source_type);
			UInt32 source_id;
			pBuf->Read<UInt32>(source_id);
			UInt32 quest_id;
			pBuf->Read<UInt32>(quest_id);
			UInt8 action_type;
			pBuf->Read<UInt8>(action_type);
			_result = LogQuest(pPeer, context, char_id, line_id, source_type, source_id, quest_id, action_type);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogPlayerCount:
		{
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			UInt16 player_count;
			pBuf->Read<UInt16>(player_count);
			_result = LogPlayerCount(pPeer, context, line_id, player_count);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogGiftBox:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 item_id;
			pBuf->Read<UInt32>(item_id);
			UInt8 itemType;
			pBuf->Read<UInt8>(itemType);
			UInt8 quantity;
			pBuf->Read<UInt8>(quantity);
			String serialNo;
			pBuf->Read<String>(serialNo);
			_result = LogGiftBox(pPeer, context, acct_id, char_id, item_id, itemType, quantity, serialNo);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogChargeShop:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 item_id;
			pBuf->Read<UInt32>(item_id);
			UInt16 item_count;
			pBuf->Read<UInt16>(item_count);
			UInt8 item_type;
			pBuf->Read<UInt8>(item_type);
			UInt32 yuanbao;
			pBuf->Read<UInt32>(yuanbao);
			_result = LogChargeShop(pPeer, context, acct_id, char_id, item_id, item_count, item_type, yuanbao);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogExchange:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 target_char_id;
			pBuf->Read<UInt32>(target_char_id);
			UInt8 process_type;
			pBuf->Read<UInt8>(process_type);
			UInt8 item_type;
			pBuf->Read<UInt8>(item_type);
			UInt32 item_uid;
			pBuf->Read<UInt32>(item_uid);
			UInt32 item_id;
			pBuf->Read<UInt32>(item_id);
			UInt8 item_count;
			pBuf->Read<UInt8>(item_count);
			UInt32 money;
			pBuf->Read<UInt32>(money);
			UInt32 tax;
			pBuf->Read<UInt32>(tax);
			_result = LogExchange(pPeer, context, char_id, target_char_id, process_type, item_type, item_uid, item_id, item_count, money, tax);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogExchangeForItemList:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 target_char_id;
			pBuf->Read<UInt32>(target_char_id);
			UInt8 process_type;
			pBuf->Read<UInt8>(process_type);
			UInt8Vector item_type;
			pBuf->Read<UInt8Vector>(item_type);
			UInt32Vector item_uid;
			pBuf->Read<UInt32Vector>(item_uid);
			UInt32Vector item_id;
			pBuf->Read<UInt32Vector>(item_id);
			UInt8Vector item_count;
			pBuf->Read<UInt8Vector>(item_count);
			_result = LogExchangeForItemList(pPeer, context, char_id, target_char_id, process_type, item_type, item_uid, item_id, item_count);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogYuanBao:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8 useplace;
			pBuf->Read<UInt8>(useplace);
			UInt32 yuanbao;
			pBuf->Read<UInt32>(yuanbao);
			_result = LogYuanBao(pPeer, context, acct_id, char_id, useplace, yuanbao);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogCheat:
		{
			UInt32 exe_id;
			pBuf->Read<UInt32>(exe_id);
			UInt32 source_acct_id;
			pBuf->Read<UInt32>(source_acct_id);
			UInt32 target_acct_id;
			pBuf->Read<UInt32>(target_acct_id);
			UInt32 target_char_id;
			pBuf->Read<UInt32>(target_char_id);
			String command;
			pBuf->Read<String>(command);
			UInt8 status;
			pBuf->Read<UInt8>(status);
			_result = LogCheat(pPeer, context, exe_id, source_acct_id, target_acct_id, target_char_id, command, status);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogCharExp:
		{
			UInt16 exp_type;
			pBuf->Read<UInt16>(exp_type);
			Int32 exp_value;
			pBuf->Read<Int32>(exp_value);
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 total_exp;
			pBuf->Read<UInt32>(total_exp);
			UInt16 level;
			pBuf->Read<UInt16>(level);
			_result = LogCharExp(pPeer, context, exp_type, exp_value, acct_id, char_id, total_exp, level);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogMoney:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt16 source;
			pBuf->Read<UInt16>(source);
			UInt32 money;
			pBuf->Read<UInt32>(money);
			UInt32 totalMoney;
			pBuf->Read<UInt32>(totalMoney);
			_result = LogMoney(pPeer, context, acct_id, char_id, source, money, totalMoney);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogSMoney:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt16 source;
			pBuf->Read<UInt16>(source);
			UInt32 smoney;
			pBuf->Read<UInt32>(smoney);
			_result = LogSMoney(pPeer, context, acct_id, char_id, source, smoney);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogMix:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8 process_type;
			pBuf->Read<UInt8>(process_type);
			UInt32 result_item_id;
			pBuf->Read<UInt32>(result_item_id);
			UInt8 result_item_count;
			pBuf->Read<UInt8>(result_item_count);
			UInt32Vector source_item_uid;
			pBuf->Read<UInt32Vector>(source_item_uid);
			UInt32Vector source_item_id;
			pBuf->Read<UInt32Vector>(source_item_id);
			UInt8Vector source_item_count;
			pBuf->Read<UInt8Vector>(source_item_count);
			_result = LogMix(pPeer, context, char_id, process_type, result_item_id, result_item_count, source_item_uid, source_item_id, source_item_count);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogGoodness:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt16 source;
			pBuf->Read<UInt16>(source);
			Int32 goodness;
			pBuf->Read<Int32>(goodness);
			_result = LogGoodness(pPeer, context, acct_id, char_id, source, goodness);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogHack:
		{
			String acctName;
			pBuf->Read<String>(acctName);
			String charName;
			pBuf->Read<String>(charName);
			UInt32 msgTime;
			pBuf->Read<UInt32>(msgTime);
			UInt16 msgType;
			pBuf->Read<UInt16>(msgType);
			String TypeName;
			pBuf->Read<String>(TypeName);
			String msg;
			pBuf->Read<String>(msg);
			_result = LogHack(pPeer, context, acctName, charName, msgTime, msgType, TypeName, msg);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogSellYuanBao:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8 process_type;
			pBuf->Read<UInt8>(process_type);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			UInt32 YB;
			pBuf->Read<UInt32>(YB);
			UInt32 YBT;
			pBuf->Read<UInt32>(YBT);
			UInt32 moneyPerUnit;
			pBuf->Read<UInt32>(moneyPerUnit);
			UInt32 totalMoney;
			pBuf->Read<UInt32>(totalMoney);
			UInt32 tax;
			pBuf->Read<UInt32>(tax);
			UInt32 expireDate;
			pBuf->Read<UInt32>(expireDate);
			_result = LogSellYuanBao(pPeer, context, acct_id, char_id, process_type, target_id, YB, YBT, moneyPerUnit, totalMoney, tax, expireDate);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogHonor:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			Int32 change;
			pBuf->Read<Int32>(change);
			_result = LogHonor(pPeer, context, char_id, change);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogPartner:
		{
			UInt32 partner_id;
			pBuf->Read<UInt32>(partner_id);
			UInt32 mob_id;
			pBuf->Read<UInt32>(mob_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8 process_type;
			pBuf->Read<UInt8>(process_type);
			_result = LogPartner(pPeer, context, partner_id, mob_id, char_id, process_type);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogGenerationAcc:
		{
			UInt32 uiTargetId;
			pBuf->Read<UInt32>(uiTargetId);
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			UInt32 uiAcc;
			pBuf->Read<UInt32>(uiAcc);
			UInt16 ui16TargetLevel;
			pBuf->Read<UInt16>(ui16TargetLevel);
			UInt16 ui16RelatedLevel;
			pBuf->Read<UInt16>(ui16RelatedLevel);
			UInt32 uiItemUId;
			pBuf->Read<UInt32>(uiItemUId);
			UInt32 uiItemId;
			pBuf->Read<UInt32>(uiItemId);
			_result = LogGenerationAcc(pPeer, context, uiTargetId, uiRelatedId, uiAcc, ui16TargetLevel, ui16RelatedLevel, uiItemUId, uiItemId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogManor:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			UInt32 fund;
			pBuf->Read<UInt32>(fund);
			UInt32 prosper;
			pBuf->Read<UInt32>(prosper);
			UInt8 scale;
			pBuf->Read<UInt8>(scale);
			UInt8 fundSrc;
			pBuf->Read<UInt8>(fundSrc);
			UInt8 prosperSrc;
			pBuf->Read<UInt8>(prosperSrc);
			_result = LogManor(pPeer, context, guild_uid, fund, prosper, scale, fundSrc, prosperSrc);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_LogService_LogGuildDonate:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			UInt32 char_uid;
			pBuf->Read<UInt32>(char_uid);
			Int32 donate;
			pBuf->Read<Int32>(donate);
			UInt32 total_donate;
			pBuf->Read<UInt32>(total_donate);
			UInt8 source;
			pBuf->Read<UInt8>(source);
			_result = LogGuildDonate(pPeer, context, guild_uid, char_uid, donate, total_donate, source);
		}
		break;
#endif // CLIENT
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}
#pragma optimize("", on)

