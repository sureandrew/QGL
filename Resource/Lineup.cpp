#include "Common.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "ResLineup.h"
#include "Lineup.h"

#define LINEUP_DATA_VERSION 1

#define LINEUP_APP_BUFF_ID_BEGIN 50

int CLineupManager::LoadCSV(const TCHAR *szFileName, const BYTE* pData, UInt32 pDataLen)
{
	if (NULL == szFileName) {
		return LINEUP_INVILAD_PARAM;
	}

	CQGLCSVTReader reader;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(szFileName, true) )
		openSrcSucceeded = TRUE;

	if( openSrcSucceeded )
	{
		LINEUPINFO* pLineup = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pLineup = SafeCreateObject(LINEUPINFO);
				reader.Read(_T("law_id"),pLineup->uiId);
				reader.ReadString(_T("law_name"),pLineup->szName,MAXLINEUPNAEMLEN-1);
				int i= 0;
				TCHAR szKey[32] = {0};
				for (;i<MAXTEAMMEM;i++) {
					memset(szKey,0,sizeof(TCHAR)*32);
					wsprintf(szKey,_T("player%d_pos"),i);
					reader.Read(szKey,pLineup->pb[i].iPos);
					memset(szKey,0,sizeof(TCHAR)*32);
					wsprintf(szKey,_T("position%d_buff"),i);
					reader.Read(szKey,pLineup->pb[i].uiBufferId);
					memset(szKey,0,sizeof(TCHAR)*32);
					wsprintf(szKey,_T("partner%d_pos"),i);
					reader.Read(szKey,pLineup->pb[i].iPosPartner);
				}
				for (i=1;i < MAXLINEUP ;i++) {
					memset(szKey,0,sizeof(TCHAR)*32);
					wsprintf(szKey,_T("law_Restraint%d"),i);
					reader.Read(szKey,pLineup->lui.Info[i].iAllelopathyValue);
					UInt16 uRealBuff=0;
					switch(pLineup->lui.Info[i].iAllelopathyValue)
					{
					case -10:
						uRealBuff = LINEUP_APP_BUFF_ID_BEGIN;
						break;
					case -5:
						uRealBuff = LINEUP_APP_BUFF_ID_BEGIN+1;
						break;
					case 5:
						uRealBuff = LINEUP_APP_BUFF_ID_BEGIN+2;
						break;
					case 10:
						uRealBuff = LINEUP_APP_BUFF_ID_BEGIN+3;
						break;
					default:
						break;
					}
					pLineup->lui.Info[i].uiLineupId = uRealBuff;
				}

				reader.ReadString(_T("law_Descption"),pLineup->szDesc,MAXLINEUPDES-1);
				
				m_Lineups.insert(std::make_pair(pLineup->uiId,pLineup));

				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: LineupManager::LoadCSV: CSV reading error due to \
											column format or missing MoveNext(), %s"), szFileName);
			reader.Close();
			return LINEUP_FILE_CONTEXT_ERROR;
		}
	}
	
	reader.Close();
	return LINEUP_OK;
}
VOID CLineupManager::Release()
{
	for (LineupMap::iterator it = m_Lineups.begin(); it != m_Lineups.end(); it++)
		SafeDeleteObject(it->second);
	m_Lineups.clear();
}
LINEUPINFO*  CLineupManager::GetLineupData(UInt32 uiLineupId)
{
	LineupMap::iterator it = m_Lineups.find(uiLineupId);
	if( it!=m_Lineups.end() )
		return it->second;
	else
		return NULL;
}
size_t CLineupManager::GetLineupDataSize()
{
	return m_Lineups.size();
}