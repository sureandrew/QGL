#pragma once
#ifndef _RESOURCE_MIXSHOP_H_
#define _RESOURCE_MIXSHOP_H_

#include <Reuben\System\Reuben.h>

struct MixShopData
{
	UInt32 shopID;
	UInt8 type;
	TCHAR shopName[21];
	UInt16 skilllvl_low;
	UInt16 skilllvl_up;
	TCHAR mixItemTable[31];
	UInt8 slotSwitch01;
	UInt8 slotItemType01;
	UInt32 slotItemID01;
	TCHAR slotItemName01[21];

	UInt8 slotSwitch02;
	UInt8 slotItemType02;
	UInt32 slotItemID02;
	TCHAR slotItemName02[21];

	UInt8 slotSwitch03;
	UInt8 slotItemType03;
	UInt32 slotItemID03;
	TCHAR slotItemName03[21];

	UInt8 slotSwitch04;
	UInt8 slotItemType04;
	UInt32 slotItemID04;
	TCHAR slotItemName04[21];

	UInt8 slotSwitch05;
	UInt8 slotItemType05;
	UInt32 slotItemID05;
	TCHAR slotItemName05[21];

	UInt8 slotSwitch06;
	UInt8 slotItemType06;
	UInt32 slotItemID06;
	TCHAR slotItemName06[21];

	TCHAR suggestion[101];
	UInt32 consumeTime;
	TCHAR lua[21];

	UInt8Vector consumeType;
	UInt32Vector consumeAmount;

	MixShopData()
		: shopID(C_UINT32(0))
		, type(C_UINT8(0))
		, skilllvl_low(C_UINT16(0))
		, skilllvl_up(C_UINT16(0))
		, slotSwitch01(C_UINT8(0))
		, slotItemType01(C_UINT8(0))
		, slotItemID01(C_UINT32(0))
		, slotSwitch02(C_UINT8(0))
		, slotItemType02(C_UINT8(0))
		, slotItemID02(C_UINT32(0))
		, slotSwitch03(C_UINT8(0))
		, slotItemType03(C_UINT8(0))
		, slotItemID03(C_UINT32(0))
		, slotSwitch04(C_UINT8(0))
		, slotItemType04(C_UINT8(0))
		, slotItemID04(C_UINT32(0))
		, slotSwitch05(C_UINT8(0))
		, slotItemType05(C_UINT8(0))
		, slotItemID05(C_UINT32(0))
		, slotSwitch06(C_UINT8(0))
		, slotItemType06(C_UINT8(0))
		, slotItemID06(C_UINT32(0))
		, consumeTime(C_UINT32(0))
	{
		ZeroMemory(shopName, sizeof(TCHAR) * 21);
		ZeroMemory(mixItemTable, sizeof(TCHAR) * 31);
		ZeroMemory(slotItemName01, sizeof(TCHAR) * 21);
		ZeroMemory(slotItemName02, sizeof(TCHAR) * 21);
		ZeroMemory(slotItemName03, sizeof(TCHAR) * 21);
		ZeroMemory(slotItemName04, sizeof(TCHAR) * 21);
		ZeroMemory(slotItemName05, sizeof(TCHAR) * 21);
		ZeroMemory(slotItemName06, sizeof(TCHAR) * 21);
		ZeroMemory(suggestion, sizeof(TCHAR) * 101);
		ZeroMemory(lua, sizeof(TCHAR) * 21);
	}

	~MixShopData() {}
};

struct ItemMixData
{
	UInt32 productID;
	TCHAR productName[21];
	TCHAR proDescription[256];
	TCHAR proFunctionDes[256];
	UInt16 skilllvl;
	UInt8 productType;
	UInt liveSkillID;
	UInt liveSkillLv;
	UInt8 Quality;

	UInt32Vector materials;
	UInt8Vector numOfMaterials;
	UInt8Vector materialSwitch;
	UInt32Vector output;
	UInt8Vector outputNum;
	UInt32Vector addon;

		
	UInt16 addon_Base;
	UInt32 consumeTime;
	TCHAR lua[21];

	UInt8Vector consumeType;
	UInt32Vector consumeAmount;

	ItemMixData()
		: productID(C_UINT32(0))
		, skilllvl(C_UINT16(0))
		, productType(C_UINT8(0))
		, liveSkillID(C_UINT(0))
		, liveSkillLv(C_UINT(0))
		, Quality(C_UINT8(0))
		, addon_Base(C_UINT16(0))
		, consumeTime(C_UINT32(0))
	{
		ZeroMemory(productName, sizeof(TCHAR) * 21);
		ZeroMemory(proDescription, sizeof(TCHAR) * 256);
		ZeroMemory(proFunctionDes, sizeof(TCHAR) * 256);
		ZeroMemory(lua, sizeof(TCHAR) * 21);
	}

	~ItemMixData() {}
};

struct StoneMixData
{
	UInt32 successItemID;
	UInt8Vector stoneNum;
	UInt8Vector stoneProb;

	StoneMixData()
		: successItemID(C_UINT32(0))
	{}

	~StoneMixData(){}
};

#endif