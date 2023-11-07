//-- Common
#include "Common.h"
//-- Self
#include "Resource/NewbieManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/BuffReader.h>
#include "Resource/ResNewbie.h"
#include "Resource/QGLCSVTReader.h"

#define NEWBIE_DATA_VER 1

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

////////////// Skill Manager //////////////////////////////////

CNewBieManager::CNewBieManager()
{
	m_newbies.clear();
}

CNewBieManager::~CNewBieManager()
{
	Release();
}

VOID CNewBieManager::Release()
{
	for (NewbieMap::iterator it = m_newbies.begin(); it != m_newbies.end(); it++)
		SafeDeleteObject(it->second);
	m_newbies.clear();
	
}

const NewbieData* CNewBieManager::GetNewBieData(UInt16 NewBie_id)
{
	NewbieMap::iterator it = m_newbies.find(NewBie_id);
	if( it!=m_newbies.end() )
		return it->second;
	else
		return NULL;
}

BOOL CNewBieManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNewBieManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		NewbieData* pNewBieData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pNewBieData = SafeCreateObject(NewbieData);
				reader.Read(_T("id"),		pNewBieData->id);
				reader.ReadString(_T("class1"),	pNewBieData->class1, 32);
				reader.ReadString(_T("class2"),	pNewBieData->class2, 32);
				reader.ReadString(_T("class3"),	pNewBieData->class3, 32);
				reader.ReadString(_T("description"),	pNewBieData->description, 2048);
				
				NewbieMap::iterator Itr = m_newbies.find(pNewBieData->id);

				if (Itr != m_newbies.end())
				{
					SafeDeleteObject(Itr->second);
					m_newbies.erase(Itr);
				}
				m_newbies.insert(std::make_pair(pNewBieData->id, pNewBieData));
				reader.MoveNext();
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Initialization: %i newBie(s) read"), (UInt)m_newbies.size());
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNewBieManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			if (pNewBieData)
				SafeDeleteObject(pNewBieData);
			return FALSE;
		}
		reader.Close();
	}

	return TRUE;
}

BOOL CNewBieManager::LoadDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			file.Read( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numNewBieData = 0;
			file.Read( (BYTE*)&numNewBieData, sizeof(UInt32) );

			NewbieData* pNewBieData;
			for (UInt32 i = 0; i < numNewBieData; i++)
			{
				pNewBieData = SafeCreateObject(NewbieData);
				file.Read( (BYTE*)pNewBieData, sizeof(NewbieData) );
				m_newbies[pNewBieData->id] = pNewBieData;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNewBieManager::LoadDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();

	return TRUE;
}

BOOL CNewBieManager::SaveDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_WRITE);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			header.version = NEWBIE_DATA_VER;
			file.Write( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numNewBie = (UInt32)m_newbies.size();
			file.Write( (BYTE*)&numNewBie, sizeof(UInt32) );

			for (NewbieMap::iterator it = m_newbies.begin(); it != m_newbies.end(); it++)
			{
				file.Write( (BYTE*)it->second, sizeof(NewbieData) );
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNewBieManager::SaveDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}
