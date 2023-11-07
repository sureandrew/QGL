//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/GuildMemberData.h"
//-- Library
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboGuildMember.h"
#include "ObjectService/Global.h"
#include "ObjectService/DboSourcePool.h"

REGISTER_CLASSTYPE(CGuildMemberData, GuildMemberData_CLASSID);

CGuildMemberData* NewGuildMember();
CdboGuildMember* &pdbGuildMember = gGlobal.gTables.pGuildMember;
StringVector CdboGuildMemberAccessor::m_params;
ThreadKey CGuildMemberData::m_key;
NetGroup* FindGuildMemberGroup(UInt32 guild_id);

struct GuildMemberRequest : public WorkRequest<UInt32>
{
	UInt32			char_id;
	GuildMemberRequest(UInt32 id, LPCPEER pPeer, RPCContext &context)
		: WorkRequest<UInt32>(id, pPeer, context), char_id(0)
	{}
};


CGuildMemberData* NewGuildMember()
{
	CGuildMemberData *pGuildMemberData = NULL;
	UInt32 guild_uid = (UInt32) pdbGuildMember->m_guild_uid.LowPart;
	UInt32 player_char_id = (UInt32) pdbGuildMember->m_char_id.LowPart;
	DataObject &dobj = gGlobal.guildMemberMap[player_char_id];

	if (dobj.pobj == NULL)
	{
		pGuildMemberData = CGuildMemberData::NewInstance();
		pGuildMemberData->SetData(pdbGuildMember); // assign values to character data
		dobj.pobj = pGuildMemberData;
	}
	else
	{
		pGuildMemberData = CastNetObject<CGuildMemberData>(dobj.pobj);
	}

	NetGroup* guildMemGroup = FindGuildMemberGroup(guild_uid);
	if (guildMemGroup)
	{
		guildMemGroup->Add(pGuildMemberData);
	}
	else
	{
		DataObject& dobj3 = gGlobal.guildMemberGrpMap[guild_uid];
		if (dobj3.pobj == NULL)
		{
			guildMemGroup = SafeCreateObject(NetGroup);
			dobj3.pobj = guildMemGroup;
		}
		else
		{
			guildMemGroup = (NetGroup*) dobj3.pobj;
		}
		guildMemGroup->Add(pGuildMemberData);
	}
	return pGuildMemberData;
}

