///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_CHARGESHOPITEM_STUB_H_
#define _RPC_CHARGESHOPITEM_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ChargeShopItem_enum.h"

class ChargeShopItem_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idInfoMask	= Parent::NextFreeMask + 0,
		itemInfoMask	= Parent::NextFreeMask + 1,
		discountInfoMask	= Parent::NextFreeMask + 2,
		stockInfoMask	= Parent::NextFreeMask + 3,
		remainInfoMask	= Parent::NextFreeMask + 4,
		limitedInfoMask	= Parent::NextFreeMask + 5,
		existTimeInfoMask	= Parent::NextFreeMask + 6,
		lastOrderInfoMask	= Parent::NextFreeMask + 7,
		sellInfoMask	= Parent::NextFreeMask + 8,
		NextFreeMask	= Parent::NextFreeMask + 9
	};
	const UInt32& GetChargeShopID() const { return m.chargeShopID; }
	const String& GetItemName() const { return m.itemName; }
	const UInt8& GetShopType() const { return m.shopType; }
	const UInt8& GetItemType() const { return m.itemType; }
	const UInt32& GetItemId() const { return m.itemId; }
	const UInt32& GetYuanBao() const { return m.yuanBao; }
	const UInt8& GetLV() const { return m.LV; }
	const UInt8& GetPreviewType() const { return m.PreviewType; }
	const UInt8& GetDiscount() const { return m.discount; }
	const UInt16& GetTotalStock() const { return m.totalStock; }
	const UInt16& GetRemain() const { return m.remain; }
	const UInt8& GetLimitType() const { return m.LimitType; }
	const UInt8& GetRestore() const { return m.restore; }
	const UInt32& GetStartTime() const { return m.StartTime; }
	const UInt32& GetEndTime() const { return m.EndTime; }
	const UInt32& GetLastOrderDate() const { return m.lastOrderDate; }
	const UInt32& GetTotalSell() const { return m.totalSell; }
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
	void SetIdInfo(const UInt32 &chargeShopID);
	void SetChargeShopID(const UInt32 &chargeShopID);
	void SetItemInfo(const String &itemName, const UInt8 &shopType, const UInt8 &itemType, const UInt32 &itemId, const UInt32 &yuanBao, const UInt8 &LV, const UInt8 &PreviewType);
	void SetItemName(const String &itemName);
	void SetShopType(const UInt8 &shopType);
	void SetItemType(const UInt8 &itemType);
	void SetItemId(const UInt32 &itemId);
	void SetYuanBao(const UInt32 &yuanBao);
	void SetLV(const UInt8 &LV);
	void SetPreviewType(const UInt8 &PreviewType);
	void SetDiscountInfo(const UInt8 &discount);
	void SetDiscount(const UInt8 &discount);
	void SetStockInfo(const UInt16 &totalStock);
	void SetTotalStock(const UInt16 &totalStock);
	void SetRemainInfo(const UInt16 &remain);
	void SetRemain(const UInt16 &remain);
	void SetLimitedInfo(const UInt8 &LimitType, const UInt8 &restore);
	void SetLimitType(const UInt8 &LimitType);
	void SetRestore(const UInt8 &restore);
	void SetExistTimeInfo(const UInt32 &StartTime, const UInt32 &EndTime);
	void SetStartTime(const UInt32 &StartTime);
	void SetEndTime(const UInt32 &EndTime);
	void SetLastOrderInfo(const UInt32 &lastOrderDate);
	void SetLastOrderDate(const UInt32 &lastOrderDate);
	void SetSellInfo(const UInt32 &totalSell);
	void SetTotalSell(const UInt32 &totalSell);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//idInfo
	UInt32 chargeShopID;
	//itemInfo
	String itemName;
	UInt8 shopType;
	UInt8 itemType;
	UInt32 itemId;
	UInt32 yuanBao;
	UInt8 LV;
	UInt8 PreviewType;
	//discountInfo
	UInt8 discount;
	//stockInfo
	UInt16 totalStock;
	//remainInfo
	UInt16 remain;
	//limitedInfo
	UInt8 LimitType;
	UInt8 restore;
	//existTimeInfo
	UInt32 StartTime;
	UInt32 EndTime;
	//lastOrderInfo
	UInt32 lastOrderDate;
	//sellInfo
	UInt32 totalSell;
} m;

public:
	INLINE ChargeShopItem_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_CHARGESHOPITEM_STUB_H_
