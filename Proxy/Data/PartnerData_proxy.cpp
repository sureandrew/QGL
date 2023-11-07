//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/PartnerData_proxy.h"

void PartnerData_Proxy::SetId(const UInt32 &partner_id, const UInt32 &mob_id)
{
	bool modified = false;
	if (m.partner_id != partner_id) { m.partner_id = partner_id; modified = true; }
	if (m.mob_id != mob_id) { m.mob_id = mob_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void PartnerData_Proxy::SetPartner_id(const UInt32 &partner_id)
{
	bool modified = false;
	if (m.partner_id != partner_id) { m.partner_id = partner_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void PartnerData_Proxy::SetMob_id(const UInt32 &mob_id)
{
	bool modified = false;
	if (m.mob_id != mob_id) { m.mob_id = mob_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void PartnerData_Proxy::SetOwner(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(ownerMask, modified);
}

void PartnerData_Proxy::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(ownerMask, modified);
}

void PartnerData_Proxy::SetInfo(const String &raname)
{
	bool modified = false;
	if (m.raname != raname) { m.raname = raname; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void PartnerData_Proxy::SetRaname(const String &raname)
{
	bool modified = false;
	if (m.raname != raname) { m.raname = raname; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void PartnerData_Proxy::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void PartnerData_Proxy::SetExp(const UInt32 &exp)
{
	bool modified = false;
	if (m.exp != exp) { m.exp = exp; modified = true; }
	MarkUpdateAll(expMask, modified);
}

void PartnerData_Proxy::SetAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI)
{
	bool modified = false;
	if (m.STA != STA) { m.STA = STA; modified = true; }
	if (m.SPR != SPR) { m.SPR = SPR; modified = true; }
	if (m.STR != STR) { m.STR = STR; modified = true; }
	if (m.CON != CON) { m.CON = CON; modified = true; }
	if (m.AGI != AGI) { m.AGI = AGI; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void PartnerData_Proxy::SetSTA(const UInt16 &STA)
{
	bool modified = false;
	if (m.STA != STA) { m.STA = STA; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void PartnerData_Proxy::SetSPR(const UInt16 &SPR)
{
	bool modified = false;
	if (m.SPR != SPR) { m.SPR = SPR; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void PartnerData_Proxy::SetSTR(const UInt16 &STR)
{
	bool modified = false;
	if (m.STR != STR) { m.STR = STR; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void PartnerData_Proxy::SetCON(const UInt16 &CON)
{
	bool modified = false;
	if (m.CON != CON) { m.CON = CON; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void PartnerData_Proxy::SetAGI(const UInt16 &AGI)
{
	bool modified = false;
	if (m.AGI != AGI) { m.AGI = AGI; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void PartnerData_Proxy::SetAttrPoint(const UInt16 &attrPoint)
{
	bool modified = false;
	if (m.attrPoint != attrPoint) { m.attrPoint = attrPoint; modified = true; }
	MarkUpdateAll(attrPointMask, modified);
}

void PartnerData_Proxy::SetNative(const UInt16 &HP_native, const UInt16 &SP_native, const UInt16 &ATK_native, const UInt16 &DEF_native, const UInt16 &SPD_native, const UInt16 &POW_native, const UInt16 &HIT_native, const UInt16 &EVA_native)
{
	bool modified = false;
	if (m.HP_native != HP_native) { m.HP_native = HP_native; modified = true; }
	if (m.SP_native != SP_native) { m.SP_native = SP_native; modified = true; }
	if (m.ATK_native != ATK_native) { m.ATK_native = ATK_native; modified = true; }
	if (m.DEF_native != DEF_native) { m.DEF_native = DEF_native; modified = true; }
	if (m.SPD_native != SPD_native) { m.SPD_native = SPD_native; modified = true; }
	if (m.POW_native != POW_native) { m.POW_native = POW_native; modified = true; }
	if (m.HIT_native != HIT_native) { m.HIT_native = HIT_native; modified = true; }
	if (m.EVA_native != EVA_native) { m.EVA_native = EVA_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void PartnerData_Proxy::SetHP_native(const UInt16 &HP_native)
{
	bool modified = false;
	if (m.HP_native != HP_native) { m.HP_native = HP_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void PartnerData_Proxy::SetSP_native(const UInt16 &SP_native)
{
	bool modified = false;
	if (m.SP_native != SP_native) { m.SP_native = SP_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void PartnerData_Proxy::SetATK_native(const UInt16 &ATK_native)
{
	bool modified = false;
	if (m.ATK_native != ATK_native) { m.ATK_native = ATK_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void PartnerData_Proxy::SetDEF_native(const UInt16 &DEF_native)
{
	bool modified = false;
	if (m.DEF_native != DEF_native) { m.DEF_native = DEF_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void PartnerData_Proxy::SetSPD_native(const UInt16 &SPD_native)
{
	bool modified = false;
	if (m.SPD_native != SPD_native) { m.SPD_native = SPD_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void PartnerData_Proxy::SetPOW_native(const UInt16 &POW_native)
{
	bool modified = false;
	if (m.POW_native != POW_native) { m.POW_native = POW_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void PartnerData_Proxy::SetHIT_native(const UInt16 &HIT_native)
{
	bool modified = false;
	if (m.HIT_native != HIT_native) { m.HIT_native = HIT_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void PartnerData_Proxy::SetEVA_native(const UInt16 &EVA_native)
{
	bool modified = false;
	if (m.EVA_native != EVA_native) { m.EVA_native = EVA_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void PartnerData_Proxy::SetGrow(const float &grow)
{
	bool modified = false;
	if (m.grow != grow) { m.grow = grow; modified = true; }
	MarkUpdateAll(growMask, modified);
}

void PartnerData_Proxy::SetHP(const UInt16 &HP, const UInt16 &maxHP)
{
	bool modified = false;
	if (m.HP != HP) { m.HP = HP; modified = true; }
	if (m.maxHP != maxHP) { m.maxHP = maxHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void PartnerData_Proxy::SetHP(const UInt16 &HP)
{
	bool modified = false;
	if (m.HP != HP) { m.HP = HP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void PartnerData_Proxy::SetMaxHP(const UInt16 &maxHP)
{
	bool modified = false;
	if (m.maxHP != maxHP) { m.maxHP = maxHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void PartnerData_Proxy::SetSP(const UInt16 &SP, const UInt16 &maxSP)
{
	bool modified = false;
	if (m.SP != SP) { m.SP = SP; modified = true; }
	if (m.maxSP != maxSP) { m.maxSP = maxSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void PartnerData_Proxy::SetSP(const UInt16 &SP)
{
	bool modified = false;
	if (m.SP != SP) { m.SP = SP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void PartnerData_Proxy::SetMaxSP(const UInt16 &maxSP)
{
	bool modified = false;
	if (m.maxSP != maxSP) { m.maxSP = maxSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void PartnerData_Proxy::SetFavor(const float &favor)
{
	bool modified = false;
	if (m.favor != favor) { m.favor = favor; modified = true; }
	MarkUpdateAll(favorMask, modified);
}

void PartnerData_Proxy::SetState(const UInt8 &state)
{
	bool modified = false;
	if (m.state != state) { m.state = state; modified = true; }
	MarkUpdateAll(stateMask, modified);
}

void PartnerData_Proxy::SetBuff(const String &buff)
{
	bool modified = false;
	if (m.buff != buff) { m.buff = buff; modified = true; }
	MarkUpdateAll(buffMask, modified);
}

void PartnerData_Proxy::SetHealth(const UInt16 &health)
{
	bool modified = false;
	if (m.health != health) { m.health = health; modified = true; }
	MarkUpdateAll(healthMask, modified);
}

void PartnerData_Proxy::SetGoodness(const Int32 &goodness)
{
	bool modified = false;
	if (m.goodness != goodness) { m.goodness = goodness; modified = true; }
	MarkUpdateAll(goodnessMask, modified);
}

void PartnerData_Proxy::SetSoulboundState(const UInt8 &soulboundState)
{
	bool modified = false;
	if (m.soulboundState != soulboundState) { m.soulboundState = soulboundState; modified = true; }
	MarkUpdateAll(soulboundStateMask, modified);
}

void PartnerData_Proxy::SetExpireDate(const UInt32 &expireDate)
{
	bool modified = false;
	if (m.expireDate != expireDate) { m.expireDate = expireDate; modified = true; }
	MarkUpdateAll(ExpireDateMask, modified);
}

void PartnerData_Proxy::SetElement(const UInt8 &element)
{
	bool modified = false;
	if (m.element != element) { m.element = element; modified = true; }
	MarkUpdateAll(ElementMask, modified);
}

void PartnerData_Proxy::SetOutLookColor(const UInt16 &outLookColor)
{
	bool modified = false;
	if (m.outLookColor != outLookColor) { m.outLookColor = outLookColor; modified = true; }
	MarkUpdateAll(OutLookColorMask, modified);
}

void PartnerData_Proxy::SetInWareHouse(const UInt8 &inWareHouse)
{
	bool modified = false;
	if (m.inWareHouse != inWareHouse) { m.inWareHouse = inWareHouse; modified = true; }
	MarkUpdateAll(inWareHouseMask, modified);
}

void PartnerData_Proxy::SetAssetLock(const Boolean &lock)
{
	bool modified = false;
	if (m.lock != lock) { m.lock = lock; modified = true; }
	MarkUpdateAll(assetLockMask, modified);
}

void PartnerData_Proxy::SetLock(const Boolean &lock)
{
	bool modified = false;
	if (m.lock != lock) { m.lock = lock; modified = true; }
	MarkUpdateAll(assetLockMask, modified);
}

void PartnerData_Proxy::SetCultivation(const UInt16 &cultivation)
{
	bool modified = false;
	if (m.cultivation != cultivation) { m.cultivation = cultivation; modified = true; }
	MarkUpdateAll(CultivationMask, modified);
}

void PartnerData_Proxy::SetPractice(const UInt16 &practice)
{
	bool modified = false;
	if (m.practice != practice) { m.practice = practice; modified = true; }
	MarkUpdateAll(PracticeMask, modified);
}


void PartnerData_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.partner_id);
		buf.Read<UInt32>(m.mob_id);
	}
	if (m_updateMask.IsBit(ownerMask))
	{
		buf.Read<UInt32>(m.owner_id);
	}
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<String>(m.raname);
	}
	if (m_updateMask.IsBit(levelMask))
	{
		buf.Read<UInt16>(m.level);
	}
	if (m_updateMask.IsBit(expMask))
	{
		buf.Read<UInt32>(m.exp);
	}
	if (m_updateMask.IsBit(attrib1Mask))
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
	if (m_updateMask.IsBit(nativeMask))
	{
		buf.Read<UInt16>(m.HP_native);
		buf.Read<UInt16>(m.SP_native);
		buf.Read<UInt16>(m.ATK_native);
		buf.Read<UInt16>(m.DEF_native);
		buf.Read<UInt16>(m.SPD_native);
		buf.Read<UInt16>(m.POW_native);
		buf.Read<UInt16>(m.HIT_native);
		buf.Read<UInt16>(m.EVA_native);
	}
	if (m_updateMask.IsBit(growMask))
	{
		buf.Read<float>(m.grow);
	}
	if (m_updateMask.IsBit(HPMask))
	{
		buf.Read<UInt16>(m.HP);
		buf.Read<UInt16>(m.maxHP);
	}
	if (m_updateMask.IsBit(SPMask))
	{
		buf.Read<UInt16>(m.SP);
		buf.Read<UInt16>(m.maxSP);
	}
	if (m_updateMask.IsBit(favorMask))
	{
		buf.Read<float>(m.favor);
	}
	if (m_updateMask.IsBit(stateMask))
	{
		buf.Read<UInt8>(m.state);
	}
	if (m_updateMask.IsBit(buffMask))
	{
		buf.Read<String>(m.buff);
	}
	if (m_updateMask.IsBit(healthMask))
	{
		buf.Read<UInt16>(m.health);
	}
	if (m_updateMask.IsBit(goodnessMask))
	{
		buf.Read<Int32>(m.goodness);
	}
	if (m_updateMask.IsBit(soulboundStateMask))
	{
		buf.Read<UInt8>(m.soulboundState);
	}
	if (m_updateMask.IsBit(ExpireDateMask))
	{
		buf.Read<UInt32>(m.expireDate);
	}
	if (m_updateMask.IsBit(ElementMask))
	{
		buf.Read<UInt8>(m.element);
	}
	if (m_updateMask.IsBit(OutLookColorMask))
	{
		buf.Read<UInt16>(m.outLookColor);
	}
	if (m_updateMask.IsBit(inWareHouseMask))
	{
		buf.Read<UInt8>(m.inWareHouse);
	}
	if (m_updateMask.IsBit(assetLockMask))
	{
		buf.Read<Boolean>(m.lock);
	}
	if (m_updateMask.IsBit(CultivationMask))
	{
		buf.Read<UInt16>(m.cultivation);
	}
	if (m_updateMask.IsBit(PracticeMask))
	{
		buf.Read<UInt16>(m.practice);
	}
}

void PartnerData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.partner_id);
		buf.Write<UInt32>(m.mob_id);
	}
	if (mask.IsBit(ownerMask))
	{
		buf.Write<UInt32>(m.owner_id);
	}
	if (mask.IsBit(infoMask))
	{
		buf.Write<String>(m.raname);
	}
	if (mask.IsBit(levelMask))
	{
		buf.Write<UInt16>(m.level);
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
	if (mask.IsBit(attrPointMask))
	{
		buf.Write<UInt16>(m.attrPoint);
	}
	if (mask.IsBit(nativeMask))
	{
		buf.Write<UInt16>(m.HP_native);
		buf.Write<UInt16>(m.SP_native);
		buf.Write<UInt16>(m.ATK_native);
		buf.Write<UInt16>(m.DEF_native);
		buf.Write<UInt16>(m.SPD_native);
		buf.Write<UInt16>(m.POW_native);
		buf.Write<UInt16>(m.HIT_native);
		buf.Write<UInt16>(m.EVA_native);
	}
	if (mask.IsBit(growMask))
	{
		buf.Write<float>(m.grow);
	}
	if (mask.IsBit(HPMask))
	{
		buf.Write<UInt16>(m.HP);
		buf.Write<UInt16>(m.maxHP);
	}
	if (mask.IsBit(SPMask))
	{
		buf.Write<UInt16>(m.SP);
		buf.Write<UInt16>(m.maxSP);
	}
	if (mask.IsBit(favorMask))
	{
		buf.Write<float>(m.favor);
	}
	if (mask.IsBit(stateMask))
	{
		buf.Write<UInt8>(m.state);
	}
	if (mask.IsBit(buffMask))
	{
		buf.Write<String>(m.buff);
	}
	if (mask.IsBit(healthMask))
	{
		buf.Write<UInt16>(m.health);
	}
	if (mask.IsBit(goodnessMask))
	{
		buf.Write<Int32>(m.goodness);
	}
	if (mask.IsBit(soulboundStateMask))
	{
		buf.Write<UInt8>(m.soulboundState);
	}
	if (mask.IsBit(ExpireDateMask))
	{
		buf.Write<UInt32>(m.expireDate);
	}
	if (mask.IsBit(ElementMask))
	{
		buf.Write<UInt8>(m.element);
	}
	if (mask.IsBit(OutLookColorMask))
	{
		buf.Write<UInt16>(m.outLookColor);
	}
	if (mask.IsBit(inWareHouseMask))
	{
		buf.Write<UInt8>(m.inWareHouse);
	}
	if (mask.IsBit(assetLockMask))
	{
		buf.Write<Boolean>(m.lock);
	}
	if (mask.IsBit(CultivationMask))
	{
		buf.Write<UInt16>(m.cultivation);
	}
	if (mask.IsBit(PracticeMask))
	{
		buf.Write<UInt16>(m.practice);
	}
}

void PartnerData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.partner_id, m.mob_id);
	if (IsUpdated(ownerMask)) OnUpdateOwner(m.owner_id);
	if (IsUpdated(infoMask)) OnUpdateInfo(m.raname);
	if (IsUpdated(levelMask)) OnUpdateLevel(m.level);
	if (IsUpdated(expMask)) OnUpdateExp(m.exp);
	if (IsUpdated(attrib1Mask)) OnUpdateAttrib1(m.STA, m.SPR, m.STR, m.CON, m.AGI);
	if (IsUpdated(attrPointMask)) OnUpdateAttrPoint(m.attrPoint);
	if (IsUpdated(nativeMask)) OnUpdateNative(m.HP_native, m.SP_native, m.ATK_native, m.DEF_native, m.SPD_native, m.POW_native, m.HIT_native, m.EVA_native);
	if (IsUpdated(growMask)) OnUpdateGrow(m.grow);
	if (IsUpdated(HPMask)) OnUpdateHP(m.HP, m.maxHP);
	if (IsUpdated(SPMask)) OnUpdateSP(m.SP, m.maxSP);
	if (IsUpdated(favorMask)) OnUpdateFavor(m.favor);
	if (IsUpdated(stateMask)) OnUpdateState(m.state);
	if (IsUpdated(buffMask)) OnUpdateBuff(m.buff);
	if (IsUpdated(healthMask)) OnUpdateHealth(m.health);
	if (IsUpdated(goodnessMask)) OnUpdateGoodness(m.goodness);
	if (IsUpdated(soulboundStateMask)) OnUpdateSoulboundState(m.soulboundState);
	if (IsUpdated(ExpireDateMask)) OnUpdateExpireDate(m.expireDate);
	if (IsUpdated(ElementMask)) OnUpdateElement(m.element);
	if (IsUpdated(OutLookColorMask)) OnUpdateOutLookColor(m.outLookColor);
	if (IsUpdated(inWareHouseMask)) OnUpdateInWareHouse(m.inWareHouse);
	if (IsUpdated(assetLockMask)) OnUpdateAssetLock(m.lock);
	if (IsUpdated(CultivationMask)) OnUpdateCultivation(m.cultivation);
	if (IsUpdated(PracticeMask)) OnUpdatePractice(m.practice);
}

#pragma optimize("", off)
RPCResult PartnerData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_PartnerData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult PartnerData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_PartnerData_UP_STATE:
			_result = RPCDecode<1536901753>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
