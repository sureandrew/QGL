#pragma once
#ifndef _ITEMBAG_H_
#define _ITEMBAG_H_

#include "Stub/Logic/ItemBag_Stub.h"

#define BAG_TYPE_COUNT		5
#define BAG_ITEM_COUNT		24
#define BAG_EQUIP_COUNT		9
#define BAG_MAX_ITEM		(BAG_TYPE_COUNT * BAG_ITEM_COUNT)
#define BAG_MAX_SIZE		(BAG_MAX_ITEM + BAG_EQUIP_COUNT)
#define BAG_CURRENT			0
#define BAG_BACKUP			1
#define BAG_BACKPACK		2
#define BAG_EQUIPMENT		5
#define DUMMY_ITEM_ADDRESS (reinterpret_cast<CItem*>(1))
#define BAG_MAX_WAREHOUSE	4
#define BAG_MAX_WAREHOUSE_ITEM	(BAG_MAX_WAREHOUSE * BAG_ITEM_COUNT)
#define BAG_MAX_WAREHOUSE_ITEM_PER_PAGE (2 * BAG_ITEM_COUNT)
#define BUSINESSSHOP_MAX_ITEM_COUNT	40
#define BUSINESS_MONEY_LEVEL		4
#define USE_FAST_FOOD_HP			0
#define USE_FAST_FOOD_SP			1
#define ADD_MAX						10000

const UInt32 BusinessMoneyStart[BUSINESS_MONEY_LEVEL]	={20000,	30000,	40000,	40000};
const UInt32 BusinessMoneyFinish[BUSINESS_MONEY_LEVEL]	={50000,	80000,	160000,	180000};

class CCharacter;
class CBatCharacter;
class CItem;
struct MixShopData;

struct ItemSetData;

class CItemBag : public ItemBag_Stub, 
				public Reuben::Simulation::IEventHandler<CItemBag>
{
public:
	DEFINE_CLASS(ItemBag_CLASSID);
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
	virtual RPCResult PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &item_uid, const UInt8 &stack, const UInt8 &desIndex);
	virtual RPCResult GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &item_uid, const UInt8 &stack, const UInt8 &desIndex);
	virtual RPCResult WareHouseItemMove(LPCPEER pPeer, RPCContext &context, const UInt8 &srcIndex, const UInt8 &desIndex);
	virtual RPCResult BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId, const UInt16 &count);
	virtual RPCResult SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const Int16 &shopId, const UInt8 &bagIndex, const UInt16 &count);
	virtual RPCResult UseFastFoodItem(LPCPEER pPeer, RPCContext &context,const UInt8 &useType,const UInt32& partnerID);
	virtual RPCResult Jpack(LPCPEER pPeer, RPCContext &context,const UInt8 &bagType);
	virtual RPCResult CleanWarehouse(LPCPEER pPeer, RPCContext &context,const UInt8 &page);

	CItemBag();
	virtual ~CItemBag();
	INLINE static CItemBag* NewInstance();
	CCharacter* GetOwner() { return m_pOwner; }
	void SetOwner(CCharacter* pchar);
	CItem* Get(int index);					// get index by existing items
	CItem* GetItem(UInt8 bagIndex);
	CItem* GetItemByUID(UInt32 item_uid);
	CItem* GetEquipByUID(UInt32 item_uid);
	CItem* GetItemAndEquipByUID(UInt32 item_uid);
	CItem* FindItemById(UInt32 item_id, UInt8 bagType, UInt8 startIdx = 0);
	
	Boolean FindEmptyIndex(UInt8 bagType, UInt8 &bagIndex, UInt8 startIdx = 0);
	Boolean HasItem(UInt32 item_id);
	Boolean IsSlotFree(UInt8 bagIndex);
	Boolean CanAddItem(UInt8 bagIndex, UInt32 item_id, UInt16 count=1);
	CItem* NewItem(UInt32 item_id, UInt8 count, bool needCheck = true);
	Boolean AddItem(CItem* item, UInt8 bagType, UInt8 quality = 0);
	UInt16 AddItem(UInt32 item_id, UInt16 count=1, UInt16 quality = 0, UInt8 bagIndex=0);
	CItem* AddRandItem(const String &csvFile);
	
	Boolean RemoveItem(CItem* item, Boolean needDelete = BOOLEAN_TRUE);
	Boolean RemoveItem(UInt32 item_id, UInt16 count=1);
	Boolean RemoveItemByUID(UInt32 item_uid, UInt16 count=1);
	Boolean RemoveUnLockItem(UInt32 item_id, UInt16 count=1);	
	Boolean StackItem(CItem* item, UInt8 bagType, UInt8 quality = 0);
	
	Boolean IsEquiped(CONST UInt32 item_id);
	UInt8 GetFreeSlot(UInt8 bagIndex);
	UInt16 GetTotalFreeSlot();
	UInt16 GetItemCount(UInt32 item_id);
	UInt16 GetUnlockItemCount(UInt32 item_id);	

	UInt GetMixShopConsumeMsgIndex(const UInt8Vector &consumeType, const UInt32Vector &consumeAmount, UInt price = 1);
	void ReduceMixShopConsume(const UInt8Vector &consumeType, const UInt32Vector &consumeAmount, UInt price = 1, UInt16 logtype = 0);

	// preprocess of create items
	RPCResult LoadItems(RPCContext &context);		// load items from object service
	RPCResult UpdateItems(RPCContext &context);		// update items to object service
	RPCResult UpdateOwner(RPCContext &context);		// update owner character and also sync items

	Boolean IsBagEnabled(UInt8 bagType);
	
	void PreSend(NetGroup *itemGroup, int count=BAG_MAX_SIZE);		// prepare to send bag attributes and items
	void SendAll();
	void SyncTryCount();							// sync try item count back to client
	void PreBattle();
	void AddToDeleteGroup(UInt8 itemIndex, UInt8 inWareHouse = 0);
	void ValidateItems(const String &header);

	virtual void HandleEvent(CONST Event &event);
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event

	void ReduceEqEnduring();

	void UpdateTurnBuff();
	void UpdateTimeBuff();
	void RemoveBuffAfterOffline();
	
	Boolean CanCreateLimitItem(UInt32 itemID, UInt16 stack);
	void RemoveDummyItem(UInt noOfDummyItem);
	bool SwapFromDummy(CItem* item);

	inline void ResetMixShopStartTime(){ m_iStartMakeTime = 0;}
	UInt8 EquipRepair(UInt32Vector itemUids, UInt8Vector consumeType, UInt32Vector consumeAmount);
	UInt8 StoneMix(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount);

	void ChangeOwnerID(const UInt32Vector &itemUIDs, const UInt32Vector &itemIDs, UInt32 targetOwnerID);
	RPCResult ChangeOwnerID(RPCContext &context);

	void SwapItemByIndex(UInt srcIndex, UInt desIndex);
	void CheckIdentifyItemExist(CItem* item);

	void SetWareHouseItem(NetGroup* grp);
	CItem* GetItemFromWareHouse(UInt bagIndex);
	void GetAllWareHouse(NetGroup* grp);
	void ClearWareHouse();
	Boolean ChangeItemLockStatus(const UInt32 &bagIndex, const Boolean &lock, UInt8 &msg);	
	void ActivateEquipedSkill();
	UInt8 UnSoulBound(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount, const MixShopData* pShop);

	UInt8 GemMix(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount);
	UInt8 GemInlay(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount);
	UInt8 GemRemove(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount, const UInt8 removeGemIndex);
	UInt8 GemMelting(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount);
	UInt8 GemPolishing(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount);
	UInt8 ColorStoneMix(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount);

	void FindFoodItemLstByType(UInt8 itemType,UInt8 useTpye);
	UInt8 FindMinItemInBagIndex();

	void JpackItemSort();
	///////////////////////////////////////
	//          ItemSet Methods          //
	///////////////////////////////////////
	void UpdateItemSet();
	void CalcItemSetBasicAttrib(Int16& sta, Int16& spr, Int16& str, Int16& con, Int16& agi);
	void CalcItemSetBatAttrib(float& atk, float& def, float& hit, float& eva, float& pow, float& spd, UInt16& eq_maxHp, UInt16& eq_maxSp);
	void CalcItemSetBatAttribMaxHpSp(float& maxHp, float& maxSp);
	INLINE void SetWareHouseUpdate(Boolean update){ m_bWareHouseUpdated = update; };

	UInt16 GetBusinessItemCount();
	void RemoveAllBusinessItem();
	UInt32 GetBusinessMoney();
	Boolean ChangeBusinessMoney(Int32 nChange);
	Boolean IsBusinessMoneyEnough();
	Boolean HaveBusinessItemOrMoney();

	enum ItemCreateType
	{
		CREATE_BY_SYSTEM = 0,
		CREATE_BY_EXCHANGE,
		CREATE_BY_WAREHOUSE,
		CREATE_NONE,
	};
