///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PARTNERCOLLECTIONDATA_PROXY_H_
#define _RPC_PARTNERCOLLECTIONDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/PartnerCollectionData_enum.h"

class PartnerCollectionData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateKey(const UInt32 &collection_uid);
	virtual void OnUpdateInfo(const UInt32 &collection_id, const UInt32 &char_id);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		keyMask	= Parent::NextFreeMask + 0,
		infoMask	= Parent::NextFreeMask + 1,
		NextFreeMask	= Parent::NextFreeMask + 2
	};
	const UInt32& GetCollection_uid() const { return m.collection_uid; }
	const UInt32& GetCollection_id() const { return m.collection_id; }
	const UInt32& GetChar_id() const { return m.char_id; }
	virtual void OnUpdateKey(const UInt32 &collection_uid) {};
	virtual void OnUpdateInfo(const UInt32 &collection_id, const UInt32 &char_id) {};
	void SetKey(const UInt32 &collection_uid);
	void SetCollection_uid(const UInt32 &collection_uid);
	void SetInfo(const UInt32 &collection_id, const UInt32 &char_id);
	void SetCollection_id(const UInt32 &collection_id);
	void SetChar_id(const UInt32 &char_id);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//key
	UInt32 collection_uid;
	//info
	UInt32 collection_id;
	UInt32 char_id;
} m;

public:
	INLINE PartnerCollectionData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult PartnerCollectionData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerCollectionData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<2041345554>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1650458909)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(2041345554)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_PARTNERCOLLECTIONDATA_PROXY_H_
