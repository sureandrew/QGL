#include "Common.h"

#include "Common/Procedure.h"
#include "Stub/Logic/PetGroup.h"
#include "Stub/Logic/SkillGroup.h"
#include "Stub/Logic/Pet.h"
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/Character.h"
//#include "Stub/Logic/QuestSystem.h"
#include "Proxy/Data/PetData.h"
#include "Proxy/Service/ObjectService.h"
#include "Proxy/Data/SkillData.h"
#include "Stub/Logic/ScriptSystem.h"

#include "MapService/Global.h"
#include "MapService/Configure.h"

REGISTER_CLASSTYPE(CPetGroup, PetGroup_CLASSID);
#define USE_NOTIME_PET_CHANGE_VP_TIME (6*60*1000)//6 min
#define USE_NOTIME_PET_CHANGE_FAVOR_TIME (1*60*1000)// 1min
#define	NOT_USE_PET 3

CPetGroup::CPetGroup()
{
	m_pCharOwner = NULL;
	m_nowUsePetType = NOT_USE_PET;
	m_pTwoUsePet = NULL;
}

CPetGroup::~CPetGroup()
{
	for (Index i = C_INDEX(0); i < m_delGroup.GetCount(); ++i)
	{
		CPet* pet = m_delGroup.GetChild<CPet>(i);
		if (pet)
			DeleteNetObject(pet);
	}
	for (Index i = C_INDEX(0); i < m_newPets.GetCount(); ++i)
	{
		CPet* pet = m_newPets.GetChild<CPet>(i);
		if (pet)
			DeleteNetObject(pet);
	}
	ClearWareHouse();
	DeleteNetChilds(this);
}

CPet* CPetGroup::GetPet(UInt32 pet_uid)
{
	CPet* pet;
	for (int i = 0; i < GetCount(); ++i)
	{
		pet = GetChild<CPet>(i);
		if (pet && pet->GetPet_uid() == pet_uid)
			return pet;
	}
	return NULL;
}

CPet* CPetGroup::GetPetByIndex(UInt16 index)
{
	CPet* pet = NULL;
	if (index < GetCount())
		pet = GetChild<CPet>(index);

	return pet;

}

void CPetGroup::GetPetIdList(UInt32Vector& idList)
{
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		CPet* pPet = GetChild<CPet>(i);
		if (pPet)
			idList.push_back(pPet->GetPet_uid());
	}
}

void CPetGroup::SetAllPetSkills(NetGroup* skillDataGroups, NetGroup *sendGroup, 
								bool forNewOrLvlUpPet, UInt inWareHouse)
{
	TRACE_ENSURE(skillDataGroups);

	CPet* pPet = NULL;

	for (Index j = C_INDEX(0); j < skillDataGroups->GetCount(); ++j)
	{
		CSkillData* pData = skillDataGroups->GetChild<CSkillData>(j);
		if (pData && pData->GetOwner_type() == SKILL_OWNER_TYPE_PET)
		{			
			if (inWareHouse == 0)
				pPet = GetPet(pData->GetOwner_id());
			else 
				pPet = GetWareHousePet(pData->GetOwner_id());

			if (pPet) 
			{
				CSkill* pSkill = CSkill::NewInstance();
				TRACE_ENSURE(pSkill);
				pSkill->SetData(pData);
				if (pPet->AddSkill(pSkill)) {
					if (sendGroup)
					{
						pSkill->SetSendMask(CSkill::AllMask);
						sendGroup->Add(pSkill);
					}
				} else {
					DeleteNetObject(pSkill);		// failed to add skill
				}
			}
		}
	}

	bool needUpdate = false;
	UInt count = GetCount();
	if (inWareHouse != 0)
		count = (UInt)m_wareHousePets.size();
	for (Index j = C_INDEX(0); j < count; ++j)
	{
		if (inWareHouse == 0)
			pPet = GetPetByIndex(j);
		else
			pPet = GetWareHousePetByIndex(j);

		if (pPet && pPet->HasAddSkill())
		{
			//only for old pet and learn one skill
			if (pPet->m_iNewSkillID > 0)
			{
				pPet->CB_LearnSkillMessage(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), pPet->m_iNewSkillID);
				pPet->m_iNewSkillID = 0;
			}
			if (forNewOrLvlUpPet)
			{
				pPet->SetVP(pPet->GetVP(), pPet->GetMaxVP());
				needUpdate = true;
			}
		}
	}

	if (needUpdate)
		RaiseUpdate();
}

Boolean CPetGroup::Add(NetObject *obj)
{
	if (obj == NULL || obj->GetClassID() != Pet_CLASSID)
		return BOOLEAN_FALSE;

	if (Parent::Add(obj)) {
		CPet* pPet = CastNetObject<CPet>(obj);
		if (pPet) {
			pPet->SetPetGroup(this);
			//if (m_pCharOwner)
			//	m_pCharOwner->GetQuests()->CheckPet(pPet->GetPet_id(), 1);
			return BOOLEAN_TRUE;
		}
	}
	
	return BOOLEAN_FALSE;
}

Boolean CPetGroup::Remove(NetObject *obj)
{
	if (obj == NULL || obj->GetClassID() != Pet_CLASSID)
		return BOOLEAN_FALSE;

	CPet* pPet = CastNetObject<CPet>(obj);
	if (pPet)
	{
		if (pPet->GetPet_uid() == m_pCharOwner->GetPettwo_uid())
		{
			m_pTwoUsePet = NULL;
			m_pCharOwner->SetCurTwoPet(NULL);
		}
		if (pPet->GetInWareHouse() == 0)
		{
			if (Parent::Remove(obj)) 
			{			
				if (pPet) {
					pPet->SetPetGroup(NULL);
					return BOOLEAN_TRUE;
				}
			}
		}
		else
		{
			RPCNetIDVector::iterator itr = remove(m_wareHousePets.begin(), m_wareHousePets.end(), pPet->GetNetId());
			if (itr != m_wareHousePets.end())
				m_wareHousePets.erase(itr);
			return BOOLEAN_TRUE;
		}

		if (m_pCharOwner)
			m_pCharOwner->UpdateSpeed();
	}
	
	return BOOLEAN_FALSE;
}

