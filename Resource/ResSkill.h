#pragma once
#ifndef _RESOURCE_SKILL_H_
#define _RESOURCE_SKILL_H_

#include <Reuben\System\Reuben.h>

enum SKILL_MOD_TYPE
{
	 SKILL_MOD_TYPE_INVALID = 0,
	 SKILL_MOD_TYPE_NUM_PER_LEVEL,
	 SKILL_MOD_TYPE_PERCENT
};

struct SkillModData
{
	UInt8	type;
	Real32	value;

	SkillModData()
		: value(0.0f)
	{
		type = SKILL_MOD_TYPE_NUM_PER_LEVEL;
	}
};

struct SkillData
{
	UInt16	id;
	UInt8	type;
	UInt8	faction;
	UInt8	class_skill;
	UInt16	reqBaseSkill;
	UInt16	reqBaseSkillRank;
	UInt16	reqLv;
	UInt32	Req_Exp;
	UInt16	maxRank;
	UInt8	active;
	UInt8	battleorField;
	UInt8	form;
	UInt8	targetType;
	UInt8	skill_UseElement;
	UInt8	skill_Script_Type;
	UInt8	skill_cooldownType;
	UInt8	Skill_ThrowItemType;
	UInt8	AtkType;
	UInt8	mod_atk;
	UInt8	mod_Def;
	UInt8	mod_Spd;
	UInt8	mod_Pow;
	UInt8	mod_Hit;
	UInt8	mod_Eva;
	UInt8	mod_maxHP;
	UInt8	mod_maxSP;
	UInt8	mod_maxDP;
	UInt8	mod_maxWc;
	UInt16	preSkill;
	UInt8	animation_Motion;
	UInt8	animation_Type;

	TCHAR	name[12];
	TCHAR	icon[100];
	TCHAR	effect_Atk[100];
	TCHAR	effect_Hurt[100];
	TCHAR	effect_Spark[100];
	TCHAR	sound_Atk[100];
	TCHAR	sound_Hurt[100];
	TCHAR	sound_Spark[100];
	TCHAR	skill_description[100];
	TCHAR	skill_functionDes[100];
	StlVector<UInt16, ManagedAllocator<UInt16> >	AutoAdd_Buffs;
	UInt8	Skill_AutoSwitch;

	SkillData()	{
		
	}

	UInt8 GetAnimationType(){return animation_Type;}

	~SkillData()	{}
};

#endif // _RESOURCE_SKILL_H_
