#pragma once
#ifndef _PETDATA_H_
#define _PETDATA_H_

#include "Proxy/Data/PetData_Proxy.h"

class CPetData : public PetData_Proxy
{
private:
	typedef PetData_Proxy Parent;
public:
	DEFINE_CLASS(PetData_CLASSID);
// OnUpdate can be implemented selectively
};

#endif //_PETDATA_H_
