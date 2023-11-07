//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/ItemBag_proxy.h"


void ItemBag_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(owner_idMask))
	{
		buf.Read<UInt32>(m.owner_id);
	}
}

void ItemBag_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void ItemBag_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(owner_idMask)) OnUpdateOwner_id(m.owner_id);
}

#pragma optimize("", off)
RPCResult ItemBag_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ItemBag_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_ItemBag_CB_Error:
		{
			UInt32 errCode;
			pBuf->Read<UInt32>(errCode);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, errCode, errMsg);
		}
		break;
	case RPC_ItemBag_CB_SyncItems:
		{
			NetGroup* items;
			pBuf->Read<NetGroup*>(items);
			_result = CB_SyncItems(pPeer, context, items);
		}
		break;
	case RPC_ItemBag_CB_ItemCount:
		{
			UInt8 bagIndex;
			pBuf->Read<UInt8>(bagIndex);
			UInt8 stack;
			pBuf->Read<UInt8>(stack);
			UInt8 remain;
			pBuf->Read<UInt8>(remain);
			_result = CB_ItemCount(pPeer, context, bagIndex, stack, remain);
		}
		break;
	case RPC_ItemBag_CB_BuytItemSuccess:
		{
			UInt32 itemId;
			pBuf->Read<UInt32>(itemId);
			UInt32 cost;
			pBuf->Read<UInt32>(cost);
			_result = CB_BuytItemSuccess(pPeer, context, itemId, cost);
		}
		break;
	case RPC_ItemBag_CB_SelltItemSuccess:
		{
			UInt32 itemId;
			pBuf->Read<UInt32>(itemId);
			UInt32 getMoney;
			pBuf->Read<UInt32>(getMoney);
			_result = CB_SelltItemSuccess(pPeer, context, itemId, getMoney);
		}
		break;
	case RPC_ItemBag_CB_ItemUseResult:
		{
			Boolean bRes;
			pBuf->Read<Boolean>(bRes);
			_result = CB_ItemUseResult(pPeer, context, bRes);
		}
		break;
	case RPC_ItemBag_CB_EqItemGiven:
		{
			UInt32Vector givenList;
			pBuf->Read<UInt32Vector>(givenList);
			_result = CB_EqItemGiven(pPeer, context, givenList);
		}
		break;
	case RPC_ItemBag_CB_MarkMixStartTime:
		{
			Boolean startOrCancel;
			pBuf->Read<Boolean>(startOrCancel);
			_result = CB_MarkMixStartTime(pPeer, context, startOrCancel);
		}
		break;
	case RPC_ItemBag_CB_MakeItemMix:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			UInt8 msgCode;
			pBuf->Read<UInt8>(msgCode);
			UInt32Vector item_uid;
			pBuf->Read<UInt32Vector>(item_uid);
			UInt8Vector reduceStacks;
			pBuf->Read<UInt8Vector>(reduceStacks);
			_result = CB_MakeItemMix(pPeer, context, success, msgCode, item_uid, reduceStacks);
		}
		break;
	case RPC_ItemBag_CB_MixItem:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			UInt8 msgCode;
			pBuf->Read<UInt8>(msgCode);
			_result = CB_MixItem(pPeer, context, success, msgCode);
		}
		break;
	case RPC_ItemBag_CB_IdentifyItem:
		{
			UInt8 index;
			pBuf->Read<UInt8>(index);
			UInt8 itemSlot;
			pBuf->Read<UInt8>(itemSlot);
			UInt8 identifyItemSlot;
			pBuf->Read<UInt8>(identifyItemSlot);
			_result = CB_IdentifyItem(pPeer, context, index, itemSlot, identifyItemSlot);
		}
		break;
	case RPC_ItemBag_CB_WareHouse:
		{
			UInt8 processIndex;
			pBuf->Read<UInt8>(processIndex);
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_WareHouse(pPeer, context, processIndex, msgIndex);
		}
		break;
	case RPC_ItemBag_CB_BuyItemFromBusinessShop:
		{
			UInt32 itemId;
			pBuf->Read<UInt32>(itemId);
			UInt32 cost;
			pBuf->Read<UInt32>(cost);
			_result = CB_BuyItemFromBusinessShop(pPeer, context, itemId, cost);
		}
		break;
	case RPC_ItemBag_CB_SellItemToBusinessShop:
		{
			UInt32 itemId;
			pBuf->Read<UInt32>(itemId);
			UInt32 getMoney;
			pBuf->Read<UInt32>(getMoney);
			Boolean bEnough;
			pBuf->Read<Boolean>(bEnough);
			_result = CB_SellItemToBusinessShop(pPeer, context, itemId, getMoney, bEnough);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult ItemBag_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_ItemBag_UP_STATE:
			_result = RPCDecode<613501223>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_Error:
			_result = RPCDecode<1647133532>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_SyncItems:
			_result = RPCDecode<1349998161>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_ItemCount:
			_result = RPCDecode<1355561297>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_BuytItemSuccess:
			_result = RPCDecode<645411917>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_SelltItemSuccess:
			_result = RPCDecode<950555533>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_ItemUseResult:
			_result = RPCDecode<1372525941>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_EqItemGiven:
			_result = RPCDecode<1096832056>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_MarkMixStartTime:
			_result = RPCDecode<1348863082>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_MakeItemMix:
			_result = RPCDecode<1631455886>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_MixItem:
			_result = RPCDecode<1489919274>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_IdentifyItem:
			_result = RPCDecode<1449733307>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_WareHouse:
			_result = RPCDecode<1716606032>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_BuyItemFromBusinessShop:
			_result = RPCDecode<1999982718>(pPeer, pBuf);
		break;
	case RPC_ItemBag_CB_SellItemToBusinessShop:
			_result = RPCDecode<1634229568>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
