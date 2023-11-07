//-- Common
#include "Common.h"
#include "Common/common_binary.h"
//-- Self
#include "OBJBlockManager.h"
//-- Library
#include "Reuben/Platform/BuffReader.h"
#include <fstream>
#include <iostream>
#include "OBJBlockManager.h"

#define DATA_OBJ_DIRECTORY			_T("Data/Map/OBJ/")

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

COBJBlockManager::COBJBlockManager()
{
	m_objBlockDataList.clear();
	m_reservedSizeList.clear();;
}

COBJBlockManager::~COBJBlockManager()
{
	for(ObjFileMap::iterator itr = m_objBlockDataList.begin(); itr!=m_objBlockDataList.end();
		++itr)
	{
		for(ObjBlockMap::iterator itr2 = itr->second.begin(); itr2!=itr->second.end(); itr2++ )
		{
			itr2->second.clear();
		}
		itr->second.clear();
	}
	m_objBlockDataList.clear();
	m_reservedSizeList.clear();
}

BlockPosVector* COBJBlockManager::Load(const TCHAR *filename, Int state)
{
	String fullfilename;
	fullfilename.Format(_T("%s%s"), DATA_OBJ_DIRECTORY, filename);

	ObjFileMap::iterator itr = m_objBlockDataList.find(fullfilename); 
	if( itr!=m_objBlockDataList.end() )
	{
		ObjBlockMap * tmpmap = &itr->second;
		ObjBlockMap::iterator itr2 = tmpmap->find((Int8)state);
		if( itr2!=tmpmap->end() )
		{
			ReservedSizeMap::iterator itr3 = m_reservedSizeList.find(fullfilename); 
			if( itr3!=m_reservedSizeList.end() )
			{
				itr3->second++;
				return &itr2->second;
			}
		}
		return NULL;
	}
	// Load this file to database
	ImportOBJBin(fullfilename.c_str());

	itr = m_objBlockDataList.find(fullfilename); 
	if( itr!=m_objBlockDataList.end() )
	{
		ObjBlockMap * tmpmap = &itr->second;
		ObjBlockMap::iterator itr2 = tmpmap->find((Int8)state);
		if( itr2!=tmpmap->end() )
		{
			ReservedSizeMap::iterator itr3 = m_reservedSizeList.find(fullfilename); 
			if( itr3!=m_reservedSizeList.end() )
			{
				itr3->second++;
				return &itr2->second;
			}
		}
		return NULL;
	}

	return NULL;
}

BlockPosVector* COBJBlockManager::LoadEnt(const TCHAR *filename, Int state)
{
	String fullfilename;
	fullfilename.Format(_T("%s%s"), DATA_OBJ_DIRECTORY, filename);

	ObjFileMap::iterator itr = m_objBlockDataList.find(fullfilename); 
	if( itr!=m_objBlockDataList.end() )
	{
		ObjBlockMap * tmpmap = &itr->second;
		ObjBlockMap::iterator itr2 = tmpmap->find((Int8)state);
		if( itr2!=tmpmap->end() )
		{
			ReservedSizeMap::iterator itr3 = m_reservedSizeList.find(fullfilename); 
			if( itr3!=m_reservedSizeList.end() )
			{
				itr3->second++;
				return &itr2->second;
			}
		}
		return NULL;
	}
	
	// Load this file to database
	ImportOBJEnt(fullfilename.c_str());
	
	itr = m_objBlockDataList.find(fullfilename); 
	if( itr!=m_objBlockDataList.end() )
	{
		ObjBlockMap * tmpmap = &itr->second;
		ObjBlockMap::iterator itr2 = tmpmap->find((Int8)state);
		if( itr2!=tmpmap->end() )
		{
			ReservedSizeMap::iterator itr3 = m_reservedSizeList.find(fullfilename); 
			if( itr3!=m_reservedSizeList.end() )
			{
				itr3->second++;
				return &itr2->second;
			}
		}
		return NULL;
	}

	return NULL;
}

BOOL COBJBlockManager::Free(const TCHAR *filename)
{
	ObjFileMap::iterator itr = m_objBlockDataList.find(filename); 
	if( itr!=m_objBlockDataList.end() )
	{
		ReservedSizeMap::iterator itr3 = m_reservedSizeList.find(filename); 
		if( itr3!=m_reservedSizeList.end() )
		{
			itr3->second--;
			if( itr3->second<=0 )
			{
				itr->second.clear();
				m_objBlockDataList.erase(filename);
				m_reservedSizeList.erase(filename);
			}
			return TRUE;
		}
	}
	return FALSE;
}

