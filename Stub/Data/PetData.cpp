//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/PetData.h"
//-- Library
#include "Stub/Data/SkillData.h"
#include "Stub/Service/ObjectService.h"
#include "ObjectService/DboSourcePool.h"
#include "ObjectService/dboPet.h"
#include "ObjectService/Global.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResPet.h"

REGISTER_CLASSTYPE(CPetData, PetData_CLASSID);

CdboPet* &pdbPet = gGlobal.gTables.pPet;
NetGroup* FindPetGroup(UInt32 char_id);
void PreSendGroup(NetGroup * petGroup);
StringVector CdboPetAccessor::m_params;
ThreadKey CPetData::m_key;

NetGroup* FindWareHousePetGroup(UInt32 char_id);

struct PetRequest : public WorkRequest<UInt32>
{
	UInt8 inWareHouse;
	PetRequest(UInt32 id, LPCPEER pPeer, RPCContext &context)
		: WorkRequest<UInt32>(id, pPeer, context)
	{}
};

////////////////////////////////////////////////////////////////////////
////						Pet Data							////
////////////////////////////////////////////////////////////////////////

CPetData* NewPet()
{
	CPetData *pPetData = NULL;
	UInt32 owner_id = (UInt32) pdbPet->m_owner_id.LowPart;
	UInt32 pet_uid = (UInt32) pdbPet->m_pet_uid.LowPart;
	DataObject &dobj = gGlobal.petIdMap[pet_uid];

	if (dobj.pobj == NULL) {
		pPetData = CPetData::NewInstance();
		pPetData->SetData(pdbPet);			// assign values to character data
		dobj.pobj = pPetData;
	} else {
		pPetData = CastNetObject<CPetData>(dobj.pobj);
	}

	if (pPetData)
	{
		NetGroup* petGroup = NULL;
		// found existing pets in cache
		if (pPetData->GetInWareHouse() == 0)
			petGroup = FindPetGroup(owner_id);
		else
			petGroup = FindWareHousePetGroup(owner_id);

		if (petGroup)
		{
			petGroup->Add(pPetData);
		}
		else
		{
			if (pPetData->GetInWareHouse() == 0)
			{
				DataObject& dobj3 = gGlobal.petGroupMap[owner_id];
				if (dobj3.pobj == NULL) {
					petGroup = SafeCreateObject(NetGroup);
					dobj3.pobj = petGroup;
				} else {
					petGroup = (NetGroup*) dobj3.pobj;
				}
				petGroup->Add(pPetData);
			}
			else
			{
				DataObject& dobj3 = gGlobal.wareHousePetGroupMap[owner_id];
				if (dobj3.pobj == NULL) {
					petGroup = SafeCreateObject(NetGroup);
					dobj3.pobj = petGroup;
				} else {
					petGroup = (NetGroup*) dobj3.pobj;
				}
				petGroup->Add(pPetData);
			}
		}
	}

	return pPetData;
}

void ShowPetError(HRESULT hr)
{
	/*String errMsg;
	pdbPet->GetError(errMsg);
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Pet DB Error: code %d: %s"), hr, errMsg.c_str());
	if (hr == DB_ERR_UNEXPECTED) {			// try to reopen OLEDB when unexpected error
		if (FAILED(hr = pdbPet->ReopenDataSource()))
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("OLE Error: failed to reopen Pet OLE DB"));
		}
	}*/
}


NetGroup* FindPetGroup(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.petGroupMap.find(char_id);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.petGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}


CPetData* FindPet(UInt32 pet_uid)
{
	DataIdMap::iterator iter = gGlobal.petIdMap.find(pet_uid);
	CPetData* pPetData = NULL;

	// found existing account in cache
	if (iter != gGlobal.petIdMap.end())
		pPetData = CastNetObject<CPetData>(iter->second.pobj);

	return pPetData;
}

NetGroup* FindWareHousePetGroup(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.wareHousePetGroupMap.find(char_id);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.wareHousePetGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}

