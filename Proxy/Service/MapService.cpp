//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/MapService.h"
#include "Proxy/Logic/Party.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/SkillGroup.h"
#include "Proxy/Logic/QuestSystem.h"
#include "Proxy/Logic/ScriptSystem.h"
#include "Proxy/Logic/Pet.h"
#include "Proxy/Logic/PetGroup.h"
#include "Proxy/Logic/ChargeShop.h"

#ifdef QGL
#include "QGL/Global.h"
#include "QGL/QGLApplication.h"
#include "QGL/CharacterControl.h"
#include "QGL/ScriptControl.h"
#include "QGL/CallLuaFunc.h"
#include "QGL/UIEntity.h"
#include "QGL/MenuPartnerProcess.h"
#include "QGL/MenuSkillProcess.h"
#include "QGL/MenuItemProcess.h"
#include "QGL/MenuHelpProcess.h"
#include "QGL/MenuPetProcess.h"
#include "QGL/MenuOnlineShopProcess.h"
#include "Resource/ResItem.h"
#include "Resource/ResMob.h"
#include "Resource/ResPet.h"
#include "Resource/ClientResManager.h"
#endif // QGL

#ifdef MESSAGE
#include "MessageService/Global.h"
#include "Stub/Service/MessageService.h"
#endif // QGL

#define MAX_PARTY_NUM 5

// private function and namespace global variable here
namespace MapService_Proxy
{
};

RPCResult MapService_Proxy::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_SyncProxy(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_ResumeSession(LPCPEER pPeer, RPCContext &context, const Boolean &valid)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_EnterSuccess(LPCPEER pPeer, RPCContext &context, NetObject * map, 
											NetObject* character)
{
#ifdef QGL
	if (g_pGlobalView)
		g_pGlobalView->NotifyEnterSuccess(pPeer, context, map, character);

#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_CharacterStuff(LPCPEER pPeer, RPCContext &context, 
											  NetObject* bag, NetObject* scriptSys, 
											  NetObject* questSys, NetGroup* skillGroup, 
											  NetGroup* partnerGroup, NetGroup* petGroup,
											  const UInt8 &addictHealth)
{
#ifdef QGL
	CItemBag *pBag = CastNetObject<CItemBag>(bag);
	if (pBag)
	{
		if (gGlobal.m_pBag && gGlobal.m_pBag != pBag) {
			DeleteNetObject(gGlobal.m_pBag);
		}
		gGlobal.m_pBag = pBag;
	}
	else
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("MapService:CB_CharacterStuff: null bag"));
	}

	gGlobal.g_SkillLst = CastNetObject<CSkillGroup>(skillGroup);
	gGlobal.g_SkillLst->SetOwnerType(SKILL_OWNER_TYPE_CHAR);
	gGlobal.g_pPartnerGroup = CastNetObject<CPartnerGroup>(partnerGroup);
	gGlobal.g_PetLst = CastNetObject<CPetGroup>(petGroup);

	if (gGlobal.g_pSkillProcess->GetIsShop())
		gGlobal.g_pSkillProcess->ConstructByShopID(gGlobal.g_pSkillProcess->GetShopID());
	else
		gGlobal.g_pSkillProcess->ConstructData();
	gGlobal.g_pSkillProcess->UpdateSkillMenu();

	gGlobal.g_QuestSystem = CastNetObject<CQuestSystem>(questSys);

	CScriptSystem* pNewScriptSys = CastNetObject<CScriptSystem>(scriptSys);
	if ( pNewScriptSys )
	{
		if ( gGlobal.g_pScriptControl == NULL )
			gGlobal.g_pScriptControl = SafeCreateObject(CScriptControl);

		if ( gGlobal.g_pScriptControl->GetScriptSysNetObject() != pNewScriptSys )
		{
			CScriptSystem *pOldScriptSys = gGlobal.g_pScriptControl->GetScriptSysNetObject();
			DeleteNetObject(pOldScriptSys);

			gGlobal.g_pScriptControl->SetScriptSysNetObject(pNewScriptSys);
		}

		//gGlobal.g_pScriptControl->Init();
	}

	//if (gGlobal.g_pItemProcess)
	//	gGlobal.g_pItemProcess->NewUpdateItemMenu();

	for(Int i = 0; i < 10; ++i)
	{
		if(gGlobal.g_ShortCutSlot[i].m_RefType== 2)
		{
			gGlobal.g_pSkillProcess->WriteToShortCut(gGlobal.g_ShortCutSlot[i].m_RefNum, i);
		}
	}

	UIImage* gdHealthIcon = gGlobal.pUIManager->FindUIImage("PlayerHeALThgood");
	UIImage* avgeHealthIcon = gGlobal.pUIManager->FindUIImage("PlayerHeALThaverage");
	UIImage* poorHealthIcon = gGlobal.pUIManager->FindUIImage("PlayerHeALThpoor");
	if ((gdHealthIcon) && (avgeHealthIcon) && (poorHealthIcon))
	{
		gdHealthIcon->SetEnable(false);	
		avgeHealthIcon->SetEnable(false);	
		poorHealthIcon->SetEnable(false);		
		
		if (addictHealth == 0)
		{
			gdHealthIcon->SetEnable(true);
			
		}
		else if (addictHealth == 1)
		{
			avgeHealthIcon->SetEnable(true);
		}
		else if (addictHealth == 2)
		{
			poorHealthIcon->SetEnable(true);
		}
		gGlobal.pUIManager->AddDirtyUIItem(gdHealthIcon);	
		gGlobal.pUIManager->AddDirtyUIItem(avgeHealthIcon);	
		gGlobal.pUIManager->AddDirtyUIItem(poorHealthIcon);		

	}
	UIPanel *panel = gGlobal.pUIManager->FindUIPanel("OnlineShopPanel");
	if (panel)
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseOnlineShop", String());	

