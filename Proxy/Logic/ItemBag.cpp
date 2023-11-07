//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/ItemBag.h"
//-- Library
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/QuestLog.h"
#include "Proxy/Logic/QuestSystem.h"
#include "QGL/Global.h"
#include "QGL/CharacterControl.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"
#include "QGL/MenuItemProcess.h"
#include "QGL/ScriptControl.h"
#include "QGL/MenuCharProcess.h"
#include "QGL/MenuMixProcess.h"
#include "QGL/MenuHawkerProcess.h"
#include "Common/CallLuaFunc.h"
#include "QGL/MenuWareHouseProcess.h"
#include "Resource/ResItemSet.h"
#include "QGL/MenuShopProcess.h"

REGISTER_CLASSTYPE(CItemBag, ItemBag_CLASSID);

RPCResult CItemBag::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Error: errcode %d, %s"), errCode, errMsg.c_str());

	#ifdef QGL
		gGlobal.g_pItemProcess->SetSoundIndex(-1);
		gGlobal.g_pItemProcess->UpdateItemMenu();
		gGlobal.g_pItemProcess->ShowMessage(errMsg.c_str());

		if (errCode == ITEM_MSG_REACH_LIMIT)
		{
			String showMsg =gGlobal.GetStringTable()->Get(_T("MSG_ITEM_REACH_LIMIT"));
			gGlobal.PrintPlayerMessage(showMsg);
			gGlobal.PrintMessage(showMsg, CHANNELTYPE_SYSTEM);
		}
	#endif

	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_SyncItems(LPCPEER pPeer, RPCContext &context, NetGroup * items)
{
	SetItems(items);
	DeleteNetGroup(items);

	for(Int i = 0; i < 10; ++i)
	{
		if(gGlobal.g_ShortCutSlot[i].m_RefType == 1 && gGlobal.g_ShortCutSlot[i].m_RefNum >= 0 && gGlobal.g_ShortCutSlot[i].m_RefNum < 24)
		{
			gGlobal.g_pItemProcess->WriteToShortCut(gGlobal.g_ShortCutSlot[i].m_RefNum, i);
		}
	}

	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_EqItemGiven(LPCPEER pPeer, RPCContext &context, const UInt32Vector &givenList)
{
	for (UInt8 i = 0; i < givenList.size(); ++i)
	{
		for (UInt8 j = 0; j < BAG_MAX_ITEM ; ++j)
		{
			if (m_items[j] && m_items[j]->GetItem_uid() == givenList[i]) 
			{
				DeleteNetObject(m_items[j]);
				m_items[j] = NULL;
				break;
			}
		}
	}

	gGlobal.g_pItemProcess->UpdateItemMenu(); 
	gGlobal.g_pHawkerProcess->UpdateOwnerItem();
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_ItemDetail(LPCPEER pPeer, RPCContext &context, NetObject * item)
{
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_ItemCount(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, 
								 const UInt8 &stack, const UInt8 &remain)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CB_ItemCount: index %d, stack %d, remain %d"), 
		bagIndex, stack, remain);

	if (bagIndex < BAG_MAX_SIZE && m_items[bagIndex]) 
	{
		CItem *item = m_items[bagIndex];
		item->SetStack(stack);
		item->SetRemain(remain);
	}

#ifdef QGL
	gGlobal.UpdateBattleItemMenu(BOOLEAN_TRUE, bagIndex);
	//gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_ITEM_USED"),
	//	CHANNELTYPE_SYSTEM);
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_BuytItemSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &cost)
{
	const ItemData* pItemData = gGlobal.m_resource->GetItemData( itemId );
	if ( pItemData )
	{
		String showMsg, tempstr;
		tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_BUY_SUCCESS"));
		showMsg.Format(tempstr.c_str(), (pItemData->name), cost);
		gGlobal.PrintPlayerMessage(showMsg);
		gGlobal.PrintMessage(showMsg, CHANNELTYPE_SYSTEM);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayBuyItemEffect", String());
	}
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_SelltItemSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &point)
{
	const ItemData* pItemData = gGlobal.m_resource->GetItemData( itemId );
	if ( pItemData )
	{
		String showMsg, tempstr;
		tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_SELL_SUCCESS"));
		showMsg.Format(tempstr.c_str(), (pItemData->name), point);
		gGlobal.PrintPlayerMessage(showMsg);
		gGlobal.PrintMessage(showMsg, CHANNELTYPE_SYSTEM);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlaySellItemEffect", String());
	}
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_ItemUseResult(LPCPEER pPeer, RPCContext &context, const Boolean &bRes)
{
	if ( gGlobal.g_pScriptControl && gGlobal.g_pScriptControl->IsItemScriptStandBy() )
	{
		gGlobal.g_pScriptControl->CancelItemScriptStandBy();
		gGlobal.g_pScriptControl->OnEndScriptHandle();
		gGlobal.g_pScriptControl->ResetMouseState();
		gGlobal.g_pScriptControl->OnLockPosHandle(BOOLEAN_FALSE);
	}

	if (!bRes)
	{
		gGlobal.g_pItemProcess->SetSoundIndex(-1);
		gGlobal.g_pItemProcess->UpdateItemMenu();
	}
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_MakeItemMix(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode, const UInt32Vector &item_uid, const UInt8Vector &reduceStacks)
{
	if (gGlobal.g_pMenuMixProcess)
		gGlobal.g_pMenuMixProcess->ShowMixShopResult(success, msgCode, item_uid, reduceStacks);
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_MarkMixStartTime(LPCPEER pPeer, RPCContext &context, const Boolean &startOrCancel)
{
	if (gGlobal.g_pMenuMixProcess && startOrCancel)
		gGlobal.g_pMenuMixProcess->StartTimeLine();
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_MixItem(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode)
{
	UInt32Vector dummy1;
	UInt8Vector dummy2;
	if (gGlobal.g_pMenuMixProcess)
		gGlobal.g_pMenuMixProcess->ShowMixShopResult(success, msgCode, dummy1, dummy2);
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_IdentifyItem(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const UInt8 &itemSlot, const UInt8 &identifyItemSlot)
{
	String msg;
	switch (index)
	{
	case IDENTIFY_CAN_IDENTIFY:
		{
			if (gGlobal.g_pItemProcess)
				gGlobal.g_pItemProcess->ShowIdentifyDiag(itemSlot, identifyItemSlot);
		}
		break;
	case IDENTIFY_NO_IDENTIFY_ITEM:
		{
			CItem* item = GetItem(itemSlot);
			if (item && item->m_data)
			{
				UInt level = item->m_data->reqLv;
				UInt mod = item->m_data->reqLv % 10;
				if (mod > 0 )
					level = level - mod + 10;
				msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_IDENTIFY_NO_IDENTIFYITEM")), level);

				gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
				gGlobal.PrintPlayerMessage(msg);
			}
		}
		break;
	case IDENTIFY_SUCCESS:
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_IDENTIFY_SUCCESS")), CHANNELTYPE_SYSTEM);
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_IDENTIFY_SUCCESS")));
		}
		break;
	case IDENTIFY_ERROR_ITEM:
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_IDENTIFY_ALREADY_IDENTIFY")), CHANNELTYPE_SYSTEM);
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_IDENTIFY_ALREADY_IDENTIFY")));
		}
		break;
	case IDENTIFY_NO_ITEM:
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_IDENTIFY_NO_ITEM")), CHANNELTYPE_SYSTEM);
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_IDENTIFY_NO_ITEM")));
		}
		break;
	case IDENTIFY_ALREADY_IDENTIFY:
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_IDENTIFY_ALREADY_IDENTIFY")), CHANNELTYPE_SYSTEM);
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_IDENTIFY_ALREADY_IDENTIFY")));
		}
		break;
	case IDENTIFY_FAIL_REQ:
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_IDENTIFY_FAIL_REQ")), CHANNELTYPE_SYSTEM);
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_IDENTIFY_FAIL_REQ")));
		}
		break;
	}
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex)
{
	gGlobal.g_pWareHouseProcess->ShowResult(processIndex, msgIndex);
	if (msgIndex != WAREHOUSE_SUCCESS)
	{
		gGlobal.g_pWareHouseProcess->UpdateWareHouse();
		gGlobal.g_pItemProcess->UpdateItemMenu();
		
	}
	return RPC_RESULT_OK;
}

CItemBag::CItemBag()
{
	m_items = SafeAllocate(CItem*, BAG_MAX_SIZE);
	TRACE_ENSURE(m_items);							// ensure allocate successful
	ZeroMemory(m_items, BAG_MAX_SIZE * sizeof(CItem*));

	m_EquipItemSetInfoMap.clear();
}

CItemBag::~CItemBag()
{
	for (int i = 0; i < BAG_MAX_SIZE; i++)
	{
		if (m_items[i])
		{
			DeleteNetObject(m_items[i]);
		}
	}
	SafeDeallocate(m_items);

	m_EquipItemSetInfoMap.clear();
}

Boolean CItemBag::IsBagEnabled(UInt8 bagType)
{
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject() && 
		bagType < (BAG_BACKPACK + gGlobal.g_pCharacter->GetCharNetObject()->GetAvailBackPack()) )
		return BOOLEAN_TRUE;

	return BOOLEAN_FALSE;
}

