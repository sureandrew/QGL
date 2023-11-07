///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MAPNPC_PROXY_H_
#define _RPC_MAPNPC_PROXY_H_

#include "Common/RPCFunc.h"
#include "MapActor.h"
#include "RPCEnum/MapNPC_enum.h"

class MapNPC_Proxy : public CMapActor
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &npc_uid, const UInt32 &npc_id);
	virtual void OnUpdateVisible(const bool &visible);
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
	virtual void OnUpdateId(const UInt32 &npc_uid, const UInt32 &npc_id) {};
	virtual void OnUpdateVisible(const bool &visible) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 npc_uid;
	UInt32 npc_id;
	//visible
	bool visible;
} m;

public:
	INLINE MapNPC_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult MapNPC_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapNPC_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<714159708>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(199364153)
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(714159708)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_MAPNPC_PROXY_H_
