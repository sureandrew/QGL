//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
#include "Common/CallLuaFunc.h"
#include "Common/PathFinder.h"
//-- Self
#include "SimClient/Map.h"
//-- Library
#include "SimClient/Character.h"
#include "SimClient/MapNPC.h"
#include "SimClient/Configure.h"
#include "SimClient/Global.h"
#include "SimClient/Party.h"
#include "SimClient/QuestSystem.h"

//#define DEBUG_CHECK_VIEW		1

REGISTER_CLASSTYPE(CMap, Map_CLASSID);

PROTO_LuaPush(CMap)
PROTO_LuaPush(CCharacter)

void CMap::OnUpdateInfo(const UInt16 &map_id, const UInt8 &line_id, const String &name)
{
}


RPCResult CMap::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	return RPC_RESULT_OK;
}


RPCResult CMap::CB_GetCharInRange(LPCPEER pPeer, RPCContext &context, NetGroup * grp)
{
	DeleteNetGroup(grp);

	return RPC_RESULT_OK;
}

RPCResult CMap::CB_ObjectChanged(LPCPEER pPeer, RPCContext &context, NetGroup* grp,
								 const RPCNetIDVector &delIds,
								 const RPCNetIDVector &curIds)
{
	TRACE_ENSURE(grp);

	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("CB_ObjectChanged: count %d, del count %d"), 
		grp->GetCount(), delIds.size());

	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pCharacter == NULL) {
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CB_ObjectChanged Warning: [main character not exist]"));
		return RPC_RESULT_FAIL;
	}

#ifdef DEBUG_CHECK_VIEW
	String s;
	UInt16 checkCount = 0;
#endif
	CMapActor* actor;

	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		actor = grp->GetChild<CMapActor>(i);
		if (actor == NULL ||
			gGlobal.m_pSimClient && actor == gGlobal.m_pSimClient->m_pCharacter)
			continue;

#ifdef DEBUG_CHECK_VIEW
		if (actor->IsUpdated(CMapActor::appearedMask)) {
			s.AppendFormat(_T("%s%d, "), (actor->GetAppeared() ? _T("A") : _T("D")), 
				actor->GetNetId().objId);
			checkCount++;
		}
