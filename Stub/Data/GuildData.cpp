//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/GuildData.h"
#include "Stub/Data/GuildMemberData.h"
//-- Library
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboGuild.h"
#include "ObjectService/Global.h"
#include "ObjectService/DboSourcePool.h"

REGISTER_CLASSTYPE(CGuildData, GuildData_CLASSID);

CGuildData* NewGuild();
CdboGuild* &pdbGuild = gGlobal.gTables.pGuild;
StringVector CdboGuildAccessor::m_params;
ThreadKey CGuildData::m_key;

#define TOP_NUM				100
#define PVP_TOP				16

struct GuildRequest : public WorkRequest<UInt32>
{
	StringVector	sqls;
	UInt8	topNum;
	Boolean	guildPvP;
	GuildRequest(UInt32 id, LPCPEER pPeer, RPCContext &context)
		: WorkRequest<UInt32>(id, pPeer, context)
	{
		guildPvP = BOOLEAN_FALSE;
	}
};



////////////////////////////////////////////////////////////////////////
////						Guild Data							////
////////////////////////////////////////////////////////////////////////
CGuildData* NewGuild()
{
	CGuildData *pGuildData = NULL;
	UInt32 guild_id = (UInt32) pdbGuild->m_guild_uid.LowPart;
	DataObject &dobj = gGlobal.guildMap[guild_id];

	if (dobj.pobj == NULL) {
		pGuildData = CGuildData::NewInstance();
		pGuildData->SetData(pdbGuild);			// assign values to character data
		dobj.pobj = pGuildData;
	} else {
		pGuildData = CastNetObject<CGuildData>(dobj.pobj);
	}

	return pGuildData;
}

HRESULT RemoveGuild(UInt32 guild_uid)
{
	DataIdMap::iterator iter = gGlobal.guildMap.find(guild_uid);
	if (iter != gGlobal.guildMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CGuildData* pGuildData = CastNetObject<CGuildData>(iter->second.pobj);
		if (pGuildData)
			gGlobal.m_deleteGroup.Add(pGuildData);
		RemoveGuildMembers(guild_uid);
		gGlobal.guildMap.erase(iter);
	}

	return S_OK;
}

HRESULT ReleaseGuild(UInt32 guild_uid)
{
	DataIdMap::iterator iter = gGlobal.guildMap.find(guild_uid);
	if (iter != gGlobal.guildMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CGuildData* pGuildData = CastNetObject<CGuildData>(iter->second.pobj);
		if (pGuildData)
		{
			pGuildData->SetOfflinetime((UInt32)time(NULL));
			if (pGuildData->IsUpdated())
				gGlobal.m_releaseGroup.Add(pGuildData);
			else
				DeleteNetObject(pGuildData);
		}
		ReleaseGuildMembers(guild_uid);
		gGlobal.guildMap.erase(iter);
	}

	return S_OK;
}

