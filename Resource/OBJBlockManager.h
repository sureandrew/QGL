#pragma once
#ifndef _OBJBLOCK_MANAGER_H_
#define _OBJBLOCK_MANAGER_H_

#include "common_char_map.h"

class COBJBlockManager
{
public:
	COBJBlockManager();
	~COBJBlockManager();

	BlockPosVector* Load(const TCHAR *filename, Int state);
	BlockPosVector* LoadEnt(const TCHAR *filename, Int state);
	BOOL Free(const TCHAR *filename);

private:
	enum PictureStorageType
	{
		Color256 = 0,
		HighQ32Bits,
		HighQ16Bits,
	};

	struct PicPosData
	{
		Int PicOffX;
		Int PicOffY;
		UInt16 PicPosMaxX;
		UInt16 PicPosMaxY;
		UInt16 PicPosMinX;
		UInt16 PicPosMinY;
		Byte TextureIndex;

		PicPosData()
		{
			PicOffX = 0;
			PicOffY = 0;
			PicPosMaxX = 0;
			PicPosMaxY = 0;
			PicPosMinX = 0;
			PicPosMinY = 0;
			TextureIndex = 0;

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
	struct EntityPicInfo
	{
		StlVector<EntityTextureData, ManagedAllocator<EntityTextureData> > m_BuffTextureLst;
		StlMultiMap<Int, AnimationFrameData> m_EntityHeader;
		StlVector<BlockPosVector, ManagedAllocator<BlockPosVector> > m_BlockingLst;
		UInt16 SrcWidth;
		UInt16 SrcHeight;
		Byte PicType;
		Byte State;
		Byte * ColorPalette;
		
		EntityPicInfo()
		{
			SrcWidth = 0;
			SrcHeight = 0;
			ColorPalette = NULL;
			PicType = 0;
			State = 0;
		}
	};

	// Required Parameter for OBJ reading
	struct EntityData
	{
		EntityPicInfo * EntPic;
		DWORD CurrFrIndex;
		AnimationFrameData PreFrData;
		DWORD PreTime;
		EntityData()
		{
			CurrFrIndex = 0;
			PreTime= 0;
			EntPic = NULL;
		}
	};

	VOID ImportOBJEnt(const TCHAR *filename);
	VOID ImportOBJBin(const TCHAR *filename);
	void ReadRLEFile(BYTE* PicInfoBase, DWORD iFilesize, BYTE* RLEPic, DWORD RLEFileSize, DWORD PixelSize);

	typedef StlMap<Int8, BlockPosVector, std::less<Int8>, 
		ManagedAllocator<std::pair<Int8, BlockPosVector> > > ObjBlockMap;
	typedef StlMap<String, ObjBlockMap, std::less<String>, 
		ManagedAllocator<std::pair<Int8, ObjBlockMap> > > ObjFileMap;
	typedef StlMap<String, Int32, std::less<String>, 
		ManagedAllocator<std::pair<Int8, Int32> > > ReservedSizeMap;
	ObjFileMap		m_objBlockDataList; // Map of (Filename, <state, block>)
	ReservedSizeMap	m_reservedSizeList;
};

#endif // _OBJBLOCK_MANAGER_H_