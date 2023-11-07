//-- Common
#include "Common.h"
#include "QGL.h"
#include "Common/PathFinder.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "CharacterControl.h"
//-- Library
#include "Configure.h"
#include "Global.h"
#include "QGLApplication.h"
#include "AppMapState.h"
#include "Proxy/Logic/Party.h"
#include "Proxy/Logic/BatCharacter.h"
#include "Proxy/Logic/Character.h"
#include "Proxy/Logic/BatMob.h"
#include "Proxy/Logic/Battle.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/Skill.h"
#include "Proxy/Logic/SkillGroup.h"
#include "AnimatedEntity.h"
#include "BattleControl.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResMob.h"
#include "Resource/ResBuff.h"
#include "MenuSkillProcess.h"
#include "MiddleMap.h"
#include "BatScriptControl.h"
#include "Common/CallLuaFunc.h"
#include "MenuBuffIconProcess.h"
#include "Resource/ResPet.h"
#include "Resource/ResTitle.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/ItemBag.h"
#include "MenuItemProcess.h"
#include "QGL/PointsShopProcess.h"
#include "Resource/ResAutoPath.h"
#include "CrossMapPathFinder.h"
#include "Proxy/Logic/Pet.h"
#include "Math.h"


CCharacterControl::CCharacterControl()
{
	//state init
	m_mapState = NULL;
	m_pCharacter = NULL;
	m_pBatActor = NULL;
	m_IsListen = TRUE;
	m_pTarget = NULL;
	
	State::ChangeState(m_mapState, (State *) SafeCreateObject(CCharMapState));

	m_MapActorType = MAPACTORTYPE_PLAYER;
	m_bHasPartner = false;
	m_bIsSentCommand = false;
	m_bHasFollowPet = false;
	m_bHasFollowTwoPet = false;

	m_AnimIdlePartnerChangeCounter = ::timeGetTime();
	m_AnimIdlePartnerChangeTime = 5000 + rand() % 5000;

	m_AnimIdlePetChangeCounter = ::timeGetTime();
	m_AnimIdlePetChangeTime = 5000 + rand() % 5000;

	m_FollowerIdleCounter = ::timeGetTime();
	m_FollowerIdleTime = 5000 + rand() % 5000;

	for (Int i = 0; i < MAX_ACTION_CMD; i++)
	{
		m_ActionCmdAva[i] = true;
	}

	m_tempBuffData = NULL;
	m_pHawkBubble = NULL;
	m_charModelLst.clear();
	m_charModelColor.clear();
}

CCharacterControl::~CCharacterControl()
{
	QGLApplication::GetInstance().RemoveCharCtrlFromWorld(this);

	if (!gConfigure.uNetwork)
	{
		if (m_pCharacter)
			SafeDeleteObject(m_pCharacter);
		if (m_pBatActor)
			SafeDeleteObject(m_pBatActor);
	}

	if (m_pCharacter)
	{
		m_pCharacter->SetCharacterView(NULL);
		m_pCharacter->SetControl(NULL);
	}

	if (gGlobal.g_pCharacter->GetTarget() == this)
		gGlobal.g_pCharacter->SetTarget(NULL);

	m_pHawkBubble = NULL;
	ClearFollower();

	//if (m_mapState)
	//SafeDeleteObject(m_mapState);
	m_charModelLst.clear();
	m_charModelColor.clear();
	State::ChangeState(m_mapState, NULL);
}


VOID CCharacterControl::SetBatActor(CBatActor * pBatActor)
{
	m_pBatActor = pBatActor;
	
	if (pBatActor == NULL)
		return;

	pBatActor->SetCharacterView(this);

	CBatCharacter* pBatChar = CastNetObject<CBatCharacter>(m_pBatActor);
	if (pBatChar)
	{
		OnUpdateInfo(pBatChar->GetNickName(), (BYTE)gConfigure.uBattleFontSyle);
		OnUpdateEqHead(pBatChar->GetEq_headBand(), pBatChar->GetEq_headColor(), pBatChar->GetSexClass());
		OnUpdateEqCloth(pBatChar->GetEq_cloth(), pBatChar->GetEq_clothColor(), pBatChar->GetSexClass());
		OnUpdateEqWeapon(pBatChar->GetEq_weapon(), pBatChar->GetEq_weaponColor(), pBatChar->GetSexClass());
		OnUpdateHead(pBatChar->GetHairStyle1(), pBatChar->GetHairColor(), pBatChar->GetSexClass());
		OnUpdateTP(pBatChar->GetTP(),pBatChar->GetModel_mobid(),true);
		//OnUpdatePKInfo(pBatChar->GetPK_Value());
		if (pBatChar->GetPK_Value() > 0)
			m_OrgFontColor = RedColor;
		else
			m_OrgFontColor = D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
			gConfigure.uNameFontColorG, gConfigure.uNameFontColorB);
		m_CharNameView.SetFontColor(m_OrgFontColor);
		

		CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pBatChar->GetChar_id());
		if (pCtrl &&
			pCtrl->GetAction() == CCharacter::ST_NEEDHELP)
		{
			m_CharHeadIconView.InitEntPicInfo();
			m_CharHeadIconView.LoadEntityFile(gConfigure.HeadIconName[HELP_ICON].c_str(), gGlobal.pD3DDevice);
		}
		OnUpdateBuff(pBatChar->GetBuff());
		OnUpdateBuffCache(pBatChar->GetBuffCache());
		return;
	}

	CBatMob * pBatMob = CastNetObject<CBatMob>(m_pBatActor);
	if (pBatMob)
	{
		OnUpdateInfo(pBatMob->GetRaname(), (BYTE)gConfigure.uBattleFontSyle);
        m_OrgFontColor = D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
		gConfigure.uNameFontColorG, gConfigure.uNameFontColorB);
		m_CharNameView.SetFontColor(m_OrgFontColor);
		OnUpdateBuff(pBatMob->GetBuff());
		OnUpdateBuffCache(pBatMob->GetBuffCache());

		if (pBatMob->GetOwner_id() > 0)
		{
			MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(pBatMob->GetMob_id());
			if( mobTemplate )
			{
				String modelName = String(mobTemplate->m_basicinfo->model);
				String colorFileName = modelName.substr(0, modelName.length() - 5);
				char abc = pBatMob->GetColorIndex() + 97;
				colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
				m_CharMainView.ChangeColorP(modelName.c_str(), colorFileName.c_str());

			}
		}
		return;
	}
}

VOID CCharacterControl::OnUpdateTitle(String title, UInt16 title_id)
{
	SetFrontName(title, EDGE_SHADOW);

	const TitleData * pTitleData = gGlobal.m_resource->GetTitleData(title_id);
	if (pTitleData)
	{
		UInt32 textColor = 0;
		switch (pTitleData->Title_Color)
		{
		case 0:	textColor = TextColor[ColorIndex_White];	break;
		case 1:	textColor = TextColor[ColorIndex_Green];	break;
		case 2:	textColor = TextColor[ColorIndex_Blue];		break;
		case 3:	textColor = TextColor[ColorIndex_Violet];	break;
		case 4:	textColor = TextColor[ColorIndex_Yellow];	break;
		case 5:	textColor = D3DCOLOR_ARGB(255, 255, 204, 0);	break;
		case 6:	textColor = RedColor;	break;
		}
		SetFrontFontColor(textColor);
		GetCharFrontNameView()->SetFontColor(textColor);
	}
	else
	{
		SetFrontFontColor(TextColor[ColorIndex_White]);
		GetCharFrontNameView()->SetFontColor(TextColor[ColorIndex_White]);
		
	}
	//GetCharFrontNameView()->SetFontColor(m_OrgFontColor);

}

VOID CCharacterControl::SetTarget(CCharacterControl* pTarget)
{
	m_pTarget = pTarget;
#ifndef REUBEN_PUBLISH
	if (m_pTarget && gGlobal.pPeerProxy && m_pTarget->m_pCharacter)
	{
		String strMsg;
		strMsg.Format(_T("Client found char_id %d at x %d y %d"), m_pTarget->m_pCharacter->GetChar_id(), m_pTarget->m_pCharacter->GetCurPosX(), m_pTarget->m_pCharacter->GetCurPosY());
		gGlobal.PrintMessage(strMsg, CHANNELTYPE_SYSTEM);
		m_pCharacter->SetTarget(gGlobal.pPeerProxy, gGlobal.g_context, pTarget->m_pCharacter->GetChar_id());
	}
#endif // REUBEN_PUBLISH
}

VOID CCharacterControl::OnUpdatePet(const UInt32 &pet, const UInt32 &pet_id, const String &petName, const UInt16 &petColorIndex)
{

	const PetData* pPetData = gGlobal.m_resource->GetPetData(pet_id);
	if (pPetData)
	{
		if (pPetData->pet_type == PET_TYPE_LITTLE_PET)
		{
			if (!HasParty())
			{
				String mobName = pPetData->pet_model;
				SetEntityView(&m_PetView, mobName, petName, ENTITY_TYPE_MAP_PLAYER, petColorIndex);
				m_CharMainView.AddFollower(&m_PetView.main);

				SetHasFollowPet(true);

				if (GetCharMainView()->IsMount())
				{
					GetCharMainView()->RemovePart(EQUIP_MOUNT);
				}

				String modelName = pPetData->pet_model;
				String colorFileName = modelName.substr(0, modelName.length() - 4);
				char abc = pPetData->pet_color + 96;
				colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
				m_PetView.main.ChangeColorP(modelName.c_str(), colorFileName.c_str());
			}
		}
		else
		{
			m_PetView.main.RemoveFollowTarget();
			GetCharMainView()->RemovePart(EQUIP_MOUNT);
			GetCharMainView()->SetMountOffset(-1 * pPetData->pet_Height);
			GetCharMainView()->LoadEntityFile(String(pPetData->pet_model).c_str(), gGlobal.pD3DDevice, EQUIP_MOUNT);
			String mobName = pPetData->pet_model;
			String colorFileName = mobName.substr(0, mobName.length() - 4);
			char abc = pPetData->pet_color + 96;
			colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
			GetCharMainView()->ChangeColorP(mobName.c_str(), colorFileName.c_str());
			GetCharMainView()->SetAnimationIndexByIndex(ANIM_MOUNT_IDLE1);
			SetHasFollowPet(false);
		}
		
	}
	else
	{
		SetHasFollowPet(false);
		m_PetView.main.RemoveFollowTarget();
		if (GetCharMainView()->IsMount())
		{
			GetCharMainView()->RemovePart(EQUIP_MOUNT);
		}
	}
	RefreshFollower();
}

VOID CCharacterControl::OnUpdatePettwo(const UInt32 &pet_uid,const UInt32 &pet_id,const String &petName,const UInt16 &petColorIndex)
{

	const PetData* pPetData = gGlobal.m_resource->GetPetData(pet_id);
	if (pPetData)
	{
		if (pPetData->pet_type == PET_TYPE_LITTLE_PET )
		{
			if (!HasParty())
			{
				String mobName = pPetData->pet_model;
				SetEntityView(&m_PetTwoView, mobName, petName, ENTITY_TYPE_MAP_PLAYER, petColorIndex);
				m_CharMainView.AddFollower(&m_PetTwoView.main);

				SetHasFollowPetTwo(true);

				String modelName = pPetData->pet_model;
				String colorFileName = modelName.substr(0, modelName.length() - 4);
				char abc = pPetData->pet_color + 96;
				colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
				m_PetTwoView.main.ChangeColorP(modelName.c_str(), colorFileName.c_str());
			}
		}
	}
	else
	{
		SetHasFollowPetTwo(false);
		m_PetTwoView.main.RemoveFollowTarget();
	}
	RefreshFollower();
}

VOID CCharacterControl::SetEntityDesPos(UInt16 x, UInt16 y, BOOL SendToServer)
{
	VMProtectBegin; VM_START;

	if (m_CharMainView.GetFollowTarget())
		return;

	if (gGlobal.g_pParty && !gGlobal.g_pParty->GetAvail())
		return;

	m_CharMainView.SetCharDesPos(x, y);
	if (m_pCharacter && gConfigure.uNetwork && SendToServer)
	{
		UInt16 hint1 = UINT16_MAX_VALUE;
		UInt16 hint2 = UINT16_MAX_VALUE;

		if (gGlobal.g_PathFinder) {
			float dist;
			PathFinder::PointList ptlist;
			PathFinder::GuideList gresult;

			if (gGlobal.g_PathFinder->GetPath(
				m_CharMainView.GetCharPos().PosX, m_CharMainView.GetCharPos().PosY, 
				x, y, dist, ptlist, gresult))
			{
				hint1 = gGlobal.g_PathFinder->GetMinGuild1();
				hint2 = gGlobal.g_PathFinder->GetMinGuild1();
			}
		}

		m_pCharacter->MoveTo(gGlobal.pPeerProxy, gGlobal.g_context, x, y, hint1, hint2);
	}

	VMProtectEnd; VM_END;
}

