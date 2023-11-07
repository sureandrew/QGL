//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/BattleService_proxy.h"

#ifdef QGL
#include "QGL/Global.h"
#endif // QGL

// private function and namespace global variable here
namespace BattleService_Proxy
{
};

RPCResult BattleService_Proxy::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _T("BattleService_Proxy CB_Error: err %d, %s"), err, errMsg.c_str());
	return RPC_RESULT_OK;
}

RPCResult BattleService_Proxy::CB_ResumeSession(LPCPEER pPeer, RPCContext &context, const Boolean &valid)
{
	return RPC_RESULT_OK;
}

RPCResult BattleService_Proxy::CB_GetAvailService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	return RPC_RESULT_OK;
}

RPCResult BattleService_Proxy::CB_CreateBattle(LPCPEER pPeer, RPCContext &context, NetObject * battle)
{
	return RPC_RESULT_OK;
}

RPCResult BattleService_Proxy::CB_EnterBattle(LPCPEER pPeer, RPCContext &context, NetObject * battle, NetGroup * grp, const UInt8 & PlayerTeam,
											  const BYTE & battleState, const UInt32 & npcId)
{
#ifdef QGL
		gGlobal.NotifyEnterBattle(pPeer, context, battle, grp, PlayerTeam, battleState, npcId);
#endif // QGL

	return RPC_RESULT_OK;
}
