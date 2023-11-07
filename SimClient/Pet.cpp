#include "Common.h"
#include "Common/common_binary.h"
#include "SimClient/Pet.h"
#include "SimClient/PetGroup.h"
#include "SimClient/Skill.h"
#include "SimClient/SkillGroup.h"
#include "SimClient/Character.h"
#include "SimClient/Global.h"

REGISTER_CLASSTYPE(CPet, Pet_CLASSID);

CPet::CPet()
{
	m_pSkillGrp = NULL;
	m_bGiven = false;
}

CPet::~CPet()
{
	//if (m_pSkillGrp && m_pSkillGrp->GetOwner_id() == GetPet_uid())
	//	DeleteNetGroup(m_pSkillGrp, BOOLEAN_TRUE);
	m_pSkillGrp = NULL;
}

RPCResult CPet::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{

	return RPC_RESULT_OK;
}
void CPet::SetSkillGroup(CSkillGroup *grp)
{
	if (grp == NULL)
		return;

	if (m_pSkillGrp && m_pSkillGrp != grp)
		gGlobal.m_pSimClient->m_pPetGroup->RemoveSkillGroup(GetPet_uid());
	m_pSkillGrp = grp;
	m_pSkillGrp->SetOwnerType(SKILL_OWNER_TYPE_PET);
}

RPCResult CPet::CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CPet::CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID)
{
	return RPC_RESULT_OK;
}
RPCResult CPet::CB_ChangePetName(LPCPEER pPeer, RPCContext &context, const bool &success)
{
	return RPC_RESULT_OK;
}