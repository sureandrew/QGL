//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/BatActor_proxy.h"


void BatActor_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(info2Mask))
	{
		buf.Read<UInt8>(m.cclass);
		buf.Read<UInt8>(m.sexClass);
		buf.Read<UInt8>(m.faction);
	}
	if (m_updateMask.IsBit(indexMask))
	{
		buf.Read<UInt8>(m.index);
	}
	if (m_updateMask.IsBit(levelMask))
	{
		buf.Read<UInt16>(m.level);
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
	if (m_updateMask.IsBit(HPMask))
	{
		buf.Read<UInt32>(m.HP);
		buf.Read<Int32>(m.dHP);
		buf.Read<Int32>(m.dShowHP);
	}
	if (m_updateMask.IsBit(mHPMask))
	{
		buf.Read<UInt32>(m.mHP);
		buf.Read<Int32>(m.dMHP);
		buf.Read<Int32>(m.dShowMHP);
	}
	if (m_updateMask.IsBit(maxHPMask))
	{
		buf.Read<UInt32>(m.maxHP);
	}
	if (m_updateMask.IsBit(SPMask))
	{
		buf.Read<UInt32>(m.SP);
		buf.Read<Int32>(m.dSP);
		buf.Read<Int32>(m.dShowSP);
	}
	if (m_updateMask.IsBit(mSPMask))
	{
		buf.Read<UInt32>(m.mSP);
		buf.Read<Int32>(m.dMSP);
		buf.Read<Int32>(m.dShowMSP);
	}
	if (m_updateMask.IsBit(maxSPMask))
	{
		buf.Read<UInt32>(m.maxSP);
	}
	if (m_updateMask.IsBit(DPMask))
	{
		buf.Read<UInt16>(m.DP);
		buf.Read<Int16>(m.dDP);
		buf.Read<Int16>(m.dShowDP);
	}
	if (m_updateMask.IsBit(mDPMask))
	{
		buf.Read<UInt16>(m.mDP);
	}
	if (m_updateMask.IsBit(maxDPMask))
	{
		buf.Read<UInt16>(m.maxDP);
	}
	if (m_updateMask.IsBit(WCMask))
	{
		buf.Read<UInt16>(m.WC);
		buf.Read<Int16>(m.dWC);
		buf.Read<Int16>(m.dShowWC);
	}
	if (m_updateMask.IsBit(WcExpMask))
	{
		buf.Read<UInt16>(m.WcExp);
	}
	if (m_updateMask.IsBit(maxWCMask))
	{
		buf.Read<UInt16>(m.maxWC);
	}
	if (m_updateMask.IsBit(VPMask))
	{
		buf.Read<UInt16>(m.VP);
		buf.Read<Int16>(m.dVP);
		buf.Read<Int16>(m.dShowVP);
	}
	if (m_updateMask.IsBit(maxVPMask))
	{
		buf.Read<UInt16>(m.maxVP);
	}
	if (m_updateMask.IsBit(stateMask))
	{
		buf.Read<UInt8>(m.state);
	}
	if (m_updateMask.IsBit(buffMask))
	{
		buf.Read<String>(m.buff);
	}
	if (m_updateMask.IsBit(buffCacheMask))
	{
		buf.Read<String>(m.buffCache);
	}
	if (m_updateMask.IsBit(teamMask))
	{
		buf.Read<UInt8>(m.team);
	}
	if (m_updateMask.IsBit(positionMask))
	{
		buf.Read<UInt8>(m.position);
	}
	if (m_updateMask.IsBit(actionMask))
	{
		buf.Read<UInt8>(m.action);
	}
	if (m_updateMask.IsBit(reactionMask))
	{
		buf.Read<UInt8>(m.reaction);
	}
	if (m_updateMask.IsBit(targetMask))
	{
		buf.Read<UInt8>(m.target);
	}
	if (m_updateMask.IsBit(subTypeMask))
	{
		buf.Read<UInt32>(m.subType);
	}
	if (m_updateMask.IsBit(additionalMask))
	{
		buf.Read<UInt32>(m.additional);
	}
	if (m_updateMask.IsBit(linkNextMask))
	{
		buf.Read<Boolean>(m.linkNext);
	}
	if (m_updateMask.IsBit(leftBattleMask))
	{
		buf.Read<Boolean>(m.leftBattle);
	}
	if (m_updateMask.IsBit(goodnessMask))
	{
		buf.Read<Int32>(m.goodness);
	}
	if (m_updateMask.IsBit(petMask))
	{
		buf.Read<UInt32>(m.pet_uid);
		buf.Read<UInt32>(m.pet_id);
		buf.Read<String>(m.petName);
		buf.Read<UInt16>(m.petColorIndex);
	}
	if (m_updateMask.IsBit(pet_activeMask))
	{
		buf.Read<bool>(m.pet_active);
	}
	if (m_updateMask.IsBit(TPMask))
	{
		buf.Read<UInt16>(m.TP);
	}
	if (m_updateMask.IsBit(hitTogetherMask))
	{
		buf.Read<Boolean>(m.hitTogether);
	}
}

