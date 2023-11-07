//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/HawkItem_proxy.h"


void HawkItem_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idInfoMask))
	{
		buf.Read<UInt32>(m.item_uid);
		buf.Read<UInt32>(m.item_id);
	}
	if (m_updateMask.IsBit(stackInfoMask))
	{
		buf.Read<UInt8>(m.item_stack);
	}
	if (m_updateMask.IsBit(priceInfoMask))
	{
		buf.Read<UInt32>(m.item_price);
	}
	if (m_updateMask.IsBit(itemTypeInfoMask))
	{
		buf.Read<UInt8>(m.item_type);
	}
	if (m_updateMask.IsBit(indexInfoMask))
	{
		buf.Read<UInt8>(m.item_index);
	}
}

void HawkItem_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void HawkItem_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idInfoMask)) OnUpdateIdInfo(m.item_uid, m.item_id);
	if (IsUpdated(stackInfoMask)) OnUpdateStackInfo(m.item_stack);
	if (IsUpdated(priceInfoMask)) OnUpdatePriceInfo(m.item_price);
	if (IsUpdated(itemTypeInfoMask)) OnUpdateItemTypeInfo(m.item_type);
	if (IsUpdated(indexInfoMask)) OnUpdateIndexInfo(m.item_index);
}

#pragma optimize("", off)
RPCResult HawkItem_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_HawkItem_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult HawkItem_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_HawkItem_UP_STATE:
			_result = RPCDecode<858285763>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
