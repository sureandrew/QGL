#pragma once
#ifndef _POINTSSHOPMANAGER_H_
#define _POINTSSHOPMANAGER_H_

struct POINTSSHOPITEM;

class CPointsShopManager
{
public:
	CPointsShopManager() { m_mapPointsShopItems.clear();}
	~CPointsShopManager() { Release();}
public:
	int LoadCSV(const TCHAR *szFileName, const BYTE* pData = NULL, UInt32 pDataLen = 0);
	VOID Release();
	POINTSSHOPITEM* GetData(UInt32 uiItemID);
	size_t GetSize();
private:
	typedef StlMap<UInt32, POINTSSHOPITEM*, std::less<UInt32>,ManagedAllocator<std::pair<UInt32, POINTSSHOPITEM*> > > PointsShopMap;
	PointsShopMap m_mapPointsShopItems;
};

#endif