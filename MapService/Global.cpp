//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "MapService/Global.h"
//-- Library
#include "Proxy/Data/MsgCharacterData.h"
#include "Proxy/Data/QuestNpcFlagData.h"
#include "Proxy/Logic/WorldEvent.h"
#include "Proxy/Service/MessageService_proxy.h"
#include "Proxy/Service/WorldService_proxy.h"
#include "Stub/Logic/Map.h"
#include "Stub/Logic/Party.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/ChargeShop.h"
#include "Stub/Logic/Messenger.h"
#include "Stub/Logic/Guild.h"
#include "Stub/Logic/GuildMember.h"
#include "Stub/Logic/Battle.h"
#include "Stub/Logic/QuestJournal.h"
#include "Stub/Logic/BatActor.h"
#include "Stub/Logic/Party.h"

#include "MapService/Configure.h"
#include "MapService/MapApplication.h"
#include "MapService/ViewManager.h"
#include "MapService/AnimTimer.h"
#include "MapService/Instance.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResNPC.h"
#include "Resource/ResMob.h"
#include "Resource/ResItem.h"
#include "Resource/ResSkill.h"
#include "Resource/ResBuff.h"
#include "Resource/ResTitle.h"
#include "Resource/ResItem.h"
#include "Resource/GameStringTable.h"

#include <atltime.h>

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"

	int luaopen_LuaI(lua_State* L);		// declare the wrapped module
}

#define TILE_IN_NORMAL_VALUE		0x0000
#define TILE_IN_BLOCK_VALUE			0x0001
#define TILE_IN_ALPHA_VALUE			0x0002
#define TILE_IN_SHADOW_VALUE		0x0004
#define TILE_IN_SEADEPTH_VALUE		0xFF00
#define TILE_IN_FOOTPRINT_VALUE		0x0010
#define TILE_IN_NAVIGATION_VALUE	0x0020

#define FILENAME_LENGTH				1024
#define SKILL_LUA_INIT				".\\Data\\Skill\\"
#define BUFF_LUA_INIT				".\\Data\\Buff\\"
#define ITEM_BATTLE_LUA_INIT		".\\Data\\ItemBattle\\"
#define GLOBAL_SKILL_FUNC			"GlobalSkillFuncS.lua"
#define GLOBAL_BUFF_FUNC			"GlobalBuffFuncS.lua"
#define BATTLE_LUA_INIT_FILE		".\\Data\\BattleLua\\battleLua.lua"
#define LUA_AI_PATH					".\\Data\\AIScript"
#define GDS_LUA_FILE				".\\Data\\Script\\GDS.lua"
#define BDS_LUA_FILE				".\\Data\\Script\\BatScript\\BDS.lua"
#define LUA_GC_STEP					1000
#define GAMESTRINGTABLE_LOCALIZATION_FILENAME _T("Data\\CSV\\localization.csv")

void Session::Release(CONST SessionID &sid)
{
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("deleted, session %s"), 
		sid.GetString().c_str());

	ProcedureManager::GetInstance().RemoveBySession(sid);

	if (pchar)
	{
		gGlobal.m_charToSession.RemoveCharToSession(pchar->GetChar_id());
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("session %s, char_id %d, pchar %d"),
			sid.GetString().c_str(), pchar->GetChar_id(), pchar->GetID());
		DeleteNetObject(pchar);
	}
}

Boolean Global::Initialize(void)
{
	m_rand.Randomize();
	LocalRPCManager::GetInstance().SetServiceId(gConfigure.serviceId);

	m_resource = SafeCreateObject(CServerResManager);
	Int bReadNpcDat  = 0;
	Int bReadMobDat  = 0;
	Int bReadItemDat = 0;
	Int bReadSkillDat = 0;
	Int bReadBuffDat	= 0;

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Npc..."));
	bReadNpcDat = GetPrivateProfileInt(_T("ReadDataOption"), _T("ReadNpcDat"), 0, CONFIGURATION_INI_FILE);
	if( bReadNpcDat )
		m_resource->LoadNpcDAT();
	else
		m_resource->LoadNpcCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Mob..."));
	bReadMobDat = GetPrivateProfileInt(_T("ReadDataOption"), _T("ReadMobDat"), 0, CONFIGURATION_INI_FILE);
	if( bReadMobDat )
		m_resource->LoadMobDAT();
	else
		m_resource->LoadMobCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Item..."));
	bReadItemDat = GetPrivateProfileInt(_T("ReadDataOption"), _T("ReadItemDat"), 0, CONFIGURATION_INI_FILE);
	if( bReadItemDat )
		m_resource->LoadItemDAT();
	else
		m_resource->LoadItemCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Skill..."));
	bReadSkillDat = GetPrivateProfileInt(_T("ReadDataOption"), _T("ReadSkillDat"), 0, CONFIGURATION_INI_FILE);
	if( bReadSkillDat )
		m_resource->LoadSkillDAT();
	else
		m_resource->LoadSkillCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Buff..."));
	bReadBuffDat = GetPrivateProfileInt(_T("ReadDataOption"), _T("ReadBuffDat"), 0, CONFIGURATION_INI_FILE);
	if( bReadBuffDat )
		m_resource->LoadBuffDAT();
	else
		m_resource->LoadBuffCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Map..."));
	m_resource->LoadMapDAT(); // LoadNPC() must be executed before.

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Shop List..."));
	m_resource->LoadShopCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading All Random Table ..."));
	m_resource->LoadAllRandTable();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Skill Shop List..."));
	m_resource->LoadSkillShopCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading MobSkill List..."));
	m_resource->LoadSkillCombinationCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Start Character List..."));
	m_resource->LoadStartCharCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading World Event Schedule List..."));
	m_resource->LoadEventScheduleCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading BlockWord List..."));
	m_resource->LoadBlockWordCSV();

	//TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading ChargeShop List..."));
	//m_resource->LoadChargeShopCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Title List..."));
	m_resource->LoadTitleCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Pet List..."));
	m_resource->LoadPetCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Mix Shop..."));
	m_resource->LoadMixShopCSV();

//	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Anti-Hack Image..."));
//	m_resource->LoadAntihackDAT();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading NPC Name..."));
	m_resource->LoadNpcNameCSV();	

	LoadQuestJournal();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Question..."));
	m_resource->LoadQuestionCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading PartnerCollection..."));
	m_resource->LoadPartnerCollectionCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading ItemSet..."));
	m_resource->LoadItemSetCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Lineup Data..."));
	m_resource->LoadLineupCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Manor Data..."));
	m_resource->LoadManorCSV();
	
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading AccShop Data..."));
	m_resource->LoadAccShopItemCSV();

	TRACE_VERBOSE(GLOBAL_LOGGER, _F("Initialization: Reading PointsShopItem..."));
	m_resource->LoadPointsShopItemCSV();


	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading localization..."));
	m_pStringTable = SafeCreateObject(CGameStringTable);
	m_pStringTable->Load(GAMESTRINGTABLE_LOCALIZATION_FILENAME);

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Init all Lua State..."));

	// GM loaded printout
	String outStr;
	for (Index i = C_INDEX(0); i < gConfigure.gmIdList.size(); ++i)
		outStr.AppendFormat(_T("%i,"), gConfigure.gmIdList[i]);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("%s"), outStr.c_str());

	g_ComboRate = GetPrivateProfileInt(_T("BattleSetting"), _T("comboRate"), 10, CONFIGURATION_INI_FILE);
	g_MaxBattleTurn = GetPrivateProfileInt(_T("BattleSetting"), _T("maxBattleTurn"), 150, CONFIGURATION_INI_FILE);

	g_bShowItemPartnerPetOfOtherLine = GetPrivateProfileInt(_T("ShowDetail"), _T("showItemPartnerPetOfOtherLine"), 1, CONFIGURATION_INI_FILE);

	// load all lua scripts
	LoadAllScript();

	// init battle animation timer
	m_animTimer = SafeCreateObject(CAnimTimer);
	m_animTimer->Initialize();

	// create character rings
	m_viewRing = SafeCreateObject(ViewRing);
	ViewRing *prevRing = m_viewRing;
	ViewRing *pRing = m_viewRing;
	for (int i = 1; i < gConfigure.viewRing; i++)
	{
		pRing = SafeCreateObject(ViewRing);
		prevRing->next = pRing;
		prevRing = pRing;
	}
	pRing->next = m_viewRing;					// close ring
	// set view manager setting
	ViewManager::m_syncLimit = gConfigure.viewSyncLimit;
	ViewManager::m_agingTime = gConfigure.viewAgeTime;

	m_itemLimitMap.clear();
	m_pChargeShop = CChargeShop::NewInstance();
	
	g_IncognitoPKTimeOver = true;
	g_charParty1.clear();
	g_charParty2.clear();
	return (BOOLEAN_TRUE);
}

