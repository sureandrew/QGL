//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuWareHouseProcess.h"
//-- Library
#include "QGL/Global.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/ItemBag.h"
//#include "MenuItemProcess.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/Pet.h"
#include "Proxy/Logic/PetGroup.h"
#include "Proxy/Logic/SkillGroup.h"
#include "Proxy/Logic/Character.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "QGL/CharacterControl.h"
#include "Resource/ResItem.h"
#include "MenuItemProcess.h"
#include "MenuPartnerProcess.h"
#include "MenuPetProcess.h"
#include "MenuTradingProcess.h"
#include "common_binary.h"
#include "QGLApplication.h"
#include "Math.h"
#include "Resource/ResPet.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResSkill.h"
#include "Proxy/Logic/Skill.h"


#define BAG_MAX_WAREHOUSE	4
#define BAG_MAX_WAREHOUSE_ITEM	(BAG_MAX_WAREHOUSE * BAG_ITEM_COUNT)
#define BAG_MAX_WAREHOUSE_ITEM_PER_PAGE (2 * BAG_ITEM_COUNT)

#define PARTNER_MAX_WAREHOUSE	8
#define PET_MAX_WAREHOUSE	8

CMenuWareHouseProcess::CMenuWareHouseProcess()
{
	m_wareHouseItems.clear();
	RPCNetID empty;
	for (Index i = C_INDEX(0); i < BAG_MAX_WAREHOUSE_ITEM; ++i)
		m_wareHouseItems.push_back(empty);
	m_iMenuIndex = 0;
	m_iStartWareHouseX = 0;
	m_iStartWareHouseY = 0;
	m_iCountDown = 0;
	m_wareHousePartners.clear();
	m_wareHousePets.clear();
}

CMenuWareHouseProcess::~CMenuWareHouseProcess()
{
	ResetWareHouse();
	m_wareHouseItems.clear();
	m_iMenuIndex = 0;
}

void CMenuWareHouseProcess::InitWareHouse(NetGroup* itemGroup, NetGroup* partnerGroup, NetGroup* petGroup, NetGroup* partnerSkillGroup, NetGroup* petSkillGroup)
{
	CItem* item = NULL;
	for (Index i = C_INDEX(0); i < itemGroup->GetCount(); ++i)
	{
		item = itemGroup->GetChild<CItem>(i);
		if (item && item->GetInWareHouse() && m_wareHouseItems[item->GetBagIndex()] != item->GetNetId())
			m_wareHouseItems[item->GetBagIndex()] = item->GetNetId();
	}

	CPartner* partner = NULL;
	for (Index i = C_INDEX(0); i < partnerGroup->GetCount(); ++i)
	{
		partner = partnerGroup->GetChild<CPartner>(i);
		if (partner && find(m_wareHousePartners.begin(), m_wareHousePartners.end(), partner->GetNetId()) == m_wareHousePartners.end())
			m_wareHousePartners.push_back(partner->GetNetId());
	}

	CSkillGroup *grp = NULL;
	for (Index i = C_INDEX(0); i < partnerSkillGroup->GetCount(); ++i) 
	{
		partner = NULL;
		grp = partnerSkillGroup->GetChild<CSkillGroup>(i);
		if (grp) 
		{
			for (Index j = C_INDEX(0); j < m_wareHousePartners.size(); ++j)
			{
				partner = FindNetObject<CPartner>(m_wareHousePartners[j]);
				if (partner && partner->GetPartner_id() == grp->GetOwner_id())
				{
					partner->SetSkillGroup(grp);
					break;
				}
			}
		}
	}

	CPet* pet = NULL;
	for (Index i = C_INDEX(0); i < petGroup->GetCount(); ++i)
	{
		pet = petGroup->GetChild<CPet>(i);
		if (pet && find(m_wareHousePets.begin(), m_wareHousePets.end(), pet->GetNetId()) == m_wareHousePets.end())
			m_wareHousePets.push_back(pet->GetNetId());
	}

	grp = NULL;
	for (Index i = C_INDEX(0); i < petSkillGroup->GetCount(); ++i) 
	{
		pet = NULL;
		grp = petSkillGroup->GetChild<CSkillGroup>(i);
		if (grp) 
		{
			for (Index j = C_INDEX(0); j < m_wareHousePets.size(); ++j)
			{
				pet = FindNetObject<CPet>(m_wareHousePets[j]);
				if (pet && pet->GetPet_uid() == grp->GetOwner_id())
				{
					pet->SetSkillGroup(grp);
					break;
				}
			}
		}
	}
}

