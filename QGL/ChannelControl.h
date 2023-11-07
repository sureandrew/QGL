#pragma once
#ifndef CHANNELCONTROL_H
#define CHANNELCONTROL_H

#include "Proxy\View\ChannelView.h"
#include "Proxy\Logic\Channel.h"

class CChannelControl : public CChannelView
{
private:

	CChannel * pChannel;
	BYTE m_ChannelType;
	bool m_Listen;
public:
	CChannelControl();
	virtual ~CChannelControl();

	virtual VOID NotifyReceiveMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName);
	virtual VOID NotifyCB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result, const UInt32 &resultFlag);
	virtual VOID NotifyOnUpdateInfo(const BYTE &ChannelType);
	virtual VOID NotifyCB_LeaveChannel();
	

	VOID SetChannelNetObject(CChannel * channel);

	VOID SendChannelMessage(String Msg);
	VOID SendPrivateMessage(String Msg, String TargetName, bool IsFriend);

	VOID SendEmotion(UInt32 emo_id, UInt32 SrcID, UInt32 TarID, bool isPet);

	VOID SetListenToChannel(bool Listen);

	inline CChannel * GetChannnelNetObject(){return pChannel;}
	inline VOID SetChannnelType(BYTE Type){m_ChannelType =  Type;}
	inline BYTE GetChannnelType(){return m_ChannelType;}

	inline bool GetListenToChannel(){ return m_Listen;};

	void Update(UInt32 CurrTime);
};


#endif