RPCResult CPetGroup::AddAllPetSkill(RPCContext &context)
{

	PROCEDURE_START_METHOD(CPetGroup, AddAllPetSkill, this, context )

		UInt32Vector petIDs;
		UInt16Vector skillIDs;
		UInt16Vector skillLvls;

		for (UInt8 i = 0; i < GetCount(); ++i)
		{
			CPet* pPet = GetChild<CPet>(i);
			if (pPet)
			{
				for (UInt8 j = 0; j < pPet->GetLearnSkillLst().size(); ++j)
				{
					petIDs.push_back(pPet->GetPet_uid());
					skillIDs.push_back(pPet->GetLearnSkillLst()[j]);
					skillLvls.push_back(1);
				}
				pPet->ClearLearnSkillLst();
			}
		}

		if (petIDs.size() > 0)
		{
			ObjectService_Proxy::CreateSkillList(gGlobal.pObjectService->pPeer, context,
				SKILL_OWNER_TYPE_PET, petIDs, skillIDs, skillLvls);
		}
		else
		{
			PROCEDURE_RETURN
		}

	PROCEDURE_WAIT1(1, ObjectService,  CB_GetSkillList, NetGroup*, skillDataGroups)

			if (skillDataGroups->GetCount() > 0)
			{
				NetGroup skillGroup;
				SetAllPetSkills(skillDataGroups, &skillGroup, true);
				if (m_pCharOwner)
					CB_AddSkills(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), &skillGroup);
			}

			DeleteNetGroup(skillDataGroups, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPetGroup AddAllPetSkill Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CPetGroup::PreSend(NetGroup* skillGroup)
{
	CPet* pPet = NULL;
	for (int i = 0; i < GetCount(); ++i)
	{
		pPet = GetChild<CPet>(i);
		if (pPet)
			pPet->PreSend(skillGroup);
	}
}

void CPetGroup::SendAll()
{
	if (m_pCharOwner == NULL)
		return;

	NetGroup petGroup;
	NetGroup skillGroup;
	CPet* pPet = NULL;
	for (int i = 0; i < GetCount(); ++i)
	{
		pPet = GetChild<CPet>(i);
		if (pPet)
		{
			pPet->PreSend(&skillGroup);
			petGroup.Add(pPet);
		}
	}
	if (petGroup.GetCount() > 0)
		CB_AddPets(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), &petGroup, &skillGroup);
}

