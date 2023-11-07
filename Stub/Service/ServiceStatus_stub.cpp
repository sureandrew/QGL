//-- Common
#include "Common.h"
//-- Self
#include "Stub/Service/ServiceStatus_stub.h"

#pragma optimize("", off)
RPCResult ServiceStatus_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
#ifndef CLIENT
	case RPC_ServiceStatus_UpdateStatus:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			UInt32 status;
			pBuf->Read<UInt32>(status);
			UInt32 state;
			pBuf->Read<UInt32>(state);
			_result = UpdateStatus(pPeer, context, serviceId, status, state);
		}
		break;
#endif // CLIENT
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}
#pragma optimize("", on)

RPCResult ServiceStatus_Stub::CB_CheckService(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ServiceStatus_CB_CheckService);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult ServiceStatus_Stub::CB_ShutdownAt(LPCPEER pPeer, RPCContext &context, const UInt32 &downTime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ServiceStatus_CB_ShutdownAt);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(downTime);
	return RPCSend(pPeer, pBuf);
}

RPCResult ServiceStatus_Stub::CB_StopService(LPCPEER pPeer, RPCContext &context, const UInt32 &stopTime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ServiceStatus_CB_StopService);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(stopTime);
	return RPCSend(pPeer, pBuf);
}

