//-- Common
#include "Common.h"
#include "Common/common_binary.h"

//-- Self
#include "ProxyManager.h"
//-- Local
#include "Configure.h"
#include "Global.h"
#include "Version.h"
#include "ApolloSvrLib.h"

using namespace Apollo;

#include "Resource/ResHackImage.h"

typedef StlMap<UInt16, UInt32, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, UInt32> > > RPCIDMap;
static RPCIDMap g_map_rpcid;
#define RPC_ENUM_MAP	g_map_rpcid
#define CP_GB2312		936

// can generate here
#include "..\RPCEnum\Account_enum.h"
#include "..\RPCEnum\Character_enum.h"
#include "..\RPCEnum\CharacterInfo_enum.h"
#include "..\RPCEnum\Map_enum.h"
#include "..\RPCEnum\Battle_enum.h"
#include "..\RPCEnum\BatActor_enum.h"
#include "..\RPCEnum\BatCharacter_enum.h"
#include "..\RPCEnum\BatMob_enum.h"
#include "..\RPCEnum\WorldService_enum.h"
#include "..\RPCEnum\MapService_enum.h"
#include "..\RPCEnum\MessageService_enum.h"
#include "..\RPCEnum\Channel_enum.h"
#include "..\RPCEnum\Party_enum.h"
#include "..\RPCEnum\MapActor_enum.h"
#include "..\RPCEnum\MapNPC_enum.h"
#include "..\RPCEnum\ItemBag_enum.h"
#include "..\RPCEnum\Item_enum.h"
#include "..\RPCEnum\ScriptSystem_enum.h"
#include "..\RPCEnum\Partner_enum.h"
#include "..\RPCEnum\PartnerGroup_enum.h"
#include "..\RPCEnum\Skill_enum.h"
#include "..\RPCEnum\SkillGroup_enum.h"
#include "..\RPCEnum\QuestLog_enum.h"
#include "..\RPCEnum\QuestType_enum.h"
#include "..\RPCEnum\QuestSystem_enum.h"
#include "..\RPCEnum\ServiceStatus_enum.h"
#include "..\RPCEnum\Trading_enum.h"
#include "..\RPCEnum\Messenger_enum.h"
#include "..\RPCEnum\MessageCharacter_enum.h"
#include "..\RPCEnum\ChargeShop_enum.h"
#include "..\RPCEnum\ChargeShopItem_enum.h"
#include "..\RPCEnum\RankCharacter_enum.h"
#include "..\RPCEnum\Pet_enum.h"
#include "..\RPCEnum\PetGroup_enum.h"
#include "..\RPCEnum\HawkGroup_enum.h"
#include "..\RPCEnum\HawkItem_enum.h"
#include "..\RPCEnum\Guild_enum.h"
#include "..\RPCEnum\GuildMember_enum.h"
#include "..\RPCEnum\GiftBox_enum.h"
#include "..\RPCEnum\Gift_enum.h"
#include "..\RPCEnum\LogService_enum.h"
#include "..\RPCEnum\Question_enum.h"
#include "..\RPCEnum\RelationSystem_enum.h"
#include "..\RPCEnum\SellYuanBao_enum.h"
#include "..\RPCEnum\SellYuanBaoGroup_enum.h"
// generate until here

Buffer* g_pBufKeyData = NULL;

ProxyManager::ProxyManager(VOID)
	: m_model(GLOBAL_NAME, GLOBAL_CONFIG, GLOBAL_LOGGER)
	, m_manage_interval(C_UINT32(0))
{
	// initialize AES secret key
	g_pBufKeyData = SafeCreateObject1(Buffer, KEY_DATA_SIZE);
	g_pBufKeyData->AddRef();
	g_pBufKeyData->Write(RPCHeader(RPC_KEY_DATA));
	Byte* pKeyData = g_pBufKeyData->GetData() + g_pBufKeyData->GetCursor();
	for (int i = 0; i < KEY_SIZE; ++i)
		pKeyData[i] = gGlobal.random.GetRandom(256);
	g_pBufKeyData->SetSize(KEY_DATA_SIZE);
	Reuben::Network::DefaultMiddle::UpdateCRC(g_pBufKeyData);
	Reuben::Math::AES::KeyExpansion(pKeyData, KEY_SIZE);

	m_model.SetPacketHandler(this);
	m_model.SetSocketHandler(this);
}

ProxyManager::~ProxyManager(VOID)
{
	SafeRelease(g_pBufKeyData);
}

Boolean ProxyManager::RegisterProxy(LPCPEER pPeer, RPCID rpcId)
{
	if (pPeer == NULL || !pPeer->IsAlive() || rpcId == INVALID_RPCID)
		return BOOLEAN_FALSE;

	Buffer* pBuf = SafeCreateObject(Buffer);
	RPCHeader header(rpcId);
	RPCContext context;

	pBuf->ResetCursor();
	pBuf->Write<RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(gConfigure.serviceId);

	MiddleFilter::UpdateCRC(pBuf);
	m_model.Send(pPeer, pBuf, BOOLEAN_FALSE);

	return BOOLEAN_TRUE;
}

VOID ProxyManager::ReleaseSession(LPCPEER pPeer, RPCID rpcId, Client *pClient)
{
	if (pPeer == NULL || !pPeer->IsAlive() || rpcId == INVALID_RPCID)
		return;

	Buffer* pBuf = SafeCreateObject(Buffer);
	RPCHeader header(rpcId);

	pBuf->ResetCursor();
	pBuf->Write<RPCHeader>(header);
	pBuf->Write<RPCContext>(pClient->GetContext());
	pBuf->Write<SessionID>(pClient->GetSessionID());
	pBuf->Write<Boolean>(BOOLEAN_FALSE);

	MiddleFilter::UpdateCRC(pBuf);
	m_model.Send(pPeer, pBuf, BOOLEAN_FALSE);
}

VOID ProxyManager::DisconnectClient(LPCPEER pPeer, RPCID rpcId, Client *pClient)
{
	if (pPeer == NULL || !pPeer->IsAlive() || rpcId == INVALID_RPCID)
		return;

	Buffer* pBuf = SafeCreateObject(Buffer);
	RPCHeader header(rpcId);

	pBuf->ResetCursor();
	pBuf->Write<RPCHeader>(header);
	pBuf->Write<RPCContext>(pClient->GetContext());

	MiddleFilter::UpdateCRC(pBuf);
	m_model.Send(pPeer, pBuf, BOOLEAN_FALSE);
}

VOID ProxyManager::UpdateStatus(LPCPEER pPeer, RPCContext &context, UInt32 status, UInt32 state)
{
	if (pPeer == NULL || !pPeer->IsAlive())
		return;

	Buffer* pBuf = SafeCreateObject(Buffer); 

	pBuf->ResetCursor();
	pBuf->Write<RPCHeader>(RPC_ServiceStatus_UpdateStatus);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(gConfigure.serviceId);
	pBuf->Write<UInt32>(status);
	pBuf->Write<UInt32>(state);

	MiddleFilter::UpdateCRC(pBuf);
	m_model.Send(pPeer, pBuf, BOOLEAN_FALSE);
}

