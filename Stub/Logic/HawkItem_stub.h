///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_HAWKITEM_STUB_H_
#define _RPC_HAWKITEM_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/HawkItem_enum.h"

class HawkItem_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	void SetIdInfo(const UInt32 &item_uid, const UInt32 &item_id);
	void SetItem_uid(const UInt32 &item_uid);
	void SetItem_id(const UInt32 &item_id);
	void SetStackInfo(const UInt8 &item_stack);
	void SetItem_stack(const UInt8 &item_stack);
	void SetPriceInfo(const UInt32 &item_price);
	void SetItem_price(const UInt32 &item_price);
	void SetItemTypeInfo(const UInt8 &item_type);
	void SetItem_type(const UInt8 &item_type);
	void SetIndexInfo(const UInt8 &item_index);
	void SetItem_index(const UInt8 &item_index);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE HawkItem_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_HAWKITEM_STUB_H_
