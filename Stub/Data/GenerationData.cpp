#include "Common.h"
#include "Common/Procedure.h"
#include "GenerationData.h"

#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboGeneration.h"
#include "ObjectService/Global.h"
#include "Common/common_binary.h"
#include "ObjectService/DboSourcePool.h"
#include "Stub/Data/MsgCharacterData.h"
#include "ObjectService/dboMsgCharacter.h"
#include "Stub/Data/CharacterData.h"

REGISTER_CLASSTYPE(CGenerationData, GenerationData_CLASSID);

void CGenerationData::SetData(CdboGeneration* dboGeneration, BOOL bAsYoung,BOOL bCreate)
{
	SetUiYoungerId((UInt32) dboGeneration->m_i64YoungerId.LowPart);
	SetUiElderId((UInt32) dboGeneration->m_i64ElderId.LowPart);
	if (!bAsYoung)
	{
		SetUiGiftPointOrHistAccm((UInt32)dboGeneration->m_iGiftPointOrHisAcc);
		SetStrNickName(dboGeneration->m_strYoungerName);
		SetUiAccmulOrRecvExp(dboGeneration->m_iAccOrRecvExp);
	}
	else
	{
		SetStrNickName(dboGeneration->m_strElderName);
		if (bCreate)
			SetUiAccmulOrRecvExp(dboGeneration->m_iRecvExpForCrt);
		else	
			SetUiAccmulOrRecvExp(dboGeneration->m_iAccOrRecvExp);
	}
	SetBYoung(bAsYoung);
}

void CGenerationData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

HRESULT CGenerationData::UpdateDbo(CdboGeneration* dboGeneration)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_keyGeneration);
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("YoungerId %d, ElderId %d,AsYong %s, update_mask %d,%d"), 
		GetUiYoungerId(),
		GetUiElderId(),
		GetBYoung() ? _F("Yes") : _F("NO"),
		m_updateMask.low,
		m_updateMask.high);

	dboGeneration->ClearParam();
	if (IsUpdated(infoMask)) 
	{
		COLUMN_UPDATE(dboGeneration, iAccOrRecvExp,GetUiAccmulOrRecvExp());
		COLUMN_UPDATE(dboGeneration,iGiftPointOrHisAcc,GetUiGiftPointOrHistAccm());
	}

	HRESULT hr;
	if (DB_FAILED(hr = dboGeneration->UpdateGeneration(GetUiYoungerId(),
														GetUiElderId(),
														GetUiAccmulOrRecvExp(),
														GetUiGiftPointOrHistAccm(),
														GetBYoung())))
		dboGeneration->ShowError(hr);
	else
		ClearUpdateMask();
	dboGeneration->ClearParam();
	dboGeneration->Close();

	return hr;
}

HRESULT CGenerationData::DeleteDbo(CdboGeneration* dboGeneration)
{
	HRESULT hr;

	if (DB_FAILED(hr = dboGeneration->Delete(GetUiYoungerId(), GetUiElderId())))
		dboGeneration->ShowError(hr);
	dboGeneration->Close();

	return hr;
}


CdboGeneration* &pdbGeneration = gGlobal.gTables.pGeneration;
StringVector CdboGenerationAccessor::m_params;
ThreadKey CGenerationData::m_keyGeneration;

NetGroup* FindGenerationGroup(GenerationGroupKey &key);
void PreSendGroup(NetGroup * pGenerationGroup);

struct GenerationRequest : public WorkRequest<GenerationGroupKey>
{
	GenerationRequest(GenerationGroupKey ggk, LPCPEER pPeer, RPCContext &context)
		: WorkRequest<GenerationGroupKey>(ggk, pPeer, context)
	{}
};

