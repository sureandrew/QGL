//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/ChargeShopData_stub.h"

void ChargeShopData_Stub::SetIdInfo(const UInt32 &chargeShopID)
{
	bool modified = false;
	if (m.chargeShopID != chargeShopID) { m.chargeShopID = chargeShopID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void ChargeShopData_Stub::SetChargeShopID(const UInt32 &chargeShopID)
{
	bool modified = false;
	if (m.chargeShopID != chargeShopID) { m.chargeShopID = chargeShopID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void ChargeShopData_Stub::SetItemInfo(const String &name, const UInt8 &shopType, const UInt8 &itemType, const UInt32 &itemId, const UInt32 &yuanBao, const UInt8 &LV, const UInt8 &PreviewType)
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

void ChargeShopData_Stub::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Stub::SetShopType(const UInt8 &shopType)
{
	bool modified = false;
	if (m.shopType != shopType) { m.shopType = shopType; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Stub::SetItemType(const UInt8 &itemType)
{
	bool modified = false;
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Stub::SetItemId(const UInt32 &itemId)
{
	bool modified = false;
	if (m.itemId != itemId) { m.itemId = itemId; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Stub::SetYuanBao(const UInt32 &yuanBao)
{
	bool modified = false;
	if (m.yuanBao != yuanBao) { m.yuanBao = yuanBao; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Stub::SetLV(const UInt8 &LV)
{
	bool modified = false;
	if (m.LV != LV) { m.LV = LV; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Stub::SetPreviewType(const UInt8 &PreviewType)
{
	bool modified = false;
	if (m.PreviewType != PreviewType) { m.PreviewType = PreviewType; modified = true; }
	MarkUpdateAll(itemInfoMask, modified);
}

void ChargeShopData_Stub::SetActiveInfo(const UInt8 &active)
{
	bool modified = false;
	if (m.active != active) { m.active = active; modified = true; }
	MarkUpdateAll(activeInfoMask, modified);
}

void ChargeShopData_Stub::SetActive(const UInt8 &active)
{
	bool modified = false;
	if (m.active != active) { m.active = active; modified = true; }
	MarkUpdateAll(activeInfoMask, modified);
}

void ChargeShopData_Stub::SetDiscountInfo(const UInt8 &discount)
{
	bool modified = false;
	if (m.discount != discount) { m.discount = discount; modified = true; }
	MarkUpdateAll(discountInfoMask, modified);
}

void ChargeShopData_Stub::SetDiscount(const UInt8 &discount)
{
	bool modified = false;
	if (m.discount != discount) { m.discount = discount; modified = true; }
	MarkUpdateAll(discountInfoMask, modified);
}

void ChargeShopData_Stub::SetStockInfo(const UInt16 &totalStock)
{
	bool modified = false;
	if (m.totalStock != totalStock) { m.totalStock = totalStock; modified = true; }
	MarkUpdateAll(stockInfoMask, modified);
}

void ChargeShopData_Stub::SetTotalStock(const UInt16 &totalStock)
{
	bool modified = false;
	if (m.totalStock != totalStock) { m.totalStock = totalStock; modified = true; }
	MarkUpdateAll(stockInfoMask, modified);
}

void ChargeShopData_Stub::SetRemainInfo(const UInt16 &remain)
{
	bool modified = false;
	if (m.remain != remain) { m.remain = remain; modified = true; }
	MarkUpdateAll(remainInfoMask, modified);
}

void ChargeShopData_Stub::SetRemain(const UInt16 &remain)
{
	bool modified = false;
	if (m.remain != remain) { m.remain = remain; modified = true; }
	MarkUpdateAll(remainInfoMask, modified);
}

void ChargeShopData_Stub::SetLimitedInfo(const UInt8 &LimitType, const UInt8 &restore)
{
	bool modified = false;
	if (m.LimitType != LimitType) { m.LimitType = LimitType; modified = true; }
	if (m.restore != restore) { m.restore = restore; modified = true; }
	MarkUpdateAll(limitedInfoMask, modified);
}

void ChargeShopData_Stub::SetLimitType(const UInt8 &LimitType)
{
	bool modified = false;
	if (m.LimitType != LimitType) { m.LimitType = LimitType; modified = true; }
	MarkUpdateAll(limitedInfoMask, modified);
}

void ChargeShopData_Stub::SetRestore(const UInt8 &restore)
{
	bool modified = false;
	if (m.restore != restore) { m.restore = restore; modified = true; }
	MarkUpdateAll(limitedInfoMask, modified);
}

void ChargeShopData_Stub::SetExistTimeInfo(const UInt32 &StartTime, const UInt32 &EndTime)
{
	bool modified = false;
	if (m.StartTime != StartTime) { m.StartTime = StartTime; modified = true; }
	if (m.EndTime != EndTime) { m.EndTime = EndTime; modified = true; }
	MarkUpdateAll(existTimeInfoMask, modified);
}

void ChargeShopData_Stub::SetStartTime(const UInt32 &StartTime)
{
	bool modified = false;
	if (m.StartTime != StartTime) { m.StartTime = StartTime; modified = true; }
	MarkUpdateAll(existTimeInfoMask, modified);
}

void ChargeShopData_Stub::SetEndTime(const UInt32 &EndTime)
{
	bool modified = false;
	if (m.EndTime != EndTime) { m.EndTime = EndTime; modified = true; }
	MarkUpdateAll(existTimeInfoMask, modified);
}

void ChargeShopData_Stub::SetLastOrderInfo(const UInt32 &lastOrderDate)
{
	bool modified = false;
	if (m.lastOrderDate != lastOrderDate) { m.lastOrderDate = lastOrderDate; modified = true; }
	MarkUpdateAll(lastOrderInfoMask, modified);
}

void ChargeShopData_Stub::SetLastOrderDate(const UInt32 &lastOrderDate)
{
	bool modified = false;
	if (m.lastOrderDate != lastOrderDate) { m.lastOrderDate = lastOrderDate; modified = true; }
	MarkUpdateAll(lastOrderInfoMask, modified);
}

void ChargeShopData_Stub::SetSellInfo(const UInt32 &totalSell)
{
	bool modified = false;
	if (m.totalSell != totalSell) { m.totalSell = totalSell; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void ChargeShopData_Stub::SetTotalSell(const UInt32 &totalSell)
{
	bool modified = false;
	if (m.totalSell != totalSell) { m.totalSell = totalSell; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}


void ChargeShopData_Stub::Clone(ChargeShopData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(idInfoMask) && !(obj->m.chargeShopID == m.chargeShopID)) {
		m.chargeShopID = obj->m.chargeShopID;
		MarkUpdateAll(idInfoMask, true);
	}
	if (mask.IsBit(itemInfoMask) && !(obj->m.name == m.name)) {
		m.name = obj->m.name;
		MarkUpdateAll(itemInfoMask, true);
	}
	if (mask.IsBit(itemInfoMask) && !(obj->m.shopType == m.shopType)) {
		m.shopType = obj->m.shopType;
		MarkUpdateAll(itemInfoMask, true);
	}
	if (mask.IsBit(itemInfoMask) && !(obj->m.itemType == m.itemType)) {
		m.itemType = obj->m.itemType;
		MarkUpdateAll(itemInfoMask, true);
	}
	if (mask.IsBit(itemInfoMask) && !(obj->m.itemId == m.itemId)) {
		m.itemId = obj->m.itemId;
		MarkUpdateAll(itemInfoMask, true);
	}
	if (mask.IsBit(itemInfoMask) && !(obj->m.yuanBao == m.yuanBao)) {
		m.yuanBao = obj->m.yuanBao;
		MarkUpdateAll(itemInfoMask, true);
	}
	if (mask.IsBit(itemInfoMask) && !(obj->m.LV == m.LV)) {
		m.LV = obj->m.LV;
		MarkUpdateAll(itemInfoMask, true);
	}
	if (mask.IsBit(itemInfoMask) && !(obj->m.PreviewType == m.PreviewType)) {
		m.PreviewType = obj->m.PreviewType;
		MarkUpdateAll(itemInfoMask, true);
	}
	if (mask.IsBit(activeInfoMask) && !(obj->m.active == m.active)) {
		m.active = obj->m.active;
		MarkUpdateAll(activeInfoMask, true);
	}
	if (mask.IsBit(discountInfoMask) && !(obj->m.discount == m.discount)) {
		m.discount = obj->m.discount;
		MarkUpdateAll(discountInfoMask, true);
	}
	if (mask.IsBit(stockInfoMask) && !(obj->m.totalStock == m.totalStock)) {
		m.totalStock = obj->m.totalStock;
		MarkUpdateAll(stockInfoMask, true);
	}
	if (mask.IsBit(remainInfoMask) && !(obj->m.remain == m.remain)) {
		m.remain = obj->m.remain;
		MarkUpdateAll(remainInfoMask, true);
	}
	if (mask.IsBit(limitedInfoMask) && !(obj->m.LimitType == m.LimitType)) {
		m.LimitType = obj->m.LimitType;
		MarkUpdateAll(limitedInfoMask, true);
	}
	if (mask.IsBit(limitedInfoMask) && !(obj->m.restore == m.restore)) {
		m.restore = obj->m.restore;
		MarkUpdateAll(limitedInfoMask, true);
	}
	if (mask.IsBit(existTimeInfoMask) && !(obj->m.StartTime == m.StartTime)) {
		m.StartTime = obj->m.StartTime;
		MarkUpdateAll(existTimeInfoMask, true);
	}
	if (mask.IsBit(existTimeInfoMask) && !(obj->m.EndTime == m.EndTime)) {
		m.EndTime = obj->m.EndTime;
		MarkUpdateAll(existTimeInfoMask, true);
	}
	if (mask.IsBit(lastOrderInfoMask) && !(obj->m.lastOrderDate == m.lastOrderDate)) {
		m.lastOrderDate = obj->m.lastOrderDate;
		MarkUpdateAll(lastOrderInfoMask, true);
	}
	if (mask.IsBit(sellInfoMask) && !(obj->m.totalSell == m.totalSell)) {
		m.totalSell = obj->m.totalSell;
		MarkUpdateAll(sellInfoMask, true);
	}
}

void ChargeShopData_Stub::ReadAttr(CONST Buffer &buf)
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

void ChargeShopData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult ChargeShopData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ChargeShopData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult ChargeShopData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ChargeShopData_REQ_STATE:
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

