///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GENERATIONDATA_PROXY_H_
#define _RPC_GENERATIONDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/GenerationData_enum.h"

class GenerationData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateIds(const UInt32 &uiYoungerId, const UInt32 &uiElderId);
	virtual void OnUpdateInfo(const String &strNickName, const UInt32 &uiAccmulOrRecvExp, const UInt32 &uiGiftPointOrHistAccm);
	virtual void OnUpdateFlag(const BOOL &bYoung);
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
	virtual void OnUpdateIds(const UInt32 &uiYoungerId, const UInt32 &uiElderId) {};
	virtual void OnUpdateInfo(const String &strNickName, const UInt32 &uiAccmulOrRecvExp, const UInt32 &uiGiftPointOrHistAccm) {};
	virtual void OnUpdateFlag(const BOOL &bYoung) {};
	void SetIds(const UInt32 &uiYoungerId, const UInt32 &uiElderId);
	void SetUiYoungerId(const UInt32 &uiYoungerId);
	void SetUiElderId(const UInt32 &uiElderId);
	void SetInfo(const String &strNickName, const UInt32 &uiAccmulOrRecvExp, const UInt32 &uiGiftPointOrHistAccm);
	void SetStrNickName(const String &strNickName);
	void SetUiAccmulOrRecvExp(const UInt32 &uiAccmulOrRecvExp);
	void SetUiGiftPointOrHistAccm(const UInt32 &uiGiftPointOrHistAccm);
	void SetFlag(const BOOL &bYoung);
	void SetBYoung(const BOOL &bYoung);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

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
	INLINE GenerationData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult GenerationData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GenerationData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<776623162>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(2008179101)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(776623162)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_GENERATIONDATA_PROXY_H_
