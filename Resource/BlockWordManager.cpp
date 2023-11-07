//-- Common
#include "Common.h"
//-- Self
#include "Resource/BlockWordManager.h"
//-- Library
#include "Reuben/Platform/BuffReader.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResBlockWord.h"


//typedef Reuben::Platform::BuffReader File;
//typedef Reuben::Platform::FileException FileException;

CBlockWordManager::CBlockWordManager()
{
	m_BlockWordMap.clear();
}

CBlockWordManager::~CBlockWordManager()
{
	Release();
}

void CBlockWordManager::Release()
{
	for (BlockWordMap::iterator itr = m_BlockWordMap.begin(); itr != m_BlockWordMap.end(); ++itr)
	{
		SafeDeleteObject(itr->second);
	}

	m_BlockWordMap.clear();
}

BOOL CBlockWordManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CBlockWordManager::LoadCSV: file error, %s"), filename);
	}

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: start load..."));
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		BlockWordData* pBlockWordData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pBlockWordData = SafeCreateObject(BlockWordData);
				reader.Read(_T("index"),		pBlockWordData->index);
				reader.ReadString(_T("word"),		pBlockWordData->blockword, 99);
				String tempString(pBlockWordData->blockword);
				tempString = tempString.Trim();				
				_stprintf(pBlockWordData->blockword, _T("%s"), tempString.c_str());
				reader.Read(_T("name"),			pBlockWordData->type);
				
				if (tempString.length() == 0)
				{
					SafeDeleteObject(pBlockWordData);
					reader.MoveNext();
					continue;
				}
				//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("LoadBlockCSV %d, %s"), pBlockWordData->index, pBlockWordData->blockword);
				BlockWordMap::iterator Itr = m_BlockWordMap.find(pBlockWordData->index);
				if (Itr != m_BlockWordMap.end())
				{
					SafeDeleteObject(Itr->second);
					m_BlockWordMap.erase(Itr);
				}
				m_BlockWordMap.insert(std::make_pair(pBlockWordData->index, pBlockWordData));
				
				if (pBlockWordData->type != BLOCK_NAME_ONLY)
					m_BlockMsgWordList.push_back(pBlockWordData->blockword);
				
				if (pBlockWordData->type != BLOCK_MSG_ONLY)
					m_BlockNameWordList.push_back(pBlockWordData->blockword);

				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: BlockWordManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
	
}

const BlockWordData* CBlockWordManager::GetBlockWordData(UInt32 index)
{
	BlockWordMap::iterator itr = m_BlockWordMap.find(index);
	if (itr != m_BlockWordMap.end())
		return itr->second;
	else
		return NULL;
}

VOID CBlockWordManager::GetBlockWordList(BlockWordDataList &blockWordList)
{
	for (BlockWordMap::iterator it = m_BlockWordMap.begin(); it != m_BlockWordMap.end(); it++)
		blockWordList.push_back(String(it->second->blockword));
}

BlockWordDataList* CBlockWordManager::GetMsgBlockWordList()
{
	/*
	for (BlockWordMap::iterator it = m_BlockWordMap.begin(); it != m_BlockWordMap.end(); it++)
	{
		if (it->second->type != BLOCK_NAME_ONLY)
			blockWordList.push_back(String(it->second->blockword));
	}
	*/
	return &m_BlockMsgWordList;
}

BlockWordDataList* CBlockWordManager::GetNameBlockWordList()
{
	/*
	for (BlockWordMap::iterator it = m_BlockWordMap.begin(); it != m_BlockWordMap.end(); it++)
	{
		if (it->second->type != BLOCK_MSG_ONLY)
			blockWordList.push_back(String(it->second->blockword));
	}
	*/
	return &m_BlockNameWordList;
}

