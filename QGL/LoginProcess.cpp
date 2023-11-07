//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "LoginProcess.h"
//-- Library
#include "UI.h"
#include "Global.h"
#include "Configure.h"
#include "CharacterControl.h"
#include "AccountControl.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"
#include "Proxy/Service/MapService_proxy.h"
#include "Proxy/Logic/Map.h"
#include "NewBieHintCtrl.h"
#include "MenuSystemSetting.h"

//////////////////////////////////
//#include "MessageBox.h"
//////////////////////////////////

#define MAX_CHAR_SHOW_IN_LOGIN 3

CLoginProcess::CLoginProcess()
{
	m_nCharacterIndex = -1;
	m_nLineIndex = -1;
}


void CLoginProcess::EnterGame()
{
	if (gConfigure.uNetwork)
	{
		Int temp = gGlobal.g_pAccountControl->m_characters->GetCount();
		if ((GetCharacterIndex() >= temp) || (GetCharacterIndex() < 0) )
			return;

		gGlobal.g_pAccountControl->EnterGame(GetCharacterIndex());
		
		UIPanel* serverChosenPanel = gGlobal.pUIManager->FindUIPanel("ServerChosenPanel");
		if (serverChosenPanel)
			serverChosenPanel->SetEnable(false);
			
		gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_LOGIN")), false);
	}
}

void CLoginProcess::ShowLogin()
{
	UIPanel* loginEditPanel = gGlobal.pUIManager->FindUIPanel("LoginEditPanel");
	UIPanel* loginButtonPanel = gGlobal.pUIManager->FindUIPanel("LoginButtonPanel");
	UIPanel* loginCheckBoxPanel = gGlobal.pUIManager->FindUIPanel("LoginCheckBoxPanel");
	UIPanel* characterChosenPanel = gGlobal.pUIManager->FindUIPanel("CharacterChosenPanel");
	UIPanel* loginBackground = gGlobal.pUIManager->FindUIPanel("LoginBackground");

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SetLoginState", String(), 0);

	if (loginEditPanel)
		loginEditPanel->SetEnable(true);

	if (loginButtonPanel)
		loginButtonPanel->SetEnable(true);

	if (loginCheckBoxPanel)
		loginCheckBoxPanel->SetEnable(true);

	if (characterChosenPanel)
		characterChosenPanel->SetEnable(false);

	if (loginBackground)
		loginBackground->SetEnable(false);

	if (gGlobal.g_pSystemSetting && gGlobal.g_pSystemSetting->GetUserName().length() > 0)
	{
		UICheckBox* checkBox = gGlobal.pUIManager->FindUICheckBox("SaveLogin");
		UIEditBox* loginNameEditBox = gGlobal.pUIManager->FindUIEditBox("LoginName");
		if (checkBox && loginNameEditBox)
		{
			checkBox->SetState(true);
			loginNameEditBox->SetText(gGlobal.g_pSystemSetting->GetUserName().c_str());
			loginNameEditBox->Redraw();
		}
	}

	gGlobal.pUIManager->ChangeFocus("LoginName");

	gGlobal.pUIManager->RenderAll();

}

