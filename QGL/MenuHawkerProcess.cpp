//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuHawkerProcess.h"
#include "MenuItemProcess.h"
#include "MenuPartnerProcess.h"
#include "MenuPetProcess.h"
#include "Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "Resource/ResItem.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/ItemBag.h"
#include "Resource/ClientResManager.h"
#include "CharacterControl.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/Pet.h"
#include "Proxy/Logic/PetGroup.h"
#include "Resource/ResPet.h"
#include "Proxy/Logic/HawkItem.h"
#include "Proxy/Logic/HawkGroup.h"

#include "QGLApplication.h"
#include "MapControl.h"
#include "Math.h"

#define MAX_RECORD_PER_PAGE 6

BOOL operator==(HawkerItemInfo first, HawkerItemInfo second)
{
	bool eq = true;
	if (first.item_uid != second.item_uid)
		eq = false;
	return eq;
};



CMenuHawkerProcess::CMenuHawkerProcess()
{
	m_iBagIndex = 0;
	m_iMenuIndex = 0;
	m_iStartIndex = 0;
	m_iSelectedPartnerIndex = 0;
	m_iSelectedPetIndex = 0;
	m_iHawkMenuIndex = HAWKER_CLOSE;
	m_iHawkPageIndex = 0;
	m_targetId = 0;

	m_hawkerItems.clear();
	m_hawkerPartners.clear();
	m_hawkerPets.clear();

	m_InithawkerItems.clear();
	m_InithawkerPartners.clear();
	m_InithawkerPets.clear();

	m_concernCharIdList.clear();
	m_startHawkCharIdList.clear();

	m_bOpenHawk = false;
	m_bStartHawk = false;
	m_bSetConcernTarget = false;
	m_allHawkEnable = true;

	m_hawkItemList.clear();
	m_hawkPartnerList.clear();
	m_hawkPetList.clear();

	m_hawkerHistory.clear();


}

CMenuHawkerProcess::~CMenuHawkerProcess()
{
	m_iBagIndex = 0;
	m_iMenuIndex = 0;
	m_iStartIndex = 0;
	m_iSelectedPartnerIndex = 0;
	m_iSelectedPetIndex = 0;
	m_iHawkMenuIndex = 0;
	m_iHawkPageIndex = 0;
	m_targetId = 0;

	m_hawkerItems.clear();
	m_hawkerPartners.clear();
	m_hawkerPets.clear();

	m_InithawkerItems.clear();
	m_InithawkerPartners.clear();
	m_InithawkerPets.clear();


	m_hawkerHistory.clear();

	m_concernCharIdList.clear();
	m_startHawkCharIdList.clear();

	m_bOpenHawk = false;
	m_bStartHawk = false;
	m_bSetConcernTarget = false;
	m_allHawkEnable = true;

	m_hawkItemList.clear();
	m_hawkPartnerList.clear();
	m_hawkPetList.clear();
}

void CMenuHawkerProcess::UpdateOwnerItem(int itemBag)
{
	if (itemBag >= 0)
		m_iBagIndex = itemBag;

	for (Index i = C_INDEX(0); i < 24; ++i)
		UpdateOwnerItemSlot(i, true);

}

void CMenuHawkerProcess::UpdateOwnerItemSlot(UInt8 itemSlot, bool addDirty)
{
	String Text;
	StringA componentName;
	UIAniIcon* pAniIcon;
	String entFileName;

	componentName.Format("HawkerOwnerSlot%03iIcon", itemSlot );

	pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(componentName.c_str());
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot +  24 * m_iBagIndex);

	if(pAniIcon)
	{
		if (item )
		{
			const ItemData* pItemData = item->m_data;
			if( pItemData )
			{
				componentName.Format("HawkerOwnerSlot%03iStack", itemSlot );
				if( item->GetStack() > 1 )
					//Text.Format(_T("%i"), item->GetStack() - item->GetGivenStack() ); 
					Text.Format(_T("%i"), item->GetStack()); 
				else
					Text = "";
				SetUIEnable(componentName, true);
				gGlobal.g_pItemProcess->SetItemText(componentName, Text, 0, addDirty);
				componentName.Format("HawkerOwnerSlot%03iLvl", itemSlot );
				if( item->GetGrade() > 0 )
					Text.Format(_T("+%i"), item->GetGrade()); 
				else
					Text = "";
				SetUIEnable(componentName, true);
				gGlobal.g_pItemProcess->SetItemText(componentName, Text, 0, addDirty);

				String temp(pItemData->icon2);

				componentName.Format("HawkerOwnerSlot%03iRed", itemSlot );
				if (gGlobal.g_pItemProcess->NeedRedBox((itemSlot +  24 * m_iBagIndex)))
					SetUIEnable(componentName, true);
				else
					SetUIEnable(componentName, false);

				if (gGlobal.g_pItemProcess->IsItemGiven((itemSlot +  24 * m_iBagIndex)))
					pAniIcon->SetRenderStyle(1);//uIm->SetRenderStyle(1);
				else
					pAniIcon->SetRenderStyle(0);//uIm->SetRenderStyle(0);

				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(temp.c_str());
				if (!pAniIcon->GetAniEnt()->HasAniFile())
				{
					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(String(_T("itemSell0000s.ent")).c_str());
				}
				pAniIcon->SetEnable(true);

				StringA lockName;
				lockName.Format("HawkerSlot%03iLock", itemSlot);
				SetUIEnable(lockName, item->GetLock());

				if (addDirty)
					gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);//uIm);
			}
			else
			{	
				SetIconOff(itemSlot, false);
				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(String(_T("itemSell0000s.ent")).c_str());
				pAniIcon->SetEnable(true);
			}
		}
		else
		{
			StringA lockName;
			lockName.Format("HawkerSlot%03iLock", itemSlot);
			SetUIEnable(lockName, false);	
			SetIconOff(itemSlot, false);
		}
	}
}

void CMenuHawkerProcess::SetIconOff(int iconPos, bool eq)
{
	String Text;
	StringA componentName;
	UIAniIcon* pAniIcon = NULL;

	if (eq)
	{
		if (iconPos >=0 && iconPos < 9)
		{
			componentName.Format("HawkerOwnerEquip%03iIcon", iconPos );
			pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(componentName.c_str());

			if (pAniIcon)
			{
				componentName.Format("HawkerOwnerEquip%03iStack", iconPos);
				Text = "";
				gGlobal.g_pItemProcess->SetItemText(componentName, Text, 0, true);
				componentName.Format("HawkerOwnerEquip%03iLvl", iconPos);
				Text = "";
				gGlobal.g_pItemProcess->SetItemText(componentName, Text, 0, true);
				pAniIcon->SetEnable(false);
			}
		}
	}
	else
	{
		if (iconPos >=0 && iconPos < 24 )
		{
			componentName.Format("HawkerOwnerSlot%03iIcon", iconPos );
			pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(componentName.c_str());

			if (pAniIcon)
			{
				componentName.Format("HawkerOwnerSlot%03iStack", iconPos );
				Text = "";
				SetUIEnable(componentName, false);
				gGlobal.g_pItemProcess->SetItemText(componentName, Text, 0, true);
				componentName.Format("HawkerOwnerSlot%03iLvl", iconPos );
				Text = "";
				SetUIEnable(componentName, false);
				gGlobal.g_pItemProcess->SetItemText(componentName, Text, 0, true);
				pAniIcon->SetEnable(false);
				componentName.Format("HawkerOwnerSlot%03iRed", iconPos );
				SetUIEnable(componentName, false);
			}
		}
	}
}

void CMenuHawkerProcess::UpdateOwnerEqItem()
{
	if( gGlobal.m_pBag && gGlobal.m_resource && m_iMenuIndex == ITEM_CHAR_MENU)
	{
		String Text;
		CItem* item;
		StringA componentName;

		UIAniIcon* pAniIcon;

		for (Index i = C_INDEX(0); i < C_INDEX(9); ++i)
		{
			componentName.Format("HawkerOwnerEquip%03iIcon", i );

			pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(componentName.c_str());
			if( pAniIcon)
			{
				item = gGlobal.m_pBag->GetItem((UInt8)i+120);
				if( item )
				{
					const ItemData* pItemData = item->m_data;
					if( pItemData )
					{
						componentName.Format("HawkerOwnerEquip%03iStack", i);
						if( item->GetStack() > 1 )
							Text.Format(_T("%i"), item->GetStack()); 
						else
							Text = "";
						gGlobal.SetUIStaticText(componentName, Text);
						componentName.Format("HawkerOwnerEquip%03iLvl", i);
						if( item->GetGrade() > 0 )
							Text.Format(_T("+%i"), item->GetGrade()); 
						else
							Text = "";
						gGlobal.SetUIStaticText(componentName, Text);

						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(String(pItemData->icon2).c_str());
						pAniIcon->SetEnable(true);
						gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
					}
					else
						SetIconOff(i, true);
				}
				else
					SetIconOff(i, true);
			}
		}
	}
}

void CMenuHawkerProcess::UpdateOwnerModel()
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("HawkerOwnerModel");

		if (pAniIcon)
		{
			CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
			if( pchar )
			{
				UInt32 eqCloth = pchar->GetOrg_eq_cloth();
				UInt16 eqClothColor = pchar->GetOrg_eq_clothColor();
				UInt16 hairStyle = pchar->GetOrg_hairStyle1();
				UInt16 hairColor = pchar->GetOrg_hairColor();

				if (pchar->GetHairStyle1() > 0)
					hairStyle = pchar->GetHairStyle1();
				if (pchar->GetHairColor() > 0)
					hairColor = pchar->GetHairColor();
				if (pchar->GetEq_cloth() > 0)
				{
					eqCloth = pchar->GetEq_cloth();
					//if (pchar->GetEq_clothColor() > 0)
					eqClothColor = pchar->GetEq_clothColor();
				}

				CAnimatedEntity temp = gGlobal.GetCharModel(pchar->GetSexClass(), hairStyle, hairColor, 
					eqCloth, eqClothColor, pchar->GetEq_headBand(), pchar->GetEq_weapon());

				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(&temp);
				pAniIcon->SetAnimationIndex(temp.GetAnimationIndex());				
				pAniIcon->SetRotation(SOUTHEAST);
				gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
			}
		}
	}
}

void CMenuHawkerProcess::UpdateOwner()
{
	m_iMenuIndex = ITEM_CHAR_MENU;
	UpdateOwnerModel();
	UpdateOwnerEqItem();
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if( pchar )
	{
		String Text;
		Text.Format(_T("%i"), pchar->GetMoney());
		gGlobal.g_pItemProcess->SetItemText("HawkerOwnerCashDollar", Text, 0, true);
		Text.Format(_T("%i"), pchar->GetBankMoney());
		gGlobal.g_pItemProcess->SetItemText("HawkerOwnerBankDollar", Text, 0, true);
		Text.Format(_T("%i"), pchar->GetYuanBao());
		gGlobal.g_pItemProcess->SetItemText("HawkerOwnerGoldDollar", Text, 0, true);
		Text.Format(_T("%i"), pchar->GetSMoney());
		gGlobal.g_pItemProcess->SetItemText("HawkerOwnerSCashDollar", Text, 0, true);
	}

}

void CMenuHawkerProcess::UpdateOwnerPartner(int index)
{
	m_iMenuIndex = ITEM_PARTNER_MENU;

	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerGroup->GetCount() > 0)
	{
		String Text;

		if (index >= 0)
			m_iSelectedPartnerIndex = index + m_iStartIndex;

		UpdateOwnerPartnerNameList();

		CPartner* partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(m_iSelectedPartnerIndex);
		UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("HawkerOwnerModel");

		if (pAniIcon && partner && partner->m_data)
		{

			MobDB_Client * mobTemplate = partner->m_data;

			CAnimatedEntity temp;
			temp.InitEntPicInfo();
			temp.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
			String modelName = String(mobTemplate->m_basicinfo->model);
			temp.LoadEntityFile(modelName.c_str(), gGlobal.pD3DDevice);

			String colorFileName = modelName.substr(0, modelName.length() - 5);
			char abc = partner->GetOutLookColor() + 97;
			colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
			temp.ChangeColorP(modelName.c_str(), colorFileName.c_str());

			pAniIcon->ClearAniData();
			pAniIcon->SetAnimationData(&temp);
			pAniIcon->SetAnimationIndex(temp.GetBattleAnimation());				
			pAniIcon->SetRotation(SOUTHEAST);
			gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);

			float tempfavor = partner->GetFavor() - (UInt8)partner->GetFavor();

			if (tempfavor > 0.1)
				Text.Format(_T("%d"), (UInt8)partner->GetFavor() + 1);
			else
				Text.Format(_T("%d"), (UInt8)partner->GetFavor());
			gGlobal.SetUIStaticText("HawkerOwnerPartnerLikeText", Text, true);
			Text.Format(_T("%d/%d"), partner->GetHP(), partner->GetMaxHP() );
			gGlobal.SetUIStaticText("HawkerOwnerPartnerHPText", Text);
			Text.Format(_T("%d/%d"), partner->GetSP(), partner->GetMaxSP() );
			gGlobal.SetUIStaticText("HawkerOwnerPartnerSPText", Text);

			gGlobal.SetUILinerImage("HawkerOwnerPartnerHPBar", partner->GetMaxHP(), partner->GetHP() );
			gGlobal.SetUILinerImage("HawkerOwnerPartnerSPBar", partner->GetMaxSP(), partner->GetSP() );			
		}
	}
	else
	{
		gGlobal.SetUILinerImage("HawkerOwnerPartnerHPBar", 0, 1 );
		gGlobal.SetUILinerImage("HawkerOwnerPartnerSPBar", 0, 1 );

		gGlobal.SetUIStaticText("HawkerOwnerPartnerLikeText", _T(""));
		gGlobal.SetUIStaticText("HawkerOwnerPartnerHPText", _T(""));
		gGlobal.SetUIStaticText("HawkerOwnerPartnerSPText", _T(""));

		UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("HawkerOwnerModel");

		if (pAniIcon)
		{
			pAniIcon->ClearAniData();
			gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
		}

		for( Index i = C_INDEX(0); i < 6; ++i )
		{
			StringA componentName;
			StringA itemNameImage;
			StringA state;
			StringA sellImage;

			componentName.Format("HawkerOwnerNameListLine%01d", i+1 );
			itemNameImage.Format("HawkerOwnerNameImage%01d", i+1 );

			SetUIEnable(itemNameImage, false);
			state.Format("HawkerOwnerPartnerState%01d2", i+1);
			SetUIEnable(state, false);
			state.Format("HawkerOwnerPartnerState%01d1", i+1);
			SetUIEnable(state, false);

			itemNameImage.Format("HawkerOwnerNameSelected%01d", i+1);
			SetUIEnable(itemNameImage, false);

			sellImage.Format("HawkerOwnerPartnerSell%01d", i+1);
			SetUIEnable(sellImage,false);

			gGlobal.SetUIStaticText(componentName, _T(""));
		}
	}
}