#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_LeaveSuccess(LPCPEER pPeer, RPCContext &context,
											const Boolean &bDelete)
{
	#ifdef QGL
	if (g_pGlobalView)
		g_pGlobalView->NotifyLeaveSuccess(bDelete);
	#endif // QGL
	return RPC_RESULT_OK;
}

/*
RPCResult MapService_Proxy::CB_PartnerGroup(LPCPEER pPeer, RPCContext &context, 
											NetGroup* partnerGroup, NetGroup* skillGroups)
{
	#ifdef QGL

	CPartnerGroup * pPartnerGroup = CastNetObject<CPartnerGroup>(partnerGroup);
	
	if (gGlobal.g_pPartnerGroup)
	{
		DeleteNetObject(gGlobal.g_pPartnerGroup);
	}

	if (pPartnerGroup)
		gGlobal.g_pPartnerGroup = pPartnerGroup;
	else 
		gGlobal.g_pPartnerGroup = SafeCreateObject(CPartnerGroup);
	
	if (gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerGroup)
	{
		CPartner* partner;
		CSkillGroup *grp;
		partner = gGlobal.g_pPartnerGroup->GetCurrBattlePartner();
		if (partner)
		{
			gGlobal.g_pPartnerProcess->UpdateSmallIcon(true, partner->GetPartner_id());
		}
		else
		{
			gGlobal.g_pPartnerProcess->UpdateSmallIcon(false);
		}

		for (UInt16 i = 0; i < skillGroups->GetCount(); i++) {
			grp = skillGroups->GetChild<CSkillGroup>(i);
			if (grp) {
				partner = gGlobal.g_pPartnerGroup->GetPartner(grp->GetOwner_id());
				partner->SetSkillGroup(grp);
				gGlobal.g_pPartnerGroup->SetSkillGroup(partner->GetPartner_id(), grp);
				if (gGlobal.g_pPartnerProcess->IsOnShowPartner(grp->GetOwner_id()))
					gGlobal.g_pPartnerProcess->UpdateSkillMenu(partner);
			}
		}
		gGlobal.g_pPartnerProcess->UpdatePartnerMenu();
		//gGlobal.g_pPartnerProcess->ShowPartnerButton();
	}

	#endif // QGL
	return RPC_RESULT_OK;
}
*/

