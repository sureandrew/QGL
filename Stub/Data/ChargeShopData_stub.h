///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_CHARGESHOPDATA_STUB_H_
#define _RPC_CHARGESHOPDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ChargeShopData_enum.h"

class ChargeShopData_Stub : public NetObject
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
		activeInfoMask	= Parent::NextFreeMask + 2,
		discountInfoMask	= Parent::NextFreeMask + 3,
		stockInfoMask	= Parent::NextFreeMask + 4,
		remainInfoMask	= Parent::NextFreeMask + 5,
		limitedInfoMask	= Parent::NextFreeMask + 6,
		existTimeInfoMask	= Parent::NextFreeMask + 7,
		lastOrderInfoMask	= Parent::NextFreeMask + 8,
		sellInfoMask	= Parent::NextFreeMask + 9,
		NextFreeMask	= Parent::NextFreeMask + 10
	};
	const UInt32& GetChargeShopID() const { return m.chargeShopID; }
	const String& GetName() const { return m.name; }
	const UInt8& GetShopType() const { return m.shopType; }
	const UInt8& GetItemType() const { return m.itemType; }
	const UInt32& GetItemId() const { return m.itemId; }
	const UInt32& GetYuanBao() const { return m.yuanBao; }
	const UInt8& GetLV() const { return m.LV; }
	const UInt8& GetPreviewType() const { return m.PreviewType; }
	const UInt8& GetActive() const { return m.active; }
	const UInt8& GetDiscount() const { return m.discount; }
	const UInt16& GetTotalStock() const { return m.totalStock; }
	const UInt16& GetRemain() const { return m.remain; }
	const UInt8& GetLimitType() const { return m.LimitType; }
	const UInt8& GetRestore() const { return m.restore; }
	const UInt32& GetStartTime() const { return m.StartTime; }
	const UInt32& GetEndTime() const { return m.EndTime; }
	const UInt32& GetLastOrderDate() const { return m.lastOrderDate; }
	const UInt32& GetTotalSell() const { return m.totalSell; }
	void SetIdInfo(const UInt32 &chargeShopID);
	void SetChargeShopID(const UInt32 &chargeShopID);
	void SetItemInfo(const String &name, const UInt8 &shopType, const UInt8 &itemType, const UInt32 &itemId, const UInt32 &yuanBao, const UInt8 &LV, const UInt8 &PreviewType);
	void SetName(const String &name);
	void SetShopType(const UInt8 &shopType);
	void SetItemType(const UInt8 &itemType);
	void SetItemId(const UInt32 &itemId);
	void SetYuanBao(const UInt32 &yuanBao);
	void SetLV(const UInt8 &LV);
	void SetPreviewType(const UInt8 &PreviewType);
	void SetActiveInfo(const UInt8 &active);
	void SetActive(const UInt8 &active);
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
	void Clone(ChargeShopData_Stub* obj, const RPCMask &mask);
	friend void ChargeShopData_Stub::Clone(ChargeShopData_Stub* obj, const RPCMask &mask);
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
	String name;
	UInt8 shopType;
	UInt8 itemType;
	UInt32 itemId;
	UInt32 yuanBao;
	UInt8 LV;
	UInt8 PreviewType;
	//activeInfo
	UInt8 active;
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
	INLINE ChargeShopData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_CHARGESHOPDATA_STUB_H_
