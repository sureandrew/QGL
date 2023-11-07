//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/CallLuaFunc.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Logic/QuestLog.h"
//-- Library
#include "Configure.h"
#include "Stub/Logic/QuestSystem.h"
#include "Stub/Logic/Battle.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/PetGroup.h"
#include "Stub/Logic/ItemBag.h"
#include "Proxy/Data/QuestLogData.h"
#include "MapService/Global.h"
#include "Resource/ResRandTable.h"
#include "Resource/ResMob.h"
#include "Resource/ResNpc.h"
#include "Resource/ResItem.h"
#include "Resource/ResPet.h"
#include "Resource/ServerResManager.h"

#define DATE_T_20070101				1167494400

#define STATE_FAIL					0x0001
#define STATE_COMPLETE				0x0002

#define FEATURE_CANCEL				0x0001
#define FEATURE_FAIL_OVERTIME		0x0002
#define FEATURE_FAIL_ESCAPE			0x0004
#define FEATURE_FAIL_PLAYERDIE		0x0008
#define FEATURE_FAIL_TEAMDIE		0x0010
#define FEATURE_FAIL_FLOWERDIE		0x0020
#define FEATURE_FAIL_NPCDIE			0x0040
#define FEATURE_FAIL_JOINPARTY		0x0080
#define FEATURE_CANCEL_OVERTIME		0x0100

#define IS_BIT(X, B)				((X & B) == B)
#define SET_BIT(X, B)				(X | B)
#define UNSET_BIT(X, B)				(X & ~B)

PROTO_LuaPush(CQuestLog)

REGISTER_CLASSTYPE(CQuestLog, QuestLog_CLASSID);
// init class static values
RPCMask CQuestLog::m_dataMask = RPCMask();

CQuestLog::CQuestLog()
{
	m_dataMask.MaskBits(idMask, infoMask, beginNpcMask, finishNpcMask, 
		target1Mask, target2Mask, target3Mask, target4Mask, target5Mask, 
		timerMask, buffMask, stateMask, featureMask, statusMask, -1);

	m_pQuestSys = NULL;
	//m_actualLimitTime = 0;
	//m_actualLimitTime_updated = FALSE;
	m_targetTotal = 0;

	m_bQuestFailAfterLeaderChanged = false;
}

CQuestLog::~CQuestLog()
{
	CancelAllEvent();
	m_concernIdSet.clear();
}

void CQuestLog::PreSend()
{	
	RPCMask mask;

	mask.MaskBits(idMask, infoMask, stateMask, featureMask);

	if (GetBeginNpc() != 0)			mask.MaskBit(beginNpcMask);
	if (GetFinishNpc() != 0)		mask.MaskBit(finishNpcMask);
	if (m_targetTotal > 0)
		mask.MaskBits(target1Mask, target1_nameMask, -1);
	if (m_targetTotal > 1)
		mask.MaskBits(target2Mask, target2_nameMask, -1);
	if (m_targetTotal > 2)
		mask.MaskBits(target3Mask, target3_nameMask, -1);
	if (m_targetTotal > 3)
		mask.MaskBits(target4Mask, target4_nameMask, -1);
	if (m_targetTotal > 4)
		mask.MaskBits(target5Mask, target5_nameMask, -1);
	if (GetTimeLimit() != 0)		mask.MaskBit(timerMask);
	if (GetBuff() != 0)				mask.MaskBit(buffMask);
	if (IsUpdated(updateIconMask))	mask.MaskBit(updateIconMask);

	SetSendMask(mask);
}

void CQuestLog::Update()
{
	if (m_pQuestSys)
		m_pQuestSys->Update();
}

void CQuestLog::SetData(CQuestLogData *pData, bool bClear)
{
	ResetData();
	if (pData->IsUpdated(CQuestLogData::idMask))
	{
		Parent::SetId(pData->GetChar_id(), pData->GetQuest_id());
	}
	if (pData->IsUpdated(CQuestLogData::infoMask))
	{
		Parent::SetInfo(pData->GetType_id(), pData->GetName(), pData->GetDescript());
	}
	if (pData->IsUpdated(CQuestLogData::beginNpcMask))
	{
		Parent::SetBeginNpc(pData->GetBeginNpc(), pData->GetBeginNpc_Map(), 
					pData->GetBeginNpc_x(), pData->GetBeginNpc_y());
	}
	if (pData->IsUpdated(CQuestLogData::finishNpcMask))
	{
		Parent::SetFinishNpc(pData->GetFinishNpc(), pData->GetFinishNpc_Map(), 
					pData->GetFinishNpc_x(), pData->GetFinishNpc_y());
	}
	if (pData->IsUpdated(CQuestLogData::target1Mask))
	{
		UInt32 tid = pData->GetTarget1_id();
		Parent::SetTarget1(tid, pData->GetTarget1_v1(), pData->GetTarget1_v2(), 
					pData->GetTarget1_v3(), pData->GetTarget1_vtype(), 
					pData->GetTarget1_num(), pData->GetTarget1_max());
		if (tid > 0) {
			m_concernIdSet.insert(tid);
			SetTargetName(1);
		}
		m_targetTotal = 1;
	}
	if (pData->IsUpdated(CQuestLogData::target2Mask))
	{
		UInt32 tid = pData->GetTarget2_id();
		Parent::SetTarget2(tid, pData->GetTarget2_v1(), pData->GetTarget2_v2(), 
					pData->GetTarget2_v3(), pData->GetTarget2_vtype(), 
					pData->GetTarget2_num(), pData->GetTarget2_max());
		if (tid > 0) {
			m_concernIdSet.insert(tid);
			SetTargetName(2);
		}
		m_targetTotal = 2;
	}
	if (pData->IsUpdated(CQuestLogData::target3Mask))
	{
		UInt32 tid = pData->GetTarget3_id();
		Parent::SetTarget3(tid, pData->GetTarget3_v1(), pData->GetTarget3_v2(), 
					pData->GetTarget3_v3(), pData->GetTarget3_vtype(), 
					pData->GetTarget3_num(), pData->GetTarget3_max());
		if (tid > 0) {
			m_concernIdSet.insert(tid);
			SetTargetName(3);
		}
		m_targetTotal = 3;
	}
	if (pData->IsUpdated(CQuestLogData::target4Mask))
	{
		UInt32 tid = pData->GetTarget4_id();
		Parent::SetTarget4(tid, pData->GetTarget4_v1(), pData->GetTarget4_v2(), 
					pData->GetTarget4_v3(), pData->GetTarget4_vtype(), 
					pData->GetTarget4_num(), pData->GetTarget4_max());
		if (tid > 0) {
			m_concernIdSet.insert(tid);
			SetTargetName(4);
		}
		m_targetTotal = 4;
	}
	if (pData->IsUpdated(CQuestLogData::target5Mask))
	{
		UInt32 tid = pData->GetTarget5_id();
		Parent::SetTarget5(tid, pData->GetTarget5_v1(), pData->GetTarget5_v2(), 
					pData->GetTarget5_v3(), pData->GetTarget5_vtype(), 
					pData->GetTarget5_num(), pData->GetTarget5_max());
		if (tid > 0) {
			m_concernIdSet.insert(tid);
			SetTargetName(5);
		}
		m_targetTotal = 5;
	}
	if (pData->IsUpdated(CQuestLogData::timerMask))
	{
		Parent::SetTimer(pData->GetTimeLimit(), pData->GetInterval());
	}
	if (pData->IsUpdated(CQuestLogData::buffMask))
	{
		Parent::SetBuff(pData->GetBuff());
	}
	if (pData->IsUpdated(CQuestLogData::stateMask))
	{
		Parent::SetState(pData->GetState());
	}
	if (pData->IsUpdated(CQuestLogData::featureMask))
	{
		Parent::SetFeature(pData->GetFeature());
	}
	Parent::SetStatus(ST_NORMAL);

	if (bClear)
		ClearAllMask();

	if (IsFailOverTime() || IsCancelOverTime())
	{
		CheckOverTime();
		ScheduleTimeover();
	}
}

