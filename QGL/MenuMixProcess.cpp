//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuMixProcess.h"

#include "MenuItemProcess.h"
#include "QGL/Global.h"
#include "Resource/ClientResManager.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "Resource/ResMixShop.h"
#include "QGLApplication.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/Item.h"
#include "CharacterControl.h"
#include "MenuSkillProcess.h"
#include "Proxy/Logic/Skill.h"
#include "Common/CallLuaFunc.h"
#include "Math.h"

#define MAX_MIX_MAKE_LINE_COUNT 10 //only display 10 line in display
#define MAX_CONSUME 4

CMenuMixProcess::CMenuMixProcess()
{
	m_typeItemList.clear();
	m_pMixShop = NULL;
	m_iStartProductIndex = 0;
	m_iSelectedIndex = -1;
	m_iShowLineCount = 0;
	m_pSelectedItemMix = NULL;
	m_sourceList.clear();
	m_iStartSourceIndex = 0;
	m_iStartTime = 0;
	m_pEqItem = NULL;
	m_iRemainItemCount = 0;
	m_bMakeAll = false;
	m_bHasLightStone = false;
	m_iStartMixShopX = 0;
	m_iStartMixShopY = 0;	
	m_isOpenMixShop = false;
	
	for (Index i = C_INDEX(0); i < 10; ++i)
	{
		ItemMixSource temp, temp1, temp2;
		if (i < 8)
			m_sourceList.push_back(temp);
		if (i < 6)
			m_mixShopItem.push_back(temp1);

		m_mixShopGem.push_back(temp2);
	}
	m_GemMixFirstItemId = 0;
	m_bNeedSecondCheck = false;
	m_iGemRemoveSelectIndex = -1;
	m_GemRemoveSelectGemId = 0;
}

CMenuMixProcess::~CMenuMixProcess()
{
	ResetMixShopMake();
	ResetMixShop();
	//ResetMixShopGem();
	m_sourceList.clear();
}

void CMenuMixProcess::OpenMixShop(UInt8 shopID)
{
	if (m_pMixShop == NULL)
	{
		m_pMixShop = gGlobal.m_resource->GetMixShopData(shopID);
		if (m_pMixShop)
		{
			m_isOpenMixShop = true;
			switch(m_pMixShop->type)
			{
			case MIX_EQUIPREPAIR:
			case MIX_STONEMIX:
			case MIX_UNSOULBOUND:
				{
					OpenMixOrRepair();
				}
				break;
			case MIX_IRONMAKE:
				{
					ItemMixDataList itemMixList;
					gGlobal.m_resource->GetSuitableItemList(m_pMixShop->mixItemTable, m_pMixShop->skilllvl_low, m_pMixShop->skilllvl_up, itemMixList);
					for (Index i = C_INDEX(0); i < itemMixList.size(); ++i)
					{
						ItemMixTypeList::iterator itr = m_typeItemList.find(itemMixList[i]->productType);
						if (itr != m_typeItemList.end())
						{		
							itr->second->dataList.push_back(itemMixList[i]);
						}
						else
						{
							ItemMixType* temp = SafeCreateObject(ItemMixType);
							temp->dataList.push_back(itemMixList[i]);
							m_typeItemList.insert(std::make_pair(itemMixList[i]->productType, temp));
						}
					}
					OpenIronMakeOrRepair();
				}
				break;
			case MIX_IRONREPAIR:
				{
					OpenIronMakeOrRepair();
				}			
				break;
			case MIX_EQUIPDRILL:			
			case MIX_GEMINLAY:
			case MIX_GEMREMOVE:
			case MIX_GEMMIX:
			case MIX_GEMMELTING:
			case MIX_GEMPOLISHING:
			case MIX_COLORSTONEMIX:
				{
					OpenGem();
				}
				break;
			case MIX_WEAPONMAKING:
			case MIX_CLOTHMAKING:
			case MIX_ORNAMENTMAKING:
				{
					ItemMixDataList itemMixList;
					gGlobal.m_resource->GetSuitableItemList(m_pMixShop->mixItemTable, m_pMixShop->skilllvl_low, m_pMixShop->skilllvl_up, itemMixList);
					for (Index i = C_INDEX(0); i < itemMixList.size(); ++i)
					{
						ItemMixTypeList::iterator itr = m_typeItemList.find(itemMixList[i]->productType);
						if (itr != m_typeItemList.end())
						{		
							itr->second->dataList.push_back(itemMixList[i]);
						}
						else
						{
							ItemMixType* temp = SafeCreateObject(ItemMixType);
							temp->dataList.push_back(itemMixList[i]);
							m_typeItemList.insert(std::make_pair(itemMixList[i]->productType, temp));
						}
					}
					OpenMixShopEqMaking();
				}
				break;
			case MIX_COOKING:
			case MIX_HERMETIC:
			case MIX_MAKEWINE:
				{
					ItemMixDataList itemMixList;
					gGlobal.m_resource->GetSuitableItemList(m_pMixShop->mixItemTable, m_pMixShop->skilllvl_low, m_pMixShop->skilllvl_up, itemMixList);
					for (Index i = C_INDEX(0); i < itemMixList.size(); ++i)
					{
						ItemMixTypeList::iterator itr = m_typeItemList.find(itemMixList[i]->productType);

						if (itr != m_typeItemList.end())
						{		
							itr->second->dataList.push_back(itemMixList[i]);
						}
						else
						{
							ItemMixType* temp = SafeCreateObject(ItemMixType);
							temp->dataList.push_back(itemMixList[i]);
							m_typeItemList.insert(std::make_pair(itemMixList[i]->productType, temp));
						}
					}
					OpenMixShopLiveSkill();
				}
				break;
			}
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenItemMenuForMixShop", String());
			if (gGlobal.g_pCharacter)
			{
                m_iStartMixShopX = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX;
				m_iStartMixShopY = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY;
			}
		}

	}
}

void CMenuMixProcess::OpenIronMakeOrRepair()
{
	if (m_pMixShop)
	{
		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("IronShopMakePanel");
		if (pPanel == NULL)
		{
			XML_UI::GetInstance().Init();
			XML_UI::GetInstance().ReadFormFile("Data\\IronShopMake.xml", gGlobal.pUIManager);

			pPanel = gGlobal.pUIManager->FindUIPanel("IronShopMakePanel");
			if (pPanel == NULL)
				return;		
		}

		pPanel->SetPos(20, 40);

		UIImage* img = NULL;

		if(m_pMixShop->type == MIX_IRONMAKE)
		{
			img = gGlobal.pUIManager->FindUIImage("IronShopMakeTitle");
			if (img)
				img->SetEnable(true);

			img = gGlobal.pUIManager->FindUIImage("IronShopMakeDesc");
			if (img)
				img->SetEnable(true);

			img = gGlobal.pUIManager->FindUIImage("IronShopMakeIron");
			if (img)
				img->SetEnable(true);

			img = gGlobal.pUIManager->FindUIImage("IronShopMakeScroll");
			if (img)
				img->SetEnable(true);
		}		
		else if (m_pMixShop->type == MIX_IRONREPAIR)
		{
			img = gGlobal.pUIManager->FindUIImage("IronShopRepairTitle");
			if (img)
				img->SetEnable(true);

			img = gGlobal.pUIManager->FindUIImage("IronShopRepairDesc");
			if (img)
				img->SetEnable(true);

			img = gGlobal.pUIManager->FindUIImage("IronShopRepairIron");
			if (img)
				img->SetEnable(true);

			img = gGlobal.pUIManager->FindUIImage("IronShopRepairScroll");
			if (img)
				img->SetEnable(true);
		}

		UIButton* buttonM = gGlobal.pUIManager->FindUIButton("IronShopButtonMake");
		UIButton* buttonR = gGlobal.pUIManager->FindUIButton("IronShopButtonRepair");
		if (buttonM && buttonR)
		{
			buttonM->SetDisableState();
			buttonR->SetDisableState();
			switch(m_pMixShop->type)
			{
			case MIX_IRONMAKE:
				buttonM->SetEnable(true);
				break;
			case MIX_IRONREPAIR:	
				buttonR->SetEnable(true);	
				break;
			}
		}
		UpdateIronMakeOrRepair();
		gGlobal.pUIManager->SetPanel2Top(pPanel);
		pPanel->SetEnable(true);
	}
}

void CMenuMixProcess::OpenMixOrRepair()
{
	if (m_pMixShop)
	{
		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("MixShopPanel");
		if (pPanel == NULL)
		{
			XML_UI::GetInstance().Init();
			XML_UI::GetInstance().ReadFormFile("Data\\MixShop.xml", gGlobal.pUIManager);

			pPanel = gGlobal.pUIManager->FindUIPanel("MixShopPanel");
			if (pPanel == NULL)
				return;		
		}

		pPanel->SetPos(20, 40);

		StringA uiItemName;
		UIImage* img = NULL;
		UIAniIcon* pAniIcon = NULL;

		uiItemName.Format("MixShopTitle%02d", m_pMixShop->type);
		img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
		if (img)
			img->SetEnable(true);
		
		UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonRepair");
		UIButton* button1 = gGlobal.pUIManager->FindUIButton("MixShopButtonRebuild");
		UIButton* button2 = gGlobal.pUIManager->FindUIButton("MixShopButtonStoneMix");
		UIButton* button3 = gGlobal.pUIManager->FindUIButton("MixShopButtonUnSoulBound");
		if (button && button1 && button2 && button3)
		{
			button->SetDisableState();
			button1->SetDisableState();
			button2->SetDisableState();
			button3->SetDisableState();
			switch(m_pMixShop->type)
			{
			case MIX_UNSOULBOUND:
				button3->SetEnable(true);
				break;
			case MIX_EQUIPREPAIR:	
				button->SetEnable(true);		
				break;
			case MIX_STONEMIX:		
				button2->SetEnable(true);		
				break;
			}
		}
		
		UpdateMixOrRepairItem();

		UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel("MixShopSuggestion");
		if (pChatPanel)
		{
			pChatPanel->ClearAllRow();
			pChatPanel->AddStringW(String(m_pMixShop->suggestion).c_str());
			pChatPanel->EndRow();
			pChatPanel->Redraw();
			pChatPanel->SetEnable(true);
		}

		ShowMixConsume(true);
		gGlobal.pUIManager->SetPanel2Top(pPanel);
		pPanel->SetEnable(true);
	}
}

void CMenuMixProcess::OpenGem()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CMenuMixProcess::OpenGem()."));

	if (m_pMixShop)
	{
		m_GemMixFirstItemId = 0;
		m_bNeedSecondCheck = false;
		m_iGemRemoveSelectIndex = -1;
		m_GemRemoveSelectGemId = 0;

		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("MixShopGemPanel");
		if (pPanel == NULL)
		{
			XML_UI::GetInstance().Init();
			XML_UI::GetInstance().ReadFormFile("Data\\MixShopGem.xml", gGlobal.pUIManager);

			pPanel = gGlobal.pUIManager->FindUIPanel("MixShopGemPanel");
			if (pPanel == NULL)
				return;		
		}

		pPanel->SetPos(20, 40);

		StringA uiItemName;
		UIImage* img = NULL;
		UIAniIcon* pAniIcon = NULL;

		uiItemName.Format("MixShopTitle%02d", m_pMixShop->type);
		img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
		if (img)
			img->SetEnable(true);

		UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopGemEqDrill");
		UIButton* button1 = gGlobal.pUIManager->FindUIButton("MixShopGemEqRemove");
		UIButton* button2 = gGlobal.pUIManager->FindUIButton("MixShopGemEqInlay");
		UIButton* button3 = gGlobal.pUIManager->FindUIButton("MixShopGemMix");
		UIButton* button4 = gGlobal.pUIManager->FindUIButton("MixShopGemMelting");
		UIButton* button5 = gGlobal.pUIManager->FindUIButton("MixShopGemPolishing");
		UIButton* button6 = gGlobal.pUIManager->FindUIButton("MixShopColorStoneMix");
		if (button && button1 && button2 && button3 && button4 && button5 && button6)
		{
			button->SetDisableState();
			button1->SetDisableState();
			button2->SetDisableState();
			button3->SetDisableState();
			button4->SetDisableState();
			button5->SetDisableState();
			button6->SetDisableState();

			switch(m_pMixShop->type)
			{
			case MIX_EQUIPDRILL:	button->SetEnable(true);		break;
			case MIX_GEMINLAY:		button2->SetEnable(true);		break;
			case MIX_GEMREMOVE:		button1->SetEnable(true);		break;
			case MIX_GEMMIX:		button3->SetEnable(true);		break;
			case MIX_GEMMELTING:	button4->SetEnable(true);		break;
			case MIX_GEMPOLISHING:	button5->SetEnable(true);		break;
			case MIX_COLORSTONEMIX:	button6->SetEnable(true);		break;
			}
		}

		/*UIStaticText* text1 = gGlobal.pUIManager->FindUIStaticText("MixShopGemEqItemName");
		UIStaticText* text2 = gGlobal.pUIManager->FindUIStaticText("MixShopGemReqItemName");

		if (text1 && text2)
		{
		text1->SetText(m_pMixShop->slotItemName01);
		text1->Redraw();
		text1->SetEnable(true);

		text2->SetText(m_pMixShop->slotItemName02);
		text2->Redraw();
		text2->SetEnable(true);
		}

		UIImage* img1 = gGlobal.pUIManager->FindUIImage("MixShopGemEqItemBG");
		UIImage* img2 = gGlobal.pUIManager->FindUIImage("MixShopGemReqItemBG");

		if (img1 && img2)
		{
		img1->SetEnable(true);
		img2->SetEnable(true);
		}

		UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel("MixShopGemSuggest");
		if (pChatPanel)
		{
		pChatPanel->ClearAllRow();
		pChatPanel->AddStringW(String(m_pMixShop->suggestion).c_str());
		pChatPanel->EndRow();
		pChatPanel->Redraw();
		pChatPanel->SetEnable(true);
		}*/

		UpdateGem();

		//ShowGemConsume(true);
		gGlobal.pUIManager->SetPanel2Top(pPanel);
		pPanel->SetEnable(true);
	}
}

void CMenuMixProcess::UpdateIronMakeOrRepair()
{
	if (m_pMixShop)
	{		
		UIAniIcon* pAniIcon = NULL;
		UIImage* img = NULL;

		for (Index i = C_INDEX(0); i < C_INDEX(2); ++i)
		{
			StringA uiName;
			if (m_pMixShop->type == MIX_IRONMAKE)
			{
				uiName.Format("IronMakeDragItemBlank%02d", i);	
				img = gGlobal.pUIManager->FindUIImage(uiName.c_str());
				uiName.Format("IronMakeDragItem%02d", i);
				pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiName.c_str());
			}
			else if (m_pMixShop->type == MIX_IRONREPAIR)
			{
				uiName.Format("IronRepairDragItemBlank%02d", i);
				img = gGlobal.pUIManager->FindUIImage(uiName.c_str());
				uiName.Format("IronRepairDragItem%02d", i);
				pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiName.c_str());
			}

			if (pAniIcon && img)
			{
				img->SetEnable(true);

				if (m_mixShopItem[i].itemSlots.size() > 0)
				{
					pAniIcon->ClearAniData();
					CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[i].itemSlots[0]);
					if (item && item->m_data)
					{
						pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
						pAniIcon->SetRenderStyle(0);
						pAniIcon->SetEnable(true);
					}
				}
				else
					pAniIcon->SetEnable(false);
			}
		}
	}
}

void CMenuMixProcess::UpdateMixOrRepairItem()
{
	if (m_pMixShop)
	{
		UIAniIcon* pAniIcon = NULL;
		UIStaticText* pText = NULL;
		UIStaticText* pStackText = NULL;

		for (Index i = C_INDEX(0); i < 3; ++i)
		{
			UInt index = i + m_iStartProductIndex;
			UInt slotSwitch = 0;
			UInt slotItemType = 0;
			UInt32 slotItemID = 0;
			String slotName;

			StringA uiName;
			StringA textName;
			uiName.Format("MixShopItem%02d", i);
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiName.c_str());
			textName.Format("%sName", uiName.c_str());
			pText = gGlobal.pUIManager->FindUIStaticText(textName.c_str());
			textName.Format("%sStack", uiName.c_str());
			pStackText = gGlobal.pUIManager->FindUIStaticText(textName.c_str());

			if (pAniIcon && pText && pStackText)
			{
				switch (index)
				{
				case 0:
					slotSwitch = m_pMixShop->slotSwitch01;
					slotItemType = m_pMixShop->slotItemType01;
					slotItemID = m_pMixShop->slotItemID01;
					slotName = String(m_pMixShop->slotItemName01);
					break;
				case 1:
					slotSwitch = m_pMixShop->slotSwitch02;
					slotItemType = m_pMixShop->slotItemType02;
					slotItemID = m_pMixShop->slotItemID02;
					slotName = String(m_pMixShop->slotItemName02);
					break;
				case 2:
					slotSwitch = m_pMixShop->slotSwitch03;
					slotItemType = m_pMixShop->slotItemType03;
					slotItemID = m_pMixShop->slotItemID03;
					slotName = String(m_pMixShop->slotItemName03);
					break;
				case 3:
					slotSwitch = m_pMixShop->slotSwitch04;
					slotItemType = m_pMixShop->slotItemType04;
					slotItemID = m_pMixShop->slotItemID04;
					slotName = String(m_pMixShop->slotItemName04);
					break;
				case 4:
					slotSwitch = m_pMixShop->slotSwitch05;
					slotItemType = m_pMixShop->slotItemType05;
					slotItemID = m_pMixShop->slotItemID05;
					slotName = String(m_pMixShop->slotItemName05);
					break;
				case 5:
					slotSwitch = m_pMixShop->slotSwitch06;
					slotItemType = m_pMixShop->slotItemType06;
					slotItemID = m_pMixShop->slotItemID06;
					slotName = String(m_pMixShop->slotItemName06);
					break;
				}

				if (slotSwitch)
				{
					if (m_mixShopItem[index].itemSlots.size() > 0)
					{
						pAniIcon->ClearAniData();
						CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[index].itemSlots[0]);
						if (item && item->m_data)
						{
							pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
							pAniIcon->SetRenderStyle(0);
							pAniIcon->SetEnable(true);
						}
					}
					else if (slotItemID > 0)
					{
						const ItemData* pItemData = gGlobal.m_resource->GetItemData(slotItemID);
						if (pItemData)
						{
							pAniIcon->SetAnimationData(String(pItemData->icon2).c_str());
							pAniIcon->SetRenderStyle(1);
							pAniIcon->SetEnable(true);
						}
					}
					else
						pAniIcon->SetEnable(false);
					
					pText->SetText(slotName.c_str());
					pText->Redraw();
					pText->SetEnable(true);

					if (m_mixShopItem[index].nowStack > 1)
					{
						String number;
						number.Format(_T("%d"), m_mixShopItem[index].nowStack);
						pStackText->SetText(number.c_str());
						pStackText->Redraw();
						pStackText->SetEnable(true);
					}
					else	
						pStackText->SetEnable(false);
				}
				else
				{
					pAniIcon->SetEnable(false);
					pText->SetEnable(false);
					pStackText->SetEnable(false);
				}
			}
		}
	}
}

void CMenuMixProcess::OpenMixShopEqMaking()
{
	if (m_pMixShop)
	{
		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("MixShopMakePanel");
		if (pPanel == NULL)
		{
			XML_UI::GetInstance().Init();
			XML_UI::GetInstance().ReadFormFile("Data\\MixShopMake.xml", gGlobal.pUIManager);

			pPanel = gGlobal.pUIManager->FindUIPanel("MixShopMakePanel");
			if (pPanel == NULL)
				return;		
		}

		pPanel->SetPos(20, 40);

		StringA uiItemName;
		UIImage* img = NULL;
	
		uiItemName.Format("MixShopTitle%02d", m_pMixShop->type);
		img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
		if (img)
			img->SetEnable(true);

		uiItemName = "MixShopButtonMake";
		UIButton* button = gGlobal.pUIManager->FindUIButton(uiItemName.c_str());
		if (button)
		{
			button->SetDisableState();
			button->SetEnable(true);
		}

		uiItemName = "MixShopButtonCancel";
		button = gGlobal.pUIManager->FindUIButton(uiItemName.c_str());
		if (button)
			button->SetEnable(true);
		
		UpdateSelectItemList();

		gGlobal.pUIManager->SetPanel2Top(pPanel);
		pPanel->SetEnable(true);
	}
}

void CMenuMixProcess::OpenMixShopLiveSkill()
{
	if (m_pMixShop)
	{
		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("MixShopMakePanel");
		if (pPanel == NULL)
		{
			XML_UI::GetInstance().Init();
			XML_UI::GetInstance().ReadFormFile("Data\\MixShopMake.xml", gGlobal.pUIManager);

			pPanel = gGlobal.pUIManager->FindUIPanel("MixShopMakePanel");
			if (pPanel == NULL)
				return;		
		}

		pPanel->SetPos(20, 40);

		StringA uiItemName;
		UIImage* img = NULL;
	
		uiItemName.Format("MixShopTitle%02d", m_pMixShop->type);
		img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
		if (img)
			img->SetEnable(true);

		uiItemName = "MixShopButtonMake2";
		UIButton* button = gGlobal.pUIManager->FindUIButton(uiItemName.c_str());
		if (button)
		{
			button->SetDisableState();
			button->SetEnable(true);
		}

		uiItemName = "MixShopButtonAllMake";
		button = gGlobal.pUIManager->FindUIButton(uiItemName.c_str());
		if (button)
		{
			button->SetDisableState();
			button->SetEnable(true);
		}

		uiItemName = "MixShopButtonCancel2";
		button = gGlobal.pUIManager->FindUIButton(uiItemName.c_str());
		if (button)
			button->SetEnable(true);

		uiItemName = "MixShopButtonAdd";
		button = gGlobal.pUIManager->FindUIButton(uiItemName.c_str());
		if (button)
			button->SetEnable(true);

		uiItemName = "MixShopButtonSub";
		button = gGlobal.pUIManager->FindUIButton(uiItemName.c_str());
		if (button)
			button->SetEnable(true);

		uiItemName = "MixShopMake2CountBg";
		img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
		if (img)
			img->SetEnable(true);

		uiItemName = "MixShopMake2Count";
		UIEditBox* editBox = gGlobal.pUIManager->FindUIEditBox(uiItemName.c_str());
		if (editBox)
			editBox->SetEnable(true);
		
		UpdateSelectItemList();
		gGlobal.pUIManager->SetPanel2Top(pPanel);
		pPanel->SetEnable(true);
	}
}

