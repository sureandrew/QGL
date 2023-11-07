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

#include "Proxy/Service/ServiceStatus_Proxy.h"
#include "Stub/Service/LogService_Stub.h"

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

	else if (rpcId >= RPC_ServiceStatus_ENUM_ST && rpcId <= RPC_ServiceStatus_ENUM_ED)
	{
		_result = ServiceStatus_Proxy::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_LogService_ENUM_ST && rpcId <= RPC_LogService_ENUM_ED)
	{
		_result = LogService_Stub::Process(rpcId, pPeer, context, pBuf);
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
	else if (rpcId >= RPC_ServiceStatus_ENUM_ST && rpcId <= RPC_ServiceStatus_ENUM_ED)
	{
		_result = ServiceStatus_Proxy::Decode(rpcId, pPeer, pBuf);
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
	case 7738: return _T("RPC_ServiceStatus_UpdateStatus");
	case 7739: return _T("RPC_ServiceStatus_CB_CheckService");
	case 7633: return _T("RPC_ServiceStatus_CB_ShutdownAt");
	case 7554: return _T("RPC_ServiceStatus_CB_StopService");
	case 6689: return _T("RPC_LogService_LogAccount");
	case 6083: return _T("RPC_LogService_LogCharacter");
	case 6729: return _T("RPC_LogService_LogItem");
	case 6942: return _T("RPC_LogService_LogQuest");
	case 6341: return _T("RPC_LogService_LogPlayerCount");
	case 6572: return _T("RPC_LogService_LogGiftBox");
	case 6740: return _T("RPC_LogService_LogChargeShop");
	case 6909: return _T("RPC_LogService_LogExchange");
	case 6248: return _T("RPC_LogService_LogExchangeForItemList");
	case 6804: return _T("RPC_LogService_LogYuanBao");
	case 6698: return _T("RPC_LogService_LogCheat");
	case 6639: return _T("RPC_LogService_LogCharExp");
	case 6235: return _T("RPC_LogService_LogMoney");
	case 6784: return _T("RPC_LogService_LogSMoney");
	case 6213: return _T("RPC_LogService_LogMix");
	case 6816: return _T("RPC_LogService_LogGoodness");
	case 6577: return _T("RPC_LogService_LogHack");
	case 6162: return _T("RPC_LogService_LogSellYuanBao");
	case 6913: return _T("RPC_LogService_LogHonor");
	case 6212: return _T("RPC_LogService_LogPartner");
	case 6752: return _T("RPC_LogService_LogGenerationAcc");
	case 6610: return _T("RPC_LogService_LogManor");
	case 6089: return _T("RPC_LogService_LogGuildDonate");
	}
	static Char buf[12];

	_stprintf(buf, _T("RPCID %d"), rpcId);
	return buf;
}
#endif // CLIENT

#pragma optimize("", on)

