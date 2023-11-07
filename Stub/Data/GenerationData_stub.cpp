//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/GenerationData_stub.h"

void GenerationData_Stub::SetIds(const UInt32 &uiYoungerId, const UInt32 &uiElderId)
{
	bool modified = false;
	if (m.uiYoungerId != uiYoungerId) { m.uiYoungerId = uiYoungerId; modified = true; }
	if (m.uiElderId != uiElderId) { m.uiElderId = uiElderId; modified = true; }
	MarkUpdateAll(idsMask, modified);
}

void GenerationData_Stub::SetUiYoungerId(const UInt32 &uiYoungerId)
{
	bool modified = false;
	if (m.uiYoungerId != uiYoungerId) { m.uiYoungerId = uiYoungerId; modified = true; }
	MarkUpdateAll(idsMask, modified);
}

void GenerationData_Stub::SetUiElderId(const UInt32 &uiElderId)
{
	bool modified = false;
	if (m.uiElderId != uiElderId) { m.uiElderId = uiElderId; modified = true; }
	MarkUpdateAll(idsMask, modified);
}

void GenerationData_Stub::SetInfo(const String &strNickName, const UInt32 &uiAccmulOrRecvExp, const UInt32 &uiGiftPointOrHistAccm)
{
	bool modified = false;
	if (m.strNickName != strNickName) { m.strNickName = strNickName; modified = true; }
	if (m.uiAccmulOrRecvExp != uiAccmulOrRecvExp) { m.uiAccmulOrRecvExp = uiAccmulOrRecvExp; modified = true; }
	if (m.uiGiftPointOrHistAccm != uiGiftPointOrHistAccm) { m.uiGiftPointOrHistAccm = uiGiftPointOrHistAccm; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void GenerationData_Stub::SetStrNickName(const String &strNickName)
{
	bool modified = false;
	if (m.strNickName != strNickName) { m.strNickName = strNickName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void GenerationData_Stub::SetUiAccmulOrRecvExp(const UInt32 &uiAccmulOrRecvExp)
{
	bool modified = false;
	if (m.uiAccmulOrRecvExp != uiAccmulOrRecvExp) { m.uiAccmulOrRecvExp = uiAccmulOrRecvExp; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void GenerationData_Stub::SetUiGiftPointOrHistAccm(const UInt32 &uiGiftPointOrHistAccm)
{
	bool modified = false;
	if (m.uiGiftPointOrHistAccm != uiGiftPointOrHistAccm) { m.uiGiftPointOrHistAccm = uiGiftPointOrHistAccm; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void GenerationData_Stub::SetFlag(const BOOL &bYoung)
{
	bool modified = false;
	if (m.bYoung != bYoung) { m.bYoung = bYoung; modified = true; }
	MarkUpdateAll(flagMask, modified);
}

void GenerationData_Stub::SetBYoung(const BOOL &bYoung)
{
	bool modified = false;
	if (m.bYoung != bYoung) { m.bYoung = bYoung; modified = true; }
	MarkUpdateAll(flagMask, modified);
}


void GenerationData_Stub::Clone(GenerationData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(idsMask) && !(obj->m.uiYoungerId == m.uiYoungerId)) {
		m.uiYoungerId = obj->m.uiYoungerId;
		MarkUpdateAll(idsMask, true);
	}
	if (mask.IsBit(idsMask) && !(obj->m.uiElderId == m.uiElderId)) {
		m.uiElderId = obj->m.uiElderId;
		MarkUpdateAll(idsMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.strNickName == m.strNickName)) {
		m.strNickName = obj->m.strNickName;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.uiAccmulOrRecvExp == m.uiAccmulOrRecvExp)) {
		m.uiAccmulOrRecvExp = obj->m.uiAccmulOrRecvExp;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.uiGiftPointOrHistAccm == m.uiGiftPointOrHistAccm)) {
		m.uiGiftPointOrHistAccm = obj->m.uiGiftPointOrHistAccm;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(flagMask) && !(obj->m.bYoung == m.bYoung)) {
		m.bYoung = obj->m.bYoung;
		MarkUpdateAll(flagMask, true);
	}
}

void GenerationData_Stub::ReadAttr(CONST Buffer &buf)
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

void GenerationData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult GenerationData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GenerationData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult GenerationData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_GenerationData_REQ_STATE:
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

