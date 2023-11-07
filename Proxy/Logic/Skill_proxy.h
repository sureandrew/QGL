///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SKILL_PROXY_H_
#define _RPC_SKILL_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Skill_enum.h"

class Skill_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo(const UInt16 &skill_id);
	virtual void OnUpdateOwner_info(const UInt32 &owner_id, const BYTE &owner_type);
	virtual void OnUpdateLevel(const UInt16 &skill_level);
	virtual void OnUpdateCoolDown(const UInt32 &cool_down);
	virtual void OnUpdatePartnerSkillType(const BYTE &partner_skill_type);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		owner_infoMask	= Parent::NextFreeMask + 1,
		LevelMask	= Parent::NextFreeMask + 2,
		CoolDownMask	= Parent::NextFreeMask + 3,
		PartnerSkillTypeMask	= Parent::NextFreeMask + 4,
		NextFreeMask	= Parent::NextFreeMask + 5
	};
	const UInt16& GetSkill_id() const { return m.skill_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const BYTE& GetOwner_type() const { return m.owner_type; }
	const UInt16& GetSkill_level() const { return m.skill_level; }
	const UInt32& GetCool_down() const { return m.cool_down; }
	const BYTE& GetPartner_skill_type() const { return m.partner_skill_type; }
	virtual void OnUpdateInfo(const UInt16 &skill_id) {};
	virtual void OnUpdateOwner_info(const UInt32 &owner_id, const BYTE &owner_type) {};
	virtual void OnUpdateLevel(const UInt16 &skill_level) {};
	virtual void OnUpdateCoolDown(const UInt32 &cool_down) {};
	virtual void OnUpdatePartnerSkillType(const BYTE &partner_skill_type) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//info
	UInt16 skill_id;
	//owner_info
	UInt32 owner_id;
	BYTE owner_type;
	//Level
	UInt16 skill_level;
	//CoolDown
	UInt32 cool_down;
	//PartnerSkillType
	BYTE partner_skill_type;
} m;

public:
	INLINE Skill_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult Skill_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Skill_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<860775349>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(923693173)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(860775349)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_SKILL_PROXY_H_
