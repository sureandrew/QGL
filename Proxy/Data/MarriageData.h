#pragma once
#ifndef _MARRIAGEDATA_H_
#define _MARRIAGEDATA_H_

#include "Proxy/Data/MarriageData_Proxy.h"

class CMarriageData : public MarriageData_Proxy
{
public:
	// OnUpdate can be implemented selectively

	DEFINE_CLASS(MarriageData_CLASSID);

private:
	typedef MarriageData_Proxy Parent;

};

#endif //_MARRIAGEDATA_H_
