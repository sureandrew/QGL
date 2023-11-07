#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
#include "Common/CallLuaFunc.h"

#include "Stub/Logic/Pet.h"
#include "Stub/Logic/PetGroup.h"
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/SkillGroup.h"
#include "Proxy/Data/SkillData.h"
#include "Proxy/Data/PetData.h"
#include "MapService/Global.h"
#include "MapService/Configure.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResSkill.h"
#include "Proxy/Service/ObjectService.h"
#include "Stub/Logic/Character.h"
#include "Resource/ResSkillCombination.h"

REGISTER_CLASSTYPE(CPet, Pet_CLASSID);
#define CONFIGURATION_INI_FILE		_T(".\\Data\\Setting\\fileconfig.ini")
#define USE_NOTIME_PET_CHANGE_VP_TIME (6*60*1000) //6 min
#define	USE_NOTIME_PET_CHANGE_FAVOR_TIME (1*60*1000)//1 min

CPet::CPet()
{
	m_pOwnerGroup = NULL;
	m_iNewSkillID = 0;
	m_bHasAddSkill = false;
	m_skillMap.clear();
	m_pSkillGroup = CSkillGroup::NewInstance();
	m_pSkillGroup->SetOwner(this);
	m_LearnSkillID.clear();
	m_PreUpdateTime = 0;
	m_pPetData = NULL;
	m_newUsetime = 0;
	m_useTime = 0;
	m_usePetTwoTime = 0;
	m_newUsePetTwoTime = 0;
}

CPet::~CPet()
{
	CancelAllEvent();
	m_skillMap.clear();
	m_LearnSkillID.clear();
	DeleteNetGroup(m_pSkillGroup, BOOLEAN_TRUE);
}

void CPet::UpdateInfo()
{
	/*
	if (m_pPetData == NULL)
		m_pPetData = gGlobal.m_resource->GetPetData(GetPet_id());

	if (m_pPetData == NULL)
		return;

	if (GetState() != PET_STATE_TYPE_USE)
		return;

	UInt32 currTime = ::timeGetTime();

	if (currTime > m_PreUpdateTime)
	{
		UInt32 diff = currTime - m_PreUpdateTime;

		UInt point = diff / gConfigure.petUpdateTimePoint;
		if (point == 0)
			return;

		if (m_pPetData->pet_breed_type == PET_BREED_TYPE_TRAIN)
		{
			if (this->GetVP() < point)
				this->SetVP(0, GetMaxVP());
			else
				this->SetVP(this->GetVP() - point, GetMaxVP());

			if (GetVP() == 0)
			{
				if (GetHappiness() > point)
					this->SetHappiness(GetHappiness() - point);
				else
					this->SetHappiness(0);

			}

			if (this->GetHappiness() == MAX_PET_HAPPINESS &&
				GetLevel() < gConfigure.petMaxLevel)
			{

				UInt32 nextExp =  0;
				CallLuaFunc(gGlobal.g_pLuaState, "CalPetNextLvExp", String(), GetLevel() + 1);
				nextExp = (UInt32)gGlobal.g_TempResult;
				if (nextExp <= point + GetExp())
				{
					SetExp(point + GetExp() - nextExp);
					SetLevel(GetLevel() + 1);
					MakeLearnSkillLst(false, true);
					ClearLearnSkillLst();
				}
				else
					SetExp(point + GetExp());
			}
		}

		if (IsUpdated())
			RaiseUpdate();

		m_PreUpdateTime = currTime;
	}*/
}

