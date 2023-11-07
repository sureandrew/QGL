#pragma once
#ifndef _SERVICE_APPLICATION_H_
#define _SERVICE_APPLICATION_H_

#include <Reuben/Platform/Application.h>
#include <Reuben/Utility/ProcessManager.h>
#include <Reuben/Utility/ConfigFile.h>

#define CONNECT_TIMEOUT			30000

#include "ServiceConfigure.h"

/*
struct Session
{
	virtual VOID Release() {}
};
*/

template <typename _APPL_>
class ServiceApplication
	: public Reuben::Platform::Application<_APPL_>
	, public Reuben::Network::ISocketHandler
{
protected:

	typedef StlMap<Reuben::Network::ServiceID, CONST Reuben::Network::Peer*, std::less<Reuben::Network::ServiceID>, 
					ManagedAllocator<std::pair<Reuben::Network::ServiceID, CONST Reuben::Network::Peer*> > > ProxyMap;

	ThreadKey	m_keyProxys;
	ProxyMap	m_proxys;
	CONST Listener* m_pListener;

	Reuben::Utility::ProcessManager m_processManager;

public:

	//
	//-- Required Interface to implement
	//
	//	Boolean OnCreate(VOID);
	//	VOID OnDispose(VOID);
	//	VOID OnUpdate(UInt32 uTimeDelta);
	//  VOID OnFatalError(Exception & ex);
	//
	//-- ISocketListener Multi-threading Functions
	//
	//  virtual VOID OnAccept(CONST Listener& listener, CONST Peer* peer) = 0;
	//  virtual VOID OnFull(CONST Listener* listener) = 0;
	//  virtual VOID OnOpen(CONST Listener* listener) = 0;
	//  virtual VOID OnOpen(CONST Listener* listener, CONST Peer* peer) = 0;
	//  virtual VOID OnClose(CONST Listener* listener) = 0;
	//  virtual VOID OnClose(CONST Listener* listener, CONST Peer* peer) = 0;
	//

	INLINE ServiceApplication(VOID)
		: m_pListener(NULL)
	{}

	Boolean StartService(ServiceConfigure& config)
	{
		LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();
		rpcmgr.SetServiceId(config.serviceId);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Starting %s Service"), config.strServiceName.c_str());

		rpcmgr.GetModel().SetSocketHandler(this);
		m_pListener = rpcmgr.GetModel().Listen(Address(config.ip, config.port), Reuben::Network::PROTOCOL_TCP, config.maxInQueue, config.maxConnection, BOOLEAN_TRUE);
		TRACE_ENSURE(m_pListener);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("%s Service Started"), config.strServiceName.c_str());

		return (BOOLEAN_TRUE);
	}

	Boolean ConnectServices(ServiceConfigure& config)
	{
		LocalRPCManager& rpcmgr = LocalRPCManager::GetInstance();
		typedef StlVector<Service*, ManagedAllocator<Service*> > ServiceList;
		ServiceList tryList;					// try to connect list

		for (ServiceMap::iterator iter = config.services.begin(); 
			iter != config.services.end(); iter++)
		{
			tryList.push_back(&iter->second);
		}

		//-- Connect to Services
		DWORD startTime = ::timeGetTime();		// connect start time
		while (::timeGetTime() - startTime < CONNECT_TIMEOUT && !tryList.empty())
		{
			for (ServiceList::iterator iter = tryList.begin(); iter != tryList.end();)
			{
				Service* service = *iter;
				service->pPeer = rpcmgr.GetModel().Connect(service->address, service->protocol, service->bTrust);
				if (service->pPeer)
					service->bConnected = BOOLEAN_TRUE;
				if (service->bConnected)
				{
					TRACE_INFODTL_2(GLOBAL_LOGGER, _F("%s Service Connected SocketID=%d"), service->strName.c_str(), service->pPeer->GetSocketID());
					iter = tryList.erase(iter);
				}
				else
				{
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Connecting %s Service ..."), service->strName.c_str());
					++iter;
				}
			}
			Sleep(200);
		}

		//-- Fail to Connect to Services finally
		for (ServiceList::iterator iter = tryList.begin(); iter != tryList.end(); iter++)
		{
			Service* service = *iter;
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Cannot connect to %s Service finally."), service->strName.c_str());
		}

		return (BOOLEAN_TRUE);
	}

	VOID StopService(ServiceConfigure& config)
	{
		LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();
		if (m_pListener)
			rpcmgr.GetModel().CloseSocket(m_pListener->GetSocketID());
		SafeRelease(m_pListener);

		for (ServiceMap::iterator iter = config.services.begin(); 
			iter != config.services.end(); iter++)
		{
			Service& service = iter->second;
			if (service.bConnected)
			{
				rpcmgr.GetModel().CloseSocket(service.pPeer->GetSocketID());
				SafeRelease(service.pPeer);
			}
		}
	}

	INLINE VOID Attach(Reuben::Utility::Process *pProcess)
	{
		m_processManager.Attach(pProcess);
	}

	INLINE VOID Detach(Reuben::Utility::Process *pProcess)
	{
		m_processManager.Detach(pProcess);
	}

	INLINE const Reuben::Network::Peer* GetProxy(const Reuben::Network::ServiceID &serviceId)
	{
		ThreadGuard guard(m_keyProxys);
		ProxyMap::iterator iter = m_proxys.find(serviceId);

		if (iter != m_proxys.end())
			return (iter->second);
		else
			return (NULL);
	}

	INLINE VOID RegisterProxy(const Reuben::Network::Peer* pPeer, const Reuben::Network::ServiceID &serviceId)
	{
		if (pPeer == NULL) return;

		ThreadGuard guard(m_keyProxys);
		m_proxys[serviceId] = pPeer->AddRef();
	}

	INLINE Boolean UnregisterProxy(const Reuben::Network::ServiceID &serviceId)
	{
		ThreadGuard guard(m_keyProxys);
		ProxyMap::iterator iter = m_proxys.find(serviceId);
		if (iter != m_proxys.end())
		{
			SafeRelease(iter->second);
			m_proxys.erase(iter);
			return (BOOLEAN_TRUE);
		}
		return (BOOLEAN_FALSE);
	}

	INLINE Reuben::Network::ServiceID UnregisterProxy(const Reuben::Network::Peer* pPeer)
	{
		ThreadGuard guard(m_keyProxys);
		for (ProxyMap::iterator it = m_proxys.begin(); it != m_proxys.end(); ++it)
		{
			if (it->second == pPeer)
			{
				ServiceID srvId = it->first;
				SafeRelease(it->second);
				it = m_proxys.erase(it);
				return (srvId);
			}
		}
		return (INVALID_SERVICE_ID);
	}

	INLINE Boolean TryReconnect(ServiceConfigure& config, const Reuben::Network::Peer* pPeer)
	{
		// reconnect service if suddenly closed
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
			// reconnect service
			service.pPeer = LocalRPCManager::GetInstance().GetModel().Connect(
				service.address, service.protocol, service.bTrust);
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

		return bConnect;
	}
};

#endif // _SERVICE_APPLICATION_H_
