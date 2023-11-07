//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuAutoWalkProcess.h"
//-- Library
#include "QGLApplication.h"
#include "Global.h"
#include "QGL/CharacterControl.h"
#include "QGL/Math.h"
#include "MiddleMap.h"
#include "Common/PathFinder.h"
#include "Proxy/Logic/Party.h"


#define HOUR_TO_SECOND 3600
#define MINUTE_TO_SECOND 60

CMenuAutoWalkProcess::CMenuAutoWalkProcess()
:m_bEnable(false),m_hitboss(true),m_startAuto(false)
{
	m_overhour = 0;
	m_overminute = 0;
	m_oversecond = 0;
	m_curMapId = 0;
	m_startTime = 0;
	m_AutoTime = 0;
	m_showAutoBox = false;
}

void CMenuAutoWalkProcess::SetAutoTime(UInt32 lastTime)
{
	m_AutoTime = lastTime*MINUTE_TO_SECOND;
}


void CMenuAutoWalkProcess::OnUpdate(UInt32 uTime)
{
	UIStaticText *pStaticText = gGlobal.pUIManager->FindUIStaticText("AutoWalkTime");
	UIPanel* pAutoBox = gGlobal.pUIManager->FindUIPanel("AutoWalkPanel");
	if (pStaticText && m_bEnable)
	{
		GetOverTime();
		StringW AllTime = _T("");
		if (m_overhour < 1)
			AllTime.Format(gGlobal.GetStringTable()->Get(_T("MSG_AUTOWALK_TIMEOVER_NOT_HOUR")),m_overminute,m_oversecond);	
		else
			AllTime.Format(gGlobal.GetStringTable()->Get(_T("MSG_AUTOWALK_TIMEOVER_HAS_HOUR")),m_overhour,m_overminute,m_oversecond);
		pStaticText->SetText(AllTime);
		pStaticText->Redraw();
	}
	if(m_bEnable && difftime(time(NULL),m_startTime) < m_AutoTime)
	{
		if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetAction() != CCharacter::ST_BATTLE
			&& gGlobal.g_pCharacter->GetAction() != CCharacter::ST_MONITOR 
			&& gGlobal.g_pCharacter->GetAction() != CCharacter::ST_MOVE )
		{
			if((gGlobal.g_pParty == NULL || gGlobal.g_pParty->IsLeader()))
			{
				if (gGlobal.g_pMap)
					gGlobal.g_pMap->MoveToRandomGuidePos();
			}
		}
	}
	if(m_bEnable && difftime(time(NULL),m_startTime) >= m_AutoTime)
	{
		StopAutoWalk();
	}
}

void CMenuAutoWalkProcess::StartAutoWalk(UInt32 lastTime)
{

	if(!IsHawking())
	{
		UIButton *StartItem = gGlobal.pUIManager->FindUIButton("StartAutoWalk");
		UIButton *StopItem = gGlobal.pUIManager->FindUIButton("StopAutoWalk");
		UIAniIcon* RunAnicon = gGlobal.pUIManager->FindUIAniIcon("RunningAuto");
		UIButton *AutoWalkItem = gGlobal.pUIManager->FindUIButton("DownAutoWalk");
		if (StartItem->IsEnable())
			StartItem->SetEnable(false);
		if (!StopItem->IsEnable())
			StopItem->SetEnable(true);
		if (AutoWalkItem->IsEnable())
			AutoWalkItem->SetEnable(false);
		if(RunAnicon)
			RunAnicon->SetAnimationData(String(_T("MainButtonZ.ent")).c_str());
		if (!RunAnicon->IsEnable())
			RunAnicon->SetEnable(true);
		gGlobal.pUIManager->AddDirtyUIItem(RunAnicon);
		//RunHitBossFun();
		SetAutoTime(lastTime);
		RunAutoWalk();
		m_startAuto = true;
	}
	else
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_DO_AUTOWALK")));
	}
}

void CMenuAutoWalkProcess::StopAutoWalk()
{
	UIPanel* AutoWalkUi = gGlobal.pUIManager->FindUIPanel("AutoWalkPanel");
	UIButton *StartItem = gGlobal.pUIManager->FindUIButton("StartAutoWalk");
	UIButton *StopItem = gGlobal.pUIManager->FindUIButton("StopAutoWalk");
	UIAniIcon* RunAnicon = gGlobal.pUIManager->FindUIAniIcon("RunningAuto");
	UIButton *AutoWalkItem = gGlobal.pUIManager->FindUIButton("DownAutoWalk");
	UIStaticText *pStaticText = gGlobal.pUIManager->FindUIStaticText("AutoWalkTime");
	StringW AllTime = gGlobal.GetStringTable()->Get(_T("MSG_AUTOWALK_STANBDY"));
	if(AutoWalkUi)
	{
		if (!StartItem->IsEnable())
			StartItem->SetEnable(true);
		if (StopItem->IsEnable())
			StopItem->SetEnable(false);
		pStaticText->SetText(AllTime);
		pStaticText->Redraw();
	}
	if(RunAnicon)
	{
		if (RunAnicon->IsEnable())
			RunAnicon->SetEnable(false);
		RunAnicon->ClearAniData();
	}
	if (!AutoWalkItem->IsEnable() &&AutoWalkItem)	
		AutoWalkItem->SetEnable(true);

	CloseAutoWalk();
}