void CMenuHawkerProcess::UpdateOwnerPetNameList()
{

	if (gGlobal.g_PetLst)
	{
		String Text;
		CPet* pet = NULL;
		{
			for( Index i = C_INDEX(0); i < MAX_PET_PARTNER_ONE_PAGE; ++i )
			{
				pet = gGlobal.g_PetLst->GetPetByIndex(i + m_iStartIndex);
				StringA componentName;
				StringA itemNameImage;
				StringA state;
				StringA sellPetImage;
				componentName.Format("HawkerOwnerNameListLine%01d", i+1 );
				itemNameImage.Format("HawkerOwnerNameImage%01d", i+1 );
				sellPetImage.Format("HawkerOwnerPartnerSell%01d", i+1 );
				if (pet)
				{
					Text.Format(pet->GetName().c_str());
					SetUIEnable(itemNameImage, true);

					if (pet->GetState() == 1)
					{
						state.Format("HawkerOwnerPartnerState%01d1", i+1);
						SetUIEnable(state, false);
						state.Format("HawkerOwnerPartnerState%01d2", i+1);
						SetUIEnable(state, true);
					}
					else
					{
						state.Format("HawkerOwnerPartnerState%01d2", i+1);
						SetUIEnable(state, false);
						state.Format("HawkerOwnerPartnerState%01d1", i+1);
						SetUIEnable(state, true);
					}

					StringA lockname;
					lockname.Format("HawkerPetPartnerLock%01d", i+1);
					SetUIEnable(lockname, pet->GetLock());

				}
				else
				{
					Text.Format(_T(""));
					SetUIEnable(itemNameImage, false);
					state.Format("HawkerOwnerPartnerState%01d2", i+1);
					SetUIEnable(state, false);
					state.Format("HawkerOwnerPartnerState%01d1", i+1);
					SetUIEnable(state, false);
					SetUIEnable(sellPetImage,false);
					StringA lockname;
					lockname.Format("HawkerPetPartnerLock%01d", i+1);
					SetUIEnable(lockname, false);	

				}

				itemNameImage.Format("HawkerOwnerNameSelected%01d", i+1);
				SetUIEnable(itemNameImage, false);
				UIStaticText* petNameText = gGlobal.pUIManager->FindUIStaticText(componentName.c_str());
				if(pet && petNameText)
				{
					if(pet->IsGiven())
					{
						Color32 petNameTextColor = D3DCOLOR_ARGB(255, 128, 128, 128);
						petNameText->SetColor(petNameTextColor);
						SetUIEnable(state,false);
						SetUIEnable(sellPetImage,true);
					}
					else
					{
						petNameText->SetColor(TextColor[ColorIndex_White]);
						SetUIEnable(sellPetImage,false);
						SetUIEnable(state,true);
					}
				}
				gGlobal.SetUIStaticText(componentName, Text);
			}

			if ( m_iSelectedPetIndex < (MAX_PET_PARTNER_ONE_PAGE + m_iStartIndex) && m_iSelectedPetIndex >= m_iStartIndex )
			{
				StringA itemNameSelected;
				itemNameSelected.Format("HawkerOwnerNameSelected%01d", m_iSelectedPetIndex - m_iStartIndex + 1);
				SetUIEnable(itemNameSelected, true);

				itemNameSelected.Format("HawkerOwnerNameImage%01d", m_iSelectedPetIndex - m_iStartIndex + 1);
				SetUIEnable(itemNameSelected, false);

				//itemNameSelected.Format("HawkerOwnerPartnerSell%01d", m_iSelectedPetIndex - m_iStartIndex + 1);
				//SetUIEnable(itemNameSelected,false);
			}
		}

		/*Text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CARRYING_NUMBER"), gGlobal.g_PetLst->GetCount(),
		gGlobal.g_pCharacter->GetCharNetObject()->GetMaxPet());
		gGlobal.SetUIStaticText("ItemNameListLine6", Text);*/

	}
}

void CMenuHawkerProcess::UpdateOwnerPet(int petIndex)
{
	m_iMenuIndex = ITEM_PET_MENU;

	if (gGlobal.g_PetLst)
	{
		String Text;

		if (petIndex >= 0)
			m_iSelectedPetIndex = petIndex + m_iStartIndex;

		UpdateOwnerPetNameList();

		CPet* pet = gGlobal.g_PetLst->GetPetByIndex(m_iSelectedPetIndex);

		UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("HawkerOwnerModel");

		if (pAniIcon)
		{
			if (pet)
			{
				const PetData * pPetData = gGlobal.m_resource->GetPetData(pet->GetPet_id());

				if (pPetData)
				{
					CAnimatedEntity temp;
					temp.InitEntPicInfo();
					temp.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
					String modelName = String(pPetData->pet_model);
					temp.LoadEntityFile(modelName.c_str(), gGlobal.pD3DDevice);

					String colorFileName = modelName.substr(0, modelName.length() - 5);
					char abc = pet->GetColor() + 97;
					colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
					temp.ChangeColorP(modelName.c_str(), colorFileName.c_str());

					pAniIcon->SetAnimationIndex(temp.GetAnimationIndex());	
					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(&temp);
					pAniIcon->SetRotation(SOUTHEAST);
					gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
				}

				Text.Format(_T("%d"), pet->GetLevel());

				if (pPetData->pet_type == 0)
					Text.Format(_T("%d/100"), pet->GetHappiness());
				else
					Text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
				gGlobal.SetUIStaticText("HawkerOwnerPetHappyText", Text);
				if(pet->GetExpireDate() == 0)
				{
					Text.Format(_T("%d/%d"), pet->GetVP(),pet->GetMaxVP());
					gGlobal.SetUILinerImage("HawkerOwnerPetVPBar", pet->GetMaxVP(), pet->GetVP());	
				}
				else
				{
					Text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
					gGlobal.SetUILinerImage("HawkerOwnerPetVPBar", 0,1);
				}
				gGlobal.SetUIStaticText("HawkerOwnerPetVPText", Text);

				if(pPetData->pet_type == 0)
					gGlobal.SetUILinerImage("HawkerOwnerPetHappyBar", 100, pet->GetHappiness());
				else
					gGlobal.SetUILinerImage("HawkerOwnerPetHappyBar", 0,1);

			}
			else
			{
				gGlobal.SetUILinerImage("HawkerOwnerPetHappyBar", 0, 1 );
				gGlobal.SetUILinerImage("HawkerOwnerPetVPBar", 0, 1 );

				gGlobal.SetUIStaticText("HawkerOwnerPetLVText", _T(""), true);
				gGlobal.SetUIStaticText("HawkerOwnerPetHappyText", _T(""));
				gGlobal.SetUIStaticText("HawkerOwnerPetVPText", _T(""));

				UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("HawkerOwnerModel");

				if (pAniIcon)
				{
					pAniIcon->ClearAniData();
					gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
				}
			}
		}
	}
	else
	{
		gGlobal.SetUILinerImage("HawkerOwnerPetHappyBar", 0, 1 );
		gGlobal.SetUILinerImage("HawkerOwnerPetVPBar", 0, 1 );

		gGlobal.SetUIStaticText("HawkerOwnerPetLVText", _T(""), true);
		gGlobal.SetUIStaticText("HawkerOwnerPetHappyText", _T(""));
		gGlobal.SetUIStaticText("HawkerOwnerPetVPText", _T(""));

		UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("HawkerOwnerModel");

		if (pAniIcon)
		{
			pAniIcon->ClearAniData();
			gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
		}

		for( Index i = C_INDEX(0); i < 6; ++i )
		{
			StringA componentName;
			StringA itemNameImage;
			StringA state;
			StringA sellImage;

			componentName.Format("HawkerOwnerNameListLine%01d", i+1 );
			itemNameImage.Format("HawkerOwnerNameImage%01d", i+1 );

			SetUIEnable(itemNameImage, false);
			state.Format("HawkerOwnerPartnerState%01d2", i+1);
			SetUIEnable(state, false);
			state.Format("HawkerOwnerPartnerState%01d1", i+1);
			SetUIEnable(state, false);

			itemNameImage.Format("HawkerOwnerNameSelected%01d", i+1);
			SetUIEnable(itemNameImage, false);

			sellImage.Format("HawkerOwnerPartnerSell%01d",i+1);
			SetUIEnable(sellImage,false);

			gGlobal.SetUIStaticText(componentName, _T(""));
		}
	}

}

void CMenuHawkerProcess::UpdateOwnerPartnerNameList()
{
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerGroup->GetCount() > 0)
	{
		String Text;
		CPartner* partner = NULL;
		for( Index i = C_INDEX(0); i < MAX_PET_PARTNER_ONE_PAGE; ++i )
		{
			partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(i + m_iStartIndex);
			StringA componentName;
			StringA itemNameImage;
			StringA state;
			StringA sellImage;
			componentName.Format("HawkerOwnerNameListLine%01d", i+1 );
			itemNameImage.Format("HawkerOwnerNameImage%01d", i+1 );
			sellImage.Format("HawkerOwnerPartnerSell%01d", i+1 );
			if (partner)
			{
				Text.Format(partner->GetRaname().c_str());
				SetUIEnable(itemNameImage, true);

				if (partner->GetState() == 1)
				{
					state.Format("HawkerOwnerPartnerState%01d1", i+1);
					SetUIEnable(state, false);
					state.Format("HawkerOwnerPartnerState%01d2", i+1);
					SetUIEnable(state, true);
				}
				else
				{
					state.Format("HawkerOwnerPartnerState%01d2", i+1);
					SetUIEnable(state, false);
					state.Format("HawkerOwnerPartnerState%01d1", i+1);
					SetUIEnable(state, true);
				}

				StringA lockname;
				lockname.Format("HawkerPetPartnerLock%01d", i+1);
				SetUIEnable(lockname, partner->GetLock());	

			}
			else
			{
				Text.Format(_T(""));
				SetUIEnable(itemNameImage, false);
				state.Format("HawkerOwnerPartnerState%01d2", i+1);
				SetUIEnable(state, false);
				state.Format("HawkerOwnerPartnerState%01d1", i+1);
				SetUIEnable(state, false);
				StringA lockname;
				lockname.Format("HawkerPetPartnerLock%01d", i+1);
				SetUIEnable(lockname, false);
				SetUIEnable(sellImage,false);
			}

			itemNameImage.Format("HawkerOwnerNameSelected%01d", i+1);
			SetUIEnable(itemNameImage, false);

			UIStaticText* partnerNameText = gGlobal.pUIManager->FindUIStaticText(componentName.c_str());
			if(partner && partnerNameText)
			{
				if(partner->IsGiven())
				{
					Color32 partnerNameTextColor = D3DCOLOR_ARGB(255, 128, 128, 128);
					partnerNameText->SetColor(partnerNameTextColor);
					SetUIEnable(state,false);
					SetUIEnable(sellImage,true);
				}
				else
				{
					partnerNameText->SetColor(TextColor[ColorIndex_White]);
					SetUIEnable(sellImage,false);
					SetUIEnable(state,true);
				}
			}
			gGlobal.SetUIStaticText(componentName, Text);
		}

		if ( m_iSelectedPartnerIndex < (MAX_PET_PARTNER_ONE_PAGE + m_iStartIndex) && m_iSelectedPartnerIndex >= m_iStartIndex )
		{
			StringA itemNameSelected;
			itemNameSelected.Format("HawkerOwnerNameSelected%01d", m_iSelectedPartnerIndex - m_iStartIndex + 1);
			SetUIEnable(itemNameSelected, true);

			itemNameSelected.Format("HawkerOwnerNameImage%01d", m_iSelectedPartnerIndex - m_iStartIndex + 1);
			SetUIEnable(itemNameSelected, false);

			//	itemNameSelected.Format("HawkerOwnerPartnerSell%01d", m_iSelectedPartnerIndex - m_iStartIndex + 1);
			//	SetUIEnable(itemNameSelected,false);
		}
	}
}

void CMenuHawkerProcess::UpdateCharWeaponEnt(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor)
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		UInt sexClass = gGlobal.g_pCharacter->GetCharNetObject()->GetSexClass();
		const ItemData* pItemData = gGlobal.m_resource->GetItemData(eq_weapon);
		if( pItemData )
		{
			if( sexClass == pItemData->reqSexClass1 )
				ChangeWeapon(String(pItemData->eq_Outlook1));
			else if( sexClass == pItemData->reqSexClass2 )
				ChangeWeapon(String(pItemData->eq_Outlook2));
			else
				ChangeWeapon(String());
		}
		else
			ChangeWeapon(String());
	}
}

