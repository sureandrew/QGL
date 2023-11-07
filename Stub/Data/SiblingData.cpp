//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/SiblingData.h"
#include "Stub/Data/CharacterData.h"
#include "ObjectService/dboSibling.h"
#include "ObjectService/Global.h"
#include "Stub/Service/ObjectService.h"

CdboSibling* &pdbSibling = gGlobal.gTables.pSibling;
StringVector CdboSiblingAccessor::m_params;
ThreadKey CSiblingData::m_key;
typedef WorkRequest<UInt32> SiblingRequest;

REGISTER_CLASSTYPE(CSiblingData, SiblingData_CLASSID);

CSiblingData* CSiblingData::NewSibling()
{
	CSiblingData *pSiblingData = NULL;
	UInt32 sibling_id = (UInt32) pdbSibling->m_sibling_id.LowPart;
	DataObject &dobj = gGlobal.siblingIdMap[sibling_id];

	if (dobj.pobj == NULL) 
	{
		pSiblingData = CSiblingData::NewInstance();
		pSiblingData->SetData(pdbSibling);			// assign values to character data
		dobj.pobj = pSiblingData;
	} else 
	{
		pSiblingData = CastNetObject<CSiblingData>(dobj.pobj);
	}

	return pSiblingData;
}

CSiblingData* CSiblingData::FindSibling(UInt32 sibling_id)
{
	DataIdMap::iterator iter = gGlobal.siblingIdMap.find(sibling_id);
	CSiblingData* pSiblingData = NULL;

	// found existing account in cache
	if (iter != gGlobal.siblingIdMap.end())
		pSiblingData = CastNetObject<CSiblingData>(iter->second.pobj);

	return pSiblingData;
}

void CSiblingData::SetData(CdboSibling* dboSibling)
{
	SetSibling_id((UInt32)dboSibling->m_sibling_id.LowPart);
	SetSibling_title_prefix(dboSibling->m_sibling_title_prefix);
	SetMember_id1((UInt32)dboSibling->m_member_id1.LowPart);
	SetMember_title_suffix1(dboSibling->m_member_title_suffix1);
	SetMember_id2((UInt32)dboSibling->m_member_id2.LowPart);
	SetMember_title_suffix2(dboSibling->m_member_title_suffix2);
	SetMember_id3((UInt32)dboSibling->m_member_id3.LowPart);
	SetMember_title_suffix3(dboSibling->m_member_title_suffix3);
	SetMember_id4((UInt32)dboSibling->m_member_id4.LowPart);
	SetMember_title_suffix4(dboSibling->m_member_title_suffix4);
	SetMember_id5((UInt32)dboSibling->m_member_id5.LowPart);
	SetMember_title_suffix5(dboSibling->m_member_title_suffix5);
	SetMember_id6((UInt32)dboSibling->m_member_id6.LowPart);
	SetMember_title_suffix6(dboSibling->m_member_title_suffix6);
	SetMember_id7((UInt32)dboSibling->m_member_id7.LowPart);
	SetMember_title_suffix7(dboSibling->m_member_title_suffix7);
	SetMember_id8((UInt32)dboSibling->m_member_id8.LowPart);
	SetMember_title_suffix8(dboSibling->m_member_title_suffix8);
	SetMember_id9((UInt32)dboSibling->m_member_id9.LowPart);
	SetMember_title_suffix9(dboSibling->m_member_title_suffix9);
	SetMember_id10((UInt32)dboSibling->m_member_id10.LowPart);
	SetMember_title_suffix10(dboSibling->m_member_title_suffix10);
	ClearUpdateMask();
}