NetGroup* FindGuildMemberGroup(UInt32 guild_id)
{
	GroupMap::iterator iter = gGlobal.guildMemberGrpMap.find(guild_id);
	NetGroup* grp = NULL;

	// found existing guild member group in cache
	if (iter != gGlobal.guildMemberGrpMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}

CGuildMemberData* FindGuildMember(UInt32 char_id)
{
	DataIdMap::iterator iter = gGlobal.guildMemberMap.find(char_id);
	CGuildMemberData* pMemberData = NULL;

	// found existing guild member in cache
	if (iter != gGlobal.guildMemberMap.end())
		pMemberData = CastNetObject<CGuildMemberData>(iter->second.pobj);

	return pMemberData;
}

void PreSendMemberGrp(NetGroup * grp)
{
	if (grp == NULL) return;

	for (UInt16 i = 0; i < grp->GetCount(); ++i)
	{
		CGuildMemberData *pGuildMemberData = grp->GetChild<CGuildMemberData>(i);
		if (pGuildMemberData)
			pGuildMemberData->PreSend();
		else
			grp->Replace(i, NULL);
	}
}

HRESULT RemoveGuildMember(UInt32 player_char_id)
{
	DataIdMap::iterator iter = gGlobal.guildMemberMap.find(player_char_id);
	if (iter != gGlobal.guildMemberMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CGuildMemberData* pGuildMemberData = CastNetObject<CGuildMemberData>(iter->second.pobj);
		if (pGuildMemberData)
		{
			NetGroup* guildMemGroup = FindGuildMemberGroup(pGuildMemberData->GetGuild_uid());
			if (guildMemGroup) 
				guildMemGroup->Remove(pGuildMemberData);
			// directly remove coz of primary constrain
			//gGlobal.m_deleteGroup.Add(pGuildMemberData);
			pGuildMemberData->DeleteDbo(pdbGuildMember);
			DeleteNetObject(pGuildMemberData);
		}
		gGlobal.guildMemberMap.erase(iter);
	}

	return S_OK;
}

UInt32 IdentifyGuild(UInt32 char_id)
{
	HRESULT hr = pdbGuildMember->GetGuildUid(char_id);
	UInt32 guild_uid = 0;
	if (DB_SUCCEEDED(hr))
	{
		hr = pdbGuildMember->MoveFirst();
		if (DB_SUCCEEDED(hr))
			guild_uid = (UInt32) pdbGuildMember->m_guild_uid.LowPart;
	}
	if (hr != S_OK && hr != DB_S_ENDOFROWSET)
		pdbGuildMember->ShowError(hr);
	pdbGuildMember->Close();

	return guild_uid;
}

HRESULT RemoveGuildMembers(UInt32 guild_id)
{
	NetGroup* guildMemberGroup = NULL;
	GroupMap::iterator it = gGlobal.guildMemberGrpMap.find(guild_id);

	if (it != gGlobal.guildMemberGrpMap.end())
	{
		CGuildMemberData* pGuildMemberData;
		DataIdMap::iterator it2;

		guildMemberGroup = CastNetObject<NetGroup>(it->second.pobj);
		for (UInt16 i = 0; i < guildMemberGroup->GetCount(); i++)
		{
			pGuildMemberData = guildMemberGroup->GetChild<CGuildMemberData>(i);
			if (pGuildMemberData)
			{
				it2 = gGlobal.guildMemberMap.find(pGuildMemberData->GetChar_id());
				if (it2 != gGlobal.guildMemberMap.end())
				{
					gGlobal.DequeueUpdate(&it2->second);
					gGlobal.guildMemberMap.erase(it2);
				}
				// directly remove coz of primary constrain
				//gGlobal.m_deleteGroup.Add(pGuildMemberData);
				DeleteNetObject(pGuildMemberData);
			}
		}
		SafeDeleteObject(guildMemberGroup);
		gGlobal.guildMemberGrpMap.erase(it);
	}

	// delete all guild members by guild_id
	HRESULT hr = pdbGuildMember->DeleteByGuild(guild_id);
	pdbGuildMember->Close();

	return hr;
}

HRESULT ReleaseGuildMembers(UInt32 guild_id)
{
	NetGroup* guildMemberGroup = NULL;
	GroupMap::iterator it = gGlobal.guildMemberGrpMap.find(guild_id);

	if (it != gGlobal.guildMemberGrpMap.end())
	{
		CGuildMemberData* pGuildMemberData;
		DataIdMap::iterator it2;

		guildMemberGroup = CastNetObject<NetGroup>(it->second.pobj);
		for (UInt16 i = 0; guildMemberGroup && i < guildMemberGroup->GetCount(); i++)
		{
			pGuildMemberData = guildMemberGroup->GetChild<CGuildMemberData>(i);
			if (pGuildMemberData)
			{
				it2 = gGlobal.guildMemberMap.find(pGuildMemberData->GetChar_id());
				if (it2 != gGlobal.guildMemberMap.end())
				{
					gGlobal.DequeueUpdate(&it2->second);
					gGlobal.guildMemberMap.erase(it2);
				}
				if (pGuildMemberData->IsUpdated())				// write to db if updated
					gGlobal.m_releaseGroup.Add(pGuildMemberData);
				else
					DeleteNetObject(pGuildMemberData);
			}
		}
		SafeDeleteObject(guildMemberGroup);
		gGlobal.guildMemberGrpMap.erase(it);
	}

	return S_OK;
}


HRESULT DoCreateGuildMember(const UInt32 &player_char_id, const UInt32 &guild_id, const UInt8 &guildPost, CGuildMemberData* &pGuildMemberData)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("player_id %d, guild_id %d, guild_post %d"),
		player_char_id,	guild_id, guildPost);

	HRESULT hr = S_OK;
	pGuildMemberData = NULL;
	
	time_t now = time(NULL); 
	DBTIMESTAMP dbtime;
	TimeConvert((time_t) now, dbtime);

	hr = pdbGuildMember->Insert(player_char_id, guild_id, guildPost, dbtime);

	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbGuildMember->MoveFirst()))
		pGuildMemberData = NewGuildMember();
	else
		pdbGuildMember->ShowError(hr);
	pdbGuildMember->Close();

	return hr;
}