RPCResult CPetGroup::LoadPets(RPCContext &context)
{
	if (m_pCharOwner == NULL)
	{
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD(CPetGroup, LoadPets, this, context)

		// get pet data from object service
		ObjectService_Proxy::GetPetList(gGlobal.pObjectService->pPeer, context, 
			m_pCharOwner->GetChar_id(), 0);

	PROCEDURE_WAIT1(1, ObjectService, CB_PetList, NetGroup*, petGroup)

		if (petGroup && petGroup->GetCount() > 0)
		{
			for (Int i = 0; i < petGroup->GetCount(); i++)
			{
				CPetData * pPetData = petGroup->GetChild<CPetData >(i);
				if (pPetData)
				{
					//try to check duplicate partner here
					bool continueLoop = false;
					for (int j = 0; j < GetCount(); ++j)
					{
						CPet* pPetExist = GetChild<CPet>(j);
						if (pPetExist && pPetExist->GetPet_id() == pPetData->GetPet_id() && pPetExist->GetPet_uid() == pPetData->GetPet_uid())
						{
							TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("char %d duplicate pet found  pet_id %d,pet_uid %d"),
								m_pCharOwner->GetChar_id(),pPetExist->GetPet_id(),pPetExist->GetPet_uid());
							continueLoop = true;

						}
					}
					
					if (continueLoop)
						continue;

					CPet * pPet = CPet::NewInstance();
					pPet->SetData(pPetData);
					pPet->PreSend();
					Add(pPet);
					TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("char %d add pet pet_id %d,pet_uid %d"),
						m_pCharOwner->GetChar_id(),pPet->GetPet_id(),pPet->GetPet_uid());
					if (pPet->GetSpirit() == 0)
					{
						pPet->SetSpirit(gGlobal.m_rand.GetRandom(MAX_PET_SPIRIT)+1);
					}
					if (pPet->GetState() == PET_STATE_TYPE_USE && pPet->GetExpireDate() == 0 && pPet->m_pPetData->pet_type == 1)
					{
						pPet->RaiseUniqEventID(CPet::EVT_UPDATE_PET_VP, 0);
					}
					if (pPet->GetState() == PET_STATE_TYPE_USE && pPet->m_pPetData->pet_type == 0)
					{
						pPet->RaiseUniqEventID(CPet::EVT_UPDATE_PET_FAVOR, USE_NOTIME_PET_CHANGE_FAVOR_TIME);
					}
				}
			}
		}
		InitCharPetState();
		DeleteNetGroup(petGroup, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
			err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CPetGroup::UpdateAllPet(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CPetGroup, UpdateAllPet, this, context, Boolean, bSkillUpdate)

		NetGroup oldGrp;
		NetGroup newGrp;
		CPetData *pPetData;
		UInt32Vector petIDs;
		RPCNetIDVector delPetNetIDList;

		for (UInt16 i = 0; i < m_delGroup.GetCount(); ++i)
		{
			CPet* pet = m_delGroup.GetChild<CPet>(i);
			if (pet)
			{
				petIDs.push_back(pet->GetPet_uid());
				pPetData = CreateNetObject<CPetData>();
				TRACE_ENSURE(pPetData);
				pet->WriteData(pPetData);
				newGrp.Add(pPetData);
				delPetNetIDList.push_back(pet->GetNetId());
			}
		}

		for (UInt i = 0; i < GetCount(); ++i)
		{
			CPet* pet = GetChild<CPet>(i);
			if (pet == NULL)
				continue;

			if (pet->IsUpdated()) {
				pet->CancelUpdate();
				pet->SetSendMask(pet->GetUpdateMask());
				pet->ClearUpdateMask();
				oldGrp.Add(pet);
			}
			if (pet->IsModified())
			{
				pPetData = CreateNetObject<CPetData>();
				TRACE_ENSURE(pPetData);
				pet->WriteData(pPetData);
				pet->ClearModifyMask();
				petIDs.push_back(pet->GetPet_uid());
				newGrp.Add(pPetData);
			}
		}

		for (Index i = C_INDEX(0); i < m_wareHousePets.size(); ++i)
		{
			CPet* pet = GetWareHousePetByIndex(i);
			if (pet == NULL)
				continue;

			if (pet->IsUpdated()) {
				pet->CancelUpdate();
				pet->SetSendMask(pet->GetUpdateMask());
				pet->ClearUpdateMask();
				oldGrp.Add(pet);
			}
			if (pet->IsModified())
			{
				pPetData = CreateNetObject<CPetData>();
				TRACE_ENSURE(pPetData);
				pet->WriteData(pPetData);
				pet->ClearModifyMask();
				petIDs.push_back(pet->GetPet_uid());
				newGrp.Add(pPetData);
			}
		}

		if (m_newPets.GetCount() > 0) {
			for (UInt i = 0; i < m_newPets.GetCount(); ++i)
			{
				CPet* pet = m_newPets.GetChild<CPet>(i);
				if (pet) {
					pPetData = CreateNetObject<CPetData>();
					TRACE_ENSURE(pPetData);
					pet->WriteData(pPetData);
					pet->CancelUpdate();
					petIDs.push_back(0);
					newGrp.Add(pPetData);
				}
			}
			DeleteNetChilds(&m_newPets);
		}

		if (oldGrp.GetCount() > 0)
			CB_UpdateAllPets(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), &oldGrp);

		PROCEDURE_STORE_VALUE1(RPCNetIDVector, delPetNetIDList)
		m_delGroup.Clear();

		if (petIDs.size() > 0)
		{
			ObjectService_Proxy::UpdatePetList(gGlobal.pObjectService->pPeer, 
				context, petIDs, &newGrp, true);
		}
		else
		{
			PROCEDURE_GO_STEP(2)
		}
		DeleteNetChilds(&newGrp);

	PROCEDURE_WAIT1(1, ObjectService, CB_UpdatePetList, NetGroup*, newPets)

		NetGroup newGrp;
		NetGroup skillGrp;
		UInt32Vector delIds;
		CPet* pet;

		PROCEDURE_RESTORE_VALUE1(RPCNetIDVector, delPetNetIDList)

		for (UInt16 i = 0; i < delPetNetIDList.size(); ++i )
		{
			pet = FindNetObject<CPet>(delPetNetIDList[i]);
			if (pet == NULL) {
				continue;
			}
			delIds.push_back(pet->GetPet_uid());
			DeleteNetObject(pet);
		}

		// delete pets
		if (!delIds.empty() && m_pCharOwner && m_pCharOwner->GetPeer())
			CB_RemovePets(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), delIds);

		if (m_pCharOwner && newPets && newPets->GetCount() > 0) {
			CPetData* pPetData;
			for (UInt16 i = 0; i < newPets->GetCount(); ++i)
			{
				pPetData = newPets->GetChild<CPetData>(i);
				if (pPetData == NULL || pPetData->GetOwner_id() != m_pCharOwner->GetChar_id())
					continue;
				pet = CPet::NewInstance();
				TRACE_ENSURE(pet);
				pet->SetData(pPetData);
				pet->MakeLearnSkillLst(true);
				pet->SetPetGroup(this);
				pet->SetSendMask(CPet::AllMask);
				pet->ClearModifyMask();
				pet->ClearUpdateMask();
				Add(pet);
				newGrp.Add(pet);
				skillGrp.Add(pet->GetSkillGroup());
			}
			if (newGrp.GetCount() > 0 && m_pCharOwner->GetPeer())
				CB_AddPets(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), &newGrp, &skillGrp);
			DeleteNetGroup(newPets, BOOLEAN_TRUE);
			PROCEDURE_DO_METHOD(CPetGroup, AddAllPetSkill, this, context);
		} else {
			DeleteNetGroup(newPets, BOOLEAN_TRUE);
			PROCEDURE_GO_STEP(2)
		}

	PROCEDURE_WAIT_RETURN(2)

		UInt16 i = 0;

	PROCEDURE_FOR1(3, 5, bSkillUpdate && i < GetCount(), UInt16, i)

		CPet *pPet = GetChild<CPet>(i);
		TRACE_ENSURE(pPet && pPet->m_pSkillGroup);
		
		i++;
		PROCEDURE_STORE_COUNTER1(UInt16, i)
		PROCEDURE_DO_METHOD(CSkillGroup, UpdateSkills, pPet->m_pSkillGroup, context)

	PROCEDURE_WAIT_RETURN(4)
	
	PROCEDURE_END_FOR(5, 3)

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPetGroup DeletePet Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH


	return RPC_RESULT_OK;
}

void CPetGroup::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_UPDATE_OBJECT:				// update all pet to object service
		if (m_pCharOwner && (GetCount() > 0 || m_newPets.GetCount() > 0 || 
			m_delGroup.GetCount() > 0 || m_wareHousePets.size() > 0))
		{
			PROCEDURE_DO_METHOD1(CPetGroup, UpdateAllPet, this, m_pCharOwner->GetContext(),Boolean, BOOLEAN_FALSE)
		}
		break;
	}
}

LPCPEER CPetGroup::GetOwnerPeer()
{
	if (m_pCharOwner)
		return m_pCharOwner->GetPeer();
	else
		return NULL;
}

RPCContext &CPetGroup::GetOwnerContext()
{
	static RPCContext dummyContext;
	if (m_pCharOwner)
		return m_pCharOwner->GetContext();
	else
		return dummyContext;
}

