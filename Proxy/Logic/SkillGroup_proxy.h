///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SKILLGROUP_PROXY_H_
#define _RPC_SKILLGROUP_PROXY_H_

#include "Common/RPCFunc.h"
#include "Reuben/Network/NetGroup.h"
#include "RPCEnum/SkillGroup_enum.h"

class SkillGroup_Proxy : public NetGroup
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp);
	virtual RPCResult CB_RemoveSkills(LPCPEER pPeer, RPCContext &context, const UInt16Vector &skill_ids);
	// OnUpdate can be implemented selectively
	virtual void OnUpdateOwner(const UInt32 &owner_id, const BYTE &owner_type);
***/

protected:
	typedef NetGroup Parent;

public:
	enum UpdateMask {
		ownerMask	= Parent::NextFreeMask + 0,
		NextFreeMask	= Parent::NextFreeMask + 1
	};
	virtual RPCResult CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp) = 0;
	virtual RPCResult CB_RemoveSkills(LPCPEER pPeer, RPCContext &context, const UInt16Vector &skill_ids) = 0;
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const BYTE& GetOwner_type() const { return m.owner_type; }
	virtual void OnUpdateOwner(const UInt32 &owner_id, const BYTE &owner_type) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//owner
	UInt32 owner_id;
	BYTE owner_type;
} m;

public:
	INLINE SkillGroup_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult SkillGroup_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SkillGroup_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<301226259>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(423504350)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(301226259)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(148256391)
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(393032429)
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_SKILLGROUP_PROXY_H_
