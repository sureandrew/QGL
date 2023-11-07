#pragma once
#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "Proxy/Logic/Channel_Proxy.h"

class CChannel : public Channel_Proxy
{
private:
	typedef Channel_Proxy Parent;

public:

	CChannel();
	virtual ~CChannel();
	DEFINE_CLASS(Channel_CLASSID);
	virtual RPCResult CB_ReceiveMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName, const SessionIDVector &listeners);
	virtual RPCResult CB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result, const UInt32 &resultFlag);
	virtual RPCResult CB_ReceiveEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const String &SrcName, const String &TargetName, const bool &isPet);
	virtual RPCResult CB_SetListen(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_LeaveChannel(LPCPEER pPeer, RPCContext &context);
	virtual void OnUpdateInfo(const BYTE &ChannelType);
};

#endif //_CHANNEL_H_
