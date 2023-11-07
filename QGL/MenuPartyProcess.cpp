//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuPartyProcess.h"
//-- Library
#include "QGLApplication.h"
#include "QGL/Global.h"
#include "QGL/CharacterControl.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "Proxy/Logic/Party.h"
#include "QGL/BattleControl.h"
#include "MenuGivenProcess.h"
#include "MenuTradingProcess.h"
#include "Proxy/Logic/MessageCharacter.h"
#include "Proxy/Logic/Messenger.h"
#include "MenuFriendProcess.h"
#include "MenuTargetProcess.h"
#include "Resource/ResLineup.h"
#include "Proxy/Logic/PartyCharInfoClient.h"
#include "TeamDescription.h"

void CMenuPartyProcess::AddPartyList(CParty* pParty)
{
	for (Index i = C_INDEX(0); i < m_PartyList.size(); ++i)
	{
		if (pParty->GetNetId() == m_PartyList[i]->GetNetId())
			return;
	}

	m_PartyList.push_back(pParty);
}

void CMenuPartyProcess::ClearPartyList()
{
	for (Index i = C_INDEX(0); i < m_PartyList.size(); ++i)
	{
		CParty* temp = m_PartyList[i];
		DeleteNetGroup(temp);
	}
	m_PartyList.clear();
	m_iPartyIndex = 0;
}


void CMenuPartyProcess::InviteComing(CParty* pParty)
{
	AddPartyList(pParty);
	SetInviting(true);
	UIButton* button = (UIButton*)gGlobal.pUIManager->GetUIItem("DownBParty");
	if (button)
	{
		button->SetLockState(false);
		button->SetState(_ButtonNotice);
		button->SetLockState(true);
		gGlobal.pUIManager->AddDirtyUIItem(button);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayAddTeamEffect", String());
	}
	ShowPartyByIndex();
}

void CMenuPartyProcess::ResetPartyPanel()
{
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("TeammatePanel");

	if (panel == NULL)
		return;

	UIAniIcon * pAniIcon;
	UIStaticText* characterName;
	UIStaticText* characterLevel;
	UIStaticText* characterClass;
	UIStaticText* characterFaction;
	UIStaticText* characterManor;
	UIImage*	  rightClickImage;

	StringA aniIcon;
	StringA name;
	StringA level;
	StringA cclass;
	StringA faction;
	StringA distriction;
	StringA popUp;

	for (Index i = C_INDEX(0) ; i < 5; ++i)
	{
		aniIcon.Format("TeammateCharacter%d", i+1);
		name.Format("TeammateName%d", i+1);
		level.Format("TeammateLevel%d", i+1);
		cclass.Format("TeammateClass%d", i+1);
		faction.Format("TeammateFaction%d", i+1);
		distriction.Format("TeammateDist%d", i+1);
		popUp.Format("TeammatePopUp%d", i+1);

		pAniIcon = (UIAniIcon*) gGlobal.pUIManager->GetUIItem(aniIcon.c_str());
		characterName = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(name.c_str());
		characterLevel = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(level.c_str());
		characterClass = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(cclass.c_str());
		characterFaction = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(faction.c_str());
		characterManor = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(distriction.c_str());
		rightClickImage = (UIImage*)gGlobal.pUIManager->FindUIImage(popUp.c_str());

		if (pAniIcon)
			pAniIcon->SetEnable(false);

		if (characterName)
			characterName->SetEnable(false);

		if (characterLevel)
			characterLevel->SetEnable(false);

		if (characterClass)
			characterClass->SetEnable(false);

		if (characterFaction)
			characterFaction->SetEnable(false);

		if (characterManor)
			characterManor->SetEnable(false);

		if (rightClickImage)
			rightClickImage->SetEnable(false);
	
	}
}

