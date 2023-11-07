//-- Common
#include "Common.h"
#include "Common/CallLuaFunc.h"
#include "QGL.h"
//-- Self
#include "TeamDescription.h"
#include "QGL/Global.h"
#include "LuaPush.h"
//-- Library
#include "Resource/QGLCSVTReader.h"
#include "QGL/MenuPartyProcess.h"
#include "QGL/CharacterControl.h"
//#include "Proxy/Service/ObjectService.h"

CTeamDescription::CTeamDescription()
{
	m_iDescriptionNum = 0;
	m_nDescriptionLevelDown = 0;
	m_nDescriptionLevelUp = 0;
	m_strInput.clear();
}

CTeamDescription::~CTeamDescription()
{
	//TCHAR     szBuffer[100];
	//MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,luafilename,100,szBuffer,100);
	//OutputDebugString(szBuffer);
	//TCHAR szBuffer[512];  
	//_snwprintf(szBuffer,sizeof(szBuffer),L"%d",i);
	//OutputDebugString(szBuffer);
}

void CTeamDescription::ClearTeamDescriptionData()
{
	m_iDescriptionNum = 0;
	m_nDescriptionLevelDown = 0;
	m_nDescriptionLevelUp = 0;
	m_strInput.clear();
}


void CTeamDescription::CloseTeamDescription()
{
	gGlobal.g_pCharacter->GetCharNetObject()->PartyJoinLevelControl(gGlobal.pPeerProxy,
		gGlobal.g_context,gGlobal.g_pCharacter->GetCharNetObject()->GetNetId(),
		m_nDescriptionLevelDown,m_nDescriptionLevelUp);
}

void CTeamDescription::ShowTeamDescription()
{
	UICheckBox * pFieldBurningCheckBox = gGlobal.pUIManager->FindUICheckBox("TeamDescriptionFieldBurning");
	if(!pFieldBurningCheckBox)
		return;

	UICheckBox * pToPunishEvilTaskCheckBox = gGlobal.pUIManager->FindUICheckBox("TeamDescriptionToPunishEvilTask");
	if(!pToPunishEvilTaskCheckBox)
		return;

	UICheckBox * pEliminateGreenTaskCheckBox = gGlobal.pUIManager->FindUICheckBox("TeamDescriptionEliminateGreenTask");
	if(!pEliminateGreenTaskCheckBox)
		return;

	UICheckBox * pVillaTaskCheckBox = gGlobal.pUIManager->FindUICheckBox("TeamDescriptionVillaTask");
	if(!pVillaTaskCheckBox)
		return;

	UICheckBox * pDuplicateCheckBox = gGlobal.pUIManager->FindUICheckBox("TeamDescriptionDuplicate");
	if(!pDuplicateCheckBox)
		return;

	UICheckBox * pEventsCheckBox = gGlobal.pUIManager->FindUICheckBox("TeamDescriptionEvents");
	if(!pEventsCheckBox)
		return;

	
	pEventsCheckBox->SetState(true);
	pToPunishEvilTaskCheckBox->SetState(true);
	pEliminateGreenTaskCheckBox->SetState(true);
	pVillaTaskCheckBox->SetState(true);
	pDuplicateCheckBox->SetState(true);
	pEventsCheckBox->SetState(true);

	UIEditBox* pDescriptionInput = gGlobal.pUIManager->FindUIEditBox("TeamDescriptionInput");
	if (!pDescriptionInput)
		return;

	TCHAR szBuffer[128];

	UIEditBox* pDescriptionLevelDown = gGlobal.pUIManager->FindUIEditBox("TeamDescriptionLevelDown");
	if (!pDescriptionLevelDown)
		return;

	_snwprintf(szBuffer,sizeof(szBuffer),L"%d",m_nDescriptionLevelDown);
	pDescriptionLevelDown->SetText(szBuffer);

	UIEditBox* pDescriptionLevelUp = gGlobal.pUIManager->FindUIEditBox("TeamDescriptionLevelUp");
	if (!pDescriptionLevelUp)
		return;

	_snwprintf(szBuffer,sizeof(szBuffer),L"%d",m_nDescriptionLevelUp);
	pDescriptionLevelUp->SetText(szBuffer);

	switch(m_iDescriptionNum)
	{
	case 1:
		pDescriptionInput->SetText(L"野外烧双");
		//pEventsCheckBox->SetState(false);
		break;
	case 2:
		pDescriptionInput->SetText(L"诛邪任务");
		//pToPunishEvilTaskCheckBox->SetState(false);
		break;
	case 3:
		pDescriptionInput->SetText(L"灭青任务");
		//pEliminateGreenTaskCheckBox->SetState(false);
		break;
	case 4:
		pDescriptionInput->SetText(L"山庄任务");
		//pVillaTaskCheckBox->SetState(false);
		break;
	case 5:
		pDescriptionInput->SetText(L"副本");
		//pDuplicateCheckBox->SetState(false);
		break;
	case 6:
		pDescriptionInput->SetText(L"活动");
		//pEventsCheckBox->SetState(false);
		break;
	default:
		pDescriptionInput->SetText(m_strInput.c_str());
		break;
	}

	pDescriptionInput->Redraw();
	pDescriptionLevelDown->Redraw();
	pDescriptionLevelUp->Redraw();

	m_iDescriptionNum = 0;
}


