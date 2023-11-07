//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/DataCode.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Stub/Logic/Skill.h"
//-- Library
#include "Proxy/Data/SkillData.h"
#include "Proxy/Service/ObjectService.h"
#include "Stub/Logic/SkillGroup.h"
#include "MapService/Global.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResSkill.h"
#include "Resource/ResMob.h"

PROTO_LuaPush(CBatActor)
REGISTER_CLASSTYPE(CSkill, Skill_CLASSID);

CSkill::CSkill()
{
	m_data = NULL;
	m_pSkillGroup = NULL;
}

void CSkill::SetData(CSkillData *pData)
{
	if (pData->IsUpdated(CSkillData::infoMask)) {
		SetInfo(pData->GetSkill_id());
		m_data = gGlobal.m_resource->GetSkillData(GetSkill_id());
		if (m_data == NULL)
			TRACE_WARNDTL_1(GLOBAL_LOGGER, 
				_T("CSkill Warning: cannot find skill_id %d in resource"), GetSkill_id());
	}

	if (pData->IsUpdated(CSkillData::owner_infoMask))
		SetOwner_info(pData->GetOwner_id(), pData->GetOwner_type());

	if (pData->IsUpdated(CSkillData::LevelMask))
		SetLevel(pData->GetSkill_level());

	if (pData->IsUpdated(CSkillData::CoolDownMask))
		SetCoolDown(pData->GetCool_down());

	ClearAllMask();
}

void CSkill::WriteData(CSkillData *pData)
{
	TRACE_ENSURE(pData);

	pData->ClearUpdateMask();

	if (IsModified(LevelMask))
		pData->SetLevel(GetSkill_level());

	if (IsModified(CoolDownMask))
		pData->SetCoolDown(GetCool_down());

	if (pData->IsUpdated()) {
		pData->SetInfo(GetSkill_id());
		pData->SetOwner_info(GetOwner_id(), GetOwner_type());
		pData->SetSendMask(pData->GetUpdateMask());
	}
}

Boolean CSkill::NeedUpdate()
{
	if (m_data == NULL)
		return BOOLEAN_FALSE;
	RPCMask mask;

	mask.MaskBits(owner_infoMask, LevelMask, CoolDownMask, PartnerSkillTypeMask, -1);

	return IsModified(mask) && m_data->reqBaseSkill == 0;
}

void CSkill::PreSend()
{	
	RPCMask mask;
	mask.MaskBits(CSkill::AllMask, -1);
	SetSendMask(mask);
}