void CQuestLog::SetData(CONST CSVQuestRow* pInfo)
{
	String sname, desc;

	ResetData();
	sname.Format(_T("%s"), pInfo->Name);
	if (sname.GetLength() > QUEST_NAME_LEN)
		sname = sname.substr(0, QUEST_NAME_LEN - 1);
	desc.Format(_T("%s"), pInfo->Descript);
	if (desc.GetLength() > QUEST_DESC_LEN)
		desc = desc.substr(0, QUEST_DESC_LEN - 1);

	Parent::SetInfo(pInfo->TypeId, sname, desc);
	Parent::SetBeginNpc(pInfo->BeginNpcId, pInfo->BeginMapId, pInfo->BeginX, pInfo->BeginY);
	Parent::SetFinishNpc(pInfo->FinishNpcId, pInfo->FinishMapId, pInfo->FinishX, pInfo->FinishY);
/*
	Parent::SetTargetNpc(pInfo->Npc01Id, pInfo->Npc01Map, pInfo->Npc01X, pInfo->Npc01Y, 0,
		pInfo->Npc02Id, pInfo->Npc02Map, pInfo->Npc02X, pInfo->Npc02Y, 0);
	Parent::SetTargetItem(pInfo->Item01Id, 0, pInfo->Item01Max, pInfo->Item01T1, pInfo->Item01V1, 
		pInfo->Item01T2, pInfo->Item01V2);
	Parent::SetTargetMob(pInfo->Mob01Id, 0, pInfo->Mob01Max, 
		pInfo->Mob02Id, 0, pInfo->Mob02Max,
		pInfo->Mob03Id, 0, pInfo->Mob03Max);
	Parent::SetTargetPartner(pInfo->Partner01Id, 0, pInfo->Partner01Max, pInfo->Partner01T1, 
		pInfo->Partner01V1, pInfo->Partner01T2);
	Parent::SetTargetPet(pInfo->Pet01Id, 0, pInfo->Pet01Max);
	Parent::SetTargetMap(pInfo->Map01Id, pInfo->Map01X, pInfo->Map01Y, 0,
		pInfo->Map02Id, pInfo->Map02X, pInfo->Map02Y, 0);
	Parent::SetTargetNewbie(pInfo->NewBieMinLv, pInfo->NewBieMaxLv, 0, pInfo->NewBieMax);
	Parent::SetEncountNpc(pInfo->EncountId, pInfo->EncountMap, pInfo->EncountTime, 
		pInfo->EncountRun, 0, pInfo->EncountMax);
*/
	if (pInfo->TimeLimit > 0) {
		UInt32 now = gGlobal.GetServiceTime();
		Parent::SetTimer(pInfo->TimeLimit * 60 + now, pInfo->Interval);
	}
	Parent::SetBuff((UInt16) pInfo->Buff);

	SetCancel(pInfo->Cancel > 0);
	SetFailOverTime(pInfo->FailOverTime > 0);
	SetFailEscape(pInfo->FailEscape > 0);
	SetFailPlayerDie(pInfo->FailPlayerDie > 0);
	SetFailTeamDie(pInfo->FailTeamDie > 0);
	SetFailFlowerDie(pInfo->FailFlowerDie > 0);
	SetFailNpcDie(pInfo->FailNpcDie > 0);
	SetStatus(CQuestLog::ST_NEW);

	//ClearAllMask();
}

void CQuestLog::WriteData(CQuestLogData *pData)
{
	TRACE_ENSURE(pData);

	pData->ClearUpdateMask();
	if (IsUpdated(idMask))
	{
		pData->SetId(GetChar_id(), GetQuest_id());
	}
	if (IsUpdated(infoMask))
	{
		pData->SetInfo(GetType_id(), GetName(), GetDescript());
	}
	if (IsUpdated(beginNpcMask))
	{
		pData->SetBeginNpc(GetBeginNpc(), GetBeginNpc_Map(), 
					GetBeginNpc_x(), GetBeginNpc_y());
	}
	if (IsUpdated(finishNpcMask))
	{
		pData->SetFinishNpc(GetFinishNpc(), GetFinishNpc_Map(), 
					GetFinishNpc_x(), GetFinishNpc_y());
	}
	if (IsUpdated(target1Mask))
	{
		pData->SetTarget1(GetTarget1_id(), GetTarget1_v1(), GetTarget1_v2(), 
			GetTarget1_v3(), GetTarget1_vtype(), GetTarget1_num(), GetTarget1_max());
	}
	if (IsUpdated(target2Mask))
	{
		pData->SetTarget2(GetTarget2_id(), GetTarget2_v1(), GetTarget2_v2(), 
			GetTarget2_v3(), GetTarget2_vtype(), GetTarget2_num(), GetTarget2_max());
	}
	if (IsUpdated(target3Mask))
	{
		pData->SetTarget3(GetTarget3_id(), GetTarget3_v1(), GetTarget3_v2(), 
			GetTarget3_v3(), GetTarget3_vtype(), GetTarget3_num(), GetTarget3_max());
	}
	if (IsUpdated(target4Mask))
	{
		pData->SetTarget4(GetTarget4_id(), GetTarget4_v1(), GetTarget4_v2(), 
			GetTarget4_v3(), GetTarget4_vtype(), GetTarget4_num(), GetTarget4_max());
	}
	if (IsUpdated(target5Mask))
	{
		pData->SetTarget5(GetTarget5_id(), GetTarget5_v1(), GetTarget5_v2(), 
			GetTarget5_v3(), GetTarget5_vtype(), GetTarget5_num(), GetTarget5_max());
	}
	if (IsUpdated(timerMask))
	{
		pData->SetTimer(GetTimeLimit(), GetInterval());
	}
	if (IsUpdated(buffMask))
	{
		pData->SetBuff(GetBuff());
	}
	if (IsUpdated(stateMask))
	{
		pData->SetState(GetState());
	}
	if (IsUpdated(featureMask))
	{
		pData->SetFeature(GetFeature());
	}
	if (IsUpdated(statusMask))
	{
		pData->SetStatus(GetStatus());
	}

	if (pData->IsUpdated())
	{
		pData->SetId(GetChar_id(), GetQuest_id());
		pData->SetSendMask(pData->GetUpdateMask());
	}
}