HRESULT CSiblingData::UpdateDbo(CdboSibling* dboSibling)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("sibling_id %d, update_mask %d,%d"), 
		this->GetSibling_id(), m_updateMask.low, m_updateMask.high);

	dboSibling->ClearParam();

	if (IsUpdated(sibling_title_prefixMask)) COLUMN_UPDATE_STRING(dboSibling, sibling_title_prefix, GetSibling_title_prefix());
	if (IsUpdated(member_id1Mask)) COLUMN_UPDATE_LARGE_QUA(dboSibling, member_id1, GetMember_id1());
	if (IsUpdated(member_title_suffix1Mask)) COLUMN_UPDATE_STRING(dboSibling, member_title_suffix1, GetMember_title_suffix1());
	if (IsUpdated(member_id2Mask)) COLUMN_UPDATE_LARGE_QUA(dboSibling, member_id2, GetMember_id2());
	if (IsUpdated(member_title_suffix2Mask)) COLUMN_UPDATE_STRING(dboSibling, member_title_suffix2, GetMember_title_suffix2());
	if (IsUpdated(member_id3Mask)) COLUMN_UPDATE_LARGE_QUA(dboSibling, member_id3, GetMember_id3());
	if (IsUpdated(member_title_suffix3Mask)) COLUMN_UPDATE_STRING(dboSibling, member_title_suffix3, GetMember_title_suffix3());
	if (IsUpdated(member_id4Mask)) COLUMN_UPDATE_LARGE_QUA(dboSibling, member_id4, GetMember_id4());
	if (IsUpdated(member_title_suffix4Mask)) COLUMN_UPDATE_STRING(dboSibling, member_title_suffix4, GetMember_title_suffix4());
	if (IsUpdated(member_id5Mask)) COLUMN_UPDATE_LARGE_QUA(dboSibling, member_id5, GetMember_id5());
	if (IsUpdated(member_title_suffix5Mask)) COLUMN_UPDATE_STRING(dboSibling, member_title_suffix5, GetMember_title_suffix5());
	if (IsUpdated(member_id6Mask)) COLUMN_UPDATE_LARGE_QUA(dboSibling, member_id6, GetMember_id6());
	if (IsUpdated(member_title_suffix6Mask)) COLUMN_UPDATE_STRING(dboSibling, member_title_suffix6, GetMember_title_suffix6());
	if (IsUpdated(member_id7Mask)) COLUMN_UPDATE_LARGE_QUA(dboSibling, member_id7, GetMember_id7());
	if (IsUpdated(member_title_suffix7Mask)) COLUMN_UPDATE_STRING(dboSibling, member_title_suffix7, GetMember_title_suffix7());
	if (IsUpdated(member_id8Mask)) COLUMN_UPDATE_LARGE_QUA(dboSibling, member_id8, GetMember_id8());
	if (IsUpdated(member_title_suffix8Mask)) COLUMN_UPDATE_STRING(dboSibling, member_title_suffix8, GetMember_title_suffix8());
	if (IsUpdated(member_id9Mask)) COLUMN_UPDATE_LARGE_QUA(dboSibling, member_id9, GetMember_id9());
	if (IsUpdated(member_title_suffix9Mask)) COLUMN_UPDATE_STRING(dboSibling, member_title_suffix9, GetMember_title_suffix9());
	if (IsUpdated(member_id10Mask)) COLUMN_UPDATE_LARGE_QUA(dboSibling, member_id10, GetMember_id10());
	if (IsUpdated(member_title_suffix10Mask)) COLUMN_UPDATE_STRING(dboSibling, member_title_suffix10, GetMember_title_suffix10());

	HRESULT hr;
	if (DB_FAILED(hr = dboSibling->UpdateByAny(GetSibling_id())))
		dboSibling->ShowError(hr);
	else
		ClearUpdateMask();
	dboSibling->ClearParam();
	dboSibling->Close();

	return hr;
}

HRESULT CSiblingData::DeleteDbo(CdboSibling* dboSibling)
{
	HRESULT hr;

	if (DB_FAILED(hr = dboSibling->Delete(GetSibling_id())))
		dboSibling->ShowError(hr);
	dboSibling->Close();

	return hr;
}

void CSiblingData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