void CMenuPartyProcess::UpdatePartyPanel(CParty* pParty)
{
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("TeammatePanel");

	if (panel == NULL)
		return;

	if (pParty)
	{
		CGameStringTable* stringTable = gGlobal.GetStringTable();

		NetObject * netobj;
		CCharacter * pChar;
		UIAniIcon * pAniIcon;
		UIStaticText* characterName;
		UIStaticText* characterLevel;
		UIStaticText* characterClass;
		UIStaticText* characterFaction;
		UIStaticText* characterDist;

		StringA aniIcon;
		StringA name;
		StringA level;
		StringA cclass;
		StringA faction;
		StringA distriction;

		ResetPartyPanel();

		if (pParty == gGlobal.g_pParty || pParty == gGlobal.g_pTempLeaveParty)
		{	
			UIImage* rightClickImage;
			StringA popUp;

			for (Index i = C_INDEX(0); i < gGlobal.g_vctPartyMemberPointer.size();i++)
			{
				popUp.Format("TeammatePopUp%d", i+1);
				rightClickImage = (UIImage*)gGlobal.pUIManager->FindUIImage(popUp.c_str());
				if (rightClickImage)
					rightClickImage->SetEnable(true);
			}
			Boolean bTempLeave = (gGlobal.g_pParty == NULL && gGlobal.g_pTempLeaveParty != NULL);
			for (Index i = C_INDEX(0); i < gGlobal.g_vctPartyMemberPointer.size();i++)
			{
				netobj = gGlobal.g_vctPartyMemberPointer[i];
				pChar = CastNetObject<CCharacter>(netobj);
				if ( (!pParty->GetM_bTempLeave(i) && !bTempLeave && pChar != NULL) || pChar == gGlobal.g_pCharacter->GetCharNetObject()) 
				{//same map not temporarily leave or myself client
					if( netobj && netobj->GetClassID()==Character_CLASSID )
					{
						aniIcon.Format("TeammateCharacter%d", i+1);
						name.Format("TeammateName%d", i+1);
						level.Format("TeammateLevel%d", i+1);
						cclass.Format("TeammateClass%d", i+1);
						faction.Format("TeammateFaction%d", i+1);
						distriction.Format("TeammateDist%d", i+1);

						pAniIcon = (UIAniIcon*) gGlobal.pUIManager->GetUIItem(aniIcon.c_str());
						characterName = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(name.c_str());
						characterLevel = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(level.c_str());
						characterClass = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(cclass.c_str());
						characterFaction = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(faction.c_str());
						characterDist = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(distriction.c_str());


						pAniIcon->SetEnable(true);
						characterName->SetEnable(true);
						characterLevel->SetEnable(true);
						characterClass->SetEnable(true);
						characterFaction->SetEnable(true);
						characterDist->SetEnable(true);

						UInt32 eqCloth = pChar->GetOrg_eq_cloth();
						UInt16 eqClothColor = pChar->GetOrg_eq_clothColor();
						UInt16 hairStyle = pChar->GetOrg_hairStyle1();
						UInt16 hairColor = pChar->GetOrg_hairColor();

						if (pChar->GetHairStyle1() > 0)
							hairStyle = pChar->GetHairStyle1();
						if (pChar->GetHairColor() > 0)
							hairColor = pChar->GetHairColor();
						if (pChar->GetEq_cloth() > 0)
						{
							eqCloth = pChar->GetEq_cloth();
							eqClothColor = pChar->GetEq_clothColor();
						}

						CAnimatedEntity temp = gGlobal.GetCharModel(pChar->GetSexClass(), hairStyle, hairColor, 
							eqCloth, eqClothColor, pChar->GetEq_headBand(), pChar->GetEq_weapon() );

						pAniIcon->SetAnimationData(&temp);

						char buf[256];
						characterName->SetText(pChar->GetNickName().c_str());
						characterName->Redraw();

						sprintf(buf, "%d", pChar->GetLevel());
						characterLevel->SetText(String(buf).c_str());
						characterLevel->Redraw();

						String tempClass;
						String tempClassName;
						tempClassName.Format(_T("MSG_ACTOR_CLASS_%d"), pChar->GetSexClass());
						tempClass = stringTable->Get(tempClassName.c_str());

						characterClass->SetText(tempClass);
						characterClass->Redraw();

						String tempRing;

						switch ( pChar->GetFaction())
						{
						case 1:
							tempRing = stringTable->Get(_T("MSG_FACTION_SIXFANDOOR"));
							break;
						case 2:
							tempRing = stringTable->Get(_T("MSG_FACTION_TWELVEDOCK"));
							break;
						case 3:
							tempRing = stringTable->Get(_T("MSG_FACTION_HOLY"));
							break;
						case 4:
							tempRing = stringTable->Get(_T("MSG_FACTION_SILVERCASINO"));
							break;
						case 5:
							tempRing = stringTable->Get(_T("MSG_FACTION_TONGDOOR"));
							break;
						case 6:
							tempRing = stringTable->Get(_T("MSG_FACTION_10THOUSAND"));
							break;
						case 7:
							tempRing = stringTable->Get(_T("MSG_FACTION_GREENCLOTH"));
							break;
						case 8:
							tempRing = stringTable->Get(_T("MSG_FACTION_GHOSTAREA"));
							break;

						}

						characterFaction->SetText(tempRing);
						characterFaction->Redraw();

						UInt16 uMapId = pChar->GetMap_id();
						const BinaryClientMapInfo* pCliMapInfo = gGlobal.m_resource->GetCliMapData(uMapId);
						if (NULL != pCliMapInfo && characterDist != NULL) 
						{
							characterDist->SetText(pCliMapInfo->m_mapname);
							characterDist->Redraw();
						}
						//pChar->GetManor();
					}
				}
				else//not in same map,if client temporarily leave.thats other member client has no data for the character
				{
					pChar = CastNetObject<CCharacter>(netobj);
					aniIcon.Format("TeammateCharacter%d", i+1);
					name.Format("TeammateName%d", i+1);
					level.Format("TeammateLevel%d", i+1);
					cclass.Format("TeammateClass%d", i+1);
					faction.Format("TeammateFaction%d", i+1);
					distriction.Format("TeammateDist%d", i+1);

					pAniIcon = (UIAniIcon*) gGlobal.pUIManager->GetUIItem(aniIcon.c_str());
					characterName = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(name.c_str());
					characterLevel = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(level.c_str());
					characterClass = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(cclass.c_str());
					characterFaction = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(faction.c_str());
					characterDist = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(distriction.c_str());


					pAniIcon->SetEnable(true);
					characterName->SetEnable(true);
					characterLevel->SetEnable(true);
					characterClass->SetEnable(true);
					characterFaction->SetEnable(true);
					characterDist->SetEnable(true);

					Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.find(i);
					if (it != gGlobal.g_mapPartyChar.end()) 
					{
						CPartyCharInfoClient* pCpc = it->second;
						if (NULL != pCpc) 
						{
							UInt32 eqCloth = pCpc->GetU32OrgCloth();
							UInt16 eqClothColor = pCpc->GetU16OrgClothColor();
							UInt16 hairStyle = pCpc->GetU16OrgHairStyle();
							UInt16 hairColor = pCpc->GetU16OrgHairColor();

							if (pCpc->GetU16HairStyle1() > 0)
								hairStyle = pCpc->GetU16HairStyle1();
							if (pCpc->GetU16HairColor() > 0)
								hairColor = pCpc->GetU16HairColor();
							if (pCpc->GetU32Cloth() > 0)
							{
								eqCloth = pCpc->GetU32Cloth();
								eqClothColor = pCpc->GetU16ClothColor();
							}

							CAnimatedEntity temp = gGlobal.GetCharModel(pCpc->GetU8SexClass(), hairStyle, hairColor, 
								eqCloth, eqClothColor, pCpc->GetU16HeadBand(), pCpc->GetU32Weapon() );

							pAniIcon->SetAnimationData(&temp);

							char buf[256];
							characterName->SetText(pCpc->GetStrNickName().c_str());
							characterName->Redraw();

							sprintf(buf, "%d", pCpc->GetU16Level());
							characterLevel->SetText(String(buf).c_str());
							characterLevel->Redraw();

							String tempClass;
							String tempClassName;
							tempClassName.Format(_T("MSG_ACTOR_CLASS_%d"), pCpc->GetU8SexClass());
							tempClass = stringTable->Get(tempClassName.c_str());

							characterClass->SetText(tempClass);
							characterClass->Redraw();

							String tempRing;

							switch ( pCpc->GetU8Faction())
							{
							case 1:
								tempRing = stringTable->Get(_T("MSG_FACTION_SIXFANDOOR"));
								break;
							case 2:
								tempRing = stringTable->Get(_T("MSG_FACTION_TWELVEDOCK"));
								break;
							case 3:
								tempRing = stringTable->Get(_T("MSG_FACTION_HOLY"));
								break;
							case 4:
								tempRing = stringTable->Get(_T("MSG_FACTION_SILVERCASINO"));
								break;
							case 5:
								tempRing = stringTable->Get(_T("MSG_FACTION_TONGDOOR"));
								break;
							case 6:
								tempRing = stringTable->Get(_T("MSG_FACTION_10THOUSAND"));
								break;
							case 7:
								tempRing = stringTable->Get(_T("MSG_FACTION_GREENCLOTH"));
								break;
							case 8:
								tempRing = stringTable->Get(_T("MSG_FACTION_GHOSTAREA"));
								break;

							}

							characterFaction->SetText(tempRing);
							characterFaction->Redraw();

							UInt16 uMapId = pCpc->GetU16MapId();
							const BinaryClientMapInfo* pCliMapInfo = gGlobal.m_resource->GetCliMapData(uMapId);
							if (NULL != pCliMapInfo && characterDist != NULL) 
							{
								characterDist->SetText(pCliMapInfo->m_mapname);
								characterDist->Redraw();
							}
						}
					}
				}
			}
			UIButton* pLeaveTemp = (UIButton*) gGlobal.pUIManager->FindUIButton("LeaveTemporarily");
			UIButton* pReturnParty = (UIButton*) gGlobal.pUIManager->FindUIButton("ReturnParty");
			if (pParty->IsLeader()) 
			{
				pLeaveTemp->SetEnable(false);
				pReturnParty->SetEnable(false);
			}
			else
			{
				if (gGlobal.g_pParty != NULL ) 
				{
					pLeaveTemp->SetEnable(true);
					pReturnParty->SetEnable(false);
				}
				else if (gGlobal.g_pTempLeaveParty != NULL) 
				{
					pLeaveTemp->SetEnable(false);
					pReturnParty->SetEnable(true);
				}
			}
		}
		else
		{
			for (Index j = C_INDEX(0); j < pParty->GetCount();j++)
			{
				netobj = pParty->GetChild(j);
				pChar = CastNetObject<CCharacter>(netobj);
				if (pChar != NULL) 
				{//same map not temporarily leave or myself client
					if( netobj && netobj->GetClassID()==Character_CLASSID )
					{
						aniIcon.Format("TeammateCharacter%d", j+1);
						name.Format("TeammateName%d", j+1);
						level.Format("TeammateLevel%d", j+1);
						cclass.Format("TeammateClass%d", j+1);
						faction.Format("TeammateFaction%d", j+1);
						distriction.Format("TeammateDist%d", j+1);

						pAniIcon = (UIAniIcon*) gGlobal.pUIManager->GetUIItem(aniIcon.c_str());
						characterName = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(name.c_str());
						characterLevel = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(level.c_str());
						characterClass = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(cclass.c_str());
						characterFaction = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(faction.c_str());
						characterDist = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(distriction.c_str());


						pAniIcon->SetEnable(true);
						characterName->SetEnable(true);
						characterLevel->SetEnable(true);
						characterClass->SetEnable(true);
						characterFaction->SetEnable(true);
						characterDist->SetEnable(true);

						UInt32 eqCloth = pChar->GetOrg_eq_cloth();
						UInt16 eqClothColor = pChar->GetOrg_eq_clothColor();
						UInt16 hairStyle = pChar->GetOrg_hairStyle1();
						UInt16 hairColor = pChar->GetOrg_hairColor();

						if (pChar->GetHairStyle1() > 0)
							hairStyle = pChar->GetHairStyle1();
						if (pChar->GetHairColor() > 0)
							hairColor = pChar->GetHairColor();
						if (pChar->GetEq_cloth() > 0)
						{
							eqCloth = pChar->GetEq_cloth();
							eqClothColor = pChar->GetEq_clothColor();
						}

						CAnimatedEntity temp = gGlobal.GetCharModel(pChar->GetSexClass(), hairStyle, hairColor, 
							eqCloth, eqClothColor, pChar->GetEq_headBand(), pChar->GetEq_weapon() );

						pAniIcon->SetAnimationData(&temp);

						char buf[256];
						characterName->SetText(pChar->GetNickName().c_str());
						characterName->Redraw();

						sprintf(buf, "%d", pChar->GetLevel());
						characterLevel->SetText(String(buf).c_str());
						characterLevel->Redraw();

						String tempClass;
						String tempClassName;
						tempClassName.Format(_T("MSG_ACTOR_CLASS_%d"), pChar->GetSexClass());
						tempClass = stringTable->Get(tempClassName.c_str());

						characterClass->SetText(tempClass);
						characterClass->Redraw();

						String tempRing;

						switch ( pChar->GetFaction())
						{
						case 1:
							tempRing = stringTable->Get(_T("MSG_FACTION_SIXFANDOOR"));
							break;
						case 2:
							tempRing = stringTable->Get(_T("MSG_FACTION_TWELVEDOCK"));
							break;
						case 3:
							tempRing = stringTable->Get(_T("MSG_FACTION_HOLY"));
							break;
						case 4:
							tempRing = stringTable->Get(_T("MSG_FACTION_SILVERCASINO"));
							break;
						case 5:
							tempRing = stringTable->Get(_T("MSG_FACTION_TONGDOOR"));
							break;
						case 6:
							tempRing = stringTable->Get(_T("MSG_FACTION_10THOUSAND"));
							break;
						case 7:
							tempRing = stringTable->Get(_T("MSG_FACTION_GREENCLOTH"));
							break;
						case 8:
							tempRing = stringTable->Get(_T("MSG_FACTION_GHOSTAREA"));
							break;

						}

						characterFaction->SetText(tempRing);
						characterFaction->Redraw();

						UInt16 uMapId = pChar->GetMap_id();
						const BinaryClientMapInfo* pCliMapInfo = gGlobal.m_resource->GetCliMapData(uMapId);
						if (NULL != pCliMapInfo && characterDist != NULL) 
						{
							characterDist->SetText(pCliMapInfo->m_mapname);
							characterDist->Redraw();
						}
						//pChar->GetManor();
					}
				}
			}
			UIButton* pLeaveTemp = (UIButton*) gGlobal.pUIManager->FindUIButton("LeaveTemporarily");
			UIButton* pReturnParty = (UIButton*) gGlobal.pUIManager->FindUIButton("ReturnParty");
			pLeaveTemp->SetEnable(false);
			pReturnParty->SetEnable(false);
		}

		UIButton* pSelectLineup = (UIButton*) gGlobal.pUIManager->FindUIButton("FormationChosen");
		if (pSelectLineup)
		{
			if (!pParty->IsLeader())
				pSelectLineup->SetEnable(false);
			else 
				pSelectLineup->SetEnable(true);
		}
	
		UIButton* pLineupInfoView = (UIButton*) gGlobal.pUIManager->FindUIButton("LineupInfoView");
		if (pLineupInfoView )
		{
			if (pParty->IsLeader())
				pLineupInfoView->SetEnable(false);
			else 
				pLineupInfoView->SetEnable(true);
		}

		const LINEUPINFO* pSelLineup=gGlobal.m_resource->GetLineupData(pParty->GetM_uLineupId());
		if (NULL != pSelLineup) {
			UIStaticText* pLineupDes= (UIStaticText*)gGlobal.pUIManager->GetUIItem("FormationName");
			if (NULL != pLineupDes) {
				pLineupDes->SetText(pSelLineup->szName);
				pLineupDes->Redraw();
			}
		}

		UIStaticText* pText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("TeamMenuLineupKeyPos");
		if (NULL != pText) {
			String strTextKeyPos;
			strTextKeyPos.Format(_T("%d"),pParty->GetM_iLineupKeyPos()+1);
			pText->SetText(strTextKeyPos);
			pText->Redraw();
		}

		UIButton* applyJoinList = (UIButton*) gGlobal.pUIManager->FindUIButton("ApplyMembership");
		if (applyJoinList)
		{
			if (!pParty->IsLeader())
				applyJoinList->SetDisableState();
			else 
				applyJoinList->SetNormalState();
		}

		UIButton* AutoAcceptButton = (UIButton*) gGlobal.pUIManager->FindUIButton("AutoAcceptButton");
		if (AutoAcceptButton)
		{
			if (!pParty->IsLeader())
				AutoAcceptButton->SetDisableState();
			else 
				AutoAcceptButton->SetNormalState();
		}

		UICheckBox* pCheckBox = gGlobal.pUIManager->FindUICheckBox("AutoAcceptCheckBox");
		if (pCheckBox)
		{
			if (!pParty->IsLeader())
				pCheckBox->SetEnable(false);
			else
				pCheckBox->SetState(m_bAutoAccept);
		}


		UIButton* TeamDescription = (UIButton*) gGlobal.pUIManager->FindUIButton("TeamDescriptionButton");
		if (TeamDescription)
		{
			if (!pParty->IsLeader())
				TeamDescription->SetDisableState();
			else 
				TeamDescription->SetNormalState();
		}

		UIStaticText* pTeamDescriptionText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("TeamDescriptionText");
		if(pTeamDescriptionText)
		{
			String strDesc(pParty->GetDesc());
			pTeamDescriptionText->SetText(strDesc);
			pTeamDescriptionText->Redraw();
		}



		UIButton* inviteButton = (UIButton*)gGlobal.pUIManager->FindUIButton("InviteToJoin");
		if (inviteButton)
		{
			if (pParty->GetCount() < 5 && pParty->IsLeader())
				inviteButton->SetNormalState();
			else
				inviteButton->SetDisableState();
		}

		UIButton* messageButton = (UIButton*)gGlobal.pUIManager->FindUIButton("Signal");
		if (messageButton)
			messageButton->SetDisableState();

		/*UIButton* formatButton = (UIButton*)gGlobal.pUIManager->FindUIButton("FormationChosen");
		if (formatButton)
			formatButton->SetDisableState();*/

		UIButton* leaveButton = (UIButton*)gGlobal.pUIManager->FindUIButton("LeaveParty");
		if (leaveButton)
			leaveButton->SetNormalState();

		gGlobal.pUIManager->AddDirtyUIItem(panel);
	}
	m_iPreChangeIndex = UCHAR_MAX;
}