void Global::Uninitialize(void)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, 
		_T("Uninitialize: m_mapIndices %d, m_pathFinderMap %d"),
		m_mapIndices.size(), m_pathFinderMap.size());

	// delete all instance
	CInstance::DeleteAll();

	for (MapIndexList::iterator it = m_mapIndices.begin();
		it!=m_mapIndices.end(); ++it)
	{
		if (it->second)
		{
			MapApplication::GetInstance().Detach(it->second);
			DeleteNetObject(it->second);
		}
	}

	for (PathFinderMap::iterator it = m_pathFinderMap.begin(); it != m_pathFinderMap.end(); ++it)
		SafeDeleteObject(it->second);

	m_mapIndices.clear();
	m_itemLimitMap.clear();
	m_pathFinderMap.clear();

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Uninitialize: alive battles %d, dead battles %d"), 
		m_aliveBattles.size(), m_deadBattles.size());
	// clear dead battles
	for (BattleList::iterator it = m_deadBattles.begin(); it != m_deadBattles.end(); ++it)
	{
		DeleteNetObject(*it);
	}
	m_deadBattles.clear();

	// clear all alive battles
	for (BattleList::iterator it = m_aliveBattles.begin(); it != m_aliveBattles.end(); ++it)
	{
		DeleteNetObject(*it);
	}
	m_aliveBattles.clear();

	// Close Lua State'
	TRACE_INFODTL(GLOBAL_LOGGER, _F("ReleaseLua"));
	ReleaseLua(g_pLuaState);
	ReleaseLua(g_pScriptState);

	// delete all character rings
	if (m_viewRing) {
		int count = 1;
		ViewRing *firstRing = m_viewRing;
		ViewRing *tmpRing;
		do
		{
			tmpRing = m_viewRing;
			m_viewRing = m_viewRing->next;
			SafeDeleteObject(tmpRing);
			++count;
		} while (m_viewRing && m_viewRing != firstRing);
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Uninitialize: view ring %d"), count);
	}

	for (GuildMap::iterator Itr = g_AllGuilds.begin();
		Itr != g_AllGuilds.end(); Itr ++)
	{
		DeleteNetObject(Itr->second);
	}
	g_AllGuilds.clear();

	for (GuildMemberMap::iterator Itr = g_AllGuildMembers.begin();
		Itr != g_AllGuildMembers.end(); Itr ++)
	{
		DeleteNetObject(Itr->second);
	}
	g_AllGuildMembers.clear();

	// delete all world events
	for (WorldEventMap::iterator it = m_worldEventMap.begin(); 
		it != m_worldEventMap.end(); ++it)
	{
		DeleteNetObject(it->second);
	}
	m_worldEventMap.clear();

	// delete all quest journals
	DeleteNetChilds(&m_journalGroup);
	DeleteNetGroup(m_pChargeShop, BOOLEAN_TRUE);

	// delete all msg character data
	for (MsgCharDataMap::iterator it = m_msgCharMap.begin(); it != m_msgCharMap.end(); ++it)
		DeleteNetObject(it->second);
	m_msgCharMap.clear();

	m_itemLimitMap.clear();
	m_closeMapSet.clear();
	g_RankTitleLst.clear();
	m_animTimer->Uninitialize();
	g_charParty1.clear();
	g_charParty2.clear();
	g_IncognitoPKTimeOver = true;
	SafeDeleteObject(m_animTimer);
	SafeDeleteObject(m_resource);
	SafeDeleteObject(m_pStringTable);
}

// load map index file
bool Global::LoadMapData()
{
	for (MapIndexSet::iterator it = gConfigure.mapIndices.begin(); 
		it != gConfigure.mapIndices.end(); ++it)
	{
		ActivateMap(*it);
	}

	// erase redundant map
	for (MapIndexList::iterator it = m_mapIndices.begin(); it != m_mapIndices.end();)
	{
		if (it->second == NULL)
		{
			it = m_mapIndices.erase(it);
		}
		else
		{
			++it;
		}
	}

	m_resource->LoadMapNpc(); // load all npc data even not including in maps

	return true;	
}


