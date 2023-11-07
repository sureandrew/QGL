//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/QuestLogData.h"
//-- Library
#include "Stub/Service/ObjectService.h"
#include "Stub/Data/QuestFlagData.h"
#include "Stub/Data/QuestTypeData.h"
#include "ObjectService/dboQuestLog.h"
#include "ObjectService/dboQuestFlag.h"
#include "ObjectService/dboQuestType.h"
#include "ObjectService/Global.h"
#include "ObjectService/DboSourcePool.h"

REGISTER_CLASSTYPE(CQuestLogData, QuestLogData_CLASSID);

typedef WorkRequest<UInt32> QuestRequest;

CdboQuestLog* &pdbQuestLog = gGlobal.gTables.pQuestLog;
StringVector CdboQuestLogAccessor::m_params;
ThreadKey CQuestLogData::m_key;

////////////////////////////////////////////////////////////////////////
////						QuestLog Data							////
////////////////////////////////////////////////////////////////////////
CQuestLogData* CQuestLogData::NewQuestLog()
{
	Boolean bNewInstance=BOOLEAN_FALSE;
	CQuestLogData *pQuestLogData = NULL;
	QuestLogDataKey QuestLogKey((UInt32) pdbQuestLog->m_char_id.LowPart, 
		(UInt16) pdbQuestLog->m_quest_id);
	DataObject &dobj = gGlobal.questLogMap[QuestLogKey];

	if (dobj.pobj == NULL) {
		pQuestLogData = CQuestLogData::NewInstance();
		pQuestLogData->SetData(pdbQuestLog);			// assign values to character data
		dobj.pobj = pQuestLogData;
		bNewInstance=BOOLEAN_TRUE;
	} else {
		pQuestLogData = CastNetObject<CQuestLogData>(dobj.pobj);
	}

	// found existing quest log group in cache
	NetGroup* questLogGroup = CQuestLogData::FindQuestLogGroup(QuestLogKey.owner_id);
	if (questLogGroup)
	{
		questLogGroup->Add(pQuestLogData);
	}
	else
	{
		DataObject& dobj3 = gGlobal.questLogGroupMap[QuestLogKey.owner_id];
		questLogGroup = SafeCreateObject(NetGroup);
		dobj3.pobj = questLogGroup;

		if (NULL != questLogGroup) 
			questLogGroup->Add(pQuestLogData);
		else if (bNewInstance) 
		{
			dobj.pobj = NULL;
			DeleteNetObject(pQuestLogData);
			pQuestLogData=NULL;
		}
	}

	return pQuestLogData;
}

CQuestLogData* FindQuestLog(QuestLogDataKey &key)
{
	CQuestLogData* pQuestLogData = NULL;
	// found existing account in cache
	QuestLogDataMap::iterator it = gGlobal.questLogMap.find(key);
	if (it != gGlobal.questLogMap.end())
		pQuestLogData = CastNetObject<CQuestLogData>(it->second.pobj);
	return pQuestLogData;
}

NetGroup* CQuestLogData::FindQuestLogGroup(UInt32 owner_id)
{
	GroupMap::iterator iter = gGlobal.questLogGroupMap.find(owner_id);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.questLogGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}

void RemoveQuestLog(UInt32 owner_id, UInt32 quest_id)
{
	QuestLogDataKey key(owner_id, quest_id);
	CQuestLogData* pQuestLogData	= NULL;
	NetGroup *questLogGroup = NULL;
	QuestLogDataMap::iterator iter	= gGlobal.questLogMap.find(key);
	if (iter != gGlobal.questLogMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		pQuestLogData = CastNetObject<CQuestLogData>(iter->second.pobj);
		gGlobal.questLogMap.erase(iter);
		if (pQuestLogData) {
			questLogGroup = CQuestLogData::FindQuestLogGroup(owner_id);
			if (questLogGroup)
				questLogGroup->Remove(pQuestLogData);
			// directly remove coz of primary constrain
			//gGlobal.m_deleteGroup.Add(pQuestLogData);
			pQuestLogData->DeleteDbo(pdbQuestLog);
			DeleteNetObject(pQuestLogData);
		}
	}
}

