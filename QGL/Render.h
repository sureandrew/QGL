#pragma once
#ifndef RENDER_H_
#define RENDER_H_

class CAnimatedEntity;
#include "Camera.h"
#include "DirtyRectManager.h"
#include "MapObjectView.h"
#include "ParticleSystem.h"
#include "Font.h"

CONST Int ExtraMapBuffX = 100;
CONST Int ExtraMapBuffY = 200;
CONST Byte HighLightValue = 40;
CONST Int BgTextureSizeX = 1024;
CONST Int BgTextureSizeY = 1024;

enum MASKTEXTURETYPE
{
	MASKTEXTURETYPE_NO_EFFECT = 0,
	MASKTEXTURETYPE_FADE_IN,
	MASKTEXTURETYPE_FADE_OUT,
};

struct DrawEntityTextureData
{
	LPDIRECT3DTEXTURE9 Texture;
	LPDIRECT3DSURFACE9 Surface;
	Byte * pData;
	
	//dirty region for the entities
	StlMap<CEntityBase *, RECT> m_PreDirtyRegionLst;
	StlMap<CEntityBase *, RECT> m_NextDirtyRegionLst;

	//dirty rect manager for render
	CDirtyRectManager m_DirRectMan;
	RECT m_CurrDisArea;

	DrawEntityTextureData()
	{
		Texture = NULL;
		pData = NULL;
		Surface = NULL;
		SetRect(&m_CurrDisArea, 0, 0, 0, 0);
	}
};



struct MaskTextureData
{
	Byte Type;
	LPDIRECT3DTEXTURE9 MaskTexture;
	UInt32 StartTime;
	UInt32 FadeInOutTime;
	Byte Alpha;
	MaskTextureData()
	{
		memset(this, 0, sizeof(MaskTextureData));
	}
};
class CRender
{
	public:
	//constrtuct and distruct
	CRender(HINSTANCE hInstance, HWND	CurrWin, Int GivenWinWidth, Int GivenWinHeight, BOOL bFullScreen,
		BOOL CreateBg = TRUE);
	~CRender();
	//varable
	

protected:
	Int32					m_CursorX, m_CursorY;
	HDC						hDC;						// Private GDI Device Context
	HWND					hWnd;					// Holds Our Window Handle
	HINSTANCE				hD3DInstance;					// Holds The Instance Of The Application
	LPDIRECT3D9				pD3D;					// DirectX 3D Version 9
	LPDIRECT3DDEVICE9		pD3DDevice;				// DirectX 3D Rendering Device

	D3DPRESENT_PARAMETERS	m_PresentPara;		
	
	LPD3DXSPRITE			m_pSprite;			// D3DXSprite to draw 2d texture


	//system memory texture in window size
	DrawEntityTextureData m_CombinedEntityTexture;

	//window orginal size
	RECT m_OrgWinRect;

	//current window size
	Int m_CurrWinWidth;
	Int m_CurrWinHeight;

	//window mode rectangle
	RECT m_rcWindowBounds;

	CEntityBase * m_CurPosEntity;

	BOOL m_CreateBgTexture;


	D3DCAPS9 caps;

	
	BOOL	m_FullScnFlag;
	BOOL	m_IsSubWindow;
	
public:

	//get the cursor position entity
	inline CEntityBase * GetCurPosEntity(){return m_CurPosEntity;}
	//get the device
	inline LPDIRECT3DDEVICE9 GetDevice(){return pD3DDevice;};

	//get the windows size
	inline Int GetWinWidth() { return m_CurrWinWidth;};
	inline Int GetWinHeight() { return m_CurrWinHeight;};
	inline HWND GetWindow(){return hWnd;};

	inline void SetCursorPos(UInt32 CursorX, UInt32 CursorY)
	{
		m_CursorX = CursorX;
		m_CursorY = CursorY;
	}

	inline BOOL GetFullScnFlag(){return m_FullScnFlag;}
	inline void SetFullScnFlag(BOOL Flag){m_FullScnFlag = Flag;}

	inline Int32 GetCursorPosX(){ return m_CursorX;}
	inline Int32 GetCursorPosY(){ return m_CursorY;}
	
	inline LPDIRECT3DTEXTURE9 GetSystemCombinedTexture(){ return m_CombinedEntityTexture.Texture;}

	inline DrawEntityTextureData * GetSystemCombinedData(){ return &m_CombinedEntityTexture;}
	
	inline void ClearDirtyRectLst(){m_CombinedEntityTexture.m_DirRectMan.ClearDirtyRect();}

