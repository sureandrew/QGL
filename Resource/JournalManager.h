#pragma once
#ifndef _JOURNAL_MANAGER_H_
#define _JOURNAL_MANAGER_H_

struct JournalData;
typedef StlVector<const JournalData*, ManagedAllocator<const JournalData*> > JournalDataList;

class CJournalManager
{
public:
	CJournalManager();
	~CJournalManager();
	const JournalData* GetJournalData(UInt32 journal_id);
	void GetJournalList(JournalDataList &journalList);
	UInt16 GetSize();
	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);
	BOOL LoadDAT(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);
	VOID DebugPrint();

private:
	JournalDataList		m_journalList;
};

#endif // _EVENTSCUEDULE_MANAGER_H_
