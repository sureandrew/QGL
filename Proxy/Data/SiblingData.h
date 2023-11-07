#pragma once
#ifndef _SIBLINGDATA_H_
#define _SIBLINGDATA_H_

#include "Proxy/Data/SiblingData_Proxy.h"

class CSiblingData : public SiblingData_Proxy
{
public:
	// OnUpdate can be implemented selectively

	DEFINE_CLASS(SiblingData_CLASSID);

private:
	typedef SiblingData_Proxy Parent;

};

#endif //_SIBLINGDATA_H_
