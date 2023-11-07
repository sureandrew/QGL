//-- Common
#include "Common.h"
//-- Self
#include "Stub/Service/ObjectService_stub.h"

#pragma optimize("", off)
RPCResult ObjectService_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
#ifndef CLIENT
	case RPC_ObjectService_CreateAccount:
		{
			String loginName;
			pBuf->Read<String>(loginName);
			ByteArray password_hash;
			pBuf->Read<ByteArray>(password_hash);
			_result = CreateAccount(pPeer, context, loginName, password_hash);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeleteAccount:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			_result = DeleteAccount(pPeer, context, acct_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_SetYuanBaoLock:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			Boolean lock;
			pBuf->Read<Boolean>(lock);
			_result = SetYuanBaoLock(pPeer, context, acct_id, lock);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_IsAssetProtected:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			_result = IsAssetProtected(pPeer, context, acct_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_StoreAssetPassword:
		{
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			UInt32 acctID;
			pBuf->Read<UInt32>(acctID);
			_result = StoreAssetPassword(pPeer, context, password, acctID);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_SetAssetPassResetDate:
		{
			UInt32 acctID;
			pBuf->Read<UInt32>(acctID);
			UInt32 now;
			pBuf->Read<UInt32>(now);
			_result = SetAssetPassResetDate(pPeer, context, acctID, now);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetAssetPassword:
		{
			UInt32 acctID;
			pBuf->Read<UInt32>(acctID);
			_result = GetAssetPassword(pPeer, context, acctID);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetAccountByName:
		{
			String loginName;
			pBuf->Read<String>(loginName);
			Boolean bReactive;
			pBuf->Read<Boolean>(bReactive);
			_result = GetAccountByName(pPeer, context, loginName, bReactive);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetYaunBaoByAcctID:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			_result = GetYaunBaoByAcctID(pPeer, context, acct_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateAccount:
		{
			UInt32 acctID;
			pBuf->Read<UInt32>(acctID);
			NetObject* acctdata;
			pBuf->Read<NetObject*>(acctdata);
			_result = UpdateAccount(pPeer, context, acctID, acctdata);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateAccountAddictInfo:
		{
			UInt32 acctID;
			pBuf->Read<UInt32>(acctID);
			UInt32 disconnectTime;
			pBuf->Read<UInt32>(disconnectTime);
			UInt32 offTime;
			pBuf->Read<UInt32>(offTime);
			UInt32 onTime;
			pBuf->Read<UInt32>(onTime);
			_result = UpdateAccountAddictInfo(pPeer, context, acctID, disconnectTime, offTime, onTime);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_ReleaseAccount:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			_result = ReleaseAccount(pPeer, context, acct_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateYuanBao:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			Int32 change;
			pBuf->Read<Int32>(change);
			_result = UpdateYuanBao(pPeer, context, acct_id, change);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateYuanBaoTicket:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			Int32 change;
			pBuf->Read<Int32>(change);
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = UpdateYuanBaoTicket(pPeer, context, acct_id, change, password);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreateCharacter:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			String nickName;
			pBuf->Read<String>(nickName);
			UInt8 sex;
			pBuf->Read<UInt8>(sex);
			UInt8 cclass;
			pBuf->Read<UInt8>(cclass);
			NetObject* charData;
			pBuf->Read<NetObject*>(charData);
			UInt8 detail;
			pBuf->Read<UInt8>(detail);
			_result = CreateCharacter(pPeer, context, acct_id, nickName, sex, cclass, charData, detail);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeleteCharacter:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			_result = DeleteCharacter(pPeer, context, char_id, acct_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_KeepCharacter:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = KeepCharacter(pPeer, context, acct_id, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_ReleaseCharacter:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = ReleaseCharacter(pPeer, context, acct_id, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_RestoreCharacter:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = RestoreCharacter(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_SetCharacterExpiryDate:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 extendPeriod;
			pBuf->Read<UInt32>(extendPeriod);
			_result = SetCharacterExpiryDate(pPeer, context, char_id, extendPeriod);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_SetBankLock:
		{
			UInt32 charID;
			pBuf->Read<UInt32>(charID);
			Boolean lock;
			pBuf->Read<Boolean>(lock);
			_result = SetBankLock(pPeer, context, charID, lock);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetCharacters:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8 detail;
			pBuf->Read<UInt8>(detail);
			_result = GetCharacters(pPeer, context, acct_id, char_id, detail);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_HasCharacter:
		{
			String charName;
			pBuf->Read<String>(charName);
			_result = HasCharacter(pPeer, context, charName);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateCharacter:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			NetObject* charData;
			pBuf->Read<NetObject*>(charData);
			_result = UpdateCharacter(pPeer, context, char_id, charData);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateCharGenAcc:
		{
			UInt32 uiCharId;
			pBuf->Read<UInt32>(uiCharId);
			UInt32 uiHistoryAcc;
			pBuf->Read<UInt32>(uiHistoryAcc);
			UInt32 uiCurAcc;
			pBuf->Read<UInt32>(uiCurAcc);
			_result = UpdateCharGenAcc(pPeer, context, uiCharId, uiHistoryAcc, uiCurAcc);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreateItemList:
		{
			UInt32Vector item_ids;
			pBuf->Read<UInt32Vector>(item_ids);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt8Vector bagIndices;
			pBuf->Read<UInt8Vector>(bagIndices);
			UInt8 detail;
			pBuf->Read<UInt8>(detail);
			_result = CreateItemList(pPeer, context, item_ids, owner_id, bagIndices, detail);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_SetItemLock:
		{
			UInt32 itemUID;
			pBuf->Read<UInt32>(itemUID);
			Boolean lock;
			pBuf->Read<Boolean>(lock);
			_result = SetItemLock(pPeer, context, itemUID, lock);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetItemsByOwner:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt8 inWareHouse;
			pBuf->Read<UInt8>(inWareHouse);
			UInt8 detail;
			pBuf->Read<UInt8>(detail);
			_result = GetItemsByOwner(pPeer, context, owner_id, inWareHouse, detail);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateItemList:
		{
			UInt32Vector itemList;
			pBuf->Read<UInt32Vector>(itemList);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			Boolean separated;
			pBuf->Read<Boolean>(separated);
			_result = UpdateItemList(pPeer, context, itemList, owner_id, grp, separated);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreatePartner:
		{
			UInt32 mob_id;
			pBuf->Read<UInt32>(mob_id);
			UInt32 level;
			pBuf->Read<UInt32>(level);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt8 detail;
			pBuf->Read<UInt8>(detail);
			_result = CreatePartner(pPeer, context, mob_id, level, owner_id, detail);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_SetPartnerLock:
		{
			UInt32 partnerID;
			pBuf->Read<UInt32>(partnerID);
			Boolean lock;
			pBuf->Read<Boolean>(lock);
			_result = SetPartnerLock(pPeer, context, partnerID, lock);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetPartnersByCharID:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8 inWareHouse;
			pBuf->Read<UInt8>(inWareHouse);
			UInt8 detail;
			pBuf->Read<UInt8>(detail);
			_result = GetPartnersByCharID(pPeer, context, char_id, inWareHouse, detail);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdatePartnerList:
		{
			UInt32Vector partnerList;
			pBuf->Read<UInt32Vector>(partnerList);
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			Boolean separated;
			pBuf->Read<Boolean>(separated);
			_result = UpdatePartnerList(pPeer, context, partnerList, grp, separated);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreateSkillList:
		{
			UInt8 owner_type;
			pBuf->Read<UInt8>(owner_type);
			UInt32Vector owner_ids;
			pBuf->Read<UInt32Vector>(owner_ids);
			UInt16Vector skill_ids;
			pBuf->Read<UInt16Vector>(skill_ids);
			UInt16Vector skill_levels;
			pBuf->Read<UInt16Vector>(skill_levels);
			_result = CreateSkillList(pPeer, context, owner_type, owner_ids, skill_ids, skill_levels);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateSkillList:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt8 owner_type;
			pBuf->Read<UInt8>(owner_type);
			NetGroup* skillData;
			pBuf->Read<NetGroup*>(skillData);
			_result = UpdateSkillList(pPeer, context, owner_id, owner_type, skillData);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeleteSkillList:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt16Vector skill_ids;
			pBuf->Read<UInt16Vector>(skill_ids);
			UInt8 owner_type;
			pBuf->Read<UInt8>(owner_type);
			_result = DeleteSkillList(pPeer, context, owner_id, skill_ids, owner_type);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetSkillList:
		{
			UInt32Vector owner_ids;
			pBuf->Read<UInt32Vector>(owner_ids);
			UInt8Vector owner_types;
			pBuf->Read<UInt8Vector>(owner_types);
			_result = GetSkillList(pPeer, context, owner_ids, owner_types);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetQuestList:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			_result = GetQuestList(pPeer, context, owner_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateQuestLogs:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = UpdateQuestLogs(pPeer, context, owner_id, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateQuestTypes:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = UpdateQuestTypes(pPeer, context, owner_id, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateQuestFlags:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			UInt16Vector del_ids;
			pBuf->Read<UInt16Vector>(del_ids);
			_result = UpdateQuestFlags(pPeer, context, owner_id, grp, del_ids);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreateMessenger:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			UInt16 relations;
			pBuf->Read<UInt16>(relations);
			UInt8 group;
			pBuf->Read<UInt8>(group);
			UInt16 friendly;
			pBuf->Read<UInt16>(friendly);
			_result = CreateMessenger(pPeer, context, owner_id, target_id, relations, group, friendly);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeleteMessenger:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			_result = DeleteMessenger(pPeer, context, owner_id, target_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeleteMessengerList:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			_result = DeleteMessengerList(pPeer, context, owner_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateMessenger:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			NetObject* relation;
			pBuf->Read<NetObject*>(relation);
			_result = UpdateMessenger(pPeer, context, owner_id, target_id, relation);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetMessengerLst:
		{
			UInt32 player_char_id;
			pBuf->Read<UInt32>(player_char_id);
			_result = GetMessengerLst(pPeer, context, player_char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetCharacterRank:
		{
			UInt8 rankType;
			pBuf->Read<UInt8>(rankType);
			UInt8 subType;
			pBuf->Read<UInt8>(subType);
			_result = GetCharacterRank(pPeer, context, rankType, subType);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetGuildRank:
		{
			_result = GetGuildRank(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetChargeShop:
		{
			UInt8 index;
			pBuf->Read<UInt8>(index);
			_result = GetChargeShop(pPeer, context, index);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetCacheChargeShop:
		{
			UInt8 index;
			pBuf->Read<UInt8>(index);
			_result = GetCacheChargeShop(pPeer, context, index);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateChargeShop:
		{
			NetGroup* chargeShop;
			pBuf->Read<NetGroup*>(chargeShop);
			Boolean needCallBack;
			pBuf->Read<Boolean>(needCallBack);
			_result = UpdateChargeShop(pPeer, context, chargeShop, needCallBack);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreatePet:
		{
			UInt32 pet_id;
			pBuf->Read<UInt32>(pet_id);
			UInt32 level;
			pBuf->Read<UInt32>(level);
			UInt32 master_id;
			pBuf->Read<UInt32>(master_id);
			UInt32 father_id;
			pBuf->Read<UInt32>(father_id);
			String father_name;
			pBuf->Read<String>(father_name);
			UInt32 mother_id;
			pBuf->Read<UInt32>(mother_id);
			String mother_name;
			pBuf->Read<String>(mother_name);
			String first_master_name;
			pBuf->Read<String>(first_master_name);
			_result = CreatePet(pPeer, context, pet_id, level, master_id, father_id, father_name, mother_id, mother_name, first_master_name);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_SetPetLock:
		{
			UInt32 petUID;
			pBuf->Read<UInt32>(petUID);
			Boolean lock;
			pBuf->Read<Boolean>(lock);
			_result = SetPetLock(pPeer, context, petUID, lock);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetPetList:
		{
			UInt32 master_id;
			pBuf->Read<UInt32>(master_id);
			UInt8 inWareHouse;
			pBuf->Read<UInt8>(inWareHouse);
			_result = GetPetList(pPeer, context, master_id, inWareHouse);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdatePetList:
		{
			UInt32Vector petList;
			pBuf->Read<UInt32Vector>(petList);
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			Boolean separated;
			pBuf->Read<Boolean>(separated);
			_result = UpdatePetList(pPeer, context, petList, grp, separated);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreateTitle:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt16 title_id;
			pBuf->Read<UInt16>(title_id);
			_result = CreateTitle(pPeer, context, owner_id, title_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeleteTitle:
		{
			UInt32 title_uid;
			pBuf->Read<UInt32>(title_uid);
			_result = DeleteTitle(pPeer, context, title_uid);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetTitleList:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			_result = GetTitleList(pPeer, context, owner_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateTitle:
		{
			UInt32 title_uid;
			pBuf->Read<UInt32>(title_uid);
			NetObject* titleData;
			pBuf->Read<NetObject*>(titleData);
			_result = UpdateTitle(pPeer, context, title_uid, titleData);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetMessageCharacters:
		{
			UInt32Vector charList;
			pBuf->Read<UInt32Vector>(charList);
			Boolean bReactive;
			pBuf->Read<Boolean>(bReactive);
			_result = GetMessageCharacters(pPeer, context, charList, bReactive);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_KeepMsgCharacter:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = KeepMsgCharacter(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_ReleaseMsgCharacter:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = ReleaseMsgCharacter(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateMsgCharacter:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			NetObject* msgCharData;
			pBuf->Read<NetObject*>(msgCharData);
			_result = UpdateMsgCharacter(pPeer, context, char_id, msgCharData);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreateGuild:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			String guild_name;
			pBuf->Read<String>(guild_name);
			UInt8 guildPost;
			pBuf->Read<UInt8>(guildPost);
			String creator;
			pBuf->Read<String>(creator);
			String master;
			pBuf->Read<String>(master);
			_result = CreateGuild(pPeer, context, char_id, guild_name, guildPost, creator, master);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetGuildByID:
		{
			UInt32 guild_id;
			pBuf->Read<UInt32>(guild_id);
			_result = GetGuildByID(pPeer, context, guild_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetGuildByPvPGroup:
		{
			_result = GetGuildByPvPGroup(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetGuildByPvPBid:
		{
			_result = GetGuildByPvPBid(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateGuildList:
		{
			UInt32Vector guild_ids;
			pBuf->Read<UInt32Vector>(guild_ids);
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			Boolean separated;
			pBuf->Read<Boolean>(separated);
			_result = UpdateGuildList(pPeer, context, guild_ids, grp, separated);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeleteGuildList:
		{
			UInt32Vector guild_uids;
			pBuf->Read<UInt32Vector>(guild_uids);
			_result = DeleteGuildList(pPeer, context, guild_uids);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_ReleaseGuildAndMem:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			_result = ReleaseGuildAndMem(pPeer, context, guild_uid);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_ResetGuildBid:
		{
			_result = ResetGuildBid(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_ResetGuildGroup:
		{
			_result = ResetGuildGroup(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetGuildMember:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = GetGuildMember(pPeer, context, guild_uid, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreateGuildMember:
		{
			UInt32 player_char_id;
			pBuf->Read<UInt32>(player_char_id);
			UInt32 guild_id;
			pBuf->Read<UInt32>(guild_id);
			UInt8 guildPost;
			pBuf->Read<UInt8>(guildPost);
			_result = CreateGuildMember(pPeer, context, player_char_id, guild_id, guildPost);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeleteGuildMemberList:
		{
			UInt32Vector player_ids;
			pBuf->Read<UInt32Vector>(player_ids);
			_result = DeleteGuildMemberList(pPeer, context, player_ids);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateGuildMemberList:
		{
			UInt32Vector player_ids;
			pBuf->Read<UInt32Vector>(player_ids);
			UInt32Vector guild_ids;
			pBuf->Read<UInt32Vector>(guild_ids);
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			Boolean separated;
			pBuf->Read<Boolean>(separated);
			_result = UpdateGuildMemberList(pPeer, context, player_ids, guild_ids, grp, separated);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetGiftBoxByAcctID:
		{
			UInt32 acctID;
			pBuf->Read<UInt32>(acctID);
			_result = GetGiftBoxByAcctID(pPeer, context, acctID);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateGiftBoxByAcctID:
		{
			UInt32 acctID;
			pBuf->Read<UInt32>(acctID);
			_result = UpdateGiftBoxByAcctID(pPeer, context, acctID);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeleteGiftBox:
		{
			UInt32Vector giftBoxID;
			pBuf->Read<UInt32Vector>(giftBoxID);
			_result = DeleteGiftBox(pPeer, context, giftBoxID);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetHawkerHistory:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = GetHawkerHistory(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateHawkerHistory:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = UpdateHawkerHistory(pPeer, context, char_id, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_RemoveRelation:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			UInt32 relation;
			pBuf->Read<UInt32>(relation);
			_result = RemoveRelation(pPeer, context, owner_id, target_id, relation);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetPartnerCollection:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = GetPartnerCollection(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreatePartnerCollection:
		{
			UInt32 collection_id;
			pBuf->Read<UInt32>(collection_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = CreatePartnerCollection(pPeer, context, collection_id, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeletePartnerCollection:
		{
			UInt32 collection_uid;
			pBuf->Read<UInt32>(collection_uid);
			_result = DeletePartnerCollection(pPeer, context, collection_uid);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetAllSellYuanBao:
		{
			_result = GetAllSellYuanBao(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_AddSellYuanBao:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8 sellType;
			pBuf->Read<UInt8>(sellType);
			UInt32 totalSell;
			pBuf->Read<UInt32>(totalSell);
			UInt32 moneyPerUnit;
			pBuf->Read<UInt32>(moneyPerUnit);
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = AddSellYuanBao(pPeer, context, char_id, sellType, totalSell, moneyPerUnit, password);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_BuySellYuanBao:
		{
			UInt32 buyerAcct_id;
			pBuf->Read<UInt32>(buyerAcct_id);
			UInt32 sellYuanBaoID;
			pBuf->Read<UInt32>(sellYuanBaoID);
			UInt32 YBT;
			pBuf->Read<UInt32>(YBT);
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = BuySellYuanBao(pPeer, context, buyerAcct_id, sellYuanBaoID, YBT, password);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetTempBank:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = GetTempBank(pPeer, context, char_id, password);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetSibling:
		{
			UInt32 sibling_id;
			pBuf->Read<UInt32>(sibling_id);
			_result = GetSibling(pPeer, context, sibling_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreateSibling:
		{
			String sibling_title_prefix;
			pBuf->Read<String>(sibling_title_prefix);
			UInt32 member_id1;
			pBuf->Read<UInt32>(member_id1);
			String member_title_suffix1;
			pBuf->Read<String>(member_title_suffix1);
			UInt32 member_id2;
			pBuf->Read<UInt32>(member_id2);
			String member_title_suffix2;
			pBuf->Read<String>(member_title_suffix2);
			UInt32 member_id3;
			pBuf->Read<UInt32>(member_id3);
			String member_title_suffix3;
			pBuf->Read<String>(member_title_suffix3);
			UInt32 member_id4;
			pBuf->Read<UInt32>(member_id4);
			String member_title_suffix4;
			pBuf->Read<String>(member_title_suffix4);
			UInt32 member_id5;
			pBuf->Read<UInt32>(member_id5);
			String member_title_suffix5;
			pBuf->Read<String>(member_title_suffix5);
			UInt32 member_id6;
			pBuf->Read<UInt32>(member_id6);
			String member_title_suffix6;
			pBuf->Read<String>(member_title_suffix6);
			UInt32 member_id7;
			pBuf->Read<UInt32>(member_id7);
			String member_title_suffix7;
			pBuf->Read<String>(member_title_suffix7);
			UInt32 member_id8;
			pBuf->Read<UInt32>(member_id8);
			String member_title_suffix8;
			pBuf->Read<String>(member_title_suffix8);
			UInt32 member_id9;
			pBuf->Read<UInt32>(member_id9);
			String member_title_suffix9;
			pBuf->Read<String>(member_title_suffix9);
			UInt32 member_id10;
			pBuf->Read<UInt32>(member_id10);
			String member_title_suffix10;
			pBuf->Read<String>(member_title_suffix10);
			_result = CreateSibling(pPeer, context, sibling_title_prefix, member_id1, member_title_suffix1, member_id2, member_title_suffix2, member_id3, member_title_suffix3, member_id4, member_title_suffix4, member_id5, member_title_suffix5, member_id6, member_title_suffix6, member_id7, member_title_suffix7, member_id8, member_title_suffix8, member_id9, member_title_suffix9, member_id10, member_title_suffix10);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateSibling:
		{
			NetObject* netObject;
			pBuf->Read<NetObject*>(netObject);
			_result = UpdateSibling(pPeer, context, netObject);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeleteSibling:
		{
			UInt32 sibling_id;
			pBuf->Read<UInt32>(sibling_id);
			_result = DeleteSibling(pPeer, context, sibling_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateWantedReward:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			UInt32 money;
			pBuf->Read<UInt32>(money);
			_result = UpdateWantedReward(pPeer, context, targetId, money);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetGeneration:
		{
			UInt32 uiCharId;
			pBuf->Read<UInt32>(uiCharId);
			BOOL bYoung;
			pBuf->Read<BOOL>(bYoung);
			_result = GetGeneration(pPeer, context, uiCharId, bYoung);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreateGeneration:
		{
			NetObject* pNewGnt;
			pBuf->Read<NetObject*>(pNewGnt);
			String strSelfName;
			pBuf->Read<String>(strSelfName);
			_result = CreateGeneration(pPeer, context, pNewGnt, strSelfName);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeleteGeneration:
		{
			UInt32 uiYoungId;
			pBuf->Read<UInt32>(uiYoungId);
			UInt32 uiElderId;
			pBuf->Read<UInt32>(uiElderId);
			BOOL bYoung;
			pBuf->Read<BOOL>(bYoung);
			_result = DeleteGeneration(pPeer, context, uiYoungId, uiElderId, bYoung);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateGeneration:
		{
			NetGroup* pGeneration;
			pBuf->Read<NetGroup*>(pGeneration);
			_result = UpdateGeneration(pPeer, context, pGeneration);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetMarriage:
		{
			UInt32 husband_id;
			pBuf->Read<UInt32>(husband_id);
			UInt32 wife_id;
			pBuf->Read<UInt32>(wife_id);
			_result = GetMarriage(pPeer, context, husband_id, wife_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CreateMarriage:
		{
			UInt32 husband_id;
			pBuf->Read<UInt32>(husband_id);
			UInt32 wife_id;
			pBuf->Read<UInt32>(wife_id);
			time_t marriage_time;
			pBuf->Read<time_t>(marriage_time);
			UInt32 state;
			pBuf->Read<UInt32>(state);
			_result = CreateMarriage(pPeer, context, husband_id, wife_id, marriage_time, state);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateMarriage:
		{
			NetObject* netObject;
			pBuf->Read<NetObject*>(netObject);
			_result = UpdateMarriage(pPeer, context, netObject);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_DeleteMarriage:
		{
			UInt32 marriage_id;
			pBuf->Read<UInt32>(marriage_id);
			_result = DeleteMarriage(pPeer, context, marriage_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetMarriageCount:
		{
			_result = GetMarriageCount(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetSingleMarriageLst:
		{
			_result = GetSingleMarriageLst(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_GetNpcFlagsByLineId:
		{
			UInt16 line_id;
			pBuf->Read<UInt16>(line_id);
			_result = GetNpcFlagsByLineId(pPeer, context, line_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_UpdateNpcFlags:
		{
			NetGroup* grpUpdate;
			pBuf->Read<NetGroup*>(grpUpdate);
			NetGroup* grpDelete;
			pBuf->Read<NetGroup*>(grpDelete);
			_result = UpdateNpcFlags(pPeer, context, grpUpdate, grpDelete);
		}
		break;
#endif // CLIENT
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}
#pragma optimize("", on)

RPCResult ObjectService_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_Error);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(errCode);
	pBuf->Write<String>(errMsg);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetAccountData(LPCPEER pPeer, RPCContext &context, NetObject* acctData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetAccountData);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(acctData);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateAccountSuccess(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateAccountSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_IsAssetProtected(LPCPEER pPeer, RPCContext &context, const Boolean &bProtext, const Boolean &bNeedResetPass)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_IsAssetProtected);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(bProtext);
	pBuf->Write<Boolean>(bNeedResetPass);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_StoreAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_StoreAssetPassword);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(success);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_ResetAssetPassDate(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_ResetAssetPassDate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(success);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &pass)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetAssetPassword);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ByteArray>(pass);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetYuanBaoByAcctID(LPCPEER pPeer, RPCContext &context, const UInt32 &yuanBao, const UInt32 &yuanBaoticket, const Boolean &yuanBaoLock, const UInt32 &locktime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetYuanBaoByAcctID);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(yuanBao);
	pBuf->Write<UInt32>(yuanBaoticket);
	pBuf->Write<Boolean>(yuanBaoLock);
	pBuf->Write<UInt32>(locktime);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &yuanBao, const Boolean &success)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(yuanBao);
	pBuf->Write<Boolean>(success);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateYuanBaoTicket(LPCPEER pPeer, RPCContext &context, const UInt32 &yuanBao, const UInt32 &yuanBaoTicket, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateYuanBaoTicket);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(yuanBao);
	pBuf->Write<UInt32>(yuanBaoTicket);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetCharacterGroup(LPCPEER pPeer, RPCContext &context, NetGroup* charGroup, const UInt8 &detail)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetCharacterGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(charGroup);
	pBuf->Write<UInt8>(detail);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateCharacter(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreateCharacterSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreateCharacterSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_DeleteCharacterSuccess(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_DeleteCharacterSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_HasCharacter(LPCPEER pPeer, RPCContext &context, const BOOL &hasChar)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_HasCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<BOOL>(hasChar);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_QueueNetGroup(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_QueueNetGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_RestoreCharacterSuccess(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_RestoreCharacterSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_SetCharExpiryDateSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &deletetime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_SetCharExpiryDateSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(deletetime);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateCharGenAcc(LPCPEER pPeer, RPCContext &context, const int &iRet)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateCharGenAcc);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<int>(iRet);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreateItemDone(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreateItemDone);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetItemData(LPCPEER pPeer, RPCContext &context, NetObject* itemData, const UInt8 &detail)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetItemData);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(itemData);
	pBuf->Write<UInt8>(detail);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetItemGroup(LPCPEER pPeer, RPCContext &context, NetGroup* itemGroup, const UInt8 &detail)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetItemGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(itemGroup);
	pBuf->Write<UInt8>(detail);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateItemList(LPCPEER pPeer, RPCContext &context, NetGroup* itemGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateItemList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(itemGroup);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_SetAssetLock(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_SetAssetLock);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(success);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreatePartnerSuccess(LPCPEER pPeer, RPCContext &context, NetObject* partner)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreatePartnerSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(partner);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetPartnerGroup(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup, const UInt8 &detail)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetPartnerGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(partnerGroup);
	pBuf->Write<UInt8>(detail);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdatePartnerList(LPCPEER pPeer, RPCContext &context, NetGroup* newGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdatePartnerList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(newGroup);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetSkillList(LPCPEER pPeer, RPCContext &context, NetGroup* skillDataGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetSkillList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(skillDataGroup);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreateSkillDone(LPCPEER pPeer, RPCContext &context, NetObject* skillData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreateSkillDone);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(skillData);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_DeleteSKillDone(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_DeleteSKillDone);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateSkillList(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateSkillList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetQuestList(LPCPEER pPeer, RPCContext &context, NetGroup* questLogDataGroup, NetGroup* questTypeDataGroup, NetGroup* questFlagDataGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetQuestList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(questLogDataGroup);
	pBuf->Write<NetGroup*>(questTypeDataGroup);
	pBuf->Write<NetGroup*>(questFlagDataGroup);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateQuestLogs(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateQuestLogs);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_QueueQuests(LPCPEER pPeer, RPCContext &context, NetGroup* questLogs, NetGroup* questTypes, NetGroup* questFlags)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_QueueQuests);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(questLogs);
	pBuf->Write<NetGroup*>(questTypes);
	pBuf->Write<NetGroup*>(questFlags);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateQuestTypes(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateQuestTypes);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateQuestFlags(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateQuestFlags);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_DeleteMessenger(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_DeleteMessenger);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_DeleteMessengerList(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_DeleteMessengerList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateMessenger(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateMessenger);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreateMessenger(LPCPEER pPeer, RPCContext &context, NetObject* relationData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreateMessenger);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(relationData);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetMessengerLst(LPCPEER pPeer, RPCContext &context, NetGroup* messengerDataGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetMessengerLst);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(messengerDataGroup);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetCharacterRank(LPCPEER pPeer, RPCContext &context, const UInt8 &rankType, const UInt8 &subType, NetGroup* charRankGrp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetCharacterRank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(rankType);
	pBuf->Write<UInt8>(subType);
	pBuf->Write<NetGroup*>(charRankGrp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetGuildRank(LPCPEER pPeer, RPCContext &context, NetGroup* charRankGrp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetGuildRank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(charRankGrp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetChargeShop(LPCPEER pPeer, RPCContext &context, NetGroup* chargeShop)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetChargeShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(chargeShop);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateChargeShopSuccess(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateChargeShopSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreatePet(LPCPEER pPeer, RPCContext &context, NetObject* pPetData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreatePet);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(pPetData);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_PetList(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_PetList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdatePetList(LPCPEER pPeer, RPCContext &context, NetGroup* newGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdatePetList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(newGroup);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_TitleList(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_TitleList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_DeleteTitle(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_DeleteTitle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreateTitle(LPCPEER pPeer, RPCContext &context, NetObject* pTitleData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreateTitle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(pTitleData);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateTitle(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateTitle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetMsgCharacterGroup(LPCPEER pPeer, RPCContext &context, NetGroup* charGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetMsgCharacterGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(charGroup);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateMsgCharacter(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateMsgCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreateGuild(LPCPEER pPeer, RPCContext &context, NetObject* guild, NetObject* member)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreateGuild);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(guild);
	pBuf->Write<NetObject*>(member);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetGuilds(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetGuilds);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateGuildList(LPCPEER pPeer, RPCContext &context, NetGroup* newGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateGuildList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(newGroup);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_DeleteGuildList(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_DeleteGuildList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateGuildMemberList(LPCPEER pPeer, RPCContext &context, NetGroup* newGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateGuildMemberList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(newGroup);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreateGuildMember(LPCPEER pPeer, RPCContext &context, NetObject* newData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreateGuildMember);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(newData);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_DeleteGuildMemberList(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_DeleteGuildMemberList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetGuildMembers(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetGuildMembers);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetGiftBoxByAcctID(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetGiftBoxByAcctID);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_DeleteGiftBox(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_DeleteGiftBox);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetHawkerHistory(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetHawkerHistory);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateHawkerHistory(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateHawkerHistory);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetPartnerCollection(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetPartnerCollection);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreatePartnerCollection(LPCPEER pPeer, RPCContext &context, NetObject* newData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreatePartnerCollection);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(newData);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_DeletePartnerCollection(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_DeletePartnerCollection);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* grp, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_BuySellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt32Vector &recordIDs, const UInt32 &totalYBT, const UInt32 &totalMoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetTempBank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(recordIDs);
	pBuf->Write<UInt32>(totalYBT);
	pBuf->Write<UInt32>(totalMoney);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetSibling(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetSibling);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreateSibling(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreateSibling);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateSibling(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateSibling);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_DeleteSibling(LPCPEER pPeer, RPCContext &context, const Boolean &result)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_DeleteSibling);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(result);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetGeneration(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetGeneration);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreateGeneration(LPCPEER pPeer, RPCContext &context, NetObject* pGeneration)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreateGeneration);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(pGeneration);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_DeleteGeneration(LPCPEER pPeer, RPCContext &context, const UInt32 &uiYoungId, const UInt32 &uiElderId, const BOOL &bSuccess)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_DeleteGeneration);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiYoungId);
	pBuf->Write<UInt32>(uiElderId);
	pBuf->Write<BOOL>(bSuccess);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateGeneration(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateGeneration);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetMarriage(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetMarriage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_CreateMarriage(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_CreateMarriage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateMarriage(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateMarriage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_DeleteMarriage(LPCPEER pPeer, RPCContext &context, const Boolean &result)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_DeleteMarriage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(result);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetMarriageCount(LPCPEER pPeer, RPCContext &context, const UInt32 &count)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetMarriageCount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(count);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetSingleMarriageLst(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetSingleMarriageLst);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_GetNpcFlagsByLineId(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_GetNpcFlagsByLineId);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

RPCResult ObjectService_Stub::CB_UpdateNpcFlags(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ObjectService_CB_UpdateNpcFlags);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