void CMenuPartyProcess::ShowPartyByIndex(int index)
{
	if (index < (int)m_PartyList.size() && (index >= 0))
	{
		if (index == 0)
		{
			UIButton* prevTeam = (UIButton*)gGlobal.pUIManager->FindUIButton("PrevTeam");
			if (prevTeam)
				prevTeam->SetEnable(false);
		}
		else
		{
			UIButton* prevTeam = (UIButton*)gGlobal.pUIManager->FindUIButton("PrevTeam");
			if (prevTeam)
				prevTeam->SetEnable(true);
		}

		if (index == ((int)m_PartyList.size() - 1))
		{
			UIButton* nextTeam = (UIButton*)gGlobal.pUIManager->FindUIButton("NextTeam");
			if (nextTeam)
				nextTeam->SetEnable(false);
		}
		else if ((int)m_PartyList.size() > 1 )
		{	
			UIButton* nextTeam = (UIButton*)gGlobal.pUIManager->FindUIButton("NextTeam");
			if (nextTeam)
				nextTeam->SetEnable(true);
		}

		m_iPartyIndex = index;
		UpdatePartyPanel(m_PartyList[index]);
	}
	else if (index < 0)
	{
		if (m_iPartyIndex == 0)
		{
			UIButton* prevTeam = (UIButton*)gGlobal.pUIManager->FindUIButton("PrevTeam");
			if (prevTeam)
				prevTeam->SetEnable(false);
		}
		else
		{
			UIButton* prevTeam = (UIButton*)gGlobal.pUIManager->FindUIButton("PrevTeam");
			if (prevTeam)
				prevTeam->SetEnable(true);
		}

		if (m_iPartyIndex == ((int)m_PartyList.size() - 1))
		{
			UIButton* nextTeam = (UIButton*)gGlobal.pUIManager->FindUIButton("NextTeam");
			if (nextTeam)
				nextTeam->SetEnable(false);
		}
		else if ((int)m_PartyList.size() > 1 )
		{	
			UIButton* nextTeam = (UIButton*)gGlobal.pUIManager->FindUIButton("NextTeam");
			if (nextTeam)
				nextTeam->SetEnable(true);
		}

		UpdatePartyPanel(m_PartyList[m_iPartyIndex]);
	}
}

