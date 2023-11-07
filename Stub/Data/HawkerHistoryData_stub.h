///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_HAWKERHISTORYDATA_STUB_H_
#define _RPC_HAWKERHISTORYDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/HawkerHistoryData_enum.h"

class HawkerHistoryData_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	void SetHistoryId(const UInt32 &historyId);
	void SetHistoryInfo(const UInt32 &char_id, const String &itemName, const UInt8 &itemType, const UInt8 &stack, const UInt32 &totalPrice, const UInt32 &tax);
	void SetChar_id(const UInt32 &char_id);
	void SetItemName(const String &itemName);
	void SetItemType(const UInt8 &itemType);
	void SetStack(const UInt8 &stack);
	void SetTotalPrice(const UInt32 &totalPrice);
	void SetTax(const UInt32 &tax);
	void SetHistoryDate(const UInt32 &historyDate);
	void Clone(HawkerHistoryData_Stub* obj, const RPCMask &mask);
	friend void HawkerHistoryData_Stub::Clone(HawkerHistoryData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE HawkerHistoryData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_HAWKERHISTORYDATA_STUB_H_
