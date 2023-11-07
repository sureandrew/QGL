//-- Common
#include "Common.h"
#include "QGL.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "MenuPetProcess.h"
//-- Library
#include "Global.h"
#include "QGLApplication.h"
#include "CharacterControl.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResPet.h"
#include "Resource/ResSkill.h"
#include "Proxy/Logic/Pet.h"
#include "Proxy/Logic/PetGroup.h"
#include "Common/common_binary.h"
#include "Proxy/Logic/Skill.h"
#include "Proxy/Logic/SkillGroup.h"
#include "Resource/ResSkillCombination.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/Item.h"
#include "MenuItemProcess.h"

#define MAX_PET_IN_ONE_PAGE	4
#define PET_USE_FOOD_MAX_VP 300
#define PET_NOT_USE_ITEM	1000

const PSTRINGA SkillDetailPanelName = "SkillDetailMenuPanel";
const PSTRINGA SkillChat = "SkillDetailChat";
const PSTRINGA SkillDetailXMLName = "Data\\SkillDetail.xml";

CMenuPetProcess::CMenuPetProcess()
{
	m_StartIndex = 0;
	m_SelIndex = -1;
	m_currPetUid = 0;
	m_useItemSlot = PET_NOT_USE_ITEM;
	m_useFoodMsg = false;
}

CMenuPetProcess::~CMenuPetProcess()
{

}

void CMenuPetProcess::ShowRemovePetName()
{
	UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("ConfirmDropPetText");
	if (pText)
	{
		if (gGlobal.g_PetLst && gGlobal.g_PetLst->GetCount() > m_SelIndex && m_SelIndex != -1)
		{
			CPet * pPet =  gGlobal.g_PetLst->GetChild<CPet>(m_SelIndex);
			if (pPet)
			{
				String nameString;
				nameString.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARNTER_REMOVE")), pPet->GetName().c_str());
				pText->SetText(nameString.c_str());
				pText->Redraw();
			}
		}
	}
}
void CMenuPetProcess::ChangePetName()
{
	if (IsHawking())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_CHANGE_PP_NAME")));
		return;
	}

	UIEditBox * pBox = gGlobal.pUIManager->FindUIEditBox("PetNewName");
	if (pBox)
	{
		if (gGlobal.g_PetLst && gGlobal.g_PetLst->GetCount() > m_SelIndex && m_SelIndex != -1)
		{
			CPet * pPet =  gGlobal.g_PetLst->GetChild<CPet>(m_SelIndex);
			if (pPet)
			{
				pPet->ChangePetName(gGlobal.pPeerProxy, RPCContext(),pBox->GetText());
			}
		}
	}

}

void CMenuPetProcess::HideRightUpItem()
{
	gGlobal.pUIManager->SetUIItemEnable("SelectPet", false);
	for (Int i = 1; i <= MAX_PET_IN_ONE_PAGE; i++)
	{

		StringA name;
		name.Format("Pet%dText",i);
		gGlobal.pUIManager->SetUIItemEnable(name.c_str(), false);

		name.Format("SelectPet%d", i);
		gGlobal.pUIManager->SetUIItemEnable(name.c_str(), false);

		
		for (Index j = C_INDEX(1); j <= 3; ++j)
		{
			StringA name;
			name.Format("PetState%d%d",i, j);
			gGlobal.pUIManager->SetUIItemEnable(name.c_str(), false);
		}
        StringA lockPetImage;
		lockPetImage.Format("LockPet%01d", i);
		gGlobal.pUIManager->SetUIItemEnable(lockPetImage.c_str(), false);	
	}

}
void CMenuPetProcess::HideSelectInfoItem()
{
	gGlobal.pUIManager->SetUIItemEnable("PetModel", false);
		
	gGlobal.pUIManager->SetUIItemEnable("PetFather", false);
	gGlobal.pUIManager->SetUIItemEnable("PetMother", false);	
	gGlobal.pUIManager->SetUIItemEnable("PetSpirit", false);
	gGlobal.pUIManager->SetUIItemEnable("PetLevel", false);	
	gGlobal.pUIManager->SetUIItemEnable("PetFavor", false);	
	gGlobal.pUIManager->SetUIItemEnable("PetLife", false);
	gGlobal.pUIManager->SetUIItemEnable("PetVPBar", false);
	gGlobal.pUIManager->SetUIItemEnable("PetVPText", false);
	gGlobal.pUIManager->SetUIItemEnable("PetExpText", false);
	gGlobal.pUIManager->SetUIItemEnable("PetExp", false);
	gGlobal.pUIManager->SetUIItemEnable("PetReqLevel", false);
	gGlobal.pUIManager->SetUIItemEnable("PetSex1", false);
	gGlobal.pUIManager->SetUIItemEnable("PetSex2", false);
	gGlobal.pUIManager->SetUIItemEnable("PetFoodBar", false);
	gGlobal.pUIManager->SetUIItemEnable("PetFood", false);
	gGlobal.pUIManager->SetUIItemEnable("PetType", false);

	for (UInt i = 0; i < 8; ++i)
	{
		StringA name;
		name.Format("PetSkill%d",i);
		gGlobal.pUIManager->SetUIItemEnable(name.c_str(), false);
	}

	
}
void CMenuPetProcess::ResetMenu()
{
	m_StartIndex = 0;
	m_SelIndex = -1;
}

