//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Skill.h"
//-- Library
#include "QGL/Global.h"
#include "QGL/MenuSkillProcess.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResSkill.h"

REGISTER_CLASSTYPE(CSkill, Skill_CLASSID);

CSkill::CSkill()
{
	m_pData = NULL;
}

UInt8 CSkill::GetType()
{
	return (m_pData ? m_pData->type : 0);
}

UInt8 CSkill::GetTargetType()
{
	return (m_pData ? m_pData->targetType : 0);
}

Boolean CSkill::CanUse(Boolean inMap)
{
	if (m_pData) {
		return (((m_pData->battleorField == FIELD_ANY) || 
			(inMap && m_pData->battleorField == FIELD_MAP) ||
			(!inMap && m_pData->battleorField == FIELD_BATTLE)) && m_pData->active == 0);
	} else 
		return BOOLEAN_FALSE;
}

void CSkill::OnUpdateInfo(const UInt16 &skill_id)
{
	if (m_pData == NULL)
		m_pData = gGlobal.m_resource->GetSkillData(skill_id);
}

void CSkill::OnUpdateOwner_info(const UInt32 &owner_id, const BYTE &owner_type)
{
}

void CSkill::OnUpdateLevel(const UInt16 &skill_level)
{
	gGlobal.g_pSkillProcess->UpdateSkillData(this);
}

void CSkill::OnUpdateCoolDown(const UInt32 &cool_down)
{
}