CMap* Global::ActivateMap(MapIndex mapidx)
{
	bool active = false;
	PathFindLoadData pathFindData;
	UInt32 numGuide = 0;
	BinaryServerNPCInstance* pNpc;
	CMap* pMap = CMap::NewInstance();

	MapApplication::GetInstance().Attach(pMap);

	// load only zone/instance when needed by this map service
	if (m_resource->ActivateMap(mapidx.map_id, pathFindData, numGuide))
	{
		MapDB* pMapData = (MapDB*) m_resource->GetMapData(mapidx.map_id);
		if (pMapData)
		{
			// Basic info
			pMap->SetInfo(mapidx.map_id, mapidx.line_id);
			pMap->SetMapDb(pMapData);

			// set map path finder
			PathFinder* pf;
			PathFinderMap::iterator itPath = m_pathFinderMap.find(mapidx.map_id);
			if (itPath != m_pathFinderMap.end())
			{
				pf = itPath->second;
			}
			else
			{
				pf = SafeCreateObject(PathFinder);
				pf->SetMap(pMapData->m_blockData, pMapData->m_zonedata_header->width, 
					pMapData->m_zonedata_header->height);
				if (numGuide != 0)
					pf->PreCalculate(pathFindData.weightData, pathFindData.distData, 
						pathFindData.linkData);
				else
					pf->PreCalculate();
				m_pathFinderMap[mapidx.map_id] = pf;
			}
			pMap->SetPathFinder(pf);

			// create npc of this map
			for (NpcInstList::const_iterator itrnpc = pMapData->m_npc.begin();
				itrnpc != pMapData->m_npc.end(); ++itrnpc)
			{
				pNpc = *itrnpc;
				pMap->CreateNpc(pNpc->nNPCUserID, pNpc->npc_position_x, 
						pNpc->npc_position_y, pNpc->npc_direction);
			}

			// Assign Jumppoint
			for (JpInstList::const_iterator itrjp = pMapData->m_jp.begin(); 
				itrjp != pMapData->m_jp.end(); ++itrjp)
				pMap->AddJumppointInstance(*itrjp);

			// add map object instance
			for (ObjInstList::iterator itInst = pMapData->m_obj.begin(); 
				itInst != pMapData->m_obj.end(); ++itInst)
			{
				pMap->AddObjInstance(*itInst);
			}

			active = true;
		}
	}
	if (active)
	{
		RPCContext context;

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("map %d, line %d, owner %d is activated"), 
			mapidx.map_id, mapidx.line_id, mapidx.owner_id);
		m_mapIndices[mapidx] = pMap;
		// register activated map in World and Message service
		WorldService_Proxy::RegisterMap(pWorldService->pPeer, context, 
			gConfigure.serviceId, mapidx.map_id, mapidx.line_id, mapidx.owner_id, pMap->IsOpen());
		MessageService_Proxy::RegisterMap(pMessageService->pPeer, context, 
			gConfigure.serviceId, mapidx.map_id, mapidx.line_id, mapidx.owner_id);
	}
	else
	{
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("map %d, line %d, owner %d is not activated"), 
			mapidx.map_id, mapidx.line_id, mapidx.owner_id);
		DeleteNetObject(pMap);
	}

	return pMap;
}

CMap* Global::DeactivateMap(MapIndex mapidx)
{
	MapIndexList::iterator itfind = m_mapIndices.find(mapidx);
	if (itfind == m_mapIndices.end()) {		// return null if not found
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("map %d, line %d, owner %d is not found"), 
			mapidx.map_id, mapidx.line_id, mapidx.owner_id);
		return NULL;
	}

	RPCContext context;
	CMap* pMap = itfind->second;

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("map %d, line %d, owner %d is deactivated"), 
		mapidx.map_id, mapidx.line_id, mapidx.owner_id);
	MapApplication::GetInstance().Detach(pMap);
	m_mapIndices.erase(itfind);
	// un-register activated map in World and Message service
	WorldService_Proxy::UnregisterMap(pWorldService->pPeer, context, 
		gConfigure.serviceId, mapidx.map_id, mapidx.line_id, mapidx.owner_id);
	MessageService_Proxy::UnregisterMap(pMessageService->pPeer, context, 
		gConfigure.serviceId, mapidx.map_id, mapidx.line_id, mapidx.owner_id);

	return pMap;
}

