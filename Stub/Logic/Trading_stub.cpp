//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Trading_stub.h"



void Trading_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Trading_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

RPCResult Trading_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Trading_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Trading_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_Trading_SetTradingItemSlot:
		{
			UInt8 itemBagIndex;
			pBuf->Read<UInt8>(itemBagIndex);
			UInt8 stack;
			pBuf->Read<UInt8>(stack);
			UInt8 tradingPosition;
			pBuf->Read<UInt8>(tradingPosition);
			_result = SetTradingItemSlot(pPeer, context, itemBagIndex, stack, tradingPosition);
		}
		break;
	case RPC_Trading_RemoveTradingItemSlot:
		{
			UInt8 tradingPosition;
			pBuf->Read<UInt8>(tradingPosition);
			_result = RemoveTradingItemSlot(pPeer, context, tradingPosition);
		}
		break;
	case RPC_Trading_SetTradingPartnerSlot:
		{
			UInt32 partnerID;
			pBuf->Read<UInt32>(partnerID);
			UInt8 position;
			pBuf->Read<UInt8>(position);
			_result = SetTradingPartnerSlot(pPeer, context, partnerID, position);
		}
		break;
	case RPC_Trading_RemoveTradingPartnerSlot:
		{
			UInt8 position;
			pBuf->Read<UInt8>(position);
			_result = RemoveTradingPartnerSlot(pPeer, context, position);
		}
		break;
	case RPC_Trading_SetTradingPetSlot:
		{
			UInt32 petID;
			pBuf->Read<UInt32>(petID);
			UInt8 position;
			pBuf->Read<UInt8>(position);
			_result = SetTradingPetSlot(pPeer, context, petID, position);
		}
		break;
	case RPC_Trading_RemoveTradingPetSlot:
		{
			UInt8 position;
			pBuf->Read<UInt8>(position);
			_result = RemoveTradingPetSlot(pPeer, context, position);
		}
		break;
	case RPC_Trading_SetTradingLock:
		{
			Boolean lock;
			pBuf->Read<Boolean>(lock);
			_result = SetTradingLock(pPeer, context, lock);
		}
		break;
	case RPC_Trading_SetTradingMoney:
		{
			UInt32 gmoney;
			pBuf->Read<UInt32>(gmoney);
			_result = SetTradingMoney(pPeer, context, gmoney);
		}
		break;
	case RPC_Trading_CancelTrading:
		{
			_result = CancelTrading(pPeer, context);
		}
		break;
	case RPC_Trading_ConfirmTrading:
		{
			_result = ConfirmTrading(pPeer, context);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult Trading_Stub::CB_SetItemSlot(LPCPEER pPeer, RPCContext &context, NetObject* item, const UInt8 &position)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_SetItemSlot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(item);
	pBuf->Write<UInt8>(position);
	return RPCSend<2134127773>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_RemoveItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_RemoveItemSlot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(position);
	return RPCSend<20530320>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_SetPartnerSlot(LPCPEER pPeer, RPCContext &context, NetObject* partner, const UInt16Vector &skillID, const UInt8 &position)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_SetPartnerSlot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(partner);
	pBuf->Write<UInt16Vector>(skillID);
	pBuf->Write<UInt8>(position);
	return RPCSend<1743412576>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_RemovePartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_RemovePartnerSlot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(position);
	return RPCSend<130701276>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_SetPetSlot(LPCPEER pPeer, RPCContext &context, NetObject* pet, const UInt16Vector &skillID, const UInt8 &position)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_SetPetSlot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(pet);
	pBuf->Write<UInt16Vector>(skillID);
	pBuf->Write<UInt8>(position);
	return RPCSend<293800319>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_RemovePetSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_RemovePetSlot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(position);
	return RPCSend<1745489520>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_SetLock(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const Boolean &lock)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_SetLock);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<Boolean>(lock);
	return RPCSend<164179813>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_SetMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_SetMoney);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(gmoney);
	return RPCSend<799174000>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_CancelTrading(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_CancelTrading);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend<85228765>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_ConfirmTrading(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_ConfirmTrading);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend<633995943>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_TradingProcess(LPCPEER pPeer, RPCContext &context, const Boolean &success, const String &message, const UInt32 &errorCharID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_TradingProcess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(success);
	pBuf->Write<String>(message);
	pBuf->Write<UInt32>(errorCharID);
	return RPCSend<88889239>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_TradingItem(LPCPEER pPeer, RPCContext &context, const UInt32Vector &itemID, const UInt8Vector &itemStacks)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_TradingItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32Vector>(itemID);
	pBuf->Write<UInt8Vector>(itemStacks);
	return RPCSend<315894103>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_TradingPartner(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_TradingPartner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32Vector>(partnerID);
	return RPCSend<1744515603>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_TradingPet(LPCPEER pPeer, RPCContext &context, const UInt32Vector &petID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_TradingPet);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32Vector>(petID);
	return RPCSend<1410363786>(pPeer, pBuf);
}

RPCResult Trading_Stub::CB_TradingMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CB_TradingMoney);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(gmoney);
	return RPCSend<672272129>(pPeer, pBuf);
}

