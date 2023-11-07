///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_WORLDEVENT_STUB_H_
#define _RPC_WORLDEVENT_STUB_H_

#include "Common/RPCFunc.h"
struct NpcInfo
{
	UInt32 npcId;
	UInt16 mapId;
	bool operator == (const NpcInfo& b) const;
	bool operator < (const NpcInfo& b) const;
};
typedef StlVector<NpcInfo, ManagedAllocator<NpcInfo> > NpcInfoVec;
#include "RPCEnum/WorldEvent_enum.h"

class WorldEvent_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		npcListMask	= Parent::NextFreeMask + 1,
		npcLiveTimeMask	= Parent::NextFreeMask + 2,
		buffEventMask	= Parent::NextFreeMask + 3,
		mapListMask	= Parent::NextFreeMask + 4,
		scriptMask	= Parent::NextFreeMask + 5,
		NextFreeMask	= Parent::NextFreeMask + 6
	};
	const UInt16& GetEventId() const { return m.eventId; }
	const UInt8& GetEventType() const { return m.eventType; }
	const UInt8& GetMapType() const { return m.mapType; }
	const UInt32& GetEndTime() const { return m.endTime; }
	const NpcInfoVec& GetNpcList() const { return m.npcList; }
	const UInt16Vector& GetNpcLineCounts() const { return m.npcLineCounts; }
	const UInt32& GetNpcLiveTime() const { return m.npcLiveTime; }
	const UInt16Vector& GetBuffList() const { return m.buffList; }
	const UInt16& GetBuffPeriod() const { return m.buffPeriod; }
	const Boolean& GetBuffClear() const { return m.buffClear; }
	const Boolean& GetBuffLeave() const { return m.buffLeave; }
	const UInt16Vector& GetMapList() const { return m.mapList; }
	const String& GetScript() const { return m.script; }
	void SetInfo(const UInt16 &eventId, const UInt8 &eventType, const UInt8 &mapType, const UInt32 &endTime);
	void SetEventId(const UInt16 &eventId);
	void SetEventType(const UInt8 &eventType);
	void SetMapType(const UInt8 &mapType);
	void SetEndTime(const UInt32 &endTime);
	void SetNpcList(const NpcInfoVec &npcList, const UInt16Vector &npcLineCounts);
	void SetNpcList(const NpcInfoVec &npcList);
	void SetNpcLineCounts(const UInt16Vector &npcLineCounts);
	void SetNpcLiveTime(const UInt32 &npcLiveTime);
	void SetBuffEvent(const UInt16Vector &buffList, const UInt16 &buffPeriod, const Boolean &buffClear, const Boolean &buffLeave);
	void SetBuffList(const UInt16Vector &buffList);
	void SetBuffPeriod(const UInt16 &buffPeriod);
	void SetBuffClear(const Boolean &buffClear);
	void SetBuffLeave(const Boolean &buffLeave);
	void SetMapList(const UInt16Vector &mapList);
	void SetScript(const String &script);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//info
	UInt16 eventId;
	UInt8 eventType;
	UInt8 mapType;
	UInt32 endTime;
	//npcList
	NpcInfoVec npcList;
	UInt16Vector npcLineCounts;
	//npcLiveTime
	UInt32 npcLiveTime;
	//buffEvent
	UInt16Vector buffList;
	UInt16 buffPeriod;
	Boolean buffClear;
	Boolean buffLeave;
	//mapList
	UInt16Vector mapList;
	//script
	String script;
} m;

public:
	INLINE WorldEvent_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_WORLDEVENT_STUB_H_