HRESULT CGuildData::UpdateDbo(CdboGuild* dboGuild)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CGuildData UpdateDbo: guild_id %d, updateMask %d,%d"), 
		GetGuild_uid(), m_updateMask.low, m_updateMask.high);

	dboGuild->ClearParam();		

	if (IsUpdated(nameMask))
	{
		COLUMN_UPDATE_STRING(dboGuild, name, GetName());
	}

	if (IsUpdated(creatorMask))
	{
		COLUMN_UPDATE_STRING(dboGuild, creator, GetCreator());
	}

	if (IsUpdated(masterMask))
	{
		COLUMN_UPDATE_STRING(dboGuild, master, GetMaster());
	}

	if (IsUpdated(allianceMask))
	{
		COLUMN_UPDATE_LARGE(dboGuild, alliance, GetAlliance_id());
	}

	if (IsUpdated(enemyMask))
	{
		COLUMN_UPDATE_LARGE(dboGuild, enemy, GetEnemy_id());
	}

	if (IsUpdated(secMasterMask))
	{
		COLUMN_UPDATE_STRING(dboGuild, secMaster, GetSecMasterName());
	}

	if (IsUpdated(levelMask))
	{
		COLUMN_UPDATE(dboGuild, level, GetLevel());
	}

	if (IsUpdated(prosperMask))
	{
		COLUMN_UPDATE_LARGE(dboGuild, prosper, GetProsper());
		COLUMN_UPDATE(dboGuild, maxProsper, GetMax_prosper());
	}


	if (IsUpdated(fundMask))
	{
		COLUMN_UPDATE_LARGE(dboGuild, fund, GetFund());
	}

	if (IsUpdated(memberNumMask))
	{
		COLUMN_UPDATE(dboGuild, totalMember, GetTotalMember());
		COLUMN_UPDATE(dboGuild, maxMember, GetMaxMember());
	}

	if (IsUpdated(aimMask))
	{
		COLUMN_UPDATE_STRING(dboGuild, aim, GetAim());
	}

	if (IsUpdated(ruleMask))
	{
		COLUMN_UPDATE_STRING(dboGuild, rule, GetRule());
	}

	if (IsUpdated(guildPvPScoreMask))
	{
		COLUMN_UPDATE_LARGE(dboGuild, guildPvPScore, GetGuildPvPScore());
	}

	if (IsUpdated(guildPvPBidMask))
	{
		COLUMN_UPDATE_LARGE(dboGuild, guildPvPBid, GetGuildPvPBid());
	}

	if (IsUpdated(guildPvPGroupMask))
	{
		COLUMN_UPDATE(dboGuild, guildPvPGroup, GetGuildPvPGroup());
	}

	if (IsUpdated(groupPvPVSGuildMask))
	{
		COLUMN_UPDATE_STRING(dboGuild, groupPvPVSGuild, GetGroupPvPVSGuild());
	}


	if (IsUpdated(manorScaleMask))
	{
		COLUMN_UPDATE(dboGuild, manorScale, GetManorScale());
	}

	if (IsUpdated(manorRoomMask))
	{
		COLUMN_UPDATE(dboGuild, manorRoom, GetRoom());
	}

	if (IsUpdated(manorMaterialMask))
	{
		COLUMN_UPDATE_LARGE(dboGuild, manorMaterial, GetMaterial());
	}

	if (IsUpdated(manorGuardMask))
	{
		COLUMN_UPDATE(dboGuild, manorGuard, GetGuard());
	}

	if (IsUpdated(manorTrainPlayerPowMask))
	{
		COLUMN_UPDATE(dboGuild, manorTrainPlayerExPow, GetTrainCharExPow());
		COLUMN_UPDATE(dboGuild, manorTrainPlayerInPow, GetTrainCharInPow());	
	}

	if (IsUpdated(manorTrainPartnerPowMask))
	{
		COLUMN_UPDATE(dboGuild, manorTrainPartnerExPow, GetTrainPartnerExPow());
		COLUMN_UPDATE(dboGuild, manorTrainPartnerInPow, GetTrainPartnerInPow());	
	}

	if (IsUpdated(manorRoomPropertyMask))
	{
		COLUMN_UPDATE(dboGuild, manorRoomDrugLv, GetDrugLv());
		COLUMN_UPDATE(dboGuild, manorRoomCookLv, GetCookLv());
		COLUMN_UPDATE(dboGuild, manorRoomWineLv, GetWineLv());
		COLUMN_UPDATE(dboGuild, manorRoomHPLv, GetHpLv());	
		COLUMN_UPDATE(dboGuild, manorRoomSPLv, GetSpLv());		
	}

	if (IsUpdated(manorMaintainNumMask))
	{
		COLUMN_UPDATE_LARGE(dboGuild, manorMaintainNum, GetMaintainNum());
	}

	if (IsUpdated(lastMaintainDateMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetLastMaintainDate(), dbtime);
		COLUMN_UPDATE(dboGuild, lastMaintainDate, dbtime);
	}

	if (IsUpdated(manorAuraMask))
	{
		COLUMN_UPDATE(dboGuild, manorAura, GetAura());
	}

	if (IsUpdated(offlinetimeMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetOfflinetime(), dbtime);
		COLUMN_UPDATE(dboGuild, offlinetime, dbtime);
	}

	if (IsUpdated(clearBusinessDateMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetClearBusinessDate(), dbtime);
		COLUMN_UPDATE(dboGuild, clearBusinessDate, dbtime);
	}

	if (IsUpdated(autoManageBusinessMask))
	{
		UInt8 autoManageBusiness = GetAutoManageBusiness()? 1:0;
		COLUMN_UPDATE(dboGuild, autoManageBusiness, autoManageBusiness);
	}

	HRESULT hr;
	if (DB_FAILED(hr = dboGuild->UpdateByAny(GetGuild_uid())))
		dboGuild->ShowError(hr);
	else
		ClearUpdateMask();
	dboGuild->ClearParam();		
	dboGuild->Close();

	return S_OK;
}

