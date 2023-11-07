//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/QuestJournal.h"
//-- Library
#include "Stub/Logic/Character.h"
#include "Stub/Logic/QuestSystem.h"
#include "MapService/Global.h"
#include "Resource/ResJournal.h"

REGISTER_CLASSTYPE(CQuestJournal, QuestJournal_CLASSID);

CQuestJournal::CQuestJournal()
{
	m_pData = NULL;
}

void CQuestJournal::SetData(const JournalData* pData)
{
	if (pData == NULL)
		return;

	m_pData = pData;
	SetJournal_id(pData->journal_id);
	SetName(pData->name);
	SetLevel(pData->minLv, pData->maxLv);
	SetDescript(pData->descript);
	SetTeamDescript(pData->teamStr);
	SetTimeStr(pData->timeStr);
	if (pData->startNpc != 0)
		SetNpcName(gGlobal.GetNpcName(pData->startNpc));
	if (pData->startMap != 0)
		SetMapName(gGlobal.GetMapName(pData->startMap));
	SetRound(pData->type_id, pData->limitRound);

	if (pData->DBClickScript[0] != _T('\0'))
		SetDBClickScript(pData->DBClickScript);

	SetSendMask(CQuestJournal::AllMask);
	ClearUpdateMask();
}

Boolean CQuestJournal::IsShown(CCharacter* pChar)
{
	if (m_pData == NULL)
		return BOOLEAN_FALSE;

	// check max level
	if (!(m_pData->maxLv == 0 || pChar->GetLevel() <= m_pData->maxLv))
		return BOOLEAN_FALSE;

	// check sex
	if (!((m_pData->sex == 0 || pChar->GetSex() == m_pData->sex)))
		return BOOLEAN_FALSE;

	// check job
	if (!m_pData->jobs.empty() && m_pData->jobs[0] != 0) {
		UInt8Vector::const_iterator it = std::find(m_pData->jobs.begin(), 
									m_pData->jobs.end(), pChar->GetSexClass());
		if (it == m_pData->jobs.end())
			return BOOLEAN_FALSE;
	}

	// check faction
	if (!m_pData->factions.empty() && m_pData->factions[0] != 0) {
		UInt8Vector::const_iterator it = std::find(m_pData->factions.begin(), 
									m_pData->factions.end(), pChar->GetFaction());
		if (it == m_pData->factions.end())
			return BOOLEAN_FALSE;
	}

	// check quest flag
	if (!(m_pData->questFlag == 0 || 
		pChar->GetQuests()->GetFlag(m_pData->questFlag) == m_pData->questFlagStep))
		return BOOLEAN_FALSE;

	// set enable or not
	time_t now = ::time(NULL);
	struct tm* ptime = localtime(&now);
	Boolean enable;

	// check weekday
	enable = (m_pData->wday == 0 || m_pData->wday == ptime->tm_wday || 
		m_pData->wday == ptime->tm_wday + 7);

	// check min level
	enable = enable && (m_pData->minLv == 0 || pChar->GetLevel() >= m_pData->minLv);

	// if enable changed
	if (GetEnable() != enable)
		SetEnable(enable);

	return BOOLEAN_TRUE;
}