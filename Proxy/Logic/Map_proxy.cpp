//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Map_proxy.h"


void Map_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<UInt16>(m.map_id);
		buf.Read<UInt8>(m.line_id);
	}
	if (m_updateMask.IsBit(instanceMask))
	{
		buf.Read<UInt16>(m.inst_id);
		buf.Read<UInt32>(m.owner_id);
	}
}

void Map_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Map_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(infoMask)) OnUpdateInfo(m.map_id, m.line_id);
	if (IsUpdated(instanceMask)) OnUpdateInstance(m.inst_id, m.owner_id);
}

#pragma optimize("", off)
RPCResult Map_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Map_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_Map_CB_Error:
		{
			UInt32 errCode;
			pBuf->Read<UInt32>(errCode);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, errCode, errMsg);
		}
		break;
	case RPC_Map_CB_GetCharInRange:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_GetCharInRange(pPeer, context, grp);
		}
		break;
	case RPC_Map_CB_GetJumpPoints:
		{
			JumpInfoVector jumpInfoList;
			pBuf->Read<JumpInfoVector>(jumpInfoList);
			_result = CB_GetJumpPoints(pPeer, context, jumpInfoList);
		}
		break;
	case RPC_Map_CB_ObjectChanged:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			RPCNetIDVector delIds;
			pBuf->Read<RPCNetIDVector>(delIds);
			RPCNetIDVector curIds;
			pBuf->Read<RPCNetIDVector>(curIds);
			_result = CB_ObjectChanged(pPeer, context, grp, delIds, curIds);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Map_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Map_UP_STATE:
			_result = RPCDecode<1831614892>(pPeer, pBuf);
		break;
	case RPC_Map_CB_Error:
			_result = RPCDecode<1564824712>(pPeer, pBuf);
		break;
	case RPC_Map_CB_GetCharInRange:
			_result = RPCDecode<814557048>(pPeer, pBuf);
		break;
	case RPC_Map_CB_GetJumpPoints:
			_result = RPCDecode<1947233008>(pPeer, pBuf);
		break;
	case RPC_Map_CB_ObjectChanged:
			_result = RPCDecode<1878792989>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