DWORD WINAPI CSiblingData::QueueGetSibling(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();
	SiblingRequest* pRequest = (SiblingRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();

	if (dbSrc)
	{
		NetGroup siblingCroup;
		CSiblingData* pSiblingData;
		CdboSibling* dboSibling = SafeCreateObject(CdboSibling);

		siblingCroup.SetValidate(BOOLEAN_FALSE);
		dboSibling->SetDataSource(dbSrc);
		if (DB_SUCCEEDED(hr = dboSibling->RSBySiblingID(pRequest->key)))
		{
			hr = dboSibling->MoveFirst();
			while (hr == S_OK)
			{
				pSiblingData = SafeCreateObject(CSiblingData);
				pSiblingData->SetData(dboSibling);
				pSiblingData->PreSend();
				siblingCroup.Add(pSiblingData);
				hr = dboSibling->MoveNext();
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetSibling"), hr, dboSibling->GetErrorMsg(hr), 
			pRequest->context, &siblingCroup);
		dboSibling->Close();
		SafeDeleteObject(dboSibling);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, sibling_id %d"),
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

HRESULT CSiblingData::RemoveSibling(UInt32 sibling_id)
{
	DataIdMap::iterator iter = gGlobal.siblingIdMap.find(sibling_id);
	if (iter != gGlobal.siblingIdMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CSiblingData* pSiblingData = CastNetObject<CSiblingData>(iter->second.pobj);
		if (pSiblingData) 
		{
			gGlobal.m_deleteGroup.Add(pSiblingData);
		}
		gGlobal.siblingIdMap.erase(iter);
	}

	return S_OK;
}

RPCResult ObjectService_Stub::GetSibling(LPCPEER pPeer, RPCContext &context, const UInt32 &sibling_id)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC1(ObjectService, GetSibling, NULL, UInt32, sibling_id)

	TRACE_ENSURE(pdbSibling != NULL);
	CSiblingData* pSiblingData = CSiblingData::FindSibling(sibling_id);

	if (pSiblingData || sibling_id == 0)
	{
		// directly send back object group
		NetGroup sendGrp;
		if(pSiblingData)
		{
			pSiblingData->PreSend();
			sendGrp.Add(pSiblingData);
		}
		CB_GetSibling(pPeer, context, &sendGrp);
		PROCEDURE_RETURN
	}
	else
	{
		// wait for thread pool to process
		gGlobal.WorkQueueBegin();
		SiblingRequest *pRequest = SafeCreateObject3(SiblingRequest, sibling_id, pPeer, context);
#ifdef DB_THREAD_POOL
		gGlobal.QueueUserWorkItem(CSiblingData::QueueGetSibling, pRequest, DB_WORKER_FLAG);
#else
		CSiblingData::QueueGetSibling(pRequest);
#endif
	}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, netGroup)

	TRACE_ENSURE(netGroup);
	CSiblingData* pSiblingData = NULL;

	if(netGroup->GetCount() >= 1)
	{
		pSiblingData = netGroup->GetChild<CSiblingData>(0);

		if (pSiblingData)
		{
			// need to register for new sibling data
			DataObject &dobj = gGlobal.siblingIdMap[pSiblingData->GetSibling_id()];
			if (dobj.pobj)
			{
				DeleteNetObject(pSiblingData);
				pSiblingData = CastNetObject<CSiblingData>(dobj.pobj);
			}
			else
			{
				LocalRPCManager::GetInstance().Register(pSiblingData);
				pSiblingData->ClearUpdateMask();
				dobj.pobj = pSiblingData;
			}
		}
	}

	NetGroup sendGrp;
	if (pSiblingData)
	{
		pSiblingData->PreSend();
		sendGrp.Add(pSiblingData);
	}
	// send sibling data to peer
	CB_GetSibling(pPeer, context, &sendGrp);
	DeleteNetGroup(netGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
		_FE("session_id %s, errcode %d, %s"),
		context.sessionId.GetString().c_str(), err, errMsg.c_str());
		CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}
void CSiblingData::ReleaseSibling(UInt32 char_id)
{
	CCharacterData* pCharacterData = FindChar(char_id);
	if(pCharacterData == NULL)
	{
		return;
	}
	UInt32 sibling_id = pCharacterData->GetSibling_id();
	if(sibling_id == 0)
	{
		return;
	}

	DataIdMap::iterator iter = gGlobal.siblingIdMap.find(sibling_id);
	CSiblingData* pSiblingData = NULL;
	if (iter != gGlobal.siblingIdMap.end())
		pSiblingData = CastNetObject<CSiblingData>(iter->second.pobj);

	if (pSiblingData)
	{
		Boolean bDelete = BOOLEAN_TRUE;
		UInt32 nTmpCharId = 0;
		do 
		{
			nTmpCharId = pSiblingData->GetMember_id1();
			if(nTmpCharId != 0 && nTmpCharId != char_id && FindChar(nTmpCharId) != NULL) 
			{
				bDelete = BOOLEAN_FALSE;
				break;
			}
			nTmpCharId = pSiblingData->GetMember_id2();
			if(nTmpCharId != 0 && nTmpCharId != char_id && FindChar(nTmpCharId) != NULL) 
			{
				bDelete = BOOLEAN_FALSE;
				break;
			}
			nTmpCharId = pSiblingData->GetMember_id3();
			if(nTmpCharId != 0 && nTmpCharId != char_id && FindChar(nTmpCharId) != NULL) 
			{
				bDelete = BOOLEAN_FALSE;
				break;
			}
			nTmpCharId = pSiblingData->GetMember_id4();
			if(nTmpCharId != 0 && nTmpCharId != char_id && FindChar(nTmpCharId) != NULL) 
			{
				bDelete = BOOLEAN_FALSE;
				break;
			}
			nTmpCharId = pSiblingData->GetMember_id5();
			if(nTmpCharId != 0 && nTmpCharId != char_id && FindChar(nTmpCharId) != NULL) 
			{
				bDelete = BOOLEAN_FALSE;
				break;
			}
			nTmpCharId = pSiblingData->GetMember_id6();
			if(nTmpCharId != 0 && nTmpCharId != char_id && FindChar(nTmpCharId) != NULL) 
			{
				bDelete = BOOLEAN_FALSE;
				break;
			}
			nTmpCharId = pSiblingData->GetMember_id7();
			if(nTmpCharId != 0 && nTmpCharId != char_id && FindChar(nTmpCharId) != NULL) 
			{
				bDelete = BOOLEAN_FALSE;
				break;
			}
			nTmpCharId = pSiblingData->GetMember_id8();
			if(nTmpCharId != 0 && nTmpCharId != char_id && FindChar(nTmpCharId) != NULL) 
			{
				bDelete = BOOLEAN_FALSE;
				break;
			}
			nTmpCharId = pSiblingData->GetMember_id9();
			if(nTmpCharId != 0 && nTmpCharId != char_id && FindChar(nTmpCharId) != NULL) 
			{
				bDelete = BOOLEAN_FALSE;
				break;
			}
			nTmpCharId = pSiblingData->GetMember_id10();
			if(nTmpCharId != 0 && nTmpCharId != char_id && FindChar(nTmpCharId) != NULL) 
			{
				bDelete = BOOLEAN_FALSE;
				break;
			}
		}while(false);

		if(bDelete)
		{
			gGlobal.DequeueUpdate(&iter->second);
			gGlobal.siblingIdMap.erase(iter);
			if (pSiblingData->IsUpdated())				// write to db if updated
				gGlobal.m_releaseGroup.Add(pSiblingData);
			else
				DeleteNetObject(pSiblingData);
		}
	}
}
HRESULT CSiblingData::DoCreateSibling(const String &sibling_title_prefix, const UInt32 &member_id1
					, const String &member_title_suffix1, const UInt32 &member_id2
					, const String &member_title_suffix2, const UInt32 &member_id3
					, const String &member_title_suffix3, const UInt32 &member_id4
					, const String &member_title_suffix4, const UInt32 &member_id5
					, const String &member_title_suffix5, const UInt32 &member_id6
					, const String &member_title_suffix6, const UInt32 &member_id7
					, const String &member_title_suffix7, const UInt32 &member_id8
					, const String &member_title_suffix8, const UInt32 &member_id9
					, const String &member_title_suffix9, const UInt32 &member_id10
					, const String &member_title_suffix10,CSiblingData* &pSiblingData)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CreateSibling: sibling_title_prefix %s"), sibling_title_prefix.c_str());

	pSiblingData = NULL;

	HRESULT hr = pdbSibling->Insert(sibling_title_prefix, member_id1
		,member_title_suffix1, member_id2
		,member_title_suffix2, member_id3
		,member_title_suffix3, member_id4
		,member_title_suffix4, member_id5
		,member_title_suffix5, member_id6
		,member_title_suffix6, member_id7
		,member_title_suffix7, member_id8
		,member_title_suffix8, member_id9
		,member_title_suffix9, member_id10
		,member_title_suffix10);

	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbSibling->MoveFirst()))
	{
		pSiblingData = NewSibling();

	}
	pdbSibling->Close();

	return hr;
}


