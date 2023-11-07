#pragma once
#ifndef _MIX_SHOP_MANAGER_H_
#define _MIX_SHOP_MANAGER_H_

#include "Common.h"
#include "Common/common_binary.h"

struct MixShopData;
struct ItemMixData;
struct StoneMixData;

typedef StlVector<const ItemMixData*, ManagedAllocator<const ItemMixData* > > ItemMixDataList;

class CMixShopManager
{
public:
	CMixShopManager();
	~CMixShopManager();

	const MixShopData* GetMixShopData(UInt32 shopID);
	
	void GetSuitableItemList(String filename, UInt16 skillLv_low, UInt16 skillLv_up, ItemMixDataList &itemList);
	const ItemMixData* GetItemMixData(String filename, UInt32 productID);
	BOOL LoadMixShopCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);
	BOOL LoadItemMixCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);
	BOOL LoadMixProbCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);
	BOOL LoadAllItemMixCSV();
	void GetAllItemMixFileName(StringVector& fileList);
	void GetStoneProbList(UInt32 stoneID, UInt8Vector &probList);

	UInt GetMaxStoneMixNum(UInt32 stoneID);
	UInt GetMinStoneMixNum(UInt32 stoneID);
    UInt GetStoneMixProb(UInt32 stoneID, UInt stack);
	UInt GetResultStoneID(UInt32 stoneID);

	void Release();

private:
	typedef StlMap<UInt32, MixShopData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, MixShopData*> > > MixShopMap;
	typedef StlMap<UInt32, ItemMixData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, ItemMixData*> > > ItemMixMap;
	typedef StlMap<UInt32, UInt8Vector, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, UInt8Vector> > > MixProb;
	typedef StlMap<String, ItemMixMap, std::less<String>, ManagedAllocator<std::pair<String, ItemMixMap> > > FileItemMixMap;
	typedef StlMap<UInt32, StoneMixData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, StoneMixData*> > > MixStoneProb;
	
	MixShopMap m_mixShop;
	MixProb m_mixProb;

	MixStoneProb m_mixStone;
	FileItemMixMap m_fileItemMix;
};

#endif // _MIX_SHOP_MANAGER_H_