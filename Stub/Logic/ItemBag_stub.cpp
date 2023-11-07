//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/ItemBag_stub.h"

void ItemBag_Stub::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(owner_idMask, modified);
}



void ItemBag_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void ItemBag_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(owner_idMask))
	{
		buf.Write<UInt32>(m.owner_id);
	}
}

RPCResult ItemBag_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult ItemBag_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ItemBag_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_ItemBag_MoveItem:
		{
			UInt8 srcIdx;
			pBuf->Read<UInt8>(srcIdx);
			UInt8 dstIdx;
			pBuf->Read<UInt8>(dstIdx);
			_result = MoveItem(pPeer, context, srcIdx, dstIdx);
		}
		break;
	case RPC_ItemBag_DropItem:
		{
			UInt8 bagIndex;
			pBuf->Read<UInt8>(bagIndex);
			UInt8 count;
			pBuf->Read<UInt8>(count);
			_result = DropItem(pPeer, context, bagIndex, count);
		}
		break;
	case RPC_ItemBag_UseItem:
		{
			UInt8 bagIndex;
			pBuf->Read<UInt8>(bagIndex);
			UInt32 itemId;
			pBuf->Read<UInt32>(itemId);
			Boolean bScript;
			pBuf->Read<Boolean>(bScript);
			UInt32 partnerID;
			pBuf->Read<UInt32>(partnerID);
			UInt32 pet_uid;
			pBuf->Read<UInt32>(pet_uid);
			_result = UseItem(pPeer, context, bagIndex, itemId, bScript, partnerID, pet_uid);
		}
		break;
	case RPC_ItemBag_EquipItem:
		{
			UInt8 srcIdx;
			pBuf->Read<UInt8>(srcIdx);
			UInt8 dstIdx;
			pBuf->Read<UInt8>(dstIdx);
			_result = EquipItem(pPeer, context, srcIdx, dstIdx);
		}
		break;
	case RPC_ItemBag_BuyItemFromShop:
		{
			UInt32 itemId;
			pBuf->Read<UInt32>(itemId);
			Int32 shopId;
			pBuf->Read<Int32>(shopId);
			UInt8 slot;
			pBuf->Read<UInt8>(slot);
			UInt16 count;
			pBuf->Read<UInt16>(count);
			_result = BuyItemFromShop(pPeer, context, itemId, shopId, slot, count);
		}
		break;
	case RPC_ItemBag_SellItemToShop:
		{
			Int32 shopId;
			pBuf->Read<Int32>(shopId);
			UInt8 bagIndex;
			pBuf->Read<UInt8>(bagIndex);
			UInt8 count;
			pBuf->Read<UInt8>(count);
			_result = SellItemToShop(pPeer, context, shopId, bagIndex, count);
		}
		break;
	case RPC_ItemBag_MarkMixStartTime:
		{
			Boolean startOrCancel;
			pBuf->Read<Boolean>(startOrCancel);
			_result = MarkMixStartTime(pPeer, context, startOrCancel);
		}
		break;
	case RPC_ItemBag_MakeItemMix:
		{
			UInt32 mixProductID;
			pBuf->Read<UInt32>(mixProductID);
			UInt32 mixShopID;
			pBuf->Read<UInt32>(mixShopID);
			UInt32Vector itemUids;
			pBuf->Read<UInt32Vector>(itemUids);
			UInt8Vector stacks;
			pBuf->Read<UInt8Vector>(stacks);
			_result = MakeItemMix(pPeer, context, mixProductID, mixShopID, itemUids, stacks);
		}
		break;
	case RPC_ItemBag_MixItem:
		{
			UInt32 mixShopID;
			pBuf->Read<UInt32>(mixShopID);
			UInt32Vector itemUids;
			pBuf->Read<UInt32Vector>(itemUids);
			UInt8Vector stacks;
			pBuf->Read<UInt8Vector>(stacks);
			UInt8 removeGemIndex;
			pBuf->Read<UInt8>(removeGemIndex);
			_result = MixItem(pPeer, context, mixShopID, itemUids, stacks, removeGemIndex);
		}
		break;
	case RPC_ItemBag_IdentifyItem:
		{
			UInt8 ItemSlot;
			pBuf->Read<UInt8>(ItemSlot);
			UInt8 IdentifyItemSlot;
			pBuf->Read<UInt8>(IdentifyItemSlot);
			_result = IdentifyItem(pPeer, context, ItemSlot, IdentifyItemSlot);
		}
		break;
	case RPC_ItemBag_PutToWareHouse:
		{
			UInt32 item_uid;
			pBuf->Read<UInt32>(item_uid);
			UInt8 stack;
			pBuf->Read<UInt8>(stack);
			UInt8 desIndex;
			pBuf->Read<UInt8>(desIndex);
			_result = PutToWareHouse(pPeer, context, item_uid, stack, desIndex);
		}
		break;
	case RPC_ItemBag_GetFromWareHouse:
		{
			UInt32 item_uid;
			pBuf->Read<UInt32>(item_uid);
			UInt8 stack;
			pBuf->Read<UInt8>(stack);
			UInt8 desIndex;
			pBuf->Read<UInt8>(desIndex);
			_result = GetFromWareHouse(pPeer, context, item_uid, stack, desIndex);
		}
		break;
	case RPC_ItemBag_WareHouseItemMove:
		{
			UInt8 srcIndex;
			pBuf->Read<UInt8>(srcIndex);
			UInt8 desIndex;
			pBuf->Read<UInt8>(desIndex);
			_result = WareHouseItemMove(pPeer, context, srcIndex, desIndex);
		}
		break;
	case RPC_ItemBag_BuyItemFromBusinessShop:
		{
			UInt32 itemId;
			pBuf->Read<UInt32>(itemId);
			Int16 shopId;
			pBuf->Read<Int16>(shopId);
			UInt16 count;
			pBuf->Read<UInt16>(count);
			_result = BuyItemFromBusinessShop(pPeer, context, itemId, shopId, count);
		}
		break;
	case RPC_ItemBag_SellItemToBusinessShop:
		{
			Int16 shopId;
			pBuf->Read<Int16>(shopId);
			UInt8 bagIndex;
			pBuf->Read<UInt8>(bagIndex);
			UInt16 count;
			pBuf->Read<UInt16>(count);
			_result = SellItemToBusinessShop(pPeer, context, shopId, bagIndex, count);
		}
		break;
	case RPC_ItemBag_UseFastFoodItem:
		{
			UInt8 useType;
			pBuf->Read<UInt8>(useType);
			UInt32 partnerID;
			pBuf->Read<UInt32>(partnerID);
			_result = UseFastFoodItem(pPeer, context, useType, partnerID);
		}
		break;
	case RPC_ItemBag_Jpack:
		{
			UInt8 bagType;
			pBuf->Read<UInt8>(bagType);
			_result = Jpack(pPeer, context, bagType);
		}
		break;
	case RPC_ItemBag_CleanWarehouse:
		{
			UInt8 page;
			pBuf->Read<UInt8>(page);
			_result = CleanWarehouse(pPeer, context, page);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult ItemBag_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_Error);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(errCode);
	pBuf->Write<String>(errMsg);
	return RPCSend<1647133532>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_SyncItems(LPCPEER pPeer, RPCContext &context, NetGroup* items)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_SyncItems);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(items);
	return RPCSend<1349998161>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_ItemCount(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, const UInt8 &stack, const UInt8 &remain)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_ItemCount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(bagIndex);
	pBuf->Write<UInt8>(stack);
	pBuf->Write<UInt8>(remain);
	return RPCSend<1355561297>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_BuytItemSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &cost)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_BuytItemSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(itemId);
	pBuf->Write<UInt32>(cost);
	return RPCSend<645411917>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_SelltItemSuccess(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &getMoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_SelltItemSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(itemId);
	pBuf->Write<UInt32>(getMoney);
	return RPCSend<950555533>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_ItemUseResult(LPCPEER pPeer, RPCContext &context, const Boolean &bRes)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_ItemUseResult);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(bRes);
	return RPCSend<1372525941>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_EqItemGiven(LPCPEER pPeer, RPCContext &context, const UInt32Vector &givenList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_EqItemGiven);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32Vector>(givenList);
	return RPCSend<1096832056>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_MarkMixStartTime(LPCPEER pPeer, RPCContext &context, const Boolean &startOrCancel)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_MarkMixStartTime);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(startOrCancel);
	return RPCSend<1348863082>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_MakeItemMix(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode, const UInt32Vector &item_uid, const UInt8Vector &reduceStacks)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_MakeItemMix);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(success);
	pBuf->Write<UInt8>(msgCode);
	pBuf->Write<UInt32Vector>(item_uid);
	pBuf->Write<UInt8Vector>(reduceStacks);
	return RPCSend<1631455886>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_MixItem(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_MixItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(success);
	pBuf->Write<UInt8>(msgCode);
	return RPCSend<1489919274>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_IdentifyItem(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const UInt8 &itemSlot, const UInt8 &identifyItemSlot)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_IdentifyItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(index);
	pBuf->Write<UInt8>(itemSlot);
	pBuf->Write<UInt8>(identifyItemSlot);
	return RPCSend<1449733307>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_WareHouse);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(processIndex);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend<1716606032>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &cost)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_BuyItemFromBusinessShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(itemId);
	pBuf->Write<UInt32>(cost);
	return RPCSend<1999982718>(pPeer, pBuf);
}

RPCResult ItemBag_Stub::CB_SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const UInt32 &getMoney, const Boolean &bEnough)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemBag_CB_SellItemToBusinessShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(itemId);
	pBuf->Write<UInt32>(getMoney);
	pBuf->Write<Boolean>(bEnough);
	return RPCSend<1634229568>(pPeer, pBuf);
}

