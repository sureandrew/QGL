//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/QuestJournal_proxy.h"


void QuestJournal_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.journal_id);
	}
	if (m_updateMask.IsBit(nameMask))
	{
		buf.Read<String>(m.name);
	}
	if (m_updateMask.IsBit(levelMask))
	{
		buf.Read<UInt16>(m.minLv);
		buf.Read<UInt16>(m.maxLv);
	}
	if (m_updateMask.IsBit(descriptMask))
	{
		buf.Read<String>(m.descript);
	}
	if (m_updateMask.IsBit(teamDescriptMask))
	{
		buf.Read<String>(m.teamDescript);
	}
	if (m_updateMask.IsBit(timeStrMask))
	{
		buf.Read<String>(m.timeStr);
	}
	if (m_updateMask.IsBit(npcMask))
	{
		buf.Read<String>(m.npcName);
		buf.Read<String>(m.mapName);
	}
	if (m_updateMask.IsBit(roundMask))
	{
		buf.Read<UInt16>(m.Type_id);
		buf.Read<UInt16>(m.limitRound);
	}
	if (m_updateMask.IsBit(enableMask))
	{
		buf.Read<Boolean>(m.enable);
	}
	if (m_updateMask.IsBit(DBClickScriptMask))
	{
		buf.Read<String>(m.DBClickScript);
	}
}

void QuestJournal_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void QuestJournal_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.journal_id);
	if (IsUpdated(nameMask)) OnUpdateName(m.name);
	if (IsUpdated(levelMask)) OnUpdateLevel(m.minLv, m.maxLv);
	if (IsUpdated(descriptMask)) OnUpdateDescript(m.descript);
	if (IsUpdated(teamDescriptMask)) OnUpdateTeamDescript(m.teamDescript);
	if (IsUpdated(timeStrMask)) OnUpdateTimeStr(m.timeStr);
	if (IsUpdated(npcMask)) OnUpdateNpc(m.npcName, m.mapName);
	if (IsUpdated(roundMask)) OnUpdateRound(m.Type_id, m.limitRound);
	if (IsUpdated(enableMask)) OnUpdateEnable(m.enable);
	if (IsUpdated(DBClickScriptMask)) OnUpdateDBClickScript(m.DBClickScript);
}

#pragma optimize("", off)
RPCResult QuestJournal_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_QuestJournal_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult QuestJournal_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_QuestJournal_UP_STATE:
			_result = RPCDecode<1715610412>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
