///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GIFT_PROXY_H_
#define _RPC_GIFT_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Gift_enum.h"

class Gift_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateIdInfo(const UInt32 &giftID, const UInt32 &acctID);
	virtual void OnUpdateGiftInfo(const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemQuantity);
	virtual void OnUpdateSerialNoInfo(const String &serialNo);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idInfoMask	= Parent::NextFreeMask + 0,
		giftInfoMask	= Parent::NextFreeMask + 1,
		serialNoInfoMask	= Parent::NextFreeMask + 2,
		NextFreeMask	= Parent::NextFreeMask + 3
	};
	const UInt32& GetGiftID() const { return m.giftID; }
	const UInt32& GetAcctID() const { return m.acctID; }
	const UInt32& GetItemID() const { return m.itemID; }
	const UInt8& GetItemType() const { return m.itemType; }
	const UInt8& GetItemQuantity() const { return m.itemQuantity; }
	const String& GetSerialNo() const { return m.serialNo; }
	virtual void OnUpdateIdInfo(const UInt32 &giftID, const UInt32 &acctID) {};
	virtual void OnUpdateGiftInfo(const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemQuantity) {};
	virtual void OnUpdateSerialNoInfo(const String &serialNo) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//idInfo
	UInt32 giftID;
	UInt32 acctID;
	//giftInfo
	UInt32 itemID;
	UInt8 itemType;
	UInt8 itemQuantity;
	//serialNoInfo
	String serialNo;
} m;

public:
	INLINE Gift_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult Gift_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Gift_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1213148945>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(929695389)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1213148945)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_GIFT_PROXY_H_
