#pragma once
#ifndef _GENERATIONDATA_H_
#define _GENERATIONDATA_H_

#include "Proxy/Data/GenerationData_Proxy.h"

class CGenerationData : public GenerationData_Proxy
{
private:
	typedef GenerationData_Proxy Parent;

public:
	DEFINE_CLASS(GenerationData_CLASSID);
};

#endif //_SKILLDATA_H_