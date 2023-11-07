#pragma once
#ifndef _QUESTLOG_H_
#define _QUESTLOG_H_

#include "Proxy/Logic/QuestLog_Proxy.h"

class CQuestLog : public QuestLog_Proxy
{
public:
	virtual void OnUpdateTarget1(const UInt32 &target1_id, const UInt16 &target1_v1, const UInt16 &target1_v2, const UInt16 &target1_v3, const UInt8 &target1_vtype, const UInt16 &target1_num, const UInt16 &target1_max);
	virtual void OnUpdateTarget2(const UInt32 &target2_id, const UInt16 &target2_v1, const UInt16 &target2_v2, const UInt16 &target2_v3, const UInt8 &target2_vtype, const UInt16 &target2_num, const UInt16 &target2_max);
	virtual void OnUpdateTarget3(const UInt32 &target3_id, const UInt16 &target3_v1, const UInt16 &target3_v2, const UInt16 &target3_v3, const UInt8 &target3_vtype, const UInt16 &target3_num, const UInt16 &target3_max);
	virtual void OnUpdateTarget4(const UInt32 &target4_id, const UInt16 &target4_v1, const UInt16 &target4_v2, const UInt16 &target4_v3, const UInt8 &target4_vtype, const UInt16 &target4_num, const UInt16 &target4_max);
	virtual void OnUpdateTarget5(const UInt32 &target5_id, const UInt16 &target5_v1, const UInt16 &target5_v2, const UInt16 &target5_v3, const UInt8 &target5_vtype, const UInt16 &target5_num, const UInt16 &target5_max);
	virtual void OnUpdateUpdateIcon(const Boolean &updateIcon);
	virtual void OnUpdateTimer(const UInt32 &TimeLimit, const UInt16 &Interval);

	DEFINE_CLASS(QuestLog_CLASSID);
	CQuestLog();
	virtual ~CQuestLog();
	Boolean IsFailed();
	Boolean IsOverTime();
	Boolean IsCancel();
	Boolean IsFailOverTime();
	Boolean IsFailEscape();
	Boolean IsFailPlayerDie();
	Boolean IsFailTeamDie();
	Boolean IsFailFlowerDie();
	Boolean IsFailNpcDie();
	Boolean IsFailJoinParty();
	Boolean IsCancelOverTime();
	Boolean IsCompleted(UInt32 npc_id=0, UInt16 map_id=0, UInt16 x=0, UInt16 y=0);
	Boolean IsFinishNpc(UInt32 npc_id, UInt16 map_id, UInt16 x, UInt16 y);
	UInt32 GetRealTime();
	UInt32 GetTarget_id(UInt8 i) const;
	UInt16 GetTarget_v1(UInt8 i) const;
	UInt16 GetTarget_v2(UInt8 i) const;
	UInt16 GetTarget_v3(UInt8 i) const;
	UInt16 GetTarget_vtype(UInt8 i) const;
	UInt16 GetTarget_num(UInt8 i) const;
	UInt16 GetTarget_max(UInt8 i) const;
	const String& GetTarget_name(UInt8 i) const;
	Boolean IsRelated(UInt16 qtype, UInt32 id);

private:
	typedef QuestLog_Proxy Parent;
	UInt32 m_LastSyncOfTimeLimit;
};

INLINE UInt32 CQuestLog::GetTarget_id(UInt8 i) const
{
	switch (i)
	{
		case 1:	return GetTarget1_id();
		case 2:	return GetTarget2_id();
		case 3:	return GetTarget3_id();
		case 4:	return GetTarget4_id();
		case 5:	return GetTarget5_id();
	}
	return 0;
}

INLINE UInt16 CQuestLog::GetTarget_v1(UInt8 i) const
{
	switch (i)
	{
		case 1:	return GetTarget1_v1();
		case 2:	return GetTarget2_v1();
		case 3:	return GetTarget3_v1();
		case 4:	return GetTarget4_v1();
		case 5:	return GetTarget5_v1();
	}
	return 0;
}

INLINE UInt16 CQuestLog::GetTarget_v2(UInt8 i) const
{
	switch (i)
	{
		case 1:	return GetTarget1_v2();
		case 2:	return GetTarget2_v2();
		case 3:	return GetTarget3_v2();
		case 4:	return GetTarget4_v2();
		case 5:	return GetTarget5_v2();
	}
	return 0;
}

INLINE UInt16 CQuestLog::GetTarget_v3(UInt8 i) const
{
	switch (i)
	{
		case 1:	return GetTarget1_v3();
		case 2:	return GetTarget2_v3();
		case 3:	return GetTarget3_v3();
		case 4:	return GetTarget4_v3();
		case 5:	return GetTarget5_v3();
	}
	return 0;
}

INLINE UInt16 CQuestLog::GetTarget_vtype(UInt8 i) const
{
	switch (i)
	{
		case 1:	return GetTarget1_vtype();
		case 2:	return GetTarget2_vtype();
		case 3:	return GetTarget3_vtype();
		case 4:	return GetTarget4_vtype();
		case 5:	return GetTarget5_vtype();
	}
	return 0;
}

INLINE UInt16 CQuestLog::GetTarget_num(UInt8 i) const
{
	switch (i)
	{
		case 1:	return GetTarget1_num();
		case 2:	return GetTarget2_num();
		case 3:	return GetTarget3_num();
		case 4:	return GetTarget4_num();
		case 5:	return GetTarget5_num();
	}
	return 0;
}

INLINE UInt16 CQuestLog::GetTarget_max(UInt8 i) const
{
	switch (i)
	{
		case 1:	return GetTarget1_max();
		case 2:	return GetTarget2_max();
		case 3:	return GetTarget3_max();
		case 4:	return GetTarget4_max();
		case 5:	return GetTarget5_max();
	}
	return 0;
}

INLINE const String& CQuestLog::GetTarget_name(UInt8 i) const
{
	static String dummy;
	switch (i)
	{
		case 1:	return GetTarget1_name();
		case 2:	return GetTarget2_name();
		case 3:	return GetTarget3_name();
		case 4:	return GetTarget4_name();
		case 5:	return GetTarget5_name();
	}
	return dummy;
}

#endif //_QUESTLOG_H_
