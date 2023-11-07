///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_BATCHARACTER_PROXY_H_
#define _RPC_BATCHARACTER_PROXY_H_

#include "Common/RPCFunc.h"
#include "BatActor.h"
#include "RPCEnum/BatCharacter_enum.h"

class BatCharacter_Proxy : public CBatActor
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo(const UInt32 &acct_id, const UInt32 &char_id, const String &nickName, const UInt8 &sex);
	virtual void OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor);
	virtual void OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);
	virtual void OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
	virtual void OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor);
	virtual void OnUpdatePKInfo(const UInt16 &PK_Value);
	virtual void OnUpdateMaxPartner(const UInt8 &maxPartner);
	virtual void OnUpdateTP(const UInt16 &TP, const UInt32 &Model_mobid);
***/

protected:
	typedef CBatActor Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		styleMask	= Parent::NextFreeMask + 1,
		eqWeaponMask	= Parent::NextFreeMask + 2,
		eqHeadMask	= Parent::NextFreeMask + 3,
		eqClothMask	= Parent::NextFreeMask + 4,
		PKInfoMask	= Parent::NextFreeMask + 5,
		maxPartnerMask	= Parent::NextFreeMask + 6,
		TPMask	= Parent::NextFreeMask + 7,
		NextFreeMask	= Parent::NextFreeMask + 8
	};
	const UInt32& GetAcct_id() const { return m.acct_id; }
	const UInt32& GetChar_id() const { return m.char_id; }
	const String& GetNickName() const { return m.nickName; }
	const UInt8& GetSex() const { return m.sex; }
	const UInt16& GetPhoto_id() const { return m.photo_id; }
	const UInt16& GetHairStyle1() const { return m.hairStyle1; }
	const UInt16& GetHairStyle2() const { return m.hairStyle2; }
	const UInt16& GetHairColor() const { return m.hairColor; }
	const UInt32& GetEq_weapon() const { return m.eq_weapon; }
	const UInt16& GetEq_weaponColor() const { return m.eq_weaponColor; }
	const UInt32& GetEq_headBand() const { return m.eq_headBand; }
	const UInt16& GetEq_headColor() const { return m.eq_headColor; }
	const UInt32& GetEq_cloth() const { return m.eq_cloth; }
	const UInt16& GetEq_clothColor() const { return m.eq_clothColor; }
	const UInt16& GetPK_Value() const { return m.PK_Value; }
	const UInt8& GetMaxPartner() const { return m.maxPartner; }
	const UInt16& GetTP() const { return m.TP; }
	const UInt32& GetModel_mobid() const { return m.Model_mobid; }
	virtual void OnUpdateInfo(const UInt32 &acct_id, const UInt32 &char_id, const String &nickName, const UInt8 &sex) {};
	virtual void OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor) {};
	virtual void OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor) {};
	virtual void OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor) {};
	virtual void OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor) {};
	virtual void OnUpdatePKInfo(const UInt16 &PK_Value) {};
	virtual void OnUpdateMaxPartner(const UInt8 &maxPartner) {};
	virtual void OnUpdateTP(const UInt16 &TP, const UInt32 &Model_mobid) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//info
	UInt32 acct_id;
	UInt32 char_id;
	String nickName;
	UInt8 sex;
	//style
	UInt16 photo_id;
	UInt16 hairStyle1;
	UInt16 hairStyle2;
	UInt16 hairColor;
	//eqWeapon
	UInt32 eq_weapon;
	UInt16 eq_weaponColor;
	//eqHead
	UInt32 eq_headBand;
	UInt16 eq_headColor;
	//eqCloth
	UInt32 eq_cloth;
	UInt16 eq_clothColor;
	//PKInfo
	UInt16 PK_Value;
	//maxPartner
	UInt8 maxPartner;
	//TP
	UInt16 TP;
	UInt32 Model_mobid;
} m;

public:
	INLINE BatCharacter_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult BatCharacter_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatCharacter_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<664798000>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1983910738)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(664798000)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_BATCHARACTER_PROXY_H_