void CQuestLog::ResetData()
{
	String emptyStr;

	Parent::SetBeginNpc(0, 0, 0, 0);
	Parent::SetFinishNpc(0, 0, 0, 0);
	Parent::SetTimer(0, 0);
	Parent::SetBuff(0);
	Parent::SetState(0);
	Parent::SetFeature(0);
	Parent::SetStatus(ST_NORMAL);

	ResetTarget();
}

void CQuestLog::ResetTarget()
{
	String emptyStr;

	Parent::SetTarget1(0, 0, 0, 0, 0, 0, 0);
	Parent::SetTarget2(0, 0, 0, 0, 0, 0, 0);
	Parent::SetTarget3(0, 0, 0, 0, 0, 0, 0);
	Parent::SetTarget4(0, 0, 0, 0, 0, 0, 0);
	Parent::SetTarget5(0, 0, 0, 0, 0, 0, 0);
	
	SetTarget1_name(emptyStr);
	SetTarget2_name(emptyStr);
	SetTarget3_name(emptyStr);
	SetTarget4_name(emptyStr);
	SetTarget5_name(emptyStr);

	m_concernIdSet.clear();
	m_targetTotal = 0;
}

Boolean CQuestLog::IsFailed()
{
	return IS_BIT(GetState(), STATE_FAIL);
}

void CQuestLog::SetFail(Boolean fail)
{
	SetState(fail ? SET_BIT(GetState(), STATE_FAIL) : 
					UNSET_BIT(GetState(), STATE_FAIL));

	if (fail && m_pQuestSys && m_pQuestSys->GetOwner())
	{
		if (HasEncountNPC())
			m_pQuestSys->GetOwner()->RemoveEncounter(GetQuest_id());
		if (HasFollower())
			RemoveFollower();
		if (IsFailOverTime())
			CancelUniqEvent(EVT_TIMEOVER);
		m_pQuestSys->RaiseUpdate();
	}
}

Boolean CQuestLog::IsCompleted()
{
	return IS_BIT(GetState(), STATE_COMPLETE);
}

void CQuestLog::SetComplete(Boolean completed)
{
	SetState(completed ? SET_BIT(GetState(), STATE_COMPLETE) :
					UNSET_BIT(GetState(), STATE_COMPLETE));
	if (completed && (IsFailOverTime() || IsCancelOverTime()))
		CancelUniqEvent(EVT_TIMEOVER);
}

Boolean CQuestLog::IsOverTime()
{
	return (gGlobal.GetServiceTime() >= GetTimeLimit());
}

void CQuestLog::SetTimeLimit(UInt32 uTime)
{
	UInt32 now = gGlobal.GetServiceTime();
	SetTimer(now + uTime, GetInterval());
	ScheduleTimeover();
}

