//-- Common
#include "Common.h"
#include "QGL.h"
#include "Common/Procedure.h"
//-- Self
#include "MenuItemProcess.h"
//-- Library
#include "Global.h"
#include "Proxy/Service/MapService.h"
#include "Configure.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "Resource/ResItem.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/Party.h"
#include "Resource/ClientResManager.h"
#include "CharacterControl.h"
#include "MenuShopProcess.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "ScriptControl.h"
#include "Resource/ResBuff.h"
#include "MenuQuestLogProcess.h"
#include "MenuSystemSetting.h"
#include "Proxy/Logic/Pet.h"
#include "Proxy/Logic/PetGroup.h"
#include "Resource/ResPet.h"
#include "MenuHawkerProcess.h"
#include "QGLApplication.h"
#include "Resource/ResSkill.h"
#include "Resource/ResItemSet.h"
#include "MenuTargetEquipProcess.h"
#include "MenuMixProcess.h"
#include "MenuPetProcess.h"


#define ITEMDETAIL_UPPERSPACE 18
#define ITEMDETAIL_LOWERSPACE 16

#define  ITEM_MIXSHOP_TYPE_INLAY 1

void  CMenuItemProcess::UpdateShortCutSlot(int itemPos)
{
	VMProtectBegin; VM_START;

	UInt32	MaxItem = 0;
	CItem* pItem = gGlobal.m_pBag->GetItem(itemPos);	
		
	if(pItem)
		MaxItem = GetMaxStack(itemPos);
	
	for(Index i = C_INDEX(0); i < 10; ++i)
	{
		if(gGlobal.g_ShortCutSlot[i].m_RefNum == itemPos && gGlobal.g_ShortCutSlot[i].m_RefType == 1)
		{
			if(MaxItem == 0)
				ClearShortCutSlot(i);
			else
			{
				StringA num;

				num.Format("HShortText%d", i);					
				UIStaticText *pStaticText = gGlobal.pUIManager->FindUIStaticText(num.c_str());
				if(MaxItem > 1)
				{
					String num;

					num.Format(_T("%d"), MaxItem);					
					pStaticText->SetText(num.c_str());
					pStaticText->Redraw();
					pStaticText->SetEnable(true);
				}
				else
				{
					pStaticText->ClearText();
					pStaticText->Redraw();
					pStaticText->SetEnable(false);
				}
				pStaticText->RenderParent();
				gGlobal.pUIManager->AddDirtyUIItem(pStaticText);
			}
			return;
		}
	}
	VMProtectEnd; VM_END;
}

void CMenuItemProcess::WriteToShortCut(int itemPos, int targetPos, bool NetUpdate)
{
	StringA num;
	num.Format("HShortCut%d", targetPos);					
	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(num.c_str());

	CItem* pItem = gGlobal.m_pBag->GetItem(itemPos);		
	if(pItem && pItem->m_data)
	{
		const ItemData* pItemData = pItem->m_data;
		if(pItemData)
		{
			UInt32	MaxItem = GetMaxStack(itemPos);
			num.Format("HShortText%d", targetPos);					
			UIStaticText *pStaticText = gGlobal.pUIManager->FindUIStaticText(num.c_str());

			switch(pItemData->item_type)
			{
			case ITEMTYPE_FOOD:
			case ITEMTYPE_DRUG:
			case ITEMTYPE_WINE:
			case ITEMTYPE_FIREWORKS:
				String temp(pItemData->icon2);
								
				temp = temp.substr(0, temp.size() - 5);
				temp += String("t.ent");
				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(temp.c_str());
				pAniIcon->SetEnable(true);

				String	ShowName;
				ShowName.Format(_T("#W%s\\r#G%s"), pItemData->name, pItemData->functionDes); 
				pAniIcon->SetTips(ShowName.c_str());
				
				if (gGlobal.g_ShortCutSlot[targetPos].m_RefType == 1 && 
					gGlobal.g_ShortCutSlot[targetPos].m_RefNum == itemPos)
				{
					NetUpdate = false;
				}
				else
				{
					gGlobal.g_ShortCutSlot[targetPos].m_RefType = 1;
					gGlobal.g_ShortCutSlot[targetPos].m_RefNum = itemPos;
				}

				CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
				if (pchar && NetUpdate)
				{	
					RPCContext context;
					pchar->UpdateShortCut(gGlobal.pPeerProxy, context, itemPos, 1, targetPos);
				}

				if(MaxItem > 1)
				{
					String num;
					num.Format(_T("%d"), MaxItem);					
					pStaticText->SetText(num.c_str());
					pStaticText->Redraw();
					pStaticText->SetEnable(true);
				}
				else
				{
					pStaticText->ClearText();
					pStaticText->SetEnable(false);
				}

				gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
			}
		}
	}
}

void  CMenuItemProcess::CopyToShortCut(int itemPos, int targetPos)
{
	VMProtectBegin; VM_START;	
	if( gGlobal.m_pBag && gGlobal.m_resource && itemPos<24 && itemPos>=0 &&
		!(gGlobal.g_ShortCutSlot[targetPos].m_RefType == 1 &&
		gGlobal.g_ShortCutSlot[targetPos].m_RefNum == itemPos))
	{
		StringA num;
		Int32 OldSlot = -1;
		UIShortCutNode		OldNode;

		memcpy(&OldNode, gGlobal.g_ShortCutSlot + targetPos, sizeof(UIShortCutNode));

		for(Index i = C_INDEX(0); i < 10; ++i)
		{
			if(gGlobal.g_ShortCutSlot[i].m_RefNum == itemPos && gGlobal.g_ShortCutSlot[i].m_RefType == 1)
			{
				if(OldSlot == -1)
					OldSlot = (Int32)i;
				ClearShortCutSlot(i);
			}
		}
				
		WriteToShortCut(itemPos, targetPos);

		if(OldNode.m_RefType == 1 && OldNode.m_RefNum >= 0 && OldNode.m_RefNum < 24)
			WriteToShortCut(OldNode.m_RefNum, OldSlot);
	}
	VMProtectEnd; VM_END;
}

void CMenuItemProcess::MoveEquipToBag(int itemPos, int targetPos)
{
	if( gGlobal.m_pBag && gGlobal.m_resource && itemPos<129 && itemPos>=0 )
	{
		CItem* pItem = gGlobal.m_pBag->GetItem(itemPos);
		if( pItem )
		{
			if (!gGlobal.m_pBag->HasItem(targetPos))
			{				
				StringA num;
			
				num.Format("ItemEquip00%dIcon",itemPos - 120);					
				UIAniIcon* pImTemp = gGlobal.pUIManager->FindUIAniIcon(num.c_str());
				if (pImTemp)
					pImTemp->SetEnable(false);
				
				int subIndex = itemPos - 120;
				switch(subIndex)
				{
				case 0:
					SetSoundIndex(ITEM_OFF_WP);
					break;
				case 1:	case 2:	case 3:
				case 5:	case 7:
					SetSoundIndex(ITEM_OFF_CLOTH);
					break;
				case 4:	case 6:	case 8:
					SetSoundIndex(ITEM_OFF_OTHER);
					break;
				}
				gGlobal.m_pBag->EquipItem(gGlobal.pPeerProxy, gGlobal.g_context, itemPos, targetPos);				
			}
			else
			{
				ShowMessage("MSE_SYS_UNEQUIP_ERR");
				UpdateItemMenu();
			}
		}
	}
}

void CMenuItemProcess::MoveToBag(int itemPos, int bag, int target)
{
	if( gGlobal.m_pBag && gGlobal.m_resource && itemPos<129 && itemPos>=0 )
	{
		CItem* pItem = gGlobal.m_pBag->GetItem(itemPos);
		if( pItem )
		{
			if (target < 0)
			{
				UInt8 bagIndex=0;
				UInt8 slot = itemPos;
				if ((bagIndex = gGlobal.m_pBag->FindEmptyIndex(bag)) == 255)
				{
					if (m_iBag >= 0)
						 slot -= m_iBag * BAG_ITEM_COUNT;
					if (slot < BAG_ITEM_COUNT)
						UpdateItemSlot(slot, true);
					return;
				}
				gGlobal.m_pBag->MoveItem(gGlobal.pPeerProxy, gGlobal.g_context, itemPos, bagIndex);
			} 
			else
			{
				UInt8 bagIndex = target + bag * BAG_ITEM_COUNT;
				StringA num;
				num.Format("ItemSlot%03dIcon", target);
				
				UIAniIcon* pImTemp = gGlobal.pUIManager->FindUIAniIcon(num.c_str());
				if (pImTemp)
					pImTemp->SetEnable(false);
			
				gGlobal.m_pBag->MoveItem(gGlobal.pPeerProxy, gGlobal.g_context, itemPos, bagIndex);
			}
			SetSoundIndex(ITEM_PUT);
		}
	}
}

void CMenuItemProcess::DropItem(int itemPos, int dropSize)
{
	VMProtectBegin; VM_START;

	if( gGlobal.m_pBag && gGlobal.m_resource && itemPos < 129 && itemPos>=0 )
	{
		CItem* pItem = gGlobal.m_pBag->GetItem(itemPos);
		if( pItem )
		{
			 if  (pItem->m_data && (pItem->m_data->dumpable == 0 || pItem->GetLock()))// || pItem->GetItem_soulboundState() )
			{
				
					ShowMessage("MSG_DROP_ITEM_BOUND");
				UInt8 index = itemPos % BAG_ITEM_COUNT;
				UpdateItemSlot(index, true);
			}
			else
			{
				gGlobal.m_pBag->DropItem(gGlobal.pPeerProxy, gGlobal.g_context, itemPos, dropSize);
				if(gGlobal.g_pMenuPetProcess && gGlobal.g_pMenuPetProcess->GetUseFoodMsg())
				{
					m_Text = "MSG_PET_USE_FOOD_OK";
					gGlobal.g_pMenuPetProcess->SetUseFoodMsg(false);
				}
				else
					m_Text = "MSG_SYS_ITEM_DROP";
				SetSoundIndex(ITEM_DROP);
			}
		}
	}

	VMProtectEnd; VM_END;
}

void CMenuItemProcess::EquipItemUse(int itemPos)
{
	VMProtectBegin; VM_START;

	UIPanel* givenPanel = gGlobal.pUIManager->FindUIPanel("GivenItemPanel");
	if (givenPanel)
	{
		ShowMessage("MSG_ITEM_GIVEN_CANT_USE");
		return;
	}

	UIPanel* tradingPanel = gGlobal.pUIManager->FindUIPanel("TradingPanel");
	if (tradingPanel)
	{
		ShowMessage("MSG_ITEM_TRADING_CANT_USE");
		return;
	}

	UIPanel* hawkerPanel = gGlobal.pUIManager->FindUIPanel("HawkerOwnerPanel");
	UIPanel* hawkerPanel1 = gGlobal.pUIManager->FindUIPanel("HawkPanel");
	if (hawkerPanel || hawkerPanel1 || gGlobal.g_pHawkerProcess->IsStartHawk() )
	{
		ShowMessage("MSG_HAWK_CANT_USE_ITEM");
		return;
	}

	VMProtectEnd; VM_END;

	ENCODE_START;

	if( gGlobal.m_pBag && gGlobal.m_resource && itemPos<129 && itemPos>=0 )
	{
		CItem* pItem = gGlobal.m_pBag->GetItem(itemPos);
		if( pItem && pItem->m_data )
		{
			const ItemData* pItemData = pItem->m_data;
			if( pItemData )
			{
				UIAniIcon* eqItemPanel = NULL;
				UIPanel* itemMenu = (UIPanel*)gGlobal.pUIManager->GetUIItem("ItemMenuPanel");
				UIImage* cover = (UIImage*)gGlobal.pUIManager->GetUIItem("ItemEquipPanel");
				Int32 relativeX = gGlobal.pUIManager->GetCursorX() - cover->GetX();
				Int32 relativeY = gGlobal.pUIManager->GetCursorY() - cover->GetY();

				if ((relativeX < 0) || (relativeY < 0 ))
				{
					NewUpdateItemMenu();
					return;
				}

				int index = relativeX / 48 + relativeY / 48 * 3;

				switch( pItemData->item_type )
				{
				case ITEMTYPE_EQUIP:
					switch( pItemData->eqType )
					{
					case EQTYPE_SPEAR:
					case EQTYPE_WIPE:
					case EQTYPE_FAN:
					case EQTYPE_PEN:
					case EQTYPE_SWORD:
					case EQTYPE_DBSWORD:
					case EQTYPE_BLADE:
					case EQTYPE_OTHER:
					case EQTYPE_HANDRING:
					case EQTYPE_CLAW:
						{
							eqItemPanel = gGlobal.pUIManager->FindUIAniIcon("ItemEquip000Icon");
							if (eqItemPanel == NULL)
								return;
							
							if (index == 3)
								SetSoundIndex(ITEM_EQUIP_WP);
							else 
							{
								if (itemPos < 120)
									NewUpdateItemMenu();
								else
									UpdateItemEq();
								ShowMessage("MSE_SYS_EQUIP_ERR");
								return;
							}
						}
						break;
					case EQTYPE_GEAR:
						{
							eqItemPanel = gGlobal.pUIManager->FindUIAniIcon("ItemEquip001Icon");
							
							if (eqItemPanel == NULL)
								return;

							if (index == 1)
								SetSoundIndex(ITEM_EQUIP_CLOTH);
							else 
							{
								if (itemPos < 120)
									NewUpdateItemMenu();
								else
									UpdateItemEq();
								ShowMessage("MSE_SYS_EQUIP_ERR");
								return;
							}
						}						
						break;
					case EQTYPE_ARMOR:
						{
							eqItemPanel = gGlobal.pUIManager->FindUIAniIcon("ItemEquip002Icon");
						
							if (eqItemPanel == NULL)
								return;

							if (index == 4)
								SetSoundIndex(ITEM_EQUIP_CLOTH);
							else 
							{
								if (itemPos < 120)
									NewUpdateItemMenu();
								else
									UpdateItemEq();
								ShowMessage("MSE_SYS_EQUIP_ERR");
								return;
							}
						}						
						break;
					case EQTYPE_SHOE:
						{
							eqItemPanel = gGlobal.pUIManager->FindUIAniIcon("ItemEquip003Icon");
						
							if (eqItemPanel == NULL)
								return;

							if (index == 7)
								SetSoundIndex(ITEM_EQUIP_CLOTH);
							else 
							{
								if (itemPos < 120)
									NewUpdateItemMenu();
								else
									UpdateItemEq();
								ShowMessage("MSE_SYS_EQUIP_ERR");
								return;
							}
						}			
						break;
					case EQTYPE_BELT:
						{
							eqItemPanel = gGlobal.pUIManager->FindUIAniIcon("ItemEquip004Icon");
							
							if (eqItemPanel == NULL)
								return;

							if (index == 8)
								SetSoundIndex(ITEM_EQUIP_CLOTH);
							else 
							{
								if (itemPos < 120)
									NewUpdateItemMenu();
								else
									UpdateItemEq();
								ShowMessage("MSE_SYS_EQUIP_ERR");
								return;
							}
						}
						
						break;
					case EQTYPE_RING:
						{
							eqItemPanel = gGlobal.pUIManager->FindUIAniIcon("ItemEquip005Icon");
							
							if (eqItemPanel == NULL)
								return;

							if (index == 6)
								SetSoundIndex(ITEM_EQUIP_OTHER);							
							else 
							{
								if (itemPos < 120)
									NewUpdateItemMenu();
								else
									UpdateItemEq();
								ShowMessage("MSE_SYS_EQUIP_ERR");
								return;
							}
						}
						
						break;
					case EQTYPE_AMULET:
						{
							eqItemPanel = gGlobal.pUIManager->FindUIAniIcon("ItemEquip006Icon");
							
							if (eqItemPanel == NULL)
								return;
			
							if (index == 0)
								SetSoundIndex(ITEM_EQUIP_OTHER);
							else
							{
								if (itemPos < 120)
									NewUpdateItemMenu();
								else
									UpdateItemEq();
								ShowMessage("MSE_SYS_EQUIP_ERR");
								return;
							}
						}						
						break;
						
					case EQTYPE_CLOTH:
						{
							eqItemPanel = gGlobal.pUIManager->FindUIAniIcon("ItemEquip008Icon");
							
							if (eqItemPanel == NULL)
								return;

							if (index == 5)
								SetSoundIndex(ITEM_EQUIP_CLOTH);
							else 
							{
								if (itemPos < 120)
									NewUpdateItemMenu();
								else
									UpdateItemEq();
								ShowMessage("MSE_SYS_EQUIP_ERR");
								return;
							}
						}					
						break;	
					case EQTYPE_HAT_WITH_HAIR:
					case EQTYPE_HAT_WITHOUT_HAIR:
						{
							eqItemPanel = gGlobal.pUIManager->FindUIAniIcon("ItemEquip007Icon");
							if (eqItemPanel == NULL)
								return;
							
							if (index == 2)			
								SetSoundIndex(ITEM_EQUIP_OTHER);
							else 
							{
								if (itemPos < 120)
									NewUpdateItemMenu();
								else
									UpdateItemEq();
								ShowMessage("MSE_SYS_EQUIP_ERR");
								return;
							}
						}
						break;
					}
					break;
				default:
					{
						if (itemPos < 120)
							NewUpdateItemMenu();
						else
							UpdateItemEq();
						ShowMessage("MSE_SYS_EQUIP_ERR");
						return;
					}
					break;								
				}
				if (eqItemPanel)
				{
						eqItemPanel->SetEnable(false);
						gGlobal.pUIManager->AddDirtyUIItem(eqItemPanel);
				}
				gGlobal.m_pBag->EquipItem(gGlobal.pPeerProxy, gGlobal.g_context, itemPos, 255);
			}
		}
	}

	ENCODE_END;
}

