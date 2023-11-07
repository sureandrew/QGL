///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PARTNERGROUP_PROXY_H_
#define _RPC_PARTNERGROUP_PROXY_H_

#include "Common/RPCFunc.h"
#include "Reuben/Network/NetGroup.h"
#include "RPCEnum/PartnerGroup_enum.h"

class PartnerGroup_Proxy : public NetGroup
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_DeleteListSuccess(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerIDs, const bool &showDelMsg);
	virtual RPCResult CB_AddPartnerGroup(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup, NetGroup* skillGroups);
	virtual RPCResult CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AddPartner(LPCPEER pPeer, RPCContext &context, NetObject* partner, NetGroup* skillGroup);
	virtual RPCResult CB_UpdateAllPartner(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup);
	virtual RPCResult CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp);
	virtual RPCResult CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex);
	virtual RPCResult CB_PartnerCultivationUp(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt8 &msgValue, const UInt32 &src_partnerID);
	// OnUpdate can be implemented selectively
***/

protected:
	typedef NetGroup Parent;

public:
	virtual RPCResult DeletePartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id);
	virtual RPCResult ChangePartnerState(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id, const UInt8 &state);
	virtual RPCResult PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID);
	virtual RPCResult GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID);
	virtual RPCResult PartnerCultivationUpByPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &src_partnerID, const UInt32 &dest_partnerID);
	virtual RPCResult PartnerCultivationUpByItem(LPCPEER pPeer, RPCContext &context, const UInt32 &src_partnerID, const UInt32 &dest_itemUid);
	virtual RPCResult CB_DeleteListSuccess(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerIDs, const bool &showDelMsg) = 0;
	virtual RPCResult CB_AddPartnerGroup(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup, NetGroup* skillGroups) = 0;
	virtual RPCResult CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_AddPartner(LPCPEER pPeer, RPCContext &context, NetObject* partner, NetGroup* skillGroup) = 0;
	virtual RPCResult CB_UpdateAllPartner(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup) = 0;
	virtual RPCResult CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp) = 0;
	virtual RPCResult CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex) = 0;
	virtual RPCResult CB_PartnerCultivationUp(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt8 &msgValue, const UInt32 &src_partnerID) = 0;
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
	INLINE PartnerGroup_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult PartnerGroup_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1535271379>(pPeer, pBuf);
}

INLINE RPCResult PartnerGroup_Proxy::DeletePartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_DeletePartner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(partner_id);
	return RPCSend<377357034>(pPeer, pBuf);
}

INLINE RPCResult PartnerGroup_Proxy::ChangePartnerState(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id, const UInt8 &state)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_ChangePartnerState);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(partner_id);
	pBuf->Write<UInt8>(state);
	return RPCSend<1793207641>(pPeer, pBuf);
}

INLINE RPCResult PartnerGroup_Proxy::PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_PutToWareHouse);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(partnerID);
	return RPCSend<939066783>(pPeer, pBuf);
}

INLINE RPCResult PartnerGroup_Proxy::GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_GetFromWareHouse);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(partnerID);
	return RPCSend<1552559496>(pPeer, pBuf);
}

INLINE RPCResult PartnerGroup_Proxy::PartnerCultivationUpByPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &src_partnerID, const UInt32 &dest_partnerID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_PartnerCultivationUpByPartner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(src_partnerID);
	pBuf->Write<UInt32>(dest_partnerID);
	return RPCSend<497361897>(pPeer, pBuf);
}

INLINE RPCResult PartnerGroup_Proxy::PartnerCultivationUpByItem(LPCPEER pPeer, RPCContext &context, const UInt32 &src_partnerID, const UInt32 &dest_itemUid)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_PartnerCultivationUpByItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(src_partnerID);
	pBuf->Write<UInt32>(dest_itemUid);
	return RPCSend<540747360>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(852316315)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1535271379)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(377357034)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1793207641)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(939066783)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1552559496)
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(497361897)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(540747360)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1305766728)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1607038446)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(251283557)
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(362050677)
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1209149290)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1252151694)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1840984601)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(677792347)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_PARTNERGROUP_PROXY_H_
