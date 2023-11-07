//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/ServiceStatus_proxy.h"

#pragma optimize("", off)
RPCResult ServiceStatus_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
#ifndef CLIENT
	case RPC_ServiceStatus_CB_CheckService:
		{
			_result = CB_CheckService(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ServiceStatus_CB_ShutdownAt:
		{
			UInt32 downTime;
			pBuf->Read<UInt32>(downTime);
			_result = CB_ShutdownAt(pPeer, context, downTime);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_ServiceStatus_CB_StopService:
		{
			UInt32 stopTime;
			pBuf->Read<UInt32>(stopTime);
			_result = CB_StopService(pPeer, context, stopTime);
		}
		break;
#endif // CLIENT
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}

RPCResult ServiceStatus_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	}
	return _result;
}
#pragma optimize("", on)