void CMenuMixProcess::UpdateSelectItemList()
{
	UInt count = 0;
	UInt listCount = 0;

	m_showList.clear();
	StringA checkBoxName;
	StringA textName;
	for (ItemMixTypeList::iterator itr = m_typeItemList.begin(); itr != m_typeItemList.end(); ++itr)
	{
		ItemMixType* itemList = itr->second;
		if (m_iStartProductIndex <= listCount)
		{
			checkBoxName.Format("MixShopCheckBox%02d", count);
			UICheckBox* checkBox = gGlobal.pUIManager->FindUICheckBox(checkBoxName.c_str());
			textName.Format("MixShopItemName%02d", count);
			UIStaticText* text = gGlobal.pUIManager->FindUIStaticText(textName.c_str());
			if (checkBox && text)
			{
				String typeName;
				UInt8 typeNum = itr->first;
				if (typeNum >= 100)
				{
					typeNum -= 100;
					switch(typeNum)
					{
					case EQTYPE_SPEAR:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_SPEAR"));	break;
					case EQTYPE_WIPE:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_WIPE"));	break;
					case EQTYPE_FAN:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_FAN"));		break;
					case EQTYPE_PEN:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_PEN"));		break;
					case EQTYPE_SWORD:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_SWORD"));	break;
					case EQTYPE_DBSWORD:typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_DBSWORD"));	break;
					case EQTYPE_BLADE:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_BLADE"));	break;
					case EQTYPE_OTHER:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_OTHER"));	break;
					case EQTYPE_HANDRING:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_HANDRING"));	break;
					case EQTYPE_CLAW:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_CLAW"));	break;
					case EQTYPE_GEAR:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_GEAR"));	break;
					case EQTYPE_ARMOR:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_ARMOR"));	break;
					case EQTYPE_SHOE:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_SHOE"));	break;
					case EQTYPE_BELT:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_BELT"));	break;
					case EQTYPE_RING:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_RING"));	break;
					case EQTYPE_AMULET:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_AMULET"));	break;
					case EQTYPE_CLOTH:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_EQTYPE_CLOTH"));	break;
					case EQTYPE_HAT_WITH_HAIR:			break;
					case EQTYPE_HAT_WITHOUT_HAIR:		break;
					}
				}
				else
				{
					switch(typeNum)
					{
					case ITEMTYPE_MISC:		typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_MISC")); break;
					case ITEMTYPE_EQUIP:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_EQUIP")); break;
					case ITEMTYPE_FOOD:		typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_FOOD")); break;
					case ITEMTYPE_FOODMAT:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_FOODMAT")); break;
					case ITEMTYPE_DRUG:		typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_DRUG")); break;
					case ITEMTYPE_DRUGMAT:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_DRUGMAT")); break;
					case ITEMTYPE_WINE:		typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_WINE")); break;
					case ITEMTYPE_WINEMAT:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_WINEMAT")); break;
					case ITEMTYPE_EQUIPMAT: typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_EQUIPMAT")); break;
					case ITEMTYPE_CLOTHMAT: typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_CLOTHMAT")); break;
					case ITEMTYPE_GEM:		typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_GEM")); break;
					case ITEMTYPE_THROW:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_THROW")); break;
					case ITEMTYPE_SKILLBOOK: typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_SKILLBOOK")); break;
					case ITEMTYPE_SCRIPT:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_SCRIPT")); break;
					case ITEMTYPE_ADDON:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_ADDON")); break;
					case ITEMTYPE_ARRAYBOOK: typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_ARRAYBOOK")); break;
					case ITEMTYPE_SPECIAL:	typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_SPECIAL")); break;
					case ITEMTYPE_BAG:		typeName =gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_BAG")); break;
					case ITEMTYPE_BUSINESS_ITEM:		typeName = gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_BUSINESS_ITEM")); break;
					case ITEMTYPE_BUSINESS_MONEY:		typeName = gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_BUSINESS_MONEY")); break;
					case ITEMTYPE_FAST_FOOD: typeName = gGlobal.GetStringTable()->Get(_T("MSG_ITEMTYPE_FAST_FOOD")); break;
					}
				}
				text->SetText(typeName.c_str());
				text->SetColor(0xFFFFFFFF);
				text->Redraw();
				text->SetEnable(true);

				ShowItem temp;
				temp.type = itr->first;
				temp.item = NULL;
				m_showList.push_back(temp);
				++count;

				if (itemList->isOpen)
					checkBox->SetState(true);
				else
					checkBox->SetState(false);

				checkBox->SetEnable(true);
			}
		}
		++listCount;

		if (itemList->isOpen)
		{
			if (count > MAX_MIX_MAKE_LINE_COUNT)
				break;
			for (UInt16 i = 0; i < itemList->dataList.size(); ++i)
			{
				if (m_iStartProductIndex <= listCount)
				{
					checkBoxName.Format("MixShopCheckBox%02d", count);
					UICheckBox* checkBox1 = gGlobal.pUIManager->FindUICheckBox(checkBoxName.c_str());
					if (checkBox1)
						checkBox1->SetEnable(false);

					textName.Format("MixShopItemName%02d", count);
					UIStaticText* text1 = gGlobal.pUIManager->FindUIStaticText(textName.c_str());
					if (text1)
					{						
						String showName;
						CSkill* liveSkill = gGlobal.g_pSkillProcess->GetSkill(itemList->dataList[i]->liveSkillID);
						showName.Format(_T("\t%s"), String(itemList->dataList[i]->productName).c_str());
						text1->SetText(showName.c_str());
						if (liveSkill && liveSkill->GetSkill_level() >= itemList->dataList[i]->liveSkillLv)
							text1->SetColor(0xFF00FF00);
						else if(liveSkill && liveSkill->GetSkill_level() < itemList->dataList[i]->liveSkillLv)
							text1->SetColor(0xFF7F7F7F);
						else if(itemList->dataList[i]->liveSkillID == 0)
							text1->SetColor(0xFFFFFFFF);
						else
							text1->SetColor(0xFF7F7F7F);
						text1->Redraw();
						text1->SetEnable(true);

						ShowItem temp;
						temp.type = itr->first;
						temp.item = itemList->dataList[i];
						m_showList.push_back(temp);
					}
					
					++count;
					if (count > MAX_MIX_MAKE_LINE_COUNT)
						break;
				}
				++listCount;
			}
		}

		if (count > MAX_MIX_MAKE_LINE_COUNT)
			break;
	}

	while (count < MAX_MIX_MAKE_LINE_COUNT)
	{
		StringA checkBoxName;
		StringA textName;
		checkBoxName.Format("MixShopCheckBox%02d", count);
		UICheckBox* checkBox = gGlobal.pUIManager->FindUICheckBox(checkBoxName.c_str());
		textName.Format("MixShopItemName%02d", count);
		UIStaticText* text = gGlobal.pUIManager->FindUIStaticText(textName.c_str());
		if (checkBox && text)
		{
			checkBox->SetEnable(false);
			text->SetEnable(false);
		}
		ShowItem temp;
		temp.type = MIX_SHOP_NONE;
		temp.item = NULL;
		m_showList.push_back(temp);
		++count;
	}

	UIImage* img = gGlobal.pUIManager->FindUIImage("MixShopItemSelected");
	if (img)
	{
		if (m_iSelectedIndex >= 0 && m_iSelectedIndex < MAX_MIX_MAKE_LINE_COUNT)
		{
			StringA checkBoxName;
			checkBoxName.Format("MixShopCheckBox%02d", m_iSelectedIndex);
			UICheckBox* checkBox = gGlobal.pUIManager->FindUICheckBox(checkBoxName.c_str());
			if (checkBox && !checkBox->IsEnable())
			{
				img->SetOffsetXY(img->GetOffsetX(), checkBox->GetOffsetY() - 5);
				img->SetEnable(true);
			}
		}
		else
			img->SetEnable(false);
	}
}

void CMenuMixProcess::SetMixItemText(StringA componentName, String text, Color32 textColor, bool addDirty)
{
	UIStaticText* pST = gGlobal.pUIManager->FindUIStaticText(componentName.c_str());
	if (pST)
	{
		pST->SetText(text.c_str());
		if (textColor != 0)
			pST->SetFontColor(textColor);
		pST->Redraw();
		if( addDirty )
			gGlobal.AddDirtyUIItem(componentName.c_str());
	}
}

void CMenuMixProcess::SetUIMixItemVerticalPosAndEnable(StringA component, UInt curY, bool enabled)
{
	UIItem* pUI = gGlobal.pUIManager->GetUIItem(component.c_str());
	if (pUI)
	{
		UInt posX = pUI->GetOffsetX();
		UInt posY = pUI->GetOffsetY();
		pUI->SetOffsetXY(posX, curY);
		pUI->SetEnable(enabled);
	}
}

void CMenuMixProcess::SetMixItemRowEnable(StringA prefix, bool show, UInt curY, String showText, Color32 textColor)
{
	StringA detailText;
	detailText.Format("MixDetailText%s", prefix.c_str());

	if (show)
	{	
		SetMixItemText(detailText, showText, textColor);
		SetUIMixItemVerticalPosAndEnable(detailText, curY, true);
	}
	else
	{
		SetUIEnable(detailText, false);
	}
}

void CMenuMixProcess::UpdateMixItemDetail(int itemPos)
{ 
	if( m_showList[itemPos].item )
	{
		String Text = _T("");
		UInt initX = 0, initY = 0;
		UInt curY = 0;
		GetUIPos("MixDetailTextDesc", initX, initY);
		curY = initY;

		Text.Format(_T("%s"), m_showList[itemPos].item->proDescription);
		if( Text.GetLength()>0 )
		{
			SetMixItemRowEnable("Desc", true, curY, Text);
			curY += (Text.GetLength() / 19 + 1) * 16;
			curY += 10;
		}
		else
			SetMixItemRowEnable("Desc", false);

		Text = m_showList[itemPos].item->proFunctionDes;
		if( Text.GetLength()>0 )
		{
			SetMixItemText("MixDetailTextFunctionDesc", Text, TextColor[ColorIndex_Green]);
			SetUIMixItemVerticalPosAndEnable("MixDetailTextFunctionDesc", curY, true);
			curY += ((Text.GetLength()+ 20) / 21) * 16;
		}
		else
			SetUIEnable("MixDetailTextFunctionDesc", false);

		UIPanelEx* uPan = gGlobal.pUIManager->FindUIPanelEx("MixDetailMenuPanel");
		if( uPan )
		{
			UInt count = curY / 13;
			uPan->ChangeCount(count);
		}
	}
}

bool CMenuMixProcess::isShowItem(int index)
{ 
	StringA textName;
	textName.Format("MixShopItemName%02d", index);
	UIStaticText* text = gGlobal.pUIManager->FindUIStaticText(textName.c_str());

	if(m_showList[index].item != NULL && text->GetColor() != 0xFF7F7F7F) 
		return true; 
	else 
		return false;
}

void CMenuMixProcess::SetSelectedIndex(int index)
{
	StringA textName;
	textName.Format("MixShopItemName%02d", index);
	UIStaticText* text = gGlobal.pUIManager->FindUIStaticText(textName.c_str());
	if(text->GetColor() == 0xFF7F7F7F)
		return;
	UIImage* img = gGlobal.pUIManager->FindUIImage("MixShopItemSelected");
	if (img)
	{
		ResetItemMix();
		UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon("MixShopMakeBigIcon");
		if (m_showList[index].item == NULL)
		{
			ItemMixTypeList::iterator itr = m_typeItemList.find(m_showList[index].type);
			if (itr != m_typeItemList.end())
			{
				itr->second->isOpen = !itr->second->isOpen;
				m_iShowLineCount = GetShowLineExist();
				if (m_iShowLineCount < (m_iStartProductIndex + MAX_MIX_MAKE_LINE_COUNT))
				{
					if (m_iShowLineCount > MAX_MIX_MAKE_LINE_COUNT)
						m_iStartProductIndex = m_iShowLineCount - MAX_MIX_MAKE_LINE_COUNT;
					else
						m_iStartProductIndex = 0;
				}

				UpdateSelectItemList();
				m_pSelectedItemMix = NULL;
				img->SetEnable(false);

				ShowMakeConsume(false);
				UpdateItemMixSource();
				m_iSelectedIndex = -1;
				ResetProductScrollBar();
				if (pAniIcon)
					pAniIcon->SetEnable(false);
			}
		}
		else
		{
			m_pSelectedItemMix = m_showList[index].item;

			StringA imgName;
			imgName.Format("MixShopItem%02d", index);
			UIImage* img1 = gGlobal.pUIManager->FindUIImage(imgName.c_str());
			if (img1)
			{
				img->SetOffsetXY(img->GetOffsetX(), img1->GetOffsetY());
				img->SetEnable(true);
			}

			if (pAniIcon)
			{
				for (Index i = C_INDEX(0); i < m_pSelectedItemMix->output.size(); ++i)
				{
					if (m_pSelectedItemMix->output[i] > 0)
					{
						const ItemData* pItemData = gGlobal.m_resource->GetItemData(m_pSelectedItemMix->output[i]);
						if (pItemData)
						{
							pAniIcon->ClearAniData();
							pAniIcon->SetAnimationData(String(pItemData->icon1).c_str());
							pAniIcon->SetEnable(true);
						}
						break;
					}
				}
			}
			MakeSourceList();
			UpdateItemMixSource();
			ShowMakeConsume(true);
			m_iSelectedIndex = index;
		}
	}
}

void CMenuMixProcess::ResetIronItem()
{
	for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
		ClearItemMixSourceList(m_mixShopItem, i);

	UIButton* buttonM = gGlobal.pUIManager->FindUIButton("IronShopButtonMake");
	UIButton* buttonR = gGlobal.pUIManager->FindUIButton("IronShopButtonRepair");
	if (buttonM && buttonR)
	{
		buttonM->SetDisableState();
		buttonR->SetDisableState();		
		gGlobal.pUIManager->AddDirtyUIItem(buttonM);	
		gGlobal.pUIManager->AddDirtyUIItem(buttonR);
	}
}

void CMenuMixProcess::ResetIronShop()
{
	ItemMixTypeList::iterator itr = m_typeItemList.begin();
	while(itr != m_typeItemList.end())
	{
		itr->second->dataList.clear();
		ItemMixType* temp = itr->second;
		SafeDeleteObject(temp);	
		++itr;
	}

	m_typeItemList.clear();
	m_pMixShop = NULL;
	m_iStartProductIndex = 0;
	m_pSelectedItemMix = NULL;
	m_iStartSourceIndex = 0;
	m_iStartTime = 0;
	ResetIronItem();
	if (m_isOpenMixShop)
		m_isOpenMixShop = false;
}

void CMenuMixProcess::ResetMixShopMake()
{
	ItemMixTypeList::iterator itr = m_typeItemList.begin();
	while(itr != m_typeItemList.end())
	{
		itr->second->dataList.clear();
		ItemMixType* temp = itr->second;
		SafeDeleteObject(temp);	
		++itr;
	}

	m_typeItemList.clear();
	m_showList.clear();
	m_pMixShop = NULL;
	m_iSelectedIndex = -1;
	m_iStartProductIndex = 0;
	m_pSelectedItemMix = NULL;
	m_iStartSourceIndex = 0;
	m_iStartTime = 0;
	m_bMakeAll = false;
	m_iRemainItemCount = 0;
	m_bHasLightStone = false;
	m_iStartMixShopX = 0;
	m_iStartMixShopY = 0;		

	ResetItemMix();	
}

void CMenuMixProcess::ResetMixShop()
{
	m_pMixShop = NULL;
	m_iSelectedIndex = -1;
	m_iStartProductIndex = 0;
	m_iStartTime = 0;
	m_iStartMixShopX = 0;
	m_iStartMixShopY = 0;		
	ResetAllMixItem();
}

void CMenuMixProcess::ResetAllMixItem()
{
	for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
		ClearItemMixSourceList(m_mixShopItem, i);

	if (gGlobal.pUIManager)
	{
		UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonRepair");
		UIButton* button1 = gGlobal.pUIManager->FindUIButton("MixShopButtonRebuild");
		UIButton* button2 = gGlobal.pUIManager->FindUIButton("MixShopButtonStoneMix");
		UIButton* button3 = gGlobal.pUIManager->FindUIButton("MixShopButtonUnSoulBound");
		if (button && button1 && button2 && button3 && m_pMixShop)
		{
			button->SetDisableState();
			button1->SetDisableState();
			button2->SetDisableState();
			button3->SetDisableState();
			switch (m_pMixShop->type)
			{
			case MIX_UNSOULBOUND:
				{
					button->SetEnable(false);
					button1->SetEnable(false);
					button2->SetEnable(false);
					button3->SetEnable(true);
				}
				break;
			case MIX_EQUIPREPAIR:
				{
					button->SetEnable(true);
					button1->SetEnable(false);
					button2->SetEnable(false);
					button3->SetEnable(false);
				}
				break;
			case MIX_STONEMIX:
				{
					button->SetEnable(false);
					button1->SetEnable(false);
					button2->SetEnable(true);
					button3->SetEnable(false);
				}
				break;
			}
			
			gGlobal.pUIManager->AddDirtyUIItem(button);			
			gGlobal.pUIManager->AddDirtyUIItem(button1);
			gGlobal.pUIManager->AddDirtyUIItem(button2);
			gGlobal.pUIManager->AddDirtyUIItem(button3);
		}
	}
	m_pEqItem = NULL;
	if (m_isOpenMixShop)
	{
		m_isOpenMixShop = false;
	}
}

void CMenuMixProcess::MixShopMakeProductScroll(UInt8 up)
{
	// up 0: up, 1: down, 2: scroll
	if ( up == 0)
	{
		if (m_iStartProductIndex > 0)
		{
			--m_iStartProductIndex;
			if (m_pSelectedItemMix)
				++m_iSelectedIndex;
			UpdateSelectItemList();
		}
	}
	else if (up == 1)
	{
		if (m_iShowLineCount > (m_iStartProductIndex + MAX_MIX_MAKE_LINE_COUNT))
		{
			++m_iStartProductIndex;
			if (m_pSelectedItemMix)
				--m_iSelectedIndex;
			UpdateSelectItemList();
		}
	}
	else if (up == 2)
	{
		UIScrollBarV* scrollBar = gGlobal.pUIManager->FindUIScrollBarV("MixShopProductScrollBar");
		if (scrollBar)
		{
			UInt32 index = scrollBar->GetSelNum();
			if (index != m_iStartProductIndex)
			{
				if (m_iStartProductIndex > index) //scroll up
					m_iSelectedIndex += (m_iStartProductIndex - index);
				else	//scroll down
					m_iSelectedIndex -= (index - m_iStartProductIndex);

				m_iStartProductIndex = index;
				UpdateSelectItemList();
			}
		}
	}
}

void CMenuMixProcess::MixShopMakeSourceScroll(UInt8 up)
{
	if ( up == 0)
	{
		if (m_iStartSourceIndex > 0)
		{
			--m_iStartSourceIndex;
			UpdateItemMixSource();
		}
	}
	else if (up == 1)
	{
		
			++m_iStartSourceIndex;
			UpdateItemMixSource();
		
	}
	else if (up == 2)
	{
		
	}
}

UInt16 CMenuMixProcess::GetShowLineExist()
{
	UInt16 count = 0;
	for (ItemMixTypeList::iterator itr = m_typeItemList.begin(); itr != m_typeItemList.end(); ++itr)
	{
		++count;
		if (itr->second->isOpen)
			count += (UInt16)itr->second->dataList.size();
	}
	
	return count;
}

void CMenuMixProcess::UpdateItemMixSource()
{
	UIAniIcon* pAniIcon = NULL;
	UIStaticText* name = NULL;
	UIStaticText* stack = NULL;
	UIImage* img = NULL;
		
	for (Index i = C_INDEX(0); i < 4; ++i)
	{
		UInt index = i + 2 * m_iStartSourceIndex;
		StringA iconName;
		iconName.Format("MixShopDragItemBlank0%d", i);
		img = gGlobal.pUIManager->FindUIImage(iconName.c_str());

		iconName.Format("MixShopDragItem0%d", i);
		pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());

		iconName.Format("MixShopDragItemName0%d", i);
		name = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());

		iconName.Format("MixShopDragItemStack0%d", i);
		stack = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());

		if (index < m_sourceList.size() && img && pAniIcon && name && stack)
		{
			if (m_sourceList[index].sourceRootID != 0)
			{
				const ItemData* pItemData = NULL;
				
				if (m_sourceList[index].itemSlots.size() == 0)
					pItemData = gGlobal.m_resource->GetItemData(m_sourceList[index].sourceRootID);
				else
				{
					CItem* tempItem = gGlobal.m_pBag->GetItem(m_sourceList[index].itemSlots[0]);
					if (tempItem)
						pItemData = tempItem->m_data;
				}

				if (pItemData)
				{
					img->SetEnable(true);
					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(String(pItemData->icon2).c_str());
					if (m_sourceList[index].nowStack == 0)
						pAniIcon->SetRenderStyle(1);
					else
						pAniIcon->SetRenderStyle(0);
					pAniIcon->SetEnable(true);

					name->SetText(String(pItemData->name).c_str());
					name->Redraw();
					name->SetEnable(true);

					String text;
					text.Format(_T("%d / %d"), m_sourceList[index].nowStack, m_sourceList[index].maxStack);
					stack->SetText(text.c_str());
					stack->Redraw();
					stack->SetEnable(true);
				}
			}
			else
			{
				img->SetEnable(false);
				pAniIcon->SetEnable(false);
				name->SetEnable(false);
				stack->SetEnable(false);
			}	
		}
	}
}

void CMenuMixProcess::MakeSourceList()
{
	m_bHasLightStone = false;
	if (m_pSelectedItemMix)
	{
		for (Index i = C_INDEX(0); i < m_sourceList.size(); ++i)
		{
			m_sourceList[i].sourceRootID = m_pSelectedItemMix->materials[i];
			m_sourceList[i].maxStack = m_pSelectedItemMix->numOfMaterials[i];
			m_sourceList[i].itemSwitch = m_pSelectedItemMix->materialSwitch[i];
			m_sourceList[i].nowStack = 0;
			m_sourceList[i].nowStacks.clear();
			m_sourceList[i].itemSlots.clear();
			m_sourceList[i].item_uids.clear();

			if (m_pSelectedItemMix->materialSwitch[i] == MIX_SWITCH_LIGHT_STONE)
				m_bHasLightStone = true;
		}
	}
	else
	{
		for (Index i = C_INDEX(0); i < m_sourceList.size(); ++i)
		{
			m_sourceList[i].sourceRootID = 0;
			m_sourceList[i].maxStack = 0;
			m_sourceList[i].nowStack = 0;
			m_sourceList[i].itemSwitch = 0;
			m_sourceList[i].nowStacks.clear();
			m_sourceList[i].itemSlots.clear();
			m_sourceList[i].item_uids.clear();
		}
	}
}