void CMenuHawkerProcess::ChangeWeapon(String name)
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("HawkerOwnerModel");

		if (pAniIcon)
		{
			CAnimatedEntity* temp = pAniIcon->GetAniEnt();
			if (temp)
			{
				temp->RemovePart(EQUIP_WEAPON);
				temp->LoadEntityFile(name.c_str(), gGlobal.pD3DDevice, EQUIP_WEAPON);
				gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
			}
		}
	}
}

void CMenuHawkerProcess::UpdateHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor)
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		const ItemData* pItemData = gGlobal.m_resource->GetItemData(eq_headBand);
		UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("HawkerOwnerModel");
		UInt8 sexClass = gGlobal.g_pCharacter->GetCharNetObject()->GetSexClass();
		if (pAniIcon)
		{
			CAnimatedEntity* temp = pAniIcon->GetAniEnt();
			UInt head = gGlobal.g_pCharacter->GetCharNetObject()->GetOrg_hairStyle1();
			UInt headColor = gGlobal.g_pCharacter->GetCharNetObject()->GetOrg_hairColor();

			if (gGlobal.g_pCharacter->GetCharNetObject()->GetHairStyle1() > 0)
			{
				head = gGlobal.g_pCharacter->GetCharNetObject()->GetHairStyle1();					
				headColor = gGlobal.g_pCharacter->GetCharNetObject()->GetHairColor();
			}

			if (head == 0)
				head = 1;

			if (headColor == 0)
				headColor = 1;

			String hair;
			String hairColor;

			if( pItemData && temp)
			{
				if (pItemData->eqType == EQTYPE_HAT_WITH_HAIR)
				{					
					char abc = 96 + headColor;

					hair.Format(_T("j%05d_t%03d.ent"), sexClass,  head);
					hairColor.Format(_T("j%05d_t%03d%c.tga"), sexClass,  head, abc);

					temp->RemovePart(EQUIP_HAIR);
					temp->LoadEntityFile(hair.c_str(), gGlobal.pD3DDevice, EQUIP_HAIR);
					temp->ChangeColorP(hair.c_str(), hairColor.c_str());
				}
				else if (pItemData->eqType == EQTYPE_HAT_WITHOUT_HAIR)
					temp->RemovePart(EQUIP_HAIR);

				String colorFile;
				switch(eq_headColor)
				{
				case 1: colorFile = String(pItemData->eq_colorPalette1); break;
				case 2: colorFile = String(pItemData->eq_colorPalette2); break;
				case 3: colorFile = String(pItemData->eq_colorPalette3); break;
				case 4: colorFile = String(pItemData->eq_colorPalette4); break;
				case 5: colorFile = String(pItemData->eq_colorPalette5); break;
				}

				if( sexClass == pItemData->reqSexClass1 )
					ChangeHeadAccessory(String(pItemData->eq_Outlook1), String());
				else if( sexClass == pItemData->reqSexClass2 )
					ChangeHeadAccessory(String(pItemData->eq_Outlook2), String());
				else
				{
					ChangeHeadAccessory(String(), String());
					char abc = 96 + headColor;

					hair.Format(_T("j%05d_t%03d.ent"), sexClass,  head);
					hairColor.Format(_T("j%05d_t%03d%c.tga"), sexClass,  head, abc);

					temp->RemovePart(EQUIP_HAIR);
					temp->LoadEntityFile(hair.c_str(), gGlobal.pD3DDevice, EQUIP_HAIR);
					temp->ChangeColorP(hair.c_str(), hairColor.c_str());
				}
			}
			else
			{
				ChangeHeadAccessory(String(), String());					

				char abc = 96 + headColor;

				hair.Format(_T("j%05d_t%03d.ent"), sexClass,  head);
				hairColor.Format(_T("j%05d_t%03d%c.tga"), sexClass,  head, abc);

				temp->RemovePart(EQUIP_HAIR);
				temp->LoadEntityFile(hair.c_str(), gGlobal.pD3DDevice, EQUIP_HAIR);
				temp->ChangeColorP(hair.c_str(), hairColor.c_str());
			}
		}
	}
}

void CMenuHawkerProcess::ChangeHeadAccessory(String name, String colorFile)
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("HawkerOwnerModel");

		if (pAniIcon)
		{
			CAnimatedEntity* temp = pAniIcon->GetAniEnt();
			if (temp)
			{
				temp->RemovePart(EQUIP_HEAD_ACCESSORY);
				temp->LoadEntityFile(name.c_str(), gGlobal.pD3DDevice, EQUIP_HEAD_ACCESSORY);
				if (!colorFile.IsEmpty())
					temp->ChangeColorP(name.c_str(), colorFile.c_str());
				gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
			}
		}
	}
}

void CMenuHawkerProcess::UpdateBody(const UInt32 &eq_cloth)
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		ChangeBody(eq_cloth);
	}
}

void CMenuHawkerProcess::ChangeBody(UInt16 eqCloth)
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("HawkerOwnerModel");
		CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();

		if (pAniIcon && pchar)
		{
			UInt8 sexClass = gGlobal.g_pCharacter->GetCharNetObject()->GetSexClass();
			const ItemData* pItemData = gGlobal.m_resource->GetItemData(eqCloth);
			String name;
			if( pItemData )
			{
				if( sexClass==pItemData->reqSexClass1 )
					name = String(pItemData->eq_Outlook1);
				else if( sexClass==pItemData->reqSexClass2 )
					name = String(pItemData->eq_Outlook2);
			}
			UInt16 cloth = pchar->GetOrg_eq_cloth();
			UInt16 clothColor = pchar->GetOrg_eq_clothColor();
			if (pchar->GetEq_cloth() > 0)
			{
				cloth = pchar->GetEq_cloth();
				clothColor = pchar->GetEq_clothColor();
			}

			CAnimatedEntity* temp = pAniIcon->GetAniEnt();
			if (temp)
			{
				temp->RemovePart(EQUIP_BODY);
				if( name.IsEmpty() )
				{					
					if (clothColor == 0)
						clothColor = 1;

					if (cloth == 0)
						cloth = 1;

					char buf[256];
					char buf2[256];
					char abc = 96 + clothColor;

					sprintf(buf, "b%05d_t%03d.ent", sexClass, cloth);
					sprintf(buf2, "b%05d_t%03d%c.tga", sexClass, cloth, abc);

					temp->LoadEntityFile(String(buf).c_str(), gGlobal.pD3DDevice, EQUIP_BODY);
					temp->ChangeColorP(String(buf).c_str(), String(buf2).c_str());
				}
				else
				{
					String colorFile;
					if (pItemData)
					{
						switch (clothColor)
						{
						case 1: colorFile = String(pItemData->eq_colorPalette1);	break;
						case 2: colorFile = String(pItemData->eq_colorPalette2);	break;
						case 3: colorFile = String(pItemData->eq_colorPalette3);	break;
						case 4: colorFile = String(pItemData->eq_colorPalette4);	break;
						case 5: colorFile = String(pItemData->eq_colorPalette5);	break;
						}
					}
					temp->LoadEntityFile(name.c_str(), gGlobal.pD3DDevice, EQUIP_BODY);
					if (!colorFile.IsEmpty())
						temp->ChangeColorP(name.c_str(), colorFile.c_str());
				}
				gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
			}
		}
	}
}

void CMenuHawkerProcess::SetItemHawk(UInt8 itemIndex, UInt8 stack, UInt32 price)
{
	CItem* item = gGlobal.m_pBag->GetItem(itemIndex);
	if (item)
	{
		HawkerItemInfo hawkerItem;
		hawkerItem.item_id = item->GetItem_id();
		hawkerItem.item_index = itemIndex;
		hawkerItem.item_price = price;
		hawkerItem.item_stack = stack;
		hawkerItem.item_uid = item->GetItem_uid();
		m_hawkerItems.push_back(hawkerItem);
		m_InithawkerItems = m_hawkerItems;
		if (m_hawkerItems.size() > HAWK_SLOT_PER_PAGE)
			UpdateHawkItem(1);
		else
			UpdateHawkItem(0);

		item->SetGivenStack(stack);
		gGlobal.g_pItemProcess->UpdateItemSlot(itemIndex, true);
		UpdateOwnerItemSlot(itemIndex, true);
		if (m_bStartHawk)
		{
			if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
			{
				//更新服务器记录摆摊信息
				gGlobal.g_pCharacter->GetCharNetObject()->AddStartHawkItem(gGlobal.pPeerProxy,gGlobal.g_context,hawkerItem,START_HAWK_ADD_ITEM);
			}
		}
	}
	/*
	if (m_bStartHawk)
	{
	//gGlobal.g_pCharacter->GetCharNetObject()->StartHawk(gGlobal.pPeerProxy, RPCContext(), _T(""), m_hawkerItems, m_hawkerPartners, m_hawkerPets);
	if(gGlobal.g_pCharacter)
	{
	if(gGlobal.g_pCharacter->GetCharNetObject())
	gGlobal.g_pCharacter->GetCharNetObject()->UpdateHawkCargo(gGlobal.pPeerProxy,gGlobal.g_context,m_hawkerItems, m_hawkerPartners, m_hawkerPets);
	else
	ShowHawkMessage(0);
	}
	else
	ShowHawkMessage(1);

	}*/

}

void CMenuHawkerProcess::SetPartnerHawk(UInt32 price)
{
	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(m_iSelectedPartnerIndex);
	if (partner)
	{
		HawkerItemInfo hawkerPartner;
		hawkerPartner.item_id = partner->GetMob_id();
		hawkerPartner.item_index = m_iSelectedPartnerIndex;
		hawkerPartner.item_price = price;
		hawkerPartner.item_stack = 1;
		hawkerPartner.item_uid = partner->GetPartner_id();
		m_hawkerPartners.push_back(hawkerPartner);
		m_InithawkerPartners = m_hawkerPartners;
		if (m_hawkerPartners.size() > HAWK_SLOT_PER_PAGE)
			UpdateHawkPartner(1);
		else
			UpdateHawkPartner(0);

		partner->SetGiven(true);
		if (m_bStartHawk)
		{
			if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
			{
				//更新服务器记录摆摊信息
				gGlobal.g_pCharacter->GetCharNetObject()->AddStartHawkItem(gGlobal.pPeerProxy,gGlobal.g_context,hawkerPartner,START_HAWK_ADD_PARTNER);
			}
		}
		UpdateOwnerPartnerNameList();
	}
}

void CMenuHawkerProcess::SetPetHawk( UInt32 price)
{
	CPet* pet = gGlobal.g_PetLst->GetPetByIndex(m_iSelectedPetIndex);
	if (pet)
	{
		HawkerItemInfo hawkerPet;
		hawkerPet.item_id = pet->GetPet_id();
		hawkerPet.item_index = m_iSelectedPetIndex;
		hawkerPet.item_price = price;
		hawkerPet.item_stack = 1;
		hawkerPet.item_uid = pet->GetPet_uid();
		m_hawkerPets.push_back(hawkerPet);
		m_InithawkerPets = m_hawkerPets;
		if (m_hawkerPets.size() > HAWK_SLOT_PER_PAGE)
			UpdateHawkPet(1);
		else
			UpdateHawkPet(0);

		pet->SetGiven(true);
		if (m_bStartHawk)
		{
			if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
			{
				//更新服务器记录摆摊信息
				gGlobal.g_pCharacter->GetCharNetObject()->AddStartHawkItem(gGlobal.pPeerProxy,gGlobal.g_context,hawkerPet,START_HAWK_ADD_PET);
			}
		}
		UpdateOwnerPetNameList();
	}
}

void CMenuHawkerProcess::UpdateHawkItem(UInt8 page)
{
	UIAniIcon* pAniIcon = NULL;
	UIStaticText* name = NULL;
	UIStaticText* stack = NULL;
	UIStaticText* price = NULL;
	UIStaticText* priceTitle = NULL;
	UIImage* bg = NULL;

	m_iHawkMenuIndex = HAWKER_ITEM;
	m_iHawkPageIndex = page;

	UIStaticText* pageNoText = gGlobal.pUIManager->FindUIStaticText("HawkerOwnerPageNo");
	if (pageNoText)
	{
		String pageNo;
		pageNo.Format(_T("%d"), page + 1);
		pageNoText->SetText(pageNo.c_str());
		pageNoText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pageNoText);
	}

	for (UInt8 i = 0; i < HAWK_SLOT_PER_PAGE; ++i)
	{
		UInt8 index = page * HAWK_SLOT_PER_PAGE + i;
		StringA iconName = "";
		iconName.Format("HawkerOwnerSell0%02dIcon", i );
		pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dName",i);
		name = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dPrice", i);
		price = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dStack",i);
		stack = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dPriceTitle", i);
		priceTitle = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dBg", i);
		bg = gGlobal.pUIManager->FindUIImage(iconName.c_str());

		if (pAniIcon && name && price && stack && priceTitle && bg)
		{
			bg->SetEnable(false);
			if (m_hawkerItems.size() > index)
			{

				CItem* item = gGlobal.m_pBag->GetItem(m_hawkerItems[index].item_index);
				if (item && item->m_data)
				{
					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
					pAniIcon->SetOffsetXY(bg->GetOffsetX(), bg->GetOffsetY());
					pAniIcon->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);

					name->SetText(String(item->m_data->name).c_str());
					name->Redraw();
					name->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(name);

					priceTitle->SetEnable(true);

					String text;
					text.Format(_T("%d%s"), m_hawkerItems[index].item_price,gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT2")));

					Color32 textColor = gGlobal.GetDisplayMoneyColor(m_hawkerItems[index].item_price);

					if (textColor != 0)
						price->SetColor(textColor);
					price->SetText(text.c_str());
					price->Redraw();
					price->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(price);

					text.Format(_T("%d"), m_hawkerItems[index].item_stack);
					stack->SetText(text.c_str());
					stack->Redraw();
					stack->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(stack);
				}
			}
			else
			{
				pAniIcon->SetEnable(false);
				name->SetEnable(false);
				price->SetEnable(false);
				stack->SetEnable(false);
				priceTitle->SetEnable(false);
			}
		}
	}
	//m_InithawkerItems = m_hawkerItems;
	//TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("m_InithawkerItems size is %d"), 
	//	m_InithawkerItems.size());
	//	TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("m_hawkerItems size is %d"), 
	//	m_hawkerItems.size());
}

