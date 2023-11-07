#pragma once
#ifndef _QGL_H_
#define _QGL_H_

#include <d3dx9.h>		// Header File For DirectX 3D
#include <d3d9.h>		// Header File For DirectX 3D
#include "Common/DataCode.h"
#include "resource.h"
#include "Resource/GDFManager.h"

extern GDFManager* g_GdfResource;
extern GDFManager* g_GdfResourceEx;

#define CONFIGURATION_INI_FILE _T(".\\Config\\Setting.ini")
#define OUTPUT_FOLDER _T("ScreenCapture")
#define PICTURE_OUTPUT_FOLDER _T(".\\ScreenCapture\\QGLGameScreen")
#define PICTURE_OUTPUT_SUBWINDOW_FOLDER _T(".\\ScreenCapture\\QGLSubWindowScreen")
#define GDF_DATA_FILE _T(".\\Data\\Data.gdf")
#define GDF_DATAEX_FILE _T(".\\Data\\DataEx.gdf")


CONST Int CharPicOffsetY = 52 + 128; //provided by J!

CONST UInt16 CanvasWidth = 64;
CONST UInt16 CanvasHeight = 32;
CONST Int BlockSizeX = 32;
CONST Int BlockSizeY = 16;

CONST UInt32 DefaultWinWidth = 800;
CONST UInt32 DefaultWinHeight = 600;

CONST UInt32 NameExtraZ = 0xC0000000;
CONST UInt32 NightExtraZ  = 0xA0000000;
CONST UInt32 ParticleExtraZ = 0x40000000;
CONST UInt32 NumberExtraZ = 0x60000000;
CONST Int32  ObjExtraZ = 0xA0000000;

CONST WORD ENTITY_VERSION_NUMBER  = 10000;

enum LeftClickStatusType
{
	LEFTCLICK_NORMAL = 0,
	LEFTCLICK_DOWN,
	LEFTCLICK_AUTO,
};
enum LoadingDataState
{
	DATA_LOADING = 0,
	DATA_FINISH_LOADING_FIRST_BGMAP,
	DATA_FINISH_LOADING_BGMAP, //only used in loading map
	DATA_FINISH_LOADING,
	DATA_READED,
	DATA_DECOMPRESSING,
	DATA_DECOMPRESSED,
};

enum MapPicType
{
	IPic = 0,
	PPic,
	DiffPic,
	OrgPic,
	RLEPPic,
};

enum BlitPicType
{
	RGBA4444 = 0,
	RGB565,
	RGBA8888,
};

enum PictureStorageType
{
	Color256 = 0,
	HighQ32Bits,
	HighQ16Bits,
	Z_Color256,
	Z_HighQ32Bits,
	Z_HighQ16Bits,
};

//effect type
enum EffectType
{
	EffectCloud = 0,
	EffectRain,
	EffectSnow,
	EffectLeaf,
	EffectFog,
	EffectDragon,
};

//effect position type
enum EffectPositionType
{
	InsideScreen = 0,
	OutsideScreen,
	AboveScreen,
	BelowScreen,
	RightOfScreen,
	LeftOfScreen,
};

//entity picture effect type
enum PiCParticleType
{
	NoEffect = 0,
	Additive,
	Subtractive,
	AdditiveBody,
	SubtractiveBody,
};

//frame rotation
enum FrameRotation
{
	SOUTH = 0,
	SOUTHEAST,
	EAST,
	NORTHEAST,
	NORTH,
	NORTHWEST,
	WEST,
	SOUTHWEST,
	MAXROTATION,
};

//entity animation type
enum StateType
{
	ANIM_IDLE1 = 0,
	ANIM_RUN,
	ANIM_SIT,
	ANIM_W1BAT,
	ANIM_W1ATK1,
	ANIM_W1ATK2,
	ANIM_W2BAT,
	ANIM_W2ATK1,
	ANIM_W2ATK2,
	ANIM_SKILL,
	ANIM_HURT,
	ANIM_DIE,
	ANIM_DEF,
	ANIM_USE,
	ANIM_JUMP_START,
	ANIM_JUMP,
	ANIM_JUMP_END,
	ANIM_IDLE2,
	ANIM_WALK,
	ANIM_MOUNT_IDLE1,
	ANIM_MOUNT_RUN,
	ANIM_MOUNT_IDLE2,
	MAX_ANIM_TYPE,
};

//entity image type
enum ImageType
{
	ImagePic = 0,
	ShadowPic,
	BlockingPic,
	AlphaPic,
	MaskPic,
	MaxImageType,
};