void CMenuWareHouseProcess::OpenWareHouse()
{
	if (gGlobal.g_pCharacter)
	{
		m_iStartWareHouseX = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX;
		m_iStartWareHouseY = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY;
	}
	m_iCountDown = 0;
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenWarehousePanel", String());
}

void CMenuWareHouseProcess::ShowWareHouseItem(int pageNo)
{	
	m_iMenuIndex = pageNo;
	UIAniIcon* pAniIcon = NULL;
	UIStaticText* pText = NULL;
	UIImage* pBlockImg = NULL;
	CItem* item = NULL;
	RPCNetID empty;
	StringA iconName;

	for (UInt i = 0; i < BAG_MAX_WAREHOUSE_ITEM_PER_PAGE; ++i)
	{
		iconName.Format("WarehouseItem%03dIcon", i);
		pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
		iconName.Format("WarehouseItem%03dStack", i);
		pText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("WarehouseItem%03dBlock", i);
		pBlockImg = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		
		if (pAniIcon && pText && pBlockImg)
		{
			UInt index = i + pageNo * BAG_MAX_WAREHOUSE_ITEM_PER_PAGE;
			if (index < (UInt)(gGlobal.g_pCharacter->GetCharNetObject()->GetAvailItemWareHouse() * BAG_ITEM_COUNT) )
				pBlockImg->SetEnable(false);

			if (m_wareHouseItems[index] != empty)
			{
				item = FindNetObject<CItem>(m_wareHouseItems[index]);
				if (item && item->m_data)
				{
					if (item->GetStack() > 1)
					{
						String stack;
						stack.Format(_T("%d"), item->GetStack());
						pText->SetText(stack.c_str());
						pText->Redraw();
						pText->SetEnable(true);
					}
					else
						pText->SetEnable(false);

					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
					if (!pAniIcon->GetAniEnt()->HasAniFile())
					{
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(String(_T("itemSell0000s.ent")).c_str());
					}
					pAniIcon->SetEnable(true);
				}
				else
				{
					pAniIcon->SetEnable(false);
					pText->SetEnable(false);
				}
			}
			else
			{
				pAniIcon->SetEnable(false);
				pText->SetEnable(false);
			}
		}		
	}
}

void CMenuWareHouseProcess::ShowWareHousePartner()
{
	m_iMenuIndex = 2;
	UIAniIcon* pAniIcon = NULL;
	UIStaticText* pNameText = NULL;
	UIStaticText* pLevelText = NULL;
	CPartner* partner = NULL;
	StringA iconName;
	UIImage* pImg = NULL;

	for (Index i = C_INDEX(0); i < PARTNER_MAX_WAREHOUSE; ++i)
	{
		iconName.Format("WarehousePP0%dIcon", i);
		pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
		iconName.Format("WarehousePP0%dName", i);
		pNameText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("WarehousePP0%dLvl", i);
		pLevelText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("WarehousePP0%dBlock", i);
		pImg = gGlobal.pUIManager->FindUIImage(iconName.c_str());

		if (pAniIcon && pNameText && pLevelText && pImg)
		{
			if ( i < m_wareHousePartners.size())
			{
				partner = FindNetObject<CPartner>(m_wareHousePartners[i]);
				if (partner && partner->m_data)
				{
					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(String(partner->m_data->m_basicinfo->mob_faceS).c_str());
					pAniIcon->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);

					String text;
					text.Format(_T("%s(%s)"), partner->GetRaname().c_str(), String(partner->m_data->m_basicinfo->mid_name).c_str());

					pNameText->SetText(text.c_str());
					pNameText->Redraw();
					pNameText->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(pNameText);

					text.Format(_T("%s%d"), gGlobal.GetStringTable()->Get(_T("MSG_SKILL_LEVEL")), partner->GetLevel());
					pLevelText->SetText(text.c_str());
					pLevelText->Redraw();
					pLevelText->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(pLevelText);

					pImg->SetEnable(false);
				}
				else
				{
					pAniIcon->SetEnable(false);
					pNameText->SetEnable(false);
					pLevelText->SetEnable(false);
					pImg->SetEnable(true);
				}
			}
			else
			{
				pAniIcon->SetEnable(false);
				pNameText->SetEnable(false);
				pLevelText->SetEnable(false);
				if ( i < gGlobal.g_pCharacter->GetCharNetObject()->GetAvailPartnerWareHouse())
					pImg->SetEnable(false);
				else
					pImg->SetEnable(true);
			}
		}
	}
}

