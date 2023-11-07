//-- Common
#include "Common.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Proxy/Service/MapService_proxy.h"
//-- Library
#include "SimClient/Global.h"
#include "SimClient/Map.h"
#include "Proxy/Logic/BatActor.h"
#include "SimClient/Character.h"
#include "SimClient/ItemBag.h"
#include "SimClient/Party.h"
#include "SimClient/PartnerGroup.h"
#include "SimClient/ScriptSystem.h"
#include "SimClient/SkillGroup.h"
#include "SimClient/QuestSystem.h"
#include "SimClient/Battle.h"
#include "SimClient/Pet.h"
#include "SimClient/PetGroup.h"
#include "SimClient/GuildMember.h"
#include "SimClient/Guild.h"
#include "SimClient/ChargeShop.h"
#include "SimClient/ChargeShopItem.h"

PROTO_LuaPush(CMap)
PROTO_LuaPush(CCharacter)

// private function and namespace global variable here
namespace MapService_Proxy
{
};

RPCResult MapService_Proxy::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("MapService_Proxy CB_Error: errCode: %d, %s\n"), err, errMsg.c_str());
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_SyncProxy(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_ResumeSession(LPCPEER pPeer, RPCContext &context, const Boolean &valid)
{
	TRACE_ERRORDTL(GLOBAL_LOGGER, _F("MapService_Proxy CB_ResumeSession:"));
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_EnterSuccess(LPCPEER pPeer, RPCContext &context, 
											NetObject* map, NetObject* character)
{
	TRACE_ENSURE(gGlobal.m_pSimClient);

	CCharacter *pchar = CastNetObject<CCharacter>(character);
	CMap *pmap = CastNetObject<CMap>(map);
	if (pchar == NULL || pmap == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CB_EnterSuccess: invalid character or map"));
		DeleteNetObject(map);
		DeleteNetObject(character);
		return (RPC_RESULT_FAIL);
	}

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CB_EnterSuccess: char_id %d, map_id %d, curPosX %d, curPosY %d"), 
		pchar->GetChar_id(), pmap->GetMap_id(), 
		pchar->GetCurPosX(), pchar->GetCurPosY());

	// delete net object bug here, wait to fix
	if (gGlobal.m_pSimClient->m_pCharacter && gGlobal.m_pSimClient->m_pCharacter != pchar)
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("CB_EnterSuccess Warning: deleted old char_id %d,%d  new char_id %d,%d:"),
			gGlobal.m_pSimClient->m_pCharacter->GetChar_id(), 
			gGlobal.m_pSimClient->m_pCharacter->GetID(), 
			pchar->GetChar_id(), pchar->GetID());
		//DeleteNetObject(gGlobal.m_pSimClient->m_pCharacter);
	}

	bool bLoad = (gGlobal.m_pSimClient->m_pMap == NULL || gGlobal.m_pSimClient->m_pMap != pmap);
	if (gGlobal.m_pSimClient->m_pMap && gGlobal.m_pSimClient->m_pMap != pmap)
	{
		DeleteNetObject(gGlobal.m_pSimClient->m_pMap);
	}

	// assign character and map
	pchar->SetMap(pmap);
	gGlobal.m_pSimClient->m_pCharacter = pchar;
	//gGlobal.m_pSimClient->m_pCharacter->m_pParty = NULL;
	gGlobal.m_pSimClient->m_pMap = pmap;

	// get map jump points information
	if (bLoad) {
		gGlobal.m_pSimClient->m_pMap->LoadData();			// load binary map data
		gGlobal.m_pSimClient->m_pMap->GetJumpPoints(pPeer, context);
	}
	gGlobal.m_pSimClient->m_lastMoveX = 0;
	gGlobal.m_pSimClient->m_lastMoveY = 0;
	gGlobal.m_pSimClient->m_resTestTime = 100000;		// reset for auto ping
	gGlobal.m_pSimClient->m_batDeadlock = 0;			// reset battle deadlock time

	//Char buf[256];
	//_stprintf(buf, _T("Map Enter %d"), pchar->GetMap_id());
	//gGlobal.Respond(buf);
	if (gGlobal.m_pSimClient->m_bRobot)
		CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "EnterMap", String(), gGlobal.m_pSimClient->m_pMap, 
					gGlobal.m_pSimClient->m_pCharacter);
	// for robot quest command
	if ((gGlobal.m_pSimClient == NULL)||(gGlobal.m_pSimClient->m_pQuestSys == NULL))
		return RPC_RESULT_OK;
	if ((gGlobal.m_pSimClient->m_bRobot)&&(gGlobal.m_pSimClient->m_pQuestSys->IsOutstandingQuestTask()))
	{
		gGlobal.m_pSimClient->QuestJumped = TRUE;	
		
	}

	

	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_CharacterStuff(LPCPEER pPeer, RPCContext &context, 
											  NetObject* bag, NetObject* scriptSys, 
											  NetObject* questSys, NetGroup* skillGroup, 
											  NetGroup* partnerGroup, NetGroup* petGroup,
											  const UInt8 &addictHealth)
{
	TRACE_ENSURE(gGlobal.m_pSimClient);

	if (gGlobal.m_pSimClient->m_pCharacter == NULL) {
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CB_CharacterStuff Error: null character"));
		DeleteNetObject(bag);
		DeleteNetObject(scriptSys);
		DeleteNetObject(questSys);
		DeleteNetGroup(partnerGroup);
		DeleteNetGroup(petGroup);
		DeleteNetGroup(skillGroup);
		return (RPC_RESULT_FAIL);
	}

	CItemBag *pBag = CastNetObject<CItemBag>(bag);
	CPartnerGroup *pPartnerGroup = CastNetObject<CPartnerGroup>(partnerGroup);
	CPetGroup *pPetGroup = CastNetObject<CPetGroup>(petGroup);
	CSkillGroup *pSkillGroup = CastNetObject<CSkillGroup>(skillGroup);
	CScriptSystem *pScript = CastNetObject<CScriptSystem>(scriptSys);
	CQuestSystem *pQuestSys = CastNetObject<CQuestSystem>(questSys);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_CharacterStuff: bag %d, skillGroup %d"),
		pBag->GetID(), pSkillGroup->GetID());

	if (gGlobal.m_pSimClient->m_pBag && gGlobal.m_pSimClient->m_pBag != pBag)
		DeleteNetObject(gGlobal.m_pSimClient->m_pBag);

	if (gGlobal.m_pSimClient->m_pPartnerGroup && 
		gGlobal.m_pSimClient->m_pPartnerGroup != pPartnerGroup)
		DeleteNetObject(gGlobal.m_pSimClient->m_pPartnerGroup);

	if (gGlobal.m_pSimClient->m_pPetGroup && 
		gGlobal.m_pSimClient->m_pPetGroup != pPetGroup)
		DeleteNetObject(gGlobal.m_pSimClient->m_pPetGroup);

	if (gGlobal.m_pSimClient->m_pSkillGroup && gGlobal.m_pSimClient->m_pSkillGroup != pSkillGroup)
		DeleteNetObject(gGlobal.m_pSimClient->m_pSkillGroup);

	if (gGlobal.m_pSimClient->m_pScriptSys && gGlobal.m_pSimClient->m_pScriptSys != pScript)
		DeleteNetObject(gGlobal.m_pSimClient->m_pScriptSys);

	if (gGlobal.m_pSimClient->m_pQuestSys && gGlobal.m_pSimClient->m_pQuestSys != pQuestSys)
		DeleteNetObject(gGlobal.m_pSimClient->m_pQuestSys);

	// assign character and map
	gGlobal.m_pSimClient->m_pBag = pBag;
	gGlobal.m_pSimClient->m_pPartnerGroup = pPartnerGroup;
	gGlobal.m_pSimClient->m_pPetGroup = pPetGroup;
	gGlobal.m_pSimClient->m_pSkillGroup = pSkillGroup;
	gGlobal.m_pSimClient->m_pScriptSys = pScript;
	gGlobal.m_pSimClient->m_pQuestSys = pQuestSys;

	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_LeaveSuccess(LPCPEER pPeer, RPCContext &context, 
											const Boolean &bDelete)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("MapService_Proxy::CB_LeaveSuccess:"));
	TRACE_ENSURE(gGlobal.m_pSimClient);

	if (gGlobal.m_pSimClient->m_pCharacter && gGlobal.m_pSimClient->m_pCharacter->m_pParty)
		DeleteNetObject(gGlobal.m_pSimClient->m_pCharacter->m_pParty);
	if (gGlobal.m_pSimClient->m_pMap)
		DeleteNetObject(gGlobal.m_pSimClient->m_pMap);
	if (bDelete) {
		DeleteNetObject(gGlobal.m_pSimClient->m_pCharacter);
		DeleteNetObject(gGlobal.m_pSimClient->m_pBag);
		DeleteNetGroup(gGlobal.m_pSimClient->m_pPartnerGroup, BOOLEAN_TRUE);
		DeleteNetGroup(gGlobal.m_pSimClient->m_pSkillGroup, BOOLEAN_TRUE);
		DeleteNetObject(gGlobal.m_pSimClient->m_pQuestSys);
		DeleteNetObject(gGlobal.m_pSimClient->m_pScriptSys);
	}

	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_SyncCharacter(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_SyncPartner(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_SyncItem(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_CharacterEnter(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_PartyEnter(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_NpcEnter(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

/*
RPCResult MapService_Proxy::CB_PartnerGroup(LPCPEER pPeer, RPCContext &context, 
											NetGroup* partnerGroup, NetGroup* skillGroups)
{
	TRACE_ENSURE(partnerGroup);

	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("MapService_Proxy::CB_PartnerGroup: partnerGroup %d, count %d"),
		partnerGroup->GetID(), partnerGroup->GetCount());
	TRACE_ENSURE(gGlobal.m_pSimClient);

	if (gGlobal.m_pSimClient->m_pPartnerGroup && 
		gGlobal.m_pSimClient->m_pPartnerGroup != partnerGroup)
		DeleteNetObject(gGlobal.m_pSimClient->m_pPartnerGroup);

	gGlobal.m_pSimClient->m_pPartnerGroup = CastNetObject<CPartnerGroup>(partnerGroup);

	TRACE_ENSURE(gGlobal.m_pSimClient->m_pPartnerGroup);
	gGlobal.m_pSimClient->m_pPartnerGroup->SetSkillGroups(skillGroups);
	DeleteNetGroup(skillGroups);

	return RPC_RESULT_OK;
}
*/

RPCResult MapService_Proxy::CB_AddPartnersSuccess(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_CharacterCheat(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_EnterBattle(LPCPEER pPeer, RPCContext &context, 
										   NetObject* battle, NetGroup* grp, 
										   const UInt8 &PlayerTeam, const UInt8 &battleState,
										   const UInt32 &npcId)
{



	// character enter battle
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CB_EnterBattle: battle %d,%d, actor count %d"), 
		battle->GetNetId().srvId, battle->GetNetId().objId, grp->GetCount());

	CBattle* pBattle = CastNetObject<CBattle>(battle);
	TRACE_ENSURE(pBattle && gGlobal.m_pSimClient);

	if (gGlobal.m_pSimClient->m_pBattle && gGlobal.m_pSimClient->m_pBattle != pBattle)
	{
		DeleteNetObject(gGlobal.m_pSimClient->m_pBattle);
	}
	pBattle->ClearContent();
	gGlobal.m_pSimClient->m_pBattle = pBattle;
	gGlobal.m_pSimClient->m_batDeadlock = 0;

	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		CBatActor *actor = grp->GetChild<CBatActor>(i);
		if (actor)
			pBattle->m_actors.push_back(actor);
	}
	DeleteNetGroup(grp, BOOLEAN_FALSE);
	pBattle->ShowAllDetail();

	return RPC_RESULT_OK;
}

/*
RPCResult MapService_Proxy::CB_PetGroup(LPCPEER pPeer, RPCContext &context, NetGroup* petGroup, NetGroup* skillGroups)
{
	CPetGroup* pGrp = CastNetObject<CPetGroup>(petGroup);
	if (pGrp != NULL)
	{
		if (gGlobal.m_pSimClient->m_pPetGroup != NULL &&
			gGlobal.m_pSimClient->m_pPetGroup != petGroup)
		{
			//if (gGlobal.m_pSimClient->m_pPetGroup->IsExistSkillGroup(skillGroups))
			//{
			//	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_PetGroup: Skill Group already exist"));
			//	TRACE_ENSURE(FALSE);
			//}
			//!!!!!! Temp use, this is a big problem behinded, because forget to clear all related net object after disconnected.
			gGlobal.m_pSimClient->m_pPetGroup->ClearSkillMap();
			DeleteNetGroup(gGlobal.m_pSimClient->m_pPetGroup);
		}
		gGlobal.m_pSimClient->m_pPetGroup = pGrp;
			
		for (UInt16 i = 0; i < skillGroups->GetCount(); i++) 
		{
			CSkillGroup * grp = skillGroups->GetChild<CSkillGroup>(i);
			if (grp) 
			{	
				CPet * pPet = gGlobal.m_pSimClient->m_pPetGroup->GetPet(grp->GetOwner_id());
				if (pPet)
				{
					pPet->SetSkillGroup(grp);
					gGlobal.m_pSimClient->m_pPetGroup->SetSkillGroup(pPet->GetPet_uid(), grp);
				}
			}
		}	
		DeleteNetGroup(skillGroups);
	}
	else
	{
		DeleteNetGroup(petGroup, BOOLEAN_TRUE);
		DeleteNetGroup(skillGroups, BOOLEAN_TRUE);
	}
    
	return RPC_RESULT_OK; 
}
*/

RPCResult MapService_Proxy::CB_ShowPublicItemDetail(LPCPEER pPeer, RPCContext &context, NetObject* item)
{
	DeleteNetObject(item);
	return RPC_RESULT_OK;
}


RPCResult MapService_Proxy::CB_ShowPublicPartnerDetail(LPCPEER pPeer, RPCContext &context, NetObject* Partner, const UInt16Vector &skillID)
{
	DeleteNetObject(Partner);
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_ShowPublicPetDetail(LPCPEER pPeer, RPCContext &context, NetObject* Pet, const UInt16Vector &skillID)
{
	DeleteNetObject(Pet);
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_ReceiveMapMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName, const SessionIDVector &listeners)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_ReceiveMapEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const String &SrcName, const String &TargetName, const bool &isPet)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_OpenChargeShop(LPCPEER pPeer, RPCContext &context, NetGroup* chargeShop, NetGroup* discountGroup, NetObject* limitedItem)
{
	if (gGlobal.m_pSimClient == NULL)
		return RPC_RESULT_OK;	

	if (gGlobal.m_pSimClient->m_bShopOpen)
	{
        CChargeShop* shop = CastNetObject<CChargeShop>(chargeShop);
		if (!shop)
			return RPC_RESULT_OK;		
		for (UInt8 i = 0; i < discountGroup->GetCount(); ++i)
		{
				shop->Add(discountGroup->GetChild(i));
		}
		if (limitedItem)
			shop->Add(limitedItem);
		
		if ((gGlobal.m_pSimClient->m_pshop) && (gGlobal.m_pSimClient->m_pshop != shop)) 
		{
			DeleteNetGroup(gGlobal.m_pSimClient->m_pshop, BOOLEAN_TRUE); 
		}
		gGlobal.m_pSimClient->m_pshop = shop;

		for (UInt16 i = 0; i < gGlobal.m_pSimClient->m_pshop->GetCount(); ++i)
		{
			CChargeShopItem* item = gGlobal.m_pSimClient->m_pshop->GetChild<CChargeShopItem>(i);
			if (item)
			{
				gGlobal.m_pSimClient->m_chShopItem.insert(std::make_pair(item->GetShopType(), item));	

			}
		}
		TRACE_INFODTL(GLOBAL_LOGGER, _F("The shop is opened. You can buy things now"));	
		DeleteNetGroup(discountGroup);
		
		if (gGlobal.m_pSimClient->m_bRobot)
            CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "BuyItem", String()); // robot action in Map
	}
	else
	{
		DeleteNetGroup(chargeShop, BOOLEAN_TRUE);
		DeleteNetGroup(discountGroup, BOOLEAN_TRUE);
		DeleteNetObject(limitedItem);
	}
	return RPC_RESULT_OK; 
}

RPCResult MapService_Proxy::CB_ForceLogout(LPCPEER pPeer, RPCContext &context, const String &forceOutMsg)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));

	if (gGlobal.m_pSimClient) {
		gGlobal.m_pSimClient->DisconnectServer();
	}
	return RPC_RESULT_OK;
}
RPCResult MapService_Proxy::CB_GetPosition(LPCPEER pPeer, RPCContext &context, const UInt32 &mapX, const UInt32 &mapY)
{
	return RPC_RESULT_OK;	
}
RPCResult MapService_Proxy::CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const Boolean &result)
{
	return RPC_RESULT_OK;	
}

RPCResult MapService_Proxy::CB_ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, NetObject* item, const UInt32 &char_idTo)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, NetObject* Partner, const UInt16Vector &skillIDs, const UInt32 &char_idTo)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, NetObject* Pet, const UInt16Vector &skillIDs, const UInt32 &char_idTo)
{
	return RPC_RESULT_OK;
}