HRESULT CGuildData::DeleteDbo(CdboGuild* dboGuild)
{
	HRESULT hr;
	//ThreadGuard guard(m_key);

	if (DB_FAILED(hr = dboGuild->Delete(GetGuild_uid())))
		dboGuild->ShowError(hr);
	dboGuild->Close();

	return hr;
}

void CGuildData::SetData(CdboGuild* dboGuild)
{
	String name(dboGuild->m_name);
	String creator(dboGuild->m_creator);
	String master(dboGuild->m_master);
	String secMaster(dboGuild->m_secMaster);

	SetId((UInt32) dboGuild->m_guild_uid.LowPart);
	SetName(name.Trim());
	SetCreator(creator.Trim());
	SetMaster(master.Trim());
	SetAlliance((UInt32)dboGuild->m_alliance.LowPart);
	SetEnemy((UInt32)dboGuild->m_enemy.LowPart);
	SetSecMaster(secMaster.Trim());
	SetLevel((UInt16)dboGuild->m_level);
	SetProsper((UInt32)dboGuild->m_prosper.LowPart, (UInt16)dboGuild->m_maxProsper);
	SetFund((UInt32)dboGuild->m_fund.LowPart);
	SetMemberNum((UInt16)dboGuild->m_totalMember, (UInt16)dboGuild->m_maxMember);
	SetAim(dboGuild->m_aim);
	SetRule(dboGuild->m_rule);
	SetGuildPvPScore((UInt32) dboGuild->m_guildPvPScore.LowPart);
	SetGuildPvPBid((UInt32) dboGuild->m_guildPvPBid.LowPart);
	SetGuildPvPGroup((UInt8)dboGuild->m_guildPvPGroup);
	SetGroupPvPVSGuild(dboGuild->m_groupPvPVSGuild);
	SetManorScale((UInt8)dboGuild->m_manorScale);
	SetRoom(dboGuild->m_manorRoom);
	SetMaterial((UInt32) dboGuild->m_manorMaterial.LowPart);
	SetGuard((UInt8)dboGuild->m_manorGuard);
	SetManorTrainPlayerPow(dboGuild->m_manorTrainPlayerExPow ,dboGuild->m_manorTrainPlayerInPow);
	SetManorTrainPartnerPow(dboGuild->m_manorTrainPartnerExPow ,dboGuild->m_manorTrainPartnerInPow);
	SetManorRoomProperty(dboGuild->m_manorRoomDrugLv, dboGuild->m_manorRoomCookLv, 
						dboGuild->m_manorRoomWineLv, dboGuild->m_manorRoomHPLv, dboGuild->m_manorRoomSPLv);
	SetMaintainNum((UInt32)dboGuild->m_manorMaintainNum.LowPart);
	SetAura(dboGuild->m_manorAura);
	time_t lastPayDate, offTime;         
	TimeConvert(dboGuild->m_lastMaintainDate, lastPayDate);
	TimeConvert(dboGuild->m_offlinetime, offTime);
	SetLastMaintainDate((UInt32) lastPayDate);
	SetOfflinetime((UInt32) offTime);
	time_t uTime;
	TimeConvert(dboGuild->m_clearBusinessDate, uTime);
	SetClearBusinessDate((UInt32)uTime);
	SetAutoManageBusiness((Boolean)(dboGuild->m_autoManageBusiness == 1));


	ClearUpdateMask();
}

