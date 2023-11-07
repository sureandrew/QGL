#pragma once
#ifndef _BUFFDATA_H_
#define _BUFFDATA_H_

#include "Proxy/Data/BuffData_Proxy.h"

class CBuffData : public BuffData_Proxy
{
private:
	typedef BuffData_Proxy Parent;

public:
	DEFINE_CLASS(BuffData_CLASSID);
};

#endif //_BUFFDATA_H_
