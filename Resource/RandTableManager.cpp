//-- Common
#include "Common.h"
//-- Self
#include "RandTableManager.h"
//-- Library
#include "Resource/ResRandTable.h"
#include "Resource/QGLCSVTReader.h"

CRandTableManager::CRandTableManager()
{
	m_CSVItemList.clear();
}

CRandTableManager::~CRandTableManager()
{
	Release();
}

VOID CRandTableManager::Release()
{
	ReleaseItemTable();
	ReleaseMobTable();
	ReleaseNpcTable();
	ReleaseQuestTable();
}

VOID CRandTableManager::SearchFolder()
{
	SearchItemFolder();
	SearchMobFolder();
	SearchNpcFolder();
}

VOID CRandTableManager::ReleaseItemTable()
{
	for ( CSVItemTable::iterator itr = m_CSVItemList.begin(); itr != m_CSVItemList.end(); ++itr )
		SafeDeleteObject(itr->second);
	m_CSVItemList.clear();	
}

VOID CRandTableManager::SearchItemFolder()
{
	StlVector<String> filequeue;
	filequeue.clear();

	{// - Search the folder of csv file
		String szPath(_T(""));
		szPath.Format(_T("%s"), _T("Data\\CSV\\RandomItem\\*.csv"));

		WIN32_FIND_DATA data = {0};
		HANDLE hFile = ::FindFirstFile(szPath.c_str(), &data);
		if ( hFile && hFile != INVALID_HANDLE_VALUE )
		{
			do
			{
				String tempfile(_T(""));
				//tempfile.Format(_T("Data\\CSV\\RandomItem\\%s"), data.cFileName);
				//filequeue.push_back(tempfile);
				tempfile.Format(_T("%s"), data.cFileName);
				filequeue.push_back(tempfile);
			}
			while ( FindNextFile(hFile, &data) );
			FindClose( hFile );
		}
	}

	CSVItemData* pTempLst = NULL;

	for ( StlVector<String>::iterator itr = filequeue.begin(); itr != filequeue.end(); ++itr )
	{
		String szFullPath(_T(""));
		szFullPath.Format(_T("Data\\CSV\\RandomItem\\%s"), (*itr).c_str());
		pTempLst = LoadCSVItem(szFullPath.c_str());
		if ( pTempLst )
		{
			m_CSVItemList.insert( std::make_pair((*itr), pTempLst) );
		}
	}
}

