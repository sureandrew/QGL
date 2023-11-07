//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/QuestNpcFlagData.h"
//-- Library
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboQuestNpcFlag.h"
#include "ObjectService/Global.h"
#include "Stub/Service/ObjectService.h"

REGISTER_CLASSTYPE(CQuestNpcFlagData, QuestNpcFlagData_CLASSID);
CdboQuestNpcFlag* &pdbQuestNpcFlag = gGlobal.gTables.pQuestNpcFlag;
StringVector CdboQuestNpcFlagAccessor::m_params;
ThreadKey CQuestNpcFlagData::m_key;
typedef WorkRequest<UInt32> QuestNpcFlagRequest;

HRESULT CQuestNpcFlagData::DoCreateNpcFlag(const UInt32 &npc_id, const UInt16 &line_id, const UInt32 &owner_id, const UInt16 &flag_id, const UInt16 &flag)
{
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("npc_id = %d,line_id = %d,owner_id = %d,flag_id = %d,flag = %d"), npc_id,line_id,owner_id,flag_id,flag);
	HRESULT hr = pdbQuestNpcFlag->Insert(npc_id, line_id,owner_id, flag_id,flag);
	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbQuestNpcFlag->MoveFirst()))
	{
		NewNpcFlag();
	}
	pdbQuestNpcFlag->Close();

	return hr;

}
DWORD WINAPI CQuestNpcFlagData::QueueGetNpcFlag(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();
	QuestNpcFlagRequest* pRequest = (QuestNpcFlagRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();

	if (dbSrc)
	{
		NetGroup questNpcFlagCroup;
		CQuestNpcFlagData* pQuestNpcFlagData;
		CdboQuestNpcFlag* dboQuestNpcFlag = SafeCreateObject(CdboQuestNpcFlag);

		questNpcFlagCroup.SetValidate(BOOLEAN_FALSE);
		dboQuestNpcFlag->SetDataSource(dbSrc);
		if (DB_SUCCEEDED(hr = dboQuestNpcFlag->RSByLineId(pRequest->key)))
		{
			hr = dboQuestNpcFlag->MoveFirst();
			while (hr == S_OK)
			{
				pQuestNpcFlagData = SafeCreateObject(CQuestNpcFlagData);
				pQuestNpcFlagData->SetData(dboQuestNpcFlag);
				pQuestNpcFlagData->PreSend();
				questNpcFlagCroup.Add(pQuestNpcFlagData);
				hr = dboQuestNpcFlag->MoveNext();
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetNpcFlag"), hr, dboQuestNpcFlag->GetErrorMsg(hr), 
			pRequest->context, &questNpcFlagCroup);
		dboQuestNpcFlag->Close();
		SafeDeleteObject(dboQuestNpcFlag);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, line_id %d"),
			pRequest->context.sessionId.GetString().c_str(), pRequest->key);
	}
	// show query time
	UInt32 dt = ::GetTickCount() - curTime;
	if (dt > 1000)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("profile, utime %d, line_id %d"), 
			dt, pRequest->key);
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}
CQuestNpcFlagData* CQuestNpcFlagData::FindNpcFlag(const UInt32 &npc_id, const UInt16 &line_id, const UInt32 &owner_id, const UInt16 &flag_id)
{
	NpcFlagIndex flagIndex(npc_id,line_id,owner_id,flag_id);

	NpcFlagMap::iterator iter = gGlobal.npcFlagMap.find(flagIndex);
	if (iter != gGlobal.npcFlagMap.end())
	{
		return CastNetObject<CQuestNpcFlagData>(iter->second.pobj);
	}
	return NULL;
}

HRESULT CQuestNpcFlagData::RemoveNpcFlag(const UInt32 &npc_id, const UInt16 &line_id, const UInt32 &owner_id, const UInt16 &flag_id)
{
	NpcFlagIndex flagIndex(npc_id,line_id,owner_id,flag_id);

	NpcFlagMap::iterator iter = gGlobal.npcFlagMap.find(flagIndex);
	if (iter != gGlobal.npcFlagMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CQuestNpcFlagData* pQuestNpcFlagData = CastNetObject<CQuestNpcFlagData>(iter->second.pobj);
		if (pQuestNpcFlagData) 
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("npc_id = %d"), npc_id);
			gGlobal.m_deleteGroup.Add(pQuestNpcFlagData);
		}
		gGlobal.npcFlagMap.erase(iter);
	}

	return S_OK;
}

