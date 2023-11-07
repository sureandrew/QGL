//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Generation_proxy.h"


void Generation_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idsMask))
	{
		buf.Read<UInt32>(m.uiRelatedId);
	}
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<String>(m.strNickName);
		buf.Read<UInt8>(m.ui8Class);
		buf.Read<UInt16>(m.ui16Level);
		buf.Read<UInt32>(m.uiOfflineTime);
		buf.Read<UInt32>(m.uiAccmulOrRecvExp);
		buf.Read<UInt32>(m.uiGiftPointOrHistAccm);
	}
	if (m_updateMask.IsBit(flagMask))
	{
		buf.Read<BOOL>(m.bYoung);
	}
}

void Generation_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Generation_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idsMask)) OnUpdateIds(m.uiRelatedId);
	if (IsUpdated(infoMask)) OnUpdateInfo(m.strNickName, m.ui8Class, m.ui16Level, m.uiOfflineTime, m.uiAccmulOrRecvExp, m.uiGiftPointOrHistAccm);
	if (IsUpdated(flagMask)) OnUpdateFlag(m.bYoung);
}

#pragma optimize("", off)
RPCResult Generation_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Generation_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Generation_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Generation_UP_STATE:
			_result = RPCDecode<1512597031>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
