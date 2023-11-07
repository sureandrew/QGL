#pragma once
#ifndef _PROXY_MANAGER_H_
#define _PROXY_MANAGER_H_

#include <Reuben/Utility/Process.h>
#include <Reuben/Network/Network.h>
#include <Reuben/Network/Peer.h>
#include <Reuben/Network/RPC.h>
#include <Reuben/Platform/Thread.h>
#include <Reuben/Utility/RefCounter.h>
#include <Reuben/Network/RPCManager.h>
#include <Reuben/Network/MiddleFilter.h>
#include <Reuben/Network/IOCPCModel.h>

typedef Reuben::Network::DefaultMiddle DefaultMiddle;
typedef Reuben::Network::MiddleFilter MiddleFilter;

#define NETWORK_MODEL Reuben::Network::IOCPCModel
#define BUFFER_HEADER_SIZE		REUBEN_NETWORK_BUFFER_HEADER_SIZE
#define INVALID_ANTIHACK_INDEX	INDEX_MAX_VALUE

struct MSGHEAD;

class ProxyManager
	: public Reuben::Utility::Process
	, public Reuben::Network::IPacketHandler
	, public Reuben::Network::ISocketHandler
{
	struct Client
		: public Reuben::Network::FilterData
	{
		struct IState 
		{
			virtual VOID OnIncoming(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model) = 0;
			virtual VOID OnOutgoing(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model) = 0;
		};

		struct VersionCheckingState : public Client::IState
		{
			virtual VOID OnIncoming(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model);
			virtual VOID OnOutgoing(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model);
		};

		struct LoginState : public Client::IState
		{
			virtual VOID OnIncoming(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model);
			virtual VOID OnOutgoing(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model);
		};

		struct IngameState : public Client::IState
		{
			virtual VOID OnIncoming(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model);
			virtual VOID OnOutgoing(Client* pClient, CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model);
		};

		RPCContext m_context;
		CONST Peer* m_pPeer;
		CONST Peer* m_pWorldPeer;
		CONST Peer* m_pMapPeer;
		CONST Peer* m_pMessagePeer;
		ThreadKey m_keySendOut;
		ThreadKey m_keySendIn;
		ThreadKey m_keyState;
		IState* m_pState;
		Index m_idxAntiHack;
		UInt8 m_preWrongHackStyle;	
		volatile LONG m_cOutgoing;

		Client(VOID)
			: m_pPeer(NULL)
			, m_pWorldPeer(NULL)
			, m_pMapPeer(NULL)
			, m_pMessagePeer(NULL)
			, m_pState(SafeCreateObject(VersionCheckingState))
			, m_idxAntiHack(INVALID_ANTIHACK_INDEX)
			, m_cOutgoing(0)
			, m_preWrongHackStyle(0)	
		{}

		virtual ~Client(VOID)
		{
			SafeRelease(m_pPeer);
			SafeRelease(m_pWorldPeer);
			SafeRelease(m_pMapPeer);
			SafeRelease(m_pMessagePeer);
			ThreadGuard guard(m_keyState);
			SafeDeleteObject(m_pState);
		}

		virtual VOID OnDispose(VOID)
		{
			Client* pData = const_cast<Client*>(this);
			SafeDeleteObject(pData);
		}

		INLINE VOID SetPeer(CONST Peer* pPeer)
		{
			SafeRelease(m_pPeer);
			if (pPeer)
				m_pPeer = pPeer->AddRef();
		}

		INLINE VOID SetWorldPeer(CONST Peer* pPeer)
		{
			SafeRelease(m_pWorldPeer);
			if (pPeer)
				m_pWorldPeer = pPeer->AddRef();
		}

		INLINE VOID SetMapPeer(CONST Peer* pPeer)
		{
			SafeRelease(m_pMapPeer);
			if (pPeer)
				m_pMapPeer = pPeer->AddRef();
		}

		INLINE VOID SetMessagePeer(CONST Peer* pPeer)
		{
			SafeRelease(m_pMessagePeer);
			if (pPeer)
				m_pMessagePeer = pPeer->AddRef();
		}

		INLINE VOID SetContext(CONST RPCContext& context) { m_context = context; }
		INLINE CONST RPCContext& GetContext(VOID) CONST { return (m_context); }
		INLINE CONST SessionID& GetSessionID(VOID) CONST { return (m_context.sessionId); }

		INLINE VOID ProcessIncoming(CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model)
		{
			ThreadGuard guard(m_keyState);
			DEBUG_ENSURE(m_pState);
			m_pState->OnIncoming(this, pPeer, pBuf, model);
		}

		INLINE VOID ProcessOutgoing(CONST Peer* pPeer, Buffer* pBuf, NETWORK_MODEL& model)
		{
			ThreadGuard guard(m_keyState);
			DEBUG_ENSURE(m_pState);
			m_pState->OnOutgoing(this, pPeer, pBuf, model);
		}

		template<typename _STATE_>
			INLINE VOID SwitchState(VOID)
		{
			ThreadGuard guard(m_keyState);
			IState* pOldState = m_pState;
			m_pState = SafeCreateObject(_STATE_);
			SafeDeleteObject(pOldState);
		}

		static CONST UInt32 GetSecret(CONST RPCID rpcid);

	};

private:

	UInt32 m_manage_interval;
	NETWORK_MODEL m_model;

public:

	ProxyManager(VOID);
	virtual ~ProxyManager(VOID);

	INLINE NETWORK_MODEL & GetModel(VOID)
	{
		return (m_model);
	}

	Boolean RegisterProxy(LPCPEER pPeer, RPCID rpcId);
	VOID ReleaseSession(LPCPEER pPeer, RPCID rpcId, Client* pClient);
	VOID DisconnectClient(LPCPEER pPeer, RPCID rpcId, Client* pClient);
	VOID UpdateStatus(LPCPEER pPeer, RPCContext &context, UInt32 status, UInt32 state);
	static CONST RPCID GetRegisterProxyRPCID(CONST ServiceType type);
	VOID SendToClients(SessionIDVector& listeners, Buffer* pBuf, UInt32 secret);
	VOID ProcessOutgoing(CONST Peer* pPeer, CONST SessionID& sid, Buffer* pBuf);
	VOID StoreApolloLog(SocketID socketId, UInt32 msgTime, UInt16 msgType, 
						PCSTRING typeName, PCSTRING msg);
	VOID SendApolloMsg(SocketID socketId, PVOID pInfo);
	static VOID RecvApolloMsg(Client* pClient, CONST Peer* pPeer, Buffer* pBuf);
	BOOL _stdcall ApolloCallBack(DWORD dwIndex, PVOID pInfo, int iLen);

	void OnUpdate(CONST UInt32 uDeltaMilliseconds);
	Boolean TryReconnect(CONST Peer* pPeer);

	virtual VOID OnAccept(CONST Listener* listener, CONST Peer* pPeer);
	virtual VOID OnFull(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener, CONST Peer* pPeer);
	virtual VOID OnClose(CONST Listener* listener);
	virtual VOID OnClose(CONST Listener* listener, CONST Peer* pPeer);

	virtual VOID OnReceived(CONST Peer* pPeer, CONST Address* pFrom, Buffer* pBuf);
	virtual VOID OnSent(CONST Peer* pPeer, CONST Address* pTo, Buffer* pBuffer);

};


#endif // _PROXY_MANAGER_H_
