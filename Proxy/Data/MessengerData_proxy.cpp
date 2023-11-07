//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/MessengerData_proxy.h"

void MessengerData_Proxy::SetKey(const UInt32 &player_char_id, const UInt32 &friend_char_id)
{
	bool modified = false;
	if (m.player_char_id != player_char_id) { m.player_char_id = player_char_id; modified = true; }
	if (m.friend_char_id != friend_char_id) { m.friend_char_id = friend_char_id; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void MessengerData_Proxy::SetPlayer_char_id(const UInt32 &player_char_id)
{
	bool modified = false;
	if (m.player_char_id != player_char_id) { m.player_char_id = player_char_id; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void MessengerData_Proxy::SetFriend_char_id(const UInt32 &friend_char_id)
{
	bool modified = false;
	if (m.friend_char_id != friend_char_id) { m.friend_char_id = friend_char_id; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void MessengerData_Proxy::SetRelation(const UInt16 &friend_relation)
{
	bool modified = false;
	if (m.friend_relation != friend_relation) { m.friend_relation = friend_relation; modified = true; }
	MarkUpdateAll(relationMask, modified);
}

void MessengerData_Proxy::SetFriend_relation(const UInt16 &friend_relation)
{
	bool modified = false;
	if (m.friend_relation != friend_relation) { m.friend_relation = friend_relation; modified = true; }
	MarkUpdateAll(relationMask, modified);
}

void MessengerData_Proxy::SetGroup(const UInt8 &friend_group)
{
	bool modified = false;
	if (m.friend_group != friend_group) { m.friend_group = friend_group; modified = true; }
	MarkUpdateAll(groupMask, modified);
}

void MessengerData_Proxy::SetFriend_group(const UInt8 &friend_group)
{
	bool modified = false;
	if (m.friend_group != friend_group) { m.friend_group = friend_group; modified = true; }
	MarkUpdateAll(groupMask, modified);
}

void MessengerData_Proxy::SetFriendly(const UInt16 &friend_friendly)
{
	bool modified = false;
	if (m.friend_friendly != friend_friendly) { m.friend_friendly = friend_friendly; modified = true; }
	MarkUpdateAll(friendlyMask, modified);
}

void MessengerData_Proxy::SetFriend_friendly(const UInt16 &friend_friendly)
{
	bool modified = false;
	if (m.friend_friendly != friend_friendly) { m.friend_friendly = friend_friendly; modified = true; }
	MarkUpdateAll(friendlyMask, modified);
}


void MessengerData_Proxy::ReadAttr(CONST Buffer &buf)
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

void MessengerData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(keyMask))
	{
		buf.Write<UInt32>(m.player_char_id);
		buf.Write<UInt32>(m.friend_char_id);
	}
	if (mask.IsBit(relationMask))
	{
		buf.Write<UInt16>(m.friend_relation);
	}
	if (mask.IsBit(groupMask))
	{
		buf.Write<UInt8>(m.friend_group);
	}
	if (mask.IsBit(friendlyMask))
	{
		buf.Write<UInt16>(m.friend_friendly);
	}
}

void MessengerData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(keyMask)) OnUpdateKey(m.player_char_id, m.friend_char_id);
	if (IsUpdated(relationMask)) OnUpdateRelation(m.friend_relation);
	if (IsUpdated(groupMask)) OnUpdateGroup(m.friend_group);
	if (IsUpdated(friendlyMask)) OnUpdateFriendly(m.friend_friendly);
}

#pragma optimize("", off)
RPCResult MessengerData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_MessengerData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult MessengerData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_MessengerData_UP_STATE:
			_result = RPCDecode<74606478>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
