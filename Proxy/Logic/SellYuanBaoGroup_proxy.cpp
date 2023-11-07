//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/SellYuanBaoGroup_proxy.h"


void SellYuanBaoGroup_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void SellYuanBaoGroup_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void SellYuanBaoGroup_Proxy::OnUpdate()
{
	Parent::OnUpdate();
}

#pragma optimize("", off)
RPCResult SellYuanBaoGroup_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_SellYuanBaoGroup_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_SellYuanBaoGroup_CB_AddSellYuanBao:
		{
			NetGroup* sellYuanBaoGrp;
			pBuf->Read<NetGroup*>(sellYuanBaoGrp);
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_AddSellYuanBao(pPeer, context, sellYuanBaoGrp, msgIndex);
		}
		break;
	case RPC_SellYuanBaoGroup_CB_GetAllYBTAndGMoney:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			UInt32 yuanbaoTicket;
			pBuf->Read<UInt32>(yuanbaoTicket);
			UInt32 gmoney;
			pBuf->Read<UInt32>(gmoney);
			_result = CB_GetAllYBTAndGMoney(pPeer, context, msgIndex, yuanbaoTicket, gmoney);
		}
		break;
	case RPC_SellYuanBaoGroup_CB_BuyYuanBao:
		{
			UInt32 msgIndex;
			pBuf->Read<UInt32>(msgIndex);
			UInt32 num;
			pBuf->Read<UInt32>(num);
			_result = CB_BuyYuanBao(pPeer, context, msgIndex, num);
		}
		break;
	case RPC_SellYuanBaoGroup_CB_RemoveSellYuanBao:
		{
			UInt32 sellYuanBaoID;
			pBuf->Read<UInt32>(sellYuanBaoID);
			_result = CB_RemoveSellYuanBao(pPeer, context, sellYuanBaoID);
		}
		break;
	case RPC_SellYuanBaoGroup_CB_GetTempBank:
		{
			UInt32 YBT;
			pBuf->Read<UInt32>(YBT);
			UInt32 money;
			pBuf->Read<UInt32>(money);
			_result = CB_GetTempBank(pPeer, context, YBT, money);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult SellYuanBaoGroup_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_SellYuanBaoGroup_UP_STATE:
			_result = RPCDecode<1271231939>(pPeer, pBuf);
		break;
	case RPC_SellYuanBaoGroup_CB_AddSellYuanBao:
			_result = RPCDecode<924075596>(pPeer, pBuf);
		break;
	case RPC_SellYuanBaoGroup_CB_GetAllYBTAndGMoney:
			_result = RPCDecode<1365920817>(pPeer, pBuf);
		break;
	case RPC_SellYuanBaoGroup_CB_BuyYuanBao:
			_result = RPCDecode<1070680463>(pPeer, pBuf);
		break;
	case RPC_SellYuanBaoGroup_CB_RemoveSellYuanBao:
			_result = RPCDecode<689386043>(pPeer, pBuf);
		break;
	case RPC_SellYuanBaoGroup_CB_GetTempBank:
			_result = RPCDecode<1399658407>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
