//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Stub/Logic/Map.h"
//-- Library
#include "Stub/Logic/Character.h"
#include "Stub/Logic/MapNPC.h"
#include "Stub/Logic/BatCharacter.h"
#include "Stub/Logic/QuestSystem.h"
#include "Stub/Service/MapService.h"
#include "Proxy/Service/WorldService_proxy.h"
#include "Proxy/Service/MessageService_proxy.h"
#include "MapService/Global.h"
#include "MapService/MapApplication.h"
#include "MapService/Math_QGL.h"
#include "MapService/ViewManager.h"
#include "MapService/Instance.h"
#include "Resource/ServerResManager.h"
#include "Resource/resEventschedule.h"
#include "Resource/ResNpc.h"
#include "MapService/Configure.h"
#include "Stub/Logic/Party.h"

#define JUMP_POINT_RADIUS	80
#define REGION_WIDTH		16		// 512 pixel
#define REGION_HEIGHT		64		// 512 pixel
#define GRIDX_EXTEND		1		// extend grid x for view area
#define GRIDY_EXTEND		1		// extend grid y for view area

#define MAX_NPC_TRIAL 1000

#define INCOGNITBOX_MODIFIED_POSY 51

#define minGridX(gx)		(gx > GRIDX_EXTEND ? gx - GRIDX_EXTEND : 0)
#define maxGridX(gx)		(gx < (m_regionCol - GRIDX_EXTEND) ? gx + GRIDX_EXTEND : m_regionCol - 1)
#define minGridY(gy)		(gy > GRIDY_EXTEND ? gy - GRIDY_EXTEND : 0)
#define maxGridY(gy)		(gy < (m_regionRow - GRIDY_EXTEND) ? gy + GRIDY_EXTEND : m_regionRow - 1)
#define POS_ID(x, y)		(((UInt32)(x) << 16) + (y))

#define REGION_INVALID		UINT16_MAX_VALUE
#define REGION_OUTSIDE		(UINT16_MAX_VALUE - 1)

PROTO_LuaPush(CCharacter)
//#ifdef _DEBUG
//	#define UPDATE_EVENT_PERIOD 100
//#else
//	#define UPDATE_EVENT_PERIOD 3000
//#endif

/*
#define SCREEN_WIDTH		800		// screen area
#define SCREEN_HEIGHT		640
#define VIEW_WIDTH			1024	// view area
#define VIEW_HEIGHT			768
#define BLOCK_WIDTH			32		// pixel per block
#define BLOCK_HEIGHT		16
#define HALF_VIEW_WIDTH		16		// VIEW_WIDTH / BLOCK_WIDTH / 2		(512 pixel)
#define HALF_VIEW_HEIGHT	64		// VIEW_HEIGHT / BLOCK_WIDTH		(512 pixel)

#define minGridX(x)		(x > HALF_VIEW_WIDTH ? (x - HALF_VIEW_WIDTH) / REGION_WIDTH : 0)
#define maxGridX(x)		((x < (m_width - HALF_VIEW_WIDTH)) ? (x + HALF_VIEW_WIDTH) / REGION_WIDTH : m_regionCol - 1)
#define minGridY(y)		(y > HALF_VIEW_HEIGHT ? (y - HALF_VIEW_HEIGHT) / REGION_HEIGHT : 0)
#define maxGridY(y)		((y < (m_height - HALF_VIEW_HEIGHT)) ? (y + HALF_VIEW_HEIGHT) / REGION_HEIGHT : m_regionRow - 1)
*/

REGISTER_CLASSTYPE(CMap, Map_CLASSID);

UInt32 gNpcUid = 1;		// global npc id counter

CMap::ActorIdType::ActorIdType(CMapActor* actor)
{
	classId = actor->GetClassID();
	actorId = actor->GetActorId();
	
}

CMap::CMap()
{
	m_width = 0;
	m_height = 0;
	m_regionCol = 0;
	m_regionRow = 0;
	m_regionTotal = 0;
	m_regions = NULL;
	m_pathfinder = NULL;
	m_bOpen = BOOLEAN_TRUE;
	m_guildWarID1 = 0;
	m_guildWarID2 = 0;
	m_guildWarWinID = 0;
	m_guildWarKick = false;
	m_inGuildWar = false;
}

CMap::~CMap()
{
	// remove all new actors
	ActorIdMap cloneList = m_chgActors;
	for (ActorIdMap::iterator it = cloneList.begin(); 
		it != cloneList.end(); ++it)
	{
		CMapActor* actor = it->second;
		if (actor && actor->m_curRegion == REGION_INVALID) {
			if (actor->IsSubClass(MapNPC_CLASSID))
				TRACE_INFODTL_2(GLOBAL_LOGGER, 
				_T("CMap::~CMap: [NPC Removal OnLeaveMap] netid=(%d:%d)"), 
					actor->GetNetId().srvId, actor->GetNetId().objId);
			actor->OnLeaveMap();
		}

	}
	cloneList.clear();

	// delete all disappeared npc
	for (NpcActorMap::iterator it = m_npcActors.begin(); 
		it != m_npcActors.end(); ++it)
	{
		DeleteNetObject(it->second);
	}
	m_npcActors.clear();

	DeleteRegions();
}

void CMap::DeleteRegions()
{
	for (UInt16 i = 0; i < m_regionTotal; i++)
	{
		/*
		Region::MapActorMap actors;
		m_regions[i]->m_actors.swap(actors);
		for (Region::MapActorMap::iterator it = actors.begin(); it != actors.end(); ++it)
		{
			//if (it->second->GetClassID() == Character_CLASSID) {
			//	CCharacter *pchar = CastNetObject<CCharacter>(it->second);
			//	if (pchar)
			//		pchar->ClearSessionId();
			//}
			//DeleteNetObject(it->second);
			pActor = it->second;
			if (pActor && pActor->GetClassID() == MapNPC_CLASSID)
				DeleteNetObject(pActor);
		}
		*/
		SafeDeleteObject(m_regions[i]);
	}
	SafeDeallocate(m_regions);
	m_actors.clear();
}

INLINE UInt16 CMap::RegionDistance(UInt16 region1, UInt16 region2)
{
	//m_mapdb->m_basicinfo->m_maptype
	UInt16 d = (region1 < region2 ? region2 - region1 : region1 - region2);
	UInt16 c = (region1 < region2 ? region1 % m_regionCol : region2 % m_regionCol);

	if (d < (1 + (c != m_regionCol - 1)))
		return d;

	if (d >= (m_regionCol - (c != 0)) && d <= (m_regionCol + (c != m_regionCol - 1)))
		return 1;
	else
		return 2;
}

RPCResult CMap::GetCharInRange(LPCPEER pPeer, RPCContext &context, 
							   const UInt16 &centerX, const UInt16 &centerY, 
							   const UInt16 &range)
{
	return RPC_RESULT_OK;
}

RPCResult CMap::GetJumpPoints(LPCPEER pPeer, RPCContext &context)
{
	BinaryServerJumppointInstance* jp = NULL;
	JumpInfoVector jumpInfoList;
	JumpInfo jpInfo;

	for (JpInstList::iterator it = m_jpList.begin(); it != m_jpList.end(); ++it)
	{
		jp = *it;
		jpInfo.jpPosX = jp->m_posx;
		jpInfo.jpPosY = jp->m_posy;
		jpInfo.dstMapId = jp->DesMapIdNum;
		jpInfo.dstPosX = jp->DesBlockX;
		jpInfo.dstPosY = jp->DesBlockY;
		jumpInfoList.push_back(jpInfo);
	}
	CB_GetJumpPoints(pPeer, context, jumpInfoList);

	return RPC_RESULT_OK;
}

