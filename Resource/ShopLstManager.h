#pragma once
#ifndef _SHOP_LIST_MANAGER_H_
#define _SHOP_LIST_MANAGER_H_

struct ShopLstData;

class CShopLstManager
{
public:
	CShopLstManager();
	~CShopLstManager();

	BOOL LoadCSV(LPCTSTR szfilename);
	CONST ShopLstData* GetShop(UInt32 shopId);

	VOID Release();
private:
	typedef StlMap<UInt32, ShopLstData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, ShopLstData*> > > CSVShopTable;

	CSVShopTable m_ShopList;
};

#endif // _SHOP_LIST_MANAGER_H_