HRESULT RemovePet(UInt32 pet_uid)
{
	DataIdMap::iterator iter = gGlobal.petIdMap.find(pet_uid);
	if (iter != gGlobal.petIdMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CPetData* pPetData = CastNetObject<CPetData>(iter->second.pobj);
		if (pPetData) {
			NetGroup* petGroup = NULL;
			
			if (pPetData->GetInWareHouse() == 0)
				petGroup = FindPetGroup(pPetData->GetOwner_id());
			else
				petGroup = FindWareHousePetGroup(pPetData->GetOwner_id());

			if (petGroup) {
				petGroup->Remove(pPetData);
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("RemovePet: pet_uid = %d, owner_id = %d"), pPetData->GetPet_uid(), pPetData->GetOwner_id());
			}
			gGlobal.m_deleteGroup.Add(pPetData);
		}
		gGlobal.petIdMap.erase(iter);
	}
	RemoveSkills(pet_uid, SKILL_OWNER_TYPE_PET);

	return S_OK;
}

void ReleasePets(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.petGroupMap.find(char_id);

	if (iter != gGlobal.petGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		CPetData *pPetData;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++) {
			pPetData = grp->GetChild<CPetData>(i);
			if (pPetData) {
				DataIdMap::iterator it = gGlobal.petIdMap.find(
					pPetData->GetPet_uid());
				if (it != gGlobal.petIdMap.end()) {		// remove from update queue
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.petIdMap.erase(it);
				}
				ReleaseSkills(pPetData->GetPet_uid(), SKILL_OWNER_TYPE_PET);
				if (pPetData->IsUpdated())				// write to db if updated
					gGlobal.m_releaseGroup.Add(pPetData);
				else
					DeleteNetObject(pPetData);
			}
		}
		gGlobal.petGroupMap.erase(iter);
		SafeDeleteObject(grp);
	}

	iter = gGlobal.wareHousePetGroupMap.find(char_id);

	if (iter != gGlobal.wareHousePetGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		CPetData *pPetData;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++) {
			pPetData = grp->GetChild<CPetData>(i);
			if (pPetData) {
				DataIdMap::iterator it = gGlobal.petIdMap.find(
					pPetData->GetPet_uid());
				if (it != gGlobal.petIdMap.end()) {		// remove from update queue
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.petIdMap.erase(it);
				}
				ReleaseSkills(pPetData->GetPet_uid(), SKILL_OWNER_TYPE_PET);
				if (pPetData->IsUpdated())				// write to db if updated
					gGlobal.m_releaseGroup.Add(pPetData);
				else
					DeleteNetObject(pPetData);
			}
		}
		gGlobal.wareHousePetGroupMap.erase(iter);
		SafeDeleteObject(grp);
	}
}

void PreSendPetGrp(NetGroup * grp)
{
	if (grp == NULL) return;

	for (UInt16 i = 0; i < grp->GetCount(); ++i)
	{
		CPetData *pPetData = grp->GetChild<CPetData>(i);
		if (pPetData)
			pPetData->PreSend();
		else
			grp->Replace(i, NULL);
	}
}

DWORD WINAPI QueueGetPets(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();

	PetRequest* pRequest = (PetRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
       	{
		NetGroup petGroup;
		CPetData* pPetData;
		CdboPet* dboPet = SafeCreateObject(CdboPet);

		petGroup.SetValidate(BOOLEAN_FALSE);
		dboPet->SetDataSource(dbSrc);
		if (DB_SUCCEEDED(hr = dboPet->RSByOwnerID(pRequest->key, pRequest->inWareHouse)))
		{
			hr = dboPet->MoveFirst();
			while (hr == S_OK)
			{
				pPetData = SafeCreateObject(CPetData);
				pPetData->SetData(dboPet);
				pPetData->PreSend();
				petGroup.Add(pPetData);
				hr = dboPet->MoveNext();
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetPets"), hr, dboPet->GetErrorMsg(hr), 
			pRequest->context, &petGroup);
		dboPet->Close();
		SafeDeleteObject(dboPet);
		dbPool.ReleaseSource(dbSrc);
	}
       	else
       	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, owner_id %d"),
			pRequest->context.sessionId.GetString().c_str(), pRequest->key);
	}
	// show query time
	UInt32 dt = ::GetTickCount() - curTime;
	if (dt > 1000)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("profile, utime %d, char_id %d"), 
			dt, pRequest->key);
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