CItem* CItemBag::GetItem(UInt8 bagIndex)
{
	if (bagIndex < BAG_MAX_SIZE)
	{
		CItem* pItem = m_items[bagIndex];
		if (pItem && pItem->IsExist())
			return pItem;
	}
	return NULL;
}

CItem* CItemBag::FindItemById(UInt32 item_id, UInt8 bagType, UInt8 startIdx)
{
	if (bagType >= BAG_TYPE_COUNT)
		return NULL;

	CItem* item = NULL;
	for (int i = (startIdx == 0 ? bagType * BAG_ITEM_COUNT : startIdx);	// start at give index or not
		i < (bagType + 1) * BAG_ITEM_COUNT; i++)
	{
		item = GetItem(i);
		if (item)
		{
			if (item->GetItem_id() == item_id && item->GetItem_uid() != 0)
				return item;
			else if (item->GetItem_uid() == 0)
				TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("(bag item uid is zero) pItem %d, item_id %d, bag index %d"), item, item->GetItem_id(), i);
		}
	}

	return NULL;
}

UInt8 CItemBag::FindBagIndex(CItem* item)
{
	for (UInt8 i = 0; i < BAG_MAX_SIZE; i++)
	{
		if (m_items[i] == item)
			return i;
	}

	return 255;
}