DWORD WINAPI QueueGetGuildMember(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();

	GuildMemberRequest* pRequest = (GuildMemberRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource(); 
	if (dbSrc)
	{
		NetGroup memberGroup;
		CGuildMemberData* pGuildMemberData;
		CdboGuildMember* dboGuildMember = SafeCreateObject(CdboGuildMember);

		memberGroup.SetValidate(BOOLEAN_FALSE);
		dboGuildMember->SetDataSource(dbSrc);
	
		if (pRequest->char_id != 0)
			hr = dboGuildMember->RSByCharID(pRequest->char_id);
		else
			hr = dboGuildMember->RSByID(pRequest->key);
		
		if (DB_SUCCEEDED(hr))
		{
			hr = dboGuildMember->MoveFirst();
			while (hr == S_OK)
			{
				pGuildMemberData = SafeCreateObject(CGuildMemberData);
				pGuildMemberData->SetData(dboGuildMember);
				pGuildMemberData->SetSendMask(CGuildMemberData::AllMask);
				memberGroup.Add(pGuildMemberData);
				hr = dboGuildMember->MoveNext();
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetGuildMembers"), hr, dboGuildMember->GetErrorMsg(hr), 
			pRequest->context, &memberGroup);
		dboGuildMember->Close();
		SafeDeleteObject(dboGuildMember);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, key %d"),
			pRequest->context.sessionId.GetString().c_str(), pRequest->key);
	}
	// show query time
	UInt32 dt = ::GetTickCount() - curTime;
	if (dt > 1000)
	{
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("profile, utime %d, char_id %d, guild_uid %d"), 
			dt, pRequest->char_id, pRequest->key);
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

RPCResult ObjectService_Stub::GetGuildMember(
	LPCPEER pPeer, RPCContext &context, 
	const UInt32 &guild_uid, const UInt32 &char_id)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC2(ObjectService, GetGuildMember, NULL, UInt32, guild_uid, 
						UInt32, char_id)

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("char_id %d"), char_id);

		HRESULT hr = S_OK;
		bool found = false;

		if (guild_uid != 0)
		{
			NetGroup* memberGrp = FindGuildMemberGroup(guild_uid);
			if (memberGrp)
			{
				PreSendMemberGrp(memberGrp);
				CB_GetGuildMembers(pPeer, context, memberGrp);
				found = true;	
			}
		}
		else
		{
			CGuildMemberData* pMemberData = FindGuildMember(char_id);
			if (pMemberData)
			{
				NetGroup memberGrp;
				pMemberData->PreSend();
				memberGrp.Add(pMemberData);
				CB_GetGuildMembers(pPeer, context, &memberGrp);
				found = true;	
			}
		}

		if (found)
		{
			PROCEDURE_RETURN
		}
		else
		{
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			GuildMemberRequest *pRequest = SafeCreateObject3(GuildMemberRequest, guild_uid,
											pPeer, context);
			pRequest->char_id = char_id;
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetGuildMember, pRequest, DB_WORKER_FLAG);
#else
			QueueGetGuildMember(pRequest);
#endif
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, grp)
	
		TRACE_ENSURE(grp);
		NetGroup* newGroup = NULL;
		if (guild_uid != 0)
		{
			DataObject& dobj3 = gGlobal.guildMemberGrpMap[guild_uid];
			if (dobj3.pobj)
			{
				newGroup = (NetGroup*) dobj3.pobj;
			}
			else
			{
				newGroup = SafeCreateObject(NetGroup);
				dobj3.pobj = newGroup;
			}
		}
		else
		{
			newGroup = SafeCreateObject(NetGroup);
		}
		CGuildMemberData* pGuildMemberData;
	
		for (UInt16 i = 0; i < grp->GetCount(); ++i)
		{
			pGuildMemberData = grp->GetChild<CGuildMemberData>(i);
			if (pGuildMemberData)
			{
				DataObject &dobj = gGlobal.guildMemberMap[pGuildMemberData->GetChar_id()];
				if (dobj.pobj)
				{
					DeleteNetObject(pGuildMemberData);
					pGuildMemberData = CastNetObject<CGuildMemberData>(dobj.pobj);
				}
				else
				{
					LocalRPCManager::GetInstance().Register(pGuildMemberData);
					pGuildMemberData->ClearUpdateMask();
					dobj.pobj = pGuildMemberData;
				}
				newGroup->Add(pGuildMemberData);
			}
		}
		PreSendMemberGrp(newGroup);
		CB_GetGuildMembers(pPeer, context, newGroup);

		if (guild_uid == 0)
			SafeDeleteObject(newGroup);
		DeleteNetGroup(grp);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::DeleteGuildMemberList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &player_ids)
{
	HRESULT hr = S_OK;

	for (UInt i = 0; i < player_ids.size(); ++i)
	{
		UInt32 player_char_id = player_ids[i];
		hr = RemoveGuildMember(player_char_id);
	}

	CB_DeleteGuildMemberList(pPeer, context);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateGuildMemberList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &player_ids, const UInt32Vector &guild_ids, NetGroup* grp, const Boolean &separated)
{
	TRACE_ENSURE(pdbGuildMember && grp);

	HRESULT hr = S_OK;
	NetGroup newGuildMembers;
	UInt16 i = 0;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("guild_member_id %s"), GetVectorString(player_ids).c_str());

	for (Index idx = C_INDEX(0); idx < player_ids.size(); ++idx)
	{
		const UInt32& player_char_id = player_ids[idx];
		CGuildMemberData* pNewData = NULL;
		CGuildMemberData* pGuildMemberData = NULL;
		NetGroup* GuildMemberGroup = NULL;
		DataIdMap::iterator iter = gGlobal.guildMemberMap.end();

		if (player_char_id != 0)
		{
			iter = gGlobal.guildMemberMap.find(player_char_id);
			if (iter != gGlobal.guildMemberMap.end())
			{
				// found existing Item in cache
				pGuildMemberData = CastNetObject<CGuildMemberData>(iter->second.pobj);
				TRACE_ENSURE(pGuildMemberData);
				
				TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("found Guild member data, player_char_id %d"), player_char_id);
			}
			else
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("player_char_id %d not found"), player_char_id);
				continue;
			}
		}

		if (pNewData == NULL || separated)
		{
			// separated data
			if (idx < grp->GetCount())
			{
				pNewData = grp->GetChild<CGuildMemberData>(idx);
			}
			else
			{
				TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("no more data in NetGroup, count %d, player_char_id %d"),
					grp->GetCount(), player_char_id);
				continue;
			}
			if (pNewData == NULL)
			{
				// skip if null data
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("invalid guild data, player_char_id %d"), player_char_id);
				continue;
			}
		}

		if (player_char_id != pNewData->GetChar_id())
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("char_id not match new data char_id, char_id %d, new data char_id %d"),
				player_char_id, pNewData->GetChar_id());
			continue;
		}

		// remove Guild if owner_id = 0
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("player_char_id %d"), player_char_id);
		if (pNewData->IsUpdated(CGuildMemberData::keyMask) && pNewData->GetGuild_uid() == 0)
		{
			hr = RemoveGuildMember(player_char_id);
		}
		else if (player_char_id == 0)
		{
			// create Guild if player_char_id == 0
			pGuildMemberData = pNewData;
			hr = DoCreateGuildMember(pGuildMemberData->GetChar_id(), pGuildMemberData->GetGuild_uid(), pGuildMemberData->GetPost(),
				 pGuildMemberData);
			if (DB_FAILED(hr))
			{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("DB_FAILED Guild create failed, hr 0x%08X"), hr);
				CB_Error(pPeer, context, hr, _FE("DB_FAILED Guild create failed"));
				break;
			}
			pGuildMemberData->PreSend();
			newGuildMembers.Add(pGuildMemberData);
		}
		else
		{
			// else update Guild			
			pGuildMemberData->Clone(pNewData, pNewData->GetUpdateMask());
			gGlobal.EnqueueUpdate(&iter->second);
		}
	}
	DeleteNetGroup(grp, BOOLEAN_TRUE);
	pdbGuildMember->Close();

	if (DB_FAILED(hr))
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("DB_FAILED, hr 0x%08X"), hr);
		DeleteNetChilds(&newGuildMembers);
		return RPC_RESULT_FAIL;
	}

	CB_UpdateGuildMemberList(pPeer, context, &newGuildMembers);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::CreateGuildMember(LPCPEER pPeer, RPCContext &context, const UInt32 &player_char_id, const UInt32 &guild_id, const UInt8 &guildPost)
{
	CGuildMemberData* pGuildMemberData = NULL;

	HRESULT hr = DoCreateGuildMember(player_char_id, guild_id, guildPost, pGuildMemberData);
	if (DB_FAILED(hr))
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("DB_FAILED GuildMember create failed, hr 0x%08X"), hr);
		CB_Error(pPeer, context, hr, _FE("DB_FAILED GuildMember create failed"));
		return RPC_RESULT_OK;
	}
	pGuildMemberData->PreSend();
	
	CB_CreateGuildMember(pPeer, context, pGuildMemberData);
	return RPC_RESULT_OK;
}