#endif // DEBUG_CHECK_VIEW
		MapActorMap::iterator itfind = m_objects.find(actor->GetNetId());
		if (itfind == m_objects.end())
		{
			if (actor->GetAppeared()) {
				TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("CB_ObjectChanged: Added actor netId %d,%d, ID %d"), 
					actor->GetNetId().srvId, actor->GetNetId().objId, actor->GetID());
				m_objects.insert(std::make_pair(actor->GetNetId(), actor));
			} else {
				TRACE_WARNDTL_5(GLOBAL_LOGGER, 
					_T("CB_ObjectChanged Warning: [disappear non-exist actor], netId %d,%d, map_id %d, pos %d,%d"), 
					actor->GetNetId().srvId, actor->GetNetId().objId, actor->GetMap_id(), 
					actor->GetCurPosX(), actor->GetCurPosY());
				//DeleteNetObject(actor);
				continue;
			}
		}
		else
			TRACE_ENSURE(itfind->second == actor);
		if (actor->GetClassID() == Character_CLASSID)
		{
			CCharacter* pchar = CastNetObject<CCharacter>(actor);
			if (pchar && pchar->IsUpdated(CMapActor::appearedMask)) {
				if (pchar->GetAppeared()) {
					TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, 
						_T("New Character Appeared: char_id %d, netId %d,%d, curPos %d,%d"), 
						pchar->GetChar_id(), pchar->GetNetId().srvId, pchar->GetNetId().objId, 
						pchar->GetCurPosX(), pchar->GetCurPosY());
				} else {
					TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, 
						_T("Character Disappeared: char_id %d, netId %d,%d, curPos %d,%d"), 
						pchar->GetChar_id(), pchar->GetNetId().srvId, pchar->GetNetId().objId, 
						pchar->GetCurPosX(), pchar->GetCurPosY());
				}
			}
		}
		else if (actor->GetClassID() == MapNPC_CLASSID)
		{
			CMapNPC* pnpc = CastNetObject<CMapNPC>(actor);
			if (pnpc && pnpc->IsUpdated(CMapActor::appearedMask)) {
				if (pnpc->GetAppeared()) {
					TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("New NPC Appeared: npc_id %d, netId %d,%d, curPos %d,%d"), 
						pnpc->GetNpc_uid(), pnpc->GetNetId().srvId, pnpc->GetNetId().objId,
						pnpc->GetCurPosX(), pnpc->GetCurPosY());
				} else {
					TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("NPC Disappeared: npc_id %d, netId %d,%d, curPos %d,%d"), 
						pnpc->GetNpc_uid(), pnpc->GetNetId().srvId, pnpc->GetNetId().objId,
						pnpc->GetCurPosX(), pnpc->GetCurPosY());
				}
			}
		}
	}

	DeleteNetGroup(grp);

	for (RPCNetIDVector::const_iterator it = delIds.begin(); it != delIds.end(); ++it)
	{
		MapActorMap::iterator it2 = m_objects.find(*it);
		if (it2 != m_objects.end()) {
#ifdef DEBUG_CHECK_VIEW
			s.AppendFormat(_T("R%d, "), (*it).objId);
			checkCount++;
#endif // DEBUG_CHECK_VIEW
			actor = it2->second;
			TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("CB_ObjectChanged: delete actor %d, netId %d,%d, objID %d, m_objects.size() %d"), 
				actor->GetActorId(), actor->GetNetId().srvId, actor->GetNetId().objId, actor->GetID(), m_objects.size());
			if (actor->GetClassID() == Character_CLASSID && gGlobal.m_pSimClient->m_pCharacter)
			{
				if (gGlobal.m_pSimClient->m_pCharacter->GetParty() &&
					gGlobal.m_pSimClient->m_pCharacter->GetParty()->FindMember(actor->GetActorId()))
				{
					TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("CB_ObjectChanged Warning: netId %d,%d, char_id %d deleted in same party of current char_id %d"), 
						it->srvId, it->objId, actor->GetActorId(),
						gGlobal.m_pSimClient->m_pCharacter->GetChar_id());
					continue;
				}
			}
			DeleteNetObject(actor);
			m_objects.erase(it2);
			//TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("CB_ObjectChanged: delete actor netId %d,%d, m_objects.size() %d"), it->srvId, it->objId, m_objects.size());
		} else {
			actor = FindNetObject<CMapActor>(*it);
			if (actor) {
				TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("CB_ObjectChanged: delete actor %d, netId %d,%d, objID %d, m_objects.size() %d"), 
					actor->GetActorId(), actor->GetNetId().srvId, actor->GetNetId().objId, actor->GetID(), m_objects.size());
				DeleteNetObject(actor);
			}
		}
	}

	RPCNetID myNetId = gGlobal.m_pSimClient->m_pCharacter->GetNetId();

#ifdef DEBUG_CHECK_VIEW
	if (checkCount > 0)
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CheckView: owner netId %d,%d, count %d, %s"), 
			myNetId.srvId, myNetId.objId, checkCount, s.c_str());
