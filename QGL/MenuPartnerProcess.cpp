//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuPartnerProcess.h"
//-- Library
#include "QGL/Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/SkillGroup.h"
#include "Proxy/Logic/ItemBag.h"
#include "common_binary.h"
#include "QGL/CharacterControl.h"
#include "QGL/BattleControl.h"
#include "Resource/ResMob.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResSkill.h"
#include "Proxy/Logic/Skill.h"
#include "QGLApplication.h"
#include "Resource/ResSkillCombination.h"
#include "Resource/ResPartnerCollection.h"
#include "MenuHawkerProcess.h"


#define NUMOFSHOWPARTNER 4
#define MAX_PARTNER_COLLECTION_EACH_PAGE	15
#define MAX_PARTNER_COLLECTION_LIGHT	5

CMenuPartnerProcess::CMenuPartnerProcess()
{
	m_pOnShowPartner = NULL;
	m_iStartIndex = 0;
	m_iSelectedIndex = 0;
	m_bRemoving = false;
	m_iSTRAdd = 0;
	m_iCONAdd = 0;
	m_iSTAAdd = 0;
	m_iAGIAdd = 0;
	m_iSPRAdd = 0;
	m_iTempSelSkillID = 0;
	m_pTempSelSkillData = NULL;
	m_iPage = 1;
}

void CMenuPartnerProcess::UpdatePartnerMenu()
{
	if (gGlobal.g_pPartnerGroup)
	{
		if (gGlobal.g_pPartnerGroup->GetCount() > 0)
		{
			if (m_pOnShowPartner == NULL)
			{
				if (gGlobal.g_pPartnerGroup->GetCount() > m_iSelectedIndex)
					m_pOnShowPartner = gGlobal.g_pPartnerGroup->GetChild<CPartner>(m_iSelectedIndex);
				else
				{
					m_pOnShowPartner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(0);
					m_iSelectedIndex = 0;
				}
			}
			UIPanel* mainPanel = (UIPanel*)gGlobal.pUIManager->GetUIItem("PartnerMainPanel");
			UIPanel* descPanel = (UIPanel*)gGlobal.pUIManager->GetUIItem("PartnerDescPanel");
			UIPanel* skillPanel = (UIPanel*)gGlobal.pUIManager->GetUIItem("PartnerSkillPanel");

			if (mainPanel && descPanel && skillPanel)
			{
									
				UpdatePartnerModel();				
				UpdatePartnerNameList();

				UpdateHP();
				UpdateSP();
				UpdateExp();
				
				UpdateLevel();
				UpdateGoodness();
				UpdateFavor();

				UpdateAttrib2();
				UpdateAttrib1();
				UpdateAttrPoint();

				UpdateStateButton();
				UpdateAllButtonState();
				UpdateSoulBoundAndUnique();

				UpdateReqLevelAndRemain();
				UpdateRaceType();

				if (QGLApplication::GetInstance().GetMapStateID() != ST_APP_BATTLE)
				{
					UpdateSkillMenu(m_pOnShowPartner);
					UpdatePartnerDesc(m_pOnShowPartner);
				}

				UpdateNative();
				UpdateHealth();

				UpdateCultivation();
				UpdatePractice();

				gGlobal.pUIManager->AddDirtyUIItem(mainPanel);
				gGlobal.pUIManager->AddDirtyUIItem(descPanel);
				gGlobal.pUIManager->AddDirtyUIItem(skillPanel);
			}
		}
		else
		{
			UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("Partner5Text");
			if (pText)
			{
				String text;
				text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CARRYING_NUMBER")), gGlobal.g_pPartnerGroup->GetCount(),
					gGlobal.g_pCharacter->GetCharNetObject()->GetMaxPartner());
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
		}
		UIButton* UseBuffTemp = (UIButton*)gGlobal.pUIManager->GetUIItem("PartnerCollectionUseBuffTemp");
		UIButton* UnUseBuffTemp = (UIButton*)gGlobal.pUIManager->GetUIItem("PartnerCollectionUnUseBuffTemp");
		if (UseBuffTemp && UnUseBuffTemp)
		{
			UseBuffTemp->SetEnable(gGlobal.m_bUsePartnerCollectionBuff);
			UnUseBuffTemp->SetEnable(!gGlobal.m_bUsePartnerCollectionBuff);
			if (!m_PartnerCollectionMap.empty())
			{
				SetButtonState("PartnerCollectionUseBuffTemp", true);
			}
			else
			{
				SetButtonState("PartnerCollectionUseBuffTemp", false);
			}
		}
		
	}
	//else
	//	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ClosePartner");
}

void CMenuPartnerProcess::UpdateSoulBoundAndUnique()
{
	if (m_pOnShowPartner)
	{
		UIImage* temp = NULL;
		UIImage* temp1 = NULL;

		temp = gGlobal.pUIManager->FindUIImage("PartnerIsBound");
		temp1 = gGlobal.pUIManager->FindUIImage("PartnerNotBound");
		if (temp && temp1)
		{
			if (m_pOnShowPartner->GetSoulboundState() == SOULBOUND_ALREADY_BOUND)
			{
				temp->SetEnable(true);
				temp1->SetEnable(false);
			}
			else
			{
				temp->SetEnable(false);
				temp1->SetEnable(true);
			}
		}
	}
}

void CMenuPartnerProcess::UpdatePartnerModel()
{
	if (m_pOnShowPartner && m_pOnShowPartner->m_data)
	{
		MobDB_Client * mobTemplate = m_pOnShowPartner->m_data;

		CAnimatedEntity temp;
		temp.InitEntPicInfo();
		temp.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
		String modelName = String(mobTemplate->m_basicinfo->model) ;
		temp.LoadEntityFile(modelName.c_str(), gGlobal.pD3DDevice);

		String colorFileName = modelName.substr(0, modelName.length() - 5);
		char abc = m_pOnShowPartner->GetOutLookColor() + 97;
		colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
		temp.ChangeColorP(modelName.c_str(), colorFileName.c_str());

		//temp.SetAnimationIndexByIndex(temp.GetBattleAnimation(), TRUE, SOUTHEAST);
		UIAniIcon* pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem("PartnerModel");
		if (pAniIcon)
		{				
			pAniIcon->ClearAniData();
			pAniIcon->SetAnimationData(&temp);
			pAniIcon->SetAnimationIndex(temp.GetBattleAnimation());				
			pAniIcon->SetRotation(SOUTHEAST);
			gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
		}
	}
}

void CMenuPartnerProcess::UpdateAttrib1()
{
	if (m_pOnShowPartner)
	{
		AttriStruct2 Attri;
		GetPartnerCollectionAttri(Attri);
		StringA name;
		name = "PartnerSTR";
		SetNumField(name, m_pOnShowPartner->GetSTR(), m_iSTRAdd, Attri.GetStr());

		name = "PartnerCON";
		SetNumField(name, m_pOnShowPartner->GetCON(), m_iCONAdd, Attri.GetCon());

		name = "PartnerSTA";
		SetNumField(name, m_pOnShowPartner->GetSTA(), m_iSTAAdd, Attri.GetSta());

		name = "PartnerAGI";
		SetNumField(name, m_pOnShowPartner->GetAGI(), m_iAGIAdd, Attri.GetAgi());

		name = "PartnerSPR";
		SetNumField(name, m_pOnShowPartner->GetSPR(), m_iSPRAdd, Attri.GetSpr());
	}
}

void CMenuPartnerProcess::UpdateAttrPoint()
{
	if (m_pOnShowPartner)
	{
		SetNumField("PartnerPOT", m_pOnShowPartner->GetAttrPoint());
		UpdateAllButtonState();
	}
}

void CMenuPartnerProcess::UpdateAttrib2()
{
	if (m_pOnShowPartner)
	{
		StringA name;
		name = "PartnerATK";
		Int32 value = (m_pOnShowPartner->GetBattleATK() > 0 ? m_pOnShowPartner->GetBattleATK():m_pOnShowPartner->GetATK());
		SetNumField(name, value);

		name = "PartnerDEF";
		value = (m_pOnShowPartner->GetBattleDEF() > 0 ? m_pOnShowPartner->GetBattleDEF():m_pOnShowPartner->GetDEF());
		SetNumField(name, value);

		name = "PartnerHIT";
		value = (m_pOnShowPartner->GetBattleHIT() > 0 ? m_pOnShowPartner->GetBattleHIT():m_pOnShowPartner->GetHIT());
		SetNumField(name, value);

		name = "PartnerEVA";
		value = (m_pOnShowPartner->GetBattleEVA() > 0 ? m_pOnShowPartner->GetBattleEVA():m_pOnShowPartner->GetEVA());
		SetNumField(name, value);

		name = "PartnerPWD";
		value = (m_pOnShowPartner->GetBattlePOW() > 0 ? m_pOnShowPartner->GetBattlePOW():m_pOnShowPartner->GetPOW());
		SetNumField(name, value);

		name = "PartnerSPD";
		value = (m_pOnShowPartner->GetBattleSPD() > 0 ? m_pOnShowPartner->GetBattleSPD():m_pOnShowPartner->GetSPD());
		SetNumField(name, value);
	}
}

void CMenuPartnerProcess::UpdateLevel()
{
	if (m_pOnShowPartner)
		SetNumField("PartnerLevel", m_pOnShowPartner->GetLevel());
}



void CMenuPartnerProcess::UpdateFavor()
{
	if (m_pOnShowPartner)
	{
		float temp = m_pOnShowPartner->GetFavor() - (UInt16)m_pOnShowPartner->GetFavor();

		if (temp > 0.1f)
			SetNumField("PartnerLovely", (UInt16)m_pOnShowPartner->GetFavor() + 1);
		else
			SetNumField("PartnerLovely", (UInt16)m_pOnShowPartner->GetFavor());
	}
}

void CMenuPartnerProcess::UpdateGoodness()
{
	if (m_pOnShowPartner)
		SetNumField("PartnerGoodness", m_pOnShowPartner->GetGoodness());
}

void CMenuPartnerProcess::UpdateHP()
{
	if (m_pOnShowPartner)
	{
		if ((gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP) 
			&& m_pOnShowPartner->GetBattleHP() >= 0)
			UpdateBar("PartnerHP", m_pOnShowPartner->GetBattleHP(), m_pOnShowPartner->GetBattleMaxHP());
		else
			UpdateBar("PartnerHP", m_pOnShowPartner->GetHP(), m_pOnShowPartner->GetMaxHP());
	}
}