/*
BOOL Global::SetPathFinderData(PathFinder &pf, WORD* block, Int width, Int height)
{
	// block array size check
	if( width==0 || height==0 )
		return FALSE;

	pf.SetMap((UInt16*) block, width, height);
	pf.FindGuidePoints();

	return TRUE;
}

PathFinder* Global::GetPathFinder(MapIndex mapidx)
{
	MapMapPathFinder::iterator itr = m_pathfinder.find(mapidx);
	if( itr!=m_pathfinder.end() )
		return itr->second;
	return NULL;
}

PathFinder* Global::MakeMapPathFinder(MapIndex mapidx, PathFindLoadData& pPathFindData, UInt32 pNumGuide)
{
	PathFinder* finder = NULL;
	MapDB* tempmap = const_cast<MapDB*>(m_resource->GetMapData(mapidx.map_id));
	if( tempmap )
	{
		MapCombinedBlock::iterator itr = m_cBlock.find(mapidx);
		if( itr!=m_cBlock.end() )
		{
			WORD* combined = itr->second;	

			finder = GetPathFinder(mapidx);
			if( finder )
			{
				SetPathFinderData(*finder, 
					combined,
					tempmap->m_zonedata_header->width,
					tempmap->m_zonedata_header->height);
				if( pNumGuide!=0 && pNumGuide!=finder->GetGuideCount() )
				{
					TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("ERR: Map %i, pathfind.DAT(GP=%i) does not match zoneinfo.DAT(GP=%i).  Export map again please."),
						mapidx.map_id, pNumGuide, finder->GetGuideCount());
					return NULL;
				}
				if( pNumGuide==0 )
					finder->PreCalculate();
				else
					finder->PreCalculate(pPathFindData.weightData, pPathFindData.distData, pPathFindData.linkData);
			}
			else
			{
				finder = SafeCreateObject( PathFinder );
				SetPathFinderData(*finder, 
					combined,
					tempmap->m_zonedata_header->width,
					tempmap->m_zonedata_header->height);
				if( pNumGuide!=0 && finder->GetGuideCount()!=pNumGuide )
				{
					TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("ERR: Map %i, pathfind.DAT(GP=%i) does not match zoneinfo.DAT(GP=%i).  Export map again please."),
						mapidx.map_id, pNumGuide, finder->GetGuideCount());
					return NULL;
				}
				if( pNumGuide==0 )
					finder->PreCalculate();
				else
					finder->PreCalculate(pPathFindData.weightData, pPathFindData.distData, pPathFindData.linkData);
				m_pathfinder.insert(std::make_pair(mapidx, finder));
			}
			return finder;
		}
	}
	return NULL;
}

void Global::DestroyGameContext()
{
	for (MapMapPathFinder::iterator itr5 = m_pathfinder.begin(); itr5!=m_pathfinder.end(); ++itr5)
	{
		SafeDeleteObject(itr5->second);
	}
	m_pathfinder.clear();
	m_objstate.clear();
}

BOOL Global::ChangeDynamicMapOBJ(Int objid, Int state, Int16 lineid)
{
	PathFindLoadData dummy;
	if( SetOBJState(objid, state, lineid) )
	{
		// for each OBJ state in map & line
		for (MapIndexSet::iterator it = gConfigure.mapIndices.begin(); it != gConfigure.mapIndices.end(); ++it)
		{
			MapIndex& mapidx = (*it);
			if( mapidx.line_id==lineid )
			{
				MakeFinalBlock(mapidx);
				MakeMapPathFinder(mapidx, dummy, 0);
			}
		}
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Dynamic Change OBJ: %i %i"), objid, state );
		return TRUE;
	}
	return FALSE;
}

WORD* Global::MakeFinalBlock(MapIndex mapidx)
{
	WORD* combined = NULL;
	MapDB* tempmap = const_cast<MapDB*>(m_resource->GetMapData(mapidx.map_id));
	if( tempmap )
	{
		Int mapheight = tempmap->m_zonedata_header->height;
		Int mapwidth  = tempmap->m_zonedata_header->width;

		BOOL found = false;
		MapCombinedBlock::iterator itr = m_cBlock.find(mapidx);
		if( itr!=m_cBlock.end() )
		{
			combined = itr->second;	
			found = true;
		}else
			combined = SafeAllocate( WORD, mapheight * mapwidth * sizeof(WORD) );

		Int sizeoflist = (Int)tempmap->m_zonedata.size();
		for( Int i = 0; i<sizeoflist; i++ )
			combined[i] = tempmap->m_zonedata[i]->sBlockType;

		// for each OBJ instance
		for( ObjInstList::iterator itr_obj = tempmap->m_obj.begin(); itr_obj!=tempmap->m_obj.end(); ++itr_obj )
		{
			UInt8 tempstate;
			// for each OBJ state in map & line
			MapMapOBJState::iterator itr_objstm = m_objstate.find(mapidx);
			if( itr_objstm!=m_objstate.end() )
			{
				MapOBJState & objstate = itr_objstm->second;
				MapOBJState::iterator itr_objst = objstate.find((*itr_obj)->m_objid);
				if( itr_objst!=objstate.end() )
				{
					while( itr_objst!=objstate.end() )
					{
						tempstate = itr_objst->second;
						// combine current obj in map
						String tempfilename;
						tempfilename.Format(_T("%s.blk"), (*itr_obj)->m_filename);
						MergeOBJBlockingToMap(combined,
							tempfilename.c_str(), 
							tempmap->m_zonedata_header->width, 
							tempmap->m_zonedata_header->height, 
							(*itr_obj)->m_posx, 
							(*itr_obj)->m_posy,
							tempstate
							);
						itr_objst++;
					}
				}else
				{
					tempstate = 0; // default state: IDLE=0
					objstate.insert(std::make_pair((*itr_obj)->m_objid, tempstate));
					// combine current obj in map
					String tempfilename;
					tempfilename.Format(_T("%s.blk"), (*itr_obj)->m_filename);
					MergeOBJBlockingToMap(combined,
						tempfilename.c_str(), 
						tempmap->m_zonedata_header->width, 
						tempmap->m_zonedata_header->height, 
						(*itr_obj)->m_posx, 
						(*itr_obj)->m_posy,
						tempstate
						);
				}
			}else
			{
				tempstate = 0; // default state: IDLE=0
				MapOBJState objstate;
				objstate.insert(std::make_pair((*itr_obj)->m_objid, tempstate));
				m_objstate.insert(std::make_pair(mapidx, objstate));

				// combine current obj in map
				String tempfilename;
				tempfilename.Format(_T("%s.blk"), (*itr_obj)->m_filename);
				MergeOBJBlockingToMap(combined,
					tempfilename.c_str(), 
					tempmap->m_zonedata_header->width, 
					tempmap->m_zonedata_header->height, 
					(*itr_obj)->m_posx, 
					(*itr_obj)->m_posy,
					tempstate
					);
			}
		}

		if( !found )
			m_cBlock.insert( std::make_pair(mapidx, combined) );
		return combined;
	}
	return NULL;
}

BOOL Global::MergeOBJBlockingToMap(WORD* mapblock, String objfilename, Int mapwidth, Int mapheight, Int objpos_x, Int objpos_y, Int state)
{
	std::vector<BlockPos> * objblock = m_resource->LoadOBJBlocking(objfilename.c_str(), state);
	if( objblock )
	{
		std::vector<BlockPos>::iterator itr_block = objblock->begin();
		while( itr_block!=objblock->end() )
		{
			Int relativeX = itr_block->PosX;
			Int relativeY = itr_block->PosY;
			Int pos = (relativeY+objpos_y) * mapwidth + relativeX + objpos_x;
			if( !(mapblock[pos] & TILE_IN_BLOCK_VALUE) )
				mapblock[pos] = mapblock[pos] + TILE_IN_BLOCK_VALUE;
			if( mapblock[pos] & TILE_IN_NAVIGATION_VALUE )
				mapblock[pos] = mapblock[pos] - TILE_IN_NAVIGATION_VALUE;

			itr_block++;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL Global::SetOBJState(Int id, Int state, Int16 lineid)
{
	bool found = false;
	// for each OBJ state in map & line
	for( MapMapOBJState::iterator itr_objstm = m_objstate.begin(); itr_objstm!=m_objstate.end(); ++itr_objstm )
	{
		MapIndex mapidx = itr_objstm->first;
		if( mapidx.line_id==lineid )
		{
			MapOBJState & objstate = itr_objstm->second;
			for( MapOBJState::iterator itr_objst = objstate.find(id); itr_objst!=objstate.end(); ++itr_objst )
			{
				itr_objst->second = state;
				found = true;
			}
		}
	}
	if( found )
		return TRUE;
	return FALSE;
}
*/

/*
NpcData* Global::GetNpcData(UInt32 npcid)
{
	NpcData* npc = const_cast<NpcData*>(m_resource->GetNpcData(npcid));
	if( npc )
		return npc;
	return NULL;
}
*/

void Global::LoadNewScript()
{
	StringA luaFile;
	StringA strPath;
	WIN32_FIND_DATAA data = {0};
	HANDLE hFile;

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Init: Reading Lua File for New Script System ..."));

	if (g_pLuaState == NULL)
		InitLua(g_pLuaState);

	if (g_pScriptState == NULL)
		InitLua(g_pScriptState);

	// load QGL_LIB.lua script function
	RunLua(g_pLuaState, QGLLIB_LUA);
	RunLua(g_pScriptState, QGLLIB_LUA);
	

	// load all skill lua file
	luaFile.Format("%s%s", SKILL_LUA_INIT, GLOBAL_SKILL_FUNC);
	RunLua(g_pLuaState, luaFile.c_str());
	RunLua(g_pScriptState, luaFile.c_str());

	SkillDataList skillList;
	m_resource->GetSkillList(skillList);
	for (SkillDataList::iterator it = skillList.begin(); it != skillList.end(); ++it)
	{
		luaFile.Format("%s%sS.lua", SKILL_LUA_INIT, GetPrefixStr((*it)->id, "skill").c_str());
		if (!IsFileExist(luaFile.c_str()))
			continue;
		RunLua(g_pLuaState, luaFile.c_str());
		RunLua(g_pScriptState, luaFile.c_str());
	}

	// load all buff lua file
	luaFile.Format("%s%s", BUFF_LUA_INIT, GLOBAL_BUFF_FUNC);
	RunLua(g_pLuaState, luaFile.c_str());
	RunLua(g_pScriptState, luaFile.c_str());

	BuffDataList buffList;
	m_resource->GetBuffList(buffList);
	for (BuffDataList::iterator it = buffList.begin(); it != buffList.end(); ++it)
	{
		luaFile.Format("%s%sS.lua", BUFF_LUA_INIT, GetPrefixStr((*it)->id, "buff").c_str());
		if (!IsFileExist(luaFile.c_str()))
			continue;
		RunLua(g_pLuaState, luaFile.c_str());
		RunLua(g_pScriptState, luaFile.c_str());
	}

	// load all battle and AI lua file
	RunLua(g_pLuaState, BATTLE_LUA_INIT_FILE);
	RunLua(g_pScriptState, BATTLE_LUA_INIT_FILE);
	strPath.Format("%s\\*.lua", LUA_AI_PATH);
	hFile = FindFirstFileA(strPath.c_str(), &data);
	while (hFile != INVALID_HANDLE_VALUE)
	{
		luaFile.Format("%s\\%s", LUA_AI_PATH, data.cFileName);
		RunLua(g_pLuaState, luaFile.c_str());
		RunLua(g_pScriptState, luaFile.c_str());
		if (!FindNextFileA(hFile, &data))
			hFile = INVALID_HANDLE_VALUE;
	}
	FindClose(hFile);

	CMap* pMap;
	for (MapIndexList::iterator it = m_mapIndices.begin(); it != m_mapIndices.end(); ++it)
	{
		pMap = it->second;
		if (pMap)
			pMap->LoadAllHints();
	}
}

