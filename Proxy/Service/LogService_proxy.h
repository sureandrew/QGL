///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_LOGSERVICE_PROXY_H_
#define _RPC_LOGSERVICE_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/LogService_enum.h"

namespace LogService_Proxy {

RPCResult LogAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &ip, const UInt8 &action_type);
RPCResult LogCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt8 &line_id, const UInt8 &action_type);
RPCResult LogItem(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &target_type, const UInt32 &target_id, const UInt32 &item_uid, const UInt32 &item_id, const UInt16 &item_count, const UInt8 &action_type);
RPCResult LogQuest(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &line_id, const UInt8 &source_type, const UInt32 &source_id, const UInt32 &quest_id, const UInt8 &action_type);
RPCResult LogPlayerCount(LPCPEER pPeer, RPCContext &context, const UInt8 &line_id, const UInt16 &player_count);
RPCResult LogGiftBox(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt32 &item_id, const UInt8 &itemType, const UInt8 &quantity, const String &serialNo);
RPCResult LogChargeShop(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt32 &item_id, const UInt16 &item_count, const UInt8 &item_type, const UInt32 &yuanbao);
RPCResult LogExchange(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &target_char_id, const UInt8 &process_type, const UInt8 &item_type, const UInt32 &item_uid, const UInt32 &item_id, const UInt8 &item_count, const UInt32 &money, const UInt32 &tax);
RPCResult LogExchangeForItemList(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &target_char_id, const UInt8 &process_type, const UInt8Vector &item_type, const UInt32Vector &item_uid, const UInt32Vector &item_id, const UInt8Vector &item_count);
RPCResult LogYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt8 &useplace, const UInt32 &yuanbao);
RPCResult LogCheat(LPCPEER pPeer, RPCContext &context, const UInt32 &exe_id, const UInt32 &source_acct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command, const UInt8 &status);
RPCResult LogCharExp(LPCPEER pPeer, RPCContext &context, const UInt16 &exp_type, const Int32 &exp_value, const UInt32 &acct_id, const UInt32 &char_id, const UInt32 &total_exp, const UInt16 &level);
RPCResult LogMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt16 &source, const UInt32 &money, const UInt32 &totalMoney);
RPCResult LogSMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt16 &source, const UInt32 &smoney);
RPCResult LogMix(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &process_type, const UInt32 &result_item_id, const UInt8 &result_item_count, const UInt32Vector &source_item_uid, const UInt32Vector &source_item_id, const UInt8Vector &source_item_count);
RPCResult LogGoodness(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt16 &source, const Int32 &goodness);
RPCResult LogHack(LPCPEER pPeer, RPCContext &context, const String &acctName, const String &charName, const UInt32 &msgTime, const UInt16 &msgType, const String &TypeName, const String &msg);
RPCResult LogSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt8 &process_type, const UInt32 &target_id, const UInt32 &YB, const UInt32 &YBT, const UInt32 &moneyPerUnit, const UInt32 &totalMoney, const UInt32 &tax, const UInt32 &expireDate);
RPCResult LogHonor(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const Int32 &change);
RPCResult LogPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id, const UInt32 &mob_id, const UInt32 &char_id, const UInt8 &process_type);
RPCResult LogGenerationAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId, const UInt32 &uiAcc, const UInt16 &ui16TargetLevel, const UInt16 &ui16RelatedLevel, const UInt32 &uiItemUId, const UInt32 &uiItemId);
RPCResult LogManor(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &fund, const UInt32 &prosper, const UInt8 &scale, const UInt8 &fundSrc, const UInt8 &prosperSrc);
RPCResult LogGuildDonate(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_uid, const Int32 &donate, const UInt32 &total_donate, const UInt8 &source);
// end of generated RPC functions

RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer *pBuf);
RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer *pBuf);

}

