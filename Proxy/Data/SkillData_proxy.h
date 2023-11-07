///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SKILLDATA_PROXY_H_
#define _RPC_SKILLDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/SkillData_enum.h"

class SkillData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo(const UInt16 &skill_id);
	virtual void OnUpdateOwner_info(const UInt32 &owner_id, const BYTE &owner_type);
	virtual void OnUpdateLevel(const UInt16 &skill_level);
	virtual void OnUpdateCoolDown(const UInt32 &cool_down);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		owner_infoMask	= Parent::NextFreeMask + 1,
		LevelMask	= Parent::NextFreeMask + 2,
		CoolDownMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	const UInt16& GetSkill_id() const { return m.skill_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const BYTE& GetOwner_type() const { return m.owner_type; }
	const UInt16& GetSkill_level() const { return m.skill_level; }
	const UInt32& GetCool_down() const { return m.cool_down; }
	virtual void OnUpdateInfo(const UInt16 &skill_id) {};
	virtual void OnUpdateOwner_info(const UInt32 &owner_id, const BYTE &owner_type) {};
	virtual void OnUpdateLevel(const UInt16 &skill_level) {};
	virtual void OnUpdateCoolDown(const UInt32 &cool_down) {};
	void SetInfo(const UInt16 &skill_id);
	void SetSkill_id(const UInt16 &skill_id);
	void SetOwner_info(const UInt32 &owner_id, const BYTE &owner_type);
	void SetOwner_id(const UInt32 &owner_id);
	void SetOwner_type(const BYTE &owner_type);
	void SetLevel(const UInt16 &skill_level);
	void SetSkill_level(const UInt16 &skill_level);
	void SetCoolDown(const UInt32 &cool_down);
	void SetCool_down(const UInt32 &cool_down);
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
} m;

public:
	INLINE SkillData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult SkillData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SkillData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1052715992>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(950277788)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1052715992)
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_SKILLDATA_PROXY_H_