void CGuildData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

RPCResult ObjectService_Stub::DeleteGuildList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &guild_uids)
{
	HRESULT hr = S_OK;

	for (UInt i = 0; i < guild_uids.size(); ++i)
	{
		UInt32 guild_uid = guild_uids[i];
		hr = RemoveGuild(guild_uid);
	}

	CB_DeleteGuildList(pPeer, context);
	return RPC_RESULT_OK;
}

DWORD WINAPI QueueGetGuild(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();

	GuildRequest* pRequest = (GuildRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource(); 
	if (dbSrc)
	{
		NetGroup guildGroup;
		CGuildData* pGuildData;
		CdboGuild* dboGuild = SafeCreateObject(CdboGuild);

		guildGroup.SetValidate(BOOLEAN_FALSE);
		dboGuild->SetDataSource(dbSrc);
	
		if (pRequest->guildPvP)
			hr = dboGuild->RSByGuildPvP();		
		else if (pRequest->key != 0)
			hr = dboGuild->RSByID(pRequest->key);
		else if (pRequest->sqls.empty())
			hr = dboGuild->RSALL();
		
		if (pRequest->sqls.empty())
		{
			if (DB_SUCCEEDED(hr))
			{
				hr = dboGuild->MoveFirst();
				while (hr == S_OK)
				{
					pGuildData = SafeCreateObject(CGuildData);
					pGuildData->SetData(dboGuild);
					pGuildData->SetSendMask(CGuildData::AllMask);
					guildGroup.Add(pGuildData);
					hr = dboGuild->MoveNext();
				}
			}
		}
		else
		{
			for (StringVector::iterator it = pRequest->sqls.begin();
				it != pRequest->sqls.end(); ++it)
			{
				if (DB_SUCCEEDED(hr = dboGuild->RSTopCase(it->c_str(), pRequest->topNum)))
				{
					hr = dboGuild->MoveFirst();
					while (hr == S_OK)
					{
						pGuildData = SafeCreateObject(CGuildData);
						pGuildData->SetData(dboGuild);
						RPCMask mask;
						mask.MaskBits(CGuildData::idMask, CGuildData::masterMask, CGuildData::guildPvPScoreMask, CGuildData::nameMask,
							CGuildData::memberNumMask, CGuildData::guildPvPGroupMask,CGuildData::fundMask, CGuildData::guildPvPBidMask,
							 -1);
						pGuildData->SetSendMask(mask);
						guildGroup.Add(pGuildData);
						hr = dboGuild->MoveNext();
					}
				}
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetGuild"), hr, dboGuild->GetErrorMsg(hr), 
			pRequest->context, &guildGroup);
		dboGuild->Close();
		SafeDeleteObject(dboGuild);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, owner_id %d"),
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

RPCResult ObjectService_Stub::GetGuildRank(
	LPCPEER pPeer, RPCContext &context)
{
	TRACE_ENSURE(pdbGuild != NULL);
	
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC(ObjectService, GetGuildRank, NULL)

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("session_id %s"),
			context.sessionId.GetString().c_str());

		gGlobal.WorkQueueBegin();
		GuildRequest *pRequest = SafeCreateObject3(GuildRequest, 0, pPeer, context);
		StringVector &sqls = pRequest->sqls;
		pRequest->topNum = TOP_NUM;
		
		sqls.push_back(_T("guildPvPScore > 0 ORDER BY guildPvPScore DESC"));

#ifdef DB_THREAD_POOL
		gGlobal.QueueUserWorkItem(QueueGetGuild, pRequest, DB_WORKER_FLAG);
#else
		QueueGetGuild(pRequest);
#endif

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, guildGroup)

		CB_GetGuildRank(pPeer, context, guildGroup);
		DeleteNetGroup(guildGroup, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetGuildByPvPGroup(
	LPCPEER pPeer, RPCContext &context)
{
	TRACE_ENSURE(pdbGuild != NULL);
	
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC(ObjectService, GetGuildByPvPGroup, NULL)
		

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("session_id %s"),
			context.sessionId.GetString().c_str());

		gGlobal.WorkQueueBegin();
		GuildRequest *pRequest = SafeCreateObject3(GuildRequest, 0, pPeer, context);
		pRequest->guildPvP = BOOLEAN_TRUE;
		

#ifdef DB_THREAD_POOL
		gGlobal.QueueUserWorkItem(QueueGetGuild, pRequest, DB_WORKER_FLAG);
#else
		QueueGetGuild(pRequest);
#endif

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, grp)

		NetGroup guildGroup;
		for (UInt16 i = 0; i < grp->GetCount(); ++i)
		{
			CGuildData* pGuildData = grp->GetChild<CGuildData>(i);
			if (pGuildData)
			{
				DataObject &dobj = gGlobal.guildMap[pGuildData->GetGuild_uid()];
				if (dobj.pobj)
				{
					DeleteNetObject(pGuildData);
					pGuildData = CastNetObject<CGuildData>(dobj.pobj);
				}
				else
				{
					LocalRPCManager::GetInstance().Register(pGuildData);
					pGuildData->ClearUpdateMask();
					dobj.pobj = pGuildData;
				}
				pGuildData->SetSendMask(CGuildData::AllMask);
				guildGroup.Add(pGuildData);
			}
		}
		CB_GetGuilds(pPeer, context, &guildGroup);
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

RPCResult ObjectService_Stub::GetGuildByPvPBid(
	LPCPEER pPeer, RPCContext &context)
{
	TRACE_ENSURE(pdbGuild != NULL);
	
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC(ObjectService, GetGuildByPvPGroup, NULL)
		

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("session_id %s"),
			context.sessionId.GetString().c_str());

		gGlobal.WorkQueueBegin();
		GuildRequest *pRequest = SafeCreateObject3(GuildRequest, 0, pPeer, context);
		StringVector &sqls = pRequest->sqls;
		pRequest->topNum = PVP_TOP;
		
		sqls.push_back(_T("guildPvPBid > 0 AND fund >= guildPvPBid ORDER BY guildPvPBid DESC"));

#ifdef DB_THREAD_POOL
		gGlobal.QueueUserWorkItem(QueueGetGuild, pRequest, DB_WORKER_FLAG);
#else
		QueueGetGuild(pRequest);
#endif

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, grp)

		NetGroup guildGroup;
		for (UInt16 i = 0; i < grp->GetCount(); ++i)
		{
			CGuildData* pGuildData = grp->GetChild<CGuildData>(i);
			if (pGuildData)
			{
				DataObject &dobj = gGlobal.guildMap[pGuildData->GetGuild_uid()];
				if (dobj.pobj)
				{
					DeleteNetObject(pGuildData);
					pGuildData = CastNetObject<CGuildData>(dobj.pobj);
				}
				else
				{
					LocalRPCManager::GetInstance().Register(pGuildData);
					pGuildData->ClearUpdateMask();
					dobj.pobj = pGuildData;
				}
				pGuildData->SetSendMask(CGuildData::AllMask);
				guildGroup.Add(pGuildData);
			}
		}
		CB_GetGuilds(pPeer, context, &guildGroup);
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

RPCResult ObjectService_Stub::GetGuildByID(LPCPEER pPeer, RPCContext &context, 
										   const UInt32 &guild_uid)
{
	TRACE_ENSURE(pdbGuild != NULL);

	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC1(ObjectService, GetGuildByID, NULL, UInt32, guild_uid)

		NetGroup guildGroup;
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GetGuildByID: guild_uid %d"), guild_uid);

		if (guild_uid == 0)
		{
			// uid 0 for getting all guilds
			TRACE_WARNDTL(GLOBAL_LOGGER, _FW("cannot get guild_uid 0"));
		}
		else
		{
			// get one guild by guild_uid
			DataIdMap::iterator it = gGlobal.guildMap.find(guild_uid);
			if (it != gGlobal.guildMap.end())
			{
				CGuildData *pGuildData = CastNetObject<CGuildData>(it->second.pobj);
				if (pGuildData)
				{
					pGuildData->PreSend();
					guildGroup.Add(pGuildData);
				}
			}
		}

		if (guildGroup.GetCount() > 0)
		{
			CB_GetGuilds(pPeer, context, &guildGroup);
			PROCEDURE_RETURN
		}
		else
		{
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			GuildRequest *pRequest = SafeCreateObject3(GuildRequest, guild_uid, pPeer, context);
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetGuild, pRequest, DB_WORKER_FLAG);
#else
			QueueGetGuild(pRequest);
#endif
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, grp)
		
		TRACE_ENSURE(grp);
		NetGroup guildGroup;

		for (UInt16 i = 0; i < grp->GetCount(); ++i)
		{
			CGuildData* pGuildData = grp->GetChild<CGuildData>(i);
			if (pGuildData)
			{
				DataObject &dobj = gGlobal.guildMap[pGuildData->GetGuild_uid()];
				if (dobj.pobj)
				{
					DeleteNetObject(pGuildData);
					pGuildData = CastNetObject<CGuildData>(dobj.pobj);
				}
				else
				{
					LocalRPCManager::GetInstance().Register(pGuildData);
					pGuildData->ClearUpdateMask();
					dobj.pobj = pGuildData;
				}
				pGuildData->SetSendMask(CGuildData::AllMask);
				guildGroup.Add(pGuildData);
			}
		}
		CB_GetGuilds(pPeer, context, &guildGroup);
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

HRESULT DoCreateGuild(UInt32 char_id, String guild_name, UInt8 guildPost, 
					  String guild_creator, String guild_master, 
					  CGuildData* &pGuildData, CGuildMemberData* &pGuildMemberData)
{
	HRESULT hr = S_OK;
	pGuildData = NULL;

	// check character haven't join any guild first
	if (IdentifyGuild(char_id) != 0)
		return 1;

	hr = pdbGuild->Insert(guild_name.c_str(), guild_creator.c_str(), guild_master.c_str(), 1);
	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbGuild->MoveFirst()))
	{
		pGuildData = NewGuild();
	}
	if (DB_FAILED(hr))
		pdbGuild->ShowError(hr);
	pdbGuild->Close();

	if (DB_SUCCEEDED(hr) && pGuildData)
	{
		hr = DoCreateGuildMember(char_id, pGuildData->GetGuild_uid(), guildPost, pGuildMemberData);
		// remove new guild if fail to create first member
		if (DB_FAILED(hr) || pGuildMemberData == NULL)
			RemoveGuild(pGuildData->GetGuild_uid());
	}

	return hr;
}

