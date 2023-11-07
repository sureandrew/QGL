//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "QGL.h"
//-- Self
#include ".\antihack.h"
//-- Library
#include "QGL/Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "QGLApplication.h"
#include "QGL/CharacterControl.h"
#include "Configure.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include "Proxy/Service/WorldService.h"
#include "AccountControl.h"



CAntiHack::CAntiHack(void)
{

	m_bEnterBattle = FALSE;	
	m_bBattleDlg = FALSE;	
	m_firstLogin = FALSE;
}

CAntiHack::~CAntiHack(void)
{
}




void CAntiHack::SetHack(UInt8 answer)
{
	RPCContext context;	
	if (m_firstLogin)// for first login, call worldservice
	{
	    WorldService_Proxy::AnswerAntiHack(gGlobal.pPeerProxy, gGlobal.g_context, answer);
		m_firstLogin = BOOLEAN_FALSE;
		m_bEnterBattle = BOOLEAN_FALSE;	
	}
	else
	{    // if not login, call mapservice instead.
		if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
			gGlobal.g_pCharacter->GetCharNetObject()->AnswerAntiHack(gGlobal.pPeerProxy, gGlobal.g_context, answer, BOOLEAN_FALSE);
	}
}

void CAntiHack::DrawAntiHackImage(UInt8 &picIndex, const UInt16 &mobID1, const UInt16 &mobID2, Boolean overlap) 
{
	if (mobID1 == mobID2)
		int abc = 0;
	MobDB_Client * mobTemplate1 = gGlobal.LoadMobTemplate(mobID1);	
	MobDB_Client * mobTemplate2 = gGlobal.LoadMobTemplate(mobID2);
	UIAniIcon *pAniIcon1 = gGlobal.pUIManager->FindUIAniIcon("AntiHackDummyIcon1");
	UIAniIcon *pAniIcon2 = gGlobal.pUIManager->FindUIAniIcon("AntiHackDummyIcon2");	
	if (!mobTemplate1 || !mobTemplate2 || !pAniIcon1 || !pAniIcon2)
		return;
	for (Index i = C_INDEX(0); i < 2; ++i)
	{
		UIAniIcon *ptempIcon = (i == 0)? pAniIcon1 : pAniIcon2;
		MobDB_Client *tempMob = (i == 0)? mobTemplate1 : mobTemplate2; 
		Byte baseAlpha = (i == 0)? 50 : 80;
		Byte randomAlpha = (i == 0)? 30 : 50;
		if (!overlap)
			baseAlpha = 10;
 		CAnimatedEntity temp;
		temp.InitEntPicInfo();
		temp.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
		temp.LoadEntityFile(String(tempMob->m_basicinfo->model).c_str(), gGlobal.pD3DDevice);
        temp.SetAlphaOffset(baseAlpha + gGlobal.m_rand.GetRandom(randomAlpha));
		ptempIcon->SetAnimationData(&temp);
		ptempIcon->SetAnimationIndex(temp.GetBattleAnimation());				
		ptempIcon->SetRotation(SOUTHEAST);
		ptempIcon->GetAniEnt()->UpDate(::timeGetTime());
	}
	StringA hackImgIndex;
	hackImgIndex.Format("AntiHackUI%i", picIndex);
	UIImage *testUI = gGlobal.pUIManager->FindUIImage(hackImgIndex.c_str());
	if (!testUI)
		return;
	CImage *cImg = testUI->GetImage();
	if (cImg)
		CImageManager::GetInstance().DeleteImage(cImg);
	cImg = SafeCreateObject3(CImage, testUI->GetWidth(), testUI->GetHeight(), 8888);
	CImageManager::GetInstance().CreateImage(cImg);
	RECT *rec = NULL;
	pAniIcon1->DrawEntityToImage(cImg, rec);
	rec = NULL;
	if (overlap)
        pAniIcon2->DrawEntityToImage(cImg, rec);
	testUI->SetImage(cImg, 0, 0, 500, 500);
	testUI->SetEnable(true);
	gGlobal.pUIManager->AddDirtyUIItem(testUI);
	pAniIcon1->ClearAniData();	
	pAniIcon2->ClearAniData();
}

