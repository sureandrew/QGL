//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/QuestTypeData_proxy.h"

void QuestTypeData_Proxy::SetId(const UInt32 &char_id, const UInt16 &Type_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	if (m.Type_id != Type_id) { m.Type_id = Type_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestTypeData_Proxy::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestTypeData_Proxy::SetType_id(const UInt16 &Type_id)
{
	bool modified = false;
	if (m.Type_id != Type_id) { m.Type_id = Type_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestTypeData_Proxy::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(nameMask, modified);
}

void QuestTypeData_Proxy::SetRound(const UInt16 &round)
{
	bool modified = false;
	if (m.round != round) { m.round = round; modified = true; }
	MarkUpdateAll(roundMask, modified);
}

void QuestTypeData_Proxy::SetMaxRound(const UInt16 &maxRound)
{
	bool modified = false;
	if (m.maxRound != maxRound) { m.maxRound = maxRound; modified = true; }
	MarkUpdateAll(maxRoundMask, modified);
}

void QuestTypeData_Proxy::SetTotalRound(const UInt16 &totalRound)
{
	bool modified = false;
	if (m.totalRound != totalRound) { m.totalRound = totalRound; modified = true; }
	MarkUpdateAll(totalRoundMask, modified);
}

void QuestTypeData_Proxy::SetLimitRound(const UInt16 &limitRound)
{
	bool modified = false;
	if (m.limitRound != limitRound) { m.limitRound = limitRound; modified = true; }
	MarkUpdateAll(limitRoundMask, modified);
}

void QuestTypeData_Proxy::SetLimitType(const UInt16 &limitType)
{
	bool modified = false;
	if (m.limitType != limitType) { m.limitType = limitType; modified = true; }
	MarkUpdateAll(limitTypeMask, modified);
}

void QuestTypeData_Proxy::SetLastClearTime(const UInt32 &lastClearTime)
{
	bool modified = false;
	if (m.lastClearTime != lastClearTime) { m.lastClearTime = lastClearTime; modified = true; }
	MarkUpdateAll(lastClearTimeMask, modified);
}

void QuestTypeData_Proxy::SetStatus(const UInt8 &status)
{
	bool modified = false;
	if (m.status != status) { m.status = status; modified = true; }
	MarkUpdateAll(statusMask, modified);
}


void QuestTypeData_Proxy::ReadAttr(CONST Buffer &buf)
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

void QuestTypeData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.char_id);
		buf.Write<UInt16>(m.Type_id);
	}
	if (mask.IsBit(nameMask))
	{
		buf.Write<String>(m.name);
	}
	if (mask.IsBit(roundMask))
	{
		buf.Write<UInt16>(m.round);
	}
	if (mask.IsBit(maxRoundMask))
	{
		buf.Write<UInt16>(m.maxRound);
	}
	if (mask.IsBit(totalRoundMask))
	{
		buf.Write<UInt16>(m.totalRound);
	}
	if (mask.IsBit(limitRoundMask))
	{
		buf.Write<UInt16>(m.limitRound);
	}
	if (mask.IsBit(limitTypeMask))
	{
		buf.Write<UInt16>(m.limitType);
	}
	if (mask.IsBit(lastClearTimeMask))
	{
		buf.Write<UInt32>(m.lastClearTime);
	}
	if (mask.IsBit(statusMask))
	{
		buf.Write<UInt8>(m.status);
	}
}

void QuestTypeData_Proxy::OnUpdate()
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
RPCResult QuestTypeData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_QuestTypeData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult QuestTypeData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_QuestTypeData_UP_STATE:
			_result = RPCDecode<676208992>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
