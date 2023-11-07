//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/HawkerHistoryData.h"

//-- Library
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboHawkerHistory.h"
#include "ObjectService/Global.h"
#include "ObjectService/DboSourcePool.h"

REGISTER_CLASSTYPE(CHawkerHistoryData, HawkerHistoryData_CLASSID);

CdboHawkerHistory* &pdbHawkerHistory = gGlobal.gTables.pHawkerHistory;
StringVector CdboHawkerHistoryAccessor::m_params;
ThreadKey CHawkerHistoryData::m_key;
typedef WorkRequest<UInt32> HawkerHistoryRequest;

NetGroup* FindHawkerHistoryGroup(UInt32 char_id);

CHawkerHistoryData* NewHistory()
{
	CHawkerHistoryData* pHawkerHistoryData = NULL;
	UInt32 historyId = (UInt32) pdbHawkerHistory->m_hawkerHistory_id.LowPart;
	UInt32 owner_id = (UInt32) pdbHawkerHistory->m_char_id.LowPart;
	DataObject &dobj = gGlobal.hawkerHistoryIdMap[historyId];

	if (dobj.pobj == NULL) {
		pHawkerHistoryData = CHawkerHistoryData::NewInstance();
		pHawkerHistoryData->SetData(pdbHawkerHistory);			// assign values to character data
		dobj.pobj = pHawkerHistoryData;
	} else {
		pHawkerHistoryData = CastNetObject<CHawkerHistoryData>(dobj.pobj);
	}

	// found existing item group in cache
	NetGroup* historyGrp = FindHawkerHistoryGroup(owner_id);
	if (historyGrp)
	{
		historyGrp->Add(pHawkerHistoryData);
	}
	else
	{
		DataObject& dobj3 = gGlobal.hawkerHistoryGroupMap[owner_id];
		if (dobj3.pobj == NULL) {
			historyGrp = SafeCreateObject(NetGroup);
			dobj3.pobj = historyGrp;
		} else {
			historyGrp = (NetGroup*) dobj3.pobj;
		}
		historyGrp->Add(pHawkerHistoryData);
	}

	return pHawkerHistoryData;
}

CHawkerHistoryData* FindHawkerHistory(UInt32 historyID)
{
	DataIdMap::iterator iter = gGlobal.hawkerHistoryIdMap.find(historyID);
	CHawkerHistoryData* pHawkerHistoryData = NULL;

	// found existing account in cache
	if (iter != gGlobal.hawkerHistoryIdMap.end())
		pHawkerHistoryData = CastNetObject<CHawkerHistoryData>(iter->second.pobj);

	return pHawkerHistoryData;
}

NetGroup* FindHawkerHistoryGroup(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.hawkerHistoryGroupMap.find(char_id);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.hawkerHistoryGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}

void PreSendHawkerHistoryGrp(NetGroup * grp)
{
	if (grp == NULL) return;

	for (UInt16 i = 0; i < grp->GetCount(); ++i)
	{
		CHawkerHistoryData *pHawkerHistoryData = grp->GetChild<CHawkerHistoryData>(i);
		if (pHawkerHistoryData)
			pHawkerHistoryData->PreSend();
		else
			grp->Replace(i, NULL);
	}
}

HRESULT DoCreateHawkerHistory(UInt32 char_id, String itemName, UInt8 itemType, UInt8 stack, 
							  UInt32 totalPrice, UInt32 tax, CHawkerHistoryData* &pHawkerHistoryData)
{
	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("CreateHawkerHistory: char_id %d, itemName %s, itemType %d, itemStack %d, totalPrice %d, tax %d"),
		char_id, itemName.c_str(), itemType, stack, totalPrice, tax);

	HRESULT hr = S_OK;

	hr = pdbHawkerHistory->Insert(char_id, itemName, itemType, stack, totalPrice, tax);

	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbHawkerHistory->MoveFirst()))
	{
		pHawkerHistoryData = NewHistory();	
	}
	pdbHawkerHistory->Close();

	return hr;
}

HRESULT RemoveHawkerHistory(UInt32 historyID)
{
	DataIdMap::iterator iter = gGlobal.hawkerHistoryIdMap.find(historyID);
	if (iter != gGlobal.hawkerHistoryIdMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CHawkerHistoryData* pHawkerHistoryData = CastNetObject<CHawkerHistoryData>(iter->second.pobj);
		if (pHawkerHistoryData) {
			NetGroup *grp = FindHawkerHistoryGroup(pHawkerHistoryData->GetChar_id());
			if (grp) 
				grp->Remove(pHawkerHistoryData);
			gGlobal.m_deleteGroup.Add(pHawkerHistoryData);
		}
		gGlobal.hawkerHistoryIdMap.erase(iter);
	}

	return S_OK;
}