INLINE RPCResult LogService_Proxy::LogAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &ip, const UInt8 &action_type)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogAccount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(ip);
	pBuf->Write<UInt8>(action_type);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt8 &line_id, const UInt8 &action_type)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8>(line_id);
	pBuf->Write<UInt8>(action_type);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogItem(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &target_type, const UInt32 &target_id, const UInt32 &item_uid, const UInt32 &item_id, const UInt16 &item_count, const UInt8 &action_type)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8>(target_type);
	pBuf->Write<UInt32>(target_id);
	pBuf->Write<UInt32>(item_uid);
	pBuf->Write<UInt32>(item_id);
	pBuf->Write<UInt16>(item_count);
	pBuf->Write<UInt8>(action_type);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogQuest(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &line_id, const UInt8 &source_type, const UInt32 &source_id, const UInt32 &quest_id, const UInt8 &action_type)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogQuest);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8>(line_id);
	pBuf->Write<UInt8>(source_type);
	pBuf->Write<UInt32>(source_id);
	pBuf->Write<UInt32>(quest_id);
	pBuf->Write<UInt8>(action_type);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogPlayerCount(LPCPEER pPeer, RPCContext &context, const UInt8 &line_id, const UInt16 &player_count)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogPlayerCount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(line_id);
	pBuf->Write<UInt16>(player_count);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogGiftBox(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt32 &item_id, const UInt8 &itemType, const UInt8 &quantity, const String &serialNo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogGiftBox);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(item_id);
	pBuf->Write<UInt8>(itemType);
	pBuf->Write<UInt8>(quantity);
	pBuf->Write<String>(serialNo);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogChargeShop(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt32 &item_id, const UInt16 &item_count, const UInt8 &item_type, const UInt32 &yuanbao)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogChargeShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(item_id);
	pBuf->Write<UInt16>(item_count);
	pBuf->Write<UInt8>(item_type);
	pBuf->Write<UInt32>(yuanbao);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogExchange(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &target_char_id, const UInt8 &process_type, const UInt8 &item_type, const UInt32 &item_uid, const UInt32 &item_id, const UInt8 &item_count, const UInt32 &money, const UInt32 &tax)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogExchange);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(target_char_id);
	pBuf->Write<UInt8>(process_type);
	pBuf->Write<UInt8>(item_type);
	pBuf->Write<UInt32>(item_uid);
	pBuf->Write<UInt32>(item_id);
	pBuf->Write<UInt8>(item_count);
	pBuf->Write<UInt32>(money);
	pBuf->Write<UInt32>(tax);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogExchangeForItemList(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &target_char_id, const UInt8 &process_type, const UInt8Vector &item_type, const UInt32Vector &item_uid, const UInt32Vector &item_id, const UInt8Vector &item_count)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogExchangeForItemList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(target_char_id);
	pBuf->Write<UInt8>(process_type);
	pBuf->Write<UInt8Vector>(item_type);
	pBuf->Write<UInt32Vector>(item_uid);
	pBuf->Write<UInt32Vector>(item_id);
	pBuf->Write<UInt8Vector>(item_count);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt8 &useplace, const UInt32 &yuanbao)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8>(useplace);
	pBuf->Write<UInt32>(yuanbao);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogCheat(LPCPEER pPeer, RPCContext &context, const UInt32 &exe_id, const UInt32 &source_acct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command, const UInt8 &status)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogCheat);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(exe_id);
	pBuf->Write<UInt32>(source_acct_id);
	pBuf->Write<UInt32>(target_acct_id);
	pBuf->Write<UInt32>(target_char_id);
	pBuf->Write<String>(command);
	pBuf->Write<UInt8>(status);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogCharExp(LPCPEER pPeer, RPCContext &context, const UInt16 &exp_type, const Int32 &exp_value, const UInt32 &acct_id, const UInt32 &char_id, const UInt32 &total_exp, const UInt16 &level)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogCharExp);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(exp_type);
	pBuf->Write<Int32>(exp_value);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(total_exp);
	pBuf->Write<UInt16>(level);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt16 &source, const UInt32 &money, const UInt32 &totalMoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogMoney);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt16>(source);
	pBuf->Write<UInt32>(money);
	pBuf->Write<UInt32>(totalMoney);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogSMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt16 &source, const UInt32 &smoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogSMoney);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt16>(source);
	pBuf->Write<UInt32>(smoney);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogMix(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &process_type, const UInt32 &result_item_id, const UInt8 &result_item_count, const UInt32Vector &source_item_uid, const UInt32Vector &source_item_id, const UInt8Vector &source_item_count)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogMix);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8>(process_type);
	pBuf->Write<UInt32>(result_item_id);
	pBuf->Write<UInt8>(result_item_count);
	pBuf->Write<UInt32Vector>(source_item_uid);
	pBuf->Write<UInt32Vector>(source_item_id);
	pBuf->Write<UInt8Vector>(source_item_count);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogGoodness(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt16 &source, const Int32 &goodness)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogGoodness);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt16>(source);
	pBuf->Write<Int32>(goodness);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogHack(LPCPEER pPeer, RPCContext &context, const String &acctName, const String &charName, const UInt32 &msgTime, const UInt16 &msgType, const String &TypeName, const String &msg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogHack);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(acctName);
	pBuf->Write<String>(charName);
	pBuf->Write<UInt32>(msgTime);
	pBuf->Write<UInt16>(msgType);
	pBuf->Write<String>(TypeName);
	pBuf->Write<String>(msg);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt8 &process_type, const UInt32 &target_id, const UInt32 &YB, const UInt32 &YBT, const UInt32 &moneyPerUnit, const UInt32 &totalMoney, const UInt32 &tax, const UInt32 &expireDate)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8>(process_type);
	pBuf->Write<UInt32>(target_id);
	pBuf->Write<UInt32>(YB);
	pBuf->Write<UInt32>(YBT);
	pBuf->Write<UInt32>(moneyPerUnit);
	pBuf->Write<UInt32>(totalMoney);
	pBuf->Write<UInt32>(tax);
	pBuf->Write<UInt32>(expireDate);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogHonor(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const Int32 &change)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogHonor);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<Int32>(change);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id, const UInt32 &mob_id, const UInt32 &char_id, const UInt8 &process_type)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogPartner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(partner_id);
	pBuf->Write<UInt32>(mob_id);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8>(process_type);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogGenerationAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId, const UInt32 &uiAcc, const UInt16 &ui16TargetLevel, const UInt16 &ui16RelatedLevel, const UInt32 &uiItemUId, const UInt32 &uiItemId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogGenerationAcc);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiTargetId);
	pBuf->Write<UInt32>(uiRelatedId);
	pBuf->Write<UInt32>(uiAcc);
	pBuf->Write<UInt16>(ui16TargetLevel);
	pBuf->Write<UInt16>(ui16RelatedLevel);
	pBuf->Write<UInt32>(uiItemUId);
	pBuf->Write<UInt32>(uiItemId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogManor(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &fund, const UInt32 &prosper, const UInt8 &scale, const UInt8 &fundSrc, const UInt8 &prosperSrc)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogManor);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<UInt32>(fund);
	pBuf->Write<UInt32>(prosper);
	pBuf->Write<UInt8>(scale);
	pBuf->Write<UInt8>(fundSrc);
	pBuf->Write<UInt8>(prosperSrc);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult LogService_Proxy::LogGuildDonate(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_uid, const Int32 &donate, const UInt32 &total_donate, const UInt8 &source)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_LogService_LogGuildDonate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<UInt32>(char_uid);
	pBuf->Write<Int32>(donate);
	pBuf->Write<UInt32>(total_donate);
	pBuf->Write<UInt8>(source);
	return RPCSend(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_LOGSERVICE_PROXY_H_
