///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_TITLEDATA_PROXY_H_
#define _RPC_TITLEDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/TitleData_enum.h"

class TitleData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateKey(const UInt32 &title_uid);
	virtual void OnUpdateInfo(const UInt16 &title_id);
	virtual void OnUpdateOwner(const UInt32 &owner_id);
	virtual void OnUpdateExpireDate(const UInt32 &expireDate);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		keyMask	= Parent::NextFreeMask + 0,
		infoMask	= Parent::NextFreeMask + 1,
		ownerMask	= Parent::NextFreeMask + 2,
		expireDateMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	const UInt32& GetTitle_uid() const { return m.title_uid; }
	const UInt16& GetTitle_id() const { return m.title_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const UInt32& GetExpireDate() const { return m.expireDate; }
	virtual void OnUpdateKey(const UInt32 &title_uid) {};
	virtual void OnUpdateInfo(const UInt16 &title_id) {};
	virtual void OnUpdateOwner(const UInt32 &owner_id) {};
	virtual void OnUpdateExpireDate(const UInt32 &expireDate) {};
	void SetKey(const UInt32 &title_uid);
	void SetTitle_uid(const UInt32 &title_uid);
	void SetInfo(const UInt16 &title_id);
	void SetTitle_id(const UInt16 &title_id);
	void SetOwner(const UInt32 &owner_id);
	void SetOwner_id(const UInt32 &owner_id);
	void SetExpireDate(const UInt32 &expireDate);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//key
	UInt32 title_uid;
	//info
	UInt16 title_id;
	//owner
	UInt32 owner_id;
	//expireDate
	UInt32 expireDate;
} m;

public:
	INLINE TitleData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult TitleData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_TitleData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<661741758>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(432293902)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(661741758)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_TITLEDATA_PROXY_H_