void ReleaseQuests(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.questLogGroupMap.find(char_id);

	if (iter != gGlobal.questLogGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		iter->second.pobj = NULL;
		CQuestLogData *pQuestLogData = NULL;
	
		if (grp == NULL) 
			return;

		for (UInt i = 0; i < grp->GetCount(); i++) 
		{
			pQuestLogData = grp->GetChild<CQuestLogData>(i);
			if (pQuestLogData) 
			{
				QuestLogDataMap::iterator it = gGlobal.questLogMap.find(
					QuestLogDataKey(pQuestLogData->GetChar_id(), pQuestLogData->GetQuest_id()));
				if (it != gGlobal.questLogMap.end()) 
				{		// remove from update queue
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.questLogMap.erase(it);
				}
				if (pQuestLogData->IsUpdated())				// write to db if updated
					gGlobal.m_releaseGroup.Add(pQuestLogData);
				else
					DeleteNetObject(pQuestLogData);
			}
		}
		gGlobal.questLogGroupMap.erase(iter);
		SafeDeleteObject(grp);
	}

	GroupMap::iterator iter2 = gGlobal.questTypeGroupMap.find(char_id);
	if (iter2 != gGlobal.questTypeGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter2->second.pobj);
		iter2->second.pobj = NULL;
		CQuestTypeData *pQuestTypeData;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++) 
		{
			pQuestTypeData = grp->GetChild<CQuestTypeData>(i);
			if (pQuestTypeData) 
			{
				QuestTypeDataMap::iterator it = gGlobal.questTypeMap.find(
					QuestTypeDataKey(pQuestTypeData->GetChar_id(), 
					pQuestTypeData->GetType_id()));
				if (it != gGlobal.questTypeMap.end()) 
				{
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.questTypeMap.erase(it);
				}
				if (pQuestTypeData->IsUpdated())
					gGlobal.m_releaseGroup.Add(pQuestTypeData);
				else
					DeleteNetObject(pQuestTypeData);
			}
		}

		// extra detection to ensure clean questTypeMap
		UInt16 type_id = gGlobal.PopUnreleasedTypeId(char_id);
		while( type_id )
		{
			QuestTypeDataMap::iterator it = gGlobal.questTypeMap.find(QuestTypeDataKey(char_id, type_id));
			if( it != gGlobal.questTypeMap.end() )
			{
				gGlobal.questTypeMap.erase(it);
				TRACE_WARNDTL_2(GLOBAL_LOGGER,
					_FW("undeleted data found, char_id %d, type_id %d"),
					char_id, type_id);
			}
			type_id = gGlobal.PopUnreleasedTypeId(char_id);
		}

		gGlobal.questTypeGroupMap.erase(iter2);
		SafeDeleteObject(grp);
	}

	GroupMap::iterator iter3 = gGlobal.questFlagGroupMap.find(char_id);
	if (iter3 != gGlobal.questFlagGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter3->second.pobj);
		iter3->second.pobj = NULL;
		CQuestFlagData *pQuestFlagData;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++) 
		{
			pQuestFlagData = grp->GetChild<CQuestFlagData>(i);
			if (pQuestFlagData) 
			{
				QuestFlagDataMap::iterator it = gGlobal.questFlagMap.find(
					QuestFlagDataKey(pQuestFlagData->GetChar_id(), 
					pQuestFlagData->GetFlag_id()));
				if (it != gGlobal.questFlagMap.end()) 
				{
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.questFlagMap.erase(it);
				}
				if (pQuestFlagData->IsUpdated())
					gGlobal.m_releaseGroup.Add(pQuestFlagData);
				else
					DeleteNetObject(pQuestFlagData);
			}
		}

		// extra detection to ensure clean questFlagMap
		UInt16 flag_id = gGlobal.PopUnreleasedFlagId(char_id);
		while( flag_id )
		{
			QuestFlagDataMap::iterator it = gGlobal.questFlagMap.find(
				QuestFlagDataKey(char_id, flag_id)
				);
			if( it != gGlobal.questFlagMap.end() )
			{
				gGlobal.questFlagMap.erase(it);
				TRACE_WARNDTL_2(GLOBAL_LOGGER,
					_FW("undeleted data found, char_id %d, flag_id %d"),
					char_id, flag_id);
			}
			flag_id = gGlobal.PopUnreleasedFlagId(char_id);
		}
		gGlobal.questFlagGroupMap.erase(iter3);
		SafeDeleteObject(grp);
	}
}

////////////////////////////////////////////////////////////////////////

