//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/GiftData_stub.h"

void GiftData_Stub::SetIdInfo(const UInt32 &giftID, const UInt32 &acctID)
{
	bool modified = false;
	if (m.giftID != giftID) { m.giftID = giftID; modified = true; }
	if (m.acctID != acctID) { m.acctID = acctID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void GiftData_Stub::SetGiftID(const UInt32 &giftID)
{
	bool modified = false;
	if (m.giftID != giftID) { m.giftID = giftID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void GiftData_Stub::SetAcctID(const UInt32 &acctID)
{
	bool modified = false;
	if (m.acctID != acctID) { m.acctID = acctID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void GiftData_Stub::SetGiftInfo(const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemQuantity)
{
	bool modified = false;
	if (m.itemID != itemID) { m.itemID = itemID; modified = true; }
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	if (m.itemQuantity != itemQuantity) { m.itemQuantity = itemQuantity; modified = true; }
	MarkUpdateAll(giftInfoMask, modified);
}

void GiftData_Stub::SetItemID(const UInt32 &itemID)
{
	bool modified = false;
	if (m.itemID != itemID) { m.itemID = itemID; modified = true; }
	MarkUpdateAll(giftInfoMask, modified);
}

void GiftData_Stub::SetItemType(const UInt8 &itemType)
{
	bool modified = false;
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	MarkUpdateAll(giftInfoMask, modified);
}

void GiftData_Stub::SetItemQuantity(const UInt8 &itemQuantity)
{
	bool modified = false;
	if (m.itemQuantity != itemQuantity) { m.itemQuantity = itemQuantity; modified = true; }
	MarkUpdateAll(giftInfoMask, modified);
}

void GiftData_Stub::SetSerialNoInfo(const String &serialNo)
{
	bool modified = false;
	if (m.serialNo != serialNo) { m.serialNo = serialNo; modified = true; }
	MarkUpdateAll(serialNoInfoMask, modified);
}

void GiftData_Stub::SetSerialNo(const String &serialNo)
{
	bool modified = false;
	if (m.serialNo != serialNo) { m.serialNo = serialNo; modified = true; }
	MarkUpdateAll(serialNoInfoMask, modified);
}


void GiftData_Stub::Clone(GiftData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(idInfoMask) && !(obj->m.giftID == m.giftID)) {
		m.giftID = obj->m.giftID;
		MarkUpdateAll(idInfoMask, true);
	}
	if (mask.IsBit(idInfoMask) && !(obj->m.acctID == m.acctID)) {
		m.acctID = obj->m.acctID;
		MarkUpdateAll(idInfoMask, true);
	}
	if (mask.IsBit(giftInfoMask) && !(obj->m.itemID == m.itemID)) {
		m.itemID = obj->m.itemID;
		MarkUpdateAll(giftInfoMask, true);
	}
	if (mask.IsBit(giftInfoMask) && !(obj->m.itemType == m.itemType)) {
		m.itemType = obj->m.itemType;
		MarkUpdateAll(giftInfoMask, true);
	}
	if (mask.IsBit(giftInfoMask) && !(obj->m.itemQuantity == m.itemQuantity)) {
		m.itemQuantity = obj->m.itemQuantity;
		MarkUpdateAll(giftInfoMask, true);
	}
	if (mask.IsBit(serialNoInfoMask) && !(obj->m.serialNo == m.serialNo)) {
		m.serialNo = obj->m.serialNo;
		MarkUpdateAll(serialNoInfoMask, true);
	}
}

void GiftData_Stub::ReadAttr(CONST Buffer &buf)
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

void GiftData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult GiftData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GiftData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult GiftData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_GiftData_REQ_STATE:
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

