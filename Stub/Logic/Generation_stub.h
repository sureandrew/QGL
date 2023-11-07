///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GENERATION_STUB_H_
#define _RPC_GENERATION_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Generation_enum.h"

class Generation_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idsMask	= Parent::NextFreeMask + 0,
		infoMask	= Parent::NextFreeMask + 1,
		flagMask	= Parent::NextFreeMask + 2,
		NextFreeMask	= Parent::NextFreeMask + 3
	};
	const UInt32& GetUiRelatedId() const { return m.uiRelatedId; }
	const String& GetStrNickName() const { return m.strNickName; }
	const UInt8& GetUi8Class() const { return m.ui8Class; }
	const UInt16& GetUi16Level() const { return m.ui16Level; }
	const UInt32& GetUiOfflineTime() const { return m.uiOfflineTime; }
	const UInt32& GetUiAccmulOrRecvExp() const { return m.uiAccmulOrRecvExp; }
	const UInt32& GetUiGiftPointOrHistAccm() const { return m.uiGiftPointOrHistAccm; }
	const BOOL& GetBYoung() const { return m.bYoung; }
	void SetIds(const UInt32 &uiRelatedId);
	void SetUiRelatedId(const UInt32 &uiRelatedId);
	void SetInfo(const String &strNickName, const UInt8 &ui8Class, const UInt16 &ui16Level, const UInt32 &uiOfflineTime, const UInt32 &uiAccmulOrRecvExp, const UInt32 &uiGiftPointOrHistAccm);
	void SetStrNickName(const String &strNickName);
	void SetUi8Class(const UInt8 &ui8Class);
	void SetUi16Level(const UInt16 &ui16Level);
	void SetUiOfflineTime(const UInt32 &uiOfflineTime);
	void SetUiAccmulOrRecvExp(const UInt32 &uiAccmulOrRecvExp);
	void SetUiGiftPointOrHistAccm(const UInt32 &uiGiftPointOrHistAccm);
	void SetFlag(const BOOL &bYoung);
	void SetBYoung(const BOOL &bYoung);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//ids
	UInt32 uiRelatedId;
	//info
	String strNickName;
	UInt8 ui8Class;
	UInt16 ui16Level;
	UInt32 uiOfflineTime;
	UInt32 uiAccmulOrRecvExp;
	UInt32 uiGiftPointOrHistAccm;
	//flag
	BOOL bYoung;
} m;

public:
	INLINE Generation_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_GENERATION_STUB_H_