void CPet::ChangeState(BYTE state)
{
	if (state != GetState())
	{
		if (state == PET_STATE_TYPE_USE)
		{
			m_PreUpdateTime = ::timeGetTime();
			RaiseUniqEventID(EVT_UPDATE_PET_INFO, gConfigure.petUpdateInterval);
			if (GetSoulbound() == SOULBOUND_USE_BOUND)
				SetSoulbound(SOULBOUND_ALREADY_BOUND);
			if (GetExpireDate() == 0 && GetPetType() == 1)
			{
				if(m_useTime <= 0)
					m_useTime = 0;
				else if(m_useTime > USE_NOTIME_PET_CHANGE_VP_TIME)
					m_useTime = USE_NOTIME_PET_CHANGE_VP_TIME;
				if(m_useTime == 0)
					RaiseUniqEventID(EVT_UPDATE_PET_VP, 0);
				else
					RaiseUniqEventID(EVT_UPDATE_PET_VP, USE_NOTIME_PET_CHANGE_VP_TIME-m_useTime);
			}
			if (GetPetType() == 0)
			{
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("m_usePetTwoTime = %d "), m_usePetTwoTime);
				if (m_usePetTwoTime > USE_NOTIME_PET_CHANGE_FAVOR_TIME)
				{
					m_usePetTwoTime = USE_NOTIME_PET_CHANGE_FAVOR_TIME;
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Change m_usePetTwoTime = %d "), m_usePetTwoTime);
				}
				RaiseUniqEventID(EVT_UPDATE_PET_FAVOR, USE_NOTIME_PET_CHANGE_FAVOR_TIME-m_usePetTwoTime);
			}
		}
		else
		{
			UpdateInfo();
			CancelUniqEvent(EVT_UPDATE_PET_INFO);
			if (GetExpireDate() == 0 && GetPetType() == 1)
			{	
				if (m_newUsetime <= 0)
					m_useTime = 0;
				else
				{
					UInt32 now =  (UInt32) time(NULL);
					m_useTime = (now - m_newUsetime)*1000;
				}
				UInt32 now =  (UInt32) time(NULL);
				m_useTime = (now - m_newUsetime)*1000;
				CancelUniqEvent(EVT_UPDATE_PET_VP);
			}
			if (GetPetType() == 0)
			{
				if (m_newUsePetTwoTime <= 0)
					m_usePetTwoTime = 0;
				else
				{
					UInt32 now =  (UInt32) time(NULL);
					if(now > m_newUsePetTwoTime)
						m_usePetTwoTime = (now - m_newUsePetTwoTime)*1000;
					else
						m_usePetTwoTime = 0;
				}
				UInt32 now =  (UInt32) time(NULL);
				m_usePetTwoTime = (now - m_newUsePetTwoTime)*1000;
				CancelUniqEvent(EVT_UPDATE_PET_FAVOR);
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("m_newUsePetTwoTime = %d ,m_usePetTwoTime = %d"), m_newUsePetTwoTime,m_usePetTwoTime);
			}
		}

		SetState(state);
	}
}

/*RPCResult CPet::UpdateToObject(RPCContext &context)
{
	PROCEDURE_START_METHOD(CPet, UpdateToObject, this, context)

		if (m_pOwnerGroup->GetOwner())
		{
			RPCMask mask;
			mask.MaskBits(AllMask, -1);
			mask &= GetUpdateMask();			// send only updated attributes
			SendAttr(m_pOwnerGroup->GetOwner()->GetPeer(), 
				m_pOwnerGroup->GetOwner()->GetContext(), mask);
			m_updateMask.Unmask(mask);
		}

		if (IsModified())
		{
			CPetData petData;
			WriteData(&petData);

			TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, _F("UpdateToObject: Update Mask %d:%d, pet_uid %d, owner_id %d"), petData.GetUpdateMask().low, petData.GetUpdateMask().high, GetPet_uid(), GetOwner_id());

			PROCEDURE_STORE_VALUE1(RPCMask, GetModifyMask())
			ClearModifyMask();
			// update to object service
			ObjectService_Proxy::UpdatePet(gGlobal.pObjectService->pPeer, context, 
				GetPet_uid(), &petData);
		}
		else
		{
			// no need update
			PROCEDURE_RETURN
		}

	PROCEDURE_WAIT(1, ObjectService, CB_UpdatePet)

		// unmask update mask
		PROCEDURE_RESTORE_VALUE1(RPCMask, oldMask)
		UnmarkModify(oldMask);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPet UpdateToObject Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

		return RPC_RESULT_OK;
}*/