#endif // DEBUG_CHECK_VIEW

	// check if all current net Ids are really in view
	if (!curIds.empty()) {
		UInt16 mapId = gGlobal.m_pSimClient->m_pCharacter->GetMap_id();
		UInt16 myCurX = gGlobal.m_pSimClient->m_pCharacter->GetCurPosX();
		UInt16 myCurY = gGlobal.m_pSimClient->m_pCharacter->GetCurPosY();

		for (RPCNetIDVector::const_iterator it = curIds.begin(); it != curIds.end(); ++it)
		{
			const RPCNetID &netId = *it;
			if (myNetId == netId)
				continue;
			MapActorMap::iterator it2 = m_objects.find(netId);
			if (it2 == m_objects.end()) {
				TRACE_WARNDTL_5(GLOBAL_LOGGER, 
					_T("CB_ObjectChanged Warning: [missing actor], netId %d,%d, map_id %d, pos %d,%d"), 
					netId.srvId, netId.objId, mapId, myCurX, myCurY);
			} else {
				actor = it2->second;
				if (actor->GetAppeared())
					continue;
				TRACE_WARNDTL_8(GLOBAL_LOGGER, _F("CB_ObjectChanged Warning: [actor disappeared] actor %d, netId %d,%d, map_id %d, pos %d,%d, myPos %d,%d"), 
					actor->GetActorId(), actor->GetNetId().srvId, 
					actor->GetNetId().objId, mapId,
					actor->GetCurPosX(), actor->GetCurPosY(),
					myCurX, myCurY);
				if (actor->GetClassID() == Character_CLASSID) {
					CCharacter* pchar = CastNetObject<CCharacter>(actor);
					TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CB_ObjectChanged Warning: [actor disappeared2] char_id %d"),
						pchar->GetChar_id());
				} else if (actor->GetClassID() == MapNPC_CLASSID) {
					CMapNPC* pnpc = CastNetObject<CMapNPC>(actor);
					TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("CB_ObjectChanged Warning: [actor disappeared2] npc_uid %d, npc_id %d"),
						pnpc->GetNpc_uid(), pnpc->GetNpc_id());
				}
			}
		}
	}

	return RPC_RESULT_OK;
}

/*
RPCResult CMap::CB_ObjectAppeared(LPCPEER pPeer, RPCContext &context, NetObject * obj)
{
	TRACE_ENSURE(obj);

	m_objects.Add(obj);
	if (obj->GetClassID() == Character_CLASSID)
	{
		CCharacter* pchar = CastNetObject<CCharacter>(obj);
		if (pchar)
			TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("New Character Appeared: char_id %d, netId %d,%d, curPos %d,%d"), 
				pchar->GetChar_id(), pchar->GetNetId().srvId, pchar->GetNetId().objId, 
				pchar->GetCurPosX(), pchar->GetCurPosY());
	}
	else if (obj->GetClassID() == MapNPC_CLASSID)
	{
		CMapNPC* pnpc = CastNetObject<CMapNPC>(obj);
		if (pnpc)
			TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("New NPC Appeared: npc_id %d, netId %d,%d, curPos %d,%d"), 
				pnpc->GetNpc_uid(), pnpc->GetNetId().srvId, pnpc->GetNetId().objId,
				pnpc->GetCurPosX(), pnpc->GetCurPosY());
	}

	return RPC_RESULT_OK;
}

RPCResult CMap::CB_ObjectDisappeared(LPCPEER pPeer, RPCContext &context, NetObject * obj)
{
	TRACE_ENSURE(obj);

	m_objects.Remove(obj);
	if (obj->GetClassID() == Character_CLASSID)
	{
		CCharacter* pchar = CastNetObject<CCharacter>(obj);
		if (pchar)
			TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("Character Disappeared: char_id %d, netId %d,%d, curPos %d,%d"), 
				pchar->GetChar_id(), pchar->GetNetId().srvId, pchar->GetNetId().objId, 
				pchar->GetCurPosX(), pchar->GetCurPosY());
	}
	else if (obj->GetClassID() == MapNPC_CLASSID)
	{
		CMapNPC* pnpc = CastNetObject<CMapNPC>(obj);
		if (pnpc)
			TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("NPC Disappeared: npc_id %d, netId %d,%d, curPos %d,%d"), 
				pnpc->GetNpc_uid(), pnpc->GetNetId().srvId, pnpc->GetNetId().objId,
				pnpc->GetCurPosX(), pnpc->GetCurPosY());
	}
	DeleteNetObject(obj);

	return RPC_RESULT_OK;
}

RPCResult CMap::CB_GroupAppeared(LPCPEER pPeer, RPCContext &context, NetGroup * grp)
{
	TRACE_ENSURE(grp);
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("CB_GroupAppeared: count %d"), grp->GetCount());

	NetObject *obj;
	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		obj = grp->GetChild(i);
		m_objects.Add(obj);
		if (obj->GetClassID() == Character_CLASSID)
		{
			CCharacter* pchar = CastNetObject<CCharacter>(obj);
			if (pchar)
				TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("New Character Appeared: char_id %d, netId %d,%d, curPos %d,%d"), 
					pchar->GetChar_id(), pchar->GetNetId().srvId, pchar->GetNetId().objId, 
					pchar->GetCurPosX(), pchar->GetCurPosY());
		}
		else if (obj->GetClassID() == MapNPC_CLASSID)
		{
			CMapNPC* pnpc = CastNetObject<CMapNPC>(obj);
			if (pnpc)
				TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("New NPC Appeared: npc_id %d, netId %d,%d, curPos %d,%d"), 
					pnpc->GetNpc_uid(), pnpc->GetNetId().srvId, pnpc->GetNetId().objId,
					pnpc->GetCurPosX(), pnpc->GetCurPosY());
		}
	}

	// remove object after used
	DeleteNetGroup(grp, BOOLEAN_FALSE);

	return RPC_RESULT_OK;
}

RPCResult CMap::CB_GroupDisappeared(LPCPEER pPeer, RPCContext &context, NetGroup * grp)
{
	TRACE_ENSURE(grp);
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("CB_GroupDisappeared: count %d"), grp->GetCount());

	NetObject *obj;
	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		obj = grp->GetChild(i);
		m_objects.Remove(obj);
		if (obj->GetClassID() == Character_CLASSID)
		{
			CCharacter* pchar = CastNetObject<CCharacter>(obj);
			if (pchar)
				TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("Character Disappeared: char_id %d, netId %d,%d, curPos %d,%d"), 
					pchar->GetChar_id(), pchar->GetNetId().srvId, pchar->GetNetId().objId, 
					pchar->GetCurPosX(), pchar->GetCurPosY());
		}
		else if (obj->GetClassID() == MapNPC_CLASSID)
		{
			CMapNPC* pnpc = CastNetObject<CMapNPC>(obj);
			if (pnpc)
				TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("NPC Disappeared: npc_id %d, netId %d,%d, curPos %d,%d"), 
					pnpc->GetNpc_uid(), pnpc->GetNetId().srvId, pnpc->GetNetId().objId,
					pnpc->GetCurPosX(), pnpc->GetCurPosY());
		}
	}

	// remove object after used
	DeleteNetGroup(grp, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}
*/

