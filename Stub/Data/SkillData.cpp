//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/SkillData.h"
//-- Library
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboSkill.h"
#include "ObjectService/Global.h"
#include "Common/common_binary.h"
#include "ObjectService/DboSourcePool.h"

REGISTER_CLASSTYPE(CSkillData, SkillData_CLASSID);

CSkillData* NewSkill();
CdboSkill* &pdbSkill = gGlobal.gTables.pSkill;
StringVector CdboSkillAccessor::m_params;
ThreadKey CSkillData::m_key;

NetGroup* FindSkillGroup(SkillGroupKey &key);
void PreSendGroup(NetGroup * skillGroup);

struct SkillRequest : public WorkRequest<UInt32>
{
	bool			bCreate;
	UInt8Vector		owner_types;
	UInt32Vector	owner_ids;
	UInt16Vector	skill_ids;
	UInt16Vector	skill_levels;
	SkillRequest(UInt32 id, LPCPEER pPeer, RPCContext &context)
		: WorkRequest<UInt32>(id, pPeer, context), bCreate(false)
	{}
};


////////////////////////////////////////////////////////////////////////
////						Skill Data							////
////////////////////////////////////////////////////////////////////////
CSkillData* NewSkill()
{
	CSkillData *pSkillData = NULL;
	SkillDataKey SkillKey((UInt16)pdbSkill->m_skill_id, (UInt32)pdbSkill->m_owner_id.LowPart, (UInt8)pdbSkill->m_owner_type);
	SkillGroupKey SkillGKey((UInt32)pdbSkill->m_owner_id.LowPart, (UInt8)pdbSkill->m_owner_type);
	DataObject &dobj = gGlobal.skillMap[SkillKey];

	// object already exist
	if (dobj.pobj) {
		pSkillData = CastNetObject<CSkillData>(dobj.pobj);
	}
	if (pSkillData == NULL)
	{
		pSkillData = CSkillData::NewInstance();
		pSkillData->SetData(pdbSkill);			// assign values to character data
		dobj.pobj = pSkillData;
	}

	// found existing skill group in cache
	NetGroup* skillGroup = FindSkillGroup(SkillGKey);
	if (skillGroup)
	{
		skillGroup->Add(pSkillData);
	}
	else
	{
		DataObject& dobj3 = gGlobal.skillGroupMap[SkillGKey];
		if (dobj3.pobj == NULL) {
			skillGroup = SafeCreateObject(NetGroup);
			dobj3.pobj = skillGroup;
		} else {
			skillGroup = (NetGroup*) dobj3.pobj;
		}
		skillGroup->Add(pSkillData);
	}

	return pSkillData;
}

CSkillData* FindSkill(SkillDataKey &key)
{
	SkillDataMap::iterator iter = gGlobal.skillMap.find(key);
	CSkillData* pSkillData = NULL;

	// found existing account in cache
	if (iter != gGlobal.skillMap.end())
		pSkillData = CastNetObject<CSkillData>(iter->second.pobj);

	return pSkillData;
}

NetGroup* FindSkillGroup(SkillGroupKey &key)
{
	SkillGroupMap::iterator iter = gGlobal.skillGroupMap.find(key);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.skillGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}

HRESULT RemoveSkill(UInt16 skill_id, UInt32 owner_id, UInt8 owner_type)
{
	SkillDataKey SkillKey(skill_id, owner_id, owner_type);
	SkillDataMap::iterator iter = gGlobal.skillMap.find(SkillKey);
	CSkillData* pSkillData;
	NetGroup* skillGroup;

	if (iter != gGlobal.skillMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		pSkillData = CastNetObject<CSkillData>(iter->second.pobj);
		if (pSkillData) {
			SkillGroupKey SkillGKey(owner_id, owner_type);

			skillGroup = FindSkillGroup(SkillGKey);
			if (skillGroup)
				skillGroup->Remove(pSkillData);
			// directly remove coz of primary constrain
			//gGlobal.m_deleteGroup.Add(pSkillData);
			pSkillData->DeleteDbo(pdbSkill);
			DeleteNetObject(pSkillData);
		}
		gGlobal.skillMap.erase(iter);
	}

	return S_OK;
}

