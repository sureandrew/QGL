#include "Common.h"
#include "SimClient/PetGroup.h"
#include "SimClient/Pet.h"
#include "Common/common_binary.h"
#include "SimClient/Global.h"	


#include "SimClient/Skill.h"
#include "SimClient/SkillGroup.h"

REGISTER_CLASSTYPE(CPetGroup, PetGroup_CLASSID);

CPet* CPetGroup::GetPetByIndex(int index)
{
	CPet* pPet = NULL;

	if (index >= 0 && index < GetCount())
		pPet = GetChild<CPet>(index);
	
	return pPet;
}

CPet* CPetGroup::GetPet(UInt32 pet_uid)
{
	CPet* pet;
	for (int i = 0; i < GetCount(); ++i)
	{
		pet = GetChild<CPet>(i);
		if (pet)
		{
			if (pet->GetPet_uid() == pet_uid)
				return pet;
		}
	}
	return NULL;
}

void CPetGroup::ShowAllPet()
{
	if (GetCount() == 0)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("No pet exist"));
		return;
	}

	CPet* pet;
	for (int i = 0; i < GetCount(); ++i)
	{
		pet = GetChild<CPet>(i);
		if (pet)
		{
			TRACE_INFOL_5(GLOBAL_LOGGER, 
				_T("Pet %d: pet_id %d, pet_uid %d, level %d, state %d"), 
				i, pet->GetPet_id(), pet->GetPet_uid(), pet->GetLevel(), pet->GetState());
		}
	}
}

RPCResult CPetGroup::CB_AddPets(LPCPEER pPeer, RPCContext &context, NetGroup* petGrp, 
								NetGroup* skillGroup)
{
	if ((!skillGroup) || (!petGrp))
		return RPC_RESULT_OK; 

	for (UInt i = 0; i < petGrp->GetCount() ; i++)
	{
		CSkillGroup* pSkillGroup =  skillGroup->GetChild<CSkillGroup>(i);
		CPet * pPet = petGrp->GetChild<CPet>(i);
		if (pPet)
		{
			SetSkillGroup(pPet->GetPet_uid(), pSkillGroup);
			pPet->SetSkillGroup(pSkillGroup);
			gGlobal.m_pSimClient->m_pPetGroup->Add(pPet);
		}
	}
	DeleteNetGroup(petGrp);
	DeleteNetGroup(skillGroup);

	return RPC_RESULT_OK;
}

RPCResult CPetGroup::CB_RemovePets(LPCPEER pPeer, RPCContext &context, const UInt32Vector &pet_ids)
{
	if (gGlobal.m_pSimClient->m_pPetGroup == NULL)
		return RPC_RESULT_OK;
	
	for (Index i = C_INDEX(0); i < pet_ids.size() ; ++i)
	{
		for (Index j = C_INDEX(0); j < gGlobal.m_pSimClient->m_pPetGroup->GetCount(); ++j)
		{
			CPet * pPet = gGlobal.m_pSimClient->m_pPetGroup->GetChild<CPet>(j);
			if (pPet && pPet->GetPet_uid() == pet_ids[i])
			{
				RemoveSkillGroup(pPet->GetPet_uid());
				gGlobal.m_pSimClient->m_pPetGroup->Remove(pPet);
				DeleteNetObject(pPet);
			}
		}
	}


	return RPC_RESULT_OK;
}

Int32 CPetGroup::GetPetIndex(UInt32 pet_uid)
{
	CPet* pet = NULL;
	for (int i = 0; i < GetCount(); ++i)
	{
		pet = GetChild<CPet>(i);
		if (pet && pet->GetPet_uid() == pet_uid)
				return i;	
	}
	return -1;
}

void CPetGroup::SetSkillGroup(UInt32 pet_uid, CSkillGroup* skillGroup)
{
	if (skillGroup)
	{
		SkillMap::iterator itr = m_petSkillMap.find(pet_uid);
		if (itr != m_petSkillMap.end())
		{
			if (skillGroup != itr->second)
			{
				DeleteNetGroup(itr->second);
				itr->second = skillGroup;
			}
		}
		else
			m_petSkillMap.insert(std::make_pair(pet_uid, skillGroup));
	}
}

CSkillGroup* CPetGroup::GetSkillGroup(UInt32 pet_uid)
{
	SkillMap::iterator itr = m_petSkillMap.find(pet_uid);
	if (itr != m_petSkillMap.end())
		return itr->second;
	return NULL;
}

bool CPetGroup::IsExistSkillGroup(NetGroup* skillGroups)
{
	if (skillGroups)
	{
		for (UInt16 i = 0; i < skillGroups->GetCount(); i++) 
		{
			CSkillGroup * grp = skillGroups->GetChild<CSkillGroup>(i);
			if (grp)
			{
				SkillMap::iterator itr = m_petSkillMap.find(grp->GetOwner_id());
				if (itr != m_petSkillMap.end())
					return true;
			}
		}
	}
	return false;
}

void CPetGroup::RemoveSkillGroup(UInt32 pet_uid)
{
	SkillMap::iterator itr = m_petSkillMap.find(pet_uid);
	if (itr != m_petSkillMap.end())
	{
		DeleteNetGroup(itr->second, BOOLEAN_TRUE);
		m_petSkillMap.erase(itr);
	}
}

RPCResult CPetGroup::CB_UpdateAllPets(LPCPEER pPeer, RPCContext &context, NetGroup* petlGrp)
{
	DeleteNetGroup(petlGrp);
	return RPC_RESULT_OK;
}

RPCResult CPetGroup::CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp)
{
	CPet* pPet = NULL;
	if (skillGrp == NULL)
		return RPC_RESULT_OK;	

	for (UInt8 j = 0; j < skillGrp->GetCount(); ++j)
	{
		CSkill* skill = skillGrp->GetChild<CSkill>(j);
		if (skill && skill->GetOwner_type() == SKILL_OWNER_TYPE_PET)
		{
			pPet = GetPet(skill->GetOwner_id());
			if (pPet && pPet->m_pSkillGrp)
				pPet->m_pSkillGrp->Add(skill);
		}
	}
	DeleteNetGroup(skillGrp);

	return RPC_RESULT_OK;
}

CPetGroup::CPetGroup()
{
	m_petSkillMap.clear();
}

CPetGroup::~CPetGroup()
{
	for (SkillMap::iterator itr = m_petSkillMap.begin(); itr != m_petSkillMap.end(); ++itr)
	{
		DeleteNetGroup(itr->second, BOOLEAN_TRUE);
	}
	m_petSkillMap.clear();
}

RPCResult CPetGroup::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	return RPC_RESULT_OK;
}

RPCResult CPetGroup::CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex)
{
	return RPC_RESULT_OK;
}