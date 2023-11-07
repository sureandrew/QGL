//-- Common
#include "Common.h"
#include "QGL.h"
#include "Common/common_binary.h"
//-- Self
#include "MenuCharProcess.h"
//-- Library
#include "QGL/Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "Proxy/Logic/Character.h"
#include "Proxy/Logic/BatActor.h"
#include "QGL/CharacterControl.h"
#include "MenuCharacterSimulator.h"
#include "QGLApplication.h"
#include "QGL/BattleControl.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/Item.h"
#include "MenuHelpProcess.h"
#include "Resource/ResFaction.h"



void CMenuCharProcess::UpdateStaticText(StringA fieldName, String value, Color32 textColor)
{
	UIStaticText* pST = gGlobal.pUIManager->FindUIStaticText(fieldName.c_str());

	if (pST )
	{
		pST->SetText(value);
		if (textColor != 0)
			pST->SetColor(textColor);
		else
			pST->SetColor(TextColor[ColorIndex_White]);
		pST->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pST);
	}
}

void CMenuCharProcess::UpdateCharFaction()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	String Text;
	if (pchar)
	{
		switch ( pchar->GetFaction())
		{
		case 1:
			Text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_SIXFANDOOR"));
			break;
		case 2:
			Text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_TWELVEDOCK"));
			break;
		case 3:
			Text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_HOLY"));
			break;
		case 4:
			Text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_SILVERCASINO"));
			break;
		case 5:
			Text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_TONGDOOR"));
			break;
		case 6:
			Text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_10THOUSAND"));
			break;
		case 7:
			Text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_GREENCLOTH"));
			break;
		case 8:
			Text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_GHOSTAREA"));
			break;
		default:
			Text = _T("");
			
		}

		UpdateStaticText("CharFaction", Text);
	}
}

void CMenuCharProcess::UpdateCharFactionDonate()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		String Text;
		Text.Format(_T("%i"), pchar->GetFDonate());
		UpdateStaticText("CharFactionDonate", Text);
	}
}

void CMenuCharProcess::UpdateCharClass()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	String Text;
	StringA tempClassName;

	if (pchar)
	{		
		for (Index i = C_INDEX(1); i <= 8;  ++i)
		{
			StringA temp;
			temp.Format("Class%dIm", i);
			UIImage * pIm = gGlobal.pUIManager->FindUIImage(temp.c_str());
			if (pIm)
			{
				if (pchar->GetSexClass() == i)
					pIm->SetEnable(true);
				else
					pIm->SetEnable(false);
			}
		}
	}
}

void CMenuCharProcess::UpdateCharFace()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	UIImage * pIm = gGlobal.pUIManager->FindUIImage("CharFace");
	StringA TextA;

	if (pchar && pIm)
	{
		TextA.Format("Data/faceL%05ia.tga", pchar->GetSexClass());
		pIm->SetImageFileA(TextA.c_str(), 0, 0, 0, 0);
		gGlobal.pUIManager->AddDirtyUIItem(pIm);
	}
}

void CMenuCharProcess::UpdateCharExp()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	UILinerImage* pLI = gGlobal.pUIManager->FindUILinerImage("CharExpBar");
	UIButton* pBtn = gGlobal.pUIManager->FindUIButton("CharLevelUp");

	UIAniIcon* pAin = gGlobal.pUIManager->FindUIAniIcon("LevelUpSpark");
	UIAniIcon* pAinFace = gGlobal.pUIManager->FindUIAniIcon("MyFaceSpark");
	if (pAin && gGlobal.g_pCharacter->GetExp() > gGlobal.g_pCharacter->GetNextLvExp() && (gGlobal.g_pCharacter->GetLevel()+1) == 10 && pAinFace->IsEnable())
	{
		if(!pAin->GetAniEnt()->HasAniFile())
			pAin->SetAnimationData(String(_T("MainButtonSZ.ent")).c_str());
		if(!pAin->IsEnable())
			pAin->SetEnable(true);
	}

	if (pchar && pLI)
	{
		String Text;

		Text.Format(_T("%i/%i"), pchar->GetExp(), pchar->GetNextLvExp() );
		UpdateStaticText("CharExp", Text);

		if (pchar->GetLevel() < 90)
		{
			if (pchar->GetExp() >= pchar->GetNextLvExp() )
			{
				pLI->SetDenumerator(1);
				pLI->SetNumerator(1);

				if (pBtn && gGlobal.g_pCharacter->GetCharNetObject()->GetAction() != CMapActor::ST_BATTLE && (pchar->GetLevel()+1) >=10)
				{
					pBtn->SetNormalState();
					gGlobal.pUIManager->AddDirtyUIItem(pBtn);
				}
				gGlobal.g_pMenuHelpProcess->HandleEvent(HELP_TIPS_FIRST_ENOUGH_EXP);
			}
			else
			{
				pLI->SetDenumerator(pchar->GetNextLvExp());
				pLI->SetNumerator(pchar->GetExp());

				if (pBtn)
				{
					pBtn->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(pBtn);
				}
			}
		}
		else
		{
			if (pchar->GetExp() >= pchar->GetNextLvExp() )
			{
				pLI->SetDenumerator(1);
				pLI->SetNumerator(1);
			}
			else
			{
				pLI->SetDenumerator(pchar->GetNextLvExp());
				pLI->SetNumerator(pchar->GetExp());
			}

			if (pBtn)
			{
				pBtn->SetDisableState();
				gGlobal.pUIManager->AddDirtyUIItem(pBtn);
			}
		}
		gGlobal.pUIManager->AddDirtyUIItem(pLI);

	}

	if (pchar)
	{
		gGlobal.SetUILinerImage("CharDownExp", pchar->GetNextLvExp(), pchar->GetExp());
	}

}

