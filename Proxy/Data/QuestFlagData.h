#pragma once
#ifndef _QUESTFLAGDATA_H_
#define _QUESTFLAGDATA_H_

#include "Proxy/Data/QuestFlagData_Proxy.h"

class CQuestFlagData : public QuestFlagData_Proxy
{
private:
	typedef QuestFlagData_Proxy Parent;

public:
	DEFINE_CLASS(QuestFlagData_CLASSID);
};

#endif //_QUESTFLAGDATA_H_
