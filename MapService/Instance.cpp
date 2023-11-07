//-- Common
#include "Common.h"
#include "Common/procedure.h"
//-- Self
#include "MapService/Instance.h"
//-- Library
#include "Stub/Logic/Map.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/Party.h"
#include "Stub/Logic/ScriptSystem.h"
#include "Stub/Service/MapService.h"
#include "Proxy/Service/WorldService_proxy.h"
#include "Proxy/Logic/WorldEvent.h"
#include "MapService/Global.h"
#include "Resource/GameStringTable.h"


#define HOUR_MS			3600000
#define MINUTE_MS		60000

REGISTER_CLASSTYPE(CInstance, Instance_CLASSID);

CInstance::CInstance()
{
	m_pScriptSys = CScriptSystem::NewInstance();
	TRACE_ENSURE(m_pScriptSys);
	m_pScriptSys->SetRunOnce(BOOLEAN_TRUE);
	m_instId = 0;
	m_instType = 0;
	m_ownerId = 0;
	m_playerLimit = 0;					// player limit inside instance, 0 for unlimited
	m_playerCount = 0;
	m_lastingTime = HOUR_MS;			// default lasting 1 hour
}

CInstance::~CInstance()
{
	Close();
	DeleteNetObject(m_pScriptSys);
}

void CInstance::Init(UInt16 instId, UInt32 ownerId, UInt8 instType)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("instId %d, ownerId %d"),
		instId, ownerId);
	m_instId = instId;
	m_ownerId = ownerId;
	m_instType = instType;
	RaiseUniqEventID(EVT_DESTROY, m_lastingTime);		// close after 1 hour if no one enter
	gGlobal.m_instanceMap[InstanceIndex(instId, ownerId)] = this;
	// register world service when new instance
	WorldService_Proxy::RegisterInstance(gGlobal.pWorldService->pPeer, RPCContext(),
		instId, ownerId);
}

void CInstance::Reset()
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("instId %d, ownerId %d"),
		m_instId, m_ownerId);
	m_pScriptSys->ExecuteInstScript(m_luaFile.c_str(), INSTANCE_INIT, this);
}

void CInstance::Close()
{
	if (m_instId == 0)
		return;

	CMap* pMap;
	MapIndex mapidx(0, 0, m_ownerId);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("instId %d, ownerId %d"),m_instId, m_ownerId);
	m_pScriptSys->ExecuteInstScript(m_luaFile.c_str(), INSTANCE_CLOSE, this);
	// clear all map objects
	for (MapList::iterator it = m_mapList.begin(); it != m_mapList.end(); ++it)
	{
		mapidx.map_id = it->first;
		pMap = gGlobal.DeactivateMap(mapidx);
		if (pMap)
		{
			pMap->ForceAllLeave();
			DeleteNetObject(pMap);
		}
	}
	m_mapList.clear();
	gGlobal.m_instanceMap.erase(InstanceIndex(m_instId, m_ownerId));

	if (m_instId != 0)	// unregister world service when instance close
		WorldService_Proxy::UnregisterInstance(gGlobal.pWorldService->pPeer, RPCContext(),
			m_instId, m_ownerId);

	m_instId = 0;
	m_ownerId = 0;
}

void CInstance::OnEnter(CCharacter* pchar)
{
	m_pScriptSys->ExecuteInstScript(m_luaFile.c_str(), INSTANCE_ENTER, this, pchar);
}

void CInstance::OnPreEnter()
{
	//if (m_playerCount == 0)
	//{
	//	CancelUniqEvent(EVT_DESTROY);
	//	RaiseUniqEventID(EVT_DESTROY, m_lastingTime + HOUR_MS);
	//}
}

void CInstance::OnLeave(CCharacter* pchar)
{
	m_pScriptSys->ExecuteInstScript(m_luaFile.c_str(), INSTANCE_LEAVE, this, pchar);
}

void CInstance::AddMap(UInt16 map_id)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("inst_id %d, owner_id %d, map_id %d"), m_instId,
		m_ownerId, map_id);

	MapIndex mapidx(map_id, 0, m_ownerId);
	CMap* pMap = gGlobal.ActivateMap(mapidx);
	if (pMap)
	{
		pMap->SetInstance(m_instId, m_ownerId);
		m_mapList[map_id] = pMap;
	}
}

void CInstance::SetLasting(int lasting)
{
	m_lastingTime = (UInt32) lasting * MINUTE_MS;

	// schedule destroy event first when no player, additional 1s for safety
	//if (m_playerCount == 0)
	//	RaiseUniqEventID(EVT_DESTROY, m_lastingTime);
}

void CInstance::ChgPlayerCount(Int16 chg)
{
	m_playerCount += chg;

	if (m_playerCount <= 0)	// no player left in instance
	{
		RaiseUniqEventID(EVT_DESTROY, m_lastingTime);
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("RaiseUniqEventID(EVT_DESTROY) instId %d, ownerId %d, time %d"),m_instId, m_ownerId, m_lastingTime);
		m_playerCount = 0;
	}
	else
	{
		if (CancelUniqEvent(EVT_DESTROY))
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CancelUniqEvent(EVT_DESTROY) instId %d, ownerId %d"),m_instId, m_ownerId);
		}
	}
}

UInt16 CInstance::GetAllowCount()
{
	if (m_playerLimit == 0)
		return UINT16_MAX_VALUE;
	else if (m_playerLimit > m_playerCount)
		return m_playerLimit - m_playerCount;
	else
		return 0;
}

