//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Gift_proxy.h"


void Gift_Proxy::ReadAttr(CONST Buffer &buf)
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

void Gift_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Gift_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idInfoMask)) OnUpdateIdInfo(m.giftID, m.acctID);
	if (IsUpdated(giftInfoMask)) OnUpdateGiftInfo(m.itemID, m.itemType, m.itemQuantity);
	if (IsUpdated(serialNoInfoMask)) OnUpdateSerialNoInfo(m.serialNo);
}

#pragma optimize("", off)
RPCResult Gift_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Gift_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Gift_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Gift_UP_STATE:
			_result = RPCDecode<929695389>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
