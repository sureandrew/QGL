///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GUILD_STUB_H_
#define _RPC_GUILD_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Guild_enum.h"

class Guild_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
		failMantainPayNumMask	= Parent::NextFreeMask + 28,
		clearBusinessDateMask	= Parent::NextFreeMask + 29,
		autoManageBusinessMask	= Parent::NextFreeMask + 30,
		NextFreeMask	= Parent::NextFreeMask + 31
	};
	virtual RPCResult CB_AddMember(LPCPEER pPeer, RPCContext &context, NetObject* pMember);
	virtual RPCResult CB_RemoveMember(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &reason);
	virtual RPCResult CB_AddApplicant(LPCPEER pPeer, RPCContext &context, NetObject* pObj);
	virtual RPCResult CB_RemoveApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
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
	const UInt32& GetFailPayNum() const { return m.failPayNum; }
	const UInt32& GetClearBusinessDate() const { return m.clearBusinessDate; }
	const Boolean& GetAutoManageBusiness() const { return m.autoManageBusiness; }
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
	void SetFailMantainPayNum(const UInt32 &failPayNum);
	void SetFailPayNum(const UInt32 &failPayNum);
	void SetClearBusinessDate(const UInt32 &clearBusinessDate);
	void SetAutoManageBusiness(const Boolean &autoManageBusiness);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	//failMantainPayNum
	UInt32 failPayNum;
	//clearBusinessDate
	UInt32 clearBusinessDate;
	//autoManageBusiness
	Boolean autoManageBusiness;
} m;

public:
	INLINE Guild_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_GUILD_STUB_H_
