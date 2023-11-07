//-- Common
#include "Common.h"
//-- Self
#include "SimClient/Channel.h"
//-- Library
#include "SimClient/Global.h"

REGISTER_CLASSTYPE(CChannel, Channel_CLASSID);

CChannel::CChannel()
{
}

CChannel::~CChannel()
{
}

RPCResult CChannel::CB_ReceiveMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName, const SessionIDVector &listeners)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Channel %d ReceiveMessage"), GetChannelType());

	return RPC_RESULT_OK;
}

void CChannel::OnUpdateInfo(const BYTE &ChannelType)
{
}

RPCResult CChannel::CB_SendGrpMessage(LPCPEER pPeer, RPCContext &context,  const BYTE &Result, const UInt32 &resultFlag)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Channel %d SendGrpMessage"), 
		GetChannelType());

	return RPC_RESULT_OK;
}


RPCResult CChannel::CB_SetListen(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CChannel::CB_LeaveChannel(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("LeaveChannel %d:"), GetChannelType());

	//gGlobal.m_pChannel[GetChannelType()] = NULL;
	TRACE_ENSURE(gGlobal.m_pSimClient);
	DeleteNetObject(gGlobal.m_pSimClient->m_pChannel[GetChannelType()]);

	return RPC_RESULT_OK;
}

RPCResult CChannel::CB_ReceiveEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const String &SrcName, const String &TargetName, const bool &isPet)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("emotion id %d"), emo_id);


	return RPC_RESULT_OK;
}
