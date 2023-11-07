#pragma once
#ifndef _EXECUTECHEATDATA_H_
#define _EXECUTECHEATDATA_H_

#include "Proxy/Data/ExecuteCheatData_Proxy.h"

class CExecuteCheatData : public ExecuteCheatData_Proxy
{
public:
	// OnUpdate can be implemented selectively

	DEFINE_CLASS(ExecuteCheatData_CLASSID);

private:
	typedef ExecuteCheatData_Proxy Parent;

};

#endif //_EXECUTECHEATDATA_H_
