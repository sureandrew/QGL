//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/MapNPC_stub.h"

void MapNPC_Stub::SetId(const UInt32 &npc_uid, const UInt32 &npc_id)
{
	bool modified = false;
	if (m.npc_uid != npc_uid) { m.npc_uid = npc_uid; modified = true; }
	if (m.npc_id != npc_id) { m.npc_id = npc_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void MapNPC_Stub::SetNpc_uid(const UInt32 &npc_uid)
{
	bool modified = false;
	if (m.npc_uid != npc_uid) { m.npc_uid = npc_uid; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void MapNPC_Stub::SetNpc_id(const UInt32 &npc_id)
{
	bool modified = false;
	if (m.npc_id != npc_id) { m.npc_id = npc_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void MapNPC_Stub::SetVisible(const bool &visible)
{
	bool modified = false;
	if (m.visible != visible) { m.visible = visible; modified = true; }
	MarkUpdateAll(visibleMask, modified);
}



void MapNPC_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void MapNPC_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.npc_uid);
		buf.Write<UInt32>(m.npc_id);
	}
	if (mask.IsBit(visibleMask))
	{
		buf.Write<bool>(m.visible);
	}
}

RPCResult MapNPC_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapNPC_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult MapNPC_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_MapNPC_REQ_STATE:
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

