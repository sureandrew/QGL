//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Messenger_proxy.h"


void Messenger_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(keyMask))
	{
		buf.Read<UInt32>(m.player_char_id);
		buf.Read<UInt32>(m.friend_char_id);
	}
	if (m_updateMask.IsBit(relationMask))
	{
		buf.Read<UInt16>(m.friend_relation);
	}
	if (m_updateMask.IsBit(groupMask))
	{
		buf.Read<UInt8>(m.friend_group);
	}
	if (m_updateMask.IsBit(friendlyMask))
	{
		buf.Read<UInt16>(m.friend_friendly);
	}
}

void Messenger_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Messenger_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(keyMask)) OnUpdateKey(m.player_char_id, m.friend_char_id);
	if (IsUpdated(relationMask)) OnUpdateRelation(m.friend_relation);
	if (IsUpdated(groupMask)) OnUpdateGroup(m.friend_group);
	if (IsUpdated(friendlyMask)) OnUpdateFriendly(m.friend_friendly);
}

#pragma optimize("", off)
RPCResult Messenger_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Messenger_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Messenger_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Messenger_UP_STATE:
			_result = RPCDecode<494946743>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
