#pragma once
#ifndef _SKILL_H_
#define _SKILL_H_

#include "Stub/Logic/Skill_Stub.h"

struct SkillData;
class CSkillData;
class CCharacter;
class CPartner;
class CSkillGroup;
class CBatActor;

class CSkill : public Skill_Stub
{
public:
	DEFINE_CLASS(Skill_CLASSID);

	CSkill();
	void SetData(CSkillData *pData);				// set skill by CSkillData
	void WriteData(CSkillData *pData);			// write skill to CSkillData
	void PreSend();			// prepare to send
	Boolean NeedUpdate();						// need update to database?
	//Boolean CanUse(CBatActor* pUser, CBatActor* pTarget, String &errMsg);

	INLINE static CSkill* NewInstance();

	const SkillData *m_data;
	CSkillGroup*	m_pSkillGroup;

private:
	typedef Skill_Stub Parent;
};

INLINE CSkill* CSkill::NewInstance()
{
	return CreateNetObject<CSkill>();
}


#endif //_SKILL_H_