void CMenuMixProcess::StartTimeLine()
{
	m_iStartTime = ::timeGetTime();

	if (m_pMixShop)
	{
		StringA timeLineName;

		switch (m_pMixShop->type)
		{
		case MIX_WEAPONMAKING:	case MIX_CLOTHMAKING:	case MIX_ORNAMENTMAKING:
		case MIX_COOKING:		case MIX_HERMETIC:		case MIX_MAKEWINE:
			timeLineName = "MixShopMakeTimeLine";
			break;
		case MIX_EQUIPREPAIR:
		case MIX_STONEMIX:
		case MIX_UNSOULBOUND:
			timeLineName = "MixShopTimeLine";
			break;
		case MIX_GEMMIX:
		case MIX_GEMINLAY:
		case MIX_GEMREMOVE:
		case MIX_GEMMELTING:
		case MIX_GEMPOLISHING:
		case MIX_COLORSTONEMIX:
			timeLineName = "MixShopGemTimeLine";
			break;
		case MIX_IRONMAKE:
		case MIX_IRONREPAIR:
			timeLineName = "IronShopTimeLine";
			break;
		}

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" start time line, %s"), timeLineName.c_str());

		UILinerImage* temp = gGlobal.pUIManager->FindUILinerImage(timeLineName.c_str());
		if (temp)
			temp->SetEnable(true);
	}

}

void CMenuMixProcess::StartServerTime()
{
	if (m_pMixShop)
	{
		bool canStart = true;

		switch (m_pMixShop->type)
		{
		case MIX_WEAPONMAKING:
		case MIX_CLOTHMAKING:
		case MIX_ORNAMENTMAKING:
			if (m_pSelectedItemMix) //for make the item, such as weapon, cloth
			{
				canStart = IsConsumeAccept(m_pSelectedItemMix->consumeType, m_pSelectedItemMix->consumeAmount, true);
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonMake");
				UIImage* img = gGlobal.pUIManager->FindUIImage("MixShopBlockImage");
				if (canStart && button && img)
				{
					button->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
					img->SetEnable(true);
				}
			}
			break;
		case MIX_UNSOULBOUND:
			{
				canStart = IsConsumeAccept(m_pMixShop->consumeType, m_pMixShop->consumeAmount, true);	
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonUnSoulBound");

				if (canStart && button)
				{
					button->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
				}
			}
			break;
		case MIX_EQUIPREPAIR:
			{
				canStart = IsConsumeAccept(m_pMixShop->consumeType, m_pMixShop->consumeAmount, true);	
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonRepair");
				UIButton* button1 = gGlobal.pUIManager->FindUIButton("MixShopButtonRebuild");

				if (canStart && button && button1)
				{
					button->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
					button1->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button1);
				}
			}
			break;
		case MIX_STONEMIX:
			{
				canStart = IsConsumeAccept(m_pMixShop->consumeType, m_pMixShop->consumeAmount, true);	
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonStoneMix");
				if (canStart && button)
				{
					button->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
				}
			}
			break;
		case MIX_COOKING:
		case MIX_HERMETIC:
		case MIX_MAKEWINE:
			if (m_pSelectedItemMix) 
			{
				canStart = IsConsumeAccept(m_pSelectedItemMix->consumeType, m_pSelectedItemMix->consumeAmount, true);
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonMake2");
				UIButton* button2 = gGlobal.pUIManager->FindUIButton("MixShopButtonAllMake");
				UIImage* img = gGlobal.pUIManager->FindUIImage("MixShopBlockImage");
				if (canStart && button && button2 && img)
				{
					button->SetDisableState();
					button2->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
					gGlobal.pUIManager->AddDirtyUIItem(button2);
					img->SetEnable(true);
				}
			}
			break;
		case MIX_GEMMIX:
			{
				if (m_bNeedSecondCheck)
				{
					m_bNeedSecondCheck = false;
					gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMMIX_BOUNDTIPS")));
					return ;
				}

				canStart = IsConsumeAccept(m_pMixShop->consumeType, m_pMixShop->consumeAmount, true);	
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopGemMix");
				if (canStart && button)
				{
					button->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
				}
			}
			break;
		case MIX_GEMINLAY:
			{
				if (m_bNeedSecondCheck)
				{
					m_bNeedSecondCheck = false;
					gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_BOUNDTIPS")));
					return ;
				}

				canStart = IsConsumeAccept(m_pMixShop->consumeType, m_pMixShop->consumeAmount, true);	
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopGemEqInlay");
				if (canStart && button)
				{
					button->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
				}
			}
			break;
		case MIX_GEMREMOVE:
			{
				canStart = IsConsumeAccept(m_pMixShop->consumeType, m_pMixShop->consumeAmount, true);	
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopGemEqRemove");
				if (canStart && button)
				{
					button->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
				}
			}
			break;
		case MIX_GEMMELTING:
			{
				if (m_bNeedSecondCheck)
				{
					m_bNeedSecondCheck = false;

					UInt32 GemItemId = 0;
					if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
					{
						CItem* pGemItem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
						if (pGemItem)
						{
							GemItemId = pGemItem->GetItem_id();
						}
					}

					gGlobal.g_TempResult = 0;
					CallLuaFunc(gGlobal.g_pParentLua, "GetGemItemLevel", String(), GemItemId);
					UInt GemLevel = (UInt)gGlobal.g_TempResult;

					if (GemLevel == 1)
					{
						gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMMELTING_RESULTTIPS1")));
					}
					else if(GemLevel == 2)
					{
						gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMMELTING_RESULTTIPS2")));
					}

					return ;
				}

				canStart = IsConsumeAccept(m_pMixShop->consumeType, m_pMixShop->consumeAmount, true);	
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopGemMelting");
				if (canStart && button)
				{
					button->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
				}
			}
			break;
		case MIX_GEMPOLISHING:
			{
				if (m_bNeedSecondCheck)
				{
					m_bNeedSecondCheck = false;
					gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMPOLISHING_BOUNDTIPS")));
					return ;
				}

				canStart = IsConsumeAccept(m_pMixShop->consumeType, m_pMixShop->consumeAmount, true);	
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopGemPolishing");
				if (canStart && button)
				{
					button->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
				}
			}
			break;
		case MIX_COLORSTONEMIX:
			{
				canStart = IsConsumeAccept(m_pMixShop->consumeType, m_pMixShop->consumeAmount, true);	
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopGemPolishing");
				if (canStart && button)
				{
					button->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
				}
			}
			break;
		case MIX_IRONMAKE:
			{
				if (m_pSelectedItemMix) 
				{
					canStart = IsConsumeAccept(m_pSelectedItemMix->consumeType, m_pSelectedItemMix->consumeAmount, true);
					UIButton* button = gGlobal.pUIManager->FindUIButton("IronShopButtonMake");
					UIImage* img = gGlobal.pUIManager->FindUIImage("IronShopBlockImage");
					if (canStart && button && img)
					{
						button->SetDisableState();
						gGlobal.pUIManager->AddDirtyUIItem(button);
						img->SetEnable(true);
					}
				}
			}
			break;
		case MIX_IRONREPAIR:
			{
				canStart = IsConsumeAccept(m_pMixShop->consumeType, m_pMixShop->consumeAmount, true);	
				UIButton* button = gGlobal.pUIManager->FindUIButton("IronShopButtonRepair");
				UIImage* img = gGlobal.pUIManager->FindUIImage("IronShopBlockImage");
				if (canStart && button && img)
				{
					button->SetDisableState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
					img->SetEnable(true);
				}
			}
			break;
		}

		if (canStart)
		{
			gGlobal.m_pBag->MarkMixStartTime(gGlobal.pPeerProxy, RPCContext(), BOOLEAN_TRUE);			
		}
	}
}

void CMenuMixProcess::UpdateTimeLine(UInt32 currTime)
{
	if (gGlobal.pUIManager && gGlobal.m_pBag && m_iStartTime > 0 && currTime > m_iStartTime && m_pMixShop)
	{
		switch (m_pMixShop->type)
		{
		case MIX_WEAPONMAKING:
		case MIX_CLOTHMAKING:
		case MIX_ORNAMENTMAKING:
		case MIX_COOKING:
		case MIX_HERMETIC:
		case MIX_MAKEWINE:
			if (m_pSelectedItemMix)
			{
				UInt32 diffTime = currTime - m_iStartTime;
				UInt32 maxTime = m_pSelectedItemMix->consumeTime * 1000;
				if (diffTime > maxTime)
				{
					UILinerImage* temp = gGlobal.pUIManager->FindUILinerImage("MixShopMakeTimeLine");
					if (temp)
						temp->SetEnable(false);
					m_iStartTime = 0;
					UInt32Vector itemUID;
					UInt8Vector itemStack;
					for (Index i = C_INDEX(0); i < m_sourceList.size(); ++i)
					{
						if (m_sourceList[i].itemSlots.size() > 0)
						{
							for (Index j = C_INDEX(0); j < m_sourceList[i].itemSlots.size(); ++j)
							{
								CItem* item = gGlobal.m_pBag->GetItem(m_sourceList[i].itemSlots[j]);
								if (item)
								{
									if (item->GetLock())
									{
										String tempStr = _T("MSG_LOCK_ITEM_CANT_MIX");				
										gGlobal.PrintMessage(tempStr, CHANNELTYPE_SYSTEM);		
										return;
									}
									itemUID.push_back(item->GetItem_uid());
									itemStack.push_back(m_sourceList[i].nowStacks[j]);
								}
							}
						}
						else
							break;
					}

					if (itemUID.size() > 0 && itemStack.size() > 0 )
						gGlobal.m_pBag->MakeItemMix(gGlobal.pPeerProxy, RPCContext(), m_pSelectedItemMix->productID, m_pMixShop->shopID, itemUID, itemStack);
				}
				else
					gGlobal.SetUILinerImage("MixShopMakeTimeLine", maxTime, diffTime);
			}
			break;
		case MIX_EQUIPREPAIR:
		case MIX_STONEMIX:
		case MIX_UNSOULBOUND:
			{
				UInt32 diffTime = currTime - m_iStartTime;
				UInt32 maxTime = m_pMixShop->consumeTime * 1000;
				if (diffTime > maxTime)
				{
					UILinerImage* temp = gGlobal.pUIManager->FindUILinerImage("MixShopTimeLine");
					if (temp)
						temp->SetEnable(false);
					m_iStartTime = 0;

					UInt32Vector itemIDs;
					UInt8Vector itemStacks;
					
					for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
					{
						if (m_mixShopItem[i].itemSlots.size() > 0)
						{
							itemIDs.insert(itemIDs.end(), m_mixShopItem[i].item_uids.begin(), m_mixShopItem[i].item_uids.end());
							itemStacks.insert(itemStacks.end(), m_mixShopItem[i].nowStacks.begin(), m_mixShopItem[i].nowStacks.end());
						}
					}

					if (itemIDs.size() > 0 && m_pMixShop)
						gGlobal.m_pBag->MixItem(gGlobal.pPeerProxy, RPCContext(), m_pMixShop->shopID, itemIDs, itemStacks,0);
				}
				else
					gGlobal.SetUILinerImage("MixShopTimeLine", maxTime, diffTime);
			}
			break;
		case MIX_GEMMIX:
		case MIX_GEMINLAY:
		case MIX_GEMREMOVE:
		case MIX_GEMMELTING:
		case MIX_GEMPOLISHING:
		case MIX_COLORSTONEMIX:
			{
				UInt32 diffTime = currTime - m_iStartTime;
				UInt32 maxTime = m_pMixShop->consumeTime * 1000;
				if (diffTime > maxTime)
				{
					UILinerImage* temp = gGlobal.pUIManager->FindUILinerImage("MixShopGemTimeLine");
					if (temp)
						temp->SetEnable(false);
					m_iStartTime = 0;

					UInt8 removeGemIndex = 0;

					if (m_pMixShop->type == MIX_GEMREMOVE)
					{
						removeGemIndex = m_iGemRemoveSelectIndex;
					}

					UInt32Vector itemIDs;
					UInt8Vector itemStacks;

					for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
					{
						if (m_mixShopItem[i].itemSlots.size() > 0)
						{
							itemIDs.insert(itemIDs.end(), m_mixShopItem[i].item_uids.begin(), m_mixShopItem[i].item_uids.end());
							itemStacks.insert(itemStacks.end(), m_mixShopItem[i].nowStacks.begin(), m_mixShopItem[i].nowStacks.end());
						}
					}

					if (itemIDs.size() > 0 && m_pMixShop)
						gGlobal.m_pBag->MixItem(gGlobal.pPeerProxy, RPCContext(), m_pMixShop->shopID, itemIDs, itemStacks, removeGemIndex);
				}
				else
					gGlobal.SetUILinerImage("MixShopGemTimeLine", maxTime, diffTime);
			}
			break;
		case MIX_IRONMAKE:
			{
				if (m_pSelectedItemMix)
				{
					UInt32 diffTime = currTime - m_iStartTime;
					UInt32 maxTime = m_pSelectedItemMix->consumeTime * 1000;
					if (diffTime > maxTime)
					{
						UILinerImage* temp = gGlobal.pUIManager->FindUILinerImage("IronShopTimeLine");
						if (temp)
							temp->SetEnable(false);
						m_iStartTime = 0;
						UInt32Vector itemIDs;
						UInt8Vector itemStacks;

						for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
						{
							if (m_mixShopItem[i].itemSlots.size() > 0)
							{
								itemIDs.insert(itemIDs.end(), m_mixShopItem[i].item_uids.begin(), m_mixShopItem[i].item_uids.end());
								itemStacks.insert(itemStacks.end(), m_mixShopItem[i].nowStacks.begin(), m_mixShopItem[i].nowStacks.end());
							}
						}

						if (itemIDs.size() > 0 && m_pMixShop)
							gGlobal.m_pBag->MakeItemMix(gGlobal.pPeerProxy, RPCContext(), m_pSelectedItemMix->productID, m_pMixShop->shopID, itemIDs, itemStacks);
					}
					else
						gGlobal.SetUILinerImage("IronShopTimeLine", maxTime, diffTime);
				}
			}
			break;
		case MIX_IRONREPAIR:
			{
				UInt32 diffTime = currTime - m_iStartTime;
				UInt32 maxTime = m_pMixShop->consumeTime * 1000;
				if (diffTime > maxTime)
				{
					UILinerImage* temp = gGlobal.pUIManager->FindUILinerImage("IronShopTimeLine");
					if (temp)
						temp->SetEnable(false);
					m_iStartTime = 0;

					UInt32Vector itemIDs;
					UInt8Vector itemStacks;

					for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
					{
						if (m_mixShopItem[i].itemSlots.size() > 0)
						{
							itemIDs.insert(itemIDs.end(), m_mixShopItem[i].item_uids.begin(), m_mixShopItem[i].item_uids.end());
							itemStacks.insert(itemStacks.end(), m_mixShopItem[i].nowStacks.begin(), m_mixShopItem[i].nowStacks.end());
						}
					}

					if (itemIDs.size() > 0 && m_pMixShop)
						gGlobal.m_pBag->MixItem(gGlobal.pPeerProxy, RPCContext(), m_pMixShop->shopID, itemIDs, itemStacks,0);
				}
				else
					gGlobal.SetUILinerImage("IronShopTimeLine", maxTime, diffTime);
			}
			break;
		}
	}
}

bool CMenuMixProcess::HaveSelectedItemMix()
{
	if (m_pSelectedItemMix)
		return true;
	return false;
}

bool CMenuMixProcess::IsSuitableIronShopItem(int picIndex, int itemSlot)
{
	if (picIndex < 0 || picIndex >= 2 || m_pMixShop == NULL)
		return false;

	UInt index = m_iStartProductIndex + picIndex;
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);

	if (index >= 2 || itemSlot >= 120)
		return false;

	if (item)
	{
		if (item->GetLock())
			return false;
		UInt8 itemType = 0;
		UInt8 itemSwitch = 0;
		switch (index)
		{
		case 0:		
			itemSwitch = m_pMixShop->slotSwitch01;
			itemType = m_pMixShop->slotItemType01;
			break;
		case 1:		
			itemSwitch = m_pMixShop->slotSwitch02;
			itemType = m_pMixShop->slotItemType02;
			break;
		}
		if (itemSwitch == MIX_SWITCH_OFF)
			return false;
		else if (itemSwitch == MIX_SWITCH_ITEM && itemType == item->m_data->item_type && m_mixShopItem[index].itemSlots.size() == 0)
		{
			if (m_pMixShop->type == MIX_IRONMAKE)
					return true;
			else if (m_pMixShop->type == MIX_IRONREPAIR)
			{
				if (item->m_data->item_type == 1)
				{
					if(item->GetItem_id() >= 50600 && item->GetItem_id() <= 50695)
						return true;
				}
				else
					return true;		
			}			
		}
	}
	return false;
}

bool CMenuMixProcess::IsSuitableItemMix(int srcIconIndex, int itemSlot)
{
	if (srcIconIndex == MIX_SHOP_MAKE_DISPLAY_SOURCE)
		return false;

	UInt index = m_iStartSourceIndex * 2 + srcIconIndex;
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);
	if (item)
	{
		if (item->GetLock())
			return false;
		if (m_bHasLightStone && m_sourceList[index].itemSwitch == MIX_SWITCH_ITEM && item->GetItem_IdentifyState() == 1)
			return false;
		
		if ( ( (item->m_data->root_id == m_sourceList[index].sourceRootID && m_sourceList[index].itemSwitch == MIX_SWITCH_STONE) ||
			(item->GetItem_id() == m_sourceList[index].sourceRootID && (m_sourceList[index].itemSwitch == MIX_SWITCH_ITEM || m_sourceList[index].itemSwitch == MIX_SWITCH_LIGHT_STONE)) ) && 
			m_sourceList[index].sourceRootID != 0 )
		{
			if (m_sourceList[index].itemSlots.size() == 0)
                return true;
			else
			{
				CItem* tempItem = gGlobal.m_pBag->GetItem(m_sourceList[index].itemSlots[0]);
				if (tempItem && tempItem->GetItem_id() == item->GetItem_id() )
					return true;
			}
		}	
	}
	return false;
}

bool CMenuMixProcess::IsSuitableMixShopItem(int picIndex, int itemSlot)
{
	if (picIndex == MIX_SHOP_ITEM_DISPLAY_SOURCE || m_pMixShop == NULL)
		return false;

	UInt index = m_iStartProductIndex + picIndex;
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);

	if (index > 5 || itemSlot >= 120)
		return false;
	
	if (item)
	{
		if (item->GetLock())
			return false;
		UInt32 itemRootID = 0;
		UInt8 itemType = 0;
		UInt8 itemSwitch = 0;
		switch (index)
		{
		case 0:		
			itemSwitch = m_pMixShop->slotSwitch01;
			itemRootID = m_pMixShop->slotItemID01;
			itemType = m_pMixShop->slotItemType01;
			break;
		case 1:		
			itemSwitch = m_pMixShop->slotSwitch02;
			itemRootID = m_pMixShop->slotItemID02;
			itemType = m_pMixShop->slotItemType02;
			break;
		case 2:		
			itemSwitch = m_pMixShop->slotSwitch03;
			itemRootID = m_pMixShop->slotItemID03;
			itemType = m_pMixShop->slotItemType03;
			break;
		case 3:		
			itemSwitch = m_pMixShop->slotSwitch04;
			itemRootID = m_pMixShop->slotItemID04;
			itemType = m_pMixShop->slotItemType04;
			break;
		case 4:		
			itemSwitch = m_pMixShop->slotSwitch05;
			itemRootID = m_pMixShop->slotItemID05;
			itemType = m_pMixShop->slotItemType05;
			break;
		case 5:		
			itemSwitch = m_pMixShop->slotSwitch06;
			itemRootID = m_pMixShop->slotItemID06;
			itemType = m_pMixShop->slotItemType06;
			break;
		}

		if (itemSwitch == MIX_SWITCH_OFF)
			return false;
		else if (itemSwitch == MIX_SWITCH_ITEM && itemType == item->m_data->item_type)
		{
			//if (m_mixShopItemSlots[index] == MIX_SHOP_NO_ITEM)
			if (m_mixShopItem[index].itemSlots.size() == 0)
			{
				if (item->m_data->item_type == 1)
				{
					if(item->GetItem_id() < 50600 || item->GetItem_id() > 50695)
						return true;
				}
				else
					return true;
			}
			else 
			{
				switch (m_pMixShop->type)
				{
				case MIX_UNSOULBOUND:
				case MIX_STONEMIX:
					{
						CItem* tempItem = gGlobal.m_pBag->GetItem(m_mixShopItem[index].itemSlots[0]);
						if (tempItem && tempItem->GetItem_id() == item->GetItem_id())
							return true;
					}
					break;
				}
			}
		}
		else 
		{
			//if (itemRootID == item->m_data->root_id && m_mixShopItemSlots[index] == MIX_SHOP_NO_ITEM)
			if (itemRootID == item->m_data->root_id)
			{
				switch (m_pMixShop->type)
				{
				case MIX_EQUIPREPAIR:
					{
						if (m_mixShopItem[index].itemSlots.size() == 0)
						{
							for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
							{
								if (m_mixShopItem[i].itemSlots.size() > 0)
								{
									CItem* tempItem = gGlobal.m_pBag->GetItem(m_mixShopItem[i].itemSlots[0]);
									if (tempItem && tempItem->m_data && (tempItem->m_data->item_type == ITEMTYPE_REPAIR || tempItem->m_data->item_type == ITEMTYPE_REBUILD) )
										return false;
								}
							}
							return true;
						}
					}
					break;
				case MIX_UNSOULBOUND:
					{
						if (m_mixShopItem[index].itemSlots.size() == 0)
						{
							for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
							{
								if (m_mixShopItem[i].itemSlots.size() > 0)
								{
									CItem* tempItem = gGlobal.m_pBag->GetItem(m_mixShopItem[i].itemSlots[0]);
									if (tempItem && tempItem->m_data && tempItem->m_data->item_type == ITEMTYPE_UNSOULBOUND)
										return false;
								}
							}
							return true;
						}
					}
					break;
				default:
					return false;
				}
			}
		}
	}
	return false;
}

