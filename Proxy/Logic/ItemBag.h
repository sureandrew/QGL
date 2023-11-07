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

struct ItemSetData;

// ItemSet
struct EquipItemSetInfo
{
	UInt32 itemset_id;
	UInt eq_count;
	const ItemSetData* pItemSetData;

	EquipItemSetInfo()
	{
		itemset_id = 0;
		eq_count = 0;
		pItemSetData = NULL;
	}
	~EquipItemSetInfo()
	{
	}
};

class CItemBag : public ItemBag_Proxy
{
private:
	typedef ItemBag_Proxy Parent;
	CItem** m_items;

	typedef StlMap<UInt32,EquipItemSetInfo, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, EquipItemSetInfo> > > EquipItemSetInfoMap;
	EquipItemSetInfoMap		m_EquipItemSetInfoMap;

public:
	DEFINE_CLASS(ItemBag_CLASSID);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_SyncItems(LPCPEER pPeer, RPCContext &context, NetGroup * items);
	virtual RPCResult CB_ItemDetail(LPCPEER pPeer, RPCContext &context, NetObject * item);
	virtual RPCResult CB_ItemCount(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, const UInt8 &stack, const UInt8 &remain);
	virtual RPCResult CB_BuytItemSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &cost);
	virtual RPCResult CB_SelltItemSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &getPoint);
	virtual RPCResult CB_ItemUseResult(LPCPEER pPeer, RPCContext &context, const Boolean &bRes);
	virtual RPCResult CB_EqItemGiven(LPCPEER pPeer, RPCContext &context, const UInt32Vector &givenList);
	virtual RPCResult CB_MakeItemMix(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode, const UInt32Vector &item_uid, const UInt8Vector &reduceStacks);
	virtual RPCResult CB_MarkMixStartTime(LPCPEER pPeer, RPCContext &context, const Boolean &startOrCancel);
	virtual RPCResult CB_MixItem(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode);
	virtual RPCResult CB_IdentifyItem(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const UInt8 &itemSlot, const UInt8 &identifyItemSlot);
	virtual RPCResult CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex);
	virtual RPCResult CB_BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &cost);
	virtual RPCResult CB_SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &getMoney, const Boolean &bEnough);

	CItemBag();
	virtual ~CItemBag();
	CItem* GetItem(UInt8 bagIndex);
	CItem* FindItemById(UInt32 item_id, UInt8 bagType, UInt8 startIdx = 0);
	UInt8 GetCount(UInt8 bagType);
	UInt8 GetTotalCount();
	UInt16 GetItemCount(UInt32 item_id);
	void SetItems(NetGroup *itemGroup);
	UInt8 FindBagIndex(CItem* item);
	void DebugPrint();
	UInt8 FindEmptyIndex(UInt8 bagType, UInt8 startIdx=0);
	Boolean HasItem(UInt8 bagIndex);
	bool HasItemByUID(UInt32 uid);
	CItem*  GetItemByUID(UInt32 uid);

	Int CalcItemTotalCount(UInt32 item_id);

	Boolean SwapItem(UInt8 SrcIndex, UInt8 DesIndex);
	Boolean IsBagEnabled(UInt8 bagType);
	void RefreshMenu();

	// itemset
	void UpdateItemSet();
	EquipItemSetInfo* GetItemSetInfo(UInt32 itemset_id);
	void CalcItemSetBasicAttrib(Int16& sta, Int16& spr, Int16& str, Int16& con, Int16& agi);

	UInt32 GetBusinessItemCount();
};

#endif //_ITEMBAG_H_
