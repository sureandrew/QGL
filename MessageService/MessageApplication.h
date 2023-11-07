#pragma once
#ifndef _MESSAGE_APPLICATION_H_
#define _MESSAGE_APPLICATION_H_

#include "ServiceApplication.h"
#include <Reuben/Utility/Singleton.h>

const UInt32 HLMsgSendTime = 60000;
const UInt SysHLMsgPerClientHLMsg = 3;

class MessageApplication
	: public ServiceApplication<MessageApplication>
	, public Reuben::Utility::Singleton<MessageApplication>
{
private:

	typedef ServiceApplication<MessageApplication> Parent;

private:

	UInt32 m_manage_interval;

private:

	INLINE MessageApplication()
		: m_manage_interval(C_UINT32(0))
	{}
	SINGLETON(MessageApplication);

public:

	Boolean OnCreate(VOID);
	VOID OnDispose(VOID);
	VOID OnFatalError(Exception& ex);
	VOID OnUpdate(UInt32 uTimeDelta);
	VOID UpDateHorseLightMessage(UInt32 CurrTime);
	VOID UpDateChannelLst(UInt32 CurrTime);
	VOID UpDateMsgSchedule(UInt32 uTimeDelta);

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
	return MessageApplication::GetInstance().GetProxy(serviceId);
}

#endif // _BATTLE_APPLICATION_H_