RPCResult CMap::CB_GetJumpPoints(LPCPEER pPeer, RPCContext &context, const JumpInfoVector &jumpInfoList)
{
	m_jumpInfoList.clear();
	m_jumpInfoList = jumpInfoList;

	return RPC_RESULT_OK;
}

CMap::CMap()
{
	m_mapdb = NULL;
	m_blockData = NULL;
	m_pathFinder = NULL;
	m_width = 0;
	m_height = 0;
}

CMap::~CMap()
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Actor Deleted: count %d"), m_objects.size());	
	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		CMapActor *actor = it->second;
		TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, _F("Actor Deleted: actor %d, netId %d,%d, objID %d"), 
			actor->GetActorId(), it->first.srvId, it->first.objId, actor->GetID());
		DeleteNetObject(actor);
	}
	m_objects.clear();
}

Boolean CMap::LoadData()
{
	m_mapdb = gGlobal.GetMapDb(GetMap_id());
	if (m_mapdb == NULL)
		return BOOLEAN_FALSE;
	m_blockData = (UInt16*) m_mapdb->m_combinedblock;
	m_width = m_mapdb->m_zonedata_header->width;
	m_height = m_mapdb->m_zonedata_header->height;
	m_pathFinder = gGlobal.GetPathFinder(m_mapdb, GetMap_id());
	if (m_pathFinder == NULL)
		return BOOLEAN_FALSE;

	return BOOLEAN_TRUE;
}

