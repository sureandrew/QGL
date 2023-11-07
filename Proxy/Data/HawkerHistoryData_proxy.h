///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_HAWKERHISTORYDATA_PROXY_H_
#define _RPC_HAWKERHISTORYDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/HawkerHistoryData_enum.h"

class HawkerHistoryData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateHistoryId(const UInt32 &historyId);
	virtual void OnUpdateHistoryInfo(const UInt32 &char_id, const String &itemName, const UInt8 &itemType, const UInt8 &stack, const UInt32 &totalPrice, const UInt32 &tax);
	virtual void OnUpdateHistoryDate(const UInt32 &historyDate);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		historyIdMask	= Parent::NextFreeMask + 0,
		historyInfoMask	= Parent::NextFreeMask + 1,
		historyDateMask	= Parent::NextFreeMask + 2,
		NextFreeMask	= Parent::NextFreeMask + 3
	};
	const UInt32& GetHistoryId() const { return m.historyId; }
	const UInt32& GetChar_id() const { return m.char_id; }
	const String& GetItemName() const { return m.itemName; }
	const UInt8& GetItemType() const { return m.itemType; }
	const UInt8& GetStack() const { return m.stack; }
	const UInt32& GetTotalPrice() const { return m.totalPrice; }
	const UInt32& GetTax() const { return m.tax; }
	const UInt32& GetHistoryDate() const { return m.historyDate; }
	virtual void OnUpdateHistoryId(const UInt32 &historyId) {};
	virtual void OnUpdateHistoryInfo(const UInt32 &char_id, const String &itemName, const UInt8 &itemType, const UInt8 &stack, const UInt32 &totalPrice, const UInt32 &tax) {};
	virtual void OnUpdateHistoryDate(const UInt32 &historyDate) {};
	void SetHistoryId(const UInt32 &historyId);
	void SetHistoryInfo(const UInt32 &char_id, const String &itemName, const UInt8 &itemType, const UInt8 &stack, const UInt32 &totalPrice, const UInt32 &tax);
	void SetChar_id(const UInt32 &char_id);
	void SetItemName(const String &itemName);
	void SetItemType(const UInt8 &itemType);
	void SetStack(const UInt8 &stack);
	void SetTotalPrice(const UInt32 &totalPrice);
	void SetTax(const UInt32 &tax);
	void SetHistoryDate(const UInt32 &historyDate);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//historyId
	UInt32 historyId;
	//historyInfo
	UInt32 char_id;
	String itemName;
	UInt8 itemType;
	UInt8 stack;
	UInt32 totalPrice;
	UInt32 tax;
	//historyDate
	UInt32 historyDate;
} m;

public:
	INLINE HawkerHistoryData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult HawkerHistoryData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_HawkerHistoryData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1222730190>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1719150326)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1222730190)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_HAWKERHISTORYDATA_PROXY_H_
