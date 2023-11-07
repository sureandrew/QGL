#pragma once

#ifndef _RES_LINE_UP_H_
#define _RES_LINE_UP_H_

#include <Reuben\System\Reuben.h>
const int MAXLINEUP = 14;
const int MAXTEAMMEM = 5;
const int MAXLINEUPDES = 128;
const int MAXLINEUPNAEMLEN = 32;

typedef struct _LINEUP_ALLELOPATHY_INFO_ //����˹�ϵ����
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
struct LINEUPINFO		//�������ṹ
{
	UInt32 uiId;					//id
	LINEUPALPTINFO lui;				//����������˹�ϵ
	POSITIONBUFFER pb[MAXTEAMMEM];		//λ����buffer����
	TCHAR szDesc[MAXLINEUPDES];			//������	
	TCHAR szName[MAXLINEUPNAEMLEN];		//������

	LINEUPINFO()
	{
		ZeroMemory(this, sizeof(LINEUPINFO));
	}

	~LINEUPINFO()	{}
};


#endif