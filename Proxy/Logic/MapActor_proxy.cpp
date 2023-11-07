//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/MapActor_proxy.h"


void MapActor_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<String>(m.nickName);
		buf.Read<UInt8>(m.sex);
		buf.Read<UInt8>(m.cclass);
		buf.Read<UInt8>(m.sexClass);
		buf.Read<UInt8>(m.faction);
	}
	if (m_updateMask.IsBit(stateMask))
	{
		buf.Read<UInt8>(m.state);
	}
	if (m_updateMask.IsBit(mapMask))
	{
		buf.Read<UInt16>(m.map_id);
		buf.Read<UInt16>(m.map_x);
		buf.Read<UInt16>(m.map_y);
		buf.Read<UInt8>(m.direction);
		buf.Read<UInt32>(m.map_ownerId);
	}
	if (m_updateMask.IsBit(rebornMask))
	{
		buf.Read<UInt16>(m.reborn_map);
		buf.Read<UInt16>(m.reborn_x);
		buf.Read<UInt16>(m.reborn_y);
	}
	if (m_updateMask.IsBit(curPosMask))
	{
		buf.Read<UInt16>(m.curPosX);
		buf.Read<UInt16>(m.curPosY);
	}
	if (m_updateMask.IsBit(dstPosMask))
	{
		buf.Read<UInt16>(m.dstPosX);
		buf.Read<UInt16>(m.dstPosY);
	}
	if (m_updateMask.IsBit(actionMask))
	{
		buf.Read<UInt16>(m.action);
		buf.Read<UInt32>(m.actionTime);
	}
	if (m_updateMask.IsBit(speedMask))
	{
		buf.Read<UInt32>(m.speed);
	}
	if (m_updateMask.IsBit(followIdMask))
	{
		buf.Read<UInt32>(m.followId);
		buf.Read<UInt8>(m.followOrder);
	}
	if (m_updateMask.IsBit(appearedMask))
	{
		buf.Read<Boolean>(m.appeared);
	}
}

void MapActor_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void MapActor_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(infoMask)) OnUpdateInfo(m.nickName, m.sex, m.cclass, m.sexClass, m.faction);
	if (IsUpdated(stateMask)) OnUpdateState(m.state);
	if (IsUpdated(mapMask)) OnUpdateMap(m.map_id, m.map_x, m.map_y, m.direction, m.map_ownerId);
	if (IsUpdated(rebornMask)) OnUpdateReborn(m.reborn_map, m.reborn_x, m.reborn_y);
	if (IsUpdated(curPosMask)) OnUpdateCurPos(m.curPosX, m.curPosY);
	if (IsUpdated(dstPosMask)) OnUpdateDstPos(m.dstPosX, m.dstPosY);
	if (IsUpdated(actionMask)) OnUpdateAction(m.action, m.actionTime);
	if (IsUpdated(speedMask)) OnUpdateSpeed(m.speed);
	if (IsUpdated(followIdMask)) OnUpdateFollowId(m.followId, m.followOrder);
	if (IsUpdated(appearedMask)) OnUpdateAppeared(m.appeared);
}

#pragma optimize("", off)
RPCResult MapActor_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_MapActor_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult MapActor_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_MapActor_UP_STATE:
			_result = RPCDecode<1882347406>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
