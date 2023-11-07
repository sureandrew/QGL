///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SIBLINGDATA_PROXY_H_
#define _RPC_SIBLINGDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/SiblingData_enum.h"

class SiblingData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateSibling_id(const UInt32 &sibling_id);
	virtual void OnUpdateSibling_title_prefix(const String &sibling_title_prefix);
	virtual void OnUpdateMember_id1(const UInt32 &member_id1);
	virtual void OnUpdateMember_title_suffix1(const String &member_title_suffix1);
	virtual void OnUpdateMember_id2(const UInt32 &member_id2);
	virtual void OnUpdateMember_title_suffix2(const String &member_title_suffix2);
	virtual void OnUpdateMember_id3(const UInt32 &member_id3);
	virtual void OnUpdateMember_title_suffix3(const String &member_title_suffix3);
	virtual void OnUpdateMember_id4(const UInt32 &member_id4);
	virtual void OnUpdateMember_title_suffix4(const String &member_title_suffix4);
	virtual void OnUpdateMember_id5(const UInt32 &member_id5);
	virtual void OnUpdateMember_title_suffix5(const String &member_title_suffix5);
	virtual void OnUpdateMember_id6(const UInt32 &member_id6);
	virtual void OnUpdateMember_title_suffix6(const String &member_title_suffix6);
	virtual void OnUpdateMember_id7(const UInt32 &member_id7);
	virtual void OnUpdateMember_title_suffix7(const String &member_title_suffix7);
	virtual void OnUpdateMember_id8(const UInt32 &member_id8);
	virtual void OnUpdateMember_title_suffix8(const String &member_title_suffix8);
	virtual void OnUpdateMember_id9(const UInt32 &member_id9);
	virtual void OnUpdateMember_title_suffix9(const String &member_title_suffix9);
	virtual void OnUpdateMember_id10(const UInt32 &member_id10);
	virtual void OnUpdateMember_title_suffix10(const String &member_title_suffix10);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		sibling_idMask	= Parent::NextFreeMask + 0,
		sibling_title_prefixMask	= Parent::NextFreeMask + 1,
		member_id1Mask	= Parent::NextFreeMask + 2,
		member_title_suffix1Mask	= Parent::NextFreeMask + 3,
		member_id2Mask	= Parent::NextFreeMask + 4,
		member_title_suffix2Mask	= Parent::NextFreeMask + 5,
		member_id3Mask	= Parent::NextFreeMask + 6,
		member_title_suffix3Mask	= Parent::NextFreeMask + 7,
		member_id4Mask	= Parent::NextFreeMask + 8,
		member_title_suffix4Mask	= Parent::NextFreeMask + 9,
		member_id5Mask	= Parent::NextFreeMask + 10,
		member_title_suffix5Mask	= Parent::NextFreeMask + 11,
		member_id6Mask	= Parent::NextFreeMask + 12,
		member_title_suffix6Mask	= Parent::NextFreeMask + 13,
		member_id7Mask	= Parent::NextFreeMask + 14,
		member_title_suffix7Mask	= Parent::NextFreeMask + 15,
		member_id8Mask	= Parent::NextFreeMask + 16,
		member_title_suffix8Mask	= Parent::NextFreeMask + 17,
		member_id9Mask	= Parent::NextFreeMask + 18,
		member_title_suffix9Mask	= Parent::NextFreeMask + 19,
		member_id10Mask	= Parent::NextFreeMask + 20,
		member_title_suffix10Mask	= Parent::NextFreeMask + 21,
		NextFreeMask	= Parent::NextFreeMask + 22
	};
	const UInt32& GetSibling_id() const { return m.sibling_id; }
	const String& GetSibling_title_prefix() const { return m.sibling_title_prefix; }
	const UInt32& GetMember_id1() const { return m.member_id1; }
	const String& GetMember_title_suffix1() const { return m.member_title_suffix1; }
	const UInt32& GetMember_id2() const { return m.member_id2; }
	const String& GetMember_title_suffix2() const { return m.member_title_suffix2; }
	const UInt32& GetMember_id3() const { return m.member_id3; }
	const String& GetMember_title_suffix3() const { return m.member_title_suffix3; }
	const UInt32& GetMember_id4() const { return m.member_id4; }
	const String& GetMember_title_suffix4() const { return m.member_title_suffix4; }
	const UInt32& GetMember_id5() const { return m.member_id5; }
	const String& GetMember_title_suffix5() const { return m.member_title_suffix5; }
	const UInt32& GetMember_id6() const { return m.member_id6; }
	const String& GetMember_title_suffix6() const { return m.member_title_suffix6; }
	const UInt32& GetMember_id7() const { return m.member_id7; }
	const String& GetMember_title_suffix7() const { return m.member_title_suffix7; }
	const UInt32& GetMember_id8() const { return m.member_id8; }
	const String& GetMember_title_suffix8() const { return m.member_title_suffix8; }
	const UInt32& GetMember_id9() const { return m.member_id9; }
	const String& GetMember_title_suffix9() const { return m.member_title_suffix9; }
	const UInt32& GetMember_id10() const { return m.member_id10; }
	const String& GetMember_title_suffix10() const { return m.member_title_suffix10; }
	virtual void OnUpdateSibling_id(const UInt32 &sibling_id) {};
	virtual void OnUpdateSibling_title_prefix(const String &sibling_title_prefix) {};
	virtual void OnUpdateMember_id1(const UInt32 &member_id1) {};
	virtual void OnUpdateMember_title_suffix1(const String &member_title_suffix1) {};
	virtual void OnUpdateMember_id2(const UInt32 &member_id2) {};
	virtual void OnUpdateMember_title_suffix2(const String &member_title_suffix2) {};
	virtual void OnUpdateMember_id3(const UInt32 &member_id3) {};
	virtual void OnUpdateMember_title_suffix3(const String &member_title_suffix3) {};
	virtual void OnUpdateMember_id4(const UInt32 &member_id4) {};
	virtual void OnUpdateMember_title_suffix4(const String &member_title_suffix4) {};
	virtual void OnUpdateMember_id5(const UInt32 &member_id5) {};
	virtual void OnUpdateMember_title_suffix5(const String &member_title_suffix5) {};
	virtual void OnUpdateMember_id6(const UInt32 &member_id6) {};
	virtual void OnUpdateMember_title_suffix6(const String &member_title_suffix6) {};
	virtual void OnUpdateMember_id7(const UInt32 &member_id7) {};
	virtual void OnUpdateMember_title_suffix7(const String &member_title_suffix7) {};
	virtual void OnUpdateMember_id8(const UInt32 &member_id8) {};
	virtual void OnUpdateMember_title_suffix8(const String &member_title_suffix8) {};
	virtual void OnUpdateMember_id9(const UInt32 &member_id9) {};
	virtual void OnUpdateMember_title_suffix9(const String &member_title_suffix9) {};
	virtual void OnUpdateMember_id10(const UInt32 &member_id10) {};
	virtual void OnUpdateMember_title_suffix10(const String &member_title_suffix10) {};
	void SetSibling_id(const UInt32 &sibling_id);
	void SetSibling_title_prefix(const String &sibling_title_prefix);
	void SetMember_id1(const UInt32 &member_id1);
	void SetMember_title_suffix1(const String &member_title_suffix1);
	void SetMember_id2(const UInt32 &member_id2);
	void SetMember_title_suffix2(const String &member_title_suffix2);
	void SetMember_id3(const UInt32 &member_id3);
	void SetMember_title_suffix3(const String &member_title_suffix3);
	void SetMember_id4(const UInt32 &member_id4);
	void SetMember_title_suffix4(const String &member_title_suffix4);
	void SetMember_id5(const UInt32 &member_id5);
	void SetMember_title_suffix5(const String &member_title_suffix5);
	void SetMember_id6(const UInt32 &member_id6);
	void SetMember_title_suffix6(const String &member_title_suffix6);
	void SetMember_id7(const UInt32 &member_id7);
	void SetMember_title_suffix7(const String &member_title_suffix7);
	void SetMember_id8(const UInt32 &member_id8);
	void SetMember_title_suffix8(const String &member_title_suffix8);
	void SetMember_id9(const UInt32 &member_id9);
	void SetMember_title_suffix9(const String &member_title_suffix9);
	void SetMember_id10(const UInt32 &member_id10);
	void SetMember_title_suffix10(const String &member_title_suffix10);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//sibling_id
	UInt32 sibling_id;
	//sibling_title_prefix
	String sibling_title_prefix;
	//member_id1
	UInt32 member_id1;
	//member_title_suffix1
	String member_title_suffix1;
	//member_id2
	UInt32 member_id2;
	//member_title_suffix2
	String member_title_suffix2;
	//member_id3
	UInt32 member_id3;
	//member_title_suffix3
	String member_title_suffix3;
	//member_id4
	UInt32 member_id4;
	//member_title_suffix4
	String member_title_suffix4;
	//member_id5
	UInt32 member_id5;
	//member_title_suffix5
	String member_title_suffix5;
	//member_id6
	UInt32 member_id6;
	//member_title_suffix6
	String member_title_suffix6;
	//member_id7
	UInt32 member_id7;
	//member_title_suffix7
	String member_title_suffix7;
	//member_id8
	UInt32 member_id8;
	//member_title_suffix8
	String member_title_suffix8;
	//member_id9
	UInt32 member_id9;
	//member_title_suffix9
	String member_title_suffix9;
	//member_id10
	UInt32 member_id10;
	//member_title_suffix10
	String member_title_suffix10;
} m;

public:
	INLINE SiblingData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult SiblingData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SiblingData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1411188480>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1063933249)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1411188480)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_SIBLINGDATA_PROXY_H_