HRESULT RemoveSkills(UInt32 owner_id, UInt8 owner_type)
{
	NetGroup* skillGroup = NULL;
	SkillGroupKey gkey(owner_id, owner_type);
	SkillGroupMap::iterator it = gGlobal.skillGroupMap.find(gkey);

	if (it != gGlobal.skillGroupMap.end()) {
		CSkillData* pSkillData;
		SkillDataMap::iterator it2;

		skillGroup = CastNetObject<NetGroup>(it->second.pobj);
		for (UInt16 i = 0; i < skillGroup->GetCount(); i++)
		{
			pSkillData = skillGroup->GetChild<CSkillData>(i);
			if (pSkillData) {
				SkillDataKey key(pSkillData->GetSkill_id(), pSkillData->GetOwner_id(), 
					pSkillData->GetOwner_type());
				it2 = gGlobal.skillMap.find(key);
				if (it2 != gGlobal.skillMap.end()) {
					gGlobal.DequeueUpdate(&it2->second);
					gGlobal.skillMap.erase(it2);
				}
				// directly remove coz of primary constrain
				//gGlobal.m_deleteGroup.Add(pSkillData);
				DeleteNetObject(pSkillData);
			}
		}
		SafeDeleteObject(skillGroup);
		gGlobal.skillGroupMap.erase(it);
	}

	// delete all skills by owner id and owner type
	HRESULT hr = pdbSkill->DeleteByOwner(owner_id, owner_type);
	pdbSkill->Close();

	return hr;
}

void ReleaseSkills(UInt32 owner_id, UInt8 owner_type)
{
	SkillGroupMap::iterator iter = gGlobal.skillGroupMap.find(
		SkillGroupKey(owner_id, owner_type));

	if (iter != gGlobal.skillGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		CSkillData *pSkillData;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++) {
			pSkillData = grp->GetChild<CSkillData>(i);
			if (pSkillData) {
				SkillDataMap::iterator it = gGlobal.skillMap.find(
					SkillDataKey(pSkillData->GetSkill_id(), owner_id, owner_type));
				if (it != gGlobal.skillMap.end()) {		// remove from update queue
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.skillMap.erase(it);
				}
				if (pSkillData->IsUpdated())				// write to db if updated
					gGlobal.m_releaseGroup.Add(pSkillData);
				else
					DeleteNetObject(pSkillData);
			}
		}
		gGlobal.skillGroupMap.erase(iter);
		SafeDeleteObject(grp);
	}
}

// set skill data detail in group
void PreSendGroup(NetGroup * skillGroup)
{
	if (skillGroup == NULL) return;

	for (UInt16 i = 0; i < skillGroup->GetCount(); i++)
	{
		CSkillData *pSkillData = skillGroup->GetChild<CSkillData>(i);
		if (pSkillData)
			pSkillData->PreSend();
		else
			skillGroup->Replace(i, NULL);
	}
}

DWORD WINAPI QueueGetSkills(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();

	SkillRequest* pRequest = (SkillRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
	{
		UInt16 count = (UInt16) pRequest->owner_ids.size();
		NetGroup skillGroup;
		CSkillData* pSkillData;
		CdboSkill* dboSkill = SafeCreateObject(CdboSkill);

		skillGroup.SetValidate(BOOLEAN_FALSE);
		dboSkill->SetDataSource(dbSrc);
		for (UInt16 i = 0; i < count; i++)
		{
			if (pRequest->bCreate)
			{
				if (DB_SUCCEEDED(hr = dboSkill->Insert(pRequest->skill_ids[i], 
					pRequest->owner_ids[i], pRequest->owner_types[i], 
					pRequest->skill_levels[i])))
				{
					if (DB_SUCCEEDED(hr = dboSkill->MoveFirst()))
					{
						pSkillData = SafeCreateObject(CSkillData);
						pSkillData->SetData(dboSkill);
						pSkillData->PreSend();
						skillGroup.Add(pSkillData);
					}
				}
			}
			else
			{
				if (DB_SUCCEEDED(hr = dboSkill->RSByID(pRequest->owner_ids[i], 
					pRequest->owner_types[i])))
				{
					hr = dboSkill->MoveFirst();
					while (hr == S_OK)
					{
						pSkillData = SafeCreateObject(CSkillData);
						pSkillData->SetData(dboSkill);
						pSkillData->PreSend();
						skillGroup.Add(pSkillData);
						hr = dboSkill->MoveNext();
					}
				}
			}
			dboSkill->Close();
			if (hr != DB_S_ENDOFROWSET && DB_FAILED(hr))
				break;
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetSkills"), hr, dboSkill->GetErrorMsg(hr), 
			pRequest->context, &skillGroup);
		SafeDeleteObject(dboSkill);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("cannot get data source, sessionId %s, owner_ids %s, owner_types %s"),
			pRequest->context.sessionId.GetString().c_str(), 
			GetVectorString(pRequest->owner_ids).c_str(), 
			GetVectorString(pRequest->owner_types).c_str());
	}
	// show query time
	UInt32 dt = ::GetTickCount() - curTime;
	if (dt > 1000)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("profile, utime %d, skill_ids %s"), 
			dt, GetVectorString(pRequest->skill_ids).c_str());
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

