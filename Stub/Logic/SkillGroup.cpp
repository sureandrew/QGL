//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Logic/SkillGroup.h"
//-- Library
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/Partner.h"
#include "Stub/Logic/Pet.h"
#include "Proxy/Service/ObjectService.h"
#include "MapService/Global.h"
#include "Proxy/Data/SkillData.h"

REGISTER_CLASSTYPE(CSkillGroup, SkillGroup_CLASSID);

CSkillGroup::CSkillGroup()
{
	m_pCharOwner = NULL;
	m_pPartnerOwner = NULL;
}

CSkillGroup::~CSkillGroup()
{
	if (m_pCharOwner)
		m_pCharOwner->m_pSkillGroup = NULL;
	if (m_pPartnerOwner)
		m_pPartnerOwner->m_pSkillGroup = NULL;
}

Boolean CSkillGroup::Add(NetObject *obj)
{
	if (obj == NULL || obj->GetClassID() != Skill_CLASSID)
		return BOOLEAN_FALSE;

	if (Parent::Add(obj)) {
		CSkill* pSkill = CastNetObject<CSkill>(obj);
		if (pSkill) {
			pSkill->m_pSkillGroup = this;
			m_skillMap[pSkill->GetSkill_id()] = pSkill;
			return BOOLEAN_TRUE;
		}
	}
	
	return BOOLEAN_FALSE;
}

Boolean CSkillGroup::Remove(NetObject *obj)
{
	if (obj == NULL || obj->GetClassID() != Skill_CLASSID)
		return BOOLEAN_FALSE;

	if (Parent::Remove(obj)) {
		CSkill* pSkill = CastNetObject<CSkill>(obj);
		if (pSkill) {
			pSkill->m_pSkillGroup = NULL;
			m_skillMap.erase(pSkill->GetSkill_id());
			return BOOLEAN_TRUE;
		}
	}
	
	return BOOLEAN_FALSE;
}

CSkill* CSkillGroup::GetSkill(UInt16 skillId)
{
	SkillMap::iterator it = m_skillMap.find(skillId);
	if (it != m_skillMap.end())
		return it->second;
	else
		return NULL;
}

void CSkillGroup::PreSend()
{
	RPCMask mask;

	mask.MaskBit(CSkillGroup::AllMask);
	SetSendMask(mask);

	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		CSkill * pSkill = GetChild<CSkill>(i);
		if (pSkill)
			pSkill->PreSend();
	}
}

void CSkillGroup::SendAll()
{
	NetGroup skillGrp;
	for (UInt i = 0; i < GetCount(); i ++)
	{
		CSkill *pSkill = GetChild<CSkill>(i);
		if (pSkill)
		{
			pSkill->PreSend();
			skillGrp.Add(pSkill);
		}
	}

	if (skillGrp.GetCount() > 0)
		CB_AddSkills(GetOwnerPeer(), GetOwnerContext(), &skillGrp);
}

LPCPEER CSkillGroup::GetOwnerPeer()
{
	if (m_pCharOwner)
		return m_pCharOwner->GetPeer();
	else if (m_pPartnerOwner)
		return m_pPartnerOwner->GetOwnerPeer();
	else
		return NULL;
}

RPCContext &CSkillGroup::GetOwnerContext()
{
	static RPCContext dummyContext;
	if (m_pCharOwner)
		return m_pCharOwner->GetContext();
	else if (m_pPartnerOwner)
		return m_pPartnerOwner->GetOwnerContext();
	else
		return dummyContext;
}

void CSkillGroup::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_UPDATE_OBJECT:				// update all pet to object service
		{
			RPCContext context;
			if (m_pCharOwner)
				context = m_pCharOwner->GetContext();
			else if (m_pPartnerOwner)
				context = m_pPartnerOwner->GetOwnerContext();
			else if (m_pPetOwner)
				context = m_pPetOwner->GetOwnerContext();
			else {
				TRACE_ERRORDT(GLOBAL_LOGGER, _F("EVT_UPDATE_OBJECT Error: no owner belong to"));
				break;
			}

			PROCEDURE_DO_METHOD(CSkillGroup, UpdateSkills, this, context)
		}
		break;
	}
}
void CSkillGroup::RaiseUpdate()
{
	RaiseUniqEventID(EVT_UPDATE_OBJECT);
}

void CSkillGroup::CancelUpdate()
{
	CancelUniqEvent(EVT_UPDATE_OBJECT);
}

RPCResult CSkillGroup::UpdateSkills(RPCContext &context)
{
	PROCEDURE_START_METHOD(CSkillGroup, UpdateSkills, this, context)

	if (m_pCharOwner == NULL && m_pPartnerOwner == NULL && m_pPetOwner == NULL)
	{
		TRACE_ERRORDT(GLOBAL_LOGGER, _F("UpdateSkills Error: no owner belong to"));
		PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
	}

	UInt32 char_id = 0;
	if (m_pCharOwner)
		char_id = m_pCharOwner->GetChar_id();
	else if (m_pPartnerOwner)
		char_id = m_pPartnerOwner->GetOwner_id();
	else if (m_pPetOwner)
		char_id = m_pPetOwner->GetOwner_id();

	NetGroup skillGrp;
	for (UInt i = 0; i < GetCount(); i ++)
	{
		CSkill *pSkill = GetChild<CSkill>(i);
		if (pSkill == NULL)
			continue;
		if (pSkill->NeedUpdate())
		{
			CSkillData* skillData = CreateNetObject<CSkillData>(); 
			pSkill->WriteData(skillData);
			skillGrp.Add(skillData);
			pSkill->ClearModifyMask();
		}
		if (pSkill->IsUpdated()) {
			pSkill->SendAttr(GetOwnerPeer(), GetOwnerContext(), pSkill->GetUpdateMask());
			pSkill->ClearUpdateMask();
		}
	}

	if (skillGrp.GetCount() > 0)
	{
		ObjectService_Proxy::UpdateSkillList(gGlobal.pObjectService->pPeer, context,
			GetOwner_id(), GetOwner_type(), &skillGrp);
		DeleteNetChilds(&skillGrp);
	}
	else
	{
		// no need update
		PROCEDURE_RETURN
	}

	PROCEDURE_WAIT(1, ObjectService, CB_UpdateSkillList)

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
				_T("CSkill UpdateToObject Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH


	return RPC_RESULT_OK;
}

void CSkillGroup::RemoveAllSkill()
{
	UInt16 count = GetCount();
	for (UInt i = 0; i < count; i ++)
	{
		CSkill *pSkill = GetChild<CSkill>(i);
		if (pSkill)
		{
			if(Remove(pSkill))
			{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("RemoveAllSkill pSkillID =  %d"),
					pSkill->GetSkill_id());
			}
			else
				TRACE_ERRORDTL(GLOBAL_LOGGER, _F("RemoveAllSkill Erro =  %d"));
		}
	}
}