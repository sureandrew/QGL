#pragma once
#ifndef _MANAGER_APPLICATION_H_
#define _MANAGER_APPLICATION_H_

#include "ServiceApplication.h"

struct ServiceEx;

class ManagerApplication
	: public ServiceApplication<ManagerApplication>
	, public Reuben::Utility::Singleton<ManagerApplication>
{
private:

	UInt32			m_connTime;
	UInt32			m_checkTime;
	UInt32			m_manage_interval;

private:
	
	INLINE ManagerApplication()
		: m_connTime(C_UINT32(0))
		, m_checkTime(C_UINT32(0))
		, m_manage_interval(C_UINT32(0))
	{}
	SINGLETON(ManagerApplication);

public:

	Boolean OnCreate(VOID);
	VOID OnDispose(VOID);
	VOID OnUpdate(UInt32 uTimeDelta);

	//-- ISocketListener Multithreading Functions
	virtual VOID OnAccept(CONST Listener* listener, CONST Peer* pPeer);
	virtual VOID OnFull(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener, CONST Peer* pPeer);
	virtual VOID OnClose(CONST Listener* listener);
	virtual VOID OnClose(CONST Listener* listener, CONST Peer* pPeer);

	Boolean StartService(ServiceConfigure& config);
	Boolean ConnectServices();
	Boolean ConnectManagers();
	void ConnectStarting();
	void ConnectAfter(UInt32 time);
};

#endif // _MANAGER_APPLICATION_H_
