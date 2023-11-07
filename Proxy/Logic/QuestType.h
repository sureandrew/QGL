#pragma once
#ifndef _QUESTTYPE_H_
#define _QUESTTYPE_H_

#include "Proxy/Logic/QuestType_Proxy.h"

class CQuestType : public QuestType_Proxy
{
private:
	typedef QuestType_Proxy Parent;

public:
	DEFINE_CLASS(QuestType_CLASSID);
// OnUpdate can be implemented selectively
	virtual void OnUpdateName(const String &name);
	virtual void OnUpdateRound(const UInt16 &round);
	virtual void OnUpdateMaxRound(const UInt16 &maxRound);
	virtual void OnUpdateTotalRound(const UInt16 &totalRound);
	virtual void OnUpdateLimitRound(const UInt16 &limitRound);
	virtual void OnUpdateLimitType(const UInt16 &limitType);
	virtual void OnUpdateLastClearTime(const UInt32 &lastClearTime);
};

#endif //_QUESTTYPE_H_
