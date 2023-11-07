///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SELLYUANBAOGROUP_PROXY_H_
#define _RPC_SELLYUANBAOGROUP_PROXY_H_

#include "Common/RPCFunc.h"
#include "Reuben/Network/NetGroup.h"
#include "RPCEnum/SellYuanBaoGroup_enum.h"

class SellYuanBaoGroup_Proxy : public NetGroup
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* sellYuanBaoGrp, const UInt8 &msgIndex);
	virtual RPCResult CB_GetAllYBTAndGMoney(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt32 &yuanbaoTicket, const UInt32 &gmoney);
	virtual RPCResult CB_BuyYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &msgIndex, const UInt32 &num);
	virtual RPCResult CB_RemoveSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID);
	virtual RPCResult CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &money);
	// OnUpdate can be implemented selectively
***/

protected:
	typedef NetGroup Parent;

public:
	virtual RPCResult GetTempBank(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CloseSellYuanBao(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* sellYuanBaoGrp, const UInt8 &msgIndex) = 0;
	virtual RPCResult CB_GetAllYBTAndGMoney(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt32 &yuanbaoTicket, const UInt32 &gmoney) = 0;
	virtual RPCResult CB_BuyYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &msgIndex, const UInt32 &num) = 0;
	virtual RPCResult CB_RemoveSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID) = 0;
	virtual RPCResult CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &money) = 0;
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
	INLINE SellYuanBaoGroup_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult SellYuanBaoGroup_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SellYuanBaoGroup_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<237203230>(pPeer, pBuf);
}

INLINE RPCResult SellYuanBaoGroup_Proxy::GetTempBank(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SellYuanBaoGroup_GetTempBank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1516584533>(pPeer, pBuf);
}

INLINE RPCResult SellYuanBaoGroup_Proxy::CloseSellYuanBao(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SellYuanBaoGroup_CloseSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<215442988>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1271231939)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(237203230)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1516584533)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(215442988)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(924075596)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1365920817)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1070680463)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(689386043)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1399658407)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_SELLYUANBAOGROUP_PROXY_H_
