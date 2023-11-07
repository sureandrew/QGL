///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_CHARACTERINFO_STUB_H_
#define _RPC_CHARACTERINFO_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/CharacterInfo_enum.h"

class CharacterInfo_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	void SetId(const UInt32 &acct_id, const UInt32 &char_id);
	void SetAcct_id(const UInt32 &acct_id);
	void SetChar_id(const UInt32 &char_id);
	void SetInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt16 &level, const UInt32 &exp, const UInt8 &faction, const UInt32 &money);
	void SetNickName(const String &nickName);
	void SetSex(const UInt8 &sex);
	void SetCclass(const UInt8 &cclass);
	void SetLevel(const UInt16 &level);
	void SetExp(const UInt32 &exp);
	void SetFaction(const UInt8 &faction);
	void SetMoney(const UInt32 &money);
	void SetMap(const UInt16 &map_id, const UInt8 &line_id, const UInt32 &map_ownerId, const UInt16 &map_x, const UInt16 &map_y);
	void SetMap_id(const UInt16 &map_id);
	void SetLine_id(const UInt8 &line_id);
	void SetMap_ownerId(const UInt32 &map_ownerId);
	void SetMap_x(const UInt16 &map_x);
	void SetMap_y(const UInt16 &map_y);
	void SetReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y);
	void SetReborn_map(const UInt16 &reborn_map);
	void SetReborn_x(const UInt16 &reborn_x);
	void SetReborn_y(const UInt16 &reborn_y);
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
	void SetStatus(const UInt8 &status);
	void SetOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor);
	void SetOrg_hairStyle1(const UInt16 &org_hairStyle1);
	void SetOrg_hairColor(const UInt16 &org_hairColor);
	void SetOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor);
	void SetOrg_eq_cloth(const UInt32 &org_eq_cloth);
	void SetOrg_eq_clothColor(const UInt16 &org_eq_clothColor);
	void SetDeleteDate(const UInt32 &delete_date);
	void SetDelete_date(const UInt32 &delete_date);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE CharacterInfo_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_CHARACTERINFO_STUB_H_
