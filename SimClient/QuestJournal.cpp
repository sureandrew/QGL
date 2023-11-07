//-- Common
#include "Common.h"
//-- Self
#include "SimClient/QuestJournal.h"
//-- Library
#include "SimClient/Character.h"
#include "SimClient/QuestSystem.h"
#include "SimClient/QuestType.h"
#include "SimClient/Global.h"

REGISTER_CLASSTYPE(CQuestJournal, QuestJournal_CLASSID);

void CQuestJournal::ShowDetail()
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pQuestSys == NULL)
		return;

	CQuestSystem* pQuestSys = gGlobal.m_pSimClient->m_pQuestSys;
	CCharacter* pChar = gGlobal.m_pSimClient->m_pCharacter;
	CQuestType* pType;
	String s;

	// level text
	if (GetMinLv() != 0 && pChar->GetLevel() < GetMinLv()) {
		s.Format(_T(">= Lv%d, "), GetMinLv());
	} else {
		s.Format(_T("Lv OK, "));
	}
	// round limit text
	if (GetLimitRound() != 0) {
		pType = pQuestSys->GetType(GetType_id());
		s.AppendFormat(_T("%d/%d"), pType ? pType->GetRound() : 0, GetLimitRound());
	} else {
		s.AppendFormat(_T("Unlimit"));
	}
}