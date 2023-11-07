#pragma once
#ifndef _RES_ACCMULATION_SHOP_HEAD_
#define _RES_ACCMULATION_SHOP_HEAD_

#include <Reuben\System\Reuben.h>

enum ErrorCode__ACCMULATION_SHOP
{
	ACCMULATION_SHOP_OK = 0,
	ACCMULATION_SHOP_PARAM,
	ACCMULATION_SHOP_FILE_NOT_EXIST,
	ACCMULATION_SHOP_FILE_CONTEXT_ERROR,
};

struct ACCSHOPITEM
{
	UInt32 uiItemId;
	UInt32 uiAccmulationCost;
	UInt32 uiIndex;

	ACCSHOPITEM()
		: uiItemId(0)
		, uiAccmulationCost(0)
		, uiIndex(0)
	{}

	~ACCSHOPITEM()	{}
};

#endif