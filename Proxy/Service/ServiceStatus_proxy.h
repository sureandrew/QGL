///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SERVICESTATUS_PROXY_H_
#define _RPC_SERVICESTATUS_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ServiceStatus_enum.h"

namespace ServiceStatus_Proxy {

RPCResult UpdateStatus(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt32 &status, const UInt32 &state);
RPCResult CB_CheckService(LPCPEER pPeer, RPCContext &context);
RPCResult CB_ShutdownAt(LPCPEER pPeer, RPCContext &context, const UInt32 &downTime);
RPCResult CB_StopService(LPCPEER pPeer, RPCContext &context, const UInt32 &stopTime);
// end of generated RPC functions

RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer *pBuf);
RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer *pBuf);

}

INLINE RPCResult ServiceStatus_Proxy::UpdateStatus(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt32 &status, const UInt32 &state)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ServiceStatus_UpdateStatus);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	pBuf->Write<UInt32>(status);
	pBuf->Write<UInt32>(state);
	return RPCSend(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_SERVICESTATUS_PROXY_H_
