//-- Common
#include "Common.h"
//-- Self
#include "SimClient/QuestLog.h"
//-- Library
#include "SimClient/ItemBag.h"
#include "SimClient/PartnerGroup.h"
#include "SimClient/Global.h"

#define STATE_FAIL					0x0001

#define FEATURE_CANCEL				0x0001
#define FEATURE_FAIL_OVERTIME		0x0002
#define FEATURE_FAIL_ESCAPE			0x0004
#define FEATURE_FAIL_PLAYERDIE		0x0008
#define FEATURE_FAIL_TEAMDIE		0x0010
#define FEATURE_FAIL_FLOWERDIE		0x0020
#define FEATURE_FAIL_NPCDIE			0x0040
#define FEATURE_FAIL_JOINPARTY		0x0080

#define IS_BIT(X, B)		((X & B) == B)
#define SET_BIT(X, B)		(X | B)
#define UNSET_BIT(X, B)		(X & ~B)

REGISTER_CLASSTYPE(CQuestLog, QuestLog_CLASSID);

CQuestLog::CQuestLog()
{
}

CQuestLog::~CQuestLog()
{
}

Boolean CQuestLog::IsFailed()
{
	return IS_BIT(GetState(), STATE_FAIL);
}

Boolean CQuestLog::IsOverTime()
{
	return BOOLEAN_FALSE;
}

Boolean CQuestLog::IsCancel()
{
	return IS_BIT(GetFeature(), FEATURE_CANCEL);
}

Boolean CQuestLog::IsFailOverTime()
{
	return IS_BIT(GetFeature(), FEATURE_FAIL_OVERTIME);
}

Boolean CQuestLog::IsFailEscape()
{
	return IS_BIT(GetFeature(), FEATURE_FAIL_ESCAPE);
}

Boolean CQuestLog::IsFailPlayerDie()
{
	return IS_BIT(GetFeature(), FEATURE_FAIL_PLAYERDIE);
}

Boolean CQuestLog::IsFailTeamDie()
{
	return IS_BIT(GetFeature(), FEATURE_FAIL_TEAMDIE);
}

Boolean CQuestLog::IsFailFlowerDie()
{
	return IS_BIT(GetFeature(), FEATURE_FAIL_FLOWERDIE);
}

Boolean CQuestLog::IsFailNpcDie()
{
	return IS_BIT(GetFeature(), FEATURE_FAIL_NPCDIE);
}

Boolean CQuestLog::IsFailJoinParty()
{
	return IS_BIT(GetFeature(), FEATURE_FAIL_JOINPARTY);
}

Boolean CQuestLog::IsCompleted(UInt32 npc_id, UInt16 map_id, UInt16 x, UInt16 y)
{
	if (IsFailed() || gGlobal.m_pSimClient == NULL)
		return BOOLEAN_FALSE;

	UInt32 targetId;
	UInt16 num, max;
	for (UInt8 i = 1; i <= 5; i++)
	{
		targetId = GetTarget_id(i);
		if (targetId == 0)				// no more target
			break;
		num = GetTarget_num(i);
		max = GetTarget_max(i);
		switch (GetTarget_vtype(i))
		{
		case Q_TARGET_NPC:
			if (targetId == npc_id && GetTarget_v1(i) == map_id && 
				GetTarget_v2(i) == x && GetTarget_v3(i) == y)
				num = 1;
			break;

		case Q_TARGET_ITEM:
			if (gGlobal.m_pSimClient->m_pBag)
				num = gGlobal.m_pSimClient->m_pBag->GetItemCount(targetId);
			break;

		case Q_TARGET_PARTNER:
			if (gGlobal.m_pSimClient->m_pPartnerGroup)
				num = gGlobal.m_pSimClient->m_pPartnerGroup->GetPartnerCount(targetId);
			break;

		case Q_TARGET_ENCOUNT:
			num = max;			// don't check encounter count for completion
			break;
		}
		if (num < max)
			return BOOLEAN_FALSE;
	}
	return BOOLEAN_TRUE;
}

Boolean CQuestLog::IsFinishNpc(UInt32 npc_id, UInt16 map_id, UInt16 x, UInt16 y)
{
	return (GetFinishNpc() == npc_id && GetFinishNpc_Map() == map_id && 
			GetFinishNpc_x() == x && GetFinishNpc_y() == y);
}

void CQuestLog::ShowDetail()
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Quest %d: type_id %d, name %s"), 
		GetQuest_id(), GetType_id(), (IsCompleted() ? _T(", Completed") : _T("")));
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("    : finish npc_id %d, map_id %d, pos %d,%d"),
		GetFinishNpc(), GetFinishNpc_Map(), GetFinishNpc_x(), GetFinishNpc_y());
	
	UInt32 id;
	UInt16 num, max;
	for (UInt8 i = 1; i <= 5; i++)
	{
		id = GetTarget_id(i);
		if (id == 0)				// no more target
			break;
		num = GetTarget_num(i);
		max = GetTarget_max(i);
		switch (GetTarget_vtype(i))
		{
		case Q_TARGET_NPC:
			TRACE_INFODTL_5(GLOBAL_LOGGER, _F("   %d:  target npc (%d,%d), %d/%d"), 
				i, id, GetTarget_v1(i), num, max);
			break;
		case Q_TARGET_MOB:
			TRACE_INFODTL_4(GLOBAL_LOGGER, _F("   %d:  target mob (%d), %d/%d"), 
				i, id, num, max);
			break;
		case Q_TARGET_ITEM:
			if (gGlobal.m_pSimClient->m_pBag)
				num = gGlobal.m_pSimClient->m_pBag->GetItemCount(id);
			TRACE_INFODTL_4(GLOBAL_LOGGER, _F("   %d:  target item (%d), %d/%d"), 
				i, id, num, max);
			break;

		case Q_TARGET_PARTNER:
			if (gGlobal.m_pSimClient->m_pPartnerGroup)
				num = gGlobal.m_pSimClient->m_pPartnerGroup->GetPartnerCount(id);
			TRACE_INFODTL_4(GLOBAL_LOGGER, _F("   %d:  target partner (%d), %d/%d"), 
				i, id, num, max);
			break;
		}
	}
}

void CQuestLog::OnUpdateUpdateIcon(const Boolean &updateIcon)
{
}

void CQuestLog::OnUpdateTimer(const UInt32 &TimeLimit, const UInt16 &Interval)
{
}