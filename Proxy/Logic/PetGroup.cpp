#include "Common.h"
#include "Proxy/Logic/PetGroup.h"
#include "Proxy/Logic/Pet.h"

#ifdef QGL
#include "QGL/Global.h"
#include "QGL/MenuPetProcess.h"
#include "QGL/MenuItemProcess.h"
#include "QGL/MenuHawkerProcess.h"
#include "QGL/MenuHelpProcess.h"
#include "QGL/MenuWareHouseProcess.h"
#include "Proxy/Logic/Skill.h"
#include "Proxy/Logic/SkillGroup.h"
#include "Common/CallLuaFunc.h"
#endif // QGL
REGISTER_CLASSTYPE(CPetGroup, PetGroup_CLASSID);

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

void CPetGroup::OnUpdateMaster(const UInt32 &master_id)
{
}

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
		if (pet->GetPet_uid() == pet_uid)
			return pet;
	}
	return NULL;
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

RPCResult CPetGroup::CB_AddPets(LPCPEER pPeer, RPCContext &context, NetGroup* petGrp, NetGroup* skillGroup)
{
#ifdef QGL
	for (UInt i = 0; i < petGrp->GetCount() ; i++)
	{
		CSkillGroup* pSkillGroup =  skillGroup->GetChild<CSkillGroup>(i);
		CPet * pPet = petGrp->GetChild<CPet>(i);
		if (pPet)
		{
			SetSkillGroup(pPet->GetPet_uid(), pSkillGroup);
			pPet->SetSkillGroup(pSkillGroup);
			Add(pPet);
		}
	}

	if (petGrp->GetCount() > 0)
		gGlobal.g_pMenuHelpProcess->HandleEvent(HELP_TIPS_FIRST_GETPET);
	if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pItemProcess->UpdateItemPet(0);
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPet(0);
	gGlobal.g_pMenuPetProcess->RefreshMenu();
#endif // QGL
	return RPC_RESULT_OK;
}

CSkillGroup* CPetGroup::GetSkillGroup(UInt32 pet_uid)
{
	SkillMap::iterator itr = m_petSkillMap.find(pet_uid);
	if (itr != m_petSkillMap.end())
		return itr->second;
	return NULL;
}

void CPetGroup::RemoveSkillGroup(UInt32 pet_uid, bool needDelete)
{
	SkillMap::iterator itr = m_petSkillMap.find(pet_uid);
	if (itr != m_petSkillMap.end())
	{
		if (needDelete)
			DeleteNetGroup(itr->second, BOOLEAN_TRUE);
		m_petSkillMap.erase(itr);
	}
}

RPCResult CPetGroup::CB_RemovePets(LPCPEER pPeer, RPCContext &context, const UInt32Vector &pet_ids)
{
	#ifdef QGL
	for (UInt i = 0; i < pet_ids.size() ; i++)
	{
		for (UInt j = 0; j < gGlobal.g_PetLst->GetCount(); j++)
		{
			CPet * pPet =  gGlobal.g_PetLst->GetChild<CPet>(j);
			if (pPet && pPet->GetPet_uid() == pet_ids[i])
			{
				if (pPet->GetInWareHouse() == 0)
					Remove(pPet);
				else
					gGlobal.g_pWareHouseProcess->RemovePet(pPet);

				RemoveSkillGroup(pPet->GetPet_uid());
				DeleteNetObject(pPet);
			}
		}
	}
	if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pItemProcess->UpdateItemPet(0);
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPet(0);
	gGlobal.g_pMenuPetProcess->RefreshMenu();
#endif // QGL
	return RPC_RESULT_OK;
}

Int32 CPetGroup::GetPetIndex(UInt32 pet_uid)
{
	CPet* pet = NULL;
	for (Index i = C_INDEX(0); i < GetCount(); ++i)
	{
		pet = GetChild<CPet>(i);
		if (pet && pet->GetPet_uid() == pet_uid)
			return i;
	}
	return -1;
}

RPCResult CPetGroup::CB_UpdateAllPets(LPCPEER pPeer, RPCContext &context, NetGroup* petlGrp)
{
	#ifdef QGL
	for (Index i = C_INDEX(0); i < petlGrp->GetCount(); ++i)
	{
		CPet * pet = petlGrp->GetChild<CPet>(i);
		if (pet && pet->IsUpdated(CPet::inWareHouseMask))
		{
			if (pet->GetInWareHouse() == 0 && GetPet(pet->GetPet_uid()) == NULL)
			{
				gGlobal.g_pWareHouseProcess->RemovePet(pet);
				Add(pet);
				SetSkillGroup(pet->GetPet_uid(), pet->m_pSkillGrp);
				pet->SetGiven(false);
			}
			else if (pet->GetInWareHouse() == 1 && gGlobal.g_pWareHouseProcess->GetPet(pet->GetPet_uid()) == NULL)
			{
				Remove(pet);
				RemoveSkillGroup(pet->GetPet_uid(), false);
				gGlobal.g_pWareHouseProcess->AddPet(pet);
				pet->SetGiven(false);
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SelectWarehousePet", String());
			}
			gGlobal.g_pWareHouseProcess->UpdateWareHouse();
		}	
	}
	gGlobal.g_pMenuPetProcess->RefreshMenu();

	if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pItemProcess->UpdateItemPet(0);
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPet(0);
	DeleteNetGroup(petlGrp);
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult CPetGroup::CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp)
{
	#ifdef QGL
	CPet* pPet = NULL;

	for (Index j = C_INDEX(0); j < skillGrp->GetCount(); ++j)
	{
		CSkill* skill = skillGrp->GetChild<CSkill>(j);
		if (skill && skill->GetOwner_type() == SKILL_OWNER_TYPE_PET)
		{
			pPet = GetPet(skill->GetOwner_id());
			if (pPet && pPet->m_pSkillGrp)
				pPet->m_pSkillGrp->Add(skill);
		}
	}
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult CPetGroup::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPetGroup CB_Error: errcode %d: %s"), errCode, errMsg.c_str());

#ifdef QGL
	//StringA textA(errMsg.c_str());
	String text(gGlobal.GetStringTable()->Get(errMsg.c_str()));
	String empty;
	if (!text.Equals(empty.c_str()))
		gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
		
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult CPetGroup::CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex)
{
	if (gGlobal.g_pWareHouseProcess)
	{
		gGlobal.g_pWareHouseProcess->ShowResult(processIndex, msgIndex);

		if (msgIndex != WAREHOUSE_SUCCESS)
			gGlobal.g_pWareHouseProcess->UpdateWareHouse();		
	}
	return RPC_RESULT_OK;
}