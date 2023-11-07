//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "ImageEntity.h"
//-- Library
#include "Global.h"
#include "Math.h"
#include "Render.h"


CImageEntity::CImageEntity()
{
	m_pImage = NULL;
	//get the cache memory
	m_CurrTexture = CResourcesManager::GetInstance().GetEntPicCacheTexture();
}

CImageEntity::~CImageEntity()
{
	SafeRelease(m_pImage);
}

Int CImageEntity::GetTotalRenderPart()
{
	//image only have one content
	return 1;
}
BOOL CImageEntity::GetEntityPart(size_t index, Int ImageType, RECT * LockRegion)
{
	if (m_pImage == NULL)
		return FALSE;

	RECT EntLockRegion = m_EntityTotalRect;

	if (LockRegion)
	{
		EntLockRegion = *LockRegion;
		EntLockRegion.left -= GetEntLeftUpPos().PixelPosX;
		EntLockRegion.right -= GetEntLeftUpPos().PixelPosX;
		EntLockRegion.top -= GetEntLeftUpPos().PixelPosY;
		EntLockRegion.bottom -= GetEntLeftUpPos().PixelPosY;
	}

	if (EntLockRegion.top < 0)
	{
		EntLockRegion.bottom -= EntLockRegion.top;
		EntLockRegion.top = 0;
	}

	RECT ResultRect;

	if (QGLMath::CalculateOverlappedRect(EntLockRegion, m_EntityTotalRect, ResultRect))
	{
		D3DSURFACE_DESC  pDesc; 
		D3DLOCKED_RECT d3dlt;
		if (CRender::LockSystemTexture( pDesc, d3dlt, 
			&ResultRect, m_pImage))
		{
			BYTE * pSrcData = (BYTE *)d3dlt.pBits;
			BYTE * pDesData = m_CurrTexture + (ResultRect.top * MaxEntSizeX  + ResultRect.left) * 4;
			UInt CopyLen = (ResultRect.right - ResultRect.left) * 4;
			for (Int y = 0; y < ResultRect.bottom - ResultRect.top; y ++)
			{
				memcpy(pDesData, pSrcData, CopyLen);
				pDesData += MaxEntSizeX * 4;
				pSrcData += d3dlt.Pitch;
			}
			m_pImage->UnlockRect(0);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CImageEntity::GetHaveEffect()
{
	return FALSE;
}


VOID CImageEntity::SetImage(LPDIRECT3DTEXTURE9 pImage)
{
	if (pImage)
	{
		SafeRelease(m_pImage);
		D3DSURFACE_DESC  pDesc; 
		D3DLOCKED_RECT d3dlt;
		if (CRender::LockSystemTexture( pDesc, d3dlt, 
			NULL, pImage))
		{
			D3DXCreateTexture(gGlobal.pD3DDevice, pDesc.Width, pDesc.Height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &m_pImage);
			D3DSURFACE_DESC  pDescDes; 
			D3DLOCKED_RECT d3dltDes;
			if (CRender::LockSystemTexture( pDescDes, d3dltDes, 
				NULL, m_pImage))
			{
				//D3DXCreateTextureFromFileInMemoryEx(gGlobal.pD3DDevice, d3dlt.pBits, d3dlt.Pitch * pDesc.Height, 
				//	pDesc.Width, pDesc.Height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH , D3DX_FILTER_NONE, D3DX_DEFAULT
				//, 0, NULL, NULL, &m_pImage);
				BYTE * pDesData = (BYTE *)d3dltDes.pBits;
				BYTE * pSrcData = (BYTE *)d3dlt.pBits;
				for (Int y = 0; y < (Int)pDesc.Height; y++)
				{
					memcpy(pDesData, pSrcData, d3dlt.Pitch);
					pDesData += d3dltDes.Pitch;
					pSrcData += d3dlt.Pitch;
				}
				m_Updated = TRUE;
				SetRect(&m_CurrRect, 0, 0, pDesc.Width, pDesc.Height);
				SetRect(&m_EntityTotalRect, 0, 0, pDesc.Width, pDesc.Height);
				m_EntOrgHeight = pDesc.Height;
				m_EntOrgWidth = pDesc.Width;
				m_pImage->UnlockRect(0);
			}	
			pImage->UnlockRect(0);
		}
	}
}
VOID CImageEntity::SetImage(LPCTSTR filename)
{
	SafeRelease(m_pImage);
	if( g_GdfResource || g_GdfResourceEx )
	{
		String imagefile = filename;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(imagefile, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(imagefile, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			D3DXCreateTextureFromFileInMemoryEx (gGlobal.pD3DDevice, buf, fileSize, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 
				D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH , D3DX_FILTER_NONE, D3DX_DEFAULT,
				0, 0, NULL, &m_pImage);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		D3DXCreateTextureFromFileEx(gGlobal.pD3DDevice, filename, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 
			D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH , D3DX_FILTER_NONE, D3DX_DEFAULT
				, 0, 0, NULL, &m_pImage);
	}
	if (m_pImage)
	{
		D3DSURFACE_DESC  pDesc; 
		m_pImage->GetLevelDesc(0, &pDesc);
		m_Updated = TRUE;
		SetRect(&m_CurrRect, 0, 0, pDesc.Width, pDesc.Height);
		SetRect(&m_EntityTotalRect, 0, 0, pDesc.Width, pDesc.Height);
		m_EntOrgHeight = pDesc.Height;
		m_EntOrgWidth = pDesc.Width;
	}
}

VOID CImageEntity::CreateImage(Int Width, Int Height, D3DPOOL  Pool, D3DFORMAT Format)
{
	D3DXCreateTexture(gGlobal.pD3DDevice, Width, Height, 0, 0, Format, Pool, &m_pImage);
}