void CItemBag::SetItems(NetGroup *itemGroup)
{
	bool related = false;
	for (UInt16 i = 0; i < itemGroup->GetCount(); i++)
	{
		CItem* item = itemGroup->GetChild<CItem>(i);
		if (item == NULL || item->GetBagIndex() >= BAG_MAX_SIZE)
			continue;

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("ItemBag: bagIndex %d, item_uid %d, item_id %d, stack %d"), 
			item->GetBagIndex(), item->GetItem_uid(), item->GetItem_id(), item->GetStack());
		UInt8 bagIndex = item->GetBagIndex();
		if (gGlobal.g_QuestSystem && 
			gGlobal.g_QuestSystem->IsRelated(Q_TARGET_ITEM, item->GetItem_id()))
			related = true;
		if (item->GetStack() > 0)
		{
			if (!item->IsUpdated(CItem::inWareHouseMask))
			{
				if (item->GetInWareHouse() == 0)
				{
					// move item
					if (item->IsUpdated(CItem::bagIndexMask))
					{
						UInt8 oldIndex = FindBagIndex(item);
						if (oldIndex < BAG_MAX_SIZE)
							m_items[oldIndex] = NULL;
					}
					m_items[bagIndex] = item;
				}
				else
				{
					if (!item->IsUpdated(CItem::idMask) && item->IsUpdated(CItem::bagIndexMask))
						gGlobal.g_pWareHouseProcess->RemoveItem(item);
					gGlobal.g_pWareHouseProcess->AddItem(item);
				}
			}
			else
			{
				if (item->GetInWareHouse() == 0)
				{
					// move item
					if (!item->IsUpdated(CItem::idMask))
						gGlobal.g_pWareHouseProcess->RemoveItem(item);
					m_items[bagIndex] = item;
				}
				else
				{
					
					UInt8 oldIndex = FindBagIndex(item);
					if (oldIndex < BAG_MAX_SIZE)
						m_items[oldIndex] = NULL;
					
					gGlobal.g_pWareHouseProcess->AddItem(item);
				}
			}
		}
		else
		{
			if (!item->IsUpdated(CItem::inWareHouseMask))
			{
				if (item->GetInWareHouse() == 0)
				{
					if (m_items[bagIndex] && m_items[bagIndex]->GetItem_uid() == item->GetItem_uid())
					{
						DeleteNetObject(m_items[bagIndex]);
						m_items[bagIndex] = NULL;
					}
				}
				else
					gGlobal.g_pWareHouseProcess->RemoveItem(item, true);
			}
		}
	}
	if (related)
		gGlobal.g_QuestSystem->RefreshNpcHint();
	RefreshMenu();
	UpdateItemSet();
}

