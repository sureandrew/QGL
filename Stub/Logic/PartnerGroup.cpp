//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/ChannelType.h"
#include "Common/Procedure.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Stub/Logic/PartnerGroup.h"
//-- Library
#include "Proxy/Service/ObjectService.h"
#include "Proxy/Data/SkillData.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/SkillGroup.h"
#include "Stub/Logic/ScriptSystem.h"
#include "MapService/Global.h"
#include "Resource/ResMob.h"
#include "Proxy/Service/LogService.h"



#include "Stub/Logic/Item.h"
#include "Stub/Logic/ItemBag.h"
#include "Resource/GameStringTable.h"
#include "Proxy/Service/MessageService_proxy.h"
#include "Resource/ServerResManager.h"

REGISTER_CLASSTYPE(CPartnerGroup, PartnerGroup_CLASSID);
#define DUMMY_PARTNER_ADDRESS (reinterpret_cast<CPartner*>(1))

CPartnerGroup::CPartnerGroup()
{
	m_pOwner = NULL;
	m_wareHousePartners.clear();
}

CPartnerGroup::~CPartnerGroup()
{
	for (Index i = C_INDEX(0); i < m_delGroup.GetCount(); ++i)
	{
		CPartner* partner = m_delGroup.GetChild<CPartner>(i);
		if (partner)
			DeleteNetObject(partner);
	}

	for (Index i = C_INDEX(0); i < m_newPartners.GetCount(); ++i)
	{
		CPartner* partner = m_newPartners.GetChild<CPartner>(i);
		if (partner)
			DeleteNetObject(partner);
	}
	ClearWareHouse();
	DeleteNetChilds(this);
}

RPCContext& CPartnerGroup::GetOwnerContext()
{
	static RPCContext dummyContext;
	return (m_pOwner ? m_pOwner->GetContext() : dummyContext);
}

Boolean CPartnerGroup::Add(NetObject *obj)
{
	if (obj == NULL || obj->GetClassID() != Partner_CLASSID)
		return BOOLEAN_FALSE;

	if (Parent::Add(obj)) 
	{
		CPartner* pPartner = CastNetObject<CPartner>(obj);
		if (pPartner) 
		{
			pPartner->SetPartnerGroup(this);
			return BOOLEAN_TRUE;
		}
	}
	
	return BOOLEAN_FALSE;
}

Boolean CPartnerGroup::Remove(NetObject *obj)
{
	if (obj == NULL || obj->GetClassID() != Partner_CLASSID)
		return BOOLEAN_FALSE;

	CPartner* pPartner = CastNetObject<CPartner>(obj);
	if (pPartner) 
	{
		if (pPartner->GetInWareHouse() == 0)
		{
			if (Parent::Remove(obj)) 
			{		
				pPartner->SetPartnerGroup(NULL);
				return BOOLEAN_TRUE;
			}
		}
		else
		{
			RPCNetIDVector::iterator itr = remove(m_wareHousePartners.begin(), m_wareHousePartners.end(), pPartner->GetNetId());
			if (itr != m_wareHousePartners.end())
				m_wareHousePartners.erase(itr);
			return BOOLEAN_TRUE;
		}
	}	
	return BOOLEAN_FALSE;
}

void CPartnerGroup::AddPartner(UInt32 mob_id, UInt8 level)
{
	if (m_pOwner) {
		PROCEDURE_DO_METHOD2(CPartnerGroup, AddPartner, this, m_pOwner->GetContext(), UInt32, mob_id, UInt8, level)
	}
}

Boolean CPartnerGroup::RemovePartner(UInt32 mob_id)
{
	return RemovePartner(GetPartnerByMobId(mob_id));
}

Boolean CPartnerGroup::RemovePartnerByUID(UInt32 partner_id)
{
	return RemovePartner(GetPartnerByID(partner_id));
}

Boolean CPartnerGroup::RemovePartner(CPartner* partner)
{
	if (partner) {
		partner->SetOwner(0);				// set no owner mean removed
		m_delGroup.Add(partner);
		if (m_pOwner)
		{
			if (m_pOwner->GetPartner_id() == partner->GetPartner_id())
			{
				m_pOwner->SetPartner(0, 0, _T(""), 0);
				m_pOwner->RaiseUpdate();
			}
			if (m_pOwner->GetPeer())
			{
				UInt32Vector partnerIDs;
				partnerIDs.push_back(partner->GetPartner_id());
				CB_DeleteListSuccess(m_pOwner->GetPeer(), m_pOwner->GetContext(), partnerIDs, FALSE);
			}
		}
			
		Remove(partner);
		RaiseUpdate();
		return BOOLEAN_TRUE;
	} else {
		return BOOLEAN_FALSE;
	}
}

