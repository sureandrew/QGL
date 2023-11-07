//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuSkillProcess.h"
#include "MenuGuildProcess.h"
//-- Library
#include "Global.h"
#include "Configure.h"
#include "Proxy/Logic/Skill.h"
#include "Proxy/Logic/SkillGroup.h"
#include "Proxy/Logic/GuildMember.h"
#include "Resource/resskill.h"
#include "Resource/resskillshop.h"
#include "Resource/ClientResManager.h"
#include "UI.h"
#include "QGLApplication.h"
#include "Common/CallLuaFunc.h"
#include "LuaPush.h"
#include "BattleControl.h"

PSTRINGA GangCheckName = "SkillCheckGang";
PSTRINGA LiveCheckName = "SkillCheckLive";
PSTRINGA GuildCheckName = "SkillCheckGuild";
PSTRINGA SpecialCheckName = "SkillCheckSpecial";
PSTRINGA BattleCheckName = "SkillCheckBattle";

PSTRINGA SkillMainSelPrefix = "SkillMainSel";
PSTRINGA SkillMainSelIconPrefix = "SkillMainSelIcon";
PSTRINGA SkillMainSelNamePrefix = "SkillMainSelName";
PSTRINGA SkillMainSelLevelPrefix = "SkillMainSelLevel";

PSTRINGA SkillSubSelPrefix = "SkillSubSel";
PSTRINGA SkillSubSelIconPrefix = "SkillSubSelIcon";
PSTRINGA SkillSubSelNamePrefix = "SkillSubSelName";
PSTRINGA SkillSubSelUsePrefix = "SkillSubSelUse";
PSTRINGA SkillSubSelUseValuePrefix = "SkillSubSelUseValue";

PSTRINGA SkillMainScroll = "SkillMainMenuScrollBar";
PSTRINGA SkillSubScroll = "SkillSubMenuScrollBar";

PSTRINGA SkillPanelName = "SkillPanel";

PSTRINGA SkillDetailPanelName = "SkillDetailMenuPanel";
PSTRINGA SkillDetailXMLName = "Data\\SkillDetail.xml";
PSTRINGA SkillChat = "SkillDetailChat";
PSTRINGA SkillDetailName = "SkillDetailName";
PSTRING TempSkillEntName = _T("skill.ent");

PSTRINGA SkillShopLevelValue = "SkillShopLevelValue";
PSTRINGA SkillShopExpValue = "SkillShopExpValue";
PSTRINGA SkillShopMoneyValue = "SkillShopMoneyValue";
PSTRINGA SkillShopSaveValue = "SkillShopSaveValue";
PSTRINGA SkillShopHonorValue = "SkillShopHonorValue";


CONST Int ErrorReqLv = 999;

CMenuSkillProcess::CMenuSkillProcess()
{
	m_CurrSkillType = 0;
	m_MainSkillStartIndex = 0;
	m_SubSkillStartIndex = 0;
	m_totalSubSkill = 0;
	m_pTempSkillData = NULL;
	m_pTempSkillDetailData = NULL;

	for (Int i = 0; i < MAX_MAIN_SKILL; i++)
	{
		m_MainSkillCurLst[i] = NULL;
	}

	for (Int i = 0; i < MAX_SUB_SKILL; i++)
	{
		m_SubSkillCurLst[i] = NULL;
	}

	m_SkillShopID = 0;
}

CMenuSkillProcess::~CMenuSkillProcess()
{

	

	for (Int i = 0; i < MAX_SKILL_TYPE; i ++)
		m_SkillGroups[i].clear();
	m_SkillBaseChildRel.clear();
	
}

BYTE CMenuSkillProcess::FindSkillTypeByBase(BYTE Base)
{
	switch (Base)
	{
	case SKILL_TYPE_GANG:
		return SKILL_TYPE_GANG_BASE;
	case SKILL_TYPE_LIVE:
		return SKILL_TYPE_LIVE_BASE;
	case SKILL_TYPE_SPECIAL:
		return SKILL_TYPE_SPECIAL_BASE;
	}
	return 255;
}
void CMenuSkillProcess::UpdateMainSkillMenu(Int Index)
{
	/*if (!gConfigure.uNetwork)
	{
		StringA buf;
		buf.Format("%s%d", SkillMainSelPrefix.c_str(), Index);
		UICheckBox * pCheckBox = gGlobal.pUIManager->FindUICheckBox(buf.c_str());
		if (pCheckBox)
			pCheckBox->SetState(true);
		return;
	}*/

	if ((Int)m_SkillGroups[m_CurrSkillType].size() >= Index + (Int)m_MainSkillStartIndex)
	{
		HideAllSubSkillButton();
		//find the skill data first
		m_MainSkillSelIndex = Index - 1;
		StringA buf;
		buf.Format("%s%d", SkillMainSelPrefix, Index);
		UICheckBox * pCheckBox = gGlobal.pUIManager->FindUICheckBox(buf.c_str());
		if (pCheckBox)
			pCheckBox->SetState(true);
		
		if (m_MainSkillCurLst[m_MainSkillSelIndex])
			m_totalSubSkill = (WORD)m_SkillBaseChildRel.count(m_MainSkillCurLst[m_MainSkillSelIndex]->id);

		UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem(SkillSubScroll);
		if (scrollBar)
		{
			if (m_totalSubSkill > MAX_SUB_SKILL)
			{
				scrollBar->SetNumCount(m_totalSubSkill);
			}

		}

		RefreshSubSkillMenu();
		if (m_SkillShopID > 0)
			RefreshShopDetail();
	}
	
}
void CMenuSkillProcess::UpdateSubSkillMenu(Int Index)
{
	/*if (!gConfigure.uNetwork)
	{
		StringA buf;
		buf.Format("%s%d", SkillSubSelPrefix.c_str(), Index);
		UICheckBox * pCheckBox = gGlobal.pUIManager->FindUICheckBox(buf.c_str());
		if (pCheckBox)
			pCheckBox->SetState(true);
		return;
	}*/

	if (m_totalSubSkill >= Index + m_SubSkillStartIndex)
	{
		m_SubSkillSelIndex = Index - 1;
		StringA buf;
		buf.Format("%s%d", SkillSubSelPrefix, Index);
		UICheckBox * pCheckBox = gGlobal.pUIManager->FindUICheckBox(buf.c_str());
		if (pCheckBox)
			pCheckBox->SetState(true);
	}
	//UInt CurrIndex = 0;
}

void CMenuSkillProcess::UpdateSkillMenu()
{
	//if (!gConfigure.uNetwork)
	//	return;
	
	//m_MainSkillStartIndex = 0;

	//check current skill type
	UICheckBox * pCheckGangBox = gGlobal.pUIManager->FindUICheckBox(GangCheckName);
	UICheckBox * pCheckLiveBox = gGlobal.pUIManager->FindUICheckBox(LiveCheckName);
	UICheckBox * pCheckGuildgBox = gGlobal.pUIManager->FindUICheckBox(GuildCheckName);
	UICheckBox * pCheckBattleBox = gGlobal.pUIManager->FindUICheckBox(BattleCheckName);
	UICheckBox * pCheckSpecialBox = gGlobal.pUIManager->FindUICheckBox(SpecialCheckName);
	
	if (pCheckGangBox && pCheckGangBox->GetState() && pCheckGangBox->IsEnable())
		m_CurrSkillType = SKILL_TYPE_GANG_BASE;
	else if (pCheckLiveBox && pCheckLiveBox->GetState() && pCheckLiveBox->IsEnable())
		m_CurrSkillType = SKILL_TYPE_LIVE_BASE;
	else if (pCheckGuildgBox && pCheckGuildgBox->GetState() && pCheckGuildgBox->IsEnable())
		m_CurrSkillType = SKILL_TYPE_GUILD;
	else if (pCheckBattleBox && pCheckBattleBox->GetState() && pCheckBattleBox->IsEnable())
		m_CurrSkillType = SKILL_TYPE_BATTLE;
	else if (pCheckSpecialBox && pCheckSpecialBox->GetState() && pCheckSpecialBox->IsEnable())
		m_CurrSkillType = SKILL_TYPE_SPECIAL_BASE;

	HideAllSkillButton();

	ResetPosition();

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem(SkillMainScroll);
	if (scrollBar)
	{
		if (m_SkillGroups[m_CurrSkillType].size() > MAX_MAIN_SKILL)
		{
			scrollBar->SetNumCount((Int)m_SkillGroups[m_CurrSkillType].size());
		}

	}
	//Add all base skill
	RefreshMainSkillMenu();
	RefreshSubSkillMenu();

	UIPanel * pPanel= gGlobal.pUIManager->FindUIPanel(SkillPanelName);
	if (pPanel && pPanel->IsEnable())
		gGlobal.pUIManager->AddDirtyUIItem(pPanel);

}