void CMap::MoveNotify(CMapActor *actor, UInt16 x1, UInt16 y1, UInt16 x2, UInt16 y2)
{
	UInt16 gx2 = x2 / REGION_WIDTH;
	UInt16 gy2 = y2 / REGION_HEIGHT;
	UInt16 newRegion = m_regionCol * gy2 + gx2;

	if (actor->m_curRegion == newRegion ||				// region no change
		actor->m_curRegion == REGION_INVALID)			// still can't move
		return;
		
	// out of range
	if (gx2 >= m_regionCol || gy2 >= m_regionRow || newRegion >= m_regionTotal)
	{
		TRACE_WARNDTL_8(GLOBAL_LOGGER, _F("MoveNotify Warning: actor %d:%d is out of range, curPos %d:%d newPos %d:%d > mapSize %d:%d"),
			actor->GetNetId().srvId, actor->GetNetId().objId, x1, y1, x2, y2, m_width, m_height);
		return;
	}

	actor->m_newRegion = newRegion;
	m_chgActors[ActorIdType(actor)] = actor;
}

void CMap::AddNotify(CMapActor *actor)
{
	if (actor == NULL) return;

	UInt16 gx1 = actor->GetCurPosX() / REGION_WIDTH;
	UInt16 gy1 = actor->GetCurPosY() / REGION_HEIGHT;

	if (gx1 >= m_regionCol || gy1 >= m_regionRow)		// out of range
	{
		TRACE_WARNDTL_6(GLOBAL_LOGGER, _F("AddNotify Warning: [out of range] actor %d:%d, curPos %d:%d > mapSize %d:%d"),
			actor->GetNetId().srvId, actor->GetNetId().objId, actor->GetCurPosX(), actor->GetCurPosY(), 
			m_width, m_height);
		return;
	}
	if (actor->m_curRegion != REGION_INVALID) {
		TRACE_WARNDTL_4(GLOBAL_LOGGER, 
			_T("MoveToRegion Warning: [already in region], actor %d:%d, oldRegion %d, newRegion %d"),
			actor->GetNetId().srvId, actor->GetNetId().objId, actor->m_curRegion, 
			actor->m_newRegion);
		return;
	} else if (actor->m_newRegion != REGION_INVALID) {
		TRACE_WARNDTL_4(GLOBAL_LOGGER, 
			_T("MoveToRegion Warning: [already added], actor %d:%d, oldRegion %d, newRegion %d"),
			actor->GetNetId().srvId, actor->GetNetId().objId, actor->m_curRegion, 
			actor->m_newRegion);
	}

	actor->m_newRegion = m_regionCol * gy1 + gx1;
	m_chgActors[ActorIdType(actor)] = actor;
}

void CMap::RemoveNotify(CMapActor *actor)
{
	ActorIdMap::iterator itfind = m_chgActors.find(ActorIdType(actor));
	if (itfind != m_chgActors.end())
		m_chgActors.erase(itfind);
	if (actor->m_curRegion != REGION_INVALID)
		RemoveFromRegion(actor);	// directly remove actor from region
}

void CMap::MoveToRegion(CMapActor *actor)
{
	if (actor->m_newRegion >= m_regionTotal ||
		actor->m_curRegion >= m_regionTotal)					// out of range
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, 
			_T("MoveToRegion Warning: [region out of range], actor %d:%d, oldRegion %d, newRegion %d"),
			actor->GetNetId().srvId, actor->GetNetId().objId, actor->m_curRegion, 
			actor->m_newRegion);
		return;
	} else if (actor->m_newRegion == actor->m_curRegion) {		// same region
		TRACE_WARNDTL_4(GLOBAL_LOGGER, 
			_T("MoveToRegion Warning: [same old and new region], actor %d:%d, oldRegion %d, newRegion %d"),
			actor->GetNetId().srvId, actor->GetNetId().objId, actor->m_curRegion, 
			actor->m_newRegion);
		return;
	}

	// character view also changed
	UInt16 gx1 = actor->m_curRegion % m_regionCol;
	UInt16 gy1 = actor->m_curRegion / m_regionCol;
	UInt16 gx2 = actor->m_newRegion % m_regionCol;
	UInt16 gy2 = actor->m_newRegion / m_regionCol;
	UInt16 min_gx1 = minGridX(gx1);
	UInt16 max_gx1 = maxGridX(gx1);
	UInt16 min_gy1 = minGridY(gy1);
	UInt16 max_gy1 = maxGridY(gy1);
	UInt16 min_gx2 = minGridX(gx2);
	UInt16 max_gx2 = maxGridX(gx2);
	UInt16 min_gy2 = minGridY(gy2);
	UInt16 max_gy2 = maxGridY(gy2);

	// first remove actor from current region
	m_regions[actor->m_curRegion]->RemoveActor(actor);

	// view regions no change, only actor region changed
	if (min_gx1 == min_gx2 && max_gx1 == max_gx2 && min_gy1 == min_gy2 && max_gy1 == max_gy2)
	{
		//TRACE_WARNDTL_4(GLOBAL_LOGGER, 
		//	_T("MoveToRegion Warning: [view regions no change] actor %d:%d, oldRegion %d, newRegion %d"),
		//	actor->GetNetId().srvId, actor->GetNetId().objId, actor->m_curRegion, 
		//	actor->m_newRegion);
		m_regions[actor->m_newRegion]->AddActor(actor);
		return;
	}

	//TRACE_INFODTL_3(GLOBAL_LOGGER, _F("MoveToRegion [view regions changed] actor %d, oldRegion %d, newRegion %d"),
	//	actor->GetID(), actor->m_curRegion, actor->m_newRegion);

	//TRACE_INFODTL_8(GLOBAL_LOGGER, _F("MoveToRegion [record] min_gx1 %d, max_gx1 %d, min_gy1 %d, max_gy1 %d, min_gx2 %d, max_gx2 %d, min_gy2 %d, max_gy2 %d"),
	//	min_gx1, max_gx1, min_gy1, max_gy1, min_gx2, max_gx2, min_gy2, max_gy2);

	Region* pRegion;
	CMapActor* actor2;
	CCharacter* pchar = CastNetObject<CCharacter>(actor);
	CCharacter* pchar2;

	// remove character view
	for (UInt16 gy = min_gy1; gy <= max_gy1; gy++)
	{
		for (UInt16 gx = min_gx1; gx <= max_gx1; gx++)
		{
			if (gx >= min_gx2 && gx <= max_gx2 &&		// skip for intersected regions
				gy >= min_gy2 && gy <= max_gy2)
				continue;

			pRegion = m_regions[m_regionCol * gy + gx];
			if (pRegion->m_actors.empty())
			{
				//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("MoveToRegion (1) [region empty] actor %d, region %d"),
				//	actor->GetID(), pRegion->m_index);
				continue;
			}

			for (Region::MapActorMap::iterator it = pRegion->m_actors.begin(); 
				it != pRegion->m_actors.end(); ++it)
			{
				actor2 = it->second;
				if (actor == actor2)
				{
					TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("MoveToRegion Error: (1) [actor == actor2] actor %d, actor2 %d"),
						actor, actor2);
					continue;
				}

				// no need to remove view cases:
				// case 1: two actors can see each other in new view
				// case 2: two actors haven't seen each other before
				if (actor2->m_newRegion != REGION_INVALID &&
					RegionDistance(actor->m_newRegion, actor2->m_newRegion) < 2 ||	// case 1
					RegionDistance(actor->m_curRegion, actor2->m_curRegion) > 1)	// case 2
				    continue;
				if (pchar)
				{
					// if source is character, just remove any actor to his view
					pchar->m_viewManager->RemoveView(actor2, BOOLEAN_FALSE);
				}
				else if (actor2->GetClassID() == Character_CLASSID)
				{
					// if source is npc and target is character, remove target's view
					pchar2 = CastNetObject<CCharacter>(actor2);
					if (pchar2)
						pchar2->m_viewManager->RemoveView(actor, BOOLEAN_FALSE);
				}
			}
		}
	}

	// add character view
	for (UInt16 gy = min_gy2; gy <= max_gy2; gy++)
	{
		for (UInt16 gx = min_gx2; gx <= max_gx2; gx++)
		{
			if (gx >= min_gx1 && gx <= max_gx1 &&		// skip for intersected regions
				gy >= min_gy1 && gy <= max_gy1)
				continue;

			pRegion = m_regions[m_regionCol * gy + gx];
			if (pRegion->m_actors.empty())
			{
				//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("MoveToRegion (2) [region empty] actor %d, region %d"),
				//	actor->GetID(), pRegion->m_index);
				continue;
			}

			for (Region::MapActorMap::iterator it = pRegion->m_actors.begin(); it != pRegion->m_actors.end(); ++it)
			{
				actor2 = it->second;
				
				if (actor == actor2)
				{
					TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("MoveToRegion (2) [actor == actor2] actor %d, actor2 %d"),
						actor, actor2);
					continue;
				}

				// no need to add view cases:
				// case 1: two actors can't see each other in new view
				// case 2: two actors have seen each other before
				if (actor2->m_newRegion != REGION_INVALID &&
					RegionDistance(actor->m_newRegion, actor2->m_newRegion) > 1 ||	// case 1
					RegionDistance(actor->m_curRegion, actor2->m_curRegion) < 2)	// case 2
				    continue;
				if (pchar)
				{
					// if source is character, just add any actor to his view
					pchar->m_viewManager->AddView(actor2);
				}
				else if (actor2->GetClassID() == Character_CLASSID)
				{
					// if source is npc and target is character, add target's view
					pchar2 = CastNetObject<CCharacter>(actor2);
					if (pchar2)
						pchar2->m_viewManager->AddView(actor);
				}
			}
		}
	}

	// final add actor to new region
	m_regions[actor->m_newRegion]->AddActor(actor);
}