void CPet::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	/*case EVT_UPDATE_OBJECT:				// update all partner to object service
		if (m_pOwnerGroup) {
			PROCEDURE_DO_METHOD(CPet, UpdateToObject, this, m_pOwnerGroup->GetOwnerContext())
		}
		break;*/
	case EVT_UPDATE_PET_INFO:
		{
			UpdateInfo();
			RaiseUniqEventID(EVT_UPDATE_PET_INFO, gConfigure.petUpdateInterval);
			break;
		}
	case EVT_UPDATE_PET_EXPIRE:
		{
			if (m_pPetData && m_pPetData->pet_TimeLimitType != PET_TIME_LIMIT_TYPE_NONE &&
				GetExpireDate() != 0)
			{
				if (m_pOwnerGroup == NULL || m_pOwnerGroup->GetOwner() == NULL || m_pOwnerGroup->GetOwner()->GetPeer() == NULL)
				{
					//wait for valid peer
					RaiseUniqEventID(EVT_UPDATE_PET_EXPIRE, 1000);
				}
				else
				{
					UInt32 now = (UInt32) time(NULL);
					if (GetExpireDate() <= now )
					{
						RPCContext context = m_pOwnerGroup->GetOwnerContext();
						LPCPEER pPeer = m_pOwnerGroup->GetOwner()->GetPeer();
						PROCEDURE_DO_RPC1(PetGroup, DeletePet, m_pOwnerGroup,  pPeer, context, UInt32, GetPet_uid())
					}
					else
						RaiseUniqEventID(EVT_UPDATE_PET_EXPIRE, (GetExpireDate() - (UInt32) now) * 1000);
				}
			}
		}
	case EVT_UPDATE_PET_VP: 
		{
			if (m_pPetData && m_pPetData->pet_type == 1 && GetExpireDate() == 0)
			{
				if (m_pOwnerGroup == NULL || m_pOwnerGroup->GetOwner() == NULL || m_pOwnerGroup->GetOwner()->GetPeer() == NULL)
				{
					//wait for valid peer
					RaiseUniqEventID(EVT_UPDATE_PET_VP, 1000);
				}
				else
				{
					if(GetState() == PET_STATE_TYPE_USE)
					{
						if (GetVP() > 0)
						{
							int newVp = GetVP() - (gGlobal.m_rand.GetRandom(m_pPetData->pet_reduce_vp_max - m_pPetData->pet_reduce_vp_min + 1) + m_pPetData->pet_reduce_vp_min);
							if (newVp < 0)
								newVp = 0;
							SetVP(newVp);
							m_newUsetime = (UInt32) time(NULL);
							TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CPet EVT_UPDATE_PET_VP : Change to New VP = %d"),
								newVp);
							m_useTime = 0;
							RaiseUpdate();
							RaiseUniqEventID(EVT_UPDATE_PET_VP,USE_NOTIME_PET_CHANGE_VP_TIME);
						}
						else
						{
							m_pOwnerGroup->ChangePetState(m_pOwnerGroup->GetOwner()->GetPeer(), m_pOwnerGroup->GetOwner()->GetContext(),
								GetPet_uid(),PET_STATE_TYPE_REST);
							RaiseUpdate();
						}
					}
					else
						RaiseUniqEventID(EVT_UPDATE_PET_VP, 1000);
				}
			}
		}
	case EVT_UPDATE_PET_FAVOR :
		{
			if (m_pPetData && m_pPetData->pet_type == 0)
			{
				if (m_pOwnerGroup == NULL || m_pOwnerGroup->GetOwner() == NULL || m_pOwnerGroup->GetOwner()->GetPeer() == NULL)
				{
					//wait for valid peer
					RaiseUniqEventID(EVT_UPDATE_PET_FAVOR, 1000);
				}
				else
				{
					if(GetState() == PET_STATE_TYPE_USE)
					{
						if (GetHappiness() > 1)
						{
							UInt32 favor = GetHappiness() - 2;
							SetHappiness(favor);
							m_newUsePetTwoTime = (UInt32) time(NULL);
							TRACE_ERRORDTL_1(GLOBAL_LOGGER, _T("CPet EVT_UPDATE_PET_VP : Change to New favor = %d"),
								favor);
							m_usePetTwoTime = 0;
							RaiseUpdate();
							RaiseUniqEventID(EVT_UPDATE_PET_FAVOR,USE_NOTIME_PET_CHANGE_FAVOR_TIME);
						}
						else
						{
							m_pOwnerGroup->ChangePetState(m_pOwnerGroup->GetOwner()->GetPeer(), m_pOwnerGroup->GetOwner()->GetContext(),
								GetPet_uid(),PET_STATE_TYPE_REST);
							RaiseUpdate();
						}
					}
					else
						RaiseUniqEventID(EVT_UPDATE_PET_FAVOR, 1000);
				}
			}
		}
	}
}