void CMenuCharProcess::UpdateCharGoodness()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		String Text;		
		Text.Format(_T("%i"), pchar->GetGoodness());
		UpdateStaticText("CharGoodness", Text);
	}
}

void CMenuCharProcess::UpdateCharNickName()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		String Text;
		Text = pchar->GetNickName();
		UpdateStaticText("CharNickName", Text);
	}
}

void CMenuCharProcess::UpdateCharHonor()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		String Text;
		Text.Format(_T("%i"), pchar->GetHonor());
		UpdateStaticText("CharHonor", Text);
	}
}

void CMenuCharProcess::UpdateCharLevel()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		String Text;
		Text.Format(_T("%i"), pchar->GetLevel());
		UpdateStaticText("CharLevel", Text);
	}
}

void CMenuCharProcess::UpdateHPBar()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		if (QGLApplication::GetInstance().GetMapStateID() != ST_APP_BATTLE)
			gGlobal.SetDisplayHP(pchar->GetHP(), pchar->GetMHP(), pchar->GetMaxHP());
		else
		{
			if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
				gGlobal.SetDisplayHP(gGlobal.g_pBattleControl->GetControlPlayer()->GetBattleHp(), gGlobal.g_pBattleControl->GetControlPlayer()->GetBattleMHp(), 
				gGlobal.g_pBattleControl->GetControlPlayer()->GetBattleMaxHp());
			else
				gGlobal.SetDisplayHP(pchar->GetHP(), pchar->GetMHP(), pchar->GetMaxHP());
		}
	}
}

void CMenuCharProcess::UpdateSPBar()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		if (QGLApplication::GetInstance().GetMapStateID() != ST_APP_BATTLE)
			gGlobal.SetDisplaySP(pchar->GetSP(), pchar->GetMSP(), pchar->GetMaxSP());
		else
		{
			if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
				gGlobal.SetDisplaySP(gGlobal.g_pBattleControl->GetControlPlayer()->GetSP(), gGlobal.g_pBattleControl->GetControlPlayer()->GetMSP(), pchar->GetMaxSP());
			else
				gGlobal.SetDisplaySP(pchar->GetSP(), pchar->GetMSP(), pchar->GetMaxSP());
		}
	}
}

void CMenuCharProcess::UpdateDPBar()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		if (QGLApplication::GetInstance().GetMapStateID() != ST_APP_BATTLE)
			gGlobal.SetDisplayDP(pchar->GetDP(), pchar->GetMaxDP());
		else
		{
			if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
				gGlobal.SetDisplayDP(gGlobal.g_pBattleControl->GetControlPlayer()->GetDP(), pchar->GetMaxDP());
			else
				gGlobal.SetDisplayDP(pchar->GetDP(), pchar->GetMaxDP());
		}
	}
}