#define QGHEADERDATA_BOM					0xFF000000
#define QGHEADERDATA_MAPGRAPH_VER				2

enum _QGMAP_FILE_HEADER
{
	FILEINFO_BGDATA = 0,
	FILEINFO_ALPHAINFO,
	FILEINFO_SHADOWINFO,
	FILEINFO_BLOCKINFO,
	FILEINFO_ALPHAINDEX,
	FILEINFO_SHADOWINDEX,
	MAX_FILEINFO_COUNT,
};


typedef struct _QGMapHeaderDataType
{
	Int Type;
	UInt64 FilePos;
	UInt64 DataSize;
	_QGMapHeaderDataType()
	{
		Type = 0;
		FilePos = 0;
		DataSize = 0;
	};
}QGMapHeaderDataType;

typedef struct _QGMapHeader
{
	UInt32 dwBOM;
	UInt16 wVersion;
	UInt32 dwMapWidth;
	UInt32 dwMapHeight;
	UInt16 wSubMapWidth;
	UInt16 wSubMapHeight;
	Int TotalDataNum;
	StlVector<QGMapHeaderDataType> DataLst;
	_QGMapHeader()
	{
		dwBOM = 0;
		wVersion= 0;
		dwMapWidth = 0;
		dwMapHeight = 0;
		wSubMapWidth = 0;
		wSubMapHeight = 0;
		TotalDataNum = 0;
		DataLst.clear();
	};

}QGMapHeader;

struct PixelDirection
{
	Real32 PixelX;
	Real32 PixelY;
	PixelDirection()
	{
		PixelX = 0;
		PixelY = 0;
	}
};

//Data for a chain of animation
struct animationsetData
{
	Int AnimationIndex;
	Int UpX;
	Int UpY;
	Int height;
	Real64 TimeInMS;
	BOOL Loop;
	Int Rotation;
	UInt32 StartTime;
	UInt32 JumpDelay;
	BOOL StopAnim;
	BOOL IsPixel;

	//sharking info
	Int ShakeX;
	Int ShakeY;
	Int ShakeSpeedX;
	Int ShakeSpeedY;
	UInt ShakeWidth;
	UInt ShakeHeight;
	UInt ShakeTime;

	animationsetData(Int GivenAnimationIndex, Int GivenUpX, Int GivenUpY, Int Givenheight, Real64 GivenTimeInMS,
	BOOL GivenLoop, Int GivenRotation, UInt32 GivenStartTime, UInt32 GivenJumpDelay, BOOL GivenStopAnim, 
	BOOL GivenIsPixel,//sharking info
	Int GivenShakeX, Int GivenShakeY, Int GivenShakeSpeedX, Int GivenShakeSpeedY, UInt GivenShakeWidth, UInt GivenShakeHeight,UInt GivenShakeTime)
	{
		AnimationIndex = GivenAnimationIndex;
		UpX = GivenUpX;
		UpY = GivenUpY;
		height = Givenheight;
		TimeInMS = GivenTimeInMS;
		Loop = GivenLoop;
		Rotation = GivenRotation;
		StartTime = GivenStartTime;
		JumpDelay = GivenJumpDelay;
		StopAnim = GivenStopAnim;
		IsPixel = GivenIsPixel;

		//sharking info
		ShakeX = GivenShakeX;
		ShakeY = GivenShakeY;
		ShakeSpeedX = GivenShakeSpeedX;
		ShakeSpeedY = GivenShakeSpeedY;
		ShakeWidth = GivenShakeWidth;
		ShakeHeight = GivenShakeHeight;
		ShakeTime = GivenShakeTime;

	}
	animationsetData()
	{
		Init();
	}

	void Init()
	{
		ZeroMemory(this, sizeof(animationsetData));
		AnimationIndex = -1;
		Rotation = -1;
		Loop = TRUE;
	}
};

struct BlockType // bit 0--- is StlMap Block//bit 1 --- Alpha Block// bit 2 --- ShadowBlock// bit 3 --- EntityBlock // bit 8 ~ 15 sea depth
{
	
	UInt16 BlockData;


	BlockType()
	{
		BlockData = 0;
	}
};

struct BgLst
{
	//LPDIRECT3DTEXTURE9 BgImage;
	Byte * pSrcData;
	Byte * pDDSPicData;

	BgLst()
	{
		//BgImage = NULL;
		pSrcData = NULL;
		pDDSPicData = NULL;
	};
};

