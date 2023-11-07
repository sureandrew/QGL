//-- Common
#include "Common.h"
//-- Self
#include "SkillCombinationManager.h"
//-- Library
#include "ResSkillCombination.h"
#include "Resource/QGLCSVTReader.h"
#include "MOBManager.h"
#include "ResMob.h"

#ifdef CLIENT
#include "ClientResManager.h"
#include "GDFManager.h"
#endif

CSkillCombinationManager::CSkillCombinationManager()
{
	m_MobSkillCombLst.clear();
}

CSkillCombinationManager::~CSkillCombinationManager()
{
	Release();
}

VOID CSkillCombinationManager::Release()
{
	for ( MobSkillCombTable::iterator itr = m_MobSkillCombLst.begin(); itr != m_MobSkillCombLst.end(); ++itr )
		SafeDeleteObject(itr->second);
	m_MobSkillCombLst.clear();
}

BOOL CSkillCombinationManager::LoadCSV(LPCTSTR szPath, const BYTE* pData, UInt32 pDataLen)
{


	String srcfile(_T(""));
	srcfile.Format(_T("%s"), szPath);
	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(srcfile.c_str(), true) )
		openSrcSucceeded = TRUE;
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CSkillCombinationManager::LoadCSV: file error, %s"), srcfile.c_str());
	}

	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{

		MobSkillDef* pDef = NULL;
		MobSkillCombination* pCombination = SafeCreateObject(MobSkillCombination);
		try{
			if (pCombination)
			{
				while( !reader.IsLineEmpty()  )
				{
					pDef = SafeAllocate(MobSkillDef, 1);
					
					if ( pDef )
					{
						
						reader.Read(_T("Rate"),	pDef->skillRate);
						reader.Read(_T("Type"), pDef->skillType);
						reader.Read(_T("skillid"), pDef->skillId);

						pCombination->skillLst.push_back(pDef);
						
						//
					}
					reader.MoveNext();
				}
				String szfilename;
				szfilename = srcfile.substr(srcfile.length() - 17, 17);
				m_MobSkillCombLst.insert( std::make_pair(String(szfilename), pCombination) );
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CSkillCombinationManager::LoadCSV: CSV reading error due to \
											column format or missing MoveNext() %d "), srcfile.c_str());
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
}

MobSkillCombination* CSkillCombinationManager::GetSkillCombination(String szeFileName)
{
	MobSkillCombTable::iterator itr = m_MobSkillCombLst.find( szeFileName );
	if ( itr != m_MobSkillCombLst.end() )
		return itr->second;
	return NULL;
}

VOID CSkillCombinationManager::SearchFolder()
{
	StlVector<String> filequeue;
	filequeue.clear();

	{// - Search the folder of csv file
		String szPath(_T(""));
		szPath.Format(_T("%s"), _T("Data\\CSV\\MobSkill\\*.csv"));

		WIN32_FIND_DATA data = {0};
		HANDLE hFile = ::FindFirstFile(szPath.c_str(), &data);
		if ( hFile && hFile != INVALID_HANDLE_VALUE )
		{
			do
			{
				String tempfile(_T(""));
				tempfile.Format(_T("Data\\CSV\\MobSkill\\%s"), data.cFileName);
				filequeue.push_back(tempfile);
			}
			while ( FindNextFile(hFile, &data) );
			FindClose( hFile );
		}
	}

	for ( StlVector<String>::iterator itr = filequeue.begin(); itr != filequeue.end(); ++itr )
	{
		
		LoadCSV((*itr).c_str());
	}
}

#ifdef CLIENT
VOID CSkillCombinationManager::SearchClientFolder(CClientResManager* pResource)
{
	if (pResource == NULL)
		return;

	StlVector<String> filequeue;
	filequeue.clear();

	MobDataList mobList;
	pResource->GetMobList(mobList);

	for (UInt32 i = 0; i < mobList.size(); ++i)
	{
		String fileName;
		fileName.Format(_T("Data\\CSV\\MobSkill\\MobSkill%05d.csv"), mobList[i]->skill_file_id);
		filequeue.push_back(fileName);
	}

	for ( StlVector<String>::iterator itr = filequeue.begin(); itr != filequeue.end(); ++itr )
	{	
		if (pResource->GetGDFManager() || pResource->GetGDFExManager())
		{
			// CSV Default Path
			String csvfilename = *itr;
			UInt32 beginOffset, fileSize;
			BYTE* fbuf = NULL;
			if ( pResource->GetGDFExManager() )
				fbuf = pResource->GetGDFExManager()->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
			if ( (!fbuf) && pResource->GetGDFManager() )
				fbuf = pResource->GetGDFManager()->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
			if( fbuf )
			{
				BYTE* buf = fbuf + beginOffset;
				LoadCSV((*itr).c_str(), buf, fileSize);
				UnmapViewOfFile(fbuf);
			}
		}
		else
		{
			LoadCSV((*itr).c_str());
		}
		
	}
}
#endif // CLIENT