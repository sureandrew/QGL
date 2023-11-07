#pragma once
#ifndef _RESOURCE_MANOR_H_
#define _RESOURCE_MANOR_H_

#include <Reuben\System\Reuben.h>

struct ManorData
{
	UInt8		manor_scale;
	UInt32		manor_maxProsper;
	UInt32		manor_maxFund;
	UInt8		manor_maxRoomNum;
	Real32		manor_meditationLv;
	UInt8		manor_tigerRuneLv;
	UInt32		manor_maintainCost;
	UInt32		manor_prosperIncome;
	UInt8		manor_Icon;
	UInt32Vector     manor_map_id;
	UInt16      manor_maxMaintain;
	UInt8		manor_maxGuard;
	UInt8		manor_maxCharEx;
	UInt8		manor_maxCharIn;
	UInt8		manor_maxPartnerEx;
	UInt8		manor_maxPartnerIn;
	UInt8		manor_maxDrug;
	UInt8		manor_maxCook;
	UInt8		manor_maxWine;
	UInt8		manor_maxHp;
	UInt8		manor_maxSp;

	ManorData()
		: manor_scale(C_UINT8(0))
		, manor_maxProsper(C_UINT32(0))
		, manor_maxFund(C_INT32(0))
		, manor_maxRoomNum(C_INT8(0))
		, manor_meditationLv(0.0f)
		, manor_tigerRuneLv(C_INT8(0))
		, manor_maintainCost(C_INT32(0))
		, manor_prosperIncome(C_INT32(0))
		, manor_Icon(C_INT8(0))
		, manor_maxMaintain(C_INT16(0))
		, manor_maxGuard(C_INT8(0))
		, manor_maxCharEx(C_INT8(0))
		, manor_maxCharIn(C_UINT8(0))
		, manor_maxPartnerEx(C_UINT8(0))
		, manor_maxPartnerIn(C_UINT8(0))
		, manor_maxDrug(C_UINT8(0))
		, manor_maxCook(C_UINT8(0))
		, manor_maxWine(C_UINT8(0))
		, manor_maxHp(C_UINT8(0))
		, manor_maxSp(C_UINT8(0))
	{}

	~ManorData()	{}
};

#endif // _RESOURCE_MANOR_H_
