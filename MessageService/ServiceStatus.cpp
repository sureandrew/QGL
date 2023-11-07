//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/ServiceStatus.h"
//-- Library
#include "MessageService/Configure.h"
#include "MessageService/Global.h"
#include "MessageService/MessageApplication.h"

namespace ServiceStatus_Proxy
{
};

RPCResult ServiceStatus_Proxy::CB_CheckService(LPCPEER pPeer, RPCContext &context)
{
	ServiceStatus_Proxy::UpdateStatus(pPeer, context, gConfigure.serviceId, SS_RUNNING, SRS_STARTED);
	SafeRelease(gGlobal.pManagerPeer);
	gGlobal.pManagerPeer = pPeer->AddRef();

	return RPC_RESULT_OK;
}

RPCResult ServiceStatus_Proxy::CB_ShutdownAt(LPCPEER pPeer, RPCContext &context, const UInt32 &downTime)
{
	gGlobal.ShutdownAt(downTime);
	return RPC_RESULT_OK;
}

RPCResult ServiceStatus_Proxy::CB_StopService(LPCPEER pPeer, RPCContext &context, const UInt32 &stopTime)
{
	//ServiceStatus_Proxy::UpdateStatus(pPeer, context, gConfigure.serviceId, SS_STOPPED);
	gGlobal.m_bExit = BOOLEAN_TRUE;

	return RPC_RESULT_OK;
}