void Global::LoadAllScript()
{
	LoadNewScript();
}

void Global::InitLua(lua_State* &lstate)
{
	lstate = luaL_newstate();
	TRACE_ENSURE(lstate);
	luaL_openlibs(lstate);
	luaopen_LuaI(lstate);
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("lua status %p top %d"), 
		lstate, lua_gettop(lstate));									
}

void Global::ReleaseLua(lua_State* &lstate)
{
	if (lstate) {
		lua_close(lstate);
		lstate = NULL;
	}
}

Boolean Global::LoadLua(lua_State* lstate, PCSTRINGA filename, Boolean bLog)
{
	if (filename == NULL)
		return BOOLEAN_FALSE;

	StringA luaFile = filename;				// uncompiled lua file
	StringA outFile = filename;				// compiled lua file
	PCSTRINGA existFile = NULL;
	luaFile.Replace(LUA_BIN_EXT, LUA_SRC_EXT);
	outFile.Replace(LUA_SRC_EXT, LUA_BIN_EXT);
	
	if (IsFileExist(luaFile.c_str()))			// run .lua file first
		existFile = luaFile.c_str();
	else if (IsFileExist(outFile.c_str()))		// if not exist, run .out file
		existFile = outFile.c_str();
	if (existFile == NULL && bLog) {
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("lua file %s not found"), 
			String(filename).c_str());
		return BOOLEAN_FALSE;
	}

	int r = luaL_loadfile(lstate, existFile);
	if (r != 0)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("lua file %s, %s"),
			String(existFile).c_str(), String(lua_tostring(lstate, -1)).c_str());
		lua_pop(lstate, 1);
		return BOOLEAN_FALSE;
	}

	return BOOLEAN_TRUE;
}

Boolean Global::RunLua(lua_State* lstate, PCSTRINGA filename)
{
	if (!LoadLua(lstate, filename, BOOLEAN_TRUE))
		return BOOLEAN_FALSE;

#ifdef LUA_STACK_TRACE_SERVER	
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("function before pcall %s, state %d, top %d"), 
			String(filename).c_str(), lua_status(lstate), lua_gettop(lstate));								
#endif // LUA_STACK_TRACE_SERVER

	int r = lua_pcall(lstate, 0, LUA_MULTRET, 0);

#ifdef LUA_STACK_TRACE_SERVER	
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("after pcall %s, state %d, top %d"), 
		String(filename).c_str(), lua_status(lstate), lua_gettop(lstate));								
#endif // LUA_STACK_TRACE_SERVER

	if (r == 0 && lua_gettop(lstate) != 0)
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("gettop Not zero function after pcall \"%s\", state %d, top %d"), 
			String(filename).c_str(), lua_status(lstate), lua_gettop(lstate));								
	}
	if (r != 0)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("lua file %s"), String(filename).c_str());
		TRACE_ERRORDTL(GLOBAL_LOGGER, String(lua_tostring(lstate, -1)).c_str());
		lua_pop(lstate, 1);
		return BOOLEAN_FALSE;
	}

	return BOOLEAN_TRUE;
}

Boolean Global::LoadLuaBuffer(lua_State* lstate, PCSTRINGA buf, size_t len, PCSTRINGA name)
{
	if (buf == NULL)
		return BOOLEAN_FALSE;

	int r = luaL_loadbuffer(lstate, buf, len, name);
	if (r != 0)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("lua buffer name %s"), 
			String(name).c_str());
		TRACE_ERRORDTL(GLOBAL_LOGGER, String(lua_tostring(lstate, -1)).c_str());
		lua_pop(lstate, 1);
		return BOOLEAN_FALSE;
	}

	return BOOLEAN_TRUE;
}

void Global::NewLuaThread(lua_State* &lstate, lua_State* parent, int &refKey)
{
	if (parent == NULL)
		return;

	//printf("NewLuaThread: garbage collection memory %d\n", lua_gc(parent, LUA_GCCOUNT, 0));
	//lua_gc(parent, LUA_GCCOLLECT, 0);
	//int count = lua_gc(parent, LUA_GCCOUNTB, 0);

	lstate = lua_newthread(parent);
	if (lstate == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("parent 0x%p"), parent);
		TRACE_ERRORDTL(GLOBAL_LOGGER, String(lua_tostring(parent, -1)).c_str());
		lua_pop(parent, 1);
		return;
	}

	lua_pushvalue(parent, -1);				// copy thread for setfenv
	refKey = luaL_ref(parent, LUA_REGISTRYINDEX);	// reference key for gc

	// copy parent state global table to separate global table for each thread:
    // local newgt = {__index = _G}			-- create new environment and act as metatable
    // setmetatable(newgt, newgt)
    // setfenv(thread, newgt)				-- set environment
	lua_newtable(parent);						// new newgt table
	lua_pushvalue(parent, -1);
	lua_pushvalue(parent, LUA_GLOBALSINDEX);
	lua_setfield(parent, -2, "__index");
	lua_setmetatable(parent, -2);				// set itself as metatable
	lua_setfenv(parent, -2);
	lua_pop(parent, 1);							// pop thread object

	TRACE_VERBOSEDTL_3(GLOBAL_LOGGER,
		_F("parent 0x%p, state 0x%p, refKey %d"),
		parent, lstate, refKey);
}

void Global::ReleaseLuaThread(lua_State* &lstate, lua_State* parent, int refKey)
{
	//static int gc_step = 0;

	if (lstate == NULL || parent == NULL)
		return;

	//int count = lua_gc(parent, LUA_GCCOUNTB, 0);
	luaL_unref(parent, LUA_REGISTRYINDEX, refKey);
	//gc_step++;
	//if (gc_step >= LUA_GC_STEP)
	//{
	//	gc_step = 0;
	//	lua_gc(parent, LUA_GCCOLLECT, 0);
	//	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("ReleaseLuaThread: gc_step >= LUA_GC_STEP, gc_step: %d"),
	//	gc_step);
	//}
	TRACE_VERBOSEDTL_3(GLOBAL_LOGGER,
		_F("parent 0x%p, state 0x%p, refKey %d"),
		parent, lstate, refKey);

	lstate = NULL;
}

int Global::LuaResume(lua_State* lstate)
{
#ifdef LUA_STACK_TRACE_SERVER
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("before resume, state 0x%p, status %d, top %d"), 
		lstate, lua_status(lstate), lua_gettop(lstate));
#endif // LUA_STACK_TRACE_SERVER
	int r = lua_resume(lstate, 0);
