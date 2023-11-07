//-- Common
#include "Common.h"
//-- Self
#include "Resource/StartCharManager.h"
//-- Library
#include "Reuben/Platform/BuffReader.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResCharacter.h"


//typedef Reuben::Platform::BuffReader File;
//typedef Reuben::Platform::FileException FileException;

CStartCharManager::CStartCharManager()
{
	m_StartChar.clear();
}

CStartCharManager::~CStartCharManager()
{
	Release();
}

void CStartCharManager::Release()
{
	for (StartCharMap::iterator itr = m_StartChar.begin(); itr != m_StartChar.end(); ++itr)
	{
		SafeDeleteObject(itr->second);
	}

	m_StartChar.clear();
}

BOOL CStartCharManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CStartCharManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		StartCharData* pCharData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pCharData = SafeCreateObject(StartCharData);
				reader.Read(_T("cclass_id"),	pCharData->sexClass);
				reader.Read(_T("STA"),			pCharData->STA);
				reader.Read(_T("SPR"),			pCharData->SPR);
				reader.Read(_T("STR"),			pCharData->STR);
				reader.Read(_T("CON"),			pCharData->CON);
				reader.Read(_T("AGI"),			pCharData->AGI);
				reader.Read(_T("MaxHP"),		pCharData->MaxHP);
				reader.Read(_T("MaxSP"),		pCharData->MaxSP);
				reader.Read(_T("MaxVP"),		pCharData->MaxVP);
				reader.Read(_T("MaxDP"),		pCharData->MaxDP);
				reader.Read(_T("MaxWc"),		pCharData->MaxWC);
				reader.Read(_T("StartMap"),		pCharData->StartMap);
				reader.Read(_T("StartX"),		pCharData->StartX);
				reader.Read(_T("StartY"),		pCharData->StartY);
				reader.Read(_T("W1"),			pCharData->Weapon1);
				reader.Read(_T("W2"),			pCharData->Weapon2);
				reader.Read(_T("Cloth"),		pCharData->Cloth);
				reader.Read(_T("Head"),		pCharData->HeadAs);
				reader.Read(_T("Foot"),		pCharData->Foot);
				reader.Read(_T("A1"),			pCharData->A1);
				reader.Read(_T("A2"),			pCharData->A2);
				reader.Read(_T("maxhp_sta"),	pCharData->MaxHP_STA);
				reader.Read(_T("maxsp_spr"),	pCharData->MaxSP_SPR);
				reader.Read(_T("atk_str"),		pCharData->Atk_STR);
				reader.Read(_T("def_con"),		pCharData->Def_CON);
				reader.Read(_T("hit_agi"),		pCharData->Hit_AGI);
				reader.Read(_T("eva_agi"),		pCharData->Eva_AGI);
				reader.Read(_T("pwd_spr"),		pCharData->Pwd_SPR);
				reader.Read(_T("pwd_sta"),		pCharData->Pwd_STA);
				reader.Read(_T("pwd_con"),		pCharData->Pwd_CON);
				reader.Read(_T("pwd_str"),		pCharData->Pwd_STR);
				reader.Read(_T("spd_agi"),		pCharData->Spd_AGI);
				reader.Read(_T("spd_str"),		pCharData->Spd_STR);
				reader.Read(_T("spd_con"),		pCharData->Spd_CON);
				reader.Read(_T("spd_sta"),		pCharData->Spd_STA);
				reader.Read(_T("start_npc"),	pCharData->Start_npc_templateID);

				StartCharMap::iterator Itr = m_StartChar.find(pCharData->sexClass);
				if (Itr != m_StartChar.end())
				{
					SafeDeleteObject(Itr->second);
					m_StartChar.erase(Itr);
				}
				m_StartChar.insert(std::make_pair(pCharData->sexClass, pCharData));
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: StartCharManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
	
}

const StartCharData* CStartCharManager::GetStartChar(UInt32 sexClass)
{
	StartCharMap::iterator itr = m_StartChar.find(sexClass);
	if (itr != m_StartChar.end())
		return itr->second;
	else
		return NULL;
}
