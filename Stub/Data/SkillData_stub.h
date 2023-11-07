///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SKILLDATA_STUB_H_
#define _RPC_SKILLDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/SkillData_enum.h"

class SkillData_Stub : public NetObject
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
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	const UInt16& GetSkill_id() const { return m.skill_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const BYTE& GetOwner_type() const { return m.owner_type; }
	const UInt16& GetSkill_level() const { return m.skill_level; }
	const UInt32& GetCool_down() const { return m.cool_down; }
	void SetInfo(const UInt16 &skill_id);
	void SetSkill_id(const UInt16 &skill_id);
	void SetOwner_info(const UInt32 &owner_id, const BYTE &owner_type);
	void SetOwner_id(const UInt32 &owner_id);
	void SetOwner_type(const BYTE &owner_type);
	void SetLevel(const UInt16 &skill_level);
	void SetSkill_level(const UInt16 &skill_level);
	void SetCoolDown(const UInt32 &cool_down);
	void SetCool_down(const UInt32 &cool_down);
	void Clone(SkillData_Stub* obj, const RPCMask &mask);
	friend void SkillData_Stub::Clone(SkillData_Stub* obj, const RPCMask &mask);
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
} m;

public:
	INLINE SkillData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_SKILLDATA_STUB_H_
