//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/ItemData_proxy.h"

void ItemData_Proxy::SetId(const UInt32 &item_uid, const UInt32 &item_id)
{
	bool modified = false;
	if (m.item_uid != item_uid) { m.item_uid = item_uid; modified = true; }
	if (m.item_id != item_id) { m.item_id = item_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void ItemData_Proxy::SetItem_uid(const UInt32 &item_uid)
{
	bool modified = false;
	if (m.item_uid != item_uid) { m.item_uid = item_uid; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void ItemData_Proxy::SetItem_id(const UInt32 &item_id)
{
	bool modified = false;
	if (m.item_id != item_id) { m.item_id = item_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void ItemData_Proxy::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(owner_idMask, modified);
}

void ItemData_Proxy::SetBagIndex(const UInt8 &bagIndex)
{
	bool modified = false;
	if (m.bagIndex != bagIndex) { m.bagIndex = bagIndex; modified = true; }
	MarkUpdateAll(bagIndexMask, modified);
}

void ItemData_Proxy::SetMaker(const String &maker)
{
	bool modified = false;
	if (m.maker != maker) { m.maker = maker; modified = true; }
	MarkUpdateAll(makerMask, modified);
}

void ItemData_Proxy::SetEq_element(const UInt8 &eq_element)
{
	bool modified = false;
	if (m.eq_element != eq_element) { m.eq_element = eq_element; modified = true; }
	MarkUpdateAll(eq_elementMask, modified);
}

void ItemData_Proxy::SetEndure(const UInt16 &endure)
{
	bool modified = false;
	if (m.endure != endure) { m.endure = endure; modified = true; }
	MarkUpdateAll(endureMask, modified);
}

void ItemData_Proxy::SetMEndure(const UInt16 &mEndure)
{
	bool modified = false;
	if (m.mEndure != mEndure) { m.mEndure = mEndure; modified = true; }
	MarkUpdateAll(mEndureMask, modified);
}

void ItemData_Proxy::SetGrade(const UInt16 &grade)
{
	bool modified = false;
	if (m.grade != grade) { m.grade = grade; modified = true; }
	MarkUpdateAll(gradeMask, modified);
}

void ItemData_Proxy::SetUpgExp(const UInt16 &upg_exp)
{
	bool modified = false;
	if (m.upg_exp != upg_exp) { m.upg_exp = upg_exp; modified = true; }
	MarkUpdateAll(upgExpMask, modified);
}

void ItemData_Proxy::SetUpg_exp(const UInt16 &upg_exp)
{
	bool modified = false;
	if (m.upg_exp != upg_exp) { m.upg_exp = upg_exp; modified = true; }
	MarkUpdateAll(upgExpMask, modified);
}

void ItemData_Proxy::SetRemain(const UInt8 &remain)
{
	bool modified = false;
	if (m.remain != remain) { m.remain = remain; modified = true; }
	MarkUpdateAll(remainMask, modified);
}

void ItemData_Proxy::SetStack(const UInt8 &stack)
{
	bool modified = false;
	if (m.stack != stack) { m.stack = stack; modified = true; }
	MarkUpdateAll(stackMask, modified);
}

void ItemData_Proxy::SetBaseAttr(const Int16 &base_STA, const Int16 &base_SPR, const Int16 &base_STR, const Int16 &base_CON, const Int16 &base_AGI, const Int16 &base_ATK, const Int16 &base_DEF, const Int16 &base_HIT, const Int16 &base_EVA, const Int16 &base_POW, const Int16 &base_SPD, const Int16 &base_maxHP, const Int16 &base_maxSP, const Int16 &base_maxVP)
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

void ItemData_Proxy::SetBase_STA(const Int16 &base_STA)
{
	bool modified = false;
	if (m.base_STA != base_STA) { m.base_STA = base_STA; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_SPR(const Int16 &base_SPR)
{
	bool modified = false;
	if (m.base_SPR != base_SPR) { m.base_SPR = base_SPR; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_STR(const Int16 &base_STR)
{
	bool modified = false;
	if (m.base_STR != base_STR) { m.base_STR = base_STR; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_CON(const Int16 &base_CON)
{
	bool modified = false;
	if (m.base_CON != base_CON) { m.base_CON = base_CON; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_AGI(const Int16 &base_AGI)
{
	bool modified = false;
	if (m.base_AGI != base_AGI) { m.base_AGI = base_AGI; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_ATK(const Int16 &base_ATK)
{
	bool modified = false;
	if (m.base_ATK != base_ATK) { m.base_ATK = base_ATK; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_DEF(const Int16 &base_DEF)
{
	bool modified = false;
	if (m.base_DEF != base_DEF) { m.base_DEF = base_DEF; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_HIT(const Int16 &base_HIT)
{
	bool modified = false;
	if (m.base_HIT != base_HIT) { m.base_HIT = base_HIT; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_EVA(const Int16 &base_EVA)
{
	bool modified = false;
	if (m.base_EVA != base_EVA) { m.base_EVA = base_EVA; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_POW(const Int16 &base_POW)
{
	bool modified = false;
	if (m.base_POW != base_POW) { m.base_POW = base_POW; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_SPD(const Int16 &base_SPD)
{
	bool modified = false;
	if (m.base_SPD != base_SPD) { m.base_SPD = base_SPD; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_maxHP(const Int16 &base_maxHP)
{
	bool modified = false;
	if (m.base_maxHP != base_maxHP) { m.base_maxHP = base_maxHP; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_maxSP(const Int16 &base_maxSP)
{
	bool modified = false;
	if (m.base_maxSP != base_maxSP) { m.base_maxSP = base_maxSP; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetBase_maxVP(const Int16 &base_maxVP)
{
	bool modified = false;
	if (m.base_maxVP != base_maxVP) { m.base_maxVP = base_maxVP; modified = true; }
	MarkUpdateAll(baseAttrMask, modified);
}

void ItemData_Proxy::SetUpgAttr(const Int16 &upg_STA, const Int16 &upg_SPR, const Int16 &upg_STR, const Int16 &upg_CON, const Int16 &upg_AGI, const Int16 &upg_ATK, const Int16 &upg_DEF, const Int16 &upg_HIT, const Int16 &upg_EVA, const Int16 &upg_POW, const Int16 &upg_SPD, const Int16 &upg_maxHP, const Int16 &upg_maxSP, const Int16 &upg_maxVP)
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

void ItemData_Proxy::SetUpg_STA(const Int16 &upg_STA)
{
	bool modified = false;
	if (m.upg_STA != upg_STA) { m.upg_STA = upg_STA; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_SPR(const Int16 &upg_SPR)
{
	bool modified = false;
	if (m.upg_SPR != upg_SPR) { m.upg_SPR = upg_SPR; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_STR(const Int16 &upg_STR)
{
	bool modified = false;
	if (m.upg_STR != upg_STR) { m.upg_STR = upg_STR; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_CON(const Int16 &upg_CON)
{
	bool modified = false;
	if (m.upg_CON != upg_CON) { m.upg_CON = upg_CON; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_AGI(const Int16 &upg_AGI)
{
	bool modified = false;
	if (m.upg_AGI != upg_AGI) { m.upg_AGI = upg_AGI; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_ATK(const Int16 &upg_ATK)
{
	bool modified = false;
	if (m.upg_ATK != upg_ATK) { m.upg_ATK = upg_ATK; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_DEF(const Int16 &upg_DEF)
{
	bool modified = false;
	if (m.upg_DEF != upg_DEF) { m.upg_DEF = upg_DEF; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_HIT(const Int16 &upg_HIT)
{
	bool modified = false;
	if (m.upg_HIT != upg_HIT) { m.upg_HIT = upg_HIT; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_EVA(const Int16 &upg_EVA)
{
	bool modified = false;
	if (m.upg_EVA != upg_EVA) { m.upg_EVA = upg_EVA; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_POW(const Int16 &upg_POW)
{
	bool modified = false;
	if (m.upg_POW != upg_POW) { m.upg_POW = upg_POW; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_SPD(const Int16 &upg_SPD)
{
	bool modified = false;
	if (m.upg_SPD != upg_SPD) { m.upg_SPD = upg_SPD; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_maxHP(const Int16 &upg_maxHP)
{
	bool modified = false;
	if (m.upg_maxHP != upg_maxHP) { m.upg_maxHP = upg_maxHP; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_maxSP(const Int16 &upg_maxSP)
{
	bool modified = false;
	if (m.upg_maxSP != upg_maxSP) { m.upg_maxSP = upg_maxSP; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetUpg_maxVP(const Int16 &upg_maxVP)
{
	bool modified = false;
	if (m.upg_maxVP != upg_maxVP) { m.upg_maxVP = upg_maxVP; modified = true; }
	MarkUpdateAll(upgAttrMask, modified);
}

void ItemData_Proxy::SetInlayGem(const UInt8 &eq_gem, const UInt16 &inlay_gem0, const UInt16 &inlay_gem1, const UInt16 &inlay_gem2, const UInt16 &inlay_gem3, const UInt16 &inlay_gem4, const UInt16 &inlay_gem5, const UInt16 &inlay_gem6, const UInt16 &inlay_gem7, const UInt16 &inlay_gem8, const UInt16 &inlay_gem9)
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

void ItemData_Proxy::SetEq_gem(const UInt8 &eq_gem)
{
	bool modified = false;
	if (m.eq_gem != eq_gem) { m.eq_gem = eq_gem; modified = true; }
	MarkUpdateAll(inlayGemMask, modified);
}

void ItemData_Proxy::SetInlay_gem(const UInt8 i, const UInt16 &inlay_gem)
{
	bool modified = false;
	if (m.inlay_gem[i] != inlay_gem) { m.inlay_gem[i] = inlay_gem; modified = true; }
	MarkUpdateAll(inlayGemMask, modified);
}

void ItemData_Proxy::SetInlayAttr(const Int16 &inlay_STA, const Int16 &inlay_SPR, const Int16 &inlay_STR, const Int16 &inlay_CON, const Int16 &inlay_AGI, const Int16 &inlay_ATK, const Int16 &inlay_DEF, const Int16 &inlay_HIT, const Int16 &inlay_EVA, const Int16 &inlay_POW, const Int16 &inlay_SPD, const Int16 &inlay_maxHP, const Int16 &inlay_maxSP, const Int16 &inlay_maxVP)
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

void ItemData_Proxy::SetInlay_STA(const Int16 &inlay_STA)
{
	bool modified = false;
	if (m.inlay_STA != inlay_STA) { m.inlay_STA = inlay_STA; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_SPR(const Int16 &inlay_SPR)
{
	bool modified = false;
	if (m.inlay_SPR != inlay_SPR) { m.inlay_SPR = inlay_SPR; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_STR(const Int16 &inlay_STR)
{
	bool modified = false;
	if (m.inlay_STR != inlay_STR) { m.inlay_STR = inlay_STR; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_CON(const Int16 &inlay_CON)
{
	bool modified = false;
	if (m.inlay_CON != inlay_CON) { m.inlay_CON = inlay_CON; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_AGI(const Int16 &inlay_AGI)
{
	bool modified = false;
	if (m.inlay_AGI != inlay_AGI) { m.inlay_AGI = inlay_AGI; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_ATK(const Int16 &inlay_ATK)
{
	bool modified = false;
	if (m.inlay_ATK != inlay_ATK) { m.inlay_ATK = inlay_ATK; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_DEF(const Int16 &inlay_DEF)
{
	bool modified = false;
	if (m.inlay_DEF != inlay_DEF) { m.inlay_DEF = inlay_DEF; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_HIT(const Int16 &inlay_HIT)
{
	bool modified = false;
	if (m.inlay_HIT != inlay_HIT) { m.inlay_HIT = inlay_HIT; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_EVA(const Int16 &inlay_EVA)
{
	bool modified = false;
	if (m.inlay_EVA != inlay_EVA) { m.inlay_EVA = inlay_EVA; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_POW(const Int16 &inlay_POW)
{
	bool modified = false;
	if (m.inlay_POW != inlay_POW) { m.inlay_POW = inlay_POW; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_SPD(const Int16 &inlay_SPD)
{
	bool modified = false;
	if (m.inlay_SPD != inlay_SPD) { m.inlay_SPD = inlay_SPD; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_maxHP(const Int16 &inlay_maxHP)
{
	bool modified = false;
	if (m.inlay_maxHP != inlay_maxHP) { m.inlay_maxHP = inlay_maxHP; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_maxSP(const Int16 &inlay_maxSP)
{
	bool modified = false;
	if (m.inlay_maxSP != inlay_maxSP) { m.inlay_maxSP = inlay_maxSP; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetInlay_maxVP(const Int16 &inlay_maxVP)
{
	bool modified = false;
	if (m.inlay_maxVP != inlay_maxVP) { m.inlay_maxVP = inlay_maxVP; modified = true; }
	MarkUpdateAll(inlayAttrMask, modified);
}

void ItemData_Proxy::SetAddon(const UInt16 &addon)
{
	bool modified = false;
	if (m.addon != addon) { m.addon = addon; modified = true; }
	MarkUpdateAll(addonMask, modified);
}

void ItemData_Proxy::SetEqColor(const UInt16 &eqColor)
{
	bool modified = false;
	if (m.eqColor != eqColor) { m.eqColor = eqColor; modified = true; }
	MarkUpdateAll(eqColorMask, modified);
}

void ItemData_Proxy::SetQuality(const UInt16 &item_quality)
{
	bool modified = false;
	if (m.item_quality != item_quality) { m.item_quality = item_quality; modified = true; }
	MarkUpdateAll(qualityMask, modified);
}

void ItemData_Proxy::SetItem_quality(const UInt16 &item_quality)
{
	bool modified = false;
	if (m.item_quality != item_quality) { m.item_quality = item_quality; modified = true; }
	MarkUpdateAll(qualityMask, modified);
}

void ItemData_Proxy::SetSoulboundState(const UInt8 &item_soulboundState)
{
	bool modified = false;
	if (m.item_soulboundState != item_soulboundState) { m.item_soulboundState = item_soulboundState; modified = true; }
	MarkUpdateAll(soulboundStateMask, modified);
}

void ItemData_Proxy::SetItem_soulboundState(const UInt8 &item_soulboundState)
{
	bool modified = false;
	if (m.item_soulboundState != item_soulboundState) { m.item_soulboundState = item_soulboundState; modified = true; }
	MarkUpdateAll(soulboundStateMask, modified);
}

void ItemData_Proxy::SetIdentifyState(const UInt8 &item_IdentifyState)
{
	bool modified = false;
	if (m.item_IdentifyState != item_IdentifyState) { m.item_IdentifyState = item_IdentifyState; modified = true; }
	MarkUpdateAll(IdentifyStateMask, modified);
}

void ItemData_Proxy::SetItem_IdentifyState(const UInt8 &item_IdentifyState)
{
	bool modified = false;
	if (m.item_IdentifyState != item_IdentifyState) { m.item_IdentifyState = item_IdentifyState; modified = true; }
	MarkUpdateAll(IdentifyStateMask, modified);
}

void ItemData_Proxy::SetExpireDate(const UInt32 &item_expireDate)
{
	bool modified = false;
	if (m.item_expireDate != item_expireDate) { m.item_expireDate = item_expireDate; modified = true; }
	MarkUpdateAll(ExpireDateMask, modified);
}

void ItemData_Proxy::SetItem_expireDate(const UInt32 &item_expireDate)
{
	bool modified = false;
	if (m.item_expireDate != item_expireDate) { m.item_expireDate = item_expireDate; modified = true; }
	MarkUpdateAll(ExpireDateMask, modified);
}

void ItemData_Proxy::SetValue(const UInt16 &itemValue1, const UInt16 &itemValue2, const UInt16 &itemValue3)
{
	bool modified = false;
	if (m.itemValue1 != itemValue1) { m.itemValue1 = itemValue1; modified = true; }
	if (m.itemValue2 != itemValue2) { m.itemValue2 = itemValue2; modified = true; }
	if (m.itemValue3 != itemValue3) { m.itemValue3 = itemValue3; modified = true; }
	MarkUpdateAll(ValueMask, modified);
}

void ItemData_Proxy::SetItemValue1(const UInt16 &itemValue1)
{
	bool modified = false;
	if (m.itemValue1 != itemValue1) { m.itemValue1 = itemValue1; modified = true; }
	MarkUpdateAll(ValueMask, modified);
}

void ItemData_Proxy::SetItemValue2(const UInt16 &itemValue2)
{
	bool modified = false;
	if (m.itemValue2 != itemValue2) { m.itemValue2 = itemValue2; modified = true; }
	MarkUpdateAll(ValueMask, modified);
}

void ItemData_Proxy::SetItemValue3(const UInt16 &itemValue3)
{
	bool modified = false;
	if (m.itemValue3 != itemValue3) { m.itemValue3 = itemValue3; modified = true; }
	MarkUpdateAll(ValueMask, modified);
}

void ItemData_Proxy::SetBuffID(const UInt16 &buffID)
{
	bool modified = false;
	if (m.buffID != buffID) { m.buffID = buffID; modified = true; }
	MarkUpdateAll(buffIDMask, modified);
}

void ItemData_Proxy::SetBuffValue1(const UInt32 &buffValue1)
{
	bool modified = false;
	if (m.buffValue1 != buffValue1) { m.buffValue1 = buffValue1; modified = true; }
	MarkUpdateAll(buffValue1Mask, modified);
}

void ItemData_Proxy::SetBuffValue2(const UInt32 &buffValue2)
{
	bool modified = false;
	if (m.buffValue2 != buffValue2) { m.buffValue2 = buffValue2; modified = true; }
	MarkUpdateAll(buffValue2Mask, modified);
}

void ItemData_Proxy::SetBuffValue3(const UInt32 &buffValue3)
{
	bool modified = false;
	if (m.buffValue3 != buffValue3) { m.buffValue3 = buffValue3; modified = true; }
	MarkUpdateAll(buffValue3Mask, modified);
}

void ItemData_Proxy::SetBuffCoolDown(const UInt32 &buffCoolDown)
{
	bool modified = false;
	if (m.buffCoolDown != buffCoolDown) { m.buffCoolDown = buffCoolDown; modified = true; }
	MarkUpdateAll(buffCoolDownMask, modified);
}

void ItemData_Proxy::SetBuffPro(const UInt8 &buffPro)
{
	bool modified = false;
	if (m.buffPro != buffPro) { m.buffPro = buffPro; modified = true; }
	MarkUpdateAll(buffProMask, modified);
}

void ItemData_Proxy::SetInWareHouse(const UInt8 &inWareHouse)
{
	bool modified = false;
	if (m.inWareHouse != inWareHouse) { m.inWareHouse = inWareHouse; modified = true; }
	MarkUpdateAll(inWareHouseMask, modified);
}

void ItemData_Proxy::SetAssetLock(const Boolean &lock)
{
	bool modified = false;
	if (m.lock != lock) { m.lock = lock; modified = true; }
	MarkUpdateAll(assetLockMask, modified);
}

void ItemData_Proxy::SetLock(const Boolean &lock)
{
	bool modified = false;
	if (m.lock != lock) { m.lock = lock; modified = true; }
	MarkUpdateAll(assetLockMask, modified);
}

void ItemData_Proxy::SetSkillID(const UInt16 &skillID)
{
	bool modified = false;
	if (m.skillID != skillID) { m.skillID = skillID; modified = true; }
	MarkUpdateAll(skillIDMask, modified);
}


void ItemData_Proxy::ReadAttr(CONST Buffer &buf)
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

void ItemData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

void ItemData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.item_uid, m.item_id);
	if (IsUpdated(owner_idMask)) OnUpdateOwner_id(m.owner_id);
	if (IsUpdated(bagIndexMask)) OnUpdateBagIndex(m.bagIndex);
	if (IsUpdated(makerMask)) OnUpdateMaker(m.maker);
	if (IsUpdated(eq_elementMask)) OnUpdateEq_element(m.eq_element);
	if (IsUpdated(endureMask)) OnUpdateEndure(m.endure);
	if (IsUpdated(mEndureMask)) OnUpdateMEndure(m.mEndure);
	if (IsUpdated(gradeMask)) OnUpdateGrade(m.grade);
	if (IsUpdated(upgExpMask)) OnUpdateUpgExp(m.upg_exp);
	if (IsUpdated(remainMask)) OnUpdateRemain(m.remain);
	if (IsUpdated(stackMask)) OnUpdateStack(m.stack);
	if (IsUpdated(baseAttrMask)) OnUpdateBaseAttr(m.base_STA, m.base_SPR, m.base_STR, m.base_CON, m.base_AGI, m.base_ATK, m.base_DEF, m.base_HIT, m.base_EVA, m.base_POW, m.base_SPD, m.base_maxHP, m.base_maxSP, m.base_maxVP);
	if (IsUpdated(upgAttrMask)) OnUpdateUpgAttr(m.upg_STA, m.upg_SPR, m.upg_STR, m.upg_CON, m.upg_AGI, m.upg_ATK, m.upg_DEF, m.upg_HIT, m.upg_EVA, m.upg_POW, m.upg_SPD, m.upg_maxHP, m.upg_maxSP, m.upg_maxVP);
	if (IsUpdated(inlayGemMask)) OnUpdateInlayGem(m.eq_gem, m.inlay_gem[0], m.inlay_gem[1], m.inlay_gem[2], m.inlay_gem[3], m.inlay_gem[4], m.inlay_gem[5], m.inlay_gem[6], m.inlay_gem[7], m.inlay_gem[8], m.inlay_gem[9]);
	if (IsUpdated(inlayAttrMask)) OnUpdateInlayAttr(m.inlay_STA, m.inlay_SPR, m.inlay_STR, m.inlay_CON, m.inlay_AGI, m.inlay_ATK, m.inlay_DEF, m.inlay_HIT, m.inlay_EVA, m.inlay_POW, m.inlay_SPD, m.inlay_maxHP, m.inlay_maxSP, m.inlay_maxVP);
	if (IsUpdated(addonMask)) OnUpdateAddon(m.addon);
	if (IsUpdated(eqColorMask)) OnUpdateEqColor(m.eqColor);
	if (IsUpdated(qualityMask)) OnUpdateQuality(m.item_quality);
	if (IsUpdated(soulboundStateMask)) OnUpdateSoulboundState(m.item_soulboundState);
	if (IsUpdated(IdentifyStateMask)) OnUpdateIdentifyState(m.item_IdentifyState);
	if (IsUpdated(ExpireDateMask)) OnUpdateExpireDate(m.item_expireDate);
	if (IsUpdated(ValueMask)) OnUpdateValue(m.itemValue1, m.itemValue2, m.itemValue3);
	if (IsUpdated(buffIDMask)) OnUpdateBuffID(m.buffID);
	if (IsUpdated(buffValue1Mask)) OnUpdateBuffValue1(m.buffValue1);
	if (IsUpdated(buffValue2Mask)) OnUpdateBuffValue2(m.buffValue2);
	if (IsUpdated(buffValue3Mask)) OnUpdateBuffValue3(m.buffValue3);
	if (IsUpdated(buffCoolDownMask)) OnUpdateBuffCoolDown(m.buffCoolDown);
	if (IsUpdated(buffProMask)) OnUpdateBuffPro(m.buffPro);
	if (IsUpdated(inWareHouseMask)) OnUpdateInWareHouse(m.inWareHouse);
	if (IsUpdated(assetLockMask)) OnUpdateAssetLock(m.lock);
	if (IsUpdated(skillIDMask)) OnUpdateSkillID(m.skillID);
}

#pragma optimize("", off)
RPCResult ItemData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ItemData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult ItemData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_ItemData_UP_STATE:
			_result = RPCDecode<2138537034>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
