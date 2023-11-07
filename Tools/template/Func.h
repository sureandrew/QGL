///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
@@beginh

#include "Common/RPCFunc.h"
@@include

@@enum

@@namespace

@@decl
// end of generated RPC functions

RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer *pBuf);
RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer *pBuf);

}

@@impl

@@encrypt

@@endh
