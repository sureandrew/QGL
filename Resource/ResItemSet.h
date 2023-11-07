#pragma once
#ifndef _RESOURCE_ITEMSET_H_
#define _RESOURCE_ITEMSET_H_

#include <Reuben\System\Reuben.h>

struct ItemSetData
{
	UInt32	itemset_id;
	TCHAR	name[20];
	UInt8	count;
	Int16	STR;
	Int16	CON;
	Int16	STA;
	Int16	AGI;
	Int16	SPR;
	Int16	ATK;
	Int16	DEF;
	Int16	HIT;
	Int16	EVA;
	Int16	POW;
	Int16	SPD;
	UInt8	bat_attrib;
	UInt8	ratio;
	UInt16	MaxHP;
	UInt16	MaxSP;
	UInt16	MaxVP;
	StlVector<UInt32, ManagedAllocator<UInt32> >	skill_ids;
	StlVector<UInt32, ManagedAllocator<UInt32> >	item_ids;

	ItemSetData()
		: itemset_id(C_UINT32(0))
		, count(C_UINT8(0))
		, STR(C_INT16(0))
		, CON(C_INT16(0))
		, STA(C_INT16(0))
		, AGI(C_INT16(0))
		, SPR(C_INT16(0))
		, ATK(C_INT16(0))
		, DEF(C_INT16(0))
		, HIT(C_INT16(0))
		, EVA(C_INT16(0))
		, POW(C_INT16(0))
		, SPD(C_INT16(0))
		, bat_attrib(C_UINT8(0))
		, ratio(C_UINT8(0))
		, MaxHP(C_UINT16(0))
		, MaxSP(C_UINT16(0))
		, MaxVP(C_UINT16(0))
	{
		ZeroMemory(name, 20*sizeof(TCHAR));
	}

	~ItemSetData()	{}
};

#endif // _RESOURCE_ITEMSET_H_