void CMenuCharProcess::UpdateWCBar()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		if (QGLApplication::GetInstance().GetMapStateID() != ST_APP_BATTLE)
			gGlobal.SetDisplayWC(pchar->GetWC(), pchar->GetMaxWC(), pchar->GetWcExp(), pchar->GetNextWCExp());
		else
		{
			if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
				gGlobal.SetDisplayWC(gGlobal.g_pBattleControl->GetControlPlayer()->GetWC(), pchar->GetMaxWC()
				, gGlobal.g_pBattleControl->GetControlPlayer()->GetWcExp(), pchar->GetNextWCExp());
			else
				gGlobal.SetDisplayWC(pchar->GetWC(), pchar->GetMaxWC(), pchar->GetWcExp(), pchar->GetNextWCExp());

		}
	}
}

void CMenuCharProcess::UpdateVPBar()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		if (QGLApplication::GetInstance().GetMapStateID() != ST_APP_BATTLE)
			gGlobal.SetDisplayVP(pchar->GetVP(), pchar->GetMaxVP());
		else
		{
			if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
				gGlobal.SetDisplayVP(gGlobal.g_pBattleControl->GetControlPlayer()->GetVP(), pchar->GetMaxVP());
			else
				gGlobal.SetDisplayVP(pchar->GetVP(), pchar->GetMaxVP());
		}
	}
}

void CMenuCharProcess::UpdateCharAttr1()
{
	//find batactor
	CBatActor * pBatActor = NULL;
	if (gGlobal.g_pBattleControl->GetControlPlayer())
		pBatActor = gGlobal.g_pBattleControl->GetControlPlayer()->GetBatActorObject();

	if (pBatActor == NULL)
	{
		CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
		if (pchar)
		{

			UInt32 modValue = 0;
			String Text;

			//ATK
						
			modValue = gGlobal.g_pCharacter->GetWeaponBuffModValue(BUFF_MOD_VALUE_ATK);
			if (modValue > 0)
				Text.Format(_T("%i+%i"), pchar->GetATK() - modValue, modValue);
			else
				Text.Format(_T("%i"), pchar->GetATK());
			UpdateStaticText("CharATK", Text);

			//DEF
			modValue = gGlobal.g_pCharacter->GetWeaponBuffModValue(BUFF_MOD_VALUE_DEF);
			if (modValue > 0)
				Text.Format(_T("%i+%i"), pchar->GetDEF() - modValue, modValue);
			else
				Text.Format(_T("%i"), pchar->GetDEF());
			UpdateStaticText("CharDEF", Text);

			//HIT
			modValue = gGlobal.g_pCharacter->GetWeaponBuffModValue(BUFF_MOD_VALUE_HIT);
			if (modValue > 0)
				Text.Format(_T("%i+%i"), pchar->GetHIT() - modValue, modValue);
			else
				Text.Format(_T("%i"), pchar->GetHIT());
			UpdateStaticText("CharHIT", Text);

			//EVA
			modValue = gGlobal.g_pCharacter->GetWeaponBuffModValue(BUFF_MOD_VALUE_EVA);
			if (modValue > 0)
				Text.Format(_T("%i+%i"), pchar->GetEVA() - modValue, modValue);
			else
				Text.Format(_T("%i"), pchar->GetEVA());
			UpdateStaticText("CharEVA", Text);

			//PWD
			modValue = gGlobal.g_pCharacter->GetWeaponBuffModValue(BUFF_MOD_VALUE_PWD);
			if (modValue > 0)
				Text.Format(_T("%i+%i"), pchar->GetPOW() - modValue, modValue);
			else				
				Text.Format(_T("%i"), pchar->GetPOW());
			UpdateStaticText("CharPWD", Text);

			//SPD
			modValue = gGlobal.g_pCharacter->GetWeaponBuffModValue(BUFF_MOD_VALUE_SPD);
			if (modValue > 0)
				Text.Format(_T("%i+%i"), pchar->GetSPD() - modValue, modValue);
			else				
				Text.Format(_T("%i"), pchar->GetSPD());
			UpdateStaticText("CharSPD", Text);

		}
	}
	else
	{
		String Text;
		UInt32 modValue = 0;

		//ATK
		modValue = gGlobal.g_pBattleControl->GetControlPlayer()->GetWeaponBuffModValue(BUFF_MOD_VALUE_ATK);
		if (modValue > 0)
			Text.Format(_T("%i+%i"), pBatActor->GetATK() - modValue, modValue);
		else
			Text.Format(_T("%i"), pBatActor->GetATK());
		UpdateStaticText("CharATK", Text);

		//DEF
		modValue = gGlobal.g_pBattleControl->GetControlPlayer()->GetWeaponBuffModValue(BUFF_MOD_VALUE_DEF);
		if (modValue > 0)
			Text.Format(_T("%i+%i"), pBatActor->GetDEF() - modValue, modValue);
		else
			Text.Format(_T("%i"), pBatActor->GetDEF());
		UpdateStaticText("CharDEF", Text);

		//HIT
		modValue = gGlobal.g_pBattleControl->GetControlPlayer()->GetWeaponBuffModValue(BUFF_MOD_VALUE_HIT);
		if (modValue > 0)
			Text.Format(_T("%i+%i"), pBatActor->GetHIT() - modValue, modValue);
		else
		Text.Format(_T("%i"), pBatActor->GetHIT());
		UpdateStaticText("CharHIT", Text);

		//EVA
		modValue = gGlobal.g_pBattleControl->GetControlPlayer()->GetWeaponBuffModValue(BUFF_MOD_VALUE_EVA);
		if (modValue > 0)
			Text.Format(_T("%i+%i"), pBatActor->GetEVA() - modValue, modValue);
		else
		Text.Format(_T("%i"), pBatActor->GetEVA());
		UpdateStaticText("CharEVA", Text);

		//PWD
		modValue = gGlobal.g_pBattleControl->GetControlPlayer()->GetWeaponBuffModValue(BUFF_MOD_VALUE_PWD);
		if (modValue > 0)
			Text.Format(_T("%i+%i"), pBatActor->GetPOW() - modValue, modValue);
		else
		Text.Format(_T("%i"), pBatActor->GetPOW());
		UpdateStaticText("CharPWD", Text);

		//SPD
		modValue = gGlobal.g_pBattleControl->GetControlPlayer()->GetWeaponBuffModValue(BUFF_MOD_VALUE_SPD);
		if (modValue > 0)
			Text.Format(_T("%i+%i"), pBatActor->GetSPD() - modValue, modValue);
		else
		Text.Format(_T("%i"), pBatActor->GetSPD());
		UpdateStaticText("CharSPD", Text);
	}
}