CInstance* CInstance::CreateInstance(UInt16 instId, UInt32 ownerId, UInt8 instType)
{
	CInstance* pInst = GetInstance(instId, ownerId);
	if (pInst == NULL)
	{
		pInst = (CInstance*) Reuben::Simulation::CreateObject(Instance_CLASSID);
		pInst->Init(instId, ownerId, instType);
	}

	return pInst;
}

void CInstance::DirectCreateInstance(CCharacter* pchar, StringA luafile, UInt16 instId, 
									 UInt32 ownerId)
{
	CInstance* pInst = GetInstance(instId, ownerId);
	if (pInst == NULL && pchar)
	{
		pchar->GetScript()->ExecuteInstScript(luafile.c_str(), INSTANCE_CREATE_FUNC, NULL);
		CInstance* pInst = CInstance::GetInstance(instId, ownerId);
		if (pInst)
		{
			pInst->SetLuaFile(luafile.c_str());
		}
	}
}

void CInstance::DestroyInstance(UInt16 instId, UInt32 ownerId)
{
	CInstance* pInst = GetInstance(instId, ownerId);
	if (pInst)
		Reuben::Simulation::DeleteObject(pInst);
}

CInstance* CInstance::GetInstance(UInt16 instId, UInt32 ownerId)
{
	InstanceMap::iterator it = gGlobal.m_instanceMap.find(InstanceIndex(instId, ownerId));
	if (it != gGlobal.m_instanceMap.end())
		return it->second;
	else
		return NULL;
}

void CInstance::DeleteAll()
{
	InstanceMap instanceMap = gGlobal.m_instanceMap;
	for (InstanceMap::iterator it = instanceMap.begin(); it != instanceMap.end(); ++it)
		Reuben::Simulation::DeleteObject(it->second);
}

RPCResult CInstance::JumpInstance(RPCContext &context)
{
	PROCEDURE_START_FUNC6(CInstance::JumpInstance, context, String, script, 
						UInt16, instId, UInt32, ownerId, UInt16, map_id, UInt16, x, UInt16, y)

		// query instance in WorldService
		WorldService_Proxy::QueryInstance(gGlobal.pWorldService->pPeer, context, 
			instId, ownerId);

	PROCEDURE_WAIT2(1, WorldService, CB_QueryInstance, Boolean, bExist, UInt16, allowCount)

		Session* psess = GetSession(context.sessionId);
		if (psess == NULL || psess->pchar == NULL)
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, 
				_FW("[session not found or char null], sessionId %s"),
				context.sessionId.GetString().c_str());
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		CCharacter* pchar = psess->pchar;
		if (bExist)
		{
			// if exist, do RemoteCharJump
			UInt16 playerCount = (pchar->HasParty() ? 
								pchar->GetParty()->GetCount() : 1);
			if (allowCount >= playerCount)
				MapService_Stub::JumpMap(pchar, map_id, 0, ownerId, x, y, instId);
			else
				pchar->CB_Error(pchar->GetPeer(), pchar->GetContext(), 1, _T("MSG_INSTANCE_FULL"));
		}
		else
		{
			// if not, create instance in this Map Service
			StringA luaFile(script.c_str());

			pchar->GetScript()->ExecuteInstScript(luaFile.c_str(), INSTANCE_CREATE_FUNC, 
				NULL);
			CInstance* pInst = CInstance::GetInstance(instId, ownerId);
			if (pInst)
			{
				pInst->SetLuaFile(luaFile.c_str());
				pchar->JumpMap(map_id, x, y, 0, ownerId, instId);
			}
		}

	PROCEDURE_END

	return RPC_RESULT_OK;
}

RPCResult CInstance::JumpEmptyInstance(RPCContext &context)
{
	PROCEDURE_START_FUNC6(CInstance::JumpEmptyInstance, context, String, script, 
						UInt16, instId, UInt32, ownerId, UInt16, map_id, UInt16, x, UInt16, y)

		// query instance in WorldService
		WorldService_Proxy::QueryInstance(gGlobal.pWorldService->pPeer, context, 
			instId, ownerId);

	PROCEDURE_WAIT2(1, WorldService, CB_QueryInstance, Boolean, bExist, UInt16, allowCount)

		Session* psess = GetSession(context.sessionId);
		if (psess == NULL || psess->pchar == NULL)
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, 
				_FW("[session not found or char null], sessionId %s"),
				context.sessionId.GetString().c_str());
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		CCharacter* pchar = psess->pchar;
		if (bExist)
		{
			// if exist, don't jump
			String errMsg;
			errMsg = gGlobal.GetStringTable()->Get(_T("MSG_INSTANCE_OCCUPIED_FAIL_ENTRY"));
			if (pchar)
                pchar->CB_AwardMsg(pchar->GetPeer(), pchar->GetContext(), errMsg);
		}
		else
		{
			// if not, create instance in this Map Service
			StringA luaFile(script.c_str());

			pchar->GetScript()->ExecuteInstScript(luaFile.c_str(), INSTANCE_CREATE_FUNC, 
				NULL);
			CInstance* pInst = CInstance::GetInstance(instId, ownerId);
			if (pInst)
			{
				pInst->SetLuaFile(luaFile.c_str());
				pchar->JumpMap(map_id, x, y, 0, ownerId, instId);
			}
		}

	PROCEDURE_END

	return RPC_RESULT_OK;
}



void CInstance::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_DESTROY:				// close instance event
		Reuben::Simulation::DeleteObject(this);
		break;
	}
}