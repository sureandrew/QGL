//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/RankCharacter_proxy.h"


void RankCharacter_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.acct_id);
		buf.Read<UInt32>(m.char_id);
	}
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<String>(m.nickName);
		buf.Read<UInt8>(m.sex);
		buf.Read<UInt8>(m.cclass);
		buf.Read<UInt8>(m.sexClass);
		buf.Read<UInt8>(m.faction);
	}
	if (m_updateMask.IsBit(levelMask))
	{
		buf.Read<UInt16>(m.level);
	}
	if (m_updateMask.IsBit(moneyMask))
	{
		buf.Read<UInt32>(m.money);
	}
	if (m_updateMask.IsBit(PKInfoMask))
	{
		buf.Read<UInt16>(m.PK_Value);
		buf.Read<UInt32>(m.wantedReward);
	}
	if (m_updateMask.IsBit(honorMask))
	{
		buf.Read<UInt32>(m.honor);
	}
	if (m_updateMask.IsBit(bankMoneyMask))
	{
		buf.Read<UInt32>(m.bankMoney);
	}
	if (m_updateMask.IsBit(PvPScoreSingleMask))
	{
		buf.Read<UInt32>(m.PvPScoreSingle);
	}
}

void RankCharacter_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void RankCharacter_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.acct_id, m.char_id);
	if (IsUpdated(infoMask)) OnUpdateInfo(m.nickName, m.sex, m.cclass, m.sexClass, m.faction);
	if (IsUpdated(levelMask)) OnUpdateLevel(m.level);
	if (IsUpdated(moneyMask)) OnUpdateMoney(m.money);
	if (IsUpdated(PKInfoMask)) OnUpdatePKInfo(m.PK_Value, m.wantedReward);
	if (IsUpdated(honorMask)) OnUpdateHonor(m.honor);
	if (IsUpdated(bankMoneyMask)) OnUpdateBankMoney(m.bankMoney);
	if (IsUpdated(PvPScoreSingleMask)) OnUpdatePvPScoreSingle(m.PvPScoreSingle);
}

#pragma optimize("", off)
RPCResult RankCharacter_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_RankCharacter_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult RankCharacter_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_RankCharacter_UP_STATE:
			_result = RPCDecode<307298890>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