CQuestNpcFlagData* CQuestNpcFlagData::NewNpcFlag()
{
	CQuestNpcFlagData *pQuestNpcFlagData = NULL;
	NpcFlagIndex flagIndex((UInt32)pdbQuestNpcFlag->m_npc_id.QuadPart,(UInt16)pdbQuestNpcFlag->m_line_id,
		(UInt32)pdbQuestNpcFlag->m_owner_id.QuadPart
		,(UInt16)pdbQuestNpcFlag->m_flag_id);

	DataObject &dobj = gGlobal.npcFlagMap[flagIndex];

	if (dobj.pobj == NULL) 
	{
		pQuestNpcFlagData = CQuestNpcFlagData::NewInstance();
		pQuestNpcFlagData->SetData(pdbQuestNpcFlag);			// assign values to character data
		dobj.pobj = pQuestNpcFlagData;
	} 
	else 
	{
		pQuestNpcFlagData = CastNetObject<CQuestNpcFlagData>(dobj.pobj);
	}

	return pQuestNpcFlagData;
}

void CQuestNpcFlagData::ReleaseNpcFlag()
{

}

void CQuestNpcFlagData::SetData(CdboQuestNpcFlag* dboQuestNpcFlag)
{
	SetNpc_id((UInt32)dboQuestNpcFlag->m_npc_id.QuadPart);
	SetLine_id((UInt16)dboQuestNpcFlag->m_line_id);
	SetOwner_id((UInt32)dboQuestNpcFlag->m_owner_id.QuadPart);
	SetFlag_id((UInt16)dboQuestNpcFlag->m_flag_id);
	SetFlag((UInt16)dboQuestNpcFlag->m_flag);
	ClearUpdateMask();
}

void CQuestNpcFlagData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

HRESULT CQuestNpcFlagData::UpdateDbo(CdboQuestNpcFlag* dboQuestNpcFlag)
{
	if (!IsUpdated()) return S_OK;

	ThreadGuard guard(m_key);

	dboQuestNpcFlag->ClearParam();

	if (IsUpdated(flagMask))
	{
		COLUMN_UPDATE(dboQuestNpcFlag, flag, GetFlag());
	}

	HRESULT hr;
	if (DB_FAILED(hr = dboQuestNpcFlag->UpdateByAny(GetNpc_id(), GetLine_id(),GetOwner_id(),GetFlag_id())))
		dboQuestNpcFlag->ShowError(hr);
	else
		ClearUpdateMask();
	dboQuestNpcFlag->ClearParam();
	dboQuestNpcFlag->Close();

	return hr;
}

HRESULT CQuestNpcFlagData::DeleteDbo(CdboQuestNpcFlag* dboQuestNpcFlag)
{
	HRESULT hr;
	//ThreadGuard guard(m_key);

	if (DB_FAILED(hr = dboQuestNpcFlag->Delete(GetNpc_id(), GetLine_id(),GetOwner_id(),GetFlag_id())))
		dboQuestNpcFlag->ShowError(hr);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("npc_id = %d"), GetNpc_id());
	dboQuestNpcFlag->Close();

	return hr;
}

RPCResult ObjectService_Stub::GetNpcFlagsByLineId(LPCPEER pPeer, RPCContext &context, const UInt16 &line_id)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC1(ObjectService, GetNpcFlagsByLineId, NULL, UInt16, line_id);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("line_id = %d"), line_id);

	TRACE_ENSURE(pdbQuestNpcFlag != NULL);
	// wait for thread pool to process
	gGlobal.WorkQueueBegin();
	QuestNpcFlagRequest *pRequest = SafeCreateObject3(QuestNpcFlagRequest, line_id, pPeer, context);
#ifdef DB_THREAD_POOL
	gGlobal.QueueUserWorkItem(CQuestNpcFlagData::QueueGetNpcFlag, pRequest, DB_WORKER_FLAG);
#else
	CQuestNpcFlagData::QueueGetNpcFlag(pRequest);