void CPetData::SetData(CdboPet* dboPet)
{
	String name(dboPet->m_name);
	
	SetId( (UInt32)dboPet->m_pet_uid.LowPart, (UInt32)dboPet->m_pet_id.LowPart );
	SetOwner_id( (UInt32)dboPet->m_owner_id.LowPart);
	SetName(name.Trim());
	SetSex((UInt8)dboPet->m_sex);
	SetLevel((UInt16)dboPet->m_level);
	SetExp( (UInt32)dboPet->m_exp.LowPart);
	SetState((UInt16)dboPet->m_state);
	SetFather((UInt32)dboPet->m_father_id.LowPart, dboPet->m_father_name);
	SetMother((UInt32)dboPet->m_mother_id.LowPart, dboPet->m_mother_name);
	SetBirthCount(dboPet->m_birthCount);
	SetFirstMaster(dboPet->m_firstMaster);
	SetHappiness((UInt16)dboPet->m_happiness);
	SetVP((UInt32)dboPet->m_VP, (UInt32)dboPet->m_maxVP);
	SetElement((UInt8)dboPet->m_element);
	
	time_t uTime;
	TimeConvert(dboPet->m_expireDate, uTime);
	SetExpireDate((UInt32)uTime);

	SetSoulbound((UInt8)dboPet->m_soulbound);
	SetColor((UInt16)dboPet->m_color);
	SetSpirit((UInt16)dboPet->m_spirit);
	SetDepute((UInt32) dboPet->m_depute_master.LowPart, (UInt32) dboPet->m_depute_time, 
		(UInt32) dboPet->m_depute_useTime, (UInt32) dboPet->m_depute_exp.LowPart);
	SetInWareHouse((UInt8)dboPet->m_inWareHouse);
	SetLock((Boolean) (dboPet->m_lock == 1));
	ClearUpdateMask();
}

HRESULT CPetData::UpdateDbo(CdboPet* dboPet)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_5(GLOBAL_LOGGER, 
		_T("CPetData UpdateDbo: pet_uid %d, owner_id %d updateMask %d,%d, dboPet %d"), 
		GetPet_uid(), this->GetOwner_id(), m_updateMask.low, m_updateMask.high, dboPet);

	dboPet->ClearParam();

	if (IsUpdated(owner_idMask))
	{
		COLUMN_UPDATE_LARGE(dboPet, owner_id, GetOwner_id());
	}

	if (IsUpdated(nameMask))
	{
		COLUMN_UPDATE_STRING(dboPet, name, GetName());
	}

	if (IsUpdated(sexMask))
	{
		COLUMN_UPDATE(dboPet, sex, GetSex());
	}

	if (IsUpdated(levelMask))
	{
		COLUMN_UPDATE(dboPet, level, GetLevel());
	}

	if (IsUpdated(expMask))
	{
		COLUMN_UPDATE_LARGE(dboPet, exp, GetExp());
	}

	if (IsUpdated(stateMask))
	{
		COLUMN_UPDATE(dboPet, state, GetState());
	}

	if (IsUpdated(birthCountMask))
	{
		COLUMN_UPDATE(dboPet, birthCount, GetBirthCount());
	}

	if (IsUpdated(firstMasterMask))
	{
		COLUMN_UPDATE_STRING(dboPet, firstMaster, GetFirstMaster());
	}

	if (IsUpdated(fatherMask))
	{
		COLUMN_UPDATE_LARGE(dboPet, father_id, GetFather_id());
		COLUMN_UPDATE_STRING(dboPet, father_name, GetFather_name());
	}

	if (IsUpdated(motherMask))
	{
		COLUMN_UPDATE_LARGE(dboPet, mother_id, GetMother_id());
		COLUMN_UPDATE_STRING(dboPet, mother_name, GetMother_name());
	}
	
	if (IsUpdated(happinessMask))
	{
		COLUMN_UPDATE(dboPet, happiness, GetHappiness());
	}

	if (IsUpdated(VPMask))
	{
		COLUMN_UPDATE(dboPet, VP, GetVP());
		COLUMN_UPDATE(dboPet, maxVP, GetMaxVP());
	}

	if (IsUpdated(elementMask))
	{
		COLUMN_UPDATE(dboPet, element, GetElement());
	}

	if (IsUpdated(expireDateMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetExpireDate(), dbtime);
		COLUMN_UPDATE(dboPet, expireDate, dbtime);
	}

	if (IsUpdated(soulboundMask))
	{
		COLUMN_UPDATE(dboPet, soulbound, GetSoulbound());
	}
	
	if (IsUpdated(colorMask))
	{
		COLUMN_UPDATE(dboPet, color, GetColor());
	}

	if (IsUpdated(spiritMask))
	{
		COLUMN_UPDATE(dboPet, spirit, GetSpirit());
	}

	if (IsUpdated(deputeMask))
	{
		COLUMN_UPDATE_LARGE(dboPet, depute_master, GetDepute_master());
		COLUMN_UPDATE(dboPet, depute_time, GetDepute_time());
		COLUMN_UPDATE(dboPet, depute_useTime, GetDepute_useTime());
		COLUMN_UPDATE_LARGE(dboPet, depute_exp, GetDepute_exp());
	}

	if (IsUpdated(inWareHouseMask))
	{
		COLUMN_UPDATE(dboPet, inWareHouse, GetInWareHouse());
	}

	if (IsUpdated(assetLockMask))
	{
		UInt8 locked = GetLock()? 1:0;
		COLUMN_UPDATE(dboPet, lock, locked);
	}

	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CPetData UpdateDbo: params %d, dboPet %d"), 
	//	dboPet->m_params.size(), dboPet);
	HRESULT hr;
	if (DB_FAILED(hr = dboPet->UpdateByAny(GetPet_uid())))
		dboPet->ShowError(hr);
	else
		ClearUpdateMask();
	dboPet->ClearParam();
	dboPet->Close();

	return hr;
}

