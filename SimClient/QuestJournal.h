#pragma once
#ifndef _QUESTJOURNAL_H_
#define _QUESTJOURNAL_H_

#include "Proxy/Logic/QuestJournal_Proxy.h"

class CQuestJournal : public QuestJournal_Proxy
{
public:
	// OnUpdate can be implemented selectively

	DEFINE_CLASS(QuestJournal_CLASSID);
	void ShowDetail();

private:
	typedef QuestJournal_Proxy Parent;

};

#endif //_QUESTJOURNAL_H_
