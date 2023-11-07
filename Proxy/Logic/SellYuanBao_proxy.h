///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SELLYUANBAO_PROXY_H_
#define _RPC_SELLYUANBAO_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/SellYuanBao_enum.h"

class SellYuanBao_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &sellYuanBao_id);
	virtual void OnUpdateCharInfo(const UInt32 &char_id, const String &charName);
	virtual void OnUpdateSellInfo(const UInt8 &sellType, const UInt32 &totalYBT, const UInt32 &moneyPerYBT, const UInt32 &expireDate);
	virtual void OnUpdateSoldYBT(const UInt32 &soldYBT);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		charInfoMask	= Parent::NextFreeMask + 1,
		sellInfoMask	= Parent::NextFreeMask + 2,
		soldYBTMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	const UInt32& GetSellYuanBao_id() const { return m.sellYuanBao_id; }
	const UInt32& GetChar_id() const { return m.char_id; }
	const String& GetCharName() const { return m.charName; }
	const UInt8& GetSellType() const { return m.sellType; }
	const UInt32& GetTotalYBT() const { return m.totalYBT; }
	const UInt32& GetMoneyPerYBT() const { return m.moneyPerYBT; }
	const UInt32& GetExpireDate() const { return m.expireDate; }
	const UInt32& GetSoldYBT() const { return m.soldYBT; }
	virtual void OnUpdateId(const UInt32 &sellYuanBao_id) {};
	virtual void OnUpdateCharInfo(const UInt32 &char_id, const String &charName) {};
	virtual void OnUpdateSellInfo(const UInt8 &sellType, const UInt32 &totalYBT, const UInt32 &moneyPerYBT, const UInt32 &expireDate) {};
	virtual void OnUpdateSoldYBT(const UInt32 &soldYBT) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 sellYuanBao_id;
	//charInfo
	UInt32 char_id;
	String charName;
	//sellInfo
	UInt8 sellType;
	UInt32 totalYBT;
	UInt32 moneyPerYBT;
	UInt32 expireDate;
	//soldYBT
	UInt32 soldYBT;
} m;

public:
	INLINE SellYuanBao_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult SellYuanBao_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SellYuanBao_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1386153849>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1102668347)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1386153849)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_SELLYUANBAO_PROXY_H_
