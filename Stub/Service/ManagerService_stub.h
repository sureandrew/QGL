///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MANAGERSERVICE_STUB_H_
#define _RPC_MANAGERSERVICE_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ManagerService_enum.h"

namespace ManagerService_Stub {

RPCResult RegisterManager(LPCPEER pPeer, RPCContext &context);
RPCResult RegisterSlave(LPCPEER pPeer, RPCContext &context);
RPCResult ForwardStatus(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt32 &status, const UInt32 &state);
RPCResult ForwardStatuses(LPCPEER pPeer, RPCContext &context, const ServiceStatusesMap &services_statuses);
RPCResult ForwardCheats(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
RPCResult LoadConfig(LPCPEER pPeer, RPCContext &context, const String &configFile);
RPCResult CheckAllServices(LPCPEER pPeer, RPCContext &context);
RPCResult StartAllServices(LPCPEER pPeer, RPCContext &context);
RPCResult StopAllServices(LPCPEER pPeer, RPCContext &context, const Boolean &bStopClose);
RPCResult RestartAllServices(LPCPEER pPeer, RPCContext &context, const Boolean &bErr);
RPCResult CheckService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId);
RPCResult StartService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId);
RPCResult StopService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId);
RPCResult RestartService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId);
RPCResult ErrorArchive(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId);
RPCResult ShutdownAt(LPCPEER pPeer, RPCContext &context, const UInt32 &downTime);
RPCResult RegisterSimClient(LPCPEER pPeer, RPCContext &context, const String &prog, const String &title, const String &workDir, const String &args);
RPCResult UnregisterSimClient(LPCPEER pPeer, RPCContext &context);
RPCResult StartProcess(LPCPEER pPeer, RPCContext &context, const UInt32 &time, const String &prog, const String &title, const String &workDir, const String &cmd);
// end of generated RPC functions

RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer *pBuf);
RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer *pBuf);

}



#endif //_RPC_MANAGERSERVICE_STUB_H_