VOID CCharacterControl::Stop()
{
	VMProtectBegin; VM_START;

	m_CharMainView.StopEntity();
	if (m_pCharacter && gConfigure.uNetwork)
	{
		UInt16 hint1 = UINT16_MAX_VALUE;
		UInt16 hint2 = UINT16_MAX_VALUE;

		if (gGlobal.g_PathFinder) {
			float dist;
			PathFinder::PointList ptlist;
			PathFinder::GuideList gresult;

			if (gGlobal.g_PathFinder->GetPath(
				m_CharMainView.GetCharPos().PosX, m_CharMainView.GetCharPos().PosY, 
				m_CharMainView.GetCharDesPos().PosX, m_CharMainView.GetCharDesPos().PosY, 
				dist, ptlist, gresult))
			{
				hint1 = gGlobal.g_PathFinder->GetMinGuild1();
				hint2 = gGlobal.g_PathFinder->GetMinGuild1();
			}
		}

		m_pCharacter->MoveTo(gGlobal.pPeerProxy, gGlobal.g_context, 
			m_CharMainView.GetCharDesPos().PosX, m_CharMainView.GetCharDesPos().PosY, 
			hint1, hint2);
		if (gGlobal.g_pMiddleMap && QGLApplication::GetInstance().GetMapStateID() != ST_APP_BATTLE)
		{
			if (gGlobal.g_pMiddleMap->IsOpenMiddleMap())
				gGlobal.g_pMiddleMap->DrawPath( );
			else
			{
				gGlobal.g_pMiddleMap->SetCharDes( m_CharMainView.GetCharDesPos().PosX, m_CharMainView.GetCharDesPos().PosY);
				gGlobal.g_pMiddleMap->SetPathEnd( m_CharMainView.GetCharDesPos().PosX, m_CharMainView.GetCharDesPos().PosY);
			}
			
		}

	}

	VMProtectEnd; VM_END;
}

VOID CCharacterControl::SendBattleCommand(UInt8 cmd, UInt8 target, UInt32 sub_type, 
										  UInt32 additional)
{
	if (m_pBatActor)
	{
		if (m_pBatActor->GetClassID() == BatCharacter_CLASSID)
		{
			CBatCharacter* pBatChar = CastNetObject<CBatCharacter>(m_pBatActor);
			if (pBatChar)
				pBatChar->SendCommand(gGlobal.pPeerProxy, gGlobal.g_context, cmd, 
										target, sub_type, additional);
		}
		else
		{
			CBatMob* pBatPartner = CastNetObject<CBatMob>(m_pBatActor);
			if (pBatPartner)
				pBatPartner->SendCommand(gGlobal.pPeerProxy, gGlobal.g_context, cmd, 
										target, sub_type, additional);
		}
		SetIsSentCommand(true);
	}
}

VOID CCharacterControl::CheatCode(const String &code)
{
	if (m_pBatActor)
	{
		CBatCharacter* pBatChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pBatChar)
			pBatChar->CheatCode(gGlobal.pPeerProxy, gGlobal.g_context, code);
	} else if (m_pCharacter) {
		m_pCharacter->CheatCode(gGlobal.pPeerProxy, gGlobal.g_context, 0, code);
	}
}

UInt16 CCharacterControl::GetBattleHp()
{
	if (m_pBatActor)
	{
		return m_pBatActor->GetHP();
	}
	return 1;
}

UInt16 CCharacterControl::GetBattleMaxHp()
{
	if (m_pBatActor)
	{
		return m_pBatActor->GetMaxHP();
	}
	return 1;
}

UInt16 CCharacterControl::GetBattleMHp()
{
	if (m_pBatActor)
	{
		return m_pBatActor->GetMHP();
	}
	return 1;
}
String CCharacterControl::GetCharName()
{
	if (m_pCharacter)
		return m_pCharacter->GetNickName();
	else if (m_pBatActor)
	{
		if (m_pBatActor->GetClassID() == BatCharacter_CLASSID)
		{
			CBatCharacter * pBatChar = CastNetObject<CBatCharacter>(m_pBatActor);
			if (pBatChar)
				return pBatChar->GetNickName();
		}
		else
		{
			CBatMob * pBatMob = CastNetObject<CBatMob>(m_pBatActor);
			if (pBatMob)
			{
				return pBatMob->GetRaname();
			}
		}
		
	}
	return String();
}

UInt8 CCharacterControl::GetJobType()
{
	if (m_pCharacter)
		return m_pCharacter->GetSexClass();
	return 0;
}
UInt32 CCharacterControl::GetMapActorID()
{
	if (m_pCharacter)
		return m_pCharacter->GetChar_id();

	if (m_pBatActor)
	{
		if (m_pBatActor->GetClassID() == BatCharacter_CLASSID)
		{
			CBatCharacter * pBatChar = CastNetObject<CBatCharacter>(m_pBatActor);
			if (pBatChar)
				return pBatChar->GetChar_id();
		}
	}
	return 0;
}

void CCharacterControl::UpdateBuffTime(UInt32 curTime)
{
	int i = 1;
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_MINUTES)
		{
			UInt32 delTime = 0 ;
			if (curTime > Itr->second.PreUpdateTime)
			{
				delTime = (curTime - Itr->second.PreUpdateTime) / 1000;
				if (delTime > 0)
					Itr->second.PreUpdateTime += delTime * 1000;
			}
			if (Itr->second.CoolDown > delTime)
				Itr->second.CoolDown -= delTime;
			else
				Itr->second.CoolDown = 0;

			if (Itr->second.pBuffData->Buff_endtype == BUFF_END_TYPE_TIME_OUT_END &&
				gGlobal.g_pCharacter == this)
			{
				if (Itr->second.CoolDown <= 60)
				{
					StringA name;
					name.Format("BuffFlashIcon%d", i);
					UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
					if (pAniIcon)
						pAniIcon->SetEnable(TRUE);

					if (delTime > 0 && Itr->second.pBuffData == gGlobal.g_pMenuBuffIconProcess->m_pDetailMenuBuff)
					{
						UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("BuffDetailChat");
						if (pChat)
						{
							pChat->ClearAllRow();
							String name;
							name.Format(_T("#W%s"), Itr->second.pBuffData->name);
							pChat->AddStringW(name.c_str());
							pChat->AddStringW(Itr->second.pBuffData->buff_description);
							String remain;
							UInt32 second = Itr->second.CoolDown;
							String time;
							time.Format(_T("%d %s"), second,gGlobal.GetStringTable()->Get(_T("MSG_SECOND")));
							remain.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_DETAIL_REMAIN_TIME")), time.c_str());
							pChat->AddStringW(remain.c_str());
							pChat->EndRow();
						}
					}
				}
			}

			i ++;
		}
	}
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_MINUTES)
		{
			UInt32 delTime = 0 ;
			if (curTime > Itr->second.PreUpdateTime)
			{
				delTime = (curTime - Itr->second.PreUpdateTime) / 1000;
				if (delTime > 0)
					Itr->second.PreUpdateTime += delTime * 1000;
			}
			if (Itr->second.CoolDown > delTime)
				Itr->second.CoolDown -= delTime;
			else
				Itr->second.CoolDown = 0;
		}
	}
}
VOID CCharacterControl::SetMapActorObject(CMapActor* pMapActor)
{
	m_pMapActor = pMapActor;
	m_pCharacter = CastNetObject<CCharacter>(pMapActor);
	CMapActorControl::SetMapActorObject(pMapActor);
	
	if (m_pCharacter)
	{
		m_pCharacter->SetCharacterView(this);
		OnUpdatePartyInfo(m_pCharacter->GetHasParty(), m_pCharacter->GetIsLeader());
		OnUpdateEqHead(m_pCharacter->GetEq_headBand(), m_pCharacter->GetEq_headColor(), m_pCharacter->GetSexClass());

		UInt16 hairStyle1 = m_pCharacter->GetOrg_hairStyle1();
		UInt16 hairColor = m_pCharacter->GetOrg_hairColor();
		UInt32 eqCloth = m_pCharacter->GetOrg_eq_cloth();
		UInt16 eqClothColor = m_pCharacter->GetOrg_eq_clothColor();

		if (m_pCharacter->GetHairStyle1() > 0)
			hairStyle1 = m_pCharacter->GetHairStyle1();
		if (m_pCharacter->GetHairColor() > 0)
			hairColor = m_pCharacter->GetHairColor();
		if (m_pCharacter->GetEq_cloth() > 0)
		{
			eqCloth = m_pCharacter->GetEq_cloth();
			eqClothColor = m_pCharacter->GetEq_clothColor();
		}

		OnUpdateHead(hairStyle1, hairColor, m_pCharacter->GetSexClass());
		OnUpdateEqCloth(eqCloth, eqClothColor, m_pCharacter->GetSexClass());
		
		OnUpdateEqWeapon(m_pCharacter->GetEq_weapon(), m_pCharacter->GetEq_weaponColor(), m_pCharacter->GetSexClass());
		OnUpdatePKInfo(m_pCharacter->GetPK_Value());
		OnUpdateAction(m_pCharacter->GetAction(), m_pCharacter->GetActionTime());
		OnUpdateBuff(m_pCharacter->GetBuff());
		OnUpdateBuffCache(m_pCharacter->GetBuffCache());
		//m_pCharacter->OnUpdatePet(m_pCharacter->GetPet());
		OnUpdateMark(m_pCharacter->GetMark());
		OnUpdatePet(m_pCharacter->GetPet_uid(), m_pCharacter->GetPet_id(), m_pCharacter->GetPetName(), m_pCharacter->GetPetColorIndex());
		m_pCharacter->OnUpdatePartner(m_pCharacter->GetPartner_id(), m_pCharacter->GetPartnerMob_id(), m_pCharacter->GetPartnerName(), m_pCharacter->GetPartnerColorIndex() );
		OnUpdatePettwo(m_pCharacter->GetPettwo_uid(), m_pCharacter->GetPettwo_id(), 
			m_pCharacter->GetPettwoName(), m_pCharacter->GetPettwoColorIndex());
		OnUpdateTitle(m_pCharacter->GetTitle(), m_pCharacter->GetTitle_id());
		OnUpdateFollowerList(m_pCharacter->GetFollowerList());
		OnUpdateTP(m_pCharacter->GetTP(),m_pCharacter->GetModel_mobid(),false);
	}
}