void CMenuWareHouseProcess::ShowWareHousePet()
{
	m_iMenuIndex = 3;
	UIAniIcon* pAniIcon = NULL;
	UIStaticText* pNameText = NULL;
	UIStaticText* pLevelText = NULL;
	CPet* pet = NULL;
	StringA iconName;
	UIImage* pImg = NULL;

	for (Index i = C_INDEX(0); i < PARTNER_MAX_WAREHOUSE; ++i)
	{
		iconName.Format("WarehousePP0%dIcon", i);
		pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
		iconName.Format("WarehousePP0%dName", i);
		pNameText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("WarehousePP0%dLvl", i);
		pLevelText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("WarehousePP0%dBlock", i);
		pImg = gGlobal.pUIManager->FindUIImage(iconName.c_str());

		if (pAniIcon && pNameText && pLevelText && pImg)
		{
			if ( i < m_wareHousePets.size())
			{				
				pet = FindNetObject<CPet>(m_wareHousePets[i]);
				if (pet)
				{
					const PetData* pData = gGlobal.m_resource->GetPetData(pet->GetPet_id());
					if (pData)
					{
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(String(pData->pet_faceS).c_str());
						pAniIcon->SetEnable(true);
						gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);

						String text;
						text.Format(_T("%s(%s)"), pet->GetName().c_str(), String(pData->pet_name).c_str());

						pNameText->SetText(text.c_str());
						pNameText->Redraw();
						pNameText->SetEnable(true);
						gGlobal.pUIManager->AddDirtyUIItem(pNameText);

						text.Format(_T("%s%d"), gGlobal.GetStringTable()->Get(_T("MSG_SKILL_LEVEL")), pet->GetLevel());
						pLevelText->SetText(text.c_str());
						pLevelText->Redraw();
						pLevelText->SetEnable(true);
						gGlobal.pUIManager->AddDirtyUIItem(pLevelText);

						pImg->SetEnable(false);
					}
				}
				else
				{
					pAniIcon->SetEnable(false);
					pNameText->SetEnable(false);
					pLevelText->SetEnable(false);
					pImg->SetEnable(true);
				}
			}
			else
			{
				pAniIcon->SetEnable(false);
				pNameText->SetEnable(false);
				pLevelText->SetEnable(false);
				if ( i < gGlobal.g_pCharacter->GetCharNetObject()->GetAvailPetWareHouse())
					pImg->SetEnable(false);
				else
					pImg->SetEnable(true);
			}
		}
	}
}

void CMenuWareHouseProcess::PutToWareHouse(int desIndex, int srcIndex, int stack)
{
	if (gGlobal.m_pBag == NULL || gGlobal.g_pCharacter == NULL || gGlobal.g_pCharacter->GetCharNetObject() == NULL)
		return;

	if (gGlobal.g_pTradingProcess && gGlobal.g_pTradingProcess->hasTarget())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CANNOT_TRADING")));
		return;
	}

	if (desIndex >= (int)(gGlobal.g_pCharacter->GetCharNetObject()->GetAvailItemWareHouse() * BAG_ITEM_COUNT) && desIndex != 255)
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_FULL")));
		return;
	}

	CItem* item = gGlobal.m_pBag->GetItem(srcIndex);
	if (item && item->m_data)
	{
		if (item->m_data->item_type == ITEMTYPE_SCRIPT || item->m_data->item_type == ITEMTYPE_BUSINESS_ITEM || item->m_data->item_type == ITEMTYPE_BUSINESS_MONEY)
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_PUT_IN_WRONG_ITEM")));
			return;
		}

		if (item->GetStack() < stack)
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_ITEM_STACK_ERROR")));
			return;
		}
		gGlobal.g_pItemProcess->SetIconOff(srcIndex % BAG_ITEM_COUNT , false);
		gGlobal.m_pBag->PutToWareHouse(gGlobal.pPeerProxy, gGlobal.g_context, item->GetItem_uid(), stack, desIndex);
	}
	
}

