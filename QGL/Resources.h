#pragma once
#ifndef RESOURCES_H_
#define RESOURCES_H_

#include "QGL.h"
#include "Reuben\Platform\File.h"
#include <ddraw.h>
#include <String.h>
namespace ZLib{
#include <zlib.h>
};

#define MaxEntSizeX 1024
#define MaxEntSizeY 1024

struct DDsHeader
{
	DWORD MagicValue;
	DDSURFACEDESC2 SurHeader;
};
struct MapPartRequestData
{
	MapData * pMapData;
	Int Index;
	LPDIRECT3DDEVICE9	pD3DDevice;
	MapPartRequestData()
	{
		memset(this, 0, sizeof(MapPartRequestData));
	}
};

struct GarbageData
{
	size_t sizeOfData;
	byte * pData;
};
struct ResetMapRequestData
{
	MapData * pMapData;
	RECT EffectArea;
	LPDIRECT3DDEVICE9	pD3DDevice;

	ResetMapRequestData()
	{
		pMapData = 0;
		EffectArea.left = EffectArea.right = EffectArea.top = EffectArea.bottom = 0;
	}
};

struct EntityLoaderData
{
	EntityPicInfo * EntData;
	LPDIRECT3DDEVICE9	pD3DDevice;
	//Reuben::Platform::File * file;

	EntityLoaderData()
	{
		EntData = NULL;
		pD3DDevice = NULL;
		//file = NULL;
	}
};

struct MapLoaderData
{
	MapData * pMapData;
	LPDIRECT3DDEVICE9	pD3DDevice;
	RECT EffectArea;

	MapLoaderData()
	{
		pMapData = NULL;
		pD3DDevice = NULL;
		EffectArea.left = EffectArea.right = EffectArea.top = EffectArea.bottom = 0;
	}
};

DWORD WINAPI ThreadFunc(LPVOID pData);


class CResourcesManager : public Reuben::Utility::Singleton<CResourcesManager>
{
private:
	CResourcesManager();
	SINGLETON(CResourcesManager);

private:

	//these would only be called in other thread or after the thread stopped
	VOID OnLoadEntityFile(EntityLoaderData * pEntityLoaderData);
	
	VOID OnResetMapResources(ResetMapRequestData * pResetMapRequestData);
	VOID OnReleaseMapResources(MapData * pMapData);
	VOID OnReleaseTexture(LPDIRECT3DTEXTURE9 GivenTexture);
	VOID OnReleaseEntityResources(EntityPicInfo * pEntData);
	VOID OnReleaseAllTexture();
	VOID OnMapPartResources(MapPartRequestData *pMapPartRequestData);
	VOID OnLoadMapFile(MapLoaderData * pMapLoaderData);

public:

	
	//construct and distruct
	~CResourcesManager();

	//loading the entity or StlMap file
	EntityPicInfo * LoadEntityResourcesFile(LPCTSTR filename, LPDIRECT3DDEVICE9	pD3DDevice);
	MapData * LoadMapResourcesFile(LPCTSTR filename, LPDIRECT3DDEVICE9	pD3DDevice, RECT EffectiveArea);

	//RLE Reading
	VOID ReadRLEFile(Byte* PicInfoBase, UInt32 iFilesize, Byte* RLEPic, UInt32 RLEFileSize,  
				 UInt32 PixelSize);

	//Release a Given Texture
	VOID ReleaseTexture(LPDIRECT3DTEXTURE9 GivenTexture);

	//create textured, every texture should be created from this function , in order to manage the memory
	LPDIRECT3DTEXTURE9 CreateTexture(Byte * pData, LPDIRECT3DDEVICE9	pD3DDevice, UInt32 SizeOfPic, UINT Width,
													UINT Height, D3DPOOL  Pool, UINT MipLevels, DWORD Usage, D3DFORMAT Format);


	inline Byte * GetEntPicCacheTexture() { return m_EntPicCacheTexture;};
	inline Byte * GetObjEntPicCacheTexture() { return m_ObjEntPicCacheTexture;};
	inline Byte * GetEntExtractCache(){ return m_EntExtractCache;}
	//inline Byte * GetWorldImage(){ return m_WorldImage;}

	VOID ResProcess();

	VOID StopThread();

	//request the map data while the camera is moving
	VOID ResetMapResourcesRequest(MapData * pMapData, RECT EffectArea, LPDIRECT3DDEVICE9	pD3DDevice);

	VOID ReleaseMapResources(MapData * pMapData);
	
	//load new color palette
	ColorPaletteStruct * CreateColorPalette(LPCTSTR filename);

	VOID ReleaseEntityData(EntityPicInfo * pEntData);

	//check the resources whether it is in use
	VOID CheckResources();

	//request only one part
	VOID MapPartRequest(MapData * pMapData, Index Index, LPDIRECT3DDEVICE9	pD3DDevice);

	//if data is added to the collector, the resources will be deleted after shut down
	template<class T>
	VOID AddToGarbageCollector(T * pData, size_t sizeofData);

	BOOL FreeGarbageRes(Byte * pData);

	VOID ReleaseEntityData(String FileName);

	VOID OnLoadAllColorPalette();
	
private:
	StlMap<String, EntityPicInfo *> AllLoadedEntityFile;
	StlMap<String, MapData *> AllLoadedMapFile;
	StlMap<String, ColorPaletteStruct> AllColorPalette;

	StlMap<UInt, LPDIRECT3DTEXTURE9 >  m_CreatedTexture;

	Byte *	m_EntPicCacheTexture;

	Byte * m_EntExtractCache;

	Byte * m_ObjEntPicCacheTexture;

	//Byte * m_WorldImage;

	DWORD ThreadID;

	HANDLE LoaderThread;

	//common resources that need to be locked
	StlVector<EntityLoaderData *> m_EntityFileReadDataLst;
	CRITICAL_SECTION EntityFileReadThreadLock; //lock of common source lst

	StlVector<MapLoaderData *> m_MapFileReadDataLst;
	CRITICAL_SECTION MapFileReadDataThreadLock; //lock of common source lst

	StlVector<ResetMapRequestData *> m_ResetMapRequestLst;
	CRITICAL_SECTION ResetMapRequestThreadLock; //lock of common source lst

	StlVector<MapData *> m_MapFileReleaseLst;
	CRITICAL_SECTION MapFileReleaseThreadLock; //lock of common source lst

	StlVector<EntityPicInfo *> m_EntityFileReleaseLst;
	CRITICAL_SECTION EntityFileReleaseThreadLock; //lock of common source lst

	StlVector<GarbageData > m_GarbageCollector;
	CRITICAL_SECTION GarbageCollectorThreadLock; //lock of common source lst

	StlVector<LPDIRECT3DTEXTURE9> m_ReleaseQueueLst;
	CRITICAL_SECTION ReleaseQueueThreadLock; //lock of common source lst

	StlVector<MapPartRequestData *> m_MapPartRequestLst;
	CRITICAL_SECTION MapPartRequestThreadLock; //lock of common source lst
	

public:
	CRITICAL_SECTION ThreadLock; //lock of common source lst

	BOOL m_BreakThreadLoop;

};
#endif