void CMenuPetProcess::RefreshRightUpPanel()
{
	HideRightUpItem();
	if (gGlobal.g_PetLst)
	{
		UInt menuIndex = 1;
		for (UInt i = m_StartIndex; i < gGlobal.g_PetLst->GetCount(); i++, menuIndex++)
		{
			if (i - m_StartIndex >= MAX_PET_IN_ONE_PAGE)
				break;
			CPet * pPet =  gGlobal.g_PetLst->GetChild<CPet>(i);
			if (pPet)
			{
				const PetData * pPetData = gGlobal.m_resource->GetPetData(pPet->GetPet_id());
				if (pPetData)
				{
					StringA name;
					name.Format("Pet%dText", menuIndex);
					UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
					if (pText)
					{
						pText->SetText(pPet->GetName().c_str());
						pText->Redraw();
						pText->SetEnable(true);
					}

					name.Format("PetState%d%d", menuIndex, pPet->GetState() + 1);
					UIImage * pIm = gGlobal.pUIManager->FindUIImage(name.c_str());
					if (pIm)
						pIm->SetEnable(true);					

					if (m_SelIndex == i)
					{
						name.Format("SelectPet%d", menuIndex);
						UIImage * pIm = gGlobal.pUIManager->FindUIImage(name.c_str());
						if (pIm)
							pIm->SetEnable(true);						
					}

                    StringA lockPetImage;
					lockPetImage.Format("LockPet%01d", i + 1);
					UIImage * pLockIm = gGlobal.pUIManager->FindUIImage(lockPetImage.c_str());
					if (pLockIm)
						pLockIm->SetEnable(pPet->GetLock());		


				}
			}
		}

		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("Pet5Text");
		if (pText)
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CARRYING_NUMBER")), gGlobal.g_PetLst->GetCount(),
				gGlobal.g_pCharacter->GetCharNetObject()->GetMaxPet());
			pText->SetText(text.c_str());
			pText->Redraw();
			pText->SetEnable(true);
		}
	}
}
void CMenuPetProcess::RefreshMenu()
{
	if (gGlobal.g_PetLst)
	{
		RefreshRightUpPanel();
		ShowSelectedInfo();

		UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("PetScrollBar");
		if (scrollBar)
			scrollBar->SetNumCount(gGlobal.g_PetLst->GetCount());		
	}
}