CONST RPCID ProxyManager::GetRegisterProxyRPCID(CONST ServiceType type)
{
	switch (type)
	{
	case SERVICE_WORLD:
		return (RPC_WorldService_RegisterProxy);
	case SERVICE_MAP:
		return (RPC_MapService_RegisterProxy);
	case SERVICE_MESSAGE:
		return (RPC_MessageService_RegisterProxy);
	}
	return (INVALID_RPCID);
}

VOID ProxyManager::SendToClients(SessionIDVector& listeners, Buffer* pBuf, UInt32 secret)
{
	Count counter = C_COUNT(0);
	for (SessionIDVector::iterator it = listeners.begin(); it != listeners.end(); ++it)
	{
		SafeAutoPointer<CONST Peer, SafeReleaseProc<CONST Peer> > apPeerClient(m_model.GetPeer((*it).socketId));
		if (!apPeerClient)
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("cannot find out the peer, session_id %s, SocketID %d"), (*it).GetString().c_str(), (*it).socketId);
			continue;
		}
		Client* pClient = dynamic_cast<Client*>(apPeerClient->GetData());
		if (pClient == NULL)
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("peer without client data, SessionID=%s SocketID %d"), (*it).GetString().c_str(), (*it).socketId);
			continue;
		}
		else if (pClient->GetSessionID() != *it)
		{
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("peer not match session id, Send session_id %s, Peer session_id %s, SocketID %d"),
				(*it).GetString().c_str(), pClient->GetSessionID().GetString().c_str(), (*it).socketId);
			continue;
		}
		else
		{
			Buffer* pBuffer = SafeCreateObject(Buffer);
			pBuffer->SetData(pBuf->GetData(), pBuf->GetSize(), pBuf->GetCapacity(), BOOLEAN_FALSE);
			ThreadGuard guard(pClient->m_keySendOut);
			MiddleFilter::Compress(apPeerClient, pBuffer);
			MiddleFilter::Encrypt(pBuffer, secret);
			m_model.Send(apPeerClient, pBuffer, BOOLEAN_FALSE);
		}
		++counter;
		if (counter >= 100)
		{
			counter = 0;
			Sleep(1);
		}
	}
}

VOID ProxyManager::SendApolloMsg(SocketID socketId, PVOID pInfo)
{
	SafeAutoPointer<CONST Peer, SafeReleaseProc<CONST Peer> > apPeerClient(
		m_model.GetPeer(socketId));
	if (!apPeerClient)
	{
		//TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("peer without client data, SessionID=%s SocketID %d"), (*it).GetString().c_str(), socketId);
		return;
	}

	Client* pClient = dynamic_cast<Client*>(apPeerClient->GetData());
	if (pClient == NULL)
	{
		//TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("peer without client data, SessionID=%s SocketID %d"), (*it).GetString().c_str(), socketId);
	}
	else
	{
		PSEND_AUTH_PACKET_INFO pSendInfo = (PSEND_AUTH_PACKET_INFO) pInfo;
		if (pInfo == NULL)
			return;

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("socketId %d, msg size %d"), socketId, 
			pSendInfo->iPacketLen);

		ThreadGuard guard(pClient->m_keySendOut);
		RPCHeader header(RPC_WorldService_CB_ApolloMsg);
		UInt32 secret = Client::GetSecret(header.rpcId);
		Buffer* pBuf = SafeCreateObject(Buffer);

		pBuf->ResetCursor();
		pBuf->Write<RPCHeader>(header);
		pBuf->Write<RPCContext>(pClient->GetContext());
		// send byte array to client
		pBuf->Write<Size>(pSendInfo->iPacketLen);
		pBuf->WriteArray<Byte>(pSendInfo->pPacket, pSendInfo->iPacketLen);
		MiddleFilter::UpdateCRC(pBuf);
		MiddleFilter::Compress(apPeerClient, pBuf);
		MiddleFilter::Encrypt(pBuf, secret);
		m_model.Send(apPeerClient, pBuf, BOOLEAN_FALSE);
	}
}

VOID ProxyManager::RecvApolloMsg(Client* pClient, CONST Peer* pPeer, Buffer* pBuf)
{
	if (pClient == NULL || pPeer == NULL || pBuf == NULL)
		return;

	ByteArray msg;
	pBuf->Read<ByteArray>(msg);
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("RecvApolloMsg socketId %d, msg size %d"), 
		pPeer->GetSocketID(), msg.GetSize());
	SendDataToApolloSRV(pPeer->GetSocketID(), (char *)msg.GetData(), (int) msg.GetSize());
	//SafeDeleteObject(pBuf);
}

BOOL _stdcall ProxyManager::ApolloCallBack(DWORD dwIndex, PVOID pInfo, int iLen)
{
	BOOL ret=TRUE;
	//MSGHEAD msgHead;

	switch (dwIndex)
	{
	case APOLLO_SERVER_ERROR:
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("errcode %d"), 
				((PSERVER_ERROR_INFO)pInfo)->dwErrorCode);
		}
		break;

	case APOLLO_SERVER_KICK_GAME_CHARACTER:
		{
			SocketID socketId = ((PKICK_GAME_INFO)pInfo)->pGameInfo->GetOnlyID();
			String msg = ((PKICK_GAME_INFO)pInfo)->cpReasonInfo;
			
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Apollo Kick : socketId %d, reason: %s"),
				socketId, msg.c_str());
		}
		break;

	case APOLLO_SERVER_SEND_AUTH_PACKET:
		{
			SocketID socketId = ((PSEND_AUTH_PACKET_INFO)pInfo)->pGameInfo->GetOnlyID();
			SendApolloMsg(socketId, pInfo);

			//if trans not OK, return false to APOLLO.
		}
		break;

	case APOLLO_SERVER_LOG_DATABASE_INFO:
		{		
			SocketID socketId =  ((PLOG_DATABASE_INFO)pInfo)->pGameInfo->GetOnlyID();
			UInt32	msgTime = (UInt32) ((PLOG_DATABASE_INFO)pInfo)->tTime;
			UInt16	msgType = (UInt16) ((PLOG_DATABASE_INFO)pInfo)->dwType;
			//String msg = ((PLOG_DATABASE_INFO)pInfo)->cpLogInfo;
			//String typeName = ((PLOG_DATABASE_INFO)pInfo)->cpTypeName;
			Char	typeName[32];
			Char	msg[260];
			int		len1 = MultiByteToWideChar(CP_GB2312, 0, 
							((PLOG_DATABASE_INFO)pInfo)->cpTypeName, -1, typeName, 32);
			int		len2 = MultiByteToWideChar(CP_GB2312, 0, 
							((PLOG_DATABASE_INFO)pInfo)->cpLogInfo, -1, msg, 260);

			if (len1 >= 0 && len2 >= 0)
			{
				typeName[len1] = _T('0');
				msg[len2] = _T('0');
				TRACE_INFODTL_3(GLOBAL_LOGGER, 
					_T("APOLLO_SERVER_LOG_DATABASE_INFO: type %d, typeName %s, %s"), 
					msgType, typeName, msg);
				ProxyManager::StoreApolloLog(socketId, msgTime, msgType, typeName, msg);
			}
		
		}	
		return FALSE; //return false apollo help write DB true user write DB
	}
	return ret;
}

