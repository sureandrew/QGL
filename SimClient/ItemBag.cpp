//-- Common
#include "Common.h"
//-- Self
#include "SimClient/ItemBag.h"
//-- Library
#include "SimClient/Item.h"
#include "SimClient/Global.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"
#include "SimClient/Global.h"

REGISTER_CLASSTYPE(CItemBag, ItemBag_CLASSID);

CItemBag::CItemBag()
{
	m_items = SafeAllocate(CItem*, BAG_MAX_SIZE);
	TRACE_ENSURE(m_items);							// ensure allocate successful
	ZeroMemory(m_items, BAG_MAX_SIZE * sizeof(CItem*));
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
}

CItem* CItemBag::GetItem(UInt8 bagIndex)
{
	if (bagIndex < BAG_MAX_SIZE)
		return m_items[bagIndex];
	else
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
	for (UInt16 i = 0; i < itemGroup->GetCount(); i++)
	{
		CItem* item = itemGroup->GetChild<CItem>(i);
		if (item == NULL || item->GetBagIndex() >= BAG_MAX_SIZE)
			continue;

		if (IsOption(OPT_SHOW_ITEM))
			TRACE_INFODTL_4(GLOBAL_LOGGER, _F("ItemBag: bagIndex %d, item_uid %d, item_id %d, stack %d"), 
				item->GetBagIndex(), item->GetItem_uid(), item->GetItem_id(), item->GetStack());
		UInt8 bagIndex = item->GetBagIndex();
		if (item->GetStack() > 0) {
			// move item
			//if (!item->IsUpdated(CItem::idMask) && item->IsUpdated(CItem::bagIndexMask))
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
			if (m_items[bagIndex] == item) {
				DeleteNetObject(m_items[bagIndex]);
				m_items[bagIndex] = NULL;
			} else {
				UInt8 bi = FindBagIndex(item);
				if (bi != 255) {
					m_items[bi] = NULL;
					TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("ItemBag SetItems Warning: zero-stack item_uid %d, item_id %d not in bagIndex %d, but in %d"), 
						item->GetItem_uid(), item->GetItem_id(), item->GetBagIndex(), bi);
				}
				DeleteNetObject(item);
			}
		}
	}
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

void CItemBag::ShowItems(Boolean bSimple)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Bag: current count %d, backup count %d"), 
		GetCount(BAG_CURRENT), GetCount(BAG_BACKUP));

	String s;
	int count = 0;
	for (int i = 0; i < BAG_MAX_SIZE; i++)
	{
		CItem *item = m_items[i];
		if (item)
		{
			if (bSimple) {
				s.AppendFormat(_T("%2d(%u, %d), "), item->GetBagIndex(), item->GetItem_id(), 
				item->GetStack());
				count++;
				if (count % 5 == 0)
					s.AppendFormat(_T("\n"));
			} else {
				TRACE_INFODTL_6(GLOBAL_LOGGER, 
					_T("%2d(%2d): Item_uid %d, item_id %d, stack %d, remain %d"),
					i, item->GetBagIndex(), item->GetItem_uid(), item->GetItem_id(), 
					item->GetStack(), item->GetRemain());
			}
		}
	}
	if (bSimple)
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Bag: %s"), s.c_str());
}

UInt32 CItemBag::RandItemId()
{
	UInt32 item_id = 0;
	UInt32 total;
	ItemDataList itemList;
	const ItemData* pData;
	bool found = false;
	
	gGlobal.m_resource->GetItemList(itemList);
	total = (UInt32) itemList.size();

	for (int j = 0; j < 100; j++) {					// try only 100 times
		pData = itemList[GetRand((int)total)];
		if (pData == NULL)
			continue;
		found = false;								// find if already exist
		for (int i = 0; i < BAG_MAX_ITEM; i++)
		{
			if (m_items[i] && m_items[i]->GetStack() > 0 &&
				m_items[i]->GetItem_id() == pData->item_id)
			{
				found = true;
				break;
			}
		}
		if (!found) {								// not found then return that id
			item_id = pData->item_id;
			break;
		}
	}

	return item_id;
}

UInt32 CItemBag::GetSlotItemId(UInt8 bagIndex)
{
	if (bagIndex < BAG_MAX_SIZE)
	{
		if ( m_items[bagIndex] )
			return m_items[bagIndex]->GetItem_id();
	}
	return 0;
}

