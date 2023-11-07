#pragma once
#ifndef _SKILLGROUP_H_
#define _SKILLGROUP_H_

#include "Proxy/Logic/SkillGroup_Proxy.h"

class CSkill;

class CSkillGroup : public SkillGroup_Proxy
{
public:
	DEFINE_CLASS(SkillGroup_CLASSID);
	CSkillGroup();
	virtual ~CSkillGroup();
	virtual RPCResult CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp);
	virtual RPCResult CB_RemoveSkills(LPCPEER pPeer, RPCContext &context, const UInt16Vector &skill_ids);
	
	INLINE void SetOwnerType(UInt8 type){ m_iOwnerType = type; }
	CSkill* GetSkillById(UInt16 skill_id);

private:
	typedef SkillGroup_Proxy Parent;
	UInt8 m_iOwnerType;
// OnUpdate can be implemented selectively
};

#endif //_SKILLGROUP_H_
