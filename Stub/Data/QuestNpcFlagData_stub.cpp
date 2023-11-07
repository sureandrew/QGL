//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/QuestNpcFlagData_stub.h"

void QuestNpcFlagData_Stub::SetId(const UInt32 &npc_id, const UInt16 &line_id, const UInt32 &owner_id, const UInt16 &flag_id)
{
	bool modified = false;
	if (m.npc_id != npc_id) { m.npc_id = npc_id; modified = true; }
	if (m.line_id != line_id) { m.line_id = line_id; modified = true; }
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	if (m.flag_id != flag_id) { m.flag_id = flag_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestNpcFlagData_Stub::SetNpc_id(const UInt32 &npc_id)
{
	bool modified = false;
	if (m.npc_id != npc_id) { m.npc_id = npc_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestNpcFlagData_Stub::SetLine_id(const UInt16 &line_id)
{
	bool modified = false;
	if (m.line_id != line_id) { m.line_id = line_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestNpcFlagData_Stub::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestNpcFlagData_Stub::SetFlag_id(const UInt16 &flag_id)
{
	bool modified = false;
	if (m.flag_id != flag_id) { m.flag_id = flag_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestNpcFlagData_Stub::SetFlag(const Int32 &flag)
{
	bool modified = false;
	if (m.flag != flag) { m.flag = flag; modified = true; }
	MarkUpdateAll(flagMask, modified);
}


void QuestNpcFlagData_Stub::Clone(QuestNpcFlagData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(idMask) && !(obj->m.npc_id == m.npc_id)) {
		m.npc_id = obj->m.npc_id;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(idMask) && !(obj->m.line_id == m.line_id)) {
		m.line_id = obj->m.line_id;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(idMask) && !(obj->m.owner_id == m.owner_id)) {
		m.owner_id = obj->m.owner_id;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(idMask) && !(obj->m.flag_id == m.flag_id)) {
		m.flag_id = obj->m.flag_id;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(flagMask) && !(obj->m.flag == m.flag)) {
		m.flag = obj->m.flag;
		MarkUpdateAll(flagMask, true);
	}
}

void QuestNpcFlagData_Stub::ReadAttr(CONST Buffer &buf)
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

void QuestNpcFlagData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult QuestNpcFlagData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_QuestNpcFlagData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult QuestNpcFlagData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_QuestNpcFlagData_REQ_STATE:
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

