///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_OBJECTSERVICE_PROXY_H_
#define _RPC_OBJECTSERVICE_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ObjectService_enum.h"

namespace ObjectService_Proxy {

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

INLINE RPCResult ObjectService_Proxy::CreateAccount(LPCPEER pPeer, RPCContext &context, const String &loginName, const ByteArray &password_hash)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreateAccount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(loginName);
	pBuf->Write<ByteArray>(password_hash);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeleteAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeleteAccount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::SetYuanBaoLock(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const Boolean &lock)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_SetYuanBaoLock);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<Boolean>(lock);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::IsAssetProtected(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_IsAssetProtected);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::StoreAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &password, const UInt32 &acctID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_StoreAssetPassword);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ByteArray>(password);
	pBuf->Write<UInt32>(acctID);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::SetAssetPassResetDate(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID, const UInt32 &now)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_SetAssetPassResetDate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acctID);
	pBuf->Write<UInt32>(now);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetAssetPassword(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetAssetPassword);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acctID);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetAccountByName(LPCPEER pPeer, RPCContext &context, const String &loginName, const Boolean &bReactive)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetAccountByName);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(loginName);
	pBuf->Write<Boolean>(bReactive);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetYaunBaoByAcctID(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetYaunBaoByAcctID);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID, NetObject* acctdata)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateAccount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acctID);
	pBuf->Write<NetObject*>(acctdata);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateAccountAddictInfo(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID, const UInt32 &disconnectTime, const UInt32 &offTime, const UInt32 &onTime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateAccountAddictInfo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acctID);
	pBuf->Write<UInt32>(disconnectTime);
	pBuf->Write<UInt32>(offTime);
	pBuf->Write<UInt32>(onTime);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::ReleaseAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_ReleaseAccount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const Int32 &change)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<Int32>(change);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateYuanBaoTicket(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const Int32 &change, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateYuanBaoTicket);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<Int32>(change);
	pBuf->Write<ByteArray>(password);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreateCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const String &nickName, const UInt8 &sex, const UInt8 &cclass, NetObject* charData, const UInt8 &detail)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreateCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<String>(nickName);
	pBuf->Write<UInt8>(sex);
	pBuf->Write<UInt8>(cclass);
	pBuf->Write<NetObject*>(charData);
	pBuf->Write<UInt8>(detail);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeleteCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &acct_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeleteCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(acct_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::KeepCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_KeepCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::ReleaseCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_ReleaseCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::RestoreCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_RestoreCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::SetCharacterExpiryDate(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &extendPeriod)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_SetCharacterExpiryDate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt32>(extendPeriod);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::SetBankLock(LPCPEER pPeer, RPCContext &context, const UInt32 &charID, const Boolean &lock)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_SetBankLock);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(charID);
	pBuf->Write<Boolean>(lock);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetCharacters(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt8 &detail)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetCharacters);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8>(detail);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::HasCharacter(LPCPEER pPeer, RPCContext &context, const String &charName)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_HasCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(charName);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, NetObject* charData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<NetObject*>(charData);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateCharGenAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &uiCharId, const UInt32 &uiHistoryAcc, const UInt32 &uiCurAcc)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateCharGenAcc);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiCharId);
	pBuf->Write<UInt32>(uiHistoryAcc);
	pBuf->Write<UInt32>(uiCurAcc);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreateItemList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &item_ids, const UInt32 &owner_id, const UInt8Vector &bagIndices, const UInt8 &detail)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreateItemList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(item_ids);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt8Vector>(bagIndices);
	pBuf->Write<UInt8>(detail);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::SetItemLock(LPCPEER pPeer, RPCContext &context, const UInt32 &itemUID, const Boolean &lock)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_SetItemLock);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(itemUID);
	pBuf->Write<Boolean>(lock);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetItemsByOwner(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt8 &inWareHouse, const UInt8 &detail)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetItemsByOwner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt8>(inWareHouse);
	pBuf->Write<UInt8>(detail);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateItemList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &itemList, const UInt32 &owner_id, NetGroup* grp, const Boolean &separated)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateItemList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(itemList);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<NetGroup*>(grp);
	pBuf->Write<Boolean>(separated);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreatePartner(LPCPEER pPeer, RPCContext &context, const UInt32 &mob_id, const UInt32 &level, const UInt32 &owner_id, const UInt8 &detail)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreatePartner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(mob_id);
	pBuf->Write<UInt32>(level);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt8>(detail);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::SetPartnerLock(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID, const Boolean &lock)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_SetPartnerLock);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(partnerID);
	pBuf->Write<Boolean>(lock);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetPartnersByCharID(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &inWareHouse, const UInt8 &detail)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetPartnersByCharID);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8>(inWareHouse);
	pBuf->Write<UInt8>(detail);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdatePartnerList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerList, NetGroup* grp, const Boolean &separated)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdatePartnerList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(partnerList);
	pBuf->Write<NetGroup*>(grp);
	pBuf->Write<Boolean>(separated);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreateSkillList(LPCPEER pPeer, RPCContext &context, const UInt8 &owner_type, const UInt32Vector &owner_ids, const UInt16Vector &skill_ids, const UInt16Vector &skill_levels)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreateSkillList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(owner_type);
	pBuf->Write<UInt32Vector>(owner_ids);
	pBuf->Write<UInt16Vector>(skill_ids);
	pBuf->Write<UInt16Vector>(skill_levels);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateSkillList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt8 &owner_type, NetGroup* skillData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateSkillList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt8>(owner_type);
	pBuf->Write<NetGroup*>(skillData);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeleteSkillList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16Vector &skill_ids, const UInt8 &owner_type)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeleteSkillList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt16Vector>(skill_ids);
	pBuf->Write<UInt8>(owner_type);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetSkillList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &owner_ids, const UInt8Vector &owner_types)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetSkillList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(owner_ids);
	pBuf->Write<UInt8Vector>(owner_types);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetQuestList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetQuestList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateQuestLogs(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateQuestLogs);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateQuestTypes(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateQuestTypes);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateQuestFlags(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, NetGroup* grp, const UInt16Vector &del_ids)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateQuestFlags);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<NetGroup*>(grp);
	pBuf->Write<UInt16Vector>(del_ids);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreateMessenger(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const UInt16 &relations, const UInt8 &group, const UInt16 &friendly)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreateMessenger);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(target_id);
	pBuf->Write<UInt16>(relations);
	pBuf->Write<UInt8>(group);
	pBuf->Write<UInt16>(friendly);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeleteMessenger(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeleteMessenger);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(target_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeleteMessengerList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeleteMessengerList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateMessenger(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, NetObject* relation)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateMessenger);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(target_id);
	pBuf->Write<NetObject*>(relation);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetMessengerLst(LPCPEER pPeer, RPCContext &context, const UInt32 &player_char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetMessengerLst);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(player_char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetCharacterRank(LPCPEER pPeer, RPCContext &context, const UInt8 &rankType, const UInt8 &subType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetCharacterRank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(rankType);
	pBuf->Write<UInt8>(subType);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetGuildRank(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetGuildRank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetChargeShop(LPCPEER pPeer, RPCContext &context, const UInt8 &index)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetChargeShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(index);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetCacheChargeShop(LPCPEER pPeer, RPCContext &context, const UInt8 &index)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetCacheChargeShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(index);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateChargeShop(LPCPEER pPeer, RPCContext &context, NetGroup* chargeShop, const Boolean &needCallBack)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateChargeShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(chargeShop);
	pBuf->Write<Boolean>(needCallBack);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreatePet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_id, const UInt32 &level, const UInt32 &master_id, const UInt32 &father_id, const String &father_name, const UInt32 &mother_id, const String &mother_name, const String &first_master_name)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreatePet);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(pet_id);
	pBuf->Write<UInt32>(level);
	pBuf->Write<UInt32>(master_id);
	pBuf->Write<UInt32>(father_id);
	pBuf->Write<String>(father_name);
	pBuf->Write<UInt32>(mother_id);
	pBuf->Write<String>(mother_name);
	pBuf->Write<String>(first_master_name);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::SetPetLock(LPCPEER pPeer, RPCContext &context, const UInt32 &petUID, const Boolean &lock)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_SetPetLock);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(petUID);
	pBuf->Write<Boolean>(lock);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetPetList(LPCPEER pPeer, RPCContext &context, const UInt32 &master_id, const UInt8 &inWareHouse)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetPetList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(master_id);
	pBuf->Write<UInt8>(inWareHouse);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdatePetList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &petList, NetGroup* grp, const Boolean &separated)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdatePetList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(petList);
	pBuf->Write<NetGroup*>(grp);
	pBuf->Write<Boolean>(separated);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreateTitle(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16 &title_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreateTitle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt16>(title_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeleteTitle(LPCPEER pPeer, RPCContext &context, const UInt32 &title_uid)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeleteTitle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(title_uid);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetTitleList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetTitleList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateTitle(LPCPEER pPeer, RPCContext &context, const UInt32 &title_uid, NetObject* titleData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateTitle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(title_uid);
	pBuf->Write<NetObject*>(titleData);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetMessageCharacters(LPCPEER pPeer, RPCContext &context, const UInt32Vector &charList, const Boolean &bReactive)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetMessageCharacters);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(charList);
	pBuf->Write<Boolean>(bReactive);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::KeepMsgCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_KeepMsgCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::ReleaseMsgCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_ReleaseMsgCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateMsgCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, NetObject* msgCharData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateMsgCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<NetObject*>(msgCharData);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreateGuild(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &guild_name, const UInt8 &guildPost, const String &creator, const String &master)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreateGuild);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<String>(guild_name);
	pBuf->Write<UInt8>(guildPost);
	pBuf->Write<String>(creator);
	pBuf->Write<String>(master);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetGuildByID(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetGuildByID);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetGuildByPvPGroup(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetGuildByPvPGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetGuildByPvPBid(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetGuildByPvPBid);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateGuildList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &guild_ids, NetGroup* grp, const Boolean &separated)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateGuildList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(guild_ids);
	pBuf->Write<NetGroup*>(grp);
	pBuf->Write<Boolean>(separated);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeleteGuildList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &guild_uids)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeleteGuildList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(guild_uids);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::ReleaseGuildAndMem(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_ReleaseGuildAndMem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::ResetGuildBid(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_ResetGuildBid);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::ResetGuildGroup(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_ResetGuildGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetGuildMember(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetGuildMember);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreateGuildMember(LPCPEER pPeer, RPCContext &context, const UInt32 &player_char_id, const UInt32 &guild_id, const UInt8 &guildPost)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreateGuildMember);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(player_char_id);
	pBuf->Write<UInt32>(guild_id);
	pBuf->Write<UInt8>(guildPost);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeleteGuildMemberList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &player_ids)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeleteGuildMemberList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(player_ids);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateGuildMemberList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &player_ids, const UInt32Vector &guild_ids, NetGroup* grp, const Boolean &separated)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateGuildMemberList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(player_ids);
	pBuf->Write<UInt32Vector>(guild_ids);
	pBuf->Write<NetGroup*>(grp);
	pBuf->Write<Boolean>(separated);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetGiftBoxByAcctID(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetGiftBoxByAcctID);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acctID);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateGiftBoxByAcctID(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateGiftBoxByAcctID);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acctID);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeleteGiftBox(LPCPEER pPeer, RPCContext &context, const UInt32Vector &giftBoxID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeleteGiftBox);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(giftBoxID);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetHawkerHistory(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetHawkerHistory);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateHawkerHistory(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateHawkerHistory);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::RemoveRelation(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const UInt32 &relation)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_RemoveRelation);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(target_id);
	pBuf->Write<UInt32>(relation);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetPartnerCollection);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreatePartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &collection_id, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreatePartnerCollection);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(collection_id);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeletePartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &collection_uid)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeletePartnerCollection);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(collection_uid);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetAllSellYuanBao(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetAllSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &sellType, const UInt32 &totalSell, const UInt32 &moneyPerUnit, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_AddSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8>(sellType);
	pBuf->Write<UInt32>(totalSell);
	pBuf->Write<UInt32>(moneyPerUnit);
	pBuf->Write<ByteArray>(password);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &buyerAcct_id, const UInt32 &sellYuanBaoID, const UInt32 &YBT, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_BuySellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(buyerAcct_id);
	pBuf->Write<UInt32>(sellYuanBaoID);
	pBuf->Write<UInt32>(YBT);
	pBuf->Write<ByteArray>(password);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetTempBank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<ByteArray>(password);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetSibling(LPCPEER pPeer, RPCContext &context, const UInt32 &sibling_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetSibling);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(sibling_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreateSibling(LPCPEER pPeer, RPCContext &context, const String &sibling_title_prefix, const UInt32 &member_id1, const String &member_title_suffix1, const UInt32 &member_id2, const String &member_title_suffix2, const UInt32 &member_id3, const String &member_title_suffix3, const UInt32 &member_id4, const String &member_title_suffix4, const UInt32 &member_id5, const String &member_title_suffix5, const UInt32 &member_id6, const String &member_title_suffix6, const UInt32 &member_id7, const String &member_title_suffix7, const UInt32 &member_id8, const String &member_title_suffix8, const UInt32 &member_id9, const String &member_title_suffix9, const UInt32 &member_id10, const String &member_title_suffix10)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreateSibling);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(sibling_title_prefix);
	pBuf->Write<UInt32>(member_id1);
	pBuf->Write<String>(member_title_suffix1);
	pBuf->Write<UInt32>(member_id2);
	pBuf->Write<String>(member_title_suffix2);
	pBuf->Write<UInt32>(member_id3);
	pBuf->Write<String>(member_title_suffix3);
	pBuf->Write<UInt32>(member_id4);
	pBuf->Write<String>(member_title_suffix4);
	pBuf->Write<UInt32>(member_id5);
	pBuf->Write<String>(member_title_suffix5);
	pBuf->Write<UInt32>(member_id6);
	pBuf->Write<String>(member_title_suffix6);
	pBuf->Write<UInt32>(member_id7);
	pBuf->Write<String>(member_title_suffix7);
	pBuf->Write<UInt32>(member_id8);
	pBuf->Write<String>(member_title_suffix8);
	pBuf->Write<UInt32>(member_id9);
	pBuf->Write<String>(member_title_suffix9);
	pBuf->Write<UInt32>(member_id10);
	pBuf->Write<String>(member_title_suffix10);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateSibling(LPCPEER pPeer, RPCContext &context, NetObject* netObject)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateSibling);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(netObject);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeleteSibling(LPCPEER pPeer, RPCContext &context, const UInt32 &sibling_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeleteSibling);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(sibling_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const UInt32 &money)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateWantedReward);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(targetId);
	pBuf->Write<UInt32>(money);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiCharId, const BOOL &bYoung)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetGeneration);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiCharId);
	pBuf->Write<BOOL>(bYoung);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreateGeneration(LPCPEER pPeer, RPCContext &context, NetObject* pNewGnt, const String &strSelfName)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreateGeneration);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(pNewGnt);
	pBuf->Write<String>(strSelfName);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeleteGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiYoungId, const UInt32 &uiElderId, const BOOL &bYoung)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeleteGeneration);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiYoungId);
	pBuf->Write<UInt32>(uiElderId);
	pBuf->Write<BOOL>(bYoung);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateGeneration(LPCPEER pPeer, RPCContext &context, NetGroup* pGeneration)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateGeneration);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(pGeneration);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetMarriage(LPCPEER pPeer, RPCContext &context, const UInt32 &husband_id, const UInt32 &wife_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetMarriage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(husband_id);
	pBuf->Write<UInt32>(wife_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::CreateMarriage(LPCPEER pPeer, RPCContext &context, const UInt32 &husband_id, const UInt32 &wife_id, const time_t &marriage_time, const UInt32 &state)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CreateMarriage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(husband_id);
	pBuf->Write<UInt32>(wife_id);
	pBuf->Write<time_t>(marriage_time);
	pBuf->Write<UInt32>(state);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateMarriage(LPCPEER pPeer, RPCContext &context, NetObject* netObject)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateMarriage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(netObject);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::DeleteMarriage(LPCPEER pPeer, RPCContext &context, const UInt32 &marriage_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_DeleteMarriage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(marriage_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetMarriageCount(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetMarriageCount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetSingleMarriageLst(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetSingleMarriageLst);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::GetNpcFlagsByLineId(LPCPEER pPeer, RPCContext &context, const UInt16 &line_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_GetNpcFlagsByLineId);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(line_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ObjectService_Proxy::UpdateNpcFlags(LPCPEER pPeer, RPCContext &context, NetGroup* grpUpdate, NetGroup* grpDelete)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_UpdateNpcFlags);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grpUpdate);
	pBuf->Write<NetGroup*>(grpDelete);
	return RPCSend(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_OBJECTSERVICE_PROXY_H_