/*
Boolean CMap::LoadData()
{
	if (m_mapdb && m_mapdb->m_basicinfo->m_mapid == GetMap_id())
		return true;

	if (!ImportClient_ZoneData(GetMap_id())) {
		return false;
	}

	m_blockData = (UInt16*) m_mapdb->m_combinedblock;
	m_width = m_mapdb->m_zonedata_header->width;
	m_height = m_mapdb->m_zonedata_header->height;
	m_pathFinder->SetMap(m_blockData, m_width, m_height);

	ImportClient_PFData(GetMap_id());

	return true;
}

Boolean CMap::ImportClient_ZoneData(UInt16 mapid)
{
	String dataFile;
	FILE *f0;

	dataFile.Format(_T("%sclient_MAP%08i_zonedata.dat"), gConfigure.zoneDataPath, mapid);

	// check if same map
	if (m_mapdb && m_mapdb->m_basicinfo->m_mapid == mapid)
		return BOOLEAN_TRUE;
	
	if (m_mapdb)
		SafeDeleteObject(m_mapdb);

	m_mapdb = SafeCreateObject(MapDB_Client);

    if ((f0 = _tfopen(dataFile.c_str(), _T("rb"))) != NULL)
	{
		m_mapdb->m_zonedata_header = SafeCreateObject(BinaryBlockInfoHeader);
		fread(m_mapdb->m_zonedata_header, 1, sizeof(BinaryBlockInfoHeader), f0);

		if (m_mapdb->m_combinedblock)
			SafeDeallocate(m_mapdb->m_combinedblock);
		m_mapdb->m_combinedblock = SafeAllocate(WORD, m_mapdb->m_zonedata_header->width * m_mapdb->m_zonedata_header->height);

		UInt32 zoneSize;
		fread(&zoneSize, 1, sizeof(UInt32), f0);

		WORD* pTempData = m_mapdb->m_combinedblock;
		BinaryClientZoneInfo* tempzone;
		for (UInt32 i = 0; i < zoneSize; i++ )
		{
			tempzone = SafeCreateObject(BinaryClientZoneInfo);
			fread(tempzone, 1, sizeof(BinaryClientZoneInfo), f0);
			m_mapdb->m_zoneinfo.push_back(tempzone);
		}
		ClientMapZoneData* blockData = NULL;
		WORD zoneList;
		UInt32 countZone;
		for(UInt i = 0; i < m_mapdb->m_zonedata_header->blocksizeinbyte; ++i)
		{
			blockData = SafeCreateObject(ClientMapZoneData);
			fread(&blockData->sBlockType, 1, sizeof(WORD), f0);
			*pTempData = blockData->sBlockType;
			pTempData ++;
			countZone = 0;
			fread(&countZone, 1, sizeof(UInt32), f0);
			blockData->ZoneIdCount = countZone;
			blockData->ZoneIdLst = SafeAllocate(WORD, blockData->ZoneIdCount);
			for (UInt32 j = 0; j < countZone; j++)
			{
				fread(&zoneList, 1, sizeof(WORD), f0);
				blockData->ZoneIdLst[j] = zoneList;
			}
			m_mapdb->m_zonedata.push_back(blockData);
		}
		fclose(f0);

		return BOOLEAN_TRUE;
	}
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CMap ImportClient_ZoneData Error: cannot open zone data file %d"),
			dataFile.c_str());
	}
	return BOOLEAN_FALSE;
}

Boolean CMap::ImportClient_PFData(UInt32 mapid)
{
	String dataFile;
	PathFindLoadData pathFindData;
	UInt32 numGuide = 0;

	dataFile.Format(_T("%sclient_MAP%08i_pathfind.dat"), gConfigure.zoneDataPath, mapid);

	FILE *f0;
	if ((f0 = _tfopen(dataFile.c_str(), _T("rb"))) != NULL)
	{
		DefaultPFHeader header;
		fread(&header, 1, sizeof(DefaultPFHeader), f0);

		UInt16* ubuf = NULL;
		Real32* fbuf = NULL;
		if( header.sizeDistGraph > 0 )
		{
			fbuf = SafeAllocate(Real32, header.sizeDistGraph);
			fread(fbuf, sizeof(Real32), header.sizeDistGraph, f0);
		}
		if( header.sizeLinkGraph > 0 )
		{
			ubuf = SafeAllocate(UInt16, header.sizeLinkGraph);
			fread(ubuf, sizeof(UInt16), header.sizeLinkGraph, f0);
		}
		fclose(f0);
		pathFindData.distData = fbuf;
		pathFindData.linkData = ubuf;
		numGuide = header.numGuildPoint;
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CMap ImportClient_PFData Warning: cannot open pathfind data file %d"),
			dataFile.c_str());
	}

	// pre-calculate if neccessary
	if (numGuide == 0)
		m_pathFinder->PreCalculate();
	else
		m_pathFinder->PreCalculate(pathFindData.weightData, pathFindData.distData, pathFindData.linkData);

	//return 0;
	return BOOLEAN_TRUE;			// if cannot load path finder data, do dynamic pre-calculation
}
*/