void CPet::SetData(CPetData *pData)				// set pet by CPetData
{
	if (pData->IsUpdated(CPetData::idMask))
	{
		SetId(pData->GetPet_uid(), pData->GetPet_id());
		m_pSkillGroup->SetOwner(pData->GetPet_uid(), SKILL_OWNER_TYPE_PET);
		m_pPetData = gGlobal.m_resource->GetPetData(pData->GetPet_id());
	}
		
	if (pData->IsUpdated(CPetData::owner_idMask))
		SetOwner_id(pData->GetOwner_id());

	if (pData->IsUpdated(CPetData::nameMask))
		SetName(pData->GetName());

	if (pData->IsUpdated(CPetData::sexMask))
		SetSex(pData->GetSex());

	if (pData->IsUpdated(CPetData::levelMask))
		SetLevel(pData->GetLevel());

	if (pData->IsUpdated(CPetData::expMask))
		SetExp(pData->GetExp());

	if (pData->IsUpdated(CPetData::stateMask))
		SetState(pData->GetState());

	if (pData->IsUpdated(CPetData::birthCountMask))
		SetBirthCount(pData->GetBirthCount());

	if (pData->IsUpdated(CPetData::firstMasterMask))
		SetFirstMaster(pData->GetFirstMaster());

	if (pData->IsUpdated(CPetData::fatherMask))
		SetFather(pData->GetFather_id(), pData->GetFather_name());

	if (pData->IsUpdated(CPetData::motherMask))
		SetMother(pData->GetMother_id(), pData->GetMother_name());

	if (pData->IsUpdated(CPetData::happinessMask))
		SetHappiness(pData->GetHappiness());

	if (pData->IsUpdated(CPetData::VPMask))
		SetVP(pData->GetVP(), pData->GetMaxVP());

	if (pData->IsUpdated(CPetData::elementMask))
		SetElement(pData->GetElement());

	if (pData->IsUpdated(CPetData::spiritMask))
	{
		SetSpirit(pData->GetSpirit());
	}

	if (pData->IsUpdated(CPetData::expireDateMask))
	{
		SetExpireDate(pData->GetExpireDate());

		if (m_pPetData && m_pPetData->pet_TimeLimitType != PET_TIME_LIMIT_TYPE_NONE &&
			GetExpireDate() != 0)
		{
			UInt32 now = (UInt32) time(NULL);
			if (GetExpireDate() > now)
				RaiseUniqEventID(EVT_UPDATE_PET_EXPIRE, (GetExpireDate() - now) * 1000);
			else
				RaiseUniqEventID(EVT_UPDATE_PET_EXPIRE);
		}
	}

	if (pData->IsUpdated(CPetData::soulboundMask))
		SetSoulbound(pData->GetSoulbound());

	if (pData->IsUpdated(CPetData::colorMask))
		SetColor(pData->GetColor());

	if (pData->IsUpdated(CPetData::deputeMask))
		SetDepute(pData->GetDepute_master(), pData->GetDepute_time(), pData->GetDepute_useTime(), pData->GetDepute_exp());
	if (pData->IsUpdated(CPetData::inWareHouseMask))
		SetInWareHouse(pData->GetInWareHouse());
	if (pData->IsUpdated(CPetData::assetLockMask))
		SetLock(pData->GetLock());
	ClearAllMask();
}

void CPet::PreSend(NetGroup* skillGroup)
{
	RPCMask mask;
	mask.MaskBit(CPet::AllMask);
	SetSendMask(mask);
	m_pSkillGroup->PreSend();
	if (skillGroup)
		skillGroup->Add(m_pSkillGroup);
}
RPCResult CPet::DoLearnSkill(RPCContext &context)
{
	PROCEDURE_START_METHOD3(CPet, DoLearnSkill, this, context, UInt16Vector, skill_ids, 
							UInt8, Level, Boolean, forNewPet)

		if (skill_ids.empty()) {
			PROCEDURE_RETURN
		} else {
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("DoLearnSkill: sessionId %s, pet_uid %d, skill_ids %s"),
				context.sessionId.GetString().c_str(), GetPet_uid(), 
				GetVectorString(skill_ids).c_str());
		}

		UInt32Vector owner_ids;
		UInt16Vector skill_levels;
		owner_ids.resize(skill_ids.size());
		skill_levels.resize(skill_ids.size());
		std::fill_n(owner_ids.begin(), skill_ids.size(), GetPet_uid());
		std::fill_n(skill_levels.begin(), skill_ids.size(), Level);
		ObjectService_Proxy::CreateSkillList(gGlobal.pObjectService->pPeer, context, 
			SKILL_OWNER_TYPE_PET, owner_ids, skill_ids, skill_levels);
	
	PROCEDURE_WAIT1(1, ObjectService, CB_GetSkillList, NetGroup*, skillDataGroup)

		TRACE_ENSURE(skillDataGroup);

		NetGroup grp;
		//bool calAttri = false;

		for (Int i = 0; i < skillDataGroup->GetCount(); i++)
		{
			CSkillData * pSkillData = skillDataGroup->GetChild<CSkillData >(i);
			if (pSkillData )
			{
				const SkillData * pSkillDataStr = gGlobal.m_resource->GetSkillData(pSkillData->GetSkill_id());
				if (pSkillDataStr)
				{
					CSkill * pSkill = CSkill::NewInstance();
					pSkill->SetData(pSkillData);
					pSkill->PreSend();
					
					m_pSkillGroup->Add(pSkill);
					grp.Add(pSkill);
					m_skillMap.insert(std::make_pair(pSkill->GetSkill_id(), pSkill));
					
					//calAttri = true;
				}	
			}
		}

		DeleteNetGroup(skillDataGroup, BOOLEAN_TRUE);
		if (m_pOwnerGroup && m_pOwnerGroup->GetOwner())
			m_pSkillGroup->CB_AddSkills(m_pOwnerGroup->GetOwner()->GetPeer(), 
				m_pOwnerGroup->GetOwner()->GetContext(), &grp);

		if (IsUpdated())
			RaiseUpdate();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPet DoLearnSkill Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CPet::DoRemoveSkill(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CPet, DoRemoveSkill, this, context, UInt16Vector, skill_ids)

		if (skill_ids.empty()) {
			PROCEDURE_RETURN
		} else {
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("DoRemoveSkill: sessionId %s, pet_uid %d, skill_ids %s"),
				context.sessionId.GetString().c_str(), GetPet_id(), 
				GetVectorString(skill_ids).c_str());
		}

		ObjectService_Proxy::DeleteSkillList(gGlobal.pObjectService->pPeer, context, 
			GetPet_uid(), skill_ids, SKILL_OWNER_TYPE_PET);

	PROCEDURE_WAIT(1, ObjectService, CB_DeleteSKillDone)

		for (UInt16Vector::const_iterator it = skill_ids.begin(); it != skill_ids.end(); ++it)
		{
			for (UInt16 i = 0; i < m_pSkillGroup->GetCount(); ++i)
			{
				CSkill * pChild = m_pSkillGroup->GetChild<CSkill>(i);
				if (pChild && pChild->GetSkill_id() == *it)
				{
					m_pSkillGroup->Remove(pChild);
					DeleteNetObject(pChild);
					break;
				}
			}

			SkillMap::iterator Itr = m_skillMap.find(*it);
			if (Itr != m_skillMap.end())
			{
				m_skillMap.erase(Itr);
			}
		}

		if (m_pOwnerGroup && m_pOwnerGroup->GetOwner())
			m_pSkillGroup->CB_RemoveSkills(m_pOwnerGroup->GetOwner()->GetPeer(), 
				m_pOwnerGroup->GetOwner()->GetContext(), skill_ids);

		if (IsUpdated())
			RaiseUpdate();


	PROCEDURE_CATCH

	PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPet DoLearnSkill Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}

