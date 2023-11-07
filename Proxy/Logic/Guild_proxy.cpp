//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Guild_proxy.h"


void Guild_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.guild_uid);
	}
	if (m_updateMask.IsBit(nameMask))
	{
		buf.Read<String>(m.name);
	}
	if (m_updateMask.IsBit(creatorMask))
	{
		buf.Read<String>(m.creator);
	}
	if (m_updateMask.IsBit(masterMask))
	{
		buf.Read<String>(m.master);
	}
	if (m_updateMask.IsBit(allianceMask))
	{
		buf.Read<UInt32>(m.alliance_id);
	}
	if (m_updateMask.IsBit(enemyMask))
	{
		buf.Read<UInt32>(m.enemy_id);
	}
	if (m_updateMask.IsBit(secMasterMask))
	{
		buf.Read<String>(m.secMasterName);
	}
	if (m_updateMask.IsBit(levelMask))
	{
		buf.Read<UInt16>(m.level);
	}
	if (m_updateMask.IsBit(prosperMask))
	{
		buf.Read<UInt32>(m.prosper);
		buf.Read<UInt16>(m.max_prosper);
	}
	if (m_updateMask.IsBit(fundMask))
	{
		buf.Read<UInt32>(m.fund);
	}
	if (m_updateMask.IsBit(memberNumMask))
	{
		buf.Read<UInt16>(m.totalMember);
		buf.Read<UInt16>(m.maxMember);
	}
	if (m_updateMask.IsBit(aimMask))
	{
		buf.Read<String>(m.aim);
	}
	if (m_updateMask.IsBit(ruleMask))
	{
		buf.Read<String>(m.rule);
	}
	if (m_updateMask.IsBit(guildPvPScoreMask))
	{
		buf.Read<UInt32>(m.guildPvPScore);
	}
	if (m_updateMask.IsBit(guildPvPBidMask))
	{
		buf.Read<UInt32>(m.guildPvPBid);
	}
	if (m_updateMask.IsBit(guildPvPGroupMask))
	{
		buf.Read<UInt8>(m.guildPvPGroup);
	}
	if (m_updateMask.IsBit(groupPvPVSGuildMask))
	{
		buf.Read<String>(m.groupPvPVSGuild);
	}
	if (m_updateMask.IsBit(manorScaleMask))
	{
		buf.Read<UInt8>(m.manorScale);
	}
	if (m_updateMask.IsBit(manorRoomMask))
	{
		buf.Read<UInt8>(m.room);
	}
	if (m_updateMask.IsBit(manorMaterialMask))
	{
		buf.Read<UInt32>(m.material);
	}
	if (m_updateMask.IsBit(manorGuardMask))
	{
		buf.Read<UInt8>(m.guard);
	}
	if (m_updateMask.IsBit(manorTrainPlayerPowMask))
	{
		buf.Read<UInt8>(m.trainCharExPow);
		buf.Read<UInt8>(m.trainCharInPow);
	}
	if (m_updateMask.IsBit(manorTrainPartnerPowMask))
	{
		buf.Read<UInt8>(m.trainPartnerExPow);
		buf.Read<UInt8>(m.trainPartnerInPow);
	}
	if (m_updateMask.IsBit(manorRoomPropertyMask))
	{
		buf.Read<UInt8>(m.drugLv);
		buf.Read<UInt8>(m.cookLv);
		buf.Read<UInt8>(m.wineLv);
		buf.Read<UInt8>(m.hpLv);
		buf.Read<UInt8>(m.spLv);
	}
	if (m_updateMask.IsBit(manorMaintainNumMask))
	{
		buf.Read<UInt32>(m.maintainNum);
	}
	if (m_updateMask.IsBit(lastMaintainDateMask))
	{
		buf.Read<UInt32>(m.lastMaintainDate);
	}
	if (m_updateMask.IsBit(manorAuraMask))
	{
		buf.Read<UInt8>(m.aura);
	}
	if (m_updateMask.IsBit(offlinetimeMask))
	{
		buf.Read<UInt32>(m.offlinetime);
	}
	if (m_updateMask.IsBit(failMantainPayNumMask))
	{
		buf.Read<UInt32>(m.failPayNum);
	}
	if (m_updateMask.IsBit(clearBusinessDateMask))
	{
		buf.Read<UInt32>(m.clearBusinessDate);
	}
	if (m_updateMask.IsBit(autoManageBusinessMask))
	{
		buf.Read<Boolean>(m.autoManageBusiness);
	}
}

