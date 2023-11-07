#pragma once
#ifndef _RESOURCE_CHARACTER_H_
#define _RESOURCE_CHARACTER_H_

#include <Reuben\System\Reuben.h>

struct StartCharData
{
	UInt16 sexClass;
	UInt16 STA;
	UInt16 SPR;
	UInt16 STR;
	UInt16 CON;
	UInt16 AGI;
	UInt16 MaxHP;
	UInt16 MaxSP;
	UInt16 MaxVP;
	UInt16 MaxDP;
	UInt16 MaxWC;
	UInt16 StartMap;
	UInt16 StartX;
	UInt16 StartY;
	UInt32 Weapon1;
	UInt32 Weapon2;
	UInt32 Cloth;
	UInt32 HeadAs;
	UInt32 Foot;
	UInt32 A1;
	UInt32 A2;
	float MaxHP_STA;
	float MaxSP_SPR;
	float Atk_STR;
	float Def_CON;
	float Hit_AGI;
	float Eva_AGI;
	float Pwd_SPR;
	float Pwd_STA;
	float Pwd_CON;
	float Pwd_STR;
	float Spd_AGI;
	float Spd_STR;
	float Spd_CON;
	float Spd_STA;
	UInt32 Start_npc_templateID;


	StartCharData()
		: sexClass(C_UINT16(0))
		, STA(C_UINT16(0))
		, SPR(C_UINT16(0))
		, STR(C_UINT16(0))
		, CON(C_UINT16(0))
		, AGI(C_UINT16(0))
		, MaxHP(C_UINT16(0))
		, MaxSP(C_UINT16(0))
		, MaxVP(C_UINT16(0))
		, MaxDP(C_UINT16(0))
		, MaxWC(C_UINT16(0))
		, StartMap(C_UINT16(0))
		, StartX(C_UINT16(0))
		, StartY(C_UINT16(0))
		, Weapon1(C_UINT32(0))
		, Weapon2(C_UINT32(0))
		, Cloth(C_UINT32(0))
		, HeadAs(C_UINT32(0))
		, Foot(C_UINT32(0))
		, A1(C_UINT32(0))
		, A2(C_UINT32(0))
		, MaxHP_STA(0.0f)
		, MaxSP_SPR(0.0f)
		, Atk_STR(0.0f)
		, Def_CON(0.0f)
		, Hit_AGI(0.0f)
		, Eva_AGI(0.0f)
		, Pwd_SPR(0.0f)
		, Pwd_STA(0.0f)
		, Pwd_CON(0.0f)
		, Pwd_STR(0.0f)
		, Spd_AGI(0.0f)
		, Spd_STR(0.0f)
		, Spd_CON(0.0f)
		, Spd_STA(0.0f)
		, Start_npc_templateID(C_UINT32(0))
	{}

	float GetMaxHP_STA(){ return MaxHP_STA;}
	float GetMaxSP_SPR(){ return MaxSP_SPR;}
	float GetAtk_STR(){ return Atk_STR;}
	float GetDef_CON(){ return Def_CON;}
	float GetHit_AGI(){ return Hit_AGI;}
	float GetEva_AGI(){ return Eva_AGI;}
	float GetPwd_SPR(){ return Pwd_SPR;}
	float GetPwd_STA(){ return Pwd_STA;}
	float GetPwd_CON(){ return Pwd_CON;}
	float GetPwd_STR(){ return Pwd_STR;}
	float GetSpd_AGI(){ return Spd_AGI;}
	float GetSpd_STR(){ return Spd_STR;}
	float GetSpd_CON(){ return Spd_CON;}
	float GetSpd_STA(){ return Spd_STA;}

};

#endif // _RESOURCE_CHARACTER_H_
