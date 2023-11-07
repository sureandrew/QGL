//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MapObjectView.h"
//-- Library
#include "Render.h"
#include "Configure.h"
#include "Global.h"
#include "Math.h"
#include "EntityBase.h"
#include "AnimatedEntity.h"
#include "MenuSystemSetting.h"
#include "MapNPCControl.h"
#include "Proxy/Logic/MapNPC.h"
#include "Resources.h"
#include "MapActorControl.h"

CONST Int MapFramePerSec = 250;

CMapObjectView::CMapObjectView()
{
	m_MapFrameIndex = 0;
	m_EffectiveRect.left = 0;
	m_EffectiveRect.right = 0;
	m_EffectiveRect.top = 0;
	m_EffectiveRect.bottom = 0;
	m_pMapData = NULL;
	m_BlockSeaDepth = 0;
	m_MapPreFrameIndex = 0;
}

CMapObjectView::~CMapObjectView()
{
	FreeEntityData();

	FreeMapData();

	m_BkObjectIndexLst.clear();

	m_Filename.Empty();

	FreeAllLoadedModelName();
}

VOID CMapObjectView::FreeMapData()
{
	if (m_pMapData)
		m_pMapData->Lock = FALSE;
	CResourcesManager::GetInstance().ReleaseMapResources(m_pMapData);
	m_pMapData = NULL;

	FreeAllLoadedModelName();
}
VOID CMapObjectView::FreeEntityData()
{
	StlVector<CAnimatedEntity *>::iterator MOItr = m_MapObjectLst.begin();
	while( MOItr != m_MapObjectLst.end())
	{
		(*MOItr)->FreeEntityData();
		SafeDeleteObject(*MOItr);
		MOItr++;
	}

	m_MapObjectLst.clear();

	StlMap<UInt, CEntityBase * >::iterator MEItr = m_MapEntLst.begin();
	while( MEItr != m_MapEntLst.end())
	{
		SafeDeleteObject(MEItr->second);
		MEItr++;
	}
	
	m_MapEntLst.clear();

	StlMap<UInt, CMapNPCControl * >::iterator MNPCItr = m_MapNPCCtrlLst.begin();
	while (MNPCItr != m_MapNPCCtrlLst.end())
	{
		MNPCItr->second->GetCharMainView()->FreeEntityData();
		SafeDeleteObject(MNPCItr->second);
		MNPCItr ++;
	}
	m_MapNPCCtrlLst.clear();
	m_BkObjectIndexLst.clear();
}
VOID CMapObjectView::LoadMapData(LPCTSTR filename, LPDIRECT3DDEVICE9	pGivenD3DDevice, RECT EffectRect)
{
	m_EffectiveRect = EffectRect;
	m_Filename = filename;
	m_pMapData = CResourcesManager::GetInstance().LoadMapResourcesFile(filename, pGivenD3DDevice, m_EffectiveRect);
	if (m_pMapData)
		m_pMapData->Lock = TRUE;
	m_BkObjectIndexLst.clear();
	AddAllObjectBlockToMap();
	m_MapFrameIndex = 0;
}

VOID CMapObjectView::ReLoadMapData(LPDIRECT3DDEVICE9	pGivenD3DDevice)
{
	m_pMapData = CResourcesManager::GetInstance().LoadMapResourcesFile(m_Filename.c_str(), pGivenD3DDevice, m_EffectiveRect);
	m_BkObjectIndexLst.clear();
	AddAllObjectBlockToMap();
	m_MapFrameIndex = 0;
}

