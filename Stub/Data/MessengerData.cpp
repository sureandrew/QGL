//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/MessengerData.h"
//-- Library
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboMessenger.h"
#include "ObjectService/Global.h"
#include "ObjectService/DboSourcePool.h"

REGISTER_CLASSTYPE(CMessengerData, MessengerData_CLASSID);

CMessengerData* NewMessenger();
CdboMessenger* &pdbMessenger = gGlobal.gTables.pMessenger;
StringVector CdboMessengerAccessor::m_params;
ThreadKey CMessengerData::m_key;

NetGroup* FindMessengerGroup(UInt32 owner_id);
void PreSendMessengersGroup(NetGroup * messengerGroup);

struct MsgRequest : public WorkRequest<UInt32>
{
	bool			bCreate;
	UInt32			target_id;
	UInt16			relations;
	UInt8			group;
	UInt16			friendly;

	MsgRequest(UInt32 id, LPCPEER pPeer, RPCContext &context)
		: WorkRequest<UInt32>(id, pPeer, context)
		, bCreate(false)
	{}
};

CMessengerData::CMessengerData()
{
}

CMessengerData::~CMessengerData()
{
}
CMessengerData* NewMessenger()
{
	CMessengerData *pMessengerData = NULL;
	UInt32 owner_id = (UInt32)pdbMessenger->m_player_char_id.LowPart;
	MessengerDataKey MessengerKey((UInt32)pdbMessenger->m_player_char_id.LowPart, 
		(UInt32)pdbMessenger->m_friend_char_id.LowPart);
	DataObject &dobj = gGlobal.messengerDataMap[MessengerKey];

	// object already exist
	if (dobj.pobj)
		pMessengerData = CastNetObject<CMessengerData>(dobj.pobj);

	if (pMessengerData == NULL)
	{
		pMessengerData = CMessengerData::NewInstance();
		pMessengerData->SetData(pdbMessenger);			// assign values to messenger data
		dobj.pobj = pMessengerData;
	}

	// found existing messenger group in cache
	NetGroup* messengerGroup = FindMessengerGroup(owner_id);
	if (messengerGroup)
	{
		messengerGroup->Add(pMessengerData);
	}
	else
	{
		DataObject& dobj3 = gGlobal.messengerGroupMap[owner_id];
		if (dobj3.pobj == NULL) {
			messengerGroup = SafeCreateObject(NetGroup);
			dobj3.pobj = messengerGroup;
		} else {
			messengerGroup = (NetGroup*) dobj3.pobj;
		}
		messengerGroup->Add(pMessengerData);
	}

	return pMessengerData;
}

void CMessengerData::SetData(CdboMessenger* dboMessenger)
{
	SetKey((UInt32)dboMessenger->m_player_char_id.LowPart, (UInt32)dboMessenger->m_friend_char_id.LowPart);
	SetRelation((UInt16) dboMessenger->m_friend_relation);
	SetGroup((UInt8)dboMessenger->m_friend_group);
	SetFriendly((UInt16)dboMessenger->m_friend_friendly);
	
	ClearUpdateMask();
}

CMessengerData* FindMessenger(MessengerDataKey &key)
{
	MessengerDataMap::iterator iter = gGlobal.messengerDataMap.find(key);
	CMessengerData* pMessengerData = NULL;

	// found existing account in cache
	if (iter != gGlobal.messengerDataMap.end())
		pMessengerData = CastNetObject<CMessengerData>(iter->second.pobj);

	return pMessengerData;
}

NetGroup* FindMessengerGroup(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.messengerGroupMap.find(char_id);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.messengerGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}

