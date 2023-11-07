//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Partner_proxy.h"


void Partner_Proxy::ReadAttr(CONST Buffer &buf)
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
	if (m_updateMask.IsBit(nextLvExpMask))
	{
		buf.Read<UInt32>(m.nextLvExp);
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
	if (m_updateMask.IsBit(attrib2Mask))
	{
		buf.Read<UInt16>(m.ATK);
		buf.Read<UInt16>(m.DEF);
		buf.Read<UInt16>(m.HIT);
		buf.Read<UInt16>(m.EVA);
		buf.Read<UInt16>(m.POW);
		buf.Read<UInt16>(m.SPD);
	}
	if (m_updateMask.IsBit(attrPointMask))
	{
		buf.Read<UInt16>(m.attrPoint);
	}
	if (m_updateMask.IsBit(HPMask))
	{
		buf.Read<UInt32>(m.HP);
		buf.Read<UInt32>(m.maxHP);
	}
	if (m_updateMask.IsBit(SPMask))
	{
		buf.Read<UInt32>(m.SP);
		buf.Read<UInt32>(m.maxSP);
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
	if (m_updateMask.IsBit(favorMask))
	{
		buf.Read<float>(m.favor);
	}
	if (m_updateMask.IsBit(healthMask))
	{
		buf.Read<UInt16>(m.health);
	}
	if (m_updateMask.IsBit(goodnessMask))
	{
		buf.Read<Int32>(m.goodness);
	}
	if (m_updateMask.IsBit(stateMask))
	{
		buf.Read<UInt8>(m.state);
	}
	if (m_updateMask.IsBit(buffMask))
	{
		buf.Read<String>(m.buff);
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
	if (m_updateMask.IsBit(buffCacheMask))
	{
		buf.Read<String>(m.buffCache);
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
	if (m_updateMask.IsBit(growLevelMask))
	{
		buf.Read<UInt8>(m.growLevel);
	}
}

void Partner_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Partner_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.partner_id, m.mob_id);
	if (IsUpdated(ownerMask)) OnUpdateOwner(m.owner_id);
	if (IsUpdated(infoMask)) OnUpdateInfo(m.raname);
	if (IsUpdated(levelMask)) OnUpdateLevel(m.level);
	if (IsUpdated(nextLvExpMask)) OnUpdateNextLvExp(m.nextLvExp);
	if (IsUpdated(expMask)) OnUpdateExp(m.exp);
	if (IsUpdated(attrib1Mask)) OnUpdateAttrib1(m.STA, m.SPR, m.STR, m.CON, m.AGI);
	if (IsUpdated(attrib2Mask)) OnUpdateAttrib2(m.ATK, m.DEF, m.HIT, m.EVA, m.POW, m.SPD);
	if (IsUpdated(attrPointMask)) OnUpdateAttrPoint(m.attrPoint);
	if (IsUpdated(HPMask)) OnUpdateHP(m.HP, m.maxHP);
	if (IsUpdated(SPMask)) OnUpdateSP(m.SP, m.maxSP);
	if (IsUpdated(nativeMask)) OnUpdateNative(m.HP_native, m.SP_native, m.ATK_native, m.DEF_native, m.SPD_native, m.POW_native, m.HIT_native, m.EVA_native);
	if (IsUpdated(growMask)) OnUpdateGrow(m.grow);
	if (IsUpdated(favorMask)) OnUpdateFavor(m.favor);
	if (IsUpdated(healthMask)) OnUpdateHealth(m.health);
	if (IsUpdated(goodnessMask)) OnUpdateGoodness(m.goodness);
	if (IsUpdated(stateMask)) OnUpdateState(m.state);
	if (IsUpdated(buffMask)) OnUpdateBuff(m.buff);
	if (IsUpdated(soulboundStateMask)) OnUpdateSoulboundState(m.soulboundState);
	if (IsUpdated(ExpireDateMask)) OnUpdateExpireDate(m.expireDate);
	if (IsUpdated(ElementMask)) OnUpdateElement(m.element);
	if (IsUpdated(OutLookColorMask)) OnUpdateOutLookColor(m.outLookColor);
	if (IsUpdated(buffCacheMask)) OnUpdateBuffCache(m.buffCache);
	if (IsUpdated(inWareHouseMask)) OnUpdateInWareHouse(m.inWareHouse);
	if (IsUpdated(assetLockMask)) OnUpdateAssetLock(m.lock);
	if (IsUpdated(CultivationMask)) OnUpdateCultivation(m.cultivation);
	if (IsUpdated(PracticeMask)) OnUpdatePractice(m.practice);
	if (IsUpdated(growLevelMask)) OnUpdateGrowLevel(m.growLevel);
}

#pragma optimize("", off)
RPCResult Partner_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Partner_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_Partner_CB_RaiseAttrib:
		{
			_result = CB_RaiseAttrib(pPeer, context);
		}
		break;
	case RPC_Partner_CB_ChangePartnerName:
		{
			bool success;
			pBuf->Read<bool>(success);
			_result = CB_ChangePartnerName(pPeer, context, success);
		}
		break;
	case RPC_Partner_CB_ChangePartnerState:
		{
			_result = CB_ChangePartnerState(pPeer, context);
		}
		break;
	case RPC_Partner_CB_UpdateSuccess:
		{
			_result = CB_UpdateSuccess(pPeer, context);
		}
		break;
	case RPC_Partner_CB_LearnSkillMessage:
		{
			UInt16 skillID;
			pBuf->Read<UInt16>(skillID);
			_result = CB_LearnSkillMessage(pPeer, context, skillID);
		}
		break;
	case RPC_Partner_CB_FalseJoinBattle:
		{
			UInt8 MsgType;
			pBuf->Read<UInt8>(MsgType);
			_result = CB_FalseJoinBattle(pPeer, context, MsgType);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Partner_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Partner_UP_STATE:
			_result = RPCDecode<169612404>(pPeer, pBuf);
		break;
	case RPC_Partner_CB_RaiseAttrib:
			_result = RPCDecode<318600191>(pPeer, pBuf);
		break;
	case RPC_Partner_CB_ChangePartnerName:
			_result = RPCDecode<469255547>(pPeer, pBuf);
		break;
	case RPC_Partner_CB_ChangePartnerState:
			_result = RPCDecode<1103260278>(pPeer, pBuf);
		break;
	case RPC_Partner_CB_UpdateSuccess:
			_result = RPCDecode<371536329>(pPeer, pBuf);
		break;
	case RPC_Partner_CB_LearnSkillMessage:
			_result = RPCDecode<1173311946>(pPeer, pBuf);
		break;
	case RPC_Partner_CB_FalseJoinBattle:
			_result = RPCDecode<649489524>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