void CMap::AddToRegion(CMapActor *actor)
{
	UInt16 gx1 = actor->GetCurPosX() / REGION_WIDTH;
	UInt16 gy1 = actor->GetCurPosY() / REGION_HEIGHT;

	if (gx1 >= m_regionCol || gy1 >= m_regionRow)		// out of range
	{
		TRACE_WARNDTL_6(GLOBAL_LOGGER, _F("AddToRegion Warning: actor %d:%d is out of range, curPos %d:%d > mapSize %d:%d"),
			actor->GetNetId().srvId, actor->GetNetId().objId, actor->GetCurPosX(), actor->GetCurPosY(), m_width, m_height);
		return;
	}

	//TRACE_INFODTL_3(GLOBAL_LOGGER, _F("AddToRegion [ready] actor %d, oldRegion %d, newRegion %d"),
	//	actor->GetID(), actor->m_curRegion, actor->m_newRegion);

	// add character view
	UInt16 min_gx1 = minGridX(gx1);
	UInt16 max_gx1 = maxGridX(gx1);
	UInt16 min_gy1 = minGridY(gy1);
	UInt16 max_gy1 = maxGridY(gy1);
	Region* pRegion;
	CCharacter* pchar = CastNetObject<CCharacter>(actor);
	CCharacter* pchar2;
	CMapActor* actor2;

	for (UInt16 gy = min_gy1; gy <= max_gy1; gy++)
	{
		for (UInt16 gx = min_gx1; gx <= max_gx1; gx++)
		{
			pRegion = m_regions[m_regionCol * gy + gx];
			if (pRegion->m_actors.empty())
				continue;
			for (Region::MapActorMap::iterator it = pRegion->m_actors.begin(); 
				it != pRegion->m_actors.end(); ++it)
			{
				actor2 = it->second;
				if (actor2 == actor)
					continue;
				//TRACE_ENSURE(actor2->GetNetId().srvId == GetNetId().srvId);
				// remove deleted actor
				while (actor2 == NULL || actor2->GetNetId().srvId != GetNetId().srvId)
				{
					TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("AddToRegion Warning: actor %d deleted in region %d"), actor2, pRegion->m_index);
					it = pRegion->m_actors.erase(it);
					actor2 = it->second;
				}

				if (pchar)
				{
					// if source is character, just add any actor to his view.
					pchar->m_viewManager->AddView(actor2);
				}
				else if (actor2->GetClassID() == Character_CLASSID)
				{
					// if source is npc and target is character then add source to target's view.
					pchar2 = CastNetObject<CCharacter>(actor2);
					if (pchar2)
						pchar2->m_viewManager->AddView(actor);
				}
			}
		}
	}

	TRACE_INFODTL_2(GLOBAL_LOGGER, 
		_T("CMap::AddToRegion: [AddActor] netid=(%d:%d)"), 
		actor->GetNetId().srvId, actor->GetNetId().objId);

	// add actor to region
	actor->m_curRegion = m_regionCol * gy1 + gx1;
	actor->m_newRegion = REGION_INVALID;
	m_regions[actor->m_curRegion]->AddActor(actor);

	// jump reborn after entered a closed map
	if (pchar && !m_bOpen)
		pchar->JumpReborn();
}

void CMap::RemoveFromRegion(CMapActor *actor)
{
	// region out of range
	if (actor->m_curRegion >= m_regionTotal)
	{
		TRACE_WARNDTL_8(GLOBAL_LOGGER, _F("RemoveFromRegion Warning: [region out of range], actor %d:%d, cur_region %d, region_total %d, curPos %d:%d, mapSize %d:%d"),
			actor->GetNetId().srvId, actor->GetNetId().objId, actor->m_curRegion, m_regionTotal, actor->GetCurPosX(), actor->GetCurPosY(), 
			m_width, m_height);
		return;
	}
	
	//TRACE_INFODTL_3(GLOBAL_LOGGER, _F("RemoveFromRegion [ready] actor %d, oldRegion %d, newRegion %d"),
	//	actor->GetID(), actor->m_oldRegion, actor->m_newRegion);

	UInt16 gx1 = actor->m_curRegion % m_regionCol;
	UInt16 gy1 = actor->m_curRegion / m_regionCol;
	UInt16 min_gx1 = minGridX(gx1);
	UInt16 max_gx1 = maxGridX(gx1);
	UInt16 min_gy1 = minGridY(gy1);
	UInt16 max_gy1 = maxGridY(gy1);
		
	// remove actor from current region
	m_regions[actor->m_curRegion]->RemoveActor(actor);

	if (actor->IsSubClass(Character_CLASSID))
	{
		CCharacter* pchar = CastNetObject<CCharacter>(actor);
		if (pchar)
			pchar->m_viewManager->RemoveAllView();
	}
	else if (actor->IsSubClass(MapNPC_CLASSID))
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, 
			_T("CMap::RemoveFromRegion : [NPC Removal RemoveFromRegion] netid=(%d:%d)"), 
			actor->GetNetId().srvId, actor->GetNetId().objId);

		CMapNPC * pnpc = CastNetObject<CMapNPC>(actor);
		if (pnpc)
			pnpc->RemoveAllLink();
	}

	actor->m_curRegion = REGION_INVALID;
	actor->m_newRegion = REGION_INVALID;
}

VOID CMap::UpdateRegions()
{
	CMapActor *actor;

	// pass 1: change actor region
	for (ActorIdMap::iterator it = m_chgActors.begin(); it != m_chgActors.end(); ++it)
	{
		actor = it->second;
		if (actor->m_curRegion != REGION_INVALID && actor->m_newRegion != REGION_INVALID)
			MoveToRegion(actor);
	}

	// pass 2: add new actor to region then, and update current region
	for (ActorIdMap::iterator it = m_chgActors.begin(); it != m_chgActors.end(); ++it)
	{
		actor = it->second;
		if (actor->m_curRegion == REGION_INVALID) {
			AddToRegion(actor);
		} else {
			actor->m_curRegion = actor->m_newRegion;
			actor->m_newRegion = REGION_INVALID;
		}
	}

	m_chgActors.clear();
}

VOID CMap::OnUpdate(CONST UInt32 dt)
{
	m_updater.UpdateProcesses(dt);
}