void CMenuCharProcess::UpdateCharAttr2()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		String Text;
		Int16 addStr = 0;
		Int16 addCon = 0;
		Int16 addSta = 0;
		Int16 addAgi = 0;
		Int16 addSpr = 0;

		for (Index i = C_INDEX(0); i < 9; ++i)
		{
			CItem* item = gGlobal.m_pBag->GetItem(120 + i);
			if (item && item->GetEndure() > 0)
			{
				addStr += item->GetBase_STR() + item->GetUpg_STR() + item->GetInlay_STR();
				addCon += item->GetBase_CON() + item->GetUpg_CON() + item->GetInlay_CON();
				addSta += item->GetBase_STA() + item->GetUpg_STA() + item->GetInlay_STA();
				addAgi += item->GetBase_AGI() + item->GetUpg_AGI() + item->GetInlay_AGI();
				addSpr += item->GetBase_SPR() + item->GetUpg_SPR() + item->GetInlay_SPR();
			}
		}

		// for item set
		gGlobal.m_pBag->CalcItemSetBasicAttrib(addSta, addSpr, addStr, addCon, addAgi);
		
		//STR
		UInt32 modValue = 0;
		modValue = gGlobal.g_pCharacter->GetWeaponBuffModValue(BUFF_MOD_VALUE_STR);

		Int16 str = pchar->GetSTR() + addStr;
		if (str < 0)
			str = 0;
		if( gGlobal.g_pMenuCharSimulator && gGlobal.g_pMenuCharSimulator->GetStrAdd() )
			Text.Format(_T("%i+%i"), str , gGlobal.g_pMenuCharSimulator->GetStrAdd() + modValue);
		else
			Text.Format(_T("%i"), str);
		if (addStr > 0 )
			UpdateStaticText("CharSTR", Text.Left(9), TextColor[ColorIndex_Green]);
		else if (addStr < 0 )
			UpdateStaticText("CharSTR", Text.Left(9), ItemTextColor[ItemColorIndex_Red]);
		else
			UpdateStaticText("CharSTR", Text.Left(9));

		//CON
		modValue = gGlobal.g_pCharacter->GetWeaponBuffModValue(BUFF_MOD_VALUE_CON);
		Int16 con = pchar->GetCON() + addCon;
		if (con < 0)
			con = 0;
		if( gGlobal.g_pMenuCharSimulator && gGlobal.g_pMenuCharSimulator->GetConAdd() )
			Text.Format(_T("%i+%i"), con, gGlobal.g_pMenuCharSimulator->GetConAdd() + modValue);
		else
			Text.Format(_T("%i"), con);

		if (addCon > 0 )
			UpdateStaticText("CharCON", Text.Left(9), TextColor[ColorIndex_Green]);
		else if (addCon < 0 )
			UpdateStaticText("CharCON", Text.Left(9), ItemTextColor[ItemColorIndex_Red]);
		else
			UpdateStaticText("CharCON", Text.Left(9));

		//STA
		modValue = gGlobal.g_pCharacter->GetWeaponBuffModValue(BUFF_MOD_VALUE_STA);
		Int16 sta = pchar->GetSTA() + addSta;
		if (sta < 0)
			sta = 0;
		if( gGlobal.g_pMenuCharSimulator && gGlobal.g_pMenuCharSimulator->GetStaAdd() )
			Text.Format(_T("%i+%i"), sta, gGlobal.g_pMenuCharSimulator->GetStaAdd() + modValue);
		else
			Text.Format(_T("%i"), sta);

		if (addSta > 0 )
			UpdateStaticText("CharSTA", Text.Left(9), TextColor[ColorIndex_Green]);
		else if (addSta < 0 )
			UpdateStaticText("CharSTA", Text.Left(9), ItemTextColor[ItemColorIndex_Red]);
		else
			UpdateStaticText("CharSTA", Text.Left(9));
		

		//AGI
		modValue = gGlobal.g_pCharacter->GetWeaponBuffModValue(BUFF_MOD_VALUE_AGI);
		Int16 agi = pchar->GetAGI() + addAgi;
		if (agi < 0)
			agi = 0;
		if( gGlobal.g_pMenuCharSimulator && gGlobal.g_pMenuCharSimulator->GetAgiAdd() )
			Text.Format(_T("%i+%i"), agi, gGlobal.g_pMenuCharSimulator->GetAgiAdd() + modValue);
		else
			Text.Format(_T("%i"), agi);

		if (addAgi > 0 )
			UpdateStaticText("CharAGI", Text.Left(9), TextColor[ColorIndex_Green]);
		else if (addAgi < 0 )
			UpdateStaticText("CharAGI", Text.Left(9), ItemTextColor[ItemColorIndex_Red]);
		else
			UpdateStaticText("CharAGI", Text.Left(9));

		//SPR
		modValue = gGlobal.g_pCharacter->GetWeaponBuffModValue(BUFF_MOD_VALUE_SPR);
		Int16 spr = pchar->GetSPR() + addSpr;
		if (spr < 0)
			spr = 0;
		if( gGlobal.g_pMenuCharSimulator && gGlobal.g_pMenuCharSimulator->GetSprAdd() )
			Text.Format(_T("%i+%i"), spr, gGlobal.g_pMenuCharSimulator->GetSprAdd() + modValue);
		else
			Text.Format(_T("%i"), spr);

		if (addSpr > 0 )
			UpdateStaticText("CharSPR", Text.Left(9), TextColor[ColorIndex_Green]);
		else if (addSpr < 0 )
			UpdateStaticText("CharSPR", Text.Left(9), ItemTextColor[ItemColorIndex_Red]);
		else
			UpdateStaticText("CharSPR", Text.Left(9));
		
	}
}