void CLoginProcess::ResetCharacterInLogin()
{
	UIAniIcon * pAniIcon;
	UIAniIcon* pShadowIcon;
	UIStaticText* Name;
	UIImage*	MHeadPic;
	UIImage*	LPic;
	UIButton*	selectButton;

	StringA CharModel;
	StringA ModelShadow;
	StringA CharName;
	StringA CharMHeadPic;
	StringA CharLPic;
	StringA CharSelect;
	// to make all the thing invisible first
	for (int j = 0; j < MAX_CHAR_SHOW_IN_LOGIN; j++)
	{
		CharModel.Format("CharModel%d", ( j + 1 ) );
		ModelShadow.Format("ModelShadow%d", ( j + 1 ) );
		CharName.Format("CharacterName%d", ( j + 1 ) );

		CharMHeadPic.Format("MHeadPic%d", ( j + 1) );
		CharSelect.Format("CharacterSelected%d", (j + 1) );

		pAniIcon = gGlobal.pUIManager->FindUIAniIcon(CharModel.c_str());
		pShadowIcon = gGlobal.pUIManager->FindUIAniIcon(ModelShadow.c_str());
		Name = gGlobal.pUIManager->FindUIStaticText(CharName.c_str()); 
		MHeadPic = gGlobal.pUIManager->FindUIImage(CharMHeadPic.c_str());
		selectButton = gGlobal.pUIManager->FindUIButton(CharSelect.c_str());

		if (pAniIcon)
			pAniIcon->SetEnable(false);

		if (pShadowIcon)
			pShadowIcon->SetEnable(false);

		if (Name)
			Name->SetEnable(false);

		if (MHeadPic)
			MHeadPic->SetEnable(false);

		if (selectButton)
			selectButton->SetNotDetected(true);

	}

	for (int i = 0; i < 8; ++i)
	{
		CharLPic.Format("CharChosenLImage%d", i + 1);
		LPic = gGlobal.pUIManager->FindUIImage(CharLPic.c_str());
		if (LPic)
			LPic->SetEnable(false);
	}

	UIStaticText* Career;
	UIStaticText* Level;
	UIStaticText* Ring;

	Name = gGlobal.pUIManager->FindUIStaticText("ChosenCharName"); 
	Career = gGlobal.pUIManager->FindUIStaticText("ChosenCharCareer"); 
	Level = gGlobal.pUIManager->FindUIStaticText("ChosenCharLevel");
	Ring = gGlobal.pUIManager->FindUIStaticText("ChosenCharRing");

	if (Name)
		Name->SetEnable(false);

	if (Career)
		Career->SetEnable(false);

	if (Level)
		Level->SetEnable(false);

	if (Ring)
		Ring->SetEnable(false);
}


