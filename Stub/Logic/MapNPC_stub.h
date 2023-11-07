///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MAPNPC_STUB_H_
#define _RPC_MAPNPC_STUB_H_

#include "Common/RPCFunc.h"
#include "MapActor.h"
#include "RPCEnum/MapNPC_enum.h"

class MapNPC_Stub : public CMapActor
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef CMapActor Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		visibleMask	= Parent::NextFreeMask + 1,
		NextFreeMask	= Parent::NextFreeMask + 2
	};
	const UInt32& GetNpc_uid() const { return m.npc_uid; }
	const UInt32& GetNpc_id() const { return m.npc_id; }
	const bool& GetVisible() const { return m.visible; }
	void SetId(const UInt32 &npc_uid, const UInt32 &npc_id);
	void SetNpc_uid(const UInt32 &npc_uid);
	void SetNpc_id(const UInt32 &npc_id);
	void SetVisible(const bool &visible);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 npc_uid;
	UInt32 npc_id;
	//visible
	bool visible;
} m;

public:
	INLINE MapNPC_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_MAPNPC_STUB_H_
