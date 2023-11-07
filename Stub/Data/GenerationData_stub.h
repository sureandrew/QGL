///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GENERATIONDATA_STUB_H_
#define _RPC_GENERATIONDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/GenerationData_enum.h"

class GenerationData_Stub : public NetObject
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
	const UInt32& GetUiYoungerId() const { return m.uiYoungerId; }
	const UInt32& GetUiElderId() const { return m.uiElderId; }
	const String& GetStrNickName() const { return m.strNickName; }
	const UInt32& GetUiAccmulOrRecvExp() const { return m.uiAccmulOrRecvExp; }
	const UInt32& GetUiGiftPointOrHistAccm() const { return m.uiGiftPointOrHistAccm; }
	const BOOL& GetBYoung() const { return m.bYoung; }
	void SetIds(const UInt32 &uiYoungerId, const UInt32 &uiElderId);
	void SetUiYoungerId(const UInt32 &uiYoungerId);
	void SetUiElderId(const UInt32 &uiElderId);
	void SetInfo(const String &strNickName, const UInt32 &uiAccmulOrRecvExp, const UInt32 &uiGiftPointOrHistAccm);
	void SetStrNickName(const String &strNickName);
	void SetUiAccmulOrRecvExp(const UInt32 &uiAccmulOrRecvExp);
	void SetUiGiftPointOrHistAccm(const UInt32 &uiGiftPointOrHistAccm);
	void SetFlag(const BOOL &bYoung);
	void SetBYoung(const BOOL &bYoung);
	void Clone(GenerationData_Stub* obj, const RPCMask &mask);
	friend void GenerationData_Stub::Clone(GenerationData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//ids
	UInt32 uiYoungerId;
	UInt32 uiElderId;
	//info
	String strNickName;
	UInt32 uiAccmulOrRecvExp;
	UInt32 uiGiftPointOrHistAccm;
	//flag
	BOOL bYoung;
} m;

public:
	INLINE GenerationData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_GENERATIONDATA_STUB_H_
