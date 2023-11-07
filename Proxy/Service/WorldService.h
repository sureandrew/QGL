#pragma once
#ifndef _WORLDSERVICE_H_
#define _WORLDSERVICE_H_
#include "Proxy/Service/WorldService_Proxy.h"

// public functions here
namespace WorldService_Proxy
{
	int SendApolloMsg(int msgId, UInt32 arg1, UInt32 arg2);
};

#endif //_WORLDSERVICE_H_