void CMap::AddActor(CMapActor *actor)
{
	if (actor == NULL)
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("Map AddActor Warning: invalid map actor"));
		return;
	}
	ActorIdMap::iterator it = m_actors.find(ActorIdType(actor));
	if (it != m_actors.end())
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("Map AddActor Warning: [actor already in map], %s"), 
			actor->DebugInfo().c_str());
		return;
	}
	if (actor->GetActorId() != 0)
	{
		m_actors[ActorIdType(actor)] = actor;
	}
	if (actor->GetClassID() == Character_CLASSID)
	{
		CCharacter *pchar = CastNetObject<CCharacter>(actor);
		if (pchar)
		{
			if (!m_eventBuffMap.empty())
				AddEventBuff(pchar);		// add event buff
			SetOwnObject(pchar->GetContext().sessionId, this);
			gGlobal.LinkView(pchar->m_viewManager);
			// add hint npc net ID to character quest system
			CMapNPC* pNpc;
			for (NpcActorMap::iterator it = m_npcActors.begin();
				it != m_npcActors.end(); ++it)
			{
				pNpc = it->second;
				if (pNpc && pNpc->HasQuestHint())
					pchar->GetQuests()->AddHintNpc(pNpc);
			}
			pchar->GetQuests()->SendQuestHint();

			// increase instance player count if instance map
			if (GetInst_id() != 0 && GetOwner_id() != 0)
			{
				CInstance* pInst = CInstance::GetInstance(GetInst_id(), GetOwner_id());
				if (pInst)
					pInst->ChgPlayerCount(1);
			}
		}
	}
	actor->SetMap_id(GetMap_id());
	AddNotify(actor);			// notify other actors
}

void CMap::RemoveActor(CMapActor *actor)
{
	if (actor == NULL)
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("Map RemoveActor Warning: [invalid map actor]"));
		return;
	}
	if (actor->GetActorId() != 0)
	{
		ActorIdMap::iterator it = m_actors.find(ActorIdType(actor));
		if (it == m_actors.end())
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("Map RemoveActor Warning: [actor not found] %s"), 
				actor->DebugInfo().c_str());
		}
		else
		{
			m_actors.erase(it);
		}
	}
	if (actor->GetClassID() == Character_CLASSID)
	{
		CCharacter *pchar = CastNetObject<CCharacter>(actor);
		if (pchar)
		{
			if (!m_eventBuffMap.empty())
				RemoveEventBuff(pchar);		// remove event buff if necessary
			UnsetOwnObject(pchar->GetContext().sessionId, this);
			gGlobal.UnlinkView(pchar->m_viewManager);
			pchar->GetQuests()->ClearQuestHint();

			// decrease instance player count if instance map
			if (GetInst_id() != 0 && GetOwner_id() != 0)
			{
				CInstance* pInst = CInstance::GetInstance(GetInst_id(), GetOwner_id());
				if (pInst)
					pInst->ChgPlayerCount(-1);
			}
		}
		else
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Map RemoveActor Error: [character not found]"));
		}
	}
	//else if (actor->GetClassID() == MapNPC_CLASSID)
	//{
	//	 remove npc by position
	//	CMapNPC *pNpc = CastNetObject<CMapNPC>(actor);
	//	if (pNpc && pNpc->GetLifeTime() == 0)
	//		m_npcActors.erase(POS_ID(actor->GetCurPosX(), actor->GetCurPosY()));
	//}
	
	RemoveNotify(actor);		// notify other actors
}

Region* CMap::FindRegion(UInt16 index)
{
	if (index >= m_regionTotal)
		return NULL;

	return m_regions[index];
}
Region* CMap::FindRegion(UInt16 x, UInt16 y)
{
	UInt16 gx = x / REGION_WIDTH;
	UInt16 gy = y / REGION_HEIGHT;
	if (gx >= m_regionCol || gy >= m_regionRow)
		return NULL;

	return m_regions[gy * m_regionCol + gx];
}

int CMap::SetMapDb(MapDB* mapdb)
{
	m_mapdb = mapdb;

	// make regions
	if (mapdb && mapdb->m_zonedata_header)
	{
		DeleteRegions();
		m_width = mapdb->m_zonedata_header->width;
		m_height = mapdb->m_zonedata_header->height;
		m_regionCol = (m_width + REGION_WIDTH - 1) / REGION_WIDTH;
		m_regionRow = (m_height + REGION_HEIGHT - 1) / REGION_HEIGHT;
		m_regionTotal = m_regionCol * m_regionRow;
		m_regions = SafeAllocate(Region*, m_regionTotal);
		UInt16 i = 0;
		for (UInt16 gy = 0; gy < m_height; gy += REGION_HEIGHT)
		{
			for (UInt16 gx = 0; gx < m_width; gx += REGION_WIDTH)
			{
				m_regions[i] = SafeCreateObject(Region);
				m_regions[i]->Set(i, gx, gy, MIN(gx + REGION_WIDTH - 1, m_width), 
					MIN(gy + REGION_HEIGHT - 1, m_height));
				i++;
			}
		}
	}

	// default open when not opened by event
	if (m_mapdb && m_mapdb->m_basicinfo) {
		m_bOpen = (m_mapdb->m_basicinfo->m_oneventopen == 0);
		if (!m_bOpen)
			AddClosedMap(mapdb->m_basicinfo->m_mapid);
	}

	return 1;
}

int CMap::AddJumppointInstance(BinaryServerJumppointInstance* jp)
{
	m_jpList.push_back(jp);
	return 1;
}

void CMap::AddObjInstance(BinaryServerOBJInstance* obj)
{
	if (obj == NULL)
		return;

	// check duplicate add
	if (m_objMap.find(obj->m_objid) != m_objMap.end())
		return;

	String objFile;
	ObjInst &objInst = m_objMap[obj->m_objid];
	
	objInst.m_objid = obj->m_objid;
	objInst.m_posx = obj->m_posx;
	objInst.m_posy = obj->m_posy;
	_tcscpy(objInst.m_filename, obj->m_filename);
	objInst.state = 0;
	objFile.Format(_T("%s.blk"), obj->m_filename);
	BlockPosVector* blockPosList = gGlobal.m_resource->LoadOBJBlocking(objFile.c_str(), 0);
	if (blockPosList)
	{
		for (BlockPosVector::iterator it = blockPosList->begin(); it != blockPosList->end();
			++it)
		{
			m_objBlockSet.insert(*it);
			objInst.blockList.push_back(*it);
		}
	}
}

int CMap::GetObjState(UInt32 objId)
{
	ObjInstMap::iterator itObj = m_objMap.find(objId);
	if (itObj != m_objMap.end())
		return itObj->second.state;
	else
		return -1;
}

void CMap::ChgObjInstance(UInt32 objId, UInt8 state)
{
	ObjInstMap::iterator itObj = m_objMap.find(objId);
	if (itObj == m_objMap.end())
		return;

	ObjInst &objInst = itObj->second;
	if (objInst.state == state)
		return;

	// remove old object block
	for (BlockPosVector::iterator it = objInst.blockList.begin(); 
		it != objInst.blockList.end(); ++it)
	{
		m_objBlockSet.erase(*it);
	}
	objInst.blockList.clear();

	// add new object block
	String objFile;
	objFile.Format(_T("%s.blk"), objInst.m_filename);
	BlockPosVector* blockPosList = gGlobal.m_resource->LoadOBJBlocking(objFile.c_str(), state);
	if (blockPosList)
	{
		for (BlockPosVector::iterator it = blockPosList->begin(); it != blockPosList->end();
			++it)
		{
			m_objBlockSet.insert(*it);
			objInst.blockList.push_back(*it);
		}
	}
}

void CMap::RemoveObjInstance(UInt32 objId)
{
	ObjInstMap::iterator itObj = m_objMap.find(objId);
	if (itObj == m_objMap.end())
		return;

	ObjInst &objInst = itObj->second;
	for (BlockPosVector::iterator it = objInst.blockList.begin(); 
		it != objInst.blockList.end(); ++it)
	{
		m_objBlockSet.erase(*it);
	}
	m_objMap.erase(itObj);
}