void CMenuHawkerProcess::UpdateHawkPartner(UInt8 page)
{
	UIAniIcon* pAniIcon = NULL;
	UIStaticText* name = NULL;
	UIStaticText* stack = NULL;
	UIStaticText* price = NULL;
	UIStaticText* priceTitle = NULL;
	UIImage* bg = NULL;

	m_iHawkMenuIndex = HAWKER_PARTNER;
	m_iHawkPageIndex = page;

	UIStaticText* pageNoText = gGlobal.pUIManager->FindUIStaticText("HawkerOwnerPageNo");
	if (pageNoText)
	{
		String pageNo;
		pageNo.Format(_T("%d"), page + 1);
		pageNoText->SetText(pageNo.c_str());
		pageNoText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pageNoText);
	}
	for (Index i = C_INDEX(0); i < HAWK_SLOT_PER_PAGE; ++i)
	{
		UInt index = page * HAWK_SLOT_PER_PAGE + i;
		StringA iconName = "";
		iconName.Format("HawkerOwnerSell0%02dIcon", i );
		pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dName",i);
		name = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dPrice", i);
		price = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dStack",i);
		stack = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dPriceTitle", i);
		priceTitle = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dBg", i);
		bg = gGlobal.pUIManager->FindUIImage(iconName.c_str());

		if (pAniIcon && name && price && stack && priceTitle && bg)
		{
			if (m_hawkerPartners.size() > index)
			{
				CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(m_hawkerPartners[index].item_uid);
				if (partner && partner->m_data)
				{
					bg->SetEnable(true);

					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(String(partner->m_data->m_basicinfo->mob_faceS).c_str());
					pAniIcon->SetOffsetXY(bg->GetOffsetX() + 2, bg->GetOffsetY() + 2);
					pAniIcon->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);

					name->SetText(partner->GetRaname().c_str());
					name->Redraw();
					name->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(name);

					String text;
					text.Format(_T("%d%s"), m_hawkerPartners[index].item_price,gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT2")));

					priceTitle->SetEnable(true);

					Color32 textColor = gGlobal.GetDisplayMoneyColor(m_hawkerPartners[index].item_price);
					
					if (textColor != 0)
						price->SetColor(textColor);

					price->SetText(text.c_str());
					price->Redraw();
					price->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(price);

					stack->SetEnable(false);
					gGlobal.pUIManager->AddDirtyUIItem(stack);

				}
				else
				{
					pAniIcon->SetEnable(false);
					name->SetEnable(false);
					price->SetEnable(false);
					stack->SetEnable(false);
					priceTitle->SetEnable(false);
					bg->SetEnable(false);
				}
			}
			else
			{
				pAniIcon->SetEnable(false);
				name->SetEnable(false);
				price->SetEnable(false);
				stack->SetEnable(false);
				priceTitle->SetEnable(false);
				bg->SetEnable(false);
			}
		}
	}
	m_InithawkerPartners = m_hawkerPartners;
}

void CMenuHawkerProcess::UpdateHawkPet(UInt8 page)
{
	UIAniIcon* pAniIcon = NULL;
	UIStaticText* name = NULL;
	UIStaticText* stack = NULL;
	UIStaticText* price = NULL;
	UIStaticText* priceTitle = NULL;
	UIImage* bg = NULL;

	m_iHawkMenuIndex = HAWKER_PET;
	m_iHawkPageIndex = page;
	UIStaticText* pageNoText = gGlobal.pUIManager->FindUIStaticText("HawkerOwnerPageNo");
	if (pageNoText)
	{
		String pageNo;
		pageNo.Format(_T("%d"), page + 1);
		pageNoText->SetText(pageNo.c_str());
		pageNoText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pageNoText);
	}
	for (Index i = C_INDEX(0); i < HAWK_SLOT_PER_PAGE; ++i)
	{
		UInt index = page * HAWK_SLOT_PER_PAGE + i;
		StringA iconName = "";
		iconName.Format("HawkerOwnerSell0%02dIcon", i );
		pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dName",i);
		name = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dPrice", i);
		price = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dStack",i);
		stack = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dPriceTitle", i);
		priceTitle = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerSell0%02dBg", i);
		bg = gGlobal.pUIManager->FindUIImage(iconName.c_str());

		if (pAniIcon && name && price && stack && priceTitle && bg)
		{
			if (m_hawkerPets.size() > index)
			{
				CPet* pet = gGlobal.g_PetLst->GetPet(m_hawkerPets[index].item_uid);
				if (pet)
				{
					const PetData * pPetData = gGlobal.m_resource->GetPetData(pet->GetPet_id());
					if (pPetData)
					{
						bg->SetEnable(true);

						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(String(pPetData->pet_faceS).c_str());
						pAniIcon->SetOffsetXY(bg->GetOffsetX(), bg->GetOffsetY());
						pAniIcon->SetEnable(true);
						gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);

						name->SetText(pet->GetName().c_str());
						name->Redraw();
						name->SetEnable(true);
						gGlobal.pUIManager->AddDirtyUIItem(name);

						String text;
						text.Format(_T("%d%s"), m_hawkerPets[index].item_price,gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT2")));

						priceTitle->SetEnable(true);

						Color32 textColor = 0;

						if ( 0 <= m_hawkerPets[index].item_price && m_hawkerPets[index].item_price < 10000)
							textColor = TextColor[ColorIndex_White];
						else if ( 10000 <= m_hawkerPets[index].item_price && m_hawkerPets[index].item_price < 100000)
							textColor = TextColor[ColorIndex_Green];
						else if ( 100000 <= m_hawkerPets[index].item_price && m_hawkerPets[index].item_price < 1000000)
							textColor = TextColor[ColorIndex_Blue];
						else if ( 1000000 <= m_hawkerPets[index].item_price && m_hawkerPets[index].item_price < 10000000)
							textColor = TextColor[ColorIndex_Violet];
						else 
							textColor = ItemTextColor[ItemColorIndex_Golden];

						if (textColor != 0)
							price->SetColor(textColor);

						price->SetText(text.c_str());
						price->Redraw();
						price->SetEnable(true);
						gGlobal.pUIManager->AddDirtyUIItem(price);

						stack->SetEnable(false);
						gGlobal.pUIManager->AddDirtyUIItem(stack);
					}
				}
			}
			else
			{
				pAniIcon->SetEnable(false);
				name->SetEnable(false);
				price->SetEnable(false);
				stack->SetEnable(false);
				priceTitle->SetEnable(false);
				bg->SetEnable(false);
			}
		}
	}
	m_InithawkerPets = m_hawkerPets;
}