void CSkillData::SetData(CdboSkill* dboSkill)
{
	SetInfo((UInt16) dboSkill->m_skill_id);
	SetOwner_info((UInt32)dboSkill->m_owner_id.LowPart, 
		(UInt8)dboSkill->m_owner_type);
	SetLevel((UInt16) dboSkill->m_skill_lv);
	SetCoolDown((UInt32) dboSkill->m_cool_down.LowPart);

	ClearUpdateMask();
}

void CSkillData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

HRESULT CSkillData::UpdateDbo(CdboSkill* dboSkill)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("skill_id %d, owner_id %d,%d, update_mask %d,%d"), 
		GetSkill_id(), GetOwner_id(), GetOwner_type(), m_updateMask.low, m_updateMask.high);

	dboSkill->ClearParam();
	if (IsUpdated(owner_infoMask)) {
		COLUMN_UPDATE_LARGE_QUA(dboSkill, owner_id, GetOwner_id());
		COLUMN_UPDATE(dboSkill, owner_type, GetOwner_type());
	}
	if (IsUpdated(infoMask)) {
		COLUMN_UPDATE(dboSkill, skill_id, GetSkill_id());
	}

	if (IsUpdated(LevelMask)) {
		COLUMN_UPDATE(dboSkill, skill_lv, GetSkill_level());
	}

	if (IsUpdated(CoolDownMask)) {
		COLUMN_UPDATE_LARGE_QUA(dboSkill, cool_down, GetCool_down());
	}

	HRESULT hr;
	if (DB_FAILED(hr = dboSkill->UpdateByAny(GetSkill_id(), GetOwner_id(), GetOwner_type())))
		dboSkill->ShowError(hr);
	else
		ClearUpdateMask();
	dboSkill->ClearParam();
	dboSkill->Close();

	return hr;
}

HRESULT CSkillData::DeleteDbo(CdboSkill* dboSkill)
{
	HRESULT hr;

	if (DB_FAILED(hr = dboSkill->Delete(GetSkill_id(), GetOwner_id(), GetOwner_type())))
		dboSkill->ShowError(hr);
	dboSkill->Close();

	return hr;
}

RPCResult ObjectService_Stub::UpdateSkillList(
	LPCPEER pPeer, RPCContext &context, 
	const UInt32 &owner_id, 
	const UInt8 &owner_type, NetGroup* skillGrp)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("owner_id %d, owner_type %d"), 
			owner_id, owner_type);
	
	for (UInt i = 0; i < skillGrp->GetCount(); ++i)
	{
		CSkillData *pNewData = skillGrp->GetChild<CSkillData>(i);
		TRACE_ENSURE(pdbSkill && pNewData);

		SkillDataKey SkillKey(pNewData->GetSkill_id(), owner_id, owner_type);
		CSkillData* pSkillData = FindSkill(SkillKey);

		if (pSkillData == NULL)
		{
			TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("skill_id %d, owner_id %d, owner_type %d not found"),
				pNewData->GetSkill_id(), owner_id, owner_type);
		}
		else
		{
			SkillDataMap::iterator iter = gGlobal.skillMap.find(SkillKey);

			pSkillData->Clone(pNewData, pNewData->GetUpdateMask());
			if (iter != gGlobal.skillMap.end())
				gGlobal.EnqueueUpdate(&iter->second);
		}
	}

	DeleteNetGroup(skillGrp, BOOLEAN_TRUE);
	CB_UpdateSkillList(pPeer, context);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::DeleteSkillList(
	LPCPEER pPeer, RPCContext &context, 
	const UInt32 &owner_id, 
	const UInt16Vector &skill_ids, 
	const UInt8 &owner_type)
{
	SkillGroupKey SkillGKey(owner_id, owner_type);
	HRESULT hr = S_OK;
	CSkillData* pSkillData = NULL;
	NetGroup* skillGroup = NULL;

	TRACE_INFODTL_3(GLOBAL_LOGGER, 
		_F("owner_id %d, owner_type %d, skill_id %s"), 
		owner_id, owner_type, GetVectorString(skill_ids).c_str());

	for (Int i = 0; i < (Int)skill_ids.size() ; i++)
	{
		hr = RemoveSkill(skill_ids[i], owner_id, owner_type);
		pdbSkill->Close();
		if (DB_FAILED(hr))
		{
			pdbSkill->ShowError(hr);
			continue;
		}
	}

	CB_DeleteSKillDone(pPeer, context);

	return RPC_RESULT_OK;
}

