#pragma once
#ifndef _CHARGESHOPDATA_H_
#define _CHARGESHOPDATA_H_

#include "Stub/Data/ChargeShopData_Stub.h"

class CdboChargeShop;


class CChargeShopData : public ChargeShopData_Stub
{
public:
	DEFINE_CLASS(ChargeShopData_CLASSID);
	static CChargeShopData * NewInstance();
	void SetData(CdboChargeShop* dboChargeShop);
	void PreSend();
	HRESULT UpdateDbo(CdboChargeShop* dboChargeShop);
	HRESULT DeleteDbo(CdboChargeShop* dboChargeShop);

private:
	typedef ChargeShopData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CChargeShopData * CChargeShopData::NewInstance()
{
	return CreateNetObject<CChargeShopData>();
}

#endif //_CHARGESHOPDATA_H_