void CMenuSkillProcess::RefreshSubSkillMenu()
{
	UInt UIStartIndex = m_MainSkillSelIndex + 1;

	UInt CurrIndex = 0;

	for (Int i = 0; i < MAX_SUB_SKILL; i++)
		m_SubSkillCurLst[i] = NULL;

	for (StlMap<UInt16, SkillComData>::iterator Itr = m_SkillGroups[m_CurrSkillType].begin(); 
		Itr != m_SkillGroups[m_CurrSkillType].end() ; ++Itr, ++CurrIndex)
	{
		if (Itr->second.pSkill == NULL && m_SkillShopID == 0)
			continue;

		if (UIStartIndex > MAX_MAIN_SKILL)
		{
			break;
		}

		if (CurrIndex + 1 == m_MainSkillStartIndex + UIStartIndex)
		{
			StringA buf;

			//find all sub skills insert to the list
			StlMultiMap<WORD, SkillComData>::iterator ItrSub = m_SkillBaseChildRel.find(Itr->first);
			if (ItrSub == m_SkillBaseChildRel.end())
				break;
			
			m_totalSubSkill = (WORD)m_SkillBaseChildRel.count(Itr->first);

			UInt UISubStartIndex = 1;
			Int peaceCount = 0;
			for (Int i = 0; i < m_totalSubSkill; ++i, ++ItrSub)
			{
				if (UISubStartIndex > MAX_SUB_SKILL)
					break;

				if (m_CurrSkillType == SKILL_TYPE_BATTLE &&
					ItrSub->second.pSkillData->battleorField == SKILL_USE_TIME_PEACE)
				{
					peaceCount ++;
					continue;
				}

				BOOL notEnoughLv = FALSE;
				gGlobal.g_TempSkillString = "";
				gGlobal.g_bSkillFail = FALSE;
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE && 
					gGlobal.g_pBattleControl->GetControlPlayer())
				{
					if (gGlobal.g_pBattleControl->GetControlPlayer()->GetSkillLv(ItrSub->second.pSkillData->reqBaseSkill)
						< ItrSub->second.pSkillData->reqBaseSkillRank ||
						gGlobal.g_pBattleControl->GetControlPlayer()->GetLevel() < ItrSub->second.pSkillData->reqLv)
					{
						gGlobal.g_bSkillFail = TRUE;
						notEnoughLv = TRUE;
					}
					else
					{
						StringA FuncName;
						FuncName.Format("%s%s", gGlobal.GetPrefixStr(ItrSub->second.pSkillData->id, "skill"), CommonSkillFuncName[SKILL_FUNC_NAME_CANUSE].c_str());
						CallLuaFunc(gGlobal.g_pParentLua, FuncName.c_str(), String(), gGlobal.g_pBattleControl->GetControlPlayer(),
						gGlobal.g_pBattleControl->GetControlPlayer()->GetSkillLv(ItrSub->second.pSkillData->reqBaseSkill), ItrSub->second.pSkillData->reqBaseSkillRank,
						ItrSub->second.pSkillData->reqLv);
					}
				}
				else
				{
					if (gGlobal.g_pCharacter->GetSkillLv(ItrSub->second.pSkillData->reqBaseSkill)
						< ItrSub->second.pSkillData->reqBaseSkillRank ||
						gGlobal.g_pCharacter->GetLevel() < ItrSub->second.pSkillData->reqLv)
					{
						gGlobal.g_bSkillFail = TRUE;
						notEnoughLv = TRUE;
					}
					else
					{
						StringA FuncName;
						FuncName.Format("%s%s", gGlobal.GetPrefixStr(ItrSub->second.pSkillData->id, "skill"), CommonSkillFuncName[SKILL_FUNC_NAME_CANUSE].c_str());
						CallLuaFunc(gGlobal.g_pParentLua, FuncName.c_str(), String(), gGlobal.g_pCharacter,
						gGlobal.g_pCharacter->GetSkillLv(ItrSub->second.pSkillData->reqBaseSkill), ItrSub->second.pSkillData->reqBaseSkillRank,
						ItrSub->second.pSkillData->reqLv);
					}
				}
				m_SubSkillCurLst[UISubStartIndex - 1] = ItrSub->second.pSkillData;

				//check whether
				StringA subBuf;
				subBuf.Format("%s%d", SkillSubSelIconPrefix, UISubStartIndex);

				UIAniIcon * pAniIcon = (UIAniIcon*) gGlobal.pUIManager->GetUIItem(subBuf.c_str());
				if (pAniIcon)
				{
					if (gConfigure.uNetwork)
					{
						if (GetIsShop())
						{
							if (notEnoughLv)
								pAniIcon->SetRenderStyle(1);
							else
								pAniIcon->SetRenderStyle(0);
						}
						else
						{
							if (gGlobal.g_bSkillFail)
								pAniIcon->SetRenderStyle(1);
							else
								pAniIcon->SetRenderStyle(0);
						}
					}

					if (!String(ItrSub->second.pSkillData->icon).empty())
					{
						CAnimatedEntity Ani;
						Ani.LoadEntityFile(ItrSub->second.pSkillData->icon, gGlobal.pD3DDevice, EQUIP_OTHERS);
						if (Ani.HasAniFile())
							pAniIcon->SetAnimationData(&Ani);//(Itr->second.pSkillData->icon);
						else
						{
							CAnimatedEntity Ani;
							Ani.LoadEntityFile(TempSkillEntName, gGlobal.pD3DDevice, EQUIP_OTHERS);
							if (Ani.HasAniFile())
								pAniIcon->SetAnimationData(&Ani);//(Itr->second.pSkillData->icon);
						}
					}
					else
					{
						CAnimatedEntity Ani;
						Ani.LoadEntityFile(TempSkillEntName, gGlobal.pD3DDevice, EQUIP_OTHERS);
						if (Ani.HasAniFile())
							pAniIcon->SetAnimationData(&Ani);//(Itr->second.pSkillData->icon);
					}
					pAniIcon->SetEnable(true);
				}

				subBuf.Format("%s%d", SkillSubSelNamePrefix, UISubStartIndex);
				UIStaticText * pTextName = gGlobal.pUIManager->FindUIStaticText(subBuf.c_str());
				if (pTextName)
				{
					pTextName->SetText(ItrSub->second.pSkillData->name);
					pTextName->FillImage();
					pTextName->DrawFont();
					pTextName->SetEnable(true);
				}

				if (gConfigure.uNetwork && !notEnoughLv)
				{
					subBuf.Format("%s%d", SkillSubSelUsePrefix, UISubStartIndex);
					UIStaticText * pTextUse = gGlobal.pUIManager->FindUIStaticText(subBuf.c_str());
					if (pTextUse)
					{
						pTextUse->SetEnable(true);
					}

					subBuf.Format("%s%d", SkillSubSelUseValuePrefix, UISubStartIndex);
					UIStaticText * pTextUseValue = gGlobal.pUIManager->FindUIStaticText(subBuf.c_str());
					if (pTextUseValue && ItrSub->second.pSkill)
					{
						String CombinedText;

						StringA FuncName;
						FuncName.Format("%s%s", gGlobal.GetPrefixStr(ItrSub->second.pSkillData->id, "skill"), CommonSkillFuncName[SKILL_FUNC_NAME_CONSUME].c_str());
						gGlobal.g_TempSkillString = "";
						CallLuaFunc(gGlobal.g_pParentLua, FuncName.c_str(), String(), Itr->second.pSkill->GetSkill_level());
						pTextUseValue->SetText(gGlobal.g_TempSkillString);
						pTextUseValue->FillImage();
						pTextUseValue->DrawFont();
						pTextUseValue->SetEnable(true);
					}
				}

				 ++UISubStartIndex;
				//SkillSubSelUsePrefix;
				//SkillSubSelUseValuePrefix;
			}

			m_totalSubSkill -= peaceCount;
		}
	}

}
void CMenuSkillProcess::SkillMenuMainUp()
{
	if (m_MainSkillStartIndex > 0)
	{
		m_MainSkillStartIndex --;

		RefreshMainSkillMenu();
		for (Int i = 1; i <= MAX_MAIN_SKILL; i++)
		{
			StringA buf;
			buf.Format("%s%d", SkillMainSelPrefix, i);
			UICheckBox * pCheckBox = gGlobal.pUIManager->FindUICheckBox(buf.c_str());
			if (pCheckBox &&
				pCheckBox->GetState())
			{
				UpdateMainSkillMenu(i);
				break;
			}
		}
	}
}
void CMenuSkillProcess::RefreshMainSkillMenu()
{
	UInt UIStartIndex = 1;

	UInt CurrIndex = 0;

	for (Int i = 0; i < MAX_MAIN_SKILL; i++)
		m_MainSkillCurLst[i] = NULL;

	for (StlMap<UInt16, SkillComData>::iterator Itr = m_SkillGroups[m_CurrSkillType].begin(); 
		Itr != m_SkillGroups[m_CurrSkillType].end() ; ++Itr)
	{
		if (UIStartIndex > MAX_MAIN_SKILL)
		{
			break;
		}

		if (CurrIndex >= m_MainSkillStartIndex)
		{
			m_MainSkillCurLst[UIStartIndex - 1] = Itr->second.pSkillData;

			StringA buf;
			buf.Format("%s%d", SkillMainSelIconPrefix, UIStartIndex);

			UIAniIcon * pAniIcon = (UIAniIcon*) gGlobal.pUIManager->GetUIItem(buf.c_str());
			if (pAniIcon)
			{
				if (!String(Itr->second.pSkillData->icon).empty())
				{
					CAnimatedEntity Ani;
					Ani.LoadEntityFile(Itr->second.pSkillData->icon, gGlobal.pD3DDevice, EQUIP_OTHERS);
					if (Ani.HasAniFile())
						pAniIcon->SetAnimationData(&Ani);
					else
					{
						CAnimatedEntity Ani;
						Ani.LoadEntityFile(TempSkillEntName, gGlobal.pD3DDevice, EQUIP_OTHERS);
						if (Ani.HasAniFile())
							pAniIcon->SetAnimationData(&Ani);//(Itr->second.pSkillData->icon);
					}
				}
				else
				{
					CAnimatedEntity Ani;
					Ani.LoadEntityFile(TempSkillEntName, gGlobal.pD3DDevice, EQUIP_OTHERS);
					if (Ani.HasAniFile())
						pAniIcon->SetAnimationData(&Ani);//(Itr->second.pSkillData->icon);
				}
				pAniIcon->SetEnable(true);
			}
				
			buf.Format("%s%d", SkillMainSelLevelPrefix, UIStartIndex);
			UIStaticText * pTextLv = gGlobal.pUIManager->FindUIStaticText(buf.c_str());
			if (pTextLv)
			{
				if (Itr->second.pSkill)
				{
					String LvBuf;
					LvBuf.Format(_T("%s%d"),gGlobal.GetStringTable()->Get(_T("MSG_SKILL_LEVEL")),
						Itr->second.pSkill->GetSkill_level());
					pTextLv->SetText(LvBuf.c_str());
					pTextLv->FillImage();
					pTextLv->DrawFont();
					pTextLv->SetEnable(true);
				}
			}

			buf.Format("%s%d", SkillMainSelNamePrefix, UIStartIndex);
			UIStaticText * pTextName = gGlobal.pUIManager->FindUIStaticText(buf.c_str());
			if (pTextName)
			{
				pTextName->SetText(Itr->second.pSkillData->name);
				pTextName->FillImage();
				pTextName->DrawFont();
				pTextName->SetEnable(true);
			}
			
			//if (
			UIStartIndex ++;
		}
		CurrIndex ++;
	}
}
void CMenuSkillProcess::SkillMenuMainDown()
{
	if (m_MainSkillStartIndex + MAX_MAIN_SKILL < (Int)m_SkillGroups[m_CurrSkillType].size())
	{
		m_MainSkillStartIndex ++;

		RefreshMainSkillMenu();
		for (Int i = 1; i <= MAX_MAIN_SKILL; i++)
		{
			StringA buf;
			buf.Format("%s%d", SkillMainSelPrefix, i);
			UICheckBox * pCheckBox = gGlobal.pUIManager->FindUICheckBox(buf.c_str());
			if (pCheckBox &&
				pCheckBox->GetState())
			{
				UpdateMainSkillMenu(i);
				break;
			}
		}
	}
}
void CMenuSkillProcess::SkillMenuMainSubUp()
{
	if (m_SubSkillStartIndex > 0)
	{
		m_SubSkillStartIndex --;
		RefreshSubSkillMenu();
			
	}
}
void CMenuSkillProcess::SkillMenuMainSubDown()
{
	if (m_SubSkillStartIndex + MAX_SUB_SKILL < m_totalSubSkill)
	{
		m_SubSkillStartIndex ++;
		RefreshSubSkillMenu();
		
	}
}
void CMenuSkillProcess::SkillMenuMainScroll()
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem(SkillMainScroll);
	if (scrollBar)
	{
		WORD PreIndex = m_MainSkillStartIndex;
		if ((Int)m_SkillGroups[m_CurrSkillType].size() <= MAX_MAIN_SKILL)
		{
			m_MainSkillStartIndex = 0;
		}
		else if (scrollBar->GetSelNum() + MAX_MAIN_SKILL >= m_SkillGroups[m_CurrSkillType].size())
		{
			m_MainSkillStartIndex = (Int)m_SkillGroups[m_CurrSkillType].size() - MAX_MAIN_SKILL;
		}
		else
			m_MainSkillStartIndex = scrollBar->GetSelNum();

		if (PreIndex != m_MainSkillStartIndex)
		{
			RefreshMainSkillMenu();

			for (Int i = 1; i <= MAX_MAIN_SKILL; i++)
			{
				StringA buf;
				buf.Format("%s%d", SkillMainSelPrefix, i);
				UICheckBox * pCheckBox = gGlobal.pUIManager->FindUICheckBox(buf.c_str());
				if (pCheckBox &&
					pCheckBox->GetState())
				{
					UpdateMainSkillMenu(i);
					break;
				}
			}

			gGlobal.pUIManager->AddDirtyUIItem(gGlobal.pUIManager->GetUIItem(SkillPanelName));
	
		}
	}

	
}
void CMenuSkillProcess::SkillMenuSubScroll()
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem(SkillSubScroll);
	if (scrollBar)
	{
		WORD PreIndex = m_SubSkillSelIndex;
		if (scrollBar->GetSelNum() + MAX_SUB_SKILL >= m_totalSubSkill)
			m_SubSkillSelIndex = m_totalSubSkill - MAX_SUB_SKILL;
		else
			m_SubSkillSelIndex = scrollBar->GetSelNum();
		if (PreIndex != m_SubSkillSelIndex)
		{
			RefreshSubSkillMenu();
			gGlobal.pUIManager->AddDirtyUIItem(gGlobal.pUIManager->GetUIItem(SkillPanelName));
		}

	}
}