void CMenuPetProcess::ShowSelectedInfo()
{
	if (gGlobal.g_PetLst == NULL)
		return;

	HideSelectInfoItem();
	CPet * pPet = NULL;
	
	if (gGlobal.g_PetLst->GetCount() > 0)
	{
		if (gGlobal.g_PetLst->GetCount() > m_SelIndex && m_SelIndex != -1)
			pPet = gGlobal.g_PetLst->GetChild<CPet>(m_SelIndex);
		else
		{
			pPet = gGlobal.g_PetLst->GetChild<CPet>(0);
			m_SelIndex = 0;
		}
	}
	if (pPet)
	{
		m_currPetUid = pPet->GetPet_uid();
		const PetData * pPetData = gGlobal.m_resource->GetPetData(pPet->GetPet_id());
		if (pPetData)
		{
			StringA name;
			name.Format("PetSex%d", pPet->GetSex() + 1);
			UIImage * pIm = gGlobal.pUIManager->FindUIImage(name.c_str());
			if (pIm)
				pIm->SetEnable(true);			
			
			UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("PetModel");
			if (pAniIcon)
			{
				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(pPetData->pet_model);
				pAniIcon->SetEnable(true);

				String modelName = pPetData->pet_model;
				String colorFileName = modelName.substr(0, modelName.length() - 4);
				char abc = pPetData->pet_color + 96;
				colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
				pAniIcon->GetAniEnt()->ChangeColorP(modelName.c_str(), colorFileName.c_str());

			}

			String none =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE"));;

			for (UInt i = 1; i <= MAX_PET_IN_ONE_PAGE ; i ++)
			{
				name.Format("PetElement%d", i);
				UIImage * pIm = gGlobal.pUIManager->FindUIImage(name.c_str());
				if (pIm)
					pIm->SetEnable(false);
			}
			name.Format("PetElement%d", pPet->GetElement());
			pIm = gGlobal.pUIManager->FindUIImage(name.c_str());
			if (pIm)
				pIm->SetEnable(true);			

			UIImage* pImIsBound = gGlobal.pUIManager->FindUIImage("PetIsBound");
			UIImage* pImNotBound = gGlobal.pUIManager->FindUIImage("PetNotBound");

			if (pImIsBound && pImNotBound)
			{
				if (pPet->GetSoulbound() == SOULBOUND_ALREADY_BOUND)
				{
					pImIsBound->SetEnable(true);
					pImNotBound->SetEnable(false);
				}
				else
				{
					pImIsBound->SetEnable(false);
					pImNotBound->SetEnable(true);
				}
			}

			UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("PetType");
			if (pText)
			{
				String text;
				String temp;
				temp.Format(_T("MSG_PET_TYPE_%d"), pPetData->pet_type);

				text = gGlobal.GetStringTable()->Get(temp.c_str());

				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
/*
			UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("PetFather");
			if (pText)
			{
			if (pPet->GetFather_name().IsEmpty())
			pText->SetText(none.c_str());
			else
			pText->SetText(pPet->GetFather_name().c_str());
			pText->Redraw();
			pText->SetEnable(true);
}
*/
			pText = gGlobal.pUIManager->FindUIStaticText("PetReqLevel");
			if (pText)
			{
				String text;
				text.Format(_T("%d"), pPetData->pet_ReqLV);
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
			//cal the remain day;
			pText = gGlobal.pUIManager->FindUIStaticText("PetLife");
			if (pText)
			{
				String text;
				if (pPet->GetExpireDate() == 0)
				{
					if (pPetData->pet_TimeLimitType == PET_TIME_LIMIT_TYPE_NONE)
						text =gGlobal.GetStringTable()->Get(_T("MSG_INFINITY"));
					else
						text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PET_AFTER_USE")), pPetData->pet_TimeLimit);		
				}
				else 
				{
					time_t now;
					time(&now);

					if (pPet->GetExpireDate() >= (UInt32)now)
					{
						UInt32 remainTime = pPet->GetExpireDate() - (UInt32)now;

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
						if (pPetData->pet_TimeLimitType == PET_TIME_LIMIT_TYPE_NONE)
							text =gGlobal.GetStringTable()->Get(_T("MSG_INFINITY"));
						else
							text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PET_AFTER_USE")), pPetData->pet_TimeLimit);		
					}
				}
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
			/*
			pText = gGlobal.pUIManager->FindUIStaticText("PetMother");
			if (pText)
			{
				
				if (pPet->GetMother_name().IsEmpty())
					pText->SetText(none.c_str());
				else
					pText->SetText(pPet->GetMother_name().c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
*/
			pText = gGlobal.pUIManager->FindUIStaticText("PetSpirit");
			if (pText)
			{
				
				String text;
				if (pPetData->pet_type == 1)
				{
					String temp;
					temp.Format(_T("MSG_PET_SPIRIT_%d"), pPet->GetSpirit());
					text = gGlobal.GetStringTable()->Get(temp.c_str());
				}
				else
					text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}


			UILinerImage * pLiner = gGlobal.pUIManager->FindUILinerImage("PetVPBar");
			if (pLiner)
			{
				if(pPetData->pet_type == 1 && pPet->GetExpireDate() == 0)
				{
					if (pPet->GetMaxVP() == 0)
						pLiner->SetDenumerator(1);
					else
						pLiner->SetDenumerator(pPet->GetMaxVP());

					pLiner->SetNumerator(pPet->GetVP());
					pLiner->SetEnable(true);
				}
				else 
					pLiner->SetEnable(false);
			}

			
			pText = gGlobal.pUIManager->FindUIStaticText("PetVPText");
			
			if (pText)
			{
				String text;
				//text.Format(_T("%d/%d"), pPet->GetVP(), pPet->GetMaxVP());
				if(pPetData->pet_type == 1 && pPet->GetExpireDate() == 0)
					text.Format(_T("%d/%d"), pPet->GetVP(),pPet->GetMaxVP());
				else
					text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}

			UInt32 nextExp =  80;
			//CallLuaFunc(gGlobal.g_pParentLua, "CalPetNextLvExp", String(), pPet->GetLevel() + 1);
			//nextExp = (UInt32)gGlobal.g_TempResult;

			pLiner = gGlobal.pUIManager->FindUILinerImage("PetExp");
			if (pLiner)
			{
				if(pPetData->pet_type == 0)
				{
					if (nextExp == 0)
						pLiner->SetDenumerator(1);
					else
						pLiner->SetDenumerator(nextExp);

					pLiner->SetNumerator(pPet->GetExp());
					
					pLiner->SetEnable(true);
				}
				else
					pLiner->SetEnable(false);
			}
			
			pText = gGlobal.pUIManager->FindUIStaticText("PetExpText");
			
			if (pText)
			{
				String text;
				if(pPetData->pet_type == 0)
					text.Format(_T("%d/%d"), pPet->GetExp(), nextExp);
				else
					text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}

			

			
			pText = gGlobal.pUIManager->FindUIStaticText("PetLevel");
			if (pText)
			{
				String text;
				text.Format(_T("%d"), pPet->GetLevel());
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}

			pText = gGlobal.pUIManager->FindUIStaticText("PetFood");
			if (pText)
			{
				String text;
				if(pPetData->pet_type == 0)
					text.Format(_T("%d/%d"), pPet->GetHappiness(), MAX_PET_HAPPINESS);
				else
					text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}

			pLiner = gGlobal.pUIManager->FindUILinerImage("PetFoodBar");
			if (pLiner)
			{				
				if(pPetData->pet_type == 0)
				{
					pLiner->SetDenumerator(MAX_PET_HAPPINESS);
					pLiner->SetNumerator(pPet->GetHappiness());				
					pLiner->SetEnable(true);
				}
				else
					pLiner->SetEnable(false);
			}

			pText = gGlobal.pUIManager->FindUIStaticText("PetFavor");
			if (pText)
			{
				String text;
				String temp;
				if (pPetData->pet_type == 0)
				{
					UInt8 PetFavorTpye = pPet->GetHappiness()/10;
					if (PetFavorTpye > 8)
						PetFavorTpye = 8;
					else if (PetFavorTpye < 2)
						PetFavorTpye = 2;
					temp.Format(_T("MSG_PET_FAVORTYPE_%d"), PetFavorTpye - 1);
					text = gGlobal.GetStringTable()->Get(temp.c_str());		
				}
				else if (pPetData->pet_type == 1)
				{
					UInt8 PetFavorTpye = pPet->GetVP()/10;
					if (PetFavorTpye > 8)
						PetFavorTpye = 8;
					else if (PetFavorTpye < 2)
						PetFavorTpye = 2;
					temp.Format(_T("MSG_PET_FAVORTYPE_%d"), PetFavorTpye - 1);
					text = gGlobal.GetStringTable()->Get(temp.c_str());	
				}
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
			//skill
			for (UInt i = 0; i < 8; i++)
			{
				StringA name;
				name.Format("PetSkill%d",i);
				gGlobal.pUIManager->SetUIItemEnable(name.c_str(), false);
				if (pPet->m_pSkillGrp)
				{
					CSkill * pSkill = pPet->m_pSkillGrp->GetChild<CSkill>(i);
					if (pSkill)
					{
						UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
						if (pAniIcon)
						{
							const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(pSkill->GetSkill_id());
							if (pSkillData)
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
						}
					}
				}
			}
		}
	}
}
void CMenuPetProcess::PageUp()
{
	if (m_StartIndex > 0)
	{
		m_StartIndex --;
		RefreshRightUpPanel();
	}
}
void CMenuPetProcess::PageDown()
{
	if (gGlobal.g_PetLst)
	{
		if (m_StartIndex + MAX_PET_IN_ONE_PAGE < gGlobal.g_PetLst->GetCount())
		{
			m_StartIndex ++;
			RefreshRightUpPanel();
		}
	}
}
void CMenuPetProcess::Scroll()
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("PetScrollBar");
	if (scrollBar)
	{
		UInt selNum = scrollBar->GetSelNum();
		if (selNum + MAX_PET_IN_ONE_PAGE >  gGlobal.g_PetLst->GetCount())
		{
			if (gGlobal.g_PetLst->GetCount() >= MAX_PET_IN_ONE_PAGE)
				selNum = gGlobal.g_PetLst->GetCount() - MAX_PET_IN_ONE_PAGE;
			else
				selNum = 0;
		}
		if (m_StartIndex != selNum)
		{
			m_StartIndex = selNum;
			RefreshRightUpPanel();
		}
	}
}

void CMenuPetProcess::SetSelIndex(Int index)
{
	if (index + m_StartIndex != m_SelIndex)
	{
		m_SelIndex = index + m_StartIndex;
		RefreshMenu();
		InitUsePetFoodPanel();
	}
}

bool CMenuPetProcess::HasPet()
{
	if (gGlobal.g_PetLst)
	{
		return gGlobal.g_PetLst->GetCount() > 0;
	}

	return false;
}

void CMenuPetProcess::UsePet()
{
	if (IsHawking())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_USE_PET")));
		return;
	}
	if (gGlobal.g_pCharacter->GetCharNetObject()->GetModel_mobid() > 0)
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_CHANGEMOB_NOT_USE")));
		return;
	}
	if (IsPetUse())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_SIT_CANT_USE_PET")));
		return;
	}

	if (gGlobal.g_PetLst)
	{
		CPet * pPet =  gGlobal.g_PetLst->GetChild<CPet>(m_SelIndex);
		if (pPet)
		{
			const PetData * pPetData = gGlobal.m_resource->GetPetData(pPet->GetPet_id());
			if(pPetData)
			{
				if (pPetData->pet_type == 1 && pPet->GetExpireDate() == 0 && pPet->GetVP() <= pPetData->pet_reduce_vp_max)
					gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_CANT_USE_PET_NO_VP")));
				else if(pPetData->pet_type == 0 && pPet->GetHappiness() <= 1)
					gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_CANT_USE_PET_NO_FAVOR")));
				else
					gGlobal.g_PetLst->ChangePetState(gGlobal.pPeerProxy, RPCContext(), pPet->GetPet_uid(), PET_STATE_TYPE_USE);
			}
		}
	}
}
void CMenuPetProcess::PetRest()
{
	if (gGlobal.g_PetLst)
	{
		CPet * pPet =  gGlobal.g_PetLst->GetChild<CPet>(m_SelIndex);
		if (pPet)
		{
			gGlobal.g_PetLst->ChangePetState(gGlobal.pPeerProxy, RPCContext(), pPet->GetPet_uid(), PET_STATE_TYPE_REST);
		}
	}
}
void CMenuPetProcess::DumpPet()
{
	if (IsHawking())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_DROP_PET")));
		return;
	}
	if (gGlobal.g_PetLst)
	{
		CPet * pPet =  gGlobal.g_PetLst->GetChild<CPet>(m_SelIndex);
		if (pPet)
		{
			if (pPet->IsGiven())
			{
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_GIVEN_CANT_DROP_PET")));				
				return;
			}
			if (pPet->GetLock())
			{
                gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_LOCK_CANT_DROP_PET")));				
				return;
			}
			gGlobal.g_PetLst->DeletePet(gGlobal.pPeerProxy, RPCContext(), pPet->GetPet_uid());
		}
	}
}

