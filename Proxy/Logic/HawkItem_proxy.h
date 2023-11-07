///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_HAWKITEM_PROXY_H_
#define _RPC_HAWKITEM_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/HawkItem_enum.h"

class HawkItem_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateIdInfo(const UInt32 &item_uid, const UInt32 &item_id);
	virtual void OnUpdateStackInfo(const UInt8 &item_stack);
	virtual void OnUpdatePriceInfo(const UInt32 &item_price);
	virtual void OnUpdateItemTypeInfo(const UInt8 &item_type);
	virtual void OnUpdateIndexInfo(const UInt8 &item_index);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idInfoMask	= Parent::NextFreeMask + 0,
		stackInfoMask	= Parent::NextFreeMask + 1,
		priceInfoMask	= Parent::NextFreeMask + 2,
		itemTypeInfoMask	= Parent::NextFreeMask + 3,
		indexInfoMask	= Parent::NextFreeMask + 4,
		NextFreeMask	= Parent::NextFreeMask + 5
	};
	const UInt32& GetItem_uid() const { return m.item_uid; }
	const UInt32& GetItem_id() const { return m.item_id; }
	const UInt8& GetItem_stack() const { return m.item_stack; }
	const UInt32& GetItem_price() const { return m.item_price; }
	const UInt8& GetItem_type() const { return m.item_type; }
	const UInt8& GetItem_index() const { return m.item_index; }
	virtual void OnUpdateIdInfo(const UInt32 &item_uid, const UInt32 &item_id) {};
	virtual void OnUpdateStackInfo(const UInt8 &item_stack) {};
	virtual void OnUpdatePriceInfo(const UInt32 &item_price) {};
	virtual void OnUpdateItemTypeInfo(const UInt8 &item_type) {};
	virtual void OnUpdateIndexInfo(const UInt8 &item_index) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//idInfo
	UInt32 item_uid;
	UInt32 item_id;
	//stackInfo
	UInt8 item_stack;
	//priceInfo
	UInt32 item_price;
	//itemTypeInfo
	UInt8 item_type;
	//indexInfo
	UInt8 item_index;
} m;

public:
	INLINE HawkItem_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult HawkItem_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_HawkItem_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1887579439>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(858285763)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1887579439)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_HAWKITEM_PROXY_H_
