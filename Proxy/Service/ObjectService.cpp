//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/common_binary.h"


//-- Self
#include "Proxy/Service/ObjectService_proxy.h"
#ifdef MAP_SERVICE
#include "MapService/MapApplication.h"
//-- Library
#include "Stub/Service/MapService_Stub.h"
#include "MapService/Global.h"
#include "Stub/Logic/RelationSystem.h"
#include "Proxy/Data/MarriageData.h"
#include "Proxy/Data/QuestNpcFlagData.h"
#endif
#ifdef WORLD_SERVICE
#include "Proxy/Data/QuestNpcFlagData.h"
#include "WorldService/Global.h"
#endif

RPCResult ObjectService_Proxy::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{ 
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ObjectService CB_Error: errCode: %d, %s"), errCode, errMsg.c_str());
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetAccountData(LPCPEER pPeer, RPCContext &context, NetObject *acctData)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetAccountData:"));
	DeleteNetObject(acctData);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetGuildRank(LPCPEER pPeer, RPCContext &context, NetGroup* guildRankGrp)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetGuildRank:"));
	DeleteNetGroup(guildRankGrp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetCharacterGroup(LPCPEER pPeer, RPCContext &context, NetGroup *charGroup, const UInt8 &detail)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetCharacterGroup:"));
	DeleteNetGroup(charGroup, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateCharacter(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdateCharacter:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_HasCharacter(LPCPEER pPeer, RPCContext &context, const BOOL &hasChar)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_HasCharacter:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_CreateItemDone(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_CreateItemDone:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetItemData(LPCPEER pPeer, RPCContext &context, NetObject * itemData, const UInt8 &detail)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetItemData:"));
	DeleteNetObject(itemData);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetItemGroup(LPCPEER pPeer, RPCContext &context, NetGroup * itemGroup, const UInt8 &detail)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetItemGroup:"));
	DeleteNetGroup(itemGroup, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateItemList(LPCPEER pPeer, RPCContext &context, NetGroup *newItems)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdateItemList:"));
	DeleteNetGroup(newItems, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_DeleteGuildList(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_CreateCharacterSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_CreateCharacterSuccess:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_CreatePartnerSuccess(LPCPEER pPeer, RPCContext &context, NetObject* partner)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_CreateCharacterSuccess:"));
	DeleteNetObject(partner);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetPartnerGroup(LPCPEER pPeer, RPCContext &context, NetGroup *partnerGroup, const UInt8 &detail)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetPartnerGroup:"));
	DeleteNetGroup(partnerGroup, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}


RPCResult ObjectService_Proxy::CB_UpdatePartnerList(LPCPEER pPeer, RPCContext &context,
													NetGroup *partnerGroup)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdatePartnerList:"));
	DeleteNetGroup(partnerGroup, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_DeleteCharacterSuccess(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_DeleteCharacter:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_RestoreCharacterSuccess(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_RestoreCharacterSuccess:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_SetCharExpiryDateSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &deletetime)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_SetCharExpiryDateSuccess:"));
	return RPC_RESULT_OK;
}


RPCResult ObjectService_Proxy::CB_GetSkillList(LPCPEER pPeer, RPCContext &context, NetGroup* skillDataGroup)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetSkillList:"));
	DeleteNetGroup(skillDataGroup, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_CreateSkillDone(LPCPEER pPeer, RPCContext &context, NetObject * skillData)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_CreateSkillDone:"));
	DeleteNetObject(skillData);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_DeleteSKillDone(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_DeleteSKillDone:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateSkillList(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdateSkillList:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetQuestList(LPCPEER pPeer, RPCContext &context, 
											   NetGroup* questLogDataGroup, 
											   NetGroup* questTypeDataGroup, 
											   NetGroup* questFlagDataGroup)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetQuestList:"));

	DeleteNetGroup(questLogDataGroup, BOOLEAN_TRUE);
	DeleteNetGroup(questTypeDataGroup, BOOLEAN_TRUE);
	DeleteNetGroup(questFlagDataGroup, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateQuestLogs(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdateQuestLogs:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateQuestTypes(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdateQuestTypes:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateQuestFlags(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdateQuestFlags:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_DeleteMessenger(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_DeleteMessenger:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateMessenger(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdateMessenger:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_CreateMessenger(LPCPEER pPeer, RPCContext &context, NetObject* relationData)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_CreateMessenger:"));
	DeleteNetObject(relationData);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetMessengerLst(LPCPEER pPeer, RPCContext &context, NetGroup* messengerDataGroup)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetMessengerLst:"));
	DeleteNetGroup(messengerDataGroup, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetChargeShop(LPCPEER pPeer, RPCContext &context, NetGroup* chargeShop)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetChargeShop:"));
	DeleteNetGroup(chargeShop, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateChargeShopSuccess(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdateChargeShopSuccess:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateAccountSuccess(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdateAccountSuccess:"));
	return RPC_RESULT_OK;
}


RPCResult ObjectService_Proxy::CB_GetCharacterRank(LPCPEER pPeer, RPCContext &context, const UInt8 &rankType, const UInt8 &subType, NetGroup* charRankGrp)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _T("CB_GetCharacterRank:"));
	DeleteNetGroup(charRankGrp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

/*RPCResult ObjectService_Proxy::CB_UpdatePet(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdatePet:"));
	return RPC_RESULT_OK;
}*/

RPCResult ObjectService_Proxy::CB_UpdatePetList(LPCPEER pPeer, RPCContext &context, NetGroup* newGroup)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdatePetList:"));
	DeleteNetGroup(newGroup, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_CreatePet(LPCPEER pPeer, RPCContext &context, NetObject* pPetData)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_CreatePet:"));
	DeleteNetObject(pPetData);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_PetList(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_PetList:"));
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_QueueNetGroup(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_QueueQuests(LPCPEER pPeer, RPCContext &context, NetGroup* questLogGroup, NetGroup* questTypeGroup, NetGroup* questFlagGroup)
{
	DeleteNetGroup(questLogGroup, BOOLEAN_TRUE);
	DeleteNetGroup(questTypeGroup, BOOLEAN_TRUE);
	DeleteNetGroup(questFlagGroup, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_DeleteMessengerList(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_DeleteMessengerList:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_TitleList(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_TitleList:"));
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_CreateTitle(LPCPEER pPeer, RPCContext &context, NetObject* pTitleData)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_CreateTitle:"));
	DeleteNetObject(pTitleData);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateTitle(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdateTitle:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetMsgCharacterGroup(LPCPEER pPeer, RPCContext &context, NetGroup* charGroup)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetMsgCharacterGroup:"));
	DeleteNetGroup(charGroup, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetGuilds(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetGuilds:"));
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateGuildList(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdateGuildList:"));
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}


RPCResult ObjectService_Proxy::CB_DeleteTitle(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_DeleteTitle:"));
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_CreateGuild(LPCPEER pPeer, RPCContext &context, NetObject* pGuild, NetObject* pMember)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_CreateGuild:"));
	DeleteNetObject(pGuild);
	DeleteNetObject(pMember);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateGuildMemberList(LPCPEER pPeer, RPCContext &context, NetGroup* newGroup)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_UpdateGuildMemberList:"));
	DeleteNetGroup(newGroup, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_CreateGuildMember(LPCPEER pPeer, RPCContext &context, NetObject* newData)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_CreateGuildMember:"));
	DeleteNetObject(newData);
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_GetGuildMembers(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetGuildMembers:"));
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_DeleteGuildMemberList(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_DeleteGuildMemberList:"));
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateMsgCharacter(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetGiftBoxByAcctID(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_DeleteGiftBox(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetYuanBaoByAcctID(LPCPEER pPeer, RPCContext &context, const UInt32 &yuanBao, const UInt32 &yuanBaoTicket, const Boolean &yuanBaoLock, 
													  const UInt32 &unlocktime)
{
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &yuanBao, const Boolean &success)
{
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateYuanBaoTicket(LPCPEER pPeer, RPCContext &context, const UInt32 &yuanBao, const UInt32 &yuanBaoTicket, const UInt8 &msgIndex)
{
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetHawkerHistory(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateHawkerHistory(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_IsAssetProtected(LPCPEER pPeer, RPCContext &context, const Boolean &bProtext, const Boolean &bNeedResetPass)
{
	
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_StoreAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &bSucess)
{
	
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_SetAssetLock(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{
    
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &pass)
{

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_ResetAssetPassDate(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* grp, const UInt8 &msgIndex)
{
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}


RPCResult ObjectService_Proxy::CB_GetPartnerCollection(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_CreatePartnerCollection(LPCPEER pPeer, RPCContext &context, NetObject *newData)
{
	DeleteNetObject(newData);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_DeletePartnerCollection(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_GetSibling(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	DeleteNetGroup(grp);
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_CreateSibling(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	DeleteNetGroup(grp);
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_UpdateSibling(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	DeleteNetGroup(grp);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_DeleteSibling(LPCPEER pPeer, RPCContext &context, const Boolean &result)
{
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt32Vector &recordIDs, const UInt32 &totalYBT, const UInt32 &totalMoney)
{
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetGeneration(LPCPEER pPeer, 
												RPCContext &context,
												NetGroup* grp)
{
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_CreateGeneration(LPCPEER pPeer,
												   RPCContext &context, 
												   NetObject* pGeneration)
{
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_DeleteGeneration(LPCPEER pPeer,
												   RPCContext &context, 
												   const UInt32 &uiYoungId, 
												   const UInt32 &uiElderId, 
												   const BOOL &bSuccess)
{
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_UpdateGeneration(LPCPEER pPeer,
												   RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateCharGenAcc(LPCPEER pPeer,
												   RPCContext &context,
												   const int &iRet )
{
	return RPC_RESULT_OK;
}


RPCResult ObjectService_Proxy::CB_GetMarriage(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_CreateMarriage(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_UpdateMarriage(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_DeleteMarriage(LPCPEER pPeer, RPCContext &context, const Boolean &result)
{
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_GetMarriageCount(LPCPEER pPeer, RPCContext &context, const UInt32 &count)
{
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Proxy::CB_GetNpcFlagsByLineId(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
#ifdef MAP_SERVICE
	if (grp == NULL)
		return RPC_RESULT_OK;
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("count = %d"),grp->GetCount());
	for (UInt32 i = 0; i < grp->GetCount(); ++i)
	{
		CQuestNpcFlagData* pData = grp->GetChild<CQuestNpcFlagData>(i);
		if (pData)
		{	
			NpcFlagIndex flagIndex(pData->GetNpc_id(),pData->GetLine_id(),pData->GetOwner_id(),pData->GetFlag_id());
			gGlobal.g_AllNpcFlags[flagIndex] = pData->GetFlag();
			TRACE_INFODTL_5(GLOBAL_LOGGER,_F("npc_id = %d,line_id = %d,owner_id = %d,flag_id = %d,npc_id = %d"),pData->GetNpc_id(),pData->GetLine_id(),pData->GetOwner_id(),pData->GetFlag_id(),pData->GetFlag());
		}
	}
#endif

#ifdef WORLD_SERVICE
	if (grp == NULL)
		return RPC_RESULT_OK;
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("count = %d"),grp->GetCount());
	for (UInt32 i = 0; i < grp->GetCount(); ++i)
	{
		CQuestNpcFlagData* pData = grp->GetChild<CQuestNpcFlagData>(i);
		if (pData)
		{	
			NpcFlagIndex flagIndex(pData->GetNpc_id(),pData->GetLine_id(),pData->GetOwner_id(),pData->GetFlag_id());
			gGlobal.m_AllNpcFlags[flagIndex] = pData->GetFlag();
			TRACE_INFODTL_5(GLOBAL_LOGGER,_F("npc_id = %d,line_id = %d,owner_id = %d,flag_id = %d,npc_id = %d"),pData->GetNpc_id(),pData->GetLine_id(),pData->GetOwner_id(),pData->GetFlag_id(),pData->GetFlag());
		}
	}
	
#endif
	DeleteNetGroup(grp,BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_UpdateNpcFlags(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Proxy::CB_GetSingleMarriageLst(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	#ifdef MAP_SERVICE
	TRACE_INFODTL(GLOBAL_LOGGER, _F("GetMarriage and divide it to different list:"));
	if (grp == NULL)
		return RPC_RESULT_OK;
	for (UInt32 i = 0; i < grp->GetCount(); ++i)
	{
		CMarriageData* pMarriageData = grp->GetChild<CMarriageData>(i);
		if (pMarriageData)
		{	
			gGlobal.g_MarriageTimeLst.push_back(pMarriageData->GetMarriage_time());
			TRACE_INFODTL_1(GLOBAL_LOGGER,_F("gGlobal.g_MarriageTimeLst push time: %d"),pMarriageData->GetMarriage_time());
		}
	}
	#endif
	DeleteNetGroup(grp,BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}