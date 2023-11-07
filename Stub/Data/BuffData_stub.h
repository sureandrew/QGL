///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_BUFFDATA_STUB_H_
#define _RPC_BUFFDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/BuffData_enum.h"

class BuffData_Stub : public NetObject
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
		ValueMask	= Parent::NextFreeMask + 2,
		CoolDownMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	const UInt16& GetBuff_id() const { return m.buff_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const BYTE& GetOwner_type() const { return m.owner_type; }
	const UInt16& GetBuff_value() const { return m.buff_value; }
	const UInt32& GetCool_down() const { return m.cool_down; }
	void SetInfo(const UInt16 &buff_id);
	void SetBuff_id(const UInt16 &buff_id);
	void SetOwner_info(const UInt32 &owner_id, const BYTE &owner_type);
	void SetOwner_id(const UInt32 &owner_id);
	void SetOwner_type(const BYTE &owner_type);
	void SetValue(const UInt16 &buff_value);
	void SetBuff_value(const UInt16 &buff_value);
	void SetCoolDown(const UInt32 &cool_down);
	void SetCool_down(const UInt32 &cool_down);
	void Clone(BuffData_Stub* obj, const RPCMask &mask);
	friend void BuffData_Stub::Clone(BuffData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//info
	UInt16 buff_id;
	//owner_info
	UInt32 owner_id;
	BYTE owner_type;
	//Value
	UInt16 buff_value;
	//CoolDown
	UInt32 cool_down;
} m;

public:
	INLINE BuffData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_BUFFDATA_STUB_H_
