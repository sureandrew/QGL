//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/WorldEvent.h"
//-- Library
#include "Stub/Logic/Map.h"
#include "Stub/Logic/MapNPC.h"
#include "MapService/Global.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResEventSchedule.h"

REGISTER_CLASSTYPE(CWorldEvent, WorldEvent_CLASSID);

enum WE_MapType
{
	WE_MAPTYPE_MAPID	= 0,
	WE_MAPTYPE_ALL,
	WE_MAPTYPE_TOWN,
	WE_MAPTYPE_WILD,
};

void CWorldEvent::Init()
{
	TRACE_ENSURE(gGlobal.m_resource);
	m_pData = gGlobal.m_resource->GetEventScheduleData(GetEventId());
	if (m_pData == NULL)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("fail to init data, eventId %d"), GetEventId());
	}
}

void CWorldEvent::Execute(CMap *pMap)
{
	if (pMap == NULL)
		return;

	switch (GetEventType())
	{
	case WE_TYPE_BUFF:
		ActivateBuff(pMap);
		break;

	case WE_TYPE_NPC:
		if (IsUpdated(npcListMask))
			CreateNpc(pMap);
		break;

	case WE_TYPE_MAP:
		OpenMap(pMap);
		break;
	case WE_TYPE_SCRIPT:
		// in WorldService_Proxy::CB_StartEvent
		//ExecuteScript(pMap);
		break;
	}
}

void CWorldEvent::Terminate(CMap *pMap)
{
	if (pMap == NULL)
		return;

	switch (GetEventType())
	{
	case WE_TYPE_BUFF:
		DeactivateBuff(pMap);
		break;

	case WE_TYPE_NPC:
		RemoveNpc(pMap);
		break;

	case WE_TYPE_MAP:
		CloseMap(pMap);
		break;
	}
}

Boolean CWorldEvent::BuffMatchMap(CMap* pMap)
{
	UInt16 mapId = pMap->GetMap_id();
	const UInt16Vector &mapIdList = GetMapList();
	bool match = false;

	switch (GetMapType()) {
	case WE_MAPTYPE_MAPID:
		for (UInt16Vector::const_iterator it = mapIdList.begin();
			it != mapIdList.end(); ++it)
		{
			if (*it == mapId) {
				match = true;
				break;
			}
		}
		break;

	case WE_MAPTYPE_ALL:
		match = true;
		break;

	case WE_MAPTYPE_TOWN:
		{
			MapDB* mapdb = pMap->GetMapInfo();
			if (mapdb && mapdb->m_basicinfo->m_maptype == 1)
				match = true;
		}
		break;

	case WE_MAPTYPE_WILD:
		{
			MapDB* mapdb = pMap->GetMapInfo();
			if (mapdb && mapdb->m_basicinfo->m_maptype == 0)
				match = true;
		}
		break;
	}

	return match;
}

void CWorldEvent::ActivateBuff(CMap* pMap)
{
	// check this map match event requirement
	if (BuffMatchMap(pMap)) {
		String strBuff = GetVectorString(GetBuffList());
		TRACE_INFODTL_3(GLOBAL_LOGGER, 
			_T("WorldEvent ActivateBuff: eventId %d, map_id %d, buff_id %s"),
				GetEventId(), pMap->GetMap_id(), strBuff.c_str());

		BuffInfo buffInfo;
		buffInfo.buffIds = GetBuffList();
		buffInfo.period = GetBuffPeriod();
		buffInfo.bLeave = GetBuffLeave();
		pMap->AddBuffInfo(GetEventId(), buffInfo);
	}
}

void CWorldEvent::DeactivateBuff(CMap* pMap)
{
	// deactivate when only buff need to clear
	if (GetBuffClear()) {
		String strBuff = GetVectorString(GetBuffList());
		TRACE_INFODTL_3(GLOBAL_LOGGER, 
			_T("WorldEvent DeactivateBuff: eventId %d, map_id %d, buff_id %s"),
				GetEventId(), pMap->GetMap_id(), strBuff.c_str());
		pMap->RemoveBuffInfo(GetEventId());
	}
}