UInt8 CItemBag::GetCount(UInt8 bagType)
{
	if (bagType >= BAG_TYPE_COUNT)
		return 0;

	UInt8 count = 0;
	for (int i = bagType * BAG_ITEM_COUNT; 
		i < (bagType + 1) * BAG_ITEM_COUNT; i++)
	{
		if (m_items[i] && m_items[i]->IsExist())
			count++;
	}
	return count;
}

UInt8 CItemBag::GetTotalCount()
{
	UInt8 count = 0;
	for (int i = 0; i < BAG_MAX_ITEM; i++)
	{
		if (m_items[i] && m_items[i]->IsExist())
			count++;
	}
	return count;
}

UInt16 CItemBag::GetItemCount(UInt32 item_id)
{
	UInt16 count = 0;
	for (int i = 0; i < BAG_MAX_ITEM; i++)
	{
		if (m_items[i] && m_items[i]->GetItem_id() == item_id)
			count += m_items[i]->GetStack();
	}
	return count;
}

void CItemBag::DebugPrint()
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Inventory Bag: %d, %d"), GetCount(BAG_CURRENT), 
		GetCount(BAG_BACKUP));

	for (int i = 0; i < BAG_MAX_SIZE; i++)
	{
		CItem *item = m_items[i];
		if (item)
		{
			TRACE_INFODTL_6(GLOBAL_LOGGER, _F("%2d(%2d): Item_uid %d, item_id %d, stack %d, remain %d"),
				i, item->GetBagIndex(), item->GetItem_uid(), item->GetItem_id(), 
				item->GetStack(), item->GetRemain());
		}
	}
}

UInt8 CItemBag::FindEmptyIndex(UInt8 bagType, UInt8 startIdx)
{
	if (bagType >= BAG_TYPE_COUNT)
		return 255;

	for (int i = (startIdx == 0 ? bagType * BAG_ITEM_COUNT : startIdx);	// start at give index or not
		i < (bagType + 1) * BAG_ITEM_COUNT; i++)
	{
		if (m_items[i] == NULL)// || !m_items[i]->IsExist())
		{
			return i;
		}
	}

	return 255;
}

