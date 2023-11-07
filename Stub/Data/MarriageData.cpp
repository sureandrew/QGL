//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/MarriageData.h"
#include "Stub/Data/CharacterData.h"
#include "ObjectService/dboMarriage.h"
#include "ObjectService/Global.h"
#include "Stub/Service/ObjectService.h"

CdboMarriage* &pdbMarriage = gGlobal.gTables.pMarriage;
StringVector CdboMarriageAccessor::m_params;
ThreadKey CMarriageData::m_key;

struct MarriageRequest : public WorkRequest<UInt32>
{
	UInt32          nGroup;
	MarriageRequest(UInt32 id, LPCPEER pPeer, RPCContext &context,UInt32 nGroup)
		: WorkRequest<UInt32>(id, pPeer, context), nGroup(nGroup)
	{}
};

REGISTER_CLASSTYPE(CMarriageData, MarriageData_CLASSID);

CMarriageData* CMarriageData::NewMarriage()
{
	CMarriageData *pMarriageData = NULL;
	UInt32 marriage_id = (UInt32) pdbMarriage->m_marriage_id.LowPart;
	DataObject &dobj = gGlobal.marriageIdMap[marriage_id];

	if (dobj.pobj == NULL) 
	{
		pMarriageData = CMarriageData::NewInstance();
		pMarriageData->SetData(pdbMarriage);			// assign values to character data
		dobj.pobj = pMarriageData;
	} else 
	{
		pMarriageData = CastNetObject<CMarriageData>(dobj.pobj);
	}

	return pMarriageData;
}

CMarriageData* CMarriageData::FindMarriage(UInt32 marriage_id)
{
	DataIdMap::iterator iter = gGlobal.marriageIdMap.find(marriage_id);
	CMarriageData* pMarriageData = NULL;

	// found existing account in cache
	if (iter != gGlobal.marriageIdMap.end())
		pMarriageData = CastNetObject<CMarriageData>(iter->second.pobj);

	return pMarriageData;
}

CMarriageData* CMarriageData::FindMarriageByHusbandId(UInt32 husband_id)
{
	DataIdMap::iterator iter = gGlobal.marriageIdMap.begin();
	DataIdMap::iterator iterend = gGlobal.marriageIdMap.end();
	CMarriageData* pMarriageData = NULL;
	for(; iter != iterend; ++iter)
	{
		pMarriageData = CastNetObject<CMarriageData>(iter->second.pobj);
		if(pMarriageData->GetHusband_id() == husband_id)
		{
			return pMarriageData;
		}
	}

	return NULL;
}

CMarriageData* CMarriageData::FindMarriageByWifeId(UInt32 wife_id)
{
	DataIdMap::iterator iter = gGlobal.marriageIdMap.begin();
	DataIdMap::iterator iterend = gGlobal.marriageIdMap.end();
	CMarriageData* pMarriageData = NULL;
	for(; iter != iterend; ++iter)
	{
		pMarriageData = CastNetObject<CMarriageData>(iter->second.pobj);
		if(pMarriageData->GetWife_id() == wife_id)
		{
			return pMarriageData;
		}
	}

	return NULL;
}

void CMarriageData::SetData(CdboMarriage* dboMarriage)
{
	SetMarriage_id((UInt32)dboMarriage->m_marriage_id.LowPart);
	SetHusband_id((UInt32)dboMarriage->m_husband_id.LowPart);
	SetWife_id((UInt32)dboMarriage->m_wife_id.LowPart);
	time_t uTime;
	TimeConvert(dboMarriage->m_marriage_time, uTime);
	SetMarriage_time((UInt32)uTime);
	SetState((UInt32)dboMarriage->m_state);

	ClearUpdateMask();
}

HRESULT CMarriageData::UpdateDbo(CdboMarriage* dboMarriage)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("marriage_id %d, update_mask %d,%d"), 
		this->GetMarriage_id(), m_updateMask.low, m_updateMask.high);

	dboMarriage->ClearParam();
	if(IsUpdated(stateMask)) 
	{
		COLUMN_UPDATE(dboMarriage,state,GetState());
	}

	HRESULT hr;
	if (DB_FAILED(hr = dboMarriage->UpdateByAny(GetMarriage_id())))
		dboMarriage->ShowError(hr);
	else
		ClearUpdateMask();
	dboMarriage->ClearParam();
	dboMarriage->Close();

	return hr;
}

