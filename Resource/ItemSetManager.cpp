//-- Common
#include "Common.h"
//-- Self
#include "Resource/ItemSetManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/BuffReader.h>
#include "Resource/ResItemSet.h"
#include "Resource/QGLCSVTReader.h"

#define ITEM_DATA_VER		1

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

////////////// Item Manager //////////////////////////////////

CItemSetManager::CItemSetManager()
{
	m_ItemSet.clear();
}

CItemSetManager::~CItemSetManager()
{
	Release();
}

VOID CItemSetManager::Release()
{
	for (ItemSetMap::iterator it = m_ItemSet.begin(); it != m_ItemSet.end(); ++it)
		SafeDeleteObject(it->second);
	m_ItemSet.clear();
}

const ItemSetData* CItemSetManager::GetItemSetData(UInt32 itemset_id)
{
	ItemSetMap::iterator it = m_ItemSet.find(itemset_id);
	if( it!=m_ItemSet.end() )
		return it->second;
	else
		return NULL;
}

BOOL CItemSetManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		ItemSetMap::iterator it;
		ItemSetData* pItemSet = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pItemSet = SafeCreateObject(ItemSetData);
				reader.Read(_T("itemset_id"),	pItemSet->itemset_id);
				reader.ReadString(_T("name"),	pItemSet->name, 18);
				reader.Read(_T("count"),	pItemSet->count);
				reader.Read(_T("STR"),	pItemSet->STR);
				reader.Read(_T("CON"),	pItemSet->CON);
				reader.Read(_T("STA"),	pItemSet->STA);
				reader.Read(_T("AGI"),	pItemSet->AGI);
				reader.Read(_T("SPR"),	pItemSet->SPR);
				reader.Read(_T("ATK"),	pItemSet->ATK);
				reader.Read(_T("DEF"),	pItemSet->DEF);
				reader.Read(_T("HIT"),	pItemSet->HIT);
				reader.Read(_T("EVA"),	pItemSet->EVA);
				reader.Read(_T("POW"),	pItemSet->POW);
				reader.Read(_T("SPD"),	pItemSet->SPD);
				reader.Read(_T("bat_attrib"),	pItemSet->bat_attrib);
				reader.Read(_T("ratio"),	pItemSet->ratio);
				reader.Read(_T("MaxHP"),	pItemSet->MaxHP);
				reader.Read(_T("MaxSP"),	pItemSet->MaxSP);
				reader.Read(_T("MaxVP"),	pItemSet->MaxVP);

				TCHAR buf[512];
				TCHAR *delim = _T("|");
				TCHAR *cmd = NULL;

				reader.ReadString(_T("skill_ids"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt32 value = _ttoi(cmd);
					pItemSet->skill_ids.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				cmd = NULL;
				reader.ReadString(_T("item_ids"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt32 value = _ttoi(cmd);
					pItemSet->item_ids.push_back(value);
					cmd = _tcstok(NULL, delim);
				}
				
				it = m_ItemSet.find(pItemSet->itemset_id);
				if (it != m_ItemSet.end())
				{
					TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("ItemSetManager LoadCSV: ItemSet->itemset_id %d already loaded"), pItemSet->itemset_id);
					SafeDeleteObject(it->second);
					m_ItemSet.erase(it);
				}

				TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" itemset_id = %d."), pItemSet->itemset_id);

				m_ItemSet.insert(std::make_pair(pItemSet->itemset_id, pItemSet));
				reader.MoveNext();
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Initialization: %i ItemSet(s) read"), (UInt)m_ItemSet.size());
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: ItemSetManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	return TRUE;
}

BOOL CItemSetManager::LoadDAT(const TCHAR *filename)
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

			ItemSetData* pItemSet;
			for (UInt32 i = 0; i < numItem; i++)
			{
				pItemSet = SafeCreateObject(ItemSetData);
				file.Read( (BYTE*)pItemSet, sizeof(ItemSetData) );
				m_ItemSet[pItemSet->itemset_id] = pItemSet;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CItemSetManager::LoadDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();

	return TRUE;
}

BOOL CItemSetManager::SaveDAT(const TCHAR *filename)
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
			UInt32 numItem = (UInt32)m_ItemSet.size();
			file.Write( (BYTE*)&numItem, sizeof(UInt32) );

			for (ItemSetMap::iterator it = m_ItemSet.begin(); it != m_ItemSet.end(); it++)
			{
				file.Write( (BYTE*)it->second, sizeof(ItemSetData) );
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CItemSetManager::SaveDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}
