//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/QuestTypeData.h"
//-- Library
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboQuestType.h"
#include "ObjectService/Global.h"

REGISTER_CLASSTYPE(CQuestTypeData, QuestTypeData_CLASSID);

CdboQuestType* &pdbQuestType = gGlobal.gTables.pQuestType;
StringVector CdboQuestTypeAccessor::m_params;
ThreadKey CQuestTypeData::m_key;

////////////////////////////////////////////////////////////////////////
////					 QuestType Data							////
////////////////////////////////////////////////////////////////////////
CQuestTypeData* CQuestTypeData::NewQuestType()
{
	Boolean bNewInstance = BOOLEAN_FALSE;
	CQuestTypeData *pQuestTypeData = NULL;
	QuestTypeDataKey QuestTypeKey((UInt32)pdbQuestType->m_char_id.LowPart, 
		(UInt16)pdbQuestType->m_type_id);
	DataObject &dobj = gGlobal.questTypeMap[QuestTypeKey];
	gGlobal.AddUnreleasedTypeId(QuestTypeKey.owner_id, QuestTypeKey.type_id);

	// object already exist
	if (dobj.pobj) 
	{
		pQuestTypeData = CastNetObject<CQuestTypeData>(dobj.pobj);
	}
	if (pQuestTypeData == NULL) 
	{
		pQuestTypeData = CQuestTypeData::NewInstance();
		TRACE_ENSURE(pQuestTypeData != NULL);
		pQuestTypeData->SetData(pdbQuestType);			// assign values to character data
		dobj.pobj = pQuestTypeData;
		bNewInstance = BOOLEAN_TRUE;
	}

	// found existing quest log group in cache
	NetGroup* questTypeGroup = CQuestTypeData::FindQuestTypeGroup(QuestTypeKey.owner_id);
	if (questTypeGroup) 
	{
		questTypeGroup->Add(pQuestTypeData);
	} 
	else 
	{
		DataObject& dobj3 = gGlobal.questTypeGroupMap[QuestTypeKey.owner_id];
		questTypeGroup = SafeCreateObject(NetGroup);
		dobj3.pobj = questTypeGroup;

		if (NULL != questTypeGroup) 
			questTypeGroup->Add(pQuestTypeData);
		else if (bNewInstance) 
		{
			dobj.pobj = NULL;
			DeleteNetObject(pQuestTypeData);
			pQuestTypeData = NULL;
		}
	}

	return pQuestTypeData;
}

CQuestTypeData* FindQuestType(QuestTypeDataKey &key)
{
	CQuestTypeData* pQTypeData = NULL;
	// found existing account in cache
	QuestTypeDataMap::iterator it = gGlobal.questTypeMap.find(key);
	if (it != gGlobal.questTypeMap.end())
		pQTypeData = CastNetObject<CQuestTypeData>(it->second.pobj);
	return pQTypeData;
}

NetGroup* CQuestTypeData::FindQuestTypeGroup(UInt32 owner_id)
{
	NetGroup* grp = NULL;
	// found existing account in cache
	GroupMap::iterator it = gGlobal.questTypeGroupMap.find(owner_id);
	if (it != gGlobal.questTypeGroupMap.end())
		grp = CastNetObject<NetGroup>(it->second.pobj);
	return grp;
}

HRESULT CreateQuestType(CQuestTypeData* pTypeData)
{
	if (pTypeData == NULL)
		return -1;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("owner_id = %d, type_id = %d"), pTypeData->GetChar_id(), pTypeData->GetType_id());

	// primary keys
	pdbQuestType->m_char_id.QuadPart	= pTypeData->GetChar_id();
	pdbQuestType->m_type_id				= pTypeData->GetType_id();
	// initial values
	COLUMN_STRING_COPY(pdbQuestType->m_Name, pTypeData->GetName().c_str());
	pdbQuestType->m_Round				= pTypeData->GetRound();
	pdbQuestType->m_MaxRound			= pTypeData->GetMaxRound();
	pdbQuestType->m_TotalRound			= pTypeData->GetTotalRound();
	pdbQuestType->m_LimitRound			= pTypeData->GetLimitRound();
	pdbQuestType->m_LimitType			= pTypeData->GetLimitType();
	DBTIMESTAMP dbtime;
	TimeConvert((time_t)pTypeData->GetLastClearTime(), dbtime);
	pdbQuestType->m_LastClearTime		= dbtime;

	// insert record
	HRESULT hr = pdbQuestType->Insert();
	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbQuestType->MoveFirst())) 
	{
		CQuestTypeData::NewQuestType();
	} 
	else 
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("owner_id = %d, type_id = %d, hr = %u"), 
			pTypeData->GetChar_id(), pTypeData->GetType_id(), hr);
		pdbQuestType->ShowError(hr);
	}
	
	pdbQuestType->Close();
	return hr;
}

void RemoveQuestType(UInt32 owner_id, UInt16 type_id)
{
	QuestTypeDataKey key(owner_id, type_id);
	CQuestTypeData* pQuestTypeData	= NULL;
	NetGroup *questTypeGroup = NULL;
	QuestTypeDataMap::iterator iter	= gGlobal.questTypeMap.find(key);
	if (iter != gGlobal.questTypeMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		pQuestTypeData = CastNetObject<CQuestTypeData>(iter->second.pobj);
		gGlobal.questTypeMap.erase(iter);
		if (pQuestTypeData) 
		{
			questTypeGroup = CQuestTypeData::FindQuestTypeGroup(owner_id);
			if (questTypeGroup)
				questTypeGroup->Remove(pQuestTypeData);

			pQuestTypeData->DeleteDbo(pdbQuestType);
			DeleteNetObject(pQuestTypeData);
		}
	}
}