void CMenuWareHouseProcess::ResetWareHouse()
{
	RPCNetID empty;
	CItem* item = NULL;
	for (Index i = C_INDEX(0); i < BAG_MAX_WAREHOUSE_ITEM; ++i)
	{
		if (m_wareHouseItems[i] != empty)
		{
			item = FindNetObject<CItem>(m_wareHouseItems[i]);
			if (item)
				DeleteNetObject(item);
			m_wareHouseItems[i] = empty;
		}
	}
	
	CPartner* partner = NULL;
	for (Index i = C_INDEX(0); i < m_wareHousePartners.size(); ++i)
	{
		partner = FindNetObject<CPartner>(m_wareHousePartners[i]);
		if(partner)
			DeleteNetObject(partner);
	}
	m_wareHousePartners.clear();

	CPet* pet = NULL;
	for (Index i = C_INDEX(0); i < m_wareHousePets.size(); ++i)
	{
		pet = FindNetObject<CPet>(m_wareHousePets[i]);
		if(pet)
			DeleteNetObject(pet);
	}
	m_wareHousePets.clear();
	m_iCountDown = 0;
}

void CMenuWareHouseProcess::CloseWareHouse()
{
	m_iStartWareHouseX = 0;
	m_iStartWareHouseY = 0;
	//ResetWareHouse();
	m_iCountDown = GetTickCount();
	gGlobal.g_pCharacter->GetCharNetObject()->CloseWareHouse(gGlobal.pPeerProxy, gGlobal.g_context);
}

void CMenuWareHouseProcess::RemoveItem(CItem* item, bool needDelete)
{
	if (item)
	{
		if (item->GetBagIndex() < BAG_MAX_WAREHOUSE_ITEM && m_wareHouseItems.size() < item->GetBagIndex() 
			&& m_wareHouseItems[item->GetBagIndex()] == item->GetNetId())
		{
			m_wareHouseItems[item->GetBagIndex()] = RPCNetID();
			if (needDelete)
				DeleteNetObject(item);
		}
		else
		{
			for (Index i = C_INDEX(0); i < BAG_MAX_WAREHOUSE_ITEM; ++i)
			{
				if (m_wareHouseItems[i] == item->GetNetId())
				{
					m_wareHouseItems[i] = RPCNetID();
					if (needDelete)
						DeleteNetObject(item);
					break;
				}
			}	
		}
	}
}

void CMenuWareHouseProcess::AddItem(CItem* item)
{
	if (item && item->GetInWareHouse() && m_wareHouseItems[item->GetBagIndex()] != item->GetNetId())
		m_wareHouseItems[item->GetBagIndex()] = item->GetNetId();
}

void CMenuWareHouseProcess::UpdateWareHouse()
{	
	switch(m_iMenuIndex)
	{
	case 0:
	case 1:
		ShowWareHouseItem(m_iMenuIndex);
		break;
	case 2:
		ShowWareHousePartner();
		break;
	case 3:
		ShowWareHousePet();
		break;
	}	
}

int CMenuWareHouseProcess::GetMaxStack(int index)
{
	if (index >= 0 && index < (int)m_wareHouseItems.size() && m_wareHouseItems[index] != RPCNetID())
	{
		CItem* item = FindNetObject<CItem>(m_wareHouseItems[index]);
		if (item)
			return (int)item->GetStack();
	}
	return 0;
}

void CMenuWareHouseProcess::GetFromWareHouse(int desIndex, int srcIndex, int stack)
{
	if (gGlobal.g_pItemProcess->GetBagPage() > 0)
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_PUT_IN_WRONG_BAG")));
		return;
	}

	if (gGlobal.g_pTradingProcess && gGlobal.g_pTradingProcess->hasTarget())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CANNOT_TRADING")));
		return;
	}

	if (desIndex > 24 && desIndex != 255)
	{
		return;
	}

	if (srcIndex >= (int)m_wareHouseItems.size())
	{
		return;
	}

	if (!gGlobal.m_pBag)
	{
		return;
	}

	CItem* tempItem = gGlobal.m_pBag->GetItem(desIndex);
	if (tempItem && tempItem->IsGiven())
	{
		return;
	}

	CItem* item = FindNetObject<CItem>(m_wareHouseItems[srcIndex]);
	if (item && item->m_data)
	{
		if (item->m_data->item_type == ITEMTYPE_SCRIPT || item->m_data->item_type == ITEMTYPE_BUSINESS_ITEM || item->m_data->item_type == ITEMTYPE_BUSINESS_MONEY)
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_PUT_IN_WRONG_ITEM")));
			return;
		}

		if (item->GetStack() < stack)
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_ITEM_STACK_ERROR")));
			return;
		}

		StringA iconName;
		iconName.Format("WarehouseItem%03dIcon", srcIndex % BAG_MAX_WAREHOUSE_ITEM_PER_PAGE);
		UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
		if (pAniIcon)
			pAniIcon->SetEnable(false);
		
		gGlobal.m_pBag->GetFromWareHouse(gGlobal.pPeerProxy, gGlobal.g_context, item->GetItem_uid(), stack, desIndex);
	}
}