VOID ProxyManager::ProcessOutgoing(CONST Peer* pPeer, CONST SessionID& sid, Buffer* pBuf)
{
	RPCHeader header;
	pBuf->Read<RPCHeader>(header);
	// ignore all outgoing RPC message rpc id < 10
	if (header.rpcId < 10)
		return;

	pBuf->ShiftCursor(RPCContext::SIZE);

	switch (header.rpcId)
	{
	case RPC_MapService_CB_ReceiveMapMessage:
		{
			String strMsg;
			pBuf->Read<String>(strMsg);
			String strSrcName;
			pBuf->Read<String>(strSrcName);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);

			pBuf->ResetCursor();
			pBuf->ShiftCursor(RPCHeader::SIZE + RPCContext::SIZE);
			pBuf->Write<String>(strMsg);
			pBuf->Write<String>(strSrcName);
			pBuf->Write<Size>(0);
			pBuf->SetSize(pBuf->GetCursor());
			MiddleFilter::UpdateCRC(pBuf);

			SendToClients(listeners, pBuf, Client::GetSecret(header.rpcId));
		}
		break;
	case RPC_Channel_CB_ReceiveMessage:
		{
			RPCNetID netid;
			pBuf->Read<RPCNetID>(netid);
			String strMsg;
			pBuf->Read<String>(strMsg);
			String strSrcName;
			pBuf->Read<String>(strSrcName);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);

			pBuf->ResetCursor();
			pBuf->ShiftCursor(RPCHeader::SIZE + RPCContext::SIZE);
			pBuf->Write<RPCNetID>(netid);
			pBuf->Write<String>(strMsg);
			pBuf->Write<String>(strSrcName);
			pBuf->Write<Size>(0);
			pBuf->SetSize(pBuf->GetCursor());
			MiddleFilter::UpdateCRC(pBuf);

			SendToClients(listeners, pBuf, Client::GetSecret(header.rpcId));
		}
		break;
	case RPC_MessageService_CB_ReceiveHorseLightMessage:
		{
			String strMsg;
			pBuf->Read<String>(strMsg);
			UInt32 count;
			pBuf->Read<UInt32>(count);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);

			pBuf->ResetCursor();
			pBuf->ShiftCursor(RPCHeader::SIZE + RPCContext::SIZE);
			pBuf->Write<String>(strMsg);
			pBuf->Write<UInt32>(count);
			pBuf->Write<Size>(0);
			pBuf->SetSize(pBuf->GetCursor());
			MiddleFilter::UpdateCRC(pBuf);

			SendToClients(listeners, pBuf, Client::GetSecret(header.rpcId));
		}
		break;
	case RPC_MessageService_CB_ReceiveRumorMessage:
		{
			String strMsg;
			pBuf->Read<String>(strMsg);
			BYTE SubType;
			pBuf->Read<BYTE>(SubType);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);

			pBuf->ResetCursor();
			pBuf->ShiftCursor(RPCHeader::SIZE + RPCContext::SIZE);
			pBuf->Write<String>(strMsg);
			pBuf->Write<BYTE>(SubType);
			pBuf->Write<Size>(0);
			pBuf->SetSize(pBuf->GetCursor());
			MiddleFilter::UpdateCRC(pBuf);

			SendToClients(listeners, pBuf, Client::GetSecret(header.rpcId));
		}
		break;
	case RPC_MessageService_CB_ReceiveSystemMessage:
		{
			String strMsg;
			pBuf->Read<String>(strMsg);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);

			pBuf->ResetCursor();
			pBuf->ShiftCursor(RPCHeader::SIZE + RPCContext::SIZE);
			pBuf->Write<String>(strMsg);
			pBuf->Write<Size>(0);
			pBuf->SetSize(pBuf->GetCursor());
			MiddleFilter::UpdateCRC(pBuf);

			SendToClients(listeners, pBuf, Client::GetSecret(header.rpcId));
		}
		break;
	case RPC_MessageService_CB_CreateGuildMsg:
		{
			String guildName;
			pBuf->Read<String>(guildName);
			String creator;
			pBuf->Read<String>(creator);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);

			pBuf->ResetCursor();
			pBuf->ShiftCursor(RPCHeader::SIZE + RPCContext::SIZE);
			pBuf->Write<String>(guildName);
			pBuf->Write<String>(creator);
			pBuf->Write<Size>(0);
			pBuf->SetSize(pBuf->GetCursor());
			MiddleFilter::UpdateCRC(pBuf);

			SendToClients(listeners, pBuf, Client::GetSecret(header.rpcId));
		}
		break;
	case RPC_WorldService_CB_RankChangeMsg:
		{
			String strMsg;
			String title;
			String charName;
			UInt16 preRank;
			UInt16 curRank;
			SessionIDVector listeners;

			pBuf->Read<String>(title);
			pBuf->Read<String>(charName);
			pBuf->Read<UInt16>(preRank);
			pBuf->Read<UInt16>(curRank);
			pBuf->Read<SessionIDVector>(listeners);

			pBuf->ResetCursor();
			pBuf->ShiftCursor(RPCHeader::SIZE + RPCContext::SIZE);
			pBuf->Write<String>(title);
			pBuf->Write<String>(charName);
			pBuf->Write<UInt16>(preRank);
			pBuf->Write<UInt16>(curRank);
			pBuf->Write<Size>(0);
			pBuf->SetSize(pBuf->GetCursor());
			MiddleFilter::UpdateCRC(pBuf);

			SendToClients(listeners, pBuf, Client::GetSecret(header.rpcId));
		}
		break;
	case RPC_ServiceStatus_CB_CheckService:
		UpdateStatus(pPeer, RPCContext(), SS_RUNNING, SRS_STARTED);
		break;
	case RPC_ServiceStatus_CB_StopService:
		gGlobal.bExit = BOOLEAN_TRUE;
		break;
	default:
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("invalid rpc_id, session_id %s, rpc_id %d"), sid.GetString().c_str(), header.rpcId);
	}
}

VOID ProxyManager::OnUpdate(CONST UInt32 uDeltaMilliseconds)
{
	m_model.Update();

	if (m_manage_interval >= gConfigure.manage_interval)
	{
		UpdateStatus(gGlobal.pManagerPeer, RPCContext(), SS_CONNECTION_COUNT, gGlobal.pWanListener->GetConnectionCount());
		DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());
		m_manage_interval = C_UINT32(0);

	}
	m_manage_interval += uDeltaMilliseconds;
}

