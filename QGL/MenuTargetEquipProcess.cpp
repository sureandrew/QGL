//-- Common
#include "Common.h"
#include "QGL.h"
#include "Common/common_binary.h"
//-- Self
#include "MenuTargetEquipProcess.h"
#include "MenuTargetProcess.h"
#include "MenuItemProcess.h"
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


CMenuTargetEquipProcess::CMenuTargetEquipProcess()
{
	m_items = SafeAllocate(CItem*, BAG_EQUIP_COUNT);
	TRACE_ENSURE(m_items);							// ensure allocate successful
	ZeroMemory(m_items, BAG_EQUIP_COUNT * sizeof(CItem*));
	m_char_id = 0;
	m_EquipItemSetInfoMap.clear();
}

CMenuTargetEquipProcess::~CMenuTargetEquipProcess()
{
	FreeTargetEquip();

	SafeDeallocate(m_items);
}

void CMenuTargetEquipProcess::UpdateTargetEquip()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CMenuTargetEquipProcess::UpdateTargetEquip()."));

	String Text;
	CItem* item;
	StringA componentName, imageFilename;
	UIAniIcon* pAniIcon;
	
	for (Index i = C_INDEX(0); i < C_INDEX(BAG_EQUIP_COUNT); ++i)
	{
		componentName.Format("TargetEquip%03iIcon", i );
		pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(componentName.c_str());

		for (Index j = C_INDEX(1); j < 6; ++j)
		{
			componentName.Format("TargetEquip%03iColor%d", i, j);
			SetUIEnable(componentName, false);
		}

		if( pAniIcon)
		{
			item = m_items[i];
			if( item && item->m_data)
			{
				const ItemData* pItemData = item->m_data;
				if( pItemData )
				{
					componentName.Format("TargetEquip%03iStack", i);
					if( item->GetStack() > 1 )
						Text.Format(_T("%i"), item->GetStack()); 
					else
						Text = "";
					gGlobal.SetUIStaticText(componentName, Text);
					componentName.Format("TargetEquip%03iLvl", i);
					if( item->GetGrade() > 0 )
						Text.Format(_T("+%i"), item->GetGrade()); 
					else
						Text = "";
					gGlobal.SetUIStaticText(componentName, Text);

					String temp(pItemData->icon2);
								
					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(temp.c_str());
					pAniIcon->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
				}
			}
			else
				SetIconOff(i);
		}
	}
}

void CMenuTargetEquipProcess::SetIconOff(int iconPos)
{
	String text;
	text.Empty();
	StringA componentName;
	UIAniIcon* pAniIcon = NULL;

	if (iconPos >=0 && iconPos < 9)
	{
		componentName.Format("TargetEquip%03iIcon", iconPos );
		pAniIcon = gGlobal.pUIManager->FindUIAniIcon(componentName.c_str());
		if (pAniIcon)
		{
			componentName.Format("TargetEquip%03iStack", iconPos);				
			SetItemText(componentName, text, 0, true);
			componentName.Format("TargetEquip%03iLvl", iconPos);
			SetItemText(componentName, text, 0, true);
			pAniIcon->SetEnable(false);
		}
	}
}

void CMenuTargetEquipProcess::SetItemText(StringA componentName, String text, Color32 textColor, bool addDirty)
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

void CMenuTargetEquipProcess::RequestTargetEquip()
{
	if(gGlobal.g_MenuTargetProcess)
	{
		if (gGlobal.g_MenuTargetProcess->m_char_id == gGlobal.g_pCharacter->GetChar_id())
		{
			return ; // do not get self equip.
		}

		m_char_id = gGlobal.g_MenuTargetProcess->m_char_id;
		m_char_name = gGlobal.g_MenuTargetProcess->m_char_name;

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CMenuTargetEquipProcess::RequestTargetEquip(). char_id = %d, CharName = %s."), m_char_id, m_char_name.c_str());

		gGlobal.g_pCharacter->GetCharNetObject()->RequestTargetEquip(gGlobal.pPeerProxy, gGlobal.g_context, m_char_id);
	}
}