bool CMenuMixProcess::IsSuitableGemMixShopItem(int picIndex, int itemSlot)
{
	if (picIndex < 0 || picIndex >= 5 || m_pMixShop == NULL)
		return false;

	UInt index = m_iStartProductIndex + picIndex;
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);

	if (index >= 5 || itemSlot >= 120)
		return false;

	if (item)
	{
		if (item->GetLock())
			return false;
		UInt32 itemRootID = 0;
		UInt8 itemType = 0;
		UInt8 itemSwitch = 0;
		switch (index)
		{
		case 0:		
			itemSwitch = m_pMixShop->slotSwitch01;
			itemRootID = m_pMixShop->slotItemID01;
			itemType = m_pMixShop->slotItemType01;
			break;
		case 1:		
			itemSwitch = m_pMixShop->slotSwitch02;
			itemRootID = m_pMixShop->slotItemID02;
			itemType = m_pMixShop->slotItemType02;
			break;
		case 2:		
			itemSwitch = m_pMixShop->slotSwitch03;
			itemRootID = m_pMixShop->slotItemID03;
			itemType = m_pMixShop->slotItemType03;
			break;
		case 3:		
			itemSwitch = m_pMixShop->slotSwitch04;
			itemRootID = m_pMixShop->slotItemID04;
			itemType = m_pMixShop->slotItemType04;
			break;
		case 4:		
			itemSwitch = m_pMixShop->slotSwitch05;
			itemRootID = m_pMixShop->slotItemID05;
			itemType = m_pMixShop->slotItemType05;
			break;
		case 5:		
			itemSwitch = m_pMixShop->slotSwitch06;
			itemRootID = m_pMixShop->slotItemID06;
			itemType = m_pMixShop->slotItemType06;
			break;
		}

		if (itemSwitch == MIX_SWITCH_OFF)
			return false;
		else if (itemSwitch == MIX_SWITCH_ITEM && itemType == item->m_data->item_type)
		{
			//if (m_mixShopItemSlots[index] == MIX_SHOP_NO_ITEM)
			if (m_mixShopItem[index].itemSlots.size() == 0)
				return true;
		}
	}

	return false;
}

void CMenuMixProcess::SetGemMixShopItem(int srcIconIndex, int itemSlot, int stack)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CMenuMixProcess::SetGemMixShopItem, srcIconIndex = %d, itemSlot = %d, stack = %d."), srcIconIndex, itemSlot, stack);

	if (!IsSuitableGemMixShopItem(srcIconIndex, itemSlot) || stack == 0)
		return;

	UInt index = m_iStartProductIndex + srcIconIndex;
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);

	if (m_pMixShop)
	{
		if (m_pMixShop->type == MIX_GEMMIX)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F(" MIX_GEMMIX "));

			if (item && index < 5 && item->GetStack() >= stack)
			{
				if (item->m_data->item_type == ITEMTYPE_GEM)
				{
					gGlobal.g_TempResult = 1;
					CallLuaFunc(gGlobal.g_pParentLua, "CheckGemMixItemId", String(), m_GemMixFirstItemId, item->GetItem_id(), item->GetLock());
					UInt Result = (UInt)gGlobal.g_TempResult;

					if (Result == 0)
					{
						if (m_GemMixFirstItemId == 0)
							m_GemMixFirstItemId = item->GetItem_id();

						if (item->IsBound())
							m_bNeedSecondCheck = true;

						UInt realStack = stack;

						m_mixShopItem[index].itemSlots.push_back(itemSlot);
						m_mixShopItem[index].item_uids.push_back(item->GetItem_uid());
						m_mixShopItem[index].nowStack += realStack;
						m_mixShopItem[index].nowStacks.push_back(realStack);

						item->SetGivenStack(realStack);
						gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot % BAG_ITEM_COUNT, true);

						UInt32 item_id = 0;
						bool needButtonNormal = true;
						for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
						{
							UInt slotSwitch = 0;
							switch (i)
							{
							case 0:	slotSwitch = m_pMixShop->slotSwitch01;	break;
							case 1:	slotSwitch = m_pMixShop->slotSwitch02;	break;
							case 2:	slotSwitch = m_pMixShop->slotSwitch03;	break;
							case 3:	slotSwitch = m_pMixShop->slotSwitch04;	break;
							case 4:	slotSwitch = m_pMixShop->slotSwitch05;	break;
							}

							if (slotSwitch == MIX_SWITCH_ITEM && m_mixShopItem[i].itemSlots.size() == 0)
							{
								needButtonNormal = false;
								break;
							}
						}

						if (m_GemMixFirstItemId != 0)
						{
							gGlobal.g_TempResult = 0;
							CallLuaFunc(gGlobal.g_pParentLua, "GetGemMixConsumeMoney", String(), m_GemMixFirstItemId);
							UInt16 ComsumeValue = (UInt)gGlobal.g_TempResult;
							if (ComsumeValue > 0)
							{
								CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
								if (pChar)
								{
									if(pChar->GetMoney() < ComsumeValue)
										needButtonNormal = false;
								}
							}
						}

						UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopGemMix");
						if (button)
						{
							if (needButtonNormal)
							{
								button->SetEnable(true);
								button->SetNormalState();
							}
						}

						UpdateGem();
					}
					else
					{
						switch(Result)
						{
						case 1:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMMIX_FAIL_GEMLOCK")));	break;
						case 2:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMMIX_FAIL_MAXGEMLEVEL")));	break;
						case 3:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMMIX_FAIL_DIFFLEVEL")));	break;
						default:gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMMIX_FAIL_DISABLE")));	break;
						}
					}
				}
			}
		}
		else if (m_pMixShop->type == MIX_GEMINLAY)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F(" MIX_GEMINLAY "));

			if (item && index < 3 && item->GetStack() >= stack)
			{
				bool bCanInlay = false;

				if (index == 0 
					&& item->m_data->item_type == ITEMTYPE_EQUIP
					&& item->m_data->eqType != EQTYPE_CLOTH 
					&& item->m_data->eqType != EQTYPE_HAT_WITH_HAIR 
					&& item->m_data->eqType != EQTYPE_HAT_WITHOUT_HAIR)
				{
					UInt InlayGemCount = 0;
					for (Index i = 0; i < 10; i++)
					{
						if (item->GetInlay_gem(i) > 0)
							InlayGemCount++;
					}

					gGlobal.g_TempResult = 1;
					CallLuaFunc(gGlobal.g_pParentLua, "CheckGemInlayEqItem", String(), item->GetItem_id(), InlayGemCount, item->GetLock(), (item->m_data->Identify == 0 || item->GetItem_IdentifyState()));
					UInt Result = (UInt)gGlobal.g_TempResult;

					TRACE_INFODTL_5(GLOBAL_LOGGER, _F(" CheckGemInlayEqItem, eq_gem = %d, count = %d, lock = %d, identify = %d, Result = %d."), 
						item->GetEq_gem(), InlayGemCount, item->GetLock(), (item->m_data->Identify == 0 || item->GetItem_IdentifyState()), Result);

					if (Result == 0)
						bCanInlay = true;
					else
					{
						switch(Result)
						{
						case 1:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_EQITEMLOCK")));	break;
						case 2:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_NOTIDENTIFY")));	break;
						case 3:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_MAXINLAYCOUNT")));	break;
						default:gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_DISABLE")));	break;
						}
					}
				}
				else if (index == 1 && item->m_data->item_type == ITEMTYPE_GEM)
				{
					// 1 : eq
					UInt32 EqItemId = 0;
					UInt EqType = 0;
					UInt32 InlayGemItemId = 0;
					UInt EqReqLv = 0;

					if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
					{
						CItem* pEqItem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
						if (pEqItem)
						{
							EqItemId = pEqItem->GetItem_id();
							EqType = pEqItem->m_data->eqType;
							EqReqLv = pEqItem->m_data->reqLv;

							UInt InlayGemCount = 0;
							for (Index i = 0; i < 10; i++)
							{
								if (pEqItem->GetInlay_gem(i) > 0)
								{
									InlayGemItemId = pEqItem->GetInlay_gem(i);
									EqReqLv = pEqItem->m_data->reqLv;
									InlayGemCount++;
								}
							}

							TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" gem count = %d."), InlayGemCount);
							if (InlayGemCount >= 2)
							{
								return ;
							}

							if (!pEqItem->IsBound() && item->IsBound())
								m_bNeedSecondCheck = true;
						}
					}

					// 2 : gem
					UInt32 GemItemId = item->GetItem_id();

					// 3 : consume
					UInt32 ConsumeItemId = 0;
					if (m_mixShopItem.size() > 2 && m_mixShopItem[2].itemSlots.size() > 0)
					{
						CItem* pConsumeItem = gGlobal.m_pBag->GetItem(m_mixShopItem[2].itemSlots[0]);
						if (pConsumeItem)
							ConsumeItemId = pConsumeItem->GetItem_id();
					}

					gGlobal.g_TempResult = 1;
					CallLuaFunc(gGlobal.g_pParentLua, "CheckGemInlayGemItem", String(), EqItemId, EqType, EqReqLv, InlayGemItemId, GemItemId, item->GetLock());
					UInt Result = (UInt)gGlobal.g_TempResult;

					TRACE_INFODTL_7(GLOBAL_LOGGER, _F(" CheckGemInlayGemItem, EqItemId : %d, EqType : %d, EqReqLv : %d, InlayGemItemId : %d, GemItemId : %d, Lock = %d, Result : %d."), 
						EqItemId, EqType, EqReqLv, InlayGemItemId, GemItemId, item->GetLock(), Result);

					if (Result == 0)
						bCanInlay = true;
					else
					{
						switch(Result)
						{
						case 1:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_REQ_EQITEM")));	break;
						case 2:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_LEVEL60")));	break;
						case 3:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_LEVEL70")));	break;
						case 4:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_SAMEGEM")));	break;
						case 5:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_EQTYPE")));	break;
						case 6:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_GEMLOCK")));	break;
						case 7:
						case 8:
						case 9:
						case 10:
						case 11:
						case 12:gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_GEMTYPE")));	break;
						default:gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_DISABLE")));	break;
						}
					}
				}
				else if (index == 2 && item->m_data->item_type == ITEMTYPE_SPECIAL)
				{
					// 0 : eq
					UInt32 EqItemId = 0;
					UInt EqType = 0;
					UInt32 InlayGemItemId = 0;
					if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
					{
						CItem* pEqItem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
						if (pEqItem)
						{
							EqItemId = pEqItem->GetItem_id();
							EqType = pEqItem->m_data->eqType;

							UInt InlayGemCount = 0;
							for (Index i = 0; i < 10; i++)
							{
								if (pEqItem->GetInlay_gem(i) > 0)
								{
									InlayGemItemId = pEqItem->GetInlay_gem(i);
									InlayGemCount++;
								}
							}

							if (InlayGemCount >= 2)
							{
								TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" gem count = %d."), InlayGemCount);
								return ;
							}
						}
					}

					// 2 : gem
					UInt32 GemItemId = 0;
					if (m_mixShopItem.size() > 1 && m_mixShopItem[1].itemSlots.size() > 0)
					{
						CItem* pGemItem = gGlobal.m_pBag->GetItem(m_mixShopItem[1].itemSlots[0]);
						if (pGemItem)
							GemItemId = pGemItem->GetItem_id();
					}

					// 3 : consume
					UInt32 ConsumeItemId = item->GetItem_id();

					gGlobal.g_TempResult = 1;
					CallLuaFunc(gGlobal.g_pParentLua, "CheckGemInlayConsumeItem", String(), EqItemId, EqType, InlayGemItemId, GemItemId, ConsumeItemId);
					UInt Result = (UInt)gGlobal.g_TempResult;

					TRACE_INFODTL_6(GLOBAL_LOGGER, _F(" CheckGemInlayConsumeItem, EqItemId : %d, EqType : %d, InlayGemItemId : %d, ConsumeItemId : %d, GemItemId : %d, Result : %d."), 
						EqItemId, EqType, InlayGemItemId, ConsumeItemId, GemItemId, Result);

					if (Result == 0)
						bCanInlay = true;
					else
					{
						switch(Result)
						{
						case 1:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_REQ_EQITEM")));	break;
						case 2:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_REQ_GEMITEM")));	break;
						case 3:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_CONSUMEITEMLEVEL")));	break;
						case 4:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_CONSUMEITEMTYPE")));	break;
						default:gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_DISABLE")));	break;
						}
					}
				}

				if (bCanInlay)
				{
					UInt realStack = stack;

					m_mixShopItem[index].itemSlots.push_back(itemSlot);
					m_mixShopItem[index].item_uids.push_back(item->GetItem_uid());
					m_mixShopItem[index].nowStack += realStack;
					m_mixShopItem[index].nowStacks.push_back(realStack);

					item->SetGivenStack(realStack);
					gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot % BAG_ITEM_COUNT, true);

					UInt32 item_id = 0;
					bool needButtonNormal = true;
					for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
					{
						UInt slotSwitch = 0;
						switch (i)
						{
						case 0:	slotSwitch = m_pMixShop->slotSwitch01;	break;
						case 1:	slotSwitch = m_pMixShop->slotSwitch02;	break;
						case 2:	slotSwitch = m_pMixShop->slotSwitch03;	break;
						}

						if (slotSwitch == MIX_SWITCH_ITEM && m_mixShopItem[i].itemSlots.size() == 0)
						{
							needButtonNormal = false;
							break;
						}
					}

					UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopGemEqInlay");
					if (button)
					{
						if (needButtonNormal)
						{
							button->SetEnable(true);
							button->SetNormalState();
						}
					}

					UpdateGem();
				}
			}
		}
		else if (m_pMixShop->type == MIX_GEMREMOVE)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F(" MIX_GEMREMOVE "));

			if (item && index < 2 && item->GetStack() >= stack)
			{
				bool bCanRemove = false;

				if (index == 0 && item->m_data->item_type == ITEMTYPE_EQUIP)
				{
					UInt InlayGemCount = 0;
					for (Index i = 0; i < 10; i++)
					{
						if (item->GetInlay_gem(i) > 0)
							InlayGemCount++;
					}

					gGlobal.g_TempResult = 1;
					CallLuaFunc(gGlobal.g_pParentLua, "CheckGemRemoveEqItem", String(), item->GetItem_id(), InlayGemCount, item->GetLock());
					UInt Result = (UInt)gGlobal.g_TempResult;

					if (Result == 0)
						bCanRemove = true;
					else
					{
						switch(Result)
						{
						case 1:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMREMOVE_FAIL_REQ_EQITEM")));	break;
						case 2:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMREMOVE_FAIL_NOTINLAYGEM")));	break;
						case 3:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_EQITEMLOCK")));	break;
						default:gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_DISABLE")));	break;
						}
					}
				}
				else if (index == 1 && item->m_data->item_type == ITEMTYPE_SPECIAL)
				{
					UInt32 EqItemId = 0;
					if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
					{
						CItem* pEqItem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
						if (pEqItem)
						{
							EqItemId = pEqItem->GetItem_id();
						}
					}

					gGlobal.g_TempResult = 1;
					CallLuaFunc(gGlobal.g_pParentLua, "CheckGemRemoveConsumeItem", String(), EqItemId, m_GemRemoveSelectGemId, item->GetItem_id());
					UInt Result = (UInt)gGlobal.g_TempResult;

					if (Result == 0)
						bCanRemove = true;
					else
					{
						switch(Result)
						{
						case 1:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMREMOVE_FAIL_REQ_EQITEM")));	break;
						case 2:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMREMOVE_FAIL_NOTSELECTGEM")));	break;
						case 3:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMREMOVE_FAIL_REQ_CONSUMEITEM")));	break;
						case 4:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMREMOVE_FAIL_CONSUMEITEMLEVEL")));	break;
						default:gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_FAIL_DISABLE")));	break;
						}
					}
				}

				if (bCanRemove)
				{
					UInt realStack = stack;

					m_mixShopItem[index].itemSlots.push_back(itemSlot);
					m_mixShopItem[index].item_uids.push_back(item->GetItem_uid());
					m_mixShopItem[index].nowStack += realStack;
					m_mixShopItem[index].nowStacks.push_back(realStack);

					item->SetGivenStack(realStack);
					gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot % BAG_ITEM_COUNT, true);

					UInt32 item_id = 0;
					bool needButtonNormal = true;
					for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
					{
						UInt slotSwitch = 0;
						switch (i)
						{
						case 0:	slotSwitch = m_pMixShop->slotSwitch01;	break;
						case 1:	slotSwitch = m_pMixShop->slotSwitch02;	break;
						}

						if (slotSwitch == MIX_SWITCH_ITEM && m_mixShopItem[i].itemSlots.size() == 0)
						{
							needButtonNormal = false;
							break;
						}
					}

					UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopGemEqRemove");
					if (button)
					{
						if (needButtonNormal)
						{
							button->SetEnable(true);
							button->SetNormalState();
						}
					}

					UpdateGem();
				}
			}
		}
		else if (m_pMixShop->type == MIX_GEMMELTING)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F(" MIX_GEMMELTING "));

			if (item && index < 1 && item->GetStack() >= stack)
			{
				bool bCanMelting = false;

				if (index == 0 && item->m_data->item_type == ITEMTYPE_GEM)
				{
					UInt32 GemItemId = item->GetItem_id();

					gGlobal.g_TempResult = 1;
					CallLuaFunc(gGlobal.g_pParentLua, "CheckGemMeltingGemItem", String(), GemItemId, item->GetLock());
					UInt Result = (UInt)gGlobal.g_TempResult;

					TRACE_INFODTL_3(GLOBAL_LOGGER, _F(" CheckGemMeltingGemItem, GemItemId : %d, Lock = %d, Result : %d."), 
						GemItemId, item->GetLock(), Result);

					if (Result == 0)
						bCanMelting = true;
					else
					{
						switch(Result)
						{
						case 1:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMMELTING_FAIL_GEMLOCK")));	break;
						case 2:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMMELTING_FAIL_GEMTYPE")));	break;
						}
					}
				}

				if (bCanMelting)
				{
					m_bNeedSecondCheck = true;

					UInt realStack = stack;

					m_mixShopItem[index].itemSlots.push_back(itemSlot);
					m_mixShopItem[index].item_uids.push_back(item->GetItem_uid());
					m_mixShopItem[index].nowStack += realStack;
					m_mixShopItem[index].nowStacks.push_back(realStack);

					item->SetGivenStack(realStack);
					gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot % BAG_ITEM_COUNT, true);

					UInt32 item_id = 0;
					bool needButtonNormal = true;
					for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
					{
						UInt slotSwitch = 0;
						switch (i)
						{
						case 0:	slotSwitch = m_pMixShop->slotSwitch01;	break;
						}

						if (slotSwitch == MIX_SWITCH_ITEM && m_mixShopItem[i].itemSlots.size() == 0)
						{
							needButtonNormal = false;
							break;
						}
					}

					UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopGemMelting");
					if (button)
					{
						if (needButtonNormal)
						{
							button->SetEnable(true);
							button->SetNormalState();
						}
					}

					UpdateGem();
				}
			}
		}
		else if (m_pMixShop->type == MIX_GEMPOLISHING)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F(" MIX_GEMPOLISHING "));

			if (item && index < 2 && item->GetStack() >= stack)
			{
				bool bCanPolishing = false;

				if (index == 0 && item->m_data->item_type == ITEMTYPE_GEM)
				{
					UInt32 GemItemId = item->GetItem_id();

					gGlobal.g_TempResult = 1;
					CallLuaFunc(gGlobal.g_pParentLua, "CheckGemPolishingGemItem", String(), GemItemId, item->GetLock());
					UInt Result = (UInt)gGlobal.g_TempResult;

					TRACE_INFODTL_3(GLOBAL_LOGGER, _F(" CheckGemPolishingGemItem, GemItemId : %d, Lock = %d, Result : %d."), 
						GemItemId, item->GetLock(), Result);

					if (Result == 0)
						bCanPolishing = true;
					else
					{
						switch(Result)
						{
						case 1:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMPOLISHING_FAIL_GEMLOCK")));	break;
						case 2:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMPOLISHING_FAIL_GEMTYPE")));	break;
						}
					}
				}
				else if (index == 1 && item->m_data->item_type == ITEMTYPE_SPECIAL)
				{
					UInt32 gemItemId = 0;
					if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
					{
						CItem* pGemItem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
						if (pGemItem)
						{
							gemItemId = pGemItem->GetItem_id();

							if (pGemItem->IsBound() || item->IsBound())
								m_bNeedSecondCheck = true;
						}
					}

					gGlobal.g_TempResult = 1;
					CallLuaFunc(gGlobal.g_pParentLua, "CheckGemPolishingConsumeItem", String(), gemItemId, item->GetItem_id());
					UInt Result = (UInt)gGlobal.g_TempResult;

					if (Result == 0)
						bCanPolishing = true;
					else
					{
						switch(Result)
						{
						case 1:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMPOLISHING_FAIL_GEMFIRST")));	break;
						case 2:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMPOLISHING_FAIL_CONSUMETYPE")));	break;
						case 3:	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMPOLISHING_FAIL_CONSUMELEVEL")));break;
						}
					}
				}

				if (bCanPolishing)
				{
					UInt realStack = stack;

					m_mixShopItem[index].itemSlots.push_back(itemSlot);
					m_mixShopItem[index].item_uids.push_back(item->GetItem_uid());
					m_mixShopItem[index].nowStack += realStack;
					m_mixShopItem[index].nowStacks.push_back(realStack);

					item->SetGivenStack(realStack);
					gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot % BAG_ITEM_COUNT, true);

					UInt32 item_id = 0;
					bool needButtonNormal = true;
					for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
					{
						UInt slotSwitch = 0;
						switch (i)
						{
						case 0:	slotSwitch = m_pMixShop->slotSwitch01;	break;
						case 1:	slotSwitch = m_pMixShop->slotSwitch02;	break;
						}

						if (slotSwitch == MIX_SWITCH_ITEM && m_mixShopItem[i].itemSlots.size() == 0)
						{
							needButtonNormal = false;
							break;
						}
					}

					UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopGemPolishing");
					if (button)
					{
						if (needButtonNormal)
						{
							button->SetEnable(true);
							button->SetNormalState();
						}
					}

					UpdateGem();
				}
			}
		}
		else if (m_pMixShop->type == MIX_COLORSTONEMIX)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F(" MIX_COLORSTONEMIX "));

			if (item && index < 2 && item->GetStack() >= stack)
			{
				bool bCanMix = false;

				if (!item->GetLock())
				{
					if (index == 1)
					{
						if (m_mixShopItem[0].itemSlots.size() > 0)
						{
							UInt16 reqLv = 0;

							if (m_mixShopItem[0].itemSlots.size() > 0)
							{
								CItem* tmpItem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
								if (tmpItem)
									reqLv = tmpItem->m_data->reqLv;
							}
							if (m_mixShopItem[1].itemSlots.size() > 0)
							{
								CItem* tmpItem = gGlobal.m_pBag->GetItem(m_mixShopItem[1].itemSlots[0]);
								if (tmpItem)
									reqLv = tmpItem->m_data->reqLv;
							}

							if (reqLv == item->m_data->reqLv)
							{
								bCanMix = true;
							}
							else
							{
								gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_COLORSTONEMIX_FAIL_REQLV")));
							}
						}
						else
						{
							gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_COLORSTONEMIX_FAIL_STONEFIRST")));
						}
					}
					else
						bCanMix = true; // index == 0, do not check reqLv.
				}
				else
					bCanMix = false; // item locked.

				if (bCanMix)
				{
					UInt realStack = stack;

					m_mixShopItem[index].itemSlots.push_back(itemSlot);
					m_mixShopItem[index].item_uids.push_back(item->GetItem_uid());
					m_mixShopItem[index].nowStack += realStack;
					m_mixShopItem[index].nowStacks.push_back(realStack);

					item->SetGivenStack(realStack);
					gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot % BAG_ITEM_COUNT, true);

					UInt32 item_id = 0;
					bool needButtonNormal = true;
					for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
					{
						UInt slotSwitch = 0;
						switch (i)
						{
						case 0:	slotSwitch = m_pMixShop->slotSwitch01;	break;
						case 1:	slotSwitch = m_pMixShop->slotSwitch02;	break;
						}

						if (slotSwitch == MIX_SWITCH_ITEM && m_mixShopItem[i].itemSlots.size() == 0)
						{
							needButtonNormal = false;
							break;
						}
					}

					UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopColorStoneMix");
					if (button)
					{
						if (needButtonNormal)
						{
							button->SetEnable(true);
							button->SetNormalState();
						}
					}

					UpdateGem();
				}
			}
		}
		else
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F(" Unknown shop type."));
		}
	}
}
void CMenuMixProcess::SetSourceItem(int srcIconIndex, int itemSlot)
{
	if (m_pMixShop)
	{
		switch (m_pMixShop->type)
		{
		case MIX_WEAPONMAKING:	case MIX_CLOTHMAKING:	case MIX_ORNAMENTMAKING:
			SetMakeEqSourceItem(srcIconIndex, itemSlot);
			break;
		case MIX_COOKING:	case MIX_HERMETIC:	case MIX_MAKEWINE:
			SetLiveSkillSourceItem(srcIconIndex, itemSlot);
			break;
		}
	}
}

