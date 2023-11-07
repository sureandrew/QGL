//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/CharacterData_stub.h"

void CharacterData_Stub::SetId(const UInt32 &acct_id, const UInt32 &char_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void CharacterData_Stub::SetAcct_id(const UInt32 &acct_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void CharacterData_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void CharacterData_Stub::SetInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &faction)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	if (m.sex != sex) { m.sex = sex; modified = true; }
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterData_Stub::SetNickName(const String &nickName)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterData_Stub::SetSex(const UInt8 &sex)
{
	bool modified = false;
	if (m.sex != sex) { m.sex = sex; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterData_Stub::SetCclass(const UInt8 &cclass)
{
	bool modified = false;
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterData_Stub::SetFaction(const UInt8 &faction)
{
	bool modified = false;
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterData_Stub::SetLevelInfo(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelInfoMask, modified);
}

void CharacterData_Stub::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelInfoMask, modified);
}

void CharacterData_Stub::SetExp(const UInt32 &exp)
{
	bool modified = false;
	if (m.exp != exp) { m.exp = exp; modified = true; }
	MarkUpdateAll(expMask, modified);
}

void CharacterData_Stub::SetAttrib(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI)
{
	bool modified = false;
	if (m.STA != STA) { m.STA = STA; modified = true; }
	if (m.SPR != SPR) { m.SPR = SPR; modified = true; }
	if (m.STR != STR) { m.STR = STR; modified = true; }
	if (m.CON != CON) { m.CON = CON; modified = true; }
	if (m.AGI != AGI) { m.AGI = AGI; modified = true; }
	MarkUpdateAll(attribMask, modified);
}

void CharacterData_Stub::SetSTA(const UInt16 &STA)
{
	bool modified = false;
	if (m.STA != STA) { m.STA = STA; modified = true; }
	MarkUpdateAll(attribMask, modified);
}

void CharacterData_Stub::SetSPR(const UInt16 &SPR)
{
	bool modified = false;
	if (m.SPR != SPR) { m.SPR = SPR; modified = true; }
	MarkUpdateAll(attribMask, modified);
}

void CharacterData_Stub::SetSTR(const UInt16 &STR)
{
	bool modified = false;
	if (m.STR != STR) { m.STR = STR; modified = true; }
	MarkUpdateAll(attribMask, modified);
}

void CharacterData_Stub::SetCON(const UInt16 &CON)
{
	bool modified = false;
	if (m.CON != CON) { m.CON = CON; modified = true; }
	MarkUpdateAll(attribMask, modified);
}

void CharacterData_Stub::SetAGI(const UInt16 &AGI)
{
	bool modified = false;
	if (m.AGI != AGI) { m.AGI = AGI; modified = true; }
	MarkUpdateAll(attribMask, modified);
}

void CharacterData_Stub::SetAttrPoint(const UInt16 &attrPoint)
{
	bool modified = false;
	if (m.attrPoint != attrPoint) { m.attrPoint = attrPoint; modified = true; }
	MarkUpdateAll(attrPointMask, modified);
}

void CharacterData_Stub::SetHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP)
{
	bool modified = false;
	if (m.HP != HP) { m.HP = HP; modified = true; }
	if (m.mHP != mHP) { m.mHP = mHP; modified = true; }
	if (m.maxHP != maxHP) { m.maxHP = maxHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void CharacterData_Stub::SetHP(const UInt16 &HP)
{
	bool modified = false;
	if (m.HP != HP) { m.HP = HP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void CharacterData_Stub::SetMHP(const UInt16 &mHP)
{
	bool modified = false;
	if (m.mHP != mHP) { m.mHP = mHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void CharacterData_Stub::SetMaxHP(const UInt16 &maxHP)
{
	bool modified = false;
	if (m.maxHP != maxHP) { m.maxHP = maxHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void CharacterData_Stub::SetSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP)
{
	bool modified = false;
	if (m.SP != SP) { m.SP = SP; modified = true; }
	if (m.mSP != mSP) { m.mSP = mSP; modified = true; }
	if (m.maxSP != maxSP) { m.maxSP = maxSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void CharacterData_Stub::SetSP(const UInt16 &SP)
{
	bool modified = false;
	if (m.SP != SP) { m.SP = SP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void CharacterData_Stub::SetMSP(const UInt16 &mSP)
{
	bool modified = false;
	if (m.mSP != mSP) { m.mSP = mSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void CharacterData_Stub::SetMaxSP(const UInt16 &maxSP)
{
	bool modified = false;
	if (m.maxSP != maxSP) { m.maxSP = maxSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void CharacterData_Stub::SetDP(const UInt16 &DP, const UInt16 &mDP, const UInt16 &maxDP)
{
	bool modified = false;
	if (m.DP != DP) { m.DP = DP; modified = true; }
	if (m.mDP != mDP) { m.mDP = mDP; modified = true; }
	if (m.maxDP != maxDP) { m.maxDP = maxDP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void CharacterData_Stub::SetDP(const UInt16 &DP)
{
	bool modified = false;
	if (m.DP != DP) { m.DP = DP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void CharacterData_Stub::SetMDP(const UInt16 &mDP)
{
	bool modified = false;
	if (m.mDP != mDP) { m.mDP = mDP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void CharacterData_Stub::SetMaxDP(const UInt16 &maxDP)
{
	bool modified = false;
	if (m.maxDP != maxDP) { m.maxDP = maxDP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void CharacterData_Stub::SetWC(const UInt16 &WC, const UInt16 &maxWC, const UInt16 &WcExp)
{
	bool modified = false;
	if (m.WC != WC) { m.WC = WC; modified = true; }
	if (m.maxWC != maxWC) { m.maxWC = maxWC; modified = true; }
	if (m.WcExp != WcExp) { m.WcExp = WcExp; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void CharacterData_Stub::SetWC(const UInt16 &WC)
{
	bool modified = false;
	if (m.WC != WC) { m.WC = WC; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void CharacterData_Stub::SetMaxWC(const UInt16 &maxWC)
{
	bool modified = false;
	if (m.maxWC != maxWC) { m.maxWC = maxWC; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void CharacterData_Stub::SetWcExp(const UInt16 &WcExp)
{
	bool modified = false;
	if (m.WcExp != WcExp) { m.WcExp = WcExp; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void CharacterData_Stub::SetVP(const UInt16 &VP, const UInt16 &maxVP)
{
	bool modified = false;
	if (m.VP != VP) { m.VP = VP; modified = true; }
	if (m.maxVP != maxVP) { m.maxVP = maxVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void CharacterData_Stub::SetVP(const UInt16 &VP)
{
	bool modified = false;
	if (m.VP != VP) { m.VP = VP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void CharacterData_Stub::SetMaxVP(const UInt16 &maxVP)
{
	bool modified = false;
	if (m.maxVP != maxVP) { m.maxVP = maxVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void CharacterData_Stub::SetGoodness(const Int32 &goodness)
{
	bool modified = false;
	if (m.goodness != goodness) { m.goodness = goodness; modified = true; }
	MarkUpdateAll(goodnessMask, modified);
}

void CharacterData_Stub::SetHonor(const UInt32 &honor)
{
	bool modified = false;
	if (m.honor != honor) { m.honor = honor; modified = true; }
	MarkUpdateAll(honorMask, modified);
}

void CharacterData_Stub::SetTitle(const String &title)
{
	bool modified = false;
	if (m.title != title) { m.title = title; modified = true; }
	MarkUpdateAll(titleMask, modified);
}

void CharacterData_Stub::SetManor(const UInt32 &manor)
{
	bool modified = false;
	if (m.manor != manor) { m.manor = manor; modified = true; }
	MarkUpdateAll(manorMask, modified);
}

void CharacterData_Stub::SetDonate(const UInt32 &fDonate, const UInt32 &totalFDonate, const UInt32 &mDonate, const UInt32 &totalMDonate)
{
	bool modified = false;
	if (m.fDonate != fDonate) { m.fDonate = fDonate; modified = true; }
	if (m.totalFDonate != totalFDonate) { m.totalFDonate = totalFDonate; modified = true; }
	if (m.mDonate != mDonate) { m.mDonate = mDonate; modified = true; }
	if (m.totalMDonate != totalMDonate) { m.totalMDonate = totalMDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void CharacterData_Stub::SetFDonate(const UInt32 &fDonate)
{
	bool modified = false;
	if (m.fDonate != fDonate) { m.fDonate = fDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void CharacterData_Stub::SetTotalFDonate(const UInt32 &totalFDonate)
{
	bool modified = false;
	if (m.totalFDonate != totalFDonate) { m.totalFDonate = totalFDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void CharacterData_Stub::SetMDonate(const UInt32 &mDonate)
{
	bool modified = false;
	if (m.mDonate != mDonate) { m.mDonate = mDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void CharacterData_Stub::SetTotalMDonate(const UInt32 &totalMDonate)
{
	bool modified = false;
	if (m.totalMDonate != totalMDonate) { m.totalMDonate = totalMDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void CharacterData_Stub::SetMoney(const UInt32 &money)
{
	bool modified = false;
	if (m.money != money) { m.money = money; modified = true; }
	MarkUpdateAll(moneyMask, modified);
}

void CharacterData_Stub::SetBankMoney(const UInt32 &bankMoney)
{
	bool modified = false;
	if (m.bankMoney != bankMoney) { m.bankMoney = bankMoney; modified = true; }
	MarkUpdateAll(bankMoneyMask, modified);
}

void CharacterData_Stub::SetSMoney(const UInt32 &SMoney)
{
	bool modified = false;
	if (m.SMoney != SMoney) { m.SMoney = SMoney; modified = true; }
	MarkUpdateAll(SMoneyMask, modified);
}

void CharacterData_Stub::SetPartner(const UInt32 &partner)
{
	bool modified = false;
	if (m.partner != partner) { m.partner = partner; modified = true; }
	MarkUpdateAll(partnerMask, modified);
}

void CharacterData_Stub::SetPet(const UInt32 &pet)
{
	bool modified = false;
	if (m.pet != pet) { m.pet = pet; modified = true; }
	MarkUpdateAll(petMask, modified);
}

void CharacterData_Stub::SetMaxOther(const UInt8 &maxPartner, const UInt8 &maxPet)
{
	bool modified = false;
	if (m.maxPartner != maxPartner) { m.maxPartner = maxPartner; modified = true; }
	if (m.maxPet != maxPet) { m.maxPet = maxPet; modified = true; }
	MarkUpdateAll(maxOtherMask, modified);
}

void CharacterData_Stub::SetMaxPartner(const UInt8 &maxPartner)
{
	bool modified = false;
	if (m.maxPartner != maxPartner) { m.maxPartner = maxPartner; modified = true; }
	MarkUpdateAll(maxOtherMask, modified);
}

void CharacterData_Stub::SetMaxPet(const UInt8 &maxPet)
{
	bool modified = false;
	if (m.maxPet != maxPet) { m.maxPet = maxPet; modified = true; }
	MarkUpdateAll(maxOtherMask, modified);
}

void CharacterData_Stub::SetMap(const UInt16 &map_id, const UInt16 &map_x, const UInt16 &map_y, const UInt32 &map_ownerId)
{
	bool modified = false;
	if (m.map_id != map_id) { m.map_id = map_id; modified = true; }
	if (m.map_x != map_x) { m.map_x = map_x; modified = true; }
	if (m.map_y != map_y) { m.map_y = map_y; modified = true; }
	if (m.map_ownerId != map_ownerId) { m.map_ownerId = map_ownerId; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterData_Stub::SetMap_id(const UInt16 &map_id)
{
	bool modified = false;
	if (m.map_id != map_id) { m.map_id = map_id; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterData_Stub::SetMap_x(const UInt16 &map_x)
{
	bool modified = false;
	if (m.map_x != map_x) { m.map_x = map_x; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterData_Stub::SetMap_y(const UInt16 &map_y)
{
	bool modified = false;
	if (m.map_y != map_y) { m.map_y = map_y; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterData_Stub::SetMap_ownerId(const UInt32 &map_ownerId)
{
	bool modified = false;
	if (m.map_ownerId != map_ownerId) { m.map_ownerId = map_ownerId; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterData_Stub::SetReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y)
{
	bool modified = false;
	if (m.reborn_map != reborn_map) { m.reborn_map = reborn_map; modified = true; }
	if (m.reborn_x != reborn_x) { m.reborn_x = reborn_x; modified = true; }
	if (m.reborn_y != reborn_y) { m.reborn_y = reborn_y; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void CharacterData_Stub::SetReborn_map(const UInt16 &reborn_map)
{
	bool modified = false;
	if (m.reborn_map != reborn_map) { m.reborn_map = reborn_map; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void CharacterData_Stub::SetReborn_x(const UInt16 &reborn_x)
{
	bool modified = false;
	if (m.reborn_x != reborn_x) { m.reborn_x = reborn_x; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void CharacterData_Stub::SetReborn_y(const UInt16 &reborn_y)
{
	bool modified = false;
	if (m.reborn_y != reborn_y) { m.reborn_y = reborn_y; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void CharacterData_Stub::SetStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor)
{
	bool modified = false;
	if (m.photo_id != photo_id) { m.photo_id = photo_id; modified = true; }
	if (m.hairStyle1 != hairStyle1) { m.hairStyle1 = hairStyle1; modified = true; }
	if (m.hairStyle2 != hairStyle2) { m.hairStyle2 = hairStyle2; modified = true; }
	if (m.hairColor != hairColor) { m.hairColor = hairColor; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterData_Stub::SetPhoto_id(const UInt16 &photo_id)
{
	bool modified = false;
	if (m.photo_id != photo_id) { m.photo_id = photo_id; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterData_Stub::SetHairStyle1(const UInt16 &hairStyle1)
{
	bool modified = false;
	if (m.hairStyle1 != hairStyle1) { m.hairStyle1 = hairStyle1; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterData_Stub::SetHairStyle2(const UInt16 &hairStyle2)
{
	bool modified = false;
	if (m.hairStyle2 != hairStyle2) { m.hairStyle2 = hairStyle2; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterData_Stub::SetHairColor(const UInt16 &hairColor)
{
	bool modified = false;
	if (m.hairColor != hairColor) { m.hairColor = hairColor; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterData_Stub::SetEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor)
{
	bool modified = false;
	if (m.eq_weapon != eq_weapon) { m.eq_weapon = eq_weapon; modified = true; }
	if (m.eq_weaponColor != eq_weaponColor) { m.eq_weaponColor = eq_weaponColor; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void CharacterData_Stub::SetEq_weapon(const UInt32 &eq_weapon)
{
	bool modified = false;
	if (m.eq_weapon != eq_weapon) { m.eq_weapon = eq_weapon; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void CharacterData_Stub::SetEq_weaponColor(const UInt16 &eq_weaponColor)
{
	bool modified = false;
	if (m.eq_weaponColor != eq_weaponColor) { m.eq_weaponColor = eq_weaponColor; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void CharacterData_Stub::SetEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor)
{
	bool modified = false;
	if (m.eq_headBand != eq_headBand) { m.eq_headBand = eq_headBand; modified = true; }
	if (m.eq_headColor != eq_headColor) { m.eq_headColor = eq_headColor; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void CharacterData_Stub::SetEq_headBand(const UInt32 &eq_headBand)
{
	bool modified = false;
	if (m.eq_headBand != eq_headBand) { m.eq_headBand = eq_headBand; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void CharacterData_Stub::SetEq_headColor(const UInt16 &eq_headColor)
{
	bool modified = false;
	if (m.eq_headColor != eq_headColor) { m.eq_headColor = eq_headColor; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void CharacterData_Stub::SetEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor)
{
	bool modified = false;
	if (m.eq_cloth != eq_cloth) { m.eq_cloth = eq_cloth; modified = true; }
	if (m.eq_clothColor != eq_clothColor) { m.eq_clothColor = eq_clothColor; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void CharacterData_Stub::SetEq_cloth(const UInt32 &eq_cloth)
{
	bool modified = false;
	if (m.eq_cloth != eq_cloth) { m.eq_cloth = eq_cloth; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void CharacterData_Stub::SetEq_clothColor(const UInt16 &eq_clothColor)
{
	bool modified = false;
	if (m.eq_clothColor != eq_clothColor) { m.eq_clothColor = eq_clothColor; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void CharacterData_Stub::SetState(const UInt8 &state)
{
	bool modified = false;
	if (m.state != state) { m.state = state; modified = true; }
	MarkUpdateAll(stateMask, modified);
}

void CharacterData_Stub::SetBuff(const String &buff)
{
	bool modified = false;
	if (m.buff != buff) { m.buff = buff; modified = true; }
	MarkUpdateAll(buffMask, modified);
}

void CharacterData_Stub::SetShortcut(const UInt16 &shortcut0, const UInt16 &shortcut1, const UInt16 &shortcut2, const UInt16 &shortcut3, const UInt16 &shortcut4, const UInt16 &shortcut5, const UInt16 &shortcut6, const UInt16 &shortcut7, const UInt16 &shortcut8, const UInt16 &shortcut9, const UInt8 &shortcuttype0, const UInt8 &shortcuttype1, const UInt8 &shortcuttype2, const UInt8 &shortcuttype3, const UInt8 &shortcuttype4, const UInt8 &shortcuttype5, const UInt8 &shortcuttype6, const UInt8 &shortcuttype7, const UInt8 &shortcuttype8, const UInt8 &shortcuttype9)
{
	bool modified = false;
	if (m.shortcut[0] != shortcut0) { m.shortcut[0] = shortcut0; modified = true; }
	if (m.shortcut[1] != shortcut1) { m.shortcut[1] = shortcut1; modified = true; }
	if (m.shortcut[2] != shortcut2) { m.shortcut[2] = shortcut2; modified = true; }
	if (m.shortcut[3] != shortcut3) { m.shortcut[3] = shortcut3; modified = true; }
	if (m.shortcut[4] != shortcut4) { m.shortcut[4] = shortcut4; modified = true; }
	if (m.shortcut[5] != shortcut5) { m.shortcut[5] = shortcut5; modified = true; }
	if (m.shortcut[6] != shortcut6) { m.shortcut[6] = shortcut6; modified = true; }
	if (m.shortcut[7] != shortcut7) { m.shortcut[7] = shortcut7; modified = true; }
	if (m.shortcut[8] != shortcut8) { m.shortcut[8] = shortcut8; modified = true; }
	if (m.shortcut[9] != shortcut9) { m.shortcut[9] = shortcut9; modified = true; }
	if (m.shortcuttype[0] != shortcuttype0) { m.shortcuttype[0] = shortcuttype0; modified = true; }
	if (m.shortcuttype[1] != shortcuttype1) { m.shortcuttype[1] = shortcuttype1; modified = true; }
	if (m.shortcuttype[2] != shortcuttype2) { m.shortcuttype[2] = shortcuttype2; modified = true; }
	if (m.shortcuttype[3] != shortcuttype3) { m.shortcuttype[3] = shortcuttype3; modified = true; }
	if (m.shortcuttype[4] != shortcuttype4) { m.shortcuttype[4] = shortcuttype4; modified = true; }
	if (m.shortcuttype[5] != shortcuttype5) { m.shortcuttype[5] = shortcuttype5; modified = true; }
	if (m.shortcuttype[6] != shortcuttype6) { m.shortcuttype[6] = shortcuttype6; modified = true; }
	if (m.shortcuttype[7] != shortcuttype7) { m.shortcuttype[7] = shortcuttype7; modified = true; }
	if (m.shortcuttype[8] != shortcuttype8) { m.shortcuttype[8] = shortcuttype8; modified = true; }
	if (m.shortcuttype[9] != shortcuttype9) { m.shortcuttype[9] = shortcuttype9; modified = true; }
	MarkUpdateAll(shortcutMask, modified);
}

void CharacterData_Stub::SetShortcut(const UInt8 i, const UInt16 &shortcut)
{
	bool modified = false;
	if (m.shortcut[i] != shortcut) { m.shortcut[i] = shortcut; modified = true; }
	MarkUpdateAll(shortcutMask, modified);
}

void CharacterData_Stub::SetShortcuttype(const UInt8 i, const UInt8 &shortcuttype)
{
	bool modified = false;
	if (m.shortcuttype[i] != shortcuttype) { m.shortcuttype[i] = shortcuttype; modified = true; }
	MarkUpdateAll(shortcutMask, modified);
}

void CharacterData_Stub::SetPKInfo(const UInt16 &PK_Value)
{
	bool modified = false;
	if (m.PK_Value != PK_Value) { m.PK_Value = PK_Value; modified = true; }
	MarkUpdateAll(PKInfoMask, modified);
}

void CharacterData_Stub::SetPK_Value(const UInt16 &PK_Value)
{
	bool modified = false;
	if (m.PK_Value != PK_Value) { m.PK_Value = PK_Value; modified = true; }
	MarkUpdateAll(PKInfoMask, modified);
}

void CharacterData_Stub::SetOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor)
{
	bool modified = false;
	if (m.org_hairStyle1 != org_hairStyle1) { m.org_hairStyle1 = org_hairStyle1; modified = true; }
	if (m.org_hairColor != org_hairColor) { m.org_hairColor = org_hairColor; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void CharacterData_Stub::SetOrg_hairStyle1(const UInt16 &org_hairStyle1)
{
	bool modified = false;
	if (m.org_hairStyle1 != org_hairStyle1) { m.org_hairStyle1 = org_hairStyle1; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void CharacterData_Stub::SetOrg_hairColor(const UInt16 &org_hairColor)
{
	bool modified = false;
	if (m.org_hairColor != org_hairColor) { m.org_hairColor = org_hairColor; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void CharacterData_Stub::SetOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor)
{
	bool modified = false;
	if (m.org_eq_cloth != org_eq_cloth) { m.org_eq_cloth = org_eq_cloth; modified = true; }
	if (m.org_eq_clothColor != org_eq_clothColor) { m.org_eq_clothColor = org_eq_clothColor; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void CharacterData_Stub::SetOrg_eq_cloth(const UInt32 &org_eq_cloth)
{
	bool modified = false;
	if (m.org_eq_cloth != org_eq_cloth) { m.org_eq_cloth = org_eq_cloth; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void CharacterData_Stub::SetOrg_eq_clothColor(const UInt16 &org_eq_clothColor)
{
	bool modified = false;
	if (m.org_eq_clothColor != org_eq_clothColor) { m.org_eq_clothColor = org_eq_clothColor; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void CharacterData_Stub::SetSpeakCount(const UInt16 &speakCount)
{
	bool modified = false;
	if (m.speakCount != speakCount) { m.speakCount = speakCount; modified = true; }
	MarkUpdateAll(speakCountMask, modified);
}

void CharacterData_Stub::SetOfflineTime(const UInt32 &offlineTime)
{
	bool modified = false;
	if (m.offlineTime != offlineTime) { m.offlineTime = offlineTime; modified = true; }
	MarkUpdateAll(offlineTimeMask, modified);
}

void CharacterData_Stub::SetTP(const UInt16 &TP)
{
	bool modified = false;
	if (m.TP != TP) { m.TP = TP; modified = true; }
	MarkUpdateAll(TPMask, modified);
}

void CharacterData_Stub::SetSpeakBlock(const UInt32 &speakBlock)
{
	bool modified = false;
	if (m.speakBlock != speakBlock) { m.speakBlock = speakBlock; modified = true; }
	MarkUpdateAll(speakBlockMask, modified);
}

void CharacterData_Stub::SetHawkerTime(const UInt32 &hawkerTime)
{
	bool modified = false;
	if (m.hawkerTime != hawkerTime) { m.hawkerTime = hawkerTime; modified = true; }
	MarkUpdateAll(hawkerTimeMask, modified);
}

void CharacterData_Stub::SetAvailBackPack(const UInt8 &availBackPack)
{
	bool modified = false;
	if (m.availBackPack != availBackPack) { m.availBackPack = availBackPack; modified = true; }
	MarkUpdateAll(availBackPackMask, modified);
}

void CharacterData_Stub::SetAvailWareHouse(const UInt8 &availItemWareHouse, const UInt8 &availPartnerWareHouse, const UInt8 &availPetWareHouse)
{
	bool modified = false;
	if (m.availItemWareHouse != availItemWareHouse) { m.availItemWareHouse = availItemWareHouse; modified = true; }
	if (m.availPartnerWareHouse != availPartnerWareHouse) { m.availPartnerWareHouse = availPartnerWareHouse; modified = true; }
	if (m.availPetWareHouse != availPetWareHouse) { m.availPetWareHouse = availPetWareHouse; modified = true; }
	MarkUpdateAll(availWareHouseMask, modified);
}

void CharacterData_Stub::SetAvailItemWareHouse(const UInt8 &availItemWareHouse)
{
	bool modified = false;
	if (m.availItemWareHouse != availItemWareHouse) { m.availItemWareHouse = availItemWareHouse; modified = true; }
	MarkUpdateAll(availWareHouseMask, modified);
}

void CharacterData_Stub::SetAvailPartnerWareHouse(const UInt8 &availPartnerWareHouse)
{
	bool modified = false;
	if (m.availPartnerWareHouse != availPartnerWareHouse) { m.availPartnerWareHouse = availPartnerWareHouse; modified = true; }
	MarkUpdateAll(availWareHouseMask, modified);
}

void CharacterData_Stub::SetAvailPetWareHouse(const UInt8 &availPetWareHouse)
{
	bool modified = false;
	if (m.availPetWareHouse != availPetWareHouse) { m.availPetWareHouse = availPetWareHouse; modified = true; }
	MarkUpdateAll(availWareHouseMask, modified);
}

void CharacterData_Stub::SetPvPScoreSingle(const UInt32 &PvPScoreSingle)
{
	bool modified = false;
	if (m.PvPScoreSingle != PvPScoreSingle) { m.PvPScoreSingle = PvPScoreSingle; modified = true; }
	MarkUpdateAll(PvPScoreSingleMask, modified);
}

void CharacterData_Stub::SetDeleteTime(const UInt32 &delete_time)
{
	bool modified = false;
	if (m.delete_time != delete_time) { m.delete_time = delete_time; modified = true; }
	MarkUpdateAll(deleteTimeMask, modified);
}

void CharacterData_Stub::SetDelete_time(const UInt32 &delete_time)
{
	bool modified = false;
	if (m.delete_time != delete_time) { m.delete_time = delete_time; modified = true; }
	MarkUpdateAll(deleteTimeMask, modified);
}

void CharacterData_Stub::SetBankLock(const Boolean &bankLock)
{
	bool modified = false;
	if (m.bankLock != bankLock) { m.bankLock = bankLock; modified = true; }
	MarkUpdateAll(bankLockMask, modified);
}

void CharacterData_Stub::SetSiblingId(const UInt32 &sibling_id)
{
	bool modified = false;
	if (m.sibling_id != sibling_id) { m.sibling_id = sibling_id; modified = true; }
	MarkUpdateAll(SiblingIdMask, modified);
}

void CharacterData_Stub::SetSibling_id(const UInt32 &sibling_id)
{
	bool modified = false;
	if (m.sibling_id != sibling_id) { m.sibling_id = sibling_id; modified = true; }
	MarkUpdateAll(SiblingIdMask, modified);
}

void CharacterData_Stub::SetMark(const UInt32 &mark)
{
	bool modified = false;
	if (m.mark != mark) { m.mark = mark; modified = true; }
	MarkUpdateAll(markMask, modified);
}

void CharacterData_Stub::SetWantedReward(const UInt32 &wantedReward)
{
	bool modified = false;
	if (m.wantedReward != wantedReward) { m.wantedReward = wantedReward; modified = true; }
	MarkUpdateAll(wantedRewardMask, modified);
}

void CharacterData_Stub::SetGuild_uid(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(guild_uidMask, modified);
}

void CharacterData_Stub::SetGenerationAcc(const UInt32 &iHisGenAcc, const UInt32 &uiCurGenAcc)
{
	bool modified = false;
	if (m.iHisGenAcc != iHisGenAcc) { m.iHisGenAcc = iHisGenAcc; modified = true; }
	if (m.uiCurGenAcc != uiCurGenAcc) { m.uiCurGenAcc = uiCurGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}

void CharacterData_Stub::SetIHisGenAcc(const UInt32 &iHisGenAcc)
{
	bool modified = false;
	if (m.iHisGenAcc != iHisGenAcc) { m.iHisGenAcc = iHisGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}

void CharacterData_Stub::SetUiCurGenAcc(const UInt32 &uiCurGenAcc)
{
	bool modified = false;
	if (m.uiCurGenAcc != uiCurGenAcc) { m.uiCurGenAcc = uiCurGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}


void CharacterData_Stub::Clone(CharacterData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(idMask) && !(obj->m.acct_id == m.acct_id)) {
		m.acct_id = obj->m.acct_id;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(idMask) && !(obj->m.char_id == m.char_id)) {
		m.char_id = obj->m.char_id;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.nickName == m.nickName)) {
		m.nickName = obj->m.nickName;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.sex == m.sex)) {
		m.sex = obj->m.sex;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.cclass == m.cclass)) {
		m.cclass = obj->m.cclass;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.faction == m.faction)) {
		m.faction = obj->m.faction;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(levelInfoMask) && !(obj->m.level == m.level)) {
		m.level = obj->m.level;
		MarkUpdateAll(levelInfoMask, true);
	}
	if (mask.IsBit(expMask) && !(obj->m.exp == m.exp)) {
		m.exp = obj->m.exp;
		MarkUpdateAll(expMask, true);
	}
	if (mask.IsBit(attribMask) && !(obj->m.STA == m.STA)) {
		m.STA = obj->m.STA;
		MarkUpdateAll(attribMask, true);
	}
	if (mask.IsBit(attribMask) && !(obj->m.SPR == m.SPR)) {
		m.SPR = obj->m.SPR;
		MarkUpdateAll(attribMask, true);
	}
	if (mask.IsBit(attribMask) && !(obj->m.STR == m.STR)) {
		m.STR = obj->m.STR;
		MarkUpdateAll(attribMask, true);
	}
	if (mask.IsBit(attribMask) && !(obj->m.CON == m.CON)) {
		m.CON = obj->m.CON;
		MarkUpdateAll(attribMask, true);
	}
	if (mask.IsBit(attribMask) && !(obj->m.AGI == m.AGI)) {
		m.AGI = obj->m.AGI;
		MarkUpdateAll(attribMask, true);
	}
	if (mask.IsBit(attrPointMask) && !(obj->m.attrPoint == m.attrPoint)) {
		m.attrPoint = obj->m.attrPoint;
		MarkUpdateAll(attrPointMask, true);
	}
	if (mask.IsBit(HPMask) && !(obj->m.HP == m.HP)) {
		m.HP = obj->m.HP;
		MarkUpdateAll(HPMask, true);
	}
	if (mask.IsBit(HPMask) && !(obj->m.mHP == m.mHP)) {
		m.mHP = obj->m.mHP;
		MarkUpdateAll(HPMask, true);
	}
	if (mask.IsBit(HPMask) && !(obj->m.maxHP == m.maxHP)) {
		m.maxHP = obj->m.maxHP;
		MarkUpdateAll(HPMask, true);
	}
	if (mask.IsBit(SPMask) && !(obj->m.SP == m.SP)) {
		m.SP = obj->m.SP;
		MarkUpdateAll(SPMask, true);
	}
	if (mask.IsBit(SPMask) && !(obj->m.mSP == m.mSP)) {
		m.mSP = obj->m.mSP;
		MarkUpdateAll(SPMask, true);
	}
	if (mask.IsBit(SPMask) && !(obj->m.maxSP == m.maxSP)) {
		m.maxSP = obj->m.maxSP;
		MarkUpdateAll(SPMask, true);
	}
	if (mask.IsBit(DPMask) && !(obj->m.DP == m.DP)) {
		m.DP = obj->m.DP;
		MarkUpdateAll(DPMask, true);
	}
	if (mask.IsBit(DPMask) && !(obj->m.mDP == m.mDP)) {
		m.mDP = obj->m.mDP;
		MarkUpdateAll(DPMask, true);
	}
	if (mask.IsBit(DPMask) && !(obj->m.maxDP == m.maxDP)) {
		m.maxDP = obj->m.maxDP;
		MarkUpdateAll(DPMask, true);
	}
	if (mask.IsBit(WCMask) && !(obj->m.WC == m.WC)) {
		m.WC = obj->m.WC;
		MarkUpdateAll(WCMask, true);
	}
	if (mask.IsBit(WCMask) && !(obj->m.maxWC == m.maxWC)) {
		m.maxWC = obj->m.maxWC;
		MarkUpdateAll(WCMask, true);
	}
	if (mask.IsBit(WCMask) && !(obj->m.WcExp == m.WcExp)) {
		m.WcExp = obj->m.WcExp;
		MarkUpdateAll(WCMask, true);
	}
	if (mask.IsBit(VPMask) && !(obj->m.VP == m.VP)) {
		m.VP = obj->m.VP;
		MarkUpdateAll(VPMask, true);
	}
	if (mask.IsBit(VPMask) && !(obj->m.maxVP == m.maxVP)) {
		m.maxVP = obj->m.maxVP;
		MarkUpdateAll(VPMask, true);
	}
	if (mask.IsBit(goodnessMask) && !(obj->m.goodness == m.goodness)) {
		m.goodness = obj->m.goodness;
		MarkUpdateAll(goodnessMask, true);
	}
	if (mask.IsBit(honorMask) && !(obj->m.honor == m.honor)) {
		m.honor = obj->m.honor;
		MarkUpdateAll(honorMask, true);
	}
	if (mask.IsBit(titleMask) && !(obj->m.title == m.title)) {
		m.title = obj->m.title;
		MarkUpdateAll(titleMask, true);
	}
	if (mask.IsBit(manorMask) && !(obj->m.manor == m.manor)) {
		m.manor = obj->m.manor;
		MarkUpdateAll(manorMask, true);
	}
	if (mask.IsBit(donateMask) && !(obj->m.fDonate == m.fDonate)) {
		m.fDonate = obj->m.fDonate;
		MarkUpdateAll(donateMask, true);
	}
	if (mask.IsBit(donateMask) && !(obj->m.totalFDonate == m.totalFDonate)) {
		m.totalFDonate = obj->m.totalFDonate;
		MarkUpdateAll(donateMask, true);
	}
	if (mask.IsBit(donateMask) && !(obj->m.mDonate == m.mDonate)) {
		m.mDonate = obj->m.mDonate;
		MarkUpdateAll(donateMask, true);
	}
	if (mask.IsBit(donateMask) && !(obj->m.totalMDonate == m.totalMDonate)) {
		m.totalMDonate = obj->m.totalMDonate;
		MarkUpdateAll(donateMask, true);
	}
	if (mask.IsBit(moneyMask) && !(obj->m.money == m.money)) {
		m.money = obj->m.money;
		MarkUpdateAll(moneyMask, true);
	}
	if (mask.IsBit(bankMoneyMask) && !(obj->m.bankMoney == m.bankMoney)) {
		m.bankMoney = obj->m.bankMoney;
		MarkUpdateAll(bankMoneyMask, true);
	}
	if (mask.IsBit(SMoneyMask) && !(obj->m.SMoney == m.SMoney)) {
		m.SMoney = obj->m.SMoney;
		MarkUpdateAll(SMoneyMask, true);
	}
	if (mask.IsBit(partnerMask) && !(obj->m.partner == m.partner)) {
		m.partner = obj->m.partner;
		MarkUpdateAll(partnerMask, true);
	}
	if (mask.IsBit(petMask) && !(obj->m.pet == m.pet)) {
		m.pet = obj->m.pet;
		MarkUpdateAll(petMask, true);
	}
	if (mask.IsBit(maxOtherMask) && !(obj->m.maxPartner == m.maxPartner)) {
		m.maxPartner = obj->m.maxPartner;
		MarkUpdateAll(maxOtherMask, true);
	}
	if (mask.IsBit(maxOtherMask) && !(obj->m.maxPet == m.maxPet)) {
		m.maxPet = obj->m.maxPet;
		MarkUpdateAll(maxOtherMask, true);
	}
	if (mask.IsBit(mapMask) && !(obj->m.map_id == m.map_id)) {
		m.map_id = obj->m.map_id;
		MarkUpdateAll(mapMask, true);
	}
	if (mask.IsBit(mapMask) && !(obj->m.map_x == m.map_x)) {
		m.map_x = obj->m.map_x;
		MarkUpdateAll(mapMask, true);
	}
	if (mask.IsBit(mapMask) && !(obj->m.map_y == m.map_y)) {
		m.map_y = obj->m.map_y;
		MarkUpdateAll(mapMask, true);
	}
	if (mask.IsBit(mapMask) && !(obj->m.map_ownerId == m.map_ownerId)) {
		m.map_ownerId = obj->m.map_ownerId;
		MarkUpdateAll(mapMask, true);
	}
	if (mask.IsBit(rebornMask) && !(obj->m.reborn_map == m.reborn_map)) {
		m.reborn_map = obj->m.reborn_map;
		MarkUpdateAll(rebornMask, true);
	}
	if (mask.IsBit(rebornMask) && !(obj->m.reborn_x == m.reborn_x)) {
		m.reborn_x = obj->m.reborn_x;
		MarkUpdateAll(rebornMask, true);
	}
	if (mask.IsBit(rebornMask) && !(obj->m.reborn_y == m.reborn_y)) {
		m.reborn_y = obj->m.reborn_y;
		MarkUpdateAll(rebornMask, true);
	}
	if (mask.IsBit(styleMask) && !(obj->m.photo_id == m.photo_id)) {
		m.photo_id = obj->m.photo_id;
		MarkUpdateAll(styleMask, true);
	}
	if (mask.IsBit(styleMask) && !(obj->m.hairStyle1 == m.hairStyle1)) {
		m.hairStyle1 = obj->m.hairStyle1;
		MarkUpdateAll(styleMask, true);
	}
	if (mask.IsBit(styleMask) && !(obj->m.hairStyle2 == m.hairStyle2)) {
		m.hairStyle2 = obj->m.hairStyle2;
		MarkUpdateAll(styleMask, true);
	}
	if (mask.IsBit(styleMask) && !(obj->m.hairColor == m.hairColor)) {
		m.hairColor = obj->m.hairColor;
		MarkUpdateAll(styleMask, true);
	}
	if (mask.IsBit(eqWeaponMask) && !(obj->m.eq_weapon == m.eq_weapon)) {
		m.eq_weapon = obj->m.eq_weapon;
		MarkUpdateAll(eqWeaponMask, true);
	}
	if (mask.IsBit(eqWeaponMask) && !(obj->m.eq_weaponColor == m.eq_weaponColor)) {
		m.eq_weaponColor = obj->m.eq_weaponColor;
		MarkUpdateAll(eqWeaponMask, true);
	}
	if (mask.IsBit(eqHeadMask) && !(obj->m.eq_headBand == m.eq_headBand)) {
		m.eq_headBand = obj->m.eq_headBand;
		MarkUpdateAll(eqHeadMask, true);
	}
	if (mask.IsBit(eqHeadMask) && !(obj->m.eq_headColor == m.eq_headColor)) {
		m.eq_headColor = obj->m.eq_headColor;
		MarkUpdateAll(eqHeadMask, true);
	}
	if (mask.IsBit(eqClothMask) && !(obj->m.eq_cloth == m.eq_cloth)) {
		m.eq_cloth = obj->m.eq_cloth;
		MarkUpdateAll(eqClothMask, true);
	}
	if (mask.IsBit(eqClothMask) && !(obj->m.eq_clothColor == m.eq_clothColor)) {
		m.eq_clothColor = obj->m.eq_clothColor;
		MarkUpdateAll(eqClothMask, true);
	}
	if (mask.IsBit(stateMask) && !(obj->m.state == m.state)) {
		m.state = obj->m.state;
		MarkUpdateAll(stateMask, true);
	}
	if (mask.IsBit(buffMask) && !(obj->m.buff == m.buff)) {
		m.buff = obj->m.buff;
		MarkUpdateAll(buffMask, true);
	}
	if (mask.IsBit(shortcutMask)) {
		bool updated = false;
		if (!(obj->m.shortcut[0] == m.shortcut[0])) {
			m.shortcut[0] = obj->m.shortcut[0];
			updated = true;
		}
		if (!(obj->m.shortcut[1] == m.shortcut[1])) {
			m.shortcut[1] = obj->m.shortcut[1];
			updated = true;
		}
		if (!(obj->m.shortcut[2] == m.shortcut[2])) {
			m.shortcut[2] = obj->m.shortcut[2];
			updated = true;
		}
		if (!(obj->m.shortcut[3] == m.shortcut[3])) {
			m.shortcut[3] = obj->m.shortcut[3];
			updated = true;
		}
		if (!(obj->m.shortcut[4] == m.shortcut[4])) {
			m.shortcut[4] = obj->m.shortcut[4];
			updated = true;
		}
		if (!(obj->m.shortcut[5] == m.shortcut[5])) {
			m.shortcut[5] = obj->m.shortcut[5];
			updated = true;
		}
		if (!(obj->m.shortcut[6] == m.shortcut[6])) {
			m.shortcut[6] = obj->m.shortcut[6];
			updated = true;
		}
		if (!(obj->m.shortcut[7] == m.shortcut[7])) {
			m.shortcut[7] = obj->m.shortcut[7];
			updated = true;
		}
		if (!(obj->m.shortcut[8] == m.shortcut[8])) {
			m.shortcut[8] = obj->m.shortcut[8];
			updated = true;
		}
		if (!(obj->m.shortcut[9] == m.shortcut[9])) {
			m.shortcut[9] = obj->m.shortcut[9];
			updated = true;
		}
		if (updated) MarkUpdateAll(shortcutMask, true);
	}
	if (mask.IsBit(shortcutMask)) {
		bool updated = false;
		if (!(obj->m.shortcuttype[0] == m.shortcuttype[0])) {
			m.shortcuttype[0] = obj->m.shortcuttype[0];
			updated = true;
		}
		if (!(obj->m.shortcuttype[1] == m.shortcuttype[1])) {
			m.shortcuttype[1] = obj->m.shortcuttype[1];
			updated = true;
		}
		if (!(obj->m.shortcuttype[2] == m.shortcuttype[2])) {
			m.shortcuttype[2] = obj->m.shortcuttype[2];
			updated = true;
		}
		if (!(obj->m.shortcuttype[3] == m.shortcuttype[3])) {
			m.shortcuttype[3] = obj->m.shortcuttype[3];
			updated = true;
		}
		if (!(obj->m.shortcuttype[4] == m.shortcuttype[4])) {
			m.shortcuttype[4] = obj->m.shortcuttype[4];
			updated = true;
		}
		if (!(obj->m.shortcuttype[5] == m.shortcuttype[5])) {
			m.shortcuttype[5] = obj->m.shortcuttype[5];
			updated = true;
		}
		if (!(obj->m.shortcuttype[6] == m.shortcuttype[6])) {
			m.shortcuttype[6] = obj->m.shortcuttype[6];
			updated = true;
		}
		if (!(obj->m.shortcuttype[7] == m.shortcuttype[7])) {
			m.shortcuttype[7] = obj->m.shortcuttype[7];
			updated = true;
		}
		if (!(obj->m.shortcuttype[8] == m.shortcuttype[8])) {
			m.shortcuttype[8] = obj->m.shortcuttype[8];
			updated = true;
		}
		if (!(obj->m.shortcuttype[9] == m.shortcuttype[9])) {
			m.shortcuttype[9] = obj->m.shortcuttype[9];
			updated = true;
		}
		if (updated) MarkUpdateAll(shortcutMask, true);
	}
	if (mask.IsBit(PKInfoMask) && !(obj->m.PK_Value == m.PK_Value)) {
		m.PK_Value = obj->m.PK_Value;
		MarkUpdateAll(PKInfoMask, true);
	}
	if (mask.IsBit(orgHairStyleMask) && !(obj->m.org_hairStyle1 == m.org_hairStyle1)) {
		m.org_hairStyle1 = obj->m.org_hairStyle1;
		MarkUpdateAll(orgHairStyleMask, true);
	}
	if (mask.IsBit(orgHairStyleMask) && !(obj->m.org_hairColor == m.org_hairColor)) {
		m.org_hairColor = obj->m.org_hairColor;
		MarkUpdateAll(orgHairStyleMask, true);
	}
	if (mask.IsBit(orgEqClothMask) && !(obj->m.org_eq_cloth == m.org_eq_cloth)) {
		m.org_eq_cloth = obj->m.org_eq_cloth;
		MarkUpdateAll(orgEqClothMask, true);
	}
	if (mask.IsBit(orgEqClothMask) && !(obj->m.org_eq_clothColor == m.org_eq_clothColor)) {
		m.org_eq_clothColor = obj->m.org_eq_clothColor;
		MarkUpdateAll(orgEqClothMask, true);
	}
	if (mask.IsBit(speakCountMask) && !(obj->m.speakCount == m.speakCount)) {
		m.speakCount = obj->m.speakCount;
		MarkUpdateAll(speakCountMask, true);
	}
	if (mask.IsBit(offlineTimeMask) && !(obj->m.offlineTime == m.offlineTime)) {
		m.offlineTime = obj->m.offlineTime;
		MarkUpdateAll(offlineTimeMask, true);
	}
	if (mask.IsBit(TPMask) && !(obj->m.TP == m.TP)) {
		m.TP = obj->m.TP;
		MarkUpdateAll(TPMask, true);
	}
	if (mask.IsBit(speakBlockMask) && !(obj->m.speakBlock == m.speakBlock)) {
		m.speakBlock = obj->m.speakBlock;
		MarkUpdateAll(speakBlockMask, true);
	}
	if (mask.IsBit(hawkerTimeMask) && !(obj->m.hawkerTime == m.hawkerTime)) {
		m.hawkerTime = obj->m.hawkerTime;
		MarkUpdateAll(hawkerTimeMask, true);
	}
	if (mask.IsBit(availBackPackMask) && !(obj->m.availBackPack == m.availBackPack)) {
		m.availBackPack = obj->m.availBackPack;
		MarkUpdateAll(availBackPackMask, true);
	}
	if (mask.IsBit(availWareHouseMask) && !(obj->m.availItemWareHouse == m.availItemWareHouse)) {
		m.availItemWareHouse = obj->m.availItemWareHouse;
		MarkUpdateAll(availWareHouseMask, true);
	}
	if (mask.IsBit(availWareHouseMask) && !(obj->m.availPartnerWareHouse == m.availPartnerWareHouse)) {
		m.availPartnerWareHouse = obj->m.availPartnerWareHouse;
		MarkUpdateAll(availWareHouseMask, true);
	}
	if (mask.IsBit(availWareHouseMask) && !(obj->m.availPetWareHouse == m.availPetWareHouse)) {
		m.availPetWareHouse = obj->m.availPetWareHouse;
		MarkUpdateAll(availWareHouseMask, true);
	}
	if (mask.IsBit(PvPScoreSingleMask) && !(obj->m.PvPScoreSingle == m.PvPScoreSingle)) {
		m.PvPScoreSingle = obj->m.PvPScoreSingle;
		MarkUpdateAll(PvPScoreSingleMask, true);
	}
	if (mask.IsBit(deleteTimeMask) && !(obj->m.delete_time == m.delete_time)) {
		m.delete_time = obj->m.delete_time;
		MarkUpdateAll(deleteTimeMask, true);
	}
	if (mask.IsBit(bankLockMask) && !(obj->m.bankLock == m.bankLock)) {
		m.bankLock = obj->m.bankLock;
		MarkUpdateAll(bankLockMask, true);
	}
	if (mask.IsBit(SiblingIdMask) && !(obj->m.sibling_id == m.sibling_id)) {
		m.sibling_id = obj->m.sibling_id;
		MarkUpdateAll(SiblingIdMask, true);
	}
	if (mask.IsBit(markMask) && !(obj->m.mark == m.mark)) {
		m.mark = obj->m.mark;
		MarkUpdateAll(markMask, true);
	}
	if (mask.IsBit(wantedRewardMask) && !(obj->m.wantedReward == m.wantedReward)) {
		m.wantedReward = obj->m.wantedReward;
		MarkUpdateAll(wantedRewardMask, true);
	}
	if (mask.IsBit(guild_uidMask) && !(obj->m.guild_uid == m.guild_uid)) {
		m.guild_uid = obj->m.guild_uid;
		MarkUpdateAll(guild_uidMask, true);
	}
	if (mask.IsBit(GenerationAccMask) && !(obj->m.iHisGenAcc == m.iHisGenAcc)) {
		m.iHisGenAcc = obj->m.iHisGenAcc;
		MarkUpdateAll(GenerationAccMask, true);
	}
	if (mask.IsBit(GenerationAccMask) && !(obj->m.uiCurGenAcc == m.uiCurGenAcc)) {
		m.uiCurGenAcc = obj->m.uiCurGenAcc;
		MarkUpdateAll(GenerationAccMask, true);
	}
}

void CharacterData_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.acct_id);
		buf.Read<UInt32>(m.char_id);
	}
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<String>(m.nickName);
		buf.Read<UInt8>(m.sex);
		buf.Read<UInt8>(m.cclass);
		buf.Read<UInt8>(m.faction);
	}
	if (m_updateMask.IsBit(levelInfoMask))
	{
		buf.Read<UInt16>(m.level);
	}
	if (m_updateMask.IsBit(expMask))
	{
		buf.Read<UInt32>(m.exp);
	}
	if (m_updateMask.IsBit(attribMask))
	{
		buf.Read<UInt16>(m.STA);
		buf.Read<UInt16>(m.SPR);
		buf.Read<UInt16>(m.STR);
		buf.Read<UInt16>(m.CON);
		buf.Read<UInt16>(m.AGI);
	}
	if (m_updateMask.IsBit(attrPointMask))
	{
		buf.Read<UInt16>(m.attrPoint);
	}
	if (m_updateMask.IsBit(HPMask))
	{
		buf.Read<UInt16>(m.HP);
		buf.Read<UInt16>(m.mHP);
		buf.Read<UInt16>(m.maxHP);
	}
	if (m_updateMask.IsBit(SPMask))
	{
		buf.Read<UInt16>(m.SP);
		buf.Read<UInt16>(m.mSP);
		buf.Read<UInt16>(m.maxSP);
	}
	if (m_updateMask.IsBit(DPMask))
	{
		buf.Read<UInt16>(m.DP);
		buf.Read<UInt16>(m.mDP);
		buf.Read<UInt16>(m.maxDP);
	}
	if (m_updateMask.IsBit(WCMask))
	{
		buf.Read<UInt16>(m.WC);
		buf.Read<UInt16>(m.maxWC);
		buf.Read<UInt16>(m.WcExp);
	}
	if (m_updateMask.IsBit(VPMask))
	{
		buf.Read<UInt16>(m.VP);
		buf.Read<UInt16>(m.maxVP);
	}
	if (m_updateMask.IsBit(goodnessMask))
	{
		buf.Read<Int32>(m.goodness);
	}
	if (m_updateMask.IsBit(honorMask))
	{
		buf.Read<UInt32>(m.honor);
	}
	if (m_updateMask.IsBit(titleMask))
	{
		buf.Read<String>(m.title);
	}
	if (m_updateMask.IsBit(manorMask))
	{
		buf.Read<UInt32>(m.manor);
	}
	if (m_updateMask.IsBit(donateMask))
	{
		buf.Read<UInt32>(m.fDonate);
		buf.Read<UInt32>(m.totalFDonate);
		buf.Read<UInt32>(m.mDonate);
		buf.Read<UInt32>(m.totalMDonate);
	}
	if (m_updateMask.IsBit(moneyMask))
	{
		buf.Read<UInt32>(m.money);
	}
	if (m_updateMask.IsBit(bankMoneyMask))
	{
		buf.Read<UInt32>(m.bankMoney);
	}
	if (m_updateMask.IsBit(SMoneyMask))
	{
		buf.Read<UInt32>(m.SMoney);
	}
	if (m_updateMask.IsBit(partnerMask))
	{
		buf.Read<UInt32>(m.partner);
	}
	if (m_updateMask.IsBit(petMask))
	{
		buf.Read<UInt32>(m.pet);
	}
	if (m_updateMask.IsBit(maxOtherMask))
	{
		buf.Read<UInt8>(m.maxPartner);
		buf.Read<UInt8>(m.maxPet);
	}
	if (m_updateMask.IsBit(mapMask))
	{
		buf.Read<UInt16>(m.map_id);
		buf.Read<UInt16>(m.map_x);
		buf.Read<UInt16>(m.map_y);
		buf.Read<UInt32>(m.map_ownerId);
	}
	if (m_updateMask.IsBit(rebornMask))
	{
		buf.Read<UInt16>(m.reborn_map);
		buf.Read<UInt16>(m.reborn_x);
		buf.Read<UInt16>(m.reborn_y);
	}
	if (m_updateMask.IsBit(styleMask))
	{
		buf.Read<UInt16>(m.photo_id);
		buf.Read<UInt16>(m.hairStyle1);
		buf.Read<UInt16>(m.hairStyle2);
		buf.Read<UInt16>(m.hairColor);
	}
	if (m_updateMask.IsBit(eqWeaponMask))
	{
		buf.Read<UInt32>(m.eq_weapon);
		buf.Read<UInt16>(m.eq_weaponColor);
	}
	if (m_updateMask.IsBit(eqHeadMask))
	{
		buf.Read<UInt32>(m.eq_headBand);
		buf.Read<UInt16>(m.eq_headColor);
	}
	if (m_updateMask.IsBit(eqClothMask))
	{
		buf.Read<UInt32>(m.eq_cloth);
		buf.Read<UInt16>(m.eq_clothColor);
	}
	if (m_updateMask.IsBit(stateMask))
	{
		buf.Read<UInt8>(m.state);
	}
	if (m_updateMask.IsBit(buffMask))
	{
		buf.Read<String>(m.buff);
	}
	if (m_updateMask.IsBit(shortcutMask))
	{
		buf.ReadArray<UInt16>(m.shortcut, 10);
		buf.ReadArray<UInt8>(m.shortcuttype, 10);
	}
	if (m_updateMask.IsBit(PKInfoMask))
	{
		buf.Read<UInt16>(m.PK_Value);
	}
	if (m_updateMask.IsBit(orgHairStyleMask))
	{
		buf.Read<UInt16>(m.org_hairStyle1);
		buf.Read<UInt16>(m.org_hairColor);
	}
	if (m_updateMask.IsBit(orgEqClothMask))
	{
		buf.Read<UInt32>(m.org_eq_cloth);
		buf.Read<UInt16>(m.org_eq_clothColor);
	}
	if (m_updateMask.IsBit(speakCountMask))
	{
		buf.Read<UInt16>(m.speakCount);
	}
	if (m_updateMask.IsBit(offlineTimeMask))
	{
		buf.Read<UInt32>(m.offlineTime);
	}
	if (m_updateMask.IsBit(TPMask))
	{
		buf.Read<UInt16>(m.TP);
	}
	if (m_updateMask.IsBit(speakBlockMask))
	{
		buf.Read<UInt32>(m.speakBlock);
	}
	if (m_updateMask.IsBit(hawkerTimeMask))
	{
		buf.Read<UInt32>(m.hawkerTime);
	}
	if (m_updateMask.IsBit(availBackPackMask))
	{
		buf.Read<UInt8>(m.availBackPack);
	}
	if (m_updateMask.IsBit(availWareHouseMask))
	{
		buf.Read<UInt8>(m.availItemWareHouse);
		buf.Read<UInt8>(m.availPartnerWareHouse);
		buf.Read<UInt8>(m.availPetWareHouse);
	}
	if (m_updateMask.IsBit(PvPScoreSingleMask))
	{
		buf.Read<UInt32>(m.PvPScoreSingle);
	}
	if (m_updateMask.IsBit(deleteTimeMask))
	{
		buf.Read<UInt32>(m.delete_time);
	}
	if (m_updateMask.IsBit(bankLockMask))
	{
		buf.Read<Boolean>(m.bankLock);
	}
	if (m_updateMask.IsBit(SiblingIdMask))
	{
		buf.Read<UInt32>(m.sibling_id);
	}
	if (m_updateMask.IsBit(markMask))
	{
		buf.Read<UInt32>(m.mark);
	}
	if (m_updateMask.IsBit(wantedRewardMask))
	{
		buf.Read<UInt32>(m.wantedReward);
	}
	if (m_updateMask.IsBit(guild_uidMask))
	{
		buf.Read<UInt32>(m.guild_uid);
	}
	if (m_updateMask.IsBit(GenerationAccMask))
	{
		buf.Read<UInt32>(m.iHisGenAcc);
		buf.Read<UInt32>(m.uiCurGenAcc);
	}
}

void CharacterData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.acct_id);
		buf.Write<UInt32>(m.char_id);
	}
	if (mask.IsBit(infoMask))
	{
		buf.Write<String>(m.nickName);
		buf.Write<UInt8>(m.sex);
		buf.Write<UInt8>(m.cclass);
		buf.Write<UInt8>(m.faction);
	}
	if (mask.IsBit(levelInfoMask))
	{
		buf.Write<UInt16>(m.level);
	}
	if (mask.IsBit(expMask))
	{
		buf.Write<UInt32>(m.exp);
	}
	if (mask.IsBit(attribMask))
	{
		buf.Write<UInt16>(m.STA);
		buf.Write<UInt16>(m.SPR);
		buf.Write<UInt16>(m.STR);
		buf.Write<UInt16>(m.CON);
		buf.Write<UInt16>(m.AGI);
	}
	if (mask.IsBit(attrPointMask))
	{
		buf.Write<UInt16>(m.attrPoint);
	}
	if (mask.IsBit(HPMask))
	{
		buf.Write<UInt16>(m.HP);
		buf.Write<UInt16>(m.mHP);
		buf.Write<UInt16>(m.maxHP);
	}
	if (mask.IsBit(SPMask))
	{
		buf.Write<UInt16>(m.SP);
		buf.Write<UInt16>(m.mSP);
		buf.Write<UInt16>(m.maxSP);
	}
	if (mask.IsBit(DPMask))
	{
		buf.Write<UInt16>(m.DP);
		buf.Write<UInt16>(m.mDP);
		buf.Write<UInt16>(m.maxDP);
	}
	if (mask.IsBit(WCMask))
	{
		buf.Write<UInt16>(m.WC);
		buf.Write<UInt16>(m.maxWC);
		buf.Write<UInt16>(m.WcExp);
	}
	if (mask.IsBit(VPMask))
	{
		buf.Write<UInt16>(m.VP);
		buf.Write<UInt16>(m.maxVP);
	}
	if (mask.IsBit(goodnessMask))
	{
		buf.Write<Int32>(m.goodness);
	}
	if (mask.IsBit(honorMask))
	{
		buf.Write<UInt32>(m.honor);
	}
	if (mask.IsBit(titleMask))
	{
		buf.Write<String>(m.title);
	}
	if (mask.IsBit(manorMask))
	{
		buf.Write<UInt32>(m.manor);
	}
	if (mask.IsBit(donateMask))
	{
		buf.Write<UInt32>(m.fDonate);
		buf.Write<UInt32>(m.totalFDonate);
		buf.Write<UInt32>(m.mDonate);
		buf.Write<UInt32>(m.totalMDonate);
	}
	if (mask.IsBit(moneyMask))
	{
		buf.Write<UInt32>(m.money);
	}
	if (mask.IsBit(bankMoneyMask))
	{
		buf.Write<UInt32>(m.bankMoney);
	}
	if (mask.IsBit(SMoneyMask))
	{
		buf.Write<UInt32>(m.SMoney);
	}
	if (mask.IsBit(partnerMask))
	{
		buf.Write<UInt32>(m.partner);
	}
	if (mask.IsBit(petMask))
	{
		buf.Write<UInt32>(m.pet);
	}
	if (mask.IsBit(maxOtherMask))
	{
		buf.Write<UInt8>(m.maxPartner);
		buf.Write<UInt8>(m.maxPet);
	}
	if (mask.IsBit(mapMask))
	{
		buf.Write<UInt16>(m.map_id);
		buf.Write<UInt16>(m.map_x);
		buf.Write<UInt16>(m.map_y);
		buf.Write<UInt32>(m.map_ownerId);
	}
	if (mask.IsBit(rebornMask))
	{
		buf.Write<UInt16>(m.reborn_map);
		buf.Write<UInt16>(m.reborn_x);
		buf.Write<UInt16>(m.reborn_y);
	}
	if (mask.IsBit(styleMask))
	{
		buf.Write<UInt16>(m.photo_id);
		buf.Write<UInt16>(m.hairStyle1);
		buf.Write<UInt16>(m.hairStyle2);
		buf.Write<UInt16>(m.hairColor);
	}
	if (mask.IsBit(eqWeaponMask))
	{
		buf.Write<UInt32>(m.eq_weapon);
		buf.Write<UInt16>(m.eq_weaponColor);
	}
	if (mask.IsBit(eqHeadMask))
	{
		buf.Write<UInt32>(m.eq_headBand);
		buf.Write<UInt16>(m.eq_headColor);
	}
	if (mask.IsBit(eqClothMask))
	{
		buf.Write<UInt32>(m.eq_cloth);
		buf.Write<UInt16>(m.eq_clothColor);
	}
	if (mask.IsBit(stateMask))
	{
		buf.Write<UInt8>(m.state);
	}
	if (mask.IsBit(buffMask))
	{
		buf.Write<String>(m.buff);
	}
	if (mask.IsBit(shortcutMask))
	{
		buf.WriteArray<UInt16>(m.shortcut, 10);
		buf.WriteArray<UInt8>(m.shortcuttype, 10);
	}
	if (mask.IsBit(PKInfoMask))
	{
		buf.Write<UInt16>(m.PK_Value);
	}
	if (mask.IsBit(orgHairStyleMask))
	{
		buf.Write<UInt16>(m.org_hairStyle1);
		buf.Write<UInt16>(m.org_hairColor);
	}
	if (mask.IsBit(orgEqClothMask))
	{
		buf.Write<UInt32>(m.org_eq_cloth);
		buf.Write<UInt16>(m.org_eq_clothColor);
	}
	if (mask.IsBit(speakCountMask))
	{
		buf.Write<UInt16>(m.speakCount);
	}
	if (mask.IsBit(offlineTimeMask))
	{
		buf.Write<UInt32>(m.offlineTime);
	}
	if (mask.IsBit(TPMask))
	{
		buf.Write<UInt16>(m.TP);
	}
	if (mask.IsBit(speakBlockMask))
	{
		buf.Write<UInt32>(m.speakBlock);
	}
	if (mask.IsBit(hawkerTimeMask))
	{
		buf.Write<UInt32>(m.hawkerTime);
	}
	if (mask.IsBit(availBackPackMask))
	{
		buf.Write<UInt8>(m.availBackPack);
	}
	if (mask.IsBit(availWareHouseMask))
	{
		buf.Write<UInt8>(m.availItemWareHouse);
		buf.Write<UInt8>(m.availPartnerWareHouse);
		buf.Write<UInt8>(m.availPetWareHouse);
	}
	if (mask.IsBit(PvPScoreSingleMask))
	{
		buf.Write<UInt32>(m.PvPScoreSingle);
	}
	if (mask.IsBit(deleteTimeMask))
	{
		buf.Write<UInt32>(m.delete_time);
	}
	if (mask.IsBit(bankLockMask))
	{
		buf.Write<Boolean>(m.bankLock);
	}
	if (mask.IsBit(SiblingIdMask))
	{
		buf.Write<UInt32>(m.sibling_id);
	}
	if (mask.IsBit(markMask))
	{
		buf.Write<UInt32>(m.mark);
	}
	if (mask.IsBit(wantedRewardMask))
	{
		buf.Write<UInt32>(m.wantedReward);
	}
	if (mask.IsBit(guild_uidMask))
	{
		buf.Write<UInt32>(m.guild_uid);
	}
	if (mask.IsBit(GenerationAccMask))
	{
		buf.Write<UInt32>(m.iHisGenAcc);
		buf.Write<UInt32>(m.uiCurGenAcc);
	}
}

RPCResult CharacterData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_CharacterData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult CharacterData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_CharacterData_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