void CMenuItemProcess::ItemUse(int itemPos)
{
	VMProtectBegin; VM_START;

	UIPanel* givenPanel = gGlobal.pUIManager->FindUIPanel("GivenItemPanel");
	if (givenPanel)
	{
		ShowMessage("MSG_ITEM_GIVEN_CANT_USE");
		return;
	}
	UIPanel* tradingPanel = gGlobal.pUIManager->FindUIPanel("TradingPanel");
	if (tradingPanel)
	{
		ShowMessage("MSG_ITEM_TRADING_CANT_USE");
		return;
	}

	UIPanel* hawkerPanel = gGlobal.pUIManager->FindUIPanel("HawkerOwnerPanel");
	UIPanel* hawkerPanel1 = gGlobal.pUIManager->FindUIPanel("HawkPanel");
	if (hawkerPanel || hawkerPanel1 || gGlobal.g_pHawkerProcess->IsStartHawk())
	{
		ShowMessage("MSG_HAWK_CANT_USE_ITEM");
		return;
	}

	VMProtectEnd; VM_END;

	ENCODE_START;

	if( gGlobal.m_pBag && gGlobal.m_resource && itemPos<129 && itemPos>=0 )
	{
		CItem* pItem = gGlobal.m_pBag->GetItem(itemPos);
		if( pItem )
		{
			if (pItem->GetLock())
			{
				ShowMessage("MSG_ITEM_LOCK_CANT_USE");	
				return;
			}
			StringA iconName;
			StringA iconStock;
			StringA iconLevel;
			if (itemPos < 120)
			{
				int subIndex = itemPos % BAG_ITEM_COUNT;
				iconName.Format("ItemSlot%03dIcon", subIndex);
				iconStock.Format("ItemSlot%03dStack", subIndex);
				iconLevel.Format("ItemSlot%03dLvl", subIndex);
			}
			else
			{
				int subIndex = itemPos - 120;
				iconName.Format("ItemEquip00%dIcon", subIndex);
				iconStock.Format("ItemEquip00%dStack", subIndex);
				iconLevel.Format("ItemEquip00%dLvl", subIndex);
			}

			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
			if (pAniIcon)
			{
				pAniIcon->SetEnable(false);
				gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
			}
			
			UIImage* itemboard = (UIImage*)gGlobal.pUIManager->GetUIItem("ItemBoard");
			if (itemboard)
			{
				itemboard->SetEnable(false);
				gGlobal.pUIManager->AddDirtyUIItem(itemboard);
			}
		
			const ItemData* pItemData = pItem->m_data;
			if( pItemData )
			{
				if (gGlobal.g_pCharacter->CheckBlockItem(pItemData->item_type))
				{
					String tempText;
					if (gGlobal.g_pCharacter->m_tempBuffData)
						tempText.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_ITEM_BLOCK")), gGlobal.g_pCharacter->m_tempBuffData->name);
					else
						tempText.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_ITEM_BLOCK")), _T(""));
					gGlobal.PrintMessage(tempText, CHANNELTYPE_SYSTEM);
					if (pAniIcon)
					{
						pAniIcon->SetEnable(true);
						gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
					}
					return;
				}

				switch( pItemData->item_type )
				{
				case ITEMTYPE_EQUIP:
					switch( pItemData->eqType )
					{
					case EQTYPE_SPEAR:	case EQTYPE_WIPE:	case EQTYPE_FAN:
					case EQTYPE_PEN:	case EQTYPE_SWORD:	case EQTYPE_DBSWORD:
					case EQTYPE_BLADE:	case EQTYPE_OTHER:	case EQTYPE_HANDRING:
					case EQTYPE_CLAW:
						if (itemPos < 120)
							SetSoundIndex(ITEM_EQUIP_WP);
						else
							SetSoundIndex(ITEM_OFF_WP);	
						break;
					case EQTYPE_GEAR:	case EQTYPE_SHOE:	case EQTYPE_ARMOR:
					case EQTYPE_CLOTH:	case EQTYPE_BELT:	case EQTYPE_HAT_WITH_HAIR:
					case EQTYPE_HAT_WITHOUT_HAIR:
						if (itemPos < 120)
							SetSoundIndex(ITEM_EQUIP_CLOTH);
						else
							SetSoundIndex(ITEM_OFF_CLOTH);
						break;					
					case EQTYPE_RING:
					case EQTYPE_AMULET:
						if (itemPos < 120)
							SetSoundIndex(ITEM_EQUIP_OTHER);
						else
							SetSoundIndex(ITEM_OFF_OTHER);
						break;
					
					}
					if (m_iMenuIndex == ITEM_CHAR_MENU)
						gGlobal.m_pBag->EquipItem(gGlobal.pPeerProxy, gGlobal.g_context, itemPos, 255);
					else
					{
						SetSoundIndex(ITEM_CANT_USE_OR_EQUIP);
						if (pAniIcon)
						{
							pAniIcon->SetEnable(true);
							gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
						}
						ShowMessage("MSE_SYS_EQUIP_PCANT_USE");
					}
					break;
				default:
					{
						SetSoundIndex(ITEM_USE);
						m_Text = "MSG_SYS_ITEM_USED";
						if ( pItemData->consume == 0 )
						{
							if (pAniIcon)
							{
								pAniIcon->SetEnable(true);
								gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
							}
							ShowMessage("MSG_SYS_ITEM_CANT_USE");
							SetSoundIndex(-1);
							m_Text = "";
						}
						else if ( pItemData->consume == 1 || pItemData->consume == 2 )
						{
							if ( pItemData->script && (_tcsicmp(pItemData->script, _T("")) != 0) )
							{
								if (pAniIcon)
								{
									pAniIcon->SetEnable(true);
									gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
								}

								bool bCanRun(true);
								if ( pItemData->memberUse == 0 && gGlobal.g_pParty && !(gGlobal.g_pParty->IsLeader()))
								{
									bCanRun = false;
									ShowMessage("MSG_SYS_ITEM_MEMBERCANNOTUSE");
								}

								if ( bCanRun )
								{
									// 0:anyone , 1:myself, 2:partner, 3:pet, 4:my team
									m_Text = "";
									UInt32 partnerID = 0;
									UInt32 petID = 0;
									bCanRun = false;

									if (m_iMenuIndex == ITEM_CHAR_MENU && 
										( pItemData->targetType == 1 || pItemData->targetType == 0 || pItemData->targetType == 4 ) )
										bCanRun = true;
									else if (m_iMenuIndex == ITEM_PARTNER_MENU && 
										( pItemData->targetType == 2 || pItemData->targetType == 0 || pItemData->targetType == 4 ) )
									{
										CPartner* partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(m_iSelectedPartnerIndex);
										if (partner)
										{
											bCanRun = true;
											partnerID = partner->GetPartner_id();
										}	
									}
									else if (m_iMenuIndex == ITEM_PET_MENU && 
										( pItemData->targetType == 3 || pItemData->targetType == 0 || pItemData->targetType == 4 ) )
									{
										CPet* pet = gGlobal.g_PetLst->GetPetByIndex(m_iSelectedPetIndex);
										if (pet)												
										{
											bCanRun = true;
											petID = pet->GetPet_uid();
										}
									}

									if ( bCanRun && gGlobal.g_pScriptControl->PrepareItemScriptHandle() )
									{
										gGlobal.m_pBag->UseItem(gGlobal.pPeerProxy, gGlobal.g_context, itemPos, pItemData->item_id, BOOLEAN_TRUE, partnerID, petID);
									}
									else
									{
										SetSoundIndex(ITEM_CANT_USE_OR_EQUIP);
										ShowMessage("MSG_SYS_ITEM_CANT_USE");
									}
								}		
								else
								{
									SetSoundIndex(-1);
									m_Text = "";
								}
							}
							else
							{// use item without script 
								if (m_iMenuIndex == ITEM_CHAR_MENU)
									gGlobal.m_pBag->UseItem(gGlobal.pPeerProxy, gGlobal.g_context, itemPos, pItemData->item_id, BOOLEAN_FALSE, 0, 0);
								else if (m_iMenuIndex == ITEM_PARTNER_MENU)
								{
									CPartner* partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(m_iSelectedPartnerIndex);
									if (partner)
										gGlobal.m_pBag->UseItem(gGlobal.pPeerProxy, gGlobal.g_context, itemPos, pItemData->item_id, BOOLEAN_FALSE, partner->GetPartner_id(), 0);
									else
									{
										SetSoundIndex(-1);
										if (pAniIcon)
										{
											pAniIcon->SetEnable(true);
											gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
										}
										m_Text = "";
									}
								}
								else if (m_iMenuIndex == ITEM_PET_MENU)
								{
									CPet* pet = gGlobal.g_PetLst->GetPetByIndex(m_iSelectedPetIndex);
									if (pet)
										gGlobal.m_pBag->UseItem(gGlobal.pPeerProxy, gGlobal.g_context, itemPos, pItemData->item_id, BOOLEAN_FALSE, 0, pet->GetPet_uid());
									else
									{
										SetSoundIndex(-1);
										if (pAniIcon)
										{
											pAniIcon->SetEnable(true);
											gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
										}
										m_Text = "";
									}
								}
							}
						}	
						else
						{
							SetSoundIndex(-1);
							
							if (pAniIcon)
							{
								pAniIcon->SetEnable(true);
								gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
							}
							m_Text = "";
						}
					}	
					break;
				}
			}
		}
	}

	ENCODE_END;
}

void CMenuItemProcess::ShowMessage(StringA message)
{
	if (message != "")
	{
		String tempStr( gGlobal.GetStringTable()->Get(String(message.c_str()).c_str()));
		String empty;
		if (!tempStr.Equals(empty.c_str()))
			gGlobal.PrintMessage(tempStr, CHANNELTYPE_SYSTEM);
	}
	else if (m_Text != "")
	{
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(String(m_Text.c_str()).c_str()), CHANNELTYPE_SYSTEM);
	}
	m_Text = "";	
}

void CMenuItemProcess::UpdateItemMenu(int bag)
{
	if( gConfigure.uNetwork )
	{
		NewUpdateItemMenu(bag);
		UpdateItemEq();
		PlaySound();

		if ( gGlobal.g_pQuestLogProcess )
			gGlobal.g_pQuestLogProcess->RefreshTargetItemCount();

		UIButton* itemButton = (UIButton*)gGlobal.pUIManager->GetUIItem("DownBItem");
		if (itemButton)
		{
			String strTips;
			UInt16 uCurrentLeavings = BAG_ITEM_COUNT - gGlobal.m_pBag->GetCount(BAG_CURRENT);
			UInt16 uTotalLeavings(0);
			for (UInt8 i=0; i<BAG_TYPE_COUNT; i++)
			{
				if (gGlobal.m_pBag->IsBagEnabled(i))
				{
					uTotalLeavings += (BAG_ITEM_COUNT - gGlobal.m_pBag->GetCount(i));
				}
			}
			strTips.Format(gGlobal.GetStringTable()->Get(_T("MSG_ITEMBAG_TIPS")),uCurrentLeavings, uTotalLeavings);
			if (uCurrentLeavings == 0)
			{
				strTips = _T("#R") + strTips;
			}
			else if (uCurrentLeavings > 5)
			{
				strTips = _T("#G") + strTips;
			}
			else
			{
				strTips = _T("#Y") + strTips;
			}
			itemButton->SetTips(strTips.c_str());
		}
	}
}

