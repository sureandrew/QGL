///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MESSENGERDATA_PROXY_H_
#define _RPC_MESSENGERDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/MessengerData_enum.h"

class MessengerData_Proxy : public NetObject
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
	const UInt32& GetPlayer_char_id() const { return m.player_char_id; }
	const UInt32& GetFriend_char_id() const { return m.friend_char_id; }
	const UInt16& GetFriend_relation() const { return m.friend_relation; }
	const UInt8& GetFriend_group() const { return m.friend_group; }
	const UInt16& GetFriend_friendly() const { return m.friend_friendly; }
	virtual void OnUpdateKey(const UInt32 &player_char_id, const UInt32 &friend_char_id) {};
	virtual void OnUpdateRelation(const UInt16 &friend_relation) {};
	virtual void OnUpdateGroup(const UInt8 &friend_group) {};
	virtual void OnUpdateFriendly(const UInt16 &friend_friendly) {};
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
	INLINE MessengerData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult MessengerData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessengerData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1926431383>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(74606478)
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1926431383)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_MESSENGERDATA_PROXY_H_