void CMenuCharProcess::UpdateAttrPoint()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		String Text;

		if( gGlobal.g_pMenuCharSimulator )
			Text.Format(_T("%i"), pchar->GetAttrPoint() - gGlobal.g_pMenuCharSimulator->GetAttribMinus());
		else
			Text.Format(_T("%i"), pchar->GetAttrPoint());

		UpdateStaticText("Charpotential", Text);

		UpdateAttriButton();
	}
}

void CMenuCharProcess::UpdateCharTitle()
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		String Text;
		Text = pchar->GetTitle();
		UpdateStaticText("CharTitle", Text);
	}
}

void CMenuCharProcess::UpdateAttriButton()
{
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
	{
		UIButton* button = NULL;
		if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE || gGlobal.g_pCharacter->GetCharNetObject()->GetAttrPoint() == 0 )
		{
			button = gGlobal.pUIManager->FindUIButton("CharSTRPlus");
			if (button)
				button->SetDisableState();
			button = gGlobal.pUIManager->FindUIButton("CharSTRMinus");
			if (button)
				button->SetDisableState();
			button = gGlobal.pUIManager->FindUIButton("CharCONPlus");
			if (button)
				button->SetDisableState();
			button = gGlobal.pUIManager->FindUIButton("CharCONMinus");
			if (button)
				button->SetDisableState();
			button = gGlobal.pUIManager->FindUIButton("CharSTAPlus");
			if (button)
				button->SetDisableState();
			button = gGlobal.pUIManager->FindUIButton("CharSTAMinus");
			if (button)
				button->SetDisableState();
			button = gGlobal.pUIManager->FindUIButton("CharAGIPlus");
			if (button)
				button->SetDisableState();
			button = gGlobal.pUIManager->FindUIButton("CharAGIMinus");
			if (button)
				button->SetDisableState();
			button = gGlobal.pUIManager->FindUIButton("CharSPRPlus");
			if (button)
				button->SetDisableState();
			button = gGlobal.pUIManager->FindUIButton("CharSPRMinus");
			if (button)
				button->SetDisableState();

			button = gGlobal.pUIManager->FindUIButton("CharAttOk");
			if (button)
				button->SetDisableState();

			if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
			{
				button = gGlobal.pUIManager->FindUIButton("CharLevelUp");
				if (button)
					button->SetDisableState();
			}
		}
		else
		{
			if (gGlobal.g_pCharacter->GetCharNetObject()->GetAttrPoint() >= 0)
			{
				button = gGlobal.pUIManager->FindUIButton("CharSTRPlus");
				if (button)
					button->SetNormalState();
				button = gGlobal.pUIManager->FindUIButton("CharSTRMinus");
				if (button)
					button->SetNormalState();
				button = gGlobal.pUIManager->FindUIButton("CharCONPlus");
				if (button)
					button->SetNormalState();
				button = gGlobal.pUIManager->FindUIButton("CharCONMinus");
				if (button)
					button->SetNormalState();
				button = gGlobal.pUIManager->FindUIButton("CharSTAPlus");
				if (button)
					button->SetNormalState();
				button = gGlobal.pUIManager->FindUIButton("CharSTAMinus");
				if (button)
					button->SetNormalState();
				button = gGlobal.pUIManager->FindUIButton("CharAGIPlus");
				if (button)
					button->SetNormalState();
				button = gGlobal.pUIManager->FindUIButton("CharAGIMinus");
				if (button)
					button->SetNormalState();
				button = gGlobal.pUIManager->FindUIButton("CharSPRPlus");
				if (button)
					button->SetNormalState();
				button = gGlobal.pUIManager->FindUIButton("CharSPRMinus");
				if (button)
					button->SetNormalState();

				if( gGlobal.g_pMenuCharSimulator->GetAttribMinus() > 0 )
				{
					button = gGlobal.pUIManager->FindUIButton("CharAttOk");
					if (button)
						button->SetNormalState();
				}

				if (gGlobal.g_pCharacter->GetCharNetObject()->GetExp() >= gGlobal.g_pCharacter->GetCharNetObject()->GetNextLvExp())
				{
					button = gGlobal.pUIManager->FindUIButton("CharLevelUp");
					if (button)
						button->SetNormalState();
				}
			}
		}
	}
}

