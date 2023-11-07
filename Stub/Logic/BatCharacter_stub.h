///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_BATCHARACTER_STUB_H_
#define _RPC_BATCHARACTER_STUB_H_

#include "Common/RPCFunc.h"
#include "BatActor.h"
#include "RPCEnum/BatCharacter_enum.h"

class BatCharacter_Stub : public CBatActor
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	void SetInfo(const UInt32 &acct_id, const UInt32 &char_id, const String &nickName, const UInt8 &sex);
	void SetAcct_id(const UInt32 &acct_id);
	void SetChar_id(const UInt32 &char_id);
	void SetNickName(const String &nickName);
	void SetSex(const UInt8 &sex);
	void SetStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor);
	void SetPhoto_id(const UInt16 &photo_id);
	void SetHairStyle1(const UInt16 &hairStyle1);
	void SetHairStyle2(const UInt16 &hairStyle2);
	void SetHairColor(const UInt16 &hairColor);
	void SetEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);
	void SetEq_weapon(const UInt32 &eq_weapon);
	void SetEq_weaponColor(const UInt16 &eq_weaponColor);
	void SetEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
	void SetEq_headBand(const UInt32 &eq_headBand);
	void SetEq_headColor(const UInt16 &eq_headColor);
	void SetEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor);
	void SetEq_cloth(const UInt32 &eq_cloth);
	void SetEq_clothColor(const UInt16 &eq_clothColor);
	void SetPKInfo(const UInt16 &PK_Value);
	void SetPK_Value(const UInt16 &PK_Value);
	void SetMaxPartner(const UInt8 &maxPartner);
	void SetTP(const UInt16 &TP, const UInt32 &Model_mobid);
	void SetTP(const UInt16 &TP);
	void SetModel_mobid(const UInt32 &Model_mobid);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE BatCharacter_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_BATCHARACTER_STUB_H_
