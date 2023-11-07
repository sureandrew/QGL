//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/CharacterData_proxy.h"

void CharacterData_Proxy::SetId(const UInt32 &acct_id, const UInt32 &char_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void CharacterData_Proxy::SetAcct_id(const UInt32 &acct_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void CharacterData_Proxy::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void CharacterData_Proxy::SetInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &faction)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	if (m.sex != sex) { m.sex = sex; modified = true; }
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterData_Proxy::SetNickName(const String &nickName)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterData_Proxy::SetSex(const UInt8 &sex)
{
	bool modified = false;
	if (m.sex != sex) { m.sex = sex; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterData_Proxy::SetCclass(const UInt8 &cclass)
{
	bool modified = false;
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterData_Proxy::SetFaction(const UInt8 &faction)
{
	bool modified = false;
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterData_Proxy::SetLevelInfo(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelInfoMask, modified);
}

void CharacterData_Proxy::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelInfoMask, modified);
}

void CharacterData_Proxy::SetExp(const UInt32 &exp)
{
	bool modified = false;
	if (m.exp != exp) { m.exp = exp; modified = true; }
	MarkUpdateAll(expMask, modified);
}

void CharacterData_Proxy::SetAttrib(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI)
{
	bool modified = false;
	if (m.STA != STA) { m.STA = STA; modified = true; }
	if (m.SPR != SPR) { m.SPR = SPR; modified = true; }
	if (m.STR != STR) { m.STR = STR; modified = true; }
	if (m.CON != CON) { m.CON = CON; modified = true; }
	if (m.AGI != AGI) { m.AGI = AGI; modified = true; }
	MarkUpdateAll(attribMask, modified);
}

void CharacterData_Proxy::SetSTA(const UInt16 &STA)
{
	bool modified = false;
	if (m.STA != STA) { m.STA = STA; modified = true; }
	MarkUpdateAll(attribMask, modified);
}

void CharacterData_Proxy::SetSPR(const UInt16 &SPR)
{
	bool modified = false;
	if (m.SPR != SPR) { m.SPR = SPR; modified = true; }
	MarkUpdateAll(attribMask, modified);
}

void CharacterData_Proxy::SetSTR(const UInt16 &STR)
{
	bool modified = false;
	if (m.STR != STR) { m.STR = STR; modified = true; }
	MarkUpdateAll(attribMask, modified);
}

void CharacterData_Proxy::SetCON(const UInt16 &CON)
{
	bool modified = false;
	if (m.CON != CON) { m.CON = CON; modified = true; }
	MarkUpdateAll(attribMask, modified);
}

void CharacterData_Proxy::SetAGI(const UInt16 &AGI)
{
	bool modified = false;
	if (m.AGI != AGI) { m.AGI = AGI; modified = true; }
	MarkUpdateAll(attribMask, modified);
}

void CharacterData_Proxy::SetAttrPoint(const UInt16 &attrPoint)
{
	bool modified = false;
	if (m.attrPoint != attrPoint) { m.attrPoint = attrPoint; modified = true; }
	MarkUpdateAll(attrPointMask, modified);
}

void CharacterData_Proxy::SetHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP)
{
	bool modified = false;
	if (m.HP != HP) { m.HP = HP; modified = true; }
	if (m.mHP != mHP) { m.mHP = mHP; modified = true; }
	if (m.maxHP != maxHP) { m.maxHP = maxHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void CharacterData_Proxy::SetHP(const UInt16 &HP)
{
	bool modified = false;
	if (m.HP != HP) { m.HP = HP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void CharacterData_Proxy::SetMHP(const UInt16 &mHP)
{
	bool modified = false;
	if (m.mHP != mHP) { m.mHP = mHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void CharacterData_Proxy::SetMaxHP(const UInt16 &maxHP)
{
	bool modified = false;
	if (m.maxHP != maxHP) { m.maxHP = maxHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void CharacterData_Proxy::SetSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP)
{
	bool modified = false;
	if (m.SP != SP) { m.SP = SP; modified = true; }
	if (m.mSP != mSP) { m.mSP = mSP; modified = true; }
	if (m.maxSP != maxSP) { m.maxSP = maxSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void CharacterData_Proxy::SetSP(const UInt16 &SP)
{
	bool modified = false;
	if (m.SP != SP) { m.SP = SP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void CharacterData_Proxy::SetMSP(const UInt16 &mSP)
{
	bool modified = false;
	if (m.mSP != mSP) { m.mSP = mSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void CharacterData_Proxy::SetMaxSP(const UInt16 &maxSP)
{
	bool modified = false;
	if (m.maxSP != maxSP) { m.maxSP = maxSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void CharacterData_Proxy::SetDP(const UInt16 &DP, const UInt16 &mDP, const UInt16 &maxDP)
{
	bool modified = false;
	if (m.DP != DP) { m.DP = DP; modified = true; }
	if (m.mDP != mDP) { m.mDP = mDP; modified = true; }
	if (m.maxDP != maxDP) { m.maxDP = maxDP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void CharacterData_Proxy::SetDP(const UInt16 &DP)
{
	bool modified = false;
	if (m.DP != DP) { m.DP = DP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void CharacterData_Proxy::SetMDP(const UInt16 &mDP)
{
	bool modified = false;
	if (m.mDP != mDP) { m.mDP = mDP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void CharacterData_Proxy::SetMaxDP(const UInt16 &maxDP)
{
	bool modified = false;
	if (m.maxDP != maxDP) { m.maxDP = maxDP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void CharacterData_Proxy::SetWC(const UInt16 &WC, const UInt16 &maxWC, const UInt16 &WcExp)
{
	bool modified = false;
	if (m.WC != WC) { m.WC = WC; modified = true; }
	if (m.maxWC != maxWC) { m.maxWC = maxWC; modified = true; }
	if (m.WcExp != WcExp) { m.WcExp = WcExp; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void CharacterData_Proxy::SetWC(const UInt16 &WC)
{
	bool modified = false;
	if (m.WC != WC) { m.WC = WC; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void CharacterData_Proxy::SetMaxWC(const UInt16 &maxWC)
{
	bool modified = false;
	if (m.maxWC != maxWC) { m.maxWC = maxWC; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void CharacterData_Proxy::SetWcExp(const UInt16 &WcExp)
{
	bool modified = false;
	if (m.WcExp != WcExp) { m.WcExp = WcExp; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void CharacterData_Proxy::SetVP(const UInt16 &VP, const UInt16 &maxVP)
{
	bool modified = false;
	if (m.VP != VP) { m.VP = VP; modified = true; }
	if (m.maxVP != maxVP) { m.maxVP = maxVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void CharacterData_Proxy::SetVP(const UInt16 &VP)
{
	bool modified = false;
	if (m.VP != VP) { m.VP = VP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void CharacterData_Proxy::SetMaxVP(const UInt16 &maxVP)
{
	bool modified = false;
	if (m.maxVP != maxVP) { m.maxVP = maxVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void CharacterData_Proxy::SetGoodness(const Int32 &goodness)
{
	bool modified = false;
	if (m.goodness != goodness) { m.goodness = goodness; modified = true; }
	MarkUpdateAll(goodnessMask, modified);
}

void CharacterData_Proxy::SetHonor(const UInt32 &honor)
{
	bool modified = false;
	if (m.honor != honor) { m.honor = honor; modified = true; }
	MarkUpdateAll(honorMask, modified);
}

void CharacterData_Proxy::SetTitle(const String &title)
{
	bool modified = false;
	if (m.title != title) { m.title = title; modified = true; }
	MarkUpdateAll(titleMask, modified);
}

void CharacterData_Proxy::SetManor(const UInt32 &manor)
{
	bool modified = false;
	if (m.manor != manor) { m.manor = manor; modified = true; }
	MarkUpdateAll(manorMask, modified);
}

void CharacterData_Proxy::SetDonate(const UInt32 &fDonate, const UInt32 &totalFDonate, const UInt32 &mDonate, const UInt32 &totalMDonate)
{
	bool modified = false;
	if (m.fDonate != fDonate) { m.fDonate = fDonate; modified = true; }
	if (m.totalFDonate != totalFDonate) { m.totalFDonate = totalFDonate; modified = true; }
	if (m.mDonate != mDonate) { m.mDonate = mDonate; modified = true; }
	if (m.totalMDonate != totalMDonate) { m.totalMDonate = totalMDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void CharacterData_Proxy::SetFDonate(const UInt32 &fDonate)
{
	bool modified = false;
	if (m.fDonate != fDonate) { m.fDonate = fDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void CharacterData_Proxy::SetTotalFDonate(const UInt32 &totalFDonate)
{
	bool modified = false;
	if (m.totalFDonate != totalFDonate) { m.totalFDonate = totalFDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void CharacterData_Proxy::SetMDonate(const UInt32 &mDonate)
{
	bool modified = false;
	if (m.mDonate != mDonate) { m.mDonate = mDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void CharacterData_Proxy::SetTotalMDonate(const UInt32 &totalMDonate)
{
	bool modified = false;
	if (m.totalMDonate != totalMDonate) { m.totalMDonate = totalMDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void CharacterData_Proxy::SetMoney(const UInt32 &money)
{
	bool modified = false;
	if (m.money != money) { m.money = money; modified = true; }
	MarkUpdateAll(moneyMask, modified);
}

void CharacterData_Proxy::SetBankMoney(const UInt32 &bankMoney)
{
	bool modified = false;
	if (m.bankMoney != bankMoney) { m.bankMoney = bankMoney; modified = true; }
	MarkUpdateAll(bankMoneyMask, modified);
}

void CharacterData_Proxy::SetSMoney(const UInt32 &SMoney)
{
	bool modified = false;
	if (m.SMoney != SMoney) { m.SMoney = SMoney; modified = true; }
	MarkUpdateAll(SMoneyMask, modified);
}

void CharacterData_Proxy::SetPartner(const UInt32 &partner)
{
	bool modified = false;
	if (m.partner != partner) { m.partner = partner; modified = true; }
	MarkUpdateAll(partnerMask, modified);
}

void CharacterData_Proxy::SetPet(const UInt32 &pet)
{
	bool modified = false;
	if (m.pet != pet) { m.pet = pet; modified = true; }
	MarkUpdateAll(petMask, modified);
}

void CharacterData_Proxy::SetMaxOther(const UInt8 &maxPartner, const UInt8 &maxPet)
{
	bool modified = false;
	if (m.maxPartner != maxPartner) { m.maxPartner = maxPartner; modified = true; }
	if (m.maxPet != maxPet) { m.maxPet = maxPet; modified = true; }
	MarkUpdateAll(maxOtherMask, modified);
}

void CharacterData_Proxy::SetMaxPartner(const UInt8 &maxPartner)
{
	bool modified = false;
	if (m.maxPartner != maxPartner) { m.maxPartner = maxPartner; modified = true; }
	MarkUpdateAll(maxOtherMask, modified);
}

void CharacterData_Proxy::SetMaxPet(const UInt8 &maxPet)
{
	bool modified = false;
	if (m.maxPet != maxPet) { m.maxPet = maxPet; modified = true; }
	MarkUpdateAll(maxOtherMask, modified);
}

void CharacterData_Proxy::SetMap(const UInt16 &map_id, const UInt16 &map_x, const UInt16 &map_y, const UInt32 &map_ownerId)
{
	bool modified = false;
	if (m.map_id != map_id) { m.map_id = map_id; modified = true; }
	if (m.map_x != map_x) { m.map_x = map_x; modified = true; }
	if (m.map_y != map_y) { m.map_y = map_y; modified = true; }
	if (m.map_ownerId != map_ownerId) { m.map_ownerId = map_ownerId; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterData_Proxy::SetMap_id(const UInt16 &map_id)
{
	bool modified = false;
	if (m.map_id != map_id) { m.map_id = map_id; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterData_Proxy::SetMap_x(const UInt16 &map_x)
{
	bool modified = false;
	if (m.map_x != map_x) { m.map_x = map_x; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterData_Proxy::SetMap_y(const UInt16 &map_y)
{
	bool modified = false;
	if (m.map_y != map_y) { m.map_y = map_y; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterData_Proxy::SetMap_ownerId(const UInt32 &map_ownerId)
{
	bool modified = false;
	if (m.map_ownerId != map_ownerId) { m.map_ownerId = map_ownerId; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterData_Proxy::SetReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y)
{
	bool modified = false;
	if (m.reborn_map != reborn_map) { m.reborn_map = reborn_map; modified = true; }
	if (m.reborn_x != reborn_x) { m.reborn_x = reborn_x; modified = true; }
	if (m.reborn_y != reborn_y) { m.reborn_y = reborn_y; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void CharacterData_Proxy::SetReborn_map(const UInt16 &reborn_map)
{
	bool modified = false;
	if (m.reborn_map != reborn_map) { m.reborn_map = reborn_map; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void CharacterData_Proxy::SetReborn_x(const UInt16 &reborn_x)
{
	bool modified = false;
	if (m.reborn_x != reborn_x) { m.reborn_x = reborn_x; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void CharacterData_Proxy::SetReborn_y(const UInt16 &reborn_y)
{
	bool modified = false;
	if (m.reborn_y != reborn_y) { m.reborn_y = reborn_y; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void CharacterData_Proxy::SetStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor)
{
	bool modified = false;
	if (m.photo_id != photo_id) { m.photo_id = photo_id; modified = true; }
	if (m.hairStyle1 != hairStyle1) { m.hairStyle1 = hairStyle1; modified = true; }
	if (m.hairStyle2 != hairStyle2) { m.hairStyle2 = hairStyle2; modified = true; }
	if (m.hairColor != hairColor) { m.hairColor = hairColor; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterData_Proxy::SetPhoto_id(const UInt16 &photo_id)
{
	bool modified = false;
	if (m.photo_id != photo_id) { m.photo_id = photo_id; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterData_Proxy::SetHairStyle1(const UInt16 &hairStyle1)
{
	bool modified = false;
	if (m.hairStyle1 != hairStyle1) { m.hairStyle1 = hairStyle1; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterData_Proxy::SetHairStyle2(const UInt16 &hairStyle2)
{
	bool modified = false;
	if (m.hairStyle2 != hairStyle2) { m.hairStyle2 = hairStyle2; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterData_Proxy::SetHairColor(const UInt16 &hairColor)
{
	bool modified = false;
	if (m.hairColor != hairColor) { m.hairColor = hairColor; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterData_Proxy::SetEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor)
{
	bool modified = false;
	if (m.eq_weapon != eq_weapon) { m.eq_weapon = eq_weapon; modified = true; }
	if (m.eq_weaponColor != eq_weaponColor) { m.eq_weaponColor = eq_weaponColor; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void CharacterData_Proxy::SetEq_weapon(const UInt32 &eq_weapon)
{
	bool modified = false;
	if (m.eq_weapon != eq_weapon) { m.eq_weapon = eq_weapon; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void CharacterData_Proxy::SetEq_weaponColor(const UInt16 &eq_weaponColor)
{
	bool modified = false;
	if (m.eq_weaponColor != eq_weaponColor) { m.eq_weaponColor = eq_weaponColor; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void CharacterData_Proxy::SetEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor)
{
	bool modified = false;
	if (m.eq_headBand != eq_headBand) { m.eq_headBand = eq_headBand; modified = true; }
	if (m.eq_headColor != eq_headColor) { m.eq_headColor = eq_headColor; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void CharacterData_Proxy::SetEq_headBand(const UInt32 &eq_headBand)
{
	bool modified = false;
	if (m.eq_headBand != eq_headBand) { m.eq_headBand = eq_headBand; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void CharacterData_Proxy::SetEq_headColor(const UInt16 &eq_headColor)
{
	bool modified = false;
	if (m.eq_headColor != eq_headColor) { m.eq_headColor = eq_headColor; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void CharacterData_Proxy::SetEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor)
{
	bool modified = false;
	if (m.eq_cloth != eq_cloth) { m.eq_cloth = eq_cloth; modified = true; }
	if (m.eq_clothColor != eq_clothColor) { m.eq_clothColor = eq_clothColor; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void CharacterData_Proxy::SetEq_cloth(const UInt32 &eq_cloth)
{
	bool modified = false;
	if (m.eq_cloth != eq_cloth) { m.eq_cloth = eq_cloth; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void CharacterData_Proxy::SetEq_clothColor(const UInt16 &eq_clothColor)
{
	bool modified = false;
	if (m.eq_clothColor != eq_clothColor) { m.eq_clothColor = eq_clothColor; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void CharacterData_Proxy::SetState(const UInt8 &state)
{
	bool modified = false;
	if (m.state != state) { m.state = state; modified = true; }
	MarkUpdateAll(stateMask, modified);
}

void CharacterData_Proxy::SetBuff(const String &buff)
{
	bool modified = false;
	if (m.buff != buff) { m.buff = buff; modified = true; }
	MarkUpdateAll(buffMask, modified);
}

void CharacterData_Proxy::SetShortcut(const UInt16 &shortcut0, const UInt16 &shortcut1, const UInt16 &shortcut2, const UInt16 &shortcut3, const UInt16 &shortcut4, const UInt16 &shortcut5, const UInt16 &shortcut6, const UInt16 &shortcut7, const UInt16 &shortcut8, const UInt16 &shortcut9, const UInt8 &shortcuttype0, const UInt8 &shortcuttype1, const UInt8 &shortcuttype2, const UInt8 &shortcuttype3, const UInt8 &shortcuttype4, const UInt8 &shortcuttype5, const UInt8 &shortcuttype6, const UInt8 &shortcuttype7, const UInt8 &shortcuttype8, const UInt8 &shortcuttype9)
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

void CharacterData_Proxy::SetShortcut(const UInt8 i, const UInt16 &shortcut)
{
	bool modified = false;
	if (m.shortcut[i] != shortcut) { m.shortcut[i] = shortcut; modified = true; }
	MarkUpdateAll(shortcutMask, modified);
}

void CharacterData_Proxy::SetShortcuttype(const UInt8 i, const UInt8 &shortcuttype)
{
	bool modified = false;
	if (m.shortcuttype[i] != shortcuttype) { m.shortcuttype[i] = shortcuttype; modified = true; }
	MarkUpdateAll(shortcutMask, modified);
}

void CharacterData_Proxy::SetPKInfo(const UInt16 &PK_Value)
{
	bool modified = false;
	if (m.PK_Value != PK_Value) { m.PK_Value = PK_Value; modified = true; }
	MarkUpdateAll(PKInfoMask, modified);
}

void CharacterData_Proxy::SetPK_Value(const UInt16 &PK_Value)
{
	bool modified = false;
	if (m.PK_Value != PK_Value) { m.PK_Value = PK_Value; modified = true; }
	MarkUpdateAll(PKInfoMask, modified);
}

void CharacterData_Proxy::SetOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor)
{
	bool modified = false;
	if (m.org_hairStyle1 != org_hairStyle1) { m.org_hairStyle1 = org_hairStyle1; modified = true; }
	if (m.org_hairColor != org_hairColor) { m.org_hairColor = org_hairColor; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void CharacterData_Proxy::SetOrg_hairStyle1(const UInt16 &org_hairStyle1)
{
	bool modified = false;
	if (m.org_hairStyle1 != org_hairStyle1) { m.org_hairStyle1 = org_hairStyle1; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void CharacterData_Proxy::SetOrg_hairColor(const UInt16 &org_hairColor)
{
	bool modified = false;
	if (m.org_hairColor != org_hairColor) { m.org_hairColor = org_hairColor; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void CharacterData_Proxy::SetOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor)
{
	bool modified = false;
	if (m.org_eq_cloth != org_eq_cloth) { m.org_eq_cloth = org_eq_cloth; modified = true; }
	if (m.org_eq_clothColor != org_eq_clothColor) { m.org_eq_clothColor = org_eq_clothColor; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void CharacterData_Proxy::SetOrg_eq_cloth(const UInt32 &org_eq_cloth)
{
	bool modified = false;
	if (m.org_eq_cloth != org_eq_cloth) { m.org_eq_cloth = org_eq_cloth; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void CharacterData_Proxy::SetOrg_eq_clothColor(const UInt16 &org_eq_clothColor)
{
	bool modified = false;
	if (m.org_eq_clothColor != org_eq_clothColor) { m.org_eq_clothColor = org_eq_clothColor; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void CharacterData_Proxy::SetSpeakCount(const UInt16 &speakCount)
{
	bool modified = false;
	if (m.speakCount != speakCount) { m.speakCount = speakCount; modified = true; }
	MarkUpdateAll(speakCountMask, modified);
}

void CharacterData_Proxy::SetOfflineTime(const UInt32 &offlineTime)
{
	bool modified = false;
	if (m.offlineTime != offlineTime) { m.offlineTime = offlineTime; modified = true; }
	MarkUpdateAll(offlineTimeMask, modified);
}

void CharacterData_Proxy::SetTP(const UInt16 &TP)
{
	bool modified = false;
	if (m.TP != TP) { m.TP = TP; modified = true; }
	MarkUpdateAll(TPMask, modified);
}

void CharacterData_Proxy::SetSpeakBlock(const UInt32 &speakBlock)
{
	bool modified = false;
	if (m.speakBlock != speakBlock) { m.speakBlock = speakBlock; modified = true; }
	MarkUpdateAll(speakBlockMask, modified);
}

void CharacterData_Proxy::SetHawkerTime(const UInt32 &hawkerTime)
{
	bool modified = false;
	if (m.hawkerTime != hawkerTime) { m.hawkerTime = hawkerTime; modified = true; }
	MarkUpdateAll(hawkerTimeMask, modified);
}

void CharacterData_Proxy::SetAvailBackPack(const UInt8 &availBackPack)
{
	bool modified = false;
	if (m.availBackPack != availBackPack) { m.availBackPack = availBackPack; modified = true; }
	MarkUpdateAll(availBackPackMask, modified);
}

void CharacterData_Proxy::SetAvailWareHouse(const UInt8 &availItemWareHouse, const UInt8 &availPartnerWareHouse, const UInt8 &availPetWareHouse)
{
	bool modified = false;
	if (m.availItemWareHouse != availItemWareHouse) { m.availItemWareHouse = availItemWareHouse; modified = true; }
	if (m.availPartnerWareHouse != availPartnerWareHouse) { m.availPartnerWareHouse = availPartnerWareHouse; modified = true; }
	if (m.availPetWareHouse != availPetWareHouse) { m.availPetWareHouse = availPetWareHouse; modified = true; }
	MarkUpdateAll(availWareHouseMask, modified);
}

void CharacterData_Proxy::SetAvailItemWareHouse(const UInt8 &availItemWareHouse)
{
	bool modified = false;
	if (m.availItemWareHouse != availItemWareHouse) { m.availItemWareHouse = availItemWareHouse; modified = true; }
	MarkUpdateAll(availWareHouseMask, modified);
}

void CharacterData_Proxy::SetAvailPartnerWareHouse(const UInt8 &availPartnerWareHouse)
{
	bool modified = false;
	if (m.availPartnerWareHouse != availPartnerWareHouse) { m.availPartnerWareHouse = availPartnerWareHouse; modified = true; }
	MarkUpdateAll(availWareHouseMask, modified);
}

void CharacterData_Proxy::SetAvailPetWareHouse(const UInt8 &availPetWareHouse)
{
	bool modified = false;
	if (m.availPetWareHouse != availPetWareHouse) { m.availPetWareHouse = availPetWareHouse; modified = true; }
	MarkUpdateAll(availWareHouseMask, modified);
}

void CharacterData_Proxy::SetPvPScoreSingle(const UInt32 &PvPScoreSingle)
{
	bool modified = false;
	if (m.PvPScoreSingle != PvPScoreSingle) { m.PvPScoreSingle = PvPScoreSingle; modified = true; }
	MarkUpdateAll(PvPScoreSingleMask, modified);
}

void CharacterData_Proxy::SetDeleteTime(const UInt32 &delete_time)
{
	bool modified = false;
	if (m.delete_time != delete_time) { m.delete_time = delete_time; modified = true; }
	MarkUpdateAll(deleteTimeMask, modified);
}

void CharacterData_Proxy::SetDelete_time(const UInt32 &delete_time)
{
	bool modified = false;
	if (m.delete_time != delete_time) { m.delete_time = delete_time; modified = true; }
	MarkUpdateAll(deleteTimeMask, modified);
}

void CharacterData_Proxy::SetBankLock(const Boolean &bankLock)
{
	bool modified = false;
	if (m.bankLock != bankLock) { m.bankLock = bankLock; modified = true; }
	MarkUpdateAll(bankLockMask, modified);
}

void CharacterData_Proxy::SetSiblingId(const UInt32 &sibling_id)
{
	bool modified = false;
	if (m.sibling_id != sibling_id) { m.sibling_id = sibling_id; modified = true; }
	MarkUpdateAll(SiblingIdMask, modified);
}

void CharacterData_Proxy::SetSibling_id(const UInt32 &sibling_id)
{
	bool modified = false;
	if (m.sibling_id != sibling_id) { m.sibling_id = sibling_id; modified = true; }
	MarkUpdateAll(SiblingIdMask, modified);
}

void CharacterData_Proxy::SetMark(const UInt32 &mark)
{
	bool modified = false;
	if (m.mark != mark) { m.mark = mark; modified = true; }
	MarkUpdateAll(markMask, modified);
}

void CharacterData_Proxy::SetWantedReward(const UInt32 &wantedReward)
{
	bool modified = false;
	if (m.wantedReward != wantedReward) { m.wantedReward = wantedReward; modified = true; }
	MarkUpdateAll(wantedRewardMask, modified);
}

void CharacterData_Proxy::SetGuild_uid(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(guild_uidMask, modified);
}

void CharacterData_Proxy::SetGenerationAcc(const UInt32 &iHisGenAcc, const UInt32 &uiCurGenAcc)
{
	bool modified = false;
	if (m.iHisGenAcc != iHisGenAcc) { m.iHisGenAcc = iHisGenAcc; modified = true; }
	if (m.uiCurGenAcc != uiCurGenAcc) { m.uiCurGenAcc = uiCurGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}

void CharacterData_Proxy::SetIHisGenAcc(const UInt32 &iHisGenAcc)
{
	bool modified = false;
	if (m.iHisGenAcc != iHisGenAcc) { m.iHisGenAcc = iHisGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}

void CharacterData_Proxy::SetUiCurGenAcc(const UInt32 &uiCurGenAcc)
{
	bool modified = false;
	if (m.uiCurGenAcc != uiCurGenAcc) { m.uiCurGenAcc = uiCurGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}


void CharacterData_Proxy::ReadAttr(CONST Buffer &buf)
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

void CharacterData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

void CharacterData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.acct_id, m.char_id);
	if (IsUpdated(infoMask)) OnUpdateInfo(m.nickName, m.sex, m.cclass, m.faction);
	if (IsUpdated(levelInfoMask)) OnUpdateLevelInfo(m.level);
	if (IsUpdated(expMask)) OnUpdateExp(m.exp);
	if (IsUpdated(attribMask)) OnUpdateAttrib(m.STA, m.SPR, m.STR, m.CON, m.AGI);
	if (IsUpdated(attrPointMask)) OnUpdateAttrPoint(m.attrPoint);
	if (IsUpdated(HPMask)) OnUpdateHP(m.HP, m.mHP, m.maxHP);
	if (IsUpdated(SPMask)) OnUpdateSP(m.SP, m.mSP, m.maxSP);
	if (IsUpdated(DPMask)) OnUpdateDP(m.DP, m.mDP, m.maxDP);
	if (IsUpdated(WCMask)) OnUpdateWC(m.WC, m.maxWC, m.WcExp);
	if (IsUpdated(VPMask)) OnUpdateVP(m.VP, m.maxVP);
	if (IsUpdated(goodnessMask)) OnUpdateGoodness(m.goodness);
	if (IsUpdated(honorMask)) OnUpdateHonor(m.honor);
	if (IsUpdated(titleMask)) OnUpdateTitle(m.title);
	if (IsUpdated(manorMask)) OnUpdateManor(m.manor);
	if (IsUpdated(donateMask)) OnUpdateDonate(m.fDonate, m.totalFDonate, m.mDonate, m.totalMDonate);
	if (IsUpdated(moneyMask)) OnUpdateMoney(m.money);
	if (IsUpdated(bankMoneyMask)) OnUpdateBankMoney(m.bankMoney);
	if (IsUpdated(SMoneyMask)) OnUpdateSMoney(m.SMoney);
	if (IsUpdated(partnerMask)) OnUpdatePartner(m.partner);
	if (IsUpdated(petMask)) OnUpdatePet(m.pet);
	if (IsUpdated(maxOtherMask)) OnUpdateMaxOther(m.maxPartner, m.maxPet);
	if (IsUpdated(mapMask)) OnUpdateMap(m.map_id, m.map_x, m.map_y, m.map_ownerId);
	if (IsUpdated(rebornMask)) OnUpdateReborn(m.reborn_map, m.reborn_x, m.reborn_y);
	if (IsUpdated(styleMask)) OnUpdateStyle(m.photo_id, m.hairStyle1, m.hairStyle2, m.hairColor);
	if (IsUpdated(eqWeaponMask)) OnUpdateEqWeapon(m.eq_weapon, m.eq_weaponColor);
	if (IsUpdated(eqHeadMask)) OnUpdateEqHead(m.eq_headBand, m.eq_headColor);
	if (IsUpdated(eqClothMask)) OnUpdateEqCloth(m.eq_cloth, m.eq_clothColor);
	if (IsUpdated(stateMask)) OnUpdateState(m.state);
	if (IsUpdated(buffMask)) OnUpdateBuff(m.buff);
	if (IsUpdated(shortcutMask)) OnUpdateShortcut(m.shortcut[0], m.shortcut[1], m.shortcut[2], m.shortcut[3], m.shortcut[4], m.shortcut[5], m.shortcut[6], m.shortcut[7], m.shortcut[8], m.shortcut[9], m.shortcuttype[0], m.shortcuttype[1], m.shortcuttype[2], m.shortcuttype[3], m.shortcuttype[4], m.shortcuttype[5], m.shortcuttype[6], m.shortcuttype[7], m.shortcuttype[8], m.shortcuttype[9]);
	if (IsUpdated(PKInfoMask)) OnUpdatePKInfo(m.PK_Value);
	if (IsUpdated(orgHairStyleMask)) OnUpdateOrgHairStyle(m.org_hairStyle1, m.org_hairColor);
	if (IsUpdated(orgEqClothMask)) OnUpdateOrgEqCloth(m.org_eq_cloth, m.org_eq_clothColor);
	if (IsUpdated(speakCountMask)) OnUpdateSpeakCount(m.speakCount);
	if (IsUpdated(offlineTimeMask)) OnUpdateOfflineTime(m.offlineTime);
	if (IsUpdated(TPMask)) OnUpdateTP(m.TP);
	if (IsUpdated(speakBlockMask)) OnUpdateSpeakBlock(m.speakBlock);
	if (IsUpdated(hawkerTimeMask)) OnUpdateHawkerTime(m.hawkerTime);
	if (IsUpdated(availBackPackMask)) OnUpdateAvailBackPack(m.availBackPack);
	if (IsUpdated(availWareHouseMask)) OnUpdateAvailWareHouse(m.availItemWareHouse, m.availPartnerWareHouse, m.availPetWareHouse);
	if (IsUpdated(PvPScoreSingleMask)) OnUpdatePvPScoreSingle(m.PvPScoreSingle);
	if (IsUpdated(deleteTimeMask)) OnUpdateDeleteTime(m.delete_time);
	if (IsUpdated(bankLockMask)) OnUpdateBankLock(m.bankLock);
	if (IsUpdated(SiblingIdMask)) OnUpdateSiblingId(m.sibling_id);
	if (IsUpdated(markMask)) OnUpdateMark(m.mark);
	if (IsUpdated(wantedRewardMask)) OnUpdateWantedReward(m.wantedReward);
	if (IsUpdated(guild_uidMask)) OnUpdateGuild_uid(m.guild_uid);
	if (IsUpdated(GenerationAccMask)) OnUpdateGenerationAcc(m.iHisGenAcc, m.uiCurGenAcc);
}

#pragma optimize("", off)
RPCResult CharacterData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_CharacterData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult CharacterData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_CharacterData_UP_STATE:
			_result = RPCDecode<592655900>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
