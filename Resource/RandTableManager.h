#pragma once
#ifndef _RANDOM_TABLE_MANAGER_H_
#define _RANDOM_TABLE_MANAGER_H_

#include <Reuben\System\Reuben.h>
struct CSVItemData;
struct CSVMobData;
struct CSVNpcData;
struct CSVQuestRow;

class CRandTableManager
{
public:
	CRandTableManager();
	~CRandTableManager();

public:
	VOID Release();
	VOID SearchFolder();
	// random item table
	VOID SearchItemFolder();
	CSVItemData* LoadCSVItem(String szfilename);
	CONST CSVItemData* GetCSVItem(String szfilename);

	VOID ReleaseItemTable();

	// random mob table
	VOID SearchMobFolder();
	CSVMobData* LoadCSVMob(String szfilename);
	CONST CSVMobData* GetCSVMob(String szfilename);

	VOID ReleaseMobTable();
	// random npc table
	VOID SearchNpcFolder();
	CSVNpcData* LoadCSVNpc(String szfilename);
	CONST CSVNpcData* GetCSVNpc(String szfilename);

	VOID ReleaseNpcTable();
	
	// quest table
	VOID LoadCSVQuest(String szfilename);
	CONST CSVQuestRow* GetCSVQuest(UInt32 Uid);

	VOID ReleaseQuestTable();

private:
	typedef StlMap<String, CSVItemData*, std::less<String>, ManagedAllocator<std::pair<String, CSVItemData*> > > CSVItemTable;

	CSVItemTable	m_CSVItemList;

	typedef StlMap<String, CSVMobData*, std::less<String>, ManagedAllocator<std::pair<String, CSVMobData*> > > CSVMobTable;

	CSVMobTable		m_CSVMobList;

	typedef StlMap<String, CSVNpcData*, std::less<String>, ManagedAllocator<std::pair<String, CSVNpcData*> > > CSVNpcTable;

	CSVNpcTable		m_CSVNpcList;

	typedef StlMap<UInt32, CSVQuestRow*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CSVQuestRow*> > > CSVQuestTable;

	CSVQuestTable	m_CSVQuestList;
};

#endif // _RANDOM_TABLE_MANAGER_H_


