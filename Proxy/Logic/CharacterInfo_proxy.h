///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_CHARACTERINFO_PROXY_H_
#define _RPC_CHARACTERINFO_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/CharacterInfo_enum.h"

class CharacterInfo_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &acct_id, const UInt32 &char_id);
	virtual void OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt16 &level, const UInt32 &exp, const UInt8 &faction, const UInt32 &money);
	virtual void OnUpdateMap(const UInt16 &map_id, const UInt8 &line_id, const UInt32 &map_ownerId, const UInt16 &map_x, const UInt16 &map_y);
	virtual void OnUpdateReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y);
	virtual void OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor);
	virtual void OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);
	virtual void OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
	virtual void OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor);
	virtual void OnUpdateStatus(const UInt8 &status);
	virtual void OnUpdateOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor);
	virtual void OnUpdateOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor);
	virtual void OnUpdateDeleteDate(const UInt32 &delete_date);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		infoMask	= Parent::NextFreeMask + 1,
		mapMask	= Parent::NextFreeMask + 2,
		rebornMask	= Parent::NextFreeMask + 3,
		styleMask	= Parent::NextFreeMask + 4,
		eqWeaponMask	= Parent::NextFreeMask + 5,
		eqHeadMask	= Parent::NextFreeMask + 6,
		eqClothMask	= Parent::NextFreeMask + 7,
		statusMask	= Parent::NextFreeMask + 8,
		orgHairStyleMask	= Parent::NextFreeMask + 9,
		orgEqClothMask	= Parent::NextFreeMask + 10,
		deleteDateMask	= Parent::NextFreeMask + 11,
		NextFreeMask	= Parent::NextFreeMask + 12
	};
	const UInt32& GetAcct_id() const { return m.acct_id; }
	const UInt32& GetChar_id() const { return m.char_id; }
	const String& GetNickName() const { return m.nickName; }
	const UInt8& GetSex() const { return m.sex; }
	const UInt8& GetCclass() const { return m.cclass; }
	const UInt16& GetLevel() const { return m.level; }
	const UInt32& GetExp() const { return m.exp; }
	const UInt8& GetFaction() const { return m.faction; }
	const UInt32& GetMoney() const { return m.money; }
	const UInt16& GetMap_id() const { return m.map_id; }
	const UInt8& GetLine_id() const { return m.line_id; }
	const UInt32& GetMap_ownerId() const { return m.map_ownerId; }
	const UInt16& GetMap_x() const { return m.map_x; }
	const UInt16& GetMap_y() const { return m.map_y; }
	const UInt16& GetReborn_map() const { return m.reborn_map; }
	const UInt16& GetReborn_x() const { return m.reborn_x; }
	const UInt16& GetReborn_y() const { return m.reborn_y; }
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
	const UInt8& GetStatus() const { return m.status; }
	const UInt16& GetOrg_hairStyle1() const { return m.org_hairStyle1; }
	const UInt16& GetOrg_hairColor() const { return m.org_hairColor; }
	const UInt32& GetOrg_eq_cloth() const { return m.org_eq_cloth; }
	const UInt16& GetOrg_eq_clothColor() const { return m.org_eq_clothColor; }
	const UInt32& GetDelete_date() const { return m.delete_date; }
	virtual void OnUpdateId(const UInt32 &acct_id, const UInt32 &char_id) {};
	virtual void OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt16 &level, const UInt32 &exp, const UInt8 &faction, const UInt32 &money) {};
	virtual void OnUpdateMap(const UInt16 &map_id, const UInt8 &line_id, const UInt32 &map_ownerId, const UInt16 &map_x, const UInt16 &map_y) {};
	virtual void OnUpdateReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y) {};
	virtual void OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor) {};
	virtual void OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor) {};
	virtual void OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor) {};
	virtual void OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor) {};
	virtual void OnUpdateStatus(const UInt8 &status) {};
	virtual void OnUpdateOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor) {};
	virtual void OnUpdateOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor) {};
	virtual void OnUpdateDeleteDate(const UInt32 &delete_date) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 acct_id;
	UInt32 char_id;
	//info
	String nickName;
	UInt8 sex;
	UInt8 cclass;
	UInt16 level;
	UInt32 exp;
	UInt8 faction;
	UInt32 money;
	//map
	UInt16 map_id;
	UInt8 line_id;
	UInt32 map_ownerId;
	UInt16 map_x;
	UInt16 map_y;
	//reborn
	UInt16 reborn_map;
	UInt16 reborn_x;
	UInt16 reborn_y;
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
	//status
	UInt8 status;
	//orgHairStyle
	UInt16 org_hairStyle1;
	UInt16 org_hairColor;
	//orgEqCloth
	UInt32 org_eq_cloth;
	UInt16 org_eq_clothColor;
	//deleteDate
	UInt32 delete_date;
} m;

public:
	INLINE CharacterInfo_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult CharacterInfo_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_CharacterInfo_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1767385828>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1730051561)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1767385828)
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_CHARACTERINFO_PROXY_H_
