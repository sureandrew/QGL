///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_WORLDEVENT_PROXY_H_
#define _RPC_WORLDEVENT_PROXY_H_

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

class WorldEvent_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo(const UInt16 &eventId, const UInt8 &eventType, const UInt8 &mapType, const UInt32 &endTime);
	virtual void OnUpdateNpcList(const NpcInfoVec &npcList, const UInt16Vector &npcLineCounts);
	virtual void OnUpdateNpcLiveTime(const UInt32 &npcLiveTime);
	virtual void OnUpdateBuffEvent(const UInt16Vector &buffList, const UInt16 &buffPeriod, const Boolean &buffClear, const Boolean &buffLeave);
	virtual void OnUpdateMapList(const UInt16Vector &mapList);
	virtual void OnUpdateScript(const String &script);
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
	virtual void OnUpdateInfo(const UInt16 &eventId, const UInt8 &eventType, const UInt8 &mapType, const UInt32 &endTime) {};
	virtual void OnUpdateNpcList(const NpcInfoVec &npcList, const UInt16Vector &npcLineCounts) {};
	virtual void OnUpdateNpcLiveTime(const UInt32 &npcLiveTime) {};
	virtual void OnUpdateBuffEvent(const UInt16Vector &buffList, const UInt16 &buffPeriod, const Boolean &buffClear, const Boolean &buffLeave) {};
	virtual void OnUpdateMapList(const UInt16Vector &mapList) {};
	virtual void OnUpdateScript(const String &script) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

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
	INLINE WorldEvent_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult WorldEvent_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldEvent_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1237604466>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(909051045)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1237604466)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_WORLDEVENT_PROXY_H_