void CMenuMixProcess::SetLiveSkillSourceItem(int srcIconIndex, int itemSlot)
{
	UInt index = m_iStartSourceIndex * 2 + srcIconIndex;
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);
	if (item && m_pSelectedItemMix && item->m_data)
	{
		if (item->GetItem_id() != m_sourceList[index].sourceRootID)
			return;

		if (m_sourceList[index].itemSlots.size() > 0)
		{
			CItem* tempItem = gGlobal.m_pBag->GetItem(m_sourceList[index].itemSlots[0]);
			if (tempItem && tempItem->GetItem_id() != item->GetItem_id() )
				return;
		}

		m_sourceList[index].nowStack += item->GetStack();
		m_sourceList[index].nowStacks.push_back(item->GetStack());
		m_sourceList[index].itemSlots.push_back(itemSlot);
		m_sourceList[index].item_uids.push_back(item->GetItem_uid());

		item->SetGivenStack(item->GetStack());
		gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot % BAG_ITEM_COUNT, true);

		bool needButtonNormal = true;
		for (Index i = C_INDEX(0); i < m_sourceList.size(); ++i)
		{
			if (m_sourceList[i].nowStack <  m_sourceList[i].maxStack)
			{
				needButtonNormal = false;
				break;
			}
		}

		if (needButtonNormal )
		{
			UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonMake2");
			UIButton* button2 = gGlobal.pUIManager->FindUIButton("MixShopButtonAllMake");
			if (button && button2)
			{
				button->SetNormalState();
				button2->SetNormalState();
				gGlobal.pUIManager->AddDirtyUIItem(button);
				gGlobal.pUIManager->AddDirtyUIItem(button2);
			}
		}
		UpdateItemMixSource();
	}
}

void CMenuMixProcess::SetMakeEqSourceItem(int srcIconIndex, int itemSlot)
{
	UInt index = m_iStartSourceIndex * 2 + srcIconIndex;
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);
	if (item && m_pSelectedItemMix && item->m_data)
	{
		if (item->m_data->root_id != m_sourceList[index].sourceRootID && m_sourceList[index].itemSwitch == MIX_SWITCH_STONE)
			return;

		if (item->GetItem_id() != m_sourceList[index].sourceRootID && 
			(m_sourceList[index].itemSwitch == MIX_SWITCH_ITEM || m_sourceList[index].itemSwitch == MIX_SWITCH_LIGHT_STONE) )
			return;

		if (m_bHasLightStone && m_sourceList[index].itemSwitch == MIX_SWITCH_ITEM && item->GetItem_IdentifyState())
			return;

		//for same item but in different slots, makw sure that they must be a same item
		if (m_sourceList[index].itemSlots.size() > 0)
		{
			CItem* tempItem = gGlobal.m_pBag->GetItem(m_sourceList[index].itemSlots[0]);
			if (tempItem && tempItem->GetItem_id() != item->GetItem_id() )
				return;
		}

		if (m_pMixShop->shopID >= 26 && m_pMixShop->shopID <= 34)
		{
			if (index == 1)
			{
				if ( m_sourceList[0].itemSlots.size() > 0 )
				{
					CItem* tempItem = gGlobal.m_pBag->GetItem(m_sourceList[0].itemSlots[0]);
					if (tempItem && tempItem->m_data->reqLv != item->m_data->reqLv )
					{
						gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_COLORSTONE_MAKEEQ_FAIL_REQLV")));
						return;
					}
				}
				else
				{
					gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_COLORSTONE_MAKEEQ_FAIL_EQUIPFIRST")));
					return ;
				}
			}
		}

		UInt8 remain = m_sourceList[index].maxStack - m_sourceList[index].nowStack;
		if (remain < item->GetStack())
			m_sourceList[index].nowStack =  m_sourceList[index].maxStack;
		else
		{
			remain = item->GetStack();
			m_sourceList[index].nowStack += item->GetStack();
		}

		m_sourceList[index].nowStacks.push_back(remain);
		m_sourceList[index].itemSlots.push_back(itemSlot);

		item->SetGivenStack(remain);

		gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot % BAG_ITEM_COUNT, true);

		if (m_sourceList[index].itemSwitch == MIX_SWITCH_STONE)
		{
			UInt8Vector probList;
			gGlobal.m_resource->GetStoneProbList(item->GetItem_id(), probList);
			UInt8Vector existItem;
			existItem.clear();
			for (Index i = 0; i < m_pSelectedItemMix->output.size(); ++i)
			{
				if (m_pSelectedItemMix->output[i] > 0)
					existItem.push_back(i + 1);
			}
			if (probList.size() > 0 && existItem.size() > 0)
			{		
				UInt8 prob = 0;
				UInt8 existItemIndex = (UInt8)existItem.size() - 1;
				for (Index i = (Index)probList.size() - 1; i > 0; --i)
				{
					prob += probList[i];
					if (existItemIndex >= 0 && existItem[existItemIndex] == i)
					{
						UpdateProbText(true, i, prob);
						prob = 0;
						--existItemIndex;
					}
				}
			}
		}
	
		bool needButtonNormal = true;
		for (UInt8 i = 0; i < m_sourceList.size(); ++i)
		{
			if (m_sourceList[i].nowStack !=  m_sourceList[i].maxStack)
			{
				needButtonNormal = false;
				break;
			}
		}

		if (needButtonNormal )
		{
			UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonMake");
			if (button)
			{
				button->SetNormalState();
				gGlobal.pUIManager->AddDirtyUIItem(button);
			}
		}
		UpdateItemMixSource();
	}
}

void CMenuMixProcess::SetIronShopItem(int srcIconIndex, int itemSlot, int stack)
{
	if (stack == 0)
		return;

	UInt index = m_iStartProductIndex + srcIconIndex;
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);

	if (m_pMixShop)
	{
		if (item && index < 2 && item->GetStack() >= stack)
		{
			bool bCanMix = false;
			bool needButtonNormal = false;
			if (m_pMixShop->type == MIX_IRONMAKE)
			{
				if (index == 1)
				{
					if (m_mixShopItem[0].itemSlots.size() > 0)
					{
						CItem* tmpItem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
						for (ItemMixTypeList::iterator itr = m_typeItemList.begin(); itr != m_typeItemList.end(); ++itr)
						{
							ItemMixType* itemList = itr->second;
							for (UInt16 i = 0; i < itemList->dataList.size(); ++i)
							{
								if ((tmpItem->GetItem_id() == itemList->dataList[i]->materials[0]) &&
									(item->GetItem_id() == itemList->dataList[i]->materials[1]))
								{
									m_pSelectedItemMix = itemList->dataList[i];
									bCanMix = true;
									needButtonNormal = true;
									break;
								}						
							}
						}
						if(bCanMix == false)
							gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_SCROLL_LEVELERROR")));
					}
					else
						gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_IRONMAKE_FAIL_IRONFIRST")));
				}
				else
				{
					UInt32 id = item->GetItem_id();
					if (id == 54005 || id == 54060 || id == 54115 || id == 54170 || id == 54225 || id == 54280)
						gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_MAXIRONLEVEL")));
					else
						bCanMix = true;
				}
			}
			else if (m_pMixShop->type == MIX_IRONREPAIR)
			{
				if (index == 1)
				{
					if (m_mixShopItem[0].itemSlots.size() > 0)
					{
						CItem* tmpItem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
						if (tmpItem->m_data->reqLv == 45)
						{
							if ((item->GetItem_id() >= 54300 && item->GetItem_id() <= 54305) ||
								(item->GetItem_id() >= 54320 && item->GetItem_id() <= 54325))
							{
								bCanMix = true;
								needButtonNormal = true;
							}
							else
								gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_SCROLL_LOWLEVEL")));
						}
						else if (tmpItem->m_data->reqLv == 55)
						{
							if ((item->GetItem_id() >= 54301 && item->GetItem_id() <= 54305) ||
								(item->GetItem_id() >= 54321 && item->GetItem_id() <= 54325))
							{
								bCanMix = true;
								needButtonNormal = true;
							}
							else
								gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_SCROLL_LOWLEVEL")));
						}
						else if (tmpItem->m_data->reqLv == 65)
						{
							if ((item->GetItem_id() >= 54302 && item->GetItem_id() <= 54305) ||
								(item->GetItem_id() >= 54322 && item->GetItem_id() <= 54325))
							{
								bCanMix = true;
								needButtonNormal = true;
							}
							else
								gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_SCROLL_LOWLEVEL")));
						}
						else if (tmpItem->m_data->reqLv == 75)
						{
							if ((item->GetItem_id() >= 54303 && item->GetItem_id() <= 54305) ||
								(item->GetItem_id() >= 54323 && item->GetItem_id() <= 54325))
							{
								bCanMix = true;
								needButtonNormal = true;
							}
							else
								gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_SCROLL_LOWLEVEL")));
						}
						else if (tmpItem->m_data->reqLv == 85)
						{
							if ((item->GetItem_id() >= 54304 && item->GetItem_id() <= 54305) ||
								(item->GetItem_id() >= 54324 && item->GetItem_id() <= 54325))
							{
								bCanMix = true;
								needButtonNormal = true;
							}
							else
								gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_SCROLL_LOWLEVEL")));
						}
						else if (tmpItem->m_data->reqLv == 95)
						{
							if (item->GetItem_id() == 54305 || item->GetItem_id() == 54325)
							{
								bCanMix = true;
								needButtonNormal = true;
							}
							else
								gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_SCROLL_LOWLEVEL")));
						}
					}
					else
						gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_IRONREPAIR_FAIL_IRONFIRST")));
				}
				else
				{
					if (item->GetEndure() == item->GetMEndure())
						gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_IRONREPAIR_NONEED")));
					else
						bCanMix = true;
				}
			}

			if(bCanMix)
			{
				UInt realStack = stack;

				m_mixShopItem[index].itemSlots.push_back(itemSlot);
				m_mixShopItem[index].item_uids.push_back(item->GetItem_uid());
				m_mixShopItem[index].nowStack += realStack;
				m_mixShopItem[index].nowStacks.push_back(realStack);

				item->SetGivenStack(realStack);
				gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot % BAG_ITEM_COUNT, true);

				UIButton* button;
				if (m_pMixShop->type == MIX_IRONREPAIR)
					button = gGlobal.pUIManager->FindUIButton("IronShopButtonRepair");
				else if(m_pMixShop->type == MIX_IRONMAKE)
					button = gGlobal.pUIManager->FindUIButton("IronShopButtonMake");
				if (button)
				{
					if (needButtonNormal)
					{
						button->SetEnable(true);
						button->SetNormalState();
					}
				}
				UpdateIronMakeOrRepair();
			}
		}
	}
}

void CMenuMixProcess::SetMixShopItem(int srcIconIndex, int itemSlot, int stack)
{
	if (!IsSuitableMixShopItem(srcIconIndex, itemSlot) || stack == 0)
		return;

	UInt index = m_iStartProductIndex + srcIconIndex;
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);

	if (item && index <= 5 && m_pMixShop && item->GetStack() >= stack)
	{
		UInt realStack = stack;
		if (m_pMixShop->type == MIX_STONEMIX && item->m_data->item_type == ITEMTYPE_STONE)
		{
			UInt maxStack = gGlobal.m_resource->GetMaxStoneMixNum(item->GetItem_id());
			if (maxStack > 0 && (UInt)(m_mixShopItem[index].nowStack + stack) > maxStack)
				realStack = maxStack - m_mixShopItem[index].nowStack;
		}

		if (m_pMixShop->type == MIX_UNSOULBOUND && item->m_data->item_type == ITEMTYPE_EQUIP)
		{
			if (SOULBOUND_ALREADY_BOUND != item->GetItem_soulboundState())
			{
				ResetItemMix();
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_SOULBOUND_ALREADY_BOUND")));
				return;
			}
		}
		if (m_pMixShop->type == MIX_UNSOULBOUND && item->m_data->item_type == ITEMTYPE_UNSOULBOUND)
		{
			if (!m_pEqItem || !m_pEqItem->m_data)
			{
				ResetItemMix();
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_SOULBOUND_FIRST_BOUND")));
				return;
			}
			CallLuaFunc(gGlobal.g_pParentLua, "GetUnSoulBoundStoneID", String(), m_pEqItem->m_data->rare);
			UInt32 ItemId = (UInt32)gGlobal.g_TempResult;
			if (ItemId != item->m_data->item_id)
			{
				const ItemData* pItemData = gGlobal.m_resource->GetItemData(ItemId);
				String temp;
				temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_SOULBOUND_STONE")),pItemData->name);
				gGlobal.PrintPlayerMessage(temp.c_str());
				return;
			}
			UInt maxStack = GetUnSoulBoundStoneNum() - m_mixShopItem[index].nowStack;
			realStack = MIN(item->GetStack(),maxStack);
		}

		m_mixShopItem[index].itemSlots.push_back(itemSlot);
		m_mixShopItem[index].item_uids.push_back(item->GetItem_uid());
		m_mixShopItem[index].nowStack += realStack;
		m_mixShopItem[index].nowStacks.push_back(realStack);
		
		item->SetGivenStack(realStack);
		gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot % BAG_ITEM_COUNT, true);

		bool needButtonNormal = true;
		for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
		{
			UInt slotSwitch = 0;
			switch (i)
			{
			case 0:	slotSwitch = m_pMixShop->slotSwitch01;	break;
			case 1:	slotSwitch = m_pMixShop->slotSwitch02;	break;
			case 2:	slotSwitch = m_pMixShop->slotSwitch03;	break;
			case 3:	slotSwitch = m_pMixShop->slotSwitch04;	break;
			case 4:	slotSwitch = m_pMixShop->slotSwitch05;	break;
			case 5:	slotSwitch = m_pMixShop->slotSwitch06;	break;
			}

			if (slotSwitch == MIX_SWITCH_ITEM && m_mixShopItem[i].itemSlots.size() == 0)
			{
				needButtonNormal = false;
				break;
			}
		}
		
		if (m_pMixShop->type == MIX_STONEMIX && item->m_data->item_type == ITEMTYPE_STONE)
		{
			UInt minStack = gGlobal.m_resource->GetMinStoneMixNum(item->GetItem_id());
			if (m_mixShopItem[index].nowStack < minStack)
				needButtonNormal = false;

			UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel("MixShopSuggestion");
			if (pChatPanel)
			{
				String probText;
				UInt prob = gGlobal.m_resource->GetStoneMixProb(item->GetItem_id(), m_mixShopItem[index].nowStack);
				const ItemData* pItemData = gGlobal.m_resource->GetItemData(gGlobal.m_resource->GetResultStoneID(item->GetItem_id()));
				if (pItemData)
				{
					probText.Format(gGlobal.GetStringTable()->Get(_T("MSG_MIX_STONE_SHOW_PROB")), m_mixShopItem[index].nowStack, 
						String(item->m_data->name).c_str(), String(pItemData->name).c_str(), prob);
					pChatPanel->ClearAllRow();
					pChatPanel->AddStringW(probText.c_str());
					pChatPanel->EndRow();
					pChatPanel->Redraw();
					pChatPanel->SetEnable(true);
				}
			}
		}

		UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonRepair");
		UIButton* button1 = gGlobal.pUIManager->FindUIButton("MixShopButtonRebuild");
		UIButton* button2 = gGlobal.pUIManager->FindUIButton("MixShopButtonStoneMix");
		UIButton* button3 = gGlobal.pUIManager->FindUIButton("MixShopButtonUnSoulBound");
		if (button && button1 && button2 && button3)
		{
			switch(m_pMixShop->type)
			{
			case MIX_UNSOULBOUND:
				{
					needButtonNormal = false;
					button->SetEnable(false);
					if (item->m_data->item_type == ITEMTYPE_UNSOULBOUND && m_pEqItem)
					{
						UInt minStack = GetUnSoulBoundStoneNum();
						if (m_mixShopItem[index].nowStack == minStack)
							button3->SetNormalState();
						else
							button3->SetDisableState();
						gGlobal.pUIManager->AddDirtyUIItem(button3);
					}
					else if (item->m_data->item_type == ITEMTYPE_EQUIP)
					{
						m_pEqItem = item;

						if (GetUnSoulBoundStoneNum() < 1 || 
							(m_pEqItem->GetEq_gem() > 0 && 
							(m_pEqItem->GetInlay_gem(0) > 0 ||
							m_pEqItem->GetInlay_gem(1) > 0 ||
							m_pEqItem->GetInlay_gem(2) > 0 ||
							m_pEqItem->GetInlay_gem(3) > 0 ||
							m_pEqItem->GetInlay_gem(4) > 0 ||
							m_pEqItem->GetInlay_gem(5) > 0 ||
							m_pEqItem->GetInlay_gem(6) > 0 ||
							m_pEqItem->GetInlay_gem(7) > 0 ||
							m_pEqItem->GetInlay_gem(8) > 0 ||
							m_pEqItem->GetInlay_gem(9) > 0
							) ) )
						{
							m_pEqItem->SetGivenStack(0);
							gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot % BAG_ITEM_COUNT, true);
							ResetAllMixItem();
							m_pEqItem = NULL;
							gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_SOULBOUND_CANNOT")));
							return;
						}

						UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel("MixShopSuggestion");
						if (pChatPanel)
						{
							String temp;
							int count = GetUnSoulBoundStoneNum();
							if (m_pEqItem && m_pEqItem->m_data)
							{
								CallLuaFunc(gGlobal.g_pParentLua, "GetUnSoulBoundStoneID", String(), m_pEqItem->m_data->rare);
								UInt32 ItemId = (UInt32)gGlobal.g_TempResult;
								const ItemData* pItemData = gGlobal.m_resource->GetItemData(ItemId);
								if (pItemData)
								{
									temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_SOULBOUND_SUGGESTION")),m_pEqItem->m_data->name,count,pItemData->name);
									pChatPanel->ClearAllRow();
									pChatPanel->AddStringW(temp.c_str());
									pChatPanel->EndRow();
									pChatPanel->Redraw();
									pChatPanel->SetEnable(true);
								}

							}
						}
					}
				}
				break;
			case MIX_EQUIPREPAIR:
				{
					if (item->m_data->item_type == ITEMTYPE_REBUILD)
					{
						button1->SetEnable(true);
						button->SetEnable(false);
					}
					else if (item->m_data->item_type == ITEMTYPE_EQUIP)
						m_pEqItem = item;
				}
				break;
			case MIX_STONEMIX:
				{
					button->SetEnable(false);
					button1->SetEnable(false);
					button2->SetEnable(true);
				}
				break;
			}
			
			ShowMixConsume(true);
			
			if (needButtonNormal )
			{					
				button->SetNormalState();
				gGlobal.pUIManager->AddDirtyUIItem(button);

				if (button1->IsEnable())
				{
					button1->SetNormalState();
					gGlobal.pUIManager->AddDirtyUIItem(button1);
				}

				button2->SetNormalState();
				gGlobal.pUIManager->AddDirtyUIItem(button2);
			}	
		}
		UpdateMixOrRepairItem();
	}
}

