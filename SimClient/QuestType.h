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
	virtual RPCResult CB_QuestTypeUpdate(LPCPEER pPeer, RPCContext &context, NetObject* questtype);
// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &char_id, const UInt32 &TypeID);
	virtual void OnUpdateRound(const UInt32 &Round);
	virtual void OnUpdateMaxround(const UInt32 &MaxRound);
	virtual void OnUpdateTotalround(const UInt32 &TotalRound);
	virtual void OnUpdateLimitround(const UInt32 &LimitRound);
	virtual void OnUpdateLimitType(const UInt32 &LimitType);
	virtual void OnUpdateLastClearTime(const UInt32 &LastClearTime);
};

#endif //_QUESTTYPE_H_
