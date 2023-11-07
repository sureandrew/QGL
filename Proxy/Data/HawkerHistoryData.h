#pragma once
#ifndef _HAWKERHISTORYDATA_H_
#define _HAWKERHISTORYDATA_H_

#include "Proxy/Data/HawkerHistoryData_Proxy.h"

class CHawkerHistoryData : public HawkerHistoryData_Proxy
{
public:
	// OnUpdate can be implemented selectively
	/*virtual void OnUpdateHistoryId(const UInt32 &historyId);
	virtual void OnUpdateHistoryInfo(const UInt32 &char_id, const String &itemName, const UInt8 &itemType, const UInt8 &stack, const UInt32 &totalPrice, const UInt32 &tax);
	virtual void OnUpdateHistoryDate(const UInt32 &historyDate);*/

	DEFINE_CLASS(HawkerHistoryData_CLASSID);

private:
	typedef HawkerHistoryData_Proxy Parent;

};

#endif //_HAWKERHISTORYDATA_H_
