#pragma once
#ifndef _LOCAL_RPC_MANAGER_H_
#define _LOCAL_RPC_MANAGER_H_

#if defined(QGL)
	#include "Reuben\Network\EventSelectModel.h"
	#define NETWORK_MODEL Reuben::Network::EventSelectModel
#elif defined(SIMCLIENT)
	#include "Reuben\Network\IOCPCModel.h"
	#define NETWORK_MODEL Reuben::Network::IOCPCModel
#elif defined(SERVICE)
	//#include "Reuben\Network\IOCPTModel.h"
	//#define NETWORK_MODEL Reuben::Network::IOCPTModel
	#include "Reuben\Network\IOCPCModel.h"
	#define NETWORK_MODEL Reuben::Network::IOCPCModel
#else
#error "The project should define QGL for client or SERVICE for service"
#endif

#include "Reuben/Network/RPC.h"
#include "Reuben/Network/RPCManager.h"
#include "Common/SessionManager.h"

#define RPC_PROFILING					// enable RPC profiling if defined
#ifdef RPC_PROFILING
	#define RPC_PROF_REQUIRE	1000	// required total OnUpdate time to show profile
	#define RPC_PROF_CONCERN	200		// concern profile only over this time
#endif

#define ERR_RPCMANAGER		2

#ifdef CLIENT
	#include "Reuben/Network/MiddleFilter.h"
	#define RPCMANGER_CLASS Reuben::Network::RPCManager<NETWORK_MODEL, LocalRPCManager, \
										Reuben::Network::MiddleFilter>
#else
	#define RPCMANGER_CLASS Reuben::Network::RPCManager<NETWORK_MODEL, LocalRPCManager>
#endif

typedef Boolean(*HandleFunc)(LPCPEER, RPCContext &);

class LocalRPCManager : 
	public RPCMANGER_CLASS,
	public Reuben::Utility::Singleton<LocalRPCManager>
{
private:
	typedef RPCMANGER_CLASS Parent;

public:
	typedef StlMap<RPCNetID, NetObject*, std::less<RPCNetID>, 
		ManagedAllocator<std::pair<RPCNetID, NetObject*> > > NetIDMap;

	const ServiceID& GetServiceId() const;
	void SetServiceId(const ServiceID &id);
	void Register(NetObject *obj);
	void Register(NetObject *obj, const RPCNetID &netId);
	Boolean Unregister(NetObject *obj);
	Boolean Unregister(const RPCNetID &netId);
	NetObject* FindNetObject(const RPCNetID &netId);
	RPCResult SendError(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	RPCResult RecvError(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	RPCResult OnProcess(Reuben::Network::RPCMessage* pMsg);
	RPCResult DoProcess(RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	RPCResult DoDecode(RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);
	RPCResult ObjectProcess(const RPCID rpcId, LPCPEER pPeer, RPCContext & context, Buffer* pBuf);
	RPCResult ObjectDecode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);
	void SetRecvHandle(HandleFunc recvHandle);
	void SetNetIdMap(NetIDMap* netIdMap);				// set separated netId maps for SimClient
	void DeleteAll();									// delete all NetObjects
	PCSTRING GetRpcName(RPCID rpcId);					// get RPC ID function name

private:
	ServiceID		m_srvId;			// network service id
	NetIDMap		m_netIdMap;			// RPCNetID -> NetObject map
	NetIDMap*		m_custIdMap;		// customized RPCNetID -> NetObject map
	UInt32			m_idCounter;		// object ID counter
	ThreadKey		m_key;				// safe thread key

	LocalRPCManager();
	~LocalRPCManager();
	
	virtual VOID OnError(LPCPEER pPeer, RPCContext & context, CONST String & msg);
	virtual VOID OnSendError(UInt16 err, LPCPEER pPeer, Buffer* pBuffer);
	virtual VOID OnRecvError(UInt16 err, LPCPEER pPeer, Buffer* pBuffer);
	virtual VOID OnSent(CONST Peer* pPeer, CONST Address* pTo, Buffer* pBuffer){}

	SINGLETON(LocalRPCManager);

#ifdef SIMCLIENT
public:
	HandleFunc GetRecvHandle() const { return m_recvHandle; }
private:
	HandleFunc		m_recvHandle;	// customized pre-receive function handler
#endif // SIMCLIENT

#ifdef RPC_PROFILING
public:
	virtual void OnUpdate(const UInt32 dt);
private:
	void ShowProfile(UInt32 dt);
	struct ProfileInfo {
		UInt32 count;
		UInt32 time;
		ProfileInfo() : count(0), time(0) {}
		bool operator > (const ProfileInfo& b) const
		{
			return (time > b.time || (time == b.time && count > b.count));
		}
	};
	typedef StlMap<RPCID, ProfileInfo, std::less<RPCID>, 
		ManagedAllocator<std::pair<RPCID, ProfileInfo> > > ProfileMap;
	typedef StlMultiMap<ProfileInfo, RPCID, std::greater<ProfileInfo>, 
		ManagedAllocator<std::pair<ProfileInfo, RPCID> > > SortProfileMap;

	ProfileMap			m_profileMap;
#endif // RPC_PROFILING
};

INLINE LocalRPCManager::LocalRPCManager()
	: RPCMANGER_CLASS(GLOBAL_NAME, GLOBAL_CONFIG, GLOBAL_LOGGER)
{
#ifdef SIMCLIENT
	m_recvHandle = NULL;
#endif // SIMCLIENT

	m_custIdMap = NULL;
	m_idCounter = 0;
}

INLINE const ServiceID& LocalRPCManager::GetServiceId() const
{
	return m_srvId;
}

INLINE void LocalRPCManager::SetServiceId(const ServiceID &id)
{
	m_srvId = id;
}

// for local create NetObject
INLINE VOID LocalRPCManager::Register(NetObject *obj)
{
	ThreadGuard guard(m_key);
	RPCNetID netId(m_srvId, ++m_idCounter);

	if (m_idCounter == UINT32_MAX_VALUE)
		m_idCounter = 0;

	Register(obj, netId);
}

INLINE VOID LocalRPCManager::Register(NetObject *obj, const RPCNetID &netId)
{
	if (netId.objId == 0)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("invalid RPCNetID %d,%d"), netId.srvId, netId.objId);
		return;
	}
	
	// remove netId if already registered before for that object
	ThreadGuard guard(m_key);
	if (m_custIdMap)
	{
		m_custIdMap->erase(obj->GetNetId());
		(*m_custIdMap)[netId] = obj;
	}
	else
	{
		m_netIdMap.erase(obj->GetNetId());
		m_netIdMap[netId] = obj;
	}
	obj->SetNetId(netId);
}

