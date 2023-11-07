#pragma once
#ifndef _QUESTLOGDATA_H_
#define _QUESTLOGDATA_H_

#include "Proxy/Data/QuestLogData_Proxy.h"

class CQuestLogData : public QuestLogData_Proxy
{
private:
	typedef QuestLogData_Proxy Parent;

public:
	DEFINE_CLASS(QuestLogData_CLASSID);
};

#endif //_QUESTLOGDATA_H_
