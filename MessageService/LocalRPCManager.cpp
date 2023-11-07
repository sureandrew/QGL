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

#include "Proxy/Service/ObjectService_Proxy.h"
#include "Proxy/Service/ServiceStatus_Proxy.h"
#include "Proxy/Data/MessengerData_Proxy.h"
#include "Proxy/Data/MsgCharacterData_Proxy.h"
#include "Proxy/Data/GuildData_Proxy.h"
#include "Proxy/Data/GuildMemberData_Proxy.h"
#include "Proxy/Service/LogService_Proxy.h"
#include "Proxy/Logic/Item_Proxy.h"
#include "Proxy/Logic/Partner_Proxy.h"
#include "Proxy/Logic/Pet_Proxy.h"
#include "Proxy/Service/MapService_Proxy.h"
#include "Stub/Service/MessageService_Stub.h"
#include "RPCEnum/Channel_enum.h"
#include "RPCEnum/Messenger_enum.h"
#include "RPCEnum/MessageCharacter_enum.h"
#include "RPCEnum/Guild_enum.h"
#include "RPCEnum/GuildMember_enum.h"
#include "RPCEnum/GuildApplicant_enum.h"

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

	else if (rpcId >= RPC_ObjectService_ENUM_ST && rpcId <= RPC_ObjectService_ENUM_ED)
	{
		_result = ObjectService_Proxy::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_ServiceStatus_ENUM_ST && rpcId <= RPC_ServiceStatus_ENUM_ED)
	{
		_result = ServiceStatus_Proxy::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_MessengerData_ENUM_ST && rpcId <= RPC_MessengerData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_MsgCharacterData_ENUM_ST && rpcId <= RPC_MsgCharacterData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_GuildData_ENUM_ST && rpcId <= RPC_GuildData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_GuildMemberData_ENUM_ST && rpcId <= RPC_GuildMemberData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_LogService_ENUM_ST && rpcId <= RPC_LogService_ENUM_ED)
	{
		_result = LogService_Proxy::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Item_ENUM_ST && rpcId <= RPC_Item_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Partner_ENUM_ST && rpcId <= RPC_Partner_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Pet_ENUM_ST && rpcId <= RPC_Pet_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_MapService_ENUM_ST && rpcId <= RPC_MapService_ENUM_ED)
	{
		_result = MapService_Proxy::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_MessageService_ENUM_ST && rpcId <= RPC_MessageService_ENUM_ED)
	{
		_result = MessageService_Stub::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Channel_ENUM_ST && rpcId <= RPC_Channel_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Messenger_ENUM_ST && rpcId <= RPC_Messenger_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_MessageCharacter_ENUM_ST && rpcId <= RPC_MessageCharacter_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Guild_ENUM_ST && rpcId <= RPC_Guild_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_GuildMember_ENUM_ST && rpcId <= RPC_GuildMember_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_GuildApplicant_ENUM_ST && rpcId <= RPC_GuildApplicant_ENUM_ED)
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
	else if (rpcId >= RPC_ObjectService_ENUM_ST && rpcId <= RPC_ObjectService_ENUM_ED)
	{
		_result = ObjectService_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_ServiceStatus_ENUM_ST && rpcId <= RPC_ServiceStatus_ENUM_ED)
	{
		_result = ServiceStatus_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_MessengerData_ENUM_ST && rpcId <= RPC_MessengerData_ENUM_ED)
	{
		_result = MessengerData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_MsgCharacterData_ENUM_ST && rpcId <= RPC_MsgCharacterData_ENUM_ED)
	{
		_result = MsgCharacterData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_GuildData_ENUM_ST && rpcId <= RPC_GuildData_ENUM_ED)
	{
		_result = GuildData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_GuildMemberData_ENUM_ST && rpcId <= RPC_GuildMemberData_ENUM_ED)
	{
		_result = GuildMemberData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_LogService_ENUM_ST && rpcId <= RPC_LogService_ENUM_ED)
	{
		_result = LogService_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_Item_ENUM_ST && rpcId <= RPC_Item_ENUM_ED)
	{
		_result = Item_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_Partner_ENUM_ST && rpcId <= RPC_Partner_ENUM_ED)
	{
		_result = Partner_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_Pet_ENUM_ST && rpcId <= RPC_Pet_ENUM_ED)
	{
		_result = Pet_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_MapService_ENUM_ST && rpcId <= RPC_MapService_ENUM_ED)
	{
		_result = MapService_Proxy::Decode(rpcId, pPeer, pBuf);
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
	case 1578: return _T("RPC_ObjectService_CB_Error");
	case 1626: return _T("RPC_ObjectService_CreateAccount");
	case 1878: return _T("RPC_ObjectService_DeleteAccount");
	case 1442: return _T("RPC_ObjectService_SetYuanBaoLock");
	case 1403: return _T("RPC_ObjectService_IsAssetProtected");
	case 1811: return _T("RPC_ObjectService_StoreAssetPassword");
	case 1960: return _T("RPC_ObjectService_SetAssetPassResetDate");
	case 1405: return _T("RPC_ObjectService_GetAssetPassword");
	case 1035: return _T("RPC_ObjectService_GetAccountByName");
	case 1735: return _T("RPC_ObjectService_GetYaunBaoByAcctID");
	case 1294: return _T("RPC_ObjectService_UpdateAccount");
	case 1210: return _T("RPC_ObjectService_UpdateAccountAddictInfo");
	case 1014: return _T("RPC_ObjectService_ReleaseAccount");
	case 1164: return _T("RPC_ObjectService_UpdateYuanBao");
	case 1914: return _T("RPC_ObjectService_UpdateYuanBaoTicket");
	case 1116: return _T("RPC_ObjectService_CB_GetAccountData");
	case 1477: return _T("RPC_ObjectService_CB_UpdateAccountSuccess");
	case 1517: return _T("RPC_ObjectService_CB_IsAssetProtected");
	case 1200: return _T("RPC_ObjectService_CB_StoreAssetPassword");
	case 1156: return _T("RPC_ObjectService_CB_ResetAssetPassDate");
	case 1874: return _T("RPC_ObjectService_CB_GetAssetPassword");
	case 1295: return _T("RPC_ObjectService_CB_GetYuanBaoByAcctID");
	case 1364: return _T("RPC_ObjectService_CB_UpdateYuanBao");
	case 1998: return _T("RPC_ObjectService_CB_UpdateYuanBaoTicket");
	case 1515: return _T("RPC_ObjectService_CreateCharacter");
	case 1659: return _T("RPC_ObjectService_DeleteCharacter");
	case 1366: return _T("RPC_ObjectService_KeepCharacter");
	case 1003: return _T("RPC_ObjectService_ReleaseCharacter");
	case 1226: return _T("RPC_ObjectService_RestoreCharacter");
	case 1688: return _T("RPC_ObjectService_SetCharacterExpiryDate");
	case 1328: return _T("RPC_ObjectService_SetBankLock");
	case 1244: return _T("RPC_ObjectService_GetCharacters");
	case 1339: return _T("RPC_ObjectService_HasCharacter");
	case 1345: return _T("RPC_ObjectService_UpdateCharacter");
	case 1310: return _T("RPC_ObjectService_CB_GetCharacterGroup");
	case 1441: return _T("RPC_ObjectService_CB_UpdateCharacter");
	case 1321: return _T("RPC_ObjectService_CB_CreateCharacterSuccess");
	case 1261: return _T("RPC_ObjectService_CB_DeleteCharacterSuccess");
	case 1595: return _T("RPC_ObjectService_CB_HasCharacter");
	case 1524: return _T("RPC_ObjectService_CB_QueueNetGroup");
	case 1300: return _T("RPC_ObjectService_CB_RestoreCharacterSuccess");
	case 1774: return _T("RPC_ObjectService_CB_SetCharExpiryDateSuccess");
	case 1866: return _T("RPC_ObjectService_UpdateCharGenAcc");
	case 1531: return _T("RPC_ObjectService_CB_UpdateCharGenAcc");
	case 1797: return _T("RPC_ObjectService_CreateItemList");
	case 1245: return _T("RPC_ObjectService_SetItemLock");
	case 1862: return _T("RPC_ObjectService_GetItemsByOwner");
	case 1897: return _T("RPC_ObjectService_UpdateItemList");
	case 1569: return _T("RPC_ObjectService_CB_CreateItemDone");
	case 1620: return _T("RPC_ObjectService_CB_GetItemData");
	case 1709: return _T("RPC_ObjectService_CB_GetItemGroup");
	case 1082: return _T("RPC_ObjectService_CB_UpdateItemList");
	case 1638: return _T("RPC_ObjectService_CB_SetAssetLock");
	case 1062: return _T("RPC_ObjectService_CreatePartner");
	case 1147: return _T("RPC_ObjectService_SetPartnerLock");
	case 1599: return _T("RPC_ObjectService_GetPartnersByCharID");
	case 1719: return _T("RPC_ObjectService_UpdatePartnerList");
	case 1143: return _T("RPC_ObjectService_CB_CreatePartnerSuccess");
	case 1252: return _T("RPC_ObjectService_CB_GetPartnerGroup");
	case 1499: return _T("RPC_ObjectService_CB_UpdatePartnerList");
	case 1767: return _T("RPC_ObjectService_CreateSkillList");
	case 1222: return _T("RPC_ObjectService_UpdateSkillList");
	case 1206: return _T("RPC_ObjectService_DeleteSkillList");
	case 1091: return _T("RPC_ObjectService_GetSkillList");
	case 1264: return _T("RPC_ObjectService_CB_GetSkillList");
	case 1957: return _T("RPC_ObjectService_CB_CreateSkillDone");
	case 1518: return _T("RPC_ObjectService_CB_DeleteSKillDone");
	case 1139: return _T("RPC_ObjectService_CB_UpdateSkillList");
	case 1368: return _T("RPC_ObjectService_GetQuestList");
	case 1257: return _T("RPC_ObjectService_UpdateQuestLogs");
	case 1397: return _T("RPC_ObjectService_CB_GetQuestList");
	case 1242: return _T("RPC_ObjectService_CB_UpdateQuestLogs");
	case 1627: return _T("RPC_ObjectService_CB_QueueQuests");
	case 1886: return _T("RPC_ObjectService_UpdateQuestTypes");
	case 1185: return _T("RPC_ObjectService_CB_UpdateQuestTypes");
	case 1107: return _T("RPC_ObjectService_UpdateQuestFlags");
	case 1512: return _T("RPC_ObjectService_CB_UpdateQuestFlags");
	case 1042: return _T("RPC_ObjectService_CreateMessenger");
	case 1439: return _T("RPC_ObjectService_DeleteMessenger");
	case 1287: return _T("RPC_ObjectService_DeleteMessengerList");
	case 1016: return _T("RPC_ObjectService_UpdateMessenger");
	case 1869: return _T("RPC_ObjectService_GetMessengerLst");
	case 1166: return _T("RPC_ObjectService_CB_DeleteMessenger");
	case 1948: return _T("RPC_ObjectService_CB_DeleteMessengerList");
	case 1530: return _T("RPC_ObjectService_CB_UpdateMessenger");
	case 1740: return _T("RPC_ObjectService_CB_CreateMessenger");
	case 1327: return _T("RPC_ObjectService_CB_GetMessengerLst");
	case 1008: return _T("RPC_ObjectService_GetCharacterRank");
	case 1611: return _T("RPC_ObjectService_GetGuildRank");
	case 1941: return _T("RPC_ObjectService_CB_GetCharacterRank");
	case 1232: return _T("RPC_ObjectService_CB_GetGuildRank");
	case 1647: return _T("RPC_ObjectService_GetChargeShop");
	case 1820: return _T("RPC_ObjectService_GetCacheChargeShop");
	case 1130: return _T("RPC_ObjectService_UpdateChargeShop");
	case 1018: return _T("RPC_ObjectService_CB_GetChargeShop");
	case 1833: return _T("RPC_ObjectService_CB_UpdateChargeShopSuccess");
	case 1204: return _T("RPC_ObjectService_CreatePet");
	case 1480: return _T("RPC_ObjectService_SetPetLock");
	case 1378: return _T("RPC_ObjectService_GetPetList");
	case 1992: return _T("RPC_ObjectService_UpdatePetList");
	case 1791: return _T("RPC_ObjectService_CB_CreatePet");
	case 1877: return _T("RPC_ObjectService_CB_PetList");
	case 1152: return _T("RPC_ObjectService_CB_UpdatePetList");
	case 1055: return _T("RPC_ObjectService_CreateTitle");
	case 1192: return _T("RPC_ObjectService_DeleteTitle");
	case 1175: return _T("RPC_ObjectService_GetTitleList");
	case 1898: return _T("RPC_ObjectService_UpdateTitle");
	case 1457: return _T("RPC_ObjectService_CB_TitleList");
	case 1591: return _T("RPC_ObjectService_CB_DeleteTitle");
	case 1844: return _T("RPC_ObjectService_CB_CreateTitle");
	case 1995: return _T("RPC_ObjectService_CB_UpdateTitle");
	case 1700: return _T("RPC_ObjectService_GetMessageCharacters");
	case 1855: return _T("RPC_ObjectService_CB_GetMsgCharacterGroup");
	case 1704: return _T("RPC_ObjectService_KeepMsgCharacter");
	case 1946: return _T("RPC_ObjectService_ReleaseMsgCharacter");
	case 1742: return _T("RPC_ObjectService_UpdateMsgCharacter");
	case 1993: return _T("RPC_ObjectService_CB_UpdateMsgCharacter");
	case 1736: return _T("RPC_ObjectService_CreateGuild");
	case 1394: return _T("RPC_ObjectService_GetGuildByID");
	case 1921: return _T("RPC_ObjectService_GetGuildByPvPGroup");
	case 1109: return _T("RPC_ObjectService_GetGuildByPvPBid");
	case 1508: return _T("RPC_ObjectService_UpdateGuildList");
	case 1406: return _T("RPC_ObjectService_DeleteGuildList");
	case 1158: return _T("RPC_ObjectService_ReleaseGuildAndMem");
	case 1739: return _T("RPC_ObjectService_ResetGuildBid");
	case 1269: return _T("RPC_ObjectService_ResetGuildGroup");
	case 1501: return _T("RPC_ObjectService_CB_CreateGuild");
	case 1096: return _T("RPC_ObjectService_CB_GetGuilds");
	case 1724: return _T("RPC_ObjectService_CB_UpdateGuildList");
	case 1349: return _T("RPC_ObjectService_CB_DeleteGuildList");
	case 1006: return _T("RPC_ObjectService_GetGuildMember");
	case 1799: return _T("RPC_ObjectService_CreateGuildMember");
	case 1825: return _T("RPC_ObjectService_DeleteGuildMemberList");
	case 1917: return _T("RPC_ObjectService_UpdateGuildMemberList");
	case 1504: return _T("RPC_ObjectService_CB_UpdateGuildMemberList");
	case 1052: return _T("RPC_ObjectService_CB_CreateGuildMember");
	case 1889: return _T("RPC_ObjectService_CB_DeleteGuildMemberList");
	case 1497: return _T("RPC_ObjectService_CB_GetGuildMembers");
	case 1522: return _T("RPC_ObjectService_GetGiftBoxByAcctID");
	case 1281: return _T("RPC_ObjectService_UpdateGiftBoxByAcctID");
	case 1533: return _T("RPC_ObjectService_DeleteGiftBox");
	case 1687: return _T("RPC_ObjectService_CB_GetGiftBoxByAcctID");
	case 1054: return _T("RPC_ObjectService_CB_DeleteGiftBox");
	case 1113: return _T("RPC_ObjectService_GetHawkerHistory");
	case 1594: return _T("RPC_ObjectService_CB_GetHawkerHistory");
	case 1472: return _T("RPC_ObjectService_UpdateHawkerHistory");
	case 1985: return _T("RPC_ObjectService_CB_UpdateHawkerHistory");
	case 1153: return _T("RPC_ObjectService_RemoveRelation");
	case 1665: return _T("RPC_ObjectService_GetPartnerCollection");
	case 1196: return _T("RPC_ObjectService_CB_GetPartnerCollection");
	case 1080: return _T("RPC_ObjectService_CreatePartnerCollection");
	case 1370: return _T("RPC_ObjectService_CB_CreatePartnerCollection");
	case 1759: return _T("RPC_ObjectService_DeletePartnerCollection");
	case 1317: return _T("RPC_ObjectService_CB_DeletePartnerCollection");
	case 1102: return _T("RPC_ObjectService_GetAllSellYuanBao");
	case 1764: return _T("RPC_ObjectService_AddSellYuanBao");
	case 1227: return _T("RPC_ObjectService_CB_GetSellYuanBao");
	case 1478: return _T("RPC_ObjectService_BuySellYuanBao");
	case 1865: return _T("RPC_ObjectService_CB_BuySellYuanBao");
	case 1040: return _T("RPC_ObjectService_GetTempBank");
	case 1952: return _T("RPC_ObjectService_CB_GetTempBank");
	case 1548: return _T("RPC_ObjectService_GetSibling");
	case 1571: return _T("RPC_ObjectService_CB_GetSibling");
	case 1884: return _T("RPC_ObjectService_CreateSibling");
	case 1344: return _T("RPC_ObjectService_CB_CreateSibling");
	case 1271: return _T("RPC_ObjectService_UpdateSibling");
	case 1750: return _T("RPC_ObjectService_CB_UpdateSibling");
	case 1785: return _T("RPC_ObjectService_DeleteSibling");
	case 1668: return _T("RPC_ObjectService_CB_DeleteSibling");
	case 1606: return _T("RPC_ObjectService_UpdateWantedReward");
	case 1757: return _T("RPC_ObjectService_GetGeneration");
	case 1159: return _T("RPC_ObjectService_CB_GetGeneration");
	case 1131: return _T("RPC_ObjectService_CreateGeneration");
	case 1689: return _T("RPC_ObjectService_CB_CreateGeneration");
	case 1717: return _T("RPC_ObjectService_DeleteGeneration");
	case 1601: return _T("RPC_ObjectService_CB_DeleteGeneration");
	case 1296: return _T("RPC_ObjectService_UpdateGeneration");
	case 1803: return _T("RPC_ObjectService_CB_UpdateGeneration");
	case 1711: return _T("RPC_ObjectService_GetMarriage");
	case 1340: return _T("RPC_ObjectService_CB_GetMarriage");
	case 1681: return _T("RPC_ObjectService_CreateMarriage");
	case 1902: return _T("RPC_ObjectService_CB_CreateMarriage");
	case 1260: return _T("RPC_ObjectService_UpdateMarriage");
	case 1009: return _T("RPC_ObjectService_CB_UpdateMarriage");
	case 1111: return _T("RPC_ObjectService_DeleteMarriage");
	case 1966: return _T("RPC_ObjectService_CB_DeleteMarriage");
	case 1208: return _T("RPC_ObjectService_GetMarriageCount");
	case 1041: return _T("RPC_ObjectService_CB_GetMarriageCount");
	case 1124: return _T("RPC_ObjectService_GetSingleMarriageLst");
	case 1184: return _T("RPC_ObjectService_CB_GetSingleMarriageLst");
	case 1602: return _T("RPC_ObjectService_GetNpcFlagsByLineId");
	case 1369: return _T("RPC_ObjectService_CB_GetNpcFlagsByLineId");
	case 1036: return _T("RPC_ObjectService_UpdateNpcFlags");
	case 1258: return _T("RPC_ObjectService_CB_UpdateNpcFlags");
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
	case 28725: return _T("RPC_Partner_UP_STATE");
	case 28209: return _T("RPC_Partner_REQ_STATE");
	case 28757: return _T("RPC_Partner_RaiseAttrib");
	case 28149: return _T("RPC_Partner_ChangePartnerName");
	case 28485: return _T("RPC_Partner_ChangePartnerState");
	case 28494: return _T("RPC_Partner_CB_RaiseAttrib");
	case 28390: return _T("RPC_Partner_CB_ChangePartnerName");
	case 28680: return _T("RPC_Partner_CB_ChangePartnerState");
	case 28370: return _T("RPC_Partner_CB_UpdateSuccess");
	case 28959: return _T("RPC_Partner_CB_LearnSkillMessage");
	case 28794: return _T("RPC_Partner_CB_FalseJoinBattle");
	case 44602: return _T("RPC_Pet_UP_STATE");
	case 44405: return _T("RPC_Pet_REQ_STATE");
	case 44258: return _T("RPC_Pet_CB_RaiseAttrib");
	case 44643: return _T("RPC_Pet_ChangePetName");
	case 44728: return _T("RPC_Pet_CB_LearnSkillMessage");
	case 44188: return _T("RPC_Pet_CB_ChangePetName");
	case 44163: return _T("RPC_Pet_ChangePetExp");
	case 44182: return _T("RPC_Pet_ChangePetVp");
	case 44306: return _T("RPC_Pet_ChangePetFavor");
	case 3046: return _T("RPC_MapService_RegisterProxy");
	case 3621: return _T("RPC_MapService_UnregisterProxy");
	case 3423: return _T("RPC_MapService_ReleaseSession");
	case 3007: return _T("RPC_MapService_SyncProxy");
	case 3094: return _T("RPC_MapService_CB_SyncProxy");
	case 3904: return _T("RPC_MapService_ChangeSessionId");
	case 3978: return _T("RPC_MapService_ResumeSession");
	case 3483: return _T("RPC_MapService_CB_ResumeSession");
	case 3998: return _T("RPC_MapService_RemoteCreateNpc");
	case 3870: return _T("RPC_MapService_QueryInstance");
	case 3195: return _T("RPC_MapService_CB_QueryInstance");
	case 3421: return _T("RPC_MapService_CharacterEnter");
	case 3831: return _T("RPC_MapService_CharacterLeave");
	case 3191: return _T("RPC_MapService_NpcEnter");
	case 3461: return _T("RPC_MapService_ChangeLoadLevel");
	case 3247: return _T("RPC_MapService_RankTitleUpdate");
	case 3706: return _T("RPC_MapService_GuildUpdate");
	case 3070: return _T("RPC_MapService_GuildRemove");
	case 3777: return _T("RPC_MapService_GuildMemberUpdate");
	case 3664: return _T("RPC_MapService_GuildMemberRemove");
	case 3159: return _T("RPC_MapService_MsgCharacterUpdate");
	case 3180: return _T("RPC_MapService_ManorForceOut");
	case 3198: return _T("RPC_MapService_CB_Error");
	case 3497: return _T("RPC_MapService_CB_EnterSuccess");
	case 3781: return _T("RPC_MapService_CB_CharacterStuff");
	case 3559: return _T("RPC_MapService_CB_LeaveSuccess");
	case 3162: return _T("RPC_MapService_CB_CharacterEnter");
	case 3185: return _T("RPC_MapService_CB_PartyEnter");
	case 3015: return _T("RPC_MapService_CB_NpcEnter");
	case 3009: return _T("RPC_MapService_CB_EnterBattle");
	case 3639: return _T("RPC_MapService_CharacterCheat");
	case 3238: return _T("RPC_MapService_CB_CharacterCheat");
	case 3342: return _T("RPC_MapService_UpdateLimitedItemRemain");
	case 3012: return _T("RPC_MapService_ShowPublicItemDetail");
	case 3634: return _T("RPC_MapService_ShowPublicPartnerDetail");
	case 3153: return _T("RPC_MapService_ShowPublicPetDetail");
	case 3540: return _T("RPC_MapService_OpenChargeShop");
	case 3600: return _T("RPC_MapService_CloseChargeShop");
	case 3791: return _T("RPC_MapService_CB_OpenChargeShop");
	case 3456: return _T("RPC_MapService_CB_SyncCharacter");
	case 3628: return _T("RPC_MapService_CB_SyncItem");
	case 3126: return _T("RPC_MapService_CB_SyncPartner");
	case 3453: return _T("RPC_MapService_CB_AddPartnersSuccess");
	case 3740: return _T("RPC_MapService_CB_ShowPublicItemDetail");
	case 3176: return _T("RPC_MapService_CB_ShowPublicPartnerDetail");
	case 3814: return _T("RPC_MapService_CB_ShowPublicPetDetail");
	case 3546: return _T("RPC_MapService_SendMapMessage");
	case 3562: return _T("RPC_MapService_SendMapEmotion");
	case 3749: return _T("RPC_MapService_CB_ReceiveMapMessage");
	case 3567: return _T("RPC_MapService_CB_ReceiveMapEmotion");
	case 3897: return _T("RPC_MapService_CB_ForceLogout");
	case 3492: return _T("RPC_MapService_ChangeMoney");
	case 3242: return _T("RPC_MapService_ClientDisconnect");
	case 3687: return _T("RPC_MapService_GetPosition");
	case 3384: return _T("RPC_MapService_CB_GetPosition");
	case 3707: return _T("RPC_MapService_UpdateWantedReward");
	case 3389: return _T("RPC_MapService_CB_UpdateWantedReward");
	case 3795: return _T("RPC_MapService_DeleteGenerationFromWorld");
	case 3477: return _T("RPC_MapService_RecvGenerationAccmulation");
	case 3107: return _T("RPC_MapService_GenerationOnlineNotify");
	case 3158: return _T("RPC_MapService_GenerationOfflineNotify");
	case 3330: return _T("RPC_MapService_GetGenenrationCharsResult");
	case 3096: return _T("RPC_MapService_ShowPublicItemDetailFromOtherLine");
	case 3894: return _T("RPC_MapService_ShowPublicPartnerDetailFromOtherLine");
	case 3482: return _T("RPC_MapService_ShowPublicPetDetailFromOtherLine");
	case 3529: return _T("RPC_MapService_CB_ShowPublicPartnerDetailFromOtherLine");
	case 3336: return _T("RPC_MapService_CB_ShowPublicPetDetailFromOtherLine");
	case 3227: return _T("RPC_MapService_CB_ShowPublicItemDetailFromOtherLine");
	case 5058: return _T("RPC_MessageService_RegisterProxy");
	case 5407: return _T("RPC_MessageService_UnregisterProxy");
	case 5178: return _T("RPC_MessageService_RegisterMap");
	case 5993: return _T("RPC_MessageService_UnregisterMap");
	case 5334: return _T("RPC_MessageService_ReleaseSession");
	case 5192: return _T("RPC_MessageService_ApplyChange");
	case 5102: return _T("RPC_MessageService_GenerateGuildGroup");
	case 5007: return _T("RPC_MessageService_GenerateGuildGroupCW");
	case 5439: return _T("RPC_MessageService_GuildWarResult");
	case 5711: return _T("RPC_MessageService_KickMember");
	case 5330: return _T("RPC_MessageService_ManorScaleChange");
	case 5195: return _T("RPC_MessageService_TransmitGuildMsg");
	case 5341: return _T("RPC_MessageService_GetAvailService");
	case 5886: return _T("RPC_MessageService_SendHorseLightMessage");
	case 5832: return _T("RPC_MessageService_SendSystemMessage");
	case 5443: return _T("RPC_MessageService_SendRumorMessage");
	case 5848: return _T("RPC_MessageService_BattleEndUpdate");
	case 5313: return _T("RPC_MessageService_CB_BattleEndUpdate");
	case 5769: return _T("RPC_MessageService_AddMsgSchedule");
	case 5643: return _T("RPC_MessageService_RemoveMsgSchedule");
	case 5695: return _T("RPC_MessageService_UpdateCharInfo");
	case 5514: return _T("RPC_MessageService_CB_GetAvailService");
	case 5343: return _T("RPC_MessageService_ChannelsBlockHandle");
	case 5472: return _T("RPC_MessageService_ChangeMsgIcon");
	case 5482: return _T("RPC_MessageService_CB_ChangeMsgIcon");
	case 5740: return _T("RPC_MessageService_AddSpeakCount");
	case 5260: return _T("RPC_MessageService_ChangeGDonate");
	case 5264: return _T("RPC_MessageService_RemoveMessengersByOwner");
	case 5013: return _T("RPC_MessageService_CB_RemoveMessengersByOwner");
	case 5960: return _T("RPC_MessageService_RemoveAllGuildApplicant");
	case 5623: return _T("RPC_MessageService_CharacterLeave");
	case 5709: return _T("RPC_MessageService_CreateGuild");
	case 5479: return _T("RPC_MessageService_OpenGuildApply");
	case 5402: return _T("RPC_MessageService_CB_CreateGuild");
	case 5630: return _T("RPC_MessageService_DonateMoney");
	case 5658: return _T("RPC_MessageService_CB_DonateMoney");
	case 5086: return _T("RPC_MessageService_InitMessageChannel");
	case 5301: return _T("RPC_MessageService_ResumeMessageChannel");
	case 5966: return _T("RPC_MessageService_ReleaseMsgCharacter");
	case 5923: return _T("RPC_MessageService_CB_Error");
	case 5381: return _T("RPC_MessageService_CB_InitMessageChannel");
	case 5992: return _T("RPC_MessageService_CB_ReceiveMessageChannel");
	case 5295: return _T("RPC_MessageService_SendPrivateMessage");
	case 5905: return _T("RPC_MessageService_CB_SendPrivateMessage");
	case 5642: return _T("RPC_MessageService_SendFriendMessage");
	case 5648: return _T("RPC_MessageService_CB_ReceiveFriendMessage");
	case 5394: return _T("RPC_MessageService_CB_SendHorseLightMessage");
	case 5670: return _T("RPC_MessageService_CB_SendSystemMessage");
	case 5582: return _T("RPC_MessageService_CB_SendRumorMessage");
	case 5561: return _T("RPC_MessageService_CB_SendShutdownMessage");
	case 5715: return _T("RPC_MessageService_CB_ReceivePrivateMessage");
	case 5951: return _T("RPC_MessageService_CB_ReceiveHorseLightMessage");
	case 5529: return _T("RPC_MessageService_CB_ReceiveSystemMessage");
	case 5523: return _T("RPC_MessageService_CB_ReceiveRumorMessage");
	case 5170: return _T("RPC_MessageService_LeaveChannel");
	case 5809: return _T("RPC_MessageService_LeaveAllChannel");
	case 5964: return _T("RPC_MessageService_SetListen");
	case 5919: return _T("RPC_MessageService_CB_SetListen");
	case 5338: return _T("RPC_MessageService_JoinMessageChannel");
	case 5070: return _T("RPC_MessageService_CB_JoinMessageChannel");
	case 5326: return _T("RPC_MessageService_CreateMessenger");
	case 5567: return _T("RPC_MessageService_RemoveMessenger");
	case 5901: return _T("RPC_MessageService_RemoveRelation");
	case 5760: return _T("RPC_MessageService_CB_UpdateMessengers");
	case 5464: return _T("RPC_MessageService_CB_UpdateMsgCharInfo");
	case 5839: return _T("RPC_MessageService_CB_RemoveMessengers");
	case 5378: return _T("RPC_MessageService_RequestInfo");
	case 5243: return _T("RPC_MessageService_CB_RequestInfo");
	case 5843: return _T("RPC_MessageService_CB_MessageReturn");
	case 5851: return _T("RPC_MessageService_GetMessengers");
	case 5290: return _T("RPC_MessageService_CB_GetMessengers");
	case 5309: return _T("RPC_MessageService_GetMessengersName");
	case 5417: return _T("RPC_MessageService_CB_GetMessengersName");
	case 5447: return _T("RPC_MessageService_GetFriendly");
	case 5055: return _T("RPC_MessageService_CB_GetFriendly");
	case 5772: return _T("RPC_MessageService_IsFriend");
	case 5425: return _T("RPC_MessageService_CB_IsFriend");
	case 5920: return _T("RPC_MessageService_ChangeFriendly");
	case 5884: return _T("RPC_MessageService_CB_ChangeFriendly");
	case 5159: return _T("RPC_MessageService_ApplyGuild");
	case 5069: return _T("RPC_MessageService_RequestGuildAim");
	case 5813: return _T("RPC_MessageService_CB_ApplyGuild");
	case 5936: return _T("RPC_MessageService_CB_Guilds");
	case 5089: return _T("RPC_MessageService_CB_GuildMembers");
	case 5359: return _T("RPC_MessageService_CB_RequestApplicant");
	case 5077: return _T("RPC_MessageService_CB_CreateGuildMsg");
	case 5218: return _T("RPC_MessageService_ChangeTitle");
	case 5132: return _T("RPC_MessageService_SetMoney");
	case 5389: return _T("RPC_MessageService_GetTargetFromHawk");
	case 5122: return _T("RPC_MessageService_CB_GetTargetFromHawk");
	case 5354: return _T("RPC_MessageService_BuyItemFromBusinessShop");
	case 5109: return _T("RPC_MessageService_CB_BuyItemFromBusinessShop");
	case 5603: return _T("RPC_MessageService_SellItemToBusinessShop");
	case 5819: return _T("RPC_MessageService_CB_SellItemToBusinessShop");
	case 5299: return _T("RPC_MessageService_OpenBusinessShopMenu");
	case 5493: return _T("RPC_MessageService_CB_OpenBusinessShopMenu");
	case 5025: return _T("RPC_MessageService_UpdateBusinessShopList");
	case 5971: return _T("RPC_MessageService_CB_UpdateBusinessShopList");
	case 5273: return _T("RPC_MessageService_ChangeBusinessProfit");
	case 5166: return _T("RPC_MessageService_ApplyTrader");
	case 5252: return _T("RPC_MessageService_GetGenerationChars");
	case 5199: return _T("RPC_MessageService_GetTargetInfo");
	case 5285: return _T("RPC_MessageService_CB_GetTargetInfo");
	case 5955: return _T("RPC_MessageService_ClientDisconnect");
	case 5367: return _T("RPC_MessageService_ShowPublicPartnerDetailFromOtherLine");
	case 5256: return _T("RPC_MessageService_ShowPublicPetDetailFromOtherLine");
	case 5763: return _T("RPC_MessageService_ShowPublicItemDetailFromOtherLine");
	case 5348: return _T("RPC_MessageService_CB_ShowPublicItemDetailFromOtherLine");
	case 5978: return _T("RPC_MessageService_CB_ShowPublicPartnerDetailFromOtherLine");
	case 5034: return _T("RPC_MessageService_CB_ShowPublicPetDetailFromOtherLine");
	case 27302: return _T("RPC_Channel_UP_STATE");
	case 27448: return _T("RPC_Channel_REQ_STATE");
	case 27940: return _T("RPC_Channel_SendGrpMessage");
	case 27028: return _T("RPC_Channel_SendGrpFlushMessage");
	case 27687: return _T("RPC_Channel_SendEmotion");
	case 27646: return _T("RPC_Channel_CB_ReceiveMessage");
	case 27109: return _T("RPC_Channel_CB_SendGrpMessage");
	case 27817: return _T("RPC_Channel_CB_ReceiveEmotion");
	case 27939: return _T("RPC_Channel_SetListen");
	case 27039: return _T("RPC_Channel_CB_SetListen");
	case 27770: return _T("RPC_Channel_LeaveChannel");
	case 27284: return _T("RPC_Channel_CB_LeaveChannel");
	case 38788: return _T("RPC_Messenger_UP_STATE");
	case 38398: return _T("RPC_Messenger_REQ_STATE");
	case 38163: return _T("RPC_Messenger_MoveGroup");
	case 38626: return _T("RPC_Messenger_ModifyRelation");
	case 40816: return _T("RPC_MessageCharacter_UP_STATE");
	case 40239: return _T("RPC_MessageCharacter_REQ_STATE");
	case 40592: return _T("RPC_MessageCharacter_SelfSettingChange");
	case 49714: return _T("RPC_Guild_UP_STATE");
	case 49905: return _T("RPC_Guild_REQ_STATE");
	case 49037: return _T("RPC_Guild_CB_AddMember");
	case 49712: return _T("RPC_Guild_CB_RemoveMember");
	case 49717: return _T("RPC_Guild_CB_AddApplicant");
	case 49888: return _T("RPC_Guild_CB_RemoveApplicant");
	case 50535: return _T("RPC_GuildMember_UP_STATE");
	case 50130: return _T("RPC_GuildMember_REQ_STATE");
	case 50112: return _T("RPC_GuildMember_ResponseApplicant");
	case 50432: return _T("RPC_GuildMember_PositionChange");
	case 50429: return _T("RPC_GuildMember_ChangeAim");
	case 50005: return _T("RPC_GuildMember_ChangeRule");
	case 50051: return _T("RPC_GuildMember_ClearGuildBusinessLog");
	case 50693: return _T("RPC_GuildMember_CB_Error");
	case 50966: return _T("RPC_GuildMember_CB_PositionChange");
	}
	static Char buf[12];

	_stprintf(buf, _T("RPCID %d"), rpcId);
	return buf;
}
#endif // CLIENT

#pragma optimize("", on)

