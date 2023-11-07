//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/QuestLog.h"
//-- Library
#include "Proxy/Logic/QuestSystem.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "QGL/MenuQuestLogProcess.h"
#include "QGL/Global.h"

#define STATE_FAIL					0x0001

#define FEATURE_CANCEL				0x0001
#define FEATURE_FAIL_OVERTIME		0x0002
#define FEATURE_FAIL_ESCAPE			0x0004
#define FEATURE_FAIL_PLAYERDIE		0x0008
#define FEATURE_FAIL_TEAMDIE		0x0010
#define FEATURE_FAIL_FLOWERDIE		0x0020
#define FEATURE_FAIL_NPCDIE			0x0040
#define FEATURE_FAIL_JOINPARTY		0x0080
#define FEATURE_CANCEL_OVERTIME		0x0100

#define IS_BIT(X, B)		((X & B) == B)
#define SET_BIT(X, B)		(X | B)
#define UNSET_BIT(X, B)		(X & ~B)

REGISTER_CLASSTYPE(CQuestLog, QuestLog_CLASSID);

CQuestLog::CQuestLog()
{
	m_LastSyncOfTimeLimit = 0;
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

Boolean CQuestLog::IsCancelOverTime()
{
	return IS_BIT(GetFeature(), FEATURE_CANCEL_OVERTIME);
}

void CQuestLog::OnUpdateTarget1(const UInt32 &target1_id, const UInt16 &target1_v1, 
								const UInt16 &target1_v2, const UInt16 &target1_v3, 
								const UInt8 &target1_vtype, const UInt16 &target1_num, 
								const UInt16 &target1_max)
{
	if (gGlobal.g_QuestSystem)
		gGlobal.g_QuestSystem->RefreshNpcHint();
}

void CQuestLog::OnUpdateTarget2(const UInt32 &target2_id, const UInt16 &target2_v1, 
								const UInt16 &target2_v2, const UInt16 &target2_v3, 
								const UInt8 &target2_vtype, const UInt16 &target2_num, 
								const UInt16 &target2_max)
{
	if (gGlobal.g_QuestSystem)
		gGlobal.g_QuestSystem->RefreshNpcHint();
}

void CQuestLog::OnUpdateTarget3(const UInt32 &target3_id, const UInt16 &target3_v1, 
								const UInt16 &target3_v2, const UInt16 &target3_v3, 
								const UInt8 &target3_vtype, const UInt16 &target3_num, 
								const UInt16 &target3_max)
{
	if (gGlobal.g_QuestSystem)
		gGlobal.g_QuestSystem->RefreshNpcHint();
}

void CQuestLog::OnUpdateTarget4(const UInt32 &target4_id, const UInt16 &target4_v1, 
								const UInt16 &target4_v2, const UInt16 &target4_v3, 
								const UInt8 &target4_vtype, const UInt16 &target4_num, 
								const UInt16 &target4_max)
{
	if (gGlobal.g_QuestSystem)
		gGlobal.g_QuestSystem->RefreshNpcHint();
}

void CQuestLog::OnUpdateTarget5(const UInt32 &target5_id, const UInt16 &target5_v1, 
								const UInt16 &target5_v2, const UInt16 &target5_v3, 
								const UInt8 &target5_vtype, const UInt16 &target5_num, 
								const UInt16 &target5_max)
{
	if (gGlobal.g_QuestSystem)
		gGlobal.g_QuestSystem->RefreshNpcHint();
}

void CQuestLog::OnUpdateUpdateIcon(const Boolean &updateIcon)
{
	if (gGlobal.g_pQuestLogProcess )
		gGlobal.g_pQuestLogProcess->ModifyQuest(GetQuest_id(), GetType_id(), this, updateIcon);
}

void CQuestLog::OnUpdateTimer(const UInt32 &TimeLimit, const UInt16 &Interval)
{
	time_t now;
	time(&now);
	m_LastSyncOfTimeLimit = (UInt32) now;
}

UInt32 CQuestLog::GetRealTime()
{
	return m_LastSyncOfTimeLimit + GetTimeLimit() ;
}

Boolean CQuestLog::IsCompleted(UInt32 npc_id, UInt16 map_id, UInt16 x, UInt16 y)
{
	if (IsFailed())
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
			if (gGlobal.m_pBag)
				num = gGlobal.m_pBag->GetItemCount(targetId);
			break;

		case Q_TARGET_PARTNER:
			if (gGlobal.g_pPartnerGroup)
				num = gGlobal.g_pPartnerGroup->GetPartnerCount(targetId);
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

Boolean CQuestLog::IsRelated(UInt16 qtype, UInt32 id)
{
	UInt16 targetId = 0;
	for (UInt8 i = 1; i <= 5; i++)
	{
		targetId = GetTarget_id(i);
		if (targetId == 0)				// no more target
			break;
		if (id == targetId && qtype == GetTarget_vtype(i))
			return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}