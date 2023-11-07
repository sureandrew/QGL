///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
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
@@setdecl
@@clone
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
@@attribute
} m;

public:
@@construct
	{ _fields _m = {}; m = _m; }

};

@@endh
