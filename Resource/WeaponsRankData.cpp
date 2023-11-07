//-- Common
#include "Common.h"
//-- Self
#include "Resource/PetManager.h"
//-- Library
#include "Reuben/Platform/BuffReader.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/WeaponsRankData.h"

CWeaponsRankData::CWeaponsRankData()
{
	m_mapWeaponsRank.clear();
}

CWeaponsRankData::~CWeaponsRankData()
{
	Release();
}

void CWeaponsRankData::Release()
{
	for (WeaponsRankInfoMap::iterator itr = m_mapWeaponsRank.begin(); itr != m_mapWeaponsRank.end(); ++itr)
	{
		SafeDeleteObject(itr->second);
	}

	m_mapWeaponsRank.clear();
}

BOOL CWeaponsRankData::LoadCSV(const TCHAR* filename, const BYTE* pData, UInt32 pDataLen)
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
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: PetManager::LoadCSV: file error, %s"), filename);
	}

	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		WeaponsRankInfo* pWeaponsRankInfo = NULL;
		UInt16	uindex = 0;
		try{
			while( !reader.IsLineEmpty()  )
			{
				uindex++;
				pWeaponsRankInfo = SafeCreateObject(WeaponsRankInfo);
				reader.ReadString(_T("WeaponsName"), pWeaponsRankInfo->WeaponsName);
				reader.ReadString(_T("WeaponsOwner"), pWeaponsRankInfo->ChiOwnerName);
				reader.ReadString(_T("WeaponsDepict"), pWeaponsRankInfo->WeaponsDepict);

				WeaponsRankInfoMap::iterator Itr = m_mapWeaponsRank.find(uindex);
				if (Itr != m_mapWeaponsRank.end())
				{
					SafeDeleteObject(Itr->second);
					m_mapWeaponsRank.erase(Itr);
				}
				m_mapWeaponsRank.insert(std::make_pair(uindex, pWeaponsRankInfo));

				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: WeaponsRankInfo::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	return TRUE;
}

BOOL CWeaponsRankData::LoadDAT(const TCHAR* filename)
{
	return FALSE;
}

BOOL CWeaponsRankData::SaveDAT(const TCHAR* filename)
{
	return FALSE;
}

WeaponsRankInfo * CWeaponsRankData::GetWeaponsRankData(UInt32 weaponsrank_id)
{
	WeaponsRankInfoMap::iterator itr = m_mapWeaponsRank.find(weaponsrank_id);
	if (itr != m_mapWeaponsRank.end())
		return itr->second;
	else
		return NULL;
}