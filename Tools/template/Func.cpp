//-- Common
#include "Common.h"
//-- Self
@@include

#pragma optimize("", off)
@@process
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
@@procase
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}
@@procend

@@decode
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
@@deccase
	}
	return _result;
}
@@decend
#pragma optimize("", on)

@@impl