VOID COBJBlockManager::ImportOBJEnt(const TCHAR *filename)
{
	const int ColorPaletteSize = 768;
	WORD								EntOrgWidth;
	WORD								EntOrgHeight;
	std::vector<EntityData>				m_EnData;

	EntOrgWidth = 0;
	EntOrgHeight = 0;

	EntityData* m_EntityPicInfo = SafeCreateObject(EntityData);
	m_EntityPicInfo->CurrFrIndex = 0;
	m_EntityPicInfo->PreTime = ::GetTickCount();

	std::ifstream fp;
	int len = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
	char* ansifilename = SafeAllocate(char, len);
	WideCharToMultiByte(CP_ACP, 0, filename, -1, ansifilename, len, NULL, NULL);
	ansifilename[len] = 0;
	fp.open(ansifilename, std::ios::binary);
	bool firstTime = true;
	if (fp.is_open())
	{
		m_EntityPicInfo->EntPic = new EntityPicInfo;

		fp.read((char*)(&m_EntityPicInfo->EntPic->SrcWidth), sizeof(WORD));
		fp.read((char*)(&m_EntityPicInfo->EntPic->SrcHeight), sizeof(WORD));
		//EntOrgWidth = m_EntityPicInfo->EntPic->SrcWidth;
		//EntOrgHeight = m_EntityPicInfo->EntPic->SrcHeight;

		WORD noOfFrame = 0;
		WORD CurrFrame = 1;
		fp.read((char*)(&noOfFrame), sizeof(WORD));
		while (fp.good() && CurrFrame <= noOfFrame)
		{
			int StateType = 0;
			fp.read((char*)&StateType,sizeof(int));

			WORD AniFrCount = 0;
			WORD CurrAniFrCount = 1;

			fp.read((char*)&AniFrCount, sizeof(WORD));

			while(CurrAniFrCount <= AniFrCount)
			{
				AnimationFrameData FData;
				fp.read((char*)&FData, sizeof(AnimationFrameData));
				
				m_EntityPicInfo->EntPic->m_EntityHeader.insert(std::make_pair(StateType, FData));
				CurrAniFrCount++;
			}
			CurrFrame++;
		}

		WORD noOfTexture = 0;
		WORD CurrnoOfTexture = 1;
		
		fp.read((char*)(&m_EntityPicInfo->EntPic->PicType), sizeof(BYTE));
		fp.read((char*)&noOfTexture, sizeof(WORD));
		if (m_EntityPicInfo->EntPic->PicType == HighQ32Bits)
		{
			while (CurrnoOfTexture <= noOfTexture)
			{
				WORD width = 0;
				WORD height = 0;
				fp.read((char*)&width, sizeof(WORD));
				fp.read((char*)&height, sizeof(WORD));
				DWORD iFileSize = 0;
				fp.read((char*)&iFileSize, sizeof(DWORD));
				BYTE* PicInfoBase = new BYTE[iFileSize];

				fp.read((char*)PicInfoBase, iFileSize);

				if (PicInfoBase)
					delete [] PicInfoBase;
				CurrnoOfTexture ++;
			}
		}
		else if (m_EntityPicInfo->EntPic->PicType == Color256)
		{
			m_EntityPicInfo->EntPic->ColorPalette = new BYTE[ColorPaletteSize];
			fp.read((char*)(m_EntityPicInfo->EntPic->ColorPalette), ColorPaletteSize);
			while (CurrnoOfTexture <= noOfTexture)
			{
				WORD width = 0;
				WORD height = 0;
				fp.read((char*)&width, sizeof(WORD));
				fp.read((char*)&height, sizeof(WORD));

//				m_EntityPicInfo->EntPic->m_PictureWidthLst.push_back(width);
//				m_EntityPicInfo->EntPic->m_PictureHeightLst.push_back(height);
				DWORD iFileSize = 0;
				fp.read((char*)&iFileSize, sizeof(DWORD));
				BYTE* RLEPicInfoBase = new BYTE[iFileSize];
				fp.read((char*)RLEPicInfoBase, iFileSize);
				BYTE* PicInfoBase = new BYTE [width * height * 2];
				ReadRLEFile(PicInfoBase, width * height * 2, RLEPicInfoBase, iFileSize, 2);
				//
				//convert it to 32 bits picture
					
				BYTE * pDesTData = SafeAllocate(BYTE, height*width*sizeof(UINT));
				BYTE * ColorPalette = m_EntityPicInfo->EntPic->ColorPalette;
				BYTE * ColorPaletteIndex = PicInfoBase;
				//
				for (int y = 0; y < height; y++)
				{
					for (int x = 0; x < width ; x++)
					{
						memcpy(pDesTData, ColorPalette + ColorPaletteIndex[0] * 3, 3);
						pDesTData[3] = ColorPaletteIndex[1];

						pDesTData += sizeof(UINT); // __int32 Color32;
						ColorPaletteIndex += 2;
					}
				}
				SafeDeallocate(pDesTData);

//				m_EntityPicInfo->EntPic->m_PictureDataLst.push_back(PicInfoBase);

				if ( RLEPicInfoBase )
					delete [] RLEPicInfoBase;
				CurrnoOfTexture ++;
			}			
		}
		else if (m_EntityPicInfo->EntPic->PicType == HighQ16Bits)
		{
			while (CurrnoOfTexture <= noOfTexture)
			{
				WORD width = 0;
				WORD height = 0;
				fp.read((char*)&width, sizeof(WORD));
				fp.read((char*)&height, sizeof(WORD));

//				m_EntityPicInfo->EntPic->m_PictureWidthLst.push_back(width);
//				m_EntityPicInfo->EntPic->m_PictureHeightLst.push_back(height);
				DWORD iFileSize = 0;
				fp.read((char*)&iFileSize, sizeof(DWORD));
				BYTE* RLEPicInfoBase = new BYTE[iFileSize];
				fp.read((char*)RLEPicInfoBase, iFileSize);
				BYTE* PicInfoBase = new BYTE [width * height * 2];
				ReadRLEFile(PicInfoBase, width * height * 2, RLEPicInfoBase, iFileSize, 2);
				//

				BYTE * pDesTData = SafeAllocate(BYTE, height*width*sizeof(UINT));
				BYTE * PictureData = PicInfoBase;

				for (int y = 0; y < height; y++)
				{
					for (int x = 0; x < width ; x++)
					{
						pDesTData[0] = PictureData[0] & 0xF0;
						pDesTData[ 1] =(PictureData[0] & 0x0F) << 4;
						pDesTData[ 2] = PictureData[ 1] & 0xF0;
						pDesTData[ 3] = (PictureData[ 1] & 0x0F) << 4;

						pDesTData += sizeof(UINT);// __int32 Color32;
						PictureData += 2;
					}
				}

				SafeDeallocate(pDesTData);

				if ( PicInfoBase )
					delete [] PicInfoBase;
				if ( RLEPicInfoBase )
				delete [] RLEPicInfoBase;
				CurrnoOfTexture ++;
			}			
		}
		WORD noOfBkLst = 0;
		WORD CurrnoOfBkLst = 1;

		fp.read((char*)&noOfBkLst, sizeof(WORD));
		while (CurrnoOfBkLst <= noOfBkLst)
		{
			BlockPosVector BkPosLst;
			WORD noOfBk = 0;
			WORD CurrnoOfBk = 1;
			fp.read((char*)&noOfBk, sizeof(WORD));
			while (CurrnoOfBk <= noOfBk)
			{
				BlockPos pos;
				fp.read((char*)&pos, sizeof(BlockPos));
				BkPosLst.push_back(pos);
				CurrnoOfBk++;
			}
			m_EntityPicInfo->EntPic->m_BlockingLst.push_back(BkPosLst);
			CurrnoOfBkLst++;
		}
	
	fp.close();
	}
	SafeDeallocate(ansifilename);

	//m_EnData.push_back(m_EntityPicInfo);
	if ( m_EntityPicInfo->EntPic )
	{
//		m_EnData.push_back(m_EntityPicInfo);

		if (m_EntityPicInfo->EntPic->SrcWidth > EntOrgWidth)
			EntOrgWidth = m_EntityPicInfo->EntPic->SrcWidth;
		if (m_EntityPicInfo->EntPic->SrcHeight > EntOrgHeight)
			EntOrgHeight = m_EntityPicInfo->EntPic->SrcHeight;

		// add to list
		std::multimap<Int, AnimationFrameData>::iterator itr_ani = m_EntityPicInfo->EntPic->m_EntityHeader.begin();
		while( itr_ani!=m_EntityPicInfo->EntPic->m_EntityHeader.end() )
		{
			Byte idx = itr_ani->second.BlockingIndex;
			if( m_EntityPicInfo->EntPic->m_BlockingLst.size() < idx )
				throw;

			Int statetype = itr_ani->second.StateType;

			ObjFileMap::iterator itr = m_objBlockDataList.find(filename); 
			if( itr!=m_objBlockDataList.end() )
			{
				ObjBlockMap tmpmap = itr->second;
				ObjBlockMap::iterator itr2 = tmpmap.find((Int8)statetype);
				if( itr2==tmpmap.end() )
				{
					tmpmap.insert( std::make_pair((Int8)statetype, m_EntityPicInfo->EntPic->m_BlockingLst[idx]) );
					m_reservedSizeList.insert(std::make_pair(filename, 0));
				}
			}else
			{
				ObjBlockMap tmpfileblock;
				tmpfileblock.insert( std::make_pair((Int8)statetype, m_EntityPicInfo->EntPic->m_BlockingLst[idx]) );
				m_objBlockDataList.insert( std::make_pair(filename, tmpfileblock) );
				m_reservedSizeList.insert(std::make_pair(filename, 0));
			}
            itr_ani++;
		}
	}
}