void CMenuTargetEquipProcess::TargetEquipReply(NetGroup * itemGroup)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CMenuTargetEquipProcess::TargetEquipReply(). count = %d"), itemGroup->GetCount());

	// open panel
	UIPanel* pTargetEquipPanel = gGlobal.pUIManager->FindUIPanel("TargetEquipPanel");
	if (!pTargetEquipPanel)
	{
		XML_UI::GetInstance().Init();
		XML_UI::GetInstance().ReadFormFile("Data\\MenuTargetEquip.xml", gGlobal.pUIManager);
		pTargetEquipPanel = gGlobal.pUIManager->FindUIPanel("TargetEquipPanel");
		if (pTargetEquipPanel)
		{
			pTargetEquipPanel->SetPos(gGlobal.pUIManager->GetCursorX(), gGlobal.pUIManager->GetCursorY() - 120);
			gGlobal.pUIManager->AddDirtyUIItem(pTargetEquipPanel);

			UIStaticText * pTextName = gGlobal.pUIManager->FindUIStaticText("TargetEquipPanelName");
			if(pTextName)
			{
				pTextName->SetText(m_char_name);
				pTextName->Redraw();
			}
		}
		else
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F(" can not open panel."));
		}

		for (UInt16 i = 0; i < itemGroup->GetCount(); i++)
		{
			CItem* item = itemGroup->GetChild<CItem>(i);
			if (item == NULL || item->GetBagIndex() >= BAG_MAX_SIZE)
				continue;

			TRACE_INFODTL_4(GLOBAL_LOGGER, _F(" TargetEquip: bagIndex %d, item_uid %d, item_id %d, stack %d"), 
				item->GetBagIndex(), item->GetItem_uid(), item->GetItem_id(), item->GetStack());

			UInt8 bagIndex = item->GetBagIndex() - 120;
			if(bagIndex >= 0 && bagIndex < BAG_EQUIP_COUNT)
				m_items[bagIndex] = item;
		}

		UpdateItemSet();

		UpdateTargetEquip();
	}
}

void CMenuTargetEquipProcess::FreeTargetEquip()
{
	m_EquipItemSetInfoMap.clear();
	for (Index i = C_INDEX(0); i < C_INDEX(9); ++i)
	{
		if (m_items[i])
		{
			DeleteNetObject(m_items[i]);
		}
	}
}

void CMenuTargetEquipProcess::UpdateTargetEquipDetail(int bagIndex)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CMenuTargetEquipProcess::UpdateTargetEquipDetail(). bagIndex = %d"), bagIndex);

	if(bagIndex >= 0 && bagIndex < BAG_EQUIP_COUNT)
	{
		CItem * item = m_items[bagIndex];
		if (item)
		{
			gGlobal.g_pItemProcess->ItemDetailPopUp(item->GetItem_id(), false, item);	
		}
	}
}

bool CMenuTargetEquipProcess::HasTargetEquip(int bagIndex)
{
	if(bagIndex >= 0 && bagIndex < BAG_EQUIP_COUNT)
	{
		CItem * item = m_items[bagIndex];
		if (item)
			return true;
	}

	return false;
}


void CMenuTargetEquipProcess::UpdateItemSet()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CMenuTargetEquipProcess::UpdateItemSet()."));

	m_EquipItemSetInfoMap.clear();

	for (UInt i = 0; i < BAG_EQUIP_COUNT; i++)
	{
		CItem *item = m_items[i];
		if (item && item->GetEndure() > 0 && item->m_data)
		{
			UInt32 itemset_id = item->m_data->itemset_id; // if item has itemset ...
			if (itemset_id > 0)
			{
				CMenuTargetEquipProcess::EquipItemSetInfoMap::iterator itr = m_EquipItemSetInfoMap.find(itemset_id);
				if (itr == m_EquipItemSetInfoMap.end())
				{
					EquipItemSetInfo itemsetinfo;
					itemsetinfo.itemset_id = itemset_id;
					itemsetinfo.eq_count = 1;
					itemsetinfo.pItemSetData = gGlobal.m_resource->GetItemSetData(itemset_id);
					if (!itemsetinfo.pItemSetData)
					{
						TRACE_INFODTL(GLOBAL_LOGGER, _F("UpdateItemSet(), can not get itemsetdata."));
					}
					m_EquipItemSetInfoMap.insert(std::make_pair(itemset_id, itemsetinfo));
				}
				else
				{
					(itr)->second.eq_count ++;
				}
			}
		}
	}
}

EquipItemSetInfo* CMenuTargetEquipProcess::GetItemSetInfo(UInt32 itemset_id)
{
	CMenuTargetEquipProcess::EquipItemSetInfoMap::iterator itr = m_EquipItemSetInfoMap.find(itemset_id);
	if (itr != m_EquipItemSetInfoMap.end())
		return &(itr->second);
	return NULL;
}