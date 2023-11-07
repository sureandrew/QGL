///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MAP_STUB_H_
#define _RPC_MAP_STUB_H_

#include "Common/RPCFunc.h"
struct JumpInfo
{
	UInt16 jpPosX;
	UInt16 jpPosY;
	UInt16 dstMapId;
	UInt16 dstPosX;
	UInt16 dstPosY;
};
typedef StlVector<JumpInfo, ManagedAllocator<JumpInfo> > JumpInfoVector;
#include "RPCEnum/Map_enum.h"

class Map_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult GetCharInRange(LPCPEER pPeer, RPCContext &context, const UInt16 &centerX, const UInt16 &centerY, const UInt16 &range);
	virtual RPCResult GetJumpPoints(LPCPEER pPeer, RPCContext &context);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		instanceMask	= Parent::NextFreeMask + 1,
		NextFreeMask	= Parent::NextFreeMask + 2
	};
	virtual RPCResult GetCharInRange(LPCPEER pPeer, RPCContext &context, const UInt16 &centerX, const UInt16 &centerY, const UInt16 &range) = 0;
	virtual RPCResult GetJumpPoints(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_GetCharInRange(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
	virtual RPCResult CB_GetJumpPoints(LPCPEER pPeer, RPCContext &context, const JumpInfoVector &jumpInfoList);
	virtual RPCResult CB_ObjectChanged(LPCPEER pPeer, RPCContext &context, NetGroup* grp, const RPCNetIDVector &delIds, const RPCNetIDVector &curIds);
	const UInt16& GetMap_id() const { return m.map_id; }
	const UInt8& GetLine_id() const { return m.line_id; }
	const UInt16& GetInst_id() const { return m.inst_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	void SetInfo(const UInt16 &map_id, const UInt8 &line_id);
	void SetMap_id(const UInt16 &map_id);
	void SetLine_id(const UInt8 &line_id);
	void SetInstance(const UInt16 &inst_id, const UInt32 &owner_id);
	void SetInst_id(const UInt16 &inst_id);
	void SetOwner_id(const UInt32 &owner_id);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//info
	UInt16 map_id;
	UInt8 line_id;
	//instance
	UInt16 inst_id;
	UInt32 owner_id;
} m;

public:
	INLINE Map_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_MAP_STUB_H_
