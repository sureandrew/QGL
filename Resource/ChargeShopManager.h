#pragma once
#ifndef _CHARGE_SHOP_MANAGER_H_
#define _CHARGE_SHOP_MANAGER_H_

#include "Common.h"
#include "Common/common_binary.h"

struct ChargeShopData;

typedef StlVector<const ChargeShopData*, ManagedAllocator<const ChargeShopData* > > ChargeShopDataList;

class CChargeShopManager
{
public:
	
	CChargeShopManager();
	~CChargeShopManager();

	const ChargeShopData* GetChargeShopItem(UInt32 chargeShopID);

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);

	ChargeShopDataList GetChargeShopList();
	UInt32 GetTotalItemCount();
	void Release();

private:
	typedef StlMap<UInt32, ChargeShopData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, ChargeShopData*> > > ChargeShopMap;
	

	ChargeShopMap m_ChargeShop;
	ChargeShopDataList m_ChargeShopList;
	
};

#endif // _CHARGE_SHOP_MANAGER_H_