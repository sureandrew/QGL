///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
@@beginh

#include "Common/RPCFunc.h"
@@include

@@enum

@@class
{
/*** virtual method for fast copy and paste in real class header ***
public:
@@vmethod
***/

protected:
@@parent

public:
@@mask
@@decl
@@getter
@@updecl
@@setdecl
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
@@attribute
} m;

public:
@@construct
			{ _fields _m = {}; m = _m; }

};

@@sendreq
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
@@reqattr
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

@@sendreq2
}

@@impl

@@encrypt

@@endh
