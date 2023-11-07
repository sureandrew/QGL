//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/SellYuanBao_stub.h"

void SellYuanBao_Stub::SetId(const UInt32 &sellYuanBao_id)
{
	bool modified = false;
	if (m.sellYuanBao_id != sellYuanBao_id) { m.sellYuanBao_id = sellYuanBao_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void SellYuanBao_Stub::SetSellYuanBao_id(const UInt32 &sellYuanBao_id)
{
	bool modified = false;
	if (m.sellYuanBao_id != sellYuanBao_id) { m.sellYuanBao_id = sellYuanBao_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void SellYuanBao_Stub::SetCharInfo(const UInt32 &char_id, const String &charName)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	if (m.charName != charName) { m.charName = charName; modified = true; }
	MarkUpdateAll(charInfoMask, modified);
}

void SellYuanBao_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(charInfoMask, modified);
}

void SellYuanBao_Stub::SetCharName(const String &charName)
{
	bool modified = false;
	if (m.charName != charName) { m.charName = charName; modified = true; }
	MarkUpdateAll(charInfoMask, modified);
}

void SellYuanBao_Stub::SetSellInfo(const UInt8 &sellType, const UInt32 &totalYBT, const UInt32 &moneyPerYBT, const UInt32 &expireDate)
{
	bool modified = false;
	if (m.sellType != sellType) { m.sellType = sellType; modified = true; }
	if (m.totalYBT != totalYBT) { m.totalYBT = totalYBT; modified = true; }
	if (m.moneyPerYBT != moneyPerYBT) { m.moneyPerYBT = moneyPerYBT; modified = true; }
	if (m.expireDate != expireDate) { m.expireDate = expireDate; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBao_Stub::SetSellType(const UInt8 &sellType)
{
	bool modified = false;
	if (m.sellType != sellType) { m.sellType = sellType; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBao_Stub::SetTotalYBT(const UInt32 &totalYBT)
{
	bool modified = false;
	if (m.totalYBT != totalYBT) { m.totalYBT = totalYBT; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBao_Stub::SetMoneyPerYBT(const UInt32 &moneyPerYBT)
{
	bool modified = false;
	if (m.moneyPerYBT != moneyPerYBT) { m.moneyPerYBT = moneyPerYBT; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBao_Stub::SetExpireDate(const UInt32 &expireDate)
{
	bool modified = false;
	if (m.expireDate != expireDate) { m.expireDate = expireDate; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBao_Stub::SetSoldYBT(const UInt32 &soldYBT)
{
	bool modified = false;
	if (m.soldYBT != soldYBT) { m.soldYBT = soldYBT; modified = true; }
	MarkUpdateAll(soldYBTMask, modified);
}



void SellYuanBao_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void SellYuanBao_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.sellYuanBao_id);
	}
	if (mask.IsBit(charInfoMask))
	{
		buf.Write<UInt32>(m.char_id);
		buf.Write<String>(m.charName);
	}
	if (mask.IsBit(sellInfoMask))
	{
		buf.Write<UInt8>(m.sellType);
		buf.Write<UInt32>(m.totalYBT);
		buf.Write<UInt32>(m.moneyPerYBT);
		buf.Write<UInt32>(m.expireDate);
	}
	if (mask.IsBit(soldYBTMask))
	{
		buf.Write<UInt32>(m.soldYBT);
	}
}

RPCResult SellYuanBao_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SellYuanBao_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult SellYuanBao_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_SellYuanBao_REQ_STATE:
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

