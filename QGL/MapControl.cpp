//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MapControl.h"
//-- Library
#include "QGLApplication.h"
#include "CharacterControl.h"
#include "MapNPCControl.h"
#include "Proxy/Logic/Character.h"
#include "Proxy/Logic/MapNPC.h"
#include "Proxy/Logic/QuestSystem.h"
#include "Global.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResItem.h"
#include "Configure.h"
#include "MenuPartyProcess.h"
#include "NewBieHintCtrl.h"
#include "Resource/ResCharacter.h"

CMapControl::CMapControl()
{
	m_pMap = NULL;
}

CMapControl::~CMapControl()
{

}

VOID CMapControl::SetMapObject( CMap * pGivMap)
{ 
	m_pMap = pGivMap;

	if (pGivMap == NULL)
		return;

	pGivMap->SetMapView(this);
}


VOID CMapControl::NotifyObjectAppeared(NetObject* obj)
{
	// return if already appeared
	if (obj == NULL || QGLApplication::GetInstance().FindMapActorCtrlByNetID(obj->GetNetId()))
		return;

	CCharacter * pChar = CastNetObject<CCharacter>(obj);
	if (pChar)
	{
		CCharacterControl * pCharCtrl = SafeCreateObject(CCharacterControl);
		pCharCtrl->LoadDefaultPart(pChar->GetSexClass());
		pCharCtrl->SetMapActorObject(pChar);
		
		pCharCtrl->InitShadow();
		pCharCtrl->SetFontColor(D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
			gConfigure.uNameFontColorG, gConfigure.uNameFontColorB));
		pCharCtrl->GetCharNameView()->SetFontColor(pCharCtrl->GetFontColor());
		QGLApplication::GetInstance().AddCharCtrlToMap(pCharCtrl);

		return;
	}

	CMapNPC * pNPC = CastNetObject<CMapNPC>(obj);

	if (pNPC)
	{
		CMapNPCControl * pMapNpcControl = SafeCreateObject(CMapNPCControl);
		pMapNpcControl->SetMapActorObject(pNPC);
		pMapNpcControl->InitShadow();
		pMapNpcControl->SetInitRotation(pNPC->GetDirection());
		pMapNpcControl->GetCharMainView()->SetRotation(pNPC->GetDirection());
		pMapNpcControl->SetFontColor(TextColor[ColorIndex_Yellow]);
		pMapNpcControl->GetCharNameView()->SetFontColor(pMapNpcControl->GetFontColor());
		pMapNpcControl->SetEnable(pNPC->GetVisible());
		NPCDB_Client* npctemp = gGlobal.LoadNpcTemplate(pNPC->GetNpc_id());
		if( npctemp )
		{
			pMapNpcControl->LoadDefaultMob(npctemp->m_basicinfo->m_npc_model, npctemp->m_basicinfo->m_npc_color);
			pMapNpcControl->SetNPCData(npctemp);
			if (npctemp->m_basicinfo->m_npc_type == NPC_INVISIBLE_EMENY)
			{
				pMapNpcControl->GetCharMainView()->SetVisible(FALSE);
				pMapNpcControl->GetCharNameView()->SetVisible(FALSE);
				pMapNpcControl->GetCharShadowView()->SetVisible(FALSE);
			}
			else if (npctemp->m_basicinfo->m_npc_type == NPC_VISIBLE_EMENY_FOR_MANY_USER)
			{
				pMapNpcControl->GetHeadIconView()->LoadEntityFile(gConfigure.HeadIconName[MISSION_ICON_EXCLAMATION_GOLD].c_str(), gGlobal.pD3DDevice,
					EQUIP_OTHERS);
			}

				
			pMapNpcControl->SetFrontName(npctemp->m_basicinfo->m_npc_pre_name, EDGE_SHADOW);

			pMapNpcControl->SetFrontFontColor(TextColor[ColorIndex_Yellow]);
			pMapNpcControl->GetCharFrontNameView()->SetFontColor(TextColor[ColorIndex_Yellow]);
		}

		// show npc quest hint icon
		if (gGlobal.g_QuestSystem)
			gGlobal.g_QuestSystem->ShowNpcHint(pMapNpcControl);
		QGLApplication::GetInstance().GetRenderMap()->InsertNPCToMap(pMapNpcControl);

		if (gGlobal.g_pNewBieHintCtrl )
		{
			if (gGlobal.g_pNewBieHintCtrl->IsNewBieByChar_id(gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id()))
			{
				const StartCharData* pCharData = gGlobal.m_resource->GetStartChar(gGlobal.g_pCharacter->GetCharNetObject()->GetSexClass());
				if (pCharData && pNPC->GetNpc_id() == pCharData->Start_npc_templateID)
				{
					pMapNpcControl->OnClickOnCtrl();
					
					gGlobal.g_pNewBieHintCtrl->SetBie(gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id(), BOOLEAN_FALSE);
					gGlobal.g_pNewBieHintCtrl->WriteSetting(gGlobal.g_pCharacter->GetCharNetObject()->GetAcct_id());
				}
			}
		}		
	}
}
VOID CMapControl::NotifyObjectDisappeared(NetObject* obj)
{
	CCharacter * pChar = CastNetObject<CCharacter>(obj);
	if (pChar)
	{
		if (gGlobal.g_pPartyProcess->IsInRequestList(pChar->GetNetId()))
		{
			gGlobal.g_pPartyProcess->JoinRejectByChar(pChar->GetNetId());
		}
		else if (gGlobal.g_pPartyProcess->IsInInviteList(pChar->GetChar_id()))
		{
			gGlobal.g_pPartyProcess->InviteRejectByCharID(pChar->GetChar_id());
		}


		QGLApplication::GetInstance().DeleteCharacterByNetObject(pChar);
		DeleteNetObject(pChar);
		return;
	}

	CMapNPC * pNPC = CastNetObject<CMapNPC>(obj);

	if (pNPC)
	{
		pNPC->CancelTalkingTarget();

		QGLApplication::GetInstance().GetRenderMap()->RemoveNPCToFromMapByNetObj(pNPC);
		DeleteNetObject(pNPC);
		return;
	}

	if (obj)
		DeleteNetObject(obj);
	
}