void CMenuPartnerProcess::UpdateSP()
{
	if (m_pOnShowPartner)
	{
		if ((gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
			&& m_pOnShowPartner->GetBattleSP() >= 0)
			UpdateBar("PartnerSP", m_pOnShowPartner->GetBattleSP(), m_pOnShowPartner->GetMaxSP());
		else
			UpdateBar("PartnerSP", m_pOnShowPartner->GetSP(), m_pOnShowPartner->GetMaxSP());
	}
}

void CMenuPartnerProcess::UpdateExp()
{
	if (m_pOnShowPartner)
		UpdateBar("PartnerExp", m_pOnShowPartner->GetExp(), m_pOnShowPartner->GetNextLvExp());
}

void CMenuPartnerProcess::UpdateBar(StringA barNamePref, UInt32 nowValue, UInt32 maxValue )
{
	StringA fullName;

	fullName.Format("%sBar", barNamePref.c_str());

	String displayText;
	
	gGlobal.SetUILinerImage(fullName, maxValue, nowValue);

	displayText.Format(_T("%d/%d"), nowValue, maxValue);

	fullName.Format("%sText", barNamePref.c_str());

	UIStaticText* barText = (UIStaticText*)gGlobal.pUIManager->GetUIItem(fullName.c_str());
	if (barText)
	{
		barText->SetText(displayText.c_str());
		barText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(barText);
	}

}

void CMenuPartnerProcess::UpdatePartnerNameList()
{
	if (m_pOnShowPartner)
	{
		if (gGlobal.g_pPartnerGroup->GetCount() > 0)
		{
			StringA fieldName;
			UIImage* state;
			UIStaticText* partnerName;
			UIImage* clickImage;
			CPartner* partner;
			for (int i = 1; i <= NUMOFSHOWPARTNER; ++i)
			{
				for (int j = 1; j <= 3; ++j)
				{
					fieldName.Format("PartnerState%d%d", i, j);
					state = (UIImage*)gGlobal.pUIManager->GetUIItem(fieldName.c_str());
					if (state)
						state->SetEnable(false);
				}
				fieldName.Format("Partner%dText", i);
				partnerName = (UIStaticText*)gGlobal.pUIManager->GetUIItem(fieldName.c_str());
				if (partnerName)
					partnerName->SetEnable(false);

				fieldName.Format("Partner%d", i);
				clickImage = (UIImage*)gGlobal.pUIManager->GetUIItem(fieldName.c_str());
				if (clickImage)
					clickImage->SetEnable(false);

				fieldName.Format("SelectPartner%d", i);
				clickImage = (UIImage*)gGlobal.pUIManager->GetUIItem(fieldName.c_str());
				if (clickImage)
					clickImage->SetEnable(false);
                StringA lockPartnerImage;
				lockPartnerImage.Format("LockPartner%01d", i);
				UIImage * pLockIm = gGlobal.pUIManager->FindUIImage(lockPartnerImage.c_str());
				if (pLockIm)
					pLockIm->SetEnable(false);	

			}

			UInt count = NUMOFSHOWPARTNER;
			if (gGlobal.g_pPartnerGroup->GetCount() < NUMOFSHOWPARTNER)
				count = gGlobal.g_pPartnerGroup->GetCount();
			for (Index i = C_INDEX(0); i < count; ++i)
			{
				partner = gGlobal.g_pPartnerGroup->GetChild<CPartner>(i + (UInt16)m_iStartIndex);
				if (partner == NULL)
					continue;

				fieldName.Format("PartnerState%d%d", i+1, partner->GetState() + 1);
				state = (UIImage*)gGlobal.pUIManager->GetUIItem(fieldName.c_str());
				if (state)
				{
					state->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(state);
				}

				fieldName.Format("Partner%dText", i + 1);
				partnerName = (UIStaticText*)gGlobal.pUIManager->GetUIItem(fieldName.c_str());
				if (partnerName)
				{
					partnerName->SetText(partner->GetRaname().c_str());
					partnerName->Redraw();
					partnerName->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(partnerName);
				}

				if (i != (m_iSelectedIndex - m_iStartIndex) )
				{
					fieldName.Format("Partner%d", i + 1);
					clickImage = (UIImage*)gGlobal.pUIManager->GetUIItem(fieldName.c_str());
					if (clickImage)
						clickImage->SetEnable(true);
				}
				else
				{
					fieldName.Format("SelectPartner%d", i + 1);
					clickImage = (UIImage*)gGlobal.pUIManager->GetUIItem(fieldName.c_str());
					if (clickImage)
					{
						clickImage->SetEnable(true);
						gGlobal.pUIManager->AddDirtyUIItem(clickImage);
					}

				}
                
                StringA lockPartnerImage;
				lockPartnerImage.Format("LockPartner%01d", i + 1);
				UIImage * pLockIm = gGlobal.pUIManager->FindUIImage(lockPartnerImage.c_str());
				if (pLockIm)
					pLockIm->SetEnable(partner->GetLock());	

			}

			UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("Partner5Text");
			if (pText)
			{
				String text;
				text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CARRYING_NUMBER")), gGlobal.g_pPartnerGroup->GetCount(),
					gGlobal.g_pCharacter->GetCharNetObject()->GetMaxPartner());
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
		}
	}
}

void CMenuPartnerProcess::SetNumField(StringA fieldName, Int32 fieldValue, UInt16 addedValue)
{
	UIStaticText* field;
	String value;
	field = (UIStaticText*)gGlobal.pUIManager->GetUIItem(fieldName.c_str());
	
	if (field)
	{
		if (addedValue == 0)
			value.Format(_T("%d"), fieldValue);	
		else
			value.Format(_T("%d + %d"), fieldValue, addedValue);
		
		field->SetText(value.c_str());
		field->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(field);
	}
}

void CMenuPartnerProcess::SetNumField(StringA fieldName, Int32 fieldValue, UInt16 addedValue, Int16 PartnerCollectionValue)
{
	UIStaticText* field;
	String value;
	field = (UIStaticText*)gGlobal.pUIManager->GetUIItem(fieldName.c_str());

	if (field)
	{
		if (addedValue == 0)
			value.Format(_T("%d"), fieldValue + PartnerCollectionValue);	
		else
			value.Format(_T("%d + %d"), fieldValue + PartnerCollectionValue, addedValue);

		if (PartnerCollectionValue == 0)
			field->SetColor(TextColor[ColorIndex_White]);
		else if (PartnerCollectionValue > 0)
			field->SetColor(TextColor[ColorIndex_Green]);
		else
			field->SetColor(TextColor[ColorIndex_LightPurple]);

		field->SetText(value.c_str());
		field->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(field);
	}
}

void CMenuPartnerProcess::SetOnShowPartner(CPartner* partner)
{
	m_pOnShowPartner = partner;
}

void CMenuPartnerProcess::DeleteOnShowPartner()
{
	if (IsHawking())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_DROP_PARTNER")));
		return;
	}
	if (gGlobal.g_pPartnerGroup && m_pOnShowPartner && !IsRemoving())
	{
		if (m_pOnShowPartner->IsGiven())
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_GIVEN_CANT_DROP_PET")));
			return;
		}
		if (m_pOnShowPartner->GetLock())
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_LOCK_CANT_DROP_PARTNER")));
			return;
		}
		SetRemoving(true);
		RPCContext context;
		gGlobal.g_pPartnerGroup->DeletePartner(gGlobal.pPeerProxy, context, m_pOnShowPartner->GetPartner_id());
	}
}

bool CMenuPartnerProcess::IsRemoving()
{
	return m_bRemoving;
}

void CMenuPartnerProcess::ResetValue()
{
	m_pOnShowPartner = NULL;
	m_iStartIndex = 0;
	m_bRemoving = false;
	m_iSTRAdd = 0;
	m_iCONAdd = 0;
	m_iSTAAdd = 0;
	m_iAGIAdd = 0;
	m_iSPRAdd = 0;
	//m_iSelectedIndex = 0;
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("PartnerScrollBar");
	if (scrollBar)
	{
		scrollBar->SetNumCount(gGlobal.g_pPartnerGroup->GetCount());
		scrollBar->HomeScroll();
	}
}

void CMenuPartnerProcess::AttrUp(int index)
{
	if (m_pOnShowPartner)
	{
		if (m_pOnShowPartner->GetAttrPoint() > (m_iAGIAdd + m_iCONAdd + m_iSPRAdd + m_iSTAAdd + m_iSTRAdd ))
		{
			AttriStruct2 Attri;
			GetPartnerCollectionAttri(Attri);

			StringA name;
			switch(index)
			{
			case 0:
				++m_iSTRAdd;
				name = "PartnerSTR";
				SetNumField(name, m_pOnShowPartner->GetSTR(), m_iSTRAdd, Attri.GetStr());
				break;
			case 1:
				++m_iCONAdd;
				name = "PartnerCON";
				SetNumField(name, m_pOnShowPartner->GetCON(), m_iCONAdd, Attri.GetCon());
				break;
			case 2:
				++m_iSTAAdd;
				name = "PartnerSTA";
				SetNumField(name, m_pOnShowPartner->GetSTA(), m_iSTAAdd, Attri.GetSta());
				break;
			case 3:
				++m_iAGIAdd;
				name = "PartnerAGI";
				SetNumField(name, m_pOnShowPartner->GetAGI(), m_iAGIAdd, Attri.GetAgi());
				break;
			case 4:
				++m_iSPRAdd;
				name = "PartnerSPR";
				SetNumField(name, m_pOnShowPartner->GetSPR(), m_iSPRAdd, Attri.GetSpr());
				break;
			}

			name = "PartnerPOT";
			UInt16 attrPoint = m_pOnShowPartner->GetAttrPoint() - m_iSPRAdd - m_iAGIAdd - m_iSTAAdd - m_iCONAdd - m_iSTRAdd;
			SetNumField(name, attrPoint);
		}
	}
}

void CMenuPartnerProcess::AttrDown(int index)
{
	if (m_pOnShowPartner)
	{
		StringA name;

		AttriStruct2 Attri;
		GetPartnerCollectionAttri(Attri);

		switch(index)
		{
		case 0:
			if (m_iSTRAdd > 0)
			{
				--m_iSTRAdd;
				name = "PartnerSTR";
				SetNumField(name, m_pOnShowPartner->GetSTR(), m_iSTRAdd, Attri.GetStr());
			}
			break;
		case 1:
			if (m_iCONAdd > 0)
			{
				--m_iCONAdd;
				name = "PartnerCON";
				SetNumField(name, m_pOnShowPartner->GetCON(), m_iCONAdd, Attri.GetCon());
			}
			break;
		case 2:
			if (m_iSTAAdd > 0)
			{
				--m_iSTAAdd;
				name = "PartnerSTA";
				SetNumField(name, m_pOnShowPartner->GetSTA(), m_iSTAAdd, Attri.GetSta());
			}
			break;
		case 3:
			if (m_iAGIAdd > 0)
			{
				--m_iAGIAdd;
				name = "PartnerAGI";
				SetNumField(name, m_pOnShowPartner->GetAGI(), m_iAGIAdd, Attri.GetAgi());
			}
			break;
		case 4:
			if (m_iSPRAdd > 0)
			{
				--m_iSPRAdd;
				name = "PartnerSPR";
				SetNumField(name, m_pOnShowPartner->GetSPR(), m_iSPRAdd, Attri.GetSpr());
			}
			break;
		}		
		name = "PartnerPOT";
		UInt16 attrPoint = m_pOnShowPartner->GetAttrPoint() - m_iSPRAdd - m_iAGIAdd - m_iSTAAdd - m_iCONAdd - m_iSTRAdd;
		SetNumField(name, attrPoint);
	}
}

void CMenuPartnerProcess::AddConfirm()
{
	if (IsHawking())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_ADD_PARTNER_ATTRI_PT")));
		return;
	}
	if (m_pOnShowPartner)
	{
		if ((m_iSPRAdd + m_iAGIAdd + m_iSTAAdd + m_iCONAdd + m_iSTRAdd) > 0 )
		{
			m_pOnShowPartner->RaiseAttrib(gGlobal.pPeerProxy, gGlobal.g_context, m_iSTAAdd, m_iSPRAdd, m_iSTRAdd, m_iCONAdd, m_iAGIAdd);
			m_iSTAAdd = 0;
			m_iSPRAdd = 0;
			m_iSTRAdd = 0;
			m_iCONAdd = 0;
			m_iAGIAdd = 0;
		}
	}
}

void CMenuPartnerProcess::SelectPartner(int index)
{
	if (gGlobal.g_pPartnerGroup)
	{
		m_iSelectedIndex = m_iStartIndex + index - 1;

		CPartner* partner = gGlobal.g_pPartnerGroup->GetChild<CPartner>(m_iSelectedIndex);
		SetOnShowPartner(partner);
		UpdatePartnerMenu();
	}
}

void CMenuPartnerProcess::ChangePartnerName()
{
	VMProtectBegin; VM_START;

	if (IsHawking())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_CHANGE_PP_NAME")));
		return;
	}

	UIEditBox* partnerNameBox = (UIEditBox*)gGlobal.pUIManager->GetUIItem("PartnerNewName");
	if (partnerNameBox)
	{
		String newName(partnerNameBox->GetText());
		if (!gGlobal.CanUsedName(newName))
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CREATE_CHECK_FAIL")), CHANNELTYPE_SYSTEM);
			return;
		}

		if (m_pOnShowPartner)
		{
			m_pOnShowPartner->ChangePartnerName(gGlobal.pPeerProxy, gGlobal.g_context, newName);
		}
	}

	VMProtectEnd; VM_END;
}

