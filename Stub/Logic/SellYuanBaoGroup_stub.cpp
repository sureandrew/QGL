//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/SellYuanBaoGroup_stub.h"



void SellYuanBaoGroup_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void SellYuanBaoGroup_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

RPCResult SellYuanBaoGroup_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SellYuanBaoGroup_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult SellYuanBaoGroup_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_SellYuanBaoGroup_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_SellYuanBaoGroup_GetTempBank:
		{
			_result = GetTempBank(pPeer, context);
		}
		break;
	case RPC_SellYuanBaoGroup_CloseSellYuanBao:
		{
			_result = CloseSellYuanBao(pPeer, context);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult SellYuanBaoGroup_Stub::CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* sellYuanBaoGrp, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SellYuanBaoGroup_CB_AddSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(sellYuanBaoGrp);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend<924075596>(pPeer, pBuf);
}

RPCResult SellYuanBaoGroup_Stub::CB_GetAllYBTAndGMoney(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt32 &yuanbaoTicket, const UInt32 &gmoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SellYuanBaoGroup_CB_GetAllYBTAndGMoney);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(msgIndex);
	pBuf->Write<UInt32>(yuanbaoTicket);
	pBuf->Write<UInt32>(gmoney);
	return RPCSend<1365920817>(pPeer, pBuf);
}

RPCResult SellYuanBaoGroup_Stub::CB_BuyYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &msgIndex, const UInt32 &num)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SellYuanBaoGroup_CB_BuyYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(msgIndex);
	pBuf->Write<UInt32>(num);
	return RPCSend<1070680463>(pPeer, pBuf);
}

RPCResult SellYuanBaoGroup_Stub::CB_RemoveSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SellYuanBaoGroup_CB_RemoveSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(sellYuanBaoID);
	return RPCSend<689386043>(pPeer, pBuf);
}

RPCResult SellYuanBaoGroup_Stub::CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &money)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SellYuanBaoGroup_CB_GetTempBank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(YBT);
	pBuf->Write<UInt32>(money);
	return RPCSend<1399658407>(pPeer, pBuf);
}