void CQuestTypeData::SetData(CdboQuestType* dboQuestType)
{
	String name(dboQuestType->m_Name);

	SetId((UInt32) dboQuestType->m_char_id.LowPart, (UInt16) dboQuestType->m_type_id);
	SetName(name.Trim());
	SetRound((UInt16) dboQuestType->m_Round);
	SetMaxRound((UInt16) dboQuestType->m_MaxRound);
	SetTotalRound((UInt16) dboQuestType->m_TotalRound);
	SetLimitRound((UInt16) dboQuestType->m_LimitRound);
	SetLimitType((UInt8) dboQuestType->m_LimitType);
	time_t uTime;
	TimeConvert(dboQuestType->m_LastClearTime, uTime);
	SetLastClearTime((UInt32)uTime);

	ClearUpdateMask();
}

VOID CQuestTypeData::PreSendGroup(NetGroup * questTypeGroup)
{
	if (questTypeGroup == NULL) 
		return;

	for (UInt16 i = 0; i < questTypeGroup->GetCount(); ++i)
	{
		CQuestTypeData *pQuestTypeData = questTypeGroup->GetChild<CQuestTypeData>(i);
		if (pQuestTypeData)
			pQuestTypeData->PreSend();
	}
}

VOID CQuestTypeData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

HRESULT CQuestTypeData::UpdateDbo(CdboQuestType* dboQuestType)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("owner_id %d, tid %d, updateMask %d,%d"),GetChar_id(), GetType_id(), m_updateMask.low, m_updateMask.high);

	dboQuestType->ClearParam();

	if (IsUpdated(nameMask))
		COLUMN_UPDATE_STRING(dboQuestType, Name, GetName());
	if (IsUpdated(roundMask))
		COLUMN_UPDATE(dboQuestType, Round, GetRound());
	if (IsUpdated(maxRoundMask))
		COLUMN_UPDATE(dboQuestType, MaxRound, GetMaxRound());
	if (IsUpdated(totalRoundMask))
		COLUMN_UPDATE(dboQuestType, TotalRound, GetTotalRound());
	if (IsUpdated(limitRoundMask))
		COLUMN_UPDATE(dboQuestType, LimitRound, GetLimitRound());
	if (IsUpdated(limitTypeMask))
		COLUMN_UPDATE(dboQuestType, LimitType, GetLimitType());

	if (IsUpdated(lastClearTimeMask)) 
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetLastClearTime(), dbtime);
		COLUMN_UPDATE(dboQuestType, LastClearTime, dbtime);
	}

	HRESULT hr;
	if (DB_FAILED(hr = dboQuestType->UpdateByAny(GetChar_id(), GetType_id())))
		dboQuestType->ShowError(hr);
	else
		ClearUpdateMask();
	dboQuestType->ClearParam();
	dboQuestType->Close();

	return hr;
}

HRESULT CQuestTypeData::DeleteDbo(CdboQuestType* dboQuestType)
{
	HRESULT hr;
	//ThreadGuard guard(m_key);

	if (DB_FAILED(hr = dboQuestType->Delete(GetChar_id(), GetType_id())))
		dboQuestType->ShowError(hr);
	dboQuestType->Close();

	return hr;
}

RPCResult ObjectService_Stub::UpdateQuestTypes(LPCPEER pPeer, RPCContext &context, 
											   const UInt32 &owner_id, NetGroup* grp)
{
	TRACE_ENSURE(pdbQuestType && grp);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("owner_id %d, count %d"), owner_id, grp->GetCount());
	NetGroup* typeGroup = CQuestTypeData::FindQuestTypeGroup(owner_id);
	if (typeGroup == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("quest type group not found, owner_id %d"), owner_id);
		return RPC_RESULT_FAIL;
	}

	QuestTypeDataMap::iterator it;
	QuestTypeDataKey key;
	CQuestTypeData *pNewData = NULL;
	CQuestTypeData *pCurData = NULL;
	UInt32Vector del_ids;

	key.owner_id = owner_id;
	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		pNewData = grp->GetChild<CQuestTypeData>(i);
		if (pNewData == NULL)
			continue;
		key.type_id = pNewData->GetType_id();
		it = gGlobal.questTypeMap.find(key);
		if (it == gGlobal.questTypeMap.end())
		{
			CreateQuestType(pNewData);// new quest type if not exist
		}
		else 
		{
			if (pNewData->GetStatus() == CQuestTypeData::ST_DELETED)
			{
				del_ids.push_back(pNewData->GetType_id());// delete if marked
			}
			else
			{
				// update quest type if exist
				pCurData = CastNetObject<CQuestTypeData>(it->second.pobj);
				if (pCurData)
				{
					pCurData->Clone(pNewData, pNewData->GetUpdateMask());
					pCurData->UnmarkUpdate(CQuestTypeData::statusMask);
					if (pCurData->IsUpdated())
						gGlobal.EnqueueUpdate(&it->second);
				}
			}
		}
	}
	DeleteNetGroup(grp, BOOLEAN_TRUE);

	// delete quest type after update
	for (UInt32Vector::iterator it2 = del_ids.begin(); it2 != del_ids.end(); ++it2)
	{
		RemoveQuestType(owner_id, *it2);
	}

	CB_UpdateQuestTypes(pPeer, context);

	return RPC_RESULT_OK;
}