#pragma once
#ifndef _GENERATION_H_
#define _GENERATION_H_

#include "Proxy/Logic/Generation_Proxy.h"

class CGeneration : public Generation_Proxy
{
public:
	// OnUpdate can be implemented selectively

	DEFINE_CLASS(Generation_CLASSID);

private:
	typedef Generation_Proxy Parent;

};

#endif //_GENERATION_H_