void BatActor_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void BatActor_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(info2Mask)) OnUpdateInfo2(m.cclass, m.sexClass, m.faction);
	if (IsUpdated(indexMask)) OnUpdateIndex(m.index);
	if (IsUpdated(levelMask)) OnUpdateLevel(m.level);
	if (IsUpdated(attrib1Mask)) OnUpdateAttrib1(m.STA, m.SPR, m.STR, m.CON, m.AGI);
	if (IsUpdated(attrib2Mask)) OnUpdateAttrib2(m.ATK, m.DEF, m.HIT, m.EVA, m.POW, m.SPD);
	if (IsUpdated(HPMask)) OnUpdateHP(m.HP, m.dHP, m.dShowHP);
	if (IsUpdated(mHPMask)) OnUpdateMHP(m.mHP, m.dMHP, m.dShowMHP);
	if (IsUpdated(maxHPMask)) OnUpdateMaxHP(m.maxHP);
	if (IsUpdated(SPMask)) OnUpdateSP(m.SP, m.dSP, m.dShowSP);
	if (IsUpdated(mSPMask)) OnUpdateMSP(m.mSP, m.dMSP, m.dShowMSP);
	if (IsUpdated(maxSPMask)) OnUpdateMaxSP(m.maxSP);
	if (IsUpdated(DPMask)) OnUpdateDP(m.DP, m.dDP, m.dShowDP);
	if (IsUpdated(mDPMask)) OnUpdateMDP(m.mDP);
	if (IsUpdated(maxDPMask)) OnUpdateMaxDP(m.maxDP);
	if (IsUpdated(WCMask)) OnUpdateWC(m.WC, m.dWC, m.dShowWC);
	if (IsUpdated(WcExpMask)) OnUpdateWcExp(m.WcExp);
	if (IsUpdated(maxWCMask)) OnUpdateMaxWC(m.maxWC);
	if (IsUpdated(VPMask)) OnUpdateVP(m.VP, m.dVP, m.dShowVP);
	if (IsUpdated(maxVPMask)) OnUpdateMaxVP(m.maxVP);
	if (IsUpdated(stateMask)) OnUpdateState(m.state);
	if (IsUpdated(buffMask)) OnUpdateBuff(m.buff);
	if (IsUpdated(buffCacheMask)) OnUpdateBuffCache(m.buffCache);
	if (IsUpdated(teamMask)) OnUpdateTeam(m.team);
	if (IsUpdated(positionMask)) OnUpdatePosition(m.position);
	if (IsUpdated(actionMask)) OnUpdateAction(m.action);
	if (IsUpdated(reactionMask)) OnUpdateReaction(m.reaction);
	if (IsUpdated(targetMask)) OnUpdateTarget(m.target);
	if (IsUpdated(subTypeMask)) OnUpdateSubType(m.subType);
	if (IsUpdated(additionalMask)) OnUpdateAdditional(m.additional);
	if (IsUpdated(linkNextMask)) OnUpdateLinkNext(m.linkNext);
	if (IsUpdated(leftBattleMask)) OnUpdateLeftBattle(m.leftBattle);
	if (IsUpdated(goodnessMask)) OnUpdateGoodness(m.goodness);
	if (IsUpdated(petMask)) OnUpdatePet(m.pet_uid, m.pet_id, m.petName, m.petColorIndex);
	if (IsUpdated(pet_activeMask)) OnUpdatePet_active(m.pet_active);
	if (IsUpdated(TPMask)) OnUpdateTP(m.TP);
	if (IsUpdated(hitTogetherMask)) OnUpdateHitTogether(m.hitTogether);
}

#pragma optimize("", off)
RPCResult BatActor_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_BatActor_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_BatActor_CB_Error:
		{
			UInt32 errCode;
			pBuf->Read<UInt32>(errCode);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, errCode, errMsg);
		}
		break;
	case RPC_BatActor_CB_CommandSuccess:
		{
			UInt8 cmd;
			pBuf->Read<UInt8>(cmd);
			_result = CB_CommandSuccess(pPeer, context, cmd);
		}
		break;
	case RPC_BatActor_CB_Inputed:
		{
			UInt8 cmd;
			pBuf->Read<UInt8>(cmd);
			_result = CB_Inputed(pPeer, context, cmd);
		}
		break;
	case RPC_BatActor_CB_AutoCount:
		{
			UInt16 autoCount;
			pBuf->Read<UInt16>(autoCount);
			_result = CB_AutoCount(pPeer, context, autoCount);
		}
		break;
	case RPC_BatActor_CB_BattleMsg:
		{
			UInt8 MsgType;
			pBuf->Read<UInt8>(MsgType);
			String Msg;
			pBuf->Read<String>(Msg);
			UInt16 subType;
			pBuf->Read<UInt16>(subType);
			_result = CB_BattleMsg(pPeer, context, MsgType, Msg, subType);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult BatActor_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_BatActor_UP_STATE:
			_result = RPCDecode<1902991750>(pPeer, pBuf);
		break;
	case RPC_BatActor_CB_Error:
			_result = RPCDecode<1236285136>(pPeer, pBuf);
		break;
	case RPC_BatActor_CB_CommandSuccess:
			_result = RPCDecode<766788022>(pPeer, pBuf);
		break;
	case RPC_BatActor_CB_Inputed:
			_result = RPCDecode<91956427>(pPeer, pBuf);
		break;
	case RPC_BatActor_CB_AutoCount:
			_result = RPCDecode<573711624>(pPeer, pBuf);
		break;
	case RPC_BatActor_CB_BattleMsg:
			_result = RPCDecode<71531305>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
