#pragma once
#ifndef _MENUQUESTJOURNAL_H_
#define _MENUQUESTJOURNAL_H_

class CQuestJournal;

class CMenuQuestJournal
{
private:
	//typedef StlDeque<CQuestJournal*, ManagedAllocator<CQuestJournal*> > JournalList;
	typedef StlMap<UInt32, CQuestJournal*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CQuestJournal*> > > JournalMap;

	//JournalList m_journalList;
	JournalMap m_journalEnableMap;
	JournalMap m_journalDisableMap;
	UInt m_iStartIndex;
	int m_iSelectedIndex;
	

public:
	CMenuQuestJournal();
	~CMenuQuestJournal();

	void ClearAllJournal(bool bDelete = false);

	void AddJournalToList(CQuestJournal* pJournal);
	void RemoveJournalFromList(CQuestJournal* pJournal);
	void ShowQuestJournal();
	void SelectJournal(int index);
	void DBClickJournal(int index);
	void JournalScroll(int scroll); //0: up, 1:down, 2: scroll
	bool IsOpenJournal();
	inline void CloseQuestJournal()
	{
		m_iStartIndex = 0;
		m_iSelectedIndex = -1;
	}

	CQuestJournal* GetJournalByMenuIndex(UInt index);

};

#endif 
