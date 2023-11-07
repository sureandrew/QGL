#pragma once

#ifndef _RES_LINE_UP_H_
#define _RES_LINE_UP_H_

#include <Reuben\System\Reuben.h>
const int MAXLINEUP = 14;
const int MAXTEAMMEM = 5;
const int MAXLINEUPDES = 128;
const int MAXLINEUPNAEMLEN = 32;

typedef struct _LINEUP_ALLELOPATHY_INFO_ //阵法相克关系描述
{
	struct 
	{
		UInt16 uiLineupId;
		int   iAllelopathyValue;
	}Info[MAXLINEUP];
}LINEUPALPTINFO,*PLINEUPALPTINFO;

struct POSITIONBUFFER
{
	UInt16 uiBufferId;
	int	   iPos;
	int    iPosPartner;
};
struct LINEUPINFO		//阵法描述结构
{
	UInt32 uiId;					//id
	LINEUPALPTINFO lui;				//与其他阵法相克关系
	POSITIONBUFFER pb[MAXTEAMMEM];		//位置与buffer关联
	TCHAR szDesc[MAXLINEUPDES];			//阵法描述	
	TCHAR szName[MAXLINEUPNAEMLEN];		//阵法名字

	LINEUPINFO()
	{
		ZeroMemory(this, sizeof(LINEUPINFO));
	}

	~LINEUPINFO()	{}
};


#endif