#endif


	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, netGroup)

	TRACE_ENSURE(netGroup);

	NetGroup sendGrp;
	CQuestNpcFlagData* pQuestNpcFlagData = NULL;

	for (UInt16 i = 0; i < netGroup->GetCount(); i++)
	{
		pQuestNpcFlagData = netGroup->GetChild<CQuestNpcFlagData>(i);

		if (pQuestNpcFlagData)
		{
			NpcFlagIndex flagIndex(pQuestNpcFlagData->GetNpc_id(),pQuestNpcFlagData->GetLine_id(),pQuestNpcFlagData->GetOwner_id(),pQuestNpcFlagData->GetFlag_id());
			DataObject &dobj = gGlobal.npcFlagMap[flagIndex];
			if (dobj.pobj != NULL)
			{
				DeleteNetObject(pQuestNpcFlagData);
				pQuestNpcFlagData = CastNetObject<CQuestNpcFlagData>(dobj.pobj);
			}
			else
			{
				LocalRPCManager::GetInstance().Register(pQuestNpcFlagData);
				pQuestNpcFlagData->ClearUpdateMask();
				dobj.pobj = pQuestNpcFlagData;
			}
			if (pQuestNpcFlagData)
			{
				pQuestNpcFlagData->PreSend();
				sendGrp.Add(pQuestNpcFlagData);
				if(sendGrp.GetCount() >= 100)
				{
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("sendGrp.GetCount() >= 100,sendGrp.GetCount() = %d"), sendGrp.GetCount());
					CB_GetNpcFlagsByLineId(pPeer, context, &sendGrp);
					sendGrp.Clear(false);
				}
			}
		}
	}

	if(sendGrp.GetCount() > 0)
	{
		CB_GetNpcFlagsByLineId(pPeer, context, &sendGrp);
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("sendGrp.GetCount() = %d"), sendGrp.GetCount());
	}
	DeleteNetGroup(netGroup);

	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("session_id %s, errcode %d, %s"),context.sessionId.GetString().c_str(), err, errMsg.c_str());
	CB_Error(pPeer, context, err, errMsg);
	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;

}

RPCResult ObjectService_Stub::UpdateNpcFlags(LPCPEER pPeer, RPCContext &context, NetGroup* grpUpdate, NetGroup* grpDelete)
{
	TRACE_ENSURE(pdbQuestNpcFlag && grpUpdate && grpDelete);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("grpUpdate count %d, grpDelete counts %d"), grpUpdate->GetCount(), grpDelete->GetCount());

	for (UInt16 i = 0; i < grpDelete->GetCount(); i++)
	{ 
		CQuestNpcFlagData *pData = grpDelete->GetChild<CQuestNpcFlagData>(i);
		if (pData == NULL)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("pData == NULL"));
			continue;
		}
		CQuestNpcFlagData::RemoveNpcFlag(pData->GetNpc_id(),pData->GetLine_id(),pData->GetOwner_id(),pData->GetFlag_id());
		if(pData != CQuestNpcFlagData::FindNpcFlag(pData->GetNpc_id(),pData->GetLine_id(),pData->GetOwner_id(),pData->GetFlag_id()))
			DeleteNetObject(pData);
	}
	DeleteNetGroup(grpDelete);

	for (UInt16 i = 0; i < grpUpdate->GetCount(); i++)
	{
		CQuestNpcFlagData *pData = grpUpdate->GetChild<CQuestNpcFlagData>(i);
		if (pData == NULL)
			continue;
		NpcFlagIndex flagIndex(pData->GetNpc_id(),pData->GetLine_id(),pData->GetOwner_id(),pData->GetFlag_id());
		NpcFlagMap::iterator iter = gGlobal.npcFlagMap.find(flagIndex);
		if (iter == gGlobal.npcFlagMap.end()) 
		{			// new flag if not exist
			CQuestNpcFlagData::DoCreateNpcFlag(pData->GetNpc_id(),pData->GetLine_id(),pData->GetOwner_id(),pData->GetFlag_id(),pData->GetFlag());
		} 
		else 
		{										// update flag if exist
			CQuestNpcFlagData *pCurData = CastNetObject<CQuestNpcFlagData>(iter->second.pobj);
			if (pCurData) 
			{
				pCurData->Clone(pData, pData->GetUpdateMask());
				gGlobal.EnqueueUpdate(&iter->second);
			}
		}
	}
	DeleteNetGroup(grpUpdate, BOOLEAN_TRUE);
	CB_UpdateNpcFlags(pPeer, context);

	return RPC_RESULT_OK;
}