BlockPos* CMap::GetGuidePoint(int i)
{
	if (m_pathFinder && i >= 0 && i < m_pathFinder->GetGuideCount())
		return &m_pathFinder->GetGuidePoint(i);
	else
		return NULL;
}

UInt16 CMap::GetGuideCount()
{
	if (m_pathFinder)
		return m_pathFinder->GetGuideCount();
	else
		return 0;
}

CCharacter* CMap::FindCharacter(UInt32 char_id)
{
	NetObject* obj;
	CCharacter* pchar;
	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		obj = it->second;
		if (obj && obj->GetClassID() == Character_CLASSID) {
			pchar = CastNetObject<CCharacter>(obj);
			if (pchar && pchar->GetChar_id() == char_id)
				return pchar;
		}
	}
	return NULL;
}

CCharacter* CMap::FindEnemyCharacterNearby()
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pCharacter == NULL)
		return NULL;
	gGlobal.m_pSimClient->m_pCharacter->GetTitle();
	NetObject* obj;
	CCharacter* pchar;
	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		obj = it->second;
		if (obj && obj->GetClassID() == Character_CLASSID) {
			pchar = CastNetObject<CCharacter>(obj);
			if (pchar && pchar->GetEnablePK() && 
				(pchar->GetAction() != CCharacter::ST_BATTLE && pchar->GetAction() != CCharacter::ST_NEEDHELP)
				&& (!IsGuildNameIdentical(gGlobal.m_pSimClient->m_pCharacter->GetTitle(), pchar->GetTitle())))
                return pchar;
		}
	}
	return NULL;
}

Boolean CMap::IsGuildNameIdentical(String name1, String name2)
{
	String guildName1, guildName2;
	for (int i = 0; i < name1.GetLength(); ++i)
	{
		Char temp = name1.GetAt(i);
		if (temp == 183)// if the characte don't equal to the "dot" symbol
			break;
		guildName1.push_back(temp);
	}
	for (int i = 0; i < name2.GetLength(); ++i)
	{
		Char temp = name2.GetAt(i);
		if (temp == 183)// if the characte don't equal to the "dot" symbol
			break;
		guildName2.push_back(temp);
	}	
	if (guildName1.GetLength() != name1.GetLength() && guildName2.GetLength() != name2.GetLength()
		&& (_tcsicmp((Char*) guildName1.c_str(), (Char*) guildName2.c_str()) == 0))
        return BOOLEAN_TRUE;

	return BOOLEAN_FALSE;
}


