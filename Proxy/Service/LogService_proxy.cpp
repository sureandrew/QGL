//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/LogService_proxy.h"

#pragma optimize("", off)
RPCResult LogService_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}

RPCResult LogService_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
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