VOID ProxyManager::OnAccept(CONST Listener* listener, CONST Peer* pPeer)
{
	TRACE_ENSURE(listener);

	if (pPeer->IsTrust())
	{
		// internal connection from trustable listener for sure is manager service
		TRACE_ENSURE(gGlobal.pManagerPeer == NULL);
		gGlobal.pManagerPeer = pPeer->AddRef();
		UpdateStatus(pPeer, RPCContext(), SS_CONNECTION_COUNT, gGlobal.pWanListener->GetConnectionCount());
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Manager Service Connected, SocketID %d"), pPeer->GetSocketID());
	} 
	else
	{
		// client connection from untrustable listener for sure is client
		Client* pClient = SafeCreateObject(Client);
		RPCContext context;
		context.sessionId.serviceId = gConfigure.serviceId;
		context.sessionId.socketId = pPeer->GetSocketID();
		context.sessionId.ticketId = ::timeGetTime();
		pClient->SetContext(context);
		pClient->SetPeer(pPeer);
		pClient->SetWorldPeer(gGlobal.pWorldService->pPeer);
		pClient->SetMessagePeer(gGlobal.pMessageService->pPeer);
		pPeer->SetData(pClient);

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Client Connected, session_id %s, SocketID %d, Client Pointer 0x%p"), 
			context.sessionId.GetString().c_str(), pPeer->GetSocketID(), pClient);
	}
}

VOID ProxyManager::OnFull(CONST Listener* listener)
{
}

VOID ProxyManager::OnOpen(CONST Listener* listener)
{
}

VOID ProxyManager::OnOpen(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID ProxyManager::OnClose(CONST Listener* listener)
{
}

VOID ProxyManager::OnClose(CONST Listener* listener, CONST Peer* pPeer)
{
	Client* pClient = dynamic_cast<Client*>(pPeer->GetData());

	if (pClient)
	{
		// release session after client disconnected
		DisconnectClient(gGlobal.pMessageService->pPeer, RPC_MessageService_ClientDisconnect, pClient);
		ReleaseSession(gGlobal.pMessageService->pPeer, RPC_MessageService_ReleaseSession, pClient);
		// try to release session in World Service from Map Service
		DisconnectClient(gGlobal.pWorldService->pPeer, RPC_WorldService_ClientDisconnect, pClient);
		ReleaseSession(gGlobal.pWorldService->pPeer, RPC_WorldService_ReleaseSession, pClient);
		if (pClient->comp && pClient->decomp)
		{
			TRACE_INFODTL_5(GLOBAL_LOGGER, _F("Client Left, session_id %s, SocketID %d, Client Pointer 0x%p, Compression Ratio (out %.3f / in %.3f)"), 
				pClient->GetSessionID().GetString().c_str(), pPeer->GetSocketID(), pClient, pClient->comp->GetAvgRatio(), pClient->decomp->GetAvgRatio());
		}
		pClient->SetPeer(NULL);
		pClient->SetWorldPeer(NULL);
		pClient->SetMapPeer(NULL);
		pClient->SetMessagePeer(NULL);
		if (gConfigure.apolloInteval != 0)
		{
			// unregister client from apollo if enable
			DelApolloAutoRequestUserSRV(pPeer->GetSocketID());
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Unregister Apollo: socketId %d"), 
				pPeer->GetSocketID());
		}
	}
	else if (gGlobal.pManagerPeer == pPeer)
		SafeRelease(gGlobal.pManagerPeer);
	else
	{
		TryReconnect(pPeer);
	}
}

Boolean ProxyManager::TryReconnect(CONST Peer* pPeer)
{
	Boolean bConnect = BOOLEAN_FALSE;
	ServiceMap::iterator it = gConfigure.services.find(pPeer->GetSocketID());

	if (it != gConfigure.services.end())
	{
		Service& service = it->second;
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("%s disconnected, socketId %d"), 
			service.strName.c_str(), pPeer->GetSocketID());
		// release peer and set disconnect first
		SafeRelease(service.pPeer);
		service.bConnected = BOOLEAN_FALSE;
		if (!gGlobal.bExit)
		{
			// reconnect service
			service.pPeer = GetModel().Connect(service.address, service.protocol, service.bTrust);
			if (service.pPeer)
			{
				service.bConnected = BOOLEAN_TRUE;
				bConnect = BOOLEAN_TRUE;
				TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("%s reconnect successfully, socketId %d"), 
					service.strName.c_str(), service.pPeer->GetSocketID());
			}
			else
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("%s reconnect failed"), 
					service.strName.c_str());
			}
		}
	}

	return bConnect;
}

VOID ProxyManager::OnReceived(CONST Peer* pPeer, CONST Address* pFrom, Buffer* pBuf)
{
	Client* pClient = dynamic_cast<Client*>(pPeer->GetData());
	if (pClient)	// Incoming
	{
		try
		{
			pClient->ProcessIncoming(pPeer, pBuf, m_model);
		}
		catch(...)
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("catch exception, session_id %s, ip %s"),
				pClient->GetSessionID().GetString().c_str(), Reuben::Network::IPToString(pPeer->GetAddressRemote().GetIP()).c_str());
			m_model.CloseSocket(pPeer->GetSocketID());
			TRACE_ERRORDTL_1(REUBEN_LOGGER, _FE("exception occus at %s"), GLOBAL_NAME);
		}
	}
	else			// Outgoing
	{
		if (!DefaultMiddle::IsValidCRC(pBuf))
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("Outgoing CRC Failed, SocketID %d, Buffer %p"), pPeer->GetSocketID(), pBuf);
			// should dump buffer here
			goto LABEL_EXIT;
		}

		RPCHeader header;
		SessionID sessionId;
		pBuf->Read<RPCHeader>(header);
		pBuf->Read<SessionID>(sessionId);

		if (sessionId.serviceId != gConfigure.serviceId || sessionId.socketId == INVALID_SOCKET_ID || sessionId.ticketId == 0)
		{
			pBuf->ResetCursor();
			ProcessOutgoing(pPeer, sessionId, pBuf);
		}
		else
		{
			SafeAutoPointer<CONST Peer, SafeReleaseProc<CONST Peer> > apPeerClient(m_model.GetPeer(sessionId.socketId));
			if (!apPeerClient)
			{
				if (header.rpcId == RPC_MapService_CB_SyncProxy)
				{
					header.rpcId = RPC_MapService_SyncProxy;
					pBuf->ResetCursor();
					pBuf->Write<RPCHeader>(header);
					DefaultMiddle::UpdateCRC(pBuf);
					m_model.Send(pPeer, pBuf, BOOLEAN_FALSE);
				}
				else
				{
					TRACE_INFODTL_2(GLOBAL_LOGGER, 
						_F("cannot find the client peer, session_id %s, SocketID %d"),
						sessionId.GetString().c_str(), sessionId.socketId);
				}
				goto LABEL_EXIT;
			}
			else
			{
				pClient = dynamic_cast<Client*>(apPeerClient->GetData());
				if (pClient == NULL)
				{
					TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("no client data, session_id %s"), sessionId.GetString().c_str());
					goto LABEL_EXIT;
				}
				else if (pClient->GetSessionID() != sessionId)
				{
					TRACE_INFODTL_2(GLOBAL_LOGGER, _F("not match session id, Target session_id %s, Client session_id %s"),
						sessionId.GetString().c_str(), pClient->GetSessionID().GetString().c_str());
					goto LABEL_EXIT;
				}
			}

			pBuf->ResetCursor();
			pClient->ProcessOutgoing(pPeer, pBuf, m_model);
		}
	}

