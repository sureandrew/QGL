#pragma once
#ifndef _TITLEDATA_H_
#define _TITLEDATA_H_

#include "Proxy/Data/TitleData_Proxy.h"

class CTitleData : public TitleData_Proxy
{
private:
	typedef TitleData_Proxy Parent;
// OnUpdate can be implemented selectively
public:
	DEFINE_CLASS(TitleData_CLASSID);

};

#endif //_TITLEDATA_H_
