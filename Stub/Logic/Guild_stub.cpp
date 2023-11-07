//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Guild_stub.h"

void Guild_Stub::SetId(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void Guild_Stub::SetGuild_uid(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void Guild_Stub::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(nameMask, modified);
}

void Guild_Stub::SetCreator(const String &creator)
{
	bool modified = false;
	if (m.creator != creator) { m.creator = creator; modified = true; }
	MarkUpdateAll(creatorMask, modified);
}

void Guild_Stub::SetMaster(const String &master)
{
	bool modified = false;
	if (m.master != master) { m.master = master; modified = true; }
	MarkUpdateAll(masterMask, modified);
}

void Guild_Stub::SetAlliance(const UInt32 &alliance_id)
{
	bool modified = false;
	if (m.alliance_id != alliance_id) { m.alliance_id = alliance_id; modified = true; }
	MarkUpdateAll(allianceMask, modified);
}

void Guild_Stub::SetAlliance_id(const UInt32 &alliance_id)
{
	bool modified = false;
	if (m.alliance_id != alliance_id) { m.alliance_id = alliance_id; modified = true; }
	MarkUpdateAll(allianceMask, modified);
}

void Guild_Stub::SetEnemy(const UInt32 &enemy_id)
{
	bool modified = false;
	if (m.enemy_id != enemy_id) { m.enemy_id = enemy_id; modified = true; }
	MarkUpdateAll(enemyMask, modified);
}

void Guild_Stub::SetEnemy_id(const UInt32 &enemy_id)
{
	bool modified = false;
	if (m.enemy_id != enemy_id) { m.enemy_id = enemy_id; modified = true; }
	MarkUpdateAll(enemyMask, modified);
}

void Guild_Stub::SetSecMaster(const String &secMasterName)
{
	bool modified = false;
	if (m.secMasterName != secMasterName) { m.secMasterName = secMasterName; modified = true; }
	MarkUpdateAll(secMasterMask, modified);
}

void Guild_Stub::SetSecMasterName(const String &secMasterName)
{
	bool modified = false;
	if (m.secMasterName != secMasterName) { m.secMasterName = secMasterName; modified = true; }
	MarkUpdateAll(secMasterMask, modified);
}

void Guild_Stub::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void Guild_Stub::SetProsper(const UInt32 &prosper, const UInt16 &max_prosper)
{
	bool modified = false;
	if (m.prosper != prosper) { m.prosper = prosper; modified = true; }
	if (m.max_prosper != max_prosper) { m.max_prosper = max_prosper; modified = true; }
	MarkUpdateAll(prosperMask, modified);
}

void Guild_Stub::SetProsper(const UInt32 &prosper)
{
	bool modified = false;
	if (m.prosper != prosper) { m.prosper = prosper; modified = true; }
	MarkUpdateAll(prosperMask, modified);
}

void Guild_Stub::SetMax_prosper(const UInt16 &max_prosper)
{
	bool modified = false;
	if (m.max_prosper != max_prosper) { m.max_prosper = max_prosper; modified = true; }
	MarkUpdateAll(prosperMask, modified);
}

void Guild_Stub::SetFund(const UInt32 &fund)
{
	bool modified = false;
	if (m.fund != fund) { m.fund = fund; modified = true; }
	MarkUpdateAll(fundMask, modified);
}

void Guild_Stub::SetMemberNum(const UInt16 &totalMember, const UInt16 &maxMember)
{
	bool modified = false;
	if (m.totalMember != totalMember) { m.totalMember = totalMember; modified = true; }
	if (m.maxMember != maxMember) { m.maxMember = maxMember; modified = true; }
	MarkUpdateAll(memberNumMask, modified);
}

void Guild_Stub::SetTotalMember(const UInt16 &totalMember)
{
	bool modified = false;
	if (m.totalMember != totalMember) { m.totalMember = totalMember; modified = true; }
	MarkUpdateAll(memberNumMask, modified);
}

void Guild_Stub::SetMaxMember(const UInt16 &maxMember)
{
	bool modified = false;
	if (m.maxMember != maxMember) { m.maxMember = maxMember; modified = true; }
	MarkUpdateAll(memberNumMask, modified);
}

void Guild_Stub::SetAim(const String &aim)
{
	bool modified = false;
	if (m.aim != aim) { m.aim = aim; modified = true; }
	MarkUpdateAll(aimMask, modified);
}

void Guild_Stub::SetRule(const String &rule)
{
	bool modified = false;
	if (m.rule != rule) { m.rule = rule; modified = true; }
	MarkUpdateAll(ruleMask, modified);
}

void Guild_Stub::SetGuildPvPScore(const UInt32 &guildPvPScore)
{
	bool modified = false;
	if (m.guildPvPScore != guildPvPScore) { m.guildPvPScore = guildPvPScore; modified = true; }
	MarkUpdateAll(guildPvPScoreMask, modified);
}

void Guild_Stub::SetGuildPvPBid(const UInt32 &guildPvPBid)
{
	bool modified = false;
	if (m.guildPvPBid != guildPvPBid) { m.guildPvPBid = guildPvPBid; modified = true; }
	MarkUpdateAll(guildPvPBidMask, modified);
}

void Guild_Stub::SetGuildPvPGroup(const UInt8 &guildPvPGroup)
{
	bool modified = false;
	if (m.guildPvPGroup != guildPvPGroup) { m.guildPvPGroup = guildPvPGroup; modified = true; }
	MarkUpdateAll(guildPvPGroupMask, modified);
}

void Guild_Stub::SetGroupPvPVSGuild(const String &groupPvPVSGuild)
{
	bool modified = false;
	if (m.groupPvPVSGuild != groupPvPVSGuild) { m.groupPvPVSGuild = groupPvPVSGuild; modified = true; }
	MarkUpdateAll(groupPvPVSGuildMask, modified);
}

void Guild_Stub::SetManorScale(const UInt8 &manorScale)
{
	bool modified = false;
	if (m.manorScale != manorScale) { m.manorScale = manorScale; modified = true; }
	MarkUpdateAll(manorScaleMask, modified);
}

void Guild_Stub::SetManorRoom(const UInt8 &room)
{
	bool modified = false;
	if (m.room != room) { m.room = room; modified = true; }
	MarkUpdateAll(manorRoomMask, modified);
}

void Guild_Stub::SetRoom(const UInt8 &room)
{
	bool modified = false;
	if (m.room != room) { m.room = room; modified = true; }
	MarkUpdateAll(manorRoomMask, modified);
}

void Guild_Stub::SetManorMaterial(const UInt32 &material)
{
	bool modified = false;
	if (m.material != material) { m.material = material; modified = true; }
	MarkUpdateAll(manorMaterialMask, modified);
}

void Guild_Stub::SetMaterial(const UInt32 &material)
{
	bool modified = false;
	if (m.material != material) { m.material = material; modified = true; }
	MarkUpdateAll(manorMaterialMask, modified);
}

void Guild_Stub::SetManorGuard(const UInt8 &guard)
{
	bool modified = false;
	if (m.guard != guard) { m.guard = guard; modified = true; }
	MarkUpdateAll(manorGuardMask, modified);
}

void Guild_Stub::SetGuard(const UInt8 &guard)
{
	bool modified = false;
	if (m.guard != guard) { m.guard = guard; modified = true; }
	MarkUpdateAll(manorGuardMask, modified);
}

void Guild_Stub::SetManorTrainPlayerPow(const UInt8 &trainCharExPow, const UInt8 &trainCharInPow)
{
	bool modified = false;
	if (m.trainCharExPow != trainCharExPow) { m.trainCharExPow = trainCharExPow; modified = true; }
	if (m.trainCharInPow != trainCharInPow) { m.trainCharInPow = trainCharInPow; modified = true; }
	MarkUpdateAll(manorTrainPlayerPowMask, modified);
}

void Guild_Stub::SetTrainCharExPow(const UInt8 &trainCharExPow)
{
	bool modified = false;
	if (m.trainCharExPow != trainCharExPow) { m.trainCharExPow = trainCharExPow; modified = true; }
	MarkUpdateAll(manorTrainPlayerPowMask, modified);
}

void Guild_Stub::SetTrainCharInPow(const UInt8 &trainCharInPow)
{
	bool modified = false;
	if (m.trainCharInPow != trainCharInPow) { m.trainCharInPow = trainCharInPow; modified = true; }
	MarkUpdateAll(manorTrainPlayerPowMask, modified);
}

void Guild_Stub::SetManorTrainPartnerPow(const UInt8 &trainPartnerExPow, const UInt8 &trainPartnerInPow)
{
	bool modified = false;
	if (m.trainPartnerExPow != trainPartnerExPow) { m.trainPartnerExPow = trainPartnerExPow; modified = true; }
	if (m.trainPartnerInPow != trainPartnerInPow) { m.trainPartnerInPow = trainPartnerInPow; modified = true; }
	MarkUpdateAll(manorTrainPartnerPowMask, modified);
}

void Guild_Stub::SetTrainPartnerExPow(const UInt8 &trainPartnerExPow)
{
	bool modified = false;
	if (m.trainPartnerExPow != trainPartnerExPow) { m.trainPartnerExPow = trainPartnerExPow; modified = true; }
	MarkUpdateAll(manorTrainPartnerPowMask, modified);
}

void Guild_Stub::SetTrainPartnerInPow(const UInt8 &trainPartnerInPow)
{
	bool modified = false;
	if (m.trainPartnerInPow != trainPartnerInPow) { m.trainPartnerInPow = trainPartnerInPow; modified = true; }
	MarkUpdateAll(manorTrainPartnerPowMask, modified);
}

void Guild_Stub::SetManorRoomProperty(const UInt8 &drugLv, const UInt8 &cookLv, const UInt8 &wineLv, const UInt8 &hpLv, const UInt8 &spLv)
{
	bool modified = false;
	if (m.drugLv != drugLv) { m.drugLv = drugLv; modified = true; }
	if (m.cookLv != cookLv) { m.cookLv = cookLv; modified = true; }
	if (m.wineLv != wineLv) { m.wineLv = wineLv; modified = true; }
	if (m.hpLv != hpLv) { m.hpLv = hpLv; modified = true; }
	if (m.spLv != spLv) { m.spLv = spLv; modified = true; }
	MarkUpdateAll(manorRoomPropertyMask, modified);
}

void Guild_Stub::SetDrugLv(const UInt8 &drugLv)
{
	bool modified = false;
	if (m.drugLv != drugLv) { m.drugLv = drugLv; modified = true; }
	MarkUpdateAll(manorRoomPropertyMask, modified);
}

void Guild_Stub::SetCookLv(const UInt8 &cookLv)
{
	bool modified = false;
	if (m.cookLv != cookLv) { m.cookLv = cookLv; modified = true; }
	MarkUpdateAll(manorRoomPropertyMask, modified);
}

void Guild_Stub::SetWineLv(const UInt8 &wineLv)
{
	bool modified = false;
	if (m.wineLv != wineLv) { m.wineLv = wineLv; modified = true; }
	MarkUpdateAll(manorRoomPropertyMask, modified);
}

void Guild_Stub::SetHpLv(const UInt8 &hpLv)
{
	bool modified = false;
	if (m.hpLv != hpLv) { m.hpLv = hpLv; modified = true; }
	MarkUpdateAll(manorRoomPropertyMask, modified);
}

void Guild_Stub::SetSpLv(const UInt8 &spLv)
{
	bool modified = false;
	if (m.spLv != spLv) { m.spLv = spLv; modified = true; }
	MarkUpdateAll(manorRoomPropertyMask, modified);
}

void Guild_Stub::SetManorMaintainNum(const UInt32 &maintainNum)
{
	bool modified = false;
	if (m.maintainNum != maintainNum) { m.maintainNum = maintainNum; modified = true; }
	MarkUpdateAll(manorMaintainNumMask, modified);
}

void Guild_Stub::SetMaintainNum(const UInt32 &maintainNum)
{
	bool modified = false;
	if (m.maintainNum != maintainNum) { m.maintainNum = maintainNum; modified = true; }
	MarkUpdateAll(manorMaintainNumMask, modified);
}

void Guild_Stub::SetLastMaintainDate(const UInt32 &lastMaintainDate)
{
	bool modified = false;
	if (m.lastMaintainDate != lastMaintainDate) { m.lastMaintainDate = lastMaintainDate; modified = true; }
	MarkUpdateAll(lastMaintainDateMask, modified);
}

void Guild_Stub::SetManorAura(const UInt8 &aura)
{
	bool modified = false;
	if (m.aura != aura) { m.aura = aura; modified = true; }
	MarkUpdateAll(manorAuraMask, modified);
}

void Guild_Stub::SetAura(const UInt8 &aura)
{
	bool modified = false;
	if (m.aura != aura) { m.aura = aura; modified = true; }
	MarkUpdateAll(manorAuraMask, modified);
}

void Guild_Stub::SetOfflinetime(const UInt32 &offlinetime)
{
	bool modified = false;
	if (m.offlinetime != offlinetime) { m.offlinetime = offlinetime; modified = true; }
	MarkUpdateAll(offlinetimeMask, modified);
}

void Guild_Stub::SetFailMantainPayNum(const UInt32 &failPayNum)
{
	bool modified = false;
	if (m.failPayNum != failPayNum) { m.failPayNum = failPayNum; modified = true; }
	MarkUpdateAll(failMantainPayNumMask, modified);
}

void Guild_Stub::SetFailPayNum(const UInt32 &failPayNum)
{
	bool modified = false;
	if (m.failPayNum != failPayNum) { m.failPayNum = failPayNum; modified = true; }
	MarkUpdateAll(failMantainPayNumMask, modified);
}

void Guild_Stub::SetClearBusinessDate(const UInt32 &clearBusinessDate)
{
	bool modified = false;
	if (m.clearBusinessDate != clearBusinessDate) { m.clearBusinessDate = clearBusinessDate; modified = true; }
	MarkUpdateAll(clearBusinessDateMask, modified);
}

void Guild_Stub::SetAutoManageBusiness(const Boolean &autoManageBusiness)
{
	bool modified = false;
	if (m.autoManageBusiness != autoManageBusiness) { m.autoManageBusiness = autoManageBusiness; modified = true; }
	MarkUpdateAll(autoManageBusinessMask, modified);
}



void Guild_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Guild_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.guild_uid);
	}
	if (mask.IsBit(nameMask))
	{
		buf.Write<String>(m.name);
	}
	if (mask.IsBit(creatorMask))
	{
		buf.Write<String>(m.creator);
	}
	if (mask.IsBit(masterMask))
	{
		buf.Write<String>(m.master);
	}
	if (mask.IsBit(allianceMask))
	{
		buf.Write<UInt32>(m.alliance_id);
	}
	if (mask.IsBit(enemyMask))
	{
		buf.Write<UInt32>(m.enemy_id);
	}
	if (mask.IsBit(secMasterMask))
	{
		buf.Write<String>(m.secMasterName);
	}
	if (mask.IsBit(levelMask))
	{
		buf.Write<UInt16>(m.level);
	}
	if (mask.IsBit(prosperMask))
	{
		buf.Write<UInt32>(m.prosper);
		buf.Write<UInt16>(m.max_prosper);
	}
	if (mask.IsBit(fundMask))
	{
		buf.Write<UInt32>(m.fund);
	}
	if (mask.IsBit(memberNumMask))
	{
		buf.Write<UInt16>(m.totalMember);
		buf.Write<UInt16>(m.maxMember);
	}
	if (mask.IsBit(aimMask))
	{
		buf.Write<String>(m.aim);
	}
	if (mask.IsBit(ruleMask))
	{
		buf.Write<String>(m.rule);
	}
	if (mask.IsBit(guildPvPScoreMask))
	{
		buf.Write<UInt32>(m.guildPvPScore);
	}
	if (mask.IsBit(guildPvPBidMask))
	{
		buf.Write<UInt32>(m.guildPvPBid);
	}
	if (mask.IsBit(guildPvPGroupMask))
	{
		buf.Write<UInt8>(m.guildPvPGroup);
	}
	if (mask.IsBit(groupPvPVSGuildMask))
	{
		buf.Write<String>(m.groupPvPVSGuild);
	}
	if (mask.IsBit(manorScaleMask))
	{
		buf.Write<UInt8>(m.manorScale);
	}
	if (mask.IsBit(manorRoomMask))
	{
		buf.Write<UInt8>(m.room);
	}
	if (mask.IsBit(manorMaterialMask))
	{
		buf.Write<UInt32>(m.material);
	}
	if (mask.IsBit(manorGuardMask))
	{
		buf.Write<UInt8>(m.guard);
	}
	if (mask.IsBit(manorTrainPlayerPowMask))
	{
		buf.Write<UInt8>(m.trainCharExPow);
		buf.Write<UInt8>(m.trainCharInPow);
	}
	if (mask.IsBit(manorTrainPartnerPowMask))
	{
		buf.Write<UInt8>(m.trainPartnerExPow);
		buf.Write<UInt8>(m.trainPartnerInPow);
	}
	if (mask.IsBit(manorRoomPropertyMask))
	{
		buf.Write<UInt8>(m.drugLv);
		buf.Write<UInt8>(m.cookLv);
		buf.Write<UInt8>(m.wineLv);
		buf.Write<UInt8>(m.hpLv);
		buf.Write<UInt8>(m.spLv);
	}
	if (mask.IsBit(manorMaintainNumMask))
	{
		buf.Write<UInt32>(m.maintainNum);
	}
	if (mask.IsBit(lastMaintainDateMask))
	{
		buf.Write<UInt32>(m.lastMaintainDate);
	}
	if (mask.IsBit(manorAuraMask))
	{
		buf.Write<UInt8>(m.aura);
	}
	if (mask.IsBit(offlinetimeMask))
	{
		buf.Write<UInt32>(m.offlinetime);
	}
	if (mask.IsBit(failMantainPayNumMask))
	{
		buf.Write<UInt32>(m.failPayNum);
	}
	if (mask.IsBit(clearBusinessDateMask))
	{
		buf.Write<UInt32>(m.clearBusinessDate);
	}
	if (mask.IsBit(autoManageBusinessMask))
	{
		buf.Write<Boolean>(m.autoManageBusiness);
	}
}

RPCResult Guild_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Guild_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Guild_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Guild_REQ_STATE:
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

RPCResult Guild_Stub::CB_AddMember(LPCPEER pPeer, RPCContext &context, NetObject* pMember)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Guild_CB_AddMember);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(pMember);
	return RPCSend<898267510>(pPeer, pBuf);
}

RPCResult Guild_Stub::CB_RemoveMember(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &reason)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Guild_CB_RemoveMember);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<String>(reason);
	return RPCSend<104226238>(pPeer, pBuf);
}

RPCResult Guild_Stub::CB_AddApplicant(LPCPEER pPeer, RPCContext &context, NetObject* pObj)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Guild_CB_AddApplicant);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(pObj);
	return RPCSend<477824439>(pPeer, pBuf);
}

RPCResult Guild_Stub::CB_RemoveApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Guild_CB_RemoveApplicant);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<1885145231>(pPeer, pBuf);
}

