//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"

#include "Stub/Data/TitleData.h"
#include "Stub/Service/ObjectService.h"
#include "ObjectService/DboSourcePool.h"
#include "ObjectService/dboTitle.h"
#include "ObjectService/Global.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResTitle.h"

REGISTER_CLASSTYPE(CTitleData, TitleData_CLASSID);

CdboTitle* &pdbTitle = gGlobal.gTables.pTitle;
NetGroup* FindTitleGroup(UInt32 char_id);
void PreSendGroup(NetGroup * titleGroup);
StringVector CdboTitleAccessor::m_params;
ThreadKey CTitleData::m_key;
typedef WorkRequest<UInt32> TitleRequest;

CTitleData* NewTitle()
{
	CTitleData *pTitleData = NULL;
	UInt32 owner_id = (UInt32) pdbTitle->m_owner_id.LowPart;
	UInt32 title_uid = (UInt32) pdbTitle->m_title_uid.LowPart;
	DataObject &dobj = gGlobal.titleIdMap[title_uid];

	if (dobj.pobj == NULL) {
		pTitleData = CTitleData::NewInstance();
		pTitleData->SetData(pdbTitle);			// assign values to character data
		dobj.pobj = pTitleData;
	} else {
		pTitleData = CastNetObject<CTitleData>(dobj.pobj);
	}

	// found existing characters in cache
	NetGroup* titleGroup = FindTitleGroup(owner_id);
	if (titleGroup)
	{
		titleGroup->Add(pTitleData);
	}
	else
	{
		DataObject& dobj3 = gGlobal.titleGroupMap[owner_id];
		if (dobj3.pobj) {
			titleGroup = SafeCreateObject(NetGroup);
			dobj3.pobj = titleGroup;
		} else {
			titleGroup = (NetGroup*) dobj3.pobj;
		}
		titleGroup->Add(pTitleData);
	}

	return pTitleData;
}

void ShowTitleError(HRESULT hr)
{
	/*String errMsg;
	pdbTitle->GetError(errMsg);
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Title DB Error: code %d: %s"), hr, errMsg.c_str());
	if (hr == DB_ERR_UNEXPECTED) {			// try to reopen OLEDB when unexpected error
		if (FAILED(hr = pdbTitle->ReopenDataSource()))
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("OLE Error: failed to reopen Title OLE DB"));
		}
	}*/
}


NetGroup* FindTitleGroup(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.titleGroupMap.find(char_id);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.titleGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}


CTitleData* FindTitle(UInt32 title_uid)
{
	DataIdMap::iterator iter = gGlobal.titleIdMap.find(title_uid);
	CTitleData* pTitleData = NULL;

	// found existing account in cache
	if (iter != gGlobal.titleIdMap.end())
		pTitleData = CastNetObject<CTitleData>(iter->second.pobj);

	return pTitleData;
}

HRESULT RemoveTitle(UInt32 title_uid)
{
	DataIdMap::iterator iter = gGlobal.titleIdMap.find(title_uid);
	if (iter != gGlobal.titleIdMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CTitleData* pTitleData = CastNetObject<CTitleData>(iter->second.pobj);
		if (pTitleData) {
			NetGroup* titleGroup = FindTitleGroup(pTitleData->GetOwner_id());
			if (titleGroup)
				titleGroup->Remove(pTitleData);
			gGlobal.m_deleteGroup.Add(pTitleData);
		}
		gGlobal.titleIdMap.erase(iter);
	}

	return S_OK;
}

void ReleaseTitles(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.titleGroupMap.find(char_id);

	if (iter != gGlobal.titleGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		CTitleData *pTitleData;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++) {
			pTitleData = grp->GetChild<CTitleData>(i);
			if (pTitleData) {
				DataIdMap::iterator it = gGlobal.titleIdMap.find(
					pTitleData->GetTitle_uid());
				if (it != gGlobal.titleIdMap.end()) {		// remove from update queue
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.titleIdMap.erase(it);
				}
				if (pTitleData->IsUpdated())				// write to db if updated
					gGlobal.m_releaseGroup.Add(pTitleData);
				else
					DeleteNetObject(pTitleData);
			}
		}
		gGlobal.titleGroupMap.erase(iter);
		SafeDeleteObject(grp);
	}
}

