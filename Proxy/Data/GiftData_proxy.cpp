//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/GiftData_proxy.h"

void GiftData_Proxy::SetIdInfo(const UInt32 &giftID, const UInt32 &acctID)
{
	bool modified = false;
	if (m.giftID != giftID) { m.giftID = giftID; modified = true; }
	if (m.acctID != acctID) { m.acctID = acctID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void GiftData_Proxy::SetGiftID(const UInt32 &giftID)
{
	bool modified = false;
	if (m.giftID != giftID) { m.giftID = giftID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void GiftData_Proxy::SetAcctID(const UInt32 &acctID)
{
	bool modified = false;
	if (m.acctID != acctID) { m.acctID = acctID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void GiftData_Proxy::SetGiftInfo(const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemQuantity)
{
	bool modified = false;
	if (m.itemID != itemID) { m.itemID = itemID; modified = true; }
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	if (m.itemQuantity != itemQuantity) { m.itemQuantity = itemQuantity; modified = true; }
	MarkUpdateAll(giftInfoMask, modified);
}

void GiftData_Proxy::SetItemID(const UInt32 &itemID)
{
	bool modified = false;
	if (m.itemID != itemID) { m.itemID = itemID; modified = true; }
	MarkUpdateAll(giftInfoMask, modified);
}

void GiftData_Proxy::SetItemType(const UInt8 &itemType)
{
	bool modified = false;
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	MarkUpdateAll(giftInfoMask, modified);
}

void GiftData_Proxy::SetItemQuantity(const UInt8 &itemQuantity)
{
	bool modified = false;
	if (m.itemQuantity != itemQuantity) { m.itemQuantity = itemQuantity; modified = true; }
	MarkUpdateAll(giftInfoMask, modified);
}

void GiftData_Proxy::SetSerialNoInfo(const String &serialNo)
{
	bool modified = false;
	if (m.serialNo != serialNo) { m.serialNo = serialNo; modified = true; }
	MarkUpdateAll(serialNoInfoMask, modified);
}

void GiftData_Proxy::SetSerialNo(const String &serialNo)
{
	bool modified = false;
	if (m.serialNo != serialNo) { m.serialNo = serialNo; modified = true; }
	MarkUpdateAll(serialNoInfoMask, modified);
}


void GiftData_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idInfoMask))
	{
		buf.Read<UInt32>(m.giftID);
		buf.Read<UInt32>(m.acctID);
	}
	if (m_updateMask.IsBit(giftInfoMask))
	{
		buf.Read<UInt32>(m.itemID);
		buf.Read<UInt8>(m.itemType);
		buf.Read<UInt8>(m.itemQuantity);
	}
	if (m_updateMask.IsBit(serialNoInfoMask))
	{
		buf.Read<String>(m.serialNo);
	}
}

void GiftData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idInfoMask))
	{
		buf.Write<UInt32>(m.giftID);
		buf.Write<UInt32>(m.acctID);
	}
	if (mask.IsBit(giftInfoMask))
	{
		buf.Write<UInt32>(m.itemID);
		buf.Write<UInt8>(m.itemType);
		buf.Write<UInt8>(m.itemQuantity);
	}
	if (mask.IsBit(serialNoInfoMask))
	{
		buf.Write<String>(m.serialNo);
	}
}

void GiftData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idInfoMask)) OnUpdateIdInfo(m.giftID, m.acctID);
	if (IsUpdated(giftInfoMask)) OnUpdateGiftInfo(m.itemID, m.itemType, m.itemQuantity);
	if (IsUpdated(serialNoInfoMask)) OnUpdateSerialNoInfo(m.serialNo);
}

#pragma optimize("", off)
RPCResult GiftData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_GiftData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult GiftData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_GiftData_UP_STATE:
			_result = RPCDecode<158225106>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
