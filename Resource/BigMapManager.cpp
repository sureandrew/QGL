//-- Common
#include "Common.h"
//-- Self
#include "Resource/BigMapManager.h"
//-- Library
#include "Reuben/Platform/BuffReader.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResBigMap.h"

CBigMapManager::CBigMapManager()
{
	m_BigMap.clear();
	m_BigMapBelong.clear();
}

CBigMapManager::~CBigMapManager()
{
	Release();
}

void CBigMapManager::Release()
{
	for (BigMapDataMap::iterator itr = m_BigMap.begin(); itr != m_BigMap.end(); ++itr)
	{
		SafeDeleteObject(itr->second);
	}

	m_BigMap.clear();

	for (BigMapBelongDataMap::iterator itr = m_BigMapBelong.begin(); itr != m_BigMapBelong.end(); ++itr)
	{
		SafeDeleteObject(itr->second);
	}
	m_BigMapBelong.clear();
}

BOOL CBigMapManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{
	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(filename, true) )
		openSrcSucceeded = TRUE;
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: BigMapManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		BigMapData* pBigMapData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pBigMapData = SafeCreateObject(BigMapData);
				reader.Read(_T("map-id"),		pBigMapData->map_id);
				reader.ReadString(_T("map-name"),		pBigMapData->map_name, 19);
				reader.Read(_T("type"),			pBigMapData->map_type);
				reader.ReadString(_T("map-file"),		pBigMapData->map_file, 19);
				reader.ReadString(_T("image-file"),	pBigMapData->map_image_file, 19);
				reader.ReadString(_T("description"),	pBigMapData->description, 199);
				reader.Read(_T("button-x"),		pBigMapData->button_x);
				reader.Read(_T("button-y"),		pBigMapData->button_y);
				reader.Read(_T("adj-map1"),		pBigMapData->adj_map1);
				reader.Read(_T("adj-map2"),		pBigMapData->adj_map2);
				reader.Read(_T("adj-map3"),		pBigMapData->adj_map3);
				reader.Read(_T("adj-map4"),		pBigMapData->adj_map4);
				reader.Read(_T("adj-map5"),		pBigMapData->adj_map5);
				reader.Read(_T("adj-map6"),		pBigMapData->adj_map6);
				reader.Read(_T("adj-map7"),		pBigMapData->adj_map7);
				reader.Read(_T("adj-map8"),		pBigMapData->adj_map8);
				reader.Read(_T("adj-map9"),		pBigMapData->adj_map9);
				reader.Read(_T("adj-map10"),	pBigMapData->adj_map10);
				reader.Read(_T("adj-map11"),	pBigMapData->adj_map11);
				reader.Read(_T("adj-map12"),	pBigMapData->adj_map12);
				reader.Read(_T("flag-x"),		pBigMapData->flag_x);
				reader.Read(_T("flag-y"),		pBigMapData->flag_y);
				reader.Read(_T("use"),			pBigMapData->use);
				
				BigMapDataMap::iterator Itr = m_BigMap.find(pBigMapData->map_id);
				if (Itr != m_BigMap.end())
				{
					SafeDeleteObject(Itr->second);
					m_BigMap.erase(Itr);

					int index = 0;
					for (BigMapVector::iterator itr = m_BigMapVector.begin(); itr != m_BigMapVector.end(); ++itr)
					{
						if (m_BigMapVector[index]->map_id == pBigMapData->map_id)
						{
							m_BigMapVector.erase(itr);
							break;
						}
						++index;

					}
				}
				m_BigMap.insert(std::make_pair(pBigMapData->map_id, pBigMapData));
				m_BigMapVector.push_back(pBigMapData);
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: BigMapManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
	
}

BOOL CBigMapManager::loadBelongCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{
	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(filename, true) )
		openSrcSucceeded = TRUE;
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: BuffManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		BigMapBelongData* pBigMapBelongData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pBigMapBelongData = SafeCreateObject(BigMapBelongData);
				reader.Read(_T("map_id"),			pBigMapBelongData->srcMap_id);
				reader.ReadString(_T("map_name"),	pBigMapBelongData->map_name, 19);
				reader.Read(_T("within_map"),		pBigMapBelongData->belongMap_id);
				
				
				BigMapBelongDataMap::iterator Itr = m_BigMapBelong.find(pBigMapBelongData->srcMap_id);
				if (Itr != m_BigMapBelong.end())
				{
					SafeDeleteObject(Itr->second);
					m_BigMapBelong.erase(Itr);	
				}
				m_BigMapBelong.insert(std::make_pair(pBigMapBelongData->srcMap_id, pBigMapBelongData));
				
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: BigMapManager::LoadBelongCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
	
}

const BigMapData* CBigMapManager::GetBigMap(UInt16 map_id)
{
	BigMapDataMap::iterator itr = m_BigMap.find(map_id);
	if (itr != m_BigMap.end())
		return itr->second;
	else
		return NULL;
}

const BigMapBelongData* CBigMapManager::GetBigMapBelong(UInt16 map_id)
{
	BigMapBelongDataMap::iterator itr = m_BigMapBelong.find(map_id);
	if (itr != m_BigMapBelong.end())
		return itr->second;
	else
		return NULL;
}

const BigMapData* CBigMapManager::GetBigMapByIndex(UInt32 index)
{
	if (index < m_BigMapVector.size())
		return m_BigMapVector[index];

	return NULL;
}

UInt32 CBigMapManager::GetMapCount()
{
	return (UInt32)m_BigMap.size();
}