void CMenuAutoWalkProcess::ChangeHitBoss()
{
	UICheckBox* HitBossUi = gGlobal.pUIManager->FindUICheckBox("ChooseHitBoss");
	if (HitBossUi)
	{
		if (m_hitboss)
			m_hitboss = false;
		else
			m_hitboss = true;
	}
	if(m_startAuto)
	{
		if(gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
		{
			if (m_hitboss)
				gGlobal.g_pCharacter->GetCharNetObject()->SetAutoWalkHitBoss(gGlobal.pPeerProxy,gGlobal.g_context,true);
			else
				gGlobal.g_pCharacter->GetCharNetObject()->SetAutoWalkHitBoss(gGlobal.pPeerProxy,gGlobal.g_context,false);
		}
	}
}


void CMenuAutoWalkProcess::OnOpenAutoWalk()
{
	UIPanel* AutoWalkUi = gGlobal.pUIManager->FindUIPanel("AutoWalkPanel");
	UIPanel* HawkUi = gGlobal.pUIManager->FindUIPanel("HawkerOwnerPanel");
	if(!IsHawking()&& !HawkUi)
	{
		if(!AutoWalkUi)
		{
			XML_UI::GetInstance().Init();
			XML_UI::GetInstance().ReadFormFile("Data\\MenuAutoWalk.xml",gGlobal.pUIManager);	
			UIButton *StartItem = gGlobal.pUIManager->FindUIButton("StartAutoWalk");
			UIButton *StopItem = gGlobal.pUIManager->FindUIButton("StopAutoWalk");
			UICheckBox* HitBossUi = gGlobal.pUIManager->FindUICheckBox("ChooseHitBoss");
			UIStaticText *pStaticText = gGlobal.pUIManager->FindUIStaticText("AutoWalkTime");
			StringW AllTime = _T("");
			if(StartItem&&StopItem)
			{
				if (m_bEnable)
				{
					if (StartItem->IsEnable())
						StartItem->SetEnable(false);
					if (!StopItem->IsEnable())
						StopItem->SetEnable(true);
				}
				else
				{
					if (!StartItem->IsEnable())
						StartItem->SetEnable(true);
					if (StopItem->IsEnable())
						StopItem->SetEnable(false);
					AllTime = gGlobal.GetStringTable()->Get(_T("MSG_AUTOWALK_STANBDY"));
				}	
				pStaticText->SetText(AllTime);
				pStaticText->Redraw();
			}
			if (HitBossUi)
			{
				if (m_hitboss)		
					HitBossUi->SetState(false);
				else
					HitBossUi->SetState(true);
			}

		}
		else
		{
			gGlobal.pUIManager->DeletePanel(AutoWalkUi);
		}
	}
	else
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_DO_AUTOWALK")));
	}
}

void CMenuAutoWalkProcess::SetRunning()
{
	if (m_bEnable)
	{
		UIButton *RunItem = gGlobal.pUIManager->FindUIButton("DownAutoWalk");
		if (RunItem->GetState() != _ButtonMouseOver)
			RunItem->SetState(_ButtonMouseOver);
	}
}

void CMenuAutoWalkProcess::ShowTime()
{
	UIAniIcon* RunUi = gGlobal.pUIManager->FindUIAniIcon("RunningAuto");
	GetOverTime();
	StringW AllTime = _T("");
	if (m_overhour < 1)
		AllTime.Format(gGlobal.GetStringTable()->Get(_T("MSG_AUTOWALK_TIMEOVER_NOT_HOUR")),m_overminute,m_oversecond);
	else
		AllTime.Format(gGlobal.GetStringTable()->Get(_T("MSG_AUTOWALK_TIMEOVER_HAS_HOUR")),m_overhour,m_overminute,m_oversecond);
	if (RunUi)
	{
		RunUi->SetTips(AllTime.c_str());
	}
	gGlobal.pUIManager->AddDirtyUIItem(RunUi);
}

void CMenuAutoWalkProcess::SetHitBoss(Boolean hit)
{
	m_hitboss = hit;
}

void CMenuAutoWalkProcess::GetOverTime()
{
	UInt16 SurplusTime = m_AutoTime - difftime(time(NULL),m_startTime);
	m_overhour = 0;
	m_overminute = 0;
	m_oversecond = 0;
	if (SurplusTime < HOUR_TO_SECOND)
	{
		while(SurplusTime > MINUTE_TO_SECOND)
		{
			SurplusTime -= MINUTE_TO_SECOND;
			m_overminute++;
		}
		m_oversecond = SurplusTime;
	}
	else
	{
		while(SurplusTime > HOUR_TO_SECOND)
		{
			SurplusTime -= HOUR_TO_SECOND;
			m_overhour++;
		}
		while(SurplusTime > MINUTE_TO_SECOND)
		{
			SurplusTime -= MINUTE_TO_SECOND;
			m_overminute++;
		}
		m_oversecond = SurplusTime;
	}
}

void CMenuAutoWalkProcess::RunAutoWalk()
{
	m_startTime = ::time(NULL);
	m_bEnable = true;
	m_curMapId = gGlobal.g_pMap->GetMap_id();
	if (gGlobal.g_pMap)
		gGlobal.g_pMap->MoveToRandomGuidePos();
//	if(gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
	//	gGlobal.g_pCharacter->GetCharNetObject()->SetAutoWalk(gGlobal.pPeerProxy,gGlobal.g_context,true);
}

void CMenuAutoWalkProcess::CloseAutoWalk()
{
	m_startTime = 0;
	if(m_bEnable)
	{
		m_bEnable = false;
		if(gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
		{
			//gGlobal.g_pCharacter->GetCharNetObject()->SetAutoWalk(gGlobal.pPeerProxy,gGlobal.g_context,false);
			gGlobal.g_pCharacter->Stop();
		}
	}
}