HRESULT CPetData::DeleteDbo(CdboPet* dboPet)
{
	HRESULT hr;

	//ThreadGuard guard(m_key);
	if (DB_FAILED(hr = dboPet->Delete(GetPet_uid())))
		dboPet->ShowError(hr);
	dboPet->Close();

	return hr;
}

void CPetData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

HRESULT DoCreatePet(UInt32 pet_id, UInt32 level, UInt32 owner_id, UInt32 father_id, 
					String father_name, UInt32 mother_id, 
					String mother_name , String first_master_name, CPetData* &pPetData)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreatePet: pet_id %d, owner_id %d"), pet_id,
		owner_id);

	HRESULT hr = S_OK;
	// get mob data from resource
	const PetData* data = gGlobal.m_resource->GetPetData(pet_id);
	if (data == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CreatePet DB_FAILED: pet_id %d not found in resource"), pet_id);
		return S_FALSE;
	}

	pPetData = NULL;
	
	DBTIMESTAMP dbtime;
	time_t expireTime = 0;
	if (data->pet_TimeLimitType == PET_TIME_LIMIT_TYPE_CREATE)
	{
		expireTime = time(NULL); 
		expireTime += data->pet_TimeLimit * 24 *3600;
	}
	TimeConvert(expireTime, dbtime);
	pdbPet->m_expireDate = dbtime;
	pdbPet->m_state = 0;
	pdbPet->m_level = level;
	COLUMN_STRING_COPY(pdbPet->m_name, data->pet_name);
	pdbPet->m_exp.QuadPart = 0;
	pdbPet->m_father_id.QuadPart = father_id;
	COLUMN_STRING_COPY(pdbPet->m_father_name, father_name.c_str());
	pdbPet->m_mother_id.QuadPart = mother_id;
	COLUMN_STRING_COPY(pdbPet->m_mother_name, mother_name.c_str());
	pdbPet->m_birthCount = 0;
	COLUMN_STRING_COPY(pdbPet->m_firstMaster, first_master_name.c_str());
	pdbPet->m_happiness = data->pet_StartHappiness;
	pdbPet->m_VP = data->pet_MaxVP;
	pdbPet->m_maxVP = data->pet_MaxVP;
	pdbPet->m_element = data->pet_element;
	UInt8 soulbound = data->pet_soulbound;
	if (soulbound == SOULBOUND_GET_BOUND)
		soulbound = SOULBOUND_ALREADY_BOUND;

	pdbPet->m_soulbound = soulbound;
	pdbPet->m_color = 0;
	if(data->pet_type == 0)
		pdbPet->m_spirit = 0;
	else
	{
		pdbPet->m_spirit = (rand()%MAX_PET_SPIRIT)+1;
	}
	pdbPet->m_depute_master.QuadPart = 0;
	pdbPet->m_depute_time = 0;
	pdbPet->m_depute_useTime = 0;
	pdbPet->m_depute_exp.QuadPart = 0;
	if (data->pet_sex == 255)
		pdbPet->m_sex = rand() % 2;
	else
		pdbPet->m_sex = data->pet_sex;
	

	hr = pdbPet->InsertPet(pet_id, owner_id, data->pet_name);

	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbPet->MoveFirst()))
	{
		pPetData = NewPet();
		
	}
	pdbPet->Close();

	return hr;
}