LABEL_EXIT:

	SafeRelease(pBuf);
}

VOID ProxyManager::OnSent(CONST Peer* pPeer, CONST Address* pTo, Buffer* pBuffer)
{
	Client* pClient = dynamic_cast<Client*>(pPeer->GetData());
	if (pClient)
		::InterlockedDecrement(&pClient->m_cOutgoing);
}

INLINE Boolean UnpackBuffer(CONST Peer* pPeer, Buffer* pBuf, CONST UInt32 secret)
{
	if (!MiddleFilter::Decrypt(pBuf, secret))
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("decrypt failed, socket_id %d, ip %s, buffer 0x%p, buffer size %d"),
			pPeer->GetSocketID(), Reuben::Network::IPToString(pPeer->GetAddressRemote().GetIP()).c_str(), pBuf, pBuf->GetSize());
		return (BOOLEAN_FALSE);
	}

	if (!MiddleFilter::Decompress(pPeer, pBuf))
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("decompress failed, socket_id %d, ip %s, buffer 0x%p, buffer size %d"),
			pPeer->GetSocketID(), Reuben::Network::IPToString(pPeer->GetAddressRemote().GetIP()).c_str(), pBuf, pBuf->GetSize());
		return (BOOLEAN_FALSE);
	}

	if (!MiddleFilter::IsValidCRC(pBuf))
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("crc failed, socket_id %d, ip %s, buffer 0x%p, buffer size %d"),
			pPeer->GetSocketID(), Reuben::Network::IPToString(pPeer->GetAddressRemote().GetIP()).c_str(), pBuf, pBuf->GetSize());
		return (BOOLEAN_FALSE);
	}

	return (BOOLEAN_TRUE);
}

CONST UInt32 ProxyManager::Client::GetSecret(CONST RPCID rpcid)
{
	RPCIDMap::iterator itfind = RPC_ENUM_MAP.find(rpcid);
	if (itfind == RPC_ENUM_MAP.end())
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("rpc_id not found, rpc_id %d"), rpcid);
		return (0);
	}

	return (itfind->second);
}

VOID ProxyManager::Client::VersionCheckingState::OnIncoming(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model)
{
	UInt32 client_version;
	pBuf->Read<UInt32>(client_version);
	if (client_version != VERSION_NUMBER) // configure able version
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("version not match, client version %d, server version %d"),
			client_version, VERSION_NUMBER);
		model.CloseSocket(pPeer->GetSocketID());
		return;
	}

	model.Send(pPeer, g_pBufKeyData, BOOLEAN_FALSE);

	if (gConfigure.apolloInteval != 0)
	{
		// register client to apollo if enable
		CGameInfo GameInfo;
		GameInfo.SetOnlyID(pPeer->GetSocketID());
		AddApolloAutoRequestUserSRV(&GameInfo);
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Register Apollo: socketId %d"), pPeer->GetSocketID());
	}

	// switch to login state
	pClient->SwitchState<LoginState>();
}

VOID ProxyManager::Client::VersionCheckingState::OnOutgoing(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model)
{
	TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("should not send anything out, session_id %s"),
		pClient->GetSessionID().GetString().c_str());
} 

VOID ProxyManager::Client::LoginState::OnIncoming(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model)
{
	RPCHeader header;
	pBuf->Read<RPCHeader>(header);
	UInt32 secret = Client::GetSecret(header.rpcId);
	if (secret == 0)
	{
		TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("invalid rpc_id, session_id %s, socket_id %d, rpc_id %d, ip %s"),
			pClient->GetSessionID().GetString().c_str(), pPeer->GetSocketID(), header.rpcId, Reuben::Network::IPToString(pPeer->GetAddressRemote().GetIP()).c_str());
		model.CloseSocket(pPeer->GetSocketID());
		return;
	}

	if (!UnpackBuffer(pPeer, pBuf, secret))
	{
		TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("unpack buffer failed, session_id %s, socket_id %d, rpc_id %d, ip %s"),
			pClient->GetSessionID().GetString().c_str(), pPeer->GetSocketID(), header.rpcId, Reuben::Network::IPToString(pPeer->GetAddressRemote().GetIP()).c_str());
		model.CloseSocket(pPeer->GetSocketID());
		return;
	}

	pBuf->Write<SessionID>(pClient->GetSessionID());
	pBuf->ShiftCursor(ProcedureID::SIZE);

	CONST Peer* pServicePeer = NULL;
	if (header.rpcId == RPC_WorldService_ApolloMsg)
	{
		RecvApolloMsg(pClient, pPeer, pBuf);
		return;
	}
	else if (header.rpcId == RPC_WorldService_LoginAccount && pClient->m_idxAntiHack == INVALID_ANTIHACK_INDEX)
	{
		pServicePeer = pClient->m_pWorldPeer;
		pBuf->Write<UInt32>(pPeer->GetAddressRemote().GetIP());
	}
	else if (header.rpcId == RPC_WorldService_AnswerAntiHack && pClient->m_idxAntiHack != INVALID_ANTIHACK_INDEX)
	{
		pServicePeer = pClient->m_pWorldPeer;
		UInt8 playerAnswer;
		pBuf->Read<UInt8>(playerAnswer);
		if (pClient->m_idxAntiHack != playerAnswer)
		{
			TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("wrong answer, SessionID %s, RPCID %d, AntiHackIndex %d"),
				pClient->GetSessionID().GetString().c_str(), header.rpcId, pClient->m_idxAntiHack);
			model.CloseSocket(pPeer->GetSocketID());
			return;
		}
		pClient->m_preWrongHackStyle = pClient->m_preWrongHackStyle == 0? 1 : 0;	
		pClient->m_idxAntiHack = INVALID_ANTIHACK_INDEX;
	}
	else
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("invalid rpc_id, session_id %s, rpc_id %d, AntiHackIndex %d"),
			pClient->GetSessionID().GetString().c_str(), header.rpcId, pClient->m_idxAntiHack);
		model.CloseSocket(pPeer->GetSocketID());
		return;
	}

	DefaultMiddle::UpdateCRC(pBuf);
	ThreadGuard guard(pClient->m_keySendIn);
	model.Send(pServicePeer, pBuf, BOOLEAN_FALSE);
}