HRESULT CMarriageData::DeleteDbo(CdboMarriage* dboMarriage)
{
	HRESULT hr;

	if (DB_FAILED(hr = dboMarriage->Delete(GetMarriage_id())))
		dboMarriage->ShowError(hr);
	dboMarriage->Close();

	return hr;
}

void CMarriageData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

DWORD WINAPI CMarriageData::QueueGetMarriage(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();
	MarriageRequest* pRequest = (MarriageRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();

	if (dbSrc)
	{
		NetGroup marriageCroup;
		CMarriageData* pMarriageData;
		CdboMarriage* dboMarriage = SafeCreateObject(CdboMarriage);

		marriageCroup.SetValidate(BOOLEAN_FALSE);
		dboMarriage->SetDataSource(dbSrc);
		TRACE_INFODTL_2(GLOBAL_LOGGER,_F("pRequest->nGroup:%d pRequest->key:%d "),pRequest->nGroup,pRequest->key);

		if(pRequest->nGroup == 1)
			hr = dboMarriage->RSByHusbandID(pRequest->key);
		else if(pRequest->nGroup == 2)
			hr = dboMarriage->RSByWifeID(pRequest->key);
		else if(pRequest->nGroup == 3)
			hr = dboMarriage->RSByState(pRequest->key);;
		if (DB_SUCCEEDED(hr))
		{
			hr = dboMarriage->MoveFirst();
			while (hr == S_OK)
			{
				pMarriageData = SafeCreateObject(CMarriageData);
				pMarriageData->SetData(dboMarriage);
				pMarriageData->PreSend();
				marriageCroup.Add(pMarriageData);
				hr = dboMarriage->MoveNext();
				TRACE_INFODTL_1(GLOBAL_LOGGER,_F("pMarriageData:%d"),pMarriageData->GetMarriage_id());
			}
		}
		else
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER,_F("!DB_SUCCEEDED(hr):%d"),hr);
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetMarriage"), hr, dboMarriage->GetErrorMsg(hr),pRequest->context, &marriageCroup);
		dboMarriage->Close();
		SafeDeleteObject(dboMarriage);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, marriage_id %d"),
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

HRESULT CMarriageData::RemoveMarriage(UInt32 marriage_id)
{
	DataIdMap::iterator iter = gGlobal.marriageIdMap.find(marriage_id);
	if (iter != gGlobal.marriageIdMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CMarriageData* pMarriageData = CastNetObject<CMarriageData>(iter->second.pobj);
		if (pMarriageData) 
		{
			gGlobal.m_deleteGroup.Add(pMarriageData);
		}
		gGlobal.marriageIdMap.erase(iter);
	}

	return S_OK;
}

RPCResult ObjectService_Stub::GetMarriage(LPCPEER pPeer, RPCContext &context, const UInt32 &husband_id,const UInt32 &wife_id)
{
	PROCEDURE_SET_NONBLOCK
		PROCEDURE_START_RPC2(ObjectService, GetMarriage, NULL, UInt32, husband_id,UInt32, wife_id)

		TRACE_ENSURE(pdbMarriage != NULL);
	CMarriageData* pMarriageData = NULL;
	if(husband_id > 0)
		pMarriageData = CMarriageData::FindMarriageByHusbandId(husband_id);
	else
		pMarriageData = CMarriageData::FindMarriageByWifeId(wife_id);

	if (pMarriageData || (husband_id == 0&&wife_id == 0))
	{
		// directly send back object group
		NetGroup sendGrp;
		if(pMarriageData)
		{
			pMarriageData->PreSend();
			sendGrp.Add(pMarriageData);
		}
		CB_GetMarriage(pPeer, context, &sendGrp);
		PROCEDURE_RETURN
	}
	else
	{
		// wait for thread pool to process
		gGlobal.WorkQueueBegin();
		MarriageRequest *pRequest = NULL;
		if(husband_id > 0)
			pRequest = SafeCreateObject4(MarriageRequest, husband_id, pPeer, context,1);
		else
			pRequest = SafeCreateObject4(MarriageRequest, wife_id, pPeer, context,2);

#ifdef DB_THREAD_POOL
		gGlobal.QueueUserWorkItem(CMarriageData::QueueGetMarriage, pRequest, DB_WORKER_FLAG);
#else
		CMarriageData::QueueGetMarriage(pRequest);
#endif
	}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, netGroup)

		TRACE_ENSURE(netGroup);
	CMarriageData* pMarriageData = NULL;

	if(netGroup->GetCount() >= 1)
	{
		pMarriageData = netGroup->GetChild<CMarriageData>(0);

		if (pMarriageData)
		{
			// need to register for new marriage data
			DataObject &dobj = gGlobal.marriageIdMap[pMarriageData->GetMarriage_id()];
			if (dobj.pobj)
			{
				DeleteNetObject(pMarriageData);
				pMarriageData = CastNetObject<CMarriageData>(dobj.pobj);
			}
			else
			{
				LocalRPCManager::GetInstance().Register(pMarriageData);
				pMarriageData->ClearUpdateMask();
				dobj.pobj = pMarriageData;
			}
		}
	}

	NetGroup sendGrp;
	if (pMarriageData)
	{
		pMarriageData->PreSend();
		sendGrp.Add(pMarriageData);
	}
	// send marriage data to peer
	CB_GetMarriage(pPeer, context, &sendGrp);
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
void CMarriageData::ReleaseMarriage(UInt32 char_id)
{
	CCharacterData* pCharacterData = FindChar(char_id);
	if(pCharacterData == NULL)
	{
		return;
	}
	CMarriageData* pMarriageData = NULL;
	DataIdMap::iterator iter = gGlobal.marriageIdMap.begin();
	DataIdMap::iterator iterend = gGlobal.marriageIdMap.end();
	for(; iter != iterend; ++iter)
	{
		CMarriageData* pMarriageDataTmp = CastNetObject<CMarriageData>(iter->second.pobj);
		if(pCharacterData->GetSex() == 0 && pMarriageDataTmp->GetHusband_id() == char_id)
		{
			pMarriageData = pMarriageDataTmp;
			break;
		}
		else if(pCharacterData->GetSex() == 1 && pMarriageDataTmp->GetWife_id() == char_id)
		{
			pMarriageData = pMarriageDataTmp;
			break;
		}
	}

	if (pMarriageData)
	{
		Boolean bDelete = BOOLEAN_TRUE;
		UInt32 nTmpCharId = 0;
		do 
		{
			nTmpCharId = pMarriageData->GetHusband_id();
			if(nTmpCharId != 0 && nTmpCharId != char_id && FindChar(nTmpCharId) != NULL) 
			{
				bDelete = BOOLEAN_FALSE;
				break;
			}
			nTmpCharId = pMarriageData->GetWife_id();
			if(nTmpCharId != 0 && nTmpCharId != char_id && FindChar(nTmpCharId) != NULL) 
			{
				bDelete = BOOLEAN_FALSE;
				break;
			}
		}while(false);

		if(bDelete)
		{
			gGlobal.DequeueUpdate(&iter->second);
			gGlobal.marriageIdMap.erase(iter);
			if (pMarriageData->IsUpdated())				// write to db if updated
				gGlobal.m_releaseGroup.Add(pMarriageData);
			else
				DeleteNetObject(pMarriageData);
		}
	}
}
HRESULT CMarriageData::DoCreateMarriage(const UInt32 & husband_id, const UInt32 & wife_id,const time_t &marriage_time,const UInt32 & state,CMarriageData* &pMarriageData)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("DoCreateMarriage: husband_id:%d wife_id:%d"), husband_id,wife_id);
	pMarriageData = NULL;
	DBTIMESTAMP uTime;
	TimeConvert(marriage_time, uTime);

	HRESULT hr = pdbMarriage->Insert(husband_id,wife_id,uTime,state);
	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbMarriage->MoveFirst()))
	{
		pMarriageData = CMarriageData::NewMarriage();
	}
	pdbMarriage->Close();

	return hr;
}