BYTE * CMapObjectView::GetBgTextureByIndex(Int Index)
{
	if (m_pMapData == NULL || m_pMapData->m_pTextureBGLst == NULL)
		return NULL;

	if (Index >= m_pMapData->TotalPic * m_pMapData->totalSubFrame)
		return NULL;

	//find whether the index is inside the effect area
	if (m_pMapData->m_pTextureBGLst[Index].pDDSPicData == NULL)
	{
		if (m_pMapData->totalSubFrame == 0 ||
			GetSubMapWidth() == 0)
			return NULL;

		//find avaiable frame
		/*Int StartIndex = Index - Index % m_pMapData->totalSubFrame;
		for (Int i = StartIndex; i < StartIndex + m_pMapData->totalSubFrame; i++)
		{
			if ( m_pMapData->m_pTextureBGLst[i].pDDSPicData)
				return m_pMapData->m_pTextureBGLst[i].pDDSPicData;
		}

		Int RowPerPic = (Int)ceil(((Real32)GetMapWidth() / (Real32)GetSubMapWidth()));

		RECT MapRect;
		MapRect.left = Index / m_pMapData->totalSubFrame % RowPerPic * GetSubMapWidth();
		MapRect.top = Index / m_pMapData->totalSubFrame / RowPerPic * GetSubMapHeight();
		MapRect.right = MapRect.left + GetSubMapWidth();
		MapRect.bottom = MapRect.top + GetSubMapHeight();

		RECT ResultRect;
		if (!QGLMath::CalculateOverlappedRect(MapRect, m_EffectiveRect, ResultRect))
		{
			return NULL;
		}*/
	}


	//wait the resources finish loading
	UInt32 PreTime = ::timeGetTime();
	while (m_pMapData->m_pTextureBGLst[Index].pDDSPicData == NULL)
	{
		UInt32 CurrTime = ::timeGetTime();
		if (CurrTime - PreTime >= 1000)
		{
			CResourcesManager::GetInstance().MapPartRequest(m_pMapData, Index, gGlobal.pD3DDevice);
		}
		Sleep(1);
	}
	

	if (m_pMapData->m_pTextureBGLst[Index].pDDSPicData == NULL)
	{
		int i = 0;
		i ++;
	}
	return m_pMapData->m_pTextureBGLst[Index].pDDSPicData;

}


Byte * CMapObjectView::GetNightTextureByIndex(Int Index)
{
	if (m_pMapData == NULL || m_pMapData->m_pTextureNightLst == NULL)
		return NULL;

	if (Index >= (Int)m_pMapData->TotalPic * m_pMapData->totalNightFrame)
		return NULL;

	//find whether the index is inside the effect area
	if (m_pMapData->m_pTextureNightLst[Index].pDDSPicData == NULL)
	{
		if (m_pMapData->totalNightFrame == 0 ||
			GetSubMapWidth() == 0)
			return NULL;

		Int RowPerPic = (Int)ceil(((Real32)GetMapWidth() / (Real32)GetSubMapWidth()));

		RECT MapRect;
		MapRect.left = Index / m_pMapData->totalNightFrame % RowPerPic * GetSubMapWidth();
		MapRect.top = Index / m_pMapData->totalNightFrame / RowPerPic * GetSubMapHeight();
		MapRect.right = MapRect.left + GetSubMapWidth();
		MapRect.bottom = MapRect.top + GetSubMapHeight();

		RECT ResultRect;
		if (!QGLMath::CalculateOverlappedRect(MapRect, m_EffectiveRect, ResultRect))
		{
			return NULL;
		}
	}


	//wait the resources finish loading
	UInt32 PreTime = ::timeGetTime();
	while (m_pMapData->m_pTextureNightLst[Index].pDDSPicData == NULL)
	{
		UInt32 CurrTime = ::timeGetTime();
		if (CurrTime - PreTime >= 1000)
		{
			CResourcesManager::GetInstance().MapPartRequest(m_pMapData, Index * m_pMapData->totalSubFrame, gGlobal.pD3DDevice);
		}
		Sleep(1);
	}
	

	return m_pMapData->m_pTextureNightLst[Index].pDDSPicData;

}

BOOL CMapObjectView::IsBlockInAlpha(BlockPos pos)
{
	if (!m_pMapData || m_pMapData->State != DATA_FINISH_LOADING)
		return FALSE;
	UInt32 BlockIndex = pos.PosY * m_pMapData->MaxBlockX + pos.PosX;
	if (BlockIndex < m_pMapData->BlockPosDataLst.size())
		return (BOOL)((m_pMapData->BlockPosDataLst[BlockIndex].BlockData & 0x02) > 0);
	return FALSE;
}

BOOL CMapObjectView::IsBlockInObject(BlockPos pos)
{
	if (!m_pMapData || m_pMapData->State != DATA_FINISH_LOADING)
		return FALSE;
	UInt32 BlockIndex = pos.PosY * m_pMapData->MaxBlockX + pos.PosX;
	StlMultiMap<UInt32,CAnimatedEntity *>::iterator Itr = m_BkObjectIndexLst.find(BlockIndex);
	return Itr != m_BkObjectIndexLst.end();
	
}