VOID ProxyManager::Client::LoginState::OnOutgoing(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model)
{
	RPCHeader header;
	pBuf->Read<RPCHeader>(header);
	// ignore all outgoing RPC message rpc id < 10
	if (header.rpcId < 10)
		return;

	UInt32 secret = Client::GetSecret(header.rpcId);
	if (secret == 0)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("invalid rpc_id, session_id %s, rpc_id %d"),
			pClient->GetSessionID().GetString().c_str(), header.rpcId);
		model.CloseSocket(pClient->m_pPeer->GetSocketID());
		return;
	}

	pBuf->Write<SessionID>(SessionID());
	pBuf->ShiftCursor(ProcedureID::SIZE);

	Boolean bDisconnect = BOOLEAN_FALSE;
	switch (header.rpcId)
	{
	case RPC_WorldService_CB_AntiHack:
		{
			if (gGlobal.resource.GetHackInfoSize() < 5)
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("not enought anti hack data, session_id %s, rpc_id %d"),
					pClient->GetSessionID().GetString().c_str(), header.rpcId);
				model.CloseSocket(pClient->m_pPeer->GetSocketID());
				return;
			}
			AntiHackData data;
			Index i = C_INDEX(0);
			pClient->m_idxAntiHack = gGlobal.random.GetRandom(5);	
//			if (pClient->m_preWrongHackStyle)
//			{
//				while (i < 5)
//				{
//LABEL_NEXT_2:
//					Char rand = gGlobal.random.GetRandom(gGlobal.resource.GetHackInfoSize());
//					for (Index j = 0; j < i; ++j)
//					{
//						if ((Char)0x4E00 + rand == data.chars[j])
//							goto LABEL_NEXT_2;
//					}
//					data.chars[i] = (Char)0x4E00 + rand;
//					++i;
//				}
//				Byte* pMapView = NULL;
//				Byte* pImage = NULL;
//				UInt32 size = C_UINT32(0);
//				gGlobal.resource.GetAntiHackInfo(data.chars[pClient->m_idxAntiHack]-0x4E00, pMapView, pImage, size);
//				if (!pMapView || !pImage)
//				{
//					TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("get anti hack failed, SessionID %s, RPCID %d, AntiHackIndex %d"),
//						pClient->GetSessionID().GetString().c_str(), header.rpcId, pClient->m_idxAntiHack);
//					model.CloseSocket(pClient->m_pPeer->GetSocketID());
//					return;
//				}
//				// Write Byte Array
//				pBuf->Write<Size>((Size)size);
//				pBuf->WriteArray<Byte>(pImage, (CONST Count)size);
//				gGlobal.resource.ReleaseHackImagePtr(pMapView);
//				if (gConfigure.giveHackAnswer)
//					data.answer = pClient->m_idxAntiHack;
//			}
//			else
//			{ 
				pBuf->Write<Size>(C_SIZE(0));
				data.answer = pClient->m_idxAntiHack;	
//			}
			pBuf->Write<AntiHackData>(data);
		}
		break;
	case RPC_WorldService_CB_ForceLogout:
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("logout, session_id %s, rpc_id %d"),
			pClient->GetSessionID().GetString().c_str(), header.rpcId);
		bDisconnect = BOOLEAN_TRUE;
		break;
	case RPC_WorldService_CB_LoginResult:
		UInt8 result;
		pBuf->Read<UInt8>(result);
		if (result == LOGIN_SUCCESSED || result == LOGIN_SUCCESSED_NEED_KICK)
			pClient->SwitchState<IngameState>();
		break;
	}

	MiddleFilter::UpdateCRC(pBuf);
	// send out to client
	{
		ThreadGuard guard(pClient->m_keySendOut);
		MiddleFilter::Compress(pClient->m_pPeer, pBuf);
		MiddleFilter::Encrypt(pBuf, secret);
		model.Send(pClient->m_pPeer, pBuf, BOOLEAN_FALSE);
	}
	if (bDisconnect)
	{
		Sleep(1);
		if (pClient->m_pPeer)
			model.CloseSocket(pClient->m_pPeer->GetSocketID());
	}
}