////////////////////////////////////////////////////////////////////////
////						Generation Data							////
////////////////////////////////////////////////////////////////////////
void ReleaseGeneration(UInt32 uiCharId,BOOL bYoung)
{
	GenerationGroupKey ggk(uiCharId,bYoung);
	GenerationGroupMap::iterator iter = gGlobal.generationGroupMap.find(ggk);
	if (iter != gGlobal.generationGroupMap.end()){
		NetGroup* pGntGrp = (NetGroup*)iter->second.pobj;
		iter->second.pobj = NULL;
		gGlobal.generationGroupMap.erase(iter);
		if (NULL != pGntGrp) 
		{
			for (int i=0;i<pGntGrp->GetCount();i++) 
			{
				CGenerationData* pGnt = pGntGrp->GetChild<CGenerationData>(i);
				if (NULL != pGnt) 
				{
					if (bYoung) 
					{
						GenerationDataKey gdk(uiCharId,pGnt->GetUiElderId(),bYoung);
						GenerationDataMap::iterator iterGd = gGlobal.generationDataMap.find(gdk);
						if (iterGd != gGlobal.generationDataMap.end())
						{
							gGlobal.generationDataMap.erase(iterGd);
						}
					}
					else
					{
						GenerationDataKey gdk(pGnt->GetUiYoungerId(),uiCharId,bYoung);
						GenerationDataMap::iterator iterGd = gGlobal.generationDataMap.find(gdk);
						if (iterGd != gGlobal.generationDataMap.end())
						{
							gGlobal.generationDataMap.erase(iterGd);
						}
					}
					DeleteNetObject(pGnt);
				}
			}
			SafeDeleteObject(pGntGrp);
		}
	}
}
CGenerationData* FindGeneration(GenerationDataKey &key)
{
	GenerationDataMap::iterator iter = gGlobal.generationDataMap.find(key);
	// found existing account in cache
	if (iter == gGlobal.generationDataMap.end())
		return NULL;

	return CastNetObject<CGenerationData>(iter->second.pobj);
	
}

NetGroup* FindGenerationGroup(GenerationGroupKey &key)
{
	GenerationGroupMap::iterator iter = gGlobal.generationGroupMap.find(key);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.generationGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}

DWORD WINAPI QueueGetGenerations(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();

	GenerationRequest* pRequest = (GenerationRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
	{
		GenerationGroupKey ggk = (GenerationGroupKey) pRequest->key;
		NetGroup ntGenerationGroup;
		CGenerationData* pGenerationData;
		CdboGeneration* dboGeneration = SafeCreateObject(CdboGeneration);

		ntGenerationGroup.SetValidate(BOOLEAN_FALSE);
		dboGeneration->SetDataSource(dbSrc);
		
		UInt32 uYoungId,uElderId;
		if (ggk.bYoung) 
		{
			uYoungId = ggk.uiOwnerId;
			uElderId = 0;
		}
		else
		{
			uYoungId = 0;
			uElderId = ggk.uiOwnerId;
		}
		if (DB_SUCCEEDED(hr = dboGeneration->RSByID(uYoungId,uElderId)))
		{
			hr = dboGeneration->MoveFirst();
			while (hr == S_OK)
			{
				pGenerationData = SafeCreateObject(CGenerationData);
				pGenerationData->SetData(dboGeneration,ggk.bYoung,FALSE);
				pGenerationData->PreSend();
				ntGenerationGroup.Add(pGenerationData);
				hr = dboGeneration->MoveNext();
			}
		}
		
		dboGeneration->Close();

		gGlobal.CB_QueueNetGroup(_T("QueueGetGeneration"), hr, dboGeneration->GetErrorMsg(hr), 
			pRequest->context, &ntGenerationGroup);
		SafeDeleteObject(dboGeneration);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("cannot get data source, sessionId %s"),
			pRequest->context.sessionId.GetString().c_str());
	}
	// show query time
	UInt32 dt = ::GetTickCount() - curTime;
	if (dt > 1000)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("profile, utime %d"), dt);
	}

	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

RPCResult  ObjectService_Stub::GetGeneration(LPCPEER pPeer, 
											 RPCContext &context,
											 const UInt32 &uiCharId,
											 const BOOL &bYoung)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC2(ObjectService, GetGeneration, NULL, UInt32, uiCharId,BOOL,bYoung)
		
		GenerationGroupKey ggk(uiCharId,bYoung);
		NetGroup sndGrp;
		NetGroup* pGroup = FindGenerationGroup(ggk);
		if (NULL != pGroup)
		{
			for (UInt16 i=0;i<pGroup->GetCount();i++)
			{
				CGenerationData* pGntCache = pGroup->GetChild<CGenerationData>(i);
				if (NULL != pGntCache) 
				{
					pGntCache->PreSend();
					sndGrp.Add(pGntCache);
				}
			}
			ObjectService_Stub::CB_GetGeneration(pPeer,context,&sndGrp);
			PROCEDURE_RETURN
		}
		else
		{
			gGlobal.WorkQueueBegin();
			GenerationGroupKey ggkTmp(uiCharId,bYoung);
			GenerationRequest *pRequest = SafeCreateObject3(GenerationRequest, ggkTmp, pPeer, context);
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetGenerations, pRequest, DB_WORKER_FLAG);
#else
			QueueGetGenerations(pRequest);
