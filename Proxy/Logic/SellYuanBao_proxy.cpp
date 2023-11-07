//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/SellYuanBao_proxy.h"


void SellYuanBao_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.sellYuanBao_id);
	}
	if (m_updateMask.IsBit(charInfoMask))
	{
		buf.Read<UInt32>(m.char_id);
		buf.Read<String>(m.charName);
	}
	if (m_updateMask.IsBit(sellInfoMask))
	{
		buf.Read<UInt8>(m.sellType);
		buf.Read<UInt32>(m.totalYBT);
		buf.Read<UInt32>(m.moneyPerYBT);
		buf.Read<UInt32>(m.expireDate);
	}
	if (m_updateMask.IsBit(soldYBTMask))
	{
		buf.Read<UInt32>(m.soldYBT);
	}
}

void SellYuanBao_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void SellYuanBao_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.sellYuanBao_id);
	if (IsUpdated(charInfoMask)) OnUpdateCharInfo(m.char_id, m.charName);
	if (IsUpdated(sellInfoMask)) OnUpdateSellInfo(m.sellType, m.totalYBT, m.moneyPerYBT, m.expireDate);
	if (IsUpdated(soldYBTMask)) OnUpdateSoldYBT(m.soldYBT);
}

#pragma optimize("", off)
RPCResult SellYuanBao_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_SellYuanBao_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult SellYuanBao_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_SellYuanBao_UP_STATE:
			_result = RPCDecode<1102668347>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