#ifdef LUA_STACK_TRACE_SERVER
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Lua status: Global::LuaResume after resume state %d, top %d"), 
			 lua_status(lstate), lua_gettop(lstate));
#endif // LUA_STACK_TRACE_SERVER
	if (r == 0 && lua_gettop(lstate) > 0)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("Lua status: Global::LuaResume after resume state not zero %d, top %d"), 
			 lua_status(lstate), lua_gettop(lstate));
	}
	if (r != 0 && r != LUA_YIELD)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("LuaResume Error: %d"), r);
		TRACE_ERRORDTL(GLOBAL_LOGGER, String(lua_tostring(lstate, -1)).c_str());
		lua_pop(lstate, 1);
	}

	return r;
}

StringA	Global::GetPrefixStr(UInt16 ID, StringA pre)
{
	StringA Result;
	Result.Format("%s%04i", pre.c_str(), ID);
	return Result;
}

void Global::LinkView(ViewManager *viewMan)
{
	if (gGlobal.m_viewRing == NULL || viewMan->m_ringBelong)
		return;

	ViewRing* minRing = gGlobal.m_viewRing;
	ViewRing* curRing = gGlobal.m_viewRing->next;

	while (curRing && curRing != gGlobal.m_viewRing) {
		if (curRing->count < minRing->count)
			minRing = curRing;
		curRing = curRing->next;
	}

	if (minRing->child == NULL) {
		minRing->child = viewMan;
	} else {
		ViewManager *curView = minRing->child;
		while (curView->m_nextView)
			curView = curView->m_nextView;
		curView->m_nextView = viewMan;
	}
	viewMan->m_ringBelong = minRing;
	viewMan->m_nextView = NULL;
	minRing->count++;
}

void Global::UnlinkView(ViewManager *viewMan)
{
	if (viewMan == NULL || viewMan->m_ringBelong == NULL)
		return;

	ViewManager *prevView = NULL;
	ViewManager *curView = viewMan->m_ringBelong->child;
	while (curView && curView != viewMan) {
		prevView = curView;
		curView = curView->m_nextView;
	}
	if (prevView == NULL)
		viewMan->m_ringBelong->child = viewMan->m_nextView;
	else if (curView)
		prevView->m_nextView = curView->m_nextView;
	else {
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("UnlinkView Error: viewMan %d not found in link"),
			viewMan);
		return;
		//prevView->m_nextView = NULL;
	}
	viewMan->m_ringBelong->count--;
	viewMan->m_ringBelong = NULL;
	viewMan->m_nextView = NULL;
}

void Global::ChgLoadLevel(UInt8 level)
{
	if (level > m_loadLevel) {				// increase ring when load level increased
		// add new ring to last ring
		ViewRing *prevRing = m_viewRing;
		ViewRing *pRing = m_viewRing->next;
		while (pRing != m_viewRing) {
			prevRing = pRing;
			pRing = pRing->next;
		}
		ViewRing* newRing = SafeCreateObject(ViewRing);
		prevRing->next = newRing;
		newRing->next = pRing;
	} else if (level < m_loadLevel) {		// decrease ring when load level decreased
		// remove next ring from ring list first
		ViewRing *oldRing = m_viewRing->next;
		m_viewRing->next = oldRing->next;

		// put back all old ring viewManager into others
		ViewManager* viewMan = oldRing->child;
		ViewManager* nextViewMan;
		while (viewMan) {
			nextViewMan = viewMan->m_nextView;
			viewMan->m_ringBelong = NULL;
			LinkView(viewMan);
			viewMan = nextViewMan;
		}
		SafeDeleteObject(oldRing);
	}
	m_loadLevel = level;
}

BOOL Global::CanUsedName(String name)
{
	BlockWordDataList* blockWord = gGlobal.m_resource->GetNameBlockWordList();
	if (blockWord == NULL)
		return false;

	String checkingWord;// = nickName;
	String space = _T(" ");
	int spaceIndex = 0;
	bool cantUse = false;//cannot use

	spaceIndex = (int)name.find(space.c_str(), spaceIndex);
	if (spaceIndex < 0)
	{
		checkingWord = name;
		checkingWord = checkingWord.ToLower();

		for (UInt16 j = 0; j < blockWord->size(); ++j)
		{
			int tempPos = (int)checkingWord.find(blockWord->at(j).c_str());
			if ( tempPos >= 0)
			{
				cantUse = true;	
				break;
			}
		}
	}
	else
		cantUse = true;	

	return (!cantUse);
}

void Global::UpdateMsgChar(CCharacter* pChar)
{
	if (pChar == NULL)
		return;

	CMsgCharacterData *pData;
	MsgCharDataMap::iterator it = m_msgCharMap.find(pChar->GetChar_id());

	if (it != m_msgCharMap.end()) {
		pData = it->second;
		pChar->GetMsgCharData(pData);
	} else {
		pData = (CMsgCharacterData*) Reuben::Simulation::CreateObject(MsgCharacterData_CLASSID);
		pData->ClearAllMask();
		pChar->GetMsgCharData(pData);
		m_msgCharMap[pChar->GetChar_id()] = pData;
	}
}

void Global::SendMsgChars()
{
	if (m_msgCharMap.empty())
		return;

	CMsgCharacterData *pData;
	NetGroup grp;
	RPCContext context;

	// send all MsgCharacterData to message service (max 20 each for buffer size)
	for (MsgCharDataMap::iterator it = m_msgCharMap.begin(); it != m_msgCharMap.end(); ++it)
	{
		pData = it->second;
		if (pData == NULL)
			continue;
		pData->SetSendMask(pData->GetUpdateMask());
		grp.Add(pData);
		if (grp.GetCount() >= 100) {
			MessageService_Proxy::UpdateCharInfo(pMessageService->pPeer, context, &grp);
			DeleteNetChilds(&grp);
		}
	}
	if (grp.GetCount() > 0) {
		MessageService_Proxy::UpdateCharInfo(pMessageService->pPeer, context, &grp);
		DeleteNetChilds(&grp);
	}
	m_msgCharMap.clear();
}

UInt8 Global::GetMapType(UInt16 map_id)
{
	const MapDB* mapdb = m_resource->GetMapData(map_id);
	if (mapdb)
		return mapdb->m_basicinfo->m_maptype;
	else
		return 0;
}

UInt32 Global::GetMapOwnerId(UInt16 map_id, CCharacter* pchar)
{
	const MapDB* mapdb = m_resource->GetMapData(map_id);
	if (pchar && mapdb)
	{
		if (mapdb->m_basicinfo->m_maptype == MAP_TYPE_INSTANCE_MANOR)
			return pchar->GetGuildUid();
		else if (mapdb->m_basicinfo->m_maptype == MAP_TYPE_INSTANCE_DUNGEON)
		{
			CParty* pParty = pchar->GetParty();
			return (pParty ? pParty->GetLeader()->GetChar_id() : pchar->GetChar_id());
		}
		else if (mapdb->m_basicinfo->m_maptype == MAP_TYPE_INSTANCE_HOUSE)
			return pchar->GetChar_id();
	}
	return 0;
}