void CMenuMixProcess::ShowMixShopResult(Boolean success, UInt8 msgCode, UInt32Vector item_uids, UInt8Vector reduceStacks)
{
	if (m_pMixShop)
	{
		UIImage* img = gGlobal.pUIManager->FindUIImage("MixShopBlockImage");
		UIImage* img1 = gGlobal.pUIManager->FindUIImage("IronShopBlockImage");
		if (img)
			img->SetEnable(false);
		if (img1)
			img1->SetEnable(false);

		if (success)
		{
			switch(m_pMixShop->type)
			{
			case MIX_WEAPONMAKING:
			case MIX_CLOTHMAKING:
			case MIX_ORNAMENTMAKING:
				{
					ResetItemMix();
					MakeSourceList();
					UpdateItemMixSource();
					ShowMakeConsume(true);
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_SUCCESS")));
						break;
					}
				}
				break;
			case MIX_UNSOULBOUND:
				{
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						{
							if (m_pEqItem)
							{
								String text =gGlobal.GetStringTable()->Get(_T("MSG_UNSOULBOUND_SUCCESS"));
								text.Format(text.c_str(), String(m_pEqItem->m_data->name).c_str());
								gGlobal.PrintPlayerMessage(text.c_str());
							}
						}
						break;
					}
					ResetAllMixItem();
					UpdateMixOrRepairItem();
					ShowMixConsume(true);
				}
				break;
			case MIX_EQUIPREPAIR:
				{
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						{
							if (m_pEqItem)
							{
								String text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_REPAIR_SUCCESS"));
								text.Format(text.c_str(), String(m_pEqItem->m_data->name).c_str());
								gGlobal.PrintPlayerMessage(text.c_str());
							}
						}
						break;
					case MIXSHOP_SUCCESS_REBUILD:
						gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_REBUILD_SUCCESS")));
						break;
					case MIXSHOP_SUCCESS_REDUCE_MENDURING:
						{
							if (m_pEqItem)
							{
								String text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_REPAIR_SUCCESS_REDUCE_ENDURE"));
								text.Format(text.c_str(), String(m_pEqItem->m_data->name).c_str());
								gGlobal.PrintPlayerMessage(text.c_str());
							}
						}
						break;
					}
					ResetAllMixItem();
					UpdateMixOrRepairItem();
					ShowMixConsume(true);
				}
				break;
			case MIX_STONEMIX:
				{
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						{
							if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
							{
								CItem* stone = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
								if (stone && stone->m_data->item_type == ITEMTYPE_STONE)
								{
									const ItemData* pItemData = gGlobal.m_resource->GetItemData(gGlobal.m_resource->GetResultStoneID(stone->GetItem_id()));
									if (pItemData)
									{
										String text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_STONE_SUCCESS"));
										text.Format(text.c_str(), String(pItemData->name).c_str());
										gGlobal.PrintPlayerMessage(text.c_str());
									}
								}
							}
						}
						break;
					}
					ResetAllMixItem();
					UpdateMixOrRepairItem();
					ShowMixConsume(true);
				}
				break;
			case MIX_COOKING:
			case MIX_HERMETIC:
			case MIX_MAKEWINE:
				{
					String msg;
					if (m_pSelectedItemMix)
						msg.Format(_T("%s%s"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_LIVESKILL_SUCCESS")), m_pSelectedItemMix->productName);
					
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						gGlobal.PrintPlayerMessage(msg);	
						break;
					default:
						if (m_bMakeAll || m_iRemainItemCount > 1)
						{
							msg.Format(_T("%s %s"), msg.c_str(),gGlobal.GetStringTable()->Get(_T("MSG_MIX_LIVESKILL_CANT_REMAKE")));
							SetMakeAll(false);
							m_iRemainItemCount = 0;
						}
						gGlobal.PrintPlayerMessage(msg);
						break;
					}
					ReduceLiveSkillItemMix(item_uids, reduceStacks);
					UpdateItemMixSource();
					ShowMakeConsume(true);
					bool canStart = true;
					for(Index i = C_INDEX(0); i < m_sourceList.size(); ++i)
					{
						if (m_sourceList[i].nowStack < m_sourceList[i].maxStack && m_sourceList[i].maxStack != 0)
						{
							canStart = false;
							SetMakeAll(false);
							break;
						}
					}

					if (canStart)
					{
						if (m_bMakeAll)
							StartServerTime();
						else if (m_iRemainItemCount > 1)
						{
							--m_iRemainItemCount;
							StartServerTime();
						}
						else
						{					
							UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonMake2");
							UIButton* button2 = gGlobal.pUIManager->FindUIButton("MixShopButtonAllMake");
							if (button && button2)
							{
								button->SetNormalState();
								button2->SetNormalState();
								gGlobal.pUIManager->AddDirtyUIItem(button);
								gGlobal.pUIManager->AddDirtyUIItem(button2);
							}
						}
					}
				}
				break;
			case MIX_GEMMIX:
				{
					TRACE_INFODTL(GLOBAL_LOGGER, _F(" mix gem success."));

					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						{
							if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
							{
								CItem* gem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
								if (gem && gem->m_data->item_type == ITEMTYPE_GEM)
								{
									const ItemData* pItemData = gGlobal.m_resource->GetItemData(gGlobal.m_resource->GetResultStoneID(gem->GetItem_id()));
									if (pItemData)
									{
										String text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_GEM_SUCCESS"));
										text.Format(text.c_str(), String(pItemData->name).c_str());
										gGlobal.PrintPlayerMessage(text.c_str());
									}
								}
							}
						}
						break;
					}
					m_GemMixFirstItemId = 0;
					m_bNeedSecondCheck = false;
					ResetAllMixItem();
					UpdateGem();
					ShowMixConsume(true);
				}
				break;
			case MIX_GEMINLAY:
				{
					TRACE_INFODTL(GLOBAL_LOGGER, _F(" gem inlay success."));

					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						{
							String text =gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMINLAY_SUCCESS"));
							gGlobal.PrintPlayerMessage(text.c_str());
						}
						break;
					}
					ResetAllMixItem();
					UpdateGem();
					ShowMixConsume(true);
				}
				break;
			case MIX_GEMREMOVE:
				{
					TRACE_INFODTL(GLOBAL_LOGGER, _F(" gem remove success."));

					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						{
							String text =gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMREMOVE_SUCCESS"));
							gGlobal.PrintPlayerMessage(text.c_str());
						}
						break;
					}
					m_iGemRemoveSelectIndex = -1;
					m_GemRemoveSelectGemId = 0;
					ResetAllMixItem();
					UpdateGem();
					ShowMixConsume(true);
				}
				break;
			case MIX_GEMMELTING:
				{
					TRACE_INFODTL(GLOBAL_LOGGER, _F(" gem melting success."));

					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						{
							String text =gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMMELTING_SUCCESS"));
							gGlobal.PrintPlayerMessage(text.c_str());
						}
						break;
					}
					m_iGemRemoveSelectIndex = -1;
					m_GemRemoveSelectGemId = 0;
					ResetAllMixItem();
					UpdateGem();
					ShowMixConsume(true);
				}
				break;
			case MIX_GEMPOLISHING:
				{
					TRACE_INFODTL(GLOBAL_LOGGER, _F(" gem polishing success."));

					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						{
							String text =gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMPOLISHING_SUCCESS"));
							gGlobal.PrintPlayerMessage(text.c_str());
						}
						break;
					}
					m_iGemRemoveSelectIndex = -1;
					m_GemRemoveSelectGemId = 0;
					ResetAllMixItem();
					UpdateGem();
					ShowMixConsume(true);
				}
				break;
			case MIX_COLORSTONEMIX:
				{
					TRACE_INFODTL(GLOBAL_LOGGER, _F(" color stone mix success."));

					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						{
							String text =gGlobal.GetStringTable()->Get(_T("MIXSHOP_COLORSTONEMIX_SUCCESS"));
							gGlobal.PrintPlayerMessage(text.c_str());
						}
						break;
					}
					m_iGemRemoveSelectIndex = -1;
					m_GemRemoveSelectGemId = 0;
					ResetAllMixItem();
					UpdateGem();
					ShowMixConsume(true);
				}
				break;
			case MIX_IRONMAKE:
				{
					String msg;
					if (m_pSelectedItemMix)
						msg.Format(_T("%s"), m_pSelectedItemMix->proFunctionDes);
					gGlobal.PrintPlayerMessage(msg);
					ResetIronItem();
					UpdateIronMakeOrRepair();
				}			
				break;
			case MIX_IRONREPAIR:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MIXSHOP_IRONREPAIR_SUCCESS")));
				ResetIronItem();
				UpdateIronMakeOrRepair();
				break;
			}
		}
		else
		{
			switch (msgCode)
			{
			case MIXSHOP_FAIL:
			case MIXSHOP_ERROR:
				switch(m_pMixShop->type)
				{
				case MIX_WEAPONMAKING:
				case MIX_CLOTHMAKING:
				case MIX_ORNAMENTMAKING:
					gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_FAIL")));
					break;					
				case MIX_EQUIPREPAIR:
					gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_REPAIR_FAIL")));
					break;
				case MIX_STONEMIX:
					{
						if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
						{
							CItem* stone = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
							if (stone && stone->m_data && stone->m_data->item_type == ITEMTYPE_STONE)
							{
								UInt stack = m_mixShopItem[0].nowStack - 1;
								String text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_STONE_FAIL"));
								text.Format(text.c_str(), stack, String(stone->m_data->name).c_str());
								gGlobal.PrintPlayerMessage(text.c_str());
							}
						}
					}
					break;
				case MIX_GEMMIX:
					{
						TRACE_INFODTL(GLOBAL_LOGGER, _F(" mix gem success."));

						String text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_GEM_FAIL"));
						gGlobal.PrintPlayerMessage(text.c_str());
					}
					break;
				case MIX_GEMINLAY:
					{
						TRACE_INFODTL(GLOBAL_LOGGER, _F(" gem inlay fail."));

						String text =gGlobal.GetStringTable()->Get(_T("MSG_GEM_INLAY_FAIL"));
						gGlobal.PrintPlayerMessage(text.c_str());
					}
					break;
				}
				break;
			case MIXSHOP_NO_VP:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_VP")));
				break;
			case MIXSHOP_NO_EXP:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_EXP")));
				break;
			case MIXSHOP_NO_GMONEY:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_MONEY_FAIL")));
				break;
			case MIXSHOP_NO_MONEY:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_CHARGE_SHOP_NO_YUANBAO")));
				break;
			case MIXSHOP_NO_HP:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_HP")));
				break;
			case MIXSHOP_NO_MHP:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_MHP")));
				break;
			case MIXSHOP_NO_SP:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_SP")));
				break;
			case MIXSHOP_NO_MSP:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_MSP")));
				break;
			case MIXSHOP_NO_GOODNESS:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_GOODNESS")));
				break;
			case MIXSHOP_NO_HONOR:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_HONOR")));
				break;
			case MIXSHOP_NO_FACTIONDONATE:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_FACTIONDONATE")));
				break;
			case MIXSHOP_NO_MANORDONATE:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_MANORDONATE")));
				break;
			case MIXSHOP_NO_ITEMSLOT:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_SLOT_FAIL")));
				break;		
			case MIXSHOP_NO_EQ:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_EQ")));
				break;
			case MIXSHOP_NO_NEED_REBUILD:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_NEED_REBUILD")));
				break;
			case MIXSHOP_NO_MENDURING:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_MENDURING")));
				break;
			case MIXSHOP_NO_NEED_REPAIR:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_NEED_REPAIR")));
				break;
			case MIXSHOP_ITEM_REACH_LIMIT:
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_ITEM_REACH_LIMIT")));
				break;
			}

			switch(m_pMixShop->type)
			{
			case MIX_WEAPONMAKING:
			case MIX_CLOTHMAKING:
			case MIX_ORNAMENTMAKING:
				{
					ResetItemMix();
					MakeSourceList();
					UpdateItemMixSource();
				}
				break;	
			case MIX_UNSOULBOUND:
			case MIX_EQUIPREPAIR:
			case MIX_STONEMIX:
				{
					ResetAllMixItem();
					UpdateMixOrRepairItem();
				}
				break;
			case MIX_COOKING:
			case MIX_HERMETIC:
			case MIX_MAKEWINE:
				{
					SetMakeAll(false);
					m_iRemainItemCount = 0;
				}
				break;
			case MIX_GEMMIX:
			case MIX_GEMINLAY:
			case MIX_GEMREMOVE:
			case MIX_GEMMELTING:
			case MIX_GEMPOLISHING:
			case MIX_COLORSTONEMIX:
				{
					m_GemMixFirstItemId = 0;
					m_bNeedSecondCheck = false;
					m_iGemRemoveSelectIndex = -1;
					m_GemRemoveSelectGemId = 0;
					ResetAllMixItem();
					UpdateGem();
				}
				break;
			case MIX_IRONMAKE:
			case MIX_IRONREPAIR:
				ResetIronItem();
				UpdateIronMakeOrRepair();
				break;
			}							
		}
	}
}

void CMenuMixProcess::ReduceLiveSkillItemMix(UInt32Vector item_uids, UInt8Vector reduceStacks)
{
	if (gGlobal.m_pBag && gGlobal.g_pItemProcess)
	{
		for (Index i = C_INDEX(0); i < m_sourceList.size(); ++i)
		{
			for (Index j = C_INDEX(0); j < m_sourceList[i].item_uids.size(); ++j)
			{
				for (Index k = C_INDEX(0); k < item_uids.size(); ++k)
				{
					if (m_sourceList[i].item_uids[j] == item_uids[k])
					{
						m_sourceList[i].nowStack -= reduceStacks[k];
						CItem* item = gGlobal.m_pBag->GetItem(m_sourceList[i].itemSlots[j]);
						if (item)
						{
							item->SetGivenStack(item->GetGivenStack() - reduceStacks[k]);
							if ((m_sourceList[i].itemSlots[j] / BAG_ITEM_COUNT) == gGlobal.g_pItemProcess->GetBagPage())
							{
								int oldIndex = m_sourceList[i].itemSlots[j] % BAG_ITEM_COUNT;
								gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
							}

							if (m_sourceList[i].nowStacks[j] > reduceStacks[k])
								m_sourceList[i].nowStacks[j] -= reduceStacks[k];
							else
							{
								m_sourceList[i].item_uids[j] = 0;
								m_sourceList[i].itemSlots[j] = 255;
								m_sourceList[i].nowStacks[j] = 0;
							}
							reduceStacks[k] = 0;
							item_uids[k] = 0;
						}
						else
						{
							m_sourceList[i].item_uids[j] = 0;
							m_sourceList[i].itemSlots[j] = 255;
							m_sourceList[i].nowStacks[j] = 0;
						}
						break;
					}
				}

				item_uids.erase(remove(item_uids.begin(), item_uids.end(), 0), item_uids.end());
				reduceStacks.erase(remove(reduceStacks.begin(), reduceStacks.end(), 0), reduceStacks.end());

				if (m_sourceList[i].nowStack == 0)
				{
					m_sourceList[i].nowStacks.clear();
					m_sourceList[i].itemSlots.clear();
					m_sourceList[i].item_uids.clear();
					break;
				}
			}
			if (m_sourceList[i].nowStack != 0)
			{
				m_sourceList[i].nowStacks.erase(remove(m_sourceList[i].nowStacks.begin(), m_sourceList[i].nowStacks.end(), 0), m_sourceList[i].nowStacks.end());
				m_sourceList[i].itemSlots.erase(remove(m_sourceList[i].itemSlots.begin(), m_sourceList[i].itemSlots.end(), 255), m_sourceList[i].itemSlots.end());
				m_sourceList[i].item_uids.erase(remove(m_sourceList[i].item_uids.begin(), m_sourceList[i].item_uids.end(), 0), m_sourceList[i].item_uids.end());
			}
		}
	}
}

void CMenuMixProcess::ResetItemMix()
{
	for (Index i = C_INDEX(0); i < m_sourceList.size(); ++i)
		ClearItemMixSourceList(m_sourceList, i);
	
	if (gGlobal.pUIManager)
	{
		UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonMake");
		UIButton* button1 = gGlobal.pUIManager->FindUIButton("MixShopButtonMake2");
		UIButton* button2 = gGlobal.pUIManager->FindUIButton("MixShopButtonAllMake");
		if (button)
		{
			button->SetDisableState();
			gGlobal.pUIManager->AddDirtyUIItem(button);		
		}
		if (button1)
		{
			button1->SetDisableState();
			gGlobal.pUIManager->AddDirtyUIItem(button1);
		}
		if (button2)
		{
			button2->SetDisableState();
			gGlobal.pUIManager->AddDirtyUIItem(button2);
		}
		for (Index i = C_INDEX(0); i < 6; ++i)
			UpdateProbText(false, i);			
	}
	if (m_isOpenMixShop)
	{
		m_isOpenMixShop = false;
	}
}

void CMenuMixProcess::ShowMakeConsume(bool show)
{
	CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
	if (show && m_pSelectedItemMix && pChar)
	{
		for (Index i = C_INDEX(0); i < m_pSelectedItemMix->consumeType.size(); ++i)
		{
			String text;
			String textVP;

			if (m_pSelectedItemMix->consumeType[i] == MIXSHOP_CONSUME_NONE )
				continue;
			bool needRedColor = false;

			switch (m_pSelectedItemMix->consumeType[i])
			{
			case MIXSHOP_CONSUME_VP:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_VP")), 
					m_pSelectedItemMix->consumeAmount[i]);
				if (pChar->GetVP() < m_pSelectedItemMix->consumeAmount[i])
					needRedColor = true;
				else
					textVP.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_REMAIN_VP")), (pChar->GetVP() - m_pSelectedItemMix->consumeAmount[i]));
				break;
			case MIXSHOP_CONSUME_EXP:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_EXP")), 
					m_pSelectedItemMix->consumeAmount[i]);
				if (pChar->GetExp() < m_pSelectedItemMix->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_MONEY:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MONEY")), 
					m_pSelectedItemMix->consumeAmount[i]);
				if (pChar->GetMoney() < m_pSelectedItemMix->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_YUANBAO:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_YUANBAO")), 
					m_pSelectedItemMix->consumeAmount[i]);
				if (pChar->GetYuanBao() < m_pSelectedItemMix->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_HP:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_HP")), 
					m_pSelectedItemMix->consumeAmount[i]);
				if (pChar->GetHP() < m_pSelectedItemMix->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_MHP:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MHP")), 
					m_pSelectedItemMix->consumeAmount[i]);
				if (pChar->GetMHP() < m_pSelectedItemMix->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_SP:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_SP")), 
					m_pSelectedItemMix->consumeAmount[i]);
				if (pChar->GetSP() < m_pSelectedItemMix->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_MSP:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MSP")), 
					m_pSelectedItemMix->consumeAmount[i]);
				if (pChar->GetMSP() < m_pSelectedItemMix->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_GOODNESS:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_GOODNESS")), 
					m_pSelectedItemMix->consumeAmount[i]);
				//if (pChar->GetGoodness() < m_pSelectedItemMix->consumeAmount[i])
				//	needRedColor = true;
				break;
			case MIXSHOP_CONSUME_HONOR:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_HONOR")), 
					m_pSelectedItemMix->consumeAmount[i]);
				if (pChar->GetHonor() < m_pSelectedItemMix->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_FACTIONDONATE:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_FACTIONDONATE")), 
					m_pSelectedItemMix->consumeAmount[i]);
				if (pChar->GetFDonate() < m_pSelectedItemMix->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_MANORDONATE:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MANORDONATE")), 
					m_pSelectedItemMix->consumeAmount[i]);
				if (pChar->GetMDonate() < m_pSelectedItemMix->consumeAmount[i])
					needRedColor = true;
				break;
			}
			
			if (text.length() > 0)
			{
				UIStaticText* uiText = gGlobal.pUIManager->FindUIStaticText("MixShopMakeNeed00");
				if (uiText)
				{
					uiText->SetText(text.c_str());
					if (needRedColor)
						uiText->SetColor(ItemTextColor[ItemColorIndex_Red]);
					else
						uiText->SetColor(TextColor[ColorIndex_White]);
					uiText->Redraw();
					uiText->SetEnable(true);
				}
			}
			if (textVP.length() > 0)
			{
				UIStaticText* uiTextVP = gGlobal.pUIManager->FindUIStaticText("MixShopMakeNeed01");
				if (uiTextVP)
				{
					uiTextVP->SetText(textVP.c_str());
					uiTextVP->Redraw();
					uiTextVP->SetEnable(true);
				}
			}
		}
	}
	else
	{
		for (Index i = C_INDEX(0); i < MAX_CONSUME; ++i)
		{
			StringA textName;
			textName.Format("MixShopMakeNeed%02d",i);
			UIStaticText* uiText = gGlobal.pUIManager->FindUIStaticText(textName.c_str());
			if (uiText)
				uiText->SetEnable(false);
		}
	}
}

/*void CMenuMixProcess::ShowGemConsume(bool show)
{
	CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
	if (show && m_pMixShop && pChar)
	{
		for (Index i = C_INDEX(0); i < m_pMixShop->consumeType.size(); ++i)
		{
			String text;

			if (m_pMixShop->consumeType[i] == MIXSHOP_CONSUME_NONE )
				continue;
			
			bool needRedColor = false;
			switch (m_pMixShop->consumeType[i])
			{
			case MIXSHOP_CONSUME_VP:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_VP")), 
					m_pMixShop->consumeAmount[i]);
				if (pChar->GetVP() < m_pMixShop->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_EXP:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_EXP")), 
					m_pMixShop->consumeAmount[i]);
				if (pChar->GetExp() < m_pMixShop->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_MONEY:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MONEY")), 
					m_pMixShop->consumeAmount[i]);
				if (pChar->GetMoney() < m_pMixShop->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_YUANBAO:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_YUANBAO")), 
					m_pMixShop->consumeAmount[i]);
				if (pChar->GetYuanBao() < m_pMixShop->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_HP:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_HP")), 
					m_pMixShop->consumeAmount[i]);
				if (pChar->GetHP() < m_pMixShop->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_MHP:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MHP")), 
					m_pMixShop->consumeAmount[i]);
				if (pChar->GetMHP() < m_pMixShop->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_SP:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_SP")), 
					m_pMixShop->consumeAmount[i]);
				if (pChar->GetSP() < m_pMixShop->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_MSP:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MSP")), 
					m_pMixShop->consumeAmount[i]);
				if (pChar->GetMSP() < m_pMixShop->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_GOODNESS:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_GOODNESS")), 
					m_pMixShop->consumeAmount[i]);
				//if (pChar->GetGoodness() < m_pSelectedItemMix->consumeAmount[i])
				//	needRedColor = true;
				break;
			case MIXSHOP_CONSUME_HONOR:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_HONOR")), 
					m_pMixShop->consumeAmount[i]);
				if (pChar->GetHonor() < m_pMixShop->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_FACTIONDONATE:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_FACTIONDONATE")), 
					m_pMixShop->consumeAmount[i]);
				if (pChar->GetFDonate() < m_pMixShop->consumeAmount[i])
					needRedColor = true;
				break;
			case MIXSHOP_CONSUME_MANORDONATE:
				text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MANORDONATE")), 
					m_pMixShop->consumeAmount[i]);
				if (pChar->GetMDonate() < m_pMixShop->consumeAmount[i])
					needRedColor = true;
				break;
			}
			
			if (text.length() > 0)
			{
				StringA textName;
				textName.Format("MixShopGemNeed%02d",i);
				UIStaticText* uiText = gGlobal.pUIManager->FindUIStaticText(textName.c_str());
				if (uiText)
				{
					uiText->SetText(text.c_str());
					if (needRedColor)
						uiText->SetColor(ItemTextColor[ItemColorIndex_Red]);
					else
						uiText->SetColor(TextColor[ColorIndex_White]);
					uiText->Redraw();
					uiText->SetEnable(true);
				}
			}
		}
	}
	else
	{
		for (Index i = C_INDEX(0); i < MAX_CONSUME; ++i)
		{
			StringA textName;
			textName.Format("MixShopGemNeed%02d",i);
			UIStaticText* uiText = gGlobal.pUIManager->FindUIStaticText(textName.c_str());
			if (uiText)
				uiText->SetEnable(false);
		}
	}
}*/