struct NightBgLst
{
	LPDIRECT3DTEXTURE9 BgImage;
	Byte * pSrcData;
	//Byte * pDDSPicData;

	NightBgLst()
	{
		BgImage = NULL;
		pSrcData = NULL;
		//pDDSPicData = NULL;
	};
};
struct Position2D
{
	Int X;
	Int Y;
	Position2D * next;
	Position2D()
	{
		 X = 0;
		 Y = 0;
		 next = NULL;
	}
};

struct MaskEffect
{
	RECT Bound;
	Byte * pData;
	Byte * pSrcData;

	MaskEffect()
	{
		pData = NULL;
		pSrcData = NULL;
		Bound.left = Bound.top = -1;
		Bound.right = Bound.bottom = 0;
	}
};

struct BlockPos
{
	UInt16 PosX;
	UInt16 PosY;
	BlockPos()
	{
		PosX = 0;
		PosY = 0;
	}
	BlockPos operator-(BlockPos second)
	{
		BlockPos pos;
		pos.PosX = PosX - second.PosX;
		pos.PosY = PosY - second.PosY;
		return pos;
	};

	BOOL operator==(BlockPos second)
	{
		return PosX == second.PosX && 
			PosY == second.PosY;
	};

	BOOL operator!=(BlockPos second)
	{
		return PosX != second.PosX || 
			PosY != second.PosY;
	};

	BlockPos operator+(BlockPos second)
	{
		BlockPos pos;
		pos.PosX = PosX + second.PosX;
		pos.PosY = PosY + second.PosY;
		return pos;
	};

};

struct PixelPos
{
	Int PixelPosX;
	Int PixelPosY;

	PixelPos()
	{
		PixelPosX = 0;
		PixelPosY = 0;
	};

	PixelPos(Int X, Int Y)
	{
		PixelPosX = X;
		PixelPosY = Y;
	};

	BOOL operator==(PixelPos second)
	{
		return PixelPosX == second.PixelPosX && 
			PixelPosY == second.PixelPosY;
	};

	BOOL operator!=(PixelPos second)
	{
		return PixelPosX != second.PixelPosX || 
			PixelPosY != second.PixelPosY;
	};

	PixelPos operator+(PixelPos second)
	{
		PixelPos Result;
		Result.PixelPosX = PixelPosX + second.PixelPosX;
		Result.PixelPosY = PixelPosY + second.PixelPosY;
		return Result;
	}

	PixelPos operator-(PixelPos second)
	{
		PixelPos Result;
		Result.PixelPosX = PixelPosX - second.PixelPosX;
		Result.PixelPosY = PixelPosY - second.PixelPosY;
		return Result;
	}
};

struct PicPosData
{
	int PicOffX;
	int PicOffY;
	DWORD PicStartPos;
	DWORD PicEndPos;
	DWORD OrgSize;
	WORD OrgWidth;
	WORD OrgHeight;

	PicPosData()
	{
		ZeroMemory(this, sizeof(PicPosData));
	}

};


struct AnimationFrameData
{
	UInt16 Layer;
	Int Rotation;
	Int Frame;
	Int Delay;
	Int Offset;
	Int Disposal;
	Int Scale;
	Int StateType;
	Byte hit;
	
	PicPosData ImagePicPos;
	PicPosData ShadowPicPos;
	Byte BlockingIndex;
	Byte AlphaIndex;
	Byte ShadowIndex;

	AnimationFrameData()
	{
		StateType = 0;
		Delay= 0;
		BlockingIndex = 0;
		AlphaIndex = 0;
		ShadowIndex = 0;
		hit = 0;
	}
	~AnimationFrameData()
	{
	}
};

struct MapData
{
	QGMapHeader Header;

	//total small StlMap pictures
	UInt16 TotalPic;
	
	//maximum block size
	UInt16 MaxBlockX;
	UInt16 MaxBlockY;

	//total Map frame (usually 4)
	Int totalSubFrame;

	//total Naight frame (usually 1)
	Int totalNightFrame;

	//store the block info in whole StlMap
	StlVector<BlockType> BlockPosDataLst;

	//store the alpha data in the StlMap
	StlVector<MaskEffect> AlphaMaskLst;

	//store the shadow data in the StlMap
	StlVector<MaskEffect> ShadowMaskLst;

	//Map picture info
	BgLst *		m_pTextureBGLst;
	BgLst *		m_pTextureNightLst;

	StlVector< StlVector< Byte * > > BkAlphaIndexLstVec;
	StlVector< StlVector< Byte * > > BkShadowIndexLstVec;