RPCResult ObjectService_Stub::CreateMarriage(LPCPEER pPeer, RPCContext &context,
											const UInt32 & husband_id, const UInt32 & wife_id,const time_t &marriage_time,const UInt32 & state)
{
	TRACE_ENSURE(pdbMarriage != NULL);

	HRESULT hr = S_OK;
	CMarriageData* pMarriageData = NULL;
	NetGroup sendGrp;
	hr = CMarriageData::DoCreateMarriage(husband_id, wife_id,marriage_time, state,pMarriageData );
	if (DB_FAILED(hr)) 
	{
		pdbMarriage->ShowError(hr);
		CB_Error(pPeer, context, hr, _T("CreateMarriage DB_FAILED:"));
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CreateMarriage failed!"));
		CB_CreateMarriage(pPeer, context, &sendGrp);
		return RPC_RESULT_FAIL;
	}
	
	if (pMarriageData)
	{
		pMarriageData->PreSend();
		sendGrp.Add(pMarriageData);
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CreateMarriage success! marriage_id:%d"),pMarriageData->GetMarriage_id());
	}

	CB_CreateMarriage(pPeer, context, &sendGrp);

	return RPC_RESULT_OK; 

}

RPCResult ObjectService_Stub::UpdateMarriage(LPCPEER pPeer, RPCContext &context, NetObject* netObject)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("UpdateMarriage"));
	TRACE_ENSURE(pdbMarriage);
	CMarriageData *pNewMarriageData = CastNetObject<CMarriageData>(netObject);
	if(pNewMarriageData == NULL) return RPC_RESULT_OK;

	const UInt32 marriage_id = pNewMarriageData->GetMarriage_id();
	CMarriageData *pMarriageData = NULL;
	DataIdMap::iterator iter = gGlobal.marriageIdMap.find(marriage_id);

	if (iter != gGlobal.marriageIdMap.end())
	{
		// found existing marriage in cache
		pMarriageData = CastNetObject<CMarriageData>(iter->second.pobj);
		TRACE_ENSURE(pMarriageData);
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("marriage_id not found, marriage_id %d"), marriage_id);
		return RPC_RESULT_OK;
	}

	NetGroup sendGrp;
	if(pMarriageData)
	{
		pMarriageData->Clone(pNewMarriageData, pNewMarriageData->GetUpdateMask());
		sendGrp.Add(pMarriageData);
		gGlobal.EnqueueUpdate(&iter->second);
	}
 
	if(pNewMarriageData != pMarriageData)
		DeleteNetObject(pNewMarriageData);
	else
		TRACE_VERBOSEDTL(GLOBAL_LOGGER, _FW("ObjectService_Stub::UpdateMarriage  pNewMarriageData == pMarriageData"));

	CB_UpdateMarriage(pPeer, context, &sendGrp);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::DeleteMarriage(LPCPEER pPeer, RPCContext &context, const UInt32 &marriage_id)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("DeleteMarriage: marriage_id %d"), marriage_id);
	CMarriageData::RemoveMarriage(marriage_id);
	CB_DeleteMarriage(pPeer,context,1);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetMarriageCount(LPCPEER pPeer, RPCContext &context)
{
	TRACE_ENSURE(pdbMarriage);
	pdbMarriage->GetCount();
	if(!DB_FAILED(pdbMarriage->MoveFirst()))
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GetMarriageCount: count %d"), pdbMarriage->m_count);
		CB_GetMarriageCount(pPeer,context,pdbMarriage->m_count);
	}
	else
	{	
		TRACE_INFODTL(GLOBAL_LOGGER, _F("GetMarriageCount: FAILED"));
		CB_GetMarriageCount(pPeer,context,0);
	}
	pdbMarriage->Close();
	
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetSingleMarriageLst(LPCPEER pPeer, RPCContext &context)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC(ObjectService, GetSingleMarriageLst, NULL)
	TRACE_ENSURE(pdbMarriage != NULL);

	gGlobal.WorkQueueBegin();
	MarriageRequest *pRequest = SafeCreateObject4(MarriageRequest, 0, pPeer, context,3);
