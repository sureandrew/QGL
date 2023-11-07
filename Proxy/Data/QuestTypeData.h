#pragma once
#ifndef _QUESTTYPEDATA_H_
#define _QUESTTYPEDATA_H_

#include "Proxy/Data/QuestTypeData_Proxy.h"

class CQuestTypeData : public QuestTypeData_Proxy
{
private:
	typedef QuestTypeData_Proxy Parent;

public:
	DEFINE_CLASS(QuestTypeData_CLASSID);
};

#endif //_QUESTTYPEDATA_H_