void CMenuHawkerProcess::UpdateHawkExchangeRecord(UInt8 page)
{
	m_iHawkMenuIndex = HAWKER_EXCHANGE_RECORD;
	m_iHawkPageIndex = page;
	UIStaticText* pageNoText = gGlobal.pUIManager->FindUIStaticText("HawkerOwnerPageNo");
	if (pageNoText)
	{
		String pageNo;
		pageNo.Format(_T("%d"), page + 1);
		pageNoText->SetText(pageNo.c_str());
		pageNoText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pageNoText);
	}

	StringA iconName;
	UIStaticText* dateText = NULL;
	UIStaticText* historyText = NULL;

	for (Index i = C_INDEX(0); i < MAX_RECORD_PER_PAGE; ++i)
	{
		iconName.Format("HawkerOwnerRecordDate%03d", i);
		dateText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("HawkerOwnerRecord%03d", i);
		historyText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		UInt index = i + MAX_RECORD_PER_PAGE * page;

		if (dateText && historyText)
		{
			if (index < m_hawkerHistory.size())
			{
				time_t historyDate = (time_t)(m_hawkerHistory[index].data.historyDate);
				struct tm * buyTime = localtime(&historyDate);

				String date;
				date.Format(_T("%d-%02d-%02d %02d:%02d"), (buyTime->tm_year + 1900), (buyTime->tm_mon + 1), buyTime->tm_mday, buyTime->tm_hour, buyTime->tm_min);

				String history;
				if(m_hawkerHistory[index].data.itemType == HAWKER_ITEM)
				{
					history.Format(_T("%d%s%s"), m_hawkerHistory[index].data.stack,gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_ITEM")), m_hawkerHistory[index].itemName.c_str());
					history.Format(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_BUY_SUCCESS_FOR_OWNER")), history.c_str(), m_hawkerHistory[index].data.totalPrice, m_hawkerHistory[index].data.tax);
				}
				else
				{
					history.Format(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_BUY_SUCCESS_FOR_OWNER")), m_hawkerHistory[index].itemName.c_str(), m_hawkerHistory[index].data.totalPrice, m_hawkerHistory[index].data.tax);
				}

				dateText->SetText(date.c_str());
				dateText->Redraw();

				historyText->SetText(history.c_str());
				historyText->Redraw();

				dateText->SetEnable(true);
				historyText->SetEnable(true);
			}
			else
			{
				dateText->SetEnable(false);
				historyText->SetEnable(false);
			}
		}
	}

}

void CMenuHawkerProcess::SelectHawkItem()
{
	if (m_iHawkMenuIndex != HAWKER_ITEM)
	{
		//InitItemPartnerPet();	
		UpdateHawkItem(0);
	}
}

void CMenuHawkerProcess::SelectHawkPartner()
{
	if (m_iHawkMenuIndex != HAWKER_PARTNER)
		UpdateHawkPartner(0);
}

void CMenuHawkerProcess::SelectHawkPet()
{
	if (m_iHawkMenuIndex != HAWKER_PET)
		UpdateHawkPet(0);
}

void CMenuHawkerProcess::SelectHawkExchangeRecord()
{
	if (m_iHawkMenuIndex != HAWKER_EXCHANGE_RECORD)
		UpdateHawkExchangeRecord(0);
}

void CMenuHawkerProcess::TurnPage(bool next)
{
	if (next)
	{
		if (m_iHawkMenuIndex == HAWKER_ITEM && m_hawkerItems.size() > HAWK_SLOT_PER_PAGE)
			UpdateHawkItem(1);
		else if (m_iHawkMenuIndex == HAWKER_PARTNER && m_hawkerPartners.size() > HAWK_SLOT_PER_PAGE)
			UpdateHawkPartner(1);
		else if (m_iHawkMenuIndex == HAWKER_PET && m_hawkerPets.size() > HAWK_SLOT_PER_PAGE)
			UpdateHawkPet(1);
		else if (m_iHawkMenuIndex == HAWKER_EXCHANGE_RECORD )
		{
			UInt count = (UInt)m_hawkerHistory.size() / (MAX_RECORD_PER_PAGE + 1);
			if (m_iHawkPageIndex < count)
				UpdateHawkExchangeRecord(++m_iHawkPageIndex);
		}
	}
	else
	{
		if (m_iHawkMenuIndex == HAWKER_ITEM)
			UpdateHawkItem(0);
		else if (m_iHawkMenuIndex == HAWKER_PARTNER)
			UpdateHawkPartner(0);
		else if (m_iHawkMenuIndex == HAWKER_PET)
			UpdateHawkPet(0);
		else if (m_iHawkMenuIndex == HAWKER_EXCHANGE_RECORD )
		{
			if (m_iHawkPageIndex > 0)
				UpdateHawkExchangeRecord(--m_iHawkPageIndex);
		}
	}
}

void CMenuHawkerProcess::SelectPartnerOrPet(UInt8 index)
{
	if (index < 6)
	{
		if (m_iMenuIndex == ITEM_PARTNER_MENU)
			UpdateOwnerPartner(index);
		else if (m_iMenuIndex == ITEM_PET_MENU)
			UpdateOwnerPet(index);
	}
}

bool CMenuHawkerProcess::NotForSellPartner(bool showMsg)
{
	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(m_iSelectedPartnerIndex);
	if (partner)
	{	
		if ((partner->GetSoulboundState() == SOULBOUND_ALREADY_BOUND)||(partner->GetLock()))
		{
			if (showMsg)
				ShowHawkMessage(HAWK_CANT_HAWK);
			return true;
		}	
		else if (partner->IsGiven() || (partner->GetState() == CPartner::STATE_FIGHT))
		{
			if (showMsg)
				ShowHawkMessage(HAWK_CANT_FOR_SELL);
			return true;
		}
		return false;
	}
	return true;
}

bool CMenuHawkerProcess::NotForSellPet(bool showMsg)
{
	CPet* pet = gGlobal.g_PetLst->GetPetByIndex(m_iSelectedPetIndex);
	if (pet)
	{
		if ((pet->GetSoulbound() == SOULBOUND_ALREADY_BOUND)||(pet->GetLock()))
		{
			if (showMsg)
				ShowHawkMessage(HAWK_CANT_HAWK);
			return true;
		}
		else if (pet->IsGiven() || pet->GetState() == PET_STATE_TYPE_USE)
		{
			if (showMsg)
				ShowHawkMessage(HAWK_CANT_FOR_SELL);
			return true;
		}
		return false;
	}
	return true;

}

void CMenuHawkerProcess::ResetHawkerOwner()
{
	if (!m_bStartHawk)
		ResetItemPartnerPet();
	m_bOpenHawk = false;
	m_iHawkMenuIndex = HAWKER_CLOSE;

	if (gGlobal.g_pMyHawkGroup == NULL)
	{
		/*DeleteNetGroup(gGlobal.g_pHawkerHistory, BOOLEAN_TRUE);
		gGlobal.g_pHawkerHistory = NULL;*/
		m_hawkerHistory.clear();
		if (gGlobal.pPeerProxy)
			gGlobal.g_pCharacter->GetCharNetObject()->ClearHawkerHistory(gGlobal.pPeerProxy, RPCContext());
	}

}

void CMenuHawkerProcess::ResetItemPartnerPet()
{
	for (UInt8 i = 0; i < m_hawkerItems.size(); ++i)
	{
		if (m_hawkerItems[i].item_id != 0)
		{
			CItem* item = gGlobal.m_pBag->GetItem(m_hawkerItems[i].item_index);
			if (item)
			{
				item->SetGivenStack(0);
				int oldItembag = m_hawkerItems[i].item_index / BAG_ITEM_COUNT;

				if (oldItembag == gGlobal.g_pItemProcess->GetBagPage())
				{
					int oldIndex = m_hawkerItems[i].item_index % BAG_ITEM_COUNT;
					gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
				}
			}
		}
	}

	for (UInt8 i = 0; i < m_hawkerPartners.size(); ++i)
	{
		if (m_hawkerPartners[i].item_uid != 0)
		{
			CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(m_hawkerPartners[i].item_uid);
			if (partner)
				partner->SetGiven(false);
		}
	}

	for (UInt8 i = 0; i < m_hawkerPets.size(); ++i)
	{
		if (m_hawkerPets[i].item_uid != 0)
		{
			CPet* pet = gGlobal.g_PetLst->GetPet(m_hawkerPets[i].item_uid);
			if (pet)
				pet->SetGiven(false);
		}
	}

	m_hawkerItems.clear();
	m_hawkerPartners.clear();
	m_hawkerPets.clear();
}

void CMenuHawkerProcess::OpenHawkerOwnerMenu()
{
	gGlobal.g_pCharacter->Stop();
	if (!m_bOpenHawk)
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenHawkerOwner", String());
		m_bOpenHawk = true;
		UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("HawkerOwnerNameBox");
		if (pEditBox)
		{
			if (gGlobal.g_pMyHawkGroup && gGlobal.g_pMyHawkGroup->GetHawkName().length() > 0)
			{
				pEditBox->SetText(gGlobal.g_pMyHawkGroup->GetHawkName().c_str());
				pEditBox->Redraw();
				gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
			}
			else
			{
				pEditBox->SetText(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_DEFAULT_NAME")));
				pEditBox->Redraw();
				gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
			}
		}
		UpdateHawkerTime();
	}
}

void CMenuHawkerProcess::DragPartner()
{
	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(m_iSelectedPartnerIndex);
	if (partner && partner->m_data)
	{
		MobDB_Client * mobTemplate = partner->m_data;

		UIAniIcon* partnerFace = gGlobal.pUIManager->FindUIAniIcon("HawkerOwnerMovingIcon");
		if (partnerFace && mobTemplate)
		{
			partnerFace->SetEnable(true);

			String pFace = String(mobTemplate->m_basicinfo->mob_faceS);

			partnerFace->ClearAniData();
			partnerFace->SetAnimationData(pFace.c_str());
			gGlobal.pUIManager->AddDirtyUIItem(partnerFace);
		}
	}
}

void CMenuHawkerProcess::DragPet()
{
	CPet* pet = gGlobal.g_PetLst->GetPetByIndex(m_iSelectedPetIndex);
	if (pet)
	{
		const PetData* pPetData = gGlobal.m_resource->GetPetData(pet->GetPet_id());
		UIAniIcon* petFace = gGlobal.pUIManager->FindUIAniIcon("HawkerOwnerMovingIcon");
		if (pPetData && petFace)
		{
			petFace->SetEnable(true);

			String pFace = String(pPetData->pet_faceS);

			petFace->ClearAniData();
			petFace->SetAnimationData(pFace.c_str());
			gGlobal.pUIManager->AddDirtyUIItem(petFace);
		}
	}
}

void CMenuHawkerProcess::ResetSellItem(UInt8 index)
{
	if (m_bStartHawk)
	{
		return;
	}

	UInt8 realIndex = index + HAWK_SLOT_PER_PAGE * m_iHawkPageIndex;
	if (m_iHawkMenuIndex == HAWKER_ITEM && m_hawkerItems.size() > realIndex)
	{
		CItem* item = gGlobal.m_pBag->GetItem(m_hawkerItems[realIndex].item_index);
		if (item)
		{
			item->SetGivenStack(0);
			int oldItembag = m_hawkerItems[realIndex].item_index / BAG_ITEM_COUNT;

			if (oldItembag == gGlobal.g_pItemProcess->GetBagPage())
			{
				int oldIndex = m_hawkerItems[realIndex].item_index % BAG_ITEM_COUNT;
				gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
			}

			if (oldItembag == m_iBagIndex)
			{
				int oldIndex = m_hawkerItems[realIndex].item_index % BAG_ITEM_COUNT;
				UpdateOwnerItemSlot(oldIndex, true);
			}
			m_hawkerItems.erase(remove(m_hawkerItems.begin(), m_hawkerItems.end(), m_hawkerItems.at(realIndex)));

			UpdateHawkItem(m_iHawkPageIndex);
			m_InithawkerItems = m_hawkerItems;
		}
	}
	else if (m_iHawkMenuIndex == HAWKER_PARTNER && m_hawkerPartners.size() > realIndex)
	{
		CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(m_hawkerPartners[realIndex].item_uid);
		if (partner)
		{
			partner->SetGiven(false);
			m_hawkerPartners.erase(remove(m_hawkerPartners.begin(), m_hawkerPartners.end(), m_hawkerPartners.at(realIndex)));
			UpdateHawkPartner(m_iHawkPageIndex);
			m_InithawkerPartners = m_hawkerPartners;
		}
	}
	else if (m_iHawkMenuIndex == HAWKER_PET && m_hawkerPets.size() > realIndex)
	{
		CPet* pet = gGlobal.g_PetLst->GetPet(m_hawkerPets[realIndex].item_uid);
		if (pet)
		{
			pet->SetGiven(false);
			m_hawkerPets.erase(remove(m_hawkerPets.begin(), m_hawkerPets.end(), m_hawkerPets.at(realIndex)));
			UpdateHawkPet(m_iHawkPageIndex);
			m_InithawkerPets = m_hawkerPets;
		}
	}
}

UInt8 CMenuHawkerProcess::GetHawkMenuIndex()
{
	return m_iHawkMenuIndex;
}

void CMenuHawkerProcess::StartHawk()
{
	UIEditBox* nameEditBox = gGlobal.pUIManager->FindUIEditBox("HawkerOwnerNameBox");
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject() && nameEditBox)
	{
		if (m_hawkerItems.size() == 0 && m_hawkerPartners.size() == 0 && m_hawkerPets.size() == 0)
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_NO_ITEM_FOR_SELL")));
			UIButton* button = gGlobal.pUIManager->FindUIButton("HawkerOwnerHawk");
			if (button)
			{
				button->SetNormalState();
				gGlobal.pUIManager->AddDirtyUIItem(button);
			}
			return;
		}

		String name = nameEditBox->GetText(); 
		if (gGlobal.IsSuitableName(name))
		{
			gGlobal.g_pCharacter->GetCharNetObject()->StartHawk(gGlobal.pPeerProxy, RPCContext(), name, m_hawkerItems, m_hawkerPartners, m_hawkerPets);
			ClosePanel();
			m_bStartHawk = true;
			gGlobal.g_pCharacter->Stop();
			//gGlobal.m_bUsePartnerCollectionBuff = !gGlobal.m_bUsePartnerCollectionBuff;
			//	m_startHawkCharIdList.push_back(gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id());
		}
		else
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CREATE_CHECK_FAIL")));
			UIButton* button = gGlobal.pUIManager->FindUIButton("HawkerOwnerHawk");
			if (button)
			{
				button->SetNormalState();
				gGlobal.pUIManager->AddDirtyUIItem(button);
			}
		}
	}
}

void CMenuHawkerProcess::StopHawk()
{
	if (gGlobal.g_pMyHawkGroup )
		gGlobal.g_pMyHawkGroup->StopHawk(gGlobal.pPeerProxy, RPCContext());
	m_bStartHawk = false;
}

bool CMenuHawkerProcess::IsStartHawk()
{
	return m_bStartHawk;
}

void CMenuHawkerProcess::ClickHawkAlertPanel()
{
	UIItem* ui = gGlobal.pUIManager->GetFocusItem();
	if (ui && !m_bOpenHawk)
	{
		StringA charIDText = ui->GetName()->substr(11);
		//charIDText = charIDText.substr(11);
		UInt32 charID = atoi(charIDText.c_str());
		if (charID == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id())
		{
			OpenHawkerOwnerMenu();
		}
		else
		{
			if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject() && charID > 0)
			{
				gGlobal.g_pCharacter->GetCharNetObject()->GetHawkFromTarget(gGlobal.pPeerProxy, RPCContext(), charID);
				m_targetId = charID;
				if (!IsConcernTarget(charID) && ui->GetColor() != ItemTextColor[ItemColorIndex_Grey])//&& 还没有看过对方的摊位
				{
					StringA panelName;
					panelName.Format("Hawker%dPanel", charID);
					UIPanel* panel = gGlobal.pUIManager->FindUIPanel(panelName.c_str());
					UIItem* textUiItem = NULL;
					for (std::vector<UIItem *>::iterator it = panel->GetList().begin(); it != panel->GetList().end(); it++ )
					{
						StringA charIDText = (*it)->GetName()->substr(8);
						if (atoi(charIDText.c_str()) == m_targetId)
							textUiItem = (*it);
					}
					if (textUiItem)
					{
						textUiItem->SetColor(ItemTextColor[ItemColorIndex_Grey]);
						gGlobal.pUIManager->AddDirtyUIItem(textUiItem);
					}
					ui->SetColor(ItemTextColor[ItemColorIndex_Grey]);
					gGlobal.pUIManager->AddDirtyUIItem(ui);
				}
			}
		}
	}
}


