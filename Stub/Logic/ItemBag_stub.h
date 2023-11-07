///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_ITEMBAG_STUB_H_
#define _RPC_ITEMBAG_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ItemBag_enum.h"

class ItemBag_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	virtual RPCResult UseFastFoodItem(LPCPEER pPeer, RPCContext &context, const UInt8 &useType, const UInt32 &partnerID);
	virtual RPCResult Jpack(LPCPEER pPeer, RPCContext &context, const UInt8 &bagType);
	virtual RPCResult CleanWarehouse(LPCPEER pPeer, RPCContext &context, const UInt8 &page);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		owner_idMask	= Parent::NextFreeMask + 0,
		NextFreeMask	= Parent::NextFreeMask + 1
	};
	virtual RPCResult MoveItem(LPCPEER pPeer, RPCContext &context, const UInt8 &srcIdx, const UInt8 &dstIdx) = 0;
	virtual RPCResult DropItem(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, const UInt8 &count) = 0;
	virtual RPCResult UseItem(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, const UInt32 &itemId, const Boolean &bScript, const UInt32 &partnerID, const UInt32 &pet_uid) = 0;
	virtual RPCResult EquipItem(LPCPEER pPeer, RPCContext &context, const UInt8 &srcIdx, const UInt8 &dstIdx) = 0;
	virtual RPCResult BuyItemFromShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int32 &shopId, const UInt8 &slot, const UInt16 &count) = 0;
	virtual RPCResult SellItemToShop(LPCPEER pPeer, RPCContext &context, const Int32 &shopId, const UInt8 &bagIndex, const UInt8 &count) = 0;
	virtual RPCResult MarkMixStartTime(LPCPEER pPeer, RPCContext &context, const Boolean &startOrCancel) = 0;
	virtual RPCResult MakeItemMix(LPCPEER pPeer, RPCContext &context, const UInt32 &mixProductID, const UInt32 &mixShopID, const UInt32Vector &itemUids, const UInt8Vector &stacks) = 0;
	virtual RPCResult MixItem(LPCPEER pPeer, RPCContext &context, const UInt32 &mixShopID, const UInt32Vector &itemUids, const UInt8Vector &stacks, const UInt8 &removeGemIndex) = 0;
	virtual RPCResult IdentifyItem(LPCPEER pPeer, RPCContext &context, const UInt8 &ItemSlot, const UInt8 &IdentifyItemSlot) = 0;
	virtual RPCResult PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &item_uid, const UInt8 &stack, const UInt8 &desIndex) = 0;
	virtual RPCResult GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &item_uid, const UInt8 &stack, const UInt8 &desIndex) = 0;
	virtual RPCResult WareHouseItemMove(LPCPEER pPeer, RPCContext &context, const UInt8 &srcIndex, const UInt8 &desIndex) = 0;
	virtual RPCResult BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId, const UInt16 &count) = 0;
	virtual RPCResult SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const Int16 &shopId, const UInt8 &bagIndex, const UInt16 &count) = 0;
	virtual RPCResult UseFastFoodItem(LPCPEER pPeer, RPCContext &context, const UInt8 &useType, const UInt32 &partnerID) = 0;
	virtual RPCResult Jpack(LPCPEER pPeer, RPCContext &context, const UInt8 &bagType) = 0;
	virtual RPCResult CleanWarehouse(LPCPEER pPeer, RPCContext &context, const UInt8 &page) = 0;
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_SyncItems(LPCPEER pPeer, RPCContext &context, NetGroup* items);
	virtual RPCResult CB_ItemCount(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, const UInt8 &stack, const UInt8 &remain);
	virtual RPCResult CB_BuytItemSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &cost);
	virtual RPCResult CB_SelltItemSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &getMoney);
	virtual RPCResult CB_ItemUseResult(LPCPEER pPeer, RPCContext &context, const Boolean &bRes);
	virtual RPCResult CB_EqItemGiven(LPCPEER pPeer, RPCContext &context, const UInt32Vector &givenList);
	virtual RPCResult CB_MarkMixStartTime(LPCPEER pPeer, RPCContext &context, const Boolean &startOrCancel);
	virtual RPCResult CB_MakeItemMix(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode, const UInt32Vector &item_uid, const UInt8Vector &reduceStacks);
	virtual RPCResult CB_MixItem(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode);
	virtual RPCResult CB_IdentifyItem(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const UInt8 &itemSlot, const UInt8 &identifyItemSlot);
	virtual RPCResult CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex);
	virtual RPCResult CB_BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &cost);
	virtual RPCResult CB_SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &getMoney, const Boolean &bEnough);
	const UInt32& GetOwner_id() const { return m.owner_id; }
	void SetOwner_id(const UInt32 &owner_id);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//owner_id
	UInt32 owner_id;
} m;

public:
	INLINE ItemBag_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_ITEMBAG_STUB_H_
