#pragma once
#ifndef _RESOURCE_NEWBIETIPS_H_
#define _RESOURCE_NEWBIETIPS_H_

#include <Reuben\System\Reuben.h>


struct NewbieTipsData
{
	UInt16	id;
	UInt16	event;
	UInt16	level_up;
	UInt16	map_id;
	UInt8	Line;
	UInt8	flash;
	StlVector<String, ManagedAllocator<String> > boyimages;
	StlVector<String, ManagedAllocator<String> > girlimages;
	StlVector<String, ManagedAllocator<String> > boystring;
	StlVector<String, ManagedAllocator<String> > girlstring;
	NewbieTipsData()	{
		
	}
	~NewbieTipsData()	{
		for (UInt i = 0; i < boyimages.size(); i ++)
		{
			boyimages[i].Empty();
		}

		for (UInt i = 0; i < girlimages.size(); i ++)
		{
			girlimages[i].Empty();
		}

		for (UInt i = 0; i < boystring.size(); i ++)
		{
			boystring[i].Empty();
		}

		for (UInt i = 0; i < girlstring.size(); i ++)
		{
			girlstring[i].Empty();
		}
	}
};

#endif // _RESOURCE_SKILL_H_