void CMenuSkillProcess::HideAllSubSkillButton()
{
	for (Int i = 1; i <= MAX_SUB_SKILL; i++)
	{
		StringA Buf;

		Buf.Format("%s%d", SkillSubSelPrefix, i);

		UICheckBox * pChkBox = gGlobal.pUIManager->FindUICheckBox(Buf.c_str());
		if (pChkBox)
			pChkBox->SetState(false);

		Buf.Format("%s%d", SkillSubSelIconPrefix, i);

		UIItem * pItem = gGlobal.pUIManager->GetUIItem(Buf.c_str());
		if (pItem)
			pItem->SetEnable(false);

		Buf.Format("%s%d", SkillSubSelNamePrefix, i);

		pItem = gGlobal.pUIManager->GetUIItem(Buf.c_str());
		if (pItem)
			pItem->SetEnable(false);

		Buf.Format("%s%d", SkillSubSelUsePrefix, i);

		pItem = gGlobal.pUIManager->GetUIItem(Buf.c_str());
		if (pItem)
			pItem->SetEnable(false);
		
		Buf.Format("%s%d", SkillSubSelUseValuePrefix, i);

		pItem = gGlobal.pUIManager->GetUIItem(Buf.c_str());
		if (pItem)
			pItem->SetEnable(false);
	}
}
void CMenuSkillProcess::HideAllSkillButton()
{
	for (Int i = 1; i <= MAX_MAIN_SKILL; i++)
	{
		StringA Buf;

		Buf.Format("%s%d", SkillMainSelPrefix, i);

		//UICheckBox * pChkBox = gGlobal.pUIManager->FindUICheckBox(Buf.c_str());
		//if (pChkBox)
		//	pChkBox->SetState(false);

		Buf.Format("%s%d", SkillMainSelIconPrefix, i);

		UIItem * pItem = gGlobal.pUIManager->GetUIItem(Buf.c_str());
		if (pItem)
			pItem->SetEnable(false);

		Buf.Format("%s%d", SkillMainSelNamePrefix, i);

		pItem = gGlobal.pUIManager->GetUIItem(Buf.c_str());
		if (pItem)
			pItem->SetEnable(false);

		Buf.Format("%s%d", SkillMainSelLevelPrefix, i);

		pItem = gGlobal.pUIManager->GetUIItem(Buf.c_str());
		if (pItem)
			pItem->SetEnable(false);
	}

	for (Int i = 1; i <= MAX_SUB_SKILL; i++)
	{
		StringA Buf;

		Buf.Format("%s%d", SkillSubSelPrefix, i);

		//UICheckBox * pChkBox = gGlobal.pUIManager->FindUICheckBox(Buf.c_str());
		//if (pChkBox)
		//	pChkBox->SetState(false);

		Buf.Format("%s%d", SkillSubSelIconPrefix, i);

		UIItem * pItem = gGlobal.pUIManager->GetUIItem(Buf.c_str());
		if (pItem)
			pItem->SetEnable(false);

		Buf.Format("%s%d", SkillSubSelNamePrefix, i);

		pItem = gGlobal.pUIManager->GetUIItem(Buf.c_str());
		if (pItem)
			pItem->SetEnable(false);

		Buf.Format("%s%d", SkillSubSelUsePrefix, i);

		pItem = gGlobal.pUIManager->GetUIItem(Buf.c_str());
		if (pItem)
			pItem->SetEnable(false);
		
		Buf.Format("%s%d", SkillSubSelUseValuePrefix, i);

		pItem = gGlobal.pUIManager->GetUIItem(Buf.c_str());
		if (pItem)
			pItem->SetEnable(false);
	}
}

