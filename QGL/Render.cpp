//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "Render.h"
//-- Library
#include "Math.h"
#include "QGLApplication.h"
#include "EntityBase.h"
#include "AnimatedEntity.h"
#include "Global.h"
#include "Configure.h"
#include <WindowsX.h>
#include <dxerr.h>
#pragma comment(lib, "dxerr.lib")

typedef Byte Byte256X256[256][256];

CONST Byte ObjectAlpha = 125;
CONST Byte SeaAlpha = 120;
CONST UInt16 SeaDepthCri = 50;

CONST Byte PixelSizeOfBackBuffer = 4;

bool ChangeFullScreen(HWND hWnd)
{
	DEVMODE   lpDevMode;   
	bool		ReturnFlag = true;

	if(gGlobal.g_OldScreenX != GetSystemMetrics(SM_CXSCREEN) || gGlobal.g_OldScreenY != GetSystemMetrics(SM_CYSCREEN))
	{
		lpDevMode.dmPelsWidth=800;   
		lpDevMode.dmPelsHeight=600;   
		lpDevMode.dmSize=sizeof(DEVMODE);   
		lpDevMode.dmFields   =DM_PELSWIDTH|DM_PELSHEIGHT;   
		LONG   result;   
		
		result=ChangeDisplaySettings(&lpDevMode,0);  
	}

	SetWindowPos( hWnd, HWND_TOPMOST,
		0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
		SWP_SHOWWINDOW );


	::SetWindowLong(hWnd, GWL_USERDATA, TRUE);

	RECT rect;
	SetRect(&rect, 0, 0, 800, 600);
	ClipCursor(&rect);

	return ReturnFlag;
}


bool ChangeOldScreen(HWND hWnd, RECT *rect)
{
	DEVMODE   lpDevMode;   
	bool		ReturnFlag = true;

	SetWindowPos( hWnd, HWND_NOTOPMOST,
			rect->left, rect->top,
			( rect->right - rect->left ),
			( rect->bottom - rect->top ),
			SWP_SHOWWINDOW );

	if(gGlobal.g_OldScreenX != GetSystemMetrics(SM_CXSCREEN) || gGlobal.g_OldScreenY != GetSystemMetrics(SM_CYSCREEN))
	{
		lpDevMode.dmPelsWidth = gGlobal.g_OldScreenX;   
		lpDevMode.dmPelsHeight= gGlobal.g_OldScreenY;   
		lpDevMode.dmSize=sizeof(DEVMODE);   
		lpDevMode.dmFields   =DM_PELSWIDTH|DM_PELSHEIGHT;   
		LONG   result;   
		
		result=ChangeDisplaySettings(&lpDevMode,0);

		ReturnFlag = true;
	}
	::SetWindowLong(hWnd, GWL_USERDATA, FALSE);
	ClipCursor(NULL);

	return ReturnFlag;
}



struct MultiTexVertex
{
	MultiTexVertex(float x, float y, float z,
		float u0, float v0,
		float u1, float v1,
		float u2, float v2)
	{
		 _x =  x;  _y =  y; _z = z;
		_u0 = u0; _v0 = v0; 
		_u1 = u1; _v1 = v1;
		_u2 = u2, _v2 = v2;
	}

	float _x, _y, _z;
	float _u0, _v0;
	float _u1, _v1;
	float _u2, _v2;

	static const DWORD FVF;
};
const DWORD MultiTexVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX3; 

CRender::CRender(HINSTANCE hInstance, HWND	CurrWin, Int GivenWinWidth, Int GivenWinHeight, BOOL bFullScreen, BOOL CreateBg)
{
	hDC				= NULL;		// Private GDI Device Context
	hWnd			= CurrWin;		// Holds Our Window Handle
	hD3DInstance	= hInstance;					// Holds The Instance Of The Application
	pD3D			= NULL;		// DirectX 3D Version 9
	pD3DDevice		= NULL;		// DirectX 3D Rendering Device
	m_pSprite = NULL;
	m_CreateBgTexture = CreateBg;

	if(CheckMyProcess(hWnd) > 1)
		bFullScreen = false;
	

//	m_CombinedDefaultTexture = NULL;

	RECT lpRect;
	GetWindowRect(hWnd,&lpRect);

	m_CombinedEntityTexture.Texture = NULL;

	//Create Our DirectX 3D Window

	m_CurrWinWidth = GivenWinWidth;
	m_CurrWinHeight = GivenWinHeight;

	m_rcWindowBounds.left = 100;
	m_rcWindowBounds.right = m_rcWindowBounds.left + GivenWinWidth;
	m_rcWindowBounds.top = 100;
	m_rcWindowBounds.bottom = m_rcWindowBounds.top + GivenWinHeight;

	gGlobal.g_OldScreenX = GetSystemMetrics(SM_CXSCREEN);
    gGlobal.g_OldScreenY = GetSystemMetrics(SM_CYSCREEN);



	CreateD3DWindow(bFullScreen);

	if(bFullScreen)
	{
		::SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
		::SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW);

		
		if(ChangeFullScreen(hWnd))
		{
			HRESULT result = pD3DDevice->TestCooperativeLevel();

			if ( FAILED( result ) )
			{
				if (result == D3DERR_DEVICENOTRESET)
					HRESULT hr = pD3DDevice->Reset(&m_PresentPara );

				result = pD3DDevice->TestCooperativeLevel();
				if ( FAILED( result ) )
				{
					if (result == D3DERR_DEVICENOTRESET)
					{
						MessageBox(NULL,_T("result == D3DERR_DEVICENOTRESET"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
					}
				}

			}
		}
	}

	Init();

	m_OrgWinRect.left = 0;
	m_OrgWinRect.top = 0;
	m_OrgWinRect.right = m_CurrWinWidth;
	m_OrgWinRect.bottom = m_CurrWinHeight;

	m_CombinedEntityTexture.m_DirRectMan.AddDirtyRectToMan(m_OrgWinRect);

	
	
	

	m_CursorX = m_CursorY = 0;

	m_CurPosEntity = NULL;
	
	gConfigure.RenderNightByHardware = FALSE;
	m_IsSubWindow = FALSE;

}


CRender::~CRender()
{
	m_CombinedEntityTexture.m_PreDirtyRegionLst.clear();
	m_CombinedEntityTexture.m_NextDirtyRegionLst.clear();
	KillD3DWindow();
	SafeRelease(m_CombinedEntityTexture.Texture);
	SafeRelease(m_CombinedEntityTexture.Surface);
	SafeDeallocate(m_CombinedEntityTexture.pData);


	if(gGlobal.g_OldScreenX != GetSystemMetrics(SM_CXSCREEN))
	{
		DEVMODE   lpDevMode;   

		lpDevMode.dmPelsWidth = gGlobal.g_OldScreenX;   
		lpDevMode.dmPelsHeight= gGlobal.g_OldScreenY;   
		lpDevMode.dmSize=sizeof(DEVMODE);   
		lpDevMode.dmFields   =DM_PELSWIDTH|DM_PELSHEIGHT;   
		LONG   result;   
		
		result=ChangeDisplaySettings(&lpDevMode,0);  
	
		::SetWindowLong(hWnd, GWL_USERDATA, FALSE);
		ClipCursor(NULL);
	}
}

VOID CRender::ReSizeD3DScene(Int width, Int height)				// Resize And Initialize The D3D Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	D3DXMATRIXA16 matProjection;						// Create A Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	D3DXMatrixPerspectiveFovLH(&matProjection, 45.0f, (Real32)width/(Real32)height, 0.1f, 100.0f );

	pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProjection );
	D3DXMatrixIdentity(&matProjection);					// Reset The Projection Matrix
}


Int CRender::InitD3D()										   
{
	pD3DDevice->SetRenderState(D3DRS_ZENABLE,  FALSE ); // Enable Z-Buffer (Depth Buffer)
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // Disable Backface Culling
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE); // Disable Light
	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE , FALSE);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA   );
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA   );
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	return TRUE;									   // Initialization Went OK
}

BOOL CRender::GetAlphaMask( BlockPos m_CharPos, 
						Byte * pResultMask, CMapObjectView * pMap, RECT WorldMapRect, UInt32 SrcMaxWidth, BOOL Flip)
{
	if (!pMap || !pMap->GetMapData() || pMap->GetMapFrameIndex() >= (Int)pMap->GetMapData()->BkAlphaIndexLstVec.size())
		return FALSE;
	if (pMap->GetMapData()->State != DATA_FINISH_LOADING)
		return FALSE;

	BOOL First = TRUE;
	if (pMap->IsBlockInAlpha(m_CharPos))
	{
		UInt32 BlockIndex = m_CharPos.PosY * pMap->GetMapData()->MaxBlockX + m_CharPos.PosX;
		Byte * pAlphaIndexData = pMap->GetMapData()->BkAlphaIndexLstVec[pMap->GetMapFrameIndex()][BlockIndex];
		Byte Count = *pAlphaIndexData; 

		for (Int i = 0; i < (Int)Count ; i ++)
		{
			UInt16 Index = *(UInt16 *)(pAlphaIndexData + sizeof(Byte) + i * sizeof(UInt16));

			if (Index >= pMap->GetMapData()->AlphaMaskLst.size())
				continue;

			MaskEffect * TempBound = &pMap->GetMapData()->AlphaMaskLst[Index];

			if (TempBound->pData == NULL)
				continue;

			RECT ResultRect;
			if (QGLMath::CalculateOverlappedRect(TempBound->Bound, WorldMapRect, ResultRect))
			{
				UInt16 SrcWidth = (UInt16)ResultRect.right - (UInt16)ResultRect.left;
				UInt16 SrcHeight = (UInt16)ResultRect.bottom - (UInt16)ResultRect.top;

				UInt32 SrcStartIndex = (ResultRect.top - TempBound->Bound.top) * (TempBound->Bound.right - 
					TempBound->Bound.left + 1)  + ResultRect.left - TempBound->Bound.left;

				UInt32 DesStartIndex = (ResultRect.top - WorldMapRect.top) * SrcMaxWidth
					+ ResultRect.left - WorldMapRect.left;

				First = FALSE;	
				if (Flip)
				{
					
					DesStartIndex = (ResultRect.top - WorldMapRect.top) * SrcMaxWidth
						+ (WorldMapRect.right - WorldMapRect.left) - (ResultRect.right - ResultRect.left) - 
						(ResultRect.left - WorldMapRect.left);

					Byte * pTempMask = pResultMask + DesStartIndex * 4;
					Byte * pSrcData = TempBound->pData + SrcStartIndex;

					Int UpdateDesLen = (SrcWidth * 4);
					Int DesMovePixel = 4;

					pTempMask += UpdateDesLen - 4;

					//UpdateDesLen *= -1;
					DesMovePixel *= -1;
					
					UpdateDesLen = SrcMaxWidth * 4 + UpdateDesLen;

					for (Int y = 0; y < SrcHeight; y++)
					{
						//Byte * pSubTempMask = pTempMask;
						Byte * pSubSrcData = pSrcData;
						for (Int x = 0; x < SrcWidth; x++)
						{
							pTempMask[3] = gGlobal.SubtractiveTable[pTempMask[3]][*pSubSrcData];

							//pSubTempMask += 4;
							pTempMask += DesMovePixel;
							pSubSrcData ++;
						}
						//pTempMask += SrcMaxWidth * 4;
						pTempMask += UpdateDesLen;
						pSrcData += TempBound->Bound.right - TempBound->Bound.left + 1;
					}
				}
				else
				{
					Byte * pTempMask = pResultMask + DesStartIndex * 4;
					Byte * pSrcData = TempBound->pData + SrcStartIndex;
					
					for (Int y = 0; y < SrcHeight; y++)
					{
						Byte * pSubTempMask = pTempMask;
						Byte * pSubSrcData = pSrcData;
						for (Int x = 0; x < SrcWidth; x++)
						{
							pSubTempMask[3] = gGlobal.SubtractiveTable[pSubTempMask[3]][*pSubSrcData];
							

							pSubTempMask += 4;
							pSubSrcData ++;
						}
						pTempMask += SrcMaxWidth * 4;
						pSrcData += TempBound->Bound.right - TempBound->Bound.left + 1;
					}
				}	
			}

		}
	}

	if (pMap->IsBlockInObject(m_CharPos))
	{
		SetObjectBlockAlpha( m_CharPos, pResultMask,  pMap, WorldMapRect, SrcMaxWidth, Flip);
	}
	return !First;
}


VOID CRender::SetShadow( BlockPos m_CharPos, 
						 UInt16 DesMaxHeight, UInt16 DesMaxWidth, 
						 Byte * pResultMask, BOOL flip, CMapObjectView * pMap, RECT WorldMapRect)
{
	if (!pMap || !pMap->GetMapData() || pMap->GetMapFrameIndex() >= (Int)pMap->GetMapData()->BkShadowIndexLstVec.size())
		return;

	if (pMap->GetMapData()->State != DATA_FINISH_LOADING)
		return;

	UInt32 BlockIndex = m_CharPos.PosY * pMap->GetMapData()->MaxBlockX + m_CharPos.PosX;
	Byte * pShadowIndexData = pMap->GetMapData()->BkShadowIndexLstVec[pMap->GetMapFrameIndex()][BlockIndex];
	Byte Count = *pShadowIndexData;
	for (Int i = 0; i < (Int)Count; i ++)
	{
		UInt16 Index = *(UInt16 *)(pShadowIndexData + sizeof(Byte) + i * sizeof(UInt16));

		if (Index >= pMap->GetMapData()->ShadowMaskLst.size())
			continue;

		MaskEffect * pTempMaskEffect = &pMap->GetMapData()->ShadowMaskLst[Index];

		if (pTempMaskEffect->pData == NULL)
			continue;
		RECT ResultRect;
		if (QGLMath::CalculateOverlappedRect(pTempMaskEffect->Bound, WorldMapRect, ResultRect))
		{
			UInt16 SrcWidth = (UInt16)ResultRect.right - (UInt16)ResultRect.left;
			UInt16 SrcHeight = (UInt16)ResultRect.bottom - (UInt16)ResultRect.top;

			UInt32 SrcStartIndex = (ResultRect.top - pTempMaskEffect->Bound.top) * (pTempMaskEffect->Bound.right - 
				pTempMaskEffect->Bound.left + 1)  + ResultRect.left - pTempMaskEffect->Bound.left;

			UInt32 DesStartIndex = (ResultRect.top - WorldMapRect.top) * DesMaxWidth
				+ ResultRect.left - WorldMapRect.left;

			if (flip)
				DesStartIndex = (ResultRect.top - WorldMapRect.top) * DesMaxWidth
				+ (WorldMapRect.right - WorldMapRect.left) - (ResultRect.right - ResultRect.left) - 
				(ResultRect.left - WorldMapRect.left);

			BlitSubtractivePic(pTempMaskEffect->pData, SrcStartIndex, SrcWidth, 
				SrcHeight, (UInt16)(pTempMaskEffect->Bound.right - pTempMaskEffect->Bound.left + 1),
				pResultMask, DesStartIndex, DesMaxWidth, flip, FALSE, sizeof(Color32));
		}
	}
}
VOID CRender::SetObjectBlockShadow( BlockPos m_CharPos, UInt16 DesMaxHeight, UInt16 DesMaxWidth, Byte * pResultMask, BOOL flip, 
								   RECT WorldLockRect, CMapObjectView * pMap)
{
	if (!pMap || !pMap->GetMapData())
		return;

	UInt32 BlockIndex = m_CharPos.PosY * pMap->GetMapData()->MaxBlockX + m_CharPos.PosX;
	StlMultiMap<UInt32,CAnimatedEntity *>::iterator itrAL = pMap->m_BkObjectIndexLst.find(BlockIndex);
	
	Int count = (Int)pMap->m_BkObjectIndexLst.count(BlockIndex);

	if (itrAL != pMap->m_BkObjectIndexLst.end())
	{
	for (Int i = 0; i < count; i++)
	{
		StlMultiMap<UInt32, RenderLstData>::iterator itrRenderLst;
		CAnimatedEntity * pMapObject = itrAL->second;
		itrRenderLst = pMapObject->m_RenderLst.begin();
		Int index = 0;
		BlockPos ObjectPos = pMapObject->GetCharPos();
		while (itrRenderLst !=pMapObject->m_RenderLst.end())
		{
			if (pMapObject->GetEntityPart(index, ShadowPic, &WorldLockRect))
			{
				StlVector<BlockPos> BkList = pMapObject->GetCurrBlockPositionLst
					(itrRenderLst->second.ShadowBkIndex);

				//find whether the player is inside the blocking list
				for (Int i = 0; i < (Int)BkList.size(); i++)
				{
					BlockPos BkCurrPos = BkList[i];
					if (ObjectPos.PosX + BkCurrPos.PosX == m_CharPos.PosX && 
						ObjectPos.PosY + BkCurrPos.PosY == m_CharPos.PosY)
					{
						RECT CurrRect = pMapObject->GetCurrRect();
						RECT NextDirtyRegion;
						int offsetX = ObjectPos.PosX * BlockSizeX + CurrRect.left;
						if (ObjectPos.PosY % 2 == 1)
							offsetX += BlockSizeX / 2;
						int offsetY = ObjectPos.PosY * (BlockSizeY / 2) + CurrRect.top;

						NextDirtyRegion.left = offsetX ;
						NextDirtyRegion.right = offsetX + CurrRect.right - CurrRect.left;
						NextDirtyRegion.top = offsetY ;
						NextDirtyRegion.bottom = offsetY + CurrRect.bottom - CurrRect.top;

						RECT ResultRect;
						if (QGLMath::CalculateOverlappedRect(NextDirtyRegion, WorldLockRect, ResultRect))
						{
							

							//D3DLOCKED_RECT d3dlt;
							//D3DSURFACE_DESC  pDescMapOb;

							if (!QGLMath::InvalidLock(&CurrRect, MaxEntSizeX, MaxEntSizeY))
							{
								UInt16 SrcWidth = (UInt16)ResultRect.right - (UInt16)ResultRect.left;
								UInt16 SrcHeight = (UInt16)ResultRect.bottom - (UInt16)ResultRect.top;

								UInt32 SrcStartIndex = (ResultRect.top - NextDirtyRegion.top)  * pMapObject->GetPicWidth() 
									+ ResultRect.left - NextDirtyRegion.left;

								UInt32 DesStartIndex = (ResultRect.top - WorldLockRect.top)  * DesMaxWidth
									+ ResultRect.left - WorldLockRect.left;

								if (flip)
									DesStartIndex = (ResultRect.top - WorldLockRect.top) * DesMaxWidth
									+ (WorldLockRect.right - WorldLockRect.left) - (ResultRect.right - ResultRect.left) - 
									(ResultRect.left - WorldLockRect.left);

								Byte * pTData2 = pMapObject->GetPicData();// + 
									//(CurrRect.top * MaxEntSizeX + CurrRect.left) * sizeof(Color32);
								CRender::BlitSubtractivePic(pTData2, 0, SrcWidth, 
									SrcHeight, pMapObject->GetPicWidth(),
									pResultMask, DesStartIndex, DesMaxWidth, flip, TRUE, sizeof(Color32));

							}
						}


						break;
					}
				}
			}
			itrRenderLst ++;
			index++;
		}
	
		itrAL ++;
	}
	
	}
}
VOID CRender::SetObjectBlockAlpha( BlockPos m_CharPos, Byte * pResultMask, CMapObjectView * pMap, 
								  RECT WorldMapRect, UInt32 SrcMaxWidth, BOOL Flip)
{
	UInt32 BlockIndex = m_CharPos.PosY * pMap->GetMapData()->MaxBlockX + m_CharPos.PosX;
		//find object alpha
		StlMultiMap<UInt32,CAnimatedEntity *>::iterator itrAL = pMap->m_BkObjectIndexLst.find(BlockIndex);
		if (itrAL != pMap->m_BkObjectIndexLst.end())
		{
			Int count = (Int)pMap->m_BkObjectIndexLst.count(BlockIndex);
			for (Int i = 0; i < count; i ++)
			{
				StlMultiMap<UInt32, RenderLstData>::iterator itrRenderLst;
				CAnimatedEntity * pMapObject = itrAL->second;
				itrRenderLst = pMapObject->m_RenderLst.begin();
				Int index = 0;
				while (itrRenderLst != pMapObject->m_RenderLst.end())
				{
					if (pMapObject->GetEntityPart(index, ImagePic, &WorldMapRect))
					{
						StlVector<BlockPos> BkList = pMapObject->GetCurrBlockPositionLst(itrRenderLst->second.AlphaBkIndex);
						BlockPos ObjectPos = pMapObject->GetCharPos();
						//find whether the player is inside the blocking list
						for (Int i = 0; i < (Int)BkList.size(); i++)
						{
							BlockPos BkCurrPos = BkList[i];
							if (ObjectPos.PosX + BkCurrPos.PosX == m_CharPos.PosX && 
								ObjectPos.PosY + BkCurrPos.PosY == m_CharPos.PosY)
							{
								RECT CurrRect = pMapObject->GetCurrRect();

								RECT NextDirtyRegion;
								NextDirtyRegion.left = pMapObject->GetEntLeftUpPos().PixelPosX + pMapObject->m_EntityTotalRect.left;
								NextDirtyRegion.top = pMapObject->GetEntLeftUpPos().PixelPosY + pMapObject->m_EntityTotalRect.top;
								NextDirtyRegion.right = NextDirtyRegion.left + pMapObject->m_EntityTotalRect.right - pMapObject->m_EntityTotalRect.left;
								NextDirtyRegion.bottom = NextDirtyRegion.top + pMapObject->m_EntityTotalRect.bottom - pMapObject->m_EntityTotalRect.top;

								RECT ResultRect;
								if (QGLMath::CalculateOverlappedRect(NextDirtyRegion, WorldMapRect, ResultRect))
								{
									//D3DLOCKED_RECT d3dlt;
									//D3DSURFACE_DESC  pDescMapOb;

									//if(LockSystemTexture(pDescMapOb, d3dlt, &CurrRect, pMapObject->m_CurrTexture))
									if (!QGLMath::InvalidLock(&CurrRect, MaxEntSizeX, MaxEntSizeY))
									{
										UInt16 SrcWidth = (UInt16)ResultRect.right - (UInt16)ResultRect.left;
										UInt16 SrcHeight = (UInt16)ResultRect.bottom - (UInt16)ResultRect.top;

										//UInt32 SrcStartIndex = (ResultRect.top - NextDirtyRegion.top)  * pMapObject->GetPicWidth() 
										//	+ ResultRect.left - NextDirtyRegion.left;

										UInt32 DesStartIndex = (ResultRect.top - WorldMapRect.top)  * SrcMaxWidth
											+ ResultRect.left - WorldMapRect.left;

										Byte * pTData2 = pMapObject->GetPicData(); 


										BYTE PixelSizeDes = 4;
										BYTE PixelSizeSrc = 4;
										//pDesData += DesStartIndex * PixelSizeDes;
										Byte * pTempMask = pResultMask + DesStartIndex * 4;
										Byte * pSrcData = pTData2;
										//pSrcData += SrcStartIndex * PixelSizeSrc;
										UInt32 SrcLen = (pMapObject->GetPicWidth() * 4);
										UInt32 DesLen = (SrcMaxWidth * PixelSizeDes);
										//assume it is 32 bit
										Int UpdateSrcLen = (SrcWidth * PixelSizeSrc);
										Int UpdateDesLen = (SrcWidth * PixelSizeDes);
										Int DesMovePixel = PixelSizeDes;

										if (Flip)
										{
											pTempMask += UpdateDesLen - PixelSizeDes;
											UpdateDesLen *= -1;
											DesMovePixel *= -1;
										}
										UpdateSrcLen = SrcLen - UpdateSrcLen;
										UpdateDesLen = DesLen - UpdateDesLen;

										BYTE Data[4];
										for (UInt16 y = 0; y < SrcHeight; y++)
										{
											for (UInt16 x = 0; x < SrcWidth; x++)
											{				
												//BlitSinglePixel(pSrcData, pDesData);
												
												if (pTempMask[3] > 0)
												{
													//*(UInt32 *)Data = *(UInt32 *)pSrcData;
													Data[3] = pTempMask[3];
													*((UInt32 *) (pTempMask)) = MMXAlpha(*((UInt32 *) (pSrcData)), 
													*((UInt32 *) (pTempMask)));
													pTempMask[3] = Data[3];
												}
													
												//MMXAlpha(
												//pTempMask[3] = gGlobal.SubtractiveTable[pTempMask[3]][pSrcData[3]];

												pSrcData += PixelSizeSrc;
												pTempMask += DesMovePixel;

											}
											pSrcData += UpdateSrcLen;
											pTempMask += UpdateDesLen;

										}
									}
								}

								break;
							}
						}
					}
					itrRenderLst ++;
					index++;
				}

				itrAL ++;
			}
		}
}