bool CMenuWareHouseProcess::CheckCanSwap(int itemBagIndex, int wareHouseIndex)
{
	CItem* itemBagItem = gGlobal.m_pBag->GetItem(itemBagIndex);
	CItem* wareHouseItem = FindNetObject<CItem>(m_wareHouseItems[wareHouseIndex]);

	if (itemBagItem && wareHouseItem)
	{
		if (itemBagItem->GetItem_id() != wareHouseItem->GetItem_id() && !itemBagItem->IsGiven())
			return true;
	}
	return false;
}

bool CMenuWareHouseProcess::AvailSlot(int wareHouseIndex)
{
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject() &&
		wareHouseIndex < gGlobal.g_pCharacter->GetCharNetObject()->GetAvailItemWareHouse() * BAG_ITEM_COUNT)
		return true;
	return false;
}

void CMenuWareHouseProcess::ShowItemDetail()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("WarehousePanel");
	if (pPanel && m_wareHouseItems.size() > 0 && (m_iMenuIndex == 0 || m_iMenuIndex == 1) )
	{
		Int rposX = gGlobal.pUIManager->GetCursorX() - pPanel->GetX() - 28;  
		Int rposY = gGlobal.pUIManager->GetCursorY() - pPanel->GetY() - 63;
		if (rposX > 0 && rposY > 0 && rposX < (Int)pPanel->GetWidth() && rposY < (Int)pPanel->GetHeight())
		{
			UInt index = rposX / 41 + 6 * (rposY / 41) + m_iMenuIndex * BAG_MAX_WAREHOUSE_ITEM_PER_PAGE;
			if (index >= m_wareHouseItems.size())
				return;

			CItem* item = FindNetObject<CItem>(m_wareHouseItems[index]);						
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
	}
}

