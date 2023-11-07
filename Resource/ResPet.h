#pragma once
#ifndef _RESOURCE_PET_H_
#define _RESOURCE_PET_H_

#include <Reuben\System\Reuben.h>

struct PetData
{
	UInt32		pet_id;
	TCHAR		pet_name[20];
	UInt8		pet_type;
	UInt8		pet_breed_type;
	UInt16		pet_lv;
	UInt16		pet_ReqLV;
	UInt8		pet_rare;
	UInt8		pet_race;
	UInt8		pet_sex;
	UInt8		pet_canbirth;
	UInt32		pet_useVP;
	UInt8		pet_element;
	UInt32		pet_MaxVP;
	UInt32		pet_StartHappiness;
	Int			pet_Speed;
	UInt16		pet_Height;
	UInt8		pet_TimeLimitType;
	UInt8		pet_TimeLimit;
	UInt8		pet_soulbound;
	UInt8		pet_unique;
	UInt16		pet_SkillList;
	TCHAR		pet_description[256];
	TCHAR		pet_model[20];
	UInt8		pet_color;
	TCHAR		pet_faceL[20];
	TCHAR		pet_faceS[20];
	TCHAR		pet_sound_idle1[20];
	TCHAR		pet_sound_idle2[20];
	TCHAR		pet_sound_happy[20];
	TCHAR		pet_sound_unhappy[20];
	TCHAR		pet_sound_atk1[20];
	TCHAR		pet_sound_atk2[20];
	TCHAR		pet_sound_skill1[20];
	TCHAR		pet_sound_skill2[20];
	TCHAR		pet_sound_hurt[20];
	TCHAR		pet_sound_die[20];
	UInt16		pet_reduce_vp_min;
	UInt16		pet_reduce_vp_max;

	PetData()
		: pet_id(C_UINT32(0))
		, pet_type(C_UINT8(0))
		, pet_breed_type(C_UINT8(0))
		, pet_lv(C_UINT16(0))
		, pet_ReqLV(C_UINT16(0))
		, pet_rare(C_UINT8(0))
		, pet_race(C_UINT8(0))
		, pet_sex(C_UINT8(0))
		, pet_canbirth(C_UINT8(0))
		, pet_useVP(C_UINT32(0))
		, pet_element(C_UINT8(0))
		, pet_MaxVP(C_UINT32(0))
		, pet_StartHappiness(C_UINT32(0))
		, pet_Speed(C_INT(0))
		, pet_Height(C_UINT16(0))
		, pet_TimeLimitType(C_UINT8(0))
		, pet_TimeLimit(C_UINT8(0))
		, pet_soulbound(C_UINT8(0))
		, pet_unique(C_UINT8(0))
		, pet_SkillList(C_UINT16(0))
		, pet_color(C_UINT8(0))
		, pet_reduce_vp_min(C_UINT16(0))
		, pet_reduce_vp_max(C_UINT16(0))
	{
		ZeroMemory(pet_name, 20*sizeof(TCHAR));
		ZeroMemory(pet_description, 256*sizeof(TCHAR));
		ZeroMemory(pet_model, 20*sizeof(TCHAR));
		ZeroMemory(pet_faceL, 20*sizeof(TCHAR));
		ZeroMemory(pet_faceS, 20*sizeof(TCHAR));
		ZeroMemory(pet_sound_idle1, 20*sizeof(TCHAR));
		ZeroMemory(pet_sound_idle2, 20*sizeof(TCHAR));
		ZeroMemory(pet_sound_happy, 20*sizeof(TCHAR));
		ZeroMemory(pet_sound_unhappy, 20*sizeof(TCHAR));
		ZeroMemory(pet_sound_atk1, 20*sizeof(TCHAR));
		ZeroMemory(pet_sound_atk2, 20*sizeof(TCHAR));
		ZeroMemory(pet_sound_skill1, 20*sizeof(TCHAR));
		ZeroMemory(pet_sound_skill2, 20*sizeof(TCHAR));
		ZeroMemory(pet_sound_hurt, 20*sizeof(TCHAR));
		ZeroMemory(pet_sound_die, 20*sizeof(TCHAR));
	}

	~PetData()	{}
};

#endif // _RESOURCE_PET_H_