VOID CRender::SetAlpha( BlockPos m_CharPos,
						UInt16 DesMaxHeight, UInt16 DesMaxWidth, 
						Byte * pResultMask, Byte * BgData, UInt16 BgWidth, CMapObjectView * pMap, RECT WorldMapRect)
{
	if (!pMap || !pMap->GetMapData() || pMap->GetMapFrameIndex() >= (Int)pMap->GetMapData()->BkAlphaIndexLstVec.size())
		return;
	if (pMap->GetMapData()->State != DATA_FINISH_LOADING)
		return;
	UInt32 BlockIndex = m_CharPos.PosY * pMap->GetMapData()->MaxBlockX + m_CharPos.PosX;
	Byte * pAlphaIndexData = pMap->GetMapData()->BkAlphaIndexLstVec[pMap->GetMapFrameIndex()][BlockIndex];
	Byte Count = *pAlphaIndexData; 
	for (Int i = 0; i < (Int)Count ; i ++)
	{
		UInt16 Index = *(UInt16 *)(pAlphaIndexData + sizeof(Byte) + i * sizeof(UInt16));

		if (Index >= pMap->GetMapData()->AlphaMaskLst.size())
			continue;

		MaskEffect * TempBound = &pMap->GetMapData()->AlphaMaskLst[Index];

		if (TempBound->pData == NULL)
			continue;

		RECT ResultRect;
		if (QGLMath::CalculateOverlappedRect(TempBound->Bound, WorldMapRect, ResultRect))
		{
			UInt16 SrcWidth = (UInt16)ResultRect.right - (UInt16)ResultRect.left;
			UInt16 SrcHeight = (UInt16)ResultRect.bottom - (UInt16)ResultRect.top;

			UInt32 SrcStartIndex = (ResultRect.top - TempBound->Bound.top) * (TempBound->Bound.right - 
				TempBound->Bound.left + 1)  + ResultRect.left - TempBound->Bound.left;

			UInt32 DesStartIndex = (ResultRect.top - WorldMapRect.top) * DesMaxWidth
				+ ResultRect.left - WorldMapRect.left;

			UInt32 BgStartIndex = (ResultRect.top - WorldMapRect.top) * BgWidth
				+ ResultRect.left - WorldMapRect.left;

			CRender::BlitAlphaPic(TempBound->pData, SrcStartIndex, SrcWidth, 
				SrcHeight, (UInt16)(TempBound->Bound.right - TempBound->Bound.left + 1),
				pResultMask, DesStartIndex,DesMaxWidth, FALSE, BgData, BgWidth, BgStartIndex, sizeof(Color32));
				
		}

	}

}


VOID CRender::SetSeaAlpha(Byte * pSrcData, RECT SrcRect, Byte SeaDepth, UInt16 SrcMaxWidth, UInt16 offsetY, Byte * BgData, UInt16 BgWidth,
						   RECT WinRect, CMapObjectView * pMap)
{
	//return;
	if (SeaDepth >= SeaDepthCri)
	{
		UInt16 depth = SeaDepth - SeaDepthCri;
		if (depth > offsetY)
			depth -= offsetY;
		else
			depth = 0;
		LONG bottom = SrcRect.bottom;
		if (bottom > (CharPicOffsetY ))
			bottom = (CharPicOffsetY );
		Int StartIndex = (bottom - depth - SrcRect.top) * SrcMaxWidth ;
		Int BgStartIndex = (bottom - depth - SrcRect.top) * BgWidth;
		if (StartIndex < 0)
			StartIndex = 0;
		UInt16 SrcWidth = (UInt16)(SrcRect.right - SrcRect.left);
		UInt16 SrcHeight = 0;
		if (bottom - depth < SrcRect.top)
		{
			SrcHeight = (UInt16)(bottom - SrcRect.top + (SrcRect.bottom - bottom));
			StartIndex = 0;
			BgStartIndex = 0;
		}
		else
			SrcHeight = depth + (UInt16)(SrcRect.bottom - bottom);
		BlitSeaAlphaPic(pSrcData, StartIndex, SrcWidth, SrcHeight, SrcMaxWidth,BgData,BgWidth,BgStartIndex, sizeof(Color32));
	}

}

VOID CRender::UpdatePlayerEntEffectBeforeCombine(CEntityBase * pEnt, Byte * pSrcTData, D3DSURFACE_DESC  pDescDes, 
											  RECT LockRegion, CMapObjectView * pMap, RECT m_EntityTotalRect, BOOL Flip )
{
	if (!pMap)
		return;

	if (pMap->IsBlockInShadow(pEnt->GetCharPos()) && pEnt->m_CurrPartBlitType == NoEffect)
	{
		SetShadow(  pEnt->GetCharPos(),
			pDescDes.Height, pDescDes.Width, pSrcTData, Flip, pMap, LockRegion);
	}
	
	if (pMap->IsBlockInObject(pEnt->GetCharPos()) && pEnt->m_CurrPartBlitType == NoEffect)
		SetObjectBlockShadow( pEnt->GetCharPos(), 
		 pDescDes.Height, pDescDes.Width, pSrcTData, Flip,  LockRegion, pMap);
		 

}

BOOL CRender::DrawEntityToTexture(CEntityBase * pEnt, DrawEntityTextureData & GivenTextureData, RECT * LockRegion, BOOL Copy)
{
	StlMap<CEntityBase *, RECT>::iterator NextDirRectItr = GivenTextureData.m_NextDirtyRegionLst.find(pEnt);
	RECT NextDirtyRegion;
	
	if (LockRegion)
		NextDirtyRegion = *LockRegion;
	else if (NextDirRectItr != GivenTextureData.m_NextDirtyRegionLst.end())
		NextDirtyRegion = NextDirRectItr->second;
	else
		return FALSE;

	D3DSURFACE_DESC  pDescDes;
	D3DLOCKED_RECT d3dltDes;
	if (!LockSystemTexture(pDescDes, d3dltDes, &NextDirtyRegion, GivenTextureData.Texture, 0))
		return FALSE;
	
	RECT TotalEntLockRect;
	TotalEntLockRect.left = NextDirtyRegion.left - pEnt->GetEntLeftUpPos().PixelPosX ;
	TotalEntLockRect.right = TotalEntLockRect.left + pEnt->m_EntityTotalRect.right - pEnt->m_EntityTotalRect.left;
	TotalEntLockRect.top = NextDirtyRegion.top - pEnt->GetEntLeftUpPos().PixelPosY ;
	TotalEntLockRect.bottom = TotalEntLockRect.top + pEnt->m_EntityTotalRect.bottom - pEnt->m_EntityTotalRect.top;

	Byte * pDesTData = (Byte *)d3dltDes.pBits;
	
	for (int i = 0; i < pEnt->GetTotalRenderPart() ; i++)
	{
		BOOL Flip = pEnt->GetFlip(i);
		for (Int j = 0; j < 2; j ++)
		{
			BOOL Get = FALSE;

			if (j == 0)
				Get = pEnt->GetEntityPart(i, ShadowPic, NULL);
			else if ( j == 1)
				Get = pEnt->GetEntityPart(i, ImagePic, NULL);
			
			if(Get)
			{
				RECT CurrRect = pEnt->GetCurrRect();

				Int ObjectLeftUpX = pEnt->GetEntLeftUpPos().PixelPosX
					+ CurrRect.left ;

				Int ObjectLeftUpY = pEnt->GetEntLeftUpPos().PixelPosY
					+ CurrRect.top ;

				RECT rect2;
				rect2.left = ObjectLeftUpX;
				rect2.top = ObjectLeftUpY;
				rect2.right = ObjectLeftUpX + CurrRect.right - CurrRect.left;
				rect2.bottom = ObjectLeftUpY + CurrRect.bottom - CurrRect.top;

				RECT resultRect;

				if (QGLMath::CalculateOverlappedRect(NextDirtyRegion,rect2, resultRect))
				{
					RECT SubEntLockRect;

					SubEntLockRect.left = resultRect.left - rect2.left + CurrRect.left;
					SubEntLockRect.right = resultRect.right - rect2.left + CurrRect.left;

					if (Flip)
					{
						SubEntLockRect.left -= resultRect.left - rect2.left;
						SubEntLockRect.left += rect2.right - resultRect.right;
						SubEntLockRect.right -= resultRect.left - rect2.left;
						SubEntLockRect.right += rect2.right - resultRect.right;
					}

					SubEntLockRect.top = resultRect.top - rect2.top + CurrRect.top;
					SubEntLockRect.bottom = resultRect.bottom - rect2.top  + CurrRect.top;

					D3DSURFACE_DESC  pDescSrc;

					//check invalid lock
					if (QGLMath::InvalidLock(&SubEntLockRect, MaxEntSizeX, MaxEntSizeY))
						continue;
				
					pDescSrc.Width = pEnt->GetPicWidth();
					pDescSrc.Height = MaxEntSizeY;

					Byte * pSrcTData = pEnt->GetPicData();

					if (pEnt->GetHighLight())
					{
						Byte * pTempSrcTData = pSrcTData;
						Int Len = (SubEntLockRect.right - SubEntLockRect.left) * sizeof(Color32);
						for (Int j = 0; j < SubEntLockRect.bottom - SubEntLockRect.top; j ++)
						{
							for (Int i = 0; i < SubEntLockRect.right - SubEntLockRect.left; i ++)
							{
								pTempSrcTData[0] = gGlobal.AddtitiveTable[pTempSrcTData[0]][HighLightValue];
								pTempSrcTData[1] = gGlobal.AddtitiveTable[pTempSrcTData[1]][HighLightValue];
								pTempSrcTData[2] = gGlobal.AddtitiveTable[pTempSrcTData[2]][HighLightValue];
								pTempSrcTData += sizeof(Color32);
							}
							pTempSrcTData += MaxEntSizeX * sizeof(Color32) - Len;
						}
					}


					Int DesStartIndex = (SubEntLockRect.top - TotalEntLockRect.top) * pDescDes.Width + 
						(SubEntLockRect.left - TotalEntLockRect.left);
					if (Flip)
					{
						DesStartIndex += resultRect.left - rect2.left;
						DesStartIndex -= rect2.right - resultRect.right;
					}

					if (Copy)
					{
						UInt copyLen = (SubEntLockRect.right - SubEntLockRect.left) * sizeof(Color32);
						pDesTData += DesStartIndex * sizeof(Color32);

						
						for (Int y = 0; y < SubEntLockRect.bottom - SubEntLockRect.top - 1; y++)
							{
								memcpy(pDesTData, pSrcTData, copyLen);
								pDesTData += pDescDes.Width * sizeof(Color32);
								pSrcTData += pDescSrc.Width * sizeof(Color32);
							}
						
					}
					else
					{
						if (pEnt->m_CurrPartBlitType == NoEffect)
						{

							BlitPic(pSrcTData, 0, (UInt16)(SubEntLockRect.right - SubEntLockRect.left ), 
								(UInt16)(SubEntLockRect.bottom - SubEntLockRect.top ), pDescSrc.Width,
								pDesTData, DesStartIndex, (UInt16)pDescDes.Width, 
								Flip, pEnt->GetAlphaOffset(), sizeof(Color32), sizeof(Color32));

						}
						else if (pEnt->m_CurrPartBlitType == Additive)
							BlitAdditivePic(pSrcTData, 0, (UInt16)(SubEntLockRect.right - SubEntLockRect.left ), 
							(UInt16)(SubEntLockRect.bottom - SubEntLockRect.top ), pDescSrc.Width,
							pDesTData, DesStartIndex, (UInt16)pDescDes.Width, Flip, sizeof(Color32));
						else if (pEnt->m_CurrPartBlitType == Subtractive)
							BlitSubtractivePic(pSrcTData, 0, (UInt16)(SubEntLockRect.right - SubEntLockRect.left ), 
							(UInt16)(SubEntLockRect.bottom -SubEntLockRect.top ), pDescSrc.Width,
							pDesTData, DesStartIndex, (UInt16)pDescDes.Width, 
							Flip, FALSE, sizeof(Color32));
							
					}

				}
			}
		}
	}

	GivenTextureData.Texture->UnlockRect(0);

	return TRUE;
}

//draw the entity, if the Given LockRegion is NULL, draw the whole entity
BOOL CRender::DrawEntityToWorld(CEntityBase * pEnt, RECT * LockRegion, CMapObjectView * pMap, CCamera * CurrCam, BOOL AddEffect)
{
	//CEntityBase * pEnt;
	//return FALSE;
	RECT CopyBgRect;
	
	StlMap<CEntityBase *, RECT>::iterator NextDirRectItr = m_CombinedEntityTexture.m_NextDirtyRegionLst.find(pEnt);
	RECT NextDirtyRegion;
	
	if (NextDirRectItr != m_CombinedEntityTexture.m_NextDirtyRegionLst.end())
		NextDirtyRegion = NextDirRectItr->second;
	else
		return false;
	
	if (LockRegion)
	{
		if (!QGLMath::CalculateOverlappedRect(NextDirtyRegion, *LockRegion, NextDirtyRegion))
			return false;
	}
	CopyBgRect.left = NextDirtyRegion.left + (Int)CurrCam->GetLeftUpPos().x - 
		pEnt->GetEntLeftUpPos().PixelPosX;
	CopyBgRect.right = NextDirtyRegion.right + (Int)CurrCam->GetLeftUpPos().x - 
		pEnt->GetEntLeftUpPos().PixelPosX;
	CopyBgRect.top = NextDirtyRegion.top + (Int)CurrCam->GetLeftUpPos().y - 
		pEnt->GetEntLeftUpPos().PixelPosY;
	CopyBgRect.bottom = NextDirtyRegion.bottom + (Int)CurrCam->GetLeftUpPos().y - 
		pEnt->GetEntLeftUpPos().PixelPosY;

	RECT TotalEntLockRect;
	TotalEntLockRect.left = NextDirtyRegion.left  + (Int)CurrCam->GetLeftUpPos().x 
		- pEnt->GetEntLeftUpPos().PixelPosX ;
	TotalEntLockRect.right = TotalEntLockRect.left + pEnt->m_EntityTotalRect.right - pEnt->m_EntityTotalRect.left;
	TotalEntLockRect.top = NextDirtyRegion.top  + (Int)CurrCam->GetLeftUpPos().y 
		- pEnt->GetEntLeftUpPos().PixelPosY ;
	TotalEntLockRect.bottom = TotalEntLockRect.top + pEnt->m_EntityTotalRect.bottom - pEnt->m_EntityTotalRect.top;

	//copy the background for effect use
	RECT ShiftRect = NextDirtyRegion;
	ShiftRect.left += m_CombinedEntityTexture.m_CurrDisArea.left;
	ShiftRect.right += m_CombinedEntityTexture.m_CurrDisArea.left;
	ShiftRect.top += m_CombinedEntityTexture.m_CurrDisArea.top;
	ShiftRect.bottom += m_CombinedEntityTexture.m_CurrDisArea.top;
	
	D3DSURFACE_DESC  pDescDes;
	pDescDes.Width = MaxEntSizeX;
	pDescDes.Height = MaxEntSizeY;
	//D3DLOCKED_RECT d3dltDes;
	//if (!LockSystemTexture(pDescDes, d3dltDes, &ShiftRect, m_CombinedEntityTexture.Texture))
	//	return FALSE;

	if (QGLMath::InvalidLock(&ShiftRect, MaxEntSizeX, MaxEntSizeY))
		return FALSE;
	
	RECT WorldLockRegion = NextDirtyRegion;
	
	WorldLockRegion.left += (Int)CurrCam->GetLeftUpPos().x;
	WorldLockRegion.right += (Int)CurrCam->GetLeftUpPos().x;
	WorldLockRegion.top += (Int)CurrCam->GetLeftUpPos().y;
	WorldLockRegion.bottom += (Int)CurrCam->GetLeftUpPos().y;

	Byte * pDesTData = m_CombinedEntityTexture.pData + (ShiftRect.top * pDescDes.Width + ShiftRect.left) * 4;

	for (int i = 0; i < pEnt->GetTotalRenderPart() ; i++)
	{
		BOOL Flip = pEnt->GetFlip(i);
		BYTE EquipType = pEnt->GetEquipType(i);
		for (Int j = 0; j < 2; j ++)
		{
			BOOL Get = FALSE;

			if (j == 0)
				Get = pEnt->GetEntityPart(i, ShadowPic, &WorldLockRegion);
			else if ( j == 1)
				Get = pEnt->GetEntityPart(i, ImagePic, &WorldLockRegion);
			
			if(Get)
			{
				RECT CurrRect = pEnt->GetCurrRect();
				Int ObjectLeftUpX = pEnt->GetEntLeftUpPos().PixelPosX
					+ CurrRect.left ;

				Int ObjectLeftUpY = pEnt->GetEntLeftUpPos().PixelPosY
					+ CurrRect.top ;

				RECT rect1;
				RECT rect2;
				RECT resultRect;

				rect1.left = (Int)CurrCam->GetLeftUpPos().x + NextDirtyRegion.left;
				rect1.top = (Int)CurrCam->GetLeftUpPos().y + NextDirtyRegion.top;
				rect1.right = (Int)CurrCam->GetLeftUpPos().x + NextDirtyRegion.right;
				rect1.bottom = (Int)CurrCam->GetLeftUpPos().y + NextDirtyRegion.bottom;

				rect2.left = ObjectLeftUpX;
				rect2.top = ObjectLeftUpY;
				rect2.right = ObjectLeftUpX + CurrRect.right - CurrRect.left;
				rect2.bottom = ObjectLeftUpY + CurrRect.bottom - CurrRect.top;

				if (QGLMath::CalculateOverlappedRect(rect1,rect2, resultRect))
				{
					RECT SubEntLockRect;

					SubEntLockRect.left = resultRect.left - rect2.left + CurrRect.left;
					SubEntLockRect.right = resultRect.right - rect2.left + CurrRect.left;

					if (Flip)
					{
						SubEntLockRect.left -= resultRect.left - rect2.left;
						SubEntLockRect.left += rect2.right - resultRect.right;
						SubEntLockRect.right -= resultRect.left - rect2.left;
						SubEntLockRect.right += rect2.right - resultRect.right;
					}

					SubEntLockRect.top = resultRect.top - rect2.top + CurrRect.top;
					SubEntLockRect.bottom = resultRect.bottom - rect2.top  + CurrRect.top;

					//D3DLOCKED_RECT d3dltSrc;
					D3DSURFACE_DESC  pDescSrc;

					Int DesStartIndex = 0;

					Int SrcTop = SubEntLockRect.top;

					if (SubEntLockRect.top < 0)
					{
						SubEntLockRect.top = 0;
					}
					
					//check invalid lock
					if (QGLMath::InvalidLock(&SubEntLockRect, MaxEntSizeX, MaxEntSizeY))
					{
						DebugBreak();
						continue;
					}

					pDescSrc.Width = pEnt->GetPicWidth();
					pDescSrc.Height = MaxEntSizeY;

					Byte * pSrcTData = pEnt->GetPicData();

					SubEntLockRect.top = SrcTop;

					if (m_CurPosEntity != pEnt)
					{

						RECT bound = resultRect;
						bound.left -= (Int)CurrCam->GetLeftUpPos().x;
						bound.right -= (Int)CurrCam->GetLeftUpPos().x;
						bound.top -= (Int)CurrCam->GetLeftUpPos().y;
						bound.bottom -= (Int)CurrCam->GetLeftUpPos().y;

						if (m_CursorX >= (Int)bound.left &&
							m_CursorX <= (Int)bound.right &&
							m_CursorY >= (Int)bound.top &&
							m_CursorY <= (Int)bound.bottom)
						{
							if (EquipType == EQUIP_HEAD || 
								EquipType == EQUIP_BODY ||
								EquipType == EQUIP_HAIR ||
								EquipType == EQUIP_MOUNT)
							{
								Int Len = pDescSrc.Width * sizeof(Color32);
								BYTE * pData = NULL;

								if (Flip && bound.right - bound.left > 1)
									pData = pSrcTData + ((bound.right - bound.left) - (m_CursorX - bound.left )) * sizeof(Color32) + Len * (m_CursorY - bound.top);
								else
									pData = pSrcTData + (m_CursorX - bound.left ) * sizeof(Color32) + Len * (m_CursorY - bound.top);

								if (pData[3] > 10)
									m_CurPosEntity = pEnt;
							}
						}
					}
					//highlight the entity if it is selected
					if (pEnt->GetHighLight())
					{
						Byte * pTempSrcTData = pSrcTData;
						Int Len = (SubEntLockRect.right - SubEntLockRect.left) * sizeof(Color32);
						for (Int j = 0; j < SubEntLockRect.bottom - SubEntLockRect.top; j ++)
						{
							for (Int i = 0; i < SubEntLockRect.right - SubEntLockRect.left; i ++)
							{
								pTempSrcTData[0] = gGlobal.AddtitiveTable[pTempSrcTData[0]][HighLightValue];
								pTempSrcTData[1] = gGlobal.AddtitiveTable[pTempSrcTData[1]][HighLightValue];
								pTempSrcTData[2] = gGlobal.AddtitiveTable[pTempSrcTData[2]][HighLightValue];
								pTempSrcTData += sizeof(Color32);
							}
							pTempSrcTData += pDescSrc.Width * sizeof(Color32) - Len;
						}
					}
					
					if (pEnt->GetHaveEffect() && AddEffect)
						UpdatePlayerEntEffectBeforeCombine(pEnt, pSrcTData, pDescSrc, resultRect, pMap, CopyBgRect, Flip);

					DesStartIndex += (SubEntLockRect.top - TotalEntLockRect.top) * pDescDes.Width + 
						(SubEntLockRect.left - TotalEntLockRect.left);

					if (Flip)
					{
						DesStartIndex += resultRect.left - rect2.left;
						DesStartIndex -= rect2.right - resultRect.right;
					}

					if (pEnt->GetHaveEffect() && AddEffect)
					{
						GetAlphaMask(pEnt->GetCharPos(), pSrcTData, pMap, resultRect, pDescSrc.Width, Flip);
					}
					if (pEnt->m_CurrPartBlitType == NoEffect)
					{
						BlitPic(pSrcTData, 0, (UInt16)(SubEntLockRect.right - SubEntLockRect.left ), 
								(UInt16)(SubEntLockRect.bottom - SubEntLockRect.top ), pDescSrc.Width,
								pDesTData, DesStartIndex, (UInt16)pDescDes.Width, 
								Flip, pEnt->GetAlphaOffset(), sizeof(Color32), sizeof(Color32));
						
					}
					else if (pEnt->m_CurrPartBlitType == Additive)
						BlitAdditivePic(pSrcTData, 0, (UInt16)(SubEntLockRect.right - SubEntLockRect.left ), 
						(UInt16)(SubEntLockRect.bottom - SubEntLockRect.top ), pDescSrc.Width,
						pDesTData, DesStartIndex, (UInt16)pDescDes.Width, Flip, sizeof(Color32));
					else if (pEnt->m_CurrPartBlitType == Subtractive)
						BlitSubtractivePic(pSrcTData, 0, (UInt16)(SubEntLockRect.right - SubEntLockRect.left ), 
						(UInt16)(SubEntLockRect.bottom -SubEntLockRect.top ), pDescSrc.Width,
						pDesTData, DesStartIndex, (UInt16)pDescDes.Width, 
						Flip, FALSE, sizeof(Color32));

					

					

				}
			}
		}
	}

	/*if (pMap->IsBlockInObject(pEnt->GetCharPos()))
	{
		SetObjectBlockAlpha( pEnt->GetCharPos(), pDesTData,  pMap, WorldLockRegion, pDescDes.Width, FALSE);
	}*/
	//m_CombinedEntityTexture.Texture->UnlockRect(0);

	return TRUE;
}
VOID CRender::AddMapDirtyRect(CMapObjectView * pMap, CCamera * CurrCam)
{
	if (pMap == NULL ||
		pMap->GetMapData() == NULL ||
		CurrCam == NULL)
		return;

	if (pMap->GetMapData()->State < DATA_FINISH_LOADING_BGMAP)
		return;

	if (pMap->GetMapPreFrameIndex() >= (Int)pMap->GetMapData()->dirtyRectLst.size())
		return;

	RECT currWorldRect = m_OrgWinRect;
	currWorldRect.left += (LONG)CurrCam->GetLeftUpPos().x;
	currWorldRect.right += (LONG)CurrCam->GetLeftUpPos().x;
	currWorldRect.top += (LONG)CurrCam->GetLeftUpPos().y;
	currWorldRect.bottom += (LONG)CurrCam->GetLeftUpPos().y;
	for (UInt i = 0; i < pMap->GetMapData()->dirtyRectLst[pMap->GetMapPreFrameIndex()].size(); ++i)
	{
		RECT resultRect;
		RECT mapdirty = pMap->GetMapData()->dirtyRectLst[pMap->GetMapPreFrameIndex()][i];
		if (QGLMath::CalculateOverlappedRect(mapdirty,
			currWorldRect, resultRect))
		{
			RECT WinRect = resultRect;
			WinRect.left -= (LONG)CurrCam->GetLeftUpPos().x;
			WinRect.right -= (LONG)CurrCam->GetLeftUpPos().x;
			WinRect.top -= (LONG)CurrCam->GetLeftUpPos().y;
			WinRect.bottom -= (LONG)CurrCam->GetLeftUpPos().y;
			m_CombinedEntityTexture.m_DirRectMan.AddDirtyRectToMan(WinRect);
		}
	}
	/*Int RowPerPic = 0;
	Int indexX = 0;
	Int MaxIndexX = 0;
	if (pMap->GetSubMapWidth() > 0)
	{
		RowPerPic = (Int)ceil(((Real32)pMap->GetMapWidth() / (Real32)pMap->GetSubMapWidth()));
		indexX = (Int)CurrCam->GetLeftUpPos().x / pMap->GetSubMapWidth();
		MaxIndexX = (Int)ceil(((Real32)(CurrCam->GetLeftUpPos().x + m_CurrWinWidth) / (Real32)pMap->GetSubMapWidth())); 
	}
	Int indexY = 0;
	Int MaxIndexY = 0;

	if (pMap->GetSubMapHeight() > 0)
	{
		indexY = (Int)CurrCam->GetLeftUpPos().y / pMap->GetSubMapHeight();
		MaxIndexY = (Int)ceil(((Real32)(CurrCam->GetLeftUpPos().y + m_CurrWinHeight) / (Real32)pMap->GetSubMapHeight())); 
	}

	Int PicCount = indexY * RowPerPic + indexX;

	for (Int y = indexY; y <= MaxIndexY; y ++)
	{
		for (Int x = indexX; x <= MaxIndexX; x ++)
		{
			Int subIndex = (y * RowPerPic + x) * pMap->GetMapData()->totalSubFrame;
			Int preSubIndex = subIndex + pMap->GetMapPreFrameIndex();
			subIndex += pMap->GetMapFrameIndex();

			if (subIndex >= 0 && preSubIndex >= 0)
			{
				if (pMap->GetBgTextureByIndex(subIndex) != pMap->GetBgTextureByIndex(preSubIndex))
				{
					RECT DirtyRect;
					DirtyRect.bottom = 0;
					RECT TempRect;
					Int WinLeftUpX =   (Int)(x * pMap->GetSubMapWidth()) - (Int)CurrCam->GetLeftUpPos().x ;
					Int WinLeftUpY =  (Int)(y * pMap->GetSubMapHeight()) - (Int)CurrCam->GetLeftUpPos().y ;
					if (WinLeftUpX < (Int)m_CurrWinWidth && WinLeftUpY < (Int)m_CurrWinHeight)
					{
						TempRect.left = TempRect.top = 0;

						if (WinLeftUpX < 0)
						{
							TempRect.left = -1 * WinLeftUpX;
							WinLeftUpX = 0;
						}
						if (WinLeftUpY < 0)
						{
							TempRect.top = -1 * WinLeftUpY;
							WinLeftUpY = 0;
						}

						TempRect.bottom = pMap->GetSubMapHeight();					
						TempRect.right = pMap->GetSubMapWidth();

						RECT LockRegion;
						LockRegion.left = WinLeftUpX;
						LockRegion.top = WinLeftUpY;
						LockRegion.right = LockRegion.left + TempRect.right - TempRect.left;
						LockRegion.bottom = LockRegion.top + TempRect.bottom -TempRect.top; 

						RECT TempLockRegion = LockRegion;

						if (QGLMath::CalculateOverlappedRect(TempLockRegion, m_OrgWinRect, LockRegion))
						{
							m_CombinedEntityTexture.m_DirRectMan.AddDirtyRectToMan(LockRegion);
						}
					}
				}
			}
		}
	}*/
}
VOID CRender::DrawBgMap(CMapObjectView * pMap, RECT UpdateRect, BOOL Clear, CCamera * CurrCam)
{
	if (pMap == NULL ||
		pMap->GetMapData() == NULL ||
		CurrCam == NULL)
	{
		//reset the background
		RECT ShiftRect = UpdateRect;
		ShiftRect.left += m_CombinedEntityTexture.m_CurrDisArea.left;
		ShiftRect.right += m_CombinedEntityTexture.m_CurrDisArea.left;
		ShiftRect.top += m_CombinedEntityTexture.m_CurrDisArea.top;
		ShiftRect.bottom += m_CombinedEntityTexture.m_CurrDisArea.top;
		//ClearTexture(m_CombinedEntityTexture.pData, &ShiftRect);
		ClearData(m_CombinedEntityTexture.pData, MaxEntSizeX, MaxEntSizeY, 4, &ShiftRect);  
		//for (Iny y = 0; y < ShiftRect.
		return;
	}

	//Int index = pMap->GetMapIndexByBlockPos(Player.GetCharPos());
	//Int index = pMap->GetMapIndexByBlockPos(Player.GetCharPos());
	Int RowPerPic = 0;
	Int indexX = 0;
	Int MaxIndexX = 0;
	if (pMap->GetSubMapWidth() > 0)
	{
		RowPerPic = (Int)ceil(((Real32)pMap->GetMapWidth() / (Real32)pMap->GetSubMapWidth()));
		indexX = (Int)CurrCam->GetLeftUpPos().x / pMap->GetSubMapWidth();
		MaxIndexX = (Int)ceil(((Real32)(CurrCam->GetLeftUpPos().x + m_CurrWinWidth) / (Real32)pMap->GetSubMapWidth())); 
	}
	Int indexY = 0;
	Int MaxIndexY = 0;

	if (pMap->GetSubMapHeight() > 0)
	{
		indexY = (Int)CurrCam->GetLeftUpPos().y / pMap->GetSubMapHeight();
		MaxIndexY = (Int)ceil(((Real32)(CurrCam->GetLeftUpPos().y + m_CurrWinHeight) / (Real32)pMap->GetSubMapHeight())); 
	}

	Int PicCount = indexY * RowPerPic + indexX;

	for (Int y = indexY; y <= MaxIndexY; y ++)
	{
		for (Int x = indexX; x <= MaxIndexX; x ++)
		{
			Int subIndex = (y * RowPerPic + x) * pMap->GetMapData()->totalSubFrame;
			subIndex += pMap->GetMapFrameIndex();

			if (subIndex >= 0)
			{
				RECT DirtyRect;
				DirtyRect.bottom = 0;
				RECT TempRect;
				Int WinLeftUpX =   (Int)(x * pMap->GetSubMapWidth()) - (Int)CurrCam->GetLeftUpPos().x ;
				Int WinLeftUpY =  (Int)(y * pMap->GetSubMapHeight()) - (Int)CurrCam->GetLeftUpPos().y ;
				if (WinLeftUpX < (Int)m_CurrWinWidth && WinLeftUpY < (Int)m_CurrWinHeight)
				{
					TempRect.left = TempRect.top = 0;

					if (WinLeftUpX < 0)
					{
						TempRect.left = -1 * WinLeftUpX;
						WinLeftUpX = 0;
					}
					if (WinLeftUpY < 0)
					{
						TempRect.top = -1 * WinLeftUpY;
						WinLeftUpY = 0;
					}

					TempRect.bottom = pMap->GetSubMapHeight();					
					TempRect.right = pMap->GetSubMapWidth();

					RECT LockRegion;
					LockRegion.left = WinLeftUpX;
					LockRegion.top = WinLeftUpY;
					LockRegion.right = LockRegion.left + TempRect.right - TempRect.left;
					LockRegion.bottom = LockRegion.top + TempRect.bottom -TempRect.top; 

					RECT TempLockRegion = LockRegion;

					if (QGLMath::CalculateOverlappedRect(TempLockRegion, UpdateRect, LockRegion))
					{
						RECT SrcUpdateRect = TempRect;
						SrcUpdateRect.left += (LockRegion.left - WinLeftUpX);
						SrcUpdateRect.right = SrcUpdateRect.left + (LockRegion.right - LockRegion.left);

						SrcUpdateRect.top += (LockRegion.top - WinLeftUpY);
						SrcUpdateRect.bottom = SrcUpdateRect.top + (LockRegion.bottom - LockRegion.top);

						//D3DSURFACE_DESC  pDesc;
						Byte * pData = pMap->GetBgTextureByIndex(subIndex);
						if (pData)
						{
							//pTexture->GetLevelDesc(0, &pDesc);
							//UpdateWorldTextureBG(pTexture,
							//	SrcUpdateRect, pDesc.Width, LockRegion);
							
							UpdateWorldDDSToBG(pData,
								SrcUpdateRect, LockRegion, pMap->GetSubMapWidth());

						}
						else
						{
							RECT ShiftRect = LockRegion;
							ShiftRect.left += m_CombinedEntityTexture.m_CurrDisArea.left;
							ShiftRect.right += m_CombinedEntityTexture.m_CurrDisArea.left;
							ShiftRect.top += m_CombinedEntityTexture.m_CurrDisArea.top;
							ShiftRect.bottom += m_CombinedEntityTexture.m_CurrDisArea.top;
							//ClearTexture(m_CombinedEntityTexture.Texture, &ShiftRect);
							ClearData(m_CombinedEntityTexture.pData, MaxEntSizeX, MaxEntSizeY, 4, &ShiftRect);  
						}
						//draw the Map object if it is overlapped with the current bg
						/*for (Int i = 0; i < (Int)pMap->m_MapObjectLst.size(); i++)
						{
							DrawEntityToWorld(pMap->m_MapObjectLst[i],   &LockRegion, pMap,
								CurrCam, FALSE);
						}*/

					}
				}
			}
		}
	}
}

