//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/ChargeShopItem_proxy.h"


void ChargeShopItem_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idInfoMask))
	{
		buf.Read<UInt32>(m.chargeShopID);
	}
	if (m_updateMask.IsBit(itemInfoMask))
	{
		buf.Read<String>(m.itemName);
		buf.Read<UInt8>(m.shopType);
		buf.Read<UInt8>(m.itemType);
		buf.Read<UInt32>(m.itemId);
		buf.Read<UInt32>(m.yuanBao);
		buf.Read<UInt8>(m.LV);
		buf.Read<UInt8>(m.PreviewType);
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

void ChargeShopItem_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void ChargeShopItem_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idInfoMask)) OnUpdateIdInfo(m.chargeShopID);
	if (IsUpdated(itemInfoMask)) OnUpdateItemInfo(m.itemName, m.shopType, m.itemType, m.itemId, m.yuanBao, m.LV, m.PreviewType);
	if (IsUpdated(discountInfoMask)) OnUpdateDiscountInfo(m.discount);
	if (IsUpdated(stockInfoMask)) OnUpdateStockInfo(m.totalStock);
	if (IsUpdated(remainInfoMask)) OnUpdateRemainInfo(m.remain);
	if (IsUpdated(limitedInfoMask)) OnUpdateLimitedInfo(m.LimitType, m.restore);
	if (IsUpdated(existTimeInfoMask)) OnUpdateExistTimeInfo(m.StartTime, m.EndTime);
	if (IsUpdated(lastOrderInfoMask)) OnUpdateLastOrderInfo(m.lastOrderDate);
	if (IsUpdated(sellInfoMask)) OnUpdateSellInfo(m.totalSell);
}

#pragma optimize("", off)
RPCResult ChargeShopItem_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ChargeShopItem_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult ChargeShopItem_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_ChargeShopItem_UP_STATE:
			_result = RPCDecode<596444539>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