CSkill * CMenuSkillProcess::FindSkill(Int ID)
{
	for (Int i = 0; i < MAX_SKILL_TYPE; i++)
	{
		StlMap<WORD, SkillComData>::iterator Itr = m_SkillGroups[i].find(ID);
		if (Itr != m_SkillGroups[i].end())
		{
			return Itr->second.pSkill;
		}
	}
	return NULL;
}

void CMenuSkillProcess::ConstructData()
{
	if (!gConfigure.uNetwork)
	{
		ConstructDataAll();
		return;
	}
	//clear the data first
	for (Int i = 0; i < MAX_SKILL_TYPE; i++)
	{
		m_SkillGroups[i].clear();
		m_SkillBaseChildRel.clear();
	}

	//m_MainSkillSelIndex = 0;
	//m_SubSkillSelIndex = 0;
	
	//add all life base skill
	if (gGlobal.g_ChangeModelSkillLst)
	{
		for (Int i = 0; i < gGlobal.g_ChangeModelSkillLst->GetCount(); i++)
		{
			CSkill * pSkill = gGlobal.g_ChangeModelSkillLst->GetChild<CSkill>(i);
			if (pSkill)
			{
				const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(pSkill->GetSkill_id());
				if (pSkillData && pSkillData->type < MAX_SKILL_TYPE)
				{
					//m_SkillGroups[pSkillData->type].
					SkillComData Data;
					Data.pSkillData = pSkillData;
					Data.pSkill = pSkill;
					m_SkillGroups[pSkillData->type].insert(std::make_pair(pSkillData->id, Data));

					if (pSkillData->reqBaseSkill > 0)
					{
						const SkillData * pSkillDataBase = gGlobal.m_resource->GetSkillData(pSkillData->reqBaseSkill);
						if (pSkillDataBase && pSkillDataBase->type < MAX_SKILL_TYPE)
						{
							SkillComData SubData;
							SubData.pSkillData = pSkillDataBase;
							if (m_SkillGroups[pSkillDataBase->type].find(pSkillDataBase->id) == m_SkillGroups[pSkillDataBase->type].end())
								m_SkillGroups[pSkillDataBase->type].insert(std::make_pair(pSkillDataBase->id, SubData));
							m_SkillBaseChildRel.insert(std::make_pair(pSkillDataBase->id, Data));
						}
					}

					//add skill for battle use
					/*if (pSkillData->type == SKILL_TYPE_GANG_BASE)
					{

					}*/
				}
			}
		}

		//add skills to battle
		for (StlMultiMap<WORD, SkillComData>::iterator Itr = m_SkillBaseChildRel.begin();
			Itr != m_SkillBaseChildRel.end(); ++ Itr)
		{//SkillData::type
			if (Itr->second.pSkillData->battleorField == SKILL_USE_TIME_ANY ||
				Itr->second.pSkillData->battleorField == SKILL_USE_TIME_BATTLE)
			{
				BYTE base = FindSkillTypeByBase(Itr->second.pSkillData->type);
				if (base == SKILL_TYPE_SPECIAL_BASE)
				{
					if (m_SkillGroups[base].size() > 0 && SKILL_TYPE_SPECIAL_BASE == Itr->second.pSkillData->type)
					{
						WORD key = Itr->first;
						StlMap<WORD, SkillComData>::iterator Itr = m_SkillGroups[base].find(key);
						if (Itr != m_SkillGroups[base].end())
							m_SkillGroups[SKILL_TYPE_SPECIAL_BASE].insert(std::make_pair(Itr->first, Itr->second));
					}
				}
				else 
					if (base < MAX_SKILL_TYPE)
					{
						if (m_SkillGroups[base].size() > 0)
						{
							WORD key = Itr->first;
							StlMap<WORD, SkillComData>::iterator Itr = m_SkillGroups[base].find(key);
							if (Itr != m_SkillGroups[base].end())
								m_SkillGroups[SKILL_TYPE_BATTLE].insert(std::make_pair(Itr->first, Itr->second));
						}
					}
			}
		}
	}
	else if (gGlobal.g_SkillLst)
	{
		for (Int i = 0; i < gGlobal.g_SkillLst->GetCount(); i++)
		{
			CSkill * pSkill = gGlobal.g_SkillLst->GetChild<CSkill>(i);
			if (pSkill)
			{
				const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(pSkill->GetSkill_id());
				if (pSkillData && pSkillData->type < MAX_SKILL_TYPE)
				{
					//m_SkillGroups[pSkillData->type].
					SkillComData Data;
					Data.pSkillData = pSkillData;
					Data.pSkill = pSkill;
					m_SkillGroups[pSkillData->type].insert(std::make_pair(pSkillData->id, Data));

					if (pSkillData->reqBaseSkill > 0)
					{
						const SkillData * pSkillDataBase = gGlobal.m_resource->GetSkillData(pSkillData->reqBaseSkill);
						if (pSkillDataBase && pSkillDataBase->type < MAX_SKILL_TYPE)
						{
							SkillComData SubData;
							SubData.pSkillData = pSkillDataBase;
							if (m_SkillGroups[pSkillDataBase->type].find(pSkillDataBase->id) == m_SkillGroups[pSkillDataBase->type].end())
								m_SkillGroups[pSkillDataBase->type].insert(std::make_pair(pSkillDataBase->id, SubData));
							m_SkillBaseChildRel.insert(std::make_pair(pSkillDataBase->id, Data));
						}
					}

					//add skill for battle use
					/*if (pSkillData->type == SKILL_TYPE_GANG_BASE)
					{

					}*/
				}
			}
		}

		//add skills to battle
		for (StlMultiMap<WORD, SkillComData>::iterator Itr = m_SkillBaseChildRel.begin();
			Itr != m_SkillBaseChildRel.end(); ++ Itr)
		{//SkillData::type
			if (Itr->second.pSkillData->battleorField == SKILL_USE_TIME_ANY ||
				Itr->second.pSkillData->battleorField == SKILL_USE_TIME_BATTLE)
			{
				BYTE base = FindSkillTypeByBase(Itr->second.pSkillData->type);
				if (base == SKILL_TYPE_SPECIAL_BASE)
				{
					if (m_SkillGroups[base].size() > 0 && SKILL_TYPE_SPECIAL_BASE == Itr->second.pSkillData->type)
					{
						WORD key = Itr->first;
						StlMap<WORD, SkillComData>::iterator Itr = m_SkillGroups[base].find(key);
						if (Itr != m_SkillGroups[base].end())
							m_SkillGroups[SKILL_TYPE_SPECIAL_BASE].insert(std::make_pair(Itr->first, Itr->second));
					}
				}
				else 
				if (base < MAX_SKILL_TYPE)
				{
					if (m_SkillGroups[base].size() > 0)
					{
						WORD key = Itr->first;
						StlMap<WORD, SkillComData>::iterator Itr = m_SkillGroups[base].find(key);
						if (Itr != m_SkillGroups[base].end())
							m_SkillGroups[SKILL_TYPE_BATTLE].insert(std::make_pair(Itr->first, Itr->second));
					}
				}
			}
		}
	}
}


void CMenuSkillProcess::WriteToShortCut(Int ID, int targetPos)
{

	StringA num;
	num.Format("HShortCut%d", targetPos);					
	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(num.c_str());

	m_pTempSkillData = gGlobal.m_resource->GetSkillData(ID);

	if(m_pTempSkillData)
	{
		num.Format("HShortText%d", targetPos);					
		UIStaticText *pStaticText = gGlobal.pUIManager->FindUIStaticText(num.c_str());

		pAniIcon->ClearAniData();
		pAniIcon->SetAnimationData(m_pTempSkillData->icon);
		pAniIcon->SetEnable(true);

		//pAniIcon->SetTips(m_pTempSkillData->name);
		String	ShowName;
		ShowName.Format(_T("#W%s\\r#Y%s"), m_pTempSkillData->name, m_pTempSkillData->skill_functionDes); 
		pAniIcon->SetTips(ShowName.c_str());

		gGlobal.g_ShortCutSlot[targetPos].m_RefType = 2;
		gGlobal.g_ShortCutSlot[targetPos].m_RefNum = ID;

		CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
		if (pchar)
		{	
			RPCContext context;
			pchar->UpdateShortCut(gGlobal.pPeerProxy, context, ID, 2, targetPos);
		}

		pStaticText->ClearText();
		pStaticText->SetEnable(false);

		gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);

	}
}