VOID CRender::UpdateCameraMove(CMapObjectView * pMap, CCamera * CurrCam)
{
	if (CurrCam == NULL)
		return;
	//redraw part of the StlMap if the player is moving
	if ( CurrCam->GetCameraHadMoved())
	{
		D3DSURFACE_DESC   pTextDesc;
		pTextDesc.Width = m_CurrWinWidth;
		pTextDesc.Height = m_CurrWinHeight;
		for (StlMap<CEntityBase *, RECT>::iterator PreDirRectItr = m_CombinedEntityTexture.m_PreDirtyRegionLst.begin();
			PreDirRectItr != m_CombinedEntityTexture.m_PreDirtyRegionLst.end(); ++PreDirRectItr)
		{
			PreDirRectItr->second.left -= (Int)CurrCam->GetPosMoveOffset().x;

			PreDirRectItr->second.right -= (Int)CurrCam->GetPosMoveOffset().x;

			PreDirRectItr->second.top -= (Int)CurrCam->GetPosMoveOffset().y;

			PreDirRectItr->second.bottom -= (Int)CurrCam->GetPosMoveOffset().y;

			if (PreDirRectItr->second.left < m_OrgWinRect.left)
				PreDirRectItr->second.left = m_OrgWinRect.left;
			else if (PreDirRectItr->second.left > m_OrgWinRect.right)
				PreDirRectItr->second.left = m_OrgWinRect.right;

			if (PreDirRectItr->second.right < m_OrgWinRect.left)
				PreDirRectItr->second.right = m_OrgWinRect.left;
			else if (PreDirRectItr->second.right > m_OrgWinRect.right)
				PreDirRectItr->second.right = m_OrgWinRect.right;

			if (PreDirRectItr->second.top < m_OrgWinRect.top)
				PreDirRectItr->second.top = m_OrgWinRect.top;
			else if (PreDirRectItr->second.top > m_OrgWinRect.bottom)
				PreDirRectItr->second.top = m_OrgWinRect.bottom;

			if (PreDirRectItr->second.bottom < m_OrgWinRect.top)
				PreDirRectItr->second.bottom = m_OrgWinRect.top;
			else if (PreDirRectItr->second.bottom > m_OrgWinRect.bottom)
				PreDirRectItr->second.bottom = m_OrgWinRect.bottom;

			if (QGLMath::InvalidLock(&PreDirRectItr->second, pTextDesc.Width, pTextDesc.Height))
			{
				PreDirRectItr->second.bottom = PreDirRectItr->second.right =
					PreDirRectItr->second.left = PreDirRectItr->second.top = 0;
			}
		}


		for (Int i = 0; i < (Int)pMap->m_MapObjectLst.size(); i++)
		{
			StlMap<CEntityBase *, RECT>::iterator PreDirRectItr = m_CombinedEntityTexture.m_PreDirtyRegionLst.find(pMap->m_MapObjectLst[i]);
			if (PreDirRectItr != m_CombinedEntityTexture.m_PreDirtyRegionLst.end())
			{
				PreDirRectItr->second.left -= (Int)CurrCam->GetPosMoveOffset().x;

				PreDirRectItr->second.right -= (Int)CurrCam->GetPosMoveOffset().x;

				PreDirRectItr->second.top -= (Int)CurrCam->GetPosMoveOffset().y;

				PreDirRectItr->second.bottom -= (Int)CurrCam->GetPosMoveOffset().y;

				if (PreDirRectItr->second.left < m_OrgWinRect.left)
					PreDirRectItr->second.left = m_OrgWinRect.left;
				else if (PreDirRectItr->second.left > m_OrgWinRect.right)
					PreDirRectItr->second.left = m_OrgWinRect.right;

				if (PreDirRectItr->second.right < m_OrgWinRect.left)
					PreDirRectItr->second.right = m_OrgWinRect.left;
				else if (PreDirRectItr->second.right > m_OrgWinRect.right)
					PreDirRectItr->second.right = m_OrgWinRect.right;

				if (PreDirRectItr->second.top < m_OrgWinRect.top)
					PreDirRectItr->second.top = m_OrgWinRect.top;
				else if (PreDirRectItr->second.top > m_OrgWinRect.bottom)
					PreDirRectItr->second.top = m_OrgWinRect.bottom;

				if (PreDirRectItr->second.bottom < m_OrgWinRect.top)
					PreDirRectItr->second.bottom = m_OrgWinRect.top;
				else if (PreDirRectItr->second.bottom > m_OrgWinRect.bottom)
					PreDirRectItr->second.bottom = m_OrgWinRect.bottom;


				if (QGLMath::InvalidLock(&PreDirRectItr->second, pTextDesc.Width, pTextDesc.Height))
				{
					PreDirRectItr->second.bottom = PreDirRectItr->second.right =
						PreDirRectItr->second.left = PreDirRectItr->second.top = 0;
				}
			}
		}
	}
}
VOID CRender::CameraMoveDraw(CMapObjectView * pMap, StlMultiMap<UInt32, CEntityBase *> & EntRenderLst, CCamera * CurrCam, 
							 DrawEntityTextureData & NightMaskData)
{
	if (CurrCam == NULL)
		return;
	//redraw part of the StlMap if the player is moving
	if ( CurrCam->GetCameraHadMoved())
	{
		RECT MapUpdateRectH;
		MapUpdateRectH.left = MapUpdateRectH.bottom = MapUpdateRectH.right = 
			MapUpdateRectH.top = 0;
		RECT MapUpdateRectV;
		MapUpdateRectV.left = MapUpdateRectV.bottom = MapUpdateRectV.right = 
			MapUpdateRectV.top = 0;

		RECT SrcRect = m_OrgWinRect;
		RECT DesRect = m_OrgWinRect;
		
		
			if ((Int)CurrCam->GetPosMoveOffset().y < 0)
			{
				MapUpdateRectV.left = 0;
				MapUpdateRectV.right = m_OrgWinRect.right;
				MapUpdateRectV.top = 0;
				MapUpdateRectV.bottom = -1 * (Int)CurrCam->GetPosMoveOffset().y;

				//move the screen
				
				SrcRect.bottom = m_OrgWinRect.bottom + (Int)CurrCam->GetPosMoveOffset().y;
				DesRect.top = -1 * (Int)CurrCam->GetPosMoveOffset().y;
				
			}
			else if ((Int)CurrCam->GetPosMoveOffset().y > 0)
			{
				MapUpdateRectV.left = 0;
				MapUpdateRectV.right = m_OrgWinRect.right;
				MapUpdateRectV.top = m_OrgWinRect.bottom - (Int)CurrCam->GetPosMoveOffset().y;
				MapUpdateRectV.bottom = m_OrgWinRect.bottom;

				
				SrcRect.top = (Int)CurrCam->GetPosMoveOffset().y;
				
				DesRect.bottom = m_OrgWinRect.bottom - (Int)CurrCam->GetPosMoveOffset().y;

			}
		

			if ((Int)CurrCam->GetPosMoveOffset().x < 0)
			{
				MapUpdateRectH.left = MapUpdateRectH.top = 0;
				MapUpdateRectH.right = -1 * (Int)CurrCam->GetPosMoveOffset().x;
				MapUpdateRectH.bottom = m_OrgWinRect.bottom;

				
				SrcRect.right = m_OrgWinRect.right + (Int)CurrCam->GetPosMoveOffset().x;

				DesRect.left = -1 * (Int)CurrCam->GetPosMoveOffset().x;
				
				
			}
			else if ((Int)CurrCam->GetPosMoveOffset().x > 0)
			{
				MapUpdateRectH.left = m_OrgWinRect.right - (Int)CurrCam->GetPosMoveOffset().x;
				MapUpdateRectH.top = 0;
				MapUpdateRectH.right = m_OrgWinRect.right;
				MapUpdateRectH.bottom = m_OrgWinRect.bottom;

				
				SrcRect.left = (Int)CurrCam->GetPosMoveOffset().x;

				DesRect.right = m_OrgWinRect.right - (Int)CurrCam->GetPosMoveOffset().x;
			}
				

			//UpDateTextureToWindowBuffer(m_CombinedDefaultTexture, SrcRect, DesRect);
			//RestoreDefaultTexture(m_CombinedDefaultTexture, DesRect, DesRect);
			//if (!pMap->m_Updated)
			{
				m_CombinedEntityTexture.m_CurrDisArea.left += (Int)CurrCam->GetPosMoveOffset().x;
				m_CombinedEntityTexture.m_CurrDisArea.right += (Int)CurrCam->GetPosMoveOffset().x;
				m_CombinedEntityTexture.m_CurrDisArea.top += (Int)CurrCam->GetPosMoveOffset().y;
				m_CombinedEntityTexture.m_CurrDisArea.bottom += (Int)CurrCam->GetPosMoveOffset().y;

				if (m_CombinedEntityTexture.m_CurrDisArea.left < 0 ||
					m_CombinedEntityTexture.m_CurrDisArea.right > ExtraMapBuffX + m_CurrWinWidth + ExtraMapBuffX ||
					m_CombinedEntityTexture.m_CurrDisArea.top < 0 ||
					m_CombinedEntityTexture.m_CurrDisArea.bottom > ExtraMapBuffY + m_CurrWinHeight + ExtraMapBuffY)
					AddAllDirtyRect();
				else
				{
					//SetRect(&m_CombinedEntityTexture.m_CurrDisArea, ExtraMapBuffX, ExtraMapBuffY, ExtraMapBuffX + m_CurrWinWidth, ExtraMapBuffY + m_CurrWinHeight);
					m_CombinedEntityTexture.m_DirRectMan.AddDirtyRectToMan(MapUpdateRectV);
					m_CombinedEntityTexture.m_DirRectMan.AddDirtyRectToMan(MapUpdateRectH);
				}
			}

			NightMaskData.m_CurrDisArea.left += (Int)CurrCam->GetPosMoveOffset().x;
			NightMaskData.m_CurrDisArea.right += (Int)CurrCam->GetPosMoveOffset().x;
			NightMaskData.m_CurrDisArea.top += (Int)CurrCam->GetPosMoveOffset().y;
			NightMaskData.m_CurrDisArea.bottom += (Int)CurrCam->GetPosMoveOffset().y;

			if (NightMaskData.m_CurrDisArea.left < 0 ||
				NightMaskData.m_CurrDisArea.right > ExtraMapBuffX + m_CurrWinWidth + ExtraMapBuffX ||
				NightMaskData.m_CurrDisArea.top < 0 ||
				NightMaskData.m_CurrDisArea.bottom > ExtraMapBuffY + m_CurrWinHeight + ExtraMapBuffY)
			{
				SetRect(&NightMaskData.m_CurrDisArea, ExtraMapBuffX, ExtraMapBuffY, m_CurrWinWidth + ExtraMapBuffX
					, m_CurrWinHeight + ExtraMapBuffY);
				NightMaskData.m_DirRectMan.AddDirtyRectToMan(m_OrgWinRect);
				NightMaskData.m_DirRectMan.SetAddAvial(FALSE);
			}
			else
			{
				NightMaskData.m_DirRectMan.AddDirtyRectToMan(MapUpdateRectV);
				NightMaskData.m_DirRectMan.AddDirtyRectToMan(MapUpdateRectH);
			}
		gGlobal.g_DrawAll = TRUE;
	}
}

VOID CRender::BeginSprite()
{
	if (m_pSprite)
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}
VOID CRender::EndSprite()
{
	if (m_pSprite)
	{
		m_pSprite->Flush();
		m_pSprite->End();
	}
}

VOID CRender::BeginScene()
{
	pD3DDevice->BeginScene();

	
}

VOID CRender::EndScene()
{


	pD3DDevice->EndScene();

	pD3DDevice->Present( NULL, NULL, NULL, NULL );		// Display Result

	m_CombinedEntityTexture.m_PreDirtyRegionLst = m_CombinedEntityTexture.m_NextDirtyRegionLst;
	m_CombinedEntityTexture.m_NextDirtyRegionLst.clear();
}


VOID CRender::AddAllDirtyRect()
{
	m_CombinedEntityTexture.m_DirRectMan.AddDirtyRectToMan(m_OrgWinRect);
	gGlobal.g_NightMaskTextureData.m_DirRectMan.AddDirtyRectToMan(m_OrgWinRect);
	m_CombinedEntityTexture.m_DirRectMan.SetAddAvial(FALSE);
	SetRect(&m_CombinedEntityTexture.m_CurrDisArea, ExtraMapBuffX, ExtraMapBuffY, ExtraMapBuffX + m_CurrWinWidth, ExtraMapBuffY + m_CurrWinHeight);
	
	gGlobal.g_DrawAll = TRUE;
}

