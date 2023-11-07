#include "Common.h"
#include "Stub/Logic/PartyCharInfo.h"

void CPartyCharInfo::SetData(const CCharacter* pChar)
{
	SetU32CharId(pChar->GetChar_id());
	SetStNetId(pChar->GetNetId());
	SetU16MapId(pChar->GetMap_id());
	SetU16XPos(pChar->GetCurPosX());
	SetU16YPos(pChar->GetCurPosY());
	SetU16Level(pChar->GetLevel());
	SetU8SexClass(pChar->GetSexClass());
	SetU8Faction(pChar->GetFaction());
	SetU16Hp(pChar->GetHP());
	SetU16MHp(pChar->GetMHP());
	SetU16MaxHp(pChar->GetMaxHP());
	SetU16Sp(pChar->GetSP());
	SetU16MSp(pChar->GetMSP());
	SetU16MaxSp(pChar->GetMaxSP());
	SetStrNickName(pChar->GetNickName());
	SetU32OrgCloth(pChar->GetOrg_eq_cloth());
	SetU16OrgClothColor(pChar->GetOrg_eq_clothColor());
	SetU16OrgHairStyle(pChar->GetOrg_hairStyle1());
	SetU16OrgHairColor(pChar->GetOrg_hairColor());
	SetU16HairStyle1(pChar->GetHairStyle1());
	SetU16HairColor(pChar->GetHairColor());
	SetU32Cloth(pChar->GetEq_cloth());
	SetU16ClothColor(pChar->GetEq_clothColor());
	SetU16HeadBand(pChar->GetEq_headBand());
	SetU32Weapon(pChar->GetEq_weapon());
}