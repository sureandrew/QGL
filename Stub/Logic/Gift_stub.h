///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GIFT_STUB_H_
#define _RPC_GIFT_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Gift_enum.h"

class Gift_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	RPCMask m_modifyMask;
	INLINE Boolean IsModified() { return (!m_modifyMask.IsZero()); }
	INLINE Boolean IsModified(UInt bit) { return (m_modifyMask.IsBit(bit)); }
	INLINE Boolean IsModified(CONST RPCMask& mask) { return (m_modifyMask.IsMask(mask)); }
	INLINE VOID SetModifyMask(CONST RPCMask& mask) { m_modifyMask = mask; }
	INLINE CONST RPCMask& GetModifyMask() CONST { return m_modifyMask; }
	INLINE VOID ClearModifyMask() { m_modifyMask.Clear(); }
	INLINE VOID MarkModify(UInt bit) { m_modifyMask.MaskBit(bit); }
	INLINE VOID MarkModify(CONST RPCMask& mask) { m_modifyMask.Mask(mask); }
	INLINE VOID UnmarkModify(UInt bit) { m_modifyMask.UnmaskBit(bit); }
	INLINE VOID UnmarkModify(CONST RPCMask& mask) { m_modifyMask.Unmask(mask); }
	INLINE virtual VOID ClearAllMask() {
		Parent::ClearAllMask();
		m_modifyMask.Clear();
	}
	INLINE virtual VOID MarkUpdateAll(UInt bit, bool modified) {
		Parent::MarkUpdateAll(bit, modified);
		if (modified) m_modifyMask.MaskBit(bit);
	}
	void SetIdInfo(const UInt32 &giftID, const UInt32 &acctID);
	void SetGiftID(const UInt32 &giftID);
	void SetAcctID(const UInt32 &acctID);
	void SetGiftInfo(const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemQuantity);
	void SetItemID(const UInt32 &itemID);
	void SetItemType(const UInt8 &itemType);
	void SetItemQuantity(const UInt8 &itemQuantity);
	void SetSerialNoInfo(const String &serialNo);
	void SetSerialNo(const String &serialNo);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE Gift_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_GIFT_STUB_H_
