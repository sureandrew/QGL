//-- Common
#include "Common.h"
//-- Self
#include "Resource/TitleManager.h"
//-- Library
#include "Reuben/Platform/BuffReader.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResTitle.h"

CTitleManager::CTitleManager()
{
	m_Title.clear();
}

CTitleManager::~CTitleManager()
{
	Release();
}

void CTitleManager::Release()
{
	for (TitleDataMap::iterator itr = m_Title.begin(); itr != m_Title.end(); ++itr)
	{
		SafeDeleteObject(itr->second);
	}

	m_Title.clear();

}

BOOL CTitleManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		TitleData* pTitleData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				TCHAR buf[512];

				pTitleData = SafeCreateObject(TitleData);
				
				reader.Read(_T("Title_id"),		pTitleData->Title_id);
				reader.ReadString(_T("Title_Name"),	pTitleData->Title_Name, 49);
				reader.Read(_T("Title_Color"),		pTitleData->Title_Color);
				reader.Read(_T("Title_Type"),		pTitleData->Title_Type);
				reader.Read(_T("Title_PreNameType"),		pTitleData->Title_PreNameType);
				reader.Read(_T("Title_RankType"),		pTitleData->Title_RankType);
				reader.Read(_T("Title_RankSubType"),		pTitleData->Title_RankSubType);
				reader.Read(_T("Title_StartRank"),		pTitleData->Title_StartRank);
				reader.Read(_T("Title_EndRank"),		pTitleData->Title_EndRank);
				reader.Read(_T("Title_TimeLimitType"),		pTitleData->Title_TimeLimitType);
				reader.Read(_T("Title_TimeLimit"),		pTitleData->Title_TimeLimit);

				reader.ReadString(_T("Title_ExpireDate"),	buf, 511);
				String temp = buf;
				if (temp.GetLength() >= 14)
				{
					String text;
					text = temp.substr(0, 4);
					pTitleData->Title_ExpireYear = _wtoi(text.c_str());

					text = temp.substr(4, 2);
					pTitleData->Title_ExpireMonth = _wtoi(text.c_str());

					text = temp.substr(6, 2);
					pTitleData->Title_ExpireDay = _wtoi(text.c_str());

					text = temp.substr(8, 2);
					pTitleData->Title_ExpireHour = _wtoi(text.c_str());

					text = temp.substr(10, 2);
					pTitleData->Title_ExpireMin = _wtoi(text.c_str());

					text = temp.substr(12, 2);
					pTitleData->Title_ExpireSec = _wtoi(text.c_str());
				}

				TitleDataMap::iterator Itr = m_Title.find(pTitleData->Title_id);
				if (Itr != m_Title.end())
				{
					SafeDeleteObject(Itr->second);
					m_Title.erase(Itr);
				}
				m_Title.insert(std::make_pair(pTitleData->Title_id, pTitleData));
				
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: PetManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
	
}


const TitleData* CTitleManager::GetTitle(UInt32 title_id)
{
	TitleDataMap::iterator itr = m_Title.find(title_id);
	if (itr != m_Title.end())
		return itr->second;
	else
		return NULL;
}

const TitleData* CTitleManager::GetTitleDataByType(UInt8	Title_Type, UInt8	Title_RankSubType, UInt8	Title_StartRank, UInt8	Title_EndRank)
{
	for (TitleDataMap::iterator itr = m_Title.begin();
		itr != m_Title.end(); ++itr)
	{
		if (itr->second->Title_Type == Title_Type &&
			itr->second->Title_RankSubType == Title_RankSubType &&
			itr->second->Title_StartRank == Title_StartRank &&
			itr->second->Title_EndRank == Title_EndRank)
			return itr->second;
	}
	
	return NULL;
}
const TitleData* CTitleManager::GetTitleByRank(UInt8	Title_RankType, UInt8	Title_RankSubType, 
											   UInt8	Title_StartRank, UInt8	Title_EndRank)
{
	for (TitleDataMap::iterator itr = m_Title.begin();
		itr != m_Title.end(); ++itr)
	{
		if (itr->second->Title_RankType == Title_RankType &&
			itr->second->Title_RankSubType == Title_RankSubType &&
			itr->second->Title_StartRank == Title_StartRank &&
			itr->second->Title_EndRank == Title_EndRank &&
			itr->second->Title_Type == 2)
			return itr->second;
	}
	
		return NULL;
}