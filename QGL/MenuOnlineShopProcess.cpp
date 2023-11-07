//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuOnlineShopProcess.h"

#include "Global.h"
#include "LuaPush.h"
#include "Configure.h"
#include "Common/CallLuaFunc.h"
#include "Resource/ResItem.h"
#include "Resource/ResMob.h"
#include "Resource/ResPet.h"
#include "Resource/ClientResManager.h"
#include "Proxy/Logic/Character.h"
#include "QGL/CharacterControl.h"
#include "Proxy/Logic/ChargeShop.h"
#include "Proxy/Logic/ChargeShopItem.h"
#include "MenuPartnerProcess.h"
#include "MenuItemProcess.h"
#include "MenuPetProcess.h"
#include "Proxy/Service/WorldService.h"
#include "Proxy/Service/MapService.h"
#include "QGLApplication.h"

#define MAX_ITEM_PER_PAGE 10
#define MAX_DISCOUNT_ITEM 3
#define MAX_LIMITED_ITEM 1
#define ONLINESHOP_SUPPORT_STACK_LIMIT 50000

CMenuOnlineShopProcess::CMenuOnlineShopProcess()
{
	m_iPage = 0;
	m_iSelItemIndex = -1;
	m_iSelLimitedItemIndex = -1;
	m_iSelDiscountItemIndex = -1;
	m_iShowIndex = HOTSALE;
	m_pChargeShop = NULL;
	m_iStack = 0;

	m_hotSaleItem.clear();
	m_mixItem.clear();
	m_asscItem.clear();
	m_petItem.clear();
	m_partnerItem.clear();
	m_clothItem.clear();
	m_otherItem.clear();
	m_limitedItem.clear();
	m_discountItem.clear();
	m_pSelectedItem = NULL;
	m_iUpdateYuanTimer = 0;
}

void CMenuOnlineShopProcess::ShowHotSale(UInt8 pageNo)
{
	m_iShowIndex = HOTSALE;
	ShowItem(m_hotSaleItem, pageNo);
}
	
void CMenuOnlineShopProcess::ShowMix(UInt8 pageNo)
{
	m_iShowIndex = MIX;
	ShowItem(m_mixItem, pageNo);
}
	
void CMenuOnlineShopProcess::ShowAssc(UInt8 pageNo)
{
	m_iShowIndex = ASSC;
	ShowItem(m_asscItem, pageNo);
}
	
void CMenuOnlineShopProcess::ShowPet(UInt8 pageNo)
{
	m_iShowIndex = PET;
	ShowItem(m_petItem, pageNo);
}

void CMenuOnlineShopProcess::ShowDiamond(UInt8 pageNo)
{
	m_iShowIndex = DIAMOND;
	ShowItem(m_DiamondItem, pageNo);
}

void CMenuOnlineShopProcess::ShowBook(UInt8 pageNo)
{
	m_iShowIndex = BOOK;
	ShowItem(m_BookItem, pageNo);
}

void CMenuOnlineShopProcess::ShowFireworks(UInt8 pageNo)
{
	m_iShowIndex = FIREWORKS;
	ShowItem(m_FireWorkItem, pageNo);
}

void CMenuOnlineShopProcess::ShowCap(UInt8 pageNo)
{
	m_iShowIndex = CAP;
	ShowItem(m_CapItem, pageNo);
}

void CMenuOnlineShopProcess::ShowMould(UInt8 pageNo)
{
	m_iShowIndex = MOULD;
	ShowItem(m_MouldItem, pageNo);
}

void CMenuOnlineShopProcess::ShowPartner(UInt8 pageNo)
{
	m_iShowIndex = PARTNER;
	ShowItem(m_partnerItem, pageNo);
}
	
void CMenuOnlineShopProcess::ShowCloth(UInt8 pageNo)
{
	m_iShowIndex = CLOTH;
	ShowItem(m_clothItem, pageNo);
}
	
void CMenuOnlineShopProcess::ShowOther(UInt8 pageNo)
{
	m_iShowIndex = OTHER;
	ShowItem(m_otherItem, pageNo);
}
	
void CMenuOnlineShopProcess::ShowLimitedItem()
{
	UIAniIcon* itemIcon = NULL;
	UIStaticText* itemName = NULL;
	UIStaticText* itemPrice = NULL;
	UIStaticText* itemPriceTitle = NULL;
	UIStaticText* itemRemain = NULL;
	UIButton* itemButton = NULL;
	UIImage* itemBack = NULL;

	itemIcon = gGlobal.pUIManager->FindUIAniIcon("OnlineShopLimitedItemIcon");
	itemName = gGlobal.pUIManager->FindUIStaticText("OnlineShopLimitedItemName");
	itemPrice = gGlobal.pUIManager->FindUIStaticText("OnlineShopLimitedItemCost");
	itemPriceTitle = gGlobal.pUIManager->FindUIStaticText("OnlineShopLimitedItemCostTitle");
	itemButton = gGlobal.pUIManager->FindUIButton("OnlineShopLimitedItem");
	itemRemain = gGlobal.pUIManager->FindUIStaticText("OnlineShopLimitedItemCount");
	itemBack = gGlobal.pUIManager->FindUIImage("OnlineShopLimitedBackIcon");

	if (itemIcon && itemName && itemPrice && itemPriceTitle && itemButton && itemRemain && itemBack)
	{
		if (m_limitedItem.size())
		{
			CChargeShopItem* item = m_limitedItem[0];
			if (item)
			{
				String iconName;
				Color32 textColor;
				String text;
				if (item->GetItemType() == CHARGE_ITEM_TYPE_ITEM)
				{
					const ItemData* pItemData = gGlobal.m_resource->GetItemData(item->GetItemId());
					if (pItemData)
					{
						text = _T("");
						if (item->GetItemName().Equals(text.c_str()))
							text = String(pItemData->name);
						else
							text = item->GetItemName();

						if (pItemData->item_TimeLimit > 0)
							text.Format(_T("%s(%d)"), text.c_str(), pItemData->item_TimeLimit);

						
						switch (pItemData->color)
						{
						case ItemColorIndex_Grey:	textColor = ItemTextColor[ItemColorIndex_Grey];			break;
						case ItemColorIndex_LightGreen:	textColor = ItemTextColor[ItemColorIndex_LightGreen];	break;
						case ItemColorIndex_LightBlue:	textColor = ItemTextColor[ItemColorIndex_LightBlue];	break;
						case ItemColorIndex_LightPurple:	textColor = ItemTextColor[ItemColorIndex_LightPurple];	break;
						case ItemColorIndex_LightGolden:	textColor = ItemTextColor[ItemColorIndex_LightGolden];	break;
						case ItemColorIndex_GodGolden:	textColor = ItemTextColor[ItemColorIndex_GodGolden];	break;
						}
						itemName->SetColor(textColor);
						itemName->SetText(text.c_str());
						itemName->Redraw();
						itemName->SetEnable(true);
						iconName = String(pItemData->icon2);

						itemBack->SetEnable(false);
						if (itemIcon->GetOffsetX() != itemBack->GetOffsetX() || itemIcon->GetOffsetY() != itemIcon->GetOffsetY())
							itemIcon->SetOffsetXY(itemBack->GetOffsetX(), itemBack->GetOffsetY());
					}
				}
				else if (item->GetItemType() == CHARGE_ITEM_TYPE_PARTNER)
					{
						MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(item->GetItemId());
						if (mobTemplate)
						{
							textColor = GetRareColor(mobTemplate->m_basicinfo->rare);
							
							text = _T("");
							if (item->GetItemName().Equals(text.c_str()))
								text = String(mobTemplate->m_basicinfo->mid_name);
							else
								text = item->GetItemName();

							if (mobTemplate->m_basicinfo->TimeLimit > 0)
								text.Format(_T("%s(%d)"), text.c_str(), mobTemplate->m_basicinfo->TimeLimit);

							itemName->SetText(text.c_str());
							itemName->SetColor(textColor);
							itemName->Redraw();
							itemName->SetEnable(true);

							iconName = String(mobTemplate->m_basicinfo->mob_faceS);
							
							itemBack->SetEnable(true);
							if (itemIcon->GetOffsetX() == itemBack->GetOffsetX() || itemIcon->GetOffsetY() == itemIcon->GetOffsetY())
								itemIcon->SetOffsetXY(itemBack->GetOffsetX() + 2, itemBack->GetOffsetY() + 2);

						}
					}
					else if (item->GetItemType() == CHARGE_ITEM_TYPE_PET)
					{
						const PetData* pPetData = gGlobal.m_resource->GetPetData(item->GetItemId());
						if (pPetData)
						{
							textColor = GetRareColor(pPetData->pet_rare);
							
							text = _T("");
							if (item->GetItemName().Equals(text.c_str()))
								text = String(pPetData->pet_name);
							else
								text = item->GetItemName();

							if (pPetData->pet_TimeLimit > 0)
								text.Format(_T("%s(%d)"), text.c_str(), pPetData->pet_TimeLimit);

							itemName->SetText(text.c_str());
							itemName->SetColor(textColor);
							itemName->Redraw();
							itemName->SetEnable(true);					
							iconName = String(pPetData->pet_faceS);

							itemBack->SetEnable(true);
							if (itemIcon->GetOffsetX() == itemBack->GetOffsetX() || itemIcon->GetOffsetY() == itemIcon->GetOffsetY())
								itemIcon->SetOffsetXY(itemBack->GetOffsetX() + 2, itemBack->GetOffsetY() + 2);
						}
					}

					itemIcon->ClearAniData();
					itemIcon->SetAnimationData(iconName.c_str());
					itemIcon->SetEnable(true);

					itemPriceTitle->SetEnable(true);

					text.Format(_T("%d"), item->GetRemain());
					itemRemain->SetText(text.c_str());
					itemRemain->Redraw();
					itemRemain->SetEnable(true);

					text.Format(_T("%d"), item->GetYuanBao());
					itemPrice->SetText(text.c_str());
					itemPrice->Redraw();
					itemPrice->SetEnable(true);
			}
		}
		else
		{
			itemIcon->SetEnable(false);
			itemName->SetEnable(false);
			itemPrice->SetEnable(false);
			itemPriceTitle->SetEnable(false);
			itemButton->SetEnable(false);
			itemRemain->SetEnable(false);
		}

	}
}
	