void ReleaseHawkerHistory(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.hawkerHistoryGroupMap.find(char_id);

	if (iter != gGlobal.hawkerHistoryGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		CHawkerHistoryData* pHawkerHistoryData = NULL;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++) {
			pHawkerHistoryData = grp->GetChild<CHawkerHistoryData>(i);
			if (pHawkerHistoryData) 
			{
				DataIdMap::iterator it = gGlobal.hawkerHistoryIdMap.find(pHawkerHistoryData->GetHistoryId());
				if (it != gGlobal.hawkerHistoryIdMap.end()) {		// remove from update queue
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.hawkerHistoryIdMap.erase(it);
					DeleteNetObject(pHawkerHistoryData);
				}
			}
		}
		gGlobal.hawkerHistoryGroupMap.erase(iter);
		SafeDeleteObject(grp);
	}
}

HRESULT CHawkerHistoryData::UpdateDbo(CdboHawkerHistory* dboHawkHist)
{
	return S_OK;
}

HRESULT CHawkerHistoryData::DeleteDbo(CdboHawkerHistory* dboHawkHist)
{
	HRESULT hr;
	//ThreadGuard guard(m_key);

	if (DB_FAILED(hr = dboHawkHist->Delete(GetHistoryId())))
		dboHawkHist->ShowError(hr);
	dboHawkHist->Close();

	return hr;
}

DWORD WINAPI QueueGetHawkerHistorys(LPVOID lpParam)
{
	HRESULT hr = S_OK;

	HawkerHistoryRequest* pRequest = (HawkerHistoryRequest*) lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource(); 
	if (dbSrc)
	{
		NetGroup historyGroup;
		CHawkerHistoryData* pHawkerHistoryData = NULL;
		CdboHawkerHistory* dboHawkerHistory = SafeCreateObject(CdboHawkerHistory);

		historyGroup.SetValidate(BOOLEAN_FALSE);
		dboHawkerHistory->SetDataSource(dbSrc);
		
		hr = dboHawkerHistory->RSByCharID(pRequest->key);
		
		if (DB_SUCCEEDED(hr))
		{
			hr = dboHawkerHistory->MoveFirst();
			while (hr == S_OK)
			{
				pHawkerHistoryData = SafeCreateObject(CHawkerHistoryData);
				pHawkerHistoryData->SetData(dboHawkerHistory);
				pHawkerHistoryData->SetSendMask(CHawkerHistoryData::AllMask);
				historyGroup.Add(pHawkerHistoryData);
				hr = dboHawkerHistory->MoveNext();
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetHawkerHistorys"), hr, 
			dboHawkerHistory->GetErrorMsg(hr), pRequest->context, &historyGroup);
		dboHawkerHistory->Close();
		SafeDeleteObject(dboHawkerHistory);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, key %d"),
			pRequest->context.sessionId.GetString().c_str(), pRequest->key);
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

void CHawkerHistoryData::SetData(CdboHawkerHistory* dboHawkerHistory)
{
	SetHistoryId((UInt32)dboHawkerHistory->m_hawkerHistory_id.LowPart);
	SetHistoryInfo((UInt32)dboHawkerHistory->m_char_id.LowPart, dboHawkerHistory->m_itemName, 
		(UInt8)dboHawkerHistory->m_itemType, (UInt8)dboHawkerHistory->m_stack, 
		(UInt32)dboHawkerHistory->m_totalPrice.LowPart, (UInt32)dboHawkerHistory->m_tax.LowPart);
	time_t uTime;
	TimeConvert(dboHawkerHistory->m_hawkerDate, uTime);
	SetHistoryDate((UInt32)uTime);
	ClearUpdateMask();
}

void CHawkerHistoryData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

RPCResult ObjectService_Stub::GetHawkerHistory(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC1(ObjectService, GetHawkerHistory, NULL, UInt32, char_id)

		TRACE_ENSURE(pdbHawkerHistory != NULL);

		NetGroup* historyGrp = FindHawkerHistoryGroup(char_id);
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GetHawkerHistory: char_id %d"), char_id);

		if (historyGrp)
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("GetHawkerHistory: hawker history group found char_id %d, count = %d"), char_id, historyGrp->GetCount());
			PreSendHawkerHistoryGrp(historyGrp);
			CB_GetHawkerHistory(pPeer, context, historyGrp);
			PROCEDURE_RETURN
		}
		else
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GetHawkerHistory: need retrive partner group from db char_id %d"), char_id);
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			HawkerHistoryRequest *pRequest = SafeCreateObject3(HawkerHistoryRequest, char_id, pPeer, context);
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetHawkerHistorys, pRequest, DB_WORKER_FLAG);
#else
			QueueGetHawkerHistorys(pRequest);
