//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/ChargeShop_proxy.h"


void ChargeShop_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void ChargeShop_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void ChargeShop_Proxy::OnUpdate()
{
	Parent::OnUpdate();
}

#pragma optimize("", off)
RPCResult ChargeShop_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ChargeShop_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_ChargeShop_CB_BuyItemInChargeShop:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			UInt8 msgCode;
			pBuf->Read<UInt8>(msgCode);
			UInt32 chargeItemID;
			pBuf->Read<UInt32>(chargeItemID);
			UInt8 itemType;
			pBuf->Read<UInt8>(itemType);
			UInt32 stack;
			pBuf->Read<UInt32>(stack);
			UInt32 yuanbao;
			pBuf->Read<UInt32>(yuanbao);
			_result = CB_BuyItemInChargeShop(pPeer, context, success, msgCode, chargeItemID, itemType, stack, yuanbao);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult ChargeShop_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_ChargeShop_UP_STATE:
			_result = RPCDecode<1096633362>(pPeer, pBuf);
		break;
	case RPC_ChargeShop_CB_BuyItemInChargeShop:
			_result = RPCDecode<1677995046>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