	inline RECT GetCurrDisRect(){return m_CombinedEntityTexture.m_CurrDisArea;}
	//VOID function
	//close the window
	VOID KillD3DScene();
	VOID KillD3DWindow();

	//init the require texture
	VOID Init();

	//resize the window
	VOID ReSizeD3DScene(Int width, Int height);				// Resize And Initialize The D3D Window

	HRESULT ResetDevice();
	
	//update the background texture to the system memory texture
	VOID UpdateWorldTextureBG(LPDIRECT3DTEXTURE9 SrcTexture, RECT SrcUpdateRect, 
		Int SrcMaxWidth, RECT LockRegion);

	VOID UpdateWorldDDSToBG(Byte * pData, RECT SrcUpdateRect,  RECT LockRegion, UInt SrcMaxWidth);
	
	//copy the texture
	VOID CopyTexture(LPDIRECT3DTEXTURE9 pDataSrc, Byte * pDataDes, Int DesMaxWidth, Int DesMaxHeight, 
						  Int DesPixelSize, RECT SrcCopyRect, RECT DesCopyRect);


	VOID CopyTexture(LPDIRECT3DTEXTURE9 pDataSrc, LPDIRECT3DTEXTURE9 pDataDes,
						   RECT SrcCopyRect, RECT DesCopyRect);

	//draw the StlMap data
	VOID DrawBgMap(CMapObjectView * pMap, RECT UpdateRect, BOOL Clear, CCamera * CurrCam);
	
	//begin and end the directx9 scene
	VOID BeginScene();
	VOID EndScene();

	//begin and end the directx9 sprite
	VOID BeginSprite();
	VOID EndSprite();

	//update the texture data from system to default which default texture
	BOOL UpDateTextureFromSysToDea();
	
	//draw the list of entity to the system texture
	VOID DrawEntityLstToWorld(BOOL DrawAll, StlMultiMap<UInt32, CEntityBase *> & EntRenderLst, CMapObjectView * pMap
		, CCamera * CurrCam, CEntityBase * pMaskEnt, BOOL AddEffect);

	//draw the list of entity to the given texture data
	VOID DrawEntityLstToTexture(BOOL DrawAll, StlMultiMap<UInt32, CEntityBase *> & EntRenderLst, 
		DrawEntityTextureData & GivenTextureData, BOOL Copy = FALSE);

	//update the scene if the camera move
	VOID CameraMoveDraw(CMapObjectView * pMap, StlMultiMap<UInt32, CEntityBase *> & EntRenderLst, CCamera * CurrCam,
		DrawEntityTextureData & NightMaskData);

	//draw the background StlMap in current dirty rect list
	VOID DrawBgMapByCurrDirtyRect(CMapObjectView * pMap, CCamera * CurrCam);

	//draw the Given texture in current dirty rect list
	VOID DrawGivenTextureByCurrDirtyRect(LPDIRECT3DTEXTURE9 GivenTexture, BOOL AlphaBlend, Byte AlphaOffset);

	//add all entity dirt rect to the list
	VOID AddEntityDirtyRect(StlMultiMap<UInt32, CEntityBase *> & EntRenderLst, CEntityBase * pMaskEnt, DrawEntityTextureData
		& TextureData);

	//add effect to the scene including raining, snowing, fog
	VOID AddPostEffectToMap(StlMultiMap<UInt32, CEntityBase *> & EntRenderLst);

	//add shadow to current entity
	VOID AddShadowToEntityLst(CEntityBase * Shadow, CEntityBase * Character, StlMultiMap<UInt32, CEntityBase *> & EntRenderLst);
	
	//draw extra texture by sprite
	VOID DrawTextureBySprite(LPDIRECT3DTEXTURE9 GivenTexture, Byte AlphaMask = 255);
	VOID DrawTextureBySprite(int X, int Y, LPDIRECT3DTEXTURE9 GivenTexture, Byte AlphaMask = 255);
	VOID DrawTextureBySprite(int X, int Y, RECT UpdateRect, LPDIRECT3DTEXTURE9 GivenTexture, Byte AlphaMask = 255);
	VOID DrawTextureBySpriteAndDirtyRect(LPDIRECT3DTEXTURE9 GivenTexture, CDirtyRectManager * pDirRectMan);
	VOID FreeVolatileResources();
	VOID InitVolatileResources();

	//update the mask texture
	VOID UpdateMaskTexture(UInt32 CurrTime);
	
	//boolean function
	BOOL DrawEntityToWorld(CEntityBase * pEnt,  
		RECT * LockRegion, CMapObjectView * pMap, CCamera * CurrCam, BOOL AddEffect);