void CMenuPartnerProcess::SetPartnerState(int state)
{
	VMProtectBegin; VM_START;

	if (IsHawking())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_CHANGE_PARTNER_STATE")));
		return;
	}

	if (m_pOnShowPartner && m_pOnShowPartner->m_data)
	{
		if ( (m_pOnShowPartner->GetLevel() > (gGlobal.g_pCharacter->GetCharNetObject()->GetLevel() + 5) || 
			  m_pOnShowPartner->m_data->m_basicinfo->req_lv > gGlobal.g_pCharacter->GetCharNetObject()->GetLevel() )
			&& gGlobal.g_pCharacter->GetAction() != CCharacter::ST_BATTLE && gGlobal.g_pCharacter->GetAction() != CCharacter::ST_NEEDHELP)
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_LEVEL_TOO_HIGH")), m_pOnShowPartner->GetRaname().c_str());
			gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
			return;
		}

		if ((gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP) 
			&& m_pOnShowPartner->GetHealth() > 50)
		{
			//assume the battle only can set the partner state to 1(Fight)
			gGlobal.g_pBattleControl->GetControlPlayer()->SendBattleCommand(BATCMD_FRIEND, 0, m_pOnShowPartner->GetPartner_id(), 0);
			SetButtonState("PartnerFight", false);
			//CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ClosePartner");
		}
		else
		{
			if (m_pOnShowPartner->GetState() != state)
			{
				if (state == CPartner::STATE_STANDBY ) //change to standby
				{
					m_pOnShowPartner->ChangePartnerState(gGlobal.pPeerProxy, gGlobal.g_context, state);
					UpdateSmallIcon(false);
				}
				else if (state == CPartner::STATE_FIGHT) //chage to fight
				{
					if (m_pOnShowPartner->GetHealth() > 50)
					{
						gGlobal.g_pPartnerGroup->ChangePartnerState(gGlobal.pPeerProxy, gGlobal.g_context, m_pOnShowPartner->GetPartner_id(), state);
						//UpdateSmallIcon(true, m_pOnShowPartner->GetPartner_id());
					}
					else
					{
						String text;
						text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_NO_HEALTH")), m_pOnShowPartner->GetRaname().c_str());
						gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
					}
				}
			}
		}
	}

	VMProtectEnd; VM_END;
}

void CMenuPartnerProcess::ScrollPartner(int index)
{
	if (index == 0)
	{
		if (m_iStartIndex > 0)
		{
			--m_iStartIndex;
			UpdatePartnerNameList();
			if (m_iStartIndex == 0)
			{
				UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("PartnerScrollBar");
				if (scrollBar)
					scrollBar->HomeScroll();
			}
		}
	}
	else if (index == 1)
	{
		if (gGlobal.g_pPartnerGroup->GetCount() > NUMOFSHOWPARTNER)
		{
			if ((m_iStartIndex + NUMOFSHOWPARTNER) < gGlobal.g_pPartnerGroup->GetCount() )
			{
				++m_iStartIndex;
				UpdatePartnerNameList();
				if ((m_iStartIndex + NUMOFSHOWPARTNER) == gGlobal.g_pPartnerGroup->GetCount())
				{
					UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("PartnerScrollBar");
					if (scrollBar)
						scrollBar->EndScroll();
				}
			}
			
		}
		else
		{
			UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("PartnerScrollBar");
				if (scrollBar)
					scrollBar->HomeScroll();
		}
	}
	else if (index == 2)
	{
		UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("PartnerScrollBar");

		if (scrollBar)
		{
			if (gGlobal.g_pPartnerGroup->GetCount() <= NUMOFSHOWPARTNER)
				scrollBar->HomeScroll();
			else
			{
				UInt8 index = scrollBar->GetSelNum();
				if ((index + NUMOFSHOWPARTNER - 1) < gGlobal.g_pPartnerGroup->GetCount() )
				{
					if (m_iStartIndex != index)
					{
						m_iStartIndex = index;
						UpdatePartnerNameList();
					}
				}	
				else
				{
					if ((gGlobal.g_pPartnerGroup->GetCount() - NUMOFSHOWPARTNER) != m_iStartIndex)
					{
						m_iStartIndex = gGlobal.g_pPartnerGroup->GetCount() - NUMOFSHOWPARTNER;
						UpdatePartnerNameList();
					}
				}
			}
		}
	}
}

void CMenuPartnerProcess::SetFollow()
{
	if (m_pOnShowPartner && m_pOnShowPartner->m_data)
	{
		if (gGlobal.g_pCharacter->GetCharNetObject()->GetPartner_id() != m_pOnShowPartner->GetPartner_id() && (gGlobal.g_pParty == NULL) )
		{
			if (m_pOnShowPartner->m_data->m_basicinfo->req_lv <=  gGlobal.g_pCharacter->GetCharNetObject()->GetLevel())
				gGlobal.g_pCharacter->GetCharNetObject()->SetFollowPartner(gGlobal.pPeerProxy, gGlobal.g_context, m_pOnShowPartner->GetPartner_id());
			else
			{
				String msg = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_LEVEL_TOO_HIGH_CANT_FOLLOW"));
				gGlobal.PrintPlayerMessage(msg);
				gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
			}
		}
		else 
			gGlobal.g_pCharacter->GetCharNetObject()->SetFollowPartner(gGlobal.pPeerProxy, gGlobal.g_context, 0);
	}
}

