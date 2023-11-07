//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/SellYuanBaoData_proxy.h"

void SellYuanBaoData_Proxy::SetId(const UInt32 &sellYuanBao_id)
{
	bool modified = false;
	if (m.sellYuanBao_id != sellYuanBao_id) { m.sellYuanBao_id = sellYuanBao_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void SellYuanBaoData_Proxy::SetSellYuanBao_id(const UInt32 &sellYuanBao_id)
{
	bool modified = false;
	if (m.sellYuanBao_id != sellYuanBao_id) { m.sellYuanBao_id = sellYuanBao_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void SellYuanBaoData_Proxy::SetCharInfo(const UInt32 &char_id, const String &charName)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	if (m.charName != charName) { m.charName = charName; modified = true; }
	MarkUpdateAll(charInfoMask, modified);
}

void SellYuanBaoData_Proxy::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(charInfoMask, modified);
}

void SellYuanBaoData_Proxy::SetCharName(const String &charName)
{
	bool modified = false;
	if (m.charName != charName) { m.charName = charName; modified = true; }
	MarkUpdateAll(charInfoMask, modified);
}

void SellYuanBaoData_Proxy::SetSellInfo(const UInt8 &sellType, const UInt32 &totalYBT, const UInt32 &moneyPerYBT, const UInt32 &expireDate)
{
	bool modified = false;
	if (m.sellType != sellType) { m.sellType = sellType; modified = true; }
	if (m.totalYBT != totalYBT) { m.totalYBT = totalYBT; modified = true; }
	if (m.moneyPerYBT != moneyPerYBT) { m.moneyPerYBT = moneyPerYBT; modified = true; }
	if (m.expireDate != expireDate) { m.expireDate = expireDate; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBaoData_Proxy::SetSellType(const UInt8 &sellType)
{
	bool modified = false;
	if (m.sellType != sellType) { m.sellType = sellType; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBaoData_Proxy::SetTotalYBT(const UInt32 &totalYBT)
{
	bool modified = false;
	if (m.totalYBT != totalYBT) { m.totalYBT = totalYBT; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBaoData_Proxy::SetMoneyPerYBT(const UInt32 &moneyPerYBT)
{
	bool modified = false;
	if (m.moneyPerYBT != moneyPerYBT) { m.moneyPerYBT = moneyPerYBT; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBaoData_Proxy::SetExpireDate(const UInt32 &expireDate)
{
	bool modified = false;
	if (m.expireDate != expireDate) { m.expireDate = expireDate; modified = true; }
	MarkUpdateAll(sellInfoMask, modified);
}

void SellYuanBaoData_Proxy::SetSoldYBT(const UInt32 &soldYBT)
{
	bool modified = false;
	if (m.soldYBT != soldYBT) { m.soldYBT = soldYBT; modified = true; }
	MarkUpdateAll(soldYBTMask, modified);
}


void SellYuanBaoData_Proxy::ReadAttr(CONST Buffer &buf)
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

void SellYuanBaoData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

void SellYuanBaoData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.sellYuanBao_id);
	if (IsUpdated(charInfoMask)) OnUpdateCharInfo(m.char_id, m.charName);
	if (IsUpdated(sellInfoMask)) OnUpdateSellInfo(m.sellType, m.totalYBT, m.moneyPerYBT, m.expireDate);
	if (IsUpdated(soldYBTMask)) OnUpdateSoldYBT(m.soldYBT);
}

#pragma optimize("", off)
RPCResult SellYuanBaoData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_SellYuanBaoData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult SellYuanBaoData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_SellYuanBaoData_UP_STATE:
			_result = RPCDecode<515912530>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