void CMenuOnlineShopProcess::ShowDiscountItem()
{
	UIAniIcon* itemIcon = NULL;
	UIStaticText* itemName = NULL;
	UIStaticText* itemOrgPrice = NULL;
	UIStaticText* itemDiscountPrice = NULL;
	UIStaticText* itemOrgPriceTitle = NULL;
	UIStaticText* itemDiscountPriceTitle = NULL;
	UIButton* itemButton = NULL;
	UIImage* itemBack = NULL;

	for (Index i = C_INDEX(0); i < MAX_DISCOUNT_ITEM; ++i)
	{
		StringA textA;
		String text;
		String modelName;

		textA.Format("OnlineShopDiscountItem0%d", i);
		itemButton = gGlobal.pUIManager->FindUIButton(textA.c_str());

		textA.Format("OnlineShopDiscountItem0%dName", i);
		itemName = gGlobal.pUIManager->FindUIStaticText(textA.c_str());

		textA.Format("OnlineShopDiscountItem0%dTitle", i);
		itemDiscountPriceTitle = gGlobal.pUIManager->FindUIStaticText(textA.c_str());

		textA.Format("OnlineShopDiscountItem0%dCost", i);
		itemDiscountPrice = gGlobal.pUIManager->FindUIStaticText(textA.c_str());

		textA.Format("OnlineShopDiscountItem0%dOrgTitle", i);
		itemOrgPriceTitle = gGlobal.pUIManager->FindUIStaticText(textA.c_str());

		textA.Format("OnlineShopDiscountItem0%dOrgCost", i);
		itemOrgPrice = gGlobal.pUIManager->FindUIStaticText(textA.c_str());

		textA.Format("OnlineShopDiscountItem0%dIcon", i);
		itemIcon = gGlobal.pUIManager->FindUIAniIcon(textA.c_str());

		textA.Format("OnlineShopDiscountBack0%dIcon", i);
		itemBack = gGlobal.pUIManager->FindUIImage(textA.c_str());

		bool allDisable = true;
		if (itemButton && itemName && itemDiscountPriceTitle && itemBack &&
			itemDiscountPrice && itemOrgPriceTitle && itemOrgPrice && itemIcon)
		{
			if (i < m_discountItem.size() )
			{
				CChargeShopItem* item = m_discountItem[i];
				if (item )
				{
					if (item->GetItemType() == CHARGE_ITEM_TYPE_ITEM)
					{
						const ItemData* pItemData = gGlobal.m_resource->GetItemData(item->GetItemId());
						if (pItemData)
						{
							allDisable = false;
							text = _T("");
							if (item->GetItemName().Equals(text.c_str()))
								text = String(pItemData->name);
							else
								text = item->GetItemName();

							if (pItemData->item_TimeLimit > 0)
								text.Format(_T("%s(%d)"), text.c_str(), pItemData->item_TimeLimit);

							Color32 textColor = TextColor[ColorIndex_White];
							switch (pItemData->color)
							{
							case ItemColorIndex_Grey:	textColor = ItemTextColor[ItemColorIndex_Grey];			break;
							case ItemColorIndex_LightGreen:	textColor = ItemTextColor[ItemColorIndex_LightGreen];	break;
							case ItemColorIndex_LightBlue:	textColor = ItemTextColor[ItemColorIndex_LightBlue];	break;
							case ItemColorIndex_LightPurple:	textColor = ItemTextColor[ItemColorIndex_LightPurple];	break;
							case ItemColorIndex_LightGolden:	textColor = ItemTextColor[ItemColorIndex_LightGolden];	break;
							case ItemColorIndex_GodGolden:	textColor = ItemTextColor[ItemColorIndex_GodGolden];	break;
							}
							itemName->SetColor(textColor);
							itemName->SetText(text.c_str());
							itemName->Redraw();
							itemName->SetEnable(true);

							modelName = String(pItemData->icon2);

							itemBack->SetEnable(false);

							if (itemIcon->GetOffsetX() != itemBack->GetOffsetX() || itemIcon->GetOffsetY() != itemIcon->GetOffsetY())
								itemIcon->SetOffsetXY(itemBack->GetOffsetX(), itemBack->GetOffsetY());
						}
					}
					else if (item->GetItemType() == CHARGE_ITEM_TYPE_PARTNER)
					{
						MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(item->GetItemId());
						if (mobTemplate)
						{
							allDisable = false;

							Color32 textColor = GetRareColor(mobTemplate->m_basicinfo->rare);
							
							text = _T("");
							if (item->GetItemName().Equals(text.c_str()))
								text = String(mobTemplate->m_basicinfo->mid_name);
							else
								text = item->GetItemName();

							if (mobTemplate->m_basicinfo->TimeLimit > 0)
								text.Format(_T("%s(%d)"), text.c_str(), mobTemplate->m_basicinfo->TimeLimit);

							itemName->SetText(text.c_str());
							itemName->SetColor(textColor);
							itemName->Redraw();
							itemName->SetEnable(true);

							modelName = String(mobTemplate->m_basicinfo->mob_faceS);
							
							itemBack->SetEnable(true);
							if (itemIcon->GetOffsetX() == itemBack->GetOffsetX() || itemIcon->GetOffsetY() == itemIcon->GetOffsetY())
								itemIcon->SetOffsetXY(itemBack->GetOffsetX() + 2, itemBack->GetOffsetY() + 2);

						}
					}
					else if (item->GetItemType() == CHARGE_ITEM_TYPE_PET)
					{
						const PetData* pPetData = gGlobal.m_resource->GetPetData(item->GetItemId());
						if (pPetData)
						{
							allDisable = false;

							UInt32 textColor = GetRareColor(pPetData->pet_rare);
							
							text = _T("");
							if (item->GetItemName().Equals(text.c_str()))
								text = String(pPetData->pet_name);
							else
								text = item->GetItemName();

							if (pPetData->pet_TimeLimit > 0)
								text.Format(_T("%s(%d)"), text.c_str(), pPetData->pet_TimeLimit);

							itemName->SetText(text.c_str());
							itemName->SetColor(textColor);
							itemName->Redraw();
							itemName->SetEnable(true);

							modelName = String(pPetData->pet_faceS);

							itemBack->SetEnable(true);

							if (itemIcon->GetOffsetX() == itemBack->GetOffsetX() || itemIcon->GetOffsetY() == itemIcon->GetOffsetY())
								itemIcon->SetOffsetXY(itemBack->GetOffsetX() + 2, itemBack->GetOffsetY() + 2);
						}
					}

					text.Format(_T("%d"), item->GetYuanBao());
					itemOrgPrice->SetText(text.c_str());
					itemOrgPrice->Redraw();
					itemOrgPrice->SetEnable(true);

					UInt32 discount = item->GetYuanBao() * item->GetDiscount() / 100 ; //item->GetDiscount() / 100 = x%

					text.Format(_T("%d"), discount);
					itemDiscountPrice->SetText(text.c_str());
					itemDiscountPrice->Redraw();
					itemDiscountPrice->SetEnable(true);

					itemIcon->ClearAniData();
					itemIcon->SetAnimationData(modelName.c_str());
					itemIcon->SetEnable(true);

					itemButton->SetEnable(true);
					itemOrgPriceTitle->SetEnable(true);
					itemDiscountPriceTitle->SetEnable(true);
				}
			}
			if (allDisable)
			{
				itemIcon->SetEnable(false);
				itemName->SetEnable(false);
				itemOrgPrice->SetEnable(false);
				itemDiscountPrice->SetEnable(false);
				itemOrgPriceTitle->SetEnable(false);
				itemDiscountPriceTitle->SetEnable(false);
				itemButton->SetEnable(false);
			}

		}
	}
}

