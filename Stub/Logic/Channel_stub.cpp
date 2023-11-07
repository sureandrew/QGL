//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Channel_stub.h"

void Channel_Stub::SetInfo(const BYTE &ChannelType)
{
	bool modified = false;
	if (m.ChannelType != ChannelType) { m.ChannelType = ChannelType; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Channel_Stub::SetChannelType(const BYTE &ChannelType)
{
	bool modified = false;
	if (m.ChannelType != ChannelType) { m.ChannelType = ChannelType; modified = true; }
	MarkUpdateAll(infoMask, modified);
}



void Channel_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Channel_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(infoMask))
	{
		buf.Write<BYTE>(m.ChannelType);
	}
}

RPCResult Channel_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Channel_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Channel_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Channel_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_Channel_SendGrpMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			UInt32 SourceId;
			pBuf->Read<UInt32>(SourceId);
			Boolean guildSysMsg;
			pBuf->Read<Boolean>(guildSysMsg);
			_result = SendGrpMessage(pPeer, context, Msg, SourceId, guildSysMsg);
		}
		break;
	case RPC_Channel_SendGrpFlushMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			UInt32 SourceId;
			pBuf->Read<UInt32>(SourceId);
			_result = SendGrpFlushMessage(pPeer, context, Msg, SourceId);
		}
		break;
	case RPC_Channel_SendEmotion:
		{
			UInt32 emo_id;
			pBuf->Read<UInt32>(emo_id);
			UInt32 SourceId;
			pBuf->Read<UInt32>(SourceId);
			UInt32 TargetID;
			pBuf->Read<UInt32>(TargetID);
			bool isPet;
			pBuf->Read<bool>(isPet);
			_result = SendEmotion(pPeer, context, emo_id, SourceId, TargetID, isPet);
		}
		break;
	case RPC_Channel_SetListen:
		{
			BOOL IsListen;
			pBuf->Read<BOOL>(IsListen);
			UInt32 char_Id;
			pBuf->Read<UInt32>(char_Id);
			_result = SetListen(pPeer, context, IsListen, char_Id);
		}
		break;
	case RPC_Channel_LeaveChannel:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = LeaveChannel(pPeer, context, char_id);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult Channel_Stub::CB_ReceiveMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName, const SessionIDVector &listeners)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Channel_CB_ReceiveMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(Msg);
	pBuf->Write<String>(SrcName);
	pBuf->Write<SessionIDVector>(listeners);
	return RPCSend<953438141>(pPeer, pBuf);
}

RPCResult Channel_Stub::CB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result, const UInt32 &resultFlag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Channel_CB_SendGrpMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<BYTE>(Result);
	pBuf->Write<UInt32>(resultFlag);
	return RPCSend<1809463618>(pPeer, pBuf);
}

RPCResult Channel_Stub::CB_ReceiveEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const String &SrcName, const String &TargetName, const bool &isPet)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Channel_CB_ReceiveEmotion);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(emo_id);
	pBuf->Write<String>(SrcName);
	pBuf->Write<String>(TargetName);
	pBuf->Write<bool>(isPet);
	return RPCSend<651116148>(pPeer, pBuf);
}

RPCResult Channel_Stub::CB_SetListen(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Channel_CB_SetListen);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1449986532>(pPeer, pBuf);
}

RPCResult Channel_Stub::CB_LeaveChannel(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Channel_CB_LeaveChannel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1267335454>(pPeer, pBuf);
}

