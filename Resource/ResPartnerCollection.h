#pragma once
#ifndef _RESOURCE_PARTNERCOLLECTION_H_
#define _RESOURCE_PARTNERCOLLECTION_H_

#include <Reuben\System\Reuben.h>

struct PartnerCollectionData
{
	UInt32	ID;
	UInt8	Type;
	UInt8	Group;
	UInt32	MOBid;
	TCHAR	Name[20];
	StlVector<UInt16, ManagedAllocator<UInt16> >	Buff;
	Int16	STR;
	Int16	CON;
	Int16	STA;
	Int16	AGI;
	Int16	SPR;
	UInt8	Light;

	PartnerCollectionData()	{
	}

	~PartnerCollectionData()	{}
};

#endif // _RESOURCE_PARTNERCOLLECTION_H_