UInt8 CMenuOnlineShopProcess::GetShowIndex()
{ 
	return m_iShowIndex;
}

UInt8 CMenuOnlineShopProcess::GetPageNo()
{ 
	return m_iPage;
}

void CMenuOnlineShopProcess::UpdateYuanBao(bool fromServer)
{
	if (fromServer)
	{
		UInt32 diffTime = GetTickCount() - m_iUpdateYuanTimer;
		if (diffTime > 60000) // 5min
		{
			if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
				gGlobal.g_pCharacter->GetCharNetObject()->UpdateYuanBao(gGlobal.pPeerProxy, gGlobal.g_context);
			m_iUpdateYuanTimer = GetTickCount();
		}
		else
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_UPDATE_YUANBAO_TOO_FREQ")), (300 - diffTime / 1000));
			gGlobal.PrintPlayerMessage(text.c_str());
		}
	}
	else
	{
		UIStaticText* text = gGlobal.pUIManager->FindUIStaticText("OnlineShopContainMoney");
		String money;
		if (text && gGlobal.g_pCharacter && 
			gGlobal.g_pCharacter->GetCharNetObject())
		{
			
			money.Format(_T("%d"), gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBao());
			text->SetText(money.c_str());
			text->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(text);
			
		}
	}
}

void CMenuOnlineShopProcess::OpenOnlineShop()
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenOnlineShop", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OnlineShopShowHotSale", String());
	UpdateYuanBao();
	ShowDiscountItem();
	ShowLimitedItem();
}

int CMenuOnlineShopProcess::GetSelectedIndex()
{
	return m_iSelItemIndex;
}

int CMenuOnlineShopProcess::GetSelectedDiscountIndex()
{
	return m_iSelDiscountItemIndex;
}
	
int CMenuOnlineShopProcess::GetSelectedLimitedIndex()
{
	return m_iSelLimitedItemIndex;
}

void CMenuOnlineShopProcess::ResetSelectedIndex()
{
	if (GetSelectedIndex() >= 0)
		m_pSelectedItem = NULL;
	m_iSelItemIndex = -1;
}
	
void CMenuOnlineShopProcess::ResetSelectedDiscountIndex()
{
	if (GetSelectedDiscountIndex() >= 0)
		m_pSelectedItem = NULL;
	m_iSelDiscountItemIndex = -1;
}
	
void CMenuOnlineShopProcess::ResetSelectedLimitedIndex()
{
	if (GetSelectedLimitedIndex() > 0)
		m_pSelectedItem = NULL;
	m_iSelLimitedItemIndex = -1;
}

void CMenuOnlineShopProcess::SetSelectedIndex(int sel)
{
	m_iSelItemIndex = sel;
	CChargeShopItem* temp = GetSelectedItem();
	
	if (temp)
	{
		if (temp->GetPreviewType() != PREVIEW_NONE )
			Preview();
		else
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseOnlineShopPreview", String());
		
		//gGlobal.pUIManager->AddDirtyUIItem(button);
	}
	
}
	
void CMenuOnlineShopProcess::SetSelectedDiscountIndex(int sel)
{
	m_iSelDiscountItemIndex = sel;
	CChargeShopItem* temp = GetSelectedItem();
	if (temp)
	{
		if (temp->GetPreviewType() != PREVIEW_NONE )
			Preview();
		else
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseOnlineShopPreview", String());
	}
}

void CMenuOnlineShopProcess::SetSelectedLimitedIndex(int sel)
{
	m_iSelLimitedItemIndex = sel;
	CChargeShopItem* temp = GetSelectedItem();
	if (temp)
	{
		if (temp->GetPreviewType() != PREVIEW_NONE )
			Preview();
		else
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseOnlineShopPreview", String());
	}
}