DWORD WINAPI QueueGetMessengers(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();

	MsgRequest* pRequest = (MsgRequest*) lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
	{
		NetGroup msgGroup;
		CMessengerData* pMessengerData;
		CdboMessenger* dboMessenger = SafeCreateObject(CdboMessenger);

		msgGroup.SetValidate(BOOLEAN_FALSE);
		dboMessenger->SetDataSource(dbSrc);
		if (pRequest->bCreate)
		{
			// create messenger
			if (DB_FAILED(hr = dboMessenger->RSByFID(pRequest->key, pRequest->target_id)) ||
				DB_FAILED(hr = dboMessenger->MoveFirst()))
			{
				dboMessenger->Close();
				hr = dboMessenger->Insert(pRequest->key, pRequest->target_id, 
					pRequest->relations, pRequest->group, pRequest->friendly);
				if (DB_SUCCEEDED(hr))
				{
					pMessengerData = SafeCreateObject(CMessengerData);
					pMessengerData->SetData(dboMessenger);
					pMessengerData->PreSend();
					msgGroup.Add(pMessengerData);
				}
			}
		}
		else
		{
			if (DB_SUCCEEDED(hr = dboMessenger->RSByID(pRequest->key)))
			{
				hr = dboMessenger->MoveFirst();
				while (hr == S_OK)
				{
					pMessengerData = SafeCreateObject(CMessengerData);
					pMessengerData->SetData(dboMessenger);
					pMessengerData->PreSend();
					msgGroup.Add(pMessengerData);
					hr = dboMessenger->MoveNext();
				}
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetMessengers"), hr, dboMessenger->GetErrorMsg(hr), 
			pRequest->context, &msgGroup);
		dboMessenger->Close();
		SafeDeleteObject(dboMessenger);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, char_id %d"),
			pRequest->context.sessionId.GetString().c_str(), pRequest->key);
	}
	// show query time
	UInt32 dt = ::GetTickCount() - curTime;
	if (dt > 1000)
	{
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("profile, utime %d, char_id %d, target_id %d"), 
			dt, pRequest->key, pRequest->target_id);
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

HRESULT CMessengerData::UpdateDbo(CdboMessenger* dboMessenger)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("player_id %d, friend_id %d, update_mask %d,%d"), 
		GetPlayer_char_id(), GetFriend_char_id(), m_updateMask.low, m_updateMask.high);

	dboMessenger->ClearParam();

	if (IsUpdated(keyMask))
	{
		COLUMN_UPDATE_LARGE_QUA(dboMessenger, player_char_id, GetPlayer_char_id());
		COLUMN_UPDATE_LARGE_QUA(dboMessenger, friend_char_id, GetFriend_char_id());
	}

	if (IsUpdated(relationMask))
	{
		COLUMN_UPDATE(dboMessenger, friend_relation, GetFriend_relation());
	}

	if (IsUpdated(groupMask))
	{
		COLUMN_UPDATE(dboMessenger, friend_group, GetFriend_group());
	}

	if (IsUpdated(friendlyMask))
	{
		COLUMN_UPDATE(dboMessenger, friend_friendly, GetFriend_friendly());
	}

	HRESULT hr;
	if (DB_FAILED(hr = dboMessenger->UpdateByAny(GetPlayer_char_id(), GetFriend_char_id())))
		dboMessenger->ShowError(hr);
	else
		ClearUpdateMask();
	dboMessenger->ClearParam();
	dboMessenger->Close();

	return hr;
}

HRESULT CMessengerData::DeleteDbo(CdboMessenger* dboMessenger)
{
	HRESULT hr;

	if (DB_FAILED(hr = dboMessenger->Delete(GetPlayer_char_id(), GetFriend_char_id())))
		dboMessenger->ShowError(hr);
	dboMessenger->Close();

	return hr;
}

HRESULT RemoveMessenger(UInt32 char_id, UInt32 friend_char_id)
{
	MessengerDataKey MessengerKey(char_id, friend_char_id);
	MessengerDataMap::iterator iter = gGlobal.messengerDataMap.find(MessengerKey);
	CMessengerData* pMessengerData;
	NetGroup* messengerGroup;

	if (iter != gGlobal.messengerDataMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		pMessengerData = CastNetObject<CMessengerData>(iter->second.pobj);
		if (pMessengerData)
		{
			messengerGroup = FindMessengerGroup(char_id);
			if (messengerGroup)
				messengerGroup->Remove(pMessengerData);
			// directly remove coz of primary constrain
			//gGlobal.m_deleteGroup.Add(pMessengerData);
			pMessengerData->DeleteDbo(pdbMessenger);
			DeleteNetObject(pMessengerData);
		}
		gGlobal.messengerDataMap.erase(iter);
	}

	return S_OK;
}

