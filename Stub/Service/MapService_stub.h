///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MAPSERVICE_STUB_H_
#define _RPC_MAPSERVICE_STUB_H_

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

namespace MapService_Stub {

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



#endif //_RPC_MAPSERVICE_STUB_H_
