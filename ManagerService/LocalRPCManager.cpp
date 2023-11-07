//-- Common
#include "Common.h"
//-- Self
#include "Common/LocalRPCManager.h"
//-- Library
#include "Common/Procedure.h"
#include "Reuben/Math/AES.h"

typedef StlMap<UInt16, bool, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, bool> > > RpcNonblockMap;
static RpcNonblockMap*  g_map_nonblock = SafeCreateObject(RpcNonblockMap);
#define RPC_NONBLOCK_MAP (*g_map_nonblock)

#include "Proxy/Service/WorldService_Proxy.h"
#include "Stub/Service/ManagerService_Stub.h"
#include "Stub/Service/ServiceStatus_Stub.h"
#include "RPCEnum/ExecuteCheatData_enum.h"

RPCResult DoNetObjectHelper(ProcedureInfo* pinfo, RPCContext& context)
{
	try
	{
		return (pinfo->psrc->*(pinfo->cbMethod)) (context);		// call object method
	}
	catch (...)
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("exception ocuss, session_id %s, pid %d:%d"),
			context.sessionId.GetString().c_str(), context.pcdId.srvId, context.pcdId.subId);
		return (RPC_RESULT_OK);
	}
}

LocalRPCManager::~LocalRPCManager()
{
	SafeDeleteObject(g_map_nonblock);
}

#ifdef RPC_PROFILING
void LocalRPCManager::ShowProfile(UInt32 dt)
{
	SortProfileMap sortProfileMap;

	// sorted profile by time and count
	for (ProfileMap::iterator it = m_profileMap.begin(); it != m_profileMap.end(); ++it) {
		const ProfileInfo &pf = it->second;
		if (pf.time >= RPC_PROF_CONCERN)
			sortProfileMap.insert(std::make_pair(pf, it->first));
	}

	TRACE_WARNDTL_1(m_logger, _FW("total update time %d"), dt);
	for (SortProfileMap::iterator it = sortProfileMap.begin(); 
		it != sortProfileMap.end(); ++it)
	{
		const ProfileInfo &pf = it->first;
		TRACE_WARNDTL_3(m_logger, _FW("%s, time %d, count %d"), 
			GetRpcName(it->second), pf.time, pf.count);
	}
}

void LocalRPCManager::OnUpdate(const UInt32 dt)
{
	ProfileInfo dummy;

	// clear profile value but remain key
	for (ProfileMap::iterator it = m_profileMap.begin(); it != m_profileMap.end(); ++it)
		it->second = dummy;

	UInt32 curTime = ::GetTickCount();
	Parent::OnUpdate(dt);

	UInt32 used = ::GetTickCount() - curTime;
	if (used > RPC_PROF_REQUIRE)			// show profile only required time reached
		ShowProfile(used);
}
#endif // RPC_PROFILING

