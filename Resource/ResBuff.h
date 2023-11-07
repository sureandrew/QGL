#pragma once
#ifndef _RESOURCE_BUFF_H_
#define _RESOURCE_BUFF_H_

#include <Reuben\System\Reuben.h>

struct BuffData
{
	UInt16	id;
	TCHAR	name[13];
	TCHAR	buff_description[101];
	UInt16	Buff_type;
	UInt16	Buff_school;
	UInt16	Buff_endtype;
	UInt16	Buff_RemoveAftOffline;
	UInt16	Buff_RemoveAftBattle;
	UInt8	Buff_ProtectPK;
	UInt8	Buff_BlockPK;
	UInt8Vector	Buff_BlockChat;
	Int		Buff_Speed;
	UInt16Vector	Buff_BlockItemType;
	UInt16	Buff_removeAfterDead;
	UInt16	Buff_attacktimes;
	UInt16	Buff_starttime;
	UInt16	Buff_HideEffect;
	UInt16Vector	Buff_acttimes;
	UInt16	Buff_probability;
	UInt16	Buff_nextBuff;
	UInt16	Buff_period_type;
	UInt16	Buff_Mod_Type_1;
	UInt16	Buff_Mod_Type_2;
	UInt16	Buff_Mod_Type_3;
	UInt16	Buff_DOTtype;
	UInt16	Buff_DOTattribute;
	UInt16Vector	Buff_Blockcommands;
	UInt16Vector	Buff_BlockActions;
	UInt16Vector	Buff_BlockSkills;
	UInt16Vector	Buff_AntiBuffs;
	UInt16Vector	Buff_ClearBuffs;
	UInt16Vector	Buff_DetectBuffs;
	TCHAR	Buff_IconFile[101];
	UInt16	Buff_IconID;
	TCHAR	Buff_EffectFile[101];
	UInt16	Buff_EddectDisplay;
	TCHAR	Buff_SFX[101];
	TCHAR	Buff_SparkFile[101];
	TCHAR	Buff_Value1String[101];
	TCHAR	Buff_Value2String[101];
	TCHAR	Buff_Value3String[101];
	UInt8	Buff_KeepBody;
	UInt16	Buff_Priority;
	UInt32	Buff_TimeOut;

	BuffData()	{
	}
	~BuffData()	{}
};

#endif // _RESOURCE_SKILL_H_
