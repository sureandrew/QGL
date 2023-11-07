///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MANAGERSERVICE_PROXY_H_
#define _RPC_MANAGERSERVICE_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ManagerService_enum.h"

namespace ManagerService_Proxy {

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

INLINE RPCResult ManagerService_Proxy::RegisterManager(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_RegisterManager);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::RegisterSlave(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_RegisterSlave);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::ForwardStatus(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt32 &status, const UInt32 &state)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_ForwardStatus);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	pBuf->Write<UInt32>(status);
	pBuf->Write<UInt32>(state);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::ForwardStatuses(LPCPEER pPeer, RPCContext &context, const ServiceStatusesMap &services_statuses)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_ForwardStatuses);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceStatusesMap>(services_statuses);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::ForwardCheats(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_ForwardCheats);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(grp);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::LoadConfig(LPCPEER pPeer, RPCContext &context, const String &configFile)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_LoadConfig);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(configFile);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::CheckAllServices(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_CheckAllServices);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::StartAllServices(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_StartAllServices);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::StopAllServices(LPCPEER pPeer, RPCContext &context, const Boolean &bStopClose)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_StopAllServices);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(bStopClose);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::RestartAllServices(LPCPEER pPeer, RPCContext &context, const Boolean &bErr)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_RestartAllServices);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(bErr);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::CheckService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_CheckService);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::StartService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_StartService);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::StopService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_StopService);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::RestartService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_RestartService);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::ErrorArchive(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_ErrorArchive);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::ShutdownAt(LPCPEER pPeer, RPCContext &context, const UInt32 &downTime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_ShutdownAt);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(downTime);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::RegisterSimClient(LPCPEER pPeer, RPCContext &context, const String &prog, const String &title, const String &workDir, const String &args)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_RegisterSimClient);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(prog);
	pBuf->Write<String>(title);
	pBuf->Write<String>(workDir);
	pBuf->Write<String>(args);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::UnregisterSimClient(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_UnregisterSimClient);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult ManagerService_Proxy::StartProcess(LPCPEER pPeer, RPCContext &context, const UInt32 &time, const String &prog, const String &title, const String &workDir, const String &cmd)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ManagerService_StartProcess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(time);
	pBuf->Write<String>(prog);
	pBuf->Write<String>(title);
	pBuf->Write<String>(workDir);
	pBuf->Write<String>(cmd);
	return RPCSend(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_MANAGERSERVICE_PROXY_H_
