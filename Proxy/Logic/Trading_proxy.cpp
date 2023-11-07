//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Trading_proxy.h"


void Trading_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Trading_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Trading_Proxy::OnUpdate()
{
	Parent::OnUpdate();
}

#pragma optimize("", off)
RPCResult Trading_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Trading_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_Trading_CB_SetItemSlot:
		{
			NetObject* item;
			pBuf->Read<NetObject*>(item);
			UInt8 position;
			pBuf->Read<UInt8>(position);
			_result = CB_SetItemSlot(pPeer, context, item, position);
		}
		break;
	case RPC_Trading_CB_RemoveItemSlot:
		{
			UInt8 position;
			pBuf->Read<UInt8>(position);
			_result = CB_RemoveItemSlot(pPeer, context, position);
		}
		break;
	case RPC_Trading_CB_SetPartnerSlot:
		{
			NetObject* partner;
			pBuf->Read<NetObject*>(partner);
			UInt16Vector skillID;
			pBuf->Read<UInt16Vector>(skillID);
			UInt8 position;
			pBuf->Read<UInt8>(position);
			_result = CB_SetPartnerSlot(pPeer, context, partner, skillID, position);
		}
		break;
	case RPC_Trading_CB_RemovePartnerSlot:
		{
			UInt8 position;
			pBuf->Read<UInt8>(position);
			_result = CB_RemovePartnerSlot(pPeer, context, position);
		}
		break;
	case RPC_Trading_CB_SetPetSlot:
		{
			NetObject* pet;
			pBuf->Read<NetObject*>(pet);
			UInt16Vector skillID;
			pBuf->Read<UInt16Vector>(skillID);
			UInt8 position;
			pBuf->Read<UInt8>(position);
			_result = CB_SetPetSlot(pPeer, context, pet, skillID, position);
		}
		break;
	case RPC_Trading_CB_RemovePetSlot:
		{
			UInt8 position;
			pBuf->Read<UInt8>(position);
			_result = CB_RemovePetSlot(pPeer, context, position);
		}
		break;
	case RPC_Trading_CB_SetLock:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			Boolean lock;
			pBuf->Read<Boolean>(lock);
			_result = CB_SetLock(pPeer, context, char_id, lock);
		}
		break;
	case RPC_Trading_CB_SetMoney:
		{
			UInt32 gmoney;
			pBuf->Read<UInt32>(gmoney);
			_result = CB_SetMoney(pPeer, context, gmoney);
		}
		break;
	case RPC_Trading_CB_CancelTrading:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			_result = CB_CancelTrading(pPeer, context, owner_id);
		}
		break;
	case RPC_Trading_CB_ConfirmTrading:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			_result = CB_ConfirmTrading(pPeer, context, owner_id);
		}
		break;
	case RPC_Trading_CB_TradingProcess:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			String message;
			pBuf->Read<String>(message);
			UInt32 errorCharID;
			pBuf->Read<UInt32>(errorCharID);
			_result = CB_TradingProcess(pPeer, context, success, message, errorCharID);
		}
		break;
	case RPC_Trading_CB_TradingItem:
		{
			UInt32Vector itemID;
			pBuf->Read<UInt32Vector>(itemID);
			UInt8Vector itemStacks;
			pBuf->Read<UInt8Vector>(itemStacks);
			_result = CB_TradingItem(pPeer, context, itemID, itemStacks);
		}
		break;
	case RPC_Trading_CB_TradingPartner:
		{
			UInt32Vector partnerID;
			pBuf->Read<UInt32Vector>(partnerID);
			_result = CB_TradingPartner(pPeer, context, partnerID);
		}
		break;
	case RPC_Trading_CB_TradingPet:
		{
			UInt32Vector petID;
			pBuf->Read<UInt32Vector>(petID);
			_result = CB_TradingPet(pPeer, context, petID);
		}
		break;
	case RPC_Trading_CB_TradingMoney:
		{
			UInt32 gmoney;
			pBuf->Read<UInt32>(gmoney);
			_result = CB_TradingMoney(pPeer, context, gmoney);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Trading_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Trading_UP_STATE:
			_result = RPCDecode<959381602>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_SetItemSlot:
			_result = RPCDecode<2134127773>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_RemoveItemSlot:
			_result = RPCDecode<20530320>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_SetPartnerSlot:
			_result = RPCDecode<1743412576>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_RemovePartnerSlot:
			_result = RPCDecode<130701276>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_SetPetSlot:
			_result = RPCDecode<293800319>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_RemovePetSlot:
			_result = RPCDecode<1745489520>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_SetLock:
			_result = RPCDecode<164179813>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_SetMoney:
			_result = RPCDecode<799174000>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_CancelTrading:
			_result = RPCDecode<85228765>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_ConfirmTrading:
			_result = RPCDecode<633995943>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_TradingProcess:
			_result = RPCDecode<88889239>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_TradingItem:
			_result = RPCDecode<315894103>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_TradingPartner:
			_result = RPCDecode<1744515603>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_TradingPet:
			_result = RPCDecode<1410363786>(pPeer, pBuf);
		break;
	case RPC_Trading_CB_TradingMoney:
			_result = RPCDecode<672272129>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
