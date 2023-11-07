//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/ManagerService_proxy.h"

#pragma optimize("", off)
RPCResult ManagerService_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
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

RPCResult ManagerService_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
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