void CMenuWareHouseProcess::ShowPartnerDetail()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("WarehousePanel");
	if (pPanel && m_wareHousePartners.size() > 0 && m_iMenuIndex == 2 )
	{
		Int rposX = gGlobal.pUIManager->GetCursorX() - pPanel->GetX() - 30;  
		Int rposY = gGlobal.pUIManager->GetCursorY() - pPanel->GetY() - 63;
		if (rposX > 0 && rposY > 0 && rposX < 38 && rposY < (41 * 8))
		{
			UInt index = rposY / 41;
			if (index >= m_wareHousePartners.size())
				return;

			CPartner* partner = FindNetObject<CPartner>(m_wareHousePartners[index]);

			if (partner)
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
	
				UInt16Vector skillVector;

				if (partner->m_pSkillGrp)
				{
					for (Index i = C_INDEX(0); i < partner->m_pSkillGrp->GetCount(); ++i)
					{
						CSkill* pSkill = partner->m_pSkillGrp->GetChild<CSkill>(i);
						if (pSkill)
							skillVector.push_back(pSkill->GetSkill_id());
					}
				}

				gGlobal.g_pPartnerProcess->ShowPartnerDetail(partner, skillVector);
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

void CMenuWareHouseProcess::ShowPetDetail()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("WarehousePanel");
	if (pPanel && m_wareHousePartners.size() > 0 && m_iMenuIndex == 3 )
	{
		Int rposX = gGlobal.pUIManager->GetCursorX() - pPanel->GetX() - 30;  
		Int rposY = gGlobal.pUIManager->GetCursorY() - pPanel->GetY() - 63;
		if (rposX > 0 && rposY > 0 && rposX < 38 && rposY < (41 * 8))
		{
			UInt index = rposY / 41;
			if (index >= m_wareHousePets.size())
				return;

			CPet* pet = FindNetObject<CPet>(m_wareHousePets[index]);

			if (pet)
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
	
				UInt16Vector skillVector;

				if (pet->m_pSkillGrp)
				{
					for (Index i = C_INDEX(0); i < pet->m_pSkillGrp->GetCount(); ++i)
					{
						CSkill* pSkill = pet->m_pSkillGrp->GetChild<CSkill>(i);
						if (pSkill)
							skillVector.push_back(pSkill->GetSkill_id());
					}
				}

				gGlobal.g_pMenuPetProcess->ShowPetDetail(pet, skillVector);
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

void CMenuWareHouseProcess::DisappearWareHouse()
{
	//for disappear
	if (m_iStartWareHouseX >  0 || m_iStartWareHouseY > 0)
	{
		BlockPos currPos = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();
		BlockPos center;
		center.PosX = m_iStartWareHouseX;
		center.PosY = m_iStartWareHouseY;
		PixelDirection dummy;

		if (QGLMath::CalculateBlockPosDistance(currPos, center, dummy) > 400)
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseWarehousePanel", String());
		}
	}

	if (m_iCountDown > 0)
	{
		UInt nowTime = GetTickCount() - m_iCountDown;
		if (nowTime > 60000) // 60 * 1000 = 1 min
			ResetWareHouse();
	}
}

void CMenuWareHouseProcess::MoveItem(int srcIndex, int desIndex)
{
	if (gGlobal.g_pTradingProcess && gGlobal.g_pTradingProcess->hasTarget())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CANNOT_TRADING")));
		return;
	}

	if (srcIndex > (int)m_wareHouseItems.size() || desIndex > (int)m_wareHouseItems.size())
	{
		return;
	}
	CItem* item = FindNetObject<CItem>(m_wareHouseItems[srcIndex]);
	if (item && item->m_data)
	{
		StringA iconName;
		iconName.Format("WarehouseItem%03dIcon", srcIndex % BAG_MAX_WAREHOUSE_ITEM_PER_PAGE);
		UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
		if (pAniIcon)
			pAniIcon->SetEnable(false);

		gGlobal.m_pBag->WareHouseItemMove(gGlobal.pPeerProxy, gGlobal.g_context, srcIndex, desIndex);
	}
}

void CMenuWareHouseProcess::ShowResult(UInt processType, UInt msgIndex)
{
	switch(msgIndex)
	{
	case WAREHOUSE_SUCCESS:
		break;
	case WAREHOUSE_RUNNING_SCIRPT:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_RUN_SCRIPT")));
		break;
	case WAREHOUSE_QUEST_ITEM:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_PUT_IN_WRONG_ITEM")));
		break;
	case WAREHOUSE_FULL:
		{
			switch(processType)
			{
			case WAREHOUSE_PROCESS_PUT_ITEM:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_FULL")));
				break;
			case WAREHOUSE_PROCESS_GET_ITEM:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CURRENT_FULL")));
				break;
			case WAREHOUSE_PROCESS_PUT_PARTNER:
				{
					String temp;
					temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_PARTNER_FULL")), 
						gGlobal.g_pCharacter->GetCharNetObject()->GetAvailPartnerWareHouse());
					gGlobal.PrintPlayerMessage(temp.c_str());
				}
				break;
			case WAREHOUSE_PROCESS_GET_PARTNER:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CURRENT_PARTNER_FULL")));
				break;
			case WAREHOUSE_PROCESS_PUT_PET:
				{
					String temp;
					temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_PET_FULL")), 
						gGlobal.g_pCharacter->GetCharNetObject()->GetAvailPetWareHouse());
					gGlobal.PrintPlayerMessage(temp.c_str());
				}
				break;
			case WAREHOUSE_PROCESS_GET_PET:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CURRENT_PET_FULL")));
				break;
			}				
		}
		break;
	case WAREHOUSE_NOT_IN_MAP:
	case WAREHOUSE_ITEM_STACK_WRONG:
	case WAREHOUSE_NO_PARTNER:
	case WAREHOUSE_NO_ITEM:
	case WAREHOUSE_NO_PET:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_ERROR")));
		break;
	case WAREHOUSE_PUT_IN_UING_STATE:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_PUT_IN_USING_STATE")));
		break;
	case WAREHOUSE_MOVE_SUCCESS:
		break;
	case WAREHOUSE_CANNOT_TRADING:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CANNOT_TRADING")));
		break;
	}	
}