void CMenuPartnerProcess::UpdateStateButton()
{
	UIButton* standby = (UIButton*)gGlobal.pUIManager->GetUIItem("PartnerStandBy");
	UIButton* fight = (UIButton*)gGlobal.pUIManager->GetUIItem("PartnerFight");

	if (m_pOnShowPartner && standby && fight)
	{
		if (gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
		{		
			standby->SetDisableState();
					
			UIPanel* battlePanel = gGlobal.pUIManager->FindUIPanel("BattlePanel");
			if (m_pOnShowPartner->GetState() == CPartner::STATE_FIGHT || m_pOnShowPartner->GetState() == CPartner::STATE_REST || 
				m_pOnShowPartner->GetHealth() <= 50 || (battlePanel && !battlePanel->IsEnable()) )
				fight->SetDisableState();				
			else			
				fight->SetNormalState();			
		}
		else
		{
			if (m_pOnShowPartner->GetState() == CPartner::STATE_FIGHT)
			{				
				fight->SetDisableState();
				standby->SetNormalState();				
			}
			else if (m_pOnShowPartner->GetState() == CPartner::STATE_STANDBY)
			{
				fight->SetNormalState();
				standby->SetDisableState();				
			}
		}
		gGlobal.pUIManager->AddDirtyUIItem(fight);
		gGlobal.pUIManager->AddDirtyUIItem(standby);
	}
}

void CMenuPartnerProcess::UpdateHealth()
{
	if (m_pOnShowPartner)
		SetNumField("PartnerIntHealth", m_pOnShowPartner->GetHealth());
}

void CMenuPartnerProcess::SetButtonState(StringA buttonName, bool state)
{
	UIButton* button = (UIButton*)gGlobal.pUIManager->GetUIItem(buttonName.c_str());
	if (button)
	{
		if (state)
			button->SetNormalState();
		else
			button->SetDisableState();

		gGlobal.pUIManager->AddDirtyUIItem(button);
	}
}

void CMenuPartnerProcess::UpdateReqLevelAndRemain()
{
	if (m_pOnShowPartner && m_pOnShowPartner->m_data)
	{
		SetNumField("PartnerReqLevel", m_pOnShowPartner->m_data->m_basicinfo->req_lv);	

		StringA name = "PartnerRemainDay";
		UIStaticText* remainDay = gGlobal.pUIManager->FindUIStaticText(name.c_str());
		if (remainDay)
		{
			String text;
			MobDB_Client * mobTemplate = m_pOnShowPartner->m_data;
			if (m_pOnShowPartner->GetExpireDate() == 0)
			{
				if (mobTemplate->m_basicinfo->TimeLimitType == PARTNER_TIME_LIMIT_TYPE_NONE)
					text =gGlobal.GetStringTable()->Get(_T("MSG_INFINITY"));
				else
					text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PET_AFTER_USE")), mobTemplate->m_basicinfo->TimeLimit);		
			}
			else 
			{
				time_t now;
				time(&now);				
				
				if (m_pOnShowPartner->GetExpireDate() >= (UInt32)now)
				{
					UInt32 remainTime = m_pOnShowPartner->GetExpireDate() - (UInt32)now;

					remainTime /= 60; //change it to min first
					UInt min = remainTime % 60;
					remainTime /= 60; //change it to hour
					UInt hour = remainTime % 24;
					UInt day = remainTime / 24;
					
					if (day > 0)
						text.Format(_T("%d%s"), day + 1,gGlobal.GetStringTable()->Get(_T("MSG_DAY")));
					else if (hour > 0)
						text.Format(_T("%d%s"), hour,gGlobal.GetStringTable()->Get(_T("MSG_HOUR")));
					else if (min > 0)
						text.Format(_T("%d%s"), min,gGlobal.GetStringTable()->Get(_T("MSG_MINUTE")));

				}
				else
				{
					if (mobTemplate->m_basicinfo->TimeLimitType == PARTNER_TIME_LIMIT_TYPE_NONE)
						text =gGlobal.GetStringTable()->Get(_T("MSG_INFINITY"));
					else
						text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PET_AFTER_USE")), mobTemplate->m_basicinfo->TimeLimit);		
				}
			}
			remainDay->SetText(text.c_str());
			remainDay->Redraw();
			remainDay->SetEnable(true);
		}
	}
}

void CMenuPartnerProcess::UpdateNative()
{
	if (m_pOnShowPartner && m_pOnShowPartner->m_data)
	{
		StringA name;

		UInt16 cultivation = m_pOnShowPartner->GetCultivation();
		gGlobal.g_TempResult = 0;
		CallLuaFunc(gGlobal.g_pParentLua, "GetNativeUpByCultivation", String(), cultivation);
		UInt16 NativeUp = (UInt)gGlobal.g_TempResult;
			
		name = "PartnerIntHP";
		SetNumField(name, m_pOnShowPartner->GetHP_native() + NativeUp);

		name = "PartnerIntSP";
		SetNumField(name, m_pOnShowPartner->GetSP_native() + NativeUp);

		name = "PartnerIntATK";
		SetNumField(name, m_pOnShowPartner->GetATK_native() + NativeUp);

		name = "PartnerIntDEF";
		SetNumField(name, m_pOnShowPartner->GetDEF_native() + NativeUp);

		name = "PartnerIntHIT";
		SetNumField(name, m_pOnShowPartner->GetHIT_native() + NativeUp);

		name = "PartnerIntEVA";
		SetNumField(name, m_pOnShowPartner->GetEVA_native() + NativeUp);

		name = "PartnerIntPWD";
		SetNumField(name, m_pOnShowPartner->GetPOW_native() + NativeUp);

		name = "PartnerIntSPD";
		SetNumField(name, m_pOnShowPartner->GetSPD_native() + NativeUp);

		name = "PartnerIntGrow";
		UInt16 grow = (UInt16)(m_pOnShowPartner->GetGrow() * 1000.0f);
		SetNumField(name, grow);

		/*gGlobal.g_TempResult = 0;
		UInt16 grow_d = (UInt16)(m_pOnShowPartner->m_data->m_basicinfo->grow_d * 1000.0f);
		UInt16 grow_u = (UInt16)(m_pOnShowPartner->m_data->m_basicinfo->grow_u * 1000.0f);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "GetPartnerGrowLevel", String(), 
			grow, grow_d, grow_u);
		UInt16 growlevel = (UInt16)gGlobal.g_TempResult;
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("GetPartnerGrowLevel(%d,%d,%d) = %d."), 
			grow, grow_d, grow_u, growlevel);*/
		for (Index i = C_INDEX(0); i < C_INDEX(5); i++)
		{
			name.Format("PartnerIntGrowLevel%i", i);
			UIStaticText* field;
			field = (UIStaticText*)gGlobal.pUIManager->GetUIItem(name.c_str());
			if (field)
			{
				if (i == m_pOnShowPartner->GetGrowLevel())
					field->SetEnable(true);
				else
					field->SetEnable(false);
				field->Redraw();
				gGlobal.pUIManager->AddDirtyUIItem(field);
			}
		}
	}
}

void CMenuPartnerProcess::UpdateAllButtonState()
{
	if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
	{
		SetButtonState("PartnerFollow", false);
		SetButtonState("PartnerSkill", false);
		SetButtonState("PartnerChangeName", false);
		SetButtonState("PartnerGiveUp", false);
		SetButtonState("PartnerCollection", false);
	}
	else
	{
		SetButtonState("PartnerFollow", true);
		SetButtonState("PartnerSkill", true);
		SetButtonState("PartnerChangeName", true);
		SetButtonState("PartnerGiveUp", true);
		SetButtonState("PartnerCollection", true);
	}


	if ( (m_pOnShowPartner->GetAttrPoint() == 0) || (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE) )
	{
		SetButtonState("PartnerSTRAdd", false);
		SetButtonState("PartnerCONAdd", false);
		SetButtonState("PartnerSTAAdd", false);
		SetButtonState("PartnerAGIAdd", false);
		SetButtonState("PartnerSPRAdd", false);
		SetButtonState("PartnerSTRSub", false);
		SetButtonState("PartnerCONSub", false);
		SetButtonState("PartnerSTASub", false);
		SetButtonState("PartnerAGISub", false);
		SetButtonState("PartnerSPRSub", false);

		SetButtonState("PartnerLearning", false);	
	}
	else
	{
		SetButtonState("PartnerSTRAdd", true);
		SetButtonState("PartnerCONAdd", true);
		SetButtonState("PartnerSTAAdd", true);
		SetButtonState("PartnerAGIAdd", true);
		SetButtonState("PartnerSPRAdd", true);
		SetButtonState("PartnerSTRSub", true);
		SetButtonState("PartnerCONSub", true);
		SetButtonState("PartnerSTASub", true);
		SetButtonState("PartnerAGISub", true);
		SetButtonState("PartnerSPRSub", true);

		SetButtonState("PartnerLearning", true);
	}
}

void CMenuPartnerProcess::ShowRemovePartnerName()
{
	UIStaticText* name = (UIStaticText*)gGlobal.pUIManager->GetUIItem("ConfirmDropPartnerText");
	if (name && m_pOnShowPartner)
	{
		String nameString;
		nameString.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARNTER_REMOVE")), m_pOnShowPartner->GetRaname().c_str());
		name->SetText(nameString.c_str());
		name->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(name);
	}
}

void CMenuPartnerProcess::UpdateSmallIcon(bool show, UInt32 partnerID)
{
	UIStaticText* partnerName = (UIStaticText*)gGlobal.pUIManager->GetUIItem("MainPartnerName");
	
	UIAniIcon* partnerIcon = gGlobal.pUIManager->FindUIAniIcon("PartnerFace");
	if ( partnerIcon && partnerName)
	{
		if (show)
		{
			CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(partnerID);
			if (partner && partner->m_data)
			{								
				partnerIcon->ClearAniData();
				String pFace = String(partner->m_data->m_basicinfo->mob_faceS);
				partnerIcon->SetAnimationData(pFace.c_str());
				partnerIcon->SetEnable(true);
					
				partnerName->SetText(partner->GetRaname().c_str());
				partnerName->Redraw();
				gGlobal.pUIManager->AddDirtyUIItem(partnerName);

				UpdateSmallHP(partnerID);
				UpdateSmallSP(partnerID);
			}
		}
		else
		{
			partnerIcon->SetEnable(false);
			gGlobal.pUIManager->AddDirtyUIItem(partnerIcon);
			
			partnerName->SetText(_T(""));
			partnerName->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(partnerName);
			gGlobal.SetUILinerImage("PartnerSHPBar", 1, 0);
			gGlobal.SetUILinerImage("PartnerSSPBar", 1, 0);

			UILinerImage* pLI = gGlobal.pUIManager->FindUILinerImage("PartnerSHPBar");
			if (pLI)
				gGlobal.pUIManager->AddDirtyUIItem(pLI);
			
			pLI = gGlobal.pUIManager->FindUILinerImage("PartnerSSPBar");
			if (pLI)
				gGlobal.pUIManager->AddDirtyUIItem(pLI);
			
		}
	}
}

void CMenuPartnerProcess::UpdateSmallHP(UInt32 partnerID)
{
	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(partnerID);

	if (partner)
	{
		UInt hp = partner->GetHP();
		
		if (partner->GetBattleHP() >= 0)
			hp = partner->GetBattleHP();	
		
		gGlobal.SetUILinerImage("PartnerSHPBar", partner->GetMaxHP(), hp);
		UILinerImage* pLI = gGlobal.pUIManager->FindUILinerImage("PartnerSHPBar");
		if (pLI)
		{
			gGlobal.pUIManager->AddDirtyUIItem(pLI);
		
			String Text, Format;
			Format.Format(_T("%i/%i"), hp, partner->GetMaxHP());

			Text = pLI->GetTips();
			Text = Text.substr(0, 3);
			Text += Format;

			pLI->SetTips(Text.c_str());
		}
	}
}

void CMenuPartnerProcess::UpdateSmallSP(UInt32 partnerID)
{
	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(partnerID);
	if (partner)
	{
		UInt sp = partner->GetSP();
		if (partner->GetBattleSP() >= 0)
			sp = partner->GetBattleSP();	
	
		gGlobal.SetUILinerImage("PartnerSSPBar", partner->GetMaxSP(), sp);
		UILinerImage* pLI = gGlobal.pUIManager->FindUILinerImage("PartnerSSPBar");
		if (pLI)
		{
			gGlobal.pUIManager->AddDirtyUIItem(pLI);

			String Text, Format;
			Format.Format(_T("%i/%i"), sp, partner->GetMaxSP());

			Text = pLI->GetTips();
			Text = Text.substr(0, 3);
			Text += Format;

			pLI->SetTips(Text.c_str());
		}
	}
}

bool CMenuPartnerProcess::IsOnShowPartner(UInt32 partnerID)
{
	if (m_pOnShowPartner)
	{
		if (m_pOnShowPartner->GetPartner_id() == partnerID)
			return true;
	}
	return false;
}

void CMenuPartnerProcess::UpdateSkillMenu(CPartner* partner)
{
	StringA iconName;
	UInt maxNormMobSkill = 7;
	UInt maxSpecMobSkill = 7;

	for (Index i = C_INDEX(0); i < maxNormMobSkill; ++i)
	{
		if (gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
			iconName.Format("PartnerBattleNormalSkill0%d", i);
		else
			iconName.Format("PartnerNormalSkill0%d", i);

		UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
		if (pAniIcon)
		{
			pAniIcon->ClearAniData();
			pAniIcon->SetEnable(false);
		}

		if (i < maxSpecMobSkill)
		{
			if (gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
				iconName.Format("PartnerBattleSpecialSkill0%d", i);
			else
				iconName.Format("PartnerSpecialSkill0%d", i);

			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
			if (pAniIcon)
			{
				pAniIcon->ClearAniData();
				pAniIcon->SetEnable(false);
			}
		}
	}

	if (gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
	{
		gGlobal.g_BPShortCutFlag = true;
		for (Index i = C_INDEX(0); i < 10; ++i)
		{
			iconName.Format("SPBSkill0%d", i);
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
			if (pAniIcon)
			{
				pAniIcon->ClearAniData();
				pAniIcon->SetEnable(false);
			}
		}
	}

	if (partner)
	{
		UInt32 ShortCutIndex = 0;
		UInt32 normalSkillIndex = 0;
		UInt32 specialSkillIndex = 0;
		
		m_SkillButtonMap.clear();
		
		StlMap<UInt16, CSkill*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, CSkill*> > > tempSkillList;
		
		for (UInt32 i = 0; i < partner->m_pSkillGrp->GetCount(); ++i)
		{
			CSkill* pSkill = partner->m_pSkillGrp->GetChild<CSkill>(i);
			if (pSkill)
				tempSkillList.insert(std::make_pair(pSkill->GetSkill_id(), pSkill));
		}

		for(StlMap<UInt16, CSkill*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, CSkill*> > >::iterator itr = tempSkillList.begin(); itr != tempSkillList.end(); ++itr)		
		{
			CSkill* pSkill = itr->second;
			if (pSkill)
			{
				const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(pSkill->GetSkill_id());
				if (pSkillData)
				{
					if (pSkillData->active == SKILL_PASSIVE && 
						(gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP ))
						continue;

					StringA ShortCutIconName = "";

					if (pSkill->GetPartner_skill_type() == PARTNER_NORMAL_SKILL)
					{
						if (gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
						{
							iconName.Format("PartnerBattleNormalSkill0%d", normalSkillIndex);
							ShortCutIconName.Format("SPBSkill0%d", ShortCutIndex);
						}
						else
							iconName.Format("PartnerNormalSkill0%d", normalSkillIndex);						
						++normalSkillIndex;
					}
					else if (pSkill->GetPartner_skill_type() == PARTNER_SPECIAL_SKILL)
					{
						if (gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
						{
							iconName.Format("PartnerBattleSpecialSkill0%d", specialSkillIndex);
							ShortCutIconName.Format("SPBSkill0%d", ShortCutIndex);
						}
						else
							iconName.Format("PartnerSpecialSkill0%d", specialSkillIndex);
						++specialSkillIndex;
					}

					m_SkillButtonMap.insert(std::make_pair(iconName, pSkill->GetSkill_id()));

					UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
					if (pAniIcon)
					{
						CAnimatedEntity Ani;
						Ani.LoadEntityFile(pSkillData->icon, gGlobal.pD3DDevice, EQUIP_OTHERS);
						if (Ani.HasAniFile())
						{
							pAniIcon->ClearAniData();
							pAniIcon->SetAnimationData(&Ani);
							pAniIcon->SetEnable(true);
						}
						else
						{
							Ani.InitEntPicInfo();
							String temp;
							temp = _T("skillicon0046.ent");
							Ani.LoadEntityFile(temp.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);
							if (Ani.HasAniFile())
							{
								pAniIcon->ClearAniData();
								pAniIcon->SetAnimationData(&Ani);
								pAniIcon->SetEnable(true);
							}
						}
					}

					if(!ShortCutIconName.IsEmpty())
					{
						UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(ShortCutIconName.c_str());
						if (pAniIcon)
						{
							m_ShortCutStr[ShortCutIndex] = iconName;
							CAnimatedEntity Ani;
							Ani.LoadEntityFile(pSkillData->icon, gGlobal.pD3DDevice, EQUIP_OTHERS);
							if (Ani.HasAniFile())
							{
								pAniIcon->ClearAniData();
								pAniIcon->SetAnimationData(&Ani);
								pAniIcon->SetEnable(true);
							}
							else
							{
								Ani.InitEntPicInfo();
								String temp;
								temp = _T("skillicon0046.ent");
								Ani.LoadEntityFile(temp.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);
								if (Ani.HasAniFile())
								{
									pAniIcon->ClearAniData();
									pAniIcon->SetAnimationData(&Ani);
									pAniIcon->SetEnable(true);
								}
							}
							String	ShowName;
							ShowName.Format(_T("#W%s\\r#Y%s"), pSkillData->name, pSkillData->skill_functionDes);
							pAniIcon->SetTips(ShowName.c_str());
							++ShortCutIndex;
						}
					}
				}
			}
		}
	}
}

void CMenuPartnerProcess::ShowSkillDetail()
{
	UIPanel* pPanel = NULL;
	UIPanel* pShortCutPanel = NULL;
	if (gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
	{
		pPanel = gGlobal.pUIManager->FindUIPanel("PartnerBattleSkillPanel");
		pShortCutPanel = gGlobal.pUIManager->FindUIPanel("PartnerBattleSkillShortCut");
	}
	else
		pPanel = gGlobal.pUIManager->FindUIPanel("PartnerSkillPanel");
	if (pPanel)
	{
		UIAniIcon* pAniIcon = (UIAniIcon*)pPanel->GetMouseOver();
		if (pAniIcon)
		{
			PartnerSkillButtonMap::iterator itr = m_SkillButtonMap.find(*pAniIcon->GetName());
			if (itr != m_SkillButtonMap.end())
			{
				Int MouseX = gGlobal.pUIManager->GetCursorX();
				Int MouseY = gGlobal.pUIManager->GetCursorY();

				const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(itr->second);

				if (pSkillData == NULL)
					return;

				UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("SkillDetailMenuPanel");
				if (pPanelEx == NULL)
				{
					XML_UI::GetInstance().Init();
					XML_UI::GetInstance().ReadFormFile("Data\\SkillDetail.xml", gGlobal.pUIManager);

					pPanelEx = gGlobal.pUIManager->FindUIPanelEx("SkillDetailMenuPanel");
					if (pPanelEx == NULL)
						return;		
				}

				gGlobal.pUIManager->SetPanelEx2Top(pPanelEx);

				UIChatPanel * pChatPanel = gGlobal.pUIManager->FindUIChatPanel("SkillDetailChat");

				if (pChatPanel)
				{
					pChatPanel->ClearAllRow();
					//add skill name
					String name;
					name.Format(_T("#W%s"), pSkillData->name);
					pChatPanel->AddStringW(name.c_str());
					if (wcscmp(pSkillData->skill_description, _T("0")) != 0)
						pChatPanel->AddStringW(pSkillData->skill_description);

					Int Level = 1;
					
					gGlobal.g_TempSkillString = "";
					
					if (!gGlobal.g_TempSkillString.IsEmpty())
					{
						String Com;
						Com.Format(_T("#%s%s"),TextColorStr[ColorIndex_Yellow], gGlobal.g_TempSkillString);
						pChatPanel->AddStringW(Com.c_str());
					}
					else
					{
						if (wcscmp(pSkillData->skill_functionDes, _T("0")) != 0)
						{
							String Com;
							Com.Format(_T("#%s%s"), TextColorStr[ColorIndex_Yellow], pSkillData->skill_functionDes);
							pChatPanel->AddStringW(Com.c_str());
						}
					}
					
					pChatPanel->SetFontColor(TextColor[ColorIndex_White]);
					
					pChatPanel->FillImage();
					pChatPanel->EndRow();
					pChatPanel->Redraw();

					//pChatPanel->end
					UInt32 LineCount = (UInt32)ceil((Real32)(pChatPanel->GetTotalHeight())/ 
						(pPanelEx->GetLineRect()->bottom - pPanelEx->GetLineRect()->top));
					pPanelEx->ChangeCount(LineCount);
				}

				if ( (pPanelEx->GetWidth() + MouseX + 20) > 800 )
					MouseX = 800 - 20 - pPanelEx->GetWidth();
				if ( (pPanelEx->GetHeight() + MouseY) > 600 )
					MouseY = 600 - pPanelEx->GetHeight();
				pPanelEx->SetPos(MouseX + 20, MouseY);

				pPanelEx->SetEnable(true);

				gGlobal.pUIManager->AddDirtyUIItem(pPanelEx);
			}
		}
	}
}

void CMenuPartnerProcess::UpdateBattleSkillMenu()
{
	if (gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
	{
		CPartner* pPartner = gGlobal.g_pPartnerGroup->GetCurrBattlePartner();
		if (pPartner)
			UpdateSkillMenu(pPartner);
	}
}

bool CMenuPartnerProcess::SelectSkill()
{
	if (gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
	{
		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartnerBattleSkillPanel");
		if (pPanel)
		{
			UIAniIcon* pAniIcon = (UIAniIcon*)pPanel->GetMouseOver();
			if (pAniIcon && pAniIcon->IsEnable())
			{
				PartnerSkillButtonMap::iterator itr = m_SkillButtonMap.find(*pAniIcon->GetName());
				if (itr != m_SkillButtonMap.end())
				{
					m_iTempSelSkillID = itr->second;
					const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(itr->second);
					
					if (pSkillData)
					{
						m_pTempSelSkillData = pSkillData;
						if (pSkillData->battleorField == SKILL_USE_TIME_PEACE)
						{
							gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_USE_FAIL_NOT_PEACE")), CHANNELTYPE_SYSTEM);
							return false;
						}
						if (pSkillData->active == SKILL_PASSIVE)
						{
							gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_USE_FAIL_TYPE_PASSIVE")), CHANNELTYPE_SYSTEM);
							return false;
						}
						gGlobal.g_bSkillFail = FALSE;
						StringA FuncName;
						FuncName.Format("%s%s", gGlobal.GetPrefixStr(pSkillData->id, "skill"), CommonSkillFuncName[SKILL_FUNC_NAME_CANUSE].c_str());
						
						CallLuaFunc(gGlobal.g_pParentLua, FuncName.c_str(), String(), gGlobal.g_pBattleControl->GetControlPartner(), 0, 0, 0);
						if (gGlobal.g_bSkillFail)
						{

							gGlobal.PrintMessage(gGlobal.g_TempSkillString, CHANNELTYPE_SYSTEM);
							return false;
						}
						State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUISkillState));
						gGlobal.g_CursorState->Block();

						if (pSkillData->Skill_ThrowItemType != 255)
						{
							gGlobal.g_pBattleControl->SetBattleItemUseIndex(-1);
							CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenBattleItemMenu", String(), 0, 0);
						}

						if (pSkillData->targetType == TARGET_TYPE_SELF)
						{
							
							if (!gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand())
								gGlobal.g_pBattleControl->SkillUseOnSeletedChar(gGlobal.g_pBattleControl->GetControlPartner());
														
							gGlobal.g_CursorState->Unblock();
							State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
						
						}
						return true;

					}
					else
						return false;
				}
			}
		}
	}
	return false;
}



bool CMenuPartnerProcess::ShortCutSelectSkill(UInt32 Index)
{
	if (gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
	{
		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartnerBattleSkillPanel");
		if (pPanel)
		{
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(m_ShortCutStr[Index].c_str());
			if (pAniIcon)
			{
				PartnerSkillButtonMap::iterator itr = m_SkillButtonMap.find(*pAniIcon->GetName());
				if (itr != m_SkillButtonMap.end())
				{
					m_iTempSelSkillID = itr->second;
					const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(itr->second);
					
					if (pSkillData)
					{
						m_pTempSelSkillData = pSkillData;
						if (pSkillData->battleorField == SKILL_USE_TIME_PEACE)
						{
							gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_USE_FAIL_NOT_PEACE")), CHANNELTYPE_SYSTEM);
							return false;
						}
						if (pSkillData->active == SKILL_PASSIVE)
						{
							gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_USE_FAIL_TYPE_PASSIVE")), CHANNELTYPE_SYSTEM);
							return false;
						}
						gGlobal.g_bSkillFail = FALSE;
						StringA FuncName;
						FuncName.Format("%s%s", gGlobal.GetPrefixStr(pSkillData->id, "skill"), CommonSkillFuncName[SKILL_FUNC_NAME_CANUSE].c_str());
						
						CallLuaFunc(gGlobal.g_pParentLua, FuncName.c_str(), String(), gGlobal.g_pBattleControl->GetControlPartner(), 0, 0, 0);
						if (gGlobal.g_bSkillFail)
						{

							gGlobal.PrintMessage(gGlobal.g_TempSkillString, CHANNELTYPE_SYSTEM);
							return false;
						}
						State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUISkillState));
						gGlobal.g_CursorState->Block();

						if (pSkillData->Skill_ThrowItemType != 255)
						{
							gGlobal.g_pBattleControl->SetBattleItemUseIndex(-1);
							CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenBattleItemMenu", String(), 0, 0);
						}

						if (pSkillData->targetType == TARGET_TYPE_SELF)
						{
							
							if (!gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand())
								gGlobal.g_pBattleControl->SkillUseOnSeletedChar(gGlobal.g_pBattleControl->GetControlPartner());
														
							gGlobal.g_CursorState->Unblock();
							State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
						
						}
						return true;

					}
					else
						return false;
				}
			}
		}
	}
	return false;
}

bool CMenuPartnerProcess::IsBattleSkillExist()
{
	if (gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE || gGlobal.g_pCharacter->GetAction() == CCharacter::ST_NEEDHELP)
	{
		CPartner* pPartner = gGlobal.g_pPartnerGroup->GetCurrBattlePartner();
		if (pPartner)
		{
			for (Index i = C_INDEX(0); i < pPartner->m_pSkillGrp->GetCount(); ++i)
			{
				CSkill* pSkill = pPartner->m_pSkillGrp->GetChild<CSkill>(i);
				if (pSkill)
				{
					const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(pSkill->GetSkill_id());
					if (pSkillData)
					{
						if (pSkillData->active == SKILL_ACTIVE)
							return true;
					}
				}
			}
		}
	}
	return false;
}

void CMenuPartnerProcess::UpdatePartnerDesc(CPartner* partner)
{
	if (partner && partner->m_data)
	{
		UIImage* pPartnerImage = gGlobal.pUIManager->FindUIImage("PartnerFaceImage");
		UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel("PartnerDesc");
		MobDB_Client * mobTemplate = partner->m_data;

		if (pPartnerImage && pChatPanel && mobTemplate)
		{
			String partnerFace = String(mobTemplate->m_basicinfo->mob_faceL);
			String str = _T("Data\\");
			if (partnerFace.length() > 0)
			{
				str.Format(_T("%s%s"), str.c_str(), partnerFace.c_str());
				pPartnerImage->SetImageFile(str.c_str());
				pPartnerImage->SetEnable(true);
			}
			else
				pPartnerImage->SetEnable(false);

			String mobDesc = gGlobal.m_resource->GetMobDesc(mobTemplate->m_basicinfo->desc_id);
			pChatPanel->ClearAllRow();
			pChatPanel->AddStringW(mobDesc.c_str());
			pChatPanel->EndRow();
			pChatPanel->Redraw();

			gGlobal.pUIManager->AddDirtyUIItem(pChatPanel);
		
		}
	}
}

void CMenuPartnerProcess::ShowPartnerDetail(CPartner* pPartner, UInt16Vector skillIDs)
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartnerDetailPanel");
	if (pPanel && pPartner && pPartner->m_data)
	{
		String text;
		UIStaticText * name = gGlobal.pUIManager->FindUIStaticText("PartnerDetailName");
		if (name)
		{
			name->SetText(String(pPartner->m_data->m_basicinfo->mid_name).c_str());
			name->Redraw();
		}

		SetNumField("PartnerDetailLevel", pPartner->GetLevel());
		SetNumField("PartnerDetailGoodness", pPartner->GetGoodness());

		float diff = pPartner->GetFavor() - (UInt16)pPartner->GetFavor();
		if (diff > 0.1f)
			SetNumField("PartnerDetailLovely", (UInt16)pPartner->GetFavor() + 1);
		else
			SetNumField("PartnerDetailLovely", (UInt16)pPartner->GetFavor());

		UpdateBar("PartnerDetailHP", pPartner->GetHP(), pPartner->GetMaxHP());
		UpdateBar("PartnerDetailSP", pPartner->GetSP(), pPartner->GetMaxSP());

		SetNumField("PartnerDetailHealth", pPartner->GetHealth());

		SetNumField("PartnerDetailATK", pPartner->GetATK());
		SetNumField("PartnerDetailEVA", pPartner->GetEVA());
		SetNumField("PartnerDetailDEF", pPartner->GetDEF());
		SetNumField("PartnerDetailPWD", pPartner->GetPOW());
		SetNumField("PartnerDetailHIT", pPartner->GetHIT());
		SetNumField("PartnerDetailSPD", pPartner->GetSPD());
		if (pPartner->m_data)
			SetNumField("PartnerDetailReqLv", pPartner->m_data->m_basicinfo->req_lv);
		else
			SetNumField("PartnerDetailReqLv", 0);

		SetNumField("PartnerDetailSTR", pPartner->GetSTR());
		SetNumField("PartnerDetailAGI", pPartner->GetAGI());
		SetNumField("PartnerDetailCON", pPartner->GetCON());
		SetNumField("PartnerDetailSPR", pPartner->GetSPR());
		SetNumField("PartnerDetailSTA", pPartner->GetSTA());
		SetNumField("PartnerDetailPOT", pPartner->GetAttrPoint());

		UInt16 cultivation = pPartner->GetCultivation();
		gGlobal.g_TempResult = 0;
		CallLuaFunc(gGlobal.g_pParentLua, "GetNativeUpByCultivation", String(), cultivation);
		UInt16 NativeUp = (UInt)gGlobal.g_TempResult;

		SetNumField("PartnerDetailIntHIT", pPartner->GetHIT_native() + NativeUp);
		SetNumField("PartnerDetailIntEVA", pPartner->GetEVA_native() + NativeUp);
		SetNumField("PartnerDetailIntPWD", pPartner->GetPOW_native() + NativeUp);
		SetNumField("PartnerDetailIntSPD", pPartner->GetSPD_native() + NativeUp);
		SetNumField("PartnerDetailIntHP", pPartner->GetHP_native() + NativeUp);
		SetNumField("PartnerDetailIntSP", pPartner->GetSP_native() + NativeUp);
		SetNumField("PartnerDetailIntATK", pPartner->GetATK_native() + NativeUp);
		SetNumField("PartnerDetailIntDEF", pPartner->GetDEF_native() + NativeUp);
		SetNumField("PartnerDetailGrow", (Int32)(pPartner->GetGrow() * 1000));

		SetNumField("PartnerDetailCultivation", pPartner->GetCultivation());
		SetNumField("PartnerDetailPractice", pPartner->GetPractice());

		/*gGlobal.g_TempResult = 0;
		UInt16 grow = (UInt16)(pPartner->GetGrow() * 1000.0f);
		UInt16 grow_d = (UInt16)(pPartner->m_data->m_basicinfo->grow_d * 1000.0f);
		UInt16 grow_u = (UInt16)(pPartner->m_data->m_basicinfo->grow_u * 1000.0f);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "GetPartnerGrowLevel", String(), 
			grow, grow_d, grow_u);
		UInt16 growlevel = (UInt16)gGlobal.g_TempResult;
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("GetPartnerGrowLevel(%d,%d,%d) = %d."), 
			grow, grow_d, grow_u, growlevel);*/
		for (Index i = C_INDEX(0); i < C_INDEX(5); i++)
		{
			StringA name;
			name.Format("PartnerDetailGrowLevel%i", i);
			UIStaticText* field;
			field = (UIStaticText*)gGlobal.pUIManager->GetUIItem(name.c_str());
			if (field)
			{
				if (i == pPartner->GetGrowLevel())
					field->SetEnable(true);
				else
					field->SetEnable(false);
				field->Redraw();
				gGlobal.pUIManager->AddDirtyUIItem(field);
			}
		}

		UInt8 normalSkillIndex = 0;
		UInt8 specialSkillIndex = 0;
		StringA iconName;

		//Ming

		for (Index i = C_INDEX(0); i < 7; ++i)
		{
			iconName.Format("PartnerDetailNormalSkill0%d", i);
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
			if (pAniIcon)
			{
				pAniIcon->ClearAniData();
				pAniIcon->SetEnable(false);
			}

			if (i < 5)
			{
				iconName.Format("PartnerDetailSpecialSkill0%d", i);
				pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
				if (pAniIcon)
				{
					pAniIcon->ClearAniData();
					pAniIcon->SetEnable(false);
				}
			}
		}

		for (Index i = C_INDEX(0); i < skillIDs.size(); ++i)
		{
			const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(skillIDs[i]);
			if (pSkillData)
			{
				if (pSkillData->type == SKILL_TYPE_PARTNER_NORMAL)
				{
					iconName.Format("PartnerDetailNormalSkill0%d", normalSkillIndex);
					++normalSkillIndex;
				}
				else if (pSkillData->type == SKILL_TYPE_PARTNER_SPECIAL)
				{
					iconName.Format("PartnerDetailSpecialSkill0%d", specialSkillIndex);
					++specialSkillIndex;
				}

				UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
				if (pAniIcon)
				{
					CAnimatedEntity Ani;
					Ani.LoadEntityFile(pSkillData->icon, gGlobal.pD3DDevice, EQUIP_OTHERS);
					if (Ani.HasAniFile())
					{
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(&Ani);
						pAniIcon->SetEnable(true);

						String	ShowName;
						ShowName.Format(_T("#W%s\\r#Y%s"), pSkillData->name, pSkillData->skill_functionDes);
						pAniIcon->SetTips(ShowName.c_str());
					}
				}
			}
		}

		MobDB_Client * mobTemplate = pPartner->m_data;

		CAnimatedEntity temp;
		temp.InitEntPicInfo();
		temp.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
		String modelName = String(mobTemplate->m_basicinfo->model) ;
		temp.LoadEntityFile(modelName.c_str(), gGlobal.pD3DDevice);

		String colorFileName = modelName.substr(0, modelName.length() - 5);
		char abc = pPartner->GetOutLookColor() + 97;
		colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
		temp.ChangeColorP(modelName.c_str(), colorFileName.c_str());

		//temp.SetAnimationIndexByIndex(temp.GetBattleAnimation(), TRUE, SOUTHEAST);
		UIAniIcon* pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem("PartnerDetailModel");
		if (pAniIcon)
		{				
			pAniIcon->ClearAniData();
			pAniIcon->SetAnimationData(&temp);
			pAniIcon->SetAnimationIndex(temp.GetBattleAnimation());				
			pAniIcon->SetRotation(SOUTHEAST);
			gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
		}

		UIStaticText* remainDay = gGlobal.pUIManager->FindUIStaticText("PartnerDetailDay");
		if (remainDay)
		{
			String text;
			if (pPartner->GetExpireDate() == 0)
			{
				if (mobTemplate->m_basicinfo->TimeLimitType == PARTNER_TIME_LIMIT_TYPE_NONE)
					text =gGlobal.GetStringTable()->Get(_T("MSG_INFINITY"));
				else
					text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PET_AFTER_USE")), mobTemplate->m_basicinfo->TimeLimit);		
			}
			else 
			{
				time_t now;
				time(&now);
				
				
				if (pPartner->GetExpireDate() >= (UInt32)now)
				{
					UInt32 remainTime = pPartner->GetExpireDate() - (UInt32)now;

					remainTime /= 60; //change it to min first
					UInt min = remainTime % 60;
					remainTime /= 60; //change it to hour
					UInt hour = remainTime % 24;
					UInt day = remainTime / 24;
					
					if (day > 0)
						text.Format(_T("%d%s"), day + 1,gGlobal.GetStringTable()->Get(_T("MSG_DAY")));
					else if (hour > 0)
						text.Format(_T("%d%s"), hour,gGlobal.GetStringTable()->Get(_T("MSG_HOUR")));
					else if (min > 0)
						text.Format(_T("%d%s"), min,gGlobal.GetStringTable()->Get(_T("MSG_MINUTE")));

				}
				else
				{
					if (mobTemplate->m_basicinfo->TimeLimitType == PARTNER_TIME_LIMIT_TYPE_NONE)
						text =gGlobal.GetStringTable()->Get(_T("MSG_INFINITY"));
					else
						text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PET_AFTER_USE")), mobTemplate->m_basicinfo->TimeLimit);		
				}
			}
			remainDay->SetText(text.c_str());
			remainDay->Redraw();
			remainDay->SetEnable(true);
		}

		UIImage* img = NULL;
		for (Index i = C_INDEX(1); i < 11; ++i)
		{
			iconName.Format("PartnerDetailRaceType%02d", i);
			img = gGlobal.pUIManager->FindUIImage(iconName.c_str());
			if (img)
				img->SetEnable(false);
		}
		
		iconName.Format("PartnerDetailRaceType%02d", mobTemplate->m_basicinfo->race);
		img = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		if (img)
			img->SetEnable(true);
		
		gGlobal.pUIManager->AddDirtyUIItem(pPanel);
	}
}

void CMenuPartnerProcess::ShowMobDetail(UInt32 mobID, UInt8 mobLvl)
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartnerDetailPanel");
	MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(mobID);

	if (pPanel && mobTemplate)
	{
		String text;
		UIStaticText * name = gGlobal.pUIManager->FindUIStaticText("PartnerDetailName");
		if (name)
		{
			name->SetText(String(mobTemplate->m_basicinfo->mid_name).c_str());
			name->Redraw();
		}

		float grow = mobTemplate->m_basicinfo->grow_d;
		UInt16 sta = mobTemplate->m_basicinfo->STA_base_d;
		UInt16 spr = mobTemplate->m_basicinfo->SPR_base_d;
		UInt16 str = mobTemplate->m_basicinfo->STR_base_d;
		UInt16 con = mobTemplate->m_basicinfo->CON_base_d;
		UInt16 agi = mobTemplate->m_basicinfo->AGI_base_d;
		UInt16 level = mobLvl;

		UInt16 atk = (UInt16)(str * (1.5f * grow - 0.3f) + level * 1.5f * grow + str * level / 500.0f + mobTemplate->m_basicinfo->ATK_native_d * level / 800.0f);
		UInt16 def = (UInt16)(con * (2.5f * grow - 1.3f) + level * grow * 1.1f + (mobTemplate->m_basicinfo->DEF_native_d - 600.0f) * level / 700.0f);
		UInt16 spd = (UInt16)(agi * mobTemplate->m_basicinfo->SPD_native_d / 1500.0f);
		UInt16 power = (UInt16)((spr * 1.2f + str * 0.7f + sta * 0.4f + con * 0.3f) + level * grow * 0.8f + mobTemplate->m_basicinfo->POW_native_d * level / 3000.0f);
		UInt16 hit = (UInt16)(agi * (grow * 5.0f - 1.5f) + level * grow * 2.0f + (mobTemplate->m_basicinfo->HIT_native_d - 600.0f) * level / 1200.0f);
		UInt16 eva = (UInt16)(agi * (grow * 2.5f - 2.0f) + level * grow * 0.8f + mobTemplate->m_basicinfo->EVA_native_d * level/ 2000.0f);

		UInt16 maxHP = (UInt16)(sta * grow * 7.0f + mobTemplate->m_basicinfo->HP_native_d * level / 1000.0f);
		UInt16 maxSP = (UInt16)(spr * (3.0f * grow + 0.5f) + mobTemplate->m_basicinfo->SP_native_d * level / 500.0f);

		SetNumField("PartnerDetailLevel", level);
		SetNumField("PartnerDetailGoodness", mobTemplate->m_basicinfo->goodness);

		SetNumField("PartnerDetailLovely", 100);
		UpdateBar("PartnerDetailHP", maxHP, maxHP);
		UpdateBar("PartnerDetailSP", maxSP, maxSP);

		SetNumField("PartnerDetailHealth", mobTemplate->m_basicinfo->maxHealth);

		SetNumField("PartnerDetailATK", atk);
		SetNumField("PartnerDetailEVA", eva);
		SetNumField("PartnerDetailDEF", def);
		SetNumField("PartnerDetailPWD", power);
		SetNumField("PartnerDetailHIT", hit);
		SetNumField("PartnerDetailSPD", spd);
		SetNumField("PartnerDetailReqLv", mobTemplate->m_basicinfo->req_lv);

		SetNumField("PartnerDetailSTR", str);
		SetNumField("PartnerDetailAGI", agi);
		SetNumField("PartnerDetailCON", con);
		SetNumField("PartnerDetailSPR", spr);
		SetNumField("PartnerDetailSTA", sta);
		SetNumField("PartnerDetailPOT", 0);

		SetNumField("PartnerDetailIntHIT", mobTemplate->m_basicinfo->HIT_native_d);
		SetNumField("PartnerDetailIntEVA", mobTemplate->m_basicinfo->EVA_native_d);
		SetNumField("PartnerDetailIntPWD", mobTemplate->m_basicinfo->POW_native_d);
		SetNumField("PartnerDetailIntSPD", mobTemplate->m_basicinfo->SPD_native_d);
		SetNumField("PartnerDetailIntHP", mobTemplate->m_basicinfo->HP_native_d);
		SetNumField("PartnerDetailIntSP", mobTemplate->m_basicinfo->SP_native_d);
		SetNumField("PartnerDetailIntATK", mobTemplate->m_basicinfo->ATK_native_d);
		SetNumField("PartnerDetailIntDEF", mobTemplate->m_basicinfo->DEF_native_d);
		SetNumField("PartnerDetailGrow", Int32(grow * 1000));

		UInt8 normalSkillIndex = 0;
		UInt8 specialSkillIndex = 0;
		StringA iconName;

		String fileName;
		fileName.Format(_T("MobSkill%05d.csv"), mobTemplate->m_basicinfo->skill_file_id);
		const MobSkillCombination* mobSkillLst = gGlobal.m_resource->GetMobSkillCombination(fileName);

		UInt16Vector skillIDs;
		if (mobSkillLst)
		{
			for (Index i = C_INDEX(0); i < mobSkillLst->skillLst.size(); ++i)
			{
				MobSkillDef* pDef = mobSkillLst->skillLst[i];
				if (pDef && pDef->skillRate == 100000)
					skillIDs.push_back(pDef->skillId);
			}
		}

		for (Index i = C_INDEX(0); i < 7; ++i)
		{
			iconName.Format("PartnerDetailNormalSkill0%d", i);
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
			if (pAniIcon)
			{
				pAniIcon->ClearAniData();
				pAniIcon->SetEnable(false);
			}

			if (i < 5)
			{
				iconName.Format("PartnerDetailSpecialSkill0%d", i);
				pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
				if (pAniIcon)
				{
					pAniIcon->ClearAniData();
					pAniIcon->SetEnable(false);
				}
			}
		}
	
		for (Index i = C_INDEX(0); i < skillIDs.size(); ++i)
		{
			const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(skillIDs[i]);
			if (pSkillData)
			{
				if (pSkillData->type == SKILL_TYPE_PARTNER_NORMAL)
				{
					iconName.Format("PartnerDetailNormalSkill0%d", normalSkillIndex);						
					++normalSkillIndex;
				}
				else if (pSkillData->type == SKILL_TYPE_PARTNER_SPECIAL)
				{
					iconName.Format("PartnerDetailSpecialSkill0%d", specialSkillIndex);
					++specialSkillIndex;
				}

				UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
				if (pAniIcon)
				{
					CAnimatedEntity Ani;
					Ani.LoadEntityFile(pSkillData->icon, gGlobal.pD3DDevice, EQUIP_OTHERS);
					if (Ani.HasAniFile())
					{
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(&Ani);
						pAniIcon->SetEnable(true);
					}
				}			
			}
		}
		
		CAnimatedEntity temp;
		temp.InitEntPicInfo();
		temp.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
		String modelName = String(mobTemplate->m_basicinfo->model) ;
		temp.LoadEntityFile(modelName.c_str(), gGlobal.pD3DDevice);

		String colorFileName = String(mobTemplate->m_basicinfo->color);		
		temp.ChangeColorP(modelName.c_str(), colorFileName.c_str());

		//temp.SetAnimationIndexByIndex(temp.GetBattleAnimation(), TRUE, SOUTHEAST);
		UIAniIcon* pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem("PartnerDetailModel");
		if (pAniIcon)
		{				
			pAniIcon->ClearAniData();
			pAniIcon->SetAnimationData(&temp);
			pAniIcon->SetAnimationIndex(temp.GetBattleAnimation());				
			pAniIcon->SetRotation(SOUTHEAST);
			gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
		}

		UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText("PartnerDetailDay");
		if (pText)
		{
			String remainDay;
			if (mobTemplate->m_basicinfo->TimeLimit > 0)
				remainDay.Format(_T("%d%s"), mobTemplate->m_basicinfo->TimeLimit,gGlobal.GetStringTable()->Get(_T("MSG_DAY")));
			else
				remainDay =gGlobal.GetStringTable()->Get(_T("MSG_INFINITY"));
			pText->SetText(remainDay);
			pText->Redraw();
			pText->SetEnable(true);
		}

		UIImage* img = NULL;
		for (Index i = C_INDEX(1); i < 11; ++i)
		{
			iconName.Format("PartnerDetailRaceType%02d", i);
			img = gGlobal.pUIManager->FindUIImage(iconName.c_str());
			if (img)
				img->SetEnable(false);
		}
		
		iconName.Format("PartnerDetailRaceType%02d", mobTemplate->m_basicinfo->race);
		img = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		if (img)
			img->SetEnable(true);

		gGlobal.pUIManager->AddDirtyUIItem(pPanel);
	}
}

void CMenuPartnerProcess::UpdateRaceType()
{
	StringA iconName;
	UIImage* img = NULL;
	for (Index i = C_INDEX(1); i < 12; ++i)
	{
		iconName.Format("PartnerRaceType%02d", i);
		img = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		if (img)
			img->SetEnable(false);
	}

	if (m_pOnShowPartner && m_pOnShowPartner->m_data)
	{
		iconName.Format("PartnerRaceType%02d", m_pOnShowPartner->m_data->m_basicinfo->race);
		img = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		if (img)
			img->SetEnable(true);
	}
}

CMenuPartnerProcess::~CMenuPartnerProcess()
{
	m_SkillButtonMap.clear();
}


void CMenuPartnerProcess::ShowPublicPartnerDetail(UInt32 char_id, UInt32 Partner_id)
{
	MapService_Proxy::ShowPublicPartnerDetail(gGlobal.pPeerProxy, RPCContext(), char_id, Partner_id, gGlobal.g_pCharacter->GetChar_id());
}

bool CMenuPartnerProcess::HasPartner()
{ 
	return (gGlobal.g_pPartnerGroup->GetCount() > 0);
}


void CMenuPartnerProcess::SetPartnerCollectionList(const UInt32Vector &PartnerIDs)
{
	m_PartnerCollectionMap.clear();
	for (UInt16 i=0; i<PartnerIDs.size(); i++)
	{
		const PartnerCollectionData * pPartnerCollectionData = gGlobal.m_resource->GetPartnerCollectionData(PartnerIDs[i]);
		if (pPartnerCollectionData)
		{
			m_PartnerCollectionMap[PartnerIDs[i]] = pPartnerCollectionData;
			TRACE_INFODTL_1(GLOBAL_LOGGER,_F("SetPartnerCollectionList::(%d)"),PartnerIDs[i]);
		}
	}
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ReSetPartnerCollectionButtonState", String(), m_iPage);
}

void CMenuPartnerProcess::PartnerCollectionNextPage(UInt8 race, bool bNext)
{
	if (bNext)
		m_iPage++;
	else
		m_iPage--;
	UpdatePartnerCollectionMenu(race,m_iPage);
}

void CMenuPartnerProcess::UpdatePartnerCollectionMenu(UInt8 race, UInt8 page)
{
	race = PartnerCollectionRace[race];
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartnerCollectionPanel");
	if (pPanel)
	{
		StringA IconName;
		UIAniIcon* pAniIcon(NULL);
		UIStaticText* pStaticText(NULL);
		UIStaticText* pStaticText1(NULL);
		UIStaticText* pStaticText2(NULL);
		UIStaticText* pStaticText3(NULL);
		PartnerCollectionDataMap PartnerCollectionLst;
		gGlobal.m_resource->GetPartnerCollectionData(PartnerCollectionLst,race);
		UInt8 MaxPage = (UInt8)PartnerCollectionLst.size() / MAX_PARTNER_COLLECTION_EACH_PAGE + 1;
		if (page > MaxPage)
		{
			page = MaxPage;
		}
		else if (page < 1)
		{
			page = 1;
		}
		m_iPage = page;
		pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText("PartnerCollectionPageNo");
		if (pStaticText)
		{
			String PageNo;
			PageNo.Format(_T("%d/%d"),m_iPage,MaxPage);
			pStaticText->SetText(PageNo);
			pStaticText->Redraw();
		}
		PartnerCollectionLst.clear();
		gGlobal.m_resource->GetPartnerCollectionData(PartnerCollectionLst,race,page);
		PartnerCollectionDataMap tempPartnerCollectionMap;
		PartnerCollectionDataMap::iterator Iter = m_PartnerCollectionMap.begin();
		while (Iter != m_PartnerCollectionMap.end())
		{
			if (Iter->second->Type == race && Iter->second->Group == page)
			{
				tempPartnerCollectionMap[Iter->second->ID] = Iter->second;
			}
			Iter++;
		}
		UInt16 offSet = (m_iPage - 1) * MAX_PARTNER_COLLECTION_EACH_PAGE;
		for (UInt16 i = 1 + offSet; i <= MAX_PARTNER_COLLECTION_EACH_PAGE + offSet; i++)
		{
			IconName.Format("PartnerCollection%02d",i - offSet);
			pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(IconName.c_str());
			IconName.Format("PartnerCollectionText%02d",i - offSet);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			IconName.Format("PartnerCollectionBuffDsc%02d1",i - offSet);
			pStaticText1= (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			IconName.Format("PartnerCollectionBuffDsc%02d2",i - offSet);
			pStaticText2= (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			IconName.Format("PartnerCollectionBuffDsc%02d3",i - offSet);
			pStaticText3= (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pAniIcon && pStaticText && pStaticText1 && pStaticText2 && pStaticText3)
			{
				if (i - offSet <= (UInt16)PartnerCollectionLst.size())
				{
					String pFace = _T("QuestionMark.ent");
					//if (Iter->second->Light > 0)
					//{
					//	pFace = _T("QuestionMarkWithLight.ent");
					//}
					UInt32 HaveCollection(1);
					MobDB_Client * mobTemplate(NULL);
					Iter = tempPartnerCollectionMap.find(i);
					if (Iter != tempPartnerCollectionMap.end())
					{
						HaveCollection = 0;
					}
					Iter = PartnerCollectionLst.find(i);
					if (Iter != PartnerCollectionLst.end())
					{
						mobTemplate = gGlobal.LoadMobTemplate(Iter->second->MOBid);
					}
					if (mobTemplate)
					{
						pAniIcon->SetRenderStyle(HaveCollection);
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(mobTemplate->m_basicinfo->mob_faceS);
						pAniIcon->SetEnable(true);
						pStaticText->SetText(mobTemplate->m_basicinfo->mid_name);
						pStaticText->Redraw();
						pStaticText->SetEnable(true);

						UInt8 AttriCount(0);
						String buffDsc;
						if (Iter->second->STR)
						{
							buffDsc.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_STR")),Iter->second->STR);
							AttriCount++;
							switch (AttriCount)
							{
							case 1:
								pStaticText1->SetText(buffDsc);
								pStaticText1->Redraw();
								pStaticText1->SetEnable(true);
								break;
							case 2:
								pStaticText2->SetText(buffDsc);
								pStaticText2->Redraw();
								pStaticText2->SetEnable(true);
								break;
							case 3:
								pStaticText3->SetText(buffDsc);
								pStaticText3->Redraw();
								pStaticText3->SetEnable(true);
								break;
							}
						}
						if (Iter->second->CON)
						{
							buffDsc.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_CON")),Iter->second->CON);
							AttriCount++;
							switch (AttriCount)
							{
							case 1:
								pStaticText1->SetText(buffDsc);
								pStaticText1->Redraw();
								pStaticText1->SetEnable(true);
								break;
							case 2:
								pStaticText2->SetText(buffDsc);
								pStaticText2->Redraw();
								pStaticText2->SetEnable(true);
								break;
							case 3:
								pStaticText3->SetText(buffDsc);
								pStaticText3->Redraw();
								pStaticText3->SetEnable(true);
								break;
							}
						}
						if (Iter->second->STA)
						{
							buffDsc.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_STA")),Iter->second->STA);
							AttriCount++;
							switch (AttriCount)
							{
							case 1:
								pStaticText1->SetText(buffDsc);
								pStaticText1->Redraw();
								pStaticText1->SetEnable(true);
								break;
							case 2:
								pStaticText2->SetText(buffDsc);
								pStaticText2->Redraw();
								pStaticText2->SetEnable(true);
								break;
							case 3:
								pStaticText3->SetText(buffDsc);
								pStaticText3->Redraw();
								pStaticText3->SetEnable(true);
								break;
							}
						}
						if (Iter->second->AGI)
						{
							buffDsc.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_AGI")),Iter->second->AGI);
							AttriCount++;
							switch (AttriCount)
							{
							case 1:
								pStaticText1->SetText(buffDsc);
								pStaticText1->Redraw();
								pStaticText1->SetEnable(true);
								break;
							case 2:
								pStaticText2->SetText(buffDsc);
								pStaticText2->Redraw();
								pStaticText2->SetEnable(true);
								break;
							case 3:
								pStaticText3->SetText(buffDsc);
								pStaticText3->Redraw();
								pStaticText3->SetEnable(true);
								break;
							}
						}
						if (Iter->second->SPR)
						{
							buffDsc.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_SPR")),Iter->second->SPR);
							AttriCount++;
							switch (AttriCount)
							{
							case 1:
								pStaticText1->SetText(buffDsc);
								pStaticText1->Redraw();
								pStaticText1->SetEnable(true);
								break;
							case 2:
								pStaticText2->SetText(buffDsc);
								pStaticText2->Redraw();
								pStaticText2->SetEnable(true);
								break;
							case 3:
								pStaticText3->SetText(buffDsc);
								pStaticText3->Redraw();
								pStaticText3->SetEnable(true);
								break;
							}
						}
						pStaticText->SetColor(TextColor[ColorIndex_LightGrey]);
						pStaticText1->SetColor(TextColor[ColorIndex_LightGrey]);
						pStaticText2->SetColor(TextColor[ColorIndex_LightGrey]);
						pStaticText3->SetColor(TextColor[ColorIndex_LightGrey]);
						switch (AttriCount)
						{
						case 0:
							pStaticText1->SetEnable(false);
							pStaticText2->SetEnable(false);
							pStaticText3->SetEnable(false);
							break;
						case 1:
							pStaticText2->SetEnable(false);
							pStaticText3->SetEnable(false);
							break;
						case 2:
							pStaticText3->SetEnable(false);
							break;
						}
					}
				}
				else
				{
					pAniIcon->SetEnable(false);
					pStaticText->SetEnable(false);
					pStaticText1->SetEnable(false);
					pStaticText2->SetEnable(false);
					pStaticText3->SetEnable(false);
				}
			}
		}
		//show the partner already collect
		//UInt16 LastID = offSet;
		for (UInt16 i = 1 + offSet; i <= MAX_PARTNER_COLLECTION_EACH_PAGE + offSet; i++)
		{
			Iter = tempPartnerCollectionMap.find(i);
			if (Iter != tempPartnerCollectionMap.end())// && LastID == (i-1))
			{
				//LastID = i;
				IconName.Format("PartnerCollectionText%02d",i - offSet);
				pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
				IconName.Format("PartnerCollectionBuffDsc%02d1",i - offSet);
				pStaticText1= (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
				IconName.Format("PartnerCollectionBuffDsc%02d2",i - offSet);
				pStaticText2= (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
				IconName.Format("PartnerCollectionBuffDsc%02d3",i - offSet);
				pStaticText3= (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
				if (pStaticText && pStaticText1 && pStaticText2 && pStaticText3)
				{
					pStaticText->SetColor(TextColor[ColorIndex_Yellow]);
					pStaticText1->SetColor(TextColor[ColorIndex_Yellow]);
					pStaticText2->SetColor(TextColor[ColorIndex_Yellow]);
					pStaticText3->SetColor(TextColor[ColorIndex_Yellow]);
					pStaticText->Redraw();
					pStaticText1->Redraw();
					pStaticText2->Redraw();
					pStaticText3->Redraw();
				}
			}
		}
		AttriStruct2 totalAttri;
		UInt16	totalCount(0);
		Iter = m_PartnerCollectionMap.begin();
		while (Iter != m_PartnerCollectionMap.end())
		{
			if (Iter->second->Type == race)
			{
				totalAttri.SetSta(totalAttri.GetSta() + Iter->second->STA);
				totalAttri.SetAgi(totalAttri.GetAgi() + Iter->second->AGI);
				totalAttri.SetCon(totalAttri.GetCon() + Iter->second->CON);
				totalAttri.SetSpr(totalAttri.GetSpr() + Iter->second->SPR);
				totalAttri.SetStr(totalAttri.GetStr() + Iter->second->STR);
				totalCount++;
			}
			++Iter;
		}
		PartnerCollectionLst.clear();
		gGlobal.m_resource->GetPartnerCollectionData(PartnerCollectionLst,race);
		Iter = PartnerCollectionLst.find(totalCount);
		if (Iter != PartnerCollectionLst.end())
		{
			totalCount = Iter->second->Light;
		}
		String strLight;
		for(UInt8 i=0; i<MAX_PARTNER_COLLECTION_LIGHT; i++)
		{
			IconName.Format("PartnerCollectionLight%d",i + 1);
			pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(IconName.c_str());
			if (pAniIcon)
			{
				pAniIcon->ClearAniData();
				if (totalCount > 0)
				{
					strLight.Format(_T("PartnerCollection%d.ent"),race);
					totalCount--;
				} 
				else
				{
					strLight.Format(_T("PartnerCollectionBase%d.ent"),race);
				}
				pAniIcon->SetAnimationData(strLight.c_str());
				pAniIcon->SetEnable(true);
			}
		}
		//update total add
		String AttriValue;
		pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText("PartnerCollectionTextSTR");
		if (pStaticText)
		{
			AttriValue.Format(_T("+%d"),totalAttri.GetStr());
			pStaticText->SetText(AttriValue);
			pStaticText->Redraw();
		}
		pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText("PartnerCollectionTextCON");
		if (pStaticText)
		{
			AttriValue.Format(_T("+%d"),totalAttri.GetCon());
			pStaticText->SetText(AttriValue);
			pStaticText->Redraw();
		}
		pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText("PartnerCollectionTextSTA");
		if (pStaticText)
		{
			AttriValue.Format(_T("+%d"),totalAttri.GetSta());
			pStaticText->SetText(AttriValue);
			pStaticText->Redraw();
		}
		pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText("PartnerCollectionTextAGI");
		if (pStaticText)
		{
			AttriValue.Format(_T("+%d"),totalAttri.GetAgi());
			pStaticText->SetText(AttriValue);
			pStaticText->Redraw();
		}
		pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText("PartnerCollectionTextSPR");
		if (pStaticText)
		{
			AttriValue.Format(_T("+%d"),totalAttri.GetSpr());
			pStaticText->SetText(AttriValue);
			pStaticText->Redraw();
		}

		gGlobal.pUIManager->AddDirtyUIItem(pPanel);	
	}
}

void CMenuPartnerProcess::ShowPartnerCollectionDetail(UInt8 race)
{
	race = PartnerCollectionRace[race];
	UIAniIcon* pAniIcon(NULL);
	UIPanel* pPanel2 = gGlobal.pUIManager->FindUIPanel("PartnerCollectionPanel");
	if (pPanel2)
	{
		pAniIcon = (UIAniIcon*)pPanel2->GetMouseOver();
	}
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartnerDescPanel");
	if (pPanel && pAniIcon)
	{
		gGlobal.pUIManager->SetPanel2Top(pPanel);
		UIImage* pPartnerImage = gGlobal.pUIManager->FindUIImage("PartnerFaceImage");
		UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel("PartnerDesc");
		PartnerCollectionDataMap PartnerCollectionLst;
		gGlobal.m_resource->GetPartnerCollectionData(PartnerCollectionLst,race,m_iPage);
		MobDB_Client * mobTemplate(NULL);
		const char *pAniIconName = pAniIcon->GetName()->c_str();
		pAniIconName = pAniIconName+pAniIcon->GetName()->GetLength()-2;
		UInt8 Num = atoi(pAniIconName) + MAX_PARTNER_COLLECTION_EACH_PAGE * (m_iPage - 1);

		PartnerCollectionDataMap::iterator Iter = PartnerCollectionLst.find(Num);
		if (Iter != PartnerCollectionLst.end())
		{
			mobTemplate = gGlobal.LoadMobTemplate(Iter->second->MOBid);
		}

		if (pPartnerImage && pChatPanel && mobTemplate)
		{
			String partnerFace = String(mobTemplate->m_basicinfo->mob_faceL);
			String str = _T("Data\\");
			if (partnerFace.length() > 0)
			{
				str.Format(_T("%s%s"), str.c_str(), partnerFace.c_str());
				pPartnerImage->SetImageFile(str.c_str());
				pPartnerImage->SetEnable(true);
			}
			else
				pPartnerImage->SetEnable(false);

			String mobDesc = gGlobal.m_resource->GetMobDesc(mobTemplate->m_basicinfo->desc_id);
			pChatPanel->ClearAllRow();
			pChatPanel->AddStringW(mobDesc.c_str());
			pChatPanel->EndRow();
			pChatPanel->Redraw();

			gGlobal.pUIManager->AddDirtyUIItem(pChatPanel);

		}
	}
}

void CMenuPartnerProcess::GetPartnerCollectionAttri(AttriStruct2 & Attri)
{
	if (!(!gGlobal.m_bUsePartnerCollectionBuff && m_pOnShowPartner && m_pOnShowPartner->m_data && m_pOnShowPartner->m_data->m_basicinfo))
		return;
	UInt8 race = m_pOnShowPartner->m_data->m_basicinfo->race;
	PartnerCollectionDataMap PartnerCollectionLst;
	PartnerCollectionDataMap::iterator Iter = m_PartnerCollectionMap.begin();
	while (Iter != m_PartnerCollectionMap.end())
	{
		if (Iter->second->Type == race)
		{
			Attri.SetSta(Attri.GetSta() + Iter->second->STA);
			Attri.SetAgi(Attri.GetAgi() + Iter->second->AGI);
			Attri.SetCon(Attri.GetCon() + Iter->second->CON);
			Attri.SetSpr(Attri.GetSpr() + Iter->second->SPR);
			Attri.SetStr(Attri.GetStr() + Iter->second->STR);
		}
		++Iter;
	}
}

void CMenuPartnerProcess::PartnerCollectionUseBuffTemp(bool bUse)
{
	//check whether have the buff
	if (!(m_pOnShowPartner && m_pOnShowPartner->m_data && m_pOnShowPartner->m_data->m_basicinfo))
		return;
	UIButton* UseBuffTemp = (UIButton*)gGlobal.pUIManager->GetUIItem("PartnerCollectionUseBuffTemp");
	UIButton* UnUseBuffTemp = (UIButton*)gGlobal.pUIManager->GetUIItem("PartnerCollectionUnUseBuffTemp");
	CCharacter* pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (UseBuffTemp && UnUseBuffTemp && pchar && gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerGroup->GetCount())
	{
		pchar->UsePartnerCollectionBuff(gGlobal.pPeerProxy, RPCContext(),bUse);
		UseBuffTemp->SetEnable(!bUse);
		UnUseBuffTemp->SetEnable(bUse);
	}
}

void CMenuPartnerProcess::DragPartner()
{
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerGroup->GetCount() > 0)
	{
		CPartner* partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(m_iSelectedIndex);
		UIAniIcon* partnerFace = gGlobal.pUIManager->FindUIAniIcon("PartnerCollectionMovingIcon");
		if (partner && partner->m_data && partnerFace)
		{	
			partnerFace->SetEnable(true);

			String pFace = String(partner->m_data->m_basicinfo->mob_faceS);

			partnerFace->ClearAniData();
			partnerFace->SetAnimationData(pFace.c_str());
			gGlobal.pUIManager->AddDirtyUIItem(partnerFace);
		}
	}
}

void CMenuPartnerProcess::AddPartnerCollection()
{
	CCharacter* pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar && m_pOnShowPartner)
	{
		pchar->AddPartnerCollection(gGlobal.pPeerProxy, RPCContext(),m_pOnShowPartner->GetPartner_id());
	}
}

void CMenuPartnerProcess::UpdateCultivation()
{
	if (m_pOnShowPartner)
		SetNumField("PartnerCultivation", m_pOnShowPartner->GetCultivation());
}

void CMenuPartnerProcess::UpdatePractice()
{
	if (m_pOnShowPartner)
		SetNumField("PartnerPractice", m_pOnShowPartner->GetPractice());
}

void CMenuPartnerProcess::PartnerEatFastFood(UInt8 eatType)
{
	if (m_pOnShowPartner)
	{
		if (gGlobal.m_pBag)
		{
			gGlobal.m_pBag->UseFastFoodItem(gGlobal.pPeerProxy, gGlobal.g_context,eatType,m_pOnShowPartner->GetPartner_id());
		}
	}
}