void CMenuMixProcess::ShowMixConsume(bool show)
{
	if (show && m_pMixShop)
	{
		UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel("MixShopNeed");
		if (pChatPanel)
		{
			pChatPanel->ClearAllRow();
			for (UInt8 i = 0; i < m_pMixShop->consumeType.size(); ++i)
			{
				String text;
				if (m_pMixShop->consumeType[i] == MIXSHOP_CONSUME_NONE )
					continue;

				switch (m_pMixShop->consumeType[i])
				{
				case MIXSHOP_CONSUME_VP:
					text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_VP")), 
						m_pMixShop->consumeAmount[i]);
					break;
				case MIXSHOP_CONSUME_EXP:
					text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_EXP")), 
						m_pMixShop->consumeAmount[i]);
					break;
				case MIXSHOP_CONSUME_MONEY:
					if (m_pMixShop->type == MIX_EQUIPREPAIR)
					{
						UInt32 price = 0;
						if (m_pEqItem)
						{
							UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonRebuild");
							
							if (button && button->IsEnable())
								price = m_pEqItem->m_data->g_price;
							else if (m_pEqItem->GetMEndure() > 0)
								price = m_pEqItem->m_data->g_price * (m_pEqItem->GetMEndure() - m_pEqItem->GetEndure()) / m_pEqItem->GetMEndure();
						}
						text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MONEY")), price);
					}
					else
						text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MONEY")), 
							m_pMixShop->consumeAmount[i]);
					break;
				case MIXSHOP_CONSUME_YUANBAO:
					text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_YUANBAO")), 
						m_pMixShop->consumeAmount[i]);
					break;
				case MIXSHOP_CONSUME_HP:
					text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_HP")), 
						m_pMixShop->consumeAmount[i]);
					break;
				case MIXSHOP_CONSUME_MHP:
					text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MHP")), 
						m_pMixShop->consumeAmount[i]);
					break;
				case MIXSHOP_CONSUME_SP:
					text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_SP")), 
						m_pMixShop->consumeAmount[i]);
					break;
				case MIXSHOP_CONSUME_MSP:
					text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MSP")), 
						m_pMixShop->consumeAmount[i]);
					break;
				case MIXSHOP_CONSUME_GOODNESS:
					text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_GOODNESS")), 
						m_pMixShop->consumeAmount[i]);
					break;
				case MIXSHOP_CONSUME_HONOR:
					text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_HONOR")), 
						m_pMixShop->consumeAmount[i]);
					break;
				case MIXSHOP_CONSUME_FACTIONDONATE:
					text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_FACTIONDONATE")), 
						m_pMixShop->consumeAmount[i]);
					break;
				case MIXSHOP_CONSUME_MANORDONATE:
					text.Format(_T("%s %d"),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKING_MANORDONATE")), 
						m_pMixShop->consumeAmount[i]);
					break;
				}
				
				if (text.length() > 0)
				{
					pChatPanel->AddStringW(text.c_str());
				}
			}
			pChatPanel->EndRow();
			pChatPanel->Redraw();
			pChatPanel->SetEnable(true);
		}
	}
	else
	{
		UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel("MixShopNeed");
		if (pChatPanel)
			pChatPanel->SetEnable(false);
	}
}

void CMenuMixProcess::ResetProductScrollBar()
{
	UInt16 lineCount = 0;
	for (ItemMixTypeList::iterator itr = m_typeItemList.begin(); itr != m_typeItemList.end(); ++itr)
	{
		++lineCount;
		if (itr->second->isOpen)
			lineCount += (UInt16)itr->second->dataList.size();

		UIScrollBarV* scrollBar = gGlobal.pUIManager->FindUIScrollBarV("MixShopProductScrollBar");
		if (scrollBar)
		{
			if (lineCount >= MAX_MIX_MAKE_LINE_COUNT)
				scrollBar->SetNumCount(lineCount - MAX_MIX_MAKE_LINE_COUNT);
			else
				scrollBar->SetNumCount(0);
		}
	}
}

bool CMenuMixProcess::IsStartMaking()
{
	if (m_iStartTime > 0)
		return true;
	return false;
}

void CMenuMixProcess::CancelMake()
{
	m_iStartTime = 0;
	if (gGlobal.pPeerProxy && gGlobal.m_pBag && IsStartMaking())
		gGlobal.m_pBag->MarkMixStartTime(gGlobal.pPeerProxy, RPCContext(), BOOLEAN_FALSE);
	if (m_pMixShop)
	{
		StringA timeLineName;
		switch(m_pMixShop->type)
		{
		case MIX_WEAPONMAKING:
		case MIX_CLOTHMAKING:
		case MIX_ORNAMENTMAKING:
			timeLineName = "MixShopMakeTimeLine";
			break;
		case MIX_COOKING:
		case MIX_HERMETIC:
		case MIX_MAKEWINE:
			{
				timeLineName = "MixShopMakeTimeLine";
				m_bMakeAll = false;
				m_iRemainItemCount = 0;

				bool needNormal = true;
				for(Index i = C_INDEX(0); i < m_sourceList.size(); ++i)
				{
					if (m_sourceList[i].nowStack < m_sourceList[i].maxStack && m_sourceList[i].maxStack != 0)
					{
						needNormal = false;							
						break;
					}
				}

				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonMake2");
				UIButton* button2 = gGlobal.pUIManager->FindUIButton("MixShopButtonAllMake");
				
				if (needNormal && button && button2)
				{
					button->SetNormalState();
					button2->SetNormalState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
					gGlobal.pUIManager->AddDirtyUIItem(button2);
				}
			}
			break;
		case MIX_UNSOULBOUND:
			{
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonUnSoulBound");
				if (button)
				{
					button->SetNormalState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
				}
			}
			timeLineName = "MixShopTimeLine";
			break;
		case MIX_EQUIPREPAIR:
			timeLineName = "MixShopTimeLine";
			break;
		case MIX_GEMMIX:
			timeLineName = "MixShopGemTimeLine";
			break;
		case MIX_IRONMAKE:
			{
				UIButton* button = gGlobal.pUIManager->FindUIButton("IronShopButtonMake");
				if (button)
				{
					button->SetNormalState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
				}
				timeLineName = "IronShopTimeLine";
			}
			break;
		case MIX_IRONREPAIR:
			{
				UIButton* button = gGlobal.pUIManager->FindUIButton("IronShopButtonRepair");
				if (button)
				{
					button->SetNormalState();
					gGlobal.pUIManager->AddDirtyUIItem(button);
				}
				timeLineName = "IronShopTimeLine";
			}
			break;
		}
		
		UILinerImage* temp = gGlobal.pUIManager->FindUILinerImage(timeLineName.c_str());
		if (temp)
			temp->SetEnable(false);
	}
}

void CMenuMixProcess::RemoveMakeItem(int sourceIndex)
{
	if (sourceIndex >= 0)
	{
		UInt index = sourceIndex + 2 * m_iStartSourceIndex;
		for (Index i = C_INDEX(0); i < m_sourceList[index].itemSlots.size(); ++i)
		{
			CItem* item = gGlobal.m_pBag->GetItem(m_sourceList[index].itemSlots[i]);
			if (item)
			{
				item->SetGivenStack(0);
				int oldItembag = m_sourceList[index].itemSlots[i] / BAG_ITEM_COUNT;

				if (oldItembag == gGlobal.g_pItemProcess->GetBagPage())
				{
					int oldIndex = m_sourceList[index].itemSlots[i] % BAG_ITEM_COUNT;
					gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
				}

				if (m_sourceList[index].itemSwitch == MIX_SWITCH_STONE)
				{
					for (UInt8 i = 0; i < 6; ++i)
						UpdateProbText(false, i);
				}
			}
		}
		m_sourceList[index].nowStack = 0;
		m_sourceList[index].nowStacks.clear();
		m_sourceList[index].itemSlots.clear();
		m_sourceList[index].item_uids.clear();
		
		UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonMake");
		if (button )
		{
			button->SetDisableState();
			gGlobal.pUIManager->AddDirtyUIItem(button);
		
		}
		UpdateItemMixSource();
	}
}

void CMenuMixProcess::RemoveIronItem(int picIndex)
{
	if (picIndex >= 0 && m_pMixShop)
	{
		UInt index = m_iStartProductIndex + picIndex;
		if (index < m_mixShopItem.size() && m_mixShopItem[index].itemSlots.size() > 0 )	
		{
			if (index == 0)
			{
				ClearItemMixSourceList(m_mixShopItem, index);
				ClearItemMixSourceList(m_mixShopItem, index+1);
			}
			else
				ClearItemMixSourceList(m_mixShopItem, index);

			UIButton* buttonM = gGlobal.pUIManager->FindUIButton("IronShopButtonMake");
			UIButton* buttonR = gGlobal.pUIManager->FindUIButton("IronShopButtonRepair");
			if (m_pMixShop->type == MIX_IRONMAKE)
			{
				buttonM->SetDisableState();
				gGlobal.pUIManager->AddDirtyUIItem(buttonM);
			}
			else if (m_pMixShop->type == MIX_IRONREPAIR)
			{
				buttonR->SetDisableState();
				gGlobal.pUIManager->AddDirtyUIItem(buttonR);
			}
		}
		UpdateIronMakeOrRepair();
	}
}

void CMenuMixProcess::RemoveMixItem(int picIndex)
{
	if (picIndex >= 0 && m_pMixShop)
	{
		UInt index = m_iStartProductIndex + picIndex;
		if (index < m_mixShopItem.size() && m_mixShopItem[index].itemSlots.size() > 0 )	
		{
			CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[index].itemSlots[0]);
			ClearItemMixSourceList(m_mixShopItem, index);
			
			UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonRepair");
			UIButton* button1 = gGlobal.pUIManager->FindUIButton("MixShopButtonRebuild");
			UIButton* button2 = gGlobal.pUIManager->FindUIButton("MixShopButtonStoneMix");
			UIButton* button3 = gGlobal.pUIManager->FindUIButton("MixShopButtonUnSoulBound");
			
			if (button && button1 && button2 && button3 && item)
			{
				if (m_pMixShop->type == MIX_EQUIPREPAIR)
				{
					button2->SetEnable(false);
					if (item->m_data->item_type == ITEMTYPE_REBUILD)
					{
						button->SetEnable(true);
						button1->SetEnable(false);
					}
					else if (item->m_data->item_type == ITEMTYPE_EQUIP)
					{
						button->SetDisableState();
						button1->SetDisableState();
						gGlobal.pUIManager->AddDirtyUIItem(button);
						gGlobal.pUIManager->AddDirtyUIItem(button1);
						m_pEqItem = NULL;
					}
				}
				else if (m_pMixShop->type == MIX_STONEMIX)
				{
					UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel("MixShopSuggestion");
					if (pChatPanel)
					{
						pChatPanel->ClearAllRow();
						pChatPanel->EndRow();
						pChatPanel->Redraw();
						pChatPanel->SetEnable(true);
					}
					button->SetEnable(false);
					button1->SetEnable(false);
					button2->SetDisableState();
					button2->SetEnable(true);
				}
				else if (m_pMixShop->type == MIX_UNSOULBOUND)
				{
					button2->SetEnable(false);
					if (item->m_data->item_type == ITEMTYPE_EQUIP)
					{
						ResetAllMixItem();
					}
					else
					{
						button3->SetDisableState();
						gGlobal.pUIManager->AddDirtyUIItem(button3);
					}
				}
			}
			ShowMixConsume(true);
			UpdateMixOrRepairItem();			
		}
	}
}

void CMenuMixProcess::UpdateProbText(bool enable, UInt8 index, UInt8 value)
{
	if (m_pMixShop && m_pMixShop->shopID >= 26 && m_pMixShop->shopID <= 34)
		return ;

	StringA imgName;
	UIImage* img = NULL;
	UIStaticText* text = NULL;

	switch (index)
	{
	case MIXITEM_QUA_FAIL:
		break;
	case MIXITEM_QUA_NORMAL:	imgName = "MixShopQualityNormal";	break;
	case MIXITEM_QUA_FINE:		imgName = "MixShopQualityFine";		break;
	case MIXITEM_QUA_PRETTY:	imgName = "MixShopQualityPretty";	break;
	case MIXITEM_QUA_HIQUALITY:	imgName = "MixShopQualityHiQuality";	break;
	case MIXITEM_QUA_BEST:		imgName = "MixShopQualityBest";		break;
	case MIXITEM_QUA_GOD:
		break;
	}
	
	img = gGlobal.pUIManager->FindUIImage(imgName.c_str());
	if (img)
		img->SetEnable(enable);

	if (index > 0)
	{
		imgName.Format("MixShopQualityProp%02d", index - 1);
		text = gGlobal.pUIManager->FindUIStaticText(imgName.c_str());
		if (text)
		{
			if (enable)
			{
				String num;
				num.Format(_T("%d%%"), value);
				text->SetText(num.c_str());
				text->Redraw();
			}
			text->SetEnable(enable);
		}
	}
}

bool CMenuMixProcess::IsConsumeAccept(UInt8Vector consumeType, UInt32Vector consumeAmount, bool showMsg)
{
	bool accept = true;
	CCharacter* m_pOwner = gGlobal.g_pCharacter->GetCharNetObject();
	if (m_pOwner == NULL || m_pMixShop == NULL)
		return false;
	String text;
	for (Index i = C_INDEX(0); i < consumeType.size(); ++i)
	{
		switch (consumeType[i])
		{
		case MIXSHOP_CONSUME_VP:
			if (m_pOwner->GetVP() < consumeAmount[i])
			{
				accept = false;
				text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_VP"));
			}
			break;
		case MIXSHOP_CONSUME_EXP:
			if (m_pOwner->GetExp() < consumeAmount[i])
			{
				accept = false;
				text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_EXP"));
			}
			break;
		case MIXSHOP_CONSUME_MONEY:
			if (m_pMixShop->type == MIX_WEAPONMAKING ||
				m_pMixShop->type == MIX_CLOTHMAKING ||
				m_pMixShop->type == MIX_ORNAMENTMAKING ||
				m_pMixShop->type == MIX_STONEMIX )
			{
				if (m_pOwner->GetMoney() < consumeAmount[i])
				{
					accept = false;
					text =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_MONEY_FAIL"));
				}
			}
			else if (m_pMixShop->type == MIX_EQUIPREPAIR)
			{
				UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonRepair");
				UIButton* button1 = gGlobal.pUIManager->FindUIButton("MixShopButtonRebuild");
				if (button && button1 && m_pEqItem)
				{
					UInt32 price = 0;
					if (m_pEqItem)
					{
						UIButton* button = gGlobal.pUIManager->FindUIButton("MixShopButtonRebuild");
						
						if (button && button->IsEnable())
							price = m_pEqItem->m_data->g_price;
						else if (m_pEqItem->GetMEndure() > 0)
							price = m_pEqItem->m_data->g_price * (m_pEqItem->GetMEndure() - m_pEqItem->GetEndure()) / m_pEqItem->GetMEndure();
					}
					if (m_pEqItem->GetMEndure() == m_pEqItem->GetEndure())
					{
						accept = false;
						text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_NEED_REPAIR"));
					}
					else if (m_pOwner->GetMoney() < (price * consumeAmount[i]) )
					{
						accept = false;
						text =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_MONEY_FAIL"));
					}
				}
				else
				{
					accept = false;
					text =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_MONEY_FAIL"));
				}
			}
			break;
		case MIXSHOP_CONSUME_YUANBAO:
			if (m_pOwner->GetYuanBao() < consumeAmount[i])
			{
				accept = false;
				text =gGlobal.GetStringTable()->Get(_T("MSG_CHARGE_SHOP_NO_YUANBAO"));
			}
			break;
		case MIXSHOP_CONSUME_HP:
			if (m_pOwner->GetHP() < consumeAmount[i])
			{
				accept = false;
				text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_HP"));
			}
			break;
		case MIXSHOP_CONSUME_MHP:
			if (m_pOwner->GetMHP() < consumeAmount[i])
			{
				accept = false;
				text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_MHP"));
			}
			break;
		case MIXSHOP_CONSUME_SP:
			if (m_pOwner->GetSP() < consumeAmount[i])
			{
				accept = false;
				text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_SP"));
			}
			break;
		case MIXSHOP_CONSUME_MSP:
			if (m_pOwner->GetMSP() < consumeAmount[i])
			{
				accept = false;
				text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_MSP"));
			}
			break;
		case MIXSHOP_CONSUME_GOODNESS:
			//if (m_pOwner->GetGoodness() < itemMix->consumeAmount[i])
			//	msgCode = MIXSHOP_NO_GOODNESS;
			break;
		case MIXSHOP_CONSUME_HONOR:
			if (m_pOwner->GetHonor() < consumeAmount[i])
			{
				accept = false;
				text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_HONOR"));
			}
			break;
		case MIXSHOP_CONSUME_FACTIONDONATE:
			if (m_pOwner->GetFDonate() < consumeAmount[i])
			{
				accept = false;
				text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_FACTIONDONATE"));
			}
			break;
		case MIXSHOP_CONSUME_MANORDONATE:
			if (m_pOwner->GetMDonate() < consumeAmount[i])
			{
				accept = false;
				text =gGlobal.GetStringTable()->Get(_T("MSG_MIX_NO_MANORDONATE"));
			}
			break;
		}
		if (!accept)
			break;
	}
	if (!accept && showMsg && text.length() > 0)
		gGlobal.PrintPlayerMessage(text.c_str());

	return accept;
}

void CMenuMixProcess::SetRemainItemCount(int remainCount) 
{ 
	m_iRemainItemCount = remainCount; 
}

bool CMenuMixProcess::IsRepairShop()
{
	if (m_pMixShop && m_pMixShop->type == MIX_EQUIPREPAIR)
		return true;
	return false;
}

bool CMenuMixProcess::IsStoneMixShop()
{
	if (m_pMixShop && m_pMixShop->type == MIX_STONEMIX)
		return true;
	return false;
}

int CMenuMixProcess::GetMaxStoneNumByItemSlot(int itemSlot)
{
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);
	if (item)
		return gGlobal.m_resource->GetMaxStoneMixNum(item->GetItem_id());
	return 0;
}
bool CMenuMixProcess::HasResultStone(int itemSlot, bool showErrMessage)
{
	CItem* tempItem = gGlobal.m_pBag->GetItem(itemSlot);
	if (tempItem)
	{
		if (gGlobal.m_resource->GetResultStoneID(tempItem->GetItem_id()) > 0)
			return true;
	}
	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_MIX_CANT_MAKE_HIGHER_STONE")));
	return false;
}