VOID CMapObjectView::AddNPCEntityToMap(CEntityBase * pEntBase)
{
	if (pEntBase == NULL)
		return;

	StlMap<UInt, CEntityBase * >::iterator MEItr = m_MapEntLst.find((UInt)pEntBase);
	if( MEItr == m_MapEntLst.end())
	{
		m_MapEntLst.insert(std::make_pair((UInt)pEntBase, pEntBase));

		//add shadow to NPC
		CAnimatedEntity * pShadow = SafeCreateObject(CAnimatedEntity);
		pShadow->LoadEntityFile(gConfigure.strShadowName.c_str(), gGlobal.pD3DDevice);
		pShadow->SetEntityType(ENTITY_TYPE_PARTICLE_WITHEFFECT);
		pShadow->SetExtraZBuffer(-1);
		pShadow->SetEntParent(pEntBase);

		m_MapEntLst.insert(std::make_pair((UInt)pShadow, pShadow));
	}
	
}
VOID CMapObjectView::AddEntityToMap(CEntityBase * pEntBase)
{
	StlMap<UInt, CEntityBase * >::iterator MEItr = m_MapEntLst.find((UInt)pEntBase);
	if( MEItr == m_MapEntLst.end())
	{
		m_MapEntLst.insert(std::make_pair((UInt)pEntBase, pEntBase));
	}
}

VOID CMapObjectView::RemoveEntityFromMap(CEntityBase * pEntBase)
{
	StlMap<UInt, CEntityBase * >::iterator MEItr = m_MapEntLst.find((UInt)pEntBase);
	if( MEItr != m_MapEntLst.end())
	{
		SafeDeleteObject(MEItr->second);
		m_MapEntLst.erase(MEItr);
	}
}

VOID CMapObjectView::RemoveNPCFromMap(CMapActorControl * pCtrl)
{
	StlMap<UInt, CMapNPCControl * >::iterator Itr = m_MapNPCCtrlLst.find((UInt)pCtrl);
	if( Itr != m_MapNPCCtrlLst.end())
	{
		SafeDeleteObject(Itr->second);
		m_MapNPCCtrlLst.erase(Itr);
	}
}
BOOL CMapObjectView::IsBlockInShadow(BlockPos pos)
{
	if (!m_pMapData || m_pMapData->State != DATA_FINISH_LOADING)
		return FALSE;
	UInt32 BlockIndex = pos.PosY * m_pMapData->MaxBlockX + pos.PosX;
	if (BlockIndex < m_pMapData->BlockPosDataLst.size())
		return (BOOL)((m_pMapData->BlockPosDataLst[BlockIndex].BlockData & 0x04) > 0);
	return FALSE;
}

BOOL CMapObjectView::IsBlockInSea(BlockPos pos)
{
	if (!m_pMapData || m_pMapData->State != DATA_FINISH_LOADING)
		return FALSE;
	UInt32 BlockIndex = pos.PosY * m_pMapData->MaxBlockX + pos.PosX;
	if (BlockIndex < m_pMapData->BlockPosDataLst.size())
		m_BlockSeaDepth = (m_pMapData->BlockPosDataLst[BlockIndex].BlockData) >> 8;
	return m_BlockSeaDepth > 0;
}

VOID CMapObjectView::SetEffectiveRect(RECT GivenRect)
{
	m_EffectiveRect = GivenRect;
}

void CMapObjectView::FreeAllLoadedModelName()
{
	for (Int i = 0; i < m_LoadedModel.size(); i++)
	{
		CResourcesManager::GetInstance().ReleaseEntityData(m_LoadedModel[i]);
		m_LoadedModel[i].Empty();
	}
	m_LoadedModel.clear();
}
void CMapObjectView::AddLoadedModelName(String filename)
{
	m_LoadedModel.push_back(filename);
}

