//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Character_stub.h"

void Character_Stub::SetId(const UInt32 &acct_id, const UInt32 &char_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void Character_Stub::SetAcct_id(const UInt32 &acct_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void Character_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void Character_Stub::SetLevelInfo(const UInt16 &level, const UInt32 &nextLvExp)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	if (m.nextLvExp != nextLvExp) { m.nextLvExp = nextLvExp; modified = true; }
	MarkUpdateAll(levelInfoMask, modified);
}

void Character_Stub::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelInfoMask, modified);
}

void Character_Stub::SetNextLvExp(const UInt32 &nextLvExp)
{
	bool modified = false;
	if (m.nextLvExp != nextLvExp) { m.nextLvExp = nextLvExp; modified = true; }
	MarkUpdateAll(levelInfoMask, modified);
}

void Character_Stub::SetExp(const UInt32 &exp)
{
	bool modified = false;
	if (m.exp != exp) { m.exp = exp; modified = true; }
	MarkUpdateAll(expMask, modified);
}

void Character_Stub::SetAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI)
{
	bool modified = false;
	if (m.STA != STA) { m.STA = STA; modified = true; }
	if (m.SPR != SPR) { m.SPR = SPR; modified = true; }
	if (m.STR != STR) { m.STR = STR; modified = true; }
	if (m.CON != CON) { m.CON = CON; modified = true; }
	if (m.AGI != AGI) { m.AGI = AGI; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void Character_Stub::SetSTA(const UInt16 &STA)
{
	bool modified = false;
	if (m.STA != STA) { m.STA = STA; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void Character_Stub::SetSPR(const UInt16 &SPR)
{
	bool modified = false;
	if (m.SPR != SPR) { m.SPR = SPR; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void Character_Stub::SetSTR(const UInt16 &STR)
{
	bool modified = false;
	if (m.STR != STR) { m.STR = STR; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void Character_Stub::SetCON(const UInt16 &CON)
{
	bool modified = false;
	if (m.CON != CON) { m.CON = CON; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void Character_Stub::SetAGI(const UInt16 &AGI)
{
	bool modified = false;
	if (m.AGI != AGI) { m.AGI = AGI; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void Character_Stub::SetAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD)
{
	bool modified = false;
	if (m.ATK != ATK) { m.ATK = ATK; modified = true; }
	if (m.DEF != DEF) { m.DEF = DEF; modified = true; }
	if (m.HIT != HIT) { m.HIT = HIT; modified = true; }
	if (m.EVA != EVA) { m.EVA = EVA; modified = true; }
	if (m.POW != POW) { m.POW = POW; modified = true; }
	if (m.SPD != SPD) { m.SPD = SPD; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Character_Stub::SetATK(const UInt16 &ATK)
{
	bool modified = false;
	if (m.ATK != ATK) { m.ATK = ATK; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Character_Stub::SetDEF(const UInt16 &DEF)
{
	bool modified = false;
	if (m.DEF != DEF) { m.DEF = DEF; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Character_Stub::SetHIT(const UInt16 &HIT)
{
	bool modified = false;
	if (m.HIT != HIT) { m.HIT = HIT; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Character_Stub::SetEVA(const UInt16 &EVA)
{
	bool modified = false;
	if (m.EVA != EVA) { m.EVA = EVA; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Character_Stub::SetPOW(const UInt16 &POW)
{
	bool modified = false;
	if (m.POW != POW) { m.POW = POW; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Character_Stub::SetSPD(const UInt16 &SPD)
{
	bool modified = false;
	if (m.SPD != SPD) { m.SPD = SPD; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Character_Stub::SetAttrPoint(const UInt16 &attrPoint)
{
	bool modified = false;
	if (m.attrPoint != attrPoint) { m.attrPoint = attrPoint; modified = true; }
	MarkUpdateAll(attrPointMask, modified);
}

void Character_Stub::SetHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP)
{
	bool modified = false;
	if (m.HP != HP) { m.HP = HP; modified = true; }
	if (m.mHP != mHP) { m.mHP = mHP; modified = true; }
	if (m.maxHP != maxHP) { m.maxHP = maxHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void Character_Stub::SetHP(const UInt16 &HP)
{
	bool modified = false;
	if (m.HP != HP) { m.HP = HP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void Character_Stub::SetMHP(const UInt16 &mHP)
{
	bool modified = false;
	if (m.mHP != mHP) { m.mHP = mHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void Character_Stub::SetMaxHP(const UInt16 &maxHP)
{
	bool modified = false;
	if (m.maxHP != maxHP) { m.maxHP = maxHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void Character_Stub::SetSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP)
{
	bool modified = false;
	if (m.SP != SP) { m.SP = SP; modified = true; }
	if (m.mSP != mSP) { m.mSP = mSP; modified = true; }
	if (m.maxSP != maxSP) { m.maxSP = maxSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void Character_Stub::SetSP(const UInt16 &SP)
{
	bool modified = false;
	if (m.SP != SP) { m.SP = SP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void Character_Stub::SetMSP(const UInt16 &mSP)
{
	bool modified = false;
	if (m.mSP != mSP) { m.mSP = mSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void Character_Stub::SetMaxSP(const UInt16 &maxSP)
{
	bool modified = false;
	if (m.maxSP != maxSP) { m.maxSP = maxSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void Character_Stub::SetDP(const UInt16 &DP, const UInt16 &mDP, const UInt16 &maxDP)
{
	bool modified = false;
	if (m.DP != DP) { m.DP = DP; modified = true; }
	if (m.mDP != mDP) { m.mDP = mDP; modified = true; }
	if (m.maxDP != maxDP) { m.maxDP = maxDP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void Character_Stub::SetDP(const UInt16 &DP)
{
	bool modified = false;
	if (m.DP != DP) { m.DP = DP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void Character_Stub::SetMDP(const UInt16 &mDP)
{
	bool modified = false;
	if (m.mDP != mDP) { m.mDP = mDP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void Character_Stub::SetMaxDP(const UInt16 &maxDP)
{
	bool modified = false;
	if (m.maxDP != maxDP) { m.maxDP = maxDP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void Character_Stub::SetWC(const UInt16 &WC, const UInt16 &maxWC, const UInt16 &WcExp, const UInt32 &nextWCExp)
{
	bool modified = false;
	if (m.WC != WC) { m.WC = WC; modified = true; }
	if (m.maxWC != maxWC) { m.maxWC = maxWC; modified = true; }
	if (m.WcExp != WcExp) { m.WcExp = WcExp; modified = true; }
	if (m.nextWCExp != nextWCExp) { m.nextWCExp = nextWCExp; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void Character_Stub::SetWC(const UInt16 &WC)
{
	bool modified = false;
	if (m.WC != WC) { m.WC = WC; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void Character_Stub::SetMaxWC(const UInt16 &maxWC)
{
	bool modified = false;
	if (m.maxWC != maxWC) { m.maxWC = maxWC; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void Character_Stub::SetWcExp(const UInt16 &WcExp)
{
	bool modified = false;
	if (m.WcExp != WcExp) { m.WcExp = WcExp; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void Character_Stub::SetNextWCExp(const UInt32 &nextWCExp)
{
	bool modified = false;
	if (m.nextWCExp != nextWCExp) { m.nextWCExp = nextWCExp; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void Character_Stub::SetVP(const UInt16 &VP, const UInt16 &maxVP)
{
	bool modified = false;
	if (m.VP != VP) { m.VP = VP; modified = true; }
	if (m.maxVP != maxVP) { m.maxVP = maxVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void Character_Stub::SetVP(const UInt16 &VP)
{
	bool modified = false;
	if (m.VP != VP) { m.VP = VP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void Character_Stub::SetMaxVP(const UInt16 &maxVP)
{
	bool modified = false;
	if (m.maxVP != maxVP) { m.maxVP = maxVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void Character_Stub::SetGoodness(const Int32 &goodness)
{
	bool modified = false;
	if (m.goodness != goodness) { m.goodness = goodness; modified = true; }
	MarkUpdateAll(goodnessMask, modified);
}

void Character_Stub::SetHonor(const UInt32 &honor)
{
	bool modified = false;
	if (m.honor != honor) { m.honor = honor; modified = true; }
	MarkUpdateAll(honorMask, modified);
}

void Character_Stub::SetTitle(const String &title, const UInt16 &title_id)
{
	bool modified = false;
	if (m.title != title) { m.title = title; modified = true; }
	if (m.title_id != title_id) { m.title_id = title_id; modified = true; }
	MarkUpdateAll(titleMask, modified);
}

void Character_Stub::SetTitle(const String &title)
{
	bool modified = false;
	if (m.title != title) { m.title = title; modified = true; }
	MarkUpdateAll(titleMask, modified);
}

void Character_Stub::SetTitle_id(const UInt16 &title_id)
{
	bool modified = false;
	if (m.title_id != title_id) { m.title_id = title_id; modified = true; }
	MarkUpdateAll(titleMask, modified);
}

void Character_Stub::SetManor(const UInt32 &manor, const UInt32 &pettwo_uid, const UInt32 &pettwo_id, const String &pettwoName, const UInt16 &pettwoColorIndex)
{
	bool modified = false;
	if (m.manor != manor) { m.manor = manor; modified = true; }
	if (m.pettwo_uid != pettwo_uid) { m.pettwo_uid = pettwo_uid; modified = true; }
	if (m.pettwo_id != pettwo_id) { m.pettwo_id = pettwo_id; modified = true; }
	if (m.pettwoName != pettwoName) { m.pettwoName = pettwoName; modified = true; }
	if (m.pettwoColorIndex != pettwoColorIndex) { m.pettwoColorIndex = pettwoColorIndex; modified = true; }
	MarkUpdateAll(manorMask, modified);
}

void Character_Stub::SetManor(const UInt32 &manor)
{
	bool modified = false;
	if (m.manor != manor) { m.manor = manor; modified = true; }
	MarkUpdateAll(manorMask, modified);
}

void Character_Stub::SetPettwo_uid(const UInt32 &pettwo_uid)
{
	bool modified = false;
	if (m.pettwo_uid != pettwo_uid) { m.pettwo_uid = pettwo_uid; modified = true; }
	MarkUpdateAll(manorMask, modified);
}

void Character_Stub::SetPettwo_id(const UInt32 &pettwo_id)
{
	bool modified = false;
	if (m.pettwo_id != pettwo_id) { m.pettwo_id = pettwo_id; modified = true; }
	MarkUpdateAll(manorMask, modified);
}

void Character_Stub::SetPettwoName(const String &pettwoName)
{
	bool modified = false;
	if (m.pettwoName != pettwoName) { m.pettwoName = pettwoName; modified = true; }
	MarkUpdateAll(manorMask, modified);
}

void Character_Stub::SetPettwoColorIndex(const UInt16 &pettwoColorIndex)
{
	bool modified = false;
	if (m.pettwoColorIndex != pettwoColorIndex) { m.pettwoColorIndex = pettwoColorIndex; modified = true; }
	MarkUpdateAll(manorMask, modified);
}

void Character_Stub::SetDonate(const UInt32 &fDonate, const UInt32 &totalFDonate, const UInt32 &mDonate, const UInt32 &totalMDonate)
{
	bool modified = false;
	if (m.fDonate != fDonate) { m.fDonate = fDonate; modified = true; }
	if (m.totalFDonate != totalFDonate) { m.totalFDonate = totalFDonate; modified = true; }
	if (m.mDonate != mDonate) { m.mDonate = mDonate; modified = true; }
	if (m.totalMDonate != totalMDonate) { m.totalMDonate = totalMDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void Character_Stub::SetFDonate(const UInt32 &fDonate)
{
	bool modified = false;
	if (m.fDonate != fDonate) { m.fDonate = fDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void Character_Stub::SetTotalFDonate(const UInt32 &totalFDonate)
{
	bool modified = false;
	if (m.totalFDonate != totalFDonate) { m.totalFDonate = totalFDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void Character_Stub::SetMDonate(const UInt32 &mDonate)
{
	bool modified = false;
	if (m.mDonate != mDonate) { m.mDonate = mDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void Character_Stub::SetTotalMDonate(const UInt32 &totalMDonate)
{
	bool modified = false;
	if (m.totalMDonate != totalMDonate) { m.totalMDonate = totalMDonate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void Character_Stub::SetYuanBao(const UInt32 &yuanBao, const UInt32 &yuanBaoTicket)
{
	bool modified = false;
	if (m.yuanBao != yuanBao) { m.yuanBao = yuanBao; modified = true; }
	if (m.yuanBaoTicket != yuanBaoTicket) { m.yuanBaoTicket = yuanBaoTicket; modified = true; }
	MarkUpdateAll(yuanBaoMask, modified);
}

void Character_Stub::SetYuanBao(const UInt32 &yuanBao)
{
	bool modified = false;
	if (m.yuanBao != yuanBao) { m.yuanBao = yuanBao; modified = true; }
	MarkUpdateAll(yuanBaoMask, modified);
}

void Character_Stub::SetYuanBaoTicket(const UInt32 &yuanBaoTicket)
{
	bool modified = false;
	if (m.yuanBaoTicket != yuanBaoTicket) { m.yuanBaoTicket = yuanBaoTicket; modified = true; }
	MarkUpdateAll(yuanBaoMask, modified);
}

void Character_Stub::SetMoney(const UInt32 &money)
{
	bool modified = false;
	if (m.money != money) { m.money = money; modified = true; }
	MarkUpdateAll(moneyMask, modified);
}

void Character_Stub::SetBankMoney(const UInt32 &bankMoney)
{
	bool modified = false;
	if (m.bankMoney != bankMoney) { m.bankMoney = bankMoney; modified = true; }
	MarkUpdateAll(bankMoneyMask, modified);
}

void Character_Stub::SetSMoney(const UInt32 &SMoney)
{
	bool modified = false;
	if (m.SMoney != SMoney) { m.SMoney = SMoney; modified = true; }
	MarkUpdateAll(SMoneyMask, modified);
}

void Character_Stub::SetPartner(const UInt32 &partner_id, const UInt32 &partnerMob_id, const String &partnerName, const UInt16 &partnerColorIndex)
{
	bool modified = false;
	if (m.partner_id != partner_id) { m.partner_id = partner_id; modified = true; }
	if (m.partnerMob_id != partnerMob_id) { m.partnerMob_id = partnerMob_id; modified = true; }
	if (m.partnerName != partnerName) { m.partnerName = partnerName; modified = true; }
	if (m.partnerColorIndex != partnerColorIndex) { m.partnerColorIndex = partnerColorIndex; modified = true; }
	MarkUpdateAll(partnerMask, modified);
}

void Character_Stub::SetPartner_id(const UInt32 &partner_id)
{
	bool modified = false;
	if (m.partner_id != partner_id) { m.partner_id = partner_id; modified = true; }
	MarkUpdateAll(partnerMask, modified);
}

void Character_Stub::SetPartnerMob_id(const UInt32 &partnerMob_id)
{
	bool modified = false;
	if (m.partnerMob_id != partnerMob_id) { m.partnerMob_id = partnerMob_id; modified = true; }
	MarkUpdateAll(partnerMask, modified);
}

void Character_Stub::SetPartnerName(const String &partnerName)
{
	bool modified = false;
	if (m.partnerName != partnerName) { m.partnerName = partnerName; modified = true; }
	MarkUpdateAll(partnerMask, modified);
}

void Character_Stub::SetPartnerColorIndex(const UInt16 &partnerColorIndex)
{
	bool modified = false;
	if (m.partnerColorIndex != partnerColorIndex) { m.partnerColorIndex = partnerColorIndex; modified = true; }
	MarkUpdateAll(partnerMask, modified);
}

void Character_Stub::SetPet(const UInt32 &pet_uid, const UInt32 &pet_id, const String &petName, const UInt16 &petColorIndex)
{
	bool modified = false;
	if (m.pet_uid != pet_uid) { m.pet_uid = pet_uid; modified = true; }
	if (m.pet_id != pet_id) { m.pet_id = pet_id; modified = true; }
	if (m.petName != petName) { m.petName = petName; modified = true; }
	if (m.petColorIndex != petColorIndex) { m.petColorIndex = petColorIndex; modified = true; }
	MarkUpdateAll(petMask, modified);
}

void Character_Stub::SetPet_uid(const UInt32 &pet_uid)
{
	bool modified = false;
	if (m.pet_uid != pet_uid) { m.pet_uid = pet_uid; modified = true; }
	MarkUpdateAll(petMask, modified);
}

void Character_Stub::SetPet_id(const UInt32 &pet_id)
{
	bool modified = false;
	if (m.pet_id != pet_id) { m.pet_id = pet_id; modified = true; }
	MarkUpdateAll(petMask, modified);
}

void Character_Stub::SetPetName(const String &petName)
{
	bool modified = false;
	if (m.petName != petName) { m.petName = petName; modified = true; }
	MarkUpdateAll(petMask, modified);
}

void Character_Stub::SetPetColorIndex(const UInt16 &petColorIndex)
{
	bool modified = false;
	if (m.petColorIndex != petColorIndex) { m.petColorIndex = petColorIndex; modified = true; }
	MarkUpdateAll(petMask, modified);
}

void Character_Stub::SetMaxOther(const UInt8 &maxPartner, const UInt8 &maxPet)
{
	bool modified = false;
	if (m.maxPartner != maxPartner) { m.maxPartner = maxPartner; modified = true; }
	if (m.maxPet != maxPet) { m.maxPet = maxPet; modified = true; }
	MarkUpdateAll(maxOtherMask, modified);
}

void Character_Stub::SetMaxPartner(const UInt8 &maxPartner)
{
	bool modified = false;
	if (m.maxPartner != maxPartner) { m.maxPartner = maxPartner; modified = true; }
	MarkUpdateAll(maxOtherMask, modified);
}

void Character_Stub::SetMaxPet(const UInt8 &maxPet)
{
	bool modified = false;
	if (m.maxPet != maxPet) { m.maxPet = maxPet; modified = true; }
	MarkUpdateAll(maxOtherMask, modified);
}

void Character_Stub::SetStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor)
{
	bool modified = false;
	if (m.photo_id != photo_id) { m.photo_id = photo_id; modified = true; }
	if (m.hairStyle1 != hairStyle1) { m.hairStyle1 = hairStyle1; modified = true; }
	if (m.hairStyle2 != hairStyle2) { m.hairStyle2 = hairStyle2; modified = true; }
	if (m.hairColor != hairColor) { m.hairColor = hairColor; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void Character_Stub::SetPhoto_id(const UInt16 &photo_id)
{
	bool modified = false;
	if (m.photo_id != photo_id) { m.photo_id = photo_id; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void Character_Stub::SetHairStyle1(const UInt16 &hairStyle1)
{
	bool modified = false;
	if (m.hairStyle1 != hairStyle1) { m.hairStyle1 = hairStyle1; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void Character_Stub::SetHairStyle2(const UInt16 &hairStyle2)
{
	bool modified = false;
	if (m.hairStyle2 != hairStyle2) { m.hairStyle2 = hairStyle2; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void Character_Stub::SetHairColor(const UInt16 &hairColor)
{
	bool modified = false;
	if (m.hairColor != hairColor) { m.hairColor = hairColor; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void Character_Stub::SetEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor)
{
	bool modified = false;
	if (m.eq_weapon != eq_weapon) { m.eq_weapon = eq_weapon; modified = true; }
	if (m.eq_weaponColor != eq_weaponColor) { m.eq_weaponColor = eq_weaponColor; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void Character_Stub::SetEq_weapon(const UInt32 &eq_weapon)
{
	bool modified = false;
	if (m.eq_weapon != eq_weapon) { m.eq_weapon = eq_weapon; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void Character_Stub::SetEq_weaponColor(const UInt16 &eq_weaponColor)
{
	bool modified = false;
	if (m.eq_weaponColor != eq_weaponColor) { m.eq_weaponColor = eq_weaponColor; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void Character_Stub::SetEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor)
{
	bool modified = false;
	if (m.eq_headBand != eq_headBand) { m.eq_headBand = eq_headBand; modified = true; }
	if (m.eq_headColor != eq_headColor) { m.eq_headColor = eq_headColor; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void Character_Stub::SetEq_headBand(const UInt32 &eq_headBand)
{
	bool modified = false;
	if (m.eq_headBand != eq_headBand) { m.eq_headBand = eq_headBand; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void Character_Stub::SetEq_headColor(const UInt16 &eq_headColor)
{
	bool modified = false;
	if (m.eq_headColor != eq_headColor) { m.eq_headColor = eq_headColor; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void Character_Stub::SetEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor)
{
	bool modified = false;
	if (m.eq_cloth != eq_cloth) { m.eq_cloth = eq_cloth; modified = true; }
	if (m.eq_clothColor != eq_clothColor) { m.eq_clothColor = eq_clothColor; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void Character_Stub::SetEq_cloth(const UInt32 &eq_cloth)
{
	bool modified = false;
	if (m.eq_cloth != eq_cloth) { m.eq_cloth = eq_cloth; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void Character_Stub::SetEq_clothColor(const UInt16 &eq_clothColor)
{
	bool modified = false;
	if (m.eq_clothColor != eq_clothColor) { m.eq_clothColor = eq_clothColor; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void Character_Stub::SetState(const UInt8 &state)
{
	bool modified = false;
	if (m.state != state) { m.state = state; modified = true; }
	MarkUpdateAll(stateMask, modified);
}

void Character_Stub::SetBuff(const String &buff)
{
	bool modified = false;
	if (m.buff != buff) { m.buff = buff; modified = true; }
	MarkUpdateAll(buffMask, modified);
}

void Character_Stub::SetBuffCache(const String &buffCache)
{
	bool modified = false;
	if (m.buffCache != buffCache) { m.buffCache = buffCache; modified = true; }
	MarkUpdateAll(buffCacheMask, modified);
}

void Character_Stub::SetShortcut(const UInt16 &shortcut0, const UInt16 &shortcut1, const UInt16 &shortcut2, const UInt16 &shortcut3, const UInt16 &shortcut4, const UInt16 &shortcut5, const UInt16 &shortcut6, const UInt16 &shortcut7, const UInt16 &shortcut8, const UInt16 &shortcut9, const UInt8 &shortcuttype0, const UInt8 &shortcuttype1, const UInt8 &shortcuttype2, const UInt8 &shortcuttype3, const UInt8 &shortcuttype4, const UInt8 &shortcuttype5, const UInt8 &shortcuttype6, const UInt8 &shortcuttype7, const UInt8 &shortcuttype8, const UInt8 &shortcuttype9)
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

void Character_Stub::SetShortcut(const UInt8 i, const UInt16 &shortcut)
{
	bool modified = false;
	if (m.shortcut[i] != shortcut) { m.shortcut[i] = shortcut; modified = true; }
	MarkUpdateAll(shortcutMask, modified);
}

void Character_Stub::SetShortcuttype(const UInt8 i, const UInt8 &shortcuttype)
{
	bool modified = false;
	if (m.shortcuttype[i] != shortcuttype) { m.shortcuttype[i] = shortcuttype; modified = true; }
	MarkUpdateAll(shortcutMask, modified);
}

void Character_Stub::SetPartyInfo(const Boolean &hasParty, const Boolean &isLeader, const RPCNetID &partyNetID)
{
	bool modified = false;
	if (m.hasParty != hasParty) { m.hasParty = hasParty; modified = true; }
	if (m.isLeader != isLeader) { m.isLeader = isLeader; modified = true; }
	if (m.partyNetID != partyNetID) { m.partyNetID = partyNetID; modified = true; }
	MarkUpdateAll(partyInfoMask, modified);
}

void Character_Stub::SetHasParty(const Boolean &hasParty)
{
	bool modified = false;
	if (m.hasParty != hasParty) { m.hasParty = hasParty; modified = true; }
	MarkUpdateAll(partyInfoMask, modified);
}

void Character_Stub::SetIsLeader(const Boolean &isLeader)
{
	bool modified = false;
	if (m.isLeader != isLeader) { m.isLeader = isLeader; modified = true; }
	MarkUpdateAll(partyInfoMask, modified);
}

void Character_Stub::SetPartyNetID(const RPCNetID &partyNetID)
{
	bool modified = false;
	if (m.partyNetID != partyNetID) { m.partyNetID = partyNetID; modified = true; }
	MarkUpdateAll(partyInfoMask, modified);
}

void Character_Stub::SetPKInfo(const UInt16 &PK_Value)
{
	bool modified = false;
	if (m.PK_Value != PK_Value) { m.PK_Value = PK_Value; modified = true; }
	MarkUpdateAll(PKInfoMask, modified);
}

void Character_Stub::SetPK_Value(const UInt16 &PK_Value)
{
	bool modified = false;
	if (m.PK_Value != PK_Value) { m.PK_Value = PK_Value; modified = true; }
	MarkUpdateAll(PKInfoMask, modified);
}

void Character_Stub::SetBattleInfo(const Boolean &EnablePK)
{
	bool modified = false;
	if (m.EnablePK != EnablePK) { m.EnablePK = EnablePK; modified = true; }
	MarkUpdateAll(BattleInfoMask, modified);
}

void Character_Stub::SetEnablePK(const Boolean &EnablePK)
{
	bool modified = false;
	if (m.EnablePK != EnablePK) { m.EnablePK = EnablePK; modified = true; }
	MarkUpdateAll(BattleInfoMask, modified);
}

void Character_Stub::SetSettingInfo(const Boolean &EnableBlockGiven, const Boolean &EnableBlockParty, const Boolean &EnableBlockMsg, const Boolean &EnableBlockTrading, const Boolean &EnableBlockMakeFriend, const Boolean &EnableShowEquip)
{
	bool modified = false;
	if (m.EnableBlockGiven != EnableBlockGiven) { m.EnableBlockGiven = EnableBlockGiven; modified = true; }
	if (m.EnableBlockParty != EnableBlockParty) { m.EnableBlockParty = EnableBlockParty; modified = true; }
	if (m.EnableBlockMsg != EnableBlockMsg) { m.EnableBlockMsg = EnableBlockMsg; modified = true; }
	if (m.EnableBlockTrading != EnableBlockTrading) { m.EnableBlockTrading = EnableBlockTrading; modified = true; }
	if (m.EnableBlockMakeFriend != EnableBlockMakeFriend) { m.EnableBlockMakeFriend = EnableBlockMakeFriend; modified = true; }
	if (m.EnableShowEquip != EnableShowEquip) { m.EnableShowEquip = EnableShowEquip; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void Character_Stub::SetEnableBlockGiven(const Boolean &EnableBlockGiven)
{
	bool modified = false;
	if (m.EnableBlockGiven != EnableBlockGiven) { m.EnableBlockGiven = EnableBlockGiven; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void Character_Stub::SetEnableBlockParty(const Boolean &EnableBlockParty)
{
	bool modified = false;
	if (m.EnableBlockParty != EnableBlockParty) { m.EnableBlockParty = EnableBlockParty; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void Character_Stub::SetEnableBlockMsg(const Boolean &EnableBlockMsg)
{
	bool modified = false;
	if (m.EnableBlockMsg != EnableBlockMsg) { m.EnableBlockMsg = EnableBlockMsg; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void Character_Stub::SetEnableBlockTrading(const Boolean &EnableBlockTrading)
{
	bool modified = false;
	if (m.EnableBlockTrading != EnableBlockTrading) { m.EnableBlockTrading = EnableBlockTrading; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void Character_Stub::SetEnableBlockMakeFriend(const Boolean &EnableBlockMakeFriend)
{
	bool modified = false;
	if (m.EnableBlockMakeFriend != EnableBlockMakeFriend) { m.EnableBlockMakeFriend = EnableBlockMakeFriend; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void Character_Stub::SetEnableShowEquip(const Boolean &EnableShowEquip)
{
	bool modified = false;
	if (m.EnableShowEquip != EnableShowEquip) { m.EnableShowEquip = EnableShowEquip; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void Character_Stub::SetOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor)
{
	bool modified = false;
	if (m.org_hairStyle1 != org_hairStyle1) { m.org_hairStyle1 = org_hairStyle1; modified = true; }
	if (m.org_hairColor != org_hairColor) { m.org_hairColor = org_hairColor; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void Character_Stub::SetOrg_hairStyle1(const UInt16 &org_hairStyle1)
{
	bool modified = false;
	if (m.org_hairStyle1 != org_hairStyle1) { m.org_hairStyle1 = org_hairStyle1; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void Character_Stub::SetOrg_hairColor(const UInt16 &org_hairColor)
{
	bool modified = false;
	if (m.org_hairColor != org_hairColor) { m.org_hairColor = org_hairColor; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void Character_Stub::SetOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor)
{
	bool modified = false;
	if (m.org_eq_cloth != org_eq_cloth) { m.org_eq_cloth = org_eq_cloth; modified = true; }
	if (m.org_eq_clothColor != org_eq_clothColor) { m.org_eq_clothColor = org_eq_clothColor; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void Character_Stub::SetOrg_eq_cloth(const UInt32 &org_eq_cloth)
{
	bool modified = false;
	if (m.org_eq_cloth != org_eq_cloth) { m.org_eq_cloth = org_eq_cloth; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void Character_Stub::SetOrg_eq_clothColor(const UInt16 &org_eq_clothColor)
{
	bool modified = false;
	if (m.org_eq_clothColor != org_eq_clothColor) { m.org_eq_clothColor = org_eq_clothColor; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void Character_Stub::SetSpeakCount(const UInt16 &speakCount)
{
	bool modified = false;
	if (m.speakCount != speakCount) { m.speakCount = speakCount; modified = true; }
	MarkUpdateAll(speakCountMask, modified);
}

void Character_Stub::SetOfflineTime(const UInt32 &offlineTime)
{
	bool modified = false;
	if (m.offlineTime != offlineTime) { m.offlineTime = offlineTime; modified = true; }
	MarkUpdateAll(offlineTimeMask, modified);
}

void Character_Stub::SetHawkInfo(const String &hawkName)
{
	bool modified = false;
	if (m.hawkName != hawkName) { m.hawkName = hawkName; modified = true; }
	MarkUpdateAll(hawkInfoMask, modified);
}

void Character_Stub::SetHawkName(const String &hawkName)
{
	bool modified = false;
	if (m.hawkName != hawkName) { m.hawkName = hawkName; modified = true; }
	MarkUpdateAll(hawkInfoMask, modified);
}

void Character_Stub::SetTP(const UInt16 &TP, const UInt32 &Model_mobid)
{
	bool modified = false;
	if (m.TP != TP) { m.TP = TP; modified = true; }
	if (m.Model_mobid != Model_mobid) { m.Model_mobid = Model_mobid; modified = true; }
	MarkUpdateAll(TPMask, modified);
}

void Character_Stub::SetTP(const UInt16 &TP)
{
	bool modified = false;
	if (m.TP != TP) { m.TP = TP; modified = true; }
	MarkUpdateAll(TPMask, modified);
}

void Character_Stub::SetModel_mobid(const UInt32 &Model_mobid)
{
	bool modified = false;
	if (m.Model_mobid != Model_mobid) { m.Model_mobid = Model_mobid; modified = true; }
	MarkUpdateAll(TPMask, modified);
}

void Character_Stub::SetSpeakBlock(const UInt32 &speakBlock)
{
	bool modified = false;
	if (m.speakBlock != speakBlock) { m.speakBlock = speakBlock; modified = true; }
	MarkUpdateAll(speakBlockMask, modified);
}

void Character_Stub::SetHawkerTime(const UInt32 &hawkerTime)
{
	bool modified = false;
	if (m.hawkerTime != hawkerTime) { m.hawkerTime = hawkerTime; modified = true; }
	MarkUpdateAll(hawkerTimeMask, modified);
}

void Character_Stub::SetAvailBackPack(const UInt8 &availBackPack)
{
	bool modified = false;
	if (m.availBackPack != availBackPack) { m.availBackPack = availBackPack; modified = true; }
	MarkUpdateAll(availBackPackMask, modified);
}

void Character_Stub::SetAvailWareHouse(const UInt8 &availItemWareHouse, const UInt8 &availPartnerWareHouse, const UInt8 &availPetWareHouse)
{
	bool modified = false;
	if (m.availItemWareHouse != availItemWareHouse) { m.availItemWareHouse = availItemWareHouse; modified = true; }
	if (m.availPartnerWareHouse != availPartnerWareHouse) { m.availPartnerWareHouse = availPartnerWareHouse; modified = true; }
	if (m.availPetWareHouse != availPetWareHouse) { m.availPetWareHouse = availPetWareHouse; modified = true; }
	MarkUpdateAll(availWareHouseMask, modified);
}

void Character_Stub::SetAvailItemWareHouse(const UInt8 &availItemWareHouse)
{
	bool modified = false;
	if (m.availItemWareHouse != availItemWareHouse) { m.availItemWareHouse = availItemWareHouse; modified = true; }
	MarkUpdateAll(availWareHouseMask, modified);
}

void Character_Stub::SetAvailPartnerWareHouse(const UInt8 &availPartnerWareHouse)
{
	bool modified = false;
	if (m.availPartnerWareHouse != availPartnerWareHouse) { m.availPartnerWareHouse = availPartnerWareHouse; modified = true; }
	MarkUpdateAll(availWareHouseMask, modified);
}

void Character_Stub::SetAvailPetWareHouse(const UInt8 &availPetWareHouse)
{
	bool modified = false;
	if (m.availPetWareHouse != availPetWareHouse) { m.availPetWareHouse = availPetWareHouse; modified = true; }
	MarkUpdateAll(availWareHouseMask, modified);
}

void Character_Stub::SetFollowerList(const UInt32Vector &followerList)
{
	bool modified = false;
	if (m.followerList != followerList) { m.followerList = followerList; modified = true; }
	MarkUpdateAll(followerListMask, modified);
}

void Character_Stub::SetPvPScoreSingle(const UInt32 &PvPScoreSingle)
{
	bool modified = false;
	if (m.PvPScoreSingle != PvPScoreSingle) { m.PvPScoreSingle = PvPScoreSingle; modified = true; }
	MarkUpdateAll(PvPScoreSingleMask, modified);
}

void Character_Stub::SetUnlockTime(const UInt32 &unlock_time)
{
	bool modified = false;
	if (m.unlock_time != unlock_time) { m.unlock_time = unlock_time; modified = true; }
	MarkUpdateAll(unlockTimeMask, modified);
}

void Character_Stub::SetUnlock_time(const UInt32 &unlock_time)
{
	bool modified = false;
	if (m.unlock_time != unlock_time) { m.unlock_time = unlock_time; modified = true; }
	MarkUpdateAll(unlockTimeMask, modified);
}

void Character_Stub::SetYuanbaoLock(const Boolean &yuanbaoLock)
{
	bool modified = false;
	if (m.yuanbaoLock != yuanbaoLock) { m.yuanbaoLock = yuanbaoLock; modified = true; }
	MarkUpdateAll(yuanbaoLockMask, modified);
}

void Character_Stub::SetBankLock(const Boolean &bankLock)
{
	bool modified = false;
	if (m.bankLock != bankLock) { m.bankLock = bankLock; modified = true; }
	MarkUpdateAll(bankLockMask, modified);
}

void Character_Stub::SetSiblingId(const UInt32 &sibling_id)
{
	bool modified = false;
	if (m.sibling_id != sibling_id) { m.sibling_id = sibling_id; modified = true; }
	MarkUpdateAll(SiblingIdMask, modified);
}

void Character_Stub::SetSibling_id(const UInt32 &sibling_id)
{
	bool modified = false;
	if (m.sibling_id != sibling_id) { m.sibling_id = sibling_id; modified = true; }
	MarkUpdateAll(SiblingIdMask, modified);
}

void Character_Stub::SetMark(const UInt32 &mark)
{
	bool modified = false;
	if (m.mark != mark) { m.mark = mark; modified = true; }
	MarkUpdateAll(markMask, modified);
}

void Character_Stub::SetWantedReward(const UInt32 &wantedReward)
{
	bool modified = false;
	if (m.wantedReward != wantedReward) { m.wantedReward = wantedReward; modified = true; }
	MarkUpdateAll(wantedRewardMask, modified);
}

void Character_Stub::SetGuildUid(const UInt32 &guildUid)
{
	bool modified = false;
	if (m.guildUid != guildUid) { m.guildUid = guildUid; modified = true; }
	MarkUpdateAll(guildUidMask, modified);
}

void Character_Stub::SetGenerationAcc(const UInt32 &iHisGenAcc, const UInt32 &uiCurGenAcc)
{
	bool modified = false;
	if (m.iHisGenAcc != iHisGenAcc) { m.iHisGenAcc = iHisGenAcc; modified = true; }
	if (m.uiCurGenAcc != uiCurGenAcc) { m.uiCurGenAcc = uiCurGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}

void Character_Stub::SetIHisGenAcc(const UInt32 &iHisGenAcc)
{
	bool modified = false;
	if (m.iHisGenAcc != iHisGenAcc) { m.iHisGenAcc = iHisGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}

void Character_Stub::SetUiCurGenAcc(const UInt32 &uiCurGenAcc)
{
	bool modified = false;
	if (m.uiCurGenAcc != uiCurGenAcc) { m.uiCurGenAcc = uiCurGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}



void Character_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Character_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.acct_id);
		buf.Write<UInt32>(m.char_id);
	}
	if (mask.IsBit(levelInfoMask))
	{
		buf.Write<UInt16>(m.level);
		buf.Write<UInt32>(m.nextLvExp);
	}
	if (mask.IsBit(expMask))
	{
		buf.Write<UInt32>(m.exp);
	}
	if (mask.IsBit(attrib1Mask))
	{
		buf.Write<UInt16>(m.STA);
		buf.Write<UInt16>(m.SPR);
		buf.Write<UInt16>(m.STR);
		buf.Write<UInt16>(m.CON);
		buf.Write<UInt16>(m.AGI);
	}
	if (mask.IsBit(attrib2Mask))
	{
		buf.Write<UInt16>(m.ATK);
		buf.Write<UInt16>(m.DEF);
		buf.Write<UInt16>(m.HIT);
		buf.Write<UInt16>(m.EVA);
		buf.Write<UInt16>(m.POW);
		buf.Write<UInt16>(m.SPD);
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
		buf.Write<UInt32>(m.nextWCExp);
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
		buf.Write<UInt16>(m.title_id);
	}
	if (mask.IsBit(manorMask))
	{
		buf.Write<UInt32>(m.manor);
		buf.Write<UInt32>(m.pettwo_uid);
		buf.Write<UInt32>(m.pettwo_id);
		buf.Write<String>(m.pettwoName);
		buf.Write<UInt16>(m.pettwoColorIndex);
	}
	if (mask.IsBit(donateMask))
	{
		buf.Write<UInt32>(m.fDonate);
		buf.Write<UInt32>(m.totalFDonate);
		buf.Write<UInt32>(m.mDonate);
		buf.Write<UInt32>(m.totalMDonate);
	}
	if (mask.IsBit(yuanBaoMask))
	{
		buf.Write<UInt32>(m.yuanBao);
		buf.Write<UInt32>(m.yuanBaoTicket);
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
		buf.Write<UInt32>(m.partner_id);
		buf.Write<UInt32>(m.partnerMob_id);
		buf.Write<String>(m.partnerName);
		buf.Write<UInt16>(m.partnerColorIndex);
	}
	if (mask.IsBit(petMask))
	{
		buf.Write<UInt32>(m.pet_uid);
		buf.Write<UInt32>(m.pet_id);
		buf.Write<String>(m.petName);
		buf.Write<UInt16>(m.petColorIndex);
	}
	if (mask.IsBit(maxOtherMask))
	{
		buf.Write<UInt8>(m.maxPartner);
		buf.Write<UInt8>(m.maxPet);
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
	if (mask.IsBit(buffCacheMask))
	{
		buf.Write<String>(m.buffCache);
	}
	if (mask.IsBit(shortcutMask))
	{
		buf.WriteArray<UInt16>(m.shortcut, 10);
		buf.WriteArray<UInt8>(m.shortcuttype, 10);
	}
	if (mask.IsBit(partyInfoMask))
	{
		buf.Write<Boolean>(m.hasParty);
		buf.Write<Boolean>(m.isLeader);
		buf.Write<RPCNetID>(m.partyNetID);
	}
	if (mask.IsBit(PKInfoMask))
	{
		buf.Write<UInt16>(m.PK_Value);
	}
	if (mask.IsBit(BattleInfoMask))
	{
		buf.Write<Boolean>(m.EnablePK);
	}
	if (mask.IsBit(SettingInfoMask))
	{
		buf.Write<Boolean>(m.EnableBlockGiven);
		buf.Write<Boolean>(m.EnableBlockParty);
		buf.Write<Boolean>(m.EnableBlockMsg);
		buf.Write<Boolean>(m.EnableBlockTrading);
		buf.Write<Boolean>(m.EnableBlockMakeFriend);
		buf.Write<Boolean>(m.EnableShowEquip);
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
	if (mask.IsBit(hawkInfoMask))
	{
		buf.Write<String>(m.hawkName);
	}
	if (mask.IsBit(TPMask))
	{
		buf.Write<UInt16>(m.TP);
		buf.Write<UInt32>(m.Model_mobid);
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
	if (mask.IsBit(followerListMask))
	{
		buf.Write<UInt32Vector>(m.followerList);
	}
	if (mask.IsBit(PvPScoreSingleMask))
	{
		buf.Write<UInt32>(m.PvPScoreSingle);
	}
	if (mask.IsBit(unlockTimeMask))
	{
		buf.Write<UInt32>(m.unlock_time);
	}
	if (mask.IsBit(yuanbaoLockMask))
	{
		buf.Write<Boolean>(m.yuanbaoLock);
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
	if (mask.IsBit(guildUidMask))
	{
		buf.Write<UInt32>(m.guildUid);
	}
	if (mask.IsBit(GenerationAccMask))
	{
		buf.Write<UInt32>(m.iHisGenAcc);
		buf.Write<UInt32>(m.uiCurGenAcc);
	}
}

RPCResult Character_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Character_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Character_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_Character_MoveTo:
		{
			UInt16 posX;
			pBuf->Read<UInt16>(posX);
			UInt16 posY;
			pBuf->Read<UInt16>(posY);
			UInt16 hint1;
			pBuf->Read<UInt16>(hint1);
			UInt16 hint2;
			pBuf->Read<UInt16>(hint2);
			_result = MoveTo(pPeer, context, posX, posY, hint1, hint2);
		}
		break;
	case RPC_Character_StopMove:
		{
			_result = StopMove(pPeer, context);
		}
		break;
	case RPC_Character_RespondTest:
		{
			UInt32 minRespond;
			pBuf->Read<UInt32>(minRespond);
			UInt32 maxRespond;
			pBuf->Read<UInt32>(maxRespond);
			float avgRespond;
			pBuf->Read<float>(avgRespond);
			UInt32 respondCount;
			pBuf->Read<UInt32>(respondCount);
			_result = RespondTest(pPeer, context, minRespond, maxRespond, avgRespond, respondCount);
		}
		break;
	case RPC_Character_TestBattle:
		{
			_result = TestBattle(pPeer, context);
		}
		break;
	case RPC_Character_CancelAuto:
		{
			_result = CancelAuto(pPeer, context);
		}
		break;
	case RPC_Character_LevelUp:
		{
			_result = LevelUp(pPeer, context);
		}
		break;
	case RPC_Character_RaiseAttrib:
		{
			UInt16 sta;
			pBuf->Read<UInt16>(sta);
			UInt16 spr;
			pBuf->Read<UInt16>(spr);
			UInt16 str;
			pBuf->Read<UInt16>(str);
			UInt16 con;
			pBuf->Read<UInt16>(con);
			UInt16 agi;
			pBuf->Read<UInt16>(agi);
			_result = RaiseAttrib(pPeer, context, sta, spr, str, con, agi);
		}
		break;
	case RPC_Character_PKRequest:
		{
			UInt32 TargetID;
			pBuf->Read<UInt32>(TargetID);
			BYTE Type;
			pBuf->Read<BYTE>(Type);
			_result = PKRequest(pPeer, context, TargetID, Type);
		}
		break;
	case RPC_Character_LeaveWatching:
		{
			_result = LeaveWatching(pPeer, context);
		}
		break;
	case RPC_Character_MonitorBattle:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			_result = MonitorBattle(pPeer, context, targetId);
		}
		break;
	case RPC_Character_JoinBattle:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = JoinBattle(pPeer, context, char_id);
		}
		break;
	case RPC_Character_RequestJoinBattle:
		{
			Boolean Force;
			pBuf->Read<Boolean>(Force);
			_result = RequestJoinBattle(pPeer, context, Force);
		}
		break;
	case RPC_Character_RejectHelper:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = RejectHelper(pPeer, context, char_id);
		}
		break;
	case RPC_Character_SetFollowPartner:
		{
			UInt32 partner_id;
			pBuf->Read<UInt32>(partner_id);
			_result = SetFollowPartner(pPeer, context, partner_id);
		}
		break;
	case RPC_Character_SetFollowPet:
		{
			UInt32 pet_id;
			pBuf->Read<UInt32>(pet_id);
			_result = SetFollowPet(pPeer, context, pet_id);
		}
		break;
	case RPC_Character_ChangeMapLine:
		{
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			_result = ChangeMapLine(pPeer, context, line_id);
		}
		break;
	case RPC_Character_UpdateYuanBao:
		{
			_result = UpdateYuanBao(pPeer, context);
		}
		break;
	case RPC_Character_SearchChar:
		{
			_result = SearchChar(pPeer, context);
		}
		break;
	case RPC_Character_SetTarget:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = SetTarget(pPeer, context, char_id);
		}
		break;
	case RPC_Character_CreateParty:
		{
			_result = CreateParty(pPeer, context);
		}
		break;
	case RPC_Character_PartyJoinRequest:
		{
			RPCNetID memberId;
			pBuf->Read<RPCNetID>(memberId);
			_result = PartyJoinRequest(pPeer, context, memberId);
		}
		break;
	case RPC_Character_PartyJoinLevelControl:
		{
			RPCNetID targetId;
			pBuf->Read<RPCNetID>(targetId);
			UInt16 levelDown;
			pBuf->Read<UInt16>(levelDown);
			UInt16 levelUp;
			pBuf->Read<UInt16>(levelUp);
			_result = PartyJoinLevelControl(pPeer, context, targetId, levelDown, levelUp);
		}
		break;
	case RPC_Character_PartyJoinReply:
		{
			RPCNetID targetId;
			pBuf->Read<RPCNetID>(targetId);
			Boolean agree;
			pBuf->Read<Boolean>(agree);
			Boolean showMsg;
			pBuf->Read<Boolean>(showMsg);
			_result = PartyJoinReply(pPeer, context, targetId, agree, showMsg);
		}
		break;
	case RPC_Character_PartyInviteRequest:
		{
			RPCNetID targetId;
			pBuf->Read<RPCNetID>(targetId);
			_result = PartyInviteRequest(pPeer, context, targetId);
		}
		break;
	case RPC_Character_PartyInviteReply:
		{
			RPCNetID partyId;
			pBuf->Read<RPCNetID>(partyId);
			Boolean agree;
			pBuf->Read<Boolean>(agree);
			Boolean showMsg;
			pBuf->Read<Boolean>(showMsg);
			_result = PartyInviteReply(pPeer, context, partyId, agree, showMsg);
		}
		break;
	case RPC_Character_SearchParty:
		{
			_result = SearchParty(pPeer, context);
		}
		break;
	case RPC_Character_GetPartyInfo:
		{
			RPCNetID memberId;
			pBuf->Read<RPCNetID>(memberId);
			_result = GetPartyInfo(pPeer, context, memberId);
		}
		break;
	case RPC_Character_ShutNewGuide:
		{
			UInt8 flag;
			pBuf->Read<UInt8>(flag);
			_result = ShutNewGuide(pPeer, context, flag);
		}
		break;
	case RPC_Character_CheatCode:
		{
			UInt32 orderMakerAcct_id;
			pBuf->Read<UInt32>(orderMakerAcct_id);
			String code;
			pBuf->Read<String>(code);
			_result = CheatCode(pPeer, context, orderMakerAcct_id, code);
		}
		break;
	case RPC_Character_LearnSkill:
		{
			UInt16 skill_id;
			pBuf->Read<UInt16>(skill_id);
			_result = LearnSkill(pPeer, context, skill_id);
		}
		break;
	case RPC_Character_UseSkill:
		{
			UInt16 skill_id;
			pBuf->Read<UInt16>(skill_id);
			UInt32 TargetID;
			pBuf->Read<UInt32>(TargetID);
			BYTE TargetType;
			pBuf->Read<BYTE>(TargetType);
			_result = UseSkill(pPeer, context, skill_id, TargetID, TargetType);
		}
		break;
	case RPC_Character_UpGradeSkill:
		{
			UInt16 skill_id;
			pBuf->Read<UInt16>(skill_id);
			_result = UpGradeSkill(pPeer, context, skill_id);
		}
		break;
	case RPC_Character_RemoveSkill:
		{
			UInt16 skill_id;
			pBuf->Read<UInt16>(skill_id);
			_result = RemoveSkill(pPeer, context, skill_id);
		}
		break;
	case RPC_Character_SkillShopLearn:
		{
			UInt16 skill_id;
			pBuf->Read<UInt16>(skill_id);
			_result = SkillShopLearn(pPeer, context, skill_id);
		}
		break;
	case RPC_Character_LeaveSkillShop:
		{
			_result = LeaveSkillShop(pPeer, context);
		}
		break;
	case RPC_Character_GivenProcess:
		{
			RPCNetID targetId;
			pBuf->Read<RPCNetID>(targetId);
			UInt8Vector itemIndexs;
			pBuf->Read<UInt8Vector>(itemIndexs);
			UInt8Vector itemStacks;
			pBuf->Read<UInt8Vector>(itemStacks);
			UInt32Vector PartnerIDs;
			pBuf->Read<UInt32Vector>(PartnerIDs);
			UInt32Vector PetIDs;
			pBuf->Read<UInt32Vector>(PetIDs);
			UInt32 gmoney;
			pBuf->Read<UInt32>(gmoney);
			_result = GivenProcess(pPeer, context, targetId, itemIndexs, itemStacks, PartnerIDs, PetIDs, gmoney);
		}
		break;
	case RPC_Character_UpdateShortCutList:
		{
			UInt16Vector shortcut;
			pBuf->Read<UInt16Vector>(shortcut);
			UInt8Vector shortcuttype;
			pBuf->Read<UInt8Vector>(shortcuttype);
			_result = UpdateShortCutList(pPeer, context, shortcut, shortcuttype);
		}
		break;
	case RPC_Character_UpdateShortCut:
		{
			UInt16 shortcut;
			pBuf->Read<UInt16>(shortcut);
			UInt8 shortcuttype;
			pBuf->Read<UInt8>(shortcuttype);
			UInt8 position;
			pBuf->Read<UInt8>(position);
			_result = UpdateShortCut(pPeer, context, shortcut, shortcuttype, position);
		}
		break;
	case RPC_Character_SaveSystemSetting:
		{
			Boolean EnableBlockGiven;
			pBuf->Read<Boolean>(EnableBlockGiven);
			Boolean EnableBlockParty;
			pBuf->Read<Boolean>(EnableBlockParty);
			Boolean EnableBlockMsg;
			pBuf->Read<Boolean>(EnableBlockMsg);
			Boolean EnableBlockTrading;
			pBuf->Read<Boolean>(EnableBlockTrading);
			Boolean EnableBlockMakeFriend;
			pBuf->Read<Boolean>(EnableBlockMakeFriend);
			Boolean EnableShowEquip;
			pBuf->Read<Boolean>(EnableShowEquip);
			_result = SaveSystemSetting(pPeer, context, EnableBlockGiven, EnableBlockParty, EnableBlockMsg, EnableBlockTrading, EnableBlockMakeFriend, EnableShowEquip);
		}
		break;
	case RPC_Character_ChangeMsgIcon:
		{
			UInt16 iconID;
			pBuf->Read<UInt16>(iconID);
			_result = ChangeMsgIcon(pPeer, context, iconID);
		}
		break;
	case RPC_Character_AskForTrading:
		{
			RPCNetID targetId;
			pBuf->Read<RPCNetID>(targetId);
			_result = AskForTrading(pPeer, context, targetId);
		}
		break;
	case RPC_Character_ReplyReqTrading:
		{
			Boolean trade;
			pBuf->Read<Boolean>(trade);
			RPCNetID targetId;
			pBuf->Read<RPCNetID>(targetId);
			_result = ReplyReqTrading(pPeer, context, trade, targetId);
		}
		break;
	case RPC_Character_TalkToNPC:
		{
			UInt32 npc_id;
			pBuf->Read<UInt32>(npc_id);
			_result = TalkToNPC(pPeer, context, npc_id);
		}
		break;
	case RPC_Character_ChangeTitle:
		{
			UInt16 title_id;
			pBuf->Read<UInt16>(title_id);
			_result = ChangeTitle(pPeer, context, title_id);
		}
		break;
	case RPC_Character_AnswerAntiHack:
		{
			UInt8 answer;
			pBuf->Read<UInt8>(answer);
			Boolean is_right;
			pBuf->Read<Boolean>(is_right);
			_result = AnswerAntiHack(pPeer, context, answer, is_right);
		}
		break;
	case RPC_Character_DepositInBank:
		{
			UInt32 money;
			pBuf->Read<UInt32>(money);
			_result = DepositInBank(pPeer, context, money);
		}
		break;
	case RPC_Character_WithdrawFmBank:
		{
			UInt32 money;
			pBuf->Read<UInt32>(money);
			_result = WithdrawFmBank(pPeer, context, money);
		}
		break;
	case RPC_Character_StartHawk:
		{
			String hawkName;
			pBuf->Read<String>(hawkName);
			HawkerItemInfoVector itemList;
			pBuf->Read<HawkerItemInfoVector>(itemList);
			HawkerItemInfoVector partnerList;
			pBuf->Read<HawkerItemInfoVector>(partnerList);
			HawkerItemInfoVector petList;
			pBuf->Read<HawkerItemInfoVector>(petList);
			_result = StartHawk(pPeer, context, hawkName, itemList, partnerList, petList);
		}
		break;
	case RPC_Character_GetHawkFromTarget:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			_result = GetHawkFromTarget(pPeer, context, targetId);
		}
		break;
	case RPC_Character_ClearHawkerHistory:
		{
			_result = ClearHawkerHistory(pPeer, context);
		}
		break;
	case RPC_Character_SetListen:
		{
			BOOL IsListen;
			pBuf->Read<BOOL>(IsListen);
			_result = SetListen(pPeer, context, IsListen);
		}
		break;
	case RPC_Character_DonateMoney:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			UInt32 money;
			pBuf->Read<UInt32>(money);
			_result = DonateMoney(pPeer, context, guild_uid, money);
		}
		break;
	case RPC_Character_CreateGuild:
		{
			String name;
			pBuf->Read<String>(name);
			bool cancel;
			pBuf->Read<bool>(cancel);
			_result = CreateGuild(pPeer, context, name, cancel);
		}
		break;
	case RPC_Character_EnterQuestion:
		{
			_result = EnterQuestion(pPeer, context);
		}
		break;
	case RPC_Character_ExitQuestion:
		{
			_result = ExitQuestion(pPeer, context);
		}
		break;
	case RPC_Character_CloseWareHouse:
		{
			_result = CloseWareHouse(pPeer, context);
		}
		break;
	case RPC_Character_LockAsset:
		{
			UInt32 bagIndex;
			pBuf->Read<UInt32>(bagIndex);
			UInt8 assetType;
			pBuf->Read<UInt8>(assetType);
			_result = LockAsset(pPeer, context, bagIndex, assetType);
		}
		break;
	case RPC_Character_UnlockAsset:
		{
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			UInt32 bagIndex;
			pBuf->Read<UInt32>(bagIndex);
			UInt8 assetType;
			pBuf->Read<UInt8>(assetType);
			_result = UnlockAsset(pPeer, context, password, bagIndex, assetType);
		}
		break;
	case RPC_Character_SetAssetPassword:
		{
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = SetAssetPassword(pPeer, context, password);
		}
		break;
	case RPC_Character_ForceResetAssetPassword:
		{
			_result = ForceResetAssetPassword(pPeer, context);
		}
		break;
	case RPC_Character_ChangeAssetPassword:
		{
			ByteArray oldPassword;
			pBuf->Read<ByteArray>(oldPassword);
			ByteArray newPassword;
			pBuf->Read<ByteArray>(newPassword);
			_result = ChangeAssetPassword(pPeer, context, oldPassword, newPassword);
		}
		break;
	case RPC_Character_SetAutoWalk:
		{
			Boolean bRobot;
			pBuf->Read<Boolean>(bRobot);
			_result = SetAutoWalk(pPeer, context, bRobot);
		}
		break;
	case RPC_Character_SetAutoWalkHitBoss:
		{
			Boolean hit;
			pBuf->Read<Boolean>(hit);
			_result = SetAutoWalkHitBoss(pPeer, context, hit);
		}
		break;
	case RPC_Character_KickMember:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			String reason;
			pBuf->Read<String>(reason);
			_result = KickMember(pPeer, context, char_id, reason);
		}
		break;
	case RPC_Character_UsePartnerCollectionBuff:
		{
			bool bUse;
			pBuf->Read<bool>(bUse);
			_result = UsePartnerCollectionBuff(pPeer, context, bUse);
		}
		break;
	case RPC_Character_AddPartnerCollection:
		{
			UInt32 partner_id;
			pBuf->Read<UInt32>(partner_id);
			_result = AddPartnerCollection(pPeer, context, partner_id);
		}
		break;
	case RPC_Character_ChangeBetweenYBandYBT:
		{
			UInt8 changeType;
			pBuf->Read<UInt8>(changeType);
			UInt32 num;
			pBuf->Read<UInt32>(num);
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = ChangeBetweenYBandYBT(pPeer, context, changeType, num, password);
		}
		break;
	case RPC_Character_AddSellYuanBao:
		{
			UInt32 YBT;
			pBuf->Read<UInt32>(YBT);
			UInt32 pricePerUnit;
			pBuf->Read<UInt32>(pricePerUnit);
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = AddSellYuanBao(pPeer, context, YBT, pricePerUnit, password);
		}
		break;
	case RPC_Character_BuySellYuanBao:
		{
			UInt32 sellYuanBaoID;
			pBuf->Read<UInt32>(sellYuanBaoID);
			UInt32 YBT;
			pBuf->Read<UInt32>(YBT);
			UInt32 moneyPerUnit;
			pBuf->Read<UInt32>(moneyPerUnit);
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = BuySellYuanBao(pPeer, context, sellYuanBaoID, YBT, moneyPerUnit, password);
		}
		break;
	case RPC_Character_GetAllMoneyAndYBT:
		{
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = GetAllMoneyAndYBT(pPeer, context, password);
		}
		break;
	case RPC_Character_RequestTargetEquip:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = RequestTargetEquip(pPeer, context, char_id);
		}
		break;
	case RPC_Character_AddStartHawkItem:
		{
			HawkerItemInfo itemInfo;
			pBuf->Read<HawkerItemInfo>(itemInfo);
			StartHawkAddType addType;
			pBuf->Read<StartHawkAddType>(addType);
			_result = AddStartHawkItem(pPeer, context, itemInfo, addType);
		}
		break;
	case RPC_Character_ModifyLineup:
		{
			UInt16 uiLineupId;
			pBuf->Read<UInt16>(uiLineupId);
			_result = ModifyLineup(pPeer, context, uiLineupId);
		}
		break;
	case RPC_Character_ModifyLineupKeyPos:
		{
			int iKeyPos;
			pBuf->Read<int>(iKeyPos);
			_result = ModifyLineupKeyPos(pPeer, context, iKeyPos);
		}
		break;
	case RPC_Character_ForgiveLineup:
		{
			UInt16 uiLineupId;
			pBuf->Read<UInt16>(uiLineupId);
			_result = ForgiveLineup(pPeer, context, uiLineupId);
		}
		break;
	case RPC_Character_SetChangePos:
		{
			UInt8 ui8PreIndex;
			pBuf->Read<UInt8>(ui8PreIndex);
			UInt8 ui8DestIndex;
			pBuf->Read<UInt8>(ui8DestIndex);
			_result = SetChangePos(pPeer, context, ui8PreIndex, ui8DestIndex);
		}
		break;
	case RPC_Character_UpdateWantedReward:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			String targetName;
			pBuf->Read<String>(targetName);
			Int32 money;
			pBuf->Read<Int32>(money);
			_result = UpdateWantedReward(pPeer, context, targetId, targetName, money);
		}
		break;
	case RPC_Character_JailDelivery:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			_result = JailDelivery(pPeer, context, targetId);
		}
		break;
	case RPC_Character_WantedBail:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			_result = WantedBail(pPeer, context, targetId);
		}
		break;
	case RPC_Character_WantedTrace:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			_result = WantedTrace(pPeer, context, targetId);
		}
		break;
	case RPC_Character_GetGenerationGroup:
		{
			_result = GetGenerationGroup(pPeer, context);
		}
		break;
	case RPC_Character_RequestGeneration:
		{
			UInt32 uiTargetId;
			pBuf->Read<UInt32>(uiTargetId);
			_result = RequestGeneration(pPeer, context, uiTargetId);
		}
		break;
	case RPC_Character_RequestGenerationAns:
		{
			BOOL bConfirm;
			pBuf->Read<BOOL>(bConfirm);
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			_result = RequestGenerationAns(pPeer, context, bConfirm, uiRelatedId);
		}
		break;
	case RPC_Character_DeleteGeneration:
		{
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			_result = DeleteGeneration(pPeer, context, uiRelatedId);
		}
		break;
	case RPC_Character_GetSavedExp:
		{
			_result = GetSavedExp(pPeer, context);
		}
		break;
	case RPC_Character_BuyAccShopItem:
		{
			UInt32 iIndex;
			pBuf->Read<UInt32>(iIndex);
			_result = BuyAccShopItem(pPeer, context, iIndex);
		}
		break;
	case RPC_Character_BuyPointsShopItem:
		{
			int iItemID;
			pBuf->Read<int>(iItemID);
			UInt32 nPointsShopID;
			pBuf->Read<UInt32>(nPointsShopID);
			UInt32 nPointsShopTypeID;
			pBuf->Read<UInt32>(nPointsShopTypeID);
			_result = BuyPointsShopItem(pPeer, context, iItemID, nPointsShopID, nPointsShopTypeID);
		}
		break;
	case RPC_Character_RequestCrossMapJump:
		{
			UInt32 uSrcJmpSeq;
			pBuf->Read<UInt32>(uSrcJmpSeq);
			UInt32 uDstJmpSeq;
			pBuf->Read<UInt32>(uDstJmpSeq);
			_result = RequestCrossMapJump(pPeer, context, uSrcJmpSeq, uDstJmpSeq);
		}
		break;
	case RPC_Character_LeaveTeamTemporarily:
		{
			_result = LeaveTeamTemporarily(pPeer, context);
		}
		break;
	case RPC_Character_ReturnParty:
		{
			_result = ReturnParty(pPeer, context);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult Character_Stub::CB_RequestJoinBattle(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RequestJoinBattle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<BYTE>(returnFlag);
	return RPCSend<884230030>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_PKRequest(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag, const UInt32 &Target_ID, const UInt8 &teamCount)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_PKRequest);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<BYTE>(returnFlag);
	pBuf->Write<UInt32>(Target_ID);
	pBuf->Write<UInt8>(teamCount);
	return RPCSend<1392330330>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SetEnablePK(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SetEnablePK);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<BYTE>(returnFlag);
	return RPCSend<472343456>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_JoinBattle(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_JoinBattle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<BYTE>(returnFlag);
	return RPCSend<977666519>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_CancelAuto(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_CancelAuto);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1666675435>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_LevelUp(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_LevelUp);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<2106011260>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RaiseAttrib);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<787505021>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AddHelper(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AddHelper);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<272109665>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RejectHelper(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RejectHelper);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<825713249>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_LeaveHelpersList(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_LeaveHelpersList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<1499016738>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ClearHelpersList(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ClearHelpersList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1043758759>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_HelpRequestFail(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_HelpRequestFail);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1840727934>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_WarningMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const Int32 &value, const UInt8 &showIn)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_WarningMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(msgIndex);
	pBuf->Write<Int32>(value);
	pBuf->Write<UInt8>(showIn);
	return RPCSend<440934718>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SearchChar(LPCPEER pPeer, RPCContext &context, const CharInfoVector &CharInfoList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SearchChar);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<CharInfoVector>(CharInfoList);
	return RPCSend<708988917>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_OpenWeaponsRankPanel(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_OpenWeaponsRankPanel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1924562885>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SetTarget(LPCPEER pPeer, RPCContext &context, const Boolean &bResult, const UInt32 &char_id, const UInt16 &x, const UInt16 &y)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SetTarget);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(bResult);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt16>(x);
	pBuf->Write<UInt16>(y);
	return RPCSend<944992856>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_Error);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(errCode);
	pBuf->Write<String>(errMsg);
	return RPCSend<2004365301>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_JoinParty(LPCPEER pPeer, RPCContext &context, NetGroup* party)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_JoinParty);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(party);
	return RPCSend<2073589861>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ChangeDesc(LPCPEER pPeer, RPCContext &context, NetGroup* party)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ChangeDesc);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(party);
	return RPCSend<2016682674>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_PartyRequestOk(LPCPEER pPeer, RPCContext &context, const String &name)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_PartyRequestOk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(name);
	return RPCSend<451881969>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_PartyRequestNo(LPCPEER pPeer, RPCContext &context, const UInt16 &nState)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_PartyRequestNo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(nState);
	return RPCSend<611017579>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GetPlayerEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GetPlayerEquip);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<1586313843>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_PartyInviteOk(LPCPEER pPeer, RPCContext &context, const String &name)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_PartyInviteOk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(name);
	return RPCSend<1788162777>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_PartyJoinRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_PartyJoinRequest);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(targetId);
	return RPCSend<263587870>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_PartyInviteRequest(LPCPEER pPeer, RPCContext &context, NetGroup* party)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_PartyInviteRequest);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(party);
	return RPCSend<408096850>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_PartyClearRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const Boolean &showMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_PartyClearRequest);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(targetId);
	pBuf->Write<Boolean>(showMsg);
	return RPCSend<1680621049>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_PartyClearInvite(LPCPEER pPeer, RPCContext &context, const RPCNetID &partyId, const Boolean &showMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_PartyClearInvite);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(partyId);
	pBuf->Write<Boolean>(showMsg);
	return RPCSend<1617130850>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_PartyMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgType, const bool &inviteOrJoin, const String &charName)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_PartyMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(msgType);
	pBuf->Write<bool>(inviteOrJoin);
	pBuf->Write<String>(charName);
	return RPCSend<164526723>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SearchParty(LPCPEER pPeer, RPCContext &context, const PartyInfoVector &PartyInfoList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SearchParty);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<PartyInfoVector>(PartyInfoList);
	return RPCSend<1868450067>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GetPartyInfo(LPCPEER pPeer, RPCContext &context, const PartyInfoVector &PartyInfoList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GetPartyInfo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<PartyInfoVector>(PartyInfoList);
	return RPCSend<788955121>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_OpenSearchPartyPanel(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_OpenSearchPartyPanel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<316170255>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_HandleGuideEvent(LPCPEER pPeer, RPCContext &context, const UInt16 &ID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_HandleGuideEvent);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(ID);
	return RPCSend<1857582614>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RespondTest(LPCPEER pPeer, RPCContext &context, const UInt16 &playerCount)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RespondTest);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(playerCount);
	return RPCSend<719536770>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_LearnSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_LearnSkill);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(skill_id);
	pBuf->Write<BYTE>(returnFlag);
	return RPCSend<1640973944>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_UseSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag, const String &Msg, const UInt32 &SrcID, const UInt32 &TargetID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_UseSkill);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(skill_id);
	pBuf->Write<BYTE>(returnFlag);
	pBuf->Write<String>(Msg);
	pBuf->Write<UInt32>(SrcID);
	pBuf->Write<UInt32>(TargetID);
	return RPCSend<440018441>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_UpGradeSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_UpGradeSkill);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(skill_id);
	pBuf->Write<BYTE>(returnFlag);
	return RPCSend<101261627>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RemoveSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RemoveSkill);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(skill_id);
	pBuf->Write<BYTE>(returnFlag);
	return RPCSend<301232851>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SkillShopLearn(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SkillShopLearn);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<BYTE>(returnFlag);
	return RPCSend<19806500>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GivenProcess(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GivenProcess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(success);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend<1341011045>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GivenItem(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const UInt32Vector &itemID, const UInt8Vector &itemStacks)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GivenItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(GivenBy);
	pBuf->Write<String>(targetName);
	pBuf->Write<UInt32Vector>(itemID);
	pBuf->Write<UInt8Vector>(itemStacks);
	return RPCSend<2120963231>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GivenPartner(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const StringVector &PartnerNames)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GivenPartner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(GivenBy);
	pBuf->Write<String>(targetName);
	pBuf->Write<StringVector>(PartnerNames);
	return RPCSend<1958285418>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GivenPet(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const StringVector &PetNames)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GivenPet);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(GivenBy);
	pBuf->Write<String>(targetName);
	pBuf->Write<StringVector>(PetNames);
	return RPCSend<57557994>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GivenMoney(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const UInt32 &gmoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GivenMoney);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(GivenBy);
	pBuf->Write<String>(targetName);
	pBuf->Write<UInt32>(gmoney);
	return RPCSend<1349417469>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_UpdateShortCutSuccess(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_UpdateShortCutSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1195318687>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SaveSystemSetting(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SaveSystemSetting);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1417040156>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AskForTrading(LPCPEER pPeer, RPCContext &context, const RPCNetID &askId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AskForTrading);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(askId);
	return RPCSend<1983474478>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ReplyReqTrading(LPCPEER pPeer, RPCContext &context, const Boolean &trade)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ReplyReqTrading);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(trade);
	return RPCSend<753271876>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_TradingMessage(LPCPEER pPeer, RPCContext &context, const Boolean &trade, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_TradingMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(trade);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend<1233933636>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_TradingObject(LPCPEER pPeer, RPCContext &context, NetObject* trading)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_TradingObject);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(trading);
	return RPCSend<1591901068>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_TalkToNPC(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const bool &bResult)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_TalkToNPC);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(npc_id);
	pBuf->Write<bool>(bResult);
	return RPCSend<1600547679>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_TitleListUpdate(LPCPEER pPeer, RPCContext &context, const UInt16Vector &title_id_list, const StringVector &title_string_list, const UInt8Vector &updateState)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_TitleListUpdate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16Vector>(title_id_list);
	pBuf->Write<StringVector>(title_string_list);
	pBuf->Write<UInt8Vector>(updateState);
	return RPCSend<174658801>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AntiHack(LPCPEER pPeer, RPCContext &context, const ByteArray &image, const AntiHackData &data)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AntiHack);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<ByteArray>(image);
	pBuf->Write<AntiHackData>(data);
	return RPCSend<1582920557>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ReportAntiHack(LPCPEER pPeer, RPCContext &context, const Boolean &is_right, const UInt8 &cwrong, const UInt16 &reward)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ReportAntiHack);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(is_right);
	pBuf->Write<UInt8>(cwrong);
	pBuf->Write<UInt16>(reward);
	return RPCSend<1776368642>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_DepositInBank(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_DepositInBank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(success);
	return RPCSend<294275337>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_WithdrawFmBank(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_WithdrawFmBank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(success);
	return RPCSend<1436378679>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_StartHawk(LPCPEER pPeer, RPCContext &context, NetGroup* hawkGroup, const UInt8 &successIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_StartHawk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(hawkGroup);
	pBuf->Write<UInt8>(successIndex);
	return RPCSend<1480197420>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GetHawk(LPCPEER pPeer, RPCContext &context, const Boolean &self, NetGroup* hawkGroup, NetGroup* itemGroup, NetGroup* partnerGroup, const UInt16Vector &partnerSkillID, NetGroup* petGroup, const UInt16Vector &petSkillID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GetHawk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(self);
	pBuf->Write<NetGroup*>(hawkGroup);
	pBuf->Write<NetGroup*>(itemGroup);
	pBuf->Write<NetGroup*>(partnerGroup);
	pBuf->Write<UInt16Vector>(partnerSkillID);
	pBuf->Write<NetGroup*>(petGroup);
	pBuf->Write<UInt16Vector>(petSkillID);
	return RPCSend<1847356866>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_HawkMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_HawkMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend<664404854>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_HawkHistory(LPCPEER pPeer, RPCContext &context, const HawkerHistoryVector &historys, const StringVector &itemNames)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_HawkHistory);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<HawkerHistoryVector>(historys);
	pBuf->Write<StringVector>(itemNames);
	return RPCSend<895776164>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_UpdateHawkHistory(LPCPEER pPeer, RPCContext &context, const HawkerHistoryVector &historys, const StringVector &itemNames, const UInt32Vector &deleteHistoryID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_UpdateHawkHistory);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<HawkerHistoryVector>(historys);
	pBuf->Write<StringVector>(itemNames);
	pBuf->Write<UInt32Vector>(deleteHistoryID);
	return RPCSend<853112723>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &Result, const UInt32 &resultFlag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SendGrpMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(Result);
	pBuf->Write<UInt32>(resultFlag);
	return RPCSend<1159035620>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_OpenCreateGuild(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_OpenCreateGuild);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<850605881>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AwardMsg(LPCPEER pPeer, RPCContext &context, const String &msg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AwardMsg);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(msg);
	return RPCSend<1389179735>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_OpenGiftBox(LPCPEER pPeer, RPCContext &context, NetGroup* giftBox)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_OpenGiftBox);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(giftBox);
	return RPCSend<1802335796>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AddictionWarning(LPCPEER pPeer, RPCContext &context, const String &warnMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AddictionWarning);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(warnMsg);
	return RPCSend<161423045>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AddictCurrentStatus(LPCPEER pPeer, RPCContext &context, const String &statusMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AddictCurrentStatus);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(statusMsg);
	return RPCSend<1538414089>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AddictionShutDown(LPCPEER pPeer, RPCContext &context, const UInt32 &accOffTime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AddictionShutDown);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(accOffTime);
	return RPCSend<733353603>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_PlyInterestDwn(LPCPEER pPeer, RPCContext &context, const UInt8 &interest)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_PlyInterestDwn);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(interest);
	return RPCSend<551160062>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_UseItemCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &item_id, const UInt32 &SrcID, const UInt32 &TargetID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_UseItemCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(item_id);
	pBuf->Write<UInt32>(SrcID);
	pBuf->Write<UInt32>(TargetID);
	return RPCSend<1531910651>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AskEnterQuestion(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AskEnterQuestion);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1674016061>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_EnterQuestion(LPCPEER pPeer, RPCContext &context, NetObject* pQuestion)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_EnterQuestion);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(pQuestion);
	return RPCSend<1280472680>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ExitQuestion(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ExitQuestion);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1821640660>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AnswerQuestion(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const UInt32 &CharID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AnswerQuestion);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(index);
	pBuf->Write<UInt32>(CharID);
	return RPCSend<605690981>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_OpenWareHouse(LPCPEER pPeer, RPCContext &context, NetGroup* items, NetGroup* partners, NetGroup* pets, NetGroup* partnerSkillGroup, NetGroup* petSkillGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_OpenWareHouse);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(items);
	pBuf->Write<NetGroup*>(partners);
	pBuf->Write<NetGroup*>(pets);
	pBuf->Write<NetGroup*>(partnerSkillGroup);
	pBuf->Write<NetGroup*>(petSkillGroup);
	return RPCSend<1243417419>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AssetProtectResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &index, const UInt8 &assetType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AssetProtectResult);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(result);
	pBuf->Write<UInt32>(index);
	pBuf->Write<UInt8>(assetType);
	return RPCSend<841229587>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AssetReleaseLockResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &index, const UInt8 &assetType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AssetReleaseLockResult);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(result);
	pBuf->Write<UInt32>(index);
	pBuf->Write<UInt8>(assetType);
	return RPCSend<580604749>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SetAssetPassword);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(success);
	return RPCSend<1607548830>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ForceResetAssetPassword);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(success);
	return RPCSend<185282243>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SetRobot(LPCPEER pPeer, RPCContext &context, const Boolean &bRobot, const String &robotScript)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SetRobot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(bRobot);
	pBuf->Write<String>(robotScript);
	return RPCSend<270298762>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RobotMapAction(LPCPEER pPeer, RPCContext &context, const String &func, const RPCNetID &mapNetId, const RPCNetID &charNetId, const RPCNetID &otherNetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RobotMapAction);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(func);
	pBuf->Write<RPCNetID>(mapNetId);
	pBuf->Write<RPCNetID>(charNetId);
	pBuf->Write<RPCNetID>(otherNetId);
	return RPCSend<1022762810>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RobotBattleTurn(LPCPEER pPeer, RPCContext &context, const String &func, const RPCNetID &battleNetId, const RPCNetID &charNetId, const RPCNetID &batCharNetId, const RPCNetID &partnerNetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RobotBattleTurn);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(func);
	pBuf->Write<RPCNetID>(battleNetId);
	pBuf->Write<RPCNetID>(charNetId);
	pBuf->Write<RPCNetID>(batCharNetId);
	pBuf->Write<RPCNetID>(partnerNetId);
	return RPCSend<1967736293>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context, const Boolean &hit)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SetAutoWalkHitBoss);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(hit);
	return RPCSend<681538378>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_DonateResult(LPCPEER pPeer, RPCContext &context, const UInt32 &donate, const UInt8 &returnFlag, const UInt32 &donateMoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_DonateResult);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(donate);
	pBuf->Write<UInt8>(returnFlag);
	pBuf->Write<UInt32>(donateMoney);
	return RPCSend<997279627>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GetPartnerCollectionList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &PartnerIDs)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GetPartnerCollectionList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32Vector>(PartnerIDs);
	return RPCSend<1794267068>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_UsePartnerCollectionBuff(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_UsePartnerCollectionBuff);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<BYTE>(returnFlag);
	return RPCSend<1430018473>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &MOBid, const BYTE &returnFlag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AddPartnerCollection);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(MOBid);
	pBuf->Write<BYTE>(returnFlag);
	return RPCSend<944062561>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ChangeBetweenYBandYBT);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend<1247889129>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_AddSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend<1987536093>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_BuySellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend<1081898104>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GetAllMoneyAndYBT);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend<1673862953>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RequestTargetEquip(LPCPEER pPeer, RPCContext &context, NetGroup* items)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RequestTargetEquip);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(items);
	return RPCSend<1645497323>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RelationSystem(LPCPEER pPeer, RPCContext &context, NetObject* pRelationSystem)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RelationSystem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(pRelationSystem);
	return RPCSend<1094416333>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SetTargetNameFormHawk(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &char_name, const Boolean &isonline)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SetTargetNameFormHawk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<String>(char_name);
	pBuf->Write<Boolean>(isonline);
	return RPCSend<1254319356>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_InitTargetHawkConcernState(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_InitTargetHawkConcernState);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<2042908018>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ModifyLineup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(uiLineupId);
	return RPCSend<1567647451>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ModifyLineupKeyPos);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<int>(iKeyPos);
	return RPCSend<969433236>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ForgiveLineup(LPCPEER pPeer, RPCContext &context, const int &iResault)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ForgiveLineup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<int>(iResault);
	return RPCSend<53431052>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SetChangePos(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SetChangePos);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<int>(iRetCode);
	pBuf->Write<UInt8>(ui8PreIndex);
	pBuf->Write<UInt8>(ui8DestIndex);
	return RPCSend<765602416>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money, const Boolean &result)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_UpdateWantedReward);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(targetId);
	pBuf->Write<Int32>(money);
	pBuf->Write<Boolean>(result);
	return RPCSend<1451180764>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_JailDelivery(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_JailDelivery);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<431580735>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_WantedBail(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_WantedBail);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1406492635>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_WantedTrace(LPCPEER pPeer, RPCContext &context, const String &name, const UInt32 &lineId, const UInt32 &mapId, const UInt32 &mapX, const UInt32 &mapY)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_WantedTrace);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(name);
	pBuf->Write<UInt32>(lineId);
	pBuf->Write<UInt32>(mapId);
	pBuf->Write<UInt32>(mapX);
	pBuf->Write<UInt32>(mapY);
	return RPCSend<609051308>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RequestWantedTrace(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RequestWantedTrace);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<683047974>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GetGenerationGroup(LPCPEER pPeer, RPCContext &context, NetGroup* pGntGrp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GetGenerationGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(pGntGrp);
	return RPCSend<474378968>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RequestGenerationTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiRequstId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RequestGenerationTrans);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(uiRequstId);
	return RPCSend<266625076>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RequestGenerationResult(LPCPEER pPeer, RPCContext &context, const int &iResultCode, const UInt32 &uiRelatedId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RequestGenerationResult);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<int>(iResultCode);
	pBuf->Write<UInt32>(uiRelatedId);
	return RPCSend<1510801710>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_DeleteGeneration(LPCPEER pPeer, RPCContext &context, const int &iResultCode, const UInt32 &uiRelatedId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_DeleteGeneration);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<int>(iResultCode);
	pBuf->Write<UInt32>(uiRelatedId);
	return RPCSend<1402830072>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ChangeGenerationType(LPCPEER pPeer, RPCContext &context, const int &iType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ChangeGenerationType);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<int>(iType);
	return RPCSend<1219760122>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_UpdateGenerationAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &uiCurAcc, const UInt32 &uiHisAcc)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_UpdateGenerationAcc);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(uiCurAcc);
	pBuf->Write<UInt32>(uiHisAcc);
	return RPCSend<270404594>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_BugAccShopItem(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &iIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_BugAccShopItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<int>(iRetCode);
	pBuf->Write<UInt32>(iIndex);
	return RPCSend<829967264>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GenerationGainAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &iGainedAcc)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GenerationGainAcc);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(iGainedAcc);
	return RPCSend<1728914333>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_BuyPointsShopItem(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &uiPoints)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_BuyPointsShopItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<int>(iRetCode);
	pBuf->Write<UInt32>(uiPoints);
	return RPCSend<947990062>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_OpenBusinessLog(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_OpenBusinessLog);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1214472072>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SetMarriageNpcAction(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_uid, const UInt8 &atcion, const UInt8 &direction)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SetMarriageNpcAction);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(npc_uid);
	pBuf->Write<UInt8>(atcion);
	pBuf->Write<UInt8>(direction);
	return RPCSend<1208045910>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ShowMarriageEff(LPCPEER pPeer, RPCContext &context, const UInt8 &state, const UInt32 &husbandID, const UInt32 &wifeID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ShowMarriageEff);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(state);
	pBuf->Write<UInt32>(husbandID);
	pBuf->Write<UInt32>(wifeID);
	return RPCSend<2102293522>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ResetChangeLine(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ResetChangeLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1832715624>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_PetUseSkillMsg(LPCPEER pPeer, RPCContext &context, const String &Msg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_PetUseSkillMsg);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(Msg);
	return RPCSend<1198619385>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_LeaveTeamTemporarily(LPCPEER pPeer, RPCContext &context, const UInt32 &uRetCode, const UInt32 &uId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_LeaveTeamTemporarily);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(uRetCode);
	pBuf->Write<UInt32>(uId);
	return RPCSend<1326673354>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_GetPartyInfoWhenJumpMap(LPCPEER pPeer, RPCContext &context, NetGroup* pParty)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_GetPartyInfoWhenJumpMap);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(pParty);
	return RPCSend<2144671047>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ReturnParty(LPCPEER pPeer, RPCContext &context, const UInt32 &uRetCode, NetObject* pChar)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ReturnParty);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(uRetCode);
	pBuf->Write<NetObject*>(pChar);
	return RPCSend<993791940>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RefreshPartyCharInfo(LPCPEER pPeer, RPCContext &context, NetObject* pPartyCharInfo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RefreshPartyCharInfo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(pPartyCharInfo);
	return RPCSend<526338154>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_RestorePartyAsTemp(LPCPEER pPeer, RPCContext &context, NetGroup* pParty)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_RestorePartyAsTemp);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(pParty);
	return RPCSend<1958242924>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_SetChangeModelSkillGroup(LPCPEER pPeer, RPCContext &context, NetGroup* skillGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_SetChangeModelSkillGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(skillGroup);
	return RPCSend<1810517220>(pPeer, pBuf);
}

RPCResult Character_Stub::CB_ShowIncognitoBoxEff(LPCPEER pPeer, RPCContext &context, const UInt16 &posx, const UInt16 &posy, const UInt8 &effType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Character_CB_ShowIncognitoBoxEff);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(posx);
	pBuf->Write<UInt16>(posy);
	pBuf->Write<UInt8>(effType);
	return RPCSend<236878823>(pPeer, pBuf);
}

