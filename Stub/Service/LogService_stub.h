///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_LOGSERVICE_STUB_H_
#define _RPC_LOGSERVICE_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/LogService_enum.h"

namespace LogService_Stub {

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



#endif //_RPC_LOGSERVICE_STUB_H_
