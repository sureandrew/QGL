//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/ItemData_stub.h"

void ItemData_Stub::SetId(const UInt32 &item_uid, const UInt32 &item_id)
{
	bool modified = false;
	if (m.item_uid != item_uid) { m.item_uid = item_uid; modified = true; }
	if (m.item_id != item_id) { m.item_id = item_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void ItemData_Stub::SetItem_uid(const UInt32 &item_uid)
{
	bool modified = false;
	if (m.item_uid != item_uid) { m.item_uid = item_uid; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void ItemData_Stub::SetItem_id(const UInt32 &item_id)
{
	bool modified = false;
	if (m.item_id != item_id) { m.item_id = item_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void ItemData_Stub::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(owner_idMask, modified);
}

void ItemData_Stub::SetBagIndex(const UInt8 &bagIndex)
{
	bool modified = false;
	if (m.bagIndex != bagIndex) { m.bagIndex = bagIndex; modified = true; }
	MarkUpdateAll(bagIndexMask, modified);
}

void ItemData_Stub::SetMaker(const String &maker)
{
	bool modified = false;
	if (m.maker != maker) { m.maker = maker; modified = true; }
	MarkUpdateAll(makerMask, modified);
}

void ItemData_Stub::SetEq_element(const UInt8 &eq_element)
{
	bool modified = false;
	if (m.eq_element != eq_element) { m.eq_element = eq_element; modified = true; }
	MarkUpdateAll(eq_elementMask, modified);
}

void ItemData_Stub::SetEndure(const UInt16 &endure)
{
	bool modified = false;
	if (m.endure != endure) { m.endure = endure; modified = true; }
	MarkUpdateAll(endureMask, modified);
}

void ItemData_Stub::SetMEndure(const UInt16 &mEndure)
{
	bool modified = false;
	if (m.mEndure != mEndure) { m.mEndure = mEndure; modified = true; }
	MarkUpdateAll(mEndureMask, modified);
}

void ItemData_Stub::SetGrade(const UInt16 &grade)
{
	bool modified = false;
	if (m.grade != grade) { m.grade = grade; modified = true; }
	MarkUpdateAll(gradeMask, modified);
}

void ItemData_Stub::SetUpgExp(const UInt16 &upg_exp)
{
	bool modified = false;
	if (m.upg_exp != upg_exp) { m.upg_exp = upg_exp; modified = true; }
	MarkUpdateAll(upgExpMask, modified);
}

void ItemData_Stub::SetUpg_exp(const UInt16 &upg_exp)
{
	bool modified = false;
	if (m.upg_exp != upg_exp) { m.upg_exp = upg_exp; modified = true; }
	MarkUpdateAll(upgExpMask, modified);
}

void ItemData_Stub::SetRemain(const UInt8 &remain)
{
	bool modified = false;
	if (m.remain != remain) { m.remain = remain; modified = true; }
	MarkUpdateAll(remainMask, modified);
}

void ItemData_Stub::SetStack(const UInt8 &stack)
{
	bool modified = false;
	if (m.stack != stack) { m.stack = stack; modified = true; }
	MarkUpdateAll(stackMask, modified);
}

void ItemData_Stub::SetBaseAttr(const Int16 &base_STA, const Int16 &base_SPR, const Int16 &base_STR, const Int16 &base_CON, const Int16 &base_AGI, const Int16 &base_ATK, const Int16 &base_DEF, const Int16 &base_HIT, const Int16 &base_EVA, const Int16 &base_POW, const Int16 &base_SPD, const Int16 &base_maxHP, const Int16 &base_maxSP, const Int16 &base_maxVP)
{
	bool modified = false;
	if (m.base_STA != base_STA) { m.base_STA = base_STA; modified = true; }
	if (m.base_SPR != base_SPR) { m.base_SPR = base_SPR; modified = true; }
	if (m.base_STR != base_STR) { m.base_STR = base_STR; modified = true; }
	if (m.base_CON != base_CON) { m.base_CON = base_CON; modified = true; }
	if (m.base_AGI != base_AGI) { m.base_AGI = base_AGI; modified = true; }
	if (m.base_ATK != base_ATK) { m.base_ATK = base_ATK; modified = true; }
	if (m.base_DEF != base_DEF) { m.base_DEF = base_DEF; modified = true; }
	if (m.base_HIT != base_HIT) { m.base_HIT = base_HIT; modified = true; }
	if (m.base_EVA != base_EVA) { m.base_EVA = base_EVA; modified = true; }
	if (m.base_POW != base_POW) { m.base_POW = base_POW; modified = true; }
	if (m.base_SPD != base_SPD) { m.base_SPD = base_SPD; modified = true; }
	if (m.base_maxHP != base_maxHP) { m.base_maxHP = base_maxHP; modified = true; }
	if (m.base_maxSP != base_maxSP) { m.base_maxSP = base_maxSP; modified = true; }
	if (m.base_maxVP != base_maxVP) { m.base_maxVP = base_maxVP; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_STA(const Int16 &base_STA)
{
	bool modified = false;
	if (m.base_STA != base_STA) { m.base_STA = base_STA; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_SPR(const Int16 &base_SPR)
{
	bool modified = false;
	if (m.base_SPR != base_SPR) { m.base_SPR = base_SPR; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_STR(const Int16 &base_STR)
{
	bool modified = false;
	if (m.base_STR != base_STR) { m.base_STR = base_STR; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_CON(const Int16 &base_CON)
{
	bool modified = false;
	if (m.base_CON != base_CON) { m.base_CON = base_CON; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_AGI(const Int16 &base_AGI)
{
	bool modified = false;
	if (m.base_AGI != base_AGI) { m.base_AGI = base_AGI; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_ATK(const Int16 &base_ATK)
{
	bool modified = false;
	if (m.base_ATK != base_ATK) { m.base_ATK = base_ATK; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_DEF(const Int16 &base_DEF)
{
	bool modified = false;
	if (m.base_DEF != base_DEF) { m.base_DEF = base_DEF; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_HIT(const Int16 &base_HIT)
{
	bool modified = false;
	if (m.base_HIT != base_HIT) { m.base_HIT = base_HIT; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_EVA(const Int16 &base_EVA)
{
	bool modified = false;
	if (m.base_EVA != base_EVA) { m.base_EVA = base_EVA; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_POW(const Int16 &base_POW)
{
	bool modified = false;
	if (m.base_POW != base_POW) { m.base_POW = base_POW; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_SPD(const Int16 &base_SPD)
{
	bool modified = false;
	if (m.base_SPD != base_SPD) { m.base_SPD = base_SPD; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_maxHP(const Int16 &base_maxHP)
{
	bool modified = false;
	if (m.base_maxHP != base_maxHP) { m.base_maxHP = base_maxHP; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_maxSP(const Int16 &base_maxSP)
{
	bool modified = false;
	if (m.base_maxSP != base_maxSP) { m.base_maxSP = base_maxSP; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetBase_maxVP(const Int16 &base_maxVP)
{
	bool modified = false;
	if (m.base_maxVP != base_maxVP) { m.base_maxVP = base_maxVP; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Stub::SetUpgAttr(const Int16 &upg_STA, const Int16 &upg_SPR, const Int16 &upg_STR, const Int16 &upg_CON, const Int16 &upg_AGI, const Int16 &upg_ATK, const Int16 &upg_DEF, const Int16 &upg_HIT, const Int16 &upg_EVA, const Int16 &upg_POW, const Int16 &upg_SPD, const Int16 &upg_maxHP, const Int16 &upg_maxSP, const Int16 &upg_maxVP)
{
	bool modified = false;
	if (m.upg_STA != upg_STA) { m.upg_STA = upg_STA; modified = true; }
	if (m.upg_SPR != upg_SPR) { m.upg_SPR = upg_SPR; modified = true; }
	if (m.upg_STR != upg_STR) { m.upg_STR = upg_STR; modified = true; }
	if (m.upg_CON != upg_CON) { m.upg_CON = upg_CON; modified = true; }
	if (m.upg_AGI != upg_AGI) { m.upg_AGI = upg_AGI; modified = true; }
	if (m.upg_ATK != upg_ATK) { m.upg_ATK = upg_ATK; modified = true; }
	if (m.upg_DEF != upg_DEF) { m.upg_DEF = upg_DEF; modified = true; }
	if (m.upg_HIT != upg_HIT) { m.upg_HIT = upg_HIT; modified = true; }
	if (m.upg_EVA != upg_EVA) { m.upg_EVA = upg_EVA; modified = true; }
	if (m.upg_POW != upg_POW) { m.upg_POW = upg_POW; modified = true; }
	if (m.upg_SPD != upg_SPD) { m.upg_SPD = upg_SPD; modified = true; }
	if (m.upg_maxHP != upg_maxHP) { m.upg_maxHP = upg_maxHP; modified = true; }
	if (m.upg_maxSP != upg_maxSP) { m.upg_maxSP = upg_maxSP; modified = true; }
	if (m.upg_maxVP != upg_maxVP) { m.upg_maxVP = upg_maxVP; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_STA(const Int16 &upg_STA)
{
	bool modified = false;
	if (m.upg_STA != upg_STA) { m.upg_STA = upg_STA; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_SPR(const Int16 &upg_SPR)
{
	bool modified = false;
	if (m.upg_SPR != upg_SPR) { m.upg_SPR = upg_SPR; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_STR(const Int16 &upg_STR)
{
	bool modified = false;
	if (m.upg_STR != upg_STR) { m.upg_STR = upg_STR; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_CON(const Int16 &upg_CON)
{
	bool modified = false;
	if (m.upg_CON != upg_CON) { m.upg_CON = upg_CON; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_AGI(const Int16 &upg_AGI)
{
	bool modified = false;
	if (m.upg_AGI != upg_AGI) { m.upg_AGI = upg_AGI; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_ATK(const Int16 &upg_ATK)
{
	bool modified = false;
	if (m.upg_ATK != upg_ATK) { m.upg_ATK = upg_ATK; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_DEF(const Int16 &upg_DEF)
{
	bool modified = false;
	if (m.upg_DEF != upg_DEF) { m.upg_DEF = upg_DEF; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_HIT(const Int16 &upg_HIT)
{
	bool modified = false;
	if (m.upg_HIT != upg_HIT) { m.upg_HIT = upg_HIT; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_EVA(const Int16 &upg_EVA)
{
	bool modified = false;
	if (m.upg_EVA != upg_EVA) { m.upg_EVA = upg_EVA; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_POW(const Int16 &upg_POW)
{
	bool modified = false;
	if (m.upg_POW != upg_POW) { m.upg_POW = upg_POW; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_SPD(const Int16 &upg_SPD)
{
	bool modified = false;
	if (m.upg_SPD != upg_SPD) { m.upg_SPD = upg_SPD; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_maxHP(const Int16 &upg_maxHP)
{
	bool modified = false;
	if (m.upg_maxHP != upg_maxHP) { m.upg_maxHP = upg_maxHP; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_maxSP(const Int16 &upg_maxSP)
{
	bool modified = false;
	if (m.upg_maxSP != upg_maxSP) { m.upg_maxSP = upg_maxSP; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetUpg_maxVP(const Int16 &upg_maxVP)
{
	bool modified = false;
	if (m.upg_maxVP != upg_maxVP) { m.upg_maxVP = upg_maxVP; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Stub::SetInlayGem(const UInt8 &eq_gem, const UInt16 &inlay_gem0, const UInt16 &inlay_gem1, const UInt16 &inlay_gem2, const UInt16 &inlay_gem3, const UInt16 &inlay_gem4, const UInt16 &inlay_gem5, const UInt16 &inlay_gem6, const UInt16 &inlay_gem7, const UInt16 &inlay_gem8, const UInt16 &inlay_gem9)
{
	bool modified = false;
	if (m.eq_gem != eq_gem) { m.eq_gem = eq_gem; modified = true; }
	if (m.inlay_gem[0] != inlay_gem0) { m.inlay_gem[0] = inlay_gem0; modified = true; }
	if (m.inlay_gem[1] != inlay_gem1) { m.inlay_gem[1] = inlay_gem1; modified = true; }
	if (m.inlay_gem[2] != inlay_gem2) { m.inlay_gem[2] = inlay_gem2; modified = true; }
	if (m.inlay_gem[3] != inlay_gem3) { m.inlay_gem[3] = inlay_gem3; modified = true; }
	if (m.inlay_gem[4] != inlay_gem4) { m.inlay_gem[4] = inlay_gem4; modified = true; }
	if (m.inlay_gem[5] != inlay_gem5) { m.inlay_gem[5] = inlay_gem5; modified = true; }
	if (m.inlay_gem[6] != inlay_gem6) { m.inlay_gem[6] = inlay_gem6; modified = true; }
	if (m.inlay_gem[7] != inlay_gem7) { m.inlay_gem[7] = inlay_gem7; modified = true; }
	if (m.inlay_gem[8] != inlay_gem8) { m.inlay_gem[8] = inlay_gem8; modified = true; }
	if (m.inlay_gem[9] != inlay_gem9) { m.inlay_gem[9] = inlay_gem9; modified = true; }
	MarkUpdateAll(inlayGemMask, modified);
}

void ItemData_Stub::SetEq_gem(const UInt8 &eq_gem)
{
	bool modified = false;
	if (m.eq_gem != eq_gem) { m.eq_gem = eq_gem; modified = true; }
	MarkUpdateAll(inlayGemMask, modified);
}

void ItemData_Stub::SetInlay_gem(const UInt8 i, const UInt16 &inlay_gem)
{
	bool modified = false;
	if (m.inlay_gem[i] != inlay_gem) { m.inlay_gem[i] = inlay_gem; modified = true; }
	MarkUpdateAll(inlayGemMask, modified);
}

void ItemData_Stub::SetInlayAttr(const Int16 &inlay_STA, const Int16 &inlay_SPR, const Int16 &inlay_STR, const Int16 &inlay_CON, const Int16 &inlay_AGI, const Int16 &inlay_ATK, const Int16 &inlay_DEF, const Int16 &inlay_HIT, const Int16 &inlay_EVA, const Int16 &inlay_POW, const Int16 &inlay_SPD, const Int16 &inlay_maxHP, const Int16 &inlay_maxSP, const Int16 &inlay_maxVP)
{
	bool modified = false;
	if (m.inlay_STA != inlay_STA) { m.inlay_STA = inlay_STA; modified = true; }
	if (m.inlay_SPR != inlay_SPR) { m.inlay_SPR = inlay_SPR; modified = true; }
	if (m.inlay_STR != inlay_STR) { m.inlay_STR = inlay_STR; modified = true; }
	if (m.inlay_CON != inlay_CON) { m.inlay_CON = inlay_CON; modified = true; }
	if (m.inlay_AGI != inlay_AGI) { m.inlay_AGI = inlay_AGI; modified = true; }
	if (m.inlay_ATK != inlay_ATK) { m.inlay_ATK = inlay_ATK; modified = true; }
	if (m.inlay_DEF != inlay_DEF) { m.inlay_DEF = inlay_DEF; modified = true; }
	if (m.inlay_HIT != inlay_HIT) { m.inlay_HIT = inlay_HIT; modified = true; }
	if (m.inlay_EVA != inlay_EVA) { m.inlay_EVA = inlay_EVA; modified = true; }
	if (m.inlay_POW != inlay_POW) { m.inlay_POW = inlay_POW; modified = true; }
	if (m.inlay_SPD != inlay_SPD) { m.inlay_SPD = inlay_SPD; modified = true; }
	if (m.inlay_maxHP != inlay_maxHP) { m.inlay_maxHP = inlay_maxHP; modified = true; }
	if (m.inlay_maxSP != inlay_maxSP) { m.inlay_maxSP = inlay_maxSP; modified = true; }
	if (m.inlay_maxVP != inlay_maxVP) { m.inlay_maxVP = inlay_maxVP; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_STA(const Int16 &inlay_STA)
{
	bool modified = false;
	if (m.inlay_STA != inlay_STA) { m.inlay_STA = inlay_STA; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_SPR(const Int16 &inlay_SPR)
{
	bool modified = false;
	if (m.inlay_SPR != inlay_SPR) { m.inlay_SPR = inlay_SPR; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_STR(const Int16 &inlay_STR)
{
	bool modified = false;
	if (m.inlay_STR != inlay_STR) { m.inlay_STR = inlay_STR; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_CON(const Int16 &inlay_CON)
{
	bool modified = false;
	if (m.inlay_CON != inlay_CON) { m.inlay_CON = inlay_CON; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_AGI(const Int16 &inlay_AGI)
{
	bool modified = false;
	if (m.inlay_AGI != inlay_AGI) { m.inlay_AGI = inlay_AGI; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_ATK(const Int16 &inlay_ATK)
{
	bool modified = false;
	if (m.inlay_ATK != inlay_ATK) { m.inlay_ATK = inlay_ATK; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_DEF(const Int16 &inlay_DEF)
{
	bool modified = false;
	if (m.inlay_DEF != inlay_DEF) { m.inlay_DEF = inlay_DEF; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_HIT(const Int16 &inlay_HIT)
{
	bool modified = false;
	if (m.inlay_HIT != inlay_HIT) { m.inlay_HIT = inlay_HIT; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_EVA(const Int16 &inlay_EVA)
{
	bool modified = false;
	if (m.inlay_EVA != inlay_EVA) { m.inlay_EVA = inlay_EVA; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_POW(const Int16 &inlay_POW)
{
	bool modified = false;
	if (m.inlay_POW != inlay_POW) { m.inlay_POW = inlay_POW; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_SPD(const Int16 &inlay_SPD)
{
	bool modified = false;
	if (m.inlay_SPD != inlay_SPD) { m.inlay_SPD = inlay_SPD; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_maxHP(const Int16 &inlay_maxHP)
{
	bool modified = false;
	if (m.inlay_maxHP != inlay_maxHP) { m.inlay_maxHP = inlay_maxHP; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_maxSP(const Int16 &inlay_maxSP)
{
	bool modified = false;
	if (m.inlay_maxSP != inlay_maxSP) { m.inlay_maxSP = inlay_maxSP; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetInlay_maxVP(const Int16 &inlay_maxVP)
{
	bool modified = false;
	if (m.inlay_maxVP != inlay_maxVP) { m.inlay_maxVP = inlay_maxVP; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Stub::SetAddon(const UInt16 &addon)
{
	bool modified = false;
	if (m.addon != addon) { m.addon = addon; modified = true; }
	MarkUpdateAll(addonMask, modified);
}

void ItemData_Stub::SetEqColor(const UInt16 &eqColor)
{
	bool modified = false;
	if (m.eqColor != eqColor) { m.eqColor = eqColor; modified = true; }
	MarkUpdateAll(eqColorMask, modified);
}

void ItemData_Stub::SetQuality(const UInt16 &item_quality)
{
	bool modified = false;
	if (m.item_quality != item_quality) { m.item_quality = item_quality; modified = true; }
	MarkUpdateAll(qualityMask, modified);
}

void ItemData_Stub::SetItem_quality(const UInt16 &item_quality)
{
	bool modified = false;
	if (m.item_quality != item_quality) { m.item_quality = item_quality; modified = true; }
	MarkUpdateAll(qualityMask, modified);
}

void ItemData_Stub::SetSoulboundState(const UInt8 &item_soulboundState)
{
	bool modified = false;
	if (m.item_soulboundState != item_soulboundState) { m.item_soulboundState = item_soulboundState; modified = true; }
	MarkUpdateAll(soulboundStateMask, modified);
}

void ItemData_Stub::SetItem_soulboundState(const UInt8 &item_soulboundState)
{
	bool modified = false;
	if (m.item_soulboundState != item_soulboundState) { m.item_soulboundState = item_soulboundState; modified = true; }
	MarkUpdateAll(soulboundStateMask, modified);
}

void ItemData_Stub::SetIdentifyState(const UInt8 &item_IdentifyState)
{
	bool modified = false;
	if (m.item_IdentifyState != item_IdentifyState) { m.item_IdentifyState = item_IdentifyState; modified = true; }
	MarkUpdateAll(IdentifyStateMask, modified);
}

void ItemData_Stub::SetItem_IdentifyState(const UInt8 &item_IdentifyState)
{
	bool modified = false;
	if (m.item_IdentifyState != item_IdentifyState) { m.item_IdentifyState = item_IdentifyState; modified = true; }
	MarkUpdateAll(IdentifyStateMask, modified);
}

void ItemData_Stub::SetExpireDate(const UInt32 &item_expireDate)
{
	bool modified = false;
	if (m.item_expireDate != item_expireDate) { m.item_expireDate = item_expireDate; modified = true; }
	MarkUpdateAll(ExpireDateMask, modified);
}

void ItemData_Stub::SetItem_expireDate(const UInt32 &item_expireDate)
{
	bool modified = false;
	if (m.item_expireDate != item_expireDate) { m.item_expireDate = item_expireDate; modified = true; }
	MarkUpdateAll(ExpireDateMask, modified);
}

void ItemData_Stub::SetValue(const UInt16 &itemValue1, const UInt16 &itemValue2, const UInt16 &itemValue3)
{
	bool modified = false;
	if (m.itemValue1 != itemValue1) { m.itemValue1 = itemValue1; modified = true; }
	if (m.itemValue2 != itemValue2) { m.itemValue2 = itemValue2; modified = true; }
	if (m.itemValue3 != itemValue3) { m.itemValue3 = itemValue3; modified = true; }
	MarkUpdateAll(ValueMask, modified);
}

void ItemData_Stub::SetItemValue1(const UInt16 &itemValue1)
{
	bool modified = false;
	if (m.itemValue1 != itemValue1) { m.itemValue1 = itemValue1; modified = true; }
	MarkUpdateAll(ValueMask, modified);
}

void ItemData_Stub::SetItemValue2(const UInt16 &itemValue2)
{
	bool modified = false;
	if (m.itemValue2 != itemValue2) { m.itemValue2 = itemValue2; modified = true; }
	MarkUpdateAll(ValueMask, modified);
}

void ItemData_Stub::SetItemValue3(const UInt16 &itemValue3)
{
	bool modified = false;
	if (m.itemValue3 != itemValue3) { m.itemValue3 = itemValue3; modified = true; }
	MarkUpdateAll(ValueMask, modified);
}

void ItemData_Stub::SetBuffID(const UInt16 &buffID)
{
	bool modified = false;
	if (m.buffID != buffID) { m.buffID = buffID; modified = true; }
	MarkUpdateAll(buffIDMask, modified);
}

void ItemData_Stub::SetBuffValue1(const UInt32 &buffValue1)
{
	bool modified = false;
	if (m.buffValue1 != buffValue1) { m.buffValue1 = buffValue1; modified = true; }
	MarkUpdateAll(buffValue1Mask, modified);
}

void ItemData_Stub::SetBuffValue2(const UInt32 &buffValue2)
{
	bool modified = false;
	if (m.buffValue2 != buffValue2) { m.buffValue2 = buffValue2; modified = true; }
	MarkUpdateAll(buffValue2Mask, modified);
}

void ItemData_Stub::SetBuffValue3(const UInt32 &buffValue3)
{
	bool modified = false;
	if (m.buffValue3 != buffValue3) { m.buffValue3 = buffValue3; modified = true; }
	MarkUpdateAll(buffValue3Mask, modified);
}

void ItemData_Stub::SetBuffCoolDown(const UInt32 &buffCoolDown)
{
	bool modified = false;
	if (m.buffCoolDown != buffCoolDown) { m.buffCoolDown = buffCoolDown; modified = true; }
	MarkUpdateAll(buffCoolDownMask, modified);
}

void ItemData_Stub::SetBuffPro(const UInt8 &buffPro)
{
	bool modified = false;
	if (m.buffPro != buffPro) { m.buffPro = buffPro; modified = true; }
	MarkUpdateAll(buffProMask, modified);
}

void ItemData_Stub::SetInWareHouse(const UInt8 &inWareHouse)
{
	bool modified = false;
	if (m.inWareHouse != inWareHouse) { m.inWareHouse = inWareHouse; modified = true; }
	MarkUpdateAll(inWareHouseMask, modified);
}

void ItemData_Stub::SetAssetLock(const Boolean &lock)
{
	bool modified = false;
	if (m.lock != lock) { m.lock = lock; modified = true; }
	MarkUpdateAll(assetLockMask, modified);
}

void ItemData_Stub::SetLock(const Boolean &lock)
{
	bool modified = false;
	if (m.lock != lock) { m.lock = lock; modified = true; }
	MarkUpdateAll(assetLockMask, modified);
}

void ItemData_Stub::SetSkillID(const UInt16 &skillID)
{
	bool modified = false;
	if (m.skillID != skillID) { m.skillID = skillID; modified = true; }
	MarkUpdateAll(skillIDMask, modified);
}


void ItemData_Stub::Clone(ItemData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(idMask) && !(obj->m.item_uid == m.item_uid)) {
		m.item_uid = obj->m.item_uid;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(idMask) && !(obj->m.item_id == m.item_id)) {
		m.item_id = obj->m.item_id;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(owner_idMask) && !(obj->m.owner_id == m.owner_id)) {
		m.owner_id = obj->m.owner_id;
		MarkUpdateAll(owner_idMask, true);
	}
	if (mask.IsBit(bagIndexMask) && !(obj->m.bagIndex == m.bagIndex)) {
		m.bagIndex = obj->m.bagIndex;
		MarkUpdateAll(bagIndexMask, true);
	}
	if (mask.IsBit(makerMask) && !(obj->m.maker == m.maker)) {
		m.maker = obj->m.maker;
		MarkUpdateAll(makerMask, true);
	}
	if (mask.IsBit(eq_elementMask) && !(obj->m.eq_element == m.eq_element)) {
		m.eq_element = obj->m.eq_element;
		MarkUpdateAll(eq_elementMask, true);
	}
	if (mask.IsBit(endureMask) && !(obj->m.endure == m.endure)) {
		m.endure = obj->m.endure;
		MarkUpdateAll(endureMask, true);
	}
	if (mask.IsBit(mEndureMask) && !(obj->m.mEndure == m.mEndure)) {
		m.mEndure = obj->m.mEndure;
		MarkUpdateAll(mEndureMask, true);
	}
	if (mask.IsBit(gradeMask) && !(obj->m.grade == m.grade)) {
		m.grade = obj->m.grade;
		MarkUpdateAll(gradeMask, true);
	}
	if (mask.IsBit(upgExpMask) && !(obj->m.upg_exp == m.upg_exp)) {
		m.upg_exp = obj->m.upg_exp;
		MarkUpdateAll(upgExpMask, true);
	}
	if (mask.IsBit(remainMask) && !(obj->m.remain == m.remain)) {
		m.remain = obj->m.remain;
		MarkUpdateAll(remainMask, true);
	}
	if (mask.IsBit(stackMask) && !(obj->m.stack == m.stack)) {
		m.stack = obj->m.stack;
		MarkUpdateAll(stackMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_STA == m.base_STA)) {
		m.base_STA = obj->m.base_STA;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_SPR == m.base_SPR)) {
		m.base_SPR = obj->m.base_SPR;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_STR == m.base_STR)) {
		m.base_STR = obj->m.base_STR;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_CON == m.base_CON)) {
		m.base_CON = obj->m.base_CON;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_AGI == m.base_AGI)) {
		m.base_AGI = obj->m.base_AGI;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_ATK == m.base_ATK)) {
		m.base_ATK = obj->m.base_ATK;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_DEF == m.base_DEF)) {
		m.base_DEF = obj->m.base_DEF;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_HIT == m.base_HIT)) {
		m.base_HIT = obj->m.base_HIT;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_EVA == m.base_EVA)) {
		m.base_EVA = obj->m.base_EVA;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_POW == m.base_POW)) {
		m.base_POW = obj->m.base_POW;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_SPD == m.base_SPD)) {
		m.base_SPD = obj->m.base_SPD;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_maxHP == m.base_maxHP)) {
		m.base_maxHP = obj->m.base_maxHP;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_maxSP == m.base_maxSP)) {
		m.base_maxSP = obj->m.base_maxSP;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(baseAttrMask) && !(obj->m.base_maxVP == m.base_maxVP)) {
		m.base_maxVP = obj->m.base_maxVP;
		MarkUpdateAll(baseAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_STA == m.upg_STA)) {
		m.upg_STA = obj->m.upg_STA;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_SPR == m.upg_SPR)) {
		m.upg_SPR = obj->m.upg_SPR;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_STR == m.upg_STR)) {
		m.upg_STR = obj->m.upg_STR;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_CON == m.upg_CON)) {
		m.upg_CON = obj->m.upg_CON;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_AGI == m.upg_AGI)) {
		m.upg_AGI = obj->m.upg_AGI;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_ATK == m.upg_ATK)) {
		m.upg_ATK = obj->m.upg_ATK;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_DEF == m.upg_DEF)) {
		m.upg_DEF = obj->m.upg_DEF;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_HIT == m.upg_HIT)) {
		m.upg_HIT = obj->m.upg_HIT;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_EVA == m.upg_EVA)) {
		m.upg_EVA = obj->m.upg_EVA;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_POW == m.upg_POW)) {
		m.upg_POW = obj->m.upg_POW;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_SPD == m.upg_SPD)) {
		m.upg_SPD = obj->m.upg_SPD;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_maxHP == m.upg_maxHP)) {
		m.upg_maxHP = obj->m.upg_maxHP;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_maxSP == m.upg_maxSP)) {
		m.upg_maxSP = obj->m.upg_maxSP;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(upgAttrMask) && !(obj->m.upg_maxVP == m.upg_maxVP)) {
		m.upg_maxVP = obj->m.upg_maxVP;
		MarkUpdateAll(upgAttrMask, true);
	}
	if (mask.IsBit(inlayGemMask) && !(obj->m.eq_gem == m.eq_gem)) {
		m.eq_gem = obj->m.eq_gem;
		MarkUpdateAll(inlayGemMask, true);
	}
	if (mask.IsBit(inlayGemMask)) {
		bool updated = false;
		if (!(obj->m.inlay_gem[0] == m.inlay_gem[0])) {
			m.inlay_gem[0] = obj->m.inlay_gem[0];
			updated = true;
		}
		if (!(obj->m.inlay_gem[1] == m.inlay_gem[1])) {
			m.inlay_gem[1] = obj->m.inlay_gem[1];
			updated = true;
		}
		if (!(obj->m.inlay_gem[2] == m.inlay_gem[2])) {
			m.inlay_gem[2] = obj->m.inlay_gem[2];
			updated = true;
		}
		if (!(obj->m.inlay_gem[3] == m.inlay_gem[3])) {
			m.inlay_gem[3] = obj->m.inlay_gem[3];
			updated = true;
		}
		if (!(obj->m.inlay_gem[4] == m.inlay_gem[4])) {
			m.inlay_gem[4] = obj->m.inlay_gem[4];
			updated = true;
		}
		if (!(obj->m.inlay_gem[5] == m.inlay_gem[5])) {
			m.inlay_gem[5] = obj->m.inlay_gem[5];
			updated = true;
		}
		if (!(obj->m.inlay_gem[6] == m.inlay_gem[6])) {
			m.inlay_gem[6] = obj->m.inlay_gem[6];
			updated = true;
		}
		if (!(obj->m.inlay_gem[7] == m.inlay_gem[7])) {
			m.inlay_gem[7] = obj->m.inlay_gem[7];
			updated = true;
		}
		if (!(obj->m.inlay_gem[8] == m.inlay_gem[8])) {
			m.inlay_gem[8] = obj->m.inlay_gem[8];
			updated = true;
		}
		if (!(obj->m.inlay_gem[9] == m.inlay_gem[9])) {
			m.inlay_gem[9] = obj->m.inlay_gem[9];
			updated = true;
		}
		if (updated) MarkUpdateAll(inlayGemMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_STA == m.inlay_STA)) {
		m.inlay_STA = obj->m.inlay_STA;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_SPR == m.inlay_SPR)) {
		m.inlay_SPR = obj->m.inlay_SPR;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_STR == m.inlay_STR)) {
		m.inlay_STR = obj->m.inlay_STR;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_CON == m.inlay_CON)) {
		m.inlay_CON = obj->m.inlay_CON;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_AGI == m.inlay_AGI)) {
		m.inlay_AGI = obj->m.inlay_AGI;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_ATK == m.inlay_ATK)) {
		m.inlay_ATK = obj->m.inlay_ATK;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_DEF == m.inlay_DEF)) {
		m.inlay_DEF = obj->m.inlay_DEF;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_HIT == m.inlay_HIT)) {
		m.inlay_HIT = obj->m.inlay_HIT;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_EVA == m.inlay_EVA)) {
		m.inlay_EVA = obj->m.inlay_EVA;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_POW == m.inlay_POW)) {
		m.inlay_POW = obj->m.inlay_POW;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_SPD == m.inlay_SPD)) {
		m.inlay_SPD = obj->m.inlay_SPD;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_maxHP == m.inlay_maxHP)) {
		m.inlay_maxHP = obj->m.inlay_maxHP;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_maxSP == m.inlay_maxSP)) {
		m.inlay_maxSP = obj->m.inlay_maxSP;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(inlayAttrMask) && !(obj->m.inlay_maxVP == m.inlay_maxVP)) {
		m.inlay_maxVP = obj->m.inlay_maxVP;
		MarkUpdateAll(inlayAttrMask, true);
	}
	if (mask.IsBit(addonMask) && !(obj->m.addon == m.addon)) {
		m.addon = obj->m.addon;
		MarkUpdateAll(addonMask, true);
	}
	if (mask.IsBit(eqColorMask) && !(obj->m.eqColor == m.eqColor)) {
		m.eqColor = obj->m.eqColor;
		MarkUpdateAll(eqColorMask, true);
	}
	if (mask.IsBit(qualityMask) && !(obj->m.item_quality == m.item_quality)) {
		m.item_quality = obj->m.item_quality;
		MarkUpdateAll(qualityMask, true);
	}
	if (mask.IsBit(soulboundStateMask) && !(obj->m.item_soulboundState == m.item_soulboundState)) {
		m.item_soulboundState = obj->m.item_soulboundState;
		MarkUpdateAll(soulboundStateMask, true);
	}
	if (mask.IsBit(IdentifyStateMask) && !(obj->m.item_IdentifyState == m.item_IdentifyState)) {
		m.item_IdentifyState = obj->m.item_IdentifyState;
		MarkUpdateAll(IdentifyStateMask, true);
	}
	if (mask.IsBit(ExpireDateMask) && !(obj->m.item_expireDate == m.item_expireDate)) {
		m.item_expireDate = obj->m.item_expireDate;
		MarkUpdateAll(ExpireDateMask, true);
	}
	if (mask.IsBit(ValueMask) && !(obj->m.itemValue1 == m.itemValue1)) {
		m.itemValue1 = obj->m.itemValue1;
		MarkUpdateAll(ValueMask, true);
	}
	if (mask.IsBit(ValueMask) && !(obj->m.itemValue2 == m.itemValue2)) {
		m.itemValue2 = obj->m.itemValue2;
		MarkUpdateAll(ValueMask, true);
	}
	if (mask.IsBit(ValueMask) && !(obj->m.itemValue3 == m.itemValue3)) {
		m.itemValue3 = obj->m.itemValue3;
		MarkUpdateAll(ValueMask, true);
	}
	if (mask.IsBit(buffIDMask) && !(obj->m.buffID == m.buffID)) {
		m.buffID = obj->m.buffID;
		MarkUpdateAll(buffIDMask, true);
	}
	if (mask.IsBit(buffValue1Mask) && !(obj->m.buffValue1 == m.buffValue1)) {
		m.buffValue1 = obj->m.buffValue1;
		MarkUpdateAll(buffValue1Mask, true);
	}
	if (mask.IsBit(buffValue2Mask) && !(obj->m.buffValue2 == m.buffValue2)) {
		m.buffValue2 = obj->m.buffValue2;
		MarkUpdateAll(buffValue2Mask, true);
	}
	if (mask.IsBit(buffValue3Mask) && !(obj->m.buffValue3 == m.buffValue3)) {
		m.buffValue3 = obj->m.buffValue3;
		MarkUpdateAll(buffValue3Mask, true);
	}
	if (mask.IsBit(buffCoolDownMask) && !(obj->m.buffCoolDown == m.buffCoolDown)) {
		m.buffCoolDown = obj->m.buffCoolDown;
		MarkUpdateAll(buffCoolDownMask, true);
	}
	if (mask.IsBit(buffProMask) && !(obj->m.buffPro == m.buffPro)) {
		m.buffPro = obj->m.buffPro;
		MarkUpdateAll(buffProMask, true);
	}
	if (mask.IsBit(inWareHouseMask) && !(obj->m.inWareHouse == m.inWareHouse)) {
		m.inWareHouse = obj->m.inWareHouse;
		MarkUpdateAll(inWareHouseMask, true);
	}
	if (mask.IsBit(assetLockMask) && !(obj->m.lock == m.lock)) {
		m.lock = obj->m.lock;
		MarkUpdateAll(assetLockMask, true);
	}
	if (mask.IsBit(skillIDMask) && !(obj->m.skillID == m.skillID)) {
		m.skillID = obj->m.skillID;
		MarkUpdateAll(skillIDMask, true);
	}
}

void ItemData_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.item_uid);
		buf.Read<UInt32>(m.item_id);
	}
	if (m_updateMask.IsBit(owner_idMask))
	{
		buf.Read<UInt32>(m.owner_id);
	}
	if (m_updateMask.IsBit(bagIndexMask))
	{
		buf.Read<UInt8>(m.bagIndex);
	}
	if (m_updateMask.IsBit(makerMask))
	{
		buf.Read<String>(m.maker);
	}
	if (m_updateMask.IsBit(eq_elementMask))
	{
		buf.Read<UInt8>(m.eq_element);
	}
	if (m_updateMask.IsBit(endureMask))
	{
		buf.Read<UInt16>(m.endure);
	}
	if (m_updateMask.IsBit(mEndureMask))
	{
		buf.Read<UInt16>(m.mEndure);
	}
	if (m_updateMask.IsBit(gradeMask))
	{
		buf.Read<UInt16>(m.grade);
	}
	if (m_updateMask.IsBit(upgExpMask))
	{
		buf.Read<UInt16>(m.upg_exp);
	}
	if (m_updateMask.IsBit(remainMask))
	{
		buf.Read<UInt8>(m.remain);
	}
	if (m_updateMask.IsBit(stackMask))
	{
		buf.Read<UInt8>(m.stack);
	}
	if (m_updateMask.IsBit(baseAttrMask))
	{
		buf.Read<Int16>(m.base_STA);
		buf.Read<Int16>(m.base_SPR);
		buf.Read<Int16>(m.base_STR);
		buf.Read<Int16>(m.base_CON);
		buf.Read<Int16>(m.base_AGI);
		buf.Read<Int16>(m.base_ATK);
		buf.Read<Int16>(m.base_DEF);
		buf.Read<Int16>(m.base_HIT);
		buf.Read<Int16>(m.base_EVA);
		buf.Read<Int16>(m.base_POW);
		buf.Read<Int16>(m.base_SPD);
		buf.Read<Int16>(m.base_maxHP);
		buf.Read<Int16>(m.base_maxSP);
		buf.Read<Int16>(m.base_maxVP);
	}
	if (m_updateMask.IsBit(upgAttrMask))
	{
		buf.Read<Int16>(m.upg_STA);
		buf.Read<Int16>(m.upg_SPR);
		buf.Read<Int16>(m.upg_STR);
		buf.Read<Int16>(m.upg_CON);
		buf.Read<Int16>(m.upg_AGI);
		buf.Read<Int16>(m.upg_ATK);
		buf.Read<Int16>(m.upg_DEF);
		buf.Read<Int16>(m.upg_HIT);
		buf.Read<Int16>(m.upg_EVA);
		buf.Read<Int16>(m.upg_POW);
		buf.Read<Int16>(m.upg_SPD);
		buf.Read<Int16>(m.upg_maxHP);
		buf.Read<Int16>(m.upg_maxSP);
		buf.Read<Int16>(m.upg_maxVP);
	}
	if (m_updateMask.IsBit(inlayGemMask))
	{
		buf.Read<UInt8>(m.eq_gem);
		buf.ReadArray<UInt16>(m.inlay_gem, 10);
	}
	if (m_updateMask.IsBit(inlayAttrMask))
	{
		buf.Read<Int16>(m.inlay_STA);
		buf.Read<Int16>(m.inlay_SPR);
		buf.Read<Int16>(m.inlay_STR);
		buf.Read<Int16>(m.inlay_CON);
		buf.Read<Int16>(m.inlay_AGI);
		buf.Read<Int16>(m.inlay_ATK);
		buf.Read<Int16>(m.inlay_DEF);
		buf.Read<Int16>(m.inlay_HIT);
		buf.Read<Int16>(m.inlay_EVA);
		buf.Read<Int16>(m.inlay_POW);
		buf.Read<Int16>(m.inlay_SPD);
		buf.Read<Int16>(m.inlay_maxHP);
		buf.Read<Int16>(m.inlay_maxSP);
		buf.Read<Int16>(m.inlay_maxVP);
	}
	if (m_updateMask.IsBit(addonMask))
	{
		buf.Read<UInt16>(m.addon);
	}
	if (m_updateMask.IsBit(eqColorMask))
	{
		buf.Read<UInt16>(m.eqColor);
	}
	if (m_updateMask.IsBit(qualityMask))
	{
		buf.Read<UInt16>(m.item_quality);
	}
	if (m_updateMask.IsBit(soulboundStateMask))
	{
		buf.Read<UInt8>(m.item_soulboundState);
	}
	if (m_updateMask.IsBit(IdentifyStateMask))
	{
		buf.Read<UInt8>(m.item_IdentifyState);
	}
	if (m_updateMask.IsBit(ExpireDateMask))
	{
		buf.Read<UInt32>(m.item_expireDate);
	}
	if (m_updateMask.IsBit(ValueMask))
	{
		buf.Read<UInt16>(m.itemValue1);
		buf.Read<UInt16>(m.itemValue2);
		buf.Read<UInt16>(m.itemValue3);
	}
	if (m_updateMask.IsBit(buffIDMask))
	{
		buf.Read<UInt16>(m.buffID);
	}
	if (m_updateMask.IsBit(buffValue1Mask))
	{
		buf.Read<UInt32>(m.buffValue1);
	}
	if (m_updateMask.IsBit(buffValue2Mask))
	{
		buf.Read<UInt32>(m.buffValue2);
	}
	if (m_updateMask.IsBit(buffValue3Mask))
	{
		buf.Read<UInt32>(m.buffValue3);
	}
	if (m_updateMask.IsBit(buffCoolDownMask))
	{
		buf.Read<UInt32>(m.buffCoolDown);
	}
	if (m_updateMask.IsBit(buffProMask))
	{
		buf.Read<UInt8>(m.buffPro);
	}
	if (m_updateMask.IsBit(inWareHouseMask))
	{
		buf.Read<UInt8>(m.inWareHouse);
	}
	if (m_updateMask.IsBit(assetLockMask))
	{
		buf.Read<Boolean>(m.lock);
	}
	if (m_updateMask.IsBit(skillIDMask))
	{
		buf.Read<UInt16>(m.skillID);
	}
}

void ItemData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.item_uid);
		buf.Write<UInt32>(m.item_id);
	}
	if (mask.IsBit(owner_idMask))
	{
		buf.Write<UInt32>(m.owner_id);
	}
	if (mask.IsBit(bagIndexMask))
	{
		buf.Write<UInt8>(m.bagIndex);
	}
	if (mask.IsBit(makerMask))
	{
		buf.Write<String>(m.maker);
	}
	if (mask.IsBit(eq_elementMask))
	{
		buf.Write<UInt8>(m.eq_element);
	}
	if (mask.IsBit(endureMask))
	{
		buf.Write<UInt16>(m.endure);
	}
	if (mask.IsBit(mEndureMask))
	{
		buf.Write<UInt16>(m.mEndure);
	}
	if (mask.IsBit(gradeMask))
	{
		buf.Write<UInt16>(m.grade);
	}
	if (mask.IsBit(upgExpMask))
	{
		buf.Write<UInt16>(m.upg_exp);
	}
	if (mask.IsBit(remainMask))
	{
		buf.Write<UInt8>(m.remain);
	}
	if (mask.IsBit(stackMask))
	{
		buf.Write<UInt8>(m.stack);
	}
	if (mask.IsBit(baseAttrMask))
	{
		buf.Write<Int16>(m.base_STA);
		buf.Write<Int16>(m.base_SPR);
		buf.Write<Int16>(m.base_STR);
		buf.Write<Int16>(m.base_CON);
		buf.Write<Int16>(m.base_AGI);
		buf.Write<Int16>(m.base_ATK);
		buf.Write<Int16>(m.base_DEF);
		buf.Write<Int16>(m.base_HIT);
		buf.Write<Int16>(m.base_EVA);
		buf.Write<Int16>(m.base_POW);
		buf.Write<Int16>(m.base_SPD);
		buf.Write<Int16>(m.base_maxHP);
		buf.Write<Int16>(m.base_maxSP);
		buf.Write<Int16>(m.base_maxVP);
	}
	if (mask.IsBit(upgAttrMask))
	{
		buf.Write<Int16>(m.upg_STA);
		buf.Write<Int16>(m.upg_SPR);
		buf.Write<Int16>(m.upg_STR);
		buf.Write<Int16>(m.upg_CON);
		buf.Write<Int16>(m.upg_AGI);
		buf.Write<Int16>(m.upg_ATK);
		buf.Write<Int16>(m.upg_DEF);
		buf.Write<Int16>(m.upg_HIT);
		buf.Write<Int16>(m.upg_EVA);
		buf.Write<Int16>(m.upg_POW);
		buf.Write<Int16>(m.upg_SPD);
		buf.Write<Int16>(m.upg_maxHP);
		buf.Write<Int16>(m.upg_maxSP);
		buf.Write<Int16>(m.upg_maxVP);
	}
	if (mask.IsBit(inlayGemMask))
	{
		buf.Write<UInt8>(m.eq_gem);
		buf.WriteArray<UInt16>(m.inlay_gem, 10);
	}
	if (mask.IsBit(inlayAttrMask))
	{
		buf.Write<Int16>(m.inlay_STA);
		buf.Write<Int16>(m.inlay_SPR);
		buf.Write<Int16>(m.inlay_STR);
		buf.Write<Int16>(m.inlay_CON);
		buf.Write<Int16>(m.inlay_AGI);
		buf.Write<Int16>(m.inlay_ATK);
		buf.Write<Int16>(m.inlay_DEF);
		buf.Write<Int16>(m.inlay_HIT);
		buf.Write<Int16>(m.inlay_EVA);
		buf.Write<Int16>(m.inlay_POW);
		buf.Write<Int16>(m.inlay_SPD);
		buf.Write<Int16>(m.inlay_maxHP);
		buf.Write<Int16>(m.inlay_maxSP);
		buf.Write<Int16>(m.inlay_maxVP);
	}
	if (mask.IsBit(addonMask))
	{
		buf.Write<UInt16>(m.addon);
	}
	if (mask.IsBit(eqColorMask))
	{
		buf.Write<UInt16>(m.eqColor);
	}
	if (mask.IsBit(qualityMask))
	{
		buf.Write<UInt16>(m.item_quality);
	}
	if (mask.IsBit(soulboundStateMask))
	{
		buf.Write<UInt8>(m.item_soulboundState);
	}
	if (mask.IsBit(IdentifyStateMask))
	{
		buf.Write<UInt8>(m.item_IdentifyState);
	}
	if (mask.IsBit(ExpireDateMask))
	{
		buf.Write<UInt32>(m.item_expireDate);
	}
	if (mask.IsBit(ValueMask))
	{
		buf.Write<UInt16>(m.itemValue1);
		buf.Write<UInt16>(m.itemValue2);
		buf.Write<UInt16>(m.itemValue3);
	}
	if (mask.IsBit(buffIDMask))
	{
		buf.Write<UInt16>(m.buffID);
	}
	if (mask.IsBit(buffValue1Mask))
	{
		buf.Write<UInt32>(m.buffValue1);
	}
	if (mask.IsBit(buffValue2Mask))
	{
		buf.Write<UInt32>(m.buffValue2);
	}
	if (mask.IsBit(buffValue3Mask))
	{
		buf.Write<UInt32>(m.buffValue3);
	}
	if (mask.IsBit(buffCoolDownMask))
	{
		buf.Write<UInt32>(m.buffCoolDown);
	}
	if (mask.IsBit(buffProMask))
	{
		buf.Write<UInt8>(m.buffPro);
	}
	if (mask.IsBit(inWareHouseMask))
	{
		buf.Write<UInt8>(m.inWareHouse);
	}
	if (mask.IsBit(assetLockMask))
	{
		buf.Write<Boolean>(m.lock);
	}
	if (mask.IsBit(skillIDMask))
	{
		buf.Write<UInt16>(m.skillID);
	}
}

RPCResult ItemData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ItemData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult ItemData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ItemData_REQ_STATE:
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

