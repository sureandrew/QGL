#pragma once
#ifndef _PROXY_APPLICATION_H_
#define _PROXY_APPLICATION_H_

#include <Reuben\Utility\Singleton.h>
#include <Reuben\Platform\Application.h>
#include <Reuben\Utility\ProcessManager.h>
#include "Configure.h"
#include "ProxyManager.h"

class ProxyApplication
	: public Reuben::Utility::Singleton<ProxyApplication>
	, public Reuben::Platform::Application<ProxyApplication>
{
private:

	Reuben::Utility::ProcessManager m_ProcessManager;
	ProxyManager m_ProxyManager;

private:

	INLINE ProxyApplication(VOID){}
	SINGLETON(ProxyApplication);

public:

	Boolean OnCreate(VOID);
	VOID OnDispose(VOID);
	VOID OnFatalError(Exception& ex);

	VOID OnUpdate(UInt32 uTimeDelta);

	Boolean StartService(VOID);
	Boolean ConnectServices(VOID);
	void StopService(VOID);

	INLINE ProxyManager& GetManager(VOID) { return (m_ProxyManager); }
};

#endif // _PROXY_APPLICATION_H_