CPet* CPetGroup::GetCurPet()
{
	CPet* pet;
	for (int i = 0; i < GetCount(); ++i)
	{
		pet = GetChild<CPet>(i);
		if (pet && pet->GetState() == PET_STATE_TYPE_USE)
			return pet;
	}
	return NULL;
}

/*RPCResult CPetGroup::DeletePetByPetID(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CPetGroup, DeletePetByPetID, this, context, 
		UInt32, pet_id)
		TRACE_ENSURE(m_pCharOwner);
		if (!m_pCharOwner->IsInMap()) {
			TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("DeletePet Warning: [owner not in map] char_id %d, sessionId %s, pet_uid %d"), 
				m_pCharOwner->GetChar_id(), context.sessionId.GetString().c_str(), pet_id);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CPet * pet = NULL;

		for (int i = 0; i < GetCount(); ++i)
		{
			CPet * pTemp = GetChild<CPet>(i);
			if (pTemp && pTemp->GetPet_id() == pet_id)
				pet = pTemp;
		}

		if (pet == NULL)
		{
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		UInt32 pet_uid = pet->GetPet_uid();
		UInt16Vector skillIDs;
		NetGroup* pSkillGroup = pet->GetSkillGroup(); 
		if (pSkillGroup)
		{
			for (UInt8 i = 0; i < pSkillGroup->GetCount(); ++i)
			{
				CSkill* pSkill = pSkillGroup->GetChild<CSkill>(i);
				if (pSkill)
					skillIDs.push_back(pSkill->GetSkill_id());
			}
		}

		if (skillIDs.size() > 0)
		{
			ObjectService_Proxy::DeleteSkillList(gGlobal.pObjectService->pPeer, context, pet_uid, skillIDs, SKILL_OWNER_TYPE_PET);
		}
		else
		{
			PROCEDURE_GO_STEP(1)
		}

	PROCEDURE_WAIT(1, ObjectService, CB_DeleteSKillDone)

		CPet * pet = NULL;

		for (int i = 0; i < GetCount(); ++i)
		{
			CPet * pTemp = GetChild<CPet>(i);
			if (pTemp && pTemp->GetPet_id() == pet_id)
				pet = pTemp;
		}

		if (pet == NULL)
		{
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		UInt32 pet_uid = pet->GetPet_uid();

		ObjectService_Proxy::DeletePet(gGlobal.pObjectService->pPeer, context,
			pet_uid);

	PROCEDURE_WAIT(2, ObjectService, CB_DeletePet)

		CPet * pet = NULL;

		for (int i = 0; i < GetCount(); ++i)
		{
			CPet * pTemp = GetChild<CPet>(i);
			if (pTemp && pTemp->GetPet_id() == pet_id)
				pet = pTemp;
		}

		if (pet == NULL)
		{
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		UInt32 pet_uid = pet->GetPet_uid();

		CPet* delPet = pet;
		
		if (delPet) {
			
			if (m_pCharOwner->GetPet_uid() == pet_uid)
				m_pCharOwner->SetPet(0, 0, String(), 0);
			Remove(delPet);
			DeleteNetObject(delPet);
			UInt32Vector pet_uids;
			pet_uids.push_back(pet_uid);
			CB_RemovePets(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), pet_uids);
		} else {
			String errMsg;
			errMsg.Format(_T("DeletePet Error: pet_uid %d not found"), pet_uid);
			TRACE_ERRORDTL(GLOBAL_LOGGER, errMsg.c_str());
			CB_Error(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), 1, errMsg);
		}

		//RaiseUniqEventID(EVT_ATTRIB_UPDATE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPetGroup DeletePet Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}*/


/*RPCResult CPetGroup::DeletePet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_uid)
{
	PROCEDURE_START_RPC1(PetGroup, DeletePet, this, UInt32, pet_uid)
	
		TRACE_ENSURE(m_pCharOwner);
		if (!m_pCharOwner->IsInMap()) {
			TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("DeletePet Warning: [owner not in map] char_id %d, sessionId %s, pet_uid %d"), 
				m_pCharOwner->GetChar_id(), context.sessionId.GetString().c_str(), pet_uid);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CPet * pet = GetPet(pet_uid);

		if (pet == NULL)
		{
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		UInt16Vector skillIDs;
		NetGroup* pSkillGroup = pet->GetSkillGroup(); 
		if (pSkillGroup)
		{
			for (UInt8 i = 0; i < pSkillGroup->GetCount(); ++i)
			{
				CSkill* pSkill = pSkillGroup->GetChild<CSkill>(i);
				if (pSkill)
					skillIDs.push_back(pSkill->GetSkill_id());
			}
		}

		if (skillIDs.size() > 0)
		{
			ObjectService_Proxy::DeleteSkillList(gGlobal.pObjectService->pPeer, context, pet_uid, skillIDs, SKILL_OWNER_TYPE_PET);
		}
		else
		{
			PROCEDURE_GO_STEP(1)
		}

	PROCEDURE_WAIT(1, ObjectService, CB_DeleteSKillDone)

		ObjectService_Proxy::DeletePet(gGlobal.pObjectService->pPeer, context,
			pet_uid);

	PROCEDURE_WAIT(2, ObjectService, CB_DeletePet)

		CPet* pet;
		CPet* delPet = NULL;
		for (int i = 0; i < GetCount(); ++i)
		{
			pet = GetChild<CPet>(i);
			if (pet && pet->GetPet_uid() == pet_uid) {
				delPet = pet;
				break;
			}
		}

		if (delPet) {
			
			if (m_pCharOwner->GetPet_uid() == pet_uid)
				m_pCharOwner->SetPet(0, 0, String(), 0);
			Remove(delPet);
			DeleteNetObject(delPet);
			UInt32Vector pet_uids;
			pet_uids.push_back(pet_uid);
			CB_RemovePets(pPeer, context, pet_uids);
		} else {
			String errMsg;
			errMsg.Format(_T("DeletePet Error: pet_uid %d not found"), pet_uid);
			TRACE_ERRORDTL(GLOBAL_LOGGER, errMsg.c_str());
			CB_Error(pPeer, context, 1, errMsg);
		}

		//RaiseUniqEventID(EVT_ATTRIB_UPDATE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPetGroup DeletePet Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}*/