void NewSkillGroup(LPCPEER pPeer, RPCContext& context, NetGroup* skillGroup, 
				   const UInt32Vector &owner_ids, const UInt8Vector &owner_types, 
				   UInt8 owner_type, bool bCreate)
{
	if (skillGroup == NULL)
	{
		ObjectService_Stub::CB_Error(pPeer, context, 1, 
			_FE("null skillGroup"));
		return;
	}

	TRACE_ENSURE(skillGroup);

	NetGroup sendGrp;
	CSkillData* pSkillData;

	// store new skill group only
	if (skillGroup->GetCount() > 0)
	{
		NetGroup* newGroup;
		SkillGroupKey prevGKey(0, 0);
		SkillGroupKey gKey(0, 0);

		for (UInt16 i = 0; i < skillGroup->GetCount(); i++)
		{
			pSkillData = skillGroup->GetChild<CSkillData>(i);
			if (pSkillData)
			{
				// need to register for new skill data
				gKey.owner_id = pSkillData->GetOwner_id();
				gKey.owner_type = pSkillData->GetOwner_type();
				SkillDataKey key(pSkillData->GetSkill_id(), pSkillData->GetOwner_id(),
					pSkillData->GetOwner_type());
				DataObject &dobj = gGlobal.skillMap[key];
				if (dobj.pobj)
				{
					DeleteNetObject(pSkillData);
					pSkillData = CastNetObject<CSkillData>(dobj.pobj);
				}
				else
				{
					LocalRPCManager::GetInstance().Register(pSkillData);
					pSkillData->ClearUpdateMask();
					dobj.pobj = pSkillData;
				}
				if (gKey.owner_id != prevGKey.owner_id || 
					gKey.owner_type != prevGKey.owner_type)
				{
					DataObject& dobj3 = gGlobal.skillGroupMap[gKey];
					if (dobj3.pobj)
					{
						newGroup = (NetGroup*) dobj3.pobj;
					}
					else
					{
						newGroup = SafeCreateObject(NetGroup);
						dobj3.pobj = newGroup;
					}
					prevGKey = gKey;
				}
				newGroup->Add(pSkillData);
				if (bCreate)		// direct add to send group when create skill
				{
					pSkillData->PreSend();
					sendGrp.Add(pSkillData);
				}
			}
		}
	}

	// find all skill group when getting skill group
	if (!bCreate)
	{
		SkillGroupKey gKey;
		NetGroup* oneGroup;

		gKey.owner_type = owner_type;
		for (UInt16 i = 0; i < (UInt16) owner_ids.size(); i++)
		{
			gKey.owner_id = owner_ids[i];
			if (!owner_types.empty())
				gKey.owner_type = owner_types[i];
			oneGroup = FindSkillGroup(gKey);
			if (oneGroup)
			{
				for (UInt16 j = 0; j < oneGroup->GetCount(); j++)
				{
					CSkillData *pSkillData = oneGroup->GetChild<CSkillData>(j);
					if (pSkillData)
					{
						pSkillData->PreSend();
						sendGrp.Add(pSkillData);
					}
				}
			}
		}
	}

	// send skill data to peer
	ObjectService_Stub::CB_GetSkillList(pPeer, context, &sendGrp);
	DeleteNetGroup(skillGroup);
}

