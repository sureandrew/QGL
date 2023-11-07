#pragma once
#ifndef _RESOURCE_EVENTSCUEDULE_H_
#define _RESOURCE_EVENTSCUEDULE_H_

#include <Reuben\System\Reuben.h>

enum EVENTSCUEDULE_EVENT_TYPE
{
	 EVENTSCUEDULE_EVENT_TYPE_INVALID = 0,
	 EVENTSCUEDULE_EVENT_TYPE_WORLD_BUFF,
	 EVENTSCUEDULE_EVENT_TYPE_NPC,
	 EVENTSCUEDULE_EVENT_TYPE_MAP,
	 EVENTSCUEDULE_EVENT_TYPE_SCRIPT
};

struct EventScheduleMapData
{
	UInt32 mapId;
	UInt32 x;
	UInt32 y;

	EventScheduleMapData()
		: mapId(C_UINT32(0))
		, x(C_UINT32(0))
		, y(C_UINT32(0))
	{};

	~EventScheduleMapData()	{};
};

typedef StlVector<EventScheduleMapData*, ManagedAllocator<EventScheduleMapData*> > EventScheduleMapDataVector;

class EventScheduleNpcData
{
public:
	UInt32Vector npcId;
	EventScheduleMapDataVector npcMapData;
	UInt32 npcMin;
	UInt32 npcMax;
	UInt32 npcLiveTime;
	UInt32 npcReborn;

	EventScheduleNpcData()
		: npcMin(0)
		, npcMax(0)
		, npcLiveTime(0)
		, npcReborn(0)
	{}

	~EventScheduleNpcData()
	{
		for( Index i = C_INDEX(0); i < npcMapData.size(); ++i )
		{
			SafeDeallocate(npcMapData[i]);
		}
	}
};

class EventScheduleBuffData
{
public:
	UInt32Vector buff;
	UInt32 buffPeriod;
	BOOL buffClear;
	BOOL removalOnLeaveMap;

	EventScheduleBuffData()
		: buffPeriod(0)
		, buffClear(FALSE)
		, removalOnLeaveMap(FALSE)
	{}
};

struct EventScheduleData
{
	UInt32 id;
	UInt32 type;
	TCHAR name[32];
	TCHAR descript[32];
	UInt32 line;
	UInt32 mapType;
	UInt32Vector map;

	UInt32 msgType;
	TCHAR msgStart[256];
	TCHAR msgPeriod[256];
	TCHAR msgKill[256];
	TCHAR msgEnd[256];
	time_t scheduleStart;
	time_t scheduleEnd;
	UInt32 startTime;
	UInt32 length;
	UInt32 period;
	UInt32 periodDay;
	UInt32 periodWeek;
	UInt32 periodMonth;
	UInt32 repeat;
	TCHAR script[256];

	EventScheduleNpcData* npcData;		// NOT NULL when type = 2 (EVENTSCUEDULE_EVENT_TYPE_NPC)
	EventScheduleBuffData* buffData;	// NOT NULL when type = 1 (EVENTSCUEDULE_EVENT_TYPE_WORLD_BUFF)

	EventScheduleData()
		: id(C_UINT32(0))
		, type(C_UINT32(0))
		, line(C_UINT32(0))
		, mapType(C_UINT32(0))
		, msgType(C_UINT32(0))
		, scheduleStart(0)
		, scheduleEnd(0)
		, startTime(C_UINT32(0))
		, length(C_UINT32(0))
		, period(C_UINT32(0))
		, periodDay(C_UINT32(0))
		, periodWeek(C_UINT32(0))
		, periodMonth(C_UINT32(0))
		, repeat(C_UINT32(0))
		, npcData(NULL)
		, buffData(NULL)
	{
		ZeroMemory(name, sizeof(TCHAR) * 32);
		ZeroMemory(descript, sizeof(TCHAR) * 32);
		ZeroMemory(msgStart, sizeof(TCHAR) * 256);
		ZeroMemory(msgPeriod, sizeof(TCHAR) * 256);
		ZeroMemory(msgKill, sizeof(TCHAR) * 256);
		ZeroMemory(msgEnd, sizeof(TCHAR) * 256);
	};

	~EventScheduleData()
	{
		SafeDeleteObject(npcData);
		SafeDeleteObject(buffData);
	};
};

#endif // _RESOURCE_EVENTSCUEDULE_H_
