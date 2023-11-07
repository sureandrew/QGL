//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/QuestFlagData.h"
//-- Library
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboQuestFlag.h"
#include "ObjectService/Global.h"

REGISTER_CLASSTYPE(CQuestFlagData, QuestFlagData_CLASSID);

CdboQuestFlag* &pdbQuestFlag = gGlobal.gTables.pQuestFlag;
StringVector CdboQuestFlagAccessor::m_params;
ThreadKey CQuestFlagData::m_key;

////////////////////////////////////////////////////////////////////////
////				 QuestFlag Data							////
////////////////////////////////////////////////////////////////////////
CQuestFlagData* CQuestFlagData::NewQuestFlag()
{
	Boolean bNewInstance = BOOLEAN_FALSE;
	CQuestFlagData *pQuestFlagData = NULL;
	QuestFlagDataKey QuestFlagKey((UInt32)pdbQuestFlag->m_char_id.LowPart, 
		(UInt16)pdbQuestFlag->m_flag_id);
	DataObject &dobj = gGlobal.questFlagMap[QuestFlagKey];
	gGlobal.AddUnreleasedFlagId(QuestFlagKey.owner_id, QuestFlagKey.flag_id);

	// object already exist
	if (dobj.pobj) 
	{
		pQuestFlagData = CastNetObject<CQuestFlagData>(dobj.pobj);
	}
	if (pQuestFlagData == NULL) 
	{
		pQuestFlagData = CQuestFlagData::NewInstance();
		TRACE_ENSURE(pQuestFlagData != NULL);
		pQuestFlagData->SetData(pdbQuestFlag);			// assign values to character data
		dobj.pobj = pQuestFlagData;
		bNewInstance = BOOLEAN_TRUE;
	}

	// found existing quest log group in cache
	NetGroup* questFlagGroup = FindQuestFlagGroup(QuestFlagKey.owner_id);
	if (questFlagGroup) 
	{
		questFlagGroup->Add(pQuestFlagData);
	} 
	else 
	{
		DataObject& dobj3 = gGlobal.questFlagGroupMap[QuestFlagKey.owner_id];

		questFlagGroup = SafeCreateObject(NetGroup);
		dobj3.pobj = questFlagGroup;

		if (NULL != questFlagGroup) 
			questFlagGroup->Add(pQuestFlagData);
		else if (bNewInstance) 
		{
			dobj.pobj = NULL;
			DeleteNetObject(pQuestFlagData);
			pQuestFlagData = NULL;
		}
	}

	return pQuestFlagData;
}

CQuestFlagData* FindQuestFlag(QuestFlagDataKey &key)
{
	CQuestFlagData* pFlagData = NULL;
	// found existing account in cache
	QuestFlagDataMap::iterator it = gGlobal.questFlagMap.find(key);
	if (it != gGlobal.questFlagMap.end())
		pFlagData = CastNetObject<CQuestFlagData>(it->second.pobj);
	return pFlagData;
}

NetGroup* CQuestFlagData::FindQuestFlagGroup(UInt32 owner_id)
{
	NetGroup* grp = NULL;
	// found existing account in cache
	GroupMap::iterator it = gGlobal.questFlagGroupMap.find(owner_id);
	if (it != gGlobal.questFlagGroupMap.end())
		grp = CastNetObject<NetGroup>(it->second.pobj);
	return grp;
}

void RemoveQuestFlag(UInt32 owner_id, UInt16 flag_id)
{
	QuestFlagDataKey QuestFlagKey(owner_id, flag_id);
	CQuestFlagData* pQuestFlagData	= NULL;
	NetGroup *questFlagGroup = NULL;
	QuestFlagDataMap::iterator iter	= gGlobal.questFlagMap.find(QuestFlagKey);
	if (iter != gGlobal.questFlagMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		pQuestFlagData = CastNetObject<CQuestFlagData>(iter->second.pobj);
		gGlobal.questFlagMap.erase(iter);
		if (pQuestFlagData) 
		{
			questFlagGroup = CQuestFlagData::FindQuestFlagGroup(owner_id);
			if (questFlagGroup)
				questFlagGroup->Remove(pQuestFlagData);
			// directly remove coz of primary constrain
			//gGlobal.m_deleteGroup.Add(pQuestFlagData);
			pQuestFlagData->DeleteDbo(pdbQuestFlag);
			DeleteNetObject(pQuestFlagData);
		}
	}
}

void CQuestFlagData::SetData(CdboQuestFlag* dboQuestFlag)
{
	SetId((UInt32) dboQuestFlag->m_char_id.LowPart, (UInt16) dboQuestFlag->m_flag_id);
	SetFlag((Int32) dboQuestFlag->m_flag);
	ClearUpdateMask();
}

