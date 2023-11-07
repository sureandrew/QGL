//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/QuestType.h"
//-- Library
#include "QGL/MenuQuestLogProcess.h"
#include "QGL/Global.h"

REGISTER_CLASSTYPE(CQuestType, QuestType_CLASSID);

void CQuestType::OnUpdateName(const String &name)
{
	if (gGlobal.g_pQuestLogProcess)
		gGlobal.g_pQuestLogProcess->UpdateCurrDescByType(GetType_id());
}

void CQuestType::OnUpdateRound(const UInt16 &round)
{
	if (gGlobal.g_pQuestLogProcess)
		gGlobal.g_pQuestLogProcess->UpdateCurrDescByType(GetType_id());
}

void CQuestType::OnUpdateMaxRound(const UInt16 &maxRound)
{
	if (gGlobal.g_pQuestLogProcess)
		gGlobal.g_pQuestLogProcess->UpdateCurrDescByType(GetType_id());
}

void CQuestType::OnUpdateTotalRound(const UInt16 &totalRound)
{
	if (gGlobal.g_pQuestLogProcess)
		gGlobal.g_pQuestLogProcess->UpdateCurrDescByType(GetType_id());
}

void CQuestType::OnUpdateLimitRound(const UInt16 &limitRound)
{
	if (gGlobal.g_pQuestLogProcess)
		gGlobal.g_pQuestLogProcess->UpdateCurrDescByType(GetType_id());
}

void CQuestType::OnUpdateLimitType(const UInt16 &limitType)
{
	if (gGlobal.g_pQuestLogProcess)
		gGlobal.g_pQuestLogProcess->UpdateCurrDescByType(GetType_id());
}

void CQuestType::OnUpdateLastClearTime(const UInt32 &lastClearTime)
{
	if (gGlobal.g_pQuestLogProcess)
		gGlobal.g_pQuestLogProcess->UpdateCurrDescByType(GetType_id());
}