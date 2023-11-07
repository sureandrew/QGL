//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/ChargeShopData_proxy.h"

void ChargeShopData_Proxy::SetIdInfo(const UInt32 &chargeShopID)
{
	bool modified = false;
	if (m.chargeShopID != chargeShopID) { m.chargeShopID = chargeShopID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void ChargeShopData_Proxy::SetChargeShopID(const UInt32 &chargeShopID)
{
	bool modified = false;
	if (m.chargeShopID != chargeShopID) { m.chargeShopID = chargeShopID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void ChargeShopData_Proxy::SetItemInfo(const String &name, const UInt8 &shopType, const UInt8 &itemType, const UInt32 &itemId, const UInt32 &yuanBao, const UInt8 &LV, const UInt8 &PreviewType)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	if (m.shopType != shopType) { m.shopType = shopType; modified = true; }
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	if (m.itemId != itemId) { m.itemId = itemId; modified = true; }
	if (m.yuanBao != yuanBao) { m.yuanBao = yuanBao; modified = true; }
	if (m.LV != LV) { m.LV = LV; modified = true; }
	if (m.PreviewType != PreviewType) { m.PreviewType = PreviewType; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Proxy::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Proxy::SetShopType(const UInt8 &shopType)
{
	bool modified = false;
	if (m.shopType != shopType) { m.shopType = shopType; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Proxy::SetItemType(const UInt8 &itemType)
{
	bool modified = false;
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Proxy::SetItemId(const UInt32 &itemId)
{
	bool modified = false;
	if (m.itemId != itemId) { m.itemId = itemId; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Proxy::SetYuanBao(const UInt32 &yuanBao)
{
	bool modified = false;
	if (m.yuanBao != yuanBao) { m.yuanBao = yuanBao; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Proxy::SetLV(const UInt8 &LV)
{
	bool modified = false;
	if (m.LV != LV) { m.LV = LV; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Proxy::SetPreviewType(const UInt8 &PreviewType)
{
	bool modified = false;
	if (m.PreviewType != PreviewType) { m.PreviewType = PreviewType; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Proxy::SetActiveInfo(const UInt8 &active)
{
	bool modified = false;
	if (m.active != active) { m.active = active; modified = true; }
	MarkUpdateAll(activeInfoMask, modified);
}

void ChargeShopData_Proxy::SetActive(const UInt8 &active)
{
	bool modified = false;
	if (m.active != active) { m.active = active; modified = true; }
	MarkUpdateAll(activeInfoMask, modified);
}

void ChargeShopData_Proxy::SetDiscountInfo(const UInt8 &discount)
{
	bool modified = false;
	if (m.discount != discount) { m.discount = discount; modified = true; }
	MarkUpdateAll(discountInfoMask, modified);
}

void ChargeShopData_Proxy::SetDiscount(const UInt8 &discount)
{
	bool modified = false;
	if (m.discount != discount) { m.discount = discount; modified = true; }
	MarkUpdateAll(discountInfoMask, modified);
}

void ChargeShopData_Proxy::SetStockInfo(const UInt16 &totalStock)
{
	bool modified = false;
	if (m.totalStock != totalStock) { m.totalStock = totalStock; modified = true; }
	MarkUpdateAll(stockInfoMask, modified);
}

void ChargeShopData_Proxy::SetTotalStock(const UInt16 &totalStock)
{
	bool modified = false;
	if (m.totalStock != totalStock) { m.totalStock = totalStock; modified = true; }
	MarkUpdateAll(stockInfoMask, modified);
}

void ChargeShopData_Proxy::SetRemainInfo(const UInt16 &remain)
{
	bool modified = false;
	if (m.remain != remain) { m.remain = remain; modified = true; }
	MarkUpdateAll(remainInfoMask, modified);
}

void ChargeShopData_Proxy::SetRemain(const UInt16 &remain)
{
	bool modified = false;
	if (m.remain != remain) { m.remain = remain; modified = true; }
	MarkUpdateAll(remainInfoMask, modified);
}

void ChargeShopData_Proxy::SetLimitedInfo(const UInt8 &LimitType, const UInt8 &restore)
{
	bool modified = false;
	if (m.LimitType != LimitType) { m.LimitType = LimitType; modified = true; }
	if (m.restore != restore) { m.restore = restore; modified = true; }
	MarkUpdateAll(limitedInfoMask, modified);
}

void ChargeShopData_Proxy::SetLimitType(const UInt8 &LimitType)
{
	bool modified = false;
	if (m.LimitType != LimitType) { m.LimitType = LimitType; modified = true; }
	MarkUpdateAll(limitedInfoMask, modified);
}

void ChargeShopData_Proxy::SetRestore(const UInt8 &restore)
{
	bool modified = false;
	if (m.restore != restore) { m.restore = restore; modified = true; }
	MarkUpdateAll(limitedInfoMask, modified);
}

void ChargeShopData_Proxy::SetExistTimeInfo(const UInt32 &StartTime, const UInt32 &EndTime)
{
	bool modified = false;
	if (m.StartTime != StartTime) { m.StartTime = StartTime; modified = true; }
	if (m.EndTime != EndTime) { m.EndTime = EndTime; modified = true; }
	MarkUpdateAll(existTimeInfoMask, modified);
}

void ChargeShopData_Proxy::SetStartTime(const UInt32 &StartTime)
{
	bool modified = false;
	if (m.StartTime != StartTime) { m.StartTime = StartTime; modified = true; }
	MarkUpdateAll(existTimeInfoMask, modified);
}

void ChargeShopData_Proxy::SetEndTime(const UInt32 &EndTime)
{
	bool modified = false;
	if (m.EndTime != EndTime) { m.EndTime = EndTime; modified = true; }
	MarkUpdateAll(existTimeInfoMask, modified);
}

void ChargeShopData_Proxy::SetLastOrderInfo(const UInt32 &lastOrderDate)
{
	bool modified = false;
	if (m.lastOrderDate != lastOrderDate) { m.lastOrderDate = lastOrderDate; modified = true; }
	MarkUpdateAll(lastOrderInfoMask, modified);
}

void ChargeShopData_Proxy::SetLastOrderDate(const UInt32 &lastOrderDate)
{
	bool modified = false;
	if (m.lastOrderDate != lastOrderDate) { m.lastOrderDate = lastOrderDate; modified = true; }
	MarkUpdateAll(lastOrderInfoMask, modified);
}

void ChargeShopData_Proxy::SetSellInfo(const UInt32 &totalSell)
{
	bool modified = false;
	if (m.totalSell != totalSell) { m.totalSell = totalSell; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void ChargeShopData_Proxy::SetTotalSell(const UInt32 &totalSell)
{
	bool modified = false;
	if (m.totalSell != totalSell) { m.totalSell = totalSell; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}


void ChargeShopData_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idInfoMask))
	{
		buf.Read<UInt32>(m.chargeShopID);
	}
	if (m_updateMask.IsBit(itemInfoMask))
	{
		buf.Read<String>(m.name);
		buf.Read<UInt8>(m.shopType);
		buf.Read<UInt8>(m.itemType);
		buf.Read<UInt32>(m.itemId);
		buf.Read<UInt32>(m.yuanBao);
		buf.Read<UInt8>(m.LV);
		buf.Read<UInt8>(m.PreviewType);
	}
	if (m_updateMask.IsBit(activeInfoMask))
	{
		buf.Read<UInt8>(m.active);
	}
	if (m_updateMask.IsBit(discountInfoMask))
	{
		buf.Read<UInt8>(m.discount);
	}
	if (m_updateMask.IsBit(stockInfoMask))
	{
		buf.Read<UInt16>(m.totalStock);
	}
	if (m_updateMask.IsBit(remainInfoMask))
	{
		buf.Read<UInt16>(m.remain);
	}
	if (m_updateMask.IsBit(limitedInfoMask))
	{
		buf.Read<UInt8>(m.LimitType);
		buf.Read<UInt8>(m.restore);
	}
	if (m_updateMask.IsBit(existTimeInfoMask))
	{
		buf.Read<UInt32>(m.StartTime);
		buf.Read<UInt32>(m.EndTime);
	}
	if (m_updateMask.IsBit(lastOrderInfoMask))
	{
		buf.Read<UInt32>(m.lastOrderDate);
	}
	if (m_updateMask.IsBit(sellInfoMask))
	{
		buf.Read<UInt32>(m.totalSell);
	}
}

void ChargeShopData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idInfoMask))
	{
		buf.Write<UInt32>(m.chargeShopID);
	}
	if (mask.IsBit(itemInfoMask))
	{
		buf.Write<String>(m.name);
		buf.Write<UInt8>(m.shopType);
		buf.Write<UInt8>(m.itemType);
		buf.Write<UInt32>(m.itemId);
		buf.Write<UInt32>(m.yuanBao);
		buf.Write<UInt8>(m.LV);
		buf.Write<UInt8>(m.PreviewType);
	}
	if (mask.IsBit(activeInfoMask))
	{
		buf.Write<UInt8>(m.active);
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

void ChargeShopData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idInfoMask)) OnUpdateIdInfo(m.chargeShopID);
	if (IsUpdated(itemInfoMask)) OnUpdateItemInfo(m.name, m.shopType, m.itemType, m.itemId, m.yuanBao, m.LV, m.PreviewType);
	if (IsUpdated(activeInfoMask)) OnUpdateActiveInfo(m.active);
	if (IsUpdated(discountInfoMask)) OnUpdateDiscountInfo(m.discount);
	if (IsUpdated(stockInfoMask)) OnUpdateStockInfo(m.totalStock);
	if (IsUpdated(remainInfoMask)) OnUpdateRemainInfo(m.remain);
	if (IsUpdated(limitedInfoMask)) OnUpdateLimitedInfo(m.LimitType, m.restore);
	if (IsUpdated(existTimeInfoMask)) OnUpdateExistTimeInfo(m.StartTime, m.EndTime);
	if (IsUpdated(lastOrderInfoMask)) OnUpdateLastOrderInfo(m.lastOrderDate);
	if (IsUpdated(sellInfoMask)) OnUpdateSellInfo(m.totalSell);
}

#pragma optimize("", off)
RPCResult ChargeShopData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ChargeShopData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult ChargeShopData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_ChargeShopData_UP_STATE:
			_result = RPCDecode<1620454845>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
