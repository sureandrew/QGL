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
#include "Proxy/Service/WorldService_Proxy.h"
#include "Proxy/Data/PartnerData_Proxy.h"
#include "Proxy/Data/CharacterData_Proxy.h"
#include "Proxy/Data/ItemData_Proxy.h"
#include "Proxy/Service/MessageService_Proxy.h"
#include "Proxy/Data/SkillData_Proxy.h"
#include "Proxy/Data/QuestLogData_Proxy.h"
#include "Proxy/Data/QuestFlagData_Proxy.h"
#include "Proxy/Data/QuestTypeData_Proxy.h"
#include "Proxy/Data/BuffData_Proxy.h"
#include "Proxy/Service/ServiceStatus_Proxy.h"
#include "Proxy/Logic/Messenger_Proxy.h"
#include "Proxy/Data/PetData_Proxy.h"
#include "Proxy/Data/TitleData_Proxy.h"
#include "Proxy/Service/LogService_Proxy.h"
#include "Proxy/Data/GiftData_Proxy.h"
#include "Proxy/Data/HawkerHistoryData_Proxy.h"
#include "Proxy/Logic/WorldEvent_Proxy.h"
#include "Proxy/Data/ChargeShopData_Proxy.h"
#include "Proxy/Data/PartnerCollectionData_Proxy.h"
#include "Proxy/Data/SiblingData_Proxy.h"
#include "Proxy/Data/GenerationData_Proxy.h"
#include "Proxy/Data/MarriageData_Proxy.h"
#include "Proxy/Data/QuestNpcFlagData_Proxy.h"
#include "Stub/Service/MapService_Stub.h"
#include "RPCEnum/Map_enum.h"
#include "RPCEnum/Party_enum.h"
#include "RPCEnum/MapActor_enum.h"
#include "RPCEnum/MapNPC_enum.h"
#include "RPCEnum/Character_enum.h"
#include "RPCEnum/Item_enum.h"
#include "RPCEnum/ItemBag_enum.h"
#include "RPCEnum/ScriptSystem_enum.h"
#include "RPCEnum/Partner_enum.h"
#include "RPCEnum/PartnerGroup_enum.h"
#include "RPCEnum/Skill_enum.h"
#include "RPCEnum/SkillGroup_enum.h"
#include "RPCEnum/QuestLog_enum.h"
#include "RPCEnum/QuestSystem_enum.h"
#include "RPCEnum/QuestType_enum.h"
#include "RPCEnum/Battle_enum.h"
#include "RPCEnum/BatActor_enum.h"
#include "RPCEnum/BatCharacter_enum.h"
#include "RPCEnum/BatMob_enum.h"
#include "RPCEnum/Trading_enum.h"
#include "RPCEnum/Pet_enum.h"
#include "RPCEnum/PetGroup_enum.h"
#include "RPCEnum/HawkItem_enum.h"
#include "RPCEnum/HawkGroup_enum.h"
#include "RPCEnum/Gift_enum.h"
#include "RPCEnum/GiftBox_enum.h"
#include "RPCEnum/QuestJournal_enum.h"
#include "RPCEnum/ChargeShop_enum.h"
#include "RPCEnum/ChargeShopItem_enum.h"
#include "RPCEnum/Question_enum.h"
#include "RPCEnum/RelationSystem_enum.h"
#include "RPCEnum/Generation_enum.h"
#include "RPCEnum/PartyCharInfo_enum.h"

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
	else if (rpcId >= RPC_WorldService_ENUM_ST && rpcId <= RPC_WorldService_ENUM_ED)
	{
		_result = WorldService_Proxy::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_PartnerData_ENUM_ST && rpcId <= RPC_PartnerData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_CharacterData_ENUM_ST && rpcId <= RPC_CharacterData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_ItemData_ENUM_ST && rpcId <= RPC_ItemData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_MessageService_ENUM_ST && rpcId <= RPC_MessageService_ENUM_ED)
	{
		_result = MessageService_Proxy::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_SkillData_ENUM_ST && rpcId <= RPC_SkillData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_QuestLogData_ENUM_ST && rpcId <= RPC_QuestLogData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_QuestFlagData_ENUM_ST && rpcId <= RPC_QuestFlagData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_QuestTypeData_ENUM_ST && rpcId <= RPC_QuestTypeData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_BuffData_ENUM_ST && rpcId <= RPC_BuffData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_ServiceStatus_ENUM_ST && rpcId <= RPC_ServiceStatus_ENUM_ED)
	{
		_result = ServiceStatus_Proxy::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Messenger_ENUM_ST && rpcId <= RPC_Messenger_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_PetData_ENUM_ST && rpcId <= RPC_PetData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_TitleData_ENUM_ST && rpcId <= RPC_TitleData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_LogService_ENUM_ST && rpcId <= RPC_LogService_ENUM_ED)
	{
		_result = LogService_Proxy::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_GiftData_ENUM_ST && rpcId <= RPC_GiftData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_HawkerHistoryData_ENUM_ST && rpcId <= RPC_HawkerHistoryData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_WorldEvent_ENUM_ST && rpcId <= RPC_WorldEvent_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_ChargeShopData_ENUM_ST && rpcId <= RPC_ChargeShopData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_PartnerCollectionData_ENUM_ST && rpcId <= RPC_PartnerCollectionData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_SiblingData_ENUM_ST && rpcId <= RPC_SiblingData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_GenerationData_ENUM_ST && rpcId <= RPC_GenerationData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_MarriageData_ENUM_ST && rpcId <= RPC_MarriageData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_QuestNpcFlagData_ENUM_ST && rpcId <= RPC_QuestNpcFlagData_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_MapService_ENUM_ST && rpcId <= RPC_MapService_ENUM_ED)
	{
		_result = MapService_Stub::Process(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Map_ENUM_ST && rpcId <= RPC_Map_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Party_ENUM_ST && rpcId <= RPC_Party_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_MapActor_ENUM_ST && rpcId <= RPC_MapActor_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_MapNPC_ENUM_ST && rpcId <= RPC_MapNPC_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Character_ENUM_ST && rpcId <= RPC_Character_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Item_ENUM_ST && rpcId <= RPC_Item_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_ItemBag_ENUM_ST && rpcId <= RPC_ItemBag_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_ScriptSystem_ENUM_ST && rpcId <= RPC_ScriptSystem_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Partner_ENUM_ST && rpcId <= RPC_Partner_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_PartnerGroup_ENUM_ST && rpcId <= RPC_PartnerGroup_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Skill_ENUM_ST && rpcId <= RPC_Skill_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_SkillGroup_ENUM_ST && rpcId <= RPC_SkillGroup_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_QuestLog_ENUM_ST && rpcId <= RPC_QuestLog_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_QuestSystem_ENUM_ST && rpcId <= RPC_QuestSystem_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_QuestType_ENUM_ST && rpcId <= RPC_QuestType_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Battle_ENUM_ST && rpcId <= RPC_Battle_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_BatActor_ENUM_ST && rpcId <= RPC_BatActor_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_BatCharacter_ENUM_ST && rpcId <= RPC_BatCharacter_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_BatMob_ENUM_ST && rpcId <= RPC_BatMob_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Trading_ENUM_ST && rpcId <= RPC_Trading_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Pet_ENUM_ST && rpcId <= RPC_Pet_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_PetGroup_ENUM_ST && rpcId <= RPC_PetGroup_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_HawkItem_ENUM_ST && rpcId <= RPC_HawkItem_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_HawkGroup_ENUM_ST && rpcId <= RPC_HawkGroup_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Gift_ENUM_ST && rpcId <= RPC_Gift_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_GiftBox_ENUM_ST && rpcId <= RPC_GiftBox_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_QuestJournal_ENUM_ST && rpcId <= RPC_QuestJournal_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_ChargeShop_ENUM_ST && rpcId <= RPC_ChargeShop_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_ChargeShopItem_ENUM_ST && rpcId <= RPC_ChargeShopItem_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Question_ENUM_ST && rpcId <= RPC_Question_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_RelationSystem_ENUM_ST && rpcId <= RPC_RelationSystem_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_Generation_ENUM_ST && rpcId <= RPC_Generation_ENUM_ED)
	{
		_result = ObjectProcess(rpcId, pPeer, context, pBuf);
	}
	else if (rpcId >= RPC_PartyCharInfo_ENUM_ST && rpcId <= RPC_PartyCharInfo_ENUM_ED)
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
	else if (rpcId >= RPC_WorldService_ENUM_ST && rpcId <= RPC_WorldService_ENUM_ED)
	{
		_result = WorldService_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_PartnerData_ENUM_ST && rpcId <= RPC_PartnerData_ENUM_ED)
	{
		_result = PartnerData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_CharacterData_ENUM_ST && rpcId <= RPC_CharacterData_ENUM_ED)
	{
		_result = CharacterData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_ItemData_ENUM_ST && rpcId <= RPC_ItemData_ENUM_ED)
	{
		_result = ItemData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_MessageService_ENUM_ST && rpcId <= RPC_MessageService_ENUM_ED)
	{
		_result = MessageService_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_SkillData_ENUM_ST && rpcId <= RPC_SkillData_ENUM_ED)
	{
		_result = SkillData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_QuestLogData_ENUM_ST && rpcId <= RPC_QuestLogData_ENUM_ED)
	{
		_result = QuestLogData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_QuestFlagData_ENUM_ST && rpcId <= RPC_QuestFlagData_ENUM_ED)
	{
		_result = QuestFlagData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_QuestTypeData_ENUM_ST && rpcId <= RPC_QuestTypeData_ENUM_ED)
	{
		_result = QuestTypeData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_BuffData_ENUM_ST && rpcId <= RPC_BuffData_ENUM_ED)
	{
		_result = BuffData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_ServiceStatus_ENUM_ST && rpcId <= RPC_ServiceStatus_ENUM_ED)
	{
		_result = ServiceStatus_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_Messenger_ENUM_ST && rpcId <= RPC_Messenger_ENUM_ED)
	{
		_result = Messenger_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_PetData_ENUM_ST && rpcId <= RPC_PetData_ENUM_ED)
	{
		_result = PetData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_TitleData_ENUM_ST && rpcId <= RPC_TitleData_ENUM_ED)
	{
		_result = TitleData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_LogService_ENUM_ST && rpcId <= RPC_LogService_ENUM_ED)
	{
		_result = LogService_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_GiftData_ENUM_ST && rpcId <= RPC_GiftData_ENUM_ED)
	{
		_result = GiftData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_HawkerHistoryData_ENUM_ST && rpcId <= RPC_HawkerHistoryData_ENUM_ED)
	{
		_result = HawkerHistoryData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_WorldEvent_ENUM_ST && rpcId <= RPC_WorldEvent_ENUM_ED)
	{
		_result = WorldEvent_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_ChargeShopData_ENUM_ST && rpcId <= RPC_ChargeShopData_ENUM_ED)
	{
		_result = ChargeShopData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_PartnerCollectionData_ENUM_ST && rpcId <= RPC_PartnerCollectionData_ENUM_ED)
	{
		_result = PartnerCollectionData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_SiblingData_ENUM_ST && rpcId <= RPC_SiblingData_ENUM_ED)
	{
		_result = SiblingData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_GenerationData_ENUM_ST && rpcId <= RPC_GenerationData_ENUM_ED)
	{
		_result = GenerationData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_MarriageData_ENUM_ST && rpcId <= RPC_MarriageData_ENUM_ED)
	{
		_result = MarriageData_Proxy::Decode(rpcId, pPeer, pBuf);
	}
	else if (rpcId >= RPC_QuestNpcFlagData_ENUM_ST && rpcId <= RPC_QuestNpcFlagData_ENUM_ED)
	{
		_result = QuestNpcFlagData_Proxy::Decode(rpcId, pPeer, pBuf);
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
	case 7738: return _T("RPC_ServiceStatus_UpdateStatus");
	case 7739: return _T("RPC_ServiceStatus_CB_CheckService");
	case 7633: return _T("RPC_ServiceStatus_CB_ShutdownAt");
	case 7554: return _T("RPC_ServiceStatus_CB_StopService");
	case 38788: return _T("RPC_Messenger_UP_STATE");
	case 38398: return _T("RPC_Messenger_REQ_STATE");
	case 38163: return _T("RPC_Messenger_MoveGroup");
	case 38626: return _T("RPC_Messenger_ModifyRelation");
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
	case 16560: return _T("RPC_Map_UP_STATE");
	case 16797: return _T("RPC_Map_REQ_STATE");
	case 16630: return _T("RPC_Map_GetCharInRange");
	case 16864: return _T("RPC_Map_GetJumpPoints");
	case 16569: return _T("RPC_Map_CB_Error");
	case 16358: return _T("RPC_Map_CB_GetCharInRange");
	case 16187: return _T("RPC_Map_CB_GetJumpPoints");
	case 16889: return _T("RPC_Map_CB_ObjectChanged");
	case 22567: return _T("RPC_Party_UP_STATE");
	case 22137: return _T("RPC_Party_REQ_STATE");
	case 22008: return _T("RPC_Party_Promote");
	case 22298: return _T("RPC_Party_Kick");
	case 22478: return _T("RPC_Party_Leave");
	case 22510: return _T("RPC_Party_Destroy");
	case 22918: return _T("RPC_Party_JoinChannel");
	case 22344: return _T("RPC_Party_ChangeDesc");
	case 22987: return _T("RPC_Party_CB_Error");
	case 22595: return _T("RPC_Party_CB_LeaderChanged");
	case 22091: return _T("RPC_Party_CB_Join");
	case 22050: return _T("RPC_Party_CB_Leave");
	case 22806: return _T("RPC_Party_CB_Destroy");
	case 22022: return _T("RPC_Party_CB_Regroup");
	case 13669: return _T("RPC_Character_UP_STATE");
	case 13240: return _T("RPC_Character_REQ_STATE");
	case 13795: return _T("RPC_Character_MoveTo");
	case 13375: return _T("RPC_Character_StopMove");
	case 13955: return _T("RPC_Character_RespondTest");
	case 13149: return _T("RPC_Character_TestBattle");
	case 13328: return _T("RPC_Character_CancelAuto");
	case 13142: return _T("RPC_Character_LevelUp");
	case 13558: return _T("RPC_Character_RaiseAttrib");
	case 13277: return _T("RPC_Character_PKRequest");
	case 13056: return _T("RPC_Character_LeaveWatching");
	case 13004: return _T("RPC_Character_MonitorBattle");
	case 13083: return _T("RPC_Character_JoinBattle");
	case 13521: return _T("RPC_Character_RequestJoinBattle");
	case 13109: return _T("RPC_Character_RejectHelper");
	case 13765: return _T("RPC_Character_SetFollowPartner");
	case 13219: return _T("RPC_Character_SetFollowPet");
	case 13568: return _T("RPC_Character_ChangeMapLine");
	case 13204: return _T("RPC_Character_UpdateYuanBao");
	case 13688: return _T("RPC_Character_CB_RequestJoinBattle");
	case 13424: return _T("RPC_Character_CB_PKRequest");
	case 13235: return _T("RPC_Character_CB_SetEnablePK");
	case 13064: return _T("RPC_Character_CB_JoinBattle");
	case 13709: return _T("RPC_Character_CB_CancelAuto");
	case 13114: return _T("RPC_Character_CB_LevelUp");
	case 13005: return _T("RPC_Character_CB_RaiseAttrib");
	case 13137: return _T("RPC_Character_CB_AddHelper");
	case 13063: return _T("RPC_Character_CB_RejectHelper");
	case 13011: return _T("RPC_Character_CB_LeaveHelpersList");
	case 13341: return _T("RPC_Character_CB_ClearHelpersList");
	case 13581: return _T("RPC_Character_CB_HelpRequestFail");
	case 13451: return _T("RPC_Character_CB_WarningMessage");
	case 13441: return _T("RPC_Character_SearchChar");
	case 13878: return _T("RPC_Character_CB_SearchChar");
	case 13293: return _T("RPC_Character_CB_OpenWeaponsRankPanel");
	case 13183: return _T("RPC_Character_SetTarget");
	case 13636: return _T("RPC_Character_CB_SetTarget");
	case 13089: return _T("RPC_Character_CreateParty");
	case 13987: return _T("RPC_Character_PartyJoinRequest");
	case 13814: return _T("RPC_Character_PartyJoinLevelControl");
	case 13824: return _T("RPC_Character_PartyJoinReply");
	case 13170: return _T("RPC_Character_PartyInviteRequest");
	case 13815: return _T("RPC_Character_PartyInviteReply");
	case 13690: return _T("RPC_Character_SearchParty");
	case 13097: return _T("RPC_Character_GetPartyInfo");
	case 13015: return _T("RPC_Character_CB_Error");
	case 13628: return _T("RPC_Character_CB_JoinParty");
	case 13278: return _T("RPC_Character_CB_ChangeDesc");
	case 13995: return _T("RPC_Character_CB_PartyRequestOk");
	case 13336: return _T("RPC_Character_CB_PartyRequestNo");
	case 13228: return _T("RPC_Character_CB_GetPlayerEquip");
	case 13086: return _T("RPC_Character_CB_PartyInviteOk");
	case 13730: return _T("RPC_Character_CB_PartyJoinRequest");
	case 13686: return _T("RPC_Character_CB_PartyInviteRequest");
	case 13279: return _T("RPC_Character_CB_PartyClearRequest");
	case 13201: return _T("RPC_Character_CB_PartyClearInvite");
	case 13322: return _T("RPC_Character_CB_PartyMessage");
	case 13349: return _T("RPC_Character_CB_SearchParty");
	case 13737: return _T("RPC_Character_CB_GetPartyInfo");
	case 13996: return _T("RPC_Character_CB_OpenSearchPartyPanel");
	case 13335: return _T("RPC_Character_ShutNewGuide");
	case 13342: return _T("RPC_Character_CB_HandleGuideEvent");
	case 13682: return _T("RPC_Character_CheatCode");
	case 13934: return _T("RPC_Character_CB_RespondTest");
	case 13369: return _T("RPC_Character_LearnSkill");
	case 13627: return _T("RPC_Character_UseSkill");
	case 13062: return _T("RPC_Character_UpGradeSkill");
	case 13495: return _T("RPC_Character_RemoveSkill");
	case 13900: return _T("RPC_Character_CB_LearnSkill");
	case 13483: return _T("RPC_Character_CB_UseSkill");
	case 13821: return _T("RPC_Character_CB_UpGradeSkill");
	case 13577: return _T("RPC_Character_CB_RemoveSkill");
	case 13481: return _T("RPC_Character_SkillShopLearn");
	case 13712: return _T("RPC_Character_LeaveSkillShop");
	case 13294: return _T("RPC_Character_CB_SkillShopLearn");
	case 13866: return _T("RPC_Character_GivenProcess");
	case 13910: return _T("RPC_Character_CB_GivenProcess");
	case 13376: return _T("RPC_Character_CB_GivenItem");
	case 13915: return _T("RPC_Character_CB_GivenPartner");
	case 13813: return _T("RPC_Character_CB_GivenPet");
	case 13725: return _T("RPC_Character_CB_GivenMoney");
	case 13194: return _T("RPC_Character_UpdateShortCutList");
	case 13469: return _T("RPC_Character_UpdateShortCut");
	case 13398: return _T("RPC_Character_CB_UpdateShortCutSuccess");
	case 13786: return _T("RPC_Character_SaveSystemSetting");
	case 13604: return _T("RPC_Character_CB_SaveSystemSetting");
	case 13347: return _T("RPC_Character_ChangeMsgIcon");
	case 13410: return _T("RPC_Character_AskForTrading");
	case 13290: return _T("RPC_Character_CB_AskForTrading");
	case 13677: return _T("RPC_Character_ReplyReqTrading");
	case 13260: return _T("RPC_Character_CB_ReplyReqTrading");
	case 13289: return _T("RPC_Character_CB_TradingMessage");
	case 13881: return _T("RPC_Character_CB_TradingObject");
	case 13790: return _T("RPC_Character_TalkToNPC");
	case 13537: return _T("RPC_Character_CB_TalkToNPC");
	case 13708: return _T("RPC_Character_ChangeTitle");
	case 13090: return _T("RPC_Character_CB_TitleListUpdate");
	case 13455: return _T("RPC_Character_CB_AntiHack");
	case 13261: return _T("RPC_Character_AnswerAntiHack");
	case 13655: return _T("RPC_Character_CB_ReportAntiHack");
	case 13214: return _T("RPC_Character_DepositInBank");
	case 13721: return _T("RPC_Character_WithdrawFmBank");
	case 13539: return _T("RPC_Character_CB_DepositInBank");
	case 13193: return _T("RPC_Character_CB_WithdrawFmBank");
	case 13673: return _T("RPC_Character_StartHawk");
	case 13868: return _T("RPC_Character_CB_StartHawk");
	case 13832: return _T("RPC_Character_GetHawkFromTarget");
	case 13498: return _T("RPC_Character_CB_GetHawk");
	case 13330: return _T("RPC_Character_CB_HawkMessage");
	case 13394: return _T("RPC_Character_CB_HawkHistory");
	case 13106: return _T("RPC_Character_CB_UpdateHawkHistory");
	case 13629: return _T("RPC_Character_ClearHawkerHistory");
	case 13819: return _T("RPC_Character_CB_SendGrpMessage");
	case 13388: return _T("RPC_Character_SetListen");
	case 13265: return _T("RPC_Character_DonateMoney");
	case 13127: return _T("RPC_Character_CreateGuild");
	case 13138: return _T("RPC_Character_CB_OpenCreateGuild");
	case 13612: return _T("RPC_Character_CB_AwardMsg");
	case 13700: return _T("RPC_Character_CB_OpenGiftBox");
	case 13415: return _T("RPC_Character_CB_AddictionWarning");
	case 13331: return _T("RPC_Character_CB_AddictCurrentStatus");
	case 13547: return _T("RPC_Character_CB_AddictionShutDown");
	case 13310: return _T("RPC_Character_CB_PlyInterestDwn");
	case 13253: return _T("RPC_Character_CB_UseItemCharacter");
	case 13560: return _T("RPC_Character_EnterQuestion");
	case 13920: return _T("RPC_Character_ExitQuestion");
	case 13023: return _T("RPC_Character_CB_AskEnterQuestion");
	case 13575: return _T("RPC_Character_CB_EnterQuestion");
	case 13189: return _T("RPC_Character_CB_ExitQuestion");
	case 13530: return _T("RPC_Character_CB_AnswerQuestion");
	case 13828: return _T("RPC_Character_CloseWareHouse");
	case 13506: return _T("RPC_Character_CB_OpenWareHouse");
	case 13414: return _T("RPC_Character_LockAsset");
	case 13428: return _T("RPC_Character_UnlockAsset");
	case 13263: return _T("RPC_Character_SetAssetPassword");
	case 13200: return _T("RPC_Character_ForceResetAssetPassword");
	case 13197: return _T("RPC_Character_ChangeAssetPassword");
	case 13238: return _T("RPC_Character_CB_AssetProtectResult");
	case 13965: return _T("RPC_Character_CB_AssetReleaseLockResult");
	case 13528: return _T("RPC_Character_CB_SetAssetPassword");
	case 13572: return _T("RPC_Character_CB_ForceResetAssetPassword");
	case 13053: return _T("RPC_Character_CB_SetRobot");
	case 13312: return _T("RPC_Character_CB_RobotMapAction");
	case 13947: return _T("RPC_Character_CB_RobotBattleTurn");
	case 13777: return _T("RPC_Character_SetAutoWalk");
	case 13213: return _T("RPC_Character_SetAutoWalkHitBoss");
	case 13071: return _T("RPC_Character_CB_SetAutoWalkHitBoss");
	case 13147: return _T("RPC_Character_KickMember");
	case 13158: return _T("RPC_Character_CB_DonateResult");
	case 13038: return _T("RPC_Character_CB_GetPartnerCollectionList");
	case 13234: return _T("RPC_Character_UsePartnerCollectionBuff");
	case 13602: return _T("RPC_Character_CB_UsePartnerCollectionBuff");
	case 13715: return _T("RPC_Character_AddPartnerCollection");
	case 13413: return _T("RPC_Character_CB_AddPartnerCollection");
	case 13998: return _T("RPC_Character_ChangeBetweenYBandYBT");
	case 13327: return _T("RPC_Character_CB_ChangeBetweenYBandYBT");
	case 13366: return _T("RPC_Character_AddSellYuanBao");
	case 13332: return _T("RPC_Character_CB_AddSellYuanBao");
	case 13355: return _T("RPC_Character_BuySellYuanBao");
	case 13096: return _T("RPC_Character_CB_BuySellYuanBao");
	case 13272: return _T("RPC_Character_GetAllMoneyAndYBT");
	case 13717: return _T("RPC_Character_CB_GetAllMoneyAndYBT");
	case 13475: return _T("RPC_Character_RequestTargetEquip");
	case 13672: return _T("RPC_Character_CB_RequestTargetEquip");
	case 13780: return _T("RPC_Character_CB_RelationSystem");
	case 13954: return _T("RPC_Character_AddStartHawkItem");
	case 13658: return _T("RPC_Character_CB_SetTargetNameFormHawk");
	case 13169: return _T("RPC_Character_CB_InitTargetHawkConcernState");
	case 13516: return _T("RPC_Character_ModifyLineup");
	case 13143: return _T("RPC_Character_CB_ModifyLineup");
	case 13614: return _T("RPC_Character_ModifyLineupKeyPos");
	case 13110: return _T("RPC_Character_CB_ModifyLineupKeyPos");
	case 13180: return _T("RPC_Character_ForgiveLineup");
	case 13948: return _T("RPC_Character_CB_ForgiveLineup");
	case 13493: return _T("RPC_Character_SetChangePos");
	case 13230: return _T("RPC_Character_CB_SetChangePos");
	case 13188: return _T("RPC_Character_UpdateWantedReward");
	case 13807: return _T("RPC_Character_CB_UpdateWantedReward");
	case 13350: return _T("RPC_Character_JailDelivery");
	case 13991: return _T("RPC_Character_CB_JailDelivery");
	case 13728: return _T("RPC_Character_WantedBail");
	case 13365: return _T("RPC_Character_CB_WantedBail");
	case 13750: return _T("RPC_Character_WantedTrace");
	case 13804: return _T("RPC_Character_CB_WantedTrace");
	case 13772: return _T("RPC_Character_CB_RequestWantedTrace");
	case 13490: return _T("RPC_Character_GetGenerationGroup");
	case 13985: return _T("RPC_Character_CB_GetGenerationGroup");
	case 13592: return _T("RPC_Character_RequestGeneration");
	case 13077: return _T("RPC_Character_CB_RequestGenerationTrans");
	case 13668: return _T("RPC_Character_RequestGenerationAns");
	case 13803: return _T("RPC_Character_CB_RequestGenerationResult");
	case 13320: return _T("RPC_Character_DeleteGeneration");
	case 13192: return _T("RPC_Character_CB_DeleteGeneration");
	case 13416: return _T("RPC_Character_CB_ChangeGenerationType");
	case 13903: return _T("RPC_Character_GetSavedExp");
	case 13032: return _T("RPC_Character_CB_UpdateGenerationAcc");
	case 13848: return _T("RPC_Character_BuyAccShopItem");
	case 13223: return _T("RPC_Character_CB_BugAccShopItem");
	case 13597: return _T("RPC_Character_CB_GenerationGainAcc");
	case 13760: return _T("RPC_Character_BuyPointsShopItem");
	case 13447: return _T("RPC_Character_CB_BuyPointsShopItem");
	case 13756: return _T("RPC_Character_CB_OpenBusinessLog");
	case 13973: return _T("RPC_Character_CB_SetMarriageNpcAction");
	case 13473: return _T("RPC_Character_CB_ShowMarriageEff");
	case 13598: return _T("RPC_Character_CB_ResetChangeLine");
	case 13315: return _T("RPC_Character_CB_PetUseSkillMsg");
	case 13754: return _T("RPC_Character_RequestCrossMapJump");
	case 13300: return _T("RPC_Character_LeaveTeamTemporarily");
	case 13761: return _T("RPC_Character_CB_LeaveTeamTemporarily");
	case 13462: return _T("RPC_Character_CB_GetPartyInfoWhenJumpMap");
	case 13764: return _T("RPC_Character_ReturnParty");
	case 13646: return _T("RPC_Character_CB_ReturnParty");
	case 13134: return _T("RPC_Character_CB_RefreshPartyCharInfo");
	case 13111: return _T("RPC_Character_CB_RestorePartyAsTemp");
	case 13256: return _T("RPC_Character_CB_SetChangeModelSkillGroup");
	case 13699: return _T("RPC_Character_CB_ShowIncognitoBoxEff");
	case 23404: return _T("RPC_ItemBag_UP_STATE");
	case 23753: return _T("RPC_ItemBag_REQ_STATE");
	case 23621: return _T("RPC_ItemBag_MoveItem");
	case 23572: return _T("RPC_ItemBag_DropItem");
	case 23401: return _T("RPC_ItemBag_UseItem");
	case 23268: return _T("RPC_ItemBag_EquipItem");
	case 23351: return _T("RPC_ItemBag_BuyItemFromShop");
	case 23393: return _T("RPC_ItemBag_SellItemToShop");
	case 23747: return _T("RPC_ItemBag_MarkMixStartTime");
	case 23185: return _T("RPC_ItemBag_MakeItemMix");
	case 23031: return _T("RPC_ItemBag_MixItem");
	case 23423: return _T("RPC_ItemBag_IdentifyItem");
	case 23078: return _T("RPC_ItemBag_PutToWareHouse");
	case 23843: return _T("RPC_ItemBag_GetFromWareHouse");
	case 23739: return _T("RPC_ItemBag_WareHouseItemMove");
	case 23725: return _T("RPC_ItemBag_BuyItemFromBusinessShop");
	case 23550: return _T("RPC_ItemBag_SellItemToBusinessShop");
	case 23530: return _T("RPC_ItemBag_UseFastFoodItem");
	case 23748: return _T("RPC_ItemBag_Jpack");
	case 23096: return _T("RPC_ItemBag_CleanWarehouse");
	case 23284: return _T("RPC_ItemBag_CB_Error");
	case 23304: return _T("RPC_ItemBag_CB_SyncItems");
	case 23723: return _T("RPC_ItemBag_CB_ItemCount");
	case 23995: return _T("RPC_ItemBag_CB_BuytItemSuccess");
	case 23893: return _T("RPC_ItemBag_CB_SelltItemSuccess");
	case 23233: return _T("RPC_ItemBag_CB_ItemUseResult");
	case 23125: return _T("RPC_ItemBag_CB_EqItemGiven");
	case 23625: return _T("RPC_ItemBag_CB_MarkMixStartTime");
	case 23390: return _T("RPC_ItemBag_CB_MakeItemMix");
	case 23157: return _T("RPC_ItemBag_CB_MixItem");
	case 23620: return _T("RPC_ItemBag_CB_IdentifyItem");
	case 23688: return _T("RPC_ItemBag_CB_WareHouse");
	case 23107: return _T("RPC_ItemBag_CB_BuyItemFromBusinessShop");
	case 23072: return _T("RPC_ItemBag_CB_SellItemToBusinessShop");
	case 32205: return _T("RPC_ScriptSystem_UP_STATE");
	case 32992: return _T("RPC_ScriptSystem_REQ_STATE");
	case 32131: return _T("RPC_ScriptSystem_ReplyAnswer");
	case 32357: return _T("RPC_ScriptSystem_ReplyAnswerString");
	case 32812: return _T("RPC_ScriptSystem_ReplyGiven");
	case 32052: return _T("RPC_ScriptSystem_StopTalk");
	case 32960: return _T("RPC_ScriptSystem_CB_TalkEnd");
	case 32391: return _T("RPC_ScriptSystem_CB_TalkNext");
	case 32095: return _T("RPC_ScriptSystem_CB_Choose");
	case 32216: return _T("RPC_ScriptSystem_CB_Say");
	case 32389: return _T("RPC_ScriptSystem_CB_OpenInterface");
	case 32505: return _T("RPC_ScriptSystem_CB_OpenShopMenu");
	case 32007: return _T("RPC_ScriptSystem_CB_ShowHint");
	case 32774: return _T("RPC_ScriptSystem_CB_OnPlaySound");
	case 32366: return _T("RPC_ScriptSystem_CB_AskGiven");
	case 32775: return _T("RPC_ScriptSystem_CB_GivenFinish");
	case 32988: return _T("RPC_ScriptSystem_CB_AskString");
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
	case 29546: return _T("RPC_PartnerGroup_UP_STATE");
	case 29435: return _T("RPC_PartnerGroup_REQ_STATE");
	case 29064: return _T("RPC_PartnerGroup_DeletePartner");
	case 29869: return _T("RPC_PartnerGroup_ChangePartnerState");
	case 29198: return _T("RPC_PartnerGroup_PutToWareHouse");
	case 29611: return _T("RPC_PartnerGroup_GetFromWareHouse");
	case 29101: return _T("RPC_PartnerGroup_PartnerCultivationUpByPartner");
	case 29644: return _T("RPC_PartnerGroup_PartnerCultivationUpByItem");
	case 29726: return _T("RPC_PartnerGroup_CB_DeleteListSuccess");
	case 29964: return _T("RPC_PartnerGroup_CB_AddPartnerGroup");
	case 29472: return _T("RPC_PartnerGroup_CB_ChangePartnerState");
	case 29396: return _T("RPC_PartnerGroup_CB_AddPartner");
	case 29000: return _T("RPC_PartnerGroup_CB_UpdateAllPartner");
	case 29292: return _T("RPC_PartnerGroup_CB_AddSkills");
	case 29269: return _T("RPC_PartnerGroup_CB_WareHouse");
	case 29625: return _T("RPC_PartnerGroup_CB_PartnerCultivationUp");
	case 34657: return _T("RPC_SkillGroup_UP_STATE");
	case 34935: return _T("RPC_SkillGroup_REQ_STATE");
	case 34843: return _T("RPC_SkillGroup_CB_AddSkills");
	case 34938: return _T("RPC_SkillGroup_CB_RemoveSkills");
	case 36335: return _T("RPC_QuestSystem_UP_STATE");
	case 36456: return _T("RPC_QuestSystem_REQ_STATE");
	case 36450: return _T("RPC_QuestSystem_CancelQuest");
	case 36813: return _T("RPC_QuestSystem_CB_QuestLogLst");
	case 36867: return _T("RPC_QuestSystem_CB_UpdateQuests");
	case 36431: return _T("RPC_QuestSystem_CB_AddQuestLogs");
	case 36717: return _T("RPC_QuestSystem_CB_RemoveQuestLogs");
	case 36815: return _T("RPC_QuestSystem_CB_AddQuestTypes");
	case 36457: return _T("RPC_QuestSystem_CB_RemoveQuestTypes");
	case 36209: return _T("RPC_QuestSystem_CB_AddQuestHints");
	case 36658: return _T("RPC_QuestSystem_CB_RemoveQuestHints");
	case 36122: return _T("RPC_QuestSystem_CB_AddJournals");
	case 36216: return _T("RPC_QuestSystem_CB_RemoveJournals");
	case 36725: return _T("RPC_QuestSystem_CB_QuestLogFlagChangeLst");
	case 36440: return _T("RPC_QuestSystem_CB_QuestLogFlagLoadLst");
	case 17567: return _T("RPC_Battle_UP_STATE");
	case 17935: return _T("RPC_Battle_REQ_STATE");
	case 17115: return _T("RPC_Battle_EndTurn");
	case 17063: return _T("RPC_Battle_NextTalk");
	case 17010: return _T("RPC_Battle_ChooseResult");
	case 17527: return _T("RPC_Battle_SendBattleMessage");
	case 17085: return _T("RPC_Battle_CB_Error");
	case 17566: return _T("RPC_Battle_CB_PlayTurn");
	case 17920: return _T("RPC_Battle_CB_PlayTurnMore");
	case 17270: return _T("RPC_Battle_CB_NextTurn");
	case 17556: return _T("RPC_Battle_CB_EndBattle");
	case 17389: return _T("RPC_Battle_CB_AddActors");
	case 17292: return _T("RPC_Battle_CB_AddPartners");
	case 17472: return _T("RPC_Battle_CB_ControlCharacter");
	case 17416: return _T("RPC_Battle_CB_ControlPartner");
	case 17904: return _T("RPC_Battle_CB_Reward");
	case 17324: return _T("RPC_Battle_CB_Penalty");
	case 17180: return _T("RPC_Battle_CB_NeedHelp");
	case 17338: return _T("RPC_Battle_CB_WaitTalk");
	case 17417: return _T("RPC_Battle_CB_Choose");
	case 17858: return _T("RPC_Battle_CB_Say");
	case 17833: return _T("RPC_Battle_CB_CancelTalk");
	case 17061: return _T("RPC_Battle_CB_DisableCmd");
	case 17665: return _T("RPC_Battle_CB_HideCmd");
	case 17900: return _T("RPC_Battle_CB_MartialResult");
	case 20458: return _T("RPC_BatActor_UP_STATE");
	case 20260: return _T("RPC_BatActor_REQ_STATE");
	case 20950: return _T("RPC_BatActor_SendCommand");
	case 20320: return _T("RPC_BatActor_CheatCode");
	case 20624: return _T("RPC_BatActor_CancelAuto");
	case 20737: return _T("RPC_BatActor_CB_Error");
	case 20712: return _T("RPC_BatActor_CB_CommandSuccess");
	case 20313: return _T("RPC_BatActor_CB_Inputed");
	case 20521: return _T("RPC_BatActor_CB_AutoCount");
	case 20522: return _T("RPC_BatActor_CB_BattleMsg");
	case 39393: return _T("RPC_Trading_UP_STATE");
	case 39246: return _T("RPC_Trading_REQ_STATE");
	case 39543: return _T("RPC_Trading_SetTradingItemSlot");
	case 39669: return _T("RPC_Trading_RemoveTradingItemSlot");
	case 39396: return _T("RPC_Trading_SetTradingPartnerSlot");
	case 39679: return _T("RPC_Trading_RemoveTradingPartnerSlot");
	case 39237: return _T("RPC_Trading_SetTradingPetSlot");
	case 39664: return _T("RPC_Trading_RemoveTradingPetSlot");
	case 39153: return _T("RPC_Trading_SetTradingLock");
	case 39452: return _T("RPC_Trading_SetTradingMoney");
	case 39991: return _T("RPC_Trading_CancelTrading");
	case 39706: return _T("RPC_Trading_ConfirmTrading");
	case 39696: return _T("RPC_Trading_CB_SetItemSlot");
	case 39951: return _T("RPC_Trading_CB_RemoveItemSlot");
	case 39613: return _T("RPC_Trading_CB_SetPartnerSlot");
	case 39832: return _T("RPC_Trading_CB_RemovePartnerSlot");
	case 39977: return _T("RPC_Trading_CB_SetPetSlot");
	case 39001: return _T("RPC_Trading_CB_RemovePetSlot");
	case 39792: return _T("RPC_Trading_CB_SetLock");
	case 39794: return _T("RPC_Trading_CB_SetMoney");
	case 39412: return _T("RPC_Trading_CB_CancelTrading");
	case 39164: return _T("RPC_Trading_CB_ConfirmTrading");
	case 39752: return _T("RPC_Trading_CB_TradingProcess");
	case 39521: return _T("RPC_Trading_CB_TradingItem");
	case 39344: return _T("RPC_Trading_CB_TradingPartner");
	case 39840: return _T("RPC_Trading_CB_TradingPet");
	case 39751: return _T("RPC_Trading_CB_TradingMoney");
	case 44602: return _T("RPC_Pet_UP_STATE");
	case 44405: return _T("RPC_Pet_REQ_STATE");
	case 44258: return _T("RPC_Pet_CB_RaiseAttrib");
	case 44643: return _T("RPC_Pet_ChangePetName");
	case 44728: return _T("RPC_Pet_CB_LearnSkillMessage");
	case 44188: return _T("RPC_Pet_CB_ChangePetName");
	case 44163: return _T("RPC_Pet_ChangePetExp");
	case 44182: return _T("RPC_Pet_ChangePetVp");
	case 44306: return _T("RPC_Pet_ChangePetFavor");
	case 45815: return _T("RPC_PetGroup_UP_STATE");
	case 45441: return _T("RPC_PetGroup_REQ_STATE");
	case 45485: return _T("RPC_PetGroup_DeletePet");
	case 45256: return _T("RPC_PetGroup_ChangePetState");
	case 45430: return _T("RPC_PetGroup_PutToWareHouse");
	case 45509: return _T("RPC_PetGroup_GetFromWareHouse");
	case 45351: return _T("RPC_PetGroup_CB_Error");
	case 45305: return _T("RPC_PetGroup_CB_AddSkills");
	case 45480: return _T("RPC_PetGroup_CB_AddPets");
	case 45559: return _T("RPC_PetGroup_CB_RemovePets");
	case 45020: return _T("RPC_PetGroup_CB_UpdateAllPets");
	case 45691: return _T("RPC_PetGroup_CB_WareHouse");
	case 47036: return _T("RPC_HawkGroup_UP_STATE");
	case 47384: return _T("RPC_HawkGroup_REQ_STATE");
	case 47430: return _T("RPC_HawkGroup_BuyHawkItem");
	case 47837: return _T("RPC_HawkGroup_CB_UpdateHawkItem");
	case 47642: return _T("RPC_HawkGroup_CB_BuyHawkItemSuccess");
	case 47119: return _T("RPC_HawkGroup_ChangeHawkName");
	case 47929: return _T("RPC_HawkGroup_CB_ChangeHawkName");
	case 47132: return _T("RPC_HawkGroup_CloseHawkForUser");
	case 47447: return _T("RPC_HawkGroup_CB_CloseHawkForUser");
	case 47286: return _T("RPC_HawkGroup_StopHawk");
	case 47974: return _T("RPC_HawkGroup_CB_StopHawk");
	case 47641: return _T("RPC_HawkGroup_CB_AddSellStartHawkItemInfo");
	case 53426: return _T("RPC_GiftBox_UP_STATE");
	case 53456: return _T("RPC_GiftBox_REQ_STATE");
	case 53179: return _T("RPC_GiftBox_TakeGiftBox");
	case 53920: return _T("RPC_GiftBox_CloseGiftBox");
	case 53043: return _T("RPC_GiftBox_CB_TakeGiftBox");
	case 53447: return _T("RPC_GiftBox_UpdateGiftBox");
	case 53678: return _T("RPC_GiftBox_CB_UpdateGiftBox");
	case 41255: return _T("RPC_ChargeShop_UP_STATE");
	case 41145: return _T("RPC_ChargeShop_REQ_STATE");
	case 41477: return _T("RPC_ChargeShop_BuyItemInChargeShop");
	case 41021: return _T("RPC_ChargeShop_CB_BuyItemInChargeShop");
	case 56693: return _T("RPC_Question_UP_STATE");
	case 56202: return _T("RPC_Question_REQ_STATE");
	case 56861: return _T("RPC_Question_CB_NextQuestion");
	case 56515: return _T("RPC_Question_Anwser");
	case 56437: return _T("RPC_Question_CB_AnswerResult");
	case 56220: return _T("RPC_Question_CB_StartCountDown");
	case 59777: return _T("RPC_RelationSystem_UP_STATE");
	case 59094: return _T("RPC_RelationSystem_REQ_STATE");
	case 59211: return _T("RPC_RelationSystem_CB_RequestSetSiblingTitlePrefix");
	case 59188: return _T("RPC_RelationSystem_SetSiblingTitlePrefix");
	case 59705: return _T("RPC_RelationSystem_CB_RequestSetSiblingTitleSuffix");
	case 59316: return _T("RPC_RelationSystem_SetSiblingTitleSuffix");
	case 59521: return _T("RPC_RelationSystem_CB_RequestSelectMarriageTime");
	case 59431: return _T("RPC_RelationSystem_SelectMarriageTime");
	}
	static Char buf[12];

	_stprintf(buf, _T("RPCID %d"), rpcId);
	return buf;
}
#endif // CLIENT

#pragma optimize("", on)

