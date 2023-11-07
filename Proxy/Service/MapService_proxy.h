///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MAPSERVICE_PROXY_H_
#define _RPC_MAPSERVICE_PROXY_H_

#include "Common/RPCFunc.h"
#include "Common/common_binary.h"
struct NpcCreateType
{
	UInt32		npc_id;
	UInt16		x;
	UInt16		y;
	UInt32		lifeTime;
	UInt32		eventId;
	UInt32		char_id;
};
typedef StlVector<NpcCreateType, ManagedAllocator<NpcCreateType> > NpcCreateList;
#include "RPCEnum/MapService_enum.h"

namespace MapService_Proxy {

RPCResult RegisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId);
RPCResult UnregisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId);
RPCResult ReleaseSession(LPCPEER pPeer, RPCContext &context, const SessionID &sid, const Boolean &bDelete);
RPCResult SyncProxy(LPCPEER pPeer, RPCContext &context);
RPCResult CB_SyncProxy(LPCPEER pPeer, RPCContext &context);
RPCResult ChangeSessionId(LPCPEER pPeer, RPCContext &context, const SessionID &oldSessId);
RPCResult ResumeSession(LPCPEER pPeer, RPCContext &context);
RPCResult CB_ResumeSession(LPCPEER pPeer, RPCContext &context, const Boolean &valid);
RPCResult RemoteCreateNpc(LPCPEER pPeer, RPCContext &context, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id, const NpcCreateList &npcList);
RPCResult QueryInstance(LPCPEER pPeer, RPCContext &context, const UInt16 &instId, const UInt32 &owner_id);
RPCResult CB_QueryInstance(LPCPEER pPeer, RPCContext &context, const Boolean &bExist, const UInt16 &allowCount);
RPCResult CharacterEnter(LPCPEER pPeer, RPCContext &context, const EnterInfo &info, const UInt32Vector &memberIdList, const AntiAddictInfo &addictInfo, const String &buffCache, const UInt32 &uiLineupId, const int &iKeyPos);
RPCResult CharacterLeave(LPCPEER pPeer, RPCContext &context, const Boolean &bCallback);
RPCResult NpcEnter(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt16 &map_id, const UInt8 &line_id, const UInt16 &x, const UInt16 &y, const UInt32 &followId);
RPCResult ChangeLoadLevel(LPCPEER pPeer, RPCContext &context, const UInt8 &level);
RPCResult RankTitleUpdate(LPCPEER pPeer, RPCContext &context, const UInt32Vector &char_ids, const UInt16Vector &title_ids);
RPCResult GuildUpdate(LPCPEER pPeer, RPCContext &context, NetGroup* addGrp);
RPCResult GuildRemove(LPCPEER pPeer, RPCContext &context, const UInt32Vector &guild_uids);
RPCResult GuildMemberUpdate(LPCPEER pPeer, RPCContext &context, NetGroup* addGrp);
RPCResult GuildMemberRemove(LPCPEER pPeer, RPCContext &context, const UInt32Vector &char_ids);
RPCResult MsgCharacterUpdate(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult ManorForceOut(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_UID, const UInt32Vector &guild_charUIDs, const Boolean &memberKick);
RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg);
RPCResult CB_EnterSuccess(LPCPEER pPeer, RPCContext &context, NetObject* map, NetObject* character);
RPCResult CB_CharacterStuff(LPCPEER pPeer, RPCContext &context, NetObject* bag, NetObject* scriptSys, NetObject* questSys, NetGroup* skillGroup, NetGroup* partnerGroup, NetGroup* petGroup, const UInt8 &addictHealth);
RPCResult CB_LeaveSuccess(LPCPEER pPeer, RPCContext &context, const Boolean &bDelete);
RPCResult CB_CharacterEnter(LPCPEER pPeer, RPCContext &context);
RPCResult CB_PartyEnter(LPCPEER pPeer, RPCContext &context);
RPCResult CB_NpcEnter(LPCPEER pPeer, RPCContext &context);
RPCResult CB_EnterBattle(LPCPEER pPeer, RPCContext &context, NetObject* battle, NetGroup* grp, const UInt8 &PlayerTeam, const UInt8 &battleState, const UInt32 &npcId);
RPCResult CharacterCheat(LPCPEER pPeer, RPCContext &context, const UInt32 &exe_id, const UInt32 &SrcAcct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command);
RPCResult CB_CharacterCheat(LPCPEER pPeer, RPCContext &context);
RPCResult UpdateLimitedItemRemain(LPCPEER pPeer, RPCContext &context, NetGroup* limitedItemData);
RPCResult ShowPublicItemDetail(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &item_uid, const UInt32 &char_idTo);
RPCResult ShowPublicPartnerDetail(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &partner_id, const UInt32 &char_idTo);
RPCResult ShowPublicPetDetail(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &pet_id, const UInt32 &char_idTo);
RPCResult OpenChargeShop(LPCPEER pPeer, RPCContext &context);
RPCResult CloseChargeShop(LPCPEER pPeer, RPCContext &context);
RPCResult CB_OpenChargeShop(LPCPEER pPeer, RPCContext &context, NetGroup* chargeShop, NetGroup* discountGroup, NetObject* limitedItem);
RPCResult CB_SyncCharacter(LPCPEER pPeer, RPCContext &context);
RPCResult CB_SyncItem(LPCPEER pPeer, RPCContext &context);
RPCResult CB_SyncPartner(LPCPEER pPeer, RPCContext &context);
RPCResult CB_AddPartnersSuccess(LPCPEER pPeer, RPCContext &context);
RPCResult CB_ShowPublicItemDetail(LPCPEER pPeer, RPCContext &context, NetObject* item);
RPCResult CB_ShowPublicPartnerDetail(LPCPEER pPeer, RPCContext &context, NetObject* Partner, const UInt16Vector &skillIDs);
RPCResult CB_ShowPublicPetDetail(LPCPEER pPeer, RPCContext &context, NetObject* Pet, const UInt16Vector &skillIDs);
RPCResult SendMapMessage(LPCPEER pPeer, RPCContext &context, const RPCNetID &charNetID, const String &Msg);
RPCResult SendMapEmotion(LPCPEER pPeer, RPCContext &context, const RPCNetID &charNetID, const UInt32 &emo_id, const RPCNetID &TargetNetID, const bool &isPet);
RPCResult CB_ReceiveMapMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName, const SessionIDVector &listeners);
RPCResult CB_ReceiveMapEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const String &SrcName, const String &TargetName, const bool &isPet);
RPCResult CB_ForceLogout(LPCPEER pPeer, RPCContext &context, const String &forceOutMsg);
RPCResult ChangeMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const Int32 &change);
RPCResult ClientDisconnect(LPCPEER pPeer, RPCContext &context);
RPCResult GetPosition(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
RPCResult CB_GetPosition(LPCPEER pPeer, RPCContext &context, const UInt32 &mapX, const UInt32 &mapY);
RPCResult UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money);
RPCResult CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const Boolean &result);
RPCResult DeleteGenerationFromWorld(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId);
RPCResult RecvGenerationAccmulation(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId, const UInt32 &uiAcc);
RPCResult GenerationOnlineNotify(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId);
RPCResult GenerationOfflineNotify(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId);
RPCResult GetGenenrationCharsResult(LPCPEER pPeer, RPCContext &context, NetGroup* pResult);
RPCResult ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &item_uid, const UInt32 &char_idTo);
RPCResult ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &partner_id, const UInt32 &char_idTo);
RPCResult ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &pet_id, const UInt32 &char_idTo);
RPCResult CB_ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, NetObject* Partner, const UInt16Vector &skillIDs, const UInt32 &char_idTo);
RPCResult CB_ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, NetObject* Pet, const UInt16Vector &skillIDs, const UInt32 &char_idTo);
RPCResult CB_ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, NetObject* item, const UInt32 &char_idTo);
// end of generated RPC functions

RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer *pBuf);
RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer *pBuf);

}

INLINE RPCResult MapService_Proxy::RegisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_RegisterProxy);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::UnregisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_UnregisterProxy);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ReleaseSession(LPCPEER pPeer, RPCContext &context, const SessionID &sid, const Boolean &bDelete)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ReleaseSession);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<SessionID>(sid);
	pBuf->Write<Boolean>(bDelete);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::SyncProxy(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_SyncProxy);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ChangeSessionId(LPCPEER pPeer, RPCContext &context, const SessionID &oldSessId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ChangeSessionId);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<SessionID>(oldSessId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ResumeSession(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ResumeSession);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::RemoteCreateNpc(LPCPEER pPeer, RPCContext &context, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id, const NpcCreateList &npcList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_RemoteCreateNpc);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(map_id);
	pBuf->Write<UInt8>(line_id);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<NpcCreateList>(npcList);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::QueryInstance(LPCPEER pPeer, RPCContext &context, const UInt16 &instId, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_QueryInstance);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(instId);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::CharacterEnter(LPCPEER pPeer, RPCContext &context, const EnterInfo &info, const UInt32Vector &memberIdList, const AntiAddictInfo &addictInfo, const String &buffCache, const UInt32 &uiLineupId, const int &iKeyPos)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CharacterEnter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<EnterInfo>(info);
	pBuf->Write<UInt32Vector>(memberIdList);
	pBuf->Write<AntiAddictInfo>(addictInfo);
	pBuf->Write<String>(buffCache);
	pBuf->Write<UInt32>(uiLineupId);
	pBuf->Write<int>(iKeyPos);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::CharacterLeave(LPCPEER pPeer, RPCContext &context, const Boolean &bCallback)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CharacterLeave);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(bCallback);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::NpcEnter(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt16 &map_id, const UInt8 &line_id, const UInt16 &x, const UInt16 &y, const UInt32 &followId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_NpcEnter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(npc_id);
	pBuf->Write<UInt16>(map_id);
	pBuf->Write<UInt8>(line_id);
	pBuf->Write<UInt16>(x);
	pBuf->Write<UInt16>(y);
	pBuf->Write<UInt32>(followId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ChangeLoadLevel(LPCPEER pPeer, RPCContext &context, const UInt8 &level)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ChangeLoadLevel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(level);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::RankTitleUpdate(LPCPEER pPeer, RPCContext &context, const UInt32Vector &char_ids, const UInt16Vector &title_ids)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_RankTitleUpdate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(char_ids);
	pBuf->Write<UInt16Vector>(title_ids);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::GuildUpdate(LPCPEER pPeer, RPCContext &context, NetGroup* addGrp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_GuildUpdate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(addGrp);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::GuildRemove(LPCPEER pPeer, RPCContext &context, const UInt32Vector &guild_uids)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_GuildRemove);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(guild_uids);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::GuildMemberUpdate(LPCPEER pPeer, RPCContext &context, NetGroup* addGrp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_GuildMemberUpdate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(addGrp);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::GuildMemberRemove(LPCPEER pPeer, RPCContext &context, const UInt32Vector &char_ids)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_GuildMemberRemove);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(char_ids);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::MsgCharacterUpdate(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_MsgCharacterUpdate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ManorForceOut(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_UID, const UInt32Vector &guild_charUIDs, const Boolean &memberKick)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ManorForceOut);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_UID);
	pBuf->Write<UInt32Vector>(guild_charUIDs);
	pBuf->Write<Boolean>(memberKick);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::CharacterCheat(LPCPEER pPeer, RPCContext &context, const UInt32 &exe_id, const UInt32 &SrcAcct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CharacterCheat);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(exe_id);
	pBuf->Write<UInt32>(SrcAcct_id);
	pBuf->Write<UInt32>(target_acct_id);
	pBuf->Write<UInt32>(target_char_id);
	pBuf->Write<String>(command);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::UpdateLimitedItemRemain(LPCPEER pPeer, RPCContext &context, NetGroup* limitedItemData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_UpdateLimitedItemRemain);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(limitedItemData);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ShowPublicItemDetail(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &item_uid, const UInt32 &char_idTo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ShowPublicItemDetail);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(item_uid);
	pBuf->Write<UInt32>(char_idTo);
	return RPCSend<1134259944>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ShowPublicPartnerDetail(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &partner_id, const UInt32 &char_idTo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ShowPublicPartnerDetail);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(partner_id);
	pBuf->Write<UInt32>(char_idTo);
	return RPCSend<1569917621>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ShowPublicPetDetail(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &pet_id, const UInt32 &char_idTo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ShowPublicPetDetail);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(pet_id);
	pBuf->Write<UInt32>(char_idTo);
	return RPCSend<1525690276>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::OpenChargeShop(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_OpenChargeShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend<1353540095>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::CloseChargeShop(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CloseChargeShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend<1925281129>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::SendMapMessage(LPCPEER pPeer, RPCContext &context, const RPCNetID &charNetID, const String &Msg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_SendMapMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(charNetID);
	pBuf->Write<String>(Msg);
	return RPCSend<210182392>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::SendMapEmotion(LPCPEER pPeer, RPCContext &context, const RPCNetID &charNetID, const UInt32 &emo_id, const RPCNetID &TargetNetID, const bool &isPet)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_SendMapEmotion);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(charNetID);
	pBuf->Write<UInt32>(emo_id);
	pBuf->Write<RPCNetID>(TargetNetID);
	pBuf->Write<bool>(isPet);
	return RPCSend<1365247911>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ChangeMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const Int32 &change)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ChangeMoney);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<Int32>(change);
	return RPCSend<647310699>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ClientDisconnect(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ClientDisconnect);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::GetPosition(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_GetPosition);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(targetId);
	return RPCSend<874598020>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_UpdateWantedReward);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(targetId);
	pBuf->Write<Int32>(money);
	return RPCSend<249116525>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::DeleteGenerationFromWorld(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_DeleteGenerationFromWorld);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiTargetId);
	pBuf->Write<UInt32>(uiRelatedId);
	return RPCSend<2137140722>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::RecvGenerationAccmulation(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId, const UInt32 &uiAcc)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_RecvGenerationAccmulation);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiTargetId);
	pBuf->Write<UInt32>(uiRelatedId);
	pBuf->Write<UInt32>(uiAcc);
	return RPCSend<727084692>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::GenerationOnlineNotify(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_GenerationOnlineNotify);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiTargetId);
	pBuf->Write<UInt32>(uiRelatedId);
	return RPCSend<334890239>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::GenerationOfflineNotify(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_GenerationOfflineNotify);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiTargetId);
	pBuf->Write<UInt32>(uiRelatedId);
	return RPCSend<1104742176>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::GetGenenrationCharsResult(LPCPEER pPeer, RPCContext &context, NetGroup* pResult)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_GetGenenrationCharsResult);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(pResult);
	return RPCSend<1931876113>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &item_uid, const UInt32 &char_idTo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ShowPublicItemDetailFromOtherLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(item_uid);
	pBuf->Write<UInt32>(char_idTo);
	return RPCSend<1766868985>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &partner_id, const UInt32 &char_idTo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ShowPublicPartnerDetailFromOtherLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(partner_id);
	pBuf->Write<UInt32>(char_idTo);
	return RPCSend<1897221527>(pPeer, pBuf);
}

INLINE RPCResult MapService_Proxy::ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &pet_id, const UInt32 &char_idTo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_ShowPublicPetDetailFromOtherLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(pet_id);
	pBuf->Write<UInt32>(char_idTo);
	return RPCSend<1519024537>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(950751607)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1732650242)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1241854683)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1047143100)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1698965209)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(688273772)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(17040657)
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1376742086)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1134259944)
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
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1569917621)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1525690276)
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1353540095)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1925281129)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1218269948)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1421242889)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(406863196)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(987312133)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1631546235)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(2122804530)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(181617556)
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1441406415)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(210182392)
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1365247911)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1763596625)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(83128094)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1131569201)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(647310699)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(874598020)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1108702494)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(249116525)
	AES_SUB_BYTES
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
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1292175556)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(2137140722)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(727084692)
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(334890239)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1104742176)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1931876113)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1766868985)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1897221527)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1519024537)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(509741308)
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
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(336362802)
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1415328226)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_MAPSERVICE_PROXY_H_