	//boolean function
	BOOL DrawEntityToTexture(CEntityBase * pEnt, DrawEntityTextureData & GivenTextureData, RECT * LockRegion, BOOL Copy = FALSE);
	
	//create the directx window
	BOOL CreateD3DWindow( BOOL fullscreenflag);

	//check whether the entity need to be rendered
	BOOL IsEntityInsideRenderWindow(CEntityBase * pEnt, CCamera * CurrCam, RECT * ResultRect = NULL);

	//check whether the entity inside texture
	BOOL IsEntityInsideTexture(CEntityBase * pEntBase, DrawEntityTextureData & GivenTextureData, PixelPos LeftUp, RECT * ResultRect = NULL);

	//init the directx window
	Int InitD3D();

	//Find Entity InPreviusDirtyRect
	RECT FindDirtRectByEntity(CEntityBase * pEntBase);

	RECT FindNextDirtRectByEntity(CEntityBase * pEntBase);

	//set shadow effect for the entity
	VOID SetShadow(BlockPos m_CharPos, UInt16 DesMaxHeight, UInt16 DesMaxWidth, Byte * pResultMask, BOOL flip, CMapObjectView * pMap, RECT WorldMapRect);
	
	//set alpha effect for the entity by object
	VOID SetObjectBlockAlpha(BlockPos m_CharPos, Byte * pResultMask, CMapObjectView * pMap, RECT WorldMapRect, UInt32 SrcMaxWidth, BOOL Flip);

	//set shadow effect for the entity by Object
	VOID SetObjectBlockShadow(BlockPos m_CharPos, UInt16 DesMaxHeight, UInt16 DesMaxWidth, Byte * pResultMask, BOOL flip, RECT WorldLockRegion, CMapObjectView * pMap);

	//set alpha effect for the entity
	VOID SetAlpha(  BlockPos m_CharPos, UInt16 DesMaxHeight, UInt16 DesMaxWidth, 
						Byte * pResultMask, Byte * BgData, UInt16 BgWidth, CMapObjectView * pMap, RECT WorldMapRect);

	//set sea alpha effect for the entity
	VOID SetSeaAlpha(Byte * pSrcData, RECT SrcRect, Byte SeaDepth, UInt16 SrcMaxWidth, UInt16 offsetY, Byte * BgData, UInt16 BgWidth, RECT WinRect, CMapObjectView * pMap);
	
	//update the entity effect
	VOID UpdatePlayerEntEffectBeforeCombine(CEntityBase * pEnt, 
											  Byte * pSrcTData, D3DSURFACE_DESC  pDescDes, RECT LockRegion, 
											  CMapObjectView * pMap, RECT m_EntityTotalRect, BOOL Flip );

	//select entity in window
	VOID SelectEntity(Int PosX, Int PosY);

	//change window / fullscreen mode
	VOID ToggleFullscreen();

	VOID AddCursorDirtyRect();

	VOID EnableAlphaBlend();
	VOID DisableAlphaBlend();

	VOID UpdateCameraMove(CMapObjectView * pMap, CCamera * CurrCam);
	VOID AddMapDirtyRect(CMapObjectView * pMap, CCamera * CurrCam);

	BOOL GetAlphaMask( BlockPos m_CharPos, Byte * pResultMask, CMapObjectView * pMap, RECT WorldMapRect, UInt32 SrcMaxWidth, BOOL Flip);

	VOID DrawFPS(String text);
	VOID UpDateTextureToWindowBuffer(LPDIRECT3DTEXTURE9 SrcTexture, RECT SrcRect, RECT DesRect);

	bool IsFullScreen();

	inline LPD3DXSPRITE GetSprite(){return m_pSprite;}

	VOID OutputBackBufferToFile(String Path);

	VOID DrawNightMaskByPS(CMapObjectView * pMap, CCamera * CurrCam, PixelPos MaskCenter);
	VOID DrawNightMask(DrawEntityTextureData NightMaskData, CEntityBase * pMaskEnt, CCamera * CurrCam, CMapObjectView * pMap);


	inline RECT GetWinRect(){return m_OrgWinRect;};


	static VOID BlitPic(Byte * pSrcData, Int SrcStartIndex, Int32 SrcWidth, Int32 SrcHeight, UInt16 SrcMaxWidth,
					  Byte * pDesData, Int DesStartIndex,  UInt16 DesMaxWidth, BOOL flip, Byte m_AlphaOffset, 
					  Int32 PixelSizeSrc, Int32 PixelSizeDes);