VOID CCharacterControl::OnUpdatePartyInfo(const Boolean &hasParty, const Boolean &isLeader)
{
	if (hasParty && isLeader)
	{
		if (m_pCharacter)
		{
			//add leader icon
			if (m_pCharacter->GetAction() != CCharacter::ST_BATTLE)
			{
				m_CharHeadIconView.InitEntPicInfo();
				m_CharHeadIconView.LoadEntityFile(gConfigure.HeadIconName[PARTY_LEADER_ICON].c_str(), gGlobal.pD3DDevice);
			}
 
			m_CharMainView.SetExtraZBuffer(1);
		}
		else
			m_CharMainView.SetExtraZBuffer(0);
	}
	else
	{
		m_CharHeadIconView.InitEntPicInfo();
		if (m_pCharacter->GetAction() == CCharacter::ST_BATTLE)
			m_CharHeadIconView.LoadEntityFile(gConfigure.HeadIconName[BATTLE_ICON].c_str(), gGlobal.pD3DDevice);
		else if (m_pCharacter->GetAction() == CCharacter::ST_MONITOR)
			m_CharHeadIconView.LoadEntityFile(gConfigure.HeadIconName[OBSERVE_BATTLE_ICON].c_str(), gGlobal.pD3DDevice);
		else if (m_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
			m_CharHeadIconView.LoadEntityFile(gConfigure.HeadIconName[HELP_ICON].c_str(), gGlobal.pD3DDevice);
		
		m_CharMainView.SetExtraZBuffer(0);
	}

	TRACE_INFODTL_3(GLOBAL_LOGGER, _T("OnUpdatePartyInfo !hasParty = %d, m_pCharacter->GetPettwo_uid() = %d,m_pCharacter->GetPet_uid() = %d"), !hasParty,m_pCharacter->GetPettwo_uid(),m_pCharacter->GetPet_uid());
	if (!hasParty &&  m_pCharacter->GetPettwo_uid() >0 )
	{
		OnUpdatePettwo(m_pCharacter->GetPettwo_uid(), m_pCharacter->GetPettwo_id(), 
			m_pCharacter->GetPettwoName(), m_pCharacter->GetPettwoColorIndex());
	}
	if (hasParty && HasFollowPet())
	{
		SetHasFollowPet(false);
		m_PetView.main.RemoveFollowTarget();
	}
	else if (hasParty && HasFollowTwoPet())
	{
		SetHasFollowPetTwo(false);
		m_PetTwoView.main.RemoveFollowTarget();
	}
	else if (!hasParty && m_pCharacter->GetPet_uid() > 0)
		OnUpdatePet(m_pCharacter->GetPet_uid(), m_pCharacter->GetPet_id(), m_pCharacter->GetPetName(), m_pCharacter->GetPetColorIndex());
	RefreshFollower();
}


VOID CCharacterControl::OnUpdateAction(const UInt16 &action, const UInt32 &actionTime)
{
	if (m_pCharacter == NULL)
		return;

	if (action == CCharacter::ST_BATTLE)
	{
		m_CharHeadIconView.InitEntPicInfo();
		m_CharHeadIconView.LoadEntityFile(gConfigure.HeadIconName[BATTLE_ICON].c_str(), gGlobal.pD3DDevice);
		m_CharMainView.StopEntity();
	}
	else if (action == CCharacter::ST_MONITOR)
	{
		m_CharHeadIconView.InitEntPicInfo();
		m_CharHeadIconView.LoadEntityFile(gConfigure.HeadIconName[OBSERVE_BATTLE_ICON].c_str(), gGlobal.pD3DDevice);
		m_CharMainView.StopEntity();
	}
	else if (action == CCharacter::ST_NEEDHELP)
	{
		m_CharHeadIconView.InitEntPicInfo();
		m_CharHeadIconView.LoadEntityFile(gConfigure.HeadIconName[HELP_ICON].c_str(), gGlobal.pD3DDevice);

		//check whether monitoring this character
		/*if (gGlobal.g_pCharacter->GetCharNetObject()->GetMonitorCharID() == GetMapActorID())
		{
			UIButton * pBu = gGlobal.pUIManager->FindUIButton("BattleJoinB");
			if (pBu)
			{
				pBu->SetNormalState();
				gGlobal.pUIManager->AddDirtyUIItem(pBu);
			}
		}*/
	}
	else if (action == CCharacter::ST_SIT)
	{
		m_CharMainView.SetAnimationIndexByIndex(ANIM_SIT);	        

	}
	else if ((m_State == CCharacter::ST_BATTLE ||
		m_State == CCharacter::ST_MONITOR ||
		m_State == CCharacter::ST_NEEDHELP) && m_State != action)
	{
		m_CharHeadIconView.InitEntPicInfo();
		if (m_pCharacter->GetHasParty() && m_pCharacter->GetIsLeader())
			m_CharHeadIconView.LoadEntityFile(gConfigure.HeadIconName[PARTY_LEADER_ICON].c_str(), gGlobal.pD3DDevice);
	}

	CCharacterControl * pCharCtrl = gGlobal.g_pBattleControl->FindCharCtrlByChar_id(GetMapActorID());
	if (pCharCtrl)
	{
		if (action == CCharacter::ST_NEEDHELP)
		{
			pCharCtrl->GetHeadIconView()->InitEntPicInfo();
			pCharCtrl->GetHeadIconView()->LoadEntityFile(gConfigure.HeadIconName[HELP_ICON].c_str(), gGlobal.pD3DDevice);
			if (this == gGlobal.g_pCharacter)
			{
				gGlobal.g_helpEnable = FALSE;
			}
		}
		else if (m_State == CCharacter::ST_NEEDHELP && m_State != action)
			pCharCtrl->GetHeadIconView()->InitEntPicInfo();
	}
	
	UInt16 u16PreState = m_State;
	m_State = action;

	if (gGlobal.g_pCharacter == this)
	{
		if (m_State != CCharacter::ST_BATTLE &&
			m_State != CCharacter::ST_MONITOR && 
			m_State != CCharacter::ST_NEEDHELP &&
			QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
		{
			gGlobal.g_pBattleControl->SetLeaveBattle(TRUE);
		}

		if (u16PreState == CCharacter::ST_MOVE && m_State == CCharacter::ST_STOP) 
		{//run stop,check if reached the jumped npc
			CCrossMapPathFinder* pcmpf = CCrossMapPathFinder::GetInstance();
			if (NULL != pcmpf && pcmpf->GetActiveState()) 
			{//in cross map
				UInt32 uCurJumpPoint = pcmpf->GetCurPathPoint();
				if (UINT_MAX != uCurJumpPoint) 
				{
					const JUMPPOINT* pjp = gGlobal.m_resource->GetAutoPathData(uCurJumpPoint);
					if (NULL != pjp) 
					{
						BlockPos bpCurPos = GetCharMainView()->GetCharPos();
						if (pjp->uJumpType > 0)//npc jump
						{
							if (gGlobal.m_mapdb->m_basicinfo->m_mapid == pjp->uMapId) 
							{//in same map
								if (bpCurPos.PosX == pjp->u16PosX
									&& bpCurPos.PosY == pjp->u16PosY) 
								{//at jump point
									CCharacter* pSelf = GetCharNetObject();
									if (NULL != pSelf)//has character 
									{
										UInt32 uNextJumpSeq = pcmpf->GetNextPathPoint();
										if (UINT_MAX != uNextJumpSeq) 
											pSelf->RequestCrossMapJump(gGlobal.pPeerProxy,gGlobal.g_context,uCurJumpPoint,uNextJumpSeq);
									}
								}
							}
							else
							{
								pcmpf->DestroyPathData();
								gGlobal.m_u32DestNpcId = 0;
							}
						}
					}
				}
			}
			//Open Npc Dialog here,I'm not sure that's a good place
			if (0 != gGlobal.m_u32DestNpcId && NULL != pcmpf && !pcmpf->GetActiveState()) 
			{
				CMapNPCControl* pNpc = QGLApplication::GetInstance().FindNPCCtrlByBaseID(gGlobal.m_u32DestNpcId);
				if (NULL != pNpc) 
				{
					QGLApplication::GetInstance().TalkToNpcByPathFinder(pNpc);
				}
				gGlobal.m_u32DestNpcId=0;
			}
		}
	}
}

VOID CCharacterControl::ChangeWeapon(String Name)
{
	if(m_pCharacter)
	{
		if(m_pCharacter->GetModel_mobid() <= 0)
			m_CharMainView.RemovePart(EQUIP_WEAPON);
		else
		{
			RemoveEqModel(EQUIP_WEAPON);
			RemoveEqModelClour(EQUIP_WEAPON);
			m_charModelLst.insert(std::make_pair(EQUIP_WEAPON,Name.c_str()));
			return;
		}
	}
	m_CharMainView.LoadEntityFile(Name.c_str(), gGlobal.pD3DDevice, EQUIP_WEAPON);
}

VOID CCharacterControl::ChangeBody(UInt8 sexClass, UInt16 cloth, UInt16 clothColor)
{
	if(m_pCharacter)
	{
		if(m_pCharacter->GetModel_mobid() <= 0)
			m_CharMainView.RemovePart(EQUIP_BODY);
		else
		{
			RemoveEqModel(EQUIP_BODY);
			RemoveEqModelClour(EQUIP_BODY);
		}
	}
	else
		m_CharMainView.RemovePart(EQUIP_BODY);

	const ItemData* pItemData = gGlobal.m_resource->GetItemData(cloth);
	String Name;
	if( pItemData )
	{		
		if( sexClass == pItemData->reqSexClass1 )
			Name = String(pItemData->eq_Outlook1);
		else if( sexClass == pItemData->reqSexClass2 )
			Name = String(pItemData->eq_Outlook2);
	}


	if( Name.IsEmpty())
	{
		if (clothColor == 0)
			clothColor = 1;

		if (cloth == 0)
			cloth = 1;
		
		char buf[256];
		char buf2[256];
		char abc = 96 + clothColor;

		sprintf(buf, "b%05d_t%03d.ent", sexClass, cloth);
		sprintf(buf2, "b%05d_t%03d%c.tga", sexClass, cloth, abc);
				

		if (m_pCharacter && m_pCharacter->GetModel_mobid() > 0)
		{
			m_charModelLst.insert(std::make_pair(EQUIP_BODY,String(buf).c_str()));
			ColorPaletteStruct* clour =	CResourcesManager::GetInstance().CreateColorPalette(String(buf2).c_str());
			m_charModelColor.insert(std::make_pair(EQUIP_BODY,clour));
			return;
		}
		m_CharMainView.LoadEntityFile(String(buf).c_str(), gGlobal.pD3DDevice, EQUIP_BODY);
		m_CharMainView.ChangeColorP(String(buf).c_str(), String(buf2).c_str());
	}
	else
	{
		String colorFile;
		if (pItemData)
		{
			switch (clothColor)
			{
			case 1: colorFile = String(pItemData->eq_colorPalette1);	break;
			case 2: colorFile = String(pItemData->eq_colorPalette2);	break;
			case 3: colorFile = String(pItemData->eq_colorPalette3);	break;
			case 4: colorFile = String(pItemData->eq_colorPalette4);	break;
			case 5: colorFile = String(pItemData->eq_colorPalette5);	break;
			}
		}


		if (m_pCharacter && m_pCharacter->GetModel_mobid() > 0)
		{
			m_charModelLst.insert(std::make_pair(EQUIP_BODY,Name.c_str()));
			if (!colorFile.IsEmpty())
			{
				ColorPaletteStruct* clour =	CResourcesManager::GetInstance().CreateColorPalette(colorFile.c_str());
				m_charModelColor.insert(std::make_pair(EQUIP_BODY,clour));			
			}
			return;
		}

		m_CharMainView.LoadEntityFile(Name.c_str(), gGlobal.pD3DDevice, EQUIP_BODY);
		if (!colorFile.IsEmpty())
			m_CharMainView.ChangeColorP(Name.c_str(), colorFile.c_str());
	}
}

VOID CCharacterControl::ChangeHeadAccessory(String Name, String colorFile)
{

	if(m_pCharacter)
	{
		if ( m_pCharacter->GetModel_mobid() <= 0)
			m_CharMainView.RemovePart(EQUIP_HEAD_ACCESSORY);
		else
		{
			RemoveEqModel(EQUIP_HEAD_ACCESSORY);
			RemoveEqModelClour(EQUIP_HEAD_ACCESSORY);
			m_charModelLst.insert(std::make_pair(EQUIP_HEAD_ACCESSORY,Name.c_str()));
			if(!colorFile.IsEmpty())
			{
				ColorPaletteStruct* clour =	CResourcesManager::GetInstance().CreateColorPalette(colorFile.c_str());
				m_charModelColor.insert(std::make_pair(EQUIP_HEAD_ACCESSORY,clour));		
			}
			return;
		}
	}
	else
		m_CharMainView.RemovePart(EQUIP_HEAD_ACCESSORY);
	m_CharMainView.LoadEntityFile(Name.c_str(), gGlobal.pD3DDevice, EQUIP_HEAD_ACCESSORY);
	if (!colorFile.IsEmpty())
		m_CharMainView.ChangeColorP(Name.c_str(), colorFile.c_str());
}

VOID CCharacterControl::RemoveHair()
{
	if(m_pCharacter && m_pCharacter->GetModel_mobid() <= 0)
		m_CharMainView.RemovePart(EQUIP_HAIR);
	else
	{
		RemoveEqModel(EQUIP_HAIR);
		RemoveEqModelClour(EQUIP_HAIR);
		return;
	}
	m_CharMainView.RemovePart(EQUIP_HAIR);
}

VOID CCharacterControl::AddHair(UInt ModelID)
{
	char buf[256];
	char buf2[256];

	UInt16 head = 0;
	UInt16 headColor = 0;

	if (m_pCharacter)
	{
		head = m_pCharacter->GetOrg_hairStyle1();
		if (m_pCharacter->GetHairStyle1() > 0)
			head = m_pCharacter->GetHairStyle1();
		
		headColor = m_pCharacter->GetOrg_hairColor();
		if (m_pCharacter->GetHairColor() > 0)
			headColor = m_pCharacter->GetHairColor();
	}
	else if (m_pBatActor)
	{
		CBatCharacter* pBatChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pBatChar)
		{
			head = pBatChar->GetHairStyle1();
			headColor = pBatChar->GetHairColor();
		}
	}

	if (head == 0)
		head = 1;

	if (headColor == 0)
		headColor = 1;

	char abc = 96 + headColor;
	sprintf(buf, "j%05d_t%03d.ent", ModelID,  head);
	sprintf(buf2, "j%05d_t%03d%c.tga", ModelID, head, abc);


	if(m_pCharacter)
	{
		if(m_pCharacter->GetModel_mobid() <= 0)
			m_CharMainView.RemovePart(EQUIP_HAIR);
		else
		{
			RemoveEqModel(EQUIP_HAIR);
			RemoveEqModelClour(EQUIP_HAIR);
			m_charModelLst.insert(std::make_pair(EQUIP_HAIR,String(buf).c_str()));
			ColorPaletteStruct* clour =	CResourcesManager::GetInstance().CreateColorPalette(String(buf2).c_str());
			m_charModelColor.insert(std::make_pair(EQUIP_HAIR,clour));	
			return;
		}
	}
	else
		m_CharMainView.RemovePart(EQUIP_HAIR);
	m_CharMainView.LoadEntityFile(String(buf).c_str(), gGlobal.pD3DDevice, EQUIP_HAIR);
	m_CharMainView.ChangeColorP(String(buf).c_str(), String(buf2).c_str());
}

VOID CCharacterControl::OnUpdateHead(const UInt32 &head, const UInt16 &headColor, UInt8 sexClass)
{
	char buf[256];
	char buf2[256];

	UInt16 ihead = head;
	UInt16 iheadColor = headColor;	

	if (head == 0)
		ihead = 1;

	if (headColor == 0)
		iheadColor = 1;

	char abc = abc = 96 + iheadColor;

	sprintf(buf, "h%05d_t%03d.ent", sexClass,  ihead);
	sprintf(buf2, "h%05d_t%03d%c.tga", sexClass, ihead, abc);

	if(m_pCharacter)
	{
		if(m_pCharacter->GetModel_mobid() <= 0)
			m_CharMainView.RemovePart(EQUIP_HEAD);
		else
		{
			RemoveEqModel(EQUIP_HEAD);
			RemoveEqModelClour(EQUIP_HEAD);
			m_charModelLst.insert(std::make_pair(EQUIP_HEAD,String(buf).c_str()));
			ColorPaletteStruct* clour =	CResourcesManager::GetInstance().CreateColorPalette(String(buf2).c_str());
			m_charModelColor.insert(std::make_pair(EQUIP_HEAD,clour));	
			return;
		}
	}
	else
		m_CharMainView.RemovePart(EQUIP_HEAD);

	m_CharMainView.LoadEntityFile(String(buf).c_str(), gGlobal.pD3DDevice, EQUIP_HEAD);
	m_CharMainView.ChangeColorP(String(buf).c_str(), String(buf2).c_str());
}

VOID CCharacterControl::CheckTalking(UInt16 CurrX, UInt16 CurrY)
{
	if ( gGlobal.g_bClickNPCFlag )
	{
		QGLApplication::GetInstance().ResumeClickNPCAction(CurrX, CurrY);
	}
}

VOID CCharacterControl::OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor, UInt8 sexClass)
{
	const ItemData* pItemData = gGlobal.m_resource->GetItemData(eq_weapon);
	if( pItemData )
	{
		if( sexClass==pItemData->reqSexClass1 )
			ChangeWeapon(String(pItemData->eq_Outlook1));
		else if( sexClass==pItemData->reqSexClass2 )
			ChangeWeapon(String(pItemData->eq_Outlook2));
		else
			ChangeWeapon(String());
	}
	else
		ChangeWeapon(String());
}
VOID CCharacterControl::OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor, UInt8 sexClass)
{
	const ItemData* pItemData = gGlobal.m_resource->GetItemData(eq_headBand);
	if( pItemData )
	{
		
		if (pItemData->eqType == EQTYPE_HAT_WITH_HAIR)
			AddHair(sexClass);
		else if (pItemData->eqType == EQTYPE_HAT_WITHOUT_HAIR)
			RemoveHair();

		String colorFile;
		switch (eq_headColor)
		{
		case 1: colorFile = pItemData->eq_colorPalette1; break;
		case 2: colorFile = pItemData->eq_colorPalette2; break;
		case 3: colorFile = pItemData->eq_colorPalette3; break;
		case 4: colorFile = pItemData->eq_colorPalette4; break;
		case 5: colorFile = pItemData->eq_colorPalette5; break;
		}
		
		if( sexClass==pItemData->reqSexClass1 )
			ChangeHeadAccessory(String(pItemData->eq_Outlook1), colorFile);
		else if( sexClass==pItemData->reqSexClass2 )
			ChangeHeadAccessory(String(pItemData->eq_Outlook2), colorFile);
		else
		{
			ChangeHeadAccessory(String(), String());
			AddHair(sexClass);
		}
	}
	else
	{
		ChangeHeadAccessory(String(), String());
		AddHair(sexClass);
	}
}
VOID CCharacterControl::OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor, UInt8 sexClass)
{
	ChangeBody(sexClass, eq_cloth, eq_clothColor);
}
VOID CCharacterControl::OnUpdateBattleInfo(const Boolean &EnablePK)
{
	if(m_OrgFontColor == RedColor)
		return;
	if(EnablePK)
		m_OrgFontColor =  PurpleColor;
	else
	{
		if (gGlobal.g_pCharacter == this ||
			(gGlobal.g_pParty && gGlobal.g_pParty->FindMember(GetMapActorID()) != NULL))
			m_OrgFontColor = TextColor[ColorIndex_LightBlue];
		else	
			m_OrgFontColor = D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
			gConfigure.uNameFontColorG, gConfigure.uNameFontColorB);
	}
	if (this == QGLApplication::GetInstance().GetSelectedControl())
		return;

	m_CharNameView.SetFontColor(m_OrgFontColor);
}
VOID CCharacterControl::OnUpdatePKInfo(const UInt16 &PK_Value)
{
	if (PK_Value > 0)
		m_OrgFontColor = RedColor;
	else if(m_OrgFontColor != PurpleColor)
	{
		
	if (gGlobal.g_pCharacter == this ||
		(gGlobal.g_pParty && gGlobal.g_pParty->FindMember(GetMapActorID()) != NULL))
		m_OrgFontColor = TextColor[ColorIndex_LightBlue];
	else	
		m_OrgFontColor = D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
		gConfigure.uNameFontColorG, gConfigure.uNameFontColorB);
	}

	if (this == QGLApplication::GetInstance().GetSelectedControl())
		return;

	m_CharNameView.SetFontColor(m_OrgFontColor);

}

