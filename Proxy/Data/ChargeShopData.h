#pragma once
#ifndef _CHARGESHOPDATA_H_
#define _CHARGESHOPDATA_H_

#include "Proxy/Data/ChargeShopData_Proxy.h"

class CChargeShopData : public ChargeShopData_Proxy
{
private:
	typedef ChargeShopData_Proxy Parent;
	UInt m_iPreBuy;

public:
	DEFINE_CLASS(ChargeShopData_CLASSID);
	CChargeShopData(){	m_iPreBuy = 0;}
	void AddPreBuy(UInt preBuy){ m_iPreBuy += preBuy;}
	void ReducePreBuy(UInt preBuy);
	UInt GetPreBuy() { return m_iPreBuy;}
	void ResetPreBuy(){ m_iPreBuy = 0;}
};

#endif //_CHARGESHOPDATA_H_