void CQuestLogData::SetData(CdboQuestLog* dboQuestLog)
{
	String name(dboQuestLog->m_name);

	SetId(		(UInt32) dboQuestLog->m_char_id.LowPart,
				(UInt16) dboQuestLog->m_quest_id);
	SetInfo(	(UInt16) dboQuestLog->m_type_id,
				name.Trim(), dboQuestLog->m_descript);
	SetBeginNpc((UInt32) dboQuestLog->m_beginNpc.LowPart,
				(UInt16) dboQuestLog->m_beginNpc_Map, 
				(UInt16) dboQuestLog->m_beginNpc_x,
				(UInt16) dboQuestLog->m_beginNpc_y);
	SetFinishNpc((UInt32) dboQuestLog->m_finishNpc.LowPart,
				(UInt16) dboQuestLog->m_finishNpc_Map, 
				(UInt16) dboQuestLog->m_finishNpc_x,
				(UInt16) dboQuestLog->m_finishNpc_y);
	SetTarget1( (UInt32) dboQuestLog->m_target1_id.LowPart,
				(UInt16) dboQuestLog->m_target1_v1, 
				(UInt16) dboQuestLog->m_target1_v2,
				(UInt16) dboQuestLog->m_target1_v3,
				(UInt8)  dboQuestLog->m_target1_vtype,
				(UInt16) dboQuestLog->m_target1_num,
				(UInt16) dboQuestLog->m_target1_max);
	SetTarget2( (UInt32) dboQuestLog->m_target2_id.LowPart,
				(UInt16) dboQuestLog->m_target2_v1, 
				(UInt16) dboQuestLog->m_target2_v2,
				(UInt16) dboQuestLog->m_target2_v3,
				(UInt8)  dboQuestLog->m_target2_vtype,
				(UInt16) dboQuestLog->m_target2_num,
				(UInt16) dboQuestLog->m_target2_max);
	SetTarget3( (UInt32) dboQuestLog->m_target3_id.LowPart,
				(UInt16) dboQuestLog->m_target3_v1, 
				(UInt16) dboQuestLog->m_target3_v2,
				(UInt16) dboQuestLog->m_target3_v3,
				(UInt8)  dboQuestLog->m_target3_vtype,
				(UInt16) dboQuestLog->m_target3_num,
				(UInt16) dboQuestLog->m_target3_max);
	SetTarget4( (UInt32) dboQuestLog->m_target4_id.LowPart,
				(UInt16) dboQuestLog->m_target4_v1, 
				(UInt16) dboQuestLog->m_target4_v2,
				(UInt16) dboQuestLog->m_target4_v3,
				(UInt8)  dboQuestLog->m_target4_vtype,
				(UInt16) dboQuestLog->m_target4_num,
				(UInt16) dboQuestLog->m_target4_max);
	SetTarget5( (UInt32) dboQuestLog->m_target5_id.LowPart,
				(UInt16) dboQuestLog->m_target5_v1, 
				(UInt16) dboQuestLog->m_target5_v2,
				(UInt16) dboQuestLog->m_target5_v3,
				(UInt8)  dboQuestLog->m_target5_vtype,
				(UInt16) dboQuestLog->m_target5_num,
				(UInt16) dboQuestLog->m_target5_max);
	time_t uTime;
	TimeConvert(dboQuestLog->m_timeLimit, uTime);
	SetTimer(	(UInt32) uTime,
				(UInt16) dboQuestLog->m_interval);
	SetBuff(	(UInt16) dboQuestLog->m_buff);
	SetState(	(UInt8)  dboQuestLog->m_state);
	SetFeature(	(UInt16) dboQuestLog->m_feature);

	ClearUpdateMask();
}

