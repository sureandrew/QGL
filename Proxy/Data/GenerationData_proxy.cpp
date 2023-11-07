//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/GenerationData_proxy.h"

void GenerationData_Proxy::SetIds(const UInt32 &uiYoungerId, const UInt32 &uiElderId)
{
	bool modified = false;
	if (m.uiYoungerId != uiYoungerId) { m.uiYoungerId = uiYoungerId; modified = true; }
	if (m.uiElderId != uiElderId) { m.uiElderId = uiElderId; modified = true; }
	MarkUpdateAll(idsMask, modified);
}

void GenerationData_Proxy::SetUiYoungerId(const UInt32 &uiYoungerId)
{
	bool modified = false;
	if (m.uiYoungerId != uiYoungerId) { m.uiYoungerId = uiYoungerId; modified = true; }
	MarkUpdateAll(idsMask, modified);
}

void GenerationData_Proxy::SetUiElderId(const UInt32 &uiElderId)
{
	bool modified = false;
	if (m.uiElderId != uiElderId) { m.uiElderId = uiElderId; modified = true; }
	MarkUpdateAll(idsMask, modified);
}

void GenerationData_Proxy::SetInfo(const String &strNickName, const UInt32 &uiAccmulOrRecvExp, const UInt32 &uiGiftPointOrHistAccm)
{
	bool modified = false;
	if (m.strNickName != strNickName) { m.strNickName = strNickName; modified = true; }
	if (m.uiAccmulOrRecvExp != uiAccmulOrRecvExp) { m.uiAccmulOrRecvExp = uiAccmulOrRecvExp; modified = true; }
	if (m.uiGiftPointOrHistAccm != uiGiftPointOrHistAccm) { m.uiGiftPointOrHistAccm = uiGiftPointOrHistAccm; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void GenerationData_Proxy::SetStrNickName(const String &strNickName)
{
	bool modified = false;
	if (m.strNickName != strNickName) { m.strNickName = strNickName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void GenerationData_Proxy::SetUiAccmulOrRecvExp(const UInt32 &uiAccmulOrRecvExp)
{
	bool modified = false;
	if (m.uiAccmulOrRecvExp != uiAccmulOrRecvExp) { m.uiAccmulOrRecvExp = uiAccmulOrRecvExp; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void GenerationData_Proxy::SetUiGiftPointOrHistAccm(const UInt32 &uiGiftPointOrHistAccm)
{
	bool modified = false;
	if (m.uiGiftPointOrHistAccm != uiGiftPointOrHistAccm) { m.uiGiftPointOrHistAccm = uiGiftPointOrHistAccm; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void GenerationData_Proxy::SetFlag(const BOOL &bYoung)
{
	bool modified = false;
	if (m.bYoung != bYoung) { m.bYoung = bYoung; modified = true; }
	MarkUpdateAll(flagMask, modified);
}

void GenerationData_Proxy::SetBYoung(const BOOL &bYoung)
{
	bool modified = false;
	if (m.bYoung != bYoung) { m.bYoung = bYoung; modified = true; }
	MarkUpdateAll(flagMask, modified);
}


void GenerationData_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idsMask))
	{
		buf.Read<UInt32>(m.uiYoungerId);
		buf.Read<UInt32>(m.uiElderId);
	}
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<String>(m.strNickName);
		buf.Read<UInt32>(m.uiAccmulOrRecvExp);
		buf.Read<UInt32>(m.uiGiftPointOrHistAccm);
	}
	if (m_updateMask.IsBit(flagMask))
	{
		buf.Read<BOOL>(m.bYoung);
	}
}

void GenerationData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idsMask))
	{
		buf.Write<UInt32>(m.uiYoungerId);
		buf.Write<UInt32>(m.uiElderId);
	}
	if (mask.IsBit(infoMask))
	{
		buf.Write<String>(m.strNickName);
		buf.Write<UInt32>(m.uiAccmulOrRecvExp);
		buf.Write<UInt32>(m.uiGiftPointOrHistAccm);
	}
	if (mask.IsBit(flagMask))
	{
		buf.Write<BOOL>(m.bYoung);
	}
}

void GenerationData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idsMask)) OnUpdateIds(m.uiYoungerId, m.uiElderId);
	if (IsUpdated(infoMask)) OnUpdateInfo(m.strNickName, m.uiAccmulOrRecvExp, m.uiGiftPointOrHistAccm);
	if (IsUpdated(flagMask)) OnUpdateFlag(m.bYoung);
}

#pragma optimize("", off)
RPCResult GenerationData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_GenerationData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult GenerationData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_GenerationData_UP_STATE:
			_result = RPCDecode<2008179101>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
