///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MESSENGER_STUB_H_
#define _RPC_MESSENGER_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Messenger_enum.h"

class Messenger_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult MoveGroup(LPCPEER pPeer, RPCContext &context, const UInt8 &group);
	virtual RPCResult ModifyRelation(LPCPEER pPeer, RPCContext &context, const UInt8Vector &relations);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		keyMask	= Parent::NextFreeMask + 0,
		relationMask	= Parent::NextFreeMask + 1,
		groupMask	= Parent::NextFreeMask + 2,
		friendlyMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	virtual RPCResult MoveGroup(LPCPEER pPeer, RPCContext &context, const UInt8 &group) = 0;
	virtual RPCResult ModifyRelation(LPCPEER pPeer, RPCContext &context, const UInt8Vector &relations) = 0;
	const UInt32& GetPlayer_char_id() const { return m.player_char_id; }
	const UInt32& GetFriend_char_id() const { return m.friend_char_id; }
	const UInt16& GetFriend_relation() const { return m.friend_relation; }
	const UInt8& GetFriend_group() const { return m.friend_group; }
	const UInt16& GetFriend_friendly() const { return m.friend_friendly; }
	void SetKey(const UInt32 &player_char_id, const UInt32 &friend_char_id);
	void SetPlayer_char_id(const UInt32 &player_char_id);
	void SetFriend_char_id(const UInt32 &friend_char_id);
	void SetRelation(const UInt16 &friend_relation);
	void SetFriend_relation(const UInt16 &friend_relation);
	void SetGroup(const UInt8 &friend_group);
	void SetFriend_group(const UInt8 &friend_group);
	void SetFriendly(const UInt16 &friend_friendly);
	void SetFriend_friendly(const UInt16 &friend_friendly);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//key
	UInt32 player_char_id;
	UInt32 friend_char_id;
	//relation
	UInt16 friend_relation;
	//group
	UInt8 friend_group;
	//friendly
	UInt16 friend_friendly;
} m;

public:
	INLINE Messenger_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_MESSENGER_STUB_H_