Int32 CMenuSkillProcess::GetUseSkillID(Int Index)
{
	m_pTempSkillData = m_SubSkillCurLst[Index - 1];
	if (m_pTempSkillData == NULL)
		return -1;
	else
		return m_pTempSkillData->id;
}


bool CMenuSkillProcess::UseSkillProc()
{
	if (m_pTempSkillData == NULL || gGlobal.pUIManager->GetMonoItem() )
		return false;

	if (IsHawking())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_USE_SKILL")));
		return false;
	}

	UIPanel* tradingPanel = gGlobal.pUIManager->FindUIPanel("TradingPanel");
	UIPanel* givenPanel = gGlobal.pUIManager->FindUIPanel("GivenItemPanel");

	if (tradingPanel || givenPanel)
		return false;

	if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
	{
		if (m_pTempSkillData->battleorField == SKILL_USE_TIME_PEACE)
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_USE_FAIL_NOT_PEACE")), CHANNELTYPE_SYSTEM);
			return false;
		}

		if (gGlobal.g_pBattleControl->GetControlPlayer() && gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand() &&
			( gGlobal.g_pBattleControl->GetControlPartner() == NULL || 
			( gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand())))
			return false;
	}
	else
	{
		if (m_pTempSkillData->battleorField == SKILL_USE_TIME_BATTLE)
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_USE_FAIL_NOT_BATTLE")), CHANNELTYPE_SYSTEM);
			return false;
		}
	}
	
	gGlobal.g_bSkillFail = FALSE;
	StringA FuncName;
	FuncName.Format("%s%s", gGlobal.GetPrefixStr(m_pTempSkillData->id, "skill"), CommonSkillFuncName[SKILL_FUNC_NAME_CANUSE].c_str());
	gGlobal.g_TempSkillString = "";
	if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
		CallLuaFunc(gGlobal.g_pParentLua, FuncName.c_str(), String(), gGlobal.g_pBattleControl->GetControlPlayer(),
		gGlobal.g_pBattleControl->GetControlPlayer()->GetSkillLv(m_pTempSkillData->reqBaseSkill), m_pTempSkillData->reqBaseSkillRank,
		m_pTempSkillData->reqLv);
	else
		CallLuaFunc(gGlobal.g_pParentLua, FuncName.c_str(), String(), gGlobal.g_pCharacter,
		gGlobal.g_pCharacter->GetSkillLv(m_pTempSkillData->reqBaseSkill), m_pTempSkillData->reqBaseSkillRank,
		m_pTempSkillData->reqLv);

	if (gConfigure.uNetwork)
	{
		if (gGlobal.g_bSkillFail)
		{

			gGlobal.PrintMessage(gGlobal.g_TempSkillString, CHANNELTYPE_SYSTEM);
			return false;
		}
	}

	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUISkillState));
	gGlobal.g_CursorState->Block();

	if (m_pTempSkillData->Skill_ThrowItemType != 255)
	{
		gGlobal.g_pBattleControl->SetBattleItemUseIndex(-1);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenBattleItemMenu", String(), 0, 0);
	}

	if (m_pTempSkillData->targetType == TARGET_TYPE_SELF)
	{
		if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
		{
			if (gGlobal.g_pBattleControl->GetControlPlayer() &&
				!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand())
				gGlobal.g_pBattleControl->SkillUseOnSeletedChar(gGlobal.g_pBattleControl->GetControlPlayer());
			else if (gGlobal.g_pBattleControl->GetControlPartner() &&
				!gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand())
				gGlobal.g_pBattleControl->SkillUseOnSeletedChar(gGlobal.g_pBattleControl->GetControlPartner());
		}
		else
		{
			gGlobal.g_pCharacter->GetCharNetObject()->UseSkill(gGlobal.pPeerProxy, gGlobal.g_context,
				m_pTempSkillData->id, gGlobal.g_pCharacter->GetMapActorID(), 0);
		}

		gGlobal.g_CursorState->Unblock();
		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	
	}
	return true;
}


bool CMenuSkillProcess::UseSkill(Int Index)
{
	if (m_SkillShopID > 0)
		return false;

	if (m_totalSubSkill >= Index + m_SubSkillStartIndex)
	{
		m_pTempSkillData = m_SubSkillCurLst[Index - 1];
		return UseSkillProc();
	}
	return false;
}


bool CMenuSkillProcess::UseSkillID(Int ID)
{
	m_pTempSkillData = gGlobal.m_resource->GetSkillData(ID);
	return UseSkillProc();
}


void CMenuSkillProcess::UpdateSkillData(CSkill * pSkill)
{
	if (pSkill == NULL)
		return;

	
	//update level
	for (Int i = 0; i < MAX_MAIN_SKILL; i++)
	{
		if (m_MainSkillCurLst[i] &&
			m_MainSkillCurLst[i]->id == pSkill->GetSkill_id())
		{
			StringA buf;
			buf.Format("%s%d", SkillMainSelLevelPrefix, i + 1);
			UIStaticText * pTextLv = gGlobal.pUIManager->FindUIStaticText(buf.c_str());
			if (pTextLv)
			{
				String LvBuf;
				LvBuf.Format(_T("%s%d"),gGlobal.GetStringTable()->Get(_T("MSG_SKILL_LEVEL")),
					pSkill->GetSkill_level());
				pTextLv->SetText(LvBuf.c_str());
				pTextLv->FillImage();
				pTextLv->DrawFont();
				pTextLv->SetEnable(true);
				gGlobal.pUIManager->AddDirtyUIItem(pTextLv);
			}
			if (i == m_MainSkillSelIndex)
			{
				RefreshSubSkillMenu();
				UIPanel * pPanel= gGlobal.pUIManager->FindUIPanel(SkillPanelName);
				if (pPanel && pPanel->IsEnable())
					gGlobal.pUIManager->AddDirtyUIItem(pPanel);
			}

			if (GetIsShop())
				RefreshShopDetail();
			return;
		}
	}


}

void CMenuSkillProcess::ConstructDataAll()
{
	//clear the data first
	for (Int i = 0; i < MAX_SKILL_TYPE; i++)
	{
		m_SkillGroups[i].clear();
		m_SkillBaseChildRel.clear();
	}

	//for (Int i = 0; i < gGlobal.g_SkillLst->GetCount(); i++)

	Int index = 1;

	SkillDataList List;
	gGlobal.m_resource->GetSkillList(List);

	//for (Int index = 0; index <= gConfigure.uMaxSkillID; index ++)
	for (Int i = 0; i < (Int)List.size(); i++)
	//{
	//while (pSkillData)
	{
		const SkillData * pSkillData = List[i];
		if (pSkillData && pSkillData->type < MAX_SKILL_TYPE)
		{
			//m_SkillGroups[pSkillData->type].
			SkillComData Data;
			Data.pSkillData = pSkillData;
			Data.pSkill = NULL;
			m_SkillGroups[pSkillData->type].insert(std::make_pair(pSkillData->id, Data));

			if (pSkillData->reqBaseSkill > 0)
			{
				const SkillData * pSkillDataBase = gGlobal.m_resource->GetSkillData(pSkillData->reqBaseSkill);
				if (pSkillDataBase && pSkillDataBase->type < MAX_SKILL_TYPE)
				{
					SkillComData SubData;
					SubData.pSkillData = pSkillDataBase;
					if (m_SkillGroups[pSkillDataBase->type].find(pSkillDataBase->id) == m_SkillGroups[pSkillDataBase->type].end())
						m_SkillGroups[pSkillDataBase->type].insert(std::make_pair(pSkillDataBase->id, SubData));
					m_SkillBaseChildRel.insert(std::make_pair(pSkillDataBase->id, Data));
				}
			}

			//add skill for battle use
			/*if (pSkillData->type == SKILL_TYPE_GANG_BASE)
			{

			}*/
		}
		index++;
		pSkillData = gGlobal.m_resource->GetSkillData(index);
	}

	//add skills to battle
	for (StlMultiMap<WORD, SkillComData>::iterator Itr = m_SkillBaseChildRel.begin();
		Itr != m_SkillBaseChildRel.end(); ++ Itr)
	{//SkillData::type
		if (Itr->second.pSkillData->battleorField == SKILL_USE_TIME_ANY ||
			Itr->second.pSkillData->battleorField == SKILL_USE_TIME_BATTLE)
		{
			BYTE base = FindSkillTypeByBase(Itr->second.pSkillData->type);
			if (base == SKILL_TYPE_SPECIAL_BASE)
			{
				if (m_SkillGroups[base].size() > 0 && SKILL_TYPE_SPECIAL_BASE == Itr->second.pSkillData->type)
				{
					WORD key = Itr->first;
					StlMap<WORD, SkillComData>::iterator Itr = m_SkillGroups[base].find(key);
					if (Itr != m_SkillGroups[base].end())
						m_SkillGroups[SKILL_TYPE_SPECIAL_BASE].insert(std::make_pair(Itr->first, Itr->second));
				}
			}
			else if (base < MAX_SKILL_TYPE)
			{
				if (m_SkillGroups[base].size() > 0)
				{
					WORD key = Itr->first;
					StlMap<WORD, SkillComData>::iterator Itr = m_SkillGroups[base].find(key);
					if (Itr != m_SkillGroups[base].end())
						m_SkillGroups[SKILL_TYPE_BATTLE].insert(std::make_pair(Itr->first, Itr->second));
				}
			}
		}
	}
}

