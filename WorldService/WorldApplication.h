#pragma once
#ifndef _WORLD_APPLICATION_H_
#define _WORLD_APPLICATION_H_

#include "ServiceApplication.h"

class WorldApplication
	: public ServiceApplication<WorldApplication>
	, public Reuben::Utility::Singleton<WorldApplication>
{
private:

	UInt32 m_manage_interval;

private:
	
	INLINE WorldApplication()
		: m_manage_interval(C_UINT32(0))
	{}
	SINGLETON(WorldApplication);

public:

	Boolean OnCreate(VOID);
	VOID OnDispose(VOID);
	VOID OnFatalError(Exception& ex);
	VOID OnUpdate(UInt32 uTimeDelta);

	//-- ISocketListener Multithreading Functions
	virtual VOID OnAccept(CONST Listener* listener, CONST Peer* pPeer);
	virtual VOID OnFull(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener, CONST Peer* pPeer);
	virtual VOID OnClose(CONST Listener* listener);
	virtual VOID OnClose(CONST Listener* listener, CONST Peer* pPeer);

private:

	UInt32 CalcInitTimezoneWait();
	UInt8 GetGameTime();
	Boolean isInOverTimeUpdateRank(UInt8 rankType,time_t now);
};

#endif // _WORLD_APPLICATION_H_