void CLoginProcess::ShowCharacterInLogin()
{
	CGameStringTable* stringTable = gGlobal.GetStringTable();
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SetLoginState", String(), 1);
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseAlertBox", String());

	UIPanel* loginEditPanel = gGlobal.pUIManager->FindUIPanel("LoginEditPanel");
	UIPanel* loginButtonPanel = gGlobal.pUIManager->FindUIPanel("LoginButtonPanel");
	UIPanel* loginCheckBoxPanel = gGlobal.pUIManager->FindUIPanel("LoginCheckBoxPanel");
	UIPanel* loginBackground = gGlobal.pUIManager->FindUIPanel("LoginBackground");

	gGlobal.pUIManager->DeletePanel("MiniKeyBoardPanel");
	UIPanel* characterChosenPanel = gGlobal.pUIManager->FindUIPanel("CharacterChosenPanel");
	UIPanel* serverChosenPanel = gGlobal.pUIManager->FindUIPanel("ServerChosenPanel");
	
	if (loginEditPanel)
		loginEditPanel->SetEnable(false);

	if (loginButtonPanel)
		loginButtonPanel->SetEnable(false);

	if (loginCheckBoxPanel)
		loginCheckBoxPanel->SetEnable(false);

	if (serverChosenPanel)
		serverChosenPanel->SetEnable(false);
	
	if (characterChosenPanel)
		characterChosenPanel->SetEnable(true);

	if (loginBackground)
		loginBackground->SetEnable(true);

	UInt16 NoOfCharacter = gGlobal.g_pAccountControl->m_characters->GetCount();

	UIButton* delDutton = gGlobal.pUIManager->FindUIButton("DeleteCharacter");
	if (delDutton)
	{
		if (NoOfCharacter > 0)
			delDutton->SetNormalState();
		else
			delDutton->SetDisableState();
	}
	UIAniIcon * pAniIcon = NULL;
	UIAniIcon* pShadowIcon = NULL;
	UIStaticText* Name = NULL;
	UIImage*	MHeadPic = NULL;
	UIButton*   selectButton = NULL;
	UIStaticText* life = NULL;
	UIButton*   recoverLifeButton = NULL;
	UIImage*  frozenImage = NULL;
	UIImage*  frozenHightlight = NULL;	

	StringA CharModel;
	StringA ModelShadow;
	StringA CharName;
	StringA CharMHeadPic;
	StringA CharSelect;
	StringA CharRemainLife; // character remaining life
	StringA CharRecoverLife; // recover character from deletion
	StringA CharFrozen;
	StringA CharFrozenHigh;

	ResetCharacterInLogin();

	Boolean charExpire = BOOLEAN_FALSE;
	UInt expireCharIndex = 0;

	for(Index i = C_INDEX(0); i < MAX_CHAR_SHOW_IN_LOGIN; ++i) // do not show life remaining time at first
	{
		StringA lifeName, recoverName, banName, banHightlightName;
		lifeName.Format("CharacterLifeRemain%d", ( i + 1 ));
		recoverName.Format("CharacterRecovery%d", ( i + 1 ));	
		banName.Format("CharacterBan%d", (i+1));
		banHightlightName.Format("CharacterBanHighlight%d", (i+1));
		UIStaticText* tempText = gGlobal.pUIManager->FindUIStaticText(lifeName.c_str());
		UIButton*   tempBut = gGlobal.pUIManager->FindUIButton(recoverName.c_str());
		UIImage* tempBan = gGlobal.pUIManager->FindUIImage(banName.c_str());
		UIImage* tempLight = gGlobal.pUIManager->FindUIImage(banHightlightName.c_str());	
		if (tempText)
			tempText->SetEnable(false);
		if (tempBut)
			tempBut->SetEnable(false);
		if (tempBan)
			tempBan->SetEnable(false);
		if (tempLight)
			tempLight->SetEnable(false);	
	}

	for(Index i = C_INDEX(0); i < NoOfCharacter; ++i)
	{
		CharModel.Format("CharModel%d", ( i + 1 ) );
		ModelShadow.Format("ModelShadow%d", ( i + 1 ) );
		CharName.Format("CharacterName%d", ( i + 1 ) );
		CharMHeadPic.Format("MHeadPic%d", ( i + 1) );
		CharSelect.Format("CharacterSelected%d", ( i + 1 ) );
		CharRemainLife.Format("CharacterLifeRemain%d", ( i + 1 ) );	
		CharRecoverLife.Format("CharacterRecovery%d", ( i + 1 ) );
		CharFrozen.Format("CharacterBan%d", (i+1));
		CharFrozenHigh.Format("CharacterBanHighlight%d", (i+1));

		pAniIcon = gGlobal.pUIManager->FindUIAniIcon(CharModel.c_str());
		pShadowIcon = gGlobal.pUIManager->FindUIAniIcon(ModelShadow.c_str());
		Name = gGlobal.pUIManager->FindUIStaticText(CharName.c_str()); 
		MHeadPic = gGlobal.pUIManager->FindUIImage(CharMHeadPic.c_str());
		selectButton = gGlobal.pUIManager->FindUIButton(CharSelect.c_str());
		life = gGlobal.pUIManager->FindUIStaticText(CharRemainLife.c_str());
		recoverLifeButton = gGlobal.pUIManager->FindUIButton(CharRecoverLife.c_str());
		frozenImage = gGlobal.pUIManager->FindUIImage(CharFrozen.c_str());
		frozenHightlight = gGlobal.pUIManager->FindUIImage(CharFrozenHigh.c_str());	

		if (pAniIcon)
		{
			CCharacterInfo *pCharData = gGlobal.g_pAccountControl->m_characters->GetChild<CCharacterInfo>(i);
			UInt8 SexClass = 0;
			if (pCharData->GetCclass() > 0)
				SexClass = 1 + (pCharData->GetCclass() - 1) * MAX_SEXTYPE + pCharData->GetSex();

			UInt32 eqCloth = pCharData->GetOrg_eq_cloth();
			UInt16 eqClothColor = pCharData->GetOrg_eq_clothColor();
			UInt16 hairStyle = pCharData->GetOrg_hairStyle1();
			UInt16 hairColor = pCharData->GetOrg_hairColor();

			if (pCharData->GetHairStyle1() > 0)
				hairStyle = pCharData->GetHairStyle1();
			if (pCharData->GetHairColor() > 0)
				hairColor = pCharData->GetHairColor();
			if (pCharData->GetEq_cloth() > 0)
			{
				eqCloth = pCharData->GetEq_cloth();
				eqClothColor = pCharData->GetEq_clothColor();
			}

			CAnimatedEntity temp = gGlobal.GetCharModel(SexClass, hairStyle, hairColor, eqCloth, 
										eqClothColor, pCharData->GetEq_headBand(), pCharData->GetEq_weapon() );

			pAniIcon->SetAnimationData(&temp);
			pAniIcon->SetEnable(true);

			if (pShadowIcon)
				pShadowIcon->SetEnable(true);

			String tempName = pCharData->GetNickName();

			if (Name)
			{
				Name->SetText(tempName);
				Name->SetEnable(true);
				Name->Redraw();
			}

			if (selectButton)
				selectButton->SetNotDetected(false);

			if (life && recoverLifeButton)
			{
				CCharacterInfo *pCharData = gGlobal.g_pAccountControl->m_characters->GetChild<CCharacterInfo>(i);
				UInt32 timeNow = (UInt32) time(NULL);
				if (pCharData->GetDelete_date() != 0)
				{
					UInt32 timeLeft = pCharData->GetDelete_date() > timeNow ? pCharData->GetDelete_date() - timeNow : 0;
					UInt16 day = timeLeft/(60*60*24);
					UInt16 hour = (timeLeft - (day * (60*60*24)))/(60*60);
					UInt16 minute = (timeLeft - (day * (60*60*24)) - (hour * (60*60)))/60;
					String lifetime;
					lifetime.Format(gGlobal.GetStringTable()->Get(_T("MSG_LOGIN_CHARACTER_LIFE")), day, hour, minute);	
					life->SetText(lifetime);
					life->SetEnable(true);
					life->Redraw();	
					recoverLifeButton->SetEnable(true);	
					if (frozenImage)
						frozenImage->SetEnable(true);
					if (frozenHightlight)
						frozenHightlight->SetEnable(true);

					if (timeNow >= pCharData->GetDelete_date())
					{
                        charExpire = BOOLEAN_TRUE;	
						expireCharIndex = i;
					}	
					if (selectButton)
						selectButton->SetNotDetected(true);	
				}
				else
				{
                    life->SetEnable(false);
					recoverLifeButton->SetEnable(false);	
				}
			}
			String MheadPicStr;
			MheadPicStr.Format(_T("Data\\NPCFACE\\ImgS%05da.tga"),SexClass ); 

			if(MHeadPic)
			{
				MHeadPic->SetImageFile(MheadPicStr.c_str());
				MHeadPic->SetEnable(true);
			}
		}		
	}

	// default character loaded from setting
	UInt32 charIndex = ::GetDefaultCharacter();
	StringA funcCallDefaultChar;

	if (charIndex < NoOfCharacter)
	{
		if (!gGlobal.g_pAccountControl->CheckCharacterFrozenStatus(charIndex))
		{
			funcCallDefaultChar.Format("SelectCharacter%i", charIndex + 1);
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), funcCallDefaultChar.c_str(), String());
		}
	}
	else if (charIndex >= NoOfCharacter && NoOfCharacter > 0 )
	{
		if (!gGlobal.g_pAccountControl->CheckCharacterFrozenStatus(0))
		{
			funcCallDefaultChar.Format("SelectCharacter%i", 1);
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), funcCallDefaultChar.c_str(), String());
		}
	}

	// BACK button modification
	UIButton* btn = gGlobal.pUIManager->FindUIButton("CharacterChosenPrev");
	if( btn )
		btn->SetNormalState();

	if (m_nCharacterIndex >= 0 )
	{
		CCharacterInfo *pCharData = gGlobal.g_pAccountControl->m_characters->GetChild<CCharacterInfo>(m_nCharacterIndex);

		if ((pCharData) && (!gGlobal.g_pAccountControl->CheckCharacterFrozenStatus(m_nCharacterIndex)))
		{
			UInt8 SexClass = 0;
			if (pCharData->GetCclass() > 0)
				SexClass = 1 + (pCharData->GetCclass() - 1) * MAX_SEXTYPE + pCharData->GetSex();

			UIStaticText* Career = NULL;
			UIStaticText* Level = NULL;
			UIStaticText* Ring = NULL;
			UIImage*	LPic = NULL;
			StringA CharLPic;

			CharLPic.Format("CharChosenLImage%d", SexClass);
			LPic = (UIImage*)gGlobal.pUIManager->GetUIItem(CharLPic.c_str());
			if (LPic)
				LPic->SetEnable(true);

			Name = gGlobal.pUIManager->FindUIStaticText("ChosenCharName"); 
			Career = gGlobal.pUIManager->FindUIStaticText("ChosenCharCareer"); 
			Level = gGlobal.pUIManager->FindUIStaticText("ChosenCharLevel");
			Ring = gGlobal.pUIManager->FindUIStaticText("ChosenCharRing");

			String tempName = pCharData->GetNickName();

			Name->SetText(tempName);
			Name->SetEnable(true);
			Name->Redraw();

			String tempCareer;
			String tempCareerName;
			tempCareerName.Format(_T("MSG_ACTOR_CLASS_%d"), SexClass);
			tempCareer = stringTable->Get(tempCareerName.c_str());

			Career->SetText(tempCareer);
			Career->SetEnable(true);
			Career->Redraw();

			String tempLevel;
			tempLevel.Format(_T("%d"),pCharData->GetLevel()); 
			Level->SetText(tempLevel.c_str());
			Level->SetEnable(true);
			Level->Redraw();

			String tempRing;

			switch ( pCharData->GetFaction())
			{
			case 1:	tempRing = stringTable->Get(_T("MSG_FACTION_SIXFANDOOR"));	break;
			case 2:	tempRing = stringTable->Get(_T("MSG_FACTION_TWELVEDOCK"));	break;
			case 3:	tempRing = stringTable->Get(_T("MSG_FACTION_HOLY"));		break;
			case 4:	tempRing = stringTable->Get(_T("MSG_FACTION_SILVERCASINO"));break;
			case 5:	tempRing = stringTable->Get(_T("MSG_FACTION_TONGDOOR"));	break;
			case 6:	tempRing = stringTable->Get(_T("MSG_FACTION_10THOUSAND"));	break;
			case 7:	tempRing = stringTable->Get(_T("MSG_FACTION_GREENCLOTH"));	break;
			case 8:	tempRing = stringTable->Get(_T("MSG_FACTION_GHOSTAREA"));	break;
			}
			Ring->SetText(tempRing);
			Ring->SetEnable(true);
			Ring->Redraw();
		}
	}

	UIButton* button = gGlobal.pUIManager->FindUIButton("CreateCharacter");
	if (NoOfCharacter >= MAX_CHAR_SHOW_IN_LOGIN)
	{
		button->SetNotDetected(true);
		button->SetState(_ButtonClick);
	}
	else 
	{
		button->SetNotDetected(false);
		button->SetNormalState();
	}

	gGlobal.pUIManager->AddDirtyUIItem(characterChosenPanel);

	if (charExpire){
		SetCharacterIndex(expireCharIndex);	
		gGlobal.g_pAccountControl->DeleteCharacter();
	}

}

