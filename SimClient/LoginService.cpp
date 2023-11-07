#include "Common.h"
#include "Proxy/Service/LoginService.h"
#include "Proxy/Logic/Account.h"

// private function and namespace global variable here
namespace LoginService_Proxy
{
};

RPCResult LoginService_Proxy::CB_Error(const Peer &peer, RPCContext &context, const UInt32 &err, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _T("LoginService Error: errcode %d, %s"), err, errMsg.c_str());
	return RPC_RESULT_OK;
}

RPCResult LoginService_Proxy::CB_GetAccount(const Peer &peer, RPCContext &context, NetObject * account)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _T("CB_GetAccount: "));
	return RPC_RESULT_OK;
}