HRESULT CreateQuestLog(CQuestLogData* pQuestLogData)
{
	if (pQuestLogData == NULL)
		return -1;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreateQuestLog: char_id = %d, quest_id = %d"), 
		pQuestLogData->GetChar_id(), pQuestLogData->GetQuest_id());

	// primary keys
	pdbQuestLog->m_char_id.QuadPart		= pQuestLogData->GetChar_id();
	pdbQuestLog->m_quest_id				= pQuestLogData->GetQuest_id();
	// initial values
	pdbQuestLog->m_type_id				= pQuestLogData->GetType_id();
	_tcsncpy(pdbQuestLog->m_name, pQuestLogData->GetName().c_str(), 8);
	pdbQuestLog->m_name[8]				= 0;
	_tcsncpy(pdbQuestLog->m_descript,  pQuestLogData->GetDescript().c_str(), 250);
	pdbQuestLog->m_descript[250]		= 0;

	pdbQuestLog->m_beginNpc.QuadPart	= pQuestLogData->GetBeginNpc();
	pdbQuestLog->m_beginNpc_Map			= pQuestLogData->GetBeginNpc_Map();
	pdbQuestLog->m_beginNpc_x			= pQuestLogData->GetBeginNpc_x();
	pdbQuestLog->m_beginNpc_y			= pQuestLogData->GetBeginNpc_y();

	pdbQuestLog->m_finishNpc.QuadPart	= pQuestLogData->GetFinishNpc();
	pdbQuestLog->m_finishNpc_Map		= pQuestLogData->GetFinishNpc_Map();
	pdbQuestLog->m_finishNpc_x			= pQuestLogData->GetFinishNpc_x();
	pdbQuestLog->m_finishNpc_y			= pQuestLogData->GetFinishNpc_y();

	pdbQuestLog->m_target1_id.QuadPart	= pQuestLogData->GetTarget1_id();
	pdbQuestLog->m_target1_v1			= pQuestLogData->GetTarget1_v1();
	pdbQuestLog->m_target1_v2			= pQuestLogData->GetTarget1_v2();
	pdbQuestLog->m_target1_v3			= pQuestLogData->GetTarget1_v3();
	pdbQuestLog->m_target1_vtype		= pQuestLogData->GetTarget1_vtype();
	pdbQuestLog->m_target1_num			= pQuestLogData->GetTarget1_num();
	pdbQuestLog->m_target1_max			= pQuestLogData->GetTarget1_max();

	pdbQuestLog->m_target2_id.QuadPart	= pQuestLogData->GetTarget2_id();
	pdbQuestLog->m_target2_v1			= pQuestLogData->GetTarget2_v1();
	pdbQuestLog->m_target2_v2			= pQuestLogData->GetTarget2_v2();
	pdbQuestLog->m_target2_v3			= pQuestLogData->GetTarget2_v3();
	pdbQuestLog->m_target2_vtype		= pQuestLogData->GetTarget2_vtype();
	pdbQuestLog->m_target2_num			= pQuestLogData->GetTarget2_num();
	pdbQuestLog->m_target2_max			= pQuestLogData->GetTarget2_max();

	pdbQuestLog->m_target3_id.QuadPart	= pQuestLogData->GetTarget3_id();
	pdbQuestLog->m_target3_v1			= pQuestLogData->GetTarget3_v1();
	pdbQuestLog->m_target3_v2			= pQuestLogData->GetTarget3_v2();
	pdbQuestLog->m_target3_v3			= pQuestLogData->GetTarget3_v3();
	pdbQuestLog->m_target3_vtype		= pQuestLogData->GetTarget3_vtype();
	pdbQuestLog->m_target3_num			= pQuestLogData->GetTarget3_num();
	pdbQuestLog->m_target3_max			= pQuestLogData->GetTarget3_max();

	pdbQuestLog->m_target4_id.QuadPart	= pQuestLogData->GetTarget4_id();
	pdbQuestLog->m_target4_v1			= pQuestLogData->GetTarget4_v1();
	pdbQuestLog->m_target4_v2			= pQuestLogData->GetTarget4_v2();
	pdbQuestLog->m_target4_v3			= pQuestLogData->GetTarget4_v3();
	pdbQuestLog->m_target4_vtype		= pQuestLogData->GetTarget4_vtype();
	pdbQuestLog->m_target4_num			= pQuestLogData->GetTarget4_num();
	pdbQuestLog->m_target4_max			= pQuestLogData->GetTarget4_max();

	pdbQuestLog->m_target5_id.QuadPart	= pQuestLogData->GetTarget5_id();
	pdbQuestLog->m_target5_v1			= pQuestLogData->GetTarget5_v1();
	pdbQuestLog->m_target5_v2			= pQuestLogData->GetTarget5_v2();
	pdbQuestLog->m_target5_v3			= pQuestLogData->GetTarget5_v3();
	pdbQuestLog->m_target5_vtype		= pQuestLogData->GetTarget5_vtype();
	pdbQuestLog->m_target5_num			= pQuestLogData->GetTarget5_num();
	pdbQuestLog->m_target5_max			= pQuestLogData->GetTarget5_max();

	TimeConvert((time_t) pQuestLogData->GetTimeLimit(), pdbQuestLog->m_timeLimit);
	pdbQuestLog->m_interval				= pQuestLogData->GetInterval();
	pdbQuestLog->m_buff					= pQuestLogData->GetBuff();
	pdbQuestLog->m_state				= pQuestLogData->GetState();
	pdbQuestLog->m_feature				= pQuestLogData->GetFeature();

	HRESULT hr = pdbQuestLog->Insert();
	// input new questlogdata to DB
	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbQuestLog->MoveFirst()))
		CQuestLogData::NewQuestLog();
	else
		pdbQuestLog->ShowError(hr);
	
	pdbQuestLog->Close();

	return hr;
}