PCSTRING Global::GetNpcName(UInt32 npc_id)
{
	const NpcData* npcData = gGlobal.m_resource->GetNpcData(npc_id);
	if (npcData) {
		return npcData->mid_name;
	} else {
		return _T("");
	}
}

PCSTRING Global::GetMobName(UInt32 mob_id)
{
	const MobData* mobData = gGlobal.m_resource->GetMobData(mob_id);
	if (mobData) {
		return mobData->mid_name;
	} else {
		return _T("");
	}
}

PCSTRING Global::GetMapName(UInt16 map_id)
{
	const MapDB* mapData = gGlobal.m_resource->GetMapData(map_id);
	if (mapData) {
		return mapData->m_basicinfo->m_mapname;
	} else {
		return _T("");
	}
}

PCSTRING Global::GetItemName(UInt32 item_id)
{
	const ItemData* itemData = gGlobal.m_resource->GetItemData(item_id);
	if (itemData) {
		return itemData->name;
	} else {
		return _T("");
	}
}

PCSTRINGA ConvAnsiString(const String& s)
{
	static char buf[1024];
	int len;

	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), (int)s.length(), 
			buf, 1024, 0, 0);
	if (len > 0)
		buf[len] = '\0';
	else
		buf[0] = '\0';

	return buf;
}

CCharacter* CharSessionRef::FindCharacter(UInt32 char_id)
{
	CharSessionMap::iterator itr = m_charToSession.find(char_id);
	if( itr != m_charToSession.end() )
	{
		Session* psess = itr->second;
		if( psess && psess->pchar )
			return psess->pchar;
	}
	return NULL;
}

void CharSessionRef::InsertCharToSession(UInt32 char_id, Session* session)
{
	m_charToSession.insert(std::make_pair(char_id, session));
}

void CharSessionRef::RemoveCharToSession(UInt32 char_id)
{
	m_charToSession.erase(char_id);
}

int UTF8ToStr(PCSTRINGA src, StringW &dst)
{
	if (src) {
		TCHAR buf[1024];
		int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, buf, 1024);
		buf[len] = _T('\0');
		dst.Format(_T("%s"), buf);
		return len;
	} else {
		dst = _T("");
		return 0;
	}
}

int StrToUTF8(PCSTRINGW src, StringA &dst)
{
	if (src)
	{
		char buf[1024];
		int len = WideCharToMultiByte(CP_UTF8, 0, src, -1, buf, 1024, NULL, NULL);
		buf[len] = '\0';
		dst.Format("%s", buf);
		return len;
	} else {
		dst = "";
		return 0;
	}
}

void Global::GetServiceTime(SYSTEMTIME& time)
{
	SYSTEMTIME st;
	CTimeSpan timespan;
	CTime outtime;
	GetLocalTime(&st);
	// World time = system - time on sync + world time when sync
	outtime = CTime(m_serviceSysTime) + (CTime(st) - CTime(m_syncTime));
	outtime.GetAsSystemTime(time);
}

void Global::GetServiceTime(time_t& time)
{
	SYSTEMTIME st;
	GetServiceTime(st);
	time = (time_t) CTime(st).GetTime();
}

UInt32 Global::GetServiceTime()
{
	SYSTEMTIME st;
	GetServiceTime(st);
	return ((UInt32) CTime(st).GetTime());
}

void Global::SetServiceTime(SYSTEMTIME st)
{
	m_serviceSysTime = st;
	GetLocalTime(&m_syncTime);
}

int Global::GetGameTime(int mode)
{
	Int16 wDH = 0;
	int daynight = 0;
	SYSTEMTIME st;

	GetServiceTime(st);
	if (gConfigure.gameDate.wHour > st.wHour )
		wDH = 24 - (gConfigure.gameDate.wHour - st.wHour);
	else
		wDH = (st.wHour - gConfigure.gameDate.wHour);
	 
	daynight = (((wDH % 2) * 60 + st.wMinute) / 10) % 12;
	
	if (mode == 0)			// Check Day , Night
		return (daynight < 3 || daynight > 8);
	else					// Get Game Time
		return daynight;
}




bool Global::IsBlockName(String &Msg)
{
	
	BlockWordDataList* blockList = m_resource->GetNameBlockWordList();
	if (blockList == NULL)
		return true;
	String space = _T(" ");
	
	for (UInt32 i = 0; i < blockList->size(); ++i)
	{
		int temp = (int)Msg.find(blockList->at(i).c_str());
		if (temp >= 0)
			return true;
	}
	return false;
}
VOID Global::BlockWord(String &Msg)
{
	BlockWordDataList* blockList = m_resource->GetMsgBlockWordList();
	if (blockList == NULL)
		return;

	String space = _T(" ");

	UInt8Vector link1Pos, link2Pos;
	UInt8Vector checkOpenClose;
	for (Index i = C_INDEX(0); i < Msg.size(); ++i)
	{
		if (Msg[i] == 15)
			checkOpenClose.push_back(i);
		else if (Msg[i] == 16 && checkOpenClose.size() > 0)
		{
			link1Pos.push_back(checkOpenClose.back());
			link2Pos.push_back(i);
			checkOpenClose.pop_back();
		}
	}
	for (UInt32 i = 0; i < blockList->size(); ++i)
	{
		int temp = (int)Msg.find(blockList->at(i).c_str());
		UInt32 j = 0;
		UInt32 msgSize = (UInt32)Msg.size();
		while (temp >= 0 && j < msgSize )
		{
			UInt pos = 0;
			bool ignor = false;
			for (Index a = C_INDEX(0); a < link1Pos.size(); ++a)
			{
				if (link1Pos[a] < temp && link2Pos[a] > temp)
				{
					ignor = true;
					pos = a;
					break;
				}
			}

			if (!ignor)
				Msg.replace(temp, blockList->at(i).length(), space.c_str());
			else
				temp += link2Pos[pos];
			temp = (int)Msg.find(blockList->at(i).c_str(), temp + 1);
			++j;
		}
	}
}

void Global::LoadQuestJournal()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Quest Journal ..."));

	CQuestJournal* pJournal;
	JournalDataList journalList;

	m_resource->LoadJournalCSV();
	m_resource->GetJournalList(journalList);
	DeleteNetChilds(&m_journalGroup);

	for (JournalDataList::const_iterator it = journalList.begin(); 
		it != journalList.end(); ++it)
	{
		pJournal = CQuestJournal::NewInstance();
		TRACE_ENSURE(pJournal);
		pJournal->SetData(*it);
		m_journalGroup.Add(pJournal);
	}
}

UInt Global::NoCodeStringSize(String &Text)
{
	Int		Result = 0;
	bool	HideNow = false;

	for(Int i = 0; i < (Int)Text.size(); ++i)
	{
		if(Text[i] == 15)
		{
			HideNow = true;
			continue;
		}
		else
		if(Text[i] == 16)
		{
			HideNow = false;
			continue;
		}
		else
		if(Text[i] <= 26 || HideNow)
		{
			continue;
		}
		
		++Result;
	}

	return Result;
}

const ItemData *	Global::GetItemCsvData(UInt32 item_id)
{
	const ItemData * pItemData = gGlobal.m_resource->GetItemData(item_id);
	return pItemData;
}