void CLoginProcess::ShowChooseLine()
{
	Int temp = gGlobal.g_pAccountControl->m_characters->GetCount();
	if ((GetCharacterIndex() >= temp) || (GetCharacterIndex() < 0) )
		return;

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SetLoginState", String(), 2);

	UIPanel* characterChosenPanel = (UIPanel*)gGlobal.pUIManager->GetUIItem("CharacterChosenPanel");

	if (characterChosenPanel)
		characterChosenPanel->SetEnable(false);

	UIPanel* serverChosenPanel = (UIPanel*)gGlobal.pUIManager->GetUIItem("ServerChosenPanel");
	if (serverChosenPanel)
		serverChosenPanel->SetEnable(true);

	UIStaticText* tempText;
	UIButton* tempButton;
	UIImage* tempImage;

	StringA tempTextName;
	StringA tempButtonName;
	StringA tempImageName;
	String tempLineName;

	int tempSize = (int)gGlobal.m_LineExist.size();
	if (serverChosenPanel)
	{
		for (int i = 0; i < ( (int)gGlobal.m_LineExist.size() - 1); i++)
		{
			tempTextName.Format("ServerName%d",( i + 1 ));
			tempButtonName.Format("Server%d",( i + 1 ));
			tempImageName.Format("ServerState%d", ( i + 1));

			tempText = gGlobal.pUIManager->FindUIStaticText(tempTextName.c_str());
			tempButton = gGlobal.pUIManager->FindUIButton(tempButtonName.c_str());
			tempImage = gGlobal.pUIManager->FindUIImage(tempImageName.c_str());
			StringA lineTextKey;
			lineTextKey.Format("MSG_INTERFACE_LINE_%i", i+1);

			switch (i)
			{
			case 0: case 1: case 2: case 3: case 4:
			case 5: case 6: case 7: case 8: case 9:
				tempLineName = gGlobal.GetStringTable()->Get(String(lineTextKey.c_str()).c_str());
				break;
			}

			tempText->SetText(tempLineName.c_str());
			tempText->Redraw();

			if (gGlobal.m_LineExist[i+1] < 0)
			{
				tempButton->SetEnable(false);
				tempImage->SetEnable(false);
				tempText->SetEnable(false);
				/*if (tempImage->GetImage() == NULL)
					tempImage->SetImageFileA("Data\\interface0016_begin_B.tga", 323 ,528, 366, 548); //repair state
				else
					tempImage->SetImage(tempImage->GetImage(), 323 ,528, 366, 548);*/
			}
			else 
			{
				tempButton->SetEnable(true);
				tempText->SetEnable(true);
				
				if (gGlobal.m_lineState[i+1] == LINE_STATE_NORMAL)
				{
					if (tempImage->GetImage() == NULL)
						tempImage->SetImageFileA("Data\\interface0016_begin_B.tga", 323 ,462, 366, 482); //normal state
					else
						tempImage->SetImage(tempImage->GetImage(), 323 ,462, 366, 482);
				}
				else if (gGlobal.m_lineState[i+1] == LINE_STATE_SUPERFULL )
				{
					if (tempImage->GetImage() == NULL)
						tempImage->SetImageFileA("Data\\interface0016_begin_B.tga", 323, 484, 366, 504); //super full
					else
						tempImage->SetImage(tempImage->GetImage(), 323, 484, 366, 504);
				}
				else if (gGlobal.m_lineState[i+1] == LINE_STATE_BUSY )
				{
					if (tempImage->GetImage() == NULL)
						tempImage->SetImageFileA("Data\\interface0016_begin_B.tga", 323 ,506, 366, 526); //busy
					else
						tempImage->SetImage(tempImage->GetImage(), 323 ,506, 366, 526);
				}
				tempImage->SetEnable(true);
			}
		}

		for (int i = ( (int)gGlobal.m_LineExist.size() - 1); i < 10; i++)
		{
			tempTextName.Format("ServerName%d",( i + 1 ));
			tempButtonName.Format("Server%d",( i + 1 ));
			tempImageName.Format("ServerState%d", ( i + 1));

			tempText = gGlobal.pUIManager->FindUIStaticText(tempTextName.c_str());
			tempButton = gGlobal.pUIManager->FindUIButton(tempButtonName.c_str());
			tempImage = gGlobal.pUIManager->FindUIImage(tempImageName.c_str());

			tempText->SetEnable(false);
			tempButton->SetEnable(false);
			tempImage->SetEnable(false);
		}

		UInt32 lineIndex = ::GetDefaultLine();
		if( gGlobal.m_lineState[lineIndex+1]==LINE_STATE_REPAIR )
			lineIndex = 0;
		StringA funcCallDefaultChar;
		funcCallDefaultChar.Format("SelectServer%i", lineIndex + 1);

		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), funcCallDefaultChar.c_str(), String());	
		gGlobal.pUIManager->AddDirtyUIItem(serverChosenPanel);
	}
}


