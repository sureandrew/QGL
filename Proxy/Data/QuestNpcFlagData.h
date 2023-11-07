#pragma once
#ifndef _QUESTNPCFLAGDATA_H_
#define _QUESTNPCFLAGDATA_H_

#include "Proxy/Data/QuestNpcFlagData_Proxy.h"

class CQuestNpcFlagData : public QuestNpcFlagData_Proxy
{
public:
	// OnUpdate can be implemented selectively

	DEFINE_CLASS(QuestNpcFlagData_CLASSID);

private:
	typedef QuestNpcFlagData_Proxy Parent;

};

#endif //_QUESTNPCFLAGDATA_H_
