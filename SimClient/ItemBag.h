#pragma once
#ifndef _ITEMBAG_H_
#define _ITEMBAG_H_

#include "Proxy/Logic/ItemBag_Proxy.h"

#define BAG_TYPE_COUNT		5
#define BAG_ITEM_COUNT		24
#define BAG_EQUIP_COUNT		9
#define BAG_MAX_ITEM		(BAG_TYPE_COUNT * BAG_ITEM_COUNT)
#define BAG_MAX_SIZE		(BAG_MAX_ITEM + BAG_EQUIP_COUNT)
#define BAG_CURRENT			0
#define BAG_BACKUP			1
#define BAG_BACKPACK		2
#define BAG_EQUIPMENT		5

class CItem;

class CItemBag : public ItemBag_Proxy
{
private:
	typedef ItemBag_Proxy Parent;
	CItem** m_items;

public:
	DEFINE_CLASS(ItemBag_CLASSID);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_SyncItems(LPCPEER pPeer, RPCContext &context, NetGroup * items);
	virtual RPCResult CB_ItemCount(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, const UInt8 &stack, const UInt8 &remain);
	virtual RPCResult CB_BuytItemSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &cost);
	virtual RPCResult CB_SelltItemSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &getMoney);
	virtual RPCResult CB_ItemUseResult(LPCPEER pPeer, RPCContext &context, const Boolean &bRes);
	virtual RPCResult CB_EqItemGiven(LPCPEER pPeer, RPCContext &context, const UInt32Vector &givenList);
	virtual RPCResult CB_MakeItemMix(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode, const UInt32Vector &item_uid, const UInt8Vector &reduceStacks);
	virtual RPCResult CB_MarkMixStartTime(LPCPEER pPeer, RPCContext &context, const Boolean &startOrCancel);
	virtual RPCResult CB_MixItem(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode);
	virtual RPCResult CB_IdentifyItem(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const UInt8 &itemSlot, const UInt8 &identifyItemSlot);
	virtual RPCResult CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex);
	virtual RPCResult CB_BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &cost);
	virtual RPCResult CB_SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &getMoney, const Boolean &bEnough);

	virtual RPCResult MoveItem(LPCPEER pPeer, RPCContext &context, const UInt8 &srcIdx, const UInt8 &dstIdx);
	virtual RPCResult DropItem(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, const UInt8 &count);
	virtual RPCResult UseItem(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, const UInt32 &itemId, const Boolean &bScript, const UInt32 &partnerID, const UInt32 &pet_uid);
	virtual RPCResult EquipItem(LPCPEER pPeer, RPCContext &context, const UInt8 &srcIdx, const UInt8 &dstIdx);
	virtual RPCResult BuyItemFromShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int32 &shopId, const UInt8 &slot, const UInt16 &count);
	virtual RPCResult SellItemToShop(LPCPEER pPeer, RPCContext &context, const Int32 &shopId, const UInt8 &bagIndex, const UInt8 &count);
	virtual RPCResult MarkMixStartTime(LPCPEER pPeer, RPCContext &context, const Boolean &startOrCancel);
	virtual RPCResult MakeItemMix(LPCPEER pPeer, RPCContext &context, const UInt32 &mixProductID, const UInt32 &mixShopID, const UInt32Vector &itemUids, const UInt8Vector &stacks);
	virtual RPCResult MixItem(LPCPEER pPeer, RPCContext &context, const UInt32 &mixShopID, const UInt32Vector &itemUids, const UInt8Vector &stacks, const UInt8 &removeGemIndex);
	virtual RPCResult IdentifyItem(LPCPEER pPeer, RPCContext &context, const UInt8 &ItemSlot, const UInt8 &IdentifyItemSlot);

	CItemBag();
	virtual ~CItemBag();
	CItem* GetItem(UInt8 bagIndex);
	UInt8 GetCount(UInt8 bagType);
	UInt8 GetTotalCount();
	UInt16 GetItemCount(UInt32 item_id);
	void SetItems(NetGroup *itemGroup);
	UInt8 FindBagIndex(CItem* item);
	void ShowItems(Boolean bSimple=BOOLEAN_FALSE);
	UInt32 GetSlotItemId(UInt8 bagIndex);
	UInt32 RandItemId();
};

#endif //_ITEMBAG_H_