void CMenuPartyProcess::InviteAccept()
{
	VMProtectBegin; VM_START;

	if (m_PartyList[m_iPartyIndex])
	{
		RPCContext context;
		gGlobal.g_pCharacter->GetCharNetObject()->PartyInviteReply(gGlobal.pPeerProxy, context, m_PartyList[m_iPartyIndex]->GetNetId(), true, true);
		SetInviting(false);

		ResetPartyButton();

		for (int i = 0 ; i < (int)m_PartyList.size(); i++)
		{
			if (i != m_iPartyIndex && m_PartyList[i])
				gGlobal.g_pCharacter->GetCharNetObject()->PartyInviteReply(gGlobal.pPeerProxy, context, m_PartyList[i]->GetNetId(), false, true);			
		}
		ClearPartyList();
	}			
	VMProtectEnd; VM_END;
}

void CMenuPartyProcess::InviteReject()
{
	VMProtectBegin; VM_START;
	if (m_PartyList[m_iPartyIndex])
	{
		RPCContext context;
		gGlobal.g_pCharacter->GetCharNetObject()->PartyInviteReply(gGlobal.pPeerProxy, context, m_PartyList[m_iPartyIndex]->GetNetId(), false, true);
		
		int i = 0;
		for(PartyList::iterator itr = m_PartyList.begin(); itr != m_PartyList.end(); ++itr)
		{
			if (i == m_iPartyIndex)
			{
				DeleteNetGroup(m_PartyList[m_iPartyIndex]);
				m_PartyList.erase(itr);
				break;
			}
			++i;
		}
		if ( !( (int)m_PartyList.size() > 0 ))
		{
			SetInviting(false);
			m_iPartyIndex = 0;

			ResetPartyButton();
		}
		else if (m_iPartyIndex >= (int)m_PartyList.size())
			m_iPartyIndex = (int)m_PartyList.size() - 1;
	}
	VMProtectEnd; VM_END;
}

void CMenuPartyProcess::RemoveParty(RPCNetID netId)
{
	int index = 0;

	RPCNetID noneId;

	//character jump map and clear all the invite
	if (netId == noneId)
	{
		ClearPartyList();
		SetInviting(false);
	}
	else
	{
		for (PartyList::iterator itr = m_PartyList.begin(); itr != m_PartyList.end(); ++itr)
		{
			if (netId == m_PartyList[index]->GetNetId())
			{
				CParty* temp = m_PartyList[index];
				DeleteNetGroup(temp);
				m_PartyList.erase(itr);
				break;
			}
			++index;
		}
	}

	if (m_PartyList.size() == 0)
	{
		SetInviting(false);
		UIButton* button = (UIButton*)gGlobal.pUIManager->GetUIItem("DownBParty");
		if (button)
		{
			button->SetLockState(false);
			if (gGlobal.g_pBattleControl->GetLeaveBattle())
			{
				button->SetNormalState();
				gGlobal.pUIManager->AddDirtyUIItem(button);
			}
		}
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseTeammateMenu", String());
	}
	else if (index == m_iPartyIndex)
	{
		if (m_iPartyIndex >= (int)m_PartyList.size())
			m_iPartyIndex = (int)m_PartyList.size() - 1;
		ShowPartyByIndex();
	}

}

void CMenuPartyProcess::AddRequestList(CCharacter* pTarget)
{
	if (pTarget&&m_bAutoAccept)
	{
		NetIDCharMap::iterator itr = m_ReqJoinList.find(pTarget->GetNetId());
		if (itr == m_ReqJoinList.end())
		{
			m_ReqJoinList.insert(std::make_pair(pTarget->GetNetId(), pTarget));
		}
	}

}

void CMenuPartyProcess::JoinRequest(CCharacter* pTarget)
{
	if (pTarget)
	{
		if(!m_bAutoAccept)
		{
			UInt16 nLevelDown = gGlobal.g_pTeamDescription->GetDescriptionLevelDown();
			UInt16 nLevelUp = gGlobal.g_pTeamDescription->GetDescriptionLevelUp();
			UInt16 nLevel = 0;
			nLevel = nLevelDown < nLevelUp ? nLevelDown : nLevelUp;
			nLevelUp = nLevelDown > nLevelUp ? nLevelDown : nLevelUp;
			nLevelDown = nLevel;
			nLevel = pTarget->GetLevel();

			if( nLevelDown == 0 && nLevelDown == 0)
				gGlobal.g_pCharacter->GetCharNetObject()->PartyJoinReply(gGlobal.pPeerProxy, RPCContext(), pTarget->GetNetId(), true, true);
			else if( nLevelDown > nLevelUp )
				gGlobal.g_pCharacter->GetCharNetObject()->PartyJoinReply(gGlobal.pPeerProxy, RPCContext(), pTarget->GetNetId(), true, true);
			else if(nLevelDown <= nLevelUp)
			{
				if( nLevelDown <= nLevel && nLevel<= nLevelUp)
					gGlobal.g_pCharacter->GetCharNetObject()->PartyJoinReply(gGlobal.pPeerProxy, RPCContext(), pTarget->GetNetId(), true, true);
				else
					gGlobal.g_pCharacter->GetCharNetObject()->PartyJoinReply(gGlobal.pPeerProxy, RPCContext(), pTarget->GetNetId(), false, true);
			}
		}
		else
		{
			AddRequestList(pTarget);
			SetJoining(true);
			UIButton* button = (UIButton*)gGlobal.pUIManager->GetUIItem("DownBParty");

			if (button && gGlobal.g_pBattleControl->GetLeaveBattle())
			{
				button->SetLockState(false);
				button->SetState(_ButtonNotice);
				button->SetLockState(true);
				gGlobal.pUIManager->AddDirtyUIItem(button);
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayAddTeamEffect", String());
			}

			UpdateApplicantMenu();
		}
	}
}