RPCResult CPet::ChangePetName(LPCPEER pPeer, RPCContext &context, const String &newName)
{
	SetName(newName);
	if (m_pOwnerGroup && m_pOwnerGroup->GetOwner() && m_pOwnerGroup->GetOwner()->GetPet_uid() == GetPet_uid())
	{
		m_pOwnerGroup->GetOwner()->SetPet(GetPet_uid(), GetPet_id(), GetName(), GetColor());
	}
	RaiseUpdate();
	return RPC_RESULT_OK;
}

NetGroup* CPet::GetSkillGroup(bool forSend)
{
	if (forSend)
	{
		m_pSkillGroup->PreSend();
		for (UInt16 i = 0; i < m_pSkillGroup->GetCount(); ++i)
		{
			CSkill * pSkill = m_pSkillGroup->GetChild<CSkill>(i);
			if (pSkill)
				pSkill->PreSend();
		}
	}

	return m_pSkillGroup;
}

void CPet::MakeLearnSkillLst(bool forNewPet, bool toObject)
{
	m_LearnSkillID.clear();
	UInt8 existNormMobSkill = 0;
	UInt8 maxNormMobSkill = 8;
	
	if (m_pPetData == NULL)
		m_pPetData = gGlobal.m_resource->GetPetData(GetPet_id());

	if (m_pPetData == NULL)
		return;

	StringA mobSkillFile;
	mobSkillFile.Format("PetSkill%05d.csv",m_pPetData->pet_SkillList); 
	const MobSkillCombination* mobSkillLst = gGlobal.m_resource->GetMobSkillCombination(mobSkillFile.c_str());
	if (mobSkillLst == NULL)
		return;

	if (mobSkillLst && m_pPetData)
	{
		if (forNewPet)
		{
			m_iNewSkillID = 0;
			for (UInt8 i = 0; i < mobSkillLst->skillLst.size(); ++i)
			{
				UInt32 rand = (UInt32)(GetRand(0.0f, 100.0f) * 1000);
				MobSkillDef* pDef = mobSkillLst->skillLst[i];
				if (pDef && rand <= pDef->skillRate)
				{
						m_LearnSkillID.push_back(pDef->skillId);
						++existNormMobSkill;
					
				}
			}
		}
		else
		{
			// 1000 = 1%
			UInt32 iRand = (UInt32)GetRand(0.0f, 100.0f) * 1000;
			UInt32 successRate =  (UInt32)GetPrivateProfileInt(_T("LoadLearnSkill"), _T("SuccessRate"), 5000, CONFIGURATION_INI_FILE);
			if (iRand <= successRate)
			{
				{
					for (UInt8 i = 0; i < mobSkillLst->skillLst.size(); ++i)
					{
						UInt32 rand = (UInt32)(GetRand(0.0f, 100.0f) * 1000);
						MobSkillDef* pDef = mobSkillLst->skillLst[i];
						if (pDef && rand <= pDef->skillRate)
						{
							m_iNewSkillID = pDef->skillId;
							if (m_skillMap.find(pDef->skillId) != m_skillMap.end())
								break;

							if (existNormMobSkill < maxNormMobSkill) 
							{
								m_LearnSkillID.push_back(pDef->skillId);
								++existNormMobSkill;
								break;
							}	
						}
					}
				}
			}
		}	
	}

	if (toObject && m_pOwnerGroup && !m_LearnSkillID.empty()) {
		PROCEDURE_DO_METHOD3(CPet, DoLearnSkill, this, m_pOwnerGroup->GetOwnerContext(),
			UInt16Vector, m_LearnSkillID, UInt8, 1, bool, true)
		m_LearnSkillID.clear();
	}
}