UInt16 CCharacterControl::GetPKValue()
{
	if (m_pCharacter)
		return m_pCharacter->GetPK_Value();

	if (m_pBatActor)
	{
		CBatCharacter* pBatChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pBatChar)
		{
			return pBatChar->GetPK_Value();
		}
	}
	return 0;
}
VOID CCharacterControl::SetFontColor(Color32 Color)
{
	if (GetPKValue() > 0)
		m_OrgFontColor = RedColor;
	else if(m_pCharacter->GetEnablePK())
	{
		m_OrgFontColor = PurpleColor;
	}
	else
		m_OrgFontColor = Color;
}

void CCharacterControl::SetEntityView(EntityView* pView, const String& image, 
									  const String& name, Byte etype, UInt16 colorIndex)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("SetEntityView: main %d, shadow %d, name %d"), 
		&pView->main, &pView->shadow, &pView->name);
	pView->main.InitEntPicInfo();
	pView->main.SetEntityType(etype);
	pView->main.LoadEntityFile(image.c_str(), gGlobal.pD3DDevice);

	String colorFileName = image.substr(0, image.length() - 5);
	char abc = colorIndex + 97;
	colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
	pView->main.ChangeColorP(image.c_str(), colorFileName.c_str());

	pView->main.SetExtraZBuffer(-1);
	pView->main.SetCharPos(m_CharMainView.GetCharPos());
	pView->main.SetEntityMoveSpeed(m_CharMainView.GetEntityMoveSpeed());
	pView->shadow.InitEntPicInfo();
	pView->shadow.LoadEntityFile(gConfigure.strShadowName.c_str(), gGlobal.pD3DDevice);
	pView->shadow.SetExtraZBuffer(-2);
	pView->shadow.SetEntParent(&pView->main);
	StringA strTemp = gConfigure.strFontStyle.c_str();
	pView->name.LoadText(name.c_str(), 
		strTemp.c_str(), gConfigure.uNameFontSize, D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
			gConfigure.uNameFontColorG, gConfigure.uNameFontColorB), EDGE_SHADOW);
	pView->name.SetEntParent(&pView->main);
	pView->name.SetExtraZBuffer(NameExtraZ - 1);
	pView->name.SetFontColor(TextColor[ColorIndex_LightBlue]);
}

void CCharacterControl::SetPartner(UInt32 mob_id, String partnerName, UInt16 colorIndex)
{

	if (mob_id == 0)
	{
		m_PartnerView.main.RemoveFollowTarget();
		//m_CharMainView.RemoveFollower(&m_PartnerView.main);
		return;
	}

	MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(mob_id);
	if (mobTemplate == NULL)
		return;

	String mobName(mobTemplate->m_basicinfo->model);
	mobName = mobName.substr(0, mobName.length() - 4);
	TCHAR temp = mobName.GetAt((int)mobName.length() - 1);

	if (temp == 'b' || temp == 'n')
		mobName = mobName.substr(0, mobName.length() - 1);
	mobName.append(_T("n.ent"));

	SetEntityView(&m_PartnerView, mobName, partnerName, ENTITY_TYPE_MAP_PLAYER, colorIndex);
	m_CharMainView.AddFollower(&m_PartnerView.main);
	RefreshFollower();
}

void CCharacterControl::UpdatePetView(UInt32 CurrTime)
{
	//update anim idle status
	if (m_PetView.main.GetAnimationIndex() == ANIM_IDLE1)
	{
		if (CurrTime > m_AnimIdlePetChangeCounter + m_AnimIdlePetChangeTime)
		{
			m_PetView.main.SetAnimationIndexByIndex(ANIM_IDLE2, FALSE);
		}
	}
	else
	{
		if (m_PetView.main.GetAnimationIndex() == ANIM_IDLE2 &&
			m_PetView.main.FrameEnd(CurrTime))
			m_PetView.main.SetAnimationIndexByIndex(ANIM_IDLE1, TRUE);
		m_AnimIdlePetChangeCounter = CurrTime;
	}
	
	m_PetView.main.UpDate(CurrTime);
	m_PetView.shadow.UpDate(CurrTime);
	m_PetView.name.UpDate(CurrTime);

}

void CCharacterControl::UpdatePetTwoView(UInt32 CurrTime)
{
	if (m_PetTwoView.main.GetAnimationIndex() == ANIM_IDLE1)
	{
		if (CurrTime > m_AnimIdlePetChangeCounter + m_AnimIdlePetChangeTime)
		{
			m_PetTwoView.main.SetAnimationIndexByIndex(ANIM_IDLE2, FALSE);
		}
	}
	else
	{
		if (m_PetTwoView.main.GetAnimationIndex() == ANIM_IDLE2 &&
			m_PetTwoView.main.FrameEnd(CurrTime))
			m_PetTwoView.main.SetAnimationIndexByIndex(ANIM_IDLE1, TRUE);
		m_AnimIdlePetChangeCounter = CurrTime;
	}

	m_PetTwoView.main.UpDate(CurrTime);
	m_PetTwoView.shadow.UpDate(CurrTime);
	m_PetTwoView.name.UpDate(CurrTime);

}