INLINE Boolean LocalRPCManager::Unregister(NetObject *obj)
{
	if (obj == NULL)
		return BOOLEAN_FALSE;
	if (!obj->GetNetId().IsValid())
		return BOOLEAN_TRUE;

	if (Reuben::Simulation::FindObject(PtrToUint(obj)) == NULL)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("already deleted, object_id %d"), obj);
		return BOOLEAN_FALSE;
	}

	ThreadGuard guard(m_key);
	if (m_custIdMap)
	{
		NetIDMap::iterator it = m_custIdMap->find(obj->GetNetId());
		if (it != m_custIdMap->end())
		{
			m_custIdMap->erase(obj->GetNetId());
			return BOOLEAN_TRUE;
		}
	}
	else
	{
		NetIDMap::iterator it = m_netIdMap.find(obj->GetNetId());
		if (it != m_netIdMap.end())
		{
			m_netIdMap.erase(obj->GetNetId());
			return BOOLEAN_TRUE;
		}
	}

	TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("failed, %s"), obj->DebugInfo().c_str());

	return BOOLEAN_FALSE;
}

INLINE Boolean LocalRPCManager::Unregister(const RPCNetID &netId)
{
	if (netId.objId == 0)
		return BOOLEAN_TRUE;

	ThreadGuard guard(m_key);
	if (m_custIdMap)
	{
		NetIDMap::iterator it = m_custIdMap->find(netId);
		if (it != m_custIdMap->end())
		{
			m_custIdMap->erase(netId);
			return BOOLEAN_TRUE;
		}
	}
	else
	{
		NetIDMap::iterator it = m_netIdMap.find(netId);
		if (it != m_netIdMap.end())
		{
			m_netIdMap.erase(netId);
			return BOOLEAN_TRUE;
		}
	}

	TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("failed, RPCNetID %d,%d"), netId.srvId, netId.objId);

	return BOOLEAN_FALSE;
}

INLINE NetObject* LocalRPCManager::FindNetObject(const RPCNetID &netId)
{
	if (m_custIdMap)
	{
		ThreadGuard guard(m_key);
		NetIDMap::iterator iter = m_custIdMap->find(netId);
		if (iter != m_custIdMap->end())
			return iter->second;
	}
	else
	{
		ThreadGuard guard(m_key);
		NetIDMap::iterator iter = m_netIdMap.find(netId);
		if (iter != m_netIdMap.end())
			return iter->second;
	}
	return NULL;
}

