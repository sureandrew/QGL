#pragma once
#ifndef _BATMOB_H_
#define _BATMOB_H_

#include "Proxy/Logic/BatActor.h"
#include "Proxy/Logic/BatMob_Proxy.h"

class CPartner;
class CSkill;

class CBatMob : public BatMob_Proxy
{
public:
	DEFINE_CLASS(BatMob_CLASSID);
	CBatMob() : m_partner(NULL) {}

	UInt16 GetSkillCount();
	CSkill* GetSkill(UInt16 i);
	CSkill* GetSkillById(UInt16 skill_id);
	void ShowDetail();
	void ShowSkills();

	virtual void OnUpdateInfo(const UInt32 &mob_id, const String &raname);
	virtual void OnUpdateOwner(const UInt32 &owner_id);
	virtual void OnUpdatePartner(const UInt32 &partner_id);

private:
	typedef BatMob_Proxy Parent;
	CPartner*	m_partner;
};

#endif //_BATMOB_H_