RPCResult CItemBag::MoveItem(LPCPEER pPeer, RPCContext &context, const UInt8 &srcIdx, const UInt8 &dstIdx)
{
	return Parent::MoveItem(pPeer, context, srcIdx, dstIdx);
}

RPCResult CItemBag::DropItem(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, const UInt8 &count)
{
	return Parent::DropItem(pPeer, context, bagIndex, count);
}

RPCResult CItemBag::UseItem(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, const UInt32 &itemId, const Boolean &bScript, const UInt32 &partnerID, const UInt32 &pet_uid)
{
	return Parent::UseItem(pPeer, context, bagIndex, itemId, bScript, partnerID, pet_uid);
}

RPCResult CItemBag::EquipItem(LPCPEER pPeer, RPCContext &context, const UInt8 &srcIdx, const UInt8 &dstIdx)
{
	return Parent::EquipItem(pPeer, context, srcIdx, dstIdx);
}

RPCResult CItemBag::BuyItemFromShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int32 &shopId, const UInt8 &slot, const UInt16 &count)
{
	return Parent::BuyItemFromShop(pPeer, context, itemId, shopId, slot, count);
}

RPCResult CItemBag::SellItemToShop(LPCPEER pPeer, RPCContext &context, const Int32 &shopId, const UInt8 &bagIndex, const UInt8 &count)
{
	return Parent::SellItemToShop(pPeer, context, shopId, bagIndex, count);
}

RPCResult CItemBag::MarkMixStartTime(LPCPEER pPeer, RPCContext &context, const Boolean &startOrCancel)
{
	return Parent::MarkMixStartTime(pPeer, context, startOrCancel);
}

RPCResult CItemBag::MakeItemMix(LPCPEER pPeer, RPCContext &context, const UInt32 &mixProductID, const UInt32 &mixShopID, const UInt32Vector &itemUids, const UInt8Vector &stacks)
{
	return Parent::MakeItemMix(pPeer, context, mixProductID, mixShopID, itemUids, stacks);
}

RPCResult CItemBag::MixItem(LPCPEER pPeer, RPCContext &context, const UInt32 &mixShopID, const UInt32Vector &itemUids, const UInt8Vector &stacks, const UInt8 &removeGemIndex)
{
	return Parent::MixItem(pPeer, context, mixShopID, itemUids, stacks, removeGemIndex);
}

RPCResult CItemBag::IdentifyItem(LPCPEER pPeer, RPCContext &context, const UInt8 &ItemSlot, const UInt8 &IdentifyItemSlot)
{
	return Parent::IdentifyItem(pPeer, context, ItemSlot, IdentifyItemSlot);
}

RPCResult CItemBag::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("errcode %d, errmsg %s"), errCode, errMsg.c_str());
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_SyncItems(LPCPEER pPeer, RPCContext &context, NetGroup * items)
{
	SetItems(items);
	DeleteNetGroup(items);

	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_ItemCount(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, 
								  const UInt8 &stack, const UInt8 &remain)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CB_ItemCount: index %d, stack %d, remain %d"), 
		bagIndex, stack, remain);

	if (bagIndex < BAG_MAX_SIZE && m_items[bagIndex]) {
		CItem *item = m_items[bagIndex];
		if (stack == 0) {
			m_items[bagIndex] = NULL;
			DeleteNetObject(item);
		} else {
			item->SetStack(stack);
			item->SetRemain(remain);
		}
	}

	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_BuytItemSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &cost)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Successfully purchase! Purchased Item:%d Cost:%d\n"), 
		                itemId, cost);

	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_SelltItemSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &getMoney)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Successfully sold! Sold Item:%d Money earned:%d\n"), 
		                itemId, getMoney);	

	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_ItemUseResult(LPCPEER pPeer, RPCContext &context, const Boolean &bRes)
{
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

	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_MakeItemMix(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode, const UInt32Vector &item_uid, const UInt8Vector &reduceStacks)
{
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_MarkMixStartTime(LPCPEER pPeer, RPCContext &context, const Boolean &startOrCancel)
{
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_MixItem(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode)
{
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_IdentifyItem(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const UInt8 &itemSlot, const UInt8 &identifyItemSlot)
{
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex)
{
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &cost)
{
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CB_SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &getMoney, const Boolean &bEnough)
{
	return RPC_RESULT_OK;
}