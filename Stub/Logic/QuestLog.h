#pragma once
#ifndef _QUESTLOG_H_
#define _QUESTLOG_H_

#include "Stub/Logic/QuestLog_Stub.h"

#define QUEST_NAME_LEN	8
#define QUEST_DESC_LEN	250
#define TYPE_NAME_LEN	8

struct CSVQuestRow;
class CQuestLogData;
class CQuestSystem;

enum EncountMapType
{
	ENC_MAP_ALL = 0,
	ENC_MAP_WILD,
	ENC_MAP_TOWN,
	ENC_MAP_CAVE,
	ENC_MAP_WILDCAVE,
	ENC_MAP_WILDTOWN,
	ENC_MAP_MAX,
};

class CQuestLog : public QuestLog_Stub, 
				public Reuben::Simulation::IEventHandler<CQuestLog>
{
public:
	CQuestLog();
	~CQuestLog();
	DEFINE_CLASS(QuestLog_CLASSID);

	INLINE static CQuestLog* NewInstance();

	void SetQuestSystem(CQuestSystem* pQuestSys) { m_pQuestSys = pQuestSys; }
	Boolean IsExist() { return GetStatus() != ST_DELETED; }
	Boolean IsModified() { return IsUpdated(m_dataMask); }
	void WriteData(CQuestLogData *pData);
	void SetData(CQuestLogData *pData, bool bClear=true);
	void SetData(CONST CSVQuestRow* pInfo);
	void ResetData();
	void ResetTarget();
	void PreSend();				// prepare to send
	void Update();

	INLINE Boolean HasState() { return GetState() != 0; }
	Boolean IsFailed();
	void SetFail(Boolean fail);
	Boolean IsOverTime();
	void SetTimeLimit(UInt32 uTime);
	UInt32 GetTimeLimitEx();
	Boolean IsCompleted();
	void SetComplete(Boolean completed);

	// feature query and enable/disable
	Boolean IsCancel();
	Boolean IsFailOverTime();
	Boolean IsFailEscape();
	Boolean IsFailPlayerDie();
	Boolean IsFailTeamDie();
	Boolean IsFailFlowerDie();
	Boolean IsFailNpcDie();
	Boolean IsFailJoinParty();
	Boolean IsCancelOverTime();
	void SetCancel(Boolean enable);
	void SetFailOverTime(Boolean enable);
	void SetFailEscape(Boolean enable);
	void SetFailPlayerDie(Boolean enable);
	void SetFailTeamDie(Boolean enable);
	void SetFailFlowerDie(Boolean enable);
	void SetFailNpcDie(Boolean enable);
	void SetFailJoinParty(Boolean enable);
	void SetCancelOverTime(Boolean enable);

	// set target name
	void SetTargetName(UInt8 index);

	// check and set fail condition with battle result
	void CheckOverTime();
	void CheckAfterBattle(UInt8 result, UInt32 npcId, const UInt32Vector& mobIndices);
	void CheckPartnerDie(UInt32 mobId);
	void CheckNpcDie(UInt32 npcId);
	void CheckNpcTalk(UInt32 npc_id, UInt16 map_id);
	void CheckItem(UInt32 item_id, int count);
	void CheckPartner(UInt32 mob_id, int count);
	void CheckPet(UInt32 pet_id, int count);
	void CheckAllTarget(UInt32 npc_id=0, UInt16 map_id=0);
	Boolean CheckFinished(UInt32 npc_id, UInt16 map_id);
	virtual void HandleEvent(CONST Event &event);

	INLINE Boolean IsQuestFailAfterLeaderChanged() { return m_bQuestFailAfterLeaderChanged; }
	INLINE void SetQuestFailAfterLeaderChanged(Boolean bFlag) { m_bQuestFailAfterLeaderChanged = bFlag; }

	enum StatusEnum
	{
		ST_NEW,
		ST_NORMAL,
		ST_DELETED,
	};

	enum EventEnum
	{
		EVT_TIMEOVER			= MAKE_EVENTID(QuestLog_CLASSID,  1),
	};

	// old compatible Lua API
public:
	/*
	void UpdateTimeLimit();		// calculate new time limit from actual limit time
	void SetActualLimitTime(UInt32 uTime);
	UInt32 GetActualLimitTime() { return m_actualLimitTime; };
	void ClearActualLimitTimeUpdate() { m_actualLimitTime_updated = FALSE; };
	Boolean IsActualLimitTimeUpdated() { return m_actualLimitTime_updated; };

	void LuaUpdateQuestData();
	*/
	void UpdateData() { Update(); }

	Boolean GetTarget(int index, UInt32 &tid, UInt16 &v1, UInt16 &v2, UInt16 &v3, 
						UInt8 &vtype, UInt16 &num, UInt16 &max);
	UInt32 GetTargetItemId(int order=1);
	UInt16 GetTargetItemNum(int order=1);
	UInt16 GetTargetItemMax(int order=1);
	UInt32 GetTargetMobId(int order=1);
	UInt16 GetTargetMobNum(int order=1);
	UInt16 GetTargetMobMax(int order=1);
	UInt32 GetTargetNpcId(int order=1);
	UInt16 GetTargetNpcMap(int order=1);
	UInt16 GetTargetNpcX(int order=1);
	UInt16 GetTargetNpcY(int order=1);
	UInt16 GetTargetNpcNum(int order=1);
	UInt32 GetTargetPartnerId(int order=1);
	UInt16 GetTargetPartnerNum(int order=1);
	UInt16 GetTargetPartnerMax(int order=1);
	UInt16 GetTargetNewBieNum(int order=1);
	UInt16 GetTargetNewBieMax(int order=1);
	UInt32 GetTargetPetId(int order=1);
	UInt16 GetTargetPetNum(int order=1);
	UInt16 GetTargetPetMax(int order=1);
	UInt32 GetTargetFollowerId(int order=1);
	UInt16 GetTargetFollowerNum(int order=1);
	UInt8  GetTargetTotal() { return m_targetTotal; }
	void SetName(PCSTRINGA name);
	void SetDescription(PCSTRINGA detail);
	void SetCancelEnable(bool bflag) { SetCancel(bflag); }
	void SetFailCondition(PCSTRINGA cond);
	void SetStartNPC(UInt32 npcId, UInt16 mapId, UInt16 posX=0, UInt16 posY=0);
	void SetEndNPC(UInt32 npcId, UInt16 mapId, UInt16 posX=0, UInt16 posY=0);
	void SetTargetNPC(int order, UInt32 npcId, UInt16 mapId, UInt16 posX=0, UInt16 posY=0);
	bool GetTargetNPC(int order);
	void FindTargetNPC(int order);
	void SetTargetItem(int order, UInt32 itemId, UInt16 maxCount, UInt8 t1, UInt16 v1,
						UInt8 t2, UInt16 v2);
	void SetTargetMOB(int order, UInt32 mobId, UInt16 maxCount);
	void SetTargetPartner(int order, UInt32 mobId, UInt16 maxCount, UInt8 t1, UInt16 v1, 
						UInt8 t2);
	void SetTargetPet(int order, UInt32 mobId, UInt16 maxCount, UInt8 t1, UInt16 v1, 
						UInt8 t2);
	void SetTargetNewBie(UInt16 minLv, UInt16 maxLv, UInt16 num);
	void SetEncountNPC(int order, UInt32 npcId, UInt16 mapId, UInt16 maxCount, UInt16 step);
	void IncEncountCount(UInt8 index);
	Boolean HasEncountNPC();
	void CheckEncountNPC();
	Boolean HasFollower();
	void CheckFollower();
	void RemoveFollower();
	void SetTargetMap(int order, UInt16 mapId, UInt16 posX, UInt16 posY);
	void ArriveTargetMap(int order);
	void SetTargetFollower(int order, UInt32 mobId, UInt16 count);

private:
	typedef QuestLog_Stub Parent;
	int FindTarget_id(UInt8 vtype, UInt32 tid);
	int SetTarget(int order, UInt32 tid, UInt16 v1, UInt16 v2, UInt16 v3, 
					UInt8 vtype, UInt16 num, UInt16 max, int index=0);
	int SetTarget_num(UInt8 vtype, int order, int num, int index=0);
	UInt32 GetTarget_id(UInt8 vtype, int order, int index=0);
	UInt16 GetTarget_v1(UInt8 vtype, int order, int index=0);
	UInt16 GetTarget_v2(UInt8 vtype, int order, int index=0);
	UInt16 GetTarget_v3(UInt8 vtype, int order, int index=0);
	UInt16 GetTarget_num(UInt8 vtype, int order, int index=0);
	UInt16 GetTarget_max(UInt8 vtype, int order, int index=0);
	void GetNpcName(UInt32 npc_id, String &name);
	void GetMobName(UInt32 mob_id, String &name);
	void GetItemName(UInt32 item_id, String &name);
	void GetMapName(UInt32 map_id, String &name);
	void GetPetName(UInt32 mob_id, String &name);
	Boolean IsConcernId(UInt32 tid);
	void ScheduleTimeover();

	typedef std::set<UInt32, std::less<UInt32>, ManagedAllocator<UInt32> > ConcernIdSet;
	CQuestSystem*	m_pQuestSys;
	UInt8			m_targetTotal;
	ConcernIdSet	m_concernIdSet;
	//UInt32			m_actualLimitTime;
	//BOOL			m_actualLimitTime_updated;
	static RPCMask	m_dataMask;		// quest log mask for QuestLogData exchange

	Boolean			m_bQuestFailAfterLeaderChanged; // quest fail if leader changed.
};

INLINE CQuestLog* CQuestLog::NewInstance()
{
	return CreateNetObject<CQuestLog>();
}

#endif //_QUESTLOG_H_
