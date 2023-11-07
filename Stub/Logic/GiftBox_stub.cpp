//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/GiftBox_stub.h"



void GiftBox_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void GiftBox_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

RPCResult GiftBox_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GiftBox_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult GiftBox_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_GiftBox_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_GiftBox_TakeGiftBox:
		{
			UInt32 giftID;
			pBuf->Read<UInt32>(giftID);
			_result = TakeGiftBox(pPeer, context, giftID);
		}
		break;
	case RPC_GiftBox_CloseGiftBox:
		{
			_result = CloseGiftBox(pPeer, context);
		}
		break;
	case RPC_GiftBox_UpdateGiftBox:
		{
			_result = UpdateGiftBox(pPeer, context);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult GiftBox_Stub::CB_TakeGiftBox(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GiftBox_CB_TakeGiftBox);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(success);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend<1442668117>(pPeer, pBuf);
}

RPCResult GiftBox_Stub::CB_UpdateGiftBox(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, NetGroup* updateGrp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GiftBox_CB_UpdateGiftBox);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(msgIndex);
	pBuf->Write<NetGroup*>(updateGrp);
	return RPCSend<1125544751>(pPeer, pBuf);
}