Boolean CPet::AddSkill(CSkill* pSkill)
{
	// no skill or skill data
	if (pSkill == NULL || pSkill->m_data == NULL)
		return BOOLEAN_FALSE;

	m_pSkillGroup->Add(pSkill);
	SetHasAddSkill(true);
	m_skillMap.insert(std::make_pair(pSkill->GetSkill_id(), pSkill));

	return BOOLEAN_TRUE;
}

void CPet::WriteData(CPetData *pData)			// write pet to CPetData
{
	TRACE_ENSURE(pData);

	pData->ClearUpdateMask();
	if (IsModified(idMask))
		pData->SetId(GetPet_uid(), GetPet_id());
		
	if (IsModified(owner_idMask))
		pData->SetOwner_id(GetOwner_id());

	if (IsModified(nameMask))
		pData->SetName(GetName());

	if (IsModified(sexMask))
		pData->SetSex(GetSex());

	if (IsModified(levelMask))
		pData->SetLevel(GetLevel());

	if (IsModified(expMask))
		pData->SetExp(GetExp());

	if (IsModified(stateMask))
		pData->SetState(GetState());

	if (IsModified(birthCountMask))
		pData->SetBirthCount(GetBirthCount());

	if (IsModified(firstMasterMask))
		pData->SetFirstMaster(GetFirstMaster());

	if (IsModified(fatherMask))
		pData->SetFather(GetFather_id(), GetFather_name());

	if (IsModified(motherMask))
		pData->SetMother(GetMother_id(), GetMother_name());

	if (IsModified(happinessMask))
		pData->SetHappiness(GetHappiness());

	if (IsModified(VPMask))
		pData->SetVP(GetVP(), GetMaxVP());

	if (IsModified(elementMask))
		pData->SetElement(GetElement());

	if (IsModified(spiritMask))
		pData->SetSpirit(GetSpirit());

	if (IsModified(expireDateMask))
		pData->SetExpireDate(GetExpireDate());

	if (IsModified(soulboundMask))
		pData->SetSoulbound(GetSoulbound());

	if (IsModified(colorMask))
		pData->SetColor(GetColor());

	if (IsModified(deputeMask))
		pData->SetDepute(GetDepute_master(), GetDepute_time(), GetDepute_useTime(), 
			GetDepute_exp());
	if (IsModified(inWareHouseMask))
		pData->SetInWareHouse(GetInWareHouse());

	if (IsModified(assetLockMask))
		pData->SetLock(GetLock());

	if (pData->IsUpdated()) {
		pData->SetId(GetPet_uid(), GetPet_id());
		pData->SetSendMask(pData->GetUpdateMask());
	}
}

void CPet::SetPetGroup(CPetGroup* ownerGroup)
{
	// unset original ownership
	if (m_pOwnerGroup && m_pOwnerGroup != ownerGroup)
	{
		UnsetOwnObject(m_pOwnerGroup->GetOwnerContext().sessionId, this);
		UnsetOwnObject(m_pOwnerGroup->GetOwnerContext().sessionId, m_pSkillGroup);
	}

	// set new ownership
	m_pOwnerGroup = ownerGroup;
	if (ownerGroup) {
		SetOwnObject(m_pOwnerGroup->GetOwnerContext().sessionId, this);
		SetOwnObject(m_pOwnerGroup->GetOwnerContext().sessionId, m_pSkillGroup);
	}
}

void CPet::ChangeLevel(Int change)
{
	SetLevel(MAX((Int) GetLevel() + change, 0));
	RaiseUpdate();
}