void CWorldEvent::CreateNpc(CMap* pMap)
{
	const NpcInfoVec &npcList = GetNpcList();
	const UInt16Vector& lineCounts = GetNpcLineCounts();
	UInt32 lifeTime = GetNpcLiveTime();
	CMapNPC *pNpc;
	UInt16 eventId = GetEventId();

	if (pMap->GetLine_id() >= lineCounts.size()) {
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
			_T("WorldEvent CreateNpc Error: [excess line id in count], eventId %d, map_id %d, line_id %d"),
				GetEventId(), pMap->GetMap_id(), pMap->GetLine_id());
		return;
	}
	UInt16 lineCount = lineCounts[pMap->GetLine_id()];
	if (lineCount > npcList.size()) {
		TRACE_ERRORDTL_5(GLOBAL_LOGGER, 
			_T("WorldEvent CreateNpc Error: [excess npc list in count], eventId %d, map_id %d, line_id %d, count %d, npc list size %d"),
				GetEventId(), pMap->GetMap_id(), pMap->GetLine_id(), lineCount,
				npcList.size());
		return;
	}

	UInt16 x, y;
	for (UInt16 i = 0; i < lineCount; ++i)
	{
		const NpcInfo &npcInfo = npcList[i];
		if (pMap->GetMap_id() == npcInfo.mapId) {
			GetRandPos(pMap, &x, &y);
			pNpc = pMap->CreateNpc(npcInfo.npcId, x, y, 0, lifeTime, GetEventId());
			if (pNpc) {
				pNpc->SetEventId(eventId);
				TRACE_INFODTL_6(GLOBAL_LOGGER, 
					_T("WorldEvent CreateNpc: eventId %d, map_id %d, npcId %d, pos %d:%d, lifeTime %d"),
						GetEventId(), pMap->GetMap_id(), npcInfo.npcId, 
						x, y, lifeTime);
			}
		}
	}
}

void CWorldEvent::RemoveNpc(CMap* pMap)
{
	pMap->RemoveNpcEvent(GetEventId());
}

void CWorldEvent::GetRandPos(CMap* pMap, UInt16* px, UInt16* py)
{
	*px = 0;
	*py = 0;
	if (pMap == NULL || m_pData == NULL || m_pData->npcData == NULL || m_pData->npcData->npcMapData.empty())
		return;

	EventScheduleMapDataVector &mapVec = m_pData->npcData->npcMapData;
	EventScheduleMapData* pMapData;
	UInt16Vector posIndices;
	UInt16 map_id = pMap->GetMap_id();
	UInt16 i = 0;
	for (EventScheduleMapDataVector::iterator it = mapVec.begin();
		it != mapVec.end(); ++it, ++i)
	{
		pMapData = *it;
		if (pMapData && pMapData->mapId == map_id && 
			!pMap->IsOccupied(pMapData->x, pMapData->y))
			posIndices.push_back(i);
	}

	if (!posIndices.empty())
	{
		UInt16 r = GetRand((UInt16) posIndices.size());
		EventScheduleMapData* pMapData = m_pData->npcData->npcMapData[posIndices[r]];
		if (pMapData)
		{
			*px = pMapData->x;
			*py = pMapData->y;
		}
	}
}

void CWorldEvent::OpenMap(CMap* pMap)
{
	UInt16 mapId = pMap->GetMap_id();
	const UInt16Vector &mapIdList = GetMapList();

	for (UInt16Vector::const_iterator it = mapIdList.begin();
		it != mapIdList.end(); ++it)
	{
		if (*it == mapId) {
			pMap->SetOpen(BOOLEAN_TRUE);
			break;
		}
	}
}

void CWorldEvent::CloseMap(CMap* pMap)
{
	UInt16 mapId = pMap->GetMap_id();
	const UInt16Vector &mapIdList = GetMapList();

	for (UInt16Vector::const_iterator it = mapIdList.begin();
		it != mapIdList.end(); ++it)
	{
		if (*it == mapId) {
			pMap->SetOpen(BOOLEAN_FALSE);
			break;
		}
	}
}

void CWorldEvent::UnmarkClosedMap()
{
	const UInt16Vector &mapIdList = GetMapList();
	for (UInt16Vector::const_iterator it = mapIdList.begin();
		it != mapIdList.end(); ++it)
	{
		CMap::RemoveClosedMap(*it);
	}
}

void CWorldEvent::MarkClosedMap()
{
	const UInt16Vector &mapIdList = GetMapList();
	for (UInt16Vector::const_iterator it = mapIdList.begin();
		it != mapIdList.end(); ++it)
	{
		CMap::AddClosedMap(*it);
	}
}

UInt32 CWorldEvent::GetTimeLeft()
{
	UInt32 now = gGlobal.GetServiceTime();
	return (GetEndTime() > now ? GetEndTime() - now : 0);
}