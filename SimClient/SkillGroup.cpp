//-- Common
#include "Common.h"
#include "Common/common_binary.h"
//-- Self
#include "SimClient/SkillGroup.h"
//-- Library
#include "SimClient/Skill.h"
#include "SimClient/SimClient.h"

REGISTER_CLASSTYPE(CSkillGroup, SkillGroup_CLASSID);

CSkillGroup::CSkillGroup()
{
}

CSkillGroup::~CSkillGroup()
{
}

void CSkillGroup::ShowSkills()
{
	CSkill* pSkill;
	for (UInt16 i = 0; i < GetCount(); i++)
	{
		pSkill = GetChild<CSkill>(i);
		if (pSkill)
			pSkill->ShowDetail();
	}
}

RPCResult CSkillGroup::CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp)
{
	CSkill* pSkill;
	String s;
	for (UInt16 i = 0; i < skillGrp->GetCount(); i++)
	{
		pSkill = skillGrp->GetChild<CSkill>(i);
		if (pSkill) {
			s.AppendFormat(_T("%d, "), pSkill->GetSkill_id());
			Add(pSkill);
		}
	}

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CSkillGroup CB_AddSkills: skill_ids %scount %d"), 
		s.c_str(), skillGrp->GetCount());

	DeleteNetGroup(skillGrp);

	return RPC_RESULT_OK;
}

RPCResult CSkillGroup::CB_RemoveSkills(LPCPEER pPeer, RPCContext &context, const UInt16Vector &skill_ids)
{
	CSkill *pSkill;
	String s;

	for (UInt16Vector::const_iterator it = skill_ids.begin(); it != skill_ids.end(); ++it)
	{
		for (UInt16 i = 0; i < GetCount(); i++)
		{
			pSkill = GetChild<CSkill>(i);
			if (pSkill && pSkill->GetSkill_id() == *it) {
				s.AppendFormat(_T("%d, "), pSkill->GetSkill_id());
				Remove(pSkill);
				DeleteNetObject(pSkill);
				break;
			}
		}
	}

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CSkillGroup CB_RemoveSkills: skill_ids %scount %d"), 
		s.c_str(), skill_ids.size());

	return RPC_RESULT_OK;
}