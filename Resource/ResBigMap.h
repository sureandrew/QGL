#pragma once
#ifndef _RESOURCE_BIGMAP_H_
#define _RESOURCE_BIGMAP_H_

#include <Reuben\System\Reuben.h>

struct BigMapData
{
	UInt16		map_id;
	TCHAR		map_name[20];
	UInt8		map_type;
	TCHAR		map_file[20];
	TCHAR		map_image_file[20];
	TCHAR		description[200];
	UInt16		button_x;
	UInt16		button_y;
	UInt16		adj_map1;
	UInt16		adj_map2;
	UInt16		adj_map3;
	UInt16		adj_map4;
	UInt16		adj_map5;
	UInt16		adj_map6;
	UInt16		adj_map7;
	UInt16		adj_map8;
	UInt16		adj_map9;
	UInt16		adj_map10;
	UInt16		adj_map11;
	UInt16		adj_map12;
	UInt16		flag_x;
	UInt16		flag_y;
	UInt8		use;

	BigMapData()
		: map_id(C_UINT16(0))
		, map_type(C_UINT8(0))
		, button_x(C_UINT16(0))
		, button_y(C_UINT16(0))
		, adj_map1(C_UINT16(0))
		, adj_map2(C_UINT16(0))
		, adj_map3(C_UINT16(0))
		, adj_map4(C_UINT16(0))
		, adj_map5(C_UINT16(0))
		, adj_map6(C_UINT16(0))
		, adj_map7(C_UINT16(0))
		, adj_map8(C_UINT16(0))
		, adj_map9(C_UINT16(0))
		, adj_map10(C_UINT16(0))
		, adj_map11(C_UINT16(0))
		, adj_map12(C_UINT16(0))
		, flag_x(C_UINT16(0))
		, flag_y(C_UINT16(0))
		, use(C_UINT8(0))
	{
		ZeroMemory(map_name, 20*sizeof(TCHAR));
		ZeroMemory(map_file, 20*sizeof(TCHAR));
		ZeroMemory(map_image_file, 20*sizeof(TCHAR));
		ZeroMemory(description, 200*sizeof(TCHAR));
	}

	~BigMapData()	{}
};

struct BigMapBelongData
{
	UInt16		srcMap_id;
	TCHAR		map_name[20];
	UInt16		belongMap_id;

	BigMapBelongData()
	{
		ZeroMemory(this, sizeof(BigMapBelongData));
	}

	~BigMapBelongData(){}
};

#endif