Boolean CItemBag::HasItem(UInt8 bagIndex)
{
	if (bagIndex < BAG_MAX_SIZE)
		return (m_items[bagIndex] != NULL && m_items[bagIndex]->IsExist());
	else
		return BOOLEAN_FALSE;
}

Boolean CItemBag::SwapItem(UInt8 SrcIndex, UInt8 DesIndex)
{
	if ( HasItem(SrcIndex) ) 
	{
		if (DesIndex < BAG_MAX_SIZE) 
		{
			CItem* pItem = m_items[SrcIndex];
			if ( pItem )
			{
				m_items[SrcIndex] = m_items[DesIndex];
				m_items[DesIndex] = pItem;
				return BOOLEAN_TRUE;
			}
		}
	}
	return BOOLEAN_FALSE;
}


Int CItemBag::CalcItemTotalCount(UInt32 item_id)
{
	Int total = 0;
	for ( UInt8 i = BAG_CURRENT; i < BAG_TYPE_COUNT; ++i )
	{
		if ( IsBagEnabled(i) )
		{
			for ( int j = (i * BAG_ITEM_COUNT); j < ((i + 1) * BAG_ITEM_COUNT); ++j )
			{
				if (m_items[j] && (m_items[j]->GetItem_id() == item_id) )
					total += (Int)(m_items[j]->GetStack());
			}
		}
	}

	return total;
}

bool CItemBag::HasItemByUID(UInt32 uid)
{
	for (UInt16 i = 0; i < BAG_MAX_SIZE; ++i)
	{
		if (m_items[i] && m_items[i]->GetItem_uid() == uid)
			return true;
	}
	return false;
}

CItem* CItemBag::GetItemByUID(UInt32 uid)
{
	for (UInt16 i = 0; i < BAG_MAX_SIZE; ++i)
	{
		if (m_items[i] && m_items[i]->GetItem_uid() == uid)
			return m_items[i];
	}
	return NULL;
}

void CItemBag::RefreshMenu()
{
	gGlobal.g_pItemProcess->UpdateItemMenu(); // Global functions
	gGlobal.g_pCharProcess->UpdateCharAttr1();
	gGlobal.g_pCharProcess->UpdateCharAttr2();
	gGlobal.g_pItemProcess->ShowMessage();
	gGlobal.g_pHawkerProcess->UpdateOwnerItem();
	gGlobal.g_pHawkerProcess->UpdateOwnerEqItem();
	gGlobal.g_pWareHouseProcess->UpdateWareHouse();
}


// ItemSet
void CItemBag::UpdateItemSet()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CItemBag::UpdateItemSet()."));

	bool bNeedUpdateAttrib = false;
	m_EquipItemSetInfoMap.clear();

	//for (UInt i = BAG_MAX_ITEM; i < BAG_MAX_ITEM + BAG_EQUIP_COUNT; i++)
	for (UInt i = 0; i < BAG_MAX_ITEM + BAG_EQUIP_COUNT; i++)
	{
		CItem *item = GetItem(i);
		if (item && item->GetEndure() > 0 && item->m_data)
		{
			UInt32 itemset_id = item->m_data->itemset_id; // if item has itemset ...
			if (itemset_id > 0)
			{
				CItemBag::EquipItemSetInfoMap::iterator itr = m_EquipItemSetInfoMap.find(itemset_id);
				if (itr == m_EquipItemSetInfoMap.end())
				{
					EquipItemSetInfo itemsetinfo;
					itemsetinfo.itemset_id = itemset_id;
					if (i >= BAG_MAX_ITEM)
						itemsetinfo.eq_count = 1;
					else
						itemsetinfo.eq_count = 0;
					itemsetinfo.pItemSetData = gGlobal.m_resource->GetItemSetData(itemset_id);
					if (!itemsetinfo.pItemSetData)
					{
						TRACE_INFODTL(GLOBAL_LOGGER, _F("UpdateItemSet(), can not get itemsetdata."));
					}
					m_EquipItemSetInfoMap.insert(std::make_pair(itemset_id, itemsetinfo));

					bNeedUpdateAttrib = true;
				}
				else
				{
					if (i >= BAG_MAX_ITEM) // equip
					{
						(itr)->second.eq_count ++;
						bNeedUpdateAttrib = true;
					}
				}
			}
		}
	}
	if (bNeedUpdateAttrib)
	{
		gGlobal.g_pCharProcess->UpdateCharAttr2();
	}
}

