#pragma once
#ifndef _CHANNELVIEW_H_
#define _CHANNELVIEW_H_

class CChannelView
{
public:
	virtual VOID NotifyReceiveMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName) = 0;
	virtual VOID NotifyCB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result, const UInt32 &resultFlag) = 0;
	virtual VOID NotifyOnUpdateInfo(const BYTE &ChannelType) = 0;
	virtual VOID NotifyCB_LeaveChannel() = 0;
};

#endif