HRESULT RemoveMessengers(UInt32 char_id)
{
	NetGroup* messengerGroup = NULL;
	GroupMap::iterator it = gGlobal.messengerGroupMap.find(char_id);

	if (it != gGlobal.messengerGroupMap.end())
	{
		CMessengerData* pMessengerData;
		MessengerDataMap::iterator it2;

		messengerGroup = CastNetObject<NetGroup>(it->second.pobj);
		for (UInt16 i = 0; i < messengerGroup->GetCount(); i++)
		{
			pMessengerData = messengerGroup->GetChild<CMessengerData>(i);
			if (pMessengerData)
			{
				MessengerDataKey key(pMessengerData->GetPlayer_char_id(), pMessengerData->GetFriend_char_id());
				it2 = gGlobal.messengerDataMap.find(key);
				if (it2 != gGlobal.messengerDataMap.end())
				{
					gGlobal.DequeueUpdate(&it2->second);
					gGlobal.messengerDataMap.erase(it2);
				}
				// directly remove coz of primary constrain
				//gGlobal.m_deleteGroup.Add(pMessengerData);
				DeleteNetObject(pMessengerData);
			}
		}
		SafeDeleteObject(messengerGroup);
		gGlobal.messengerGroupMap.erase(it);
	}

	// delete all messengers by char_id
	HRESULT hr = pdbMessenger->DeleteByOwner(char_id);
	pdbMessenger->Close();

	return hr;
}

void ReleaseMessengers(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.messengerGroupMap.find(char_id);

	if (iter != gGlobal.messengerGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		CMessengerData *pMessengerData;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++)
		{
			pMessengerData = grp->GetChild<CMessengerData>(i);
			if (pMessengerData)
			{
				MessengerDataMap::iterator it = gGlobal.messengerDataMap.find(
					MessengerDataKey(pMessengerData->GetPlayer_char_id(), pMessengerData->GetFriend_char_id()));
				if (it != gGlobal.messengerDataMap.end())
				{
					// remove from update queue
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.messengerDataMap.erase(it);
				}
				if (pMessengerData->IsUpdated())				// write to db if updated
					gGlobal.m_releaseGroup.Add(pMessengerData);
				else
					DeleteNetObject(pMessengerData);
			}
		}
		SafeDeleteObject(grp);
		gGlobal.messengerGroupMap.erase(iter);
	}
}

void CMessengerData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

void PreSendMessengersGroup(NetGroup * messengerGroup)
{
	if (messengerGroup == NULL) return;

	for (UInt16 i = 0; i < messengerGroup->GetCount(); i++) {
		CMessengerData *pMessengerData = messengerGroup->GetChild<CMessengerData>(i);
		if (pMessengerData)
			pMessengerData->PreSend();
		else
			messengerGroup->Replace(i, NULL);
	}
}

RPCResult ObjectService_Stub::GetMessengerLst(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC1(ObjectService, GetMessengerLst, NULL, UInt32, char_id)

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("session_id %s, char_id %d"), 
			context.sessionId.GetString().c_str(), char_id);
		PROCEDURE_STORE_VALUE1(UInt32, ::GetTickCount())

		HRESULT hr = S_OK;
		CMessengerData *pMessengerData = NULL;
		NetGroup* messengerGroup = FindMessengerGroup(char_id);

		if (messengerGroup)
		{
			// directly send back object group
			PreSendMessengersGroup(messengerGroup);
			CB_GetMessengerLst(pPeer, context, messengerGroup);
			PROCEDURE_RETURN
		}
		else
		{
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			MsgRequest *pRequest = SafeCreateObject3(MsgRequest, char_id, pPeer, context);
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetMessengers, pRequest, DB_WORKER_FLAG);
#else
			QueueGetMessengers(pRequest);
