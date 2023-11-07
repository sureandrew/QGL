//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/ChargeShopItem_stub.h"

void ChargeShopItem_Stub::SetIdInfo(const UInt32 &chargeShopID)
{
	bool modified = false;
	if (m.chargeShopID != chargeShopID) { m.chargeShopID = chargeShopID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void ChargeShopItem_Stub::SetChargeShopID(const UInt32 &chargeShopID)
{
	bool modified = false;
	if (m.chargeShopID != chargeShopID) { m.chargeShopID = chargeShopID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void ChargeShopItem_Stub::SetItemInfo(const String &itemName, const UInt8 &shopType, const UInt8 &itemType, const UInt32 &itemId, const UInt32 &yuanBao, const UInt8 &LV, const UInt8 &PreviewType)
{
	bool modified = false;
	if (m.itemName != itemName) { m.itemName = itemName; modified = true; }
	if (m.shopType != shopType) { m.shopType = shopType; modified = true; }
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	if (m.itemId != itemId) { m.itemId = itemId; modified = true; }
	if (m.yuanBao != yuanBao) { m.yuanBao = yuanBao; modified = true; }
	if (m.LV != LV) { m.LV = LV; modified = true; }
	if (m.PreviewType != PreviewType) { m.PreviewType = PreviewType; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopItem_Stub::SetItemName(const String &itemName)
{
	bool modified = false;
	if (m.itemName != itemName) { m.itemName = itemName; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopItem_Stub::SetShopType(const UInt8 &shopType)
{
	bool modified = false;
	if (m.shopType != shopType) { m.shopType = shopType; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopItem_Stub::SetItemType(const UInt8 &itemType)
{
	bool modified = false;
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopItem_Stub::SetItemId(const UInt32 &itemId)
{
	bool modified = false;
	if (m.itemId != itemId) { m.itemId = itemId; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopItem_Stub::SetYuanBao(const UInt32 &yuanBao)
{
	bool modified = false;
	if (m.yuanBao != yuanBao) { m.yuanBao = yuanBao; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopItem_Stub::SetLV(const UInt8 &LV)
{
	bool modified = false;
	if (m.LV != LV) { m.LV = LV; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopItem_Stub::SetPreviewType(const UInt8 &PreviewType)
{
	bool modified = false;
	if (m.PreviewType != PreviewType) { m.PreviewType = PreviewType; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopItem_Stub::SetDiscountInfo(const UInt8 &discount)
{
	bool modified = false;
	if (m.discount != discount) { m.discount = discount; modified = true; }
	MarkUpdateAll(discountInfoMask, modified);
}

void ChargeShopItem_Stub::SetDiscount(const UInt8 &discount)
{
	bool modified = false;
	if (m.discount != discount) { m.discount = discount; modified = true; }
	MarkUpdateAll(discountInfoMask, modified);
}

void ChargeShopItem_Stub::SetStockInfo(const UInt16 &totalStock)
{
	bool modified = false;
	if (m.totalStock != totalStock) { m.totalStock = totalStock; modified = true; }
	MarkUpdateAll(stockInfoMask, modified);
}

void ChargeShopItem_Stub::SetTotalStock(const UInt16 &totalStock)
{
	bool modified = false;
	if (m.totalStock != totalStock) { m.totalStock = totalStock; modified = true; }
	MarkUpdateAll(stockInfoMask, modified);
}

void ChargeShopItem_Stub::SetRemainInfo(const UInt16 &remain)
{
	bool modified = false;
	if (m.remain != remain) { m.remain = remain; modified = true; }
	MarkUpdateAll(remainInfoMask, modified);
}

void ChargeShopItem_Stub::SetRemain(const UInt16 &remain)
{
	bool modified = false;
	if (m.remain != remain) { m.remain = remain; modified = true; }
	MarkUpdateAll(remainInfoMask, modified);
}

void ChargeShopItem_Stub::SetLimitedInfo(const UInt8 &LimitType, const UInt8 &restore)
{
	bool modified = false;
	if (m.LimitType != LimitType) { m.LimitType = LimitType; modified = true; }
	if (m.restore != restore) { m.restore = restore; modified = true; }
	MarkUpdateAll(limitedInfoMask, modified);
}

void ChargeShopItem_Stub::SetLimitType(const UInt8 &LimitType)
{
	bool modified = false;
	if (m.LimitType != LimitType) { m.LimitType = LimitType; modified = true; }
	MarkUpdateAll(limitedInfoMask, modified);
}

void ChargeShopItem_Stub::SetRestore(const UInt8 &restore)
{
	bool modified = false;
	if (m.restore != restore) { m.restore = restore; modified = true; }
	MarkUpdateAll(limitedInfoMask, modified);
}

void ChargeShopItem_Stub::SetExistTimeInfo(const UInt32 &StartTime, const UInt32 &EndTime)
{
	bool modified = false;
	if (m.StartTime != StartTime) { m.StartTime = StartTime; modified = true; }
	if (m.EndTime != EndTime) { m.EndTime = EndTime; modified = true; }
	MarkUpdateAll(existTimeInfoMask, modified);
}

void ChargeShopItem_Stub::SetStartTime(const UInt32 &StartTime)
{
	bool modified = false;
	if (m.StartTime != StartTime) { m.StartTime = StartTime; modified = true; }
	MarkUpdateAll(existTimeInfoMask, modified);
}

void ChargeShopItem_Stub::SetEndTime(const UInt32 &EndTime)
{
	bool modified = false;
	if (m.EndTime != EndTime) { m.EndTime = EndTime; modified = true; }
	MarkUpdateAll(existTimeInfoMask, modified);
}

void ChargeShopItem_Stub::SetLastOrderInfo(const UInt32 &lastOrderDate)
{
	bool modified = false;
	if (m.lastOrderDate != lastOrderDate) { m.lastOrderDate = lastOrderDate; modified = true; }
	MarkUpdateAll(lastOrderInfoMask, modified);
}

void ChargeShopItem_Stub::SetLastOrderDate(const UInt32 &lastOrderDate)
{
	bool modified = false;
	if (m.lastOrderDate != lastOrderDate) { m.lastOrderDate = lastOrderDate; modified = true; }
	MarkUpdateAll(lastOrderInfoMask, modified);
}

void ChargeShopItem_Stub::SetSellInfo(const UInt32 &totalSell)
{
	bool modified = false;
	if (m.totalSell != totalSell) { m.totalSell = totalSell; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void ChargeShopItem_Stub::SetTotalSell(const UInt32 &totalSell)
{
	bool modified = false;
	if (m.totalSell != totalSell) { m.totalSell = totalSell; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}



void ChargeShopItem_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void ChargeShopItem_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idInfoMask))
	{
		buf.Write<UInt32>(m.chargeShopID);
	}
	if (mask.IsBit(itemInfoMask))
	{
		buf.Write<String>(m.itemName);
		buf.Write<UInt8>(m.shopType);
		buf.Write<UInt8>(m.itemType);
		buf.Write<UInt32>(m.itemId);
		buf.Write<UInt32>(m.yuanBao);
		buf.Write<UInt8>(m.LV);
		buf.Write<UInt8>(m.PreviewType);
	}
	if (mask.IsBit(discountInfoMask))
	{
		buf.Write<UInt8>(m.discount);
	}
	if (mask.IsBit(stockInfoMask))
	{
		buf.Write<UInt16>(m.totalStock);
	}
	if (mask.IsBit(remainInfoMask))
	{
		buf.Write<UInt16>(m.remain);
	}
	if (mask.IsBit(limitedInfoMask))
	{
		buf.Write<UInt8>(m.LimitType);
		buf.Write<UInt8>(m.restore);
	}
	if (mask.IsBit(existTimeInfoMask))
	{
		buf.Write<UInt32>(m.StartTime);
		buf.Write<UInt32>(m.EndTime);
	}
	if (mask.IsBit(lastOrderInfoMask))
	{
		buf.Write<UInt32>(m.lastOrderDate);
	}
	if (mask.IsBit(sellInfoMask))
	{
		buf.Write<UInt32>(m.totalSell);
	}
}

RPCResult ChargeShopItem_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ChargeShopItem_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult ChargeShopItem_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ChargeShopItem_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

