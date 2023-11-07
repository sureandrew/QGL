#pragma once
#ifndef _GIFT_H_
#define _GIFT_H_

#include "Proxy/Logic/Gift_Proxy.h"

class CGift : public Gift_Proxy
{
public:
	DEFINE_CLASS(Gift_CLASSID);

private:
	typedef Gift_Proxy Parent;

};

#endif //_GIFT_H_
