//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/HawkerHistoryData_stub.h"

void HawkerHistoryData_Stub::SetHistoryId(const UInt32 &historyId)
{
	bool modified = false;
	if (m.historyId != historyId) { m.historyId = historyId; modified = true; }
	MarkUpdateAll(historyIdMask, modified);
}

void HawkerHistoryData_Stub::SetHistoryInfo(const UInt32 &char_id, const String &itemName, const UInt8 &itemType, const UInt8 &stack, const UInt32 &totalPrice, const UInt32 &tax)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	if (m.itemName != itemName) { m.itemName = itemName; modified = true; }
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	if (m.stack != stack) { m.stack = stack; modified = true; }
	if (m.totalPrice != totalPrice) { m.totalPrice = totalPrice; modified = true; }
	if (m.tax != tax) { m.tax = tax; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Stub::SetItemName(const String &itemName)
{
	bool modified = false;
	if (m.itemName != itemName) { m.itemName = itemName; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Stub::SetItemType(const UInt8 &itemType)
{
	bool modified = false;
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Stub::SetStack(const UInt8 &stack)
{
	bool modified = false;
	if (m.stack != stack) { m.stack = stack; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Stub::SetTotalPrice(const UInt32 &totalPrice)
{
	bool modified = false;
	if (m.totalPrice != totalPrice) { m.totalPrice = totalPrice; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Stub::SetTax(const UInt32 &tax)
{
	bool modified = false;
	if (m.tax != tax) { m.tax = tax; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Stub::SetHistoryDate(const UInt32 &historyDate)
{
	bool modified = false;
	if (m.historyDate != historyDate) { m.historyDate = historyDate; modified = true; }
	MarkUpdateAll(historyDateMask, modified);
}


void HawkerHistoryData_Stub::Clone(HawkerHistoryData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(historyIdMask) && !(obj->m.historyId == m.historyId)) {
		m.historyId = obj->m.historyId;
		MarkUpdateAll(historyIdMask, true);
	}
	if (mask.IsBit(historyInfoMask) && !(obj->m.char_id == m.char_id)) {
		m.char_id = obj->m.char_id;
		MarkUpdateAll(historyInfoMask, true);
	}
	if (mask.IsBit(historyInfoMask) && !(obj->m.itemName == m.itemName)) {
		m.itemName = obj->m.itemName;
		MarkUpdateAll(historyInfoMask, true);
	}
	if (mask.IsBit(historyInfoMask) && !(obj->m.itemType == m.itemType)) {
		m.itemType = obj->m.itemType;
		MarkUpdateAll(historyInfoMask, true);
	}
	if (mask.IsBit(historyInfoMask) && !(obj->m.stack == m.stack)) {
		m.stack = obj->m.stack;
		MarkUpdateAll(historyInfoMask, true);
	}
	if (mask.IsBit(historyInfoMask) && !(obj->m.totalPrice == m.totalPrice)) {
		m.totalPrice = obj->m.totalPrice;
		MarkUpdateAll(historyInfoMask, true);
	}
	if (mask.IsBit(historyInfoMask) && !(obj->m.tax == m.tax)) {
		m.tax = obj->m.tax;
		MarkUpdateAll(historyInfoMask, true);
	}
	if (mask.IsBit(historyDateMask) && !(obj->m.historyDate == m.historyDate)) {
		m.historyDate = obj->m.historyDate;
		MarkUpdateAll(historyDateMask, true);
	}
}

void HawkerHistoryData_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(historyIdMask))
	{
		buf.Read<UInt32>(m.historyId);
	}
	if (m_updateMask.IsBit(historyInfoMask))
	{
		buf.Read<UInt32>(m.char_id);
		buf.Read<String>(m.itemName);
		buf.Read<UInt8>(m.itemType);
		buf.Read<UInt8>(m.stack);
		buf.Read<UInt32>(m.totalPrice);
		buf.Read<UInt32>(m.tax);
	}
	if (m_updateMask.IsBit(historyDateMask))
	{
		buf.Read<UInt32>(m.historyDate);
	}
}

void HawkerHistoryData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(historyIdMask))
	{
		buf.Write<UInt32>(m.historyId);
	}
	if (mask.IsBit(historyInfoMask))
	{
		buf.Write<UInt32>(m.char_id);
		buf.Write<String>(m.itemName);
		buf.Write<UInt8>(m.itemType);
		buf.Write<UInt8>(m.stack);
		buf.Write<UInt32>(m.totalPrice);
		buf.Write<UInt32>(m.tax);
	}
	if (mask.IsBit(historyDateMask))
	{
		buf.Write<UInt32>(m.historyDate);
	}
}

RPCResult HawkerHistoryData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_HawkerHistoryData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult HawkerHistoryData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_HawkerHistoryData_REQ_STATE:
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