void CCharacterControl::UpdatePartnerView(UInt32 CurrTime)
{
	//update anim idle status
	if (m_PartnerView.main.GetAnimationIndex() == ANIM_IDLE1)
	{
		if (CurrTime > m_AnimIdlePartnerChangeCounter + m_AnimIdlePartnerChangeTime)
		{
			m_PartnerView.main.SetAnimationIndexByIndex(ANIM_IDLE2, FALSE);
		}
	}
	else
	{
		if (m_PartnerView.main.GetAnimationIndex() == ANIM_IDLE2 &&
			m_PartnerView.main.FrameEnd(CurrTime))
			m_PartnerView.main.SetAnimationIndexByIndex(ANIM_IDLE1, TRUE);
		m_AnimIdlePartnerChangeCounter = CurrTime;
	}
	
	m_PartnerView.main.UpDate(CurrTime);
	m_PartnerView.shadow.UpDate(CurrTime);
	m_PartnerView.name.UpDate(CurrTime);

}

void CCharacterControl::UpdateHawkBubbleView(UInt32 CurrTime)
{
	if (m_pHawkBubble == NULL)
	{
		StringA panelName;
		panelName.Format("Hawker%dPanel", GetCharNetObject()->GetChar_id());
		m_pHawkBubble = gGlobal.pUIManager->FindUIPanel(panelName.c_str());
	}
	if (m_pHawkBubble && GetCharMainView() && QGLApplication::GetInstance().GetRenderCamera() )
	{
		if (QGLApplication::GetInstance().GetMapStateID() != ST_APP_BATTLE)
		{
			Int32 posX = GetCharMainView()->GetEntPixelPos().PixelPosX - (Int32)QGLApplication::GetInstance().GetRenderCamera()->GetLeftUpPos().x - m_pHawkBubble->GetWidth() / 2;
			Int32 posY = GetCharMainView()->GetEntPixelPos().PixelPosY - (Int32)QGLApplication::GetInstance().GetRenderCamera()->GetLeftUpPos().y - m_pHawkBubble->GetHeight() - 100;
			m_pHawkBubble->SetPos( posX, posY);
			m_pHawkBubble->SetEnable(true);
		}
		else
			m_pHawkBubble->SetEnable(false);
	}
}

void CCharacterControl::UpdateFollowerView(UInt32 CurrTime)
{
	//update anim idle status
	for (EntityViewList::iterator it = m_FollowerViews.begin(); it != m_FollowerViews.end(); ++it)
	{
		EntityView* pView = *it;
		if (pView == NULL)
			return;

		if (pView->main.GetAnimationIndex() == ANIM_IDLE1)
		{
			if (CurrTime > m_FollowerIdleCounter + m_FollowerIdleTime)
			{
				pView->main.SetAnimationIndexByIndex(ANIM_IDLE2, FALSE);
			}
		}
		else
		{
			if (pView->main.GetAnimationIndex() == ANIM_IDLE2 &&
				pView->main.FrameEnd(CurrTime))
				pView->main.SetAnimationIndexByIndex(ANIM_IDLE1, TRUE);
			m_FollowerIdleCounter = CurrTime;
		}
		
		pView->main.UpDate(CurrTime);
		pView->shadow.UpDate(CurrTime);
		pView->name.UpDate(CurrTime);
	}
}

void CCharacterControl::BlockAllCmd()
{
	for (Int i = 0; i < MAX_ACTION_CMD; i++)
		m_ActionCmdAva[i] = false;
}

bool CCharacterControl::CheckBlockChat(UInt8 ChannelType)
{
	m_tempBuffData = NULL;
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_BlockChat.size() > 0)
		{
			for (Int j = 0; j < (Int)Itr->second.pBuffData->Buff_BlockChat.size(); j++)
			{
				if (Itr->second.pBuffData->Buff_BlockChat[j] == ChannelType + 1)
				{
					m_tempBuffData = Itr->second.pBuffData;
					return true;
				}
				else if (Itr->second.pBuffData->Buff_BlockChat[j] == 255)
				{
					m_tempBuffData = Itr->second.pBuffData;
					return true;
				}
			}
		}
	}

	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_BlockChat.size() > 0)
		{
			for (Int j = 0; j < (Int)Itr->second.pBuffData->Buff_BlockChat.size(); j++)
			{
				if (Itr->second.pBuffData->Buff_BlockChat[j] == ChannelType + 1)
				{
					m_tempBuffData = Itr->second.pBuffData;
					return true;
				}
				else if (Itr->second.pBuffData->Buff_BlockChat[j] == 255)
				{
					m_tempBuffData = Itr->second.pBuffData;
					return true;
				}
			}
		}
	}

	return false;
}

void CCharacterControl::UpdateBlockCmd()
{
	for (Int i = 0; i < MAX_ACTION_CMD; i++)
		m_ActionCmdAva[i] = true;

	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_Blockcommands.size() > 0)
		{
			for (Int j = 0; j < (Int)Itr->second.pBuffData->Buff_Blockcommands.size(); j++)
			{
				switch(Itr->second.pBuffData->Buff_Blockcommands[j])
				{
				case BUFF_BLOCK_COMMAND_ATTACK:
					m_ActionCmdAva[BATCMD_ATTACK] = false;
					break;
				case BUFF_BLOCK_COMMAND_DEFEND:
					m_ActionCmdAva[BATCMD_DEFEND] = false;
					break;
				case BUFF_BLOCK_COMMAND_SKILL:
					m_ActionCmdAva[BATCMD_SKILL] = false;
					break;
				case BUFF_BLOCK_COMMAND_SPECAIL:
					m_ActionCmdAva[BATCMD_SPECIAL] = false;
					break;
				case BUFF_BLOCK_COMMAND_ITEM:
					m_ActionCmdAva[BATCMD_ITEM] = false;
					break;
				case BUFF_BLOCK_COMMAND_PROTECT:
					m_ActionCmdAva[BATCMD_PROTECT] = false;
					break;
				case BUFF_BLOCK_COMMAND_CATCH:
					m_ActionCmdAva[BATCMD_CATCH] = false;
					break;
				case BUFF_BLOCK_COMMAND_HELP:
					m_ActionCmdAva[BATCMD_HELP] = false;
					break;
				case BUFF_BLOCK_COMMAND_AUTO:
					m_ActionCmdAva[BATCMD_AUTO] = false;
					break;
				case BUFF_BLOCK_COMMAND_ESCAPE:
					m_ActionCmdAva[BATCMD_ESCAPE] = false;
					break;
				case BUFF_BLOCK_COMMAND_FRIEND:
					m_ActionCmdAva[BATCMD_FRIEND] = false;
				break;
				case BUFF_BLOCK_COMMAND_ALL:
					for (Int i = 0; i < MAX_ACTION_CMD; i++)
						m_ActionCmdAva[i] = false;
					break;
				}
			}
		}
	}

	// battle script disenable command 
	{
		if ( gGlobal.g_pBatScriptControl && gGlobal.g_pBatScriptControl->DisEnableCmd() )
		{
			for (Int i = 0; i < MAX_ACTION_CMD; i++)
			{
				if ( gGlobal.g_pBatScriptControl->NeedToBlockCmd(i) )
					m_ActionCmdAva[i] = false;
			}
		}
	}

	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_Blockcommands.size() > 0)
		{
			for (Int j = 0; j < (Int)Itr->second.pBuffData->Buff_Blockcommands.size(); j++)
			{
				switch(Itr->second.pBuffData->Buff_Blockcommands[j])
				{
				case BUFF_BLOCK_COMMAND_ATTACK:
					m_ActionCmdAva[BATCMD_ATTACK] = false;
					break;
				case BUFF_BLOCK_COMMAND_DEFEND:
					m_ActionCmdAva[BATCMD_DEFEND] = false;
					break;
				case BUFF_BLOCK_COMMAND_SKILL:
					m_ActionCmdAva[BATCMD_SKILL] = false;
					break;
				case BUFF_BLOCK_COMMAND_SPECAIL:
					m_ActionCmdAva[BATCMD_SPECIAL] = false;
					break;
				case BUFF_BLOCK_COMMAND_ITEM:
					m_ActionCmdAva[BATCMD_ITEM] = false;
					break;
				case BUFF_BLOCK_COMMAND_PROTECT:
					m_ActionCmdAva[BATCMD_PROTECT] = false;
					break;
				case BUFF_BLOCK_COMMAND_CATCH:
					m_ActionCmdAva[BATCMD_CATCH] = false;
					break;
				case BUFF_BLOCK_COMMAND_HELP:
					m_ActionCmdAva[BATCMD_HELP] = false;
					break;
				case BUFF_BLOCK_COMMAND_AUTO:
					m_ActionCmdAva[BATCMD_AUTO] = false;
					break;
				case BUFF_BLOCK_COMMAND_ESCAPE:
					m_ActionCmdAva[BATCMD_ESCAPE] = false;
					break;
				case BUFF_BLOCK_COMMAND_FRIEND:
					m_ActionCmdAva[BATCMD_FRIEND] = false;
					break;
				case BUFF_BLOCK_COMMAND_ALL:
					for (Int i = 0; i < MAX_ACTION_CMD; i++)
						m_ActionCmdAva[i] = false;
					break;
				}
			}
		}
	}

}
void CCharacterControl::UpdateBlockSkill()
{
	m_BlockSkills.clear();
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		for (Int i = 0; i < (Int)Itr->second.pBuffData->Buff_BlockSkills.size(); i++)
		{
			if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_BlockSkills.size() > 0)
			{
				const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(Itr->second.pBuffData->Buff_BlockSkills[i]);
				if (pSkillData)
					m_BlockSkills.insert(std::make_pair(Itr->second.pBuffData->Buff_BlockSkills[i], pSkillData));

			}
		}
	}

	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		for (Int i = 0; i < (Int)Itr->second.pBuffData->Buff_BlockSkills.size(); i++)
		{
			if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_BlockSkills.size() > 0)
			{
				const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(Itr->second.pBuffData->Buff_BlockSkills[i]);
				if (pSkillData)
					m_BlockSkills.insert(std::make_pair(Itr->second.pBuffData->Buff_BlockSkills[i], pSkillData));
			}
		}
	}
}

BOOL CCharacterControl::DetectBuff(UInt16 Buff_ID)
{
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr ++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_DetectBuffs.size() > 0)
		{
			for (UInt i = 0; i < Itr->second.pBuffData->Buff_DetectBuffs.size(); i ++)
			{
				if (Itr->second.pBuffData->Buff_DetectBuffs[i] == Buff_ID)
					return true;
			}
		}
	}

	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr ++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_DetectBuffs.size() > 0)
		{
			for (UInt i = 0; i < Itr->second.pBuffData->Buff_DetectBuffs.size(); i ++)
			{
				if (Itr->second.pBuffData->Buff_DetectBuffs[i] == Buff_ID)
					return true;
			}
		}
	}

	return false;
}

UInt8 CCharacterControl::GetIndex()
{
	if (m_pBatActor)
		return m_pBatActor->GetIndex();
	return 0;
}

UInt16 CCharacterControl::GetTeam()
{
	if (m_pBatActor)
		return m_pBatActor->GetTeam();
	return 0;
}

