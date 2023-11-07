#pragma once
#ifndef _GIFTDATA_H_
#define _GIFTDATA_H_

#include "Proxy/Data/GiftData_Proxy.h"

class CGiftData : public GiftData_Proxy
{
private:
	typedef GiftData_Proxy Parent;

public:
	DEFINE_CLASS(GiftData_CLASSID);
// OnUpdate can be implemented selectively
	virtual void OnUpdateIdInfo(const UInt32 &giftID, const UInt32 &acctID);
	virtual void OnUpdateGiftInfo(const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemQuantity);
};

#endif //_GIFTDATA_H_