void CMenuPetProcess::OpenPetSkillDetail(int index)
{
	CPet * pPet =  gGlobal.g_PetLst->GetChild<CPet>(m_SelIndex);
	if (pPet && pPet->m_pSkillGrp && index >= 0 && index < pPet->m_pSkillGrp->GetCount())
	{
		CSkill * pSkill = pPet->m_pSkillGrp->GetChild<CSkill>(index);
		if (pSkill)
		{
			const SkillData * m_pTempSkillDetailData = gGlobal.m_resource->GetSkillData(pSkill->GetSkill_id());

			if (m_pTempSkillDetailData)
			{
				Int MouseX = gGlobal.pUIManager->GetCursorX();
				Int MouseY = gGlobal.pUIManager->GetCursorY();

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
		
				UIChatPanel * pChatPanel = gGlobal.pUIManager->FindUIChatPanel(SkillChat);

				if (pChatPanel)
				{
					pChatPanel->ClearAllRow();
					//add skill name
					//pChatPanel->AddStringW(m_pTempSkillDetailData->name);
					String name;
					//name.Format(_T("#W%s"), m_pTempSkillDetailData->name);
					name.Format(_T("#W%s\\r#Y%s"), m_pTempSkillDetailData->name, m_pTempSkillDetailData->skill_functionDes);
					pChatPanel->AddStringW(name.c_str());
					if (wcscmp(m_pTempSkillDetailData->skill_description, _T("0")) != 0)
						pChatPanel->AddStringW(m_pTempSkillDetailData->skill_description);

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
void CMenuPetProcess::ClosePetSkillDetail()
{
	gGlobal.pUIManager->DeletePanel(SkillDetailPanelName);
}

void CMenuPetProcess::ShowPetDetail(CPet* pPet, UInt16Vector skillIDs)
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PetDetailPanel");
	if (pPet && pPanel)
	{
		m_currPetUid = pPet->GetPet_uid();
		const PetData * pPetData = gGlobal.m_resource->GetPetData(pPet->GetPet_id());
		if (pPetData)
		{
			
			UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("PetDetailModel");
			if (pAniIcon)
			{
				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(pPetData->pet_model);

				String modelName = pPetData->pet_model;
				String colorFileName = modelName.substr(0, modelName.length() - 4);
				char abc = pPetData->pet_color + 96;
				colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
				pAniIcon->GetAniEnt()->ChangeColorP(modelName.c_str(), colorFileName.c_str());

				pAniIcon->SetEnable(true);
			}

			String none =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE"));;

			StringA name;

			name.Format("PetDetailElement%d", pPet->GetElement());
			UIImage * pIm = gGlobal.pUIManager->FindUIImage(name.c_str());
			if (pIm)
				pIm->SetEnable(true);
	
			UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("PetDetailName");
			if (pText)
			{
				pText->SetText(pPet->GetName().c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}

			StringA sexName;
			sexName.Format("PetDetailSex%d", pPet->GetSex());
			UIImage* img = gGlobal.pUIManager->FindUIImage(sexName.c_str());
			if (img)
				img->SetEnable(true);

			//cal the remain day;
			pText = gGlobal.pUIManager->FindUIStaticText("PetDetailLife");
			if (pText)
			{
				String text;
				if (pPet->GetExpireDate() == 0)
				{
					if (pPetData->pet_TimeLimitType == PET_TIME_LIMIT_TYPE_NONE)
						text =gGlobal.GetStringTable()->Get(_T("MSG_INFINITY"));
					else
						text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PET_AFTER_USE")), pPetData->pet_TimeLimit);		
				}
				else 
				{
					time_t now;
					time(&now);

					if (pPet->GetExpireDate() >= (UInt32)now)
					{
						UInt32 remainTime = pPet->GetExpireDate() - (UInt32)now;

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
						if (pPetData->pet_TimeLimitType == PET_TIME_LIMIT_TYPE_NONE)
							text =gGlobal.GetStringTable()->Get(_T("MSG_INFINITY"));
						else
							text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PET_AFTER_USE")), pPetData->pet_TimeLimit);		
					}
				}
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
			

			pText = gGlobal.pUIManager->FindUIStaticText("PetDetailSpirit");
			if (pText)
			{	
				String text;
				//text.Format(_T("%d"), pPet->GetSpirit());
				if (pPetData->pet_type == 1)
				{
					String temp;
					temp.Format(_T("MSG_PET_SPIRIT_%d"), pPet->GetSpirit());
					text = gGlobal.GetStringTable()->Get(temp.c_str());
				}
				else
					text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}

			UILinerImage * pLiner = gGlobal.pUIManager->FindUILinerImage("PetDetailVPBar");
			if (pLiner)
			{
				if(pPetData->pet_type == 1)
				{
					if (pPet->GetMaxVP() == 0)
						pLiner->SetDenumerator(1);
					else
						pLiner->SetDenumerator(pPet->GetMaxVP());
					
					pLiner->SetNumerator(pPet->GetVP());
					pLiner->SetEnable(true);
				}
				else
					pLiner->SetEnable(false);
			}
			
			pText = gGlobal.pUIManager->FindUIStaticText("PetDetailVPText");
			
			if (pText)
			{
				String text;
				if(pPetData->pet_type == 1)
					text.Format(_T("%d/%d"), pPet->GetVP(), pPet->GetMaxVP());
				else
					text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}


			pLiner = gGlobal.pUIManager->FindUILinerImage("PetDetailFoodBar");
			if (pLiner)
			{				
				if(pPetData->pet_type == 0)
				{
					pLiner->SetDenumerator(MAX_PET_HAPPINESS);
					pLiner->SetNumerator(pPet->GetHappiness());				
					pLiner->SetEnable(true);
				}
				else
					pLiner->SetEnable(false);
			}

			pText = gGlobal.pUIManager->FindUIStaticText("PetDetailFood");
			if (pText)
			{
				String text;
				if(pPetData->pet_type == 0)
					text.Format(_T("%d/%d"), pPet->GetHappiness(), MAX_PET_HAPPINESS);
				else
					text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}


			UInt32 nextExp =  80;
			//CallLuaFunc(gGlobal.g_pParentLua, "CalPetNextLvExp", String(), pPet->GetLevel() + 1);
			//nextExp = (UInt32)gGlobal.g_TempResult;

			pLiner = gGlobal.pUIManager->FindUILinerImage("PetDetailExp");
			if (pLiner)
			{
				if(pPetData->pet_type == 0)
				{
					if (nextExp == 0)
						pLiner->SetDenumerator(1);
					else
						pLiner->SetDenumerator(nextExp);

					pLiner->SetNumerator(pPet->GetExp());
					pLiner->SetEnable(true);
				}
				else
					pLiner->SetEnable(false);
			}
			
			pText = gGlobal.pUIManager->FindUIStaticText("PetDetailExpText");
			
			if (pText)
			{
				String text;
				if(pPetData->pet_type == 0)
					text.Format(_T("%d/%d"), pPet->GetExp(), nextExp);
				else
					text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}

			pText = gGlobal.pUIManager->FindUIStaticText("PetDetailLevel");
			if (pText)
			{
				String text;
				text.Format(_T("%d"), pPet->GetLevel());
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}

			pText = gGlobal.pUIManager->FindUIStaticText("PetDetailType");
			if (pText)
			{
				String text;
				String temp;
				temp.Format(_T("MSG_PET_TYPE_%d"), pPetData->pet_type);
				text = gGlobal.GetStringTable()->Get(temp.c_str());

				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}


			pText = gGlobal.pUIManager->FindUIStaticText("PetDetailFavor");
			if (pText)
			{
				String text;
				String temp;
				if (pPetData->pet_type == 0)
				{
					UInt8 PetFavorTpye = pPet->GetHappiness()/10 ;
					if (PetFavorTpye > 8)
						PetFavorTpye = 8;
					else if (PetFavorTpye < 2)
						PetFavorTpye = 2;
					temp.Format(_T("MSG_PET_FAVORTYPE_%d"), PetFavorTpye -1);
					text = gGlobal.GetStringTable()->Get(temp.c_str());		
				}
				else if (pPetData->pet_type == 1)
				{
					UInt8 PetFavorTpye = pPet->GetVP()/10;
					if (PetFavorTpye > 8)
						PetFavorTpye = 8;
					else if (PetFavorTpye < 2)
						PetFavorTpye = 2;
					temp.Format(_T("MSG_PET_FAVORTYPE_%d"), PetFavorTpye -1);
					text = gGlobal.GetStringTable()->Get(temp.c_str());	
				}
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}

			//skill
			for (UInt i = 0; i < skillIDs.size(); i++)
			{
				StringA name;
				name.Format("PetDetailSkill%d",i);
				gGlobal.pUIManager->SetUIItemEnable(name.c_str(), false);
				
				UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
				const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(skillIDs[i]);
				if (pAniIcon && pSkillData)
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
					String	ShowName;
					if(pSkillData->skill_functionDes[0] == _T('0'))
						ShowName.Format(_T("#W%s"), pSkillData->name);
					else
						ShowName.Format(_T( "#W%s\\r#Y%s"), pSkillData->name, pSkillData->skill_functionDes);
									pAniIcon->SetTips(ShowName.c_str());
				}					
			}
		}
	}
}

void CMenuPetProcess::ShowPetDetail(UInt32 petID, UInt8 petLvl)
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PetDetailPanel");
	const PetData * pPetData = gGlobal.m_resource->GetPetData(petID);
	if (pPetData)
	{
		UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("PetDetailModel");
		if (pAniIcon)
		{
			pAniIcon->ClearAniData();
			pAniIcon->SetAnimationData(pPetData->pet_model);
			pAniIcon->SetEnable(true);

			String modelName = pPetData->pet_model;
			String colorFileName = modelName.substr(0, modelName.length() - 4);
			char abc = pPetData->pet_color + 96;
			colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
			pAniIcon->GetAniEnt()->ChangeColorP(modelName.c_str(), colorFileName.c_str());
		}

		String none =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE"));;
		StringA name;

		name.Format("PetDetailElement%d", pPetData->pet_element);
		UIImage * pIm = gGlobal.pUIManager->FindUIImage(name.c_str());
		if (pIm)
		{
			pIm->SetEnable(true);
		}

		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("PetDetailName");
		if (pText)
		{
			pText->SetText(String(pPetData->pet_name).c_str());
			pText->Redraw();
			pText->SetEnable(true);
		}

		StringA sexName;
		sexName.Format("PetDetailSex%d", pPetData->pet_sex);
		UIImage* img = gGlobal.pUIManager->FindUIImage(sexName.c_str());
		if (img)
			img->SetEnable(true);

		pText = gGlobal.pUIManager->FindUIStaticText("PetDetailType");
		if (pText)
		{
			String text;
			String temp;
			temp.Format(_T("MSG_PET_TYPE_%d"), pPetData->pet_type);
			text = gGlobal.GetStringTable()->Get(temp.c_str());

			pText->SetText(text.c_str());
			pText->Redraw();
			pText->SetEnable(true);
		}

		pText = gGlobal.pUIManager->FindUIStaticText("PetDetailLife");
		if (pText)
		{
			String remainDay;
			if (pPetData->pet_TimeLimit > 0)
				remainDay.Format(_T("%d%s"), pPetData->pet_TimeLimit,gGlobal.GetStringTable()->Get(_T("MSG_DAY")));
			else
				remainDay =gGlobal.GetStringTable()->Get(_T("MSG_INFINITY"));
			pText->SetText(remainDay);
			pText->Redraw();
			pText->SetEnable(true);
		}

		pText = gGlobal.pUIManager->FindUIStaticText("PetDetailSpirit");
		if (pText)
		{	
			String text;
			//text.Format(_T("%d"), pPet->GetSpirit());
			text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
			pText->SetText(text.c_str());
			pText->Redraw();
			pText->SetEnable(true);
		}

		UILinerImage * pLiner = gGlobal.pUIManager->FindUILinerImage("PetDetailVPBar");
		if (pLiner)
		{
			if(pPetData->pet_type == 1)
			{
				if (pPetData->pet_MaxVP == 0)
					pLiner->SetDenumerator(1);
				else
					pLiner->SetDenumerator(pPetData->pet_MaxVP);

				pLiner->SetNumerator(pPetData->pet_MaxVP);
				pLiner->SetEnable(true);
			}
			else
				pLiner->SetEnable(false);
		}

		pText = gGlobal.pUIManager->FindUIStaticText("PetDetailVPText");

		if (pText)
		{
			String text;
			if(pPetData->pet_type == 1)
				text.Format(_T("%d/%d"), pPetData->pet_MaxVP, pPetData->pet_MaxVP);
			else
				text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
			pText->SetText(text.c_str());
			pText->Redraw();
			pText->SetEnable(true);
		}

		pLiner = gGlobal.pUIManager->FindUILinerImage("PetDetailExp");
		if (pLiner)
		{
			if(pPetData->pet_type == 0)
			{
				pLiner->SetDenumerator(80);
				pLiner->SetNumerator(0);
				pLiner->SetEnable(true);
			}
			else
				pLiner->SetEnable(false);
		}

		pText = gGlobal.pUIManager->FindUIStaticText("PetDetailExpText");

		if (pText)
		{
			String text;
			if(pPetData->pet_type == 0)
				text.Format(_T("%d/%d"), 0, 80);
			else
				text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
			pText->SetText(text.c_str());
			pText->Redraw();
			pText->SetEnable(true);
		}

		pText = gGlobal.pUIManager->FindUIStaticText("PetDetailLevel");
		if (pText)
		{
			String text;
			text.Format(_T("%d"), petLvl);
			pText->SetText(text.c_str());
			pText->Redraw();
			pText->SetEnable(true);
		}

		pText = gGlobal.pUIManager->FindUIStaticText("PetDetailReqLv");
		if (pText)
		{
			String text;
			text.Format(_T("%d"), pPetData->pet_ReqLV);
			pText->SetText(text.c_str());
			pText->Redraw();
			pText->SetEnable(true);
		}

		StringA mobSkillFile;
		mobSkillFile.Format("PetSkill%05d.csv",pPetData->pet_SkillList); 
		const MobSkillCombination* mobSkillLst = gGlobal.m_resource->GetMobSkillCombination(mobSkillFile.c_str());
		UInt16Vector skillIDs;
		if (mobSkillLst)
		{
			for (UInt8 i = 0; i < mobSkillLst->skillLst.size(); ++i)
			{
				MobSkillDef* pDef = mobSkillLst->skillLst[i];
				if (pDef && pDef->skillRate == 100000)
					skillIDs.push_back(pDef->skillId);
			}
		}

		//skill
		for (UInt i = 0; i < skillIDs.size(); i++)
		{
			StringA name;
			name.Format("PetDetailSkill%d",i);
			gGlobal.pUIManager->SetUIItemEnable(name.c_str(), false);

			UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
			const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(skillIDs[i]);
			if (pAniIcon && pSkillData)
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

				String	ShowName;
				if(pSkillData->skill_functionDes[0] == _T('0'))
					ShowName.Format(_T("#W%s"), pSkillData->name);
				else
					ShowName.Format(_T("#W%s\\r#Y%s"), pSkillData->name, pSkillData->skill_functionDes);
				pAniIcon->SetTips(ShowName.c_str());
			}					
		}
	}
}




void CMenuPetProcess::ShowPublicPetDetail(UInt32 char_id, UInt32 Pet_id)
{
	MapService_Proxy::ShowPublicPetDetail(gGlobal.pPeerProxy, RPCContext(), char_id, Pet_id, gGlobal.g_pCharacter->GetChar_id());
}

bool CMenuPetProcess::CanEatFood()
{
	if (gGlobal.g_PetLst)
	{
		CPet * pPet =  gGlobal.g_PetLst->GetChild<CPet>(m_SelIndex);
		if (pPet)
		{
			const PetData * pPetData = gGlobal.m_resource->GetPetData(pPet->GetPet_id());
			if (pPet->GetLevel() >= 10)
			{
				String tempStr = gGlobal.GetStringTable()->Get(_T("MSG_NOT_EATFOOD_PETLEVEL_MAX"));
				gGlobal.PrintMessage(tempStr, CHANNELTYPE_SYSTEM);	
				return false;
			}
			if(pPetData)
			{
				if (pPet->GetExpireDate() == 0 || pPetData->pet_type == 0)
				{
					return true;
				}
				else
				{
					String temp = gGlobal.GetStringTable()->Get(_T("MSG_NOT_CAN_EAT_FOOD"));
					gGlobal.PrintMessage(temp, CHANNELTYPE_SYSTEM);	
					return false;
				}
			}
		}
	}
	return false;
}

bool CMenuPetProcess::PetCanUseItem(UInt16 itemSlot)
{
	if (!HasPet())
		return false;
	if (m_useItemSlot != PET_NOT_USE_ITEM)
		return false;
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);
	if(item)
	{
		if (gGlobal.g_PetLst)
		{
			CPet * pPet =  gGlobal.g_PetLst->GetChild<CPet>(m_SelIndex);
			if (pPet)
			{
				const PetData * pPetData = gGlobal.m_resource->GetPetData(pPet->GetPet_id());
				if(pPetData)
				{
					if (pPetData->pet_type == 0 && item->m_data->item_type == 1 &&item->m_data->eqType < EQTYPE_AMULET)
						return true;
					else if (pPet->GetExpireDate() == 0 && pPetData->pet_type == 1 &&
						item->m_data->item_type == ITEMTYPE_PET_FOOD)
						return true;
					else
						return false;
				}
				else
					return false;
			}
		}
	}
	return false;
}

void  CMenuPetProcess::SetUseFoodItem(UInt16 itemSlot)
{
	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon("UseItemAI");
	UIImage* img = gGlobal.pUIManager->FindUIImage("UseItemBG");

	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);
	if(m_useItemSlot == PET_NOT_USE_ITEM)
	{
		if (item && item->m_data)
		{
			if (img && pAniIcon)
			{		
				pAniIcon->ClearAniData();
				img->SetEnable(true);
				pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
				pAniIcon->SetRenderStyle(1);
				pAniIcon->SetEnable(true);
			}
			item->SetGivenStack(1);
			gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot % BAG_ITEM_COUNT, true);
			
		}
		m_useItemSlot = itemSlot;
	}
}