void CMenuOnlineShopProcess::SetChargeShop(CChargeShop* shop)
{
	if (m_pChargeShop)
	{
		if (m_pChargeShop != shop)
		{
			DeleteNetGroup(m_pChargeShop, BOOLEAN_TRUE); 
			m_hotSaleItem.clear();
			m_mixItem.clear();
			m_asscItem.clear();
			m_petItem.clear();
			m_partnerItem.clear();
			m_clothItem.clear();
			m_otherItem.clear();
			m_limitedItem.clear();
			m_discountItem.clear();
			m_DiamondItem.clear();
			m_BookItem.clear();
			m_FireWorkItem.clear();
			m_CapItem.clear();
			m_MouldItem.clear();
			m_pSelectedItem = NULL;
		}
		else
			return;
	}

	m_pChargeShop = shop;

	typedef StlMultiMap<UInt32, CChargeShopItem *, std::greater_equal<UInt32>, ManagedAllocator<std::pair<UInt32, CChargeShopItem *> > > SellSorting;
	SellSorting tempSellSortList;
	for (Index i = C_INDEX(0); i < m_pChargeShop->GetCount(); ++i)
	{
		CChargeShopItem* item = m_pChargeShop->GetChild<CChargeShopItem>(i);
		if (item)
		{
			switch(item->GetShopType())
			{
			case MIX:		m_mixItem.push_back(item);		break;
			case ASSC:		m_asscItem.push_back(item);		break;
			case PET:		m_petItem.push_back(item);		break;
			case PARTNER:	m_partnerItem.push_back(item);	break;
			case CLOTH:		m_clothItem.push_back(item);	break;
			case OTHER:		m_otherItem.push_back(item);	break;
			case HOTSALE:	/*m_hotSaleItem.push_back(item);*/ break;
			case LIMITED:	m_limitedItem.push_back(item);	break;
			case DISCOUNT:	m_discountItem.push_back(item);	break;
			case DIAMOND:	m_DiamondItem.push_back(item); break;
			case BOOK:		m_BookItem.push_back(item);	   break;
			case CAP:       m_CapItem.push_back(item);     break;
			case FIREWORKS: m_FireWorkItem.push_back(item);break;
			case MOULD:		m_MouldItem.push_back(item);   break;
			}
		
			//for hotSellItem
			if (item->GetTotalSell() > 0 && item->GetShopType() != LIMITED && item->GetShopType() != DISCOUNT)
				tempSellSortList.insert(std::make_pair(item->GetTotalSell(), item));
		}
	}

	for (SellSorting::iterator itr = tempSellSortList.begin(); itr != tempSellSortList.end(); ++itr)
	{
		m_hotSaleItem.push_back(itr->second);
		if (m_hotSaleItem.size() == 10)
			break;
	}
	OpenOnlineShop();
}

void CMenuOnlineShopProcess::ShowItem(ChargeShopCategory itemList, UInt8 pageNo)
{
	UIAniIcon* itemIcon = NULL;
	UIStaticText* itemName = NULL;
	UIStaticText* itemPrice = NULL;
	UIStaticText* itemPriceTitle = NULL;
	UIStaticText* itemDay = NULL;
	UIButton* itemButton = NULL;
	UIImage* itemBack = NULL;
	
	UIStaticText* pageNoText = gGlobal.pUIManager->FindUIStaticText("OnlineShopPageNo");
	if (pageNoText)
	{		
		UInt8 totalPage = (UInt8)itemList.size() / MAX_ITEM_PER_PAGE;
		if ( (itemList.size() % MAX_ITEM_PER_PAGE) > 0)
			++totalPage;
		if (totalPage <= pageNo)
			m_iPage = totalPage - 1;
		else
			m_iPage = pageNo;

		UInt8 nowPage = m_iPage + 1;
		String text;
		text.Format(_T("%d / %d"), nowPage, totalPage);
		pageNoText->SetText(text.c_str() );
		pageNoText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pageNoText);
	}
	
	for (Index i = C_INDEX(0); i < MAX_ITEM_PER_PAGE; ++i )
	{
		String text, modelName;
		StringA textA;
		UInt16 timeLimit = 0;
		textA.Format("OnlineShopItem0%dIcon", i);
		itemIcon = gGlobal.pUIManager->FindUIAniIcon(textA.c_str());

		textA.Format("OnlineShopItem0%dName", i);
		itemName = gGlobal.pUIManager->FindUIStaticText(textA.c_str());

		textA.Format("OnlineShopItem0%dCost", i);
		itemPrice = gGlobal.pUIManager->FindUIStaticText(textA.c_str());

		textA.Format("OnlineShopItem0%dCostTitle", i);
		itemPriceTitle = gGlobal.pUIManager->FindUIStaticText(textA.c_str());

		textA.Format("OnlineShopItem0%dDay", i);
		itemDay = gGlobal.pUIManager->FindUIStaticText(textA.c_str());

		textA.Format("OnlineShopItem0%d", i);
		itemButton = gGlobal.pUIManager->FindUIButton(textA.c_str());

		textA.Format("OnlineShopBack0%dIcon", i);
		itemBack = gGlobal.pUIManager->FindUIImage(textA.c_str());

		bool allDisable = true;
		if (itemIcon && itemName && itemPrice && itemDay && itemButton && itemBack)
		{
			UInt index = i + m_iPage * MAX_ITEM_PER_PAGE;
			if (index < itemList.size() )
			{
				CChargeShopItem* item = itemList[index];
				if (item)
				{
					if (item->GetItemType() == CHARGE_ITEM_TYPE_ITEM)
					{
						const ItemData* pItemData = gGlobal.m_resource->GetItemData(item->GetItemId());
						if (pItemData)
						{
							allDisable = false;
							text = _T("");
							if (item->GetItemName().Equals(text.c_str()))
								itemName->SetText(String(pItemData->name).c_str());
							else
								itemName->SetText(item->GetItemName().c_str());

							Color32 textColor = TextColor[ColorIndex_White];
							switch (pItemData->color)
							{
							case ItemColorIndex_Grey:	textColor = ItemTextColor[ItemColorIndex_Grey];			break;
							case ItemColorIndex_LightGreen:	textColor = ItemTextColor[ItemColorIndex_LightGreen];	break;
							case ItemColorIndex_LightBlue:	textColor = ItemTextColor[ItemColorIndex_LightBlue];	break;
							case ItemColorIndex_LightPurple:	textColor = ItemTextColor[ItemColorIndex_LightPurple];	break;
							case ItemColorIndex_LightGolden:	textColor = ItemTextColor[ItemColorIndex_LightGolden];	break;
							case ItemColorIndex_GodGolden:	textColor = ItemTextColor[ItemColorIndex_GodGolden];	break;
							}
							itemName->SetColor(textColor);
							itemName->Redraw();
							itemName->SetEnable(true);

							timeLimit = pItemData->item_TimeLimit;

							modelName = String(pItemData->icon2);
							
							itemBack->SetEnable(false);

							if (itemIcon->GetOffsetX() != itemBack->GetOffsetX() || itemIcon->GetOffsetY() != itemIcon->GetOffsetY())
								itemIcon->SetOffsetXY(itemBack->GetOffsetX(), itemBack->GetOffsetY());
							
						}
					}
					else if (item->GetItemType() == CHARGE_ITEM_TYPE_PARTNER)
					{
						MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(item->GetItemId());
						if (mobTemplate)
						{
							allDisable = false;

							Color32 textColor = this->GetRareColor(mobTemplate->m_basicinfo->rare);
							
							text = _T("");
							if (item->GetItemName().Equals(text.c_str()))
								itemName->SetText(String(mobTemplate->m_basicinfo->mid_name).c_str());
							else
								itemName->SetText(item->GetItemName().c_str());
							itemName->SetColor(textColor);
							itemName->Redraw();
							itemName->SetEnable(true);

							timeLimit = mobTemplate->m_basicinfo->TimeLimit;

							modelName = String(mobTemplate->m_basicinfo->mob_faceS);
							
							itemBack->SetEnable(true);
							if (itemIcon->GetOffsetX() == itemBack->GetOffsetX() || itemIcon->GetOffsetY() == itemIcon->GetOffsetY())
								itemIcon->SetOffsetXY(itemBack->GetOffsetX() + 2, itemBack->GetOffsetY() + 2);

						}
					}
					else if (item->GetItemType() == CHARGE_ITEM_TYPE_PET)
					{
						const PetData* pPetData = gGlobal.m_resource->GetPetData(item->GetItemId());
						if (pPetData)
						{
							allDisable = false;

							Color32 textColor = GetRareColor(pPetData->pet_rare);
							
							text = _T("");
							if (item->GetItemName().Equals(text.c_str()))
								itemName->SetText(String(pPetData->pet_name).c_str());
							else
								itemName->SetText(item->GetItemName().c_str());
							itemName->SetColor(textColor);
							itemName->Redraw();
							itemName->SetEnable(true);

							timeLimit = pPetData->pet_TimeLimit;
							
							modelName = String(pPetData->pet_faceS);

							itemBack->SetEnable(true);

							if (itemIcon->GetOffsetX() == itemBack->GetOffsetX() || itemIcon->GetOffsetY() == itemIcon->GetOffsetY())
								itemIcon->SetOffsetXY(itemBack->GetOffsetX() + 2, itemBack->GetOffsetY() + 2);
						}
					}

					if (timeLimit > 0)
					{
						text.Format(_T("%d%s"), timeLimit,gGlobal.GetStringTable()->Get(_T("MSG_DAY")));
						itemDay->SetText(text.c_str());
						itemDay->Redraw();
						itemDay->SetEnable(true);
					}
					else
						itemDay->SetEnable(false);

					if (!modelName.Equals(_T("")))
					{
						itemIcon->ClearAniData();
						itemIcon->SetAnimationData(modelName.c_str());
						itemIcon->SetEnable(true);
					}

					text.Format(_T("%d"), item->GetYuanBao());
					itemPrice->SetText(text.c_str());
					itemPrice->Redraw();
					itemPrice->SetEnable(true);

					itemPriceTitle->SetEnable(true);

					itemButton->SetEnable(true);
				}
			}
			if (allDisable)
			{
				itemIcon->SetEnable(false);
				itemName->SetEnable(false);
				itemPrice->SetEnable(false);
				itemDay->SetEnable(false);
				itemButton->SetEnable(false);
				itemPriceTitle->SetEnable(false);
				itemBack->SetEnable(false);
			}
		}
	}
}

