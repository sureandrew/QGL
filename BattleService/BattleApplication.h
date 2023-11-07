#pragma once
#ifndef _BATTLE_APPLICATION_H_
#define _BATTLE_APPLICATION_H_

#include "ServiceApplication.h"
#include <Reuben/Utility/Singleton.h>


typedef Reuben::Platform::CriticalSection ThreadKey;
typedef Reuben::Platform::ThreadGuard<ThreadKey> ThreadGuard;

class BattleApplication
	: public ServiceApplication<BattleApplication>
	, public Reuben::Utility::Singleton<BattleApplication>
{
private:

	typedef ServiceApplication<BattleApplication> Parent;

private:

	SINGLETON(BattleApplication);
	INLINE BattleApplication(){}

public:

	virtual INLINE ~BattleApplication(){}
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
	virtual VOID OnClose(CONST Listener* listener, CONST Peer* pPeer, Buffer* pIncomplete);

};

INLINE LPCPEER GetProxy(const ServiceID &serviceId)
{
	return BattleApplication::GetInstance().GetProxy(serviceId);
}

#endif // _BATTLE_APPLICATION_H_
