///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_TRADING_PROXY_H_
#define _RPC_TRADING_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Trading_enum.h"

class Trading_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	// OnUpdate can be implemented selectively
***/

protected:
	typedef NetObject Parent;

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
	virtual RPCResult CB_SetItemSlot(LPCPEER pPeer, RPCContext &context, NetObject* item, const UInt8 &position) = 0;
	virtual RPCResult CB_RemoveItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position) = 0;
	virtual RPCResult CB_SetPartnerSlot(LPCPEER pPeer, RPCContext &context, NetObject* partner, const UInt16Vector &skillID, const UInt8 &position) = 0;
	virtual RPCResult CB_RemovePartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position) = 0;
	virtual RPCResult CB_SetPetSlot(LPCPEER pPeer, RPCContext &context, NetObject* pet, const UInt16Vector &skillID, const UInt8 &position) = 0;
	virtual RPCResult CB_RemovePetSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position) = 0;
	virtual RPCResult CB_SetLock(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const Boolean &lock) = 0;
	virtual RPCResult CB_SetMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney) = 0;
	virtual RPCResult CB_CancelTrading(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id) = 0;
	virtual RPCResult CB_ConfirmTrading(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id) = 0;
	virtual RPCResult CB_TradingProcess(LPCPEER pPeer, RPCContext &context, const Boolean &success, const String &message, const UInt32 &errorCharID) = 0;
	virtual RPCResult CB_TradingItem(LPCPEER pPeer, RPCContext &context, const UInt32Vector &itemID, const UInt8Vector &itemStacks) = 0;
	virtual RPCResult CB_TradingPartner(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerID) = 0;
	virtual RPCResult CB_TradingPet(LPCPEER pPeer, RPCContext &context, const UInt32Vector &petID) = 0;
	virtual RPCResult CB_TradingMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney) = 0;
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
} m;

public:
	INLINE Trading_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult Trading_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<523592870>(pPeer, pBuf);
}

INLINE RPCResult Trading_Proxy::SetTradingItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &itemBagIndex, const UInt8 &stack, const UInt8 &tradingPosition)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_SetTradingItemSlot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(itemBagIndex);
	pBuf->Write<UInt8>(stack);
	pBuf->Write<UInt8>(tradingPosition);
	return RPCSend<1871933618>(pPeer, pBuf);
}

INLINE RPCResult Trading_Proxy::RemoveTradingItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &tradingPosition)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_RemoveTradingItemSlot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(tradingPosition);
	return RPCSend<41514946>(pPeer, pBuf);
}

INLINE RPCResult Trading_Proxy::SetTradingPartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID, const UInt8 &position)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_SetTradingPartnerSlot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(partnerID);
	pBuf->Write<UInt8>(position);
	return RPCSend<133844652>(pPeer, pBuf);
}

INLINE RPCResult Trading_Proxy::RemoveTradingPartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_RemoveTradingPartnerSlot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(position);
	return RPCSend<1111360652>(pPeer, pBuf);
}

INLINE RPCResult Trading_Proxy::SetTradingPetSlot(LPCPEER pPeer, RPCContext &context, const UInt32 &petID, const UInt8 &position)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_SetTradingPetSlot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(petID);
	pBuf->Write<UInt8>(position);
	return RPCSend<874194450>(pPeer, pBuf);
}

INLINE RPCResult Trading_Proxy::RemoveTradingPetSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_RemoveTradingPetSlot);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(position);
	return RPCSend<1081767140>(pPeer, pBuf);
}

INLINE RPCResult Trading_Proxy::SetTradingLock(LPCPEER pPeer, RPCContext &context, const Boolean &lock)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_SetTradingLock);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(lock);
	return RPCSend<271393074>(pPeer, pBuf);
}

INLINE RPCResult Trading_Proxy::SetTradingMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_SetTradingMoney);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(gmoney);
	return RPCSend<1330329215>(pPeer, pBuf);
}

INLINE RPCResult Trading_Proxy::CancelTrading(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_CancelTrading);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<143218057>(pPeer, pBuf);
}

INLINE RPCResult Trading_Proxy::ConfirmTrading(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Trading_ConfirmTrading);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<896626933>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(959381602)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(523592870)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1871933618)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(41514946)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(133844652)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1111360652)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(874194450)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1081767140)
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(271393074)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1330329215)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(143218057)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(896626933)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(2134127773)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(20530320)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1743412576)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(130701276)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(293800319)
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1745489520)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(164179813)
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(799174000)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(85228765)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(633995943)
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(88889239)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(315894103)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1744515603)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1410363786)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(672272129)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_TRADING_PROXY_H_
