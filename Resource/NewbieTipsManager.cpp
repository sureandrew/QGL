//-- Common
#include "Common.h"
//-- Self
#include "Resource/NewbieTipsManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/BuffReader.h>
#include "Resource/ResNewbieTips.h"
#include "Resource/QGLCSVTReader.h"

#define NEWBIE_DATA_VER 1

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

////////////// Skill Manager //////////////////////////////////

CNewbieTipsManager::CNewbieTipsManager()
{
	m_NewbieTips.clear();
}

CNewbieTipsManager::~CNewbieTipsManager()
{
	Release();
}

VOID CNewbieTipsManager::Release()
{
	for (NewbieTipsMap::iterator it = m_NewbieTips.begin(); it != m_NewbieTips.end(); it++)
		SafeDeleteObject(it->second);
	m_NewbieTips.clear();
	
}

const NewbieTipsData* CNewbieTipsManager::GetNewbieTipsData(UInt16 NewbieTips_id)
{
	NewbieTipsMap::iterator it = m_NewbieTips.find(NewbieTips_id);
	if( it!=m_NewbieTips.end() )
		return it->second;
	else
		return NULL;
}

BOOL CNewbieTipsManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: SkillManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		NewbieTipsData* pNewBieTipsData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				UInt16 ID = 0;
				reader.Read(_T("helpid"),		ID);
				TCHAR buf[257];
				NewbieTipsMap::iterator Itr = m_NewbieTips.find(ID);
				if (Itr != m_NewbieTips.end())
				{
					reader.ReadString(_T("boyimage"),	buf, 256);
					Itr->second->boyimages.push_back(String(buf));

					reader.ReadString(_T("girlimage"),	buf, 256);
					Itr->second->girlimages.push_back(String(buf));

					reader.ReadString(_T("boystring"),	buf, 256);
					Itr->second->boystring.push_back(String(buf));

					reader.ReadString(_T("girlstring"),	buf, 256);
					Itr->second->girlstring.push_back(String(buf));
				}
				else
				{
					
					pNewBieTipsData = SafeCreateObject(NewbieTipsData);
					pNewBieTipsData->id = ID;
					reader.Read(_T("event"),		pNewBieTipsData->event);
					reader.Read(_T("levelup"),		pNewBieTipsData->level_up);
					reader.Read(_T("mapid"),		pNewBieTipsData->map_id);
					reader.Read(_T("line"),		pNewBieTipsData->Line);
					reader.Read(_T("flash"),		pNewBieTipsData->flash);
					

					reader.ReadString(_T("boyimage"),	buf, 256);
					pNewBieTipsData->boyimages.push_back(String(buf));

					reader.ReadString(_T("girlimage"),	buf, 256);
					pNewBieTipsData->girlimages.push_back(String(buf));

					reader.ReadString(_T("boystring"),	buf, 256);
					pNewBieTipsData->boystring.push_back(String(buf));

					reader.ReadString(_T("girlstring"),	buf, 256);
					pNewBieTipsData->girlstring.push_back(String(buf));

					m_NewbieTips.insert(std::make_pair(pNewBieTipsData->id, pNewBieTipsData));
				}
				reader.MoveNext();
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Initialization: %i newBie(s) read"), (UInt)m_NewbieTips.size());
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: SkillManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	return TRUE;
}

BOOL CNewbieTipsManager::LoadDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			file.Read( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numNewBieTipsData = 0;
			file.Read( (BYTE*)&numNewBieTipsData, sizeof(UInt32) );

			NewbieTipsData* pNewbieTipsData;
			for (UInt32 i = 0; i < numNewBieTipsData; i++)
			{
				pNewbieTipsData = SafeCreateObject(NewbieTipsData);
				file.Read( (BYTE*)pNewbieTipsData, sizeof(NewbieTipsData) );
				m_NewbieTips[pNewbieTipsData->id] = pNewbieTipsData;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNewbieTipsTipsManager::LoadDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();

	return TRUE;
}

BOOL CNewbieTipsManager::SaveDAT(const TCHAR *filename)
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
			UInt32 numNewbieTips = (UInt32)m_NewbieTips.size();
			file.Write( (BYTE*)&numNewbieTips, sizeof(UInt32) );

			for (NewbieTipsMap::iterator it = m_NewbieTips.begin(); it != m_NewbieTips.end(); it++)
			{
				file.Write( (BYTE*)it->second, sizeof(NewbieTipsData) );
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNewbieTipsTipsManager::SaveDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}