void CMenuWareHouseProcess::PutPartnerToWareHouse(UInt32 partnerID)
{
	if (gGlobal.g_pCharacter == NULL || gGlobal.g_pCharacter->GetCharNetObject() == NULL)
		return;

	if (gGlobal.g_pTradingProcess && gGlobal.g_pTradingProcess->hasTarget())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CANNOT_TRADING")));
		return;
	}

	if (m_wareHousePartners.size() >= PARTNER_MAX_WAREHOUSE || 
		m_wareHousePartners.size() >= gGlobal.g_pCharacter->GetCharNetObject()->GetAvailPartnerWareHouse())
	{
		String temp;
		temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_PARTNER_FULL")), gGlobal.g_pCharacter->GetCharNetObject()->GetAvailPartnerWareHouse());
		gGlobal.PrintPlayerMessage(temp.c_str());
		return;
	}

	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(partnerID);
	if (partner )
	{
		if (partner->GetState() == CPartner::STATE_FIGHT)
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_PUT_IN_USING_STATE")));
			return;
		}

		if (partner->GetInWareHouse() == 0 && !partner->IsGiven())
		{
			partner->SetGiven(true);
			gGlobal.g_pPartnerGroup->PutToWareHouse(gGlobal.pPeerProxy, gGlobal.g_context, partnerID);
		}
	}
}

void CMenuWareHouseProcess::AddPartner(CPartner* partner)
{
	if (partner && find(m_wareHousePartners.begin(), m_wareHousePartners.end(), partner->GetNetId()) == m_wareHousePartners.end())
			m_wareHousePartners.push_back(partner->GetNetId());
}

void CMenuWareHouseProcess::GetPartnerFromWareHouse(int wareHouseIndex)
{
	if (gGlobal.g_pCharacter == NULL || gGlobal.g_pCharacter->GetCharNetObject() == NULL)
		return;

	if (gGlobal.g_pTradingProcess && gGlobal.g_pTradingProcess->hasTarget())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CANNOT_TRADING")));
		return;
	}

	if (wareHouseIndex >= (int)m_wareHousePartners.size() )
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_ERROR")));
		return;
	}
	
	if (gGlobal.g_pPartnerGroup->GetCount() >= gGlobal.g_pCharacter->GetCharNetObject()->GetMaxPartner())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CURRENT_PARTNER_FULL")));
		return;
	}

	CPartner* partner = FindNetObject<CPartner>(m_wareHousePartners[wareHouseIndex]);
	if (partner && partner->GetInWareHouse() == 1 && !partner->IsGiven())
	{
		StringA temp;
		temp.Format("WarehousePP%02dIcon", wareHouseIndex);
		UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(temp.c_str());
		if (pAniIcon)
			pAniIcon->SetEnable(false);
		gGlobal.g_pPartnerGroup->GetFromWareHouse(gGlobal.pPeerProxy, gGlobal.g_context, partner->GetPartner_id());
	}
}

void CMenuWareHouseProcess::RemovePartner(CPartner* partner, bool needDelete)
{
	if (partner == NULL)
		return;

	RPCNetIDVector::iterator itr = remove(m_wareHousePartners.begin(), m_wareHousePartners.end(), partner->GetNetId());
	if (itr != m_wareHousePartners.end())
		m_wareHousePartners.erase(itr);
	if (needDelete)
		DeleteNetObject(partner);
}

CPartner* CMenuWareHouseProcess::GetPartner(UInt32 partnerID)
{
	for (Index i = C_INDEX(0); i < m_wareHousePartners.size(); ++i)
	{
		CPartner* partner = FindNetObject<CPartner>(m_wareHousePartners[i]);
		if (partner && partner->GetPartner_id() == partnerID)
			return partner;
	}
	return NULL;
}

