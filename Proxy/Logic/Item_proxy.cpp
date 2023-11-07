//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Item_proxy.h"


void Item_Proxy::ReadAttr(CONST Buffer &buf)
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
	if (m_updateMask.IsBit(detailLvMask))
	{
		buf.Read<UInt8>(m.detailLv);
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

void Item_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Item_Proxy::OnUpdate()
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
	if (IsUpdated(detailLvMask)) OnUpdateDetailLv(m.detailLv);
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
RPCResult Item_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Item_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Item_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Item_UP_STATE:
			_result = RPCDecode<98268314>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