RPCResult LocalRPCManager::OnProcess(Reuben::Network::RPCMessage *pMsg)
{
	LPCPEER pPeer = pMsg->pPeer;
	Buffer* pBuf = pMsg->pBuffer;

	DEBUG_ENSURE(pPeer && pBuf);

	if (!pPeer->IsAlive())
		return RPC_RESULT_FAIL;

	if (pBuf == NULL || pBuf->GetData() == NULL || 
		pBuf->GetSize() < REUBEN_NETWORK_BUFFER_HEADER_SIZE)
	{
		TRACE_ERRORDTL_2(m_logger, _FE("invalid buffer, socket_id %d, size %d"),
			pPeer->GetSocketID(), pBuf ? pBuf->GetSize() : 0);
		return RPC_RESULT_FAIL;
	}

	Reuben::Network::RPCHeader header;
	RPCContext context;
	RPCID rpcId;

	pBuf->Read<Reuben::Network::RPCHeader>(header);
	rpcId = header.rpcId;

#ifdef CLIENT
	if (rpcId == RPC_KEY_DATA)
	{
		if (!Reuben::Math::AES::KeyChanged())
		{
			if (Reuben::Network::DefaultMiddle::IsValidCRC(pBuf))
				Reuben::Math::AES::KeyExpansion(pBuf->GetData() + pBuf->GetCursor(), KEY_SIZE);
			else
			{
				TRACE_ERRORDTL_1(m_logger, _FE("key data CRC failed, socket_id %d"), pPeer->GetSocketID());
				return (RPC_RESULT_FAIL);
			}
		}
		return (RPC_RESULT_OK);
	}
	else if (DoDecode(rpcId, pPeer, pBuf) != RPC_RESULT_OK)
	{
		TRACE_ERRORDTL_2(m_logger, _FE("decode failed, SocketID %d, %s"), 
			pPeer->GetSocketID(), GetRpcName(rpcId));
		return (RPC_RESULT_FAIL);
	}
#else
	if (Decode(pPeer, pBuf) != RPC_RESULT_OK)
	{
		TRACE_ERRORDTL_2(m_logger, _FE("decode failed, SocketID %d, %s"), 
			pPeer->GetSocketID(), GetRpcName(rpcId));
		return (RPC_RESULT_FAIL);
	}
#endif // CLIENT

	pBuf->Read<RPCContext>(context);
	
	// try to redirect to new session Id first
	if (context.sessionId.serviceId != INVALID_SERVICE_ID)
		SessionManager::GetInstance().RedirectSessionId(context.sessionId);

#ifdef SIMCLIENT
	if (m_recvHandle)
	{
		if (!(*m_recvHandle) (pMsg->pPeer, context))
		{
			TRACE_ERRORDTL_1(m_logger, _FE("recv handle failed, socket %d"), pPeer->GetSocketID());
			return RPC_RESULT_FAIL;
		}
	}
#endif // SIMCLIENT

	// may be a procedure return call
	ProcedureManager &pcmgr = ProcedureManager::GetInstance();
	ProcedureInfo *pinfo = NULL;
	if (context.pcdId.srvId == m_srvId && context.pcdId.subId != 0)
	{
		pinfo = pcmgr.Get(context.pcdId);
		// find if match trapped RPC ID
		if (pinfo && (context.sessionId.serviceId == 0 || 
			pinfo->origContext.sessionId == context.sessionId))
		{
			// cancel waiting resume
			pcmgr.CancelResume(context.pcdId);

			// error if not match trapRpcId
			pinfo->errRpcId = (pinfo->trapRpcId != rpcId) ? rpcId : 0;
			pinfo->pSubBuf = pBuf;
			switch (pinfo->pcdType)
			{
			case ProcedureInfo::PCD_RPC:
				rpcId = pinfo->srcRpcId;
				pPeer = pinfo->pSrcPeer;
				pBuf = pinfo->pSrcBuf;
				pBuf->ResetCursor();
				break;

			case ProcedureInfo::PCD_METHOD:
				return DoNetObjectHelper(pinfo, context);

			case ProcedureInfo::PCD_FUNC:
				return (*pinfo->cbFunc) (context);		// call function

			default:
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("wrong procedure type %d"), pinfo->pcdType);
				pcmgr.EndReturn(pinfo, context.pcdId, PCD_ERR_REMOVED);
				pinfo = NULL;
			}
		}
		else
		{
			if( pinfo )
			{
				TRACE_ERRORDT_6(GLOBAL_LOGGER, _FE("wrong session id, orig=%s %d:%d new=%s %d:%d, "),
					pinfo->origContext.sessionId.GetString().c_str(), pinfo->origContext.pcdId.srvId,
					pinfo->origContext.pcdId.subId, context.sessionId.GetString().c_str(),
					context.pcdId.srvId, context.pcdId.subId);

				TRACE_ERRORL_6(GLOBAL_LOGGER, _T("rpc_id %d, socket %d, remote addr %s:%d, local addr %s:%d"),
					rpcId, pPeer->GetSocketID(),
					Reuben::Network::IPToString(pPeer->GetAddressRemote().GetIP()).c_str(), Reuben::Network::NetToHost(pPeer->GetAddressRemote().GetPort()),
					Reuben::Network::IPToString(pPeer->GetAddressLocal().GetIP()).c_str(), Reuben::Network::NetToHost(pPeer->GetAddressLocal().GetPort()));
				pcmgr.EndReturn(pinfo, context.pcdId, PCD_ERR_REMOVED);
				pinfo = NULL;
			}
			// reset procedure ID to 0 if not found
			context.pcdId.srvId = 0;
			context.pcdId.subId = 0;
		}
	}

	// if blocking RPC function, try to create procedure when session locked
	// otherwise, nonblocking RPC function or session not locked, go straightly
	if (pinfo == NULL && context.sessionId.serviceId != INVALID_SERVICE_ID &&
		(RPC_NONBLOCK_MAP.empty() || RPC_NONBLOCK_MAP.find(rpcId) == RPC_NONBLOCK_MAP.end()) && 
		pcmgr.IsSessionLocked(context.sessionId))
	{
		pinfo = pcmgr.NewRPC(pPeer, context, rpcId);
		TRACE_ENSURE(pinfo != NULL);
		// assume this is a single step procedure
		pinfo->singleRpc = BOOLEAN_TRUE;
		pinfo->label.Format(_T("%s"), GetRpcName(rpcId));
	
		// copy current buffer to procedure source buffer
		Size nSize = pBuf->GetSize() - pBuf->GetCursor();
		Byte *p = pBuf->GetData() + pBuf->GetCursor(); 
		pinfo->pSrcBuf->Write(p, nSize);
		pcmgr.Lock(pinfo, context.pcdId, &context.sessionId);
		return RPC_RESULT_OK;
	}

	RPCResult _do_result_ = RPC_RESULT_FAIL;
	try
	{
		_do_result_ = DoProcess(rpcId, pPeer, context, pBuf);
	}
	catch(...)
	{
		TRACE_ERRORDTL_6(GLOBAL_LOGGER,
			_FE("do process exception, session_id %s, rpc_id %d, pcd_id %d:%d, buffer size %d, buffer cursor %d"),
			context.sessionId.GetString().c_str(), rpcId, context.pcdId.srvId, context.pcdId.subId, pBuf->GetSize(), pBuf->GetCursor());

		for (Size i = 0; i < pBuf->GetSize(); ++i)
		{
			TRACE_ERROR_1(GLOBAL_LOGGER, _T("%02X "), *(pBuf->GetData() + i));
		}

		//TRACE_ERRORDTL_1(REUBEN_LOGGER, _FE("exception occus at %s"), GLOBAL_NAME);
	}
	return (_do_result_);
}