CMapActor* CMap::FindMapActor(UInt16 classId, UInt32 actorId)
{
	ActorIdMap::iterator itfind = m_actors.find(ActorIdType(classId, actorId));
	if (itfind != m_actors.end())
		return itfind->second;
	else
		return NULL;
}

Boolean CMap::IsBlocked(UInt16 x, UInt16 y)
{
	return (m_pathfinder && m_pathfinder->IsBlocked(x, y));
}

Boolean CMap::IsOccupied(UInt16 x, UInt16 y)
{
	if (IsBlocked(x, y))
		return BOOLEAN_TRUE;

	NpcActorMap::iterator it = m_npcActors.find(POS_ID(x, y));
	if (it != m_npcActors.end())
		return BOOLEAN_TRUE;

	return BOOLEAN_FALSE;
}

Boolean CMap::IsHawkOccupied(UInt16 x, UInt16 y)
{
	if (IsBlocked(x, y)) return BOOLEAN_TRUE;

	UInt16 ridx = m_regionCol * (y / REGION_HEIGHT) + (x / REGION_WIDTH);
	if (ridx >= m_regionTotal) return BOOLEAN_TRUE;
	Region* pRegion = m_regions[ridx];
	
	if (pRegion->m_actors.empty()) return BOOLEAN_FALSE;

	CMapActor* actor;
	for (Region::MapActorMap::iterator it = pRegion->m_actors.begin();
		it != pRegion->m_actors.end(); ++it)
	{
		actor = it->second;
		if (actor->IsSubClass(Character_CLASSID) && actor->GetCurPosX() == x &&
			actor->GetCurPosY() == y)
		{
			CCharacter* pchar = CastNetObject<CCharacter>(actor);
			if (pchar && pchar->IsHawking())
			{
				return BOOLEAN_TRUE;
			}
		}
	}
	return BOOLEAN_FALSE;
}

int CMap::IsJumpPoint(UInt16 x, UInt16 y)
{
	BlockPos jpPos;
	PixelDirection dummy;
	Int16 cx1, cy1, cx2, cy2;
	int index = 0;
	BinaryServerJumppointInstance* jp = NULL;

	if (m_pathfinder == NULL)
		return -1;

	m_pathfinder->ConvToSBlock(x, y, cx1, cy1);
	for (JpInstList::iterator it = m_jpList.begin(); it != m_jpList.end(); ++it)
	{
		jp = *it;
		jpPos.PosX = jp->m_posx;
		jpPos.PosY = jp->m_posy;
		m_pathfinder->ConvToSBlock(jpPos.PosX, jpPos.PosY, cx2, cy2);
		if (((cx1 - cx2) * (cx1 - cx2) + (cy1 - cy2) * (cy1 - cy2)) <= 16 &&
			IsOpenMap(jp->DesMapIdNum))
			return index;
		++index;
	}

	return -1;
}

bool CMap::IsHawkZone(UInt16 x, UInt16 y)
{
	UInt32 index = y * m_width + x;
	if (index >= m_mapdb->m_zonedata.size())
		return false;

	for (UInt8 i = 0; i < m_mapdb->m_zonedata[index]->ZoneIdCount; ++i)
	{
		UInt32 currZone = m_mapdb->m_zonedata[index]->ZoneIdLst[i];
		
		if (currZone > m_mapdb->m_zoneinfo.size())
			return false;

		if( !m_mapdb->m_zoneinfo[currZone] )
		{
			TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("zone null currZone=%d x=%d y=%d"), currZone, x, y);
		}

		if (m_mapdb->m_zoneinfo[currZone] && m_mapdb->m_zoneinfo[currZone]->zone_Hawker)
			return true;
		
	}
	return false;
}

BinaryServerJumppointInstance* CMap::GetJumpPoint(int index)
{
	if (index < 0 || index >= (int) m_jpList.size())
		return NULL;

	return m_jpList[index];
}

void CMap::SafeRemove(CMapActor *actor)
{
	for (UInt16 i = 0; i < m_regionTotal; i++)
	{
		for (Region::MapActorMap::iterator it = m_regions[i]->m_actors.begin();
			it != m_regions[i]->m_actors.end();)
		{
			if (it->second == actor) {
				TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("SafeRemove Warning: actor %d:%d still in region %d"),
					actor->GetClassID(), actor->GetActorId(), i);
				it = m_regions[i]->m_actors.erase(it);
			} else {
				it ++;
			}
		}
	}
}

void CMap::GetViewNetIds(UInt16 regionIdx, RPCNetIDVector& netIds)
{
	UInt16 gx1 = regionIdx % m_regionCol;
	UInt16 gy1 = regionIdx / m_regionCol;
	UInt16 min_gx1 = minGridX(gx1);
	UInt16 max_gx1 = maxGridX(gx1);
	UInt16 min_gy1 = minGridY(gy1);
	UInt16 max_gy1 = maxGridY(gy1);
	Region* pRegion;

	netIds.clear();
	for (UInt16 gy = min_gy1; gy <= max_gy1; gy++)
	{
		for (UInt16 gx = min_gx1; gx <= max_gx1; gx++)
		{
			pRegion = m_regions[m_regionCol * gy + gx];
			netIds.insert(netIds.end(), pRegion->m_netIds.begin(), pRegion->m_netIds.end());
		}
	}
}

VOID Region::AddActor(CMapActor* actor)
{
	if (actor == NULL) return;

	if (m_actors.find(actor->GetNetId()) != m_actors.end())		// already inside
		return;
	m_actors[actor->GetNetId()] = actor;
	m_netIds.push_back(actor->GetNetId());
	//TRACE_INFODTL_5(GLOBAL_LOGGER, _F("Region::AddActor actor %d, map_id %d, map_x %d, map_y %d, region %d"),
	//	actor->GetID(), actor->GetMap_id(), actor->GetMap_x(), actor->GetMap_y(), m_index);
}

VOID Region::RemoveActor(CMapActor* actor)
{
	if (actor == NULL || m_actors.empty()) return;
	MapActorMap::iterator itfind = m_actors.find(actor->GetNetId());
	if (itfind == m_actors.end())
	{
		// not inside
		TRACE_WARNDTL_7(GLOBAL_LOGGER, _F("RemoveActor Error: [actor not inside region], this region %d, actorId=%d:%d, curRegion %d, newRegion %d, curPos=%d:%d"),
			m_index, actor->GetClassID(), actor->GetActorId(), actor->m_curRegion, actor->m_newRegion, actor->GetCurPosX(), actor->GetCurPosY());
		return;
	}
	m_actors.erase(itfind);
	RPCNetIDVector::iterator it = std::find(m_netIds.begin(), m_netIds.end(), actor->GetNetId());
	m_netIds.erase(it);
	//TRACE_INFODTL_5(GLOBAL_LOGGER, _F("Region::RemoveActor actor %d, map_id %d, map_x %d, map_y %d, region %d"),
	//	actor->GetID(), actor->GetMap_id(), actor->GetMap_x(), actor->GetMap_y(), m_index);
}

