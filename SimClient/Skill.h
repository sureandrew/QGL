#pragma once
#ifndef _SKILL_H_
#define _SKILL_H_

#include "Proxy/Logic/Skill_Proxy.h"

struct SkillData;

class CSkill : public Skill_Proxy
{
private:
	typedef Skill_Proxy Parent;
	const SkillData* m_pData;

public:
	DEFINE_CLASS(Skill_CLASSID);
// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo(const UInt16 &skill_id);
	virtual void OnUpdateOwner_info(const UInt32 &owner_id, const BYTE &owner_type);
	virtual void OnUpdateLevel(const UInt16 &skill_level);
	virtual void OnUpdateCoolDown(const UInt32 &cool_down);

	CSkill();
	UInt8 GetType();
	UInt8 GetTargetType();
	Boolean CanUse(Boolean inMap);
	void ShowDetail();
};

#endif //_SKILL_H_