EquipItemSetInfo* CItemBag::GetItemSetInfo(UInt32 itemset_id)
{
	CItemBag::EquipItemSetInfoMap::iterator itr = m_EquipItemSetInfoMap.find(itemset_id);
	if (itr != m_EquipItemSetInfoMap.end())
		return &(itr->second);
	return NULL;
}

void CItemBag::CalcItemSetBasicAttrib(Int16& sta, Int16& spr, Int16& str, Int16& con, Int16& agi)
{
	// increate STA, SPR, STR, CON, AGI by ItemSet
	if (m_EquipItemSetInfoMap.size() > 0)
	{
		CItemBag::EquipItemSetInfoMap::iterator itr = m_EquipItemSetInfoMap.begin();
		while (itr != m_EquipItemSetInfoMap.end())
		{
			const ItemSetData* pItemSetData = (itr)->second.pItemSetData;
			if (pItemSetData && (itr)->second.eq_count >= pItemSetData->count)
			{
				sta += pItemSetData->STA;
				spr += pItemSetData->SPR;
				str += pItemSetData->STR;
				con += pItemSetData->CON;
				agi += pItemSetData->AGI;
			}

			itr++;
		}
	}
}

UInt32 CItemBag::GetBusinessItemCount()
{
	UInt32 TotalCount = 0;
	for ( UInt8 i = BAG_CURRENT; i < BAG_TYPE_COUNT; ++i )
	{
		if ( IsBagEnabled(i) )
		{
			for ( int j = (i * BAG_ITEM_COUNT); j < ((i + 1) * BAG_ITEM_COUNT); ++j )
			{
				if (m_items[j] && ITEMTYPE_BUSINESS_ITEM == m_items[j]->GetItemType() )
				{
					TotalCount += m_items[j]->GetStack();
				}
			}
		}
	}
	return TotalCount;
}

RPCResult CItemBag::CB_BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &cost)
{
#ifdef QGL
	const ItemData* pItemData = gGlobal.m_resource->GetItemData( itemId );
	if ( pItemData )
	{
		String showMsg, tempstr;
		tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_BUY_SUCCESS"));
		showMsg.Format(tempstr.c_str(), (pItemData->name), cost);
		gGlobal.PrintPlayerMessage(showMsg);
		gGlobal.PrintMessage(showMsg, CHANNELTYPE_SYSTEM);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayBuyItemEffect", String());
		if (gGlobal.g_pShopProcess)
		{
			gGlobal.g_pShopProcess->ChangeBusinessShopMoney(-(Int32)cost);
		}
	}
#endif
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &getMoney, const Boolean &bEnough)
{
#ifdef QGL
	const ItemData* pItemData = gGlobal.m_resource->GetItemData( itemId );
	if ( pItemData )
	{
		String showMsg, tempstr;
		tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SHOP_SELL_SUCCESS"));
		showMsg.Format(tempstr.c_str(), (pItemData->name), getMoney);
		gGlobal.PrintPlayerMessage(showMsg);
		gGlobal.PrintMessage(showMsg, CHANNELTYPE_SYSTEM);
		if (bEnough)
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BUSINESS_ALREADY_ENOUGH")));
		}
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlaySellItemEffect", String());
		if (gGlobal.g_pShopProcess)
		{
			gGlobal.g_pShopProcess->ChangeBusinessShopMoney((Int32)getMoney);
		}
	}
#endif
	return RPC_RESULT_OK;
}