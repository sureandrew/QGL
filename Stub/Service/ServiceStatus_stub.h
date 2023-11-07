///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SERVICESTATUS_STUB_H_
#define _RPC_SERVICESTATUS_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ServiceStatus_enum.h"

namespace ServiceStatus_Stub {

RPCResult UpdateStatus(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt32 &status, const UInt32 &state);
RPCResult CB_CheckService(LPCPEER pPeer, RPCContext &context);
RPCResult CB_ShutdownAt(LPCPEER pPeer, RPCContext &context, const UInt32 &downTime);
RPCResult CB_StopService(LPCPEER pPeer, RPCContext &context, const UInt32 &stopTime);
// end of generated RPC functions

RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer *pBuf);
RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer *pBuf);

}



#endif //_RPC_SERVICESTATUS_STUB_H_
