#pragma once
#ifndef _OBJECT_APPLICATION_H_
#define _OBJECT_APPLICATION_H_

#include "Common.h"
#include "Common/ServiceApplication.h"

class ObjectApplication
	: public ServiceApplication<ObjectApplication>
	, public Reuben::Utility::Singleton<ObjectApplication>
{
private:

	UInt32 m_manage_interval;

private:

	INLINE ObjectApplication()
		: m_manage_interval(C_UINT32(0))
	{}
	SINGLETON(ObjectApplication);

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

#endif // _OBJECT_APPLICATION_H_
