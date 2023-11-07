//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/GiftBox_proxy.h"


void GiftBox_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void GiftBox_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void GiftBox_Proxy::OnUpdate()
{
	Parent::OnUpdate();
}

#pragma optimize("", off)
RPCResult GiftBox_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_GiftBox_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_GiftBox_CB_TakeGiftBox:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_TakeGiftBox(pPeer, context, success, msgIndex);
		}
		break;
	case RPC_GiftBox_CB_UpdateGiftBox:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			NetGroup* updateGrp;
			pBuf->Read<NetGroup*>(updateGrp);
			_result = CB_UpdateGiftBox(pPeer, context, msgIndex, updateGrp);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult GiftBox_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_GiftBox_UP_STATE:
			_result = RPCDecode<429473798>(pPeer, pBuf);
		break;
	case RPC_GiftBox_CB_TakeGiftBox:
			_result = RPCDecode<1442668117>(pPeer, pBuf);
		break;
	case RPC_GiftBox_CB_UpdateGiftBox:
			_result = RPCDecode<1125544751>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