void CMenuHawkerProcess::SelectHawkSellItem()
{
	if (gGlobal.g_pTargetHawkGroup)
	{
		HawkItemMap itemList = gGlobal.g_pTargetHawkGroup->GetItemList();
		m_hawkItemList.clear();
		UInt8 i = 0;
		m_iHawkMenuIndex = HAWKER_ITEM;
		for (HawkItemMap::iterator itr = itemList.begin(); itr != itemList.end(); ++itr, ++i)
		{
			StringA iconName = "";
			StringA buttonName = "";
			StringA priceName = "";
			StringA stackName = "";
			StringA itemName = "";
			StringA priceTitleName = "";
			StringA buttonBg = "";

			iconName.Format("HawkSell0%02dIcon", i);
			buttonName.Format("HawkSellSlot0%02d", i);
			priceName.Format("HawkSell0%02dPrice", i);
			itemName.Format("HawkSell0%02dName", i);
			stackName.Format("HawkSell0%02dStack", i);
			priceTitleName.Format("HawkSell0%02dPriceTitle", i);
			buttonBg.Format("HawkSellSlot0%02dButtonBg", i);

			UIButton* pButton = gGlobal.pUIManager->FindUIButton(buttonName.c_str());
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
			UIStaticText* pPrice = gGlobal.pUIManager->FindUIStaticText(priceName.c_str());
			UIStaticText* pStack = gGlobal.pUIManager->FindUIStaticText(stackName.c_str());
			UIStaticText* pName = gGlobal.pUIManager->FindUIStaticText(itemName.c_str());
			UIStaticText* pPriceTitle = gGlobal.pUIManager->FindUIStaticText(priceTitleName.c_str());
			UIImage* pImg = gGlobal.pUIManager->FindUIImage(buttonBg.c_str());

			const ItemData* pItemData = gGlobal.m_resource->GetItemData(itr->second->GetItem_id());

			if (pButton && pAniIcon && pPrice && pStack && pName && pItemData && pPriceTitle && pImg)
			{
				pButton->SetEnable(true);
				pPriceTitle->SetEnable(true);
				pImg->SetEnable(false); //turn off the dummy button image

				String text;
				text.Format(_T("%d%s"), itr->second->GetItem_price(),gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT2")));

				Color32 textColor = 0;

				if ( 0 <= itr->second->GetItem_price() && itr->second->GetItem_price() < 10000)
					textColor = TextColor[ColorIndex_White];
				else if ( 10000 <= itr->second->GetItem_price() && itr->second->GetItem_price() < 100000)
					textColor = TextColor[ColorIndex_Green];
				else if ( 100000 <= itr->second->GetItem_price() && itr->second->GetItem_price() < 1000000)
					textColor = TextColor[ColorIndex_Blue];
				else if ( 1000000 <= itr->second->GetItem_price() && itr->second->GetItem_price() < 10000000)
					textColor = TextColor[ColorIndex_Violet];
				else 
					textColor = ItemTextColor[ItemColorIndex_Golden];

				if (textColor != 0)
					pPrice->SetColor(textColor);

				pPrice->SetText(text.c_str());
				pPrice->Redraw();
				pPrice->SetEnable(true);

				text.Format(_T("%d"), itr->second->GetItem_stack());
				pStack->SetText(text.c_str());
				pStack->Redraw();
				pStack->SetEnable(true);

				pName->SetText(String(pItemData->name).c_str());
				pName->Redraw();
				pName->SetEnable(true);

				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(String(pItemData->icon2).c_str());
				pAniIcon->SetEnable(true);

				m_hawkItemList.push_back(itr->second);

			}
		}
	}
}

void CMenuHawkerProcess::SelectHawkSellPartner()
{
	if (gGlobal.g_pTargetHawkGroup)
	{
		m_hawkPartnerList.clear();
		HawkItemMap partnerList = gGlobal.g_pTargetHawkGroup->GetPartnerList();
		UInt8 i = 0;
		m_iHawkMenuIndex = HAWKER_PARTNER;
		for (HawkItemMap::iterator itr = partnerList.begin(); itr != partnerList.end(); ++itr, ++i)
		{
			StringA iconName = "";
			StringA buttonName = "";
			StringA priceName = "";
			StringA itemName = "";
			StringA priceTitleName = "";
			StringA bgName = "";
			StringA buttonBg = "";

			iconName.Format("HawkSellPP0%02dIcon", i);
			buttonName.Format("HawkSellPPSlot0%02d", i);
			priceName.Format("HawkSellPP0%02dPrice", i);
			itemName.Format("HawkSellPP0%02dName", i);
			priceTitleName.Format("HawkSellPP0%02dPriceTitle", i);
			bgName.Format("HawkSellPP0%02dBg", i);
			buttonBg.Format("HawkSellPPSlot0%02dButtonBg", i);

			UIButton* pButton = gGlobal.pUIManager->FindUIButton(buttonName.c_str());
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
			UIStaticText* pPrice = gGlobal.pUIManager->FindUIStaticText(priceName.c_str());
			UIStaticText* pName = gGlobal.pUIManager->FindUIStaticText(itemName.c_str());
			UIStaticText* pPriceTitle = gGlobal.pUIManager->FindUIStaticText(priceTitleName.c_str());
			UIImage* pBg = gGlobal.pUIManager->FindUIImage(bgName.c_str());
			UIImage* pImg = gGlobal.pUIManager->FindUIImage(buttonBg.c_str());

			MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(itr->second->GetItem_id());

			if (pButton && pAniIcon && pPrice && pName && mobTemplate && pPriceTitle && pBg && pImg)
			{
				pButton->SetEnable(true);
				pPriceTitle->SetEnable(true);
				pBg->SetEnable(true);
				pImg->SetEnable(false); //turn off the dummy button image

				String text;
				text.Format(_T("%d%s"), itr->second->GetItem_price(),gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT2")));

				Color32 textColor = gGlobal.GetDisplayMoneyColor(itr->second->GetItem_price());

				if (textColor != 0)
					pPrice->SetColor(textColor);
				pPrice->SetText(text.c_str());
				pPrice->Redraw();
				pPrice->SetEnable(true);

				pName->SetText(String(mobTemplate->m_basicinfo->mid_name).c_str());
				pName->Redraw();
				pName->SetEnable(true);

				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(String(mobTemplate->m_basicinfo->mob_faceS).c_str());
				pAniIcon->SetEnable(true);

				m_hawkPartnerList.push_back(itr->second);
			}
		}
	}
}

void CMenuHawkerProcess::SelectHawkSellPet()
{
	if (gGlobal.g_pTargetHawkGroup)
	{
		m_hawkPetList.clear();
		HawkItemMap petList = gGlobal.g_pTargetHawkGroup->GetPetList();
		UInt8 i = 0;
		m_iHawkMenuIndex = HAWKER_PET;
		for (HawkItemMap::iterator itr = petList.begin(); itr != petList.end(); ++itr, ++i)
		{
			StringA iconName = "";
			StringA buttonName = "";
			StringA priceName = "";
			StringA itemName = "";
			StringA priceTitleName = "";
			StringA bgName = "";
			StringA buttonBg = "";

			iconName.Format("HawkSellPP0%02dIcon", i);
			buttonName.Format("HawkSellPPSlot0%02d", i);
			priceName.Format("HawkSellPP0%02dPrice", i);
			itemName.Format("HawkSellPP0%02dName", i);
			priceTitleName.Format("HawkSellPP0%02dPriceTitle", i);
			bgName.Format("HawkSellPP0%02dBg", i);
			buttonBg.Format("HawkSellPPSlot0%02dButtonBg", i);

			UIButton* pButton = gGlobal.pUIManager->FindUIButton(buttonName.c_str());
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
			UIStaticText* pPrice = gGlobal.pUIManager->FindUIStaticText(priceName.c_str());
			UIStaticText* pName = gGlobal.pUIManager->FindUIStaticText(itemName.c_str());
			UIStaticText* pPriceTitle = gGlobal.pUIManager->FindUIStaticText(priceTitleName.c_str());
			UIImage* pBg = gGlobal.pUIManager->FindUIImage(bgName.c_str());
			UIImage* pImg = gGlobal.pUIManager->FindUIImage(buttonBg.c_str());

			const PetData* pPetData = gGlobal.m_resource->GetPetData(itr->second->GetItem_id());

			if (pButton && pAniIcon && pPrice && pName && pPetData && pPriceTitle && pBg && pImg)
			{
				pButton->SetEnable(true);
				pPriceTitle->SetEnable(true);
				pBg->SetEnable(true);
				pImg->SetEnable(false); //turn off the dummy button image

				String text;
				text.Format(_T("%d%s"), itr->second->GetItem_price(),gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT2")));

				Color32 textColor = gGlobal.GetDisplayMoneyColor(itr->second->GetItem_price());

				if (textColor != 0)
					pPrice->SetColor(textColor);

				pPrice->SetText(text.c_str());
				pPrice->Redraw();
				pPrice->SetEnable(true);

				pName->SetText(String(pPetData->pet_name).c_str());
				pName->Redraw();
				pName->SetEnable(true);

				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(String(pPetData->pet_faceS).c_str());
				pAniIcon->SetEnable(true);

				m_hawkPetList.push_back(itr->second);
			}
		}
	}
}

void CMenuHawkerProcess::ShowHawkMessage(UInt8 index)
{
	String text;
	switch(index)
	{
	case HAWK_CANT_DO_ANYTHING:
		text =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_DO_ANYTHING"));
		break;
	case HAWK_CANT_HAWK:
		text =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_CANT_FOR_HAWK"));
		break;
	case HAWK_CANT_FOR_SELL:
		text =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_CANT_FOR_SELL"));
		break;
	case HAWK_STOP_HAWK:
		text =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_STOP_HAWK"));
		break;
	}
	if (text.length() > 0)
		gGlobal.PrintPlayerMessage(text.c_str());
}

void CMenuHawkerProcess::ChangeHawkName()
{
	if (m_bStartHawk)
	{
		String text;
		UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("HawkerOwnerNameBox");
		if (pEditBox)
		{
			text = String(pEditBox->GetText());
			if (gGlobal.IsSuitableName(text) && gGlobal.g_pMyHawkGroup)
				gGlobal.g_pMyHawkGroup->ChangeHawkName(gGlobal.pPeerProxy, RPCContext(), text);
			else
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CREATE_CHECK_FAIL")));
		}
	}
}

void CMenuHawkerProcess::CloseHawkFromTarget()
{
	if (gGlobal.g_pTargetHawkGroup)
	{
		m_iHawkMenuIndex = HAWKER_CLOSE;
		gGlobal.g_pTargetHawkGroup->CloseHawkForUser(gGlobal.pPeerProxy, RPCContext(), gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id());
		CHawkGroup* temp = gGlobal.g_pTargetHawkGroup;
		gGlobal.g_pTargetHawkGroup = NULL;
		DeleteNetGroup(temp, BOOLEAN_TRUE);
	}
}

int CMenuHawkerProcess::GetMaxItemStackForHawk(UInt8 index)
{
	if (m_hawkItemList.size() > index && m_iHawkMenuIndex == HAWKER_ITEM)
	{
		if(gGlobal.g_pCharacter->GetCharNetObject()->GetMoney() < m_hawkItemList[index]->GetItem_price()*m_hawkItemList[index]->GetItem_stack())
		{
			UInt8 stack = 0;
			UInt32 charatermoney = gGlobal.g_pCharacter->GetCharNetObject()->GetMoney();
			while(charatermoney >= m_hawkItemList[index]->GetItem_price())
			{
				charatermoney -= m_hawkItemList[index]->GetItem_price();
				stack ++;
			}
			return stack;
		}
		else
			return m_hawkItemList[index]->GetItem_stack();
	}
	return 0;
}

int CMenuHawkerProcess::GetItemPriceForHawk(UInt8 index)
{
	if(m_iHawkMenuIndex == HAWKER_ITEM && m_hawkItemList.size() > index)
		return m_hawkItemList[index]->GetItem_price();
	else if (m_iHawkMenuIndex == HAWKER_PARTNER && m_hawkPartnerList.size() > index)
		return m_hawkPartnerList[index]->GetItem_price();
	else if (m_iHawkMenuIndex == HAWKER_PET && m_hawkPetList.size() > index)
		return m_hawkPetList[index]->GetItem_price();

	return 0;
}

void CMenuHawkerProcess::ShowItemPriceForHawk(UInt8 index, UInt8 stack)
{
	UIStaticText* text = gGlobal.pUIManager->FindUIStaticText("HawkTotalPrice");
	if (text)
	{
		UInt32 price = 0;
		if(m_iHawkMenuIndex == HAWKER_ITEM && m_hawkItemList.size() > index)
			price = m_hawkItemList[index]->GetItem_price();
		else if (m_iHawkMenuIndex == HAWKER_PARTNER && m_hawkPartnerList.size() > index)
			price = m_hawkPartnerList[index]->GetItem_price();
		else if (m_iHawkMenuIndex == HAWKER_PET && m_hawkPetList.size() > index)
			price = m_hawkPetList[index]->GetItem_price();

		price *= stack;
		String temp;
		temp.Format(_T("%d%s"), price,gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT2")));
		text->SetText(temp.c_str());
		SetTextMoneyColor(text,price);
		text->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(text);
	}
}

void CMenuHawkerProcess::BuyHawkItem(UInt8 itemIndex, UInt8 itemMenuIndex, UInt8 itemStack)
{
	if (itemMenuIndex == m_iHawkMenuIndex && itemStack > 0 && gGlobal.g_pTargetHawkGroup)
	{
		UInt32 price = 0;
		UInt32 itemID = 0;
		switch(m_iHawkMenuIndex)
		{
		case HAWKER_ITEM:
			if (m_hawkItemList.size() > itemIndex)
			{
				itemID = m_hawkItemList[itemIndex]->GetItem_uid();
				price = m_hawkItemList[itemIndex]->GetItem_price() * itemStack;
			}
			break;
		case HAWKER_PARTNER:
			if (m_hawkPartnerList.size() > itemIndex)
			{
				itemID = m_hawkPartnerList[itemIndex]->GetItem_uid();
				price = m_hawkPartnerList[itemIndex]->GetItem_price();
			}
			break;
		case HAWKER_PET:
			if (m_hawkPetList.size() > itemIndex)
			{
				itemID = m_hawkPetList[itemIndex]->GetItem_uid();
				price = m_hawkPetList[itemIndex]->GetItem_price();
			}
			break;
		}

		if (price > gGlobal.g_pCharacter->GetCharNetObject()->GetMoney())
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_MONEY_FAIL")));
			return;
		}
		if (itemID > 0)
			gGlobal.g_pTargetHawkGroup->BuyHawkItem(gGlobal.pPeerProxy, RPCContext(), itemID, m_iHawkMenuIndex, itemStack);
	}
}

void CMenuHawkerProcess::UpdateHawkerItemList(UInt32 item_uid, UInt8 remainStack)
{
	for(Index i = C_INDEX(0); i < m_hawkerItems.size(); ++i)
	{
		if (m_hawkerItems[i].item_uid == item_uid)
		{
			CItem* item = gGlobal.m_pBag->GetItem(m_hawkerItems[i].item_index);
			if (item)
			{
				item->SetGivenStack(remainStack);
				int oldItembag = m_hawkerItems[i].item_index / BAG_ITEM_COUNT;

				if (oldItembag == gGlobal.g_pItemProcess->GetBagPage())
				{
					int oldIndex = m_hawkerItems[i].item_index % BAG_ITEM_COUNT;
					gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
				}
			}

			if (remainStack == 0)
				m_hawkerItems.erase(remove(m_hawkerItems.begin(), m_hawkerItems.end(), m_hawkerItems.at(i)));
			else
				m_hawkerItems[i].item_stack = remainStack;
		}
	}
	if (m_iHawkMenuIndex == HAWKER_ITEM)
		UpdateHawkItem(m_iHawkPageIndex);
	m_InithawkerItems = m_hawkerItems;
}


void CMenuHawkerProcess::UpdateHawkerPartnerList(UInt32 partner_id)
{
	for(Index i = C_INDEX(0); i < m_hawkerPartners.size(); ++i)
	{
		if (m_hawkerPartners[i].item_uid == partner_id)
			m_hawkerPartners.erase(remove(m_hawkerPartners.begin(), m_hawkerPartners.end(), m_hawkerPartners.at(i)));
	}
	if (m_iHawkMenuIndex == HAWKER_PARTNER)
		UpdateHawkPartner(m_iHawkPageIndex);
	m_InithawkerPartners = m_hawkerPartners;
}

void CMenuHawkerProcess::UpdateHawkerPetList(UInt32 pet_id)
{
	for(Index i = C_INDEX(0); i < m_hawkerPets.size(); ++i)
	{
		if (m_hawkerPets[i].item_uid == pet_id)
			m_hawkerPets.erase(remove(m_hawkerPets.begin(), m_hawkerPets.end(), m_hawkerPets.at(i)));
	}
	if (m_iHawkMenuIndex == HAWKER_PET)
		UpdateHawkPet(m_iHawkPageIndex);
	m_InithawkerPets = m_hawkerPets;
}

void CMenuHawkerProcess::SetMajorPanelToTop()
{

	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("Partner");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("Char");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("FriendLeaderMe");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("ClockPanel");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("MiniMap");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("Location");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("Time");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	for (UInt8 i = 1; i < 5; ++i)
	{
		StringA panelName;
		panelName.Format("Friend%d", i);
		panel = gGlobal.pUIManager->FindUIPanel(panelName.c_str());
		if (panel)
			gGlobal.pUIManager->SetPanel2Top(panel);
		panelName.Format("FriendLeader%d", i);
		panel = gGlobal.pUIManager->FindUIPanel(panelName.c_str());
		if (panel)
			gGlobal.pUIManager->SetPanel2Top(panel);
	}

	panel = gGlobal.pUIManager->FindUIPanel("MapInfo");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("Bottom");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("HroseLight");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("HShortCutPanel");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("VShortCutPanel");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("HideShortCutPanel");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	UIPanelEx* panel1 = gGlobal.pUIManager->FindUIPanelEx("ChatPanel");
	if (panel1)
		gGlobal.pUIManager->SetPanelEx2Top(panel1);

	panel = gGlobal.pUIManager->FindUIPanel("ChatButtonArea");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("ChatRightButtonArea");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);

	panel = gGlobal.pUIManager->FindUIPanel("BottomBar");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);
	panel = gGlobal.pUIManager->FindUIPanel("TargetInfo");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);
	panel = gGlobal.pUIManager->FindUIPanel("HelpPanel");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);
	panel = gGlobal.pUIManager->FindUIPanel("BuffIconPanel");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);
	panel = gGlobal.pUIManager->FindUIPanel("BuffDetailMenuPanel");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);
	panel = gGlobal.pUIManager->FindUIPanel("TipsPanel");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);
	panel = gGlobal.pUIManager->FindUIPanel("FloatNamePanel");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);
	panel = gGlobal.pUIManager->FindUIPanel("HawkPanel");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);
	panel = gGlobal.pUIManager->FindUIPanel("HawkerOwnerPanel");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);
	panel = gGlobal.pUIManager->FindUIPanel("PartyPanel");
	if (panel)
		gGlobal.pUIManager->SetPanel2Top(panel);
}