BOOL CRender::UpDateTextureFromSysToDea()
{
	D3DSURFACE_DESC  pDesc;
	
	pDesc.Width = m_CurrWinWidth;
	pDesc.Height = m_CurrWinHeight;
	
	BOOL Result = TRUE;
	LPDIRECT3DSURFACE9  pDesBuffer;
	pDesBuffer = NULL;
	
	if (!FAILED(pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pDesBuffer)))
	{
		{
			D3DSURFACE_DESC  pDesc;
			m_CombinedEntityTexture.Surface->GetDesc(&pDesc);

			D3DSURFACE_DESC  pDesc2;
			pDesBuffer->GetDesc(&pDesc2);

			{
	
				if (!gGlobal.g_DrawAll)
				{
	
					for (Int i = 0; i < (Int)gGlobal.g_AllDirtyRectMan->GetDirtyRectLst().size(); i++)
					{
						RECT OrgRect = gGlobal.g_AllDirtyRectMan->GetDirtyRectLst()[i];

						if (!QGLMath::CalculateOverlappedRect(OrgRect, m_OrgWinRect, OrgRect))
							continue;
						RECT ShiftRect = OrgRect;
						ShiftRect.left += m_CombinedEntityTexture.m_CurrDisArea.left;
						ShiftRect.right += m_CombinedEntityTexture.m_CurrDisArea.left;
						ShiftRect.top += m_CombinedEntityTexture.m_CurrDisArea.top;
						ShiftRect.bottom += m_CombinedEntityTexture.m_CurrDisArea.top;
						POINT point;
						point.x = OrgRect.left;
						point.y = OrgRect.top;
						pD3DDevice->UpdateSurface(m_CombinedEntityTexture.Surface, &ShiftRect, pDesBuffer, &point);
					}		
				}
				else
				{
					pD3DDevice->UpdateSurface(m_CombinedEntityTexture.Surface, &m_CombinedEntityTexture.m_CurrDisArea, pDesBuffer, NULL);

				}
			}
		}
		pDesBuffer->Release();
	}
	else
		Result = FALSE;

	

	return Result;

}

VOID CRender::UpDateTextureToWindowBuffer(LPDIRECT3DTEXTURE9 SrcTexture, RECT SrcRect, RECT DesRect)
{
	LPDIRECT3DSURFACE9  pDesBuffer;
	pDesBuffer = NULL;
	pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pDesBuffer);
	if (pDesBuffer)
	{
		LPDIRECT3DSURFACE9  pSrcBuffer;
		pSrcBuffer = NULL;
		SrcTexture->GetSurfaceLevel(0, &pSrcBuffer);
		if (pSrcBuffer)
		{
			D3DSURFACE_DESC  pDesc;
			pSrcBuffer->GetDesc(&pDesc);

			D3DSURFACE_DESC  pDesc2;
			pDesBuffer->GetDesc(&pDesc2);


			if (QGLMath::InvalidLock(&SrcRect, pDesc.Width, pDesc.Height) || QGLMath::InvalidLock(&DesRect, pDesc2.Width, pDesc2.Height))
			{



			}
			else
				if (FAILED(pD3DDevice->StretchRect(pSrcBuffer , &SrcRect, pDesBuffer, &DesRect,D3DTEXF_NONE )))
				DebugBreak();
			

			pDesBuffer->Release();
			pSrcBuffer->Release();
		}
		else
			pDesBuffer->Release();
	}
}

/*BOOL CRender::UpDateTextureFromSysToDea()
{
	BOOL Result = FALSE;
	//LPDIRECT3DSURFACE9  pDesBuffer = NULL;
	//pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pDesBuffer);
	//if (pDesBuffer)
	{
		D3DLOCKED_RECT d3dltDes;				
		D3DSURFACE_DESC  pDescDes;
		//pDesBuffer->GetDesc(&pDescDes);
		

		//D3DSURFACE_DESC  pDescSrc;
		//D3DLOCKED_RECT d3dltSrc;

		//if (LockSystemTexture(pDescDes, d3dltDes, NULL, m_CombinedEntityTexture.Texture))
		//if (!FAILED(pDesBuffer->LockRect(&d3dltDes, NULL, D3DLOCK_DISCARD )))
		{
			//memcpy((Byte *)d3dltDes.pBits, m_CombinedEntityTexture.pData, d3dltDes.Pitch * pDescDes.Height);
			Byte * pSrcData = m_CombinedEntityTexture.pData + (m_CombinedEntityTexture.m_CurrDisArea.top * MaxEntSizeX + m_CombinedEntityTexture.m_CurrDisArea.left) * 4;
			Byte * pDesData = (Byte *)d3dltDes.pBits;
			for (Int y = 0; y < (Int)m_CombinedEntityTexture.m_CurrDisArea.bottom - m_CombinedEntityTexture.m_CurrDisArea.top; y++)
			{
				memcpy(pDesData, pSrcData, (m_CombinedEntityTexture.m_CurrDisArea.right - m_CombinedEntityTexture.m_CurrDisArea.left) * 4);
				pSrcData +=  MaxEntSizeX * 4;
				pDesData += d3dltDes.Pitch;
			}
			
			m_CombinedEntityTexture.Texture->UnlockRect(0);
			Result = TRUE;
		}

		//pDesBuffer->UnlockRect();
		//pDesBuffer->Release();
	}

	return Result;
	/*D3DSURFACE_DESC  pDesc;
	pDesc.Width = m_CurrWinWidth;
	pDesc.Height = m_CurrWinHeight;
	
	for (Int i = 0; i < (Int)m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst().size(); i++)
	{
		RECT TempRect = m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i];
		if (QGLMath::InvalidLock(&m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i], pDesc.Width, pDesc.Height))
		{
			continue;
		}

		RECT ShiftRect = m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i];
		ShiftRect.left += m_CombinedEntityTexture.m_CurrDisArea.left;
		ShiftRect.right += m_CombinedEntityTexture.m_CurrDisArea.left;
		ShiftRect.top += m_CombinedEntityTexture.m_CurrDisArea.top;
		ShiftRect.bottom += m_CombinedEntityTexture.m_CurrDisArea.top;

		m_CombinedEntityTexture.Texture->AddDirtyRectToMan(&ShiftRect);
		//DrawTextureBySprite(m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i].left
		//	, m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i].top, ShiftRect, m_CombinedEntityTexture.Texture); 
		//if (FAILED(pD3DDevice->UpdateTexture(m_CombinedEntityTexture.Texture, m_CombinedDefaultTexture)))
		//{
		//	continue;
		//}
		
	}

	DrawTextureBySprite(0, 0, m_CombinedEntityTexture.m_CurrDisArea, m_CombinedEntityTexture.Texture); 
	*/

//}

RECT CRender::FindDirtRectByEntity(CEntityBase * pEntBase)
{
	RECT ResultRect;
	ResultRect.left = ResultRect.right = ResultRect.bottom = ResultRect.top = 0;

	StlMap<CEntityBase *, RECT>::iterator PreDirtyRegionItr =  m_CombinedEntityTexture.m_PreDirtyRegionLst.find(pEntBase);

	if (PreDirtyRegionItr != m_CombinedEntityTexture.m_PreDirtyRegionLst.end())
		return PreDirtyRegionItr->second;
	return ResultRect;
}

RECT CRender::FindNextDirtRectByEntity(CEntityBase * pEntBase)
{
	RECT ResultRect;
	SetRect(&ResultRect, 0, 0, 0, 0);

	StlMap<CEntityBase *, RECT>::iterator NextDirtyRegionItr =  m_CombinedEntityTexture.m_NextDirtyRegionLst.find(pEntBase);

	if (NextDirtyRegionItr != m_CombinedEntityTexture.m_NextDirtyRegionLst.end())
		return NextDirtyRegionItr->second;
	return ResultRect;
}
BOOL CRender::IsEntityInsideTexture(CEntityBase * pEntBase, DrawEntityTextureData & GivenTextureData, PixelPos LeftUp, 
									RECT * ResultRect)
{
	D3DSURFACE_DESC  pDesc;
	GivenTextureData.Texture->GetLevelDesc(0, &pDesc);

	RECT m_EntityTotalRect = pEntBase->m_EntityTotalRect;

	Int EntityLeftUpPosX = pEntBase->GetEntPixelPos().PixelPosX + pEntBase->GetEntOffsetX() + pEntBase->GetShakeX();
	Int EntityLeftUpPosY = pEntBase->GetEntPixelPos().PixelPosY + pEntBase->GetEntOffsetY() + pEntBase->GetShakeX();
	
	pEntBase->SetEntLeftUpPos(EntityLeftUpPosX, EntityLeftUpPosY);

	RECT rect1;
	
	rect1.left = LeftUp.PixelPosX;
	rect1.right = LeftUp.PixelPosX + GivenTextureData.m_CurrDisArea.right - GivenTextureData.m_CurrDisArea.left;
	rect1.top = LeftUp.PixelPosY;
	rect1.bottom = LeftUp.PixelPosY + GivenTextureData.m_CurrDisArea.bottom - GivenTextureData.m_CurrDisArea.top;
	
	RECT rect2;
	RECT resultRect;

	/*rect1.left = 0;
	rect1.top = 0;
	rect1.right = 0 + pDesc.Width ;
	rect1.bottom = 0 + pDesc.Height ;
	*/

	rect2.left = pEntBase->GetEntLeftUpPos().PixelPosX + m_EntityTotalRect.left;
	rect2.top = pEntBase->GetEntLeftUpPos().PixelPosY + m_EntityTotalRect.top;
	rect2.right = rect2.left + m_EntityTotalRect.right - m_EntityTotalRect.left;
	rect2.bottom = rect2.top + m_EntityTotalRect.bottom - m_EntityTotalRect.top;

	if (QGLMath::CalculateOverlappedRect(rect1,rect2, resultRect))
	{
		Int WinLeftUpX = resultRect.left;
		Int WinLeftUpY = resultRect.top;

		RECT LockRect;
		LockRect.left = WinLeftUpX;
		LockRect.top = WinLeftUpY;
		LockRect.right = LockRect.left + resultRect.right - resultRect.left;
		LockRect.bottom = LockRect.top + resultRect.bottom - resultRect.top;
		if (ResultRect)
			*ResultRect = LockRect;
		GivenTextureData.m_NextDirtyRegionLst.insert(std::make_pair(pEntBase, LockRect));
		return TRUE;
	}

	return FALSE;
}

BOOL CRender::IsEntityInsideRenderWindow(CEntityBase * Entity, CCamera * CurrCam, RECT * ResultRect)
{
	if (CurrCam == NULL)
		return FALSE;
	
	if (Entity == NULL)
		return FALSE;

	RECT m_EntityTotalRect = Entity->m_EntityTotalRect;

	Int EntityLeftUpPosX = Entity->GetEntPixelPos().PixelPosX - Entity->m_EntOrgWidth / 2 
		+ Entity->GetEntOffsetX();
	Int EntityLeftUpPosY = 0;
	
	if (Entity->m_EntOrgHeight <= CharPicOffsetY)
	{	
		EntityLeftUpPosY = Entity->GetEntPixelPos().PixelPosY
		+ (-1 * Entity->m_OffsetJumpY + Entity->GetEntOffsetY())  - Entity->m_EntOrgHeight / 2;
	}
	else
	{	
		EntityLeftUpPosY = Entity->GetEntPixelPos().PixelPosY - (Entity->m_EntOrgHeight - CharPicOffsetY)
			+ (-1 * Entity->m_OffsetJumpY + Entity->GetEntOffsetY());
	}

	if (Entity->m_pParentEnt)
	{

		if (Entity->GetOffsetType() == ENTITYOFFSETTYPE_FOOT)
		{
			EntityLeftUpPosY = Entity->GetEntPixelPos().PixelPosY
				+ (-1 * Entity->m_OffsetJumpY + Entity->GetEntOffsetY()) - Entity->m_EntOrgHeight / 2 - AboveHeadOffset;
		}
		else if (Entity->GetOffsetType() == ENTITYOFFSETTYPE_CENTER)
		{
			RECT TotalRect = Entity->m_pParentEnt->GetBattleIdleRect();
			EntityLeftUpPosY = Entity->GetEntPixelPos().PixelPosY
				+ (-1 * Entity->m_OffsetJumpY + Entity->GetEntOffsetY()) - Entity->m_EntOrgHeight / 2
				- (TotalRect.bottom - TotalRect.top) / 2 ;
		}
		else if (Entity->GetOffsetType() == ENTITYOFFSETTYPE_HEAD)
		{
			RECT TotalRect = Entity->m_pParentEnt->GetBattleIdleRect();
			EntityLeftUpPosY = Entity->GetEntPixelPos().PixelPosY
				+ (-1 * Entity->m_OffsetJumpY + Entity->GetEntOffsetY()) - Entity->m_EntOrgHeight / 2
				- ((Entity->m_pParentEnt->m_EntOrgHeight - CharPicOffsetY) - TotalRect.top) - AboveHeadOffset ;
		}
	}

	if (Entity->GetOffsetType() == ENTITYOFFSETTYPE_FULLSCREEN)
	{
		EntityLeftUpPosX = (Int)CurrCam->GetLeftUpPos().x;
		EntityLeftUpPosY = (Int)CurrCam->GetLeftUpPos().y;
	}

	 EntityLeftUpPosX += Entity->GetShakeX();
	 EntityLeftUpPosY += Entity->GetShakeY();

	Entity->SetEntLeftUpPos(EntityLeftUpPosX, EntityLeftUpPosY);

	RECT rect1;
	RECT rect2;
	RECT resultRect;

	rect1.left = (Int)CurrCam->GetLeftUpPos().x;
	rect1.top = (Int)CurrCam->GetLeftUpPos().y;
	rect1.right = (Int)CurrCam->GetLeftUpPos().x + m_CurrWinWidth ;
	rect1.bottom = (Int)CurrCam->GetLeftUpPos().y + m_CurrWinHeight ;

	rect2.left = Entity->GetEntLeftUpPos().PixelPosX + m_EntityTotalRect.left;
	rect2.top = Entity->GetEntLeftUpPos().PixelPosY + m_EntityTotalRect.top;
	rect2.right = rect2.left + m_EntityTotalRect.right - m_EntityTotalRect.left;
	rect2.bottom = rect2.top + m_EntityTotalRect.bottom - m_EntityTotalRect.top;

	if (QGLMath::CalculateOverlappedRect(rect1,rect2, resultRect))
	{
		Int WinLeftUpX = resultRect.left - (Int)CurrCam->GetLeftUpPos().x;
		Int WinLeftUpY = resultRect.top - (Int)CurrCam->GetLeftUpPos().y;

		RECT LockRect;
		LockRect.left = WinLeftUpX;
		LockRect.top = WinLeftUpY;
		LockRect.right = LockRect.left + resultRect.right - resultRect.left;
		LockRect.bottom = LockRect.top + resultRect.bottom - resultRect.top;
		if (ResultRect)
			*ResultRect = LockRect;
		m_CombinedEntityTexture.m_NextDirtyRegionLst.insert(std::make_pair(Entity, LockRect));
		return TRUE;
	}

	return FALSE;
}

//draw the list of entity to the given texture data
VOID CRender::DrawEntityLstToTexture(BOOL DrawAll, StlMultiMap<UInt32, CEntityBase *> & EntRenderLst, 
									 DrawEntityTextureData & GivenTextureData, BOOL Copy)
{
	StlMultiMap<UInt32, CEntityBase *>::iterator EntRenderLstItr;

	EntRenderLstItr = EntRenderLst.begin();

	while (EntRenderLstItr != EntRenderLst.end())
	{
		if (EntRenderLstItr->second->m_Updated 
			|| DrawAll)
		{
			//draw the whole entity
			if (DrawAll || EntRenderLstItr->second->m_OverlappedRegion.size() == 0)
			{

				DrawEntityToTexture(EntRenderLstItr->second, GivenTextureData, NULL, Copy);

			}
			//draw the entity by region
			else
			{
				for (Int i = 0; i < (Int)EntRenderLstItr->second->m_OverlappedRegion.size(); i++)
				{
					DrawEntityToTexture(EntRenderLstItr->second, GivenTextureData, &EntRenderLstItr->second->m_OverlappedRegion[i],
						Copy);
				}
				EntRenderLstItr->second->initOverlappedRect();
			}
		}

		EntRenderLstItr++;
	}
}
VOID CRender::DrawEntityLstToWorld(BOOL DrawAll, StlMultiMap<UInt32, CEntityBase *> & EntRenderLst, CMapObjectView * pMap, 
								   CCamera * CurrCam, CEntityBase * pMaskEnt, BOOL AddEffect)
{	
	if (CurrCam == NULL)
		return;

	m_CurPosEntity = NULL;
	StlMultiMap<UInt32, CEntityBase *>::iterator EntRenderLstItr;

	//copy all background first
	StlVector<RECT> DirtyList = m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst();
	//for (


		
	//for (size_t i = 0; i < DirtyList.size(); i++)
	{
		EntRenderLstItr = EntRenderLst.begin();
		while (EntRenderLstItr != EntRenderLst.end())
		{
			//don't draw mask entity here
			if (pMaskEnt == EntRenderLstItr->second)
			{
				EntRenderLstItr++;
				continue;
			}
			//if (!EntRenderLstItr->second->GetIsMapObject())
			{
				//DrawEntityToWorld(EntRenderLstItr->second, pSelectedEnt,
				//&DirtyList[i], pMap, CurrCam);

				if (EntRenderLstItr->second->m_Updated 
					|| DrawAll)
				{
					//draw the whole entity
					if (DrawAll || EntRenderLstItr->second->m_OverlappedRegion.size() == 0)
					{

						DrawEntityToWorld(EntRenderLstItr->second, 
							NULL, pMap, CurrCam, AddEffect);

					}
					//draw the entity by region
					else
					{
						for (Int i = 0; i < (Int)EntRenderLstItr->second->m_OverlappedRegion.size(); i++)
						{
							DrawEntityToWorld(EntRenderLstItr->second, 
								&EntRenderLstItr->second->m_OverlappedRegion[i], pMap, CurrCam, AddEffect);
						}
						EntRenderLstItr->second->initOverlappedRect();
					}
				}

			}
			EntRenderLstItr++;
		}
	}
}
VOID CRender::DrawTextureBySprite(LPDIRECT3DTEXTURE9 GivenTexture, Byte AlphaMask)
{
	if (GivenTexture)
	{
		if (m_pSprite)
		{
			RECT TextRect;
			D3DSURFACE_DESC  pDesc;
			GivenTexture->GetLevelDesc(0, &pDesc);
			SetRect(&TextRect, 0, 0, pDesc.Width, pDesc.Height);
			RECT ResultRect;

			if (QGLMath::CalculateOverlappedRect(m_OrgWinRect, TextRect, ResultRect))
			{
				if (AlphaMask == 255)
					m_pSprite->Draw(GivenTexture, &ResultRect, NULL, NULL,0xFFFFFFFF);
				else
				{
					UInt32 ColorMask = AlphaMask << 24;
					ColorMask += 0x00FFFFFF;
					m_pSprite->Draw(GivenTexture, &ResultRect, NULL, NULL,ColorMask);
				}
			}
		}
	}
}


VOID CRender::DrawTextureBySprite(int X, int Y, LPDIRECT3DTEXTURE9 GivenTexture, Byte AlphaMask)
{
	D3DXVECTOR3 LeftUpPos;

	LeftUpPos.x = (float)X;
	LeftUpPos.y = (float)Y;
	LeftUpPos.z = 0;
	if (m_pSprite)
	{
		RECT TextRect;
		D3DSURFACE_DESC  pDesc;
		GivenTexture->GetLevelDesc(0, &pDesc);
		SetRect(&TextRect, X, Y, pDesc.Width + X, pDesc.Height + Y);
		RECT ResultRect;
		if (QGLMath::CalculateOverlappedRect(m_OrgWinRect, TextRect, ResultRect))
		{
			ResultRect.left -= X;
			ResultRect.right -= X;
			ResultRect.top -= Y;
			ResultRect.bottom -= Y;
			if (AlphaMask == 255)
				m_pSprite->Draw(GivenTexture, &ResultRect, NULL, &LeftUpPos,0xFFFFFFFF);
			else
			{
				UInt32 ColorMask = AlphaMask << 24;
				ColorMask += 0x00FFFFFF;
				m_pSprite->Draw(GivenTexture, &ResultRect, NULL, &LeftUpPos,ColorMask);
			}
		}
	}
}

VOID CRender::DrawTextureBySprite(int X, int Y, RECT UpdateRect, LPDIRECT3DTEXTURE9 GivenTexture, Byte AlphaMask)
{
	D3DXVECTOR3 LeftUpPos;

	LeftUpPos.x = (float)X;
	LeftUpPos.y = (float)Y;
	LeftUpPos.z = 0;
	if (m_pSprite)
	{
		D3DSURFACE_DESC  pDesc;
		GivenTexture->GetLevelDesc(0, &pDesc);
		
		if (UpdateRect.right > (LONG)pDesc.Width)
			UpdateRect.right = pDesc.Width;
		if (UpdateRect.bottom > (LONG)pDesc.Height)
			UpdateRect.bottom = pDesc.Height;

		UpdateRect.left += X;
		UpdateRect.right += X;
		UpdateRect.top += Y;
		UpdateRect.bottom += Y;
		RECT WinRect = m_OrgWinRect;
		WinRect.left += UpdateRect.left;
		WinRect.right += UpdateRect.left;
		WinRect.top += UpdateRect.top;
		WinRect.bottom += UpdateRect.top;
		RECT ResultRect;
		if (QGLMath::CalculateOverlappedRect(WinRect, UpdateRect, ResultRect))
		{
			ResultRect.left -= X;
			ResultRect.right -= X;
			ResultRect.top -= Y;
			ResultRect.bottom -= Y;

			if (AlphaMask == 255)
				m_pSprite->Draw(GivenTexture, &ResultRect, NULL, &LeftUpPos,0xFFFFFFFF);
			else
			{
				UInt32 ColorMask = AlphaMask << 24;
				ColorMask += 0x00FFFFFF;
				m_pSprite->Draw(GivenTexture, &ResultRect, NULL, &LeftUpPos,ColorMask);
			}
		}
	}
}
VOID CRender::DrawTextureBySpriteAndDirtyRect(LPDIRECT3DTEXTURE9 GivenTexture, CDirtyRectManager * pDirRectMan)
{
	if (GivenTexture == NULL || pDirRectMan == NULL)
		return;

	for (Int i = 0; i < (Int)pDirRectMan->GetDirtyRectLst().size(); i++)
	{
		D3DXVECTOR3 LeftUpPos;
		LeftUpPos.x = (float)pDirRectMan->GetDirtyRectLst()[i].left;
		LeftUpPos.y = (float)pDirRectMan->GetDirtyRectLst()[i].top;
		LeftUpPos.z = 0;
		if (m_pSprite)
		{
			GivenTexture->AddDirtyRect(&pDirRectMan->GetDirtyRectLst()[i]);
			m_pSprite->Draw(GivenTexture, &pDirRectMan->GetDirtyRectLst()[i], NULL, &LeftUpPos,0xFFFFFFFF);
		}
	}

}
VOID CRender::DrawGivenTextureByCurrDirtyRect(LPDIRECT3DTEXTURE9 GivenTexture, BOOL AlphaBlend, Byte AlphaOffset)
{
	if (GivenTexture == NULL)
		return;

	for (Int i = 0; i < (Int)m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst().size(); i++)
	{
		if (AlphaBlend)
		{
			//D3DSURFACE_DESC  pDescDes;
			//D3DLOCKED_RECT d3dltDes;
			RECT ShiftRect = m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i];
			ShiftRect.left += m_CombinedEntityTexture.m_CurrDisArea.left;
			ShiftRect.right += m_CombinedEntityTexture.m_CurrDisArea.left;
			ShiftRect.top += m_CombinedEntityTexture.m_CurrDisArea.top;
			ShiftRect.bottom += m_CombinedEntityTexture.m_CurrDisArea.top;

			//if (LockSystemTexture(pDescDes, d3dltDes, &ShiftRect, m_CombinedEntityTexture.Texture))
			{
				D3DSURFACE_DESC  pDescSrc;
				D3DLOCKED_RECT d3dltSrc;
				if (LockSystemTexture(pDescSrc, d3dltSrc, &m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i], GivenTexture))
				{
					Byte * pDataDes = m_CombinedEntityTexture.pData + (ShiftRect.top * MaxEntSizeX + ShiftRect.left) * 4;
					UInt SrcWidth = m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i].right - m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i].left;
					UInt SrcHeight = m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i].bottom - m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i].top;
					BlitPic((Byte *)d3dltSrc.pBits, 0, SrcWidth, SrcHeight, pDescSrc.Width, pDataDes, 0, 
						MaxEntSizeX, FALSE, AlphaOffset,d3dltSrc.Pitch / pDescSrc.Width, 4);
					GivenTexture->UnlockRect(0);
				}

				//m_CombinedEntityTexture.Texture->UnlockRect(0);
			}
		}
		else
		
		{
			RECT ShiftRect = m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i];
			ShiftRect.left += m_CombinedEntityTexture.m_CurrDisArea.left;
			ShiftRect.right += m_CombinedEntityTexture.m_CurrDisArea.left;
			ShiftRect.top += m_CombinedEntityTexture.m_CurrDisArea.top;
			ShiftRect.bottom += m_CombinedEntityTexture.m_CurrDisArea.top;

			//CopyTexture(GivenTexture, m_CombinedEntityTexture.Texture, m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i], 
			//	ShiftRect);
			CopyTexture(GivenTexture, m_CombinedEntityTexture.pData, MaxEntSizeX, MaxEntSizeY, 4, 
				m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i], 
				ShiftRect);
		}
	}
}