void CPet::ChangeHappiness(Int change)
{
	SetHappiness(WITHIN((Int) GetHappiness() + change, 0, MAX_PET_HAPPINESS));
	RaiseUpdate();
}

void CPet::ChangeSpirit(Int change)
{
	SetSpirit(MAX((Int) GetSpirit() + change, 0));
	RaiseUpdate();
}

void CPet::ChangeLv(Int change)
{
	SetLevel(MAX((Int) GetLevel() + change, 0));
	RaiseUpdate();
}

void CPet::ChangeVP(Int change)
{
	SetVP(WITHIN((Int) GetVP() + change, 0, GetMaxVP()));
	RaiseUpdate();
}

void CPet::ChangeExp(Int change)
{
	if (change < 0) {
		SetExp(MAX((Int) GetVP() + change, 0));
	} else {
		UInt32 nextExp;
		while (change > 0) {
			//nextExp = 5 * GetLevel() * (GetLevel()  + 1) + 100;
			nextExp = 80;
			if (GetExp() + change > nextExp) {
				SetExp(0);
				SetLevel(GetLevel() + 1);
				change -= (nextExp - GetExp());
			} else {
				SetExp(GetExp() + change);
				change = 0;
			}
		}
	}
	if (GetLevel() >= 10)
	{
		SetExp(0);
		SetLevel(10);
		m_pOwnerGroup->GetOwner()->CB_HawkMessage(m_pOwnerGroup->GetOwner()->GetPeer(), 
			m_pOwnerGroup->GetOwner()->GetContext(),HAWK_MIXPET_LEVEL_MAX);
	}
	RaiseUpdate();
}

RPCResult CPet::RegenSkill(RPCContext &context)
{
	PROCEDURE_START_METHOD(CPet, RegenSkill, this, context)

		UInt16Vector skill_ids;

		for (UInt8 i = 0; i < m_pSkillGroup->GetCount(); ++i)
		{
			CSkill* pSkill = m_pSkillGroup->GetChild<CSkill>(i);
			if (pSkill)
				skill_ids.push_back(pSkill->GetSkill_id());
		}

		if (skill_ids.size() > 0)
		{
			PROCEDURE_DO_METHOD1(CPet, DoRemoveSkill, this, context, UInt16Vector, skill_ids)
		}
		else
		{
			PROCEDURE_GO_STEP(1)
		}
		
	PROCEDURE_WAIT_RETURN(1)

		MakeLearnSkillLst(true);

		UInt8 level = 1;
		bool forNewPet = true;

		if (GetLearnSkillLst().size() > 0)
		{
			PROCEDURE_DO_METHOD3(CPet, DoLearnSkill, this, context, UInt16Vector, GetLearnSkillLst(), UInt8, level, bool, forNewPet)
			ClearLearnSkillLst();
		}
		else
		{
			ClearLearnSkillLst();
			PROCEDURE_GO_STEP(2)
		}

	PROCEDURE_WAIT_RETURN(2)

	PROCEDURE_CATCH

		//PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		//TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPet UpdateToObject Error: ObjectService:CB_Error, errcode %d, %s"),
		//err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;

}

void CPet::ResetPet()
{	
	ResetLevel(1);
	if (m_pOwnerGroup && m_pOwnerGroup->GetOwner()) {
		PROCEDURE_DO_METHOD(CPet, RegenSkill, this, m_pOwnerGroup->GetOwner()->GetContext())
	}
}

void CPet::ResetLevel(UInt16 value)
{
	this->SetLevel(value);
}

bool CPet::HasSkill(UInt16 skill_id)
{
	SkillMap::iterator Itr = m_skillMap.find(skill_id);
	if (Itr != m_skillMap.end())
		return true;
	return false;
}

void CPet::ClonePet(CPet* pPet)
{
	pPet->SetId(GetPet_uid(), GetPet_id());
	pPet->SetOwner_id(GetOwner_id());
	pPet->SetSex(GetSex());
	pPet->SetName(GetName());
	pPet->SetLevel(GetLevel());
	pPet->SetExp(GetExp());
	pPet->SetFather(GetFather_id(), GetFather_name());
	pPet->SetMother(GetMother_id(), GetMother_name());
	pPet->SetBirthCount(GetBirthCount());
	pPet->SetFirstMaster(GetFirstMaster());
	pPet->SetHappiness(GetHappiness());
	pPet->SetVP(GetVP(), GetMaxVP());
	pPet->SetElement(GetElement());
	pPet->SetExpireDate(GetExpireDate());
	pPet->SetSoulbound(GetSoulbound());
	pPet->SetColor(GetColor());
	pPet->SetSpirit(GetSpirit());
	pPet->SetDepute(GetDepute_master(), GetDepute_time(), GetDepute_useTime(),
		GetDepute_exp());
	pPet->SetInWareHouse(GetInWareHouse());
	pPet->SetLock(GetLock());
}