// load partners from object service
RPCResult CPartnerGroup::LoadPartners(RPCContext &context)
{
	if (m_pOwner == NULL)
	{
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD(CPartnerGroup, LoadPartners, this, context)

		// get partner data from object service
		ObjectService_Proxy::GetPartnersByCharID(gGlobal.pObjectService->pPeer, context, 
			m_pOwner->GetChar_id(), 0, CPartnerData::DETAIL_MAP);

	PROCEDURE_WAIT2(1, ObjectService, CB_GetPartnerGroup, NetGroup*, partnerGroup, BYTE, detail)

		if (partnerGroup && partnerGroup->GetCount() > 0)
		{
			RPCMask mask;
			mask.MaskBit(CPartner::AllMask);
			CPartnerData* pPartnerData;
			CPartner* pPartner;

			for (int i = 0; i < partnerGroup->GetCount(); ++i)
			{
				pPartnerData = partnerGroup->GetChild<CPartnerData>(i);
				if (pPartnerData == NULL) continue;

				//try to check duplicate partner here
				bool continueLoop = false;
				for (int j = 0; j < GetCount(); ++j)
				{
					CPartner* pPartnerExist = GetChild<CPartner>(j);
					if (pPartnerExist && pPartnerExist->GetPartner_id() == pPartnerData->GetPartner_id())
					{
						TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("duplicate partner found  partner_id %d"),
						pPartnerExist->GetPartner_id());
						continueLoop = true;

					}
				}

				if (continueLoop)
					continue;

				pPartner = CPartner::NewInstance();
				pPartner->SetData(pPartnerData);
				pPartner->ExtractBuffString();
				pPartner->CalcAttrib();
				pPartner->ClearAllMask();
				pPartner->SetSendMask(mask);
				Add(pPartner);

				/*
				// partner collection buff, shouldn't here
				const MobData* pMobData = gGlobal.m_resource->GetMobData(pPartnerData->GetMob_id());
				if (pMobData)
				{
					const StlVector<UInt16>* pBuffList = 
						m_pOwner->GetPartnerCollectionBuffByRace(pMobData->race);
					if (pBuffList)
					{
						for (UInt16 j=0; j<pBuffList->size(); j++)
						{
							pPartner->AddBuff((*pBuffList)[j],0,0,0,99999,0);
						}
					}
				}
				*/
			}
		}
		DeleteNetGroup(partnerGroup, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
			err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::AddPartner(RPCContext &context)
{
	PROCEDURE_START_METHOD2(CPartnerGroup, AddPartner, this, context , UInt32, mob_id, UInt8, level)

		TRACE_ENSURE(m_pOwner);
		// fix remove and add partner at the same time
		UInt16 count = 0;
		CPartner* partner;
		for (UInt16 i = 0; i < GetCount(); i++)
		{
			partner = GetChild<CPartner>(i);
			if (partner && partner->GetOwner_id() == m_pOwner->GetChar_id())
				++count;
		}
		if (count >= m_pOwner->GetMaxPartner())
		{
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("AddPartner Warning: char_id %d, excess partner max %d"), 
				m_pOwner->GetChar_id(), m_pOwner->GetMaxPartner());
			//CB_Error(pchar->GetPeer(), context, 1, _T("SyncPartner Error: character netId not found "));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

		ObjectService_Proxy::CreatePartner(gGlobal.pObjectService->pPeer, context, mob_id, level, m_pOwner->GetChar_id(), CPartnerData::DETAIL_MAP);

	PROCEDURE_WAIT1(1, ObjectService, CB_CreatePartnerSuccess, NetObject*, partner)

		CPartnerData* pPartnerData;
		CPartner* pPartner;

		pPartnerData = CastNetObject<CPartnerData>(partner);
		pPartner = CPartner::NewInstance();
		pPartner->SetData(pPartnerData);
		pPartner->SetPartnerGroup(this);
		pPartner->CalcAttrib();
		pPartner->SetHP(pPartner->GetMaxHP(), pPartner->GetMaxHP());
		pPartner->SetSP(pPartner->GetMaxSP(), pPartner->GetMaxSP());
		pPartner->ClearAllMask();
		pPartner->PreSend();
		Add(pPartner);

		LogService_Proxy::LogPartner(gGlobal.pLogService->pPeer, context, pPartner->GetPartner_id(), 
			pPartner->GetMob_id(), pPartner->GetOwner_id(), LogService_Proxy::Partner::PARTNER_CREATE);

		if (m_pOwner && m_pOwner->GetPeer())
			CB_AddPartner(m_pOwner->GetPeer(), m_pOwner->GetContext(), pPartner, 
				pPartner->GetSkillGroup());

		pPartner->MakeLearnSkillLst(true, false);

		UInt8 level = 1;
		bool forNewPartner = true;
		PROCEDURE_DO_METHOD3(CPartner, DoLearnSkill, pPartner, context, 
			UInt16Vector, pPartner->GetLearnSkillLst(), UInt8, level, bool, forNewPartner)
			pPartner->ClearLearnSkillLst();
			//pPartner->ClearAllMask();

		DeleteNetObject(partner);

	PROCEDURE_WAIT_RETURN(2)

		UpdatePartnerCollection();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("AddPartner Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::DeletePartner(LPCPEER pPeer, RPCContext &context, 
									   const UInt32 &partner_id)
{
	TRACE_ENSURE(m_pOwner);
	if (!m_pOwner->IsInMap()) {
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("DeletePartner Warning: [owner not in map] char_id %d, sessionId %s, partner_id %d"), 
			m_pOwner->GetChar_id(), context.sessionId.GetString().c_str(), partner_id);
		return RPC_RESULT_OK;
	}

	CPartner * partner = GetPartner(partner_id);
	if (partner == NULL)
	{
		partner = GetWareHousePartner(partner_id);
		if (partner == NULL)
			return RPC_RESULT_OK;
	}
	if (partner && partner->GetLock())
	{
		UInt32 now = (UInt32) time(NULL);
		if (partner->GetExpireDate() == 0 || partner->GetExpireDate() > now)
			return RPC_RESULT_OK;
	}

	if (partner)
	{
		UInt32 now = (UInt32) time(NULL);
		if (partner->GetExpireDate() != 0 && partner->GetExpireDate() < now)
			m_pOwner->CB_HawkMessage(m_pOwner->GetPeer(),m_pOwner->GetContext(), HAWK_PARTNER_REMOVE);
	}

	RemovePartner(partner);
	
	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::AddAllPartnerSkill(RPCContext &context)
{

	PROCEDURE_START_METHOD(CPartnerGroup, AddAllPartnerSkill, this, context )

		UInt32Vector partnerIDs;
		UInt16Vector skillIDs;
		UInt16Vector skillLvls;

		for (UInt8 i = 0; i < GetCount(); ++i)
		{
			CPartner* pPartner = GetChild<CPartner>(i);
			if (pPartner)
			{
				for (UInt8 j = 0; j < pPartner->GetLearnSkillLst().size(); ++j)
				{
					partnerIDs.push_back(pPartner->GetPartner_id());
					skillIDs.push_back(pPartner->GetLearnSkillLst()[j]);
					skillLvls.push_back(1);
				}
				pPartner->ClearLearnSkillLst();
			}
		}

		if (partnerIDs.size() > 0)
		{
			ObjectService_Proxy::CreateSkillList(gGlobal.pObjectService->pPeer, context,
				SKILL_OWNER_TYPE_PARTNER, partnerIDs, skillIDs, skillLvls);
		}
		else
		{
			PROCEDURE_RETURN
		}

	PROCEDURE_WAIT1(1, ObjectService,  CB_GetSkillList, NetGroup*, skillDataGroups)

			if (skillDataGroups->GetCount() > 0)
			{
				NetGroup skillGroup;
				SetAllPartnerSkills(skillDataGroups, &skillGroup, true);
				if (m_pOwner)
					CB_AddSkills(m_pOwner->GetPeer(), m_pOwner->GetContext(), &skillGroup);
			}

			DeleteNetGroup(skillDataGroups, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPartnerGroup AddAllPartnerSkill Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::ChangePartnerState(LPCPEER pPeer, RPCContext &context, 
											const UInt32 &partner_id, const UInt8 &state)
{
	TRACE_ENSURE(m_pOwner);

	CPartner* partner = GetPartner(partner_id);

	if (partner && partner->m_data && partner->m_data->req_lv <= m_pOwner->GetLevel() && 
		partner->GetLevel() <= m_pOwner->GetLevel() + 5 && m_pOwner->GetAction() != CCharacter::ST_BATTLE)
	{
		if (state == CPartner::ST_FIGHT) {
			// change all partner standby state if set fight state
			for (int i = 0; i < GetCount(); ++i)
			{
				partner = GetChild<CPartner>(i);
				if (partner == NULL) continue;
				if (partner->GetPartner_id() == partner_id)
				{
					partner->ChangeState(CPartner::ST_FIGHT);
					if (partner->GetExpireDate() == 0 && partner->m_data && partner->m_data->TimeLimitType == PARTNER_TIME_LIMIT_TYPE_USE)
					{    
						UInt32 now = (UInt32) time(NULL);
						UInt32 expire;
						expire = now + 3600 * 24 * partner->m_data->TimeLimit;
						partner->SetExpireDate(expire);
						partner->RaiseUpdate();

						partner->RaiseUniqEventID(CPartner::EVT_UPDATE_PARTNER_EXPIRE, (now - expire) * 1000);
					}
				} 
				else
				{
					partner->ChangeState(m_pOwner->GetAction() == CCharacter::ST_BATTLE ? 
						CPartner::ST_REST : CPartner::ST_STANDBY);
				}
			}
		} else if (state == CPartner::ST_STANDBY) {
				partner->ChangeState(CPartner::ST_STANDBY);
		}
	}

	return RPC_RESULT_OK;
}

void CPartnerGroup::SetAllFull(bool HP, bool SP, bool favor)
{
	for (UInt i = 0; i < GetCount(); ++i)
	{
		CPartner* partner = GetChild<CPartner>(i);
		if (partner)
			partner->SetFull(HP, SP, favor);
	}
	RaiseUpdate();
}

RPCResult CPartnerGroup::UpdateAllPartner(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CPartnerGroup, UpdateAllPartner, this, context, Boolean, bSkillUpdate)

 		NetGroup oldGrp;
		NetGroup newGrp;
		CPartnerData *pPartnerData;
		UInt32Vector partnerIDs;
		RPCNetIDVector delPartnerNetIDList;

		for (UInt16 i = 0; i < m_delGroup.GetCount(); ++i)
		{
			CPartner* partner = m_delGroup.GetChild<CPartner>(i);
			if (partner)
			{
				partnerIDs.push_back(partner->GetPartner_id());
				pPartnerData = CreateNetObject<CPartnerData>();
				TRACE_ENSURE(pPartnerData);
				partner->WriteData(pPartnerData);
				newGrp.Add(pPartnerData);
				delPartnerNetIDList.push_back(partner->GetNetId());
			}
		}

		for (Index i = C_INDEX(0); i < GetCount(); ++i)
		{
			CPartner* partner = GetChild<CPartner>(i);
			if (partner == NULL)
				continue;

			if (partner->GetState() == CPartner::ST_REST && m_pOwner->IsInMap() )
				partner->ChangeState(CPartner::ST_STANDBY);

			if (partner->IsUpdated()) {
				partner->CancelUpdate();
				partner->SetSendMask(partner->GetUpdateMask());
				partner->ClearUpdateMask();
				oldGrp.Add(partner);
			}
			if (partner->NeedUpdate())
			{
				pPartnerData = CreateNetObject<CPartnerData>();
				TRACE_ENSURE(pPartnerData);
				partner->WriteData(pPartnerData);
				partner->ClearModifyMask();
				partnerIDs.push_back(partner->GetPartner_id());
				newGrp.Add(pPartnerData);
			}
		}

		for (Index i = C_INDEX(0); i < m_wareHousePartners.size(); ++i)
		{
			CPartner* partner = GetWareHousePartnerByIndex(i);
			if (partner == NULL)
				continue;

			if (partner->GetState() == CPartner::ST_REST && m_pOwner->IsInMap() )
				partner->ChangeState(CPartner::ST_STANDBY);

			if (partner->IsUpdated()) {
				partner->CancelUpdate();
				partner->SetSendMask(partner->GetUpdateMask());
				partner->ClearUpdateMask();
				oldGrp.Add(partner);
			}
			if (partner->NeedUpdate())
			{
				pPartnerData = CreateNetObject<CPartnerData>();
				TRACE_ENSURE(pPartnerData);
				partner->WriteData(pPartnerData);
				partner->ClearModifyMask();
				partnerIDs.push_back(partner->GetPartner_id());
				newGrp.Add(pPartnerData);
			}
		}

		if (m_newPartners.GetCount() > 0) {
			for (UInt i = 0; i < m_newPartners.GetCount(); ++i)
			{
				CPartner* partner = m_newPartners.GetChild<CPartner>(i);
				if (partner && partner != DUMMY_PARTNER_ADDRESS) {
					pPartnerData = CreateNetObject<CPartnerData>();
					TRACE_ENSURE(pPartnerData);
					partner->WriteData(pPartnerData);
					partner->CancelUpdate();
					partnerIDs.push_back(0);
					newGrp.Add(pPartnerData);
				}
			}
			DeleteNetChilds(&m_newPartners);
		}

		if (oldGrp.GetCount() > 0)
			CB_UpdateAllPartner(m_pOwner->GetPeer(), m_pOwner->GetContext(), &oldGrp);

		PROCEDURE_STORE_VALUE1(RPCNetIDVector, delPartnerNetIDList)
		m_delGroup.Clear();

		if (partnerIDs.size() > 0)
		{
			ObjectService_Proxy::UpdatePartnerList(gGlobal.pObjectService->pPeer, 
				context, partnerIDs, &newGrp, true);
		}
		else
		{
			PROCEDURE_GO_STEP(2)
		}
		DeleteNetChilds(&newGrp);

	PROCEDURE_WAIT1(1, ObjectService, CB_UpdatePartnerList, NetGroup*, newPartners)

		NetGroup newGrp;
		NetGroup skillGrp;
		UInt32Vector delIds;
		CPartner* partner;

		// Update successfully
		PROCEDURE_RESTORE_VALUE1(RPCNetIDVector, delPartnerNetIDList)

		for (UInt16 i = 0; i < delPartnerNetIDList.size(); ++i )
		{
			partner = FindNetObject<CPartner>(delPartnerNetIDList[i]);
			if (partner == NULL) {
				continue;
			}
			LogService_Proxy::LogPartner(gGlobal.pLogService->pPeer, context, partner->GetPartner_id(), 
				partner->GetMob_id(), m_pOwner->GetChar_id(), LogService_Proxy::Partner::PARTNER_DELETE);
			delIds.push_back(partner->GetPartner_id());
			DeleteNetObject(partner);
		}

		// delete partners
		if (!delIds.empty() && m_pOwner && m_pOwner->GetPeer())
			CB_DeleteListSuccess(m_pOwner->GetPeer(), m_pOwner->GetContext(), delIds, true);

		if (m_pOwner && newPartners && newPartners->GetCount() > 0) {
			CPartnerData* pPartnerData;
			for (UInt16 i = 0; i < newPartners->GetCount(); ++i)
			{
				pPartnerData = newPartners->GetChild<CPartnerData>(i);
				if (pPartnerData == NULL || pPartnerData->GetOwner_id() != m_pOwner->GetChar_id())
					continue;
				partner = CPartner::NewInstance();
				TRACE_ENSURE(partner);
				partner->SetData(pPartnerData);
				partner->MakeLearnSkillLst(true);
				partner->CalcAttrib();
				partner->SetPartnerGroup(this);
				partner->SetSendMask(CPartner::AllMask);
				partner->ClearModifyMask();
				partner->ClearUpdateMask();
				Add(partner);
				newGrp.Add(partner);
				skillGrp.Add(partner->GetSkillGroup());

				LogService_Proxy::LogPartner(gGlobal.pLogService->pPeer, context, partner->GetPartner_id(), 
					partner->GetMob_id(), partner->GetOwner_id(), LogService_Proxy::Partner::PARTNER_CREATE);
			}
			if (newGrp.GetCount() > 0 && m_pOwner->GetPeer())
				CB_AddPartnerGroup(m_pOwner->GetPeer(), m_pOwner->GetContext(), &newGrp, &skillGrp);
			DeleteNetGroup(newPartners, BOOLEAN_TRUE);
			PROCEDURE_DO_METHOD(CPartnerGroup, AddAllPartnerSkill, this, context);
		} else {
			DeleteNetGroup(newPartners, BOOLEAN_TRUE);
			PROCEDURE_GO_STEP(2)
		}

	PROCEDURE_WAIT_RETURN(2)
/*
	// 1) This may crash the server with unknown reason: TRACE_ENSURE(pPartner && pPartner->m_pSkillGroup);
	// 2) Logically, Partner skill don't have Cooldown or Level to update, so it can be commented.

		UInt16 i = 0;

	PROCEDURE_FOR1(3, 5, bSkillUpdate && i < GetCount(), UInt16, i)

		CPartner *pPartner = GetChild<CPartner>(i);
		TRACE_ENSURE(pPartner && pPartner->m_pSkillGroup);
		
		i++;
		PROCEDURE_STORE_COUNTER1(UInt16, i)
		PROCEDURE_DO_METHOD(CSkillGroup, UpdateSkills, pPartner->m_pSkillGroup, context)

	PROCEDURE_WAIT_RETURN(4)
	
	PROCEDURE_END_FOR(5, 3)
*/
	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPartnerGroup DeletePartner Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH


	return RPC_RESULT_OK;
}

CPartner* CPartnerGroup::GetPartner(UInt32 partner_id)
{
	CPartner* partner;
	for (int i = 0; i < GetCount(); ++i)
	{
		partner = GetChild<CPartner>(i);
		if (partner && partner->GetPartner_id() == partner_id)
			return partner;
	}
	return NULL;
}

CPartner* CPartnerGroup::GetPartnerByMobId(UInt32 mob_id)
{
	CPartner* partner;
	for (int i = 0; i < GetCount(); ++i)
	{
		partner = GetChild<CPartner>(i);
		if (partner && partner->GetMob_id() == mob_id)
			return partner;
	}
	return NULL;
}

CPartner* CPartnerGroup::GetPartnerByIndex(UInt16 index)
{
	CPartner* partner = NULL;
	if (index < GetCount())
		partner = GetChild<CPartner>(index);

	return partner;

}

CPartner* CPartnerGroup::GetPartnerByID(UInt32 partner_id)
{
	CPartner* partner;
	for (int i = 0; i < GetCount(); ++i)
	{
		partner = GetChild<CPartner>(i);
		if (partner && partner->GetPartner_id() == partner_id)
			return partner;
	}
	return NULL;
}

void CPartnerGroup::SendAll()
{
	if (m_pOwner == NULL)
		return;

	NetGroup partnerGroup;
	NetGroup skillGroup;
	CPartner* pPartner = NULL;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{		
		pPartner = GetChild<CPartner>(i);
		if (pPartner)
		{
			partnerGroup.Add(pPartner);
			pPartner->PreSend(&skillGroup);
		}
	}
	if (partnerGroup.GetCount() > 0)
		CB_AddPartnerGroup(m_pOwner->GetPeer(), m_pOwner->GetContext(), &partnerGroup, &skillGroup);
}

void CPartnerGroup::InitPartnerOwnObject()
{
	CPartner* pPartner;
	for (int i = 0; i < GetCount(); ++i)
	{
		pPartner = GetChild<CPartner>(i);
		if (pPartner)
			pPartner->SetPartnerGroup(this); //setpartnergroup will do setownobject.
	}
}

void CPartnerGroup::Destroy()
{
	DeleteNetChilds(this);
}

UInt32 CPartnerGroup::GetFightPartnerID()
{
	for (UInt i = 0 ; i < GetCount(); ++i)
	{
		CPartner* pPartner = GetChild<CPartner>(i);
		if (pPartner && pPartner->GetState() == CPartner::ST_FIGHT)
				return pPartner->GetPartner_id();
	}
	return 0;
}

CPartner* CPartnerGroup::GetFightPartner()
{
	for ( Index i = C_INDEX(0); i < GetCount(); ++i )
	{
		CPartner* pPartner = GetChild<CPartner>(i);
		if (pPartner && pPartner->GetState() == CPartner::ST_FIGHT)
			return pPartner;
	}
	return NULL;
}

UInt32 CPartnerGroup::GetPartnerID(UInt16 index)
{
	CPartner* pPartner = GetChild<CPartner>(index);
	if (pPartner)
		return pPartner->GetPartner_id();
	
	return 0;	
}

UInt16 CPartnerGroup::GetPartnerCount(UInt32 modId)
{
	UInt16 num = 0;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		CPartner* pPartner = GetChild<CPartner>(i);
		if (pPartner && pPartner->GetMob_id() == modId)
			++num;
	}
	return num;
}

void CPartnerGroup::GetPartnerIdList(UInt32Vector& idList)
{
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		CPartner* pPartner = GetChild<CPartner>(i);
		if (pPartner)
			idList.push_back(pPartner->GetPartner_id());
	}
}

void CPartnerGroup::SetAllPartnerSkills(NetGroup* skillDataGroups, NetGroup *sendGroup,
										bool forNewOrLvlUpPartner, UInt inWareHouse)
{
	TRACE_ENSURE(skillDataGroups);

	CPartner* pPartner = NULL;

	for (Index j = C_INDEX(0); j < skillDataGroups->GetCount(); ++j)
	{
		CSkillData* pData = skillDataGroups->GetChild<CSkillData>(j);
		if (pData && pData->GetOwner_type() == SKILL_OWNER_TYPE_PARTNER)
		{
			if (inWareHouse == 0)
				pPartner = GetPartner(pData->GetOwner_id());
			else 
				pPartner = GetWareHousePartner(pData->GetOwner_id());

			if (pPartner) {
				CSkill* pSkill = CSkill::NewInstance();
				TRACE_ENSURE(pSkill);
				pSkill->SetData(pData);
				if (pPartner->AddSkill(pSkill)) {
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

	bool needRaise = false;
	UInt count = GetCount();
	if (inWareHouse != 0)
		count = (UInt)m_wareHousePartners.size();

	for (Index j = C_INDEX(0); j < count; ++j)
	{
		if (inWareHouse == 0)
			pPartner = GetPartnerByIndex(j);
		else
			pPartner = GetWareHousePartnerByIndex(j);

		if (pPartner && pPartner->HasAddSkill())
		{
			//if there are new skills, the partner need to re-calculate
			pPartner->CalcAttrib();

			//only for old partner and learn one skill
			if (pPartner->m_iNewSkillID > 0)
			{
				pPartner->CB_LearnSkillMessage(m_pOwner->GetPeer(), m_pOwner->GetContext(), pPartner->m_iNewSkillID);
				pPartner->m_iNewSkillID = 0;
			}
			if (forNewOrLvlUpPartner)
			{
				pPartner->SetHP(pPartner->GetMaxHP(), pPartner->GetMaxHP());
				pPartner->SetSP(pPartner->GetMaxSP(), pPartner->GetMaxSP());
				needRaise = true;
			}
		}
	}
	if (needRaise)
		RaiseUpdate();
}

void CPartnerGroup::ChangeFavor(UInt8 targetType, float value)
{
	CPartner* pPartner = NULL;
	if (targetType == 0 )
	{
		pPartner = GetFightPartner();
		if (pPartner)
			pPartner->ChangeFavor(value);
	} else if (targetType == 255) {
		for ( UInt16 i = 0; i < GetCount(); ++i )
		{
			pPartner = GetChild<CPartner>(i);
			if (pPartner)
				pPartner->ChangeFavor(value);
		}
	}
}

void CPartnerGroup::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_UPDATE_OBJECT:				// update all partner to object service
		if (m_pOwner && ( GetCount() > 0 || m_delGroup.GetCount() > 0 || 
			m_newPartners.GetCount() > 0 || m_wareHousePartners.size() > 0))
		{
			PROCEDURE_DO_METHOD1(CPartnerGroup, UpdateAllPartner, this, m_pOwner->GetContext(),
				Boolean, BOOLEAN_FALSE)
		}
		break;
	}
}

UInt16 CPartnerGroup::GetCount(bool withNewPartner)
{
	if (withNewPartner)
		return Parent::GetCount() + m_newPartners.GetCount(); 
	return Parent::GetCount();
}

void CPartnerGroup::ChangeOwnerID(const UInt32Vector &partnerIDs, UInt32 targetOwnerID)
{
	UInt32Vector modIds;
	for (Index i = 0; i < partnerIDs.size(); ++i)
	{
		CPartner* partner = GetPartner(partnerIDs[i]);
		if (partner && m_pOwner->GetChar_id() != targetOwnerID)
		{
			partner->SetOwner(targetOwnerID);
			partner->SetState(CPartner::ST_STANDBY);

			if (partner->GetFavor() > 20.0f)
				partner->SetFavor(partner->GetFavor() - 20.0f);
			else
				partner->SetFavor(0.0f);

			modIds.push_back(partner->GetMob_id());
			Remove(partner);
		}
		else
			return;
	}

	PROCEDURE_DO_METHOD3(CPartnerGroup, ChangeOwnerID, this, m_pOwner->GetContext(),
				UInt32Vector, partnerIDs, UInt32Vector, modIds, UInt32, targetOwnerID)
}

RPCResult CPartnerGroup::ChangeOwnerID(RPCContext &context)
{
	PROCEDURE_START_METHOD3(CPartnerGroup, ChangeOwnerID, this, context, UInt32Vector, partnerIDs, UInt32Vector, modIds, UInt32, targetOwnerID)

		NetGroup grp;

		for (Index i = 0; i < partnerIDs.size(); ++i)
		{
			CPartnerData* pPartnerData = CreateNetObject<CPartnerData>();
			TRACE_ENSURE(pPartnerData);
			pPartnerData->SetPartner_id(partnerIDs[i]);
			pPartnerData->SetId(partnerIDs[i], modIds[i]);
			pPartnerData->SetOwner(targetOwnerID);	
			pPartnerData->SetSendMask(pPartnerData->GetUpdateMask());
			grp.Add(pPartnerData);
		}

		ObjectService_Proxy::UpdatePartnerList(gGlobal.pObjectService->pPeer, 
			context, partnerIDs, &grp, true);
		
		DeleteNetChilds(&grp);

	PROCEDURE_WAIT1(1, ObjectService, CB_UpdatePartnerList, NetGroup*, newPartners)

		DeleteNetGroup(newPartners, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPartnerGroup ChangeOwner Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

Boolean CPartnerGroup::ChangePartnerLockStatus(const UInt32 &bagIndex, const Boolean &lock, UInt8 &msg)
{
	CPartner* pPartner = GetPartnerByIndex(bagIndex);
	if (!pPartner)
	{
		msg = ASSET_INVALID_ERR;
		return BOOLEAN_FALSE;
	}
	if (pPartner->GetLock() == lock)
	{
		if (lock)
			msg = ASSET_LOCK_ALREADY;	
		else
			msg = ASSET_UNLOCK_ALREADY;	
		return BOOLEAN_FALSE;
	}
	pPartner->SetLock(lock);
	pPartner->RaiseUpdate();
	msg = lock? ASSET_LOCK_SUCCESS : ASSET_UNLOCK_SUCCESS;	
	return BOOLEAN_TRUE;
}

void CPartnerGroup::SetWareHousePartner(NetGroup* partnerGroup)
{
	m_wareHousePartners.clear();
	if (partnerGroup && partnerGroup->GetCount() > 0)
	{
		CPartnerData* pPartnerData;
		CPartner* pPartner;

		for (int i = 0; i < partnerGroup->GetCount(); ++i)
		{
			pPartnerData = partnerGroup->GetChild<CPartnerData>(i);
			if (pPartnerData == NULL) continue;
			pPartner = CPartner::NewInstance();
			pPartner->SetData(pPartnerData);
			pPartner->ExtractBuffString();
			pPartner->CalcAttrib();
			pPartner->ClearAllMask();
			m_wareHousePartners.push_back(pPartner->GetNetId());
		}
	}
}

void CPartnerGroup::GetWareHousePartnerID(UInt32Vector& idVector)
{
	CPartner* partner = NULL;
	for (Index i = C_INDEX(0); i < m_wareHousePartners.size(); ++i)
	{
		partner = GetWareHousePartnerByIndex(i);
		if (partner)
			idVector.push_back(partner->GetPartner_id());
	}
}
	
CPartner* CPartnerGroup::GetWareHousePartnerByIndex(UInt index)
{
	if (index >= m_wareHousePartners.size())
		return NULL;

	return FindNetObject<CPartner>(m_wareHousePartners[index]);	
}

CPartner* CPartnerGroup::GetWareHousePartner(UInt32 partnerID)
{
	CPartner* partner = NULL;
	for (Index i = C_INDEX(0); i < m_wareHousePartners.size(); ++i)
	{
		partner = GetWareHousePartnerByIndex(i);
		if (partner && partner->GetPartner_id() == partnerID)
			return partner;
	}
	return NULL;
}

void CPartnerGroup::ClearWareHouse()
{
	CPartner* partner = NULL;
	for (Index i = C_INDEX(0); i < PARTNER_MAX_WAREHOUSE; ++i)
	{
		partner = GetWareHousePartnerByIndex(i);
		if (partner)
			DeleteNetObject(partner);
	}
	m_wareHousePartners.clear();
}

void CPartnerGroup::GetAllWareHousePartner(NetGroup* partnerGroup, NetGroup* skillGroup)
{
	if (partnerGroup && skillGroup)
	{
		CPartner* partner = NULL;
		for (Index i = C_INDEX(0); i < PARTNER_MAX_WAREHOUSE; ++i)
		{
			partner = GetWareHousePartnerByIndex(i);
			if (partner)
			{
				partner->PreSend(skillGroup);
				partnerGroup->Add(partner);				
			}
		}
	}
}

RPCResult CPartnerGroup::PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("sessionId %s, char_id %d, partner_id %d"), 
		context.sessionId.GetString().c_str(), m_pOwner->GetChar_id(), partnerID);

	UInt msgIndex = WAREHOUSE_SUCCESS;
	do
	{
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			msgIndex = WAREHOUSE_NOT_IN_MAP;
			break;
		}

		// cannot move item when the script is running
		if (m_pOwner->GetScriptSystem() && m_pOwner->GetScriptSystem()->IsRunning())
		{
			msgIndex = WAREHOUSE_RUNNING_SCIRPT;
			break;
		}

		if (m_pOwner->GetAvailPartnerWareHouse() <= m_wareHousePartners.size())
		{
			msgIndex = WAREHOUSE_FULL;
			break;
		}

		if (m_pOwner->IsTrading())
		{
			msgIndex = WAREHOUSE_CANNOT_TRADING;
			break;
		}

		CPartner* partner = GetPartner(partnerID);
		if (partner == NULL || partner->m_data == NULL || partner->GetInWareHouse() != 0)
		{
			msgIndex = WAREHOUSE_NO_PARTNER;
			break;
		}

		if (partner->GetState() == CPartner::ST_FIGHT)
		{
			msgIndex = WAREHOUSE_PUT_IN_UING_STATE;
			break;
		}
	
		Remove(partner);
		if (m_pOwner->GetPartner_id() == partner->GetPartner_id())
			m_pOwner->SetPartner(0, 0, _T(""), 0);
		partner->SetState(CPartner::ST_STANDBY);
		partner->SetInWareHouse(1);
		m_wareHousePartners.push_back(partner->GetNetId());
		RaiseUpdate();
		
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("sessionId %s, char_id %d, partner_id %d, partner inWareHouse %d"), 
			context.sessionId.GetString().c_str(), m_pOwner->GetChar_id(), partnerID, partner->GetInWareHouse());
	} 
	while(false);

	CB_WareHouse(pPeer, context, WAREHOUSE_PROCESS_PUT_PARTNER, msgIndex);

	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("sessionId %s, char_id %d, partner_id %d"), 
		context.sessionId.GetString().c_str(), m_pOwner->GetChar_id(), partnerID);
	UInt msgIndex = WAREHOUSE_SUCCESS;
	do
	{
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			msgIndex = WAREHOUSE_NOT_IN_MAP;
			break;
		}

		// cannot move item when the script is running
		if (m_pOwner->GetScriptSystem() && m_pOwner->GetScriptSystem()->IsRunning())
		{
			msgIndex = WAREHOUSE_RUNNING_SCIRPT;
			break;
		}

		if (GetCount() >= m_pOwner->GetMaxPartner())
		{
			msgIndex = WAREHOUSE_FULL;
			break;
		}

		if (m_pOwner->IsTrading())
		{
			msgIndex = WAREHOUSE_CANNOT_TRADING;
			break;
		}

		CPartner* partner = GetWareHousePartner(partnerID);
		if (partner == NULL || partner->m_data == NULL || partner->GetInWareHouse() != 1)
		{
			msgIndex = WAREHOUSE_NO_PARTNER;
			break;
		}

		RPCNetIDVector::iterator itr = remove(m_wareHousePartners.begin(), m_wareHousePartners.end(), partner->GetNetId());
		if (itr == m_wareHousePartners.end())
		{
			msgIndex = WAREHOUSE_NO_PARTNER;
			break;
		}

		m_wareHousePartners.erase(itr);
		partner->SetInWareHouse(0);
		partner->SetState(CPartner::ST_STANDBY);
		if (m_pOwner && m_pOwner->m_bUsePartnerCollectionBuff)
		{
			UInt16 Count = m_pOwner->GetPartnerCollectionBuff(partner->GetRace(), partner->m_AttriOfPartnerCollection);
			if (Count > 0)
			{
				UInt16 BuffID = gGlobal.m_resource->GetPartnerCollectionBuffID(Count, partner->GetRace());
				partner->RemoveBuffSchool(PARTNER_COLLECTION_BUFF_SCHOOL_ID);
				if (BuffID > 0)
				{
					partner->AddBuff(BuffID, 0, 0, 0, 99999, 0);
					partner->CalcAttrib();
				}
			}
		}
		else if(partner->HasBuffSchool(PARTNER_COLLECTION_BUFF_SCHOOL_ID))
		{
			ZeroMemory(&partner->m_AttriOfPartnerCollection, sizeof(AttriStruct2));
			partner->RemoveBuffSchool(PARTNER_COLLECTION_BUFF_SCHOOL_ID);
			partner->CalcAttrib();
		}
		Add(partner);
		RaiseUpdate();

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("sessionId %s, char_id %d, partner_id %d, partner inWareHouse %d"), 
			context.sessionId.GetString().c_str(), m_pOwner->GetChar_id(), partnerID, partner->GetInWareHouse());
	}
	while(false);

	CB_WareHouse(pPeer, context, WAREHOUSE_PROCESS_GET_PARTNER, msgIndex);

	return RPC_RESULT_OK;
}

