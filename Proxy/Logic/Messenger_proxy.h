///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MESSENGER_PROXY_H_
#define _RPC_MESSENGER_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Messenger_enum.h"

class Messenger_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateKey(const UInt32 &player_char_id, const UInt32 &friend_char_id);
	virtual void OnUpdateRelation(const UInt16 &friend_relation);
	virtual void OnUpdateGroup(const UInt8 &friend_group);
	virtual void OnUpdateFriendly(const UInt16 &friend_friendly);
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
	virtual RPCResult MoveGroup(LPCPEER pPeer, RPCContext &context, const UInt8 &group);
	virtual RPCResult ModifyRelation(LPCPEER pPeer, RPCContext &context, const UInt8Vector &relations);
	const UInt32& GetPlayer_char_id() const { return m.player_char_id; }
	const UInt32& GetFriend_char_id() const { return m.friend_char_id; }
	const UInt16& GetFriend_relation() const { return m.friend_relation; }
	const UInt8& GetFriend_group() const { return m.friend_group; }
	const UInt16& GetFriend_friendly() const { return m.friend_friendly; }
	virtual void OnUpdateKey(const UInt32 &player_char_id, const UInt32 &friend_char_id) {};
	virtual void OnUpdateRelation(const UInt16 &friend_relation) {};
	virtual void OnUpdateGroup(const UInt8 &friend_group) {};
	virtual void OnUpdateFriendly(const UInt16 &friend_friendly) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

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
	INLINE Messenger_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult Messenger_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Messenger_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1774213878>(pPeer, pBuf);
}

INLINE RPCResult Messenger_Proxy::MoveGroup(LPCPEER pPeer, RPCContext &context, const UInt8 &group)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Messenger_MoveGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(group);
	return RPCSend<1860468663>(pPeer, pBuf);
}

INLINE RPCResult Messenger_Proxy::ModifyRelation(LPCPEER pPeer, RPCContext &context, const UInt8Vector &relations)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Messenger_ModifyRelation);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8Vector>(relations);
	return RPCSend<2088570675>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(494946743)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1774213878)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1860468663)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(2088570675)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_MESSENGER_PROXY_H_