RPCResult ObjectService_Stub::CreateSibling(LPCPEER pPeer, RPCContext &context
											, const String &sibling_title_prefix, const UInt32 &member_id1
											, const String &member_title_suffix1, const UInt32 &member_id2
											, const String &member_title_suffix2, const UInt32 &member_id3
											, const String &member_title_suffix3, const UInt32 &member_id4
											, const String &member_title_suffix4, const UInt32 &member_id5
											, const String &member_title_suffix5, const UInt32 &member_id6
											, const String &member_title_suffix6, const UInt32 &member_id7
											, const String &member_title_suffix7, const UInt32 &member_id8
											, const String &member_title_suffix8, const UInt32 &member_id9
											, const String &member_title_suffix9, const UInt32 &member_id10
											, const String &member_title_suffix10)
{
	TRACE_ENSURE(pdbSibling != NULL);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CreateSibling: sibling_title_prefix = %s"), sibling_title_prefix.c_str());

	HRESULT hr = S_OK;
	CSiblingData* pSiblingData = NULL;
	NetGroup sendGrp;
	hr = CSiblingData::DoCreateSibling(sibling_title_prefix, member_id1
						,member_title_suffix1, member_id2
						,member_title_suffix2, member_id3
						,member_title_suffix3, member_id4
						,member_title_suffix4, member_id5
						,member_title_suffix5, member_id6
						,member_title_suffix6, member_id7
						,member_title_suffix7, member_id8
						,member_title_suffix8, member_id9
						,member_title_suffix9, member_id10
						,member_title_suffix10,pSiblingData );
	if (DB_FAILED(hr)) 
	{
		pdbSibling->ShowError(hr);
		CB_Error(pPeer, context, hr, _T("CreateSibling DB_FAILED:"));
		CB_CreateSibling(pPeer, context, &sendGrp);
		return RPC_RESULT_FAIL;
	}
	
	if (pSiblingData)
	{
		pSiblingData->PreSend();
		sendGrp.Add(pSiblingData);
	}

	CB_CreateSibling(pPeer, context, &sendGrp);

	return RPC_RESULT_OK; 

}

