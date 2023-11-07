#pragma once
#ifndef _LOG_APPLICATION_H_
#define _LOG_APPLICATION_H_

#include "Common.h"
#include "Common/ServiceApplication.h"

class LogApplication
	: public ServiceApplication<LogApplication>
	, public Reuben::Utility::Singleton<LogApplication>
{
private:

	UInt32 m_manage_interval;

private:

	INLINE LogApplication()
		: m_manage_interval(C_UINT32(0))
	{}
	SINGLETON(LogApplication);

public:

	Boolean OnCreate(VOID);
	VOID OnDispose(VOID);
	VOID OnUpdate(UInt32 uTimeDelta);

	//-- ISocketListener Multithreading Functions
	virtual VOID OnAccept(CONST Listener* listener, CONST Peer* peer);
	virtual VOID OnFull(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener, CONST Peer* peer);
	virtual VOID OnClose(CONST Listener* listener);
	virtual VOID OnClose(CONST Listener* listener, CONST Peer* peer);
};

#endif // _LOG_APPLICATION_H_