void PreSendTitleGrp(NetGroup * grp)
{
	if (grp == NULL) return;

	for (UInt16 i = 0; i < grp->GetCount(); ++i)
	{
		CTitleData *pTitleData = grp->GetChild<CTitleData>(i);
		if (pTitleData)
			pTitleData->PreSend();
		else
			grp->Replace(i, NULL);
	}
}

DWORD WINAPI QueueGetTitles(LPVOID lpParam)
{
	HRESULT hr = S_OK;

	TitleRequest* pRequest = (TitleRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
	{
		NetGroup titleGroup;
		CTitleData* pTitleData;
		CdboTitle* dboTitle = SafeCreateObject(CdboTitle);

		titleGroup.SetValidate(BOOLEAN_FALSE);
		dboTitle->SetDataSource(dbSrc);
		if (DB_SUCCEEDED(hr = dboTitle->RSByOwnerID(pRequest->key)))
		{
			hr = dboTitle->MoveFirst();
			while (hr == S_OK)
			{
				pTitleData = SafeCreateObject(CTitleData);
				pTitleData->SetData(dboTitle);
				pTitleData->PreSend();
				titleGroup.Add(pTitleData);
				hr = dboTitle->MoveNext();
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetTitles"), hr, dboTitle->GetErrorMsg(hr), 
			pRequest->context, &titleGroup);
		dboTitle->Close();
		SafeDeleteObject(dboTitle);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, owner_id %d"),
			pRequest->context.sessionId.GetString().c_str(), pRequest->key);
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

void CTitleData::SetData(CdboTitle* dboTitle)
{
	SetKey((UInt32)dboTitle->m_title_uid.LowPart);
	SetInfo((UInt16)dboTitle->m_title_id);
	SetOwner((UInt32)dboTitle->m_owner_id.LowPart);
	
	time_t uTime;
	TimeConvert(dboTitle->m_expireDate, uTime);
	SetExpireDate((UInt32)uTime);

	ClearUpdateMask();
}

HRESULT CTitleData::UpdateDbo(CdboTitle* dboTitle)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("title_uid %d, owner_id %d update_mask %d,%d"), 
		GetTitle_uid(), this->GetOwner_id(), m_updateMask.low, m_updateMask.high);

	dboTitle->ClearParam();

	if (IsUpdated(infoMask))
	{
		COLUMN_UPDATE(dboTitle, title_id, GetTitle_id());
	}

	if (IsUpdated(ownerMask))
	{
		COLUMN_UPDATE_LARGE(dboTitle, owner_id, GetOwner_id());
	}
	
	if (IsUpdated(expireDateMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetExpireDate(), dbtime);
		COLUMN_UPDATE(dboTitle, expireDate, dbtime);
	}
	

	HRESULT hr;
	if (DB_FAILED(hr = dboTitle->UpdateByAny(GetTitle_uid())))
		dboTitle->ShowError(hr);
	else
		ClearUpdateMask();
	dboTitle->ClearParam();
	dboTitle->Close();

	return hr;
}

HRESULT CTitleData::DeleteDbo(CdboTitle* dboTitle)
{
	HRESULT hr;

	if (DB_FAILED(hr = dboTitle->Delete(GetTitle_uid())))
		dboTitle->ShowError(hr);
	dboTitle->Close();

	return hr;
}

void CTitleData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

HRESULT DoCreateTitle(UInt16 title_id, UInt32 owner_id, CTitleData* &pTitleData)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("title_id %d, owner_id %d"), title_id,
		owner_id);

	HRESULT hr = S_OK;
	// get mob data from resource
	const TitleData* data = gGlobal.m_resource->GetTitleData(title_id);
	if (data == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("DB_FAILED, title_id %d not found in resource"), title_id);
		return S_FALSE;
	}

	pTitleData = NULL;
	
	DBTIMESTAMP dbtime;
	if (data->Title_TimeLimitType == TITLE_TIME_LIMIT_TYPE_ADDDAY)
	{				
		time_t expireTime = time(NULL);
		expireTime += data->Title_TimeLimit * 24 *3600;
		TimeConvert((time_t) expireTime, dbtime);
	}
	else if (data->Title_TimeLimitType == TITLE_TIME_LIMIT_TYPE_EXACTDAY)
	{
		dbtime.year		= data->Title_ExpireYear;
		dbtime.month	= data->Title_ExpireMonth;
		dbtime.day		= data->Title_ExpireDay;
		dbtime.hour		= data->Title_ExpireHour;
		dbtime.minute	= data->Title_ExpireMin;
		dbtime.second	= data->Title_ExpireSec;
		dbtime.fraction	= 0;						// 0 - 999,999,999
	}
	else
	{
		dbtime.year		= 1970;
		dbtime.month	= 1;
		dbtime.day		= 1;
		dbtime.hour		= 0;
		dbtime.minute	= 0;
		dbtime.second	= 0;
		dbtime.fraction	= 0;						// 0 - 999,999,999
	}
	pdbTitle->m_expireDate = dbtime;

	hr = pdbTitle->Insert(owner_id, title_id);

	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbTitle->MoveFirst()))
	{
		pTitleData = NewTitle();
	}

	pdbTitle->Close();

	return hr;
}

