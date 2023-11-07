//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Gift_stub.h"

void Gift_Stub::SetIdInfo(const UInt32 &giftID, const UInt32 &acctID)
{
	bool modified = false;
	if (m.giftID != giftID) { m.giftID = giftID; modified = true; }
	if (m.acctID != acctID) { m.acctID = acctID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void Gift_Stub::SetGiftID(const UInt32 &giftID)
{
	bool modified = false;
	if (m.giftID != giftID) { m.giftID = giftID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void Gift_Stub::SetAcctID(const UInt32 &acctID)
{
	bool modified = false;
	if (m.acctID != acctID) { m.acctID = acctID; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void Gift_Stub::SetGiftInfo(const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemQuantity)
{
	bool modified = false;
	if (m.itemID != itemID) { m.itemID = itemID; modified = true; }
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	if (m.itemQuantity != itemQuantity) { m.itemQuantity = itemQuantity; modified = true; }
	MarkUpdateAll(giftInfoMask, modified);
}

void Gift_Stub::SetItemID(const UInt32 &itemID)
{
	bool modified = false;
	if (m.itemID != itemID) { m.itemID = itemID; modified = true; }
	MarkUpdateAll(giftInfoMask, modified);
}

void Gift_Stub::SetItemType(const UInt8 &itemType)
{
	bool modified = false;
	if (m.itemType != itemType) { m.itemType = itemType; modified = true; }
	MarkUpdateAll(giftInfoMask, modified);
}

void Gift_Stub::SetItemQuantity(const UInt8 &itemQuantity)
{
	bool modified = false;
	if (m.itemQuantity != itemQuantity) { m.itemQuantity = itemQuantity; modified = true; }
	MarkUpdateAll(giftInfoMask, modified);
}

void Gift_Stub::SetSerialNoInfo(const String &serialNo)
{
	bool modified = false;
	if (m.serialNo != serialNo) { m.serialNo = serialNo; modified = true; }
	MarkUpdateAll(serialNoInfoMask, modified);
}

void Gift_Stub::SetSerialNo(const String &serialNo)
{
	bool modified = false;
	if (m.serialNo != serialNo) { m.serialNo = serialNo; modified = true; }
	MarkUpdateAll(serialNoInfoMask, modified);
}



void Gift_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Gift_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult Gift_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Gift_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Gift_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Gift_REQ_STATE:
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