void CMenuPetProcess::ReSetFoodUseItem()
{
	if (m_useItemSlot !=  PET_NOT_USE_ITEM)
	{
		CItem* item = gGlobal.m_pBag->GetItem(m_useItemSlot);
		if (item)
		{
			item->SetGivenStack(0);
			gGlobal.g_pItemProcess->UpdateItemSlot(m_useItemSlot % BAG_ITEM_COUNT, true);
		}
		m_useItemSlot = PET_NOT_USE_ITEM;
	}
}

void CMenuPetProcess::PetUseFood()
{
	String tempStr;
	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon("UseItemAI");

	if(m_useItemSlot != PET_NOT_USE_ITEM)
	{
		CItem* item = gGlobal.m_pBag->GetItem(m_useItemSlot);
		if(item)
		{
			if (item->GetLock())
			{
				tempStr = gGlobal.GetStringTable()->Get(_T("MSG_FOOD_ITEN_LOCK"));
				gGlobal.PrintMessage(tempStr, CHANNELTYPE_SYSTEM);	
				return;
			}
			if (gGlobal.g_PetLst)
			{
				CPet * pPet =  gGlobal.g_PetLst->GetChild<CPet>(m_SelIndex);
				if (pPet)
				{
					const PetData * pPetData = gGlobal.m_resource->GetPetData(pPet->GetPet_id());
					if(pPetData)
					{
						if (pPetData->pet_type == 0 && item->m_data->item_type == 1 &&item->m_data->eqType < EQTYPE_AMULET)
						{
							if(pPet->GetHappiness() <= 90 && pPet->GetLevel() < 10)
							{
								m_useFoodMsg = true;
								UInt16 addExp = (item->m_data->reqLv/10+1)*((item->m_data->color+1)*2);
								pPet->ChangePetExp(gGlobal.pPeerProxy, RPCContext(),addExp);
								pPet->ChangePetFavor(gGlobal.pPeerProxy, RPCContext(),10);
								gGlobal.g_pItemProcess->DropItem(m_useItemSlot, 1);
							}
							else if (pPet->GetLevel() >= 10)
							{
								tempStr = gGlobal.GetStringTable()->Get(_T("MSG_NOT_EATFOOD_PETLEVEL_MAX"));
								item->SetGivenStack(0);
								gGlobal.PrintMessage(tempStr, CHANNELTYPE_SYSTEM);	
								gGlobal.g_pItemProcess->UpdateItemSlot(m_useItemSlot % BAG_ITEM_COUNT, true);
							}
							else
							{
								tempStr = gGlobal.GetStringTable()->Get(_T("MSG_PET_FAVOR_MAX"));
								item->SetGivenStack(0);
								gGlobal.PrintMessage(tempStr, CHANNELTYPE_SYSTEM);	
								gGlobal.g_pItemProcess->UpdateItemSlot(m_useItemSlot % BAG_ITEM_COUNT, true);
							}
						}
						else if (pPet->GetExpireDate() == 0 && pPetData->pet_type == 1 && 
							item->m_data->item_type == ITEMTYPE_PET_FOOD)
						{
							if(pPet->GetVP() <= PET_USE_FOOD_MAX_VP)
							{
								m_useFoodMsg = true;
								pPet->ChangePetVp(gGlobal.pPeerProxy, RPCContext(),item->m_data->restore_VP);
								gGlobal.g_pItemProcess->DropItem(m_useItemSlot, 1);
							}
							else
							{
								tempStr = gGlobal.GetStringTable()->Get(_T("MSG_NOT_EATFOOD_PETVP_MAX"));
								item->SetGivenStack(0);
								gGlobal.PrintMessage(tempStr, CHANNELTYPE_SYSTEM);	
								gGlobal.g_pItemProcess->UpdateItemSlot(m_useItemSlot % BAG_ITEM_COUNT, true);
							}
						}
						else
						{
							tempStr = gGlobal.GetStringTable()->Get(_T("MSG_PET_USE_FOODTPYE_ERRO"));
							item->SetGivenStack(0);
							gGlobal.PrintMessage(tempStr, CHANNELTYPE_SYSTEM);	
							gGlobal.g_pItemProcess->UpdateItemSlot(m_useItemSlot % BAG_ITEM_COUNT, true);
						}
						if (pAniIcon )
						{		
							pAniIcon->ClearAniData();
							gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
						}
						m_useItemSlot = PET_NOT_USE_ITEM;
					}
				}
			}
		}
	}
	else
	{
		tempStr = gGlobal.GetStringTable()->Get(_T("MSG_NOT_FOOD"));
		gGlobal.PrintMessage(tempStr, CHANNELTYPE_SYSTEM);	
	}
}

