///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MESSAGESERVICE_STUB_H_
#define _RPC_MESSAGESERVICE_STUB_H_

#include "Common/RPCFunc.h"
#include "Common/common_binary.h"
#ifndef SIMCLIENT
#ifndef MESSAGE
#ifndef QGL
#include "Stub/Logic/Item.h"
#include "Stub/Logic/Partner.h"
#include "Stub/Logic/Pet.h"
#else
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/Pet.h"
#endif
#else
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/Pet.h"
#endif
#else
#include "SimClient/Item.h"
#include "SimClient/Partner.h"
#include "SimClient/Pet.h"
#endif
#include "RPCEnum/MessageService_enum.h"

namespace MessageService_Stub {

RPCResult RegisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId);
RPCResult UnregisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId);
RPCResult RegisterMap(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id);
RPCResult UnregisterMap(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id);
RPCResult ReleaseSession(LPCPEER pPeer, RPCContext &context, const SessionID &sid, const Boolean &bDelete);
RPCResult ApplyChange(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const RPCMask &changeMask, const Int32Vector &updatedata, const UInt8 &source);
RPCResult GenerateGuildGroup(LPCPEER pPeer, RPCContext &context, const UInt8 &guildWarState);
RPCResult GenerateGuildGroupCW(LPCPEER pPeer, RPCContext &context, const UInt8 &guildWarState);
RPCResult GuildWarResult(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid1, const UInt32 &guild_uid2, const UInt32 &win_guild_uid);
RPCResult KickMember(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &kicker_id, const UInt32 &member_id, const String &reason);
RPCResult ManorScaleChange(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt8 &scale);
RPCResult TransmitGuildMsg(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &guildUID);
RPCResult GetAvailService(LPCPEER pPeer, RPCContext &context);
RPCResult SendHorseLightMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId, const UInt32 &count);
RPCResult SendSystemMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId);
RPCResult SendRumorMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId, const BYTE &SubType);
RPCResult BattleEndUpdate(LPCPEER pPeer, RPCContext &context, const UInt32Vector &char_idList);
RPCResult CB_BattleEndUpdate(LPCPEER pPeer, RPCContext &context);
RPCResult AddMsgSchedule(LPCPEER pPeer, RPCContext &context, const UInt16 &id, const UInt32 &startTime, const UInt16 &periodCount, const UInt16 &count, const UInt32 &period, const UInt8 &msgType, const String &msg, const UInt32 &char_id);
RPCResult RemoveMsgSchedule(LPCPEER pPeer, RPCContext &context, const UInt16 &id);
RPCResult UpdateCharInfo(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult CB_GetAvailService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId);
RPCResult ChannelsBlockHandle(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8Vector &bkChannels, const UInt8Vector &unBkChannels);
RPCResult ChangeMsgIcon(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt16 &iconID, const Boolean &change);
RPCResult CB_ChangeMsgIcon(LPCPEER pPeer, RPCContext &context, const Boolean &canChange);
RPCResult AddSpeakCount(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const Int16 &count);
RPCResult ChangeGDonate(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_id, const Int &gChange, const UInt8 &source);
RPCResult RemoveMessengersByOwner(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult CB_RemoveMessengersByOwner(LPCPEER pPeer, RPCContext &context);
RPCResult RemoveAllGuildApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult CharacterLeave(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult CreateGuild(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &guild_name, const String &guild_creator, const String &guild_master);
RPCResult OpenGuildApply(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult CB_CreateGuild(LPCPEER pPeer, RPCContext &context, const UInt8 &bResult);
RPCResult DonateMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_id, const UInt32 &money);
RPCResult CB_DonateMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &addedGDonate, const UInt8 &returnFlag, const UInt32 &addGMoney);
RPCResult InitMessageChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult ResumeMessageChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult ReleaseMsgCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg);
RPCResult CB_InitMessageChannel(LPCPEER pPeer, RPCContext &context, const BYTE &Result);
RPCResult CB_ReceiveMessageChannel(LPCPEER pPeer, RPCContext &context, NetObject* channel, const BYTE &Type);
RPCResult SendPrivateMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId, const UInt32 &TargetID, const String &TargetName, const boolean &isFriend);
RPCResult CB_SendPrivateMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result, const boolean &isFriend, const UInt32 &resultFlag);
RPCResult SendFriendMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &Char_id);
RPCResult CB_ReceiveFriendMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName);
RPCResult CB_SendHorseLightMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result);
RPCResult CB_SendSystemMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result);
RPCResult CB_SendRumorMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result);
RPCResult CB_SendShutdownMessage(LPCPEER pPeer, RPCContext &context, const UInt32 &downTime);
RPCResult CB_ReceivePrivateMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName, const UInt32 &SrcChar_id, const String &TargetName, const UInt32 &TargetID, const boolean &isFriend);
RPCResult CB_ReceiveHorseLightMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &count, const SessionIDVector &listeners);
RPCResult CB_ReceiveSystemMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const SessionIDVector &listeners);
RPCResult CB_ReceiveRumorMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const BYTE &SubType, const SessionIDVector &listeners);
RPCResult LeaveChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &ChannelName);
RPCResult LeaveAllChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult SetListen(LPCPEER pPeer, RPCContext &context, const BOOL &IsListen, const UInt32 &char_Id);
RPCResult CB_SetListen(LPCPEER pPeer, RPCContext &context);
RPCResult JoinMessageChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &char_Name, const String &ChannelName, const BYTE &JoinMode);
RPCResult CB_JoinMessageChannel(LPCPEER pPeer, RPCContext &context, const BYTE &Result);
RPCResult CreateMessenger(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const UInt16Vector &relations, const UInt8 &group, const UInt16 &friendly, const String &targetName);
RPCResult RemoveMessenger(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id);
RPCResult RemoveRelation(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const UInt16 &relation);
RPCResult CB_UpdateMessengers(LPCPEER pPeer, RPCContext &context, NetGroup* messengers);
RPCResult CB_UpdateMsgCharInfo(LPCPEER pPeer, RPCContext &context, NetGroup* magCharInfoGrp);
RPCResult CB_RemoveMessengers(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id);
RPCResult RequestInfo(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &name, const bool &update, const bool &tip);
RPCResult CB_RequestInfo(LPCPEER pPeer, RPCContext &context, NetObject* msgCharData, const bool &update);
RPCResult CB_MessageReturn(LPCPEER pPeer, RPCContext &context, const String &msg, const String &name);
RPCResult GetMessengers(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16 &relation);
RPCResult CB_GetMessengers(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32Vector &messgers);
RPCResult GetMessengersName(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16 &relation);
RPCResult CB_GetMessengersName(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const StringVector &messgers_name);
RPCResult GetFriendly(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id);
RPCResult CB_GetFriendly(LPCPEER pPeer, RPCContext &context, const UInt32 &friendly);
RPCResult IsFriend(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id);
RPCResult CB_IsFriend(LPCPEER pPeer, RPCContext &context, const UInt32 &isFriend);
RPCResult ChangeFriendly(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const Int32 &changed);
RPCResult CB_ChangeFriendly(LPCPEER pPeer, RPCContext &context);
RPCResult ApplyGuild(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &guild_uid);
RPCResult RequestGuildAim(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid);
RPCResult CB_ApplyGuild(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid);
RPCResult CB_Guilds(LPCPEER pPeer, RPCContext &context, NetGroup* GuildGrp, const bool &openApply);
RPCResult CB_GuildMembers(LPCPEER pPeer, RPCContext &context, NetGroup* GuildMemberGrp);
RPCResult CB_RequestApplicant(LPCPEER pPeer, RPCContext &context, NetGroup* pGrp);
RPCResult CB_CreateGuildMsg(LPCPEER pPeer, RPCContext &context, const String &guildName, const String &creator, const SessionIDVector &listeners);
RPCResult ChangeTitle(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &title);
RPCResult SetMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &money);
RPCResult GetTargetFromHawk(LPCPEER pPeer, RPCContext &context, const UInt32 &target_id);
RPCResult CB_GetTargetFromHawk(LPCPEER pPeer, RPCContext &context, const bool &isonline, const String &char_name);
RPCResult BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId, const UInt16 &count, const UInt32 &money, const UInt32 &guild_uid, const UInt32 &char_id);
RPCResult CB_BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt16 &finalCount, const UInt32 &finalPrice);
RPCResult SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId, const UInt16 &count, const UInt32 &guild_uid, const UInt32 &char_id);
RPCResult CB_SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt32 &finalPrice);
RPCResult OpenBusinessShopMenu(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16 &shop_id, const UInt32 &money);
RPCResult CB_OpenBusinessShopMenu(LPCPEER pPeer, RPCContext &context, const Boolean &sell, const UInt16 &shop_id, const UInt8 &shopName, const UInt32 &SynTime, const UInt32 &BusinessMoney, const BusinessShopItemInfoVector &ItemInfoList);
RPCResult UpdateBusinessShopList(LPCPEER pPeer, RPCContext &context, const UInt16 &shop_id);
RPCResult CB_UpdateBusinessShopList(LPCPEER pPeer, RPCContext &context, const BusinessShopItemInfoVector &ItemInfoList);
RPCResult ChangeBusinessProfit(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_id, const Int32 &profit);
RPCResult ApplyTrader(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_id);
RPCResult GetGenerationChars(LPCPEER pPeer, RPCContext &context, const UInt32Vector &uiVtRelatedIds);
RPCResult GetTargetInfo(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult CB_GetTargetInfo(LPCPEER pPeer, RPCContext &context, NetObject* msgTargetData);
RPCResult ClientDisconnect(LPCPEER pPeer, RPCContext &context);
RPCResult ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &partner_id, const UInt32 &char_idTo);
RPCResult ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &pet_id, const UInt32 &char_idTo);
RPCResult ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &item_uid, const UInt32 &char_idTo);
RPCResult CB_ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const CItem &item);
RPCResult CB_ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const CPartner &partner, const UInt16Vector &skillIDs);
RPCResult CB_ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const CPet &pet, const UInt16Vector &skillIDs);
// end of generated RPC functions

RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer *pBuf);
RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer *pBuf);

}



#endif //_RPC_MESSAGESERVICE_STUB_H_
