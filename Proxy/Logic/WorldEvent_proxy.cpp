//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/WorldEvent_proxy.h"


void WorldEvent_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<UInt16>(m.eventId);
		buf.Read<UInt8>(m.eventType);
		buf.Read<UInt8>(m.mapType);
		buf.Read<UInt32>(m.endTime);
	}
	if (m_updateMask.IsBit(npcListMask))
	{
		buf.Read<NpcInfoVec>(m.npcList);
		buf.Read<UInt16Vector>(m.npcLineCounts);
	}
	if (m_updateMask.IsBit(npcLiveTimeMask))
	{
		buf.Read<UInt32>(m.npcLiveTime);
	}
	if (m_updateMask.IsBit(buffEventMask))
	{
		buf.Read<UInt16Vector>(m.buffList);
		buf.Read<UInt16>(m.buffPeriod);
		buf.Read<Boolean>(m.buffClear);
		buf.Read<Boolean>(m.buffLeave);
	}
	if (m_updateMask.IsBit(mapListMask))
	{
		buf.Read<UInt16Vector>(m.mapList);
	}
	if (m_updateMask.IsBit(scriptMask))
	{
		buf.Read<String>(m.script);
	}
}

void WorldEvent_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void WorldEvent_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(infoMask)) OnUpdateInfo(m.eventId, m.eventType, m.mapType, m.endTime);
	if (IsUpdated(npcListMask)) OnUpdateNpcList(m.npcList, m.npcLineCounts);
	if (IsUpdated(npcLiveTimeMask)) OnUpdateNpcLiveTime(m.npcLiveTime);
	if (IsUpdated(buffEventMask)) OnUpdateBuffEvent(m.buffList, m.buffPeriod, m.buffClear, m.buffLeave);
	if (IsUpdated(mapListMask)) OnUpdateMapList(m.mapList);
	if (IsUpdated(scriptMask)) OnUpdateScript(m.script);
}

#pragma optimize("", off)
RPCResult WorldEvent_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_WorldEvent_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult WorldEvent_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_WorldEvent_UP_STATE:
			_result = RPCDecode<909051045>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