void CMenuPartyProcess::ResetApplicantPanel()
{
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("ApplyMembershipListPanel");
	if (panel == NULL)
		return;

	UIStaticText* characterName;
	UIStaticText* characterLevel;
	UIStaticText* characterClass;
	UIStaticText* characterFaction;
	UIStaticText* characterManor;
	UIImage*	  characterPopup;

	StringA name;
	StringA level;
	StringA cclass;
	StringA faction;
	StringA manor;
	StringA popup;

	for (Index i = C_INDEX(0); i < 10; ++i)
	{
		name.Format("ApplicantName%d", i+1);
		level.Format("ApplicantLevel%d", i+1);
		cclass.Format("ApplicantClass%d", i+1);
		faction.Format("ApplicantFaction%d", i+1);
		manor.Format("ApplicantManor%d", i+1);
		popup.Format("Applicant%d", i+1);

		characterName = gGlobal.pUIManager->FindUIStaticText(name.c_str());
		characterLevel = gGlobal.pUIManager->FindUIStaticText(level.c_str());
		characterClass = gGlobal.pUIManager->FindUIStaticText(cclass.c_str());
		characterFaction = gGlobal.pUIManager->FindUIStaticText(faction.c_str());
		characterManor = gGlobal.pUIManager->FindUIStaticText(manor.c_str());
		characterPopup = gGlobal.pUIManager->FindUIImage(popup.c_str());

		characterName->SetEnable(false);
		characterLevel->SetEnable(false);
		characterClass->SetEnable(false);
		characterFaction->SetEnable(false);
		characterManor->SetEnable(false);
		characterPopup->SetEnable(false);
	}
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ResetApplicantIndex", String());
}

void CMenuPartyProcess::UpdateApplicantMenu()
{
	if(!gGlobal.pUIManager)
		return;

	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->FindUIPanel("ApplyMembershipListPanel");
	if (panel == NULL)
		return;

	ResetApplicantPanel();

	if (GetRequestListSize() == 0)
	{
		SetJoining(false);
		ResetPartyButton();
	}

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("Applicant");
	if (scrollBar)
		scrollBar->SetNumCount((UInt32)m_ReqJoinList.size());

	UIStaticText* characterName;
	UIStaticText* characterLevel;
	UIStaticText* characterClass;
	UIStaticText* characterFaction;
	UIStaticText* characterManor;
	CCharacter * pChar;
	UIImage*	  characterPopup;

	StringA name;
	StringA level;
	StringA cclass;
	StringA faction;
	StringA manor;
	StringA popup;

	CGameStringTable* stringTable = gGlobal.GetStringTable();

	UInt noOfRow = 10;
	//if ((int)m_RequestList.size() <= 10)
	//	noOfRow = (int)m_RequestList.size();

	if (GetRequestListSize() <= 10)
		noOfRow = GetRequestListSize();

	for (Index i = C_INDEX(0); i < noOfRow; ++i)
	{
		NetIDCharMap::iterator itr = m_ReqJoinList.begin();

		UInt8 loop = 0;
		if (GetRequestListSize() > 10)
			loop = i + m_iRequestIndex;
		else
			loop = i;

		for (Index j = C_INDEX(0); j < loop; ++j)
			++itr;

		if (itr != m_ReqJoinList.end() )
			pChar = itr->second;
		else
			continue;

		name.Format("ApplicantName%d", i+1);
		level.Format("ApplicantLevel%d", i+1);
		cclass.Format("ApplicantClass%d", i+1);
		faction.Format("ApplicantFaction%d", i+1);
		manor.Format("ApplicantManor%d", i+1);
		popup.Format("Applicant%d", i+1);

		characterName =  gGlobal.pUIManager->FindUIStaticText(name.c_str());
		characterLevel = gGlobal.pUIManager->FindUIStaticText(level.c_str());
		characterClass = gGlobal.pUIManager->FindUIStaticText(cclass.c_str());
		characterFaction = gGlobal.pUIManager->FindUIStaticText(faction.c_str());
		characterManor = gGlobal.pUIManager->FindUIStaticText(manor.c_str());
		characterPopup = gGlobal.pUIManager->FindUIImage(popup.c_str());

		characterName->SetEnable(true);
		characterLevel->SetEnable(true);
		characterClass->SetEnable(true);
		characterFaction->SetEnable(true);
		characterManor->SetEnable(true);
		characterPopup->SetEnable(true);

		characterName->SetText(pChar->GetNickName().c_str());
		characterName->Redraw();

		char buf[256];
		sprintf(buf, "%d", pChar->GetLevel());
		characterLevel->SetText(String(buf).c_str());
		characterLevel->Redraw();

		String tempClass;
		String tempClassName;
		tempClassName.Format(_T("MSG_ACTOR_CLASS_%d"), pChar->GetSexClass());
		tempClass = stringTable->Get(tempClassName.c_str());

		characterClass->SetText(tempClass);
		characterClass->Redraw();

		String tempFaction;

		switch ( pChar->GetFaction())
		{
		case 1:	tempFaction = stringTable->Get(_T("MSG_FACTION_SIXFANDOOR"));	break;
		case 2:	tempFaction = stringTable->Get(_T("MSG_FACTION_TWELVEDOCK"));	break;
		case 3:	tempFaction = stringTable->Get(_T("MSG_FACTION_HOLY"));		break;
		case 4:	tempFaction = stringTable->Get(_T("MSG_FACTION_SILVERCASINO"));break;
		case 5:	tempFaction = stringTable->Get(_T("MSG_FACTION_TONGDOOR"));	break;
		case 6:	tempFaction = stringTable->Get(_T("MSG_FACTION_10THOUSAND"));	break;
		case 7:	tempFaction = stringTable->Get(_T("MSG_FACTION_GREENCLOTH"));	break;
		case 8:	tempFaction = stringTable->Get(_T("MSG_FACTION_GHOSTAREA"));	break;

		}
		characterFaction->SetText(tempFaction);
		characterFaction->Redraw();
	}
	gGlobal.pUIManager->AddDirtyUIItem(panel);
}

void CMenuPartyProcess::JoinAccept(int index)
{
	VMProtectBegin; VM_START;

	index += m_iRequestIndex;

	NetIDCharMap::iterator itr = m_ReqJoinList.begin();

	if (index >= 0 && index < GetRequestListSize())
	{
		for (int j = 0; j < index; ++j)
			++itr;
		if (itr != m_ReqJoinList.end())
		{
			gGlobal.g_pCharacter->GetCharNetObject()->PartyJoinReply(gGlobal.pPeerProxy, RPCContext(), itr->first, true, true);
			m_ReqJoinList.erase(itr);
		}

		if (GetRequestListSize() <= 10)
			m_iRequestIndex = 0;

		UpdateApplicantMenu();
	}

	VMProtectEnd; VM_END;
}

void CMenuPartyProcess::JoinReject(int index)
{
	VMProtectBegin; VM_START;

	index += m_iRequestIndex;
	NetIDCharMap::iterator itr = m_ReqJoinList.begin();
	
	if (index >= 0 && index < GetRequestListSize())
	{
		for (int j = 0; j < index; ++j)
			++itr;
		if (itr != m_ReqJoinList.end())
		{
			gGlobal.g_pCharacter->GetCharNetObject()->PartyJoinReply(gGlobal.pPeerProxy, RPCContext(), itr->first, false, true);
			m_ReqJoinList.erase(itr);
		}

		if (GetRequestListSize() <= 10)
			m_iRequestIndex = 0;

		UpdateApplicantMenu();	
	}

	VMProtectEnd; VM_END;
}

void CMenuPartyProcess::RemoveRequest(RPCNetID netId)
{
	int i = 0;

	RPCNetID noneId;

	if (noneId == netId)
		ClearAllRequest();
	else
	{
		NetIDCharMap::iterator itr = m_ReqJoinList.find(netId);
		if (itr != m_ReqJoinList.end())
			m_ReqJoinList.erase(itr);
	}

	if (m_iRequestIndex > 0)
	{
		if ( m_iRequestIndex + 10 > GetRequestListSize() )
			--m_iRequestIndex;		
	}

	UpdateApplicantMenu();

}