CSVItemData* CRandTableManager::LoadCSVItem(String szfilename)
{
	CQGLCSVTReader reader;
	if( reader.Open(szfilename.c_str(), true) == false )
		return NULL;

	CSVItemData* pNewRowLst = NULL;
	{
		try{
			while( !reader.IsLineEmpty()  )
			{
				CSVItemRow* pRow = SafeCreateObject(CSVItemRow);
				if ( pRow )
				{
					reader.Read(_T("ItemRate"),	pRow->uRange);
					reader.Read(_T("NumofColumn"),	pRow->nCount);

					for ( int i = 1; i <= pRow->nCount; ++i )
					{
						String sztemp("");
						sztemp.Format(_T("C%02d"), i);
						UInt32 tempid = 0;
						reader.Read(sztemp, tempid);
						pRow->ItemLst.push_back(tempid);
					}

					if ( pNewRowLst == NULL )
					{
						pNewRowLst = SafeCreateObject(CSVItemData);
					}
					
					if ( pNewRowLst ) {
						pNewRowLst->RowLst.push_back(pRow);
					}
					else {
						SafeDeleteObject(pRow);
					}
				}
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CRandTableManager::LoadCSVItem: CSV reading error due to \
											   column format or missing MoveNext(), %s"), szfilename.c_str());
			reader.Close();
			return NULL;
		}
		reader.Close();
	}

	return pNewRowLst;
}

CONST CSVItemData* CRandTableManager::GetCSVItem(String szfilename)
{
	// file date check for hotplug should be supported here.
	CSVItemTable::iterator itr = m_CSVItemList.find(szfilename);
	if ( itr != m_CSVItemList.end() )
		return (itr->second);
	return NULL;
}

VOID CRandTableManager::ReleaseMobTable()
{
	for ( CSVMobTable::iterator itr = m_CSVMobList.begin(); itr != m_CSVMobList.end(); ++itr )
		SafeDeleteObject(itr->second);
	m_CSVMobList.clear();	
}

VOID CRandTableManager::SearchMobFolder()
{
	StlVector<String> filequeue;
	filequeue.clear();

	{// - Search the folder of csv file
		String szPath(_T(""));
		szPath.Format(_T("%s"), _T("Data\\CSV\\RandomMob\\*.csv"));

		WIN32_FIND_DATA data = {0};
		HANDLE hFile = ::FindFirstFile(szPath.c_str(), &data);
		if ( hFile && hFile != INVALID_HANDLE_VALUE )
		{
			do
			{
				String tempfile(_T(""));
				tempfile.Format(_T("%s"), data.cFileName);
				filequeue.push_back(tempfile);
			}
			while ( FindNextFile(hFile, &data) );
			FindClose( hFile );
		}
	}

	CSVMobData* pTempLst = NULL;

	for ( StlVector<String>::iterator itr = filequeue.begin(); itr != filequeue.end(); ++itr )
	{
		String szFullPath(_T(""));
		szFullPath.Format(_T("Data\\CSV\\RandomMob\\%s"), (*itr).c_str());
		pTempLst = LoadCSVMob(szFullPath.c_str());
		if ( pTempLst )
		{
			String test = (*itr);
			m_CSVMobList.insert( std::make_pair((*itr), pTempLst) );
		}
	}
}

CSVMobData* CRandTableManager::LoadCSVMob(String szfilename)
{
	CQGLCSVTReader reader;
	if( reader.Open(szfilename.c_str(), true) == false )
		return NULL;

	CSVMobData* pNewRowLst = NULL;
	{
		try{
			while( !reader.IsLineEmpty()  )
			{
				CSVMobRow* pRow = SafeCreateObject(CSVMobRow);
				if ( pRow )
				{
					reader.Read(_T("MobRate"),	pRow->uRange);
					reader.Read(_T("NumofColumn"),	pRow->nCount);

					for ( int i = 1; i <= pRow->nCount; ++i )
					{
						String sztemp("");
						sztemp.Format(_T("C%02d"), i);
						UInt32 tempid = 0;
						reader.Read(sztemp, tempid);
						pRow->MobLst.push_back(tempid);
					}

					if ( pNewRowLst == NULL )
					{
						pNewRowLst = SafeCreateObject(CSVMobData);
					}
					
					if ( pNewRowLst ) {
						pNewRowLst->RowLst.push_back(pRow);
					}
					else {
						SafeDeleteObject(pRow);
					}
				}
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CRandTableManager::LoadCSVMob: CSV reading error due to \
											   column format or missing MoveNext(), %s"), szfilename.c_str());
			reader.Close();
			return NULL;
		}
		reader.Close();
	}

	return pNewRowLst;
}

CONST CSVMobData* CRandTableManager::GetCSVMob(String szfilename)
{
	CSVMobTable::iterator itr = m_CSVMobList.find(szfilename);
	if ( itr != m_CSVMobList.end() )
		return (itr->second);
	return NULL;
}

VOID CRandTableManager::ReleaseNpcTable()
{
	for ( CSVNpcTable::iterator itr = m_CSVNpcList.begin(); itr != m_CSVNpcList.end(); ++itr )
		SafeDeleteObject(itr->second);
	m_CSVNpcList.clear();	
}

VOID CRandTableManager::SearchNpcFolder()
{
	StlVector<String> filequeue;
	filequeue.clear();

	{// - Search the folder of csv file
		String szPath(_T(""));
		szPath.Format(_T("%s"), _T("Data\\CSV\\RandomNpc\\*.csv"));

		WIN32_FIND_DATA data = {0};
		HANDLE hFile = ::FindFirstFile(szPath.c_str(), &data);
		if ( hFile && hFile != INVALID_HANDLE_VALUE )
		{
			do
			{
				String tempfile(_T(""));
				tempfile.Format(_T("%s"), data.cFileName);
				filequeue.push_back(tempfile);
			}
			while ( FindNextFile(hFile, &data) );
			FindClose( hFile );
		}
	}

	CSVNpcData* pTempLst = NULL;

	for ( StlVector<String>::iterator itr = filequeue.begin(); itr != filequeue.end(); ++itr )
	{
		String szFullPath(_T(""));
		szFullPath.Format(_T("Data\\CSV\\RandomNpc\\%s"), (*itr).c_str());
		pTempLst = LoadCSVNpc(szFullPath.c_str());
		if ( pTempLst )
		{
			String test = (*itr);
			m_CSVNpcList.insert( std::make_pair((*itr), pTempLst) );
		}
	}
}

CSVNpcData* CRandTableManager::LoadCSVNpc(String szfilename)
{
	CQGLCSVTReader reader;
	if( reader.Open(szfilename.c_str(), true) == false )
		return NULL;

	CSVNpcData* pNewRowLst = NULL;
	{
		try{
			while( !reader.IsLineEmpty()  )
			{
				CSVNpcRow* pRow = SafeCreateObject(CSVNpcRow);
				if ( pRow )
				{
					reader.Read(_T("NpcRate"),	pRow->uRange);
					reader.Read(_T("NumofColumn"),	pRow->nCount);

					for ( int i = 1; i <= pRow->nCount; ++i )
					{
						String sztemp("");
						sztemp.Format(_T("C%02d"), i);
						UInt32 tempid = 0;
						reader.Read(sztemp, tempid);
						pRow->NpcLst.push_back(tempid);
					}

					if ( pNewRowLst == NULL )
					{
						pNewRowLst = SafeCreateObject(CSVNpcData);
					}
					
					if ( pNewRowLst ) {
						pNewRowLst->RowLst.push_back(pRow);
					}
					else {
						SafeDeleteObject(pRow);
					}
				}
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CRandTableManager::LoadCSVNpc: CSV reading error due to \
											   column format or missing MoveNext(), %s"), szfilename.c_str());
			reader.Close();
			return NULL;
		}
		reader.Close();
	}

	return pNewRowLst;
}

CONST CSVNpcData* CRandTableManager::GetCSVNpc(String szfilename)
{
	CSVNpcTable::iterator itr = m_CSVNpcList.find(szfilename);
	if ( itr != m_CSVNpcList.end() )
		return (itr->second);
	return NULL;
}

VOID CRandTableManager::ReleaseQuestTable()
{
	for ( CSVQuestTable::iterator itr = m_CSVQuestList.begin(); itr != m_CSVQuestList.end(); ++itr )
		SafeDeleteObject(itr->second);
	m_CSVQuestList.clear();
}

VOID CRandTableManager::LoadCSVQuest(String szfilename)
{
	ReleaseQuestTable();

	CQGLCSVTReader reader;
	if( reader.Open(szfilename.c_str(), true) == false )
		return;
	{
		try{
			while( !reader.IsLineEmpty()  )
			{
				UInt32 Uid = 0;
				reader.Read(_T("Uid"),	Uid);
				CSVQuestTable::iterator it = m_CSVQuestList.find( Uid );
				if ( it == m_CSVQuestList.end() )
				{
					CSVQuestRow* pRow = SafeCreateObject(CSVQuestRow);
					if ( pRow )
					{
						pRow->Uid = Uid;

						reader.Read(_T("TypeId"),	pRow->TypeId);
						reader.ReadString(_T("TypeName"), pRow->TypeName, 64);
						reader.ReadString(_T("Name"), pRow->Name, 64);
						reader.Read(_T("Cancel"),	pRow->Cancel);
						reader.ReadString(_T("Descript"), pRow->Descript, 1024);

						reader.Read(_T("BeginNpcId"),	pRow->BeginNpcId);
						reader.Read(_T("BeginMap"),	pRow->BeginMapId);
						reader.Read(_T("BeginX"),	pRow->BeginX);
						reader.Read(_T("BeginY"),	pRow->BeginY);

						reader.Read(_T("FinishNpcId"),	pRow->FinishNpcId);
						reader.Read(_T("FinishMap"),	pRow->FinishMapId);
						reader.Read(_T("FinishX"),	pRow->FinishX);
						reader.Read(_T("FinishY"),	pRow->FinishY);

						reader.Read(_T("Npc01Id"),	pRow->Npc01Id);
						reader.Read(_T("Npc01Map"),	pRow->Npc01Map);
						reader.Read(_T("Npc01X"),	pRow->Npc01X);
						reader.Read(_T("Npc01Y"),	pRow->Npc01Y);

						reader.Read(_T("Npc02Id"),	pRow->Npc02Id);
						reader.Read(_T("Npc02Map"),	pRow->Npc02Map);
						reader.Read(_T("Npc02X"),	pRow->Npc02X);
						reader.Read(_T("Npc02Y"),	pRow->Npc02Y);

						reader.Read(_T("Item01Id"),	pRow->Item01Id);
						reader.Read(_T("Item01Max"), pRow->Item01Max);
						reader.Read(_T("Item01T1"),	pRow->Item01T1);
						reader.Read(_T("Item01V1"),	pRow->Item01V1);
						reader.Read(_T("Item01T2"),	pRow->Item01T2);
						reader.Read(_T("Item01V2"),	pRow->Item01V2);

						reader.Read(_T("Mob01Id"),	pRow->Mob01Id);
						reader.Read(_T("Mob01Max"),	pRow->Mob01Max);
						reader.Read(_T("Mob02Id"),	pRow->Mob02Id);
						reader.Read(_T("Mob02Max"),	pRow->Mob02Max);
						reader.Read(_T("Mob03Id"),	pRow->Mob03Id);
						reader.Read(_T("Mob03Max"),	pRow->Mob03Max);

						reader.Read(_T("Partner01Id"),	pRow->Partner01Id);
						reader.Read(_T("Partner01Max"),	pRow->Partner01Max);
						reader.Read(_T("Partner01T1"),	pRow->Partner01T1);
						reader.Read(_T("Partner01V1"),	pRow->Partner01V1);
						reader.Read(_T("Partner01T2"),	pRow->Partner01T2);

						reader.Read(_T("Pet01Id"),	pRow->Pet01Id);
						reader.Read(_T("Pet01Max"),	pRow->Pet01Max);

						reader.Read(_T("Map01Id"),	pRow->Map01Id);
						reader.Read(_T("Map01X"),	pRow->Map01X);
						reader.Read(_T("Map01Y"),	pRow->Map01Y);

						reader.Read(_T("Map02Id"),	pRow->Map02Id);
						reader.Read(_T("Map02X"),	pRow->Map02X);
						reader.Read(_T("Map02Y"),	pRow->Map02Y);

						reader.Read(_T("NewBieMinLv"),	pRow->NewBieMinLv);
						reader.Read(_T("NewBieMaxLv"),	pRow->NewBieMaxLv);
						reader.Read(_T("NewbieMax"),	pRow->NewBieMax);

						reader.Read(_T("EncountId"),	pRow->EncountId);
						reader.Read(_T("EncountMap"),	pRow->EncountMap);
						reader.Read(_T("EncountTime"),	pRow->EncountTime);
						reader.Read(_T("EncountRun"),	pRow->EncountRun);
						reader.Read(_T("EncountMax"),	pRow->EncountMax);

						reader.Read(_T("TimeLimit"),	pRow->TimeLimit);
						reader.Read(_T("Interval"),	pRow->Interval);
						reader.Read(_T("Buff"),	pRow->Buff);

						reader.Read(_T("FailOverTime"),	pRow->FailOverTime);
						reader.Read(_T("FailEscape"),	pRow->FailEscape);
						reader.Read(_T("FailPlayerDie"), pRow->FailPlayerDie);
						reader.Read(_T("FailTeamDie"),	pRow->FailTeamDie);					
						reader.Read(_T("FailFlowerDie"), pRow->FailFlowerDie);
						reader.Read(_T("FailNpcDie"),	pRow->FailNpcDie);
			
						m_CSVQuestList.insert(std::make_pair(pRow->Uid, pRow));
					}
				}
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CRandTableManager::LoadCSVQuest: CSV reading error due to \
											   column format or missing MoveNext(), %s"), szfilename.c_str());
			reader.Close();
			return;
		}
		reader.Close();
	}
}

CONST CSVQuestRow* CRandTableManager::GetCSVQuest(UInt32 Uid)
{
	CSVQuestTable::iterator itr = m_CSVQuestList.find(Uid);
	if ( itr != m_CSVQuestList.end() )
		return (itr->second);
	return NULL;
}