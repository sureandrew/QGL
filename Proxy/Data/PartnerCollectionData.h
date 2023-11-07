#pragma once
#ifndef _PARTNERCOLLECTIONDATA_H_
#define _PARTNERCOLLECTIONDATA_H_

#include "Proxy/Data/PartnerCollectionData_Proxy.h"

class CPartnerCollectionData : public PartnerCollectionData_Proxy
{
public:
	// OnUpdate can be implemented selectively

	DEFINE_CLASS(PartnerCollectionData_CLASSID);

private:
	typedef PartnerCollectionData_Proxy Parent;

};

#endif //_PARTNERCOLLECTIONDATA_H_
