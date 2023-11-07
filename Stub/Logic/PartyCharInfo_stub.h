///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PARTYCHARINFO_STUB_H_
#define _RPC_PARTYCHARINFO_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/PartyCharInfo_enum.h"

class PartyCharInfo_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		PartyIndexMask	= Parent::NextFreeMask + 0,
		CharBaseInfoMask	= Parent::NextFreeMask + 1,
		CharOrgEqMask	= Parent::NextFreeMask + 2,
		CharEqMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	const UInt16& GetU16Index() const { return m.u16Index; }
	const UInt32& GetU32CharId() const { return m.u32CharId; }
	const RPCNetID& GetStNetId() const { return m.stNetId; }
	const UInt16& GetU16MapId() const { return m.u16MapId; }
	const UInt16& GetU16XPos() const { return m.u16XPos; }
	const UInt16& GetU16YPos() const { return m.u16YPos; }
	const UInt16& GetU16Level() const { return m.u16Level; }
	const UInt8& GetU8SexClass() const { return m.u8SexClass; }
	const UInt8& GetU8Faction() const { return m.u8Faction; }
	const UInt16& GetU16Hp() const { return m.u16Hp; }
	const UInt16& GetU16MHp() const { return m.u16MHp; }
	const UInt16& GetU16MaxHp() const { return m.u16MaxHp; }
	const UInt16& GetU16Sp() const { return m.u16Sp; }
	const UInt16& GetU16MSp() const { return m.u16MSp; }
	const UInt16& GetU16MaxSp() const { return m.u16MaxSp; }
	const String& GetStrNickName() const { return m.strNickName; }
	const UInt32& GetU32OrgCloth() const { return m.u32OrgCloth; }
	const UInt16& GetU16OrgClothColor() const { return m.u16OrgClothColor; }
	const UInt16& GetU16OrgHairStyle() const { return m.u16OrgHairStyle; }
	const UInt16& GetU16OrgHairColor() const { return m.u16OrgHairColor; }
	const UInt16& GetU16HairStyle1() const { return m.u16HairStyle1; }
	const UInt16& GetU16HairColor() const { return m.u16HairColor; }
	const UInt32& GetU32Cloth() const { return m.u32Cloth; }
	const UInt16& GetU16ClothColor() const { return m.u16ClothColor; }
	const UInt16& GetU16HeadBand() const { return m.u16HeadBand; }
	const UInt32& GetU32Weapon() const { return m.u32Weapon; }
	void SetPartyIndex(const UInt16 &u16Index);
	void SetU16Index(const UInt16 &u16Index);
	void SetCharBaseInfo(const UInt32 &u32CharId, const RPCNetID &stNetId, const UInt16 &u16MapId, const UInt16 &u16XPos, const UInt16 &u16YPos, const UInt16 &u16Level, const UInt8 &u8SexClass, const UInt8 &u8Faction, const UInt16 &u16Hp, const UInt16 &u16MHp, const UInt16 &u16MaxHp, const UInt16 &u16Sp, const UInt16 &u16MSp, const UInt16 &u16MaxSp, const String &strNickName);
	void SetU32CharId(const UInt32 &u32CharId);
	void SetStNetId(const RPCNetID &stNetId);
	void SetU16MapId(const UInt16 &u16MapId);
	void SetU16XPos(const UInt16 &u16XPos);
	void SetU16YPos(const UInt16 &u16YPos);
	void SetU16Level(const UInt16 &u16Level);
	void SetU8SexClass(const UInt8 &u8SexClass);
	void SetU8Faction(const UInt8 &u8Faction);
	void SetU16Hp(const UInt16 &u16Hp);
	void SetU16MHp(const UInt16 &u16MHp);
	void SetU16MaxHp(const UInt16 &u16MaxHp);
	void SetU16Sp(const UInt16 &u16Sp);
	void SetU16MSp(const UInt16 &u16MSp);
	void SetU16MaxSp(const UInt16 &u16MaxSp);
	void SetStrNickName(const String &strNickName);
	void SetCharOrgEq(const UInt32 &u32OrgCloth, const UInt16 &u16OrgClothColor, const UInt16 &u16OrgHairStyle, const UInt16 &u16OrgHairColor, const UInt16 &u16HairStyle1, const UInt16 &u16HairColor);
	void SetU32OrgCloth(const UInt32 &u32OrgCloth);
	void SetU16OrgClothColor(const UInt16 &u16OrgClothColor);
	void SetU16OrgHairStyle(const UInt16 &u16OrgHairStyle);
	void SetU16OrgHairColor(const UInt16 &u16OrgHairColor);
	void SetU16HairStyle1(const UInt16 &u16HairStyle1);
	void SetU16HairColor(const UInt16 &u16HairColor);
	void SetCharEq(const UInt32 &u32Cloth, const UInt16 &u16ClothColor, const UInt16 &u16HeadBand, const UInt32 &u32Weapon);
	void SetU32Cloth(const UInt32 &u32Cloth);
	void SetU16ClothColor(const UInt16 &u16ClothColor);
	void SetU16HeadBand(const UInt16 &u16HeadBand);
	void SetU32Weapon(const UInt32 &u32Weapon);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//PartyIndex
	UInt16 u16Index;
	//CharBaseInfo
	UInt32 u32CharId;
	RPCNetID stNetId;
	UInt16 u16MapId;
	UInt16 u16XPos;
	UInt16 u16YPos;
	UInt16 u16Level;
	UInt8 u8SexClass;
	UInt8 u8Faction;
	UInt16 u16Hp;
	UInt16 u16MHp;
	UInt16 u16MaxHp;
	UInt16 u16Sp;
	UInt16 u16MSp;
	UInt16 u16MaxSp;
	String strNickName;
	//CharOrgEq
	UInt32 u32OrgCloth;
	UInt16 u16OrgClothColor;
	UInt16 u16OrgHairStyle;
	UInt16 u16OrgHairColor;
	UInt16 u16HairStyle1;
	UInt16 u16HairColor;
	//CharEq
	UInt32 u32Cloth;
	UInt16 u16ClothColor;
	UInt16 u16HeadBand;
	UInt32 u32Weapon;
} m;

public:
	INLINE PartyCharInfo_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_PARTYCHARINFO_STUB_H_
