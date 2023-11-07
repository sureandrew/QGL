//-- Common
#include "Common.h"
//-- Self
#include "Resource/PartnerCollectionManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/BuffReader.h>
#include "Resource/ResPartnerCollection.h"
#include "Resource/QGLCSVTReader.h"

#define ITEM_DATA_VER		1

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

////////////// Item Manager //////////////////////////////////

CPartnerCollectionManager::CPartnerCollectionManager()
{
	m_PartnerCollection.clear();
}

CPartnerCollectionManager::~CPartnerCollectionManager()
{
	Release();
}

VOID CPartnerCollectionManager::Release()
{
	for (PartnerCollectionMap::iterator it = m_PartnerCollection.begin(); it != m_PartnerCollection.end(); it++)
		SafeDeleteObject(it->second);
	m_PartnerCollection.clear();
}

UInt16 CPartnerCollectionManager::GetPartnerCollectionBuffID(UInt16 Count, UInt8 Type)
{
	PartnerCollectionMap::iterator it = m_PartnerCollection.begin();
	while( it!=m_PartnerCollection.end() )
	{
		if (it->second->Type == Type && it->second->ID == Count && it->second->Buff.size() > 0)
		{
			return it->second->Buff[0];
		}
		++it;
	}
	return 0;
}

void CPartnerCollectionManager::GetPartnerCollectionData(PartnerCollectionDataMap &PartnerCollectionList, UInt8 Type)
{
	PartnerCollectionMap::iterator it = m_PartnerCollection.begin();
	while( it!=m_PartnerCollection.end() )
	{
		if (it->second->Type == Type)
		{
			PartnerCollectionList[it->second->ID] = it->second;
		}
		it++;
	}
}

void CPartnerCollectionManager::GetPartnerCollectionData(PartnerCollectionDataMap &PartnerCollectionList, UInt8 Type, UInt8 Group)
{
	PartnerCollectionMap::iterator it = m_PartnerCollection.begin();
	while( it!=m_PartnerCollection.end() )
	{
		if (it->second->Type == Type && it->second->Group == Group)
		{
			PartnerCollectionList[it->second->ID] = it->second;
		}
		it++;
	}
}

const PartnerCollectionData* CPartnerCollectionManager::GetPartnerCollectionData(UInt32 collect_id)
{
	PartnerCollectionMap::iterator it = m_PartnerCollection.find(collect_id);
	if( it!=m_PartnerCollection.end() )
		return it->second;
	else
		return NULL;
}

BOOL CPartnerCollectionManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{
	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(filename, true) )
		openSrcSucceeded = TRUE;
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		PartnerCollectionMap::iterator it;
		PartnerCollectionData* pPartnerCollection = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pPartnerCollection = SafeCreateObject(PartnerCollectionData);
				reader.Read(_T("ID"),	pPartnerCollection->ID);
				reader.Read(_T("Type"),	pPartnerCollection->Type);
				reader.Read(_T("Group"),pPartnerCollection->Group);
				reader.Read(_T("MOBid"),pPartnerCollection->MOBid);
				reader.Read(_T("STR"),	pPartnerCollection->STR);
				reader.Read(_T("CON"),	pPartnerCollection->CON);
				reader.Read(_T("STA"),	pPartnerCollection->STA);
				reader.Read(_T("AGI"),	pPartnerCollection->AGI);
				reader.Read(_T("SPR"),	pPartnerCollection->SPR);
				reader.Read(_T("Light"),	pPartnerCollection->Light);
				reader.ReadString(_T("Name"),	pPartnerCollection->Name, 18);

				TCHAR buf[512];
				TCHAR *delim = _T("|");
				TCHAR *cmd = NULL; 
				reader.ReadString(_T("Buff"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					if (value != 0)
						pPartnerCollection->Buff.push_back(value);
					cmd = _tcstok(NULL, delim);
				}
				
				it = m_PartnerCollection.find(pPartnerCollection->MOBid);
				if (it != m_PartnerCollection.end())
				{
					TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("PartnerCollectionManager LoadCSV: PartnerCollection->MOBid %d already loaded"), pPartnerCollection->MOBid);
					SafeDeleteObject(it->second);
					m_PartnerCollection.erase(it);
				}

				m_PartnerCollection.insert(std::make_pair(pPartnerCollection->MOBid, pPartnerCollection));
				reader.MoveNext();
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Initialization: %i PartnerCollection(s) read"), (UInt)m_PartnerCollection.size());
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: PartnerCollectionManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	return TRUE;
}

BOOL CPartnerCollectionManager::LoadDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			file.Read( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numItem = 0;
			file.Read( (BYTE*)&numItem, sizeof(UInt32) );

			PartnerCollectionData* pPartnerCollection;
			for (UInt32 i = 0; i < numItem; i++)
			{
				pPartnerCollection = SafeCreateObject(PartnerCollectionData);
				file.Read( (BYTE*)pPartnerCollection, sizeof(PartnerCollectionData) );
				m_PartnerCollection[pPartnerCollection->MOBid] = pPartnerCollection;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CPartnerCollectionManager::LoadDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();

	return TRUE;
}

BOOL CPartnerCollectionManager::SaveDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_WRITE);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			header.version = ITEM_DATA_VER;
			file.Write( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numItem = (UInt32)m_PartnerCollection.size();
			file.Write( (BYTE*)&numItem, sizeof(UInt32) );

			for (PartnerCollectionMap::iterator it = m_PartnerCollection.begin(); it != m_PartnerCollection.end(); it++)
			{
				file.Write( (BYTE*)it->second, sizeof(PartnerCollectionData) );
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CPartnerCollectionManager::SaveDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}