VOID CMapObjectView::UpDate(UInt32 CurrentTime)
{
	if (!m_pMapData || m_pMapData->State <= DATA_FINISH_LOADING_FIRST_BGMAP)
		return;

	m_pMapData->PreInUseTime = CurrentTime;
	if (gGlobal.g_pSystemSetting->IsMapAnimeModeEnable() && CurrentTime - m_MapCountTime > MapFramePerSec)
	{
		m_MapPreFrameIndex = m_MapFrameIndex;
		m_MapFrameIndex ++;

		if (m_MapFrameIndex >= m_pMapData->totalSubFrame)
			m_MapFrameIndex = 0;

		m_Updated = m_MapPreFrameIndex != m_MapFrameIndex;
		
		m_MapCountTime = CurrentTime;
		//m_Updated = FALSE;
	}
	
	UpDataMapObject(CurrentTime);

}
VOID CMapObjectView::UpDataMapObject(UInt32 CurrentTime)
{
	for (Int i = 0; i < (Int)m_MapObjectLst.size(); i++)
	{
		m_MapObjectLst[i]->UpDate(CurrentTime);
	}

	StlMap<UInt, CEntityBase * >::iterator MEItr = m_MapEntLst.begin();
	while( MEItr != m_MapEntLst.end())
	{
		MEItr->second->UpDate(CurrentTime);
		MEItr++;
	}

	StlMap<UInt, CMapNPCControl * >::iterator MapNPCItr = m_MapNPCCtrlLst.begin();
	while( MapNPCItr != m_MapNPCCtrlLst.end())
	{
		MapNPCItr->second->UpDateView(CurrentTime);
		
		MapNPCItr++;
	}
}

VOID CMapObjectView::AddAllObjectBlockToMap()
{
	for (Int i = 0; i < (Int)m_MapObjectLst.size(); i++)
	{
		CAnimatedEntity * pObject = m_MapObjectLst[i];
		
		if (pObject == NULL)
			continue;

		Int totalXBlock = pObject->m_EntOrgWidth/ BlockSizeX;
		Int totalYBlock = pObject->m_EntOrgHeight/ (BlockSizeY / 2);
		UInt32 ObjectBlockIndex = pObject->GetCharPos().PosY  * m_pMapData->MaxBlockX + pObject->GetCharPos().PosX;
		for (Int i = 0; i < totalXBlock; i++)
		{
			for (Int j = 0; j < totalYBlock; j++)
			{
				UInt32 offsetIndex = ObjectBlockIndex + j * m_pMapData->MaxBlockX + i;
				//if (!(m_pMapData->BlockPosDataLst[offsetIndex].BlockData & 0x08))
				//	m_pMapData->BlockPosDataLst[offsetIndex].BlockData += 8 ;
				//BlockPosDataLst[offsetIndex].isEntityBlock = TRUE;
				m_BkObjectIndexLst.insert(std::make_pair(offsetIndex, pObject));
			}
		}
	}
}

VOID CMapObjectView::AddObjectBlockToMap(CAnimatedEntity * pObject)
{
	if (m_pMapData == NULL)
		return;

	Int totalXBlock = pObject->m_EntOrgWidth/ BlockSizeX;
	Int totalYBlock = pObject->m_EntOrgHeight/ (BlockSizeY / 2);
	UInt32 ObjectBlockIndex = pObject->GetCharPos().PosY  * m_pMapData->MaxBlockX + pObject->GetCharPos().PosX;
	for (Int i = 0; i < totalXBlock; i++)
	{
		for (Int j = 0; j < totalYBlock; j++)
		{
			UInt32 offsetIndex = ObjectBlockIndex + j * m_pMapData->MaxBlockX + i;
			m_BkObjectIndexLst.insert(std::make_pair(offsetIndex, pObject));
		}
	}
}

VOID CMapObjectView::AddMapObject(LPCTSTR filename, LPDIRECT3DDEVICE9	pD3DDevice, Int PosX, Int PosY)
{
	CAnimatedEntity * Object = SafeCreateObject(CAnimatedEntity);
	Object->SetEntityType(ENTITY_TYPE_MAP_OBJECT);
	Object->LoadEntityFile(filename, pD3DDevice);
	Object->SetCharPos(PosX, PosY);
	//position adjust so that, the left upper corner of the object will be at the center of the entity's block
	Object->SetEntOffsetX(Object->m_EntOrgWidth/2 - BlockSizeX / 2);
	
	if (Object->m_EntOrgHeight > CharPicOffsetY)
		Object->SetEntOffsetY((Object->m_EntOrgHeight - CharPicOffsetY ) - BlockSizeY / 2);
	else
		Object->SetEntOffsetY((Object->m_EntOrgHeight ) - BlockSizeY / 2);
	
	Object->SetExtraZBuffer(ObjExtraZ);
	AddObjectBlockToMap(Object);
	m_MapObjectLst.push_back(Object);
}