CCharacter* CMap::FindGuildMemberNearby()
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pCharacter == NULL)
		return NULL;
	gGlobal.m_pSimClient->m_pCharacter->GetTitle();
	NetObject* obj;
	CCharacter* pchar;
	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		obj = it->second;
		if (obj && obj->GetClassID() == Character_CLASSID) {
			pchar = CastNetObject<CCharacter>(obj);
			if (pchar && pchar->GetEnablePK() &&
				(pchar->GetAction() != CCharacter::ST_BATTLE && pchar->GetAction() != CCharacter::ST_NEEDHELP))
			{
                String myTitle = gGlobal.m_pSimClient->m_pCharacter->GetTitle();
				String targetTitle = pchar->GetTitle();
                Char delim = 183; // the "dot" symbol
				Char *myGuildName = _tcstok((Char*)myTitle.c_str() , &delim);
				Char *targetGuildName = _tcstok((Char*)targetTitle.c_str() , &delim);	
				if (myGuildName && targetGuildName && (_tcsicmp(myGuildName, targetGuildName) == 0))
                    return pchar;
			}
		}
	}
	return NULL;
}


UInt32 CMap::FindRandomHawker()
{
	NetObject* obj;
	CCharacter* pchar;
	UInt32Vector hawkerList;
	hawkerList.clear();
	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		obj = it->second;
		if (obj && obj->GetClassID() == Character_CLASSID)
		{
			pchar = CastNetObject<CCharacter>(obj);
			if (pchar && (pchar->GetAction() == CMapActor::ST_HAWK))
			{
				hawkerList.push_back(pchar->GetChar_id());
			}
		}
	}
	if (hawkerList.size() > 0)
	{
		UInt16 hawk = GetRand((UInt16) hawkerList.size());
		return hawkerList[hawk];
	}
	else
		return 0;
	
}

CMapNPC* CMap::FindNpc(UInt32 npc_id)
{
	NetObject* obj;
	CMapNPC* pnpc;
	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		obj = it->second;
		if (obj && obj->GetClassID() == MapNPC_CLASSID) {
			pnpc = CastNetObject<CMapNPC>(obj);
			if (pnpc && pnpc->GetNpc_uid() == npc_id)
				return pnpc;
		}
	}
	return NULL;
}

UInt32 CMap::FindNpcUID(UInt32 npc_id)
{

	NetObject* obj;
	CMapNPC* pnpc;
	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		obj = it->second;
		if (obj && obj->GetClassID() == MapNPC_CLASSID) {
			pnpc = CastNetObject<CMapNPC>(obj);
			if (pnpc && pnpc->GetNpc_id() == npc_id)
				return pnpc->GetNpc_uid();
		}
	}
	return 0;
}

UInt32Vector CMap::FindNpcNearby()
{
	UInt32Vector npcIds;
	NetObject* obj;
	CMapNPC* pnpc;

	if (m_objects.size() > 0)
	{
		for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end();
			++it)
		{
			obj = it->second;
			if (obj && obj->GetClassID() == MapNPC_CLASSID) 
			{
				pnpc = CastNetObject<CMapNPC>(obj);
				npcIds.push_back(pnpc->GetNpc_uid());
			}
		}
		
	}

	return npcIds;
}

UInt16 CMap::GetMapLineID()
{
	return GetLine_id();

}

// get nearby character within radius block
// and type, 0=all, 1=single, 2=in party, 3=in battle, 4=battle help, 5=pk enable
CCharacter* CMap::GetNearCharacter(UInt16 x, UInt16 y, UInt16 radius, UInt8 type)
{
	NetObject* obj;
	CCharacter* pchar;
	UInt16 d2, r2;

	r2 = radius * radius;
	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		obj = it->second;
		if (obj && obj->GetClassID() == Character_CLASSID) {
			pchar = CastNetObject<CCharacter>(obj);
			if (pchar && pchar->GetAppeared() && pchar->GetChar_id() > 0 &&
				(type == 0 ||
				type == 1 && !pchar->HasParty() ||
				type == 2 && pchar->HasParty() ||
				type == 3 && 
					(pchar->GetAction() == CCharacter::ST_BATTLE ||
					 pchar->GetAction() == CCharacter::ST_NEEDHELP) ||
				type == 4 && pchar->GetAction() == CCharacter::ST_NEEDHELP ||
				type == 5 && pchar->GetEnablePK() && 
					(pchar->GetAction() != CCharacter::ST_BATTLE && 
					 pchar->GetAction() != CCharacter::ST_NEEDHELP))
				)
			{
				d2 = (pchar->GetCurPosX() - x) * (pchar->GetCurPosX() - x) +
					(pchar->GetCurPosY() - y) * (pchar->GetCurPosY() - y);
				if (d2 <= radius)
					return pchar;
			}
		}
	}

	return NULL;
}