RPCResult ObjectService_Stub::CreateTitle(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16 &title_id)
{
	TRACE_ENSURE(pdbTitle != NULL);
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("title_id = %d, owner_id = %d"), owner_id, title_id);

	HRESULT hr = S_OK;
	CTitleData* pTitleData = NULL;

	hr = DoCreateTitle(title_id, owner_id, pTitleData);
	if (DB_FAILED(hr))
	{
		pdbTitle->ShowError(hr);
		CB_Error(pPeer, context, hr, _FE("DB_FAILED"));
		return RPC_RESULT_FAIL;
	}

	if (pTitleData)
		pTitleData->PreSend();

	CB_CreateTitle(pPeer, context, pTitleData);

	return RPC_RESULT_OK;
}
RPCResult ObjectService_Stub::DeleteTitle(LPCPEER pPeer, RPCContext &context, const UInt32 &title_uid)
{
	TRACE_ENSURE(pdbTitle != NULL);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("title_uid %d"), title_uid);
	RemoveTitle(title_uid);
	CB_DeleteTitle(pPeer, context);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetTitleList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC1(ObjectService, GetTitleList, NULL, UInt32, owner_id)

		TRACE_ENSURE(pdbTitle != NULL);
		CTitleData* pTitleData = NULL;
		NetGroup* titleGroup = FindTitleGroup(owner_id);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("owner_id %d"), owner_id);

		if (titleGroup)
		{
			// directly send back object group
			PreSendTitleGrp(titleGroup);
			CB_TitleList(pPeer, context, titleGroup);
			PROCEDURE_RETURN
		}
		else
		{
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			TitleRequest *pRequest = SafeCreateObject3(TitleRequest, owner_id, pPeer, context);
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetTitles, pRequest, DB_WORKER_FLAG);
#else
			QueueGetTitles(pRequest);
#endif
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, titleGroup)

		TRACE_ENSURE(titleGroup);

		// store new pet group
		CTitleData* pTitleData;
		DataObject& dobj3 = gGlobal.titleGroupMap[owner_id];
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
		for (UInt16 i = 0; i < titleGroup->GetCount(); i++)
		{
			pTitleData = titleGroup->GetChild<CTitleData>(i);
			if (pTitleData)
			{
				// need to register for new pet data
				DataObject &dobj = gGlobal.titleIdMap[pTitleData->GetTitle_uid()];
				if (dobj.pobj)
				{
					DeleteNetObject(pTitleData);
					pTitleData = CastNetObject<CTitleData>(dobj.pobj);
				}
				else
				{
					LocalRPCManager::GetInstance().Register(pTitleData);
					pTitleData->ClearUpdateMask();
					dobj.pobj = pTitleData;
				}
				newGroup->Add(pTitleData);
			}
		}

		// send pet data to peer
		PreSendTitleGrp(newGroup);
		CB_TitleList(pPeer, context, newGroup);
		DeleteNetGroup(titleGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateTitle(LPCPEER pPeer, RPCContext &context, const UInt32 &title_uid, NetObject* titleData)
{
	CTitleData *pNewData = CastNetObject<CTitleData>(titleData);
	TRACE_ENSURE(pdbTitle && pNewData);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("title_uid %d"), title_uid);

	CTitleData* pTitleData = FindTitle(title_uid);

	if (pTitleData == NULL)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("title_uid %d not found"),
			title_uid);
	}
	else
	{
		DataIdMap::iterator iter = gGlobal.titleIdMap.find(title_uid);

		pTitleData->Clone(pNewData, pNewData->GetUpdateMask());
		if (iter != gGlobal.titleIdMap.end())
			gGlobal.EnqueueUpdate(&iter->second);
	}
	DeleteNetObject(pNewData);

	CB_UpdateTitle(pPeer, context);

	return RPC_RESULT_OK;
}
