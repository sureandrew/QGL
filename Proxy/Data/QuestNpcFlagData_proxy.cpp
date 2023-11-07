//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/QuestNpcFlagData_proxy.h"

void QuestNpcFlagData_Proxy::SetId(const UInt32 &npc_id, const UInt16 &line_id, const UInt32 &owner_id, const UInt16 &flag_id)
{
	bool modified = false;
	if (m.npc_id != npc_id) { m.npc_id = npc_id; modified = true; }
	if (m.line_id != line_id) { m.line_id = line_id; modified = true; }
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	if (m.flag_id != flag_id) { m.flag_id = flag_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestNpcFlagData_Proxy::SetNpc_id(const UInt32 &npc_id)
{
	bool modified = false;
	if (m.npc_id != npc_id) { m.npc_id = npc_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestNpcFlagData_Proxy::SetLine_id(const UInt16 &line_id)
{
	bool modified = false;
	if (m.line_id != line_id) { m.line_id = line_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestNpcFlagData_Proxy::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestNpcFlagData_Proxy::SetFlag_id(const UInt16 &flag_id)
{
	bool modified = false;
	if (m.flag_id != flag_id) { m.flag_id = flag_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestNpcFlagData_Proxy::SetFlag(const Int32 &flag)
{
	bool modified = false;
	if (m.flag != flag) { m.flag = flag; modified = true; }
	MarkUpdateAll(flagMask, modified);
}


void QuestNpcFlagData_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.npc_id);
		buf.Read<UInt16>(m.line_id);
		buf.Read<UInt32>(m.owner_id);
		buf.Read<UInt16>(m.flag_id);
	}
	if (m_updateMask.IsBit(flagMask))
	{
		buf.Read<Int32>(m.flag);
	}
}

void QuestNpcFlagData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.npc_id);
		buf.Write<UInt16>(m.line_id);
		buf.Write<UInt32>(m.owner_id);
		buf.Write<UInt16>(m.flag_id);
	}
	if (mask.IsBit(flagMask))
	{
		buf.Write<Int32>(m.flag);
	}
}

void QuestNpcFlagData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.npc_id, m.line_id, m.owner_id, m.flag_id);
	if (IsUpdated(flagMask)) OnUpdateFlag(m.flag);
}

#pragma optimize("", off)
RPCResult QuestNpcFlagData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_QuestNpcFlagData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult QuestNpcFlagData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_QuestNpcFlagData_UP_STATE:
			_result = RPCDecode<1391466966>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
