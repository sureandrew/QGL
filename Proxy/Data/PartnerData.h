#pragma once
#ifndef _PARTNERDATA_H_
#define _PARTNERDATA_H_

#include "Proxy/Data/PartnerData_Proxy.h"

class CPartnerData : public PartnerData_Proxy
{
public:
	enum DataDetail {
		DETAIL_NONE = 0,
		DETAIL_INFO,
		DETAIL_MAP,
		DETAIL_BATTLE,
		DETAIL_ALL,
	};

private:
	typedef PartnerData_Proxy Parent;

public:
	DEFINE_CLASS(PartnerData_CLASSID);
// OnUpdate can be implemented selectively
};

#endif //_PARTNERDATA_H_
