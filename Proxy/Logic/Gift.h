#pragma once
#ifndef _GIFT_H_
#define _GIFT_H_

#include "Proxy/Logic/Gift_Proxy.h"

class CGift : public Gift_Proxy
{
private:
	typedef Gift_Proxy Parent;

public:
	DEFINE_CLASS(Gift_CLASSID);
// OnUpdate can be implemented selectively
	virtual void OnUpdateIdInfo(const UInt32 &giftID, const UInt32 &acctID);
	virtual void OnUpdateGiftInfo(const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemQuantity);
};

#endif //_GIFT_H_