VOID CRender::DrawBgMapByCurrDirtyRect(CMapObjectView * pMap, CCamera * CurrCam)
{
	for (Int i = 0; i < (Int)m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst().size(); i++)
		DrawBgMap(pMap, m_CombinedEntityTexture.m_DirRectMan.GetDirtyRectLst()[i], FALSE, CurrCam);
}

VOID CRender::AddDirtyRectToMan(RECT GivenRect)
{
	m_CombinedEntityTexture.m_DirRectMan.AddDirtyRectToMan(GivenRect);
}

VOID CRender::AddEntityDirtyRect(StlMultiMap<UInt32, CEntityBase *> & EntRenderLst, CEntityBase * pMaskEnt, DrawEntityTextureData
		& TextureData)
{

	StlMultiMap<UInt32, CEntityBase *>::iterator EntRenderLstItr;
	EntRenderLstItr = EntRenderLst.begin();
	while (EntRenderLstItr != EntRenderLst.end())
	{
		EntRenderLstItr->second->initOverlappedRect();
		StlMap<CEntityBase *, RECT>::iterator PreDirRectItr = TextureData.m_PreDirtyRegionLst.find(EntRenderLstItr->second);
		if (EntRenderLstItr->second->m_Updated)
		{
			if (PreDirRectItr != TextureData.m_PreDirtyRegionLst.end())
			{
				TextureData.m_DirRectMan.AddDirtyRectToMan(PreDirRectItr->second);
			}

			StlMap<CEntityBase *, RECT>::iterator NextDirRectItr = TextureData.m_NextDirtyRegionLst.find(EntRenderLstItr->second);
			if (NextDirRectItr != TextureData.m_NextDirtyRegionLst.end())
				TextureData.m_DirRectMan.AddDirtyRectToMan(NextDirRectItr->second);
		}

		if (PreDirRectItr != TextureData.m_PreDirtyRegionLst.end())
			TextureData.m_PreDirtyRegionLst.erase(PreDirRectItr);
		EntRenderLstItr ++;
	}

	//clear other odd dirty rect
	StlMap<CEntityBase *, RECT>::iterator PreDirRectItr = TextureData.m_PreDirtyRegionLst.begin();
	while (PreDirRectItr != TextureData.m_PreDirtyRegionLst.end())
	{
		if (PreDirRectItr->first != pMaskEnt)
		{
			//find total rect of this object and delete it 
			TextureData.m_DirRectMan.AddDirtyRectToMan(PreDirRectItr->second);
		}
		PreDirRectItr++;
	}

	EntRenderLstItr = EntRenderLst.begin();

	while (EntRenderLstItr != EntRenderLst.end())
	{
		if (!EntRenderLstItr->second->m_Updated //&& !EntRenderLstItr->second->GetIsMapObject()
			&& EntRenderLstItr->second != pMaskEnt)
		{
			StlMap<CEntityBase *, RECT>::iterator NextDirRectItr = TextureData.m_NextDirtyRegionLst.find(EntRenderLstItr->second);
			if (NextDirRectItr != TextureData.m_NextDirtyRegionLst.end())
			{
				if (TextureData.m_DirRectMan.OverlappedWithDirtyRect(NextDirRectItr->second,
					EntRenderLstItr->second->m_OverlappedRegion))
				{
					EntRenderLstItr->second->m_Updated = TRUE;
				}
			}
		}

		EntRenderLstItr ++;
	}
	
}

VOID CRender::DXT3To32BitsAlphaMask(Byte * pDXT3Data, Byte * pDesData, RECT CopyRect, UInt32 DesWidth, Byte * pMaskData)
{
	DDsHeader Header; 
	memcpy(&Header, pDXT3Data, sizeof(DDsHeader));

	DWORD DesPitch = DesWidth * 4;
	DWORD SrcPitch = Header.SurHeader.dwWidth * 4;

	DWORD White = 0xFFFFFFFF;
	DWORD Black = 0;

	Int Count = 0;

	//BYTE TempData[4];
	pDXT3Data += sizeof(DDsHeader);
	DWORD	Color0, Color1;
	BYTE	*pColor0 = (BYTE *)(&Color0);
	BYTE	*pColor1 = (BYTE *)(&Color1);

	//move the DDS Data
	pDXT3Data += CopyRect.top / 4 * SrcPitch + (CopyRect.left / 4) * 16;

	Int PreSkipX = CopyRect.left % 4;
	Int PreSkipY = CopyRect.top % 4;

	Int PostSkipX = CopyRect.right % 4;
	if (PostSkipX == 0)
		PostSkipX = 4;
	Int PostSkipY = CopyRect.bottom % 4;
	if (PostSkipY == 0)
		PostSkipY = 4;


	for (Int y = - PreSkipY ; y < CopyRect.bottom - CopyRect.top ; y += 4)
	{
		Byte * pSubDesData = pDesData;
		Byte * pSubMaskData = pMaskData;
		Byte * pSubDXT3Data = pDXT3Data;

		Int		Startj = (y == - PreSkipY ? PreSkipY : 0);
		Int		Endj = (y >= CopyRect.bottom - CopyRect.top - 4 ? PostSkipY : 4);

		Int TStartj = max(0, Startj);
		Int TEndj = min(4, Endj);
		Int	Offset_j = TEndj - TStartj;
		
		for (Int x = - PreSkipX; x < CopyRect.right - CopyRect.left ; x += 4)
		{
			Int		Starti =  (x == - PreSkipX ? PreSkipX : 0);
			Int		Endi = (x >= CopyRect.right - CopyRect.left - 4 ? PostSkipX : 4);				

			Int TStarti = max(0, Starti);
			Int TEndi = min(4, Endi);
			Int	Offset_i = TEndi - TStarti;

			//read the alpha data first
			ULONGLONG AlphaData = *(ULONGLONG *)pSubDXT3Data;

			pSubDXT3Data += sizeof(ULONGLONG);
				

			/*
			Color0[ 2] = (*(WORD *)pSubDXT3Data & 0xF800) >> 8; //R
			Color0[ 1] = (*(WORD *)pSubDXT3Data & 0x7E0) >> 3; //G
			Color0[0] = (*(WORD *)pSubDXT3Data & 0x1F) << 3; //B


			Color1[ 2] = (*(WORD *)(pSubDXT3Data + 2) & 0xF800) >> 8; //R
			Color1[ 1] = (*(WORD *)(pSubDXT3Data + 2) & 0x7E0) >> 3; //G
			Color1[0] = (*(WORD *)(pSubDXT3Data + 2) & 0x1F) << 3; //B
			*/

			__asm
			{
				pushad

				mov			esi, pSubDXT3Data
				xor			eax, eax
				lodsw
				mov			ebx, 0xFF000000

				mov			edx, eax
				and			edx, 0xF800
				shl			edx, 8
				or			ebx, edx

				mov			edx, eax
				and			edx, 0x7E0
				shl			edx, 5
				or			ebx, edx

				and			eax, 0x1F
				shl			eax, 3
				or			ebx, eax

				mov			Color0, ebx

				xor			eax, eax
				lodsw
				mov			ebx, 0xFF000000

				mov			edx, eax
				and			edx, 0xF800
				shl			edx, 8
				or			ebx, edx

				mov			edx, eax
				and			edx, 0x7E0
				shl			edx, 5
				or			ebx, edx

				and			eax, 0x1F
				shl			eax, 3
				or			ebx, eax

				mov			Color1, ebx

				popad
			}


			DWORD PicData = *(DWORD *)(pSubDXT3Data + 4);

			BYTE * pWorkingDesData = pSubDesData;
			BYTE * pWorkingMaskData = pSubMaskData;

			Byte256X256 *pDDSTable = (*(WORD *)pSubDXT3Data > *(WORD *)(pSubDXT3Data + 2) ? gGlobal.BgDDSTable1 : gGlobal.BgDDSTable2);

			/*
			for (Int j = 0; j < 4; j++)
			{
				for (Int i = 0; i < 4; i++)
				*/
			for (Int j = TStartj; j < TEndj; ++j)
			{
				BYTE	*TDesData = pWorkingDesData;
				for (Int i = Starti; i < Endi; ++i)
				{

					/*
					if (i >= Starti && i < Endi &&
						j >= Startj && j < Endj)
						*/
					{
						//DWORD BitCode = PicData & 0x03;	
						DWORD BitCode = (PicData >> (j * 8 + i * 2)) & 0x03;
						ULONGLONG	AlphaCode = (AlphaData >> (j * 16 + i * 4));

						*TDesData++ = pDDSTable[BitCode][pColor0[0]][pColor1[0]];
						*TDesData++ = pDDSTable[BitCode][pColor0[1]][pColor1[1]];
						*TDesData++ = pDDSTable[BitCode][pColor0[2]][pColor1[2]];
						*TDesData = (BYTE)(AlphaData & 0x0F) << 4;

						*TDesData++ = gGlobal.SubtractiveTable[*TDesData][pWorkingMaskData[3]];
						
						*(UInt32 *)&pWorkingDesData[(i - Starti) * 4] = MMXAlpha(*((UInt32 *) 
										(*TDesData++)), 
										(UInt32)pWorkingDesData[(i - Starti) * 4]);

					}
					pWorkingDesData += DesPitch;

					/*
					PicData = PicData >> 2;
					AlphaData = AlphaData >> 4;
					*/

				}
				//if (j < Endj && j >= Startj)
				{
					pWorkingDesData += DesPitch;
					pWorkingMaskData += DesPitch;
				}
			}

			pSubDXT3Data += 8;
			pSubDesData += (4 - Starti) * 4;
			pSubMaskData += (4 - Starti) * 4;
		}
		pMaskData += (4 - Startj) * DesPitch;
		pDesData += (4 - Startj) * DesPitch;
		pDXT3Data += SrcPitch;
	}
}

VOID CRender::DXT3To32Bits(Byte * pDXT3Data, Byte * pDesData, RECT CopyRect, UInt32 DesWidth)
{
	DDsHeader Header; 
	memcpy(&Header, pDXT3Data, sizeof(DDsHeader));

	DWORD DesPitch = DesWidth * 4;
	DWORD SrcPitch = Header.SurHeader.dwWidth * 4;

	DWORD White = 0xFFFFFFFF;
	DWORD Black = 0;

	Int Count = 0;

	pDXT3Data += sizeof(DDsHeader);
	DWORD	Color0, Color1;
	BYTE	*pColor0 = (BYTE *)(&Color0);
	BYTE	*pColor1 = (BYTE *)(&Color1);

	//move the DDS Data
	pDXT3Data += CopyRect.top / 4 * SrcPitch + (CopyRect.left / 4) * 16;

	Int PreSkipX = CopyRect.left % 4;
	Int PreSkipY = CopyRect.top % 4;

	Int PostSkipX = CopyRect.right % 4;
	if (PostSkipX == 0)
		PostSkipX = 4;
	Int PostSkipY = CopyRect.bottom % 4;
	if (PostSkipY == 0)
		PostSkipY = 4;


	for (Int y = - PreSkipY ; y < CopyRect.bottom - CopyRect.top ; y += 4)
	{
		Byte * pSubDesData = pDesData;
		Byte * pSubDXT3Data = pDXT3Data;

		Int		Startj = (y == - PreSkipY ? PreSkipY : 0);
		Int		Endj = (y >= CopyRect.bottom - CopyRect.top - 4 ? PostSkipY : 4);

		Int TStartj = max(0, Startj);
		Int TEndj = min(4, Endj);
		Int	Offset_j = TEndj - TStartj;
		
		for (Int x = - PreSkipX; x < CopyRect.right - CopyRect.left ; x += 4)
		{
			Int		Starti =  (x == - PreSkipX ? PreSkipX : 0);
			Int		Endi = (x >= CopyRect.right - CopyRect.left - 4 ? PostSkipX : 4);						


			Int TStarti = max(0, Starti);
			Int TEndi = min(4, Endi);
			Int	Offset_i = TEndi - TStarti;

			//read the alpha data first
			ULONGLONG AlphaData = *(ULONGLONG *)pSubDXT3Data;

			pSubDXT3Data += sizeof(ULONGLONG);
			if (x == - PreSkipX)
				Starti = PreSkipX;
			else
				Starti = 0;

			if (x >= CopyRect.right - CopyRect.left - 4)
				Endi = PostSkipX;
			else
				Endi = 4;						
				
			/*
			Color0[ 2] = (*(WORD *)pSubDXT3Data & 0xF800) >> 8; //R
			Color0[ 1] = (*(WORD *)pSubDXT3Data & 0x7E0) >> 3; //G
			Color0[0] = (*(WORD *)pSubDXT3Data & 0x1F) << 3; //B


			Color1[ 2] = (*(WORD *)(pSubDXT3Data + 2) & 0xF800) >> 8; //R
			Color1[ 1] = (*(WORD *)(pSubDXT3Data + 2) & 0x7E0) >> 3; //G
			Color1[0] = (*(WORD *)(pSubDXT3Data + 2) & 0x1F) << 3; //B
			*/

			__asm
			{
				pushad

				mov			esi, pSubDXT3Data
				xor			eax, eax
				lodsw
				mov			ebx, 0xFF000000

				mov			edx, eax
				and			edx, 0xF800
				shl			edx, 8
				or			ebx, edx

				mov			edx, eax
				and			edx, 0x7E0
				shl			edx, 5
				or			ebx, edx

				and			eax, 0x1F
				shl			eax, 3
				or			ebx, eax

				mov			Color0, ebx

				xor			eax, eax
				lodsw
				mov			ebx, 0xFF000000

				mov			edx, eax
				and			edx, 0xF800
				shl			edx, 8
				or			ebx, edx

				mov			edx, eax
				and			edx, 0x7E0
				shl			edx, 5
				or			ebx, edx

				and			eax, 0x1F
				shl			eax, 3
				or			ebx, eax

				mov			Color1, ebx

				popad
			}


			DWORD PicData = *(DWORD *)(pSubDXT3Data + 4);
			BYTE * pWorkingDesData = pSubDesData;
			Byte256X256 *pDDSTable = (*(WORD *)pSubDXT3Data > *(WORD *)(pSubDXT3Data + 2) ? gGlobal.BgDDSTable1 : gGlobal.BgDDSTable2);

			/*
			for (Int j = TStartj; j < TEndj; ++j)
			{
				BYTE	*TDesData = pWorkingDesData;
				for (Int i = Starti; i < Endi; ++i)
				{
					DWORD BitCode = (PicData >> (j * 8 + i * 2)) & 0x03;
					ULONGLONG	AlphaCode = (AlphaData >> (j * 16 + i * 4));

					*TDesData++ = pDDSTable[BitCode][pColor0[0]][pColor1[0]];
					*TDesData++ = pDDSTable[BitCode][pColor0[1]][pColor1[1]];
					*TDesData++ = pDDSTable[BitCode][pColor0[2]][pColor1[2]];
					*TDesData++ = (BYTE)(AlphaCode & 0x0F) << 4;
				}
				pWorkingDesData += DesPitch;
			}
			*/


			Int		i, j;

			__asm
			{
				pushad

				mov			eax, Startj
				mov			j, eax
				mov			ecx, Offset_j
Label_L2_1:
				push		ecx
				


				mov			eax, Starti
				mov			i, eax
				mov			edi, pWorkingDesData
				mov			esi, pDDSTable

				mov			ecx, Offset_i
Label_L2_2:
				push		ecx
				
				
				mov			ebx, PicData

				mov			ecx, j
				shl			ecx, 2
				add			ecx, i
				shl			ecx, 1
				
				shr			ebx, cl
				and			ebx, 3
				shl			ebx, 16

				movq		mm2, AlphaData
				shl			ecx, 1
				movd		mm3, ecx
				PSRLQ		mm2, mm3
				

				mov			eax, Color0
				movd		mm1, eax
				mov			eax, Color1
				movd		mm0, eax
				PUNPCKLBW	mm0, mm1

				movd		eax, mm0
				mov			bx, ax
				mov			al, [esi][ebx]
				stosb
				
				PSRLQ		mm0, 16
				movd		eax, mm0
				mov			bx, ax
				mov			al, [esi][ebx]
				stosb

				PSRLQ		mm0, 16
				movd		eax, mm0
				mov			bx, ax
				mov			al, [esi][ebx]
				stosb

				movd		eax, mm2
				and			eax, 0x0F
				shl			eax, 4
				stosb
			
				
				inc			i
				pop			ecx
				loop		Label_L2_2
				
				mov			eax, DesPitch
				add			pWorkingDesData, eax;

				inc			j
				pop			ecx
				dec			ecx
				jecxz		L2_1_END
				jmp			Label_L2_1
L2_1_END:
				popad
				emms
			}
			
			pSubDXT3Data += 8;
			pSubDesData += (4 - Starti) * 4;
		}
		pDesData += (4 - Startj) * DesPitch;
		pDXT3Data += SrcPitch;
	}
}
VOID CRender::DXT3To32BitsAlpha(Byte * pDXT3Data, Byte * pDesData, RECT CopyRect, UInt32 DesWidth)
{
	DDsHeader Header; 
	memcpy(&Header, pDXT3Data, sizeof(DDsHeader));

	DWORD DesPitch = DesWidth * 4;
	DWORD SrcPitch = Header.SurHeader.dwWidth * 4;

	DWORD White = 0xFFFFFFFF;
	DWORD Black = 0;

	Int Count = 0;

	pDXT3Data += sizeof(DDsHeader);
	BYTE Color0[4];
	Color0[3] = 0xFF;
	BYTE Color1[4];
	Color1[3] = 0xFF;

	BYTE TempData[4];
	//move the DDS Data
	pDXT3Data += CopyRect.top / 4 * SrcPitch + (CopyRect.left / 4) * 16;

	Int PreSkipX = CopyRect.left % 4;
	Int PreSkipY = CopyRect.top % 4;

	Int PostSkipX = CopyRect.right % 4;
	if (PostSkipX == 0)
		PostSkipX = 4;
	Int PostSkipY = CopyRect.bottom % 4;
	if (PostSkipY == 0)
		PostSkipY = 4;

	Int Starti = 0;
	Int Startj = 0;

	Int Endi = 4;
	Int Endj = 4;

	for (Int y = - PreSkipY ; y < CopyRect.bottom - CopyRect.top ; y += 4)
	{
		Byte * pSubDesData = pDesData;
		Byte * pSubDXT3Data = pDXT3Data;
		
		if (y == - PreSkipY)
			Startj = PreSkipY;
		else
			Startj = 0;
		

		if (y >= CopyRect.bottom - CopyRect.top - 4)
			Endj = PostSkipY;
		else
			Endj = 4;
			
			

		for (Int x = - PreSkipX; x < CopyRect.right - CopyRect.left ; x += 4)
		{
			//read the alpha data first
			ULONGLONG AlphaData = *(ULONGLONG *)pSubDXT3Data;

			pSubDXT3Data += sizeof(ULONGLONG);
			if (x == - PreSkipX)
				Starti = PreSkipX;
			else
				Starti = 0;

			if (x >= CopyRect.right - CopyRect.left - 4)
				Endi = PostSkipX;
			else
				Endi = 4;						
				

			Color0[ 2] = (*(WORD *)pSubDXT3Data & 0xF800) >> 8; //R
			Color0[ 1] = (*(WORD *)pSubDXT3Data & 0x7E0) >> 3; //G
			Color0[0] = (*(WORD *)pSubDXT3Data & 0x1F) << 3; //B


			Color1[ 2] = (*(WORD *)(pSubDXT3Data + 2) & 0xF800) >> 8; //R
			Color1[ 1] = (*(WORD *)(pSubDXT3Data + 2) & 0x7E0) >> 3; //G
			Color1[0] = (*(WORD *)(pSubDXT3Data + 2) & 0x1F) << 3; //B

			DWORD PicData = *(DWORD *)(pSubDXT3Data + 4);

			BYTE * pWorkingDesData = pSubDesData;
			if (*(WORD *)pSubDXT3Data > *(WORD *)(pSubDXT3Data + 2))
			{
				for (Int j = 0; j < 4; j++)
				{
					for (Int i = 0; i < 4; i++)
					{
						if (i >= Starti && i < Endi &&
							j >= Startj && j < Endj)
						{
							DWORD BitCode = PicData & 0x03;	
							/*
							pWorkingDesData[(i - Starti) * 4] = gGlobal.BgDDSTable1[BitCode][Color0[0]][Color1[0]];
							pWorkingDesData[(i - Starti) * 4 + 1] = gGlobal.BgDDSTable1[BitCode][Color0[1]][Color1[1]];
							pWorkingDesData[(i - Starti) * 4 + 2] = gGlobal.BgDDSTable1[BitCode][Color0[2]][Color1[2]];
							pWorkingDesData[(i - Starti) * 4 + 3] = (BYTE)(AlphaData & 0x0F) << 4;
							*/
							TempData[(i - Starti) * 4] = gGlobal.BgDDSTable1[BitCode][Color0[0]][Color1[0]];
							TempData[(i - Starti) * 4 + 1] = gGlobal.BgDDSTable1[BitCode][Color0[1]][Color1[1]];
							TempData[(i - Starti) * 4 + 2] = gGlobal.BgDDSTable1[BitCode][Color0[2]][Color1[2]];
							TempData[(i - Starti) * 4 + 3] = (BYTE)(AlphaData & 0x0F) << 4;
							
							*(UInt32 *)&pWorkingDesData[(i - Starti) * 4] = MMXAlpha(*((UInt32 *) 
											(TempData)), 
											(UInt32)pWorkingDesData[(i - Starti) * 4]);

						}
						PicData = PicData >> 2;
						AlphaData = AlphaData >> 4;

					}
					if (j < Endj && j >= Startj)
						pWorkingDesData += DesPitch;
				}
			}
			else
			{
				for (Int j = 0; j < 4; j++)
				{
					for (Int i = 0; i < 4; i++)
					{
						if (i >= Starti && i < Endi &&
							j >= Startj && j < Endj)
						{
							DWORD BitCode = PicData & 0x03;	
							/*pWorkingDesData[(i - Starti) * 4] = gGlobal.BgDDSTable2[BitCode][Color0[0]][Color1[0]];
							pWorkingDesData[(i - Starti) * 4 + 1] = gGlobal.BgDDSTable2[BitCode][Color0[1]][Color1[1]];
							pWorkingDesData[(i - Starti) * 4 + 2] = gGlobal.BgDDSTable2[BitCode][Color0[2]][Color1[2]];
							pWorkingDesData[(i - Starti) * 4 + 3] = (BYTE)(AlphaData & 0x0F) << 4;
							*/
							TempData[(i - Starti) * 4] = gGlobal.BgDDSTable2[BitCode][Color0[0]][Color1[0]];
							TempData[(i - Starti) * 4 + 1] = gGlobal.BgDDSTable2[BitCode][Color0[1]][Color1[1]];
							TempData[(i - Starti) * 4 + 2] = gGlobal.BgDDSTable2[BitCode][Color0[2]][Color1[2]];
							TempData[(i - Starti) * 4 + 3] = (BYTE)(AlphaData & 0x0F) << 4;

							* (UInt32 *) &pWorkingDesData[(i - Starti) * 4] = MMXAlpha(*((UInt32 *) 
											(TempData)), 
											(UInt32)pWorkingDesData[(i - Starti) * 4]);

						}
						PicData = PicData >> 2;
						AlphaData = AlphaData >> 4;

					}
					if (j < Endj && j >= Startj)
						pWorkingDesData += DesPitch;
				}
			}	
			pSubDXT3Data += 8;
			pSubDesData += (4 - Starti) * 4;
		}
		pDesData += (4 - Startj) * DesPitch;
		pDXT3Data += SrcPitch;
	}
}

