#ifndef _PARTY_CHAR_INFO_CLIENT_H_
#define _PARTY_CHAR_INFO_CLIENT_H_

#include "Proxy/Logic/Character.h"
#include "Proxy/Logic/PartyCharInfo.h"

class CPartyCharInfoClient : public Reuben::Simulation::Object
{
public:
	CPartyCharInfoClient() {}
	~CPartyCharInfoClient() {}
public:
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
public:
	void SetU32CharId(const UInt32 &u32CharId) {m.u32CharId=u32CharId;}
	void SetStNetId(const RPCNetID &stNetId){m.stNetId = stNetId;}
	void SetU16MapId(const UInt16 &u16MapId){m.u16MapId=u16MapId;}
	void SetU16XPos(const UInt16 &u16XPos){m.u16XPos=u16XPos;}
	void SetU16YPos(const UInt16 &u16YPos){m.u16YPos=u16YPos;}
	void SetU16Level(const UInt16 &u16Level){m.u16Level=u16Level;}
	void SetU8SexClass(const UInt8 &u8SexClass){m.u8SexClass=u8SexClass;}
	void SetU8Faction(const UInt8 &u8Faction){m.u8Faction=u8Faction;}
	void SetU16Hp(const UInt16 &u16Hp){m.u16Hp=u16Hp;}
	void SetU16MHp(const UInt16 &u16MHp){m.u16MHp=u16MHp;}
	void SetU16MaxHp(const UInt16 &u16MaxHp){m.u16MaxHp=u16MaxHp;}
	void SetU16Sp(const UInt16 &u16Sp){m.u16Sp=u16Sp;}
	void SetU16MSp(const UInt16 &u16MSp){m.u16MSp=u16MSp;}
	void SetU16MaxSp(const UInt16 &u16MaxSp){m.u16MaxSp=u16MaxSp;}
	void SetStrNickName(const String &strNickName){m.strNickName=strNickName;}
	void SetU32OrgCloth(const UInt32 &u32OrgCloth){m.u32OrgCloth=u32OrgCloth;}
	void SetU16OrgClothColor(const UInt16 &u16OrgClothColor){m.u16OrgClothColor=u16OrgClothColor;}
	void SetU16OrgHairStyle(const UInt16 &u16OrgHairStyle){m.u16OrgHairStyle=u16OrgHairStyle;}
	void SetU16OrgHairColor(const UInt16 &u16OrgHairColor){m.u16OrgHairColor=u16OrgHairColor;}
	void SetU16HairStyle1(const UInt16 &u16HairStyle1){m.u16HairStyle1=u16HairStyle1;}
	void SetU16HairColor(const UInt16 &u16HairColor){m.u16HairColor=u16HairColor;}
	void SetU32Cloth(const UInt32 &u32Cloth){m.u32Cloth=u32Cloth;}
	void SetU16ClothColor(const UInt16 &u16ClothColor){m.u16ClothColor=u16ClothColor;}
	void SetU16HeadBand(const UInt16 &u16HeadBand){m.u16HeadBand=u16HeadBand;}
	void SetU32Weapon(const UInt32 &u32Weapon){m.u32Weapon=u32Weapon;}
public:
	void SetData(CCharacter* pChar);
	void SetData(CPartyCharInfo* pNewInfo);
	void SetData(CPartyCharInfoClient* pCpcc);

	DEFINE_PARENT_CLASS(Reuben::Simulation::Object);
	DEFINE_CLASS(0);
private:
	struct _fields {
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
};

#endif