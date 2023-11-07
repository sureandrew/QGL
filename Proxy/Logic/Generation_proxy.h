///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GENERATION_PROXY_H_
#define _RPC_GENERATION_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Generation_enum.h"

class Generation_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateIds(const UInt32 &uiRelatedId);
	virtual void OnUpdateInfo(const String &strNickName, const UInt8 &ui8Class, const UInt16 &ui16Level, const UInt32 &uiOfflineTime, const UInt32 &uiAccmulOrRecvExp, const UInt32 &uiGiftPointOrHistAccm);
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
	const UInt32& GetUiRelatedId() const { return m.uiRelatedId; }
	const String& GetStrNickName() const { return m.strNickName; }
	const UInt8& GetUi8Class() const { return m.ui8Class; }
	const UInt16& GetUi16Level() const { return m.ui16Level; }
	const UInt32& GetUiOfflineTime() const { return m.uiOfflineTime; }
	const UInt32& GetUiAccmulOrRecvExp() const { return m.uiAccmulOrRecvExp; }
	const UInt32& GetUiGiftPointOrHistAccm() const { return m.uiGiftPointOrHistAccm; }
	const BOOL& GetBYoung() const { return m.bYoung; }
	virtual void OnUpdateIds(const UInt32 &uiRelatedId) {};
	virtual void OnUpdateInfo(const String &strNickName, const UInt8 &ui8Class, const UInt16 &ui16Level, const UInt32 &uiOfflineTime, const UInt32 &uiAccmulOrRecvExp, const UInt32 &uiGiftPointOrHistAccm) {};
	virtual void OnUpdateFlag(const BOOL &bYoung) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

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
	INLINE Generation_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult Generation_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Generation_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1707025338>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1512597031)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1707025338)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_GENERATION_PROXY_H_
