#pragma once
#ifndef _RESOURCE_TITLE_H_
#define _RESOURCE_TITLE_H_

#include <Reuben\System\Reuben.h>

struct TitleData
{
	UInt16	Title_id;
	UInt8	Title_Type;
	TCHAR	Title_Name[50];
	UInt8	Title_Color;
	UInt8	Title_PreNameType;
	UInt8	Title_RankType;
	UInt8	Title_RankSubType;
	UInt8	Title_StartRank;
	UInt8	Title_EndRank;
	UInt8	Title_TimeLimitType;
	UInt16	Title_TimeLimit;
	
	UInt16	Title_ExpireYear;
	UInt8	Title_ExpireMonth;
	UInt8	Title_ExpireDay;
	UInt8	Title_ExpireHour;
	UInt8	Title_ExpireMin;
	UInt8	Title_ExpireSec;
	
	
	TitleData()
		: Title_id(C_UINT16(0))
		, Title_Type(C_UINT8(0))
		, Title_Color(C_UINT8(0))
		, Title_PreNameType(C_UINT8(0))
		, Title_RankType(C_UINT8(0))
		, Title_RankSubType(C_UINT8(0))
		, Title_StartRank(C_UINT8(0))
		, Title_EndRank(C_UINT8(0))
		, Title_TimeLimitType(C_UINT8(0))
		, Title_TimeLimit(C_UINT16(0))
		, Title_ExpireYear(C_UINT16(0))
		, Title_ExpireMonth(C_UINT8(0))
		, Title_ExpireDay(C_UINT8(0))
		, Title_ExpireHour(C_UINT8(0))
		, Title_ExpireMin(C_UINT8(0))
		, Title_ExpireSec(C_UINT8(0))
	{
		ZeroMemory(Title_Name, 50*sizeof(TCHAR));
	}

	~TitleData()	{}
};

#endif // _RESOURCE_PET_H_