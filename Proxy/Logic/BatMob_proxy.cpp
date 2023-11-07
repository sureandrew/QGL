//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/BatMob_proxy.h"


void BatMob_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<UInt32>(m.mob_id);
		buf.Read<String>(m.raname);
	}
	if (m_updateMask.IsBit(ownerMask))
	{
		buf.Read<UInt32>(m.owner_id);
	}
	if (m_updateMask.IsBit(partnerMask))
	{
		buf.Read<UInt32>(m.partner_id);
	}
	if (m_updateMask.IsBit(mobTypeMask))
	{
		buf.Read<UInt8>(m.mob_type);
	}
	if (m_updateMask.IsBit(bossMask))
	{
		buf.Read<Boolean>(m.boss);
	}
	if (m_updateMask.IsBit(partnerTypeMask))
	{
		buf.Read<UInt8>(m.partner_type);
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
	if (m_updateMask.IsBit(partnerColorMask))
	{
		buf.Read<UInt16>(m.colorIndex);
	}
}

void BatMob_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void BatMob_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(infoMask)) OnUpdateInfo(m.mob_id, m.raname);
	if (IsUpdated(ownerMask)) OnUpdateOwner(m.owner_id);
	if (IsUpdated(partnerMask)) OnUpdatePartner(m.partner_id);
	if (IsUpdated(mobTypeMask)) OnUpdateMobType(m.mob_type);
	if (IsUpdated(bossMask)) OnUpdateBoss(m.boss);
	if (IsUpdated(partnerTypeMask)) OnUpdatePartnerType(m.partner_type);
	if (IsUpdated(nativeMask)) OnUpdateNative(m.HP_native, m.SP_native, m.ATK_native, m.DEF_native, m.SPD_native, m.POW_native, m.HIT_native, m.EVA_native);
	if (IsUpdated(growMask)) OnUpdateGrow(m.grow);
	if (IsUpdated(partnerColorMask)) OnUpdatePartnerColor(m.colorIndex);
}

#pragma optimize("", off)
RPCResult BatMob_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_BatMob_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult BatMob_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_BatMob_UP_STATE:
			_result = RPCDecode<270741010>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
