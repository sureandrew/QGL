///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GUILDDATA_PROXY_H_
#define _RPC_GUILDDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/GuildData_enum.h"

class GuildData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &guild_uid);
	virtual void OnUpdateName(const String &name);
	virtual void OnUpdateCreator(const String &creator);
	virtual void OnUpdateMaster(const String &master);
	virtual void OnUpdateAlliance(const UInt32 &alliance_id);
	virtual void OnUpdateEnemy(const UInt32 &enemy_id);
	virtual void OnUpdateSecMaster(const String &secMasterName);
	virtual void OnUpdateLevel(const UInt16 &level);
	virtual void OnUpdateProsper(const UInt32 &prosper, const UInt16 &max_prosper);
	virtual void OnUpdateFund(const UInt32 &fund);
	virtual void OnUpdateMemberNum(const UInt16 &totalMember, const UInt16 &maxMember);
	virtual void OnUpdateAim(const String &aim);
	virtual void OnUpdateRule(const String &rule);
	virtual void OnUpdateGuildPvPScore(const UInt32 &guildPvPScore);
	virtual void OnUpdateGuildPvPBid(const UInt32 &guildPvPBid);
	virtual void OnUpdateGuildPvPGroup(const UInt8 &guildPvPGroup);
	virtual void OnUpdateGroupPvPVSGuild(const String &groupPvPVSGuild);
	virtual void OnUpdateManorScale(const UInt8 &manorScale);
	virtual void OnUpdateManorRoom(const UInt8 &room);
	virtual void OnUpdateManorMaterial(const UInt32 &material);
	virtual void OnUpdateManorGuard(const UInt8 &guard);
	virtual void OnUpdateManorTrainPlayerPow(const UInt8 &trainCharExPow, const UInt8 &trainCharInPow);
	virtual void OnUpdateManorTrainPartnerPow(const UInt8 &trainPartnerExPow, const UInt8 &trainPartnerInPow);
	virtual void OnUpdateManorRoomProperty(const UInt8 &drugLv, const UInt8 &cookLv, const UInt8 &wineLv, const UInt8 &hpLv, const UInt8 &spLv);
	virtual void OnUpdateManorMaintainNum(const UInt32 &maintainNum);
	virtual void OnUpdateLastMaintainDate(const UInt32 &lastMaintainDate);
	virtual void OnUpdateManorAura(const UInt8 &aura);
	virtual void OnUpdateOfflinetime(const UInt32 &offlinetime);
	virtual void OnUpdateClearBusinessDate(const UInt32 &clearBusinessDate);
	virtual void OnUpdateAutoManageBusiness(const Boolean &autoManageBusiness);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		nameMask	= Parent::NextFreeMask + 1,
		creatorMask	= Parent::NextFreeMask + 2,
		masterMask	= Parent::NextFreeMask + 3,
		allianceMask	= Parent::NextFreeMask + 4,
		enemyMask	= Parent::NextFreeMask + 5,
		secMasterMask	= Parent::NextFreeMask + 6,
		levelMask	= Parent::NextFreeMask + 7,
		prosperMask	= Parent::NextFreeMask + 8,
		fundMask	= Parent::NextFreeMask + 9,
		memberNumMask	= Parent::NextFreeMask + 10,
		aimMask	= Parent::NextFreeMask + 11,
		ruleMask	= Parent::NextFreeMask + 12,
		guildPvPScoreMask	= Parent::NextFreeMask + 13,
		guildPvPBidMask	= Parent::NextFreeMask + 14,
		guildPvPGroupMask	= Parent::NextFreeMask + 15,
		groupPvPVSGuildMask	= Parent::NextFreeMask + 16,
		manorScaleMask	= Parent::NextFreeMask + 17,
		manorRoomMask	= Parent::NextFreeMask + 18,
		manorMaterialMask	= Parent::NextFreeMask + 19,
		manorGuardMask	= Parent::NextFreeMask + 20,
		manorTrainPlayerPowMask	= Parent::NextFreeMask + 21,
		manorTrainPartnerPowMask	= Parent::NextFreeMask + 22,
		manorRoomPropertyMask	= Parent::NextFreeMask + 23,
		manorMaintainNumMask	= Parent::NextFreeMask + 24,
		lastMaintainDateMask	= Parent::NextFreeMask + 25,
		manorAuraMask	= Parent::NextFreeMask + 26,
		offlinetimeMask	= Parent::NextFreeMask + 27,
		clearBusinessDateMask	= Parent::NextFreeMask + 28,
		autoManageBusinessMask	= Parent::NextFreeMask + 29,
		NextFreeMask	= Parent::NextFreeMask + 30
	};
	const UInt32& GetGuild_uid() const { return m.guild_uid; }
	const String& GetName() const { return m.name; }
	const String& GetCreator() const { return m.creator; }
	const String& GetMaster() const { return m.master; }
	const UInt32& GetAlliance_id() const { return m.alliance_id; }
	const UInt32& GetEnemy_id() const { return m.enemy_id; }
	const String& GetSecMasterName() const { return m.secMasterName; }
	const UInt16& GetLevel() const { return m.level; }
	const UInt32& GetProsper() const { return m.prosper; }
	const UInt16& GetMax_prosper() const { return m.max_prosper; }
	const UInt32& GetFund() const { return m.fund; }
	const UInt16& GetTotalMember() const { return m.totalMember; }
	const UInt16& GetMaxMember() const { return m.maxMember; }
	const String& GetAim() const { return m.aim; }
	const String& GetRule() const { return m.rule; }
	const UInt32& GetGuildPvPScore() const { return m.guildPvPScore; }
	const UInt32& GetGuildPvPBid() const { return m.guildPvPBid; }
	const UInt8& GetGuildPvPGroup() const { return m.guildPvPGroup; }
	const String& GetGroupPvPVSGuild() const { return m.groupPvPVSGuild; }
	const UInt8& GetManorScale() const { return m.manorScale; }
	const UInt8& GetRoom() const { return m.room; }
	const UInt32& GetMaterial() const { return m.material; }
	const UInt8& GetGuard() const { return m.guard; }
	const UInt8& GetTrainCharExPow() const { return m.trainCharExPow; }
	const UInt8& GetTrainCharInPow() const { return m.trainCharInPow; }
	const UInt8& GetTrainPartnerExPow() const { return m.trainPartnerExPow; }
	const UInt8& GetTrainPartnerInPow() const { return m.trainPartnerInPow; }
	const UInt8& GetDrugLv() const { return m.drugLv; }
	const UInt8& GetCookLv() const { return m.cookLv; }
	const UInt8& GetWineLv() const { return m.wineLv; }
	const UInt8& GetHpLv() const { return m.hpLv; }
	const UInt8& GetSpLv() const { return m.spLv; }
	const UInt32& GetMaintainNum() const { return m.maintainNum; }
	const UInt32& GetLastMaintainDate() const { return m.lastMaintainDate; }
	const UInt8& GetAura() const { return m.aura; }
	const UInt32& GetOfflinetime() const { return m.offlinetime; }
	const UInt32& GetClearBusinessDate() const { return m.clearBusinessDate; }
	const Boolean& GetAutoManageBusiness() const { return m.autoManageBusiness; }
	virtual void OnUpdateId(const UInt32 &guild_uid) {};
	virtual void OnUpdateName(const String &name) {};
	virtual void OnUpdateCreator(const String &creator) {};
	virtual void OnUpdateMaster(const String &master) {};
	virtual void OnUpdateAlliance(const UInt32 &alliance_id) {};
	virtual void OnUpdateEnemy(const UInt32 &enemy_id) {};
	virtual void OnUpdateSecMaster(const String &secMasterName) {};
	virtual void OnUpdateLevel(const UInt16 &level) {};
	virtual void OnUpdateProsper(const UInt32 &prosper, const UInt16 &max_prosper) {};
	virtual void OnUpdateFund(const UInt32 &fund) {};
	virtual void OnUpdateMemberNum(const UInt16 &totalMember, const UInt16 &maxMember) {};
	virtual void OnUpdateAim(const String &aim) {};
	virtual void OnUpdateRule(const String &rule) {};
	virtual void OnUpdateGuildPvPScore(const UInt32 &guildPvPScore) {};
	virtual void OnUpdateGuildPvPBid(const UInt32 &guildPvPBid) {};
	virtual void OnUpdateGuildPvPGroup(const UInt8 &guildPvPGroup) {};
	virtual void OnUpdateGroupPvPVSGuild(const String &groupPvPVSGuild) {};
	virtual void OnUpdateManorScale(const UInt8 &manorScale) {};
	virtual void OnUpdateManorRoom(const UInt8 &room) {};
	virtual void OnUpdateManorMaterial(const UInt32 &material) {};
	virtual void OnUpdateManorGuard(const UInt8 &guard) {};
	virtual void OnUpdateManorTrainPlayerPow(const UInt8 &trainCharExPow, const UInt8 &trainCharInPow) {};
	virtual void OnUpdateManorTrainPartnerPow(const UInt8 &trainPartnerExPow, const UInt8 &trainPartnerInPow) {};
	virtual void OnUpdateManorRoomProperty(const UInt8 &drugLv, const UInt8 &cookLv, const UInt8 &wineLv, const UInt8 &hpLv, const UInt8 &spLv) {};
	virtual void OnUpdateManorMaintainNum(const UInt32 &maintainNum) {};
	virtual void OnUpdateLastMaintainDate(const UInt32 &lastMaintainDate) {};
	virtual void OnUpdateManorAura(const UInt8 &aura) {};
	virtual void OnUpdateOfflinetime(const UInt32 &offlinetime) {};
	virtual void OnUpdateClearBusinessDate(const UInt32 &clearBusinessDate) {};
	virtual void OnUpdateAutoManageBusiness(const Boolean &autoManageBusiness) {};
	void SetId(const UInt32 &guild_uid);
	void SetGuild_uid(const UInt32 &guild_uid);
	void SetName(const String &name);
	void SetCreator(const String &creator);
	void SetMaster(const String &master);
	void SetAlliance(const UInt32 &alliance_id);
	void SetAlliance_id(const UInt32 &alliance_id);
	void SetEnemy(const UInt32 &enemy_id);
	void SetEnemy_id(const UInt32 &enemy_id);
	void SetSecMaster(const String &secMasterName);
	void SetSecMasterName(const String &secMasterName);
	void SetLevel(const UInt16 &level);
	void SetProsper(const UInt32 &prosper, const UInt16 &max_prosper);
	void SetProsper(const UInt32 &prosper);
	void SetMax_prosper(const UInt16 &max_prosper);
	void SetFund(const UInt32 &fund);
	void SetMemberNum(const UInt16 &totalMember, const UInt16 &maxMember);
	void SetTotalMember(const UInt16 &totalMember);
	void SetMaxMember(const UInt16 &maxMember);
	void SetAim(const String &aim);
	void SetRule(const String &rule);
	void SetGuildPvPScore(const UInt32 &guildPvPScore);
	void SetGuildPvPBid(const UInt32 &guildPvPBid);
	void SetGuildPvPGroup(const UInt8 &guildPvPGroup);
	void SetGroupPvPVSGuild(const String &groupPvPVSGuild);
	void SetManorScale(const UInt8 &manorScale);
	void SetManorRoom(const UInt8 &room);
	void SetRoom(const UInt8 &room);
	void SetManorMaterial(const UInt32 &material);
	void SetMaterial(const UInt32 &material);
	void SetManorGuard(const UInt8 &guard);
	void SetGuard(const UInt8 &guard);
	void SetManorTrainPlayerPow(const UInt8 &trainCharExPow, const UInt8 &trainCharInPow);
	void SetTrainCharExPow(const UInt8 &trainCharExPow);
	void SetTrainCharInPow(const UInt8 &trainCharInPow);
	void SetManorTrainPartnerPow(const UInt8 &trainPartnerExPow, const UInt8 &trainPartnerInPow);
	void SetTrainPartnerExPow(const UInt8 &trainPartnerExPow);
	void SetTrainPartnerInPow(const UInt8 &trainPartnerInPow);
	void SetManorRoomProperty(const UInt8 &drugLv, const UInt8 &cookLv, const UInt8 &wineLv, const UInt8 &hpLv, const UInt8 &spLv);
	void SetDrugLv(const UInt8 &drugLv);
	void SetCookLv(const UInt8 &cookLv);
	void SetWineLv(const UInt8 &wineLv);
	void SetHpLv(const UInt8 &hpLv);
	void SetSpLv(const UInt8 &spLv);
	void SetManorMaintainNum(const UInt32 &maintainNum);
	void SetMaintainNum(const UInt32 &maintainNum);
	void SetLastMaintainDate(const UInt32 &lastMaintainDate);
	void SetManorAura(const UInt8 &aura);
	void SetAura(const UInt8 &aura);
	void SetOfflinetime(const UInt32 &offlinetime);
	void SetClearBusinessDate(const UInt32 &clearBusinessDate);
	void SetAutoManageBusiness(const Boolean &autoManageBusiness);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 guild_uid;
	//name
	String name;
	//creator
	String creator;
	//master
	String master;
	//alliance
	UInt32 alliance_id;
	//enemy
	UInt32 enemy_id;
	//secMaster
	String secMasterName;
	//level
	UInt16 level;
	//prosper
	UInt32 prosper;
	UInt16 max_prosper;
	//fund
	UInt32 fund;
	//memberNum
	UInt16 totalMember;
	UInt16 maxMember;
	//aim
	String aim;
	//rule
	String rule;
	//guildPvPScore
	UInt32 guildPvPScore;
	//guildPvPBid
	UInt32 guildPvPBid;
	//guildPvPGroup
	UInt8 guildPvPGroup;
	//groupPvPVSGuild
	String groupPvPVSGuild;
	//manorScale
	UInt8 manorScale;
	//manorRoom
	UInt8 room;
	//manorMaterial
	UInt32 material;
	//manorGuard
	UInt8 guard;
	//manorTrainPlayerPow
	UInt8 trainCharExPow;
	UInt8 trainCharInPow;
	//manorTrainPartnerPow
	UInt8 trainPartnerExPow;
	UInt8 trainPartnerInPow;
	//manorRoomProperty
	UInt8 drugLv;
	UInt8 cookLv;
	UInt8 wineLv;
	UInt8 hpLv;
	UInt8 spLv;
	//manorMaintainNum
	UInt32 maintainNum;
	//lastMaintainDate
	UInt32 lastMaintainDate;
	//manorAura
	UInt8 aura;
	//offlinetime
	UInt32 offlinetime;
	//clearBusinessDate
	UInt32 clearBusinessDate;
	//autoManageBusiness
	Boolean autoManageBusiness;
} m;

public:
	INLINE GuildData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult GuildData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<339936166>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1361462902)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(339936166)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_GUILDDATA_PROXY_H_