void CLoginProcess::SetCharacterIndex(Int32 charIndex)
{
	m_nCharacterIndex = charIndex;
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("CharacterChosenPanel");	
	if (panel && charIndex >= 0 )
	{
		CCharacterInfo *pCharData = gGlobal.g_pAccountControl->m_characters->GetChild<CCharacterInfo>(m_nCharacterIndex);
		CGameStringTable* stringTable = gGlobal.GetStringTable();
		if (pCharData)
		{
			UInt8 SexClass = 0;
			if (pCharData->GetCclass() > 0)
				SexClass = 1 + (pCharData->GetCclass() - 1) * MAX_SEXTYPE + pCharData->GetSex();

			UIStaticText* Name = NULL;
			UIStaticText* Career = NULL;
			UIStaticText* Level = NULL;
			UIStaticText* Ring = NULL;
			UIImage*	LPic = NULL;
			StringA CharLPic;

			CharLPic.Format("CharChosenLImage%d", SexClass);
			LPic = (UIImage*)gGlobal.pUIManager->GetUIItem(CharLPic.c_str());
			if (LPic)
				LPic->SetEnable(true);

			Name = gGlobal.pUIManager->FindUIStaticText("ChosenCharName"); 
			Career = gGlobal.pUIManager->FindUIStaticText("ChosenCharCareer"); 
			Level = gGlobal.pUIManager->FindUIStaticText("ChosenCharLevel");
			Ring = gGlobal.pUIManager->FindUIStaticText("ChosenCharRing");

			String tempName = pCharData->GetNickName();

			Name->SetText(tempName);
			Name->SetEnable(true);
			Name->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(Name);

			String tempCareer;
			String tempCareerName;
			tempCareerName.Format(_T("MSG_ACTOR_CLASS_%d"), SexClass);
			tempCareer = stringTable->Get(tempCareerName.c_str());

			Career->SetText(tempCareer);
			Career->SetEnable(true);
			Career->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(Career);

			String tempLevel;
			tempLevel.Format(_T("%d"),pCharData->GetLevel()); 
			Level->SetText(tempLevel.c_str());
			Level->SetEnable(true);
			Level->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(Level);

			String tempRing;

			switch ( pCharData->GetFaction())
			{
			case 1:	tempRing = stringTable->Get(_T("MSG_FACTION_SIXFANDOOR"));	break;
			case 2:	tempRing = stringTable->Get(_T("MSG_FACTION_TWELVEDOCK"));	break;
			case 3:	tempRing = stringTable->Get(_T("MSG_FACTION_HOLY"));		break;
			case 4:	tempRing = stringTable->Get(_T("MSG_FACTION_SILVERCASINO"));break;
			case 5:	tempRing = stringTable->Get(_T("MSG_FACTION_TONGDOOR"));	break;
			case 6:	tempRing = stringTable->Get(_T("MSG_FACTION_10THOUSAND"));	break;
			case 7:	tempRing = stringTable->Get(_T("MSG_FACTION_GREENCLOTH"));	break;
			case 8:	tempRing = stringTable->Get(_T("MSG_FACTION_GHOSTAREA"));	break;
			}
			Ring->SetText(tempRing);
			Ring->SetEnable(true);
			Ring->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(Ring);
		}
	}
}