void CMenuHawkerProcess::UpdateUserMoneyDisplay()
{
	UIStaticText* text = gGlobal.pUIManager->FindUIStaticText("HawkMoneyText");
	if (text && gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
	{
		String money;
		money.Format(_T("%d"), gGlobal.g_pCharacter->GetCharNetObject()->GetMoney());
		text->SetText(money.c_str());
		SetTextMoneyColor(text,gGlobal.g_pCharacter->GetCharNetObject()->GetMoney());
		text->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(text);
	}
}

void CMenuHawkerProcess::ShowHawkSelltemDetail()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("HawkPanel");
	if (pPanel && gGlobal.g_pTargetHawkGroup)
	{
		Int posX = gGlobal.pUIManager->GetCursorX() - pPanel->GetX();  
		Int posY = gGlobal.pUIManager->GetCursorY() - pPanel->GetY();
		if (posX > 0 && posY > 0 && posX < (Int)pPanel->GetWidth() && posY < (Int)pPanel->GetHeight())
		{
			UInt8 index = 0;
			UInt8 row = 0;
			UInt8 colume = 0;
			if (m_iHawkMenuIndex == HAWKER_ITEM)
			{
				if (posX > 16 && posX < 56)
					colume = 0;
				else if (posX > 179 && posX < 219)
					colume = 1;
				else if (posX > 342 && posX < 382)
					colume = 2;
				else if (posX > 506 && posX < 546)
					colume = 3;
				else
					colume = 4;

				if (posY > 65 && posY < 105)
					row = 0;
				else if (posY > 119 && posY < 159)
					row = 1;
				else if (posY > 172 && posY < 212)
					row = 2;
				else if (posY > 225 && posY < 265)
					row = 3;
				else if (posY > 278 && posY < 318)
					row = 4;
				else if (posY > 331 && posY < 371)
					row = 5;
				else 
					row = 6;

				if (colume == 4 || row == 6)
					return;

				index = colume + row * 4;

				if (m_hawkItemList.size() <= index)
					return;

				CItem* item = CastNetObject<CItem>(m_hawkItemList[index]->GetDataObject());
				if (item)
				{
					UIPanelEx* pPanelEx = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");
					if (pPanelEx == NULL)
					{
						XML_UI::GetInstance().Init();
						XML_UI::GetInstance().ReadFormFile("Data\\ItemDetail.xml", gGlobal.pUIManager);

						pPanelEx = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");
						if (pPanelEx == NULL)
							return;		
					}
					gGlobal.pUIManager->SetPanelEx2Top(pPanelEx);


					gGlobal.g_pItemProcess->ItemDetailPopUp(item->GetItem_id(), false, item);

					Int32 posX = 0, posY = 0;

					if ( ( gGlobal.pUIManager->GetCursorX() + pPanelEx->GetWidth() ) > 800 )
						posX = 800 - pPanelEx->GetWidth();
					else
						posX = gGlobal.pUIManager->GetCursorX();

					if ( ( gGlobal.pUIManager->GetCursorY() + pPanelEx->GetHeight() ) > 600 )
						posY = 600 - pPanelEx->GetHeight();
					else
						posY = gGlobal.pUIManager->GetCursorY();

					pPanelEx->SetPos(posX, posY);
					pPanelEx->SetEnable(true);					
				}				
			}
			else
			{
				if (posX > 16 && posX < 56)
					colume = 0;
				else if (posX > 338 && posX < 378)
					colume = 1;
				else
					colume = 2;

				if (posY > 65 && posY < 105)
					row = 0;
				else if (posY > 119 && posY < 159)
					row = 1;
				else if (posY > 172 && posY < 212)
					row = 2;
				else if (posY > 225 && posY < 265)
					row = 3;
				else if (posY > 278 && posY < 318)
					row = 4;
				else if (posY > 331 && posY < 371)
					row = 5;
				else 
					row = 6;

				if (colume == 2 || row == 6)
					return;

				index = colume + row * 2;

				if (m_iHawkMenuIndex == HAWKER_PARTNER)
				{
					if (m_hawkPartnerList.size() <= index)
						return;

					CPartner* pPartner = CastNetObject<CPartner>(m_hawkPartnerList[index]->GetDataObject());
					if (pPartner)
					{
						UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartnerDetailPanel");
						if (pPanel == NULL)
						{
							XML_UI::GetInstance().Init();
							XML_UI::GetInstance().ReadFormFile("Data\\PartnerDetail.xml", gGlobal.pUIManager);

							pPanel = gGlobal.pUIManager->FindUIPanel("PartnerDetailPanel");
							if (pPanel == NULL)
								return;		
						}
						UInt16Vector skillList = gGlobal.g_pTargetHawkGroup->GetPartnerSkillList(pPartner->GetPartner_id());
						gGlobal.g_pPartnerProcess->ShowPartnerDetail(pPartner, skillList);
						gGlobal.pUIManager->SetPanel2Top(pPanel);

						Int32 posX = 0, posY = 0;

						if ( ( gGlobal.pUIManager->GetCursorX() + pPanel->GetWidth() ) > 800 )
							posX = 800 - pPanel->GetWidth();
						else
							posX = gGlobal.pUIManager->GetCursorX();

						if ( ( gGlobal.pUIManager->GetCursorY() + pPanel->GetHeight() ) > 600 )
							posY = 600 - pPanel->GetHeight();
						else
							posY = gGlobal.pUIManager->GetCursorY();

						pPanel->SetPos(posX, posY);
						pPanel->SetEnable(true);
					}
				}
				else if (m_iHawkMenuIndex == HAWKER_PET)
				{
					if (m_hawkPetList.size() <= index)
						return;

					CPet* pPet = CastNetObject<CPet>(m_hawkPetList[index]->GetDataObject());
					if (pPet)
					{
						UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PetDetailPanel");
						if (pPanel == NULL)
						{
							XML_UI::GetInstance().Init();
							XML_UI::GetInstance().ReadFormFile("Data\\PetDetail.xml", gGlobal.pUIManager);

							pPanel = gGlobal.pUIManager->FindUIPanel("PetDetailPanel");
							if (pPanel == NULL)
								return;		
						}
						UInt16Vector skillList = gGlobal.g_pTargetHawkGroup->GetPetSkillList(pPet->GetPet_uid());
						gGlobal.g_pMenuPetProcess->ShowPetDetail(pPet, skillList);
						gGlobal.pUIManager->SetPanel2Top(pPanel);

						Int32 posX = 0, posY = 0;

						if ( ( gGlobal.pUIManager->GetCursorX() + pPanel->GetWidth() ) > 800 )
							posX = 800 - pPanel->GetWidth();
						else
							posX = gGlobal.pUIManager->GetCursorX();

						if ( ( gGlobal.pUIManager->GetCursorY() + pPanel->GetHeight() ) > 600 )
							posY = 600 - pPanel->GetHeight();
						else
							posY = gGlobal.pUIManager->GetCursorY();

						pPanel->SetPos(posX, posY);
						pPanel->SetEnable(true);
					}
				}
			}
		}
	}
}

void CMenuHawkerProcess::ResumeHawkerOwner()
{
	if (gGlobal.g_pMyHawkGroup)
	{
		m_hawkerItems.clear();
		m_hawkerPartners.clear();
		m_hawkerPets.clear();

		HawkItemMap itemList = gGlobal.g_pMyHawkGroup->GetItemList();
		for (HawkItemMap::iterator itr = itemList.begin(); itr != itemList.end(); ++itr)
		{
			HawkerItemInfo hawkerItem;
			hawkerItem.item_id = itr->second->GetItem_id();
			hawkerItem.item_index = itr->second->GetItem_index();
			hawkerItem.item_price = itr->second->GetItem_price();
			hawkerItem.item_stack = itr->second->GetItem_stack();
			hawkerItem.item_uid = itr->second->GetItem_uid();
			m_hawkerItems.push_back(hawkerItem);

			CItem* item = gGlobal.m_pBag->GetItem(hawkerItem.item_index);
			if (item)
				item->SetGivenStack(hawkerItem.item_stack);
		}

		HawkItemMap partnerList = gGlobal.g_pMyHawkGroup->GetPartnerList();
		for (HawkItemMap::iterator itr = partnerList.begin(); itr != partnerList.end(); ++itr)
		{
			HawkerItemInfo hawkerPartner;
			hawkerPartner.item_id = itr->second->GetItem_id();
			hawkerPartner.item_index = itr->second->GetItem_index();
			hawkerPartner.item_price = itr->second->GetItem_price();
			hawkerPartner.item_stack = itr->second->GetItem_stack();
			hawkerPartner.item_uid = itr->second->GetItem_uid();
			m_hawkerPartners.push_back(hawkerPartner);

			CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(hawkerPartner.item_uid);
			if (partner)
				partner->SetGiven(true);
		}

		HawkItemMap petList = gGlobal.g_pMyHawkGroup->GetPetList();
		for (HawkItemMap::iterator itr = petList.begin(); itr != petList.end(); ++itr)
		{
			HawkerItemInfo hawkerPet;
			hawkerPet.item_id = itr->second->GetItem_id();
			hawkerPet.item_index = itr->second->GetItem_index();
			hawkerPet.item_price = itr->second->GetItem_price();
			hawkerPet.item_stack = itr->second->GetItem_stack();
			hawkerPet.item_uid = itr->second->GetItem_uid();
			m_hawkerPets.push_back(hawkerPet);

			CPet* pet = gGlobal.g_PetLst->GetPet(hawkerPet.item_uid);
			if (pet)
				pet->SetGiven(true);
		}
		m_bStartHawk = true;
		if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
			gGlobal.g_pCharacter->GetCharNetObject()->OnUpdateHawkInfo(gGlobal.g_pCharacter->GetCharNetObject()->GetHawkName());
	}
}

void CMenuHawkerProcess::UpdateHawkerTitle()
{
	UIStaticText* text = gGlobal.pUIManager->FindUIStaticText("HawkerTitle");
	if (text && gGlobal.g_pTargetHawkGroup)
	{
		text->SetText(gGlobal.g_pTargetHawkGroup->GetHawkName().c_str());
		text->Redraw();
		text->SetEnable(true);
	}
}

void CMenuHawkerProcess::HawkerOwnerNameListScroll(UInt8 index) //0: up, 1:down, 2:scroll
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("HawkerOwnerNameListScroll");
	UInt8 maxCount = 0;
	if (m_iMenuIndex == ITEM_PARTNER_MENU)
	{
		if (gGlobal.g_pPartnerGroup)
			maxCount = (UInt8)gGlobal.g_pPartnerGroup->GetCount();
	}
	else if (m_iMenuIndex == ITEM_PET_MENU)
	{
		if (gGlobal.g_PetLst)
			maxCount = (UInt8)gGlobal.g_PetLst->GetCount();
	}

	if (index == 0)
	{
		if (m_iStartIndex > 0)
			--m_iStartIndex;
	}
	else if (index == 1)
	{

		if ( (m_iStartIndex + MAX_PET_PARTNER_ONE_PAGE) < (maxCount) )
			++m_iStartIndex;
	}
	else if (index == 2 && scrollBar)
	{
		if (maxCount < MAX_PET_PARTNER_ONE_PAGE)
			m_iStartIndex = 0;
		else if (scrollBar->GetSelNum() + MAX_PET_PARTNER_ONE_PAGE >= maxCount)
			m_iStartIndex = maxCount - MAX_PET_PARTNER_ONE_PAGE;
		else 
			m_iStartIndex = scrollBar->GetSelNum();
	}


	if (m_iMenuIndex == ITEM_PARTNER_MENU)
		UpdateOwnerPartnerNameList();
	else if (m_iMenuIndex == ITEM_PET_MENU)
		UpdateOwnerPetNameList();
}

