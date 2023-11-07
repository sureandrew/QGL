///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MESSAGESERVICE_PROXY_H_
#define _RPC_MESSAGESERVICE_PROXY_H_

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

namespace MessageService_Proxy {

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

INLINE RPCResult MessageService_Proxy::RegisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_RegisterProxy);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::UnregisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_UnregisterProxy);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::RegisterMap(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_RegisterMap);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	pBuf->Write<UInt16>(map_id);
	pBuf->Write<UInt8>(line_id);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::UnregisterMap(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_UnregisterMap);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	pBuf->Write<UInt16>(map_id);
	pBuf->Write<UInt8>(line_id);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ReleaseSession(LPCPEER pPeer, RPCContext &context, const SessionID &sid, const Boolean &bDelete)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ReleaseSession);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<SessionID>(sid);
	pBuf->Write<Boolean>(bDelete);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ApplyChange(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const RPCMask &changeMask, const Int32Vector &updatedata, const UInt8 &source)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ApplyChange);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<RPCMask>(changeMask);
	pBuf->Write<Int32Vector>(updatedata);
	pBuf->Write<UInt8>(source);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::GenerateGuildGroup(LPCPEER pPeer, RPCContext &context, const UInt8 &guildWarState)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_GenerateGuildGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(guildWarState);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::GenerateGuildGroupCW(LPCPEER pPeer, RPCContext &context, const UInt8 &guildWarState)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_GenerateGuildGroupCW);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(guildWarState);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::GuildWarResult(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid1, const UInt32 &guild_uid2, const UInt32 &win_guild_uid)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_GuildWarResult);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid1);
	pBuf->Write<UInt32>(guild_uid2);
	pBuf->Write<UInt32>(win_guild_uid);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::KickMember(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &kicker_id, const UInt32 &member_id, const String &reason)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_KickMember);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<UInt32>(kicker_id);
	pBuf->Write<UInt32>(member_id);
	pBuf->Write<String>(reason);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ManorScaleChange(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt8 &scale)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ManorScaleChange);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<UInt8>(scale);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::TransmitGuildMsg(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &guildUID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_TransmitGuildMsg);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(Msg);
	pBuf->Write<UInt32>(guildUID);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::GetAvailService(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_GetAvailService);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::SendHorseLightMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId, const UInt32 &count)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_SendHorseLightMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(Msg);
	pBuf->Write<UInt32>(SourceId);
	pBuf->Write<UInt32>(count);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::SendSystemMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_SendSystemMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(Msg);
	pBuf->Write<UInt32>(SourceId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::SendRumorMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId, const BYTE &SubType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_SendRumorMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(Msg);
	pBuf->Write<UInt32>(SourceId);
	pBuf->Write<BYTE>(SubType);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::BattleEndUpdate(LPCPEER pPeer, RPCContext &context, const UInt32Vector &char_idList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_BattleEndUpdate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(char_idList);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::AddMsgSchedule(LPCPEER pPeer, RPCContext &context, const UInt16 &id, const UInt32 &startTime, const UInt16 &periodCount, const UInt16 &count, const UInt32 &period, const UInt8 &msgType, const String &msg, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_AddMsgSchedule);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(id);
	pBuf->Write<UInt32>(startTime);
	pBuf->Write<UInt16>(periodCount);
	pBuf->Write<UInt16>(count);
	pBuf->Write<UInt32>(period);
	pBuf->Write<UInt8>(msgType);
	pBuf->Write<String>(msg);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::RemoveMsgSchedule(LPCPEER pPeer, RPCContext &context, const UInt16 &id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_RemoveMsgSchedule);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::UpdateCharInfo(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_UpdateCharInfo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ChannelsBlockHandle(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8Vector &bkChannels, const UInt8Vector &unBkChannels)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ChannelsBlockHandle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8Vector>(bkChannels);
	pBuf->Write<UInt8Vector>(unBkChannels);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ChangeMsgIcon(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt16 &iconID, const Boolean &change)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ChangeMsgIcon);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt16>(iconID);
	pBuf->Write<Boolean>(change);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::AddSpeakCount(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const Int16 &count)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_AddSpeakCount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<Int16>(count);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ChangeGDonate(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_id, const Int &gChange, const UInt8 &source)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ChangeGDonate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<Int>(gChange);
	pBuf->Write<UInt8>(source);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::RemoveMessengersByOwner(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_RemoveMessengersByOwner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::RemoveAllGuildApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_RemoveAllGuildApplicant);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::CharacterLeave(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CharacterLeave);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::CreateGuild(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &guild_name, const String &guild_creator, const String &guild_master)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CreateGuild);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<String>(guild_name);
	pBuf->Write<String>(guild_creator);
	pBuf->Write<String>(guild_master);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::OpenGuildApply(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_OpenGuildApply);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::DonateMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_id, const UInt32 &money)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_DonateMoney);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(money);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::InitMessageChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_InitMessageChannel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ResumeMessageChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ResumeMessageChannel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ReleaseMsgCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ReleaseMsgCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::SendPrivateMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId, const UInt32 &TargetID, const String &TargetName, const boolean &isFriend)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_SendPrivateMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(Msg);
	pBuf->Write<UInt32>(SourceId);
	pBuf->Write<UInt32>(TargetID);
	pBuf->Write<String>(TargetName);
	pBuf->Write<boolean>(isFriend);
	return RPCSend<969301656>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::SendFriendMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &Char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_SendFriendMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(Msg);
	pBuf->Write<UInt32>(Char_id);
	return RPCSend<1016207171>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::LeaveChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &ChannelName)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_LeaveChannel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<String>(ChannelName);
	return RPCSend<162798176>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::LeaveAllChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_LeaveAllChannel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<426802447>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::SetListen(LPCPEER pPeer, RPCContext &context, const BOOL &IsListen, const UInt32 &char_Id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_SetListen);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<BOOL>(IsListen);
	pBuf->Write<UInt32>(char_Id);
	return RPCSend<1119703199>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::JoinMessageChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &char_Name, const String &ChannelName, const BYTE &JoinMode)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_JoinMessageChannel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<String>(char_Name);
	pBuf->Write<String>(ChannelName);
	pBuf->Write<BYTE>(JoinMode);
	return RPCSend<2038366817>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::CreateMessenger(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const UInt16Vector &relations, const UInt8 &group, const UInt16 &friendly, const String &targetName)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CreateMessenger);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(target_id);
	pBuf->Write<UInt16Vector>(relations);
	pBuf->Write<UInt8>(group);
	pBuf->Write<UInt16>(friendly);
	pBuf->Write<String>(targetName);
	return RPCSend<734136569>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::RemoveMessenger(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_RemoveMessenger);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(target_id);
	return RPCSend<316161854>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::RemoveRelation(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const UInt16 &relation)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_RemoveRelation);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(target_id);
	pBuf->Write<UInt16>(relation);
	return RPCSend<1869423667>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::RequestInfo(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &name, const bool &update, const bool &tip)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_RequestInfo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<String>(name);
	pBuf->Write<bool>(update);
	pBuf->Write<bool>(tip);
	return RPCSend<659055789>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::GetMessengers(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16 &relation)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_GetMessengers);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt16>(relation);
	return RPCSend<2091322726>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::GetMessengersName(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16 &relation)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_GetMessengersName);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt16>(relation);
	return RPCSend<1107840472>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::GetFriendly(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_GetFriendly);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(target_id);
	return RPCSend<1147099271>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::IsFriend(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_IsFriend);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(target_id);
	return RPCSend<744771546>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ChangeFriendly(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const Int32 &changed)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ChangeFriendly);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(target_id);
	pBuf->Write<Int32>(changed);
	return RPCSend<1864195743>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ApplyGuild(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &guild_uid)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ApplyGuild);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(guild_uid);
	return RPCSend<568080960>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::RequestGuildAim(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_RequestGuildAim);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	return RPCSend<1205224630>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ChangeTitle(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &title)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ChangeTitle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<String>(title);
	return RPCSend<272460563>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::SetMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &money)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_SetMoney);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(money);
	return RPCSend<2053656100>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::GetTargetFromHawk(LPCPEER pPeer, RPCContext &context, const UInt32 &target_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_GetTargetFromHawk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(target_id);
	return RPCSend<1289379044>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId, const UInt16 &count, const UInt32 &money, const UInt32 &guild_uid, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_BuyItemFromBusinessShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(itemId);
	pBuf->Write<Int16>(shopId);
	pBuf->Write<UInt16>(count);
	pBuf->Write<UInt32>(money);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<177542451>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId, const UInt16 &count, const UInt32 &guild_uid, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_SellItemToBusinessShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(itemId);
	pBuf->Write<Int16>(shopId);
	pBuf->Write<UInt16>(count);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<1952972806>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::OpenBusinessShopMenu(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16 &shop_id, const UInt32 &money)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_OpenBusinessShopMenu);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt16>(shop_id);
	pBuf->Write<UInt32>(money);
	return RPCSend<90522801>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::UpdateBusinessShopList(LPCPEER pPeer, RPCContext &context, const UInt16 &shop_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_UpdateBusinessShopList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(shop_id);
	return RPCSend<541354832>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ChangeBusinessProfit(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_id, const Int32 &profit)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ChangeBusinessProfit);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<Int32>(profit);
	return RPCSend<1355167759>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ApplyTrader(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ApplyTrader);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<1550920278>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::GetGenerationChars(LPCPEER pPeer, RPCContext &context, const UInt32Vector &uiVtRelatedIds)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_GetGenerationChars);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(uiVtRelatedIds);
	return RPCSend<554457848>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::GetTargetInfo(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_GetTargetInfo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<561467285>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ClientDisconnect(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ClientDisconnect);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &partner_id, const UInt32 &char_idTo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ShowPublicPartnerDetailFromOtherLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(partner_id);
	pBuf->Write<UInt32>(char_idTo);
	return RPCSend<406526207>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &pet_id, const UInt32 &char_idTo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ShowPublicPetDetailFromOtherLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(pet_id);
	pBuf->Write<UInt32>(char_idTo);
	return RPCSend<1936994031>(pPeer, pBuf);
}

