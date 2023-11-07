//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/QuestType_proxy.h"


void QuestType_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.char_id);
		buf.Read<UInt16>(m.Type_id);
	}
	if (m_updateMask.IsBit(nameMask))
	{
		buf.Read<String>(m.name);
	}
	if (m_updateMask.IsBit(roundMask))
	{
		buf.Read<UInt16>(m.round);
	}
	if (m_updateMask.IsBit(maxRoundMask))
	{
		buf.Read<UInt16>(m.maxRound);
	}
	if (m_updateMask.IsBit(totalRoundMask))
	{
		buf.Read<UInt16>(m.totalRound);
	}
	if (m_updateMask.IsBit(limitRoundMask))
	{
		buf.Read<UInt16>(m.limitRound);
	}
	if (m_updateMask.IsBit(limitTypeMask))
	{
		buf.Read<UInt16>(m.limitType);
	}
	if (m_updateMask.IsBit(lastClearTimeMask))
	{
		buf.Read<UInt32>(m.lastClearTime);
	}
	if (m_updateMask.IsBit(statusMask))
	{
		buf.Read<UInt8>(m.status);
	}
}

void QuestType_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void QuestType_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.char_id, m.Type_id);
	if (IsUpdated(nameMask)) OnUpdateName(m.name);
	if (IsUpdated(roundMask)) OnUpdateRound(m.round);
	if (IsUpdated(maxRoundMask)) OnUpdateMaxRound(m.maxRound);
	if (IsUpdated(totalRoundMask)) OnUpdateTotalRound(m.totalRound);
	if (IsUpdated(limitRoundMask)) OnUpdateLimitRound(m.limitRound);
	if (IsUpdated(limitTypeMask)) OnUpdateLimitType(m.limitType);
	if (IsUpdated(lastClearTimeMask)) OnUpdateLastClearTime(m.lastClearTime);
	if (IsUpdated(statusMask)) OnUpdateStatus(m.status);
}

#pragma optimize("", off)
RPCResult QuestType_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_QuestType_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult QuestType_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_QuestType_UP_STATE:
			_result = RPCDecode<169762045>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
