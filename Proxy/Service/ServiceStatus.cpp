//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/ServiceStatus.h"
//-- Library
#include "ObjectService/Configure.h"
#include "ObjectService/Global.h"
#include "ObjectService/ObjectApplication.h"

namespace ServiceStatus_Proxy
{
};

RPCResult ServiceStatus_Proxy::CB_CheckService(const Peer &peer, RPCContext &context)
{
	ServiceStatus_Proxy::UpdateStatus(peer, context, gConfigure.serviceId, SS_STARTED);

	return RPC_RESULT_OK;
}

RPCResult ServiceStatus_Proxy::CB_StopService(const Peer &peer, RPCContext &context)
{
	ServiceStatus_Proxy::UpdateStatus(peer, context, gConfigure.serviceId, SS_STOPPED);
	gGlobal.m_bExit = BOOLEAN_TRUE;

	return RPC_RESULT_OK;
}