const SkillData * CMenuSkillProcess::GetSubSkillData(Int Index)
{
	if (Index >= MAX_SUB_SKILL)
		return NULL;

	return m_SubSkillCurLst[Index];
}
const SkillData * CMenuSkillProcess::GetMainSkillData(Int Index)
{
	if (Index >= MAX_MAIN_SKILL)
		return NULL;

	return m_MainSkillCurLst[Index];
}

void CMenuSkillProcess::OpenSkillDetail()
{
	Int MouseX = gGlobal.pUIManager->GetCursorX();
	Int MouseY = gGlobal.pUIManager->GetCursorY();

	Int Index = -1;
	for (Int i = 0; i < MAX_MAIN_SKILL; i++)
	{
		StringA buf;
		buf.Format("%s%d", SkillMainSelIconPrefix, i + 1);
		UIItem * pItem = gGlobal.pUIManager->GetUIItem(buf.c_str());
		if (pItem && pItem->Detect(MouseX, MouseY))
		{
			Index = i;
			break;
		}
	}
	
	if (Index == -1)
	{
		for (Int i = 0; i < MAX_SUB_SKILL; i++)
		{
			StringA buf;
			buf.Format("%s%d", SkillSubSelIconPrefix, i + 1);
			UIItem * pItem = gGlobal.pUIManager->GetUIItem(buf.c_str());
			if (pItem && pItem->Detect(MouseX, MouseY))
			{
				Index = i + MAX_MAIN_SKILL;
				break;
			}
		}
	}

	SetSkillDetailIndex(Index);

	if (m_pTempSkillDetailData == NULL)
		return;

	UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx(SkillDetailPanelName);
	if (pPanelEx == NULL)
	{
		XML_UI::GetInstance().Init();
		XML_UI::GetInstance().ReadFormFile(SkillDetailXMLName, gGlobal.pUIManager);

		pPanelEx = gGlobal.pUIManager->FindUIPanelEx(SkillDetailPanelName);
		if (pPanelEx == NULL)
			return;		
	}

	gGlobal.pUIManager->SetPanelEx2Top(pPanelEx);
	

	/*UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(SkillDetailName.c_str());
	if (pText)
	{
		pText->SetText(m_pTempSkillDetailData->name);
		pText->Redraw();
	}*/
	UIChatPanel * pChatPanel = gGlobal.pUIManager->FindUIChatPanel(SkillChat);

	if (pChatPanel)
	{
		pChatPanel->ClearAllRow();
		//add skill name
		//pChatPanel->AddStringW(m_pTempSkillDetailData->name);
		String name;
		name.Format(_T("#W%s"), m_pTempSkillDetailData->name);
		pChatPanel->AddStringW(name.c_str());
		if (wcscmp(m_pTempSkillDetailData->skill_description, _T("0")) != 0)
			pChatPanel->AddStringW(m_pTempSkillDetailData->skill_description);

		CSkill * pSkill = FindSkill(m_pTempSkillDetailData->reqBaseSkill);

		Int Level = 0;
		if (pSkill)
			Level = pSkill->GetSkill_level();
		
		gGlobal.g_TempSkillString = "";
		
		StringA FuncName;
		FuncName.Format("%s%s", gGlobal.GetPrefixStr(m_pTempSkillDetailData->id, "skill"), CommonSkillFuncName[SKILL_FUNC_NAME_DESC].c_str());
		CallLuaFunc(gGlobal.g_pParentLua, FuncName.c_str(), String(), Level);
		

		if (!gGlobal.g_TempSkillString.IsEmpty())
		{
			String Com;
			Com.Format(_T("#%s%s"),TextColorStr[ColorIndex_Yellow], gGlobal.g_TempSkillString.c_str());
			pChatPanel->AddStringW(Com.c_str());
		}
		else
		{
			if (wcscmp(m_pTempSkillDetailData->skill_functionDes, _T("0")) != 0)
			{
				String Com;
				Com.Format(_T("#%s%s"), TextColorStr[ColorIndex_Yellow], m_pTempSkillDetailData->skill_functionDes);
				pChatPanel->AddStringW(Com.c_str());
			}
		}
		String TextLvReq;

		if (m_pTempSkillDetailData->reqLv == ErrorReqLv)
		{
			TextLvReq =gGlobal.GetStringTable()->Get(_T("MSG_SKILL_USE_DISABLE"));
			pChatPanel->SetFontColor(RedColor);
		}
		else
		{
			
			TextLvReq.Format(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_REQ_CHAR_LV")), m_pTempSkillDetailData->reqLv);

			if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
			{
				if (gGlobal.g_pBattleControl->GetControlPlayer()->GetLevel() < m_pTempSkillDetailData->reqLv)
				{
					pChatPanel->SetFontColor(RedColor);
					pChatPanel->AddStringW(TextLvReq.c_str());
				}
			}
			else
			{
				if (gGlobal.g_pCharacter->GetLevel() < m_pTempSkillDetailData->reqLv)
				{
					pChatPanel->SetFontColor(RedColor);
					pChatPanel->AddStringW(TextLvReq.c_str());
				}
			}
		}

		

		pChatPanel->SetFontColor(TextColor[ColorIndex_White]);
		if (m_pTempSkillDetailData->reqBaseSkillRank != ErrorReqLv &&
			m_pTempSkillDetailData->reqBaseSkillRank != 0)
		{
			const SkillData * pBaseRank = gGlobal.m_resource->GetSkillData(m_pTempSkillDetailData->reqBaseSkill);
			if (pBaseRank)
			{
				String TextSkillLvReq;
				TextSkillLvReq.Format(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_REQ_BASE_RANK")), pBaseRank->name, 
					m_pTempSkillDetailData->reqBaseSkillRank);

				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				{
					if (gGlobal.g_pBattleControl->GetControlPlayer()->GetSkillLv(m_pTempSkillDetailData->reqBaseSkill)
						< m_pTempSkillDetailData->reqBaseSkillRank )
					{
						pChatPanel->SetFontColor(RedColor);
						pChatPanel->AddStringW(TextSkillLvReq.c_str());
					}
				}
				else
				{
					if (gGlobal.g_pCharacter->GetSkillLv(m_pTempSkillDetailData->reqBaseSkill)
						< m_pTempSkillDetailData->reqBaseSkillRank )
					{
						pChatPanel->SetFontColor(RedColor);
						pChatPanel->AddStringW(TextSkillLvReq.c_str());
					}
				}
				pChatPanel->SetFontColor(TextColor[ColorIndex_White]);
			}
		}
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

void CMenuSkillProcess::SetSkillDetailIndex(int Index)
{
	m_pTempSkillDetailData = NULL;
	
	if (Index < 0)
		return;

	if (Index >= MAX_MAIN_SKILL)
	{
		Index -= MAX_MAIN_SKILL;
		m_pTempSkillDetailData = GetSubSkillData(Index);
	}
	else
		m_pTempSkillDetailData = GetMainSkillData(Index);
}

void CMenuSkillProcess::ConstructByShopID(UInt32 shopID)
{
	const SkillShopData * pShopData = gGlobal.m_resource->GetSkillShop(shopID);
	if (pShopData)
	{
		//clear the data first
		for (Int i = 0; i < MAX_SKILL_TYPE; i++)
		{
			m_SkillGroups[i].clear();
			m_SkillBaseChildRel.clear();
		}

		//for (Int i = 0; i < gGlobal.g_SkillLst->GetCount(); i++)

		Int index = 1;
		//for (Int index = 0; index <= gConfigure.uMaxSkillID; index ++)
		for (Int i = 0; i < (Int)pShopData->SkillShopIDLst.size(); i++)
			//{
			//while (pSkillData)
		{
			const SkillData * pSkillDataBase = gGlobal.m_resource->GetSkillData(pShopData->SkillShopIDLst[i]);
			if (pSkillDataBase && pSkillDataBase->type < MAX_SKILL_TYPE)
			{
				//m_SkillGroups[pSkillData->type].
				SkillComData Data;
				Data.pSkillData = pSkillDataBase;
				Data.pSkill = GetSkill(pSkillDataBase->id);
				m_SkillGroups[pSkillDataBase->type].insert(std::make_pair(pSkillDataBase->id, Data));

				//get the children
				SkillDataList ChildLst;
				gGlobal.m_resource->GetSkillLink(pShopData->SkillShopIDLst[i], ChildLst);
				for (Int j = 0; j < (Int)ChildLst.size(); j++)
				{
					const SkillData * pSkillData = ChildLst[j];
					if (pSkillData && pSkillData->type < MAX_SKILL_TYPE)
					{
						//m_SkillGroups[pSkillData->type].
						SkillComData Data;
						Data.pSkillData = pSkillData;
						Data.pSkill = GetSkill(pSkillData->id);
						m_SkillGroups[pSkillData->type].insert(std::make_pair(pSkillData->id, Data));
						m_SkillBaseChildRel.insert(std::make_pair(pSkillDataBase->id, Data));
					}
				}
				/*if (pSkillData->reqBaseSkill > 0)
				{
					const SkillData * pSkillDataBase = gGlobal.m_resource->GetSkillData(pSkillData->reqBaseSkill);
					if (pSkillDataBase && pSkillDataBase->type < MAX_SKILL_TYPE)
					{
						SkillComData SubData;
						SubData.pSkillData = pSkillDataBase;
						if (m_SkillGroups[pSkillDataBase->type].find(pSkillDataBase->id) == m_SkillGroups[pSkillDataBase->type].end())
							m_SkillGroups[pSkillDataBase->type].insert(std::make_pair(pSkillDataBase->id, SubData));
						m_SkillBaseChildRel.insert(std::make_pair(pSkillDataBase->id, Data));
					}
				}*/	
			}		
		}
		
		m_CurrSkillType = (BYTE)pShopData->SkillBaseType;
		m_SkillShopID = shopID;
		RefreshShopDetail();
	}

}

CSkill * CMenuSkillProcess::GetSkill(UInt16 skill_id)
{
	if (gGlobal.g_SkillLst == NULL)
		return NULL;

	for (Int i = 0; i < (Int)gGlobal.g_SkillLst->GetCount(); i++)
	{
		CSkill * pSkill = gGlobal.g_SkillLst->GetChild<CSkill>(i);
		if (pSkill && pSkill->GetSkill_id() == skill_id)
			return pSkill;
	}

	return NULL;
}

void CMenuSkillProcess::SetSkillShop(int Value)
{
	if (Value == 0)
	{
		ConstructData();
	}
	if (m_SkillShopID > 0 && Value == 0)
			gGlobal.g_pCharacter->GetCharNetObject()->LeaveSkillShop(gGlobal.pPeerProxy, RPCContext());
	m_SkillShopID = Value;
}

void CMenuSkillProcess::RefreshShopDetailItem()
{
	const SkillShopData * pShopData = gGlobal.m_resource->GetSkillShop(m_SkillShopID);
	if (pShopData)
	{
		UInt count = 1;
		UInt y = 14;
		const UInt sizeOfName = 18;
		PSTRINGA name[sizeOfName] = {
				"SkillShopLevel",
				"SkillShopLevelValue",
				"SkillShopExp",
				"SkillShopExpValue",
				"SkillShopMoney",
				"SkillShopMoneyValue",
				"SkillShopYuanBao",
				"SkillShopYuanBaoValue",
				"SkillShopHonor",
				"SkillShopHonorValue",
				"SkillShopFDonate",
				"SkillShopFDonateValue",
				"SkillShopGDonate",
				"SkillShopGDonateValue",
				"SkillShopSMoney",
				"SkillShopSMoneyValue",
				"SkillShopSave",
				"SkillShopSaveValue",
		};
		const bool show[sizeOfName] = 
		{
				true,
				true,
				pShopData->UseEXP > 0,
				pShopData->UseEXP > 0,
				pShopData->UseMoney > 0,
				pShopData->UseMoney > 0,
				pShopData->UseYuanBao > 0,
				pShopData->UseYuanBao > 0,
				pShopData->UseHonor > 0,
				pShopData->UseHonor > 0,				
				pShopData->UseDonateFaction > 0,
				pShopData->UseDonateFaction > 0,
				pShopData->UseDonateManor > 0,
				pShopData->UseDonateManor > 0,
				pShopData->UseSmoney > 0,
				pShopData->UseSmoney > 0,
				pShopData->UseMoney > 0,
				pShopData->UseMoney > 0,

		};

		for (UInt i = 0; i < sizeOfName; i ++)
		{
			UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(name[i]);
			if (pText)
			{
				if (show[i])
				{
					pText->SetEnable(true);
					pText->SetOffsetXY(pText->GetOffsetX(), y);
					if (i % 2 == 1)
					{
						y += 18;
						count ++;
					}
				}
				else
					pText->SetEnable(false);
			}
			
		}

		UIButton * pBu = gGlobal.pUIManager->FindUIButton("LearnSkill");
		if (pBu)
		{
			for (Int i = _ButtonNormal; i < _MaxButton_State; i++)
				pBu->SetSubOffSet(i, pBu->GetOffsetX(), y);
			pBu->SetOffsetXY(pBu->GetOffsetX(), (y));
			pBu->SetEnable(true);
		}

		UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("SkillShopPanel");
		if (pPanelEx)
		{
			pPanelEx->ChangeCount(count);
			pPanelEx->SetEnable(true);
		}

	}
}
void CMenuSkillProcess::RefreshShopDetail()
{
	const SkillShopData * pSkillShopData = gGlobal.m_resource->GetSkillShop(m_SkillShopID);
	if (pSkillShopData == NULL)
		return;

	UInt UIStartIndex = m_MainSkillSelIndex + 1;

	UInt CurrIndex = 0;

	for (StlMap<UInt16, SkillComData>::iterator Itr = m_SkillGroups[m_CurrSkillType].begin(); 
		Itr != m_SkillGroups[m_CurrSkillType].end() ; ++Itr, ++CurrIndex)
	{
		if (UIStartIndex > MAX_MAIN_SKILL)
		{
			break;
		}

		if (CurrIndex + 1 == m_MainSkillStartIndex + UIStartIndex)
		{
			Int skillLv = 0;
			if (Itr->second.pSkill)
				skillLv = Itr->second.pSkill->GetSkill_level();

			UInt32 useExp =  0;
			UInt32 useMoney =  0;
			UInt honor = 0;
			UInt32 useFDonate = 0;
			UInt32 useGDonate = 0;
			UInt32 useYuanBao = 0;

			if (pSkillShopData->UseEXP)
			{
				CallLuaFunc(gGlobal.g_pParentLua, "CalSkillNextLvExp", String(), skillLv + 1, m_SkillShopID);
				useExp = (UInt32)gGlobal.g_TempResult;			
			}

			if (pSkillShopData->UseMoney)
			{
				CallLuaFunc(gGlobal.g_pParentLua, "CalSkillNextLvMoney", String(), skillLv + 1, m_SkillShopID);
				useMoney = (UInt32)gGlobal.g_TempResult;			
			}

			if (pSkillShopData->UseHonor)
			{
				CallLuaFunc(gGlobal.g_pParentLua, "CalSkillNextLvHonor", String(), skillLv + 1, m_SkillShopID);
				honor = (UInt32)gGlobal.g_TempResult;
			}

			if (pSkillShopData->UseDonateFaction)
			{
				CallLuaFunc(gGlobal.g_pParentLua, "CalSkillNextLvDonateFaction", String(), skillLv + 1, m_SkillShopID);
				useFDonate = (UInt32)gGlobal.g_TempResult;			
			}

			if (pSkillShopData->UseDonateManor)
			{
				CallLuaFunc(gGlobal.g_pParentLua, "CalSkillNextLvDonateManor", String(), skillLv + 1, m_SkillShopID);
				useGDonate = (UInt32)gGlobal.g_TempResult;			
			}

			if (pSkillShopData->UseYuanBao)
			{
				CallLuaFunc(gGlobal.g_pParentLua, "CalSkillNextLvYuanBao", String(), skillLv + 1, m_SkillShopID);
				useYuanBao = (UInt32)gGlobal.g_TempResult;
			}

			UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(SkillShopLevelValue);

			if (pText)
			{
				String temp;
				temp.Format(_T("%d/%d"), skillLv, Itr->second.pSkillData->maxRank);

				if (skillLv >= Itr->second.pSkillData->maxRank)
					pText->SetColor(RedColor);
				else
					pText->SetColor(TextColor[ColorIndex_White]);

				pText->SetText(temp);
				pText->Redraw();
				
			}

			pText = gGlobal.pUIManager->FindUIStaticText(SkillShopExpValue);

			if (pText)
			{
				String temp;
				temp.Format(_T("%d/%d"), gGlobal.g_pCharacter->GetExp(), useExp);

				if (useExp > gGlobal.g_pCharacter->GetExp())
					pText->SetColor(RedColor);
				else
					pText->SetColor(TextColor[ColorIndex_White]);

				pText->SetText(temp);
				pText->Redraw();
				
			}

			pText = gGlobal.pUIManager->FindUIStaticText(SkillShopMoneyValue);

			if (pText)
			{
				String temp;
				temp.Format(_T("%d/%d"), gGlobal.g_pCharacter->GetCharNetObject()->GetMoney(), useMoney);

				if (useMoney > gGlobal.g_pCharacter->GetCharNetObject()->GetMoney() + gGlobal.g_pCharacter->GetBankMoney())
					pText->SetColor(RedColor);
				else
					pText->SetColor(TextColor[ColorIndex_White]);

				pText->SetText(temp);
				pText->Redraw();
				
			}

			pText = gGlobal.pUIManager->FindUIStaticText(SkillShopHonorValue);

			if (pText)
			{
				
				String temp;
				temp.Format(_T("%d/%d"), gGlobal.g_pCharacter->GetCharNetObject()->GetHonor(), honor);

				if (honor > gGlobal.g_pCharacter->GetCharNetObject()->GetHonor())
					pText->SetColor(RedColor);
				else
					pText->SetColor(TextColor[ColorIndex_White]);

				pText->SetText(temp);
				pText->Redraw();
					
				
			}

			pText = gGlobal.pUIManager->FindUIStaticText(SkillShopSaveValue);

			if (pText)
			{
				String temp;
				temp.Format(_T("%d"), gGlobal.g_pCharacter->GetBankMoney());

				pText->SetText(temp);
				pText->Redraw();
				
			}

			pText = gGlobal.pUIManager->FindUIStaticText("SkillShopYuanBaoValue");

			if (pText)
			{
				String temp;
				temp.Format(_T("%d/%d"), useYuanBao, gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBao());

				if (useYuanBao > gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBao())
					pText->SetColor(RedColor);
				else
					pText->SetColor(TextColor[ColorIndex_White]);

				pText->SetText(temp);
				pText->Redraw();			
			}

			pText = gGlobal.pUIManager->FindUIStaticText("SkillShopFDonateValue");

			if (pText)
			{
				String temp;
				temp.Format(_T("%d/%d"), useFDonate, gGlobal.g_pCharacter->GetCharNetObject()->GetFDonate());

				if (useFDonate > gGlobal.g_pCharacter->GetCharNetObject()->GetFDonate())
					pText->SetColor(RedColor);
				else
					pText->SetColor(TextColor[ColorIndex_White]);

				pText->SetText(temp);
				pText->Redraw();			
			}

			pText = gGlobal.pUIManager->FindUIStaticText("SkillShopGDonateValue");

			if (pText && gGlobal.g_pMenuGuildProcess->m_ownGuildMember)
			{
				String temp;
				temp.Format(_T("%d/%d"), useGDonate, gGlobal.g_pMenuGuildProcess->m_ownGuildMember->GetDonate());

				if (useGDonate > gGlobal.g_pMenuGuildProcess->m_ownGuildMember->GetDonate())
					pText->SetColor(RedColor);
				else
					pText->SetColor(TextColor[ColorIndex_White]);

				pText->SetText(temp);
				pText->Redraw();			
			}

			pText = gGlobal.pUIManager->FindUIStaticText("SkillShopSMoneyValue");

			if (pText)
			{
				String temp;
				temp.Format(_T("%d"), gGlobal.g_pCharacter->GetCharNetObject()->GetSMoney());

				pText->SetText(temp);
				pText->Redraw();			
			}

			
			
			break;
		}
	}
}

void CMenuSkillProcess::ShopLearn()
{
	const SkillShopData * pSkillShopData = gGlobal.m_resource->GetSkillShop(m_SkillShopID);

	if (pSkillShopData == NULL)
		return;

	UInt UIStartIndex = m_MainSkillSelIndex + 1;

	UInt CurrIndex = 0;

	for (Int i = 0; i < MAX_SUB_SKILL; i++)
		m_SubSkillCurLst[i] = NULL;

	for (StlMap<UInt16, SkillComData>::iterator Itr = m_SkillGroups[m_CurrSkillType].begin(); 
		Itr != m_SkillGroups[m_CurrSkillType].end() ; ++Itr, ++CurrIndex)
	{
		if (UIStartIndex > MAX_MAIN_SKILL)
		{
			break;
		}

		if (CurrIndex + 1 == m_MainSkillStartIndex + UIStartIndex)
		{
			Int skillLv = 0;
			if (Itr->second.pSkill)
				skillLv = Itr->second.pSkill->GetSkill_level();

			UInt32 useExp =  0;
			UInt32 useMoney =  0;
			UInt honor = 0;
			UInt32 useFDonate = 0;
			UInt32 useGDonate = 0;
			UInt32 useYuanBao = 0;

			if (pSkillShopData->UseEXP)
			{
				CallLuaFunc(gGlobal.g_pParentLua, "CalSkillNextLvExp", String(), skillLv + 1, m_SkillShopID);
				useExp = (UInt32)gGlobal.g_TempResult;			
			}

			if (pSkillShopData->UseMoney)
			{
				CallLuaFunc(gGlobal.g_pParentLua, "CalSkillNextLvMoney", String(), skillLv + 1, m_SkillShopID);
				useMoney = (UInt32)gGlobal.g_TempResult;			
			}

			if (pSkillShopData->UseHonor)
			{
				CallLuaFunc(gGlobal.g_pParentLua, "CalSkillNextLvHonor", String(), skillLv + 1, m_SkillShopID);
				honor = (UInt32)gGlobal.g_TempResult;
			}

			if (pSkillShopData->UseDonateFaction)
			{
				CallLuaFunc(gGlobal.g_pParentLua, "CalSkillNextLvDonateFaction", String(), skillLv + 1, m_SkillShopID);
				useFDonate = (UInt32)gGlobal.g_TempResult;			
			}

			if (pSkillShopData->UseDonateManor)
			{
				CallLuaFunc(gGlobal.g_pParentLua, "CalSkillNextLvDonateManor", String(), skillLv + 1, m_SkillShopID);
				useGDonate = (UInt32)gGlobal.g_TempResult;			
			}

			if (pSkillShopData->UseYuanBao)
			{
				CallLuaFunc(gGlobal.g_pParentLua, "CalSkillNextLvYuanBao", String(), skillLv + 1, m_SkillShopID);
				useYuanBao = (UInt32)gGlobal.g_TempResult;
			}


			if (gGlobal.g_pCharacter->GetLevel() < Itr->second.pSkillData->reqLv)
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_LV")), CHANNELTYPE_SYSTEM);
				return;
			}
			else if (skillLv >= Itr->second.pSkillData->maxRank)
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_MAX_RANK")), CHANNELTYPE_SYSTEM);
				return;
			}
			else if (gGlobal.g_pCharacter->GetExp() < useExp)
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_EXP")), CHANNELTYPE_SYSTEM);
				return;
			}
			else if ((!pSkillShopData->UseSmoney && gGlobal.g_pCharacter->GetMoney() + gGlobal.g_pCharacter->GetBankMoney() < useMoney) ||
				(pSkillShopData->UseSmoney && gGlobal.g_pCharacter->GetCharNetObject()->GetSMoney() + gGlobal.g_pCharacter->GetMoney() + gGlobal.g_pCharacter->GetBankMoney() < useMoney))
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_MONEY")), CHANNELTYPE_SYSTEM);
				return;
			}
			else if (skillLv - gGlobal.g_pCharacter->GetLevel() >=  10)
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_LV2")), CHANNELTYPE_SYSTEM);
				return;
			}
			else if (gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBao() < useYuanBao)
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_YUANBAO")), CHANNELTYPE_SYSTEM);
				return;
			}
			else if (gGlobal.g_pCharacter->GetCharNetObject()->GetFDonate() < useFDonate)
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_DONATEFACTION")), CHANNELTYPE_SYSTEM);
				return;
			}
			else if ((useGDonate > 0) && (gGlobal.g_pMenuGuildProcess->m_ownGuildMember == NULL ||
				(gGlobal.g_pMenuGuildProcess->m_ownGuildMember && gGlobal.g_pMenuGuildProcess->m_ownGuildMember->GetDonate() < useGDonate)))
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_DONATEGUILD")), CHANNELTYPE_SYSTEM);
				return;
			}
			else if (gGlobal.g_pCharacter->GetCharNetObject()->GetHonor() < honor)
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_HONOR")), CHANNELTYPE_SYSTEM);
				return;
			}

			gGlobal.g_pCharacter->GetCharNetObject()->SkillShopLearn(gGlobal.pPeerProxy, gGlobal.g_context, Itr->second.pSkillData->id);
			
			break;
		}
	}
}

void CMenuSkillProcess::ResetPosition()
{
	m_MainSkillStartIndex = 0;
	m_SubSkillStartIndex = 0;

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem(SkillSubScroll);
	if (scrollBar)
	{
		scrollBar->SetSelNum(0);
	}

	scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem(SkillMainScroll);
	if (scrollBar)
	{
		scrollBar->SetSelNum(0);
	}
}
