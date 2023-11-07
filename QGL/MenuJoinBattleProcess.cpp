//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuJoinBattleProcess.h"
//-- Library
#include "QGL/Global.h"
#include "QGL/Configure.h"
#include "QGL/CharacterControl.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "Proxy/Logic/Party.h"
#include "QGL/BattleControl.h"

CMenuJoinBattleProcess::CMenuJoinBattleProcess()
{
	m_StartIndex = 0;
}

CMenuJoinBattleProcess::~CMenuJoinBattleProcess()
{
}

VOID CMenuJoinBattleProcess::UpdateBattleJoinMenu()
{
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->FindUIPanel("JoinBattleListPanel");
	if (panel == NULL)
		return;

	CGameStringTable* stringTable = gGlobal.GetStringTable();

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

	int tempStartIndex = m_StartIndex;
	for (int i = 0; i < 10; ++i, ++tempStartIndex)
	{
		name.Format("JoinBattleApplicantName%d", i+1);
		level.Format("JoinBattleApplicantLevel%d", i+1);
		cclass.Format("JoinBattleApplicantClass%d", i+1);
		faction.Format("JoinBattleApplicantFaction%d", i+1);
		manor.Format("JoinBattleApplicantManor%d", i+1);
		popup.Format("JoinBattleApplicant%d", i+1);

		characterName = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(name.c_str());
		characterLevel = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(level.c_str());
		characterClass = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(cclass.c_str());
		characterFaction = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(faction.c_str());
		characterManor = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(manor.c_str());
		characterPopup = (UIImage*) gGlobal.pUIManager->FindUIImage(popup.c_str());

		if (tempStartIndex < (Int)m_RequestList.size())
		{
			characterName->SetEnable(true);
			characterLevel->SetEnable(true);
			characterClass->SetEnable(true);
			characterFaction->SetEnable(true);
			characterManor->SetEnable(true);
			characterPopup->SetEnable(true);

			characterName->SetText(m_RequestList[i]->GetNickName().c_str());
			characterName->Redraw();

			char buf[256];
			sprintf(buf, "%d", m_RequestList[i]->GetLevel());
			characterLevel->SetText(String(buf).c_str());
			characterLevel->Redraw();

			String tempClass;
			String tempClassName;
			tempClassName.Format(_T("MSG_ACTOR_CLASS_%d"), m_RequestList[i]->GetSexClass());
			tempClass = stringTable->Get(tempClassName.c_str());

			/*switch ()
			{
			case 1:
				tempClass = stringTable->Get("MSG_ACTOR_CLASS_PALADIN");
				break;
			case 2:
				tempClass = stringTable->Get("MSG_ACTOR_CLASS_THIEF");
				break;
			case 3:
				tempClass = stringTable->Get("MSG_ACTOR_CLASS_DOCTOR");
				break;
			case 4:
				tempClass = stringTable->Get("MSG_ACTOR_CLASS_ASSASSIN");
				break;
			}*/

			characterClass->SetText(tempClass);
			characterClass->Redraw();

			String tempRing;

			switch ( m_RequestList[i]->GetFaction())
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

		}
		else
		{
			characterName->SetEnable(false);
			characterLevel->SetEnable(false);
			characterClass->SetEnable(false);
			characterFaction->SetEnable(false);
			characterManor->SetEnable(false);
			characterPopup->SetEnable(false);
		}
	}

	gGlobal.pUIManager->AddDirtyUIItem(panel);
}

VOID CMenuJoinBattleProcess::SendPrivateMessageTo(Int Index)
{
	if (m_StartIndex + Index < m_RequestList.size())
	{
		String Msg;
		Msg.Format(_T("%s%s %s "), gConfigure.strMsgDel.c_str(), gConfigure.strChannelMsgCom[CHANNELTYPE_PRIVATE].c_str(), 
			m_RequestList[m_StartIndex + Index]->GetNickName().c_str());
		UIEditBox * pBox = gGlobal.pUIManager->FindUIEditBox("EditLine");
		pBox->SetText(Msg);
	}
}

VOID CMenuJoinBattleProcess::AcceptJoin(Int Index)
{
	if (m_StartIndex + Index < m_RequestList.size())
	{
		gGlobal.g_pCharacter->GetCharNetObject()->JoinBattle(gGlobal.pPeerProxy, gGlobal.g_context, m_RequestList[m_StartIndex + Index]->GetChar_id());
	}
}

VOID CMenuJoinBattleProcess::RejectJoin(Int Index)
{
	if (m_StartIndex + Index < m_RequestList.size())
	{
		gGlobal.g_pCharacter->GetCharNetObject()->RejectHelper(gGlobal.pPeerProxy, gGlobal.g_context, m_RequestList[m_StartIndex + Index]->GetChar_id());
	}
}

VOID CMenuJoinBattleProcess::ScrollUp()
{
	if (m_StartIndex > 0)
		m_StartIndex --;
	UpdateBattleJoinMenu();
}
VOID CMenuJoinBattleProcess::ScrollDown()
{
	if (m_StartIndex + m_RequestList.size() > 10)
		m_StartIndex ++;

	UpdateBattleJoinMenu();

}
VOID CMenuJoinBattleProcess::ClearList()
{
	m_RequestList.clear();
	UpdateBattleJoinMenu();
}

VOID CMenuJoinBattleProcess::AddCharacterToList(CCharacter * pChar)
{
	if (pChar == NULL)
		return;

	for (TargetList::iterator it = m_RequestList.begin(); it != m_RequestList.end(); it++)
	{
		if ((*it) == pChar)
		{
			return;
		}
	}

	m_RequestList.push_back(pChar);
	UpdateBattleJoinMenu();
	//highlight party button
	UIButton *pItem = gGlobal.pUIManager->FindUIButton("DownBParty");
	if (pItem)
	{
		pItem->SetMouseOverState();
		gGlobal.pUIManager->AddDirtyUIItem(pItem);
	}

}

VOID CMenuJoinBattleProcess::RemoveCharacterFromList(CCharacter * pChar)
{
	if (pChar == NULL)
		return;

	for (TargetList::iterator it = m_RequestList.begin(); it != m_RequestList.end(); it++)
	{
		if ((*it) == pChar)
		{
			m_RequestList.erase(it);
			UpdateBattleJoinMenu();
			break;
		}
	}
}