INLINE RPCResult LocalRPCManager::ObjectProcess(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCNetID netId;

	pBuf->Read<RPCNetID>(netId);
	NetObject *obj = FindNetObject(netId);

	if (obj)
	{
#ifdef SERVICE
		if (SessionManager::GetInstance().IsOwnObject(context.sessionId, obj))
		{
			return obj->Process(rpcId, pPeer, context, pBuf);
		}
		else
		{
			TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("object is not owned by session, netId %d:%d sessionId %s, %s"), 
				netId.srvId, netId.objId, context.sessionId.GetString().c_str(), 
				GetRpcName(rpcId));
			return RPC_RESULT_FAIL;
		}
#else
		return obj->Process(rpcId, pPeer, context, pBuf);
#endif
	}
	else
	{
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("object not found, netId %d:%d, %s"), 
			netId.srvId, netId.objId, GetRpcName(rpcId));
		return RPC_RESULT_FAIL;
	}
}

INLINE RPCResult LocalRPCManager::ObjectDecode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCNetID netId;

	pBuf->Read<RPCNetID>(netId);
	NetObject *obj = FindNetObject(netId);

	if (obj)
		return obj->Decode(rpcId, pPeer, pBuf);
	else
	{
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("object not found, netId %d:%d, %s"), 
			netId.srvId, netId.objId, GetRpcName(rpcId));
		return RPC_RESULT_FAIL;
	}
}

INLINE RPCResult LocalRPCManager::SendError(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	Buffer* pBuf = SafeCreateObject(Buffer);
	pBuf->Write<Reuben::Network::RPCHeader>(ERR_RPCMANAGER);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(errCode);
	pBuf->Write<String>(errMsg);

	return Send(pPeer, pBuf);
}

INLINE RPCResult LocalRPCManager::RecvError(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	if (pPeer == NULL) RPC_RESULT_FAIL;

#ifdef SIMCLIENT
	if (m_recvHandle)
		(*m_recvHandle) (pPeer, context);
#endif // SIMCLIENT

	TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("sessionId %s, errCode %d, %s"), 
		context.sessionId.GetString().c_str(), errCode, errMsg.c_str());

	return RPC_RESULT_OK;
}

INLINE VOID LocalRPCManager::OnError(LPCPEER pPeer, RPCContext &context, CONST String &msg)
{
	if (pPeer == NULL) return;

#ifdef SIMCLIENT
	if (m_recvHandle)
		(*m_recvHandle) (pPeer, context);
#endif // SIMCLIENT

	TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("sessionId %s, socketId %d, %s"), 
		context.sessionId.GetString().c_str(), pPeer->GetSocketID(), msg.c_str());
}

#ifdef SIMCLIENT

INLINE void LocalRPCManager::SetRecvHandle(HandleFunc recvHandle) 
{
	m_recvHandle = recvHandle;
}

#endif // SIMCLIENT

INLINE void LocalRPCManager::SetNetIdMap(NetIDMap* netIdMap)
{
	ThreadGuard guard(m_key);
	m_custIdMap = netIdMap;
}

INLINE VOID LocalRPCManager::OnSendError(UInt16 err, LPCPEER pPeer, Buffer* pBuffer)
{
	if (pPeer == NULL) return;

#ifdef SIMCLIENT
	if (m_recvHandle)
	{
		RPCContext context;
		(*m_recvHandle) (pPeer, context);
	}
#endif // SIMCLIENT

	Parent::OnSendError(err, pPeer, pBuffer);

#ifndef SERVICE
	m_model.CloseSocket(pPeer->GetSocketID());
#endif // SERVICE
}

INLINE VOID LocalRPCManager::OnRecvError(UInt16 err, LPCPEER pPeer, Buffer* pBuffer)
{
	if (pPeer == NULL) return;

#ifdef SIMCLIENT
	if (m_recvHandle)
	{
		RPCContext context;
		(*m_recvHandle) (pPeer, context);
	}
#endif // SIMCLIENT

	Parent::OnRecvError(err, pPeer, pBuffer);

#ifndef SERVICE
	m_model.CloseSocket(pPeer->GetSocketID());
#endif // SERVICE
}

#endif		// _LOCAL_RPC_MANAGER_H_