RPCResult ObjectService_Stub::CreateSkillList(
	LPCPEER pPeer, RPCContext &context, 
	const UInt8 &owner_type, 
	const UInt32Vector &owner_ids, 
	const UInt16Vector &skill_ids, 
	const UInt16Vector &skill_levels)
{
	PROCEDURE_START_RPC4(ObjectService, CreateSkillList, NULL, UInt8, owner_type,
			UInt32Vector, owner_ids, UInt16Vector, skill_ids, UInt16Vector, skill_levels)

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("owner_type %d, owner_ids %s"), 
		owner_type, GetVectorString(owner_ids).c_str());

	SkillRequest *pRequest = SafeCreateObject3(SkillRequest, 0, pPeer, context);
	UInt16 count = (UInt16) owner_ids.size();
	UInt32 owner_id;
	UInt16 skill_id;
	UInt16 skill_lv;
	NetGroup newSkills;
	CSkillData *pSkillData = NULL;
	HRESULT hr = S_OK;

	for (UInt16 i = 0; i < count; i++)
	{
		owner_id = owner_ids[i];
		skill_id = skill_ids[i];
		skill_lv = skill_levels[i];
		pSkillData = FindSkill(SkillDataKey(skill_id, owner_id, owner_type));
		if (pSkillData == NULL)
		{
			pRequest->owner_types.push_back(owner_type);
			pRequest->owner_ids.push_back(owner_id);
			pRequest->skill_ids.push_back(skill_id);
			pRequest->skill_levels.push_back(skill_lv);
			pRequest->bCreate = true;
		}
	}

	if (pRequest->skill_ids.empty())
	{
		// no skill to create
		NetGroup newSkills;

		TRACE_WARNDTL(GLOBAL_LOGGER, _FW("no skill to create"));
		CB_GetSkillList(pPeer, context, &newSkills);
		SafeDeleteObject(pRequest);
		PROCEDURE_RETURN
	}
	else
	{
		// wait for thread pool to process
		gGlobal.WorkQueueBegin();
#ifdef DB_THREAD_POOL
		gGlobal.QueueUserWorkItem(QueueGetSkills, pRequest, DB_WORKER_FLAG);
#else
		QueueGetSkills(pRequest);
#endif
	}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, skillGroup)

		UInt8Vector owner_types;
		NewSkillGroup(pPeer, context, skillGroup, owner_ids, owner_types, owner_type, true);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetSkillList(
	LPCPEER pPeer, RPCContext &context, 
	const UInt32Vector &owner_ids, 
	const UInt8Vector &owner_types)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC2(ObjectService, GetSkillList, NULL, UInt32Vector, owner_ids, 
						UInt8Vector, owner_types)

		TRACE_ENSURE(pdbSkill != NULL);
		HRESULT hr = S_OK;
		CSkillData *pSkillData = NULL;
		NetGroup skillGroup;
		NetGroup* oneGroup;
		UInt32Vector newIds;
		UInt8Vector newTypes;
		UInt16 count = (UInt16) owner_ids.size();
		SkillGroupKey gKey;

		TRACE_INFODTL_3(GLOBAL_LOGGER, 
			_F("count %d, owner_ids %s, owner_types %s"), count,
			GetVectorString(owner_ids).c_str(), GetVectorString(owner_types).c_str());

		for (UInt16 i = 0; i < count; i++)
		{
			gKey.owner_id = owner_ids[i];
			gKey.owner_type = owner_types[i];
			oneGroup = FindSkillGroup(gKey);
			if (oneGroup)
			{
				for (UInt16 j = 0; j < oneGroup->GetCount(); j++)
					skillGroup.Add(oneGroup->GetChild(j));
			}
			else
			{
				newIds.push_back(gKey.owner_id);
				newTypes.push_back(gKey.owner_type);
			}
		}

		if (newIds.empty())
		{
			// directly send back object group
			PreSendGroup(&skillGroup);
			CB_GetSkillList(pPeer, context, &skillGroup);
			PROCEDURE_RETURN
		}
		else
		{
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			SkillRequest *pRequest = SafeCreateObject3(SkillRequest, 0, pPeer, context);
			pRequest->bCreate = false;
			pRequest->owner_ids.swap(newIds);
			pRequest->owner_types.swap(newTypes);
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetSkills, pRequest, DB_WORKER_FLAG);
#else
			QueueGetSkills(pRequest);
#endif
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, skillGroup)
		
		NewSkillGroup(pPeer, context, skillGroup, owner_ids, owner_types, 0, false);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}
