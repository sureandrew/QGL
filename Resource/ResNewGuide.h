#pragma once
#ifndef _RESOURCE_NEWGUIDE_H_
#define _RESOURCE_NEWGUIDE_H_

#include <Reuben\System\Reuben.h>


struct NewGuideData
{
	UInt8	guide_id;	
	UInt8	ArrLeft;
	UInt16	ArrLeft_X;
	UInt16	ArrLeft_Y;
	UInt8	ArrRight;
	UInt16	ArrRight_X;
	UInt16	ArrRight_Y;
	UInt8	ArrDown;
	UInt16	ArrDown_X;
	UInt16	ArrDown_Y;
	UInt8	ArrLeft02;
	UInt16	ArrLeft02_X;
	UInt16	ArrLeft02_Y;
	TCHAR img1_name[33];
	UInt16 img1_X;
	UInt16 img1_Y;
	TCHAR img2_name[33];
	UInt16 img2_X;
	UInt16 img2_Y;
	TCHAR img3_name[33];
	UInt16 img3_X;
	UInt16 img3_Y;
	TCHAR img4_name[33];
	UInt16 img4_X;
	UInt16 img4_Y;
	TCHAR img5_name[33];
	UInt16 img5_X;
	UInt16 img5_Y;	
	TCHAR context[513];
	
	NewGuideData()
		:guide_id(C_UINT8(0))
		,ArrLeft(C_UINT8(0))
		,ArrLeft_X(C_UINT16(0))
		,ArrLeft_Y(C_UINT16(0))
		,ArrRight(C_UINT8(0))
		,ArrRight_X(C_UINT16(0))
		,ArrRight_Y(C_UINT16(0))
		,ArrDown(C_UINT8(0))
		,ArrDown_X(C_UINT16(0))
		,ArrDown_Y(C_UINT16(0))
		,ArrLeft02(C_UINT8(0))
		,ArrLeft02_X(C_UINT16(0))
		,ArrLeft02_Y(C_UINT16(0))
		,img1_X(C_UINT16(0))
		,img1_Y(C_UINT16(0))
		,img2_X(C_UINT16(0))
		,img2_Y(C_UINT16(0))
		,img3_X(C_UINT16(0))
		,img3_Y(C_UINT16(0))
		,img4_X(C_UINT16(0))
		,img4_Y(C_UINT16(0))
		,img5_X(C_UINT16(0))
		,img5_Y(C_UINT16(0))
	{
		ZeroMemory(img1_name, 33*sizeof(TCHAR));
		ZeroMemory(img2_name, 33*sizeof(TCHAR));
		ZeroMemory(img3_name, 33*sizeof(TCHAR));
		ZeroMemory(img4_name, 33*sizeof(TCHAR));
		ZeroMemory(img5_name, 33*sizeof(TCHAR));
		ZeroMemory(context, 513*sizeof(TCHAR));
	}
	~NewGuideData()	{}
};

#endif // _RESOURCE_NEWGUIDE_H_
