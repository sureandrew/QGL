#pragma once
#ifndef _SELLYUANBAODATA_H_
#define _SELLYUANBAODATA_H_

#include "Proxy/Data/SellYuanBaoData_Proxy.h"

class CSellYuanBaoData : public SellYuanBaoData_Proxy
{
public:
	// OnUpdate can be implemented selectively

	DEFINE_CLASS(SellYuanBaoData_CLASSID);

private:
	typedef SellYuanBaoData_Proxy Parent;

};

#endif //_SELLYUANBAODATA_H_