RPCResult CPetGroup::DeletePet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_uid)
{
	
	
	TRACE_ENSURE(m_pCharOwner);
	if (!m_pCharOwner->IsInMap()) {
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("DeletePet Warning: [owner not in map] char_id %d, sessionId %s, pet_uid %d"), 
			m_pCharOwner->GetChar_id(), context.sessionId.GetString().c_str(), pet_uid);
		return RPC_RESULT_OK;
	}

	CPet * pet = GetPet(pet_uid);

	if (pet == NULL)
		return RPC_RESULT_OK;

	if (pet && pet->GetLock())
	{
		UInt32 now = (UInt32) time(NULL);
		if (pet->GetExpireDate() == 0 || pet->GetExpireDate() > now)
			return RPC_RESULT_OK;
	}
	if (m_pTwoUsePet && m_pTwoUsePet->GetPet_uid() == pet_uid)
	{
		m_pTwoUsePet = NULL;
		m_pCharOwner->SetCurTwoPet(NULL);
	}
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Charater %d DeletePet pet  pet_uid %d"),
		m_pCharOwner->GetChar_id(),pet_uid);	
	RemovePet(pet);

	return RPC_RESULT_OK;
}


void CPetGroup::InitCharPetState()
{
	if (m_pCharOwner == NULL)
		return;

	UInt32 happinessDrop = 0;
	UInt32 now	= (UInt32) time(NULL); 
	if (now > m_pCharOwner->GetOfflineTime())
		happinessDrop = (now - m_pCharOwner->GetOfflineTime()) / 3600;
	
	//m_pCharOwner->SetPet(0, 0, String(), 0);
	for (UInt i = 0; i < GetCount(); i ++)
	{
		CPet* pPet = GetChild<CPet>(i);
		if (pPet)
		{
			if (pPet->GetPet_uid() == m_pCharOwner->GetPet_uid())
			{
				pPet->ChangeState(PET_STATE_TYPE_USE);
				m_pCharOwner->SetPet(pPet->GetPet_uid(), pPet->GetPet_id(), pPet->GetName(), 
					pPet->GetColor());
				pPet->m_PreUpdateTime = ::timeGetTime();
				pPet->RaiseUniqEventID(CPet::EVT_UPDATE_PET_INFO, gConfigure.petUpdateInterval);
			}
			else
			{
				pPet->ChangeState(PET_STATE_TYPE_REST);
			}
			/*
			if (pPet->GetPet_State() == PET_STATE_TYPE_USE)
			{
				m_pCharOwner->SetPet(pPet->GetPet_uid(), pPet->GetPet_id(), pPet->GetPet_Name(), pPet->GetPet_color());
				pPet->m_PreUpdateTime = ::timeGetTime();
				pPet->RaiseUniqEventID(CPet::EVT_UPDATE_PET_INFO, gConfigure.m_petUpdateInterval);
			}
			

			if (pPet->GetHappiness() > happinessDrop)
				pPet->SetHappiness(pPet->GetHappiness() - happinessDrop);
			else
				pPet->SetHappiness(0);
		*/
			
		}
	}
}