void Guild_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Guild_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.guild_uid);
	if (IsUpdated(nameMask)) OnUpdateName(m.name);
	if (IsUpdated(creatorMask)) OnUpdateCreator(m.creator);
	if (IsUpdated(masterMask)) OnUpdateMaster(m.master);
	if (IsUpdated(allianceMask)) OnUpdateAlliance(m.alliance_id);
	if (IsUpdated(enemyMask)) OnUpdateEnemy(m.enemy_id);
	if (IsUpdated(secMasterMask)) OnUpdateSecMaster(m.secMasterName);
	if (IsUpdated(levelMask)) OnUpdateLevel(m.level);
	if (IsUpdated(prosperMask)) OnUpdateProsper(m.prosper, m.max_prosper);
	if (IsUpdated(fundMask)) OnUpdateFund(m.fund);
	if (IsUpdated(memberNumMask)) OnUpdateMemberNum(m.totalMember, m.maxMember);
	if (IsUpdated(aimMask)) OnUpdateAim(m.aim);
	if (IsUpdated(ruleMask)) OnUpdateRule(m.rule);
	if (IsUpdated(guildPvPScoreMask)) OnUpdateGuildPvPScore(m.guildPvPScore);
	if (IsUpdated(guildPvPBidMask)) OnUpdateGuildPvPBid(m.guildPvPBid);
	if (IsUpdated(guildPvPGroupMask)) OnUpdateGuildPvPGroup(m.guildPvPGroup);
	if (IsUpdated(groupPvPVSGuildMask)) OnUpdateGroupPvPVSGuild(m.groupPvPVSGuild);
	if (IsUpdated(manorScaleMask)) OnUpdateManorScale(m.manorScale);
	if (IsUpdated(manorRoomMask)) OnUpdateManorRoom(m.room);
	if (IsUpdated(manorMaterialMask)) OnUpdateManorMaterial(m.material);
	if (IsUpdated(manorGuardMask)) OnUpdateManorGuard(m.guard);
	if (IsUpdated(manorTrainPlayerPowMask)) OnUpdateManorTrainPlayerPow(m.trainCharExPow, m.trainCharInPow);
	if (IsUpdated(manorTrainPartnerPowMask)) OnUpdateManorTrainPartnerPow(m.trainPartnerExPow, m.trainPartnerInPow);
	if (IsUpdated(manorRoomPropertyMask)) OnUpdateManorRoomProperty(m.drugLv, m.cookLv, m.wineLv, m.hpLv, m.spLv);
	if (IsUpdated(manorMaintainNumMask)) OnUpdateManorMaintainNum(m.maintainNum);
	if (IsUpdated(lastMaintainDateMask)) OnUpdateLastMaintainDate(m.lastMaintainDate);
	if (IsUpdated(manorAuraMask)) OnUpdateManorAura(m.aura);
	if (IsUpdated(offlinetimeMask)) OnUpdateOfflinetime(m.offlinetime);
	if (IsUpdated(failMantainPayNumMask)) OnUpdateFailMantainPayNum(m.failPayNum);
	if (IsUpdated(clearBusinessDateMask)) OnUpdateClearBusinessDate(m.clearBusinessDate);
	if (IsUpdated(autoManageBusinessMask)) OnUpdateAutoManageBusiness(m.autoManageBusiness);
}

#pragma optimize("", off)
RPCResult Guild_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Guild_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_Guild_CB_AddMember:
		{
			NetObject* pMember;
			pBuf->Read<NetObject*>(pMember);
			_result = CB_AddMember(pPeer, context, pMember);
		}
		break;
	case RPC_Guild_CB_RemoveMember:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			String reason;
			pBuf->Read<String>(reason);
			_result = CB_RemoveMember(pPeer, context, char_id, reason);
		}
		break;
	case RPC_Guild_CB_AddApplicant:
		{
			NetObject* pObj;
			pBuf->Read<NetObject*>(pObj);
			_result = CB_AddApplicant(pPeer, context, pObj);
		}
		break;
	case RPC_Guild_CB_RemoveApplicant:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = CB_RemoveApplicant(pPeer, context, char_id);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Guild_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Guild_UP_STATE:
			_result = RPCDecode<327943234>(pPeer, pBuf);
		break;
	case RPC_Guild_CB_AddMember:
			_result = RPCDecode<898267510>(pPeer, pBuf);
		break;
	case RPC_Guild_CB_RemoveMember:
			_result = RPCDecode<104226238>(pPeer, pBuf);
		break;
	case RPC_Guild_CB_AddApplicant:
			_result = RPCDecode<477824439>(pPeer, pBuf);
		break;
	case RPC_Guild_CB_RemoveApplicant:
			_result = RPCDecode<1885145231>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
