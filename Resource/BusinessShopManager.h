#pragma once
#ifndef _BUSINESSSHOP_MANAGER_H_
#define _BUSINESSSHOP_MANAGER_H_

struct BusinessShopData;
typedef StlMap<UInt16, BusinessShopData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, BusinessShopData*> > > BusinessShopDataMap;

class CBusinessShopManager
{
public:
	CBusinessShopManager();
	~CBusinessShopManager();

	const BusinessShopData* GetBusinessShopData(UInt32 shop_id);
	BusinessShopDataMap* GetBusinessShopDataMap(){return &m_BusinessShop;};
	
	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);

	void Release();

private:
	
	//typedef StlMap<UInt16, BusinessShopData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, BusinessShopData*> > > BusinessShopDataMap;
	BusinessShopDataMap	m_BusinessShop;
	
};

#endif // _BUSINESSSHOP_MANAGER_H_