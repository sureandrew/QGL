#pragma once
#ifndef _ACCMULATION_SHOP_MANAGER_H_
#define _ACCMULATION_SHOP_MANAGER_H_

struct ACCSHOPITEM;

class CAccmulationShopManager
{
public:
	CAccmulationShopManager() { m_mapAccShopItems.clear();}
	~CAccmulationShopManager() { Release();}
public:
	int LoadCSV(const TCHAR *szFileName, const BYTE* pData = NULL, UInt32 pDataLen = 0);
	VOID Release();
	ACCSHOPITEM* GetAccmulationShopData(UInt32 uiIndex);
	size_t GetCAccmulationShopDataSize();
private:
	typedef StlMap<UInt32, ACCSHOPITEM*, std::less<UInt32>,ManagedAllocator<std::pair<UInt32, ACCSHOPITEM*> > > AccShopMap;
	AccShopMap m_mapAccShopItems;
};

#endif