void CTeamDescription::SetTeamDescriptionData()
{
	UIStaticText* pTeamDescription = NULL;
	pTeamDescription = gGlobal.pUIManager->FindUIStaticText("TeamDescriptionText");
	if(!pTeamDescription)
		return;

	if(m_iDescriptionNum == 0 && m_strInput.empty())
	{
		return;
	}
	else if( m_iDescriptionNum!= 0 )
	{
		switch(m_iDescriptionNum)
		{
		case 1:
			pTeamDescription->SetText(L"野外烧双");
			break;
		case 2:
			pTeamDescription->SetText(L"诛邪任务");
			break;
		case 3:
			pTeamDescription->SetText(L"灭青任务");
			break;
		case 4:
			pTeamDescription->SetText(L"山庄任务");
			break;
		case 5:
			pTeamDescription->SetText(L"副本");
			break;
		case 6:
			pTeamDescription->SetText(L"活动");
			break;
		}
		pTeamDescription->Redraw();
		pTeamDescription->SetEnable(true);

		gGlobal.g_pPartyProcess->ChangePartyDesc();
	}
	else if(!m_strInput.empty())
	{
		pTeamDescription->SetText(m_strInput.c_str());
		pTeamDescription->Redraw();
		pTeamDescription->SetEnable(true);

		gGlobal.g_pPartyProcess->ChangePartyDesc();
	}
}

void CTeamDescription::TeamDescriptionTabControl(int index)
{
	m_iDescriptionNum = index;

	//if(index == 1)
	//{
	//	UInt32 point,ccalss,faction,level ;
	//	point = 2;
	//	ccalss = 3;
	//	faction = 4;
	//	level = 5;

	//	time_t now = ::time(NULL);
	//	struct tm* ptime = localtime(&now);
	//	ptime->tm_hour = 3;
	//	ptime->tm_min = 15;
	//	ptime->tm_sec = 0;
	//	time_t time = mktime(ptime);

	//	String name = "wangkui";
	//	//ObjectService_Proxy::CreateWeaponsPoint(gGlobal.pPeerProxy, RPCContext(),gGlobal.g_pCharacter->GetChar_id(),point,name,ccalss,faction,level,time);

	//	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
	//	{
	//		gGlobal.g_pCharacter->GetCharNetObject()->CreateWeaponsPointData(gGlobal.pPeerProxy,gGlobal.g_context,gGlobal.g_pCharacter->GetChar_id(),point,name,ccalss,faction,level,time);
	//	}
	//}
	//else if(index == 2)
	//{
	//	//ObjectService_Proxy::GetWeaponsPoint(gGlobal.pPeerProxy, RPCContext(),gGlobal.g_pCharacter->GetChar_id());
	//	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
	//	{
	//		gGlobal.g_pCharacter->GetCharNetObject()->GetWeaponsPointData(gGlobal.pPeerProxy,gGlobal.g_context,gGlobal.g_pCharacter->GetChar_id());
	//	}
	//}
	//else if(index == 3)
	//{
	//	OutputDebugString(L"CTeamDescription::index == 3");
	//}
}

void CTeamDescription::TeamDescriptionInput(PCSTRINGW text)
{
	m_strInput = text;
	if(m_strInput.empty())
		return;
}

void CTeamDescription::TeamDescriptionLevelDown(int nNum)
{
	m_nDescriptionLevelDown = nNum;
}

void CTeamDescription::TeamDescriptionLevelUp(int nNum)
{
	m_nDescriptionLevelUp = nNum;
}