RPCResult MapService_Proxy::CB_SyncCharacter(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_SyncPartner(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_AddPartnersSuccess(LPCPEER pPeer, RPCContext &context)
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

RPCResult MapService_Proxy::CB_CharacterCheat(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_EnterBattle(LPCPEER pPeer, RPCContext &context, 
										   NetObject* battle, NetGroup* grp, 
										   const UInt8 &PlayerTeam, const UInt8 &battleState,
										   const UInt32 &npcId)
{
#ifdef QGL
		gGlobal.NotifyEnterBattle(pPeer, context, battle, grp, PlayerTeam, battleState, npcId);
		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("GenerationRequestPanel");
		if (NULL != pPanel) {
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "FunCloseGenerationPanel", String());
		}
		
		pPanel = gGlobal.pUIManager->FindUIPanel("MessagerPanel");
		if (NULL != pPanel) {
			if (NULL != gGlobal.g_pCharacter){
				CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
				if (NULL != pChar) {
					UIButton* pBtn =NULL;
					if (pChar->GetLevel() < 50)
						pBtn = gGlobal.pUIManager->FindUIButton("OpenAsYounger");
					else
						pBtn = gGlobal.pUIManager->FindUIButton("OpenAdElder");
					if (NULL != pBtn){
						pBtn->SetState(_ButtonClick);
						pBtn->SetNotDetected(true);
					}
				}
			}
		}
#endif // QGL

	return RPC_RESULT_OK;
}

/*
RPCResult MapService_Proxy::CB_PetGroup(LPCPEER pPeer, RPCContext &context, NetGroup* petGroup, NetGroup* skillGroups)
{
	#ifdef QGL
	CPetGroup* pGrp = CastNetObject<CPetGroup>(petGroup);
	if (pGrp != NULL)
	{
		if (gGlobal.g_PetLst != NULL &&
			gGlobal.g_PetLst != petGroup)
		{
			DeleteNetGroup(gGlobal.g_PetLst);
		}
		gGlobal.g_PetLst = pGrp;

		for (UInt16 i = 0; i < skillGroups->GetCount(); i++) {
			CSkillGroup * grp = skillGroups->GetChild<CSkillGroup>(i);
			if (grp) {
				CPet * pPet = gGlobal.g_PetLst->GetPet(grp->GetOwner_id());
				if (pPet)
				{
					pPet->SetSkillGroup(grp);
					gGlobal.g_PetLst->SetSkillGroup(pPet->GetPet_uid(), grp);
				}
			}
		}
		gGlobal.g_pMenuPetProcess->ResetMenu();
		gGlobal.g_pMenuPetProcess->RefreshMenu();
	}
	#endif // QGL
	return RPC_RESULT_OK; 
}
*/

RPCResult MapService_Proxy::CB_ShowPublicItemDetail(LPCPEER pPeer, RPCContext &context, NetObject* item)
{

	#ifdef QGL

	CItem* temp =  CastNetObject<CItem>(item);

	UIPanelEx* pPanelEx = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");

	if (pPanelEx)
	{
		gGlobal.pUIManager->DeletePanel("ItemDetailMenuPanel");
	}
	
	XML_UI::GetInstance().Init();
	XML_UI::GetInstance().ReadFormFile("Data\\ItemDetail.xml", gGlobal.pUIManager);

	pPanelEx = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");
	if (pPanelEx == NULL)
		return RPC_RESULT_OK;		

	gGlobal.pUIManager->SetPanelEx2Top(pPanelEx);
	pPanelEx->SetEnable(true);

	gGlobal.g_pItemProcess->ItemDetailPopUp(temp->GetItem_id(), false, temp);

	Int Width = pPanelEx->GetWidth();
	Int Height = pPanelEx->GetHeight();
	Int ItemDetailX = gGlobal.pUIManager->GetCursorX();
	Int ItemDetailY = gGlobal.pUIManager->GetCursorY();

	if(ItemDetailY + Height > 599)
		ItemDetailY -= Height;
	if(ItemDetailX + Width > 799)
		ItemDetailX -= Width;
	if(ItemDetailY < 0)
		ItemDetailY = 0;
	if(ItemDetailX < 0)
		ItemDetailX = 0;

	pPanelEx->SetPos(ItemDetailX, ItemDetailY);


	DeleteNetObject(item);

	#endif
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, NetObject* item, const UInt32 &char_idTo)
{
#ifdef MESSAGE
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_idTo);
	if (pMsgInfo && pMsgInfo->pMsgChar && pMsgInfo->pProxyPeer)
	{
		CItem* temp =  CastNetObject<CItem>(item);
		if (temp)
		{
			MessageService_Stub::CB_ShowPublicItemDetailFromOtherLine(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), *temp);
		}
	}
	DeleteNetObject(item);
#endif
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, NetObject* Partner, const UInt16Vector &skillIDs, const UInt32 &char_idTo)
{
#ifdef MESSAGE
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_idTo);
	if (pMsgInfo && pMsgInfo->pMsgChar && pMsgInfo->pProxyPeer)
	{
		CPartner* temp =  CastNetObject<CPartner>(Partner);
		if (temp)
		{
			MessageService_Stub::CB_ShowPublicPartnerDetailFromOtherLine(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), *temp, skillIDs);
		}
	}
	DeleteNetObject(Partner);
#endif
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, NetObject* Pet, const UInt16Vector &skillIDs, const UInt32 &char_idTo)
{
#ifdef MESSAGE
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_idTo);
	if (pMsgInfo && pMsgInfo->pMsgChar && pMsgInfo->pProxyPeer)
	{
		CPet* temp =  CastNetObject<CPet>(Pet);
		if (temp)
		{
			MessageService_Stub::CB_ShowPublicPetDetailFromOtherLine(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), *temp, skillIDs);
		}
	}
	DeleteNetObject(Pet);