void CMenuPartyProcess::ClearAllRequest()
{
	for (NetIDCharMap::iterator itr = m_ReqJoinList.begin(); itr != m_ReqJoinList.end(); ++itr)
		gGlobal.g_pCharacter->GetCharNetObject()->PartyJoinReply(gGlobal.pPeerProxy, RPCContext(), itr->first, false, true);

	m_ReqJoinList.clear();

	m_iRequestIndex = 0;

	UpdateApplicantMenu();
}

void CMenuPartyProcess::ShowOtherParty(int next)
{
	if ((next > 1) || (next < 0))
		return;

	if (next == 1)
	{
		if (m_iPartyIndex < ((int)m_PartyList.size() - 1))
			++m_iPartyIndex;
		else 
			m_iPartyIndex = (int)m_PartyList.size() - 1;

	}
	else if (next == 0)
	{
		if (m_iPartyIndex > 0)
			--m_iPartyIndex;
	}

	ShowPartyByIndex(m_iPartyIndex);
}

void CMenuPartyProcess::ApplicantPanelScroll(int scroll)
{
	if (scroll == 1)
	{
		if (m_iRequestIndex > 0)
			--m_iRequestIndex;
		
		if (m_iRequestIndex == 0)
		{
			UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("Applicant");
			if (scrollBar)
				scrollBar->HomeScroll();
		}
		
	}
	else if (scroll == 0)
	{
		if (m_iRequestIndex < (GetRequestListSize() - 10) )
			++m_iRequestIndex;
		if (m_iRequestIndex == (GetRequestListSize() - 10) )
		{
			UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("Applicant");
			if (scrollBar)
				scrollBar->EndScroll();
		}
	}
	else if (scroll == 2)
	{
		UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("Applicant");

		if (scrollBar)
		{
			if (GetRequestListSize() <= 10)
				scrollBar->HomeScroll();
			else
			{
				UInt8 index = scrollBar->GetSelNum();
				if ((index + 9) < GetRequestListSize() )
				{
					if (m_iRequestIndex != index)
						m_iRequestIndex = index;
				}	
				else
				{
					if ((GetRequestListSize() - 10) != m_iRequestIndex)
						m_iRequestIndex = GetRequestListSize() - 10;
				}
			}
		}
	}

	UpdateApplicantMenu();
}

void CMenuPartyProcess::ClearAllInvite()
{
	RPCContext context;
	SetInviting(false);

	for (Index i = C_INDEX(0) ; i < m_PartyList.size(); i++)
		gGlobal.g_pCharacter->GetCharNetObject()->PartyInviteReply(gGlobal.pPeerProxy, context, m_PartyList[i]->GetNetId(), false, true);			
	
	ClearPartyList();
}

void CMenuPartyProcess::ShowAllPanel()
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(),"SetCloseAllPanelFlag", String(), false);

	gGlobal.pUIManager->RestoreAllPanelEnable();

	lua_getglobal(gGlobal.pUIManager->GetLuaState(), "g_CurrentShortCut"); 
	UIItem * item = NULL;
	if (lua_isnumber(gGlobal.pUIManager->GetLuaState(), -1)) 
	{
		Int CurrentShortCut = (Int)lua_tonumber(gGlobal.pUIManager->GetLuaState(), -1); 
		switch(CurrentShortCut)
		{
		case 0:
			item = gGlobal.pUIManager->GetUIItem("HShortCutPanel");
			item->SetEnable(true);
			break;
		case 1:
			item = (UIItem*)gGlobal.pUIManager->GetUIItem("VShortCutPanel");
			item->SetEnable(true);
			break;
		case 2:
			item = (UIItem*)gGlobal.pUIManager->GetUIItem("HideShortCutPanel");
			item->SetEnable(true);
			break;
		}
	}
}

void CMenuPartyProcess::HideAllPanel()
{
	gGlobal.pUIManager->SetAllPanelDisable();
}

void CMenuPartyProcess::ShowPartyButton()
{
	UIButton* button = gGlobal.pUIManager->FindUIButton("DownBParty");
	if (button)
	{
		button->SetLockState(false);
		if (m_PartyList.size() != 0 || GetRequestListSize() != 0)
		{
			button->SetState(_ButtonNotice);
			button->SetLockState(true);
		}
		else
			button->SetNormalState();
		gGlobal.pUIManager->AddDirtyUIItem(button);
	}
}

void CMenuPartyProcess::ResetPartyButton()
{
	UIButton* button = gGlobal.pUIManager->FindUIButton("DownBParty");
	
	if (button && gGlobal.g_pBattleControl->GetLeaveBattle())
	{
		button->SetLockState(false);
		button->SetNormalState();
		gGlobal.pUIManager->AddDirtyUIItem(button);
	}		
}

void CMenuPartyProcess::ApplicantPrivateCall(int index)
{
	index += m_iRequestIndex;
	NetIDCharMap::iterator itr = m_ReqJoinList.begin();
	
	if (index >= 0 && index < GetRequestListSize())
	{
		for (int j = 0; j < index; ++j)
			++itr;
	
		UIEditBox * pEditBox = gGlobal.pUIManager->GetUIThing<UIEditBox *>("EditLine");
		if (pEditBox && itr != m_ReqJoinList.end())
		{
			String Temp;
			Temp.Format( _T("/p %s "), itr->second->GetNickName().c_str() );
			pEditBox->SetText(Temp);
			pEditBox->SetFocus(true);
			pEditBox->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
		}
	}
		
}

void CMenuPartyProcess::TeammatePrivateCall(int index)
{
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	if (pParty && index >= 0 && index < gGlobal.g_vctPartyMemberPointer.size() )
	{
		CCharacter * pchar = CastNetObject<CCharacter>(gGlobal.g_vctPartyMemberPointer[index]);
		if (pchar)
		{
			UIEditBox * pEditBox = gGlobal.pUIManager->GetUIThing<UIEditBox *>("EditLine");
			if (pEditBox)
			{
				String Temp;
				Temp.Format( _T("/p %s "), pchar->GetNickName().c_str() );
				pEditBox->SetText(Temp);
				pEditBox->SetFocus(true);
				pEditBox->Redraw();
				gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
			}
		}
		else
		{
			Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.find(index);
			if (it != gGlobal.g_mapPartyChar.end()) 
			{
				CPartyCharInfoClient* pCpcic = it->second;
				if (NULL != pCpcic) 
				{
					UIEditBox * pEditBox = gGlobal.pUIManager->GetUIThing<UIEditBox *>("EditLine");
					if (pEditBox)
					{
						String Temp;
						Temp.Format( _T("/p %s "), pCpcic->GetStrNickName().c_str() );
						pEditBox->SetText(Temp);
						pEditBox->SetFocus(true);
						pEditBox->Redraw();
						gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
					}
				}
			}
		}
	}
}

void CMenuPartyProcess::TeammateKick(int index)
{
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	if (pParty && index >= 0 && index < gGlobal.g_vctPartyMemberPointer.size() )
	{
		CCharacter * pchar = CastNetObject<CCharacter>(gGlobal.g_vctPartyMemberPointer[index]);

		if (pchar)
			pParty->Kick(gGlobal.pPeerProxy, gGlobal.g_context, pchar->GetNetId());
		else
		{
			Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.find(index);
			if (it != gGlobal.g_mapPartyChar.end()) 
			{
				CPartyCharInfoClient* pCpcic = it->second;
				if (NULL != pCpcic) 
				{
					pParty->Kick(gGlobal.pPeerProxy, gGlobal.g_context, pCpcic->GetStNetId());
				}
			}
		}
	}
}


