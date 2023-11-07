#pragma once
#ifndef COMMON_CHAR_MAP_H
#define COMMON_CHAR_MAP_H

#include <vector>

//CONST Int CharPicOffsetY = 52; //provided by J!
//
//CONST UInt16 CanvasWidth = 64;
//CONST UInt16 CanvasHeight = 32;
//CONST Int BlockSizeX = 32;
//CONST Int BlockSizeY = 16;
//
//enum LoadingDataState
//{
//	DATA_LOADING = 0,
//	DATA_FINISH_LOADING,
//	DATA_READED,
//};
//
//enum MapPicType
//{
//	IPic = 0,
//	PPic,
//	DiffPic,
//	OrgPic,
//	RLEPPic,
//};
//
//enum BlitPicType
//{
//	RGBA4444 = 0,
//	RGB565,
//	RGBA8888,
//};
//
//enum PictureStorageType
//{
//	Color256 = 0,
//	HighQ32Bits,
//	HighQ16Bits,
//};
//
////effect type
//enum EffectType
//{
//	EffectCloud = 0,
//	EffectRain,
//	EffectSnow,
//	EffectLeaf,
//	EffectFog,
//	EffectDragon,
//};
//
////effect position type
//enum EffectPositionType
//{
//	InsideScreen = 0,
//	OutsideScreen,
//	AboveScreen,
//	BelowScreen,
//	RightOfScreen,
//	LeftOfScreen,
//};
//
////entity picture effect type
//enum PiCParticleType
//{
//	NoEffect = 0,
//	Additive,
//	Subtractive,
//	AdditiveBody,
//	SubtractiveBody,
//};
//
////frame rotation
//enum FrameRotation
//{
//	SOUTH = 0,
//	SOUTHEAST,
//	EAST,
//	NORTHEAST,
//	NORTH,
//	NORTHWEST,
//	WEST,
//	SOUTHWEST,
//	MAXROTATION,
//};
////entity animation type
//enum StateType
//{
//	ANIM_IDLE1 = 0,
//	ANIM_RUN,
//	ANIM_SIT,
//	ANIM_W1BAT,
//	ANIM_W1ATK1,
//	ANIM_W1ATK2,
//	ANIM_W2BAT,
//	ANIM_W2ATK1,
//	ANIM_W2ATK2,
//	ANIM_SKILL,
//	ANIM_HURT,
//	ANIM_DIE,
//	ANIM_DEF,
//	ANIM_USE,
//	ANIM_JUMP_START,
//	ANIM_JUMP,
//	ANIM_JUMP_END,
//	ANIM_IDLE2,
//	ANIM_WALK,
//	MAX_ANIM_TYPE,
//};
//
////entity image type
//enum ImageType
//{
//	ImagePic = 0,
//	ShadowPic,
//	BlockingPic,
//	AlphaPic,
//	MaskPic,
//	MaxImageType,
//};
//
//struct PixelDirection
//{
//	Real32 PixelX;
//	Real32 PixelY;
//	PixelDirection()
//	{
//		PixelX = 0;
//		PixelY = 0;
//	}
//};
//
////Data for a chain of animation
//struct animationsetData
//{
//	Int AnimationIndex;
//	Int UpX;
//	Int UpY;
//	Int InitialSpeed;
//	Int height;
//	Real64 TimeInMS;
//	BOOL Loop;
//	Int Rotation;
//	UInt32 StartTime;
//	UInt32 TimeToChange;
//	animationsetData()
//	{
//		AnimationIndex = 0;
//		UpX = 0;
//		UpY = 0;
//		InitialSpeed = 0;
//		height = 0;
//		TimeInMS = 0;
//		Rotation = 0;
//		Loop = TRUE;
//		StartTime = 0;
//		TimeToChange = 0;
//	}
//};
//
//struct BlockType // bit 0--- is StlMap Block//bit 1 --- Alpha Block// bit 2 --- ShadowBlock// bit 3 --- EntityBlock // bit 8 ~ 15 sea depth
//{
//	
//	UInt16 BlockData;
//
//
//	BlockType()
//	{
//		BlockData = 0;
//	}
//};
//
//struct BgLst
//{
//	LPDIRECT3DTEXTURE9 BgImage;
//	//StlVector<UInt32> DiffSubPicX;
//	//StlVector<UInt32> DiffSubPicY;
//	Byte * pSrcData;
//	BgLst()
//	{
//		BgImage = NULL;
//		pSrcData = NULL;
//	};
//};
//struct Position2D
//{
//	Int X;
//	Int Y;
//	Position2D * next;
//	Position2D()
//	{
//		 X = 0;
//		 Y = 0;
//		 next = NULL;
//	}
//};
//
//struct MaskEffect
//{
//	RECT Bound;
//	Byte * pData;
//	Byte * pSrcData;
//
//	MaskEffect()
//	{
//		pData = NULL;
//		pSrcData = NULL;
//		Bound.left = Bound.top = -1;
//		Bound.right = Bound.bottom = 0;
//	}
//};
//
#ifndef _QGL_H_
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

	bool operator < (const BlockPos& b) const
	{
		return (PosX < b.PosX || (PosX == b.PosX && PosY < b.PosY));
	}

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

