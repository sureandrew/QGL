//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/HawkerHistoryData_proxy.h"

void HawkerHistoryData_Proxy::SetHistoryId(const UInt32 &historyId)
{
	bool modified = false;
	if (m.historyId != historyId) { m.historyId = historyId; modified = true; }
	MarkUpdateAll(historyIdMask, modified);
}

void HawkerHistoryData_Proxy::SetHistoryInfo(const UInt32 &char_id, const String &itemName, const UInt8 &itemType, const UInt8 &stack, const UInt32 &totalPrice, const UInt32 &tax)
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

void HawkerHistoryData_Proxy::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Proxy::SetItemName(const String &itemName)
{
	bool modified = false;
	if (m.itemName != itemName) { m.itemName = itemName; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Proxy::SetItemType(const UInt8 &itemType)
{
	bool modified = false;
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Proxy::SetStack(const UInt8 &stack)
{
	bool modified = false;
	if (m.stack != stack) { m.stack = stack; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Proxy::SetTotalPrice(const UInt32 &totalPrice)
{
	bool modified = false;
	if (m.totalPrice != totalPrice) { m.totalPrice = totalPrice; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Proxy::SetTax(const UInt32 &tax)
{
	bool modified = false;
	if (m.tax != tax) { m.tax = tax; modified = true; }
	MarkUpdateAll(historyInfoMask, modified);
}

void HawkerHistoryData_Proxy::SetHistoryDate(const UInt32 &historyDate)
{
	bool modified = false;
	if (m.historyDate != historyDate) { m.historyDate = historyDate; modified = true; }
	MarkUpdateAll(historyDateMask, modified);
}


void HawkerHistoryData_Proxy::ReadAttr(CONST Buffer &buf)
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

void HawkerHistoryData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

void HawkerHistoryData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(historyIdMask)) OnUpdateHistoryId(m.historyId);
	if (IsUpdated(historyInfoMask)) OnUpdateHistoryInfo(m.char_id, m.itemName, m.itemType, m.stack, m.totalPrice, m.tax);
	if (IsUpdated(historyDateMask)) OnUpdateHistoryDate(m.historyDate);
}

#pragma optimize("", off)
RPCResult HawkerHistoryData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_HawkerHistoryData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult HawkerHistoryData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_HawkerHistoryData_UP_STATE:
			_result = RPCDecode<1719150326>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