void CMenuHawkerProcess::ResetOwnerScroll()
{

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("HawkerOwnerNameListScroll");

	if (m_iMenuIndex == ITEM_PARTNER_MENU)
	{
		if (scrollBar && gGlobal.g_pPartnerGroup)
		{

			scrollBar->SetNumCount(gGlobal.g_pPartnerGroup->GetCount());
			scrollBar->HomeScroll();
		}
	}
	else if (m_iMenuIndex == ITEM_PET_MENU)
	{
		if (scrollBar && gGlobal.g_PetLst)
		{
			scrollBar->SetNumCount(gGlobal.g_PetLst->GetCount());
			scrollBar->HomeScroll();
		}
	}
}

void CMenuHawkerProcess::UpdateHawkerTime()
{
	UIStaticText* text = gGlobal.pUIManager->FindUIStaticText("HawkerOwnerHawkTime");
	if (text && gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject() )
	{
		String hawkerTime;
		hawkerTime.Format(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_REMAIN_HAWK_TIME")), gGlobal.g_pCharacter->GetCharNetObject()->GetHawkerTime());
		text->SetText(hawkerTime.c_str());
		text->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(text);
	}
}

void CMenuHawkerProcess::DisappearHawkPanel()
{
	if (gGlobal.g_pTargetHawkGroup && gGlobal.g_pMap)
	{
		CCharacter* pChar = gGlobal.g_pMap->FindCharacter(gGlobal.g_pTargetHawkGroup->GetOwnerID());
		if (pChar)
		{
			BlockPos currPos = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();
			BlockPos center;
			center.PosX = pChar->GetCharacterView()->GetCharMainView()->GetCharPos().PosX;
			center.PosY = pChar->GetCharacterView()->GetCharMainView()->GetCharPos().PosY;

			PixelDirection dummy;

			if (QGLMath::CalculateBlockPosDistance(currPos, center, dummy) > 400)
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseHawk", String());
		}
	}
}

void CMenuHawkerProcess::ClosePanel()
{
	if (gGlobal.pUIManager)
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(),"CloseOnlineShop", String());
	}
}

void CMenuHawkerProcess::AddHawkerHistory(HawkerHistory data, String itemName, bool pushback)
{
	HawkerHistoryInfo temp;
	temp.data = data;
	temp.itemName = itemName;
	if (pushback)
		m_hawkerHistory.push_back(temp);
	else
		m_hawkerHistory.push_front(temp);
}

void CMenuHawkerProcess::RemoveHawkerHistory(UInt32Vector delHistoryID)
{
	if (m_hawkerHistory.size() > 0)
	{
		for (Index i = C_INDEX(0); i < delHistoryID.size(); ++i)
		{
			for (UInt j = (UInt)m_hawkerHistory.size(); j > 0; --j)
			{
				if (m_hawkerHistory[j - 1].data.historyId == delHistoryID[i])
				{
					if (j == m_hawkerHistory.size())
						m_hawkerHistory.pop_back();
					else
						m_hawkerHistory.erase(remove(m_hawkerHistory.begin(), m_hawkerHistory.end(), m_hawkerHistory.at(j - 1)));
					break;
				}
			}
		}
	}
}

void CMenuHawkerProcess::SetItemMoneyColour()
{
	UIEditBox* gmoneyBox = gGlobal.pUIManager->FindUIEditBox("PriceOfHawkerItem");	
	if(gmoneyBox)
	{
		SetEditBoxMoneyColor(gmoneyBox);
	}
}

void CMenuHawkerProcess::InitItemPartnerPet()
{
	if(m_hawkerItems.size() == 0)
		m_hawkerItems = m_InithawkerItems;
	else
		m_InithawkerItems = m_hawkerItems;
	if(m_hawkerPartners.size() == 0)
		m_hawkerPartners = m_InithawkerPartners;
	else
		m_InithawkerPartners = m_hawkerPartners;
	if(m_hawkerPets.size() == 0)
		m_hawkerPets =  m_InithawkerPets;  
	else
		m_InithawkerPets = m_hawkerPets;
	UICheckBox* itemBox = gGlobal.pUIManager->FindUICheckBox("HawkerOwnerSellItem");
	UICheckBox* partnerBox = gGlobal.pUIManager->FindUICheckBox("HawkerOwnerSellPartner");
	UICheckBox* petBox = gGlobal.pUIManager->FindUICheckBox("HawkerOwnerSellPet");
	if(itemBox && partnerBox && petBox)	
	{

		if(m_InithawkerPets.size() > 0)
		{
			if (itemBox->GetState())
			{
				itemBox->SetState(false);
				gGlobal.pUIManager->AddDirtyUIItem(itemBox);
			}
			if (partnerBox->GetState())
			{
				partnerBox->SetState(false);
				gGlobal.pUIManager->AddDirtyUIItem(partnerBox);
			}
			if(!petBox->GetState())
			{
				petBox->SetState(true);
				gGlobal.pUIManager->AddDirtyUIItem(petBox);
			}
			for (Index i = C_INDEX(0); i < m_InithawkerPets.size(); i++)
			{
				CPet* pet = gGlobal.g_PetLst->GetPetByIndex(m_InithawkerPets[i].item_index);
				if ( pet->GetPet_id() != m_InithawkerPets[i].item_id ||pet->GetPet_uid() != m_InithawkerPets[i].item_uid )
				{
					HawkerItemList::iterator iteminfo = m_InithawkerPets.begin();
					iteminfo += i;
					m_InithawkerPets.erase(iteminfo);
					m_hawkerPets = m_InithawkerPets;
					i--;
				}
				else
				{
					if (pet && !pet->IsGiven())			
						pet->SetGiven(true);
				}
			}
		}
		if(m_InithawkerPartners.size() > 0)
		{
			if (itemBox->GetState())
			{
				itemBox->SetState(false);
				gGlobal.pUIManager->AddDirtyUIItem(itemBox);
			}
			if (!partnerBox->GetState())
			{
				partnerBox->SetState(true);
				gGlobal.pUIManager->AddDirtyUIItem(partnerBox);
			}
			if(petBox->GetState())
			{
				petBox->SetState(false);
				gGlobal.pUIManager->AddDirtyUIItem(petBox);
			}
			for (Index i = C_INDEX(0); i < m_InithawkerPartners.size(); i++)
			{
				CPartner* partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(m_InithawkerPartners[i].item_index);
				if ( partner->GetMob_id() != m_InithawkerPartners[i].item_id ||partner->GetPartner_id() != m_InithawkerPartners[i].item_uid)
				{

					HawkerItemList::iterator iteminfo = m_InithawkerPartners.begin();
					iteminfo += i;
					m_InithawkerPets.erase(iteminfo);
					m_hawkerPartners = m_InithawkerPartners;
					i--;
				}
				else
				{	
					if(partner && !partner->IsGiven())
						partner->SetGiven(true);
				}
			}
		}
		if (m_InithawkerItems.size() > 0)
		{
			if (!itemBox->GetState())
			{
				itemBox->SetState(true);
				gGlobal.pUIManager->AddDirtyUIItem(itemBox);
			}
			if (partnerBox->GetState())
			{
				partnerBox->SetState(false);
				gGlobal.pUIManager->AddDirtyUIItem(partnerBox);
			}
			if(petBox->GetState())
			{
				petBox->SetState(false);
				gGlobal.pUIManager->AddDirtyUIItem(petBox);
			}
			for (Index i = C_INDEX(0); i < m_InithawkerItems.size(); i++)
			{
				CItem* item = gGlobal.m_pBag->GetItem(m_InithawkerItems[i].item_index);
				if(item)
				{
					if (item->GetItem_id() != m_InithawkerItems[i].item_id || item->GetItem_uid() != m_InithawkerItems[i].item_uid || item->GetStack() < m_InithawkerItems[i].item_stack)
					{
						HawkerItemList::iterator iteminfo = m_InithawkerItems.begin();
						iteminfo += i;
						m_InithawkerItems.erase(iteminfo);
						m_hawkerItems = m_InithawkerItems;
						i--;
					}
					else
					{
						item->SetGivenStack(m_InithawkerItems[i].item_stack);
						gGlobal.g_pItemProcess->UpdateItemSlot(m_InithawkerItems[i].item_index, true);
						UpdateOwnerItemSlot(m_InithawkerItems[i].item_index, true);
					}
				}
			}
		}
		if (m_InithawkerItems.size() > 0)
			UpdateHawkItem(0);
		else if(m_InithawkerPartners.size()>0)
			UpdateHawkPartner(0);
		else if(m_InithawkerPets.size() > 0)
			UpdateHawkPet(0);
		else
			return;
	}

}

void CMenuHawkerProcess::SetConcernTarget()
{
	UICheckBox* concernTargetUi = gGlobal.pUIManager->FindUICheckBox("SetConcern");
	if(concernTargetUi)
	{
		if (m_targetId > 0)
		{
			m_bSetConcernTarget =  concernTargetUi->GetState();
			StringA panelName;
			panelName.Format("Hawker%dPanel", m_targetId);
			UIPanel* panel = gGlobal.pUIManager->FindUIPanel(panelName.c_str());
			if (panel)
			{
				UIItem* imageUiItem = NULL;
				UIItem* textUiItem = NULL;
				for (std::vector<UIItem *>::iterator it = panel->GetList().begin(); it != panel->GetList().end(); it++ )
				{
					if((*it)->GetName()->size() < 8)
						break;
					StringA charIDText = (*it)->GetName()->substr(8);
					if (atoi(charIDText.c_str()) == m_targetId)
					{
						textUiItem = (*it);
						break;
					}
				}
				for (std::vector<UIItem *>::iterator it = panel->GetList().begin(); it != panel->GetList().end(); it++ )
				{
					if((*it)->GetName()->size() < 11)
						break;
					StringA charIDText = (*it)->GetName()->substr(11);
					if (atoi(charIDText.c_str()) == m_targetId)
					{
						imageUiItem = (*it);
						break;
					}
				}

				if(imageUiItem && textUiItem)
				{
					if(m_bSetConcernTarget)
					{

						Color32 VermeilColor = D3DCOLOR_ARGB(255, 240, 133, 62);
						imageUiItem->SetColor(TextColor[ColorIndex_Yellow]);
						textUiItem->SetColor(VermeilColor);
						m_concernCharIdList.push_back(m_targetId);
					}
					else
					{
						imageUiItem->SetColor(ItemTextColor[ItemColorIndex_Grey]);
						textUiItem->SetColor(ItemTextColor[ItemColorIndex_Grey]);
						m_concernCharIdList.remove(m_targetId);
					}
					gGlobal.pUIManager->AddDirtyUIItem(imageUiItem);
					gGlobal.pUIManager->AddDirtyUIItem(textUiItem);
				}
			}
		}
	}
}

void CMenuHawkerProcess::InitTargetConcernState(UInt32 targetId)
{
	UICheckBox* concernTargetUi = gGlobal.pUIManager->FindUICheckBox("SetConcern");
	if(concernTargetUi)
	{
		concernTargetUi->SetState(IsConcernTarget(targetId));
		gGlobal.pUIManager->AddDirtyUIItem(concernTargetUi);
	}
}

Boolean CMenuHawkerProcess::IsConcernTarget(UInt32 targetId)
{
	for (std::list<UInt32>::iterator it = m_concernCharIdList.begin(); it != m_concernCharIdList.end(); it++)
	{
		if ((*it) == targetId)
			return true;
	}
	return false;
}

void CMenuHawkerProcess::SetAllHawkEnable(Boolean enable)
{
	StringA panelName;
	for (std::list<UInt32>::iterator it = m_startHawkCharIdList.begin(); it != m_startHawkCharIdList.end(); it++)
	{
		panelName.Format("Hawker%dPanel", (*it));
		UInt32 tempChar_id = (*it);
		UIPanel* panel = gGlobal.pUIManager->FindUIPanel(panelName.c_str());
		if (panel)
		{
			UIItem* imageUiItem = NULL;
			UIItem* textUiItem = NULL;
			for (std::vector<UIItem *>::iterator it = panel->GetList().begin(); it != panel->GetList().end(); it++ )
			{
				if((*it)->GetName()->size() < 8)
					break;
				StringA charIDText = (*it)->GetName()->substr(8);
				if (atoi(charIDText.c_str()) == tempChar_id)
				{
					textUiItem = (*it);
					break;
				}
			}
			for (std::vector<UIItem *>::iterator it = panel->GetList().begin(); it != panel->GetList().end(); it++ )
			{
				if((*it)->GetName()->size() < 11)
					break;
				StringA charIDText = (*it)->GetName()->substr(11);
				if (atoi(charIDText.c_str()) == tempChar_id)
				{
					imageUiItem = (*it);
					break;
				}
			}
			if(imageUiItem && textUiItem)
			{
				imageUiItem->SetEnable(enable);
				textUiItem->SetEnable(enable);
				gGlobal.pUIManager->AddDirtyUIItem(imageUiItem);
				gGlobal.pUIManager->AddDirtyUIItem(textUiItem);
				m_allHawkEnable = enable;
			}
		}
	}
}

Boolean CMenuHawkerProcess::GetAllHawkEnable()
{
	return m_allHawkEnable;
}

void CMenuHawkerProcess::AddStartHawkInAllList(UInt32 targetId)
{
	for(std::list<UInt32>::iterator it = m_startHawkCharIdList.begin(); it!= m_startHawkCharIdList.end(); it++)
	{
		if ((*it) == targetId)
			return;
	}
	m_startHawkCharIdList.push_back(targetId);
}

void CMenuHawkerProcess::ReSetConcernTarget(UInt32 targetId)
{
	if (IsConcernTarget(targetId))
		m_concernCharIdList.remove(targetId);
}

void CMenuHawkerProcess::ReSetHawkInAllList(UInt32 targetId)
{
	for(std::list<UInt32>::iterator it = m_startHawkCharIdList.begin(); it!= m_startHawkCharIdList.end(); it++)
	{
		if ((*it) == targetId)
		{
			m_startHawkCharIdList.remove(targetId);
			break;
		}
	}
}