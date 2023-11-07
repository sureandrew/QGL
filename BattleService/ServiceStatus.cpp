//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/ServiceStatus.h"
//-- Library
#include "BattleService/Configure.h"
#include "BattleService/Global.h"
#include "BattleService/BattleApplication.h"

namespace ServiceStatus_Proxy
{
};

RPCResult ServiceStatus_Proxy::CB_CheckService(LPCPEER pPeer, RPCContext &context)
{
	ServiceStatus_Proxy::UpdateStatus(pPeer, context, gConfigure.serviceId, SS_STARTED);
	gGlobal.pManagerPeer = pPeer;

	return RPC_RESULT_OK;
}

RPCResult ServiceStatus_Proxy::CB_StopService(LPCPEER pPeer, RPCContext &context)
{
	//ServiceStatus_Proxy::UpdateStatus(pPeer, context, gConfigure.serviceId, SS_STOPPED);
	gGlobal.m_bExit = BOOLEAN_TRUE;

	return RPC_RESULT_OK;
}