CMapNPC* CMap::CreateNpc(UInt32 npcId, UInt16 x, UInt16 y, UInt8 dir, UInt32 lifeTime,
						 UInt16 eventId, UInt32 char_id,bool visble)
{
	const NpcData* pNpcData = gGlobal.m_resource->GetNpcData(npcId);
	if (pNpcData == NULL) {
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CMap CreateNpc Warning(1): npcId %d not found"),
			npcId);
		return NULL;
	}

	UInt32 posId = 0;
	if (x == 0 || y == 0) {
		// random a non-occupied position when x or y = 0
		do {
			x = GetRand(5, m_width - 5);
			y = GetRand(5, m_height - 5);
		} while (IsOccupied(x, y));	
		posId = POS_ID(x, y);
	} else {
		// check x,y position if not occupied by any NPC
		posId = POS_ID(x, y);
		NpcActorMap::iterator it = m_npcActors.find(posId);
		if (it != m_npcActors.end())
		{
			TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("CMap CreateNpc Warning(2): new npcId %d, npcId %d already occupied at %d:%d"), npcId, it->second->GetNpc_id(), x, y);
			return NULL;
		}
	}

	CMapNPC * pNpc = CMapNPC::NewInstance();
	TRACE_ENSURE(pNpc);

	pNpc->SetData(pNpcData);
	pNpc->SetId(gNpcUid, npcId);
	pNpc->SetLifeTime(lifeTime);
	pNpc->SetMap(GetMap_id(), x, y, dir, GetOwner_id());
	pNpc->SetCurPos(x, y);
	pNpc->SetDstPos(x, y);
	pNpc->SetVisible(visble);
	// if only talk to specifiy character, set accept char_id
	if (char_id != 0)
		pNpc->AddAcceptId(char_id);
	pNpc->CheckAppear(this, gGlobal.GetGameTime(1));	// check appear and enter
	pNpc->LoadQuestHint();				// load npc quest hints
	m_npcActors[posId] = pNpc;			// add to npc actor map
	gNpcUid ++;		// increase npc uid counter

	// notify npc create to world service
	if (gGlobal.pWorldService && gGlobal.pWorldService->pPeer) {
		NpcLiveInfo npcInfo;

		npcInfo.npcId = npcId;
		npcInfo.mapId = GetMap_id();
		npcInfo.lineId = GetLine_id();
		npcInfo.x = x;
		npcInfo.y = y;
		npcInfo.eventId = eventId;
		WorldService_Proxy::NotifyNpcCreate(gGlobal.pWorldService->pPeer, RPCContext(),
			npcInfo);
	} else {
		TRACE_WARNDTL_2(GLOBAL_LOGGER, 
			_T("CreateNpc Warning: cannot notify world service, npcId %d, mapId %d"),
			pNpc->GetNpc_id(), pNpc->GetMap_id());
	}

	return pNpc;
}

String CMap::GetNpcActorNickName(UInt32 id)
{
	for (NpcActorMap::iterator it = m_npcActors.begin(); it != m_npcActors.end(); ++it)
	{
		CMapNPC * pNpc = it->second;
		if (pNpc && (pNpc->GetNpc_id() == id))
			return pNpc->GetNickName();
    }
	String empty = _T("");
    return empty;

}

void CMap::RemoveNpc(CMapNPC* pNpc)
{
	m_npcActors.erase(POS_ID(pNpc->GetCurPosX(), pNpc->GetCurPosY()));
	if (pNpc->GetAppearWorld()) {
		
		TRACE_INFODTL_2(GLOBAL_LOGGER, 
			_T("CMap::RemoveNpc : [NPC Removal OnLeaveMap] netid=(%d:%d)"), 
			pNpc->GetNetId().srvId, pNpc->GetNetId().objId);

		pNpc->OnLeaveMap();
	} else {
		DeleteNetObject(pNpc);
	}
}

void CMap::RemoveNpcEvent(UInt16 eventId)
{
	CMapNPC* pNpc;
	NpcActorMap delNpcList;

	for (NpcActorMap::iterator it = m_npcActors.begin();
		it != m_npcActors.end(); ++it)
	{
		pNpc = it->second;
		if (pNpc && pNpc->GetEventId() == eventId)
			pNpc->SetLifeTime(0);
	}
}

void CMap::AddBuffInfo(UInt16 eventId, BuffInfo &buffInfo)
{
	CMapActor* pActor;
	CCharacter* pChar;

	// add buff to all character in map
	for (ActorIdMap::iterator it2 = m_actors.begin(); 
		it2 != m_actors.end(); ++it2)
	{
		pActor = it2->second;
		if (pActor == NULL || !pActor->IsSubClass(Character_CLASSID))
			continue;
		pChar = CastNetObject<CCharacter>(pActor);
		if( pChar->IsState(CCharacter::ST_BATTLE) )		// if in monitor state, map-buff is used.
		{
			if( pChar->m_pBatChar )
			{
				for (UInt16Vector::iterator it3 = buffInfo.buffIds.begin();
					it3 != buffInfo.buffIds.end(); ++it3)
				{
					if (buffInfo.period / 60 > 0)
						pChar->m_pBatChar->AddBuff(*it3, 0, 0, 0, buffInfo.period / 60, 100);
				}
			}
			else
			{
				TRACE_INFODTL_2(GLOBAL_LOGGER, 
					_T("CMap::AddBuffInfo Info [Char's Battle obj not found in Battle state] char_id=%d, event_id=%d"),
					pChar->GetChar_id(), eventId);
			}
		}
		else
		{
			for (UInt16Vector::iterator it3 = buffInfo.buffIds.begin();
				it3 != buffInfo.buffIds.end(); ++it3)
			{
				if (buffInfo.period / 60 > 0)
					pChar->AddBuff(*it3, 0, 0, 0, buffInfo.period / 60, 100);
			}
		}
	}

	m_eventBuffMap[eventId] = buffInfo;
}

void CMap::RemoveBuffInfo(UInt16 eventId)
{
	CMapActor* pActor;
	CCharacter* pChar;
	BuffInfoMap::iterator it = m_eventBuffMap.find(eventId);

	// remove buff from all character in map
	if (it != m_eventBuffMap.end()) {
		BuffInfo &buffInfo = it->second;
		for (ActorIdMap::iterator it2 = m_actors.begin(); 
			it2 != m_actors.end(); ++it2)
		{
			pActor = it2->second;
			if (pActor == NULL || !pActor->IsSubClass(Character_CLASSID))
				continue;
			pChar = CastNetObject<CCharacter>(pActor);
			if (pChar->m_pBatChar)
			{
				for (UInt16Vector::iterator it3 = buffInfo.buffIds.begin();
					it3 != buffInfo.buffIds.end(); ++it3)
				{
					pChar->m_pBatChar->RemoveBuff(*it3);
				}
			}
            else
			{
				for (UInt16Vector::iterator it3 = buffInfo.buffIds.begin();
					it3 != buffInfo.buffIds.end(); ++it3)
				{
					pChar->RemoveBuff(*it3);
				}
			}
		}
		m_eventBuffMap.erase(it);
	}
}

void CMap::AddEventBuff(CCharacter *pChar)
{
	if (pChar == NULL)
		return;

	for (BuffInfoMap::iterator it = m_eventBuffMap.begin();
		it != m_eventBuffMap.end(); ++it)
	{
		BuffInfo &buffInfo = it->second;
		if (buffInfo.period > 0)
		{
			for (UInt16Vector::iterator it2 = buffInfo.buffIds.begin();
				it2 != buffInfo.buffIds.end(); ++it2)
			{
				if (buffInfo.period / 60 > 0)
					pChar->AddBuff(*it2, 0, 0, 0, buffInfo.period / 60, 100);
			}
			RPCMask mask;
			mask.MaskBits(CCharacter::buffMask, CCharacter::buffCacheMask, -1);
			mask &= pChar->GetUpdateMask();
			if (pChar->GetPeer())
				pChar->SendAttr(pChar->GetPeer(), pChar->GetContext(), mask);
		}
	}
}

void CMap::RemoveEventBuff(CCharacter *pChar)
{
	if (pChar == NULL)
		return;

	for (BuffInfoMap::iterator it = m_eventBuffMap.begin();
		it != m_eventBuffMap.end(); ++it)
	{
		BuffInfo &buffInfo = it->second;
		if (buffInfo.bLeave) {
			for (UInt16Vector::iterator it2 = buffInfo.buffIds.begin();
				it2 != buffInfo.buffIds.end(); ++it2)
			{
				pChar->RemoveBuff(*it2);
			}
		}
	}
}

