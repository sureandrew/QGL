//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/MessengerData_stub.h"

void MessengerData_Stub::SetKey(const UInt32 &player_char_id, const UInt32 &friend_char_id)
{
	bool modified = false;
	if (m.player_char_id != player_char_id) { m.player_char_id = player_char_id; modified = true; }
	if (m.friend_char_id != friend_char_id) { m.friend_char_id = friend_char_id; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void MessengerData_Stub::SetPlayer_char_id(const UInt32 &player_char_id)
{
	bool modified = false;
	if (m.player_char_id != player_char_id) { m.player_char_id = player_char_id; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void MessengerData_Stub::SetFriend_char_id(const UInt32 &friend_char_id)
{
	bool modified = false;
	if (m.friend_char_id != friend_char_id) { m.friend_char_id = friend_char_id; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void MessengerData_Stub::SetRelation(const UInt16 &friend_relation)
{
	bool modified = false;
	if (m.friend_relation != friend_relation) { m.friend_relation = friend_relation; modified = true; }
	MarkUpdateAll(relationMask, modified);
}

void MessengerData_Stub::SetFriend_relation(const UInt16 &friend_relation)
{
	bool modified = false;
	if (m.friend_relation != friend_relation) { m.friend_relation = friend_relation; modified = true; }
	MarkUpdateAll(relationMask, modified);
}

void MessengerData_Stub::SetGroup(const UInt8 &friend_group)
{
	bool modified = false;
	if (m.friend_group != friend_group) { m.friend_group = friend_group; modified = true; }
	MarkUpdateAll(groupMask, modified);
}

void MessengerData_Stub::SetFriend_group(const UInt8 &friend_group)
{
	bool modified = false;
	if (m.friend_group != friend_group) { m.friend_group = friend_group; modified = true; }
	MarkUpdateAll(groupMask, modified);
}

void MessengerData_Stub::SetFriendly(const UInt16 &friend_friendly)
{
	bool modified = false;
	if (m.friend_friendly != friend_friendly) { m.friend_friendly = friend_friendly; modified = true; }
	MarkUpdateAll(friendlyMask, modified);
}

void MessengerData_Stub::SetFriend_friendly(const UInt16 &friend_friendly)
{
	bool modified = false;
	if (m.friend_friendly != friend_friendly) { m.friend_friendly = friend_friendly; modified = true; }
	MarkUpdateAll(friendlyMask, modified);
}


void MessengerData_Stub::Clone(MessengerData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(keyMask) && !(obj->m.player_char_id == m.player_char_id)) {
		m.player_char_id = obj->m.player_char_id;
		MarkUpdateAll(keyMask, true);
	}
	if (mask.IsBit(keyMask) && !(obj->m.friend_char_id == m.friend_char_id)) {
		m.friend_char_id = obj->m.friend_char_id;
		MarkUpdateAll(keyMask, true);
	}
	if (mask.IsBit(relationMask) && !(obj->m.friend_relation == m.friend_relation)) {
		m.friend_relation = obj->m.friend_relation;
		MarkUpdateAll(relationMask, true);
	}
	if (mask.IsBit(groupMask) && !(obj->m.friend_group == m.friend_group)) {
		m.friend_group = obj->m.friend_group;
		MarkUpdateAll(groupMask, true);
	}
	if (mask.IsBit(friendlyMask) && !(obj->m.friend_friendly == m.friend_friendly)) {
		m.friend_friendly = obj->m.friend_friendly;
		MarkUpdateAll(friendlyMask, true);
	}
}

void MessengerData_Stub::ReadAttr(CONST Buffer &buf)
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

void MessengerData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult MessengerData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessengerData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult MessengerData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_MessengerData_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