VOID CCharacterControl::AddBuff(const BuffData * pBuff)
{
	if (pBuff == NULL)
		return;

	if (pBuff->Buff_HideEffect && gGlobal.g_pCharacter 
		&& gGlobal.g_pCharacter->GetAction() == CCharacter::ST_MONITOR)
		return;

	if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE &&
		GetBatActorObject())
	{
	if (!pBuff->Buff_HideEffect || (gGlobal.g_pBattleControl->GetControlPlayer() && 
		((pBuff->Buff_HideEffect && gGlobal.g_pBattleControl->GetControlPlayer()->DetectBuff(pBuff->id) ||
		gGlobal.g_pBattleControl->GetControlPlayer()->GetTeam() == GetTeam()))))
		addBuffList.push_back(pBuff);
	}
	else
	{
		if (!pBuff->Buff_HideEffect)
		{
			m_CharMainView.LoadEntityFile(pBuff->Buff_EffectFile, gGlobal.pD3DDevice, EQUIP_OTHERS);
		}
		else
			addBuffList.push_back(pBuff);

		CallLuaFunc(gGlobal.g_pParentLua, "IsInvisibleBuff", String(), pBuff->id);
		if (gGlobal.g_TempResult > 0)
		{
			if (gGlobal.g_pBattleControl->GetControlPlayer() == NULL || 
				!gGlobal.g_pBattleControl->GetControlPlayer()->DetectBuff(pBuff->id))
			m_CharMainView.SetAlphaOffset(InvisibleAlphaOffset, TRUE);
		}
	}

	if (pBuff->Buff_Blockcommands.size() > 0)
		UpdateBlockCmd();

	if (pBuff->Buff_BlockSkills.size() > 0)
		UpdateBlockSkill();

	
}
VOID CCharacterControl::RemoveBuff(const BuffData * pBuff)
{
	if (pBuff == NULL)
		return;
	
	if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE &&
		GetBatActorObject())
	{
		removeBuffList.push_back(pBuff);
	}
	else
	{
		m_CharMainView.RemovePartByName(pBuff->Buff_EffectFile);
		CallLuaFunc(gGlobal.g_pParentLua, "IsInvisibleBuff", String(), pBuff->id);
		if (gGlobal.g_TempResult > 0)
		{
			m_CharMainView.SetAlphaOffset(0, TRUE);
		}
	}

	if (pBuff->Buff_Blockcommands.size() > 0)
		UpdateBlockCmd();

	if (pBuff->Buff_BlockSkills.size() > 0)
		UpdateBlockSkill();

}

UInt32 CCharacterControl::GetWeaponBuffModValue(UInt8 type)
{
	UInt32 result = 0;
	if (gGlobal.m_pBag)
	{
		for (UInt16 i = 0; i < BAG_EQUIP_COUNT; i++)
		{
			CItem* pItem = gGlobal.m_pBag->GetItem(BAG_MAX_ITEM + i);
			if (pItem && pItem->GetBuffID() > 0 && pItem->GetEndure() > 0)
			{
				const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(pItem->GetBuffID());
				if (pBuffData)
				{
					if (pBuffData->Buff_Mod_Type_1 == type && pItem->GetBuffValue1() > 0)
					{
						result += pItem->GetBuffValue1();
					}

					if (pBuffData->Buff_Mod_Type_2 == type && pItem->GetBuffValue2() > 0)
					{
						result += pItem->GetBuffValue2();
					}

					if (pBuffData->Buff_Mod_Type_3 == type && pItem->GetBuffValue3() > 0)
					{
						result += pItem->GetBuffValue3();
					}
				}
			}
		}
	}

	return result;
}

void CCharacterControl::OnUpdateBuffCache(const String & buffCache)
{
	
	BuffMap rmBuffMap = m_BuffCacheMap;
	BuffMap addBuffMap;

	//m_BuffCacheMap.clear();
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	TCHAR *value = NULL; 
	TCHAR *buf = SafeAllocate(TCHAR, buffCache.size() + 1);
	TCHAR *bufOrg = buf;
	_tcscpy(buf, buffCache.c_str());
	_tcstok(buf, valueDelim);
	bool first = true;
	while (true)
	{
		ExtractedBuffData Data;
		//buff id
		if (first)
		{
			if (buf)
				Data.ID = _ttoi(buf);
			else
				break;
			first = false;
		}
		else
		{

			value = _tcstok(NULL, valueDelim);
			if (value)
				Data.ID = _ttoi(value);
			else
				break;
		}

		//buff Value1
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value1 = _ttoi(value);
		else
			break;

		//buff Value2
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value2 = _ttoi(value);
		else
			break;

		//DOT Value3
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value3 = _ttoi(value);
		else
			break;

		//Pro
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Pro = _ttoi(value);
		else
			break;

		//buff CoolDown
		value = _tcstok(NULL, buffDelim);
		if (value)
		{
			Data.CoolDown = _ttoi(value);
			while (value && (((*value >= 48) && (*value <= 57)) || (*value == 44) || (*value == 130))) // while the values are numbers or ","
			{
				if (value && (*value == 44 || *value == 130)) // check if there is a comma "," in the string
					break;
				value++;
			}
			if (value && (*value == 44 || *value == 130))// buff Repeat
			{
				value++;
				if (value && (*value >= 48) && (*value <= 57))
				{
					Data.Repeat = _ttoi(value);
				}
				else
					Data.Repeat = 0;
			}
			else
				Data.Repeat = 0;
		}
		else
			break;

		const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Data.ID);
		if (pBuffData)
		{
			Data.pBuffData = pBuffData;

			BuffMap::iterator Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
			
			if (Itr == m_BuffCacheMap.end() || Data.ID != Itr->second.ID)
			{
				addBuffMap.insert(std::make_pair(pBuffData->Buff_school, Data));
			}
			else
			{
				BuffMap::iterator Itr = rmBuffMap.find(pBuffData->Buff_school);
				if (Itr != rmBuffMap.end())
					rmBuffMap.erase(Itr);
			}

			if (Itr == m_BuffCacheMap.end())
				m_BuffCacheMap.insert(std::make_pair(pBuffData->Buff_school, Data));
			else
				Itr->second = Data;
		}
	}

	for (BuffMap::iterator Itr = rmBuffMap.begin(); Itr != rmBuffMap.end(); Itr ++)
	{
		if (Itr->second.pBuffData)
		{
			
			BuffMap::iterator ItrMap = m_BuffCacheMap.find(Itr->second.pBuffData->Buff_school);
			UInt16 ID1 = ItrMap->second.ID;
			UInt16 ID2 = Itr->second.ID;
			if (ItrMap != m_BuffCacheMap.end() && ItrMap->second.ID == Itr->second.ID)
				m_BuffCacheMap.erase(ItrMap);

			RemoveBuff(Itr->second.pBuffData);
		}
	}

	for (BuffMap::iterator Itr = addBuffMap.begin(); Itr != addBuffMap.end(); Itr ++)
	{
		AddBuff(Itr->second.pBuffData);
	}
	SafeDeallocate(bufOrg);

	if (gGlobal.g_pCharacter == this)
	{
		gGlobal.g_pMenuBuffIconProcess->ShowBuffIcon();
	}
}
void CCharacterControl::OnUpdateBuff(const String & buff)
{
	BuffMap rmBuffMap = m_BuffMap;
	BuffMap addBuffMap;
	//m_BuffMap.clear();
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	TCHAR *value = NULL; 
	TCHAR *buf = SafeAllocate(TCHAR, buff.size() + 1);
	TCHAR *bufOrg = buf;
	_tcscpy(buf, buff.c_str());
	_tcstok(buf, valueDelim);
	bool first = true;
	while (true)
	{
		ExtractedBuffData Data;
		//buff id
		if (first)
		{
			if (buf)
				Data.ID = _ttoi(buf);
			else
				break;
			first = false;
		}
		else
		{

			value = _tcstok(NULL, valueDelim);
			if (value)
				Data.ID = _ttoi(value);
			else
				break;
		}

		//buff Value1
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value1 = _ttoi(value);
		else
			break;

		//buff Value2
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value2 = _ttoi(value);
		else
			break;

		//DOT Value3
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value3 = _ttoi(value);
		else
			break;

		//Pro
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Pro = _ttoi(value);
		else
			break;

		//buff CoolDown
		value = _tcstok(NULL, buffDelim);
		if (value)
		{
			Data.CoolDown = _ttoi(value);
			while (value && (((*value >= 48) && (*value <= 57)) || (*value == 44) || (*value == 130))) // while the values are numbers or ","
			{
				if (value && (*value == 44 || *value == 130)) // check if there is a comma "," in the string
					break;
				value++;
			}
			if (value && (*value == 44 || *value == 130))// buff Repeat
			{
				value++;
				if (value && (*value >= 48) && (*value <= 57))
				{
					Data.Repeat = _ttoi(value);
				}
				else
					Data.Repeat = 0;
			}
			else
				Data.Repeat = 0;
		}
		else
			break;

		const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Data.ID);
		if (pBuffData)
		{
			Data.pBuffData = pBuffData;

			BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
			
			if (Itr == m_BuffMap.end() || Data.ID != Itr->second.ID)
			{
				addBuffMap.insert(std::make_pair(pBuffData->Buff_school, Data));
				
			}
			else
			{
				BuffMap::iterator Itr = rmBuffMap.find(pBuffData->Buff_school);
				if (Itr != rmBuffMap.end())
					rmBuffMap.erase(Itr);
			}

			if (Itr == m_BuffMap.end())
				m_BuffMap.insert(std::make_pair(pBuffData->Buff_school, Data));			
			else
				Itr->second = Data;
				
		}
	}
	
	for (BuffMap::iterator Itr = rmBuffMap.begin(); Itr != rmBuffMap.end(); Itr ++)
	{
		if (Itr->second.pBuffData)
		{
			
			BuffMap::iterator ItrMap = m_BuffMap.find(Itr->second.pBuffData->Buff_school);
			if (ItrMap != m_BuffMap.end() && ItrMap->second.ID == Itr->second.ID)
				m_BuffMap.erase(ItrMap);
			RemoveBuff(Itr->second.pBuffData);
		}
	}

	for (BuffMap::iterator Itr = addBuffMap.begin(); Itr != addBuffMap.end(); Itr ++)
	{
		AddBuff(Itr->second.pBuffData);
	}

	SafeDeallocate(bufOrg);

	if (gGlobal.g_pCharacter == this)
	{
		gGlobal.g_pMenuBuffIconProcess->ShowBuffIcon();
	}
	else if (gGlobal.g_pCharacter->GetMapActorID() == GetMapActorID())
	{
		gGlobal.g_pCharacter->OnUpdateBuff(buff);
		gGlobal.g_pCharacter->UpdateBuffTime(::timeGetTime());
		gGlobal.g_pMenuBuffIconProcess->ShowBuffIcon();
	}
}

bool CCharacterControl::IsPartner()
{
	if (m_pBatActor && m_pBatActor->GetClassID() == BatMob_CLASSID)
	{
		CBatMob* pMob = CastNetObject<CBatMob>(m_pBatActor);
		return (pMob && pMob->GetPartner_id() > 0);
	}
	else
		return false;
}

CPartner* CCharacterControl::GetPartner()
{
	CPartner* pPartner = NULL;
	if (m_pBatActor && m_pBatActor->GetClassID() == BatMob_CLASSID)
	{
		CBatMob* pMob = CastNetObject<CBatMob>(m_pBatActor);
		if (pMob && pMob->GetPartner_id() > 0)
			pPartner = gGlobal.g_pPartnerGroup->GetPartner(pMob->GetPartner_id());
	}
	return pPartner;
}

CSkillGroup* CCharacterControl::GetSkillGroup()
{	
	CPartner* pPartner = GetPartner();
	if (pPartner)
		return pPartner->m_pSkillGrp;
	else
		return gGlobal.g_SkillLst;
}

UInt16 CCharacterControl::GetSkillCount()
{
	CSkillGroup* grp = GetSkillGroup();
	return (grp ? grp->GetCount() : 0); 
}

CSkill* CCharacterControl::GetSkill(UInt16 index)
{
	CSkill* pSkill = NULL;
	CSkillGroup* grp = GetSkillGroup();

	if (grp)
		pSkill = grp->GetChild<CSkill>(index);

	return pSkill;
}

CSkill* CCharacterControl::GetSkillById(UInt16 skill_id)
{
	CSkill* pSkill = NULL;
	CSkillGroup* grp = GetSkillGroup();

	if (grp)
		pSkill = grp->GetSkillById(skill_id);

	return pSkill;
}

int CCharacterControl::GetSkillLv(int index)
{
	CSkill *pSkill = GetSkillById(index);

	return (pSkill ? pSkill->GetSkill_level() : 0);
}

UInt16 CCharacterControl::GetSP()
{
	if (m_pCharacter)
		return m_pCharacter->GetSP();

	if (m_pBatActor)
		return m_pBatActor->GetSP();

	return 0;
}

UInt16 CCharacterControl::GetHP()
{
	if (m_pCharacter)
		return m_pCharacter->GetHP();

	if (m_pBatActor)
		return m_pBatActor->GetHP();

	return 0;
}

UInt16 CCharacterControl::GetDP()
{
	if (m_pCharacter)
		return m_pCharacter->GetDP();

	if (m_pBatActor)
	{
		CBatCharacter * pChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pChar)
			return pChar->GetDP();
	}
	return 0;
}

