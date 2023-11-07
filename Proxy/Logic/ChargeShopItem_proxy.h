///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_CHARGESHOPITEM_PROXY_H_
#define _RPC_CHARGESHOPITEM_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ChargeShopItem_enum.h"

class ChargeShopItem_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateIdInfo(const UInt32 &chargeShopID);
	virtual void OnUpdateItemInfo(const String &itemName, const UInt8 &shopType, const UInt8 &itemType, const UInt32 &itemId, const UInt32 &yuanBao, const UInt8 &LV, const UInt8 &PreviewType);
	virtual void OnUpdateDiscountInfo(const UInt8 &discount);
	virtual void OnUpdateStockInfo(const UInt16 &totalStock);
	virtual void OnUpdateRemainInfo(const UInt16 &remain);
	virtual void OnUpdateLimitedInfo(const UInt8 &LimitType, const UInt8 &restore);
	virtual void OnUpdateExistTimeInfo(const UInt32 &StartTime, const UInt32 &EndTime);
	virtual void OnUpdateLastOrderInfo(const UInt32 &lastOrderDate);
	virtual void OnUpdateSellInfo(const UInt32 &totalSell);
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
	virtual void OnUpdateIdInfo(const UInt32 &chargeShopID) {};
	virtual void OnUpdateItemInfo(const String &itemName, const UInt8 &shopType, const UInt8 &itemType, const UInt32 &itemId, const UInt32 &yuanBao, const UInt8 &LV, const UInt8 &PreviewType) {};
	virtual void OnUpdateDiscountInfo(const UInt8 &discount) {};
	virtual void OnUpdateStockInfo(const UInt16 &totalStock) {};
	virtual void OnUpdateRemainInfo(const UInt16 &remain) {};
	virtual void OnUpdateLimitedInfo(const UInt8 &LimitType, const UInt8 &restore) {};
	virtual void OnUpdateExistTimeInfo(const UInt32 &StartTime, const UInt32 &EndTime) {};
	virtual void OnUpdateLastOrderInfo(const UInt32 &lastOrderDate) {};
	virtual void OnUpdateSellInfo(const UInt32 &totalSell) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

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
	INLINE ChargeShopItem_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult ChargeShopItem_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ChargeShopItem_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<474165627>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(596444539)
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(474165627)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_CHARGESHOPITEM_PROXY_H_
