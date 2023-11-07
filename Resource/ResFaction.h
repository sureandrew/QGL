#pragma once
#ifndef _RESOURCE_FACTION_H_
#define _RESOURCE_FACTION_H_

#include <Reuben\System\Reuben.h>

struct FactionData
{
	UInt16	id;
	TCHAR	Faction_Name[20];
	TCHAR	Faction_Logo[20];
	TCHAR	Faction_class1[10];
	TCHAR	Faction_class2[10];
	TCHAR	Faction_class3[10];
	TCHAR	Faction_class4[10];
	TCHAR	Faction_class5[10];
	TCHAR	Faction_class6[10];
	TCHAR	Faction_Talent[130];
	TCHAR	Faction_Description[200];
	
	FactionData()
		: id(C_UINT16(0))
	{
		ZeroMemory(Faction_Name, 20*sizeof(TCHAR));
		ZeroMemory(Faction_Logo, 20*sizeof(TCHAR));
		ZeroMemory(Faction_class1, 10*sizeof(TCHAR));
		ZeroMemory(Faction_class2, 10*sizeof(TCHAR));
		ZeroMemory(Faction_class3, 10*sizeof(TCHAR));
		ZeroMemory(Faction_class4, 10*sizeof(TCHAR));
		ZeroMemory(Faction_class5, 10*sizeof(TCHAR));
		ZeroMemory(Faction_class6, 10*sizeof(TCHAR));
		ZeroMemory(Faction_Talent, 130*sizeof(TCHAR));
		ZeroMemory(Faction_Description, 200*sizeof(TCHAR));
	}

	~FactionData()	{}
};

#endif // _RESOURCE_PET_H_