#ifdef DB_THREAD_POOL
	gGlobal.QueueUserWorkItem(CMarriageData::QueueGetMarriage, pRequest, DB_WORKER_FLAG);
#else
	CMarriageData::QueueGetMarriage(pRequest);
#endif

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, marriageGroup)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GetSingleMarriageLst %d"),marriageGroup->GetCount());

	CMarriageData* pMarriageData = NULL;
	NetGroup sendGrp;
	for (Index i = C_INDEX(0); i < marriageGroup->GetCount(); ++i)
	{
		pMarriageData = marriageGroup->GetChild<CMarriageData>(i);
		if (pMarriageData)
		{
			pMarriageData->PreSend();
			DataObject &dobj = gGlobal.marriageIdMap[pMarriageData->GetHusband_id()];
			if (dobj.pobj) 
			{
				DeleteNetObject(pMarriageData);
				pMarriageData = CastNetObject<CMarriageData>(dobj.pobj);
			} 
			else 
			{
				LocalRPCManager::GetInstance().Register(pMarriageData);
				pMarriageData->ClearUpdateMask();
				dobj.pobj = pMarriageData;
			}
			sendGrp.Add(pMarriageData);
		}
	}

	CB_GetSingleMarriageLst(pPeer, context, &sendGrp);
	DeleteNetGroup(marriageGroup);			

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
		_FE("sessionId %s, errcode %d, %s"),
		context.sessionId.GetString().c_str(), err, errMsg.c_str());
	CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;

}