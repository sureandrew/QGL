#pragma once
#ifndef _RESOURCE_NEWBIE_H_
#define _RESOURCE_NEWBIE_H_

#include <Reuben\System\Reuben.h>


struct NewbieData
{
	UInt16	id;
	TCHAR	class1[33];
	TCHAR	class2[33];
	TCHAR	class3[33];
	TCHAR	description[2049];

	NewbieData()
		: id(C_UINT16(0))
	{
		ZeroMemory(class1, 33*sizeof(TCHAR));
		ZeroMemory(class2, 33*sizeof(TCHAR));
		ZeroMemory(class3, 33*sizeof(TCHAR));
		ZeroMemory(description, 2049*sizeof(TCHAR));
	}
	~NewbieData()	{}
};

#endif // _RESOURCE_SKILL_H_
