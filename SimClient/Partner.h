#pragma once
#ifndef _PARTNER_H_
#define _PARTNER_H_

#include "Proxy/Logic/Partner_Proxy.h"

class CSkill;
class CSkillGroup;

class CPartner : public Partner_Proxy
{
public:
	DEFINE_CLASS(Partner_CLASSID);
	CPartner() : m_pSkillGroup(NULL) {};
	
	enum StateEnum
	{
		STATE_STANDBY = 0,
		STATE_FIGHT = 1,
		STATE_REST = 2,
	};

	virtual ~CPartner();
	void SetSkillGroup(CSkillGroup *grp);
	Boolean CanCall();
	UInt16 GetSkillCount();
	CSkill* GetSkill(UInt16 i);
	CSkill* GetSkillById(UInt16 skill_id);
	void ShowDetail();
	void ShowSkills();

	virtual RPCResult CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_ChangePartnerName(LPCPEER pPeer, RPCContext &context, const bool &success);
	virtual RPCResult CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_UpdateSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID);
	virtual RPCResult CB_FalseJoinBattle(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType);

// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &partner_id, const UInt32 &mob_id);
	virtual void OnUpdateOwner(const UInt32 &owner_id);
	virtual void OnUpdateInfo(const String &raname);
	virtual void OnUpdateLevel(const UInt16 &level);
	virtual void OnUpdateNextLvExp(const UInt32 &nextLvExp);
	virtual void OnUpdateExp(const UInt32 &exp);
	virtual void OnUpdateAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI);
	virtual void OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD);
	virtual void OnUpdateAttrPoint(const UInt16 &attrPoint);
	virtual void OnUpdateHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP);
	virtual void OnUpdateSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP);
	virtual void OnUpdateGrow(const float &grow);
	virtual void OnUpdateFavor(const float &favor);
	virtual void OnUpdateHealth(const UInt16 &health);
	virtual void OnUpdateState(const UInt8 &state);
	virtual void OnUpdateBuff(const String &buff);

	CSkillGroup *m_pSkillGroup;

	// partner data member used in SimClient.cpp
	int m_STA;
	int m_STR;
	int m_CON;
	int m_SPR;
	int m_AGI;
	int attriSign;	


private:
	typedef Partner_Proxy Parent;

};

#endif //_PARTNER_H_