VOID Global::StartGuildWar()
{
	for (MapIndexList::iterator Itr = m_mapIndices.begin(); Itr !=m_mapIndices.end(); Itr ++)
	{
		Itr->second->StartGuildWar();
		Itr->second->CheckGuildWarResult(false);
	}
}
VOID Global::EndGuildWar()
{
	for (MapIndexList::iterator Itr = m_mapIndices.begin(); Itr !=m_mapIndices.end(); Itr ++)
	{
		Itr->second->CheckGuildWarResult(true);
		Itr->second->CancelUniqEvent(CMap::EVT_UPDATE_GUILD_WAR);
	}
}
 
UInt32 Global::GetRealDay(UInt32 time)
{
	time_t now = time==0?::time(NULL):time;
	struct tm* ptime = localtime(&now);
	return ptime->tm_year + 1900;
}
UInt32 Global::GetRealMonth(UInt32 time)
{
	time_t now = time==0?::time(NULL):time;
	struct tm* ptime = localtime(&now);
	return ptime->tm_mon + 1;
}
UInt32 Global::GetRealYear(UInt32 time)
{
	time_t now = time==0?::time(NULL):time;
	struct tm* ptime = localtime(&now);
	return ptime->tm_mday;
}
UInt32 Global::GetRealHour(UInt32 time)
{
	time_t now = time==0?::time(NULL):time;
	struct tm* ptime = localtime(&now);
	return ptime->tm_hour;
}
UInt32 Global::GetRealMinute(UInt32 time)
{
	time_t now = time==0?::time(NULL):time;
	struct tm* ptime = localtime(&now);
	return ptime->tm_min;
}

PCSTRINGA Global::ConvertUTF8(const String &src)
{	
	static char buf[1024];	StringA dst;	
	StrToUTF8(src.c_str(), dst);	
	strcpy(buf, dst.c_str());	
	return buf;
}

void Global::AddCharInIncognitoPkLstOne(UInt32 char_id)
{
	if(g_charParty1.size() < 5)
		g_charParty1.push_back(char_id);
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("AddCharInIncognitoPkLstOne party1:add charId =  %d"), 
		char_id);
}

void Global::AddCharInIncognitoPkLstTwo(UInt32 char_id)
{
	if(g_charParty2.size() < 5)
		g_charParty2.push_back(char_id);
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("AddCharInIncognitoPkLstOne party2:add charId =  %d"), 
		char_id);
}

void Global::CharLstCreateParty()
{
	if(g_charParty1.size() > 0)
	{
		UInt32 charId = g_charParty1[0];
		CCharacter* leaderChar = m_charToSession.FindCharacter(charId);
		CParty* partyOne = leaderChar->CreateParty();
		if(leaderChar && partyOne)
		{
			for (int i = 1; i < g_charParty1.size(); i++)
			{
				charId = g_charParty1[i];
				CCharacter* memberChar = m_charToSession.FindCharacter(charId);
				if(memberChar)
				{
					partyOne->Join(memberChar,true);
					TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("CharLstCreateParty party1:Jion charId =  %d"), 
						memberChar->GetChar_id());
				}
			}
		}
	}
	if(g_charParty2.size() > 0)
	{
		UInt32 charId = g_charParty2[0];
		CCharacter* leaderChar = m_charToSession.FindCharacter(charId);
		CParty* partyOne = leaderChar->CreateParty();
		if(leaderChar && partyOne)
		{
			for (int i = 1; i < g_charParty2.size(); i++)
			{
				charId = g_charParty2[i];
				CCharacter* memberChar = m_charToSession.FindCharacter(charId);
				if(memberChar)
				{
					partyOne->Join(memberChar,true);
					TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("CharLstCreateParty party2:Jion charId =  %d"), 
						memberChar->GetChar_id());
				}
			}
		}
	}
}

void Global::CharLstLeaveParty()
{
	if(g_charParty1.size() > 0)
	{
		for (int i = 0; i < g_charParty1.size(); i++)
		{
			UInt32 charId = g_charParty1[i];
			CCharacter* memberChar = m_charToSession.FindCharacter(charId);
			if(memberChar && memberChar->GetParty())
			{
				memberChar->GetParty()->Leave(memberChar->GetPeer(),memberChar->GetContext());
				TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("CharLstCreateParty party1:level charId =  %d"), 
					memberChar->GetChar_id());
			}
		}
	}
	if(g_charParty2.size() > 0)
	{
		for (int i = 0; i < g_charParty2.size(); i++)
		{
			UInt32 charId = g_charParty2[i];
			CCharacter* memberChar = m_charToSession.FindCharacter(charId);
			if(memberChar && memberChar->GetParty())
			{
				memberChar->GetParty()->Leave(memberChar->GetPeer(),memberChar->GetContext());
				TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("CharLstCreateParty party2:level charId =  %d"), 
					memberChar->GetChar_id());
			}
		}
	}
}

void  Global::SetCharLstBattleOver()
{
	g_charParty1.clear();
	g_charParty2.clear();
	g_IncognitoPKTimeOver = true;
}

void Global::StartIncognitoPK()
{
	g_IncognitoPKTimeOver = false;
	RemoveOutCharInParty();
	if (g_charParty1.size() > 0 && g_charParty2.size() > 0)
	{
		CharLstCreateParty();
		UInt32 charId1 = g_charParty1[0];
		UInt32 charId2 = g_charParty2[0];
		CCharacter* leaderChar1 = m_charToSession.FindCharacter(charId1);
		CCharacter* leaderChar2 = m_charToSession.FindCharacter(charId2);
		if (leaderChar1 && leaderChar2)
		{
			PROCEDURE_DO_METHOD3(CCharacter, DoEncounterBattle, leaderChar1, leaderChar1->GetContext(), 
				UInt32, leaderChar2->GetChar_id(), RPCNetID, leaderChar2->GetNetId(), UInt8, INCOGNITO_PK_BATTLE)
		}
	}
}

void Global::RandomIncognitoBox(UInt16 map_id,UInt8 boxType)
{
	CMap* pMap;
	for (MapIndexList::iterator it = m_mapIndices.begin(); it != m_mapIndices.end(); ++it)
	{
		pMap = it->second;
		if (pMap->GetMap_id() == map_id)
			pMap->RandomIncognitoBox(boxType);
	}
}

void Global::RemoveOutCharInParty()
{
	if (g_charParty1.size() > 0)
	{
		CharIncognitoPkLst::iterator it = g_charParty1.begin();
		while (it != g_charParty1.end())
		{
			UInt32 charID = *it;
			CCharacter* memberChar = m_charToSession.FindCharacter(charID);
			if (memberChar == NULL)
				it = g_charParty1.erase(it);
			else
				it++;
		}
	}
	if (g_charParty2.size() > 0)
	{
		CharIncognitoPkLst::iterator it = g_charParty2.begin();
		while (it != g_charParty2.end())
		{
			UInt32 charID = *it;
			CCharacter* memberChar = m_charToSession.FindCharacter(charID);
			if (memberChar == NULL)
				it = g_charParty2.erase(it);
			else
				it++;
		}
	}
}