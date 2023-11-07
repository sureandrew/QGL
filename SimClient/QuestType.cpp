//-- Common
#include "Common.h"
//-- Self
#include "SimClient/QuestType.h"
//-- Library

REGISTER_CLASSTYPE(CQuestType, QuestType_CLASSID);

RPCResult CQuestType::CB_QuestTypeUpdate(LPCPEER pPeer, RPCContext &context, NetObject* questtype)
{
	DeleteNetObject(questtype); // for preventing memory leak. 	
	return RPC_RESULT_OK;
}

void CQuestType::OnUpdateId(const UInt32 &char_id, const UInt32 &TypeID)
{
}

void CQuestType::OnUpdateRound(const UInt32 &Round)
{
}

void CQuestType::OnUpdateMaxround(const UInt32 &MaxRound)
{
}

void CQuestType::OnUpdateTotalround(const UInt32 &TotalRound)
{
}

void CQuestType::OnUpdateLimitround(const UInt32 &LimitRound)
{
}

void CQuestType::OnUpdateLimitType(const UInt32 &LimitType)
{
}

void CQuestType::OnUpdateLastClearTime(const UInt32 &LastClearTime)
{
}
