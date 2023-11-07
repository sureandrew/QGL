//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/WorldEvent_stub.h"

void WorldEvent_Stub::SetInfo(const UInt16 &eventId, const UInt8 &eventType, const UInt8 &mapType, const UInt32 &endTime)
{
	bool modified = false;
	if (m.eventId != eventId) { m.eventId = eventId; modified = true; }
	if (m.eventType != eventType) { m.eventType = eventType; modified = true; }
	if (m.mapType != mapType) { m.mapType = mapType; modified = true; }
	if (m.endTime != endTime) { m.endTime = endTime; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void WorldEvent_Stub::SetEventId(const UInt16 &eventId)
{
	bool modified = false;
	if (m.eventId != eventId) { m.eventId = eventId; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void WorldEvent_Stub::SetEventType(const UInt8 &eventType)
{
	bool modified = false;
	if (m.eventType != eventType) { m.eventType = eventType; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void WorldEvent_Stub::SetMapType(const UInt8 &mapType)
{
	bool modified = false;
	if (m.mapType != mapType) { m.mapType = mapType; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void WorldEvent_Stub::SetEndTime(const UInt32 &endTime)
{
	bool modified = false;
	if (m.endTime != endTime) { m.endTime = endTime; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void WorldEvent_Stub::SetNpcList(const NpcInfoVec &npcList, const UInt16Vector &npcLineCounts)
{
	bool modified = false;
	if (m.npcList != npcList) { m.npcList = npcList; modified = true; }
	if (m.npcLineCounts != npcLineCounts) { m.npcLineCounts = npcLineCounts; modified = true; }
	MarkUpdateAll(npcListMask, modified);
}

void WorldEvent_Stub::SetNpcList(const NpcInfoVec &npcList)
{
	bool modified = false;
	if (m.npcList != npcList) { m.npcList = npcList; modified = true; }
	MarkUpdateAll(npcListMask, modified);
}

void WorldEvent_Stub::SetNpcLineCounts(const UInt16Vector &npcLineCounts)
{
	bool modified = false;
	if (m.npcLineCounts != npcLineCounts) { m.npcLineCounts = npcLineCounts; modified = true; }
	MarkUpdateAll(npcListMask, modified);
}

void WorldEvent_Stub::SetNpcLiveTime(const UInt32 &npcLiveTime)
{
	bool modified = false;
	if (m.npcLiveTime != npcLiveTime) { m.npcLiveTime = npcLiveTime; modified = true; }
	MarkUpdateAll(npcLiveTimeMask, modified);
}

void WorldEvent_Stub::SetBuffEvent(const UInt16Vector &buffList, const UInt16 &buffPeriod, const Boolean &buffClear, const Boolean &buffLeave)
{
	bool modified = false;
	if (m.buffList != buffList) { m.buffList = buffList; modified = true; }
	if (m.buffPeriod != buffPeriod) { m.buffPeriod = buffPeriod; modified = true; }
	if (m.buffClear != buffClear) { m.buffClear = buffClear; modified = true; }
	if (m.buffLeave != buffLeave) { m.buffLeave = buffLeave; modified = true; }
	MarkUpdateAll(buffEventMask, modified);
}

void WorldEvent_Stub::SetBuffList(const UInt16Vector &buffList)
{
	bool modified = false;
	if (m.buffList != buffList) { m.buffList = buffList; modified = true; }
	MarkUpdateAll(buffEventMask, modified);
}

void WorldEvent_Stub::SetBuffPeriod(const UInt16 &buffPeriod)
{
	bool modified = false;
	if (m.buffPeriod != buffPeriod) { m.buffPeriod = buffPeriod; modified = true; }
	MarkUpdateAll(buffEventMask, modified);
}

void WorldEvent_Stub::SetBuffClear(const Boolean &buffClear)
{
	bool modified = false;
	if (m.buffClear != buffClear) { m.buffClear = buffClear; modified = true; }
	MarkUpdateAll(buffEventMask, modified);
}

void WorldEvent_Stub::SetBuffLeave(const Boolean &buffLeave)
{
	bool modified = false;
	if (m.buffLeave != buffLeave) { m.buffLeave = buffLeave; modified = true; }
	MarkUpdateAll(buffEventMask, modified);
}

void WorldEvent_Stub::SetMapList(const UInt16Vector &mapList)
{
	bool modified = false;
	if (m.mapList != mapList) { m.mapList = mapList; modified = true; }
	MarkUpdateAll(mapListMask, modified);
}

void WorldEvent_Stub::SetScript(const String &script)
{
	bool modified = false;
	if (m.script != script) { m.script = script; modified = true; }
	MarkUpdateAll(scriptMask, modified);
}



void WorldEvent_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void WorldEvent_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(infoMask))
	{
		buf.Write<UInt16>(m.eventId);
		buf.Write<UInt8>(m.eventType);
		buf.Write<UInt8>(m.mapType);
		buf.Write<UInt32>(m.endTime);
	}
	if (mask.IsBit(npcListMask))
	{
		buf.Write<NpcInfoVec>(m.npcList);
		buf.Write<UInt16Vector>(m.npcLineCounts);
	}
	if (mask.IsBit(npcLiveTimeMask))
	{
		buf.Write<UInt32>(m.npcLiveTime);
	}
	if (mask.IsBit(buffEventMask))
	{
		buf.Write<UInt16Vector>(m.buffList);
		buf.Write<UInt16>(m.buffPeriod);
		buf.Write<Boolean>(m.buffClear);
		buf.Write<Boolean>(m.buffLeave);
	}
	if (mask.IsBit(mapListMask))
	{
		buf.Write<UInt16Vector>(m.mapList);
	}
	if (mask.IsBit(scriptMask))
	{
		buf.Write<String>(m.script);
	}
}

RPCResult WorldEvent_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldEvent_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult WorldEvent_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_WorldEvent_REQ_STATE:
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