void CPartnerGroup::UpdatePartnerCollection()
{
	if (m_pOwner)
		m_pOwner->ResetPartnerCollectionBuff();
}

RPCResult CPartnerGroup::PartnerCultivationUpByPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &src_partnerID, const UInt32 &dest_partnerID)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CPartnerGroup::PartnerCultivationUpByPartner(), sessionId %s, char_id %d, src_partnerID %d, dest_partnerID %d"), 
		context.sessionId.GetString().c_str(), m_pOwner->GetChar_id(), src_partnerID, dest_partnerID);

	UInt8 msgIndex = PARTNER_CULTIVATION_UP_SUCCESS;
	UInt8 msgValue = 0;

	CPartner* src_partner = GetPartner(src_partnerID);
	CPartner* dest_partner = GetPartner(dest_partnerID);
	if (src_partner && dest_partner)
	{
		if (!src_partner->GetLock())
		{
			if (!dest_partner->GetLock())
			{
				UInt8 src_race = src_partner->GetRace();
				if (src_race == 2
					|| src_race == 3
					|| src_race == 4
					|| src_race == 5
					|| src_race == 11)
				{
					if (src_partner->GetCultivation() < 10)
					{
						gGlobal.g_TempResult = 0;
						CallLuaFunc(gGlobal.g_pLuaState, "GetPartnerCultivationUpConsumeMoney", String(), 0, src_partner->GetCultivation());
						Int32 consumeMoney = (Int32)gGlobal.g_TempResult;
						if (consumeMoney < 0)
							consumeMoney = 0;
						if (m_pOwner->GetMoney() >= consumeMoney)
						{
							if (dest_partner->GetRace() == 1)
							{
								UInt16 src_partnerCultivation = src_partner->GetCultivation();

								if (dest_partner->GetReqLv() >= src_partner->GetReqLv())
								{
									if (dest_partner->GetPractice() > src_partnerCultivation)
									{
										gGlobal.g_TempResult = 0;
										CallLuaFunc(gGlobal.g_pLuaState, "GetPartnerCultivationUpRate", String(), src_partner->GetCultivation());
										Int32 rate = (Int32)gGlobal.g_TempResult;

										m_pOwner->ChangeMoney(-consumeMoney);

										if (RemovePartnerByUID(dest_partnerID))
										{
											Int32 range = GetRand(100);
											TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" range = %d, rate = %d."), range, rate);
											if (range < rate)
											{
												src_partner->ChangeCultivation(1);

												if (src_partner->GetCultivation() == 7)
												{
													String msg("");
													msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_SUCCESS_LEVEL7")), 
														m_pOwner->GetCharNameLink(), m_pOwner->GetCharPartnerNameLink(src_partner->GetPartner_id()));
													MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
														RPCContext(), msg, 0, 1);
												}
												else if (src_partner->GetCultivation() == 8)
												{
													String msg("");
													msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_SUCCESS_LEVEL8")), 
														m_pOwner->GetCharNameLink(), m_pOwner->GetCharPartnerNameLink(src_partner->GetPartner_id()));
													MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
														RPCContext(), msg, 0, 1);
												}
												else if (src_partner->GetCultivation() == 9)
												{
													String msg("");
													msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_SUCCESS_LEVEL9")), 
														m_pOwner->GetCharNameLink(), m_pOwner->GetCharPartnerNameLink(src_partner->GetPartner_id()));
													MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
														RPCContext(), msg, 0, 1);
												}
												else if (src_partner->GetCultivation() == 10)
												{
													String msg("");
													msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_SUCCESS_LEVEL10")), 
														m_pOwner->GetCharNameLink(), m_pOwner->GetCharPartnerNameLink(src_partner->GetPartner_id()));
													MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
														RPCContext(), msg, 0, 1);
												}

												msgIndex = PARTNER_CULTIVATION_UP_SUCCESS;
											}
											else
											{
												if (src_partnerCultivation > 0 && src_partnerCultivation <= 3)
												{
													src_partner->SetCultivation(0);
													src_partner->CalcAttrib();
													src_partner->RaiseUpdate();
												}
												else if(src_partnerCultivation > 4 && src_partnerCultivation <= 6)
												{
													src_partner->SetCultivation(4);
													src_partner->CalcAttrib();
													src_partner->RaiseUpdate();
												}
												else if(src_partnerCultivation > 7 && src_partnerCultivation <= 8)
												{
													src_partner->SetCultivation(7);
													src_partner->CalcAttrib();
													src_partner->RaiseUpdate();
												}
												else if(src_partnerCultivation > 9)
												{
													src_partner->SetCultivation(9);
													src_partner->CalcAttrib();
													src_partner->RaiseUpdate();
												}

												msgIndex = PARTNER_CULTIVATION_UP_FAIL;
												msgValue = src_partner->GetCultivation();
											}
										}
										else
										{
											msgIndex = PARTNER_CULTIVATION_UP_FAIL_NOT_PARTNER;
										}
									}
									else
									{
										msgIndex = PARTNER_CULTIVATION_UP_FAIL_PRACTICE;
										msgValue = src_partnerCultivation;
									}
								}
								else
								{
									msgIndex = PARTNER_CULTIVATION_UP_FAIL_REQLV;
								}
							}
							else
							{
								msgIndex = PARTNER_CULTIVATION_UP_FAIL_DEST_PARTNER_RACE;
							}
						}
						else
						{
							msgIndex = PARTNER_CULTIVATION_UP_FAIL_MONEY;
						}
					}
					else
					{
						msgIndex = PARTNER_CULTIVATION_UP_FAIL_MAX_CULTIVATION;
					}
				}
				else
				{
					msgIndex = PARTNER_CULTIVATION_UP_FAIL_SRC_PARTNER_RACE;
				}
			}
			else
			{
				msgIndex = PARTNER_CULTIVATION_UP_FAIL_DEST_PARTNER_LOCK;
			}
		}
		else
		{
			msgIndex = PARTNER_CULTIVATION_UP_FAIL_SRC_PARTNER_LOCK;
		}
	}
	else
	{
		msgIndex = PARTNER_CULTIVATION_UP_FAIL_NOT_PARTNER;
	}

	CB_PartnerCultivationUp(pPeer, context, msgIndex, msgValue, src_partnerID);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" msgIndex = %d, msgValue =%d."), msgIndex, msgValue);
	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::PartnerCultivationUpByItem(LPCPEER pPeer, RPCContext &context, const UInt32 &src_partnerID, const UInt32 &dest_itemUid)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CPartnerGroup::PartnerCultivationUpByItem(), sessionId %s, char_id %d, src_partnerID %d, dest_itemUid %d"), 
		context.sessionId.GetString().c_str(), m_pOwner->GetChar_id(), src_partnerID, dest_itemUid);

	UInt8 msgIndex = PARTNER_CULTIVATION_UP_SUCCESS;
	UInt8 msgValue = 0;

	CPartner* src_partner = GetPartner(src_partnerID);
	CItem* dest_item = m_pOwner->GetItems()->GetItemByUID(dest_itemUid);
	if (src_partner && dest_item)
	{
		if (!src_partner->GetLock())
		{
			if (!dest_item->GetLock())
			{
				UInt8 src_race = src_partner->GetRace();
				if (src_race == 2
					|| src_race == 3
					|| src_race == 4
					|| src_race == 5
					|| src_race == 11)
				{
					gGlobal.g_TempResult = 0;
					CallLuaFunc(gGlobal.g_pLuaState, "GetPartnerCultivationUpConsumeMoney", String(), 1, src_partner->GetCultivation());
					Int32 consumeMoney = (Int32)gGlobal.g_TempResult;
					if (consumeMoney < 0)
						consumeMoney = 0;
					if (m_pOwner->GetMoney() >= consumeMoney)
					{
						UInt16 src_partnerCultivation = src_partner->GetCultivation();

						gGlobal.g_TempResult = 0;
						CallLuaFunc(gGlobal.g_pLuaState, "CheckPartnerCultivationUpItem", String(), dest_item->GetItem_id(), src_partner->GetCultivation());
						Int32 result = (Int32)gGlobal.g_TempResult;

						if (result == 0)
						{
							m_pOwner->ChangeMoney(-consumeMoney);

							//m_pOwner->GetItems()->RemoveItem(dest_item->GetItem_id(), 1);
							dest_item->SetStack(dest_item->GetStack() - 1);
							if (dest_item->GetStack() == 0)
								m_pOwner->GetItems()->AddToDeleteGroup(dest_item->GetBagIndex());
							m_pOwner->GetItems()->RaiseUpdate();

							gGlobal.g_TempResult = 0;
							CallLuaFunc(gGlobal.g_pLuaState, "GetPartnerCultivationUpRate", String(), src_partner->GetCultivation());
							Int32 rate = (Int32)gGlobal.g_TempResult;

							Int32 range = GetRand(100);
							TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" range = %d, rate = %d."), range, rate);
							if (range < rate)
							{
								src_partner->ChangeCultivation(1);

								if (src_partner->GetCultivation() == 7)
								{
									String msg("");
									msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_SUCCESS_LEVEL7")), 
										m_pOwner->GetCharNameLink(), m_pOwner->GetCharPartnerNameLink(src_partner->GetPartner_id()));
									MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
										RPCContext(), msg, 0, 1);
								}
								else if (src_partner->GetCultivation() == 8)
								{
									String msg("");
									msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_SUCCESS_LEVEL8")), 
										m_pOwner->GetCharNameLink(), m_pOwner->GetCharPartnerNameLink(src_partner->GetPartner_id()));
									MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
										RPCContext(), msg, 0, 1);
								}
								else if (src_partner->GetCultivation() == 9)
								{
									String msg("");
									msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_SUCCESS_LEVEL9")), 
										m_pOwner->GetCharNameLink(), m_pOwner->GetCharPartnerNameLink(src_partner->GetPartner_id()));
									MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
										RPCContext(), msg, 0, 1);
								}
								else if (src_partner->GetCultivation() == 10)
								{
									String msg("");
									msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_SUCCESS_LEVEL10")), 
										m_pOwner->GetCharNameLink(), m_pOwner->GetCharPartnerNameLink(src_partner->GetPartner_id()));
									MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
										RPCContext(), msg, 0, 1);
								}

								msgIndex = PARTNER_CULTIVATION_UP_SUCCESS;
							}
							else
							{
								if (src_partnerCultivation > 0 && src_partnerCultivation <= 3)
								{
									src_partner->SetCultivation(0);
									src_partner->CalcAttrib();
									src_partner->RaiseUpdate();
								}
								else if(src_partnerCultivation > 4 && src_partnerCultivation <= 6)
								{
									src_partner->SetCultivation(4);
									src_partner->CalcAttrib();
									src_partner->RaiseUpdate();
								}
								else if(src_partnerCultivation > 7 && src_partnerCultivation <= 8)
								{
									src_partner->SetCultivation(7);
									src_partner->CalcAttrib();
									src_partner->RaiseUpdate();
								}
								else if(src_partnerCultivation > 9)
								{
									src_partner->SetCultivation(9);
									src_partner->CalcAttrib();
									src_partner->RaiseUpdate();
								}

								msgIndex = PARTNER_CULTIVATION_UP_FAIL;
								msgValue = src_partner->GetCultivation();
							}
						}
						else
						{
							if (result == 1)
								msgIndex = PARTNER_CULTIVATION_UP_FAIL_ITEMTYPE;
							else
								msgIndex = PARTNER_CULTIVATION_UP_FAIL_ITEMLEVEL;
						}
					}
					else
					{
						msgIndex = PARTNER_CULTIVATION_UP_FAIL_MONEY;
					}
				}
				else
				{
					msgIndex = PARTNER_CULTIVATION_UP_FAIL_SRC_PARTNER_RACE;
				}
			}
			else
			{
				msgIndex = PARTNER_CULTIVATION_UP_FAIL_ITEMLOCK;
			}
		}
		else
		{
			msgIndex = PARTNER_CULTIVATION_UP_FAIL_SRC_PARTNER_LOCK;
		}
	}
	else
	{
		msgIndex = PARTNER_CULTIVATION_UP_FAIL_NOT_PARTNER;
	}

	CB_PartnerCultivationUp(pPeer, context, msgIndex, msgValue, src_partnerID);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" msgIndex = %d, msgValue =%d."), msgIndex, msgValue);
	return RPC_RESULT_OK;
}
