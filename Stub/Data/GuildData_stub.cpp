//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/GuildData_stub.h"

void GuildData_Stub::SetId(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void GuildData_Stub::SetGuild_uid(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void GuildData_Stub::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(nameMask, modified);
}

void GuildData_Stub::SetCreator(const String &creator)
{
	bool modified = false;
	if (m.creator != creator) { m.creator = creator; modified = true; }
	MarkUpdateAll(creatorMask, modified);
}

void GuildData_Stub::SetMaster(const String &master)
{
	bool modified = false;
	if (m.master != master) { m.master = master; modified = true; }
	MarkUpdateAll(masterMask, modified);
}

void GuildData_Stub::SetAlliance(const UInt32 &alliance_id)
{
	bool modified = false;
	if (m.alliance_id != alliance_id) { m.alliance_id = alliance_id; modified = true; }
	MarkUpdateAll(allianceMask, modified);
}

void GuildData_Stub::SetAlliance_id(const UInt32 &alliance_id)
{
	bool modified = false;
	if (m.alliance_id != alliance_id) { m.alliance_id = alliance_id; modified = true; }
	MarkUpdateAll(allianceMask, modified);
}

void GuildData_Stub::SetEnemy(const UInt32 &enemy_id)
{
	bool modified = false;
	if (m.enemy_id != enemy_id) { m.enemy_id = enemy_id; modified = true; }
	MarkUpdateAll(enemyMask, modified);
}

void GuildData_Stub::SetEnemy_id(const UInt32 &enemy_id)
{
	bool modified = false;
	if (m.enemy_id != enemy_id) { m.enemy_id = enemy_id; modified = true; }
	MarkUpdateAll(enemyMask, modified);
}

void GuildData_Stub::SetSecMaster(const String &secMasterName)
{
	bool modified = false;
	if (m.secMasterName != secMasterName) { m.secMasterName = secMasterName; modified = true; }
	MarkUpdateAll(secMasterMask, modified);
}

void GuildData_Stub::SetSecMasterName(const String &secMasterName)
{
	bool modified = false;
	if (m.secMasterName != secMasterName) { m.secMasterName = secMasterName; modified = true; }
	MarkUpdateAll(secMasterMask, modified);
}

void GuildData_Stub::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void GuildData_Stub::SetProsper(const UInt32 &prosper, const UInt16 &max_prosper)
{
	bool modified = false;
	if (m.prosper != prosper) { m.prosper = prosper; modified = true; }
	if (m.max_prosper != max_prosper) { m.max_prosper = max_prosper; modified = true; }
	MarkUpdateAll(prosperMask, modified);
}

void GuildData_Stub::SetProsper(const UInt32 &prosper)
{
	bool modified = false;
	if (m.prosper != prosper) { m.prosper = prosper; modified = true; }
	MarkUpdateAll(prosperMask, modified);
}

void GuildData_Stub::SetMax_prosper(const UInt16 &max_prosper)
{
	bool modified = false;
	if (m.max_prosper != max_prosper) { m.max_prosper = max_prosper; modified = true; }
	MarkUpdateAll(prosperMask, modified);
}

void GuildData_Stub::SetFund(const UInt32 &fund)
{
	bool modified = false;
	if (m.fund != fund) { m.fund = fund; modified = true; }
	MarkUpdateAll(fundMask, modified);
}

void GuildData_Stub::SetMemberNum(const UInt16 &totalMember, const UInt16 &maxMember)
{
	bool modified = false;
	if (m.totalMember != totalMember) { m.totalMember = totalMember; modified = true; }
	if (m.maxMember != maxMember) { m.maxMember = maxMember; modified = true; }
	MarkUpdateAll(memberNumMask, modified);
}

void GuildData_Stub::SetTotalMember(const UInt16 &totalMember)
{
	bool modified = false;
	if (m.totalMember != totalMember) { m.totalMember = totalMember; modified = true; }
	MarkUpdateAll(memberNumMask, modified);
}

void GuildData_Stub::SetMaxMember(const UInt16 &maxMember)
{
	bool modified = false;
	if (m.maxMember != maxMember) { m.maxMember = maxMember; modified = true; }
	MarkUpdateAll(memberNumMask, modified);
}

void GuildData_Stub::SetAim(const String &aim)
{
	bool modified = false;
	if (m.aim != aim) { m.aim = aim; modified = true; }
	MarkUpdateAll(aimMask, modified);
}

void GuildData_Stub::SetRule(const String &rule)
{
	bool modified = false;
	if (m.rule != rule) { m.rule = rule; modified = true; }
	MarkUpdateAll(ruleMask, modified);
}

void GuildData_Stub::SetGuildPvPScore(const UInt32 &guildPvPScore)
{
	bool modified = false;
	if (m.guildPvPScore != guildPvPScore) { m.guildPvPScore = guildPvPScore; modified = true; }
	MarkUpdateAll(guildPvPScoreMask, modified);
}

void GuildData_Stub::SetGuildPvPBid(const UInt32 &guildPvPBid)
{
	bool modified = false;
	if (m.guildPvPBid != guildPvPBid) { m.guildPvPBid = guildPvPBid; modified = true; }
	MarkUpdateAll(guildPvPBidMask, modified);
}

void GuildData_Stub::SetGuildPvPGroup(const UInt8 &guildPvPGroup)
{
	bool modified = false;
	if (m.guildPvPGroup != guildPvPGroup) { m.guildPvPGroup = guildPvPGroup; modified = true; }
	MarkUpdateAll(guildPvPGroupMask, modified);
}

void GuildData_Stub::SetGroupPvPVSGuild(const String &groupPvPVSGuild)
{
	bool modified = false;
	if (m.groupPvPVSGuild != groupPvPVSGuild) { m.groupPvPVSGuild = groupPvPVSGuild; modified = true; }
	MarkUpdateAll(groupPvPVSGuildMask, modified);
}

void GuildData_Stub::SetManorScale(const UInt8 &manorScale)
{
	bool modified = false;
	if (m.manorScale != manorScale) { m.manorScale = manorScale; modified = true; }
	MarkUpdateAll(manorScaleMask, modified);
}

void GuildData_Stub::SetManorRoom(const UInt8 &room)
{
	bool modified = false;
	if (m.room != room) { m.room = room; modified = true; }
	MarkUpdateAll(manorRoomMask, modified);
}

void GuildData_Stub::SetRoom(const UInt8 &room)
{
	bool modified = false;
	if (m.room != room) { m.room = room; modified = true; }
	MarkUpdateAll(manorRoomMask, modified);
}

void GuildData_Stub::SetManorMaterial(const UInt32 &material)
{
	bool modified = false;
	if (m.material != material) { m.material = material; modified = true; }
	MarkUpdateAll(manorMaterialMask, modified);
}

void GuildData_Stub::SetMaterial(const UInt32 &material)
{
	bool modified = false;
	if (m.material != material) { m.material = material; modified = true; }
	MarkUpdateAll(manorMaterialMask, modified);
}

void GuildData_Stub::SetManorGuard(const UInt8 &guard)
{
	bool modified = false;
	if (m.guard != guard) { m.guard = guard; modified = true; }
	MarkUpdateAll(manorGuardMask, modified);
}

void GuildData_Stub::SetGuard(const UInt8 &guard)
{
	bool modified = false;
	if (m.guard != guard) { m.guard = guard; modified = true; }
	MarkUpdateAll(manorGuardMask, modified);
}

void GuildData_Stub::SetManorTrainPlayerPow(const UInt8 &trainCharExPow, const UInt8 &trainCharInPow)
{
	bool modified = false;
	if (m.trainCharExPow != trainCharExPow) { m.trainCharExPow = trainCharExPow; modified = true; }
	if (m.trainCharInPow != trainCharInPow) { m.trainCharInPow = trainCharInPow; modified = true; }
	MarkUpdateAll(manorTrainPlayerPowMask, modified);
}

void GuildData_Stub::SetTrainCharExPow(const UInt8 &trainCharExPow)
{
	bool modified = false;
	if (m.trainCharExPow != trainCharExPow) { m.trainCharExPow = trainCharExPow; modified = true; }
	MarkUpdateAll(manorTrainPlayerPowMask, modified);
}

void GuildData_Stub::SetTrainCharInPow(const UInt8 &trainCharInPow)
{
	bool modified = false;
	if (m.trainCharInPow != trainCharInPow) { m.trainCharInPow = trainCharInPow; modified = true; }
	MarkUpdateAll(manorTrainPlayerPowMask, modified);
}

void GuildData_Stub::SetManorTrainPartnerPow(const UInt8 &trainPartnerExPow, const UInt8 &trainPartnerInPow)
{
	bool modified = false;
	if (m.trainPartnerExPow != trainPartnerExPow) { m.trainPartnerExPow = trainPartnerExPow; modified = true; }
	if (m.trainPartnerInPow != trainPartnerInPow) { m.trainPartnerInPow = trainPartnerInPow; modified = true; }
	MarkUpdateAll(manorTrainPartnerPowMask, modified);
}

void GuildData_Stub::SetTrainPartnerExPow(const UInt8 &trainPartnerExPow)
{
	bool modified = false;
	if (m.trainPartnerExPow != trainPartnerExPow) { m.trainPartnerExPow = trainPartnerExPow; modified = true; }
	MarkUpdateAll(manorTrainPartnerPowMask, modified);
}

void GuildData_Stub::SetTrainPartnerInPow(const UInt8 &trainPartnerInPow)
{
	bool modified = false;
	if (m.trainPartnerInPow != trainPartnerInPow) { m.trainPartnerInPow = trainPartnerInPow; modified = true; }
	MarkUpdateAll(manorTrainPartnerPowMask, modified);
}

void GuildData_Stub::SetManorRoomProperty(const UInt8 &drugLv, const UInt8 &cookLv, const UInt8 &wineLv, const UInt8 &hpLv, const UInt8 &spLv)
{
	bool modified = false;
	if (m.drugLv != drugLv) { m.drugLv = drugLv; modified = true; }
	if (m.cookLv != cookLv) { m.cookLv = cookLv; modified = true; }
	if (m.wineLv != wineLv) { m.wineLv = wineLv; modified = true; }
	if (m.hpLv != hpLv) { m.hpLv = hpLv; modified = true; }
	if (m.spLv != spLv) { m.spLv = spLv; modified = true; }
	MarkUpdateAll(manorRoomPropertyMask, modified);
}

void GuildData_Stub::SetDrugLv(const UInt8 &drugLv)
{
	bool modified = false;
	if (m.drugLv != drugLv) { m.drugLv = drugLv; modified = true; }
	MarkUpdateAll(manorRoomPropertyMask, modified);
}

void GuildData_Stub::SetCookLv(const UInt8 &cookLv)
{
	bool modified = false;
	if (m.cookLv != cookLv) { m.cookLv = cookLv; modified = true; }
	MarkUpdateAll(manorRoomPropertyMask, modified);
}

void GuildData_Stub::SetWineLv(const UInt8 &wineLv)
{
	bool modified = false;
	if (m.wineLv != wineLv) { m.wineLv = wineLv; modified = true; }
	MarkUpdateAll(manorRoomPropertyMask, modified);
}

void GuildData_Stub::SetHpLv(const UInt8 &hpLv)
{
	bool modified = false;
	if (m.hpLv != hpLv) { m.hpLv = hpLv; modified = true; }
	MarkUpdateAll(manorRoomPropertyMask, modified);
}

void GuildData_Stub::SetSpLv(const UInt8 &spLv)
{
	bool modified = false;
	if (m.spLv != spLv) { m.spLv = spLv; modified = true; }
	MarkUpdateAll(manorRoomPropertyMask, modified);
}

void GuildData_Stub::SetManorMaintainNum(const UInt32 &maintainNum)
{
	bool modified = false;
	if (m.maintainNum != maintainNum) { m.maintainNum = maintainNum; modified = true; }
	MarkUpdateAll(manorMaintainNumMask, modified);
}

void GuildData_Stub::SetMaintainNum(const UInt32 &maintainNum)
{
	bool modified = false;
	if (m.maintainNum != maintainNum) { m.maintainNum = maintainNum; modified = true; }
	MarkUpdateAll(manorMaintainNumMask, modified);
}

void GuildData_Stub::SetLastMaintainDate(const UInt32 &lastMaintainDate)
{
	bool modified = false;
	if (m.lastMaintainDate != lastMaintainDate) { m.lastMaintainDate = lastMaintainDate; modified = true; }
	MarkUpdateAll(lastMaintainDateMask, modified);
}

void GuildData_Stub::SetManorAura(const UInt8 &aura)
{
	bool modified = false;
	if (m.aura != aura) { m.aura = aura; modified = true; }
	MarkUpdateAll(manorAuraMask, modified);
}

void GuildData_Stub::SetAura(const UInt8 &aura)
{
	bool modified = false;
	if (m.aura != aura) { m.aura = aura; modified = true; }
	MarkUpdateAll(manorAuraMask, modified);
}

void GuildData_Stub::SetOfflinetime(const UInt32 &offlinetime)
{
	bool modified = false;
	if (m.offlinetime != offlinetime) { m.offlinetime = offlinetime; modified = true; }
	MarkUpdateAll(offlinetimeMask, modified);
}

void GuildData_Stub::SetClearBusinessDate(const UInt32 &clearBusinessDate)
{
	bool modified = false;
	if (m.clearBusinessDate != clearBusinessDate) { m.clearBusinessDate = clearBusinessDate; modified = true; }
	MarkUpdateAll(clearBusinessDateMask, modified);
}

void GuildData_Stub::SetAutoManageBusiness(const Boolean &autoManageBusiness)
{
	bool modified = false;
	if (m.autoManageBusiness != autoManageBusiness) { m.autoManageBusiness = autoManageBusiness; modified = true; }
	MarkUpdateAll(autoManageBusinessMask, modified);
}


void GuildData_Stub::Clone(GuildData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(idMask) && !(obj->m.guild_uid == m.guild_uid)) {
		m.guild_uid = obj->m.guild_uid;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(nameMask) && !(obj->m.name == m.name)) {
		m.name = obj->m.name;
		MarkUpdateAll(nameMask, true);
	}
	if (mask.IsBit(creatorMask) && !(obj->m.creator == m.creator)) {
		m.creator = obj->m.creator;
		MarkUpdateAll(creatorMask, true);
	}
	if (mask.IsBit(masterMask) && !(obj->m.master == m.master)) {
		m.master = obj->m.master;
		MarkUpdateAll(masterMask, true);
	}
	if (mask.IsBit(allianceMask) && !(obj->m.alliance_id == m.alliance_id)) {
		m.alliance_id = obj->m.alliance_id;
		MarkUpdateAll(allianceMask, true);
	}
	if (mask.IsBit(enemyMask) && !(obj->m.enemy_id == m.enemy_id)) {
		m.enemy_id = obj->m.enemy_id;
		MarkUpdateAll(enemyMask, true);
	}
	if (mask.IsBit(secMasterMask) && !(obj->m.secMasterName == m.secMasterName)) {
		m.secMasterName = obj->m.secMasterName;
		MarkUpdateAll(secMasterMask, true);
	}
	if (mask.IsBit(levelMask) && !(obj->m.level == m.level)) {
		m.level = obj->m.level;
		MarkUpdateAll(levelMask, true);
	}
	if (mask.IsBit(prosperMask) && !(obj->m.prosper == m.prosper)) {
		m.prosper = obj->m.prosper;
		MarkUpdateAll(prosperMask, true);
	}
	if (mask.IsBit(prosperMask) && !(obj->m.max_prosper == m.max_prosper)) {
		m.max_prosper = obj->m.max_prosper;
		MarkUpdateAll(prosperMask, true);
	}
	if (mask.IsBit(fundMask) && !(obj->m.fund == m.fund)) {
		m.fund = obj->m.fund;
		MarkUpdateAll(fundMask, true);
	}
	if (mask.IsBit(memberNumMask) && !(obj->m.totalMember == m.totalMember)) {
		m.totalMember = obj->m.totalMember;
		MarkUpdateAll(memberNumMask, true);
	}
	if (mask.IsBit(memberNumMask) && !(obj->m.maxMember == m.maxMember)) {
		m.maxMember = obj->m.maxMember;
		MarkUpdateAll(memberNumMask, true);
	}
	if (mask.IsBit(aimMask) && !(obj->m.aim == m.aim)) {
		m.aim = obj->m.aim;
		MarkUpdateAll(aimMask, true);
	}
	if (mask.IsBit(ruleMask) && !(obj->m.rule == m.rule)) {
		m.rule = obj->m.rule;
		MarkUpdateAll(ruleMask, true);
	}
	if (mask.IsBit(guildPvPScoreMask) && !(obj->m.guildPvPScore == m.guildPvPScore)) {
		m.guildPvPScore = obj->m.guildPvPScore;
		MarkUpdateAll(guildPvPScoreMask, true);
	}
	if (mask.IsBit(guildPvPBidMask) && !(obj->m.guildPvPBid == m.guildPvPBid)) {
		m.guildPvPBid = obj->m.guildPvPBid;
		MarkUpdateAll(guildPvPBidMask, true);
	}
	if (mask.IsBit(guildPvPGroupMask) && !(obj->m.guildPvPGroup == m.guildPvPGroup)) {
		m.guildPvPGroup = obj->m.guildPvPGroup;
		MarkUpdateAll(guildPvPGroupMask, true);
	}
	if (mask.IsBit(groupPvPVSGuildMask) && !(obj->m.groupPvPVSGuild == m.groupPvPVSGuild)) {
		m.groupPvPVSGuild = obj->m.groupPvPVSGuild;
		MarkUpdateAll(groupPvPVSGuildMask, true);
	}
	if (mask.IsBit(manorScaleMask) && !(obj->m.manorScale == m.manorScale)) {
		m.manorScale = obj->m.manorScale;
		MarkUpdateAll(manorScaleMask, true);
	}
	if (mask.IsBit(manorRoomMask) && !(obj->m.room == m.room)) {
		m.room = obj->m.room;
		MarkUpdateAll(manorRoomMask, true);
	}
	if (mask.IsBit(manorMaterialMask) && !(obj->m.material == m.material)) {
		m.material = obj->m.material;
		MarkUpdateAll(manorMaterialMask, true);
	}
	if (mask.IsBit(manorGuardMask) && !(obj->m.guard == m.guard)) {
		m.guard = obj->m.guard;
		MarkUpdateAll(manorGuardMask, true);
	}
	if (mask.IsBit(manorTrainPlayerPowMask) && !(obj->m.trainCharExPow == m.trainCharExPow)) {
		m.trainCharExPow = obj->m.trainCharExPow;
		MarkUpdateAll(manorTrainPlayerPowMask, true);
	}
	if (mask.IsBit(manorTrainPlayerPowMask) && !(obj->m.trainCharInPow == m.trainCharInPow)) {
		m.trainCharInPow = obj->m.trainCharInPow;
		MarkUpdateAll(manorTrainPlayerPowMask, true);
	}
	if (mask.IsBit(manorTrainPartnerPowMask) && !(obj->m.trainPartnerExPow == m.trainPartnerExPow)) {
		m.trainPartnerExPow = obj->m.trainPartnerExPow;
		MarkUpdateAll(manorTrainPartnerPowMask, true);
	}
	if (mask.IsBit(manorTrainPartnerPowMask) && !(obj->m.trainPartnerInPow == m.trainPartnerInPow)) {
		m.trainPartnerInPow = obj->m.trainPartnerInPow;
		MarkUpdateAll(manorTrainPartnerPowMask, true);
	}
	if (mask.IsBit(manorRoomPropertyMask) && !(obj->m.drugLv == m.drugLv)) {
		m.drugLv = obj->m.drugLv;
		MarkUpdateAll(manorRoomPropertyMask, true);
	}
	if (mask.IsBit(manorRoomPropertyMask) && !(obj->m.cookLv == m.cookLv)) {
		m.cookLv = obj->m.cookLv;
		MarkUpdateAll(manorRoomPropertyMask, true);
	}
	if (mask.IsBit(manorRoomPropertyMask) && !(obj->m.wineLv == m.wineLv)) {
		m.wineLv = obj->m.wineLv;
		MarkUpdateAll(manorRoomPropertyMask, true);
	}
	if (mask.IsBit(manorRoomPropertyMask) && !(obj->m.hpLv == m.hpLv)) {
		m.hpLv = obj->m.hpLv;
		MarkUpdateAll(manorRoomPropertyMask, true);
	}
	if (mask.IsBit(manorRoomPropertyMask) && !(obj->m.spLv == m.spLv)) {
		m.spLv = obj->m.spLv;
		MarkUpdateAll(manorRoomPropertyMask, true);
	}
	if (mask.IsBit(manorMaintainNumMask) && !(obj->m.maintainNum == m.maintainNum)) {
		m.maintainNum = obj->m.maintainNum;
		MarkUpdateAll(manorMaintainNumMask, true);
	}
	if (mask.IsBit(lastMaintainDateMask) && !(obj->m.lastMaintainDate == m.lastMaintainDate)) {
		m.lastMaintainDate = obj->m.lastMaintainDate;
		MarkUpdateAll(lastMaintainDateMask, true);
	}
	if (mask.IsBit(manorAuraMask) && !(obj->m.aura == m.aura)) {
		m.aura = obj->m.aura;
		MarkUpdateAll(manorAuraMask, true);
	}
	if (mask.IsBit(offlinetimeMask) && !(obj->m.offlinetime == m.offlinetime)) {
		m.offlinetime = obj->m.offlinetime;
		MarkUpdateAll(offlinetimeMask, true);
	}
	if (mask.IsBit(clearBusinessDateMask) && !(obj->m.clearBusinessDate == m.clearBusinessDate)) {
		m.clearBusinessDate = obj->m.clearBusinessDate;
		MarkUpdateAll(clearBusinessDateMask, true);
	}
	if (mask.IsBit(autoManageBusinessMask) && !(obj->m.autoManageBusiness == m.autoManageBusiness)) {
		m.autoManageBusiness = obj->m.autoManageBusiness;
		MarkUpdateAll(autoManageBusinessMask, true);
	}
}

void GuildData_Stub::ReadAttr(CONST Buffer &buf)
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
	if (m_updateMask.IsBit(clearBusinessDateMask))
	{
		buf.Read<UInt32>(m.clearBusinessDate);
	}
	if (m_updateMask.IsBit(autoManageBusinessMask))
	{
		buf.Read<Boolean>(m.autoManageBusiness);
	}
}

void GuildData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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
	if (mask.IsBit(clearBusinessDateMask))
	{
		buf.Write<UInt32>(m.clearBusinessDate);
	}
	if (mask.IsBit(autoManageBusinessMask))
	{
		buf.Write<Boolean>(m.autoManageBusiness);
	}
}

RPCResult GuildData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult GuildData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_GuildData_REQ_STATE:
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

