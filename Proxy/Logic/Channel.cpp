//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Channel.h"
#include "QGL/Global.h"

REGISTER_CLASSTYPE(CChannel, Channel_CLASSID);

CChannel::CChannel()
{
#ifdef QGL
	m_pChannelView = NULL;
#endif // QGL
}

CChannel::~CChannel()
{
}

RPCResult CChannel::CB_ReceiveMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName, const SessionIDVector &listeners)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ReceiveMessage from %s: %s"), SrcName.c_str(), Msg.c_str());
#ifdef QGL
	if (m_pChannelView)
		m_pChannelView->NotifyReceiveMessage(pPeer, context, Msg, SrcName);
#endif // QGL
	return RPC_RESULT_OK;
}

void CChannel::OnUpdateInfo(const BYTE &ChannelType)
{
}

VOID CChannel::SetView(CChannelView * pView)
{
#ifdef QGL
	m_pChannelView = pView;
#endif // QGL
}

RPCResult CChannel::CB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result, const UInt32 &resultFlag)
{
#ifdef QGL
	if (m_pChannelView)
		m_pChannelView->NotifyCB_SendGrpMessage(pPeer, context, Result, resultFlag);
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult CChannel::CB_SetListen(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CChannel::CB_LeaveChannel(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("LeaveChannel:"));
#ifdef QGL
	if (m_pChannelView)
		m_pChannelView->NotifyCB_LeaveChannel();
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult CChannel::CB_ReceiveEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const String &SrcName, const String &TargetName, const bool &isPet)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_ReceiveEmotion:"));
#ifdef QGL
		gGlobal.NotifyCB_ReceiveEmotion(emo_id, SrcName, TargetName, isPet);
#endif // QGL

	return RPC_RESULT_OK;
}