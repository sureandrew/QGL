#pragma once
#ifndef _RESOURCE_JOURNAL_H_
#define _RESOURCE_JOURNAL_H_

#include <Reuben\System\Reuben.h>

enum JournalType
{
	 JOURNAL_TYPE_QUEST = 0,
	 JOURNAL_TYPE_EVENT,
	 JOURNAL_TYPE_SPECIAL,
};

struct JournalData
{
	UInt16 journal_id;
	UInt8  jtype;
	UInt8  wday;
	String timeStr;
	String name;
	String descript;
	UInt32 startNpc;
	UInt16 startMap;
	String teamStr;
	UInt16 type_id;
	UInt16 limitRound;
	UInt16 minLv;
	UInt16 maxLv;
	UInt8  sex;
	UInt8Vector jobs;
	UInt8Vector factions;
	UInt16 questFlag;
	UInt32 questFlagStep;
	TCHAR DBClickScript[256];

	JournalData() : journal_id(0), jtype(0), wday(0), startNpc(0), startMap(0), type_id(0),
					limitRound(0), minLv(0), maxLv(0), sex(0), questFlag(0), questFlagStep(0)
	{
		DBClickScript[0] = _T('\0');
	};

	~JournalData()
	{};
};

#endif // _RESOURCE_EVENTSCUEDULE_H_