VOID CMapObjectView::AddMapObject(CAnimatedEntity * Object)
{
	if (Object == NULL)
		return;

	m_MapObjectLst.push_back(Object);
	//position adjust so that, the left upper corner of the object will be at the center of the entity's block
	Object->SetEntOffsetX(Object->m_EntOrgWidth/2 - BlockSizeX / 2);
	if (Object->m_EntOrgHeight > CharPicOffsetY)
		Object->SetEntOffsetY((Object->m_EntOrgHeight - CharPicOffsetY ) - BlockSizeY / 2);
	else
		Object->SetEntOffsetY((Object->m_EntOrgHeight / 2 ) - BlockSizeY / 2);
	Object->SetEntityType(ENTITY_TYPE_MAP_OBJECT);

	AddObjectBlockToMap(Object);
}

BOOL CMapObjectView::IsBlcok(BlockPos pos)
{
	UInt32 posIndex = pos.PosY * m_pMapData->MaxBlockX + pos.PosX;
	BOOL Blocked = TRUE;

	if (posIndex >= m_pMapData->BlockPosDataLst.size())
		Blocked = FALSE;
	else
		Blocked = (BOOL)((m_pMapData->BlockPosDataLst[posIndex].BlockData & 0x01) > 0);

	Int x = pos.PosX;
	Int y = pos.PosY;
	if (!Blocked)
	{
		if (IsBlockInObject(pos))
		{
			for (Int k = 0; k < (Int)m_MapObjectLst.size(); k++)
			{
				BlockPos ObjectPos = m_MapObjectLst[k]->GetCharPos();
				x = x - ObjectPos.PosX;
				y = y - ObjectPos.PosY;
				if (x >= 0 && y >= 0)
				{
					StlMultiMap<UInt32, RenderLstData>::iterator itrRenderLst;

					itrRenderLst = m_MapObjectLst[k]->m_RenderLst.begin();

					Int i = 0;
					while (itrRenderLst != m_MapObjectLst[k]->m_RenderLst.end())
					{
						EntityPicInfo * EntPic = m_MapObjectLst[k]->GetEntPic((UInt)i);

						if (EntPic == NULL || EntPic->State == DATA_LOADING)
							continue;

						if (itrRenderLst->second.BlockBkIndex <= (Int)EntPic->m_BlockingLst.size())
						{
							for (Int j = 0; j < (Int)EntPic->
								m_BlockingLst[itrRenderLst->second.BlockBkIndex - 1].size(); j++)
							{
								if (x == EntPic->m_BlockingLst[itrRenderLst->
									second.BlockBkIndex ][j].PosX && 
									y == EntPic->m_BlockingLst[itrRenderLst
									->second.BlockBkIndex ][j].PosY)
									return TRUE;
							}
						}

						i++;
						itrRenderLst++;
					}
				}
			}
		}
	}
	return Blocked;
}


VOID CMapObjectView::InsertNPCToMap(CMapNPCControl * NPCCtrl)
{
	StlMap<UInt, CMapNPCControl * >::iterator Itr = m_MapNPCCtrlLst.find((UInt)NPCCtrl);

	if (Itr == m_MapNPCCtrlLst.end())
	{
		m_MapNPCCtrlLst.insert(std::make_pair((UInt)NPCCtrl, NPCCtrl));
	}
}

VOID CMapObjectView::RemoveNPCToFromMapByNetObj(CMapNPC * NPC)
{
	StlMap<UInt, CMapNPCControl * >::iterator Itr = m_MapNPCCtrlLst.begin();
	while (Itr != m_MapNPCCtrlLst.end())
	{
		if (Itr->second->GetNetObject() == NPC)
		{
			SafeDeleteObject(Itr->second);
			m_MapNPCCtrlLst.erase(Itr);
			break;
		}
		Itr ++;
	}
}