VOID COBJBlockManager::ImportOBJBin(const TCHAR *filename)
{
	File file;
	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			BinaryBlockInfoHeader header;
			file.Read((BYTE*)&header, sizeof(BinaryBlockInfoHeader));

			for( int j = 0; j<19; j++ )
			{
				Int8 statetype;
				file.Read( (BYTE*)&statetype, sizeof(Int8) );
				Int64 blocklen;
				file.Read( (BYTE*)&blocklen, sizeof(Int64) );

				BlockPosVector tempblock;
				tempblock.clear();
				BlockPos tt;
				for( int i = 0; i<blocklen; i++ )
				{
					file.Read( (BYTE*)&tt, sizeof(BlockPos) );
					tempblock.push_back(tt);
				}

				ObjFileMap::iterator itr = m_objBlockDataList.find(filename); 
				if( itr!=m_objBlockDataList.end() )
				{
					ObjBlockMap tmpmap = itr->second;
					ObjBlockMap::iterator itr2 = tmpmap.find((Int8)statetype);
					if( itr2==tmpmap.end() )
					{
						tmpmap.insert( std::make_pair((Int8)statetype, tempblock) );
						m_reservedSizeList.insert(std::make_pair(filename, 0));
					}
				}else
				{
					ObjBlockMap tmpfileblock;
					tmpfileblock.insert( std::make_pair((Int8)statetype, tempblock) );
					m_objBlockDataList.insert( std::make_pair(filename, tmpfileblock) );
					m_reservedSizeList.insert(std::make_pair(filename, 0));
				}
			}
			file.Close();
			return;
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CServerFileManager::Load, open error, %s"), filename);
	}
}

void COBJBlockManager::ReadRLEFile(BYTE* PicInfoBase, DWORD iFilesize, BYTE* RLEPic, DWORD RLEFileSize,  
				 DWORD PixelSize)
{
	DWORD Count = 0;
	DWORD BaseCount = 0;
	while(Count < RLEFileSize && BaseCount < iFilesize)
	{
		BYTE relID = 0;
		memcpy(&relID, &RLEPic[Count], sizeof(BYTE));
		Count += sizeof(BYTE);

		if (relID > 127)
		{
			relID -= 127;
			for (int i= 0; i < relID; i++)
			{
				memcpy(&PicInfoBase[BaseCount] , &RLEPic[Count], PixelSize);
				BaseCount += PixelSize;
			}
			Count += PixelSize;
		}
		else
		{
			for (int i = 0; i < relID; i++)
			{
				memcpy(&PicInfoBase[BaseCount] , &RLEPic[Count], PixelSize);
				BaseCount += PixelSize;
				Count += PixelSize;
			}
		}
	}
}