VOID ProxyManager::Client::IngameState::OnIncoming(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model)
{
	RPCHeader header;
	pBuf->Read<RPCHeader>(header);
	UInt32 secret = Client::GetSecret(header.rpcId);
	if (secret == 0)
	{
		TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("invalid rpc_id, session_id %s, socket_id %d, rpc_id %d, ip %s"),
			pClient->GetSessionID().GetString().c_str(), pPeer->GetSocketID(), header.rpcId, Reuben::Network::IPToString(pPeer->GetAddressRemote().GetIP()).c_str());
		model.CloseSocket(pPeer->GetSocketID());
		return;
	}

	if (!UnpackBuffer(pPeer, pBuf, secret))
	{
		TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("unpack buffer failed, session_id %s, socket_id %d, rpc_id %d, ip %s"),
			pClient->GetSessionID().GetString().c_str(), pPeer->GetSocketID(), header.rpcId, Reuben::Network::IPToString(pPeer->GetAddressRemote().GetIP()).c_str());
		model.CloseSocket(pPeer->GetSocketID());
		return;
	}

	if (pClient->m_idxAntiHack != INVALID_ANTIHACK_INDEX && header.rpcId != RPC_Character_AnswerAntiHack)
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("antihack checking, session_id %s, rpc_id %d, AntiHackIndex %d"),
			pClient->GetSessionID().GetString().c_str(), header.rpcId, pClient->m_idxAntiHack);
		return;
	}

	pBuf->Write<SessionID>(pClient->GetSessionID());
	pBuf->ShiftCursor(ProcedureID::SIZE);

	if (header.rpcId == RPC_WorldService_ApolloMsg)
	{
		RecvApolloMsg(pClient, pPeer, pBuf);
		return;
	}

	RPCID rpcid = header.rpcId;
	CONST Peer* pServicePeer = NULL;

	if (rpcid >= RPC_MapService_ENUM_ST && rpcid <= RPC_MapService_ENUM_ED ||
		rpcid >= RPC_Map_ENUM_ST && rpcid <= RPC_Map_ENUM_ED ||
		rpcid >= RPC_MapActor_ENUM_ST && rpcid <= RPC_MapActor_ENUM_ED ||
		rpcid >= RPC_MapNPC_ENUM_ST && rpcid <= RPC_MapNPC_ENUM_ED ||
		rpcid >= RPC_ItemBag_ENUM_ST && rpcid <= RPC_ItemBag_ENUM_ED ||
		rpcid >= RPC_Party_ENUM_ST && rpcid <= RPC_Party_ENUM_ED ||
		rpcid >= RPC_ScriptSystem_ENUM_ST && rpcid <= RPC_ScriptSystem_ENUM_ED ||
		rpcid >= RPC_PartnerGroup_ENUM_ST && rpcid <= RPC_PartnerGroup_ENUM_ED ||
		rpcid >= RPC_Partner_ENUM_ST && rpcid <= RPC_Partner_ENUM_ED ||
		rpcid >= RPC_Skill_ENUM_ST && rpcid <= RPC_Skill_ENUM_ED ||
		rpcid >= RPC_Battle_ENUM_ST && rpcid <= RPC_Battle_ENUM_ED ||
		rpcid >= RPC_BatActor_ENUM_ST && rpcid <= RPC_BatActor_ENUM_ED ||
		rpcid >= RPC_BatCharacter_ENUM_ST && rpcid <= RPC_BatCharacter_ENUM_ED ||
		rpcid >= RPC_BatMob_ENUM_ST && rpcid <= RPC_BatMob_ENUM_ED ||
		rpcid >= RPC_QuestLog_ENUM_ST && rpcid <= RPC_QuestLog_ENUM_ED ||
		rpcid >= RPC_QuestType_ENUM_ST && rpcid <= RPC_QuestType_ENUM_ED ||
		rpcid >= RPC_QuestSystem_ENUM_ST && rpcid <= RPC_QuestSystem_ENUM_ED ||
		rpcid >= RPC_Trading_ENUM_ST && rpcid <= RPC_Trading_ENUM_ED ||
		rpcid >= RPC_ChargeShop_ENUM_ST && rpcid <= RPC_ChargeShop_ENUM_ED ||
		rpcid >= RPC_ChargeShopItem_ENUM_ST && rpcid <= RPC_ChargeShopItem_ENUM_ED ||
		rpcid >= RPC_PetGroup_ENUM_ST && rpcid <= RPC_PetGroup_ENUM_ED ||
		rpcid >= RPC_Pet_ENUM_ST && rpcid <= RPC_Pet_ENUM_ED ||
		rpcid >= RPC_HawkItem_ENUM_ST && rpcid <= RPC_HawkItem_ENUM_ED ||
		rpcid >= RPC_HawkGroup_ENUM_ST && rpcid <= RPC_HawkGroup_ENUM_ED ||
		rpcid >= RPC_GiftBox_ENUM_ST && rpcid <= RPC_GiftBox_ENUM_ED ||
		rpcid >= RPC_Gift_ENUM_ST && rpcid <= RPC_Gift_ENUM_ED ||
		rpcid >= RPC_Question_ENUM_ST && rpcid <= RPC_Question_ENUM_ED||
		rpcid >= RPC_RelationSystem_ENUM_ST && rpcid <= RPC_RelationSystem_ENUM_ED )
	{
		if (pClient->m_pMapPeer)
			pServicePeer = pClient->m_pMapPeer;
		else
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("no map peer, session_id %s, rpc_id %d"), pClient->GetSessionID().GetString().c_str(), rpcid);
	}
	else if (rpcid >= RPC_Account_ENUM_ST && rpcid <= RPC_Account_ENUM_ED ||
		rpcid >= RPC_RankCharacter_ENUM_ST && rpcid <= RPC_RankCharacter_ENUM_ED ||
		rpcid >= RPC_CharacterInfo_ENUM_ST && rpcid <= RPC_CharacterInfo_ENUM_ED ||
		rpcid >= RPC_SellYuanBao_ENUM_ST && rpcid <= RPC_SellYuanBao_ENUM_ED ||
		rpcid >= RPC_SellYuanBaoGroup_ENUM_ST && rpcid <= RPC_SellYuanBaoGroup_ENUM_ED)
	{
		if (pClient->m_pWorldPeer)
			pServicePeer = pClient->m_pWorldPeer;
		else
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("no world peer, session_id %s, rpc_id %d"), pClient->GetSessionID().GetString().c_str(), rpcid);
	}
	else if (rpcid >= RPC_MessageService_ENUM_ST && rpcid <= RPC_MessageService_ENUM_ED ||
		rpcid >= RPC_Channel_ENUM_ST && rpcid <= RPC_Channel_ENUM_ED ||
		rpcid >= RPC_Messenger_ENUM_ST && rpcid <= RPC_Messenger_ENUM_ED ||
		rpcid >= RPC_MessageCharacter_ENUM_ST && rpcid <= RPC_MessageCharacter_ENUM_ED ||
		rpcid >= RPC_Guild_ENUM_ST && rpcid <= RPC_Guild_ENUM_ED ||
		rpcid >= RPC_GuildMember_ENUM_ST && rpcid <= RPC_GuildMember_ENUM_ED)
	{
		if (pClient->m_pMessagePeer)
			pServicePeer = pClient->m_pMessagePeer;
		else
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("no message peer, session_id %s, rpc_id %d"), pClient->GetSessionID().GetString().c_str(), rpcid);
	}
	else if (rpcid >= RPC_WorldService_ENUM_ST && rpcid <= RPC_WorldService_ENUM_ED)
	{
		if (rpcid == RPC_WorldService_LoginAccount || rpcid == RPC_WorldService_AnswerAntiHack)
		{
			TRACE_ERRORDT_2(GLOBAL_LOGGER, _FE("invalid rpc_id, session_id %s, rpc_id %d"),
				pClient->GetSessionID().GetString().c_str(), rpcid);
			model.CloseSocket(pPeer->GetSocketID());
			return;
		}
		if (pClient->m_pWorldPeer)
			pServicePeer = pClient->m_pWorldPeer;
		else
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("no world peer, session_id %s, rpc_id %d"), pClient->GetSessionID().GetString().c_str(), rpcid);
	}
	else if (rpcid >= RPC_Character_ENUM_ST && rpcid <= RPC_Character_ENUM_ED)
	{
		if (pClient->m_pMapPeer)
		{
			pServicePeer = pClient->m_pMapPeer;
			if (rpcid == RPC_Character_AnswerAntiHack)
			{
				pBuf->ShiftCursor(RPCNetID::SIZE);
				UInt8 playerAnswer;
				pBuf->Read<UInt8>(playerAnswer);				
				Boolean is_right = BOOLEAN_TRUE;
				if (pClient->m_idxAntiHack != playerAnswer)
				{
					TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("wrong answer, session_id %s, rpc_id %d, AntiHackIndex %d"),
						pClient->GetSessionID().GetString().c_str(), header.rpcId, pClient->m_idxAntiHack);
					is_right = BOOLEAN_FALSE;
				}
				if (is_right)
					pClient->m_preWrongHackStyle = pClient->m_preWrongHackStyle == 0? 1 : 0;		
				pBuf->Write<Boolean>(is_right);
			}
		}
		else
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("no map peer, session_id %s, rpc_id %d"), pClient->GetSessionID().GetString().c_str(), rpcid);
	}
	else
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("invalid rpc_id, session_id %s, rpc_id %d, AntiHackIndex %d"),
			pClient->GetSessionID().GetString().c_str(), rpcid, pClient->m_idxAntiHack);
		model.CloseSocket(pPeer->GetSocketID());
		return;
	};

	DefaultMiddle::UpdateCRC(pBuf);
	ThreadGuard guard(pClient->m_keySendIn);
	model.Send(pServicePeer, pBuf, BOOLEAN_FALSE);
}