RPCResult LocalRPCManager::DoProcess(RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer *pBuf)
{
	RPCResult _result = RPC_NOT_FOUND;
#ifdef RPC_PROFILING
	UInt32 curTime = ::GetTickCount();
#endif // RPC_PROFILING

	// switch of different RPC ID
	if (rpcId == ERR_RPCMANAGER)
	{
		// Generic or RPC manager error
		UInt32 errCode;
		String errMsg;
		pBuf->Read<UInt32>(errCode);
		pBuf->Read<String>(errMsg);
		_result = RecvError(pPeer, context, errCode, errMsg);
	}

	else if (rpcId >= RPC_WorldService_ENUM_ST && rpcId <= RPC_WorldService_ENUM_ED)
	{
		_result = WorldService_Proxy::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_ManagerService_ENUM_ST && rpcId <= RPC_ManagerService_ENUM_ED)
	{
		_result = ManagerService_Stub::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_ServiceStatus_ENUM_ST && rpcId <= RPC_ServiceStatus_ENUM_ED)
	{
		_result = ServiceStatus_Stub::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_ExecuteCheatData_ENUM_ST && rpcId <= RPC_ExecuteCheatData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}

	if (_result == RPC_NOT_FOUND)
	{
		String errMsg;
		errMsg.Format(_FE("RPCID not found %d"), rpcId);
		TRACE_ERRORDTL(GLOBAL_LOGGER, errMsg.c_str());
		SendError(pPeer, context, ERR_RPCMANAGER, errMsg);
	}
	else if (_result == RPC_NOT_ACCESS)
	{
		String errMsg;
		errMsg.Format(_FE("cannot access private RPCID %d"), rpcId);
		TRACE_ERRORDTL(GLOBAL_LOGGER, errMsg.c_str());
		SendError(pPeer, context, ERR_RPCMANAGER, errMsg);
	}
#ifdef RPC_PROFILING
	else
	{
		ProfileInfo &pf = m_profileMap[rpcId];
		pf.time += ::GetTickCount() - curTime;
		pf.count++;
	}
#endif // RPC_PROFILING

	return _result;
}

