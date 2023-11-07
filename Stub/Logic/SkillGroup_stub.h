///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SKILLGROUP_STUB_H_
#define _RPC_SKILLGROUP_STUB_H_

#include "Common/RPCFunc.h"
#include "Reuben/Network/NetGroup.h"
#include "RPCEnum/SkillGroup_enum.h"

class SkillGroup_Stub : public NetGroup
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetGroup Parent;

public:
	enum UpdateMask {
		ownerMask	= Parent::NextFreeMask + 0,
		NextFreeMask	= Parent::NextFreeMask + 1
	};
	virtual RPCResult CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp);
	virtual RPCResult CB_RemoveSkills(LPCPEER pPeer, RPCContext &context, const UInt16Vector &skill_ids);
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const BYTE& GetOwner_type() const { return m.owner_type; }
	void SetOwner(const UInt32 &owner_id, const BYTE &owner_type);
	void SetOwner_id(const UInt32 &owner_id);
	void SetOwner_type(const BYTE &owner_type);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//owner
	UInt32 owner_id;
	BYTE owner_type;
} m;

public:
	INLINE SkillGroup_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_SKILLGROUP_STUB_H_