#endif
		}
	
	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, netGroup)

		TRACE_ENSURE(netGroup);
		
		NetGroup* pGenerationGrp=NULL;
		GenerationGroupKey ggk(uiCharId,bYoung);
		DataObject& dobj3 = gGlobal.generationGroupMap[ggk];
		if (dobj3.pobj == NULL) 
		{
			pGenerationGrp = SafeCreateObject(NetGroup);
			dobj3.pobj = pGenerationGrp;
		} else
		{
			pGenerationGrp = (NetGroup*)dobj3.pobj;
		}

		NetGroup sndGrp;
		for (UInt16 i=0;i<netGroup->GetCount();i++) 
		{
			CGenerationData* pGnt = netGroup->GetChild<CGenerationData>(i);
			if (NULL != pGnt) 
			{
				LocalRPCManager::GetInstance().Register(pGnt);
				GenerationDataKey gdk(pGnt->GetUiYoungerId(),
					pGnt->GetUiElderId(),
					pGnt->GetBYoung());
				DataObject& dobj=gGlobal.generationDataMap[gdk];
				//if (NULL != dobj.pobj)
				//	DeleteNetObject(dobj.pobj);
				if(NULL != dobj.pobj)
				{
					DeleteNetObject(pGnt);
					pGnt = CastNetObject<CGenerationData>(dobj.pobj);
				}
				dobj.pobj = pGnt;
				if (!pGenerationGrp->IsChild(pGnt)) 
				{
					pGenerationGrp->Add(pGnt);
				}
				sndGrp.Add(pGnt);
			}
		}

		ObjectService_Stub::CB_GetGeneration(pPeer,context,&sndGrp);
	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			ObjectService_Stub::CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult  ObjectService_Stub::CreateGeneration(LPCPEER pPeer,RPCContext &context, 
												NetObject* pNewGnt, const String &strSelfName)
{
	CGenerationData* pNewGeneration = CastNetObject<CGenerationData>(pNewGnt);
	if (NULL == pNewGnt)
		return RPC_RESULT_FAIL;
	CdboGeneration* dboGeneration = gGlobal.gTables.pGeneration;
	if (NULL != dboGeneration) 
	{
		HRESULT hr;
		if (pNewGeneration->GetBYoung()) 
		{
			hr = dboGeneration->Insert(pNewGeneration->GetUiYoungerId(),
				pNewGeneration->GetUiElderId(),
				pNewGeneration->GetBYoung(),
				strSelfName.c_str(),pNewGeneration->GetStrNickName().c_str());
		}
		else
		{
			hr = dboGeneration->Insert(pNewGeneration->GetUiYoungerId(),
				pNewGeneration->GetUiElderId(),
				pNewGeneration->GetBYoung(),
				pNewGeneration->GetStrNickName().c_str(),strSelfName.c_str());
		}

		if (DB_SUCCEEDED(hr))
		{
			if (DB_SUCCEEDED(hr = dboGeneration->MoveFirst()))
			{
				CGenerationData* pGenerationData =  CreateNetObject<CGenerationData>();
				pGenerationData->SetData(dboGeneration,pNewGeneration->GetBYoung(),TRUE);
				pGenerationData->PreSend();
				
				NetGroup* pGenerationGrpYounger=NULL,*pGenerationGrpElder=NULL;
				GenerationGroupKey ggk(pGenerationData->GetUiYoungerId(),TRUE);
				DataObject& dobj3 = gGlobal.generationGroupMap[ggk];
				if (dobj3.pobj == NULL) 
				{
					pGenerationGrpYounger = SafeCreateObject(NetGroup);
					dobj3.pobj = pGenerationGrpYounger;
				} else 
				{
					pGenerationGrpYounger = (NetGroup*)dobj3.pobj;
				}

				GenerationGroupKey ggkElder(pGenerationData->GetUiElderId(),FALSE);
				DataObject& dobj4 = gGlobal.generationGroupMap[ggkElder];
				if (dobj4.pobj == NULL) 
				{
					pGenerationGrpElder = SafeCreateObject(NetGroup);
					dobj4.pobj = pGenerationGrpElder;
				} else 
				{
					pGenerationGrpElder = (NetGroup*)dobj4.pobj;
				}

				NetGroup sndGrp;
				CGenerationData* pGntTmp =  CreateNetObject<CGenerationData>();
				if (NULL != pGntTmp) 
				{
					if (pGenerationData->GetBYoung()) 
					{
						GenerationDataKey gdk(pGenerationData->GetUiYoungerId(),
							pGenerationData->GetUiElderId(),
							TRUE);
						DataObject& dobj=gGlobal.generationDataMap[gdk];
						/*if (NULL != dobj.pobj)
							DeleteNetObject(dobj.pobj);*/
						if(NULL != dobj.pobj)
						{
							DeleteNetObject(pGenerationData);
							pGenerationData = CastNetObject<CGenerationData>(dobj.pobj);
						}
						dobj.pobj = pGenerationData;
						pGenerationData->PreSend();
						if (!pGenerationGrpYounger->IsChild(pGenerationData)) 
						{
							pGenerationGrpYounger->Add(pGenerationData);
						}

						pGntTmp->Clone(pGenerationData,CGenerationData::AllMask);
						pGntTmp->SetStrNickName(strSelfName);
						pGntTmp->SetBYoung(FALSE);
						GenerationDataKey gdkTmp(pGntTmp->GetUiYoungerId(),
							pGntTmp->GetUiElderId(),
							FALSE);
						DataObject& dobjTmp=gGlobal.generationDataMap[gdkTmp];
						/*if (NULL != dobjTmp.pobj)
							DeleteNetObject(dobjTmp.pobj);*/
						if(NULL != dobjTmp.pobj)
						{
							DeleteNetObject(pGntTmp);
							pGntTmp = CastNetObject<CGenerationData>(dobjTmp.pobj);
						}
						dobjTmp.pobj = pGntTmp;
						pGntTmp->PreSend();
						if (!pGenerationGrpElder->IsChild(pGntTmp)) 
						{
							pGenerationGrpElder->Add(pGntTmp);
						}
					}
					else
					{
						GenerationDataKey gdkElder(pGenerationData->GetUiYoungerId(),
							pGenerationData->GetUiElderId(),
							FALSE);
						DataObject& dobjElder=gGlobal.generationDataMap[gdkElder];
						/*if (NULL != dobjElder.pobj)
							DeleteNetObject(dobjElder.pobj);*/
						if(NULL != dobjElder.pobj)
						{
							DeleteNetObject(pGenerationData);
							pGenerationData = CastNetObject<CGenerationData>(dobjElder.pobj);
						}
						dobjElder.pobj = pGenerationData;
						pGenerationData->PreSend();
						if (!pGenerationGrpElder->IsChild(pGenerationData)) 
						{
							pGenerationGrpElder->Add(pGenerationData);
						}

						pGntTmp->Clone(pGenerationData,CGenerationData::AllMask);
						pGntTmp->SetStrNickName(strSelfName);
						pGntTmp->SetBYoung(TRUE);
						GenerationDataKey gdkTmp(pGntTmp->GetUiYoungerId(),
							pGntTmp->GetUiElderId(),
							TRUE);
						DataObject& dobjTmp=gGlobal.generationDataMap[gdkTmp];
						/*if (NULL != dobjTmp.pobj)
							DeleteNetObject(dobjTmp.pobj);*/
						if(NULL != dobjTmp.pobj)
						{
							DeleteNetObject(pGntTmp);
							pGntTmp = CastNetObject<CGenerationData>(dobjTmp.pobj);
						}
						dobjTmp.pobj = pGntTmp;
						pGntTmp->PreSend();
						if (!pGenerationGrpYounger->IsChild(pGntTmp))
						{
							pGenerationGrpYounger->Add(pGntTmp);
						}
					}
				}
				sndGrp.Add(pGenerationData);
				ObjectService_Stub::CB_CreateGeneration(pPeer,context,&sndGrp);
				dboGeneration->Close();
				return RPC_RESULT_OK;
			}
		}
		dboGeneration->Close();
	}

	return RPC_RESULT_FAIL;
}