VOID ProxyManager::Client::IngameState::OnOutgoing(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model)
{
	RPCHeader header;
	pBuf->Read<RPCHeader>(header);

	// ignore all outgoing RPC message rpc id < 10
	if (header.rpcId < 10)
		return;

	if (header.rpcId == RPC_MapService_CB_SyncProxy)
	{
		header.rpcId = RPC_MapService_SyncProxy;
		pBuf->ResetCursor();
		pBuf->Write<RPCHeader>(header);
		DefaultMiddle::UpdateCRC(pBuf);
		ThreadGuard guard(pClient->m_keySendOut);
		model.Send(pPeer, pBuf, BOOLEAN_FALSE);
		return;
	}

	UInt32 secret = Client::GetSecret(header.rpcId);
	if (secret == 0)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("invalid rpc_id, session_id %s, rpc_id %d"),
			pClient->GetSessionID().GetString().c_str(), header.rpcId);
		model.CloseSocket(pClient->m_pPeer->GetSocketID());
		return;
	}

	SessionID sid(0, pClient->GetSessionID().socketId, 0);
	pBuf->Write<SessionID>(sid);
	pBuf->ShiftCursor(ProcedureID::SIZE);

	Boolean bDisconnect = BOOLEAN_FALSE;
	switch (header.rpcId)
	{
	case RPC_MapService_CB_EnterSuccess:
		TRACE_WARNDTL_IF_2(GLOBAL_LOGGER, pClient->m_pMapPeer != NULL, _FW("map peer is not null, session_id %s, rpc_id %d"),
			pClient->GetSessionID().GetString().c_str(), header.rpcId);
		pClient->SetMapPeer(pPeer);
		break;
	case RPC_MapService_CB_LeaveSuccess:
		TRACE_WARNDTL_IF_2(GLOBAL_LOGGER, pClient->m_pMapPeer == NULL, _FW("map peer already null, session_id %s, rpc_id %d"),
			pClient->GetSessionID().GetString().c_str(), header.rpcId);
		pClient->SetMapPeer(NULL);
		break;
	case RPC_MapService_CB_ForceLogout:
	case RPC_WorldService_CB_ForceLogout:
	case RPC_Account_CB_LogoutSuccess:
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("logout, session_id %s, rpc_id %d"),
			pClient->GetSessionID().GetString().c_str(), header.rpcId);
		bDisconnect = BOOLEAN_TRUE;
		break;
	case RPC_Character_CB_AntiHack:
		{
			pBuf->ShiftCursor(RPCNetID::SIZE);
			if (gGlobal.resource.GetHackInfoSize() < 5)
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("not enought anti hack data, session_id %s, rpc_id %d"),
					pClient->GetSessionID().GetString().c_str(), header.rpcId);
				model.CloseSocket(pClient->m_pPeer->GetSocketID());
				return;
			}
			AntiHackData data;
			Index i = C_INDEX(0);
			pClient->m_idxAntiHack = gGlobal.random.GetRandom(5);

//			if (pClient->m_preWrongHackStyle)
//			{
//				while (i < 5)
//				{
//LABEL_NEXT_1:
//					Char rand = gGlobal.random.GetRandom(gGlobal.resource.GetHackInfoSize());
//					for (Index j = 0; j < i; ++j)
//					{
//						if ((Char)0x4E00 + rand == data.chars[j])
//							goto LABEL_NEXT_1;
//					}
//					data.chars[i] = (Char)0x4E00 + rand;
//					++i;
//				}
//				Byte* pMapView = NULL;
//				Byte* pImage = NULL;
//				UInt32 size = C_UINT32(0);
//				gGlobal.resource.GetAntiHackInfo(data.chars[pClient->m_idxAntiHack]-0x4E00, pMapView, pImage, size);
//				if (!pMapView || !pImage)
//				{
//					TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("get anti hack failed, SessionID %s, RPCID %d, AntiHackIndex %d"),
//						pClient->GetSessionID().GetString().c_str(), header.rpcId, pClient->m_idxAntiHack);
//					model.CloseSocket(pClient->m_pPeer->GetSocketID());
//					return;
//				}
//				// Write Byte Array
//				pBuf->Write<Size>((Size)size);
//				pBuf->WriteArray<Byte>(pImage, (CONST Count)size);
//				gGlobal.resource.ReleaseHackImagePtr(pMapView);
//				if (gConfigure.giveHackAnswer)
//					data.answer = pClient->m_idxAntiHack;
//			}
//			else
//			{
				pBuf->Write<Size>(C_SIZE(0));
				data.answer = pClient->m_idxAntiHack;
//			}
			pBuf->Write<AntiHackData>(data);	
		}
		break;
	case RPC_Character_CB_ReportAntiHack:
		pClient->m_idxAntiHack = INVALID_ANTIHACK_INDEX;
		break;
	}

	MiddleFilter::UpdateCRC(pBuf);
	// send out to client
	{
		::InterlockedIncrement(&pClient->m_cOutgoing);
		if (pClient->m_cOutgoing < gConfigure.max_out_pending)
		{
			ThreadGuard guard(pClient->m_keySendOut);
			MiddleFilter::Compress(pClient->m_pPeer, pBuf);
			MiddleFilter::Encrypt(pBuf, secret);
			model.Send(pClient->m_pPeer, pBuf, BOOLEAN_FALSE);
		}
		else if (pClient->m_cOutgoing == gConfigure.max_out_pending)
		{
			TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("over out pending, session_id %s, socket_id %d, ip %s"),
				pClient->m_context.sessionId.GetString().c_str(), pClient->m_pPeer->GetSocketID(),
				Reuben::Network::IPToString(pClient->m_pPeer->GetAddressRemote().GetIP()).c_str());
			bDisconnect = BOOLEAN_TRUE;
		}
	}
	if (bDisconnect)
	{
		Sleep(1);
		if (pClient->m_pPeer)
			model.CloseSocket(pClient->m_pPeer->GetSocketID());
	}
}

VOID ProxyManager::StoreApolloLog(SocketID socketId, UInt32 msgTime, UInt16 msgType, 
								  PCSTRING typeName, PCSTRING msg)
{
	SafeAutoPointer<CONST Peer, SafeReleaseProc<CONST Peer> > apPeerClient(
		m_model.GetPeer(socketId));
	if (!apPeerClient)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("cannot find out the peer, socketId %d"), socketId);
		return;
	}

	Client* pClient = dynamic_cast<Client*>(apPeerClient->GetData());
	if (pClient == NULL || pClient->m_pWorldPeer == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("peer without client data, socketId %d"), socketId);
	}
	else
	{
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_F("socketId %d, msgType %d, typeName %s, msg size %d"), 
			socketId, msgType, typeName, msg);

		RPCHeader header(RPC_WorldService_StoreApolloLog);
		Buffer* pBuf = SafeCreateObject(Buffer);

		pBuf->ResetCursor();
		pBuf->Write<RPCHeader>(header);
		pBuf->Write<RPCContext>(pClient->GetContext());
		pBuf->Write<UInt32>(msgTime);
		pBuf->Write<UInt16>(msgType);
		pBuf->Write<String>(typeName);
		pBuf->Write<String>(msg);

		DefaultMiddle::UpdateCRC(pBuf);
		ThreadGuard guard(pClient->m_keySendIn);
		m_model.Send(pClient->m_pWorldPeer, pBuf, BOOLEAN_FALSE);
	}
}