	static VOID	BlitPicBW(Byte * pSrcData, Int SrcStartIndex, Int32 SrcWidth, Int32 SrcHeight, UInt16 SrcMaxWidth,
					  Byte * pDesData, Int DesStartIndex,  UInt16 DesMaxWidth, BOOL flip, Byte m_AlphaOffset, 
					  Byte PixelSizeSrc, Byte PixelSizeDes);

	//assume des has enough space to decompress
	static VOID DXT1To32Bits(Byte * pDXT1Data, Byte * pDesData, RECT CopyRect, UInt32 DesWidth);

	static VOID DXT3To32Bits(Byte * pDXT3Data, Byte * pDesData, RECT CopyRect, UInt32 DesWidth);

	static VOID DXT3To32BitsAlpha(Byte * pDXT3Data, Byte * pDesData, RECT CopyRect, UInt32 DesWidth);

	static VOID DXT3To32BitsAlphaMask(Byte * pDXT3Data, Byte * pDesData, RECT CopyRect, UInt32 DesWidth, Byte * pMaskData);

	static VOID Copy565To8888(Byte * pSrcData, Int SrcStartIndex, Int32 SrcWidth, Int32 SrcHeight, UInt16 SrcMaxWidth,
		Byte * pDesData, Int DesStartIndex, UInt16 DesMaxWidth);

	static VOID Copy8888To8888(Byte * pSrcData, Int SrcStartIndex, Int32 SrcWidth, Int32 SrcHeight, UInt16 SrcMaxWidth,
		Byte * pDesData, Int DesStartIndex, UInt16 DesMaxWidth);

	//normal alpha blit
	static VOID BlitPicWithEffect(Byte * pSrcData, Int SrcStartIndex, Int32 SrcWidth, Int32 SrcHeight, UInt16 SrcMaxWidth,
					  Byte * pDesData, Int DesStartIndex,  UInt16 DesMaxWidth, BOOL flip, Byte m_AlphaOffset, 
					  Byte PixelSizeSrc, Byte PixelSizeDes, BOOL AlphaEffect,
					  Byte * pBgData);

	//alpha mask blit with StlMap data
	static VOID BlitAlphaPic(Byte * pSrcData, UInt32 SrcStartIndex, UInt16 SrcWidth, UInt16 SrcHeight, UInt16 SrcMaxWidth,
		Byte * pDesData, UInt32 DesStartIndex, UInt16 DesMaxWidth, BOOL IsObject
		, Byte * BgData, UInt16 BgWidth, UInt32 BgStartIndex, Byte PixelSize);

	static VOID BlitAlphaMapPic(Byte * pSrcData, UInt32 SrcStartIndex, UInt16 SrcWidth, UInt16 SrcHeight, UInt16 SrcMaxWidth,
						  Byte * pDesData, UInt32 DesStartIndex, UInt16 DesMaxWidth
						  , Byte * BgData, UInt16 BgWidth, UInt32 BgStartIndex);

	//subtractive blit
	static VOID BlitSubtractivePic(Byte * pSrcData, UInt32 SrcStartIndex, UInt16 SrcWidth, UInt16 SrcHeight, UInt16 SrcMaxWidth,
		Byte * pDesData, UInt32 DesStartIndex, UInt16 DesMaxWidth, BOOL flip, BOOL IsObject
		, Byte PixelSize);

	//additive blit
	static VOID BlitAdditivePic(Byte * pSrcData, UInt32 SrcStartIndex, UInt16 SrcWidth, UInt16 SrcHeight, UInt16 SrcMaxWidth,
		Byte * pDesData, UInt32 DesStartIndex,  UInt16 DesMaxWidth, BOOL flip, Byte PixelSize);

	//sea alpha blit
	static VOID BlitSeaAlphaPic(Byte * pSrcData, UInt32 SrcStartIndex, UInt16 SrcWidth, UInt16 SrcHeight, UInt16 SrcMaxWidth, Byte * BgData, UInt16 BgWidth, UInt32 BgStartIndex, Byte PixelSize);

	static BOOL LockSystemTexture(D3DSURFACE_DESC  & pDesc, D3DLOCKED_RECT & d3dlt, 
								 RECT * LockRect, LPDIRECT3DTEXTURE9 SysTexture, DWORD Flags = D3DLOCK_NO_DIRTY_UPDATE);
	//MMX alpha blending
	static inline UInt32 __fastcall MMXAlpha(UInt32 pSrcData, UInt32 pDesData)
	{
		const __int64		T1 = 0x0001000100010001;
		const __int64		T2 = 0x0000FFFFFFFFFFFF;
		const __int64		T3 = 0xFFFF000000000000;

		__asm
		{
			movd		mm0, pSrcData
			movd		mm1, pDesData
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

			movd	pDesData, mm1

			emms
		}

		return pDesData;

	}


