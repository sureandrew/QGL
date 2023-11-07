//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/ObjectService_proxy.h"

#pragma optimize("", off)
RPCResult ObjectService_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
#ifndef CLIENT
	case RPC_ObjectService_CB_Error:
		{
			UInt32 errCode;
			pBuf->Read<UInt32>(errCode);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, errCode, errMsg);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetAccountData:
		{
			NetObject* acctData;
			pBuf->Read<NetObject*>(acctData);
			_result = CB_GetAccountData(pPeer, context, acctData);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateAccountSuccess:
		{
			_result = CB_UpdateAccountSuccess(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_IsAssetProtected:
		{
			Boolean bProtext;
			pBuf->Read<Boolean>(bProtext);
			Boolean bNeedResetPass;
			pBuf->Read<Boolean>(bNeedResetPass);
			_result = CB_IsAssetProtected(pPeer, context, bProtext, bNeedResetPass);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_StoreAssetPassword:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			_result = CB_StoreAssetPassword(pPeer, context, success);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_ResetAssetPassDate:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			_result = CB_ResetAssetPassDate(pPeer, context, success);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetAssetPassword:
		{
			ByteArray pass;
			pBuf->Read<ByteArray>(pass);
			_result = CB_GetAssetPassword(pPeer, context, pass);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetYuanBaoByAcctID:
		{
			UInt32 yuanBao;
			pBuf->Read<UInt32>(yuanBao);
			UInt32 yuanBaoticket;
			pBuf->Read<UInt32>(yuanBaoticket);
			Boolean yuanBaoLock;
			pBuf->Read<Boolean>(yuanBaoLock);
			UInt32 locktime;
			pBuf->Read<UInt32>(locktime);
			_result = CB_GetYuanBaoByAcctID(pPeer, context, yuanBao, yuanBaoticket, yuanBaoLock, locktime);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateYuanBao:
		{
			UInt32 yuanBao;
			pBuf->Read<UInt32>(yuanBao);
			Boolean success;
			pBuf->Read<Boolean>(success);
			_result = CB_UpdateYuanBao(pPeer, context, yuanBao, success);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateYuanBaoTicket:
		{
			UInt32 yuanBao;
			pBuf->Read<UInt32>(yuanBao);
			UInt32 yuanBaoTicket;
			pBuf->Read<UInt32>(yuanBaoTicket);
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_UpdateYuanBaoTicket(pPeer, context, yuanBao, yuanBaoTicket, msgIndex);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetCharacterGroup:
		{
			NetGroup* charGroup;
			pBuf->Read<NetGroup*>(charGroup);
			UInt8 detail;
			pBuf->Read<UInt8>(detail);
			_result = CB_GetCharacterGroup(pPeer, context, charGroup, detail);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateCharacter:
		{
			_result = CB_UpdateCharacter(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreateCharacterSuccess:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = CB_CreateCharacterSuccess(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_DeleteCharacterSuccess:
		{
			_result = CB_DeleteCharacterSuccess(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_HasCharacter:
		{
			BOOL hasChar;
			pBuf->Read<BOOL>(hasChar);
			_result = CB_HasCharacter(pPeer, context, hasChar);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_QueueNetGroup:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_QueueNetGroup(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_RestoreCharacterSuccess:
		{
			_result = CB_RestoreCharacterSuccess(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_SetCharExpiryDateSuccess:
		{
			UInt32 deletetime;
			pBuf->Read<UInt32>(deletetime);
			_result = CB_SetCharExpiryDateSuccess(pPeer, context, deletetime);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateCharGenAcc:
		{
			int iRet;
			pBuf->Read<int>(iRet);
			_result = CB_UpdateCharGenAcc(pPeer, context, iRet);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreateItemDone:
		{
			_result = CB_CreateItemDone(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetItemData:
		{
			NetObject* itemData;
			pBuf->Read<NetObject*>(itemData);
			UInt8 detail;
			pBuf->Read<UInt8>(detail);
			_result = CB_GetItemData(pPeer, context, itemData, detail);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetItemGroup:
		{
			NetGroup* itemGroup;
			pBuf->Read<NetGroup*>(itemGroup);
			UInt8 detail;
			pBuf->Read<UInt8>(detail);
			_result = CB_GetItemGroup(pPeer, context, itemGroup, detail);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateItemList:
		{
			NetGroup* itemGroup;
			pBuf->Read<NetGroup*>(itemGroup);
			_result = CB_UpdateItemList(pPeer, context, itemGroup);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_SetAssetLock:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			_result = CB_SetAssetLock(pPeer, context, success);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreatePartnerSuccess:
		{
			NetObject* partner;
			pBuf->Read<NetObject*>(partner);
			_result = CB_CreatePartnerSuccess(pPeer, context, partner);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetPartnerGroup:
		{
			NetGroup* partnerGroup;
			pBuf->Read<NetGroup*>(partnerGroup);
			UInt8 detail;
			pBuf->Read<UInt8>(detail);
			_result = CB_GetPartnerGroup(pPeer, context, partnerGroup, detail);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdatePartnerList:
		{
			NetGroup* newGroup;
			pBuf->Read<NetGroup*>(newGroup);
			_result = CB_UpdatePartnerList(pPeer, context, newGroup);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetSkillList:
		{
			NetGroup* skillDataGroup;
			pBuf->Read<NetGroup*>(skillDataGroup);
			_result = CB_GetSkillList(pPeer, context, skillDataGroup);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreateSkillDone:
		{
			NetObject* skillData;
			pBuf->Read<NetObject*>(skillData);
			_result = CB_CreateSkillDone(pPeer, context, skillData);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_DeleteSKillDone:
		{
			_result = CB_DeleteSKillDone(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateSkillList:
		{
			_result = CB_UpdateSkillList(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetQuestList:
		{
			NetGroup* questLogDataGroup;
			pBuf->Read<NetGroup*>(questLogDataGroup);
			NetGroup* questTypeDataGroup;
			pBuf->Read<NetGroup*>(questTypeDataGroup);
			NetGroup* questFlagDataGroup;
			pBuf->Read<NetGroup*>(questFlagDataGroup);
			_result = CB_GetQuestList(pPeer, context, questLogDataGroup, questTypeDataGroup, questFlagDataGroup);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateQuestLogs:
		{
			_result = CB_UpdateQuestLogs(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_QueueQuests:
		{
			NetGroup* questLogs;
			pBuf->Read<NetGroup*>(questLogs);
			NetGroup* questTypes;
			pBuf->Read<NetGroup*>(questTypes);
			NetGroup* questFlags;
			pBuf->Read<NetGroup*>(questFlags);
			_result = CB_QueueQuests(pPeer, context, questLogs, questTypes, questFlags);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateQuestTypes:
		{
			_result = CB_UpdateQuestTypes(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateQuestFlags:
		{
			_result = CB_UpdateQuestFlags(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_DeleteMessenger:
		{
			_result = CB_DeleteMessenger(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_DeleteMessengerList:
		{
			_result = CB_DeleteMessengerList(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateMessenger:
		{
			_result = CB_UpdateMessenger(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreateMessenger:
		{
			NetObject* relationData;
			pBuf->Read<NetObject*>(relationData);
			_result = CB_CreateMessenger(pPeer, context, relationData);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetMessengerLst:
		{
			NetGroup* messengerDataGroup;
			pBuf->Read<NetGroup*>(messengerDataGroup);
			_result = CB_GetMessengerLst(pPeer, context, messengerDataGroup);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetCharacterRank:
		{
			UInt8 rankType;
			pBuf->Read<UInt8>(rankType);
			UInt8 subType;
			pBuf->Read<UInt8>(subType);
			NetGroup* charRankGrp;
			pBuf->Read<NetGroup*>(charRankGrp);
			_result = CB_GetCharacterRank(pPeer, context, rankType, subType, charRankGrp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetGuildRank:
		{
			NetGroup* charRankGrp;
			pBuf->Read<NetGroup*>(charRankGrp);
			_result = CB_GetGuildRank(pPeer, context, charRankGrp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetChargeShop:
		{
			NetGroup* chargeShop;
			pBuf->Read<NetGroup*>(chargeShop);
			_result = CB_GetChargeShop(pPeer, context, chargeShop);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateChargeShopSuccess:
		{
			_result = CB_UpdateChargeShopSuccess(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreatePet:
		{
			NetObject* pPetData;
			pBuf->Read<NetObject*>(pPetData);
			_result = CB_CreatePet(pPeer, context, pPetData);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_PetList:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_PetList(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdatePetList:
		{
			NetGroup* newGroup;
			pBuf->Read<NetGroup*>(newGroup);
			_result = CB_UpdatePetList(pPeer, context, newGroup);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_TitleList:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_TitleList(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_DeleteTitle:
		{
			_result = CB_DeleteTitle(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreateTitle:
		{
			NetObject* pTitleData;
			pBuf->Read<NetObject*>(pTitleData);
			_result = CB_CreateTitle(pPeer, context, pTitleData);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateTitle:
		{
			_result = CB_UpdateTitle(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetMsgCharacterGroup:
		{
			NetGroup* charGroup;
			pBuf->Read<NetGroup*>(charGroup);
			_result = CB_GetMsgCharacterGroup(pPeer, context, charGroup);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateMsgCharacter:
		{
			_result = CB_UpdateMsgCharacter(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreateGuild:
		{
			NetObject* guild;
			pBuf->Read<NetObject*>(guild);
			NetObject* member;
			pBuf->Read<NetObject*>(member);
			_result = CB_CreateGuild(pPeer, context, guild, member);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetGuilds:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_GetGuilds(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateGuildList:
		{
			NetGroup* newGroup;
			pBuf->Read<NetGroup*>(newGroup);
			_result = CB_UpdateGuildList(pPeer, context, newGroup);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_DeleteGuildList:
		{
			_result = CB_DeleteGuildList(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateGuildMemberList:
		{
			NetGroup* newGroup;
			pBuf->Read<NetGroup*>(newGroup);
			_result = CB_UpdateGuildMemberList(pPeer, context, newGroup);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreateGuildMember:
		{
			NetObject* newData;
			pBuf->Read<NetObject*>(newData);
			_result = CB_CreateGuildMember(pPeer, context, newData);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_DeleteGuildMemberList:
		{
			_result = CB_DeleteGuildMemberList(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetGuildMembers:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_GetGuildMembers(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetGiftBoxByAcctID:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_GetGiftBoxByAcctID(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_DeleteGiftBox:
		{
			_result = CB_DeleteGiftBox(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetHawkerHistory:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_GetHawkerHistory(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateHawkerHistory:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_UpdateHawkerHistory(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetPartnerCollection:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_GetPartnerCollection(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreatePartnerCollection:
		{
			NetObject* newData;
			pBuf->Read<NetObject*>(newData);
			_result = CB_CreatePartnerCollection(pPeer, context, newData);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_DeletePartnerCollection:
		{
			_result = CB_DeletePartnerCollection(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetSellYuanBao:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_GetSellYuanBao(pPeer, context, grp, msgIndex);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_BuySellYuanBao:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_BuySellYuanBao(pPeer, context, msgIndex);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetTempBank:
		{
			UInt32Vector recordIDs;
			pBuf->Read<UInt32Vector>(recordIDs);
			UInt32 totalYBT;
			pBuf->Read<UInt32>(totalYBT);
			UInt32 totalMoney;
			pBuf->Read<UInt32>(totalMoney);
			_result = CB_GetTempBank(pPeer, context, recordIDs, totalYBT, totalMoney);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetSibling:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_GetSibling(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreateSibling:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_CreateSibling(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateSibling:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_UpdateSibling(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_DeleteSibling:
		{
			Boolean result;
			pBuf->Read<Boolean>(result);
			_result = CB_DeleteSibling(pPeer, context, result);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetGeneration:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_GetGeneration(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreateGeneration:
		{
			NetObject* pGeneration;
			pBuf->Read<NetObject*>(pGeneration);
			_result = CB_CreateGeneration(pPeer, context, pGeneration);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_DeleteGeneration:
		{
			UInt32 uiYoungId;
			pBuf->Read<UInt32>(uiYoungId);
			UInt32 uiElderId;
			pBuf->Read<UInt32>(uiElderId);
			BOOL bSuccess;
			pBuf->Read<BOOL>(bSuccess);
			_result = CB_DeleteGeneration(pPeer, context, uiYoungId, uiElderId, bSuccess);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateGeneration:
		{
			_result = CB_UpdateGeneration(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetMarriage:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_GetMarriage(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_CreateMarriage:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_CreateMarriage(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateMarriage:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_UpdateMarriage(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_DeleteMarriage:
		{
			Boolean result;
			pBuf->Read<Boolean>(result);
			_result = CB_DeleteMarriage(pPeer, context, result);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetMarriageCount:
		{
			UInt32 count;
			pBuf->Read<UInt32>(count);
			_result = CB_GetMarriageCount(pPeer, context, count);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetSingleMarriageLst:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_GetSingleMarriageLst(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_GetNpcFlagsByLineId:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_GetNpcFlagsByLineId(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ObjectService_CB_UpdateNpcFlags:
		{
			_result = CB_UpdateNpcFlags(pPeer, context);
		}
		break;
#endif // CLIENT
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}

RPCResult ObjectService_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	}
	return _result;
}
#pragma optimize("", on)