VOID CQuestFlagData::PreSendGroup(NetGroup * questFlagGroup)
{
	if (questFlagGroup == NULL) return;

	for (UInt16 i = 0; i < questFlagGroup->GetCount(); ++i)
	{
		CQuestFlagData *pQuestFlagData = questFlagGroup->GetChild<CQuestFlagData>(i);
		if (pQuestFlagData)
			pQuestFlagData->PreSend();
	}
}

void CQuestFlagData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

HRESULT CreateQuestFlag(CQuestFlagData* pFlagData)
{
	if (pFlagData == NULL)
		return -1;

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CreateQuestFlag: owner_id = %d, flag_id = %d, flag = %d"), 
		pFlagData->GetChar_id(), pFlagData->GetFlag_id(), pFlagData->GetFlag());

	// primary keys
	pdbQuestFlag->m_char_id.QuadPart = pFlagData->GetChar_id();
	pdbQuestFlag->m_flag_id = pFlagData->GetFlag_id();
	// initial values
	pdbQuestFlag->m_flag = pFlagData->GetFlag();

	// insert record
	HRESULT hr = pdbQuestFlag->Insert();
	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbQuestFlag->MoveFirst())) 
	{
		CQuestFlagData::NewQuestFlag();
	} 
	else 
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("CreateQuestFlag Error: owner_id = %d, flag_id = %d, hr = %u"), 
			pFlagData->GetChar_id(), pFlagData->GetFlag_id(), hr);
		pdbQuestFlag->ShowError(hr);
	}
	
	pdbQuestFlag->Close();
	return hr;
}

HRESULT CQuestFlagData::UpdateDbo(CdboQuestFlag* dboQuestFlag)
{
	if (!IsUpdated()) return S_OK;

	ThreadGuard guard(m_key);
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CQuestFlagData UpdateDbo: owner_id %d, flag_id %d, updateMask %d,%d"), 
		GetChar_id(), GetFlag_id(), m_updateMask.low, m_updateMask.high);

	dboQuestFlag->ClearParam();

	if (IsUpdated(flagMask))
	{
		COLUMN_UPDATE(dboQuestFlag, flag, GetFlag());
	}

	HRESULT hr;
	if (DB_FAILED(hr = dboQuestFlag->UpdateByAny(GetChar_id(), GetFlag_id())))
		dboQuestFlag->ShowError(hr);
	else
		ClearUpdateMask();
	dboQuestFlag->ClearParam();
	dboQuestFlag->Close();

	return hr;
}

HRESULT CQuestFlagData::DeleteDbo(CdboQuestFlag* dboQuestFlag)
{
	HRESULT hr;
	//ThreadGuard guard(m_key);

	if (DB_FAILED(hr = dboQuestFlag->Delete(GetChar_id(), GetFlag_id())))
		dboQuestFlag->ShowError(hr);
	dboQuestFlag->Close();

	return hr;
}

RPCResult ObjectService_Stub::UpdateQuestFlags(LPCPEER pPeer, RPCContext &context, 
											  const UInt32 &owner_id, NetGroup* grp,
											  const UInt16Vector &del_ids)
{
	TRACE_ENSURE(pdbQuestFlag && grp);

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("UpdateQuestFlags: owner_id %d, flag_id count %d, del_ids counts %d"), 
		owner_id, grp->GetCount(), del_ids.size());

	NetGroup* flagGroup = CQuestFlagData::FindQuestFlagGroup(owner_id);
	if (flagGroup == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CQuestFlagData UpdateQuestFlags: quest flag group not found, owner_id %d"), owner_id);
		CB_Error(pPeer, context, 1, _T("UpdateQuestFlags: quest flag group not found"));
		return RPC_RESULT_FAIL;
	}

	QuestFlagDataMap::iterator it;
	QuestFlagDataKey key;
	CQuestFlagData *pNewData, *pCurData;

	// delete flag ids first
	for (UInt16Vector::const_iterator it2 = del_ids.begin(); it2 != del_ids.end(); ++it2)
	{
		RemoveQuestFlag(owner_id, *it2);
	}

	key.owner_id = owner_id;
	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		pNewData = grp->GetChild<CQuestFlagData>(i);
		if (pNewData == NULL)
			continue;
		key.flag_id = pNewData->GetFlag_id();
		it = gGlobal.questFlagMap.find(key);
		if (it == gGlobal.questFlagMap.end()) 
		{			// new flag if not exist
			CreateQuestFlag(pNewData);
		} 
		else 
		{										// update flag if exist
			pCurData = CastNetObject<CQuestFlagData>(it->second.pobj);
			if (pCurData) 
			{
				pCurData->Clone(pNewData, pNewData->GetUpdateMask());
				gGlobal.EnqueueUpdate(&it->second);
			}
		}
	}
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	CB_UpdateQuestFlags(pPeer, context);

	return RPC_RESULT_OK;
}