#endif
		}

		PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, historyGrp)

		TRACE_ENSURE(historyGrp);
		TRACE_ENSURE(historyGrp->GetClassID() == NetGroup_CLASSID);

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("GetHawkerHistory: retrived history group from db char_id %d, count %d"), char_id, historyGrp->GetCount());

		// store new partner group
		CHawkerHistoryData* pHistoryData;
		NetGroup* newGroup = NULL;
		DataObject& dobj3 = gGlobal.hawkerHistoryGroupMap[char_id];
		if (dobj3.pobj) {
			newGroup = (NetGroup*) dobj3.pobj;
		} else {
			newGroup = SafeCreateObject(NetGroup);
			dobj3.pobj = newGroup;
		}
		for (Index i = C_INDEX(0); i < historyGrp->GetCount(); ++i)
		{
			pHistoryData = historyGrp->GetChild<CHawkerHistoryData>(i);
			if (pHistoryData)
			{
				// need to register for new partner data
				DataObject &dobj = gGlobal.hawkerHistoryIdMap[pHistoryData->GetHistoryId()];
				if (dobj.pobj) {
					DeleteNetObject(pHistoryData);
					pHistoryData = CastNetObject<CHawkerHistoryData>(dobj.pobj);
				} else {
					LocalRPCManager::GetInstance().Register(pHistoryData);
					pHistoryData->ClearUpdateMask();
					dobj.pobj = pHistoryData;
				}
				newGroup->Add(pHistoryData);
			}
		}
		PreSendHawkerHistoryGrp(newGroup);
		CB_GetHawkerHistory(pPeer, context, newGroup);
		DeleteNetGroup(historyGrp);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_T("GetHawkerHistory Error: sessionId %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH
		
	return RPC_RESULT_OK;

}

RPCResult ObjectService_Stub::UpdateHawkerHistory(LPCPEER pPeer, RPCContext &context, 
												  const UInt32 &char_id, NetGroup* grp)
{
	TRACE_ENSURE(pdbHawkerHistory && grp);

	NetGroup newHistorys;
	HRESULT hr = S_OK;
	for(Index i = C_INDEX(0); i < grp->GetCount(); ++i)
	{
		CHawkerHistoryData* pHistoryData = grp->GetChild<CHawkerHistoryData>(i);
		if (pHistoryData)
		{
			if (pHistoryData->GetChar_id() == 0)
			{
				UInt32 historyID = pHistoryData->GetHistoryId();
				hr = RemoveHawkerHistory(historyID);
			}
			else if (pHistoryData->GetHistoryId() == 0)
			{

				hr = DoCreateHawkerHistory(pHistoryData->GetChar_id(), pHistoryData->GetItemName(), 
					pHistoryData->GetItemType(), pHistoryData->GetStack(), pHistoryData->GetTotalPrice(), pHistoryData->GetTax(), pHistoryData );
				if (DB_FAILED(hr))
				{
					TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("UpdateHawkerHistory DB_FAILED: history create failed, hr=%X"), hr);
					pdbHawkerHistory->ShowError(hr);
					CB_Error(pPeer, context, hr, _T("UpdateHawkerHistory DB_FAILED: history create failed"));
					break;
				}
				pHistoryData->SetSendMask(CHawkerHistoryData::AllMask);
				newHistorys.Add(pHistoryData);
			}
		}
	}

	DeleteNetGroup(grp, BOOLEAN_TRUE);
	pdbHawkerHistory->Close();

	if (DB_FAILED(hr))
	{
		pdbHawkerHistory->ShowError(hr);
		DeleteNetChilds(&newHistorys);
		return RPC_RESULT_FAIL;
	}

	CB_UpdateHawkerHistory(pPeer, context, &newHistorys);
	return RPC_RESULT_OK;
}