RPCResult ObjectService_Stub::UpdateSibling(LPCPEER pPeer, RPCContext &context, NetObject* netObject)
{
	TRACE_ENSURE(pdbSibling);

	CSiblingData *pNewSiblingData = CastNetObject<CSiblingData>(netObject);
	if(pNewSiblingData == NULL) return RPC_RESULT_OK;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("sibling_id %d"), pNewSiblingData->GetSibling_id());


	const UInt32 sibling_id = pNewSiblingData->GetSibling_id();
	CSiblingData *pSiblingData = NULL;
	DataIdMap::iterator iter = gGlobal.siblingIdMap.find(sibling_id);

	if (iter != gGlobal.siblingIdMap.end())
	{
		// found existing Sibling in cache
		pSiblingData = CastNetObject<CSiblingData>(iter->second.pobj);
		TRACE_ENSURE(pSiblingData);
		TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("found sibling data, sibling_id %d"), sibling_id);
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sibling_id not found, sibling_id %d"), sibling_id);
		return RPC_RESULT_OK;
	}

	NetGroup sendGrp;
	if(pSiblingData)
	{
		pSiblingData->Clone(pNewSiblingData, pNewSiblingData->GetUpdateMask());
		sendGrp.Add(pSiblingData);
		gGlobal.EnqueueUpdate(&iter->second);
	}
	
	if(pNewSiblingData != pSiblingData)
		DeleteNetObject(pNewSiblingData);
	else
		TRACE_VERBOSEDTL(GLOBAL_LOGGER, _FW("ObjectService_Stub::UpdateSibling  pNewSiblingData == pSiblingData"));

	CB_UpdateSibling(pPeer, context, &sendGrp);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::DeleteSibling(LPCPEER pPeer, RPCContext &context, const UInt32 &sibling_id)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("DeleteSibling: sibling_id %d"), sibling_id);
	CSiblingData::RemoveSibling(sibling_id);
	CB_DeleteSibling(pPeer,context,1);
	return RPC_RESULT_OK;
}

