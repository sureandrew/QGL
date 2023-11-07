//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/ChargeShopData.h"

REGISTER_CLASSTYPE(CChargeShopData, ChargeShopData_CLASSID);

void CChargeShopData::ReducePreBuy(UInt preBuy)
{ 
	if (preBuy > m_iPreBuy)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("ReducePreBuy Error: m_iPreBuy %d < preBuy %d"), m_iPreBuy, preBuy);
		m_iPreBuy = 0;
	}
	else
		m_iPreBuy -= preBuy;
}