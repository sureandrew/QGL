//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Channel_proxy.h"


void Channel_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<BYTE>(m.ChannelType);
	}
}

void Channel_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Channel_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(infoMask)) OnUpdateInfo(m.ChannelType);
}

#pragma optimize("", off)
RPCResult Channel_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Channel_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_Channel_CB_ReceiveMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			String SrcName;
			pBuf->Read<String>(SrcName);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);
			_result = CB_ReceiveMessage(pPeer, context, Msg, SrcName, listeners);
		}
		break;
	case RPC_Channel_CB_SendGrpMessage:
		{
			BYTE Result;
			pBuf->Read<BYTE>(Result);
			UInt32 resultFlag;
			pBuf->Read<UInt32>(resultFlag);
			_result = CB_SendGrpMessage(pPeer, context, Result, resultFlag);
		}
		break;
	case RPC_Channel_CB_ReceiveEmotion:
		{
			UInt32 emo_id;
			pBuf->Read<UInt32>(emo_id);
			String SrcName;
			pBuf->Read<String>(SrcName);
			String TargetName;
			pBuf->Read<String>(TargetName);
			bool isPet;
			pBuf->Read<bool>(isPet);
			_result = CB_ReceiveEmotion(pPeer, context, emo_id, SrcName, TargetName, isPet);
		}
		break;
	case RPC_Channel_CB_SetListen:
		{
			_result = CB_SetListen(pPeer, context);
		}
		break;
	case RPC_Channel_CB_LeaveChannel:
		{
			_result = CB_LeaveChannel(pPeer, context);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Channel_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Channel_UP_STATE:
			_result = RPCDecode<684878080>(pPeer, pBuf);
		break;
	case RPC_Channel_CB_ReceiveMessage:
			_result = RPCDecode<953438141>(pPeer, pBuf);
		break;
	case RPC_Channel_CB_SendGrpMessage:
			_result = RPCDecode<1809463618>(pPeer, pBuf);
		break;
	case RPC_Channel_CB_ReceiveEmotion:
			_result = RPCDecode<651116148>(pPeer, pBuf);
		break;
	case RPC_Channel_CB_SetListen:
			_result = RPCDecode<1449986532>(pPeer, pBuf);
		break;
	case RPC_Channel_CB_LeaveChannel:
			_result = RPCDecode<1267335454>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
