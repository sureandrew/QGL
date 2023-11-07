//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/PartnerCollectionData.h"
#include "ObjectService/dboPartnerCollection.h"
#include "Stub/Service/ObjectService.h"
#include "ObjectService/DboSourcePool.h"
#include "ObjectService/Global.h"

REGISTER_CLASSTYPE(CPartnerCollectionData, PartnerCollectionData_CLASSID);

CdboPartnerCollection* &pdbPartnerCollection = gGlobal.gTables.pPartnerCollection;
NetGroup* FindPartnerCollectionGroup(UInt32 char_id);
void PreSendGroup(NetGroup * petGroup);
StringVector CdboPartnerCollectionAccessor::m_params;
ThreadKey CPartnerCollectionData::m_key;
typedef WorkRequest<UInt32> PartnerCollectionRequest;

CPartnerCollectionData* NewPartnerCollection()
{
	CPartnerCollectionData *pPartnerCollectionData = NULL;
	UInt32 collection_uid = (UInt32) pdbPartnerCollection->m_collection_uid.LowPart;
	UInt32 collection_id = (UInt32) pdbPartnerCollection->m_collection_id.LowPart;
	UInt32 char_id = (UInt32) pdbPartnerCollection->m_char_id.LowPart;
	DataObject &dobj = gGlobal.partnerCollectionIdMap[collection_uid];

	if (dobj.pobj == NULL) {
		pPartnerCollectionData = CPartnerCollectionData::NewInstance();
		pPartnerCollectionData->SetData(pdbPartnerCollection);			// assign values to character data
		dobj.pobj = pPartnerCollectionData;
	} else {
		pPartnerCollectionData = CastNetObject<CPartnerCollectionData>(dobj.pobj);
	}
	pPartnerCollectionData->PreSend();

	// found existing pets in cache
	NetGroup* partnerCollectionGroup = FindPartnerCollectionGroup(char_id);
	if (partnerCollectionGroup)
	{
		partnerCollectionGroup->Add(pPartnerCollectionData);
	}
	else
	{
		DataObject& dobj3 = gGlobal.partnerCollectionGroupMap[char_id];
		if (dobj3.pobj == NULL) {
			partnerCollectionGroup = SafeCreateObject(NetGroup);
			dobj3.pobj = partnerCollectionGroup;
		} else {
			partnerCollectionGroup = (NetGroup*) dobj3.pobj;
		}
		partnerCollectionGroup->Add(pPartnerCollectionData);
	}

	return pPartnerCollectionData;
}

NetGroup* FindPartnerCollectionGroup(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.partnerCollectionGroupMap.find(char_id);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.partnerCollectionGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}

CPartnerCollectionData* FindPartnerCollection(UInt32 collection_uid)
{
	DataIdMap::iterator iter = gGlobal.partnerCollectionIdMap.find(collection_uid);
	CPartnerCollectionData* pPartnerCollectionData = NULL;

	// found existing account in cache
	if (iter != gGlobal.petIdMap.end())
		pPartnerCollectionData = CastNetObject<CPartnerCollectionData>(iter->second.pobj);

	return pPartnerCollectionData;
}

HRESULT RemovePartnerCollection(UInt32 collection_uid)
{
	DataIdMap::iterator iter = gGlobal.partnerCollectionIdMap.find(collection_uid);
	if (iter != gGlobal.partnerCollectionIdMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CPartnerCollectionData* pPartnerCollectionData = CastNetObject<CPartnerCollectionData>(iter->second.pobj);
		if (pPartnerCollectionData) {
			NetGroup* partnerCollectionGroup = FindPartnerCollectionGroup(pPartnerCollectionData->GetChar_id());
			if (partnerCollectionGroup) {
				partnerCollectionGroup->Remove(pPartnerCollectionData);
				TRACE_INFODTL_3(GLOBAL_LOGGER, _F("RemovePartnerCollection: Char_id = %d, Collection_uid = %d,, Collection_id = %d"), 
					pPartnerCollectionData->GetChar_id(), pPartnerCollectionData->GetCollection_uid(),pPartnerCollectionData->GetCollection_id());
			}
			gGlobal.m_deleteGroup.Add(pPartnerCollectionData);
		}
		gGlobal.partnerCollectionIdMap.erase(iter);
	}

	return S_OK;
}

