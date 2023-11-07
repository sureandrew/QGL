#pragma once
#ifndef _GUILD_H_
#define _GUILD_H_

#include "Proxy/Logic/Guild_Proxy.h"

class CGuild : public Guild_Proxy
{
private:
	typedef Guild_Proxy Parent;

public:
	DEFINE_CLASS(Guild_CLASSID);
// OnUpdate can be implemented selectively
	virtual RPCResult CB_AddMember(LPCPEER pPeer, RPCContext &context, NetObject* pMember);
	virtual RPCResult CB_RemoveMember(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &reason);
	virtual RPCResult CB_RemoveApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CB_AddApplicant(LPCPEER pPeer, RPCContext &context, NetObject* pObj);
	virtual void OnUpdateAlliance(const UInt32 &alliance_id);
	virtual void OnUpdateEnemy(const UInt32 &enemy_id);
	virtual void OnUpdateSecMaster(const String &secMasterName);
	virtual void OnUpdateLevel(const UInt16 &level);
	virtual void OnUpdateProsper(const UInt32 &prosper, const UInt16 &max_prosper);
	virtual void OnUpdateFund(const UInt32 &fund);
	virtual void OnUpdateMemberNum(const UInt16 &numOfMember, const UInt16 &numOfMaxMember);
	virtual void OnUpdateAim(const String &Aim);
	virtual void OnUpdateRule(const String &Rule);
	virtual void OnUpdateGuildPvPScore(const UInt32 &guildPvPScore);
	virtual void OnUpdateGuildPvPBid(const UInt32 &guildPvPBid);
	virtual void OnUpdateGuildPvPGroup(const UInt8 &guildPvPGroup);
	virtual void OnUpdateGroupPvPVSGuild(const String &groupPvPVSGuild);
	////// manor property
	virtual void OnUpdateManorScale(const UInt8 &manorScale);
	virtual void OnUpdateManorRoom(const UInt8 &room);
	virtual void OnUpdateManorMaterial(const UInt32 &material);
	virtual void OnUpdateManorGuard(const UInt8 &guard);
	virtual void OnUpdateManorTrainPlayerPow(const UInt8 &trainCharExPow, const UInt8 &trainCharInPow);
	virtual void OnUpdateManorTrainPartnerPow(const UInt8 &trainPartnerExPow, const UInt8 &trainPartnerInPow);
	virtual void OnUpdateManorRoomProperty(const UInt8 &drugLv, const UInt8 &cookLv, const UInt8 &wineLv, const UInt8 &hpLv, const UInt8 &spLv);
	virtual void OnUpdateManorMaintainNum(const UInt32 &maintainNum);
	virtual void OnUpdateManorAura(const UInt8 &aura);
	virtual void OnUpdateFailMantainPayNum(const UInt32 &failPayNum);

	void ChangeFund(Int32 change, UInt8 source = 5);
	void ChangeGuildPvPBid(Int32 change);
	void ChangeProsper(Int32 change, UInt8 source = 2);
	void ChangeMaterial(Int32 change);
	void ChangeRoom(Int8 change);
	void SetAuraValue(Int8 change);
	void ChangeTrainCharEx(Int8 change);
	void ChangeTrainCharIn(Int8 change);
	void ChangeTrainPartnerEx(Int8 change);
	void ChangeTrainPartnerIn(Int8 change);
	void ChangeDrug(Int8 change);
	void ChangeCook(Int8 change);
	void ChangeWine(Int8 change);
	void ChangeManorHP(Int8 change);	
	void ChangeManorSP(Int8 change);
	void SetNewManorScale(UInt8 scale);	

	void ChangeAutoManageBusiness(Boolean bOpen);

};

#endif //_GUILD_H_
