//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Partner_stub.h"

void Partner_Stub::SetId(const UInt32 &partner_id, const UInt32 &mob_id)
{
	bool modified = false;
	if (m.partner_id != partner_id) { m.partner_id = partner_id; modified = true; }
	if (m.mob_id != mob_id) { m.mob_id = mob_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void Partner_Stub::SetPartner_id(const UInt32 &partner_id)
{
	bool modified = false;
	if (m.partner_id != partner_id) { m.partner_id = partner_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void Partner_Stub::SetMob_id(const UInt32 &mob_id)
{
	bool modified = false;
	if (m.mob_id != mob_id) { m.mob_id = mob_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void Partner_Stub::SetOwner(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(ownerMask, modified);
}

void Partner_Stub::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(ownerMask, modified);
}

void Partner_Stub::SetInfo(const String &raname)
{
	bool modified = false;
	if (m.raname != raname) { m.raname = raname; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Partner_Stub::SetRaname(const String &raname)
{
	bool modified = false;
	if (m.raname != raname) { m.raname = raname; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Partner_Stub::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void Partner_Stub::SetNextLvExp(const UInt32 &nextLvExp)
{
	bool modified = false;
	if (m.nextLvExp != nextLvExp) { m.nextLvExp = nextLvExp; modified = true; }
	MarkUpdateAll(nextLvExpMask, modified);
}

void Partner_Stub::SetExp(const UInt32 &exp)
{
	bool modified = false;
	if (m.exp != exp) { m.exp = exp; modified = true; }
	MarkUpdateAll(expMask, modified);
}

void Partner_Stub::SetAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI)
{
	bool modified = false;
	if (m.STA != STA) { m.STA = STA; modified = true; }
	if (m.SPR != SPR) { m.SPR = SPR; modified = true; }
	if (m.STR != STR) { m.STR = STR; modified = true; }
	if (m.CON != CON) { m.CON = CON; modified = true; }
	if (m.AGI != AGI) { m.AGI = AGI; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void Partner_Stub::SetSTA(const UInt16 &STA)
{
	bool modified = false;
	if (m.STA != STA) { m.STA = STA; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void Partner_Stub::SetSPR(const UInt16 &SPR)
{
	bool modified = false;
	if (m.SPR != SPR) { m.SPR = SPR; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void Partner_Stub::SetSTR(const UInt16 &STR)
{
	bool modified = false;
	if (m.STR != STR) { m.STR = STR; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void Partner_Stub::SetCON(const UInt16 &CON)
{
	bool modified = false;
	if (m.CON != CON) { m.CON = CON; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void Partner_Stub::SetAGI(const UInt16 &AGI)
{
	bool modified = false;
	if (m.AGI != AGI) { m.AGI = AGI; modified = true; }
	MarkUpdateAll(attrib1Mask, modified);
}

void Partner_Stub::SetAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD)
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

void Partner_Stub::SetATK(const UInt16 &ATK)
{
	bool modified = false;
	if (m.ATK != ATK) { m.ATK = ATK; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Partner_Stub::SetDEF(const UInt16 &DEF)
{
	bool modified = false;
	if (m.DEF != DEF) { m.DEF = DEF; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Partner_Stub::SetHIT(const UInt16 &HIT)
{
	bool modified = false;
	if (m.HIT != HIT) { m.HIT = HIT; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Partner_Stub::SetEVA(const UInt16 &EVA)
{
	bool modified = false;
	if (m.EVA != EVA) { m.EVA = EVA; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Partner_Stub::SetPOW(const UInt16 &POW)
{
	bool modified = false;
	if (m.POW != POW) { m.POW = POW; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Partner_Stub::SetSPD(const UInt16 &SPD)
{
	bool modified = false;
	if (m.SPD != SPD) { m.SPD = SPD; modified = true; }
	MarkUpdateAll(attrib2Mask, modified);
}

void Partner_Stub::SetAttrPoint(const UInt16 &attrPoint)
{
	bool modified = false;
	if (m.attrPoint != attrPoint) { m.attrPoint = attrPoint; modified = true; }
	MarkUpdateAll(attrPointMask, modified);
}

void Partner_Stub::SetHP(const UInt32 &HP, const UInt32 &maxHP)
{
	bool modified = false;
	if (m.HP != HP) { m.HP = HP; modified = true; }
	if (m.maxHP != maxHP) { m.maxHP = maxHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void Partner_Stub::SetHP(const UInt32 &HP)
{
	bool modified = false;
	if (m.HP != HP) { m.HP = HP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void Partner_Stub::SetMaxHP(const UInt32 &maxHP)
{
	bool modified = false;
	if (m.maxHP != maxHP) { m.maxHP = maxHP; modified = true; }
	MarkUpdateAll(HPMask, modified);
}

void Partner_Stub::SetSP(const UInt32 &SP, const UInt32 &maxSP)
{
	bool modified = false;
	if (m.SP != SP) { m.SP = SP; modified = true; }
	if (m.maxSP != maxSP) { m.maxSP = maxSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void Partner_Stub::SetSP(const UInt32 &SP)
{
	bool modified = false;
	if (m.SP != SP) { m.SP = SP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void Partner_Stub::SetMaxSP(const UInt32 &maxSP)
{
	bool modified = false;
	if (m.maxSP != maxSP) { m.maxSP = maxSP; modified = true; }
	MarkUpdateAll(SPMask, modified);
}

void Partner_Stub::SetNative(const UInt16 &HP_native, const UInt16 &SP_native, const UInt16 &ATK_native, const UInt16 &DEF_native, const UInt16 &SPD_native, const UInt16 &POW_native, const UInt16 &HIT_native, const UInt16 &EVA_native)
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

void Partner_Stub::SetHP_native(const UInt16 &HP_native)
{
	bool modified = false;
	if (m.HP_native != HP_native) { m.HP_native = HP_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void Partner_Stub::SetSP_native(const UInt16 &SP_native)
{
	bool modified = false;
	if (m.SP_native != SP_native) { m.SP_native = SP_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void Partner_Stub::SetATK_native(const UInt16 &ATK_native)
{
	bool modified = false;
	if (m.ATK_native != ATK_native) { m.ATK_native = ATK_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void Partner_Stub::SetDEF_native(const UInt16 &DEF_native)
{
	bool modified = false;
	if (m.DEF_native != DEF_native) { m.DEF_native = DEF_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void Partner_Stub::SetSPD_native(const UInt16 &SPD_native)
{
	bool modified = false;
	if (m.SPD_native != SPD_native) { m.SPD_native = SPD_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void Partner_Stub::SetPOW_native(const UInt16 &POW_native)
{
	bool modified = false;
	if (m.POW_native != POW_native) { m.POW_native = POW_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void Partner_Stub::SetHIT_native(const UInt16 &HIT_native)
{
	bool modified = false;
	if (m.HIT_native != HIT_native) { m.HIT_native = HIT_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void Partner_Stub::SetEVA_native(const UInt16 &EVA_native)
{
	bool modified = false;
	if (m.EVA_native != EVA_native) { m.EVA_native = EVA_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void Partner_Stub::SetGrow(const float &grow)
{
	bool modified = false;
	if (m.grow != grow) { m.grow = grow; modified = true; }
	MarkUpdateAll(growMask, modified);
}

void Partner_Stub::SetFavor(const float &favor)
{
	bool modified = false;
	if (m.favor != favor) { m.favor = favor; modified = true; }
	MarkUpdateAll(favorMask, modified);
}

void Partner_Stub::SetHealth(const UInt16 &health)
{
	bool modified = false;
	if (m.health != health) { m.health = health; modified = true; }
	MarkUpdateAll(healthMask, modified);
}

void Partner_Stub::SetGoodness(const Int32 &goodness)
{
	bool modified = false;
	if (m.goodness != goodness) { m.goodness = goodness; modified = true; }
	MarkUpdateAll(goodnessMask, modified);
}

void Partner_Stub::SetState(const UInt8 &state)
{
	bool modified = false;
	if (m.state != state) { m.state = state; modified = true; }
	MarkUpdateAll(stateMask, modified);
}

void Partner_Stub::SetBuff(const String &buff)
{
	bool modified = false;
	if (m.buff != buff) { m.buff = buff; modified = true; }
	MarkUpdateAll(buffMask, modified);
}

void Partner_Stub::SetSoulboundState(const UInt8 &soulboundState)
{
	bool modified = false;
	if (m.soulboundState != soulboundState) { m.soulboundState = soulboundState; modified = true; }
	MarkUpdateAll(soulboundStateMask, modified);
}

void Partner_Stub::SetExpireDate(const UInt32 &expireDate)
{
	bool modified = false;
	if (m.expireDate != expireDate) { m.expireDate = expireDate; modified = true; }
	MarkUpdateAll(ExpireDateMask, modified);
}

void Partner_Stub::SetElement(const UInt8 &element)
{
	bool modified = false;
	if (m.element != element) { m.element = element; modified = true; }
	MarkUpdateAll(ElementMask, modified);
}

void Partner_Stub::SetOutLookColor(const UInt16 &outLookColor)
{
	bool modified = false;
	if (m.outLookColor != outLookColor) { m.outLookColor = outLookColor; modified = true; }
	MarkUpdateAll(OutLookColorMask, modified);
}

void Partner_Stub::SetBuffCache(const String &buffCache)
{
	bool modified = false;
	if (m.buffCache != buffCache) { m.buffCache = buffCache; modified = true; }
	MarkUpdateAll(buffCacheMask, modified);
}

void Partner_Stub::SetInWareHouse(const UInt8 &inWareHouse)
{
	bool modified = false;
	if (m.inWareHouse != inWareHouse) { m.inWareHouse = inWareHouse; modified = true; }
	MarkUpdateAll(inWareHouseMask, modified);
}

void Partner_Stub::SetAssetLock(const Boolean &lock)
{
	bool modified = false;
	if (m.lock != lock) { m.lock = lock; modified = true; }
	MarkUpdateAll(assetLockMask, modified);
}

void Partner_Stub::SetLock(const Boolean &lock)
{
	bool modified = false;
	if (m.lock != lock) { m.lock = lock; modified = true; }
	MarkUpdateAll(assetLockMask, modified);
}

void Partner_Stub::SetCultivation(const UInt16 &cultivation)
{
	bool modified = false;
	if (m.cultivation != cultivation) { m.cultivation = cultivation; modified = true; }
	MarkUpdateAll(CultivationMask, modified);
}

void Partner_Stub::SetPractice(const UInt16 &practice)
{
	bool modified = false;
	if (m.practice != practice) { m.practice = practice; modified = true; }
	MarkUpdateAll(PracticeMask, modified);
}

void Partner_Stub::SetGrowLevel(const UInt8 &growLevel)
{
	bool modified = false;
	if (m.growLevel != growLevel) { m.growLevel = growLevel; modified = true; }
	MarkUpdateAll(growLevelMask, modified);
}



void Partner_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Partner_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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
	if (mask.IsBit(nextLvExpMask))
	{
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
		buf.Write<UInt32>(m.HP);
		buf.Write<UInt32>(m.maxHP);
	}
	if (mask.IsBit(SPMask))
	{
		buf.Write<UInt32>(m.SP);
		buf.Write<UInt32>(m.maxSP);
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
	if (mask.IsBit(favorMask))
	{
		buf.Write<float>(m.favor);
	}
	if (mask.IsBit(healthMask))
	{
		buf.Write<UInt16>(m.health);
	}
	if (mask.IsBit(goodnessMask))
	{
		buf.Write<Int32>(m.goodness);
	}
	if (mask.IsBit(stateMask))
	{
		buf.Write<UInt8>(m.state);
	}
	if (mask.IsBit(buffMask))
	{
		buf.Write<String>(m.buff);
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
	if (mask.IsBit(buffCacheMask))
	{
		buf.Write<String>(m.buffCache);
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
	if (mask.IsBit(growLevelMask))
	{
		buf.Write<UInt8>(m.growLevel);
	}
}

RPCResult Partner_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Partner_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Partner_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Partner_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_Partner_RaiseAttrib:
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
	case RPC_Partner_ChangePartnerName:
		{
			String newName;
			pBuf->Read<String>(newName);
			_result = ChangePartnerName(pPeer, context, newName);
		}
		break;
	case RPC_Partner_ChangePartnerState:
		{
			UInt8 state;
			pBuf->Read<UInt8>(state);
			_result = ChangePartnerState(pPeer, context, state);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult Partner_Stub::CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Partner_CB_RaiseAttrib);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<318600191>(pPeer, pBuf);
}

RPCResult Partner_Stub::CB_ChangePartnerName(LPCPEER pPeer, RPCContext &context, const bool &success)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Partner_CB_ChangePartnerName);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<bool>(success);
	return RPCSend<469255547>(pPeer, pBuf);
}

RPCResult Partner_Stub::CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Partner_CB_ChangePartnerState);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1103260278>(pPeer, pBuf);
}

RPCResult Partner_Stub::CB_UpdateSuccess(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Partner_CB_UpdateSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<371536329>(pPeer, pBuf);
}

RPCResult Partner_Stub::CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Partner_CB_LearnSkillMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(skillID);
	return RPCSend<1173311946>(pPeer, pBuf);
}

RPCResult Partner_Stub::CB_FalseJoinBattle(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Partner_CB_FalseJoinBattle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(MsgType);
	return RPCSend<649489524>(pPeer, pBuf);
}