#endif
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_ShowPublicPartnerDetail(LPCPEER pPeer, RPCContext &context, NetObject* Partner, const UInt16Vector &skillID)
{

	#ifdef QGL

	CPartner* pPartner = CastNetObject<CPartner>(Partner);

	if (pPartner)
	{
		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartnerDetailPanel");
		if (pPanel == NULL)
		{
			XML_UI::GetInstance().Init();
			XML_UI::GetInstance().ReadFormFile("Data\\PartnerDetail.xml", gGlobal.pUIManager);

			pPanel = gGlobal.pUIManager->FindUIPanel("PartnerDetailPanel");
			if (pPanel == NULL)
				return RPC_RESULT_OK;		
		}

		gGlobal.g_pPartnerProcess->ShowPartnerDetail(pPartner, skillID);

		gGlobal.pUIManager->SetPanel2Top(pPanel);
		pPanel->SetNotDetected(false);

		Int32 posX = 0;
		Int32 posY = gGlobal.pUIManager->GetCursorY() - pPanel->GetHeight();

		if ( ( gGlobal.pUIManager->GetCursorX() + pPanel->GetWidth() ) > 800 )
			posX = 800 - pPanel->GetWidth();
		else
			posX = gGlobal.pUIManager->GetCursorX();

		if ( posY + pPanel->GetHeight() > 600 )
			posY = 600 - pPanel->GetHeight();
		else
		if ( posY < 0 )
			posY = 0;


		pPanel->SetPos(posX, posY);
		pPanel->SetEnable(true);
	}

	DeleteNetObject(Partner);

	#endif
	return RPC_RESULT_OK;

}



RPCResult MapService_Proxy::CB_ShowPublicPetDetail(LPCPEER pPeer, RPCContext &context, NetObject* Pet, const UInt16Vector &skillID)
{

	#ifdef QGL


	CPet* pPet = CastNetObject<CPet>(Pet);

	if (pPet)
	{
		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PetDetailPanel");
		if (pPanel == NULL)
		{
			XML_UI::GetInstance().Init();
			XML_UI::GetInstance().ReadFormFile("Data\\PetDetail.xml", gGlobal.pUIManager);

			pPanel = gGlobal.pUIManager->FindUIPanel("PetDetailPanel");
			if (pPanel == NULL)
				return RPC_RESULT_OK;		
		}
		gGlobal.g_pMenuPetProcess->ShowPetDetail(pPet, skillID);
		gGlobal.pUIManager->SetPanel2Top(pPanel);

		Int32 posX = 0;
		Int32 posY = gGlobal.pUIManager->GetCursorY() - pPanel->GetHeight();

		if ( ( gGlobal.pUIManager->GetCursorX() + pPanel->GetWidth() ) > 800 )
			posX = 800 - pPanel->GetWidth();
		else
			posX = gGlobal.pUIManager->GetCursorX();

		if ( posY + pPanel->GetHeight() > 600 )
			posY = 600 - pPanel->GetHeight();
		else
		if ( posY < 0 )
			posY = 0;


		pPanel->SetPos(posX, posY);
		pPanel->SetEnable(true);
	}

	DeleteNetObject(Pet);

	#endif
	return RPC_RESULT_OK;

}

RPCResult MapService_Proxy::CB_ReceiveMapMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName, const SessionIDVector &listeners)
{
#ifdef QGL
	gGlobal.PrintMessage(Msg, CHANNELTYPE_MAP, SrcName);
#endif
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_ReceiveMapEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const String &SrcName, const String &TargetName, const bool &isPet)
{
#ifdef QGL
	gGlobal.NotifyCB_ReceiveEmotion(emo_id, SrcName, TargetName, isPet);
#endif
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_OpenChargeShop(LPCPEER pPeer, RPCContext &context, NetGroup* chargeShop, NetGroup* discountGroup, NetObject* limitedItem)
{
#ifdef QGL
	if (gGlobal.g_pOnlineShopProcess)
	{
		CChargeShop* shop = CastNetObject<CChargeShop>(chargeShop);
		for (UInt8 i = 0; i < discountGroup->GetCount(); ++i)
		{
				shop->Add(discountGroup->GetChild(i));
		}
		if (limitedItem)
			shop->Add(limitedItem);
		gGlobal.g_pOnlineShopProcess->SetChargeShop(shop);

	}
	else
	{
		DeleteNetGroup(chargeShop, BOOLEAN_TRUE);
		DeleteNetGroup(discountGroup, BOOLEAN_TRUE);
		DeleteNetObject(limitedItem);
	}
	
#endif
	return RPC_RESULT_OK; 
}


RPCResult MapService_Proxy::CB_ForceLogout(LPCPEER pPeer, RPCContext &context, const String &forceOutMsg)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));
#ifdef QGL
	gGlobal.g_ForceOutMsg = forceOutMsg;
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult MapService_Proxy::CB_QueryInstance(LPCPEER pPeer, RPCContext &context, 
											 const Boolean &bExist, const UInt16 &allowCount)
{
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