void CPet::RaiseUpdate()
{
	if (m_pOwnerGroup)
		m_pOwnerGroup->RaiseUpdate();
}

void CPet::CancelUpdate()
{
	if (m_pOwnerGroup)
		m_pOwnerGroup->CancelUpdate();
}

RPCContext &CPet::GetOwnerContext()
{
	static RPCContext dummyContext;

	return (m_pOwnerGroup ? m_pOwnerGroup->GetOwnerContext() : dummyContext);
}


UInt8 CPet::GetPetType()
{
	if (m_pPetData)
		return m_pPetData->pet_type;
	return -1;
}

UInt8 CPet::GetPetTimeLimitType()
{
	if (m_pPetData)
		return m_pPetData->pet_TimeLimitType;
	return -1;
}


RPCResult CPet::ChangePetExp(LPCPEER pPeer, RPCContext &context, const UInt32 &addExp)
{
	if(m_pPetData)
	{
		ChangeExp(addExp);
		RaiseUpdate();
	}
	return RPC_RESULT_OK;
}

RPCResult CPet::ChangePetVp(LPCPEER pPeer, RPCContext &context, const int &addVp)
{	
	if(m_pPetData && m_pPetData->pet_type == 1 && !m_pOwnerGroup->GetOwner()->IsInBattle())
	{
		SetVP(GetVP()+addVp);
		RaiseUpdate();
	}
	return RPC_RESULT_OK;
}

RPCResult CPet::ChangePetFavor(LPCPEER pPeer, RPCContext &context, const int &addFavor)
{
	if(m_pPetData && m_pPetData->pet_type == 0)
	{
		SetHappiness(GetHappiness()+addFavor);
		RaiseUpdate();
	}
	return RPC_RESULT_OK;
}

void CPet::RemoveSkill(UInt16 skillId)
{
	SkillMap::iterator Itr = m_skillMap.find(skillId);
	if (Itr != m_skillMap.end())
	{
		UInt16Vector IdLst;
		gGlobal.m_resource->GetSkillLink(skillId, IdLst);

		PROCEDURE_DO_METHOD1(CPet, DoRemoveSkill, this, m_pOwnerGroup->GetOwner()->GetContext(), UInt16Vector, IdLst)
	}
}

void CPet::AddSkill(UInt16 skillId)
{
	if (m_pSkillGroup == NULL)
		return;

	// check if it is a partner skill
	const SkillData* skillData = gGlobal.m_resource->GetSkillData(skillId);

	if (skillData == NULL)
		return;

	if (skillData->type != PET_SPEED_SKILL && skillData->type != PET_NORMAL_SKILL)
		return;

	SkillMap::iterator itr = m_skillMap.find(skillId);
	if (itr == m_skillMap.end())
	{
		UInt16Vector idLst;
		idLst.push_back(skillId);
		PROCEDURE_DO_METHOD3(CPet, DoLearnSkill, this, 
			m_pSkillGroup->GetOwnerContext(), UInt16Vector, idLst, UInt8, 1, 
			Boolean, BOOLEAN_FALSE)
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CPet AddSkill Warning : skill %d exist"),
			skillId);
	}
}


UInt16 CPet::GetRandomSkillID()
{
	if (m_skillMap.size() > 1)
	{	
		UInt16Vector NormalSkillID;
		for (SkillMap::iterator itr = m_skillMap.begin(); itr != m_skillMap.end(); itr++)
		{
			const SkillData* skillData = gGlobal.m_resource->GetSkillData(itr->first);
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _T("CPet  GetRandomSkillID :add normal skill id =  %d"),itr->first);	
			if (skillData && skillData->type == PET_NORMAL_SKILL)
				NormalSkillID.push_back(itr->first);
		}
		if (NormalSkillID.size() > 0)
		{
			UInt32 r = gGlobal.m_rand.GetRandom((Int)NormalSkillID.size());
			for (int Count = 0; Count < NormalSkillID.size(); Count++)
			{
				if (Count == r)
				{
					TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CPet  GetRandomSkillID :m_rand =  %d"),NormalSkillID[Count]);					
					return NormalSkillID[Count];
				}
			}
		}
	}
	return 0;
}

UInt8 CPet::GetSkillCount()
{
	return m_skillMap.size();
}

Boolean CPet::GetPetState()
{
	TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("GetPetState :state =  %d"),GetState());					
	return GetState();
}