INLINE RPCResult MessageService_Proxy::ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &item_uid, const UInt32 &char_idTo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_ShowPublicItemDetailFromOtherLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(item_uid);
	pBuf->Write<UInt32>(char_idTo);
	return RPCSend<251463633>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1330994912)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1627015832)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1478577585)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(969301656)
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(2136552202)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1016207171)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1000022068)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1771708645)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(225803454)
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(850679110)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(2019961070)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(608457820)
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1076901719)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1718880279)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(302327494)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(162798176)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(426802447)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1119703199)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1326528604)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(2038366817)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(595267595)
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(734136569)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(316161854)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1869423667)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1042965161)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(698774636)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1832067147)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(659055789)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1752050440)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(2020163493)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(2091322726)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(333594757)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1107840472)
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(689261880)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1147099271)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(963340648)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(744771546)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(173611235)
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1864195743)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1097009387)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(568080960)
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1205224630)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(286279200)
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1080500677)
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1004997677)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1398277389)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(769019868)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(272460563)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(2053656100)
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1289379044)
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(355163471)
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(177542451)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(579892677)
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1952972806)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1460503)
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(90522801)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(313945116)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(541354832)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(677403017)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1355167759)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1550920278)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(554457848)
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(561467285)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1879651114)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(406526207)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1936994031)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(251463633)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(541660631)
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(409940819)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(202073965)
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_MESSAGESERVICE_PROXY_H_