RPCResult CPetGroup::ChangePetState(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_uid, const UInt8 &state)
{
	if (m_pCharOwner == NULL)
		return RPC_RESULT_OK;
	CPet* m_nowUsePet = NULL;
	if (GetCurPet() && m_pTwoUsePet)
	{
		for (int i = 0; i < GetCount(); ++i)
		{
			CPet* pet = GetChild<CPet>(i);
			if (pet && pet->GetState() == PET_STATE_TYPE_USE && pet->GetPet_uid() != m_pTwoUsePet->GetPet_uid())
				m_nowUsePet = pet ;
		}
		if(m_nowUsePet)
			m_nowUsePetType = m_nowUsePet->m_pPetData->pet_type;
	}
	else if (GetCurPet() && m_pTwoUsePet == NULL)
	{
		m_nowUsePet = GetCurPet();	
		m_nowUsePetType = GetCurPet()->m_pPetData->pet_type;
	}
	else
		m_nowUsePetType = NOT_USE_PET;
	//find whther it can use the pet first
	if (state == PET_STATE_TYPE_USE)
	{
		for (UInt i = 0; i < GetCount(); i ++)
		{
			CPet* pPet = GetChild<CPet>(i);
			if (pPet && pPet->GetPet_uid() == pet_uid && pPet->m_pPetData)
			{
				if (pPet->m_pPetData->pet_ReqLV > m_pCharOwner->GetLevel())
				{
					m_pCharOwner->CB_Error(pPeer, context, 1, _T("MSG_USE_PET_FAIL_LV"));
					return RPC_RESULT_OK;
				}
			}
		}
	}
	m_pCharOwner->SetPet(0, 0, String(), 0);
	if (m_nowUsePetType == 1 && GetPet(pet_uid)->m_pPetData->pet_type == 0)
	{
		m_pTwoUsePet = GetPet(pet_uid);
		if (m_pCharOwner && m_pTwoUsePet)
		{
			if(state == PET_STATE_TYPE_REST)
			{
				if(m_pCharOwner->m_pUseTwoPet)
				{
					if( m_pCharOwner->m_pUseTwoPet == m_pTwoUsePet)
					{
						m_pCharOwner->SetCurTwoPet(NULL);
					}
				}
				m_pTwoUsePet->ChangeState(state);
				m_pTwoUsePet->SendAttr(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), m_pTwoUsePet->GetUpdateMask());
				m_pTwoUsePet = NULL;
			}
			else
			{
				if(m_pCharOwner->m_pUseTwoPet)
				{
					if(m_pCharOwner->m_pUseTwoPet != m_pTwoUsePet)
					{
						m_pCharOwner->m_pUseTwoPet->ChangeState(PET_STATE_TYPE_REST);
						m_pCharOwner->m_pUseTwoPet->SendAttr(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), m_pCharOwner->m_pUseTwoPet->GetUpdateMask());
					}
				}
				m_pCharOwner->SetCurTwoPet(m_pTwoUsePet);
				m_pTwoUsePet->ChangeState(state);
				m_pTwoUsePet->SendAttr(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), m_pTwoUsePet->GetUpdateMask());
			}
			if(m_nowUsePet)
			{
				m_pCharOwner->SetPet(m_nowUsePet->GetPet_uid(), m_nowUsePet->GetPet_id(), m_nowUsePet->GetName(), m_nowUsePet->GetColor());
				m_pCharOwner->SendAttr(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), m_pCharOwner->GetUpdateMask());
				m_nowUsePet->SendAttr(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), m_nowUsePet->GetUpdateMask());
				m_pCharOwner->UpdateSpeed();
			}
		}
		return RPC_RESULT_OK;
	}
	else if (state == PET_STATE_TYPE_REST && m_nowUsePetType == 1 && m_pTwoUsePet && m_nowUsePet)
	{
		if (pet_uid != m_nowUsePet->GetPet_uid())
			return RPC_RESULT_OK;
		m_nowUsePet->ChangeState(PET_STATE_TYPE_REST);
		m_pCharOwner->SetCurTwoPet(NULL);
		m_pCharOwner->SetPet(m_pTwoUsePet->GetPet_uid(), m_pTwoUsePet->GetPet_id(), m_pTwoUsePet->GetName(), m_pTwoUsePet->GetColor());
		m_pCharOwner->SendAttr(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), m_pCharOwner->GetUpdateMask());
		m_pTwoUsePet->SendAttr(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), m_pTwoUsePet->GetUpdateMask());
		m_nowUsePet->SendAttr(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), m_nowUsePet->GetUpdateMask());
		m_pCharOwner->UpdateSpeed();
		m_pTwoUsePet = NULL;
		return RPC_RESULT_OK;
	}
	else
	{
		if (m_nowUsePetType == 0 && GetPet(pet_uid)->m_pPetData->pet_type == 1)
		{
			m_pTwoUsePet = GetCurPet();
			m_pCharOwner->SetCurTwoPet(m_pTwoUsePet);
		}
		for (UInt i = 0; i < GetCount(); i ++)
		{
			CPet* pPet = GetChild<CPet>(i);
			if (pPet)
			{
				if (pet_uid == pPet->GetPet_uid())
					pPet->ChangeState(state);
				else if (state == PET_STATE_TYPE_USE)
				{
					if( m_pTwoUsePet&& pPet->GetPet_uid() == m_pTwoUsePet->GetPet_uid())
						continue;
					else
						pPet->ChangeState(PET_STATE_TYPE_REST);
				}
				if (pPet->GetState() == PET_STATE_TYPE_USE)
				{
					if (m_pTwoUsePet && pPet->GetPet_uid() == m_pTwoUsePet->GetPet_uid())
						continue;
					if (pPet->GetExpireDate() == 0 && pPet->m_pPetData->pet_TimeLimitType == PET_TIME_LIMIT_TYPE_USE)
					{
						UInt32 now = (UInt32) time(NULL);
						UInt32 expire;
						expire = now + 3600 * 24 * pPet->m_pPetData->pet_TimeLimit;
						pPet->SetExpireDate(expire);
						pPet->RaiseUpdate();

						pPet->RaiseUniqEventID(CPet::EVT_UPDATE_PET_EXPIRE, (now - expire) * 1000);
					}
					m_pCharOwner->SetPet(pPet->GetPet_uid(), pPet->GetPet_id(), pPet->GetName(), pPet->GetColor());
					m_pCharOwner->SendAttr(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), m_pCharOwner->GetUpdateMask());
				}
				pPet->SendAttr(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), pPet->GetUpdateMask());
			}
		}
	}

	m_pCharOwner->UpdateSpeed();

	return RPC_RESULT_OK;
}

UInt16 CPetGroup::GetIdCount(UInt32 pet_id)
{
	UInt16 num = 0;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		CPet* pPet = GetChild<CPet>(i);
		if (pPet && pPet->GetPet_id() == pet_id)
			++num;
	}

	return num;
}

void CPetGroup::AddPet(UInt32 pet_id)
{
	if (m_pCharOwner) {
		PROCEDURE_DO_METHOD1(CPetGroup, AddPet, this, m_pCharOwner->GetContext(), UInt32, pet_id)
	}
}

void CPetGroup::RemovePet(UInt32 pet_id)
{
	CPet * pet = NULL;
	for (int i = 0; i < GetCount(); ++i) {
		CPet * pTemp = GetChild<CPet>(i);
		if (pTemp && pTemp->GetPet_id() == pet_id)
			pet = pTemp;
	}

	if (pet)
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("charater %d RemovePet pet  pet_uid %d"),
			m_pCharOwner->GetChar_id(),pet->GetPet_id(),pet->GetPet_uid());	
		RemovePet(pet);
	}
}

void CPetGroup::RemovePetByUID(UInt32 pet_uid)
{
	CPet * pet = NULL;
	for (int i = 0; i < GetCount(); ++i) {
		CPet * pTemp = GetChild<CPet>(i);
		if (pTemp && pTemp->GetPet_uid() == pet_uid)
			pet = pTemp;
	}

	if (pet)
		RemovePet(pet);
}

void CPetGroup::RemovePet(CPet* pPet)
{
	TRACE_ENSURE(m_pCharOwner);
	if (pPet == NULL)
		return;

	if (!m_pCharOwner->IsInMap()) {
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("DeletePet Warning: [owner not in map] char_id %d, pet_id %d"), 
			m_pCharOwner->GetChar_id(), pPet->GetPet_uid());
		return;
	}
	TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("charater %d RemovePet pet obj pet_id %d pet_uid %d"),
		m_pCharOwner->GetChar_id(),pPet->GetPet_id(),pPet->GetPet_uid());	
	pPet->SetOwner_id(0);				// set no owner mean removed
	m_delGroup.Add(pPet);
	Remove(pPet);
	UInt32Vector pet_uids;
	pet_uids.push_back(pPet->GetPet_uid());
	if (m_pCharOwner )
	{
		if (m_pCharOwner->GetPet_uid() == pPet->GetPet_uid())
		{
			m_pCharOwner->SetPet(0, 0, String(), 0);
			m_pCharOwner->UpdateSpeed();
		}
		if (m_pCharOwner->GetPeer())
			CB_RemovePets(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), pet_uids);
	}

	RaiseUpdate();
}