void ReleasePartnerCollections(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.partnerCollectionGroupMap.find(char_id);

	if (iter != gGlobal.partnerCollectionGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		CPartnerCollectionData *pPartnerCollectionData;

		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++) {
			pPartnerCollectionData = grp->GetChild<CPartnerCollectionData>(i);
			if (pPartnerCollectionData) {
				DataIdMap::iterator it = gGlobal.partnerCollectionIdMap.find(pPartnerCollectionData->GetCollection_uid());
				if (it != gGlobal.partnerCollectionIdMap.end()) {		// remove from update queue
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.partnerCollectionIdMap.erase(it);
				}
				if (pPartnerCollectionData->IsUpdated())				// write to db if updated
					gGlobal.m_releaseGroup.Add(pPartnerCollectionData);
				else
					DeleteNetObject(pPartnerCollectionData);
			}
		}
		gGlobal.partnerCollectionGroupMap.erase(iter);
		SafeDeleteObject(grp);
	}
}

DWORD WINAPI QueueGetPartnerCollections(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();

	PartnerCollectionRequest* pRequest = (PartnerCollectionRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
	{
		NetGroup partnerCollectionGroup;
		CPartnerCollectionData* pPartnerCollectionData;
		CdboPartnerCollection* dboPartnerCollection = SafeCreateObject(CdboPartnerCollection);

		partnerCollectionGroup.SetValidate(BOOLEAN_FALSE);
		dboPartnerCollection->SetDataSource(dbSrc);
		if (DB_SUCCEEDED(hr = dboPartnerCollection->RSByCharID(pRequest->key)))
		{
			hr = dboPartnerCollection->MoveFirst();
			while (hr == S_OK)
			{
				pPartnerCollectionData = SafeCreateObject(CPartnerCollectionData);
				pPartnerCollectionData->SetData(dboPartnerCollection);
				pPartnerCollectionData->PreSend();
				partnerCollectionGroup.Add(pPartnerCollectionData);
				hr = dboPartnerCollection->MoveNext();
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetPartnerCollections"), hr, dboPartnerCollection->GetErrorMsg(hr), 
			pRequest->context, &partnerCollectionGroup);
		dboPartnerCollection->Close();
		SafeDeleteObject(dboPartnerCollection);
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

HRESULT CreatePartnerCollection(UInt32 collection_id, UInt32 char_id, CPartnerCollectionData* &pPartnerCollectionData)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreatePartnerCollection: collection_id %d, char_id %d"), collection_id, char_id);

	HRESULT hr = S_OK;
	// get mob data from resource
	//const PetData* data = gGlobal.m_resource->GetPetData(pet_id);
	//if (data == NULL)
	//{
	//	TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CreatePet DB_FAILED: pet_id %d not found in resource"), pet_id);
	//	return S_FALSE;
	//}

	pPartnerCollectionData = NULL;
	pdbPartnerCollection->m_collection_id.QuadPart = collection_id;
	pdbPartnerCollection->m_char_id.QuadPart = char_id;

	hr = pdbPartnerCollection->Insert(collection_id, char_id);

	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbPartnerCollection->MoveFirst()))
	{
		pPartnerCollectionData = NewPartnerCollection();

	}
	pdbPartnerCollection->Close();

	return hr;
}


void CPartnerCollectionData::SetData(CdboPartnerCollection* dboPartnerCollection)
{
	SetCollection_uid((UInt32)dboPartnerCollection->m_collection_uid.LowPart);
	SetCollection_id((UInt32)dboPartnerCollection->m_collection_id.LowPart);
	SetChar_id((UInt32)dboPartnerCollection->m_char_id.LowPart);

	ClearUpdateMask();
}

void CPartnerCollectionData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

HRESULT CPartnerCollectionData::DeleteDbo(CdboPartnerCollection* dboPartnerCollection)
{
	HRESULT hr;

	//ThreadGuard guard(m_key);
	if (DB_FAILED(hr = dboPartnerCollection->Delete(GetChar_id())))
		dboPartnerCollection->ShowError(hr);
	dboPartnerCollection->Close();

	return hr;
}

HRESULT CPartnerCollectionData::UpdateDbo(CdboPartnerCollection* dboPartnerCollection)
{
	return RPC_RESULT_OK;
}


RPCResult ObjectService_Stub::GetPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC1(ObjectService, GetPartnerCollection, NULL, UInt32, char_id)

		TRACE_ENSURE(pdbPartnerCollection != NULL);
		CPartnerCollectionData* pPartnerCollectionData = NULL;
		NetGroup* partnerCollectionGroup = FindPartnerCollectionGroup(char_id);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("char_id %d"), char_id);

		if (partnerCollectionGroup)
		{
			for (UInt16 i = 0; i < partnerCollectionGroup->GetCount(); i++)
			{
				CPartnerCollectionData *pPartnerCollectionData = partnerCollectionGroup->GetChild<CPartnerCollectionData>(i);
				if (pPartnerCollectionData)
					pPartnerCollectionData->PreSend();
			}
			// directly send back object group
			CB_GetPartnerCollection(pPeer, context, partnerCollectionGroup);
			PROCEDURE_RETURN
		}
		else
		{
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			PartnerCollectionRequest *pRequest = SafeCreateObject3(PartnerCollectionRequest, char_id, pPeer, context);
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetPartnerCollections, pRequest, DB_WORKER_FLAG);
#else
			QueueGetPartnerCollections(pRequest);
