///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SKILL_STUB_H_
#define _RPC_SKILL_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Skill_enum.h"

class Skill_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	RPCMask m_modifyMask;
	INLINE Boolean IsModified() { return (!m_modifyMask.IsZero()); }
	INLINE Boolean IsModified(UInt bit) { return (m_modifyMask.IsBit(bit)); }
	INLINE Boolean IsModified(CONST RPCMask& mask) { return (m_modifyMask.IsMask(mask)); }
	INLINE VOID SetModifyMask(CONST RPCMask& mask) { m_modifyMask = mask; }
	INLINE CONST RPCMask& GetModifyMask() CONST { return m_modifyMask; }
	INLINE VOID ClearModifyMask() { m_modifyMask.Clear(); }
	INLINE VOID MarkModify(UInt bit) { m_modifyMask.MaskBit(bit); }
	INLINE VOID MarkModify(CONST RPCMask& mask) { m_modifyMask.Mask(mask); }
	INLINE VOID UnmarkModify(UInt bit) { m_modifyMask.UnmaskBit(bit); }
	INLINE VOID UnmarkModify(CONST RPCMask& mask) { m_modifyMask.Unmask(mask); }
	INLINE virtual VOID ClearAllMask() {
		Parent::ClearAllMask();
		m_modifyMask.Clear();
	}
	INLINE virtual VOID MarkUpdateAll(UInt bit, bool modified) {
		Parent::MarkUpdateAll(bit, modified);
		if (modified) m_modifyMask.MaskBit(bit);
	}
	void SetInfo(const UInt16 &skill_id);
	void SetSkill_id(const UInt16 &skill_id);
	void SetOwner_info(const UInt32 &owner_id, const BYTE &owner_type);
	void SetOwner_id(const UInt32 &owner_id);
	void SetOwner_type(const BYTE &owner_type);
	void SetLevel(const UInt16 &skill_level);
	void SetSkill_level(const UInt16 &skill_level);
	void SetCoolDown(const UInt32 &cool_down);
	void SetCool_down(const UInt32 &cool_down);
	void SetPartnerSkillType(const BYTE &partner_skill_type);
	void SetPartner_skill_type(const BYTE &partner_skill_type);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE Skill_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_SKILL_STUB_H_
