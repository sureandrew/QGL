#pragma once
#ifndef _MAP_OBJECT_VIEW_H_
#define _MAP_OBJECT_VIEW_H_

class CEntityBase;
class CAnimatedEntity;
class CMapNPCControl;
class CMapActorControl;
class CMapNPC;


class CMapObjectView
{
public:
	//construct and distruct
	CMapObjectView();
	~CMapObjectView();
	
	//a list of object entity inside the StlMap
	StlVector<CAnimatedEntity * >		m_MapObjectLst;

	//a list of other entity in the map(e.g Jump Point etc)
	StlMap<UInt, CEntityBase * >		m_MapEntLst;

	//a list of NPC in the map
	StlMap<UInt, CMapNPCControl * >		m_MapNPCCtrlLst;

	//storing the block index point to different regions
	StlMultiMap<UInt32,CAnimatedEntity *> m_BkObjectIndexLst;
	
	//check whether the StlMap frame is changed
	BOOL m_Updated;

	void AddLoadedModelName(String filename);
	void FreeAllLoadedModelName();

	StlVector<String> m_LoadedModel;

private:
	//reference rectangle in world map position
	RECT m_EffectiveRect;

	MapData * m_pMapData;

	//StlMap update count
	UInt32 m_MapCountTime;

	Int m_MapFrameIndex;

	Int m_MapPreFrameIndex;
	
	//sea depth block
	Byte m_BlockSeaDepth;

	String m_Filename;

public:
	//load StlMap data from file
	VOID LoadMapData(LPCTSTR filename, LPDIRECT3DDEVICE9	pD3DDevice, RECT EffectRect);

	//add object block to the StlMap where the object located
	VOID AddObjectBlockToMap(CAnimatedEntity * pObject);

	VOID AddAllObjectBlockToMap();

	//add StlMap object to the StlMap by reference
	VOID AddMapObject(CAnimatedEntity * pObject);
	
	//update all StlMap objects
	VOID UpDataMapObject(UInt32 CurrentTime);

	//add StlMap object by loading file
	VOID AddMapObject(LPCTSTR filename, LPDIRECT3DDEVICE9 pD3DDevice, Int PosX, Int PosY);

	//add footprInt32 positions to the StlMap
	VOID AddFootPrInt32Pos(BlockPos GivenPos, Int Rotation, Byte PicTyp);

	//remove footprInt32 position in the StlMap
	VOID RemoveFootPrInt32Pos(UInt32 Index);

	//update the StlMap
	VOID UpDate(UInt32 CurrentTime);

	//free Entity data if change map happens
	VOID FreeEntityData();

	//free map data
	VOID FreeMapData();

	//add and remove the entity in map, it won't release the data
	VOID AddEntityToMap(CEntityBase * pEntBase);
	VOID RemoveEntityFromMap(CEntityBase * pEntBase);

	VOID RemoveNPCFromMap(CMapActorControl * pCtrl);

	//add npc to map
	VOID AddNPCEntityToMap(CEntityBase * pEntBase);
	inline UInt32 GetMapWidth(){
		if (m_pMapData)
			return m_pMapData->Header.dwMapWidth;
		return 0;
	}

	inline UInt32 GetMapHeight(){
		if (m_pMapData)
			return m_pMapData->Header.dwMapHeight;
		return 0;
	}

	inline UInt16 GetSubMapWidth(){
		if (m_pMapData)
			return m_pMapData->Header.wSubMapWidth;
		return 0;
	}

	inline UInt16 GetSubMapHeight(){
		if (m_pMapData)
			return m_pMapData->Header.wSubMapHeight;
		return 0;
	}

	inline UInt16 GetMapMaxBlockX(){
		if (m_pMapData)
			return m_pMapData->MaxBlockX;
		return 0;
	}

	inline UInt16 GetMapMaxBlockY(){
		if (m_pMapData)
			return m_pMapData->MaxBlockY;
		return 0;
	}

	//Get the background map texture by index
	Byte * GetBgTextureByIndex(Int Index);

	//Get Night Bg Map
	Byte * GetNightTextureByIndex(Int Index);
	
	//check the current block type
	BOOL IsBlcok(BlockPos pos);
	BOOL IsBlockInAlpha(BlockPos pos);
	BOOL IsBlockInObject(BlockPos pos);
	BOOL IsBlockInShadow(BlockPos pos);
	BOOL IsBlockInSea(BlockPos pos);

	VOID SetEffectiveRect(RECT GivenRect);

	inline RECT GetEffectiveRect() { return m_EffectiveRect;}

	inline MapData * GetMapData(){ return m_pMapData;}

	inline Int GetMapFrameIndex(){ return m_MapFrameIndex;}

	inline Int GetMapPreFrameIndex(){ return m_MapPreFrameIndex;}

	inline Byte GetSeaDepth(){ return m_BlockSeaDepth;}

	VOID ReLoadMapData(LPDIRECT3DDEVICE9	pGivenD3DDevice);

	inline String GetFileName(){return m_Filename;}

	VOID InsertNPCToMap(CMapNPCControl * NPCCtrl);
	VOID RemoveNPCToFromMapByNetObj(CMapNPC * NPC);
	const StlMap<UInt, CMapNPCControl * >* GetNpcCtrlList() const { return &m_MapNPCCtrlLst; }
};

#endif // _MAP_VIEW_H_
