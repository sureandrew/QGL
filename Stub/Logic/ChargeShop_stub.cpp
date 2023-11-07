//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/ChargeShop_stub.h"



void ChargeShop_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void ChargeShop_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

RPCResult ChargeShop_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ChargeShop_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult ChargeShop_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ChargeShop_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_ChargeShop_BuyItemInChargeShop:
		{
			UInt32 chargeShopID;
			pBuf->Read<UInt32>(chargeShopID);
			UInt16 stack;
			pBuf->Read<UInt16>(stack);
			_result = BuyItemInChargeShop(pPeer, context, chargeShopID, stack);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult ChargeShop_Stub::CB_BuyItemInChargeShop(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode, const UInt32 &chargeItemID, const UInt8 &itemType, const UInt32 &stack, const UInt32 &yuanbao)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ChargeShop_CB_BuyItemInChargeShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(success);
	pBuf->Write<UInt8>(msgCode);
	pBuf->Write<UInt32>(chargeItemID);
	pBuf->Write<UInt8>(itemType);
	pBuf->Write<UInt32>(stack);
	pBuf->Write<UInt32>(yuanbao);
	return RPCSend<1677995046>(pPeer, pBuf);
}

