//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/BatMob_stub.h"

void BatMob_Stub::SetInfo(const UInt32 &mob_id, const String &raname)
{
	bool modified = false;
	if (m.mob_id != mob_id) { m.mob_id = mob_id; modified = true; }
	if (m.raname != raname) { m.raname = raname; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void BatMob_Stub::SetMob_id(const UInt32 &mob_id)
{
	bool modified = false;
	if (m.mob_id != mob_id) { m.mob_id = mob_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void BatMob_Stub::SetRaname(const String &raname)
{
	bool modified = false;
	if (m.raname != raname) { m.raname = raname; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void BatMob_Stub::SetOwner(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(ownerMask, modified);
}

void BatMob_Stub::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(ownerMask, modified);
}

void BatMob_Stub::SetPartner(const UInt32 &partner_id)
{
	bool modified = false;
	if (m.partner_id != partner_id) { m.partner_id = partner_id; modified = true; }
	MarkUpdateAll(partnerMask, modified);
}

void BatMob_Stub::SetPartner_id(const UInt32 &partner_id)
{
	bool modified = false;
	if (m.partner_id != partner_id) { m.partner_id = partner_id; modified = true; }
	MarkUpdateAll(partnerMask, modified);
}

void BatMob_Stub::SetMobType(const UInt8 &mob_type)
{
	bool modified = false;
	if (m.mob_type != mob_type) { m.mob_type = mob_type; modified = true; }
	MarkUpdateAll(mobTypeMask, modified);
}

void BatMob_Stub::SetMob_type(const UInt8 &mob_type)
{
	bool modified = false;
	if (m.mob_type != mob_type) { m.mob_type = mob_type; modified = true; }
	MarkUpdateAll(mobTypeMask, modified);
}

void BatMob_Stub::SetBoss(const Boolean &boss)
{
	bool modified = false;
	if (m.boss != boss) { m.boss = boss; modified = true; }
	MarkUpdateAll(bossMask, modified);
}

void BatMob_Stub::SetPartnerType(const UInt8 &partner_type)
{
	bool modified = false;
	if (m.partner_type != partner_type) { m.partner_type = partner_type; modified = true; }
	MarkUpdateAll(partnerTypeMask, modified);
}

void BatMob_Stub::SetPartner_type(const UInt8 &partner_type)
{
	bool modified = false;
	if (m.partner_type != partner_type) { m.partner_type = partner_type; modified = true; }
	MarkUpdateAll(partnerTypeMask, modified);
}

void BatMob_Stub::SetNative(const UInt16 &HP_native, const UInt16 &SP_native, const UInt16 &ATK_native, const UInt16 &DEF_native, const UInt16 &SPD_native, const UInt16 &POW_native, const UInt16 &HIT_native, const UInt16 &EVA_native)
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

void BatMob_Stub::SetHP_native(const UInt16 &HP_native)
{
	bool modified = false;
	if (m.HP_native != HP_native) { m.HP_native = HP_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void BatMob_Stub::SetSP_native(const UInt16 &SP_native)
{
	bool modified = false;
	if (m.SP_native != SP_native) { m.SP_native = SP_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void BatMob_Stub::SetATK_native(const UInt16 &ATK_native)
{
	bool modified = false;
	if (m.ATK_native != ATK_native) { m.ATK_native = ATK_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void BatMob_Stub::SetDEF_native(const UInt16 &DEF_native)
{
	bool modified = false;
	if (m.DEF_native != DEF_native) { m.DEF_native = DEF_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void BatMob_Stub::SetSPD_native(const UInt16 &SPD_native)
{
	bool modified = false;
	if (m.SPD_native != SPD_native) { m.SPD_native = SPD_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void BatMob_Stub::SetPOW_native(const UInt16 &POW_native)
{
	bool modified = false;
	if (m.POW_native != POW_native) { m.POW_native = POW_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void BatMob_Stub::SetHIT_native(const UInt16 &HIT_native)
{
	bool modified = false;
	if (m.HIT_native != HIT_native) { m.HIT_native = HIT_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void BatMob_Stub::SetEVA_native(const UInt16 &EVA_native)
{
	bool modified = false;
	if (m.EVA_native != EVA_native) { m.EVA_native = EVA_native; modified = true; }
	MarkUpdateAll(nativeMask, modified);
}

void BatMob_Stub::SetGrow(const float &grow)
{
	bool modified = false;
	if (m.grow != grow) { m.grow = grow; modified = true; }
	MarkUpdateAll(growMask, modified);
}

void BatMob_Stub::SetPartnerColor(const UInt16 &colorIndex)
{
	bool modified = false;
	if (m.colorIndex != colorIndex) { m.colorIndex = colorIndex; modified = true; }
	MarkUpdateAll(partnerColorMask, modified);
}

void BatMob_Stub::SetColorIndex(const UInt16 &colorIndex)
{
	bool modified = false;
	if (m.colorIndex != colorIndex) { m.colorIndex = colorIndex; modified = true; }
	MarkUpdateAll(partnerColorMask, modified);
}



void BatMob_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void BatMob_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(infoMask))
	{
		buf.Write<UInt32>(m.mob_id);
		buf.Write<String>(m.raname);
	}
	if (mask.IsBit(ownerMask))
	{
		buf.Write<UInt32>(m.owner_id);
	}
	if (mask.IsBit(partnerMask))
	{
		buf.Write<UInt32>(m.partner_id);
	}
	if (mask.IsBit(mobTypeMask))
	{
		buf.Write<UInt8>(m.mob_type);
	}
	if (mask.IsBit(bossMask))
	{
		buf.Write<Boolean>(m.boss);
	}
	if (mask.IsBit(partnerTypeMask))
	{
		buf.Write<UInt8>(m.partner_type);
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
	if (mask.IsBit(partnerColorMask))
	{
		buf.Write<UInt16>(m.colorIndex);
	}
}

RPCResult BatMob_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatMob_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult BatMob_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_BatMob_REQ_STATE:
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

