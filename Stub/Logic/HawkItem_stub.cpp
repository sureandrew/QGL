//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/HawkItem_stub.h"

void HawkItem_Stub::SetIdInfo(const UInt32 &item_uid, const UInt32 &item_id)
{
	bool modified = false;
	if (m.item_uid != item_uid) { m.item_uid = item_uid; modified = true; }
	if (m.item_id != item_id) { m.item_id = item_id; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void HawkItem_Stub::SetItem_uid(const UInt32 &item_uid)
{
	bool modified = false;
	if (m.item_uid != item_uid) { m.item_uid = item_uid; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void HawkItem_Stub::SetItem_id(const UInt32 &item_id)
{
	bool modified = false;
	if (m.item_id != item_id) { m.item_id = item_id; modified = true; }
	MarkUpdateAll(idInfoMask, modified);
}

void HawkItem_Stub::SetStackInfo(const UInt8 &item_stack)
{
	bool modified = false;
	if (m.item_stack != item_stack) { m.item_stack = item_stack; modified = true; }
	MarkUpdateAll(stackInfoMask, modified);
}

void HawkItem_Stub::SetItem_stack(const UInt8 &item_stack)
{
	bool modified = false;
	if (m.item_stack != item_stack) { m.item_stack = item_stack; modified = true; }
	MarkUpdateAll(stackInfoMask, modified);
}

void HawkItem_Stub::SetPriceInfo(const UInt32 &item_price)
{
	bool modified = false;
	if (m.item_price != item_price) { m.item_price = item_price; modified = true; }
	MarkUpdateAll(priceInfoMask, modified);
}

void HawkItem_Stub::SetItem_price(const UInt32 &item_price)
{
	bool modified = false;
	if (m.item_price != item_price) { m.item_price = item_price; modified = true; }
	MarkUpdateAll(priceInfoMask, modified);
}

void HawkItem_Stub::SetItemTypeInfo(const UInt8 &item_type)
{
	bool modified = false;
	if (m.item_type != item_type) { m.item_type = item_type; modified = true; }
	MarkUpdateAll(itemTypeInfoMask, modified);
}

void HawkItem_Stub::SetItem_type(const UInt8 &item_type)
{
	bool modified = false;
	if (m.item_type != item_type) { m.item_type = item_type; modified = true; }
	MarkUpdateAll(itemTypeInfoMask, modified);
}

void HawkItem_Stub::SetIndexInfo(const UInt8 &item_index)
{
	bool modified = false;
	if (m.item_index != item_index) { m.item_index = item_index; modified = true; }
	MarkUpdateAll(indexInfoMask, modified);
}

void HawkItem_Stub::SetItem_index(const UInt8 &item_index)
{
	bool modified = false;
	if (m.item_index != item_index) { m.item_index = item_index; modified = true; }
	MarkUpdateAll(indexInfoMask, modified);
}



void HawkItem_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void HawkItem_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idInfoMask))
	{
		buf.Write<UInt32>(m.item_uid);
		buf.Write<UInt32>(m.item_id);
	}
	if (mask.IsBit(stackInfoMask))
	{
		buf.Write<UInt8>(m.item_stack);
	}
	if (mask.IsBit(priceInfoMask))
	{
		buf.Write<UInt32>(m.item_price);
	}
	if (mask.IsBit(itemTypeInfoMask))
	{
		buf.Write<UInt8>(m.item_type);
	}
	if (mask.IsBit(indexInfoMask))
	{
		buf.Write<UInt8>(m.item_index);
	}
}

RPCResult HawkItem_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_HawkItem_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult HawkItem_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_HawkItem_REQ_STATE:
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

