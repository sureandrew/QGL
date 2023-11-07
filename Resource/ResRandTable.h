#pragma once
#ifndef _RESOURCE_RANDOM_TABLE_H_
#define _RESOURCE_RANDOM_TABLE_H_

#include <Reuben\System\Reuben.h>

struct CSVItemRow
{
	UInt32			uRange;
	Int				nCount;

	UInt32Vector	ItemLst;

	CSVItemRow()
		: uRange(C_UINT32(0))
		, nCount(C_INT(0))
	{}

	~CSVItemRow()
	{
		ItemLst.clear();
	};
};

struct CSVItemData
{
	typedef StlVector<CSVItemRow*, ManagedAllocator<CSVItemRow*> > CSVItemRowLst;
	
	CSVItemRowLst	RowLst;

	CSVItemData()
	{
		RowLst.clear();
	};

	~CSVItemData()
	{
		for ( CSVItemRowLst::iterator itr = RowLst.begin(); itr != RowLst.end(); ++itr )
			SafeDeleteObject((*itr));
		RowLst.clear();
	};
};





struct CSVMobRow
{
	UInt32			uRange;
	Int				nCount;

	UInt32Vector	MobLst;
	
	CSVMobRow()
	{
		uRange = 0;
		nCount = 0;
		MobLst.clear();
	};

	~CSVMobRow()
	{
		MobLst.clear();
	};

};

struct CSVMobData
{
	typedef StlVector<CSVMobRow*, ManagedAllocator<CSVMobRow*> > CSVMobRowLst;

	CSVMobRowLst	RowLst;

	CSVMobData()
	{
		RowLst.clear();
	};

	~CSVMobData()
	{
		for ( CSVMobRowLst::iterator itr = RowLst.begin(); itr != RowLst.end(); ++itr )
			SafeDeleteObject((*itr));
		RowLst.clear();
	};
};

struct CSVNpcRow
{
	UInt32			uRange;
	Int				nCount;

	UInt32Vector	NpcLst;
	
	CSVNpcRow()
	{
		uRange = 0;
		nCount = 0;
		NpcLst.clear();
	};

	~CSVNpcRow()
	{
		NpcLst.clear();
	};
};

struct CSVNpcData
{
	typedef StlVector<CSVNpcRow*, ManagedAllocator<CSVNpcRow*> > CSVNpcRowLst;

	CSVNpcRowLst	RowLst;

	CSVNpcData()
	{
		RowLst.clear();
	};

	~CSVNpcData()
	{
		for ( CSVNpcRowLst::iterator itr = RowLst.begin(); itr != RowLst.end(); ++itr )
			SafeDeleteObject((*itr));
		RowLst.clear();
	};
};


struct CSVQuestRow
{
	UInt32			Uid;
	UInt32			TypeId;
	TCHAR			TypeName[65];
	TCHAR			Name[65];
	UInt32			Cancel;
	TCHAR			Descript[1025];
	
	UInt32			BeginNpcId;
	UInt32			BeginMapId;
	UInt32			BeginX;
	UInt32			BeginY;

	UInt32			FinishNpcId;
	UInt32			FinishMapId;
	UInt32			FinishX;
	UInt32			FinishY;

	UInt32			Npc01Id;
	UInt32			Npc01Map;
	UInt32			Npc01X;
	UInt32			Npc01Y;

	UInt32			Npc02Id;
	UInt32			Npc02Map;
	UInt32			Npc02X;
	UInt32			Npc02Y;

	UInt32			Item01Id;
	UInt32			Item01Max;
	UInt32			Item01T1;
	UInt32			Item01V1;
	UInt32			Item01T2;
	UInt32			Item01V2;

	UInt32			Mob01Id;
	UInt32			Mob01Max;

	UInt32			Mob02Id;
	UInt32			Mob02Max;

	UInt32			Mob03Id;
	UInt32			Mob03Max;

	UInt32			Partner01Id;
	UInt32			Partner01Max;
	UInt32			Partner01T1;
	UInt32			Partner01V1;
	UInt32			Partner01T2;

	UInt32			Pet01Id;
	UInt32			Pet01Max;

	UInt32			Map01Id;
	UInt32			Map01X;
	UInt32			Map01Y;

	UInt32			Map02Id;
	UInt32			Map02X;
	UInt32			Map02Y;

	UInt32			NewBieMinLv;
	UInt32			NewBieMaxLv;
	UInt32			NewBieMax;

	UInt32			EncountId;
	UInt32			EncountMap;
	UInt32			EncountTime;
	UInt32			EncountRun;
	UInt32			EncountMax;

	UInt32			TimeLimit;
	UInt32			Interval;
	UInt32			Buff;
	
	UInt32			FailOverTime;
	UInt32			FailEscape;
	UInt32			FailPlayerDie;
	UInt32			FailTeamDie;
	UInt32			FailFlowerDie;
	UInt32			FailNpcDie;

	CSVQuestRow()
	{
		ZeroMemory(this, sizeof(CSVQuestRow));
	};
};
/*
struct CSVQuestData
{
	typedef StlMap<UInt32, CSVQuestRow*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CSVQuestRow*> > > CSVQuestLst;

	CSVQuestLst	RowLst;

	CSVQuestData()
	{
		RowLst.clear();
	};

	~CSVQuestData()
	{
		for ( CSVQuestLst::iterator itr = RowLst.begin(); itr != RowLst.end(); ++itr )
			SafeDeleteObject(itr->second);
		RowLst.clear();
	};
};
*/



#endif // _RESOURCE_RANDOM_TABLE_H_