void LocalRPCManager::DeleteAll()
{
	// check if any NetObjects for deletion
	if (m_custIdMap && m_custIdMap->empty() || m_custIdMap == NULL && m_netIdMap.empty())
		return;

	// change the global logger level for logging all undeleted object
	Reuben::Diagnostics::Logger& logger = GLOBAL_LOGGER;
	Reuben::Diagnostics::Logger::LevelEnum logLevel = logger.GetLevel();
	logger.SetLevel(Reuben::Diagnostics::Logger::LEVEL_VERBOSE);

	NetObject *obj;
	NetIDMap::iterator it;
	if (m_custIdMap)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("NetObject remains %d"), m_custIdMap->size());
		while (!m_custIdMap->empty())
		{
			it = m_custIdMap->begin();
			obj = it->second;
			m_custIdMap->erase(it);
			if (obj)
			{
				ProcedureManager::GetInstance().RemoveByObject(obj);
				Reuben::Simulation::DeleteObject(obj);
			}
		}
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("NetObject remains %d"), m_netIdMap.size());
		while (!m_netIdMap.empty())
		{
			it = m_netIdMap.begin();
			obj = it->second;
			m_netIdMap.erase(it);
			if (obj)
			{
				ProcedureManager::GetInstance().RemoveByObject(obj);
				Reuben::Simulation::DeleteObject(obj);
			}
		}
	}

	logger.SetLevel(logLevel);					// restore back log level
}

#pragma optimize("", off)

RPCResult LocalRPCManager::DoDecode(RPCID rpcId, LPCPEER pPeer, Buffer *pBuf)
{
#ifdef CLIENT

	RPCResult _result = RPC_NOT_FOUND;

	// switch of different RPC ID
	if (rpcId == ERR_RPCMANAGER) {
		_result = RPC_RESULT_FAIL;
	}
	else if (rpcId >= RPC_WorldService_ENUM_ST && rpcId <= RPC_WorldService_ENUM_ED)
	{
		_result = WorldService_Proxy::Decode(rpcId, pPeer, pBuf);
	}

	return _result;
#else
	return RPC_RESULT_OK;
#endif // CLIENT
}

