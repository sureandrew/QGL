//-- Common
#include "Common.h"
//-- Self
@@include

@@setimpl

@@cloneimpl
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
@@cloneset
}
@@cloneend

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

@@sendattr
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
@@upattr
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
@@process
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
@@reqcase
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
@@procase
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
@@procend
#pragma optimize("", on)

@@impl

