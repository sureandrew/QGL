#pragma once
#ifndef _MAP_APPLICATION_H_
#define _MAP_APPLICATION_H_

#include "ServiceApplication.h"
#include <Reuben/Utility/Singleton.h>

#define MAP_LINE_COUNT		50

class MapApplication
	: public ServiceApplication<MapApplication>
	, public Reuben::Utility::Singleton<MapApplication>
{
private:

	typedef ServiceApplication<MapApplication> Parent;

private:

	UInt32 m_stopTime;
	UInt32 m_manage_interval;

private:

	INLINE MapApplication()
		: m_stopTime(C_UINT32(0))
		, m_manage_interval(C_UINT32(0))
	{}
	SINGLETON(MapApplication);

public:

	VOID Shutdown(UInt32 stopTime);				// graceful shutdown at stop time
	Boolean OnCreate(VOID);
	VOID OnDispose(VOID);
	VOID OnFatalError(Exception& ex);
	VOID OnUpdate(UInt32 uTimeDelta);
	VOID Attach(Reuben::Utility::Process *pProcess) { m_processManager.Attach(pProcess); }
	VOID Detach(Reuben::Utility::Process *pProcess) { m_processManager.Detach(pProcess); }

	//-- ISocketListener Multithreading Functions
	virtual VOID OnAccept(CONST Listener* listener, CONST Peer* pPeer);
	virtual VOID OnFull(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener, CONST Peer* pPeer);
	virtual VOID OnClose(CONST Listener* listener);
	virtual VOID OnClose(CONST Listener* listener, CONST Peer* pPeer);

};

INLINE LPCPEER GetProxy(const ServiceID &serviceId)
{
	return MapApplication::GetInstance().GetProxy(serviceId);
}

#endif // _MAP_APPLICATION_H_