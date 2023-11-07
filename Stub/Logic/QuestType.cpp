//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/QuestType.h"
//-- Library
#include "MapService/Global.h"
#include "Stub/Logic/QuestSystem.h"
#include "Proxy/Data/QuestTypeData.h"

REGISTER_CLASSTYPE(CQuestType, QuestType_CLASSID);

#define WEEKDAY_OFFSET					345600			// 1970/1/1 sunday offset
#define WEEK_SECOND						604800
#define DAY_SECOND						86400

CQuestType::CQuestType()
{
	m_pQuestSys = NULL;
}

void CQuestType::PreSend()
{	
	RPCMask mask;
	mask.MaskBits(idMask, nameMask, roundMask, maxRoundMask, totalRoundMask,
		limitRoundMask, limitTypeMask, -1);
	SetSendMask(mask);
}

void CQuestType::SetData(CQuestTypeData *pData)
{
	if (pData->IsUpdated(CQuestTypeData::idMask))
	{
		SetId(pData->GetChar_id(), pData->GetType_id());
	}
	if (pData->IsUpdated(CQuestTypeData::nameMask))
	{
		SetName(pData->GetName());
	}
	if (pData->IsUpdated(CQuestTypeData::roundMask))
	{
		SetRound(pData->GetRound());
	}
	if (pData->IsUpdated(CQuestTypeData::maxRoundMask))
	{
		SetMaxRound(pData->GetMaxRound());
	}
	if (pData->IsUpdated(CQuestTypeData::totalRoundMask))
	{
		SetTotalRound(pData->GetTotalRound());
	}
	if (pData->IsUpdated(CQuestTypeData::limitRoundMask))
	{
		SetLimitRound(pData->GetLimitRound());
	}
	if (pData->IsUpdated(CQuestTypeData::limitTypeMask))
	{
		SetLimitType(pData->GetLimitType());
	}
	if (pData->IsUpdated(CQuestTypeData::lastClearTimeMask))
	{
		SetLastClearTime(pData->GetLastClearTime());
	}

	SetStatus(ST_NORMAL);

	ClearAllMask();
}

void CQuestType::WriteData(CQuestTypeData *pData)
{
	TRACE_ENSURE(pData);

	pData->ClearUpdateMask();
	if (IsUpdated(roundMask)) {
		pData->SetRound(GetRound());
	}
	if (IsUpdated(nameMask)) {
		pData->SetName(GetName());
	}
	if (IsUpdated(maxRoundMask)) {
		pData->SetMaxRound(GetMaxRound());
	}
	if (IsUpdated(totalRoundMask)) {
		pData->SetTotalRound(GetTotalRound());
	}
	if (IsUpdated(limitRoundMask)) {
		pData->SetLimitRound(GetLimitRound());
	}
	if (IsUpdated(limitTypeMask)) {
		pData->SetLimitType(GetLimitType());
	}
	if (IsUpdated(lastClearTimeMask)) {
		pData->SetLastClearTime(GetLastClearTime());
	}
	if (IsUpdated(statusMask)) {
		pData->SetStatus(GetStatus());
	}

	if (pData->IsUpdated()) {
		pData->SetId(GetChar_id(), GetType_id());
		pData->SetSendMask(pData->GetUpdateMask());
	}
}

void CQuestType::Update()
{
	if (m_pQuestSys)
		m_pQuestSys->Update();
}

void CQuestType::ResetData()
{
	SetId(0, 0);
	SetName(_T(""));
	SetRound(0);
	SetMaxRound(0);
	SetTotalRound(0);
	SetLimitRound(0);
	SetLimitType(0);
	SetLastClearTime(0);
	SetStatus(ST_NORMAL);
}

void CQuestType::ChangeRound(Int change)
{
	if (GetMaxRound() > 0 && change != 0)
	{
		SetRound(WITHIN((Int) GetRound() + change, 0, GetMaxRound() + 1));
		Update();
	}
}

void CQuestType::ChangeTotalRound(Int change)
{
	if (GetLimitRound() > 0 && change != 0)
	{
		SetClearTime();
		SetTotalRound(WITHIN((Int) GetTotalRound() + change, 0, GetLimitRound() + 1));
		Update();
	}
}

void CQuestType::SetTotalRound(const UInt16 &totalRound)
{
	SetClearTime();
	Parent::SetTotalRound(totalRound);
}

void CQuestType::SetClearTime()
{
	UInt32 clearTime = 0;
	time_t now_t = (time_t)gGlobal.GetServiceTime();

	struct tm* ptime = localtime(&now_t);
	ptime->tm_hour = 0;
	ptime->tm_min = 0;
	ptime->tm_sec = 0;

	if (GetLimitType() == TYPE_PER_WEEK)		 // back to Monday 00:00:00
		clearTime = ((UInt32)(mktime(ptime) - ((ptime->tm_wday + 6) % 7) * DAY_SECOND));
	else
		clearTime = ((UInt32) mktime(ptime));

	if (clearTime != GetLastClearTime())
		SetLastClearTime(clearTime);
}