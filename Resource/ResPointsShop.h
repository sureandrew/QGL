#pragma once
#ifndef _RES_POINTSSHOP_HEAD_
#define _RES_POINTSSHOP_HEAD_

#include <Reuben\System\Reuben.h>

enum ErrorCode__POINTS_SHOP
{
	POINTS_SHOP_OK = 0,
	POINTS_SHOP_PARAM,
	POINTS_SHOP_FILE_NOT_EXIST,
	POINTS_SHOP_FILE_CONTEXT_ERROR,
};

struct POINTSSHOPITEM
{
	UInt32 uiItemId;
	UInt32 uiPointsCost;
	UInt32 uiIndex;

	POINTSSHOPITEM()
		: uiItemId(0)
		, uiPointsCost(0)
		, uiIndex(0)
	{}

	~POINTSSHOPITEM()	{}
};

#endif