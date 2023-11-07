#pragma once
#ifndef _MESSENGERDATA_H_
#define _MESSENGERDATA_H_

#include "Proxy/Data/MessengerData_Proxy.h"

class CMessengerData : public MessengerData_Proxy
{
private:
	typedef MessengerData_Proxy Parent;

public:
	DEFINE_CLASS(MessengerData_CLASSID);
	static CMessengerData * NewInstance();
};

inline CMessengerData * CMessengerData::NewInstance()
{
	return CreateNetObject<CMessengerData>();
}

#endif //_MESSENGERDATA_H_