#endif
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, messengerGroup)

		TRACE_ENSURE(messengerGroup);

		// store new messenger group
		CMessengerData* pMessengerData;
		NetGroup* newGroup = FindMessengerGroup(char_id);
	
		if (newGroup == NULL)
		{
			DataObject& dobj3 = gGlobal.messengerGroupMap[char_id];
			if (dobj3.pobj)
			{
				newGroup = (NetGroup*) dobj3.pobj;
			}
			else
			{
				newGroup = SafeCreateObject(NetGroup);
				dobj3.pobj = newGroup;
			}
			for (UInt16 i = 0; i < messengerGroup->GetCount(); i++)
			{
				pMessengerData = messengerGroup->GetChild<CMessengerData>(i);
				if (pMessengerData)
				{
					MessengerDataKey key(pMessengerData->GetPlayer_char_id(), 
						pMessengerData->GetFriend_char_id());
					DataObject &dobj = gGlobal.messengerDataMap[key];
					if (dobj.pobj)
					{
						// use old messenger data
						DeleteNetObject(pMessengerData);
						pMessengerData = CastNetObject<CMessengerData>(dobj.pobj);
					}
					else
					{
						// need to register for new messenger data
						LocalRPCManager::GetInstance().Register(pMessengerData);
						pMessengerData->ClearUpdateMask();
						dobj.pobj = pMessengerData;
					}
					newGroup->Add(pMessengerData);
				}
			}
		}
		else
		{
			DeleteNetChilds(messengerGroup);
		}

		// send messenger data to peer
		PreSendMessengersGroup(newGroup);
		CB_GetMessengerLst(pPeer, context, newGroup);
		DeleteNetGroup(messengerGroup);

		PROCEDURE_RESTORE_VALUE1(UInt32, lastTime)
		UInt32 dt = ::GetTickCount() - lastTime;
		if (dt >= 1000)
		{
			TRACE_WARNDTL_3(GLOBAL_LOGGER, 
				_FW("profile, utime %d, session_id %s, char_id %d"), 
				dt, context.sessionId.GetString().c_str(), char_id);
		}

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::CreateMessenger(LPCPEER pPeer, RPCContext &context, 
											  const UInt32 &owner_id, const UInt32 &target_id,
											  const UInt16 &relations, const UInt8 &group,
											  const UInt16 &friendly)
{
	PROCEDURE_START_RPC5(ObjectService, CreateMessenger, NULL, UInt32, owner_id, 
		UInt32, target_id, UInt16, relations, UInt8, group, UInt16, friendly)

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("owner_id %d, target_id %d"), 
			owner_id, target_id);
		if (group == FRIEND_GROUP_TEMP)
		{
			CB_Error(pPeer, context, 1, 
				_FE("DB_FAILED group FRIEND_GROUP_TEMP"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CMessengerData * pData = FindMessenger(MessengerDataKey(owner_id, target_id));
		if (pData)
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("found, owner_id %d, target_id %d"), 
			owner_id, target_id);
			pData->PreSend();
			CB_CreateMessenger(pPeer, context, pData);
			PROCEDURE_RETURN
		}
		else
		{
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			MsgRequest *pRequest = SafeCreateObject3(MsgRequest, owner_id, pPeer, context);
			pRequest->bCreate = true;
			pRequest->target_id = target_id;
			pRequest->relations = relations;
			pRequest->group = group;
			pRequest->friendly = friendly;
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetMessengers, pRequest, DB_WORKER_FLAG);
#else
			QueueGetMessengers(pRequest);
#endif
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, messengerGroup)

		TRACE_ENSURE(messengerGroup);

		// store new messenger group
		CMessengerData* pMessengerData = FindMessenger(MessengerDataKey(owner_id, target_id));
		if (pMessengerData)
		{
			DeleteNetChilds(messengerGroup);
		}
		else
		{
			if (messengerGroup->GetCount() > 0)
			{
				pMessengerData = messengerGroup->GetChild<CMessengerData>(0);
				if (pMessengerData)
				{
					// need to register for new messenger data
					MessengerDataKey key(pMessengerData->GetPlayer_char_id(), 
						pMessengerData->GetFriend_char_id());
					DataObject &dobj = gGlobal.messengerDataMap[key];
					if (dobj.pobj)
					{
						DeleteNetObject(pMessengerData);
						pMessengerData = CastNetObject<CMessengerData>(dobj.pobj);
					}
					else
					{
						LocalRPCManager::GetInstance().Register(pMessengerData);
						pMessengerData->ClearUpdateMask();
						dobj.pobj = pMessengerData;
					}
				}
			}
		}

		// send messenger data to peer
		if (pMessengerData)
		{
			pMessengerData->PreSend();
			CB_CreateMessenger(pPeer, context, pMessengerData);

			//add back to group
			NetGroup* group = NULL;
			DataObject& dobj3 = gGlobal.messengerGroupMap[owner_id];
			if (dobj3.pobj == NULL) {
				group = SafeCreateObject(NetGroup);
				dobj3.pobj = group;
			} else {
				group = (NetGroup*) dobj3.pobj;
			}
			group->Add(pMessengerData);
		}
		else
		{
			CB_Error(pPeer, context, 1, _FE(""));
		}
		DeleteNetGroup(messengerGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::DeleteMessenger(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id)
{
	HRESULT hr = S_OK;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("owner_id %d, target_id %d"), 
		owner_id, target_id);

	if (DB_FAILED(hr = RemoveMessenger(owner_id, target_id)))
	{
		pdbMessenger->ShowError(hr);
		return CB_Error(pPeer, context, hr, _FE("DB_FAILED"));
	}

	CB_DeleteMessenger(pPeer, context);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::DeleteMessengerList(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id)
{
	HRESULT hr = S_OK;
	
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("owner_id %d"), owner_id);

	if (DB_FAILED(hr = RemoveMessengers(owner_id)))
	{
		pdbMessenger->ShowError(hr);
		return CB_Error(pPeer, context, hr, _FE("DB_FAILED"));
	}

	CB_DeleteMessengerList(pPeer, context);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateMessenger(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, NetObject* relation)
{
	CMessengerData *pNewData = CastNetObject<CMessengerData>(relation);
	TRACE_ENSURE(pdbMessenger && pNewData);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("owner_id %d, target_id %d"), 
		owner_id, target_id);

	MessengerDataKey MessengerKey(owner_id, target_id);
	CMessengerData* pMessengerData = FindMessenger(MessengerKey);

	//only do update in this function
	if (pMessengerData != NULL)
	{		
		MessengerDataMap::iterator iter = gGlobal.messengerDataMap.find(MessengerKey);

		pMessengerData->Clone(pNewData, pNewData->GetUpdateMask());
		if (iter != gGlobal.messengerDataMap.end())
			gGlobal.EnqueueUpdate(&iter->second);
		
	}
	DeleteNetObject(pNewData);

	CB_UpdateMessenger(pPeer, context);
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Stub::RemoveRelation(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const UInt32 &relation)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ObjectService_Stub::RemoveRelation owner_id %d, target_id %d, relation %d"), owner_id, target_id, relation);
	MessengerDataKey MessengerKey(owner_id, target_id);
	CMessengerData* pMessengerData = FindMessenger(MessengerKey);

	if (pMessengerData != NULL)
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ObjectService_Stub::RemoveRelation owner_id %d, target_id %d, pre relation %d"), owner_id, target_id, pMessengerData->GetFriend_relation());
		pMessengerData->SetFriend_relation(pMessengerData->GetFriend_relation()&(~(1<<relation)));
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ObjectService_Stub::RemoveRelation owner_id %d, target_id %d, modfied relation %d"), owner_id, target_id, pMessengerData->GetFriend_relation());

		MessengerDataMap::iterator iter = gGlobal.messengerDataMap.find(MessengerKey);
		//pMessengerData->Clone(pNewData, pNewData->GetUpdateMask());
		if (iter != gGlobal.messengerDataMap.end())
			gGlobal.EnqueueUpdate(&iter->second);
	}
	else
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ObjectService_Stub::RemoveRelation offline owner_id %d, target_id %d, relation %d"), owner_id, target_id, relation);
		pdbMessenger->RemoveRelation(owner_id,target_id,relation);
		pdbMessenger->Close();
	}
	return RPC_RESULT_OK;
}