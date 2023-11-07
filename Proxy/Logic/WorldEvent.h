#pragma once
#ifndef _WORLDEVENT_H_
#define _WORLDEVENT_H_

#include "Proxy/Logic/WorldEvent_Proxy.h"

class CMap;
struct EventScheduleData;

class CWorldEvent : public WorldEvent_Proxy
{
public:
	DEFINE_CLASS(WorldEvent_CLASSID);
// OnUpdate can be implemented selectively
//	virtual void OnUpdateInfo(const UInt16 &eventId, const UInt8 &eventType);
//	virtual void OnUpdateNpcList(const NpcInfoVec &npcList);
//	virtual void OnUpdateNpcPosList(const PosInfoVec &npcPosList);
//	virtual void OnUpdateNpcLiveTime(const UInt32 &npcLiveTime);
//	virtual void OnUpdateBuffEvent(const UInt16Vector &buffList, const UInt16 &buffPeriod, const Boolean &buffClear, const Boolean &buffLeave);

	void Init();
	Boolean IsBuffEvent() { return GetEventType() == WE_TYPE_BUFF; }
	Boolean IsNpcEvent() { return GetEventType() == WE_TYPE_NPC; }
	Boolean IsMapEvent() { return GetEventType() == WE_TYPE_MAP; }
	UInt32 GetTimeLeft();
	void Execute(CMap *pMap);
	void Terminate(CMap *pMap);
	void UnmarkClosedMap();
	void MarkClosedMap();

	enum WE_Type
	{
		WE_TYPE_NONE = 0,
		WE_TYPE_BUFF,
		WE_TYPE_NPC,
		WE_TYPE_MAP,
		WE_TYPE_SCRIPT,
	};

private:
	void ActivateBuff(CMap* pMap);
	void DeactivateBuff(CMap* pMap);
	void CreateNpc(CMap* pMap);
	void RemoveNpc(CMap* pMap);
	void GetRandPos(CMap* pMap, UInt16* px, UInt16* py);
	Boolean BuffMatchMap(CMap* pMap);
	void OpenMap(CMap* pMap);
	void CloseMap(CMap* pMap);

	typedef WorldEvent_Proxy Parent;
	const EventScheduleData*	m_pData;
};

#endif //_WORLDEVENT_H_