void CMap::TimeZoneChanged(UInt8 gameTime)
{
	CMapNPC *pNpc;

	for (NpcActorMap::iterator it = m_npcActors.begin();
		it != m_npcActors.end(); ++it)
	{
		pNpc = it->second;
		if (pNpc)
			pNpc->CheckAppear(this, gameTime);
	}
}

void CMap::SetOpen(Boolean bOpen)
{
	// map is closing, kick out all characters
	CMapActor* pActor;
	CCharacter* pChar;
	if (m_bOpen && !bOpen) {
		ActorIdMap cloneList = m_actors;
		for (ActorIdMap::iterator it = cloneList.begin(); 
			it != cloneList.end(); ++it)
		{
			pActor = it->second;
			if (pActor == NULL || !pActor->IsSubClass(Character_CLASSID))
				continue;
			pChar = CastNetObject<CCharacter>(pActor);
			if (pChar && pChar->IsInMap())
				pChar->JumpReborn();
		}
	}
	m_bOpen = bOpen;
}

void CMap::LoadAllHints()
{
	CMapNPC* pNpc;
	for (NpcActorMap::iterator it = m_npcActors.begin();
		it != m_npcActors.end(); ++it)
	{
		pNpc = it->second;
		if (pNpc)
			pNpc->LoadQuestHint();
	}

	CMapActor* actor;
	CCharacter* pchar;
	for (ActorIdMap::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		actor = it->second;
		if (actor && actor->GetClassID() == Character_CLASSID) {
			pchar = CastNetObject<CCharacter>(actor);
			if (pchar)
				pchar->GetQuests()->CheckAllHints();
		}
	}
}

void CMap::CheckAllHints(CCharacter* pChar, RPCNetIDVector &addNpcIds, UInt16Vector &addQuestIds, 
						 RPCNetIDVector &delNpcIds, UInt16Vector &delQuestIds)
{
	if (pChar == NULL)
		return;

	CMapNPC *pNpc;
	for (NpcActorMap::iterator it = m_npcActors.begin();
		it != m_npcActors.end(); ++it)
	{
		pNpc = it->second;
		if (pNpc && pNpc->HasQuestHint())
			pChar->GetQuests()->CheckQuestHint(pNpc, addNpcIds, addQuestIds, delNpcIds, delQuestIds);
	}
}

Boolean CMap::IsOpenMap(UInt16 map_id)
{
	return (gGlobal.m_closeMapSet.find(map_id) == gGlobal.m_closeMapSet.end());
}

Boolean CMap::IsClosedMap(UInt16 map_id)
{
	return (gGlobal.m_closeMapSet.find(map_id) != gGlobal.m_closeMapSet.end());
}

void CMap::AddClosedMap(UInt16 map_id)
{
	gGlobal.m_closeMapSet.insert(map_id);
}

void CMap::RemoveClosedMap(UInt16 map_id)
{
	gGlobal.m_closeMapSet.erase(map_id);
}

void CMap::CalcRewards()
{
	CMapActor* actor = NULL;
	CCharacter* pchar = NULL;
	ActorIdMap actors = m_actors;
	for (ActorIdMap::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		actor = it->second;
		if (actor && actor->GetClassID() == Character_CLASSID)
		{
			pchar = CastNetObject<CCharacter>(actor);
			if (pchar)
			{
				UInt8 result = 0;
				if (m_guildWarWinID == 0)				
					result = 2;				
				else if (pchar->GetGuildUid()  != m_guildWarWinID)
					result = 1;
				CallLuaFunc(gGlobal.g_pLuaState, "CalGuildWarReward", String(), pchar, result);
			}
		}
	}
}

void CMap::ForceLeaveGuildWar(UInt16 map_id, UInt16 x, UInt16 y)
{
	CMapActor* actor;
	CCharacter* pchar;
	ActorIdMap actors = m_actors;

	UpdateRegions();
	for (ActorIdMap::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		actor = it->second;
		if (actor && actor->GetClassID() == Character_CLASSID)
		{
			pchar = CastNetObject<CCharacter>(actor);
			if (pchar)
			{				
				pchar->JumpMap(map_id, x, y);				
			}
		}
	}

	m_guildWarKick = true;
}
void CMap::ForceAllLeave()
{
	CMapActor* actor;
	CCharacter* pchar;
	ActorIdMap actors = m_actors;

	UpdateRegions();
	for (ActorIdMap::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		actor = it->second;
		if (actor && actor->GetClassID() == Character_CLASSID)
		{
			pchar = CastNetObject<CCharacter>(actor);
			if (pchar)
				pchar->JumpReborn();
		}
	}
}

void CMap::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
		case EVT_UPDATE_GUILD_WAR:		
			CheckGuildWarResult(false);
			break;
		case EVT_UPDATE_GUILD_END:
			CheckGuildWarResult(true);
			break;
		case EVT_UPDATE_GUILD_KICK:
			ForceLeaveGuildWar(gConfigure.guildWarRebornMapID, gConfigure.guildWarRebornMapX, gConfigure.guildWarRebornMapY);
			break;
	}
}

void CMap::StartGuildWar()
{
	ActorIdMap actors = m_actors;
	CMapActor * actor = NULL;
	CCharacter * pchar = NULL;
	m_inGuildWar = true;
	m_guildWarID1 = 0;
	m_guildWarID2 = 0;
	m_guildWarWinID = 0;
	if (m_mapdb && m_mapdb->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR)
	{
		for (ActorIdMap::iterator it = actors.begin(); it != actors.end(); ++it)
		{
			actor = it->second;
			if (actor && actor->GetClassID() == Character_CLASSID)
			{
				pchar = CastNetObject<CCharacter>(actor);
				if (pchar)
				{
					if (m_guildWarID1 == 0)
					{
						m_guildWarID1 = pchar->GetGuildUid();
						m_guildWarID2 = pchar->GetOppGuildUid();
						break;
					}					
				}
			}
		}
	}
	m_guildWarKick = false;
}

void CMap::RandomPos(UInt16 & x, UInt16 & y)
{
	if (GetMapInfo())
	{
		UInt16 startX = GetRand(GetMapInfo()->m_zonedata_header->width);
		UInt16 startY = GetRand(GetMapInfo()->m_zonedata_header->height);
		x = startX;
		y = startY;
		bool first = true;
		while (m_pathfinder->IsBlocked(x, y))
		{
			if (!first && x == startX && y == startY)
				break;
			first = false;
			x ++;
			if (x >= GetMapInfo()->m_zonedata_header->width)
			{
				y ++;
				x = 0;
				if (y >= GetMapInfo()->m_zonedata_header->height)
				{
					y = 0;
				}
			}
		}
	}
}
void CMap::GenGuildWarBox()
{
	for (UInt i = 0; i < gConfigure.guildWarBoxNum; i++)
	{
		UInt16 x = 0;
		UInt16 y = 0;
		RandomPos(x, y);
		CreateNpc(gConfigure.guildWarBoxID, x, y, 0, gConfigure.guildWarCheckTime * 1000 * 60);
	}
}