BOOL CRender::CreateD3DWindow(BOOL fullscreenflag)
{
	// Create The Window
	if (hWnd == NULL)
	{
		return FALSE;
	}

	UInt32		dwExStyle;				// Window Extended Style
	UInt32		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = 0;
	WindowRect.top = 0;
	WindowRect.right = m_CurrWinWidth;
	WindowRect.bottom = m_CurrWinHeight;

	if (fullscreenflag)												// Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
		GetWindowRect( hWnd, &m_rcWindowBounds );
	}


	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	// Adjust Window To TRUE Requested Size

	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillD3DWindow();								// Reset The Display
		MessageBox(NULL,_T("Can't Create A Device Context."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (pD3D == NULL)
		pD3D = Direct3DCreate9( D3D_SDK_VERSION );		// Check For The Correct DirectX 3D version
	if ( pD3D == NULL )
	{
		KillD3DWindow();							// Reset The Display
		MessageBox(NULL,_T("Can't find D3D SDK Version 9."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	m_PresentPara.BackBufferWidth = m_CurrWinWidth;								// Back Buffer Width
	m_PresentPara.BackBufferHeight = m_CurrWinHeight;										// Back Buffer Height
	m_PresentPara.MultiSampleType = D3DMULTISAMPLE_NONE;						// No Multi Sample Type
	m_PresentPara.MultiSampleQuality = 0;											// No Multi Sample Quality
	m_PresentPara.EnableAutoDepthStencil = FALSE;										// Enable Auto Depth Stencil  
	m_PresentPara.AutoDepthStencilFormat = D3DFMT_D16;									// 16Bit Z-Buffer (Depth Buffer)
	m_PresentPara.Flags = D3DPRESENTFLAG_VIDEO;											// No Flags
	m_PresentPara.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;					// Default Refresh Rate
	//m_PresentPara.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	// Default Presentation Int32erval (vertical sync)
	m_PresentPara.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	m_PresentPara.hDeviceWindow		= hWnd;
	m_FullScnFlag = fullscreenflag;
	m_PresentPara.Windowed			= TRUE;//!fullscreenflag;
	m_PresentPara.SwapEffect		= D3DSWAPEFFECT_DISCARD;
	m_PresentPara.BackBufferCount	= 1;
	m_PresentPara.BackBufferFormat	= D3DFMT_X8R8G8B8;//D3DFMT_X8R8G8B8;

	HRESULT hr;
	
	hr = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &m_PresentPara, &pD3DDevice );
	// Create The DirectX 3D Device 
	if(FAILED(hr ) )
	{
		
				if (hr == D3DERR_DEVICELOST)
					MessageBox(NULL,_T("Can't Create DirectX 3D Device. D3DERR_DEVICELOST"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
				else if (hr == D3DERR_INVALIDCALL)
					MessageBox(NULL,_T("Can't Create DirectX 3D Device. D3DERR_INVALIDCALL"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
				else if (hr == D3DERR_NOTAVAILABLE)
					MessageBox(NULL,_T("Can't Create DirectX 3D Device. D3DERR_NOTAVAILABLE"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
				else if (hr == D3DERR_OUTOFVIDEOMEMORY)
					MessageBox(NULL,_T("Can't Create DirectX 3D Device. D3DERR_OUTOFVIDEOMEMORY"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
				
				KillD3DWindow();							// Reset The Display
				return FALSE;								// Return FALSE
			//}
		//}
	}

	m_pSprite = NULL;

	hr = D3DXCreateSprite(pD3DDevice,&m_pSprite);
	
	if (FAILED(hr))
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("MapView::Create Error D3DXCreateSprite Failed : hr = %d"), hr);
		KillD3DWindow();	
		return (BOOLEAN_FALSE);
	}

	//free all resources
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Video Resources before free %d "), pD3DDevice->GetAvailableTextureMem());
	pD3DDevice->EvictManagedResources();
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Video Resources after free %d "), pD3DDevice->GetAvailableTextureMem());

	/*
	m_PSProfile =  D3DXGetPixelShaderProfile(pD3DDevice);
	if (m_PSProfile == NULL)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("No Pixel Shader support"));
		gConfigure.RenderNightByHardware = FALSE;
	}
	else
		gConfigure.RenderNightByHardware = TRUE;
	*/

	return TRUE;
}

VOID CRender::Init()
{
	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeD3DScene(m_CurrWinWidth, m_CurrWinHeight/2);					// Set Up Our Perspective D3D Screen
	
	if (!InitD3D())									// Initialize Our Newly Created D3D Window
	{
		KillD3DWindow();							// Reset The Display
		MessageBox(NULL,_T("Initialization Failed."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return;								// Return FALSE
	}

	//m_CombinedEntityTexture.pData = CResourcesManager::GetInstance().GetWorldImage();updatesurface
	if (m_CreateBgTexture)
	{
		if (m_CombinedEntityTexture.pData == NULL)
		{
			HRESULT hr;
			hr = pD3DDevice->CreateOffscreenPlainSurface(BgTextureSizeX, BgTextureSizeY, D3DFMT_X8R8G8B8,
				D3DPOOL_SYSTEMMEM, &m_CombinedEntityTexture.Surface, NULL);
			//hr = D3DXCreateTexture(pD3DDevice, BgTextureSizeX, BgTextureSizeY, 0, 0 ,
			//	D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &m_CombinedEntityTexture.Texture);
			if (FAILED(hr))
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Video Resources Not Enough %d Error Code 0x%X"), pD3DDevice->GetAvailableTextureMem(),
					hr);
				if (hr == D3DERR_INVALIDCALL)
				{
					MessageBox(NULL,_T("Initialization Failed. D3DERR_INVALIDCALL"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
					TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Initialization Failed. D3DERR_INVALIDCALL"));
				}
				else if (hr == D3DERR_NOTAVAILABLE)
				{
					MessageBox(NULL,_T("Initialization Failed. D3DERR_NOTAVAILABLE"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
					TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Initialization Failed. D3DERR_NOTAVAILABLE"));
				}
				else if (hr == D3DERR_OUTOFVIDEOMEMORY)
				{
					MessageBox(NULL,_T("Initialization Failed. D3DERR_OUTOFVIDEOMEMORY"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
					TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Initialization Failed. D3DERR_OUTOFVIDEOMEMORY"));
				}
				else if (hr == E_OUTOFMEMORY)
				{
					MessageBox(NULL,_T("Initialization Failed. E_OUTOFMEMORY"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
					TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Initialization Failed. E_OUTOFMEMORY"));
				}
				KillD3DWindow();
			}
			//m_CombinedEntityTexture.pData = SafeAllocate(Byte, BgTextureSizeX * BgTextureSizeY * 4);
			//ZeroMemory(m_CombinedEntityTexture.pData, BgTextureSizeX * BgTextureSizeY * 4);
			SetRect(&m_CombinedEntityTexture.m_CurrDisArea, ExtraMapBuffX, ExtraMapBuffY, ExtraMapBuffX + m_CurrWinWidth, ExtraMapBuffY + m_CurrWinHeight);

		}

		

	}
	
	if(!m_FullScnFlag)
	{
		SetWindowPos( hWnd, HWND_NOTOPMOST,
			m_rcWindowBounds.left, m_rcWindowBounds.top,
			( m_rcWindowBounds.right - m_rcWindowBounds.left ),
			( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
			SWP_SHOWWINDOW );
	}
	
}

VOID CRender::DXT1To32Bits(Byte * pDXT1Data, Byte * pDesData, RECT CopyRect, UInt32 DesWidth)
{
	DDsHeader Header; 
	memcpy(&Header, pDXT1Data, sizeof(DDsHeader));

	DWORD DesPitch = DesWidth * 4;
	DWORD SrcPitch = Header.SurHeader.dwWidth * 2;

	DWORD White = 0xFFFFFFFF;
	DWORD Black = 0;

	Int Count = 0;

	pDXT1Data += sizeof(DDsHeader);
	DWORD	Color0, Color1;
	BYTE	*pColor0 = (BYTE *)(&Color0);
	BYTE	*pColor1 = (BYTE *)(&Color1);

	//pColor0[3] = 0xFF;
	//pColor1[3] = 0xFF;

	//move the DDS Data
	pDXT1Data += CopyRect.top / 4 * SrcPitch + CopyRect.left / 4 * 8;

	Int PreSkipX = CopyRect.left % 4;
	Int PreSkipY = CopyRect.top % 4;

	Int PostSkipX = CopyRect.right % 4;
	if (PostSkipX == 0)
		PostSkipX = 4;
	Int PostSkipY = CopyRect.bottom % 4;
	if (PostSkipY == 0)
		PostSkipY = 4;

	
	for (Int y = - PreSkipY ; y < CopyRect.bottom - CopyRect.top ; y += 4)
	{
		Byte * pSubDesData = pDesData;
		Byte * pSubDXT1Data = pDXT1Data;

		Int		Startj = (y == - PreSkipY ? PreSkipY : 0);
		Int		Endj = (y >= CopyRect.bottom - CopyRect.top - 4 ? PostSkipY : 4);
			
		Int TStartj = max(0, Startj);
		Int TEndj = min(4, Endj);
		Int	Offset_j = TEndj - TStartj;

		for (Int x = - PreSkipX; x < CopyRect.right - CopyRect.left ; x += 4)
		{
			Int		Starti =  (x == - PreSkipX ? PreSkipX : 0);
			Int		Endi = (x >= CopyRect.right - CopyRect.left - 4 ? PostSkipX : 4);						


			Int TStarti = max(0, Starti);
			Int TEndi = min(4, Endi);
			Int	Offset_i = TEndi - TStarti;
				
			/*
			pColor0[ 2] = (*(WORD *)pSubDXT1Data & 0xF800) >> 8; //R
			pColor0[ 1] = (*(WORD *)pSubDXT1Data & 0x7E0) >> 3; //G
			pColor0[0] = (*(WORD *)pSubDXT1Data & 0x1F) << 3; //B


			pColor1[ 2] = (*(WORD *)(pSubDXT1Data + 2) & 0xF800) >> 8; //R
			pColor1[ 1] = (*(WORD *)(pSubDXT1Data + 2) & 0x7E0) >> 3; //G
			pColor1[0] = (*(WORD *)(pSubDXT1Data + 2) & 0x1F) << 3; //B
			*/


			__asm
			{
				pushad

				mov			esi, pSubDXT1Data
				xor			eax, eax
				lodsw
				mov			ebx, 0xFF000000

				mov			edx, eax
				and			edx, 0xF800
				shl			edx, 8
				or			ebx, edx

				mov			edx, eax
				and			edx, 0x7E0
				shl			edx, 5
				or			ebx, edx

				and			eax, 0x1F
				shl			eax, 3
				or			ebx, eax

				mov			Color0, ebx

				xor			eax, eax
				lodsw
				mov			ebx, 0xFF000000

				mov			edx, eax
				and			edx, 0xF800
				shl			edx, 8
				or			ebx, edx

				mov			edx, eax
				and			edx, 0x7E0
				shl			edx, 5
				or			ebx, edx

				and			eax, 0x1F
				shl			eax, 3
				or			ebx, eax

				mov			Color1, ebx

				popad
			}
			

			DWORD PicData = *(DWORD *)(pSubDXT1Data + 4);
			BYTE * pWorkingDesData = pSubDesData;
			Byte256X256 *pDDSTable = (*(WORD *)pSubDXT1Data > *(WORD *)(pSubDXT1Data + 2) ? gGlobal.BgDDSTable1 : gGlobal.BgDDSTable2);

			/*
			for (Int j = TStartj; j < TEndj; ++j)
			{
				BYTE	*TDesData = pWorkingDesData;
				for (Int i = Starti; i < Endi; ++i)
				{
					
					DWORD BitCode = (PicData >> (j * 8 + i * 2)) & 0x03;	

					*TDesData++ = pDDSTable[BitCode][pColor0[0]][pColor1[0]];
					*TDesData++ = pDDSTable[BitCode][pColor0[1]][pColor1[1]];
					*TDesData++ = pDDSTable[BitCode][pColor0[2]][pColor1[2]];
					*TDesData++ = 0xFF;
				}
				pWorkingDesData += DesPitch;

			}
			*/

			Int		i, j;

			__asm
			{
				pushad

				mov			eax, Startj
				mov			j, eax
				mov			ecx, Offset_j
Label_L2_1:
				push		ecx
				


				mov			eax, Starti
				mov			i, eax
				mov			edi, pWorkingDesData
				mov			esi, pDDSTable

				mov			ecx, Offset_i
Label_L2_2:
				push		ecx
				
				
				mov			ebx, PicData

				mov			ecx, j
				shl			ecx, 2
				add			ecx, i
				shl			ecx, 1
				
				shr			ebx, cl
				and			ebx, 3
				shl			ebx, 16

				mov			eax, Color0
				movd		mm1, eax
				mov			eax, Color1
				movd		mm0, eax
				PUNPCKLBW	mm0, mm1

				movd		eax, mm0
				mov			bx, ax
				mov			al, [esi][ebx]
				stosb
				
				PSRLQ		mm0, 16
				movd		eax, mm0
				mov			bx, ax
				mov			al, [esi][ebx]
				stosb

				PSRLQ		mm0, 16
				movd		eax, mm0
				mov			bx, ax
				mov			al, [esi][ebx]
				stosb

				mov			al,0xFF
				stosb
			
				
				inc			i
				pop			ecx
				loop		Label_L2_2
				
				mov			eax, DesPitch
				add			pWorkingDesData, eax;

				inc			j
				pop			ecx
				dec			ecx
				jecxz		L2_1_END
				jmp			Label_L2_1
L2_1_END:
				popad
				emms
			}
			

			pSubDXT1Data += 8;
			pSubDesData += (4 - Starti) * 4;
		}
		pDesData += (4 - Startj) * DesPitch;
		pDXT1Data += SrcPitch;
	}
}
VOID CRender::UpdateWorldDDSToBG(Byte * pData, RECT SrcUpdateRect,  RECT LockRegion, UInt SrcMaxWidth)
{
	RECT ShiftRect = LockRegion;
	ShiftRect.left += m_CombinedEntityTexture.m_CurrDisArea.left;
	ShiftRect.right += m_CombinedEntityTexture.m_CurrDisArea.left;
	ShiftRect.top += m_CombinedEntityTexture.m_CurrDisArea.top;
	ShiftRect.bottom += m_CombinedEntityTexture.m_CurrDisArea.top;
	//if (!LockSystemTexture(pDesc2, d3dlt2, &ShiftRect, m_CombinedEntityTexture.Texture))
	//	return;
	
	Byte * pDesTData = m_CombinedEntityTexture.pData + (ShiftRect.top * BgTextureSizeX + ShiftRect.left) * 4;

	if (gConfigure.uCompressedMap)
	{
		DXT1To32Bits(pData, pDesTData, SrcUpdateRect, BgTextureSizeX);
	}
	else
	{
		Byte * pSrcData = pData + (SrcUpdateRect.top * SrcMaxWidth + SrcUpdateRect.left) * 4;

		Copy8888To8888(pSrcData, 0, (UInt16)(SrcUpdateRect.right - SrcUpdateRect.left), 
			(UInt16)(SrcUpdateRect.bottom - SrcUpdateRect.top ),SrcMaxWidth,
			pDesTData, 0, MaxEntSizeX);
	}
		
	

}

VOID CRender::UpdateWorldTextureBG(LPDIRECT3DTEXTURE9 SrcTexture, RECT SrcUpdateRect, 
		Int SrcMaxWidth, RECT LockRegion)
{

	//D3DSURFACE_DESC  pDesc2;

	//D3DLOCKED_RECT d3dlt2;

	RECT ShiftRect = LockRegion;
	ShiftRect.left += m_CombinedEntityTexture.m_CurrDisArea.left;
	ShiftRect.right += m_CombinedEntityTexture.m_CurrDisArea.left;
	ShiftRect.top += m_CombinedEntityTexture.m_CurrDisArea.top;
	ShiftRect.bottom += m_CombinedEntityTexture.m_CurrDisArea.top;
	//if (!LockSystemTexture(pDesc2, d3dlt2, &ShiftRect, m_CombinedEntityTexture.Texture))
	//	return;
	
	Byte * pDesTData = m_CombinedEntityTexture.pData + (ShiftRect.top * MaxEntSizeX + ShiftRect.left) * 4;

	D3DLOCKED_RECT d3dlt;
	D3DSURFACE_DESC  pDesc;
	if(!LockSystemTexture(pDesc, d3dlt,&SrcUpdateRect, SrcTexture))
	{
		//m_CombinedEntityTexture.Texture->UnlockRect(0);
		return;
	}
	Byte * pSrcTData = (Byte *)d3dlt.pBits;

	Copy8888To8888(pSrcTData, 0, (UInt16)(SrcUpdateRect.right - SrcUpdateRect.left), 
		(UInt16)(SrcUpdateRect.bottom - SrcUpdateRect.top ),SrcMaxWidth,
		pDesTData, 0, MaxEntSizeX);
		

	/*Copy565To8888(pSrcTData, 0, (UInt16)(SrcUpdateRect.right - SrcUpdateRect.left), 
		(UInt16)(SrcUpdateRect.bottom - SrcUpdateRect.top ),SrcMaxWidth,
		pDesTData, 0, MaxEntSizeX);*/
		
		
	/*BlitPic(pSrcTData, 0, (UInt16)(SrcUpdateRect.right - SrcUpdateRect.left), 
		(UInt16)(SrcUpdateRect.bottom - SrcUpdateRect.top ),SrcMaxWidth,
		pDesTData, 0, (UInt16)pDesc2.Width, FALSE, RGB565,  0, sizeof(Color16), sizeof(Color32));
		*/

	//m_CombinedEntityTexture.Texture->UnlockRect(0);
	SrcTexture->UnlockRect(0);
}


VOID CRender::KillD3DWindow()									// Properly Kill The Window
{
	KillD3DScene();										// Release D3D Scene
	

	SafeRelease(m_pSprite);
	SafeRelease(m_CombinedEntityTexture.Texture);
	SafeRelease(pD3DDevice);
	SafeRelease(pD3D);
	    
	if (!m_PresentPara.Windowed)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse PoInt32er
	}

}

VOID CRender::KillD3DScene()										// Properly Kill D3D Scene
{
	// Nothing here to unload.
}

VOID CRender::CopyTexture(LPDIRECT3DTEXTURE9 pDataSrc, Byte * pDataDes, Int DesMaxWidth, Int DesMaxHeight, 
						  Int DesPixelSize, RECT SrcCopyRect, RECT DesCopyRect)
{
	//copy The background data first
	if (QGLMath::InvalidLock(&DesCopyRect, DesMaxWidth, DesMaxHeight))
		return;

	Byte * pDesTData2 = pDataDes + (DesCopyRect.top * DesMaxWidth + DesCopyRect.left) * DesPixelSize;

	D3DSURFACE_DESC  pDescSrc;
	D3DLOCKED_RECT d3dlt2;

	if(!LockSystemTexture(pDescSrc, d3dlt2, &SrcCopyRect, pDataSrc))
	{
		return;
	}

	Byte * pSrcTData2 = (Byte *)d3dlt2.pBits;

	//copy the texture line by line
	UInt32 CopyLen = (SrcCopyRect.right - SrcCopyRect.left ) * sizeof(Color32);

	for (Int y = 0; y < SrcCopyRect.bottom - SrcCopyRect.top; y++)
	{
		memcpy(pDesTData2, pSrcTData2, CopyLen);
		pDesTData2 += DesMaxWidth * sizeof(Color32);
		pSrcTData2 += pDescSrc.Width * sizeof(Color32);
	}
	
	pDataSrc->UnlockRect(0);
}

VOID CRender::CopyTexture(LPDIRECT3DTEXTURE9 pDataSrc, LPDIRECT3DTEXTURE9 pDataDes,
						   RECT SrcCopyRect, RECT DesCopyRect)
{
	//copy The background data first
	D3DSURFACE_DESC  pDescDes;

	D3DLOCKED_RECT d3dlt;

	if(!LockSystemTexture(pDescDes, d3dlt, &DesCopyRect, pDataDes))
	{
		return;
	};
	Byte * pDesTData2 = (Byte *)d3dlt.pBits;

	D3DSURFACE_DESC  pDescSrc;
	D3DLOCKED_RECT d3dlt2;

	if(!LockSystemTexture(pDescSrc, d3dlt2, &SrcCopyRect, pDataSrc))
	{
		//DebugBreak();
		pDataDes->UnlockRect(0);
		return;
	}

	Byte * pSrcTData2 = (Byte *)d3dlt2.pBits;

	//copy the texture line by line
	UInt32 CopyLen = (SrcCopyRect.right - SrcCopyRect.left ) * sizeof(Color32);

	for (Int y = 0; y < SrcCopyRect.bottom - SrcCopyRect.top; y++)
	{
		memcpy(pDesTData2, pSrcTData2, CopyLen);
		pDesTData2 += pDescDes.Width * sizeof(Color32);
		pSrcTData2 += pDescSrc.Width * sizeof(Color32);
	}
	
	pDataDes->UnlockRect(0);
	pDataSrc->UnlockRect(0);
}

VOID CRender::Copy8888To8888(Byte * pSrcData, Int SrcStartIndex, Int32 SrcWidth, Int32 SrcHeight, UInt16 SrcMaxWidth,
		Byte * pDesData, Int DesStartIndex, UInt16 DesMaxWidth)
{
	pDesData += DesStartIndex * 4;
	pSrcData += SrcStartIndex * 4;
	UInt32 SrcLen = (SrcMaxWidth * 4);
	UInt32 DesLen = (DesMaxWidth * 4);
	Int UpdateLen = (SrcWidth * 4);
	UInt32 SrcLen2 = SrcLen - UpdateLen;
	UInt32 DesLen2 = DesLen - UpdateLen;

	/*
	for (WORD y = 0; y < SrcHeight; y++)
	{
		memcpy(pDesData, pSrcData, UpdateLen);

		pDesData += DesLen;
		pSrcData += SrcLen;
	}*/

	__asm
	{
		pushad

		mov		esi, pSrcData
		mov		edi, pDesData

		mov		ecx, SrcHeight

LOOP_L1_1:

		push	ecx

		mov		ecx, SrcWidth
		rep		movsd

		add		esi, SrcLen2
		add		edi, DesLen2

		pop		ecx
		loop	LOOP_L1_1

		popad
	}
}

VOID CRender::Copy565To8888(Byte * pSrcData, Int SrcStartIndex, Int32 SrcWidth, Int32 SrcHeight, UInt16 SrcMaxWidth,
		Byte * pDesData, Int DesStartIndex, UInt16 DesMaxWidth)
{
	pDesData += DesStartIndex * 4;
	pSrcData += SrcStartIndex * 2;
	UInt32 SrcLen = (SrcMaxWidth * 2);
	UInt32 DesLen = (DesMaxWidth * 4);

	Int UpdateSrcLen = (SrcWidth * 2);
	Int UpdateDesLen = (SrcWidth * 4);

	for (WORD y = 0; y < SrcHeight; y++)
	{
		for (WORD x = 0; x < SrcWidth; x++)
		{
			//change RGB565 to RGB888

			WORD w16 = *((WORD*)pSrcData);

			pDesData[ 3] = 0xFF;	//A
			pDesData[ 2] = (w16 & 0xF800) >> 8; //R
			pDesData[ 1] = (w16 & 0x7E0) >> 3; //G
			pDesData[0] = (w16 & 0x1F) << 3; //B

			pDesData += 4;
			pSrcData += 2;

		}

		pDesData += DesLen - UpdateDesLen;
		pSrcData += SrcLen - UpdateSrcLen;
	}

}
VOID CRender::BlitPicWithEffect(Byte * pSrcData, Int SrcStartIndex, Int32 SrcWidth, Int32 SrcHeight, UInt16 SrcMaxWidth,
					  Byte * pDesData, Int DesStartIndex,  UInt16 DesMaxWidth, BOOL flip, Byte m_AlphaOffset, 
					  Byte PixelSizeSrc, Byte PixelSizeDes, BOOL AlphaEffect,
					  Byte * pBgData)
{	
	pDesData += DesStartIndex * PixelSizeDes;
	pBgData += DesStartIndex * PixelSizeDes;
	
	pSrcData += SrcStartIndex * PixelSizeSrc;
	UInt32 SrcLen = (SrcMaxWidth * PixelSizeSrc);
	UInt32 DesLen = (DesMaxWidth * PixelSizeDes);
	//assume it is 32 bit
	Int UpdateSrcLen = (SrcWidth * PixelSizeSrc);
	Int UpdateDesLen = (SrcWidth * PixelSizeDes);
	Int DesMovePixel = PixelSizeDes;

	if (flip)
	{
		pDesData += UpdateDesLen - PixelSizeDes;
		pBgData += UpdateDesLen - PixelSizeDes;
		
		UpdateDesLen *= -1;
		DesMovePixel *= -1;

	}

	UpdateSrcLen = SrcLen - UpdateSrcLen;
	UpdateDesLen = DesLen - UpdateDesLen;

	if (m_AlphaOffset > 0)
	{
		if (AlphaEffect)
		{
			for (UInt16 y = 0; y < SrcHeight; y++)
			{
				for (UInt16 x = 0; x < SrcWidth; x++)
				{		
					//convert to 32 bits first
					pSrcData[3] = pSrcData[3] * ( 255 - m_AlphaOffset) / 255;

					//BlitSinglePixel(pSrcData, pDesData);
					*((UInt32 *) (pDesData)) = MMXAlpha(*((UInt32 *) (pSrcData)), 
						*((UInt32 *) (pDesData)));

					if (pSrcData[3] && pBgData[3])
					{
						*((UInt32 *) (pDesData)) = MMXAlpha(*((UInt32 *) (pBgData)), 
							*((UInt32 *) (pDesData)));
					}

					pSrcData += PixelSizeSrc;
					pDesData += DesMovePixel;
					pBgData += DesMovePixel;
				}

				pSrcData += UpdateSrcLen;
				pDesData += UpdateDesLen;
				pBgData += UpdateDesLen;
			}
		}
		else
		{
			for (UInt16 y = 0; y < SrcHeight; y++)
			{
				for (UInt16 x = 0; x < SrcWidth; x++)
				{		
					//convert to 32 bits first
					pSrcData[3] = pSrcData[3] * ( 255 - m_AlphaOffset) / 255;

					//BlitSinglePixel(pSrcData, pDesData);
					*((UInt32 *) (pDesData)) = MMXAlpha(*((UInt32 *) (pSrcData)), 
						*((UInt32 *) (pDesData)));

					pSrcData += PixelSizeSrc;
					pDesData += DesMovePixel;
					
				}
				pSrcData += UpdateSrcLen;
				pDesData += UpdateDesLen;		
			}
		}
	}
	else
	{
		if (AlphaEffect)
		{
			for (UInt16 y = 0; y < SrcHeight; y++)
			{
				for (UInt16 x = 0; x < SrcWidth; x++)
				{		
					//BlitSinglePixel(pSrcData, pDesData);
					*((UInt32 *) (pDesData)) = MMXAlpha(*((UInt32 *) (pSrcData)), 
						*((UInt32 *) (pDesData)));

					if (pSrcData[3] && pBgData[3])
					{
						*((UInt32 *) (pDesData)) = MMXAlpha(*((UInt32 *) (pBgData)), 
							*((UInt32 *) (pDesData)));
					}

					pSrcData += PixelSizeSrc;
					pDesData += DesMovePixel;
					pBgData += DesMovePixel;
				}

				pSrcData += UpdateSrcLen;
				pDesData += UpdateDesLen;
				pBgData += UpdateDesLen;
			}
		}
		else
		{
			for (UInt16 y = 0; y < SrcHeight; y++)
			{
				for (UInt16 x = 0; x < SrcWidth; x++)
				{		
					//BlitSinglePixel(pSrcData, pDesData);
					*((UInt32 *) (pDesData)) = MMXAlpha(*((UInt32 *) (pSrcData)), 
						*((UInt32 *) (pDesData)));

					pSrcData += PixelSizeSrc;
					pDesData += DesMovePixel;
					
				}
				pSrcData += UpdateSrcLen;
				pDesData += UpdateDesLen;		
			}
		}
	}

}

VOID CRender::BlitAdditivePic(Byte * pSrcData, UInt32 SrcStartIndex, UInt16 SrcWidth, UInt16 SrcHeight, UInt16 SrcMaxWidth,
							  Byte * pDesData, UInt32 DesStartIndex,  UInt16 DesMaxWidth, BOOL flip, Byte PixelSize)
{
	pDesData += DesStartIndex * PixelSize;
	pSrcData += SrcStartIndex * PixelSize;
	UInt32 SrcLen = (SrcMaxWidth * PixelSize);
	UInt32 DesLen = (DesMaxWidth * PixelSize);

	UInt32 UpdateSrcLen = (SrcWidth * PixelSize);

	Int UpdateDesLen = (SrcWidth * PixelSize);
	Int DesMovePixel = PixelSize;

	if (flip)
	{
		pDesData += UpdateDesLen - PixelSize;
		UpdateDesLen *= -1;
		DesMovePixel *= -1;
	}

	UpdateSrcLen = SrcLen - UpdateSrcLen;
	UpdateDesLen = DesLen - UpdateDesLen;

	for (UInt16 y = 0; y < SrcHeight; y++)
	{
		for (UInt16 x = 0; x < SrcWidth; x++)
		{
			if (pDesData[0] + pSrcData[0] > 255)
				pDesData[0] = 255;
			else
				pDesData[0] = pDesData[0] + pSrcData[0];
			if (pDesData[ 1] + pSrcData[ 1] > 255)
				pDesData[ 1] = 255;
			else
				pDesData[ 1] = pDesData[ 1] + pSrcData[ 1];
			if (pDesData[ 2] + pSrcData[ 2]  > 255)
				pDesData[ 2] = 255;
			else
				pDesData[ 2] = pDesData[ 2] + pSrcData[ 2];

			pSrcData += PixelSize;

			pDesData += DesMovePixel;


		}	
		pSrcData += UpdateSrcLen;
		pDesData += UpdateDesLen;
	}

}

VOID CRender::BlitSeaAlphaPic(Byte * pSrcData, UInt32 SrcStartIndex, UInt16 SrcWidth, UInt16 SrcHeight, UInt16 SrcMaxWidth
							  , Byte * BgData, UInt16 BgWidth, UInt32 BgStartIndex, Byte PixelSize)
{

	pSrcData += SrcStartIndex * PixelSize;

	UInt32 SrcLen = (SrcMaxWidth * PixelSize);

	UInt32 UpdateSrcLen = (SrcWidth * PixelSize);

	BgData += BgStartIndex * PixelSize;

	UInt32 BgLen = BgWidth * PixelSize;

	Byte  pTempData[4];

	for (UInt16 y = 0; y < SrcHeight; y++)
	{
		for (UInt16 x = 0; x < SrcWidth; x++)
		{
			if (pSrcData[ 3] > SeaAlpha)
			{
				//UInt32 BgTempStartIndex = (y * BgWidth + x  + BgStartIndex) * PixelSize;
				memcpy(pTempData, BgData, PixelSize);
				pTempData[ 3] = SeaAlpha;
				BlitSinglePixel(pTempData, pSrcData);
			}
			else
			{
				//UInt32 BgTempStartIndex = (y * BgWidth + x  + BgStartIndex) * PixelSize;
				memcpy(pSrcData,BgData, PixelSize);
			}

			BgData += PixelSize;
			pSrcData += PixelSize;
		}

		pSrcData += SrcLen - UpdateSrcLen;
		BgData += BgLen - UpdateSrcLen;
	}
}

VOID CRender::BlitAlphaPic(Byte * pSrcData, UInt32 SrcStartIndex, UInt16 SrcWidth, UInt16 SrcHeight, UInt16 SrcMaxWidth,
						  Byte * pDesData, UInt32 DesStartIndex, UInt16 DesMaxWidth, BOOL IsObject
						  , Byte * BgData, UInt16 BgWidth, UInt32 BgStartIndex, Byte PixelSize)
{

	UInt32 SrcLen = (SrcMaxWidth * PixelSize);
	UInt32 UpdateSrcLen = (SrcWidth * PixelSize);
	if (IsObject)
		pSrcData += SrcStartIndex * PixelSize;
	else
	{
		pSrcData += SrcStartIndex;
		SrcLen = SrcMaxWidth;
		UpdateSrcLen = SrcWidth;
	}

	pDesData += DesStartIndex * PixelSize;
	
	UInt32 DesLen = (DesMaxWidth * PixelSize);

	Int UpdateDesLen = (SrcWidth * PixelSize);
	Int DesMovePixel = PixelSize;

	BgData += BgStartIndex * PixelSize;

	UInt32 BgLen = BgWidth * PixelSize;

	if (IsObject)
	{
		for (UInt16 y = 0; y < SrcHeight; y++)
		{
			for (UInt16 x = 0; x < SrcWidth; x++)
			{

				if (pSrcData[3] > 0)
				{
					//memcpy(pTempData, BgData, PixelSize);
					BgData[ 3] = ObjectAlpha;
					*((UInt32 *) (pDesData)) = MMXAlpha(*((UInt32 *) (BgData)), 
						*((UInt32 *) (pDesData)));
				}
				pSrcData += PixelSize;

				BgData += PixelSize;

				pDesData += DesMovePixel;

			}

			pSrcData += SrcLen - UpdateSrcLen;
			BgData += BgLen - UpdateDesLen;
			pDesData += DesLen - UpdateDesLen;

		}
	}
	else
	{
		for (UInt16 y = 0; y < SrcHeight; y++)
		{
			for (UInt16 x = 0; x < SrcWidth; x++)
			{
				if (*pSrcData > 0)
				{
				BgData[ 3] = *pSrcData;

				*((UInt32 *) (pDesData)) = MMXAlpha(*((UInt32 *) (BgData)), 
					*((UInt32 *) (pDesData)));
				}
				pSrcData ++;

				BgData += PixelSize;

				pDesData += DesMovePixel;

			}

			pSrcData += SrcLen - UpdateSrcLen;
			BgData += BgLen - UpdateDesLen;

			pDesData += DesLen - UpdateDesLen;

		}
	}

	
}

VOID CRender::BlitAlphaMapPic(Byte * pSrcData, UInt32 SrcStartIndex, UInt16 SrcWidth, UInt16 SrcHeight, UInt16 SrcMaxWidth,
						  Byte * pDesData, UInt32 DesStartIndex, UInt16 DesMaxWidth
						  , Byte * BgData, UInt16 BgWidth, UInt32 BgStartIndex)
{

	UInt32 SrcLen = (SrcMaxWidth * 4);
	UInt32 UpdateSrcLen = (SrcWidth * 4);

	pSrcData += SrcStartIndex;
	SrcLen = SrcMaxWidth;
	UpdateSrcLen = SrcWidth;

	pDesData += DesStartIndex * 4;

	UInt32 DesLen = (DesMaxWidth * 4);

	Int UpdateDesLen = (SrcWidth * 4);
	Int UpdateBgLen = (SrcWidth * 2);
	Int DesMovePixel = 4;

	BgData += BgStartIndex * 2;

	UInt32 BgLen = BgWidth * 2;

	Byte TempData[4];

	for (UInt16 y = 0; y < SrcHeight; y++)
	{
		for (UInt16 x = 0; x < SrcWidth; x++)
		{
			if (*pSrcData > 0)
			{
				//BgData[ 3] = *pSrcData;
				TempData[3] = *pSrcData;
				TempData[ 2] = (*((WORD *)BgData) & 0xF800) >> 8; //R
				TempData[ 1] = (*((WORD *)BgData) & 0x7E0) >> 3; //G
				TempData[0] = (*((WORD *)BgData) & 0x1F) << 3; //B

				*((UInt32 *) (pDesData)) = MMXAlpha(*((UInt32 *) (TempData)), 
					*((UInt32 *) (pDesData)));
			}
			pSrcData ++;

			BgData += 2;

			pDesData += DesMovePixel;

		}

		pSrcData += SrcLen - UpdateSrcLen;
		BgData += BgLen - UpdateBgLen;

		pDesData += DesLen - UpdateDesLen;

	}
	

	
}

VOID CRender::BlitSubtractivePic(Byte * pSrcData, UInt32 SrcStartIndex, UInt16 SrcWidth, UInt16 SrcHeight, UInt16 SrcMaxWidth,
						  Byte * pDesData, UInt32 DesStartIndex, UInt16 DesMaxWidth, BOOL flip, BOOL IsObject
						  , Byte PixelSize)
{
	UInt32 SrcLen = (SrcMaxWidth * PixelSize);
	UInt32 UpdateSrcLen = (SrcWidth * PixelSize);
	if (IsObject)
		pSrcData += SrcStartIndex * PixelSize;
	else
	{
		pSrcData += SrcStartIndex;
		SrcLen = SrcMaxWidth;
		UpdateSrcLen = SrcWidth;
	}

	pDesData += DesStartIndex * PixelSize;
	
	UInt32 DesLen = (DesMaxWidth * PixelSize);
	
	Int UpdateDesLen = (SrcWidth * PixelSize);
	Int DesMovePixel = PixelSize;

	if (flip)
	{
		pDesData += UpdateDesLen - PixelSize;
		UpdateDesLen *= -1;
		DesMovePixel *= -1;
	}

	//UpdateSrcLen = SrcLen - UpdateSrcLen;
	//UpdateDesLen = DesLen - UpdateDesLen;

	if (IsObject)
	{
		for (UInt16 y = 0; y < SrcHeight; y++)
		{
			for (UInt16 x = 0; x < SrcWidth; x++)
			{
				//Byte srcAl = pSrcData[SrcPos + 3];
				/*if (pDesData[0] > pSrcData[0])
					pDesData[0] -= pSrcData[0];
				else
					pDesData[0] = 0;
				if (pDesData[ 1] > pSrcData[ 1])
					pDesData[ 1] -= pSrcData[ 1];
				else
					pDesData[ 1] = 0;
				if (pDesData[ 2] > pSrcData[ 2])
					pDesData[ 2] -= pSrcData[ 2];
				else
					pDesData[ 2] = 0;
					*/
				pDesData[0] = gGlobal.SubtractiveTable[pDesData[0]][pSrcData[3]];
				pDesData[1] = gGlobal.SubtractiveTable[pDesData[1]][pSrcData[3]];
				pDesData[2] = gGlobal.SubtractiveTable[pDesData[2]][pSrcData[3]];


				pSrcData += PixelSize;
				pDesData += DesMovePixel;
			}

			pSrcData += SrcLen - UpdateSrcLen;

			pDesData += DesLen - UpdateDesLen;
		}
	}
	else
	{
		//pDesData[m_DesPos] = pApp->SubtractiveTable[pDesData[m_DesPos]][pSrcData[SrcPos]];
		//pDesData[m_DesPos + 1] = pApp->SubtractiveTable[pDesData[m_DesPos + 1]][pSrcData[SrcPos + 1]];
		//pDesData[m_DesPos + 2] = pApp->SubtractiveTable[pDesData[m_DesPos + 2]][pSrcData[SrcPos + 2]];

		for (UInt16 y = 0; y < SrcHeight; y++)
		{
			for (UInt16 x = 0; x < SrcWidth; x++)
			{
				/*if (pDesData[0] > pSrcData[0])
					pDesData[0] -= pSrcData[0];
				else
					pDesData[0] = 0;
				if (pDesData[ 1] > pSrcData[0 ])
					pDesData[ 1] -= pSrcData[0 ];
				else
					pDesData[ 1] = 0;
				if (pDesData[ 2] > pSrcData[0 ])
					pDesData[ 2] -= pSrcData[0 ];
				else
					pDesData[ 2] = 0;
					*/
				pDesData[0] = gGlobal.SubtractiveTable[pDesData[0]][pSrcData[0]];
				pDesData[1] = gGlobal.SubtractiveTable[pDesData[1]][pSrcData[0]];
				pDesData[2] = gGlobal.SubtractiveTable[pDesData[2]][pSrcData[0]];

				pSrcData ++;
				pDesData += DesMovePixel;
			}

			pSrcData += SrcLen - UpdateSrcLen;

			pDesData += DesLen - UpdateDesLen;
		}
	}
	
}

//lock rectangle  function
BOOL CRender::LockSystemTexture(D3DSURFACE_DESC  & pDesc, D3DLOCKED_RECT & d3dlt, 
								 RECT * LockRect, LPDIRECT3DTEXTURE9 SysTexture, DWORD Flags)
{
	if (SysTexture == NULL)
	{
		DebugBreak();
		return FALSE;
	}

	SysTexture->GetLevelDesc(0, &pDesc);
	//safety check for invalid lock
	if (LockRect && QGLMath::InvalidLock(LockRect, pDesc.Width, pDesc.Height))
	{
		return FALSE;
	}

	if (!LockRect)
	{
		if (FAILED(SysTexture->LockRect(0, &d3dlt, 0, Flags)))
		{
			DebugBreak();
			return FALSE;
		}
	}
	else
	{
		if (FAILED(SysTexture->LockRect(0, &d3dlt, LockRect, Flags)))
		{
			DebugBreak();
			return FALSE;
		}
	}
	return TRUE;
}


VOID CRender::DrawNightMask(DrawEntityTextureData NightMaskData, CEntityBase * pMaskEnt, CCamera * CurrCam, CMapObjectView * pMap)
{
	RECT MaskRect;
	SetRect(&MaskRect, 0,0,0,0);
	if (!IsEntityInsideRenderWindow(pMaskEnt, CurrCam, &MaskRect))
		return;
	if (pMap == NULL || pMap->GetMapData() == NULL )
		return;
	StlMap<CEntityBase *, RECT>::iterator PreDirRectItr = NightMaskData.m_PreDirtyRegionLst.find(pMaskEnt);
	if (PreDirRectItr != NightMaskData.m_PreDirtyRegionLst.end())
	{
		QGLMath::CalculateBiggestRect(MaskRect, PreDirRectItr->second, MaskRect);
		//MaskRect = PreDirRectItr->second;
	}
	//clean  all dirty rect first
	for (Int i = 0; i < (Int)NightMaskData.m_DirRectMan.GetDirtyRectLst().size(); i++)
	{
		RECT WinRect = NightMaskData.m_DirRectMan.GetDirtyRectLst()[i];
		Int RowPerPic = 0;
		Int indexX = 0;
		Int MaxIndexX = 0;
		if (pMap->GetSubMapWidth() > 0)
		{
			RowPerPic = (Int)ceil(((Real32)pMap->GetMapWidth() / (Real32)pMap->GetSubMapWidth()));
			indexX = (Int)CurrCam->GetLeftUpPos().x / pMap->GetSubMapWidth();
			MaxIndexX = (Int)ceil(((Real32)(CurrCam->GetLeftUpPos().x + m_CurrWinWidth) / (Real32)pMap->GetSubMapWidth())); 
		}
		Int indexY = 0;
		Int MaxIndexY = 0;

		if (pMap->GetSubMapHeight() > 0)
		{
			indexY = (Int)CurrCam->GetLeftUpPos().y / pMap->GetSubMapHeight();
			MaxIndexY = (Int)ceil(((Real32)(CurrCam->GetLeftUpPos().y + m_CurrWinHeight) / (Real32)pMap->GetSubMapHeight())); 
		}

		Int PicCount = indexY * RowPerPic + indexX;

		for (Int y = indexY; y <= MaxIndexY; y ++)
		{
			for (Int x = indexX; x <= MaxIndexX; x ++)
			{
				Int subIndex = (y * RowPerPic + x) * pMap->GetMapData()->totalNightFrame;
				if (subIndex >= 0)
				{
					RECT DirtyRect;
					DirtyRect.bottom = 0;
					RECT TempRect;
					Int WinLeftUpX =   (Int)(x * pMap->GetSubMapWidth()) - (Int)CurrCam->GetLeftUpPos().x ;
					Int WinLeftUpY =  (Int)(y * pMap->GetSubMapHeight()) - (Int)CurrCam->GetLeftUpPos().y ;
					if (WinLeftUpX < (Int)m_CurrWinWidth && WinLeftUpY < (Int)m_CurrWinHeight)
					{
						TempRect.left = TempRect.top = 0;

						if (WinLeftUpX < 0)
						{
							TempRect.left = -1 * WinLeftUpX;
							WinLeftUpX = 0;
						}
						if (WinLeftUpY < 0)
						{
							TempRect.top = -1 * WinLeftUpY;
							WinLeftUpY = 0;
						}

						TempRect.bottom = pMap->GetSubMapHeight();					
						TempRect.right = pMap->GetSubMapWidth();

						RECT LockRegion;
						LockRegion.left = WinLeftUpX;
						LockRegion.top = WinLeftUpY;
						LockRegion.right = LockRegion.left + TempRect.right - TempRect.left;
						LockRegion.bottom = LockRegion.top + TempRect.bottom -TempRect.top; 

						RECT TempLockRegion = LockRegion;

						if (QGLMath::CalculateOverlappedRect(TempLockRegion, WinRect, LockRegion))
						{
							RECT SrcUpdateRect = TempRect;
							SrcUpdateRect.left += (LockRegion.left - WinLeftUpX);
							SrcUpdateRect.right = SrcUpdateRect.left + (LockRegion.right - LockRegion.left);

							SrcUpdateRect.top += (LockRegion.top - WinLeftUpY);
							SrcUpdateRect.bottom = SrcUpdateRect.top + (LockRegion.bottom - LockRegion.top);

							//D3DSURFACE_DESC  pDesc;
							Byte * pData = pMap->GetNightTextureByIndex(subIndex);
							if (pData)
							{
								RECT ShiftRect = LockRegion;
								ShiftRect.left += NightMaskData.m_CurrDisArea.left;
								ShiftRect.right += NightMaskData.m_CurrDisArea.left;
								ShiftRect.top += NightMaskData.m_CurrDisArea.top;
								ShiftRect.bottom += NightMaskData.m_CurrDisArea.top;
								D3DSURFACE_DESC  pDescDes;
								D3DLOCKED_RECT d3dltDes;
								if (LockSystemTexture(pDescDes, d3dltDes, &ShiftRect, NightMaskData.Texture))
								{
									Byte * pDesData = (Byte *)d3dltDes.pBits;
									if (gConfigure.uCompressedMap)
										DXT3To32Bits(pData, pDesData, SrcUpdateRect, pDescDes.Width);
									else
									{
										//Copy8888
										Byte * pSrcData = pData + (SrcUpdateRect.top * pMap->GetSubMapWidth() + SrcUpdateRect.left) * 4;
										Copy8888To8888(pSrcData, 0, (UInt16)(SrcUpdateRect.right - SrcUpdateRect.left), 
											(UInt16)(SrcUpdateRect.bottom - SrcUpdateRect.top ), pMap->GetSubMapWidth(),
											pDesData, 0, MaxEntSizeX);
									}
									NightMaskData.Texture->UnlockRect(0);
								}
								//CopyTexture(pTexture, NightMaskData.Texture, SrcUpdateRect, ShiftRect);
								RECT ResultRect;
								if (QGLMath::CalculateOverlappedRect(MaskRect, LockRegion, ResultRect))
								{
									RECT nightRect;
									nightRect = MaskRect;
									nightRect.left += (Int)CurrCam->GetLeftUpPos().x;
									nightRect.right += (Int)CurrCam->GetLeftUpPos().x;
									nightRect.top += (Int)CurrCam->GetLeftUpPos().y;
									nightRect.bottom += (Int)CurrCam->GetLeftUpPos().y;
									if (pMaskEnt->GetEntityPart(0, ImagePic, &nightRect))
									{
										Int SrcWidth = pMaskEnt->GetPicWidth();
										
										Byte * pSrcData = pMaskEnt->GetPicData() + ((ResultRect.top - MaskRect.top) * 
											SrcWidth + (ResultRect.left - MaskRect.left)) * 4;
										
										

										D3DSURFACE_DESC  pDescDes;
										D3DLOCKED_RECT d3dltDes;

										RECT ShiftRect = ResultRect;
										ShiftRect.left += NightMaskData.m_CurrDisArea.left;
										ShiftRect.right += NightMaskData.m_CurrDisArea.left;
										ShiftRect.top += NightMaskData.m_CurrDisArea.top;
										ShiftRect.bottom += NightMaskData.m_CurrDisArea.top;
										if (LockSystemTexture(pDescDes, d3dltDes, &ShiftRect, NightMaskData.Texture))
										{
											Byte * pDesData = (Byte *)d3dltDes.pBits;
											for (Int y = 0; y < ResultRect.bottom - ResultRect.top; y++)
											{
												Byte * pSubSrcData = pSrcData;
												Byte * pSubDesData = pDesData;
												for (Int x = 0; x < ResultRect.right - ResultRect.left; x++)
												{
													pSubDesData[3] = gGlobal.SubtractiveTable[pSubDesData[3]][pSubSrcData[3]];
													pSubSrcData += 4;
													pSubDesData += 4;
												}
												pDesData += d3dltDes.Pitch;
												pSrcData += SrcWidth * 4;
											}
											NightMaskData.Texture->UnlockRect(0);
										}
									}
								}
							}
						}
					}
				}
			}
		}
		//check whether overlap with the mask entity

		RECT ShiftRect = WinRect;
		ShiftRect.left += NightMaskData.m_CurrDisArea.left;
		ShiftRect.right += NightMaskData.m_CurrDisArea.left;
		ShiftRect.top += NightMaskData.m_CurrDisArea.top;
		ShiftRect.bottom += NightMaskData.m_CurrDisArea.top;

		NightMaskData.Texture->AddDirtyRect(&ShiftRect);
		gGlobal.pUIManager->GetDirtyRectMan()->AddDirtyRectToMan(WinRect);
		/*DrawTextureBySprite(WinRect.left
			, WinRect.top, ShiftRect, NightMaskData.Texture, gGlobal.g_NightMaskAlpha);
		m_pSprite->Flush();
		*/
	}

	/*for (Int i = 0; i < (Int)NightMaskData.m_DirRectMan.GetNotDirtyRectLst().size(); i++)
	{
		RECT NotDirtyRect = NightMaskData.m_DirRectMan.GetNotDirtyRectLst()[i];

		RECT ShiftRect = NotDirtyRect;
		ShiftRect.left += NightMaskData.m_CurrDisArea.left;
		ShiftRect.right += NightMaskData.m_CurrDisArea.left;
		ShiftRect.top += NightMaskData.m_CurrDisArea.top;
		ShiftRect.bottom += NightMaskData.m_CurrDisArea.top;
		DrawTextureBySprite(NotDirtyRect.left
			, NotDirtyRect.top, ShiftRect, NightMaskData.Texture, gGlobal.g_NightMaskAlpha);
	}*/

	//m_pSprite->Flush();
	
}
VOID CRender::DrawNightMaskByPS(CMapObjectView * pMap, CCamera * CurrCam, PixelPos MaskCenter)
{
	if (pMap == NULL ||
		pMap->GetMapData() == NULL ||
		CurrCam == NULL)
	{
		return;
	}

	//Int index = pMap->GetMapIndexByBlockPos(Player.GetCharPos());
	//Int index = pMap->GetMapIndexByBlockPos(Player.GetCharPos());
	

	Int RowPerPic = 0;
	Int indexX = 0;
	Int MaxIndexX = 0;
	if (pMap->GetSubMapWidth() > 0)
	{
		RowPerPic = (Int)ceil(((Real32)pMap->GetMapWidth() / (Real32)pMap->GetSubMapWidth()));
		indexX = (Int)CurrCam->GetLeftUpPos().x / pMap->GetSubMapWidth();
		MaxIndexX = (Int)ceil(((Real32)(CurrCam->GetLeftUpPos().x + m_CurrWinWidth) / (Real32)pMap->GetSubMapWidth())); 
	}
	Int indexY = 0;
	Int MaxIndexY = 0;

	if (pMap->GetSubMapHeight() > 0)
	{
		indexY = (Int)CurrCam->GetLeftUpPos().y / pMap->GetSubMapHeight();
		MaxIndexY = (Int)ceil(((Real32)(CurrCam->GetLeftUpPos().y + m_CurrWinHeight) / (Real32)pMap->GetSubMapHeight())); 
	}

	Int PicCount = indexY * RowPerPic + indexX;

	for (Int y = indexY; y <= MaxIndexY; y ++)
	{
		for (Int x = indexX; x <= MaxIndexX; x ++)
		{
			Int subIndex = (y * RowPerPic + x) * pMap->GetMapData()->totalNightFrame;
			if (subIndex >= 0)
			{
				RECT DirtyRect;
				DirtyRect.bottom = 0;
				RECT TempRect;
				Int WinLeftUpX =   (Int)(x * pMap->GetSubMapWidth()) - (Int)CurrCam->GetLeftUpPos().x ;
				Int WinLeftUpY =  (Int)(y * pMap->GetSubMapHeight()) - (Int)CurrCam->GetLeftUpPos().y ;
				if (WinLeftUpX < (Int)m_CurrWinWidth && WinLeftUpY < (Int)m_CurrWinHeight)
				{
					TempRect.left = TempRect.top = 0;

					if (WinLeftUpX < 0)
					{
						TempRect.left = -1 * WinLeftUpX;
						WinLeftUpX = 0;
					}
					if (WinLeftUpY < 0)
					{
						TempRect.top = -1 * WinLeftUpY;
						WinLeftUpY = 0;
					}

					TempRect.bottom = pMap->GetSubMapHeight();					
					TempRect.right = pMap->GetSubMapWidth();

					RECT LockRegion;
					LockRegion.left = WinLeftUpX;
					LockRegion.top = WinLeftUpY;
					LockRegion.right = LockRegion.left + TempRect.right - TempRect.left;
					LockRegion.bottom = LockRegion.top + TempRect.bottom -TempRect.top; 

					RECT TempLockRegion = LockRegion;

					if (QGLMath::CalculateOverlappedRect(TempLockRegion, m_OrgWinRect, LockRegion))
					{
						RECT SrcUpdateRect = TempRect;
						SrcUpdateRect.left += (LockRegion.left - WinLeftUpX);
						SrcUpdateRect.right = SrcUpdateRect.left + (LockRegion.right - LockRegion.left);

						SrcUpdateRect.top += (LockRegion.top - WinLeftUpY);
						SrcUpdateRect.bottom = SrcUpdateRect.top + (LockRegion.bottom - LockRegion.top);
					}
				}
			}
		}
	}
}
VOID CRender::FreeVolatileResources()
{
	
	if (m_pSprite)
	{
		m_pSprite->OnLostDevice();//Release();
		//m_pSprite->Release();
		//m_pSprite = NULL;
	}
	
	/*
	LPDIRECT3DSURFACE9  pDesBuffer = NULL;
	pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pDesBuffer);
	if (pDesBuffer)
	{
		pDesBuffer->Release();
		MessageBox(NULL,_T("pDesBuffer->Release;"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
	}
	*/
/*	if (m_CombinedDefaultTexture)
	{
		m_CombinedDefaultTexture->Release();
		m_CombinedDefaultTexture = NULL;
	}*/
}

VOID CRender::InitVolatileResources()
{
	/*
	if (m_pSprite)
		m_pSprite->OnResetDevice();

	if(CheckMyProcess(hWnd) > 1)
		m_FullScnFlag = false;


	if(IsFullScreen())
	{
		::SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
		::SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
	}
	else
	{
		if(m_IsSubWindow)
		{
			::SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_CAPTION | WS_VISIBLE);
			::SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
		}
		else
		{
			::SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE);
			::SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
		}
	}

	
	Init();


	if(IsFullScreen())
	{
		ChangeFullScreen(hWnd);
	}
	else
	{
		ChangeOldScreen(hWnd, &m_rcWindowBounds);
	}
	*/
		
}
HRESULT CRender::ResetDevice()
{
	if(CheckMyProcess(hWnd) > 1)
		m_FullScnFlag = false;

	
	m_pSprite->OnLostDevice();
	HRESULT hr = pD3DDevice->Reset(&m_PresentPara );
	if (FAILED(hr))
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Renderer2D::ResetDevice Error Reset Failed : hr = %d"), hr);
		SafeRelease(m_pSprite);
		SafeRelease(pD3DDevice);
		if (!CreateD3DWindow(m_FullScnFlag))
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Renderer2D::ResetDevice Error CreateDevice Failed"));
			return (BOOLEAN_FALSE);
		}
	}
	
	ReSizeD3DScene(m_CurrWinWidth, m_CurrWinHeight/2);	

	InitD3D();

	m_pSprite->OnResetDevice();

	
	if(IsFullScreen())
	{
		::SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
		::SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
	}
	else
	{
		if(m_IsSubWindow)
		{
			::SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_CAPTION | WS_VISIBLE);
			::SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
		}
		else
		{
			::SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE);
			::SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
		}
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window

	bool	ChangeScreenFlag;

	if(IsFullScreen())
		ChangeScreenFlag = ChangeFullScreen(hWnd);
	else
		ChangeScreenFlag = ChangeOldScreen(hWnd, &m_rcWindowBounds);
		
	if(ChangeScreenFlag)
	{
		HRESULT result = pD3DDevice->TestCooperativeLevel();

		if ( FAILED( result ) )
		{
			if (result == D3DERR_DEVICENOTRESET)
				HRESULT hr = pD3DDevice->Reset(&m_PresentPara );

			result = pD3DDevice->TestCooperativeLevel();
			if ( FAILED( result ) )
			{
				if (result == D3DERR_DEVICENOTRESET)
				{
					MessageBox(NULL,_T("result == D3DERR_DEVICENOTRESET"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
				}
			}

		}
	}


	
	

	AddAllDirtyRect();

	
	
    return S_OK;
}

VOID CRender::ToggleFullscreen()
{
	
	if (!m_FullScnFlag)//m_PresentPara.Windowed)
		GetWindowRect( hWnd, &m_rcWindowBounds );

	m_FullScnFlag = !m_FullScnFlag;
	m_PresentPara.Windowed = TRUE;//!m_PresentPara.Windowed;

	
	bool	ChangeScreenFlag;

	if(m_FullScnFlag)
	{
		::SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
		::SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW);

		ChangeScreenFlag = ChangeFullScreen(hWnd);
		
	}
	else
	{
		::SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE);
		::SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

		ChangeScreenFlag = ChangeOldScreen(hWnd, &m_rcWindowBounds);
	}
	if(ChangeScreenFlag)
	{
		HRESULT result = pD3DDevice->TestCooperativeLevel();

		if ( FAILED( result ) )
		{
			if (result == D3DERR_DEVICENOTRESET)
				HRESULT hr = pD3DDevice->Reset(&m_PresentPara );

			result = pD3DDevice->TestCooperativeLevel();
			if ( FAILED( result ) )
			{
				if (result == D3DERR_DEVICENOTRESET)
				{
					MessageBox(NULL,_T("result == D3DERR_DEVICENOTRESET"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
				}
			}

		}
	}

	AddAllDirtyRect();
	
	
}

BOOL CRender::IsDeviceValide()
{
	//check Device Lost
	HRESULT result = pD3DDevice->TestCooperativeLevel();
	if ( FAILED( result ) )
	{
		if ( result == D3DERR_DEVICELOST )
		{
			FreeVolatileResources();
			//Sleep( 50 );
		}
		else if (result == D3DERR_DEVICENOTRESET)
		{
			
			ResetDevice();
			

			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

VOID CRender::ClearTexture(LPDIRECT3DTEXTURE9 pTexture, RECT  * ClearRect)
{
	D3DSURFACE_DESC  pDescDes;
	D3DLOCKED_RECT d3dltDes;
	if (LockSystemTexture(pDescDes, d3dltDes, ClearRect, pTexture))
	{
		Byte * pDataDes = (Byte *)d3dltDes.pBits;
		if (ClearRect == NULL)
		{
			ZeroMemory(pDataDes, d3dltDes.Pitch * pDescDes.Height);
		}
		else
		{
			UInt32 Len = (ClearRect->right - ClearRect->left) * sizeof(Color32);
			for (Int y = 0; y < ClearRect->bottom - ClearRect->top; y ++)
			{
				ZeroMemory(pDataDes, Len);
				pDataDes += d3dltDes.Pitch;
			}
		}
		pTexture->UnlockRect(0);
	}
}

VOID CRender::AddCursorDirtyRect()
{
	RECT CursorRect;
	CursorRect.left = m_CursorX;
	CursorRect.right = m_CursorX + 1;
	CursorRect.top = m_CursorY;
	CursorRect.bottom = m_CursorY + 1;

	m_CombinedEntityTexture.m_DirRectMan.AddDirtyRectToMan(CursorRect);
}

VOID CRender::EnableAlphaBlend()
{
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
}
VOID CRender::DisableAlphaBlend()
{
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

bool CRender::IsFullScreen()
{
	return (m_FullScnFlag ? true : false);//!m_PresentPara.Windowed;
}

VOID CRender::OutputBackBufferToFile(String Path)
{
	String temp;
	
	TCHAR szBuffer[_MAX_PATH];
	//::GetCurrentDirectory(_MAX_PATH, szBuffer);
	::GetModuleFileName(::GetModuleHandle(NULL), szBuffer, _MAX_PATH);
	*(::_tcsrchr(szBuffer, _T('\\'))) =_T('\0');

	temp.Format(_T("%s\\ScreenCapture"), szBuffer);
	if (Reuben::Platform::File::IsExists(temp.c_str()) ||
		::CreateDirectory(temp.c_str(), NULL))
		
	{
		LPDIRECT3DSURFACE9  pSrcBuffer = NULL;
		pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSrcBuffer);
		if (pSrcBuffer)
		{
			String dateStr;
			SYSTEMTIME st;

			::GetLocalTime(&st);
			dateStr.Format(_T("%s%02d%02d%02d%02d%02d%02d.jpg"), Path.c_str(), st.wYear % 100, 
				st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			D3DXSaveSurfaceToFile(dateStr.c_str(), D3DXIFF_JPG, pSrcBuffer, NULL, NULL);
			pSrcBuffer->Release();
		}
	}
}

VOID CRender::BlitPicBW(Byte * pSrcData, Int SrcStartIndex, Int32 SrcWidth, Int32 SrcHeight, UInt16 SrcMaxWidth,
					  Byte * pDesData, Int DesStartIndex,  UInt16 DesMaxWidth, BOOL flip, Byte m_AlphaOffset, 
					  Byte PixelSizeSrc, Byte PixelSizeDes)

{	
	pDesData += DesStartIndex * PixelSizeDes;
	pSrcData += SrcStartIndex * PixelSizeSrc;
	UInt32 SrcLen = (SrcMaxWidth * PixelSizeSrc);
	UInt32 DesLen = (DesMaxWidth * PixelSizeDes);
	//assume it is 32 bit
	Int UpdateSrcLen = (SrcWidth * PixelSizeSrc);
	Int UpdateDesLen = (SrcWidth * PixelSizeDes);
	Int DesMovePixel = PixelSizeDes;

	if (flip)
	{
		pDesData += UpdateDesLen - PixelSizeDes;
		UpdateDesLen *= -1;
		DesMovePixel *= -1;
	}

	UpdateSrcLen = SrcLen - UpdateSrcLen;
	UpdateDesLen = DesLen - UpdateDesLen;

	if (m_AlphaOffset > 0)
	{
		Byte Data[4];
		for (UInt16 y = 0; y < SrcHeight; y++)
		{
			for (UInt16 x = 0; x < SrcWidth; x++)
			{				
				memcpy(Data, pSrcData, PixelSizeSrc);
				
				Data[3] = gGlobal.AlphaOffsetTable[Data[3]][m_AlphaOffset];
				
				*((UInt32 *) (pDesData)) = MMXAlphaBW(*((UInt32 *) (Data)), 
					*((UInt32 *) (pDesData)));

				pSrcData += PixelSizeSrc;

				pDesData += DesMovePixel;
			}
			pSrcData += UpdateSrcLen;
			pDesData += UpdateDesLen;

		}
	}
	else
	{
		for (UInt16 y = 0; y < SrcHeight; y++)
		{
			for (UInt16 x = 0; x < SrcWidth; x++)
			{				
				//BlitSinglePixel(pSrcData, pDesData);
				*((UInt32 *) (pDesData)) = MMXAlphaBW(*((UInt32 *) (pSrcData)), 
					*((UInt32 *) (pDesData)));

				pSrcData += PixelSizeSrc;
				pDesData += DesMovePixel;

			}
			pSrcData += UpdateSrcLen;
			pDesData += UpdateDesLen;

		}

	}

}

VOID CRender::BlitPic(Byte * pSrcData, Int SrcStartIndex, Int32 SrcWidth, Int32 SrcHeight, UInt16 SrcMaxWidth,
					  Byte * pDesData, Int DesStartIndex,  UInt16 DesMaxWidth, BOOL flip, Byte m_AlphaOffset, 
					  Int32 PixelSizeSrc, Int32 PixelSizeDes)

{	
	const __int64		T1 = 0x0001000100010001;
	const __int64		T2 = 0x0000FFFFFFFFFFFF;
	const __int64		T3 = 0xFFFF000000000000;

	if(!pSrcData || !pDesData)
		return;

	pDesData += DesStartIndex * PixelSizeDes;
	pSrcData += SrcStartIndex * PixelSizeSrc;
	UInt32 SrcLen = (SrcMaxWidth * PixelSizeSrc);
	UInt32 DesLen = (DesMaxWidth * PixelSizeDes);
	//assume it is 32 bit
	Int UpdateSrcLen = (SrcWidth * PixelSizeSrc);
	Int UpdateDesLen = (SrcWidth * PixelSizeDes);
	Int DesMovePixel = PixelSizeDes;

	if (flip)
	{
		pDesData += UpdateDesLen - PixelSizeDes;
		UpdateDesLen *= -1;
		DesMovePixel *= -1;
	}

	UpdateSrcLen = SrcLen - UpdateSrcLen;
	UpdateDesLen = DesLen - UpdateDesLen;

	if (m_AlphaOffset > 0)
	{
		/*
		Byte Data[4];
		for (UInt16 y = 0; y < SrcHeight; y++)
		{
			
			for (UInt16 x = 0; x < SrcWidth; x++)
			{				
				memcpy(Data, pSrcData, PixelSizeSrc);
				Data[3] = gGlobal.AlphaOffsetTable[Data[3]][m_AlphaOffset];
				//Data[3] = Data[3] * ( 255 - m_AlphaOffset) / 255;

				//BlitSinglePixel(Data, pDesData);
				*((UInt32 *) (pDesData)) = MMXAlpha(*((UInt32 *) (Data)), 
					*((UInt32 *) (pDesData)));

				pSrcData += PixelSizeSrc;

				pDesData += DesMovePixel;
			}
			pSrcData += UpdateSrcLen;
			pDesData += UpdateDesLen;

		}
		*/

		Byte			*pAlphaTable = gGlobal.AlphaOffsetTable[0];
		UInt32			AlphaOffset = m_AlphaOffset;

		__asm
		{
			mov			esi, pSrcData
			mov			edi, pDesData
			mov			edx, pAlphaTable

			mov			ecx, SrcHeight

LOOP_L1_1:
				
			push		ecx
			mov			ecx, SrcWidth

LOOP_L1_2:
			mov			eax, DWORD PTR [esi]

			bswap		eax
			xor			ebx, ebx
			mov			bl, al
			shl			ebx, 8
			add			ebx, AlphaOffset
			mov			al, [edx][ebx]
			bswap		eax
			

			movd		mm0, eax
			movd		mm1, DWORD PTR [edi]
		
			pxor		mm2, mm2

			PUNPCKLBW	mm0, mm2
			PUNPCKLBW	mm1, mm2
			movq		mm3, mm0
			PUNPCKHWD	mm3, mm3
			PUNPCKHDQ	mm3, mm3

			movq		mm4, mm0
			movq		mm5, mm0

			pminsw		mm4, mm1
			pmaxsw		mm5, mm1

			psubw		mm0, mm1
			paddw		mm3, T1
			pmullw		mm0, mm3
			PSRLW		mm0, 8
			paddb		mm1, mm0

			pmullw		mm4, mm5
			PSRLW		mm4, 8
			paddusb		mm4, mm5

			PAND		mm4, T3
			PAND		mm1, T2
			por			mm1, mm4

			Packuswb	mm1, mm1
		
			movd		eax, mm1

			emms

			mov			DWORD PTR [edi], eax

			add			esi, PixelSizeSrc	
			add			edi, DesMovePixel

			loop		LOOP_L1_2

			add			esi, UpdateSrcLen	
			add			edi, UpdateDesLen

			pop			ecx
			loop		LOOP_L1_1
		}
	}
	else
	{
		/*
		for (UInt16 y = 0; y < SrcHeight; y++)
		{
			for (UInt16 x = 0; x < SrcWidth; x++)
			{				
				//BlitSinglePixel(pSrcData, pDesData);
				*((UInt32 *) (pDesData)) = MMXAlpha(*((UInt32 *) (pSrcData)), 
					*((UInt32 *) (pDesData)));

				pSrcData += PixelSizeSrc;
				pDesData += DesMovePixel;

			}
			pSrcData += UpdateSrcLen;
			pDesData += UpdateDesLen;

		}
		*/

		__asm
		{
			mov			esi, pSrcData
			mov			edi, pDesData

			mov			ecx, SrcHeight

LOOP_L2_1:
				
			push		ecx
			mov			ecx, SrcWidth

LOOP_L2_2:
			movd		mm0, DWORD PTR [esi]
			movd		mm1, DWORD PTR [edi]
		
			pxor		mm2, mm2

			PUNPCKLBW	mm0, mm2
			PUNPCKLBW	mm1, mm2
			movq		mm3, mm0
			PUNPCKHWD	mm3, mm3
			PUNPCKHDQ	mm3, mm3

			movq		mm4, mm0
			movq		mm5, mm0

			pminsw		mm4, mm1
			pmaxsw		mm5, mm1

			psubw		mm0, mm1
			paddw		mm3, T1
			pmullw		mm0, mm3
			PSRLW		mm0, 8
			paddb		mm1, mm0

			pmullw		mm4, mm5
			PSRLW		mm4, 8
			paddusb		mm4, mm5

			PAND		mm4, T3
			PAND		mm1, T2
			por			mm1, mm4

			Packuswb	mm1, mm1
		
			movd		eax, mm1

			emms

			mov			DWORD PTR [edi], eax

			add			esi, PixelSizeSrc	
			add			edi, DesMovePixel

			loop		LOOP_L2_2

			add			esi, UpdateSrcLen	
			add			edi, UpdateDesLen

			pop			ecx
			loop		LOOP_L2_1
		}
	}

}

VOID CRender::DrawFPS(String text)
{
	
}

VOID CRender::ClearData(Byte * pData, UInt Width, UInt Height, UInt PixelSize, RECT * ClearRect)
{
	
	if (ClearRect == NULL)
		ZeroMemory(pData, Width * Height * PixelSize);
	else
	{
		Byte * pSubData = pData + (ClearRect->top * Width + ClearRect->left) * PixelSize;
		UInt Len = (ClearRect->right - ClearRect->left ) * PixelSize;
		for (Int y = 0; y < ClearRect->bottom - ClearRect->top; y++)
		{
			ZeroMemory(pSubData, Len);
			pSubData += Width * PixelSize;
		}
	}
}

VOID CRender::LockTexture()
{
	D3DLOCKED_RECT d3dlt;
	//m_CombinedEntityTexture.Texture->LockRect(0, &d3dlt, 0, D3DLOCK_NO_DIRTY_UPDATE );
	m_CombinedEntityTexture.Surface->LockRect( &d3dlt, 0, D3DLOCK_NO_DIRTY_UPDATE );
	m_CombinedEntityTexture.pData = (Byte *)d3dlt.pBits;
}

VOID CRender::UnLockTexture()
{
	//m_CombinedEntityTexture.Texture->UnlockRect(0);
	m_CombinedEntityTexture.Surface->UnlockRect();
	m_CombinedEntityTexture.pData = NULL;
}