UInt32 CQuestLog::GetTimeLimitEx()
{
	UInt32 now = gGlobal.GetServiceTime();
	UInt32 timeLimit = Parent::GetTimeLimit() - now;
	return ((timeLimit > 0) ? timeLimit : 0);
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

Boolean CQuestLog::CheckFinished(UInt32 npc_id, UInt16 map_id)
{
	if (IsFailed() || npc_id != 0 && (GetFinishNpc() != npc_id || GetFinishNpc_Map() != map_id))
		return BOOLEAN_FALSE;

	CheckAllTarget(npc_id, map_id);

	return IsCompleted();
}

void CQuestLog::SetCancel(Boolean enable)
{
	SetFeature(enable ? SET_BIT(GetFeature(), FEATURE_CANCEL) : 
						UNSET_BIT(GetFeature(), FEATURE_CANCEL));
}

void CQuestLog::SetFailOverTime(Boolean enable)
{
	SetFeature(enable ? SET_BIT(GetFeature(), FEATURE_FAIL_OVERTIME) :
						UNSET_BIT(GetFeature(), FEATURE_FAIL_OVERTIME));
	ScheduleTimeover();
}

void CQuestLog::SetFailEscape(Boolean enable)
{
	SetFeature(enable ? SET_BIT(GetFeature(), FEATURE_FAIL_ESCAPE) :
						UNSET_BIT(GetFeature(), FEATURE_FAIL_ESCAPE));
}

void CQuestLog::SetFailPlayerDie(Boolean enable)
{
	SetFeature(enable ? SET_BIT(GetFeature(), FEATURE_FAIL_PLAYERDIE) :
						UNSET_BIT(GetFeature(), FEATURE_FAIL_PLAYERDIE));
}

void CQuestLog::SetFailTeamDie(Boolean enable)
{
	SetFeature(enable ? SET_BIT(GetFeature(), FEATURE_FAIL_TEAMDIE) :
						UNSET_BIT(GetFeature(), FEATURE_FAIL_TEAMDIE));
}

void CQuestLog::SetFailFlowerDie(Boolean enable)
{
	SetFeature(enable ? SET_BIT(GetFeature(), FEATURE_FAIL_FLOWERDIE) :
						UNSET_BIT(GetFeature(), FEATURE_FAIL_FLOWERDIE));
}

void CQuestLog::SetFailNpcDie(Boolean enable)
{
	SetFeature(enable ? SET_BIT(GetFeature(), FEATURE_FAIL_NPCDIE) :
						UNSET_BIT(GetFeature(), FEATURE_FAIL_NPCDIE));
}

void CQuestLog::SetFailJoinParty(Boolean enable)
{
	SetFeature(enable ? SET_BIT(GetFeature(), FEATURE_FAIL_JOINPARTY) :
						UNSET_BIT(GetFeature(), FEATURE_FAIL_JOINPARTY));
}

void CQuestLog::SetCancelOverTime(Boolean enable)
{
	SetFeature(enable ? SET_BIT(GetFeature(), FEATURE_CANCEL_OVERTIME) : 
						UNSET_BIT(GetFeature(), FEATURE_CANCEL_OVERTIME));
	ScheduleTimeover();
}

void CQuestLog::SetTargetName(UInt8 index)
{
	UInt8 vtype = 0;
	UInt32 tid = 0;
	String name;

	switch (index)
	{
	case 1:
		vtype = GetTarget1_vtype();
		tid = GetTarget1_id();
		break;
	case 2:
		vtype = GetTarget2_vtype();
		tid = GetTarget2_id();
		break;
	case 3:
		vtype = GetTarget3_vtype();
		tid = GetTarget3_id();
		break;
	case 4:
		vtype = GetTarget4_vtype();
		tid = GetTarget4_id();
		break;
	case 5:
		vtype = GetTarget5_vtype();
		tid = GetTarget5_id();
		break;
	}
	switch (vtype) {
		case Q_TARGET_NPC:
			GetNpcName(tid, name);
			break;
		case Q_TARGET_MOB:
		case Q_TARGET_PARTNER:
		case Q_TARGET_FOLLOWER:
			GetMobName(tid, name);
			break;
		case Q_TARGET_ITEM:
			GetItemName(tid, name);
			break;
		case Q_TARGET_MAP:
			GetMapName(tid, name);
			break;
		case Q_TARGET_PET:
			GetPetName(tid, name);
			break;
	}

	if (!name.IsEmpty()) {
		switch (index)
		{
		case 1:
			SetTarget1_name(name);
			break;
		case 2:
			SetTarget2_name(name);
			break;
		case 3:
			SetTarget3_name(name);
			break;
		case 4:
			SetTarget4_name(name);
			break;
		case 5:
			SetTarget5_name(name);
			break;
		}
	}
}

void CQuestLog::CheckOverTime()
{
	bool overTime = (GetTimeLimit() > DATE_T_20070101 && 
					GetTimeLimit() <= gGlobal.GetServiceTime());

	if (!HasState() && IsFailOverTime() && overTime)
		SetFail(BOOLEAN_TRUE);

	if (!IsCompleted() && IsCancelOverTime() && overTime)
	{
		SetFail(BOOLEAN_TRUE);
		SetCancel(BOOLEAN_TRUE);
		if (m_pQuestSys)
		{
			PROCEDURE_DO_RPC1(QuestSystem, CancelQuest, m_pQuestSys, 
				m_pQuestSys->GetOwnerPeer(), m_pQuestSys->GetOwnerContext(), 
				UInt32, GetQuest_id())
		}
	}
}

void CQuestLog::CheckAfterBattle(UInt8 result, UInt32 npcId, const UInt32Vector& mobIndices)
{
	if (HasState() || gGlobal.m_resource->GetNpcData(npcId) == NULL)
		return;

	if (result == END_LOSE && (IsFailPlayerDie() || IsFailTeamDie()) ||
		result == END_ESCAPED && IsFailEscape())
	{
		if (npcId != 0 && IsConcernId(npcId) &&
			(FindTarget_id(Q_TARGET_NPC, npcId) > 0 || 
			FindTarget_id(Q_TARGET_ENCOUNT, npcId) > 0))
		{
			SetFail(BOOLEAN_TRUE);
		} else {
			UInt32 mob_id;
			for (UInt32Vector::const_iterator it = mobIndices.begin(); it != mobIndices.end(); ++it)
			{
				mob_id = *it;
				if (mob_id != 0 && IsConcernId(mob_id) && 
					FindTarget_id(Q_TARGET_MOB, mob_id) > 0)
				{
					SetFail(BOOLEAN_TRUE);
					break;
				}
			}
		}
	}
	else if	(result == END_WIN)
	{
		int index;
		/*if (IsConcernId(npcId)) {
			index = FindTarget_id(Q_TARGET_ENCOUNT, npcId);
			if (index > 0) {
				SetTarget_num(Q_TARGET_ENCOUNT, 0, 1, index);
			} else {
				index = FindTarget_id(Q_TARGET_NPC, npcId);
				if (index > 0)
					SetTarget_num(Q_TARGET_NPC, 0, 1, index);
			}
		}*/

		UInt32 mob_id;
		for (UInt32Vector::const_iterator it = mobIndices.begin(); it != mobIndices.end(); ++it)
		{
			mob_id = *it;
			if (mob_id > 0 && IsConcernId(mob_id)) {
				index = FindTarget_id(Q_TARGET_MOB, mob_id);
				SetTarget_num(Q_TARGET_MOB, 0, 1, index);
			}
		}
	}
}

void CQuestLog::CheckPartnerDie(UInt32 npcId)
{
	if (IsFailed() || gGlobal.m_resource->GetNpcData(npcId) == NULL || !IsFailFlowerDie())
		return;

	if (IsConcernId(npcId) && (FindTarget_id(Q_TARGET_NPC, npcId) > 0 ||
		FindTarget_id(Q_TARGET_ENCOUNT, npcId) > 0))
		SetFail(BOOLEAN_TRUE);
}

void CQuestLog::CheckNpcDie(UInt32 npcId)
{
	if (IsFailed() || gGlobal.m_resource->GetNpcData(npcId) == NULL || !IsFailNpcDie())
		return;

	if (IsConcernId(npcId) && (FindTarget_id(Q_TARGET_NPC, npcId) > 0 ||
		FindTarget_id(Q_TARGET_ENCOUNT, npcId) > 0))
		SetFail(BOOLEAN_TRUE);
}

void CQuestLog::CheckNpcTalk(UInt32 npc_id, UInt16 map_id)
{
	// if this npc is also one of target npc
	if (GetState() != 0 || !IsConcernId(npc_id))
		return;

	int index = FindTarget_id(Q_TARGET_NPC, npc_id);
	if (index > 0 && GetTarget_v1(Q_TARGET_NPC, 0, index) == map_id)
		SetTarget_num(Q_TARGET_NPC, 0, 1, index);
}

void CQuestLog::CheckItem(UInt32 item_id, int count)
{
	// if this item is also one of target item
	if (!IsConcernId(item_id))
		return;

	int index = FindTarget_id(Q_TARGET_ITEM, item_id);
	if (index > 0)
		SetTarget_num(Q_TARGET_ITEM, 0, count, index);
}

void CQuestLog::CheckPartner(UInt32 mob_id, int count)
{
	// if this item is also one of target partner
	if (!IsConcernId(mob_id))
		return;

	int index = FindTarget_id(Q_TARGET_PARTNER, mob_id);
	if (index > 0)
		SetTarget_num(Q_TARGET_PARTNER, 0, count, index);
}

void CQuestLog::CheckPet(UInt32 pet_id, int count)
{
	// if this item is also one of target pet
	if (!IsConcernId(pet_id))
		return;

	int index = FindTarget_id(Q_TARGET_PET, pet_id);
	if (index > 0)
		SetTarget_num(Q_TARGET_PET, 0, count, index);
}

void CQuestLog::CheckAllTarget(UInt32 npc_id, UInt16 map_id)
{
	if (m_pQuestSys == NULL || IsFailed())
		return;

	UInt32 tid;
	UInt8 vtype;
	UInt16 v1, v2, v3;
	UInt16 num, max, count;
	CCharacter* pchar = m_pQuestSys->GetOwner();
	bool completed = true;
	bool updated = false;

	if (pchar == NULL)
		return;

	for (int i = 1; i <= m_targetTotal; i++) {
		if (GetTarget(i, tid, v1, v2, v3, vtype, num, max)) {
			switch (vtype) {
			case Q_TARGET_NPC:
				if (num < max && npc_id != 0 && npc_id == tid && map_id == v1) {
					num ++;
					updated = true;
					SetTarget(0, tid, v1, v2, v3, vtype, 1, max, i);
				}
				break;
			case Q_TARGET_ITEM:
				count = pchar->GetItems()->GetItemCount(tid);
				if (count != num) {
					num = count;
					updated = true;
					SetTarget(0, tid, v1, v2, v3, vtype, num, max, i);
				}
				break;
			case Q_TARGET_PARTNER:
				count = pchar->GetPartners()->GetPartnerCount(tid);
				if (count != num) {
					num = count;
					updated = true;
					SetTarget(0, tid, v1, v2, v3, vtype, num, max, i);
				}
				break;
			case Q_TARGET_PET:
				count = pchar->GetPets()->GetIdCount(tid);
				if (count != num) {
					num = count;
					updated = true;
					SetTarget(0, tid, v1, v2, v3, vtype, num, max, i);
				}
				break;
			case Q_TARGET_ENCOUNT:
				num = max;		// don't check encounter count for completion
				break;
			}
			if (num < max) {
				completed = false;
				break;
			}
		}
	}
	if (completed != IsCompleted()) {
		SetComplete(completed);
		updated = true;
	}
	if (updated)
		m_pQuestSys->RaiseUpdate();
}

Boolean CQuestLog::GetTarget(int index, UInt32 &tid, UInt16 &v1, UInt16 &v2, UInt16 &v3, 
							UInt8 &vtype, UInt16 &num, UInt16 &max)
{
	Boolean r = BOOLEAN_FALSE;
	if (index == 1 && index <= m_targetTotal) {
		tid = GetTarget1_id();
		v1 = GetTarget1_v1();
		v2 = GetTarget1_v2();
		v3 = GetTarget1_v3();
		vtype = GetTarget1_vtype();
		num = GetTarget1_num();
		max = GetTarget1_max();
		r = BOOLEAN_TRUE;
	} else if (index == 2 && index <= m_targetTotal) {
		tid = GetTarget2_id();
		v1 = GetTarget2_v1();
		v2 = GetTarget2_v2();
		v3 = GetTarget2_v3();
		vtype = GetTarget2_vtype();
		num = GetTarget2_num();
		max = GetTarget2_max();
		r = BOOLEAN_TRUE;
	} else if (index == 3 && index <= m_targetTotal) {
		tid = GetTarget3_id();
		v1 = GetTarget3_v1();
		v2 = GetTarget3_v2();
		v3 = GetTarget3_v3();
		vtype = GetTarget3_vtype();
		num = GetTarget3_num();
		max = GetTarget3_max();
		r = BOOLEAN_TRUE;
	} else if (index == 4 && index <= m_targetTotal) {
		tid = GetTarget4_id();
		v1 = GetTarget4_v1();
		v2 = GetTarget4_v2();
		v3 = GetTarget4_v3();
		vtype = GetTarget4_vtype();
		num = GetTarget4_num();
		max = GetTarget4_max();
		r = BOOLEAN_TRUE;
	} else if (index == 5 && index <= m_targetTotal) {
		tid = GetTarget5_id();
		v1 = GetTarget5_v1();
		v2 = GetTarget5_v2();
		v3 = GetTarget5_v3();
		vtype = GetTarget5_vtype();
		num = GetTarget5_num();
		max = GetTarget5_max();
		r = BOOLEAN_TRUE;
	}
	return r;
}

bool CQuestLog::GetTargetNPC(int order)
{
	return (GetTarget_num(Q_TARGET_NPC, order) != 0);
}

UInt32 CQuestLog::GetTargetItemId(int order)
{
	return (GetTarget_id(Q_TARGET_ITEM, order));
}

UInt16 CQuestLog::GetTargetItemNum(int order)
{
	CheckAllTarget(0, 0);		// check if num need to update
	return (GetTarget_num(Q_TARGET_ITEM, order));
}

UInt16 CQuestLog::GetTargetItemMax(int order)
{
	return (GetTarget_max(Q_TARGET_ITEM, order));
}

UInt32 CQuestLog::GetTargetMobId(int order)
{
	return (GetTarget_id(Q_TARGET_MOB, order));
}

UInt16 CQuestLog::GetTargetMobNum(int order)
{
	return (GetTarget_num(Q_TARGET_MOB, order));
}

UInt16 CQuestLog::GetTargetMobMax(int order)
{
	return (GetTarget_max(Q_TARGET_MOB, order));
}

UInt32 CQuestLog::GetTargetNpcId(int order)
{
	return (GetTarget_id(Q_TARGET_NPC, order));
}

UInt16 CQuestLog::GetTargetNpcMap(int order)
{
	return (GetTarget_v1(Q_TARGET_NPC, order));
}

UInt16 CQuestLog::GetTargetNpcX(int order)
{
	return (GetTarget_v2(Q_TARGET_NPC, order));
}

UInt16 CQuestLog::GetTargetNpcY(int order)
{
	return (GetTarget_v3(Q_TARGET_NPC, order));
}

UInt16 CQuestLog::GetTargetNpcNum(int order)
{
	return (GetTarget_num(Q_TARGET_NPC, order));
}

UInt32 CQuestLog::GetTargetPartnerId(int order)
{
	return (GetTarget_id(Q_TARGET_PARTNER, order));
}

UInt16 CQuestLog::GetTargetPartnerNum(int order)
{
	CheckAllTarget(0, 0);		// check if num need to update
	return (GetTarget_num(Q_TARGET_PARTNER, order));
}

UInt16 CQuestLog::GetTargetPartnerMax(int order)
{
	return (GetTarget_max(Q_TARGET_PARTNER, order));
}

UInt16 CQuestLog::GetTargetNewBieNum(int order)
{
	return (GetTarget_num(Q_TARGET_NEWBIE, order));
}

UInt16 CQuestLog::GetTargetNewBieMax(int order)
{
	return (GetTarget_max(Q_TARGET_NEWBIE, order));
}

UInt32 CQuestLog::GetTargetPetId(int order)
{
	return (GetTarget_id(Q_TARGET_PET, order));
}

UInt16 CQuestLog::GetTargetPetNum(int order)
{
	CheckAllTarget(0, 0);		// check if num need to update
	return (GetTarget_num(Q_TARGET_PET, order));
}

UInt16 CQuestLog::GetTargetPetMax(int order)
{
	return (GetTarget_max(Q_TARGET_PET, order));
}

UInt32 CQuestLog::GetTargetFollowerId(int order)
{
	return (GetTarget_id(Q_TARGET_FOLLOWER, order));
}

UInt16 CQuestLog::GetTargetFollowerNum(int order)
{
	return (GetTarget_num(Q_TARGET_FOLLOWER, order));
}

void CQuestLog::SetName(PCSTRINGA name)
{
	StringW buf;
	if (name && UTF8ToStr(name, buf) > 0)
	{
		if (buf.GetLength() > QUEST_NAME_LEN)
			buf = buf.substr(0, QUEST_NAME_LEN - 1);
		Parent::SetName(buf);
	}
}

void CQuestLog::SetDescription(PCSTRINGA detail)
{
	StringW buf;
	if (detail && UTF8ToStr(detail, buf) > 0)
	{
		if (buf.GetLength() > QUEST_DESC_LEN)
			buf = buf.substr(0, QUEST_DESC_LEN - 1);
		SetDescript(buf); 
	}
}

void CQuestLog::SetFailCondition(PCSTRINGA cond)
{
	if (!cond == NULL)
		return;

	if (stricmp(cond, "overtime") == 0) 
		SetFailOverTime(BOOLEAN_TRUE);
	else if (stricmp(cond, "escape") == 0) 
		SetFailEscape(BOOLEAN_TRUE);
	else if (stricmp(cond, "playerdead") == 0) 
		SetFailPlayerDie(BOOLEAN_TRUE);
	else if (stricmp(cond, "alldead") == 0)
		SetFailTeamDie(BOOLEAN_TRUE);
	else if (stricmp(cond, "flowerdead") == 0)
		SetFailFlowerDie(BOOLEAN_TRUE);
	else if (stricmp(cond, "npcdead") == 0) 
		SetFailNpcDie(BOOLEAN_TRUE);
}

void CQuestLog::SetStartNPC(UInt32 npcId, UInt16 mapId, UInt16 posX, UInt16 posY)
{
	if (posX == 0 || posY == 0) {
		BinaryServerNPCInstance* npcInst = gGlobal.m_resource->GetNpcInstance(mapId, npcId);
		if (npcInst) {
			posX = npcInst->npc_position_x;
			posY = npcInst->npc_position_y;
		}
	}
	Parent::SetBeginNpc(npcId, mapId, posX, posY);
}

void CQuestLog::SetEndNPC(UInt32 npcId, UInt16 mapId, UInt16 posX, UInt16 posY)
{
	if (posX == 0 || posY == 0) {
		BinaryServerNPCInstance* npcInst = gGlobal.m_resource->GetNpcInstance(mapId, npcId);
		if (npcInst) {
			posX = npcInst->npc_position_x;
			posY = npcInst->npc_position_y;
		}
	}
	Parent::SetFinishNpc(npcId, mapId, posX, posY);
}

void CQuestLog::SetTargetNPC(int order, UInt32 npcId, UInt16 mapId, UInt16 posX, UInt16 posY)
{
	if (posX == 0 || posY == 0) {
		BinaryServerNPCInstance* npcInst = gGlobal.m_resource->GetNpcInstance(mapId, npcId);
		if (npcInst) {
			posX = npcInst->npc_position_x;
			posY = npcInst->npc_position_y;
		}
	}
	SetTarget(order, npcId, mapId, posX, posY, Q_TARGET_NPC, 0, 1);
}

void CQuestLog::FindTargetNPC(int order)
{
	SetTarget_num(Q_TARGET_NPC, order, 1);
}

void CQuestLog::SetTargetItem(int order, UInt32 itemId, UInt16 maxCount, UInt8 t1, UInt16 v1,
							  UInt8 t2, UInt16 v2)
{
	if (gGlobal.m_resource->GetItemData(itemId)) {
		UInt16 v3 = (((UInt16) t2 << 8) & 0xff00) | (t1 & 0xff);
		SetTarget(order, itemId, v1, v2, v3, Q_TARGET_ITEM, 0, maxCount);
	}
}

void CQuestLog::SetTargetMOB(int order, UInt32 mobId, UInt16 maxCount)
{
	if (gGlobal.m_resource->GetMobData(mobId))
		SetTarget(order, mobId, 0, 0, 0, Q_TARGET_MOB, 0, maxCount);
}

void CQuestLog::SetTargetPartner(int order, UInt32 mobId, UInt16 maxCount, UInt8 t1, UInt16 v1,
								 UInt8 t2)
{
	if (gGlobal.m_resource->GetMobData(mobId))
		SetTarget(order, mobId, v1, (UInt16) t1, t2, Q_TARGET_PARTNER, 0, maxCount);
}

void CQuestLog::SetTargetPet(int order, UInt32 mobId, UInt16 maxCount, UInt8 t1, UInt16 v1, 
							 UInt8 t2)
{
	if (gGlobal.m_resource->GetPetData(mobId))
		SetTarget(order, mobId, v1, (UInt16) t1, t2, Q_TARGET_PET, 0, maxCount);
}

void CQuestLog::SetTargetNewBie(UInt16 minLv, UInt16 maxLv, UInt16 num)
{
	if (minLv <= maxLv)
		SetTarget(1, 0, minLv, maxLv, 0, Q_TARGET_NEWBIE, 0, num);
}

void CQuestLog::SetEncountNPC(int order, UInt32 npcId, UInt16 mapId, UInt16 maxCount, UInt16 step)
{
	if (gGlobal.m_resource->GetNpcData(npcId))
	{
		int index = SetTarget(order, npcId, mapId, step, 0, Q_TARGET_ENCOUNT, 0, maxCount);
		// add encounter info in character owner
		if (m_pQuestSys && m_pQuestSys->GetOwner())
			m_pQuestSys->GetOwner()->AddEncounter((UInt8) index, GetQuest_id(), 
				npcId, mapId, step, 0, maxCount);
	}
}

void CQuestLog::IncEncountCount(UInt8 index)
{
	SetTarget_num(Q_TARGET_ENCOUNT, 0, 1, index);
}

Boolean CQuestLog::HasEncountNPC()
{
	return (GetTarget_id(Q_TARGET_ENCOUNT, 1) != 0);
}

void CQuestLog::CheckEncountNPC()
{
	if (m_pQuestSys == NULL || m_pQuestSys->GetOwner() == NULL || IsFailed())
		return;

	UInt32 npcId;
	UInt16 mapId, step, v3, num, max;
	UInt8 vtype; 

	for (UInt8 i = 1; i <= m_targetTotal; i++) {
		if (GetTarget(i, npcId, mapId, step, v3, vtype, num, max)) {
			if (vtype == Q_TARGET_ENCOUNT && num < max) {
				m_pQuestSys->GetOwner()->AddEncounter(i, GetQuest_id(), 
					npcId, mapId, step, num, max);
			}
		}
	}
}

void CQuestLog::SetTargetMap(int order, UInt16 mapId, UInt16 posX, UInt16 posY)
{
	if (gGlobal.m_resource->GetMapData(mapId))
		SetTarget(order, (UInt32) mapId, posX, posY, 0, Q_TARGET_MAP, 0, 1);
}

void CQuestLog::ArriveTargetMap(int order)
{
	SetTarget_num(Q_TARGET_MAP, order, 1);
}

void CQuestLog::SetTargetFollower(int order, UInt32 mobId, UInt16 count)
{
	if (gGlobal.m_resource->GetMobData(mobId))
	{
		SetTarget(order, mobId, 0, 0, 0, Q_TARGET_FOLLOWER, count, count);
		if (m_pQuestSys && m_pQuestSys->GetOwner())
			m_pQuestSys->GetOwner()->AddMobFollower(mobId);
	}
}

Boolean CQuestLog::HasFollower()
{
	return (GetTarget_id(Q_TARGET_FOLLOWER, 1) != 0);
}

void CQuestLog::CheckFollower()
{
	if (m_pQuestSys == NULL || m_pQuestSys->GetOwner() == NULL || IsFailed())
		return;

	UInt32 mobId;
	UInt16 v1, v2, v3, num, max;
	UInt8 vtype; 

	for (UInt8 i = 1; i <= m_targetTotal; i++)
	{
		if (GetTarget(i, mobId, v1, v2, v3, vtype, num, max))
		{
			if (vtype == Q_TARGET_FOLLOWER && num > 0)
				m_pQuestSys->GetOwner()->AddMobFollower(mobId);
		}
	}
}

void CQuestLog::RemoveFollower()
{
	if (m_pQuestSys == NULL || m_pQuestSys->GetOwner() == NULL)
		return;

	UInt32 mobId;
	UInt16 v1, v2, v3, num, max;
	UInt8 vtype; 

	for (UInt8 i = 1; i <= m_targetTotal; i++)
	{
		if (GetTarget(i, mobId, v1, v2, v3, vtype, num, max))
		{
			if (vtype == Q_TARGET_FOLLOWER && num > 0)
			{
				SetTarget_num(Q_TARGET_FOLLOWER, 0, - (int) num, i);
				m_pQuestSys->GetOwner()->RemoveMobFollower(mobId);
			}
		}
	}
}

int CQuestLog::FindTarget_id(UInt8 vtype, UInt32 tid)
{
	if (m_targetTotal == 0)
		return 0;
	else if (GetTarget1_vtype() == vtype && GetTarget1_id() == tid)
		return 1;
	else if (GetTarget2_vtype() == vtype && GetTarget2_id() == tid)
		return 2;
	else if (GetTarget3_vtype() == vtype && GetTarget3_id() == tid)
		return 3;
	else if (GetTarget4_vtype() == vtype && GetTarget4_id() == tid)
		return 4;
	else if (GetTarget5_vtype() == vtype && GetTarget5_id() == tid)
		return 5;
	else
		return 0;
}

int CQuestLog::SetTarget(int order, UInt32 tid, UInt16 v1, UInt16 v2, 
						  UInt16 v3, UInt8 vtype, UInt16 num, UInt16 max,
						  int index)
{
	int i = 0;
	int r = 0;
	if ((GetTarget1_vtype() == Q_TARGET_NONE || GetTarget1_vtype() == vtype)
		&& ((++i) == order || index == 1))
	{
		r = 1;
		SetTarget1(tid, v1, v2, v3, vtype, num, max);
		if (m_targetTotal < 1) m_targetTotal = 1;
	}
	else if ((GetTarget2_vtype() == Q_TARGET_NONE || GetTarget2_vtype() == vtype)
			&& ((++i) == order || index == 2))
	{
		r = 2;
		SetTarget2(tid, v1, v2, v3, vtype, num, max);
		if (m_targetTotal < 2) m_targetTotal = 2;
	}
	else if ((GetTarget3_vtype() == Q_TARGET_NONE || GetTarget3_vtype() == vtype)
			&& ((++i) == order || index == 3))
	{
		r = 3;
		SetTarget3(tid, v1, v2, v3, vtype, num, max);
		if (m_targetTotal < 3) m_targetTotal = 3;
	}
	else if ((GetTarget4_vtype() == Q_TARGET_NONE || GetTarget4_vtype() == vtype)
			&& ((++i) == order || index == 4))
	{
		r = 4;
		SetTarget4(tid, v1, v2, v3, vtype, num, max);
		if (m_targetTotal < 4) m_targetTotal = 4;
	}
	else if ((GetTarget5_vtype() == Q_TARGET_NONE || GetTarget5_vtype() == vtype)
			&& ((++i) == order || index == 5))
	{
		r = 5;
		SetTarget5(tid, v1, v2, v3, vtype, num, max);
		if (m_targetTotal < 5) m_targetTotal = 5;
	}
	if (r > 0) {
		if (tid > 0)
			m_concernIdSet.insert(tid);
		SetTargetName(r);
		SetState(0);				// clear fail/complete state
	}
	return r;
}

int CQuestLog::SetTarget_num(UInt8 vtype, int order, int num, int index)
{
	int i = 0;
	int r = 0;
	if (GetTarget1_vtype() == vtype && ((++i) == order || index == 1)) {
		r = 1;
		SetTarget1_num(MIN(GetTarget1_num() + num,  GetTarget1_max()));
	} else if (GetTarget2_vtype() == vtype && ((++i) == order || index == 2)) {
		r = 2;
		SetTarget2_num(MIN(GetTarget2_num() + num,  GetTarget2_max()));
	} else if (GetTarget3_vtype() == vtype && ((++i) == order || index == 3)) {
		r = 3;
		SetTarget3_num(MIN(GetTarget3_num() + num,  GetTarget3_max()));
	} else if (GetTarget4_vtype() == vtype && ((++i) == order || index == 4)) {
		r = 4;
		SetTarget4_num(MIN(GetTarget4_num() + num,  GetTarget4_max()));
	} else if (GetTarget5_vtype() == vtype && ((++i) == order || index == 5)) {
		r = 5;
		SetTarget5_num(MIN(GetTarget5_num() + num,  GetTarget5_max()));
	}
	//if (r > 0) SetState(0);				// clear fail/complete state

	return r;
}

UInt32 CQuestLog::GetTarget_id(UInt8 vtype, int order, int index)
{
	int i = 0;
	if (GetTarget1_vtype() == vtype && ((++i) == order || index == 1))
		return GetTarget1_id();
	else if (GetTarget2_vtype() == vtype && ((++i) == order || index == 2))
		return GetTarget2_id();
	else if (GetTarget3_vtype() == vtype && ((++i) == order || index == 3))
		return GetTarget3_id();
	else if (GetTarget4_vtype() == vtype && ((++i) == order || index == 4))
		return GetTarget4_id();
	else if (GetTarget5_vtype() == vtype && ((++i) == order || index == 5))
		return GetTarget5_id();
	else
		return 0;
}

UInt16 CQuestLog::GetTarget_v1(UInt8 vtype, int order, int index)
{
	int i = 0;
	if (GetTarget1_vtype() == vtype && ((++i) == order || index == 1))
		return GetTarget1_v1();
	else if (GetTarget2_vtype() == vtype && ((++i) == order || index == 2))
		return GetTarget2_v1();
	else if (GetTarget3_vtype() == vtype && ((++i) == order || index == 3))
		return GetTarget3_v1();
	else if (GetTarget4_vtype() == vtype && ((++i) == order || index == 4))
		return GetTarget4_v1();
	else if (GetTarget5_vtype() == vtype && ((++i) == order || index == 5))
		return GetTarget5_v1();
	else
		return 0;
}

UInt16 CQuestLog::GetTarget_v2(UInt8 vtype, int order, int index)
{
	int i = 0;
	if (GetTarget1_vtype() == vtype && ((++i) == order || index == 1))
		return GetTarget1_v2();
	else if (GetTarget2_vtype() == vtype && ((++i) == order || index == 2))
		return GetTarget2_v2();
	else if (GetTarget3_vtype() == vtype && ((++i) == order || index == 3))
		return GetTarget3_v2();
	else if (GetTarget4_vtype() == vtype && ((++i) == order || index == 4))
		return GetTarget4_v2();
	else if (GetTarget5_vtype() == vtype && ((++i) == order || index == 5))
		return GetTarget5_v2();
	else
		return 0;
}

UInt16 CQuestLog::GetTarget_v3(UInt8 vtype, int order, int index)
{
	int i = 0;
	if (GetTarget1_vtype() == vtype && ((++i) == order || index == 1))
		return GetTarget1_v3();
	else if (GetTarget2_vtype() == vtype && ((++i) == order || index == 2))
		return GetTarget2_v3();
	else if (GetTarget3_vtype() == vtype && ((++i) == order || index == 3))
		return GetTarget3_v3();
	else if (GetTarget4_vtype() == vtype && ((++i) == order || index == 4))
		return GetTarget4_v3();
	else if (GetTarget5_vtype() == vtype && ((++i) == order || index == 5))
		return GetTarget5_v3();
	else
		return 0;
}

UInt16 CQuestLog::GetTarget_num(UInt8 vtype, int order, int index)
{
	int i = 0;
	if (GetTarget1_vtype() == vtype && ((++i) == order || index == 1))
		return GetTarget1_num();
	else if (GetTarget2_vtype() == vtype && ((++i) == order || index == 2))
		return GetTarget2_num();
	else if (GetTarget3_vtype() == vtype && ((++i) == order || index == 3))
		return GetTarget3_num();
	else if (GetTarget4_vtype() == vtype && ((++i) == order || index == 4))
		return GetTarget4_num();
	else if (GetTarget5_vtype() == vtype && ((++i) == order || index == 5))
		return GetTarget5_num();
	else
		return 0;
}

UInt16 CQuestLog::GetTarget_max(UInt8 vtype, int order, int index)
{
	int i = 0;
	if (GetTarget1_vtype() == vtype && ((++i) == order || index == 1))
		return GetTarget1_max();
	else if (GetTarget2_vtype() == vtype && ((++i) == order || index == 2))
		return GetTarget2_max();
	else if (GetTarget3_vtype() == vtype && ((++i) == order || index == 3))
		return GetTarget3_max();
	else if (GetTarget4_vtype() == vtype && ((++i) == order || index == 4))
		return GetTarget4_max();
	else if (GetTarget5_vtype() == vtype && ((++i) == order || index == 5))
		return GetTarget5_max();
	else
		return 0;
}

void CQuestLog::GetNpcName(UInt32 npc_id, String &name)
{
	const NpcData* pNpcData = gGlobal.m_resource->GetNpcData(npc_id);
	if (pNpcData)
		name.Format(_T("%s%s"), pNpcData->pre_name, pNpcData->mid_name);
}

void CQuestLog::GetMobName(UInt32 mob_id, String &name)
{
	const MobData* pMobData = gGlobal.m_resource->GetMobData(mob_id);
	if (pMobData)
		name.Format(_T("%s%s"), pMobData->pre_name, pMobData->mid_name);		
}

void CQuestLog::GetItemName(UInt32 item_id, String &name)
{
	const ItemData* pItemData = gGlobal.m_resource->GetItemData(item_id);
	if (pItemData)
		name.Format(_T("%s"), pItemData->name);
}

void CQuestLog::GetMapName(UInt32 map_id, String &name)
{
	const MapDB* pMapInfo = gGlobal.m_resource->GetMapData(map_id);
	if (pMapInfo && pMapInfo->m_basicinfo && pMapInfo->m_basicinfo->m_mapname )
		name.Format(_T("%s"), pMapInfo->m_basicinfo->m_mapname );
}

void CQuestLog::GetPetName(UInt32 mob_id, String &name)
{
	const PetData* pPetData = gGlobal.m_resource->GetPetData(mob_id);
	if (pPetData)
		name.Format(_T("%s"), pPetData->pet_name);
}

Boolean CQuestLog::IsConcernId(UInt32 tid)
{
	return (m_concernIdSet.find(tid) != m_concernIdSet.end());
}

void CQuestLog::ScheduleTimeover()
{
	UInt32 now = gGlobal.GetServiceTime();
	if (!HasState() && (IsFailOverTime() || IsCancelOverTime()) && GetTimeLimit() > now)
		RaiseUniqEventID(EVT_TIMEOVER, (GetTimeLimit() - now) * 1000);
	else
		CancelUniqEvent(EVT_TIMEOVER);
}

void CQuestLog::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_TIMEOVER:				// quest timeover event
		CheckOverTime();
		break;
	}
}