void CMenuOnlineShopProcess::AskForOnlineShop()
{
	if (IsHawking())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_OPEN_CHARGESHOP")));
		return;
	}
	
	if (gGlobal.g_pCharacter->GetCharNetObject() && QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP)
	{
		MapService_Proxy::OpenChargeShop(gGlobal.pPeerProxy, gGlobal.g_context);
	}
}

void CMenuOnlineShopProcess::NextPage()
{
	++m_iPage;
	switch(m_iShowIndex)
	{
	case MIX:		ShowMix(m_iPage);	break;
	case ASSC:		ShowAssc(m_iPage);	break;
	case PET:		ShowPet(m_iPage);	break;
	case PARTNER:	ShowPartner(m_iPage);	break;
	case CLOTH:		ShowCloth(m_iPage);	break;
	case OTHER:		ShowOther(m_iPage);	break;
	case HOTSALE:	ShowHotSale(m_iPage);	break;
	case DIAMOND:	ShowDiamond(m_iPage);	break;
	case BOOK:		ShowBook(m_iPage);	break;
	case CAP:		ShowCap(m_iPage);	break;
	case FIREWORKS:	ShowFireworks(m_iPage);	break;
	case MOULD:		ShowMould(m_iPage);	break;

	}
}

void CMenuOnlineShopProcess::PrevPage()
{
	if (m_iPage > 0)
	{
		--m_iPage;
		switch(m_iShowIndex)
		{
		case MIX:	ShowMix(m_iPage);	break;
		case ASSC:	ShowAssc(m_iPage);	break;
		case PET:	ShowPet(m_iPage);	break;
		case PARTNER:	ShowPartner(m_iPage);	break;
		case CLOTH:	ShowCloth(m_iPage);	break;
		case OTHER:	ShowOther(m_iPage);	break;
		case HOTSALE:	ShowHotSale(m_iPage);	break;
		case DIAMOND:	ShowDiamond(m_iPage);	break;
		case BOOK:		ShowBook(m_iPage);	break;
		case CAP:		ShowCap(m_iPage);	break;
		case FIREWORKS:	ShowFireworks(m_iPage);	break;
		case MOULD:		ShowMould(m_iPage);	break;
		}
	}
}


void CMenuOnlineShopProcess::ShowItemDetail()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("OnlineShopPanel");
	if (pPanel && m_pChargeShop)
	{
		Int posX = gGlobal.pUIManager->GetCursorX() - pPanel->GetX();  
		Int posY = gGlobal.pUIManager->GetCursorY() - pPanel->GetY();
		if (posX > 0 && posY > 0 && posX < (Int)pPanel->GetWidth() && posY < (Int)pPanel->GetHeight())
		{
			UInt8 index = 0;
			UInt8 row1 = 0;
			if (posX > 161 && posX < 201)
				row1 = 0;
			else if (posX > 363 && posX < 403)
				row1 = 1;
			else
				return;
		
			if (posY > 73 + 42 && posY < 113 + 42)
				index = row1 + 2 * 0;
			else if (posY > 138 + 36 && posY < 178 + 36)
				index = row1 + 2 * 1;
			else if (posY > 203 + 30 && posY < 243 + 30)
				index = row1 + 2 * 2;
			else if (posY > 267 + 24 && posY < 307 + 24)
				index = row1 + 2 * 3;
			else if (posY > 332 + 18 && posY < 372 + 18)
				index = row1 + 2 * 4;
			else 
				return;

			index = index + m_iPage * 10;
			CChargeShopItem* item = NULL;
			switch(m_iShowIndex)
			{
			case MIX:
				{
					if (index < m_mixItem.size())
						item = m_mixItem[index];
				}
				break;
			case ASSC:
				{
					if (index < m_asscItem.size())
						item = m_asscItem[index];
				}
				break;
			case PET:
				{
					if (index < m_petItem.size())
					item = m_petItem[index];
				}
				break;
			case PARTNER:
				{
					if (index < m_partnerItem.size())
						item = m_partnerItem[index];
				}
				break;
			case CLOTH:
				{
					if (index < m_clothItem.size())
						item = m_clothItem[index];
				}
				break;
			case OTHER:
				{
					if (index < m_otherItem.size())
						item = m_otherItem[index];
				}
				break;
			case HOTSALE:
				{
					if (index < m_hotSaleItem.size())
						item = m_hotSaleItem[index];
				}
				break;
			case DIAMOND:
				{
					if (index < m_DiamondItem.size())
						item = m_DiamondItem[index];
				}
				break;

			case BOOK:
				{
					if (index < m_BookItem.size())
						item = m_BookItem[index];
				}
				break;
			case CAP:
				{
					if (index < m_CapItem.size())
						item = m_CapItem[index];
				}
				break;
			case FIREWORKS:
				{
					if (index < m_FireWorkItem.size())
						item = m_FireWorkItem[index];
				}
				break;
			case MOULD:
				{
					if (index < m_MouldItem.size())
						item = m_MouldItem[index];
				}
				break;
			}

			if (item)
				ShowOnlineShopItemDetail(item);
		}
	}
}
	