void CMenuPartyProcess::TeammatePromote(int index)
{
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	if (pParty && index >= 0 && index < gGlobal.g_vctPartyMemberPointer.size() )
	{
		CCharacter * pchar = CastNetObject<CCharacter>(gGlobal.g_vctPartyMemberPointer[index]);

		if (pchar)
			pParty->Promote(gGlobal.pPeerProxy, gGlobal.g_context, pchar->GetNetId());
		else
		{
			Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.find(index);
			if (it != gGlobal.g_mapPartyChar.end()) 
			{
				CPartyCharInfoClient* pCpcic = it->second;
				if (NULL != pCpcic) 
				{
					pParty->Promote(gGlobal.pPeerProxy, gGlobal.g_context, pCpcic->GetStNetId());
				}
			}
		}
	}
}

bool CMenuPartyProcess::IsInInviteList(UInt32 char_id)
{
	for (Index i = C_INDEX(0); i < m_PartyList.size(); ++i)
	{
		if (m_PartyList[i])
		{
			for(Index j = C_INDEX(0); j < m_PartyList[i]->GetCount(); ++j)
			{
				if( m_PartyList[i]->GetMember(j) )
				{
					if (m_PartyList[i]->GetMember(j)->GetChar_id() == char_id)
					{
						if (j == 0)
							return true;
						else
						{
							m_PartyList[i]->Remove(m_PartyList[i]->GetMember(j));
							UpdatePartyPanel(m_PartyList[i]);
							return false;
						}
					}
				}else
				{
					TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("CMenuPartyProcess::IsInInviteList Warning (1) [Member NULL] PartyListSize=%d, PartySize=%d, ListIndex=%d, MemberIndex=%d"), 
						(UInt)m_PartyList.size(), m_PartyList[i]->GetCount(), i, j
						);
				}
			}
		}
	}
	return false;
}

bool CMenuPartyProcess::IsInRequestList(RPCNetID netID)
{
	NetIDCharMap::iterator itr = m_ReqJoinList.find(netID);
	if (itr != m_ReqJoinList.end())
		return true;
	return false;
}

void CMenuPartyProcess::JoinRejectByChar(RPCNetID netID)
{
	NetIDCharMap::iterator itr = m_ReqJoinList.find(netID);

	if (itr != m_ReqJoinList.end())
	{
		gGlobal.g_pCharacter->GetCharNetObject()->PartyJoinReply(gGlobal.pPeerProxy, RPCContext(), itr->first, false, false);
		m_ReqJoinList.erase(itr);
		if (GetRequestListSize() <= 10)
			m_iRequestIndex = 0;
	
		UpdateApplicantMenu();	
	}
}

void CMenuPartyProcess::InviteRejectByCharID(UInt32 char_id)
{
	for (Index i = C_INDEX(0); i < m_PartyList.size(); ++i)
	{
		if (m_PartyList[i]->GetMember(0) && m_PartyList[i]->GetMember(0)->GetChar_id() == char_id)
		{
			RPCContext context;
			gGlobal.g_pCharacter->GetCharNetObject()->PartyInviteReply(gGlobal.pPeerProxy, context, m_PartyList[i]->GetNetId(), false, false);
			
			int j = 0;
			for(PartyList::iterator itr = m_PartyList.begin(); itr != m_PartyList.end(); ++itr)
			{
				if (i == j)
				{
					DeleteNetGroup(m_PartyList[i]);
					m_PartyList.erase(itr);
					break;
				}
				++j;
			}
			if ( !( (int)m_PartyList.size() > 0 ))
			{
				SetInviting(false);
				m_iPartyIndex = 0;

				ResetPartyButton();
			}
			else if (m_iPartyIndex >= (int)m_PartyList.size())
				m_iPartyIndex = (int)m_PartyList.size() - 1;
			ShowPartyByIndex(m_iPartyIndex);
			break;
		}
	}
}


void CMenuPartyProcess::InsertCharToOtherParty(RPCNetID partyID, CCharacter* pChar)
{
	for (Index i = C_INDEX(0); i < m_PartyList.size(); ++i)
	{
		if (m_PartyList[i] && m_PartyList[i]->GetNetId() == partyID)
		{
			for(UInt8 j = 0; j < m_PartyList[i]->GetCount(); ++j)
			{				
				if (m_PartyList[i]->GetMember(j) && m_PartyList[i]->GetMember(j)->GetChar_id() == pChar->GetChar_id())
					return;
			}
			m_PartyList[i]->Add(pChar);
			ShowPartyByIndex();
			break;
		}
	}
}


CMenuPartyProcess::~CMenuPartyProcess()
{
	ClearPartyList();	
	
	m_ReqJoinList.clear();
	m_iRequestIndex = 0;
	SetJoining(false);
	SetInviting(false);
}

void CMenuPartyProcess::ShowTeammatePopUp(UInt8 index)
{
	UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("MenuTeammatePopup");
	UIPanelEx* pPanel = gGlobal.pUIManager->FindUIPanelEx("MenuTeammatePopupPanel");
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	if (pParty && pPopUpMenu && pPanel && gGlobal.g_vctPartyMemberPointer.size() > index)
	{
		pPopUpMenu->ClearData();
		CCharacter* pChar = CastNetObject<CCharacter>(gGlobal.g_vctPartyMemberPointer[index]);
		if (pChar)
		{
			if (pChar->GetChar_id() == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id())
			{
				pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE4")), 0, "");
				pPopUpMenu->Redraw();
				pPanel->ChangeCount(1);
			}
			else
			{
				UInt8 selectIndex = 0;
				pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE1")), selectIndex, "");
				++selectIndex;

				if (pParty->IsLeader())
				{
					pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE2")), selectIndex, "");
					++selectIndex;
					pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE3")), selectIndex, "");
					++selectIndex;
				}

				pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE5")), selectIndex, "");
				++selectIndex;
				pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE6")), selectIndex, "");
				++selectIndex;
				pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE7")), selectIndex, "");
				++selectIndex;
				pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE8")), selectIndex, "");
				++selectIndex;
				
				if (pParty->IsLeader() && gGlobal.g_vctPartyMemberPointer.size() > 2) {
					pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE9")),selectIndex,"");
					++selectIndex;
				}

				pPopUpMenu->Redraw();
				pPanel->ChangeCount(selectIndex);
			}
			gGlobal.pUIManager->AddDirtyUIItem(pPanel);
		}
		else
		{
			Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.find(index);
			if (it != gGlobal.g_mapPartyChar.end()) 
			{
				CPartyCharInfoClient* pCpcic = it->second;
				if (NULL != pCpcic) 
				{
					if (pCpcic->GetU32CharId() == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id())
					{
						pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE4")), 0, "");
						pPopUpMenu->Redraw();
						pPanel->ChangeCount(1);
					}
					else
					{
						UInt8 selectIndex = 0;
						pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE1")), selectIndex, "");
						++selectIndex;

						if (pParty->IsLeader())
						{
							pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE2")), selectIndex, "");
							++selectIndex;
							pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE3")), selectIndex, "");
							++selectIndex;
						}

						if (pParty->IsLeader() && gGlobal.g_vctPartyMemberPointer.size() > 2) {
							pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE9")),selectIndex,"");
							++selectIndex;
						}

						pPopUpMenu->Redraw();
						pPanel->ChangeCount(selectIndex);
					}
					gGlobal.pUIManager->AddDirtyUIItem(pPanel);
				}
			}
		}
	}
	m_iPreChangeIndex = UCHAR_MAX;
}


void CMenuPartyProcess::AutoAcceptCheckBox(bool on)
{
	if(gGlobal.g_pParty->IsLeader())
		m_bAutoAccept = on;
	else
		m_bAutoAccept = false;
}

