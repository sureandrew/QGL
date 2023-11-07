#pragma once
#ifndef _MAP_MANAGER_H_
#define _MAP_MANAGER_H_

/********************************************************
* CMapManager: this class manages maps, blocking and zone,
*              as well as NPC, OBJ, and JP inside.
*********************************************************/
struct MapDB;
struct BinaryClientMapInfo;
struct PathFindLoadData;
struct BinaryServerNPCInstance;
class CNpcManager;

class CMapManager
{
public:
	CMapManager();
	~CMapManager();

	const MapDB* GetMap(UInt32 mapid);

	INLINE VOID SetNpcManager(CNpcManager* npcmgr) { m_npcmgr = npcmgr; }

	BOOL LoadDAT(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);

	BOOL LoadAllNpc(const TCHAR *folder);
	BOOL LoadZoneDAT(UInt32 mapid, const TCHAR *folder); // load blocking data
	BOOL LoadPFDAT(UInt32 mapid, const TCHAR *folder, PathFindLoadData& pPathFindData, UInt32& pNumGuide); // load precomputed path-find data
	// load npc instance, obj instance, jumppoint instance
	BOOL LoadInstanceDAT(UInt32 mapid, const TCHAR *folder, Boolean bNpcOnly);

	BOOL LoadClientMapInfo(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);
	const BinaryClientMapInfo* GetClientMap(UInt32 mapid);
	BOOL GetNpcPosition(UInt32 map_id, UInt32 npc_id, UInt16 &x, UInt16 &y);
	BinaryServerNPCInstance* GetNpcInstance(UInt32 map_id, UInt32 npc_id);

	BOOL TestMapData(UInt32 map_id);

	VOID Release();

private:
	typedef StlMap<UInt32, MapDB*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, MapDB*> > > MapMapInt;

	typedef StlMap<UInt32, BinaryClientMapInfo*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, BinaryClientMapInfo*> > > ClientMapInfoMap;

	CNpcManager*		m_npcmgr; // for reference to npc information, not required.  if null, npc cannot be set properly
	MapMapInt			m_mapint;
	ClientMapInfoMap	m_cliMapInfo;

public:
	void GetMapList(MapMapInt& mapList);
};

#endif // _Map_MANAGER_H_