DWORD WINAPI QueueGetQuests(LPVOID lpParam)
{
	HRESULT hr = S_OK;

	QuestRequest* pRequest = (QuestRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
	{
		NetGroup questLogGroup;
		CQuestLogData* pQuestLogData;
		CdboQuestLog* dboQuestLog = SafeCreateObject(CdboQuestLog);

		questLogGroup.SetValidate(BOOLEAN_FALSE);
		dboQuestLog->SetDataSource(dbSrc);
		if (DB_SUCCEEDED(hr = dboQuestLog->RSByOwner(pRequest->key)))
		{
			hr = dboQuestLog->MoveFirst();
			while (hr == S_OK)
			{
				pQuestLogData = SafeCreateObject(CQuestLogData);
				pQuestLogData->SetData(dboQuestLog);
				pQuestLogData->PreSend();
				questLogGroup.Add(pQuestLogData);
				hr = dboQuestLog->MoveNext();
			}
		}
		dboQuestLog->Close();

		NetGroup questTypeGroup;
		CQuestTypeData* pQuestTypeData;
		CdboQuestType* dboQuestType = SafeCreateObject(CdboQuestType);

		questTypeGroup.SetValidate(BOOLEAN_FALSE);
		dboQuestType->SetDataSource(dbSrc);
		if (DB_SUCCEEDED(hr = dboQuestType->RSByOwner(pRequest->key)))
		{
			hr = dboQuestType->MoveFirst();
			while (hr == S_OK)
			{
				pQuestTypeData = SafeCreateObject(CQuestTypeData);
				pQuestTypeData->SetData(dboQuestType);
				pQuestTypeData->PreSend();
				questTypeGroup.Add(pQuestTypeData);
				hr = dboQuestType->MoveNext();
			}
		}
		dboQuestType->Close();

		NetGroup questFlagGroup;
		CQuestFlagData* pQuestFlagData;
		CdboQuestFlag* dboQuestFlag = SafeCreateObject(CdboQuestFlag);

		questFlagGroup.SetValidate(BOOLEAN_FALSE);
		dboQuestFlag->SetDataSource(dbSrc);
		if (DB_SUCCEEDED(hr = dboQuestFlag->RSByOwner(pRequest->key)))
		{
			hr = dboQuestFlag->MoveFirst();
			while (hr == S_OK)
			{
				pQuestFlagData = SafeCreateObject(CQuestFlagData);
				pQuestFlagData->SetData(dboQuestFlag);
				pQuestFlagData->PreSend();
				questFlagGroup.Add(pQuestFlagData);
				hr = dboQuestFlag->MoveNext();
			}
		}
		dboQuestFlag->Close();

		pRequest->context.bCallback = BOOLEAN_FALSE;
		if (DB_SUCCEEDED(hr) || hr == DB_S_ENDOFROWSET)
		{
			Buffer* pBuf = SafeCreateObject(Buffer);
			Reuben::Network::RPCHeader header(RPC_ObjectService_CB_QueueQuests);
			pBuf->Write<Reuben::Network::RPCHeader>(header);
			pBuf->Write<RPCContext>(pRequest->context);
			pBuf->Write<NetGroup*>(&questLogGroup);
			pBuf->Write<NetGroup*>(&questTypeGroup);
			pBuf->Write<NetGroup*>(&questFlagGroup);
			pBuf->ResetCursor();
			Reuben::Network::DefaultMiddle::UpdateCRC(pBuf);
			LocalRPCManager::GetInstance().OnReceived(gGlobal.pLocalPeer, 
				&gGlobal.pLocalPeer->GetAddressRemote(), pBuf->AddRef());
		}
		else
		{
			String errMsg;

			errMsg.Format(_FE("%s"), dboQuestLog->GetErrorMsg(hr).c_str());
			Buffer* pBuf = SafeCreateObject(Buffer);
			Reuben::Network::RPCHeader header(RPC_ObjectService_CB_Error);
			pBuf->Write<Reuben::Network::RPCHeader>(header);
			pBuf->Write<RPCContext>(pRequest->context);
			pBuf->Write<UInt32>(hr);
			pBuf->Write<String>(errMsg);
			pBuf->ResetCursor();
			Reuben::Network::DefaultMiddle::UpdateCRC(pBuf);
			LocalRPCManager::GetInstance().OnReceived(gGlobal.pLocalPeer, 
				&gGlobal.pLocalPeer->GetAddressRemote(), pBuf->AddRef());
		}

		// delete all temp objects
		for (UInt16 i = 0; i < questLogGroup.GetCount(); ++i)
		{
			pQuestLogData = questLogGroup.GetChild<CQuestLogData>(i);
			SafeDeleteObject(pQuestLogData);
		}

		for (UInt16 i = 0; i < questTypeGroup.GetCount(); ++i)
		{
			pQuestTypeData = questTypeGroup.GetChild<CQuestTypeData>(i);
			SafeDeleteObject(pQuestTypeData);
		}

		for (UInt16 i = 0; i < questFlagGroup.GetCount(); ++i)
		{
			pQuestFlagData = questFlagGroup.GetChild<CQuestFlagData>(i);
			SafeDeleteObject(pQuestFlagData);
		}

		SafeDeleteObject(dboQuestLog);
		SafeDeleteObject(dboQuestType);
		SafeDeleteObject(dboQuestFlag);
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

VOID CQuestLogData::PreSendGroup(NetGroup * questLogGroup)
{
	if (questLogGroup == NULL) return;

	for (UInt16 i = 0; i < questLogGroup->GetCount(); i++) {
		CQuestLogData *pQuestLogData = questLogGroup->GetChild<CQuestLogData>(i);
		if (pQuestLogData)
			pQuestLogData->PreSend();
	}
}

void CQuestLogData::PreSend()
{
	RPCMask mask;

	mask.MaskBits(idMask, infoMask, stateMask, featureMask);

	if (GetBeginNpc() != 0)			mask.MaskBit(beginNpcMask);
	if (GetFinishNpc() != 0)		mask.MaskBit(finishNpcMask);
	if (GetTarget1_id() != 0 || GetTarget1_v1() != 0)	mask.MaskBit(target1Mask);
	if (GetTarget2_id() != 0 || GetTarget2_v1() != 0)	mask.MaskBit(target2Mask);
	if (GetTarget3_id() != 0 || GetTarget3_v1() != 0)	mask.MaskBit(target3Mask);
	if (GetTarget4_id() != 0 || GetTarget4_v1() != 0)	mask.MaskBit(target4Mask);
	if (GetTarget5_id() != 0 || GetTarget5_v1() != 0)	mask.MaskBit(target5Mask);
	if (GetTimeLimit() != 0)		mask.MaskBit(timerMask);
	if (GetBuff() != 0)				mask.MaskBit(buffMask);

	SetSendMask(mask);
}

void CQuestLogData::SetDetail(UInt8 detail)
{
	RPCMask mask;

	switch (detail)
	{
	case DETAIL_NONE:
		break;

	case DETAIL_INFO:
		mask.MaskBits(idMask, infoMask, -1);
		break;

	case DETAIL_ALL:
		mask = CQuestLogData::AllMask;
		break;
	}

	SetSendMask(mask);
}

HRESULT CQuestLogData::UpdateDbo(CdboQuestLog* dboQuestLog)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CQuestLogData UpdateDbo: owner_id %d, ID %d, updateMask %d,%d"), 
		GetChar_id(), GetQuest_id(), m_updateMask.low, m_updateMask.high);

	dboQuestLog->ClearParam();

	if (IsUpdated(infoMask))
	{
		COLUMN_UPDATE(dboQuestLog, type_id, GetType_id());
		COLUMN_UPDATE_STRING(dboQuestLog, name, GetName());
		COLUMN_UPDATE_STRING(dboQuestLog, descript, GetDescript());
	}
	if (IsUpdated(beginNpcMask))
	{
		COLUMN_UPDATE_LARGE(dboQuestLog, beginNpc, GetBeginNpc());
		COLUMN_UPDATE(dboQuestLog, beginNpc_Map, GetBeginNpc_Map());
		COLUMN_UPDATE(dboQuestLog, beginNpc_x, GetBeginNpc_x());
		COLUMN_UPDATE(dboQuestLog, beginNpc_y, GetBeginNpc_y());
	}
	if (IsUpdated(finishNpcMask))
	{
		COLUMN_UPDATE_LARGE(dboQuestLog, finishNpc, GetFinishNpc());
		COLUMN_UPDATE(dboQuestLog, finishNpc_Map, GetFinishNpc_Map());
		COLUMN_UPDATE(dboQuestLog, finishNpc_x, GetFinishNpc_x());
		COLUMN_UPDATE(dboQuestLog, finishNpc_y, GetFinishNpc_y());
	}
	if (IsUpdated(target1Mask))
	{
		COLUMN_UPDATE_LARGE(dboQuestLog, target1_id, GetTarget1_id());
		COLUMN_UPDATE(dboQuestLog, target1_v1, GetTarget1_v1());
		COLUMN_UPDATE(dboQuestLog, target1_v2, GetTarget1_v2());
		COLUMN_UPDATE(dboQuestLog, target1_v3, GetTarget1_v3());
		COLUMN_UPDATE(dboQuestLog, target1_vtype, GetTarget1_vtype());
		COLUMN_UPDATE(dboQuestLog, target1_num, GetTarget1_num());
		COLUMN_UPDATE(dboQuestLog, target1_max, GetTarget1_max());
	}
	if (IsUpdated(target2Mask))
	{
		COLUMN_UPDATE_LARGE(dboQuestLog, target2_id, GetTarget2_id());
		COLUMN_UPDATE(dboQuestLog, target2_v1, GetTarget2_v1());
		COLUMN_UPDATE(dboQuestLog, target2_v2, GetTarget2_v2());
		COLUMN_UPDATE(dboQuestLog, target2_v3, GetTarget2_v3());
		COLUMN_UPDATE(dboQuestLog, target2_vtype, GetTarget2_vtype());
		COLUMN_UPDATE(dboQuestLog, target2_num, GetTarget2_num());
		COLUMN_UPDATE(dboQuestLog, target2_max, GetTarget2_max());
	}
	if (IsUpdated(target3Mask))
	{
		COLUMN_UPDATE_LARGE(dboQuestLog, target3_id, GetTarget3_id());
		COLUMN_UPDATE(dboQuestLog, target3_v1, GetTarget3_v1());
		COLUMN_UPDATE(dboQuestLog, target3_v2, GetTarget3_v2());
		COLUMN_UPDATE(dboQuestLog, target3_v3, GetTarget3_v3());
		COLUMN_UPDATE(dboQuestLog, target3_vtype, GetTarget3_vtype());
		COLUMN_UPDATE(dboQuestLog, target3_num, GetTarget3_num());
		COLUMN_UPDATE(dboQuestLog, target3_max, GetTarget3_max());
	}
	if (IsUpdated(target4Mask))
	{
		COLUMN_UPDATE_LARGE(dboQuestLog, target4_id, GetTarget4_id());
		COLUMN_UPDATE(dboQuestLog, target4_v1, GetTarget4_v1());
		COLUMN_UPDATE(dboQuestLog, target4_v2, GetTarget4_v2());
		COLUMN_UPDATE(dboQuestLog, target4_v3, GetTarget4_v3());
		COLUMN_UPDATE(dboQuestLog, target4_vtype, GetTarget4_vtype());
		COLUMN_UPDATE(dboQuestLog, target4_num, GetTarget4_num());
		COLUMN_UPDATE(dboQuestLog, target4_max, GetTarget4_max());
	}
	if (IsUpdated(target5Mask))
	{
		COLUMN_UPDATE_LARGE(dboQuestLog, target5_id, GetTarget5_id());
		COLUMN_UPDATE(dboQuestLog, target5_v1, GetTarget5_v1());
		COLUMN_UPDATE(dboQuestLog, target5_v2, GetTarget5_v2());
		COLUMN_UPDATE(dboQuestLog, target5_v3, GetTarget5_v3());
		COLUMN_UPDATE(dboQuestLog, target5_vtype, GetTarget5_vtype());
		COLUMN_UPDATE(dboQuestLog, target5_num, GetTarget5_num());
		COLUMN_UPDATE(dboQuestLog, target5_max, GetTarget5_max());
	}
	if (IsUpdated(timerMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetTimeLimit(), dbtime);
		TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("update time limit quest id %d"), GetQuest_id());
		COLUMN_UPDATE(dboQuestLog, timeLimit, dbtime);
		COLUMN_UPDATE(dboQuestLog, interval, GetInterval());
	}
	if (IsUpdated(buffMask))
	{
		COLUMN_UPDATE(dboQuestLog, buff, GetBuff());
	}
	if (IsUpdated(stateMask))
	{
		COLUMN_UPDATE(dboQuestLog, state, GetState());
	}
	if (IsUpdated(featureMask))
	{
		COLUMN_UPDATE(dboQuestLog, feature, GetFeature());
	}

	HRESULT hr;
	if (DB_FAILED(hr = dboQuestLog->UpdateByAny(GetChar_id(), GetQuest_id())))
		dboQuestLog->ShowError(hr);
	else
		ClearUpdateMask();
	dboQuestLog->ClearParam();
	dboQuestLog->Close();

	return hr;
}