void CMenuOnlineShopProcess::ShowDiscountItemDetail()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("OnlineShopPanel");
	if (pPanel && m_pChargeShop && m_discountItem.size() > 0)
	{
		Int posX = gGlobal.pUIManager->GetCursorX() - pPanel->GetX();  
		Int posY = gGlobal.pUIManager->GetCursorY() - pPanel->GetY();
		if (posX > 0 && posY > 0 && posX < (Int)pPanel->GetWidth() && posY < (Int)pPanel->GetHeight())
		{
			if (posX > 19 && posX < 59)
			{
				CChargeShopItem* item = NULL;
				if (posY > 191 && posY < 231 && m_discountItem.size() > 0)
					item = m_discountItem[0];
				else if (posY > 273 && posY < 313 && m_discountItem.size() > 1)
					item = m_discountItem[1];
				else if (posY > 356 && posY < 396 && m_discountItem.size() > 2)
					item = m_discountItem[2];
				else
					return;

				if (item)
					ShowOnlineShopItemDetail(item);
			}
		}
	}
}

void CMenuOnlineShopProcess::ShowOnlineShopItemDetail(CChargeShopItem* item)
{
	if (item)
	{
		Int32 XOffset = 20;
		Int32 YOffset = 20;
		if (item->GetItemType() == CHARGE_ITEM_TYPE_ITEM)
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
			gGlobal.g_pItemProcess->ItemDetailPopUp(item->GetItemId(), true, NULL, false);

			Int32 posX = 0, posY = 0;

			if ( ( gGlobal.pUIManager->GetCursorX() + pPanelEx->GetWidth() ) > 800 )
				posX = 800 - pPanelEx->GetWidth() - XOffset;
			else
				posX = gGlobal.pUIManager->GetCursorX() + XOffset;

			if ( ( gGlobal.pUIManager->GetCursorY() + pPanelEx->GetHeight() ) > 600 )
				posY = 600 - pPanelEx->GetHeight() - YOffset;
			else
				posY = gGlobal.pUIManager->GetCursorY() + YOffset;
			
			pPanelEx->SetPos(posX, posY);
			pPanelEx->SetEnable(true);
		}
		else if (item->GetItemType() == CHARGE_ITEM_TYPE_PARTNER)
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
			gGlobal.g_pPartnerProcess->ShowMobDetail(item->GetItemId(), item->GetLV());
			gGlobal.pUIManager->SetPanel2Top(pPanel);
			Int32 posX = 0, posY = 0;

			if ( ( gGlobal.pUIManager->GetCursorX() + pPanel->GetWidth() ) > 800 )
				posX = 800 - pPanel->GetWidth() - XOffset;
			else
				posX = gGlobal.pUIManager->GetCursorX() + XOffset;

			if ( ( gGlobal.pUIManager->GetCursorY() + pPanel->GetHeight() ) > 600 )
				posY = 600 - pPanel->GetHeight() - YOffset;
			else
				posY = gGlobal.pUIManager->GetCursorY() + YOffset;

			pPanel->SetPos(posX, posY);
			pPanel->SetEnable(true);
		}
		else if (item->GetItemType() == CHARGE_ITEM_TYPE_PET)
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
			gGlobal.g_pMenuPetProcess->ShowPetDetail(item->GetItemId(), item->GetLV());

			gGlobal.pUIManager->SetPanel2Top(pPanel);
			Int32 posX = 0, posY = 0;

			if ( ( gGlobal.pUIManager->GetCursorX() + pPanel->GetWidth() ) > 800 )
				posX = 800 - pPanel->GetWidth() - XOffset;
			else
				posX = gGlobal.pUIManager->GetCursorX() + XOffset;

			if ( ( gGlobal.pUIManager->GetCursorY() + pPanel->GetHeight() ) > 600 )
				posY = 600 - pPanel->GetHeight() - YOffset;
			else
				posY = gGlobal.pUIManager->GetCursorY() + YOffset;

			pPanel->SetPos(posX, posY);
			pPanel->SetEnable(true);
		}
	}
}
	
void CMenuOnlineShopProcess::ShowLimitedItemDetail()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("OnlineShopPanel");
	if (pPanel && m_pChargeShop && m_limitedItem.size() > 0)
	{
		Int posX = gGlobal.pUIManager->GetCursorX() - pPanel->GetX();  
		Int posY = gGlobal.pUIManager->GetCursorY() - pPanel->GetY();
		if (posX > 0 && posY > 0 && posX < (Int)pPanel->GetWidth() && posY < (Int)pPanel->GetHeight())
		{
			if (posX > 56 && posX < 96 && posY > 67 && posY < 107)
			{
				CChargeShopItem* item = m_limitedItem[0];
				if (item)
					ShowOnlineShopItemDetail(item);
			}
		}
	}
}

CChargeShopItem* CMenuOnlineShopProcess::GetSelectedItem()
{
	if (GetSelectedDiscountIndex() >= 0)
	{
		if (GetSelectedDiscountIndex() < (int)m_discountItem.size() )
			m_pSelectedItem = m_discountItem[GetSelectedDiscountIndex()] ;
	}
	else if (GetSelectedLimitedIndex() > 0)
	{
		m_pSelectedItem = m_limitedItem[0];
	}
	else if (GetSelectedIndex() >= 0)
	{
		UInt pos = GetPageNo() * MAX_ITEM_PER_PAGE + GetSelectedIndex();
		switch(m_iShowIndex)
		{
		case MIX:
			{				
				if (pos < m_mixItem.size())
					m_pSelectedItem = m_mixItem[pos];
			}
			break;
		case ASSC:
			{
				if (pos < m_asscItem.size())
					m_pSelectedItem = m_asscItem[pos];
			}
			break;
		case PET:
			{
				if (pos < m_petItem.size())
					m_pSelectedItem = m_petItem[pos];
			}
			break;
		case PARTNER:
			{
				if (pos < m_partnerItem.size())
					m_pSelectedItem = m_partnerItem[pos];
			}
			break;
		case CLOTH:
			{
				if (pos < m_clothItem.size())
					m_pSelectedItem = m_clothItem[pos];
			}
			break;
		case OTHER:
			{
				if (pos < m_otherItem.size())
					m_pSelectedItem = m_otherItem[pos];
			}
			break;
		case HOTSALE:
			{
				if (pos < m_hotSaleItem.size())
					m_pSelectedItem = m_hotSaleItem[pos];
			}	
			break;

		case DIAMOND:
			{
				if (pos < m_DiamondItem.size())
					m_pSelectedItem = m_DiamondItem[pos];
			}	
			break;

		case BOOK:
			{
				if (pos < m_BookItem.size())
					m_pSelectedItem = m_BookItem[pos];
			}	
			break;

		case CAP:
			{
				if (pos < m_CapItem.size())
					m_pSelectedItem = m_CapItem[pos];
			}	
			break;
		case FIREWORKS:
			{
				if (pos < m_FireWorkItem.size())
					m_pSelectedItem = m_FireWorkItem[pos];
			}	
			break;
		case MOULD:
			{
				if (pos < m_MouldItem.size())
					m_pSelectedItem = m_MouldItem[pos];
			}	
			break;
		}
	}

	return m_pSelectedItem;
}