void CLoginProcess::SetAniState(int index, int state)
{
	if (index < 0 || index >= gGlobal.g_pAccountControl->m_characters->GetCount())
		return;

	StringA aniIconName;
	aniIconName.Format("CharModel%d", index + 1);
	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(aniIconName.c_str());
	CAnimatedEntity* characterModel = NULL;

	if (pAniIcon)
	{
		characterModel = pAniIcon->GetAniEnt();
		if (characterModel)
		{
			if (state == 0)
				characterModel->SetAnimationIndexByIndex(ANIM_IDLE1);
			else if (state == 1)
				characterModel->SetAnimationIndexByIndex(ANIM_RUN);
			
			gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
		}
	}
}

void CLoginProcess::UpdateChangeLine()
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("ChangeLinePanel");

	if (panel)
	{
		gGlobal.g_pCharacter->Stop();

		UIStaticText* tempText;
		UIButton* tempButton;
		UIImage* tempImage;

		StringA tempTextName;
		StringA tempButtonName;
		StringA tempImageName;
		String tempLineName;

		int tempSize = (int)gGlobal.m_LineExist.size();
		for (int i = 0; i < ( (int)gGlobal.m_LineExist.size() - 1); i++)
		{
			tempTextName.Format("MapLineName%d",( i + 1 ));
			tempButtonName.Format("MapLine%d",( i + 1 ));
			tempImageName.Format("MapLineState%d", ( i + 1));

			tempText = gGlobal.pUIManager->FindUIStaticText(tempTextName.c_str());
			tempButton = gGlobal.pUIManager->FindUIButton(tempButtonName.c_str());
			tempImage = gGlobal.pUIManager->FindUIImage(tempImageName.c_str());

			StringA lineTextKey;
			lineTextKey.Format("MSG_INTERFACE_LINE_%i", i+1);

			switch (i)
			{
			case 0: case 1: case 2: case 3: case 4:
			case 5: case 6: case 7: case 8: case 9:
				tempLineName = gGlobal.GetStringTable()->Get(String(lineTextKey.c_str()).c_str());
				break;
			}

			tempText->SetText(tempLineName.c_str());
			tempText->Redraw();

			if (gGlobal.m_LineExist[i+1] < 0)
			{
				tempText->SetEnable(false);
				tempButton->SetEnable(false);
				tempImage->SetEnable(false);
				//tempImage->SetImageFileA("Data\\interface0016_begin_B.tga", 323 ,528, 366, 548); //repair state
			}
			else 
			{
				tempText->SetEnable(true);
				tempButton->SetEnable(true);
				if (gGlobal.m_lineState[i+1] == LINE_STATE_NORMAL)
				{
					if (tempImage->GetImage() == NULL)
						tempImage->SetImageFileA("Data\\interface0016_begin_B.tga", 323 ,462, 366, 482); //normal state
					else
						tempImage->SetImage(tempImage->GetImage(), 323 ,462, 366, 482);
				}
				else if (gGlobal.m_lineState[i+1] == LINE_STATE_SUPERFULL )
				{
					if (tempImage->GetImage() == NULL)
						tempImage->SetImageFileA("Data\\interface0016_begin_B.tga", 323, 484, 366, 504); //super full
					else
						tempImage->SetImage(tempImage->GetImage(), 323, 484, 366, 504);
				}
				else if (gGlobal.m_lineState[i+1] == LINE_STATE_BUSY )
				{
					if (tempImage->GetImage() == NULL)
						tempImage->SetImageFileA("Data\\interface0016_begin_B.tga", 323 ,506, 366, 526); //busy
					else
						tempImage->SetImage(tempImage->GetImage(), 323 ,506, 366, 526);
				}
				tempImage->SetEnable(true);
			}
		}
		for (int i = ( (int)gGlobal.m_LineExist.size() - 1); i < 10; i++)
		{
			tempTextName.Format("MapLineName%d",( i + 1 ));
			tempButtonName.Format("MapLine%d",( i + 1 ));
			tempImageName.Format("MapLineState%d", ( i + 1));

			tempText = gGlobal.pUIManager->FindUIStaticText(tempTextName.c_str());
			tempButton = gGlobal.pUIManager->FindUIButton(tempButtonName.c_str());
			tempImage = gGlobal.pUIManager->FindUIImage(tempImageName.c_str());

			tempText->SetEnable(false);
			tempButton->SetEnable(false);
			tempImage->SetEnable(false);
		}

		if (gGlobal.g_pMap)
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(),"SelectMapLine", String(), gGlobal.g_pMap->GetLine_id());
		}

		gGlobal.pUIManager->AddDirtyUIItem(panel);
	}	
}

void CLoginProcess::ShowRemoveCharacterName()
{
	UIStaticText* name = (UIStaticText*)gGlobal.pUIManager->GetUIItem("ConfirmDeleteCharacterText");
	CCharacterInfo *pCharData = gGlobal.g_pAccountControl->m_characters->GetChild<CCharacterInfo>(m_nCharacterIndex);
	if (name && pCharData)
	{
		String nameString;
		nameString.Format(gGlobal.GetStringTable()->Get(_T("MSE_SYS_REMOVE_CHARACTER")), pCharData->GetNickName().c_str());
		name->SetText(nameString.c_str());
		name->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(name);
	}
}