	Byte * FileReadedData;

	LoadingDataState State;

	//indicate previous using time
	UInt32 PreInUseTime;

	//filename
	String FileName;

	//if res is locked, it won't be free
	BOOL Lock; 

	HANDLE hFile;
	HANDLE hMapFile;
	Byte * pInFile;

	StlVector<StlVector<RECT> > dirtyRectLst;
	MapData()
	{
		TotalPic = 0;
		MaxBlockX = 0;
		MaxBlockY = 0;
		totalSubFrame = 0; 
		FileReadedData = NULL;
		State = DATA_LOADING;
		totalNightFrame = 0;
		PreInUseTime = 0;
		m_pTextureBGLst = NULL;
		m_pTextureNightLst = NULL;
		Lock = FALSE;

		hFile = NULL;
		hMapFile = NULL;
		pInFile = NULL;
	}
	~MapData()
	{
		for (Int i = 0; i < (Int)dirtyRectLst.size(); i ++)
		{
			dirtyRectLst[i].clear();
		}
		dirtyRectLst.clear();
	}

};

struct EntityTextureData
{
	Byte * pData;
	WORD SizeX;
	WORD SizeY;
	WORD PixelSize;
	Byte * ColorPaletteRef;

	EntityTextureData()
	{
		pData = NULL;
		SizeX = 0;
		SizeY = 0;
		PixelSize = 0;
		ColorPaletteRef = NULL;
	}
};


struct ColorPaletteStruct
{
	Byte * ColorPaletteData;
	UInt32 PreInUseTime;

	ColorPaletteStruct()
	{
		ColorPaletteData = NULL;
		PreInUseTime = 0;
	}
};
struct EntityPicInfo
{
	//StlVector<EntityTextureData> m_BuffTextureLst;
	BYTE * m_PictureData;
	//StlMultiMap<Int, AnimationFrameData> m_EntityHeader;
	StlVector<AnimationFrameData> m_EntityHeader[MAX_ANIM_TYPE];
	StlVector<StlVector<BlockPos> > m_BlockingLst;
	UInt16 SrcWidth;
	UInt16 SrcHeight;
	Byte PicType;
	LoadingDataState State;
	Byte * ColorPalette;
	
	//indicate previous using time
	UInt32 PreInUseTime;

	//maximum size of the picture
	UInt32 MaxSize;
	StlMap<UInt32, BYTE *> m_UncompressedDataLst;

	//if res is locked, it won't be free
	BOOL Lock;

	HANDLE hFile;
	HANDLE hMapFile;
	Byte * pInFile;
	Byte * pInFileTemp;
	EntityPicInfo()
	{
		SrcWidth = 0;
		SrcHeight = 0;
		PicType = 0;
		State = DATA_LOADING;
		PreInUseTime = 0;
		ColorPalette = NULL;
		m_PictureData = NULL;
		MaxSize = 0;
		Lock = FALSE;
		hFile = NULL;
		hMapFile = NULL;
		pInFile = NULL;
		pInFileTemp = NULL;
	}
	~EntityPicInfo()
	{
		//m_EntityHeader.clear();
		m_BlockingLst.clear();
	}
};



struct LTFileHeader
{
	UInt32	m_Width;
	UInt32	m_Height;
	UInt32	m_nTileCount;
};


inline BOOL IsMoveKeyIsPressed(){return FALSE;};



inline VOID mem4set(VOID *d1, DWORD c, DWORD len)
{
	__asm
	{
		mov edi, d1

		mov ecx, len
		mov eax, c
		rep stosd
	}
}


inline VOID mem4cpy(VOID *d1, VOID *s1, DWORD len)
{
	__asm
	{
		mov edi, d1
		mov esi, s1

		mov ecx, len
		rep movsd
	}
}


inline VOID memfset(VOID *d1, DWORD c, DWORD len)
{
	__asm
	{
		mov edi, d1

		mov ecx, len
		mov eax, c
		rep stosw
	}
}

inline VOID memfcpy(VOID *d1, VOID *s1, DWORD len)
{
	__asm
	{
		mov edi, d1
		mov esi, s1
		
		mov ecx, len
		rep movsw
	}
}

#define SafeCloseLua(L)			\
{								\
	if(L != NULL)				\
		lua_close(L);			\
	L = NULL;					\
}

LRESULT	WINAPI WndProc(HWND, UInt, WPARAM, LPARAM);	// Declaration For WndProc
#endif // _COMMON_H_