	static inline UInt32 __fastcall MMXAlphaBW(UInt32 pSrcData, UInt32 pDesData)
	{
		const __int64		T1 = 0x0001000100010001;
		const __int64		T2 = 0x0000FFFFFFFFFFFF;
		const __int64		T3 = 0xFFFF000000000000;
		const __int64		T4 = 85;

		__asm
		{
			movd		mm0, pSrcData
			movd		mm1, pDesData
			pxor		mm2, mm2

			PUNPCKLBW	mm0, mm2
			PUNPCKLBW	mm1, mm2
			movq		mm3, mm0
			PUNPCKHWD	mm3, mm3
			PUNPCKHDQ	mm3, mm3

			movq		mm4, mm0
			movq		mm5, mm0
			PSRLQ		mm5, 16
			paddw		mm4, mm5
			PSRLQ		mm5, 16
			paddw		mm4, mm5
			pmullw		mm4, T4
			PSRLD		mm4, 8

			PUNPCKLWD	mm4, mm4
			PUNPCKLDQ	mm4, mm4
			PAND		mm4, T2
			PAND		mm0, T3
			POR			mm0, mm4


			movq		mm4, mm0
			movq		mm5, mm1


			pminsw		mm4, mm1
			pmaxsw		mm5, mm1


			psubw		mm0, mm1
			paddw		mm0, T1
			pmullw		mm0, mm3
			PSRLW		mm0, 8
			paddb		mm1, mm0

			paddw		mm4, T1
			pmullw		mm4, mm5
			PSRLW		mm4, 8
			paddusb		mm4, mm5

			PAND		mm4, T3
			PAND		mm1, T2
			por			mm1, mm4

			Packuswb	mm1, mm1

			movd	pDesData, mm1

			emms
		}

		return pDesData;

	}

	//single pixel alpha blending
	static inline VOID __fastcall BlitSinglePixel(Byte * pSrcData, Byte * pDesData)

	{
		UInt32 dwSA = pSrcData[ 3];

		UInt32 dwDA = pDesData[ 3];
		UInt32 * dwSrcData = (UInt32 *)(pSrcData);
		if (dwSA  == 255 || (dwDA == 0 && dwSA > 0))
			memcpy(pDesData, pSrcData, sizeof(Color32));
		else if (pSrcData[ 3] > 0)
		{
			// Extract all channels of source and dest to those temp variable containers

			UInt32 dwSR = pSrcData[0];
			UInt32 dwDR = pDesData[0];
			UInt32 dwSG = pSrcData[ 1];
			UInt32 dwDG = pDesData[ 1];
			UInt32 dwSB = pSrcData[ 2];
			UInt32 dwDB = pDesData[ 2];

			// Calculate the alpha blending for all channels
			dwDA = ((dwSA - dwDA) >> 8) + dwDA;
			dwDR = (dwSA * (dwSR - dwDR) >> 8) + dwDR;
			dwDG = (dwSA * (dwSG - dwDG) >> 8) + dwDG;
			dwDB = (dwSA * (dwSB - dwDB) >> 8) + dwDB;

			// Combine all channel back to result and aassign back to dest buffer

			pDesData[0] = (Byte)dwDR;
			pDesData[ 1] = (Byte)dwDG;
			pDesData[ 2] = (Byte)dwDB;
			pDesData[ 3] = (Byte)dwDA;// - m_AlphaOffset;


		}

	}

	//add windows size dirty rect to manager
	VOID AddAllDirtyRect();

	//add a RECT to manager
	VOID AddDirtyRectToMan(RECT GivenRect);

	//check the device
	BOOL IsDeviceValide();

	void SetSubWindow(BOOL Flag){ m_IsSubWindow = Flag; }
	BOOL IsSubWindow(){return m_IsSubWindow; };

	VOID ClearTexture(LPDIRECT3DTEXTURE9 pTexture, RECT * ClearRect);
	VOID ClearData(Byte * pData, UInt Width, UInt Height, UInt PixelSize, RECT * ClearRect);

	inline CDirtyRectManager * GetDirtyRectManager(){return &m_CombinedEntityTexture.m_DirRectMan;}

	VOID LockTexture();

	VOID UnLockTexture();

};
#endif