RPCResult ObjectService_Stub::CreatePet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_id,
										const UInt32 &level, const UInt32 &owner_id, 
										const UInt32 &father_id, const String &father_name, 
										const UInt32 &mother_id, const String &mother_name, 
										const String &first_master_name)
{
	TRACE_ENSURE(pdbPet != NULL);
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreatePet: pet_id = %d, owner_id = %d"), pet_id, owner_id);

	HRESULT hr = S_OK;
	CPetData* pPetData = NULL;

	hr = DoCreatePet(pet_id, level, owner_id, father_id, father_name, mother_id, mother_name, first_master_name, pPetData);
	if (DB_FAILED(hr)) {
		pdbPet->ShowError(hr);
		CB_Error(pPeer, context, hr, _T("CreatePet DB_FAILED:"));
		return RPC_RESULT_FAIL;
	}

	if (pPetData)
		pPetData->PreSend();
	
	CB_CreatePet(pPeer, context, pPetData);

	return RPC_RESULT_OK; 
}

RPCResult ObjectService_Stub::GetPetList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt8 &inWareHouse)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC2(ObjectService, GetPetList, NULL, UInt32, owner_id, UInt8, inWareHouse)

		TRACE_ENSURE(pdbPet != NULL);
		CPetData* pPetData = NULL;
		NetGroup* petGroup = NULL;
		if (inWareHouse == 0)
			petGroup = FindPetGroup(owner_id);
		else
			petGroup = FindWareHousePetGroup(owner_id);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("owner_id %d"), owner_id);

		if (petGroup)
		{
			// directly send back object group
			PreSendPetGrp(petGroup);
			CB_PetList(pPeer, context, petGroup);
			PROCEDURE_RETURN
		}
		else
		{
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			PetRequest *pRequest = SafeCreateObject3(PetRequest, owner_id, pPeer, context);
			if (pRequest)
				pRequest->inWareHouse = inWareHouse;
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetPets, pRequest, DB_WORKER_FLAG);
#else
			QueueGetPets(pRequest);