void CMenuPartyProcess::SelectTeammatePopUp(UInt8 popSelect, UInt8 index)
{
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	if (pParty && gGlobal.g_vctPartyMemberPointer.size() > index)
	{
		CCharacter* pChar = pParty->GetMember(index);
		if (pChar)
		{
			if (pChar->GetChar_id() == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id())
			{
				if (popSelect == 0)
				{
					ClearAllRequest();
					ClearAllInvite();
					pParty->Leave(gGlobal.pPeerProxy, gGlobal.g_context);
				}
			}
			else if (pParty->IsLeader())
			{
				switch(popSelect)
				{
				case 0:	TeammatePrivateCall(index);		break;
				case 1: TeammateKick(index);			break;
				case 2: TeammatePromote(index);			break;
				case 3:
					{
						if (pChar->GetEnableBlockGiven() )
							gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_BLOCK_GIVEN")), CHANNELTYPE_SYSTEM);
						else if (pChar->GetAction() != CCharacter::ST_BATTLE)
							gGlobal.g_pGivenProcess->OpenGivenPanel(pChar->GetNickName(), pChar->GetChar_id(), pChar->GetNetId() );
						else
							gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_NOT_FRIEND")), CHANNELTYPE_SYSTEM);
					}
					break;
				case 4:
					{
						if (pChar->GetEnableBlockTrading() )
							gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_BLOCK_TRADING")), CHANNELTYPE_SYSTEM);
						else
						{
							gGlobal.g_pCharacter->GetCharNetObject()->AskForTrading(gGlobal.pPeerProxy, RPCContext(), pChar->GetNetId());
							if (gGlobal.g_pTradingProcess)
								gGlobal.g_pTradingProcess->SetTargetNetID(pChar->GetNetId());

							if (gGlobal.g_pMenuFriendProcess)
							{
								CMessenger* messenger = gGlobal.g_pMenuFriendProcess->FindMessenger(pChar->GetChar_id() );
								if (messenger == NULL || messenger->GetFriend_relation() == 0)
								{
									gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_NOT_FRIEND_REQ")));
								}
							}
						}
					}
					break;
				case 5:
					{
						CCharacterControl* pCharCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pChar->GetChar_id());
						if (pCharCtrl)
						{
							if (pCharCtrl->GetCharNetObject() && gGlobal.g_pCharacter->GetCharNetObject() && 
								pCharCtrl != gGlobal.g_pCharacter)
							{
								if (pCharCtrl->GetCharNetObject()->GetEnableBlockMakeFriend() )
									gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_BLOCK_FRIEND")), CHANNELTYPE_SYSTEM);
								else
								{
									gGlobal.g_pMenuFriendProcess->m_TargetCharID = pCharCtrl->GetMapActorID();
									gGlobal.g_pMenuFriendProcess->m_TargetCharName = pCharCtrl->GetCharName();
									gGlobal.g_pMenuFriendProcess->AddFriend(FRIEND_GROUP_ONE);
								}
							}
						}
					}
					break;
				case 6:
					{
						CCharacterControl* pCharCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pChar->GetChar_id());
						if (pCharCtrl)
						{
							gGlobal.g_MenuTargetProcess->SetFriendDetail(pCharCtrl);
							gGlobal.g_MenuTargetProcess->ShowDetail();
						}
					}
					break;
				case 7:
					m_iPreChangeIndex = index;
					break;
				}
			}
			else
			{
				switch(popSelect)
				{
				case 0:	TeammatePrivateCall(index);		break;
				case 1:
					{
						if (pChar->GetEnableBlockGiven() )
							gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_BLOCK_GIVEN")), CHANNELTYPE_SYSTEM);
						else if (pChar->GetAction() != CCharacter::ST_BATTLE)
							gGlobal.g_pGivenProcess->OpenGivenPanel(pChar->GetNickName(), pChar->GetChar_id(), pChar->GetNetId() );
						else
							gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_NOT_FRIEND")), CHANNELTYPE_SYSTEM);
					}
					break;
				case 2:
					{
						if (pChar->GetEnableBlockTrading() )
							gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_BLOCK_TRADING")), CHANNELTYPE_SYSTEM);
						else
						{
							gGlobal.g_pCharacter->GetCharNetObject()->AskForTrading(gGlobal.pPeerProxy, RPCContext(), pChar->GetNetId());
							if (gGlobal.g_pTradingProcess)
								gGlobal.g_pTradingProcess->SetTargetNetID(pChar->GetNetId());

							if (gGlobal.g_pMenuFriendProcess)
							{
								CMessenger* messenger = gGlobal.g_pMenuFriendProcess->FindMessenger(pChar->GetChar_id() );
								if (messenger == NULL || messenger->GetFriend_relation() == 0 )
								{
									gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_NOT_FRIEND_REQ")));
								}
							}
						}
					}
					break;
				case 3:
					{
						CCharacterControl* pCharCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pChar->GetChar_id());
						if (pCharCtrl)
						{
							if (pCharCtrl->GetCharNetObject() && gGlobal.g_pCharacter->GetCharNetObject() && 
								pCharCtrl != gGlobal.g_pCharacter)
							{
								if (pCharCtrl->GetCharNetObject()->GetEnableBlockMakeFriend() )
									gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_BLOCK_FRIEND")), CHANNELTYPE_SYSTEM);
								else
								{
									gGlobal.g_pMenuFriendProcess->m_TargetCharID = pCharCtrl->GetMapActorID();
									gGlobal.g_pMenuFriendProcess->m_TargetCharName = pCharCtrl->GetCharName();
									gGlobal.g_pMenuFriendProcess->AddFriend(FRIEND_GROUP_ONE);
								}
							}
						}
					}
					break;
				case 4:
					{
						CCharacterControl* pCharCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pChar->GetChar_id());
						if (pCharCtrl)
						{
							gGlobal.g_MenuTargetProcess->SetFriendDetail(pCharCtrl);
							gGlobal.g_MenuTargetProcess->ShowDetail();
						}
					}
					break;
				}
			}
		}
		else
		{
			Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.find(index);
			if (it != gGlobal.g_mapPartyChar.end()) 
			{
				CPartyCharInfoClient* pCpcic = it->second;
				if (NULL != pCpcic) 
				{
					if (pParty->IsLeader())
					{
						switch(popSelect)
						{
						case 0:	TeammatePrivateCall(index);		break;
						case 1: TeammateKick(index);			break;
						case 2: TeammatePromote(index);			break;
						case 3:
							m_iPreChangeIndex = index;
							break;
						}
					}
					else if (popSelect == 0)
						TeammatePrivateCall(index);
				}
			}
		}
	}
}

void CMenuPartyProcess::SetChangePos(UInt8 iIndex)
{
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	if (pParty && gGlobal.g_vctPartyMemberPointer.size() > iIndex)
	{
		if (m_iPreChangeIndex != UCHAR_MAX 
			&& gGlobal.g_vctPartyMemberPointer.size() > m_iPreChangeIndex
			&& m_iPreChangeIndex != iIndex) {
			if (NULL != gGlobal.g_pCharacter) {
				if (NULL != gGlobal.g_pCharacter->GetCharNetObject()) {
					gGlobal.g_pCharacter->GetCharNetObject()->SetChangePos(gGlobal.pPeerProxy,gGlobal.g_context,m_iPreChangeIndex,iIndex);
				}
			}
		}
	}
	m_iPreChangeIndex = UCHAR_MAX;
}

void CMenuPartyProcess::ChangePartyDesc()
{
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	if (pParty)
	{
		pParty->ChangeDesc(gGlobal.pPeerProxy, gGlobal.g_context, _T("test Desc"));
		UIStaticText* pTeamDescription = NULL;
		pTeamDescription = gGlobal.pUIManager->FindUIStaticText("TeamDescriptionText");
		if(!pTeamDescription)
			return;

		String strDesc(pTeamDescription->GetText());

		gGlobal.g_pParty->ChangeDesc(gGlobal.pPeerProxy, gGlobal.g_context, strDesc);
	}
}