int CMenuOnlineShopProcess::GetSelectedItemType()
{
	if (m_pSelectedItem)
		return m_pSelectedItem->GetItemType();
	else
	{
		GetSelectedItem();

		if (m_pSelectedItem)
			return m_pSelectedItem->GetItemType();
		else
			return -1;
	}
}

UInt32 CMenuOnlineShopProcess::GetSelectedItemPrice(UInt16 stack)
{
	UInt32 money = 0;
	if (m_pSelectedItem)
	{
		money = m_pSelectedItem->GetYuanBao() * stack;
		if (m_pSelectedItem->GetShopType() == DISCOUNT && m_pSelectedItem->GetDiscount() > 0)
			money = stack * (m_pSelectedItem->GetYuanBao()  * m_pSelectedItem->GetDiscount() / 100);
	}
	return money;
}

UInt16 CMenuOnlineShopProcess::GetMaxSelectedItemStack()
{
	UInt16 stack = 0;

	if (m_pSelectedItem == NULL)
		GetSelectedItem();

	if (m_pSelectedItem)
	{
		UInt yuanbao = m_pSelectedItem->GetYuanBao();
		if (m_pSelectedItem->GetShopType() == DISCOUNT)
			yuanbao = m_pSelectedItem->GetYuanBao() * m_pSelectedItem->GetDiscount() / 100;

		stack = (UInt16)(MIN(gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBao() / yuanbao, ONLINESHOP_SUPPORT_STACK_LIMIT));

	}
	return stack;
}

void CMenuOnlineShopProcess::SetBuyItemStack(UInt16 stack)
{
	m_iStack = stack;
}

void CMenuOnlineShopProcess::ConfirmBuyItem()
{

	if (m_pSelectedItem && m_iStack > 0 && gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject() && m_pChargeShop)
	{
		if (!gGlobal.g_pCharacter->GetCharNetObject()->GetYuanbaoLock())
		{
			m_pChargeShop->BuyItemInChargeShop(gGlobal.pPeerProxy, gGlobal.g_context, m_pSelectedItem->GetChargeShopID(), m_iStack);
			gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_BUYING")), false, 0, 2000);
		}
		else
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_YUANBAO_LOCK_CANT_USE")));	
		}
	}
}

UInt8 CMenuOnlineShopProcess::GetSelectedItemOverLap()
{
	if (m_pSelectedItem == NULL)
		GetSelectedItem();

	if (m_pSelectedItem == NULL)
		return 0;

	if (m_pSelectedItem->GetItemType() == CHARGE_ITEM_TYPE_ITEM)
	{	
		const ItemData* pItemData = gGlobal.m_resource->GetItemData(m_pSelectedItem->GetItemId());
		if (pItemData)
			return pItemData->overlap;
		else
			return 0;
	}
	else
		return 1;
}