#endif
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, petGroup)

		TRACE_ENSURE(petGroup);

		// store new pet group
		CPetData* pPetData;
		
		NetGroup* newGroup = NULL;
		if (inWareHouse == 0)
		{
			DataObject& dobj3 = gGlobal.petGroupMap[owner_id];
			if (dobj3.pobj) {
				newGroup = (NetGroup*) dobj3.pobj;
			} else {
				newGroup = SafeCreateObject(NetGroup);
				dobj3.pobj = newGroup;
			}
		}
		else
		{
			DataObject& dobj3 = gGlobal.wareHousePetGroupMap[owner_id];
			if (dobj3.pobj) {
				newGroup = (NetGroup*) dobj3.pobj;
			} else {
				newGroup = SafeCreateObject(NetGroup);
				dobj3.pobj = newGroup;
			}
		}

		for (UInt16 i = 0; i < petGroup->GetCount(); ++i)
		{
			pPetData = petGroup->GetChild<CPetData>(i);
			if (pPetData && pPetData->GetOwner_id() == owner_id)
			{
				// need to register for new pet data
				DataObject &dobj = gGlobal.petIdMap[pPetData->GetPet_uid()];
				if (dobj.pobj)
				{
					DeleteNetObject(pPetData);
					pPetData = CastNetObject<CPetData>(dobj.pobj);
					if (pPetData->GetOwner_id() != owner_id)
					{
						TRACE_WARNDTL_4(GLOBAL_LOGGER, 
							_FW("not owned, pet_uid %d, pet_id %d, owner_id %d, new data owner_id %d"),
							pPetData->GetPet_uid(), pPetData->GetPet_id(), 
							owner_id, pPetData->GetOwner_id());
						pPetData = NULL;
					}
				}
				else
				{
					LocalRPCManager::GetInstance().Register(pPetData);
					pPetData->ClearUpdateMask();
					dobj.pobj = pPetData;
				}
				if (pPetData)
					newGroup->Add(pPetData);
			}
		}

		// send pet data to peer
		PreSendPetGrp(newGroup);
		CB_PetList(pPeer, context, newGroup);
		DeleteNetGroup(petGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdatePetList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &petList, NetGroup* grp, const Boolean &separated)
{
	TRACE_ENSURE(pdbPet && grp);

	HRESULT hr = S_OK;
	NetGroup newPets;
	UInt16 i = 0;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("pet_uid %s"), GetVectorString(petList).c_str());

	for (Index idx = C_INDEX(0); idx < petList.size(); ++idx)
	{
		const UInt32& pet_uid = petList[idx];
		CPetData* pNewData = NULL;
		CPetData* pPetData = NULL;
		NetGroup* petGroup = NULL;
		DataIdMap::iterator iter = gGlobal.petIdMap.end();

		if (pet_uid != 0)
		{
			iter = gGlobal.petIdMap.find(pet_uid);
			if (iter != gGlobal.petIdMap.end())
			{
				// found existing Item in cache
				pPetData = CastNetObject<CPetData>(iter->second.pobj);
				TRACE_ENSURE(pPetData);
				if (petGroup == NULL)
				{
					if (pPetData->GetInWareHouse() == 0)
						petGroup = FindPetGroup(pPetData->GetOwner_id());
					else
						petGroup = FindWareHousePetGroup(pPetData->GetOwner_id());
				}
				TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("found pet data, owner_id %d, pet_uid %d"), pPetData->GetOwner_id(), pet_uid);
			}
			else
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("pet_uid not found, pet_uid %d"), pet_uid);
				continue;
			}
		}

		if (pNewData == NULL || separated)
		{
			// separated data
			if (idx < grp->GetCount())
			{
				pNewData = grp->GetChild<CPetData>(idx);
			}
			else
			{
				TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("no more data in NetGroup, count %d, pet_uid %d"), grp->GetCount(), pet_uid);
				continue;
			}
			if (pNewData == NULL)
			{
				// skip if null data
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("invalid pet data, pet_uid %d"), pet_uid);
				continue;
			}
		}

		if (pet_uid != pNewData->GetPet_uid())
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("pet_uid not match new data pet_uid, pet_uid %d, new data pet_uid %d"),
				pet_uid, pNewData->GetPet_uid());
			continue;
		}

		// remove pet if owner_id = 0
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("pet_uid %d, owner_id %d, pet_id %d"), pet_uid, pNewData->GetOwner_id(), pNewData->GetPet_id());
		if (pNewData->IsUpdated(CPetData::owner_idMask) && pNewData->GetOwner_id() == 0)
		{
			hr = RemovePet(pet_uid);
		}
		else if (pet_uid == 0)
		{
			// create pet if pet_uid == 0
			pPetData = pNewData;
			hr = DoCreatePet(pPetData->GetPet_id(), pPetData->GetLevel(), pPetData->GetOwner_id(),
				pPetData->GetFather_id(), pPetData->GetFather_name(), pPetData->GetMother_id(), 
				pPetData->GetMother_name(), pPetData->GetFirstMaster(), pPetData);
			if (DB_FAILED(hr))
			{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("DB_FAILED pet create failed, hr 0x%08X"), hr);
				ShowPetError(hr);
				CB_Error(pPeer, context, hr, _FE("DB_FAILED pet create failed"));
				break;
			}
			pPetData->PreSend();
			newPets.Add(pPetData);
		}
		else
		{
			// else update pet			
			if (pNewData->IsUpdated(CPetData::owner_idMask) && pNewData->GetOwner_id() != pPetData->GetOwner_id())
			{
				TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("change pet owner, pet_uid %d, from char_id %d to char_id %d"),
					pet_uid, pPetData->GetOwner_id(), pNewData->GetOwner_id());

				//change the ownerID in the GroupMap
				NetGroup* tempPetGroup = NULL;
				if (pPetData->GetInWareHouse() == 0)
					tempPetGroup = FindPetGroup(pPetData->GetOwner_id());
				else
					tempPetGroup = FindWareHousePetGroup(pPetData->GetOwner_id());

				if (tempPetGroup && tempPetGroup->GetCount() > 0)
					tempPetGroup->Remove(pPetData);

				if (pNewData->IsUpdated(CPetData::inWareHouseMask) && pNewData->GetInWareHouse() == 1)
				{
					tempPetGroup = FindWareHousePetGroup(pNewData->GetOwner_id());
					if (tempPetGroup == NULL)
					{
						DataObject& dobj3 = gGlobal.wareHousePetGroupMap[pNewData->GetOwner_id()];
						if (dobj3.pobj)
						{
							tempPetGroup = (NetGroup*) dobj3.pobj;
						}
						else
						{
							tempPetGroup = SafeCreateObject(NetGroup);
							dobj3.pobj = tempPetGroup;
						}
					}
				}
				else
				{
					tempPetGroup = FindPetGroup(pNewData->GetOwner_id());
					if (tempPetGroup == NULL)
					{
						DataObject& dobj3 = gGlobal.petGroupMap[pNewData->GetOwner_id()];
						if (dobj3.pobj)
						{
							tempPetGroup = (NetGroup*) dobj3.pobj;
						}
						else
						{
							tempPetGroup = SafeCreateObject(NetGroup);
							dobj3.pobj = tempPetGroup;
						}
					}
				}
				tempPetGroup->Add(pPetData);
			}
			else if (pNewData->IsUpdated(CPetData::inWareHouseMask) &&
				pPetData->GetInWareHouse() != pNewData->GetInWareHouse())
			{
				NetGroup* srcGrp = NULL;
				NetGroup* desGrp = NULL;
				
				if (pPetData->GetInWareHouse() == 0)
				{
					srcGrp = FindPetGroup(pPetData->GetOwner_id());
					desGrp = FindWareHousePetGroup(pPetData->GetOwner_id());
				}
				else
				{
					srcGrp = FindWareHousePetGroup(pPetData->GetOwner_id());
					desGrp = FindPetGroup(pPetData->GetOwner_id());					
				}

				if (srcGrp && desGrp)
				{
					srcGrp->Remove(pPetData);
					desGrp->Add(pPetData);
				}				
			}
			pPetData->Clone(pNewData, pNewData->GetUpdateMask());
			gGlobal.EnqueueUpdate(&iter->second);
		}
	}
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	pdbPet->Close();

	if (DB_FAILED(hr))
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("DB_FAILED, hr 0x%08X"), hr);
		ShowPetError(hr);
		DeleteNetChilds(&newPets);
		return RPC_RESULT_FAIL;
	}

	CB_UpdatePetList(pPeer, context, &newPets);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::SetPetLock(LPCPEER pPeer, RPCContext &context, const UInt32 &petUID, const Boolean &lock)
{
	HRESULT hr = S_OK;
	TRACE_ENSURE(pdbPet);	
	DataIdMap::iterator itfind = gGlobal.petIdMap.find(petUID);
	if (itfind != gGlobal.petIdMap.end())
	{
		CPetData* pData = CastNetObject<CPetData>(itfind->second.pobj);
		if (pData)
			pData->SetLock(lock);
	}
	Boolean success = DB_SUCCEEDED(hr = pdbPet->SetLock(petUID, lock));
	pdbPet->Close();
	CB_SetAssetLock(pPeer, context, success);

	return RPC_RESULT_OK;

}