typedef StlVector<BlockPos, ManagedAllocator<BlockPos> > BlockPosVector;

#endif // _QGL_H_
//
//struct PicPosData
//{
//	Int PicOffX;
//	Int PicOffY;
//	UInt16 PicPosMaxX;
//	UInt16 PicPosMaxY;
//	UInt16 PicPosMinX;
//	UInt16 PicPosMinY;
//	Byte TextureIndex;
//
//	PicPosData()
//	{
//		PicOffX = 0;
//		PicOffY = 0;
//		PicPosMaxX = 0;
//		PicPosMaxY = 0;
//		PicPosMinX = 0;
//		PicPosMinY = 0;
//		TextureIndex = 0;
//
//	}
//
//};
//
//struct AnimationFrameData
//{
//	UInt16 Layer;
//	Int Rotation;
//	Int Frame;
//	Int Delay;
//	Int Offset;
//	Int Disposal;
//	Int Scale;
//	Int StateType;
//	Byte hit;
//	
//	PicPosData ImagePicPos;
//	PicPosData ShadowPicPos;
//	Byte BlockingIndex;
//	Byte AlphaIndex;
//	Byte ShadowIndex;
//
//	AnimationFrameData()
//	{
//		StateType = 0;
//		Delay= 0;
//		BlockingIndex = 0;
//		AlphaIndex = 0;
//		ShadowIndex = 0;
//		hit = 0;
//	}
//	~AnimationFrameData()
//	{
//	}
//};
//
//struct MapData
//{
//	//StlMap size
//	UInt32 MapWidth;
//	UInt32 MapHeight;
//	
//	//small StlMap size
//	UInt16 SubMapWidth;
//	UInt16 SubMapHeight;
//
//	//total small StlMap pictures
//	UInt16 TotalPic;
//	
//	//maximum block size
//	UInt16 MaxBlockX;
//	UInt16 MaxBlockY;
//
//	//total Map frame (usually 4)
//	Int totalSubFrame;
//
//	//total Naight frame (usually 1)
//	Int totalNightFrame;
//
//	//store the block info in whole StlMap
//	StlVector<BlockType> BlockPosDataLst;
//
//	//store the alpha data in the StlMap
//	StlVector<MaskEffect> AlphaMaskLst;
//
//	//store the shadow data in the StlMap
//	StlVector<MaskEffect> ShadowMaskLst;
//
//	//StlMap picture info
//	StlVector<BgLst>		m_pTextureBGLst;
//	StlVector<BgLst>		m_pTextureNightLst;
//
//	StlVector< StlVector< Byte * > > BkAlphaIndexLstVec;
//	StlVector< StlVector< Byte * > > BkShadowIndexLstVec;
//
//	Byte * FileReadedData;
//
//	Byte State;
//	MapData()
//	{
//		MapWidth = 0;
//		MapHeight = 0;
//		SubMapWidth = 0;
//		SubMapHeight = 0;
//		TotalPic = 0;
//		MaxBlockX = 0;
//		MaxBlockY = 0;
//		totalSubFrame = 0; 
//		FileReadedData = NULL;
//		State = DATA_LOADING;
//		totalNightFrame = 0;
//	}
//
//};
//
//struct EntityTextureData
//{
//	Byte * pData;
//	WORD SizeX;
//	WORD SizeY;
//	WORD PixelSize;
//	Byte * ColorPaletteRef;
//
//	EntityTextureData()
//	{
//		pData = NULL;
//		SizeX = 0;
//		SizeY = 0;
//		PixelSize = 0;
//		ColorPaletteRef = NULL;
//	}
//};
//struct EntityPicInfo
//{
//	StlVector<EntityTextureData> m_BuffTextureLst;
//	StlMultiMap<Int, AnimationFrameData> m_EntityHeader;
//	StlVector<StlVector<BlockPos> > m_BlockingLst;
//	UInt16 SrcWidth;
//	UInt16 SrcHeight;
//	Byte PicType;
//	Byte State;
//	Byte * ColorPalette;
//	
//	EntityPicInfo()
//	{
//		SrcWidth = 0;
//		SrcHeight = 0;
//		ColorPalette = NULL;
//		PicType = 0;
//		State = DATA_LOADING;
//	}
//};
//
//
//struct RenderLstData
//{
//	PicPosData PicData;
//	EntityTextureData Image;
//	PicPosData ShadowPicData;
//	EntityTextureData ShadowImage;
//	Int EffectType;
//	Int BlockBkIndex;
//	Int AlphaBkIndex;
//	Int ShadowBkIndex;
//	Byte PicType;
//	BOOL Flip;
//	RenderLstData()
//	{
//		PicType = 0;
//		Flip = FALSE;
//	}
//
//};

#endif