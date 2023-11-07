///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_TRADING_STUB_H_
#define _RPC_TRADING_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Trading_enum.h"

class Trading_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult SetTradingItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &itemBagIndex, const UInt8 &stack, const UInt8 &tradingPosition);
	virtual RPCResult RemoveTradingItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &tradingPosition);
	virtual RPCResult SetTradingPartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID, const UInt8 &position);
	virtual RPCResult RemoveTradingPartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position);
	virtual RPCResult SetTradingPetSlot(LPCPEER pPeer, RPCContext &context, const UInt32 &petID, const UInt8 &position);
	virtual RPCResult RemoveTradingPetSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position);
	virtual RPCResult SetTradingLock(LPCPEER pPeer, RPCContext &context, const Boolean &lock);
	virtual RPCResult SetTradingMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney);
	virtual RPCResult CancelTrading(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult ConfirmTrading(LPCPEER pPeer, RPCContext &context);
***/

protected:
	typedef NetObject Parent;

public:
	virtual RPCResult SetTradingItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &itemBagIndex, const UInt8 &stack, const UInt8 &tradingPosition) = 0;
	virtual RPCResult RemoveTradingItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &tradingPosition) = 0;
	virtual RPCResult SetTradingPartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID, const UInt8 &position) = 0;
	virtual RPCResult RemoveTradingPartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position) = 0;
	virtual RPCResult SetTradingPetSlot(LPCPEER pPeer, RPCContext &context, const UInt32 &petID, const UInt8 &position) = 0;
	virtual RPCResult RemoveTradingPetSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position) = 0;
	virtual RPCResult SetTradingLock(LPCPEER pPeer, RPCContext &context, const Boolean &lock) = 0;
	virtual RPCResult SetTradingMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney) = 0;
	virtual RPCResult CancelTrading(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult ConfirmTrading(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_SetItemSlot(LPCPEER pPeer, RPCContext &context, NetObject* item, const UInt8 &position);
	virtual RPCResult CB_RemoveItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position);
	virtual RPCResult CB_SetPartnerSlot(LPCPEER pPeer, RPCContext &context, NetObject* partner, const UInt16Vector &skillID, const UInt8 &position);
	virtual RPCResult CB_RemovePartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position);
	virtual RPCResult CB_SetPetSlot(LPCPEER pPeer, RPCContext &context, NetObject* pet, const UInt16Vector &skillID, const UInt8 &position);
	virtual RPCResult CB_RemovePetSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position);
	virtual RPCResult CB_SetLock(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const Boolean &lock);
	virtual RPCResult CB_SetMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney);
	virtual RPCResult CB_CancelTrading(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id);
	virtual RPCResult CB_ConfirmTrading(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id);
	virtual RPCResult CB_TradingProcess(LPCPEER pPeer, RPCContext &context, const Boolean &success, const String &message, const UInt32 &errorCharID);
	virtual RPCResult CB_TradingItem(LPCPEER pPeer, RPCContext &context, const UInt32Vector &itemID, const UInt8Vector &itemStacks);
	virtual RPCResult CB_TradingPartner(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerID);
	virtual RPCResult CB_TradingPet(LPCPEER pPeer, RPCContext &context, const UInt32Vector &petID);
	virtual RPCResult CB_TradingMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
} m;

public:
	INLINE Trading_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_TRADING_STUB_H_