void CMenuItemProcess::UpdateItemEq()
{
	if( gGlobal.m_pBag && gGlobal.m_resource && m_iMenuIndex == ITEM_CHAR_MENU)
	{
		UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("ItemMenuPanel");
		if (panel)
		{
			String Text;
			CItem* item;
			StringA componentName, imageFilename;
			UIAniIcon* pAniIcon;
		
			for (Index i = C_INDEX(0); i < C_INDEX(9); ++i)
			{
				componentName.Format("ItemEquip%03iIcon", i );
				pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(componentName.c_str());

				for (Index j = C_INDEX(1); j < 6; ++j)
				{
					componentName.Format("ItemEquip%03iColor%d", i, j);
					SetUIEnable(componentName, false);
				}

				if( pAniIcon)
				{
					item = gGlobal.m_pBag->GetItem((UInt8)i+120);
					if( item && item->m_data)
					{
						const ItemData* pItemData = item->m_data;
						if( pItemData )
						{
							componentName.Format("ItemEquip%03iStack", i);
							if( item->GetStack() > 1 )
								Text.Format(_T("%i"), item->GetStack()); 
							else
								Text = "";
							gGlobal.SetUIStaticText(componentName, Text);
							componentName.Format("ItemEquip%03iLvl", i);
							if( item->GetGrade() > 0 )
								Text.Format(_T("+%i"), item->GetGrade()); 
							else
								Text = "";
							gGlobal.SetUIStaticText(componentName, Text);

							/*if (item->m_data->color > 0)
							{
								componentName.Format("ItemEquip%03iColor%d", i, item->m_data->color);
								SetUIEnable(componentName, true);
							}*/
							
							String temp(pItemData->icon2);
							
							pAniIcon->ClearAniData();
							pAniIcon->SetAnimationData(temp.c_str());
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
}

void CMenuItemProcess::PlaySound()
{
	FSound* sfx = NULL;
	switch(m_iSoundIndex)
	{
	case ITEM_TAKE:
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayDragItemUpEffect", String());
		break;
	case ITEM_PUT:
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayPutItemEffect", String());
		break;
	case ITEM_EQUIP_WP:
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayEqWeaponEffect", String());
		break;
	case ITEM_OFF_WP:
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayOffEqWeaponEffect", String());
		break;
	case ITEM_EQUIP_CLOTH:
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayEqClothEffect", String());
		break;
	case ITEM_OFF_CLOTH:
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayOffEqClothEffect", String());
		break;
	case ITEM_EQUIP_OTHER:
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayEqOtherEffect", String());
		break;
	case ITEM_OFF_OTHER:
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayOffEqOtherEffect", String());
		break;
	case ITEM_USE:
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayUseItemEffect", String());
		break;
	case ITEM_CANT_USE_OR_EQUIP:
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayCantEquipEffect", String());
		break;
	case ITEM_DROP:
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayDropItemEffect", String());
		break;
	}
	m_iSoundIndex = -1;
}

void CMenuItemProcess::UpdateItemDetail(int itemPos)
{
	if( gGlobal.m_pBag && gGlobal.m_resource )
	{
		CItem* item = gGlobal.m_pBag->GetItem(itemPos);
		if( item )
			ItemDetailPopUp(item->GetItem_id(), false, item);		
	}
}


void CMenuItemProcess::ItemDetailPopUp(UInt32 ItemId, bool bShop, CItem* item, bool showPrice)
{
	if ( !bShop && !item ) return;

	const ItemData* pItemData = gGlobal.m_resource->GetItemData(ItemId);
	UIImage* uIm = gGlobal.pUIManager->FindUIImage("ItemDetailBaseElement");
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon("ItemDetailImage");
	
	
	if( pItemData && pchar && pAniIcon && uIm)
	{
		String Text;
		bool needShow = true;
		
		CGameStringTable* stringTable = gGlobal.GetStringTable();

		String temp(pItemData->icon1);
		String empty;
		
		pAniIcon->ClearAniData();
		pAniIcon->SetAnimationData(temp.c_str());
		if (!pAniIcon->GetAniEnt()->HasAniFile())
		{
			pAniIcon->ClearAniData();
			pAniIcon->SetAnimationData(String(_T("itemSell0000.ent")).c_str());
		}
		pAniIcon->SetEnable(true);

		// item base element
		switch (pItemData->eq_baseElement)
		{	
		case ITEM_ELEMENT_GOLDEN:
			uIm->SetImage(uIm->GetImage(), 531, 189, 553, 211);
			break;
		case ITEM_ELEMENT_TREE:
			uIm->SetImage(uIm->GetImage(), 507, 189, 529, 211);
			break;
		case ITEM_ELEMENT_WATER:
			uIm->SetImage(uIm->GetImage(), 483, 189, 505, 211);
			break;
		case ITEM_ELEMENT_FIRE:
			uIm->SetImage(uIm->GetImage(), 458, 189, 480, 211);
			break;
		case ITEM_ELEMENT_EARTH:
			uIm->SetImage(uIm->GetImage(), 555, 189, 577, 211);
			break;
		case ITEM_ELEMENT_NONE:
		default:
			needShow = false;
			break;
		}

		if (needShow)
		{
			uIm->SetDetectRect();
			uIm->SetEnable(true);
		}
		else
			uIm->SetEnable(false);
		
		// item name
		Text.Format(_T("%s"), pItemData->name);
		
		Color32 textColor = 0;
		switch (pItemData->color)
		{
		case ItemColorIndex_Grey:	textColor = ItemTextColor[ItemColorIndex_Grey];			break;
		case ItemColorIndex_LightGreen:	textColor = ItemTextColor[ItemColorIndex_LightGreen];	break;
		case ItemColorIndex_LightBlue:	textColor = ItemTextColor[ItemColorIndex_LightBlue];	break;
		case ItemColorIndex_LightPurple:	textColor = ItemTextColor[ItemColorIndex_LightPurple];	break;
		case ItemColorIndex_LightGolden:	textColor = ItemTextColor[ItemColorIndex_LightGolden];	break;
		case ItemColorIndex_GodGolden:	textColor = ItemTextColor[ItemColorIndex_GodGolden];	break;
		}
	
		SetItemText("ItemDetailTextName", Text, textColor);

		//item maker
		if (!bShop)
		{
			Text.Format(_T("%s"), item->GetMaker());
			if (empty.Equals(Text.c_str()) || pItemData->item_type != ITEMTYPE_EQUIP)
				SetUIEnable("ItemDetailMakeByName", false);
			else
			{
				String tempMaker;
				tempMaker.Format(_T("%s%s"), Text.c_str(),gGlobal.GetStringTable()->Get(_T("MSG_MIX_MAKE")));
				SetItemText("ItemDetailMakeByName", tempMaker, TextColor[ColorIndex_Blue]);
			}
		}
		else
			SetUIEnable("ItemDetailMakeByName", false);

		Text = _T("");
		textColor = 0;
		
		UInt initX = 0, initY = 0;
		UInt curY;
		GetUIPos("ItemDetailTextType", initX, initY);
		curY = initY;

		//item bound
		UInt statusIndex = 0;
		StringA iconName;
		for (Index i = C_INDEX(0); i < 2; ++i)
		{
			iconName.Format("ItemDetailTextStatus%d", i);
			SetUIEnable(iconName, false);
		}

		textColor = TextColor[ColorIndex_LightBlue];

		if (bShop)
		{
			needShow = true;
			switch (pItemData->soulBound)
			{
			case SOULBOUND_NO_BOUND:	needShow = false;			break;
			case SOULBOUND_GET_BOUND:	Text = stringTable->Get(_T("MSG_ITEM_BOUND_GAIN"));		break;
			case SOULBOUND_EQ_BOUND:	Text = stringTable->Get(_T("MSG_ITEM_BOUND_USE"));		break;
			case SOULBOUND_USE_BOUND:	Text = stringTable->Get(_T("MSG_ITEM_BOUND_USE"));		break;
			}
		}
		else
		{
			needShow = true;
			switch(item->GetItem_soulboundState())
			{
			case SOULBOUND_NO_BOUND:
				needShow = false;
				break;
			case SOULBOUND_GET_BOUND:
				Text = stringTable->Get(_T("MSG_ITEM_BOUND_GAIN"));
				break;
			case SOULBOUND_EQ_BOUND:
				Text = stringTable->Get(_T("MSG_ITEM_BOUND_USE"));
				break;
			case SOULBOUND_USE_BOUND:
				Text = stringTable->Get(_T("MSG_ITEM_BOUND_USE"));
				break;
			case SOULBOUND_ALREADY_BOUND:
				Text = stringTable->Get(_T("MSG_ITEM_BOUND_ALREADY"));
				textColor = TextColor[ColorIndex_Yellow];
				break;
			}
		}
		iconName.Format("ItemDetailTextStatus%d", statusIndex);
		if (needShow)
		{
			SetItemText(iconName, Text, textColor);
			SetUIItemVerticalPosAndEnable(iconName, curY, true);
			++statusIndex;
		}
		else
			SetUIEnable(iconName, false);

		//item_sellable
		iconName.Format("ItemDetailTextStatus%d", statusIndex);
		Text = _T("");
		textColor = 0;
		if (bShop)
			SetUIEnable(iconName, false);
		else
		{
			if (!pItemData->sellable)
			{
				Text = stringTable->Get(_T("MSG_ITEM_CANT_SELL"));
				SetItemText(iconName, Text, ItemTextColor[ItemColorIndex_Red]);
				SetUIItemVerticalPosAndEnable(iconName, curY, true);
				++statusIndex;
			}
			else
				SetUIEnable(iconName, false);

		}

		if (statusIndex > 0)
			curY += ITEMDETAIL_UPPERSPACE;

		Text = _T("");
		textColor = 0;

		// item price
		if (showPrice)
		{
			if ( bShop )
			{
				String tempstr = stringTable->Get(_T("MSG_ITEM_TYPE_PRICE"));
				SetItemText("ItemDetailTextSellingType", tempstr);
				SetUIItemVerticalPosAndEnable("ItemDetailTextSellingType", curY, true);

				UInt32 price = 0;
				if ( gGlobal.g_pShopProcess )
				{
					UInt32 lastprice = gGlobal.g_pShopProcess->GetBuyPrice(pItemData->item_id);
					if (lastprice == 0)
					{
						lastprice =  (UInt32)((pItemData->g_price) * ( gGlobal.g_pShopProcess->GetBuyDiscount() / 100.0));
					}
					Text.Format(_T("%d"), lastprice);
					price = lastprice;
				}
				else
				{
					Text.Format(_T("%d"), pItemData->g_price);
					price = pItemData->g_price;
				}

				if ( 0 <= price && price < 1000)
					textColor = TextColor[ColorIndex_White];
				else if ( 1000 <= price && price < 10000)
					textColor = TextColor[ColorIndex_Green];
				else if ( 10000 <= price && price < 100000)
					textColor = TextColor[ColorIndex_Blue];
				else if ( 100000 <= price && price < 1000000)
					textColor = TextColor[ColorIndex_Violet];
				else if ( 1000000 <= price && price < 10000000)
					textColor = ItemTextColor[ItemColorIndex_Golden];
				
				SetItemText("ItemDetailTextPrice", Text, textColor);
				SetUIItemVerticalPosAndEnable("ItemDetailTextPrice", curY, true);

				tempstr = stringTable->Get(_T("MSG_ITEM_PRICE_UNIT2"));
				UIStaticText* temp = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextPriceUnit");
				if (temp)
				{
					temp->SetOffsetXY(180 + (Int32)Text.length() * 10, curY);
				}
				SetItemText("ItemDetailTextPriceUnit", tempstr);
				SetUIItemVerticalPosAndEnable("ItemDetailTextPriceUnit", curY, true);

				curY += ITEMDETAIL_UPPERSPACE;
			}
			else
			{
				UIPanel* shoppanel = gGlobal.pUIManager->FindUIPanel("ShopMenuPanel");
				if ( shoppanel && shoppanel->IsEnable() && gGlobal.g_CursorState && 
					(gGlobal.g_CursorState->GetStateID() == ST_APP_SHOP_SELL) )
				{

					String tempstr = stringTable->Get(_T("MSG_SHOP_SELL_PRICE"));
					SetItemText("ItemDetailTextSellingType", tempstr);
					SetUIItemVerticalPosAndEnable("ItemDetailTextSellingType", curY, true);

					UInt32 price = 0;
					if ( gGlobal.g_pShopProcess )
					{
						UInt32 lastprice = gGlobal.g_pShopProcess->GetSellPrice(pItemData->item_id);
						if (lastprice == 0)
						{
							lastprice =  (UInt32)((pItemData->g_price) * ( gGlobal.g_pShopProcess->GetSellDiscount() / 100.0));
						}
						Text.Format(_T("%d"), lastprice);
						price = lastprice;
					}
					else
					{
						Text.Format(_T("%d"), pItemData->g_price);
						price = pItemData->g_price;
					}

					if ( 0 <= price && price < 1000)
						textColor = TextColor[ColorIndex_White];
					else if ( 1000 <= price && price < 10000)
						textColor = TextColor[ColorIndex_Green];
					else if ( 10000 <= price && price < 100000)
						textColor = TextColor[ColorIndex_Blue];
					else if ( 100000 <= price && price < 1000000)
						textColor = TextColor[ColorIndex_Violet];
					else if ( 1000000 <= price && price < 10000000)
						textColor = ItemTextColor[ItemColorIndex_Golden];
					
					SetItemText("ItemDetailTextPrice", Text, textColor);
					SetUIItemVerticalPosAndEnable("ItemDetailTextPrice", curY, true);

					tempstr = stringTable->Get(_T("MSG_ITEM_PRICE_UNIT2"));
					UIStaticText* temp = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextPriceUnit");
					if (temp)
					{
						temp->SetOffsetXY(180 + (Int32)Text.length() * 10, curY);
					}
					SetItemText("ItemDetailTextPriceUnit", tempstr);
					SetUIItemVerticalPosAndEnable("ItemDetailTextPriceUnit", curY, true);

					SetUIItemVerticalPosAndEnable("ItemDetailTextPrice", curY, true);
					curY += ITEMDETAIL_UPPERSPACE;
				}
				else
				{
					SetUIEnable("ItemDetailTextSellingType", false);
					SetUIEnable("ItemDetailTextPrice", false);
					SetUIEnable("ItemDetailTextPriceUnit", false);
				}
			}
		}
		Text = _T("");
		// item type
		switch( pItemData->item_type )
		{
		case ITEMTYPE_MISC:		Text = stringTable->Get(_T("MSG_ITEMTYPE_MISC")); break;
		case ITEMTYPE_EQUIP:	Text = stringTable->Get(_T("MSG_ITEMTYPE_EQUIP")); break;
		case ITEMTYPE_FOOD:		Text = stringTable->Get(_T("MSG_ITEMTYPE_FOOD")); break;
		case ITEMTYPE_FOODMAT:	Text = stringTable->Get(_T("MSG_ITEMTYPE_FOODMAT")); break;
		case ITEMTYPE_DRUG:		Text = stringTable->Get(_T("MSG_ITEMTYPE_DRUG")); break;
		case ITEMTYPE_DRUGMAT:	Text = stringTable->Get(_T("MSG_ITEMTYPE_DRUGMAT")); break;
		case ITEMTYPE_WINE:		Text = stringTable->Get(_T("MSG_ITEMTYPE_WINE")); break;
		case ITEMTYPE_WINEMAT:	Text = stringTable->Get(_T("MSG_ITEMTYPE_WINEMAT")); break;
		case ITEMTYPE_EQUIPMAT: Text = stringTable->Get(_T("MSG_ITEMTYPE_EQUIPMAT")); break;
		case ITEMTYPE_CLOTHMAT: Text = stringTable->Get(_T("MSG_ITEMTYPE_CLOTHMAT")); break;
		case ITEMTYPE_GEM:		Text = stringTable->Get(_T("MSG_ITEMTYPE_GEM")); break;
		case ITEMTYPE_THROW:	Text = stringTable->Get(_T("MSG_ITEMTYPE_THROW")); break;
		case ITEMTYPE_SKILLBOOK: Text = stringTable->Get(_T("MSG_ITEMTYPE_SKILLBOOK")); break;
		case ITEMTYPE_SCRIPT:	Text = stringTable->Get(_T("MSG_ITEMTYPE_SCRIPT")); break;
		case ITEMTYPE_ADDON:	Text = stringTable->Get(_T("MSG_ITEMTYPE_ADDON")); break;
		case ITEMTYPE_ARRAYBOOK: Text = stringTable->Get(_T("MSG_ITEMTYPE_ARRAYBOOK")); break;
		//case ITEMTYPE_SPECIAL: Text = stringTable->Get("MSG_ITEMTYPE_SPECIAL"); break;
		case ITEMTYPE_BAG:		Text = stringTable->Get(_T("MSG_ITEMTYPE_BAG")); break;
		case ITEMTYPE_BUSINESS_ITEM:		Text = stringTable->Get(_T("MSG_ITEMTYPE_BUSINESS_ITEM")); break;
		case ITEMTYPE_BUSINESS_MONEY:		Text = stringTable->Get(_T("MSG_ITEMTYPE_BUSINESS_MONEY")); break;
		case ITEMTYPE_FAST_FOOD: Text = stringTable->Get(_T("MSG_ITEMTYPE_FAST_FOOD")); break;
		}
		bool needInc = false;
		if( Text.GetLength()>0 )
		{
			gGlobal.SetUIStaticText("ItemDetailTextType", Text);
			SetUIItemVerticalPosAndEnable("ItemDetailTextType", curY, true);
			needInc = true;
		}
		else
			SetUIEnable("ItemDetailTextType", false);
		//item sub type
		Text = _T("");
		if( pItemData->item_type==1 )
		{
			switch( pItemData->eqType )
			{
			case EQTYPE_SPEAR: Text = stringTable->Get(_T("MSG_EQTYPE_SPEAR")); break;
			case EQTYPE_WIPE: Text = stringTable->Get(_T("MSG_EQTYPE_WIPE")); break;
			case EQTYPE_FAN: Text = stringTable->Get(_T("MSG_EQTYPE_FAN")); break;
			case EQTYPE_PEN: Text = stringTable->Get(_T("MSG_EQTYPE_PEN")); break;
			case EQTYPE_SWORD: Text = stringTable->Get(_T("MSG_EQTYPE_SWORD")); break;
			case EQTYPE_DBSWORD: Text = stringTable->Get(_T("MSG_EQTYPE_DBSWORD")); break;
			case EQTYPE_BLADE: Text = stringTable->Get(_T("MSG_EQTYPE_BLADE")); break;
			case EQTYPE_OTHER: Text = stringTable->Get(_T("MSG_EQTYPE_OTHER")); break;
			case EQTYPE_HANDRING: Text = stringTable->Get(_T("MSG_EQTYPE_HANDRING")); break;
			case EQTYPE_CLAW: Text = stringTable->Get(_T("MSG_EQTYPE_CLAW")); break;
			case EQTYPE_GEAR: Text = stringTable->Get(_T("MSG_EQTYPE_GEAR")); break;
			case EQTYPE_ARMOR: Text = stringTable->Get(_T("MSG_EQTYPE_ARMOR")); break;
			case EQTYPE_SHOE: Text = stringTable->Get(_T("MSG_EQTYPE_SHOE")); break;
			case EQTYPE_BELT: Text = stringTable->Get(_T("MSG_EQTYPE_BELT")); break;
			case EQTYPE_RING: Text = stringTable->Get(_T("MSG_EQTYPE_RING")); break;
			case EQTYPE_AMULET: Text = stringTable->Get(_T("MSG_EQTYPE_AMULET")); break;
			case EQTYPE_CLOTH: Text = stringTable->Get(_T("MSG_EQTYPE_CLOTH")); break;
			}
		}
		if( Text.GetLength()>0 )
		{
			gGlobal.SetUIStaticText("ItemDetailTextSubType", Text);
			SetUIItemVerticalPosAndEnable("ItemDetailTextSubType", curY, true);
		}
		else
			SetUIEnable("ItemDetailTextSubType", false);
		
		if (needInc)
			curY += ITEMDETAIL_UPPERSPACE;

		
		//item req level
		if( pItemData->reqLv && pItemData->item_type != ITEMTYPE_IDENTIFY && pItemData->item_type != ITEMTYPE_REPAIR && pItemData->item_type != ITEMTYPE_REBUILD )
		{
			Text.Format(_T("%i"), pItemData->reqLv);			
			SetItemRowEnable("ReqLvl", true, curY, Text);
			curY += ITEMDETAIL_UPPERSPACE;

			UIStaticText* uiText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("ItemDetailLabelReqLvl");
			UIStaticText* uiText2 = (UIStaticText*)gGlobal.pUIManager->GetUIItem("ItemDetailTextReqLvl");
			if (pItemData->reqLv > pchar->GetLevel())
			{
				uiText->SetFontColor(ItemTextColor[ItemColorIndex_Red]);
				uiText2->SetFontColor(ItemTextColor[ItemColorIndex_Red]);
			}
			else
			{
				uiText->SetFontColor(TextColor[ColorIndex_White]);
				uiText2->SetFontColor(TextColor[ColorIndex_White]);
			}
			uiText->Redraw();
			uiText2->Redraw();
		}
		else
			SetItemRowEnable("ReqLvl", false);

		if( pItemData->reqSexClass1>0 || pItemData->reqSexClass2>0 )
		{
			SetUIItemVerticalPosAndEnable("ItemDetailLabelReqSexClass", curY, true);
			UIStaticText* uiText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("ItemDetailLabelReqSexClass");
			bool req1 = false;
			bool req2 = false;

			if( pItemData->reqSexClass1>0 )
			{
				int reqSex = -1;
				Text = _T("");
				switch( pItemData->reqSexClass1 )
				{
				case 1: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_1")); break;
				case 2: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_2")); break;
				case 3: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_3")); break;
				case 4: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_4")); break;
				case 5: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_5")); break;
				case 6: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_6")); break;
				case 7: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_7")); break;
				case 8: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_8")); break;
				case 100: Text = stringTable->Get(_T("MSG_SEXCLASS_MALE")); reqSex = 0;  break;
				case 101: Text = stringTable->Get(_T("MSG_SEXCLASS_FEMALE")); reqSex = 1; break;
				}
				gGlobal.SetUIStaticText("ItemDetailTextReqSexClass1", Text);
				SetUIItemVerticalPosAndEnable("ItemDetailTextReqSexClass1", curY, true);
				curY += ITEMDETAIL_UPPERSPACE;

				UIStaticText* uiText2 = (UIStaticText*)gGlobal.pUIManager->GetUIItem("ItemDetailTextReqSexClass1");
				if (pItemData->reqSexClass1 == pchar->GetSexClass() || reqSex == pchar->GetSex())
				{
					uiText2->SetFontColor(TextColor[ColorIndex_White]);
					req1 = true;
				}
				else
				{
					uiText2->SetFontColor(ItemTextColor[ItemColorIndex_Red]);
					req1 = false;
				}
				uiText2->Redraw();
			}
			else
			{
				SetUIEnable("ItemDetailTextReqSexClass1", false);
			}
			if( pItemData->reqSexClass2>0 )
			{
				int reqSex = -1;
				Text = _T("");
				switch( pItemData->reqSexClass2 )
				{
				case 1: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_1")); break;
				case 2: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_2")); break;
				case 3: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_3")); break;
				case 4: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_4")); break;
				case 5: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_5")); break;
				case 6: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_6")); break;
				case 7: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_7")); break;
				case 8: Text = stringTable->Get(_T("MSG_ACTOR_CLASS_8")); break;
				case 100: Text = stringTable->Get(_T("MSG_SEXCLASS_MALE")); reqSex = 0; break;
				case 101: Text = stringTable->Get(_T("MSG_SEXCLASS_FEMALE")); reqSex = 1; break;
				}
				gGlobal.SetUIStaticText("ItemDetailTextReqSexClass2", Text);
				SetUIItemVerticalPosAndEnable("ItemDetailTextReqSexClass2", curY, true);
				curY += ITEMDETAIL_UPPERSPACE;

				UIStaticText* uiText2 = (UIStaticText*)gGlobal.pUIManager->GetUIItem("ItemDetailTextReqSexClass2");
				if (pItemData->reqSexClass2 == pchar->GetSexClass() || reqSex == pchar->GetSex())
				{
					uiText2->SetFontColor(TextColor[ColorIndex_White]);
					req1 = true;
				}
				else
				{
					uiText2->SetFontColor(ItemTextColor[ItemColorIndex_Red]);
					req1 = false;
				}
				uiText2->Redraw();
			}
			else
			{
				SetUIEnable("ItemDetailTextReqSexClass2", false);
			}

			if (req1 || req2)
				uiText->SetFontColor(TextColor[ColorIndex_White]);
			else
				uiText->SetFontColor(ItemTextColor[ItemColorIndex_Red]);

			uiText->Redraw();
		}
		else
		{
			SetUIEnable("ItemDetailLabelReqSexClass", false);
			SetUIEnable("ItemDetailTextReqSexClass1", false);
			SetUIEnable("ItemDetailTextReqSexClass2", false);
		}

		//item req faction
		if( pItemData->reqFaction>0 )
		{
			Text = _T("");
			switch( pItemData->reqFaction )
			{
			case 1: Text = stringTable->Get(_T("MSG_FACTION_SIXFANDOOR")); break;
			case 2: Text = stringTable->Get(_T("MSG_FACTION_TWELVEDOCK")); break;
			case 3: Text = stringTable->Get(_T("MSG_FACTION_HOLY")); break;
			case 4: Text = stringTable->Get(_T("MSG_FACTION_SILVERCASINO")); break;
			case 5: Text = stringTable->Get(_T("MSG_FACTION_TONGDOOR")); break;
			case 6: Text = stringTable->Get(_T("MSG_FACTION_10THOUSAND")); break;
			case 7: Text = stringTable->Get(_T("MSG_FACTION_GREENCLOTH")); break;
			case 8: Text = stringTable->Get(_T("MSG_FACTION_GHOSTAREA")); break;
			}
			if (pItemData->reqFaction == pchar->GetFaction())
				SetItemRowEnable("ReqFaction", true, curY, Text, TextColor[ColorIndex_White]);
			else
				SetItemRowEnable("ReqFaction", true, curY, Text, ItemTextColor[ItemColorIndex_Red]);
			curY += ITEMDETAIL_LOWERSPACE;
		}
		else
			SetItemRowEnable("ReqFaction", false);

		initX = 0, initY = 0;
		GetUIPos("ItemDetailTextDesc", initX, initY);

		if (initY > curY)
			curY = initY;

		Text = _T("");
		Text.Format(_T("%s"), pItemData->description);
		if( Text.GetLength()>0 )
		{
			SetItemRowEnable("Desc", true, curY, Text);
			curY += (Text.GetLength() / 19 + 1) * ITEMDETAIL_LOWERSPACE;
			curY += 3;
		}
		else
			SetItemRowEnable("Desc", false);

		Text = pItemData->functionDes;
		if( pItemData->item_type!=1 && Text.GetLength()>0 )
		{
			//Text.Format(_T("%s%s"), stringTable->Get("MSG_ITEM_FUNCTIONHEADER"), Text.c_str());
			SetItemText("ItemDetailTextFunctionDesc", Text, TextColor[ColorIndex_Green]);
			SetUIItemVerticalPosAndEnable("ItemDetailTextFunctionDesc", curY, true);
			curY += ((Text.GetLength()+ 20) / 21) * ITEMDETAIL_LOWERSPACE;
		}
		else
			SetUIEnable("ItemDetailTextFunctionDesc", false);

		if (item && pItemData && pItemData->useCount > 1 && item->GetRemain() >= 1)
		{	
			Text.Format(_T("%d/%d"), item->GetRemain(), pItemData->useCount);
			SetItemRowEnable("UseCount", true, curY, Text);
			curY += ITEMDETAIL_LOWERSPACE;
		}
		else
			SetItemRowEnable("UseCount", false);

		Text = _T("");		
		if (pItemData->item_type == 1)
		{
			UInt rightCurY = curY;
			// max hp 
			if ( bShop )
			{
				if ( pItemData->maxHP_u != 0 ) {
					if (pItemData->maxHP_d != pItemData->maxHP_u)
                        Text.Format(_T("%i - %i"), pItemData->maxHP_d, pItemData->maxHP_u);
					else
						Text.Format(_T("%i"), pItemData->maxHP_d);	

					SetItemRowEnable("MaxHp", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else				
					SetItemRowEnable("MaxHp", false);				
			}
			else
			{
				if( item->GetBase_maxHP() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{					
					Text.Format(_T("%i "), item->GetBase_maxHP());
					SetItemRowEnable("MaxHp", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else				
					SetItemRowEnable("MaxHp", false);				
			}
			// max sp
			if ( bShop )
			{
				if ( pItemData->maxSP_u != 0 ) {
					if (pItemData->maxSP_d != pItemData->maxSP_u)
                        Text.Format(_T("%i - %i"), pItemData->maxSP_d, pItemData->maxSP_u);
					else
						Text.Format(_T("%i"), pItemData->maxSP_d);	

					SetItemRowEnable("MaxSp", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("MaxSp", false);				
			}
			else
			{
				if( item->GetBase_maxSP() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_maxSP());
					SetItemRowEnable("MaxSp", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("MaxSp", false);				
			}
			// atk
			if ( bShop )
			{
				if ( pItemData->ATK_u !=0 ) {
					if (pItemData->ATK_d != pItemData->ATK_u)
                        Text.Format(_T("%i - %i"), pItemData->ATK_d, pItemData->ATK_u);					
					else
						Text.Format(_T("%i"), pItemData->ATK_d);	
					SetItemRowEnable("Atk", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Atk", false);
			}
			else
			{
				if( item->GetBase_ATK() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_ATK());
					SetItemRowEnable("Atk", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Atk", false);
			}
			
			// def
			if ( bShop )
			{
				if ( pItemData->DEF_u != 0 ) {
					if (pItemData->DEF_d != pItemData->DEF_u)
                        Text.Format(_T("%i - %i"), pItemData->DEF_d, pItemData->DEF_u);
					else
						Text.Format(_T("%i"), pItemData->DEF_d);
					SetItemRowEnable("Def", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}else
				{
					SetItemRowEnable("Def", false);
				}
			}
			else
			{
				if(item->GetBase_DEF() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_DEF());
					SetItemRowEnable("Def", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Def", false);				
			}
			// hit
			if ( bShop )
			{
				if ( pItemData->HIT_u != 0 ) {
					if (pItemData->HIT_d != pItemData->HIT_u)
                        Text.Format(_T("%i - %i"), pItemData->HIT_d, pItemData->HIT_u);
					else
						Text.Format(_T("%i"), pItemData->HIT_d);
					SetItemRowEnable("Hit", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Hit", false);
			}
			else
			{
				if( item->GetBase_HIT() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_HIT());
					SetItemRowEnable("Hit", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Hit", false);
			}
			// eva
			if ( bShop )
			{
				if (  pItemData->EVA_u != 0 ) {
					if (pItemData->EVA_d != pItemData->EVA_u)
                        Text.Format(_T("%i - %i"), pItemData->EVA_d, pItemData->EVA_u);
					else
						Text.Format(_T("%i"), pItemData->EVA_d);	
					SetItemRowEnable("Eva", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Eva", false);
			}
			else
			{
				if(item->GetBase_EVA() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_EVA());
					SetItemRowEnable("Eva", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Eva", false);
			}
			// pow
			if ( bShop )
			{
				if ( pItemData->POW_u != 0 ) {
					if (pItemData->POW_d != pItemData->POW_u)
                        Text.Format(_T("%i - %i"), pItemData->POW_d, pItemData->POW_u);
					else
						Text.Format(_T("%i"), pItemData->POW_d);	
					SetItemRowEnable("Pow", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Pow", false);
			}
			else 
			{
				if( item->GetBase_POW() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_POW());
					SetItemRowEnable("Pow", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Pow", false);
			}
			// speed
			if ( bShop )
			{
				if ( pItemData->SPD_u != 0 ) {
					if (pItemData->SPD_d != pItemData->SPD_u)
                        Text.Format(_T("%i - %i"), pItemData->SPD_d, pItemData->SPD_u);
					else
						Text.Format(_T("%i"), pItemData->SPD_d);	
					SetItemRowEnable("Spd", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Spd", false);
			}
			else
			{
				if( item->GetBase_SPD() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_SPD());
					SetItemRowEnable("Spd", true, curY, Text);	
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Spd", false, curY, Text);
			}
			// str
			if ( bShop )
			{
				if ( pItemData->STR_u != 0 ) {
					if (pItemData->STR_d != pItemData->STR_u)
                        Text.Format(_T("%i - %i"), pItemData->STR_d, pItemData->STR_u);
					else
						Text.Format(_T("%i"), pItemData->STR_d);	
					SetItemRowEnable("Str", true, rightCurY, Text);
					rightCurY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Str", false);
			}
			else
			{
				if( item->GetBase_STR() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_STR());
					SetItemRowEnable("Str", true, rightCurY, Text);
					rightCurY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Str", false);
			}
			// con
			if ( bShop )
			{
				if ( pItemData->CON_u != 0 ) {
					if (pItemData->CON_d != pItemData->CON_u)
                        Text.Format(_T("%i - %i"), pItemData->CON_d, pItemData->CON_u);
					else
						Text.Format(_T("%i"), pItemData->CON_d);
					SetItemRowEnable("Con", true, rightCurY, Text);
					rightCurY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Con", false);
			}
			else
			{
				if( item->GetBase_CON() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_CON());
					SetItemRowEnable("Con", true, rightCurY, Text);
					rightCurY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Con", false);
			}
			// sta
			if ( bShop )
			{
				if ( pItemData->STA_u != 0 ) {
					if (pItemData->STA_d != pItemData->STA_u)
                        Text.Format(_T("%i - %i"), pItemData->STA_d, pItemData->STA_u);
					else
						Text.Format(_T("%i"), pItemData->STA_d);	
					SetItemRowEnable("Sta", true, rightCurY, Text);
					rightCurY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Sta", false);
			}
			else
			{
				if( item->GetBase_STA() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_STA());
					SetItemRowEnable("Sta", true, rightCurY, Text);
					rightCurY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Sta", false);
			}
			// agi
			if ( bShop )
			{
				if ( pItemData->AGI_u != 0 ) {
					if (pItemData->AGI_d != pItemData->AGI_u)
                        Text.Format(_T("%i - %i"), pItemData->AGI_d, pItemData->AGI_u);
					else
						Text.Format(_T("%i"), pItemData->AGI_d);	
					SetItemRowEnable("Agi", true, rightCurY, Text);
					rightCurY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Agi", false);
			}
			else
			{
				if( item->GetBase_AGI() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_AGI());
					SetItemRowEnable("Agi", true, rightCurY, Text);
					rightCurY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Agi", false);
			}
			// spr
			if ( bShop )
			{
				if ( pItemData->SPR_u != 0 ) {
					if (pItemData->SPR_d != pItemData->SPR_u)
                        Text.Format(_T("%i - %i"), pItemData->SPR_d, pItemData->SPR_u);
					else
						Text.Format(_T("%i"), pItemData->SPR_d);	
					SetItemRowEnable("Spr", true, rightCurY, Text);
					rightCurY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Spr", false);
			}
			else
			{
				if( item->GetBase_SPR() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_SPR());
					SetItemRowEnable("Spr", true, rightCurY, Text);
					rightCurY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("Spr", false);				
			}
			// max vp
			if ( bShop )
			{
				if ( pItemData->maxVP_u != 0 ) {
					if (pItemData->maxVP_d != pItemData->maxVP_u)
                        Text.Format(_T("%i - %i"), pItemData->maxVP_d, pItemData->maxVP_u);
					else
						Text.Format(_T("%i"), pItemData->maxVP_d);	
					SetItemRowEnable("MaxVp", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("MaxVp", false);
			}
			else
			{
				if( item->GetBase_maxVP() != 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
				{
					Text.Format(_T("%i "), item->GetBase_maxVP());
					SetItemRowEnable("MaxVp", true, curY, Text);
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
					SetItemRowEnable("MaxVp", false);
			}
			if (rightCurY > curY)
				curY = rightCurY;
		}
		else
		{
			SetItemRowEnable("MaxVp", false);
			SetItemRowEnable("MaxSp", false);				
			SetItemRowEnable("Atk", false);
			SetItemRowEnable("Def", false);
			SetItemRowEnable("Hit", false);				
			SetItemRowEnable("Eva", false);	
			SetItemRowEnable("Pow", false);	
			SetItemRowEnable("Spr", false);				
			SetItemRowEnable("Spd", false);				
			SetItemRowEnable("Str", false);				
			SetItemRowEnable("Con", false);				
			SetItemRowEnable("Sta", false);	
			SetItemRowEnable("Agi", false);				
			SetItemRowEnable("Spr", false);				
			SetItemRowEnable("MaxHp", false);				
		}

		// Normal Item
		/*
		Text = _T("");
		if( pItemData->item_type!=1 )
		{
			switch( pItemData->usePlace )
			{
			case 0: Text = stringTable->Get("MSG_ITEM_USEPLACE_ALL"); break;
			case 1: Text = stringTable->Get("MSG_ITEM_USEPLACE_BATTLE"); break;
			case 2: Text = stringTable->Get("MSG_ITEM_USEPLACE_NONBAT"); break;
			}
			SetItemRowEnable("UsePlace", true, curY, Text);
			curY += ITEMDETAIL_LOWERSPACE;
		}else
		{
			SetItemRowEnable("UsePlace", false);
		}
	
		Text = _T("");
		if( pItemData->item_type!=1 && pItemData->targetType>0 )
		{
			switch( pItemData->targetType )
			{
			case 0: Text = stringTable->Get("MSG_ITEM_TARGETTYPE_ALL"); break;
			case 1: Text = stringTable->Get("MSG_ITEM_TARGETTYPE_PLAYER"); break;
			case 2: Text = stringTable->Get("MSG_ITEM_TARGETTYPE_MOB"); break;
			case 3: Text = stringTable->Get("MSG_ITEM_TARGETTYPE_MOBONLY"); break;
			case 4: Text = stringTable->Get("MSG_ITEM_TARGETTYPE_BOSSONLY"); break;
			}
		
			SetItemRowEnable("TargetType", true, curY, Text);
			curY += ITEMDETAIL_LOWERSPACE;
		}else
		{
			SetItemRowEnable("TargetType", false);
		}
		if( pItemData->item_type!=1 && pItemData->target>0 && pItemData->nTarget>0 )
		{
			Text.Format(_T("%i(%i)"), pItemData->target, pItemData->nTarget);
			SetItemRowEnable("Target", true, curY, Text);
			curY += ITEMDETAIL_LOWERSPACE;
		}else
		{
			SetItemRowEnable("Target", false);
		}
		if( pItemData->item_type!=1 && pItemData->period )
		{
			Text.Format(_T("%i"), pItemData->period);
			SetItemRowEnable("Period", true, curY, Text);
			curY += ITEMDETAIL_LOWERSPACE;
		}else
		{
			SetItemRowEnable("Period", false);
		}
		if( pItemData->item_type!=1 && pItemData->coolTime )
		{
			Text.Format(_T("%i"), pItemData->coolTime);
			SetItemRowEnable("CoolTime", true, curY, Text);
			curY += ITEMDETAIL_LOWERSPACE;
		}else
		{
			SetItemRowEnable("CoolTime", false);
		}*/

		curY += 3;
		
		// currently disabled fields
		
		/*if (!bShop && item && item->GetBuff().length() > 0)
		{
			ExtractedBuffData data = ExtractBuffString(item);

			if (data.ID > 0)
			{
				SetUIItemVerticalPosAndEnable("ItemDetailLabelBuff", curY, true);

				Text.Format(_T("%i"), pItemData->bBuff);
				gGlobal.SetUIStaticText("ItemDetailTextBuffName", Text);
				Text.Format(_T("%s"), _T("BuffTime"));
				gGlobal.SetUIStaticText("ItemDetailTextBuffTime", Text);
			}
			else
			{
				SetUIEnable("ItemDetailLabelBuff", false);
				SetUIEnable("ItemDetailTextBuffName", false);
				SetUIEnable("ItemDetailLabelBuffName", false);
				SetUIEnable("ItemDetailTextBuffTime", false);
				SetUIEnable("ItemDetailLabelBuffTime", false);
			}			
		}
		else*/
		{
			SetUIEnable("ItemDetailLabelBuff", false);
			SetUIEnable("ItemDetailTextBuffName", false);
			SetUIEnable("ItemDetailLabelBuffName", false);
			SetUIEnable("ItemDetailTextBuffTime", false);
			SetUIEnable("ItemDetailLabelBuffTime", false);
		}

		//upgradeExp
		/*if (!bShop && item && pItemData->eq_maxUpgrade > 0)
		{
			Text.Format(_T("%i/%i"), item->GetGrade(), pItemData->eq_maxUpgrade);
			SetItemRowEnable("Blacksmith", true, curY, Text);
			curY += ITEMDETAIL_LOWERSPACE;
		}
		else
		{
			SetItemRowEnable("Blacksmith", false);
		}*/

		if (bShop)
		{
			String limitText;
			switch (pItemData->item_TimeLimitType)
			{
			case ITEM_TIME_LIMIT_TYPE_CREATE:
				limitText.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_TIME_LIMIT_TYPE_GET")));
				break;
			case ITEM_TIME_LIMIT_TYPE_EQUIP:
				limitText.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_TIME_LIMIT_TYPE_EQ")));
				break;
			case ITEM_TIME_LIMIT_TYPE_USE:
				limitText.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_TIME_LIMIT_TYPE_USE")));
				break;
			}

			if (limitText.IsEmpty())
				SetItemRowEnable("Period", false);
			else
			{
				UIStaticText* pLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelPeriod");
				UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextPeriod");

				if (pLabel && pText)
				{
					limitText.Format(_T("%d%s (%s)"), pItemData->item_TimeLimit,gGlobal.GetStringTable()->Get(_T("MSG_DAY")), limitText.c_str());
					
					pLabel->SetText(gGlobal.GetStringTable()->Get(_T("MSG_TIME_LIMIT_HAVE")));
					pLabel->Redraw();
					pLabel->SetOffsetXY(pLabel->GetOffsetX(), curY);

					pText->SetText(limitText.c_str());
					pText->Redraw();
					pText->SetOffsetXY(pText->GetOffsetX(), curY);
					pLabel->SetEnable(true);
					pText->SetEnable(true);
					curY += ITEMDETAIL_LOWERSPACE;
				}
			}
		}
		else
		{
			if (item->GetItem_expireDate() == 0)
			{
				String limitText;
				switch (pItemData->item_TimeLimitType)
				{
				case ITEM_TIME_LIMIT_TYPE_EQUIP:
					limitText.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_TIME_LIMIT_TYPE_EQ")));
					break;
				case ITEM_TIME_LIMIT_TYPE_USE:
					limitText.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_TIME_LIMIT_TYPE_USE")));
					break;
				}

				if (limitText.IsEmpty())
					SetItemRowEnable("Period", false);
				else
				{
					UIStaticText* pLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelPeriod");
					UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextPeriod");

					if (pLabel && pText)
					{
						limitText.Format(_T("%d%s (%s)"), pItemData->item_TimeLimit,gGlobal.GetStringTable()->Get(_T("MSG_DAY")), limitText.c_str());
						
						pLabel->SetText(gGlobal.GetStringTable()->Get(_T("MSG_TIME_LIMIT_HAVE")));
						pLabel->Redraw();
						pLabel->SetOffsetXY(pLabel->GetOffsetX(), curY);

						pText->SetText(limitText.c_str());
						pText->Redraw();
						pText->SetOffsetXY(pText->GetOffsetX(), curY);
						pLabel->SetEnable(true);
						pText->SetEnable(true);
						curY += ITEMDETAIL_LOWERSPACE;
					}
				}
			}
			else
			{
				UIStaticText* pLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelPeriod");
				UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextPeriod");

				if (pLabel && pText)
				{
					time_t now;
					time(&now);
					UInt32 remainTime = item->GetItem_expireDate() - (UInt32)now;

					remainTime /= 60; //change it to min first
					UInt min = remainTime % 60;
					remainTime /= 60; //change it to hour
					UInt hour = remainTime % 24;
					UInt day = remainTime / 24;
					
					if (day > 0 || hour > 0 || min > 0)
					{
						String limitText;
						if (day > 0)
							limitText.Format(_T("%d%s"), day + 1,gGlobal.GetStringTable()->Get(_T("MSG_DAY")));
						else if (hour > 0)
							limitText.Format(_T("%d%s"), hour,gGlobal.GetStringTable()->Get(_T("MSG_HOUR")));
						else if (min > 0)
							limitText.Format(_T("%d%s"), min,gGlobal.GetStringTable()->Get(_T("MSG_MINUTE")));
					
						pLabel->SetText(gGlobal.GetStringTable()->Get(_T("MSG_TIME_LIMIT_REMAIN")));
						pLabel->Redraw();
						pLabel->SetOffsetXY(pLabel->GetOffsetX(), curY);
						pText->SetText(limitText.c_str());
						pText->Redraw();
						pText->SetOffsetXY(pText->GetOffsetX(), curY);
						pLabel->SetEnable(true);
						pText->SetEnable(true);
						curY += ITEMDETAIL_LOWERSPACE;
					}
					else
						SetItemRowEnable("Period", false);
				}
			}
		}

		//enduring
		if ( bShop )
			Text.Format(_T("%i/%i"), pItemData->eq_maxEndure, pItemData->eq_maxEndure);
		else
			Text.Format(_T("%i/%i"), item->GetEndure(), item->GetMEndure());
		
		if (pItemData->eq_maxEndure > 0 && (item == NULL || pItemData->Identify == 0 || item->GetItem_IdentifyState() ) )
		{
			SetItemRowEnable("Dura", true, curY, Text);
			curY += ITEMDETAIL_LOWERSPACE;
		}
		else
			SetItemRowEnable("Dura", false);

		if (item && pItemData->Identify && item->GetItem_IdentifyState() == 0)
		{
			curY += 3 * ITEMDETAIL_LOWERSPACE;
			SetItemRowEnable("UnknowItem", true, curY,gGlobal.GetStringTable()->Get(_T("MSG_ITEM_HAVENT_IDENTIFY")), ItemTextColor[ItemColorIndex_Red]);
			curY += 3 * ITEMDETAIL_LOWERSPACE;
		}
		else
			SetItemRowEnable("UnknowItem", false);
		// gem inlay
		curY += 5;
		UInt8 InlayGemLevel[8] = {0, 0, 0, 0, 0, 0, 0, 0};
		Boolean CanInlayCam[8] = {false, false, false, false, false, false, false, false};
		if (!bShop)
		{
			for (UInt8 i = 0; i < 10; i++)
			{
				UInt32 InlayItemId = item->GetInlay_gem(i);
				if (InlayItemId > 0)
				{
					// Gem Type : 1=atk, 2=def, 3=pow, 4=maxHp, 5=maxSp, 6=hit, 7=eva, 8=spd
					gGlobal.g_TempResult = 0;
					CallLuaFunc(gGlobal.g_pParentLua, "GetMixShopInlayGemType", String(), InlayItemId);
					UInt GemType = (UInt)gGlobal.g_TempResult;

					gGlobal.g_TempResult = 0;
					CallLuaFunc(gGlobal.g_pParentLua, "GetGemItemLevel", String(), InlayItemId);
					UInt GemLevel = (UInt)gGlobal.g_TempResult;

					if (GemType >= 1 && GemType <= 8)
						InlayGemLevel[GemType - 1] = GemLevel;

					TRACE_INFODTL_3(GLOBAL_LOGGER, _F(" gemid : %d, gem level : %d, gem type : %d."),
						InlayItemId, GemLevel, GemType);
				}
			}

			if(ITEM_MIXSHOP_TYPE_INLAY == item->m_data->item_type)
			{
				for (UInt i = 0; i < 8; i++)
				{
					gGlobal.g_TempResult = 0;
					CallLuaFunc(gGlobal.g_pParentLua, "CanInlayGamItemType", String(), item->m_data->eqType , i+1);
					Boolean CanInlay = (Boolean)gGlobal.g_TempResult;

					if (CanInlay)
						CanInlayCam[i] = CanInlay;

					TRACE_INFODTL_2(GLOBAL_LOGGER, _F("InlayItemTpye %d,Can Inlay %d"),
						i+1, CanInlay);
				}
			}
		}

		for (Index a = C_INDEX(0); a < C_INDEX(8); a++)
		{
			UIStaticText* pInlayLabel = NULL;
			UIStaticText* pInlayText = NULL;
			UIImage* img = NULL;
			UInt16 InlayValue = 0;
			StringA uiItemName;

			switch(a)
			{
			case 0: // atk
				pInlayLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelInlayGemAtk");
				pInlayText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextInlayGemAtk");
				if (!bShop)
					InlayValue = item->GetInlay_ATK();
				uiItemName.Format("ItemDetailBGInlayGemAtk");
				break;
			case 1: // def
				pInlayLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelInlayGemDef");
				pInlayText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextInlayGemDef");
				if (!bShop)
					InlayValue = item->GetInlay_DEF();
				uiItemName.Format("ItemDetailBGInlayGemDef");
				break;
			case 2: // pow
				pInlayLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelInlayGemPow");
				pInlayText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextInlayGemPow");
				if (!bShop)
					InlayValue = item->GetInlay_POW();
				uiItemName.Format("ItemDetailBGInlayGemPow");
				break;
			case 3:
				pInlayLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelInlayGemMaxHp");
				pInlayText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextInlayGemMaxHp");
				if (!bShop)
					InlayValue = item->GetInlay_maxHP();
				uiItemName.Format("ItemDetailBGInlayGemMaxHp");
				break;
			case 4:
				pInlayLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelInlayGemMaxSp");
				pInlayText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextInlayGemMaxSp");
				if (!bShop)
					InlayValue = item->GetInlay_maxSP();
				uiItemName.Format("ItemDetailBGInlayGemMaxSp");
				break;
			case 5:
				pInlayLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelInlayGemHit");
				pInlayText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextInlayGemHit");
				if (!bShop)
					InlayValue = item->GetInlay_HIT();
				uiItemName.Format("ItemDetailBGInlayGemHit");
				break;
			case 6:
				pInlayLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelInlayGemEva");
				pInlayText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextInlayGemEva");
				if (!bShop)
					InlayValue = item->GetInlay_EVA();
				uiItemName.Format("ItemDetailBGInlayGemEva");
				break;
			case 7:
				pInlayLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelInlayGemSpd");
				pInlayText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextInlayGemSpd");
				if (!bShop)
					InlayValue = item->GetInlay_SPD();
				uiItemName.Format("ItemDetailBGInlayGemSpd");
				break;
			}

			if (pInlayLabel && pInlayText)
			{
				if (InlayValue > 0)
				{
					pInlayLabel->SetOffsetXY(pInlayLabel->GetOffsetX(), curY);
					pInlayLabel->SetEnable(true);

					Text.Format(_T("%d"), InlayValue);
					pInlayText->SetText(Text);
					pInlayText->Redraw();
					pInlayText->SetOffsetXY(pInlayText->GetOffsetX(), curY);
					pInlayText->SetEnable(true);

					StringA temp;
					for (Index b = C_INDEX(0); b < C_INDEX(10); b++)
					{
						temp.Format("%s%02i", uiItemName.c_str(), b);
						img = gGlobal.pUIManager->FindUIImage(temp.c_str());
						if (img)
						{
							if (InlayGemLevel[a] >= b + 1)
							{
								img->SetOffsetXY(img->GetOffsetX(), curY - 4);
								img->SetEnable(true);
							}
							else
								img->SetEnable(false);
						}
					}

					curY += ITEMDETAIL_LOWERSPACE;
				}
				else if(InlayValue <= 0 && CanInlayCam[a] && item->m_data->eqType < EQTYPE_AMULET)
				{
					pInlayLabel->SetOffsetXY(pInlayLabel->GetOffsetX(), curY);
					pInlayLabel->SetEnable(true);
					StringA temp;
					temp.Format("%sBlock", uiItemName.c_str());
					img = gGlobal.pUIManager->FindUIImage(temp.c_str());
					if (img)
					{
						img->SetOffsetXY(img->GetOffsetX(), curY - 4);
						img->SetEnable(true);
					}
					curY += ITEMDETAIL_LOWERSPACE;
				}
				else
				{
					pInlayLabel->SetEnable(false);
					pInlayText->SetEnable(false);

					StringA temp;
					for (Index b = C_INDEX(0); b < C_INDEX(10); b++)
					{
						temp.Format("%s%i", uiItemName.c_str(), b);
						img = gGlobal.pUIManager->FindUIImage(temp.c_str());
						if (img)
							img->SetEnable(false);
					}
				}
			}
		}

		SetItemRowEnable("AddOn", false);
		SetUIEnable("ItemDetailTextSetName", false);
		SetUIEnable("ItemDetailLabelSet", false);
		SetUIEnable("ItemDetailTextSetName", false);
		SetUIEnable("ItemDetailTextSetRatio", false);
		SetUIEnable("ItemDetailLabelSetBatAttrib1", false);
		SetUIEnable("ItemDetailTextSetBatAttrib1", false);
		SetUIEnable("ItemDetailLabelSetBatAttrib2", false);
		SetUIEnable("ItemDetailTextSetBatAttrib2", false);
		SetUIEnable("ItemDetailLabelSetBatAttrib3", false);
		SetUIEnable("ItemDetailTextSetBatAttrib3", false);
		SetUIEnable("ItemDetailLabelSetBatAttrib4", false);
		SetUIEnable("ItemDetailTextSetBatAttrib4", false);
		SetUIEnable("ItemDetailLabelSetBatAttrib5", false);
		SetUIEnable("ItemDetailTextSetBatAttrib5", false);
		SetUIEnable("ItemDetailLabelSetBatAttrib6", false);
		SetUIEnable("ItemDetailTextSetBatAttrib6", false);
		SetUIEnable("ItemDetailLabelSetBatAttrib7", false);
		SetUIEnable("ItemDetailTextSetBatAttrib7", false);
		SetUIEnable("ItemDetailLabelSetBatAttrib8", false);
		SetUIEnable("ItemDetailTextSetBatAttrib8", false);

		SetUIEnable("ItemDetailLabelSetAttrib1", false);
		SetUIEnable("ItemDetailTextSetAttrib1", false);
		SetUIEnable("ItemDetailLabelSetAttrib2", false);
		SetUIEnable("ItemDetailTextSetAttrib2", false);
		SetUIEnable("ItemDetailLabelSetAttrib3", false);
		SetUIEnable("ItemDetailTextSetAttrib3", false);
		SetUIEnable("ItemDetailLabelSetAttrib4", false);
		SetUIEnable("ItemDetailTextSetAttrib4", false);
		SetUIEnable("ItemDetailLabelSetAttrib5", false);
		SetUIEnable("ItemDetailTextSetAttrib5", false);
		SetUIEnable("ItemDetailLabelSetAtkRatio", false);
		SetUIEnable("ItemDetailLabelSetDefRatio", false);
		SetUIEnable("ItemDetailLabelSetHitRatio", false);
		SetUIEnable("ItemDetailLabelSetEvaRatio", false);
		SetUIEnable("ItemDetailLabelSetPowRatio", false);
		SetUIEnable("ItemDetailLabelSetSpdRatio", false);
		SetUIEnable("ItemDetailTextSetBatAttribRatio", false);
		SetUIEnable("ItemDetailLabelSetSkill", false);
		SetUIEnable("ItemDetailTextSetSkill", false);

		if (pItemData->itemset_id > 0)
		{
			UIStaticText* pLabel = NULL;
			UIStaticText* pText = NULL;

			EquipItemSetInfo* pEqItemSetInfo = NULL;
			const ItemSetData* pItemSetData = NULL;

			if(gGlobal.m_pBag->HasItemByUID(item->GetItem_uid())) // self item
				pEqItemSetInfo = gGlobal.m_pBag->GetItemSetInfo(pItemData->itemset_id);
			else // target equip item
				pEqItemSetInfo = gGlobal.g_pTargetEquipProcess->GetItemSetInfo(pItemData->itemset_id);

			if (pEqItemSetInfo && pEqItemSetInfo->pItemSetData)
				pItemSetData = pEqItemSetInfo->pItemSetData;

			if (pItemSetData == NULL) // for public show item detail
				pItemSetData = gGlobal.m_resource->GetItemSetData(pItemData->itemset_id);

			D3DCOLOR FontColor = 0xFF7F7F7F;
			if (pEqItemSetInfo && pEqItemSetInfo->pItemSetData)
			{
				if (pEqItemSetInfo->eq_count >= pEqItemSetInfo->pItemSetData->count)
					FontColor = 0xFFF0F800;

				Text.Format(_T("%s %d/%d"), pEqItemSetInfo->pItemSetData->name, pEqItemSetInfo->eq_count, pEqItemSetInfo->pItemSetData->count);
			}
			else if(pItemSetData)
			{
				Text.Format(_T("%s"), pItemSetData->name);
			}

			pText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextSetName");
			if (pText)
			{
				pText->SetText(Text.c_str());
				if(FontColor == 0xFF7F7F7F)
					pText->SetFontColor(0xFFFFFFFF);
				else
					pText->SetFontColor(FontColor);
				pText->Redraw();
				pText->SetOffsetXY(pText->GetOffsetX(), curY);
				pText->SetEnable(true);
			}

			curY += ITEMDETAIL_LOWERSPACE;

			if(pItemSetData)
			{
				UInt itemsetY1 = curY;
				StringA componentName;
				Index i = C_INDEX(0);
				for (i = C_INDEX(0); i < C_INDEX(8); ++i)
				{
					UInt16 BatValue = 0;
					switch(i)
					{
					case 0:	BatValue = pItemSetData->ATK;	break;
					case 1:	BatValue = pItemSetData->DEF;	break;
					case 2:	BatValue = pItemSetData->POW;	break;
					case 3:	BatValue = pItemSetData->MaxHP;	break;
					case 4:	BatValue = pItemSetData->MaxSP;	break;
					case 5:	BatValue = pItemSetData->HIT;	break;
					case 6:	BatValue = pItemSetData->EVA;	break;
					case 7:	BatValue = pItemSetData->SPD;	break;
					}

					if (BatValue > 0)
					{
						componentName.Format("ItemDetailLabelSetBatAttrib%i", i + 1);
						pLabel = gGlobal.pUIManager->FindUIStaticText(componentName.c_str());
						componentName.Format("ItemDetailTextSetBatAttrib%i", i + 1);
						pText = gGlobal.pUIManager->FindUIStaticText(componentName.c_str());
						if (pLabel && pText)
						{
							pLabel->SetFontColor(FontColor);
							pLabel->Redraw();
							pLabel->SetOffsetXY(pLabel->GetOffsetX(), itemsetY1);
							pLabel->SetEnable(true);

							Text.Format(_T("%d"), BatValue);
							pText->SetText(Text.c_str());
							pText->SetFontColor(FontColor);
							pText->Redraw();
							pText->SetOffsetXY(pText->GetOffsetX(), itemsetY1);
							pText->SetEnable(true);

							itemsetY1 += ITEMDETAIL_LOWERSPACE;
						}
					}
				}

				UInt itemsetY2 = curY;
				for (i = C_INDEX(0); i < C_INDEX(5); ++i)
				{
					UInt16 Value = 0;
					switch(i)
					{
					case 0:	Value = pItemSetData->STR;	break;
					case 1:	Value = pItemSetData->CON;	break;
					case 2:	Value = pItemSetData->STA;	break;
					case 3:	Value = pItemSetData->AGI;	break;
					case 4:	Value = pItemSetData->SPR;	break;
					}

					if (i >= 0 && i < 5 && Value > 0)
					{
						componentName.Format("ItemDetailLabelSetAttrib%i", i + 1);
						pLabel = gGlobal.pUIManager->FindUIStaticText(componentName.c_str());
						componentName.Format("ItemDetailTextSetAttrib%i", i + 1);
						pText = gGlobal.pUIManager->FindUIStaticText(componentName.c_str());
						if (pLabel && pText)
						{
							pLabel->SetFontColor(FontColor);
							pLabel->Redraw();
							pLabel->SetOffsetXY(pLabel->GetOffsetX(), itemsetY2);
							pLabel->SetEnable(true);

							Text.Format(_T("%d"), Value);
							pText->SetText(Text.c_str());
							pText->SetFontColor(FontColor);
							pText->Redraw();
							pText->SetOffsetXY(pText->GetOffsetX(), itemsetY2);
							pText->SetEnable(true);

							itemsetY2 += ITEMDETAIL_LOWERSPACE;
						}
					}
				}

				if (itemsetY1 > itemsetY2)
					curY = itemsetY1;
				else
					curY = itemsetY2;

				UInt8 bat_attrib = pItemSetData->bat_attrib;
				UInt8 ratio = pItemSetData->ratio;
				if (bat_attrib > 0 && ratio > 0)
				{
					pLabel = NULL;
					switch(bat_attrib)
					{
					case 1:	pLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelSetAtkRatio");	break;
					case 2:	pLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelSetDefRatio");	break;
					case 3:	pLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelSetHitRatio");	break;
					case 4:	pLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelSetEvaRatio");	break;
					case 5:	pLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelSetPowRatio");	break;
					case 6:	pLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelSetSpdRatio");	break;
					}
					pText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextSetBatAttribRatio");

					if (pLabel && pText)
					{
						pLabel->SetFontColor(FontColor);
						pLabel->Redraw();
						pLabel->SetOffsetXY(pLabel->GetOffsetX(), curY);
						pLabel->SetEnable(true);

						Text.Format(_T("+%d%%"), ratio);
						pText->SetText(Text.c_str());
						pText->SetFontColor(FontColor);
						pText->Redraw();
						pText->SetOffsetXY(pText->GetOffsetX(), curY);
						pText->SetEnable(true);

						curY += ITEMDETAIL_LOWERSPACE;
					}
				}

				if (pItemSetData->skill_ids.size() > 0)
				{
					pLabel = gGlobal.pUIManager->FindUIStaticText("ItemDetailLabelSetSkill");
					pText = gGlobal.pUIManager->FindUIStaticText("ItemDetailTextSetSkill");
					if (pLabel && pText)
					{
						pLabel->SetFontColor(FontColor);
						pLabel->Redraw();
						pLabel->SetOffsetXY(pLabel->GetOffsetX(), curY);
						pLabel->SetEnable(true);

						Text.Empty();
						for (Index j = Index(0); j < pItemSetData->skill_ids.size(); ++j)
						{
							const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(pItemSetData->skill_ids[j]);
							if (pSkillData)
							{
								if (j == 0)
									Text.Format(_T("%s"), pSkillData->name);
								else
									Text.Format(_T("%s,%s"), Text.c_str(), pSkillData->name);
							}
						}
						pText->SetText(Text.c_str());
						pText->SetFontColor(FontColor);
						pText->Redraw();
						pText->SetOffsetXY(pText->GetOffsetX(), curY);
						pText->SetEnable(true);

						curY += (Text.GetLength() / 19 + 1) * ITEMDETAIL_LOWERSPACE;
						//curY += ITEMDETAIL_LOWERSPACE;
					}
				}
			}
			else
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F(" ItemDetailPopUp: can not get itemsetdata."));
			}
		}

		if (!bShop && item && item->GetBuffID() > 0)
		{
			//if (data.ID > 0)
			{
				UIChatPanel * pChatPanel = gGlobal.pUIManager->FindUIChatPanel("ItemDetailBuffDesc");
				const BuffData* pBuffData = gGlobal.m_resource->GetBuffData(item->GetBuffID());
				if (pBuffData && pChatPanel)
				{
					pChatPanel->ClearAllRow();
					//add skill name
					String name;
					{
						String text;
						String  temp;
						text.Format(_T("#G%s"), pBuffData->name);
						if (item->GetBuffValue1() > 0)
						{							
							temp.Format(_T(" %d"), item->GetBuffValue1());
							text.append(temp.c_str());
						}
						if (item->GetBuffValue2() > 0)
						{
							temp.Format(_T(" %d"), item->GetBuffValue2());
							text.append(temp.c_str());
						}
						if (item->GetBuffValue3() > 0)
						{
							temp.Format(_T(" %d"), item->GetBuffValue3());
							text.append(temp.c_str());
						}
						
						pChatPanel->AddStringW(text.c_str());
						if (wcscmp(pBuffData->buff_description, _T("0")) != 0)
							pChatPanel->AddStringW(pBuffData->buff_description);
						pChatPanel->FillImage();
						pChatPanel->EndRow();
						pChatPanel->Redraw();

						pChatPanel->SetOffsetXY(pChatPanel->GetOffsetX(), curY);
						pChatPanel->SetEnable(true);
						curY += pChatPanel->GetTotalHeight();
					}
					//else
					//	pChatPanel->SetEnable(false);
				}
			}
		}
		else
			SetUIEnable("ItemDetailBuffDesc", false);

		if (item && item->GetSkillID() > 0 && (pItemData->Identify == 0 || item->GetItem_IdentifyState() ))
		{
			//if (data.ID > 0)
			{
				UIChatPanel * pChatPanel = gGlobal.pUIManager->FindUIChatPanel("ItemDetailSkillDesc");
				const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(item->GetSkillID());
				if (pSkillData && pChatPanel)
				{
					pChatPanel->ClearAllRow();
					//add skill name
					String text;
					text.Format(gGlobal.GetStringTable()->Get(_T("MSG_ITEM_DETAIL_SKILL")),pSkillData->name);
					if (!text.IsEmpty())
					{
						pChatPanel->AddStringW(text.c_str());
						if (wcscmp(pSkillData->skill_description, _T("0")) != 0)
							pChatPanel->AddStringW(pSkillData->skill_description);
						if (wcscmp(pSkillData->skill_functionDes, _T("0")) != 0)
							pChatPanel->AddStringW(pSkillData->skill_functionDes);
						pChatPanel->FillImage();
						pChatPanel->EndRow();
						pChatPanel->Redraw();

						pChatPanel->SetOffsetXY(pChatPanel->GetOffsetX(), curY);
						pChatPanel->SetEnable(true);
						curY += pChatPanel->GetTotalHeight();
					}
					//else
					//	pChatPanel->SetEnable(false);
				}
			}
		}
		else
		{
			SetUIEnable("ItemDetailSkillDesc", false);
		}

		if (item && item->GetItem_quality() > 0)
		{
			Text.Format(_T("%d"), item->GetItem_quality() );
			SetItemRowEnable("Quality", true, curY, Text);
			curY += ITEMDETAIL_LOWERSPACE;
		}
		else
			SetItemRowEnable("Quality", false);

		UIPanelEx* uPan = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");
		if( uPan )
		{
			UInt count = curY / 13;
			uPan->ChangeCount(count);
		}
	}else
	{
		if (pAniIcon)
			pAniIcon->SetEnable(false);
	}
}

void CMenuItemProcess::UpdateItemPetNameList()
{
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->FindUIPanel("ItemMenuPanel");
	if (panel && gGlobal.g_PetLst)
	{
		String Text;
		CPet* pet = NULL;
		{
			for( Index i = C_INDEX(0); i < MAX_PET_PARTNER_ONE_PAGE; ++i )
			{
				pet = gGlobal.g_PetLst->GetPetByIndex(i + m_iStartIndex);
				StringA componentName;
				StringA itemNameImage;
				StringA itemLockImage;
				StringA state;
				componentName.Format("ItemNameListLine%01d", i+1 );
				itemNameImage.Format("ItemNameImage%01d", i+1 );
				itemLockImage.Format("PetPartnerLock%01d", i+1);	
				if (pet)
				{
					Text.Format(pet->GetName().c_str());
					SetUIEnable(itemNameImage, true);

					if (pet->GetState() == 1)
					{
						state.Format("ItemPartnerState%01d1", i+1);
						SetUIEnable(state, false);
						state.Format("ItemPartnerState%01d2", i+1);
						SetUIEnable(state, true);
					}
					else
					{
						state.Format("ItemPartnerState%01d2", i+1);
						SetUIEnable(state, false);
						state.Format("ItemPartnerState%01d1", i+1);
						SetUIEnable(state, true);
					}
					SetUIEnable(itemLockImage, pet->GetLock());
				}
				else
				{
					Text.Format(_T(""));
					SetUIEnable(itemNameImage, false);
					state.Format("ItemPartnerState%01d2", i+1);
					SetUIEnable(state, false);
					state.Format("ItemPartnerState%01d1", i+1);
					SetUIEnable(state, false);
					SetUIEnable(itemLockImage, false);	
				}

				itemNameImage.Format("ItemNameSelected%01d", i+1);
				SetUIEnable(itemNameImage, false);

				gGlobal.SetUIStaticText(componentName, Text);
			}

			if ( m_iSelectedPetIndex < (MAX_PET_PARTNER_ONE_PAGE + m_iStartIndex) && m_iSelectedPetIndex >= m_iStartIndex )
			{
				StringA itemNameSelected;
				itemNameSelected.Format("ItemNameSelected%01d", m_iSelectedPetIndex - m_iStartIndex + 1);
				SetUIEnable(itemNameSelected, true);

				itemNameSelected.Format("ItemNameImage%01d", m_iSelectedPetIndex - m_iStartIndex + 1);
				SetUIEnable(itemNameSelected, false);
			}
		}
	}
}
void CMenuItemProcess::UpdateItemPartnerNameList()
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("ItemMenuPanel");
	UIScrollBarV* scrollBar = gGlobal.pUIManager->FindUIScrollBarV("ItemNameListScroll");
	if (panel && gGlobal.g_pPartnerGroup && scrollBar)
	{
		String Text;
		CPartner* partner = NULL;
		for( Index i = C_INDEX(0); i < MAX_PET_PARTNER_ONE_PAGE; ++i )
		{
			partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(i + m_iStartIndex);
			StringA componentName;
			StringA itemNameImage;
			StringA state;
			StringA itemLockImage;
			componentName.Format("ItemNameListLine%01d", i+1 );
			itemNameImage.Format("ItemNameImage%01d", i+1 );
			itemLockImage.Format("PetPartnerLock%01d", i+1 );
			if (partner)
			{
				Text.Format(partner->GetRaname().c_str());
				SetUIEnable(itemNameImage, true);

				if (partner->GetState() == 1)
				{
					state.Format("ItemPartnerState%01d1", i+1);
					SetUIEnable(state, false);
					state.Format("ItemPartnerState%01d2", i+1);
					SetUIEnable(state, true);
				}
				else
				{
					state.Format("ItemPartnerState%01d2", i+1);
					SetUIEnable(state, false);
					state.Format("ItemPartnerState%01d1", i+1);
					SetUIEnable(state, true);
				}
				SetUIEnable(itemLockImage, partner->GetLock());
			}
			else
			{
				Text.Format(_T(""));
				SetUIEnable(itemNameImage, false);
				state.Format("ItemPartnerState%01d2", i+1);
				SetUIEnable(state, false);
				state.Format("ItemPartnerState%01d1", i+1);
				SetUIEnable(state, false);
				SetUIEnable(itemLockImage, false);	
			}

			itemNameImage.Format("ItemNameSelected%01d", i+1);
			SetUIEnable(itemNameImage, false);
			gGlobal.SetUIStaticText(componentName, Text);
		}
			
		if ( m_iSelectedPartnerIndex < (MAX_PET_PARTNER_ONE_PAGE + m_iStartIndex) && m_iSelectedPartnerIndex >= m_iStartIndex )
		{
			StringA itemNameSelected;
			itemNameSelected.Format("ItemNameSelected%01d", m_iSelectedPartnerIndex - m_iStartIndex + 1);
			SetUIEnable(itemNameSelected, true);

			itemNameSelected.Format("ItemNameImage%01d", m_iSelectedPartnerIndex - m_iStartIndex + 1);
			SetUIEnable(itemNameSelected, false);
		}
	}
}

void CMenuItemProcess::UpdateItemPartner(int index)
{
	m_iMenuIndex = ITEM_PARTNER_MENU;
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->FindUIPanel("ItemMenuPanel");
	if (panel)
	{
		StringA bankMoneyLock = "BankMoneyLockImage";
		SetUIEnable(bankMoneyLock, false);
		StringA yuanBaoLock = "YuanBaoLockImage";
		SetUIEnable(yuanBaoLock, false);

		if (gGlobal.g_pPartnerGroup)
		{
			String Text;
			
			if (index >= 0)
				m_iSelectedPartnerIndex = index + m_iStartIndex;

			UpdateItemPartnerNameList();

			CPartner* partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(m_iSelectedPartnerIndex);
			UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("ItemModelDummy");
			if (pAniIcon)
			{
				if (partner && partner->m_data)
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
					gGlobal.SetUIStaticText("ItemPartyLikeText", Text, true);
					Text.Format(_T("%d/%d"), partner->GetHP(), partner->GetMaxHP() );
					gGlobal.SetUIStaticText("ItemPartyHPText", Text);
					Text.Format(_T("%d/%d"), partner->GetSP(), partner->GetMaxSP() );
					gGlobal.SetUIStaticText("ItemPartySPText", Text);

					gGlobal.SetUILinerImage("ItemPartyHPBar", partner->GetMaxHP(), partner->GetHP() );
					gGlobal.SetUILinerImage("ItemPartySPBar", partner->GetMaxSP(), partner->GetSP() );

				}
				else
				{
					gGlobal.SetUILinerImage("ItemPartyHPBar", 0, 1 );
					gGlobal.SetUILinerImage("ItemPartySPBar", 0, 1 );

					gGlobal.SetUIStaticText("ItemPartyLikeText", _T(""));
					gGlobal.SetUIStaticText("ItemPartyHPText", _T(""));
					gGlobal.SetUIStaticText("ItemPartySPText", _T(""));
					UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("ItemModelDummy");

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
			gGlobal.SetUILinerImage("ItemPartyHPBar", 0, 1 );
			gGlobal.SetUILinerImage("ItemPartySPBar", 0, 1 );

			gGlobal.SetUIStaticText("ItemPartyLikeText", _T(""));
			gGlobal.SetUIStaticText("ItemPartyHPText", _T(""));
			gGlobal.SetUIStaticText("ItemPartySPText", _T(""));

			UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("ItemModelDummy");

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
				componentName.Format("ItemNameListLine%01d", i+1 );
				itemNameImage.Format("ItemNameImage%01d", i+1 );

				SetUIEnable(itemNameImage, false);
				state.Format("ItemPartnerState%01d2", i+1);
				SetUIEnable(state, false);
				state.Format("ItemPartnerState%01d1", i+1);
				SetUIEnable(state, false);

				itemNameImage.Format("ItemNameSelected%01d", i+1);
				SetUIEnable(itemNameImage, false);

				gGlobal.SetUIStaticText(componentName, _T(""));
			}
		}
	}
}

void CMenuItemProcess::UpdateItemPet(UInt8 petIndex)
{
	m_iMenuIndex = ITEM_PET_MENU;
	
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->FindUIPanel("ItemMenuPanel");
	if (panel)
	{
		StringA bankMoneyLock = "BankMoneyLockImage";
		SetUIEnable(bankMoneyLock, false);
		StringA yuanBaoLock = "YuanBaoLockImage";
		SetUIEnable(yuanBaoLock, false);		
		if (gGlobal.g_PetLst)
		{
			String Text;
			if (petIndex >= 0)
				m_iSelectedPetIndex = petIndex + m_iStartIndex;

			UpdateItemPetNameList();

			CPet* pet = gGlobal.g_PetLst->GetPetByIndex(m_iSelectedPetIndex);
			UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("ItemModelDummy");

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

						String colorFileName = modelName.substr(0, modelName.length() - 4);
						char abc = pPetData->pet_color + 96;
						colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
						temp.ChangeColorP(modelName.c_str(), colorFileName.c_str());

						pAniIcon->SetAnimationIndex(temp.GetAnimationIndex());	
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(&temp);
						pAniIcon->SetRotation(SOUTHEAST);
						gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
					}
	
					Text.Format(_T("%d"), pet->GetLevel());
					
					gGlobal.SetUIStaticText("ItemPetLVText", Text, true);
					if(pPetData->pet_type ==0)
					{
						Text.Format(_T("%d/100"), pet->GetHappiness());
						gGlobal.SetUILinerImage("ItemPetHappyBar", 100, pet->GetHappiness());
						SetUIEnable("ItemPetHappyBar", true);	
					}
					else
					{
						Text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
						SetUIEnable("ItemPetHappyBar", false);	
					}
					gGlobal.SetUIStaticText("ItemPetHappyText", Text);
					if (pPetData->pet_type == 1 && pet->GetExpireDate() == 0)
					{
						Text.Format(_T("%d/%d"), pet->GetVP(),pet->GetMaxVP());
						gGlobal.SetUILinerImage("ItemPetVPBar", pet->GetMaxVP(), pet->GetVP());
						SetUIEnable("ItemPetVPBar", true);
					}
					else
					{
						Text = gGlobal.GetStringTable()->Get(_T("MSG_PET_NONE"));
						SetUIEnable("ItemPetVPBar", false);	
					}
					gGlobal.SetUIStaticText("ItemPetVPText", Text);		

				}
				else
				{
					gGlobal.SetUILinerImage("ItemPetHappyBar", 0, 1 );
					gGlobal.SetUILinerImage("ItemPetVPBar", 0, 1 );
					gGlobal.SetUIStaticText("ItemPetLVText", _T(""), true);
					gGlobal.SetUIStaticText("ItemPetHappyText", _T(""));
					gGlobal.SetUIStaticText("ItemPetVPText", _T(""));

					UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("ItemModelDummy");

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
			gGlobal.SetUILinerImage("ItemPetHappyBar", 0, 1 );
			gGlobal.SetUILinerImage("ItemPetVPBar", 0, 1 );
			gGlobal.SetUIStaticText("ItemPetLVText", _T(""), true);
			gGlobal.SetUIStaticText("ItemPetHappyText", _T(""));
			gGlobal.SetUIStaticText("ItemPetVPText", _T(""));

			UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("ItemModelDummy");

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
				componentName.Format("ItemNameListLine%01d", i+1 );
				itemNameImage.Format("ItemNameImage%01d", i+1 );

				SetUIEnable(itemNameImage, false);
				state.Format("ItemPartnerState%01d2", i+1);
				SetUIEnable(state, false);
				state.Format("ItemPartnerState%01d1", i+1);
				SetUIEnable(state, false);

				itemNameImage.Format("ItemNameSelected%01d", i+1);
				SetUIEnable(itemNameImage, false);

				gGlobal.SetUIStaticText(componentName, _T(""));
			}
		}
	}
}


void CMenuItemProcess::NewUpdateItemMenu(int bag)
{
	if( gConfigure.uNetwork )
	{
		UIPanel* panel = gGlobal.pUIManager->FindUIPanel("ItemMenuPanel");
		if (panel)
		{
			int nowBag = 0;
			if (bag == -1)
				nowBag = m_iBag;
			else
			{
				nowBag = bag;
				m_iBag = bag;
			}

			for (Index i = C_INDEX(0) ; i < C_INDEX(BAG_ITEM_COUNT) ; ++i)
				UpdateItemSlot(i);
			
			gGlobal.pUIManager->AddDirtyUIItem(panel);	
		}
		else
		{
			for (Index i = C_INDEX(0) ; i < C_INDEX(BAG_ITEM_COUNT) ; ++i)
				UpdateShortCutSlot(i);
		}
	}
}

void CMenuItemProcess::NewUpdateItemCharacter()
{
	m_iMenuIndex = ITEM_CHAR_MENU;
	if( gGlobal.m_pBag && gGlobal.m_resource )
	{
		UpdateCharacterEnt();
		CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
		if( pchar )
		{
			String Text;
			Text.Format(_T("%i"), pchar->GetSMoney());
			SetItemText("ItemSCashDollar", Text, 0, true);		
			Text.Format(_T("%i"), pchar->GetMoney());
			SetItemText("ItemCashDollar", Text, 0, true);
			Text.Format(_T("%i"), pchar->GetBankMoney());
			SetItemText("ItemBankDollar", Text, 0, true);
			Text.Format(_T("%i"), pchar->GetYuanBao());
			SetItemText("ItemGoldDollar", Text, 0, true);
            StringA bankMoneyLock = "BankMoneyLockImage";
			SetUIEnable(bankMoneyLock, pchar->GetBankLock());
			StringA yuanBaoLock = "YuanBaoLockImage";
			SetUIEnable(yuanBaoLock, pchar->GetYuanbaoLock());
		}
		UpdateItemEq();
	}
}

void CMenuItemProcess::UpdateCharacterEnt()
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("ItemModelDummy");
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

void CMenuItemProcess::UpdateCharWeaponEnt(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor)
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		UInt8 sexClass = gGlobal.g_pCharacter->GetCharNetObject()->GetSexClass();
		const ItemData* pItemData = gGlobal.m_resource->GetItemData(eq_weapon);
		if( pItemData )
		{
			if( sexClass==pItemData->reqSexClass1 )
				ChangeWeapon(String(pItemData->eq_Outlook1));
			else if( sexClass==pItemData->reqSexClass2 )
				ChangeWeapon(String(pItemData->eq_Outlook2));
			else
				ChangeWeapon(String());
		}
		else
			ChangeWeapon(String());
	}
}

void CMenuItemProcess::ChangeWeapon(String name)
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("ItemModelDummy");
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

void CMenuItemProcess::UpdateHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor)
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		const ItemData* pItemData = gGlobal.m_resource->GetItemData(eq_headBand);
		UIAniIcon * pAniIcon = (UIAniIcon *)gGlobal.pUIManager->GetUIItem("ItemModelDummy");
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

void CMenuItemProcess::ChangeHeadAccessory(String name, String colorFile)
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("ItemModelDummy");
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

void CMenuItemProcess::UpdateBody(const UInt32 &eq_cloth)
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
		ChangeBody(eq_cloth);
}

void CMenuItemProcess::ChangeBody(UInt16 eqCloth)
{
	if (m_iMenuIndex == ITEM_CHAR_MENU)
	{
		UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("ItemModelDummy");
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

void CMenuItemProcess::SetIconOff(int iconPos, bool eq)
{
	String text;
	text.Empty();
	StringA componentName;
	UIAniIcon* pAniIcon = NULL;

	if (eq)
	{
		if (iconPos >=0 && iconPos < 9)
		{
			componentName.Format("ItemEquip%03iIcon", iconPos );
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(componentName.c_str());
			if (pAniIcon)
			{
				componentName.Format("ItemEquip%03iStack", iconPos);				
				SetItemText(componentName, text, 0, true);
				componentName.Format("ItemEquip%03iLvl", iconPos);
				SetItemText(componentName, text, 0, true);
				pAniIcon->SetEnable(false);
			}
		}
	}
	else
	{
		if (iconPos >=0 && iconPos < BAG_ITEM_COUNT )
		{
			componentName.Format("ItemSlot%03iIcon", iconPos );
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(componentName.c_str());

			if (pAniIcon)
			{
				componentName.Format("ItemSlot%03iStack", iconPos );				
				SetUIEnable(componentName, false);
				SetItemText(componentName, text, 0, true);
				componentName.Format("ItemSlot%03iLvl", iconPos );

				SetUIEnable(componentName, false);
				SetItemText(componentName, text, 0, true);
				pAniIcon->SetEnable(false);
				componentName.Format("ItemSlot%03iRed", iconPos );
				SetUIEnable(componentName, false);
				for (Index i = C_INDEX(1); i < 6; ++i)
				{
					componentName.Format("ItemSlot%03iColor%d", iconPos, i);
					SetUIEnable(componentName, false);
				}
			}
		}
	}
}

void CMenuItemProcess::Scroll(int up)
{
	UInt8 maxCount = 0;
	UIScrollBarV* scrollBar = gGlobal.pUIManager->FindUIScrollBarV("ItemNameListScroll");
	
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

	if (up == 0)
	{
		if (m_iStartIndex > 0)
			--m_iStartIndex;
	}
	else if (up == 1)
	{
		if ( (m_iStartIndex + MAX_PET_PARTNER_ONE_PAGE) < (maxCount ) )
			++m_iStartIndex;
	}
	else if (up == 2 && scrollBar)
	{
		if (maxCount < MAX_PET_PARTNER_ONE_PAGE)
			m_iStartIndex = 0;
		else if (scrollBar->GetSelNum() + MAX_PET_PARTNER_ONE_PAGE >= maxCount)
			m_iStartIndex = maxCount - MAX_PET_PARTNER_ONE_PAGE;
		else 
			m_iStartIndex = scrollBar->GetSelNum();
	}

	if (m_iMenuIndex == ITEM_PARTNER_MENU)
		UpdateItemPartnerNameList();
	else if (m_iMenuIndex == ITEM_PET_MENU)
		UpdateItemPetNameList();

}

void CMenuItemProcess::ResetScroll()
{
	UIScrollBarV* scrollBar = gGlobal.pUIManager->FindUIScrollBarV("ItemNameListScroll");

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

void CMenuItemProcess::SetUIItemVerticalPosAndEnable(StringA component, UInt curY, bool enabled)
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

void CMenuItemProcess::SetItemRowEnable(StringA prefix, bool show, UInt curY, String showText, Color32 textColor)
{
	StringA label;
	label.Format("ItemDetailLabel%s", prefix.c_str());
	StringA detailText;
	detailText.Format("ItemDetailText%s", prefix.c_str());

	if (show)
	{	
		SetItemText(detailText, showText, textColor);
		SetUIItemVerticalPosAndEnable(detailText, curY, true);
		SetUIItemVerticalPosAndEnable(label, curY, true);
	}
	else
	{
		SetUIEnable(detailText, false);
		SetUIEnable(label, false);
	}
}

void CMenuItemProcess::UpdateItemSlot(UInt8 slot, bool addDirty)
{
	String Text;
	StringA componentName;	
	UIAniIcon* pAniIcon;

	componentName.Format("ItemSlot%03iIcon", slot );	
	pAniIcon = gGlobal.pUIManager->FindUIAniIcon(componentName.c_str());
	if(pAniIcon && gGlobal.m_pBag)
	{
		CItem* item = gGlobal.m_pBag->GetItem(slot +  BAG_ITEM_COUNT * m_iBag);
		if( item && item->m_data)
		{
			const ItemData* pItemData = item->m_data;
			if( pItemData )
			{
				componentName.Format("ItemSlot%03iStack", slot );
				if( item->GetStack() > 1 )
					Text.Format(_T("%i"), item->GetStack() - item->GetGivenStack() ); 
				else
					Text = "";
				SetUIEnable(componentName, true);
				SetItemText(componentName, Text, 0, addDirty);
				componentName.Format("ItemSlot%03iLvl", slot );
				if( item->GetGrade() > 0 )
					Text.Format(_T("+%i"), item->GetGrade()); 
				else
					Text = "";
				SetUIEnable(componentName, true);
				SetItemText(componentName, Text, 0, addDirty);

				String temp(item->m_data->icon2);

				for (Index i = C_INDEX(1); i < 6; ++i)
				{
					componentName.Format("ItemSlot%03iColor%d", slot, i);
					SetUIEnable(componentName, false);
				}

				componentName.Format("ItemSlot%03iRed", slot );
				if (NeedRedBox((slot +  BAG_ITEM_COUNT * m_iBag)))
					SetUIEnable(componentName, true);
				else
				{
					SetUIEnable(componentName, false);
					/*if (item->m_data->item_type == ITEMTYPE_EQUIP && item->m_data->color > 0)
					{
						componentName.Format("ItemSlot%03iColor%d", slot, item->m_data->color);
						SetUIEnable(componentName, true);
					}*/
				}

				if (item->IsGiven())
					pAniIcon->SetRenderStyle(1);
				else
					pAniIcon->SetRenderStyle(0);

				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(temp.c_str());
				if (!pAniIcon->GetAniEnt()->HasAniFile())
				{
					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(String(_T("itemSell0000s.ent")).c_str());
				}
				pAniIcon->SetEnable(true);

				StringA lockItemName;
				lockItemName.Format("ItemSlot%03iLock", slot);
                SetUIEnable(lockItemName, item->GetLock());
				
				if (addDirty)
					gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);//uIm);
			}
			else
			{	
				SetIconOff(slot, false);
				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(String(_T("itemSell0000s.ent")).c_str());
				pAniIcon->SetEnable(true);
			}
		}
		else
		{
			StringA lockItemName;
			lockItemName.Format("ItemSlot%03iLock", slot);
			SetUIEnable(lockItemName, false);	
			SetIconOff(slot, false);
		}
	}

	UpdateShortCutSlot(slot);
}

void CMenuItemProcess::DragPartner()
{
	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(m_iSelectedPartnerIndex);
	UIAniIcon* partnerFace = gGlobal.pUIManager->FindUIAniIcon("ItemMovingIcon");
	if (partner && partner->m_data && partnerFace)
	{	
		partnerFace->SetEnable(true);			
		partnerFace->ClearAniData();
		partnerFace->SetAnimationData(String(partner->m_data->m_basicinfo->mob_faceS).c_str());
		gGlobal.pUIManager->AddDirtyUIItem(partnerFace);
	}
}

void CMenuItemProcess::DragPet()
{
	CPet* pet = gGlobal.g_PetLst->GetPetByIndex(m_iSelectedPetIndex);
	if (pet)
	{
		const PetData* pPetData = gGlobal.m_resource->GetPetData(pet->GetPet_id());
		UIAniIcon* petFace = gGlobal.pUIManager->FindUIAniIcon("ItemMovingIcon");
		if (pPetData && petFace)
		{
			petFace->SetEnable(true);
			petFace->ClearAniData();
			petFace->SetAnimationData(String(pPetData->pet_faceS).c_str());
			gGlobal.pUIManager->AddDirtyUIItem(petFace);
		}
	}
}

UInt32 CMenuItemProcess::GetSelectedPartnerID()
{
	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(m_iSelectedPartnerIndex);
	if (partner)
		return partner->GetPartner_id();
	return 0;
}

UInt32 CMenuItemProcess::GetSelectedPetID()
{
	CPet* pet = gGlobal.g_PetLst->GetPetByIndex(m_iSelectedPetIndex);
	if (pet)
		return pet->GetPet_uid();
	return 0;
}

bool CMenuItemProcess::IsGivenPartner()
{
	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(m_iSelectedPartnerIndex);
	if (partner)
		return partner->IsGiven();
	return true;
}

bool CMenuItemProcess::IsGivenPet()
{
	CPet* pet = gGlobal.g_PetLst->GetPetByIndex(m_iSelectedPetIndex);
	if (pet)
		return pet->IsGiven();
	return true;

}

UInt8 CMenuItemProcess::GetItemStack(UInt8 index)
{
	CItem* item = gGlobal.m_pBag->GetItem(index);
	if (item)
		return item->GetStack();
	return 0;
}
bool CMenuItemProcess::IsItemSent(UInt8 index)
{
	CItem* item = gGlobal.m_pBag->GetItem(index);
	if (item)
		return ((item->GetItemValue1() == 1)? true:false);
	return false;

}
bool CMenuItemProcess::IsItemBind(UInt8 index)
{
	CItem* item = gGlobal.m_pBag->GetItem(index);
	if (item)
		return (((item->GetItem_soulboundState() == SOULBOUND_ALREADY_BOUND) || (item->GetLock()))? true:false);
	return false;
}

bool CMenuItemProcess::IsItemGiven(UInt8 index)
{
	CItem* item = gGlobal.m_pBag->GetItem(index);
	if (item)
		return item->IsGiven();
	return false;
}

void CMenuItemProcess::SetItemText(StringA componentName, String text, Color32 textColor, bool addDirty)
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

bool CMenuItemProcess::NeedRedBox(UInt8 index)
{
	CItem* item = gGlobal.m_pBag->GetItem(index);
	if( item && item->m_data)
	{
		if (item->m_data->item_type != ITEMTYPE_REPAIR && item->m_data->item_type != ITEMTYPE_REBUILD && item->m_data->item_type != ITEMTYPE_STONE &&
			item->m_data->item_type != ITEMTYPE_IDENTIFY && item->m_data->reqLv > gGlobal.g_pCharacter->GetLevel() )
			return true;

		if (item->m_data->reqSexClass1 > 0 || item->m_data->reqSexClass2 > 0)
		{
			bool canUse = false;
			if (item->m_data->reqSexClass1 > 0)
			{
				int reqSex = 2;
				if (item->m_data->reqSexClass1 == 100)
					reqSex = 0;
				else if (item->m_data->reqSexClass1 == 101)
					reqSex = 1;
				if (item->m_data->reqSexClass1 == gGlobal.g_pCharacter->GetCharNetObject()->GetSexClass() ||
					reqSex == gGlobal.g_pCharacter->GetCharNetObject()->GetSex() )
					canUse = true;
			}

			if (item->m_data->reqSexClass2 > 0)
			{
				int reqSex = 2;
				if (item->m_data->reqSexClass2 == 100)
					reqSex = 0;
				else if (item->m_data->reqSexClass2 == 101)
					reqSex = 1;
				if (item->m_data->reqSexClass2 == gGlobal.g_pCharacter->GetCharNetObject()->GetSexClass() || 
					reqSex == gGlobal.g_pCharacter->GetCharNetObject()->GetSex() )
					canUse = true;
			}
			if (!canUse)
				return true;
		}
		if (item->m_data->reqFaction > 0 &&
			item->m_data->reqFaction != gGlobal.g_pCharacter->GetCharNetObject()->GetFaction())
				return true;		
	}
	return false;
}

void CMenuItemProcess::BindCantTrading(UInt8 type) //0:trading, 1:given
{
	if (type == 0)
		ShowMessage("MSG_TRADING_ITEM_BOUND");
	else if (type == 1)
		ShowMessage("MSG_GIVEN_ITEM_BOUND");
}

void CMenuItemProcess::ShowPublicItemDetail(UInt32 char_id, UInt32 item_uid)
{
	MapService_Proxy::ShowPublicItemDetail(gGlobal.pPeerProxy, RPCContext(), char_id, item_uid, gGlobal.g_pCharacter->GetChar_id());
}


bool CMenuItemProcess::IsItemSellable(UInt8 index)
{
	CItem* item = gGlobal.m_pBag->GetItem(index);
	if (item && item->m_data && item->m_data->sellable > 0 && !item->GetLock())
		return true;
	return false;
}		

void CMenuItemProcess::ShowIdentifyDiag(UInt identifyItemSlot, UInt identifyToolSlot)
{
	SetIdentify(identifyItemSlot, identifyToolSlot);
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("IdentifyItemPanel");
	if (panel == NULL)
	{
		XML_UI::GetInstance().ReadFormFile("Data\\IdentifyItem.xml", gGlobal.pUIManager);
		panel = gGlobal.pUIManager->FindUIPanel("IdentifyItemPanel");
		if (panel == NULL)
			return;
	}

	CItem* identifyItem = gGlobal.m_pBag->GetItem(identifyItemSlot);
	CItem* identifyTool = gGlobal.m_pBag->GetItem(identifyToolSlot);
	UIStaticText* textField = gGlobal.pUIManager->FindUIStaticText("IdentifyItemText");
	if (identifyItem && identifyTool && textField && identifyItem->m_data && identifyTool->m_data)
	{
		String text;
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_ITEM_CONFIRM_IDENTIFY")), identifyTool->m_data->name, identifyItem->m_data->name);
		textField->SetText(text.c_str());
		textField->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(textField);
		gGlobal.pUIManager->SetMonoPanel(panel);
		if (gGlobal.g_pCharacter)
			gGlobal.g_pCharacter->Stop();
	}
}

void CMenuItemProcess::IdentifyItem()
{
	if (m_iIdentifyItemSlot < BAG_ITEM_COUNT && m_iIdentifyToolItemSlot < BAG_ITEM_COUNT && gGlobal.m_pBag)
		gGlobal.m_pBag->IdentifyItem(gGlobal.pPeerProxy, RPCContext(), m_iIdentifyItemSlot, m_iIdentifyToolItemSlot);
}

void CMenuItemProcess::LockItem(UInt32 bagIndex, UInt32 assetType)
{
	if (CanPerformUnlockORLock(bagIndex, assetType, BOOLEAN_TRUE))
	{
		if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
			gGlobal.g_pCharacter->GetCharNetObject()->LockAsset(gGlobal.pPeerProxy, gGlobal.g_context, bagIndex, assetType);
	}
}

void CMenuItemProcess::UnlockItem(UInt32 bagIndex, UInt32 assetType)
{
	if (CanPerformUnlockORLock(bagIndex, assetType, BOOLEAN_FALSE))
	{
		if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
			gGlobal.g_pCharacter->GetCharNetObject()->UnlockAsset(gGlobal.pPeerProxy, gGlobal.g_context, m_hashed_empty_pw, bagIndex, assetType);
	}
}

void CMenuItemProcess::ForceResetAssetPassword()
{
    if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
        gGlobal.g_pCharacter->GetCharNetObject()->ForceResetAssetPassword(gGlobal.pPeerProxy, gGlobal.g_context);	
    
}

void CMenuItemProcess::OpenUnlockItemPanel(UInt32 index, UInt32 assetType)
{
	if (CanPerformUnlockORLock(index, assetType, BOOLEAN_FALSE))
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenAssetUnLockPanel", String(), index, assetType);
		UInt8 count = 2;
		StringA tempButtonName;
		tempButtonName.Format("ClearAssetPasswordButton");
		UIButton* forceUnlockButton = (UIButton*)gGlobal.pUIManager->GetUIItem(tempButtonName.c_str());
		if (forceUnlockButton)
		{
			if (gGlobal.g_pCharacter->GetCharNetObject()->GetUnlock_time() != 0)
			{
				forceUnlockButton->SetState(_ButtonDisable);
				count = 3;	
			}
			else
				forceUnlockButton->SetEnable(true);
		}
		for (int i = 1; i <= count; ++i) // display the tips to user at the center of unlock panel 
		{
			StringA chatPanelName;
			chatPanelName.Format("AssetUnLockTip%i", i);
			UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel(chatPanelName.c_str());
			if (pChatPanel)
			{
				String tipName;
				tipName.Format(_T("MSG_ASSET_LOCK_TIPS%d"), i + 2);
				pChatPanel->ClearAllRow();
				String tips;
				if (i != 3)
                    tips  = gGlobal.GetStringTable()->Get(tipName.c_str());		
				else
				{
					CCharacter *pchar = gGlobal.g_pCharacter->GetCharNetObject();
					UInt32 timeNow = (UInt32) time(NULL);	
                    UInt32 timeLeft = pchar->GetUnlock_time() > timeNow ? pchar->GetUnlock_time() - timeNow : 0;
					UInt16 day = timeLeft/(60*60*24);
					UInt16 hour = (timeLeft - (day * (60*60*24)))/(60*60);
					UInt16 minute = (timeLeft - (day * (60*60*24)) - (hour * (60*60)))/60;
					tips.Format(gGlobal.GetStringTable()->Get(tipName.c_str()), day, hour, minute);
				}
				pChatPanel->AddStringW(tips.c_str());	
				pChatPanel->EndRow();
				gGlobal.pUIManager->AddDirtyUIItem(pChatPanel); 	
			}
		}
		
		if (m_bUnlockPassEnter)
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_ITEM_LOCK_PASSWORD_INVALID")));
			m_bUnlockPassEnter = BOOLEAN_FALSE;
		}
	}
}


void CMenuItemProcess::ConfirmAssetUnLockPassword(Int index, UInt32 assetType)
{
	UIEditBox* passwordEdit = gGlobal.pUIManager->FindUIEditBox("AssetUnLockPasswordEdit");
	if ((passwordEdit) && CanPerformUnlockORLock(index, assetType, BOOLEAN_FALSE))
	{
		CONST ByteArray& inputPassword = passwordEdit->GetHashedPassword();
		String strPassword(passwordEdit->GetText());
		if ((strPassword.size() < 4) || (strPassword.size() > 12))
		{
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_ITEM_LOCK_PASSWORD_LENGTH_INVALID")));
				return;
		}
		CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
        pchar->UnlockAsset(gGlobal.pPeerProxy, gGlobal.g_context, inputPassword, index, assetType);
		m_bUnlockPassEnter = BOOLEAN_TRUE;
	}
}

void CMenuItemProcess::ConfirmAssetLockPassword(Int lockType)
{
	switch(lockType)
	{
	case 0:
		{
			UIEditBox* newPasswordEdit1 = gGlobal.pUIManager->FindUIEditBox("AssetNewLockPasswordEdit11");
			UIEditBox* newPasswordEdit2 = gGlobal.pUIManager->FindUIEditBox("AssetNewLockPasswordEdit12");	

			if (newPasswordEdit1 && newPasswordEdit2)
			{
				CONST ByteArray& newPassword1 = newPasswordEdit1->GetHashedPassword();
				CONST ByteArray& newPassword2 = newPasswordEdit2->GetHashedPassword();
				String strPassword1(newPasswordEdit1->GetText());
				String strPassword2(newPasswordEdit2->GetText());	

				if ((strPassword1.size() < 4) || (strPassword2.size() < 4) || (strPassword1.size() > 12 ) || (strPassword2.size() > 12) ||
					(newPassword1 != newPassword2))
				{
						gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_ITEM_LOCK_PASSWORD_LENGTH_INVALID")));
						return;
				}
				CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();

				if ((pchar) && (pchar->GetAction() == CCharacter::ST_NOBATTLE || pchar->GetAction() == CCharacter::ST_STOP ||
					pchar->GetAction() == CCharacter::ST_MOVE || pchar->GetAction() == CCharacter::ST_MAP_TALKING))
				{
					pchar->SetAssetPassword(gGlobal.pPeerProxy, gGlobal.g_context, newPassword1); 
				}
			}
		}
		break;
	case 1:
		{
			UIEditBox* oldPasswordEdit1 = gGlobal.pUIManager->FindUIEditBox("AssetOldLockPasswordEdit");
			UIEditBox* newPasswordEdit2 = gGlobal.pUIManager->FindUIEditBox("AssetNewLockPasswordEdit21");	
			UIEditBox* newPasswordEdit3 = gGlobal.pUIManager->FindUIEditBox("AssetNewLockPasswordEdit22");	
			if (oldPasswordEdit1 && newPasswordEdit2 && newPasswordEdit3)
			{
				CONST ByteArray& oldPassword1 = oldPasswordEdit1->GetHashedPassword();
				CONST ByteArray& newPassword2 = newPasswordEdit2->GetHashedPassword();
				CONST ByteArray& newPassword3 = newPasswordEdit3->GetHashedPassword();
				String strPassword1(oldPasswordEdit1->GetText());
				String strPassword2(newPasswordEdit2->GetText());
				String strPassword3(newPasswordEdit3->GetText());

				if ((strPassword1.size() < 4) || (strPassword2.size() < 4) || (strPassword3.size() < 4) 
					|| (strPassword1.size() > 12 ) || (strPassword2.size() > 12) || (strPassword3.size() > 12) ||
					(newPassword2 != newPassword3))
				{
						gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_ITEM_LOCK_PASSWORD_LENGTH_INVALID")));	
						return;
				}
				CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
				if ((pchar) && (pchar->GetAction() == CCharacter::ST_NOBATTLE || pchar->GetAction() == CCharacter::ST_STOP ||
					pchar->GetAction() == CCharacter::ST_MOVE || pchar->GetAction() == CCharacter::ST_MAP_TALKING))
				{	
					pchar->ChangeAssetPassword(gGlobal.pPeerProxy, gGlobal.g_context, oldPassword1, newPassword2); 
				}
			}
		}
		break;
	}

}

bool CMenuItemProcess::IsPartnerLocked(UInt32 partnerID)
{
	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(partnerID);
	if (partner)
		return partner->GetLock();
	return false;
	
}

bool CMenuItemProcess::IsPetLocked(UInt32 petID)
{
	CPet* pet = gGlobal.g_PetLst->GetPet(petID);
	if (pet)
		return pet->GetLock();
	return false;
	
}


Boolean CMenuItemProcess::CanPerformUnlockORLock(UInt32 index, UInt32 assetType, Boolean locktype)
{
	if (!gGlobal.g_pCharacter|| !gGlobal.m_pBag || !gGlobal.g_pPartnerGroup || !gGlobal.g_PetLst)
		return BOOLEAN_FALSE;

	switch(assetType)
	{
	case ASSET_ITEM:
		{
			CItem* pItem = gGlobal.m_pBag->GetItem(index);
			if (!pItem)
				return BOOLEAN_FALSE;	
			if (pItem->GetLock() == locktype)
			{
				if (pItem->GetLock())
					ShowMessage("MSG_ITEM_LOCK_ALREADY");
				else
                    ShowMessage("MSG_ITEM_UNLOCK_ALREADY");
				return BOOLEAN_FALSE;
			}
			if (pItem->IsGiven())
				return BOOLEAN_FALSE;
		}
		break;
	case ASSET_PARTNER:
		{
			CPartner* pPartner = gGlobal.g_pPartnerGroup->GetPartnerByIndex(index);
			if (!pPartner)
				return BOOLEAN_FALSE;	
			if (pPartner->GetLock() == locktype)
			{
				if (pPartner->GetLock())
					ShowMessage("MSG_PARTNER_LOCK_ALREADY");
				else
					ShowMessage("MSG_PARTNER_UNLOCK_ALREADY");
				return BOOLEAN_FALSE;
			}
			if (pPartner->IsGiven())
				return BOOLEAN_FALSE;
		}
		break;
	case ASSET_PET:
		{
			CPet* pPet = gGlobal.g_PetLst->GetPetByIndex(index);
			if (!pPet)
				return BOOLEAN_FALSE;	
			if (pPet->GetLock() == locktype)
			{
				if (pPet->GetLock())
					ShowMessage("MSG_PET_LOCK_ALREADY");
                else
					ShowMessage("MSG_PET_UNLOCK_ALREADY");
				return BOOLEAN_FALSE;
			}
			if (pPet->IsGiven())
				return BOOLEAN_FALSE;
		}
		break;
	}
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();

	if ((pchar) && (pchar->GetAction() == CCharacter::ST_NOBATTLE || pchar->GetAction() == CCharacter::ST_STOP || 
		pchar->GetAction() == CCharacter::ST_MOVE || pchar->GetAction() == CCharacter::ST_MAP_TALKING))
	{
		if (assetType == ASSET_BANKMONEY){
			if (pchar->GetBankMoney() == 0){
				ShowMessage("MSG_BANKMONEY_LOCK_FAIL");
				return BOOLEAN_FALSE;}	
			if (pchar->GetBankLock() == locktype){
				if (pchar->GetBankLock())
					ShowMessage("MSG_BANKMONEY_ALREADY_LOCK");
				else
                    ShowMessage("MSG_BANKMONEY_ALREADY_UNLOCK");
				return BOOLEAN_FALSE;}
		}
		else if (assetType == ASSET_YUANBAO){
			if (pchar->GetYuanBao() == 0){
				ShowMessage("MSG_YUANBAO_LOCK_FAIL");
				return BOOLEAN_FALSE;}
			if (pchar->GetYuanbaoLock() == locktype){
				if (pchar->GetYuanbaoLock())
					ShowMessage("MSG_YUANBAO_ALREADY_LOCK");
				else
                    ShowMessage("MSG_YUANBAO_ALREADY_UNLOCK");
				return BOOLEAN_FALSE;}
		}
			
		UIPanel* givenPanel = gGlobal.pUIManager->FindUIPanel("GivenItemPanel");
		if (givenPanel)
		{
			ShowMessage("MSG_ITEM_GIVEN_CANT_USE");
			return BOOLEAN_FALSE;
		}
		UIPanel* tradingPanel = gGlobal.pUIManager->FindUIPanel("TradingPanel");
		if (tradingPanel)
		{
			ShowMessage("MSG_ITEM_TRADING_CANT_USE");
			return BOOLEAN_FALSE;
		}

		UIPanel* hawkerPanel = gGlobal.pUIManager->FindUIPanel("HawkerOwnerPanel");
		UIPanel* hawkerPanel1 = gGlobal.pUIManager->FindUIPanel("HawkPanel");
		if (hawkerPanel || hawkerPanel1 || gGlobal.g_pHawkerProcess->IsStartHawk())
		{
			ShowMessage("MSG_HAWK_CANT_USE_ITEM");
			return BOOLEAN_FALSE;
		}
		return BOOLEAN_TRUE;
	}

	return BOOLEAN_FALSE;	
}

void CMenuItemProcess::JpackBag(UInt8 bagType)
{
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	UIPanel* pPanelTrading = gGlobal.pUIManager->FindUIPanel("TradingPanel");
	UIPanel* pPanelGiven = gGlobal.pUIManager->FindUIPanel("GivenItemPanel");
	UIPanel* pPanelHawker = gGlobal.pUIManager->FindUIPanel("HawkerOwnerPanel"); 
	if(pchar)
	{
		if (pchar->GetAction() == CCharacter::ST_HAWK || pPanelHawker)
			ShowMessage("MSG_JPACK_NOT_HAWK");
		else if (pPanelTrading)
			ShowMessage("MSG_JPACK_NOT_TARING");
		else if (pPanelGiven)
			ShowMessage("MSG_JPACK_NOT_GIVEN");
		else if (gGlobal.g_pMenuMixProcess && gGlobal.g_pMenuMixProcess->GetMixShopState())
			ShowMessage("MSG_JPACK_MIXSHOP_ISOPEN");
		else
		{
			if (gGlobal.m_pBag)
			{
				gGlobal.m_pBag->Jpack(gGlobal.pPeerProxy,gGlobal.g_context,bagType);	
			}
		}
	}

}