RPCResult ObjectService_Stub::CreateGuild(LPCPEER pPeer, RPCContext &context, 
										  const UInt32 &char_id, const String &guild_name, 
										  const UInt8 &guild_post, const String &creator, 
										  const String &master)
{
	CGuildData* pGuildData = NULL;
	CGuildMemberData* pGuildMemberData = NULL;

	HRESULT hr = DoCreateGuild(char_id, guild_name, guild_post, creator, master, 
		pGuildData, pGuildMemberData);
	if (DB_FAILED(hr))
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("DB_FAILED Guild create failed, hr 0x%08X"), hr);
		CB_Error(pPeer, context, hr, _FE("DB_FAILED Guild create failed"));
		return RPC_RESULT_OK;
	}
	pGuildData->PreSend();
	pGuildMemberData->PreSend();
	CB_CreateGuild(pPeer, context, pGuildData, pGuildMemberData);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::ReleaseGuildAndMem(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("guild_uid %d"), guild_uid);
	ReleaseGuild(guild_uid);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateGuildList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &guild_ids, NetGroup* grp, const Boolean &separated)
{
	TRACE_ENSURE(pdbGuild && grp);

	HRESULT hr = S_OK;
	NetGroup newGuilds;
	UInt16 i = 0;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("guild_uid %s"), GetVectorString(guild_ids).c_str());

	for (Index idx = C_INDEX(0); idx < guild_ids.size(); ++idx)
	{
		const UInt32& guild_uid = guild_ids[idx];
		CGuildData* pNewData = NULL;
		CGuildData* pGuildData = NULL;
		NetGroup* guildGroup = NULL;
		DataIdMap::iterator iter = gGlobal.guildMap.end();

		if (guild_uid != 0)
		{
			iter = gGlobal.guildMap.find(guild_uid);
			if (iter != gGlobal.guildMap.end())
			{
				// found existing Item in cache
				pGuildData = CastNetObject<CGuildData>(iter->second.pobj);
				TRACE_ENSURE(pGuildData);
				
				TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("found Guild data, guild_uid %d"), guild_uid);
			}
			else
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("guild_uid %d not found"), guild_uid);
				continue;
			}
		}

		if (pNewData == NULL || separated)
		{
			// separated data
			if (idx < grp->GetCount())
			{
				pNewData = grp->GetChild<CGuildData>(idx);
			}
			else
			{
				TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("no more data in NetGroup, count %d, guild_uid %d"), grp->GetCount(), guild_uid);
				continue;
			}
			if (pNewData == NULL)
			{
				// skip if null data
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("invalid guild data, guild_uid %d"), guild_uid);
				continue;
			}
		}

		if (guild_uid != pNewData->GetGuild_uid())
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("guild_uid not match new data guild_uid, guild_uid %d, new data guild_uid %d"),
				guild_uid, pNewData->GetGuild_uid());
			continue;
		}

		// remove Guild if owner_id = 0
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("guild_uid %d"), guild_uid);
		if (pNewData->IsUpdated(CGuildData::idMask) && pNewData->GetGuild_uid() == 0)
		{
			hr = RemoveGuild(guild_uid);
		}
		else if (guild_uid == 0)
		{
			/* dont' create guild by update guild list
			// create Guild if Guild_uid == 0
			pGuildData = pNewData;
			hr = DoCreateGuild(pGuildData->GetName(), pGuildData->GetCreator(), pGuildData->GetMaster(),
				 pGuildData);
			if (DB_FAILED(hr))
			{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("DB_FAILED guild create failed, hr 0x%08X"), hr);
				CB_Error(pPeer, context, hr, _FE("DB_FAILED guild create failed"));
				break;
			}
			pGuildData->PreSend();
			newGuilds.Add(pGuildData);
			*/
		}
		else
		{
			// else update Guild			
			pGuildData->Clone(pNewData, pNewData->GetUpdateMask());
			gGlobal.EnqueueUpdate(&iter->second);
		}
	}
	DeleteNetGroup(grp, BOOLEAN_TRUE);

	CB_UpdateGuildList(pPeer, context, &newGuilds);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::ResetGuildBid(LPCPEER pPeer, RPCContext &context)
{
	pdbGuild->ResetBid();
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::ResetGuildGroup(LPCPEER pPeer, RPCContext &context)
{
	pdbGuild->ResetGroup();
	return RPC_RESULT_OK;
}

