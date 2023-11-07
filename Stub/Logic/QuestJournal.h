#pragma once
#ifndef _QUESTJOURNAL_H_
#define _QUESTJOURNAL_H_

#include "Stub/Logic/QuestJournal_Stub.h"

struct JournalData;
class CCharacter;

class CQuestJournal : public QuestJournal_Stub
{
public:
	DEFINE_CLASS(QuestJournal_CLASSID);
	CQuestJournal();
	Boolean IsShown(CCharacter* pChar);
	Boolean IsEnable(CCharacter* pChar);
	void SetData(const JournalData* pData);
	INLINE static CQuestJournal* NewInstance();

private:
	typedef QuestJournal_Stub Parent;

	const JournalData* m_pData;
};

INLINE CQuestJournal* CQuestJournal::NewInstance()
{
	return CreateNetObject<CQuestJournal>();
}

#endif //_QUESTJOURNAL_H_