#if defined(CLIENT)
PCSTRING LocalRPCManager::GetRpcName(RPCID rpcId)
{
	static Char buf[12];

	_stprintf(buf, _T("RPCID %d"), rpcId);
	return buf;
}
#else
PCSTRING LocalRPCManager::GetRpcName(RPCID rpcId)
{
	switch (rpcId) {
	case 2791: return _T("RPC_WorldService_RegisterProxy");
	case 2994: return _T("RPC_WorldService_UnregisterProxy");
	case 2575: return _T("RPC_WorldService_RegisterMap");
	case 2479: return _T("RPC_WorldService_UnregisterMap");
	case 2864: return _T("RPC_WorldService_RegisterInstance");
	case 2090: return _T("RPC_WorldService_UnregisterInstance");
	case 2420: return _T("RPC_WorldService_ValidateMap");
	case 2551: return _T("RPC_WorldService_QueryInstance");
	case 2279: return _T("RPC_WorldService_CB_QueryInstance");
	case 2116: return _T("RPC_WorldService_CanChangeLine");
	case 2595: return _T("RPC_WorldService_RemoteCreateNpc");
	case 2784: return _T("RPC_WorldService_ReleaseSession");
	case 2948: return _T("RPC_WorldService_CharacterLeave");
	case 2052: return _T("RPC_WorldService_CharJumpMap");
	case 2746: return _T("RPC_WorldService_NpcJumpMap");
	case 2438: return _T("RPC_WorldService_CharUpdateLocation");
	case 2142: return _T("RPC_WorldService_CB_JumpSuccess");
	case 2233: return _T("RPC_WorldService_CB_ValidMap");
	case 2387: return _T("RPC_WorldService_CB_UpdateLocation");
	case 2456: return _T("RPC_WorldService_CB_CanChangeLine");
	case 2307: return _T("RPC_WorldService_GetWorldGameTime");
	case 2313: return _T("RPC_WorldService_CB_GetWorldGameTime");
	case 2457: return _T("RPC_WorldService_NotifyNpcCreate");
	case 2916: return _T("RPC_WorldService_NotifyNpcDead");
	case 2901: return _T("RPC_WorldService_QueryNpcExist");
	case 2576: return _T("RPC_WorldService_ForceLogout");
	case 2174: return _T("RPC_WorldService_CB_NpcNotExist");
	case 2204: return _T("RPC_WorldService_CB_StartEvent");
	case 2589: return _T("RPC_WorldService_CB_StopEvent");
	case 2041: return _T("RPC_WorldService_CB_PeriodEvent");
	case 2167: return _T("RPC_WorldService_CB_TimeZoneChanged");
	case 2042: return _T("RPC_WorldService_StartEvent");
	case 2681: return _T("RPC_WorldService_LoginAccount");
	case 2066: return _T("RPC_WorldService_KickAccount");
	case 2567: return _T("RPC_WorldService_GetMapLines");
	case 2490: return _T("RPC_WorldService_VersionChecking");
	case 2332: return _T("RPC_WorldService_CB_GetMapLines");
	case 2642: return _T("RPC_WorldService_CB_LoginResult");
	case 2302: return _T("RPC_WorldService_CB_WaitAccount");
	case 2430: return _T("RPC_WorldService_CB_LoginAccount");
	case 2148: return _T("RPC_WorldService_CB_VersionCorrect");
	case 2212: return _T("RPC_WorldService_CB_ForceLogout");
	case 2530: return _T("RPC_WorldService_CheckCanBuyLimitedItem");
	case 2918: return _T("RPC_WorldService_ReduceLimitItem");
	case 2383: return _T("RPC_WorldService_CB_CheckCanBuyLimitedItem");
	case 2213: return _T("RPC_WorldService_SendCheatCommand");
	case 2648: return _T("RPC_WorldService_CB_AntiHack");
	case 2164: return _T("RPC_WorldService_AnswerAntiHack");
	case 2514: return _T("RPC_WorldService_WorldCheat");
	case 2139: return _T("RPC_WorldService_GetCharacterRank");
	case 2236: return _T("RPC_WorldService_GetGuildRank");
	case 2728: return _T("RPC_WorldService_StoreApolloLog");
	case 2819: return _T("RPC_WorldService_CB_Error");
	case 2570: return _T("RPC_WorldService_CB_WorldCheat");
	case 2467: return _T("RPC_WorldService_CB_GetCharacterRank");
	case 2372: return _T("RPC_WorldService_CB_RankChangeMsg");
	case 2820: return _T("RPC_WorldService_CB_GetGuildRank");
	case 2545: return _T("RPC_WorldService_ApolloMsg");
	case 2847: return _T("RPC_WorldService_CB_ApolloMsg");
	case 2389: return _T("RPC_WorldService_GetSellYuanBao");
	case 2521: return _T("RPC_WorldService_CB_GetSellYuanBao");
	case 2280: return _T("RPC_WorldService_AddSellYuanBao");
	case 2955: return _T("RPC_WorldService_CB_AddSellYuanBao");
	case 2535: return _T("RPC_WorldService_BuySellYuanBao");
	case 2053: return _T("RPC_WorldService_CB_BuySellYuanBao");
	case 2218: return _T("RPC_WorldService_GetTempBank");
	case 2401: return _T("RPC_WorldService_CB_GetTempBank");
	case 2993: return _T("RPC_WorldService_ClientDisconnect");
	case 2416: return _T("RPC_WorldService_UpdateWantedReward");
	case 2253: return _T("RPC_WorldService_CB_UpdateWantedReward");
	case 2404: return _T("RPC_WorldService_WantedDetail");
	case 2366: return _T("RPC_WorldService_CB_WantedDetail");
	case 2846: return _T("RPC_WorldService_RemoveFromWantedRank");
	case 2722: return _T("RPC_WorldService_DeleteGenerationTrans");
	case 2569: return _T("RPC_WorldService_RecvGenerationAccmulationTrans");
	case 2063: return _T("RPC_WorldService_GenerationOnlineNotifyTrans");
	case 2076: return _T("RPC_WorldService_GenerationOfflineNotifyTrans");
	case 2787: return _T("RPC_WorldService_HasGlobalNpcFlag");
	case 2452: return _T("RPC_WorldService_CB_HasGlobalNpcFlag");
	case 2758: return _T("RPC_WorldService_GetGlobalNpcFlag");
	case 2699: return _T("RPC_WorldService_CB_GetGlobalNpcFlag");
	case 2975: return _T("RPC_WorldService_SetGlobalNpcFlag");
	case 2054: return _T("RPC_WorldService_CB_SetGlobalNpcFlag");
	case 2220: return _T("RPC_WorldService_RemoveGlobalNpcFlag");
	case 2807: return _T("RPC_WorldService_CB_RemoveGlobalNpcFlag");
	case 7027: return _T("RPC_ManagerService_RegisterManager");
	case 7403: return _T("RPC_ManagerService_RegisterSlave");
	case 7455: return _T("RPC_ManagerService_ForwardStatus");
	case 7290: return _T("RPC_ManagerService_ForwardStatuses");
	case 7257: return _T("RPC_ManagerService_ForwardCheats");
	case 7053: return _T("RPC_ManagerService_LoadConfig");
	case 7120: return _T("RPC_ManagerService_CheckAllServices");
	case 7084: return _T("RPC_ManagerService_StartAllServices");
	case 7079: return _T("RPC_ManagerService_StopAllServices");
	case 7230: return _T("RPC_ManagerService_RestartAllServices");
	case 7138: return _T("RPC_ManagerService_CheckService");
	case 7158: return _T("RPC_ManagerService_StartService");
	case 7145: return _T("RPC_ManagerService_StopService");
	case 7148: return _T("RPC_ManagerService_RestartService");
	case 7306: return _T("RPC_ManagerService_ErrorArchive");
	case 7310: return _T("RPC_ManagerService_ShutdownAt");
	case 7365: return _T("RPC_ManagerService_RegisterSimClient");
	case 7078: return _T("RPC_ManagerService_UnregisterSimClient");
	case 7150: return _T("RPC_ManagerService_StartProcess");
	case 7738: return _T("RPC_ServiceStatus_UpdateStatus");
	case 7739: return _T("RPC_ServiceStatus_CB_CheckService");
	case 7633: return _T("RPC_ServiceStatus_CB_ShutdownAt");
	case 7554: return _T("RPC_ServiceStatus_CB_StopService");
	}
	static Char buf[12];

	_stprintf(buf, _T("RPCID %d"), rpcId);
	return buf;
}
#endif // CLIENT

#pragma optimize("", on)

