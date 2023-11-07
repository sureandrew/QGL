///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_OBJECTSERVICE_STUB_H_
#define _RPC_OBJECTSERVICE_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ObjectService_enum.h"

namespace ObjectService_Stub {

RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
RPCResult CreateAccount(LPCPEER pPeer, RPCContext &context, const String &loginName, const ByteArray &password_hash);
RPCResult DeleteAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id);
RPCResult SetYuanBaoLock(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const Boolean &lock);
RPCResult IsAssetProtected(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id);
RPCResult StoreAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &password, const UInt32 &acctID);
RPCResult SetAssetPassResetDate(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID, const UInt32 &now);
RPCResult GetAssetPassword(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID);
RPCResult GetAccountByName(LPCPEER pPeer, RPCContext &context, const String &loginName, const Boolean &bReactive);
RPCResult GetYaunBaoByAcctID(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id);
RPCResult UpdateAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID, NetObject* acctdata);
RPCResult UpdateAccountAddictInfo(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID, const UInt32 &disconnectTime, const UInt32 &offTime, const UInt32 &onTime);
RPCResult ReleaseAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id);
RPCResult UpdateYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const Int32 &change);
RPCResult UpdateYuanBaoTicket(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const Int32 &change, const ByteArray &password);
RPCResult CB_GetAccountData(LPCPEER pPeer, RPCContext &context, NetObject* acctData);
RPCResult CB_UpdateAccountSuccess(LPCPEER pPeer, RPCContext &context);
RPCResult CB_IsAssetProtected(LPCPEER pPeer, RPCContext &context, const Boolean &bProtext, const Boolean &bNeedResetPass);
RPCResult CB_StoreAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success);
RPCResult CB_ResetAssetPassDate(LPCPEER pPeer, RPCContext &context, const Boolean &success);
RPCResult CB_GetAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &pass);
RPCResult CB_GetYuanBaoByAcctID(LPCPEER pPeer, RPCContext &context, const UInt32 &yuanBao, const UInt32 &yuanBaoticket, const Boolean &yuanBaoLock, const UInt32 &locktime);
RPCResult CB_UpdateYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &yuanBao, const Boolean &success);
RPCResult CB_UpdateYuanBaoTicket(LPCPEER pPeer, RPCContext &context, const UInt32 &yuanBao, const UInt32 &yuanBaoTicket, const UInt8 &msgIndex);
RPCResult CreateCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const String &nickName, const UInt8 &sex, const UInt8 &cclass, NetObject* charData, const UInt8 &detail);
RPCResult DeleteCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &acct_id);
RPCResult KeepCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id);
RPCResult ReleaseCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id);
RPCResult RestoreCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult SetCharacterExpiryDate(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &extendPeriod);
RPCResult SetBankLock(LPCPEER pPeer, RPCContext &context, const UInt32 &charID, const Boolean &lock);
RPCResult GetCharacters(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt8 &detail);
RPCResult HasCharacter(LPCPEER pPeer, RPCContext &context, const String &charName);
RPCResult UpdateCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, NetObject* charData);
RPCResult CB_GetCharacterGroup(LPCPEER pPeer, RPCContext &context, NetGroup* charGroup, const UInt8 &detail);
RPCResult CB_UpdateCharacter(LPCPEER pPeer, RPCContext &context);
RPCResult CB_CreateCharacterSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult CB_DeleteCharacterSuccess(LPCPEER pPeer, RPCContext &context);
RPCResult CB_HasCharacter(LPCPEER pPeer, RPCContext &context, const BOOL &hasChar);
RPCResult CB_QueueNetGroup(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult CB_RestoreCharacterSuccess(LPCPEER pPeer, RPCContext &context);
RPCResult CB_SetCharExpiryDateSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &deletetime);
RPCResult UpdateCharGenAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &uiCharId, const UInt32 &uiHistoryAcc, const UInt32 &uiCurAcc);
RPCResult CB_UpdateCharGenAcc(LPCPEER pPeer, RPCContext &context, const int &iRet);
RPCResult CreateItemList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &item_ids, const UInt32 &owner_id, const UInt8Vector &bagIndices, const UInt8 &detail);
RPCResult SetItemLock(LPCPEER pPeer, RPCContext &context, const UInt32 &itemUID, const Boolean &lock);
RPCResult GetItemsByOwner(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt8 &inWareHouse, const UInt8 &detail);
RPCResult UpdateItemList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &itemList, const UInt32 &owner_id, NetGroup* grp, const Boolean &separated);
RPCResult CB_CreateItemDone(LPCPEER pPeer, RPCContext &context);
RPCResult CB_GetItemData(LPCPEER pPeer, RPCContext &context, NetObject* itemData, const UInt8 &detail);
RPCResult CB_GetItemGroup(LPCPEER pPeer, RPCContext &context, NetGroup* itemGroup, const UInt8 &detail);
RPCResult CB_UpdateItemList(LPCPEER pPeer, RPCContext &context, NetGroup* itemGroup);
RPCResult CB_SetAssetLock(LPCPEER pPeer, RPCContext &context, const Boolean &success);
RPCResult CreatePartner(LPCPEER pPeer, RPCContext &context, const UInt32 &mob_id, const UInt32 &level, const UInt32 &owner_id, const UInt8 &detail);
RPCResult SetPartnerLock(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID, const Boolean &lock);
RPCResult GetPartnersByCharID(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &inWareHouse, const UInt8 &detail);
RPCResult UpdatePartnerList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerList, NetGroup* grp, const Boolean &separated);
RPCResult CB_CreatePartnerSuccess(LPCPEER pPeer, RPCContext &context, NetObject* partner);
RPCResult CB_GetPartnerGroup(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup, const UInt8 &detail);
RPCResult CB_UpdatePartnerList(LPCPEER pPeer, RPCContext &context, NetGroup* newGroup);
RPCResult CreateSkillList(LPCPEER pPeer, RPCContext &context, const UInt8 &owner_type, const UInt32Vector &owner_ids, const UInt16Vector &skill_ids, const UInt16Vector &skill_levels);
RPCResult UpdateSkillList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt8 &owner_type, NetGroup* skillData);
RPCResult DeleteSkillList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16Vector &skill_ids, const UInt8 &owner_type);
RPCResult GetSkillList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &owner_ids, const UInt8Vector &owner_types);
RPCResult CB_GetSkillList(LPCPEER pPeer, RPCContext &context, NetGroup* skillDataGroup);
RPCResult CB_CreateSkillDone(LPCPEER pPeer, RPCContext &context, NetObject* skillData);
RPCResult CB_DeleteSKillDone(LPCPEER pPeer, RPCContext &context);
RPCResult CB_UpdateSkillList(LPCPEER pPeer, RPCContext &context);
RPCResult GetQuestList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id);
RPCResult UpdateQuestLogs(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, NetGroup* grp);
RPCResult CB_GetQuestList(LPCPEER pPeer, RPCContext &context, NetGroup* questLogDataGroup, NetGroup* questTypeDataGroup, NetGroup* questFlagDataGroup);
RPCResult CB_UpdateQuestLogs(LPCPEER pPeer, RPCContext &context);
RPCResult CB_QueueQuests(LPCPEER pPeer, RPCContext &context, NetGroup* questLogs, NetGroup* questTypes, NetGroup* questFlags);
RPCResult UpdateQuestTypes(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, NetGroup* grp);
RPCResult CB_UpdateQuestTypes(LPCPEER pPeer, RPCContext &context);
RPCResult UpdateQuestFlags(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, NetGroup* grp, const UInt16Vector &del_ids);
RPCResult CB_UpdateQuestFlags(LPCPEER pPeer, RPCContext &context);
RPCResult CreateMessenger(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const UInt16 &relations, const UInt8 &group, const UInt16 &friendly);
RPCResult DeleteMessenger(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id);
RPCResult DeleteMessengerList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id);
RPCResult UpdateMessenger(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, NetObject* relation);
RPCResult GetMessengerLst(LPCPEER pPeer, RPCContext &context, const UInt32 &player_char_id);
RPCResult CB_DeleteMessenger(LPCPEER pPeer, RPCContext &context);
RPCResult CB_DeleteMessengerList(LPCPEER pPeer, RPCContext &context);
RPCResult CB_UpdateMessenger(LPCPEER pPeer, RPCContext &context);
RPCResult CB_CreateMessenger(LPCPEER pPeer, RPCContext &context, NetObject* relationData);
RPCResult CB_GetMessengerLst(LPCPEER pPeer, RPCContext &context, NetGroup* messengerDataGroup);
RPCResult GetCharacterRank(LPCPEER pPeer, RPCContext &context, const UInt8 &rankType, const UInt8 &subType);
RPCResult GetGuildRank(LPCPEER pPeer, RPCContext &context);
RPCResult CB_GetCharacterRank(LPCPEER pPeer, RPCContext &context, const UInt8 &rankType, const UInt8 &subType, NetGroup* charRankGrp);
RPCResult CB_GetGuildRank(LPCPEER pPeer, RPCContext &context, NetGroup* charRankGrp);
RPCResult GetChargeShop(LPCPEER pPeer, RPCContext &context, const UInt8 &index);
RPCResult GetCacheChargeShop(LPCPEER pPeer, RPCContext &context, const UInt8 &index);
RPCResult UpdateChargeShop(LPCPEER pPeer, RPCContext &context, NetGroup* chargeShop, const Boolean &needCallBack);
RPCResult CB_GetChargeShop(LPCPEER pPeer, RPCContext &context, NetGroup* chargeShop);
RPCResult CB_UpdateChargeShopSuccess(LPCPEER pPeer, RPCContext &context);
RPCResult CreatePet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_id, const UInt32 &level, const UInt32 &master_id, const UInt32 &father_id, const String &father_name, const UInt32 &mother_id, const String &mother_name, const String &first_master_name);
RPCResult SetPetLock(LPCPEER pPeer, RPCContext &context, const UInt32 &petUID, const Boolean &lock);
RPCResult GetPetList(LPCPEER pPeer, RPCContext &context, const UInt32 &master_id, const UInt8 &inWareHouse);
RPCResult UpdatePetList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &petList, NetGroup* grp, const Boolean &separated);
RPCResult CB_CreatePet(LPCPEER pPeer, RPCContext &context, NetObject* pPetData);
RPCResult CB_PetList(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult CB_UpdatePetList(LPCPEER pPeer, RPCContext &context, NetGroup* newGroup);
RPCResult CreateTitle(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16 &title_id);
RPCResult DeleteTitle(LPCPEER pPeer, RPCContext &context, const UInt32 &title_uid);
RPCResult GetTitleList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id);
RPCResult UpdateTitle(LPCPEER pPeer, RPCContext &context, const UInt32 &title_uid, NetObject* titleData);
RPCResult CB_TitleList(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult CB_DeleteTitle(LPCPEER pPeer, RPCContext &context);
RPCResult CB_CreateTitle(LPCPEER pPeer, RPCContext &context, NetObject* pTitleData);
RPCResult CB_UpdateTitle(LPCPEER pPeer, RPCContext &context);
RPCResult GetMessageCharacters(LPCPEER pPeer, RPCContext &context, const UInt32Vector &charList, const Boolean &bReactive);
RPCResult CB_GetMsgCharacterGroup(LPCPEER pPeer, RPCContext &context, NetGroup* charGroup);
RPCResult KeepMsgCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult ReleaseMsgCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult UpdateMsgCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, NetObject* msgCharData);
RPCResult CB_UpdateMsgCharacter(LPCPEER pPeer, RPCContext &context);
RPCResult CreateGuild(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &guild_name, const UInt8 &guildPost, const String &creator, const String &master);
RPCResult GetGuildByID(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_id);
RPCResult GetGuildByPvPGroup(LPCPEER pPeer, RPCContext &context);
RPCResult GetGuildByPvPBid(LPCPEER pPeer, RPCContext &context);
RPCResult UpdateGuildList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &guild_ids, NetGroup* grp, const Boolean &separated);
RPCResult DeleteGuildList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &guild_uids);
RPCResult ReleaseGuildAndMem(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid);
RPCResult ResetGuildBid(LPCPEER pPeer, RPCContext &context);
RPCResult ResetGuildGroup(LPCPEER pPeer, RPCContext &context);
RPCResult CB_CreateGuild(LPCPEER pPeer, RPCContext &context, NetObject* guild, NetObject* member);
RPCResult CB_GetGuilds(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult CB_UpdateGuildList(LPCPEER pPeer, RPCContext &context, NetGroup* newGroup);
RPCResult CB_DeleteGuildList(LPCPEER pPeer, RPCContext &context);
RPCResult GetGuildMember(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_id);
RPCResult CreateGuildMember(LPCPEER pPeer, RPCContext &context, const UInt32 &player_char_id, const UInt32 &guild_id, const UInt8 &guildPost);
RPCResult DeleteGuildMemberList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &player_ids);
RPCResult UpdateGuildMemberList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &player_ids, const UInt32Vector &guild_ids, NetGroup* grp, const Boolean &separated);
RPCResult CB_UpdateGuildMemberList(LPCPEER pPeer, RPCContext &context, NetGroup* newGroup);
RPCResult CB_CreateGuildMember(LPCPEER pPeer, RPCContext &context, NetObject* newData);
RPCResult CB_DeleteGuildMemberList(LPCPEER pPeer, RPCContext &context);
RPCResult CB_GetGuildMembers(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult GetGiftBoxByAcctID(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID);
RPCResult UpdateGiftBoxByAcctID(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID);
RPCResult DeleteGiftBox(LPCPEER pPeer, RPCContext &context, const UInt32Vector &giftBoxID);
RPCResult CB_GetGiftBoxByAcctID(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult CB_DeleteGiftBox(LPCPEER pPeer, RPCContext &context);
RPCResult GetHawkerHistory(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult CB_GetHawkerHistory(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult UpdateHawkerHistory(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, NetGroup* grp);
RPCResult CB_UpdateHawkerHistory(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult RemoveRelation(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const UInt32 &relation);
RPCResult GetPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
RPCResult CB_GetPartnerCollection(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult CreatePartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &collection_id, const UInt32 &char_id);
RPCResult CB_CreatePartnerCollection(LPCPEER pPeer, RPCContext &context, NetObject* newData);
RPCResult DeletePartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &collection_uid);
RPCResult CB_DeletePartnerCollection(LPCPEER pPeer, RPCContext &context);
RPCResult GetAllSellYuanBao(LPCPEER pPeer, RPCContext &context);
RPCResult AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &sellType, const UInt32 &totalSell, const UInt32 &moneyPerUnit, const ByteArray &password);
RPCResult CB_GetSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* grp, const UInt8 &msgIndex);
RPCResult BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &buyerAcct_id, const UInt32 &sellYuanBaoID, const UInt32 &YBT, const ByteArray &password);
RPCResult CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
RPCResult GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const ByteArray &password);
RPCResult CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt32Vector &recordIDs, const UInt32 &totalYBT, const UInt32 &totalMoney);
RPCResult GetSibling(LPCPEER pPeer, RPCContext &context, const UInt32 &sibling_id);
RPCResult CB_GetSibling(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult CreateSibling(LPCPEER pPeer, RPCContext &context, const String &sibling_title_prefix, const UInt32 &member_id1, const String &member_title_suffix1, const UInt32 &member_id2, const String &member_title_suffix2, const UInt32 &member_id3, const String &member_title_suffix3, const UInt32 &member_id4, const String &member_title_suffix4, const UInt32 &member_id5, const String &member_title_suffix5, const UInt32 &member_id6, const String &member_title_suffix6, const UInt32 &member_id7, const String &member_title_suffix7, const UInt32 &member_id8, const String &member_title_suffix8, const UInt32 &member_id9, const String &member_title_suffix9, const UInt32 &member_id10, const String &member_title_suffix10);
RPCResult CB_CreateSibling(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult UpdateSibling(LPCPEER pPeer, RPCContext &context, NetObject* netObject);
RPCResult CB_UpdateSibling(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult DeleteSibling(LPCPEER pPeer, RPCContext &context, const UInt32 &sibling_id);
RPCResult CB_DeleteSibling(LPCPEER pPeer, RPCContext &context, const Boolean &result);
RPCResult UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const UInt32 &money);
RPCResult GetGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiCharId, const BOOL &bYoung);
RPCResult CB_GetGeneration(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult CreateGeneration(LPCPEER pPeer, RPCContext &context, NetObject* pNewGnt, const String &strSelfName);
RPCResult CB_CreateGeneration(LPCPEER pPeer, RPCContext &context, NetObject* pGeneration);
RPCResult DeleteGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiYoungId, const UInt32 &uiElderId, const BOOL &bYoung);
RPCResult CB_DeleteGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiYoungId, const UInt32 &uiElderId, const BOOL &bSuccess);
RPCResult UpdateGeneration(LPCPEER pPeer, RPCContext &context, NetGroup* pGeneration);
RPCResult CB_UpdateGeneration(LPCPEER pPeer, RPCContext &context);
RPCResult GetMarriage(LPCPEER pPeer, RPCContext &context, const UInt32 &husband_id, const UInt32 &wife_id);
RPCResult CB_GetMarriage(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult CreateMarriage(LPCPEER pPeer, RPCContext &context, const UInt32 &husband_id, const UInt32 &wife_id, const time_t &marriage_time, const UInt32 &state);
RPCResult CB_CreateMarriage(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult UpdateMarriage(LPCPEER pPeer, RPCContext &context, NetObject* netObject);
RPCResult CB_UpdateMarriage(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult DeleteMarriage(LPCPEER pPeer, RPCContext &context, const UInt32 &marriage_id);
RPCResult CB_DeleteMarriage(LPCPEER pPeer, RPCContext &context, const Boolean &result);
RPCResult GetMarriageCount(LPCPEER pPeer, RPCContext &context);
RPCResult CB_GetMarriageCount(LPCPEER pPeer, RPCContext &context, const UInt32 &count);
RPCResult GetSingleMarriageLst(LPCPEER pPeer, RPCContext &context);
RPCResult CB_GetSingleMarriageLst(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult GetNpcFlagsByLineId(LPCPEER pPeer, RPCContext &context, const UInt16 &line_id);
RPCResult CB_GetNpcFlagsByLineId(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult UpdateNpcFlags(LPCPEER pPeer, RPCContext &context, NetGroup* grpUpdate, NetGroup* grpDelete);
RPCResult CB_UpdateNpcFlags(LPCPEER pPeer, RPCContext &context);
// end of generated RPC functions

RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer *pBuf);
RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer *pBuf);

}



#endif //_RPC_OBJECTSERVICE_STUB_H_