void CMenuPetProcess::InitUsePetFoodPanel()
{
	UIStaticText * pPetName = gGlobal.pUIManager->FindUIStaticText("UseFoodPetName");
	String foodtypeStr;
	String pettypeStr;
	if (pPetName)
	{
		if (gGlobal.g_PetLst)
		{
			CPet * pPet =  gGlobal.g_PetLst->GetChild<CPet>(m_SelIndex);
			if (pPet)
			{
				const PetData * pPetData = gGlobal.m_resource->GetPetData(pPet->GetPet_id());
				if(pPetData)
				{
					if (pPetData->pet_type == 0)
					{
						SetUIEnable("UseFoodPet0",true);
						SetUIEnable("UseFoodEatEQ",true);
						SetUIEnable("UseFoodEatFD",false);
						SetUIEnable("UseFoodPet1",false);
					}
					else if (pPetData->pet_type == 1)
					{
						SetUIEnable("UseFoodPet0",false);
						SetUIEnable("UseFoodEatEQ",false);
						SetUIEnable("UseFoodEatFD",true);
						SetUIEnable("UseFoodPet1",true);
					}
					pPetName->SetText(pPet->GetName().c_str());
					pPetName->Redraw();
					pPetName->SetEnable(true);
				}
			}
		}
	}
}