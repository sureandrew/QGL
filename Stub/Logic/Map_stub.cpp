//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Map_stub.h"

void Map_Stub::SetInfo(const UInt16 &map_id, const UInt8 &line_id)
{
	bool modified = false;
	if (m.map_id != map_id) { m.map_id = map_id; modified = true; }
	if (m.line_id != line_id) { m.line_id = line_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Map_Stub::SetMap_id(const UInt16 &map_id)
{
	bool modified = false;
	if (m.map_id != map_id) { m.map_id = map_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Map_Stub::SetLine_id(const UInt8 &line_id)
{
	bool modified = false;
	if (m.line_id != line_id) { m.line_id = line_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Map_Stub::SetInstance(const UInt16 &inst_id, const UInt32 &owner_id)
{
	bool modified = false;
	if (m.inst_id != inst_id) { m.inst_id = inst_id; modified = true; }
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(instanceMask, modified);
}

void Map_Stub::SetInst_id(const UInt16 &inst_id)
{
	bool modified = false;
	if (m.inst_id != inst_id) { m.inst_id = inst_id; modified = true; }
	MarkUpdateAll(instanceMask, modified);
}

void Map_Stub::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(instanceMask, modified);
}



void Map_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Map_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(infoMask))
	{
		buf.Write<UInt16>(m.map_id);
		buf.Write<UInt8>(m.line_id);
	}
	if (mask.IsBit(instanceMask))
	{
		buf.Write<UInt16>(m.inst_id);
		buf.Write<UInt32>(m.owner_id);
	}
}

RPCResult Map_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Map_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Map_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Map_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_Map_GetCharInRange:
		{
			UInt16 centerX;
			pBuf->Read<UInt16>(centerX);
			UInt16 centerY;
			pBuf->Read<UInt16>(centerY);
			UInt16 range;
			pBuf->Read<UInt16>(range);
			_result = GetCharInRange(pPeer, context, centerX, centerY, range);
		}
		break;
	case RPC_Map_GetJumpPoints:
		{
			_result = GetJumpPoints(pPeer, context);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult Map_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Map_CB_Error);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(errCode);
	pBuf->Write<String>(errMsg);
	return RPCSend<1564824712>(pPeer, pBuf);
}

RPCResult Map_Stub::CB_GetCharInRange(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Map_CB_GetCharInRange);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend<814557048>(pPeer, pBuf);
}

RPCResult Map_Stub::CB_GetJumpPoints(LPCPEER pPeer, RPCContext &context, const JumpInfoVector &jumpInfoList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Map_CB_GetJumpPoints);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<JumpInfoVector>(jumpInfoList);
	return RPCSend<1947233008>(pPeer, pBuf);
}

RPCResult Map_Stub::CB_ObjectChanged(LPCPEER pPeer, RPCContext &context, NetGroup* grp, const RPCNetIDVector &delIds, const RPCNetIDVector &curIds)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Map_CB_ObjectChanged);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(grp);
	pBuf->Write<RPCNetIDVector>(delIds);
	pBuf->Write<RPCNetIDVector>(curIds);
	return RPCSend<1878792989>(pPeer, pBuf);
}