void CPetGroup::InitPetOwnObject()
{
	CPet* pPet = NULL;
	for (int i = 0; i < GetCount(); ++i)
	{
		pPet = GetChild<CPet>(i);
		if (pPet)
			pPet->SetPetGroup(this); //setpetgroup will do setownobject.
	}
}

RPCResult CPetGroup::AddPet(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CPetGroup, AddPet, this, context, UInt32, pet_id)

		TRACE_ENSURE(m_pCharOwner);
		// fix remove and add pet at the same time
		UInt16 count = 0;
		CPet* pet;
		for (UInt16 i = 0; i < GetCount(); i++)
		{
			pet = GetChild<CPet>(i);
			if (pet && pet->GetOwner_id() == m_pCharOwner->GetChar_id())
				++count;
		}
		if (count >= m_pCharOwner->GetMaxPet())
		{
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("AddPet Warning: char_id %d, excess pet max %d"), 
				m_pCharOwner->GetChar_id(), m_pCharOwner->GetMaxPet());
			CB_Error(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), 1, _T("MSG_PET_FULL"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

		ObjectService_Proxy::CreatePet(gGlobal.pObjectService->pPeer, context, pet_id, 0,
			m_pCharOwner->GetChar_id(), 0, String(), 0, String(), m_pCharOwner->GetNickName());

	PROCEDURE_WAIT1(1, ObjectService, CB_CreatePet, NetObject*, pet)

		CPetData* pPetData;
		CPet* pPet;

		NetGroup grp;

		pPetData = CastNetObject<CPetData>(pet);
		pPet = CPet::NewInstance();
		pPet->SetData(pPetData);
		pPet->SetPetGroup(this);
		pPet->PreSend();
		Add(pPet);
		grp.Add(pPet);
		NetGroup skillGrp;
		skillGrp.Add(pPet->GetSkillGroup());

		if (m_pCharOwner && m_pCharOwner->GetPeer())
			CB_AddPets(m_pCharOwner->GetPeer(), m_pCharOwner->GetContext(), &grp, &skillGrp);

		DeleteNetObject(pet);
		pPet->MakeLearnSkillLst(true, false);

		UInt8 level = 1;
		bool forNewPet = true;
		PROCEDURE_DO_METHOD3(CPet, DoLearnSkill, pPet, context, 
			UInt16Vector, pPet->GetLearnSkillLst(), UInt8, level, bool, forNewPet)
		pPet->ClearLearnSkillLst();
		//pPet->ClearAllMask();

	PROCEDURE_WAIT_RETURN(2)
	

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("AddPet Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

UInt16 CPetGroup::GetCount(bool withNewPet)
{
	if (withNewPet)
		return Parent::GetCount() + m_newPets.GetCount();
	return Parent::GetCount();
}

void CPetGroup::ChangeOwnerID(const UInt32Vector &petUIDs, UInt32 targetOwnerID)
{
	UInt32Vector petIds;
	for (Index i = C_INDEX(0); i < petUIDs.size(); ++i)
	{
		CPet* pet = GetPet(petUIDs[i]);
		if (pet && m_pCharOwner->GetChar_id() != targetOwnerID)
		{
			pet->SetOwner_id(targetOwnerID);
			if (pet->GetState() == PET_STATE_TYPE_USE)
			{
				pet->ChangeState(PET_STATE_TYPE_REST);
				m_pCharOwner->UpdateSpeed();
			}
			petIds.push_back(pet->GetPet_id());
			
			Remove(pet);
		}
		else
			return;
	}

	PROCEDURE_DO_METHOD3(CPetGroup, ChangeOwnerID, this, m_pCharOwner->GetContext(),
				UInt32Vector, petUIDs, UInt32Vector, petIds, UInt32, targetOwnerID)
}

RPCResult CPetGroup::ChangeOwnerID(RPCContext &context)
{
	PROCEDURE_START_METHOD3(CPetGroup, ChangeOwnerID, this, context, UInt32Vector, petUIDs, UInt32Vector, petIds, UInt32, targetOwnerID)

		NetGroup grp;

		for (Index i = C_INDEX(0); i < petUIDs.size(); ++i)
		{
			CPetData* pPetData = CreateNetObject<CPetData>();
			TRACE_ENSURE(pPetData);
			pPetData->SetId(petUIDs[i], petIds[i]);
			pPetData->SetOwner_id(targetOwnerID);	
			pPetData->SetSendMask(pPetData->GetUpdateMask());
			grp.Add(pPetData);
		}

		ObjectService_Proxy::UpdatePetList(gGlobal.pObjectService->pPeer, 
			context, petUIDs, &grp, true);
		
		DeleteNetChilds(&grp);

	PROCEDURE_WAIT1(1, ObjectService, CB_UpdatePetList, NetGroup*, newPets)

		DeleteNetGroup(newPets, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPetGroup ChangeOwner Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

Boolean CPetGroup::ChangePetLockStatus(const UInt32 &bagIndex, const Boolean &lock, UInt8 &msg)
{
	CPet* pPet = GetPetByIndex(bagIndex);
	if (!pPet)
	{
		msg = ASSET_INVALID_ERR;
		return BOOLEAN_FALSE;
	}
	if (pPet->GetLock() == lock)
	{
		if (lock)
			msg = ASSET_LOCK_ALREADY;	
		else
			msg = ASSET_UNLOCK_ALREADY;	
			
		return BOOLEAN_FALSE;
	}
	pPet->SetLock(lock);
	pPet->RaiseUpdate(); 
	msg = lock? ASSET_LOCK_SUCCESS : ASSET_UNLOCK_SUCCESS;	
	return BOOLEAN_TRUE;
}

void CPetGroup::SetWareHousePet(NetGroup* petGroup)
{
	m_wareHousePets.clear();
	if (petGroup && petGroup->GetCount() > 0)
	{
		CPetData* pPetData;
		CPet* pPet;

		for (int i = 0; i < petGroup->GetCount(); ++i)
		{
			pPetData = petGroup->GetChild<CPetData>(i);
			if (pPetData == NULL) continue;
			pPet = CPet::NewInstance();
			pPet->SetData(pPetData);
			m_wareHousePets.push_back(pPet->GetNetId());
		}
	}
}

void CPetGroup::GetWareHousePetUID(UInt32Vector& idVector)
{
	CPet* pet = NULL;
	for (Index i = C_INDEX(0); i < m_wareHousePets.size(); ++i)
	{
		pet = GetWareHousePetByIndex(i);
		if (pet)
			idVector.push_back(pet->GetPet_uid());
	}
}
	
CPet* CPetGroup::GetWareHousePetByIndex(UInt index)
{
	if (index >= m_wareHousePets.size())
		return NULL;

	return FindNetObject<CPet>(m_wareHousePets[index]);	
}

CPet* CPetGroup::GetWareHousePet(UInt32 petUID)
{
	CPet* pet = NULL;
	for (Index i = C_INDEX(0); i < m_wareHousePets.size(); ++i)
	{
		pet = GetWareHousePetByIndex(i);
		if (pet && pet->GetPet_uid() == petUID)
			return pet;
	}
	return NULL;
}

void CPetGroup::ClearWareHouse()
{
	CPet* pet = NULL;
	for (Index i = C_INDEX(0); i < PET_MAX_WAREHOUSE; ++i)
	{
		pet = GetWareHousePetByIndex(i);
		if (pet)
			DeleteNetObject(pet);
	}
	m_wareHousePets.clear();
}

void CPetGroup::GetAllWareHousePet(NetGroup* petGroup, NetGroup* skillGroup)
{
	if (petGroup && skillGroup)
	{
		CPet* pet = NULL;
		for (Index i = C_INDEX(0); i < PET_MAX_WAREHOUSE; ++i)
		{
			pet = GetWareHousePetByIndex(i);
			if (pet)
			{
				pet->PreSend(skillGroup);
				petGroup->Add(pet);				
			}
		}
	}
}

RPCResult CPetGroup::PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &petUID)
{
	UInt msgIndex = WAREHOUSE_SUCCESS;
	do
	{
		if (m_pCharOwner == NULL || !m_pCharOwner->IsInMap())
		{
			msgIndex = WAREHOUSE_NOT_IN_MAP;
			break;
		}

		// cannot move item when the script is running
		if (m_pCharOwner->GetScriptSystem() && m_pCharOwner->GetScriptSystem()->IsRunning())
		{
			msgIndex = WAREHOUSE_RUNNING_SCIRPT;
			break;
		}

		if (m_pCharOwner->GetAvailPetWareHouse() <= m_wareHousePets.size())
		{
			msgIndex = WAREHOUSE_FULL;
			break;
		}

		if (m_pCharOwner->IsTrading())
		{
			msgIndex = WAREHOUSE_CANNOT_TRADING;
			break;
		}

		CPet* pet = GetPet(petUID);
		if (pet == NULL || pet->GetInWareHouse() != 0)
		{
			msgIndex = WAREHOUSE_NO_PET;
			break;
		}

		if (pet->GetState() == PET_STATE_TYPE_USE)
		{
			msgIndex = WAREHOUSE_PUT_IN_UING_STATE;
			break;
		}
	
		Remove(pet);
		if (m_pCharOwner->GetPet_uid() == pet->GetPet_uid())
			m_pCharOwner->SetPet(0, 0, _T(""), 0);
		pet->ChangeState(PET_STATE_TYPE_REST);
		pet->SetInWareHouse(1);
		m_wareHousePets.push_back(pet->GetNetId());
		RaiseUpdate();

	} 
	while(false);

	CB_WareHouse(pPeer, context, WAREHOUSE_PROCESS_PUT_PET, msgIndex);

	return RPC_RESULT_OK;
}

RPCResult CPetGroup::GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &petUID)
{
	UInt msgIndex = WAREHOUSE_SUCCESS;
	do
	{
		if (m_pCharOwner == NULL || !m_pCharOwner->IsInMap())
		{
			msgIndex = WAREHOUSE_NOT_IN_MAP;
			break;
		}

		// cannot move item when the script is running
		if (m_pCharOwner->GetScriptSystem() && m_pCharOwner->GetScriptSystem()->IsRunning())
		{
			msgIndex = WAREHOUSE_RUNNING_SCIRPT;
			break;
		}

		if (GetCount() >= m_pCharOwner->GetMaxPet())
		{
			msgIndex = WAREHOUSE_FULL;
			break;
		}

		if (m_pCharOwner->IsTrading())
		{
			msgIndex = WAREHOUSE_CANNOT_TRADING;
			break;
		}

		CPet* pet = GetWareHousePet(petUID);
		if (pet == NULL || pet->GetInWareHouse() != 1)
		{
			msgIndex = WAREHOUSE_NO_PET;
			break;
		}
		
		RPCNetIDVector::iterator itr = remove(m_wareHousePets.begin(), m_wareHousePets.end(), pet->GetNetId());
		if (itr == m_wareHousePets.end())
		{
			msgIndex = WAREHOUSE_NO_PET;
			break;
		}

		m_wareHousePets.erase(itr);
		pet->SetInWareHouse(0);
		pet->ChangeState(PET_STATE_TYPE_REST);
		Add(pet);
		RaiseUpdate();
	}
	while(false);

	CB_WareHouse(pPeer, context, WAREHOUSE_PROCESS_GET_PET, msgIndex);

	return RPC_RESULT_OK;
}