void CMenuWareHouseProcess::PutPetToWareHouse(UInt32 petUID)
{
	if (gGlobal.g_pCharacter == NULL || gGlobal.g_pCharacter->GetCharNetObject() == NULL)
		return;

	if (gGlobal.g_pTradingProcess && gGlobal.g_pTradingProcess->hasTarget())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CANNOT_TRADING")));
		return;
	}

	if (m_wareHousePets.size() >= PET_MAX_WAREHOUSE || 
		m_wareHousePets.size() >= gGlobal.g_pCharacter->GetCharNetObject()->GetAvailPetWareHouse())
	{
		String temp;
		temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_PET_FULL")), gGlobal.g_pCharacter->GetCharNetObject()->GetAvailPetWareHouse());
		gGlobal.PrintPlayerMessage(temp.c_str());
		return;
	}

	CPet* pet = gGlobal.g_PetLst->GetPet(petUID);
	if (pet)
	{
		if (pet->GetState() == PET_STATE_TYPE_USE)
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_PUT_IN_USING_STATE")));
			return;
		}

		if (pet->GetInWareHouse() == 0 && !pet->IsGiven())
		{
			pet->SetGiven(true);
			gGlobal.g_PetLst->PutToWareHouse(gGlobal.pPeerProxy, gGlobal.g_context, petUID);
		}
	}
}

void CMenuWareHouseProcess::AddPet(CPet* pet)
{
	if (pet && find(m_wareHousePets.begin(), m_wareHousePets.end(), pet->GetNetId()) == m_wareHousePets.end())
			m_wareHousePets.push_back(pet->GetNetId());
}

void CMenuWareHouseProcess::GetPetFromWareHouse(int wareHouseIndex)
{
	if (gGlobal.g_pCharacter == NULL || gGlobal.g_pCharacter->GetCharNetObject() == NULL)
		return;

	if (gGlobal.g_pTradingProcess && gGlobal.g_pTradingProcess->hasTarget())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CANNOT_TRADING")));
		return;
	}

	if (wareHouseIndex >= (int)m_wareHousePets.size() )
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_ERROR")));
		return;
	}
	
	if (gGlobal.g_PetLst->GetCount() >= gGlobal.g_pCharacter->GetCharNetObject()->GetMaxPet())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WAREHOUSE_CURRENT_PET_FULL")));
		return;
	}

	CPet* pet = FindNetObject<CPet>(m_wareHousePets[wareHouseIndex]);
	if (pet && pet->GetInWareHouse() == 1 && !pet->IsGiven())
	{
		StringA temp;
		temp.Format("WarehousePP%02dIcon", wareHouseIndex);
		UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(temp.c_str());
		if (pAniIcon)
			pAniIcon->SetEnable(false);
		gGlobal.g_PetLst->GetFromWareHouse(gGlobal.pPeerProxy, gGlobal.g_context, pet->GetPet_uid());
	}
}

void CMenuWareHouseProcess::RemovePet(CPet* pet, bool needDelete)
{
	if (pet == NULL)
		return;

	RPCNetIDVector::iterator itr = remove(m_wareHousePets.begin(), m_wareHousePets.end(), pet->GetNetId());
	if (itr != m_wareHousePets.end())
		m_wareHousePets.erase(itr);
	if (needDelete)
		DeleteNetObject(pet);
}

CPet* CMenuWareHouseProcess::GetPet(UInt32 petUID)
{
	for (Index i = C_INDEX(0); i < m_wareHousePets.size(); ++i)
	{
		CPet* pet = FindNetObject<CPet>(m_wareHousePets[i]);
		if (pet && pet->GetPet_uid() == petUID)
			return pet;
	}
	return NULL;
}

void CMenuWareHouseProcess::JpackWareHouse(UInt8 itemPage)
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	UIPanel* pPanelTrading = gGlobal.pUIManager->FindUIPanel("TradingPanel");
	UIPanel* pPanelGiven = gGlobal.pUIManager->FindUIPanel("GivenItemPanel");
	UIPanel* pPanelHawker = gGlobal.pUIManager->FindUIPanel("HawkerOwnerPanel"); 
	if(pchar)
	{
		if (pchar->GetAction() == CCharacter::ST_HAWK || pPanelHawker)
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_JPACK_NOT_HAWK")));
		else if(pPanelTrading)
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_JPACK_NOT_TARING")));
		else if(pPanelGiven)
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_JPACK_NOT_GIVEN")));
		else if (m_iMenuIndex >= 2)
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_JPACK_NOT_PETORPARTNER")));
		else
		{
			if (gGlobal.m_pBag)
				gGlobal.m_pBag->CleanWarehouse(gGlobal.pPeerProxy,gGlobal.g_context,itemPage);
		}
	}
}