RPCResult  ObjectService_Stub::DeleteGeneration(LPCPEER pPeer,
												RPCContext &context, 
												const UInt32 &uiYoungId,
												const UInt32 &uiElderId,
												const BOOL &bYoung)
{
	GenerationGroupKey ggk;
	ggk.uiOwnerId = uiYoungId;
	ggk.bYoung = TRUE;
	GenerationDataKey gdk;
	gdk.bYoung = TRUE;
	gdk.uiElderId = uiElderId;
	gdk.uiYoungerId = uiYoungId;
	CGenerationData* pGd = FindGeneration(gdk);
	if (NULL != pGd) 
	{
		NetGroup* pGenerationGrp = FindGenerationGroup(ggk);
		if (NULL != pGenerationGrp) 
		{
			if (pGenerationGrp->IsChild(pGd)) 
			{
				pGenerationGrp->Remove(pGd);
			}
		}
		DataObject& dobj = gGlobal.generationDataMap[gdk];
		dobj.pobj = NULL;
		GenerationDataMap::iterator it = gGlobal.generationDataMap.find(gdk);
		if (it != gGlobal.generationDataMap.end())
			gGlobal.generationDataMap.erase(it);
		DeleteNetObject(pGd);
	}
	
	ggk.uiOwnerId = uiElderId;
	ggk.bYoung = FALSE;
	gdk.bYoung = FALSE;
	gdk.uiElderId = uiElderId;
	gdk.uiYoungerId = uiYoungId;
	pGd = FindGeneration(gdk);
	if (NULL != pGd) 
	{
		NetGroup* pGenerationGrp = FindGenerationGroup(ggk);
		if (NULL != pGenerationGrp) 
		{
			if (pGenerationGrp->IsChild(pGd)) 
			{
				pGenerationGrp->Remove(pGd);
			}
		}
		DataObject& dobj = gGlobal.generationDataMap[gdk];
		dobj.pobj = NULL;
		GenerationDataMap::iterator it = gGlobal.generationDataMap.find(gdk);
		if (it != gGlobal.generationDataMap.end())
			gGlobal.generationDataMap.erase(it);
		DeleteNetObject(pGd);
	}

	gGlobal.gTables.pGeneration->Delete(uiYoungId,uiElderId);
	ObjectService_Stub::CB_DeleteGeneration(pPeer, context,uiYoungId,uiElderId,TRUE);
	gGlobal.gTables.pGeneration->Close();

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateGeneration(LPCPEER pPeer,
											   RPCContext &context, 
											   NetGroup* pGeneration)
{
 	NetGroup* pGntGrp = CastNetObject<NetGroup>(pGeneration);
	if (NULL == pGntGrp)
		return RPC_RESULT_FAIL;
	CdboGeneration* dboGeneration = gGlobal.gTables.pGeneration;
	if (NULL == dboGeneration)
		return RPC_RESULT_FAIL;
	for (int i=0;i<pGntGrp->GetCount();i++) 
	{
		CGenerationData* pGd = pGntGrp->GetChild<CGenerationData>(i);
		if (NULL == pGd) 
			continue;

		GenerationDataKey gdk;
		gdk.bYoung = pGd->GetBYoung();
		gdk.uiElderId = pGd->GetUiElderId();
		gdk.uiYoungerId = pGd->GetUiYoungerId();
		CGenerationData* pGdTmp = FindGeneration(gdk);
		if (NULL != pGdTmp)
		{
			pGdTmp->SetUiAccmulOrRecvExp(pGd->GetUiAccmulOrRecvExp());
			pGdTmp->SetUiGiftPointOrHistAccm(pGd->GetUiGiftPointOrHistAccm());
			pGdTmp->UpdateDbo(dboGeneration);
			if (pGdTmp != pGd) 
			{
				DeleteNetObject(pGd);
			}
		}
	}
	DeleteNetGroup(pGeneration);
	ObjectService_Stub::CB_UpdateGeneration(pPeer,context);
	return RPC_RESULT_OK;
}