void CMenuCharProcess::UpdateCharMenu()
{
	VMProtectBegin; VM_START;

	UpdateAttrPoint();
	UpdateCharAttr1();
	UpdateCharAttr2();
	UpdateCharClass();
	UpdateCharExp();
	UpdateCharFace();
	UpdateCharFaction();
	UpdateCharFactionDonate();
	UpdateCharGoodness();
	UpdateCharHonor();
	UpdateCharLevel();
	UpdateCharNickName();
	UpdateCharTitle();
	UpdateDPBar();
	UpdateHPBar();
	UpdateSPBar();
	UpdateVPBar();
	UpdateWCBar();
	UpdateAttriButton();

	VMProtectEnd; VM_END;
}

void CMenuCharProcess::OpenFactionDetail()
{
	if (gGlobal.g_pCharacter == NULL ||
		gGlobal.g_pCharacter->GetCharNetObject() == NULL)
		return;

	const FactionData * pFactionData = gGlobal.m_resource->GetFactionData(gGlobal.g_pCharacter->GetCharNetObject()->GetFaction());

	if (pFactionData == NULL)
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseFactionDetailMenu", String());
		return;
	}

	UIPanel * pPanel = gGlobal.pUIManager->FindUIPanel("FactionDetailMenuPanel");
	if (pPanel == NULL)
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseFactionDetailMenu", String());
		return;		
	}

	gGlobal.pUIManager->SetPanel2Top(pPanel);
	

	/*UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(SkillDetailName.c_str());
	if (pText)
	{
		pText->SetText(m_pTempSkillDetailData->name);
		pText->Redraw();
	}*/

	UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("FactionDetailName");
	if (pText)
	{
		pText->SetText(pFactionData->Faction_Name);
		pText->Redraw();
	}

	pText = gGlobal.pUIManager->FindUIStaticText("FactionDetailDonate");
	if (pText)
	{
		String temp;
		temp.Format(_T("%d"), gGlobal.g_pCharacter->GetCharNetObject()->GetFDonate());
		pText->SetText(temp.c_str());
		pText->Redraw();
	}

	pText = gGlobal.pUIManager->FindUIStaticText("FactionDetailChar1");
	if (pText)
	{
		String charlist1;
		String charlist2;

		UInt count = 0;

		if (wcsncmp(pFactionData->Faction_class1,  _T("0"), 1) != 0)
		{
			charlist1 += pFactionData->Faction_class1;
			charlist1 += _T(" ");
			count ++;
		}
		if (wcsncmp(pFactionData->Faction_class2,  _T("0"), 1) != 0)
		{
			charlist1 += pFactionData->Faction_class2;
			charlist1 += _T(" ");
			count ++;
		}
		if (wcsncmp(pFactionData->Faction_class3,  _T("0"), 1) != 0)
		{
			charlist1 += pFactionData->Faction_class3;
			charlist1 += _T(" ");
			count ++;
		}
		if (wcsncmp(pFactionData->Faction_class4,  _T("0"), 1) != 0)
		{
			if (count >= 3)
			{
				charlist2 += pFactionData->Faction_class4;
				charlist2 += _T(" ");
			}
			else
			{
				charlist1 += pFactionData->Faction_class4;
				charlist1 += _T(" ");
			}
			count ++;
		}
		if (wcsncmp(pFactionData->Faction_class5,  _T("0"), 1) != 0)
		{
			if (count >= 3)
			{
				charlist2 += pFactionData->Faction_class5;
				charlist2 += _T(" ");
			}
			else
			{
				charlist1 += pFactionData->Faction_class5;
				charlist1 += _T(" ");
			}
			count ++;
		}
		if (wcsncmp(pFactionData->Faction_class6,  _T("0"), 1) != 0)
		{
			if (count >= 3)
			{
				charlist2 += pFactionData->Faction_class6;
				charlist2 += _T(" ");
			}
			else
			{
				charlist1 += pFactionData->Faction_class6;
				charlist1 += _T(" ");
			}
			count ++;
		}
		pText->SetText(charlist1.c_str());
		pText->Redraw();

		pText = gGlobal.pUIManager->FindUIStaticText("FactionDetailChar2");
		pText->SetText(charlist2.c_str());
		pText->Redraw();
	}

	pText = gGlobal.pUIManager->FindUIStaticText("FactionDetailTalent");
	if (pText)
	{
		pText->SetText(pFactionData->Faction_Talent);
		pText->Redraw();
	}

	UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("FactionDetailDes");
	if (pChat)
	{
		pChat->ClearAllRow();
		pChat->AddStringW(pFactionData->Faction_Description);
		pChat->EndRow();
	}

	UIAniIcon * pAni = gGlobal.pUIManager->FindUIAniIcon("FactionDetailIcon");
	if (pAni)
	{
		pAni->ClearAniData();
		pAni->SetAnimationData(pFactionData->Faction_Logo);
	}

	Int MouseX = gGlobal.pUIManager->GetCursorX();
	Int MouseY = gGlobal.pUIManager->GetCursorY();

	if ( (pPanel->GetWidth() + MouseX + 20) > 800 )
		MouseX = 800 - 20 - pPanel->GetWidth();
	if ( (pPanel->GetHeight() + MouseY) > 600 )
		MouseY = 600 - pPanel->GetHeight();

	pPanel->SetPos(MouseX + 20, MouseY);

	pPanel->SetEnable(true);

	gGlobal.pUIManager->AddDirtyUIItem(pPanel);
}
