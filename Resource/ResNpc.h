#pragma once
#ifndef _RESOURCE_NPC_H_
#define _RESOURCE_NPC_H_

#include <Reuben\System\Reuben.h>

struct NpcData
{
	typedef StlVector<NPCMobInfo, ManagedAllocator<NPCMobInfo> > MobList;
	typedef StlVector<UInt32, ManagedAllocator<UInt32> > QuestList;
	typedef StlVector<UInt32, ManagedAllocator<UInt32> > ShopList;

	UInt32		npc_id;
	UInt8		npc_type;
	TCHAR		pre_name[20];
	TCHAR		mid_name[20];
	UInt8		pos_name;
	UInt8		bFaction;
	UInt8		faction;
	UInt8		job;
	UInt8		sex;
	UInt8		direction;
	DWORD		appearTime;
	UInt8		squad;
	UInt8		minLackey;
	UInt8		maxLackey;
	TCHAR		script[32];
	ShopList	shop_id;
	QuestList	quest_id;
	UInt32		exp;
	UInt32		money;
	TCHAR		model[32];
	TCHAR		image[32];
	MobList		mobLackey;
	MobList		mobLeader;
	TCHAR		dropitemcsv[32];
	TCHAR		battleScript[32];
		
	NpcData()
		: npc_id(C_UINT32(0))
		, npc_type(C_UINT8(0))
		, pos_name(C_UINT8(0))
		, bFaction(C_UINT8(0))
		, faction(C_UINT8(0))
		, job(C_UINT8(0))
		, sex(C_UINT8(0))
		, direction(C_UINT8(0))
		, appearTime(0)
		, squad(C_UINT8(0))
		, minLackey(C_UINT8(0))
		, maxLackey(C_UINT8(0))
		, exp(C_UINT32(0))
		, money(C_UINT32(0))
	{
		ZeroMemory(pre_name, sizeof(TCHAR) * 20);
		ZeroMemory(mid_name, sizeof(TCHAR) * 20);
		ZeroMemory(script, sizeof(TCHAR) * 32);
		ZeroMemory(model, sizeof(TCHAR) * 32);
		ZeroMemory(image, sizeof(TCHAR) * 32);
		ZeroMemory(dropitemcsv, sizeof(TCHAR) * 32);
		ZeroMemory(battleScript, sizeof(TCHAR) * 32);
	}
};

#endif // _RESOURCE_NPC_H_