HRESULT CGuildMemberData::UpdateDbo(CdboGuildMember* dboGuildMember)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	dboGuildMember->ClearParam();
	if (IsUpdated(keyMask))
	{
		COLUMN_UPDATE_LARGE(dboGuildMember, char_id, GetChar_id());
		COLUMN_UPDATE_LARGE(dboGuildMember, guild_uid, GetGuild_uid());
	}

	if (IsUpdated(donateMask))
	{
		COLUMN_UPDATE_LARGE(dboGuildMember, donate, GetDonate());
	}

	if (IsUpdated(totalDonateMask))
	{
		COLUMN_UPDATE_LARGE(dboGuildMember, totalDonate, GetTotalDonate());
	}

	if (IsUpdated(postMask))
	{
		COLUMN_UPDATE(dboGuildMember, post, GetPost());
	}

	if (IsUpdated(joinDateMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetJoinDate(), dbtime);
		COLUMN_UPDATE(dboGuildMember, joinDate, dbtime);
	}

	if (IsUpdated(businessMask))
	{
		COLUMN_UPDATE(dboGuildMember, businessCount, GetBusinessCount());
		COLUMN_UPDATE_LARGE(dboGuildMember, businessProfit, GetBusinessProfit());
	}
	
	HRESULT hr;
	if (DB_FAILED(hr = dboGuildMember->UpdateByAny(GetChar_id())))
		dboGuildMember->ShowError(hr);
	else
		ClearUpdateMask();
	dboGuildMember->ClearParam();
	dboGuildMember->Close();

	return S_OK;
}

HRESULT CGuildMemberData::DeleteDbo(CdboGuildMember* dboGuildMember)
{
	HRESULT hr;
	if (DB_FAILED(hr = dboGuildMember->Delete(GetChar_id())))
		dboGuildMember->ShowError(hr);
	dboGuildMember->Close();
	return hr;
}

void CGuildMemberData::SetData(CdboGuildMember* dboGuildMember)
{
	SetKey(dboGuildMember->m_char_id.LowPart, dboGuildMember->m_guild_uid.LowPart);
	SetDonate(dboGuildMember->m_donate.LowPart);
	SetTotalDonate(dboGuildMember->m_totalDonate.LowPart);
	SetPost(dboGuildMember->m_post);
	SetBusiness((UInt16) dboGuildMember->m_businessCount, 
		(UInt32) dboGuildMember->m_businessProfit.LowPart);

	time_t uTime;
	TimeConvert(dboGuildMember->m_joinDate, uTime);
	SetJoinDate((UInt32)uTime);
	ClearUpdateMask();
}

void CGuildMemberData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}