HRESULT CQuestLogData::DeleteDbo(CdboQuestLog* dboQuestLog)
{
	HRESULT hr;
	//ThreadGuard guard(m_key);

	if (DB_FAILED(hr = dboQuestLog->Delete(GetChar_id(), GetQuest_id())))
		dboQuestLog->ShowError(hr);
	dboQuestLog->Close();

	return hr;
}

RPCResult ObjectService_Stub::GetQuestList(LPCPEER pPeer, RPCContext &context, 
										   const UInt32 &owner_id)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC1(ObjectService, GetQuestList, NULL, UInt32, owner_id)

		TRACE_ENSURE(pdbQuestLog != NULL);

		HRESULT hr = S_OK;
		NetGroup* questLogGroup			= CQuestLogData::FindQuestLogGroup(owner_id);
		NetGroup* questTypeGroup		= CQuestTypeData::FindQuestTypeGroup(owner_id);
		NetGroup* questFlagGroup		= CQuestFlagData::FindQuestFlagGroup(owner_id);
		CQuestLogData* pQuestLogData	= NULL;
		CQuestTypeData* pQuestTypeData	= NULL;
		CQuestFlagData* pQuestFlagData	= NULL;

		if (questLogGroup && questFlagGroup && questTypeGroup) {
			// directly send back object group
			CQuestLogData::PreSendGroup(questLogGroup);
			CQuestTypeData::PreSendGroup(questTypeGroup);
			CQuestFlagData::PreSendGroup(questFlagGroup);
			CB_GetQuestList(pPeer, context, questLogGroup, questTypeGroup, questFlagGroup);
			PROCEDURE_RETURN
		} else {
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			QuestRequest *pRequest = SafeCreateObject3(QuestRequest, owner_id, pPeer, context);
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetQuests, pRequest, DB_WORKER_FLAG);
#else
			QueueGetQuests(pRequest);
