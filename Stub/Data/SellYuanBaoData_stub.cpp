//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/SellYuanBaoData_stub.h"

void SellYuanBaoData_Stub::SetId(const UInt32 &sellYuanBao_id)
{
	bool modified = false;
	if (m.sellYuanBao_id != sellYuanBao_id) { m.sellYuanBao_id = sellYuanBao_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void SellYuanBaoData_Stub::SetSellYuanBao_id(const UInt32 &sellYuanBao_id)
{
	bool modified = false;
	if (m.sellYuanBao_id != sellYuanBao_id) { m.sellYuanBao_id = sellYuanBao_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void SellYuanBaoData_Stub::SetCharInfo(const UInt32 &char_id, const String &charName)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	if (m.charName != charName) { m.charName = charName; modified = true; }
	MarkUpdateAll(charInfoMask, modified);
}

void SellYuanBaoData_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(charInfoMask, modified);
}

void SellYuanBaoData_Stub::SetCharName(const String &charName)
{
	bool modified = false;
	if (m.charName != charName) { m.charName = charName; modified = true; }
	MarkUpdateAll(charInfoMask, modified);
}

void SellYuanBaoData_Stub::SetSellInfo(const UInt8 &sellType, const UInt32 &totalYBT, const UInt32 &moneyPerYBT, const UInt32 &expireDate)
{
	bool modified = false;
	if (m.sellType != sellType) { m.sellType = sellType; modified = true; }
	if (m.totalYBT != totalYBT) { m.totalYBT = totalYBT; modified = true; }
	if (m.moneyPerYBT != moneyPerYBT) { m.moneyPerYBT = moneyPerYBT; modified = true; }
	if (m.expireDate != expireDate) { m.expireDate = expireDate; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBaoData_Stub::SetSellType(const UInt8 &sellType)
{
	bool modified = false;
	if (m.sellType != sellType) { m.sellType = sellType; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBaoData_Stub::SetTotalYBT(const UInt32 &totalYBT)
{
	bool modified = false;
	if (m.totalYBT != totalYBT) { m.totalYBT = totalYBT; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBaoData_Stub::SetMoneyPerYBT(const UInt32 &moneyPerYBT)
{
	bool modified = false;
	if (m.moneyPerYBT != moneyPerYBT) { m.moneyPerYBT = moneyPerYBT; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBaoData_Stub::SetExpireDate(const UInt32 &expireDate)
{
	bool modified = false;
	if (m.expireDate != expireDate) { m.expireDate = expireDate; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBaoData_Stub::SetSoldYBT(const UInt32 &soldYBT)
{
	bool modified = false;
	if (m.soldYBT != soldYBT) { m.soldYBT = soldYBT; modified = true; }
	MarkUpdateAll(soldYBTMask, modified);
}


void SellYuanBaoData_Stub::Clone(SellYuanBaoData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(idMask) && !(obj->m.sellYuanBao_id == m.sellYuanBao_id)) {
		m.sellYuanBao_id = obj->m.sellYuanBao_id;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(charInfoMask) && !(obj->m.char_id == m.char_id)) {
		m.char_id = obj->m.char_id;
		MarkUpdateAll(charInfoMask, true);
	}
	if (mask.IsBit(charInfoMask) && !(obj->m.charName == m.charName)) {
		m.charName = obj->m.charName;
		MarkUpdateAll(charInfoMask, true);
	}
	if (mask.IsBit(sellInfoMask) && !(obj->m.sellType == m.sellType)) {
		m.sellType = obj->m.sellType;
		MarkUpdateAll(sellInfoMask, true);
	}
	if (mask.IsBit(sellInfoMask) && !(obj->m.totalYBT == m.totalYBT)) {
		m.totalYBT = obj->m.totalYBT;
		MarkUpdateAll(sellInfoMask, true);
	}
	if (mask.IsBit(sellInfoMask) && !(obj->m.moneyPerYBT == m.moneyPerYBT)) {
		m.moneyPerYBT = obj->m.moneyPerYBT;
		MarkUpdateAll(sellInfoMask, true);
	}
	if (mask.IsBit(sellInfoMask) && !(obj->m.expireDate == m.expireDate)) {
		m.expireDate = obj->m.expireDate;
		MarkUpdateAll(sellInfoMask, true);
	}
	if (mask.IsBit(soldYBTMask) && !(obj->m.soldYBT == m.soldYBT)) {
		m.soldYBT = obj->m.soldYBT;
		MarkUpdateAll(soldYBTMask, true);
	}
}

void SellYuanBaoData_Stub::ReadAttr(CONST Buffer &buf)
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

void SellYuanBaoData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult SellYuanBaoData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SellYuanBaoData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult SellYuanBaoData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_SellYuanBaoData_REQ_STATE:
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

