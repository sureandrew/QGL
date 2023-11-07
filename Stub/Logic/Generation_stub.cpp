//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Generation_stub.h"

void Generation_Stub::SetIds(const UInt32 &uiRelatedId)
{
	bool modified = false;
	if (m.uiRelatedId != uiRelatedId) { m.uiRelatedId = uiRelatedId; modified = true; }
	MarkUpdateAll(idsMask, modified);
}

void Generation_Stub::SetUiRelatedId(const UInt32 &uiRelatedId)
{
	bool modified = false;
	if (m.uiRelatedId != uiRelatedId) { m.uiRelatedId = uiRelatedId; modified = true; }
	MarkUpdateAll(idsMask, modified);
}

void Generation_Stub::SetInfo(const String &strNickName, const UInt8 &ui8Class, const UInt16 &ui16Level, const UInt32 &uiOfflineTime, const UInt32 &uiAccmulOrRecvExp, const UInt32 &uiGiftPointOrHistAccm)
{
	bool modified = false;
	if (m.strNickName != strNickName) { m.strNickName = strNickName; modified = true; }
	if (m.ui8Class != ui8Class) { m.ui8Class = ui8Class; modified = true; }
	if (m.ui16Level != ui16Level) { m.ui16Level = ui16Level; modified = true; }
	if (m.uiOfflineTime != uiOfflineTime) { m.uiOfflineTime = uiOfflineTime; modified = true; }
	if (m.uiAccmulOrRecvExp != uiAccmulOrRecvExp) { m.uiAccmulOrRecvExp = uiAccmulOrRecvExp; modified = true; }
	if (m.uiGiftPointOrHistAccm != uiGiftPointOrHistAccm) { m.uiGiftPointOrHistAccm = uiGiftPointOrHistAccm; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Generation_Stub::SetStrNickName(const String &strNickName)
{
	bool modified = false;
	if (m.strNickName != strNickName) { m.strNickName = strNickName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Generation_Stub::SetUi8Class(const UInt8 &ui8Class)
{
	bool modified = false;
	if (m.ui8Class != ui8Class) { m.ui8Class = ui8Class; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Generation_Stub::SetUi16Level(const UInt16 &ui16Level)
{
	bool modified = false;
	if (m.ui16Level != ui16Level) { m.ui16Level = ui16Level; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Generation_Stub::SetUiOfflineTime(const UInt32 &uiOfflineTime)
{
	bool modified = false;
	if (m.uiOfflineTime != uiOfflineTime) { m.uiOfflineTime = uiOfflineTime; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Generation_Stub::SetUiAccmulOrRecvExp(const UInt32 &uiAccmulOrRecvExp)
{
	bool modified = false;
	if (m.uiAccmulOrRecvExp != uiAccmulOrRecvExp) { m.uiAccmulOrRecvExp = uiAccmulOrRecvExp; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Generation_Stub::SetUiGiftPointOrHistAccm(const UInt32 &uiGiftPointOrHistAccm)
{
	bool modified = false;
	if (m.uiGiftPointOrHistAccm != uiGiftPointOrHistAccm) { m.uiGiftPointOrHistAccm = uiGiftPointOrHistAccm; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Generation_Stub::SetFlag(const BOOL &bYoung)
{
	bool modified = false;
	if (m.bYoung != bYoung) { m.bYoung = bYoung; modified = true; }
	MarkUpdateAll(flagMask, modified);
}

void Generation_Stub::SetBYoung(const BOOL &bYoung)
{
	bool modified = false;
	if (m.bYoung != bYoung) { m.bYoung = bYoung; modified = true; }
	MarkUpdateAll(flagMask, modified);
}



void Generation_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Generation_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idsMask))
	{
		buf.Write<UInt32>(m.uiRelatedId);
	}
	if (mask.IsBit(infoMask))
	{
		buf.Write<String>(m.strNickName);
		buf.Write<UInt8>(m.ui8Class);
		buf.Write<UInt16>(m.ui16Level);
		buf.Write<UInt32>(m.uiOfflineTime);
		buf.Write<UInt32>(m.uiAccmulOrRecvExp);
		buf.Write<UInt32>(m.uiGiftPointOrHistAccm);
	}
	if (mask.IsBit(flagMask))
	{
		buf.Write<BOOL>(m.bYoung);
	}
}

RPCResult Generation_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Generation_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Generation_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Generation_REQ_STATE:
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

