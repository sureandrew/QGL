//-- Common
#include "Common.h"
//-- Self
@@include

@@setimpl

@@readattr
{
	Parent::ReadAttr(buf);
@@readbit
}

@@writeattr
{
	Parent::WriteAttr(buf, mask);
@@writebit
}

@@onupdate
{
	Parent::OnUpdate();
@@updatecall
}

#pragma optimize("", off)
@@process
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
@@upcase
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
@@procase
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
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
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
@@decend
#pragma optimize("", on)