#endif
		}

	PROCEDURE_WAIT3(1, ObjectService, CB_QueueQuests, NetGroup*, questLogGroup, 
					NetGroup*, questTypeGroup, NetGroup*, questFlagGroup)

		TRACE_ENSURE(questLogGroup && questTypeGroup && questFlagGroup);

		// store new quest groups
		CQuestLogData* pQuestLogData;
		DataObject& dobj1 = gGlobal.questLogGroupMap[owner_id];
		NetGroup* newGroup = NULL;
		if (dobj1.pobj) {
			newGroup = (NetGroup*) dobj1.pobj;
		} else {
			newGroup = SafeCreateObject(NetGroup);
			dobj1.pobj = newGroup;
		}
		for (UInt16 i = 0; i < questLogGroup->GetCount(); i++)
		{
			pQuestLogData = questLogGroup->GetChild<CQuestLogData>(i);
			if (pQuestLogData) {
				// need to register for new questLog data
				QuestLogDataKey key(owner_id, pQuestLogData->GetQuest_id());
				DataObject &dobj = gGlobal.questLogMap[key];
				if (dobj.pobj) {
					DeleteNetObject(pQuestLogData);
					pQuestLogData = CastNetObject<CQuestLogData>(dobj.pobj);
				} else {
					LocalRPCManager::GetInstance().Register(pQuestLogData);
					pQuestLogData->ClearUpdateMask();
					dobj.pobj = pQuestLogData;
				}
				newGroup->Add(pQuestLogData);
			}
		}
		NetGroup* newLogGrp = newGroup;

		CQuestTypeData* pQuestTypeData;
		DataObject& dobj2 = gGlobal.questTypeGroupMap[owner_id];
		if (dobj2.pobj) {
			newGroup = (NetGroup*) dobj2.pobj;
		} else {
			newGroup = SafeCreateObject(NetGroup);
			dobj2.pobj = newGroup;
		}
		for (UInt16 i = 0; i < questTypeGroup->GetCount(); i++)
		{
			pQuestTypeData = questTypeGroup->GetChild<CQuestTypeData>(i);
			if (pQuestTypeData) {
				// need to register for new questType data
				QuestTypeDataKey key(owner_id, pQuestTypeData->GetType_id());
				DataObject &dobj = gGlobal.questTypeMap[key];
				gGlobal.AddUnreleasedTypeId(key.owner_id, key.type_id);
				if (dobj.pobj) {
					DeleteNetObject(pQuestTypeData);
					pQuestTypeData = CastNetObject<CQuestTypeData>(dobj.pobj);
				} else {
					LocalRPCManager::GetInstance().Register(pQuestTypeData);
					pQuestTypeData->ClearUpdateMask();
					dobj.pobj = pQuestTypeData;
				}
				newGroup->Add(pQuestTypeData);
			}
		}
		NetGroup* newTypeGrp = newGroup;

		CQuestFlagData* pQuestFlagData;
		DataObject& dobj3 = gGlobal.questFlagGroupMap[owner_id];
		if (dobj3.pobj) {
			newGroup = (NetGroup*) dobj3.pobj;
		} else {
			newGroup = SafeCreateObject(NetGroup);
			dobj3.pobj = newGroup;
		}
		for (UInt16 i = 0; i < questFlagGroup->GetCount(); i++)
		{
			pQuestFlagData = questFlagGroup->GetChild<CQuestFlagData>(i);
			if (pQuestFlagData) {
				// need to register for new questFlag data
				QuestFlagDataKey key(owner_id, pQuestFlagData->GetFlag_id());
				DataObject &dobj = gGlobal.questFlagMap[key];
				gGlobal.AddUnreleasedFlagId(key.owner_id, key.flag_id);
				if (dobj.pobj) {
					DeleteNetObject(pQuestFlagData);
					pQuestFlagData = CastNetObject<CQuestFlagData>(dobj.pobj);
				} else {
					LocalRPCManager::GetInstance().Register(pQuestFlagData);
					pQuestFlagData->ClearUpdateMask();
					dobj.pobj = pQuestFlagData;
				}
				newGroup->Add(pQuestFlagData);
			}
		}
		NetGroup* newFlagGrp = newGroup;

		// send quest data to peer
		CQuestLogData::PreSendGroup(newLogGrp);
		CQuestTypeData::PreSendGroup(newTypeGrp);
		CQuestFlagData::PreSendGroup(newFlagGrp);
		CB_GetQuestList(pPeer, context, newLogGrp, newTypeGrp, newFlagGrp);

		DeleteNetGroup(questLogGroup);
		DeleteNetGroup(questTypeGroup);
		DeleteNetGroup(questFlagGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_T("GetQuestList Error: sessionId %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateQuestLogs(LPCPEER pPeer, RPCContext &context, 
											  const UInt32 &owner_id, NetGroup* grp)
{
	TRACE_ENSURE(pdbQuestLog && grp);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("UpdateQuestLogs: owner_id %d, group count %d"), 
		owner_id, grp->GetCount());

	NetGroup* logGroup = CQuestLogData::FindQuestLogGroup(owner_id);
	if (logGroup == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CQuestLogData UpdateQuestLogs: quest log group not found, owner_id %d"), owner_id);
		CB_Error(pPeer, context, 1, _T("UpdateQuestLogs: quest log group not found"));
		return RPC_RESULT_FAIL;
	}

	QuestLogDataMap::iterator it;
	QuestLogDataKey key;
	CQuestLogData *pNewData, *pCurData;
	UInt32Vector del_ids;

	key.owner_id = owner_id;
	for (Index i = C_INDEX(0); i < grp->GetCount(); ++i)
	{
		pNewData = grp->GetChild<CQuestLogData>(i);
		if (pNewData == NULL)
			continue;
		key.quest_id = pNewData->GetQuest_id();
		it = gGlobal.questLogMap.find(key);
		if (it == gGlobal.questLogMap.end())
		{
			// new quest log if not exist
			CreateQuestLog(pNewData);
		}
		else if (pNewData->GetStatus() == CQuestLogData::ST_DELETED)
		{
			// delete if marked
			del_ids.push_back(pNewData->GetQuest_id());
		}
		else
		{
			// update quest log if exist
			pCurData = CastNetObject<CQuestLogData>(it->second.pobj);
			if (pCurData)
			{
				pCurData->Clone(pNewData, pNewData->GetUpdateMask());
				pCurData->UnmarkUpdate(CQuestLogData::statusMask);
				if (pCurData->IsUpdated())
					gGlobal.EnqueueUpdate(&it->second);
			}
		}
	}
	DeleteNetGroup(grp, BOOLEAN_TRUE);

	// delete quest log after update
	for (UInt32Vector::iterator it2 = del_ids.begin(); it2 != del_ids.end(); ++it2)
	{
		RemoveQuestLog(owner_id, *it2);
	}

	CB_UpdateQuestLogs(pPeer, context);

	return RPC_RESULT_OK;
}