void CMenuOnlineShopProcess::Preview()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("OnlineShopPreviewPanel");
	if (pPanel == NULL)
	{
		XML_UI::GetInstance().Init();
		XML_UI::GetInstance().ReadFormFile("Data\\OnlineShopPreview.xml", gGlobal.pUIManager);

		pPanel = gGlobal.pUIManager->FindUIPanel("OnlineShopPreviewPanel");
		if (pPanel == NULL)
			return;		

		pPanel->SetPos(596, 73);
	}

	if ( m_pSelectedItem == NULL )
		GetSelectedItem();

	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon("OnlineShopPreviewModel");
	
	if (m_pSelectedItem && pAniIcon)
	{
		CAnimatedEntity temp;
		temp.InitEntPicInfo();

		if (m_pSelectedItem->GetPreviewType() == PREVIEW_CLOTH)
		{
			UInt32 eqCloth = gGlobal.g_pCharacter->GetCharNetObject()->GetOrg_eq_cloth();
			UInt16 eqClothColor = gGlobal.g_pCharacter->GetCharNetObject()->GetOrg_eq_clothColor();
			UInt16 hairStyle = gGlobal.g_pCharacter->GetCharNetObject()->GetOrg_hairStyle1();
			UInt16 hairColor = gGlobal.g_pCharacter->GetCharNetObject()->GetOrg_hairColor();
			UInt32 headBand = gGlobal.g_pCharacter->GetCharNetObject()->GetEq_headBand();
			UInt32 weapon = gGlobal.g_pCharacter->GetCharNetObject()->GetEq_weapon();
			UInt8 sexClass = gGlobal.g_pCharacter->GetCharNetObject()->GetSexClass();

			if (gGlobal.g_pCharacter->GetCharNetObject()->GetHairStyle1() > 0)
				hairStyle = gGlobal.g_pCharacter->GetCharNetObject()->GetHairStyle1();
			if (gGlobal.g_pCharacter->GetCharNetObject()->GetHairColor() > 0)
				hairColor = gGlobal.g_pCharacter->GetCharNetObject()->GetHairColor();
			if (gGlobal.g_pCharacter->GetCharNetObject()->GetEq_cloth() > 0)
			{
				eqCloth = gGlobal.g_pCharacter->GetCharNetObject()->GetEq_cloth();
				eqClothColor = gGlobal.g_pCharacter->GetCharNetObject()->GetEq_clothColor();
			}

			const ItemData* pItemData = gGlobal.m_resource->GetItemData(m_pSelectedItem->GetItemId());
			if (pItemData && (pItemData->reqSexClass1 == sexClass || pItemData->reqSexClass2 == sexClass))
			{
				if (pItemData->eqType == EQTYPE_CLOTH)
				{
					eqCloth = pItemData->item_id;
					eqClothColor = pItemData->eq_Color;
				}
				else if (pItemData->eqType == EQTYPE_HAT_WITH_HAIR ||pItemData->eqType == EQTYPE_HAT_WITHOUT_HAIR)
					headBand = pItemData->item_id;
				
			}
			else
			{
				//gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSE_SYS_EQUIP_CANT_USE"));
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseOnlineShopPreview", String());
				return;
			}

			temp = gGlobal.GetCharModel(sexClass, hairStyle, hairColor, 
					eqCloth, eqClothColor, headBand, weapon);	
		}
		else if (m_pSelectedItem->GetPreviewType() == PREVIEW_PARTNER)
		{
			MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(m_pSelectedItem->GetItemId());

			temp.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
			String mobName = String(mobTemplate->m_basicinfo->model);
			mobName = mobName.substr(0, mobName.length() - 4);
			TCHAR tempC = mobName.GetAt((int)mobName.length() - 1);

			if (tempC == 'b' || tempC == 'n')
				mobName = mobName.substr(0, mobName.length() - 1);
			mobName.append(_T("n.ent"));
			temp.LoadEntityFile(mobName.c_str(), gGlobal.pD3DDevice);

		}
		else if (m_pSelectedItem->GetPreviewType() == PREVIEW_PET_FOLLOW)
		{
			const PetData * pPetData = gGlobal.m_resource->GetPetData(m_pSelectedItem->GetItemId());

			if (pPetData)
			{
				temp.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
				String modelName = String(pPetData->pet_model);
				temp.LoadEntityFile(modelName.c_str(), gGlobal.pD3DDevice);
			}
		}
		else if (m_pSelectedItem->GetPreviewType() == PREVIEW_PET_MOUNT)
		{
			const PetData * pPetData = gGlobal.m_resource->GetPetData(m_pSelectedItem->GetItemId());

			if (pPetData)
			{
				UInt32 eqCloth = gGlobal.g_pCharacter->GetCharNetObject()->GetOrg_eq_cloth();
				UInt16 eqClothColor = gGlobal.g_pCharacter->GetCharNetObject()->GetOrg_eq_clothColor();
				UInt16 hairStyle = gGlobal.g_pCharacter->GetCharNetObject()->GetOrg_hairStyle1();
				UInt16 hairColor = gGlobal.g_pCharacter->GetCharNetObject()->GetOrg_hairColor();
				UInt32 headBand = gGlobal.g_pCharacter->GetCharNetObject()->GetEq_headBand();
				UInt32 weapon = gGlobal.g_pCharacter->GetCharNetObject()->GetEq_weapon();
				UInt8 sexClass = gGlobal.g_pCharacter->GetCharNetObject()->GetSexClass();

				if (gGlobal.g_pCharacter->GetCharNetObject()->GetHairStyle1() > 0)
					hairStyle = gGlobal.g_pCharacter->GetCharNetObject()->GetHairStyle1();
				if (gGlobal.g_pCharacter->GetCharNetObject()->GetHairColor() > 0)
					hairColor = gGlobal.g_pCharacter->GetCharNetObject()->GetHairColor();
				if (gGlobal.g_pCharacter->GetCharNetObject()->GetEq_cloth() > 0)
				{
					eqCloth = gGlobal.g_pCharacter->GetCharNetObject()->GetEq_cloth();
					eqClothColor = gGlobal.g_pCharacter->GetCharNetObject()->GetEq_clothColor();
				}

				temp = gGlobal.GetCharModel(sexClass, hairStyle, hairColor, 
						eqCloth, eqClothColor, headBand, weapon);

				temp.RemovePart(EQUIP_MOUNT);
				temp.SetMountOffset(-1 * pPetData->pet_Height);
				temp.LoadEntityFile(String(pPetData->pet_model).c_str(), gGlobal.pD3DDevice, EQUIP_MOUNT);
				
				//temp.ChangeColorP(String(pPetData->pet_model).c_str(), String(pPetData->pet_color).c_str());
				String mobName = pPetData->pet_model;
				String colorFileName = mobName.substr(0, mobName.length() - 4);
				char abc = pPetData->pet_color + 96;
				colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
				temp.ChangeColorP(mobName.c_str(), colorFileName.c_str());
				temp.SetAnimationIndexByIndex(ANIM_MOUNT_IDLE1);
			}
		}

		pAniIcon->ClearAniData();
		pAniIcon->SetAnimationData(&temp);
		//if (m_pSelectedItem->GetPreviewType() == PREVIEW_PARTNER)
		//	pAniIcon->SetAnimationIndex(temp.GetBattleAnimation());
		//else
			pAniIcon->SetAnimationIndex(temp.GetAnimationIndex());				
		pAniIcon->SetRotation(SOUTHEAST);
		gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
	}
}

void CMenuOnlineShopProcess::PreviewRotate(bool left)
{
	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon("OnlineShopPreviewModel");
	if (pAniIcon)
	{
		int rotation = pAniIcon->GetRotation();
		if (left)
		{
			++rotation;
			if ( rotation == MAXROTATION )
				rotation = SOUTH;
		}
		else
		{
			if (rotation == SOUTH)
				rotation = MAXROTATION ; 
			--rotation;
		}
		pAniIcon->SetRotation(rotation);
		gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
	}
}

void CMenuOnlineShopProcess::CloseOnlineShop(bool removeFromWorld)
{
	m_iPage = 0;
	m_iSelItemIndex = -1;
	m_iSelLimitedItemIndex = -1;
	m_iSelDiscountItemIndex = -1;
	m_iShowIndex = HOTSALE;
	m_iStack = 0;

	if (m_pChargeShop)
	{
		DeleteNetGroup(m_pChargeShop, BOOLEAN_TRUE);
		m_pChargeShop = NULL;
	}
	m_hotSaleItem.clear();
	m_mixItem.clear();
	m_asscItem.clear();
	m_petItem.clear();
	m_partnerItem.clear();
	m_clothItem.clear();
	m_otherItem.clear();
	m_limitedItem.clear();
	m_discountItem.clear();
	m_DiamondItem.clear();
	m_BookItem.clear();
	m_FireWorkItem.clear();
	m_CapItem.clear();
	m_MouldItem.clear();

	m_pSelectedItem = NULL;

	if (removeFromWorld && gGlobal.pPeerProxy && gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject()  )
	{
		MapService_Proxy::CloseChargeShop(gGlobal.pPeerProxy, gGlobal.g_context);
	}
}

CMenuOnlineShopProcess::~CMenuOnlineShopProcess()
{
	CloseOnlineShop(false);
}

void CMenuOnlineShopProcess::ShowLackMoneyMessage()
{
	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_CHARGE_SHOP_NO_YUANBAO")));
}

void CMenuOnlineShopProcess::OpenChargeShopWebSite(int index)
{
	switch(index)
	{
	case 0:
		{
			ShellExecute(NULL, _T("open"), gConfigure.strChargeLink.c_str(),
                NULL, NULL, SW_SHOWNORMAL);
		}
		break;
	case 1:
		{
			ShellExecute(NULL, _T("open"), gConfigure.strTransferLink.c_str(),
                NULL, NULL, SW_SHOWNORMAL);
		}
		break;
	}
}

Color32 CMenuOnlineShopProcess::GetRareColor(UInt rare)
{
	UInt textColor = 0;
	switch (rare)
	{
	case 0:	textColor = TextColor[ColorIndex_White];	break;
	case 1:	textColor = TextColor[ColorIndex_Green];	break;
	case 2:	textColor = TextColor[ColorIndex_Blue];		break;
	case 3:	textColor = TextColor[ColorIndex_Violet];	break;
	case 4:	textColor = TextColor[ColorIndex_Yellow];	break;
	}

	return textColor;
}