#endif
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, partnerCollectionGroup)

		TRACE_ENSURE(partnerCollectionGroup);

		// store new Partner Collection group
		CPartnerCollectionData* pPartnerCollectionData;
		DataObject& dobj3 = gGlobal.partnerCollectionGroupMap[char_id];
		NetGroup* newGroup = NULL;
		if (dobj3.pobj)
		{
			newGroup = (NetGroup*) dobj3.pobj;
		}
		else
		{
			newGroup = SafeCreateObject(NetGroup);
			dobj3.pobj = newGroup;
		}

		for (UInt16 i = 0; i < partnerCollectionGroup->GetCount(); ++i)
		{
			pPartnerCollectionData = partnerCollectionGroup->GetChild<CPartnerCollectionData>(i);
			if (pPartnerCollectionData)
			{
				DataIdMap::iterator itr = gGlobal.partnerCollectionIdMap.find(pPartnerCollectionData->GetCollection_uid());
				if (itr != gGlobal.partnerCollectionIdMap.end())
				{
					CPartnerCollectionData* pData = CastNetObject<CPartnerCollectionData>(itr->second.pobj);
					if (pData == NULL)
					{
						DeleteNetObject(itr->second.pobj);
						gGlobal.partnerCollectionIdMap.erase(itr);
						TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("cast old pet data failed, pet_uid %d"),
							pPartnerCollectionData->GetCollection_uid());
					}
					else
					{	
						if (pData->GetChar_id() != char_id)
						{
							TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("not owned, pet_uid %d, pet_id %d, owner_id %d, new data owner_id %d"),
								pPartnerCollectionData->GetCollection_uid(), pPartnerCollectionData->GetCollection_id(), char_id, pPartnerCollectionData->GetChar_id());
							DeleteNetObject(pPartnerCollectionData);
						}
						else
							newGroup->Add(pPartnerCollectionData);
						continue;
					}
				}
				// need to register for new pet data
				LocalRPCManager::GetInstance().Register(pPartnerCollectionData);
				DataObject &dobj = gGlobal.partnerCollectionIdMap[pPartnerCollectionData->GetCollection_uid()];
				if (dobj.pobj)
				{
					DeleteNetObject(pPartnerCollectionData);
					pPartnerCollectionData = CastNetObject<CPartnerCollectionData>(dobj.pobj);
				}
				else
				{
					dobj.pobj = pPartnerCollectionData;
				}
				newGroup->Add(pPartnerCollectionData);
			}
		}

		for (UInt16 i = 0; i < partnerCollectionGroup->GetCount(); ++i)
		{
			CPartnerCollectionData *pPartnerCollectionData = partnerCollectionGroup->GetChild<CPartnerCollectionData>(i);
			if (pPartnerCollectionData)
				pPartnerCollectionData->PreSend();
		}

		// send pet data to peer
		CB_GetPartnerCollection(pPeer, context, partnerCollectionGroup);
		DeleteNetGroup(partnerCollectionGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::CreatePartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &colloction_id, const UInt32 &char_id)
{
	TRACE_ENSURE(pdbPartnerCollection != NULL);
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreatePartnerCollection: colloction_id = %d, char_id = %d"), colloction_id, char_id);

	HRESULT hr = S_OK;
	CPartnerCollectionData* pPartnerCollectionData = NULL;

	hr = CreatePartnerCollection(colloction_id, char_id, pPartnerCollectionData);
	if (DB_FAILED(hr)) {
		pdbPartnerCollection->ShowError(hr);
		CB_Error(pPeer, context, hr, _T("CreatePartnerCollection DB_FAILED:"));
		return RPC_RESULT_FAIL;
	}

	if (pPartnerCollectionData)
		pPartnerCollectionData->PreSend();
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CreatePartnerCollection: (%d, %d, %d)"),
		pPartnerCollectionData->GetCollection_uid(), pPartnerCollectionData->GetCollection_id(),pPartnerCollectionData->GetChar_id());

	CB_CreatePartnerCollection(pPeer, context, pPartnerCollectionData);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::DeletePartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &collection_uid)
{
	HRESULT hr = S_OK;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("DeletePartnerCollections collection_uid %d"), collection_uid);

	if (DB_FAILED(hr = RemovePartnerCollection(collection_uid)))
	{
		pdbPartnerCollection->ShowError(hr);
		return CB_Error(pPeer, context, hr, _FE("DB_FAILED"));
	}

	CB_DeletePartnerCollection(pPeer, context);
	return RPC_RESULT_OK;
}

