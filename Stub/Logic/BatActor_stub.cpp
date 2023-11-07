//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/BatActor_stub.h"

void BatActor_Stub::SetInfo2(const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction)
{
	bool modified = false;
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	if (m.sexClass != sexClass) { m.sexClass = sexClass; modified = true; }
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(info2Mask, modified);
}

void BatActor_Stub::SetCclass(const UInt8 &cclass)
{
	bool modified = false;
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	MarkUpdateAll(info2Mask, modified);
}

void BatActor_Stub::SetSexClass(const UInt8 &sexClass)
{
	bool modified = false;
	if (m.sexClass != sexClass) { m.sexClass = sexClass; modified = true; }
	MarkUpdateAll(info2Mask, modified);
}

void BatActor_Stub::SetFaction(const UInt8 &faction)
{
	bool modified = false;
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(info2Mask, modified);
}

void BatActor_Stub::SetIndex(const UInt8 &index)
{
	bool modified = false;
	if (m.index != index) { m.index = index; modified = true; }
	MarkUpdateAll(indexMask, modified);
}

void BatActor_Stub::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void BatActor_Stub::SetAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI)
{
	bool modified = false;
	if (m.STA != STA) { m.STA = STA; modified = true; }
	if (m.SPR != SPR) { m.SPR = SPR; modified = true; }
	if (m.STR != STR) { m.STR = STR; modified = true; }
	if (m.CON != CON) { m.CON = CON; modified = true; }
	if (m.AGI != AGI) { m.AGI = AGI; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void BatActor_Stub::SetSTA(const UInt16 &STA)
{
	bool modified = false;
	if (m.STA != STA) { m.STA = STA; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void BatActor_Stub::SetSPR(const UInt16 &SPR)
{
	bool modified = false;
	if (m.SPR != SPR) { m.SPR = SPR; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void BatActor_Stub::SetSTR(const UInt16 &STR)
{
	bool modified = false;
	if (m.STR != STR) { m.STR = STR; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void BatActor_Stub::SetCON(const UInt16 &CON)
{
	bool modified = false;
	if (m.CON != CON) { m.CON = CON; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void BatActor_Stub::SetAGI(const UInt16 &AGI)
{
	bool modified = false;
	if (m.AGI != AGI) { m.AGI = AGI; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void BatActor_Stub::SetAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD)
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

void BatActor_Stub::SetATK(const UInt16 &ATK)
{
	bool modified = false;
	if (m.ATK != ATK) { m.ATK = ATK; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void BatActor_Stub::SetDEF(const UInt16 &DEF)
{
	bool modified = false;
	if (m.DEF != DEF) { m.DEF = DEF; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void BatActor_Stub::SetHIT(const UInt16 &HIT)
{
	bool modified = false;
	if (m.HIT != HIT) { m.HIT = HIT; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void BatActor_Stub::SetEVA(const UInt16 &EVA)
{
	bool modified = false;
	if (m.EVA != EVA) { m.EVA = EVA; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void BatActor_Stub::SetPOW(const UInt16 &POW)
{
	bool modified = false;
	if (m.POW != POW) { m.POW = POW; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void BatActor_Stub::SetSPD(const UInt16 &SPD)
{
	bool modified = false;
	if (m.SPD != SPD) { m.SPD = SPD; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void BatActor_Stub::SetHP(const UInt32 &HP, const Int32 &dHP, const Int32 &dShowHP)
{
	bool modified = false;
	if (m.HP != HP) { m.HP = HP; modified = true; }
	if (m.dHP != dHP) { m.dHP = dHP; modified = true; }
	if (m.dShowHP != dShowHP) { m.dShowHP = dShowHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void BatActor_Stub::SetHP(const UInt32 &HP)
{
	bool modified = false;
	if (m.HP != HP) { m.HP = HP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void BatActor_Stub::SetDHP(const Int32 &dHP)
{
	bool modified = false;
	if (m.dHP != dHP) { m.dHP = dHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void BatActor_Stub::SetDShowHP(const Int32 &dShowHP)
{
	bool modified = false;
	if (m.dShowHP != dShowHP) { m.dShowHP = dShowHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void BatActor_Stub::SetMHP(const UInt32 &mHP, const Int32 &dMHP, const Int32 &dShowMHP)
{
	bool modified = false;
	if (m.mHP != mHP) { m.mHP = mHP; modified = true; }
	if (m.dMHP != dMHP) { m.dMHP = dMHP; modified = true; }
	if (m.dShowMHP != dShowMHP) { m.dShowMHP = dShowMHP; modified = true; }
	MarkUpdateAll(mHPMask, modified);
}

void BatActor_Stub::SetMHP(const UInt32 &mHP)
{
	bool modified = false;
	if (m.mHP != mHP) { m.mHP = mHP; modified = true; }
	MarkUpdateAll(mHPMask, modified);
}

void BatActor_Stub::SetDMHP(const Int32 &dMHP)
{
	bool modified = false;
	if (m.dMHP != dMHP) { m.dMHP = dMHP; modified = true; }
	MarkUpdateAll(mHPMask, modified);
}

void BatActor_Stub::SetDShowMHP(const Int32 &dShowMHP)
{
	bool modified = false;
	if (m.dShowMHP != dShowMHP) { m.dShowMHP = dShowMHP; modified = true; }
	MarkUpdateAll(mHPMask, modified);
}

void BatActor_Stub::SetMaxHP(const UInt32 &maxHP)
{
	bool modified = false;
	if (m.maxHP != maxHP) { m.maxHP = maxHP; modified = true; }
	MarkUpdateAll(maxHPMask, modified);
}

void BatActor_Stub::SetSP(const UInt32 &SP, const Int32 &dSP, const Int32 &dShowSP)
{
	bool modified = false;
	if (m.SP != SP) { m.SP = SP; modified = true; }
	if (m.dSP != dSP) { m.dSP = dSP; modified = true; }
	if (m.dShowSP != dShowSP) { m.dShowSP = dShowSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void BatActor_Stub::SetSP(const UInt32 &SP)
{
	bool modified = false;
	if (m.SP != SP) { m.SP = SP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void BatActor_Stub::SetDSP(const Int32 &dSP)
{
	bool modified = false;
	if (m.dSP != dSP) { m.dSP = dSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void BatActor_Stub::SetDShowSP(const Int32 &dShowSP)
{
	bool modified = false;
	if (m.dShowSP != dShowSP) { m.dShowSP = dShowSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void BatActor_Stub::SetMSP(const UInt32 &mSP, const Int32 &dMSP, const Int32 &dShowMSP)
{
	bool modified = false;
	if (m.mSP != mSP) { m.mSP = mSP; modified = true; }
	if (m.dMSP != dMSP) { m.dMSP = dMSP; modified = true; }
	if (m.dShowMSP != dShowMSP) { m.dShowMSP = dShowMSP; modified = true; }
	MarkUpdateAll(mSPMask, modified);
}

void BatActor_Stub::SetMSP(const UInt32 &mSP)
{
	bool modified = false;
	if (m.mSP != mSP) { m.mSP = mSP; modified = true; }
	MarkUpdateAll(mSPMask, modified);
}

void BatActor_Stub::SetDMSP(const Int32 &dMSP)
{
	bool modified = false;
	if (m.dMSP != dMSP) { m.dMSP = dMSP; modified = true; }
	MarkUpdateAll(mSPMask, modified);
}

void BatActor_Stub::SetDShowMSP(const Int32 &dShowMSP)
{
	bool modified = false;
	if (m.dShowMSP != dShowMSP) { m.dShowMSP = dShowMSP; modified = true; }
	MarkUpdateAll(mSPMask, modified);
}

void BatActor_Stub::SetMaxSP(const UInt32 &maxSP)
{
	bool modified = false;
	if (m.maxSP != maxSP) { m.maxSP = maxSP; modified = true; }
	MarkUpdateAll(maxSPMask, modified);
}

void BatActor_Stub::SetDP(const UInt16 &DP, const Int16 &dDP, const Int16 &dShowDP)
{
	bool modified = false;
	if (m.DP != DP) { m.DP = DP; modified = true; }
	if (m.dDP != dDP) { m.dDP = dDP; modified = true; }
	if (m.dShowDP != dShowDP) { m.dShowDP = dShowDP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void BatActor_Stub::SetDP(const UInt16 &DP)
{
	bool modified = false;
	if (m.DP != DP) { m.DP = DP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void BatActor_Stub::SetDDP(const Int16 &dDP)
{
	bool modified = false;
	if (m.dDP != dDP) { m.dDP = dDP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void BatActor_Stub::SetDShowDP(const Int16 &dShowDP)
{
	bool modified = false;
	if (m.dShowDP != dShowDP) { m.dShowDP = dShowDP; modified = true; }
	MarkUpdateAll(DPMask, modified);
}

void BatActor_Stub::SetMDP(const UInt16 &mDP)
{
	bool modified = false;
	if (m.mDP != mDP) { m.mDP = mDP; modified = true; }
	MarkUpdateAll(mDPMask, modified);
}

void BatActor_Stub::SetMaxDP(const UInt16 &maxDP)
{
	bool modified = false;
	if (m.maxDP != maxDP) { m.maxDP = maxDP; modified = true; }
	MarkUpdateAll(maxDPMask, modified);
}

void BatActor_Stub::SetWC(const UInt16 &WC, const Int16 &dWC, const Int16 &dShowWC)
{
	bool modified = false;
	if (m.WC != WC) { m.WC = WC; modified = true; }
	if (m.dWC != dWC) { m.dWC = dWC; modified = true; }
	if (m.dShowWC != dShowWC) { m.dShowWC = dShowWC; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void BatActor_Stub::SetWC(const UInt16 &WC)
{
	bool modified = false;
	if (m.WC != WC) { m.WC = WC; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void BatActor_Stub::SetDWC(const Int16 &dWC)
{
	bool modified = false;
	if (m.dWC != dWC) { m.dWC = dWC; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void BatActor_Stub::SetDShowWC(const Int16 &dShowWC)
{
	bool modified = false;
	if (m.dShowWC != dShowWC) { m.dShowWC = dShowWC; modified = true; }
	MarkUpdateAll(WCMask, modified);
}

void BatActor_Stub::SetWcExp(const UInt16 &WcExp)
{
	bool modified = false;
	if (m.WcExp != WcExp) { m.WcExp = WcExp; modified = true; }
	MarkUpdateAll(WcExpMask, modified);
}

void BatActor_Stub::SetMaxWC(const UInt16 &maxWC)
{
	bool modified = false;
	if (m.maxWC != maxWC) { m.maxWC = maxWC; modified = true; }
	MarkUpdateAll(maxWCMask, modified);
}

void BatActor_Stub::SetVP(const UInt16 &VP, const Int16 &dVP, const Int16 &dShowVP)
{
	bool modified = false;
	if (m.VP != VP) { m.VP = VP; modified = true; }
	if (m.dVP != dVP) { m.dVP = dVP; modified = true; }
	if (m.dShowVP != dShowVP) { m.dShowVP = dShowVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void BatActor_Stub::SetVP(const UInt16 &VP)
{
	bool modified = false;
	if (m.VP != VP) { m.VP = VP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void BatActor_Stub::SetDVP(const Int16 &dVP)
{
	bool modified = false;
	if (m.dVP != dVP) { m.dVP = dVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void BatActor_Stub::SetDShowVP(const Int16 &dShowVP)
{
	bool modified = false;
	if (m.dShowVP != dShowVP) { m.dShowVP = dShowVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void BatActor_Stub::SetMaxVP(const UInt16 &maxVP)
{
	bool modified = false;
	if (m.maxVP != maxVP) { m.maxVP = maxVP; modified = true; }
	MarkUpdateAll(maxVPMask, modified);
}

void BatActor_Stub::SetState(const UInt8 &state)
{
	bool modified = false;
	if (m.state != state) { m.state = state; modified = true; }
	MarkUpdateAll(stateMask, modified);
}

void BatActor_Stub::SetBuff(const String &buff)
{
	bool modified = false;
	if (m.buff != buff) { m.buff = buff; modified = true; }
	MarkUpdateAll(buffMask, modified);
}

void BatActor_Stub::SetBuffCache(const String &buffCache)
{
	bool modified = false;
	if (m.buffCache != buffCache) { m.buffCache = buffCache; modified = true; }
	MarkUpdateAll(buffCacheMask, modified);
}

void BatActor_Stub::SetTeam(const UInt8 &team)
{
	bool modified = false;
	if (m.team != team) { m.team = team; modified = true; }
	MarkUpdateAll(teamMask, modified);
}

void BatActor_Stub::SetPosition(const UInt8 &position)
{
	bool modified = false;
	if (m.position != position) { m.position = position; modified = true; }
	MarkUpdateAll(positionMask, modified);
}

void BatActor_Stub::SetAction(const UInt8 &action)
{
	bool modified = false;
	if (m.action != action) { m.action = action; modified = true; }
	MarkUpdateAll(actionMask, modified);
}

void BatActor_Stub::SetReaction(const UInt8 &reaction)
{
	bool modified = false;
	if (m.reaction != reaction) { m.reaction = reaction; modified = true; }
	MarkUpdateAll(reactionMask, modified);
}

void BatActor_Stub::SetTarget(const UInt8 &target)
{
	bool modified = false;
	if (m.target != target) { m.target = target; modified = true; }
	MarkUpdateAll(targetMask, modified);
}

void BatActor_Stub::SetSubType(const UInt32 &subType)
{
	bool modified = false;
	if (m.subType != subType) { m.subType = subType; modified = true; }
	MarkUpdateAll(subTypeMask, modified);
}

void BatActor_Stub::SetAdditional(const UInt32 &additional)
{
	bool modified = false;
	if (m.additional != additional) { m.additional = additional; modified = true; }
	MarkUpdateAll(additionalMask, modified);
}

void BatActor_Stub::SetLinkNext(const Boolean &linkNext)
{
	bool modified = false;
	if (m.linkNext != linkNext) { m.linkNext = linkNext; modified = true; }
	MarkUpdateAll(linkNextMask, modified);
}

void BatActor_Stub::SetLeftBattle(const Boolean &leftBattle)
{
	bool modified = false;
	if (m.leftBattle != leftBattle) { m.leftBattle = leftBattle; modified = true; }
	MarkUpdateAll(leftBattleMask, modified);
}

void BatActor_Stub::SetGoodness(const Int32 &goodness)
{
	bool modified = false;
	if (m.goodness != goodness) { m.goodness = goodness; modified = true; }
	MarkUpdateAll(goodnessMask, modified);
}

void BatActor_Stub::SetPet(const UInt32 &pet_uid, const UInt32 &pet_id, const String &petName, const UInt16 &petColorIndex)
{
	bool modified = false;
	if (m.pet_uid != pet_uid) { m.pet_uid = pet_uid; modified = true; }
	if (m.pet_id != pet_id) { m.pet_id = pet_id; modified = true; }
	if (m.petName != petName) { m.petName = petName; modified = true; }
	if (m.petColorIndex != petColorIndex) { m.petColorIndex = petColorIndex; modified = true; }
	MarkUpdateAll(petMask, modified);
}

void BatActor_Stub::SetPet_uid(const UInt32 &pet_uid)
{
	bool modified = false;
	if (m.pet_uid != pet_uid) { m.pet_uid = pet_uid; modified = true; }
	MarkUpdateAll(petMask, modified);
}

void BatActor_Stub::SetPet_id(const UInt32 &pet_id)
{
	bool modified = false;
	if (m.pet_id != pet_id) { m.pet_id = pet_id; modified = true; }
	MarkUpdateAll(petMask, modified);
}

void BatActor_Stub::SetPetName(const String &petName)
{
	bool modified = false;
	if (m.petName != petName) { m.petName = petName; modified = true; }
	MarkUpdateAll(petMask, modified);
}

void BatActor_Stub::SetPetColorIndex(const UInt16 &petColorIndex)
{
	bool modified = false;
	if (m.petColorIndex != petColorIndex) { m.petColorIndex = petColorIndex; modified = true; }
	MarkUpdateAll(petMask, modified);
}

void BatActor_Stub::SetPet_active(const bool &pet_active)
{
	bool modified = false;
	if (m.pet_active != pet_active) { m.pet_active = pet_active; modified = true; }
	MarkUpdateAll(pet_activeMask, modified);
}

void BatActor_Stub::SetTP(const UInt16 &TP)
{
	bool modified = false;
	if (m.TP != TP) { m.TP = TP; modified = true; }
	MarkUpdateAll(TPMask, modified);
}

void BatActor_Stub::SetHitTogether(const Boolean &hitTogether)
{
	bool modified = false;
	if (m.hitTogether != hitTogether) { m.hitTogether = hitTogether; modified = true; }
	MarkUpdateAll(hitTogetherMask, modified);
}



void BatActor_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void BatActor_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(info2Mask))
	{
		buf.Write<UInt8>(m.cclass);
		buf.Write<UInt8>(m.sexClass);
		buf.Write<UInt8>(m.faction);
	}
	if (mask.IsBit(indexMask))
	{
		buf.Write<UInt8>(m.index);
	}
	if (mask.IsBit(levelMask))
	{
		buf.Write<UInt16>(m.level);
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
	if (mask.IsBit(HPMask))
	{
		buf.Write<UInt32>(m.HP);
		buf.Write<Int32>(m.dHP);
		buf.Write<Int32>(m.dShowHP);
	}
	if (mask.IsBit(mHPMask))
	{
		buf.Write<UInt32>(m.mHP);
		buf.Write<Int32>(m.dMHP);
		buf.Write<Int32>(m.dShowMHP);
	}
	if (mask.IsBit(maxHPMask))
	{
		buf.Write<UInt32>(m.maxHP);
	}
	if (mask.IsBit(SPMask))
	{
		buf.Write<UInt32>(m.SP);
		buf.Write<Int32>(m.dSP);
		buf.Write<Int32>(m.dShowSP);
	}
	if (mask.IsBit(mSPMask))
	{
		buf.Write<UInt32>(m.mSP);
		buf.Write<Int32>(m.dMSP);
		buf.Write<Int32>(m.dShowMSP);
	}
	if (mask.IsBit(maxSPMask))
	{
		buf.Write<UInt32>(m.maxSP);
	}
	if (mask.IsBit(DPMask))
	{
		buf.Write<UInt16>(m.DP);
		buf.Write<Int16>(m.dDP);
		buf.Write<Int16>(m.dShowDP);
	}
	if (mask.IsBit(mDPMask))
	{
		buf.Write<UInt16>(m.mDP);
	}
	if (mask.IsBit(maxDPMask))
	{
		buf.Write<UInt16>(m.maxDP);
	}
	if (mask.IsBit(WCMask))
	{
		buf.Write<UInt16>(m.WC);
		buf.Write<Int16>(m.dWC);
		buf.Write<Int16>(m.dShowWC);
	}
	if (mask.IsBit(WcExpMask))
	{
		buf.Write<UInt16>(m.WcExp);
	}
	if (mask.IsBit(maxWCMask))
	{
		buf.Write<UInt16>(m.maxWC);
	}
	if (mask.IsBit(VPMask))
	{
		buf.Write<UInt16>(m.VP);
		buf.Write<Int16>(m.dVP);
		buf.Write<Int16>(m.dShowVP);
	}
	if (mask.IsBit(maxVPMask))
	{
		buf.Write<UInt16>(m.maxVP);
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
	if (mask.IsBit(teamMask))
	{
		buf.Write<UInt8>(m.team);
	}
	if (mask.IsBit(positionMask))
	{
		buf.Write<UInt8>(m.position);
	}
	if (mask.IsBit(actionMask))
	{
		buf.Write<UInt8>(m.action);
	}
	if (mask.IsBit(reactionMask))
	{
		buf.Write<UInt8>(m.reaction);
	}
	if (mask.IsBit(targetMask))
	{
		buf.Write<UInt8>(m.target);
	}
	if (mask.IsBit(subTypeMask))
	{
		buf.Write<UInt32>(m.subType);
	}
	if (mask.IsBit(additionalMask))
	{
		buf.Write<UInt32>(m.additional);
	}
	if (mask.IsBit(linkNextMask))
	{
		buf.Write<Boolean>(m.linkNext);
	}
	if (mask.IsBit(leftBattleMask))
	{
		buf.Write<Boolean>(m.leftBattle);
	}
	if (mask.IsBit(goodnessMask))
	{
		buf.Write<Int32>(m.goodness);
	}
	if (mask.IsBit(petMask))
	{
		buf.Write<UInt32>(m.pet_uid);
		buf.Write<UInt32>(m.pet_id);
		buf.Write<String>(m.petName);
		buf.Write<UInt16>(m.petColorIndex);
	}
	if (mask.IsBit(pet_activeMask))
	{
		buf.Write<bool>(m.pet_active);
	}
	if (mask.IsBit(TPMask))
	{
		buf.Write<UInt16>(m.TP);
	}
	if (mask.IsBit(hitTogetherMask))
	{
		buf.Write<Boolean>(m.hitTogether);
	}
}

RPCResult BatActor_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatActor_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult BatActor_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_BatActor_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_BatActor_SendCommand:
		{
			UInt8 cmd;
			pBuf->Read<UInt8>(cmd);
			UInt8 target;
			pBuf->Read<UInt8>(target);
			UInt32 sub_type;
			pBuf->Read<UInt32>(sub_type);
			UInt32 additional;
			pBuf->Read<UInt32>(additional);
			_result = SendCommand(pPeer, context, cmd, target, sub_type, additional);
		}
		break;
	case RPC_BatActor_CheatCode:
		{
			String code;
			pBuf->Read<String>(code);
			_result = CheatCode(pPeer, context, code);
		}
		break;
	case RPC_BatActor_CancelAuto:
		{
			_result = CancelAuto(pPeer, context);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult BatActor_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatActor_CB_Error);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(errCode);
	pBuf->Write<String>(errMsg);
	return RPCSend<1236285136>(pPeer, pBuf);
}

RPCResult BatActor_Stub::CB_CommandSuccess(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatActor_CB_CommandSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(cmd);
	return RPCSend<766788022>(pPeer, pBuf);
}

RPCResult BatActor_Stub::CB_Inputed(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatActor_CB_Inputed);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(cmd);
	return RPCSend<91956427>(pPeer, pBuf);
}

RPCResult BatActor_Stub::CB_AutoCount(LPCPEER pPeer, RPCContext &context, const UInt16 &autoCount)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatActor_CB_AutoCount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(autoCount);
	return RPCSend<573711624>(pPeer, pBuf);
}

RPCResult BatActor_Stub::CB_BattleMsg(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType, const String &Msg, const UInt16 &subType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatActor_CB_BattleMsg);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(MsgType);
	pBuf->Write<String>(Msg);
	pBuf->Write<UInt16>(subType);
	return RPCSend<71531305>(pPeer, pBuf);
}

