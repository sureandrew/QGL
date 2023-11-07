//-- Common
#include "Common.h"
//-- Self
#include "Resource/JournalManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/File.h>
#include "Resource/ResJournal.h"
#include "Resource/QGLCSVTReader.h"

typedef Reuben::Platform::File File;
typedef Reuben::Platform::FileException FileException;

////////////// Journal Manager //////////////////////////////////

CJournalManager::CJournalManager()
{
}

CJournalManager::~CJournalManager()
{
	for (JournalDataList::iterator it = m_journalList.begin(); it != m_journalList.end(); ++it)
	{
		JournalData* pJournal = (JournalData*) (*it);
		SafeDeleteObject(pJournal);
	}
	m_journalList.clear();
}

const JournalData* CJournalManager::GetJournalData(UInt32 journal_id)
{
	for (JournalDataList::iterator it = m_journalList.begin(); it != m_journalList.end(); ++it)
	{
		if ((*it)->journal_id == journal_id)
			return *it;
	}
	return NULL;
}

void CJournalManager::GetJournalList(JournalDataList &journalList)
{
	journalList = m_journalList;
}

BOOL CJournalManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{
	CQGLCSVTReader reader;
	BOOL bOpen = FALSE;
	if(pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		bOpen = TRUE;
	else if( reader.Open(filename, true) )
		bOpen = TRUE;
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: JournalManager::LoadCSV: file error, %s"), filename);
		return FALSE;
	}
	
	JournalData* pJournal = NULL;
	try {
		while(!reader.IsLineEmpty())
		{
			pJournal = SafeCreateObject(JournalData);
			// basic info
			reader.Read(		_T("Journal_Id"),		pJournal->journal_id);
			reader.Read(		_T("Type"),				pJournal->jtype);
			reader.Read(		_T("Weekday"),			pJournal->wday);
			reader.ReadString(	_T("Time"),				pJournal->timeStr);
			reader.ReadString(	_T("Name"),				pJournal->name);
			reader.ReadString(	_T("Description"),		pJournal->descript);
			reader.Read(		_T("StartNpc"),			pJournal->startNpc);
			reader.Read(		_T("StartMap"),			pJournal->startMap);
			reader.ReadString(	_T("Team"),				pJournal->teamStr);
			reader.Read(		_T("Type_id"),			pJournal->type_id);
			reader.Read(		_T("LimitRound"),		pJournal->limitRound);
			reader.Read(		_T("MinLv"),			pJournal->minLv);
			reader.Read(		_T("MaxLv"),			pJournal->maxLv);
			reader.Read(		_T("Sex"),				pJournal->sex);
			reader.ReadVector(	_T("Job"),				pJournal->jobs);
			reader.ReadVector(	_T("Faction"),			pJournal->factions);
			reader.Read(		_T("QuestFlag"),		pJournal->questFlag);
			reader.Read(		_T("QuestFlagStep"),	pJournal->questFlagStep);
			reader.ReadString(	_T("DBClickScript"),		pJournal->DBClickScript, 255);

			m_journalList.push_back(pJournal);
			reader.MoveNext();
		}
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Initialization: %d quest journal(s) read"), 
			m_journalList.size());
	} catch(...) {
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("JournalManager LoadCSV Error: ") 
			_T("CSV reading error due to column format or missing MoveNext(), %s"), 
			filename);
		reader.Close();
		return FALSE;
	}
	reader.Close();

	return TRUE;
}

BOOL CJournalManager::LoadDAT(const TCHAR *filename)
{
	TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CJournalManager::LoadDAT Error (1) [function missing], filename=%s"), filename);
	return FALSE;
}

BOOL CJournalManager::SaveDAT(const TCHAR *filename)
{
	TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CJournalManager::SaveDAT Error (1) [function missing], filename=%s"), filename);
	return FALSE;
}

UInt16 CJournalManager::GetSize()
{
	return ((UInt16) m_journalList.size());
}

VOID CJournalManager::DebugPrint()
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("JournalManager: size = %d"), GetSize());
	for (JournalDataList::iterator it = m_journalList.begin(); it != m_journalList.end(); ++it)
	{
		const JournalData* pJournal = *it;
		if (pJournal)
		{
			TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Journal_id=%d, Type=%d, Wday=%d, Time=%s"), 
				pJournal->journal_id, pJournal->jtype, pJournal->wday, 
				pJournal->timeStr.c_str());
		}
	}
}