void CMenuMixProcess::UpdateGem()
{
	if (m_pMixShop)
	{
		if(m_pMixShop->type == MIX_GEMMIX)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("CMenuMixProcess::UpdateGem() MIX_GEMMIX."));

			StringA uiItemName;
			UIImage* img = NULL;
			UIAniIcon* pAniIcon = NULL;

			uiItemName.Format("MixShopGemTipsBG");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopGemTipsBG2");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			for (Index i = C_INDEX(0); i < C_INDEX(5); ++i)
			{
				uiItemName.Format("MixShopGemBG%02i", i);
				img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
				uiItemName.Format("MixShopGemItem%02i", i);
				pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
				if (img && pAniIcon)
				{
					img->SetEnable(true);

					UInt index = i + m_iStartProductIndex;
					UInt slotSwitch = 0;
					UInt slotItemType = 0;
					UInt32 slotItemID = 0;
					String slotName;

					switch (index)
					{
					case 0:
						slotSwitch = m_pMixShop->slotSwitch01;
						slotItemType = m_pMixShop->slotItemType01;
						slotItemID = m_pMixShop->slotItemID01;
						slotName = String(m_pMixShop->slotItemName01);
						break;
					case 1:
						slotSwitch = m_pMixShop->slotSwitch02;
						slotItemType = m_pMixShop->slotItemType02;
						slotItemID = m_pMixShop->slotItemID02;
						slotName = String(m_pMixShop->slotItemName02);
						break;
					case 2:
						slotSwitch = m_pMixShop->slotSwitch03;
						slotItemType = m_pMixShop->slotItemType03;
						slotItemID = m_pMixShop->slotItemID03;
						slotName = String(m_pMixShop->slotItemName03);
						break;
					case 3:
						slotSwitch = m_pMixShop->slotSwitch04;
						slotItemType = m_pMixShop->slotItemType04;
						slotItemID = m_pMixShop->slotItemID04;
						slotName = String(m_pMixShop->slotItemName04);
						break;
					case 4:
						slotSwitch = m_pMixShop->slotSwitch05;
						slotItemType = m_pMixShop->slotItemType05;
						slotItemID = m_pMixShop->slotItemID05;
						slotName = String(m_pMixShop->slotItemName05);
						break;
					}

					if (slotSwitch)
					{
						if (m_mixShopItem.size() > 0 && m_mixShopItem[index].itemSlots.size() > 0)
						{
							pAniIcon->ClearAniData();
							CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[index].itemSlots[0]);
							if (item && item->m_data)
							{
								pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
								pAniIcon->SetRenderStyle(0);
								pAniIcon->SetEnable(true);
							}
						}
						else if (slotItemID > 0)
						{
							const ItemData* pItemData = gGlobal.m_resource->GetItemData(slotItemID);
							if (pItemData)
							{
								pAniIcon->SetAnimationData(String(pItemData->icon2).c_str());
								pAniIcon->SetRenderStyle(1);
								pAniIcon->SetEnable(true);
							}
						}
						else
							pAniIcon->SetEnable(false);
					}
					else
					{
						pAniIcon->SetEnable(false);
					}
				}

				/*UIImage* img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
				if (img)
				{
				img->SetEnable(true);
				}*/
			}
		}
		else if(m_pMixShop->type == MIX_GEMINLAY)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("CMenuMixProcess::UpdateGem() MIX_GEMINLAY."));

			StringA uiItemName;
			UIImage* img = NULL;
			UIAniIcon* pAniIcon = NULL;
			UIImage* namebg = NULL;

			// eq item

			uiItemName.Format("MixShopGemInlayEqItemBG");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			uiItemName.Format("MixShopGemInlayEqItemNameBG");
			namebg = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img && namebg)
			{
				img->SetEnable(true);
				namebg->SetEnable(true);
			}

			uiItemName.Format("MixShopGemInlayEqItem");
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
			if (pAniIcon)
			{
				if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
				{
					pAniIcon->ClearAniData();
					CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
					if (item && item->m_data)
					{
						pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
						pAniIcon->SetRenderStyle(0);
						pAniIcon->SetEnable(true);
					}
					else
					{
						pAniIcon->SetEnable(false);
					}
				}
				else
				{
					pAniIcon->SetEnable(false);
				}
			}

			// gem
			uiItemName.Format("MixShopGemInlayGemItemBG");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			uiItemName.Format("MixShopGemInlayGemItemNameBG");
			namebg = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img && namebg)
			{
				img->SetEnable(true);
				namebg->SetEnable(true);
			}

			uiItemName.Format("MixShopGemInlayGemItem");
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
			if (pAniIcon)
			{
				if (m_mixShopItem.size() > 1 && m_mixShopItem[1].itemSlots.size() > 0)
				{
					pAniIcon->ClearAniData();
					CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[1].itemSlots[0]);
					if (item && item->m_data)
					{
						pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
						pAniIcon->SetRenderStyle(0);
						pAniIcon->SetEnable(true);
					}
					else
					{
						pAniIcon->SetEnable(false);
					}
				}
				else
				{
					pAniIcon->SetEnable(false);
				}
			}

			// consume
			uiItemName.Format("MixShopGemInlayConsumeItemBG");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			uiItemName.Format("MixShopGemInlayConsumeItemNameBG");
			namebg = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img && namebg)
			{
				img->SetEnable(true);
				namebg->SetEnable(true);
			}

			uiItemName.Format("MixShopGemInlayConsumeItem");
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
			if (pAniIcon)
			{
				if (m_mixShopItem.size() > 2 && m_mixShopItem[2].itemSlots.size() > 0)
				{
					pAniIcon->ClearAniData();
					CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[2].itemSlots[0]);
					if (item && item->m_data)
					{
						pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
						pAniIcon->SetRenderStyle(0);
						pAniIcon->SetEnable(true);
					}
					else
					{
						pAniIcon->SetEnable(false);
					}
				}
				else
				{
					pAniIcon->SetEnable(false);
				}
			}
		}
		else if(m_pMixShop->type == MIX_GEMREMOVE)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("CMenuMixProcess::UpdateGem() MIX_GEMREMOVE."));

			StringA uiItemName;
			UIImage* img = NULL;
			UIAniIcon* pAniIcon = NULL;
			UIAniIcon* pGemAniIcon = NULL;
			UIImage* namebg = NULL;

			uiItemName.Format("MixShopGemRemoveEqItemBG");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			uiItemName.Format("MixShopGemRemoveEqItemNameBG");
			namebg = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img && namebg)
			{
				img->SetEnable(true);
				namebg->SetEnable(true);
			}

			uiItemName.Format("MixShopGemRemoveGemItemBG00");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}
			uiItemName.Format("MixShopGemRemoveGemItemBG01");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}
			uiItemName.Format("MixShopGemRemoveGemItemNameBG");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopGemRemoveEqItem");
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
			if (pAniIcon)
			{
				if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
				{
					pAniIcon->ClearAniData();
					CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
					if (item && item->m_data)
					{
						pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
						pAniIcon->SetRenderStyle(0);
						pAniIcon->SetEnable(true);

						// inlay gem
						for (Index i = C_INDEX(0); i < C_INDEX(2); ++i)
						{
							uiItemName.Format("MixShopGemRemoveGemItem%02i", i);
							pGemAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
							uiItemName.Format("MixShopGemRemoveGemItemSelect%02i", i);
							img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
							if (pGemAniIcon && img)
							{
								UInt16 Inlay_gem = item->GetInlay_gem(i);
								if (Inlay_gem > 0)
								{
									pGemAniIcon->ClearAniData();
									const ItemData* pItemData = gGlobal.m_resource->GetItemData(Inlay_gem);
									if (pItemData)
									{
										pGemAniIcon->SetAnimationData(String(pItemData->icon2).c_str());
										pGemAniIcon->SetRenderStyle(0);
										pGemAniIcon->SetEnable(true);
									}

									if (i == m_iGemRemoveSelectIndex)
									{
										img->SetEnable(true);
									}
									else
									{
										img->SetEnable(false);
									}
								}
								else
								{
									pGemAniIcon->SetEnable(false);
									img->SetEnable(false);
								}
							}
						}
					}
					else
					{
						pAniIcon->SetEnable(false);

						for (Index i = C_INDEX(0); i < C_INDEX(2); ++i)
						{
							uiItemName.Format("MixShopGemRemoveGemItem%02i", i);
							pGemAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
							uiItemName.Format("MixShopGemRemoveGemItemSelect%02i", i);
							img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
							if (pGemAniIcon && img)
							{
								pGemAniIcon->SetEnable(false);
								img->SetEnable(false);
							}
						}
					}
				}
				else
				{
					pAniIcon->SetEnable(false);

					for (Index i = C_INDEX(0); i < C_INDEX(2); ++i)
					{
						uiItemName.Format("MixShopGemRemoveGemItem%02i", i);
						pGemAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
						uiItemName.Format("MixShopGemRemoveGemItemSelect%02i", i);
						img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
						if (pGemAniIcon && img)
						{
							pGemAniIcon->SetEnable(false);
							img->SetEnable(false);
						}
					}
				}
			}

			uiItemName.Format("MixShopGemRemoveConsumeItemBG");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			uiItemName.Format("MixShopGemRemoveConsumeItemNameBG");
			namebg = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img && namebg)
			{
				img->SetEnable(true);
				namebg->SetEnable(true);
			}

			uiItemName.Format("MixShopGemRemoveConsumeItem");
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
			if (pAniIcon)
			{
				if (m_mixShopItem.size() > 1 && m_mixShopItem[1].itemSlots.size() > 0)
				{
					pAniIcon->ClearAniData();
					CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[1].itemSlots[0]);
					if (item && item->m_data)
					{
						pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
						pAniIcon->SetRenderStyle(0);
						pAniIcon->SetEnable(true);
					}
					else
					{
						pAniIcon->SetEnable(false);
					}
				}
				else
				{
					pAniIcon->SetEnable(false);
				}
			}
		}
		else if(m_pMixShop->type == MIX_GEMMELTING)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("CMenuMixProcess::UpdateGem() MIX_GEMMELTING."));

			StringA uiItemName;
			UIImage* img = NULL;
			UIAniIcon* pAniIcon = NULL;

			uiItemName.Format("MixShopGemMeltingTipsBG");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopGemMeltingTipsBG2");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopGemMeltingItemBG");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopGemMeltingItem");
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
			if (pAniIcon)
			{
				if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
				{
					pAniIcon->ClearAniData();
					CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
					if (item && item->m_data)
					{
						pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
						pAniIcon->SetRenderStyle(0);
						pAniIcon->SetEnable(true);
					}
					else
					{
						pAniIcon->SetEnable(false);
					}
				}
				else
				{
					pAniIcon->SetEnable(false);
				}
			}
		}
		else if(m_pMixShop->type == MIX_GEMPOLISHING)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("CMenuMixProcess::UpdateGem() MIX_GEMPOLISHING."));

			StringA uiItemName;
			UIImage* img = NULL;
			UIAniIcon* pAniIcon = NULL;

			uiItemName.Format("MixShopGemPolishingTipsBG");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopGemPolishingTipsBG1");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopGemPolishingBG1");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			Int8 PolishingGemLevel = 0;
			uiItemName.Format("MixShopGemPolishingItem1");
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
			if (pAniIcon)
			{
				if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
				{
					pAniIcon->ClearAniData();
					CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
					if (item && item->m_data)
					{
						pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
						pAniIcon->SetRenderStyle(0);
						pAniIcon->SetEnable(true);

						gGlobal.g_TempResult = 0;
						CallLuaFunc(gGlobal.g_pParentLua, "GetMixShopInlayGemType", String(), item->GetItem_id());
						PolishingGemLevel = (Int8)gGlobal.g_TempResult;
					}
					else
					{
						pAniIcon->SetEnable(false);
					}
				}
				else
				{
					pAniIcon->SetEnable(false);
				}
			}

			uiItemName.Format("MixShopGemPolishingTipsBG2");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopGemPolishingBG2");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopGemPolishingItem2");
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
			if (pAniIcon)
			{
				if (m_mixShopItem.size() > 1 && m_mixShopItem[1].itemSlots.size() > 0)
				{
					pAniIcon->ClearAniData();
					CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[1].itemSlots[0]);
					if (item && item->m_data)
					{
						pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
						pAniIcon->SetRenderStyle(0);
						pAniIcon->SetEnable(true);
					}
					else
					{
						pAniIcon->SetEnable(false);
					}
				}
				else
				{
					pAniIcon->SetEnable(false);
				}
			}

			uiItemName.Format("MixShopGemPolishingTipsBG3");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopGemPolishingBG3");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			if (PolishingGemLevel > 0)
			{
				for (int i = 0; i < 8; i++)
				{
					uiItemName.Format("MixShopGemPolishingItemEff%d", i);
					img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
					if (img)
					{
						if (i == PolishingGemLevel - 1)
						{
							img->SetEnable(true);
						}
						else
						{
							img->SetEnable(false);
						}
					}
				}
			}
		}
		else if(m_pMixShop->type == MIX_COLORSTONEMIX)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("CMenuMixProcess::UpdateGem() MIX_COLORSTONEMIX."));

			StringA uiItemName;
			UIImage* img = NULL;
			UIAniIcon* pAniIcon = NULL;

			uiItemName.Format("MixShopColorStoneMixTipsBG");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopColorStoneMixTipsBG1");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopColorStoneMixBG1");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			Int8 PolishingGemLevel = 0;
			uiItemName.Format("MixShopColorStoneMixItem1");
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
			if (pAniIcon)
			{
				if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
				{
					pAniIcon->ClearAniData();
					CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
					if (item && item->m_data)
					{
						pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
						pAniIcon->SetRenderStyle(0);
						pAniIcon->SetEnable(true);
					}
					else
					{
						pAniIcon->SetEnable(false);
					}
				}
				else
				{
					pAniIcon->SetEnable(false);
				}
			}

			uiItemName.Format("MixShopColorStoneMixTipsBG2");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopColorStoneMixBG2");
			img = gGlobal.pUIManager->FindUIImage(uiItemName.c_str());
			if (img)
			{
				img->SetEnable(true);
			}

			uiItemName.Format("MixShopColorStoneMixItem2");
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiItemName.c_str());
			if (pAniIcon)
			{
				if (m_mixShopItem.size() > 1 && m_mixShopItem[1].itemSlots.size() > 0)
				{
					pAniIcon->ClearAniData();
					CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[1].itemSlots[0]);
					if (item && item->m_data)
					{
						pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
						pAniIcon->SetRenderStyle(0);
						pAniIcon->SetEnable(true);
					}
					else
					{
						pAniIcon->SetEnable(false);
					}
				}
				else
				{
					pAniIcon->SetEnable(false);
				}
			}
		}
		else
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CMenuMixProcess::UpdateGem() Unknown shop type."));
		}

		// consume money
		StringA uiItemName;
		uiItemName.Format("MixShopGemConsumeMoney");
		UIStaticText* uiText = gGlobal.pUIManager->FindUIStaticText(uiItemName.c_str());
		if (uiText)
		{
			Int32 ComsumeMoney = 0;
			if(m_pMixShop->type == MIX_GEMMIX && m_GemMixFirstItemId != 0)
			{
				gGlobal.g_TempResult = 0;
				CallLuaFunc(gGlobal.g_pParentLua, "GetGemMixConsumeMoney", String(), m_GemMixFirstItemId);
				ComsumeMoney = (Int32)gGlobal.g_TempResult;
			}
			else if(m_pMixShop->type == MIX_GEMINLAY)
			{
				if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
				{
					CItem* eqItem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
					if (eqItem)
					{
						gGlobal.g_TempResult = 0;
						CallLuaFunc(gGlobal.g_pParentLua, "GetGemInlayConsumeMoney", String(), eqItem->m_data->reqLv);
						ComsumeMoney = (Int32)gGlobal.g_TempResult;
					}
				}
			}
			else if(m_pMixShop->type == MIX_GEMREMOVE)
			{
				if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
				{
					CItem* eqItem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
					if (eqItem)
					{
						gGlobal.g_TempResult = 0;
						CallLuaFunc(gGlobal.g_pParentLua, "GetGemRemoveConsumeMoney", String(), eqItem->m_data->reqLv);
						ComsumeMoney = (Int32)gGlobal.g_TempResult;
					}
				}
			}
			else if(m_pMixShop->type == MIX_GEMMELTING)
			{
				if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
				{
					CItem* gemItem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
					if (gemItem)
					{
						gGlobal.g_TempResult = 0;
						CallLuaFunc(gGlobal.g_pParentLua, "GetGemMeltingConsumeMoney", String(), gemItem->GetItem_id());
						ComsumeMoney = (Int32)gGlobal.g_TempResult;
					}
				}
			}
			else if(m_pMixShop->type == MIX_GEMPOLISHING)
			{
				if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
				{
					CItem* gemItem = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
					if (gemItem)
					{
						gGlobal.g_TempResult = 0;
						CallLuaFunc(gGlobal.g_pParentLua, "GetGemPolishingConsumeMoney", String(), gemItem->GetItem_id());
						ComsumeMoney = (Int32)gGlobal.g_TempResult;
					}
				}
			}
			else if(m_pMixShop->type == MIX_COLORSTONEMIX)
			{
			}

			if (ComsumeMoney > 0)
			{
				bool needRedColor = false;
				CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
				if (pChar)
				{
					if(pChar->GetMoney() < (UInt32)ComsumeMoney)
						needRedColor = true;
				}
				if (needRedColor)
					uiText->SetColor(ItemTextColor[ItemColorIndex_Red]);
				else
					uiText->SetColor(TextColor[ColorIndex_White]);

				String temp;
				temp.Format(_T("%d"), ComsumeMoney);
				uiText->SetText(temp);
				uiText->Redraw();
				uiText->SetEnable(true);
			}
			else
				uiText->SetEnable(false);
		}
	}

	/*if (m_pMixShop && m_pEqItem && m_pEqItem->m_data)
	{
	UIAniIcon* pAniIcon = NULL;
	UIImage* pIm = NULL;
	StringA uiName;
	UInt posOfGemList = 0;
	for (Index i = C_INDEX(0); i < m_pEqItem->m_data->eq_maxGem; ++i)
	{
	uiName.Empty();
	if (m_pEqItem->GetEq_gem() < i)
	{
	switch(m_pEqItem->m_data->eqType)
	{
	case EQTYPE_SPEAR:	case EQTYPE_WIPE:	case EQTYPE_FAN:	case EQTYPE_PEN:
	case EQTYPE_SWORD:	case EQTYPE_DBSWORD:case EQTYPE_BLADE:	case EQTYPE_OTHER:
	case EQTYPE_HANDRING:	case EQTYPE_CLAW:	
	uiName.Format("MixShopGemColor%02d%02d", GEM_FOR_WEAPON, i);
	break;
	case EQTYPE_GEAR:	case EQTYPE_HAT_WITH_HAIR:	case EQTYPE_HAT_WITHOUT_HAIR:		
	uiName.Format("MixShopGemColor%02d%02d", GEM_FOR_HEAD_ASS, i);
	break;
	case EQTYPE_ARMOR:	case EQTYPE_CLOTH:	
	uiName.Format("MixShopGemColor%02d%02d", GEM_FOR_CLOTH, i);
	break;				
	case EQTYPE_BELT:
	uiName.Format("MixShopGemColor%02d%02d", GEM_FOR_BELT, i);
	break;
	case EQTYPE_RING:
	uiName.Format("MixShopGemColor%02d%02d", GEM_FOR_RING, i);
	break;
	case EQTYPE_SHOE:	
	uiName.Format("MixShopGemColor%02d%02d", GEM_FOR_SHOE, i);
	break;				
	}
	if (uiName.length() > 0)
	{
	pIm = gGlobal.pUIManager->FindUIImage(uiName.c_str());
	if (pIm)
	pIm->SetEnable(true);
	}

	uiName.Format("MixShopGemBG%02d", i);
	pIm = gGlobal.pUIManager->FindUIImage(uiName.c_str());
	if (pIm)
	pIm->SetEnable(true);

	if (m_pEqItem->GetInlay_gem(i) > 0)
	{
	uiName.Format("MixShopGemContain%02d", i);
	pIm = gGlobal.pUIManager->FindUIImage(uiName.c_str());
	if (pIm)
	pIm->SetEnable(true);

	const ItemData* pItemData = gGlobal.m_resource->GetItemData(m_pEqItem->GetInlay_gem(i));
	if (pItemData)
	{
	uiName.Format("MixShopGemItem%02d",i);
	pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiName.c_str());
	if (pAniIcon)
	{
	pAniIcon->ClearAniData();
	pAniIcon->SetAnimationData(String(pItemData->icon2).c_str());
	pAniIcon->SetEnable(true);
	}
	}
	}
	else
	{
	uiName.Format("MixShopGemEmpty%02d", i);
	pIm = gGlobal.pUIManager->FindUIImage(uiName.c_str());
	if (pIm)
	pIm->SetEnable(true);

	if ( m_mixShopGem[posOfGemList].itemSlots.size() > 0)
	{
	CItem* item = gGlobal.m_pBag->GetItem(m_mixShopGem[posOfGemList].itemSlots[0]);
	if (item && item->m_data)
	{
	uiName.Format("MixShopGemItem%02d",i);
	pAniIcon = gGlobal.pUIManager->FindUIAniIcon(uiName.c_str());
	if (pAniIcon)							
	{
	pAniIcon->ClearAniData();
	pAniIcon->SetAnimationData(String(item->m_data->icon2).c_str());
	pAniIcon->SetEnable(true);
	}
	}
	++posOfGemList;
	}
	}
	}
	else
	{
	uiName.Format("MixShopGemBlock%02d", i);
	pIm = gGlobal.pUIManager->FindUIImage(uiName.c_str());
	if (pIm)
	pIm->SetEnable(true);
	}
	}
	}*/
}

/*void CMenuMixProcess::ResetAllGemItem()
{
	for (Index i = C_INDEX(0); i < m_mixShopItem.size(); ++i)
		ClearItemMixSourceList(m_mixShopItem, i);
	
	for (Index i = C_INDEX(0); i < m_mixShopGem.size(); ++i)
		ClearItemMixSourceList(m_mixShopGem, i);

	m_pEqItem = NULL;
	
}*/

void CMenuMixProcess::ClearItemMixSourceList(ItemMixSourceList &list, UInt index)
{
	if(index < list.size())
	{
		for (Index j = C_INDEX(0); j < list[index].itemSlots.size(); ++j)
		{
			if (gGlobal.m_pBag && gGlobal.g_pItemProcess)
			{
				CItem* item = gGlobal.m_pBag->GetItem(list[index].itemSlots[j]);
				if (item)
				{
					item->SetGivenStack(0);
					int oldItembag = list[index].itemSlots[j] / BAG_ITEM_COUNT;

					if (oldItembag == gGlobal.g_pItemProcess->GetBagPage())
					{
						int oldIndex = list[index].itemSlots[j] % BAG_ITEM_COUNT;
						gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
					}
				}
			}
		}
		list[index].sourceRootID = 0;
		list[index].itemSwitch = 0;
		list[index].maxStack = 0;
		list[index].nowStack = 0;
		list[index].nowStacks.clear();
		list[index].itemSlots.clear();
		list[index].item_uids.clear();
	}
}

bool CMenuMixProcess::IsUnSoulBoundShop()
{
	if (m_pMixShop && m_pMixShop->type == MIX_UNSOULBOUND)
		return true;
	return false;
}

int CMenuMixProcess::GetUnSoulBoundStoneNum()
{
	int nRet(1);
	if (m_pEqItem && m_pEqItem->m_data)
	{
		float lv = (float)m_pEqItem->m_data->reqLv;
		CallLuaFunc(gGlobal.g_pParentLua, "GetUnSoulBoundStoneNum", String(), lv);
		nRet = (int)gGlobal.g_TempResult;
	}
	return nRet;
}

void CMenuMixProcess::DisappearMixShop()
{
	//for disappear
	if (m_iStartMixShopX >  0 || m_iStartMixShopY > 0)
	{
		BlockPos currPos = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();
		BlockPos center;
		center.PosX = m_iStartMixShopX;
		center.PosY = m_iStartMixShopY;
		PixelDirection dummy;

		if (QGLMath::CalculateBlockPosDistance(currPos, center, dummy) > 400)
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CancelMixShopMaking", String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CancelMixShopMixing", String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseMixShop", String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseMixShopMake", String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseIronShopMake", String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CancelMixShopGem", String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseMixShopGem", String());
		}
	}

}

bool CMenuMixProcess::IsIronShopMake()
{
	if (m_pMixShop && m_pMixShop->type == MIX_IRONMAKE)
		return true;
	return false;
}

bool CMenuMixProcess::IsIronShopRepair()
{
	if (m_pMixShop && m_pMixShop->type == MIX_IRONREPAIR)
		return true;
	return false;
}

bool CMenuMixProcess::IsGemMixShop()
{
	if (m_pMixShop && m_pMixShop->type == MIX_GEMMIX)
		return true;
	return false;
}

bool CMenuMixProcess::IsGemInlayShop()
{
	if (m_pMixShop && m_pMixShop->type == MIX_GEMINLAY)
		return true;
	return false;
}

bool CMenuMixProcess::IsGemRemoveShop()
{
	if (m_pMixShop && m_pMixShop->type == MIX_GEMREMOVE)
		return true;
	return false;
}

bool CMenuMixProcess::IsGemMeltingShop()
{
	if (m_pMixShop && m_pMixShop->type == MIX_GEMMELTING)
		return true;
	return false;
}

bool CMenuMixProcess::IsGemPolishingShop()
{
	if (m_pMixShop && m_pMixShop->type == MIX_GEMPOLISHING)
		return true;
	return false;
}

bool CMenuMixProcess::IsColorStoneMixShop()
{
	if (m_pMixShop && m_pMixShop->type == MIX_COLORSTONEMIX)
		return true;
	return false;
}

void CMenuMixProcess::SetGemRemoveIndex(int index)
{
	if (index == m_iGemRemoveSelectIndex)
		return ;

	if (m_mixShopItem.size() > 1 && m_mixShopItem[1].itemSlots.size() > 0)
		return ;

	if (m_mixShopItem.size() > 0 && m_mixShopItem[0].itemSlots.size() > 0)
	{
		CItem* item = gGlobal.m_pBag->GetItem(m_mixShopItem[0].itemSlots[0]);
		if (item && item->m_data)
		{
			// inlay gem
			UInt16 Inlay_gem = item->GetInlay_gem(index);
			if (Inlay_gem > 0)
			{
				m_iGemRemoveSelectIndex = index;
				m_GemRemoveSelectGemId = Inlay_gem;
				UpdateGem();
			}
		}
	}
}
