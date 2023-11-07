#include "Common.h"
#include "Resource/ManorManager.h"

//-- Library
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResManor.h"
#include "Reuben/Platform/BuffReader.h"

CManorManager::CManorManager(void)
{
	m_Manor.clear();	
}

CManorManager::~CManorManager(void)
{
	Release();
}

void CManorManager::Release()
{
	for (ManorDataMap::iterator itr = m_Manor.begin(); itr != m_Manor.end(); ++itr)
	{
		SafeDeleteObject(itr->second);
	}
	m_Manor.clear();
}

BOOL CManorManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{	
	CQGLCSVTReader reader;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(filename, true) )
		openSrcSucceeded = TRUE;

	if( openSrcSucceeded )
	{
		ManorData* pManor = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				TCHAR buf[512];
				pManor = SafeCreateObject(ManorData);
				if ( pManor ) {
				reader.Read(_T("Manor_Scale"),			pManor->manor_scale);
				reader.Read(_T("Manor_MaxProsper"),		pManor->manor_maxProsper);
				reader.Read(_T("Manor_MaxFund"),		pManor->manor_maxFund);
				reader.Read(_T("Manor_MaxNumofRoom"),	pManor->manor_maxRoomNum);
				reader.Read(_T("Manor_MeditationLv"),	pManor->manor_meditationLv);
				reader.Read(_T("Manor_TigerRuneLv"),	pManor->manor_tigerRuneLv);
				reader.Read(_T("Manor_MaintainCost"),	pManor->manor_maintainCost);
				reader.Read(_T("Manor_ProsperIncome"),	pManor->manor_prosperIncome);
				reader.Read(_T("Manor_Icon"),			pManor->manor_Icon);

				TCHAR *delim = _T("|");
				TCHAR *cmd = NULL; 
				reader.ReadString(_T("Map_id"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					pManor->manor_map_id.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				reader.Read(_T("Manor_UpgradeMainTimes"),		pManor->manor_maxMaintain);
				reader.Read(_T("Manor_MaxNumofGuard"),			pManor->manor_maxGuard);
				reader.Read(_T("Manor_MaxTrainPlayerExPow"),	pManor->manor_maxCharEx);
				reader.Read(_T("Manor_MaxTrainPlayerInPow"),	pManor->manor_maxCharIn);
				reader.Read(_T("Manor_MaxTrainPartnerExPow"),	pManor->manor_maxPartnerEx);
				reader.Read(_T("Manor_MaxTrainPartnerInPow"),	pManor->manor_maxPartnerIn);
				reader.Read(_T("Manor_MaxRoomDrugLv"),			pManor->manor_maxDrug);
				reader.Read(_T("Manor_MaxRoomCookLv"),			pManor->manor_maxCook);
				reader.Read(_T("Manor_MaxRoomWineLv"),			pManor->manor_maxWine);
				reader.Read(_T("Manor_MaxRoomHPLv"),			pManor->manor_maxHp);
				reader.Read(_T("Manor_MaxRoomSPLv"),			pManor->manor_maxSp);
				
				m_Manor.insert(std::make_pair(pManor->manor_scale, pManor));
				}
				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CManorManager::LoadCSV, open error, CSV reading error due to column format or missing MoveNext(), %s"), filename );
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CManorManager::LoadCSV, open error, no corresponding CSV file: %s"), filename );
		return FALSE;
	}

	return TRUE;

}


const ManorData* CManorManager::GetManor(UInt8 manor_scale)
{
	ManorDataMap::iterator itr = m_Manor.find(manor_scale);
	if (itr != m_Manor.end())
		return itr->second;
	else
		return NULL;
}