void CMap::CheckGuildWarResult(bool end)
{
	if (!m_inGuildWar)
		return;

	UInt32 updateTime = gConfigure.guildWarCheckTime * 1000 * 60;
	if (m_mapdb && m_mapdb->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR)
	{
		ActorIdMap actors = m_actors;
		CMapActor * actor = NULL;
		CCharacter * pchar = NULL;
		UInt32 guild1Count = 0;
		UInt32 guild2Count = 0;
		for (ActorIdMap::iterator it = actors.begin(); it != actors.end(); ++it)
		{
			actor = it->second;
			if (actor && actor->GetClassID() == Character_CLASSID)
			{
				pchar = CastNetObject<CCharacter>(actor);
				if (pchar)
				{
					if (pchar->GetGuildUid() == m_guildWarID1)
						guild1Count ++;
					else if (pchar->GetGuildUid() == m_guildWarID2)
						guild2Count ++;
				}
			}
		}

		
		if (guild1Count == 0 && guild2Count == 0)
		{
			m_guildWarWinID = 0;
			MessageService_Proxy::GuildWarResult(gGlobal.pMessageService->pPeer, RPCContext(), m_guildWarID1, m_guildWarID2, 0);			
			m_inGuildWar = false;
		}
		else if (end)
		{
			if (guild1Count  < guild2Count)
			{
				m_guildWarWinID = m_guildWarID2;
				MessageService_Proxy::GuildWarResult(gGlobal.pMessageService->pPeer, RPCContext(), m_guildWarID1, m_guildWarID2, m_guildWarWinID);
				GenGuildWarBox();				
				

			}
			else if (guild1Count > guild2Count )
			{
				m_guildWarWinID = m_guildWarID1;
				MessageService_Proxy::GuildWarResult(gGlobal.pMessageService->pPeer, RPCContext(), m_guildWarID1, m_guildWarID2, m_guildWarWinID);
				GenGuildWarBox();
				
			}
			else
			{
				m_guildWarWinID = 0;
				MessageService_Proxy::GuildWarResult(gGlobal.pMessageService->pPeer, RPCContext(), m_guildWarID1, m_guildWarID2, 0);			
			}
			m_inGuildWar = false;

			RaiseUniqEventID(EVT_UPDATE_GUILD_KICK, updateTime);
		}
		else
		{
			if (guild1Count == 0)
			{
				m_guildWarWinID = m_guildWarID2;
				MessageService_Proxy::GuildWarResult(gGlobal.pMessageService->pPeer, RPCContext(), m_guildWarID1, m_guildWarID2, m_guildWarWinID);
				GenGuildWarBox();				
				m_inGuildWar = false;
			}
			else if (guild2Count == 0)
			{
				m_guildWarWinID = m_guildWarID1;
				MessageService_Proxy::GuildWarResult(gGlobal.pMessageService->pPeer, RPCContext(), m_guildWarID1, m_guildWarID2, m_guildWarWinID);
				GenGuildWarBox();				
				m_inGuildWar = false;
			}
			else
				RaiseUniqEventID(EVT_UPDATE_GUILD_WAR, updateTime);
		}
	}

	if (!m_inGuildWar)
	{
		
		RaiseUniqEventID(EVT_UPDATE_GUILD_KICK, updateTime);
		CalcRewards();
	}
}

void CMap::GetAllPartyInfo(PartyInfoVector &PartyInfoList)
{
	PartyInfoList.clear();
	for (ActorIdMap::iterator iter = m_actors.begin(); iter != m_actors.end(); ++iter)
	{
		CMapActor* pActor = iter->second;
		if (pActor == NULL || !pActor->IsSubClass(Character_CLASSID))
			continue;
		CCharacter* pChar = CastNetObject<CCharacter>(pActor);
		if (pChar &&
			pChar->IsLeader() &&
			pChar->GetParty() &&
			pChar->GetParty()->GetCount() < PARTY_MAX_MEMBER)
		{
			UInt8 SexClass = (pChar->GetCclass() > 0 ? 
				((pChar->GetCclass() - 1) * 2 + pChar->GetSex() + 1) : 0);

			if(pChar->GetParty()->GetDesc().size()>20)
				return;

			TCHAR wcDesc[20];
			wmemcpy(wcDesc,pChar->GetParty()->GetDesc().c_str(),sizeof(wcDesc));

			PartyInfo info(pChar->GetNickName(), pChar->GetFaction(), SexClass, pChar->GetLevel(),
				pChar->GetParty()->GetCount(), wcDesc, pChar->GetNetId(), pChar->GetChar_id());

			OutputDebugString(pChar->GetParty()->GetDesc().c_str());
			OutputDebugString(L"GetAllPartyInfo\n");
			OutputDebugString(info.desc);
			OutputDebugString(L"GetAllPartyInfo\n");

			PartyInfoList.push_back(info);
		}
	}
}

void CMap::GetAllCharInfo(CharInfoVector &CharInfoList)
{
	CharInfoList.clear();
	for (ActorIdMap::iterator iter = m_actors.begin(); iter != m_actors.end(); ++iter)
	{
		CMapActor* pActor = iter->second;
		if (pActor == NULL || !pActor->IsSubClass(Character_CLASSID))
			continue;
		CCharacter* pChar = CastNetObject<CCharacter>(pActor);
		if (pChar &&
			(pChar->IsLeader()) &&
			(!pChar->GetParty()) &&
			(!pChar->IsHawking()))
		{
			UInt8 SexClass = (pChar->GetCclass() > 0 ? 
				((pChar->GetCclass() - 1) * 2 + pChar->GetSex() + 1) : 0);
			CharInfo_new info(pChar->GetNickName(), pChar->GetFaction(), SexClass, pChar->GetLevel(),
				pChar->GetNetId(), pChar->GetChar_id());
			CharInfoList.push_back(info);
		}
	}
}


void CMap::RandomIncognitoBox(UInt8 boxType)
{
	UInt16 x = 0;
	UInt16 y = 0;
	while((x < 6 || y <60 ) || (x > 65 || y > 132))
	{
		RandomPos(x, y);
	}
	CMapActor* actor;
	CCharacter* pchar;
	ActorIdMap actors = m_actors;
	for (ActorIdMap::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		actor = it->second;
		if (actor && actor->GetClassID() == Character_CLASSID)
		{
			pchar = CastNetObject<CCharacter>(actor);
			if (pchar && !pchar->IsInBattle())
			{				
				pchar->CB_ShowIncognitoBoxEff(pchar->GetPeer(),pchar->GetContext(),x,y-INCOGNITBOX_MODIFIED_POSY,boxType);
			}
		}
	}
	CMapNPC* pMapNpc = CreateNpc(gConfigure.IncognitoBoxID, x, y, 0, gConfigure.IncognitoCheckTime * 1000 * 60,0,0,false);
	pMapNpc->RaiseUniqEventID(CMapNPC::EVT_DISPLAY, INCOGNITOBOX_EFF_TIME);
}

void CMap::RandomBox(UInt16 x,UInt16 y,UInt8 boxType)
{
	CMapActor* actor;
	CCharacter* pchar;
	ActorIdMap actors = m_actors;
	for (ActorIdMap::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		actor = it->second;
		if (actor && actor->GetClassID() == Character_CLASSID)
		{
			pchar = CastNetObject<CCharacter>(actor);
			if (pchar && !pchar->IsInBattle())
			{				
				pchar->CB_ShowIncognitoBoxEff(pchar->GetPeer(),pchar->GetContext(),x,y-INCOGNITBOX_MODIFIED_POSY,boxType);
			}
		}
	}
	CMapNPC* pMapNpc = CreateNpc(gConfigure.IncognitoBoxID, x, y, 0, gConfigure.IncognitoCheckTime * 1000 * 60,0,0,false);
	pMapNpc->RaiseUniqEventID(CMapNPC::EVT_DISPLAY, INCOGNITOBOX_EFF_TIME);
}

void CMap::UpdateMapNpcVisbleMask(CMapNPC* pMapNpc,bool visble)
{
	CMapActor* actor;
	CCharacter* pchar;
	ActorIdMap actors = m_actors;
	for (ActorIdMap::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		actor = it->second;
		if (actor && actor->GetClassID() == Character_CLASSID)
		{
			pchar = CastNetObject<CCharacter>(actor);
			if (pchar && !pchar->IsInBattle())
			{				
				pMapNpc->SetVisible(visble);
				pMapNpc->SendAttr(pchar->GetPeer(),pchar->GetContext(),CMapNPC::visibleMask);
				TRACE_WARNDTL_3(GLOBAL_LOGGER, _T("Map AddActor UpdateMapNpcVisbleMask: charId = %d,npcID = %d,visble = %d"), 
					pchar->GetChar_id(),pMapNpc->GetNpc_id(),pMapNpc->GetVisible());
			}
		}
	}
}