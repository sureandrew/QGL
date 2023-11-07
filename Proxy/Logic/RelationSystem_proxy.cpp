//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/RelationSystem_proxy.h"


void RelationSystem_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void RelationSystem_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void RelationSystem_Proxy::OnUpdate()
{
	Parent::OnUpdate();
}

#pragma optimize("", off)
RPCResult RelationSystem_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_RelationSystem_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_RelationSystem_CB_RequestSetSiblingTitlePrefix:
		{
			String thirdWord;
			pBuf->Read<String>(thirdWord);
			_result = CB_RequestSetSiblingTitlePrefix(pPeer, context, thirdWord);
		}
		break;
	case RPC_RelationSystem_CB_RequestSetSiblingTitleSuffix:
		{
			String prefix;
			pBuf->Read<String>(prefix);
			_result = CB_RequestSetSiblingTitleSuffix(pPeer, context, prefix);
		}
		break;
	case RPC_RelationSystem_CB_RequestSelectMarriageTime:
		{
			_result = CB_RequestSelectMarriageTime(pPeer, context);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult RelationSystem_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_RelationSystem_UP_STATE:
			_result = RPCDecode<653212037>(pPeer, pBuf);
		break;
	case RPC_RelationSystem_CB_RequestSetSiblingTitlePrefix:
			_result = RPCDecode<634673698>(pPeer, pBuf);
		break;
	case RPC_RelationSystem_CB_RequestSetSiblingTitleSuffix:
			_result = RPCDecode<921392118>(pPeer, pBuf);
		break;
	case RPC_RelationSystem_CB_RequestSelectMarriageTime:
			_result = RPCDecode<665868924>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