bool CCharacterControl::IsBuffExist(int Buff_id)
{
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffMap.end() &&
			Itr->second.ID == Buff_id)
		{
			return true;
		}

		Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffCacheMap.end() &&
			Itr->second.ID == Buff_id)
			return true;
	}
	return false;
}

VOID CCharacterControl::RemoveHideEffectBuff()
{
	BuffMap::iterator Itr;
	for (Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); ++Itr)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_HideEffect)
		{
			RemoveBuff(Itr->second.pBuffData);
		}
	}
	for (Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); ++Itr)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_HideEffect)
		{
			RemoveBuff(Itr->second.pBuffData);
		}
	}
	BuffDataList::iterator Itr2 = addBuffList.begin();
	while (Itr2 != addBuffList.end())
	{
		if ((*Itr2) && (*Itr2)->Buff_HideEffect)
		{
			m_CharMainView.RemovePartByName((*Itr2)->Buff_EffectFile);
			Itr2 = addBuffList.erase(Itr2);
		}
		else
			++Itr2;
	}
}

Int CCharacterControl::GetBatPosition()
{
	if (m_pBatActor)
		return (Int)(m_pBatActor->GetPosition());
	return -1;
}

UInt8 CCharacterControl::GetBatTeam()
{
	if (m_pBatActor)
		return m_pBatActor->GetTeam();
	return 0;
}

UInt16 CCharacterControl::GetLevel()
{
	if (m_pCharacter)
		return m_pCharacter->GetLevel();

	if (m_pBatActor)
		return m_pBatActor->GetLevel();

	return 0;
}

UInt32 CCharacterControl::GetNextLvExp()
{
	if (m_pCharacter)
		return m_pCharacter->GetNextLvExp();

	return 0;
}

UInt16 CCharacterControl::GetMaxHP()
{
	if (m_pCharacter)
		return m_pCharacter->GetMaxHP();

	if (m_pBatActor)
		return m_pBatActor->GetMaxHP();

	return 0;
}

UInt32 CCharacterControl::GetExp()
{
	if (m_pCharacter)
		return m_pCharacter->GetExp();

	return 0;
}

UInt16 CCharacterControl::GetSTA()
{
	if (m_pCharacter)
		return m_pCharacter->GetSTA();

	if (m_pBatActor)
		return m_pBatActor->GetSTA();

	return 0;
}

UInt16 CCharacterControl::GetSPR()
{
	if (m_pCharacter)
		return m_pCharacter->GetSPR();

	if (m_pBatActor)
		return m_pBatActor->GetSPR();

	return 0;
}

UInt16 CCharacterControl::GetSTR()
{
	if (m_pCharacter)
		return m_pCharacter->GetSTR();

	if (m_pBatActor)
		return m_pBatActor->GetSTR();

	return 0;
}

UInt16 CCharacterControl::GetCON()
{
	if (m_pCharacter)
		return m_pCharacter->GetCON();

	if (m_pBatActor)
		return m_pBatActor->GetCON();

	return 0;
}

UInt16 CCharacterControl::GetAGI()
{
	if (m_pCharacter)
		return m_pCharacter->GetAGI();

	if (m_pBatActor)
		return m_pBatActor->GetAGI();

	return 0;
}

UInt16 CCharacterControl::GetATK()
{
	if (m_pCharacter)
		return m_pCharacter->GetATK();

	if (m_pBatActor)
		return m_pBatActor->GetATK();

	return 0;
}

UInt16 CCharacterControl::GetDEF()
{
	if (m_pCharacter)
		return m_pCharacter->GetDEF();

	if (m_pBatActor)
		return m_pBatActor->GetDEF();

	return 0;
}

UInt16 CCharacterControl::GetSPD()
{
	if (m_pCharacter)
		return m_pCharacter->GetSPD();

	if (m_pBatActor)
		return m_pBatActor->GetSPD();

	return 0;
}

UInt16 CCharacterControl::GetPOW()
{
	if (m_pCharacter)
		return m_pCharacter->GetPOW();

	if (m_pBatActor)
		return m_pBatActor->GetPOW();

	return 0;
}

UInt16 CCharacterControl::GetHIT()
{
	if (m_pCharacter)
		return m_pCharacter->GetHIT();

	if (m_pBatActor)
		return m_pBatActor->GetHIT();

	return 0;
}

UInt16 CCharacterControl::GetEVA()
{
	if (m_pCharacter)
		return m_pCharacter->GetEVA();

	if (m_pBatActor)
		return m_pBatActor->GetEVA();

	return 0;
}

UInt16 CCharacterControl::GetMHP()
{
	if (m_pCharacter)
		return m_pCharacter->GetMHP();

	if (m_pBatActor)
		return m_pBatActor->GetMHP();

	return 0;
}

UInt16 CCharacterControl::GetMSP()
{
	if (m_pCharacter)
		return m_pCharacter->GetMSP();

	if (m_pBatActor)
		return m_pBatActor->GetMSP();

	return 0;
}

UInt16 CCharacterControl::GetMaxSP()
{
	if (m_pCharacter)
		return m_pCharacter->GetMaxSP();

	if (m_pBatActor)
		return m_pBatActor->GetMaxSP();

	return 0;
}

UInt16 CCharacterControl::GetMDP()
{
	if (m_pCharacter)
		return m_pCharacter->GetMDP();

	if (m_pBatActor)
	{
		CBatCharacter * pChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pChar)
			return pChar->GetMDP();
	}

	return 0;
}

UInt16 CCharacterControl::GetMaxDP()
{
	if (m_pCharacter)
		return m_pCharacter->GetMaxDP();

	if (m_pBatActor)
	{
		CBatCharacter * pChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pChar)
			return pChar->GetMaxDP();
	}
	return 0;
}

UInt16 CCharacterControl::GetWC()
{
	if (m_pCharacter)
		return m_pCharacter->GetWC();

	if (m_pBatActor)
	{
		CBatCharacter * pChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pChar)
			return pChar->GetWC();
	}

	return 0;
}

UInt16 CCharacterControl::GetWcExp()
{
	if (m_pCharacter)
		return m_pCharacter->GetWcExp();

	if (m_pBatActor)
	{
		CBatCharacter * pChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pChar)
			return pChar->GetWcExp();
	}

	return 0;
}

UInt32 CCharacterControl::GetNextWcExp()
{
	if (m_pCharacter)
		return m_pCharacter->GetNextWCExp();

	return 0;
}

UInt16 CCharacterControl::GetMaxWC()
{
	if (m_pCharacter)
		return m_pCharacter->GetMaxWC();

	if (m_pBatActor)
	{
		CBatCharacter * pChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pChar)
			return pChar->GetMaxWC();
	}

	return 0;
}

UInt16 CCharacterControl::GetVP()
{
	if (m_pCharacter)
		return m_pCharacter->GetVP();

	if (m_pBatActor)
	{
		CBatCharacter * pChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pChar)
			return pChar->GetVP();
	}

	return 0;
}

UInt16 CCharacterControl::GetMaxVP()
{
	if (m_pCharacter)
		return m_pCharacter->GetMaxVP();

	if (m_pBatActor)
	{
		CBatCharacter * pChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pChar)
			return pChar->GetMaxVP();
	}

	return 0;
}

UInt16 CCharacterControl::GetAttrPoint()
{
	if (m_pCharacter)
		return m_pCharacter->GetAttrPoint();
	else
		return 0;
}

UInt32 CCharacterControl::GetYuanBao()
{
	if (m_pCharacter)
		return m_pCharacter->GetYuanBao();
	return 0;
}

UInt32 CCharacterControl::GetMoney()
{
	if (m_pCharacter)
		return m_pCharacter->GetMoney();
	return 0;
}

UInt32 CCharacterControl::GetBankMoney()
{
	if (m_pCharacter)
		return m_pCharacter->GetBankMoney();
	return 0;
}

Int32 CCharacterControl::GetGoodness()
{
	if (m_pCharacter)
		return m_pCharacter->GetGoodness();
	return 0;
}

UInt32 CCharacterControl::GetHonor()
{
	if (m_pCharacter)
		return m_pCharacter->GetHonor();
	return 0;
}

UInt32 CCharacterControl::GetManor()
{
	if (m_pCharacter)
		return m_pCharacter->GetManor();
	return 0;
}

UInt8 CCharacterControl::GetMaxPartner()
{
	if (m_pCharacter)
		return m_pCharacter->GetMaxPartner();
	return 0;
}

UInt8 CCharacterControl::GetMaxPet()
{
	if (m_pCharacter)
		return m_pCharacter->GetMaxPet();
	return 0;
}

UInt16 CCharacterControl::GetPK_Value()
{
	if (m_pCharacter)
		return m_pCharacter->GetPK_Value();
	return 0;
}

Boolean CCharacterControl::GetEnablePK()
{
	if (m_pCharacter)
		return m_pCharacter->GetEnablePK();
	return 0;
}

bool CCharacterControl::HasParty()
{
	if (m_pCharacter)
		return m_pCharacter->GetHasParty();

	if (m_pBatActor)
	{
		CBatCharacter * pChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pChar)
		{
			CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pChar->GetChar_id());
			if (pCtrl)
				return pCtrl->HasParty();
		}

		if (m_pBatActor->IsParnter())
		{
			CBatMob * pMob = CastNetObject<CBatMob>(m_pBatActor);
			CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pMob->GetOwner_id());
			if (pCtrl)
				return pCtrl->HasParty();
		}
	}
	return false;
}

bool CCharacterControl::IsLeader()
{
	if (m_pCharacter)
	{
		if (!m_pCharacter->GetHasParty())
			return false;
		if ( m_pCharacter->GetIsLeader())
			return true;
	}

	if (m_pBatActor)
	{
		CBatCharacter * pChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pChar)
		{
			CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pChar->GetChar_id());
			if (pCtrl)
				return pCtrl->IsLeader();
		}

		if (m_pBatActor->IsParnter())
		{
			CBatMob * pMob = CastNetObject<CBatMob>(m_pBatActor);
			CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pMob->GetOwner_id());
			if (pCtrl)
				return pCtrl->IsLeader();
		}
	}
	return false;
}

bool CCharacterControl::IsMember()
{
	if (m_pCharacter)
	{
		if (m_pCharacter->GetHasParty() && !m_pCharacter->GetIsLeader())
			return true;
	}
	if (m_pBatActor)
	{
		CBatCharacter * pChar = CastNetObject<CBatCharacter>(m_pBatActor);
		if (pChar)
		{
			CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pChar->GetChar_id());
			if (pCtrl)
				return pCtrl->IsMember();
		}

		if (m_pBatActor->IsParnter())
		{
			CBatMob * pMob = CastNetObject<CBatMob>(m_pBatActor);
			CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pMob->GetOwner_id());
			if (pCtrl)
				return pCtrl->IsMember();
		}
	}
	return false;
}


bool CCharacterControl::CheckProtectedPK()
{
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_ProtectPK)
		{
			return true;
		}
	}
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_ProtectPK)
		{
			return true;
		}
	}
	return false;
}
bool CCharacterControl::CheckBlockPK()
{
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_BlockPK)
		{
			return true;
		}
	}
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_BlockPK)
		{
			return true;
		}
	}
	return false;
}

bool CCharacterControl::CheckBlockItem(UInt8 itemType)
{
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_BlockItemType.size() > 0)
		{
			for (Int i = 0; i < (Int)Itr->second.pBuffData->Buff_BlockItemType.size(); i++)
			{
				if (Itr->second.pBuffData->Buff_BlockItemType[i] == itemType)
				{
					m_tempBuffData = Itr->second.pBuffData;
					return true;
				}
			}
		}
	}
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_BlockItemType.size() > 0)
		{
			for (Int i = 0; i < (Int)Itr->second.pBuffData->Buff_BlockItemType.size(); i++)
			{
				if (Itr->second.pBuffData->Buff_BlockItemType[i] == itemType)
				{
					m_tempBuffData = Itr->second.pBuffData;
					return true;
				}
			}
		}
	}
	return false;
}

bool CCharacterControl::AllCmdBlock()
{
	for (Int i = 1; i < MAX_ACTION_CMD; i++)
	{
		if (m_ActionCmdAva[i])
			return false;
	}
	return true;
}

