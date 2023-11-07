///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_RELATIONSYSTEM_PROXY_H_
#define _RPC_RELATIONSYSTEM_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/RelationSystem_enum.h"

class RelationSystem_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_RequestSetSiblingTitlePrefix(LPCPEER pPeer, RPCContext &context, const String &thirdWord);
	virtual RPCResult CB_RequestSetSiblingTitleSuffix(LPCPEER pPeer, RPCContext &context, const String &prefix);
	virtual RPCResult CB_RequestSelectMarriageTime(LPCPEER pPeer, RPCContext &context);
	// OnUpdate can be implemented selectively
***/

protected:
	typedef NetObject Parent;

public:
	virtual RPCResult CB_RequestSetSiblingTitlePrefix(LPCPEER pPeer, RPCContext &context, const String &thirdWord) = 0;
	virtual RPCResult SetSiblingTitlePrefix(LPCPEER pPeer, RPCContext &context, const String &prefix);
	virtual RPCResult CB_RequestSetSiblingTitleSuffix(LPCPEER pPeer, RPCContext &context, const String &prefix) = 0;
	virtual RPCResult SetSiblingTitleSuffix(LPCPEER pPeer, RPCContext &context, const String &suffix);
	virtual RPCResult CB_RequestSelectMarriageTime(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult SelectMarriageTime(LPCPEER pPeer, RPCContext &context, const UInt32 &hour, const UInt32 &minute);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
} m;

public:
	INLINE RelationSystem_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult RelationSystem_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_RelationSystem_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<176564249>(pPeer, pBuf);
}

INLINE RPCResult RelationSystem_Proxy::SetSiblingTitlePrefix(LPCPEER pPeer, RPCContext &context, const String &prefix)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_RelationSystem_SetSiblingTitlePrefix);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(prefix);
	return RPCSend<550316454>(pPeer, pBuf);
}

INLINE RPCResult RelationSystem_Proxy::SetSiblingTitleSuffix(LPCPEER pPeer, RPCContext &context, const String &suffix)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_RelationSystem_SetSiblingTitleSuffix);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(suffix);
	return RPCSend<366479036>(pPeer, pBuf);
}

INLINE RPCResult RelationSystem_Proxy::SelectMarriageTime(LPCPEER pPeer, RPCContext &context, const UInt32 &hour, const UInt32 &minute)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_RelationSystem_SelectMarriageTime);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(hour);
	pBuf->Write<UInt32>(minute);
	return RPCSend<1460735450>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(653212037)
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(176564249)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(634673698)
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(550316454)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(921392118)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(366479036)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(665868924)
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1460735450)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_RELATIONSYSTEM_PROXY_H_
