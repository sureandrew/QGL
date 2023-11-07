///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_QUESTNPCFLAGDATA_STUB_H_
#define _RPC_QUESTNPCFLAGDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/QuestNpcFlagData_enum.h"

class QuestNpcFlagData_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		flagMask	= Parent::NextFreeMask + 1,
		NextFreeMask	= Parent::NextFreeMask + 2
	};
	const UInt32& GetNpc_id() const { return m.npc_id; }
	const UInt16& GetLine_id() const { return m.line_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const UInt16& GetFlag_id() const { return m.flag_id; }
	const Int32& GetFlag() const { return m.flag; }
	void SetId(const UInt32 &npc_id, const UInt16 &line_id, const UInt32 &owner_id, const UInt16 &flag_id);
	void SetNpc_id(const UInt32 &npc_id);
	void SetLine_id(const UInt16 &line_id);
	void SetOwner_id(const UInt32 &owner_id);
	void SetFlag_id(const UInt16 &flag_id);
	void SetFlag(const Int32 &flag);
	void Clone(QuestNpcFlagData_Stub* obj, const RPCMask &mask);
	friend void QuestNpcFlagData_Stub::Clone(QuestNpcFlagData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 npc_id;
	UInt16 line_id;
	UInt32 owner_id;
	UInt16 flag_id;
	//flag
	Int32 flag;
} m;

public:
	INLINE QuestNpcFlagData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_QUESTNPCFLAGDATA_STUB_H_