void CCharacterControl::RemoveBuffAfterBattle()
{
	BuffDataList rmBuffIDs;
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); ++ Itr)
	{
		if (Itr->second.pBuffData &&
			Itr->second.pBuffData->Buff_RemoveAftBattle)
			rmBuffIDs.push_back(Itr->second.pBuffData);
	}

	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); ++ Itr)
	{
		if (Itr->second.pBuffData &&
			Itr->second.pBuffData->Buff_RemoveAftBattle)
			rmBuffIDs.push_back(Itr->second.pBuffData);
	}

	for (Int i = 0; i < (Int)rmBuffIDs.size(); i++)
	{
		RemoveBuff(rmBuffIDs[i]);
	}
}

bool CCharacterControl::HasFollower()
{
	return (!m_FollowerViews.empty());
}

int CCharacterControl::GetFollowerCount()
{
	return ((int) m_FollowerViews.size());
}

void CCharacterControl::OnUpdateFollowerList(const UInt32Vector &followerList)
{
	MobDB_Client* mobTemplate;
	EntityView* pView;

	ClearFollower();

	// only single or party leader need entity view of followers
	if (m_pCharacter && m_pCharacter->GetIsLeader())
	{
		for (UInt32Vector::const_iterator it = followerList.begin(); 
			it != followerList.end(); ++it)
		{
			mobTemplate = gGlobal.LoadMobTemplate(*it);
			if (mobTemplate)
			{
				String mobName(mobTemplate->m_basicinfo->model);
				mobName = mobName.substr(0, mobName.length() - 4);
				TCHAR temp = mobName.GetAt((int)mobName.length() - 1);

				if (temp == 'b' || temp == 'n')
					mobName = mobName.substr(0, mobName.length() - 1);
				mobName.append(_T("n.ent"));

				pView = SafeCreateObject(EntityView);
				m_FollowerViews.push_back(pView);
				SetEntityView(pView, mobName, mobTemplate->m_basicinfo->mid_name,
					ENTITY_TYPE_MAP_PLAYER, 0);
				m_CharMainView.AddFollower(&pView->main);
			}
		}
	}
}

void CCharacterControl::RefreshFollower()
{
	if (!m_FollowerViews.empty())
	{
		for (EntityViewList::iterator it = m_FollowerViews.begin(); 
			it != m_FollowerViews.end(); ++it)
		{
			EntityView* pView = *it;
			if (pView)
			{
				pView->main.RemoveFollowTarget();
				m_CharMainView.AddFollower(&pView->main);
			}
		}
	}
}

void CCharacterControl::ClearFollower()
{
	if (m_FollowerViews.empty())
		return;

	EntityView* pView;
	for (EntityViewList::iterator itEnt = m_FollowerViews.begin(); 
		itEnt != m_FollowerViews.end(); ++itEnt)
	{
		pView = *itEnt;
		if (pView) {
			pView->main.RemoveFollowTarget();
			SafeDeleteObject(pView);
		}
	}
	m_FollowerViews.clear();
}

CAnimatedEntity* CCharacterControl::GetFollowerMainView(int i)
{
	if (i < (int) m_FollowerViews.size())
		return &m_FollowerViews[i]->main;
	else
		return NULL;
}

CAnimatedEntity* CCharacterControl::GetFollowerShadowView(int i)
{
	if (i < (int) m_FollowerViews.size())
		return &m_FollowerViews[i]->shadow;
	else
		return NULL;
}

CFontEntity* CCharacterControl::GetFollowerNameView(int i)
{
	if (i < (int) m_FollowerViews.size())
		return &m_FollowerViews[i]->name;
	else
		return NULL;
}

VOID CCharacterControl::OnUpdateMark(const UInt32 &mark)
{
	Boolean visible = mark & (1 << MARK_VISIBLE);
	//m_CharMainView.SetVisible(abs(visible-1));
	GetCharMainView()->SetVisible(abs(visible-1));
	GetCharShadowView()->SetVisible(abs(visible-1));
	GetCharNameView()->SetVisible(abs(visible-1));
	GetHeadIconView()->SetVisible(abs(visible-1));
	GetCharFrontNameView()->SetVisible(abs(visible-1));
}

UInt32 CCharacterControl::GetCharMark()
{
	if (m_pCharacter)
		return m_pCharacter->GetMark();
}

/*
void CCharacterControl::OpenPointsShop(UInt32Vector& Lst, UInt32 &uiTypeID)
{
	RPCContext context1;
	PROCEDURE_DO_METHOD2(CCharacterControl, OpenPointsShop, this, context1, UInt32Vector, Lst, UInt32 ,uiTypeID)
}

RPCResult CCharacterControl::OpenPointsShop( RPCContext &context )
{
	PROCEDURE_START_METHOD2(CCharacterControl, OpenPointsShop, this, context, UInt32Vector, Lst, UInt32 ,uiTypeID)

		m_pCharacter->BuyPointsShopItem(gGlobal.pPeerProxy, gGlobal.g_context, -6, uiTypeID);

	PROCEDURE_WAIT_OBJ2(1, Character, CB_BuyPointsShopItem, character, int, iRetCode, UInt32, uiPoints)

	//	gGlobal.g_pPointsShopProcess->ShowPointsShop(Lst);

	PROCEDURE_CATCH
		return RPC_RESULT_FAIL;
	PROCEDURE_END_CATCH
		return RPC_RESULT_OK;
}*/
VOID CCharacterControl::ChangeModel(UInt32 mob_id,Boolean isBattle)
{
	if(mob_id > 0)
	{
		String charEQ = m_CharMainView.GetPartByEQType(EQUIP_HEAD);
		//head
		if (charEQ.size() > 1)
		{
			if (!HasEqModel(EQUIP_HEAD))
				m_charModelLst.insert(std::make_pair(EQUIP_HEAD,charEQ));
			ColorPaletteStruct* clour = m_CharMainView.GetPartClourByEQType(EQUIP_HEAD);
			if (clour && !HasEqModelClour(EQUIP_HEAD))
			{
				m_charModelColor.insert(std::make_pair(EQUIP_HEAD,clour));
			}
			m_CharMainView.RemovePart(EQUIP_HEAD);
		}
		//body
		charEQ = m_CharMainView.GetPartByEQType(EQUIP_BODY);
		if (charEQ.size() > 1)
		{
			if(!HasEqModel(EQUIP_BODY))
				m_charModelLst.insert(std::make_pair(EQUIP_BODY,charEQ));
			ColorPaletteStruct* clour = m_CharMainView.GetPartClourByEQType(EQUIP_BODY);
			if (clour && !HasEqModelClour(EQUIP_BODY))
			{
				m_charModelColor.insert(std::make_pair(EQUIP_BODY,clour));
			}
			m_CharMainView.RemovePart(EQUIP_BODY);
		}
		//head accessory
		charEQ = m_CharMainView.GetPartByEQType(EQUIP_HEAD_ACCESSORY);
		if (charEQ.size() > 1)
		{
			if(!HasEqModel(EQUIP_HEAD_ACCESSORY))
				m_charModelLst.insert(std::make_pair(EQUIP_HEAD_ACCESSORY,charEQ));
			ColorPaletteStruct* clour = m_CharMainView.GetPartClourByEQType(EQUIP_HEAD_ACCESSORY);
			if (clour && !HasEqModelClour(EQUIP_HEAD_ACCESSORY))
			{
				m_charModelColor.insert(std::make_pair(EQUIP_HEAD_ACCESSORY,clour));
			}
			m_CharMainView.RemovePart(EQUIP_HEAD_ACCESSORY);
		}
		//weapon
		charEQ = m_CharMainView.GetPartByEQType(EQUIP_WEAPON);
		if (charEQ.size() > 1)
		{
			if(!HasEqModel(EQUIP_WEAPON))
				m_charModelLst.insert(std::make_pair(EQUIP_WEAPON,charEQ));
			ColorPaletteStruct* clour = m_CharMainView.GetPartClourByEQType(EQUIP_WEAPON);
			if (clour && !HasEqModelClour(EQUIP_WEAPON))
			{
				m_charModelColor.insert(std::make_pair(EQUIP_WEAPON,clour));
			}
			m_CharMainView.RemovePart(EQUIP_WEAPON);
		}
		//hair
		charEQ = m_CharMainView.GetPartByEQType(EQUIP_HAIR);
		if (charEQ.size() > 1)
		{
			if(!HasEqModel(EQUIP_HAIR))
				m_charModelLst.insert(std::make_pair(EQUIP_HAIR,charEQ));
			ColorPaletteStruct* clour = m_CharMainView.GetPartClourByEQType(EQUIP_HAIR);
			if (clour && !HasEqModelClour(EQUIP_HAIR))
			{
				m_charModelColor.insert(std::make_pair(EQUIP_HAIR,clour));
			}
			m_CharMainView.RemovePart(EQUIP_HAIR);
		}
		if(isBattle)
		{

			MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(mob_id);
			if( mobTemplate )
			{
				m_CharMainView.LoadEntityFile(mobTemplate->m_basicinfo->model, gGlobal.pD3DDevice, EQUIP_BODY);
			}
		}
		else
		{
			NPCDB_Client* npctemp = gGlobal.LoadNpcTemplate(mob_id);
			if( npctemp )
			{
				m_CharMainView.LoadEntityFile(npctemp->m_basicinfo->m_npc_model, gGlobal.pD3DDevice, EQUIP_BODY);
			}
		}
	}
	else
	{
		if(m_charModelLst.size() > 0)
		{
			for (CharModelFile::iterator it =m_charModelLst.begin(); it != m_charModelLst.end(); ++it)
			{
				String CharModelFileName = it->second;
				Equip_Type eqType = it->first;
				m_CharMainView.RemovePart(eqType);
				m_CharMainView.LoadEntityFile(CharModelFileName.c_str(),gGlobal.pD3DDevice,eqType);
				TRACE_INFODTL_1(GLOBAL_LOGGER, _T("changeMob 0 :m_charModelLst eqType = %d"), eqType);
			}
			for (CharModelFileClour::iterator it =m_charModelColor.begin(); it != m_charModelColor.end(); ++it)
			{
				ColorPaletteStruct* clourP = it->second;
				Equip_Type eqType = it->first;
				m_CharMainView.ChangePartClourByEQType(eqType,clourP);
				TRACE_INFODTL_1(GLOBAL_LOGGER, _T("changeMob 0 :m_charModelColor eqType = %d"), eqType);

			}
		}
		m_charModelLst.clear();
		//m_charModelColor.clear();
	}
}


VOID CCharacterControl::OnUpdateTP(const UInt16 &TP,const UInt32 &Model_mobid,const Boolean &isBattle)
{
	ChangeModel(Model_mobid,isBattle);
}

bool  CCharacterControl::HasEqModel(Equip_Type EQUIPTYPE)
{
	for ( CharModelFile::iterator itr =m_charModelLst.begin(); itr != m_charModelLst.end(); itr++)
	{
		Equip_Type eqType = itr->first;
		if(eqType == EQUIPTYPE)
			return true;
	}
	return false;
}

bool  CCharacterControl::HasEqModelClour(Equip_Type EQUIPTYPE)
{
	for (CharModelFileClour::iterator itr = m_charModelColor.begin(); itr != m_charModelColor.end(); itr++)
	{
		Equip_Type eqType = itr->first;
		if(eqType == EQUIPTYPE)
			return true;
	}
	return false;
}

void CCharacterControl::RemoveEqModel(Equip_Type EQUIPTYPE)
{
	CharModelFile::iterator it =m_charModelLst.begin();
	while ( it != m_charModelLst.end())
	{
		Equip_Type eqType = it->first;
		if(eqType == EQUIPTYPE)
		{
			it = m_charModelLst.erase(it);
		}
		else
			it++;
	}
}

void CCharacterControl::RemoveEqModelClour(Equip_Type EQUIPTYPE)
{
	CharModelFileClour::iterator itr = m_charModelColor.begin();
	while ( itr != m_charModelColor.end())
	{
		Equip_Type eqType = itr->first;
		if(eqType == EQUIPTYPE)
		{
			itr = m_charModelColor.erase(itr);
		}
		else
			itr++;
	}
}

String CCharacterControl::GetEqModelFileName(Equip_Type EQUIPTYPE)
{
	String temp = "";
	CharModelFile::iterator it =m_charModelLst.begin();
	while ( it != m_charModelLst.end())
	{
		Equip_Type eqType = it->first;
		if(eqType == EQUIPTYPE)
		{
			temp = it->second;
		}
		else
			it++;
	}
	return temp;
}