private:
	enum UpdateType {
		SYNC_ITEM = 0,
		USE_ITEM,
		EQUIP_ITEM,
	};
	enum EventEnum
	{
		EVT_UPDATE_OBJECT		= MAKE_EVENTID(ItemBag_CLASSID,  1),
	};

	typedef ItemBag_Stub Parent;
	CCharacter* m_pOwner;
	//CItem**		m_items;
	UInt32		m_iStartMakeTime;	// for itemMix
	NetGroup	m_delGroup;
	RPCNetIDVector m_items;
	RPCNetIDVector m_wareHouseItems;
	CItem* m_dummyItem; //for reserve
	Boolean m_bWareHouseUpdated;
	// ItemSet
	typedef StlMap<UInt32, UInt8, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, UInt8> > > EquipItemSetCountMap;
	typedef StlMap<UInt32,const ItemSetData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, const ItemSetData*> > > ItemSetDataMap;

	ItemSetDataMap			m_EquipItemSetMap;

	typedef StlMap<UInt8, CItem*,std::less<UInt8>, ManagedAllocator<std::pair<UInt8, CItem*> > > FoodItemMap;
	FoodItemMap				m_FoodItemMap;

	typedef StlVector<CItem*, ManagedAllocator<CItem*> > ItemVec;
	ItemVec					m_JpackItemVec;
	ItemVec					m_WarehouseItemVec;

};

INLINE CItemBag* CItemBag::NewInstance()
{
	return CreateNetObject<CItemBag>();
}

INLINE void CItemBag::RaiseUpdate()
{
	RaiseUniqEventID(EVT_UPDATE_OBJECT);
}

INLINE void CItemBag::CancelUpdate()
{
	CancelUniqEvent(EVT_UPDATE_OBJECT);
}

#endif //_ITEMBAG_H_