void CAntiHack::OpenAntiHackPopUp(ByteArray &image, AntiHackData &data)
{
	VMProtectBegin; VM_START;
	////// open the Anti-Hack Panel and set it as a mono panel ////////
	if (m_firstLogin)
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenAntiHackLoginVerification", String());
	else
        CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenAntiHackPanel", String());

	m_bEnterBattle = TRUE;
	m_bBattleDlg = FALSE;
	UIPanel *panel = gGlobal.pUIManager->FindUIPanel("AntiHackPanel");
	if (panel)
	{
		gGlobal.pUIManager->ClearMonoPanel();
		gGlobal.pUIManager->SetPanel2Top(panel);	
		gGlobal.pUIManager->SetMonoPanel(panel);	
	}
	UInt16Vector imageIndex;
	for (Index c = C_INDEX(0); c < 49; ++c)
		imageIndex.push_back(hackImage[c]);
    
	if (data.chars[0] == 0)
	{
		for (UInt8 i = 1;i < 6; ++i) // draw each antihack overlapped image, except for the "answer image"
		{   
			UInt16 mobID1, mobID2;
			for (Index j = C_INDEX(0); j < 2; ++j)
			{
                UInt16 key = gGlobal.m_rand.GetRandom((UInt16)imageIndex.size());
				if (j == 0)
                    mobID1 = imageIndex[key];
				else
					mobID2 = imageIndex[key];
				imageIndex.erase(imageIndex.begin() + key);	
			}
			DrawAntiHackImage(i, mobID1, mobID2, ((data.answer + 1) != i));
			StringA hackPersonIndex;
			hackPersonIndex.Format("HackImageIndex%d", i);
			UIStaticText* pStackText = gGlobal.pUIManager->FindUIStaticText(hackPersonIndex.c_str());
			pStackText->SetEnable(true);
		}
	}
	else
	{	
		UIImage* AHImage = gGlobal.pUIManager->FindUIImage("AntiHackImage");
		AHImage->LoadCZipMem(image.GetData(), (UInt)image.GetSize()); 
		AHImage->SetEnable(true);
	}

	/// set the anti-hack question
	String mainQ; 
	if (data.chars[0] == 0)
		mainQ  = gGlobal.GetStringTable()->Get(_T("MSG_HACK_MAIN_Q_2"));	
	else
		mainQ  = gGlobal.GetStringTable()->Get(_T("MSG_HACK_MAIN_Q_1"));	

	gGlobal.SetUIStaticText("AntiHackQuestion", mainQ);	

	if (!m_firstLogin)
	{
		String subQ  = gGlobal.GetStringTable()->Get(_T("MSG_HACK_SUB_Q"));		
		gGlobal.SetUIStaticText("AntiHackSubQuestion", subQ);		
	}
	//////// show the answer list ////////////////
	for (Index i = C_INDEX(0);i < 5; ++i)
	{
		StringA chatPanelName;
		chatPanelName.Format("AntiHackAnswer%d", i);
		UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel(chatPanelName.c_str());
		String str_answer;
		if (pChatPanel)
		{
			if (data.chars[0] == 0)
			{
				pChatPanel->ClearAllRow();
				String choice  = gGlobal.GetStringTable()->Get(_T("MSG_HACK_CHOICE"));		
				str_answer.Format(_T("%s%d"), choice.c_str(), i+1);	
				pChatPanel->AddStringW(str_answer.c_str());	
				str_answer.Empty();
				pChatPanel->EndRow();
				gGlobal.pUIManager->AddDirtyUIItem(pChatPanel);                

			}
			else
			{
				pChatPanel->ClearAllRow();
				Char temp = data.chars[i];	
				str_answer.Format(_T("%d) "), i+1);	
				str_answer.push_back(temp);
				pChatPanel->AddStringW(str_answer.c_str());	
				str_answer.Empty();
				pChatPanel->EndRow();
				gGlobal.pUIManager->AddDirtyUIItem(pChatPanel);
			}
		}
	}

	VMProtectEnd; VM_END;
}

void CAntiHack::CloseAntiHackPanel()
{
	m_bEnterBattle = FALSE;
	if (m_bBattleDlg)
	{
		UIPanelEx *pBatDlg = gGlobal.pUIManager->FindUIPanelEx("BDB_DialogueBox");
		UIPanel *pBatImgDlg = gGlobal.pUIManager->FindUIPanel("BDB_DialogImagePanel");
		if (pBatDlg && pBatImgDlg)
		{
			gGlobal.pUIManager->ClearMonoPanel();
			gGlobal.pUIManager->SetPanel2Top(pBatImgDlg);
			gGlobal.pUIManager->SetPanelEx2Top(pBatDlg);
			gGlobal.pUIManager->SetMonoPanel(pBatDlg);	
		}
		m_bBattleDlg = FALSE;

	}

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SwitchOffAntiHack", String());


}




