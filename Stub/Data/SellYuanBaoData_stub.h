///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SELLYUANBAODATA_STUB_H_
#define _RPC_SELLYUANBAODATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/SellYuanBaoData_enum.h"

class SellYuanBaoData_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	void SetId(const UInt32 &sellYuanBao_id);
	void SetSellYuanBao_id(const UInt32 &sellYuanBao_id);
	void SetCharInfo(const UInt32 &char_id, const String &charName);
	void SetChar_id(const UInt32 &char_id);
	void SetCharName(const String &charName);
	void SetSellInfo(const UInt8 &sellType, const UInt32 &totalYBT, const UInt32 &moneyPerYBT, const UInt32 &expireDate);
	void SetSellType(const UInt8 &sellType);
	void SetTotalYBT(const UInt32 &totalYBT);
	void SetMoneyPerYBT(const UInt32 &moneyPerYBT);
	void SetExpireDate(const UInt32 &expireDate);
	void SetSoldYBT(const UInt32 &soldYBT);
	void Clone(SellYuanBaoData_Stub* obj, const RPCMask &mask);
	friend void SellYuanBaoData_Stub::Clone(SellYuanBaoData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE SellYuanBaoData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_SELLYUANBAODATA_STUB_H_