RPCResult CMap::GetNearCharacter(RPCContext &context)
{
	TRACE_ENSURE(gGlobal.m_pSimClient);

	PROCEDURE_START_METHOD4(CMap, GetNearCharacter, this, context, UInt16, x, UInt16, y, 
							UInt16, radius, UInt8, type)

		GetCharInRange(gGlobal.m_pSimClient->m_pProxyPeer, context, x, y, radius);

	PROCEDURE_WAIT_OBJ1(1, Map, CB_GetCharInRange, pmap, NetGroup*, grp)

		// find nearest neighbor character
		if (gGlobal.m_pSimClient->m_bRobot) {
			NetObject* obj;
			CCharacter* neighbor = NULL;
			CCharacter* pchar;
			UInt16 minRange = UINT16_MAX_VALUE;
			UInt16 d2;
			for (UInt16 i = 0; i < grp->GetCount(); i++) {
				obj = grp->GetChild(i);
				if (obj && obj->GetClassID() == Character_CLASSID) {
					pchar = (CCharacter*) obj;
					if (pchar) {
						d2 = (pchar->GetCurPosX() - x) * (pchar->GetCurPosX() - x) +
							(pchar->GetCurPosY() - y) * (pchar->GetCurPosY() - y);
						if (d2 < minRange) {
							neighbor = pchar;
							minRange = d2;
						}
					}
				}
			}
			UInt16 ri = GetRand(grp->GetCount());		// get random 
			if (neighbor) {
				CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "GotNearCharacter", String(),
					gGlobal.m_pSimClient->m_pMap, gGlobal.m_pSimClient->m_pCharacter, neighbor);
			} else {
				CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "GotNearCharacter", String(),
					gGlobal.m_pSimClient->m_pMap, gGlobal.m_pSimClient->m_pCharacter);
			}
		}
		DeleteNetGroup(grp);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_OBJ2(Map, CB_Error, pmap, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("GetAccount: LoginService::CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_FAIL;
}

void CMap::ShowObjects()
{
	if (gGlobal.m_pSimClient->m_pCharacter == NULL)
		return; 

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Show objects in map %d, line %d, curPos %d,%d:"),
		GetMap_id(), GetLine_id(), gGlobal.m_pSimClient->m_pCharacter->GetCurPosX(),
		gGlobal.m_pSimClient->m_pCharacter->GetCurPosY());

	int			i = 0;
	CMapActor*	actor;
	CCharacter* pchar;
	CMapNPC*	pnpc;

	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		actor = it->second;
		if (actor == NULL || !actor->GetAppeared()) continue;
		if (actor->GetClassID() == Character_CLASSID)
		{
			pchar = CastNetObject<CCharacter>(actor);
			TRACE_ENSURE_0(pchar, _T("null Character"));
			pchar->ShowSummary(i);
		}
		else if (actor->GetClassID() == MapNPC_CLASSID)
		{
			pnpc = CastNetObject<CMapNPC>(actor);
			TRACE_ENSURE_0(pnpc, _T("null MapNPC"));
			pnpc->ShowSummary(i);
			if (gGlobal.m_pSimClient->m_pQuestSys)
				gGlobal.m_pSimClient->m_pQuestSys->ShowNpcHint(pnpc);
		}
		else
		{
			TRACE_INFODTL_4(GLOBAL_LOGGER, _F("%3d: INVALID OBJECT: classID %d, netId %d,%d\n"),
				i, actor->GetClassID(), actor->GetNetId().srvId, actor->GetNetId().objId);
		}

		++i;
	}
}

